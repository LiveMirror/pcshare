// XTShellListCtrlView.h : header file
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
#if !defined(__XTSHELLLISTCTRLVIEW_H__)
#define __XTSHELLLISTCTRLVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

DECLATE_SHELLLIST_BASE(CXTShellListViewBase, CXTListViewHelper, CXTShellListBase)
DECLATE_SHELLLIST_BASE(CXTShellListCtrlBase, CXTListCtrlHelper, CXTShellListBase)

//===========================================================================
// Summary:
//     CXTShellListView is a multiple inheritance class derived from CXTListView
//     and CXTShellPidl. It is used to create a CXTShellListView class object.
//===========================================================================
class _XT_EXT_CLASS CXTShellListView : public CXTShellListViewBase
{
	DECLARE_DYNCREATE(CXTShellListView)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTShellListView object
	//-----------------------------------------------------------------------
	CXTShellListView();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTShellListView object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTShellListView();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Occurs when the user double clicks on the list control.
	// Parameters:
	//     pNMHDR  - Address of an NMHDR structure that contains the notification
	//               code and additional information. For some notification messages,
	//               this parameter points to a larger structure that has the NMHDR
	//               structure as its first member.
	//     pResult - Pointer to an LRESULT variable in which to store the result
	//               code if the message is handled.
	//-----------------------------------------------------------------------
	virtual void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTShellListView)
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTShellListView)
	afx_msg LRESULT OnUpdateShell(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

};

//===========================================================================
// Summary:
//     CXTShellListCtrl is a multiple inheritance class derived from CXTListCtrl
//     and CXTShellPidl. It is used to create a CXTShellListCtrl window.
//===========================================================================
class _XT_EXT_CLASS CXTShellListCtrl : public CXTShellListCtrlBase
{

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTShellListCtrl object
	//-----------------------------------------------------------------------
	CXTShellListCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTShellListCtrl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTShellListCtrl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to associate the tree control with the list.
	// Parameters:
	//     pWnd - Points to the tree control that is associated with the list.
	//-----------------------------------------------------------------------
	virtual void AssociateTree(CWnd* pWnd);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTShellListCtrl)
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnUpdateShell(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE


protected:
	CWnd*               m_pTreeCtrl;    // Window that receives update notification, usually a CXTShellTreeCtrl.
	bool                m_bInitControl; // true if the control has not been initialized.

};

//{{AFX_CODEJOCK_PRIVATE
#define CXTShellList CXTShellListCtrl
//}}AFX_CODEJOCK_PRIVATE

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTShellListCtrl::AssociateTree(CWnd* pWnd) {
	ASSERT_VALID(pWnd); m_pTreeCtrl = pWnd;
}

//===========================================================================
// Summary:
//     CXTShellListCtrlEx is a CXTShellListCtrl derived class. It is used to create
//     a stand-alone shell list control that is not dependent on a CXTShellTreeCtrl
//     for initialization. It is used to create a CXTShellListCtrlEx window for
//     displaying the contents of file folders.
//===========================================================================
class _XT_EXT_CLASS CXTShellListCtrlEx : public CXTShellListCtrl
{

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTShellListCtrlEx object
	//-----------------------------------------------------------------------
	CXTShellListCtrlEx();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTShellListCtrlEx object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTShellListCtrlEx();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to browse to a particular file folder.
	// Parameters:
	//     lpszPath - A NULL terminated string that represents the folder to select.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	bool BrowseToFolder(LPCTSTR lpszPath);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will associate a CWnd object with the shell list control. This
	//     window is usually a CEdit control, but can be any CWnd object. This window will have
	//     its window text updated whenever the selected folder is changed.
	// Parameters:
	//     pSyncWnd - Points to a valid CWnd object.
	//-----------------------------------------------------------------------
	void SetSyncWnd(CWnd* pSyncWnd);


protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTShellListCtrlEx)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTShellListCtrlEx)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE


protected:

	CWnd*   m_pSyncWnd;    // Points to the window to synchronize with the shell list control.
	CString m_strItemPath; // A NULL terminated string that represents the currently selected folder.
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTShellListCtrlEx::SetSyncWnd(CWnd* pSyncWnd) {
	m_pSyncWnd = pSyncWnd;
}

#endif // !defined(__XTSHELLLISTCTRLVIEW_H__)
