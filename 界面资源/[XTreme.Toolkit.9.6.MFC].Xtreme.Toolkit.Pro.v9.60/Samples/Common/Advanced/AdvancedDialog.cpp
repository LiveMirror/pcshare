// AdvancedDialog.cpp : implementation file
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
#include "advanced.h"
#include "AdvancedDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdvancedDialog dialog


CAdvancedDialog::CAdvancedDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAdvancedDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdvancedDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAdvancedDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdvancedDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdvancedDialog, CDialog)
	//{{AFX_MSG_MAP(CAdvancedDialog)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KICKIDLE,       OnKickIdle)

	ON_COMMAND_RANGE(ID_DEBUG_RESTART, ID_DEBUG_DISASSEMBLY, OnDebug)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvancedDialog message handlers
static UINT indicators[] =
{
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


LRESULT CAdvancedDialog::OnKickIdle(WPARAM, LPARAM)
{
	//UpdateDialogControls(this, TRUE);
	SendMessageToDescendants(WM_IDLEUPDATECMDUI);
	return 0;
}


BOOL CAdvancedDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}


	VERIFY(m_wndToolBar.CreateToolBar(WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS|CBRS_GRIPPER,
		this, AFX_IDW_TOOLBAR));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_DEBUG));

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CAdvancedDialog::OnDebug(UINT)
{
	AfxMessageBox(_T("Clicked!"));

}
