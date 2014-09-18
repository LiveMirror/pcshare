// XTPReportRecord.h: interface for the CXTPReportRecord class.
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
#if !defined(__XTPREPORTRECORD_H__)
#define __XTPREPORTRECORD_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPReportRecordItem;
class CXTPReportRecordItemPreview;
class CXTPReportColumn;
class CXTPReportRecords;

struct XTP_REPORTRECORDITEM_DRAWARGS;
struct XTP_REPORTRECORDITEM_METRICS;

//===========================================================================
// Summary:
//    CXTPReportRecord represents a collection of objects of CXTPReportRecordItem
//    or objects of classes that are children of CXTPReportRecordItem.
//    A collection of such items is one record in ReportControl list.
//    You create an object of CXTPReportRecord by simply calling constructor.
//    After it you can add items to the record.
// Example:
// <code>
// // add new record to the record list
// CXTPReportRecords* pList = new CXTPReportRecords();
// pList->Add(new CXTPReportRecord());
// </code>
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRecord : public CCmdTarget
{
	friend class CXTPReportRecords;
	friend class CXTPReportRow;
	friend class CXTPReportControl;
	friend class CXTPReportRecordItem;
	DECLARE_DYNCREATE(CXTPReportRecord)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportRecord object
	//-----------------------------------------------------------------------
	CXTPReportRecord(void);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPReportRecord object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPReportRecord(void);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the number of items inside record.
	// Returns:
	//     The number of items inside the record.
	// See Also: GetItem
	//-----------------------------------------------------------------------
	int GetItemCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns an item by its index or column.
	// Parameters:
	//     nIndex - zero-based index of item in the collection.
	//     pColumn - A pointer to a column object to find an item by.
	// Remarks:
	//     You use this member function to get an item from collection by
	//     item index. If the given index is less than 0 or greater than
	//     the value returned by GetItemCount, GetItem returns NULL.
	// Returns:
	//     Pointer to the found record item, if any, or NULL otherwise.
	// See Also: GetItemCount
	//-----------------------------------------------------------------------
	CXTPReportRecordItem* GetItem(int nIndex);
	CXTPReportRecordItem* GetItem(CXTPReportColumn* pColumn); // <COMBINE CXTPReportRecord::GetItem@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a preview item.
	// Remarks:
	//     Preview items stores separate from all others items. One
	//     Record can have only one Preview Item. If ReportRecord doesn't
	//     have a preview item, GetItemPreview() returns NULL
	// Returns:
	//     Associated Preview item.
	// See Also: SetPreviewItem
	//-----------------------------------------------------------------------
	CXTPReportRecordItemPreview* GetItemPreview();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets preview item for the record.
	// Parameters:
	//     pItemPreview - Pointer to the associated preview item.
	// Remarks:
	//     Preview items stores separate from all others items. One
	//     Record can have only one Preview Item.
	// See Also: GetItemPreview
	//-----------------------------------------------------------------------
	void SetPreviewItem(CXTPReportRecordItemPreview* pItemPreview);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns record visible state.
	// Remarks:
	//     This member function returns FALSE if the record should be
	//     invisible, TRUE if visible.
	// Returns:
	//     Boolean value that represents visible state.
	// See Also: SetVisible
	//-----------------------------------------------------------------------
	BOOL IsVisible();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets record visibility state.
	// Parameters:
	//     bVisible - TRUE if the record should be visible,
	//                FALSE if the record should be hidden.
	// Returns:
	//     Old record visibility state.
	// See Also: IsVisible
	//-----------------------------------------------------------------------
	BOOL SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns record locked state.
	// Returns:
	//     TRUE if the record is locked, FALSE otherwise.
	// See Also: SetLocked
	//-----------------------------------------------------------------------
	BOOL IsLocked();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets record locked state.
	// Parameters:
	//     bLocked - TRUE if record is locked, FALSE otherwise.
	// Remarks:
	//     You set the record locked when it shouldn't be sorted on hide.
	//     To set record locked or unlocked, you use SetLocked(BOOL)
	// Returns:
	//     Old record locked state.
	// See Also: IsLocked
	//-----------------------------------------------------------------------
	BOOL SetLocked(BOOL bLocked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets record editable state.
	// Parameters:
	//     bEditable - TRUE if the record is editable, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetEditable(BOOL bEditable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns record children.
	// Remarks:
	//     ReportRecord can have a list of child items.
	//     To get the pointer to that list, use GetChilds()
	// Returns:
	//     A collection of record children.
	//-----------------------------------------------------------------------
	CXTPReportRecords* GetChilds();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if record has children records
	// Returns:
	//     TRUE if record has children records
	// See Also: GetChilds
	//-----------------------------------------------------------------------
	BOOL HasChildren();

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all items.
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns an index of the provided record item object.
	// Parameters:
	//     pItem - record item which index need to retrieve.
	// Remarks:
	//    This member function is used to get the zero-based index
	//    of items in the record collection. If the item is not found, it returns
	//    -1
	// Returns:
	//    zero-based index of provided record item
	//-----------------------------------------------------------------------
	int IndexOf(CXTPReportRecordItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates item metrics based on the provided draw arguments.
	//     Could be overridden by descendants.
	// Parameters:
	//     pDrawArgs - Draw arguments for calculating item metrics.
	//     pItemMetrics - Pointer to the metrics item to fill with values.
	//-----------------------------------------------------------------------
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the expanded state of the Record
	// Remarks:
	//     Returns expanded state of the Record. If expanded state is set to
	//     TRUE, the  control tries to draw the record with all its children
	// Returns:
	//     BOOLEAN value the represent expanded state of ReportRecord
	// See Also: SetExpanded
	//-----------------------------------------------------------------------
	BOOL IsExpanded();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the expanded state of the Record
	// Parameters:
	//     bExpanded - TRUE to set record expanded.
	// Remarks:
	//     Returns the expanded state of the Record. If the expanded state is set to
	//     TRUE, the  control tries to draw the record with all its children
	// See Also: IsExpanded
	//-----------------------------------------------------------------------
	void SetExpanded(BOOL bExpanded);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns TRUE if record is filtered.
	// Returns:
	//     TRUE if the record is filtered, FALSE otherwise
	//-----------------------------------------------------------------------
	virtual BOOL IsFiltered();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves record index in the array.
	// Returns:
	//     The record index in the array.
	//-----------------------------------------------------------------------
	int GetIndex();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a record item to the internal records item collection.
	// Parameters:
	//     pItem - Pointer to the new record item.
	// Returns:
	//     Pointer to the recently added record item.
	//-----------------------------------------------------------------------
	CXTPReportRecordItem* AddItem(CXTPReportRecordItem* pItem);

protected:
	CArray<CXTPReportRecordItem*, CXTPReportRecordItem*> m_arrItems;    // An internal storage for record items.

	BOOL m_bVisible;    // Is this record visible or not
	BOOL m_bLocked;     // Stores locked state for the record.

	CXTPReportRecordItemPreview* m_pPreviewItem; // store Preview  Item for record

	CXTPReportRecords* m_pChildren; // A collection of children records.
	BOOL m_bExpanded;               // Contains record expanded state.
	BOOL m_bEditable;               // TRUE if the record is editable, FALSE otherwise.
	int m_nIndex;                   // Index of the record within the collection records.

};

AFX_INLINE int CXTPReportRecord::GetItemCount() const{
	return (int)m_arrItems.GetSize();
}

AFX_INLINE CXTPReportRecordItem* CXTPReportRecord::GetItem(int nIndex) {
	if (nIndex >= 0)
		return m_arrItems.GetAt(nIndex);
	else
		return NULL;
}


AFX_INLINE BOOL CXTPReportRecord::IsVisible() {
	return m_bVisible;
}

AFX_INLINE BOOL CXTPReportRecord::SetVisible(BOOL bVisible) {
	BOOL bOldVisible = m_bVisible; m_bVisible = bVisible; return bOldVisible;
}

AFX_INLINE BOOL CXTPReportRecord::IsLocked() {
	return m_bLocked;
}

AFX_INLINE BOOL CXTPReportRecord::SetLocked(BOOL bLocked) {
	BOOL bOldLocked = m_bLocked; m_bLocked = bLocked; return bOldLocked;
}

AFX_INLINE void CXTPReportRecord::GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* /*pDrawArgs*/, XTP_REPORTRECORDITEM_METRICS* /*pItemMetrics*/) {

}
AFX_INLINE BOOL CXTPReportRecord::IsExpanded() {
	return m_bExpanded;
}

AFX_INLINE void CXTPReportRecord::SetExpanded(BOOL bExpanded) {
	m_bExpanded = bExpanded;
}


#endif //#if !defined(__XTPREPORTRECORD_H__)
