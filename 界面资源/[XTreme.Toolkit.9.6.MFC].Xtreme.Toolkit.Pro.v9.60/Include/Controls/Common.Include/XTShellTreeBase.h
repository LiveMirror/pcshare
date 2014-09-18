// XTShellTreeBase.h : header file
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
#ifndef __XTSHELLTREEBASE_H__
#define __XTSHELLTREEBASE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// -------------------------------------------------------------------
// Summary:
//     Enumeration used to determine folder or drive type.
// Remarks:
//     XTFindType type defines the constants used by the CXTShellTreeBase
//     class to determine the type of object enumerated either folder or drive.
// See Also:
//    CXTFontListBox, CXTFontListBox::SetListStyle
//
// <KEYWORDS xtFindTypeDrive, xtFindTypeFolder>
// -------------------------------------------------------------------
enum XTFindType
{
	xtFindTypeDrive,  // The object type is a drive.
	xtFindTypeFolder  // The object type is a folder.
};

//===========================================================================
// Summary:
//     CXTShellTreeBase is a multiple inheritance class derived from CXTTreeView
//     and CXTShellPidl. It is used to create a CXTShellTreeBase class object.
//===========================================================================
class _XT_EXT_CLASS CXTShellTreeBase : public CXTTreeBase, public CXTShellPidl
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTShellTreeBase object
	//-----------------------------------------------------------------------
	CXTShellTreeBase();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTShellTreeBase object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTShellTreeBase();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function fills a branch of the TreeView control. Given
	//     the shell folder, it enumerates the subitems of this folder and adds
	//     the appropriate items to the tree.
	// Parameters:
	//     lpsf    - Pointer to the parent shell folder.
	//     lpifq   - Fully qualified item ID list to the item having items enumerated.
	//               This is the PIDL to the item identified by the 'lpsf' parameter.
	//     hParent - Parent tree node.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL InitTreeViewItems(LPSHELLFOLDER lpsf,LPITEMIDLIST lpifq,HTREEITEM hParent);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the index for the normal and selected
	//     icons for the current item.
	// Parameters:
	//     lpifq    - Fully qualified item ID list for the current item.
	//     lptvitem - Pointer to the tree view item being added to the tree.
	//-----------------------------------------------------------------------
	virtual void GetNormalAndSelectedIcons(LPITEMIDLIST lpifq,LPTV_ITEM lptvitem);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable the display of the
	//     shell context menu on right item click.
	// Parameters:
	//     bEnable - TRUE to display a context menu.
	//-----------------------------------------------------------------------
	void EnableContextMenu(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine the type of items included
	//     in the shell enumeration. The default is SHCONTF_FOLDERS | SHCONTF_NONFOLDERS.
	// Parameters:
	//     uFlags - Determines the type of items included in an enumeration. It
	//              can be one or more of the values listed in the Remarks section.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:<p/>
	//     * <b>SHCONTF_FOLDERS</b> Include items that are folders in
	//       the enumeration.
	//     * <b>SHCONTF_NONFOLDERS</b> Include items that are not folders
	//       in the enumeration.
	//     * <b>SHCONTF_INCLUDEHIDDEN</b> Include hidden items in the
	//       enumeration.
	//     * <b>SHCONTF_INIT_ON_FIRST_NEXT</b> IShellFolder::EnumObjects
	//       can return without validating the enumeration object. Validation
	//       can be postponed until the first call to IEnumIDList::Next.
	//       This flag is intended to be used when a user interface may be
	//       displayed prior to the first IEnumIDList::Next call. For a
	//       user interface to be presented, 'hwndOwner' must be set to a valid
	//       window handle.
	//     * <b>SHCONTF_NETPRINTERSRCH</b> The caller is looking for
	//       printer objects.
	//-----------------------------------------------------------------------
	void SetEnumFlags(UINT uFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function obtains a handle to the system image list and
	//     attaches it to the tree control.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL InitSystemImageLists();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function adds items to the tree view.
	//-----------------------------------------------------------------------
	virtual void PopulateTreeView();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function responds to a TVN_ITEMEXPANDING message in
	//     order to fill up subdirectories.
	// Parameters:
	//     pNMTreeView - Address of an NM_TREEVIEW struct.
	//-----------------------------------------------------------------------
	virtual void OnFolderExpanding(NM_TREEVIEW* pNMTreeView);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function displays the system popup menu for the selected
	//     item or folder.
	// Returns:
	//     A handle to the currently selected HTREEITEM.
	//-----------------------------------------------------------------------
	virtual HTREEITEM GetContextMenu();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function responds to a TVN_SELCHANGED message to retrieve
	//     the specified folder path.
	// Parameters:
	//     pNMTreeView   - Address of an NM_TREEVIEW struct.
	//     strFolderPath - Address of a CString object to receive the file system path.
	// Returns:
	//     TRUE if the folder path was found, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnFolderSelected(NM_TREEVIEW* pNMTreeView,CString &strFolderPath);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the path of the currently selected
	//     tree item.
	// Parameters:
	//     strFolderPath - Address of a CString object to receive the file system path.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL GetSelectedFolderPath(CString &strFolderPath);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function recursively searches the tree control beginning
	//     at 'hItem' to find the item specified by 'lplvid'. This is typically used when
	//     the user double clicks an item in the list view.
	// Parameters:
	//     hItem  - Handle to the tree item to search from.
	//     lplvid - Pointer to the list view item data.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL FindTreeItem(HTREEITEM hItem,XT_LVITEMDATA* lplvid);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to associate a CComboBox object with
	//     the control. Whenever the path changes, the combo is updated.
	// Parameters:
	//     pWnd - Points to the combo box that is associated with the tree.
	//-----------------------------------------------------------------------
	virtual void AssociateCombo(CWnd* pWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to initialize a branch of the shell
	//     tree.
	// Parameters:
	//     hItem  - Handle to a tree node.
	//     lptvid - Pointer to the list view item data.
	//-----------------------------------------------------------------------
	virtual void InitTreeNode(HTREEITEM hItem,XT_TVITEMDATA* lptvid);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function searches the tree for a specified folder.
	// Parameters:
	//     hItem         - Tree item to begin the search from.
	//     strSearchName - String to search for.
	//     attr          - Looking for folder or drive.
	//     bFindRoot     - TRUE to look for the drive letter only.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SearchTree(HTREEITEM hItem,CString strSearchName,XTFindType attr,BOOL bFindRoot=FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will "tunnel" the tree to find the specified
	//     path. This will work only when the PopulateTreeView() method is used
	//     to populate the tree.
	// Parameters:
	//     strFindPath - Path to find.
	//-----------------------------------------------------------------------
	virtual void TunnelTree(CString strFindPath);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the path of a folder item, which
	//     does not have to be selected. Pass a CString object that will hold
	//     the folder path.
	// Parameters:
	//     hItem         - Tree item to get the path for.
	//     strFolderPath - Reference to a CString object to contain the folder path.
	// Returns:
	//     TRUE if the path is not in the file system (e.g.
	//     MyComputer); if none is selected, it returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL GetFolderItemPath(HTREEITEM hItem,CString &strFolderPath);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function populates a tree based upon a path. You must
	//     call InitializeTree() before populating the tree for the first time.
	// Parameters:
	//     lpszPath - Path to populate.
	// See Also: InitializeTree()
	//-----------------------------------------------------------------------
	virtual void PopulateTree(LPCTSTR lpszPath);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to initialize the shell tree control prior
	//     to calling PopulateTree.
	// Parameters:
	//     dwStyle - TVS_ style flags for tree control.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	// See Also: PopulateTree()
	//-----------------------------------------------------------------------
	virtual BOOL InitializeTree(DWORD dwStyle=TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT);


	//-----------------------------------------------------------------------
	// Summary:
	//     Used by the shell list control to handle a drag drop event.
	// Parameters:
	//     pNMTreeView - Points to a NM_TREEVIEW structure.
	//-----------------------------------------------------------------------
	virtual void OnDragDrop(NM_TREEVIEW* pNMTreeView);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to insert the Desktop as the root
	//     item in the shell tree control.
	// Returns:
	//     A handle to the desktop tree item if successful, otherwise NULL.
	//-----------------------------------------------------------------------
	virtual HTREEITEM InsertDesktopItem();

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

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the shell attribute flags for the specified
	//     tree item.
	// Parameters:
	//     hItem        - Handle to HTREEITEM node.
	//     dwAttributes - Flags retrieved from SHELLFOLDER::GetAttributesOf.
	//-----------------------------------------------------------------------
	void SetAttributes(HTREEITEM hItem,DWORD dwAttributes);

protected:
//{{AFX_CODEJOCK_PRIVATE

	//{{AFX_MSG(CXTShellTreeBase)
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:

	UINT             m_uFlags;       // Flags indicating which items to include in the enumeration.
	bool             m_bTunneling;   // true if tree is currently traversing.
	BOOL             m_bContextMenu; // TRUE to display the shell context menu on right item click.
	CWnd*            m_pComboBox;    // CComboBox that is associated with this control. See AssociateCombo(...)
	CXTShellSettings m_shSettings;   // Contains SHELLFLAGSTATE info.

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTShellTreeBase::SetEnumFlags(UINT uFlags) {
	m_uFlags = uFlags;
}
AFX_INLINE void CXTShellTreeBase::EnableContextMenu(BOOL bEnable) {
	m_bContextMenu = bEnable;
}

// --------------------------------------------------------------------------------------
// Summary:
//     The DELCLATE_SHELLTREE_BASE class is a template class used by the
//     CXTShellTreeView and CXTShellTreeCtrl class to declare a multiple
//     inheritance base class so that both classes could share a common
//     code base.
// Parameters:
//     ClassName -  Name of the object to be declared as the base class.
//     Tree -       Existing class that this object will derive from.
//     Base -       Existing shared class that this object will derive from.
// Remarks:
//     <i>Tree</i> must be an existing class that is derived from either <i>CTreeCtrl</i>
//     or <i>CTreeView</i>.
// Example:
// <code>
// DECLATE_SHELLTREE_BASE(CXTShellTreeViewBase, CTreeView, CXTShellTreeBase)
//
// class _XT_EXT_CLASS CXTShellTreeView : public CXTShellTreeViewBase
// {
// public:
//     CXTShellTreeView();
// };
// </code>
// --------------------------------------------------------------------------------------
#define DECLATE_SHELLTREE_BASE(ClassName, Tree, Base)
//{{AFX_CODEJOCK_PRIVATE
#undef DECLATE_SHELLTREE_BASE
#define DECLATE_SHELLTREE_BASE(ClassName, Tree, Base)\
DECLATE_TREE_BASE(Base##Tree, Tree, Base)\
class _XT_EXT_CLASS ClassName : public Base##Tree\
{\
protected:\
	void OnPaint() {\
		Base::OnPaint();\
	}   \
	void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) {\
		NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;\
		OnDragDrop(pNMTreeView);\
		*pResult = 0;\
	}\
	void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) {\
		Base::OnSelchanged(pNMHDR, pResult);\
	}   \
	void OnRclick(NMHDR* pNMHDR, LRESULT* pResult) {\
		Base::OnRclick(pNMHDR, pResult);\
	}   \
	void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) {\
		Base::OnItemexpanding(pNMHDR, pResult);\
	}\
};

#define ON_SHELLTREE_REFLECT\
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)\
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)\
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)\
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBeginDrag)\
	ON_NOTIFY_REFLECT(TVN_BEGINRDRAG, OnBeginDrag)\
	ON_TREECTRL_REFLECT
//}}AFX_CODEJOCK_PRIVATE

#endif // __XTSHELLTREEVIEW_H__
