// XTCaptionTheme.h: interface for the CXTCaptionTheme class.
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
#if !defined(__XTCAPTIONTHEME_H__)
#define __XTCAPTIONTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTCaption;
class CXTButtonTheme;

#include "XTThemeManager.h"
#include "CommonPro.Include/XTPDrawHelpers.h"

DECLARE_THEME_FACTORY(CXTCaptionTheme)

// ----------------------------------------------------------------------
// Summary:
//     CXTCaptionTheme is a CXTThemeManagerStyle derived class. It
//     is used to control the caption theme. It is used to control the
//     theme of captions. Derive custom themes from CXTCaptionTheme.
// ----------------------------------------------------------------------
class _XT_EXT_CLASS CXTCaptionTheme : public CXTThemeManagerStyle
{
public:

	// -----------------------------------------------
	// Summary:
	//     Constructs a CXTCaptionTheme object.
	// -----------------------------------------------
	CXTCaptionTheme();

	// ----------------------------------------------------------------
	// Summary:
	//     This function is called to set the edges of a caption to the
	//     system default colors for edge shadow and edge light.
	// ----------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw the background of the caption at the
	//     location specified in the rcItem rect.
	// Parameters:
	//     pDC      - A CDC pointer that represents the current device
	//                context.
	//     pCaption - A pointer to a CXTCaption object.
	//     rcItem   - Location where the caption is drawn.
	//-----------------------------------------------------------------------
	virtual void DrawCaptionBack(CDC* pDC, CXTCaption* pCaption, CRect& rcItem);

	// ----------------------------------------------------------------
	// Summary:
	//     Called by CXTCaption to draw the caption text.
	// Parameters:
	//     pDC      -  A CDC pointer that represents the current device
	//                 context.
	//     pCaption -  A pointer to a CXTCaption object.
	// ----------------------------------------------------------------
	virtual void DrawCaptionText(CDC* pDC, CXTCaption* pCaption);

	// ----------------------------------------------------------------------
	// Summary:
	//     Called by CXTCaption to draw the caption icon.
	// Parameters:
	//     pDC      -  A CDC pointer that represents the current device
	//                 context.
	//     pCaption -  A pointer to a CXTCaption object.
	//     rcItem   -  Location where the caption is drawn.
	// Remarks:
	//     Call this function to draw the icon on the caption. Only draws the
	//     icon if there is enough space between the caption borders.
	// ----------------------------------------------------------------------
	virtual void DrawCaptionIcon(CDC* pDC, CXTCaption* pCaption, CRect& rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to retrieve the current theme for a button.
	// Parameters:
	//     pCaption - A pointer to a CXTCaption object.
	// Returns:
	//     CXTButtonTheme pointer with information of the
	//     current button theme.
	//-----------------------------------------------------------------------
	virtual CXTButtonTheme* GetButtonTheme(CXTCaption* pCaption);

public:
	CXTPPaintManagerColor m_clrEdgeShadow; // The color of the edge shadow for the current theme.
	CXTPPaintManagerColor m_clrEdgeLight;  // The color of the edge light for the current theme.

};


// ---------------------------------------------------------------------
// Summary:
//     CXTCaptionThemeOffice2003 is derived from CXTCaptionTheme.
//     It is used to set the caption to the Office 2003 theme.
// ---------------------------------------------------------------------
class _XT_EXT_CLASS CXTCaptionThemeOffice2003 : public CXTCaptionTheme
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the primary caption theme to Office 2003.
	// Parameters:
	//     bPrimaryCaption - TRUE  to set the Office 2003
	//     theme for the captions primary theme.  FALSE otherwise.
	//-----------------------------------------------------------------------
	CXTCaptionThemeOffice2003(BOOL bPrimaryCaption = TRUE);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw the background of the caption.
	// Parameters:
	//     pDC      - A CDC pointer that represents the current device
	//                context.
	//     pCaption - A pointer to a CXTCaption object.
	//     rcItem   - CRect object with the size and location where the caption is drawn.
	//-----------------------------------------------------------------------
	void DrawCaptionBack(CDC* pDC, CXTCaption* pCaption, CRect& rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draws the gradient fill of the background.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device
	//           context.
	//     rc  - CRect object that contains the size and location where the caption is drawn.
	//-----------------------------------------------------------------------
	void FillCaptionGradient(CDC* pDC, CRect& rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to retrieve the current theme for a button.
	// Parameters:
	//     pCaption - A pointer to a CXTCaption object.
	// Returns:
	//     A CXTButtonTheme pointer with the theme information for a button.
	//-----------------------------------------------------------------------
	CXTButtonTheme* GetButtonTheme(CXTCaption* pCaption);

protected:

	BOOL m_bPrimaryCaption;  // True if current caption is the primary caption. False otherwise.
};
//===========================================================================


/////////////////////////////////////////////////////////////////////////////

#endif // !defined(__XTCAPTIONTHEME_H__)
