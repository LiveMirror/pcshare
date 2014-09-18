// ReportSampleDoc.cpp : implementation of the CReportSampleDoc class
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
#include "ReportSample.h"

#include "ReportSampleDoc.h"
#include "ReportSampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportSampleDoc

IMPLEMENT_DYNCREATE(CReportSampleDoc, CDocument)

BEGIN_MESSAGE_MAP(CReportSampleDoc, CDocument)
	//{{AFX_MSG_MAP(CReportSampleDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportSampleDoc construction/destruction

CReportSampleDoc::CReportSampleDoc()
{
	// TODO: add one-time construction code here

}

CReportSampleDoc::~CReportSampleDoc()
{
}

BOOL CReportSampleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CReportSampleDoc serialization

void CReportSampleDoc::Serialize(CArchive& ar)
{
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		CReportSampleView* pReportView = DYNAMIC_DOWNCAST(CReportSampleView, pView);
		pReportView->GetReportCtrl().SerializeState(ar);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CReportSampleDoc diagnostics

#ifdef _DEBUG
void CReportSampleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CReportSampleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CReportSampleDoc commands
