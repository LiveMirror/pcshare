// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__926E219E_DA17_493D_9933_9030D9C8CA6E__INCLUDED_)
#define AFX_STDAFX_H__926E219E_DA17_493D_9933_9030D9C8CA6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WINVER 0x0501		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#define _WIN32_WINNT 0x0501		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#define _WIN32_IE 0x0501	//为 IE 5.0 及更新版本改变为适当的值。

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <wininet.h>
#include <process.h>
#include <Shlwapi.h>
#define _XTP_STATICLINK	 0
#include <XTToolkitPro.h>

typedef struct _SAVEFILEINFO_
{
	char	m_FilePath[256];
	DWORD	m_FileLen;
}SAVEFILEINFO, *LPSAVEFILEINFO;

#define PS_START_KEY		0x10001
#define PS_START_NOKEY		0x10002

#define PS_VER_ULONGLONG	0x8877333322887764
#define PS_CMD_ULONGLONG	0x7733845761628835
#define	PS_CUT_FILE_LEN		5 * 1024
#define PS_STARTUP_FLAG		0x2233466156389774			//启动检查

typedef
BOOL
(WINAPI*
MyGetVolumeInformation)(
    IN LPCSTR lpRootPathName,
    OUT LPSTR lpVolumeNameBuffer,
    IN DWORD nVolumeNameSize,
    OUT LPDWORD lpVolumeSerialNumber,
    OUT LPDWORD lpMaximumComponentLength,
    OUT LPDWORD lpFileSystemFlags,
    OUT LPSTR lpFileSystemNameBuffer,
    IN DWORD nFileSystemNameSize
    );

BOOL MakeFilePath(TCHAR* pathName);
DWORD GetMySerialNumber();
BOOL WriteMyFile(LPCTSTR m_FileName, LPVOID pData, DWORD nLen);
BOOL ReadMyFile(LPCTSTR m_FileName, LPVOID pData, DWORD nLen);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__926E219E_DA17_493D_9933_9030D9C8CA6E__INCLUDED_)
