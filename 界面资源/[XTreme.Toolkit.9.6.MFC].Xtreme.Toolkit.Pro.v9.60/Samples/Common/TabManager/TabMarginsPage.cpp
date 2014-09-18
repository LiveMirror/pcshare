// TabMarginsPage.cpp : implementation file
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
#include "tabmanager.h"
#include "TabMarginsPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabMarginsPage dialog


CTabMarginsPage::CTabMarginsPage(CWnd* pParent /*=NULL*/)
	: CDialog(CTabMarginsPage::IDD, pParent)
{
	m_bApply[0] = TRUE;
	m_bApply[1] = FALSE;
	m_bApply[2] = FALSE;
	m_bApply[3] = FALSE;
	//{{AFX_DATA_INIT(CTabMarginsPage)
	//}}AFX_DATA_INIT
}


void AFXAPI DDX_TextEx(CDataExchange* pDX, int nIDC, long& value)
{
	if (pDX->m_bSaveAndValidate)
	{
		CString str;
		pDX->m_pDlgWnd->GetDlgItem(nIDC)->GetWindowText(str);

		if (str.IsEmpty())
		{
			value = 0;
			return;
		}
	}

	DDX_Text(pDX, nIDC, value);
}

void CTabMarginsPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabMarginsPage)
	//}}AFX_DATA_MAP

	DDX_TextEx(pDX, IDC_EDIT_CONTROL_LEFT, m_rcControlMargin.left);
	DDX_TextEx(pDX, IDC_EDIT_CONTROL_RIGHT, m_rcControlMargin.right);
	DDX_TextEx(pDX, IDC_EDIT_CONTROL_TOP, m_rcControlMargin.top);
	DDX_TextEx(pDX, IDC_EDIT_CONTROL_BOTTOM, m_rcControlMargin.bottom);

	DDX_TextEx(pDX, IDC_EDIT_CLIENT_LEFT, m_rcClientMargin.left);
	DDX_TextEx(pDX, IDC_EDIT_CLIENT_RIGHT, m_rcClientMargin.right);
	DDX_TextEx(pDX, IDC_EDIT_CLIENT_TOP, m_rcClientMargin.top);
	DDX_TextEx(pDX, IDC_EDIT_CLIENT_BOTTOM, m_rcClientMargin.bottom);

	DDX_TextEx(pDX, IDC_EDIT_HEADER_LEFT, m_rcHeaderMargin.left);
	DDX_TextEx(pDX, IDC_EDIT_HEADER_RIGHT, m_rcHeaderMargin.right);
	DDX_TextEx(pDX, IDC_EDIT_HEADER_TOP, m_rcHeaderMargin.top);
	DDX_TextEx(pDX, IDC_EDIT_HEADER_BOTTOM, m_rcHeaderMargin.bottom);

	DDX_TextEx(pDX, IDC_EDIT_BUTTON_LEFT, m_rcButtonMargin.left);
	DDX_TextEx(pDX, IDC_EDIT_BUTTON_RIGHT, m_rcButtonMargin.right);
	DDX_TextEx(pDX, IDC_EDIT_BUTTON_TOP, m_rcButtonMargin.top);
	DDX_TextEx(pDX, IDC_EDIT_BUTTON_BOTTOM, m_rcButtonMargin.bottom);


	DDX_Check(pDX, IDC_CHECK_TABWORKSAPCE, m_bApply[0]);
	DDX_Check(pDX, IDC_CHECK_TABCONTROL, m_bApply[1]);
	DDX_Check(pDX, IDC_CHECK_DOCKINGPANE, m_bApply[2]);
	DDX_Check(pDX, IDC_CHECK_PANEL, m_bApply[3]);
}

CXTPTabPaintManager* CTabMarginsPage::GetPaintManager(int nIndex)
{
	UpdateData();

	if (m_bApply[nIndex])
	{
		switch (nIndex)
		{
			case 0: return ((CMainFrame*)GetParentFrame())->m_MTIClientWnd.GetPaintManager();
			case 1: return ((CXTPTabControl*)GetParent())->GetPaintManager();
			case 2: return ((CMainFrame*)GetParentFrame())->m_paneManager.GetPaintManager()->GetTabPaintManager();
			case 3: return ((CMainFrame*)GetParentFrame())->m_paneManager.GetPaintManager()->GetPanelPaintManager();
		}
	}
	return NULL;
}


CXTPTabPaintManager* CTabMarginsPage::GetPaintManager()
{
	for (int i = 0; i < 4; i++)
	{
		if (GetPaintManager(i) != 0)
		{
			return GetPaintManager(i);
		}
	}
	return NULL;
}

void CTabMarginsPage::Update()
{
	CXTPTabPaintManager* pManager = GetPaintManager();

	if (pManager)
	{
		m_rcControlMargin = pManager->m_rcControlMargin;
		m_rcHeaderMargin = pManager->GetAppearanceSet()->m_rcHeaderMargin;
		m_rcClientMargin = pManager->m_rcClientMargin;
		m_rcButtonMargin = pManager->m_rcButtonMargin;
	} else
	{
		m_rcControlMargin.SetRect(0, 0, 0, 0);
		m_rcHeaderMargin.SetRect(0, 0, 0, 0);
		m_rcClientMargin.SetRect(0, 0, 0, 0);
		m_rcButtonMargin.SetRect(0, 0, 0, 0);

	}

	for (int i = IDC_EDIT_CONTROL_LEFT; i <= IDC_EDIT_BUTTON_BOTTOM; i++)
		GetDlgItem(i)->EnableWindow(pManager != NULL);

	UpdateData(FALSE);

}

void CTabMarginsPage::OnCancel()
{

}

void CTabMarginsPage::OnOK()
{

}


BEGIN_MESSAGE_MAP(CTabMarginsPage, CDialog)
	//{{AFX_MSG_MAP(CTabMarginsPage)
	ON_BN_CLICKED(IDC_CHECK_TABWORKSAPCE, OnCheckApply)
	ON_BN_CLICKED(IDC_CHECK_TABCONTROL, OnCheckApply)
	ON_BN_CLICKED(IDC_CHECK_DOCKINGPANE, OnCheckApply)
	ON_BN_CLICKED(IDC_CHECK_PANEL, OnCheckApply)

	ON_EN_CHANGE(IDC_EDIT_CONTROL_LEFT, OnChangeEditControl)
	ON_EN_CHANGE(IDC_EDIT_CONTROL_TOP, OnChangeEditControl)
	ON_EN_CHANGE(IDC_EDIT_CONTROL_RIGHT, OnChangeEditControl)
	ON_EN_CHANGE(IDC_EDIT_CONTROL_BOTTOM, OnChangeEditControl)

	ON_EN_CHANGE(IDC_EDIT_CLIENT_LEFT, OnChangeEditClient)
	ON_EN_CHANGE(IDC_EDIT_CLIENT_TOP, OnChangeEditClient)
	ON_EN_CHANGE(IDC_EDIT_CLIENT_RIGHT, OnChangeEditClient)
	ON_EN_CHANGE(IDC_EDIT_CLIENT_BOTTOM, OnChangeEditClient)

	ON_EN_CHANGE(IDC_EDIT_HEADER_LEFT, OnChangeEditHeader)
	ON_EN_CHANGE(IDC_EDIT_HEADER_TOP, OnChangeEditHeader)
	ON_EN_CHANGE(IDC_EDIT_HEADER_RIGHT, OnChangeEditHeader)
	ON_EN_CHANGE(IDC_EDIT_HEADER_BOTTOM, OnChangeEditHeader)

	ON_EN_CHANGE(IDC_EDIT_BUTTON_LEFT, OnChangeEditButton)
	ON_EN_CHANGE(IDC_EDIT_BUTTON_TOP, OnChangeEditButton)
	ON_EN_CHANGE(IDC_EDIT_BUTTON_RIGHT, OnChangeEditButton)
	ON_EN_CHANGE(IDC_EDIT_BUTTON_BOTTOM, OnChangeEditButton)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CTabMarginsPage message handlers

BOOL CTabMarginsPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	Update();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabMarginsPage::OnCheckApply()
{
	Update();

}

void CTabMarginsPage::OnChangeEditControl()
{
	UpdateData();

	for (int i = 0; i < 4; i++)
	{
		if (GetPaintManager(i) != 0)
		{
			GetPaintManager(i)->m_rcControlMargin = m_rcControlMargin;
		}
	}

	((CMainFrame*)GetParentFrame())->m_paneManager.RedrawPanes();
	((CXTPTabControl*)GetParent())->Reposition();
}

void CTabMarginsPage::OnChangeEditClient()
{
	UpdateData();

	for (int i = 0; i < 4; i++)
	{
		if (GetPaintManager(i) != 0)
		{
			GetPaintManager(i)->m_rcClientMargin = m_rcClientMargin;
		}
	}

	((CMainFrame*)GetParentFrame())->m_paneManager.RedrawPanes();
	((CXTPTabControl*)GetParent())->Reposition();
}

void CTabMarginsPage::OnChangeEditHeader()
{
	UpdateData();

	for (int i = 0; i < 4; i++)
	{
		if (GetPaintManager(i) != 0)
		{
			GetPaintManager(i)->GetAppearanceSet()->m_rcHeaderMargin = m_rcHeaderMargin;
		}
	}

	((CMainFrame*)GetParentFrame())->m_paneManager.RedrawPanes();
	((CXTPTabControl*)GetParent())->Reposition();
}

void CTabMarginsPage::OnChangeEditButton()
{
	UpdateData();

	for (int i = 0; i < 4; i++)
	{
		if (GetPaintManager(i) != 0)
		{
			GetPaintManager(i)->m_rcButtonMargin = m_rcButtonMargin;
		}
	}

	((CMainFrame*)GetParentFrame())->m_paneManager.RedrawPanes();
	((CXTPTabControl*)GetParent())->Reposition();
}
