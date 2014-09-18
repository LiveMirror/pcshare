// FlatTabPropDlg.cpp : implementation file
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
#include "FlatTabView.h"
#include "FlatTabPropDlg.h"
#include "FlatTabViewDoc.h"
#include "FlatTabViewView.h"
#include "TabManipulationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlatTabPropDlg dialog


CFlatTabPropDlg::CFlatTabPropDlg(CXTFlatTabCtrl* pFlatTabCtrl, CFlatTabViewView* pParent /*=NULL*/)
	: CDialog(CFlatTabPropDlg::IDD, pParent),
	m_FTS_XT_BOTTOM(pParent->m_FTS_XT_BOTTOM),
	m_FTS_XT_HASARROWS(pParent->m_FTS_XT_HASARROWS),
	m_FTS_XT_HASHOMEEND(pParent->m_FTS_XT_HASHOMEEND),
	m_FTS_XT_HSCROLL(pParent->m_FTS_XT_HSCROLL),
	m_dwTabStyle(pParent->m_dwTabStyle)
{
	//{{AFX_DATA_INIT(CFlatTabPropDlg)
	//}}AFX_DATA_INIT

	m_pManipDlg = NULL;

	m_pParentWnd = pParent;
	ASSERT_VALID(m_pParentWnd);

	m_pFlatTabCtrl = pFlatTabCtrl;
	ASSERT_VALID(m_pFlatTabCtrl);
}

CFlatTabPropDlg::~CFlatTabPropDlg()
{
	SAFE_DELETE( m_pManipDlg );
}

void CFlatTabPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlatTabPropDlg)
	DDX_Check(pDX, IDC_FTS_XT_BOTTOM, m_FTS_XT_BOTTOM);
	DDX_Check(pDX, IDC_FTS_XT_HASARROWS, m_FTS_XT_HASARROWS);
	DDX_Check(pDX, IDC_FTS_XT_HASHOMEEND, m_FTS_XT_HASHOMEEND);
	DDX_Check(pDX, IDC_FTS_XT_HSCROLL, m_FTS_XT_HSCROLL);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFlatTabPropDlg, CDialog)
	//{{AFX_MSG_MAP(CFlatTabPropDlg)
	ON_BN_CLICKED(IDC_FTS_XT_BOTTOM, UpdateTabsStyle)
	ON_BN_CLICKED(IDC_FTS_XT_HASARROWS, UpdateTabsStyle)
	ON_BN_CLICKED(IDC_FTS_XT_HASHOMEEND, UpdateTabsStyle)
	ON_BN_CLICKED(IDC_FTS_XT_HSCROLL, UpdateTabsStyle)
	ON_BN_CLICKED(IDC_MANIPULATE, OnManipulate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlatTabPropDlg message handlers

void CFlatTabPropDlg::UpdateTabsStyle()
{
	int dwNewStyle = 0;
	UpdateData();
	if (m_FTS_XT_HASHOMEEND)
	{
		dwNewStyle |= FTS_XT_HASHOMEEND;
		m_FTS_XT_HASARROWS = TRUE;
		UpdateData(FALSE);
	}
	GetDlgItem(IDC_FTS_XT_HASARROWS)->EnableWindow(!m_FTS_XT_HASHOMEEND);
	if (m_FTS_XT_BOTTOM)     dwNewStyle |= FTS_XT_BOTTOM;
	if (m_FTS_XT_HASARROWS)  dwNewStyle |= FTS_XT_HASARROWS;
	if (m_FTS_XT_HSCROLL)    dwNewStyle |= FTS_XT_HSCROLL;
	dwNewStyle |= FTS_XT_TOOLTIPS;
	if (dwNewStyle != m_dwTabStyle)
	{
		m_dwTabStyle = dwNewStyle;
		m_pParentWnd->CreateTabs();
	}
}

void CFlatTabPropDlg::OnManipulate()
{
	if (m_pManipDlg == NULL)
	{
		m_pManipDlg = new CTabManipulationDlg(m_pFlatTabCtrl);
		m_pManipDlg->Create(CTabManipulationDlg::IDD, this);
	}
	m_pManipDlg->UpdateDlgStatus();
	m_pManipDlg->ShowWindow(SW_SHOW);
	m_pManipDlg->SetFocus();
}
