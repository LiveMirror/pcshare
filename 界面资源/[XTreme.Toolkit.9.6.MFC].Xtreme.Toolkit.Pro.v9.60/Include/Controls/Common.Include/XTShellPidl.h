// XTShellPidl.h : header file
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
#ifndef __XTSHELLPIDL_H__
#define __XTSHELLPIDL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTShellPidl is a stand alone base class. This class is used by the
//     shell tree and list controls to handle PIDL creation and management.
//===========================================================================
class _XT_EXT_CLASS CXTShellPidl
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTShellPidl object
	//-----------------------------------------------------------------------
	CXTShellPidl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTShellPidl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTShellPidl();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the fully qualified PIDL for the path string.
	// Parameters:
	//     path - File system path string.
	// Returns:
	//     A pointer to an item ID list. Returns NULL if it fails.
	//-----------------------------------------------------------------------
	LPITEMIDLIST IDLFromPath(CString path);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function performs the OneUp or back function.
	// Parameters:
	//     pidlPath - Fully qualified PIDL.
	// Returns:
	//     A fully qualified parent PIDL.
	//-----------------------------------------------------------------------
	LPITEMIDLIST OneUpPIDL(LPITEMIDLIST pidlPath);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the parent folder using PIDLs.
	// Parameters:
	//     path - Path string.
	// Returns:
	//     A path string to the parent.
	//-----------------------------------------------------------------------
	CString OneUpPATH(CString path);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function walks an ITEMIDLIST and points to the last one.
	// Parameters:
	//     pidl - PIDL list.
	// Returns:
	//     A pointer to the last IDL in the list.
	//-----------------------------------------------------------------------
	LPITEMIDLIST GetLastITEM(LPITEMIDLIST pidl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function copies a whole ITEMIDLIST. Remember to Free()
	//     the old one if it is no longer needed.
	// Parameters:
	//     pidl - Pointer to an ITEMIDLIST.
	// Returns:
	//     A new pointer to a copy of the PIDL.
	//-----------------------------------------------------------------------
	LPITEMIDLIST CopyIDList(LPITEMIDLIST pidl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function concatenates two PIDLs.
	// Parameters:
	//     pidl1 - Pointer to an item ID list.
	//     pidl2 - Pointer to an item ID list.
	// Returns:
	//     A pointer to an item ID list.
	//-----------------------------------------------------------------------
	LPITEMIDLIST ConcatPidls(LPCITEMIDLIST pidl1,LPCITEMIDLIST pidl2);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the fully qualified PIDLs for the specified
	//     folder.
	// Parameters:
	//     lpsf - Pointer to the parent shell folder.
	//     lpi  - Pointer to the item ID that is relative to 'lpsf'.
	// Returns:
	//     A pointer to an item ID list.
	//-----------------------------------------------------------------------
	LPITEMIDLIST GetFullyQualPidl(LPSHELLFOLDER lpsf,LPITEMIDLIST lpi);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function copies the ITEMID.
	// Parameters:
	//     lpMalloc - Points to the shell’s IMalloc interface.
	//     lpi      - Pointer to item ID that is to be copied.
	// Returns:
	//     A pointer to an item ID list.
	//-----------------------------------------------------------------------
	LPITEMIDLIST DuplicateItem(LPMALLOC lpMalloc,LPITEMIDLIST lpi);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the friendly name for the folder or file.
	// Parameters:
	//     lpsf           - Pointer to the parent shell folder.
	//     lpi            - Pointer to the item ID that is relative to 'lpsf'.
	//     dwFlags        - Flags to determine which value to return. See SHGNO for more details.
	//     lpFriendlyName - Buffer to receive the friendly name of the folder.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL GetName(LPSHELLFOLDER lpsf,LPITEMIDLIST  lpi,DWORD dwFlags,TCHAR* lpFriendlyName);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function allocates a PIDL.
	// Parameters:
	//     cbSize - Initial size of the PIDL.
	// Returns:
	//     A pointer to an item ID list.
	//-----------------------------------------------------------------------
	LPITEMIDLIST CreatePidl(UINT cbSize);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function computes the number of item IDs in an item ID
	//     list.
	// Parameters:
	//     pidl - Pointer to an item ID list.
	// Returns:
	//     The number of item IDs in the list.
	//-----------------------------------------------------------------------
	UINT GetPidlItemCount(LPCITEMIDLIST pidl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the next PIDL in the list.
	// Parameters:
	//     pidl - Pointer to an item ID list.
	// Returns:
	//     A pointer to the next PIDL item in the list.
	//-----------------------------------------------------------------------
	LPITEMIDLIST GetNextPidlItem(LPCITEMIDLIST pidl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function displays a popup context menu if given a parent
	//     shell folder, relative item ID, and screen location.
	// Parameters:
	//     hwnd       - Context menu owner.
	//     lpsfParent - Pointer to the parent shell folder.
	//     lpi        - Pointer to the item ID that is relative to 'lpsfParent'.
	//     nCount     - Number of PIDLs.
	//     lppt       - Screen location of where to popup the menu.
	// Returns:
	//     TRUE on success, or FALSE on failure.
	//-----------------------------------------------------------------------
	BOOL ShowContextMenu(HWND hwnd,LPSHELLFOLDER lpsfParent,LPCITEMIDLIST *lpi,int nCount,LPPOINT lppt);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function displays a popup context menu if given a parent
	//     shell folder, relative item ID, and screen location.
	// Parameters:
	//     hwnd       - Context menu owner.
	//     lpsfParent - Pointer to the parent shell folder.
	//     lpi        - Pointer to the item ID that is relative to 'lpsfParent'.
	//     lppt       - Screen location of where to popup the menu.
	// Returns:
	//     TRUE on success, or FALSE on failure.
	//-----------------------------------------------------------------------
	BOOL ShowContextMenu(HWND hwnd,LPSHELLFOLDER lpsfParent,LPITEMIDLIST lpi,LPPOINT lppt);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the index for the current icon. Index is
	//     the index into the system image list.
	// Parameters:
	//     lpi    - Fully qualified item ID list for the current item.
	//     uFlags - Flags for SHGetFileInfo().
	// Returns:
	//     An icon index for the current item.
	//-----------------------------------------------------------------------
	int GetItemIcon(LPITEMIDLIST lpi,UINT uFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the IContextMenu, IContextMenu2 or IContextMenu3
	//     interface.
	// Parameters:
	//     psfFolder - A pointer to a valid IShellFolder data type.
	//     localPidl - A pointer to a valid _ITEMIDLIST structure.
	//     nCount    - Number of items in the context menu.
	//     ppCM      - Long pointer to a CONTEXTMENU struct.
	//     pcmType   - A pointer to a valid int data type that represents the version number
	//     of the context menu.
	// Returns:
	//     An HRESULT value.
	//-----------------------------------------------------------------------
	HRESULT GetSHContextMenu(LPSHELLFOLDER psfFolder,LPCITEMIDLIST *localPidl,int nCount,void** ppCM,int* pcmType);

public:
	// ---------------------------------------------------------------------------
	// Summary:
	//     CXTShellTreeCtrl callback sort function.
	// Parameters:
	//     lparam1 -     Corresponds to the lParam member of the TV_ITEM structure
	//                   for the two items being compared.
	//     lparam2 -     Corresponds to the lParam member of the TV_ITEM structure
	//                   for the two items being compared.
	//     lparamSort -  The 'lParamSort' member corresponds to the lParam member of
	//                   TV_SORTCB.
	// Remarks:
	//     This member function is a callback function used by
	//     CXTShellTreeCtrl and CXTShellTreeView, and is called during a sort
	//     operation each time the relative order of two list items needs to
	//     be compared.
	// Returns:
	//     A negative value if the first item should precede the second, a
	//     positive value if the first item should follow the second, or zero
	//     if the two items are equivalent.
	// ---------------------------------------------------------------------------
	static int CALLBACK TreeViewCompareProc(LPARAM lparam1,LPARAM lparam2,LPARAM lparamSort);

	// ---------------------------------------------------------------------------
	// Summary:
	//     CXTShellListCtrl callback sort function.
	// Parameters:
	//     lparam1 -     Corresponds to the lParam member of the LV_ITEM structure
	//                   for the two items being compared.
	//     lparam2 -     Corresponds to the lParam member of the LV_ITEM structure
	//                   for the two items being compared.
	//     lparamSort -  The 'lParamSort' member corresponds to the lParam member of
	//                   LV_SORTCB.
	// Remarks:
	//     This member function is a callback function used by
	//     CXTShellListCtrl and CXTShellListView, and is called during a sort
	//     operation each time the relative order of two list items needs to
	//     be compared.
	// Returns:
	//     A negative value if the first item should precede the second, a
	//     positive value if the first item should follow the second, or zero
	//     if the two items are equivalent.
	// ---------------------------------------------------------------------------
	static int CALLBACK ListViewCompareProc(LPARAM lparam1,LPARAM lparam2,LPARAM lparamSort);


protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when item of context menu was executed
	// Parameters:
	//     idCmd - Item identifier was executed.
	//     cmi   - CMINVOKECOMMANDINFO structure reference contained additional information about item was executed.
	//-----------------------------------------------------------------------
	virtual void OnShowContextMenu(int idCmd, CMINVOKECOMMANDINFO& cmi);

private:
	static LRESULT CALLBACK HookWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

private:
	static WNDPROC          m_pOldWndProc; // regular window proc
	static LPCONTEXTMENU2   m_pIContext2;  // active shell context menu

};

#endif // __XTSHELLPIDL_H__
