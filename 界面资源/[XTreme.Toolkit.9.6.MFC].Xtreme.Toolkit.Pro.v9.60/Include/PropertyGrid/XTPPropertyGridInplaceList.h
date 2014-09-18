// XTPPropertyGridInplaceList.h interface for the CXTPPropertyGridInplaceList class.
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
#if !defined(__XTPPROPERTYGRIDINPLACELIST_H__)
#define __XTPPROPERTYGRIDINPLACELIST_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// class forwards.

class CXTPPropertyGridView;
class CXTPPropertyGridItem;

//===========================================================================
// Summary:
//     CXTPPropertyGridInplaceList is a CListBox derived class.
//     It is an internal class used by Property Grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridInplaceList : public CListBox
{
	DECLARE_DYNAMIC(CXTPPropertyGridInplaceList)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridInplaceList object
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridInplaceList object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridInplaceList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to create in-place list control.
	// Parameters:
	//     pItem - Parent item of the list.
	//     rect  -  Specifies the size and position of the item.
	//-----------------------------------------------------------------------
	virtual void Create(CXTPPropertyGridItem* pItem, CRect rect);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the parent item is destroyed.
	//-----------------------------------------------------------------------
	virtual void DestroyItem();

private:

	CXTPPropertyGridItem* m_pItem;

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to cancel user changes.
	//-----------------------------------------------------------------------
	virtual void Cancel(void);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to apply user changes.
	//-----------------------------------------------------------------------
	virtual void Apply(void);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	//{{AFX_VIRTUAL(CXTPPropertyGridInplaceList)
	void PostNcDestroy();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPPropertyGridInplaceList)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	friend class CXTPPropertyGridItem;
};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPPropertyGridInplaceList::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTPPropertyGridInplaceList::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
	return CListBox::Create(dwStyle, rect, pParentWnd, nID);
}

#endif // #if !defined(__XTPPROPERTYGRIDINPLACELIST_H__)
