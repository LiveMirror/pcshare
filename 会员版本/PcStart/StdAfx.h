// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__5EDA2D1B_07CD_4776_87B9_A9F959EF3FA2__INCLUDED_)
#define AFX_STDAFX_H__5EDA2D1B_07CD_4776_87B9_A9F959EF3FA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _PCINIT_

#include <windows.h>
#include <shlwapi.h>
#include <time.h>
#include <process.h>
#include <objbase.h>
#include <Psapi.h>
#include <Shellapi.h>
#include <winsock2.h>
#include "..\PcShare\MyMarco.h"

BOOL WriteMyFile(LPCTSTR m_FileName, LPVOID pData, DWORD nLen);
BOOL LoadInitInfo(LPPSDLLINFO pInfo, char* m_DllFilePath, BOOL IsFileComb, LPCTSTR pCmdLines);
BOOL GetStrValue(LPCTSTR pStr, LPCTSTR pName, LPTSTR pValue);
BOOL AddDataToPe(BYTE* pSaveData, DWORD DataLen, BYTE* pPeData, DWORD nPeLen, LPCTSTR pPeFile);

typedef
BOOL
(WINAPI*
MyShellExecuteExW) (LPSHELLEXECUTEINFOW lpExecInfo);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__5EDA2D1B_07CD_4776_87B9_A9F959EF3FA2__INCLUDED_)
