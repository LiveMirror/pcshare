// XTPVersion.h Xtreme Toolkit version definitions file
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
#if !defined(__XTPVERSION_H__)
#define __XTPVERSION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//}}AFX_CODEJOCK_PRIVATE

#undef _XTPLIB_PACKAGE_PREFIX

//////////////////////////////////////////////////////////////////////
// Xtreme Toolkit Macros
//////////////////////////////////////////////////////////////////////
#include "CommonPro.Include/XTPMacros.h"

//////////////////////////////////////////////////////////////////////
// Xtreme Toolkit Package
//////////////////////////////////////////////////////////////////////
#include "XTPPackage.h"

//////////////////////////////////////////////////////////////////////
// Xtreme Toolkit version definitions:
//////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
// Xtreme Toolkit Pro version number
#define _XTPLIB_VERSION_       MAKELONG(60, 9)

// Xtreme Toolkit Pro Major version number
#define _XTPLIB_VERSION_MAJOR  9

// Xtreme Toolkit Pro Minor version number
#define _XTPLIB_VERSION_MINOR  60

// Xtreme Toolkit Pro version prefix
#define _XTPLIB_VERSION_PREFIX 9601

#define _XTPLIB_FILE_PREFIX_EX_SINGLE(x)  #x
#define _XTPLIB_FILE_PREFIX_EX(x, y) _XTPLIB_FILE_PREFIX_EX_SINGLE(x) _XTPLIB_FILE_PREFIX_EX_SINGLE(y)

#undef _XTPLIB_FILE_PREFIX
#define _XTPLIB_FILE_PREFIX _XTPLIB_FILE_PREFIX_EX(_XTPLIB_PACKAGE_PREFIX, _XTPLIB_VERSION_PREFIX)

#endif  // __XTPVERSION_H__
//}}AFX_CODEJOCK_PRIVATE
