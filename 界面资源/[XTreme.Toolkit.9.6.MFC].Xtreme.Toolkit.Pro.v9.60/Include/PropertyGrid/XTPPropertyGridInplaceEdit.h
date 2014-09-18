// XTPPropertyGridInplaceEdit.h interface for the CXTPPropertyGridInplaceEdit class.
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
#if !defined(__XTPPROPERTYGRIDINPLACEEDIT_H__)
#define __XTPPROPERTYGRIDINPLACEEDIT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// class forwards.

#include "CommonPro.Include\XTMaskEditT.h"

class CXTPPropertyGridView;
class CXTPPropertyGridItem;

//===========================================================================
// Summary:
//     CXTPPropertyGridInplaceEdit is a CEdit derived class.
//     It is an internal class used by Property Grid control.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridInplaceEdit : public CXTMaskEditT<CEdit>
{
	DECLARE_DYNAMIC(CXTPPropertyGridInplaceEdit)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridInplaceEdit object
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceEdit();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridInplaceEdit object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridInplaceEdit();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set edit text value.
	// Parameters:
	//     strValue - String value.
	//-----------------------------------------------------------------------
	virtual void SetValue(CString strValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to hide in-place edit control.
	//-----------------------------------------------------------------------
	virtual void HideWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to create in-place edit control.
	// Parameters:
	//     pItem - Points to a CXTPPropertyGridItem object
	//     rect -  Specifies the size and position of the item.
	//-----------------------------------------------------------------------
	virtual void Create(CXTPPropertyGridItem* pItem, CRect rect);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a parent item is destroyed.
	//-----------------------------------------------------------------------
	virtual void DestroyItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves a child property grid item
	// Returns:
	//     A pointer to a CXTPPropertyGridItem object
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* GetItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to select a constraint in child item constraint list.
	// Parameters:
	//     nDirection - Direction of constraint search.
	//     bCircle - TRUE to find by cycle.
	//-----------------------------------------------------------------------
	virtual BOOL SelectConstraint(int nDirection, BOOL bCircle);

private:

	CString              m_strValue;
	CBrush               m_brBack;
	COLORREF             m_clrBack;
	CWnd*                m_pGrid;
	CXTPPropertyGridItem* m_pItem;

protected:
//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_VIRTUAL(CXTPPropertyGridInplaceEdit)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


	//{{AFX_MSG(CXTPPropertyGridInplaceEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnEnKillfocus();
	afx_msg void OnEnSetfocus();
	afx_msg void OnEnChange();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	DECLARE_MESSAGE_MAP()
	friend class CXTPPropertyGridItem;
};

AFX_INLINE CXTPPropertyGridItem* CXTPPropertyGridInplaceEdit::GetItem() {
	return m_pItem;
}
AFX_INLINE BOOL CXTPPropertyGridInplaceEdit::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTPPropertyGridInplaceEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
	return CEdit::Create(dwStyle, rect, pParentWnd, nID);
}

#endif // #if !defined(__XTPPROPERTYGRIDINPLACEEDIT_H__)
