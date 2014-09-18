// XTTreeBase.h interface for the CXTTreeBase class.
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
#if !defined(__XTTREEBASE_H__)
#define __XTTREEBASE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTTreeBase is a CTreeView derived class. It extends the CTreeView
//     class to add additional functionality, including tree traversal, searching,
//     color, and settings.
//===========================================================================
class _XT_EXT_CLASS CXTTreeBase
{
protected:
	// ----------------------------------------------------------------------
	// Summary:
	//     CLRFONT structure is used to by the CXTShellTreeCtrl and
	//     CXTShellTreeView class to maintain font information for a
	//     particular tree item.
	// See Also:
	//     CXTShellTreeCtrl, CXTShellTreeView
	// ----------------------------------------------------------------------
	struct CLRFONT
	{
		LOGFONT  logfont; // A LOGFONT object that represents the tree item font.
		COLORREF color;   // An RGB value that represents the color for a tree item.
	};

	// ----------------------------------------------------------------------
	// Summary:
	//     Map for maintaining HTREEITEM to CLRFONT structure relationships.
	// Remarks:
	//     CMap definition used by the CXTTreeBase class to maintain
	//     a list of HTREEITEM to CLRFONT structure relationships representing
	//     tree item that has user defined color information.
	// See Also:
	//     CXTTreeBase, CLRFONT
	// ----------------------------------------------------------------------
	typedef CMap<void*, void*, CLRFONT, CLRFONT&> CColorFontMap;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Protected constructor used by dynamic creation. Constructs a CXTTreeBase
	//     object
	//-----------------------------------------------------------------------
	CXTTreeBase();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTTreeBase object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTTreeBase();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will retrieve the tree view item that has the
	//     specified relationship, indicated by the 'nCode' parameter, to 'hItem'.
	// Parameters:
	//     hItem -  Handle of a tree item.
	//     nCode - A flag indicating the type of relation to 'hItem'. This flag can be
	//             one of the values listed in the Remarks section.
	// Remarks:
	//     nCode can be one of the following values:
	//     * <b>TVGN_CARET</b> Retrieves the currently selected item.
	//     * <b>TVGN_CHILD</b> Retrieves the first child item. The 'hItem'
	//       parameter must be NULL.
	//     * <b>TVGN_DROPHILITE</b> Retrieves the item that is the target
	//       of a drag-and-drop operation.
	//     * <b>TVGN_FIRSTVISIBLE</b> Retrieves the first visible item.
	//     * <b>TVGN_NEXT</b> Retrieves the next sibling item.
	//     * <b>TVGN_NEXTVISIBLE</b> Retrieves the next visible item
	//        that follows the specified item.
	//     * <b>TVGN_PARENT</b> Retrieves the parent of the specified
	//       item.
	//     * <b>TVGN_PREVIOUS</b> Retrieves the previous sibling item.
	//     * <b>TVGN_PREVIOUSVISIBLE</b> Retrieves the first visible
	//       item that precedes the specified item.
	//     * <b>TVGN_ROOT</b> Retrieves the first child item of the
	//       root item of which the specified item is a part.
	// Returns:
	//     The handle of the next item if successful, otherwise returns
	//     NULL.
	//-----------------------------------------------------------------------
	virtual HTREEITEM GetNextItem(HTREEITEM hItem,UINT nCode) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the next item as if the outline was completely
	//     expanded.
	// Parameters:
	//     hItem - Handle of the reference item.
	// Returns:
	//     The handle of the item immediately below the reference item.
	//-----------------------------------------------------------------------
	virtual HTREEITEM GetNextItem(HTREEITEM hItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the previous item as if the outline was
	//     completely expanded.
	// Parameters:
	//     hItem - Handle of the reference item.
	// Returns:
	//     The handle of the item immediately above the reference item.
	//-----------------------------------------------------------------------
	virtual HTREEITEM GetPrevItem(HTREEITEM hItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the last item in the branch.
	// Parameters:
	//     hItem - Node identifying the branch. NULL will return the last item in the
	//             outline.
	// Returns:
	//     The handle of the last item.
	//-----------------------------------------------------------------------
	virtual HTREEITEM GetLastItem(HTREEITEM hItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function searches the entire tree for an item label
	//     that contains the search string.
	// Parameters:
	//     lpszSearch     - String to search for.
	//     bCaseSensitive - TRUE if the search should be case sensitive.
	//     bDownDir       - TRUE for down.
	//     bWholeWord     - TRUE if search should match whole words.
	//     hItem          - Handle of the tree item to start searching from, NULL to use
	//     the currently selected tree item.
	// Returns:
	//     The handle to the item, or returns NULL.
	// See Also:
	//     IsFindValid
	//-----------------------------------------------------------------------
	virtual HTREEITEM FindItem(LPCTSTR lpszSearch,BOOL bCaseSensitive=FALSE,BOOL bDownDir=TRUE,BOOL bWholeWord=FALSE,HTREEITEM hItem=NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function searches the entire branch specified by htiItem for an item label
	//     that contains the search string.
	// Parameters:
	//     lpszSearch     - String to search for.
	//     bCaseSensitive - TRUE if the search should be case sensitive.
	//     bWholeWord     - TRUE if search should match whole words.
	//     htiItem        - Handle of the tree item to start searching from, NULL to use
	//                      the currently selected tree item.
	// Returns:
	//     The handle to the item, or returns NULL.
	// See Also:
	//     IsFindValid
	//-----------------------------------------------------------------------
	virtual HTREEITEM FindItemInBranch(LPCTSTR lpszSearch, BOOL bCaseSensitive /*= FALSE*/, BOOL bWholeWord /*= FALSE*/, HTREEITEM htiItem /*= NULL*/);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the font for the reference tree item.
	// Parameters:
	//     hItem   - Handle of the reference item.
	//     logfont - New font for the tree item.
	//-----------------------------------------------------------------------
	virtual void SetItemFont(HTREEITEM hItem,LOGFONT& logfont);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the current LOGFONT font used by
	//     the tree item.
	// Parameters:
	//     hItem    - Handle of the reference item.
	//     plogfont - Pointer to receive LOGFONT information.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL GetItemFont(HTREEITEM hItem,LOGFONT* plogfont);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the reference tree item font to bold.
	// Parameters:
	//     hItem - Handle of the reference item.
	//     bBold - TRUE for bold font.
	//-----------------------------------------------------------------------
	virtual void SetItemBold(HTREEITEM hItem,BOOL bBold=TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the tree item has a bold font.
	// Parameters:
	//     hItem - Handle of the reference item.
	// Returns:
	//     TRUE if the tree item has a bold font, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL GetItemBold(HTREEITEM hItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the tree item text color.
	// Parameters:
	//     hItem - Handle of the reference item.
	//     color - RGB value for the tree item's text.
	//-----------------------------------------------------------------------
	virtual void SetItemColor(HTREEITEM hItem,COLORREF color);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns the RGB value for the referenced
	//     tree item text or (COLORREF)-1, if color was not set.
	// Parameters:
	//     hItem - Handle of the reference item.
	// Returns:
	//     The RGB value for the referenced tree item text, or (COLORREF)-1, if the
	//     color was not set.
	//-----------------------------------------------------------------------
	virtual COLORREF GetItemColor(HTREEITEM hItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function enables or disables multi-selection for the
	//     tree control.
	// Parameters:
	//     bMultiSelect - TRUE for a multi-selection tree control.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL EnableMultiSelect(BOOL bMultiSelect=TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns the previously selected tree item in
	//     a multi-selection tree control.
	// Parameters:
	//     hItem - Handle of the reference item.
	// Returns:
	//     A handle to the previously selected tree item.
	//-----------------------------------------------------------------------
	virtual HTREEITEM GetPrevSelectedItem(HTREEITEM hItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns the next selected item in a multi-selection
	//     tree control.
	// Parameters:
	//     hItem - Handle of the reference item.
	// Returns:
	//     A handle to the next selected tree item.
	//-----------------------------------------------------------------------
	HTREEITEM GetNextSelectedItem(HTREEITEM hItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns the first selected item in a multi-selection
	//     tree control.
	// Returns:
	//     A handle to the first selected tree item.
	//-----------------------------------------------------------------------
	HTREEITEM GetFirstSelectedItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns the number of tree items that are selected.
	// Returns:
	//     A UNIT value that represents the number of tree items selected.
	//-----------------------------------------------------------------------
	UINT GetSelectedCount() const;

	// --------------------------------------------------------------------------
	// Summary:
	//     This member function is a replacement for the base class function
	//     of the same name, to handle TVIS_FOCUSED in a multi-select
	//     tree control. It sets the state of the item specified by <i>hItem</i>.
	// Parameters:
	//     hItem -       Handle of the item whose state is to be set.
	//     nState -      Specifies the new states for the item.
	//     nStateMask -  Specifies which states are to be changed.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	// Remarks:
	//     The TVIS_FOCUSED state flag when set will draw a focus
	//     rectangle around the tree item that currently has input focus.
	//     Only one tree item can have focus at any given time. For a list of
	//     possible item state flags, see Tree-View Control Item States in
	//     the MSDN documentation.
	// --------------------------------------------------------------------------
	BOOL SetItemState(HTREEITEM hItem,UINT nState,UINT nStateMask);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is a replacement for the base class function of
	//     the same name to handle TVIS_FOCUSED in a multi-select tree control.
	// Parameters:
	//     hItem      - Handle of the item whose state is to be retrieved.
	//     nStateMask - Mask indicating which states are to be retrieved. For more information
	//                  on possible values for 'nStateMask', see the discussion of the 'state'
	//                  and 'stateMask' members of the TVITEM structure in the Platform SDK.
	// Returns:
	//     The state of the item specified by 'hItem'.
	//-----------------------------------------------------------------------
	UINT GetItemState(HTREEITEM hItem,UINT nStateMask) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is a replacement for the base class function of
	//     the same name, to handle TVIS_FOCUSED in a multi-select tree control.
	//     Call this function to select the given tree view item. If 'hItem' is
	//     NULL, then no items are selected.
	// Parameters:
	//     hItem - Handle of a tree item.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL SelectItem(HTREEITEM hItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns the handle to the tree item that currently
	//     has focus.
	// Returns:
	//     The handle of the item that has focus, otherwise returns NULL.
	//-----------------------------------------------------------------------
	HTREEITEM GetFocusedItem() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the focus for the tree item specified
	//     by 'hItem'.
	// Parameters:
	//     hItem - Handle of a tree item.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL FocusItem(HTREEITEM hItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to clear, or select, all of the visible items
	//     in the tree control. This will not effect the focus of the tree items.
	// Parameters:
	//     bSelect - TRUE to select all the items, or FALSE to clear the selection.
	//     htItem  - Tree item to start selection from. If NULL selection will begin at the root.
	//-----------------------------------------------------------------------
	void SelectAll(BOOL bSelect = TRUE, HTREEITEM htItem=NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function selects items from 'hItemFrom' to 'hItemTo' in
	//     a multi-selection tree control. It does not select a child item if
	//     the parent is collapsed. It will remove the selection from all other
	//     items if 'bOnly' is set to TRUE.
	// Parameters:
	//     hItemFrom - Handle of the item to start selecting from.
	//     hItemTo   - Handle of the item to end selection at.
	//     bOnly     - TRUE to only select the specified range, or FALSE to keep existing
	//                 selections.
	//-----------------------------------------------------------------------
	void SelectItems(HTREEITEM hItemFrom,HTREEITEM hItemTo,BOOL bOnly = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the specified item is selected.
	// Parameters:
	//     hItem - Handle of a tree item.
	// Returns:
	//     TRUE if the specified item is selected, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL IsSelected(HTREEITEM hItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will cause all of the children of the specified
	//     tree item to be selected or deselected.
	// Parameters:
	//     hParent  - Handle of tree item to begin selection from.
	//     bSelect  - TRUE to select only the child items or FALSE to keep existing selections.
	//     bRecurse - TRUE to recurse all siblings or FALSE to select only children of the
	//                parent item.
	// Returns:
	//     TRUE if focus was on a child item, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL SelectChildren(HTREEITEM hParent,BOOL bSelect = TRUE,BOOL bRecurse = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will retrieve a reference to the typed pointer
	//     array that contains the items selected in the tree control.
	// Parameters:
	//     list - Reference to a CTypedPtrList\<CPtrList, HTREEITEM\> object.
	//-----------------------------------------------------------------------
	void GetSelectedList(CTypedPtrList<CPtrList, HTREEITEM>& list) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks to see if the tree control is a multi-select
	//     tree.
	// Returns:
	//     TRUE if the tree control is a multi-select tree, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL IsMultiSelect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the banding mode for a multi-selection tree
	//     control. If 'bLabel' is TRUE, then items are selected only when the
	//     banding rect passes over the tree item label. If FALSE, passing
	//     over any part of the tree item will cause selection to be made when
	//     the banding rect passes over it.
	// Parameters:
	//     bLabel - TRUE to select items only when banding rect passes over the text label,
	//              or FALSE to select items when banding rect passes over any part of
	//              the tree item.
	// Returns:
	//     The previous banding state.
	//-----------------------------------------------------------------------
	BOOL SetBandingHit(BOOL bLabel);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the WM_NOTIFY message that was
	//     sent was sent by the tree control or the framework.
	// Returns:
	//     true if the tree control sent the WM_NOTIFY message, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsTreeNotify();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Sends a WM_NOTIFY message to the tree control owner window.
	// Parameters:
	//     pNMHDR - Pointer to an NMHDR structure that contains the notification
	//              code and additional information. For some notification messages,
	//              this parameter points to a larger structure that has the NMHDR
	//              structure as its first member.
	// Remarks:
	//     Called by the tree control to package and send a WM_NOTIFY message
	//     to the tree control's owner window. You can override this member
	//     to provide additional functionality.
	// Returns:
	//     The return value is ignored except for notification messages that
	//     specify otherwise.
	//-----------------------------------------------------------------------
	virtual LRESULT SendNotify(LPNMHDR pNMHDR);

	//-----------------------------------------------------------------------
	// Summary:
	//     Toggles selection for all items in a multiple selection tree control.
	// Parameters:
	//     bSelect - TRUE to select, FALSE to remove selection.
	//     hIgnore - Handle to a tree item to skip while performing the operation.
	// Remarks:
	//     Called to toggle selection for all items in a multiple selection
	//     tree control. You can override this member to provide additional
	//     functionality.
	//-----------------------------------------------------------------------
	virtual void SelectAllIgnore(BOOL bSelect, HTREEITEM hIgnore);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called when a mouse button click event occurs.
	// Parameters:
	//     bLeft  - TRUE if the left mouse button is down.
	//     nFlags - Indicates whether various virtual keys are down. See Remarks
	//              section for a list of values.
	//     point  - xy cursor location.
	// Remarks:
	//     <i>nFlags,/i> can be any combination of the following values:<p/>
	//     * <b>MK_CONTROL</b> Set if the CTRL key is down.
	//     * <b>MK_LBUTTON</b> Set if the left mouse button is down.
	//     * <b>MK_MBUTTON</b> Set if the middle mouse button is down.
	//     * <b>MK_RBUTTON</b> Set if the right mouse button is down.
	//     * <b>MK_SHIFT</b>   Set if the SHIFT key is down.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnButtonDown(BOOL bLeft, UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called during item selection in a multiple selection tree control.
	// Parameters:
	//     hItem  - Handle to the last item clicked that will receive focus.
	//     bLeft  - TRUE if the left mouse button is down.
	//     nFlags - Indicates whether various virtual keys are down. See Remarks
	//              section for a list of values.
	// Remarks:
	//     <i>nFlags,/i> can be any combination of the following values:<p/>
	//     * <b>MK_CONTROL</b> Set if the CTRL key is down.
	//     * <b>MK_LBUTTON</b> Set if the left mouse button is down.
	//     * <b>MK_MBUTTON</b> Set if the middle mouse button is down.
	//     * <b>MK_RBUTTON</b> Set if the right mouse button is down.
	//     * <b>MK_SHIFT</b>   Set if the SHIFT key is down.
	//-----------------------------------------------------------------------
	virtual void DoPreSelection(HTREEITEM hItem, BOOL bLeft, UINT nFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called during item selection in a multiple selection tree control.
	// Parameters:
	//     hItem  - Handle to the last item clicked that will receive focus.
	//     bLeft  - TRUE if the left mouse button is down.
	//     nFlags - Indicates whether various virtual keys are down. See Remarks
	//              section for a list of values.
	//     point  - Mouse cursor position.
	// Remarks:
	//     <i>nFlags,/i> can be any combination of the following values:<p/>
	//     * <b>MK_CONTROL</b> Set if the CTRL key is down.
	//     * <b>MK_LBUTTON</b> Set if the left mouse button is down.
	//     * <b>MK_MBUTTON</b> Set if the middle mouse button is down.
	//     * <b>MK_RBUTTON</b> Set if the right mouse button is down.
	//     * <b>MK_SHIFT</b>   Set if the SHIFT key is down.
	//-----------------------------------------------------------------------
	virtual void DoAction(HTREEITEM hItem, BOOL bLeft, UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a selection rectangle during item selection.
	// Parameters:
	//     point  - xy location of the mouse cursor.
	//     nFlags - Indicates whether various virtual keys are down. See Remarks
	//              section for a list of values.
	// Remarks:
	//     <i>nFlags,/i> can be any combination of the following values:<p/>
	//     * <b>MK_CONTROL</b> Set if the CTRL key is down.
	//     * <b>MK_LBUTTON</b> Set if the left mouse button is down.
	//     * <b>MK_MBUTTON</b> Set if the middle mouse button is down.
	//     * <b>MK_RBUTTON</b> Set if the right mouse button is down.
	//     * <b>MK_SHIFT</b>   Set if the SHIFT key is down.
	//-----------------------------------------------------------------------
	virtual void DoBanding(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates tree item selection.
	// Parameters:
	//     pRect  - Cursor drag rectangle representing tree items to select.
	//     nFlags - Indicates whether various virtual keys are down. See Remarks
	//              section for a list of values.
	//     list   - An array of tree items that are currently selected.
	// Remarks:
	//     Called by the tree control to select tree items based upon the area
	//     specified by <i>pRect</i>.
	//     <i>nFlags,/i> can be any combination of the following values:<p/>
	//     * <b>MK_CONTROL</b> Set if the CTRL key is down.
	//     * <b>MK_LBUTTON</b> Set if the left mouse button is down.
	//     * <b>MK_MBUTTON</b> Set if the middle mouse button is down.
	//     * <b>MK_RBUTTON</b> Set if the right mouse button is down.
	//     * <b>MK_SHIFT</b>   Set if the SHIFT key is down.
	//-----------------------------------------------------------------------
	virtual void UpdateSelectionForRect(LPCRECT pRect, UINT nFlags, CTypedPtrList<CPtrList, HTREEITEM>& list);

	//-----------------------------------------------------------------------
	// Summary:
	//     Member override to determine if a tree item is valid.
	// Parameters:
	//     hti - Handle to the tree item that was found when FindItem or
	//           FindItemInBranch has been called.
	// Remarks:
	//     Override this member function to determine if the tree item
	//     specified by <i>hti</i> is a valid. If IsFindValid returns
	//     FALSE the calling functions FindItem and FindItemInBranch
	//     will return NULL.
	// Returns:
	//     TRUE in the base class. Derived classes must return TRUE to
	//     indicate success and FALSE to indicate failure.
	//-----------------------------------------------------------------------
	virtual BOOL IsFindValid(HTREEITEM hti);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called to render the tree control.
	// Parameters:
	//     dc        - Reference to the tree control device context.
	//     bInternal - FALSE to skip extended rendering tasks and to
	//                 the tree control's default rendering.
	// Remarks:
	//     This member is called to render the tree control using user defined
	//     fonts and colors. You can override this member to provide additional
	//     functionality.
	//-----------------------------------------------------------------------
	virtual void DoPaint(CDC& dc, BOOL bInternal = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the tree control background color.
	// Returns:
	//     An RGB value representing the tree control background color.
	// See Also:
	//     GetTreeTextColor, GetItemBackColor, GetItemTextColor
	//-----------------------------------------------------------------------
	COLORREF GetTreeBackColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the tree control text color.
	// Returns:
	//     An RGB value representing the tree control text color.
	// See Also:
	//     GetTreeBackColor, GetItemBackColor, GetItemTextColor
	//-----------------------------------------------------------------------
	COLORREF GetTreeTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a tree items background color.
	// Parameters:
	//     uState        - Mask indicating which states are to be retrieved. For
	//                     more information on possible values for nStateMask, see
	//                     the discussion of the state and stateMask members of
	//                     the TVITEM structure in the Platform SDK.
	//     bTreeHasFocus - true if the tree item has input focus.
	//     dwStyle       - Set of flags specifying tree item state.
	//     crBack        - Default color to be returned, usually the tree control background color.
	// Remarks:
	//     This member function is called to retrieve a tree items background
	//     color. The value retrieved will depend on the tree items state and
	//     TVS_ style.
	// Returns:
	//     An RGB value representing the specified item background color.
	// See Also:
	//     GetTreeBackColor, GetTreeTextColor, GetItemTextColor
	//-----------------------------------------------------------------------
	COLORREF GetItemBackColor(UINT uState, bool bTreeHasFocus, DWORD dwStyle, COLORREF crBack) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves a tree items text color.
	// Parameters:
	//     uState        - Mask indicating which states are to be retrieved. For
	//                     more information on possible values for nStateMask, see
	//                     the discussion of the state and stateMask members of
	//                     the TVITEM structure in the Platform SDK.
	//     bTreeHasFocus - true if the tree item has input focus.
	//     dwStyle       - Set of flags specifying tree item state.
	//     crText        - Default color to be returned, usually the tree control text color.
	// Remarks:
	//     This member function is called to retrieve a tree items text
	//     color. The value retrieved will depend on the tree items state and
	//     TVS_ style.
	// Returns:
	//     An RGB value representing the specified item text color.
	// See Also:
	//     GetTreeBackColor, GetTreeTextColor, GetItemBackColor
	//-----------------------------------------------------------------------
	COLORREF GetItemTextColor(UINT uState, bool bTreeHasFocus, DWORD dwStyle, COLORREF crText) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the tree control has edit labels.
	// Returns:
	//     true if the TVS_EDITLABELS style is set for the tree control, otherwise false.
	//-----------------------------------------------------------------------
	bool HasEditLabels() const;


protected:

//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_VIRTUAL(CXTTreeBase)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTTreeBase)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE



protected:
//{{AFX_CODEJOCK_PRIVATE
	// Needed in order to access protected functions.
	class CTreeCtrl_Friendly : public CTreeCtrl { friend class CXTTreeBase; };
//}}AFX_CODEJOCK_PRIVATE

protected:
	BOOL                m_bMultiSelect; // TRUE for a multi-selection tree control.
	BOOL                m_bBandLabel;   // TRUE to include the label when selecting tree items.
	HTREEITEM           m_hSelect;      // For shift selection.
	HTREEITEM           m_htiEdit;      // Tree item that is currently edited.
	HTREEITEM           m_htiLast;      // Tree item that was last had the mouse over.
	CColorFontMap       m_mapColorFont; // Maps HTREEITEM handles with CLRFONT structures that contains the color and logfont information for the tree item.
	CTreeCtrl_Friendly* m_pTreeCtrl;    // Self tree pointer.

private:

	bool m_bActionDone;
	bool m_bTreeNotify;
	bool m_bOkToEdit;


};

//////////////////////////////////////////////////////////////////////

AFX_INLINE HTREEITEM CXTTreeBase::GetNextItem( HTREEITEM hItem, UINT nCode ) const {
	ASSERT(::IsWindow(m_pTreeCtrl->m_hWnd)); return m_pTreeCtrl->GetNextItem( hItem, nCode );
}
AFX_INLINE HTREEITEM CXTTreeBase::GetFocusedItem() const {
	ASSERT(m_bMultiSelect); return m_pTreeCtrl->GetSelectedItem();
}
AFX_INLINE BOOL CXTTreeBase::IsSelected(HTREEITEM hItem) const {
	return !!(TVIS_SELECTED & m_pTreeCtrl->GetItemState(hItem, TVIS_SELECTED));
}
AFX_INLINE BOOL CXTTreeBase::IsMultiSelect() const {
	return m_bMultiSelect;
}
AFX_INLINE BOOL CXTTreeBase::SetBandingHit(BOOL bLabel) {
	BOOL bReturn = m_bBandLabel; m_bBandLabel = bLabel; return bReturn;
}
AFX_INLINE bool CXTTreeBase::IsTreeNotify() {
	return m_bTreeNotify;
}

//{{AFX_CODEJOCK_PRIVATE
#define DECLATE_TREE_BASE(ClassName, Tree, Base)\
class _XT_EXT_CLASS ClassName : public Tree, public Base\
{\
public:\
	UINT GetItemState(HTREEITEM hItem,UINT nStateMask) const  {\
		return Base::GetItemState(hItem, nStateMask);\
	}\
	BOOL SelectItem(HTREEITEM hItem) {\
		return Base::SelectItem(hItem);\
	}\
	BOOL SetItemState(HTREEITEM hItem,UINT nState,UINT nStateMask) {\
		return Base::SetItemState(hItem, nState, nStateMask);\
	}\
	HTREEITEM GetNextItem(HTREEITEM hItem,UINT nCode) const {\
		return Base::GetNextItem(hItem, nCode);\
	}\
	HTREEITEM GetNextItem(HTREEITEM hItem) const {\
		return Base::GetNextItem(hItem);\
	}\
protected:\
	void OnLButtonDown(UINT nFlags, CPoint point) {\
		Base::OnLButtonDown(nFlags, point);\
	}\
	void OnRButtonDown(UINT nFlags, CPoint point) {\
		Base::OnRButtonDown(nFlags, point);\
	}\
	void OnSetFocus(CWnd* pOldWnd) {\
		Base::OnSetFocus(pOldWnd);\
	}\
	void OnKillFocus(CWnd* pNewWnd) {\
		Base::OnKillFocus(pNewWnd); \
	}\
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {\
		Base::OnKeyDown(nChar, nRepCnt, nFlags);\
	}\
	BOOL OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) {\
		return Base::OnBeginLabelEdit(pNMHDR, pResult); \
	}\
	BOOL OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) {\
		return Base::OnEndLabelEdit(pNMHDR, pResult);\
	}\
	BOOL OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult) {\
		return Base::OnItemExpanding(pNMHDR, pResult);\
	}\
	BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) {\
		return Base::OnNotify(wParam, lParam, pResult);\
	}   \
	BOOL PreTranslateMessage(MSG* pMsg) {\
		return Base::PreTranslateMessage(pMsg);\
	}   \
	BOOL OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult) {\
		return Base::OnDeleteItem(pNMHDR, pResult);\
	}   \
	void OnSize(UINT nType, int cx, int cy) {\
		Base::OnSize(nType, cx, cy);\
	}   \
	void OnMouseMove(UINT nFlags, CPoint point) {   \
		Base::OnMouseMove(nFlags, point);\
	}   \
	void OnTimer(UINT_PTR nIDEvent) {       \
		Base::OnTimer(nIDEvent);\
	}   \
	void OnNcMouseMove(UINT nHitTest, CPoint point) {\
		Base::OnNcMouseMove(nHitTest, point);\
	}\
	BOOL OnEraseBkgnd(CDC*) {\
		return TRUE;\
	}\
	void OnPaint() {\
		CPaintDC dc(this);\
		DoPaint(dc); \
	}\
};

#define ON_TREECTRL_REFLECT\
	ON_WM_LBUTTONDOWN()\
	ON_WM_SETFOCUS()\
	ON_WM_KILLFOCUS()\
	ON_WM_RBUTTONDOWN()\
	ON_WM_KEYDOWN()\
	ON_WM_ERASEBKGND()\
	ON_WM_PAINT()\
	ON_WM_SIZE()\
	ON_NOTIFY_REFLECT_EX(TVN_DELETEITEM, OnDeleteItem)\
	ON_NOTIFY_REFLECT_EX(TVN_ITEMEXPANDING, OnItemExpanding)\
	ON_NOTIFY_REFLECT_EX(TVN_BEGINLABELEDIT, OnBeginLabelEdit)\
	ON_NOTIFY_REFLECT_EX(TVN_ENDLABELEDIT, OnEndLabelEdit)\
	ON_WM_MOUSEMOVE()\
	ON_WM_TIMER()\
	ON_WM_NCMOUSEMOVE

//}}AFX_CODEJOCK_PRIVATE

const DWORD TVIS_FOCUSED           = 0x0001;   //<ALIAS CXTTreeBase::SetItemState@HTREEITEM@UINT@UINT>

#endif // !defined(__XTTREEBASE_H__)
