// XTPDockBar.h : interface for the CXTPDockBar class.
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
#if !defined(__XTPDOCKBAR_H__)
#define __XTPDOCKBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPCommandBarsDefines.h"

class CXTPToolBar;
class CXTPCommandBars;

//===========================================================================
// Summary:
//     CXTPDockBar is a CWnd derived class. It is used internally by
//     CXTPCommandBars.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockBar : public CWnd
{

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Array of CXTPToolBar objects.
	//-----------------------------------------------------------------------
	typedef CArray<CXTPToolBar*, CXTPToolBar*> CToolBarArray;

private:
	class CXTPDockInfoArray;
	struct XTP_DOCK_INFO;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockBar object
	// Parameters:
	//     pCommandBars - Points to a CXTPCommandBars object
	//-----------------------------------------------------------------------
	CXTPDockBar(CXTPCommandBars* pCommandBars);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockBar object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPDockBar();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the dockbar position.
	// Returns:
	//     Position of the dock bar.
	//-----------------------------------------------------------------------
	XTPBarPosition GetPosition();

	//-----------------------------------------------------------------------
	// Summary:
	//     Checks if dockbar is vertical.
	// Returns:
	//     TRUE if docked vertically; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL IsVerticalPosition();

private:
	BOOL RemoveCommandBar(CXTPToolBar* pBar, int nPosExclude = -1);
	BOOL Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID);
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	BOOL DockCommandBar(CXTPToolBar* bBar);
	void DockCommandBar(CXTPToolBar* pBar, LPCRECT lpRect);

	CXTPToolBar* GetDockedCommandBar(int nPos) const;
	int GetDockedCount() const;
	int GetDockedVisibleCount() const;
	int FindBar(CXTPToolBar* pBar, int nPosExclude = -1);
	int Insert(CXTPToolBar* pBarIns, CRect rect, CPoint ptMid);

	void GetVisibleToolbars(int nPos, CToolBarArray& arrBars);
	void AdjustStretchBars();

	CFrameWnd* GetDockingFrame()
	{
		return (CFrameWnd*)GetParent();
	}
	int AdjustRow(CToolBarArray&, CPoint pt, int nLength, BOOL bHorz, AFX_SIZEPARENTPARAMS* lpLayout);
	int _AdjustRow(CToolBarArray&, CPoint pt, int nLength, BOOL bHorz, AFX_SIZEPARENTPARAMS* lpLayout, int& nRemove);
	int _GetMode(BOOL bHorz, CXTPToolBar* pBar);

	CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz, AFX_SIZEPARENTPARAMS* lpLayout);
	CSize CalcDynamicLayout(int, DWORD nMode, AFX_SIZEPARENTPARAMS* lpLayout);

private:
	//{{AFX_MSG(CXTPDockBar)
	afx_msg void OnPaint();
	afx_msg LRESULT OnPrint(WPARAM wParam, LPARAM);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnSizeParent(WPARAM, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_DYNAMIC(CXTPDockBar)
	friend class CXTPToolBar;
	friend class CXTPCommandBars;
	friend class CXTPReBar;

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	DECLARE_MESSAGE_MAP()

private:
	DWORD m_dwStyle;
	CToolBarArray m_arrBars;
	CXTPCommandBars* m_pCommandBars;
};

AFX_INLINE BOOL CXTPDockBar::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

#endif // #if !defined(__XTPDOCKBAR_H__)
