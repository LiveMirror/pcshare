// GrepView.cpp : implementation of the CGrepView class
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
#include "Grep.h"

#include "GrepDoc.h"
#include "GrepView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrepView

IMPLEMENT_DYNCREATE(CGrepView, CXTPReportView)

BEGIN_MESSAGE_MAP(CGrepView, CXTPReportView)
	//{{AFX_MSG_MAP(CGrepView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_NOTIFY(NM_DBLCLK, XTP_ID_REPORT_CONTROL, OnReportItemDblClick)
	ON_NOTIFY(NM_KEYDOWN, XTP_ID_REPORT_CONTROL, OnReportKeyDown)

	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrepView construction/destruction

CGrepView::CGrepView()
{
	// TODO: add construction code here

}

CGrepView::~CGrepView()
{
}

BOOL CGrepView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	return CXTPReportView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGrepView diagnostics

#ifdef _DEBUG
void CGrepView::AssertValid() const
{
	CXTPReportView::AssertValid();
}

void CGrepView::Dump(CDumpContext& dc) const
{
	CXTPReportView::Dump(dc);
}

CGrepDoc* CGrepView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGrepDoc)));
	return (CGrepDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGrepView message handlers

int CGrepView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPReportView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CXTPReportControl& wndReport = GetReportCtrl();

	wndReport.AddColumn(new CXTPReportColumn(0, _T("Name"), 80));
	wndReport.AddColumn(new CXTPReportColumn(5, _T("Ext"), 18));
	wndReport.AddColumn(new CXTPReportColumn(3, _T("Line"), 20));
	CXTPReportColumn* pColumnDirectory = wndReport.AddColumn(new CXTPReportColumn(1, _T("Directory"), 180));
	pColumnDirectory->SetAlignment(DT_LEFT|DT_PATH_ELLIPSIS);

	wndReport.AddColumn(new CXTPReportColumn(2, _T("Match"), 80));
	wndReport.AddColumn(new CXTPReportColumn(4, _T("Apply"), 18, FALSE));

	wndReport.ShowGroupBy();
	wndReport.GetReportHeader()->AllowColumnRemove(FALSE);
	wndReport.EnablePreviewMode(TRUE);


	return 0;
}

void CGrepView::OnReportItemDblClick(NMHDR * pNotifyStruct, LRESULT * /*result*/)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (pItemNotify->pRow)
	{
		TRACE(_T("Double Click on row %d\n"),
			pItemNotify->pRow->GetIndex());

		CGrepRecord* pRecord = (CGrepRecord*)pItemNotify->pRow->GetRecord();
		if (pRecord)
		{
			ShellExecute(0, 0, _T("notepad.exe"), (LPTSTR)(LPCTSTR)pRecord->m_strPath, 0, SW_SHOW);
		}
	}
}

void CGrepView::OnReportKeyDown(NMHDR * pNotifyStruct, LRESULT * /*result*/)
{
	LPNMKEY lpNMKey = (LPNMKEY)pNotifyStruct;

	if (lpNMKey->nVKey == VK_SPACE)
	{
		BOOL bChecked = FALSE;

		if (GetReportCtrl().GetFocusedRow() && GetReportCtrl().GetFocusedRow()->GetRecord())
			bChecked = ! ((CGrepRecord*)(GetReportCtrl().GetFocusedRow()->GetRecord()))->IsChecked();


		CXTPReportSelectedRows* pSelectedRows = GetReportCtrl().GetSelectedRows();

		POSITION pos = pSelectedRows->GetFirstSelectedRowPosition();
		while (pos)
		{
			CXTPReportRow* pRow = pSelectedRows->GetNextSelectedRow(pos);
			
			if (pRow->GetRecord())
			{
				((CGrepRecord*)pRow->GetRecord())->SetChecked(bChecked);
			}
		}

		m_wndReport.RedrawControl();
	}
}