// XTFlatTabCtrl.h interface for the CXTFlatTabCtrl class.
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
#if !defined(__XTFLATTABCTRL_H__)
#define __XTFLATTABCTRL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Theme\XTFlatTabCtrlTheme.h"

// ---------------------------------------------------------------
// Summary:
//     Enumeration used to determine arrow button display.
// Remarks:
//     XTArrowIcon type defines constants used by the
//     CXTFlatTabCtrlButtonState to determine which arrow is to be
//     displayed for a particular tab button.
// See Also:
//     CXTFlatTabCtrlButtonState, CXTFlatTabCtrl
//
// <KEYWORDS xtArrowIconLeft, xtArrowIconLeftHome, xtArrowIconRight, xtArrowIconRightHome>
// ---------------------------------------------------------------
enum XTArrowIcon
{
	xtArrowIconLeft      =   0x0200, // Left arrow display.
	xtArrowIconLeftHome  =   0x0201, // Left home arrow display.
	xtArrowIconRight     =   0x0000, // Right arrow display.
	xtArrowIconRightHome =   0x0001  // Right home arrow display.
};

//===========================================================================
// Summary:
//    The CXTFlatTabCtrlButtonState class is used by the CXTFlatTabCtrl to
//    manage navigation button information. This class contains information
//    about the state and command identifiers for the button.
//===========================================================================
class CXTFlatTabCtrlButtonState
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTFlatTabCtrlButtonState object.
	//-------------------------------------------------------------------------
	CXTFlatTabCtrlButtonState();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to initialize the information
	//     about the tab control navigation button.
	// Parameters:
	//     rect     - Size and location of the button.
	//     iCommand - Command ID for the button.
	//     iconType - Icon style of the button.
	//-----------------------------------------------------------------------
	void SetInfo(CRect rect, int iCommand, XTArrowIcon iconType);

public:
	int             m_iCommand; // Button command ID.
	bool            m_bPressed; // true if the button is pressed.
	bool            m_bEnabled; // true if the button is enabled.
	CRect           m_rect;     // Size and location of the button.
	XTArrowIcon     m_IconType; // Icon style of the button.
};

//===========================================================================
// Summary:
//     CXTFlatTabCtrl is a CWnd derived class.  It is used to create an Excel
//     style sheet control. This control allows you to define if you want the
//     control to have home, end, back, and next buttons.
//===========================================================================
class _XT_EXT_CLASS CXTFlatTabCtrl : public CWnd, public CXTThemeManagerStyleHost
{
	DECLARE_DYNAMIC(CXTFlatTabCtrl)
	DECLARE_THEME_HOST(CXTFlatTabCtrl, CXTFlatTabCtrlTheme)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTFlatTabCtrl object
	//-----------------------------------------------------------------------
	CXTFlatTabCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTFlatTabCtrl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTFlatTabCtrl();


public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the text of a specific tab.
	// Parameters:
	//     nIndex - The index of the tab whose text is to be retrieved.
	// Returns:
	//     The text of a particular tab, or NULL if an error occurs.
	//-----------------------------------------------------------------------
	LPCTSTR GetItemText(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the text of a particular tab.
	// Parameters:
	//     nIndex  - The index of the tab whose text is to be changed.
	//     pszText - The new title for the tab.
	// Returns:
	//     true when successful.
	//-----------------------------------------------------------------------
	bool SetItemText(int nIndex,LPCTSTR pszText);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets a CWnd pointer to the window that is associated
	//     with a specific tab.
	// Parameters:
	//     nIndex - The index of the tab whose managed window is to be retrieved.
	// Returns:
	//     A pointer to the window that is associated with a tab, or it returns NULL
	//     if no window is associated with (managed by) the tab.
	//-----------------------------------------------------------------------
	CWnd *GetItemWindow(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the fonts to be used by the tab control.
	// Parameters:
	//     pNormFont - Represents the font used by non-selected tabs.
	//     pBoldFont - Represents the font used by selected tabs.
	//-----------------------------------------------------------------------
	virtual void SetTabFonts(CFont* pNormFont,CFont* pBoldFont);

	// ---------------------------------------------------------------------
	// Summary:
	//     This member function will insert a tab into the flat tab control.
	// Parameters:
	//     nItem -        Index of the tab to insert.
	//     nTextID -      String resource ID of the tab label.
	//     lpszItem -     NULL terminated string that represents the tab label.
	//     pWndControl -  Optional pointer to the managed control.
	// Returns:
	//     The index of the tab that has been inserted if successful,
	//     \otherwise -1.
	// ---------------------------------------------------------------------
	virtual int InsertItem(int nItem,UINT nTextID,CWnd *pWndControl = NULL);
	virtual BOOL InsertItem(int nItem,LPCTSTR lpszItem,CWnd *pWndControl = NULL); //<combine CXTFlatTabCtrl::InsertItem@int@UINT@CWnd *>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will delete the tab specified by 'nItem' from the
	//     tab control.
	// Parameters:
	//     nItem - Index of the tab to delete.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DeleteItem(int nItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will remove all of the tabs from the tab control.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DeleteAllItems();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will retrieve the size of the tab specified by
	//     'nItem'.
	// Parameters:
	//     nItem  - Index of the tab to retrieve the size of.
	//     lpRect - Points to a RECT structure to receive the size of the tab.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL GetItemRect(int nItem,LPRECT lpRect);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to determine which tab, if any, is at the
	//     specified screen position.
	// Parameters:
	//     pHitTestInfo - Pointer to a TCHITTESTINFO structure, as
	//                    described in the Platform SDK, which specifies
	//                    the screen position to test.
	// Returns:
	//     The zero-based index of the tab, or returns -1 if no tab is at
	//     the specified position.
	//-----------------------------------------------------------------------
	virtual int HitTest(TCHITTESTINFO *pHitTestInfo) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to retrieve the currently selected tab in a flat
	//     tab control.
	// Returns:
	//     A zero-based index of the selected tab if successful, or returns
	//     -1 if no tab is selected.
	//-----------------------------------------------------------------------
	virtual int GetCurSel() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function selects a tab in a flat tab control.
	// Parameters:
	//     nItem - The zero-based index of the item to be selected.
	// Returns:
	//     A zero-based index of the previously selected tab if successful,
	//     otherwise returns -1.
	//-----------------------------------------------------------------------
	virtual int SetCurSel(int nItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to retrieve the number of tabs in the tab
	//     control.
	// Returns:
	//     The number of items in the tab control.
	//-----------------------------------------------------------------------
	virtual int GetItemCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the tooltip for the tab specified
	//     by 'nItem'.
	// Parameters:
	//     nItem      - The zero-based index of tab to receive the tooltip text.
	//     lpszTabTip - A pointer to a string containing the tooltip text.
	//-----------------------------------------------------------------------
	virtual void SetTipText(int nItem,LPCTSTR lpszTabTip);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will get the tooltip text associated with the
	//     tab specified by 'nItem'.
	// Parameters:
	//     nItem - The zero-based index of the tab to retrieve the tooltip
	//             text for.
	// Returns:
	//     A CString object containing the text to be used in the tooltip.
	//-----------------------------------------------------------------------
	virtual CString GetTipText(int nItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will cause the tab control to reposition
	//     the tabs to the home position.
	//-----------------------------------------------------------------------
	virtual void Home();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function changes the location of the sizing gripper.
	//     The function has no effect if the FTS_XT_HSCROLL is not used.
	// Parameters:
	//     x        - The position for the gripper, relative to the left-hand-side of
	//                the control.
	//     bPercent - Indicates that the position is a percent of the control width,
	//                as opposed to an absolute location in pixels.
	//-----------------------------------------------------------------------
	void SetGripperPosition(int x,bool bPercent);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the location of the sizing gripper in pixels
	//     relative to the left hand side of the control.
	// Returns:
	//     An integer value representing the location of the sizing gripper
	//     in pixels relative to the left hand side of the control.
	//-----------------------------------------------------------------------
	int GetGripperPosition() const;

	// --------------------------------------------------------------------
	// Summary:
	//     Synchronizes the scroll bar with the horizontal scroll position.
	// Remarks:
	//     Call this function to synchronize the tab control's horizontal
	//     scroll bar with the horizontal scroll bar of the current view.
	//
	// You should call this function if anything happens in the view that
	//     affects the horizontal scroll bar (e.g., a user typing text into
	//     an edit control could make the text wider, thus requiring a call
	//     to this function).
	// --------------------------------------------------------------------
	void SyncScrollBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to set the shadow color for all tabs.
	// Parameters:
	//     crShadow - RGB value that represents the tab shadow color.
	//-----------------------------------------------------------------------
	void SetTabShadowColor(COLORREF crShadow);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets an RGB value that represents the shadow color
	//     of the tab.
	// Returns:
	//     An RGB value that represents the tab shadow color.
	//-----------------------------------------------------------------------
	COLORREF GetTabShadowColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to set the highlight color for all tabs.
	// Parameters:
	//     crHilight - RGB value that represents the tab highlight color.
	//-----------------------------------------------------------------------
	void SetTabHilightColor(COLORREF crHilight);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets an RGB value that represents the highlight
	//     color of the tab.
	// Returns:
	//     An RGB value that represents the tab highlight color.
	//-----------------------------------------------------------------------
	COLORREF GetTabHilightColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to set the background color for normal tabs.
	// Parameters:
	//     crBack - RGB value that represents the tab background color.
	//-----------------------------------------------------------------------
	void SetTabBackColor(COLORREF crBack);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets an RGB value that represents the background
	//     color of the tab.
	// Returns:
	//     An RGB value that represents the tab background color.
	//-----------------------------------------------------------------------
	COLORREF GetTabBackColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to set the text color for normal tabs.
	// Parameters:
	//     crText - RGB value that represents the tab text color.
	//-----------------------------------------------------------------------
	void SetTabTextColor(COLORREF crText);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets an RGB value that represents the text color
	//     of the tab.
	// Returns:
	//     An RGB value that represents the tab text color.
	//-----------------------------------------------------------------------
	COLORREF GetTabTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to set the background color for selected tabs.
	// Parameters:
	//     crBack - RGB value that represents the selected tab background color.
	//-----------------------------------------------------------------------
	void SetSelTabBackColor(COLORREF crBack);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets an RGB value that represents the background
	//     color for selected tabs.
	// Returns:
	//     An RGB value that represents the selected tabs background color.
	//-----------------------------------------------------------------------
	COLORREF GetSelTabBackColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to set the text color for selected tabs.
	// Parameters:
	//     crText - RGB value that represents the selected tab text color.
	//-----------------------------------------------------------------------
	void SetSelTabTextColor(COLORREF crText);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets an RGB value that represents the text color
	//     for selected tabs.
	// Returns:
	//     An RGB value that represents the selected tab text color.
	//-----------------------------------------------------------------------
	COLORREF GetSelTabTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to reset the tab control to use default
	//     system colors.
	//-----------------------------------------------------------------------
	void UpdateDefaultColors();

	// -------------------------------------------------------------------------
	// Summary:
	//     This method obtains a pointer to the specified sibling scroll bar for
	//     the flat tab control.
	// Parameters:
	//     nBar -  Specifies the type of scroll bar. The parameter can take one
	//             of the values specified in the Remarks section.
	// Remarks:
	//     The scroll bar type can be one of the following values.
	// <code>
	// SB_HORZ - Retrieves the position of the horizontal scroll bar.
	// SB_VERT - Retrieves the position of the vertical scroll bar.
	// </code>
	// -------------------------------------------------------------------------
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the area occupied by the tabs.
	// Remarks:
	//     This member function is called to retrieve the size and location
	//     of the area that is occupied by the tab items.
	// Returns:
	//     A CRect object representing the tab item area.
	//-----------------------------------------------------------------------
	CRect GetTabsRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the tab font.
	// Parameters:
	//     bBold - TRUE to return the bold font.
	// Remarks:
	//     Call this member function to return a pointer to the font
	//     used to display the tab item label for the tab control.
	// Returns:
	//     A pointer to a CFont object.
	//-----------------------------------------------------------------------
	CFont* GetTabFont(BOOL bBold);

	// ---------------------------------------------------------------------
	// Summary:
	//     Retrieves the FTS_XT_ styles that are currently in use for
	//     the flat tab control.
	// Returns:
	//     A <i>DWORD</i> value that represents the FTS_XT_ styles
	//     currently in use for the flat tab control.
	// Remarks:
	//     This value is a combination of tab control FTS_XT_ styles,
	//     see the CXTFlatTabCtrl::Create method for a list of
	//     available styles.
	// See Also:
	//     Create, SetTabStyle
	// ---------------------------------------------------------------------
	DWORD GetTabStyle();

	// --------------------------------------------------------------------------------
	// Summary:
	//     Sets the FTS_XT_ styles for a flat tab control.
	// Parameters:
	//     dwStyle -  Value specifying a combination of flat tab control FTS_XT_
	//                styles.
	// Returns:
	//     A DWORD value that contains the previous tab control FTS_XT_
	//     styles.
	// Remarks:
	//     The <i>dwStyle</i> parameter allows you to set one or more FTS_XT_
	//     styles, see the CXTFlatTabCtrl::Create method for a list of
	//     available styles.
	// See Also:
	//     Create, GetTabStyle
	// --------------------------------------------------------------------------------
	void SetTabStyle(DWORD dwStyle);

	// ----------------------------------------------------------------------------------------
	// Summary:
	//     Creates a flat tab control and attaches it to an instance of a CXTFlatTabCtrl
	//     object.
	// Parameters:
	//     dwStyle -     Specifies the tab control's style. Apply any combination of
	//                   tab control styles mentioned below in the Remarks section.
	//     rect -        Specifies the tab control's size and position. It can be
	//                   either a CRect object or a RECT structure.
	//     pParentWnd -  Specifies the tab control's parent window, usually a CDialog.
	//                   It must not be NULL.
	//     nID -         Specifies the tab control's ID.
	// Returns:
	//     TRUE if initialization of the object was successful;
	//     otherwise FALSE.
	// Remarks:
	//     You construct a CXTFlatTabCtrl object in two steps. First,
	//     call the constructor, and then call Create, which creates the tab
	//     control and attaches it to the CXTFlatTabCtrl object. You
	//     can use the following tab control styles when creating the object:
	//
	//     * <b>FTS_XT_BOTTOM</b> Show tabs on bottom.
	//     * <b>FTS_XT_HASARROWS</b> Show back and next arrows.
	//     * <b>FTS_XT_HASHOMEEND</b> Show home and end arrows. Used with
	//           FTS_XT_HASARROWS.
	//     * <b>FTS_XT_TOOLTIPS</b> Show tab tooltips.
	//     * <b>FTS_XT_DEFAULT</b> Same as (FTS_XT_BOTTOM | FTS_XT_HASARROWS
	//           | FTS_XT_HASHOMEEND | FTS_XT_TOOLTIPS).
	//     * <b>FTS_XT_HSCROLL</b> Show a horizontal scroll bar.
	//     * <b>FTS_XT_MASK</b> Combines all FTS_ styles.
	//
	//     In addition to tab control styles, you can apply the following
	//     window styles to a tab control:
	//
	//     * <b>WS_CHILD</b> Creates a child window that represents the
	//           tab control. Cannot be used with the WS_POPUP style.
	//     * <b>WS_VISIBLE</b> Creates a tab control that is initially
	//           visible.
	//     * <b>WS_DISABLED</b> Creates a window that is initially
	//           disabled.
	//     * <b>WS_GROUP</b> Specifies the first control of a group of
	//           controls in which the user can move from one control to the next
	//           with the arrow keys. All controls defined with the WS_GROUP
	//           style after the first control belong to the same group. The next
	//           control with the WS_GROUP style ends the style group and
	//           starts the next group (that is, one group ends where the next
	//           begins).
	//     * <b>WS_TABSTOP</b> Specifies one of any number of controls
	//           through which the user can move by using the TAB key. The TAB key
	//           moves the user to the next control specified by the WS_TABSTOP
	//           style.
	// Example:
	//     The following code sample demonstrates the usage of this function.
	// <code>
	// // Create  the flat tab control.
	// if (!m_wndFlatTabCtrl.Create(WS_CHILD|WS_VISIBLE|FTS_XT_DEFAULT,
	//     m_rcTabs, this, IDC_FLAT_TABCTRL))
	// {
	//     TRACE0( "Failed to create flattab control\n" );
	//     return;
	// }
	//
	// CreateViews();
	//
	// // Insert tabs into the flat tab control.
	// m_wndFlatTabCtrl.InsertItem(0, _T("Build"),           &m_sheet1);
	// m_wndFlatTabCtrl.InsertItem(1, _T("Debug"),           &m_sheet2);
	// m_wndFlatTabCtrl.InsertItem(2, _T("Find in Files 1"), &m_sheet3);
	// m_wndFlatTabCtrl.InsertItem(3, _T("Find in Files 2"), &m_sheet4);
	// m_wndFlatTabCtrl.InsertItem(4, _T("Results"),         &m_sheet5);
	// m_wndFlatTabCtrl.InsertItem(5, _T("SQL Debugging"),   &m_sheet6);
	// m_wndFlatTabCtrl.InsertItem(6, _T("Form View"),       m_pSheet7);
	// </code>
	// See Also:
	//     GetTabStyle, SetTabStyle
	// ----------------------------------------------------------------------------------------
	virtual BOOL Create(DWORD dwStyle, const CRect& rect, CWnd* pParentWnd, UINT nID);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to determine which button, if any, is at the specified
	//     screen position.
	// Parameters:
	//     pt - A CPoint reference representing the specified screen position.
	// Returns:
	//     The zero-based index of the button within the button's array or -1 if no
	//     button is at the specified position.
	//-----------------------------------------------------------------------
	virtual int ButtonHitTest(CPoint& pt) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will get the width in pixels of the tab specified
	//     by 'nItem'.
	// Parameters:
	//     nItem - The zero-based index of the tab to retrieve the width for.
	// Returns:
	//     The width in pixels of the tab.
	//-----------------------------------------------------------------------
	int GetTabWidth(int nItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will return the total width of all the tabs
	//     in the flat tab control.
	// Returns:
	//     The total combined width in pixels of all the tabs in the control.
	//-----------------------------------------------------------------------
	int GetTotalTabWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will return the total width of all the arrow
	//     buttons that are visible in the flat tab control.
	// Returns:
	//     The total width in pixels of all the visible arrow buttons.
	//-----------------------------------------------------------------------
	int GetTotalArrowWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will return the total area width of all the
	//     tabs in the flat tab control.
	// Returns:
	//     The total area width in pixels of all the tabs in the flat tab control.
	//-----------------------------------------------------------------------
	int GetTotalTabAreaWidth() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will draw a tab to the device context specified
	//     by 'pDC'.
	// Parameters:
	//     pDC          - Points to the device context to draw the tab to.
	//     pt           - XY location of the top left corner of the tab to draw.
	//     bSelected    - true if the tab is currently selected.
	//     lpszTabLabel - A NULL terminated string that represents the tab label.
	// Returns:
	//     The x position of the next tab to be drawn.
	//-----------------------------------------------------------------------
	virtual int DrawTab(CDC* pDC,const CPoint& pt,bool bSelected,LPCTSTR lpszTabLabel);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used by the flat tab control to draw an arrow
	//     button to the device context specified by 'pDC'.
	// Parameters:
	//     pDC          - Points to the device context to draw the tab to.
	//     button_state - XY location of the top left corner of the tab to draw.
	//-----------------------------------------------------------------------
	void DrawButton(CDC* pDC,CXTFlatTabCtrlButtonState& button_state);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will force all of the tabs to be repainted.
	//-----------------------------------------------------------------------
	void InvalidateTabs();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will enable or disable the arrow buttons
	//     depending on the current tab display state.
	//-----------------------------------------------------------------------
	void EnableButtons();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will free the resources allocated for the
	//     icons used by the arrow buttons.
	//-----------------------------------------------------------------------
	void FreeButtonIcons();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will create the icon resources that are
	//     used by the arrow buttons.
	//-----------------------------------------------------------------------
	void CreateButtonIcons();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will draw the horizontal sizing gripper at a
	//     specified location.
	// Parameters:
	//     pDC  - Points to the device context to draw the gripper to.
	//     rect - Location of the gripper.
	//-----------------------------------------------------------------------
	void DrawGripper(CDC* pDC,CRect rect) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     The member function is used internally to toggle the state of the
	//     sizing-grip tracking mode.
	// Parameters:
	//     bTracking - true to enable tracking, or false to disable tracking.
	//-----------------------------------------------------------------------
	void SetTracking(bool bTracking);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function frees all memory occupied by the tab items.
	//-----------------------------------------------------------------------
	void ClearAllItems();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called when the tab control is resized. It
	//     is responsible for updating internal structures that are dependant
	//     on the control's size.
	//-----------------------------------------------------------------------
	void RecalcLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     This internal function deletes an item from the tab item list.
	// Parameters:
	//     nItem - Index of the tab to delete.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL _DeleteItem(int nItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This internal function calculates the overlap between two tabs.
	// Returns:
	//     An integer value that represents the overlap between the tabs.
	//-----------------------------------------------------------------------
	virtual int GetOverlap() const;

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTFlatTabCtrl)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnNotify(WPARAM, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTFlatTabCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLeftArrow();
	afx_msg void OnRightArrow();
	afx_msg void OnHomeArrow();
	afx_msg void OnEndArrow();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:

	int m_cx;                 // Width for each arrow button.
	int m_cy;                 // Height for each arrow button.
	int m_nCurSel;            // Index of the currently selected tab.
	int m_nClientWidth;       // Width in pixels of the tab control client area.
	int m_nClientHeight;      // Height in pixels of the tab control client area.
	int m_nOffset;            // Amount in pixels of the displayed tab offset.
	int m_xGripperPos;        // The current gripper position in pixels from the left.
	int m_iGripperPosPerCent; // The current gripper position in percent of the control width.
	int m_xTrackingDelta;     // Tracking delta.
	int m_iBtnLeft;           // Index of "left" button in button array.
	int m_iBtnRight;          // Index of "right" button in button array.
	int m_iBtnHome;           // Index of "home" button in button array.
	int m_iBtnEnd;            // Index of "end" button in button array.

	bool m_bTracking;      // Tracking state.
	bool m_bManagingViews; // true if the control is managing views.
	bool m_bUserColors;    // true if user defined colors are used.

	DWORD m_dwStyle;       // Tab control style.
	CWnd* m_pWndLastFocus; // Last focused window.
	CRect m_rectTabs;      // Area occupied by tabs.
	CRect m_rectViews;     // Area occupied by managed views.
	CRect m_rectSB_H;      // Area occupied by the horizontal scroll bar.
	CRect m_rectGripper;   // Area occupied by the sizing gripper.

	CFont* m_pNormFont; // Font that is used for non-selected tabs.
	CFont* m_pBoldFont; // Font that is used for selected tabs.

	CScrollBar                          m_wndHScrollBar; // The horizontal scroll bar (used with FTS_XT_HSCROLL).
	CToolTipCtrl                        m_ToolTip;       // Tooltip for the flat tab control.
	CXTFlatTabCtrlButtonState           m_buttons[4];    // Array of button information.
	CArray <XT_TCB_ITEM*, XT_TCB_ITEM*> m_tcbItems;      // Template list containing tab information.

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTFlatTabCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTFlatTabCtrl::InsertItem(int nItem, UINT nTextID, CWnd *pWndControl) {
	ASSERT(IsWindow(m_hWnd)); CString strItem; strItem.LoadString(nTextID); return InsertItem(nItem, strItem, pWndControl);
}
AFX_INLINE int CXTFlatTabCtrl::GetCurSel() const {
	ASSERT(IsWindow(m_hWnd)); return m_nCurSel;
}
AFX_INLINE int CXTFlatTabCtrl::GetItemCount() const {
	ASSERT(IsWindow(m_hWnd)); int iItemCount = (int)m_tcbItems.GetSize(); return iItemCount;
}
AFX_INLINE void CXTFlatTabCtrl::SetTabFonts(CFont* pNormFont, CFont* pBoldFont) {
	if (pNormFont){ m_pNormFont = pNormFont; } if (pBoldFont){ m_pBoldFont = pBoldFont; }
}
AFX_INLINE DWORD CXTFlatTabCtrl::GetTabStyle() {
	return m_dwStyle;
}
AFX_INLINE void CXTFlatTabCtrl::SetTabStyle(DWORD dwStyle) {
	m_dwStyle = dwStyle;
}
AFX_INLINE  CRect CXTFlatTabCtrl::GetTabsRect() {
	return m_rectTabs;
}
AFX_INLINE CFont* CXTFlatTabCtrl::GetTabFont(BOOL bBold) {
	return bBold? m_pBoldFont: m_pNormFont;
}

const DWORD FTS_XT_BOTTOM          = 0x0001;  //<ALIAS CXTFlatTabCtrl::Create@DWORD@const CRect&@CWnd*@UINT>
const DWORD FTS_XT_HASARROWS       = 0x0002;  //<ALIAS CXTFlatTabCtrl::Create@DWORD@const CRect&@CWnd*@UINT>
const DWORD FTS_XT_HASHOMEEND      = 0x0004;  //<ALIAS CXTFlatTabCtrl::Create@DWORD@const CRect&@CWnd*@UINT>
const DWORD FTS_XT_TOOLTIPS        = 0x0008;  //<ALIAS CXTFlatTabCtrl::Create@DWORD@const CRect&@CWnd*@UINT>
const DWORD FTS_XT_DEFAULT         = 0x000F;  //<ALIAS CXTFlatTabCtrl::Create@DWORD@const CRect&@CWnd*@UINT>
const DWORD FTS_XT_HSCROLL         = 0x0010;  //<ALIAS CXTFlatTabCtrl::Create@DWORD@const CRect&@CWnd*@UINT>
const DWORD FTS_XT_MASK            = 0x001F;  //<ALIAS CXTFlatTabCtrl::Create@DWORD@const CRect&@CWnd*@UINT>

#endif // #if !defined(__XTFLATTABCTRL_H__)
