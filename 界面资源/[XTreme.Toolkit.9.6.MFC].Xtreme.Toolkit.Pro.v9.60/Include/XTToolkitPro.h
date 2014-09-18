// XTToolkitPro.h
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

#undef _XTP_INCLUDE_COMMANDBARS
#undef _XTP_INCLUDE_DOCKINGPANE
#undef _XTP_INCLUDE_PROPERTYGRID
#undef _XTP_INCLUDE_CONTROLS
#undef _XTP_INCLUDE_REPORTCONTROL

#undef __XTPVERSION_H__

#include "XTPIncludes.h"


//------------------------------------------------------
// Xtreme link to the appropriate dll or static library:
//------------------------------------------------------
#if defined( _AFXDLL ) && defined( _XTP_STATICLINK )
	#if defined( _DEBUG )
		#if defined( _UNICODE )
			#define _XTPLIB_FILE_SUFFIX "LibDynStaticud"
		#else
			#define _XTPLIB_FILE_SUFFIX "LibDynStaticd"
		#endif // _UNICODE
	#else
		#if defined( _UNICODE )
			#define _XTPLIB_FILE_SUFFIX "LibDynStaticu"
		#else
			#define _XTPLIB_FILE_SUFFIX "LibDynStatic"
		#endif // _UNICODE
	#endif // _DEBUG
	#define _XTPLIB_LINK_TYPE   "lib"
	#define _XTPLIB_LINK_IS_DLL 0
#elif !defined( _AFXDLL )
	#if defined( _DEBUG )
		#if defined( _UNICODE )
			#define _XTPLIB_FILE_SUFFIX "LibStaticud"
		#else
			#define _XTPLIB_FILE_SUFFIX "LibStaticd"
		#endif // _UNICODE
	#else
		#if defined( _UNICODE )
			#define _XTPLIB_FILE_SUFFIX "LibStaticu"
		#else
			#define _XTPLIB_FILE_SUFFIX "LibStatic"
		#endif // _UNICODE
	#endif // _DEBUG
	#define _XTPLIB_LINK_TYPE   "lib"
	#define _XTPLIB_LINK_IS_DLL 0
#else
	#if defined( _DEBUG )
		#if defined( _UNICODE )
			#define _XTPLIB_FILE_SUFFIX "Libud"
		#else
			#define _XTPLIB_FILE_SUFFIX "Libd"
		#endif // _UNICODE
	#else
		#if defined( _UNICODE )
			#define _XTPLIB_FILE_SUFFIX "Libu"
		#else
			#define _XTPLIB_FILE_SUFFIX "Lib"
		#endif // _UNICODE
	#endif // _DEBUG
	#define _XTPLIB_LINK_TYPE   "dll"
	#define _XTPLIB_LINK_IS_DLL 1
#endif // !defined( _AFXDLL ) || defined( _XTP_STATICLINK )

#if (_XTPLIB_LINK_IS_DLL == 0)  &&  defined(_XTP_DEMOMODE)
	#pragma message(" ")
	#pragma message("----------------------------------------------------------------------------------------------")
	#pragma message(" The evaluation version of the toolkit only supports DLL configurations.")
	#pragma message(" To purchase the full version (with static link support) please visit http://www.codejock.com")
	#pragma message("----------------------------------------------------------------------------------------------")
	#pragma message(" ")
	#error This build configuration is not supported by the evaluation library
#endif

#if !(defined(_DEBUG)) && defined(_XTP_DEMOMODE)
	#pragma message(" ")
	#pragma message("----------------------------------------------------------------------------------------------")
	#pragma message(" The evaluation version of the toolkit only supports DEBUG configurations.")
	#pragma message(" To purchase the full version (with static link support) please visit http://www.codejock.com")
	#pragma message("----------------------------------------------------------------------------------------------")
	#pragma message(" ")
	#error This build configuration is not supported by the evaluation library
#endif


#if defined(_XTP_DEMOMODE)
	#define _XTPLIB_FILE_PREFIX_FULL _XTPLIB_FILE_PREFIX "Eval"
#else
	#define _XTPLIB_FILE_PREFIX_FULL _XTPLIB_FILE_PREFIX
#endif

#ifndef _XTPLIB_NOAUTOLINK
#if defined( _XTPLIB_LINK_TYPE )  &&  defined ( _XTPLIB_FILE_SUFFIX )
	#pragma comment(lib, _XTPLIB_FILE_PREFIX_FULL _XTPLIB_FILE_SUFFIX ".lib")
	#pragma message("Automatically linking with " _XTPLIB_FILE_PREFIX_FULL _XTPLIB_FILE_SUFFIX "." _XTPLIB_LINK_TYPE)
#endif //_XTPLIB_LINK_TYPE && _XTPLIB_FILE_SUFFIX
#endif //_XTPLIB_NOAUTOLINK

#if !defined(_AFXDLL) && !defined(_XTPLIB_NOAUTOLIBIGNORE)
#ifdef _DEBUG
	#pragma comment(linker, "/NODEFAULTLIB:libcd.lib")
#else
	#pragma comment(linker, "/NODEFAULTLIB:libc.lib")
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
