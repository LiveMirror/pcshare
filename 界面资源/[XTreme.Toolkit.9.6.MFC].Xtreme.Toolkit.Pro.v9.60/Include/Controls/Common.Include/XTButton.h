// XTButton.h : interface for the CXTButton class.
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
#if !defined(__XTBUTTON_H__)
#define __XTBUTTON_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// forwards
class CXTThemeManager;
class CXTButtonTheme;
class CXTPImageManagerIcon;

#include "Theme/XTThemeManager.h"

// ---------------------------------------------------------------------
// Summary:
//     CXTButton is a CButton derived class. It is used to create
//     flat style and image buttons. This control has several features
//     including flat, semi-flat, and three dimensional borders.
// ---------------------------------------------------------------------
class _XT_EXT_CLASS CXTButton : public CButton, public CXTThemeManagerStyleHost
{
	DECLARE_DYNAMIC(CXTButton)
	DECLARE_THEME_HOST(CXTButton, CXTButtonTheme)

public:

	// ----------------------------------------
	// Summary:
	//     Constructs a CXTButton object
	// ----------------------------------------
	CXTButton();

	// -----------------------------------------------------------
	// Summary:
	//     Destroys a CXTButton object, handles cleanup and
	//     deallocation
	// -----------------------------------------------------------
	virtual ~CXTButton();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the highlight state for the button control.
	// Parameters:
	//     bChecked - Specifies whether the button is to be checked. TRUE
	//                will check the button. FALSE will uncheck it.
	// Remarks:
	//     This member function sets the highlighting state of a button
	//     control. This is the same as its CButton counterpart, except
	//     the m_bChecked flag is set.  This is done because MFC does not
	//     recognize the button as being checked unless it is a radio or
	//     check box.
	//-----------------------------------------------------------------------
	void SetStateX(BOOL bChecked);

	// ------------------------------------------------------------------------
	// Summary:
	//     Sets the highlight state for the button control.
	// Parameters:
	//     bChecked -  Specifies whether the button is to be checked. TRUE will
	//                 check the button. FALSE will uncheck it.
	// Remarks:
	//     This member function sets the highlighting state of the button
	//     control. This is the same as its CButton counterpart, except the
	//     m_bChecked flag is set. This is done because MFC does not recognize
	//     the button as being checked unless it is a radio or check box.
	// ------------------------------------------------------------------------
	void SetChecked(BOOL bChecked);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the gap between the button's edge
	//     and the image.
	// Parameters:
	//     nGap - Amount in pixels of the gap between the button edge and
	//            the image.
	// Returns:
	//     The previous border gap value.
	//-----------------------------------------------------------------------
	int SetBorderGap(int nGap);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the gap between the button's text
	//     and the image.
	// Parameters:
	//     nGap - Amount in pixels of the gap between the button text and
	//            the image.
	// Returns:
	//     The previous border gap value.
	//-----------------------------------------------------------------------
	int SetImageGap(int nGap);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the normal and hot state icons for
	//     the push button.
	// Parameters:
	//     size      - CSize object that represents the size of the icon.
	//     hIcon     - Handle to the normal icon.
	//     hIconHot  - Handle to the hot icon.
	//     nID       - Resource ID for the normal icon.
	//     nHotID    - Resource ID for the pressed icon.
	//     lpszID    - Resource string ID for the normal icon.
	//     lpszHotID - Resource string ID for the hot icon.
	//     bRedraw   - Specifies whether the button is to be redrawn. A
	//                 nonzero value redraws the button. A zero value does
	//                not redraw the button. The button is redrawn by default.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SetIcon(CSize size, HICON hIcon, HICON hIconHot=NULL, BOOL bRedraw=TRUE);
	virtual BOOL SetIcon(CSize size, UINT nID, UINT nHotID=0, BOOL bRedraw=TRUE);                //<COMBINE CXTButton::SetIcon@CSize@HICON@HICON@BOOL>
	virtual BOOL SetIcon(CSize size, LPCTSTR lpszID, LPCTSTR lpszHotID=NULL, BOOL bRedraw=TRUE); //<COMBINE CXTButton::SetIcon@CSize@HICON@HICON@BOOL>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the bitmap associated with the
	//     button.
	// Parameters:
	//     size    - CSize object that represents the size of the bitmap.
	//     nID     - Resource ID for the bitmap.
	//     bRedraw - Specifies whether the button is to be redrawn. A nonzero
	//               value redraws the button. A zero value does not redraw
	//               the button. The button is redrawn by default.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SetBitmap(CSize size,UINT nID,BOOL bRedraw=TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the XY location of the text and image
	//     that is displayed on the push button.
	// Parameters:
	//     ptImage - XY location of the image displayed on the button.
	//     ptText  - XY location of the text displayed on the button.
	//     bRedraw - Specifies whether the button is to be redrawn. A nonzero
	//               value redraws the button. A zero value does not redraw
	//               the button. The button is redrawn by default.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SetTextAndImagePos(CPoint ptImage,CPoint ptText,BOOL bRedraw=TRUE);

	// ----------------------------------------------------------------------------
	// Summary:
	//     This member function will set the display style for the button.
	// Parameters:
	//     dwxStyle -  Specifies the button style. The button style can be defined
	//                 as one or more of the styles listed in the Remarks section.
	//     bRedraw -   Specifies whether the button is to be redrawn. A nonzero value
	//                 redraws the button. A zero value does not redraw the button.
	//                 The button is redrawn by default.
	// Remarks:
	//     The CXTButton can be set to use one or more of the following
	//     styles:
	//     * <b>BS_XT_FLAT</b> Draws the button flat without any borders.
	//     * <b>BS_XT_SEMIFLAT</b> Draws the button with a thin 3D
	//           border.
	//     * <b>BS_XT_TWOROWS</b> Draws images and text that are
	//           centered.
	//     * <b>BS_XT_SHOWFOCUS</b> Draws a focus rect when the button
	//           has input focus.
	//     * <b>BS_XT_HILITEPRESSED</b> Highlights the button when
	//           pressed.
	//     * <b>BS_XT_XPFLAT</b> Draws a flat button using Office XP
	//           themes.
	//     * <b>BS_XT_FLAT_ANY</b> Same as (BS_XT_FLAT | BS_XT_SEMIFLAT)
	//     * <b>BS_XT_WINXP_COMPAT</b> Uses Windows XP themes if
	//           available. This setting overrides BS_XT_FLAT and BS_XT_SEMIFLAT,
	//           but <i>does not</i> override BS_XT_XPFLAT.
	// Returns:
	//     The previous style that was set.
	// See Also:
	//     GetXButtonStyle
	// ----------------------------------------------------------------------------
	virtual DWORD SetXButtonStyle(DWORD dwxStyle,BOOL bRedraw=TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function returns only the BS_XT_ style values, not any of the
	//     other window styles.
	// Returns:
	//     The button styles for this CXTButton object.
	//-----------------------------------------------------------------------
	virtual DWORD GetXButtonStyle();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine the gap between the
	//     button image and the button's edge.
	// Returns:
	//     The amount in pixels between the button image and border.
	//-----------------------------------------------------------------------
	int GetBorderGap() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine the gap between the
	//     button text and the image.
	// Returns:
	//     The amount in pixels between the button text and image displayed.
	//-----------------------------------------------------------------------
	int GetImageGap() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the button should
	//     display highlights on mouse over.
	// Returns:
	//     TRUE if the button display highlights on mouse over, otherwise
	//     returns FALSE.
	//-----------------------------------------------------------------------
	BOOL GetHilite() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the user has defined
	//     specific locations for text and image display.
	// Returns:
	//     TRUE if the user has defined text and image display location.
	//-----------------------------------------------------------------------
	BOOL GetUserPosition() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the left mouse button
	//     is currently pressed while hovering the button control.
	// Returns:
	//     TRUE if the left mouse button is pressed while over the button.
	//-----------------------------------------------------------------------
	BOOL GetLBtnDown() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the button control needs
	//     repainting.
	// Returns:
	//     TRUE if the button needs to be redrawn, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetPainted() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the checked state has
	//     been defined for the button control.
	// Returns:
	//     TRUE if the checked state is defined for the button control,
	//     otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL GetChecked() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the normal icon image
	//     used by the button control.
	// Returns:
	//     A handle to a HICON object.
	//-----------------------------------------------------------------------
	HICON GetNormalIcon();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the selected icon image
	//     used by the button control.
	// Returns:
	//     A handle to a HICON object.
	//-----------------------------------------------------------------------
	HICON GetSelectedIcon();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the disabled bitmap
	//     used by the button control.
	// Returns:
	//     A handle to a HBITMAP object.
	//-----------------------------------------------------------------------
	HBITMAP GetDisabledBitmap() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the size of the image
	//     displayed for the button control.
	// Returns:
	//     A CSize object.
	//-----------------------------------------------------------------------
	CSize GetImageSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the x-y location of the
	//     top left corner where the button image is displayed.
	// Returns:
	//     A CPoint object.
	//-----------------------------------------------------------------------
	CPoint GetImagePoint() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the x-y location of the
	//     top left corner where text is displayed.
	// Returns:
	//     A CPoint object.
	//-----------------------------------------------------------------------
	CPoint GetTextPoint() const;

	//--------------------------------------------------------------------
	// Summary:
	//     This member function is called to check if the button control
	//     is using a theme.
	// Returns:
	//     TRUE if a theme is used, otherwise FALSE.
	//--------------------------------------------------------------------
	virtual BOOL IsThemeValid() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return a pointer to the image
	//     manager for the button control.
	// Returns:
	//     A pointer to a CXTPImageManagerIcon object.
	//-----------------------------------------------------------------------
	CXTPImageManagerIcon* GetIcon() const;

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

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the colors for the BS_XT_XPFLAT mode.
	// Parameters:
	//     clrBorder    - An RGB value that represents the user defined
	//                    border color for the flat button.
	//     clrHighlight - An RGB value that represents the user defined
	//                    highlight color for the flat button.
	//     clrPressed   - An RGB value that represents the user defined
	//                    color for when the flat button is pressed.
	//-----------------------------------------------------------------------
	virtual void SetXPFlatColors(COLORREF clrBorder, COLORREF clrHighlight, COLORREF clrPressed);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function toggles the image display for the button on or off.
	//-----------------------------------------------------------------------
	virtual void ResetImage();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the font for the button control.
	// Parameters:
	//     pFont - Points to a valid CFont object.
	// Returns:
	//     A pointer to the previously set font for the button control.
	//-----------------------------------------------------------------------
	virtual CFont* SetFontEx(CFont* pFont);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to free all associated GDI resources
	//     that have been allocated.
	//-----------------------------------------------------------------------
	virtual void CleanUpGDI();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the button's text minus the '&'.
	// Parameters:
	//     bRemoveAmpersand - TRUE to remove the '&' used for mnemonics.
	// Returns:
	//     A CString object that represents the button's text.
	//-----------------------------------------------------------------------
	virtual CString GetButtonText(BOOL bRemoveAmpersand);

	// ---------------------------------------------------------------------
	// Summary:
	//     Call this helper function to set the pressed state and redraw the
	//     button.
	// Parameters:
	//     bDepressed -  true if the button is pressed, otherwise FALSE.
	// ---------------------------------------------------------------------
	void NoteButtonDepressed(BOOL bDepressed);

	//-----------------------------------------------------------------------
	// Summary:
	//     Defers control initialization
	//-----------------------------------------------------------------------
	void DeferInitialUpdate();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTButton)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:

	int                   m_nBorderGap;    // Gap between the button edge and the image.
	int                   m_nImageGap;     // Gap between the button text and the image.
	BOOL                  m_bHilite;       // TRUE if the button is highlighted.
	BOOL                  m_bUserPosition; // TRUE if the user has defined the XY coordinates for the image and text.
	BOOL                  m_bLBtnDown;     // TRUE if the left mouse button is pressed.
	BOOL                  m_bPainted;      // Used during paint operations.
	BOOL                  m_bChecked;      // TRUE if the button is checked.
	DWORD                 m_dwxStyle;      // The style of the button, see SetXButtonStyle.
	CSize                 m_sizeImage;     // Initial size of the button.
	CPoint                m_ptImage;       // XY location of the button image.
	CPoint                m_ptText;        // XY location of the button text.
	CXTPImageManagerIcon* m_pIcon;         // Multiple state icon manager converts icon to different states.

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTButton::SetImageGap(int nGap) {
	int nOldGap = m_nImageGap; m_nImageGap = nGap; return nOldGap;
}
AFX_INLINE int CXTButton::SetBorderGap(int nGap) {
	int nOldGap = m_nBorderGap; m_nBorderGap = nGap; return nOldGap;
}
AFX_INLINE DWORD CXTButton::GetXButtonStyle() {
	ASSERT(::IsWindow(m_hWnd)); return m_dwxStyle;
}
AFX_INLINE void CXTButton::SetStateX(BOOL bChecked) {
	m_bChecked = bChecked; if (GetSafeHwnd()) Invalidate(FALSE);
}
AFX_INLINE void CXTButton::SetChecked(BOOL bChecked) {
	m_bChecked = bChecked; if (GetSafeHwnd()) Invalidate(FALSE);
}
AFX_INLINE int CXTButton::GetBorderGap() const {
	return m_nBorderGap;
}
AFX_INLINE int CXTButton::GetImageGap() const {
	return m_nImageGap;
}
AFX_INLINE BOOL CXTButton::GetHilite() const {
	return m_bHilite;
}
AFX_INLINE BOOL CXTButton::GetUserPosition() const {
	return m_bUserPosition;
}
AFX_INLINE BOOL CXTButton::GetLBtnDown() const {
	return m_bLBtnDown;
}
AFX_INLINE BOOL CXTButton::GetPainted() const {
	return m_bPainted;
}
AFX_INLINE BOOL CXTButton::GetChecked() const {
	return m_bChecked;
}

const DWORD BS_XT_FLAT             = 0x0001;  //<ALIAS CXTButton::SetXButtonStyle@DWORD@BOOL>
const DWORD BS_XT_SEMIFLAT         = 0x0002;  //<ALIAS CXTButton::SetXButtonStyle@DWORD@BOOL>
const DWORD BS_XT_FLAT_ANY         = 0x0003;  //<ALIAS CXTButton::SetXButtonStyle@DWORD@BOOL>
const DWORD BS_XT_TWOROWS          = 0x0004;  //<ALIAS CXTButton::SetXButtonStyle@DWORD@BOOL>
const DWORD BS_XT_SHOWFOCUS        = 0x0010;  //<ALIAS CXTButton::SetXButtonStyle@DWORD@BOOL>
const DWORD BS_XT_HILITEPRESSED    = 0x0020;  //<ALIAS CXTButton::SetXButtonStyle@DWORD@BOOL>
const DWORD BS_XT_XPFLAT           = 0x0040;  //<ALIAS CXTButton::SetXButtonStyle@DWORD@BOOL>
const DWORD BS_XT_WINXP_COMPAT     = 0x1000;  //<ALIAS CXTButton::SetXButtonStyle@DWORD@BOOL>

#endif // #if !defined(__XTBUTTON_H__)
