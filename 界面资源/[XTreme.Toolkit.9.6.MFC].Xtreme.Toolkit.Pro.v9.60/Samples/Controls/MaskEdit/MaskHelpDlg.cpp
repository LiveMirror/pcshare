// MaskHelpDlg.cpp : implementation file
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
#include "maskedit.h"
#include "MaskHelpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaskHelpDlg dialog


CMaskHelpDlg::CMaskHelpDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CMaskHelpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMaskHelpDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pHelpView = NULL;

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	int cx = ::GetSystemMetrics(SM_CXSMICON);
	int cy = ::GetSystemMetrics(SM_CYSMICON);

	// Load the icon for the dialog.
	m_hIcon = (HICON)::LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE( IDR_MASKHELP ), IMAGE_ICON, cx, cy, LR_DEFAULTCOLOR);
}


void CMaskHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaskHelpDlg)
	DDX_Control(pDX, IDC_BORDER_TAB, m_tabBorder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMaskHelpDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CMaskHelpDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaskHelpDlg message handlers

BOOL CMaskHelpDlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);         // Set big icon
	SetIcon(m_hIcon, FALSE);        // Set small icon

	CRect r;
	m_tabBorder.GetWindowRect( &r );
	ScreenToClient( &r );

	// Create  the flat tab control.
	m_pHelpView = ( CHelpView* )XTFuncCreateView( this, RUNTIME_CLASS( CHelpView ) );
	ASSERT_VALID( m_pHelpView );

	m_pHelpView->ModifyStyleEx( WS_EX_CLIENTEDGE, WS_EX_STATICEDGE );
	m_pHelpView->SendMessage( WM_INITIALUPDATE );
	m_pHelpView->SetWindowPos(NULL, r.left, r.top,
		r.Width(), r.Height(), SWP_FRAMECHANGED);

	SetResize(AFX_IDW_PANE_FIRST, SZ_TOP_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDOK,               SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDCANCEL,           SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	// Load window placement
	AutoLoadPlacement(_T("CMaskHelpDlg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
