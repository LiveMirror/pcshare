// XTPPropertyGridItemColor.h interface for the CXTPPropertyGridItemColor class.
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
#if !defined(__XTPPROPERTYGRIDITEMCOLOR_H__)
#define __XTPPROPERTYGRIDITEMCOLOR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPPropertyGridItemColor is a CXTPPropertyGridItem derived class.
//     It is used to create an RGB value item in a Property Grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemColor : public CXTPPropertyGridItem
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemColor object.
	// Parameters:
	//     strCaption - Caption of the item.
	//     nID        - Identifier of the item.
	//     clr        - Initial color value for this item.
	//     pBindColor - If not NULL, then the value of this item
	//                  is bound the value of this variable.
	// Remarks:
	//     Class CXTPPropertyGridItemColor has no default constructor.
	//
	//          When using the second constructor, the Identifier (nID) of the
	//          second constructor can be linked with a STRINGTABLE resource
	//          with the same id in such form "Caption\\nDescription".
	//
	//          BINDING:
	//            Variables can be bound to an item in two ways, the first is
	//            to pass in a variable at the time of creation, the second allows
	//            variables to be bound to an item after creation with the
	//            BindToColor member.
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
	// See Also: BindToColor
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemColor(CString strCaption, COLORREF clr = 0, COLORREF* pBindColor = NULL);
	CXTPPropertyGridItemColor(UINT nID, COLORREF clr = 0, COLORREF* pBindColor = NULL);  // <COMBINE CXTPPropertyGridItemColor::CXTPPropertyGridItemColor@CString@COLORREF@COLORREF*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemColor object
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridItemColor();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to change the item's value.
	// Parameters:
	//     clr - The new RGB value of the item.
	//-----------------------------------------------------------------------
	void SetColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the RGB value of the item.
	// Returns:
	//     An RGB value.
	//-----------------------------------------------------------------------
	COLORREF GetColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     The StringToRGB function converts a string to a COLORREF.
	// Parameters:
	//     str - String that is converted to a COLORREF.
	//           Format of string is RRR;GGG;BBB
	// Returns:
	//     An COLORREF RGB value.
	//-----------------------------------------------------------------------
	static COLORREF AFX_CDECL StringToRGB(CString str);

	//-----------------------------------------------------------------------
	// Summary:
	//     The RGBToString function converts a COLORREF to a string.
	// Parameters:
	//     clr - COLORREF that is converted to a string.
	// Returns:
	//     Returns a string in the format RRR;GGG;BBB
	//-----------------------------------------------------------------------
	static CString AFX_CDECL RGBToString(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to bind an item to a COLORREF object.
	// Parameters:
	//     pBindColor - COLORREF object to bind to item.
	// Remarks:
	//     Variables can be bound to an item in two ways, the first is
	//     to pass in a variable at the time of creation, the second allows
	//     variables to be bound to an item after creation with the
	//     BindToColor member.
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
	void BindToColor(COLORREF* pBindColor);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item is drawn. Override this function if
	//     needed.
	// Parameters:
	//     dc      - Reference to the device context to be used for rendering an image
	//               of the item.
	//     rcValue - Bounding rectangle of the item.
	// Returns:
	//     TRUE if the item is self-drawn.
	//-----------------------------------------------------------------------
	virtual BOOL OnDrawItemValue(CDC& dc, CRect rcValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the bounding rectangle of the item value's text.
	//     Override this member function to change it.
	// Returns:
	//     A CRect object that represents the bounding rectangle of the
	//     item value's text.
	//-----------------------------------------------------------------------
	virtual CRect GetValueRect();

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

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses the in-place button.
	//     Override the method to show an item-specific dialog.
	// Remarks:
	//     The in-place button is the button that the user presses to
	//     display the color picker dialog.
	//-----------------------------------------------------------------------
	virtual void OnInplaceButtonDown();

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

	COLORREF m_clrValue;        // Color value of the item.
	COLORREF* m_pBindColor;     // Pointer to variable bound to this item, this is the color of the bound item.

private:
	DECLARE_DYNAMIC(CXTPPropertyGridItemColor)

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE COLORREF CXTPPropertyGridItemColor::GetColor() {
	return m_clrValue;
}

#endif //#if !defined(__XTPPROPERTYGRIDITEMCOLOR_H__)
