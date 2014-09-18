// InternetView.cpp : implementation file
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
#include "gui_explorer.h"
#include "InternetView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInternetView

IMPLEMENT_DYNCREATE(CInternetView, CXTHtmlView)

CInternetView::CInternetView()
{
	//{{AFX_DATA_INIT(CInternetView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CInternetView::~CInternetView()
{
}

void CInternetView::DoDataExchange(CDataExchange* pDX)
{
	CXTHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInternetView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInternetView, CXTHtmlView)
	//{{AFX_MSG_MAP(CInternetView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInternetView diagnostics

#ifdef _DEBUG
void CInternetView::AssertValid() const
{
	CXTHtmlView::AssertValid();
}

void CInternetView::Dump(CDumpContext& dc) const
{
	CXTHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInternetView message handlers

void CInternetView::OnInitialUpdate()
{
	//TODO: This code navigates to a popular spot on the web.
	//Change the code to go where you'd like.
	Navigate2(_T("http://www.codejock.com/"),NULL,NULL);
}

BOOL CInternetView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.dwExStyle |= WS_EX_STATICEDGE;
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	return CXTHtmlView::PreCreateWindow(cs);
}
