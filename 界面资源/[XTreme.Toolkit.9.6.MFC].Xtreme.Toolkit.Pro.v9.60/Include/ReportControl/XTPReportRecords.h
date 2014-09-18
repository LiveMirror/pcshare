// XTPReportRecords.h: interface for the CXTPReportRecords class.
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
#if !defined(__XTPREPORTRECORDS_H__)
#define __XTPREPORTRECORDS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPReportRecord;

//===========================================================================
// Summary:
//     This class represents a records collection class.
//     It supports an array of CXTPReportRecord pointers.
// Example:
//     See example for CXTPReportRecords::Add method.
// See Also: CXTPReportRecord
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRecords : public CCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs an empty CXTPReportRecord pointer array.
	// Example:
	// <code>
	// // Declare a local CXTPReportRecords object.
	// CXTPReportRecords myList;
	//
	// // Declare a dynamic CXTPReportRecords object.
	// CXTPReportRecords* pTree = new CXTPReportRecords();
	// </code>
	//-----------------------------------------------------------------------
	CXTPReportRecords();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys CXTPReportRecords object. Performs cleanup operations.
	//-----------------------------------------------------------------------
	virtual ~CXTPReportRecords(void);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets the number of CXTPReportRecord elements in this collection.
	// Remarks:
	//     Call this method to retrieve the number of elements in the array.
	//     Because indexes are zero-based, the size is 1 greater than
	//     the largest index.
	// Returns:
	//     The number of items in the collection.
	// Example:
	//     See example for CXTPReportRecords::Add method.
	// See Also: CXTPReportRecords overview
	//-----------------------------------------------------------------------
	int GetCount();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a record at the specified numeric index.
	// Parameters:
	//     nIndex - An integer index that is greater than or equal to 0
	//              and less than the value returned by GetCount.
	// Remarks:
	//     Returns the array element at the specified index.
	// Returns:
	//     The record element currently at this index.
	// Example:
	//     See example for CXTPReportRecords::Add method.
	//-----------------------------------------------------------------------
	CXTPReportRecord* GetAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new Record element to the end of an array.
	// Parameters:
	//     pRecord - The record element to be added to this array.
	// Remarks:
	//     Use this method to add the specified CXTPReportRecord pointer
	//     to the end of the CXTPReportRecords collection.
	// Example:
	// <code>
	// CXTPReportRecords* pList = new CXTPReportRecords();
	// pList->Add(new CXTPReportRecord());
	// pList->Add(new CXTPReportRecord());
	// CXTPReportRecord* pRecord0 = pList->GetAt(0);
	// CXTPReportRecord* pRecord1 = pList->GetAt(1);
	// ASSERT( 2 == pList->GetCount() );
	// pList->RemoveAll();
	// ASSERT( 0 == pList->GetCount() );
	// </code>
	// See Also: CXTPReportRecords overview, GetAt, RemoveAll, GetCount
	//-----------------------------------------------------------------------
	CXTPReportRecord* Add(CXTPReportRecord* pRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all the elements from this array.
	// Remarks:
	//     Removes all the pointers from this array and releases instances
	//     of all stored CXTPReportRecord objects.
	// Example:
	//     See example for CXTPReportRecords::Add method.
	// See Also: CXTPReportRecords overview
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes record by its index.
	// Parameters:
	//     nIndex - index of record to remove.
	//-----------------------------------------------------------------------
	void RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to add record in the specified position.
	// Parameters:
	//     nIndex - Index of record to insert.
	//     pRecord - Record to be inserted.
	//-----------------------------------------------------------------------
	void InsertAt(int nIndex, CXTPReportRecord* pRecord);

private:
	void UpdateIndexes(int nStart = 0);
	void SetVirtualMode(CXTPReportRecord* pVirtualRecord, int nCount);
	BOOL IsVirtualMode();

protected:
	CArray<CXTPReportRecord*, CXTPReportRecord*> m_arrRecords;  // Internal storage array for Record items.
	CXTPReportRecord* m_pVirtualRecord;                // Virtual record.
	int m_nVirtualRecordsCount;                        // Virtual records count.


	friend class CXTPReportControl;
};

AFX_INLINE BOOL CXTPReportRecords::IsVirtualMode() {
	return m_pVirtualRecord != NULL;
}


#endif //#if !defined(__XTPREPORTRECORDS_H__)
