// XTPTabControl.h: interface for the CXTPTabControl class.
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
#if !defined(__XTPTABCONTROL_H__)
#define __XTPTABCONTROL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTPTabManager.h"


class CXTPTabPaintManager;
class CXTPImageManager;

//===========================================================================
// Summary:
//     CXTPTabControl is a multiple inheritance class derived from CXTPTabManager and
//     CWnd. It is used to draw enhance tab control with Office 2003, Visual Studio, Win XP tabs.
// See Also:
//     Create, CXTPTabManager::SetColor, CXTPTabManager::SetAppearance
//===========================================================================
class _XTP_EXT_CLASS CXTPTabControl : public CWnd, public CXTPTabManager
{
private:
	class CTabControlDropTarget;

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabControl object.
	//-------------------------------------------------------------------------
	CXTPTabControl();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabControl object, handles cleanup and de-allocation
	//-------------------------------------------------------------------------
	virtual ~CXTPTabControl();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts a new tab at the position specified by nItem.
	// Parameters:
	//     nItem     - Index of the new tab.
	//     lpszItem  - Caption of the new tab.
	//     hwndChild - Handle to the object to be displayed in the tab. I.e. A Dialog page
	//     nImage    - Id of image resource to display in tab is icons are displayed.
	// Returns:
	//     A pointer to the newly added CXTPTabManagerItem.
	//-----------------------------------------------------------------------
	CXTPTabManagerItem* InsertItem(int nItem, LPCTSTR lpszItem, HWND hwndChild = NULL, int nImage = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to create the Tab Control.
	// Parameters:
	//     dwStyle    - Style of the Tab Control.
	//     rect       - Size of the control.
	//     pParentWnd - Parent of the control.
	//     nID        - Identifier of the Tab control.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	// See Also:
	//     CXTPTabControl::CXTPTabControl, GetPaintManager, CXTPTabManager::SetPosition,
	//     CXTPTabManager::SetLayout, GetImageManager
	//-----------------------------------------------------------------------
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);


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
	virtual CXTPTabPaintManager* GetPaintManager();

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
	//     This member recalculates the layout of the tab manager and
	//     then repositions itself.  This member must be overridden in
	//     derived classes.
	//-------------------------------------------------------------------------
	virtual void Reposition();

	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called when visual properties of the tabs are
	//     changed.  For example, color, mouse-over, and on-click. This member
	//     must be overridden in derived classes.
	//-------------------------------------------------------------------------
	virtual void RedrawControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to select a tab.
	// Parameters:
	//     pItem - Pointer to the tab to be selected.
	//-----------------------------------------------------------------------
	virtual void SetSelectedItem(CXTPTabManagerItem* pItem);

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
	virtual BOOL DrawIcon(CDC* pDC, CPoint pt, CXTPTabManagerItem* pItem, BOOL bDraw, CSize& szIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set icons of the tabs.
	// Parameters:
	//     pImageList - Image list containing icons of the tabs
	// Remarks:
	//     Recommended to use GetImageManager method and CXTPImageManager::SetIcons to add icons.
	//-----------------------------------------------------------------------
	void SetImageList(CImageList* pImageList);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get a pointer to the image manager collected icons of tab control.
	// Returns:
	//     Pointer to CXTPImageManager that contains the icons of the tabs.
	// Example:
	//     <code>m_wmdTabControl.GetImageManager()->SetIcons(IDR_MAINFRAME);</code>
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager();

protected:

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
	void OnNavigateButtonClick(UINT nID);


protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPTabControl)
	virtual void PreSubclassWindow();
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPTabControl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

private:
	HWND m_hwndClient;

	CXTPTabPaintManager* m_pPaintManager;
	CXTPImageManager* m_pImageManager;
	CTabControlDropTarget* m_pDropTarget;


	friend class CTabControlDropTarget;
};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPTabPaintManager* CXTPTabControl::GetPaintManager() {
	return m_pPaintManager;
}
AFX_INLINE CXTPImageManager* CXTPTabControl::GetImageManager() {
	return m_pImageManager;
}
AFX_INLINE BOOL CXTPTabControl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

#endif // !defined(__XTPTABCONTROL_H__)
