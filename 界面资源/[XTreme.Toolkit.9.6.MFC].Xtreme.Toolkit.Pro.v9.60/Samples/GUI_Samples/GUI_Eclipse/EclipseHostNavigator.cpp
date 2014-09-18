// EclipseHostNavigator.cpp : implementation file
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
#include "EclipseHostNavigator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEclipseHostNavigator

CEclipseHostNavigator::CEclipseHostNavigator()
{
	m_ilTreeIcons.Create(16, 16, ILC_COLOR24|ILC_MASK, 0, 1);

	CBitmap bmp;
	bmp.LoadBitmap(IDB_NAVIGATOR_ICONS);

	m_ilTreeIcons.Add(&bmp, RGB(0, 255, 0));
}

CEclipseHostNavigator::~CEclipseHostNavigator()
{
}


BEGIN_MESSAGE_MAP(CEclipseHostNavigator, CEclipseHost)
	//{{AFX_MSG_MAP(CEclipseHostNavigator)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CEclipseHostNavigator message handlers

int CEclipseHostNavigator::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEclipseHost::OnCreate(lpCreateStruct) == -1)
		return -1;

	VERIFY(m_wndTree.Create(WS_VISIBLE|TVS_HASBUTTONS|TVS_LINESATROOT|TVS_HASLINES|WS_CLIPCHILDREN, CXTPEmptyRect(), this, 0));

	m_wndTree.SetImageList(&m_ilTreeIcons, TVSIL_NORMAL);

	m_wndTree.InsertItem (_T("Project"), 0, 0);

	return 0;
}
