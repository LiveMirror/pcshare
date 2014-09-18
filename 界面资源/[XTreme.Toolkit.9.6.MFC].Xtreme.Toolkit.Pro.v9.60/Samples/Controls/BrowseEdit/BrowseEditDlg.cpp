// BrowseEditDlg.cpp : implementation file
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
#include "BrowseEdit.h"
#include "BrowseEditDlg.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBrowseEditDlg dialog

CBrowseEditDlg::CBrowseEditDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CBrowseEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBrowseEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBrowseEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrowseEditDlg)
	DDX_Control(pDX, IDC_EDIT_MENU, m_editMenu);
	DDX_Control(pDX, IDC_EDIT_DIR,  m_editDir);
	DDX_Control(pDX, IDC_EDIT_FILE, m_editFile);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBrowseEditDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CBrowseEditDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_POPUP_1, OnPopup1)
	ON_COMMAND(ID_POPUP_2, OnPopup2)
	ON_COMMAND(ID_POPUP_3, OnPopup3)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrowseEditDlg message handlers

BOOL CBrowseEditDlg::OnInitDialog()
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

	// Initialize the browse edit controls.
	m_editMenu.Initialize(this, BES_XT_POPUPMENU, IDR_POPUP);
	m_editDir.Initialize(this, BES_XT_CHOOSEDIR);
	m_editFile.Initialize(this, BES_XT_CHOOSEFILE);

#if 0 // remove this to use your own CFileDialog.

	// Instead of using the default file open dialog, create our own
	// file open dialog. You should make this a member variable
	// and initialize it in the constructor initializer list.
	static CFileDialog dlg( TRUE, _T("txt"), _T("myfile.txt"),
		OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST,
		_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"), this );

	// Set the directory where the file open dialog will start from.
	dlg.m_ofn.lpstrInitialDir = _T("C:\\WINNT");

	// Set the title for the file open dialog.
	dlg.m_ofn.lpstrTitle = _T("User Defined File Open Dialog");

	// Set the CFileDialog object for the browse edit class.
	m_editFile.SetFileDialog(&dlg);

#endif

	// Set control resizing.
	SetResize(IDC_EDIT_FILE, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_DIR, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_MENU, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	// prevent vertical resizing.
	SetFlag(xtResizeNoVertical);

	// Load window placement
	LoadPlacement(_T("CBrowseEditDlg"));

	CXTPPaintManager::SetTheme(xtpThemeWhidbey);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBrowseEditDlg::OnDestroy()
{
	// Save window placement
	SavePlacement(_T("CBrowseEditDlg"));

	CXTResizeDialog::OnDestroy();
}

void CBrowseEditDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBrowseEditDlg::OnPaint()
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
HCURSOR CBrowseEditDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBrowseEditDlg::OnPopup1()
{
	m_editMenu.SetWindowText(_T("Selection One..."));
}

void CBrowseEditDlg::OnPopup2()
{
	m_editMenu.SetWindowText(_T("Selection Two..."));
}

void CBrowseEditDlg::OnPopup3()
{
	m_editMenu.SetWindowText(_T("Selection Three..."));
}
