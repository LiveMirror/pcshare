// XTPIncludes.h : header file
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

//--------------
// MFC includes:
//--------------
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>
#endif // _AFX_NO_OLE_SUPPORT

#ifdef _MFC_OVERRIDES_NEW
#define _INC_MALLOC
#endif
// MFC support for docking windows
#include <afxpriv.h>
// MFC template classes
#include <afxtempl.h>
// MFC ListView / TreeView support
#include <afxcview.h>


// MFC support for shell extensions
#include <shlobj.h>

// Version and Package of Xtreme Toolkit Pro
#include "XTPVersion.h"

#ifndef AFX_STATIC
#define AFX_STATIC static
#endif

//////////////////////////////////////////////////////////////////////
// NOTE: If using the Xtreme Toolkit Pro as a static library linked to an application
// that is dynamically linked with MFC, you will need to do the following:
//
// Open the XTPToolkit_Lib project workspace and select one of the
// Win32 Dynamic build settings and build the library. Add the following lines
// of code to your stdafx.h file:
//
//<code>#define _XTP_STATICLINK
// #include <XTToolkitPro.h></code>
//
// Add the following line of code to your *.rc2 file after the comment:
// "Add manually edited resources here...":
//
//<code>#include "XTToolkitPro.rc"</code>
//
#if !defined( _AFXDLL ) || defined( _XTP_STATICLINK )
#define _XTP_EXT_CLASS
#else
#define _XTP_EXT_CLASS  __declspec( dllimport )
#endif // !defined( _AFXDLL ) || defined( _XT_STATICLINK )

#if !defined(_XT_EXT_CLASS)
#define _XT_EXT_CLASS _XTP_EXT_CLASS
#endif // #if !defined(_XT_EXT_CLASS)

//-----------------------------------------
// Xtreme global and resource definitions:
//-----------------------------------------
#include "XTPResource.h"
#include "XTPResourceDll.h"

#ifdef _XTP_INCLUDE_DEPRECIATED
#include "CommonPro.include/XTPDepreciated.h"
#endif

#include "CommonPro.include/XTPMacros.h"
#include "CommonPro.Include/XTPVC80Helpers.h"
#include "CommonPro.Include/XTPVC50Helpers.h"
#include "CommonPro.include/XTPManageState.h"
#include "CommonPro.include/XTPResourceManager.h"
#include "CommonPro.include/Tmschema.h"
#include "CommonPro.include/UxTheme.h"
#include "CommonPro.include/XTPColorManager.h"
#include "CommonPro.include/XTPWinThemeWrapper.h"
#include "CommonPro.include/XTPDrawHelpers.h"
#include "CommonPro.include/XTPImageManager.h"
#include "CommonPro.include/XTMaskEditT.h"
#include "CommonPro.include/XTPPropExchange.h"




//-----------------------------------------------
// Xtreme Tab Manager
//-----------------------------------------------

#if defined(_XTP_INCLUDE_COMMANDBARS) || defined(_XTP_INCLUDE_DOCKINGPANE)
#include "TabManager/XTPTabManagerIncludes.h"
#endif

//-----------------------------------------------
// Xtreme Pro Controls -
//-----------------------------------------------

#ifdef _XTP_INCLUDE_CONTROLS
#include "Controls/XTIncludes.h"
#endif


//-----------------------------------------------
// Xtreme Pro Command Bars -
//-----------------------------------------------

#ifdef _XTP_INCLUDE_COMMANDBARS
#include "CommandBars/XTPCommandBarsIncludes.h"
#endif

//-----------------------------------------------
// Xtreme Pro Docking Pane -
//-----------------------------------------------

#ifdef _XTP_INCLUDE_DOCKINGPANE
#include "DockingPane/XTPDockingPaneIncludes.h"
#endif

//----------------------------
// Xtreme Property Grid classes:
//----------------------------

#ifdef _XTP_INCLUDE_PROPERTYGRID
#include "PropertyGrid/XTPPropertyGridIncludes.h"
#endif

//----------------------------
// Xtreme Report Control classes:
//----------------------------

#ifdef _XTP_INCLUDE_REPORTCONTROL
#include "ReportControl/XTPReportControlIncludes.h"
#endif
