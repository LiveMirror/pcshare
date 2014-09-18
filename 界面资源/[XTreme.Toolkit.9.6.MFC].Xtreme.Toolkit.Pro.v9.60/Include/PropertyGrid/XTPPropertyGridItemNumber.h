// XTPPropertyGridItemNumber.h interface for the CXTPPropertyGridItemNumber class.
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
#if !defined(__XTPPROPERTYGRIDITEMNUMBER_H__)
#define __XTPPROPERTYGRIDITEMNUMBER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPPropertyGridItemNumber is a CXTPPropertyGridItem derived class.
//     It is used to create an integer value item in a Property Grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemNumber : public CXTPPropertyGridItem
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemNumber object
	// Parameters:
	//     strCaption  - Caption of the item.
	//     nID         - Identifier of the item.
	//     nValue      - Initial value of item.
	//     pBindNumber - If not NULL, then the value of this item
	//                   is bound the value of this variable.
	// Remarks:
	//     Class CXTPPropertyGridItemNumber has no default constructor.
	//
	//          When using the second constructor, the Identifier (nID) of the
	//          second constructor can be linked with a STRINGTABLE resource
	//          with the same id in such form "Caption\\nDescription".
	//
	//          BINDING:
	//            Variables can be bound to an item in two ways, the first is
	//            to pass in a variable at the time of creation, the second allows
	//            variables to be bound to an item after creation with the
	//            BindToNumber member.
	//
	//            Bound variables store the values of the property grid items
	//            and can be accessed without using the property grid methods
	//            and properties.  Bound variables allow the property grid to
	//            store data in variables.  When the value of a PropertyGridItem
	//            is changed, the value of the bound variable will be changed to
	//            the PropertyGridItem value.  The advantage of binding is that
	//            the variable can be used and manipulated without using
	//            PropertyGridItem methods and properties.
	//
	//            NOTE:  If the value of the variable is changed without using
	//            the PropertyGrid, the PropertyGridItem value will not be
	//            updated until you call CXTPPropertyGrid::Refresh.
	// See Also: BindToNumber
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemNumber(CString strCaption, long nValue = 0, long* pBindNumber = NULL);
	CXTPPropertyGridItemNumber(UINT nID, long nValue = 0, long* pBindNumber = NULL);  // <COMBINE CXTPPropertyGridItemNumber::CXTPPropertyGridItemNumber@CString@long@long*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemNumber object
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridItemNumber();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to change the item's value.
	// Parameters:
	//     nValue - The new integer value of the item.
	//-----------------------------------------------------------------------
	void SetNumber(long nValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the integer value of the item.
	// Returns:
	//     The integer value of the item.
	//-----------------------------------------------------------------------
	long GetNumber();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to bind an item to a long value.
	// Parameters:
	//     pBindNumber - Long value to bind to item.
	// Remarks:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToNumber member.
	//
	//     Bound variables store the values of the property grid items
	//     and can be accessed without using the property grid methods
	//     and properties.  Bound variables allow the property grid to
	//     store data in variables.  When the value of a PropertyGridItem
	//     is changed, the value of the bound variable will be changed to
	//     the PropertyGridItem value.  The advantage of binding is that
	//     the variable can be used and manipulated without using
	//     PropertyGridItem methods and properties.
	//
	//     NOTE:  If the value of the variable is changed without using
	//     the PropertyGrid, the PropertyGridItem value will not be
	//     updated until you call CXTPPropertyGrid::Refresh.
	//-----------------------------------------------------------------------
	void BindToNumber(long* pBindNumber);


protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to change an item's value.
	//     Override this method to add new functionality.
	//     You should call the base class version of this function from your
	//     override.
	// Parameters:
	//     strValue - New value of the item.
	//-----------------------------------------------------------------------
	virtual void SetValue(CString strValue);

	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called before the item becomes visible in the
	//     property grid.
	// Remarks:
	//     Before the item is inserted, it is first check to see if it
	//     is bound to a variable, if it is, then the value of the item
	//     is updated with the value stored in the bound variable.
	//
	//          OnBeforeInsert is called when an item is inserted,
	//          when a category is inserted, when a category or item is
	//          expanded, and when the sort property has changed.
	//-------------------------------------------------------------------------
	virtual void OnBeforeInsert();

protected:

	long m_nValue;          // Value of the item.
	long* m_pBindNumber;    // Binded object.  This is a pointer to the variable bound to this item.

private:

	DECLARE_DYNAMIC(CXTPPropertyGridItemNumber)
};


//===========================================================================
// Summary:
//     CXTPPropertyGridItemDouble is a CXTPPropertyGridItem derived class.
//     It is used to create an double value item in a Property Grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemDouble : public CXTPPropertyGridItem
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemDouble object.
	// Parameters:
	//     strCaption  - Caption of the item.
	//     nID         - Identifier of the item.
	//     fValue      - Initial value.
	//     strFormat   - The number of digits after the decimal point.
	//                   For example "%0.5f" would display 5 digits
	//                   past the decimal place.
	//     pBindNumber - If not NULL, then the value of this item
	//                   is bound the value of this variable.
	// Remarks:
	//     Class CXTPPropertyGridItemDouble has no default constructor.
	//
	//          When using the second constructor, the Identifier (nID) of the
	//          second constructor can be linked with a STRINGTABLE resource
	//          with the same id in such form "Caption\\nDescription".
	//
	//          BINDING:
	//            Variables can be bound to an item in two ways, the first is
	//            to pass in a variable at the time of creation, the second allows
	//            variables to be bound to an item after creation with the
	//            BindToDouble member.
	//
	//            Bound variables store the values of the property grid items
	//            and can be accessed without using the property grid methods
	//            and properties.  Bound variables allow the property grid to
	//            store data in variables.  When the value of a PropertyGridItem
	//            is changed, the value of the bound variable will be changed to
	//            the PropertyGridItem value.  The advantage of binding is that
	//            the variable can be used and manipulated without using
	//            PropertyGridItem methods and properties.
	//
	//            NOTE:  If the value of the variable is changed without using
	//            the PropertyGrid, the PropertyGridItem value will not be
	//            updated until you call CXTPPropertyGrid::Refresh.
	// See Also: BindToDouble
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemDouble(CString strCaption, double fValue = 0, LPCTSTR strFormat = _T("%0.2f"), double* pBindNumber = NULL);
	CXTPPropertyGridItemDouble(UINT nID, double fValue = 0, LPCTSTR strFormat = _T("%0.2f"), double* pBindNumber = NULL); // <COMBINE CXTPPropertyGridItemDouble::CXTPPropertyGridItemDouble@CString@double@LPCTSTR@double*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemDouble object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridItemDouble();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to change the item's value.
	// Parameters:
	//     fValue - The new double value of the item.
	//-----------------------------------------------------------------------
	void SetDouble(double fValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the double value of the item.
	// Returns:
	//     The double value of the item.
	//-----------------------------------------------------------------------
	double GetDouble();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to bind the item to a double value.
	// Parameters:
	//     pBindDouble - Double value to bind to item.
	// Remarks:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToDouble member.
	//
	//     Bound variables store the values of the property grid items
	//     and can be accessed without using the property grid methods
	//     and properties.  Bound variables allow the property grid to
	//     store data in variables.  When the value of a PropertyGridItem
	//     is changed, the value of the bound variable will be changed to
	//     the PropertyGridItem value.  The advantage of binding is that
	//     the variable can be used and manipulated without using
	//     PropertyGridItem methods and properties.
	//
	//     NOTE:  If the value of the variable is changed without using
	//     the PropertyGrid, the PropertyGridItem value will not be
	//     updated until you call CXTPPropertyGrid::Refresh.
	//-----------------------------------------------------------------------
	void BindToDouble(double* pBindDouble);


protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to change an item's value.
	//     Override this method to add new functionality.
	//     You should call the base class version of this function from your
	//     override.
	// Parameters:
	//     strValue - New value of the item.
	//-----------------------------------------------------------------------
	virtual void SetValue(CString strValue);

	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called before the item becomes visible in the
	//     property grid.
	// Remarks:
	//     Before the item is inserted, it is first check to see if it
	//     is bound to a variable, if it is, then the value of the item
	//     is updated with the value stored in the bound variable.
	//
	//          OnBeforeInsert is called when an item is inserted,
	//          when a category is inserted, when a category or item is
	//          expanded, and when the sort property has changed.
	//-------------------------------------------------------------------------
	void OnBeforeInsert();


protected:

	double m_fValue;        // Double value of item.
	double* m_pBindDouble;  // Binded value.  This is a pointer to the variable bound to this item.

private:
	DECLARE_DYNAMIC(CXTPPropertyGridItemDouble)

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE long CXTPPropertyGridItemNumber::GetNumber() {
	return m_nValue;
}

//////////////////////////////////////////////////////////////////////

AFX_INLINE double CXTPPropertyGridItemDouble::GetDouble() {
	return m_fValue;
}

#endif // #if !defined(__XTPPROPERTYGRIDITEMNUMBER_H__)
