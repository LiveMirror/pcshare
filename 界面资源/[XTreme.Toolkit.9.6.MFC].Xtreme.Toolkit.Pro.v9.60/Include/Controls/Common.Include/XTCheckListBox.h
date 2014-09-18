// XTCheckListBox.h : interface for the CXTCheckListBox class.
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
#if !defined(__XTCHECKLISTBOX_H__)
#define __XTCHECKLISTBOX_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// ----------------------------------------------------------------------
// Summary:
//     The CXTCheckListBox class is a CXTListBox derived class. It
//     provides the functionality of Office style checklist box. A
//     "checklist box" displays a list of items such as filenames. Each
//     item in the list has a check box next to it that the user can
//     either check or clear.
//
// Remarks:
//     CXTCheckListBox is only for owner-drawn controls because
//     the list contains more than text strings. At its simplest, a
//     checklist box contains text strings and check boxes, but you do
//     not need to have text at all. For example, you could have a list
//     of small bitmaps with a check box next to each item.
//
//     To create your own checklist box, you must derive your own class
//     from CXTCheckListBox. To derive your own class, write a
//     constructor for the derived class, then call Create.
//
//     If your checklist box is a default checklist box (a list of
//     strings with the default-sized check boxes to the left of each),
//     you can use the default CXTCheckListBox::DrawItem to draw the
//     checklist box. Otherwise, you must override the
//     CListBox::CompareItem function and the CXTCheckListBox::DrawItem
//     and CXTCheckListBox::MeasureItem functions.
// ----------------------------------------------------------------------
class _XT_EXT_CLASS CXTCheckListBox : public CXTListBox
{
	DECLARE_DYNAMIC(CXTCheckListBox)

protected:
	// ----------------------------------------------------------------------
	// Summary:
	//     CCheckListState is a helper class used by CXTCheckListBox
	//     that contains information about the checkbox image that is used by
	//     the check list box.
	// ----------------------------------------------------------------------
	class _XT_EXT_CLASS CCheckListState
	{
	public:

		// -------------------------------------------------------------------
		// Summary:
		//     Constructs a CCheckListState object
		// Parameters:
		//     uBitmapID -  Resource identifier of the bitmap to use for check
		//                  marks.
		// -------------------------------------------------------------------
		CCheckListState(UINT uBitmapID);

		//-----------------------------------------------------------------------
		// Summary:
		//     Destroys a CCheckListState object, handles cleanup and deallocation
		//-----------------------------------------------------------------------
		virtual ~CCheckListState();

	public:
		HBITMAP m_hbitmapCheck; // Handle to the check mark image bitmap.
		CSize   m_sizeCheck;    // Width and height of the check mark bitmap.
	};

private:
	struct CHECK_DATA;


public:
	// -------------------------------------------------------------
	// Summary:
	//     Constructs a CXTCheckListBox object
	// Parameters:
	//     uBitmapID -  Resource identifier of the bitmap to use for
	//                  checkmarks.
	// -------------------------------------------------------------
	CXTCheckListBox(UINT uBitmapID=XT_IDB_CHECKLISTBOX);

	// -----------------------------------------------------------------
	// Summary:
	//     Destroys a CXTCheckListBox object, handles cleanup and
	//     deallocation
	// -----------------------------------------------------------------
	virtual ~CXTCheckListBox();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates the Windows checklist box and attaches it to the CXTListBox
	//     object
	// Parameters:
	//     dwStyle    - Specifies the style of the checklist box. The style
	//                  must be either LBS_OWNERDRAWFIXED (all items in
	//                  the list are the same height) or LBS_OWNERDRAWVARIABLE
	//                  (items in the list are of varying heights). This
	//                  style can be combined with other list-box styles.
	//     rect       - Specifies the checklist-box size and position. Can
	//                  be either a CRect object or a RECT structure.
	//     pParentWnd - Specifies the checklist box’s parent window (usually a
	//                  CDialog object). It must not be NULL.
	//     nID        - Specifies the checklist box’s control ID.
	// Returns:
	//     Nonzero if successful; otherwise 0.
	//-----------------------------------------------------------------------
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	// ---------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the style of check boxes in the
	//     checklist box.
	// Parameters:
	//     nStyle -  Determines the style of check boxes in the checklist box. See
	//               the Remarks section for available styles.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:
	//     * <b>BS_3STATE</b> Same as a check box, except that the box can be dimmed
	//       as well as checked. The dimmed state typically is used to show that a
	//       check box has been disabled.
	//     * <b>BS_AUTO3STATE</b> Same as a three-state check box, except that the box
	//       changes its state when the user selects it.
	//     * <b>BS_AUTOCHECKBOX</b> Same as a check box, except that a check mark
	//       appears in the check box when the user selects the box; the check mark
	//       disappears the next time the user selects the box.
	//     * <b>BS_CHECKBOX</b> Creates a small square that has text displayed to its
	//       right (unless this style is combined with the BS_LEFTTEXT style).
	//     For information on these styles, see Button Styles.
	// ---------------------------------------------------------------------------
	void SetCheckStyle(UINT nStyle);

	// ------------------------------------------------------------
	// Summary:
	//     Retrieves the CXTCheckListBox style.
	// Remarks:
	//     Call this function to get the checklist box’s style. For
	//     information on possible styles, see SetCheckStyle.
	// Returns:
	//     The style of the control’s check boxes.
	// See Also:
	//     SetCheckStyle
	// ------------------------------------------------------------
	UINT GetCheckStyle();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the check box of the item specified
	//     by nIndex.
	// Parameters:
	//     nIndex - Index of the item whose check box is to be set.
	//     nCheck - State of the check box: 0 for clear, 1 for checked,
	//              and 2 for indeterminate.
	//-----------------------------------------------------------------------
	void SetCheck(int nIndex, int nCheck);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to determine the check state of an item.
	// Parameters:
	//     nIndex - Index of the item whose check status is to be retrieved.
	// Returns:
	//     Zero if the item is not checked, 1 if it is checked, and 2 if it is
	//     indeterminate.
	//-----------------------------------------------------------------------
	int GetCheck(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to enable or disable a checklist box item.
	// Parameters:
	//     nIndex - Index of the checklist box item to be enabled.
	//     bEnabled - Specifies whether the item is enabled or disabled.
	//-----------------------------------------------------------------------
	void Enable(int nIndex, BOOL  = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to determine whether or not an item is enabled.
	// Parameters:
	//     nIndex - Index of the item.
	// Returns:
	//     Nonzero if the item is enabled; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL IsEnabled(int nIndex);

	// ---------------------------------------------------------------------------------
	// Summary:
	//     The framework calls this function to get the position and size of
	//     the check box in an item.
	// Parameters:
	//     rectItem -      The position and size of the list item.
	//     rectCheckBox -  The default position and size of an item's check
	//                     box.
	// Remarks:
	//     The default implementation only returns the default position and
	//     size of the check box (rectCheckBox). By default, a check box is
	//     aligned in the upper-left corner of an item and is the standard
	//     check box size. There may be cases where you want the check boxes
	//     on the right or want a larger or smaller check box. In these
	//     cases, override OnGetCheckPosition to change the check box
	//     position and size within the item.
	// Example:
	//     The following function overrides the default and puts the check box
	//     on the right of the item, makes it the same height as the item
	//     (minus a pixel offset at the top and bottom), and makes it the
	//     standard check box width:
	// <code>
	// CRect CMyCheckListBox::OnGetCheckPosition(CRect rectItem, CRect rectCheckBox)
	// {
	//    CRect rectMyCheckBox;
	//    rectMyCheckBox.top = rectItem.top -1;
	//    rectMyCheckBox.bottom = rectItem.bottom -1;
	//    rectMyCheckBox.right = rectItem.right -1;
	//    rectMyCheckBox.left = rectItem.right -1 - rectCheckBox.Width();
	//    return rectMyCheckBox;
	// }
	// </code>
	// Returns:
	//     The position and size of an item's check box.
	// ---------------------------------------------------------------------------------
	virtual CRect OnGetCheckPosition(CRect rectItem, CRect rectCheckBox);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the height for the font used by the check list box.
	// Returns:
	//     An integer that represents the font height.
	//-----------------------------------------------------------------------
	virtual int GetFontHeight();

	// ---------------------------------------------------------------------------------------
	// Summary:
	//     Called by the framework to determine the relative position of a
	//     new item in the check list box.
	// Parameters:
	//     lpCompareItemStruct -  Pointer to a COMPAREITEMSTRUCT struct.
	// Returns:
	//     Indicates the relative position of the two items described in the COMPAREITEMSTRUCT
	//     structure. It may be any of the following values:
	// <code>
	// –1 Item 1 sorts before item 2.
	//  0 Item 1 and item 2 sort the same.
	//  1 Item 1 sorts after item 2.
	// </code>
	// See CWnd::OnCompareItem for a description of the COMPAREITEMSTRUCT
	//     structure.
	// ---------------------------------------------------------------------------------------
	virtual int PreCompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);

	// --------------------------------------------------------------------
	// Summary:
	//     Called by the check list box to determine the minimum height for
	//     each item in the check list box.
	// Returns:
	//     An integer that represents the minimum item height.
	// --------------------------------------------------------------------
	virtual int CalcMinimumItemHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to determine if a left mouse click occurred
	//     in the check box to the left of a list box item.
	// Parameters:
	//     point    - A CPoint object that specifies the XY point where the
	//                left mouse button was clicked.
	//     bInCheck - TRUE if the current list box item is checked.
	// Returns:
	//     The index of the list box item that was checked.  If no list box
	//     item was checked, then return -1.
	//-----------------------------------------------------------------------
	virtual int CheckFromPoint(CPoint point, BOOL& bInCheck);

	// ---------------------------------------------------------------------------------------
	// Summary:
	//     Called to paint the control to an off screen device context for a
	//     themed application.
	// Parameters:
	//     lpDrawItemStruct -  Contains information about the item to draw. The DRAWITEMSTRUCT
	//                         structure provides information the owner window must have to
	//                         determine how to paint an owner\-drawn control or menu item.
	// <code>
	// typedef struct tagDRAWITEMSTRUCT {
	//     UINT   CtlType;
	//     UINT   CtlID;
	//     UINT   itemID;
	//     UINT   itemAction;
	//     UINT   itemState;
	//     HWND   hwndItem;
	//     HDC    hDC;
	//     RECT   rcItem;
	//     DWORD  itemData;
	// } DRAWITEMSTRUCT;
	// </code>
	// Remarks:
	//     Call this function to pre-draw the item to a memory DC or to the
	//     ThemeManager. This prevents flickering of the window during
	//     drawing.
	// ---------------------------------------------------------------------------------------
	virtual void PreDrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// -------------------------------------------------------------------------------------------
	// Summary:
	//     This function is called to measure the size of the item in the
	//     list box for the proper handling of an owner drawn list box.
	// Parameters:
	//     lpMeasureItemStruct -  A LPMEASUREITEMSTRUCT struct. The MEASUREITEMSTRUCT structure
	//                            informs Windows of the dimensions of an owner\-drawn control or
	//                            menu item. This allows Windows to process user interaction with
	//                            the control correctly. Failure to fill out the proper members in
	//                            the MEASUREITEMSTRUCT structure will cause improper operation of
	//                            the control.
	//
	// <code>
	// typedef struct tagMEASUREITEMSTRUCT {
	//     UINT   CtlType;
	//     UINT   CtlID;
	//     UINT   itemID;
	//     UINT   itemWidth;
	//     UINT   itemHeight;
	//     DWORD  itemData
	// } MEASUREITEMSTRUCT;
	// </code>
	//
	//                            itemWidth Specifies the width of a menu item. The
	//                            owner of the owner\-draw menu item must fill this member before
	//                            it returns from the message.
	//
	//                            itemHeight Specifies the height of an individual item in
	//                            a list box or a menu. Before it returns from the message, the owner
	//                            of the owner\-draw combo box, list box, or menu item must fill out
	//                            this member. The maximum height of a list box item is 255.
	// -------------------------------------------------------------------------------------------
	virtual void PreMeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	// -----------------------------------------------------------------------------------------
	// Summary:
	//     This function is called so that an owner drawn list box control
	//     can properly handle the delete item message WM_DELETEITEM.
	// Parameters:
	//     lpDeleteItemStruct -  A LPDELETEITEMSTRUCT struct that contains information about the
	//                           items to delete. The DELETEITEMSTRUCT structure has the
	//                           following form\:
	//
	// <code>
	// typedef struct tagDELETEITEMSTRUCT { /* ditms */
	//     UINT CtlType;
	//     UINT CtlID;
	//     UINT itemID;
	//     HWND hwndItem;
	//     UINT itemData;
	// } DELETEITEMSTRUCT;
	// </code>
	//
	//                           The DELETEITEMSTRUCT structure describes a deleted
	//                           owner\-drawn list\-box or combo\-box item.
	// -----------------------------------------------------------------------------------------
	virtual void PreDeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check all items in the list box that are
	//     currently highlighted and not disabled.
	// Parameters:
	//     nCheck - Integer value that specifies the check state.  1 for
	//              checked, 0 for not checked, and 2 for intermediate.
	//-----------------------------------------------------------------------
	virtual void SetSelectionCheck( int nCheck );

	// ----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTCheckListBox to redraw a check box.
	// Parameters:
	//     nIndex -  Integer value that specifies the index of the value to
	//               invalidate.
	// Remarks:
	//     This member function determines the size of the bounding rectangle
	//     of the selected item's check box. Then calls invalidate rectangle
	//     so that the background of the rectangle is redrawn during the next
	//     WM_PAINT message.
	// ----------------------------------------------------------------------
	virtual void InvalidateCheck(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTCheckListBox to redraw an item.
	// Parameters:
	//     nIndex - Integer value that specifies the index of the value to
	//              invalidate.
	// Remarks:
	//     This member function determines the size of the bounding rectangle of
	//     the selected item.  Then calls invalidate rectangle so that the item's
	//     background is redrawn during the next WM_PAINT message.
	//-----------------------------------------------------------------------
	virtual void InvalidateItem(int nIndex);

	// -------------------------------------------------------------------------
	// Summary:
	//     Called to paint the control to an off screen device context for a
	//     themed application.
	// Parameters:
	//     pDC -       A CDC pointer that represents the current device context.
	// drawItem -  A DRAWITEMSTRUCT that contains information about the item to
	//                 draw. This is necessary because the list box is an owner
	//                 drawn control.
	// nCheck -    Integer value that specifies the check state. 1 for checked,
	//                 0 for not checked, and 2 for intermediate.
	// cyItem -    The vertical position of the item in the list box.
	// Remarks:
	//     Call this function to pre-draw an item in a themed application.
	//     This is used to prevent flickering when re-drawing the control.
	// Returns:
	//     \Returns true if the item is successfully drawn, otherwise return
	//     false. Also, if the application is not themed, then this function
	//     \returns false.
	// -------------------------------------------------------------------------
	bool PreDrawItemThemed(CDC* pDC, DRAWITEMSTRUCT &drawItem, int nCheck, int cyItem);

	// -------------------------------------------------------------------------
	// Summary:
	//     Called to paint the control to an off screen device context for a
	//     non-themed application.
	// Parameters:
	//     pDC -       A CDC pointer that represents the current device context.
	// drawItem -  A DRAWITEMSTRUCT that contains information about the item to
	//                 draw. This is necessary because the list box is an owner
	//                 drawn control.
	// nCheck -    Integer value that specifies the check state. 1 for checked,
	//                 0 for not checked, and 2 for intermediate.
	// cyItem -    The vertical position of the item in the list box.
	// Remarks:
	//     Call this function to pre-draw an item in a non-themed
	//     application. This is used to prevent flickering when re-drawing
	//     the control.
	// -------------------------------------------------------------------------
	void PreDrawItemNonThemed(CDC* pDC, DRAWITEMSTRUCT &drawItem, int nCheck, int cyItem);

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTCheckListBox)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual BOOL OnChildNotify(UINT, WPARAM, LPARAM, LRESULT*);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTCheckListBox)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLBAddString(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLBFindString(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLBFindStringExact(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLBGetItemData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLBGetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLBInsertString(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLBSelectString(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLBSetItemData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLBSetItemHeight(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:

	int               m_cyText;         // Represents the text height for the check list box.
	UINT              m_nStyle;         // Specifies the style for the check list box.
	CCheckListState   m_checkListState; // Holds the check mark image information.

};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE UINT CXTCheckListBox::GetCheckStyle() {
	return m_nStyle;
}
AFX_INLINE BOOL CXTCheckListBox::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

#endif // #if !defined(__XTCHECKLISTBOX_H__)
