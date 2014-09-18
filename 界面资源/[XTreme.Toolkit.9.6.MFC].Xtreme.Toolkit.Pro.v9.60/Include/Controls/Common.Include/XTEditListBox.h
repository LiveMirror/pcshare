// XTEditListBox.h interface for the CXTEditListBoxToolBar class.
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
#if !defined(__XTEDITLISTBOX_H__)
#define __XTEDITLISTBOX_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// Summary:
//     CXTListBox is a CListBox derived class. CXTListBox extends the standard
//     list box control to enable flicker free drawing.
//===========================================================================
class _XT_EXT_CLASS CXTListBox : public CListBox
{
	DECLARE_DYNAMIC(CXTListBox)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTListBox object
	//-----------------------------------------------------------------------
	CXTListBox();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTListBox object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTListBox();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to initialize the list box.  This method
	//     should be called directly after creating or sub-classing the control.
	// Parameters:
	//     bAutoFont - True to enable automatic font initialization.
	//-----------------------------------------------------------------------
	virtual void Initialize(bool bAutoFont=true);

	//-----------------------------------------------------------------------
	// Summary:
	//     This is a helper function is used by the list box to suspend drawing while the list
	//          is performing a scroll event.
	// Parameters:
	//     bEnable - Specifies the state of the redraw flag. If this parameter is true, the
	//               redraw flag is set; if false, the flag is cleared.
	//-----------------------------------------------------------------------
	virtual void SetRedrawEx(bool bEnable);

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTListBox)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTListBox)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE
};

// ----------------------------------------------------------------------
// Summary:
//     List used for enumerating CXTLogFont structures.
// Remarks:
//     CList definition used by the CXTFontEnum class to maintain
//     a list of CXTLogFont objects representing each font displayed
//     for a CXTFontListBox control.
// See Also:
//     CXTFontEnum::GetFontList
// ----------------------------------------------------------------------
typedef CList<CXTLogFont, CXTLogFont&> CXTFontList;

//===========================================================================
// Summary:
//     CXTFontEnum is a stand alone singleton font enumeration class. It is
//     used to enumerate a list of fonts found installed for the operating
//     system.
// Remarks:
//     CXTFontEnum is a singleton class, which means it can only be
//     instantiated a single time. The constructor is private, so the only
//     way to access members of this class is to use the objects Get() method.
//     To retrieve a list of available fonts for your operating system, you
//     would make the following call:
//
// Example: <code>CXTFontEnum::Get().GetFontList()</code>
//===========================================================================
class _XT_EXT_CLASS CXTFontEnum
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTFontEnum object. CXTFontEnum is a singleton
	//     class, to instantiate an object, use the static method Get().
	//-----------------------------------------------------------------------
	CXTFontEnum();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTFontEnum object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTFontEnum();


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This static member function will return a reference to the one
	//     and only CXTFontEnum object. You can use this function to access
	//     data members for the CXTFontEnum class.
	// Example:
	//     <code>CXTFontEnum::Get().GetFontList()</code>
	// Returns:
	//     A reference to a CXTFontEnum object.
	//-----------------------------------------------------------------------
	static CXTFontEnum& Get();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine the existence of the font
	//     specified by 'strFaceName'.
	// Parameters:
	//     strFaceName - Reference to a NULL terminated string that represents
	//                   the font name.
	// Returns:
	//     true if the font exists, otherwise returns false.
	//-----------------------------------------------------------------------
	bool DoesFontExist(CString& strFaceName);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to get a pointer to the font specified
	//     by 'strFaceName'.
	// Parameters:
	//     strFaceName - A NULL terminated string that represents the font
	//                   name.
	// Returns:
	//     A pointer to the CXTLogFont structure for the specified item,
	//     or NULL if no font was found.
	//-----------------------------------------------------------------------
	CXTLogFont* GetLogFont(CString strFaceName);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used by the callback function to retrieve the
	//     current width for the longest font name in the list.
	// Returns:
	//     An integer value that represents the width for the longest font
	//     in the list.
	//-----------------------------------------------------------------------
	int GetMaxWidth();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to get a reference to the font list.
	// Returns:
	//     A reference to the CXTFontList used by this class.
	//-----------------------------------------------------------------------
	CXTFontList& GetFontList();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the CXTFontEnum class to initialize
	//     the font list. You can also call this member function to reinitialize
	//     the font enumeration. For example, if you changed printers and you want
	//     to enumerate printer fonts, or you wanted to use a different character
	//     set.
	// Parameters:
	//     pDC      - Points to a valid device context, if NULL, the screen
	//                device context is used.
	//     nCharSet - Represents the character set to enumerate.
	//-----------------------------------------------------------------------
	void Init(CDC* pDC=NULL,BYTE nCharSet=DEFAULT_CHARSET);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     The EnumFontFamExProc function is an application defined–callback
	//     function used with the EnumFontFamiliesEx function. It is used to process
	//     the fonts and is called once for each enumerated font. The FONTENUMPROC
	//     type defines a pointer to this callback function. EnumFontFamExProc
	//     is a placeholder for the application defined–function name.
	// Parameters:
	//     pelf       - Pointer to an ENUMLOGFONTEX structure that contains
	//                  information about the logical attributes of the font.
	//     lpntm      - Pointer to a structure that contains information
	//                  about the physical attributes of a font. The function
	//                  uses the NEWTEXTMETRICEX structure for TrueType fonts;
	//                  and the TEXTMETRIC structure for other fonts.
	//     dwFontType - Specifies the type of the font. This parameter can be a combination
	//              of the values in the Remarks section.
	//     lParam - Specifies the application-defined data passed by the EnumFontFamiliesEx
	//     function.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:<p/>
	//          * <b>DEVICE_FONTTYPE</b> The font is a device font.
	//          * <b>RASTER_FONTTYPE</b> The font is a raster font.
	//          * <b>TRUETYPE_FONTTYPE</b> The font is a TrueType font.
	// Returns:
	//     The return value must be a nonzero value to continue enumeration.
	//     To stop enumeration, the return value must be zero.
	//-----------------------------------------------------------------------
	static BOOL CALLBACK EnumFontFamExProc(ENUMLOGFONTEX* pelf,NEWTEXTMETRICEX* lpntm,DWORD dwFontType,LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the font enumeration callback to
	//     add a font to the font list.
	// Parameters:
	//     pLF    - Points to a valid LOGFONT structure.
	//     dwType - Specifies the type of the font. This parameter can be a combination
	//              of the values in the Remarks section.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:<p/>
	//          * <b>DEVICE_FONTTYPE</b> The font is a device font.
	//          * <b>RASTER_FONTTYPE</b> The font is a raster font.
	//          * <b>TRUETYPE_FONTTYPE</b> The font is a TrueType font.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	bool AddFont(const LOGFONT* pLF,DWORD dwType);

protected:
	CXTFontList m_listFonts; // List of fonts found during enumeration
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTFontList& CXTFontEnum::GetFontList() {
	return m_listFonts;
}

// -------------------------------------------------------------------
// Summary:
//     Enumeration used to determine font display.
// Remarks:
//     XTFontStyle type defines the constants used by the CXTFontListBox
//     class to determine how the fonts will be displayed in the CXTFontListBox
//     control.
// See Also:
//    CXTFontListBox, CXTFontListBox::SetListStyle
//
// <KEYWORDS xtFontGUI, xtFontSample, xtFontBoth>
// -------------------------------------------------------------------
enum XTFontStyle
{
	xtFontGUI,     // Display font name with GUI font style.
	xtFontSample,  // Display font name with its own font style.
	xtFontBoth     // Display font name with GUI font style, then a sample display to the right.
};

class CXTFontListBoxWndHook;

//===========================================================================
// Summary:
//     CXTFontListBox is a CXTListBox derived class. It is used to create
//     a font selection list box.
// Remarks:
//     You can choose to display the font name
//     with the GUI font style, display the font name with its own font style,
//     or display the font name with the default GUI font style and a sample
//     display to the right.
//===========================================================================
class _XT_EXT_CLASS CXTFontListBox : public CXTListBox
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTFontListBox object
	//-----------------------------------------------------------------------
	CXTFontListBox();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTFontListBox object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTFontListBox();

public:

	// ------------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the font for the currently selected
	//     item.
	// Parameters:
	//     lf -           Reference to an CXTLogFont structure.
	//     strFaceName -  A reference to a valid CString object to receive the
	//                    logfont face name.
	// Returns:
	//     true if successful, otherwise returns false.
	// ------------------------------------------------------------------------
	virtual bool GetSelFont(CXTLogFont& lf);
	virtual bool GetSelFont(CString& strFaceName); //<combine CXTFontListBox::GetSelFont@CXTLogFont&>

	// -------------------------------------------------------------------
	// Summary:
	//     Call this member function to select the font for the list box.
	// Parameters:
	//     lf -           Reference to an CXTLogFont structure.
	//     strFaceName -  A NULL terminated string that represents the logfont
	//                    face name.
	// Returns:
	//     true if successful, otherwise returns false.
	// -------------------------------------------------------------------
	virtual bool SetSelFont(CXTLogFont& lf);
	virtual bool SetSelFont(CString strFaceName); //<combine CXTFontListBox::SetSelFont@CXTLogFont&>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the font display style for the font
	//     list box. There are three styles to choose from that include displaying
	//     the font in the default GUI font, displaying the font in its own font
	//     style, or displaying both the font name in the default GUI font and
	//     a sample to the right.
	// Parameters:
	//     eStyle - Specifies the style for the font list box. See XTFontStyle for
	//              a list of available styles.
	// See Also: XTFontStyle
	//-----------------------------------------------------------------------
	void SetListStyle(XTFontStyle eStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to initialize the font list box and populate it
	//     with a list of available fonts.  This method should be called directly
	//     after creating or sub-classing the control.
	// Parameters:
	//     bAutoFont - True to enable automatic font initialization.
	//-----------------------------------------------------------------------
	virtual void Initialize(bool bAutoFont=true);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTFontListBox)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTFontListBox)
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:

	XTFontStyle      m_eStyle;     // Enumerated style indicating how to display the font list.
	CString     m_strSymbol;       // String displayed for the symbol characters.
	CImageList  m_ilFontType;      // True type font image list.

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTFontListBox::SetListStyle(XTFontStyle eStyle) {
	m_eStyle = eStyle;
}

//////////////////////////////////////////////////////////////////////

// ---------------------------------------------------------------------
// Summary:
//     CXTEditListBoxToolBar is a CStatic derived class. It used by the
//     CXTEditListBox class to create a toolbar above the edit list box
//     to display icons for editing.
// Remarks:
//     CXTEditListBoxToolBar can be used for other classes by
//     setting the notify window in Initialize. This window will receive
//     notification messages whenever the new, delete, up, and down
//     buttons are pressed. You can handle these messages by adding an
//     ON_BN_CLICKED handler for each of the buttons XT_IDC_BTN_NEW,
//     XT_IDC_BTN_DELETE, XT_IDC_BTN_UP and XT_IDC_BTN_DOWN.
// ---------------------------------------------------------------------
class _XT_EXT_CLASS CXTEditListBoxToolBar : public CStatic
{
	DECLARE_DYNAMIC(CXTEditListBoxToolBar)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTEditListBoxToolBar object
	//-----------------------------------------------------------------------
	CXTEditListBoxToolBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTEditListBoxToolBar object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTEditListBoxToolBar();

public:

	// ------------------------------------------------------------------------
	// Summary:
	//     Initializes the CXTEditListBoxToolBar control.
	// Parameters:
	//     bAutoFont -  True to enable automatic font initialization.
	// Remarks:
	//     Call this member function to initialize the edit group control. This
	//     method should be called directly after creating or sub-classing the
	//     control.
	// ------------------------------------------------------------------------
	virtual void Initialize(bool bAutoFont=true);

	// ---------------------------------------------------------------------
	// Summary:
	//     This member function returns a reference to the new button of the
	//     edit group.
	// Returns:
	//     A reference to a CXTButton object.
	// ---------------------------------------------------------------------
	CXTButton& GetNewButton();

	// --------------------------------------------------------------------
	// Summary:
	//     This member function returns a reference to the delete button of
	//     the edit group.
	// Returns:
	//     A reference to a CXTButton object.
	// --------------------------------------------------------------------
	CXTButton& GetDeleteButton();

	// --------------------------------------------------------------------
	// Summary:
	//     This member function returns a reference to the up button of the
	//     edit group.
	// Returns:
	//     A reference to a CXTButton object.
	// --------------------------------------------------------------------
	CXTButton& GetUpButton();

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function returns a reference to the down button of the
	//     edit group.
	// Returns:
	//     A reference to a CXTButton object.
	// ----------------------------------------------------------------------
	CXTButton& GetDownButton();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will enable or disable editing.
	// Parameters:
	//     bEnable - True to enable editing.
	//-----------------------------------------------------------------------
	void EnableEdit(bool bEnable);

	// --------------------------------------------------------------------------
	// Summary:
	//     This member function will set the display style for the toolbar
	//     buttons.
	// Parameters:
	//     dwxStyle -  Specifies the button style as defined in the Remarks
	//                 section.
	//     bRedraw -   Specifies whether the button is to be redrawn. A nonzero
	//                 value redraws the button. A zero value does not redraw the
	//                 button. The button is redrawn by default.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:<p/>
	//     * <b>BS_XT_FLAT</b> Draws a flat button.
	//     * <b>BS_XT_SEMIFLAT</b> Draws a semi-flat button.
	//     * <b>BS_XT_TWOROWS</b> Draws images and text that are
	//           centered.
	//     * <b>BS_XT_SHOWFOCUS</b> Draws a focus rect when the button
	//           has input focus.
	//     * <b>BS_XT_HILITEPRESSED</b> Highlights the button when
	//           pressed.
	//     * <b>BS_XT_XPFLAT</b> Draws a flat button ala Office XP.
	//     * <b>BS_XT_WINXP_COMPAT</b> Enables Windows XP themes if
	//           available.
	// Returns:
	//     The previous style that was set.
	// --------------------------------------------------------------------------
	virtual void SetXButtonStyle(DWORD dwxStyle,BOOL bRedraw=TRUE);

	//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_VIRTUAL(CXTEditListBoxToolBar)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the CXTEditListBoxToolBar object to
	//     render text display for the control.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rcClient - Area to draw text on.
	//-----------------------------------------------------------------------
	virtual void DrawText(CDC* pDC, CRect& rcClient);

	// ------------------------------------------------------------------
	// Summary:
	//     Recalculates the button layout within the CXTEditListBoxToolBar window.
	// Remarks:
	//     This member function is called by the CXTEditListBoxToolBar object to
	//     position the group bar buttons when the window is sized.
	// ------------------------------------------------------------------
	virtual void MoveButtons();

	// ---------------------------------------------------------------------
	// Summary:
	//     Sends notification to the owner window.
	// Parameters:
	//     nCmdID -  Command ID to send.
	// Remarks:
	//     This member function sends the command specified by <i>nCmdID</i>
	//     to the owner of the CXTEditListBoxToolBar object. The command is
	//     sent whenever a button is pressed on the group bar.
	// ---------------------------------------------------------------------
	virtual void SendCommand(UINT nCmdID);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTEditListBoxToolBar)
	afx_msg void OnButtonNew();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:

	bool          m_bEnableEdit;   // True if editing is enabled.
	CRect         m_arClipRect[4]; // Array of toolbar button sizes.
	CXTButton     m_arButton[4];   // Array of toolbar buttons.
	CXTIconHandle m_arIcon[4];     // Array of toolbar button icons.
	CToolTipCtrl  m_tooltip;       // Tooltip control for edit buttons.
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTButton& CXTEditListBoxToolBar::GetNewButton() {
	return m_arButton[0];
}
AFX_INLINE CXTButton& CXTEditListBoxToolBar::GetDeleteButton() {
	return m_arButton[1];
}
AFX_INLINE CXTButton& CXTEditListBoxToolBar::GetUpButton() {
	return m_arButton[2];
}
AFX_INLINE CXTButton& CXTEditListBoxToolBar::GetDownButton() {
	return m_arButton[3];
}
AFX_INLINE void CXTEditListBoxToolBar::EnableEdit(bool bEnable) {
	m_bEnableEdit = bEnable;
}

// forwards

class CXTItemEdit;

const DWORD LBS_XT_DEFAULT         = 0x0000;  //<ALIAS CXTEditListBox::SetListEditStyle@UINT@DWORD>
const DWORD LBS_XT_CHOOSEDIR       = 0x0001;  //<ALIAS CXTEditListBox::SetListEditStyle@UINT@DWORD>
const DWORD LBS_XT_CHOOSEFILE      = 0x0002;  //<ALIAS CXTEditListBox::SetListEditStyle@UINT@DWORD>
const DWORD LBS_XT_NOTOOLBAR       = 0x0008;  //<ALIAS CXTEditListBox::SetListEditStyle@UINT@DWORD>

//===========================================================================
// Summary:
//     CXTEditListBox is a CXTListBox derived class. It is used to create an
//     editable list box. This list box can be configured to display a toolbar
//     for editing. You can define browse styles to search for files or folders.
//     Each entry is made editable with a double mouse click.
//===========================================================================
class _XT_EXT_CLASS CXTEditListBox : public CXTListBox
{
	DECLARE_DYNAMIC(CXTEditListBox)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTEditListBox object
	//-----------------------------------------------------------------------
	CXTEditListBox();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTEditListBox object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTEditListBox();

public:

	// -----------------------------------------------------------------------------
	// Summary:
	//     Sets the edit style for the edit list box.
	// Parameters:
	//     lpszTitle -  NULL terminated string that represents the caption title.
	//     nTitle -     Resource ID of the string to load for the caption title.
	//     dwLStyle -   Style for the list edit control. Pass in LBS_XT_NOTOOLBAR
	//                  if you do not wish the caption edit navigation control bar
	//                  to be displayed.
	// Remarks:
	//     Call this member function to set the style and title for the edit
	//     list box. The style of the edit list box can be set to one or more
	//     of the following values:<p/>
	//
	//     * <b>LBS_XT_DEFAULT</b> Standard edit field.
	//     * <b>LBS_XT_CHOOSEDIR</b> Choose directory browse edit field.
	//     * <b>LBS_XT_CHOOSEFILE</b> Choose file browse edit field.
	//     * <b>LBS_XT_NOTOOLBAR</b> Do not display edit toolbar.
	// -----------------------------------------------------------------------------
	void SetListEditStyle(UINT nTitle, DWORD dwLStyle=LBS_XT_DEFAULT);
	void SetListEditStyle(LPCTSTR lpszTitle, DWORD dwLStyle=LBS_XT_DEFAULT); //<combine CXTEditListBox::SetListEditStyle@UINT@DWORD>

	// --------------------------------------------------------------------
	// Summary:
	//     Retrieves the current item index.
	// Returns:
	//     An integer value that represents the edit control index.
	// Remarks:
	//     Call this member function to get the current index for the edit
	//     control. Similar to GetCurSel; however, the current index is the
	//     index of the last item to be modified or added to the edit list
	//     box and not necessarily the selected item.
	// --------------------------------------------------------------------
	int GetCurrentIndex();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will enable editing for the list box item.
	// Parameters:
	//     iItem - Index of the item to edit.
	//-----------------------------------------------------------------------
	void EditItem(int iItem);

	// --------------------------------------------------------------------
	// Summary:
	//     Retrieves the edited item's text label.
	// Remarks:
	//     This member function is called to retrieve the text for the item
	//     that is being edited in the list box and save the value to
	//     m_strItemText.
	// --------------------------------------------------------------------
	virtual void GetEditItemText();

	// --------------------------------------------------------------------
	// Summary:
	//     Returns a pointer to the CXTEditListBoxToolBar toolbar.
	// Returns:
	//     A reference to a CXTEditListBoxToolBar object.
	// Remarks:
	//     Call this member function to return a reference to the
	//     CXTEditListBoxToolBar control that is associated with the edit list box.
	// --------------------------------------------------------------------
	CXTEditListBoxToolBar& GetEditGroup();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the default filter for the
	//     file dialog.
	// Parameters:
	//     strFilter - Points to a NULL terminated string that represents
	//                 the file filter used by the file open dialog.
	//-----------------------------------------------------------------------
	virtual void SetDlgFilter(LPCTSTR strFilter=NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the edit list has a toolbar.
	// Returns:
	//     true if the toolbar is turned on, otherwise returns false.
	//-----------------------------------------------------------------------
	bool HasToolbar();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will enable or disable editing.
	// Parameters:
	//     bEnable - True to enable editing.
	//-----------------------------------------------------------------------
	void EnableEdit(bool bEnable);

	// ---------------------------------------------------------------------
	// Summary:
	//     Initializes the CXTEditListBox control.
	// Parameters:
	//     bAutoFont -  True to enable automatic font initialization.
	// Remarks:
	//     Call this member function to initialize the list box. This method
	//     should be called directly after creating or sub-classing the
	//     control.
	// ---------------------------------------------------------------------
	virtual void Initialize(bool bAutoFont=true);

	// -------------------------------------------------------------------
	// Summary:
	//     Recalculates the toolbar layout for the CXTEditListBox.
	// Remarks:
	//     Call this member function to correctly reposition the edit list
	//     box toolbar. This will readjust the layout to correctly and
	//     position the toolbar in relation to the list.
	// -------------------------------------------------------------------
	virtual void RecalcLayout();

	// -------------------------------------------------------------
	// Summary:
	//     Sets the default text for new items.
	// Parameters:
	//     strItemDefaultText -  NULL terminated string.
	// Remarks:
	//     Call this member function to set the default text that is
	//     displayed when a new item is added to the edit list box.
	// -------------------------------------------------------------
	void SetNewItemDefaultText(CString strItemDefaultText);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will create the edit group control.
	// Parameters:
	//     bAutoFont - True to enable automatic font initialization.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL CreateEditGroup(bool bAutoFont=true);

	// ----------------------------------------------------------------------
	// Summary:
	//     Enables editing for the currently selected item.
	// Parameters:
	//     bNewItem -  TRUE to add a new item.
	// Remarks:
	//     This member function will enable editing for the currently
	//     selected list box item. If 'bNewItem' is TRUE, a new item is added
	//     to the end of the list box.
	// ----------------------------------------------------------------------
	void EditListItem(BOOL bNewItem);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTEditListBox)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTEditListBox)
	afx_msg void OnEndLabelEdit();
	afx_msg void OnNewItem();
	afx_msg void OnDeleteItem();
	afx_msg void OnMoveItemUp();
	afx_msg void OnMoveItemDown();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcMButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	CWnd* m_pParentWnd;             // Pointer to the parent window.
	CString m_strItemDefaultText;   // Default text used when new items are created.
	int             m_nIndex;       // Current index when edit functions are performed.
	BOOL            m_bNewItem;     // TRUE if a new item is being entered into the list box.
	bool            m_bEnableEdit;  // True if editing is enabled.
	DWORD           m_dwLStyle;     // List edit styles.
	CString         m_strTitle;     // Caption area title.
	CString         m_strFilter;    // Default file filter.
	CString         m_strItemText;  // Current text of a selected item during edit.
	CXTItemEdit*    m_pItemEdit;    // Points to the in-place edit item.
	CXTEditListBoxToolBar    m_editGroup;    // The edit group (toolbar) that appears above the list box.

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTEditListBox::GetCurrentIndex() {
	return m_nIndex;
}
AFX_INLINE CXTEditListBoxToolBar& CXTEditListBox::GetEditGroup() {
	return m_editGroup;
}
AFX_INLINE void CXTEditListBox::SetDlgFilter(LPCTSTR strFilter/*=NULL*/) {
	m_strFilter = strFilter;
}
AFX_INLINE bool CXTEditListBox::HasToolbar() {
	return ((m_dwLStyle & LBS_XT_NOTOOLBAR) == 0);
}
AFX_INLINE void CXTEditListBox::EnableEdit(bool bEnable) {
	m_bEnableEdit = bEnable; m_editGroup.EnableEdit(bEnable);
}
AFX_INLINE void CXTEditListBox::SetNewItemDefaultText(CString strItemDefaultText) {
	m_strItemDefaultText = strItemDefaultText;
}

#endif // #if !defined(__XTEDITLISTBOX_H__)
