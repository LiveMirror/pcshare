// XTPReportTip.h: interface for the CXTPReportTip class.
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
#if !defined(__XTPREPORTTIP_H__)
#define __XTPREPORTTIP_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// Summary:
//     The CXTPReportTip class encapsulates the functionality of
//     a "tip control," a small pop-up window that displays a
//     single line of text describing the hidden text when hovering an
//     item on the report area.
// Remarks:
//     CXTPReportTip provides the functionality to control the
//     the tip text, the size of the tip window itself, and the text
//     font of the tip.
//
//     This class is used by the report control internally.
//
// See Also: CXTPReportControl, CXTPReportRow::ShowToolTip
//===========================================================================
class _XTP_EXT_CLASS CXTPReportTip : public CWnd
{
	friend class CXTPReportRow;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportTip object.
	// Remarks:
	//     You construct a object object in two steps.
	//     First, call the constructor object and then call
	//     Create, which initializes the window and
	//     attaches it to the parent window.
	//
	// Example:
	// <code>
	// // Declare a local CXTPReportTip object.
	// CXTPReportTip wndTip;
	//
	// // Declare a dynamic CXTPReportTip object.
	// CXTPReportTip* pTipWnd = new CXTPReportTip;
	// </code>
	//
	// See Also: CWnd, Create, Activate, CXTPReportRow::ShowToolTip
	//-----------------------------------------------------------------------
	CXTPReportTip();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPReportTip object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPReportTip();

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates report tip window.
	// Parameters:
	//     pParentWnd - Parent report control window.
	// Remarks:
	//     You construct an object in two steps.
	//     First, call the constructor object and then call
	//     Create, which initializes the window and
	//     attaches it to the parent window.
	// Returns:
	//     TRUE if the tip window was created successfully, FALSE otherwise.
	// See Also: CXTPReportTip::CXTPReportTip, Activate
	//-----------------------------------------------------------------------
	virtual BOOL Create(CWnd *pParentWnd);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Returns tooltip text string.
	// Returns:
	//     Current tooltip text.
	// See Also: SetTooltipText
	//-----------------------------------------------------------------------
	CString GetTooltipText();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets new tooltip text to show.
	// Parameters:
	//     str - A string with the new tooltip caption.
	// Remarks:
	//     Should be called before activating the tooltip window.
	// See Also: GetTooltipText
	//-----------------------------------------------------------------------
	void SetTooltipText(LPCTSTR str);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets tooltip text displaying font.
	// Parameters:
	//     pFont - Pointer to the new font object.
	//-----------------------------------------------------------------------
	void SetFont(CFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets coordinates of the tooltip hover rectangle.
	// Parameters:
	//     rc - New tooltip hover rectangle in parent window coordinates.
	// Remarks:
	//     Should be called before activating the tooltip window.
	// See Also: GetHoverRect
	//-----------------------------------------------------------------------
	void SetHoverRect(CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the coordinates of the tooltip hover rectangle.
	// Returns:
	//     Coordinates of the tooltip hover rectangle in parent window coordinates.
	// See Also: SetHoverRect
	//-----------------------------------------------------------------------
	CRect GetHoverRect();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to activate or deactivate a tool tip control.
	// Parameters:
	//     bActive - Specifies whether the tool tip control is to be activated or deactivated.
	// Remarks:
	//     If <i>bActivate</i> is <b>TRUE</b>, the control is activated;
	//     if <b>FALSE</b>, it is deactivated.
	//
	//     When a tool tip control is active, the tool tip information
	//     appears when the cursor is on a tool that is registered with
	//     the control; when it is inactive, the tool tip information
	//     does not appear, even when the cursor is on a tool.
	// See Also: Create, CXTPReportTip::CXTPReportTip, SetTooltipText
	//-----------------------------------------------------------------------
	void Activate(BOOL bActive);

protected:
	CRect  m_rcHover;           // Hover window coordinates.
	CWnd * m_pParentWnd;        // Pointer to the parent report window.
	CString m_strTooltipText;   // Tooltip text to display.
	CXTPReportRecordItem* m_pItem;  // Item which tooltip is visible.
	int m_nRowIndex;            // Index of item's row.
	CFont m_fntToolTip;         // Font for displaying tooltip text.

protected:
//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_MSG(CXTPReportTip)
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC*);
	afx_msg void OnPaint();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	DECLARE_MESSAGE_MAP()
};

AFX_INLINE CString CXTPReportTip::GetTooltipText() {
	return m_strTooltipText;
}
AFX_INLINE void CXTPReportTip::SetTooltipText(LPCTSTR str) {
	m_strTooltipText = str;
}
AFX_INLINE void CXTPReportTip::SetFont(CFont* pFont) {
	m_fntToolTip.DeleteObject();
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	m_fntToolTip.CreateFontIndirect(&lf);
}
AFX_INLINE void CXTPReportTip::SetHoverRect(CRect rc) {
	m_rcHover = rc;
	MoveWindow(rc);
}
AFX_INLINE CRect CXTPReportTip::GetHoverRect() {
	return m_rcHover;
}
AFX_INLINE BOOL CXTPReportTip::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

#endif // !defined(__XTPREPORTTIP_H__)
