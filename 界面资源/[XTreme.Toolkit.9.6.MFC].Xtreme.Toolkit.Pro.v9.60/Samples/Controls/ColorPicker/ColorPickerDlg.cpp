// ColorPickerDlg.cpp : implementation file
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
#include "ColorPicker.h"
#include "ColorPickerDlg.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorPickerDlg dialog

CColorPickerDlg::CColorPickerDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CColorPickerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorPickerDlg)
	m_bExtended = TRUE;
	m_bMoreColors = TRUE;
	m_bNoFill = FALSE;
	m_3DBorder = FALSE;
	m_bDisable = FALSE;
	m_bDisplayHex = FALSE;
	m_bColorText = FALSE;
	m_bShowText = FALSE;
	//}}AFX_DATA_INIT

	m_clrText = RGB(0xff,0xff,0xff);
	m_clrBack = RGB(0x00,0x00,0xff);

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CColorPickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorPickerDlg)
	DDX_Control(pDX, IDC_GBOX_PICKER, m_gboxPicker);
	DDX_Control(pDX, IDC_GBOX_DIALOG, m_gboxDialog);
	DDX_Control(pDX, IDC_COMBO_DEMO, m_comboBox);
	DDX_Control(pDX, IDC_CLR_TEXT, m_cpTextColor);
	DDX_Control(pDX, IDC_CLR_BACK, m_cpBackColor);
	DDX_Check(pDX, IDC_CHK_EXTENDED, m_bExtended);
	DDX_Check(pDX, IDC_CHK_MORECOLORS, m_bMoreColors);
	DDX_Check(pDX, IDC_CHK_NOFILL, m_bNoFill);
	DDX_Check(pDX, IDC_CHK_3DBORDER, m_3DBorder);
	DDX_Check(pDX, IDC_CHK_DISABLE, m_bDisable);
	DDX_Check(pDX, IDC_CHK_DISPLAY_HEX, m_bDisplayHex);
	DDX_Check(pDX, IDC_CHK_COLORTEXT, m_bColorText);
	DDX_Check(pDX, IDC_CHK_NOTEXT, m_bShowText);
	//}}AFX_DATA_MAP
	DDX_XTColorPicker(pDX, IDC_CLR_TEXT, m_clrText);
	DDX_XTColorPicker(pDX, IDC_CLR_BACK, m_clrBack);
}

BEGIN_MESSAGE_MAP(CColorPickerDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CColorPickerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_CPN_XT_SELENDOK(IDC_CLR_TEXT, OnSelEndOkTextClr)
	ON_CPN_XT_SELENDOK(IDC_CLR_BACK, OnSelEndOkBackClr)
	ON_BN_CLICKED(IDC_CHK_NOFILL, OnChkNofill)
	ON_BN_CLICKED(IDC_CHK_EXTENDED, OnChkExtended)
	ON_BN_CLICKED(IDC_CHK_MORECOLORS, OnChkMorecolors)
	ON_CBN_SETFOCUS(IDC_COMBO_DEMO, OnSetfocusComboDemo)
	ON_BN_CLICKED(IDC_CHK_3DBORDER, OnChk3dborder)
	ON_BN_CLICKED(IDC_CHK_DISPLAY_HEX, OnChkDisplayHex)
	ON_BN_CLICKED(IDC_CHK_DISABLE, OnChkDisable)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHK_NOTEXT, OnChkNoText)
	ON_BN_CLICKED(IDC_CHK_COLORTEXT, OnChkColorText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorPickerDlg message handlers

BOOL CColorPickerDlg::OnInitDialog()
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
	SetResize(IDC_COMBO_DEMO,  SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDCANCEL,        SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_GBOX_BUTTON, SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_GBOX_POPUP,  SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_GBOX_DIALOG, SZ_TOP_LEFT,  SZ_TOP_RIGHT);

	// prevent vertical resizing.
	SetFlag(xtResizeNoVertical);

	// Load window placement
	LoadPlacement(_T("CColorPickerDlg"));

	//------------------------------------
	// TODO: Add extra initialization here
	//------------------------------------

	m_comboBox.SetCurSel(0);

	m_cpBackColor.SetColor(m_clrBack);
	m_cpBackColor.SetDefaultColor(GetSysColor(COLOR_WINDOW));
	m_cpBackColor.ShowText(m_bShowText);

	m_cpTextColor.SetColor(m_clrText);
	m_cpTextColor.SetDefaultColor(GetSysColor(COLOR_WINDOWTEXT));
	m_cpTextColor.ShowText(m_bShowText);

	// Add some user defined color values.
	CUIntArray arUserColors;
	arUserColors.Add(RGB(150,0,0));
	arUserColors.Add(RGB(0,150,0));
	arUserColors.Add(RGB(0,0,150));
	CXTColorSelectorCtrl::SetUserColors(arUserColors);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CColorPickerDlg::OnDestroy()
{
	// Save window placement
	SavePlacement(_T("CColorPickerDlg"));

	CXTResizeDialog::OnDestroy();
}

void CColorPickerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CColorPickerDlg::OnPaint()
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
HCURSOR CColorPickerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

HBRUSH CColorPickerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXTResizeDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	int nItem = pWnd->GetDlgCtrlID();
	switch (nItem)
	{
		case IDC_COMBO_DEMO:
			if( pWnd->IsWindowEnabled( ))
			{
				pDC->SetTextColor(m_clrText);
				pDC->SetBkColor(m_clrBack);
			}
			break;
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CColorPickerDlg::OnSelEndOkTextClr()
{
	UpdateData();
	m_comboBox.Invalidate();
}

void CColorPickerDlg::OnSelEndOkBackClr()
{
	UpdateData();
	m_comboBox.Invalidate();
}


void CColorPickerDlg::OnChkNofill()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	DWORD dwRem = m_bNoFill ? 0 : CPS_XT_NOFILL;
	DWORD dwAdd = m_bNoFill ? CPS_XT_NOFILL : 0;

	m_cpBackColor.ModifyCPStyle(dwRem, dwAdd);
	m_cpTextColor.ModifyCPStyle(dwRem, dwAdd);
}

void CColorPickerDlg::OnChkExtended()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	DWORD dwRem = m_bExtended ? 0 : CPS_XT_EXTENDED;
	DWORD dwAdd = m_bExtended ? CPS_XT_EXTENDED : 0;

	m_cpBackColor.ModifyCPStyle(dwRem, dwAdd);
	m_cpTextColor.ModifyCPStyle(dwRem, dwAdd);
}

void CColorPickerDlg::OnChkMorecolors()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	DWORD dwRem = m_bMoreColors ? 0 : CPS_XT_MORECOLORS;
	DWORD dwAdd = m_bMoreColors ? CPS_XT_MORECOLORS : 0;

	m_cpBackColor.ModifyCPStyle(dwRem, dwAdd);
	m_cpTextColor.ModifyCPStyle(dwRem, dwAdd);
}

void CColorPickerDlg::OnChk3dborder()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	DWORD dwRem = m_3DBorder ? 0 : CPS_XT_SHOW3DSELECTION;
	DWORD dwAdd = m_3DBorder ? CPS_XT_SHOW3DSELECTION : 0;

	m_cpBackColor.ModifyCPStyle(dwRem, dwAdd);
	m_cpTextColor.ModifyCPStyle(dwRem, dwAdd);
}

void CColorPickerDlg::OnChkDisplayHex()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	DWORD dwRem = m_bDisplayHex ? 0 : CPS_XT_SHOWHEXVALUE;
	DWORD dwAdd = m_bDisplayHex ? CPS_XT_SHOWHEXVALUE : 0;

	m_cpBackColor.ModifyCPStyle(dwRem, dwAdd);
	m_cpTextColor.ModifyCPStyle(dwRem, dwAdd);
}

void CColorPickerDlg::OnChkDisable()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_cpBackColor.EnableWindow(!m_bDisable);
	m_cpTextColor.EnableWindow(!m_bDisable);
}

void CColorPickerDlg::OnSetfocusComboDemo()
{
	// don't allow interaction with combo box.
	m_cpTextColor.SetFocus();
}

void CColorPickerDlg::OnChkNoText()
{
	UpdateData();
	m_cpBackColor.ShowText(m_bShowText);
	m_cpTextColor.ShowText(m_bShowText);
}

void CColorPickerDlg::OnChkColorText()
{
	UpdateData();
	m_cpBackColor.ShowTextInColor(m_bColorText);
	m_cpTextColor.ShowTextInColor(m_bColorText);
}
