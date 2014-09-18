// ViewType1.cpp : implementation of the CViewType1 class
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
#include "MDIMenus.h"

#include "MDIMenusDoc.h"
#include "ViewType1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewType1

IMPLEMENT_DYNCREATE(CViewType1, CView)

BEGIN_MESSAGE_MAP(CViewType1, CView)
	//{{AFX_MSG_MAP(CViewType1)
	ON_COMMAND(ID_COLOR_RED, OnColorRed)
	ON_COMMAND(ID_COLOR_GREEN, OnColorGreen)
	ON_COMMAND(ID_COLOR_BLUE, OnColorBlue)
	ON_UPDATE_COMMAND_UI(ID_COLOR_RED, OnUpdateColorRed)
	ON_UPDATE_COMMAND_UI(ID_COLOR_GREEN, OnUpdateColorGreen)
	ON_UPDATE_COMMAND_UI(ID_COLOR_BLUE, OnUpdateColorBlue)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewType1 construction/destruction

CViewType1::CViewType1()
{
	m_clr = 0;

}

CViewType1::~CViewType1()
{
}

BOOL CViewType1::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CViewType1 drawing

void CViewType1::OnDraw(CDC* pDC)
{
	CMDIMenusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	pDC->SetTextColor(m_clr);
	pDC->DrawText(_T("Type1"), CXTPClientRect(this), DT_CENTER|DT_VCENTER);
}

/////////////////////////////////////////////////////////////////////////////
// CViewType1 printing

BOOL CViewType1::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CViewType1::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CViewType1::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CViewType1 diagnostics

#ifdef _DEBUG
void CViewType1::AssertValid() const
{
	CView::AssertValid();
}

void CViewType1::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMDIMenusDoc* CViewType1::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMDIMenusDoc)));
	return (CMDIMenusDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewType1 message handlers

void CViewType1::OnColorRed()
{
	m_clr = RGB(0xFF, 0, 0);
	Invalidate(FALSE);

}

void CViewType1::OnColorGreen()
{
	m_clr = RGB(0, 0xFF, 0);
	Invalidate(FALSE);

}

void CViewType1::OnColorBlue()
{
	m_clr = RGB(0, 0, 0xFF);
	Invalidate(FALSE);

}

void CViewType1::OnUpdateColorRed(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_clr == RGB(0xFF, 0, 0));

}

void CViewType1::OnUpdateColorGreen(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_clr == RGB(0, 0xFF, 0));

}

void CViewType1::OnUpdateColorBlue(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_clr == RGB(0, 0, 0xFF));

}
