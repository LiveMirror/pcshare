// EclipseHostOutline.cpp : implementation file
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
#include "gui_eclipse.h"
#include "EclipseHostOutline.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEclipseHostOutline

CEclipseHostOutline::CEclipseHostOutline()
{
}

CEclipseHostOutline::~CEclipseHostOutline()
{
}


BEGIN_MESSAGE_MAP(CEclipseHostOutline, CEclipseHost)
	//{{AFX_MSG_MAP(CEclipseHostOutline)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CEclipseHostOutline message handlers

int CEclipseHostOutline::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEclipseHost::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndEdit.Create(WS_CHILD|WS_VISIBLE|ES_AUTOVSCROLL|ES_MULTILINE, CRect(0, 0,200, 120), this, 0))
	{

	}
	m_wndEdit.SetFont(&m_fntNormal);
	m_wndEdit.SetWindowText(_T("An outline is not availaible"));
	m_wndEdit.SetReadOnly();

	return 0;
}
