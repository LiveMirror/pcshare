// CXTPReportColumn.h: interface for the CXTPReportColumn class.
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
#if !defined(__XTPREPORTCOLUMN_H__)
#define __XTPREPORTCOLUMN_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "XTPReportDefines.h"
#include "XTPReportHeader.h"

class CXTPReportGroups;
class CXTPReportRecordItemEditOptions;
class CXTPPropExchange;

//===========================================================================
// Summary:
//     Represents report column item with its properties and operations.
// Remarks:
//     You create object of CXTPReportColumn by calling a constructor and
//     provide all necessary parameters. Many of the parameters have default
//     values. Column has 3 properties that greatly influence its behavior:
//
//     bAutoSize - TRUE allows auto resizing of the width of columns when
//                 ReportControl's client area is resized by the user. If
//                 bAutoSize is set to FALSE, the column keeps its initial
//                 size
//
//     bSortable - Allows column to be sortable if it has value TRUE. If
//                 you set it to FALSE, you won't be able to sort records
//                 by their column
//
//     bVisible  - Determines if columns will draw by control or not. For
//                 example, if the column is dragged into FieldChooser,
//                 the column still exists in the columns list, but it
//                 becomes invisible.
// See Also: CXTPReportControl, CXTPReportColumns
//===========================================================================
class _XTP_EXT_CLASS CXTPReportColumn : public CCmdTarget
{
	friend class CXTPReportControl;
	friend class CXTPReportColumns;
	friend class CXTPReportHeader;
	friend class CXTPReportRows;
	friend class CXTPReportRow;
	friend class CXTPReportColumnOrder;

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportColumn object.
	// Parameters:
	//     strName    - Column name.
	//     nItemIndex - An index of the corresponding record item.
	//     nWidth     - An initial width of the created column.
	//     nMinWidth  - Minimal width of the column.
	//     nIconID    - Number of the corresponding icon in the image list.
	//     bAutoSize  - Is column resizable flag.
	//     bSortable  - Is column sortable flag.
	//     bVisible   - Is column visible flag.
	// Remarks:
	//     There are 3 parameters that greatly influence a column's behavior:
	//     * <b>bAutoSize</b> TRUE allows auto resizing of width of columns when
	//           the ReportControl's client area is resized by the
	//           user. If bAutoSize is set to FALSE, the column
	//           keeps its initial size
	//     * <b>bSortable</b> Allows the column to be sortable if has value TRUE.
	//           If you set it to FALSE, you won't be able to sort
	//           records by their column
	//     * <b>bVisible</b>  Determines if columns will draw by control or not.
	//           For example if the column is dragged into FieldChooser,
	//           the column still exists in the columns list,
	//           but it becomes invisible.
	// Example:
	// <code>
	// #define COLUMN_ICON 1
	// #define COLUMN_MAIL_ICON 1
	//
	// CXTPReportControl wndReport;
	// wndReport.AddColumn(new CXTPReportColumn(COLUMN_ICON, _T("Message Class"), 18, FALSE, COLUMN_MAIL_ICON));
	// </code>
	//-----------------------------------------------------------------------
	CXTPReportColumn();

public:

	CXTPReportColumn(int nItemIndex, CString strName, int nWidth, BOOL bAutoSize = TRUE, int nIconID = XTP_REPORT_NOICON , BOOL bSortable = TRUE, BOOL bVisible = TRUE); // <COMBINE CXTPReportColumn::CXTPReportColumn>


	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPReportColumn object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPReportColumn(void);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns current column header drag mode.
	// Remarks:
	//     ReportColumn may be moved (change order of columns, delete columns from
	//     header, move column to FieldChooser) using drag and drop. When column
	//     is moved, it is in Dragged mode. You use IsDragging() to determine if
	//     columns are in Dragged mode
	// Returns:
	//     TRUE if the column header is in dragging mode, FALSE otherwise.
	// See Also: IsAllowDragging
	//-----------------------------------------------------------------------
	BOOL IsDragging();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the last drawn column rect.
	// Remarks:
	//     Each time when the column is drawing, its rectangle is recalculated
	//     to fit in size after ReportControl resizing, Column resizing
	//     or moving, etc. You use GetRect() to get the latest rectangle sizes.
	// Returns:
	//     The client rectangle where the column was drawn last time.
	//-----------------------------------------------------------------------
	CRect GetRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns column width.
	// Remarks:
	//     Each time when the column is drawing, its rectangle is recalculated
	//     to fit in size after ReportControl resizing, Column resizing
	//     or moving, etc. You use GetWidth() to get the latest rectangle width.
	// Returns:
	//     The current column width in logical units.
	//-----------------------------------------------------------------------
	int GetWidth();

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates caption length.
	// Parameters:
	//     pDC - Device context to calculate width at.
	// Remarks:
	//     This function calculates caption text length in logical units
	//     using the given device context, current caption font, and string
	//     of text.
	// Returns:
	//     Caption length in pixels.
	//-----------------------------------------------------------------------
	int GetCaptionWidth(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets column width.
	// Parameters:
	//     nNewWidth - New column width in pixels.
	// Remarks:
	//     This member function gives you possibility to change column width
	//     manually. To set initial width or set width for columns which
	//     AutoSize = FALSE.
	// Returns:
	//     Previous column width in pixels.
	//-----------------------------------------------------------------------
	int SetWidth(int nNewWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns column indentation.
	// Returns:
	//     Previous column indentation in pixels.
	//-----------------------------------------------------------------------
	int GetIndent();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the index of the associated record item.
	// Returns:
	//     The index of the associated record item.
	//-----------------------------------------------------------------------
	int GetItemIndex();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the index of the column.
	// Returns:
	//     The index of the column.
	//-----------------------------------------------------------------------
	int GetIndex();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the ID of the associated column header icon.
	// Remarks:
	//     Column can have an image that will be drawn on it. You use
	//     GetIconID() to get the current image id which passes to the
	//     column ID of the column. It must match to one of the ids in the
	//     imagelist
	// Returns:
	//     The ID of the associated column header icon.
	//-----------------------------------------------------------------------
	int GetIconID();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the text caption for the column header.
	// Remarks:
	//     You use GetCaption() to get the current column caption.
	// Returns:
	//     The text caption for the column header.
	//-----------------------------------------------------------------------
	CString GetCaption();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set/change the column header caption.
	// Parameters:
	//     strCaption - String caption of the column.  Text displayed in the column header.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR strCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the current column header visibility mode.
	// Returns:
	//     TRUE if the column header is visible, FALSE otherwise.
	// See Also: SetVisible
	//-----------------------------------------------------------------------
	BOOL IsVisible();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets column visible mode (TRUE - visible, FALSE - invisible).
	// Parameters:
	//     bVisible - New column visible state.
	// Remarks:
	//     When you move a column to FieldChooser, it becomes invisible.
	//     To change visibility of column you use SetVisible(BOOL)
	// Returns:
	//     Previous column visible state.
	// See Also: IsVisible
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the current column header filterability mode.
	// Returns:
	//     TRUE if the column header is filterable, FALSE otherwise.
	// See Also: SetFiltrable
	//-----------------------------------------------------------------------
	BOOL IsFiltrable();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets column filterable mode (TRUE - filterable, FALSE - not filterable).
	// Parameters:
	//     bFiltrable - New column filterable state.
	// Returns:
	//     Previous column filterable state.
	// See Also: IsFiltrable
	//-----------------------------------------------------------------------
	BOOL SetFiltrable(BOOL bFiltrable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns column allow resizing state.
	// Returns:
	//     TRUE if the column allows resizing, FALSE otherwise.
	// See Also: EnableResize
	//-----------------------------------------------------------------------
	BOOL IsResizable();

	//-----------------------------------------------------------------------
	// Summary:
	//     Enable/Disables the column's allow resizing mode.
	// Parameters:
	//     bIsResizable - TRUE if column will be resizable, FALSE otherwise.
	// See Also: IsResizable
	//-----------------------------------------------------------------------
	void EnableResize(BOOL bIsResizable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets minimal width value for column.
	// Parameters:
	//     nMinWidth - New column minimal width value.
	// Remarks:
	//     After you set the minimal width for the column, it cannot be
	//     resizable to less than minimal width.
	// Returns:
	//     Previous column minimal width value.
	// See Also: GetMinWidth
	//-----------------------------------------------------------------------
	int SetMinWidth(int nMinWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the current minimal column width.
	// Returns:
	//     Column minimal width value in pixels.
	// See Also: SetMinWidth
	//-----------------------------------------------------------------------
	int GetMinWidth();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the column sortable flag.
	// Returns:
	//     TRUE if column is sortable, FALSE otherwise.
	// See Also: IsSorted, IsSortedIncreasing, IsSortedDecreasing
	//-----------------------------------------------------------------------
	BOOL IsSortable();

	//-----------------------------------------------------------------------
	// Summary:
	//    Call this member to determine if the column can be grouped.
	// Remarks:
	//     Not all columns allow for grouping. You use IsGroupable
	//     to determine if the column can be grouped.  I.e. Added
	//     to the "Group By" box.
	// Returns:
	//     TRUE if the column can be grouped, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsGroupable();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the column dragging allowance flag.
	// Returns:
	//     TRUE if the column can be dragged, FALSE otherwise.
	// See Also: IsDragging
	//-----------------------------------------------------------------------
	BOOL IsAllowDragging();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the column is sorted in the increasing order.
	// Returns:
	//     TRUE if the column is sorted in increasing order, FALSE otherwise.
	// See Also: IsSorted, IsSortable, IsSortedDecreasing
	//-----------------------------------------------------------------------
	BOOL IsSortedIncreasing();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the column is sorted in the decreasing order.
	// Returns:
	//     TRUE if the column is sorted in decreasing order, FALSE otherwise.
	// See Also: IsSorted, IsSortable, IsSortedIncreasing
	//-----------------------------------------------------------------------
	BOOL IsSortedDecreasing();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify that the column is sorted in the increasing order.
	// Parameters:
	//     bSortIncreasing - TRUE if the column should be sorted in increasing
	//                       order, FALSE otherwise.
	// See Also: IsSorted, IsSortable, IsSortedIncreasing, IsSortedDecreasing
	//-----------------------------------------------------------------------
	void SetSortIncreasing(BOOL bSortIncreasing);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the column is sorted.
	// Returns:
	//     TRUE if the column is sorted, FALSE otherwise.
	// Remarks:
	//     If the records list is sorted by given column IsSorted() returns
	//     TRUE, otherwise - FALSE
	// See Also: IsSortable, IsSortedIncreasing, IsSortedDecreasing
	//-----------------------------------------------------------------------
	BOOL IsSorted();

	//-----------------------------------------------------------------------
	// Summary:
	//     Makes/unmakes the current column as containing a tree inside.
	// Parameters:
	//     bIsTreeColumn - TRUE if this column contains tree items
	//                     (collapse/expand buttons) drawn inside,
	//                     FALSE otherwise.
	// See Also: IsTreeColumn
	//-----------------------------------------------------------------------
	void SetTreeColumn(BOOL bIsTreeColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns TRUE if the current column contains tree.
	// Returns:
	//     TRUE if the current column contains tree items, FALSE otherwise.
	// See Also: SetTreeColumn
	//-----------------------------------------------------------------------
	BOOL IsTreeColumn();

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes column data to/from the provided archive stream.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets new Alignment value for column.
	// Parameters:
	//     nAlignment - New alignment value.
	//                  Possible alignment values: DT_LEFT, DT_CENTER, DT_RIGHT.
	//                  See also DrawText() Format Flags.
	// Returns:
	//     Previous alignment column value.
	//-----------------------------------------------------------------------
	int SetAlignment(int nAlignment);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the current alignment value for the column.
	// Returns:
	//     Current column alignment value.
	//     The result is one of the following: DT_LEFT, DT_CENTER, DT_RIGHT.
	//-----------------------------------------------------------------------
	int GetAlignment();

	//-----------------------------------------------------------------------
	// Summary:
	//     Checks if the column header should have the sort triangle drawn.
	// Returns:
	//     TRUE if the sort triangle should be drawn for this column,
	//     FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL HasSortTriangle();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the column auto sized.
	// Returns:
	//     TRUE if the column is automatically sized.
	//-----------------------------------------------------------------------
	BOOL IsAutoSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether the column can be sorted when
	//     the user clicks on the column header.
	// Parameters:
	//     bSortable - If TRUE, records can be sorted by this column by
	//                 clicking on the column header.  Each time the
	//                 column header is clicked the sort order will
	//                 toggle between ascending and descending.  by
	//                 default, bSortable is TRUE.
	//                 If FALSE, records can not be sorted by this column
	//                 by clicking on the column header.  The column can
	//                 still be sorted, but not by clicking on the column
	//                 header.  If you would like to sort the column when
	//                 bSortable is FALSE, then you can add the column to
	//                 the ReportControl's SortOrder, this will allow you
	//                 to keep a column sorted at all times.
	// See Also: SetGroupable
	//-----------------------------------------------------------------------
	void SetSortable(BOOL bSortable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether the column can be grouped.
	// Parameters:
	//     bGroupable - If TRUE, the column can be added to the "Group By"
	//                  box and the contents of the ReportControl will be
	//                  grouped by the column.
	//                  If FALSE, the column can't be added to the "Group By" box.
	// See Also: SetSortable
	//-----------------------------------------------------------------------
	void SetGroupable(BOOL bGroupable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves columns array
	// Returns:
	//     CXTPReportColumns object
	//-----------------------------------------------------------------------
	CXTPReportColumns* GetColumns();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieved parent report control
	// Returns:
	//     Parent report control
	// See Also: GetColumns
	//-----------------------------------------------------------------------
	CXTPReportControl* GetControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the column header will appear in
	//     the field chooser when hidden or in the GroupBy Box.
	// Returns:
	//     TRUE is if the column header will be displayed in the field chooser when
	//     the column is hidden or in the GroupBy Box, FALSE if it will not.
	// See Also: SetShowInFieldChooser
	//-----------------------------------------------------------------------
	BOOL IsShowInFieldChooser();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether the column will appear in
	//     the FieldChooser when the column is hidden or in the "Group By Box."
	// Parameters:
	//     bShow - TRUE is the column should be displayed in the field chooser
	//             when hidden, FALSE otherwise.
	// Remarks:
	//     When a column is hidden or added to the "Group By Box" it will be
	//     automatically added to the FieldChooser unless the ShowInFieldChooser
	//     property is set to FALSE.
	//     If you would like to have a hidden column that the user can not access
	//     and you enable the FieldChooser, then you will need to set the
	//     ShowInFieldChooser property to FALSE.  By default this property is TRUE.
	// See Also: IsShowInFieldChooser
	//-----------------------------------------------------------------------
	void SetShowInFieldChooser(BOOL bShow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves CXTPReportRecordItemEditOptions.
	// Returns:
	//     Edit Options of column
	//-----------------------------------------------------------------------
	CXTPReportRecordItemEditOptions* GetEditOptions();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if an edit box is added for the items
	//     in the column.
	// Returns:
	//     TRUE is the items in the column are editable, FALSE if the items
	//     can not be edited.
	// See Also: SetEditable
	//-----------------------------------------------------------------------
	BOOL IsEditable();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether the CXTPReportRecordItem(s)
	//     in the column can be edited.  This will add an edit box to edit the item
	//     when the item is click/double-clicked.  This can be overridden for individual
	//     items if the CXTPReportRecordItem::SetEditable member is called.
	// Parameters:
	//     bEditable - TRUE is the items in the column are editable (an
	//                 edit box is added to edit the contents of the item).
	// See Also: IsEditable
	//-----------------------------------------------------------------------
	void SetEditable(BOOL bEditable = TRUE);


	//-----------------------------------------------------------------------
	// Summary:
	//     Allows/disallows column removing.
	// Parameters:
	//     bAllow - TRUE for allowing column removing, FALSE for disallowing.
	// Remarks:
	//     Usually user is allowed to remove a column using mouse drag and drop.
	//     Using this member function, you can change the user's permissions
	//     force allowing or disallowing of column removing.
	// See Also: CXTPReportHeader::AllowColumnRemove
	//-----------------------------------------------------------------------
	void AllowRemove(BOOL bAllow);

protected:
	CString m_strName;                  // Column name.
	int m_nItemIndex;                   // Column index.

	BOOL m_bSortIncreasing;             // Stores column sort direction (used when m_bSortable is TRUE).
	BOOL m_bSortable;                   // Determines if the column could be sortable by.
	BOOL m_bGroupable;                  // Determines if the column could be grouped by.

	BOOL m_bAllowDrag;                  // Allow/disallow dragging of the column
	BOOL m_bAllowRemove;                // TRUE to allow column remove

	CXTPReportColumns* m_pColumns;      // Stores a pointer to the parent columns collection.

	int m_nIconID;                      // Associated icon ID.
	BOOL m_bVisible;                    // Visible state.
	BOOL m_bFiltrable;                  // Is filtering by this column allowed?.

	BOOL m_bIsResizable;                // Indicates if the column allows width resizing
	CRect m_rcColumn;                   // Saves rectangle where column was drawn last time.
	CRect m_rcGroupBy;                  // Saves rectangle where column was drawn in group box.
	int m_nMinWidth;                    // Minimal column width. It is not allowed to do it less then minimal.
	int m_nMaxItemWidth;                // Contains maximal width of the corresponding item drawn in the column.
	int m_nIndent;                      // Width indent at the left of the column.
	int m_nAlignment;                   // Alignment mode for column: DT_LEFT || DT_RIGHT || DT_CENTER

	int m_nColumnAutoWidth;             // The column a  automatic width
	int m_nColumnStaticWidth;           // The user defined with.
	BOOL m_bAutoSize;                   // TRUE if column auto sized.

	BOOL m_bShowInFieldChooser;         // Indicates if column is shown in the field chooser when the column is hidden or in the GroupBy Box.
	BOOL m_bEditable;                   // Indicates whether an edit box should be added to the items in the column.

	CXTPReportRecordItemEditOptions* m_pEditOptions;    // Edit options of the column

};


AFX_INLINE CString CXTPReportColumn::GetCaption() {
	return m_strName;
}
AFX_INLINE int CXTPReportColumn::GetIconID() {
	return m_nIconID;
}
AFX_INLINE int CXTPReportColumn::SetMinWidth(int nMinWidth) {
	int OldMinWidth = m_nMinWidth;
	m_nMinWidth = nMinWidth;
	return OldMinWidth;
}
AFX_INLINE BOOL CXTPReportColumn::IsFiltrable() {
	return m_bFiltrable;
}
AFX_INLINE BOOL CXTPReportColumn::SetFiltrable(BOOL bFiltrable) {
	BOOL bOldFiltrable = m_bFiltrable; m_bFiltrable = bFiltrable; return bOldFiltrable;
}
AFX_INLINE BOOL CXTPReportColumn::IsResizable() {
	return m_bIsResizable;
}
AFX_INLINE void CXTPReportColumn::EnableResize(BOOL bIsResizable) {
	m_bIsResizable = bIsResizable;
}
AFX_INLINE BOOL CXTPReportColumn::IsSortable() {
	return m_bSortable;
}
AFX_INLINE BOOL CXTPReportColumn::IsGroupable() {
	return m_bGroupable;
}
AFX_INLINE BOOL CXTPReportColumn::IsAllowDragging() {
	return m_bAllowDrag;
}
AFX_INLINE int CXTPReportColumn::SetAlignment(int nAlignment) {
	int nOld = m_nAlignment; m_nAlignment = nAlignment; return nOld;
}
AFX_INLINE int CXTPReportColumn::GetAlignment() {
	return m_nAlignment;
}
AFX_INLINE BOOL CXTPReportColumn::IsAutoSize() {
	return m_bAutoSize;
}
AFX_INLINE void CXTPReportColumn::SetSortable(BOOL bSortable) {
	m_bSortable = bSortable;
}
AFX_INLINE void CXTPReportColumn::SetGroupable(BOOL bGroupable) {
	m_bGroupable = bGroupable;
}
AFX_INLINE CXTPReportColumns* CXTPReportColumn::GetColumns() {
	return m_pColumns;
}
AFX_INLINE void CXTPReportColumn::SetSortIncreasing(BOOL bSortIncreasing)  {
	m_bSortIncreasing = bSortIncreasing;
}
AFX_INLINE CXTPReportRecordItemEditOptions* CXTPReportColumn::GetEditOptions(){
	return m_pEditOptions;
}
AFX_INLINE BOOL CXTPReportColumn::IsEditable() {
	return m_bEditable;
}
AFX_INLINE void CXTPReportColumn::SetEditable(BOOL bEditable /*= TRUE*/) {
	m_bEditable = bEditable;
}
AFX_INLINE void CXTPReportColumn::AllowRemove(BOOL bAllow) {
	m_bAllowRemove = bAllow;
}


#endif //#if !defined(__XTPREPORTCOLUMN_H__)
