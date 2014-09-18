// XTTabCtrlbuttons.h : interface for the CXTTabCtrlbuttons class.
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

#if !defined(__XTTABCTRLBUTTONS_H_)
#define __XTTABCTRLBUTTONS_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTTabCtrlButtons;
class CXTWndHook;
class CXTTabCtrlBase;


//////////////////////////////////////////////////////////////////////
// Summary: CXTTabCtrlButton is a stand alone structure class.  It is internal used in CXTTabCtrlButtons class.
class CXTTabCtrlButton
{
public:
	CXTTabCtrlButton (DWORD wStyle);

	void Draw(CDC* pDC, COLORREF clrButton);
	DWORD Click(CXTTabCtrlButtons* pWnd, CPoint pt, BOOL bRepeat = FALSE);
	void SetRect(CRect rc);
	CRect GetRect();
	BOOL PtInRect(POINT pt) const;
	void CheckForMouseOver (CWnd* pWnd, CPoint pt);
	void SetEnable(BOOL bEnable);

	CRect m_Rect;
	DWORD m_wStyle;
	CTabCtrl* m_pTabCtrl;
	CXTTabCtrlButtons* m_pButtons;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTTabCtrlButton::CXTTabCtrlButton(DWORD wStyle) {
	m_wStyle = wStyle;
	m_pButtons = 0;
}
AFX_INLINE void CXTTabCtrlButton::SetRect (CRect rc) {
	m_Rect = rc;
}
AFX_INLINE CRect CXTTabCtrlButton::GetRect() {
	return m_Rect;
}
AFX_INLINE BOOL CXTTabCtrlButton::PtInRect (POINT pt) const {
	return m_Rect.PtInRect (pt) != 0;
}

//////////////////////////////////////////////////////////////////////
// Summary: CXTTabCtrlButtonsis a CWnd derived class.  It is internally used in the CXTTabCtrlBase class.
class CXTTabCtrlButtons: public CWnd
{
public:
	CXTTabCtrlButtons();
	~CXTTabCtrlButtons();

	BOOL Create(CXTTabCtrlBase* pCtrlBase);

	int GetWidth();

	void ShowButtons(BOOL bShow);
	BOOL ShowButtons();
	void Refresh();
	void SubclassTabButtons(CWnd* pWnd);
	void SetButtonStyle(DWORD dwFlags);

private:
	CRect AdjustRect();
private:
	CXTTabCtrlButton m_btnClose;
	CXTTabCtrlButton m_btnLeft;
	CXTTabCtrlButton m_btnRight;

	DWORD m_dwFlags;
	CTabCtrl* m_pTabCtrl;
	CXTTabCtrlBase* m_pTabCtrlBase;
	BOOL m_bShow;

	HWND m_hwndSubclassed;
	HWND m_hwndPrimary;

private:

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);

	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	DECLARE_MESSAGE_MAP()

	friend class CXTTabCtrlBase;
	friend class CXTTabCtrlButton;
};

AFX_INLINE BOOL CXTTabCtrlButtons::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

#endif // #if !defined(__XTTABCTRLBUTTONS_H_)
