// XTPTaskPanel.h interface for the CXTPTaskPanel class.
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
#if !defined(__XTPTASKPANEL_H__)
#define __XTPTASKPANEL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPTaskPanelGroups;
class CXTPTaskPanelGroup;
class CXTPTaskPanelPaintManager;
class CXTPTaskPanelItems;
class CXTPTaskPanelItem;
class CXTPTaskPanelGroupItem;
class CXTPImageManager;

#include "XTPTaskPanelDefines.h"

//===========================================================================
// Summary:
//     CXTPTaskPanelScrollButton is a standalone class that represents a
//     task panel scroll button.
// Remarks:
//     The task panel uses a scroll button in the group caption if the
//     group is expandable to expand and retract the group.
//
//     The task panel also uses scroll buttons in task panel group objects
//     when there are more items in the group than can be viewed.  Only the
//     necessary buttons are displayed.  If the items in the group can not
//     be scrolled anymore, then that button is removed.  Scroll buttons are
//     only visible when using the xtpTaskPanelBehaviourList and
//     xtpTaskPanelBehaviourToolbox task panel behaviors.  The group scroll
//     buttons are stored in m_pScrollButton.
// See Also:
//     m_pScrollButton
//===========================================================================
class CXTPTaskPanelScrollButton
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelScrollButton object.
	// Parameters:
	//     bScrollUp - If TRUE, the button is the "scroll up" button, If FALSE,
	//                 the button is a "scroll down" button.
	//-----------------------------------------------------------------------
	CXTPTaskPanelScrollButton(BOOL bScrollUp);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the scroll button is visible.
	// Returns:
	//     TRUE is the scroll button is visible, FALSE is the scroll button
	//     is not visible.
	//-----------------------------------------------------------------------
	BOOL IsVisible();

public:
	BOOL bScrollUp;                     // If TRUE, the button is the "scroll up" button, If FALSE, the button is a "scroll down" button.
	BOOL bEnabled;                      // TRUE if the scroll button is enabled, FALSE if disabled.
	BOOL bPressed;                      // TRUE if scroll button is currently pressed, FALSE otherwise.
	CRect rcButton;                     // Bounding rectangle of the scroll button.  Will be empty is the button is not visible.
	BOOL bHot;                          // TRUE if scroll button is "hot," FALSE otherwise.  A scroll button is how when the mouse cursor is placed over the button.

	CXTPTaskPanelGroup* pGroupScroll;   // Pointer to the group that uses this group item scroll button.
	CXTPTaskPanelGroup* pGroupCaption;  // Pointer to the group that uses this group caption scroll button.
};

//---------------------------------------------------------------------------
// Summary:
//     CXTPTaskPanel is a CWnd derived class. It is used to implement
//     an Explorer and Office-like Task Panel control.
//---------------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTaskPanel : public CWnd
{
	DECLARE_DYNAMIC(CXTPTaskPanel)

private:
	class CPanelDropTarget;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a TaskPanel object
	//-----------------------------------------------------------------------
	CXTPTaskPanel();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTaskPanel object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTaskPanel();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method creates the TaskPanel control.
	// Parameters:
	//     dwStyle - Style for the TaskPanel.
	//     rect -  Specifies the size and position of the TaskPanel control.
	//     pParentWnd - Specifies the parent window of the TaskPanel control.
	//     nID - Specifies the TaskPanel control ID.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL Create(DWORD dwStyle, const RECT&  rect, CWnd* pParentWnd, UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get group collection of TaskPanel control.
	// Returns:
	//     A pointer to CXTPTaskPanelItems class that represents the groups of TaskPanel.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItems* GetGroups();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to add a new group.
	// Parameters:
	//     nID    - Identifier of the group to be added.
	//     nImage - Image index of group to be added.
	// Returns:
	//     A pointer to the created CXTPTaskPanelGroup class
	// See Also: SetGroupImageList
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroup* AddGroup(UINT nID, int nImage = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to find a group with specified identifier.
	// Parameters:
	//     nID - Identifier of group to be found.
	// Returns:
	//     A pointer to group if successful; otherwise returns NULL.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroup* FindGroup(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves a group with specified index.
	// Parameters:
	//     nIndex - Specifies the zero-based index of the group to retrieve.
	// Returns:
	//     A pointer to a CXTPTaskPanelGroup object.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroup* GetAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get scroll offset.
	// Returns:
	//     Scroll offset.
	// Remarks:
	//     This returns the scroll offset of the scroll bar that is displayed to
	//     scroll the entire task panel.  This does return the offset to the
	//     scroll bar placed inside a task panel group.  The task panel group offset
	//     can be retrieved with CXTPTaskPanelGroup::GetScrollOffsetPos and
	//     CXTPTaskPanelGroup::GetOffsetItem
	// See Also:
	//  CXTPTaskPanelGroup::GetOffsetItem, CXTPTaskPanelGroup::GetScrollOffsetPos
	//-----------------------------------------------------------------------
	int GetScrollOffset();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the current height of a client area.
	// Returns:
	//     Current height of a client area
	//-----------------------------------------------------------------------
	int GetClientHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to reposition the groups in the task panel.
	// Parameters:
	//     bRecalcOnly - TRUE to recalculate only without positioning.
	//-----------------------------------------------------------------------
	void Reposition(BOOL bRecalcOnly = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set hot item of the TaskPanel control.
	// Parameters:
	//     pItem - Item to be hot.
	//-----------------------------------------------------------------------
	void SetHotItem(CXTPTaskPanelItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the current "hot" item.
	// Returns:
	//     Retrieves the current hot item.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem* GetHotItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to focus the specified item.
	// Parameters:
	//     pItem - Item to be focused.
	//     bDrawFocusRect - TRUE to draw focused rect.
	//-----------------------------------------------------------------------
	void SetFocusedItem(CXTPTaskPanelItem* pItem, BOOL bDrawFocusRect = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to make sure that an item is visible in the task panel.
	//     This will scroll the task panel and group that the item is in
	//     until the item is visible.
	// Parameters:
	//     pItem - Item that should be visible.
	//-----------------------------------------------------------------------
	void EnsureVisible(CXTPTaskPanelItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get focused item.
	// Returns:
	//     Item currently focused.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem* GetFocusedItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if a focus rectangle is drawn around
	//     items when the Up and Down arrows are used to navigate items.
	// Returns:
	//     TRUE to draw a focus rectangle around an item when the Up and Down arrows
	//     are used to navigate the items in the TaskPanel.
	// See Also:
	//     DrawFocusRect
	//-----------------------------------------------------------------------
	BOOL IsDrawFocusRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method determines where a point lies in a specified group.
	// Parameters:
	//     pt - Specifies the point to be tested.
	//     pbCaption - Pointer to BOOL value to return if point lies on group caption.
	// Returns:
	//     A pointer to a CXTPTaskPanelGroup item that occupies the specified point or NULL if
	//     no group occupies the point.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroup* HitTestGroup(CPoint pt, BOOL* pbCaption = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method determines where a point lies in a specified item.
	// Parameters:
	//     pt - Specifies the point to be tested.
	//     bClientAvail - TRUE to return group item if point lies in its client area.
	// Returns:
	//     A pointer to a CXTPTaskPanelItem item that occupies the specified point or NULL if
	//     no item occupies the point.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem* HitTestItem(CPoint pt, BOOL bClientAvail = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set group outer margins.
	// Parameters:
	//     nLeft - Left margin.
	//     nTop - Top margin,
	//     nRight - Right margin,
	//     nBottom - Bottom margin.
	//     nMiddle - Margin between groups.
	// Remarks:
	//     This is used as the default margins for all groups.
	// See Also:
	//     GetMargins, CXTPTaskPanelPaintManager::GetGroupSpacing,
	//     CXTPTaskPanelPaintManager::GetGroupOuterMargins, CXTPTaskPanelPaintManager::GetGroupInnerMargins
	//     CXTPTaskPanelPaintManager::GetControlMargins
	//-----------------------------------------------------------------------
	void SetMargins(long nLeft, long nTop, long nRight, long nBottom, long nMiddle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Assigns an image list to a TaskPanel control.
	// Parameters:
	//     pImageList - Pointer to the image list to be assigned to the TaskPanel control.
	//                  This parameter can be NULL to update size of item icons.
	//     szItemIcon - Items icon size to be used.
	// See Also: SetGroupImageList, GetImageManager
	//-----------------------------------------------------------------------
	void SetImageList(CImageList* pImageList, CSize szItemIcon = CSize(16, 16));

	//-----------------------------------------------------------------------
	// Summary:
	//     Assigns an image list to a TaskPanel's groups.
	// Parameters:
	//     pImageList  - Pointer to the image list to be assigned to the groups of TaskPanel.
	//                   This parameter can be NULL to update size of group icons.
	//     szGroupIcon - Groups icon size to be used.
	//-----------------------------------------------------------------------
	// See Also: SetImageList, GetImageManager
	void SetGroupImageList(CImageList* pImageList, CSize szGroupIcon = CSize(32, 32));

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves Image manager associated with Task Panel.
	// Returns:
	//     Pointer to the image manager object that stores the images in
	//     the task panel.
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to assign new image manager.
	// Parameters:
	//     pImageManager - Pointer to the image manager object that stores the
	//     images in the task panel.
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set groups to expandable.
	// Parameters:
	//     bExpandable - TRUE if TaskPanel groups are expandable.
	//-----------------------------------------------------------------------
	void SetExpandable(BOOL bExpandable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if groups are expandable
	// Returns:
	//     TRUE if groups are expandable, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsExpandable();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set animation of the TaskPanel control
	// Parameters:
	//     panelAnimation - Animation type to be set. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     Animation type can be one of the following:
	//     * <b>xtpTaskPanelAnimationYes</b> Default value, enables animation.
	//     * <b>xtpTaskPanelAnimationNo</b> Disables animation.
	//     * <b>xtpTaskPanelAnimationSystem</b> Use system settings to determine animation.
	//
	// See Also:
	//     CXTPTaskPanel::AnimateGroups, CXTPTaskPanel::GetAnimation,
	//     CXTPTaskPanelGroup::IsDirty, CXTPTaskPanelGroup::OnAnimate, XTPTaskPanelAnimation
	//-----------------------------------------------------------------------
	void SetAnimation(XTPTaskPanelAnimation panelAnimation);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get animation type.
	// Returns:
	//     Animation type of TaskPanel control.
	// See Also:
	//     CXTPTaskPanel::AnimateGroups, CXTPTaskPanel::SetAnimation,
	//     CXTPTaskPanelGroup::IsDirty, CXTPTaskPanelGroup::OnAnimate, XTPTaskPanelAnimation
	//-----------------------------------------------------------------------
	XTPTaskPanelAnimation GetAnimation();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to lock/unlock redraw of controls.
	// Parameters:
	//     bLockRedraw - TRUE to lock redraw.
	// Remarks:
	//     bLockRedraw should be TRUE when you want to add a lot of items and
	//     don't want CXTPTaskPanel::Reposition and CXTPTaskPanel::ReDraw called
	//     each time an item is added.
	//-----------------------------------------------------------------------
	void SetLockRedraw(BOOL bLockRedraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get next item in tab order.
	// Parameters:
	//     pItem - Current item.
	//     nDirection - Direction to find (1 to forward, -1 to backward)
	//     bTab - TRUE to use tabulation.
	//     bSkipRowItems - TRUE to get next item in next row.
	// Returns:
	//     Next item.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem* GetNextItem(CXTPTaskPanelItem* pItem, int nDirection, BOOL bTab, BOOL bSkipRowItems = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when user clicks on the specified item.
	// Parameters:
	//     pItem - Pointer to clicked item.
	//-----------------------------------------------------------------------
	virtual void OnClick(CXTPTaskPanelGroupItem* pItem);

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
	//     AnimateGroups, SetAnimation, GetAnimation, CXTPTaskPanelGroup::OnAnimate,
	//     CXTPTaskPanelGroup::m_rcGroupTarget, CXTPTaskPanelGroup::m_rcGroupCurrent,
	//     CXTPTaskPanelGroup::IsExpanding, CXTPTaskPanelGroup::IsExpanded,
	//     CXTPTaskPanelGroup::SetExpanded
	//-----------------------------------------------------------------------
	BOOL IsDirty();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called then the task panel's scroll bar has been scrolled.
	// Parameters:
	//     nCurPos - Position on scroll bar to scroll to.
	// Remarks:
	//     This member is called when the scroll bar that is displayed to scroll the entire
	//     task panel is scrolled.  This does not get called when the scroll bar placed inside
	//     a task panel group is scrolled.
	//-----------------------------------------------------------------------
	void OnSrollChanged(int nCurPos);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to animate the groups when animation is enabled.
	// Parameters:
	//     bCheckDirty - TRUE if the group(s) have not reached their target location
	//                   during animation, FALSE if the group(s) have reached their
	//                   target location.
	// Returns:
	//     TRUE if the group(s) are still dirty (has not reached their target location),
	//     FALSE if the group(s) have moved to their target location.
	// Remarks:
	//     This is called when the expand\collapse button is pressed in the
	//     group caption causing the group to expand\contract.
	// See Also:
	//     SetAnimation, GetAnimation, CXTPTaskPanelGroup::OnAnimate,
	//     CXTPTaskPanelGroup::m_rcGroupTarget, CXTPTaskPanelGroup::m_rcGroupCurrent,
	//     CXTPTaskPanelGroup::IsExpanding, CXTPTaskPanelGroup::IsExpanded,
	//     CXTPTaskPanelGroup::SetExpanded
	//-----------------------------------------------------------------------
	BOOL AnimateGroups(BOOL bCheckDirty = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the size used to display item icons.
	// Remarks:
	//     By default, item icons are 16x16.  To change this size you must
	//     specify the size in m_szItemIcon.  "m_szItemIcon = CSize(16, 16)"
	// Returns:
	//     Size of item icons.
	//-----------------------------------------------------------------------
	CSize GetItemIconSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the size used to display group icons.
	// Remarks:
	//     By default, item icons are 32x32.  To change this size you must
	//     specify the size in m_szGroupIcon.
	//     <code>m_szGroupIcon = CSize(32, 32)"</code>
	// Returns:
	//     Size of group icons.
	//-----------------------------------------------------------------------
	CSize GetGroupIconSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to specify the size of the Icons
	//     used in the TaskPanel.
	// Parameters:
	//     szIcon - Size of icons used.
	//-----------------------------------------------------------------------
	void SetIconSize(CSize szIcon);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to switch the visual theme of the TaskPanel control.
	// Parameters:
	//     paintTheme - New visual theme.  Can be any of the values listed in the Remarks section.
	// Remarks:
	//     paintTheme can be one of the following:
	//     * <b>xtpTaskPanelThemeOffice2000</b> Enables Office 2000 style theme.
	//     * <b>xtpTaskPanelThemeOffice2003</b> Enables Office 2003 style theme.
	//     * <b>xtpTaskPanelThemeNativeWinXP</b> Enables Explorer style theme.
	//     * <b>xtpTaskPanelThemeOffice2000Plain</b> Enables classic Office 2000 style theme.
	//     * <b>xtpTaskPanelThemeOfficeXPPlain</b> Enables classic Office XP style theme.
	//     * <b>xtpTaskPanelThemeOffice2003Plain</b> Enables classic Office 2003 style theme.
	//     * <b>xtpTaskPanelThemeNativeWinXPPlain</b> Enables classic Explorer style theme.
	//     * <b>xtpTaskPanelThemeToolbox</b> Enables VS.NET style Toolbox theme.
	//     * <b>xtpTaskPanelThemeToolboxWhidbey</b> Enables Whidbey VS 2005 style Toolbox theme.
	//     * <b>xtpTaskPanelThemeListView</b> Enables Standard List View theme.
	//     * <b>xtpTaskPanelThemeListViewOfficeXP</b> Enables Office XP List View theme.
	//     * <b>xtpTaskPanelThemeListViewOffice2003</b> Enables Office 2003 List View theme.
	//     * <b>xtpTaskPanelThemeShortcutBarOffice2003</b> Enables ShortcutBar Office 2003 theme.
	//
	// See Also: CXTPTaskPanel::GetCurrentTheme
	//-----------------------------------------------------------------------
	void SetTheme(XTPTaskPanelPaintTheme paintTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set a custom theme.
	// Parameters:
	//     pPaintManager - New paint manager.
	//-----------------------------------------------------------------------
	void SetCustomTheme(CXTPTaskPanelPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the current paint manager.
	// Returns:
	//     The current paint manager.
	// See Also: CXTPTaskPanel::SetTheme
	//-----------------------------------------------------------------------
	CXTPTaskPanelPaintManager* GetPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the current visual theme.
	// Returns:
	//     The current theme.
	// See Also: CXTPTaskPanel::SetTheme
	//-----------------------------------------------------------------------
	XTPTaskPanelPaintTheme GetCurrentTheme();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to expand or collapse a group.
	// Parameters:
	//     pGroup    - CXTPTaskPanelGroup to be expanded or closed
	//     bExpanded - TRUE to expand group, FALSE to collapse group
	// See Also: CXTPTaskPanelGroup::SetExpandable, CXTPTaskPanelGroup::IsExpandable
	//-----------------------------------------------------------------------
	virtual void ExpandGroup(CXTPTaskPanelGroup* pGroup, BOOL bExpanded);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if a group is currently expanded
	//     or collapsed.
	// Parameters:
	//     pGroup - CXTPTaskPanelGroup to test.
	// Returns:
	//     TRUE if the group is currently expanded, FALSE if it is closed
	//-----------------------------------------------------------------------
	BOOL IsGroupExpanded(CXTPTaskPanelGroup* pGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the type of hot tracking that is used
	//     when the mouse is moved over a group or item.
	// Parameters:
	//     hotTrackStyle - The desired XTPTaskPanelHotTrackStyle.
	//-----------------------------------------------------------------------
	void SetHotTrackStyle(XTPTaskPanelHotTrackStyle hotTrackStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine the type of hot tracking currently used.
	// Returns:
	//     The current XTPTaskPanelHotTrackStyle used.
	//-----------------------------------------------------------------------
	XTPTaskPanelHotTrackStyle GetHotTrackStyle();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine the item layout currently used.
	// Returns:
	//     The currently set XTPTaskPanelItemLayout.
	//-----------------------------------------------------------------------
	XTPTaskPanelItemLayout GetItemLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify the item layout of the XTPTaskPanelItems.
	//     Layouts include text only, image only, image and text, and
	//     image with text below the image.    This applies to all groups
	//     unless each groups layout is set individually.
	// Parameters:
	//     itemLayout - The desired XTPTaskPanelItemLayout.
	// See Also: CXTPTaskPanelGroup::SetItemLayout
	//-----------------------------------------------------------------------
	void SetItemLayout(XTPTaskPanelItemLayout itemLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify the behavior of the TaskPanel.
	//     The TaskPanel can behave like a ToolBox, List View, or Explorer.
	// Parameters:
	//     panelBehaviour - The desired XTPTaskPanelBehaviour.
	//-----------------------------------------------------------------------
	void SetBehaviour(XTPTaskPanelBehaviour panelBehaviour);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine the behavior the TaskPanel is using.
	// Returns:
	//     The currently set XTPTaskPanelBehaviour.
	//-----------------------------------------------------------------------
	XTPTaskPanelBehaviour GetBehaviour();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the currently active group.
	// Returns:
	//     Pointer to the active CXTPTaskPanelGroup.
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroup* GetActiveGroup();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether items will receive focus when
	//     they are clicked.  If bSelect is TRUE, the item clicked receives
	//     focus and the previously focused item loses focus.  This must be
	//     FALSE to use toggle buttons or to allow multiple items to have
	//     focus.
	// Parameters:
	//     bSelect - TRUE if items will receive focus when they are clicked.
	//               If FALSE, items can be clicked, but they will not receive
	//               focus.
	// See Also: SetFocusedItem, GetFocusedItem
	//-----------------------------------------------------------------------
	void SetSelectItemOnFocus(BOOL bSelect);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to allow only one item to be selected.
	// Parameters:
	//     bSingleSelection - TRUE to use single selection.
	// See Also: IsSingleSelection, SetSelectItemOnFocus
	//-----------------------------------------------------------------------
	void SetSingleSelection(BOOL bSingleSelection = TRUE);

	
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get Single Selection property of control.
	// See Also: SetSingleSelection, SetSelectItemOnFocus
	//-----------------------------------------------------------------------
	BOOL IsSingleSelection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to draw a focus rectangle around a
	//     CXTPTaskPanelGroupItem \ CXTPTaskPanelItem when the Up and Down arrows
	//     are used to navigate the items in the TaskPanel.
	// Parameters:
	//     bAccept - TRUE if the focus rectangle is to be drawn, FALSE otherwise.
	// See Also:
	//     IsDrawFocusRect
	//-----------------------------------------------------------------------
	void DrawFocusRect(BOOL bAccept);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the outer margins of groups in the task
	//     panel.
	// Returns:
	//     Outer group margins.
	//-----------------------------------------------------------------------
	CRect GetMargins();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the current clipboard format.
	// Returns:
	//     Current CLIPFORMAT clipboard format.
	//-----------------------------------------------------------------------
	static CLIPFORMAT AFX_CDECL GetClipboardFormat();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether you can drag items within the
	//     TaskPanel and from the TaskPanel to other object.
	// Parameters:
	//     dragOptions - Drag options
	// Remarks:
	//     dragOptions parameter can be one or more of the following values:
	//          * <b>xtpTaskItemAllowDragCopyWithinGroup</b> To allow copy within group only
	//          * <b>xtpTaskItemAllowDragCopyWithinControl</b> To allow copy within task panel only
	//          * <b>xtpTaskItemAllowDragCopyOutsideControl</b> To allow copy outside task panel only
	//          * <b>xtpTaskItemAllowDragCopy</b> To allow copy operation
	//          * <b>xtpTaskItemAllowDragMoveWithinGroup</b> To allow move within group only
	//          * <b>xtpTaskItemAllowDragMoveWithinControl</b> To allow move within task panel only
	//          * <b>xtpTaskItemAllowDragMoveOutsideControl</b> To allow move outside task panel only
	//          * <b>xtpTaskItemAllowDragMove </b> To allow move operation
	//          * <b>xtpTaskItemAllowDragAll</b> To allow all drag operations
	// See Also:
	//     AllowDrop, XTPTaskPanelItemAllowDrag
	//-----------------------------------------------------------------------
	void AllowDrag(long nAllowDrag = xtpTaskItemAllowDragDefault);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether you can drop items within the
	//     TaskPanel from other objects to the TaskPanel.
	// Parameters:
	//     bAllowDrop - TRUE to allow task panel group items to be dropped
	//                  inside the task panel, FALSE to only allow items to be
	//                  dropped outside the task panel.
	// See Also:
	//     AllowDrag
	//-----------------------------------------------------------------------
	void AllowDrop(BOOL bAllowDrop);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to change the caption of a CXTPTaskPanelGroup
	//     or CXTPTaskPanelItem.  When the RenameItem method is called, an
	//     edit box appears on the group caption allowing you to type in a
	//     new name.
	// Parameters:
	//     pItem - TaskPanelGroup or TaskPanelItem who's caption will be renamed.
	// Remarks:
	//     OnEndLabelEdit is called when the user has finished editing the
	//     caption of the item.  This causes NotifyOwner to send a message
	//     to the parent that the caption is finished being edited.
	// See Also:
	//     OnEndLabelEdit
	//-----------------------------------------------------------------------
	void RenameItem(CXTPTaskPanelItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve a pointer to a group scroll button.
	// Parameters:
	//     bScrollUp - TRUE to retrieve the Up scroll button, FALSE to retrieve
	//                 the Down scroll button.
	// Returns:
	//     Pointer to the specified group scroll button.
	// See Also:
	//     CXTPTaskPanelScrollButton, m_pScrollButton
	//-----------------------------------------------------------------------
	CXTPTaskPanelScrollButton* GetScrollButton(BOOL bScrollUp);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method determines where a point lies in a specified group scroll
	//     button.
	// Parameters:
	//     point - CPoint object that specifies the x and y to be tested.
	// Returns:
	//     A pointer to a CXTPTaskPanelScrollButton item that occupies the
	//     specified point or 0 (zero) if no group scroll button occupies the point.
	//-----------------------------------------------------------------------
	CXTPTaskPanelScrollButton* HitTestScrollButton(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework to draw the task panel using the specified
	//     device context.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	// Remarks:
	//     OnDraw calls the CXTPTaskPanelGroup::OnPaint method for each group in the task panel.
	//     OnPaint also calls CXTPTaskPanelPaintManager::FillTaskPanel and
	//     CXTPTaskPanelPaintManager::DrawScrollButton
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC);


	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to notify the parent window that an event
	//     has occurred in the task panel.
	// Parameters:
	//     wParam -  Specifies which event has occurred.
	//     lParam - Additional message-specific information.
	// Remarks:
	//     wParam parameter can be one of the following values:
	//          * <b>XTP_TPN_CLICK</b> Indicates the user clicked a TaskPanel item.
	//          * <b>XTP_TPN_RCLICK</b> Indicates the user pressed the right mouse button on a TaskPanel item.
	//          * <b>XTP_TPN_STARTDRAG</b> Indicates the user started to dragging an item.
	//                                          You can return XTP_ACTION_CANCEL to cancel the drag.
	//          * <b>XTP_TPN_ENDLABELEDIT</b> Indicates the user starts to rename an item.
	//          * <b>XTP_TPN_ITEMDROP</b> Indicates the user dropped item in task panel.
	//          * <b>XTP_TPN_GROUPEXPANDING</b> Indicates the group is about to be expanded.
	//                                          <i>lParam</i> contains XTP_TPNGROUPEXPANDING pointer.
	//                                          You can return XTP_ACTION_CANCEL to ignore expanding.
	//          * <b>XTP_TPN_GROUPEXPANDED</b> Indicates the user has expanded a group.
	//     <p/>
	//     The following method in the task panel use NotifyOwner to send
	//     messages:
	//          * <b>CXTPTaskPanelGroup::OnAnimate</b>
	//          * <b>CXTPTaskPanel::ExpandGroup</b> lParam contains XTP_TPNGROUPEXPANDING pointer.
	//                                              You can return XTP_ACTION_CANCEL to ignore expanding.
	//          * <b>CXTPTaskPanel::OnDrop</b>  lParam contains item dropped.
	//          * <b>CXTPTaskPanel::OnStartItemDrag</b> lParam contains item dragged.
	//          * <b>CXTPTaskPanel::OnRButtonDown</b> lParam contains item right-clicked.
	//          * <b>CXTPTaskPanel::OnClick</b> lParam contains item clicked.
	//          * <b>CXTPTaskPanel::OnEndLabelEdit</b> lParam contains item who's caption is being renamed.
	// See Also:
	//     XTPWM_TASKPANEL_NOTIFY
	//-----------------------------------------------------------------------
	virtual LRESULT NotifyOwner(WPARAM wParam, LPARAM lParam);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the Explorer behavior
	//     is currently being used.
	// Returns:
	//     TRUE if the Explorer behavior is used, FALSE if a different
	//     behavior is used.
	// See Also:
	//     SetBehaviour, GetBehaviour, XTPTaskPanelBehaviour
	//-----------------------------------------------------------------------
	BOOL IsExplorerBehaviour();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when a group item is just starting to be
	//     dragged.
	// Parameters:
	//     pItem - Group item being dragged.
	// Remarks:
	//     OnMouseMove calls OnStartItemDrag is a group item is dragged.
	// See Also:
	//     OnStartGroupDrag, OnMouseMove
	//-----------------------------------------------------------------------
	virtual void OnStartItemDrag(CXTPTaskPanelGroupItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when a group is just starting to be
	//     dragged.
	// Parameters:
	//     pItem - Group being dragged.
	// Remarks:
	//     OnMouseMove calls OnStartGroupDrag is a group is dragged.
	// See Also:
	//     OnStartItemDrag, OnMouseMove
	//-----------------------------------------------------------------------
	virtual void OnStartGroupDrag(CXTPTaskPanelItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the caption of an item is finished
	//     being edited\renamed.
	// Parameters:
	//     pItem - Item being edited.
	//     str   - New caption of item.
	// Remarks:
	//     This member is called after RenameItem.
	// See Also:
	//     RenameItem
	//-----------------------------------------------------------------------
	virtual void OnEndLabelEdit(CXTPTaskPanelItem* pItem, LPCTSTR str);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when an item is dragged over the task panel.
	// Parameters:
	//     pDataObject - Points to the COleDataObject being dragged over the
	//                   drop target.
	//     dwKeyState  - State of keys on keyboard.  Contains the state of the
	//                   modifier keys. This is a combination of any number of
	//                   the following: MK_CONTROL, MK_SHIFT, MK_ALT, MK_LBUTTON,
	//                   MK_MBUTTON, and MK_RBUTTON.
	//     point - The current mouse position relative to the task panel.
	// Returns:
	//     A value from the DROPEFFECT enumerated type, which indicates the type
	//     of drop that would occur if the user dropped the object at this
	//     position. The type of drop often depends on the current key state as
	//     indicated by dwKeyState. A standard mapping of keystates to DROPEFFECT
	//     values is:
	//       * <b>DROPEFFECT_NONE</b> The data object cannot be dropped in this
	//                                window.
	//       * <b>DROPEFFECT_COPY</b> for <b>MK_CONTROL</b>  Creates a copy of
	//                                                       the dropped object.
	//       * <b>DROPEFFECT_MOVE</b> for <b>MK_ALT</b> Creates a copy of the dropped
	//                                                  object and delete the original object.
	//                                                  This is typically the default drop effect,
	//                                                  when the view can accept the data object.
	//-----------------------------------------------------------------------
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when an item has been dropped into the task panel.
	// Parameters:
	//     pDataObject - Points to the COleDataObject that is dropped into the drop target.
	//     dropEffect  - The drop effect that the user has requested. Can be any of the values listed in the Remarks section.
	//     point - The current mouse position relative to the task panel.
	// Remarks:
	//     The <i>dropEffect,/i> parameter can be one of the following values:
	//     * <b>DROPEFFECT_COPY</b> Creates a copy of the data object being dropped.
	//     * <b>DROPEFFECT_MOVE</b> Moves the data object to the current mouse location.
	// Returns:
	//     TRUE if the drop was successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called to reposition the group scroll buttons.
	// Remarks:
	//     Reposition and AnimateGroups call RepositionScrollButtons when the
	//     groups in the task panel need to be repositioned.  I.e. During animation
	//     while the group is expanding or collapsing.
	//-------------------------------------------------------------------------
	void RepositionScrollButtons();

	//-----------------------------------------------------------------------
	// Summary:
	//     The member is called when a group scroll button is clicked.
	// Parameters:
	//     pScrollButton - Pointer to the group scroll button that
	//                     was clicked.
	// Remarks:
	//     OnLButtonDown calls OnClickScrollButton when a group scroll button
	//     is clicked.
	// See Also:
	//     CXTPTaskPanelScrollButton, m_pScrollButton
	//-----------------------------------------------------------------------
	void OnClickScrollButton(CXTPTaskPanelScrollButton* pScrollButton);

	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called to update the enabled\disabled state of the
	//     group scroll buttons.
	// Remarks:
	//     RepositionScrollButtons, CXTPTaskPanelGroup::Scroll, and
	//     CXTPTaskPanelGroup::EnsureVisible call UpdateScrollButtons to update
	//     the state of the group scroll buttons.
	//-------------------------------------------------------------------------
	void UpdateScrollButtons();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to determine if the mouse cursor is positioned
	//     over one of the group scroll buttons.
	// Parameters:
	//     point - CPoint object that specifies the x and y to be tested.
	// Remarks:
	//     If the mouse cursor is over a group scroll button, then the
	//     buttons appearance is changed to "hot" is the button is enabled.
	//     When the mouse cursor leaves the button, the appearance is
	//     returned to normal.
	//
	//     OnMouseMove and OnMouseLeave call CheckScrollButtonMouseOver to
	//     update the appearance of the group scroll buttons.
	//-----------------------------------------------------------------------
	void CheckScrollButtonMouseOver(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when an item has been removed from the task panel.
	// Parameters:
	//     pItem - Item that was removed.
	// Remarks:
	//     CXTPTaskPanelItems::Clear and CXTPTaskPanelItems::RemoveAt call
	//     OnItemRemoved.
	// See Also:
	//     CXTPTaskPanelItems::Clear, CXTPTaskPanelItems::RemoveAt
	//-----------------------------------------------------------------------
	virtual void OnItemRemoved(CXTPTaskPanelItem* pItem);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to update the task panel's scroll bar.
	// Remarks:
	//     This updates the scroll bar that is displayed to scroll the entire
	//     task panel.  This does not update the scroll bar placed inside
	//     a task panel group, they are updated with UpdateScrollButtons.
	//-------------------------------------------------------------------------
	virtual void UpdateScrollBar();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPTaskPanel)
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPTaskPanel)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSysColorChange();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseLeave();
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg LRESULT OnPrint(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL GetMessageExt(MSG& msg);
	void EnsureVisibleRect(const CRect& rc);

protected:
	CXTPTaskPanelGroups* m_pGroups;                 // Collection of groups.

	CXTPTaskPanelPaintManager* m_pPaintManager;     // Current Paint Manager.
	XTPTaskPanelPaintTheme m_paintTheme;            // Current theme.

	BOOL m_nLockRedraw;                             // TRUE when control is locked.

	int m_nClientHeight;                            // Group client height.  This is the area displayed under the group caption.

	CRect m_rcMargins;                              // Outer margins of groups.
	int m_nGroupSpacing;                            // Amount of space placed between groups.

	BOOL m_bScrollVisible;                          // TRUE if scroll bar is visible. This refers to the scroll bar of the entire task panel, the the scroll bar uses to scroll group items or the group caption.

	CXTPTaskPanelItem* m_pItemHot;                  // Currently hot item.
	CXTPTaskPanelItem* m_pItemFocused;              // Currently focused item.
	CXTPTaskPanelItem* m_pItemPressed;              // Currently pressed item.

	CXTPTaskPanelItem* m_pItemDragging;             // Pointer to item currently being dragged.
	CXTPTaskPanelItem* m_pItemDragOver;             // Pointer to item under mouse cursor with dragged icon.  This is the item that will appear directly under the dragged item if the item were dropped.

	BOOL m_bAnimation;                              // TRUE if animation enabled.
	XTPTaskPanelAnimation m_eAnimation;             // Current animation state.

	HCURSOR m_hHandCursor;                          // Hand cursor.

	UINT_PTR m_nAnimateTimer;                       // Timer identifier for animation.
	int m_nAnimationStep;                           // Current step of animation.

	CXTPImageManager* m_pImageManager;              // Image manager of task panel.  This is the collection of images that are used for task panel group and group item icons.

	BOOL m_bExpandable;                             // TRUE if groups are expandable.

	BOOL m_bAcceptFocus;                            // TRUE if control accept focus.
	BOOL m_bDrawFocusRect;                          // TRUE to draw focused rectangle drawn around the item when the Up and Down arrows are used to navigate the items in the TaskPanel.
	CSize m_szItemIcon;                             // Size of group item icons.
	CSize m_szGroupIcon;                            // Size of icons displayed in group captions.
	BOOL m_bSelectItemOnFocus;                      // TRUE if items receive focus when they are clicked.

	CPoint m_ptPressed;                             // Point on task panel that was clicked.

	long m_nAllowDrag;                              // TRUE to allow task panel group items to be dragged.
	BOOL m_bAllowDrop;                              // TRUE to allow task panel group items to be dropped inside the task panel, FALSE to only allow items to be dropped outside the task panel.

	XTPTaskPanelItemLayout m_itemLayout;            // Currently set item layout.
	XTPTaskPanelHotTrackStyle m_hotTrackStyle;      // Currently set hot tracking style.
	XTPTaskPanelBehaviour m_panelBehaviour;         // Currently set TaskPanel behavior.

	CXTPTaskPanelGroup* m_pActiveGroup;             // Pointer to currently active group.
	CPanelDropTarget* m_pDropTarget;                // Internal drag'n'drop helper.
	static CLIPFORMAT m_cfItem;                     // Clipboard format.

	BOOL m_bTimerGroupHover;                        // TRUE if Timer for Hover event is currently set.  The timer for the Hover event is set when the mouse is left in the same position for at least 500 milliseconds.

	CXTPTaskPanelScrollButton* m_pScrollButton[2];  // Scroll buttons of the task panel.  The task panel uses these scroll button in CXTPTaskPanelGroup objects when there are more items in the group than can be viewed.
	                                                // Only the necessary buttons are displayed.  If the items in the group can not be scrolled anymore, then that button is removed.  Scroll buttons are only visible
	                                                // when using the xtpTaskPanelBehaviourList and xtpTaskPanelBehaviourToolbox task panel behaviors.
	BOOL m_bSingleSelection;                        // TRUE to use focused item as selected.

private:


	friend class CXTPTaskPanelGroup;
	friend class CXTPTaskPanelItem;
	friend class CTaskPanelCtrl;
	friend class CXTPTaskPanelPaintManager;
	friend class CPanelDropTarget;
	friend class CXTPTaskPanelGroupItem;
	friend class CXTPTaskPanelEditItem;
	friend class CXTPTaskPanelItems;
};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPTaskPanelItems* CXTPTaskPanel::GetGroups() {
	return (CXTPTaskPanelItems*)m_pGroups;
}
AFX_INLINE CXTPTaskPanelPaintManager* CXTPTaskPanel::GetPaintManager() {
	return m_pPaintManager;
}
AFX_INLINE CXTPTaskPanelItem* CXTPTaskPanel::GetHotItem() {
	return m_pItemHot;
}
AFX_INLINE XTPTaskPanelPaintTheme CXTPTaskPanel::GetCurrentTheme() {
	return m_paintTheme;
}
AFX_INLINE void CXTPTaskPanel::SetExpandable(BOOL bExpandable) {
	m_bExpandable = bExpandable;
}
AFX_INLINE XTPTaskPanelAnimation CXTPTaskPanel::GetAnimation() {
	return m_eAnimation;
}
AFX_INLINE BOOL CXTPTaskPanel::IsExpandable() {
	return m_bExpandable;
}
AFX_INLINE CSize CXTPTaskPanel::GetItemIconSize() {
	return m_szItemIcon;
}
AFX_INLINE CSize CXTPTaskPanel::GetGroupIconSize() {
	return m_szGroupIcon;
}
AFX_INLINE BOOL CXTPTaskPanel::IsExplorerBehaviour() {
	return m_panelBehaviour == xtpTaskPanelBehaviourExplorer;
}
AFX_INLINE CXTPTaskPanelGroup* CXTPTaskPanel::GetActiveGroup() {
	return m_pActiveGroup;
}
AFX_INLINE void CXTPTaskPanel::SetSelectItemOnFocus(BOOL bSelect) {
	m_bSelectItemOnFocus = bSelect;
}
AFX_INLINE void CXTPTaskPanel::DrawFocusRect(BOOL bAccept) {
	m_bAcceptFocus = bAccept;
}
AFX_INLINE CRect CXTPTaskPanel::GetMargins() {
	return m_rcMargins;
}
AFX_INLINE void CXTPTaskPanel::SetIconSize(CSize szIcon) {
	m_szItemIcon = szIcon;
	Reposition();
}
AFX_INLINE CLIPFORMAT AFX_CDECL CXTPTaskPanel::GetClipboardFormat() {
	return m_cfItem;
}
AFX_INLINE void CXTPTaskPanel::AllowDrop(BOOL bAllowDrop) {
	m_bAllowDrop = bAllowDrop;
}
AFX_INLINE CXTPTaskPanelScrollButton* CXTPTaskPanel::GetScrollButton(BOOL bScrollUp) {
	return bScrollUp? m_pScrollButton[TRUE]: m_pScrollButton[FALSE];
}
AFX_INLINE BOOL CXTPTaskPanel::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE void CXTPTaskPanel::SetSingleSelection(BOOL bSingleSelection) {
	m_bSingleSelection = bSingleSelection;
	Reposition();
}
AFX_INLINE BOOL CXTPTaskPanel::IsSingleSelection() {
	return m_bSingleSelection;
}

#endif // !defined(__XTPTASKPANEL_H__)
