// XTSortClass.h interface for the CXTSortClass  class.
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
#if !defined(__XTSORTCLASS_H__)
#define __XTSORTCLASS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------
// Summary:
//     Enumeration used to determine sort type.
// Remarks:
//     XTSortType type defines the constants used by the CXTSortClass
//     to determine for what type of sorting to perform.
// See Also:
//     CXTSortClass::Sort
//
// <KEYWORDS xtSortInt, xtSortString, xtSortDateTime, xtSortDecimal>
// ----------------------------------------------------------------------
enum XTSortType
{
	xtSortInt = 1,  // Sort type integer.
	xtSortString,   // Sort type string.
	xtSortDateTime, // Sort type date / time.
	xtSortDecimal   // Sort type decimal.
};

//===========================================================================
// Summary:
//     CXTSortClass is a stand alone class. This class will sort a list control
//     by a column of text, integer, float or date/time type. It could be
//     easily extended for other data types. To use this class, instantiate
//     a CXTSortClass object, passing in a pointer to the list control as
//     the first argument and the column index as the second argument.
//
// Example:
// <code>CXTSortClass sortClass(pListCtrl, 0);</code>
//
//     Then call the sort method setting ascending as the first argument either
//     TRUE or FALSE, and the type of sort to perform in the second argument.
//
// <code>sortClass.Sort(TRUE, xtSortString);</code>
//===========================================================================
class _XT_EXT_CLASS CXTSortClass
{
private:
	struct SORT_ITEM;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTSortClass object
	// Parameters:
	//     pListCtrl - Pointer to a CListCtrl object.
	//     nCol - Index of the column to be sorted.
	//-----------------------------------------------------------------------
	CXTSortClass(CListCtrl* pListCtrl,const int nCol);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTSortClass object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTSortClass();


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to perform the actual sort procedure.
	// Parameters:
	//     bAsc  - true to sort ascending.
	//     eType - The type of sort to perform can be one of the values listed in the Remarks section.
	// Remarks:
	//     Sort type can be one of the following.<p/>
	//     * <b>xtSortInt</b> Sort type int.
	//     * <b>xtSortString</b> Sort type string.
	//     * <b>xtSortDateTime</b> Sort type date / time.
	//     * <b>xtSortDecimal</b> Sort type decimal.
	//-----------------------------------------------------------------------
	virtual void Sort(bool bAsc,XTSortType eType);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This callback member function is called to compare two data items during
	//     sorting operations. The comparison function must return a negative
	//     value if the first item should precede the second, a positive value
	//     if the first item should follow the second, or zero if the two items
	//     are equivalent.
	// Parameters:
	//     lParam1    - Specify the item data for the two items being compared.
	//     lParam2    - Specify the item data for the two items being compared.
	//     lParamSort - Specifies the application defined value that is passed to the
	//     comparison function.
	//-----------------------------------------------------------------------
	static int CALLBACK Compare(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort);

protected:
	CListCtrl* m_pListCtrl; // Pointer to the CListCtrl object to perform the sort on
};

#endif // #if !defined(__XTSORTCLASS_H__)
