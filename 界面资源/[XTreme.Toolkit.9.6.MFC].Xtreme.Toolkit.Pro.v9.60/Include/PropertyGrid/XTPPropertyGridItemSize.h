// XTPPropertyGridItemSize.h interface for the CXTPPropertyGridItemSize class.
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
#if !defined(__XTPPROPERTYGRIDITEMSIZE_H__)
#define __XTPPROPERTYGRIDITEMSIZE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPPropertyGridItemSize is a CXTPPropertyGridItem derived class.
//     It is used to create a size value item in a Property Grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemSize : public CXTPPropertyGridItem
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemSize object.
	// Parameters:
	//     strCaption - Caption of the item.
	//     nID        - Identifier of the item.
	//     size       - Initial value of item.
	//     pBindSize  - If not NULL, then the value of this item
	//                   is bound the value of this variable.
	// Remarks:
	//     Class CXTPPropertyGridItemSize has no default constructor.
	//
	//          When using the second constructor, the Identifier (nID) of the
	//          second constructor can be linked with a STRINGTABLE resource
	//          with the same id in such form "Caption\\nDescription".
	//
	//          BINDING:
	//            Variables can be bound to an item in two ways, the first is
	//            to pass in a variable at the time of creation, the second allows
	//            variables to be bound to an item after creation with the
	//            BindToSize member.
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
	// See Also: BindToSize
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemSize(CString strCaption, CSize size = 0, CSize* pBindSize = NULL);
	CXTPPropertyGridItemSize(UINT nID, CSize size = 0, CSize* pBindSize = NULL); //<COMBINE CXTPPropertyGridItemSize::CXTPPropertyGridItemSize@CString@CSize@CSize*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemSize object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridItemSize();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to change the item's value.
	// Parameters:
	//     size - The new size value of the item.
	//-----------------------------------------------------------------------
	void SetSize(CSize size);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the size value of the item.
	// Returns:
	//     A CSize object representing the size value of the item.
	//-----------------------------------------------------------------------
	CSize GetSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to bind an item to a CSize object.
	// Parameters:
	//     pBindSize - CSize object to bind to item.
	// Remarks:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToSize member.
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
	void BindToSize(CSize* pBindSize);

protected:

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

private:

	void SetWidth(CString strWidth);
	void SetHeight(CString strHeight);

	CString SizeToString(CSize size);
	CSize StringToSize(CString str);

	virtual void OnAddChildItem();
	virtual void SetValue(CString strValue);

	void UpdateChilds();

private:

	class CXTPPropertyGridItemSizeWidth;
	class CXTPPropertyGridItemSizeHeight;

	CSize                          m_szValue;
	CXTPPropertyGridItemSizeWidth*  m_itemWidth;
	CXTPPropertyGridItemSizeHeight* m_itemHeight;

	CSize* m_pBindSize;

private:

	DECLARE_DYNAMIC(CXTPPropertyGridItemSize)

	friend class CXTPPropertyGridItemSizeWidth;
	friend class CXTPPropertyGridItemSizeHeight;

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CSize CXTPPropertyGridItemSize::GetSize() {
	return m_szValue;
}


#endif // #if !defined(__XTPPROPERTYGRIDITEMSIZE_H__)
