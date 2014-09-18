// XTPDockingPaneSplitterContainer.h : interface for the CXTPDockingPaneSplitterContainer class.
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
#if !defined(__XTPDOCKINGPANESPLITTERCONTAINER_H__)
#define __XTPDOCKINGPANESPLITTERCONTAINER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPDockingPaneDefines.h"

class CXTPDockingPaneSplitterContainer;
class CXTPDockingPaneLayout;

//===========================================================================
// Summary:
//     CXTPDockingPaneSplitterWnd is a CWnd derived class. It is
//     used internally for splitting Panes.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneSplitterWnd: public CWnd
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneSplitterWnd object
	//-----------------------------------------------------------------------
	CXTPDockingPaneSplitterWnd();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneSplitterWnd object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPDockingPaneSplitterWnd();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if splitter is horizontal
	// Returns:
	//     TRUE if splitter is horizontal located.
	//-----------------------------------------------------------------------
	BOOL IsHorizontal();

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates CXTPDockingPaneSplitterWnd window.
	// Parameters:
	//     pContainer - Points to a parent CXTPDockingPaneSplitterContainer object
	//     pFirst     - Points to a first splitting CXTPDockingPaneBase object
	//     pSecond    - Points to a second splitting CXTPDockingPaneBase object
	//-----------------------------------------------------------------------
	void Create(CXTPDockingPaneSplitterContainer* pContainer, CXTPDockingPaneBase* pFirst, CXTPDockingPaneBase* pSecond);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPDockingPaneSplitterWnd)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	CXTPDockingPaneManager* GetDockingPaneManager();
	void Reposition(CRect rc, CRect rcAvail);
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL GetAvailableRect(CRect& rcAvail, CRect& rcUnion);

private:
	CXTPDockingPaneSplitterContainer* m_pContainer;
	CXTPDockingPaneBase* m_pFirst;
	CXTPDockingPaneBase* m_pSecond;
	HCURSOR m_hCursor;

private:
	friend class CXTPDockingPaneSplitterContainer;
};


//===========================================================================
// Summary:
//     CXTPDockingPaneSplitterContainer is a CXTPDockingPaneBase derived class.
//     It is used internally as a splitter container for CXTPDockingPaneBase
//     derived classes.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneSplitterContainer : public CXTPDockingPaneBase
{

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneSplitterContainer object.
	// Parameters:
	//     pLayout - Points to a CXTPDockingPaneLayout object.
	//-----------------------------------------------------------------------
	CXTPDockingPaneSplitterContainer(CXTPDockingPaneLayout* pLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneSplitterContainer object, handles
	//     cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPDockingPaneSplitterContainer();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if there are child panes in the
	//     splitter container.
	// Returns:
	//     TRUE if there are no child panes; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsEmpty();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to set the position of the pane.
	// Parameters:
	//     pParent - Points to a CFrameWnd object
	//     rect    - Rectangle of the pane to be set
	//     lParam  - Pointer to AFX_SIZEPARENTPARAMS structure
	//-----------------------------------------------------------------------
	virtual void OnSizeParent(CFrameWnd* pParent, CRect rect, LPVOID lParam);

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
	//     Call this member to retrieve a collection of the panes with
	//     the specified type.
	// Parameters:
	//     type  - Type of the panes to find.
	//     pList - Points to a CXTPDockingPaneBaseList object that will
	//             contain found panes.
	//-----------------------------------------------------------------------
	virtual void FindPane(XTPDockingPaneType type, CXTPDockingPaneBaseList* pList);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the splitter orientation
	//     is horizontal.
	// Returns:
	//     TRUE if the splitter orientation is horizontal, FALSE
	//     otherwise.
	//-----------------------------------------------------------------------
	BOOL IsHoriz() { return m_bHoriz; }

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to fill the pMinMaxInfo structure with the
	//     minimum and maximum width and height of the splitter container.
	//     This will look at the MinMaxInfo for each pane in the splitter
	//     container and set the appropriate minimum and maximum width
	//     and height for the splitter container.
	// Parameters:
	//     pMinMaxInfo - Pointer to MINMAXINFO structure
	//-----------------------------------------------------------------------
	void GetMinMaxInfo(LPMINMAXINFO pMinMaxInfo);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneSplitterContainer object
	//     Protected constructor. Internally constructed only.
	// Parameters:
	//     pPane   - Child pane.
	//     bHoriz - TRUE if the splitter orientation is horizontal
	//     pFrame - Parent Frame.
	//-----------------------------------------------------------------------
	void Init(CXTPDockingPaneBase* pPane, BOOL bHoriz, CFrameWnd* pFrame);

	//-----------------------------------------------------------------------
	// Summary:
	//     Copies a CXTPDockingPaneSplitterContainer object
	// Parameters:
	//     pClone  - Points to a CXTPDockingPaneSplitterContainer object
	//     pMap - Points to a CXTPPaneToPaneMap object
	// See Also:
	//     CXTPPaneToPaneMap
	//-----------------------------------------------------------------------
	void Copy(CXTPDockingPaneSplitterContainer* pClone, CXTPPaneToPaneMap* pMap);


	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called by the framework when a splitter
	//     container is created.
	// Returns:
	//     Pointer to the newly created CXTPDockingPaneSplitterWnd
	//     object.
	//-----------------------------------------------------------------------
	virtual CXTPDockingPaneSplitterWnd* OnCreateSplitter();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to remove a pane from the collection.
	// Parameters:
	//     pPane - Points to a CXTPDockingPaneBase object.
	//-----------------------------------------------------------------------
	virtual void RemovePane(CXTPDockingPaneBase* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when a child pane collection is changed.
	// Parameters:
	//     pContainer - Points to a CXTPDockingPaneBase object.
	//-----------------------------------------------------------------------
	virtual void OnChildConainerChanged(CXTPDockingPaneBase* pContainer);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the parent pane collection is changed.
	// Parameters:
	//     pContainer - Points to a CXTPDockingPaneBase object.
	//-----------------------------------------------------------------------
	virtual void OnParentConainerChanged(CXTPDockingPaneBase* pContainer);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to clone the pane.
	// Parameters:
	//     pLayout          - Points to a CXTPDockingPaneLayout object.
	//     pMap             - Points to a CXTPPaneToPaneMap object.
	//     dwIgnoredOptions - TRUE to ignore the options of the pane.
	// Returns:
	//     Pointer to cloned pane.
	// See Also:
	//     CXTPPaneToPaneMap
	//-----------------------------------------------------------------------
	virtual CXTPDockingPaneBase* Clone(CXTPDockingPaneLayout* pLayout, CXTPPaneToPaneMap* pMap, DWORD dwIgnoredOptions = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to save/restore the settings of the pane.
	// Parameters:
	//     pPX - Points to a CXTPPropExchange object.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to normalize the size of docking panes
	//     in the splitter container.  Normalizing the size of the panes
	//     means that the value of the left coordinate is less than the
	//     right and the top is less than the bottom.
	//-----------------------------------------------------------------------
	void NormalizeDockingSize();

private:
	void _Replace(CXTPDockingPaneBase* pPane, CXTPDockingPaneBase* pReplace);
	void _InsertPane(CXTPDockingPaneBase* pPane, CXTPDockingPaneBase* pNeighbour = NULL, BOOL bAfter = TRUE);
	void _UpdateSplitters();
	BOOL _Before(CXTPDockingPaneBase* pPane, POSITION pos);
	CRect _CalculateResultDockingRectSelf(CXTPDockingPaneBase* pPane, XTPDockingPaneDirection direction, CXTPDockingPaneBase* pNeighbour);
	static CRect _CalculateResultDockingRectChild(CXTPDockingPaneBase* pSplitter, CXTPDockingPaneBase* pPane, XTPDockingPaneDirection direction);
	static CRect _CalculateResultDockingRect(BOOL bHoriz, CXTPDockingPaneBaseList& lst, CRect rect, CXTPDockingPaneBase* pPaneI);
	static void _AdjustPanesLength(CXTPDockingPaneManager* pManager, CXTPDockingPaneBaseList& lst, CRect rect, BOOL bHoriz, BOOL bApply, int& nTotalLength, int& nLengthAvail);
	static LONG& GetMinSize(LPMINMAXINFO pMinMaxInfo, BOOL bHoriz);
	static LONG& GetMaxSize(LPMINMAXINFO pMinMaxInfo, BOOL bHoriz);


protected:
	CXTPDockingPaneBaseList m_lstPanes;   // List of child panes
	CList<CXTPDockingPaneSplitterWnd*, CXTPDockingPaneSplitterWnd*> m_lstSpliters;  // List of splitter windows
	BOOL m_bHoriz;                        // TRUE if the splitter orientation is horizontal
	BOOL m_bRecalcLayout;                 // TRUE if RecalcLayout method currently executed.

private:
	friend class CXTPDockingPaneManager;
	friend class CXTPDockingPaneSplitterWnd;
	friend class CXTPDockingPaneMiniWnd;
	friend class CXTPDockingPaneLayout;
};

AFX_INLINE BOOL CXTPDockingPaneSplitterWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE LONG& CXTPDockingPaneSplitterContainer::GetMinSize(LPMINMAXINFO pMinMaxInfo, BOOL bHoriz) {
	return bHoriz? pMinMaxInfo->ptMinTrackSize.x: pMinMaxInfo->ptMinTrackSize.y;
}
AFX_INLINE LONG& CXTPDockingPaneSplitterContainer::GetMaxSize(LPMINMAXINFO pMinMaxInfo, BOOL bHoriz) {
	return bHoriz? pMinMaxInfo->ptMaxTrackSize.x: pMinMaxInfo->ptMaxTrackSize.y;
}


#endif // #if !defined(__XTPDOCKINGPANESPLITTERCONTAINER_H__)
