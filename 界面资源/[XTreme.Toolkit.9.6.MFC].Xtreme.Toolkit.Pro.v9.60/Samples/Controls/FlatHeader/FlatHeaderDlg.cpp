// FlatHeaderDlg.cpp : implementation file
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
#include "FlatHeader.h"
#include "FlatHeaderDlg.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BoolType(b) b?true:false

/////////////////////////////////////////////////////////////////////////////
// CFlatHeaderDlg dialog

CFlatHeaderDlg::CFlatHeaderDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CFlatHeaderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlatHeaderDlg)
	m_iTheme = 1;
	m_iDisableSizing = 0;
	m_iMinSize = 25;
	m_bBoldText  = FALSE;
	m_bSortArrow = TRUE;
	m_bAutoSize = TRUE;
	m_bContextMenus = TRUE;
	m_bHotTracking = TRUE;
	m_bHeaderImages = TRUE;
	m_bWinTheme = FALSE;
	m_bDisableSizing = FALSE;
	m_bMinSize = FALSE;
	m_bDragDrop = FALSE;
	//}}AFX_DATA_INIT

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nSortedCol = 1;
	m_bAscending = true;

	CXTPWinThemeWrapper themeWrapper;
	m_bWinTheme  = themeWrapper.IsThemeActive();
}

void CFlatHeaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlatHeaderDlg)
	DDX_CBIndex(pDX, IDC_COMBO_THEMES, m_iTheme);
	DDX_CBIndex(pDX, IDC_COMBO_DISABLESIZE, m_iDisableSizing);
	DDX_Text(pDX, IDC_EDIT_MINSIZE, m_iMinSize);
	DDX_Check(pDX, IDC_CHK_BOLDTEXT, m_bBoldText);
	DDX_Check(pDX, IDC_CHK_SORTARW, m_bSortArrow);
	DDX_Check(pDX, IDC_CHK_AUTOSIZE, m_bAutoSize);
	DDX_Check(pDX, IDC_CHK_CONTEXTMENUS, m_bContextMenus);
	DDX_Check(pDX, IDC_CHK_HOTTRACKING, m_bHotTracking);
	DDX_Check(pDX, IDC_CHK_IMAGES, m_bHeaderImages);
	DDX_Check(pDX, IDC_CHK_WINTHEME, m_bWinTheme);
	DDX_Check(pDX, IDC_CHK_DISABLESIZING, m_bDisableSizing);
	DDX_Check(pDX, IDC_CHK_MINIMUMSIZE, m_bMinSize);
	DDX_Check(pDX, IDC_CHK_DRAGDROP, m_bDragDrop);
	DDX_Control(pDX, IDC_EDIT_MINSIZE, m_editMinSize);
	DDX_Control(pDX, IDC_CHK_WINTHEME, m_btnWinTheme);
	DDX_Control(pDX, IDC_CHK_HOTTRACKING, m_btnHotTracking);
	DDX_Control(pDX, IDC_COMBO_THEMES, m_comboThemes);
	DDX_Control(pDX, IDC_COMBO_DISABLESIZE, m_comboDisableSize);
	DDX_Control(pDX, IDC_LIST_CTRL, m_listCtrl);
	DDX_Control(pDX, IDC_GBOX_HEADER, m_gboxHeader);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFlatHeaderDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CFlatHeaderDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHK_BOLDTEXT, OnChkBoldtext)
	ON_BN_CLICKED(IDC_CHK_SORTARW, OnChkSortarw)
	ON_WM_DESTROY()
	ON_CBN_SELENDOK(IDC_COMBO_THEMES, OnSelendokComboThemes)
	ON_CBN_SELENDOK(IDC_COMBO_DISABLESIZE, OnSelendokComboDisablesize)
	ON_BN_CLICKED(IDC_CHK_WINTHEME, OnChkWintheme)
	ON_BN_CLICKED(IDC_CHK_HOTTRACKING, OnChkHottracking)
	ON_BN_CLICKED(IDC_CHK_IMAGES, OnChkImages)
	ON_BN_CLICKED(IDC_CHK_AUTOSIZE, OnChkAutosize)
	ON_BN_CLICKED(IDC_CHK_CONTEXTMENUS, OnChkContextmenus)
	ON_EN_CHANGE(IDC_EDIT_MINSIZE, OnChangeEditMinsize)
	ON_BN_CLICKED(IDC_CHK_DISABLESIZING, OnChkDisablesizing)
	ON_BN_CLICKED(IDC_CHK_MINIMUMSIZE, OnChkMinimumsize)
	ON_BN_CLICKED(IDC_CHK_DRAGDROP, OnChkHeaderDragDrop)
	ON_BN_CLICKED(IDC_BTN_SIZETOFIT, OnBtnSizeToFit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlatHeaderDlg message handlers

BOOL CFlatHeaderDlg::OnInitDialog()
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
	SetResize(IDOK,                  SZ_TOP_RIGHT,   SZ_TOP_RIGHT);
	SetResize(IDCANCEL,              SZ_TOP_RIGHT,   SZ_TOP_RIGHT);
	SetResize(IDC_LIST_CTRL,         SZ_TOP_LEFT,    SZ_BOTTOM_RIGHT);

	SetResize(IDC_GBOX_HEADER,       SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_COMBO_THEMES,      SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHK_AUTOSIZE,      SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BTN_SIZETOFIT,     SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHK_DISABLESIZING, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_COMBO_DISABLESIZE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHK_MINIMUMSIZE,   SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MINSIZE,      SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHK_WINTHEME,      SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHK_HOTTRACKING,   SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHK_IMAGES,        SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHK_BOLDTEXT,      SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHK_SORTARW,       SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHK_CONTEXTMENUS,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHK_DRAGDROP,      SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_TXT_SEEALSO,       SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	// Enable/Disable XP GUI Mode
	XTAuxData().bXPMode = TRUE;

	// Load window placement
	LoadPlacement(_T("CFlatHeaderDlg"));

	// TODO: Add extra initialization here
	m_listCtrl.InsertColumn(0, _T("Column One"),   LVCFMT_LEFT, 125);
	m_listCtrl.InsertColumn(1, _T("Column Two"),   LVCFMT_LEFT, 125);
	m_listCtrl.InsertColumn(2, _T("Column Three"), LVCFMT_LEFT, 125);

	int iItem;
	for (iItem = 0; iItem < 100; ++iItem)
	{
		CString strItem;
		CString strSubA;
		CString strSubB;

		strItem.Format(_T("Item %d"), iItem);
		strSubA.Format(_T("Sub A %d"), iItem);
		strSubB.Format(_T("Sub B %d"), iItem);

		m_listCtrl.InsertItem(iItem, strItem, 0);
		m_listCtrl.SetItem(iItem, 1, LVIF_TEXT, strSubA, 0, NULL, NULL, NULL);
		m_listCtrl.SetItem(iItem, 2, LVIF_TEXT, strSubB, 0, NULL, NULL, NULL);
	}

	// Get the windows handle to the header control for the
	// list control then subclass the control.
	HWND hWndHeader = m_listCtrl.GetDlgItem(0)->GetSafeHwnd();
	m_header.SubclassWindow(hWndHeader);

	// add bitmap images.
	m_header.SetBitmap(0, IDB_COLUMN_0, FALSE, RGB(0,255,0));
	m_header.SetBitmap(1, IDB_COLUMN_1, FALSE, RGB(0,255,0));
	m_header.SetBitmap(2, IDB_COLUMN_2, FALSE, RGB(0,255,0));

	// enable auto sizing.
	m_header.EnableAutoSize(TRUE);
	m_header.ResizeColumnsToFit();
	m_listCtrl.EnableUserSortColor(true);
	SortColumn(m_nSortedCol, m_bAscending);

	OnSelendokComboThemes();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFlatHeaderDlg::OnDestroy()
{
	// Save window placement
	SavePlacement(_T("CFlatHeaderDlg"));

	CXTResizeDialog::OnDestroy();
}

void CFlatHeaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFlatHeaderDlg::OnPaint()
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
HCURSOR CFlatHeaderDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFlatHeaderDlg::SortColumn(int iCol, bool bAsc)
{
	m_bAscending = bAsc;
	m_nSortedCol = iCol;

	// set sort image for header and sort column.
	m_listCtrl.SetSortImage(m_nSortedCol, m_bAscending);

	CXTSortClass csc(&m_listCtrl, m_nSortedCol);
	csc.Sort(m_bAscending, xtSortString);
}

BOOL CFlatHeaderDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	HD_NOTIFY *pHDNotify = (HD_NOTIFY*)lParam;

	if (pHDNotify->hdr.code == HDN_ITEMCLICKA ||
		pHDNotify->hdr.code == HDN_ITEMCLICKW)
	{
		if (pHDNotify->iItem == m_nSortedCol)
			SortColumn(pHDNotify->iItem, !m_bAscending);
		else
			SortColumn(pHDNotify->iItem, BoolType(m_header.GetAscending()));
	}

	return CXTResizeDialog::OnNotify(wParam, lParam, pResult);
}

void CFlatHeaderDlg::OnSelendokComboThemes()
{
	UpdateData();

	switch (m_iTheme)
	{
	case 0:
		m_bHotTracking = FALSE;
		m_bWinTheme = FALSE;
		m_header.SetTheme(new CXTHeaderThemeOfficeXP());
		CXTPPaintManager::SetTheme(xtpThemeOfficeXP);
		break;
	case 1:
		m_bHotTracking = FALSE;
		m_bWinTheme = FALSE;
		m_header.SetTheme(new CXTHeaderThemeOffice2003());
		CXTPPaintManager::SetTheme(xtpThemeOffice2003);
		break;
	case 2:
		m_bHotTracking = TRUE;
		m_bWinTheme = TRUE;
		m_header.SetTheme(new CXTHeaderThemeExplorer());
		CXTPPaintManager::SetTheme(xtpThemeOffice2000);
		break;
	case 3:
		m_bHotTracking = TRUE;
		m_bWinTheme = TRUE;
		m_header.SetTheme(new CXTHeaderTheme());
		CXTPPaintManager::SetTheme(xtpThemeOffice2000);
		break;
	}

	UpdateData(FALSE);

	DWORD dwStyle = 0L;
	if (m_bWinTheme && m_iTheme >= 2)
		dwStyle |= XTTHEME_WINXPTHEMES;

	if (m_bWinTheme && m_iTheme >= 2 && m_bHotTracking)
		dwStyle |= XTTHEME_HOTTRACKING;

	if (m_bSortArrow)
		dwStyle |= XTTHEME_SORTARROW;

	CXTHeaderTheme* pTheme = m_header.GetTheme();
	if (pTheme)
	{
		pTheme->SetDrawStyle(dwStyle, &m_header);
		EnableControls();
	}
}

void CFlatHeaderDlg::OnSelendokComboDisablesize()
{
	UpdateData();

	// thaw all columns.
	m_header.ThawAllColumns();

	if (m_iDisableSizing > 0)
		m_header.FreezeColumn(m_iDisableSizing-1);
}

void CFlatHeaderDlg::ModifyDrawStyle(UINT uFlag, BOOL bRemove)
{
	CXTHeaderTheme* pTheme = m_header.GetTheme();
	if (pTheme)
	{
		DWORD dwStyle = pTheme->GetDrawStyle() & ~uFlag;

		if (bRemove)
			dwStyle |= uFlag;

		pTheme->SetDrawStyle(dwStyle, &m_header);
		m_header.RedrawWindow();
	}
}

void CFlatHeaderDlg::OnChkWintheme()
{
	UpdateData();
	EnableControls(FALSE);
	ModifyDrawStyle(XTTHEME_WINXPTHEMES, m_bWinTheme);
}

void CFlatHeaderDlg::OnChkHottracking()
{
	UpdateData();
	ModifyDrawStyle(XTTHEME_HOTTRACKING, m_bHotTracking);
}

void CFlatHeaderDlg::OnChkImages()
{
	UpdateData();

	// add / remove bitmap images.
	m_header.SetBitmap(0, IDB_COLUMN_0, !m_bHeaderImages, RGB(0,255,0));
	m_header.SetBitmap(1, IDB_COLUMN_1, !m_bHeaderImages, RGB(0,255,0));
	m_header.SetBitmap(2, IDB_COLUMN_2, !m_bHeaderImages, RGB(0,255,0));
}

void CFlatHeaderDlg::OnChkBoldtext()
{
	UpdateData();
	m_header.InitializeHeader(BoolType(m_bBoldText));
	m_header.RedrawWindow();
}

void CFlatHeaderDlg::OnChkSortarw()
{
	UpdateData();
	ModifyDrawStyle(XTTHEME_SORTARROW, m_bSortArrow);
}

void CFlatHeaderDlg::OnChkContextmenus()
{
	UpdateData();
	m_header.EnablePopupMenus(m_bContextMenus);
}

void CFlatHeaderDlg::OnChangeEditMinsize()
{
	UpdateData();
	m_header.SetMinSize(m_iMinSize);
}

void CFlatHeaderDlg::EnableControls(BOOL bRedraw/*=TRUE*/)
{
	m_editMinSize.EnableWindow(m_bMinSize);
	m_comboDisableSize.EnableWindow(m_bDisableSizing);
	m_btnWinTheme.EnableWindow(XTOSVersionInfo()->IsWinXPOrGreater());
	m_btnHotTracking.EnableWindow(XTOSVersionInfo()->IsWinXPOrGreater() && m_bWinTheme);

	if (bRedraw)
	{
		RedrawWindow(NULL, NULL,
			RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
	}
}

void CFlatHeaderDlg::OnChkAutosize()
{
	UpdateData();
	EnableControls(FALSE);

	// enable auto sizing.
	m_header.EnableAutoSize(BoolType(m_bAutoSize));

	if (m_bAutoSize)
		m_header.ResizeColumnsToFit();
}

void CFlatHeaderDlg::OnChkDisablesizing()
{
	UpdateData();
	EnableControls(FALSE);

	if (!m_bDisableSizing)
	{
		m_iDisableSizing = 0;
		m_header.ThawAllColumns();
		UpdateData(FALSE);
	}
}

void CFlatHeaderDlg::OnChkMinimumsize()
{
	UpdateData();
	EnableControls(FALSE);

	// enable minimum sizing.
	m_iMinSize = 25;
	m_header.SetMinSize(m_iMinSize);
	UpdateData(FALSE);
}

void CFlatHeaderDlg::OnChkHeaderDragDrop()
{
	UpdateData();

	m_listCtrl.ModifyExtendedStyle( m_bDragDrop ? 0 : LVS_EX_HEADERDRAGDROP,
		m_bDragDrop ? LVS_EX_HEADERDRAGDROP : 0 );
}

void CFlatHeaderDlg::OnBtnSizeToFit()
{
	m_header.ResizeColumnsToFit();
}
