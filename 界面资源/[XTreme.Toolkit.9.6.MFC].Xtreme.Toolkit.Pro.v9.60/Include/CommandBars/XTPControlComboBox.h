// XTPControlComboBox.h : interface for the CXTPControlComboBox class.
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
#if !defined(__XTPCONTOLCOMBOBOX_H__)
#define __XTPCONTOLCOMBOBOX_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPControl.h"
#include "XTPControlPopup.h"
#include "XTPPopupBar.h"

class CXTPControlComboBox;

//===========================================================================
// Summary:
//     CXTPEdit is a CEdit derived class. It is for internal usage only.
//===========================================================================
class _XTP_EXT_CLASS CXTPEdit : public CEdit
{
	DECLARE_DYNCREATE(CXTPEdit)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework to route and dispatch command messages
	//     and to handle the update of command user-interface objects.
	// Parameters:
	//     nID          - Contains the command ID.
	//     nCode        - Identifies the command notification code.
	//     pExtra       - Used according to the value of nCode.
	//     pHandlerInfo - If not NULL, OnCmdMsg fills in the pTarget and
	//                    pmf members of the pHandlerInfo structure instead
	//                    of dispatching the command. Typically, this parameter
	//                    should be NULL.
	// Returns:
	//     Nonzero if the message is handled; otherwise 0.
	//-----------------------------------------------------------------------
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function when the user selects
	//     an item from a menu, when a child control sends a notification
	//     message, or when an accelerator keystroke is translated.
	// Parameters:
	//     wParam - The low-order word of wParam identifies the command
	//              ID of the menu item, control, or accelerator. The
	//              high-order word of wParam specifies the notification
	//              message if the message is from a control. If the message
	//              is from an accelerator, the high-order word is 1. If
	//              the message is from a menu, the high-order word is 0.
	// Returns:
	//     An application returns nonzero if it processes this message;
	//     otherwise 0.
	//-----------------------------------------------------------------------
	virtual BOOL OnCommand(WPARAM wParam, LPARAM);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function displays a popup context menu.
	// Parameters:
	//     pControl - Pointer to a CXTPControl control.
	//     point - CPoint object specifies xy coordinates.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL ShowContextMenu(CXTPControl* pControl, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function displays a popup context menu.
	// Parameters:
	//     dwStyle - Specifies object's style flags.
	//     pParentWnd - Pointer to the parent window.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL CreateEdit(DWORD dwStyle, CWnd* pParentWnd);

public:
	static BOOL m_bRichEditClass;   // TRUE to use RichEdit class instead Edit.
};


//===========================================================================
// Summary:
//     CXTPControlComboBoxList is a CXTPPopupBar derived class.
//     It represents a list box of CXTPControlComboBox control.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlComboBoxList : public CXTPPopupBar
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlButton object
	//-----------------------------------------------------------------------
	CXTPControlComboBoxList() { m_barPosition = xtpBarListBox; m_nMRUWidth = 0;};

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by WindowProc, or is called during
	//                message reflection.
	// Parameters:
	//     hWnd     - Window handle that the message belongs to.
	//     nMessage - Specifies the message to be sent.
	//     wParam   - Specifies additional message-dependent information.
	//     lParam   - Specifies additional message-dependent information.
	//     lResult  - The return value of WindowProc. Depends on the message;
	//                may be NULL.
	//-----------------------------------------------------------------------
	int OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called, then PopupBar becomes visible.
	// Parameters:
	//     pControlPopup - Points to a CXTPControlPopup object
	//     bSelectFirst  - TRUE to select the first item.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Popup(CXTPControlPopup* pControlPopup, BOOL bSelectFirst = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to change the tracking state.
	// Parameters:
	//     bMode - TRUE to set the tracking mode; otherwise FALSE.
	//     bSelectFirst - TRUE to select the first item.
	//     bKeyboard    - TRUE if the item is popuped by the keyboard.
	// See Also: IsTrackingMode.
	// Returns:
	//     TRUE if the method was successful.
	//-----------------------------------------------------------------------
	virtual BOOL SetTrackingMode(int bMode, BOOL bSelectFirst, BOOL bKeyboard = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the command bar in the given context.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	virtual void DrawCommandBar(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function when a non-system key
	//     is pressed.
	// Parameters:
	//     nChar - Specifies the virtual key code of the given key.
	// Returns:
	//     TRUE if key handled, otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL OnKeyDown(UINT nChar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the customize mode of the command
	//     bars.
	// Returns:
	//     TRUE if command bars are in customized mode; otherwise returns
	//     FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsCustomizable() { return FALSE; }

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method makes a copy of the command bar.
	// Parameters:
	//     pCommandBar - Command bar needed to be copied.
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	virtual void Copy(CXTPCommandBar* pCommandBar, BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates list box.
	//-----------------------------------------------------------------------
	virtual void CreateListBox();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPControlComboBoxList)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE


private:
	DECLARE_XTP_COMMANDBAR(CXTPControlComboBoxList)
	friend class CXTPControlComboBox;
};

//////////////////////////////////////////////////////////////////////////

//===========================================================================
// Summary:
//     Inplace Edit control of the combo.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlComboBoxEditCtrl : public CXTPEdit
{

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves parent CXTPControlComboBox object.
	// Returns:
	//     Pointer to parent CXTPControlComboBox.
	//-----------------------------------------------------------------------
	CXTPControlComboBox* GetControlComboBox();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPControlComboBoxEditCtrl)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd *pWnd, CPoint point);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPControlComboBox* m_pControl;        // Parent Combo Box.

private:
	friend class CXTPControlComboBox;
};


//-----------------------------------------------------------------------
// Summary:
//     CXTPControlComboBox is a CXTPControl derived class. It represents a combo box control.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPControlComboBox : public CXTPControlPopup
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlComboBox object
	//-----------------------------------------------------------------------
	CXTPControlComboBox(void);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControlComboBox object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPControlComboBox(void);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the style of the combo box.
	// Parameters:
	//     comboStyle - The style to be set. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     comboStyle parameter can be one of the following:
	//         * <b>xtpComboNormal</b> Normal combo box.
	//         * <b>xtpComboLabel</b> Combo box with label.
	// See Also: XTPComboStyle, GetStyle
	//-----------------------------------------------------------------------
	void SetStyle(XTPComboStyle comboStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the control's style.
	// Returns:
	//     The style of the control.
	// See Also: XTPComboStyle
	//-----------------------------------------------------------------------
	XTPComboStyle GetStyle();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to insert\delete an edit box in the combo box
	//     when the control has focus.
	// Parameters:
	//     bSet - TRUE if the combo box has an edit control.
	// Remarks:
	//     If bSet is TRUE, when the combo box control is click an edit
	//     control is used to display the text and the user can edit or
	//     copy the text.
	// See Also: GetDropDownListStyle
	//-----------------------------------------------------------------------
	void SetDropDownListStyle(BOOL bSet = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the combo box has an edit control.
	// Returns:
	//     TRUE is the combo box has an edit control, FALSE otherwise.
	// See Also: SetDropDownListStyle
	//-----------------------------------------------------------------------
	BOOL GetDropDownListStyle();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the width of the dropdown list.
	// Parameters:
	//     nWidth - The width of the dropdown list.
	//-----------------------------------------------------------------------
	void SetDropDownWidth(int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the dropdown list width.
	// Returns:
	//     Width of the dropdown list.
	//-----------------------------------------------------------------------
	int GetDropDownWidth();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add a string to a list box.
	// Parameters:
	//     str - The string that is to be added.
	// Returns:
	//     The zero-based index of the string in the list box. The return value is LB_ERR
	//     if an error occurs.
	//-----------------------------------------------------------------------
	int AddString(CString str);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the number of items in a list box.
	// Returns:
	//     The number of items in the list box, or LB_ERR if an error occurs.
	//-----------------------------------------------------------------------
	int GetCount();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve a string from the list box of a combo box control.
	// Parameters:
	//     nIndex - Contains the zero-based index of the list-box string to be copied.
	//     str - A reference to a CString.
	//-----------------------------------------------------------------------
	void GetLBText(int nIndex, CString& str);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to removes all items from the list box of a combo box control.
	//-----------------------------------------------------------------------
	void ResetContent();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call the FindStringExact member function to find the first list-box string
	//     (in a combo box) that matches the string specified in str.
	// Parameters:
	//     nIndexStart - Specifies the zero-based index of the item before the first item
	//     to be searched.
	//     str - The string to search for.
	// Returns:
	//     The zero-based index of the matching item, or CB_ERR if the search was
	//     unsuccessful.
	// See Also: FindString
	//-----------------------------------------------------------------------
	int FindStringExact(int nIndexStart, CString str);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to insert a string into the list box of a combo box control
	// Parameters:
	//     nIndex - Contains the zero-based index to the position in the list box that will receive
	//     the string.
	//     str - The string that is to be inserted.
	//-----------------------------------------------------------------------
	int InsertString(int nIndex, CString str);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the width of the control
	// Parameters:
	//     nWidth - The width of the control
	//-----------------------------------------------------------------------
	void SetWidth(int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the width of the control
	// Returns:
	//     Width of the control
	//-----------------------------------------------------------------------
	int GetWidth();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the currently selected item's text
	// Returns:
	//     The text that is currently selected.
	//-----------------------------------------------------------------------
	virtual CString GetText();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the edit control of the combo box control.
	// Returns:
	//     A pointer to the CEdit control.
	//-----------------------------------------------------------------------
	CXTPControlComboBoxEditCtrl* GetEditCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to select a string in the list box of a combo box.
	// Parameters:
	//     nIndex - Specifies the zero-based index of the string to select.
	//-----------------------------------------------------------------------
	void SetCurSel(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine which item in the combo box is selected.
	// Returns:
	//     The zero-based index of the currently selected item in the list box of a combo box,
	//     or CB_ERR if no item is selected.
	//-----------------------------------------------------------------------
	int GetCurSel();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call the GetDroppedState member function to determine whether the list box of a drop-down
	//     combo box is visible (dropped down).
	// Returns:
	//     Nonzero if the list box is visible; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL GetDroppedState();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the edit text.
	// Returns:
	//     The Edit control text.
	//-----------------------------------------------------------------------
	CString GetEditText();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the edit control text.
	// Parameters:
	//     lpszText    - New text of the edit control.
	//     bMatchExact - TRUE if FindStringExact needs to use instead of FindString
	//                   to update current index of list box
	// See Also:
	//     FindStringExact, FindString
	//-----------------------------------------------------------------------
	void SetEditText(LPCTSTR lpszText,BOOL bMatchExact = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method finds the first string in a list box that contains the specified prefix,
	//     without changing the list-box selection
	// Parameters:
	//     nStartAfter - Contains the zero-based index of the item before the first item to be
	//                   searched. When the search reaches the bottom of the list box, it continues from the
	//                   top of the list box back to the item specified by nStartAfter. If nStartAfter is –1,
	//                   the entire list box is searched from the beginning.
	//     lpszItem    - Points to the null-terminated string that contains the prefix to search for.
	//                   The search is case independent, so this string may contain any combination of uppercase
	//                   and lowercase letters.
	// Returns:
	//     The zero-based index of the matching item, or LB_ERR if the search was unsuccessful.
	// See Also: FindStringExact
	//-----------------------------------------------------------------------
	int FindString(int nStartAfter, LPCTSTR lpszItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves the application-supplied 32-bit value associated with the
	//     specified combo box item.
	// Parameters:
	//     nIndex - Contains the zero-based index of an item in the combo box’s list box.
	// Returns:
	//     The 32-bit value associated with the item, or CB_ERR if an error occurs.
	//-----------------------------------------------------------------------
	DWORD_PTR GetItemData(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the 32-bit value associated with the specified item in a combo box.
	// Parameters:
	//     nIndex     - Contains a zero-based index of the item to set.
	//     dwItemData - Contains the new value to associate with the item.
	// Returns:
	//     CB_ERR if an error occurs.
	//-----------------------------------------------------------------------
	int SetItemData(int nIndex, DWORD_PTR dwItemData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to delete a string.
	// Parameters:
	//     nIndex - Contains a zero-based index of the item to delete.
	//-----------------------------------------------------------------------
	void DeleteItem(long nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable or disable the control.
	//     If the control does not have flags xtpFlagManualUpdate, you must call
	//     the Enable member of CCmdUI in the ON_UPDATE_COMMAND_UI handler.
	// Parameters:
	//     bEnabled - TRUE if the control is enabled.
	// See Also: GetEnabled, SetChecked
	//-----------------------------------------------------------------------
	void SetEnabled(BOOL bEnabled);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to compare controls.
	// Parameters:
	//     pOther - The control need compare with.
	// Returns:
	//     TRUE if the controls are identical.
	//-----------------------------------------------------------------------
	virtual BOOL Compare(CXTPControl* pOther);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get with of label.
	// Returns:
	//     Width of label of edit control.
	// See Also: SetLabelWidth, SetStyle, GetStyle
	//-----------------------------------------------------------------------
	int GetLabelWidth();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to set width of the label.
	// Parameters:
	//     nLabelWidth - Width of label to be set
	// See Also: GetLabelWidth, SetStyle, GetStyle
	//-----------------------------------------------------------------------
	void SetLabelWidth(int nLabelWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get with of thumb button.
	// Returns:
	//     Width of thumb button of combo box.
	// See Also: SetThumbWidth
	//-----------------------------------------------------------------------
	int GetThumbWidth();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to set width of thumb button.
	// Parameters:
	//     nThumbWidth - Width of the thumb button to be set.
	// See Also: GetThumbWidth
	//-----------------------------------------------------------------------
	void SetThumbWidth(int nThumbWidth);




protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method create edit control. Override it to use inherited edit control.
	//-----------------------------------------------------------------------
	virtual CXTPControlComboBoxEditCtrl* CreateEditControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Called after the mouse hovers over the control.
	//-----------------------------------------------------------------------
	void OnMouseHover();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called, then edit control gets the focus.
	// Parameters:
	//     pOldWnd - Points to a CWnd object
	//-----------------------------------------------------------------------
	virtual void OnSetFocus(CWnd* pOldWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called, then the edit control loses the focus.
	//-----------------------------------------------------------------------
	virtual void OnKillFocus();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called, then the selected string is changed.
	//-----------------------------------------------------------------------
	virtual void OnSelChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function when a child control is about to be drawn.
	// Parameters:
	//     pDC       - Contains a pointer to the display context for the child window.
	//     nCtlColor - Must be CTLCOLOR_EDIT for edit.
	// Returns:
	//     OnCtlColor must return a handle to the brush that is to be used for painting the control
	//     background.
	//-----------------------------------------------------------------------
	virtual HBRUSH OnCtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/) { return 0;}

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the control becomes selected.
	// Parameters:
	//     bSelected - TRUE if the control becomes selected.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL OnSetSelected(int bSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the bounding rectangle of the control.
	// Parameters:
	//     rcControl - Bounding rectangle of the control.
	//-----------------------------------------------------------------------
	void SetRect(CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to hide the control.
	// Parameters:
	//     dwFlags - Reasons to hide.
	// See Also: XTPControlHideFlags
	//-----------------------------------------------------------------------
	void SetHideFlags(DWORD dwFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user clicks the control.
	// Parameters:
	//     bKeyboard - TRUE if the control is selected using the keyboard.
	//     pt - Mouse cursor position.
	//-----------------------------------------------------------------------
	void OnClick(BOOL bKeyboard = FALSE, CPoint pt = CPoint(0, 0));

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to copy the control.
	// Parameters:
	//     pControl - Points to a source CXTPControl object
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	void Copy(CXTPControl* pControl, BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a non-system key is pressed.
	// Parameters:
	//     nChar - Specifies the virtual key code of the given key.
	// Returns:
	//     TRUE if key handled, otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL OnKeyDown(UINT nChar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to assign a parent command bar object.
	// Parameters:
	//     pParent - Points to a CXTPCommandBar object
	//-----------------------------------------------------------------------
	void SetParent(CXTPCommandBar* pParent);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before recalculating the parent command
	//     bar size to calculate the dimensions of the control.
	// Parameters:
	//     dwMode - Flags used to determine the height and width of the
	//              dynamic command bar. See Remarks section for a list of
	//              values.
	// Remarks:
	//     The following predefined flags are used to determine the height and
	//     width of the dynamic command bar. Use the bitwise-OR (|) operator to
	//     combine the flags.<p/>
	//
	//     * <b>LM_STRETCH</b> Indicates whether the command bar should be
	//                stretched to the size of the frame. Set if the bar is
	//                not a docking bar (not available for docking). Not set
	//                when the bar is docked or floating (available for
	//                docking). If set, LM_STRETCH returns dimensions based
	//                on the LM_HORZ state. LM_STRETCH works similarly to
	//                the the bStretch parameter used in CalcFixedLayout;
	//                see that member function for more information about
	//                the relationship between stretching and orientation.
	//     * <b>LM_HORZ</b> Indicates that the bar is horizontally or
	//                vertically oriented. Set if the bar is horizontally
	//                oriented, and if it is vertically oriented, it is not
	//                set. LM_HORZ works similarly to the the bHorz
	//                parameter used in CalcFixedLayout; see that member
	//                function for more information about the relationship
	//                between stretching and orientation.
	//     * <b>LM_MRUWIDTH</b> Most Recently Used Dynamic Width. Uses the
	//                remembered most recently used width.
	//     * <b>LM_HORZDOCK</b> Horizontal Docked Dimensions. Returns the
	//                dynamic size with the largest width.
	//     * <b>LM_VERTDOCK</b> Vertical Docked Dimensions. Returns the dynamic
	//                size with the largest height.
	//     * <b>LM_COMMIT</b> Resets LM_MRUWIDTH to current width of
	//                floating command bar.
	//
	//     The framework calls this member function to calculate the dimensions
	//     of a dynamic command bar.<p/>
	//
	//     Override this member function to provide your own layout in classes
	//     you derive from CXTPControl. XTP classes derived from CXTPControl,
	//     such as CXTPControlComboBox, override this member function to provide
	//     their own implementation.
	// See Also:
	//     CXTPControl, CXTPControlCustom, CXTPControlEdit,
	//     CXTPControlWindowList, CXTPControlWorkspaceActions, CXTPControlToolbars,
	//     CXTPControlOleItems, CXTPControlRecentFileList, CXTPControlSelector,
	//     CXTPControlListBox
	//-----------------------------------------------------------------------
	virtual void OnCalcDynamicSize(DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to popup the control.
	// Parameters:
	//     bPopup - TRUE to set popup.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL OnSetPopup(BOOL bPopup);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the control is executed.
	//-----------------------------------------------------------------------
	virtual void OnExecute();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member checks if the user can resize control.
	// Returns:
	//     TRUE if resize available.
	//-----------------------------------------------------------------------
	virtual BOOL IsCustomizeResizeAllow();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member returns the minimum width that the combo box
	//     can be sized by the user while in customization mode.
	// Returns:
	//     Width of label + Width of Dropdown button + 5
	//-----------------------------------------------------------------------
	virtual int GetCustomizeMinWidth();

protected:

	CXTPControlComboBoxEditCtrl* m_pEdit;       // Child edit control.
	BOOL                         m_bDropDown;   // TRUE if the combo is dropdown.
	XTPComboStyle                m_comboStyle;  // Style of the combo box.
	int                          m_nWidth;      // Width of the combo box.
	CString                      m_strEditText; // Edit text.
	int                          m_nLastSel;    // last user selected index, (used during display of list box)
	BOOL                         m_bDelayReposition; // Need to reposition control.
	int                          m_nLabelWidth; // Width of the label.
	int                          m_nThumbWidth; // Width of the thumb area.

private:

	DECLARE_XTP_CONTROL(CXTPControlComboBox)

	friend class CXTPControlComboBoxList;
	friend class CXTPControlComboBoxEditCtrl;

};

//////////////////////////////////////////////////////////////////////////


AFX_INLINE void CXTPControlComboBox::SetStyle(XTPComboStyle comboStyle) {
	m_comboStyle = comboStyle;
}
AFX_INLINE XTPComboStyle CXTPControlComboBox::GetStyle() {
	return m_comboStyle;
}

AFX_INLINE void CXTPControlComboBox::SetDropDownWidth(int nWidth) {
	m_pCommandBar->SetWidth(nWidth);
}
AFX_INLINE int CXTPControlComboBox::AddString(CString str) {
	return ((CListBox*)m_pCommandBar)->AddString(str);
}
AFX_INLINE int CXTPControlComboBox::GetCount() {
	return ((CListBox*)m_pCommandBar)->GetCount();
}
AFX_INLINE void CXTPControlComboBox::GetLBText(int nIndex, CString& str) {
	((CListBox*)m_pCommandBar)->GetText(nIndex, str);
}
AFX_INLINE void CXTPControlComboBox::ResetContent() {
	((CListBox*)m_pCommandBar)->ResetContent();
}
AFX_INLINE int CXTPControlComboBox::FindStringExact(int nIndexStart, CString str) {
	return ((CListBox*)m_pCommandBar)->FindStringExact(nIndexStart, str);
}
AFX_INLINE int CXTPControlComboBox::InsertString(int nIndex, CString str) {
	return ((CListBox*)m_pCommandBar)->InsertString(nIndex, str);
}
AFX_INLINE void CXTPControlComboBox::SetWidth(int nWidth) {
	if (m_nWidth != nWidth)
	{	
		m_nWidth = nWidth;
		DelayLayoutParent();
	}
}
AFX_INLINE int CXTPControlComboBox::GetWidth() {
	return m_nWidth;
}
AFX_INLINE CXTPControlComboBoxEditCtrl* CXTPControlComboBox::GetEditCtrl() {
	return m_pEdit;
}
AFX_INLINE int CXTPControlComboBox::FindString(int nStartAfter, LPCTSTR lpszItem) const {
	return ((CListBox*)m_pCommandBar)->FindString(nStartAfter, lpszItem);
}
AFX_INLINE DWORD_PTR CXTPControlComboBox::GetItemData(int nIndex) const {
	return (DWORD_PTR)((CListBox*)m_pCommandBar)->GetItemData(nIndex);
}
AFX_INLINE int CXTPControlComboBox::SetItemData(int nIndex, DWORD_PTR dwItemData) {
	return ((CListBox*)m_pCommandBar)->SetItemData(nIndex, dwItemData);
}
AFX_INLINE void CXTPControlComboBox::DeleteItem(long nIndex) {
	if (nIndex < GetCount()) ((CListBox*)m_pCommandBar)->DeleteString(nIndex);
}
AFX_INLINE BOOL CXTPControlComboBox::IsCustomizeResizeAllow() {
	return TRUE;
}
AFX_INLINE CXTPControlComboBox* CXTPControlComboBoxEditCtrl::GetControlComboBox() {
	return m_pControl;
}
AFX_INLINE int CXTPControlComboBox::GetLabelWidth() {
	return m_nLabelWidth;
}
AFX_INLINE void CXTPControlComboBox::SetLabelWidth(int nLabelWidth) {
	if (m_nLabelWidth != nLabelWidth)
	{
		m_nLabelWidth = nLabelWidth;
		m_bDelayReposition = TRUE;		
	}
}
AFX_INLINE int CXTPControlComboBox::GetThumbWidth() {
	return m_nThumbWidth;
}
AFX_INLINE void CXTPControlComboBox::SetThumbWidth(int nThumbWidth) {
	if (m_nThumbWidth != nThumbWidth)
	{
		m_nThumbWidth = nThumbWidth;
		m_bDelayReposition = TRUE;		
	}
}

#endif //#if !defined(__XTPCONTOLCOMBOBOX_H__)
