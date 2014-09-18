// XTShellListBase.h : header file
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
#ifndef __XTSHELLLISTBASE_H__
#define __XTSHELLLISTBASE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTDirWatcher;

//===========================================================================
// Summary:
//     CXTShellListBase is a multiple inheritance class derived from CXTListView
//     and CXTShellPidl. It is used to create a CXTShellListBase class object.
//===========================================================================
class _XT_EXT_CLASS CXTShellListBase : public CXTListBase, public CXTShellPidl
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTShellListBase object
	//-----------------------------------------------------------------------
	CXTShellListBase();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTShellListBase object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTShellListBase();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable the display of the
	//     shell context menu on the right click of the item.
	// Parameters:
	//     bEnable - TRUE to display a context menu.
	//-----------------------------------------------------------------------
	virtual void EnableContextMenu(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine the type of items included
	//     in the shell enumeration. The default is SHCONTF_FOLDERS | SHCONTF_NONFOLDERS.
	// Parameters:
	//     uFlags - Determines the type of items included in an enumeration and can
	//              be one or more of the flags listed in the Remarks section.
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
	virtual void SetEnumFlags(UINT uFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function associates the system image list with the list
	//     control.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL InitSystemImageLists();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function populates the list view control.
	// Parameters:
	//     lptvid - Pointer to TreeView item data.
	//     lpsf   - Pointer to the parent shell folder.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL PopulateListView(XT_TVITEMDATA* lptvid,LPSHELLFOLDER lpsf);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns the index of the list view item that
	//     was double clicked on.
	// Returns:
	//     The index of the item that was double clicked; or -1, if the item
	//     was not found.
	//-----------------------------------------------------------------------
	virtual int GetDoubleClickedItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function displays the system popup menu for the selected
	//     item or folder.
	// Returns:
	//     The index of the item selected; or -1, if the item was not found.
	//-----------------------------------------------------------------------
	virtual int GetContextMenu();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function executes the item clicked on
	//     in the list control via the Shell.
	// Parameters:
	//     iItem - Index of the list view item clicked on.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	virtual bool ShellOpenItem(int iItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function executes the item clicked on
	//     in the list control via the Shell.
	// Parameters:
	//     lplvid - Pointer to the list view item data.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	virtual bool ShellOpenItem(XT_LVITEMDATA* lplvid);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the path for the specified list view
	//     item.
	// Parameters:
	//     iItem       - Index of the list view item to get the path of.
	//     strItemPath - Reference to a CString object that receives the path string.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL GetItemPath(int iItem,CString &strItemPath);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function creates default columns for the list view
	//-----------------------------------------------------------------------
	virtual void BuildDefaultColumns();

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this member function in your derived class to perform custom
	//     sort routines.
	// Parameters:
	//     nCol        - Passed in from the control. The index of the column clicked.
	//     bAscending  - Passed in from the control. true if the sort order should be ascending.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	virtual bool SortList(int nCol,bool bAscending );

	//-----------------------------------------------------------------------
	// Summary:
	//     Used by the shell list control to handle a drag drop event.
	// Parameters:
	//     pNMListView - Points to a NM_LISTVIEW structure.
	//-----------------------------------------------------------------------
	virtual void OnDragDrop(NM_LISTVIEW* pNMListView);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the shell list control to insert commas into the specified
	//     numeric value. The result is then returned in the szBufferOut item. An
	//     example of this would be to format a file size 1,024 KB.
	// Parameters:
	//     value    - [in] Numeric value to insert commas.
	//     szBuffer - [out] String buffer to receive the formatted string.
	//     nSize    - [in] Size of the string specified by szBuffer.
	// Returns:
	//     A string representing a comma separated numeric value.
	//-----------------------------------------------------------------------
	virtual TCHAR* InsertCommas(LONGLONG value, TCHAR* szBuffer, UINT nSize);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member function to initialize the control. This member
	//     should be called after the control has been created or subclassed.
	//-------------------------------------------------------------------------
	virtual void InitializeControl();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function adds items to the list view.
	// Parameters:
	//     lptvid - Pointer to tree view item data.
	//     lpsf   - Pointer to the parent shell folder.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL InitListViewItems(XT_TVITEMDATA* lptvid,LPSHELLFOLDER lpsf);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the index for the normal and selected
	//     icons of the current item.
	// Parameters:
	//     lpifq    - Fully qualified item ID list for the current item.
	//     lptvitem - Pointer to the tree view item that is about to be added to the tree.
	//-----------------------------------------------------------------------
	virtual void GetNormalAndSelectedIcons(LPITEMIDLIST lpifq,LPTV_ITEM lptvitem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the shell attribute flags for the specified
	//     list item.
	// Parameters:
	//     iItem        - Index of the item to set the attributes for.
	//     dwAttributes - Flags retrieved from SHELLFOLDER::GetAttributesOf.
	//-----------------------------------------------------------------------
	virtual void SetAttributes(int iItem,DWORD dwAttributes);

protected:
//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_VIRTUAL(CXTShellListBase)
	virtual void UpdateList(int nMessage, XT_TVITEMDATA* pItemData);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTShellListBase)
	afx_msg void OnRclickImpl(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	UINT             m_uFlags;           // Flags indicating which items to include in the enumeration.
	BOOL             m_bUninitOLE;       // TRUE if OleUninitialize has to be called.
	BOOL             m_bContextMenu;     // TRUE to display the shell context menu on right item click.
	LPITEMIDLIST     m_pidlINet;         // Points to the CSIDL_INTERNET folder location.
	CXTShellSettings m_shSettings;       // Contains SHELLFLAGSTATE info.
	CXTDirWatcher*   m_pDirThread;       // Thread used to monitor directory activity.
	int              m_nNameColumnWidth; // Size in pixels for the name column width.

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTShellListBase::SetEnumFlags(UINT uFlags) {
	m_uFlags = uFlags;
}
AFX_INLINE void CXTShellListBase::EnableContextMenu(BOOL bEnable) {
	m_bContextMenu = bEnable;
}

//{{AFX_CODEJOCK_PRIVATE
#define DECLATE_SHELLLIST_BASE(ClassName, List, Base)\
DECLATE_LIST_BASE(Base##List, List, Base)\
class _XT_EXT_CLASS ClassName : public Base##List\
{\
protected:  \
	void OnRclick(NMHDR* pNMHDR, LRESULT* pResult) {\
		OnRclickImpl(pNMHDR, pResult);\
	}\
	void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) {\
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;\
		OnDragDrop(pNMListView);    \
		*pResult = 0;\
	}   \
};

#define ON_SHELLLIST_REFLECT\
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)\
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)\
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBeginDrag)\
	ON_NOTIFY_REFLECT(LVN_BEGINRDRAG, OnBeginDrag)\
	ON_LISTCTRL_REFLECT

#endif // __XTSHELLLISTBASE_H__
//}}AFX_CODEJOCK_PRIVATE
