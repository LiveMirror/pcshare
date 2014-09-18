// XTSplitterTheme.h: interface for the CXTSplitterTheme class.
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
#if !defined(__XTSPLITTERTHEME_H__)
#define __XTSPLITTERTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTSplitter;

#include "XTThemeManager.h"

DECLARE_THEME_FACTORY(CXTSplitterTheme)

//===========================================================================
// Summary:
//     Class CXTSplitterTheme is derived from CXTThemeManagerStyle.
//     This class is used to apply a Theme to splitter windows.
//===========================================================================
class _XT_EXT_CLASS CXTSplitterTheme : public CXTThemeManagerStyle
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Construct a CXTSplitterTheme object.
	//-----------------------------------------------------------------------
	CXTSplitterTheme();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the splitter windows face and the
	//     borders to the system default colors.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

public:
	COLORREF m_clrSplitterFace;     // The color of the splitter.
	COLORREF m_clrSplitterBorders;  // The color of the splitter borders.
};


//===========================================================================
// Summary:
//     Class CXTSplitterThemeOffice2003 is derived from CXTSplitterTheme.
//     This class is used to implement the Office 2003 theme for splitter
//     windows.
//===========================================================================
class _XT_EXT_CLASS CXTSplitterThemeOffice2003: public CXTSplitterTheme
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the splitter windows face and the
	//     borders to the system default colors.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(__XTSPLITTERTHEME_H__)
