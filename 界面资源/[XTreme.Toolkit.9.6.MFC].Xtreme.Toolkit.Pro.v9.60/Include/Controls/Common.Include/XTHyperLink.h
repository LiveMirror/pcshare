// XTHyperLink.h interface for the CXTHyperLink class.
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
#if !defined(__XTHYPERLINK_H__)
#define __XTHYPERLINK_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTHyperLink is a CStatic derived class. It is used to display a text
//     string that is used as a hyperlink control. The link's active, hover,
//     and selected color attributes are configurable.
//===========================================================================
class _XT_EXT_CLASS CXTHyperLink : public CStatic
{
	DECLARE_DYNAMIC(CXTHyperLink)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTHyperLink object.
	//-----------------------------------------------------------------------
	CXTHyperLink();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTHyperLink object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTHyperLink();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable ShellExecute to be called
	//     for the URL that is specified for the hyperlink control.  If disabled,
	//     you can add a BN_CLICKED message handler to perform your own task when
	//     the link is clicked.
	// Parameters:
	//     bEnable - true to enable ShellExecute, false to disable.
	//-----------------------------------------------------------------------
	void EnableShellExecute(bool bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the URL for the hyperlink control.
	// Parameters:
	//     lpszLink - A NULL terminated character string that
	//                represents the URL for the hyperlink.
	//-----------------------------------------------------------------------
	virtual void SetURL(LPCTSTR lpszLink);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the URL for the hyperlink.
	// Returns:
	//     A CString object that represents the URL for the hyperlink control.
	//-----------------------------------------------------------------------
	virtual CString GetURL() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the RGB color values for the hyperlink text.
	// Parameters:
	//     clrLink    - An RGB value representing the color for the hyperlink text color.
	//     clrVisited - An RGB value representing the color for the hyperlink text color
	//                  when the mouse is hovering.
	//     clrHover   - An RGB value representing the color for the hyperlink text color
	//                  when the URL has been visited.
	//-----------------------------------------------------------------------
	virtual void SetColors(COLORREF clrLink,COLORREF clrVisited,COLORREF clrHover);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the current hyperlink text color.
	// Returns:
	//     An RGB color value that represents the current hyperlink text color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetLinkColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the current hyperlink text color
	//     when the link has been activated.
	// Returns:
	//     An RGB color value that represents the current hyperlink text color when
	//     the link has been activated.
	//-----------------------------------------------------------------------
	virtual COLORREF GetVisitedColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the current hyperlink text color
	//     when the mouse hovers over the hyperlink.
	// Returns:
	//     An RGB color value that represents the current hyperlink text color when
	//     the mouse hovers over the hyperlink.
	//-----------------------------------------------------------------------
	virtual COLORREF GetHoverColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will cause the control to display the hyperlink
	//     text color as visited.
	// Parameters:
	//     bVisited - A boolean flag that, when set to true, causes the control to
	//                display the hyperlink text color as visited.
	//-----------------------------------------------------------------------
	virtual void SetVisited(bool bVisited = true);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks to see if the user has visited the URL
	//     associated with the hyperlink.
	// Returns:
	//     true if the URL has been visited, otherwise returns false.
	//-----------------------------------------------------------------------
	virtual bool GetVisited() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the cursor to be displayed when the
	//     mouse hovers over the hyperlink control.
	// Parameters:
	//     hCursor - Handle of the cursor to be associated with the hyperlink control.
	//-----------------------------------------------------------------------
	virtual void SetLinkCursor(HCURSOR hCursor);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will get a handle to the current cursor associated
	//     with the hyperlink control.
	// Returns:
	//     A handle to the current cursor for the hyperlink control.
	//-----------------------------------------------------------------------
	virtual HCURSOR GetLinkCursor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will cause the hyperlink to display the text
	//     as underlined.  The default value is 'true.'
	// Parameters:
	//     bUnderline - A boolean flag that, when set to true, causes the control to
	//                  display the text as underlined.
	//-----------------------------------------------------------------------
	virtual void SetUnderline(bool bUnderline = true);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks to see if the hyperlink text is underlined.
	// Returns:
	//     true if the hyperlink text is underlined, otherwise returns false.
	//-----------------------------------------------------------------------
	virtual bool GetUnderline() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the background fill color for the hyperlink
	//     control. By default the hyperlink control uses the system color for COLOR_3DFACE.
	// Parameters:
	//     clrBack - An RGB value representing the background fill color.
	//-----------------------------------------------------------------------
	virtual void SetBackColor(COLORREF clrBack);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will return the current background fill color for the
	//     hyperlink control.
	// Returns:
	//     A COLOREF value.
	//-----------------------------------------------------------------------
	virtual COLORREF GetBackColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to execute the shell command associated
	//     with the current URL.
	// Parameters:
	//     lpszLink - Points to a null terminated string that represents the URL to
	//                jump to.  If NULL, the default URL is activated.
	//     nShowCmd - Display state for shell command associated with the URL.  The default
	//                is SW_SHOW.
	// Returns:
	//     TRUE if SUCCESSFUL, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual bool GotoURL(LPCTSTR lpszLink=NULL,int nShowCmd=SW_SHOW);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks to see if the hyperlink control is a
	//     text control.
	// Returns:
	//     true if the hyperlink control is a text control, otherwise returns false.
	//-----------------------------------------------------------------------
	virtual bool IsTextControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable tooltips for the hyperlink
	//     control.
	// Parameters:
	//     bEnable - true to enable tooltips.
	//-----------------------------------------------------------------------
	void EnableToolTips(bool bEnable=true);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the tooltip text for the tooltip associated
	//     with the hyperlink control.
	// Parameters:
	//     lpszTipText - NULL terminated string that represents the new tooltip text to display.
	//-----------------------------------------------------------------------
	void SetTipText(LPCTSTR lpszTipText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the hyperlink is currently hot.
	// Remarks:
	//     If the link is hot, the mouse pointer is positioned over the link.
	// Returns:
	//     TRUE if the hyperlink is currently hot.  FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual bool IsLinkHot();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the CXTHyperLink object to
	//     display a message box whenever an error occurs.
	// Parameters:
	//     iErrorNo - See the ShellExecute API for a list of error codes.
	//-----------------------------------------------------------------------
	virtual void ReportError(int iErrorNo);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTHyperLink)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTHyperLink)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnClicked();
	afx_msg LRESULT OnInitControl(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	bool            m_bShellExec;   // true to cause the hyperlink to call ShellExecute for the specified URL.
	bool            m_bInitControl; // true for initialization.
	bool            m_bUnderline;   // Set to true for underlined text.
	bool            m_bMouseOver;   // Set to true when the cursor is hovering over the control.
	bool            m_bVisited;     // Set to true when the hyperlink has been activated.
	bool            m_bTipEnabled;  // Set to false to disable tooltips.
	CFont           m_font;         // Font used by hyperlink control.
	CString         m_strLink;      // String object that represents the URL for this control.
	CString         m_strTipText;   // String object that represents the tooltip text for this control.
	HCURSOR         m_hcurHand;     // Handle to the default cursor for this control.
	COLORREF        m_clrLink;      // An RGB value representing the color for the hyperlink text color.
	COLORREF        m_clrHover;     // An RGB value representing the color for the hyperlink text color when the mouse is hovering.
	COLORREF        m_clrVisited;   // An RGB value representing the color for the hyperlink text color when the URL has been visited.
	COLORREF        m_clrBack;      // An RGB value representing the background fill color for the hyperlink control.
	CToolTipCtrl    m_toolTip;      // Tooltip to be displayed for the control.

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTHyperLink::SetURL(LPCTSTR lpszLink) {
	m_strLink = lpszLink; if (!m_bInitControl) m_toolTip.UpdateTipText(lpszLink, this);
}
AFX_INLINE CString CXTHyperLink::GetURL() const {
	return m_strLink;
}
AFX_INLINE void CXTHyperLink::SetColors(COLORREF clrLink, COLORREF clrVisited, COLORREF clrHover) {
	m_clrLink = clrLink; m_clrVisited = clrVisited; m_clrHover = clrHover; Invalidate();
}
AFX_INLINE COLORREF CXTHyperLink::GetLinkColor() const {
	return m_clrLink;
}
AFX_INLINE COLORREF CXTHyperLink::GetVisitedColor() const {
	return m_clrVisited;
}
AFX_INLINE COLORREF CXTHyperLink::GetHoverColor() const {
	return m_clrHover;
}
AFX_INLINE void CXTHyperLink::SetVisited(bool bVisited) {
	m_bVisited = bVisited; InvalidateRect(NULL);
}
AFX_INLINE bool CXTHyperLink::GetVisited() const {
	return m_bVisited;
}
AFX_INLINE void CXTHyperLink::SetLinkCursor(HCURSOR hCursor) {
	m_hcurHand = hCursor;
}
AFX_INLINE HCURSOR CXTHyperLink::GetLinkCursor() const {
	return m_hcurHand;
}
AFX_INLINE void CXTHyperLink::SetUnderline(bool bUnderline) {
	m_bUnderline = bUnderline; SetFont(m_bUnderline ? &XTAuxData().fontULine : &XTAuxData().font); Invalidate();
}
AFX_INLINE bool CXTHyperLink::GetUnderline() const {
	return m_bUnderline;
}
AFX_INLINE bool CXTHyperLink::IsTextControl() {
	return ((GetStyle() & 0xFF) <= SS_RIGHT) ? true:false;
}
AFX_INLINE void CXTHyperLink::EnableShellExecute(bool bEnable) {
	m_bShellExec = bEnable;
}
AFX_INLINE void CXTHyperLink::SetBackColor(COLORREF clrBack) {
	m_clrBack = clrBack;
}
AFX_INLINE COLORREF CXTHyperLink::GetBackColor() const {
	return ( m_clrBack == (COLORREF)-1 ) ? GetXtremeColor(COLOR_3DFACE) : m_clrBack;
}

#endif // #if !defined(__XTHYPERLINK_H__)
