// XTPDockingPaneManager.h : interface for the CXTPDockingPaneManager class.
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
#if !defined(__XTPDOCKINGPANEMANAGER_H__)
#define __XTPDOCKINGPANEMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPDockingPaneDefines.h"
#include "CommonPro.include/XTPImageManager.h"

class CXTPDockingPane;
class CXTPDockingPaneBase;
class CXTPDockingPaneBase;
class CXTPDockingPaneManager;
class CXTPDockingPaneSplitterContainer;
class CXTPDockingPaneTabbedContainer;
class CXTPDockingPaneMiniWnd;
class CXTPDockingPaneContext;
class CXTPDockingPaneLayout;
class CXTPDockingPanePaintManager;
class CXTPImageManagerIcon;
class CXTPImageManager;

//===========================================================================
// Summary:
//     CXTPDockingPaneManager is a CWnd derived class. It is used to manipulate
//     docking panes.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneManager : public CWnd
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneManager object
	//-----------------------------------------------------------------------
	CXTPDockingPaneManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneManager object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	~CXTPDockingPaneManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to install docking panes.
	// Parameters:
	//     pParent - Parent frame of the panes.
	// Remarks:
	//     You must call it before any other members.
	//-----------------------------------------------------------------------
	void InstallDockingPanes(CFrameWnd* pParent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to create a docking pane.
	// Parameters:
	//     nID        - Unique identifier of the pane.
	//     rc         - Initial size of the pane.
	//     direction  - Docking direction. Can be any of the values listed in the Remarks section.
	//     pNeighbour - Pane's Neighbor. It can be NULL to use the frame as a neighbor.
	// Remarks:
	//     direction parameter can be one of the following:
	//     * <b>xtpPaneDockTop</b> Docks the pane to the top of the neighbor.
	//     * <b>xtpPaneDockLeft</b> Docks the pane to the left of the neighbor.
	//     * <b>xtpPaneDockRight</b> Docks the pane to the right of the neighbor.
	//     * <b>xtpPaneDockBottom</b> Docks the pane to the bottom of the neighbor.
	// Returns:
	//     The pointer to the created pane.
	//-----------------------------------------------------------------------
	CXTPDockingPane* CreatePane(UINT nID, CRect rc, XTPDockingPaneDirection direction, CXTPDockingPaneBase* pNeighbour = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to dock an existing pane.
	// Parameters:
	//     pPane      - Pane to be docked.
	//     direction  - Docking direction. Can be any of the values listed in the Remarks section.
	//     pNeighbour - Pane's Neighbor.
	// Remarks:
	//     direction parameter can be one of the following:
	//     * <b>xtpPaneDockTop</b> Docks the pane to the top of the neighbor.
	//     * <b>xtpPaneDockLeft</b> Docks the pane to the left of the neighbor.
	//     * <b>xtpPaneDockRight</b> Docks the pane to the right of the neighbor.
	//     * <b>xtpPaneDockBottom</b> Docks the pane to the bottom of the neighbor.
	//-----------------------------------------------------------------------
	void DockPane(CXTPDockingPaneBase* pPane, XTPDockingPaneDirection direction, CXTPDockingPaneBase* pNeighbour = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to float an existing pane.
	// Parameters:
	//     pPane - Pane to be floated.
	//     rc    - Floating rectangle.
	//-----------------------------------------------------------------------
	void FloatPane(CXTPDockingPaneBase* pPane, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to attach a pane to another one.
	// Parameters:
	//     pPane      - Pane to be attached.
	//     pNeighbour - Pane's Neighbor.
	//-----------------------------------------------------------------------
	void AttachPane(CXTPDockingPaneBase* pPane, CXTPDockingPaneBase* pNeighbour);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to switch the pane's position (docking/floating).
	// Parameters:
	//     pPane - Docking Pane.
	//-----------------------------------------------------------------------
	void ToggleDocking(CXTPDockingPaneBase* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to show (activate) a pane
	// Parameters:
	//     nID - Pane's identifier.
	//     pPane - Pane need to show
	//-----------------------------------------------------------------------
	void ShowPane(int nID);
	void ShowPane(CXTPDockingPane* pPane); // <COMBINE CXTPDockingPaneManager::ShowPane@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to close a pane
	// Parameters:
	//     nID - Pane's identifier.
	//     pPane - Pane need to close
	//-----------------------------------------------------------------------
	void ClosePane(int nID);
	void ClosePane(CXTPDockingPane* pPane); // <COMBINE CXTPDockingPaneManager::ClosePane@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to destroy a pane
	// Parameters:
	//     nID - Pane's identifier.
	//     pPane - Pane need to destroy
	// See Also: ClosePane
	//-----------------------------------------------------------------------
	void DestroyPane(int nID);
	void DestroyPane(CXTPDockingPane* pPane); // <COMBINE CXTPDockingPaneManager::DestroyPane@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to hide a pane.
	// Parameters:
	//     nID - Pane's identifier.
	//     pPane - Pane need to hide
	//-----------------------------------------------------------------------
	void HidePane(int nID);
	void HidePane(CXTPDockingPaneBase* pPane);// <COMBINE CXTPDockingPaneManager::HidePane@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to find a pane by its identifier.
	// Parameters:
	//     nID - Pane's identifier.
	// Returns:
	//     A pointer to a CXTPDockingPane object.
	//-----------------------------------------------------------------------
	CXTPDockingPane* FindPane(int nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to check if the pane is closed.
	// Parameters:
	//     nID - Pane's identifier.
	//     pPane - Pane need to close.
	// Returns:
	//     TRUE if the pane is closed.
	//-----------------------------------------------------------------------
	BOOL IsPaneClosed(int nID);
	BOOL IsPaneClosed(CXTPDockingPane* pPane); // <COMBINE CXTPDockingPaneManager::IsPaneClosed@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to check if the pane is hidden.
	// Parameters:
	//     nID - Pane's identifier.
	//     pPane - Points to a CXTPDockingPane object.
	// Returns:
	//     TRUE if the pane is hidden.
	//-----------------------------------------------------------------------
	BOOL IsPaneHidden(int nID);
	BOOL IsPaneHidden(CXTPDockingPane* pPane); // <COMBINE CXTPDockingPaneManager::IsPaneHidden@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to check if the pane is selected.
	// Parameters:
	//     nID - Pane's identifier.
	//     pPane - Points to a CXTPDockingPane object.
	// Returns:
	//     TRUE if the pane is selected.
	//-----------------------------------------------------------------------
	BOOL IsPaneSelected(int nID);
	BOOL IsPaneSelected(CXTPDockingPane* pPane); // <COMBINE CXTPDockingPaneManager::IsPaneSelected@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set icons to panes.
	// Parameters:
	//     nIDResource - Resource Identifier.
	//     nIDs        - Pointer to an array of pane Ids.
	//     nCount      - Number of elements in the array pointed to by nIDs.
	//     clrMask     - RGB value of transparent color.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL SetIcons(UINT nIDResource, const int* nIDs, int nCount, COLORREF clrMask = 0xC0C0C0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set an icon to the pane.
	// Parameters:
	//     nID   - Docking pane's identifier.
	//     hIcon - Icon handle.
	//-----------------------------------------------------------------------
	void SetIcon(UINT nID, CXTPImageManagerIconHandle hIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get an icon of the pane.
	// Parameters:
	//     nID - Docking pane's identifier.
	//     nWidth - Width of icon to retrieve.
	// Returns:
	//     The docking pane's icon.
	//-----------------------------------------------------------------------
	CXTPImageManagerIcon* GetIcon(UINT nID, int nWidth = 16);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove all icons.
	//-----------------------------------------------------------------------
	void ClearIconMap();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the speed of animation.
	// Parameters:
	//     dAnimationDelay    - Must be -1 to use nAnimationDuration and  nAnimationInterval.
	//     nAnimationDuration - Total time of animation, in milliseconds
	//     nAnimationInterval - Amount of time to rest, in milliseconds, between
	//                          each step.
	// Remarks:
	//     To disable animation set nAnimationDuration = 0.
	// See Also: GetAnimationDelay
	//-----------------------------------------------------------------------
	void SetAnimationDelay(double dAnimationDelay = -1, int nAnimationDuration = 128, int nAnimationInterval = 16);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the animation speed.
	// Parameters:
	//     pAnimationDuration - Pointer to receive total time of animation, in milliseconds.
	//     pAnimationInterval - Pointer to receive amount of time to rest, in milliseconds, between
	//                          each step.
	// Returns:
	//     Animation delay.
	// See Also: SetAnimationDelay
	//-----------------------------------------------------------------------
	double GetAnimationDelay(int* pAnimationDuration = NULL, int* pAnimationInterval = NULL);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the list of the created panes.
	// Returns:
	//     A list of created panes.
	// See Also:
	//     GetPaneStack, CXTPDockingPaneInfoList
	//-----------------------------------------------------------------------
	CXTPDockingPaneInfoList& GetPaneList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the list of the created panes.including virtual containers
	// Returns:
	//     A list of created panes with its containers.
	// See Also: GetPaneList
	//-----------------------------------------------------------------------
	CXTPDockingPaneBaseList& GetPaneStack();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to return the docking site of the manager.
	// Returns:
	//     The docking site.
	//-----------------------------------------------------------------------
	CFrameWnd* GetSite();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to switch the visual theme of the panes.
	// Parameters:
	//     theme - New visual theme. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     theme can be one of the following:
	//     * <b>xtpPaneThemeDefault</b> Enables the default theme.
	//     * <b>xtpPaneThemeOffice</b> Enables Visual Studio .NET style theme.
	//     * <b>xtpPaneThemeGrippered</b> Enables Visual Studio 6 style theme.
	//     * <b>xtpPaneThemeVisio</b> Enables Visio style theme.
	//     * <b>xtpPaneThemeOffice2003</b> Enables Office 2003 style theme.
	//     * <b>xtpPaneThemeNativeWinXP</b> Enables XP Theme.
	//     * <b>xtpPaneThemeWhidbey</b> Enables Whidbey theme.
	//-----------------------------------------------------------------------
	void SetTheme(XTPDockingPanePaintTheme theme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set a custom theme. See the DockingPane sample
	//     as a sample of creating a new theme.
	// Parameters:
	//     pTheme - New Theme.
	//-----------------------------------------------------------------------
	void SetCustomTheme(CXTPDockingPanePaintManager* pTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the current visual theme.
	// Returns:
	//     The current theme.
	//-----------------------------------------------------------------------
	XTPDockingPanePaintTheme GetCurrentTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves paint manager of docking panes.
	// Returns:
	//     Returns paint manager.
	//-----------------------------------------------------------------------
	CXTPDockingPanePaintManager* GetPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to create a new layout.
	// Remarks:
	//    You must delete layout when it no longer used.
	// Returns:
	//     A pointer to a CXTPDockingPaneLayout object.
	//-----------------------------------------------------------------------
	virtual CXTPDockingPaneLayout* CreateLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the current layout.
	// Parameters:
	//     pLayout - Pointer to the existing layout.
	//-----------------------------------------------------------------------
	void GetLayout(CXTPDockingPaneLayout* pLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the layout.
	// Parameters:
	//     pLayout - Pointer to the existing layout.
	//-----------------------------------------------------------------------
	void SetLayout(CXTPDockingPaneLayout* pLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to redraw all panes.
	//-----------------------------------------------------------------------
	void RedrawPanes();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve client pane.
	// Returns:
	//     A pointer to a CXTPDockingPaneBase object.
	//-----------------------------------------------------------------------
	CXTPDockingPaneBase* GetClientPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to destroy all panes.
	//-----------------------------------------------------------------------
	void DestroyAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to close all panes.
	//-----------------------------------------------------------------------
	void CloseAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve Image Manager of Docking Panes.
	// Returns:
	//     Image Manager of Docking Panes.
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the specified image manager.
	// Parameters:
	//     pImageManager - Points to a CXTPImageManager object.
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to hide client area.
	// Parameters:
	//     bHide - TRUE to hide client.
	//-----------------------------------------------------------------------
	void HideClient(BOOL bHide);

	//-----------------------------------------------------------------------
	// Summary:
	//     Checks to see if the client is hidden.
	// Returns:
	//     TRUE if the client is hidden, FALSE if otherwise
	//-----------------------------------------------------------------------
	BOOL IsClientHidden();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to use trackers.
	// Parameters:
	//     bSplitterTracker - TRUE to use trackers.
	// Remarks:
	//     This member function will display the contents for child pane
	//     while the splitter is resized if bSplitterTracker is set to FALSE.
	// See Also: IsSplitterTrackerUsed
	//-----------------------------------------------------------------------
	void UseSplitterTracker(BOOL bSplitterTracker);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the splitter tracker is used.
	// Returns:
	//     TRUE if the splitter tracker is used; otherwise returns FALSE.
	// See Also: UseSplitterTracker
	//-----------------------------------------------------------------------
	BOOL IsSplitterTrackerUsed();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to lock splitters.
	// Parameters:
	//     bLock - TRUE to forbid splitter window panes to
	//             be resized.
	//-----------------------------------------------------------------------
	void LockSplitters(BOOL bLock = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the splitters are locked.
	// Returns:
	//     TRUE if the splitters are locked; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL IsSplittersLocked();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set common caption for floating frames.
	// Parameters:
	//     strCaption - Caption to be set.
	//-----------------------------------------------------------------------
	void SetFloatingFrameCaption(CString strCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to use themes for floating frames.
	// Parameters:
	//     bThemedFloatingFrames - TRUE to use themed floating panes, FALSE otherwise.
	//-----------------------------------------------------------------------
	void SetThemedFloatingFrames(BOOL bThemedFloatingFrames);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if themes are used for the floating frames.
	// Returns:
	//     TRUE if they are used; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL IsThemedFloatingFrames();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the current layout is Right-to-Left (RTL).
	// Returns:
	//     TRUE if the current layout is Right-to-Left (RTL), FALSE if the
	//     layout is Left-to-Right.
	//-----------------------------------------------------------------------
	BOOL IsLayoutRTL();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to return the Right-to-Left (RTL) docking direction
	//     equivalent to the direction passed in if IsLayoutRTL returns TRUE.
	// Parameters:
	//     direction - Docking direction
	// Returns:
	//     Returns the Righ-to-Left docking direction of the supplied direction
	//     if IsLayoutRTL returns TRUE.  If IsLayoutRTL returns FALSE, then
	//     the direction passed in remains the same.
	//-----------------------------------------------------------------------
	XTPDockingPaneDirection GetRTLDirection(XTPDockingPaneDirection direction);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine the XTPDockingPaneDirection of pPane.
	// Parameters:
	//     pPane - Points to a CXTPDockingPaneBase object
	// Returns:
	//     The current XTPDockingPaneDirection of pPane.
	//-----------------------------------------------------------------------
	XTPDockingPaneDirection GetPaneDirection(CXTPDockingPaneBase* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves top level virtual container for site window.
	// Remarks:
	//     Top level container always splitter container (xtpPaneTypeSplitterContainer)
	// Returns:
	//     Top level container.
	//-----------------------------------------------------------------------
	CXTPDockingPaneBase* GetTopPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to use custom docking context.
	// Parameters:
	//     pDockingContext - Points to a CXTPDockingPaneContext object
	//-----------------------------------------------------------------------
	void SetDockingContext(CXTPDockingPaneContext* pDockingContext);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves current docking context pointer.
	// Returns:
	//     Pointer to current docking context.
	//-----------------------------------------------------------------------
	CXTPDockingPaneContext* GetDockingContext();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if Alpha Docking Context is used when
	//     panes are being dragged and dropped.
	// Returns:
	//     TRUE if AlphaDockingContext is enabled, FALSE if it is disabled.
	// Remarks:
	//     AlphaDockingContext must be TRUE if Docking Context Stickers will
	//     be used.
	// See Also: SetAlphaDockingContext, IsShowDockingContextStickers, SetShowDockingContextStickers
	//-----------------------------------------------------------------------
	BOOL IsAlphaDockingContext();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether an alpha docking context is
	//     used to indicate where a pane can be docked while dragging the pane.
	// Parameters:
	//     bAlphaDockingContext - TRUE to enable Alpha colorization while docking.
	// Remarks:
	//     Specifies whether the area that a docking pane can occupy is
	//     shaded in gray as the pane is dragged to its new location.  The
	//     shaded area indicates the area on the application the docking
	//     pane will occupy if docked in that location.
	//     AlphaDockingContext must be TRUE if Docking Context Stickers will
	//     be used.
	// See Also: IsAlphaDockingContext, IsShowDockingContextStickers, SetShowDockingContextStickers
	//-----------------------------------------------------------------------
	void SetAlphaDockingContext(BOOL bAlphaDockingContext);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine whether Docking Context Stickers are used.
	// Returns:
	//     TRUE if Docking Context Stickers are used, FALSE if they are not used.
	// See Also: SetAlphaDockingContext, IsAlphaDockingContext, SetShowDockingContextStickers
	//-----------------------------------------------------------------------
	BOOL IsShowDockingContextStickers();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether Visual Studio 2005 style
	//     Docking Context Stickers are used while dragging a docking pane.
	// Parameters:
	//     bShowDockingContextStickers - If True, docking stickers are drawn
	//     on the screen indicating all the possible locations that the docking
	//     pane can be docked while the pane is dragged over the application.
	//     AlphaDockingContext must also be True to display the stickers.
	// Remarks:
	//     AlphaDockingContext must be TRUE if Docking Context Stickers will
	//     be used.
	// See Also: SetAlphaDockingContext, IsAlphaDockingContext, IsShowDockingContextStickers
	//-----------------------------------------------------------------------
	void SetShowDockingContextStickers(BOOL bShowDockingContextStickers);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to save current splitter positions for all splitter containers
	// See Also: CXTPDockingPaneSplitterContainer
	//-----------------------------------------------------------------------
	void NormalizeSplitters();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to recalculate layout for all parent frames of panes.
	//-----------------------------------------------------------------------
	void RecalcFramesLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method updates XTP_DOCKINGPANE_INFO structure for each panes filling
	//     its members.
	//-----------------------------------------------------------------------
	void SyncPanesState();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set default options for each pane
	// Parameters:
	//     dwOptions - Option applied for each pane. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     dwOptions parameter can be one or more of the following:
	//     * <b>xtpPaneNoCloseable</b> Indicates the pane cannot be closed.
	//     * <b>xtpPaneNoHideable</b> Indicates the pane cannot be hidden.
	//     * <b>xtpPaneNoFloatable</b> Indicates the pane cannot be floated.
	//     * <b>xtpPaneNoCaption</b> Indicates the pane has no caption..
	// See Also: GetDefaultPaneOptions, CXTPDockingPane::SetOptions, XTPDockingPaneOptions
	//-----------------------------------------------------------------------
	void SetDefaultPaneOptions(DWORD dwOptions);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves default panes options.
	// Returns:
	//     Default options used for each pane.
	// See Also: SetDefaultPaneOptions, XTPDockingPaneOptions
	//-----------------------------------------------------------------------
	DWORD GetDefaultPaneOptions();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the application’s panes into and out of print-preview mode.
	// Paramets:
	//     bPreview - Specifies whether or not to place the application in print-preview mode. Set to TRUE to place in print preview, FALSE to cancel preview mode.
	//-----------------------------------------------------------------------
	void OnSetPreviewMode (BOOL bPreview);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a pane is created.
	// Parameters:
	//     type    - Type of pane that is created.
	//     pLayout - Points to a CXTPDockingPaneLayout object.
	// Returns:
	//     Created pane.
	//-----------------------------------------------------------------------
	virtual CXTPDockingPaneBase* OnCreatePane(XTPDockingPaneType type, CXTPDockingPaneLayout* pLayout);
protected:

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPDockingPaneManager)
	afx_msg void OnSysColorChange();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnSizeParent(WPARAM, LPARAM lParam);
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	CXTPDockingPaneLayout* GetCurrentLayout();

	CRect _CalculateResultDockingRect(CXTPDockingPaneBase* pPane, XTPDockingPaneDirection direction, CXTPDockingPaneBase* pNeighbour);
	void _InsertPane(CXTPDockingPaneBase* pPane, XTPDockingPaneDirection direction, CXTPDockingPaneBase* pNeighbour);
	void _RemovePane(CXTPDockingPaneBase* pPane);
	void _AttachPane(CXTPDockingPaneBase* pPane, CXTPDockingPaneBase* pNeighbour);
	CXTPDockingPaneBase* _GetHolder(CXTPDockingPaneBase* pPane, BOOL bFloating);
	CXTPDockingPaneBase* _Clone(CXTPDockingPaneBase* pPane);

	void _Redraw();
	void _DetachAll();
	BOOL _ToggleDocking(CXTPDockingPane* pPane, CXTPDockingPaneBase* pHolder);
	BOOL _OnAction(XTPDockingPaneAction action, CXTPDockingPane* pPane, CXTPDockingPaneBase* pDockContainer = NULL, XTPDockingPaneDirection dockDirection = xtpPaneDockLeft);

	void UpdatePanes();

public:
	int m_nSplitterGap;                             // Minimum available width of panes.
	BOOL m_bCloseGroupOnButtonClick;                // If TRUE, when the close button on a group of panes is clicked, then the entire group of panes is closed.  If FALSE, the only the currently visible pane in the group will be closed.
	BOOL m_bHideGroupOnButtonClick;                 // If TRUE, when the hide button on a group of panes is clicked, then the entire group of panes is hidden.  If FALSE, the only the currently visible pane in the group will be hidden.

protected:
	CFrameWnd* m_pSite;                             // Parent window of docking manager.
	CXTPDockingPaneLayout* m_pLayout;               // Selected layout.
	CXTPDockingPanePaintManager* m_pPaintManager;   // Current paint manager.

	CXTPImageManager* m_pImageManager;              // Image manager of docking panes.

	BOOL m_bHideClient;                             // If TRUE, the client area is hidden so that only the docking panes are visible and occupy the entire area.
	BOOL m_bUseSplitterTracker;                     // If TRUE, splitter trackers are used.  When resizing a docking pane, an outline of the pane is drawn as the splitter is dragged.  If FALSE, the docking pane will be resized in "real-time."

	BOOL m_bLockSplitters;                          // If TRUE, you can not resize the panes when they are docked. However, panes can be resized via code and when they are floated.
	BOOL m_bAlphaDockingContext;                    // If TRUE, alpha docking context is used when dragging a pane, the shaded area indicates the panes new location of dropped.
	BOOL m_bShowDockingContextStickers;             // If TRUE, docking context stickers are drawn when the pane is being dragged and dropped. m_bAlphaDockingContext must be TRUE.

	BOOL m_bThemedFloatingFrames;                   // If TRUE, floating docking panes will use the currently set theme.

	DWORD m_dwDefaultPaneOptions;                   // Default Panes options.

	CString m_strFloatingFrameCaption;              // The caption that is displayed in the title bar of a floating frame that has panes docked. This is the floating frame that contains other docking panes.

	CXTPDockingPaneContext* m_pDockingContext;      // Docking context helper.
	CXTPDockingPaneLayout* m_pPreviewLayout;        // Preview mode layout.

private:
	BOOL m_bAttachingPane;



	friend class CXTPDockingPaneBase;
	friend class CXTPDockingPaneSplitterContainer;
	friend class CXTPDockingPaneTabbedContainer;
	friend class CXTPDockingPaneMiniWnd;
	friend class CXTPDockingPaneAutoHideWnd;
	friend class CXTPDockingPaneContext;
	friend class CXTPDockingPane;
	friend class CXTPDockingPaneLayout;
	friend class CXTPDockingPaneAutoHidePanel;
	friend class CDockingPaneSite;
	friend class CDockingPaneOptions;

};

AFX_INLINE CFrameWnd* CXTPDockingPaneManager::GetSite() {
	return m_pSite;
}
AFX_INLINE CXTPDockingPaneLayout* CXTPDockingPaneManager::GetCurrentLayout() {
	return m_pLayout;
}

AFX_INLINE void CXTPDockingPaneManager::RedrawPanes() {
	_Redraw();
}
AFX_INLINE CXTPDockingPanePaintManager* CXTPDockingPaneManager::GetPaintManager() {
	return m_pPaintManager;
}
AFX_INLINE BOOL CXTPDockingPaneManager::IsClientHidden() {
	return m_bHideClient;
}
AFX_INLINE void CXTPDockingPaneManager::UseSplitterTracker(BOOL bSplitterTracker) {
	m_bUseSplitterTracker = bSplitterTracker;
}
AFX_INLINE BOOL CXTPDockingPaneManager::IsSplitterTrackerUsed() {
	return m_bUseSplitterTracker;
}
AFX_INLINE CXTPImageManager* CXTPDockingPaneManager::GetImageManager() {
	return m_pImageManager;
}
AFX_INLINE BOOL CXTPDockingPaneManager::IsSplittersLocked() {
	return m_bLockSplitters;
}
AFX_INLINE void CXTPDockingPaneManager::SetFloatingFrameCaption(CString strCaption) {
	m_strFloatingFrameCaption = strCaption;
}
AFX_INLINE BOOL CXTPDockingPaneManager::IsThemedFloatingFrames() {
	return m_bThemedFloatingFrames;
}
AFX_INLINE CXTPDockingPaneContext* CXTPDockingPaneManager::GetDockingContext() {
	return m_pDockingContext;
}
AFX_INLINE BOOL CXTPDockingPaneManager::IsAlphaDockingContext() {
	return m_bAlphaDockingContext;
}
AFX_INLINE void CXTPDockingPaneManager::SetAlphaDockingContext(BOOL bAlphaDockingContext) {
	m_bAlphaDockingContext = bAlphaDockingContext;
}
AFX_INLINE BOOL CXTPDockingPaneManager::IsShowDockingContextStickers() {
	return m_bShowDockingContextStickers;
}
AFX_INLINE void CXTPDockingPaneManager::SetShowDockingContextStickers(BOOL bShowDockingContextStickers) {
	m_bShowDockingContextStickers = bShowDockingContextStickers;
}
AFX_INLINE void CXTPDockingPaneManager::SetDefaultPaneOptions(DWORD dwOptions) {
	m_dwDefaultPaneOptions = dwOptions;
}
AFX_INLINE DWORD CXTPDockingPaneManager::GetDefaultPaneOptions() {
	return m_dwDefaultPaneOptions;
}
AFX_INLINE void CXTPDockingPaneManager::HidePane(int nID) {
	HidePane((CXTPDockingPaneBase*)FindPane(nID));
}
AFX_INLINE void CXTPDockingPaneManager::ClosePane(int nID) {
	ClosePane(FindPane(nID));
}
AFX_INLINE void CXTPDockingPaneManager::ShowPane(int nID) {
	ShowPane(FindPane(nID));
}
AFX_INLINE void CXTPDockingPaneManager::DestroyPane(int nID) {
	DestroyPane(FindPane(nID));
}
AFX_INLINE BOOL CXTPDockingPaneManager::IsPaneClosed(int nID) {
	return IsPaneClosed(FindPane(nID));
}
AFX_INLINE BOOL CXTPDockingPaneManager::IsPaneHidden(int nID) {
	return IsPaneHidden(FindPane(nID));
}
AFX_INLINE BOOL CXTPDockingPaneManager::IsPaneSelected(int nID) {
	return IsPaneSelected(FindPane(nID));
}









#endif //#if !defined(__XTPDOCKINGPANEMANAGER_H__)
