// ViewTwo.cpp : implementation file
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
#include "ViewTwo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTwo

IMPLEMENT_DYNCREATE(CViewTwo, CViewDraw)

CViewTwo::CViewTwo()
{
	m_strTitle = _T("Two...");
}

CViewTwo::~CViewTwo()
{
}

BEGIN_MESSAGE_MAP(CViewTwo, CViewDraw)
	//{{AFX_MSG_MAP(CViewTwo)
	ON_COMMAND(ID_VIEW_TWO, OnViewTwo)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TWO, OnUpdateViewTwo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTwo drawing

void CViewTwo::OnDraw(CDC* pDC)
{
	CViewDraw::OnDraw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CViewTwo diagnostics

#ifdef _DEBUG
void CViewTwo::AssertValid() const
{
	CViewDraw::AssertValid();
}

void CViewTwo::Dump(CDumpContext& dc) const
{
	CViewDraw::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewTwo message handlers

void CViewTwo::OnViewTwo()
{
	// alert user.
	AfxMessageBox(_T("Properties Pane - No. 2"));

	// set input focus
	SetFocus();
}

void CViewTwo::OnUpdateViewTwo(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here

}
