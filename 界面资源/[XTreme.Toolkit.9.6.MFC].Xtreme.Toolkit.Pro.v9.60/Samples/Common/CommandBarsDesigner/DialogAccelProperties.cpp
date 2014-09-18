// DialogAccelProperties.cpp : implementation file
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
#include "DialogAccelProperties.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogAccelProperties dialog


CDialogAccelProperties::CDialogAccelProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogAccelProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogAccelProperties)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_lpAccel = NULL;
}


void CDialogAccelProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogAccelProperties)
	DDX_Control(pDX, IDC_EDIT_CAPTION, m_wndAccel);
	DDX_Control(pDX, IDC_COMBO_ID, m_comboId);
	DDX_CBString(pDX, IDC_COMBO_ID, m_strId);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogAccelProperties, CDialog)
	//{{AFX_MSG_MAP(CDialogAccelProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogAccelProperties message handlers

BOOL CDialogAccelProperties::OnInitDialog()
{
	CDialog::OnInitDialog();


	CResourceManager* pResourceManager = ((CMainFrame*)AfxGetMainWnd())->GetActiveEmbeddedFrame()->ResourceManager();
	ASSERT(pResourceManager);

	CMapResources* pResources = pResourceManager->GetResources();

	POSITION pos = pResources->GetStartPosition();
	while (pos)
	{
		CResourceInfo* pInfo;
		CString strCaption;
		pResources->GetNextAssoc(pos, strCaption, (CObject*&)pInfo);

		int nIndex = m_comboId.AddString(strCaption);
		if (m_strId == strCaption)
			m_comboId.SetCurSel(nIndex);
	}

	if (m_lpAccel)
	{
		m_wndAccel.SetAccel(*m_lpAccel);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogAccelProperties::OnOK()
{
	UpdateData();

	if (m_strId.IsEmpty())
	{
		m_comboId.SetFocus();
		return;
	}
	if (!m_wndAccel.IsKeyDefined())
	{
		m_wndAccel.SetFocus();
		return;
	}


	CDialog::OnOK();
}
