// XTPReportHeader.h: interface for the CXTPReportHeader class.
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
#if !defined(__XTPREPORTHEADER_H__)
#define __XTPREPORTHEADER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPReportColumn;
class CXTPReportColumns;
class CXTPReportControl;
class CXTPReportSubListControl;
class CXTPReportFilterEditControl;
class CXTPReportHeaderDropWnd;
class CXTPReportHeaderDragWnd;

//===========================================================================
// Summary:
//     A report header is a part of the report window positioned above
//     columns of text or numbers. It contains a title for each column,
//     and it can be divided into parts. The user can drag the dividers
//     that separate the parts to set the width of each column.
// Remarks:
//     This class is responsible for header drawing on a report window,
//     processing user events, and changing header properties.
//
//     Functionality of the header includes the following:
//     * Header self-drawing.
//     * Header item custom ordering.
//     * Header item drag and drop, for the reordering of header items.
//     * Sorting report rows with mouse click on column header.
//     * Filtering of the report row items using associated CXTPReportFilterEditControl.
//     * Header items choosing using associated filed chooser control: CXTPReportSubListControl.
//     * Group By area support containing the order of grouped columns.
//
// See Also: CXTPReportControl, CXTPReportColumn, CXTPReportSubListControl
//===========================================================================
class _XTP_EXT_CLASS CXTPReportHeader : public CCmdTarget
{
	friend class CXTPReportSubListControl;
	friend class CXTPReportControl;
	friend class CXTPReportColumn;
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportHeader object.
	// Parameters:
	//     pColumns - Pointer to associated columns array.
	//     pControl - Pointer to the parent report control.
	// Example:
	// <code>
	// // Declare a local CXTPReportHeader object.
	// CXTPReportHeader  myHeader;
	//
	// // Declare a dynamic CXTPReportHeader object.
	// CXTPReportHeader* pmyHeader = new CXTPReportHeader;
	// </code>
	//-----------------------------------------------------------------------
	CXTPReportHeader(CXTPReportControl* pControl, CXTPReportColumns* pColumns);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys CXTPReportHeader object. Performs cleanup operations.
	//-----------------------------------------------------------------------
	virtual ~CXTPReportHeader(void);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws columns header on the provided DC.
	// Parameters:
	//     pDC      - Pointer to the destination device context.
	//     rcHeader - Reference to the rectangle in which the header is drawn.
	// Remarks:
	//     Call this function to draw the header on the provided
	//     device context. Note that it does not contain Group By area.
	// See Also: DrawGroupByControl
	//-----------------------------------------------------------------------
	void Draw(CDC* pDC, CRect& rcHeader);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws Group By area on the provided DC.
	// Parameters:
	//     pDC       - Pointer to the destination device context.
	//     rcGroupBy - Reference to the rectangle of the Group By area.
	// Remarks:
	//     Call this function to draw the header on the provided
	//     device context. Note that it does not contain Group By area.
	// See Also: Draw
	//-----------------------------------------------------------------------
	void DrawGroupByControl(CDC* pDC, CRect& rcGroupBy);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes mouse left button down clicks.
	// Parameters:
	//     ptClick - Point coordinates where the message was fired from.
	// Remarks:
	//     Call this member function if you want Report Header to process
	//     a mouse left button down message. You should provide the method
	//     with the coordinates of the mouse message to proceed.
	// See Also: OnLButtonUp, OnContextMenu, OnMouseMove
	//-----------------------------------------------------------------------
	void OnLButtonDown(CPoint ptClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     Process mouse left button up clicks.
	// Parameters:
	//     nFlags  - Indicates whether various virtual keys are down.
	//     ptClick - Point coordinates where the message was fired from.
	// Remarks:
	//     Call this member function if you want Report Header to process
	//     a mouse left button up message. You should provide the method
	//     with the coordinates of the mouse message to proceed and a flag
	//     showing which of virtual keys were down.
	// See Also: OnLButtonDown, OnContextMenu, OnMouseMove
	//-----------------------------------------------------------------------
	void OnLButtonUp(UINT nFlags, CPoint ptClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     Process context menu. Called by the framework when the user
	//     has clicked the right mouse button (right clicked) in the window.
	// Parameters:
	//     ptClick - Position of the cursor, in screen coordinates,
	//               at the time of the mouse click.
	// Remarks:
	//     Call this member function if you want Report Header to process
	//     a mouse right button down (context menu call) message.
	//     You should provide the method with the coordinates of the
	//     mouse message to proceed.
	// See Also: OnLButtonDown, OnLButtonUp, OnMouseMove
	//-----------------------------------------------------------------------
	void OnContextMenu(CPoint ptClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     Process mouse moving.
	// Parameters:
	//     nFlags - Indicates whether various virtual keys are down.
	//     point  - Point coordinates where the message was fired from.
	// Remarks:
	//     Call this member function if you want Report Header to process
	//     a mouse move message. You should provide the method with the
	//     coordinates of the mouse message to proceed and a flag
	//     showing which of virtual keys were down.
	// See Also: OnLButtonDown, OnLButtonUp, OnContextMenu
	//-----------------------------------------------------------------------
	void OnMouseMove(UINT nFlags, CPoint point);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if dragging the header now.
	// Remarks:
	//     When user starts dragging a row, Report Header enters a dragging
	//     loop and processes each message in the specific way.
	//     Using this function you can determine if we are in such a dragging
	//     loop or not.
	// Returns:
	//     TRUE when dragging, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsDragHeader();

	//-----------------------------------------------------------------------
	// Summary:
	//     Allows/disallows column removing.
	// Parameters:
	//     bAllow - TRUE for allowing column removing, FALSE for disallowing.
	// Remarks:
	//     Usually user is allowed to remove a column using mouse drag and drop.
	//     Using this member function, you can change the user's permissions
	//     force allowing or disallowing of column removing.
	// See Also: IsAllowColumnRemove, CXTPReportColumn::AllowColumnRemove
	//-----------------------------------------------------------------------
	void AllowColumnRemove(BOOL bAllow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the current column removing allowance.
	// Remarks:
	//     Call this member function to determine whether the user is allowed
	//     to remove the column using drag and drop or not.
	// Returns:
	//     TRUE when column removing is allowed, FALSE otherwise.
	// See Also: AllowColumnRemove
	//-----------------------------------------------------------------------
	BOOL IsAllowColumnRemove();

	//-----------------------------------------------------------------------
	// Summary:
	//     Allows/disallows column reordering.
	// Parameters:
	//     bAllow - TRUE for allowing column reordering, FALSE for disallowing.
	// Remarks:
	//     Usually user is allowed to reorder a column using mouse drag and drop.
	//     Using this member function, you can change the user's permissions
	//     and force allowing or disallowing of column reordering.
	// See Also: IsAllowColumnReorder
	//-----------------------------------------------------------------------
	void AllowColumnReorder(BOOL bAllow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the current column reordering allowance.
	// Remarks:
	//     Call this member function to determine whether the user is allowed
	//     to reorder the column using drag and drop or not.
	// Returns:
	//     TRUE when column reordering is allowed, FALSE otherwise.
	// See Also: AllowColumnReorder
	//-----------------------------------------------------------------------
	BOOL IsAllowColumnReorder();

	//-----------------------------------------------------------------------
	// Summary:
	//     Allows/disallows column resizing.
	// Parameters:
	//     bAllow - TRUE for allowing column resizing, FALSE for disallowing.
	// Remarks:
	//     Usually the user is allowed to resize column using mouse drag and drop.
	//     Using this member function you can change user's permissions
	//     and force allowing or disallowing of column resizing.
	// See Also: IsAllowColumnResize
	//-----------------------------------------------------------------------
	void AllowColumnResize(BOOL bAllow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns current column resizing allowance.
	// Remarks:
	//     Call this member function to determine whether the user is allowed
	//     to resize a column using drag and drop or not.
	// Returns:
	//     TRUE when column resizing is allowed, FALSE otherwise.
	// See Also: AllowColumnResize
	//-----------------------------------------------------------------------
	BOOL IsAllowColumnResize();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets show items in groups mode.
	// Parameters:
	//     bShowInGroups - TRUE for show items in groups, FALSE otherwise.
	// Remarks:
	//     Using this member function you can change Report Header
	//     showing items in groups mode.
	// See Also: IsShowItemsInGroups
	//-----------------------------------------------------------------------
	void ShowItemsInGroups(BOOL bShowInGroups);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns current show items in groups mode.
	// Remarks:
	//     Using this member function you can retrieve current
	//     showing items in groups mode.
	// Returns:
	//     TRUE when items are shown in groups, FALSE otherwise.
	// See Also: ShowItemsInGroups
	//-----------------------------------------------------------------------
	BOOL IsShowItemsInGroups();


	//-----------------------------------------------------------------------
	// Summary:
	//     Sets allowing column sorting mode.
	// Parameters:
	//     bAllow - TRUE for allowing, FALSE for disallowing.
	// Remarks:
	//     Using this member function you can allow or disallow sorting
	//     columns by mouse.
	// See Also: IsAllowColumnSort
	//-----------------------------------------------------------------------
	void AllowColumnSort(BOOL bAllow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns current allowing column sorting mode.
	// Remarks:
	//     Using this member function you can retrieve the current
	//     allowing column sorting mode.
	// Returns:
	//     TRUE when column sorting is allowed, FALSE otherwise.
	// See Also: AllowColumnSort
	//-----------------------------------------------------------------------
	BOOL IsAllowColumnSort();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the next visible column.
	// Parameters:
	//     nIndex     - Position of a column in the array of columns to start from.
	//     nDirection - The direction to move (left or right) in the array, starting and nIndex.
	// Returns:
	//     A pointer to the next visible column.
	//-----------------------------------------------------------------------
	CXTPReportColumn* GetNextVisibleColumn(int nIndex, int nDirection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Finds a column from point.
	// Parameters:
	//     ptPoint - A point to test in report control client coordinates.
	// Remarks:
	//     Call this member function if you want to retrieve a pointer to a
	//     column in the specified position.
	// Returns:
	//     A pointer to the column placed at the specified position, if any,
	//     or NULL otherwise.
	// See Also: CXTPReportControl::HitTest, CXTPReportRow::HitTest
	//-----------------------------------------------------------------------
	CXTPReportColumn* HitTest(CPoint ptPoint);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates index of where to insert a column nearest to the provided point.
	// Parameters:
	//     ptPoint - Point coordinates to test
	// Returns:
	//     Header column index.
	//-----------------------------------------------------------------------
	int FindHeaderColumn(CPoint ptPoint);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates index of where to insert a column nearest to the provided point.
	// Parameters:
	//     ptPoint      - Point coordinates to test.
	//     bExactSearch - TRUE when search by exact position,
	//                    FALSE when searching average value.
	// Returns:
	//     GroupBy column position.
	//-----------------------------------------------------------------------
	int FindGroupByColumn(CPoint ptPoint, BOOL bExactSearch = FALSE);

private:
	//-----------------------------------------------------------------------
	// Summary:
	//     Finds header column index by provided point.
	// Parameters:
	//     ptPoint - A point to test in report control window client coordinates.
	// Returns:
	//     An index of the found column in the header column array, if found,
	//     or -1 if column was not found.
	//-----------------------------------------------------------------------
	int HitTestHeaderColumnIndex(CPoint ptPoint);

	//-----------------------------------------------------------------------
	// Summary:
	//    Checks if the provided point is over column resize area.
	// Parameters:
	//    ptPoint - Point coordinates to test
	// Returns:
	//    TRUE if mouse is under column resize area, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL MouseOverColumnResizeArea(CPoint ptPoint);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Checks if the provided point is over column resize area.
	// Parameters:
	//     nTotalWidth - Total width of the header area in pixels.
	//     nFirstIndex - Index of a column to start adjusting from.
	// Remarks:
	//     Call this member function when you want to adjust the width of
	//     columns in reaction to any specific event.
	//     For example, this member function is called by the framework
	//     when changing the report control size (WM_SIZE), and when
	//     changing a size of any column by mouse dragging.
	// See Also: ResizeColumn, GetMaxAvailWidth
	//-----------------------------------------------------------------------
	void AdjustColumnsWidth(int nTotalWidth, int nFirstIndex = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Starts column header dragging mode.
	// Parameters:
	//     pColumn - Pointer to the column, which is entering in dragging mode.
	//     bHeader - TRUE when starting dragging for column header items,
	//               FALSE when starting dragging for Group By column items.
	// Remarks:
	//     Usually this method is called internally in a reaction to any
	//     specific user actions. For example when a user starts dragging
	//     header by mouse.
	// See Also: GetDraggingColumn, CancelMouseMode
	//-----------------------------------------------------------------------
	void StartDragging(CXTPReportColumn* pColumn, BOOL bHeader = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Resizes a provided column to a specific new width.
	// Parameters:
	//     pColumn - Pointer to a column to resize.
	//     nWidth  - New column width in pixels.
	// Remarks:
	//     This method resizes a provided column to a specific new width,
	//     and adjusts the width of other columns in correspondence with
	//     the new one.
	// See Also: AdjustColumnsWidth, GetMaxAvailWidth
	//-----------------------------------------------------------------------
	void ResizeColumn(CXTPReportColumn* pColumn, int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Resets any mouse mode to xtpMouseNothing.
	// Remarks:
	//     Usually this member function is called when finishing
	//     dragging or resizing columns. It performs all
	//     necessary cleanup and resets mouse mode to xtpMouseNothing.
	// See Also: StartDragging
	//-----------------------------------------------------------------------
	void CancelMouseMode();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns maximum available width for the column.
	// Parameters:
	//     pColumn - Pointer to the column to check.
	// Remarks:
	//     Calculates maximum possible width for the column depending
	//     on the width and modes for other columns.
	// Returns:
	//     Maximum available width for the column in pixels.
	// See Also: AdjustColumnsWidth, ResizeColumn
	//-----------------------------------------------------------------------
	int GetMaxAvailWidth(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a divider between columns.
	// Parameters:
	//     nIndex  - An index of the column to set hot divider before.
	//     bHeader - TRUE if you want to set hot divider in columns header,
	//               FALSE to set it between on Group By area items.
	// Remarks:
	//     When dragging columns, call this member function to show a
	//     hot divider (red arrows by default) between columns as a target
	//     dropping a currently dragging one.
	// Returns:
	//     An index of the column before which a hot divider was shown.
	//-----------------------------------------------------------------------
	int SetHotDivider(int nIndex, BOOL bHeader = TRUE);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a column, which is dragging at the moment.
	// Remarks:
	//     Call this member function if you want to retrieve a pointer
	//     to the currently dragging column.
	// Returns:
	//     A pointer to the dragging column if the control is in
	//     dragging mode, or NULL otherwise.
	// Example: <code>CXTPReportColumn* pColumn = GetDraggingColumn();</code>
	// See Also: StartDragging, GetResizingColumn,
	//-----------------------------------------------------------------------
	CXTPReportColumn* GetDraggingColumn();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets associated Field chooser list control.
	// Parameters:
	//     pSubList - Pointer to the field chooser list box control.
	// Remarks:
	//     You should call this member function to assign the user's custom
	//     List control as report control Field chooser control.
	//     After assigning, the framework will automatically be processing
	//     all windows messages from the control.
	// Returns:
	//     TRUE if successful; otherwise FALSE.
	// Example:
	// <code>
	// // CXTPReportControl wndReport;
	// // CXTPReportSubListControl wndSubList;
	// wndReport.GetColumns()->GetReportHeader()->SetSubListCtrl(&wndSubList);
	// </code>
	// See Also: CXTPReportSubListControl overview, SetFilterEditCtrl
	//-----------------------------------------------------------------------
	BOOL SetSubListCtrl(CXTPReportSubListControl* pSubList);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves Pointer to the field chooser
	// Returns:
	//     Pointer to the field chooser list box control.
	// See Also: SetSubListCtrl
	//-----------------------------------------------------------------------
	CXTPReportSubListControl* GetSubListCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets associated filter edit control.
	// Parameters:
	//     pFilterEdit - Pointer to the user's filter edit control.
	// Remarks:
	//     You should call this member function to assign the user's custom
	//     edit box control as report control filter edit control.
	//     After assigning, the framework will automatically be processing
	//     all windows messages for the control.
	// Returns:
	//     TRUE if successful; otherwise FALSE.
	// Example:
	// <code>
	// CXTPReportControl wndReport;
	// CXTPReportFilterEditControl wndFilterEdit;
	// wndReport.GetColumns()->GetReportHeader()->SetFilterEditCtrl(&wndFilterEdit);
	// </code>
	// See Also: CXTPReportFilterEditControl overview, GetFilterEditCtrl, SetSubListCtrl
	//-----------------------------------------------------------------------
	BOOL SetFilterEditCtrl(CXTPReportFilterEditControl* pFilterEdit);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the associated Filter edit control.
	// Remarks:
	//     Call this member function to retrieve a pointer to the
	//     associated filter edit control.
	// Returns:
	//     A pointer to the associated filter edit control.
	// See Also: CXTPReportFilterEditControl overview, SetFilterEditCtrl
	//-----------------------------------------------------------------------
	CXTPReportFilterEditControl* GetFilterEditCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the Group By area height.
	// Remarks:
	//     Calculates Group By area height depending on the number of
	//     items contained inside.
	// Returns:
	//     A Group By area height in pixels.
	//-----------------------------------------------------------------------
	int GetGroupByHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Fits the provided column width to the longest column item width.
	// Parameters:
	//     pColumn - A column to fit width.
	// Remarks:
	//     Call this method to adjust column width to the width of the
	//     widest column item.
	//-----------------------------------------------------------------------
	void BestFit(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Checks a column for its position in report header.
	// Parameters:
	//     pColumn - A pointer to the column to check.
	// Returns:
	//     TRUE if a provided column is the last visible column in a report
	//     header, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsLastVisibleColumn(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Checks a column for its position in report header.
	// Parameters:
	//     pColumn - A pointer to the column to check.
	// Returns:
	//     TRUE if a provided column is the last resizable column in a report
	//     header, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsLastResizebleColumn(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns an indent for report rows by 1 tree level.
	// Remarks:
	//     Call this member function to retrieve an indent for 1 level
	//     of tree depth for the report control in tree mode.
	// Returns:
	//     An indent by one level in pixels.
	//-----------------------------------------------------------------------
	int GetIndentLevel();

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates header settings after any changes.
	// Remarks:
	//     Call this member function if you want to automatically
	//     adjust columns widths and redraw report control depending
	//     on current columns settings.
	//-----------------------------------------------------------------------
	void OnColumnsChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves parent Report Control
	// Returns:
	//     CXTPReportControl pointer
	//-----------------------------------------------------------------------
	CXTPReportControl* GetControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify if AutoColumnSizing should be used.
	// Parameters:
	//     bAutoColumnSizing - If TRUE, all columns are sized to fill the
	//                         entire width of the ReportControl.  When the
	//                         ReportControl is resized, the Columns will
	//                         automatically be resized to fit the new size
	//                         of the RepotControl.
	//                         If FALSE, the columns will use the width they
	//                         were assigned when they were created, or whatever
	//                         value is stored in the columns width property.
	//                         The columns widths my occupy more or less that
	//                         the total width of the ReportControl.  If the
	//                         total width of the columns exceeds the width
	//                         of the ReportControl, then those columns will not
	//                         be visible.  When the ReportControl is resized,
	//                         the size of the columns will not be effected.
	//-----------------------------------------------------------------------
	void SetAutoColumnSizing(BOOL bAutoColumnSizing);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if AutoColumnSizing is currently used.
	// Returns:
	//     TRUE is AutoColumnSizing is used, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsAutoColumnSizing();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the total width of the header (total combined column widths).
	// Returns:
	//     The total combined column widths, which make up the header width.
	//-----------------------------------------------------------------------
	int GetWidth();

private:
	void TrackColumn(CXTPReportColumn* pColumn, CPoint pt);
	void DestroyDropWnd();

protected:
	CXTPReportColumns* m_pColumns;  // Contains associated columns list.
	CXTPReportControl* m_pControl;  // Pointer to the parent Report control.

	int m_nDropIndex;                           // Item index of the column to drop to.
	CXTPReportHeaderDropWnd* m_pDropWnd;        // Column drop window.
	CXTPReportHeaderDragWnd* m_pDragWnd;        // Column drag window.

	HCURSOR m_hDontDropCursor;                  // Handler of user's Don't Drop cursor.

	CXTPReportColumn* m_pDragColumn;            // Dragging column.
	BOOL m_bDragHeader;                         // Determines whether control is in header dragging mode or group by dragging mode.
	BOOL m_bDragGroupBox;                       // Determines whether control is in group box dragging mode.

	CXTPReportSubListControl* m_pSubList;       // Field chooser list control.
	CXTPReportFilterEditControl* m_pFilterEdit; // Filter edit control

	CRect m_rcGroupBy;      // Group By area coordinates.
	CRect m_rcHeader;       // Header coordinates.
	CPoint m_ptMouse;       // Stores mouse coordinates for processing when required (resizing, etc).

	HCURSOR m_hResizeCursor;    // Handler of user's Resizing cursor.

	BOOL m_bAllowColumnRemove;  // FALSE if columns removing is not allowed.
	BOOL m_bAllowColumnResize;  // FALSE if columns resizing is not allowed.
	BOOL m_bAllowColumnReorder; // FALSE if columns reordering is not allowed.
	BOOL m_bAllowColumnSort;    // FALSE if columns sorting is not allowed.

	BOOL m_bAutoColumnSizing;   // TRUE if AutoColumnSizing should be used.
	int m_nResizeCaptureRange;  // Distance around column divider where column re-sizer is shown.
	int m_nIndentLevel;         // Width indent at the left of the column.
	BOOL m_bShowItemsInGroups;  // TRUE when showing items in groups, FALSE when grouping is not used.
	int m_nHeaderWidth;         // The combined width of all the visible column headers.

private:
	enum ReportDraggingMode
	{
		dragNothing      = 0,    // User is dragging nothing.
		dragOutTarget    = 1,    // User is dragging an item outside the drop target.
		dragInHeader     = 2,    // User is dragging an item inside header
		dragInGroupBox   = 4,    // User is dragging an item inside group box
		dragInTarget     = 6,    // User is dragging an item inside the drop target.
		dragFieldChooser = 8     // User is dragging an item inside field chooser.
	} m_dragMode;                   // Column dragging mode.

};


AFX_INLINE CXTPReportFilterEditControl* CXTPReportHeader::GetFilterEditCtrl() {
	return m_pFilterEdit;
}

AFX_INLINE BOOL CXTPReportHeader::IsDragHeader() {
	return m_bDragHeader;
}
AFX_INLINE int CXTPReportHeader::GetIndentLevel() {
	return m_nIndentLevel;
}
AFX_INLINE void CXTPReportHeader::AllowColumnRemove(BOOL bAllow) {
	m_bAllowColumnRemove = bAllow;
}
AFX_INLINE void CXTPReportHeader::AllowColumnResize(BOOL bAllow) {
	m_bAllowColumnResize = bAllow;
}
AFX_INLINE void CXTPReportHeader::AllowColumnReorder(BOOL bAllow) {
	m_bAllowColumnReorder = bAllow;
}
AFX_INLINE BOOL CXTPReportHeader::IsAllowColumnRemove() {
	return m_bAllowColumnRemove;
}
AFX_INLINE BOOL CXTPReportHeader::IsAllowColumnReorder() {
	return m_bAllowColumnReorder;
}
AFX_INLINE BOOL CXTPReportHeader::IsAllowColumnResize() {
	return m_bAllowColumnResize;
}
AFX_INLINE void CXTPReportHeader::AllowColumnSort(BOOL bAllow) {
	m_bAllowColumnSort = bAllow;
}
AFX_INLINE BOOL CXTPReportHeader::IsAllowColumnSort() {
	return m_bAllowColumnSort;
}

AFX_INLINE BOOL CXTPReportHeader::IsShowItemsInGroups() {
	return m_bShowItemsInGroups;
}
AFX_INLINE CXTPReportControl* CXTPReportHeader::GetControl() {
	return m_pControl;
}
AFX_INLINE CXTPReportSubListControl* CXTPReportHeader::GetSubListCtrl() {
	return m_pSubList;
}
AFX_INLINE int CXTPReportHeader::GetWidth() {
	return m_nHeaderWidth;
}
AFX_INLINE BOOL CXTPReportHeader::IsAutoColumnSizing() {
	return m_bAutoColumnSizing;
}

#endif //#if !defined(__XTPREPORTHEADER_H__)
