// XTTabCtrlTheme.h: interface for the CXTTabCtrlTheme class.
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
#if !defined(__XTTABCTRLTHEME_H__)
#define __XTTABCTRLTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonPro.Include\XTPDrawHelpers.h"
#include "XTThemeManager.h"

class CXTTabCtrl;
class CXTTabCtrlBase;
class CXTTabCtrlButton;

DECLARE_THEME_FACTORY(CXTTabCtrlTheme)

//===========================================================================
// Summary:
//     CXTTabCtrlTheme is used to draw the CXTTabCtrl object.  All themes
//     used for CXTTabCtrl should inherit from this base class.
//===========================================================================
class _XT_EXT_CLASS CXTTabCtrlTheme : public CXTThemeManagerStyle
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Construct a CXTTabCtrlTheme object.
	//-------------------------------------------------------------------------
	CXTTabCtrlTheme();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTTabCtrlTheme object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	~CXTTabCtrlTheme();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to refresh the colors of the Tab control.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to render the tab control using
	//     the specified theme.
	// Parameters:
	//     pDC      - A CDC pointer that represents the current device
	//                context.
	//     pTabCtrl - A pointer to a CXTTabCtrlBase object.  Contains information
	//                about the tab control.
	//-----------------------------------------------------------------------
	virtual void DrawTabCtrl(CDC* pDC, CXTTabCtrlBase* pTabCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to paint the Navigation buttons on the tab control.
	// Parameters:
	//     pTabCtrlBase - A pointer to a CXTTabCtrlBase object.Contains
	//                    information about the tab control.
	//-----------------------------------------------------------------------
	virtual void PaintButtons(CXTTabCtrlBase* pTabCtrlBase);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to draw single button of tab control.
	// Parameters:
	//     pDC       - A CDC pointer that represents the current device
	//                 context.
	//     pButton   - A pointer to a CXTTabCtrlButton object.
	//     clrButton - The color of the button.
	// Note:
	//     This function ASSERTS if called. Must be overriden.
	//-----------------------------------------------------------------------
	virtual void DrawButton(CDC* pDC, CXTTabCtrlButton* pButton, COLORREF clrButton);

protected:
	friend class CXTThemeManager;

};


//===========================================================================
// Summary:
//     Class CXTTabCtrlThemeOfficeXP is derived from CXTTabCtrlTheme.
//     This class is used to implement the office XP theme for the tab controls.
//===========================================================================
class _XT_EXT_CLASS CXTTabCtrlThemeOfficeXP : public CXTTabCtrlTheme
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Construct a CXTTabCtrlThemeOfficeXP object.
	//-------------------------------------------------------------------------
	CXTTabCtrlThemeOfficeXP();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     This function calls the base class RefreshMetrics and then
	//     calls DeleteObject() for the CPen member variables m_penBlack,
	//     m_penWhite, m_penFace, and m_penText.  Then the CPen member variables
	//     are created with the system default colors for COLOR_BTNTEXT,
	//     COLOR_3DHILIGHT, COLOR_3DFACE, and COLOR_3DSHADOW.  Finally, the
	//     member variable m_iEdge is set to the SM_CYEDGE system value.
	//-------------------------------------------------------------------------
	void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw the tab control.
	// Parameters:
	//     pDC          - A CDC pointer that represents the current device
	//                    context.
	//     pTabCtrlBase - A pointer to a CXTTabCtrlBase object.  Contains information
	//                    about the tab control.
	//-----------------------------------------------------------------------
	void DrawTabCtrl(CDC* pDC, CXTTabCtrlBase* pTabCtrlBase);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw the button.
	// Parameters:
	//     pDC       - A CDC pointer that represents the current device
	//                 context.
	//     pButton   - A pointer to a CXTTabCtrlButton object.
	//     clrButton - The color of the button.
	//-----------------------------------------------------------------------
	void DrawButton(CDC* pDC, CXTTabCtrlButton* pButton, COLORREF clrButton);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the tab control to draw bottom aligned
	//     XP style tabs.
	// Parameters:
	//     pDC          - Points to the client device context.
	//     pTabCtrlBase - A pointer to a CXTTabCtrlBase object.  Contains information
	//                    about the tab control.
	//     rcClient     - Size of the client area to paint.
	//     rcItem       - Size of the selected tab item.
	//-----------------------------------------------------------------------
	virtual void OnDrawBottom(CDC *pDC, CXTTabCtrlBase* pTabCtrlBase, CRect rcClient, CRect rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the tab control to draw top aligned
	//     XP style tabs.
	// Parameters:
	//     pDC          - Points to the client device context.
	//     pTabCtrlBase - Points to tab control to be drawn.
	//     rcClient     - Size of the client area to paint.
	//     rcItem       - Size of the selected tab item.
	//-----------------------------------------------------------------------
	virtual void OnDrawTop(CDC *pDC, CXTTabCtrlBase* pTabCtrlBase, CRect rcClient, CRect rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the tab control to draw left aligned
	//     XP style tabs.
	// Parameters:
	//     pDC          - Points to the client device context.
	//     pTabCtrlBase - Points to tab control to be drawn.
	//     rcClient     - Size of the client area to paint.
	//     rcItem       - Size of the selected tab item.
	//-----------------------------------------------------------------------
	virtual void OnDrawLeft(CDC *pDC, CXTTabCtrlBase* pTabCtrlBase, CRect rcClient, CRect rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the tab control to draw right aligned
	//     XP style tabs.
	// Parameters:
	//     pDC          - Points to the client device context.
	//     pTabCtrlBase - Points to tab control to be drawn.
	//     rcClient     - Size of the client area to paint.
	//     rcItem       - Size of the selected tab item.
	//-----------------------------------------------------------------------
	virtual void OnDrawRight(CDC *pDC, CXTTabCtrlBase* pTabCtrlBase, CRect rcClient, CRect rcItem);

protected:
	CPen m_penBlack; // Black pen (XP only).
	CPen m_penWhite; // White pen (XP only).
	CPen m_penFace;  // 3D face pen (XP only).
	CPen m_penText;  // Non-selected text pen (XP only).
	int  m_iEdge;    // The Y dimension of a 3D border.

};


/////////////////////////////////////////////////////////////////////////////

#endif // !defined(__XTTABCTRLTHEME_H__)
