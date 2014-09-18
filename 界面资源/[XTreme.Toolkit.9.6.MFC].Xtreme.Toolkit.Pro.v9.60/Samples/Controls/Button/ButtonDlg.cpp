// ButtonDlg.cpp : implementation file
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
#include "Button.h"
#include "ButtonDlg.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonDlg dialog

CButtonDlg::CButtonDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CButtonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CButtonDlg)
	m_bEnable = TRUE;
	m_bFocus = FALSE;
	m_bHilite = FALSE;
	m_nFlatState = 1;
	m_nTextJustification = 1;
	m_bHideIcons = FALSE;
	m_bWinXPThemes = FALSE;
	m_bWordTheme = TRUE;
	m_intTheme = 1;
	m_bAlpha = FALSE;
	m_bToggleButtons = FALSE;
	//}}AFX_DATA_INIT

	m_dwXTBtnStyle = BS_XT_SEMIFLAT;

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CButtonDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CButtonDlg)
	DDX_Control(pDX, IDC_CHK_ALPHA, m_chkAlpha);
	DDX_Control(pDX, IDC_CHK_WINXP_THEMES, m_btnWinXPThemes);
	DDX_Control(pDX, IDC_CHK_HILITE, m_chkHilite);
	DDX_Control(pDX, IDC_RAD_FLAT, m_radFlat);
	DDX_Control(pDX, IDC_RAD_SEMIFLAT, m_radSemiFlat);
	DDX_Control(pDX, IDC_RAD_NORMAL, m_radNormal);
	DDX_Control(pDX, IDC_CHK_WORD_THEME, m_chkWordTheme);
	DDX_Control(pDX, IDC_GBOX_TWOROWS, m_gboxTwoRows);
	DDX_Control(pDX, IDC_GBOX_REGULAR, m_gboxRegular);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_BUTTON4, m_btn4);
	DDX_Control(pDX, IDC_BUTTON3, m_btn3);
	DDX_Control(pDX, IDC_BUTTON2, m_btn2);
	DDX_Control(pDX, IDC_BUTTON1, m_btn1);
	DDX_Check(pDX, IDC_CHK_ENABLED, m_bEnable);
	DDX_Check(pDX, IDC_CHK_FOCUS, m_bFocus);
	DDX_Check(pDX, IDC_CHK_HILITE, m_bHilite);
	DDX_Radio(pDX, IDC_RAD_NORMAL, m_nFlatState);
	DDX_Radio(pDX, IDC_RAD_LEFT, m_nTextJustification);
	DDX_Check(pDX, IDC_HIDEICONS, m_bHideIcons);
	DDX_Check(pDX, IDC_CHK_WINXP_THEMES, m_bWinXPThemes);
	DDX_Check(pDX, IDC_CHK_WORD_THEME, m_bWordTheme);
	DDX_Radio(pDX, IDC_RAD_CLASSIC, m_intTheme);
	DDX_Check(pDX, IDC_CHK_ALPHA, m_bAlpha);
	DDX_Check(pDX, IDC_CHECK_TOGGLE, m_bToggleButtons);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CButtonDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CButtonDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHK_ENABLED, OnChkEnabled)
	ON_BN_CLICKED(IDC_CHK_CENTERED, OnChkCentered)
	ON_BN_CLICKED(IDC_CHK_FOCUS, OnChkFocus)
	ON_BN_CLICKED(IDC_RAD_NORMAL, OnSetBorder)
	ON_BN_CLICKED(IDC_CHK_HILITE, OnChkHilite)
	ON_BN_CLICKED(IDC_RAD_LEFT, OnUpdateTextJustification)
	ON_BN_CLICKED(IDC_HIDEICONS, OnHideicons)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RAD_CLASSIC, OnRadClassic)
	ON_BN_CLICKED(IDC_RAD_OFFICE_XP, OnRadOfficeXp)
	ON_BN_CLICKED(IDC_RAD_OFFICE_2003, OnRadOffice2003)
	ON_BN_CLICKED(IDC_CHK_WINXP_THEMES, OnChkWinxpThemes)
	ON_BN_CLICKED(IDC_CHK_WORD_THEME, OnChkWordTheme)
	ON_WM_SYSCOLORCHANGE()
	ON_BN_CLICKED(IDC_CHK_ALPHA, OnChkAlpha)
	ON_BN_CLICKED(IDC_RAD_FLAT, OnSetBorder)
	ON_BN_CLICKED(IDC_RAD_SEMIFLAT, OnSetBorder)
	ON_BN_CLICKED(IDC_RAD_CENTER, OnUpdateTextJustification)
	ON_BN_CLICKED(IDC_RAD_RIGHT, OnUpdateTextJustification)
	ON_BN_CLICKED(IDC_CHECK_TOGGLE, OnCheckToggle)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON1, IDC_BUTTON4, OnButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonDlg message handlers

BOOL CButtonDlg::OnInitDialog()
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
	SetResize(IDC_TXT_THEME, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_FRAME_THEME, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RAD_CLASSIC, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RAD_OFFICE_XP, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RAD_OFFICE_2003, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHK_WINXP_THEMES, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHK_WORD_THEME, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHK_ALPHA, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_TXT_GENERAL, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_FRAME_GENERAL, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHK_ENABLED, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHK_FOCUS, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHK_HILITE, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_HIDEICONS, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_TOGGLE, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_TXT_BORDER, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_FRAME_BORDER, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RAD_NORMAL, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RAD_SEMIFLAT, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RAD_FLAT, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_TXT_JUST, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_FRAME_JUST, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RAD_LEFT, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RAD_CENTER, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RAD_RIGHT, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDOK, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_GBOX_REGULAR, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON1, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON2, SZ_TOP_LEFT, SZ_TOP_RIGHT);

	SetResize(IDC_GBOX_TWOROWS, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON3, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON4, SZ_TOP_LEFT, SZ_TOP_RIGHT);

	// prevent vertical resizing.
	SetFlag(xtResizeNoVertical);

	// Load window placement
	LoadPlacement(_T("CButtonDlg"));

	//------------------------------------
	// TODO: Add extra initialization here
	//------------------------------------

	m_bitmap.LoadBitmap(IDB_IMAGELIST);
	m_imageList.Create(24, 24, ILC_COLOR24|ILC_MASK, 4, 1);
	m_imageList.Add(&m_bitmap, RGB(0,255,0));

	m_btnOK.SetBitmap(CSize(16, 16), IDB_OK);

	UpdateIcons(FALSE);
	m_btnOK.SetTheme(new CXTButtonThemeOfficeXP(TRUE));

	OnRadOfficeXp();
	UpdateXTStyle();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CButtonDlg::OnDestroy()
{
	// Save window placement
	SavePlacement(_T("CButtonDlg"));

	CXTResizeDialog::OnDestroy();
}

void CButtonDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CButtonDlg::OnPaint()
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
HCURSOR CButtonDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CButtonDlg::OnChkEnabled()
{
	UpdateData();

	m_btn1.EnableWindow(m_bEnable);
	m_btn2.EnableWindow(m_bEnable);
	m_btn3.EnableWindow(m_bEnable);
	m_btn4.EnableWindow(m_bEnable);
}

void CButtonDlg::OnChkCentered()
{
	UpdateData();
	UpdateXTStyle();
}

void CButtonDlg::OnChkFocus()
{
	UpdateData();

	m_dwXTBtnStyle = (m_bFocus ?
		(m_dwXTBtnStyle |  BS_XT_SHOWFOCUS) :
		(m_dwXTBtnStyle & ~BS_XT_SHOWFOCUS));

	UpdateXTStyle();
}

void CButtonDlg::OnSetBorder()
{
	UpdateData();

	m_dwXTBtnStyle &= ~(BS_XT_SEMIFLAT | BS_XT_FLAT);
	switch (m_nFlatState)
	{
	case 0: // normal
		break;
	case 1: // semi-flat
		m_dwXTBtnStyle |= BS_XT_SEMIFLAT;
		break;
	case 2: // flat
		m_dwXTBtnStyle |= BS_XT_FLAT;
		break;
	}

	UpdateXTStyle();
}

void CButtonDlg::OnChkHilite()
{
	UpdateData();

	m_dwXTBtnStyle = (m_bHilite ?
		(m_dwXTBtnStyle |  BS_XT_HILITEPRESSED) :
		(m_dwXTBtnStyle & ~BS_XT_HILITEPRESSED));

	UpdateXTStyle();
}

void CButtonDlg::UpdateXTStyle()
{
	m_btn1.SetXButtonStyle(m_dwXTBtnStyle);
	m_btn2.SetXButtonStyle(m_dwXTBtnStyle);
	m_btn3.SetXButtonStyle(m_dwXTBtnStyle | BS_XT_TWOROWS);
	m_btn4.SetXButtonStyle(m_dwXTBtnStyle | BS_XT_TWOROWS);
}

void CButtonDlg::ModifyBtnStyle(DWORD dwRemove, DWORD dwAdd)
{
	m_btn1.ModifyStyle(dwRemove, dwAdd);
	m_btn2.ModifyStyle(dwRemove, dwAdd);
	m_btn3.ModifyStyle(dwRemove, dwAdd);
	m_btn4.ModifyStyle(dwRemove, dwAdd);
	m_btn1.Invalidate();
	m_btn2.Invalidate();
	m_btn3.Invalidate();
	m_btn4.Invalidate();
}

void CButtonDlg::OnUpdateTextJustification()
{
	const DWORD STYLE_ALL = BS_LEFT | BS_RIGHT | BS_CENTER;
	UpdateData();
	DWORD dwAdd = 0;
	switch (m_nTextJustification)
	{
		case 0:
			dwAdd = BS_LEFT;
			break;
		case 1:
			dwAdd = BS_CENTER;
			break;
		case 2:
			dwAdd = BS_RIGHT;
			break;
	}
	ModifyBtnStyle(STYLE_ALL & ~dwAdd, dwAdd);
}

void CButtonDlg::OnHideicons()
{
	UpdateData();

	CXTButton::GetDefaultTheme()->ShowIcon(!m_bHideIcons);

	RedrawWindow(NULL, NULL,
		RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
}

void CButtonDlg::EnableButtons(XTThemeStyle enumTheme)
{
	if (!XTOSVersionInfo()->IsWinXPOrGreater())
	{
		m_bWinXPThemes = FALSE;
		m_btnWinXPThemes.EnableWindow(FALSE);
	}

	if (!XTPImageManager()->IsAlphaIconsSupported())
	{
		m_chkAlpha.EnableWindow(FALSE);
	}

	m_chkWordTheme.EnableWindow(
		!m_bWinXPThemes && enumTheme != xtThemeDefault);
	m_radFlat.EnableWindow(
		!m_bWinXPThemes && enumTheme == xtThemeDefault);
	m_radSemiFlat.EnableWindow(
		!m_bWinXPThemes && enumTheme == xtThemeDefault);
	m_radNormal.EnableWindow(
		!m_bWinXPThemes && enumTheme == xtThemeDefault);
	m_chkHilite.EnableWindow(
		!m_bWinXPThemes && enumTheme == xtThemeDefault);

	RedrawWindow(NULL, NULL,
		RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
}

void CButtonDlg::ChangeTheme(XTThemeStyle enumTheme)
{
	XTThemeManager()->SetTheme(enumTheme);

	SetWinxpThemes();
	SetWordTheme();

	EnableButtons(enumTheme);
}

void CButtonDlg::OnRadClassic()
{
	UpdateData();

	ChangeTheme(xtThemeDefault);
}

void CButtonDlg::OnRadOfficeXp()
{
	ChangeTheme(xtThemeOfficeXP);
}

void CButtonDlg::OnRadOffice2003()
{
	ChangeTheme(xtThemeOffice2003);
}

void CButtonDlg::SetWinxpThemes()
{
	UpdateData();

	m_dwXTBtnStyle = (m_bWinXPThemes ?
		(m_dwXTBtnStyle |  BS_XT_WINXP_COMPAT) :
		(m_dwXTBtnStyle & ~BS_XT_WINXP_COMPAT));

	UpdateXTStyle();
}

void CButtonDlg::OnChkWinxpThemes()
{
	SetWinxpThemes();
	EnableButtons(XTThemeManager()->GetTheme());
}

void CButtonDlg::SetWordTheme()
{
	UpdateData();

	if (XTThemeManager()->GetTheme() == xtThemeOfficeXP ||
		XTThemeManager()->GetTheme() == xtThemeOffice2003)
	{
		((CXTButtonThemeOfficeXP*)CXTButton::GetDefaultTheme())->UseWordTheme(m_bWordTheme);
	}
}

void CButtonDlg::OnChkWordTheme()
{
	SetWordTheme();
	EnableButtons(XTThemeManager()->GetTheme());
}

void CButtonDlg::OnChkAlpha()
{
	UpdateData();

	UpdateIcons(m_bAlpha);
}


void CButtonDlg::OnSysColorChange()
{
	CXTResizeDialog::OnSysColorChange();

	XTPColorManager()->RefreshColors();
	XTThemeManager()->RefreshMetrics();

	RedrawWindow(NULL, NULL,
		RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
}


void CButtonDlg::UpdateIcons(BOOL bAlpha)
{
	if (!bAlpha)
	{
		m_btn1.SetIcon(CSize(24,24), m_imageList.ExtractIcon(0));
		m_btn2.SetIcon(CSize(24,24), m_imageList.ExtractIcon(1), m_imageList.ExtractIcon(2));
		m_btn3.SetIcon(CSize(24,24), m_imageList.ExtractIcon(3));
		m_btn4.SetIcon(CSize(24,24), m_imageList.ExtractIcon(4));

	} else
	{
		m_btn1.SetBitmap(0, IDB_ALPHA_SETTINGS);
		m_btn2.SetBitmap(0, IDB_ALPHA_FIND);
		m_btn3.SetBitmap(0, IDB_ALPHA_HELP);
		m_btn4.SetBitmap(0, IDB_ALPHA_RUN);
	}
}

void CButtonDlg::OnCheckToggle()
{
	UpdateData();

	if (!m_bToggleButtons)
	{
		m_btn1.SetChecked(FALSE);
		m_btn2.SetChecked(FALSE);
		m_btn3.SetChecked(FALSE);
		m_btn4.SetChecked(FALSE);
	}
}

void CButtonDlg::OnButtonClicked(UINT nID)
{
	CXTButton* pButton = DYNAMIC_DOWNCAST(CXTButton, GetDlgItem(nID));
	ASSERT(pButton);

	if (m_bToggleButtons)
	{
		pButton->SetChecked(!pButton->GetChecked());
	}
}
