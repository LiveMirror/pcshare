// ToolBoxDemoView.cpp : implementation of the CToolBoxDemoView class
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
#include "ToolBoxDemo.h"

#include "ToolBoxDemoDoc.h"
#include "ToolBoxDemoView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBoxDemoView

IMPLEMENT_DYNCREATE(CToolBoxDemoView, CView)

BEGIN_MESSAGE_MAP(CToolBoxDemoView, CView)
	//{{AFX_MSG_MAP(CToolBoxDemoView)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBoxDemoView construction/destruction

CToolBoxDemoView::CToolBoxDemoView()
{
	// TODO: add construction code here

}

CToolBoxDemoView::~CToolBoxDemoView()
{
}

BOOL CToolBoxDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CView::PreCreateWindow(cs))
		return FALSE;

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CToolBoxDemoView drawing

void CToolBoxDemoView::OnDraw(CDC* pDC)
{
	// Get a pointer to the frame window
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT_VALID(pMainFrame);

	// Get the index for selected toolbox folder.
	int iFolder = pMainFrame->m_wndToolBox.GetSelFolder();

	// Get a pointer to the selected toolbox folder.
	CXTOutBarFolder* pFolder = pMainFrame->m_wndToolBox.GetBarFolder(iFolder);
	ASSERT(pFolder != NULL);

	// Get the index for the currently selected item.
	int iCurSel = pFolder->GetSelIndex();

	// Construct our display text.
	CString strSelected;
	strSelected.Format(_T("Selected Index: %d, Text: %s"),
		iCurSel, pMainFrame->m_wndToolBox.GetItemText(iCurSel) );

	// Draw the text.
	CFont* pFontOld = pDC->SelectObject(&XTAuxData().fontBold);
	pDC->TextOut( 5, 5, strSelected );
	pDC->SelectObject(pFontOld);
}

/////////////////////////////////////////////////////////////////////////////
// CToolBoxDemoView printing

BOOL CToolBoxDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CToolBoxDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CToolBoxDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CToolBoxDemoView diagnostics

#ifdef _DEBUG
void CToolBoxDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CToolBoxDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolBoxDemoDoc* CToolBoxDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolBoxDemoDoc)));
	return (CToolBoxDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CToolBoxDemoView message handlers

void CToolBoxDemoView::OnPaint()
{
	CPaintDC dc(this);

	// Get the client rect.
	CRect rectClient;
	GetClientRect(&rectClient);

	// Paint to a memory device context to reduce screen flicker.
	CXTMemDC memDC(&dc, rectClient, GetXtremeColor(COLOR_WINDOW));

	OnPrepareDC(&memDC);
	OnDraw(&memDC);
}

BOOL CToolBoxDemoView::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return FALSE;
}
