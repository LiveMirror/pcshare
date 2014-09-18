// ViewOne.cpp : implementation file
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
#include "ActivePaneView.h"
#include "ViewOne.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewOne

IMPLEMENT_DYNCREATE(CViewOne, CViewDraw)

CViewOne::CViewOne()
{
	m_strTitle = _T("One...");
}

CViewOne::~CViewOne()
{
}

BEGIN_MESSAGE_MAP(CViewOne, CViewDraw)
	//{{AFX_MSG_MAP(CViewOne)
	ON_COMMAND(ID_VIEW_ONE, OnViewOne)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ONE, OnUpdateViewOne)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewOne drawing

void CViewOne::OnDraw(CDC* pDC)
{
	CViewDraw::OnDraw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CViewOne diagnostics

#ifdef _DEBUG
void CViewOne::AssertValid() const
{
	CViewDraw::AssertValid();
}

void CViewOne::Dump(CDumpContext& dc) const
{
	CViewDraw::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewOne message handlers

void CViewOne::OnViewOne()
{
	// alert user.
	AfxMessageBox(_T("Options Pane - No. 1"));

	// set input focus
	SetFocus();
}

void CViewOne::OnUpdateViewOne(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here

}
