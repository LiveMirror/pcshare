// XTShellTreeCtrl.h : header file
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
#if !defined(__XTSHELLTREECTRL_H__)
#define __XTSHELLTREECTRL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

DECLATE_SHELLTREE_BASE(CXTShellTreeViewBase, CTreeView, CXTShellTreeBase)
DECLATE_SHELLTREE_BASE(CXTShellTreeCtrlBase, CTreeCtrl, CXTShellTreeBase)

//===========================================================================
// Summary:
//     CXTShellTreeView is a multiple inheritance class derived from CXTTreeView
//     and CXTShellPidl. It is used to create a CXTShellTreeView class object.
//===========================================================================
class _XT_EXT_CLASS CXTShellTreeView : public CXTShellTreeViewBase
{
	DECLARE_DYNCREATE(CXTShellTreeView)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTShellTreeView object
	//-----------------------------------------------------------------------
	CXTShellTreeView();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTShellTreeView object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTShellTreeView();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Called after a user makes a new tree selection.
	// Remarks:
	//     This member function is called whenever a user makes a selection
	//     within the tree control to synchronize any shell list or shell
	//     combo boxes associated with the tree. You can override this member
	//     to provide additional functionality.
	// Parameters:
	//     hItem         - Handle to the newly selected tree item.
	//     strFolderPath - NULL terminated string representing the fully
	//                     qualified path to the selected tree item.
	//-----------------------------------------------------------------------
	virtual void SelectionChanged(HTREEITEM hItem, CString strFolderPath);


//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTShellTreeView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL
//}}AFX_CODEJOCK_PRIVATE

protected:
};

//===========================================================================
// Summary:
//     CXTShellTreeCtrl is a multiple inheritance class derived from CXTTreeCtrl
//     and CXTShellPidl. CXTShellTreeCtrl is used to create a tree control
//     that displays an explorer style tree, and can be associated with a combo box
//     and list control.
//===========================================================================
class _XT_EXT_CLASS CXTShellTreeCtrl : public CXTShellTreeCtrlBase
{
	DECLARE_DYNAMIC(CXTShellTreeCtrl)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTShellTreeCtrl object
	//-----------------------------------------------------------------------
	CXTShellTreeCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTShellTreeCtrl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTShellTreeCtrl();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable auto-initialization
	//     of the shell tree control.
	// Parameters:
	//     bEnable - TRUE if the tree initializes upon creation.
	//-----------------------------------------------------------------------
	void EnableAutoInit(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to associate the list control with the tree.
	// Parameters:
	//     pWnd - Points to the list control that is associated with the tree.
	//-----------------------------------------------------------------------
	virtual void AssociateList(CWnd* pWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will "tunnel" the tree to find the specified
	//     path. This will work only when the PopulateTreeView() method is used
	//     to populate the tree.
	// Parameters:
	//     strFindPath - Path to find.
	//-----------------------------------------------------------------------
	virtual void TunnelTree(CString strFindPath);

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Called by CXTShellTreeCtrl to delay content initialization.
	// Remarks:
	//     Called by the shell tree control to delay content initialization by
	//     placing the XTWM_INITIAL_UPDATE message in the calling threads
	//     message queue to ensure that all windows have been properly initialized
	//     first. You can override this member to provide additional functionality.
	//-------------------------------------------------------------------------
	virtual void DelayContentInit();

	//-------------------------------------------------------------------------
	// Summary:
	//     Called by CXTShellTreeCtrl control to resume initialization.
	// Remarks:
	//     Called by the shell tree control to resume content initialization after
	//     it has been delayed. Content initialization will initialize and
	//     populate the tree control with data. You can override this member to
	//     provide additional functionality.
	//-------------------------------------------------------------------------
	virtual void ProcessContentInit();

	//-----------------------------------------------------------------------
	// Summary:
	//     Called after a user makes a new tree selection.
	// Remarks:
	//     This member function is called whenever a user makes a selection
	//     within the tree control to synchronize any shell list or shell
	//     combo boxes associated with the tree. You can override this member
	//     to provide additional functionality.
	// Parameters:
	//     hItem         - Handle to the newly selected tree item.
	//     strFolderPath - NULL terminated string representing the fully
	//                     qualified path to the selected tree item.
	//-----------------------------------------------------------------------
	virtual void SelectionChanged(HTREEITEM hItem, CString strFolderPath);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTShellTreeCtrl)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTShellTreeCtrl)
	afx_msg LRESULT OnUpdateShell(WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE


protected:

	BOOL             m_bAutoInit;           // TRUE if the tree control is to initialize when created.
	BOOL             m_bContentInitPending; // TRUE if content initialization is pending.
	BOOL             m_bTunnelPath;         // TRUE if tunnel initialization is pending.
	CWnd*            m_pListCtrl;           // Window that receives the update notification, usually a CXTShellListCtrl.
	CString          m_strTunnelPath;       // Current path to tunnel if tree initialization is still pending. Valid if 'm_bTunnelPath' is TRUE.

};

//{{AFX_CODEJOCK_PRIVATE
#define CXTShellTree    CXTShellTreeCtrl
//}}AFX_CODEJOCK_PRIVATE

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTShellTreeCtrl::EnableAutoInit(BOOL bEnable) {
	m_bAutoInit = bEnable;
}
AFX_INLINE void CXTShellTreeCtrl::AssociateList(CWnd* pWnd) {
	ASSERT_VALID(pWnd); m_pListCtrl = pWnd;
}

#endif // !defined(__XTSHELLTREECTRL_H__)
