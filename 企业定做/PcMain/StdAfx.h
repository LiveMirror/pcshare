// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__DAAF01B9_2810_40F0_907A_531EEE19FF12__INCLUDED_)
#define AFX_STDAFX_H__DAAF01B9_2810_40F0_907A_531EEE19FF12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#define WINVER 0x0501		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#define _WIN32_WINNT 0x0501		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#define _WIN32_IE 0x0501	//为 IE 5.0 及更新版本改变为适当的值。

#include <windows.h>

#include "..\PcShare\MyMarco.h"
#include "MyFunc.h"
#include "MyMainFunc.h"
#include "MyClientTran.h"

typedef void (WINAPI* SERVICEMAINPCMAKE)(ULONG, LPTSTR*);

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__DAAF01B9_2810_40F0_907A_531EEE19FF12__INCLUDED_)
