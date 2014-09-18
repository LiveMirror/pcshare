// PropertiesDialog.cpp : implementation file
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
#include "resource.h"
#include "PropertiesDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertiesDialog dialog


CPropertiesDialog::CPropertiesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertiesDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertiesDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertiesDialog)
	DDX_Control(pDX, IDC_TAB, m_wndTabControl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertiesDialog, CDialog)
	//{{AFX_MSG_MAP(CPropertiesDialog)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertiesDialog message handlers

void CPropertiesDialog::UpdateAll()
{
	m_pageTabClient.Update();
	m_pageTabControl.Update();
	m_pageDockingPane.Update();
	m_pagePanel.Update();

}

BOOL CPropertiesDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_wndTabControl.ModifyStyle(0, WS_CLIPCHILDREN|WS_CLIPSIBLINGS);

	m_ilTabCtrl.Create(16, 16, ILC_COLOR24|ILC_MASK, 0, 1);
	CBitmap bmp;
	bmp.LoadBitmap(IDB_TABCTRL_ICONS);
	m_ilTabCtrl.Add(&bmp, RGB(0xC0, 0xC0, 0xC0));

	m_pageTabClient.Create(CTabManagerPage::IDD, &m_wndTabControl);
	m_pageTabControl.Create(CTabManagerPage::IDD, &m_wndTabControl);
	m_pageDockingPane.Create(CTabManagerPage::IDD, &m_wndTabControl);
	m_pagePanel.Create(CTabManagerPage::IDD, &m_wndTabControl);
	m_pageMargins.Create(CTabMarginsPage::IDD, &m_wndTabControl);

	m_wndTabControl.SetImageList(&m_ilTabCtrl);

	m_wndTabControl.InsertItem(0, _T("TabWorkspace"), m_pageTabClient, 0);
	m_wndTabControl.InsertItem(1, _T("TabControl"), m_pageTabControl, 0);
	m_wndTabControl.InsertItem(2, _T("DockingPane"), m_pageDockingPane, 0);
	m_wndTabControl.InsertItem(3, _T("AutoHidePanel"), m_pagePanel, 0);
	m_wndTabControl.InsertItem(4, _T("Margins"), m_pageMargins, 0);


	m_wndTabControl.ShowIcons(FALSE);



	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertiesDialog::OnClose()
{
	ShowWindow(SW_HIDE);
}
