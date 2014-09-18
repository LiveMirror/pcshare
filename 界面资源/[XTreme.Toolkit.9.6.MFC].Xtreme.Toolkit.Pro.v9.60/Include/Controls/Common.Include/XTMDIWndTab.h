// XTMDIWndTab.h : header file
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
#if !defined(__XTMDIWNDTAB_H__)
#define __XTMDIWNDTAB_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// forwards
class CXTMDIWndTab;

//===========================================================================
// Summary:
//     CXTMDIClientWnd is a CWnd derived helper class for the CXTMDIWndTab tab
//     control. This class routes messages sent to the client window back
//     to the tab control.
//===========================================================================
class _XT_EXT_CLASS CXTMDIClientWnd : public CWnd
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTMDIClientWnd object
	// Parameters:
	//     pMDIWndTab - Pointer to a CXTMDITabWnd object.
	//-----------------------------------------------------------------------
	CXTMDIClientWnd(CXTMDIWndTab* pMDIWndTab);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTMDIClientWnd object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTMDIClientWnd();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the size of the gap between the client
	//     area and the tab control.
	// Parameters:
	//     iSize - Amount in pixels of gap between the tab control and the client.
	//-----------------------------------------------------------------------
	void SetBorderGap(int iSize);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTMDIClientWnd)
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTMDIClientWnd)
	afx_msg LRESULT OnMDICreate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMDIDestroy(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMDIActivate(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE


protected:
	int             m_iBorderGap; // Amount in pixels between the client and the tab control.
	CXTMDIWndTab*   m_pMDIWndTab; // Pointer to the MDI tab control.
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTMDIClientWnd::SetBorderGap(int iBorderGap) {
	m_iBorderGap = iBorderGap;
}


// -------------------------------------------------------------------
// Summary:
//     CXTMDIWndTab class is used for creating a tabbed MDI document
//     interface.
// Remarks:
//     CXTMDIWndTab is derived from CTabCtrl and CXTTabCtrlBase and is
//     used to create a tab control for a multiple document interface
//     (MDI) application.
// Example:
// <code>
// // Install the MDI tab window.
// m_wndMDITabWindow.Install(this, NULL, TRUE);
// m_wndMDITabWindow.SetBorderGap(1);
// </code>
// -------------------------------------------------------------------
class _XT_EXT_CLASS CXTMDIWndTab : public CTabCtrl, public CXTTabCtrlBase
{
	DECLARE_DYNAMIC(CXTMDIWndTab)

	// ----------------------------------------------------------------------
	// Remarks:
	//     MDICHILD structure is used by the CXTMDIWndTab class for
	//     maintaining information on each window in a MDI tab.
	// See Also:
	//     CMDIChildList, CXTMDIWndTab
	// ----------------------------------------------------------------------
	struct MDICHILD
	{
		int     iItem;      // Index of the item in the tab control.
		HWND    hWnd;       // Window handle of the tab item.
		CString strItem;    // Tab label.
	};

	// ----------------------------------------------------------------------
	// Summary:
	//     List for maintaining MDICHILD structures.
	// Remarks:
	//     CList definition used by the CXTMDIWndTab class to maintain a list
	//     \of MDICHILD structures representing each MDI child item in the
	//     tab control.
	// See Also:
	//     CXTMDIWndTab, MDICHILD
	// ----------------------------------------------------------------------
	typedef CList<MDICHILD*,MDICHILD*> CMDIChildList;

	// ----------------------------------------------------------------------
	// Summary:
	//     Map for maintaining HWND to HICON handle relationships.
	// Remarks:
	//     CMap definition used by the CXTMDIWndTab class to maintain
	//     a list of HWND to HICON handle relationships to map a MDI child
	//     window with the tab icon displayed.
	// See Also:
	//     CXTMDIWndTab
	// -----------------------------------------------------------------
	typedef CMap<HWND, HWND, HICON, HICON&> CIconWndMap;

	// ----------------------------------------------------------------------
	// Summary:
	//     Map for maintaining HWND to CString object relationships.
	// Remarks:
	//     CMap definition used by the CXTMDIWndTab class to maintain
	//     a list of HWND to CString object relationships to map a MDI child
	//     window with the tab label displayed.
	// See Also:
	//     CXTMDIWndTab
	// -----------------------------------------------------------------
	typedef CMap<HWND, HWND, CString, CString&> CStringMap;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTMDIWndTab object
	//-----------------------------------------------------------------------
	CXTMDIWndTab();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTMDIWndTab object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTMDIWndTab();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the icon for the tab item specified
	//     by 'iIndex'.
	// Parameters:
	//     iIndex - Zero-based index of the tab to set the tab icon for.
	//     hIcon  - Reference to an HICON handle that represents the tab icon.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	bool SetTabIcon(int iIndex,HICON& hIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the icon for the tab item specified
	//     by 'hChildWnd'.
	// Parameters:
	//     hChildWnd - Valid HWND of the MDI child window to set the tab icon for.
	//     hIcon     - Reference to an HICON handle that represents the tab icon.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	bool SetTabIcon(HWND hChildWnd,HICON& hIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the label for the tab item specified
	//     by 'iIndex'.
	// Parameters:
	//     iIndex   - Zero-based index of the tab to set the tab text for.
	//     strLabel - Reference to a CString object that represents the tab label.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	bool SetTabLabel(int iIndex,CString& strLabel);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the label for the tab item specified
	//     by 'hChildWnd'.
	// Parameters:
	//     hChildWnd - Valid HWND of the MDI child window to set the tab text for.
	//     strLabel  - Reference to a CString object that represents the tab label.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	bool SetTabLabel(HWND hChildWnd,CString& strLabel);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will install the MDI tab views with your application.
	// Parameters:
	//     pMDIFrameWnd - Points to the parent MDI frame window.
	//     dwStyle      - Style for the tab control. It can be any of the TCS_ values.
	//     bNoIcons     - Set to TRUE for no icon to display
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Install(CMDIFrameWnd* pMDIFrameWnd,DWORD dwStyle=TCS_BOTTOM|TCS_HOTTRACK,BOOL bNoIcons=XTAuxData().bXPMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will uninstall the MDI tab views from your application.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL UnInstall();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the tab index from the current
	//     cursor position.
	// Parameters:
	//     point - Pointer to a CPoint object that contains the cursor screen
	//             coordinates. Use default for the current cursor position.
	// Returns:
	//     The zero-based index of the tab, or -1 if no tab is at the specified point.
	//-----------------------------------------------------------------------
	int TabFromPoint(CPoint point) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will return a pointer to the child frame associated
	//     with the tab item.
	// Parameters:
	//     iIndex - Index of the tab control. If -1, then the tab under the mouse when
	//              the last right click was performed will be used.
	// Returns:
	//     A CWnd object if successful, otherwise returns NULL.
	//-----------------------------------------------------------------------
	CMDIChildWnd* GetFrameWnd(int iIndex=-1) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will return a pointer to the child frame associated
	//     with the tab item.
	// Parameters:
	//     point - CPoint object that represents the cursor position.
	// Returns:
	//     A CWnd object if successful, otherwise returns NULL.
	//-----------------------------------------------------------------------
	CMDIChildWnd* GetFrameWnd(CPoint point) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to set the resource ID for the popup menu
	//     used by the tab control.
	// Parameters:
	//     popupMenuID - ID for the tab control popup menu.
	//     nPos        - Index position in the menu resource.
	//     nDefCmd     - ID of the default menu command, will display bold.
	//-----------------------------------------------------------------------
	void SetMenuID(UINT popupMenuID,int nPos = 0,UINT nDefCmd = (UINT)-1);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the size of the gap between the client
	//     area and the tab control.
	// Parameters:
	//     iSize - Amount in pixels of gap between the tab control and the client.
	//-----------------------------------------------------------------------
	void SetBorderGap(int iSize);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will get the text for the specified MDI child
	//     window.
	// Parameters:
	//     hWnd - Handle to a valid CMDIChildWnd object.
	// Returns:
	//     A const CString object that represents the document title.
	//-----------------------------------------------------------------------
	CString GetChildWndText(HWND hWnd) const;

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to initialize the font for the tab control
	//     associated with this view.
	//-----------------------------------------------------------------------
	virtual void InitializeFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to insert a child frame into the MDI
	//     tab control.
	// Parameters:
	//     pChildFrame   - A pointer to a valid child frame window.
	//     bRecalcLayout - TRUE to force the MDI frame window to recalc the layout.
	//-----------------------------------------------------------------------
	virtual void InsertTabWnd(CMDIChildWnd* pChildFrame,BOOL bRecalcLayout=TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the tab control to ensure the current
	//     selection matches the active document.
	// Returns:
	//     FALSE if there is no active document, otherwise returns TRUE.
	//-----------------------------------------------------------------------
	BOOL RefreshActiveSel();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the tab control to ensure the tab
	//     labels match their corresponding views.
	//-----------------------------------------------------------------------
	void RefreshTabLabels();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the tab control to ensure the stored
	//     indexes match their corresponding tab indexes.
	//-----------------------------------------------------------------------
	void RefreshIndexes();

	//-----------------------------------------------------------------------
	// Summary:
	//     Called when an WM_MDICREATE message is sent to a multiple-document
	//     interface (MDI) client window to create an MDI child window.
	// Parameters:
	//     hWnd - handle to destination control
	//-----------------------------------------------------------------------
	virtual void OnMDICreate(HWND hWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called when an WM_MDIDESTROY message is sent to a multiple-document
	//     interface (MDI) client window to close an MDI child window.
	// Parameters:
	//     hWnd - handle to destination control
	//-----------------------------------------------------------------------
	virtual void OnMDIDestroy(HWND hWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called when an WM_MDIACTIVATE message is sent to a multiple-document
	//     interface (MDI) client window to instruct the client window
	//     to activate a different MDI child window.
	// Parameters:
	//     hWnd - handle to destination control
	//-----------------------------------------------------------------------
	virtual void OnMDIActivate(HWND hWnd);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTMDIWndTab)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnIdleUpdateCmdUI();
	afx_msg LRESULT OnXtUpdate(WPARAM, LPARAM);
	afx_msg void OnPaint();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTabClose();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:

	int                 m_iBorderGap;           // Amount in pixels between the client and the tab control.
	int                 m_iHitTest;             // Index of the tab that received a right click.
	int                 m_nPos;                 // Index of the popup menu contained in the menu.
	UINT                m_nDefCmd;              // Command ID of the default menu item for the popup menu.
	UINT                m_popupMenuID;          // Popup menu resource ID.
	HWND                m_hActiveChild;         // Active MDI child.
	BOOL                m_bNoIcons;             // TRUE if no icons are used.
	BOOL                m_bRecalcLayoutPending; // Tells if a request to recalc the parent frame is pending.
	DWORD               m_dwInitSignature;      // Initialization state signature to synch up posted init requests.
	CImageList          m_imageList;            // Tab image list.
	CMDIFrameWnd*       m_pMDIFrameWnd;         // Points to the owner frame.
	CMDIChildList      m_arMDIChildern;         // Array of the MDI windows added to the tab control.
	CXTMDIClientWnd*    m_pMDIClientWnd;        // Window that receives messages on behalf of the MDI client.
	CIconWndMap       m_mapTabIcons;            // Hash table that maps icons to the MDI child frame windows.
	CStringMap        m_mapTabLabels;           // Hash table that maps labels to the MDI child frame windows.

	friend class CXTMDIClientWnd;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTMDIWndTab::SetMenuID(UINT popupMenuID, int nPos, UINT nDefCmd) {
	m_popupMenuID = popupMenuID; m_nPos = nPos; m_nDefCmd = nDefCmd;
}
AFX_INLINE void CXTMDIWndTab::SetBorderGap(int iBorderGap) {
	m_iBorderGap = iBorderGap; m_pMDIClientWnd->SetBorderGap(iBorderGap + ::GetSystemMetrics(SM_CXSIZEFRAME));
}

#endif // !defined(__XTMDIWNDTAB_H__)
