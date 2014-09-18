// BrowseDialogDlg.cpp : implementation file
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
#include "BrowseDialog.h"
#include "BrowseDialogDlg.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBrowseDialogDlg dialog

CBrowseDialogDlg::CBrowseDialogDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CBrowseDialogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBrowseDialogDlg)
	m_strDirectory = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBrowseDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrowseDialogDlg)
	DDX_Text(pDX, IDC_EDIT_DIR, m_strDirectory);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBrowseDialogDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CBrowseDialogDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBtnBrowse)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrowseDialogDlg message handlers

BOOL CBrowseDialogDlg::OnInitDialog()
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

	// Set control resizing.
	SetResize(IDC_EDIT_DIR, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_BTN_BROWSE, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	// prevent vertical resizing.
	SetFlag(xtResizeNoVertical);

	// Load window placement
	LoadPlacement(_T("CBrowseDialogDlg"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBrowseDialogDlg::OnDestroy()
{
	// Save window placement
	SavePlacement(_T("CBrowseDialogDlg"));

	CXTResizeDialog::OnDestroy();
}

void CBrowseDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBrowseDialogDlg::OnPaint()
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
HCURSOR CBrowseDialogDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBrowseDialogDlg::OnBtnBrowse()
{
	if (UpdateData(TRUE))
	{
		CXTBrowseDialog dlg;
		dlg.SetTitle(_T("Select Your Directory"));
		if (!m_strDirectory.IsEmpty())
		{
			TCHAR path[_MAX_PATH];
			STRCPY_S(path, _MAX_PATH, m_strDirectory);
			dlg.SetSelPath(path);
		}
		if (dlg.DoModal() == IDOK)
		{
			m_strDirectory = dlg.GetSelPath();
			UpdateData(FALSE);
		}
	}
}
