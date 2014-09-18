// XTPTabManager.h: interface for the CXTPTabManager class.
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
#if !defined(__XTPTABMANAGER_H__)
#define __XTPTABMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTPTabPaintManager.h"

class CXTPTabManager;
class CXTPTabPaintManager;

//===========================================================================
// Summary:
//     CXTPTabManagerItem is a CCmdTarget derived class that represents
//     a tab item.  This is the base class for the tabs that appear
//     in the CommandBars TabWorkSpace, DockingPanes, and TabControl.
//===========================================================================
class _XTP_EXT_CLASS  CXTPTabManagerItem : public CCmdTarget
{
protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabManagerItem object.
	//-------------------------------------------------------------------------
	CXTPTabManagerItem();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the text caption that is displayed
	//     in this tab's button.
	// Parameters:
	//     lpszCaption - Text caption of the tab.
	// See Also: GetCaption
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the text caption to the tab.
	// Returns:
	//     Text caption of tab.  This is the text displayed in the tab
	//     button.
	// See Also: GetCaption
	//-----------------------------------------------------------------------
	CString GetCaption();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the tooltip for this tab.  This is the
	//     text that will be displayed when the mouse pointer is positioned
	//     over the tab button.
	// Parameters: lpszTooltip - Tooltip to set for this tab.
	// See Also: GetTooltip, CXTPTabManager::GetItemTooltip
	//-----------------------------------------------------------------------
	void SetTooltip(LPCTSTR lpszTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the tooltip displayed when the mouse
	//     pointer is positioned over the tab button.
	// Returns:
	//     The tooltip displayed when the mouse if over the tab button.
	// See Also: SetTooltip, CXTPTabManager::GetItemTooltip
	//-----------------------------------------------------------------------
	CString GetTooltip() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the color of the tab button.
	// Returns:
	//     The color of the tab button.
	// See Also: SetColor, CXTPTabManager::GetItemColor
	//-----------------------------------------------------------------------
	COLORREF GetColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the color of the tab button.
	// Parameters:   clr - New color of the tab button.
	// Remarks:
	//     After the new color is set, CXTPTabManager::RedrawControl is
	//     called.
	// See Also: GetColor, CXTPTabManager::GetItemColor
	//-----------------------------------------------------------------------
	void SetColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the bounding rectangle of the tab
	//     button.
	// Returns:
	//     Bounding rectangle of the tab button.
	// See Also: SetRect
	//-----------------------------------------------------------------------
	CRect GetRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the width of the tab button.
	// Returns:
	//     Width of the tab button.
	// Remarks:
	//     The width might change depending on the tab layout and
	//     position of the tabs.
	// See Also: XTPTabLayoutStyle, XTPTabPosition
	//-----------------------------------------------------------------------
	int GetLength();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the index of the tab within the
	//     collection of tabs.
	// Returns:
	//     Index of tab.
	// See Also: CXTPTabManager::GetItem, CXTPTabManager
	//-----------------------------------------------------------------------
	int GetIndex();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the image index of the tab.
	// Returns:
	//     Index of the image displayed in the tab button.  The index is
	//     the index of an image within an CXTPImageManager or CImageList
	//     object.
	// Remarks:
	//     This is the index of an image within an CXTPImageManager or CImageList
	//     object that will be displayed in the tab button.  CXTPTabManager::ShowIcons
	//     is used to hide\show icons.
	// See Also: SetImageIndex, CXTPTabControl::InsertItem, CXTPTabControl::GetImageManager,
	//           CXTPTabControl::SetImageList, CXTPTabManager::ShowIcons
	//-----------------------------------------------------------------------
	int GetImageIndex();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the image index of the tab.
	// Parameters: nImage - Index of an image within an CXTPImageManager or CImageList
	//                      object that will be displayed in the tab button.
	// Remarks:
	//     This is the index of an image within an CXTPImageManager or CImageList
	//     object that will be displayed in the tab button.  CXTPTabManager::ShowIcons
	//     is used to hide\show icons.
	// See Also: GetImageIndex, CXTPTabControl::InsertItem, CXTPTabControl::GetImageManager,
	//           CXTPTabControl::SetImageList, CXTPTabManager::ShowIcons
	//-----------------------------------------------------------------------
	void SetImageIndex(int nImage);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves the application-supplied 32-bit value
	//     associated with the tab item.
	// Remarks:
	//     This returns the "extra" information such as pointers or
	//     integers that was associated with the tab item using the SetData
	//     function.
	//
	//          NOTE: Do not use GetData when working with docking pane tabs.
	//                With DockingPanes, GetData returns a pointer to a CXTPDockingPane.
	//                Use CXTPDockingPane::GetPaneData when working with
	//                docking panes.
	// Returns:
	//     The 32-bit value associated with the tab item.
	// See Also: SetData, CXTPDockingPane::SetPaneData, CXTPDockingPane::GetPaneData
	//-----------------------------------------------------------------------
	DWORD_PTR GetData();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the 32-bit value associated with the
	//     tab item.
	// Parameters:
	//     dwData - Contains the new value to associate with the tab item.
	// Remarks:
	//     This allows any "extra" information such as pointers or
	//     integers to be associated with the tab item.
	//
	//          NOTE: Do not use SetData when working with docking pane tabs.
	//                For DockingPanes, dwData stores a pointer to a CXTPDockingPane.
	//                Use CXTPDockingPane::SetPaneData when working with
	//                docking panes.
	// See Also: SetData, CXTPDockingPane::SetPaneData
	//-----------------------------------------------------------------------
	void SetData(DWORD_PTR dwData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to associate an object with the tab.  This
	//     will tell the tab what to display in the tab client area when
	//     the tab is selected.
	//     I.e. The HWND of a dialog.
	// Parameters:   hWnd - HWND of object that should be displayed in this tab
	//                      when the tab is selected.
	// See Also: GetHandle
	//-----------------------------------------------------------------------
	void SetHandle(HWND hWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the HWND of the object associated
	//     with the tab.  This is the HWND of the object that is displayed
	//     in the tab client area when the tab is selected.
	// Returns:
	//     HWND of object associated with the tab.
	// See Also: SetHandle
	//-----------------------------------------------------------------------
	HWND GetHandle();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the bounding rectangle of the tab.
	// Parameters:   rcItem - Bounding rectangle of tab.
	// See Also: GetRect
	//-----------------------------------------------------------------------
	void SetRect(CRect rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the tab is visible.
	// Returns:
	//     TRUE if the tab is visible, FALSE otherwise.
	// Remarks:
	//     A tab might not be visible because of the tab layout.  You
	//     can also use the SetVisible member to hide\show a tab.
	// See Also: SetVisible
	//-----------------------------------------------------------------------
	BOOL IsVisible();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to hide or show the tab.
	// Parameters: bVisible - TRUE to show the tab, FALSE to hide the tab.
	// See Also: IsVisible
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the tab is enabled.
	// Returns:
	//     TRUE if the tab is enabled, FALSE if the tab is disabled.
	// See Also: SetEnabled
	//-----------------------------------------------------------------------
	BOOL IsEnabled();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set whether the tab is enabled.
	// Parameters:   bEnabled - TRUE to enable the tab, FALSE to disable the tab.
	// Remarks:
	//     If FALSE, the tab will appear "grayed out" and the user can
	//     not access the tab.
	// See Also: IsEnabled
	//-----------------------------------------------------------------------
	void SetEnabled(BOOL bEnabled);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine it the tab is currently
	//     selected.  I.e. when a tab is clicked.
	// Returns:
	//     TRUE if the tab is currently selected, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsSelected();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if the tab is highlighted.
	// Returns:
	//     TRUE if the tab is highlighted (If CXTPTabPaintManager::m_bHotTracking
	//     is TRUE), otherwise FALSE.
	// Remarks:
	//     A tab will be highlighted when CXTPTabPaintManager::m_bHotTracking
	//     is TRUE and the mouse pointer is positioned over the tab button.
	//     and when dragging a tab.  I.e. Reordering tabs.
	//-----------------------------------------------------------------------
	BOOL IsHighlighted();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to delete this tab.
	//-------------------------------------------------------------------------
	void Remove();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to select this tab.
	// Remarks:
	//     This will invoke the OnItemClick method.  A tab can also be selected
	//     by clicking on the tab.
	//-------------------------------------------------------------------------
	void Select();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to get a pointer to the CXTPTabManager object.
	//     The CXTPTabManager object manages all CXTPTabManagerItem objects.
	// Returns:
	//     Pointer to parent CXTPTabManager.
	//-------------------------------------------------------------------------
	CXTPTabManager* GetTabManager();

private:

	int             m_nLength;      // Width of the tab button.
	int             m_nImage;       // Image index, this is the index of an image within a CXTPImageManger or CImageList object.
	int             m_nIndex;       // Index of the tab within the collection of tabs.
	BOOL            m_bFound;
	BOOL            m_bEnabled;     // TRUE if the tab is enabled.
	BOOL            m_bVisible;     // TRUE if the tab is visible.
	HWND            m_hWnd;         // A CWnd pointer to the window associated with a tab.
	HICON           m_hIcon;        // Icon associated with the tab.
	DWORD_PTR       m_dwData;
	CRect           m_rcItem;       // Rectangle of this tab.
	CString         m_strCaption;   // User specified label for the tab.
	CString         m_strToolTip;   // Tooltip text for the tab.
	COLORREF        m_clrItem;      // Color of tab button.
	CXTPTabManager* m_pTabManager;  // Parent item.

	friend class CXTPTabManager;
	friend class CXTPTabPaintManager;
	friend class CXTPTabWorkspace;
	friend class CXTPTabClientWnd;
	friend class CXTPGroupClientWnd;


};

//===========================================================================
// Summary:
//     CXTPTabManagerNavigateButton is a class used to represent the
//     tab navigation buttons.
// Remarks:
//     Navigation button are the buttons that appear in the tab header
//     area.  The buttons include the left arrow, right arrow, and close
//     buttons.  Any combination of these buttons can be shown.  You
//     can choose to never display then, always display them, or
//     automatically display them.  If yo automatically display the buttons,
//     they are only displayed when needed.  I.e, When there are too many
//     tabs to display in the tab header, the arrow buttons appear.
//
//          By default these all these buttons are displayed in the
//          CommandBars TabWorkSpace.  You will need to add them to the
//          TabControl.
//
//          If the xtpTabNavigateButtonAutomatic flag is used, then the button
//          will appear only when needed.  I.e. When the XTPTabLayoutStyle is
//          set to xtpTabLayoutAutoSize, all tab might not fit in the tab header
//          area.  When there are more tabs than can fit in the header, the
//          button will automatically be displayed.
//
// Example:
//     This example code illustrates how to specify when the tab navigation
//     buttons are displayed.
// <code>
// //Finds the left navigation button and specifies that it is always displayed
// m_wndTabControl.FindNavigateButton(xtpTabNavigateButtonLeft)->SetFlags(xtpTabNavigateButtonAlways);
// //Finds the right navigation button and specifies that it is never displayed
// m_wndTabControl.FindNavigateButton(xtpTabNavigateButtonRight)->SetFlags(xtpTabNavigateButtonNone);
// //Finds the close navigation button and specifies that it is always displayed
// m_wndTabControl.FindNavigateButton(xtpTabNavigateButtonClose)->SetFlags(xtpTabNavigateButtonAlways);
//
// //Called to recalculate tab area and reposition components
// Reposition();
// </code>
// See Also: XTPTabNavigateButtonFlags, CXTPTabManager::FindNavigateButton
//===========================================================================
class _XTP_EXT_CLASS CXTPTabManagerNavigateButton
{

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabManagerNavigateButton object.
	// Parameters:
	//     pManager - CXTPTabManager object to draw the button on.
	//     nID      - Id of the button, can be one of the values listed in the
	//                Remarks section.
	//     dwFlags  - Indicates when to display the button.
	// Remarks:
	//     Standard ids of buttons are listed below:
	//     * <b>xtpTabNavigateButtonLeft</b>  Left tab navigation button.
	//     * <b>xtpTabNavigateButtonRight</b> Right tab navigation button.
	//     * <b>xtpTabNavigateButtonClose</b> Close tab navigation button.
	// See also:
	//     CXTPTabManager::FindNavigateButton, XTPTabNavigateButtonFlags,
	//     XTPTabNavigateButton
	//-----------------------------------------------------------------------
	CXTPTabManagerNavigateButton(CXTPTabManager* pManager, UINT nID, XTPTabNavigateButtonFlags dwFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to adjust the width of the tab header area.
	//     This will subtract the width of the navigate button from
	//     the width of the tab header area supplied.
	// Parameters:
	//     nWidth - Width of the tab header area.
	//
	// Remarks:
	//     The width will only be adjusted if the xtpTabNavigateButtonAlways
	//     is set.  If the tabs are vertical, then the height of the buttons
	//     are subtracted from the tab header area.
	//-----------------------------------------------------------------------
	void AdjustWidth(int& nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify when the navigation button is displayed.
	// Parameters:
	//     dwFlags - Indicates when to display the button.
	// See Also: XTPTabNavigateButtonFlags, CXTPTabManager::FindNavigateButton, GetFlags
	//-----------------------------------------------------------------------
	void SetFlags(XTPTabNavigateButtonFlags dwFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine when the navigation button is displayed.
	// Returns:
	//     XTPTabNavigateButtonFlags indicating when the navigation
	//     button is displayed.
	// See Also: XTPTabNavigateButtonFlags, CXTPTabManager::FindNavigateButton, SetFlags
	//-----------------------------------------------------------------------
	XTPTabNavigateButtonFlags GetFlags();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to draw the navigation button in the tab
	//     header area.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the mouse cursor moves.
	// Parameters:
	//     hWnd - Handle to the CWnd object beneath the mouse cursor.
	//     pt   - Specifies the x- and y-coordinate of the cursor. These
	//            coordinates are always relative to the upper-left
	//            corner of the window.
	// Remarks:
	//     When the mouse cursor is moved across a tab navigation button,
	//     CXTPTabManagerNavigateButton::PerformMouseMove is called to
	//     perform actions related to mouse movement over the navigation
	//     buttons.  This will highlight and un-highlight the tab
	//     navigation buttons.
	//-----------------------------------------------------------------------
	void PerformMouseMove(HWND hWnd, CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the user presses the left mouse button.
	// Parameters:
	//     hWnd - Handle to the CWnd object beneath the mouse cursor.
	//     pt   - Specifies the x- and y-coordinate of the cursor. These
	//            coordinates are always relative to the upper-left
	//            corner of the window.
	// Remarks:
	//     When the user clicks on a tab navigation button,
	//     CXTPTabManagerNavigateButton::PerformClick will call the OnExecute
	//     member, the OnExecute member will then call the
	//     CXTPTabManager::OnNavigateButtonClick member passing in the ID
	//     of the tab navigation button that was pressed.
	//
	//          This member performs the common operations of when a button
	//          is clicked such as drawing the "pressed" button and calling
	//          the OnExecute function of the clicked tab navigation button.
	//
	// See Also: CXTPTabManager::OnNavigateButtonClick
	//-----------------------------------------------------------------------
	void PerformClick(HWND hWnd, CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the navigation button is enabled.
	// Returns:
	//     TRUE if the navigation button is enabled, FALSE if it is disabled.
	//-----------------------------------------------------------------------
	BOOL IsEnabled();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to determine if the navigation button
	//     is currently pressed.
	// Returns:
	//     TRUE if the navigation button is currently pressed.
	// Remarks:
	//     This member is used in the CColorSetDefault::FillNavigateButton
	//     function and all other objects derived from CColorSetDefault to
	//     determine how to colorize the button.
	// See Also: CColorSetDefault::FillNavigateButton
	//-----------------------------------------------------------------------
	BOOL IsPressed();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to determine if the navigation button
	//     is currently highlighted.  This occurs when the mouse cursor
	//     is positioned over the navigation button.
	// Remarks:
	//     This member is used in the CColorSetDefault::FillNavigateButton
	//     function and all other objects derived from CColorSetDefault to
	//     determine how to colorize the button.
	// Returns:
	//     TRUE if the navigation button is currently highlighted.
	// See Also: CColorSetDefault::FillNavigateButton
	//-----------------------------------------------------------------------
	BOOL IsHighlighted();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the navigation button is
	//     currently visible.
	// Returns:
	//     TRUE if the navigation button is currently visible, FALSE
	//     otherwise.
	//-----------------------------------------------------------------------
	BOOL IsVisible();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the bounding rectangle of the
	//     navigation button.
	// Returns:
	//     Bounding rectangle of navigation button.
	//-----------------------------------------------------------------------
	CRect GetRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to update self position.
	// Parameters:
	//     rcNavigateButtons - Bounding rectangle of the tab navigation
	//                         buttons.
	//-----------------------------------------------------------------------
	virtual void Reposition(CRect& rcNavigateButtons);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to draw a symbol in the tab navigation
	//     button. I.e. "<", ">" or "x".  This member must be overridden in
	//     derived classes.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc  - Bounding rectangle of tab navigation button.
	//-----------------------------------------------------------------------
	virtual void DrawEntry(CDC* pDC, CRect rc) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when a navigation button is clicked.
	// Parameters:
	//     bTick - If TRUE, CXTPTabManager::OnNavigateButtonClick will
	//             be called passing in the Id of the navigation
	//             button that was clicked as a parameter.
	//-----------------------------------------------------------------------
	virtual void OnExecute(BOOL bTick);

protected:
	UINT m_nID;                             // Id of the navigation button.
	XTPTabNavigateButtonFlags m_dwFlags;    // Flag that indicates when the navigation button will be drawn.
	CSize m_szButton;                       // Size of the navigation button.
	CXTPTabManager* m_pManager;             // Pointer to the TabManager the navigation buttons are drawn on.
	CRect m_rcButton;                       // Bounding rectangle of the navigation button.

	BOOL m_bEnabled;                        // TRUE if the navigation button is enables.
	BOOL m_bPressed;                        // TRUE if the navigation button is pressed\clicked.
	BOOL m_bHighlighted;                    // TRUE if the navigation button is highlighted\mouseover.

private:
	friend class CXTPTabManager;
};


//===========================================================================
// Summary:
//     CXTPTabManagerAtom is a class that has only one purpose, which
//     is to catch the event when a property of the PaintManager has changed.
//     When a property of the PaintManager has changed, the OnPropertyChanged
//     event if called. When the OnPropertyChanged is caught, Reposition
//     is called to recalculate self layout.
// Remarks:
//     This is the base class for CXTPTabManager.  When a property of the
//     CXTPTabPaintManager has changed, then OnPropertyChanged is called
//     to recalculate the layout.  The paint manager can be accessed
//     with the CXTPTabManager::GetPaintManager member.
// See Also: CXTPTabManager::GetPaintManager
//===========================================================================
class CXTPTabManagerAtom
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called when a property of the PaintManager has
	//     changed.
	// See Also: CXTPTabManager::GetPaintManager
	//-------------------------------------------------------------------------
	virtual void OnPropertyChanged()
	{
		Reposition();
	}

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabManagerAtom object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPTabManagerAtom()
	{

	}

	//-------------------------------------------------------------------------
	// Summary:
	//     This member recalculates the layout of the tab manager and
	//     then repositions itself.  This member must be overridden in
	//     derived classes.
	//-------------------------------------------------------------------------
	virtual void Reposition() = 0;
};


//===========================================================================
// Summary:
//     CXTPTabManager is a CXTPTabManagerAtom derived class.  This is the
//     base class for docking pane tabs, tab control tabs, and
//     MDI client tabs.
// See Also: CXTPTabClientWnd, CXTPDockingPaneTabbedContainer, CXTPTabControl
//===========================================================================
class _XTP_EXT_CLASS CXTPTabManager : public CXTPTabManagerAtom
{
protected:
	class CNavigateButtonArrow;
	class CNavigateButtonArrowLeft;
	class CNavigateButtonArrowRight;
	class CNavigateButtonClose;

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabManager object.
	//-------------------------------------------------------------------------
	CXTPTabManager();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabManager object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPTabManager();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves information about a tab in a tab manager.
	// Parameters:
	//     nIndex - Zero-based index of the tab.
	// Returns:
	//     Pointer to CXTPTabManagerItem if successful; NULL otherwise.
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* GetItem(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the specified item from a tab manager.
	// Parameters:
	//     nItem - Zero-based value of the item to delete.
	// Returns:
	//     Nonzero if successful; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL DeleteItem(int nItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all items from a tab manager.
	//-----------------------------------------------------------------------
	void DeleteAllItems();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the number of tabs in the tab manager.
	// Returns:
	//     Number of items in the tab manager.
	//-----------------------------------------------------------------------
	int GetItemCount() const;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the caption text of a tab.
	// Parameters:
	//     pItem - CXTPTabManagerItem tab item.
	// Returns:
	//     Caption text of the specified tab.
	//-----------------------------------------------------------------------
	virtual CString GetItemCaption(CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the HICON of a CXTPTabManagerItem.
	// Parameters:
	//     pItem - CXTPTabManagerItem to get HICON from.
	// Returns:
	//     HICON of specified CXTPTabManagerItem.
	//-----------------------------------------------------------------------
	virtual HICON GetItemIcon(CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the color of a tab button.
	// Parameters:
	//     pItem - Tab to get color from.
	// Returns:
	//     The color of the tab button.
	// See Also: CXTPTabManagerItem::SetColor, CXTPTabManagerItem::GetColor
	//-----------------------------------------------------------------------
	virtual COLORREF GetItemColor(CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the tooltip that is displayed when
	//     the mouse pointer is positioned over the specified tab button.
	// Parameters:
	//     pItem - Get tooltip of this CXTPTabManagerItem tab.
	// Returns:
	//     The tooltip that is displayed when the mouse if over the
	//     specified tab button.
	// See Also: CXTPTabManagerItem::SetTooltip, CXTPTabManagerItem::GetTooltip
	//-----------------------------------------------------------------------
	virtual CString GetItemTooltip(CXTPTabManagerItem* pItem) const;

	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called when the tab items have changed.  Tab items will
	//     change after an item is added, deleted, or reordered.
	// Remarks:
	//     This member will recalculate the index for all items, then call
	//     Reposition.
	// See Also: CXTPTabManager::AddItem, CXTPTabManager::ReOrder, CXTPTabManager::DeleteItem,
	//           CXTPTabManager::DeleteAllItems
	//-------------------------------------------------------------------------
	virtual void OnItemsChanged();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to select a tab by its index.  A selected tab
	//     if the currently active tab.
	// Parameters:
	//     nIndex - Index of the tab to select.
	// Remarks:
	//     This member will set the current CXTPTabManagerItem by its index
	//     within the collection of items.
	//
	//           SetSelectedItem will select the specified CXTPTabManagerItem.
	//
	//           NOTE: SetCurSel will call SetSelectedItem after it gets a
	//                 pointer to the specified item.
	// See Also: CXTPTabManagerItem, GetCurSel, SetSelectedItem, GetSelectedItem
	//-----------------------------------------------------------------------
	void SetCurSel(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the index of the currently selected
	//     tab.
	// Remarks:
	//     Gets the index of m_pSelected, which is the currently selected
	//     tab.  The currently selected tab can be retrieved with the
	//     GetSelectedItem member.
	// Returns:
	//     Index of the currently selected tab.
	// See Also: CXTPTabManagerItem, SetSelectedItem, SetCurSel, GetSelectedItem
	//-----------------------------------------------------------------------
	int GetCurSel();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to select a CXTPTabManagerItem tab.  A selected tab
	//     if the currently active tab.
	// Parameters:
	//     pItem - CXTPTabManagerItem tab to select.
	// Remarks:
	//     This member will will select the specified CXTPTabManagerItem.
	//
	//           SetCurSel will set the current CXTPTabManagerItem by its index
	//           within the collection of items.
	//
	//           A tab is selected when it has focus or is clicked.
	// See Also: CXTPTabManagerItem, GetCurSel, SetCurSel, GetSelectedItem
	//-----------------------------------------------------------------------
	virtual void SetSelectedItem(CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the currently selected tab.
	// Remarks:
	//     A tab is selected when it has focus or is clicked.
	// Returns:
	//     Pointer to the currently selected CXTPTabManagerItem tab.
	// See Also: CXTPTabManagerItem, SetSelectedItem, SetCurSel, GetCurSel
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* GetSelectedItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to determine is a CXTPTabManagerItem tab
	//     is at a specific x and y coordinate.  This can be used to
	//     determine if the user clicked on a tab.
	// Parameters:
	//     point - CPoint to test.
	// Remarks:
	//     The tab must both be visible and enabled or NULL will be
	//     returned.
	// Returns:
	//     If a CXTPTabManagerItem is at the specified point, then the
	//     tab at the point is returned, otherwise NULL is returned.
	// See Also: PerformClick, PerformMouseMove
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* HitTest(CPoint point) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the minimum, maximum, and normal lengths
	//     of tab buttons.  These are the values used when calculating the lengths of
	//     tabs for each XTPTabLayoutStyle.
	// Parameters:
	//     szNormal - Used only when XTPTabLayoutStyle = xtpTabLayoutFixed.
	//                All tabs will have a length of szNormal with this layout.
	//                For all other layouts, szMin and szMax are used as bounds
	//                for the available length of a single tab.
	//     szMin    - Minimum length of a single tab when layout is not
	//                xtpTabLayoutFixed.
	//     szMax    - Maximum length of a single tab when layout is not
	//                xtpTabLayoutFixed.
	// Remarks:
	//     szNormal is only used to size tabs when the layout is
	//     xtpTabLayoutFixed.
	//     szMin and szMax are used to calculate the length of tabs for
	//     all other layouts.  For example, SetItemMetrics might be called
	//     to set the maximum available length for a tab when using the
	//     xtpTabLayoutSizeToFit layout.
	// See Also: XTPTabLayoutStyle, SetItemMetrics, SetLayoutStyle
	//-----------------------------------------------------------------------
	virtual void GetItemMetrics(CSize* szNormal, CSize* szMin = NULL, CSize* szMax = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the minimum, maximum, and normal lengths
	//     of tab buttons.  Each is used when calculating the lengths of
	//     tabs for each XTPTabLayoutStyle.
	// Parameters:
	//     szNormal - Used only when XTPTabLayoutStyle = xtpTabLayoutFixed.
	//                All tabs will have a length of szNormal with this layout.
	//                For all other layouts, szMin and szMax are used as bounds
	//                for the available length of a single tab.
	//     szMin    - Minimum length of a single tab when layout is not
	//                xtpTabLayoutFixed.
	//     szMax    - Maximum length of a single tab when layout is not
	//                xtpTabLayoutFixed.
	// Remarks:
	//     szNormal is only used to size tabs when the layout is
	//     xtpTabLayoutFixed.
	//     szMin and szMax are used to calculate the length of tabs for
	//     all other layouts.  For example, SetItemMetrics might be called
	//     to set the maximum available length for a tab when using the
	//     xtpTabLayoutSizeToFit layout.
	// See Also: XTPTabLayoutStyle, GetItemMetrics, SetLayoutStyle
	//-----------------------------------------------------------------------
	void SetItemMetrics(CSize szNormal, CSize szMin = CSize(0, 0), CSize szMax = CSize(0, 0));

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to reorder items.
	// Parameters:
	//     pItem  - Item you want to reorder.
	//     nIndex - New item position to be set.
	//-----------------------------------------------------------------------
	void MoveItem(CXTPTabManagerItem* pItem, int nIndex);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the total width of all the tab buttons.
	// Returns:
	//     Total width of all the tab buttons.
	// Remarks:
	//     The width might change depending on the tab layout and
	//     position of the tabs.
	// See Also: XTPTabLayoutStyle, XTPTabPosition
	//-----------------------------------------------------------------------
	int GetItemsLength();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to get the length of the bounding
	//     rectangle of the tab client header area.  This is the area that
	//     has all of the tab buttons.
	// Parameters:
	//     rc - Bounding rectangle of tab client header area
	// Returns:
	//     If tabs are horizontal, then the width of the tab client header
	//     area is returned.  If the tabs are vertical, then the height of
	//     the tab client header area is returned.
	// Remarks:
	// See Also: GetAppearanceSet, CAppearanceSet::GetHeaderRect
	//-----------------------------------------------------------------------
	int GetRectLength(CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get a pointer to one of the tab
	//     navigation buttons.  Buttons include the left arrow, right
	//     arrow, and close buttons in the tab header area.
	// Parameters:
	//     nID - Id of the button, can be one of the values listed in the
	//           Remarks section.
	// Remarks:
	//     Standard ids of buttons are listed below:
	//     * <b>xtpTabNavigateButtonLeft</b>  Left tab navigation button.
	//     * <b>xtpTabNavigateButtonRight</b> Right tab navigation button.
	//     * <b>xtpTabNavigateButtonClose</b> Close tab navigation button.
	// Returns:
	//     Pointer to the specified navigation button.
	// See Also:
	//     XTPTabNavigateButton
	//-----------------------------------------------------------------------
	CXTPTabManagerNavigateButton* FindNavigateButton(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the tabs are positioned
	//     horizontally.
	// Returns:
	//     TRUE if the tabs are located at the top or bottom, FALSE if
	//     the tabs are on the left or right.
	// See Also: GetPosition, XTPTabPosition
	//-----------------------------------------------------------------------
	BOOL IsHorizontalPosition();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the scroll offset of the tab buttons in the
	//     tab header.  This is how much the tabs have been scrolled by
	//     pressing the left "<" and right ">" tab navigation buttons.
	// Returns:
	//     The scroll offset of the tab buttons in the tab header.
	// Remarks:
	//     The scroll offset starts at zero.  The offset will remain at
	//     zero until there are enough tabs to scroll.  If the tabs are scrolled,
	//     the offset will be a negative number indicating how much the tabs
	//     have been scrolled to the right.
	//
	//          NOTE: GetHeaderOffset and SetHeaderOffset are used when the
	//                left and right tab navigate buttons are pressed to scroll
	//                the tabs by 20 pixels to the left or right.
	//
	//                EnsureVisible uses GetHeaderOffset and SetHeaderOffset to
	//                ensure that a tab button is visible.
	//
	// Example:
	//          This sample illustrate how to programmatically set the scroll offset
	//          of the tabs in the tab header of the CXTPTabClientWnd.
	// <code>
	// //Scroll the tabs in the tab header to the right by 100 pixels.
	// m_MTIClientWnd.GetWorkspace(0)->SetHeaderOffset((m_MTIClientWnd.GetWorkspace(0)->GetHeaderOffset()) - 100);
	//
	// //Scroll the tabs in the tab header to the left by 100 pixels.//
	// m_MTIClientWnd.GetWorkspace(0)->SetHeaderOffset((m_MTIClientWnd.GetWorkspace(0)->GetHeaderOffset()) + 100);
	// </code>
	// See Also: SetHeaderOffset, CXTPTabManagerNavigateButton, EnsureVisible
	//-----------------------------------------------------------------------
	int GetHeaderOffset();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to adjust the scroll offset of the tabs in the
	//     tab header.
	// Parameters:
	//     nOffset - Scroll offset, number must be <= 0.  You should
	//               use GetHeaderOffset to get the current offset and add
	//               or subtract to move left and right.  If 0, then the
	//               tabs will be reset to there starting position.
	// Remarks:
	//     The scroll offset starts at zero.  The offset will remain at
	//     zero until there are enough tabs to scroll.  If the tabs are scrolled,
	//     the offset will be a negative number indicating how much the tabs
	//     have been scrolled to the right.
	//
	//          NOTE: GetHeaderOffset and SetHeaderOffset are used when the
	//                left and right tab navigate buttons are pressed to scroll
	//                the tabs by 20 pixels to the left or right.
	//
	//                EnsureVisible uses GetHeaderOffset and SetHeaderOffset to
	//                ensure that a tab button is visible.
	//
	// Example:
	//     This sample illustrate how to programmatically set the scroll offset
	//     of the tabs in the tab header of the CXTPTabClientWnd.
	// <code>
	// //Scroll the tabs in the tab header to the right by 100 pixels.
	// m_MTIClientWnd.GetWorkspace(0)->SetHeaderOffset((m_MTIClientWnd.GetWorkspace(0)->GetHeaderOffset()) - 100);
	//
	// //Scroll the tabs in the tab header to the left by 100 pixels.//
	// m_MTIClientWnd.GetWorkspace(0)->SetHeaderOffset((m_MTIClientWnd.GetWorkspace(0)->GetHeaderOffset()) + 100);
	// </code>
	// See Also: GetHeaderOffset, CXTPTabManagerNavigateButton, EnsureVisible
	//-----------------------------------------------------------------------
	void SetHeaderOffset(int nOffset);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to ensure that a tab is visible.
	// Parameters:
	//     pItem - CXTPTabManagerItem tab to make visible.
	// See Also: SetHeaderOffset, GetHeaderOffset
	//-----------------------------------------------------------------------
	void EnsureVisible(CXTPTabManagerItem* pItem);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get a pointer to the tab paint manager.
	//     The tab paint manager is used to customize the appearance of
	//     CXTPTabManagerItem objects and the tab manager.  I.e. Tab colors,
	//     styles, etc...  This member must be overridden in
	//     derived classes.
	// Returns:
	//     Pointer to CXTPTabPaintManager that contains the visual elements
	//     of the tabs.
	//-----------------------------------------------------------------------
	virtual CXTPTabPaintManager* GetPaintManager() = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set a CXTPTabPaintManager object to the
	//     tab manager.
	// Parameters:
	//     pPaintManager - Pointer to a CXTPTabPaintManager object.
	// Remarks:
	//     The tab paint manager is used to customize the appearance of
	//     CXTPTabManagerItem objects and the tab manager.
	//-----------------------------------------------------------------------
	virtual void SetPaintManager(CXTPTabPaintManager* pPaintManager);

	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called when visual properties of the tabs are
	//     changed.  For example, color, mouse-over, and on-click. This member
	//     must be overridden in derived classes.
	//-------------------------------------------------------------------------
	virtual void RedrawControl() = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if tab can be reordered.
	// Returns:
	//     TRUE id tabs can be reordered, FALSE if tabs cannot be reordered.
	// See Also: SetAllowReorder
	//-----------------------------------------------------------------------
	virtual BOOL IsAllowReorder();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether the user can reorder the
	//     tabs.
	// Parameters:
	//     bAllowReorder - TRUE to allow tabs to be reordered, FALSE
	//                     to disallow tab reordering.
	// See Also: IsAllowReorder
	//-----------------------------------------------------------------------
	virtual void SetAllowReorder(BOOL bAllowReorder);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if a static frame is used.
	// Returns:
	//     TRUE if a static frame is used, FALSE otherwise.
	// Remarks:
	//     If TRUE, a static frame will be drawn around the entire tab
	//     manager control.  The frame will be drawn around both the
	//     tab header and tab client.
	// See Also: CXTPTabPaintManager::m_bStaticFrame
	//-----------------------------------------------------------------------
	virtual BOOL IsDrawStaticFrame();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the icon of the tab needs to be
	//     drawn.
	// Parameters:
	//     pDC    - Pointer to the destination device context.
	//     pt     - Specifies the location of the image.
	//     pItem  - CXTPTabManagerItem object to draw icon on.
	//     bDraw  - TRUE if the icon needs to be drawn, I.e. the icon size
	//              changed.  FALSE if the icon does not need to be
	//              drawn or redrawn.
	//     szIcon - Size of the tab icon.
	// Remarks:
	//     For example, on mouse-over.  This member is overridden by its
	//     descendants.  This member must be overridden in
	//     derived classes.
	// Returns:
	//     TRUE if the icon was successfully drawn, FALSE if the icon
	//     was not drawn.
	//-----------------------------------------------------------------------
	virtual BOOL DrawIcon(CDC* pDC, CPoint pt, CXTPTabManagerItem* pItem, BOOL bDraw, CSize& szIcon) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable Windows XP themed back color.
	// Parameters:
	//     hWnd    - The window handle of the target object or dialog.
	//     bEnable - TRUE to enable Windows XP themed back color, FALSE to
	//               disable Windows XP themed back color.
	// Remarks:
	//     This should only be used when the tab color is set to xtpTabColorWinXP.
	// Example:
	//     This sample code illustrates how to apply a Windows XP themed back color to a tab control.
	// <code>
	// m_wndTabControl.GetPaintManager()->SetColor(xtpTabColorWinXP);
	// XTPTabColorStyle color = m_wndTabControl.GetPaintManager()->GetColor();
	// m_wndTabControl.EnableTabThemeTexture(m_wndTabControl.GetSafeHwnd(), color == xtpTabColorWinXP ? TRUE : FALSE);
	// RedrawWindow();
	// </code>
	// See Also: XTPTabColorStyle, GetColor, SetColor
	//-----------------------------------------------------------------------
	void EnableTabThemeTexture(HWND hWnd, BOOL bEnable = TRUE);


public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to apply one of the "built-in" color sets to
	//     the tabs.
	// Parameters:
	//     tabColor - XTPTabColorStyle to apply to the tabs. Can be one of the
	//                values listed in the remarks section.
	// Remarks:
	//     tabColor can be one of the following:
	//         * <b>xtpTabColorDefault</b>      Tabs will use the default color for the currently set Appearance.
	//         * <b>xtpTabColorVisualStudio</b> Tabs will use the Visual Studio color style for the currently set Appearance.
	//         * <b>xtpTabColorOffice2003</b>   Tabs will use the Office 2003 color style for the currently set Appearance.
	//         * <b>xtpTabColorWinXP</b>        Tabs will use the Windows XP color style for the currently set Appearance.
	//         * <b>xtpTabColorWhidbey</b>      Tabs will use the Visual Studio 2005 "Whidbey" color style for the currently set Appearance.
	// Returns:
	//     Pointer to the CXTPTabPaintManager::CColorSet object applied.
	// See Also:
	//     XTPTabColorStyle, SetColorSet, GetColor, GetColorSet, XTPTabAppearanceStyle,
	//     SetAppearanceSet, SetAppearance, GetAppearance, GetAppearanceSet
	//-----------------------------------------------------------------------
	CXTPTabPaintManager::CColorSet* SetColor(XTPTabColorStyle tabColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to apply a custom color set to the tabs.
	// Parameters:
	//     pColorSet - Pointer to custom CXTPTabPaintManager::CColorSet
	//                 appearance set.
	// Returns:
	//     Pointer to the newly set custom color set.
	// See Also:
	//     XTPTabColorStyle, SetColor, GetColor, GetColorSet, XTPTabAppearanceStyle,
	//     SetAppearanceSet, SetAppearance, GetAppearance, GetAppearanceSet
	//-----------------------------------------------------------------------
	CXTPTabPaintManager::CColorSet* SetColorSet(CXTPTabPaintManager::CColorSet* pColorSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to apply one of the "built-in" appearances to
	//     the tabs.
	// Parameters:
	//     tabAppearance - XTPTabAppearanceStyle to apply to the tabs. Can be
	//                     one of the values listed in the Remarks section.
	// Returns:
	//     Pointer to the CXTPTabPaintManager::CAppearanceSet object applied.
	// Remarks:
	//     Use SetAppearance to apply a "Built-In" theme such as
	//     xtpTabAppearanceExcel.  To apply a custom them, use the
	//     SetAppearanceSet member.
	//
	//     tabAppearance can be one of the following:
	//         * <b>xtpTabAppearancePropertyPage</b>         Gives your tabs an Office 2000 appearance.
	//         * <b>xtpTabAppearancePropertyPageSelected</b> Gives your tabs an Office 2000 selected appearance.
	//         * <b>xtpTabAppearancePropertyPageFlat</b>     Gives your tabs an Office 2000 Flat appearance.
	//         * <b>xtpTabAppearancePropertyPage2003</b>     Gives your tabs an Office 2003 appearance.
	//         * <b>xtpTabAppearanceStateButtons</b>         Gives your tabs a State Button appearance.
	//         * <b>xtpTabAppearanceVisualStudio</b>         Gives your tabs a Visual Studio appearance.
	//         * <b>xtpTabAppearanceFlat</b>                 Gives your tabs Flat appearance.
	//         * <b>xtpTabAppearanceExcel</b>                Gives your tabs an Excel appearance.
	//         * <b>xtpTabAppearanceVisio</b>                Gives your tabs a Visio appearance.   // See Also:
	//     XTPTabAppearanceStyle, SetAppearanceSet, GetAppearance, GetAppearanceSet,
	//     SetColor, GetColor, GetColorSet, SetColorSet
	//-----------------------------------------------------------------------
	CXTPTabPaintManager::CAppearanceSet* SetAppearance(XTPTabAppearanceStyle tabAppearance);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to apply a custom appearance set.
	// Parameters:
	//     pAppearanceSet - Pointer to custom CXTPTabPaintManager::CAppearanceSet
	//                      appearance set.
	// Remarks:
	//     An appearance set specifies how the tabs will look.  This
	//     is the actual physical structure of the tab buttons.  For example,
	//     tab buttons with an Excel appearance style will have a
	//     half hexagon shape.  SetColorSet is used to set custom
	//     colors.
	//
	//          See the GUI_Eclipse sample for an example of how to create
	//          a custom appearance set and apply it to the tabs.
	// Returns:
	//     Pointer to newly set custom appearance set.
	// See Also:
	//     XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
	//     SetColor, GetColor, GetColorSet, SetColorSet
	//-----------------------------------------------------------------------
	CXTPTabPaintManager::CAppearanceSet* SetAppearanceSet(CXTPTabPaintManager::CAppearanceSet* pAppearanceSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the currently set custom appearance set.
	// Returns:
	//     Pointer to the currently used custom appearance set.
	// Remarks:
	//     An appearance set specifies how the tabs will look.  This
	//     is the actual physical structure of the tab buttons.  For example,
	//     tab buttons with an Excel appearance style will have a
	//     half hexagon shape.  SetColorSet is used to set
	//     custom colors.
	//
	//          See the GUI_Eclipse sample for an example of how to create
	//          a custom appearance set and apply it to the tabs.
	// See Also: GetAppearanceSet, SetAppearanceSet, SetAppearance
	//-----------------------------------------------------------------------
	CXTPTabPaintManager::CAppearanceSet* GetAppearanceSet();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the currently used custom color set.
	// Returns:
	//     Currently used custom color set.
	// See Also: XTPTabColorStyle, GetColor, SetColorSet, SetColor
	//-----------------------------------------------------------------------
	CXTPTabPaintManager::CColorSet* GetColorSet();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine the currently set appearance style.
	// Returns:
	//     The currently set XTPTabAppearanceStyle.
	// Remarks:
	//     The appearance is the physical structure of the tab buttons.
	// See Also: XTPTabAppearanceStyle, GetAppearanceSet, SetAppearanceSet, SetAppearance
	//-----------------------------------------------------------------------
	XTPTabAppearanceStyle GetAppearance();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the currently used tab color style.
	// Returns:
	//     Currently used color style.
	// Remarks:
	//     Color styles are used to colorize the tabs.  For example,
	//     applying the xtpTabColorWinXP color style will give the
	//     tabs Windows XP colorization.
	// See Also: XTPTabColorStyle, SetColor, GetColorSet, SetColorSet
	//-----------------------------------------------------------------------
	XTPTabColorStyle GetColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the position of the tabs.
	// Parameters:
	//     tabPosition - New XTPTabPosition to apply to tab manager. Can be one
	//                   of the values listed in the remarks section.
	// Remarks:
	//     The position refers to where the tabs are located in relation
	//     to the TabClient.  The tabs can be positioned on the
	//     top, bottom, left, or right side of the TabClient.
	//
	//     tabPosition can be one of the following:
	//         * <b>xtpTabPositionTop</b>    Tabs will be drawn on the Top.
	//         * <b>xtpTabPositionLeft</b>   Tabs will be drawn on the Left.
	//         * <b>xtpTabPositionBottom</b> Tabs will be drawn on the Bottom.
	//         * <b>xtpTabPositionRight</b>  Tabs will be drawn on the Right.
	// See Also: XTPTabPosition, GetPosition
	//-----------------------------------------------------------------------
	void SetPosition(XTPTabPosition tabPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the tab layout style.
	// Parameters:
	//     tabLayout - New XTPTabLayoutStyle to apply to the tabs. Can be one
	//                 of the values listed in the Remarks section.
	// Remarks:
	//     The layout refers to how the tabs are sized within the tab
	//     header.  Tabs can be in a compressed, fixed, auto size, or
	//     sized-to-fit layout.
	//
	//     tabLayout can be one of the following:
	//         * <b>xtpTabLayoutAutoSize</b>   Tabs will be automatically sized based on the caption and image size.  With this flag set, tabs will appear in their normal size.
	//         * <b>xtpTabLayoutSizeToFit</b>  Tabs are sized to fit within the tab panel.  All tabs will be compressed and forced to fit into the tab panel.
	//         * <b>xtpTabLayoutFixed</b>      All tabs will be set to a fixed size within the tab panel.
	//         * <b>xtpTabLayoutCompressed</b> Tabs will be compressed within the tab panel.  This will compress the size of the tabs, but all tabs will not be forced into the tab panel.
	// See Also: XTPTabLayoutStyle, GetLayout
	//-----------------------------------------------------------------------
	void SetLayoutStyle(XTPTabLayoutStyle tabLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the current position of the tabs.
	// Returns:
	//     Current XTPTabPosition of the tabs.
	// Remarks:
	//     The position refers to where the tabs are located in relation
	//     to the TabClient.  The tabs can be positioned on the
	//     top, bottom, left, or right side of the TabClient.
	// See Also: XTPTabPosition, SetPosition
	//-----------------------------------------------------------------------
	virtual XTPTabPosition GetPosition();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the currently used tab layout style.
	// Returns:
	//     Currently set tab layout style.
	// Remarks:
	//     The layout refers to how the tabs are sized within the tab
	//     header.  Tabs can be in a compressed, fixed, auto size, or
	//     sized-to-fit layout.
	// See Also: XTPTabLayoutStyle, SetLayoutStyle
	//-----------------------------------------------------------------------
	XTPTabLayoutStyle GetLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to hide or show tab icons.
	// Parameters:
	//     bShowIcons - TRUE to show icons in the tabs, FALSE to hide
	//                  tab icons.
	//-----------------------------------------------------------------------
	void ShowIcons(BOOL bShowIcons);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to make a MDITabClient tab group active.
	//     SetActive works only for tab groups in the MDITabClient when group
	//     is enabled. Only one group can  be active.
	// Parameters:
	//     bActive - TRUE to make this tab group active, FALSE to make
	//               this tab group inactive.
	// Remarks:
	//     When grouping is enabled, each tab group has its own CXTPTabManager.
	//     SetActive is used to activate\set focus to the tab group.
	//
	//          NOTE: This only applies to tab groups in the MDITabClient, this
	//          is the tabs that appear in the CommandBars TabWorkspace.
	// See Also: IsActive, CXTPTabClientWnd::Attach
	//-----------------------------------------------------------------------
	void SetActive(BOOL bActive);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if a group of tabs in the
	//     MDITabClient is active.  This only applies to groups of tabs
	//     in the TabWorkspace with grouping enabled.
	//     enabled
	// Returns:
	//     TRUE if this group of tabs is active, FALSE otherwise.
	// Remarks:
	//     When grouping is enabled, each tab group has its own CXTPTabManager.
	//     SetActive is used to activate\set focus to the tab group.
	//
	//          NOTE: This only applies to tab groups in the MDITabClient, this
	//          is the tabs that appear in the CommandBars TabWorkspace.
	// See Also: SetActive, CXTPTabClientWnd::Attach
	//-----------------------------------------------------------------------
	BOOL IsActive();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the bounding rectangle to the tab
	//     manager control area.
	// Returns:
	//     Bounding rectangle of tab manager control area.
	// See Also: GetHeaderRect, GetClientRect
	//-----------------------------------------------------------------------
	CRect GetControlRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the bounding rectangle to the tab
	//     client area.
	// Returns:
	//     Bounding rectangle of tab client area.
	// See Also: GetHeaderRect, GetControlRect
	//-----------------------------------------------------------------------
	CRect GetClientRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the bounding rectangle to the tab
	//     client header area.
	// Returns:
	//     Bounding rectangle of tab header area.
	// See Also: GetClientRect, GetControlRect
	//-----------------------------------------------------------------------
	CRect GetHeaderRect();


protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when a tab button is clicked.  This
	//     will select the tab that was clicked.
	// Parameters:
	//     pItem - Pointer to the CXTPTabManagerItem tab that was clicked.
	// See Also: PerformClick
	//-----------------------------------------------------------------------
	virtual void OnItemClick(CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when a CXTPTabManagerItem is clicked
	//     and dragged within the tab header.  This will reorder the
	//     selected tab to the location it is dragged to.
	// Parameters:
	//     hWnd  - Handle to the CWnd object beneath the mouse cursor.
	//     pt   - Specifies the x- and y-coordinate of the cursor. These
	//            coordinates are always relative to the upper-left
	//            corner of the window.
	//     pItem - Pointer to the CXTPTabManagerItem tab that is being
	//             clicked and dragged.
	// See Also: PerformClick
	//-----------------------------------------------------------------------
	virtual void ReOrder(HWND hWnd, CPoint pt, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when a tab navigation button is clicked.
	// Parameters:
	//     nID - Id of the button, can be one of the values listed in the
	//           Remarks section.
	// Remarks:
	//     Standard ids of buttons are listed below:
	//     * <b>xtpTabNavigateButtonLeft</b>  Left tab navigation button.
	//     * <b>xtpTabNavigateButtonRight</b> Right tab navigation button.
	//     * <b>xtpTabNavigateButtonClose</b> Close tab navigation button.
	//
	//     This method is overridden in CXTPTabClientWnd::CWorkspace and
	//     CXTPTabControl to capture when the tab navigation close "x"
	//     button was pressed and deletes the tab.
	//
	//     This member is called after CXTPTabManagerNavigateButton::PerformClick.
	// See Also:
	//     CXTPTabManagerNavigateButton::PerformClick, XTPTabNavigateButton
	//-----------------------------------------------------------------------
	virtual void OnNavigateButtonClick(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to determine if the MouseMove event
	//     have been disabled.  This member is overridden in
	//     CXTPTabClientWnd::CWorkspace.
	// Returns:
	//     Always returns FALSE in the base class.
	// Remarks:
	//     The MouseMove event is disabled when a command bar popup
	//     becomes active (visible).
	// See Also: CXTPTabClientWnd::CWorkspace::IsMouseLocked
	//-----------------------------------------------------------------------
	virtual BOOL IsMouseLocked();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to add a new or existing CXTPTabManagerItem
	//     to the collection of tabs for this tab manager.
	// Parameters:
	//     nItem - Position in the collection of CXTPTabManagerItem tabs
	//             to add or insert the CXTPTabManagerItem.  If the
	//             position specified is less than 0 or greater than
	//             the amount of tabs in the collection, then it will
	//             be added to the end of the collection.
	//     pItem - CXTPTabManagerItem to insert into the collection of
	//             tabs.  If NULL, then a new CXTPTabManagerItem will
	//             be created and inserted at the specified position.
	// Returns:
	//     Pointer to the newly added CXTPTabManagerItem.
	// Remarks:
	//     This member is overridden in its derives classes to provide
	//     additional functionality, then this base class member is called.
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* AddItem(int nItem, CXTPTabManagerItem* pItem = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the mouse cursor moves.
	// Parameters:
	//     hWnd - Handle to the CWnd object beneath the mouse cursor.
	//     pt   - Specifies the x- and y-coordinate of the cursor. These
	//            coordinates are always relative to the upper-left
	//            corner of the window.
	// Remarks:
	//     When the mouse cursor is moved across a tab and hot tracking
	//     is enabled, PerformMouseMove will highlight and un-highlight
	//     the tabs.  It will then call CXTPTabManagerNavigateButton::PerformMouseMove
	//     for each tab navigation button.
	//
	//          When the mouse cursor is moved across a tab navigation button,
	//          CXTPTabManagerNavigateButton::PerformMouseMove is called to
	//          perform actions related to mouse movement over the navigation
	//          buttons.  This will highlight and un-highlight the tab
	//          navigation buttons.
	// See Also: CXTPTabManagerNavigateButton::PerformMouseMove
	//-----------------------------------------------------------------------
	void PerformMouseMove(HWND hWnd, CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the user presses the left mouse button.
	// Parameters:
	//     hWnd - Handle to the CWnd object beneath the mouse cursor.
	//     pt   - Specifies the x- and y-coordinate of the cursor. These
	//            coordinates are always relative to the upper-left
	//            corner of the window.
	//     bNoItemClick - TRUE to only perform a click on tab navigation
	//                    buttons.  If TRUE, only the tab navigation
	//                    buttons will be tested to see if they were clicked,
	//                    the tabs will not receive the click.
	//
	//                    FALSE to test both the tab navigation buttons and
	//                    the tabs to see if they were clicked.
	//
	// Remarks:
	//     When the user clicks on a tab navigation button, this member
	//     will call the CXTPTabManagerNavigateButton::PerformClick
	//     member of the navigation button that was clicked.
	//
	//          If a tab was clicked, then the OnItemClick member is called,
	//          which will select the tab.
	//
	//          If IsAllowReorder is TRUE, then ReOrder is called before
	//          OnItemClick.  ReOrder will check to see if the tab is being
	//          moved, if it is, then the tab remains selected so OnItemClick
	//          is not called.  If the tab was not being moved, then
	//          OnItemClick is called.
	//
	// Returns:
	//     TRUE if a tab navigation button or tab was clicked, FALSE
	//     not items were clicked or if bNoItemClick is TRUE and a tab
	//     was clicked.
	// See Also: OnItemClick, ReOrder, CXTPTabManager::OnNavigateButtonClick
	//-----------------------------------------------------------------------
	BOOL PerformClick(HWND hWnd, CPoint pt, BOOL bNoItemClick = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to retrieve tooltip information about a
	//     tab and stores the information in a TOOLINFO object.
	// Parameters:
	//     hWnd  - Handle to the window that contains the tab.
	//     pItem - CXTPTabManagerItem to copy tool tip information from.
	//     pTI   - The TOOLINFO structure to copy information about a
	//             CXTPTabManagerItem.
	// Returns: -1 if a tooltip was not found for the CXTPTabManagerItem, If a
	//          tooltip was found, then the Id of the CXTPTabManagerItem is returned.
	// Remarks:
	//     This member is called in the CXTPTabControl::OnToolHitTest,
	//     CXTPDockingPaneTabbedContainer::OnToolHitTest, and
	//     CXTPTabClientWnd::OnToolHitTest members.
	//-----------------------------------------------------------------------
	INT_PTR PerformToolHitTest(HWND hWnd, CXTPTabManagerItem* pItem, TOOLINFO* pTI) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to get the total number of tab
	//     navigation buttons added to the tab header.
	// Returns:
	//     Total number of tab navigation buttons added to this
	//     tab manager.
	// See Also: GetNavigateButton, CXTPTabManagerNavigateButton, OnNavigateButtonClick
	//-----------------------------------------------------------------------
	int GetNavigateButtonCount();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member will retrieve a tab navigation button by its index.
	// Parameters:
	//     nIndex - Zero based index into the array of tab navigation buttons.
	// Remarks:
	//     Call this member function to retrieve a tab navigation button by
	//     its index in button array <i>m_arrNavigateButtons</i>.
	// Returns:
	//     A CXTPTabManagerNavigateButton object that represents the tab
	//     navigation button at the specified index.
	// See Also:
	//     GetNavigateButtonCount, CXTPTabManagerNavigateButton, OnNavigateButtonClick
	//-----------------------------------------------------------------------
	CXTPTabManagerNavigateButton* GetNavigateButton(int nIndex);


protected:
	CXTPTabManagerItem* m_pSelected;    // TRUE if tab is currently selected.  I.e. When the tab is clicked.
	CXTPTabManagerItem* m_pHighlighted; // TRUE if tab is currently highlighted.  I.e. When the mouse cursor is over the tab button.

	int m_nHeaderOffset;                // Scroll offset, this is how much the tabs in the tab header have been scrolled
	                                    // using the left and right tab navigation buttons.  See: GetHeaderOffset, SetHeaderOffset
	BOOL m_bAllowReorder;               // TRUE to allow tabs to be reordered.
	BOOL m_bActive;                     // TRUE if the MDITabClient tab group is active.  This only applies to
	                                    // a TabWorkspace when grouping is enabled.

	CSize m_szItemNormal;               // Size of tab buttons only when XTPTabLayoutStyle = xtpTabLayoutFixed.
	                                    // All tabs will have a length of szNormal with this layout.
	                                    // For all other layouts, szMin and szMax are used as bounds
	                                    // for the available length of a single tab.
	CSize m_szItemMin;                  // Minimum length of a single tab when layout is not xtpTabLayoutFixed.
	CSize m_szItemMax;                  // Maximum length of a single tab when layout is not xtpTabLayoutFixed.

	CRect m_rcHeaderRect;               // Bounding rectangle of the tab header area.
	CRect m_rcControl;                  // Bounding rectangle of the tab manager control.  This is the bounding
	                                    // rectangle for both the tab header and tab client.
	CRect m_rcClient;                   // Bounding rectangle of the tab client area.

	CArray<CXTPTabManagerItem*, CXTPTabManagerItem*> m_arrItems;                                // Collection of tabs of this tab manager.
	CArray<CXTPTabManagerNavigateButton*, CXTPTabManagerNavigateButton*> m_arrNavigateButtons;  // Collection of tab navigation buttons for this tab manager

	CXTPWinThemeWrapper m_themeTabControl;  // Internal helper for drawing XP interface parts.

private:
	int m_nItemColor;

	friend class CXTPTabManagerItem;
	friend class CXTPTabPaintManager;
	friend class CXTPTabManagerNavigateButton;
	friend class CXTPTabClientWnd;


};


//////////////////////////////////////////////////////////////////////////
// CXTPTabManagerItem

AFX_INLINE CXTPTabManager* CXTPTabManagerItem::GetTabManager() {
	return m_pTabManager;
}
AFX_INLINE BOOL CXTPTabManagerItem::IsSelected() {
	return m_pTabManager->m_pSelected == this;
}
AFX_INLINE BOOL CXTPTabManagerItem::IsHighlighted() {
	return m_pTabManager->m_pHighlighted == this;
}
AFX_INLINE CRect CXTPTabManagerItem::GetRect() {
	return m_rcItem;
}
AFX_INLINE void CXTPTabManagerItem::SetRect(CRect rcItem) {
	m_rcItem = rcItem;
}
AFX_INLINE int CXTPTabManagerItem::GetIndex() {
	return m_nIndex;
}
AFX_INLINE DWORD_PTR CXTPTabManagerItem::GetData() {
	return m_dwData;
}
AFX_INLINE void CXTPTabManagerItem::SetData(DWORD_PTR dwData) {
	m_dwData = dwData;
}
AFX_INLINE int CXTPTabManagerItem::GetImageIndex() {
	return m_nImage;
}
AFX_INLINE void CXTPTabManagerItem::SetImageIndex(int nImage) {
	m_nImage = nImage;
	m_pTabManager->Reposition();
}
AFX_INLINE int CXTPTabManagerItem::GetLength() {
	return m_nLength;
}
AFX_INLINE void CXTPTabManagerItem::SetTooltip(LPCTSTR lpszTooltip) {
	m_strToolTip = lpszTooltip;
}

//////////////////////////////////////////////////////////////////////////
// CXTPTabManagerNavigateButton

AFX_INLINE void CXTPTabManagerNavigateButton::SetFlags(XTPTabNavigateButtonFlags dwFlags) {
	m_dwFlags = dwFlags;
}
AFX_INLINE XTPTabNavigateButtonFlags CXTPTabManagerNavigateButton::GetFlags() {
	return m_dwFlags;
}
AFX_INLINE BOOL CXTPTabManagerNavigateButton::IsEnabled() {
	return m_bEnabled;
}
AFX_INLINE BOOL CXTPTabManagerNavigateButton::IsPressed() {
	return m_bPressed;
}
AFX_INLINE BOOL CXTPTabManagerNavigateButton::IsHighlighted() {
	return m_bHighlighted;
}
AFX_INLINE CRect CXTPTabManagerNavigateButton::GetRect() {
	return m_rcButton;
}
AFX_INLINE BOOL CXTPTabManagerNavigateButton::IsVisible() {
	return !m_rcButton.IsRectEmpty();
}

//////////////////////////////////////////////////////////////////////////
// CXTPTabManager

AFX_INLINE CXTPTabPaintManager::CAppearanceSet* CXTPTabManager::GetAppearanceSet() {
	return GetPaintManager()->GetAppearanceSet();
}
AFX_INLINE CXTPTabPaintManager::CColorSet* CXTPTabManager::GetColorSet() {
	return GetPaintManager()->GetColorSet();
}
AFX_INLINE XTPTabAppearanceStyle CXTPTabManager::GetAppearance() {
	return GetPaintManager()->GetAppearance();
}
AFX_INLINE XTPTabColorStyle CXTPTabManager::GetColor() {
	return GetPaintManager()->GetColor();
}
AFX_INLINE int CXTPTabManager::GetItemCount() const {
	return (int)m_arrItems.GetSize();
}
AFX_INLINE CXTPTabManagerItem* CXTPTabManager::GetItem(int nIndex) const{
	return nIndex >= 0 && nIndex < GetItemCount()? m_arrItems[nIndex]: NULL;
}
AFX_INLINE int CXTPTabManager::GetNavigateButtonCount() {
	return (int)m_arrNavigateButtons.GetSize();
}
AFX_INLINE CXTPTabManagerNavigateButton* CXTPTabManager::GetNavigateButton(int nIndex) {
	return nIndex >= 0 && nIndex < GetNavigateButtonCount()? m_arrNavigateButtons[nIndex]: NULL;
}
AFX_INLINE CXTPTabManagerItem* CXTPTabManager::GetSelectedItem() {
	return m_pSelected;
}
AFX_INLINE BOOL CXTPTabManager::IsHorizontalPosition()
{
	return (GetPosition() == xtpTabPositionBottom || GetPosition() == xtpTabPositionTop);
}
AFX_INLINE int CXTPTabManager::GetRectLength(CRect rc)
{
	if (IsHorizontalPosition()) return rc.Width();
	return rc.Height();
}
AFX_INLINE int CXTPTabManager::GetHeaderOffset()
{
	return m_nHeaderOffset;
}
AFX_INLINE void CXTPTabManager::OnNavigateButtonClick(UINT /*nID*/) {

}
AFX_INLINE BOOL CXTPTabManager::IsAllowReorder() {
	return m_bAllowReorder;
}
AFX_INLINE void CXTPTabManager::SetAllowReorder(BOOL bAllowReorder) {
	m_bAllowReorder = bAllowReorder;
}
AFX_INLINE void CXTPTabManager::SetPaintManager(CXTPTabPaintManager* /*pPaintManager*/) {

}
AFX_INLINE BOOL CXTPTabManager::IsActive() {
	return m_bActive;
}
AFX_INLINE CRect CXTPTabManager::GetControlRect() {
	return m_rcControl;
}
AFX_INLINE CRect CXTPTabManager::GetClientRect() {
	return m_rcClient;
}
AFX_INLINE CRect CXTPTabManager::GetHeaderRect() {
	return m_rcHeaderRect;
}
AFX_INLINE BOOL CXTPTabManager::IsMouseLocked() {
	return FALSE;
}

#endif // !defined(__XTPTABMANAGER_H__)
