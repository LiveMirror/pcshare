// XTPShortcutBar.h interface for the CXTPShortcutBar class.
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
#if !defined(__XTPSHORTCUTBAR_H__)
#define __XTPSHORTCUTBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTPShortcutBarPaintManager.h"

//-----------------------------------------------------------------------
// Summary:
//     XTP_SBN_SELECTION_CHANGING is used to indicate that the selection
//     has changed in the shortcut bar.
// Remarks:
//     XTP_SBN_SELECTION_CHANGING is sent in the XTPWM_SHORTCUTBAR_NOTIFY
//     message to the owner window when the selection has changed.
//
//     The selection changed when a user clicks on a shortcut bar item or
//     the items are navigated with the arrow keys.
// Example:
//   See example of XTPWM_SHORTCUTBAR_NOTIFY
// See Also:
//     XTPWM_SHORTCUTBAR_NOTIFY, XTP_SBN_RCLICK
//-----------------------------------------------------------------------
const UINT XTP_SBN_SELECTION_CHANGING  = 1;

//-----------------------------------------------------------------------
// Summary:
//     XTP_SBN_RCLICK is used to indicate that the user has pressed the
//     right mouse button on a shortcut bar item.
// Remarks:
//     XTP_SBN_RCLICK is sent in the XTPWM_SHORTCUTBAR_NOTIFY message to the
//     owner window when the user has right clicked on a shortcut bar item.
// Example:
//   See example of XTPWM_SHORTCUTBAR_NOTIFY
// See Also:
//     XTPWM_SHORTCUTBAR_NOTIFY, XTP_SBN_SELECTION_CHANGING
//-----------------------------------------------------------------------
const UINT XTP_SBN_RCLICK = 2;


//-----------------------------------------------------------------------
// Summary:
//     The XTPWM_SHORTCUTBAR_NOTIFY message is sent to the CXTPShortcutBar owner window
//     whenever an action occurs within the CXTPShortcutBar
// Parameters:
//     nAction -  Value of wParam specifies a ShortcutBar value that indicates the user's
//                request.
//     pItem    - The value of lParam points to an CXTPShortcutBarItem object that contains information for the
//                specified item. This pointer should <b>never</b> be NULL.
// Remarks:
//     nAction parameter can be one of the following values:
//         * <b>XTP_SBN_SELECTION_CHANGING</b> Indicates the selection has changed in the shortcut bar.
//         * <b>XTP_SBN_RCLICK</b> Indicates the user pressed the right mouse button on the shortcut bar item.
//
// Returns:
//     If the application is to process this message, the return value should be TRUE, otherwise the
//     return value is FALSE.
// Example:
//     Here is an example of how an application would process the XTPWM_SHORTCUTBAR_NOTIFY
//     message.
// <code>
//
// BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_MESSAGE(XTPWM_SHORTCUTBAR_NOTIFY, OnShortcutBarNotify)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CMainFrame::OnShortcutBarNotify(WPARAM wParam, LPARAM lParam)
// {
//      switch (wParam)
//      {
//          case XTP_SBN_SELECTION_CHANGING:
//              {
//                  CXTPShortcutBarItem* pItem = (CXTPShortcutBarItem*)lParam;
//                  TRACE(_T("Selection Changing. Item.Caption = %s\n"), pItem->GetCaption());
//
//                  // TODO: You can return -1 to ignore changing
//
//              }
//              return TRUE;
//          case XTP_SBN_RCLICK:
//              {
//                  CPoint point(lParam);
//                  CXTPShortcutBarItem* pItem = m_wndShortcutBar.HitTest(point);
//                  if (pItem)
//                  {
//                      TRACE(_T("RClick. Item.ID = %i\n"), pItem->GetID());
//
//                      CMenu mnu;
//                      mnu.LoadMenu(IDR_POPUP_MENU);
//
//                      m_wndShortcutBar.ClientToScreen(&point);
//
//                      CXTPCommandBars::TrackPopupMenu(mnu.GetSubMenu(0), 0, point.x, point.y, AfxGetMainWnd());
//
//                  }
//              }
//              return TRUE;
//
//
//      }
//      return 0;
//  }
// </code>
// See Also:
//     XTP_SBN_SELECTION_CHANGING, XTP_SBN_RCLICK, CXTPShortcutBar
//-----------------------------------------------------------------------
const UINT XTPWM_SHORTCUTBAR_NOTIFY = (WM_APP + 2583);


class CXTPShortcutBar;
class CXTPImageManager;

//===========================================================================
// Summary:
//     CXTPShortcutBarItem is the base class representing an item of the
//     ShortcutBar Control.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarItem : public CCmdTarget
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBarItem object.
	// Parameters:
	//     pShortcutBar - Points to the parent ShortcutBar class.
	//     nID - Identifier of the item.
	//     pWnd - Client window of the item
	//-----------------------------------------------------------------------
	CXTPShortcutBarItem(CXTPShortcutBar* pShortcutBar, UINT nID, CWnd* pWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBarItem object.
	// Parameters:
	//     pShortcutBar - Points to the parent ShortcutBar class.
	//-----------------------------------------------------------------------
	CXTPShortcutBarItem(CXTPShortcutBar* pShortcutBar);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the identifier of the item.
	// Parameters:
	//     nID - The new identifier of the item.
	//-----------------------------------------------------------------------
	void SetID(int nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the identifier of the item.
	// Returns:
	//     Identifier of the item.
	//-----------------------------------------------------------------------
	int GetID();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the caption of the item.
	// Parameters:
	//     strCaption - The new caption of the item.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR strCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the caption of the item.
	// Returns:
	//     A CString object containing the caption of the item.
	//-----------------------------------------------------------------------
	CString GetCaption();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the visibility state of the item.
	// Parameters:
	//     bVisible - TRUE if the item is visible; otherwise FALSE.
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the item is visible.
	// Returns:
	//     TRUE if the item is visible; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsVisible();

	//-----------------------------------------------------------------------
	// Summary:
	//     Associates ToolTip text with the item.
	// Parameters:
	//     strTooltip - The ToolTip text to display when the mouse cursor is over the item.
	//-----------------------------------------------------------------------
	void SetTooltip(LPCTSTR strTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the ToolTip text associated with the item.
	// Returns:
	//     The ToolTip text for the item.
	//-----------------------------------------------------------------------
	CString GetTooltip();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the item is expanded.
	// Returns:
	//     TRUE if the item is expanded; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsExpanded();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the item is expand button.
	// Returns:
	//     TRUE if the item is expand button; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsItemExpandButton();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the parent ShortcutBar class.
	// Returns:
	//     A Pointer to the parent CXTPShortcutBar class.
	//-----------------------------------------------------------------------
	CXTPShortcutBar* GetShortcutBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves the dimensions of the rectangle that bounds the item.
	// Returns:
	//     CRect object contains the bounding rectangle of the item.
	//-----------------------------------------------------------------------
	CRect GetItemRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the 32-bit value associated with the item.
	// Parameters:
	//     dwData - Contains the new value to associate with the item.
	//-----------------------------------------------------------------------
	void SetItemData(DWORD_PTR dwData);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves the application-supplied 32-bit value associated with the item
	// Returns:
	//     The 32-bit value associated with the item
	//-----------------------------------------------------------------------
	DWORD_PTR GetItemData();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the image of this shortcut bar item.
	// Parameters:
	//     nWidth - Width of the icon to be retrieved. (CXTPImageManagerIcon
	//              can have multiple image sizes for the same item, the
	//              shortcut bar uses both 16x16 and 24x24 images depending on
	//              if the item is displayed in the extended shortcut list).
	//              If a 16x16 icon is to be retrieved, then pass in 16 for
	//              the width.
	// Returns:
	//     A pointer the CXTPImageManagerIcon object for this shortcut bar
	//     item.
	//-----------------------------------------------------------------------
	CXTPImageManagerIcon* GetImage(int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine is the ShortcutBarItem is hidden.
	//     An item is hidden when there are more items then can be displayed
	//     in the bottom shortcut bar.  As the shortcut bar is expanded, items
	//     that were hidden will become visible as more items are added to the
	//     expanded shortcut bar.
	// Returns:
	//     TRUE is the ShortcutBarItem is hidden, otherwise FALSE
	//-----------------------------------------------------------------------
	BOOL IsHidden();

protected:
	CString m_strCaption;               // Caption for the item.
	CString m_strTooltip;               // Tooltip for the item.
	int m_nID;                          // Item's identifier.
	CRect m_rcItem;                     // Bounding rectangle of the item
	BOOL m_bVisible;                    // TRUE if the item is visible
	BOOL m_bExpanded;                   // TRUE if the item is expanded
	BOOL m_bHidden;                     // TRUE if the item is hidden
	HWND m_hwndChild;                   // Child window associated with the item
	BOOL m_bExpandButton;               // True if the item is expand button
	DWORD_PTR m_dwData;                 // The 32-bit value associated with the item
	CXTPShortcutBar* m_pShortcutBar;    // Parent CXTPShortcutBar class

private:

	friend class CXTPShortcutBar;
};

//-----------------------------------------------------------------------
// Summary:
//     CXTPShortcutBar is a CWnd derived class. It is used to implement
//     an Outlook2003 ShortcutBar style control.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPShortcutBar : public CWnd
{
	//-----------------------------------------------------------------------
	// Summary:
	//     Array of CXTPShortcutBarItem objects
	//-----------------------------------------------------------------------
	typedef CArray<CXTPShortcutBarItem*, CXTPShortcutBarItem*> CShortcutArray;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBar object.
	//-----------------------------------------------------------------------
	CXTPShortcutBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShortcutBar object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPShortcutBar();


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method creates the ShortcutBar control
	// Parameters:
	//     dwStyle - Style for the ShortcutBar.
	//     rect -  Specifies the size and position of the ShortcutBar control.
	//     pParentWnd - Specifies the parent window of the ShortcutBar control.
	//     nID - Specifies the ShortcutBar control ID.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL Create(DWORD dwStyle, const RECT&  rect, CWnd* pParentWnd, UINT nID);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to retrieve state information from the registry or .INI file.
	// Parameters:
	//     lpszProfileName - Points to a null-terminated string that specifies the name of a
	//     section in the initialization file or a key in the Windows registry where state
	//     information is stored.
	//-----------------------------------------------------------------------
	void LoadState(LPCTSTR lpszProfileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to save the state information to the registry or .INI file.
	// Parameters:
	//     lpszProfileName - Points to a null-terminated string that specifies the name of a
	//     section in the initialization file or a key in the Windows registry where state
	//     information is stored.
	//-----------------------------------------------------------------------
	void SaveState(LPCTSTR lpszProfileName);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to add a new item.
	// Parameters:
	//     nID - Identifier of item that is to be added.
	//     pWnd - A Pointer to CWnd class associated with new item.
	// Returns:
	//     A pointer to a CXTPShortcutBarItem object.
	//-----------------------------------------------------------------------
	CXTPShortcutBarItem* AddItem(UINT nID, CWnd* pWnd = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to find an item with the specified identifier.
	// Parameters:
	//     nID - Identifier of the item needed to find.
	// Returns:
	//     A pointer to a CXTPShortcutBarItem object.
	//-----------------------------------------------------------------------
	CXTPShortcutBarItem* FindItem(int nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to select the specified item.
	// Parameters:
	//     pItem - Item to be selected.
	//-----------------------------------------------------------------------
	void SelectItem(CXTPShortcutBarItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to remove the item.
	// Parameters:
	//     nID - Identifier of the item to delete.
	//-----------------------------------------------------------------------
	void RemoveItem(int nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to remove all items.
	//-----------------------------------------------------------------------
	void RemoveAllItems();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves an item with specified index.
	// Parameters:
	//     nIndex - Specifies the zero-based index of the item to retrieve.
	// Returns:
	//     A pointer to a CXTPShortcutBarItem object.
	//-----------------------------------------------------------------------
	CXTPShortcutBarItem* GetItem(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method determines where a point lies in a specified item.
	// Parameters:
	//     pt - Specifies the point to be tested.
	// Returns:
	//     A pointer to a CXTPShortcutBarItem item that occupies the specified point or NULL
	//     if no item occupies the point.
	//-----------------------------------------------------------------------
	CXTPShortcutBarItem* HitTest(CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the minimum height of the client area in a ShortcutBar control.
	//     The default value is 200.
	// Parameters:
	//     nMinHeight - Specifies the minimum height in pixels of the client.
	//-----------------------------------------------------------------------
	void SetMinimumClientHeight(int nMinHeight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine gripper rectangle.
	// Returns:
	//     CRect object contains gripper bounding rectangle.
	//-----------------------------------------------------------------------
	CRect GetGripperRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get size of items
	// Returns:
	//     CSize object contains size of items
	//-----------------------------------------------------------------------
	CSize GetItemSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method determines the count of visible lines in the ShortcutBar control.
	// Returns:
	//     Count of visible lines
	// See Also:
	//     SetExpandedLinesCount
	//-----------------------------------------------------------------------
	int GetExpandedLinesCount();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the count of lines that are visible in the ShortcutBar control.
	// Parameters:
	//     nCount - Number of visible lines.
	// See Also:
	//     GetExpandedLinesCount
	//-----------------------------------------------------------------------
	void SetExpandedLinesCount(int nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the hot item in the ShortcutBar control.
	// Returns:
	//     Pointer to the hot item.
	//-----------------------------------------------------------------------
	CXTPShortcutBarItem* GetHotItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the selected item in the ShortcutBar control.
	// Returns:
	//     Pointer to the selected item.
	//-----------------------------------------------------------------------
	CXTPShortcutBarItem* GetSelectedItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the pressed item in the ShortcutBar control.
	// Returns:
	//     Pointer to the pressed item.
	//-----------------------------------------------------------------------
	CXTPShortcutBarItem* GetPressedItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines visibility of client pane.
	// Returns:
	//     TRUE if client pane is visible.
	//-----------------------------------------------------------------------
	BOOL IsClientPaneVisible();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to show or hide client pane.
	// Parameters:
	//     bVisible - TRUE to show client pane; FALSE to hide.
	//-----------------------------------------------------------------------
	void SetClientPaneVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set new image manager.
	// Parameters:
	//     pImageManager - Points to a CXTPImageManager object to be set
	// Example:
	// <code>
	//     CXTPImageManager* pImageManager = new CXTPImageManager();
	//     pImageManager->SetIcons(IDR_MAINFRAME);
	//     m_wndShortcutBar.SetImageManager(pImageManager);
	// </code>
	// See Also:
	//     GetImageManager
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get a pointer to the image manager of shortcut
	//     bar control.
	// Returns:
	//     Pointer to the image manager of shortcut bar control.
	// Remarks:
	//     The image manager is used to hold all of the icons displayed in the
	//     shortcut bar control.
	// See Also:
	//     SetImageManager
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine how many CXTPShortcutBarItem(s) are int he
	//     shortcut bar.
	// Returns:
	//     Number of shortcut bar items in the shortcut bar.
	//-----------------------------------------------------------------------
	int GetItemCount();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to switch the visual theme of the ShortcutBar control.
	// Parameters:
	//     paintTheme - New visual theme. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     paintTheme can be one of the following:
	//     * <b>xtpShortcutThemeOffice2000</b> Enables Office 2000 style theme.
	//     * <b>xtpShortcutThemeOfficeXP</b> Enables Office XP style theme.
	//     * <b>xtpShortcutThemeOffice2003</b> Enables Office 2003 style theme.
	//
	// See Also:
	//     GetCurrentTheme, SetCustomTheme, XTPShortcutBarPaintTheme
	//-----------------------------------------------------------------------
	void SetTheme(XTPShortcutBarPaintTheme paintTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set a custom theme.
	// Parameters:
	//     pPaintManager - New paint manager.
	// See Also:
	//     GetCurrentTheme, SetTheme, XTPShortcutBarPaintTheme
	//-----------------------------------------------------------------------
	void SetCustomTheme(CXTPShortcutBarPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the current paint manager.
	// Returns:
	//     The current paint manager.
	//-----------------------------------------------------------------------
	CXTPShortcutBarPaintManager* GetPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the current visual theme.
	// Returns:
	//     The current theme.
	// See Also:
	//     SetTheme, SetCustomTheme, XTPShortcutBarPaintTheme
	//-----------------------------------------------------------------------
	XTPShortcutBarPaintTheme GetCurrentTheme();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to reposition items.
	//-----------------------------------------------------------------------
	void Reposition();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when reposition is done.
	//-----------------------------------------------------------------------
	virtual void OnRepositionDone();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when expand button is pressed.
	// Parameters:
	//     pExpandButton - A pointer to expand button.
	//-----------------------------------------------------------------------
	virtual void OnExpandButtonDown(CXTPShortcutBarItem* pExpandButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine the number of visible items
	// Returns:
	//     Number of visible items
	//-----------------------------------------------------------------------
	int GetVisibleItemsCount();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set hot item.
	// Parameters:
	//     pItem - Item to be hot.
	//-----------------------------------------------------------------------
	void SetHotItem(CXTPShortcutBarItem* pItem);

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPShortcutBar)
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPShortcutBar)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnMouseLeave();
	afx_msg void OnSysColorChange();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point) ;
	afx_msg LRESULT OnExpandPopupExecute(WPARAM wparam, LPARAM lParam);
	afx_msg void OnShowMoreButtons();
	afx_msg void OnShowFewerButtons();
	afx_msg void OnUpdateShowMoreButtons(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowFewerButtons(CCmdUI* pCmdUI);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:
	CXTPShortcutBarPaintManager* m_pPaintManager;   // Current paint manager.
	CXTPImageManager* m_pImageManager;              // Current image manager.
	int m_nExpandedLines;                           // Number of lines currently visible
	int m_nDesiredExpandedLines;                    // Number of lines desired to be visible.
	int m_nMinClientHeight;                         // The height of client area.
	XTPShortcutBarPaintTheme m_paintTheme;          // Current theme.
	CShortcutArray m_arrItems;                      // Array of items.
	CSize m_szItem;                                 // Size of items
	CRect m_rcClient;                               // Client's rectangle.
	CRect m_rcGripper;                              // Gripper's rectangle.
	HWND m_hwndClient;                              // Client window handle
	HCURSOR m_hSizeCursor;                          // Handle to the cursor displayed for the size icon.
	BOOL m_bTrackingSize;                           // TRUE if control in tracking mode.
	CXTPShortcutBarItem* m_pHotItem;                // Pointer to hot item.
	CXTPShortcutBarItem* m_pSelectedItem;           // Pointer to selected item.
	CXTPShortcutBarItem* m_pPressedItem;            // Pointer to pressed item.
	CToolTipCtrl m_wndToolTip;                      // Tooltip to be displayed for the control.

	BOOL m_bClientPaneVisible;                      // TRUE is client Pane in ShortcutBar is visible
	HCURSOR m_hHandCursor;                          // Hand cursor that is displayed when the cursor is positioned over a shortcut bar item.


	friend class CXTPShortcutBarItem;
	friend class CShortcutBarCtrl;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CString CXTPShortcutBarItem::GetCaption() {
	return m_strCaption;
}
AFX_INLINE CString CXTPShortcutBarItem::GetTooltip() {
	return m_strTooltip;
}
AFX_INLINE BOOL CXTPShortcutBarItem::IsItemExpandButton() {
	return m_bExpandButton;
}
AFX_INLINE BOOL CXTPShortcutBarItem::IsExpanded() {
	return m_bExpanded;
}
AFX_INLINE CXTPShortcutBar* CXTPShortcutBarItem::GetShortcutBar() {
	return m_pShortcutBar;
}
AFX_INLINE CRect CXTPShortcutBarItem::GetItemRect() {
	return m_rcItem;
}
AFX_INLINE void CXTPShortcutBarItem::SetItemData(DWORD_PTR dwData) {
	m_dwData = dwData;
}
AFX_INLINE DWORD_PTR CXTPShortcutBarItem::GetItemData() {
	return m_dwData;
}



AFX_INLINE CXTPShortcutBarItem* CXTPShortcutBar::GetHotItem() {
	return m_pHotItem;
}
AFX_INLINE CXTPShortcutBarItem* CXTPShortcutBar::GetSelectedItem() {
	return m_pSelectedItem;
}
AFX_INLINE CXTPShortcutBarItem* CXTPShortcutBar::GetPressedItem() {
	return m_pPressedItem;
}
AFX_INLINE CRect CXTPShortcutBar::GetGripperRect() {
	return m_rcGripper;
}
AFX_INLINE CSize CXTPShortcutBar::GetItemSize() {
	return m_szItem;
}
AFX_INLINE int CXTPShortcutBar::GetExpandedLinesCount() {
	return m_nExpandedLines;
}
AFX_INLINE BOOL CXTPShortcutBar::IsClientPaneVisible() {
	return m_bClientPaneVisible;
}
AFX_INLINE void CXTPShortcutBar::SetClientPaneVisible(BOOL bVisible) {
	m_bClientPaneVisible = bVisible;
	Reposition();
}
AFX_INLINE CXTPShortcutBarPaintManager* CXTPShortcutBar::GetPaintManager() {
	return m_pPaintManager;
}
AFX_INLINE void CXTPShortcutBar::SetMinimumClientHeight(int nMinHeight) {
	m_nMinClientHeight = nMinHeight;
}
AFX_INLINE XTPShortcutBarPaintTheme CXTPShortcutBar::GetCurrentTheme() {
	return m_paintTheme;
}
AFX_INLINE BOOL CXTPShortcutBar::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

#endif // !defined(__XTPSHORTCUTBAR_H__)
