// XTPDockingPaneBase.h : interface for the CXTPDockingPaneBase class.
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
#if !defined(__XTPDOCKINGPANEBASE_H__)
#define __XTPDOCKINGPANEBASE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPDockingPaneDefines.h"

class CXTPDockingPaneBase;
class CXTPDockingPane;
class CXTPDockingPaneTabbedContainer;
class CXTPDockingPaneLayout;
class CXTPDockingPaneManager;
class CXTPDockingPanePaintManager;
class CXTPDockingPaneCaptionButton;
class CXTPPropExchange;

//===========================================================================
// Summary:
//     CXTPDockingPaneCaptionButtons is CArray derived class used for manipulate Caption Buttons.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneCaptionButtons: public CArray <CXTPDockingPaneCaptionButton*, CXTPDockingPaneCaptionButton*>
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the mouse pointer is positioned
	//     over one of the buttons in the pane caption.
	// Parameters:
	//     pt - CPoint object containing coordinates of mouse pointer.
	// Returns:
	//     TRUE if the mouse pointer is positioned over a caption button,
	//     FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL CheckForMouseOver(CPoint pt);
};


//===========================================================================
// Summary:
//     CXTPDockingPaneBase is a stand alone class. It represents the parent
//     class for a Docking Pane and its containers.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneBase
{
protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Protected constructor. You cannot create this class.
	// Parameters:
	//     type    - Docking Pane type. Can be any of the values listed in the Remarks section.
	//     pLayout - Points to a CXTPDockingPaneLayout object.
	// Remarks:
	//     Docking Pane type can be one of the following:
	//     * <b>xtpPaneTypeDockingPane</b> Indicates the pane's style is a docking pane.
	//     * <b>xtpPaneTypeTabbedContainer</b> Indicates the pane's style is a tabbed container for pane.
	//     * <b>xtpPaneTypeSplitterContainer</b> Indicates the pane's style is a splitter container.
	//     * <b>xtpPaneTypeMiniWnd</b> Indicates the pane's style is a floating window container.
	//     * <b>xtpPaneTypeClient</b> Indicates the pane's style is a container for client area.
	//     * <b>xtpPaneTypeAutoHidePanel</b> Indicates the pane's style is an auto-hide panel container.
	//-----------------------------------------------------------------------
	CXTPDockingPaneBase(XTPDockingPaneType type, CXTPDockingPaneLayout* pLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneBase object, handles cleanup and
	//     deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPDockingPaneBase();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine the type of the pane.
	// Returns:
	//     The type of the Pane. It can be one of the following:
	//     * <b>xtpPaneTypeDockingPane</b> Indicates the pane's style is a docking pane.
	//     * <b>xtpPaneTypeTabbedContainer</b> Indicates the pane's style is a tabbed container for pane.
	//     * <b>xtpPaneTypeSplitterContainer</b> Indicates the pane's style is a splitter container.
	//     * <b>xtpPaneTypeMiniWnd</b> Indicates the pane's style is a floating window container.
	//     * <b>xtpPaneTypeClient</b> Indicates the pane's style is a container for client area.
	//     * <b>xtpPaneTypeAutoHidePanel</b> Indicates the pane's style is an auto-hide panel container.
	//-----------------------------------------------------------------------
	XTPDockingPaneType GetType();

	//-----------------------------------------------------------------------
	// Summary:
	//     The GetWindowRect function retrieves the dimensions of the
	//     bounding rectangle of the pane.
	// Returns:
	//     The visible rectangle of the pane.
	//-----------------------------------------------------------------------
	CRect GetWindowRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the parent container of
	//     the pane.
	// Returns:
	//     The parent container.
	//-----------------------------------------------------------------------
	CXTPDockingPaneBase* GetContainer();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the pane manager
	// Returns:
	//     A pointer to a CXTPDockingPaneManager object
	//-----------------------------------------------------------------------
	CXTPDockingPaneManager* GetDockingPaneManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the paint manager
	// Returns:
	//     A pointer to a CXTPDockingPanePaintManager object
	//-----------------------------------------------------------------------
	CXTPDockingPanePaintManager* GetPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to find the specified caption button
	// Parameters:
	//     nID - Identifier of the button
	// Returns:
	//     The CXTPDockingPaneCaptionButton object if successful; otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPDockingPaneCaptionButton* FindCaptionButton(int nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get a reference to the caption's close/
	//     popup button
	// Returns:
	//     A reference to a CXTPDockingPaneCaptionButtons object
	//-----------------------------------------------------------------------
	CXTPDockingPaneCaptionButtons* GetCaptionButtons() {
		return &m_arrCaptionButtons;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to redraw the pane.
	//-----------------------------------------------------------------------
	virtual void RedrawPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if one pane is contained in the
	//     collection of another.
	// Parameters:
	//     pPane - Points to a CXTPDockingPaneBase object
	// Returns:
	//     POSITION of the pane in the collection if successful; otherwise
	//     returns NULL.
	//-----------------------------------------------------------------------
	virtual POSITION ContainPane(CXTPDockingPaneBase* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve a collection of the panes with
	//     specified type.
	// Parameters:
	//     type  - Type of the panes to find.
	//     pList - Points to a CXTPDockingPaneBaseList object that will
	//             contain found panes.
	// See Also:
	//     CXTPDockingPaneBaseList
	//-----------------------------------------------------------------------
	virtual void FindPane(XTPDockingPaneType type, CXTPDockingPaneBaseList* pList);

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
	virtual void GetMinMaxInfo(LPMINMAXINFO pMinMaxInfo);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to set position of the pane.
	// Parameters:
	//     pParent - Points to a CFrameWnd object.
	//     rect - Rectangle of the pane to be set.
	//     lParam - Pointer to AFX_SIZEPARENTPARAMS structure.
	//-----------------------------------------------------------------------
	virtual void OnSizeParent(CFrameWnd* pParent, CRect rect, LPVOID lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to update the state of the pane.
	//-----------------------------------------------------------------------
	virtual void OnIdleUpdate();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the parent frame of the pane.
	// Parameters:
	//     pFrame - Points to a CFrameWnd object
	//-----------------------------------------------------------------------
	virtual void SetParentFrame(CFrameWnd* pFrame);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the parent container of the pane.
	// Parameters:
	//     pContainer - Points to a CXTPDockingPaneBase object
	//-----------------------------------------------------------------------
	virtual void SetParentContainer(CXTPDockingPaneBase* pContainer);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when a child pane collection is changed.
	// Parameters:
	//     pContainer - Points to a CXTPDockingPaneBase object that is changed.
	//-----------------------------------------------------------------------
	virtual void OnChildConainerChanged(CXTPDockingPaneBase* pContainer);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the parent pane collection is changed.
	// Parameters:
	//     pContainer - Points to a CXTPDockingPaneBase object that is changed.
	//-----------------------------------------------------------------------
	virtual void OnParentConainerChanged(CXTPDockingPaneBase* pContainer);

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
	//     This member is called to clone the pane.
	// Parameters:
	//     pLayout - Points to a CXTPDockingPaneLayout object.
	//     pMap - Points to a CXTPPaneToPaneMap object.
	//     dwIgnoredOptions - Options that must be skipped.
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
	//     pPane - Points to a CXTPDockingPaneBase object
	//-----------------------------------------------------------------------
	virtual void RemovePane(CXTPDockingPaneBase* pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if there are children panes
	// Returns:
	//     TRUE if no children panes; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsEmpty();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the dimensions of the caption button
	//     are changed.
	// Parameters:
	//     pButton - caption button pointer.
	//-----------------------------------------------------------------------
	virtual void OnCaptionButtonRectChanged(CXTPDockingPaneCaptionButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the caption button is visible.
	// Parameters:
	//     pButton - caption button pointer.
	// Returns:
	//     TRUE if the caption button is visible; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsCaptionButtonVisible(CXTPDockingPaneCaptionButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the window handle for a window.
	// Returns:
	//     Returns NULL if the HWND is not attached to a window or if it
	//     is used with a NULL HWND pointer.
	//-----------------------------------------------------------------------
	virtual HWND GetSafeHwnd() {
		return 0;
	}

protected:
	CSize m_szDocking;                          // Size of the pane.
	CXTPDockingPaneLayout* m_pLayout;           // Parent layout.

	CXTPDockingPaneBase* m_pParentContainer;    // Parent container.
	CFrameWnd* m_pParentFrame;                  // Parent frame.
	XTPDockingPaneType m_type;                  // Type of the pane.
	CRect m_rcWindow;                           // Position of the pane.
	CXTPDockingPaneCaptionButtons m_arrCaptionButtons;  // Collection of caption buttons.

private:
	int m_nLength;
	int m_nIndex;


private:
	friend class CXTPDockingPaneManager;
	friend class CXTPDockingPaneSplitterContainer;
	friend class CXTPDockingPaneTabbedContainer;
	friend class CXTPDockingPaneMiniWnd;
	friend class CXTPDockingPaneSplitterWnd;
	friend class CXTPDockingPaneContext;
	friend class CXTPDockingPane;
	friend class CXTPDockingPaneLayout;
	friend class CXTPDockingPaneAutoHidePanel;
	friend class CXTPDockingPaneAutoHideWnd;
	friend class CXTPDockingPaneCaptionButton;

};

AFX_INLINE XTPDockingPaneType CXTPDockingPaneBase::GetType() {
	return m_type;
}
AFX_INLINE CXTPDockingPaneBase* CXTPDockingPaneBase::GetContainer() {
	return m_pParentContainer;
}
AFX_INLINE void CXTPDockingPaneBase::SetParentFrame(CFrameWnd* pFrame) {
	m_pParentFrame = pFrame;
}
AFX_INLINE void CXTPDockingPaneBase::SetParentContainer(CXTPDockingPaneBase* pContainer) {
	m_pParentContainer = pContainer;
}
AFX_INLINE void CXTPDockingPaneBase::OnCaptionButtonRectChanged(CXTPDockingPaneCaptionButton* ) {
}
AFX_INLINE BOOL CXTPDockingPaneBase::IsCaptionButtonVisible(CXTPDockingPaneCaptionButton* /*pButton*/) {
	return TRUE;
}

#endif // #if !defined(__XTPDOCKINGPANEBASE_H__)
