// CheckListBoxDlg.cpp : implementation file
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
#include "CheckListBox.h"
#include "CheckListBoxDlg.h"
#include "AboutDlg.h"
#include "NewToolbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckListBoxDlg dialog

CCheckListBoxDlg::CCheckListBoxDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CCheckListBoxDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckListBoxDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCheckListBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckListBoxDlg)
	DDX_Control(pDX, IDC_LBOX_CHECK, m_checkList);
	DDX_Control(pDX, IDC_BTN_RENAME, m_renameButton);
	DDX_Control(pDX, IDC_BTN_DELETE, m_deleteButton);
	DDX_Control(pDX, IDCANCEL,          m_exitButton);
	DDX_Control(pDX, IDC_BTN_NEW, m_newButton);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCheckListBoxDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CCheckListBoxDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_RENAME, OnIdcBtnRename)
	ON_LBN_SELCHANGE(IDC_LBOX_CHECK, OnSelchangeIdcLboxCheck)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnIdcBtnDelete)
	ON_BN_CLICKED(IDC_BTN_NEW, OnIdcBtnNew)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckListBoxDlg message handlers

BOOL CCheckListBoxDlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);         // Set big icon
	SetIcon(m_hIcon, FALSE);        // Set small icon

	// initialize the list box.
	m_checkList.Initialize();

	// Set control resizing.
	SetResize(IDC_LBOX_CHECK, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BTN_NEW, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BTN_RENAME, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BTN_DELETE, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	// Load window placement
	LoadPlacement(_T("CCheckListBoxDlg"));

	// Add items to the check box.
	InitCheckListBox();
	OnSelchangeIdcLboxCheck();

	m_renameButton.SetXButtonStyle(BS_XT_SEMIFLAT | BS_XT_WINXP_COMPAT);
	m_deleteButton.SetXButtonStyle(BS_XT_SEMIFLAT | BS_XT_WINXP_COMPAT);
	m_newButton.SetXButtonStyle(BS_XT_SEMIFLAT | BS_XT_WINXP_COMPAT);
	m_exitButton.SetXButtonStyle(BS_XT_SEMIFLAT | BS_XT_WINXP_COMPAT);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCheckListBoxDlg::OnDestroy()
{
	// Save window placement
	SavePlacement(_T("CCheckListBoxDlg"));

	CXTResizeDialog::OnDestroy();
}

void CCheckListBoxDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CXTResizeDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCheckListBoxDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CXTResizeDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCheckListBoxDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCheckListBoxDlg::InitCheckListBox()
{
	m_checkList.AddString (_T("Standard"));
	m_checkList.AddString (_T("Advanced"));
	m_checkList.AddString (_T("Menu Bar"));
	m_checkList.AddString (_T("Web"));

	m_checkList.SetCheck(1, TRUE);
}

void CCheckListBoxDlg::OnSelchangeIdcLboxCheck()
{
	m_renameButton.EnableWindow (m_checkList.GetCurSel() != LB_ERR);
	m_deleteButton.EnableWindow (m_checkList.GetCurSel() != LB_ERR);
}

void CCheckListBoxDlg::OnIdcBtnRename()
{
	int nIndex = m_checkList.GetCurSel();
	if (nIndex == LB_ERR)
		return;

	bool bCheck = m_checkList.GetCheck(nIndex) ? true : false;

	CString strText;
	m_checkList.GetText( nIndex, strText );

	CNewToolbar dlg(strText, _T("Rename Toolbar"), this);
	if (dlg.DoModal() == IDOK)
	{
		m_checkList.DeleteString(nIndex);
		m_checkList.InsertString(nIndex, dlg.m_strToolbar);
		m_checkList.SetCheck(nIndex, bCheck);
		m_checkList.SetCurSel(nIndex);
	}

	OnSelchangeIdcLboxCheck();
}

void CCheckListBoxDlg::OnIdcBtnDelete()
{
	int nIndex = m_checkList.GetCurSel();
	if (nIndex == LB_ERR)
		return;

	CString strText;
	m_checkList.GetText( nIndex, strText );

	CString strMessage;
	strMessage.Format(XTP_IDS_CONFIRM_DELETE, strText);

	if (AfxMessageBox(strMessage, MB_ICONWARNING|MB_OKCANCEL) == IDOK)
	{
		m_checkList.DeleteString(nIndex);

		int nCount = m_checkList.GetCount();

		if (nCount != 0)
		{
			if (nIndex < nCount) {
				m_checkList.SetCurSel(nIndex);
			}
			else {
				m_checkList.SetCurSel(nCount-1);
			}
		}
	}

	OnSelchangeIdcLboxCheck();
}

void CCheckListBoxDlg::OnIdcBtnNew()
{
	int nIndex = m_checkList.GetCount();

	CNewToolbar dlg(_T("New Toolbar"), _T("Rename Toolbar"), this);
	if (dlg.DoModal() == IDOK)
	{
		m_checkList.InsertString(nIndex, dlg.m_strToolbar);
		m_checkList.SetCurSel(nIndex);
	}

	OnSelchangeIdcLboxCheck();
}
