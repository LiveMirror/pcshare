// XTPPropertyGridItemFont.h interface for the CXTPPropertyGridItemFont class.
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
#if !defined(__XTPPROPERTYGRIDITEMFONT_H__)
#define __XTPPROPERTYGRIDITEMFONT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPPropertyGridItemFont is a CXTPPropertyGridItem derived class.
//     It is used to create a Font value item in a Property Grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemFont : public CXTPPropertyGridItem
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemFont object.
	// Parameters:
	//     strCaption - Caption of the item.
	//     nID        - Identifier of the item.
	//     font       - Initial font value.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemFont(CString strCaption, LOGFONT& font);
	CXTPPropertyGridItemFont(UINT nID, LOGFONT& font); // <COMBINE CXTPPropertyGridItemFont::CXTPPropertyGridItemFont@CString@LOGFONT&>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemFont object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridItemFont();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to change the item's value.
	// Parameters:
	//     font - The new value of the item.
	//-----------------------------------------------------------------------
	void SetFont(LOGFONT& font);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the LOGFONT value of the item.
	// Parameters:
	//     lf - Pointer to the LOGFONT structure to receive the font information.
	//-----------------------------------------------------------------------
	void GetFont(LOGFONT* lf);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to change the item's value.
	// Parameters:
	//     clr - The new COLORREF value of the item.
	//-----------------------------------------------------------------------
	void SetColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call the member to get the COLORREF value of this item.
	// Returns:
	//     The COLORREF value of the item.
	//-----------------------------------------------------------------------
	COLORREF GetColor();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses the in-place button.
	//     Override the method to show an item-specific dialog.
	// Remarks:
	//     The in-place button is the button that the user presses to
	//     display the font picker dialog.
	//-----------------------------------------------------------------------
	virtual void OnInplaceButtonDown();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item is drawn. Override this function if
	//     needed.
	// Parameters:
	//     dc      - Pointer to a valid device context.
	//     rcValue - Bounding rectangle of the item.
	// Returns:
	//     TRUE if the item is self-drawn.
	//-----------------------------------------------------------------------
	BOOL OnDrawItemValue(CDC& dc, CRect rcValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the bounding rectangle of the item value's text.
	//     Override this member function to change it.
	// Returns:
	//     A CRect object that represents the bounding rectangle of the
	//     item value's text.
	//-----------------------------------------------------------------------
	CRect GetValueRect();

private:
	static UINT CALLBACK FontDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


protected:

	LOGFONT m_lfValue;      // Value of the item, this holds the current font value.
	COLORREF m_clrValue;    // Font color.


protected:
	DECLARE_DYNAMIC(CXTPPropertyGridItemFont)

};

//////////////////////////////////////////////////////////////////////

#endif // #if !defined(__XTPPROPERTYGRIDITEMFONT_H__)
