// XTColorDialog.h : header file
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
#if !defined(__XTCOLORSHEET_H__)
#define __XTCOLORSHEET_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTColorDialog is a multiple inheritance class derived from CXTPDialogState
//     and CPropertySheet. It is an Office™ style color dialog and can be used
//     as a drop in replacement for the MFC CColorDialog API.
//===========================================================================
class _XT_EXT_CLASS CXTColorDialog : public CXTPDialogState, public CPropertySheet
{
	DECLARE_DYNAMIC(CXTColorDialog)

public:

	// ---------------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTColorDialog object
	// Parameters:
	//     clrNew -      An RGB value that represents the new color selection.
	//     clrCurrent -  An RGB value that represents the default color
	//                   selection.
	//     dwFlags -     Style for color dialog. See the Remarks section for available styles.
	//     pWndParent -  A pointer to the dialog box’s parent or owner window.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:
	//     * <b>CPS_XT_SHOW3DSELECTION</b> Displays the color selection
	//       box with a 3D raised border.
	//     * <b>CPS_XT_SHOWHEXVALUE</b> Displays the hex equivalent of the
	//       selected color.
	// See Also:
	//     CXTColorPopup::Create, CXTColorSelectorCtrl::Create,
	//     CXTColorPicker::ModifyCPStyle
	// ---------------------------------------------------------------------------------
	CXTColorDialog(COLORREF clrNew,COLORREF clrCurrent,DWORD dwFlags = 0L,CWnd* pWndParent = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTColorDialog object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTColorDialog();

public:

	// -------------------------------------------------------------------
	// Summary:
	//     Sets the active color for the dialog.
	// Parameters:
	//     clr -      An RGB value that represents the color.
	//     bNotify -  TRUE to notify tab pages of a color change.
	//
	// Remarks:
	//     Call this member function to set the current color selection to
	//     the color value specified in <i>'clr'</i>. The dialog box will
	//     automatically update the user’s selection based on the value of
	//     the <i>'clr'</i> parameter.
	// -------------------------------------------------------------------
	void SetNewColor(COLORREF clr, BOOL bNotify=TRUE);

	// ------------------------------------------------------------------
	// Summary:
	//     Call this function to set the new color selection to the color
	//     value specified in 'clr'.
	// Parameters:
	//     clr -  An RGB value that represents the color.
	// ------------------------------------------------------------------
	void SetCurrentColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the information about
	//     the current color.
	// Returns:
	//     A COLORREF value that contains the RGB information for
	//     the current color specified when the dialog was instantiated.
	//-----------------------------------------------------------------------
	COLORREF GetCurrentColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the information about
	//     the color that the user selected.
	// Returns:
	//     A COLORREF value that contains the RGB information
	//     for the new color selected in the color dialog box.
	//-----------------------------------------------------------------------
	COLORREF GetColor();

	// -------------------------------------------------------------
	// Summary:
	//     Call this member function to copy the string specified by
	//     'strText' to the Windows clipboard.
	// Parameters:
	//     strText -  A NULL terminated string.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	// -------------------------------------------------------------
	BOOL CopyToClipboard(CString strText);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns a CString object that represents the
	//     HEX conversion for the specified RGB color.
	// Parameters:
	//     clr - An RGB value that represents the color.
	// Returns:
	//     A CString object.
	//-----------------------------------------------------------------------
	static CString RGBtoHex(COLORREF clr);

	// ------------------------------------------------------------------------
	// Summary:
	//     Calculates the size of the dialog's tab control.
	// Remarks:
	//     This function is used to calculate the size of the dialog's tab
	//     control, the first tab item, the size of the OK button, and the size
	//     \of the cancel button. Then, resize the tab control to fit on the
	//     dialog. Also, reposition the OK and Cancel buttons. Next, reposition
	//     the Hex-Display. Finally, resize the property sheet.
	// ------------------------------------------------------------------------
	virtual void CalculateRects();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTColorDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTColorDialog)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:

	DWORD    m_dwStyle;    // Styles for the dialog.
	CEdit    m_editHex;    // Edit window to display the color hex value.
	CRect    m_rcNew;      // Size of the rectangle that represents the new color.
	CRect    m_rcCurrent;  // Size of the rectangle that represents the current color.
	COLORREF m_clrNew;     // A COLORREF value that contains the RGB information for the new color.
	COLORREF m_clrCurrent; // A COLORREF value that contains the RGB information for the current color.

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE COLORREF CXTColorDialog::GetCurrentColor() {
	return m_clrCurrent;
}
AFX_INLINE COLORREF CXTColorDialog::GetColor() {
	return m_clrNew;
}

//===========================================================================
// Summary:
//     CXTThemePropertyPage is a multiple inheritance class derived from
//     CXTPDialogState and CPropertyPage. It is used as a base class
//     for drawing Windows XP themed property pages.
//===========================================================================
class _XT_EXT_CLASS CXTThemePropertyPage : public CXTPDialogState, public CPropertyPage
{
	DECLARE_DYNCREATE(CXTThemePropertyPage)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTThemePropertyPage object
	// Parameters:
	//     pParentSheet - Points to the parent property sheet.
	//     nIDTemplate  - Dialog resource id.
	//     nIDCaption   - Dialog caption string id.
	//-----------------------------------------------------------------------
	CXTThemePropertyPage(CXTColorDialog* pParentSheet = 0, UINT nIDTemplate = 0, UINT nIDCaption = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTThemePropertyPage object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTThemePropertyPage();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw the specified text transparent.
	// Parameters:
	//     nIDResource - Resource ID of the text item to draw.
	//-----------------------------------------------------------------------
	virtual void MakeTextTransparent(UINT nIDResource);

	//-----------------------------------------------------------------------
	// Summary:
	//     This functions draws the themed background and the transparent
	//     text on the themed property page.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device
	//           context.
	//-----------------------------------------------------------------------
	virtual void OnDrawPage(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function draws the background of the themed property page.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device
	//           context.
	//     rc  - A CRect object that is used determine the location and the size
	//           of the property page.
	//-----------------------------------------------------------------------
	virtual void DrawThemedBack(CDC* pDC, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the CXTColorPageStandard
	//     class to render text transparently.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device
	//           context.
	//-----------------------------------------------------------------------
	virtual void DrawTransparentText(CDC* pDC);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTColorPageStandard)
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTColorPageStandard)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	CPtrArray           m_arTransText;  // Array of transparent text items.
	CXTColorDialog*     m_pParentSheet; // Points to the parent property sheet
	CXTPWinThemeWrapper m_themeWrapper; // Windows XP theme wrapper.

};

const DWORD CPS_XT_SHOWHEXVALUE    = 0x0020;  //<ALIAS CXTColorDialog::CXTColorDialog@COLORREF@COLORREF@DWORD@CWnd*>
const DWORD CPS_XT_SHOW3DSELECTION = 0x0040;  //<ALIAS CXTColorDialog::CXTColorDialog@COLORREF@COLORREF@DWORD@CWnd*>

#endif // !defined(__XTCOLORSHEET_H__)
