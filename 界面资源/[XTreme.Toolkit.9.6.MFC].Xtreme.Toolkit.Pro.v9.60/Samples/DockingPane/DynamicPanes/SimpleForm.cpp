// SimpleForm.cpp : implementation file
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
#include "DynamicPanes.h"
#include "SimpleForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimpleForm

IMPLEMENT_DYNCREATE(CSimpleForm, CFormView)

CSimpleForm::CSimpleForm()
	: CFormView(CSimpleForm::IDD)
{
	//{{AFX_DATA_INIT(CSimpleForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSimpleForm::~CSimpleForm()
{
}

void CSimpleForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimpleForm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSimpleForm, CFormView)
	//{{AFX_MSG_MAP(CSimpleForm)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimpleForm diagnostics

#ifdef _DEBUG
void CSimpleForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CSimpleForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSimpleForm message handlers

BOOL CSimpleForm::PreCreateWindow(CREATESTRUCT& cs)
{

	return CFormView::PreCreateWindow(cs);
}

void CSimpleForm::OnPaint()
{
	CPaintDC dc(this);
	CRect rectClient;
	GetClientRect(&rectClient);
	CXTPBufferDC memDC(dc, rectClient);
	memDC.FillSolidRect(rectClient, GetSysColor(COLOR_3DFACE));
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
}

BOOL CSimpleForm::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CSimpleForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	SetScaleToFitSize(CSize(0));

}
