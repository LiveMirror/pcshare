// HelpView.cpp : implementation file
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
#include "maskedit.h"
#include "HelpView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelpView

IMPLEMENT_DYNCREATE(CHelpView, CXTHtmlView)

CHelpView::CHelpView()
{
	//{{AFX_DATA_INIT(CHelpView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CHelpView::~CHelpView()
{
}

void CHelpView::DoDataExchange(CDataExchange* pDX)
{
	CXTHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelpView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHelpView, CXTHtmlView)
	//{{AFX_MSG_MAP(CHelpView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelpView diagnostics

#ifdef _DEBUG
void CHelpView::AssertValid() const
{
	CXTHtmlView::AssertValid();
}

void CHelpView::Dump(CDumpContext& dc) const
{
	CXTHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHelpView message handlers

void CHelpView::OnInitialUpdate()
{
	//TODO: This code navigates to a popular spot on the web.
	//Change the code to go where you'd like.
	LoadFromResource( IDR_README );
}
