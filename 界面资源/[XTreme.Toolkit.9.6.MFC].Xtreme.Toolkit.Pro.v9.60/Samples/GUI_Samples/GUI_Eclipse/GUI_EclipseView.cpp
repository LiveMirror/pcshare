// GUI_EclipseView.cpp : implementation of the CGUI_EclipseView class
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
#include "GUI_Eclipse.h"

#include "GUI_EclipseDoc.h"
#include "GUI_EclipseView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUI_EclipseView

IMPLEMENT_DYNCREATE(CGUI_EclipseView, CEditView)

BEGIN_MESSAGE_MAP(CGUI_EclipseView, CEditView)
	//{{AFX_MSG_MAP(CGUI_EclipseView)
	ON_COMMAND(ID_NAVIGATE_BACK, OnEmptyCommand)
	ON_COMMAND(ID_NAVIGATE_FORWARD, OnEmptyCommand)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_EDIT_SEARCH, CEditView::OnEditFind)
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUI_EclipseView construction/destruction

CGUI_EclipseView::CGUI_EclipseView()
{
	// TODO: add construction code here

}

CGUI_EclipseView::~CGUI_EclipseView()
{
}

BOOL CGUI_EclipseView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);   // Enable word-wrapping

	//cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	//cs.style &= ~WS_BORDER;

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_EclipseView drawing

void CGUI_EclipseView::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_EclipseView printing

BOOL CGUI_EclipseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CGUI_EclipseView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing.
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CGUI_EclipseView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_EclipseView diagnostics

#ifdef _DEBUG
void CGUI_EclipseView::AssertValid() const
{
	CEditView::AssertValid();
}

void CGUI_EclipseView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CGUI_EclipseDoc* CGUI_EclipseView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGUI_EclipseDoc)));
	return (CGUI_EclipseDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGUI_EclipseView message handlers

void CGUI_EclipseView::OnEmptyCommand()
{
	// TODO: Add your command handler code here

}
