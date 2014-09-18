// XTPDLLExports.h : common include file for all dll projects.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// ©1998-2005 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPDLLEXPORTS_H__)
#define __XTPDLLEXPORTS_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//-----------------------------------------------------------------------------
// Supporting MFC include files
//-----------------------------------------------------------------------------

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>
#endif // _AFX_NO_OLE_SUPPORT

#ifdef _MFC_OVERRIDES_NEW
#define _INC_MALLOC
#endif

#include <afxpriv.h>            // MFC support for docking windows
#include <afxtempl.h>           // MFC template classes
#include <afxcview.h>           // MFC ListView / TreeView support

#if (_MSC_VER >= 1300)
#include <..\src\mfc\afximpl.h> // Visual Studio .NET MFC Global data
#else
#include <..\src\afximpl.h>     // MFC Global data
#endif

#include <shlobj.h>             // MFC support for shell extensions
#include <afxres.h>
#include <io.h>
#include <stdio.h>
#include <tchar.h>

#ifndef AFX_STATIC
#define AFX_STATIC        extern
#define AFX_STATIC_DATA   extern __declspec(selectany)
#endif

//=============================================================================
// NOTE: If you are linking the static library to a client application that is
// dynamically linked with MFC, you will need to do the following:
//
// 1. Open the static library project workspace and select one of the Win32
//    Dynamic build settings and build the library.
//
// 2. Add the following lines of code to your project's stdafx.h file.  The
//    include file name may differ depending on the library you are using.
//
//    #define _XT_STATICLINK
//    #include <XTPToolkit.h>
//
// 3. Add the following line of code to your *.rc2 file after the comment "Add
//    manually edited resources here".  The include file name may differ
//    depending on the library you are using.
//
//    #include "XTPResource.rc"
//=============================================================================

//-----------------------------------------------------------------------------
// DLL Export Macro
//-----------------------------------------------------------------------------

#if !defined(_AFXDLL) || defined(_XT_STATICLINK)  // Static lib doesn't export
	#define _XTP_EXT_CLASS
#else
	#if defined(_XT_DLLEXPORTS)
	#define _XTP_EXT_CLASS    __declspec(dllexport) // Dynamic DLL exports
	#else
	#define _XTP_EXT_CLASS    __declspec(dllimport) // Client imports
	#endif
#endif // #if !defined(_AFXDLL) || defined(_XT_STATICLINK)

#if !defined(_XT_EXT_CLASS)
#define _XT_EXT_CLASS _XTP_EXT_CLASS
#endif // #if !defined(_XT_EXT_CLASS)

//}}AFX_CODEJOCK_PRIVATE

//////////////////////////////////////////////////////////////////////

#endif // #if !defined(__XTPDLLEXPORTS_H__)
