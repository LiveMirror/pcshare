// XTPPopupItem.h: interface for the CXTPPopupItem class.
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
#if !defined(_XTPPOPUPITEM_H__)
#define _XTPPOPUPITEM_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPPopupControl;
class CXTPImageManagerIcon;

//-----------------------------------------------------------------------
// Summary:
//     Enumeration used to determine the icon type of CXTPPopupItem.
// Example:
// <code>
// CXTPPopupItem* pItemIcon = m_wndPopupControl.AddItem(
//     new CXTPPopupItem(CRect(14, 40, 20, 49)));
// pItemIcon->SetIcons(IDB_LOGOOFFICE, 0, xtpPopupItemIconNormal);
// </code>
// See Also:
//     CXTPPopupItem::SetIcon, CXTPPopupItem::SetIcons
//
// <KEYWORDS xtpPopupItemIconNormal, xtpPopupItemIconSelected, xtpPopupItemIconPressed>
//-----------------------------------------------------------------------
enum XTPPopupItemIcon
{
	xtpPopupItemIconNormal = 1,     //normal icon handler
	xtpPopupItemIconSelected = 2,   //selected icon handler
	xtpPopupItemIconPressed = 4     //pressed icon handler
};


//===========================================================================
// Summary:
//     CXTPPopupItem is a class used to management popup item object
//===========================================================================
class _XTP_EXT_CLASS  CXTPPopupItem : public CCmdTarget
{
	DECLARE_DYNCREATE(CXTPPopupItem)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPopupItem object.
	// Parameters:
	//     rcItem        - Bounding rectangle of the item.
	//     lpszCaption   - NULL terminated string specifying the caption
	//                     text.
	//     clrBackground - Background color of the item.
	//     clrBorder     - Border color of the item
	//-----------------------------------------------------------------------
	CXTPPopupItem(LPRECT rcItem = NULL, LPCTSTR lpszCaption = NULL, COLORREF clrBackground = (COLORREF)-1, COLORREF clrBorder = (COLORREF)-1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPopupItem object.
	//-----------------------------------------------------------------------
	~CXTPPopupItem();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the item.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device context.
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get rectangular of an item.
	// Returns:
	//     CRect object containing rectangular of an item.
	//-----------------------------------------------------------------------
	CRect GetRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get button state of an item.
	// Returns:
	//     TRUE if button state is enabled, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL IsButton();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set button state of an item.
	// Parameters:
	//     bButton - TRUE to enable button state, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetButton(BOOL bButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get selected state of an item.
	// Returns:
	//     TRUE if item selected, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL IsSelected();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get pressed state of an item.
	// Returns:
	//     TRUE if item pressed, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL IsPressed();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set ID number for an item.
	// Parameters:
	//     nID - value of ID number.
	//-----------------------------------------------------------------------
	void SetID(int nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get ID number of an item.
	//     window.
	// Returns:
	//     Value of ID number.
	//-----------------------------------------------------------------------
	int GetID();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the text of an item.
	// Parameters:
	//     str -  a CString object that represents the item text.
	//-----------------------------------------------------------------------
	void SetCaption(CString str);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the text of an item.
	// Returns:
	//     A CString object that represents the item text.
	//-----------------------------------------------------------------------
	CString GetCaption();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the color of an item text.
	// Parameters:
	//     clrText -  COLORREF object that represents the color of an item
	//     text.
	//-----------------------------------------------------------------------
	void SetTextColor(COLORREF clrText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the color of an item text.
	// Returns:
	//     A COLORREF object that represents the color of an item text.
	//-----------------------------------------------------------------------
	COLORREF GetTextColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the background color of an item.
	// Returns:
	//     The background color of a popup control item.
	//-----------------------------------------------------------------------
	COLORREF GetBackgroundColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the border color of an item.
	// Returns:
	//     The border color of a popup control item.
	//-----------------------------------------------------------------------
	COLORREF GetBorderColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the alignment of an item text.
	// Parameters:
	//     nAlign -  The alignment style of an item text.
	//-----------------------------------------------------------------------
	void SetTextAlignment(int nAlign);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the alignment of an item text.
	// Returns:
	//     The alignment style of an item text.
	//-----------------------------------------------------------------------
	int GetTextAlignment();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the font of an item text.
	// Parameters:
	//     pFntText -  point to CFont object.
	//-----------------------------------------------------------------------
	void SetTextFont(CFont * pFntText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the font of an item text.
	// Parameters:
	//     pLogfText -  point to LOGFONT structure.
	//-----------------------------------------------------------------------
	void SetTextFont(PLOGFONT pLogfText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the font of an item text.
	// Returns:
	//     Pointer to CFont object.
	//-----------------------------------------------------------------------
	virtual CFont* GetTextFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get hyperlink support of a text
	//     item.
	// Returns:
	//     TRUE if hyperlink support, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL IsHyperLink();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set hyperlink support of a text
	//     item.
	// Parameters:
	//     bHyperLink - TRUE to set hyperlink support, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetHyperLink(BOOL bHyperLink);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get bold state of a text item.
	// Returns:
	//     TRUE if item state is bold, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL IsBold();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set bold state of a text item.
	// Parameters:
	//     bBold - TRUE to set bold state, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetBold(BOOL bBold);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to update rectangular height of a text.
	//-----------------------------------------------------------------------
	void CalculateHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to update rectangular width of a text.
	//-----------------------------------------------------------------------
	void CalculateWidth();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the normal, selected ore pressed
	//     icon for icon item.
	// Parameters:
	//     hIcon - Handle to the icon.
	//     itemIcon - type of icon in use, see XTPPopupItemIcon.
	//-----------------------------------------------------------------------
	void SetIcon(HICON hIcon, XTPPopupItemIcon itemIcon = xtpPopupItemIconNormal);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the normal, selected ore pressed
	//     icon from resource bitmap picture.
	// Parameters:
	//     nIDBitmap - Resource ID for the bitmap.
	//     clrTransparent - transparent color.
	//     itemIcon - type of icon in use, see XTPPopupItemIcon.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL SetIcons(UINT nIDBitmap, COLORREF clrTransparent, int itemIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the normal, selected ore pressed
	//     icon from resource bitmap picture.
	// Parameters:
	//     hBitmap        - bitmap handle to set.
	//     clrTransparent - transparent color.
	//     itemIcon        - type of icon in use, see XTPPopupItemIcon.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL SetIcons(HBITMAP hBitmap, COLORREF clrTransparent, int itemIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get a handle the normal, selected
	//     ore pressed icon from icon item.
	// Returns:
	//     CXTPImageManagerIcon pointer to icon.
	//-----------------------------------------------------------------------
	CXTPImageManagerIcon* GetImage();

	//-------------------------------------------------------------------------
	// Summary:
	//     Calculates offset to move icon in center.
	//-------------------------------------------------------------------------
	void CenterIcon();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieve icon offset.
	// Returns:
	//     Poisition of icon inside item bounding rectangle.
	//-----------------------------------------------------------------------
	CPoint GetIconOffset();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves icon index.
	// Returns:
	//     Icon index.
	// See Also: SetIconIndex, CXTPPopupControl::GetImageManager
	//-----------------------------------------------------------------------
	int GetIconIndex();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set icon index of item.
	// Parameters:
	//     nIconIndex - Icon index to be set.
	// See Also: GetIconIndex, CXTPPopupControl::GetImageManager
	//-----------------------------------------------------------------------
	void SetIconIndex(int nIconIndex);

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to redraw parent control.
	//-------------------------------------------------------------------------
	void RedrawControl();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when item is inserted to control.
	//-------------------------------------------------------------------------
	virtual void OnItemInserted();

protected:
	BOOL m_bButton;                 // Flag of button state
	int m_nID;                      // ID number of an item
	CRect m_rcItem;                 // Rectangular of  an item
	CXTPPopupControl* m_pControl;   // Point to CXTPPopupControl object containing this item

	CFont m_fntText;                // Font of an item text
	BOOL m_bHyperLink;              // Flag of a hyperlink support
	CString m_strCaption;           // Caption string
	int m_nAlign;                   // Alignment style of an item text
	BOOL m_bBold;                   // Flag of bold state text

	COLORREF m_clrText;             // Text color
	COLORREF m_clrBackground;       // Background color
	COLORREF m_clrBorder;           // Border color

	CPoint m_ptOffset;              // Icon offset
	int m_nIndex;                   // Index of item in parent list.
	int m_nIconIndex;               // Icon Index in parent image maanger;

protected:

	friend class CXTPPopupControl;
};



AFX_INLINE CRect CXTPPopupItem::GetRect() {
	return m_rcItem;
}
AFX_INLINE BOOL CXTPPopupItem::IsButton() {
	return m_bButton;
}
AFX_INLINE void CXTPPopupItem::SetButton(BOOL bButton) {
	m_bButton = bButton;
}
AFX_INLINE void CXTPPopupItem::SetID(int nID) {
	m_nID = nID;
}
AFX_INLINE int CXTPPopupItem::GetID() {
	return m_nID;
}
AFX_INLINE COLORREF CXTPPopupItem::GetTextColor() {
	return m_clrText;
}
AFX_INLINE int CXTPPopupItem::GetTextAlignment() {
	return m_nAlign;
}
AFX_INLINE void CXTPPopupItem::SetTextAlignment(int nAlign) {
	m_nAlign = nAlign;
	RedrawControl();
}
AFX_INLINE void CXTPPopupItem::SetCaption(CString str) {
	m_strCaption = str;
	RedrawControl();
}
AFX_INLINE CString CXTPPopupItem::GetCaption() {
	return m_strCaption;
}
AFX_INLINE void CXTPPopupItem::SetTextColor(COLORREF clrText) {
	m_clrText = clrText;
	RedrawControl();
}
AFX_INLINE BOOL CXTPPopupItem::IsHyperLink() {
	return m_bHyperLink;
}
AFX_INLINE BOOL CXTPPopupItem::IsBold() {
	return m_bBold;
}
AFX_INLINE void CXTPPopupItem::SetBold(BOOL bBold) {
	m_bBold = bBold;
	RedrawControl();
}
AFX_INLINE void CXTPPopupItem::SetHyperLink(BOOL bHyperLink) {
	m_bHyperLink = bHyperLink;
}
AFX_INLINE COLORREF CXTPPopupItem::GetBackgroundColor() {
	return m_clrBackground;
}
AFX_INLINE COLORREF CXTPPopupItem::GetBorderColor() {
	return m_clrBorder;
}
AFX_INLINE CPoint CXTPPopupItem::GetIconOffset() {
	return m_ptOffset;
}
AFX_INLINE void CXTPPopupItem::SetIconIndex(int nIconIndex) {
	m_nIconIndex = nIconIndex;
	RedrawControl();
}
AFX_INLINE int CXTPPopupItem::GetIconIndex() {
	return m_nIconIndex == -1? m_nIndex: m_nIconIndex;
}

#endif // !defined(_XTPPOPUPITEM_H__)
