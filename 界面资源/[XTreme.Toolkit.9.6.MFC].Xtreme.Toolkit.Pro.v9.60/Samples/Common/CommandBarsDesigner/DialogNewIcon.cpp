// DialogNewIcon.cpp : implementation file
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
#include "DialogNewIcon.h"
#include "EmbeddedFrame.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogNewIcon dialog


CDialogNewIcon::CDialogNewIcon(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogNewIcon::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogNewIcon)
	m_strId = _T("");
	m_strHeight = _T("16");
	m_strWidth = _T("16");
	//}}AFX_DATA_INIT
}


void CDialogNewIcon::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogNewIcon)
	DDX_Control(pDX, IDC_COMBO_WIDTH, m_comboWidth);
	DDX_Control(pDX, IDC_COMBO_HEIGHT, m_comboHeight);
	DDX_Control(pDX, IDC_COMBO_ID, m_comboId);
	DDX_CBString(pDX, IDC_COMBO_ID, m_strId);
	DDX_CBString(pDX, IDC_COMBO_HEIGHT, m_strHeight);
	DDX_CBString(pDX, IDC_COMBO_WIDTH, m_strWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogNewIcon, CDialog)
	//{{AFX_MSG_MAP(CDialogNewIcon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogNewIcon message handlers

BOOL CDialogNewIcon::OnInitDialog()
{
	CDialog::OnInitDialog();

	CResourceManager* pResourceManager = GetMainFrame()->GetActiveEmbeddedFrame()->ResourceManager();
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

void CDialogNewIcon::OnOK()
{
	UpdateData();

	int nWidth = _ttoi(m_strWidth);
	if  (nWidth <= 0 || nWidth > 60)
	{
		m_comboWidth.SetFocus();
		return;
	}
	int nHeight = _ttoi(m_strHeight);
	if  (nHeight <= 0 || nHeight > 60)
	{
		m_comboHeight.SetFocus();
		return;
	}

	CDialog::OnOK();
}
