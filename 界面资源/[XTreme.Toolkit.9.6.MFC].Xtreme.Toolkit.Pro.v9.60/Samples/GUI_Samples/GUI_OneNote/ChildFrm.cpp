// ChildFrm.cpp : implementation of the CChildFrame class
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
#include "GUI_OneNote.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static int nColor = xtpTabColorBlue;

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XTP_GETTABCOLOR, OnGetTabColor)
	ON_COMMAND_RANGE(ID_COLOR_BLUE, ID_COLOR_MAGENTA, OnSelectColor)
	ON_UPDATE_COMMAND_UI_RANGE(ID_COLOR_BLUE, ID_COLOR_MAGENTA, OnUpdateSelectColor)

	ON_COMMAND(ID_COLOR_NONE, OnSelectColorNone)
	ON_UPDATE_COMMAND_UI(ID_COLOR_NONE, OnUpdateSelectColorNone)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	m_nColor = nColor++;
	if (nColor > xtpTabColorMagenta)
		nColor = xtpTabColorBlue;
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers
void CChildFrame::ActivateFrame(int /*nCmdShow*/)
{
	CMDIChildWnd::ActivateFrame(SW_MAXIMIZE);

}

LRESULT CChildFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCPAINT)
	{
		// prevent caption blinking
		return TRUE;
	}

	return CMDIChildWnd::WindowProc(message, wParam, lParam);
}



LRESULT CChildFrame::OnGetTabColor(WPARAM, LPARAM)
{
	// Can return any one of the XTPTabOneNoteColor RGB color values or any
	// valid COLORREF value for example:
	//
	// return CXTPTabPaintManager::GetOneNoteColor(xtpTabColorOrange);

	return m_nColor;
}

void CChildFrame::OnSelectColor(UINT nColor)
{
	m_nColor = nColor - ID_COLOR_BLUE + xtpTabColorBlue;
}

void CChildFrame::OnUpdateSelectColor(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck((int)pCmdUI->m_nID - ID_COLOR_BLUE == m_nColor - xtpTabColorBlue);

}

void CChildFrame::OnSelectColorNone()
{
	m_nColor = RGB(255, 255, 255);
}

void CChildFrame::OnUpdateSelectColorNone(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nColor == RGB(255, 255, 255));

}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	if (pContext != NULL && pContext->m_pNewViewClass != NULL)
	{
		CWnd* pView = CreateView(pContext, AFX_IDW_PANE_FIRST);
		if (pView == NULL)
			return FALSE;

		pView->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	}
	return TRUE;
}
