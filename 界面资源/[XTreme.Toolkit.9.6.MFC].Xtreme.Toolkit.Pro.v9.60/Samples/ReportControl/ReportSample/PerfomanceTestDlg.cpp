// PerfomanceTestDlg.cpp : implementation file
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
#include "reportsample.h"
#include "PerfomanceTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPerfomanceTestDlg dialog


CPerfomanceTestDlg::CPerfomanceTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPerfomanceTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPerfomanceTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPerfomanceTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPerfomanceTestDlg)
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPerfomanceTestDlg, CDialog)
	//{{AFX_MSG_MAP(CPerfomanceTestDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPerfomanceTestDlg message handlers

BOOL CPerfomanceTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_wndReport.ModifyStyle(0, WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_TABSTOP);
	m_wndReport.GetReportHeader()->AllowColumnRemove(FALSE);

	//
	//  Add sample columns
	//
	m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Column 1"), 50));
	m_wndReport.AddColumn(new CXTPReportColumn(1, _T("Column 2"), 50));
	m_wndReport.AddColumn(new CXTPReportColumn(2, _T("Column 3"), 50));


	CString str;
	for (int i = 0; i < 1000; i++)
	{
		CXTPReportRecord* pRecord = m_wndReport.AddRecord(new CXTPReportRecord());

		pRecord->AddItem(new CXTPReportRecordItemNumber(i, _T("Item %2.0f")));

		str.Format(_T("Item %i"), i);
		pRecord->AddItem(new CXTPReportRecordItemText(str));

		str.Format(_T("Item %.4i"), i);
		pRecord->AddItem(new CXTPReportRecordItemText(str));

	}


	m_wndReport.GetPaintManager()->m_columnStyle = xtpColumnFlat;
	m_wndReport.Populate();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
