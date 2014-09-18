// FlatComboDlg.cpp : implementation file
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
#include "FlatCombo.h"
#include "FlatComboDlg.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFlatComboDlg dialog

CFlatComboDlg::CFlatComboDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CFlatComboDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlatComboDlg)
	m_bAutoComplete = FALSE;
	m_bDisable1 = FALSE;
	m_bDisable2 = FALSE;
	m_nTheme = 2;
	//}}AFX_DATA_INIT

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFlatComboDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlatComboDlg)
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_FLAT_EDIT, m_wndFlatEdit);
	DDX_Control(pDX, IDC_GBOX_LIST, m_gboxList);
	DDX_Control(pDX, IDC_GBOX_DOWN, m_gboxDown);
	DDX_Control(pDX, IDC_GBOX_APPEAR, m_gboxAppear);
	DDX_Control(pDX, IDC_DROPLIST_TXT_CLR, m_cpText2);
	DDX_Control(pDX, IDC_DROPLIST_BAK_CLR, m_cpBack2);
	DDX_Control(pDX, IDC_DROPDOWN_TXT_CLR, m_cpText1);
	DDX_Control(pDX, IDC_DROPDOWN_BAK_CLR, m_cpBack1);
	DDX_Control(pDX, IDC_DROPDOWN_COMBO, m_wndFlatCombo1);
	DDX_Control(pDX, IDC_DROPLIST_COMBO, m_wndFlatCombo2);
	DDX_Check(pDX, IDC_DROPDOWN_AUTOCOMP, m_bAutoComplete);
	DDX_Check(pDX, IDC_DROPDOWN_DISABLE, m_bDisable1);
	DDX_Check(pDX, IDC_DROPLIST_DISABLE, m_bDisable2);
	DDX_Radio(pDX, IDC_RADIO_THEME, m_nTheme);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFlatComboDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CFlatComboDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DROPLIST_DISABLE, OnDroplistDisable)
	ON_BN_CLICKED(IDC_DROPDOWN_DISABLE, OnDropdownDisable)
	ON_BN_CLICKED(IDC_DROPDOWN_AUTOCOMP, OnDropdownAutocomp)
	ON_CPN_XT_SELENDOK(IDC_DROPLIST_TXT_CLR, OnSelEndOKDLTxtClr)
	ON_CPN_XT_SELENDOK(IDC_DROPLIST_BAK_CLR, OnSelEndOKDLBakClr)
	ON_CPN_XT_SELENDOK(IDC_DROPDOWN_TXT_CLR, OnSelEndOKDDTxtClr)
	ON_CPN_XT_SELENDOK(IDC_DROPDOWN_BAK_CLR, OnSelEndOKDDBakClr)
	ON_BN_CLICKED(IDC_RADIO_THEME, OnThemeChanged)
	ON_BN_CLICKED(IDC_RADIO_THEME_FLAT, OnThemeChanged)
	ON_BN_CLICKED(IDC_RADIO_THEME_OFFICEXP, OnThemeChanged)
	ON_BN_CLICKED(IDC_RADIO_THEME_OFFICE2003, OnThemeChanged)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlatComboDlg message handlers

BOOL CFlatComboDlg::OnInitDialog()
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
	SetResize(IDC_GBOX_LIST, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_DROPLIST_COMBO, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_GBOX_DOWN, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_DROPDOWN_COMBO, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_GBOX_EDIT, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_GBOX_APPEAR, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDOK, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDCANCEL, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	// prevent vertical resizing.
	SetFlag(xtResizeNoVertical);

	// Load window placement
	LoadPlacement(_T("CFlatComboDlg2"));

	// change the font for the dialog its children...
	XTFuncChangeWindowFont (this, &XTAuxData().font);

	// set each combo's index.
	m_wndFlatCombo1.SetCurSel(0);
	m_wndFlatCombo2.SetCurSel(1);

	// enable auto completion based on boolean.
	m_wndFlatCombo1.EnableAutoCompletion( m_bAutoComplete );

	// set the text color controls based on current combo color.
	m_cpText2.SetColor( m_wndFlatCombo2.m_clrText );
	m_cpBack2.SetColor( m_wndFlatCombo2.m_clrBack );
	m_cpText1.SetColor( m_wndFlatCombo1.m_clrText );
	m_cpBack1.SetColor( m_wndFlatCombo1.m_clrBack );

	OnThemeChanged();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFlatComboDlg::OnDestroy()
{
	// Save window placement
	SavePlacement(_T("CFlatComboDlg2"));

	CXTResizeDialog::OnDestroy();
}

void CFlatComboDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFlatComboDlg::OnPaint()
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
HCURSOR CFlatComboDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFlatComboDlg::OnSelEndOKDLTxtClr()
{
	m_wndFlatCombo2.m_clrText = m_cpText2.GetColor();
	m_wndFlatCombo2.Invalidate();
}

void CFlatComboDlg::OnSelEndOKDLBakClr()
{
	m_wndFlatCombo2.m_clrBack = m_cpBack2.GetColor();
	m_wndFlatCombo2.Invalidate();
}

void CFlatComboDlg::OnSelEndOKDDTxtClr()
{
	m_wndFlatCombo1.m_clrText = m_cpText1.GetColor();
	m_wndFlatCombo1.Invalidate();
}

void CFlatComboDlg::OnSelEndOKDDBakClr()
{
	m_wndFlatCombo1.m_clrBack = m_cpBack1.GetColor();
	m_wndFlatCombo1.Invalidate();
}

void CFlatComboDlg::OnDroplistDisable()
{
	UpdateData();
	m_wndFlatCombo2.EnableWindow( !m_bDisable2 );
}

void CFlatComboDlg::OnDropdownDisable()
{
	UpdateData();
	m_wndFlatCombo1.EnableWindow( !m_bDisable1 );
}

void CFlatComboDlg::OnDropdownAutocomp()
{
	UpdateData();
	m_wndFlatCombo1.EnableAutoCompletion( m_bAutoComplete );
}

HBRUSH CFlatComboDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXTResizeDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// set the combo boxes text and background colors...
	int nItem = pWnd->GetDlgCtrlID();
	switch (nItem)
	{
		case IDC_DROPLIST_COMBO:
		case IDC_DROPDOWN_COMBO:
			if( pWnd->IsWindowEnabled( ))
			{
				pDC->SetBkColor  (((CFlatComboBox*)pWnd)->m_clrBack);
				pDC->SetTextColor(((CFlatComboBox*)pWnd)->m_clrText);
			}
			break;
	}

	return hbr;
}

void CFlatComboDlg::OnThemeChanged()
{
	UpdateData(TRUE);

	m_wndFlatCombo1.DisableFlatLook(m_nTheme == 0);
	m_wndFlatCombo2.DisableFlatLook(m_nTheme == 0);
	m_wndFlatEdit.DisableFlatLook(m_nTheme == 0);

	m_btnOk.SetXButtonStyle(m_nTheme == 0? BS_XT_WINXP_COMPAT: m_nTheme == 1? BS_XT_FLAT: 0);
	m_btnCancel.SetXButtonStyle(m_nTheme == 0? BS_XT_WINXP_COMPAT: m_nTheme == 1? BS_XT_FLAT: 0);

	switch (m_nTheme)
	{
		case 0:
		case 1:
			XTThemeManager()->SetTheme(xtThemeDefault);
			break;
		case 2:
			XTThemeManager()->SetTheme(xtThemeOfficeXP);
			CXTButton::SetDefaultTheme(new CXTButtonThemeOfficeXP(TRUE));
			break;
		case 3:
			XTThemeManager()->SetTheme(xtThemeOffice2003);
			CXTButton::SetDefaultTheme(new CXTButtonThemeOffice2003(TRUE));
			break;

	}

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
}
