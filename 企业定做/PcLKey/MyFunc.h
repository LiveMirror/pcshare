// MyFunc.h: interface for the CMyFunc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYFUNC_H__193B5ABB_5499_4A04_BBCF_12AD0B1494D8__INCLUDED_)
#define AFX_MYFUNC_H__193B5ABB_5499_4A04_BBCF_12AD0B1494D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <process.h>
#include <winioctl.h>
#include <shellapi.h>
#include <Winsvc.h>
#include <psapi.h>
#include <Shlwapi.h>
#include <gdiplus.h>
#include <Tlhelp32.h>
#include <objbase.h>
#include <atlbase.h>
#include <streams.h>
#include <qedit.h>
#include <Vfw.h>
#include <wininet.h>
#include <Userenv.h>
#include <WtsApi32.h>
#include <winternl.h>
#include <winsock2.h>


//Shell32.dll
typedef BOOL (WINAPI* MyShellExecuteExW) (LPSHELLEXECUTEINFOW lpExecInfo);
typedef HINSTANCE (WINAPI* MyShellExecuteA)(HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd);

//Kernel32.dll
typedef HANDLE (WINAPI* MyCreateFileA)(  IN LPCSTR lpFileName,    IN DWORD dwDesiredAccess,    IN DWORD dwShareMode,    IN LPSECURITY_ATTRIBUTES lpSecurityAttributes,    IN DWORD dwCreationDisposition,    IN DWORD dwFlagsAndAttributes,    IN HANDLE hTemplateFile    );
typedef BOOL (WINAPI* MyWriteFile)(    IN HANDLE hFile,    IN LPCVOID lpBuffer,    IN DWORD nNumberOfBytesToWrite,    OUT LPDWORD lpNumberOfBytesWritten,    IN LPOVERLAPPED lpOverlapped    );
typedef BOOL (WINAPI* MyReadFile)(    IN HANDLE hFile,    OUT LPVOID lpBuffer,    IN DWORD nNumberOfBytesToRead,    OUT LPDWORD lpNumberOfBytesRead,    IN LPOVERLAPPED lpOverlapped    );
typedef DWORD (WINAPI* MySetFilePointer)(    IN HANDLE hFile,    IN LONG lDistanceToMove,    IN PLONG lpDistanceToMoveHigh,    IN DWORD dwMoveMethod    );
typedef BOOL (WINAPI* MyCloseHandle)( IN OUT HANDLE hObject );
typedef BOOL (WINAPI* MyGetVersionExA)(IN OUT LPOSVERSIONINFOA lpVersionInformation    );
typedef DWORD (WINAPI* MyGetVersion)( VOID );
typedef DWORD (WINAPI* MyGetCurrentThreadId)(   VOID    );
typedef DWORD (WINAPI* MyGetTempPathA)(  IN DWORD nBufferLength,   OUT LPSTR lpBuffer    );
typedef DWORD (WINAPI* MyGetFileSize)(    IN HANDLE hFile,    OUT LPDWORD lpFileSizeHigh    );
typedef VOID (WINAPI* MySleep)(  IN DWORD dwMilliseconds    );
typedef int (WINAPI* MyMultiByteToWideChar)(    IN UINT     CodePage,    IN DWORD    dwFlags,    IN LPCSTR   lpMultiByteStr,    IN int      cbMultiByte,    OUT LPWSTR  lpWideCharStr,    IN int      cchWideChar);
typedef int (WINAPI* MyWideCharToMultiByte)(    IN UINT     CodePage,    IN DWORD    dwFlags,    IN LPCWSTR  lpWideCharStr,    IN int      cchWideChar,    OUT LPSTR   lpMultiByteStr,    IN int      cbMultiByte,    IN LPCSTR   lpDefaultChar,    OUT LPBOOL  lpUsedDefaultChar);
typedef HMODULE (WINAPI* MyGetModuleHandleA)(    IN LPCSTR lpModuleName    );
typedef HANDLE (WINAPI* MyOpenProcess)(    IN DWORD dwDesiredAccess,    IN BOOL bInheritHandle,    IN DWORD dwProcessId    );
typedef HANDLE (WINAPI* MyGetCurrentProcess)(   VOID    );
typedef UINT (WINAPI* MyGetSystemDirectoryA)(    OUT LPSTR lpBuffer,    IN UINT uSize    );
typedef HANDLE (WINAPI* MyCreateThread)(    IN LPSECURITY_ATTRIBUTES lpThreadAttributes,    IN SIZE_T dwStackSize,    IN LPTHREAD_START_ROUTINE lpStartAddress,    IN LPVOID lpParameter,    IN DWORD dwCreationFlags,    OUT LPDWORD lpThreadId    );
typedef LPWSTR (WINAPI* MylstrcpyW)(    OUT LPWSTR lpString1,    IN LPCWSTR lpString2    );
typedef int (WINAPI* MylstrlenW)(    IN LPCWSTR lpString    );
typedef int (WINAPI* MylstrcmpW)(    IN LPCWSTR lpString1,    IN LPCWSTR lpString2    );
typedef LPWSTR (WINAPI* MylstrcatW)(    IN OUT LPWSTR lpString1,     IN LPCWSTR lpString2     );
typedef DWORD (WINAPI* MyGetTickCount)(    VOID    );
typedef HANDLE (WINAPI* MyCreateMutexA)(    IN LPSECURITY_ATTRIBUTES lpMutexAttributes,    IN BOOL bInitialOwner,    IN LPCSTR lpName    );
typedef HANDLE (WINAPI* MyOpenMutexA)(    IN DWORD dwDesiredAccess,    IN BOOL bInheritHandle,    IN LPCSTR lpName    );
typedef BOOL (WINAPI* MyReleaseMutex)(    IN HANDLE hMutex    );
typedef HANDLE (WINAPI* MyCreateEventA)(    IN LPSECURITY_ATTRIBUTES lpEventAttributes,    IN BOOL bManualReset,    IN BOOL bInitialState,    IN LPCSTR lpName    );
typedef HANDLE (WINAPI* MyOpenEventA)(    IN DWORD dwDesiredAccess,    IN BOOL bInheritHandle,    IN LPCSTR lpName    );
typedef BOOL (WINAPI* MySetEvent)(    IN HANDLE hEvent    );
typedef DWORD (WINAPI* MyWaitForSingleObject)(    IN HANDLE hHandle,    IN DWORD dwMilliseconds    );
typedef VOID (WINAPI* MyGetLocalTime)(    OUT LPSYSTEMTIME lpSystemTime    );
typedef DWORD (WINAPI* MyGetModuleFileNameA)(    IN HMODULE hModule,    OUT LPSTR lpFilename,    IN DWORD nSize    );
typedef BOOL (WINAPI* MyDeleteFileA)(    IN LPCSTR lpFileName    );


//User32.dll
typedef BOOL (WINAPI* MyPostThreadMessageA)(   IN DWORD idThread,    IN UINT Msg,    IN WPARAM wParam,    IN LPARAM lParam);
typedef BOOL (WINAPI* MyPostMessageA)(    IN HWND hWnd,    IN UINT Msg,    IN WPARAM wParam,    IN LPARAM lParam);
typedef LPSTR (WINAPI* MyCharLowerA)(    IN OUT LPSTR lpsz);
typedef BOOL (WINAPI* MyGetMessageA)(    OUT LPMSG lpMsg,    IN HWND hWnd,    IN UINT wMsgFilterMin,    IN UINT wMsgFilterMax);
typedef int (WINAPI* MywsprintfW)(    OUT LPWSTR,     IN LPCWSTR,  ...);
typedef HWND (WINAPI* MyGetForegroundWindow)(VOID);
typedef int (WINAPI* MyGetWindowTextA)(    IN HWND hWnd,    OUT LPSTR lpString,    IN int nMaxCount);
typedef LRESULT (WINAPI* MyCallNextHookEx)(    IN HHOOK hhk,    IN int nCode,    IN WPARAM wParam,    IN LPARAM lParam);
typedef DWORD (WINAPI* MyGetWindowThreadProcessId)(IN HWND hWnd, OUT  LPDWORD lpdwProcessId);
typedef HWND (WINAPI* MyGetActiveWindow)(    VOID);
typedef BOOL (WINAPI* MyUnhookWindowsHookEx)(    IN HHOOK hhk);
typedef HHOOK (WINAPI* MySetWindowsHookExW)(    IN int idHook,    IN HOOKPROC lpfn,    IN HINSTANCE hmod,    IN DWORD dwThreadId);
typedef BOOL (WINAPI* MySetThreadDesktop)(    IN HDESK hDesktop);
typedef HDESK (WINAPI* MyOpenDesktopA)(    IN LPCSTR lpszDesktop,    IN DWORD dwFlags,    IN BOOL fInherit,    IN ACCESS_MASK dwDesiredAccess);
typedef HDESK (WINAPI* MyGetThreadDesktop)( IN DWORD dwThreadId);

//Psapi.dll
typedef DWORD (WINAPI* MyGetModuleFileNameExA)(HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize);
typedef BOOL (WINAPI* MyEnumProcesses)(DWORD * lpidProcess, DWORD cb, DWORD* cbNeeded);

//MSVCRT.dll
typedef int (* Mysprintf)(char *, const char *, ...);
typedef char *  (* Mystrstr)(const char *, const char *);
typedef char *  (* Mystrchr)(const char *, int);
typedef void   (* Mysrand)(unsigned int);
typedef int    (* Myrand)(void);
typedef char * (* Mystrrchr)(const char *, int);
typedef size_t (* Mystrlen)(const char *);
typedef void *  (* Mymemcpy)(void *, const void *, size_t);
typedef int     (* Mymemcmp)(const void *, const void *, size_t);
typedef void *  (* Mymemset)(void *, int, size_t);
typedef char *  (* Mystrcpy)(char *, const char *);
typedef char *  (* Mystrcat)(char *, const char *);
typedef int     (* Mystrcmp)(const char *, const char *);

//ole32.dll
typedef HRESULT (WINAPI* MyCoCreateGuid)(OUT GUID FAR *pguid);

//ADVAPI32.dll
typedef LONG (WINAPI* MyRegCloseKey)(    IN HKEY hKey    );
typedef LONG (WINAPI* MyRegCreateKeyExA)(    IN HKEY hKey,    IN LPCSTR lpSubKey,    IN DWORD Reserved,    IN LPSTR lpClass,    IN DWORD dwOptions,    IN REGSAM samDesired,    IN LPSECURITY_ATTRIBUTES lpSecurityAttributes,    OUT PHKEY phkResult,    OUT LPDWORD lpdwDisposition    );
typedef LONG (WINAPI* MyRegSetValueExA) (    IN HKEY hKey,    IN LPCSTR lpValueName,    IN DWORD Reserved,    IN DWORD dwType,    IN CONST BYTE* lpData,    IN DWORD cbData    );
typedef BOOL (WINAPI* MyAdjustTokenPrivileges)(    IN HANDLE TokenHandle,    IN BOOL DisableAllPrivileges,    IN PTOKEN_PRIVILEGES NewState,    IN DWORD BufferLength,    OUT PTOKEN_PRIVILEGES PreviousState,    OUT PDWORD ReturnLength    );
typedef BOOL (WINAPI* MyLookupPrivilegeValueA)(    IN LPCSTR lpSystemName,    IN LPCSTR lpName,    OUT PLUID   lpLuid    );
typedef BOOL (WINAPI* MyOpenProcessToken) (    IN HANDLE ProcessHandle,    IN DWORD DesiredAccess,    OUT PHANDLE TokenHandle    );
typedef BOOL (WINAPI* MyGetUserNameW)(    OUT LPWSTR lpBuffer,    IN OUT LPDWORD nSize    );

//IMM32.dll
typedef BOOL (WINAPI* MyImmReleaseContext)(IN HWND, IN HIMC);
typedef LONG (WINAPI* MyImmGetCompositionStringW)(IN HIMC, IN DWORD, OUT LPVOID, IN DWORD);
typedef LONG (WINAPI* MyImmGetCompositionStringA)(IN HIMC, IN DWORD, OUT LPVOID, IN DWORD);
typedef HIMC (WINAPI* MyImmGetContext)(IN HWND);

class CMyFunc  
{
public:

	//DLL句柄
	HMODULE hModule_Kernel32;				//Kernel32.dll
	HMODULE hModule_Psapi;					//Psapi.dll
	HMODULE hModule_MSVCRT;					//MSVCRT.dll
	HMODULE hModule_User32;					//User32.dll
	HMODULE hModule_ADVAPI32;				//ADVAPI32.dll
	HMODULE hModule_IMM32;					//IMM32.dll

	//IMM32.dll
	MyImmReleaseContext				ImmReleaseContext;
	MyImmGetCompositionStringW		ImmGetCompositionStringW;
	MyImmGetCompositionStringA		ImmGetCompositionString;
	MyImmGetContext					ImmGetContext;

	//MSVCRT.dll
	Mysprintf						sprintf;
	Mystrstr						strstr;
	Mystrrchr						strrchr;
	Mystrlen						strlen;
	Mymemcpy						memcpy;
	Mymemset						memset;
	Mystrcpy						strcpy;
	Mystrcat						strcat;

	//User32.dll
	MyCharLowerA					CharLower;
	MywsprintfW						wsprintfW;
	MyCallNextHookEx				CallNextHookEx;
	MyGetForegroundWindow			GetForegroundWindow;
	MyGetWindowTextA				GetWindowText;
	MyGetWindowThreadProcessId		GetWindowThreadProcessId;
	MyGetActiveWindow				GetActiveWindow;
	MyUnhookWindowsHookEx			UnhookWindowsHookEx;
	MySetWindowsHookExW				SetWindowsHookExW;
	MySetThreadDesktop				SetThreadDesktop;
	MyOpenDesktopA					OpenDesktop;
	MyGetThreadDesktop				GetThreadDesktop;

	//Kernel32.dll
	MyCreateFileA					CreateFile;
	MyWriteFile						WriteFile;
	MyDeleteFileA					DeleteFile;
	MySetFilePointer				SetFilePointer;
	MyCloseHandle					CloseHandle;
	MyGetModuleFileNameA			GetModuleFileName;
	MyGetCurrentThreadId			GetCurrentThreadId;
	MyMultiByteToWideChar			MultiByteToWideChar;
	MyWideCharToMultiByte			WideCharToMultiByte;
	MyOpenProcess					OpenProcess;
	MylstrcpyW						lstrcpyW;
	MylstrlenW						lstrlenW;
	MylstrcmpW						lstrcmpW;
	MylstrcatW						lstrcatW;
	MyGetTickCount					GetTickCount;
	MyCreateMutexA					CreateMutex;
	MyOpenMutexA					OpenMutex;
	MyReleaseMutex					ReleaseMutex;
	MyCreateEventA					CreateEvent;
	MyOpenEventA					OpenEvent;
	MySetEvent						SetEvent;
	MyWaitForSingleObject			WaitForSingleObject;
	MyGetLocalTime					GetLocalTime;

	//Psapi.dll
	MyGetModuleFileNameExA			GetModuleFileNameEx;

	//ADVAPI32.dll
	MyGetUserNameW					GetUserNameW;

	//全局缓冲变量
	CMyFunc();
	~CMyFunc();

	BOOL LoadMyData(BYTE* pModuel);
	void GetMyString(DWORD nIndex, char* pOutString, char* pInString, DWORD nInStringLen);

	//全局缓冲变量
	char*	pModuleString;
	DWORD	nModuleStringLen;

private:
	void  DoMyMemCpy(char* pSrc, char* pDes, int len);
	BOOL DoMyMemCmp(char* pSrc, char* pDes, int len);
	int GetMySprintf(int nValue, char* pOutStr);
};

extern CMyFunc	 m_gFunc;
extern UINT		 m_Ext_A[3];
extern HINSTANCE m_ghInstance;
extern PSDLLINFO m_DllInfo;

#endif // !defined(AFX_MYFUNC_H__193B5ABB_5499_4A04_BBCF_12AD0B1494D8__INCLUDED_)
