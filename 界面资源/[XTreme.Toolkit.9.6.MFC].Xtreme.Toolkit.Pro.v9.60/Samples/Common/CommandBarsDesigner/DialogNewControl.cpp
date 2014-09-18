// DialogNewControl.cpp : implementation file
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
#include "commandbarsdesigner.h"
#include "DialogNewControl.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogNewControl dialog


CDialogNewControl::CDialogNewControl(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogNewControl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogNewControl)
	m_strCaption = _T("");
	//}}AFX_DATA_INIT
	m_bGenerateId = TRUE;
}


void CDialogNewControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogNewControl)
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_Control(pDX, IDC_COMBO_ID, m_comboId);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Text(pDX, IDC_EDIT_CAPTION, m_strCaption);
	DDX_CBString(pDX, IDC_COMBO_CATEGORY, m_strCategory);
	DDX_CBString(pDX, IDC_COMBO_ID, m_strId);
	DDX_CBString(pDX, IDC_COMBO_TYPE, m_strType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogNewControl, CDialog)
	//{{AFX_MSG_MAP(CDialogNewControl)
	ON_EN_CHANGE(IDC_EDIT_CAPTION, OnChangeEditCaption)
	ON_CBN_EDITCHANGE(IDC_COMBO_CATEGORY, OnEditchangeComboCategory)
	ON_CBN_SELCHANGE(IDC_COMBO_CATEGORY, OnSelchangeComboCategory)
	ON_CBN_EDITCHANGE(IDC_COMBO_ID, OnEditchangeComboId)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogNewControl message handlers

BOOL CDialogNewControl::OnInitDialog()
{
	CDialog::OnInitDialog();

	CXTPControls* pControls = ((CMainFrame*)AfxGetMainWnd())->GetActiveEmbeddedFrame()->m_pControls;

	ASSERT(pControls);

	for (int i = 0; i < pControls->GetCount(); i++)
	{
		CXTPControl* pControl = pControls->GetAt(i);

		if (m_comboCategory.FindStringExact(0, pControl->GetCategory()) == CB_ERR)
		{
			m_comboCategory.AddString(pControl->GetCategory());
		}
	}

	for (int j = 1; j < _countof(lpTypes); j++)
	{
		if (lpTypes[j])
		{
			m_comboType.AddString(lpTypes[j]);
		}
	}
	m_comboType.SetCurSel(0);



	CResourceManager* pResourceManager = ((CMainFrame*)AfxGetMainWnd())->GetActiveEmbeddedFrame()->ResourceManager();
	ASSERT(pResourceManager);

	CMapResources* pResources = pResourceManager->GetResources();

	POSITION pos = pResources->GetStartPosition();
	while (pos)
	{
		CResourceInfo* pInfo;
		CString strCaption;
		pResources->GetNextAssoc(pos, strCaption, (CObject*&)pInfo);

		m_comboId.AddString(strCaption);

	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogNewControl::GenerateID()
{
	if (m_bGenerateId)
	{
		CString strCaption = StripChars(m_strCaption);

		CString strCategory;
		m_comboCategory.GetWindowText(strCategory);
		strCategory = StripChars(strCategory);

		CString strId = _T("ID");

		if (!strCategory.IsEmpty()) strId += _T("_") + strCategory;
		if (!strCaption.IsEmpty()) strId += _T("_") + strCaption;

		m_comboId.SetWindowText(strId);
	}
}

void CDialogNewControl::OnChangeEditCaption()
{
	UpdateData();
	GenerateID();
}

void CDialogNewControl::OnEditchangeComboCategory()
{
	GenerateID();
}

void CDialogNewControl::OnSelchangeComboCategory()
{
	int nIndex = m_comboCategory.GetCurSel();
	if (nIndex != CB_ERR)
	{
		CString strCategory;
		m_comboCategory.GetLBText(nIndex, strCategory);
		m_comboCategory.SetWindowText(strCategory);
	}
	GenerateID();
}


void CDialogNewControl::OnEditchangeComboId()
{
	m_bGenerateId = FALSE;

}

void CDialogNewControl::OnOK()
{
	UpdateData();


	CDialog::OnOK();
}
