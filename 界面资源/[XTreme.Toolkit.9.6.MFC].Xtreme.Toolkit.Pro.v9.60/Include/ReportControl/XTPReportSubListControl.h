// XTPReportSubListControl.h: interface for the CXTPReportSubListControl class.
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
#if !defined(__XTPREPORTSUBLISTCONTROL_H__)
#define __XTPREPORTSUBLISTCONTROL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPReportHeaderDragWnd;
class CXTPReportControl;

//===========================================================================
// Summary:
//     Field chooser list box control.
//     This control allows the user to move column names from associated
//     CXTPReportControl object from and to its header.
// Remarks:
//     To use a field chooser in an existing dialog box of your application,
//     add a standard list box to your dialog template using the dialog
//     editor and then initialize your CXTPReportSubListControl object
//     inside dialog OnInitialUpdate handler. See example below:
// Example:
// <code>
//  // CXTPReportSubListControl m_wndSubList;
//  wndSubList.SubclassDlgItem(IDC_COLUMNLIST, &pFieldChooserDialog);
//  // CXTPReportControl wndReport;
//  wndReport.GetColumns()->GetReportHeader()->SetSubListCtrl(&wndSubList);
// </code>
//
// See Also:
//     CDragListBox, CXTPReportControl, CXTPReportHeader,
//     CXTPReportHeaderDragWnd, ReportControl sample application
//===========================================================================
class _XTP_EXT_CLASS CXTPReportSubListControl : public CDragListBox
{

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportSubListControl object.
	// Remarks:
	//     You construct a CXTPReportSubListControl object in two steps.
	//     First, call the constructor CXTPReportSubListControl and then call
	//     CListBox::Create, which initializes the Windows list box and
	//     attaches it to the CXTPReportSubListControl.
	// Example:
	// <code>
	// // Declare a local CXTPReportSubListControl object.
	// CXTPReportSubListControl mySubList;
	//
	// // Declare a dynamic CXTPReportSubListControl object.
	// CXTPReportSubListControl* pMySubList = new CXTPReportSubListControl;
	// </code>
	//
	// See Also:
	//     CListBox, CXTPReportSubListControl::SetReportCtrl,
	//     CXTPReportHeader::SetSubListCtrl
	//-----------------------------------------------------------------------
	CXTPReportSubListControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys CXTPReportSubListControl object, handles its cleanup.
	//-----------------------------------------------------------------------
	~CXTPReportSubListControl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets associated report control.
	// Parameters:
	//     pReportCtrl - Pointer to the associated report control.
	// Remarks:
	//     Usually this method should not be called by user, instead
	//     you should call CXTPReportHeader::SetSubListCtrl.
	// Returns:
	//     TRUE if set successfully, FALSE otherwise.
	// See Also: CXTPReportHeader::SetSubListCtrl, GetReportCtrl
	//-----------------------------------------------------------------------
	BOOL SetReportCtrl(CXTPReportControl* pReportCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a pointer to the associated report control.
	// Returns:
	//     An associated report control.
	// See Also: CXTPReportHeader::SetSubListCtrl, SetReportCtrl
	//-----------------------------------------------------------------------
	CXTPReportControl* GetReportCtrl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Updates columns list data in field chooser.
	// Remarks:
	//     Reloads column list from the associated CXTPReportControl object,
	//     deletes all visible column items from the internal list,
	//     and adds the rest of invisible items.
	// Example:
	//     <code>wndSubList.UpdateList();</code>
	// Returns:
	//     TRUE when updated successfully, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL UpdateList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Not used for the moment.
	// Parameters:
	//     iSubItem - Sub-item number for including.
	// Returns:
	//     Non-zero when completed successfully, zero otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL Include(int iSubItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Not used for the moment.
	// Parameters:
	//     iSubItem - Sub-item number for disabling.
	// Returns:
	//     Non-zero when completed successfully, zero otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL Disable(int iSubItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to use sorted list.
	// Parameters:
	//     bSort - TRUE to sort columns in list.
	//-----------------------------------------------------------------------
	void SortAscending(BOOL bSort);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves caption text by its index.
	// Parameters:
	//     nIndex - Index of item to retrieve.
	// Returns:
	//     Caption of the item.
	//-----------------------------------------------------------------------
	CString GetItemCaption(int nIndex);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Finds item index by its data.
	// Parameters:
	//     nItemData - Item data for search.
	// Returns:
	//     Found item index, -1 if item was not found.
	//-----------------------------------------------------------------------
	int FindItemData(int nItemData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework when a drag operation starts.
	// Parameters:
	//     pt - Coordinates of the starting drag position (typically, the cursor position).
	// Remarks:
	//     Override this function if you want to control what happens when
	//     a drag operation begins. This implementation captures the mouse,
	//     creates CXTPReportHeaderDragWnd and stays in drag mode until the
	//     user clicks the left or right mouse button or presses ESC,
	//     at which time the drag operation is canceled.
	// Returns:
	//     Nonzero if dragging is allowed, otherwise 0.
	// See Also: CDragListBox::BeginDrag
	//-----------------------------------------------------------------------
	BOOL BeginDrag(CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework when an item is being dragged within the
	//     CDragListBox object.
	// Parameters:
	//     pt - A CPoint object that contains the x and y screen coordinates of the cursor.
	// Remarks:
	//     This implementation allows dropping items only on the control itself
	//     and associated report control header.
	//     Override this function if you want to provide additional functionality.
	// Returns:
	//     The resource ID of the cursor to be displayed.
	//     The following values are possible:
	//     * <b>DL_COPYCURSOR</b>    Indicates that the item will be copied.
	//     * <b>DL_MOVECURSOR</b>    Indicates that the item will be moved.
	//     * <b>DL_STOPCURSOR</b>    Indicates that the current drop target is not acceptable.
	//
	// See Also: CDragListBox::Dragging
	//-----------------------------------------------------------------------
	UINT Dragging(CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework when a drag operation has been canceled.
	// Parameters:
	//     pt - A CPoint object that contains the coordinates of the item being dragged.
	// Remarks:
	//     Destroys dragging CXTPReportHeaderDragWnd window and resets
	//     associated report control mouse mode.
	// See Also: CDragListBox::CancelDrag
	//-----------------------------------------------------------------------
	void CancelDrag(CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework when an item is dropped within a
	//     CDragListBox object.
	// Parameters:
	//     iSrcIndex - Specifies the zero-based index of the dropped string.
	//     pt        - A CPoint object that contains the coordinates of the drop site.
	// Remarks:
	//     Destroys dragging CXTPReportHeaderDragWnd window and returns the
	//     control to the caller.
	// See Also: CDragListBox::Dropped
	//-----------------------------------------------------------------------
	void Dropped(int iSrcIndex, CPoint pt);


protected:
//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_VIRTUAL(CXTPReportSubListControl)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	virtual void MeasureItem(LPMEASUREITEMSTRUCT){}
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPReportSubListControl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPReportControl* m_pReportCtrl;       // Pointer to the parent report control.

	int m_nSubItem;                         // Item index of the dragging column.

	CXTPReportHeaderDragWnd* m_pDragWnd;    // Pointer to the associated dragging window.
	CRect m_rcDragWnd;                      // Coordinates of the associated dragging window.
	CRect m_rcDropTarget1;                  // Contains first drop target - the list window itself.
	CRect m_rcDropTarget2;                  // Contains second drop target - report header.

	int m_nDropIndex;                       // Drop item index.
	BOOL m_bSortAscending;                  // TRUE to sort items in list.

};


#endif //#if !defined(__XTPREPORTSUBLISTCONTROL_H__)
