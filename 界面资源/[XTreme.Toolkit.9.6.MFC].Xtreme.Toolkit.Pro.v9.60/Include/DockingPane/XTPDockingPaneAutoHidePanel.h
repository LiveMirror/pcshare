// XTPDockingPaneAutoHidePanel.h : interface for the CXTPDockingPaneAutoHidePanel class.
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
#if !defined(__XTPDOCKINGPANEAUTOHIDEPANEL_H__)
#define __XTPDOCKINGPANEAUTOHIDEPANEL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPDockingPaneBase.h"

class CXTPDockingPaneLayout;
class CXTPDockingPaneAutoHidePanel;

//===========================================================================
// Summary:
//     CXTPDockingPaneAutoHideWnd is a CMiniFrameWnd derived class. It is
//     used internally by CXTPDockingPaneAutoHidePanel
//     as a sliding auto-hide window for the hidden pane.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneAutoHideWnd : public CMiniFrameWnd
{
	DECLARE_DYNAMIC(CXTPDockingPaneAutoHideWnd)

private:
	CXTPDockingPaneAutoHideWnd(CXTPDockingPaneAutoHidePanel* pPanel, CXTPDockingPaneTabbedContainer* pPane);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves child container of sliding auto-hide window.
	//-----------------------------------------------------------------------
	CXTPDockingPaneTabbedContainer* GetPane();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPDockingPaneAutoHideWnd)
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL


	//{{AFX_MSG(CXTPDockingPaneAutoHideWnd)
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	void CloseWindow();
	CXTPDockingPaneManager* GetDockingPaneManager();
	
	void GetMinMaxInfo (LPMINMAXINFO pMinMaxInfo, BOOL bIncludeSplitter = TRUE);

	void RecalcLayout(BOOL bNotify);
	void DoSlideStep();
	void Reposition(CRect rc, CRect rcClient);
	BOOL GetAvailableRect(CRect& rcAvail, CRect& rc);

public:
	static double m_dAnimationDelay;              // Animation delay
	static int m_nAnimationInterval;              // Delay in ms between animation steps
	static int m_nAnimationDuration;              // Duration of sliding animation in ms
	static DWORD m_nMouseHoverDelay;              // Duration of mouse hover before expanding
	static UINT m_nInactiveCollapseDelay;          // Delay before collapsing inactive.

private:
	CXTPDockingPaneTabbedContainer* m_pPane;
	CXTPDockingPaneAutoHidePanel* m_pPanel;
	XTPDockingPaneDirection m_direction;

	BOOL m_bHoriz;
	int m_nSlideStep;
	int m_nStepsCount;
	BOOL m_bSlideOut;
	CRect m_rcWindow;
	int m_nDeactivationCount;
	BOOL m_bTracking;

private:
	friend class CXTPDockingPaneAutoHidePanel;
};

//===========================================================================
// Summary:
//     CXTPDockingPaneAutoHidePanel is a multiple inheritance class derived from
//     CWnd and CXTPDockingPaneBase. It is used internally as an auto-hide container
//     for CXTPDockingPaneBase derived classes.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneAutoHidePanel : public CWnd, public CXTPDockingPaneBase
{
private:
	class CAutoHidePanelTabManager;
	class CAutoHidePanelTabManagersArray;
	class CPanelDropTarget;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneAutoHidePanel object
	//     Protected constructor. Internally constructed only.
	// Parameters:
	//     pLayout - Points to a CXTPDockingPaneLayout object.
	//-----------------------------------------------------------------------
	CXTPDockingPaneAutoHidePanel(CXTPDockingPaneLayout* pLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneAutoHidePanel object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	~CXTPDockingPaneAutoHidePanel();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine the position of the
	//     auto-hide panel.
	// Returns:
	//     The panel's direction.
	//-----------------------------------------------------------------------
	XTPDockingPaneDirection GetDirection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to return collection of the child panes.
	// Returns:
	//     A reference to the child panes collection.
	//-----------------------------------------------------------------------
	CXTPDockingPaneBaseList& GetPanes();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to return collection of the tabs.
	// Returns:
	//     A reference to the tabs collection.
	//-----------------------------------------------------------------------
	CXTPDockingPaneHiddenTabsArray& GetTabs();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the window handle for a window.
	// Returns:
	//     Returns NULL if the HWND is not attached to a window or if it
	//     is used with a NULL HWND pointer.
	//-----------------------------------------------------------------------
	virtual HWND GetSafeHwnd();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to set the position of the pane
	// Parameters:
	//     pParent - Points to a CFrameWnd object.
	//     rect    - Rectangle of the pane to be set.
	//     lParam  - Pointer to AFX_SIZEPARENTPARAMS structure.
	// ----------------------------------------------------------------------
	virtual void OnSizeParentEx(CWnd* pParent, CRect& rect, LPVOID lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to clone the pane.
	// Parameters:
	//     pLayout          - Points to a CXTPDockingPaneLayout object.
	//     pMap             - Points to a CXTPPaneToPaneMap object.
	//     dwIgnoredOptions - TRUE to ignore options of the pane.
	// Returns:
	//     A pointer to a CXTPDockingPaneBase object.
	// See Also:
	//     CXTPPaneToPaneMap
	//-----------------------------------------------------------------------
	virtual CXTPDockingPaneBase* Clone(CXTPDockingPaneLayout* pLayout, CXTPPaneToPaneMap* pMap, DWORD dwIgnoredOptions = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove a pane from collection.
	// Parameters:
	//     pPane - Points to a CXTPDockingPaneBase object.
	//-----------------------------------------------------------------------
	virtual void RemovePane(CXTPDockingPaneBase* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when a child pane collection is changed.
	// Parameters:
	//     pContainer - Points to a CXTPDockingPaneBase object that is changed.
	//-----------------------------------------------------------------------
	virtual void OnChildConainerChanged(CXTPDockingPaneBase* pContainer);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if there are children panes
	// Returns:
	//     TRUE if no children panes; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsEmpty();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function determines which header item, if any, is
	//     at a specified cursor position
	// Parameters:
	//     point - Point to be tested.
	// Returns:
	//     The index of the item at the position specified by 'pt',
	//     otherwise returns -1.
	//-----------------------------------------------------------------------
	CXTPDockingPane* HitTest(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to insert a pane into a collection
	// Parameters:
	//     pPane - Points to a CXTPDockingPaneBase object
	//-----------------------------------------------------------------------
	virtual void _InsertPane(CXTPDockingPaneBase* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to save/restore the settings of the pane.
	// Parameters:
	//     pPX - Points to a CXTPPropExchange object.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to close the active window
	// Parameters:
	//     bDelayed - FALSE to delay the closing
	//-----------------------------------------------------------------------
	void CloseActiveWindow(BOOL bDelayed = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to show (activate) a pane by its identifier.
	// Parameters:
	//     pPane     - Points to a CXTPDockingPane object.
	//     bSetFocus - TRUE to set focus to child docking pane
	//-----------------------------------------------------------------------
	void ShowPane(CXTPDockingPane* pPane, BOOL bSetFocus = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to refresh the manager array
	// Parameters:
	//     pDC      - Points to a CDC object
	//     rcClient - Size of the client area
	//-----------------------------------------------------------------------
	void RefreshManagersArray(CDC* pDC, CRect rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the height of the panel
	// Returns:
	//     The height of the panel
	//-----------------------------------------------------------------------
	int GetPanelHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates auto-hide panel.
	// Parameters:
	//     pParent - Parent frame site.
	//-----------------------------------------------------------------------
	virtual void Create(CWnd* pParent);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPDockingPaneAutoHidePanel)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseHover(WPARAM, LPARAM lParam);
	afx_msg void OnMouseLeave();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	void Copy(CXTPDockingPaneAutoHidePanel* pClone, CXTPPaneToPaneMap* pMap);
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

public:
	static BOOL m_bCloseActiveWindow;                             // TRUE to allow the AutoHide window to be closed.


protected:
	CXTPDockingPaneAutoHideWnd* m_pActiveWnd;                     // Active window
	XTPDockingPaneDirection m_direction;                          // Location of hidden panel
	CXTPDockingPaneBaseList m_lstPanes;                           // Array of Tabbed containers.

private:
	CAutoHidePanelTabManagersArray* m_pTabManagers;
	CPanelDropTarget* m_pDropTarget;

private:
	friend class CXTPDockingPaneLayout;
	friend class CXTPDockingPaneAutoHideWnd;
	friend class CXTPDockingPaneTabbedContainer;
	friend class CXTPDockingPaneManager;
	friend class CPanelDropTarget;
};

AFX_INLINE BOOL CXTPDockingPaneAutoHidePanel::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE XTPDockingPaneDirection CXTPDockingPaneAutoHidePanel::GetDirection() {
	return m_direction;
}
AFX_INLINE CXTPDockingPaneBaseList& CXTPDockingPaneAutoHidePanel::GetPanes() {
	return m_lstPanes;
}
AFX_INLINE HWND CXTPDockingPaneAutoHidePanel::GetSafeHwnd() {
	return CWnd::GetSafeHwnd();
}

AFX_INLINE CXTPDockingPaneTabbedContainer* CXTPDockingPaneAutoHideWnd::GetPane() {
	return m_pPane;
}

#endif // #if !defined(__XTPDOCKINGPANEAUTOHIDEPANEL_H__)
