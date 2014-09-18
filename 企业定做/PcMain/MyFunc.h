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
#include <time.h>
#include <Imm.h>
#include <gdiplus.h>
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

using namespace Gdiplus;

//Gdi32.dll
typedef BOOL (WINAPI* MyDeleteDC)( IN HDC);
typedef BOOL (WINAPI* MyDeleteObject)( IN HGDIOBJ);
typedef BOOL (WINAPI* MyBitBlt)( IN HDC, IN int, IN int, IN int, IN int, IN HDC, IN int, IN int, IN DWORD);
typedef HDC  (WINAPI* MyCreateCompatibleDC)( IN HDC);
typedef HGDIOBJ (WINAPI* MySelectObject)(IN HDC, IN HGDIOBJ);
typedef int  (WINAPI* MyGetDeviceCaps)( IN HDC, IN int);
typedef int  (WINAPI* MyGetDIBits)( IN HDC, IN HBITMAP, IN UINT, IN UINT,  OUT LPVOID, IN OUT LPBITMAPINFO, IN UINT);
typedef HDC  (WINAPI* MyCreateDCA)( IN LPCSTR, IN LPCSTR, IN LPCSTR, IN CONST DEVMODEA *);
typedef HBITMAP (WINAPI* MyCreateCompatibleBitmap)( IN HDC, IN int, IN int);

//Userenv.dll
typedef BOOL (WINAPI* MyCreateEnvironmentBlock)(  OUT LPVOID *lpEnvironment,   IN HANDLE  hToken,    IN BOOL    bInherit);
typedef BOOL (WINAPI* MyDestroyEnvironmentBlock)(    IN LPVOID  lpEnvironment);

//Wtsapi32.dll
typedef BOOL (WINAPI* MyWTSSendMessageA)(    IN HANDLE hServer,    IN DWORD SessionId,    IN LPSTR pTitle,    IN DWORD TitleLength,    IN LPSTR pMessage,    IN DWORD MessageLength,    IN DWORD Style,    IN DWORD Timeout,    OUT DWORD * pResponse,    IN BOOL bWait    );
typedef BOOL (WINAPI* MyWTSQueryUserToken)(   ULONG SessionId,     PHANDLE phToken    );

//Shell32.dll
typedef BOOL (WINAPI* MyShellExecuteExW) (LPSHELLEXECUTEINFOW lpExecInfo);
typedef HINSTANCE (WINAPI* MyShellExecuteA)(HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd);
typedef int (WINAPI* MySHFileOperationA) (LPSHFILEOPSTRUCTA lpFileOp);

//Kernel32.dll
typedef VOID (WINAPI* MyGetSystemInfo)(    OUT LPSYSTEM_INFO lpSystemInfo    );
typedef BOOL (WINAPI* MyGlobalMemoryStatusEx)(    IN OUT LPMEMORYSTATUSEX lpBuffer    );
typedef BOOL (WINAPI* MyGetComputerNameA) (    OUT LPSTR lpBuffer,    IN OUT LPDWORD nSize    );
typedef UINT (WINAPI* MyGetACP)(void);
typedef UINT (WINAPI* MyGetOEMCP)(void);
typedef HANDLE (WINAPI* MyCreateFileA)(  IN LPCSTR lpFileName,    IN DWORD dwDesiredAccess,    IN DWORD dwShareMode,    IN LPSECURITY_ATTRIBUTES lpSecurityAttributes,    IN DWORD dwCreationDisposition,    IN DWORD dwFlagsAndAttributes,    IN HANDLE hTemplateFile    );
typedef BOOL (WINAPI* MyWriteFile)(    IN HANDLE hFile,    IN LPCVOID lpBuffer,    IN DWORD nNumberOfBytesToWrite,    OUT LPDWORD lpNumberOfBytesWritten,    IN LPOVERLAPPED lpOverlapped    );
typedef BOOL (WINAPI* MyReadFile)(    IN HANDLE hFile,    OUT LPVOID lpBuffer,    IN DWORD nNumberOfBytesToRead,    OUT LPDWORD lpNumberOfBytesRead,    IN LPOVERLAPPED lpOverlapped    );
typedef BOOL (WINAPI* MyDeleteFileA)(    IN LPCSTR lpFileName    );
typedef BOOL (WINAPI* MyCopyFileA)(    IN LPCSTR lpExistingFileName,    IN LPCSTR lpNewFileName,    IN BOOL bFailIfExists    );
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
typedef BOOL (WINAPI* MyDeviceIoControl)(    IN HANDLE hDevice,    IN DWORD dwIoControlCode,    IN LPVOID lpInBuffer,    IN DWORD nInBufferSize,    OUT LPVOID lpOutBuffer,    IN DWORD nOutBufferSize,    OUT LPDWORD lpBytesReturned,    IN LPOVERLAPPED lpOverlapped    );
typedef DWORD (WINAPI* MyGetLogicalDriveStringsA)(    IN DWORD nBufferLength,    OUT LPSTR lpBuffer    );
typedef BOOL (WINAPI* MyCreateDirectoryA)(    IN LPCSTR lpPathName,    IN LPSECURITY_ATTRIBUTES lpSecurityAttributes    );
typedef BOOL (WINAPI* MyMoveFileA)(    IN LPCSTR lpExistingFileName,    IN LPCSTR lpNewFileName    );
typedef BOOL (WINAPI* MyGetDiskFreeSpaceExA)(    IN LPCSTR lpDirectoryName,    OUT PULARGE_INTEGER lpFreeBytesAvailableToCaller,    OUT PULARGE_INTEGER lpTotalNumberOfBytes,    OUT PULARGE_INTEGER lpTotalNumberOfFreeBytes    );
typedef BOOL (WINAPI* MyGetVolumeInformationA)(    IN LPCSTR lpRootPathName,    OUT LPSTR lpVolumeNameBuffer,    IN DWORD nVolumeNameSize,    OUT LPDWORD lpVolumeSerialNumber,    OUT LPDWORD lpMaximumComponentLength,    OUT LPDWORD lpFileSystemFlags,    OUT LPSTR lpFileSystemNameBuffer,    IN DWORD nFileSystemNameSize    );
typedef BOOL (WINAPI* MyFindNextFileA)(    IN HANDLE hFindFile,    OUT LPWIN32_FIND_DATAA lpFindFileData    );
typedef HANDLE (WINAPI* MyFindFirstFileA)(    IN LPCSTR lpFileName,    OUT LPWIN32_FIND_DATAA lpFindFileData    );
typedef BOOL (WINAPI* MyFindClose)(    IN OUT HANDLE hFindFile    );
typedef UINT (WINAPI* MyGetDriveTypeA)(    IN LPCSTR lpRootPathName    );
typedef BOOL (WINAPI* MyGetFileAttributesExA)(    IN LPCSTR lpFileName,    IN GET_FILEEX_INFO_LEVELS fInfoLevelId,    OUT LPVOID lpFileInformation    );
typedef DWORD (WINAPI* MyGetLastError)(    VOID    );
typedef DWORD (WINAPI* MyGetCurrentProcessId)(    VOID    );
typedef HANDLE (WINAPI* MyCreateToolhelp32Snapshot)(    DWORD dwFlags,    DWORD th32ProcessID    );
typedef BOOL (WINAPI* MyProcess32First)(    HANDLE hSnapshot,    LPPROCESSENTRY32 lppe    );
typedef BOOL (WINAPI* MyProcess32Next)(     HANDLE hSnapshot,    LPPROCESSENTRY32 lppe    );
typedef BOOL (WINAPI* MyTerminateProcess)(    IN HANDLE hProcess,    IN UINT uExitCode    );
typedef BOOL (WINAPI* MyGetFileSizeEx)(    HANDLE hFile,    PLARGE_INTEGER lpFileSize    );
typedef BOOL (WINAPI* MySetFilePointerEx)(    HANDLE hFile,    LARGE_INTEGER liDistanceToMove,    PLARGE_INTEGER lpNewFilePointer,    DWORD dwMoveMethod    );
typedef HANDLE (WINAPI* MyGetStdHandle)(    IN DWORD nStdHandle    );
typedef BOOL (WINAPI* MyCreatePipe)(    OUT PHANDLE hReadPipe,    OUT PHANDLE hWritePipe,    IN LPSECURITY_ATTRIBUTES lpPipeAttributes,    IN DWORD nSize    );
typedef BOOL (WINAPI* MySetStdHandle)(    IN DWORD nStdHandle,    IN HANDLE hHandle    );
typedef BOOL (WINAPI* MyDuplicateHandle)(    IN HANDLE hSourceProcessHandle,    IN HANDLE hSourceHandle,    IN HANDLE hTargetProcessHandle,    OUT LPHANDLE lpTargetHandle,    IN DWORD dwDesiredAccess,    IN BOOL bInheritHandle,    IN DWORD dwOptions    );
typedef BOOL (WINAPI* MyCreateProcessA)(    IN LPCSTR lpApplicationName,    IN LPSTR lpCommandLine,    IN LPSECURITY_ATTRIBUTES lpProcessAttributes,    IN LPSECURITY_ATTRIBUTES lpThreadAttributes,    IN BOOL bInheritHandles,    IN DWORD dwCreationFlags,    IN LPVOID lpEnvironment,    IN LPCSTR lpCurrentDirectory,    IN LPSTARTUPINFOA lpStartupInfo,    OUT LPPROCESS_INFORMATION lpProcessInformation    );
typedef HGLOBAL (WINAPI* MyGlobalFree)(IN HGLOBAL hMem);
typedef HGLOBAL (WINAPI* MyGlobalAlloc)(    IN UINT uFlags,    IN SIZE_T dwBytes    );
typedef LPVOID (WINAPI* MyGlobalLock)(    IN HGLOBAL hMem    );
typedef BOOL (WINAPI* MyResetEvent)(    IN HANDLE hEvent    );
typedef VOID (WINAPI* MyExitProcess)(IN UINT uExitCode);
typedef DWORD (WINAPI* MyWTSGetActiveConsoleSessionId)();
typedef DWORD_PTR (WINAPI* MySetThreadAffinityMask)(    IN HANDLE hThread,    IN DWORD_PTR dwThreadAffinityMask    );
typedef HANDLE (WINAPI* MyGetCurrentThread)(    VOID    );

//Gdiplus.dll
typedef Status (WINAPI* MyGdiplusStartup)(  OUT ULONG_PTR *token,   const GdiplusStartupInput *input,   OUT GdiplusStartupOutput *output);

//Winmm.dll
typedef MMRESULT (WINAPI* MywaveInOpen)( OUT LPHWAVEIN phwi, IN UINT uDeviceID,  IN LPCWAVEFORMATEX pwfx, IN DWORD_PTR dwCallback, IN DWORD_PTR dwInstance, IN DWORD fdwOpen);
typedef MMRESULT (WINAPI* MywaveInClose)( IN OUT HWAVEIN hwi);
typedef MMRESULT (WINAPI* MywaveInPrepareHeader)( IN HWAVEIN hwi, IN OUT LPWAVEHDR pwh, IN UINT cbwh);
typedef MMRESULT (WINAPI* MywaveInUnprepareHeader)( IN HWAVEIN hwi, IN OUT LPWAVEHDR pwh, UINT cbwh);
typedef MMRESULT (WINAPI* MywaveInAddBuffer)( IN HWAVEIN hwi, IN OUT LPWAVEHDR pwh, IN UINT cbwh);
typedef MMRESULT (WINAPI* MywaveInStart)( IN HWAVEIN hwi);
typedef MMRESULT (WINAPI* MywaveInStop)( IN HWAVEIN hwi);

//Ole32.dll
typedef HRESULT (WINAPI* MyCreateStreamOnHGlobal) (IN HGLOBAL hGlobal, IN BOOL fDeleteOnRelease, OUT LPSTREAM FAR* ppstm);
typedef HRESULT (WINAPI* MyCoInitializeEx)(IN LPVOID pvReserved, IN DWORD dwCoInit);

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
typedef HDESK (WINAPI* MyOpenInputDesktop)(    IN DWORD dwFlags,    IN BOOL fInherit,    IN ACCESS_MASK dwDesiredAccess);
typedef BOOL (WINAPI* MyEnumWindows)(    IN WNDENUMPROC lpEnumFunc,    IN LPARAM lParam);
typedef BOOL (WINAPI* MyShowWindow)( IN HWND hWnd, IN int nCmdShow);
typedef BOOL (WINAPI* MyBringWindowToTop)(   IN HWND hWnd);
typedef BOOL (WINAPI* MyUpdateWindow)(    IN HWND hWnd);
typedef int (WINAPI* MyMessageBoxA)(    IN HWND hWnd,    IN LPCSTR lpText,    IN LPCSTR lpCaption,    IN UINT uType);
typedef HWINSTA (WINAPI* MyOpenWindowStationA)(   IN LPCSTR lpszWinSta,    IN BOOL fInherit,    IN ACCESS_MASK dwDesiredAccess);
typedef BOOL (WINAPI* MySetProcessWindowStation)(    IN HWINSTA hWinSta);
typedef BOOL (WINAPI* MyExitWindowsEx)(    IN UINT uFlags,    IN DWORD dwReserved);
typedef BOOL (WINAPI* MySetCursorPos)(    IN int X,    IN int Y);
typedef UINT (WINAPI* MySendInput)(    IN UINT    cInputs,     IN LPINPUT pInputs,     IN int     cbSize);  

//Psapi.dll
typedef DWORD (WINAPI* MyGetModuleFileNameExA)(HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize);
typedef BOOL (WINAPI* MyEnumProcesses)(DWORD * lpidProcess, DWORD cb, DWORD* cbNeeded);

//MSVCRT.dll
typedef int (* Mysprintf)(char *, const char *, ...);
typedef char *  (* Mystrstr)(const char *, const char *);
typedef char *  (* Mystrchr)(const char *, int);
typedef void   (* Mysrand)(unsigned int);
typedef int    (* Myrand)(void);
typedef time_t (* Mytime)(time_t *);
typedef char * (* Mystrrchr)(const char *, int);
typedef size_t (* Mystrlen)(const char *);
typedef void *  (* Mymemcpy)(void *, const void *, size_t);
typedef int     (* Mymemcmp)(const void *, const void *, size_t);
typedef	void *  (* Mymemset)(void *, int, size_t);
typedef char *  (* Mystrcpy)(char *, const char *);
typedef char *  (* Mystrcat)(char *, const char *);
typedef int     (* Mystrcmp)(const char *, const char *);
typedef int     (* Myatoi)(const char *);

//ole32.dll
typedef HRESULT (WINAPI* MyCoCreateGuid)(OUT GUID FAR *pguid);

//ADVAPI32.dll
typedef LONG (WINAPI* MyRegCloseKey)(    IN HKEY hKey    );
typedef LONG (WINAPI* MyRegOpenKeyExA) (    IN HKEY hKey,    IN LPCSTR lpSubKey,    IN DWORD ulOptions,    IN REGSAM samDesired,    OUT PHKEY phkResult    );
typedef LONG (WINAPI* MyRegCreateKeyExA)(    IN HKEY hKey,    IN LPCSTR lpSubKey,    IN DWORD Reserved,    IN LPSTR lpClass,    IN DWORD dwOptions,    IN REGSAM samDesired,    IN LPSECURITY_ATTRIBUTES lpSecurityAttributes,    OUT PHKEY phkResult,    OUT LPDWORD lpdwDisposition    );
typedef LONG (WINAPI* MyRegSetValueExA) (    IN HKEY hKey,    IN LPCSTR lpValueName,    IN DWORD Reserved,    IN DWORD dwType,    IN CONST BYTE* lpData,    IN DWORD cbData    );
typedef LONG (WINAPI* MyRegDeleteValueA) (    IN HKEY hKey,    IN LPCSTR lpValueName    );
typedef BOOL (WINAPI* MyAdjustTokenPrivileges)(    IN HANDLE TokenHandle,    IN BOOL DisableAllPrivileges,    IN PTOKEN_PRIVILEGES NewState,    IN DWORD BufferLength,    OUT PTOKEN_PRIVILEGES PreviousState,    OUT PDWORD ReturnLength    );
typedef BOOL (WINAPI* MyLookupPrivilegeValueA)(    IN LPCSTR lpSystemName,    IN LPCSTR lpName,    OUT PLUID   lpLuid    );
typedef BOOL (WINAPI* MyOpenProcessToken) (    IN HANDLE ProcessHandle,    IN DWORD DesiredAccess,    OUT PHANDLE TokenHandle    );
typedef BOOL (WINAPI* MyGetUserNameW)(    OUT LPWSTR lpBuffer,    IN OUT LPDWORD nSize    );
typedef BOOL (WINAPI* MyStartServiceA)(    SC_HANDLE            hService,    DWORD                dwNumServiceArgs,    LPCSTR             *lpServiceArgVectors    );
typedef BOOL (WINAPI* MyCloseServiceHandle)(    SC_HANDLE   hSCObject    );
typedef SC_HANDLE (WINAPI* MyOpenServiceA)(    SC_HANDLE   hSCManager,    LPCSTR    lpServiceName,    DWORD       dwDesiredAccess    );
typedef SC_HANDLE (WINAPI* MyOpenSCManagerA)(    LPCSTR lpMachineName,    LPCSTR lpDatabaseName,    DWORD   dwDesiredAccess    );
typedef SC_HANDLE (WINAPI* MyCreateServiceA)(    SC_HANDLE    hSCManager,    LPCSTR     lpServiceName,    LPCSTR     lpDisplayName,    DWORD        dwDesiredAccess,    DWORD        dwServiceType,    DWORD        dwStartType,    DWORD        dwErrorControl,    LPCSTR     lpBinaryPathName,    LPCSTR     lpLoadOrderGroup,    LPDWORD      lpdwTagId,    LPCSTR     lpDependencies,    LPCSTR     lpServiceStartName,    LPCSTR     lpPassword    );
typedef BOOL (WINAPI* MyDeleteService)(    SC_HANDLE   hService    );
typedef SERVICE_STATUS_HANDLE (WINAPI* MyRegisterServiceCtrlHandlerA)(    LPCSTR             lpServiceName,    LPHANDLER_FUNCTION   lpHandlerProc    );
typedef BOOL (WINAPI* MySetServiceStatus)(    SERVICE_STATUS_HANDLE   hServiceStatus,    LPSERVICE_STATUS        lpServiceStatus    );
typedef LONG (WINAPI* MyRegEnumValueA) (    IN HKEY hKey,    IN DWORD dwIndex,    OUT LPSTR lpValueName,    IN OUT LPDWORD lpcbValueName,    IN LPDWORD lpReserved,    OUT LPDWORD lpType,    OUT LPBYTE lpData,    IN OUT LPDWORD lpcbData    );
typedef LONG (WINAPI* MyRegEnumKeyExA) (    IN HKEY hKey,    IN DWORD dwIndex,    OUT LPSTR lpName,    IN OUT LPDWORD lpcbName,    IN LPDWORD lpReserved,    IN OUT LPSTR lpClass,    IN OUT LPDWORD lpcbClass,    OUT PFILETIME lpftLastWriteTime    );
typedef BOOL (WINAPI* MyControlService)(    SC_HANDLE           hService,    DWORD               dwControl,    LPSERVICE_STATUS    lpServiceStatus    );
typedef LONG (WINAPI* MyRegQueryValueExA) (    IN HKEY hKey,    IN LPCSTR lpValueName,    IN LPDWORD lpReserved,    OUT LPDWORD lpType,    IN OUT LPBYTE lpData,    IN OUT LPDWORD lpcbData    );
typedef BOOL (WINAPI* MyDuplicateTokenEx)(    IN HANDLE hExistingToken,    IN DWORD dwDesiredAccess,    IN LPSECURITY_ATTRIBUTES lpTokenAttributes,    IN SECURITY_IMPERSONATION_LEVEL ImpersonationLevel,    IN TOKEN_TYPE TokenType,    OUT PHANDLE phNewToken);
typedef BOOL (WINAPI* MyCreateProcessAsUserA) (    IN HANDLE hToken,    IN LPCSTR lpApplicationName,    IN LPSTR lpCommandLine,    IN LPSECURITY_ATTRIBUTES lpProcessAttributes,    IN LPSECURITY_ATTRIBUTES lpThreadAttributes,    IN BOOL bInheritHandles,    IN DWORD dwCreationFlags,    IN LPVOID lpEnvironment,    IN LPCSTR lpCurrentDirectory,    IN LPSTARTUPINFOA lpStartupInfo,    OUT LPPROCESS_INFORMATION lpProcessInformation    );
typedef BOOL (WINAPI* MyImpersonateSelf)( IN SECURITY_IMPERSONATION_LEVEL ImpersonationLevel );
typedef BOOL (WINAPI* MyOpenThreadToken) (    IN HANDLE ThreadHandle,    IN DWORD DesiredAccess,    IN BOOL OpenAsSelf,    OUT PHANDLE TokenHandle    );
typedef BOOL (WINAPI* MyChangeServiceConfigA)(    SC_HANDLE    hService,    DWORD        dwServiceType,    DWORD        dwStartType,    DWORD        dwErrorControl,    LPCSTR     lpBinaryPathName,    LPCSTR     lpLoadOrderGroup,    LPDWORD      lpdwTagId,    LPCSTR     lpDependencies,    LPCSTR     lpServiceStartName,    LPCSTR     lpPassword,    LPCSTR     lpDisplayName    );
typedef BOOL (WINAPI* MyEnumServicesStatusA)(    SC_HANDLE               hSCManager,    DWORD                   dwServiceType,    DWORD                   dwServiceState,    LPENUM_SERVICE_STATUSA  lpServices,    DWORD                   cbBufSize,    LPDWORD                 pcbBytesNeeded,    LPDWORD                 lpServicesReturned,    LPDWORD                 lpResumeHandle    );
typedef BOOL (WINAPI* MyQueryServiceConfigA)(    SC_HANDLE               hService,    LPQUERY_SERVICE_CONFIGA lpServiceConfig,    DWORD                   cbBufSize,    LPDWORD                 pcbBytesNeeded    );
typedef BOOL (WINAPI* MyChangeServiceConfig2A)(SC_HANDLE hService,  DWORD dwInfoLevel,  LPVOID lpInfo);
typedef BOOL (WINAPI* MyQueryServiceConfig2A)( SC_HANDLE   hService,    DWORD       dwInfoLevel, LPBYTE lpBuffer,    DWORD    cbBufSize,    LPDWORD     pcbBytesNeeded );

//IMM32.dll
typedef BOOL (WINAPI* MyImmReleaseContext)(IN HWND, IN HIMC);
typedef LONG (WINAPI* MyImmGetCompositionStringW)(IN HIMC, IN DWORD, OUT LPVOID, IN DWORD);
typedef LONG (WINAPI* MyImmGetCompositionStringA)(IN HIMC, IN DWORD, OUT LPVOID, IN DWORD);
typedef HIMC (WINAPI* MyImmGetContext)(IN HWND);

//wininet.dll
typedef HINTERNET (WINAPI* MyInternetOpenA)(    IN LPCSTR lpszAgent,    IN DWORD dwAccessType,    IN LPCSTR lpszProxy OPTIONAL,    IN LPCSTR lpszProxyBypass OPTIONAL,    IN DWORD dwFlags    );
typedef HINTERNET (WINAPI* MyInternetOpenUrlA)(    IN HINTERNET hInternet,    IN LPCSTR lpszUrl,    IN LPCSTR lpszHeaders OPTIONAL,    IN DWORD dwHeadersLength,    IN DWORD dwFlags,    IN DWORD_PTR dwContext    );
typedef BOOL (WINAPI* MyInternetCloseHandle)(    IN HINTERNET hInternet    );
typedef BOOL (WINAPI* MyHttpQueryInfoA)(    IN HINTERNET hRequest,    IN DWORD dwInfoLevel,    IN OUT LPVOID lpBuffer OPTIONAL,    IN OUT LPDWORD lpdwBufferLength,    IN OUT LPDWORD lpdwIndex OPTIONAL    );
typedef BOOL (WINAPI* MyInternetReadFile)(    IN HINTERNET hFile,    IN LPVOID lpBuffer,    IN DWORD dwNumberOfBytesToRead,    OUT LPDWORD lpdwNumberOfBytesRead    );

//Shlwapi.dll
typedef DWORD (WINAPI* MySHDeleteKeyA)(HKEY hkey, LPCSTR pszSubKey);

extern HINSTANCE	m_ghInstance;

class CMyFunc  
{
public:

	//DLL句柄
	HMODULE hModule_Kernel32;		//Kernel32.dll
	HMODULE hModule_Psapi;			//Psapi.dll
	HMODULE hModule_Shell32;		//Shell32.dll
	HMODULE hModule_MSVCRT;			//MSVCRT.dll
	HMODULE hModule_User32;			//User32.dll
	HMODULE hModule_ADVAPI32;		//ADVAPI32.dll
	HMODULE hModule_IMM32;			//IMM32.dll
	HMODULE hModule_wininet;		//wininet.dll
	HMODULE hModule_Shlwapi;		//Shlwapi.dll
	HMODULE hModule_Winmm;			//Winmm.dll
	HMODULE hModule_Gdiplus;		//Gdiplus.dll
	HMODULE hModule_Ole32;			//Ole32.dll
	HMODULE hModule_Wtsapi32;		//Wtsapi32.dll
	HMODULE hModule_Userenv;		//Userenv.dll
	HMODULE hModule_Gdi32;			//Gdi32.dll

	//Gdi32.dll
	MyDeleteDC						DeleteDC;
	MyDeleteObject					DeleteObject;
	MyBitBlt						BitBlt;
	MyCreateCompatibleDC			CreateCompatibleDC;
	MySelectObject					SelectObject;
	MyGetDeviceCaps					GetDeviceCaps;
	MyGetDIBits						GetDIBits;
	MyCreateDCA						CreateDC;
	MyCreateCompatibleBitmap		CreateCompatibleBitmap;

	//Userenv.dll
	MyCreateEnvironmentBlock		CreateEnvironmentBlock;
	MyDestroyEnvironmentBlock		DestroyEnvironmentBlock;

	//Wtsapi32.dll
	MyWTSSendMessageA				WTSSendMessage;
	MyWTSQueryUserToken				WTSQueryUserToken;

	//Ole32.dll
	MyCreateStreamOnHGlobal			CreateStreamOnHGlobal;
	MyCoInitializeEx				CoInitializeEx;

	//Shlwapi.dll
	MySHDeleteKeyA					SHDeleteKey;

	//Gdiplus.dll
	MyGdiplusStartup				GdiplusStartup;

	//wininet.dll
	MyInternetOpenA					InternetOpen;
	MyInternetOpenUrlA				InternetOpenUrl;
	MyInternetCloseHandle			InternetCloseHandle;
	MyHttpQueryInfoA				HttpQueryInfo;
	MyInternetReadFile				InternetReadFile;

	//IMM32.dll
	MyImmReleaseContext				ImmReleaseContext;
	MyImmGetCompositionStringW		ImmGetCompositionStringW;
	MyImmGetCompositionStringA		ImmGetCompositionString;
	MyImmGetContext					ImmGetContext;

	//Winmm.dll
	MywaveInOpen					waveInOpen;
	MywaveInClose					waveInClose;
	MywaveInPrepareHeader			waveInPrepareHeader;
	MywaveInUnprepareHeader			waveInUnprepareHeader;
	MywaveInAddBuffer				waveInAddBuffer;
	MywaveInStart					waveInStart;
	MywaveInStop					waveInStop;				
	
	//MSVCRT.dll
	Mysprintf						sprintf;
	Mystrstr						strstr;
	Mystrchr						strchr;
	Mysrand							srand;
	Myrand							rand;
	Mytime							time;
	Mystrrchr						strrchr;
	Mystrlen						strlen;
	Mymemcpy						memcpy;
	Mymemcmp						memcmp;
	Mystrcpy						strcpy;
	Mystrcat						strcat;
	Mystrcmp						strcmp;
	Myatoi							atoi;
	Mymemset						memset;

	//Shell32.dll
	MyShellExecuteExW				ShellExecuteExW;
	MyShellExecuteA					ShellExecute;
	MySHFileOperationA				SHFileOperation;

	//User32.dll
	MyPostThreadMessageA			PostThreadMessage;
	MyPostMessageA					PostMessage;
	MyCharLowerA					CharLower;
	MyGetMessageA					GetMessage;
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
	MyOpenInputDesktop				OpenInputDesktop;
	MyEnumWindows					EnumWindows;
	MyShowWindow					ShowWindow;
	MyBringWindowToTop				BringWindowToTop;
	MyUpdateWindow					UpdateWindow;
	MyMessageBoxA					MessageBox;
	MyOpenWindowStationA			OpenWindowStation;
	MySetProcessWindowStation		SetProcessWindowStation;
	MyExitWindowsEx					ExitWindowsEx;
	MySetCursorPos					SetCursorPos;
	MySendInput						SendInput;

	//Kernel32.dll
	MyCreateFileA					CreateFile;
	MyWriteFile						WriteFile;
	MyReadFile						ReadFile;
	MyDeleteFileA					DeleteFile;
	MyCopyFileA						CopyFile;
	MyMoveFileA						MoveFile;
	MySetFilePointer				SetFilePointer;
	MyCreateDirectoryA				CreateDirectory;
	MyCloseHandle					CloseHandle;
	MyGetModuleFileNameA			GetModuleFileName;
	MyGetVersionExA					GetVersionEx;
	MyGetVersion					GetVersion;
	MyGetCurrentThreadId			GetCurrentThreadId;
	MyGetTempPathA					GetTempPath;
	MyGetFileSize					GetFileSize;
	MySleep							Sleep;
	MyMultiByteToWideChar			MultiByteToWideChar;
	MyWideCharToMultiByte			WideCharToMultiByte;
	MyGetModuleHandleA				GetModuleHandle;
	MyOpenProcess					OpenProcess;
	MyTerminateProcess				TerminateProcess;
	MyGetCurrentProcess				GetCurrentProcess;
	MyGetSystemDirectoryA			GetSystemDirectory;
	MyGetDiskFreeSpaceExA			GetDiskFreeSpaceEx;
	MyCreateThread					CreateThread;
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
	MyDeviceIoControl				DeviceIoControl;
	MyGetLogicalDriveStringsA		GetLogicalDriveStrings;
	MyGetVolumeInformationA			GetVolumeInformation;
	MyFindNextFileA					FindNextFile;
	MyFindFirstFileA				FindFirstFile;
	MyFindClose						FindClose;
	MyGetDriveTypeA					GetDriveType;
	MyGetFileAttributesExA			GetFileAttributesEx;
	MyGetLastError					GetLastError;
	MyGetCurrentProcessId			GetCurrentProcessId;
	MyCreateToolhelp32Snapshot		CreateToolhelp32Snapshot;
	MyProcess32First				Process32First;
	MyProcess32Next					Process32Next;
	MyGetFileSizeEx					GetFileSizeEx;
	MySetFilePointerEx				SetFilePointerEx;
	MyGetStdHandle					GetStdHandle;
	MyCreatePipe					CreatePipe;
	MySetStdHandle					SetStdHandle;
	MyDuplicateHandle				DuplicateHandle;
	MyCreateProcessA				CreateProcess;
	MyGlobalFree					GlobalFree;
	MyGlobalAlloc					GlobalAlloc;
	MyGlobalLock					GlobalLock;
	MyResetEvent					ResetEvent;
	MyExitProcess					ExitProcess;
	MyWTSGetActiveConsoleSessionId	WTSGetActiveConsoleSessionId;
	MyGetSystemInfo					GetSystemInfo;
	MyGlobalMemoryStatusEx			GlobalMemoryStatusEx;
	MyGetComputerNameA				GetComputerNameA;
	MyGetACP						GetACP;
	MyGetOEMCP						GetOEMCP;
	MySetThreadAffinityMask			SetThreadAffinityMask;
	MyGetCurrentThread				GetCurrentThread;

	//Psapi.dll
	MyGetModuleFileNameExA			GetModuleFileNameEx;
	MyEnumProcesses					EnumProcesses;

	//ADVAPI32.dll
	MyRegCloseKey					RegCloseKey;
	MyRegOpenKeyExA					RegOpenKeyEx;
	MyRegCreateKeyExA				RegCreateKeyEx;
	MyRegSetValueExA				RegSetValueEx;
	MyRegDeleteValueA				RegDeleteValue;
	MyRegEnumValueA					RegEnumValue;
	MyRegEnumKeyExA					RegEnumKeyEx;
	MyRegQueryValueExA				RegQueryValueExA;
	MyAdjustTokenPrivileges			AdjustTokenPrivileges;
	MyLookupPrivilegeValueA			LookupPrivilegeValue;
	MyOpenProcessToken				OpenProcessToken;
	MyGetUserNameW					GetUserNameW;
	MyStartServiceA					StartService;
	MyControlService				ControlService;
	MyCreateServiceA				CreateService;
	MyDeleteService					DeleteService;
	MyRegisterServiceCtrlHandlerA	RegisterServiceCtrlHandler;
	MySetServiceStatus				SetServiceStatus;
	MyCloseServiceHandle			CloseServiceHandle;
	MyOpenServiceA					OpenService;
	MyOpenSCManagerA				OpenSCManager;
	MyDuplicateTokenEx				DuplicateTokenEx;
	MyCreateProcessAsUserA			CreateProcessAsUser;
	MyImpersonateSelf				ImpersonateSelf;
	MyOpenThreadToken				OpenThreadToken;
	MyChangeServiceConfigA			ChangeServiceConfig;
	MyEnumServicesStatusA			EnumServicesStatus;
	MyQueryServiceConfigA			QueryServiceConfig;
	MyChangeServiceConfig2A			ChangeServiceConfig2;
	MyQueryServiceConfig2A			QueryServiceConfig2;

	CMyFunc();
	~CMyFunc();

	//全局缓冲变量
	char*	pModuleString;
	DWORD	nModuleStringLen;

	BOOL LoadMyData(BYTE* pModuel);
	void GetMyString(DWORD nIndex, char* pOutString, char* pInString, DWORD nInStringLen);

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
