// XTPReportView.h: interface for the CXTPReportView class.
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
#if !defined(__XTPREPORTVIEW_H__)
#define __XTPREPORTVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     Identifier of report control child window of CXTPReportView.
// Example:
// <code>
// int CXTPReportView::OnCreate(LPCREATESTRUCT lpCreateStruct)
// {
//     if (CView::OnCreate(lpCreateStruct) == -1)
//         return -1;
//
//     if (!m_wndReport.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|WM_VSCROLL,
//         CRect(0, 0, 0, 0), this, XTP_ID_REPORT_CONTROL))
//     {
//         TRACE(_T("Failed to create tab control window\n"));
//         return -1;
//     }
//     return 0;
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_COLUMNORDERCHANGED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_NM_REPORT_SELCHANGED
//     XTP_NM_REPORT_SORTORDERCHANGED, XTP_NM_REPORT_VALUECHANGED
//     CXTPReportView, CXTPReportControl, CXTPReportHeader
//===========================================================================
const UINT XTP_ID_REPORT_CONTROL = 100;

#include "XTPReportControl.h"

//===========================================================================
// Summary:
//     The CXTPReportView class provides an implementation of
//     the Report control as view class. Use GetReportCtrl method to access methods
//     of CXTPReportControl
// See Also: CXTPReportControl
//===========================================================================
class _XTP_EXT_CLASS CXTPReportView : public CView
{
	DECLARE_DYNCREATE(CXTPReportView)
protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Protected constructor used by dynamic creation
	//-------------------------------------------------------------------------
	CXTPReportView();


public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to access Report control object.
	// Returns:
	//     Reference to child CXTPReportControl window
	//-----------------------------------------------------------------------
	CXTPReportControl& GetReportCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieved paint manager of the child report control.
	// Returns:
	//     Pointer to paint manager of report control.
	//-----------------------------------------------------------------------
	CXTPReportPaintManager* GetPaintManager();


protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPReportView object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPReportView();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to print single page of report control.
	// Parameters:
	//     pDC         - Pointer to a device context for page output.
	//     pInfo       - Points to a CPrintInfo structure that describes the current print job.
	//     rcPage      - Page bounding rectangle
	//     nIndexStart - First row to print
	// Remarks:
	//    This method call PrintHeader, PrintRows and PrintFooter methods.
	// Returns:
	//     Index of last printed row
	//-----------------------------------------------------------------------
	virtual long PrintPage (CDC* pDC, CPrintInfo* pInfo, CRect rcPage, long nIndexStart);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to print header of each page.
	// Parameters:
	//     pDC      - Pointer to a device context for page output.
	//     rcHeader - Header bounding rectangle
	//-----------------------------------------------------------------------
	virtual void PrintHeader(CDC* pDC, CRect rcHeader);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw all rows inside bounding rectangle.
	// Parameters:
	//     pDC         - Pointer to a device context for page output.
	//     rcRows      - Bounding rectangle of rows
	//     nIndexStart - First row to print
	// Returns:
	//     Index of last printed row
	//-----------------------------------------------------------------------
	virtual int PrintRows(CDC* pDC, CRect rcRows, long nIndexStart);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called by PrintRows to print single row.
	// Parameters:
	//     pDC   - Pointer to a device context for page output.
	//     pRow  - Row to print.
	//     rcRow - Bounding rectangle of row
	//-----------------------------------------------------------------------
	virtual void PrintRow(CDC* pDC, CXTPReportRow* pRow, CRect rcRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to print footer information for each page.
	// Parameters:
	//     pDC      - Pointer to a device context for page output.
	//     pInfo    - Points to a CPrintInfo structure that describes the current print job.
	//     rcFooter - Footer bounding rectangle
	//-----------------------------------------------------------------------
	virtual void PrintFooter(CDC* pDC, CPrintInfo* pInfo, CRect rcFooter);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method calculates width of column need to print.
	// Parameters:
	//     pColumn     - Column need to print
	//     nTotalWidth - Total width of all columns.
	// Returns:
	//     Width of column.
	//-----------------------------------------------------------------------
	int GetColumnWidth(CXTPReportColumn* pColumn, int nTotalWidth);

protected:
//{{AFX_CODEJOCK_PRIVATE

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPReportView)
	virtual BOOL PaginateTo(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw(CDC* pDC);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPReportView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnPaint();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPReportControl m_wndReport;  // Child report control window.
	CUIntArray m_aPageStart;        // Printed indexes.
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPReportControl& CXTPReportView::GetReportCtrl() {
	return m_wndReport;

}

#endif // !defined(__XTPREPORTVIEW_H__)
