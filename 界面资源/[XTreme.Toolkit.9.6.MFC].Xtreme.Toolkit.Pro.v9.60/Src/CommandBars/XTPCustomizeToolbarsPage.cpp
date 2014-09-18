// XTPCustomizeToolbarsPage.cpp : implementation of the CXTPCustomizeToolbarsPage class.
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
#include "XTPResourceManager.h"

#include "XTPCommandBarsResource.h"
#include "XTPCustomizeToolbarsPage.h"
#include "XTPCustomizeSheet.h"
#include "XTPToolBar.h"
#include "XTPCommandBars.h"
#include "XTPCustomizeTools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPCustomizeToolbarsPage property page

IMPLEMENT_DYNCREATE(CXTPCustomizeToolbarsPage, CPropertyPage)

CXTPCustomizeToolbarsPage::CXTPCustomizeToolbarsPage(CXTPCustomizeSheet* pSheet)
	: CPropertyPage(CXTPCustomizeToolbarsPage::IDD), m_pSheet(pSheet)
{
	//{{AFX_DATA_INIT(CXTPCustomizeToolbarsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CXTPCustomizeToolbarsPage::~CXTPCustomizeToolbarsPage()
{
}

void CXTPCustomizeToolbarsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPCustomizeToolbarsPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, XTP_IDC_LIST_TOOLBARS, m_checkList);
	DDX_Control(pDX, XTP_IDC_BUTTON_RENAME, m_btnRename);
	DDX_Control(pDX, XTP_IDC_BUTTON_NEW, m_btnNew);
	DDX_Control(pDX, XTP_IDC_BUTTON_DELETE, m_btnDelete);
	DDX_Control(pDX, XTP_IDC_BUTTON_RESET, m_btnReset);
	//}}AFX_DATA_MAP
}


BOOL CXTPCustomizeToolbarsPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

#if (_MSC_VER >= 1300)
	if (m_pSheet->GetCommandBars()->GetSite()->GetExStyle() & WS_EX_LAYOUTRTL)
	{
		m_checkList.ModifyStyleEx(0, WS_EX_LAYOUTRTL);
	}
#endif

	RefreshToolbarsList();

	return FALSE;   // return TRUE unless you set the focus to a control
	                // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTPCustomizeToolbarsPage::RefreshToolbarsList()
{
	int nIndexSel = 0;
	int nIndex = m_checkList.GetCurSel();
	if (nIndex != LB_ERR)
	{
		nIndexSel = (int)m_checkList.GetItemData(nIndex);
	}
	int nTopIndex = m_checkList.GetTopIndex();

	m_checkList.ResetContent();

	CXTPCommandBars* pCommandBars = m_pSheet->GetCommandBars();
	int nCount = pCommandBars->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPToolBar* pToolBar = pCommandBars->GetAt(i);
		if (pToolBar->GetType() == xtpBarTypeMenuBar && !pToolBar->IsVisible())
			continue;

		int nIndex = m_checkList.AddString(pToolBar->GetTitle());
		m_checkList.SetCheck(nIndex, pToolBar->IsVisible());
		m_checkList.SetItemData(nIndex, i);

		if (i == nIndexSel) m_checkList.SetCurSel(nIndex);
	}

	if (m_checkList.GetTopIndex() != nTopIndex)
		m_checkList.SetTopIndex(nTopIndex);

	OnSelectionChanged();
}


BEGIN_MESSAGE_MAP(CXTPCustomizeToolbarsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CXTPCustomizeToolbarsPage)
		// NOTE: the ClassWizard will add message map macros here
	ON_LBN_SELCHANGE(XTP_IDC_LIST_TOOLBARS, OnSelectionChanged)
	ON_CLBN_CHKCHANGE(XTP_IDC_LIST_TOOLBARS, OnChkChange)
	ON_BN_CLICKED(XTP_IDC_BUTTON_NEW, OnNewToolbar)
	ON_BN_CLICKED(XTP_IDC_BUTTON_RENAME, OnRenameToolbar)
	ON_BN_CLICKED(XTP_IDC_BUTTON_DELETE, OnDeleteToolbar)
	ON_BN_CLICKED(XTP_IDC_BUTTON_RESET, OnResetToolbar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPCustomizeToolbarsPage message handlers


void CXTPCustomizeToolbarsPage::OnSelectionChanged()
{
	int nIndex = m_checkList.GetCurSel();

	if (nIndex != LB_ERR)
	{
		CXTPCommandBars* pCommandBars = m_pSheet->GetCommandBars();
		int i = (int)m_checkList.GetItemData(nIndex);

		if (i >= 0 && i < pCommandBars->GetCount())
		{
			CXTPToolBar* pToolBar = pCommandBars->GetAt(i);

			BOOL bBuiltIn = pToolBar->IsBuiltIn();
			m_btnReset.EnableWindow(bBuiltIn);
			m_btnRename.EnableWindow(!bBuiltIn);
			m_btnDelete.EnableWindow(!bBuiltIn);

		}
	} else
	{
		m_btnReset.EnableWindow(FALSE);
		m_btnRename.EnableWindow(FALSE);
		m_btnDelete.EnableWindow(FALSE);
	}
}

void CXTPCustomizeToolbarsPage::OnChkChange()
{
	int nIndex = m_checkList.GetCurSel();
	if (nIndex == LB_ERR)
		return;

	CXTPCommandBars* pCommandBars = m_pSheet->GetCommandBars();
	int i = (int)m_checkList.GetItemData(nIndex);
	if (i >= 0 && i < pCommandBars->GetCount())
	{
		CXTPToolBar* pToolBar = pCommandBars->GetAt(i);
		if (pToolBar->GetType() == xtpBarTypeMenuBar || !pToolBar->IsCloseable())
		{
			m_checkList.SetCheck(nIndex, 1);

		} else
		{
			BOOL bVisible = m_checkList.GetCheck(nIndex);
			pToolBar->SetVisible(bVisible);
		}
	}
	RefreshToolbarsList();
}

void CXTPCustomizeToolbarsPage::OnNewToolbar()
{
	CXTPCommandBars* pCommandBars = m_pSheet->GetCommandBars();

	CXTPNewToolbarDlg dlg(m_pSheet, pCommandBars);
	if (dlg.DoModal() == IDOK)
	{
		/*CXTPToolBar* pBar = pCommandBars->Add(dlg.m_strToolbar, xtpBarTop);
		pBar->SetBarID(dlg.m_nNewID);
		pBar->m_bBuiltIn = FALSE;
		pCommandBars->GetSite()->RecalcLayout();*/
		pCommandBars->AddCustomBar(dlg.m_strToolbar, dlg.m_nNewID);


		RefreshToolbarsList();
	}
}

void CXTPCustomizeToolbarsPage::OnRenameToolbar()
{
	int nIndex = m_checkList.GetCurSel();
	if (nIndex == LB_ERR)
		return;

	CXTPCommandBars* pCommandBars = m_pSheet->GetCommandBars();

	int i = (int)m_checkList.GetItemData(nIndex);
	if (i < 0 || i >= pCommandBars->GetCount())
		return;

	CXTPToolBar* pToolBar = pCommandBars->GetAt(i);

	if (pToolBar->IsBuiltIn())
		return;


	CXTPNewToolbarDlg dlg(m_pSheet, pCommandBars, pToolBar);
	if (dlg.DoModal() == IDOK)
	{
		pToolBar->SetTitle(dlg.m_strToolbar);

		m_checkList.DeleteString(nIndex);
		nIndex = m_checkList.InsertString(nIndex, pToolBar->GetTitle());
		m_checkList.SetCheck(nIndex, pToolBar->IsVisible());
		m_checkList.SetItemData(nIndex, i);
		m_checkList.SetCurSel(nIndex);

	}
}


void CXTPCustomizeToolbarsPage::OnDeleteToolbar()
{
	int nIndex = m_checkList.GetCurSel();
	if (nIndex == LB_ERR)
		return;

	CXTPCommandBars* pCommandBars = m_pSheet->GetCommandBars();

	int i = (int)m_checkList.GetItemData(nIndex);
	if (i < 0 || i >= pCommandBars->GetCount())
		return;

	CXTPToolBar* pToolBar = pCommandBars->GetAt(i);

	if (pToolBar->IsBuiltIn())
		return;

	CString strMessage;

	CString strDelete;
	VERIFY(XTPResourceManager()->LoadString(&strDelete, XTP_IDS_CONFIRM_DELETE));
	strMessage.Format(strDelete, pToolBar->GetTitle());

	if (AfxMessageBox(strMessage, MB_ICONWARNING|MB_OKCANCEL) == IDOK)
	{
		pCommandBars->Remove(pToolBar);
		RefreshToolbarsList();
	}
}

void CXTPCustomizeToolbarsPage::OnResetToolbar()
{
	int nIndex = m_checkList.GetCurSel();
	if (nIndex == LB_ERR)
		return;

	CXTPCommandBars* pCommandBars = m_pSheet->GetCommandBars();

	int i = (int)m_checkList.GetItemData(nIndex);
	if (i < 0 || i >= pCommandBars->GetCount())
		return;

	CXTPToolBar* pToolBar = pCommandBars->GetAt(i);

	if (!pToolBar->IsBuiltIn())
		return;

	pToolBar->Reset();
	RefreshToolbarsList();

}
