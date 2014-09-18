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
#include "CommandBarIcons.h"
#include "PropertiesDialog.h"
//#include ".\propertiesdialog.h"

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
	DDX_Control(pDX, IDC_TAB, m_tabCtrl);
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
	m_pageIconsInfo.UpdateContent();
	m_pageHotIcons.UpdateContent();
	if (m_pageAlphaIcons.GetSafeHwnd()) m_pageAlphaIcons.UpdateContent();
}

BOOL CPropertiesDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pageIconsInfo.Create(CIconsInfoPage::IDD, &m_tabCtrl);
	m_tabCtrl.AddTab(&m_pageIconsInfo, _T("Icons Options"));

	m_pageHotIcons.Create(CHotIconsPage::IDD, &m_tabCtrl);
	m_tabCtrl.AddTab(&m_pageHotIcons, _T("Hot Icons Sample"));

	m_pageAlphaIcons.Create(CAlphaIconsPage::IDD, &m_tabCtrl);
	m_tabCtrl.AddTab(&m_pageAlphaIcons, _T("Alpha Icons Sample"));


	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertiesDialog::OnClose()
{
	ShowWindow(SW_HIDE);
}

//BOOL CPropertiesDialog::PreCreateWindow(CREATESTRUCT& cs)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	return CDialog::PreCreateWindow(cs);
//}
