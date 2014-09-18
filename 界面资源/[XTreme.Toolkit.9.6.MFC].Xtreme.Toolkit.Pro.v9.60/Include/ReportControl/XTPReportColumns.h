// CXTPReportColumns.h: interface for the CXTPReportColumns class.
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
#if !defined(__XTPREPORTCOLUMNS_H__)
#define __XTPREPORTCOLUMNS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPReportColumn;
class CXTPReportHeader;
class CXTPReportControl;
class CXTPReportColumnOrder;
class CXTPPropExchange;

//===========================================================================
// Summary:
//     This class is a collection of report columns objects,
//     each of which can be referred to by its zero-based index.
// Remarks:
//     It provides main operations to manipulate with list of columns:
//          * Adding/removing columns
//          * Showing columns header
//          * Working with field chooser
//          * Working with "group by" field
//          * (Re)Storing column settings
//     To create an object of that class simply call it constructor with
//     one parameter - pointer to the Report Control object
//===========================================================================
class _XTP_EXT_CLASS CXTPReportColumns : public CCmdTarget
{
	friend class CXTPReportColumn;
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportColumns object
	// Parameters:
	//     pControl - Pointer to the parent Report Control object.
	//-----------------------------------------------------------------------
	CXTPReportColumns(CXTPReportControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPReportColumns object,
	// Remarks:
	//     handles cleanup and deallocation, including all of objects
	//     presented in list by calling Clear() member function
	//-----------------------------------------------------------------------
	virtual ~CXTPReportColumns(void);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the total number of column items in the collection.
	// Returns:
	//     Total count of column items in the collection.
	//-----------------------------------------------------------------------
	int GetCount();

	//-----------------------------------------------------------------------
	// Summary:
	//     Clears columns collection items.
	//-----------------------------------------------------------------------
	void Clear();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the number of visible columns in the collection.
	// Remarks:
	//     In opposite to GetCount() member function this one return
	//     count only those columns that have visibility property set to true
	// Returns:
	//     The number of visible columns in the collection.
	//-----------------------------------------------------------------------
	int GetVisibleColumnsCount();

	// ---------------------------------------------------------------------
	// Summary:
	//     Returns specific column item by its numeric index.
	// Parameters:
	//     nIndex - An integer index that is greater than or equal to 0 and
	//              less than the value returned by GetCount().
	// Remarks:
	//     Returns the columns list element at the specified index. Passing
	//     a negative value or a value greater than the value returned by
	//     GetCount() will return NULL.
	// Returns:
	//     The pointer to column array element currently at this index.
	// ---------------------------------------------------------------------
	CXTPReportColumn* GetAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a column at specified visible position.
	// Parameters:
	//     nIndex - Index of column that is visible on position number nIndex.
	// Remarks:
	//     Returns first visible column before column with given index
	//     or column with given index.
	//     Passing a negative value or a value greater than the value returned by
	//     GetCount() or if visible element not found will return NULL.
	// Returns:
	//     Pointer to column object.
	//-----------------------------------------------------------------------
	CXTPReportColumn* GetVisibleAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets specific indentation column.
	// Remarks:
	//     Indentation column used for representing it ends in
	//     grouping mode. Indentation column usually it is the first column
	//     in list.
	// Returns:
	//     An indentation column.
	//-----------------------------------------------------------------------
	CXTPReportColumn* GetIndentColumn();

	//-----------------------------------------------------------------------
	// Summary:
	//     Finds the column by its item index.
	// Parameters:
	//     nItemIndex - Column item index.
	// Remarks:
	//     Searches the list sequentially to find the first element matching
	//     the specified item index column's value. If no matching found
	//     NULL is returned
	// Returns:
	//     Found column.
	//-----------------------------------------------------------------------
	CXTPReportColumn* Find(int nItemIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds one more column item to the collection.
	// Parameters:
	//     pColumn - A column to add to the collection.
	// Remarks:
	//     Adds a new element to the end of a collection, growing the
	//     collection by 1
	//-----------------------------------------------------------------------
	void Add(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes column from the collection.
	// Parameters:
	//     pColumn - column to be removed.
	//-----------------------------------------------------------------------
	void Remove(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Clear sort order for each column.
	//-----------------------------------------------------------------------
	void ResetSortOrder();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns pointer to the associated report header object.
	// Returns:
	//     A pointer to the associated report header.
	//-----------------------------------------------------------------------
	CXTPReportHeader* GetReportHeader();

	//-----------------------------------------------------------------------
	// Summary:
	//     Changes column order in the collection.
	// Parameters:
	//     nNewOrder - A new column order index.
	//     nOldIndex - Index of the changing column.
	// Remarks:
	//     Moves column with index nItemIndex to the new position with
	//     nNewOrder. If any parameter's value is negative, no changes
	//     will be done and returns -1
	// Returns:
	//     New column order index.
	//-----------------------------------------------------------------------
	int ChangeColumnOrder(int nNewOrder, int nOldIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get index of the column.
	// Parameters:
	//     pColumn - The column.
	// Remarks:
	//     Searches for the first occurrence of the given Column, beginning
	//     the search at index, and first position for equality using pointers
	//     equality. If no matching found return -1
	// Returns:
	//     The index of the column;
	//-----------------------------------------------------------------------
	int IndexOf(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts provided column to the sort columns array according to
	//     the columns order in the group columns array.
	// Parameters:
	//     pColumn - A column to insert.
	// Returns:
	//     The index of the column inserted into the sort columns array.
	//-----------------------------------------------------------------------
	void InsertSortColumn(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a sort order for the column.
	// Parameters:
	//     pColumn - Pointer to the specified column object.
	//     bIncreasing - TRUE to sort increasing.
	// Remarks:
	//     Sets given column as the sorted column, reset previous sort order
	//-----------------------------------------------------------------------
	void SetSortColumn(CXTPReportColumn* pColumn, BOOL bIncreasing);

	//-----------------------------------------------------------------------
	// Summary:
	//     Provides a user with a columns groups order array.
	// Returns:
	//     A pointer to the columns groups order array.
	//-----------------------------------------------------------------------
	CXTPReportColumnOrder* GetGroupsOrder();

	//-----------------------------------------------------------------------
	// Summary:
	//     Provides a user with a columns sort order array.
	// Returns:
	//     A pointer to the columns sort order array.
	//-----------------------------------------------------------------------
	CXTPReportColumnOrder* GetSortOrder();

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes configuration to/from the provided archive stream.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

protected:

	CXTPReportHeader* m_pReportHeader;      // List header member.
	CXTPReportColumnOrder* m_pGroupsOrder;  // Container for the columns order in auto grouping mode.
	CXTPReportColumnOrder* m_pSortOrder;    // Container for the columns sort order.
	CArray<CXTPReportColumn*, CXTPReportColumn*> m_arrColumns;  // Storage for column items.
	CXTPReportControl* m_pControl;          // Pointer to the associated report control object.
	CXTPReportColumn* m_pTreeColumn;       // Column containing tree view images.

};

AFX_INLINE int CXTPReportColumns::GetCount() {
	return (int)m_arrColumns.GetSize();
}
AFX_INLINE CXTPReportColumn* CXTPReportColumns::GetAt(int nIndex) {
	return (nIndex >= 0 && nIndex < GetCount()) ? m_arrColumns.GetAt(nIndex) : NULL;
}
AFX_INLINE CXTPReportColumnOrder* CXTPReportColumns::GetGroupsOrder() {
	return m_pGroupsOrder;
}
AFX_INLINE CXTPReportColumnOrder* CXTPReportColumns::GetSortOrder() {
	return m_pSortOrder;
}
AFX_INLINE CXTPReportColumn* CXTPReportColumns::GetIndentColumn() {
	return (GetCount() > 0) ? m_arrColumns.GetAt(0) : NULL;
}
AFX_INLINE CXTPReportHeader* CXTPReportColumns::GetReportHeader() {
	return m_pReportHeader;
}


//===========================================================================
// Summary:
//     Represents column order, used for sorting and grouping.
// Remarks:
//     This class is a collection of sorted columns objects,
//     each of which can be referred to by its zero-based index.
//     It provides main operations to manipulate with list of columns:
//     Adding/removing columns, get at given position, clear collection,
//     get index of object by it pointer
//     You create object simply by calling constructor with one parameter
//     pointer to the columns collection
//===========================================================================
class _XTP_EXT_CLASS CXTPReportColumnOrder : public CCmdTarget
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Default column order constructor.
	// Parameters:
	//     pColumns - Parent columns collection.
	//-----------------------------------------------------------------------
	CXTPReportColumnOrder(CXTPReportColumns* pColumns);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns specific column item by its numeric index.
	// Parameters:
	//     nIndex - An integer index that is greater than or equal to 0 and
	//              less than the value returned by GetCount().
	// Remarks:
	//     Returns the columns list element at the specified index.
	//     Passing a negative value or a value greater than the value
	//     returned by GetCount() will return NULL.
	// Returns:
	//     The pointer to column array element currently at this index.
	//-----------------------------------------------------------------------
	CXTPReportColumn* GetAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts specified column at a specified position.
	// Parameters:
	//     nIndex - Order index to insert column to.
	//     pColumn - A pointer to the inserted column.
	// Returns:
	//     The index of the inserted column.
	//     -1 in case of an error.
	//-----------------------------------------------------------------------
	int InsertAt(int nIndex, CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes specified column at a specified position from the order array.
	// Parameters:
	//     nIndex - Order index of the column to remove.
	//-----------------------------------------------------------------------
	void RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes specified column from order array.
	// Parameters:
	//     pColumn - A column to remove from the order array.
	//-----------------------------------------------------------------------
	void Remove(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts specified column at the end of the order list.
	// Parameters:
	//     pColumn         - An added column pointer.
	//     bSortIncreasing - TRUE if this column should be sorted in
	//                            increasing order, FALSE for decreasing order.
	// Remarks:
	//     Adds a new element to the end of a collection, growing the
	//     collection by 1
	// Returns:
	//     The index of the added element.
	//-----------------------------------------------------------------------
	int Add(CXTPReportColumn* pColumn);
	int Add(CXTPReportColumn* pColumn, BOOL bSortIncreasing); // <COMBINE CXTPReportColumnOrder::Add@CXTPReportColumn*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get index of the column.
	// Parameters:
	//     pColumn - The column.
	// Remarks:
	//     Searches for the first occurrence of the given Column, beginning
	//     the search at index, and first position for equality using pointers
	//     If no matching found return -1
	// Returns:
	//     The index of the column;
	//-----------------------------------------------------------------------
	int IndexOf(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Clears all stored order information.
	//-----------------------------------------------------------------------
	void Clear();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the number of stored order items.
	// Returns:
	//     The count of items stored in the order array.
	//-----------------------------------------------------------------------
	int GetCount();

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes column order settings to/from the provided archive stream.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);


protected:
	CXTPReportColumns* m_pColumns;  // Parent columns array.

	CArray<CXTPReportColumn*, CXTPReportColumn*> m_arrColumns;  // Storage for column items with the corresponding order.

};

#endif // #if !defined(__XTPREPORTCOLUMNS_H__)
