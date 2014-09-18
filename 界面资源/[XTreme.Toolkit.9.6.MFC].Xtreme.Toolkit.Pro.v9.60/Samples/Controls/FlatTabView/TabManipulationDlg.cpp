// TabManipulationDlg.cpp : implementation file
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
#include "TabManipulationDlg.h"
#include "EditCtrlForFlatTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

template <class T>
class CAutoDestructCtrl : public T
{
public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = T::WindowProc(message, wParam, lParam);

		if (message == WM_DESTROY)
		{
			delete this;
		}

		return lRes;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CTabManipulationDlg dialog


CTabManipulationDlg::CTabManipulationDlg(CXTFlatTabCtrl *pFlatTabCtrl,
	CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CTabManipulationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabManipulationDlg)
	m_iTabNum = 0;
	m_sText = _T("");
	//}}AFX_DATA_INIT

	m_pFlatTabCtrl = pFlatTabCtrl;
}


void CTabManipulationDlg::DoDataExchange(CDataExchange* pDX)
{
	const int nTabs = m_pFlatTabCtrl->GetItemCount() - 1;

	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabManipulationDlg)
	DDX_Text(pDX, IDC_TAB_NUM, m_iTabNum);
	DDV_MinMaxUInt(pDX, m_iTabNum, 0, nTabs);
	DDX_Text(pDX, IDC_TEXT, m_sText);
	//}}AFX_DATA_MAP
}

CTabManipulationDlg::~CTabManipulationDlg()
{
}


BEGIN_MESSAGE_MAP(CTabManipulationDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CTabManipulationDlg)
	ON_BN_CLICKED(IDC_ADD_TAB, OnAddTab)
	ON_BN_CLICKED(IDC_DELETE_ALL, OnDeleteAll)
	ON_BN_CLICKED(IDC_DELETE_TAB, OnDeleteTab)
	ON_BN_CLICKED(IDC_MODIFY_TAB, OnModifyTab)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabManipulationDlg message handlers

BOOL CTabManipulationDlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	UpdateDlgStatus();

	// Set control resizing.
	SetResize(IDC_TEXT,       SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDOK,           SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_DELETE_TAB, SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_MODIFY_TAB, SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_ADD_TAB,    SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_DELETE_ALL, SZ_TOP_LEFT,     SZ_TOP_RIGHT);

	// prevent vertical resizing.
	SetFlag(xtResizeNoVertical);

	// Load window placement
	LoadPlacement(_T("CTabManipulationDlg"));

	return TRUE;
}

void CTabManipulationDlg::OnDestroy()
{
	// Save window placement
	SavePlacement(_T("CTabManipulationDlg"));

	CXTResizeDialog::OnDestroy();
}

void CTabManipulationDlg::OnAddTab()
{
	if (UpdateData())
	{
		CAutoDestructCtrl<CEditCtrlForFlatTab> *pSheet = new
			CAutoDestructCtrl<CEditCtrlForFlatTab>();
		CRect rectDummy(0, 0, 0, 0);
		pSheet->Create(WS_CHILD | WS_TABSTOP | WS_CLIPCHILDREN |
			WS_HSCROLL | WS_VSCROLL | ES_MULTILINE, rectDummy,
			m_pFlatTabCtrl, 1000);
		pSheet->SetWindowText(m_sText);
		m_pFlatTabCtrl->InsertItem(m_iTabNum, m_sText, pSheet);
	}
	UpdateDlgStatus();
}

void CTabManipulationDlg::OnDeleteAll()
{
	m_pFlatTabCtrl->DeleteAllItems();
	UpdateDlgStatus();
}

void CTabManipulationDlg::OnDeleteTab()
{
	if (m_pFlatTabCtrl->GetItemCount() > 0  &&  UpdateData())
	{
		m_pFlatTabCtrl->DeleteItem(m_iTabNum);
	}
	UpdateDlgStatus();
}

void CTabManipulationDlg::OnModifyTab()
{
	if (m_pFlatTabCtrl->GetItemCount() > 0  &&  UpdateData())
	{
		m_pFlatTabCtrl->SetItemText(m_iTabNum, m_sText);
	}
	UpdateDlgStatus();
}

void CTabManipulationDlg::UpdateDlgStatus()
{
	const bool bHasTabs = (m_pFlatTabCtrl->GetItemCount() > 0);
	GetDlgItem(IDC_DELETE_ALL)->EnableWindow(bHasTabs);
	GetDlgItem(IDC_DELETE_TAB)->EnableWindow(bHasTabs);
	GetDlgItem(IDC_MODIFY_TAB)->EnableWindow(bHasTabs);
}
