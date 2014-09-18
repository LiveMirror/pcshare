// PageDragAndDrop.cpp : implementation file
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
#include "toolbox.h"
#include "PageDragAndDrop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageDragAndDrop property page

IMPLEMENT_DYNCREATE(CPageDragAndDrop, CDialog)

CPageDragAndDrop::CPageDragAndDrop() : CDialog(CPageDragAndDrop::IDD)
{
	//{{AFX_DATA_INIT(CPageDragAndDrop)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPageDragAndDrop::~CPageDragAndDrop()
{
}

void CPageDragAndDrop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageDragAndDrop)
	DDX_Control(pDX, IDC_STATIC_CLIENT, m_wndClient);
	//}}AFX_DATA_MAP
}


void CPageDragAndDrop::OnCancel()
{
}

void CPageDragAndDrop::OnOK()
{

}

BEGIN_MESSAGE_MAP(CPageDragAndDrop, CDialog)
	//{{AFX_MSG_MAP(CPageDragAndDrop)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageDragAndDrop message handlers

void CPageDragAndDrop::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_wndClient)
	{
		m_wndClient.MoveWindow(10, 10, cx - 20, cy - 20);
		m_wndClient.Invalidate(FALSE);
	}
}
