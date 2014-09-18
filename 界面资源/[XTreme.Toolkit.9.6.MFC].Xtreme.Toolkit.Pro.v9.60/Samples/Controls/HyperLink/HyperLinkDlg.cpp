// HyperLinkDlg.cpp : implementation file
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
#include "HyperLink.h"
#include "HyperLinkDlg.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHyperLinkDlg dialog

CHyperLinkDlg::CHyperLinkDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CHyperLinkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHyperLinkDlg)
	m_bUnderline = TRUE;
	m_bVisited = FALSE;
	m_strURL = _T("http://www.codejock.com");
	m_strEditTxt = _T("Visit Codejock Software's home page");
	m_bShellExecute = TRUE;
	//}}AFX_DATA_INIT

	m_clrLink    = RGB(0x00, 0x00, 0xFF);   // blue
	m_clrHover   = RGB(0xFF, 0x00, 0x00);   // red
	m_clrVisited = RGB(0x80, 0x00, 0x80);   // purple

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHyperLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHyperLinkDlg)
	DDX_Control(pDX, IDC_HYP_TEXT, m_hyperLink);
	DDX_Control(pDX, IDC_CLR_LINK, m_cpLink);
	DDX_Control(pDX, IDC_CLR_HOVER, m_cpHover);
	DDX_Control(pDX, IDC_CLR_VISITED, m_cpVisited);
	DDX_Check(pDX, IDC_CHK_UNDERLINE, m_bUnderline);
	DDX_Check(pDX, IDC_CHK_VISITED, m_bVisited);
	DDX_Text(pDX, IDC_EDIT_URL, m_strURL);
	DDX_Text(pDX, IDC_EDIT_TXT, m_strEditTxt);
	DDX_Check(pDX, IDC_CHK_SHELL, m_bShellExecute);
	//}}AFX_DATA_MAP
	DDX_XTColorPicker(pDX, IDC_CLR_LINK, m_clrLink);
	DDX_XTColorPicker(pDX, IDC_CLR_HOVER, m_clrHover);
	DDX_XTColorPicker(pDX, IDC_CLR_VISITED, m_clrVisited);
}

BEGIN_MESSAGE_MAP(CHyperLinkDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CHyperLinkDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT_URL, OnChangeEditUrl)
	ON_BN_CLICKED(IDC_CHK_UNDERLINE, OnChkUnderline)
	ON_BN_CLICKED(IDC_CHK_VISITED, OnChkVisited)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_TXT, OnChangeEditTxt)
	ON_BN_CLICKED(IDC_HYP_TEXT, OnHypText)
	ON_BN_CLICKED(IDC_CHK_SHELL, OnChkShell)
	//}}AFX_MSG_MAP
	ON_CPN_XT_SELENDOK(IDC_CLR_LINK, OnSelEndOkColor)
	ON_CPN_XT_SELENDOK(IDC_CLR_HOVER, OnSelEndOkColor)
	ON_CPN_XT_SELENDOK(IDC_CLR_VISITED, OnSelEndOkColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHyperLinkDlg message handlers

BOOL CHyperLinkDlg::OnInitDialog()
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
	SetResize(IDC_HYP_TEXT, SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_TXT, SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_URL, SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDCANCEL,     SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	// prevent vertical resizing.
	SetFlag(xtResizeNoVertical);

	// Load window placement
	LoadPlacement(_T("CHyperLinkDlg"));

	m_cpVisited.SetColor(m_clrVisited);
	m_cpVisited.SetDefaultColor(m_clrVisited);

	m_cpLink.SetColor(m_clrLink);
	m_cpLink.SetDefaultColor(m_clrLink);

	m_cpHover.SetColor(m_clrHover);
	m_cpHover.SetDefaultColor(m_clrHover);

	m_hyperLink.SetURL(m_strURL);
	m_hyperLink.SetColors(m_clrLink, m_clrVisited, m_clrHover);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHyperLinkDlg::OnDestroy()
{
	// Save window placement
	SavePlacement(_T("CHyperLinkDlg"));

	CXTResizeDialog::OnDestroy();
}

void CHyperLinkDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHyperLinkDlg::OnPaint()
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
HCURSOR CHyperLinkDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CHyperLinkDlg::OnSelEndOkColor()
{
	UpdateData();
	m_hyperLink.SetColors(m_clrLink, m_clrVisited, m_clrHover);
}

void CHyperLinkDlg::OnChangeEditUrl()
{
	UpdateData();
	m_hyperLink.SetURL(m_strURL);
}

void CHyperLinkDlg::OnChkUnderline()
{
	UpdateData();
	m_hyperLink.SetUnderline(m_bUnderline?true:false);
	m_hyperLink.Invalidate();
}

void CHyperLinkDlg::OnChkVisited()
{
	UpdateData();
	m_hyperLink.SetVisited(m_bVisited?true:false);
}

void CHyperLinkDlg::OnChangeEditTxt()
{
	UpdateData();
	m_hyperLink.SetWindowText(m_strEditTxt);
	m_hyperLink.Invalidate();
}

void CHyperLinkDlg::OnHypText()
{
	m_bVisited = TRUE;
	UpdateData(false);

	if (!m_bShellExecute) {
		AfxMessageBox(_T("Disabling Shell Execute will allow you to perform your own task when the hyperlink is clicked."));
	}
}

void CHyperLinkDlg::OnChkShell()
{
	UpdateData();
	m_hyperLink.EnableShellExecute(m_bShellExecute ? true : false);
}
