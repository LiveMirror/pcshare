// XTOutBarTheme.h: interface for the CXTOutBarTheme class.
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
#if !defined(__XTOUTBARTHEME_H__)
#define __XTOUTBARTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonPro.Include\XTPDrawHelpers.h"
#include "XTThemeManager.h"

class CXTOutBarCtrl;
class CXTOutBarItem;
class CXTOutBarFolder;

DECLARE_THEME_FACTORY(CXTOutBarTheme)

//===========================================================================
// Summary:
//     CXTOutBarTheme is used to draw the CXTOutBarCtrl object.  All themes
//     used for CXTOutBarCtrl should inherit from this base class.
//===========================================================================
class _XT_EXT_CLASS CXTOutBarTheme : public CXTThemeManagerStyle
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTOutBarTheme object.
	//-----------------------------------------------------------------------
	CXTOutBarTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTOutBarTheme object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTOutBarTheme();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Refresh the colors on the OutBar.  Also, set the default colors
	//     for the background and the text on the OutBar.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Set the font used by the OutBar.
	// Parameters:
	//     pFont - A pointer to a CFont object.
	//-----------------------------------------------------------------------
	void SetFont(CFont* pFont);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw the icon associated with the pBarItem.
	// Parameters:
	//     pDC        - A CDC pointer that represents the current device
	//                  context.
	//     pt         - A CPoint object that specifies the XY location of the icon.
	//     pBarItem   - A pointer to a CXTOutBarItem object.  Used to maintain information
	//                  about the folder item.
	//     pImageList - A pointer to a CImageList object.  Used to store the icons
	//                  that are displayed in the folder.
	//-----------------------------------------------------------------------
	virtual void DrawItemIcon(CDC* pDC, CPoint pt, CXTOutBarItem* pBarItem, CImageList* pImageList);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw text associated with the item in the OutBar.
	// Parameters:
	//     pDC      - A CDC pointer that represents the current device
	//                context.
	//     rc       - A CRect object.  Used to specify the XY position of where
	//                the text is drawn.
	//     pBarItem - A pointer to a CXTOutBarItem object.  Used to maintain information
	//                about the folder item.  Contains the text to draw.
	//     nFormat  - The method of formatting the text.  Can be any valid formatting
	//                used for the MFC CDC::DrawText function.
	//-----------------------------------------------------------------------
	virtual void DrawItemText(CDC* pDC, CRect rc, CXTOutBarItem* pBarItem, UINT nFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw a filled rectangle specified at position rc.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device
	//           context.
	//     rc  - A CRect object.  Used to specify the XY position of where
	//           the rectangle is drawn.
	//-----------------------------------------------------------------------
	virtual void FillInsideRect(CDC* pDC, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw the folder on the OutBar.  The folder can be drawn
	//     in any one of three states, normal, selected, and highlighted.
	// Parameters:
	//     pDC        - A CDC pointer that represents the current device
	//                  context.
	//     rect       - A CRect object that specifies the XY location of the folder.
	//     pBarFolder - A CXTOutBarFolder object that maintains information about the folder.
	//     eHilight   - An int that specifies the height of the folder.  The possible
	//                  values are: xtMouseNormal, xtMouseSelect, and xtMouseHover.
	//-----------------------------------------------------------------------
	virtual void DrawFolder(CDC* pDC, CRect rect, CXTOutBarFolder* pBarFolder, const XTMouseState eHilight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw a 3D rectangle around an icon.
	// Parameters:
	//     pDC       - A CDC pointer that represents the current device
	//                 context.
	//     rcIcon    - A CRect object that specifies the XY location of the icon.
	//     bSelected - TRUE if the icon is currently selected.  FALSE otherwise.
	//     bPressed  - TRUE if the icon is pressed.  FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual void DrawIconFrame(CDC* pDC, CRect rcIcon, BOOL bSelected, BOOL bPressed);


protected:
	BOOL m_bFlatIcon;   // Set this to TRUE to draw flat icons.
	BOOL m_bPlainText;  // Set to TRUE to draw plain text.  Uses the system default font and font attributes.

public:
	CXTPPaintManagerColor m_clrBack;    // The color used to draw the background of the OutBar.
	CXTPPaintManagerColor m_clrText;    // The color used to draw the text on the OutBar.
};


//===========================================================================
// Summary:
//     Class CXTOutBarThemeOfficeXP is derived form CXTOutBarTheme.
//     This class is used to draw an OutBar with the Office XP theme.
//===========================================================================
class _XT_EXT_CLASS CXTOutBarThemeOfficeXP : public CXTOutBarTheme
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTOutBarThemeOfficeXP object.
	//-------------------------------------------------------------------------
	CXTOutBarThemeOfficeXP();

	//-------------------------------------------------------------------------
	// Summary:
	//     Refresh the colors on the OutBar.  Also, set the default colors
	//     for the background and the text on the OutBar.
	//-------------------------------------------------------------------------
	void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw a 3D rectangle around an icon.
	// Parameters:
	//     pDC       - A CDC pointer that represents the current device
	//                 context.
	//     rcIcon    - A CRect object that specifies the XY location of the icon.
	//     bSelected - TRUE if the icon is currently selected.  FALSE otherwise.
	//     bPressed  - TRUE if the icon is pressed.  FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual void DrawIconFrame(CDC* pDC, CRect rcIcon, BOOL bSelected, BOOL bPressed);

};


//===========================================================================
// Summary:
//     Class CXTOutBarThemeOffice2003 is derived form CXTOutBarThemeXP.
//     This class is used to draw an OutBar with the Office XP theme.
//===========================================================================
class _XT_EXT_CLASS CXTOutBarThemeOffice2003 : public CXTOutBarThemeOfficeXP
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTOutBarThemeOffice2003 object.
	//-------------------------------------------------------------------------
	CXTOutBarThemeOffice2003();

	//-------------------------------------------------------------------------
	// Summary:
	//     Refresh the colors on the OutBar.  Also, set the default colors
	//     for the background and the text on the OutBar.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw the folder on the OutBar.  The folder can be drawn
	//     in any one of three states, normal, selected, or highlighted.
	// Parameters:
	//     pDC        - A CDC pointer that represents the current device
	//                  context.
	//     rect       - A CRect object that specifies the XY location of the folder.
	//     pBarFolder - A CXTOutBarFolder object that maintains information about the folder.
	//     eHilight   - An int that specifies the height of the folder.  The possible
	//                  values are: xtMouseNormal, xtMouseSelect, and xtMouseHover.
	//-----------------------------------------------------------------------
	virtual void DrawFolder(CDC* pDC, CRect rect, CXTOutBarFolder* pBarFolder, const XTMouseState eHilight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw a 3D rectangle around an icon.
	// Parameters:
	//     pDC       - A CDC pointer that represents the current device
	//                 context.
	//     rcIcon    - A CRect object that specifies the XY location of the icon.
	//     bSelected - TRUE if the icon is currently selected.  FALSE otherwise.
	//     bPressed  - TRUE if the icon is pressed.  FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual void DrawIconFrame(CDC* pDC, CRect rcIcon, BOOL bSelected, BOOL bPressed);

protected:
	CXTPPaintManagerColorGradient m_grcSelected; // The selected gradient color.
	CXTPPaintManagerColorGradient m_grcHot;      // The hot gradient color.
	CXTPPaintManagerColorGradient m_grcPushed;   // The pushed gradient color.
	BOOL                          m_bLunaTheme;  // TRUE if the current theme is a Luna Theme.
};


/////////////////////////////////////////////////////////////////////////////

#endif // !defined(__XTOUTBARTHEME_H__)
