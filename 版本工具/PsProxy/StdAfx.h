// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__66042B08_617F_4693_86BA_D1A0B5E64BED__INCLUDED_)
#define AFX_STDAFX_H__66042B08_617F_4693_86BA_D1A0B5E64BED__INCLUDED_

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

#include "Winsock2.h"
#include <Shlwapi.h>

typedef struct _SOCKETINFO_
{
	char	m_RemoteAddr[256];
	DWORD	m_RemoteProt;
	DWORD	m_LocalPort;
}SOCKETINFO, *LPSOCKETINFO;

typedef struct _SENDRECVINFO_
{
	SOCKET sSend;
	SOCKET sRecv;
}SENDRECVINFO, *LPSENDRECVINFO;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__66042B08_617F_4693_86BA_D1A0B5E64BED__INCLUDED_)
