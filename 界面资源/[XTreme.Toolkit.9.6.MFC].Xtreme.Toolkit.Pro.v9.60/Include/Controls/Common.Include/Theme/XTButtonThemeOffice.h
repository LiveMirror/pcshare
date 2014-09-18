// XTButtonThemeOffice.h: interface for the CXTButtonThemeOfficeXP class.
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
#if !defined(__XTBUTTONTHEMEOFFICEXP_H__)
#define __XTBUTTONTHEMEOFFICEXP_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// -------------------------------------------------------------------
// Summary:
//     CXTButtonThemeOfficeXP is a CXTButtonTheme derived class
//     that is used to draw an Office XP style theme for the button.
// -------------------------------------------------------------------
class _XT_EXT_CLASS CXTButtonThemeOfficeXP : public CXTButtonTheme
{
public:
	// ----------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTButtonThemeOfficeXP object.
	// Parameters:
	//     bWordTheme - TRUE to use the MS Word style theme to draw the border and
	//                  focus rectangle for the button.
	// ----------------------------------------------------------------------------
	CXTButtonThemeOfficeXP(BOOL bWordTheme=FALSE);

	// --------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTButtonThemeOfficeXP object, handles cleanup
	//     and deallocation.
	// --------------------------------------------------------------------
	virtual ~CXTButtonThemeOfficeXP();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine if the button should
	//     displayed raised borders on mouse over.
	// Parameters:
	//     pButton - Points to a CXTButton object.
	// Returns:
	//     TRUE if the button can display border highlights, otherwise
	//     returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL CanHilite(CXTButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each components theme.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable MS Word style
	//     theme for the button.
	// Parameters:
	//     bWordTheme - TRUE to use the MS Word style theme to draw the
	//                  border and focus rectangle for the button.
	//-----------------------------------------------------------------------
	virtual void UseWordTheme(BOOL bWordTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the pushed background color.
	// Parameters:
	//     color - RGB color value.
	//-----------------------------------------------------------------------
	virtual void SetBackPushedColor(COLORREF color);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the highlight background color.
	// Parameters:
	//     color - RGB color value.
	//-----------------------------------------------------------------------
	virtual void SetBackHiliteColor(COLORREF color);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the pushed text color.
	// Parameters:
	//     color - RGB color value.
	//-----------------------------------------------------------------------
	virtual void SetTextPushedColor(COLORREF color);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the SetAlternateColors text color.
	// Parameters:
	//     color - RGB color value.
	//-----------------------------------------------------------------------
	virtual void SetTextHiliteColor(COLORREF color);

protected:

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
	BOOL     m_bWordTheme;   // TRUE if MS Word theme is used.
	BOOL     m_bFadedIcon;   // TRUE if the button uses faded icons.
	BOOL     m_bAnimateIcon; // TRUE if the button animates on mouse over.
	CXTPPaintManagerColor m_crBackPushed; // RGB value for pushed background color.
	CXTPPaintManagerColor m_crBackHilite; // RGB value for highlighted background color.
	CXTPPaintManagerColor m_crTextPushed; // RGB value for highlighted text color.
	CXTPPaintManagerColor m_crTextHilite; // RGB value for pushed text color.
	CXTPPaintManagerColor m_crBackChecked;// RGB value for when the control is checked.
};

//===========================================================================

AFX_INLINE void CXTButtonThemeOfficeXP::UseWordTheme(BOOL bWordTheme) {
	m_bWordTheme = bWordTheme;
	RefreshMetrics();
}

// ---------------------------------------------------------------------
// Summary:
//     CXTButtonThemeOffice2003 is a CXTButtonThemeOfficeXP
//     derived class that is used to draw an Office 2003 style theme for
//     the button.
// ---------------------------------------------------------------------
class _XT_EXT_CLASS CXTButtonThemeOffice2003 : public CXTButtonThemeOfficeXP
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTButtonThemeOfficeXP object.
	// Parameters:
	//     bWordTheme - TRUE to use the MS Word style theme to draw the
	//                  border and focus rectangle for the button.
	//-----------------------------------------------------------------------
	CXTButtonThemeOffice2003(BOOL bWordTheme=FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each components theme.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

protected:
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

};
/////////////////////////////////////////////////////////////////////////////

#endif // !defined(__XTBUTTONTHEMEOFFICEXP_H__)
