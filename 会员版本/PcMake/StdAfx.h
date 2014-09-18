// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__FF92BA34_7FC7_4854_9C78_0F5924613CDD__INCLUDED_)
#define AFX_STDAFX_H__FF92BA34_7FC7_4854_9C78_0F5924613CDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
//	Header Files and import library
//
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <Wininet.h>
#include <winioctl.h>
#include "IOCTLCMD.H"
#include "Winsock2.h"
#include <stdlib.h>
#include <stdio.h>
#include <shellapi.h>
#include <psapi.h>
#include <Shlwapi.h>
#include <process.h>
#include <winsock2.h>

#define _PCINIT_

#include "..\PcShare\MyMarco.h"


//void ShowText(char* pText);

// TODO: reference additional headers your program requires here
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__FF92BA34_7FC7_4854_9C78_0F5924613CDD__INCLUDED_)
