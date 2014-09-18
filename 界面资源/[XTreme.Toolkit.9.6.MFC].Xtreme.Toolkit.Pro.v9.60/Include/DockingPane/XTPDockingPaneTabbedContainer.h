// XTPDockingPaneTabbedContainer.h : interface for the CXTPDockingPaneTabbedContainer class.
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
#if !defined(__XTPDOCKINGPANETABBEDCONTAINER_H__)
#define __XTPDOCKINGPANETABBEDCONTAINER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPDockingPaneBase.h"
#include "XTPDockingPanePaintManager.h"


class CXTPDockingPaneAutoHidePanel;
class CXTPDockingPaneCaptionButton;

//===========================================================================
// Summary:
//     CXTPDockingPaneTabbedContainer is a multiple inheritance class derived from
//     CWnd and CXTPDockingPaneBase. It represents a tabbed container for Docking
//     Panes.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneTabbedContainer : public CWnd, public CXTPDockingPaneBase, public CXTPTabManager
{
	DECLARE_DYNAMIC(CXTPDockingPaneTabbedContainer)
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneTabbedContainer object
	//     Protected constructor. Internally constructed only.
	// Parameters:
	//     pLayout - Points to a CXTPDockingPaneLayout object.
	//-----------------------------------------------------------------------
	CXTPDockingPaneTabbedContainer(CXTPDockingPaneLayout* pLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneTabbedContainer object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPDockingPaneTabbedContainer();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to activate a child docking pane.
	// Parameters:
	//     pPane     - Pane to be activated.
	//     bSetFocus - TRUE to set focus to child docking pane.
	//-----------------------------------------------------------------------
	virtual void SelectPane(CXTPDockingPane* pPane, BOOL bSetFocus = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get child pane state.
	// Returns:
	//     TRUE if a child pane is active.
	//-----------------------------------------------------------------------
	BOOL IsActive();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get selected pane.
	// Returns:
	//     The selected child pane.
	//-----------------------------------------------------------------------
	CXTPDockingPane* GetSelected();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to access title's close button.
	// Returns:
	//     A reference to the title's close button.
	//-----------------------------------------------------------------------
	CXTPDockingPaneCaptionButton* GetCloseButton();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to access title's pin button.
	// Returns:
	//     A reference to the title's pin button.
	//-----------------------------------------------------------------------
	CXTPDockingPaneCaptionButton* GetPinButton();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get collection of panes.
	// Returns:
	//     A reference to the child panes collection.
	// See Also:
	//     CXTPDockingPaneList
	//-----------------------------------------------------------------------
	CXTPDockingPaneList& GetPanes();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the title is visible.
	// Returns:
	//     TRUE if the title is visible.
	//-----------------------------------------------------------------------
	BOOL IsTitleVisible();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the tab bar is visible.
	// Returns:
	//     TRUE if the tab bar is visible.
	//-----------------------------------------------------------------------
	BOOL IsTabsVisible();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the tabbed container contains
	//     children panes.
	// Returns:
	//     TRUE if the container contains children.
	//-----------------------------------------------------------------------
	virtual BOOL IsEmpty();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves safe window handle.
	//-----------------------------------------------------------------------
	virtual HWND GetSafeHwnd();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the tabbed container is
	//     auto-hidden.  If one pane in a group of panes is auto-hidden,
	//     then the entire group will be auto-hidden.
	// Returns:
	//     TRUE if the tabbed container is auto-hidden, FALSE if the
	//     tab group is visible.
	//-----------------------------------------------------------------------
	BOOL IsHide();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called by the framework when the left mouse
	//     button is clicked while the mouse pointer is positioned over
	//     a pane caption.  This will give the pane focus and begin the
	//     pane dragging process (If the left button is held down).
	// Parameters:
	//     point - Cursor location on the pane's caption that was clicked.
	//-----------------------------------------------------------------------
	virtual void OnCaptionLButtonDown(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method called to check if docking pane can be attached to tabbed container.
	// Parameters:
	//     rcClient - Client rectangle of container.
	//     pt       - Point to attach.
	// Returns:
	//     TRUE if pane can be attached as new tab to tabbed container.
	//-----------------------------------------------------------------------
	virtual BOOL CanAttach(CRect& rcClient, CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to find all panes of a specific XTPDockingPaneType
	//     within the tabbed container.
	// Parameters:
	//     type  - Type of docking pane to search for within tabbed container.
	//     pList - An array of type CXTPDockingPaneBaseList that will
	//             contain a list of all panes of the specified type
	//             that are in the tabbed container.  This points to a
	//             CXTPDockingPaneBaseList object that will contain
	//             found panes.
	// Remarks:
	//     pList is an external list that you must create and pass in as a
	//     parameter.  See example below:
	// Example:
	//     This will get the count of panes inside container:
	// <code>
	// CXTPDockingPaneBaseList lst;
	//
	// m_pTopContainer->FindPane(xtpPaneTypeTabbedContainer, &lst);
	// int nCount = (int)lst.GetCount();
	// </code>
	//-----------------------------------------------------------------------
	virtual void FindPane(XTPDockingPaneType type, CXTPDockingPaneBaseList* pList);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if one pane is contained within
	//     the collection of another.
	// Parameters:
	//     pPane - Points to a CXTPDockingPaneBase object.
	// Returns:
	//     POSITION of the pane if the collection is successful; otherwise
	//     returns NULL.
	//-----------------------------------------------------------------------
	virtual POSITION ContainPane(CXTPDockingPaneBase* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to fill the pMinMaxInfo structure with the
	//     minimum and maximum width and height of the container.
	//     This will look at the MinMaxInfo for each pane in the
	//     container and set the appropriate minimum and maximum width
	//     and height for the container.
	// Parameters:
	//     pMinMaxInfo - Pointer to MINMAXINFO structure
	//-----------------------------------------------------------------------
	void GetMinMaxInfo(LPMINMAXINFO pMinMaxInfo);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to determine if a specific caption button
	//     is visible.
	// Parameters:
	//     pButton - Button to check to see if it is visible.
	// Returns:
	//     TRUE if the caption button specified is visible, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsCaptionButtonVisible(CXTPDockingPaneCaptionButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called by the framework when a caption button
	//     is clicked.
	// Parameters:
	//     pButton - Caption button that was clicked.
	//-----------------------------------------------------------------------
	virtual void OnCaptionButtonClick(CXTPDockingPaneCaptionButton* pButton);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneTabbedContainer object
	//     Protected constructor. Internally constructed only.
	// Parameters:
	//     pPane   - Child pane.
	//     pFrame  - Parent Frame.
	//     pLayout - Points to a CXTPDockingPaneLayout object.
	//-----------------------------------------------------------------------
	void Init(CXTPDockingPane* pPane, CFrameWnd* pFrame);

	//-----------------------------------------------------------------------
	// Summary:
	//     Copies a CXTPDockingPaneTabbedContainer object
	// Parameters:
	//     pClone  - Points to a CXTPDockingPaneTabbedContainer object
	//     pMap    - Points to a CXTPPaneToPaneMap object
	//     dwIgnoredOptions - Options that must be skipped.
	// See Also:
	//     CXTPPaneToPaneMap
	//-----------------------------------------------------------------------
	virtual void Copy(CXTPDockingPaneTabbedContainer* pClone, CXTPPaneToPaneMap* pMap, DWORD dwIgnoredOptions);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to recalculate the positions of
	//     tab groups/Items.
	//-----------------------------------------------------------------------
	virtual void Reposition();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to redraw the tabbed control, I.e. When
	//     an Icon has changed.
	//-----------------------------------------------------------------------
	virtual void RedrawControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to access the visual elements of the docking
	//     pane tabs.  I.e. Tab colors, styles, etc...
	// Returns:
	//     Pointer to CXTPTabPaintManager that contains the visual elements
	//     of the docking pane tabs.
	//-----------------------------------------------------------------------
	virtual CXTPTabPaintManager* GetPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called in a derived class to set a pointer
	//     the tab paint manager.
	// Parameters:
	//     pPaintManager - Pointer to a CXTPTabPaintManager object.
	//-----------------------------------------------------------------------
	virtual void SetPaintManager(CXTPTabPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the icon of the tab in the tabbed
	//     container needs to be drawn.  I.e. On mouse over.
	// Parameters:
	//     pDC    - Pointer to the destination device context.
	//     pt     - Specifies the location of the image.
	//     pItem  - CXTPTabManagerItem object to draw icon on.
	//     bDraw  - TRUE if the icon needs to be drawn, I.e. the icon size
	//                 changed.  FALSE if the icon does not need to be
	//                 drawn or redrawn.
	//     szIcon - Size of the tab icon.
	// Returns:
	//     TRUE if the icon was successfully drawn, FALSE if the icon
	//     was not drawn.
	//-----------------------------------------------------------------------
	virtual BOOL DrawIcon(CDC* pDC, CPoint pt, CXTPTabManagerItem* pItem, BOOL bDraw, CSize& szIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to save/restore the settings of the pane.
	// Parameters:
	//     pPX - Points to a CXTPPropExchange object.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DoPropExchange(CXTPPropExchange* pPX);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPDockingPaneTabbedContainer)
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPDockingPaneTabbedContainer)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE


private:
	virtual void OnSizeParent(CFrameWnd* pParent, CRect rect, LPVOID lParam);
	virtual void OnIdleUpdate();
	virtual void SetParentFrame(CFrameWnd* pFrame);
	virtual void RemovePane(CXTPDockingPaneBase* pPane);
	virtual void OnParentConainerChanged(CXTPDockingPaneBase* pContainer);
	virtual CXTPDockingPaneBase* Clone(CXTPDockingPaneLayout* pLayout, CXTPPaneToPaneMap* pMap, DWORD dwIgnoredOptions = 0);
	void ShowTitle(BOOL bShow);
	CString GetTitle();
	void Show();
	void _UpdateTabs();
	void _Swap(CXTPDockingPane* p1, CXTPDockingPane* p2);
	void _InsertPane(CXTPDockingPane* pPane);
	int HitTest(CPoint point);
	CXTPDockingPane* GetItemPane(int nIndex);
	void AdjustMinMaxInfoClientRect(LPMINMAXINFO pMinMaxInfo, BOOL bCaptionOnly = FALSE);


private:
	BOOL m_bActive;
	BOOL m_bTitleVisible;
	BOOL m_bLockReposition;
	CXTPDockingPane*      m_pSelectedPane;
	CXTPDockingPane*      m_pTrackingPane;
	CArray<CRect, CRect&> m_lstRects;
	CXTPDockingPaneList      m_lstPanes;
	COleDropTarget* m_pDropTarget;

private:
	class CContainerDropTarget;

private:
	friend class CXTPDockingPaneManager;
	friend class CXTPDockingPane;
	friend class CXTPDockingPaneContext;
	friend class CXTPDockingPaneMiniWnd;
	friend class CXTPDockingPaneLayout;
	friend class CXTPDockingPaneAutoHidePanel;
	friend class CXTPDockingPaneAutoHideWnd;
	friend class CXTPDockingPaneCaptionButton;
	friend class CContainerDropTarget;
};

AFX_INLINE BOOL CXTPDockingPaneTabbedContainer::IsActive() {
	return m_bActive;
}
AFX_INLINE CXTPDockingPane* CXTPDockingPaneTabbedContainer::GetSelected() {
	return m_pSelectedPane;
}
AFX_INLINE CXTPDockingPaneList& CXTPDockingPaneTabbedContainer::GetPanes() {
	return m_lstPanes;
}
AFX_INLINE HWND CXTPDockingPaneTabbedContainer::GetSafeHwnd() {
	return CWnd::GetSafeHwnd();
}

#endif // #if !defined(__XTPDOCKINGPANETABBEDCONTAINER_H__)
