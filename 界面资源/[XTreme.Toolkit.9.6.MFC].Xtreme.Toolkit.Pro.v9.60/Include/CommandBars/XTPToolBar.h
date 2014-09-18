// XTPToolBar.h : interface for the CXTPToolBar class.
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
#if !defined(__XTPTOOLBAR_H__)
#define __XTPTOOLBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPCommandBar.h"
#include "CommonPro.include/XTPManageState.h"

class CXTPDockBar;
class CXTPCommandBars;
class CXTPDockContext;

//===========================================================================
// Summary:
//     CXTPToolBar is a CXTPCommandBar derived class.
//===========================================================================
class _XTP_EXT_CLASS CXTPToolBar : public CXTPCommandBar
{
public:
//{{AFX_CODEJOCK_PRIVATE
	class CToolBarInfo;
//}}AFX_CODEJOCK_PRIVATE

private:
	class CXTPControlExpandButton;
	class CXTPControlButtonHide;
	class CXTPControlButtonCustomize;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPToolBar object
	//-----------------------------------------------------------------------
	CXTPToolBar(void);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPToolBar object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPToolBar(void);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to show or hide the toolbar.
	// Parameters:
	//     bVisible - TRUE to show the toolbar; FALSE to hide.
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a Boolean value that indicates whether the toolbar is
	//     visible or hidden.
	// Returns:
	//     TRUE if the toolbar is visible.
	//-----------------------------------------------------------------------
	BOOL IsVisible() { return m_bVisible; }

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the position of the toolbar.
	// Parameters:
	//     barPosition - New position of toolbar.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL SetPosition(XTPBarPosition barPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to load the toolbar specified by nIDResource.
	// Parameters:
	//     nIDResource - Resource ID of the toolbar to be loaded.
	//     bLoadIcons - TRUE to load icons from application resources.
	// Returns:
	//     Nonzero if successful; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL LoadToolBar(UINT nIDResource, BOOL bLoadIcons = TRUE);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method creates the ToolBar control.
	// Parameters:
	//     dwStyle - Styles for the toolbars.
	//     pParentWnd - Specifies the parent window of the TaskPanel control.
	//     nID - Specifies the Toolbar control ID.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL CreateToolBar(DWORD dwStyle, CWnd* pParentWnd, UINT nID = AFX_IDW_TOOLBAR);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves bar information for serialization routines.
	// Parameters:
	//     pInfo - Points to a CToolBarInfo object.
	//-----------------------------------------------------------------------
	void GetBarInfo(CToolBarInfo* pInfo);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets bar information.
	// Parameters:
	//     pInfo - Points to a CToolBarInfo object.
	//-----------------------------------------------------------------------
	void SetBarInfo(CToolBarInfo* pInfo);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method calculates the dimensions of a toolbar.
	// Parameters:
	//     nLength - Length of the bar.
	//     dwMode - Mode to dock.
	//     nWidth - Width of the bar.
	// Returns:
	//     Size of the docked toolbar.
	//-----------------------------------------------------------------------
	virtual CSize CalcDockingLayout(int nLength, DWORD dwMode, int nWidth = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method calculates the dimensions of a toolbar.
	// Parameters:
	//     dwMode - Mode to dock.
	// Returns:
	//     Size of the docked toolbar.
	//-----------------------------------------------------------------------
	virtual CSize CalcDynamicLayout(int, DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set docking flags.
	// Parameters:
	//     dwFlags - Flags of docking. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     dwFlags parameter can be one or more of the following:
	//     * <b>xtpFlagAlignTop</b> Allows docking at the top of the client area. .
	//     * <b>xtpFlagAlignBottom</b> Allows docking at the bottom of the client area.
	//     * <b>xtpFlagAlignLeft</b> Allows docking on the left side of the client area.
	//     * <b>xtpFlagAlignRight</b> Allows docking on the left side of the client area.
	//     * <b>xtpFlagFloating</b> Allows floating.
	//     * <b>xtpFlagHideWrap</b> Allows to hide wrapped controls..
	//     * <b>xtpFlagStretched</b> Indicates the bar is stretched..
	//-----------------------------------------------------------------------
	void EnableDocking(DWORD dwFlags = xtpFlagAlignAny | xtpFlagFloating | xtpFlagHideWrap);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the toolbar is built-in.
	// Returns:
	//     TRUE if toolbar is built-in; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL IsBuiltIn();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the identifier.
	// Parameters:
	//     nID - Identifier to set.
	//-----------------------------------------------------------------------
	void SetBarID(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the parent dock bar.
	// Returns:
	//     A pointer to a CXTPDockBar object.
	//-----------------------------------------------------------------------
	CXTPDockBar* GetDockBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Resets the controls to their original state.
	//-----------------------------------------------------------------------
	virtual void Reset();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to hide/show toolbars in preview mode.
	// Parameters:
	//     bPreview - Preview mode.
	//-----------------------------------------------------------------------
	void OnSetPreviewMode(BOOL bPreview);

	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function when a non-system key
	//     is pressed.
	// Parameters:
	//     nChar - Specifies the virtual key code of the given key.
	// Returns:
	//     TRUE if key handled, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL OnKeyDown(UINT nChar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method allows to hide toolbar in a context menu.
	// Parameters:
	//     bPresent - TRUE to show the toolbar in a context menu.
	//-----------------------------------------------------------------------
	void SetContextMenuPresent(BOOL bPresent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if this toolbar is included in
	// the right-click context menu.
	//-----------------------------------------------------------------------
	BOOL IsContextMenuPresent();

	//-----------------------------------------------------------------------
	// Summary:
	//     Modifies the styles of the toolbar.
	// Parameters:
	//     dwRemove - Styles to remove.
	//     dwAdd - Styles to add.
	//-----------------------------------------------------------------------
	void ModifyBarStyle(DWORD dwRemove, DWORD dwAdd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable/disable close able property of the
	//     command bar.
	// Parameters:
	//     bCloseable - TRUE to allow toolbar to be closed by user.
	//-----------------------------------------------------------------------
	void SetCloseable(BOOL bCloseable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the close able property of the
	//     command bar.
	// Returns:
	//     TRUE if the control is close able.
	//-----------------------------------------------------------------------
	BOOL IsCloseable();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if the control is temporary.
	// Remarks:
	//     Temporary toolbars will not be saved to the system registry when
	//     the application is closed (If SaveCommandBars is used), and they
	//     will not be restored when the application is opened
	//     (If LoadCommandBars is used).
	// Returns:
	//     TRUE if the control is temporary.
	//-----------------------------------------------------------------------
	BOOL IsTemporary();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set this control to temporary.
	// Parameters:
	//     bTemporary - TRUE if this control is temporary.
	// Remarks:
	//     Temporary toolbars will not be saved to the system registry when
	//     the application is closed (If SaveCommandBars is used), and they
	//     will not be restored when the application is opened
	//     (If LoadCommandBars is used).
	//-----------------------------------------------------------------------
	void SetTemporary(BOOL bTemporary);


protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a toolbar.
	// Parameters:
	//     pParentWnd - Points to a CWnd object.
	//     bFloat     - TRUE to create a floating bar.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Create(CWnd* pParentWnd, BOOL bFloat);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method recalculates the position of the toolbar.
	//-----------------------------------------------------------------------
	void OnRecalcLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method makes a copy of the command bar.
	// Parameters:
	//     pCommandBar - Command bar needed to be copied.
	//     bRecursive  - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	virtual void Copy(CXTPCommandBar* pCommandBar, BOOL bRecursive = FALSE);

private:
	CSize _CalcDynamicLayout(int nLength, DWORD dwMode);

	void BuildCustomizePopup(CXTPCommandBar* pExpandBar);
	int _FindNearest(CXTPControls* pControls, CXTPControl* pFind, int nPos, BOOL bVisible = FALSE);
	void _RestoreDeletedControls();

	BOOL IsFloatingFrameFocused();

protected:
	CXTPDockBar* m_pDockBar;            // Parent dock bar.
	CXTPDockContext* m_pDockContext;    // Docking context.
	BOOL m_bBuiltIn;                    // TRUE if toolbar is built-in.
	BOOL m_bTearOff;                    // TRUE if toolbar is tear-offed.
	BOOL m_bPreviewVisible;             // TRUE if Toolbar was visible in preview mode.
	BOOL m_bPreviewMode;                // TRUE if Toolbar was visible in preview mode.
	BOOL m_bTemporary;                  // TRUE if Toolbar is temporary (If it will be saved when the CommandBar layout is saved.)

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPToolBar)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg LRESULT OnFloatStatus(WPARAM wParam, LPARAM);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnClose();
	afx_msg LRESULT OnSizeParent(WPARAM, LPARAM lParam);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint pt);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	DECLARE_XTP_COMMANDBAR(CXTPToolBar)

private:
	friend class CXTPCommandBars;
	friend class CXTPDockContext;
	friend class CXTPDockBar;
	friend class CXTPCustomizeToolbarsPage;
	friend class CXTPPopupBar;
};

//////////////////////////////////////////////////////////////////////////


AFX_INLINE BOOL CXTPToolBar::IsBuiltIn() {
	return m_bBuiltIn;
}

AFX_INLINE void CXTPToolBar::SetBarID(UINT nID) {
	m_nBarID = nID;
}
AFX_INLINE CXTPDockBar* CXTPToolBar::GetDockBar()
{
	return m_pDockBar;
}
AFX_INLINE void CXTPToolBar::SetContextMenuPresent(BOOL bPresent) {
	m_bContextMenuPresent = bPresent;
}
AFX_INLINE BOOL CXTPToolBar::IsContextMenuPresent() {
	return m_bContextMenuPresent;
}
AFX_INLINE void CXTPToolBar::ModifyBarStyle(DWORD dwRemove, DWORD dwAdd) {
	m_dwStyle |= dwAdd;
	m_dwStyle &= ~dwRemove;
}
AFX_INLINE void CXTPToolBar::SetCloseable(BOOL bCloseable) {
	m_bCloseable = bCloseable;
}
AFX_INLINE BOOL CXTPToolBar::IsCloseable() {
	return m_bCloseable;
}
AFX_INLINE BOOL CXTPToolBar::IsTemporary() {
	return m_bTemporary;

}
AFX_INLINE void CXTPToolBar::SetTemporary(BOOL bTemporary) {
	m_bTemporary = bTemporary;
}
AFX_INLINE BOOL CXTPToolBar::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

#endif //#if !defined(__XTPTOOLBAR_H__)
