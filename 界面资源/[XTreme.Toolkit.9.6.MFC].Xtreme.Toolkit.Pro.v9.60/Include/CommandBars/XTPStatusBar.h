// XTPStatusBar.h : interface for the CXTPStatusBar class.
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
#if !defined(__XTPSTATUSBAR_H__)
#define __XTPSTATUSBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPCommandBars;
class CXTPPaintManager;

// ----------------------------------------------------------------------
// Summary:
//     XTP_STATUSPANE structure is used to maintain information for
//     a particular pane in a CXTPStatusBar window.
// See Also:
//     CXTPStatusBar
// ----------------------------------------------------------------------
struct XTP_STATUSPANE
{
	UINT    nID;        // IDC of indicator: 0 => normal text area.
	int     cxText;     // Width of the string area in pixels. On both sides there is a 3 pixel gap and a 1 pixel border, making a pane 6 pixels wider.
	UINT    nStyle;     // Style flags (SBPS_*).
	UINT    nFlags;     // State flags (SBPF_*).
	CString strText;    // Text in the pane.
};

// ---------------------------------------------------------------------
// Summary:
//     XTP_STATUSPANE_CTRL structure is used to maintain
//     information for a CWnd object that has been added to a particular
//     pane in a CXTPStatusBar window.
// See Also:
//     CXTPStatusBar
// ---------------------------------------------------------------------
struct XTP_STATUSPANE_CTRL
{
	CWnd* pWnd;         // A pointer to a valid CWnd object.
	UINT  nID;          // ID of the indicator pane.
	BOOL  bAutoDelete;  // TRUE if the control is to be deleted when destroyed.
};


//===========================================================================
// Summary:
//     CXTPStatusBar is a CStatusBar derived class.
//===========================================================================
class _XTP_EXT_CLASS CXTPStatusBar : public CStatusBar
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPStatusBar object
	//-----------------------------------------------------------------------
	CXTPStatusBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPStatusBar object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPStatusBar();
public:


	//-----------------------------------------------------------------------
	// Summary:
	//     Sets each indicator's ID to the value specified by the corresponding
	//     element of the array lpIDArray, loads the string resource specified
	//     by each ID, and sets the indicator's text to the string.
	// Parameters:
	//     lpIDArray - Pointer to an array of IDs.
	//     nIDCount  - Number of elements in the array pointed to by lpIDArray.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL SetIndicators (const UINT* lpIDArray, int nIDCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the command bar's object.
	// Parameters:
	//     pCommandBars - CXTPCommandBars to be set.
	//-----------------------------------------------------------------------
	void SetCommandBars(CXTPCommandBars* pCommandBars);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the paint manager.
	// Returns:
	//     A pointer to a CXTPPaintManager object.
	//-----------------------------------------------------------------------
	CXTPPaintManager* GetPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to allow status bar use global command bars theme
	// Parameters:
	//     bCommandBarsTheme - TRUE to allow.
	//-----------------------------------------------------------------------
	void UseCommandBarsTheme(BOOL bCommandBarsTheme);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the status bar to reposition pane
	//     item controls.
	//-----------------------------------------------------------------------
	void PositionControls();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve information for the specified
	//     indicator pane.
	// Parameters:
	//     nID - ID of the indicator pane.
	// Returns:
	//     A pointer to an XTP_STATUSPANE_CTRL struct.
	//-----------------------------------------------------------------------
	XTP_STATUSPANE_CTRL* GetPaneControl(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add a control to an indicator pane.
	// Parameters:
	//     pWnd        - Points to a control window.
	//     nID         - ID of the indicator pane.
	//     bAutoDelete - TRUE if the control is to be deleted when destroyed.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL AddControl(CWnd* pWnd,UINT nID,BOOL bAutoDelete=TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the width for an indicator pane.
	// Parameters:
	//     nIndex - Index of the indicator pane.
	//     cxText - New width for the indicator pane.
	//-----------------------------------------------------------------------
	void SetPaneWidth(int nIndex,int cxText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add an indicator pane to the status bar.
	// Parameters:
	//     nID    - ID of the indicator pane.
	//     nIndex - Index of the indicator pane.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL AddIndicator(UINT nID,int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove an indicator pane from the status bar.
	// Parameters:
	//     nID - ID of the indicator pane.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL RemoveIndicator(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve information for an indicator pane.
	// Parameters:
	//     nIndex - Index of the indicator pane.
	//     pSBP   - Address of an XTP_STATUSPANE struct.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL GetPaneInfoEx(int nIndex,XTP_STATUSPANE* pSBP);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the information for an indicator pane.
	// Parameters:
	//     nIndex - Index of the indicator pane.
	//     pSBP   - Address of an XTP_STATUSPANE struct.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL SetPaneInfoEx(int nIndex,XTP_STATUSPANE* pSBP);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the pane information for the given index.
	// Parameters:
	//     nIndex - Index of the indicator pane.
	// Returns:
	//     A pointer to an XTP_STATUSPANE struct.
	//-----------------------------------------------------------------------
	XTP_STATUSPANE* GetPanePtr(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the number of panes that are created
	//     for the status bar.
	// Returns:
	//     An integer value that represents the number of panes for the status bar.
	//-----------------------------------------------------------------------
	int GetPaneCount() const;


protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the stretch gripper of the status bar.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rcClient - Client rectangle.
	//-----------------------------------------------------------------------
	void DrawStretch(CDC* pDC, CRect& rcClient);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPStatusBar)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	public:
	afx_msg LRESULT OnIdleUpdateCmdUI (WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	CArray <XTP_STATUSPANE_CTRL*,XTP_STATUSPANE_CTRL*> m_arControls;  // Array of pane controls

private:
	CXTPCommandBars* m_pCommandBars;
	CFont m_fontStatus;
	BOOL m_bCommandBarsTheme;
};

// --------------------------------------------------------------------
// Summary:
//     CXTPStatusBarBasePane is a CWnd derived class. It is the base class
//     used for creating CWnd objects to place in status bar panes that
//     do custom paint routines.
// --------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPStatusBarBasePane : public CWnd
{
	DECLARE_DYNCREATE(CXTPStatusBarBasePane)

public:
	// -------------------------------------------
	// Summary:
	//     Constructs a CXTPStatusBarBasePane object.
	// -------------------------------------------
	CXTPStatusBarBasePane();

	// -------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPStatusBarBasePane object, handles cleanup and
	//     deallocation
	// -------------------------------------------------------------
	virtual ~CXTPStatusBarBasePane();

public:
	// -----------------------------------------------------------------------
	// Summary:
	//     Call this member function to create a status bar pane object that
	//     performs custom draw routines.
	// Parameters:
	//     lpszWindowName -  [in] Text string to be associated with this pane.
	// pParentWnd -      [in] Pointer to the owner status bar window.
	// dwStyle -         [in] Window style.
	// nID -             [in] Control ID.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	// -----------------------------------------------------------------------
	virtual BOOL Create(LPCTSTR lpszWindowName,CWnd* pParentWnd,DWORD dwStyle=WS_CHILD|WS_VISIBLE,UINT nID=0xffff);

protected:

	// -------------------------------------------------------------------
	// Summary:
	//     Override this virtual function in your derived class to perform
	//     your custom drawing routines.
	// Parameters:
	//     pDC -       [in] Pointer to the device context.
	// rcClient -  [in] Size of the client area to draw.
	// -------------------------------------------------------------------
	virtual void DoPaint(CDC* pDC,CRect& rcClient);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPStatusBarBasePane)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:
	CString m_strWindowText;  // NULL terminated string that represents the pane text

};

//===========================================================================
// Summary:
//     CXTPStatusBarLogoPane is a CXTPStatusBarBasePane derived class. CXTPStatusBarLogoPane works with
//     CXTStatusBar and allows you to create a logo to place in your status
//     bar area.
//===========================================================================
class _XTP_EXT_CLASS CXTPStatusBarLogoPane : public CXTPStatusBarBasePane
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPStatusBarLogoPane object
	//-----------------------------------------------------------------------
	CXTPStatusBarLogoPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPStatusBarLogoPane object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPStatusBarLogoPane();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the text to be displayed in the logo
	//     pane.
	// Parameters:
	//     lpszLogoText - A NULL terminated string that represents the text to be displayed.
	//-----------------------------------------------------------------------
	virtual void SetLogoText(LPCTSTR lpszLogoText);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns a CString that represents the logo text.
	// Returns:
	//     A CString object that represents the text that is displayed
	//     in the logo pane.
	//-----------------------------------------------------------------------
	virtual CString GetLogoText();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the font to be displayed in the logo pane.
	// Parameters:
	//     lpszFontName - A NULL terminated string that represents the text to be displayed.
	//     nHeight      - Initial height for the font.
	//     nWeight      - Initial weight for the font.
	//     bItalic      - TRUE if the font is italic.
	//     bUnderline   - TRUE if the font is underlined.
	//-----------------------------------------------------------------------
	virtual void SetLogoFont(LPCTSTR lpszFontName,int nHeight=24,int nWeight=FW_BOLD,BOOL bItalic=TRUE,BOOL bUnderline=FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the font to be displayed in the logo pane.
	// Parameters:
	//     logFont - Address of a LOGFONT structure.
	//-----------------------------------------------------------------------
	virtual void SetLogoFont(LOGFONT& logFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will return the size of the text displayed in
	//     the logo pane.
	// Returns:
	//     A CSize object that represents the current size of the logo pane text.
	//-----------------------------------------------------------------------
	CSize GetTextSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will calculate the size of the text that is displayed
	//     in the logo pane and initializes 'm_sizeText'.
	//-----------------------------------------------------------------------
	void SetTextSize();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this virtual function in your derived class to perform your
	//     custom drawing routines.
	// Parameters:
	//     pDC      - Pointer to the device context.
	//     rcClient - Size of the client area to draw.
	//-----------------------------------------------------------------------
	virtual void DoPaint(CDC* pDC,CRect& rcClient);

	DECLARE_MESSAGE_MAP()

protected:
	CFont m_Font;       // Font to be used
	CSize m_sizeText;   // Size of the text to be displayed
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CSize CXTPStatusBarLogoPane::GetTextSize() {
	SetTextSize(); m_sizeText.cx+=2; return m_sizeText;
}
AFX_INLINE void CXTPStatusBarLogoPane::SetLogoText(LPCTSTR lpszLogoText) {
	m_strWindowText = lpszLogoText;
}
AFX_INLINE CString CXTPStatusBarLogoPane::GetLogoText() {
	return m_strWindowText;
}

//===========================================================================
// Summary:
//     CXTPStatusBarIconPane is a CXTPStatusBarBasePane derived class. CXTPStatusBarIconPane works with
//     CXTStatusBar and allows you to create a logo to place in your status
//     bar area.
//===========================================================================
class _XTP_EXT_CLASS CXTPStatusBarIconPane : public CXTPStatusBarBasePane
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPStatusBarIconPane object
	//-----------------------------------------------------------------------
	CXTPStatusBarIconPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPStatusBarIconPane object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPStatusBarIconPane();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the pane icon for this object.
	// Parameters:
	//     nIconID - Resource ID of the icon to display.
	//-----------------------------------------------------------------------
	void SetPaneIcon(int nIconID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the pane icon for this object.
	// Parameters:
	//     lpszIconID - Resource ID of the icon to display.
	//-----------------------------------------------------------------------
	void SetPaneIcon(LPCTSTR lpszIconID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this virtual function in your derived class to perform your
	//     custom drawing routines.
	// Parameters:
	//     pDC      - Pointer to the device context.
	//     rcClient - Size of the client area to draw.
	//-----------------------------------------------------------------------
	virtual void DoPaint(CDC* pDC,CRect& rcClient);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPStatusBarIconPane)
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	CSize           m_sizeIcon; // Size of the icon to display.
	HICON           m_hIcon;    // Icon that is displayed in the status pane.
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPStatusBarBasePane::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}



AFX_INLINE void CXTPStatusBar::SetCommandBars(CXTPCommandBars* pCommandBars) {
	m_pCommandBars = pCommandBars;
}
AFX_INLINE void CXTPStatusBar::UseCommandBarsTheme(BOOL bCommandBarsTheme) {
	m_bCommandBarsTheme = bCommandBarsTheme;
}
AFX_INLINE int CXTPStatusBar::GetPaneCount() const {
	return m_nCount;
}

#endif //#if !defined(__XTPSTATUSBAR_H__)
