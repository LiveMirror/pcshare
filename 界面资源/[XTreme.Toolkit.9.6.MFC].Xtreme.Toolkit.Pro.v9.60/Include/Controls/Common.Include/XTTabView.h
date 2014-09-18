// XTTabView.h interface for the CXTTabView class.
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
#if !defined(__XTTABVIEW_H__)
#define __XTTABVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTTabView is a multiple inheritance class derived from CCtrlView and
//     CXTTabCtrlBaseEx. CXTTabView can be used to create a view that contains
//     nested views displayed in a tab control. See CXTTabCtrlBaseEx for additional
//     functionality.
//===========================================================================
class _XT_EXT_CLASS CXTTabView : public CCtrlView, public CXTTabCtrlBaseEx
{
	DECLARE_DYNCREATE(CXTTabView)

	friend class CXTTabCtrlBase;
	friend class CXTTabCtrlBaseEx;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTTabView object
	//-----------------------------------------------------------------------
	CXTTabView();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTTabView object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTTabView();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve a reference pointer to the CTabCtrl
	//     object associated with this view.
	// Returns:
	//     A CTabCtrl reference to the object associated with this view.
	//-----------------------------------------------------------------------
	virtual CTabCtrl& GetTabCtrl () const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to assign an image list to the tab control associated
	//     with this view.
	// Parameters:
	//     pImageList - Pointer to the image list to be assigned to the tab control.
	// Returns:
	//     A pointer to the previous image list or NULL, if there is no previous image list.
	//-----------------------------------------------------------------------
	virtual CImageList* SetTabImageList(CImageList *pImageList);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the handle of the tooltip control associated
	//     with the tab control. The tab control creates a tooltip control if
	//     it has the TCS_TOOLTIPS style. You can also assign a tooltip control
	//     to a tab control by using the SetToolTips member function.
	// Returns:
	//     The handle of the tooltip control if successful, otherwise returns NULL.
	//-----------------------------------------------------------------------
	virtual CToolTipCtrl* GetToolTips();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to assign a tooltip control to the tab control.
	//     You can associate the tooltip control with a tab control by making
	//     a call to GetToolTips.
	// Parameters:
	//     pWndTip - Pointer to a tooltip control.
	//-----------------------------------------------------------------------
	virtual void SetToolTips(CToolTipCtrl* pWndTip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable tooltip usage.
	// Parameters:
	//     bEnable - TRUE to enable tooltip usage.
	// Returns:
	//     TRUE if the tooltip control was found and updated, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL EnableToolTips(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to update the document name with the tab
	//     label.
	//-----------------------------------------------------------------------
	void UpdateDocTitle();

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	//{{AFX_VIRTUAL(CXTTabView)
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTTabView)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	afx_msg LRESULT OnInitialize(WPARAM wp, LPARAM lp) { return OnInitializeImpl(wp, lp); };
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CTabCtrl& CXTTabView::GetTabCtrl() const {
	ASSERT_VALID(this); return (CTabCtrl&)*this;
}
AFX_INLINE CImageList* CXTTabView::SetTabImageList(CImageList *pImageList) {
	ASSERT_VALID(this); return GetTabCtrl().SetImageList(pImageList);
}
AFX_INLINE CToolTipCtrl* CXTTabView::GetToolTips() {
	return GetTips();
}
AFX_INLINE void CXTTabView::SetToolTips(CToolTipCtrl* pWndTip) {
	SetTips(pWndTip);
}

#endif // !defined(__XTTABVIEW_H__)
