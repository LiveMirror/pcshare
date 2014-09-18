// XTPTaskPanelDefines.h
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
#if !defined(__XTPTASKPANELDEFINES_H__)
#define __XTPTASKPANELDEFINES_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPTaskPanelGroup;

//-----------------------------------------------------------------------
// Summary:
//     This structure passed as parameter when XTP_TPN_GROUPEXPANDING event occur.
//-----------------------------------------------------------------------
struct XTP_TPNGROUPEXPANDING
{
	CXTPTaskPanelGroup* pGroup;  // Group that is about to be expanded.
	BOOL bExpanding;             // TRUE if group must be expanded.
};


//-----------------------------------------------------------------------
// Summary:
//     XTPTaskPanelItemType is an enumeration used by CXTPTaskPanelItem to
//     specify the type of the item.
// Example:
// <code>
// CXTPTaskPanelGroup* pGroupSystem = m_wndTaskPanel.AddGroup(ID_TASKGROUP_SYSTEM);
// pGroupSystem->AddLinkItem(ID_TASKITEM_HIDECONTENTS, 0);
// ASSERT(pGroupSystem->GetType() == xtpTaskItemTypeLink);
// </code>
// See Also: CXTPTaskPanelItem::SetType, CXTPTaskPanelItem::GetType
//
// <KEYWORDS xtpTaskItemTypeGroup, xtpTaskItemTypeLink, xtpTaskItemTypeText, xtpTaskItemTypeControl>
//-----------------------------------------------------------------------
enum XTPTaskPanelItemType
{
	xtpTaskItemTypeGroup,       // Item is group.
	xtpTaskItemTypeLink,        // Item is link.
	xtpTaskItemTypeText,        // Item is text.
	xtpTaskItemTypeControl      // Item is control.
};


//-----------------------------------------------------------------------
// Summary:
//     XTPTaskPanelItemAllowDrag is an enumeration used by CXTPTaskPanel to
//     specify the drag options
// Example:
// <code>
// m_wndTaskPanel.AllowDrag(xtpTaskItemAllowDragCopyWithinGroup);
// </code>
// See Also: CXTPTaskPanel::AllowDrag
// 
// <KEYWORDS xtpTaskItemAllowDragNone, xtpTaskItemAllowDragDefault, xtpTaskItemAllowDragCopy, xtpTaskItemAllowDragMove, xtpTaskItemAllowDragAll>
//-----------------------------------------------------------------------
enum XTPTaskPanelItemAllowDrag
{ 
	xtpTaskItemAllowDragNone                 = 0x00,            // Disable drag
	xtpTaskItemAllowDragDefault              = 0x01,            // Default drag options (xtpTaskItemAllowDragAll)
	xtpTaskItemAllowDragCopyWithinGroup      = 0x02,            // Allow copy within group only
	xtpTaskItemAllowDragCopyWithinControl    = 0x04,            // Allow copy within task panel only
	xtpTaskItemAllowDragCopyOutsideControl   = 0x08,            // Allow copy outside task panel only
	xtpTaskItemAllowDragCopy                 = 0x04 + 0x08,     // Allow copy operation
	xtpTaskItemAllowDragMoveWithinGroup      = 0x20,            // Allow move within group only
	xtpTaskItemAllowDragMoveWithinControl    = 0x40,            // Allow move within task panel only
	xtpTaskItemAllowDragMoveOutsideControl   = 0x80,            // Allow move outside task panel only
	xtpTaskItemAllowDragMove                 = 0x40 + 0x80,     // Allow move operation
	xtpTaskItemAllowDragWithinGroup          = 0x02 + 0x20,     // Allow drag within group only
	xtpTaskItemAllowDragWithinControl        = 0x04 + 0x40,     // Allow drag within task panel only
	xtpTaskItemAllowDrag                     = 0x04 + 0x08 + 0x40 + 0x80    // Allow all drag operations
};

//-----------------------------------------------------------------------
// Summary:
//     XTPTaskPanelItemLayout is an enumeration used by CXTPTaskpanel and
//     CXTPTaskPanelGroup to set how the items are laid out in the group(s).
//     Each group can have a different layout or they can all be the same.
// Example:
//     <code>pGroup->SetItemLayout(xtpTaskItemLayoutImagesWithText);</code>
// See Also:
//     CXTPTaskPanel::SetItemLayout, CXTPTaskPanel::GetItemLayout,
//     CXTPTaskPanelGroup::SetItemLayout, CXTPTaskPanelGroup::GetItemLayout
//
// <KEYWORDS xtpTaskItemLayoutDefault, xtpTaskItemLayoutImages, xtpTaskItemLayoutImagesWithText, xtpTaskItemLayoutImagesWithTextBelow>
//-----------------------------------------------------------------------
enum XTPTaskPanelItemLayout
{
	xtpTaskItemLayoutDefault,           // Default Layout.
	xtpTaskItemLayoutImages,            // Only images are shown.
	xtpTaskItemLayoutImagesWithText,    // Both images and text are shown.
	xtpTaskItemLayoutImagesWithTextBelow// Images with the text shown below the image.
};


//-----------------------------------------------------------------------
// Summary:
//     XTPTaskPanelHotTrackStyle is an enumeration used by CXTPTaskPanel
//     to set the hot tracking style used for CXTPTaskPanelItems.  Hot tracking is
//     how the items look as you move the mouse pointer over them.
// Example:
//     <code>m_wndTaskPanel.SetHotTrackStyle(xtpTaskPanelHighlightItem);</code>
// See Also:
//     CXTPTaskPanel::SetHotTrackStyle, CXTPTaskPanel::GetHotTrackStyle, CXTPTaskPanelItem::IsItemHot
//
// <KEYWORDS xtpTaskPanelHighlightDefault, xtpTaskPanelHighlightText, xtpTaskPanelHighlightImage, xtpTaskPanelHighlightItem, xtpTaskPanelHighlightNone>
//-----------------------------------------------------------------------
enum XTPTaskPanelHotTrackStyle
{
	xtpTaskPanelHighlightDefault,       // Uses the default hot tracking style.
	xtpTaskPanelHighlightText,          // Only text is highlighted on mouse over
	xtpTaskPanelHighlightImage,         // Only Images are highlighted on mouse over.
	xtpTaskPanelHighlightItem,          // The entire item is highlighted on mouse over.
	xtpTaskPanelHighlightNone           // No hot tracking is used, nothing will happen on mouse over.
};


//-----------------------------------------------------------------------
// Summary:
//     XTPTaskPanelBehaviour is an enumeration used by CXTPTaskPanel and
//     CXTPTaskPanelGroup to set the behavior of the groups in the TaskPanel.
// Example:
//     <code>m_wndTaskPanel.SetBehaviour(xtpTaskPanelBehaviourToolbox);</code>
// See Also:
//     CXTPTaskPanel::SetBehaviour, CXTPTaskPanelGroup::GetBehaviour.
//
// <KEYWORDS xtpTaskPanelBehaviourExplorer, xtpTaskPanelBehaviourList, xtpTaskPanelBehaviourToolbox>
//-----------------------------------------------------------------------
enum XTPTaskPanelBehaviour
{
	xtpTaskPanelBehaviourExplorer,      // TaskPanel will behave like Windows Explorer.
	xtpTaskPanelBehaviourList,          // TaskPanel will behave like Windows List Box.
	xtpTaskPanelBehaviourToolbox        // TaskPanel will behave like VS. NET Toolbox.
};

//-----------------------------------------------------------------------
// Summary:
//     XTPTaskPanelPaintTheme is an enumeration used by CXTPTaskPanel to set
//     the theme of the control.
// Example:
//     <code>m_wndTaskPanel.SetTheme(xtpTaskPanelThemeOffice2000Plain);</code>
// See Also:
//     CXTPTaskPanel::SetTheme, CXTPTaskPanel::GetCurrentTheme
//
// <KEYWORDS xtpTaskPanelThemeOffice2000, xtpTaskPanelThemeOffice2003, xtpTaskPanelThemeNativeWinXP, xtpTaskPanelThemeOffice2000Plain, xtpTaskPanelThemeOfficeXPPlain, xtpTaskPanelThemeOffice2003Plain, xtpTaskPanelThemeNativeWinXPPlain, xtpTaskPanelThemeToolbox, xtpTaskPanelThemeToolboxWhidbey, xtpTaskPanelThemeListView, xtpTaskPanelThemeListViewOfficeXP, xtpTaskPanelThemeListViewOffice2003, xtpTaskPanelThemeShortcutBarOffice2003, xtpTaskPanelThemeCustom>
//-----------------------------------------------------------------------
enum XTPTaskPanelPaintTheme
{
	xtpTaskPanelThemeOffice2000,            // Office 2000 theme
	xtpTaskPanelThemeOffice2003,            // Office 2003 theme
	xtpTaskPanelThemeNativeWinXP,           // Win XP theme
	xtpTaskPanelThemeOffice2000Plain,       // Office 2000 classic theme
	xtpTaskPanelThemeOfficeXPPlain,         // Office XP classic theme
	xtpTaskPanelThemeOffice2003Plain,       // Office 2003 classic theme
	xtpTaskPanelThemeNativeWinXPPlain,      // Win XP classic theme
	xtpTaskPanelThemeToolbox,               // VS.NET style Toolbox theme.
	xtpTaskPanelThemeToolboxWhidbey,        // Whidbey VS 2005 style Toolbox theme.
	xtpTaskPanelThemeListView,              // Standard List View theme.
	xtpTaskPanelThemeListViewOfficeXP,      // Office XP List View theme.
	xtpTaskPanelThemeListViewOffice2003,    // Office 2003 List View theme.
	xtpTaskPanelThemeShortcutBarOffice2003, // ShortcutBar Office 2003 theme.
	xtpTaskPanelThemeCustom                 // User defined Custom theme is used.
};


//-----------------------------------------------------------------------
// Summary:
//     XTPTaskPanelAnimation is an enumeration used by CXTPTaskPanel to set
//     the animation of the control.
// Example:
//     <code>m_wndTaskPanel.SetAnimation(xtpTaskPanelAnimationYes);</code>
// See Also: CXTPTaskPanel::SetAnimation, CXTPTaskPanel::GetAnimation
//
// <KEYWORDS xtpTaskPanelAnimationYes, xtpTaskPanelAnimationNo, xtpTaskPanelAnimationSystem>
//-----------------------------------------------------------------------
enum XTPTaskPanelAnimation
{
	xtpTaskPanelAnimationYes,               // Enable animation
	xtpTaskPanelAnimationNo,                // Disable animation
	xtpTaskPanelAnimationSystem             // Use system settings to determine animation
};


//-----------------------------------------------------------------------
// Summary:
//     The XTPWM_TASKPANEL_NOTIFY message is sent to the CXTPTaskPanel owner window
//     whenever an action occurs within the CXTPTaskPanel
// Parameters:
//     nAction -  Value of wParam specifies a TaskPanel value that indicates the user's
//                request.
//     pItem   -  The value of lParam points to an CXTPTaskPanelItem object that contains information for the
//                specified item. This pointer should <b>never</b> be NULL.
// Remarks:
//     <code>XTPWM_SHORTCUTBAR_NOTIFY
//     nAction = (int) wParam;             // TaskPanel action
//     pItem = (CXTPTaskPanelItem*) lParam;  // pointer to an CXTPTaskPanelItem object</code>
//     <p/>
//
//     nAction parameter can be one of the following values:
//     * <b>XTP_TPN_CLICK</b> Indicates the user click TaskPanel item.
//     * <b>XTP_TPN_RCLICK</b> Indicates the user presses the right mouse button on the TaskPanel item.
//     * <b>XTP_TPN_STARTDRAG</b> Indicates the user start dragging item.
//     * <b>XTP_TPN_ENDLABELEDIT</b> Indicates the user apply item rename.
//     * <b>XTP_TPN_ITEMDROP</b> Indicates the user dropped item to task panel
//     * <b>XTP_TPN_GROUPEXPANDING</b> Indicates the group is about to be expanded.
//                                       lParam contains XTP_TPNGROUPEXPANDING pointer.
//                                       You can return XTP_ACTION_CANCEL to ignore expanding.
//     * <b>XTP_TPN_GROUPEXPANDED</b> Indicates the user expand group.
//     * <b>XTP_TPN_DBLCLICK</b> Indicates the user double-clicks TaskPanel item.
//     * <b>XTP_TPN_FOCUSEDITEMCHANGED</b> Focused item of control changed.
// Returns:
//     If the application is to process this message, the return value should be TRUE, otherwise the
//     return value is FALSE.
// Example:
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_MESSAGE(XTPWM_TASKPANEL_NOTIFY, OnTaskPanelNotify)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CMainFrame::OnTaskPanelNotify(WPARAM wParam, LPARAM lParam)
// {
//      switch(wParam)
//      {
//          case XTP_TPN_CLICK:
//              {
//                  CXTPTaskPanelGroupItem* pItem = (CXTPTaskPanelGroupItem*)lParam;
//                  TRACE(_T("Click Event: pItem.Caption = %s, pItem.ID = %i\n"), pItem->GetCaption(), pItem->GetID());
//
//                  if (IsToggleButtons())
//                  {
//                      pItem->SetItemSelected(!pItem->IsItemSelected());
//                  }
//              }
//              break;
//
//          case XTP_TPN_RCLICK:
//
//              OnTaskPanelRButtonDown((CXTPTaskPanelItem*)lParam);
//              return TRUE;
//
//      }
//      return 0;
// }
// </code>
// See Also:
//     CXTPTaskPanel::NotifyOwner
//-----------------------------------------------------------------------
const UINT XTPWM_TASKPANEL_NOTIFY = (WM_APP + 2584);

const UINT XTP_TPN_CLICK = 1; //<ALIAS XTPWM_TASKPANEL_NOTIFY>
const UINT XTP_TPN_RCLICK = 2; //<ALIAS XTPWM_TASKPANEL_NOTIFY>
const UINT XTP_TPN_STARTDRAG = 3; //<ALIAS XTPWM_TASKPANEL_NOTIFY>
const UINT XTP_TPN_ENDLABELEDIT = 4; //<ALIAS XTPWM_TASKPANEL_NOTIFY>
const UINT XTP_TPN_ITEMDROP = 5; //<ALIAS XTPWM_TASKPANEL_NOTIFY>
const UINT XTP_TPN_GROUPEXPANDING = 6; //<ALIAS XTPWM_TASKPANEL_NOTIFY>
const UINT XTP_TPN_GROUPEXPANDED = 7; //<ALIAS XTPWM_TASKPANEL_NOTIFY>
const UINT XTP_TPN_DBLCLICK = 8; //<ALIAS XTPWM_TASKPANEL_NOTIFY>
const UINT XTP_TPN_FOCUSEDITEMCHANGED = 9; //<ALIAS XTPWM_TASKPANEL_NOTIFY>
const int XTP_ACTION_CANCEL = -1; //<ALIAS XTPWM_TASKPANEL_NOTIFY>


#endif // !defined(__XTPTASKPANELDEFINES_H__)
