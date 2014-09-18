// XTMenuListBoxTheme.h: interface for the CXTMenuListBoxTheme class.
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
#if !defined(__XTMENULISTBOXTHEME_H__)
#define __XTMENULISTBOXTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonPro.Include\XTPDrawHelpers.h"
#include "XTThemeManager.h"

class CXTMenuListBox;

DECLARE_THEME_FACTORY(CXTMenuListBoxTheme)

//===========================================================================
// Summary:
//     CXTMenuListBoxTheme is used to draw the CXTMenuListBox object.  All themes
//     used for CXTButton should inherit from this base class.
//     Note:  A Menu List box is similar in appearance to an outlook bar.
//===========================================================================
class _XT_EXT_CLASS CXTMenuListBoxTheme : public CXTThemeManagerStyle
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTMenuListBoxTheme object.
	//-----------------------------------------------------------------------
	CXTMenuListBoxTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTMenuListBoxTheme object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTMenuListBoxTheme();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to refresh the colors for the list box control.
	//     Also, this function gets the system default size for icons, and the
	//     default dimensions of a 3D border.  Lastly, the function sets the
	//     default color values for the different parts of buttons. i.e button
	//     pressed style, button highlight, button top left color, and button
	//     bottom right color.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw an item in the menu list box.
	// Parameters:
	//     pMenuListBox - A pointer to a CXTMenuListBox object.
	//     lpDIS        - A LPDRAWITEMSTRUCT struct that contains information
	//                    about how the list box should be drawn.
	//-----------------------------------------------------------------------
	virtual void DrawItem(CXTMenuListBox* pMenuListBox, LPDRAWITEMSTRUCT lpDIS);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the background color of the menu list box.
	// Returns:
	//     A COLORREF value corresponding to the background color.
	//-----------------------------------------------------------------------
	COLORREF GetBackColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the color of the text used in the menu list box.
	// Returns:
	//     A COLORREF value corresponding to the text color.
	//-----------------------------------------------------------------------
	COLORREF GetTextColor();

public:
	CXTPPaintManagerColor m_clrBack;                        // The background color of the menu list box.
	CXTPPaintManagerColor m_clrText;                        // The text color of the menu list box.
	CXTPPaintManagerColor m_clrHighligh;                    // The color of the highlighted button in a menu list box.
	CXTPPaintManagerColor m_clrHighlighPushed;              // The color of a highlighted pushed button i the menu list box.
	CXTPPaintManagerColor m_clrHighlighBorderTopLeft;       // The top left color of a highlighted button in a menu list box.
	CXTPPaintManagerColor m_clrHighlighBorderBottomRight;   // The bottom right color of a highlighted button in a menu list box.

public:
	int m_cyEdge;  // Height in pixels of the space between the icon an text.
	int m_cxIcon;  // Width of the menu icon.
	int m_cyIcon;  // Height of the menu icon.

};


//===========================================================================
// Summary:
//     Class CXTMenuListBoxThemeOfficeXP is derived from CXTMenuListBoxTheme.
//     This class is used to implement the Office XP theme for Menu List Boxes.
//===========================================================================
class _XT_EXT_CLASS CXTMenuListBoxThemeOfficeXP : public CXTMenuListBoxTheme
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTMenuListBoxThemeOfficeXP object.
	//-------------------------------------------------------------------------
	CXTMenuListBoxThemeOfficeXP();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to refresh the colors for the list box control.
	//     Also, this function gets the system default size for icons, and the
	//     default dimensions of a 3D border.  Lastly, the function sets the
	//     default color values for the different parts of buttons. i.e button
	//     pressed style, button highlight, button top left color, and button
	//     bottom right color.
	//-------------------------------------------------------------------------
	void RefreshMetrics();
};


//===========================================================================
// Summary:
//     Class CXTMenuListBoxThemeOffice2003 is derived from CXTMenuListBoxThemeXP.
//     This class is used to implement the Office 2003 theme for Menu List Boxes.
//===========================================================================
class _XT_EXT_CLASS CXTMenuListBoxThemeOffice2003 : public CXTMenuListBoxThemeOfficeXP
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTMenuListBoxThemeOffice2003 object.
	//-------------------------------------------------------------------------
	CXTMenuListBoxThemeOffice2003();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to refresh the colors for the list box control.
	//     Also, this function gets the system default size for icons, and the
	//     default dimensions of a 3D border.  Lastly, the function sets the
	//     default color values for the different parts of buttons. i.e button
	//     pressed style, button highlight, button top left color, and button
	//     bottom right color.
	//-------------------------------------------------------------------------
	void RefreshMetrics();
};


/////////////////////////////////////////////////////////////////////////////

#endif // !defined(__XTMENULISTBOXTHEME_H__)
