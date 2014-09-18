// XTPReportDragDrop.h: interface for the CXTPReportHeaderDragWnd and
//                      CXTPReportHeaderDropWnd classes.
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
#if !defined(__XTPREPORTDRAGDROP_H__)
#define __XTPREPORTDRAGDROP_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPReportHeader;
class CXTPReportPaintManager;
class CXTPReportColumn;

//===========================================================================
// Summary:
//     CXTPReportHeaderDragWnd is CWnd derived class, it used internally to
//     represent dragging column window.
//===========================================================================
class _XTP_EXT_CLASS CXTPReportHeaderDragWnd : public CWnd
{

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates CXTPReportHeaderDragWnd object.
	//-----------------------------------------------------------------------
	CXTPReportHeaderDragWnd();

	//-----------------------------------------------------------------------
	// Summary:
	//    Default CXTPReportHeaderDragWnd destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPReportHeaderDragWnd();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates dragging window with the specified parameters.
	// Parameters:
	//     rect          - Drawing rectangle position.
	//     pHeader       - Pointer to parent report header object.
	//     pPaintManager - Paint manager for performing draw operations.
	//     pColumn       - Pointer to the associated column.
	// Returns:
	//     TRUE if created successfully, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL Create(CRect rect, CXTPReportHeader* pHeader, CXTPReportPaintManager* pPaintManager, CXTPReportColumn* pColumn);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the framework to draw the contents of the window using the specified device context.
	// Parameters:
	//     pDC - A pointer to the device context in which the drawing occurs.
	//     rcClient - Coordinates of the client window position.
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC, CRect rcClient);

protected:
//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_MSG(CXTPReportHeaderDragWnd)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	//{{AFX_VIRTUAL(CXTPReportHeaderDragWnd)
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	DECLARE_MESSAGE_MAP()

protected:
	CXTPReportHeader* m_pHeader;             // Pointer to the associated report header object.
	CXTPReportPaintManager* m_pPaintManager; // Pointer to the associated paint manager object.
	CXTPReportColumn* m_pColumn;             // Pointer to the associated report column object.

};

//===========================================================================
// Summary:
//     CXTPReportHeaderDropWnd is CWnd derived class, it used internally to
//     represent drop target arrows.
//===========================================================================
class _XTP_EXT_CLASS CXTPReportHeaderDropWnd : public CWnd
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates CXTPReportHeaderDropWnd object.
	// Parameters:
	//     crColor - Drop window color.
	//-----------------------------------------------------------------------
	CXTPReportHeaderDropWnd(COLORREF crColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys CXTPReportHeaderDropWnd object. Performs cleanup operations.
	//-----------------------------------------------------------------------
	virtual ~CXTPReportHeaderDropWnd();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Changes window position.
	// Parameters:
	//     x - New horizontal position.
	//     y - New vertical position.
	//-----------------------------------------------------------------------
	void SetWindowPos(int x, int y);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a drop window with the specified gap inside.
	// Parameters:
	//     nHeight - Height between 2 arrows in pixels.
	// Returns:
	//     TRUE if created successfully, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL Create(int nHeight);

protected:

//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_VIRTUAL(CXTPReportHeaderDropWnd)
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPReportHeaderDropWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	DECLARE_MESSAGE_MAP()

protected:
	CBrush m_brush; // Drawing brush.
	CRgn m_rgn;     // Arrows region.
	int m_nHeight;  // Height of the gap between 2 arrows.
};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPReportHeaderDragWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTPReportHeaderDropWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

#endif //#if !defined(__XTPREPORTDRAGDROP_H__)
