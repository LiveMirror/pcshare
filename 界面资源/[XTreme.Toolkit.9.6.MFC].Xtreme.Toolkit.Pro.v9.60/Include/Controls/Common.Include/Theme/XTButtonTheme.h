// XTButtonTheme.h: interface for the CXTButtonTheme class.
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
#if !defined(__XTBUTTONTHEME_H__)
#define __XTBUTTONTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonPro.Include\XTPDrawHelpers.h"

class CXTButton;

DECLARE_THEME_FACTORY(CXTButtonTheme)

// -------------------------------------------------------------------
// Summary:
//     CXTButtonTheme is used to draw the CXTButton object. All
//     themes used for CXTButton should inherit from this base class.
// -------------------------------------------------------------------
class _XT_EXT_CLASS CXTButtonTheme : public CXTThemeManagerStyle
{
public:
	// ----------------------------------------------
	// Summary:
	//     Constructs a CXTButtonTheme object.
	// ----------------------------------------------
	CXTButtonTheme();

	// ----------------------------------------------------------------
	// Summary:
	//     Destroys a CXTButtonTheme object, handles cleanup and
	//     deallocation.
	// ----------------------------------------------------------------
	virtual ~CXTButtonTheme();

public:

	//--------------------------------------------------------------------
	// Summary:
	//     Call this member function to return a pointer to the Windows
	//     theme API wrapper for this control.
	// Returns:
	//     A CXTPWinThemeWrapper pointer.
	//--------------------------------------------------------------------
	CXTPWinThemeWrapper* GetThemeWrapper();

	// -----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine if the button should
	//     displayed raised borders on mouse over.
	// Parameters:
	//     pButton - Points to a CXTButton object.
	// Returns:
	//     TRUE if the button can display border highlights, otherwise returns
	//     FALSE.
	// -----------------------------------------------------------------------
	virtual BOOL CanHilite(CXTButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to determine if the button should
	//     be drawn using Windows XP themes.
	// Parameters:
	//     pButton - Points to a CXTButton object.
	// Returns:
	//     TRUE if the button is to be drawn using Windows XP themes,
	//     otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL UseWinXPThemes(CXTButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if icons are displayed
	//     for the button.
	// Returns:
	//     TRUE if icons are displayed, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsIconVisible() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable icon display
	//     for the button.
	// Parameters:
	//     bShowIcon - TRUE to display button icon.
	//-----------------------------------------------------------------------
	virtual void ShowIcon(BOOL bShowIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each components theme.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to return a pointer the font used
	//     by the button to display text.
	// Parameters:
	//     pButton - Points to a CXTButton object.
	// Returns:
	//     A CFont object that represents the font used to display text
	//     for the button if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	virtual CFont* GetThemeFont(CXTButton* pButton) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the font used by the button
	// Parameters:
	//     pFont - Points to a CFont object.
	//-----------------------------------------------------------------------
	virtual void SetThemeFont(CFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method will allow the user to define the default colors for the
	//     background shadow and highlight colors for the button.
	// Parameters:
	//     clr3DFace    - An RGB value that represents the user defined
	//                    face color for three dimensional display elements.
	//     clr3DHilight - An RGB value that represents the user defined
	//                    highlight color for three dimensional display
	//                    elements (edges facing the light source.)
	//     clr3DShadow  - An RGB value that represents the user defined
	//                    shadow color for three dimensional display
	//                    elements (edges facing away from the light source).
	//     clrBtnText   - An RGB value that represents the user defined
	//                    text color on push buttons.
	//-----------------------------------------------------------------------
	virtual void SetAlternateColors(COLORREF clr3DFace, COLORREF clr3DHilight, COLORREF clr3DShadow, COLORREF clrBtnText);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the default face color for the button.
	// Parameters:
	//     clrFace - An RGB value that represents the user defined face color.
	//-----------------------------------------------------------------------
	virtual void SetColorFace(COLORREF clrFace);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the current face color.
	// Returns:
	//     An RGB value that represents the current face color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetColorFace();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the default highlight color for the button.
	// Parameters:
	//     clrHilite - An RGB value that represents the user defined highlight color.
	//-----------------------------------------------------------------------
	virtual void SetColorHilite(COLORREF clrHilite);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the default shadow color for the button.
	// Parameters:
	//     clrShadow - An RGB value that represents the user defined shadow color.
	//-----------------------------------------------------------------------
	virtual void SetColorShadow(COLORREF clrShadow);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the default text color for the button.
	// Parameters:
	//     clrText - An RGB value that represents the user defined text color.
	//-----------------------------------------------------------------------
	virtual void SetColorText(COLORREF clrText);

protected:

	// --------------------------------------------------------------------
	// Summary:
	//     This member function will set the proper XY coordinates for the
	//     button text and image.
	// Parameters:
	//     point   - XY location of the text and image that are displayed.
	//     size    - Initial size of the image associated with the button.
	//     pButton - Points to a CXTButton object.
	// --------------------------------------------------------------------
	virtual void OffsetPoint(CPoint& point, CSize size, CXTButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to determine the location of
	//     the text displayed for the button.
	// Parameters:
	//     nState   - A UINT value that represents the current state for
	//                the button.
	//     rcItem   - A CRect object that represents the current size
	//                for the button.
	//     sizeText - A CSize object that represents the current size for
	//                the button text.
	//     pButton  - Points to a CXTButton object.
	// Returns:
	//     A CPoint object that represents the x-y location for the
	//     button text.
	//-----------------------------------------------------------------------
	virtual CPoint GetTextPosition(UINT nState, CRect& rcItem, CSize& sizeText, CXTButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the color used to draw
	//     the button text.
	// Parameters:
	//     nState  - A UINT value that represents the current state for
	//               the button.
	//     pButton - Points to a CXTButton object.
	// Returns:
	//     An RGB value that represents the button text color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetTextColor(UINT nState, CXTButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the text for the button.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     nState  - A UINT value that represents the current state for
	//               the button.
	//     rcItem  - A CRect reference that represents the current size
	//               for the button.
	//     pButton - Points to a CXTButton object.
	//-----------------------------------------------------------------------
	virtual void DrawButtonText(CDC* pDC, UINT nState, CRect& rcItem, CXTButton* pButton);

	// --------------------------------------------------------------------------
	// Summary:
	//     This member function is called to determine the location for the image
	//     displayed for the button.
	// Parameters:
	//     pDC             -  A CDC pointer that represents the current device
	//                        context.
	//     nState          -  A UINT value that represents the current state for the
	//                        button.
	//     rcItem          -  A CRect reference that represents the current size for
	//                        the button.
	//     bHasPushedImage -  Indicates if the button has a separate image for the
	//                        pushed state.
	//     pButton         -  Points to a CXTButton object.
	// Returns:
	//     A CPoint object that represents the x-y location for the button
	//     image.
	// --------------------------------------------------------------------------
	virtual CPoint CalculateImagePosition(CDC* pDC, UINT nState, CRect& rcItem, bool bHasPushedImage, CXTButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function draws the icon for the button, if one has
	//     been defined.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     nState  - A UINT value that represents the current state for
	//               the button.
	//     rcItem  - A CRect reference that represents the current size
	//               for the button.
	//     pButton - Points to a CXTButton object.
	//-----------------------------------------------------------------------
	virtual void DrawButtonIcon(CDC* pDC, UINT nState, CRect& rcItem, CXTButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the button.
	// Parameters:
	//     lpDIS   - A long pointer to a DRAWITEMSTRUCT structure. The
	//               structure contains information about the item to
	//               be drawn and the type of drawing required.
	//     pButton - Points to a CXTButton object.
	//-----------------------------------------------------------------------
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS, CXTButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the Windows XP theme
	//     background for the button.
	// Parameters:
	//     lpDIS   - A long pointer to a DRAWITEMSTRUCT structure. The
	//               structure contains information about the item to
	//               be drawn and the type of drawing required.
	//     pButton - Points to a CXTButton object.
	// Returns:
	//     TRUE if the background was drawn successfully, otherwise returns
	//     FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DrawWinThemeBackground(LPDRAWITEMSTRUCT lpDIS, CXTButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the background for the
	//     button.
	// Parameters:
	//     lpDIS   - A long pointer to a DRAWITEMSTRUCT structure. The
	//               structure contains information about the item to
	//               be drawn and the type of drawing required.
	//     pButton - Points to a CXTButton object.
	// Returns:
	//     TRUE if the background was drawn successfully, otherwise returns
	//     FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DrawButtonThemeBackground(LPDRAWITEMSTRUCT lpDIS, CXTButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the focus rectangle
	//     for the button.
	// Parameters:
	//     pDC     - A CDC pointer that represents the current device
	//               context.
	//     nState  - A UINT value that represents the current state for
	//               the button.
	//     rcItem  - A CRect reference that represents the current size
	//               for the button.
	//     pButton - Points to a CXTButton object.
	//-----------------------------------------------------------------------
	virtual void DrawFocusRect(CDC* pDC, UINT nState, CRect& rcItem, CXTButton* pButton);

protected:

	BOOL                  m_bShowIcon;          // TRUE if the buttons displays an icon.
	BOOL                  m_bOffsetHiliteText;  // TRUE if highlighted text is animated.
	CFont*                m_pFont;              // Points to a user defined font for the theme.
	CXTPPaintManagerColor m_crBack;             // RGB value for background color.
	CXTPPaintManagerColor m_crText;             // RGB value for text color.
	CXTPPaintManagerColor m_crTextDisabled;     // RGB value for disabled text color.
	CXTPPaintManagerColor m_crBorderHilite;     // RGB value for border highlight color.
	CXTPPaintManagerColor m_crBorderShadow;     // RGB value for border shadow color.
	CXTPPaintManagerColor m_crBorder3DHilite;   // RGB value for 3D border highlight color.
	CXTPPaintManagerColor m_crBorder3DShadow;   // RGB value for 3D border shadow color.
	CXTPWinThemeWrapper   m_themeWrapper;       // Wraps the Windows XP theme API that is used to display Windows XP themes.

	friend class CXTButton;
	friend class CXTThemeManager;
};

//===========================================================================

AFX_INLINE CXTPWinThemeWrapper* CXTButtonTheme::GetThemeWrapper() {
	return &m_themeWrapper;
}
AFX_INLINE void CXTButtonTheme::ShowIcon(BOOL bShowIcon) {
	m_bShowIcon = bShowIcon;
}
AFX_INLINE BOOL CXTButtonTheme::IsIconVisible() const {
	return m_bShowIcon;
}

//===========================================================================
// Summary:
//     CXTDisabledButtonTheme is a CXTButtonTheme derived class
//     that is used to draw 3D disabled icons for the button.
//===========================================================================
class _XT_EXT_CLASS CXTDisabledButtonTheme : public CXTButtonTheme
{
protected:
	// ------------------------------------------------------------------------
	// Summary:
	//     This member function draws the icon for the button, if one has
	//     been defined.
	// Parameters:
	//     pDC     -  A CDC pointer that represents the current device context.
	//     nState  -  A UINT value that represents the current state for the
	//                button.
	//     rcItem  -  A CRect reference that represents the current size for the
	//                button.
	//     pButton -  Points to a CXTButton object.
	// ------------------------------------------------------------------------
	virtual void DrawButtonIcon(CDC* pDC, UINT nState, CRect& rcItem, CXTButton* pButton);

	friend class CXTButton;
};

//===========================================================================
// Summary:
//     CXTToolbarButtonTheme is a CXTDisabledButtonTheme derived class
//     that is used to draw Windows XP style toolbar theme for the button.
//===========================================================================
class _XT_EXT_CLASS CXTToolbarButtonTheme : public CXTDisabledButtonTheme
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the Windows XP theme
	//     background for the button.
	// Parameters:
	//     lpDIS   - A long pointer to a DRAWITEMSTRUCT structure. The
	//               structure contains information about the item to
	//               be drawn and the type of drawing required.
	//     pButton - Points to a CXTButton object.
	// Returns:
	//     TRUE if the background was drawn successfully, otherwise returns
	//     FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DrawWinThemeBackground(LPDRAWITEMSTRUCT lpDIS, CXTButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to determine if the button should
	//     be drawn using Windows XP themes.
	// Parameters:
	//     pButton - Points to a CXTButton object.
	// Returns:
	//     TRUE if the button is to be drawn using Windows XP themes,
	//     otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL UseWinXPThemes(CXTButton* pButton);

protected:
	friend class CXTButton;
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(__XTBUTTONTHEME_H__)
