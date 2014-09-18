// XTPTaskPanelGroup.h interface for the CXTPTaskPanelGroup class.
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
#if !defined(__XTPTASKPANELGROUP_H__)
#define __XTPTASKPANELGROUP_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTPTaskPanelItem.h"

class CXTPTaskPanel;
class CXTPTaskPanelItems;
class CXTPTaskPanelItem;
class CXTPTaskPanelPaintManager;
class CXTPTaskPanelGroupItem;
class CXTPTaskPanelGroupItems;

//===========================================================================
// Summary:
//     CXTPTaskPanelGroup is a CXTPTaskPanelItem derived class. It is used in TaskPanel control
//     as single group.
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelGroup : public CXTPTaskPanelItem
{
	DECLARE_SERIAL(CXTPTaskPanelGroup)

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelGroup object
	// Parameters:
	//     pPanel - Parent task panel pointer.
	//-------------------------------------------------------------------------
	CXTPTaskPanelGroup(CXTPTaskPanel* pPanel = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTaskPanelGroup object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTaskPanelGroup();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the parent TaskPanel control that the group belongs to
	// Returns:
	//     Parent TaskPanel control.
	//-----------------------------------------------------------------------
	CXTPTaskPanel* GetTaskPanel();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get collection of group items.
	// Returns:
	//     Collection of items of group
	//-----------------------------------------------------------------------
	CXTPTaskPanelItems* GetItems();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves an item with the specified index.
	// Parameters:
	//     nIndex - Specifies the zero-based index of the item to retrieve.
	// Returns:
	//     A pointer to a CXTPTaskPanelGroupItem object.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroupItem* GetAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to find item with specified identifier.
	// Parameters:
	//     nID - Identifier of the item to be found.
	// Returns:
	//     A pointer to item if successful; otherwise returns NULL.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroupItem* FindItem(int nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to add a new link item to group.
	// Parameters:
	//     nID - Identifier of item to be added.
	//     nImage - Image index in TaskPanel image list.
	// Returns:
	//     A pointer to the created CXTPTaskPanelGroupItem class
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroupItem* AddLinkItem(UINT nID, int nImage = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to add a new text item to group.
	// Parameters:
	//     strText - Items text
	// Returns:
	//     A pointer to created CXTPTaskPanelGroupItem class
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroupItem* AddTextItem(CString strText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to add a new control item to group.
	// Parameters:
	//     hWnd - Window Handle
	// Returns:
	//     A pointer to created CXTPTaskPanelGroupItem class
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroupItem* AddControlItem(HWND hWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method determines where a point lies in a specified item.
	// Parameters:
	//     pt     - Specifies the point to be tested.
	//     lpRect - Retrieves bounding rectangle of the item. Can be NULL if you don't need it.
	// Returns:
	//     A pointer to a CXTPTaskPanelGroupItem item that occupies the specified point or NULL if
	//     no item occupies the point.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroupItem* HitTest(CPoint pt, CRect* lpRect = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a group is repositioned.
	// Parameters:
	//     rc - new group's position.
	//     bRecalcOnly - TRUE to recalculate only.
	// Returns:
	//     Target location of group (m_rcGroupTarget).
	// Remarks:
	//     During animation the groups need to be repositioned.  This method
	//     will return the target location of the group.
	// Remarks:
	//     CXTPTaskPanel::AnimateGroups, CXTPTaskPanel::SetAnimation,
	//     CXTPTaskPanel::GetAnimation, IsDirty, OnAnimate
	//-----------------------------------------------------------------------
	CRect OnReposition(CRect rc, BOOL bRecalcOnly = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an animation step is executed.
	// Parameters:
	//     nStep - Step of animation.
	// Remarks:
	//     CXTPTaskPanel::AnimateGroups, CXTPTaskPanel::SetAnimation,
	//     CXTPTaskPanel::GetAnimation, IsDirty, OnReposition
	//-----------------------------------------------------------------------
	void OnAnimate(int nStep);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a group.
	// Parameters:
	//     pDC - Points to the client device context.
	// Remarks:
	//     OnPaint calls CXTPTaskPanelPaintManager::DrawGroupCaption and
	//     CXTPTaskPanelPaintManager::DrawGroupClient.
	//-----------------------------------------------------------------------
	void OnPaint(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the rectangle of the caption.
	// Returns:
	//     Caption rectangle.
	//-----------------------------------------------------------------------
	CRect GetCaptionRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the current rectangle of the client.
	// Returns:
	//     Client rectangle.
	//-----------------------------------------------------------------------
	CRect GetClientRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the target rectangle of the client.
	// Returns:
	//     Target client rectangle
	//-----------------------------------------------------------------------
	CRect GetTargetClientRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the target rectangle of caption.
	// Returns:
	//     Target caption rectangle
	//-----------------------------------------------------------------------
	CRect GetTargetCaptionRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to make the group special.
	// Parameters:
	//     bSpecial - TRUE if group is special.
	// Remarks:
	//     If a group is special, it will appear in a different color than the
	//     other groups.  Groups marked as special will be displayed differently
	//     than normal groups drawing the users attention to that particular
	//     group.  This is an excellent way to show the user the "default" or
	//     important items.
	//-----------------------------------------------------------------------
	void SetSpecialGroup(BOOL bSpecial = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to determine if the group is special.  A
	//     If a group is special, it will appear in a different color than
	//     the other groups.  Groups marked as special will be displayed
	//     differently than normal groups drawing the users attention to
	//     that particular group.  This is an excellent way to show the
	//     user the "default" or important items
	// Returns:
	//     TRUE is the groups is special, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsSpecialGroup();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set expandable state of group.
	// Parameters:
	//     bExpandable - TRUE if group is expandable.
	//-----------------------------------------------------------------------
	void SetExpandable(BOOL bExpandable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if group is expandable.
	// Returns:
	//     TRUE is the groups is expandable, FALSE if it can't be expanded.
	//-----------------------------------------------------------------------
	BOOL IsExpandable();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to show or hide group's caption.
	// Parameters:
	//     bShow - TRUE to show caption; otherwise FALSE.
	//-----------------------------------------------------------------------
	void ShowCaption(BOOL bShow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves if caption is visible.
	// Returns:
	//     TRUE if caption is visible, FALSE if caption is hidden.
	// See Also: ShowCaption
	//-----------------------------------------------------------------------
	BOOL IsCaptionVisible();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the group is currently expanded.
	// Returns:
	//     TRUE if the group is expanded, FALSE if the group is collapsed.
	// See Also:
	//     SetExpanded, CXTPTaskPanel::AnimateGroups, CXTPTaskPanel::SetAnimation,
	//     CXTPTaskPanel::GetAnimation, IsDirty, IsExpanding
	//-----------------------------------------------------------------------
	BOOL IsExpanded();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if a group is currently expanding.
	// Returns:
	//     TRUE is group is currently in the process of expanding, FALSE otherwise.
	// See Also:
	//     SetExpanded, CXTPTaskPanel::AnimateGroups, CXTPTaskPanel::SetAnimation,
	//     CXTPTaskPanel::GetAnimation, IsDirty, IsExpanded
	//-----------------------------------------------------------------------
	BOOL IsExpanding();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the height of expanded group.
	// Returns:
	//     Client height of expanded group.
	//-----------------------------------------------------------------------
	int GetExpandedClientHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to expand or collapse the group.
	// Parameters:
	//     bExpanded - TRUE to expand group; otherwise FALSE.
	// See Also:
	//     CXTPTaskPanel::AnimateGroups, CXTPTaskPanel::SetAnimation,
	//     CXTPTaskPanel::GetAnimation, IsDirty, IsExpanding, IsExpanded
	//-----------------------------------------------------------------------
	void SetExpanded(BOOL bExpanded);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to determine if the current group position
	//     is "dirty" i.e. Moving during animation.
	// Returns:
	//     TRUE if current position of group is dirty.
	// Remarks:
	//     During animation, the group moves from it's old\current position to the new\target
	//     position.  IsDirty indicates that the animation has not finished yet
	//     and its current position is <> to the target position.  The old\current position
	//     is stored in m_rcGroupCurrent, and the new\target position is stored in
	//     m_rcGroupTarget.
	//
	//     Animation occurs when the task panel first becomes visible and when
	//     the expand\collapse button is clicked in the group caption.
	// See Also:
	//     CXTPTaskPanel::AnimateGroups, CXTPTaskPanel::SetAnimation,
	//     CXTPTaskPanel::GetAnimation, OnAnimate, m_rcGroupTarget, m_rcGroupCurrent
	//-----------------------------------------------------------------------
	BOOL IsDirty();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves outer margins of group
	// Returns:
	//     Outer margins
	//-----------------------------------------------------------------------
	CRect& GetOuterMargins();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves inner margins of group
	// Returns:
	//     Inner margins
	//-----------------------------------------------------------------------
	CRect& GetInnerMargins();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the group can accept focus.
	// Returns:
	//     TRUE if the group can receive focus, FALSE if the group
	//     cannot receive focus.
	//-----------------------------------------------------------------------
	BOOL IsAcceptFocus();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to draw a bitmap in the lower right corner of a
	//     task panel group.
	// Parameters:
	//     nID            - Bitmap resource identifier.
	//     clrTransparent - Color in bitmap to make transparent when bAlpha is
	//                      FALSE.
	//     bAlpha         - TRUE to use alpha transparency and load a
	//                      32-bit bitmap.  FALSE to load a 24-bit bitmap and
	//                      use the color stored in clrTransparent as the
	//                      transparent color.
	// Remarks:
	//     The bitmap will always be in the lower right corner of the group,
	//     and will not move.
	//     The bitmap is drawn on the group in the OnFillClient method.
	// See Also:
	//     OnFillClient, m_szClientBitmap
	//-----------------------------------------------------------------------
	void SetClientBitmap(UINT nID, COLORREF clrTransparent, BOOL bAlpha = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the task panel group client area is
	//     drawn.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc  - CRect object specifying size of area.
	// Remarks:
	//     This method is called from CXTPTaskPanelPaintManager::DrawGroupClientFace
	//     when the group client face needs filled.
	//
	//     This method checks to see if a bitmap should be drawn in the lower
	//     right corner of the task panel group.  Then it will drawn the bitmap
	//     if a bitmap was added.  A bitmap can be added with the SetClientBitmap
	//     method.
	// See Also:
	//     SetClientBitmap
	//-----------------------------------------------------------------------
	virtual void OnFillClient(CDC* pDC, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine the item layout currently used for this group.
	// Returns:
	//     The currently set XTPTaskPanelItemLayout for this group.
	//-----------------------------------------------------------------------
	XTPTaskPanelItemLayout GetItemLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify the item layout of the XTPTaskPanelItems in
	//     this group.  Layouts include text only, image only, image and text, and
	//     image with text below the image.  This will apply to this group only.
	// Parameters:
	//     itemLayout - The desired XTPTaskPanelItemLayout for this group.
	//-----------------------------------------------------------------------
	void SetItemLayout(XTPTaskPanelItemLayout itemLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to select a task panel group item.  Multiple items can have
	//     focus if CXTPTaskPanel::m_bSelectItemOnFocus is FALSE.
	// Parameters:
	//     pItem - Pointer to a task panel group item.
	// Remarks:
	//     Selected items will appear "pressed" or highlighted depending on
	//     the selected theme.  Selected items will have
	//     XTPTaskPanelGroupItem::m_bSelected as TRUE.
	// See Also: XTPTaskPanelGroupItem::IsItemSelected, XTPTaskPanelGroupItem::SetItemSelected
	//-----------------------------------------------------------------------
	void SetSelectedItem(CXTPTaskPanelGroupItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to check to see if the item currently being
	//     dragged over the group belongs to the group.
	// Returns:
	//     TRUE if the item belongs to the group, FALSE otherwise.
	// Remarks:
	//     This checks to see if CSTPTaskPanel::m_pItemDragOver belongs to the group.
	//     This is used in the PaintManager to determine if an inverted rectangle
	//     must be drawn for the item.  An inverted rectangle is only drawn when
	//     the XTPTaskPanelPaintTheme is set to xtpTaskPanelThemeToolbox.
	// See Also:
	//     XTPTaskPanelPaintTheme, CXTPTaskPanelToolboxTheme::DrawGroupCaption
	//-----------------------------------------------------------------------
	BOOL IsChildItemDragOver();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine the number of items that are in this group.
	// Returns:
	//     Number of items in this group.
	//-----------------------------------------------------------------------
	int GetItemCount();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if a group scroll button is enabled.
	// Parameters:
	//     bScrollButtonUp - If TRUE, check to see if the "scroll up" button
	//                       is enabled, If FALSE, check to see if the
	//                       "scroll down" button is enabled.
	// Returns:
	//     TRUE is the group scroll button is enabled, FALSE if the buttons
	//     is disabled.
	// Remarks:
	//     CXTPTaskPanel::UpdateScrollButtons uses this method to update the
	//     state of the scroll buttons.
	//-----------------------------------------------------------------------
	BOOL IsScrollButtonEnabled(BOOL bScrollButtonUp);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to get the scroll offset of the first visible
	//     item in the group in pixels.
	// Returns:
	//     Scroll offset of first visible item in group in pixels.
	// Remarks:
	//     GetScrollOffsetPos returns the height in pixels that has been
	//     scrolled.  GetOffsetItem gets the index of the first visible item.
	// See Also:
	//     Scroll, GetOffsetItem, GetItemCount, GetItemsInRow
	//-----------------------------------------------------------------------
	int GetScrollOffsetPos();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the index of the first visible item in group.
	// Returns:
	//     Index of first visible item in group.
	// Remarks:
	//     This will return the index of the first visible item in the group,
	//     this is helpful when the items in the group are scrolled.
	// See Also:
	//     GetScrollOffsetPos, Scroll, GetItemCount, GetItemsInRow
	//-----------------------------------------------------------------------
	int GetOffsetItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to scroll to an item in the task panel group. When
	//     the group has scroll buttons.
	// Parameters:
	//     nDelta - Offset of item to scroll to.  GetItemCount and
	//              GetOffsetItem can be used to determine which
	//              item you need to scroll to.
	// Remarks:
	//     The scroll offset starts at zero.  The offset will remain at
	//     zero until the group's view is too small to display all
	//     of items.  If the group is scrolled, the offset will increment
	//     by 1 for each item that is scrolled from the top.  For example,
	//     If the top top items in the group are not visible, then the
	//     scroll offset is 4 (zero based index).
	//
	//     GetItemCount and GetOffsetItem can be used to determine which
	//     item to scroll to.  Setting nDelta to 0 will scroll to the top
	//     of the group.
	//
	//     Note:  If there are more than 1 item in a row like when the XTPTaskPanelItemLayout
	//            is set to xtpTaskItemLayoutImages, then the offset will
	//            increment by GetItemsInRow.
	// See Also:
	//     GetScrollOffsetPos, GetOffsetItem, GetItemCount, GetItemsInRow
	//-----------------------------------------------------------------------
	void Scroll(int nDelta);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to make sure that an item is visible in the task panel.
	//     This will scroll the task panel and group that the item is in
	//     until the item is visible.
	// Parameters:
	//     pItem - Item that should be visible.
	//-----------------------------------------------------------------------
	void EnsureVisible(CXTPTaskPanelGroupItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the number of group items per row in the
	//     task panel group.
	// Returns:
	//     The number of group items per row in the group.
	// See Also:
	//     If the task panel behavior is set to xtpTaskPanelBehaviourExplorer or
	//     xtpTaskPanelBehaviourList, then the number of items per row is
	//     always 1.  If the behavior is set to xtpTaskPanelBehaviourToolbox, then
	//     there can be multiple items per row.
	//
	//     If the behavior is set to to xtpTaskPanelBehaviourExplorer or
	//     xtpTaskPanelBehaviourList, then the layout must be set to
	//     xtpTaskItemLayoutImages for multiple items on a row.
	// See Also:
	//     XTPTaskPanelBehaviour, XTPTaskPanelItemLayout, Scroll
	//-----------------------------------------------------------------------
	int GetItemsInRow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the rectangle of the "tooltip zone"
	//     while the mouse is positioned over the group caption.
	// Returns:
	//     The rectangle of the "tooltip zone" while the mouse is positioned
	//     over the group caption.
	// Remarks:
	//     The "tooltip zone" is the area that a tooltip will be displayed
	//     while the mouse is positioned over the group caption.
	// See Also:
	//     CXTPTaskPanelGroupItem::GetHitTestRect
	//-----------------------------------------------------------------------
	virtual CRect GetHitTestRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the height of the group caption.
	// Returns:
	//     Height of group caption.
	//-----------------------------------------------------------------------
	int GetCaptionHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member too get the size used to draw group item icons.
	// Returns:
	//     Currently set group item icon size.
	// See Also:
	//     SetItemIconSize
	//-----------------------------------------------------------------------
	CSize GetItemIconSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the icon size of the group items.
	// Parameters:
	//     szItemIcon - Item icon size.
	// See Also:
	//     GetItemIconSize
	//-----------------------------------------------------------------------
	void SetIconSize(CSize szItemIcon);


protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine the area that can be used to draw
	//     the Active group if the behavior is not xtpTaskPanelBehaviourExplorer.
	// Returns:
	//     Height in pixels after group caption or available area.
	// See Also:
	//     OnCalcClientHeight
	//-----------------------------------------------------------------------
	int CalcInsideHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to calculate the group client area needed to
	//     display all items.
	// Returns:
	//     Height in pixels group client area needed to display all items.
	// See Also:
	//     CalcInsideHeight
	//-----------------------------------------------------------------------
	int OnCalcClientHeight();

protected:

	CRect m_rcGroupTarget;              // Target rectangle of group during animation.
	CRect m_rcGroupCurrent;             // Current rectangle of group during animation.

	CRect m_rcMarginsOuter;             // Outer margins.
	CRect m_rcMarginsInner;             // Inner margins.

	int m_nCaptionHeight;               // Height of caption.

	BOOL m_bSpecial;                    // TRUE if group is special.

	BOOL m_bExpandable;                 // TRUE if group is expandable.
	BOOL m_bExpanded;                   // TRUE if group is expanded.
	BOOL m_bExpanding;                  // TRUE if group is expanding now.
	int m_nExpandedClientHeight;        // Expanded client height.
	BOOL m_bCaptionVisible;             // TRUE if caption is visible.

	CImageList m_ilClient;              // ImageList that holds the image to be displayed int he lower right corner of the group.
	CSize m_szClientBitmap;             // Image to be displayed in the lower right corner of the group.

	int m_nScrollOffset;                // Scroll offset of group.  Only applies if the task panel behavior is set to xtpTaskPanelBehaviourList or xtpTaskPanelBehaviourToolbox.
	int m_nItemsInRow;                  // Number of group items in a row when using the xtpTaskPanelBehaviourToolbox XTPTaskPanelBehaviour. For all other behaviors it will only be 1 unless the layout is set to to xtpTaskPanelBehaviourExplorer or xtpTaskPanelBehaviourList.
	CSize m_szItemIcon;                 // Size of group item icons.

	CXTPTaskPanelGroupItems* m_pItems;  // Collection of items.
	XTPTaskPanelItemLayout m_itemLayout;// Currently set layout for the group.

private:

	friend class CXTPTaskPanel;
};

AFX_INLINE CXTPTaskPanel* CXTPTaskPanelGroup::GetTaskPanel() {
	ASSERT(m_pPanel != NULL);
	return m_pPanel;
}
AFX_INLINE CXTPTaskPanelItems* CXTPTaskPanelGroup::GetItems() {
	ASSERT(m_pItems);
	return (CXTPTaskPanelItems*)m_pItems;
}


AFX_INLINE int CXTPTaskPanelGroup::GetExpandedClientHeight() {
	return m_nExpandedClientHeight;
}
AFX_INLINE CRect& CXTPTaskPanelGroup::GetOuterMargins() {
	return m_rcMarginsOuter;
}
AFX_INLINE CRect& CXTPTaskPanelGroup::GetInnerMargins() {
	return m_rcMarginsInner;
}
AFX_INLINE int CXTPTaskPanelGroup::GetOffsetItem() {
	return m_nScrollOffset;
}
int AFX_INLINE CXTPTaskPanelGroup::GetItemsInRow() {
	return m_nItemsInRow;
}
int AFX_INLINE CXTPTaskPanelGroup::GetCaptionHeight() {
	return m_nCaptionHeight;
}

#endif // !defined(__XTPTASKPANELGROUP_H__)
