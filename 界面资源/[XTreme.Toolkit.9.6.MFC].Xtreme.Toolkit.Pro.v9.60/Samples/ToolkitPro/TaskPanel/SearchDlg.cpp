// SearchDlg.cpp : implementation file
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
#include "taskpanel.h"
#include "SearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchDlg dialog


CSearchDlg::CSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CSearchDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchDlg message handlers

BOOL CSearchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CXTPClientRect rc(this);
	rc.right = 260;

	m_wndTaskPanel.Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, rc, this, 0);
	m_wndTaskPanel.SetTheme(xtpTaskPanelThemeNativeWinXPPlain);

	CXTPTaskPanelGroup* pGroup = m_wndTaskPanel.AddGroup(0);
	pGroup->SetCaption(_T("Search by any or all of the criteria below."));

	pGroup->AddTextItem(_T("All or part of the file name:"));

	m_edtFileName.CreateEx(WS_EX_CLIENTEDGE, _T("Edit"), NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 20), &m_wndTaskPanel, 0);
	m_edtFileName.SetFont(XTPPaintManager()->GetRegularFont());
	pGroup->AddControlItem(m_edtFileName);

	pGroup->AddTextItem(_T("A word or phrase in the file:"));

	m_edtContains.CreateEx(WS_EX_CLIENTEDGE, _T("Edit"), NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 20), &m_wndTaskPanel, 0);
	m_edtContains.SetFont(XTPPaintManager()->GetRegularFont());
	pGroup->AddControlItem(m_edtContains);

	pGroup->AddTextItem(_T("Look in:"));

	m_cboLookIn.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|CBS_DROPDOWN, CRect(0, 0, 0, 18), &m_wndTaskPanel, 0);
	m_cboLookIn.SetFont(XTPPaintManager()->GetRegularFont());

	pGroup->AddControlItem(m_cboLookIn);

	pGroup->SetExpandable(FALSE);



	pGroup = m_wndTaskPanel.AddGroup(0);
	pGroup->SetCaption(_T("When was it modified?"));


	m_dlgModified.Create(IDD_DIALOG_SEARCH_MODIFIED, &m_wndTaskPanel);
	CXTPTaskPanelGroupItem* pItem = pGroup->AddControlItem(m_dlgModified);
	pItem->GetMargins().SetRect(0, 0, 0, 0);
	m_dlgModified.SetItem(pItem);
	pGroup->SetExpanded(FALSE);


	pGroup = m_wndTaskPanel.AddGroup(0);
	pGroup->SetCaption(_T("What size is it?"));


	m_dlgSize.Create(IDD_DIALOG_SEARCH_SIZE, &m_wndTaskPanel);
	pItem = pGroup->AddControlItem(m_dlgSize);
	pItem->GetMargins().SetRect(0, 0, 0, 0);
	m_dlgSize.SetItem(pItem);

	pGroup->SetExpanded(FALSE);

	pGroup = m_wndTaskPanel.AddGroup(0);
	pGroup->SetCaption(_T("More advanced options"));
	pGroup->AddTextItem(_T("Add controls to the Advanced panel here..."));

	pGroup->SetExpanded(FALSE);

	pGroup = m_wndTaskPanel.AddGroup(0);
	pGroup->SetExpandable(FALSE);

	pItem = pGroup->AddLinkItem(0);
	pItem->SetCaption(_T("Other search options"));

	pItem = pGroup->AddLinkItem(0);
	pItem->SetCaption(_T("Change preferences"));

	m_dlgStart.Create(IDD_DIALOG_SEARCH_START, &m_wndTaskPanel);
	pItem = pGroup->AddControlItem(m_dlgStart);
	pItem->GetMargins().SetRect(0, 0, 0, 0);
	m_dlgStart.SetItem(pItem);



	m_wndTaskPanel.Reposition(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
