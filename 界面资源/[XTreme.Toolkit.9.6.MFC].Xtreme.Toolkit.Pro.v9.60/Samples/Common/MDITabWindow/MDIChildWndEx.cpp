// MDIChildWndEx.cpp : implementation file
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
#include "mditabwindow.h"
#include "MDIChildWndEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDIChildWndEx

IMPLEMENT_DYNCREATE(CMDIChildWndEx, CMDIChildWnd)

static int nColor = xtpTabColorBlue;

CMDIChildWndEx::CMDIChildWndEx()
{
	m_nColor = nColor++;
	if (nColor > xtpTabColorMagenta)
		nColor = xtpTabColorBlue;

}

CMDIChildWndEx::~CMDIChildWndEx()
{
}


BEGIN_MESSAGE_MAP(CMDIChildWndEx, CMDIChildWnd)
	//{{AFX_MSG_MAP(CMDIChildWndEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XTP_GETTABCOLOR, OnGetTabColor)
	ON_COMMAND_RANGE(ID_COLOR_BLUE, ID_COLOR_MAGENTA, OnSelectColor)
	ON_UPDATE_COMMAND_UI_RANGE(ID_COLOR_BLUE, ID_COLOR_MAGENTA, OnUpdateSelectColor)

	ON_COMMAND(ID_COLOR_NONE, OnSelectColorNone)
	ON_UPDATE_COMMAND_UI(ID_COLOR_NONE, OnUpdateSelectColorNone)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDIChildWndEx message handlers

LRESULT CMDIChildWndEx::OnGetTabColor(WPARAM, LPARAM)
{
	// Can return any one of the XTPTabOneNoteColor RGB color values or any
	// valid COLORREF value for example:
	//
	// return CXTPTabPaintManager::GetOneNoteColor(xtpTabColorOrange);

	return m_nColor;
}

void CMDIChildWndEx::OnSelectColor(UINT nColor)
{
	m_nColor = nColor - ID_COLOR_BLUE + xtpTabColorBlue;
}

void CMDIChildWndEx::OnUpdateSelectColor(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck((int)pCmdUI->m_nID - ID_COLOR_BLUE == m_nColor - xtpTabColorBlue);

}

void CMDIChildWndEx::OnSelectColorNone()
{
	m_nColor = RGB(255, 255, 255);
}

void CMDIChildWndEx::OnUpdateSelectColorNone(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nColor == RGB(255, 255, 255));

}
