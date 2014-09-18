// XTFlatTabCtrlTheme.h: interface for the CXTFlatTabCtrlTheme class.
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
#if !defined(__XTFLATTABCTRLTHEME_H__)
#define __XTFLATTABCTRLTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonPro.Include\XTPDrawHelpers.h"
#include "XTThemeManager.h"

class CXTFlatTabCtrl;
class CXTFlatTabCtrlButtonState;

DECLARE_THEME_FACTORY(CXTFlatTabCtrlTheme)

//===========================================================================
// Summary:
//     CXTFlatTabCtrlTheme is used to draw the CXTFlatTabCtrl object.  All themes
//     used for CXTFlatTabCtrl should inherit from this base class.
//===========================================================================
class _XT_EXT_CLASS CXTFlatTabCtrlTheme : public CXTThemeManagerStyle
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTFlatTabCtrlTheme object.
	//-------------------------------------------------------------------------
	CXTFlatTabCtrlTheme();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTFlatTabCtrlTheme object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	~CXTFlatTabCtrlTheme();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to refresh the flat tab control colors,
	//     position any scroll bars to their default positions, and then
	//     reset the control to the system defaults.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     This member function will draw a tab to the device context specified
	//     by 'pDC'.
	// Parameters:
	//     pDC          - A CDC pointer that represents the current device
	//                    context.
	//     pTabCtrl     - A pointer to a CXTFlatTabCtrl object.
	//     pt           - A CPoint object that specifies the position of
	//                    the XY location of the top left corner of the tab to draw.
	//     bSelected    - True if the tab is currently selected.
	//     lpszTabLabel - A NULL terminated string that represents the tab label.
	// Returns:
	//     The x position of the next tab to be drawn.
	//-------------------------------------------------------------------------
	virtual int DrawTab(CDC* pDC, CXTFlatTabCtrl* pTabCtrl, const CPoint& pt, bool bSelected, LPCTSTR lpszTabLabel);

	//-------------------------------------------------------------------------
	// Summary:
	//     This member function is used by the flat tab control to draw an arrow.
	//     button to the device context specified by 'pDC'.
	// Parameters:
	//     pDC          - A CDC pointer that represents the current device
	//                    context.
	//     pTabCtrl     - A pointer to a CXTFlatTabCtrl object.
	//     button_state - XY location of the top left corner of the tab to draw.
	//-------------------------------------------------------------------------
	virtual void DrawButton(CDC* pDC, CXTFlatTabCtrl* pTabCtrl, CXTFlatTabCtrlButtonState& button_state);

public:

	CXTPPaintManagerColor m_clr3DShadow;   // RGB value that represents the tab shadow color.
	CXTPPaintManagerColor m_clrBtnText;    // RGB value that represents the tab outline color.
	CXTPPaintManagerColor m_clr3DHilight;  // RGB value that represents the tab highlight color.
	CXTPPaintManagerColor m_clrWindow;     // RGB value that represents the selected tab face color.
	CXTPPaintManagerColor m_clr3DFace;     // RGB value that represents the normal tab face color.
	CXTPPaintManagerColor m_clrWindowText; // RGB value that represents the tab text color.

protected:
	int m_cx; // Width for each arrow button.
	int m_cy; // Height for each arrow button
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(__XTFLATTABCTRLTHEME_H__)
