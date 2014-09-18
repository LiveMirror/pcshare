// XTPShortcutBarPane.h interface for the CXTPShortcutBarPane class.
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
#if !defined(__XTPSHORTCUTBARPANE_H__)
#define __XTPSHORTCUTBARPANE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPShortcutBar;


//-----------------------------------------------------------------------
// Summary:
//     CXTPShortcutBarPaneItem is the base class representing an item of the panes.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPShortcutBarPaneItem
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBarPaneItem object.
	// Parameters:
	//     lpszCaption - Caption of the item
	//     pWnd - Child window associated with item
	//     nHeight - Height of the item
	//-----------------------------------------------------------------------
	CXTPShortcutBarPaneItem(LPCTSTR lpszCaption, CWnd* pWnd, int nHeight);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Returns rectangle of caption.
	//-----------------------------------------------------------------------
	CRect GetCaptionRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the caption of the pane.
	//-----------------------------------------------------------------------
	CString GetCaption();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set height of the item.
	// Parameters:
	//     nHeight - Height of client area of the item.
	//-----------------------------------------------------------------------
	void SetHeight(int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine height of the item.
	// Returns:
	//     Height of the item.
	//-----------------------------------------------------------------------
	int GetHeight();

protected:
	CString m_strCaption;   // Caption of the item
	CRect m_rcCaption;      // Rectangle of the caption.
	CRect m_rcClient;       // Bounding rectangle of the client area.
	CWnd* m_pWndClient;     // Associated child window.
	int m_nHeight;          // Height of the item.


	friend class CXTPShortcutBarPane;
};

//===========================================================================
// Summary:
//     CXTPShortcutBarPane is a CWnd derived class. It is used as client area
//     of ShortcutBar control.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarPane : public CWnd
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBarPane object.
	//-----------------------------------------------------------------------
	CXTPShortcutBarPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShortcutBarPane object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPShortcutBarPane();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method creates the CXTPShortcutBarPane control.
	// Parameters:
	//     lpszCaption - Caption of the pane.
	//     pParent - parent CXTPShortcutBar class.
	//-----------------------------------------------------------------------
	BOOL Create(LPCTSTR lpszCaption, CXTPShortcutBar* pParent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the caption of the pane.
	//-----------------------------------------------------------------------
	CString GetCaption();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to add sub item of pane.
	// Parameters:
	//     lpszCaption - Caption of the item that is to be added.
	//     pWnd - Associated child window of the item.
	//     nHeight - Height of the item.
	//-----------------------------------------------------------------------
	CXTPShortcutBarPaneItem* AddItem(LPCTSTR lpszCaption, CWnd* pWnd, int nHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves item with specified index.
	// Parameters:
	//     nIndex - Specifies the zero-based index of the item to retrieve.
	// Returns:
	//     A pointer to a CXTPShortcutBarPaneItem object.
	//-----------------------------------------------------------------------
	CXTPShortcutBarPaneItem* GetItem(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the indent placed around the
	//     client area of the ShortcutBar.  The indent will be placed around the
	//     corresponding CXTPShortcutBarPaneItem when displayed.
	// Parameters:
	//     left   - Amount of space placed between the left client border
	//              and the left border of the ShortcutBar client area.
	//     top    - Amount of space placed between the top client border
	//              and the top border of the ShortcutBar client area.
	//     right  - Amount of space placed between the right client border
	//              and the right border of the ShortcutBar client area.
	//     bottom - Amount of space placed between the bottom client border
	//              and the bottom border of the ShortcutBar client area.
	//-----------------------------------------------------------------------
	void SetIndent(int left, int top, int right, int bottom);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPShortcutBarPane)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:
	CXTPShortcutBar* m_pShortcutBar;                                        // Parent CXTPShortcutBar class.
	CString m_strCaption;                                                   // Caption of the pane.
	CArray<CXTPShortcutBarPaneItem*, CXTPShortcutBarPaneItem*> m_arrItems;  // Array of items.
	CRect m_rcIndent;                                                       // Indentation\padding around the client pane

};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE CString CXTPShortcutBarPane::GetCaption() {
	return m_strCaption;
}
AFX_INLINE void CXTPShortcutBarPane::SetIndent(int left, int top, int right, int bottom) {
	m_rcIndent.SetRect(left, top, right, bottom);
}
AFX_INLINE BOOL CXTPShortcutBarPane::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE CRect CXTPShortcutBarPaneItem::GetCaptionRect() {
	return m_rcCaption;
}
AFX_INLINE int CXTPShortcutBarPaneItem::GetHeight() {
	return m_nHeight;
}
AFX_INLINE CString CXTPShortcutBarPaneItem::GetCaption() {
	return m_strCaption;
}

#endif // !defined(__XTPSHORTCUTBARPANE_H__)
