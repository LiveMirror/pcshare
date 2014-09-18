// VirtualListView.cpp : implementation of the CVirtualListView class
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

#include "stdafx.h"
#include "VirtualList.h"

#include "VirtualListDoc.h"
#include "VirtualListView.h"

#include "DialogRowsCount.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CVirtualRecord : public CXTPReportRecord
{
public:
	CVirtualRecord()
	{
		AddItem(new CXTPReportRecordItem());
		AddItem(new CXTPReportRecordItem());
		AddItem(new CXTPReportRecordItem());
	}

	void GetItemMetrics (XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics)
	{
		CXTPReportColumnOrder* pSortOrder = pDrawArgs->pControl->GetColumns()->GetSortOrder();

		BOOL bDecreasing = pSortOrder->GetCount() > 0 && pSortOrder->GetAt(0)->IsSortedDecreasing();

		CString strColumn = pDrawArgs->pColumn->GetCaption();
		int nIndex = pDrawArgs->pRow->GetIndex();

		int nCount = pDrawArgs->pControl->GetRows()->GetCount();
		pItemMetrics->strText.Format(_T("%s - Row %i"), strColumn, bDecreasing? nCount - nIndex: nIndex + 1);

		CVirtualListView* pView = DYNAMIC_DOWNCAST(CVirtualListView, pDrawArgs->pControl->GetParent());

		if (pView && pView->m_bColorize)
		{
			if (nIndex % 2)
			{
				pItemMetrics->clrBackground = RGB(245, 245, 245);
			}
		}
	}
};

/////////////////////////////////////////////////////////////////////////////
// CVirtualListView

IMPLEMENT_DYNCREATE(CVirtualListView, CXTPReportView)

BEGIN_MESSAGE_MAP(CVirtualListView, CXTPReportView)
	//{{AFX_MSG_MAP(CVirtualListView)
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_REPORT_SETROWSCOUNT, OnReportSetrowscount)
	ON_COMMAND(ID_REPORT_COLORIZE, OnReportColorize)
	ON_UPDATE_COMMAND_UI(ID_REPORT_COLORIZE, OnUpdateReportColorize)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CXTPReportView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CXTPReportView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CXTPReportView::OnFilePrintPreview)

	ON_NOTIFY(NM_CLICK, XTP_ID_REPORT_CONTROL, OnReportItemClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVirtualListView construction/destruction

CVirtualListView::CVirtualListView()
{
	m_bColorize = FALSE;

}

CVirtualListView::~CVirtualListView()
{
}

BOOL CVirtualListView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CXTPReportView::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// CVirtualListView diagnostics

#ifdef _DEBUG
void CVirtualListView::AssertValid() const
{
	CView::AssertValid();
}

void CVirtualListView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVirtualListDoc* CVirtualListView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVirtualListDoc)));
	return (CVirtualListDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVirtualListView message handlers

int CVirtualListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPReportView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndReport.GetReportHeader()->AllowColumnRemove(FALSE);

	m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Column 1"), 50));
	m_wndReport.AddColumn(new CXTPReportColumn(1, _T("Column 2"), 50));
	m_wndReport.AddColumn(new CXTPReportColumn(2, _T("Column 3"), 50));


	m_wndReport.SetVirtualMode(new CVirtualRecord(), 1000000);

	m_wndReport.Populate();

	return 0;
}

void CVirtualListView::OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * /*result*/)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	TRACE(_T("Click on row %d, col %d\n"),
			pItemNotify->pRow->GetIndex(), pItemNotify->pColumn->GetItemIndex());

}


void CVirtualListView::OnEditCopy()
{
	CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows();
	POSITION pos = pSelectedRows->GetFirstSelectedRowPosition();

	while (pos)
	{
		int nIndex = pSelectedRows->GetNextSelectedRow(pos)->GetIndex();
		TRACE(_T("%i, "), nIndex + 1);
	}

}

void CVirtualListView::OnReportSetrowscount()
{
	CDialogRowsCount dr;
	if (dr.DoModal() == IDOK)
	{
		m_wndReport.SetVirtualMode(new CVirtualRecord(), dr.m_nRowsCount);
		m_wndReport.Populate();
	}

}

void CVirtualListView::OnReportColorize()
{
	m_bColorize = !m_bColorize;
	m_wndReport.RedrawControl();

}

void CVirtualListView::OnUpdateReportColorize(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bColorize);

}
