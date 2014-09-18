// PopupControlDemoDlg.cpp : implementation file
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
#include "PopupControlDemo.h"
#include "PopupControlDemoDlg.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_POPUP_CLOSE (-1)
#define ID_GOTO_SITE (-2)


/////////////////////////////////////////////////////////////////////////////
// CPopupControlDemoDlg dialog

CPopupControlDemoDlg::CPopupControlDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPopupControlDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPopupControlDemoDlg)
	m_nTheme = 2;
	m_nAnimation = 1;
	m_uAnimDelay = 500;
	m_uShowDelay = 3000;
	m_bAllowMove = TRUE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPopupControlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPopupControlDemoDlg)
	DDX_Control(pDX, IDC_SLIDER_TRANSPARENT, m_Slider);
	DDX_CBIndex(pDX, IDC_COMBO_THEME, m_nTheme);
	DDX_CBIndex(pDX, IDC_COMBO_ANIMATION, m_nAnimation);
	DDX_Text(pDX, IDC_EDIT_ANIMATIONDELAY, m_uAnimDelay);
	DDV_MinMaxUInt(pDX, m_uAnimDelay, 0, 60000);
	DDX_Text(pDX, IDC_EDIT_SHOWDELAY, m_uShowDelay);
	DDV_MinMaxUInt(pDX, m_uShowDelay, 0, 600000);
	DDX_Check(pDX, IDC_CHECK_ALLOWMOVE, m_bAllowMove);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPopupControlDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CPopupControlDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SHOW, OnButtonShow)
	ON_CBN_SELCHANGE(IDC_COMBO_THEME, OnSelchangeComboTheme)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_TRANSPARENT, OnReleasedcaptureSliderTransparent)
	ON_CBN_SELCHANGE(IDC_COMBO_ANIMATION, OnSelchangeComboAnimation)
	ON_EN_CHANGE(IDC_EDIT_ANIMATIONDELAY, OnChangeEditAnimationdelay)
	ON_EN_CHANGE(IDC_EDIT_SHOWDELAY, OnChangeEditShowdelay)
	ON_BN_CLICKED(IDC_CHECK_ALLOWMOVE, OnCheckAllowMove)
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTPWM_POPUPCONTROL_NOTIFY, OnPopUpNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopupControlDemoDlg message handlers

BOOL CPopupControlDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	//
	m_Slider.SetRange(0, 255, TRUE);
	m_Slider.SetPos(200);
	m_wndPopupControl.SetTransparency((BYTE)m_Slider.GetPos());

	OnSelchangeComboTheme();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPopupControlDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPopupControlDemoDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPopupControlDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPopupControlDemoDlg::OnButtonShow()
{
	if (m_wndPopupControl.GetSafeHwnd())
		return;

	m_wndPopupControl.GetPaintManager()->RefreshMetrics();
	UpdateData();

	CSize szPopup (170, 130);

	if(m_nTheme == xtpPopupThemeOffice2003)
		szPopup = CSize(329, 74);
	if(m_nTheme == 8)
		szPopup = CSize(220, 90);

	m_wndPopupControl.SetPopupAnimation((XTPPopupAnimation)m_nAnimation);
	m_wndPopupControl.SetShowDelay(m_uShowDelay);
	m_wndPopupControl.SetAnimateDelay(m_uAnimDelay);
	m_wndPopupControl.SetTransparency((BYTE)m_Slider.GetPos());
	m_wndPopupControl.SetPopupSize(szPopup);
	m_wndPopupControl.AllowMove(m_bAllowMove);
	m_wndPopupControl.Show(this);

	EnableItems(FALSE);
}

void CPopupControlDemoDlg::EnableItems(BOOL bEnable)
{
	CWnd* pWnd = GetWindow(GW_CHILD);
	while (pWnd)
	{
		pWnd->EnableWindow(bEnable);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
}

void CPopupControlDemoDlg::SetDefaults()
{
	switch (m_nTheme)
	{
		case xtpPopupThemeOffice2000:
		case xtpPopupThemeOfficeXP:
			{
				m_nAnimation = 1;
				m_uAnimDelay = 500;
				m_uShowDelay = 3000;
				m_Slider.SetPos(255);
			}
			break;
		case xtpPopupThemeOffice2003:
			{
				m_nAnimation = 1;
				m_uAnimDelay = 500;
				m_uShowDelay = 3000;
				m_Slider.SetPos(200);
			}
			break;
		case xtpPopupThemeMSN:
		case 4:
		case 5:
		case 6:
		case 7:
			{
				m_nAnimation = 2;
				m_uAnimDelay = 350;
				m_uShowDelay = 3000;
				m_Slider.SetPos(255);
			}
			break;
		case 8:
			{
				m_nAnimation = 3;
				m_uAnimDelay = 500;
				m_uShowDelay = 3000;
				m_Slider.SetPos(255);
			}
			break;
	}

	UpdateData(FALSE);

	m_wndPopupControl.SetTransparency(
		(BYTE)m_Slider.GetPos());
}
void CPopupControlDemoDlg::SetOffice2000Theme()
{
	m_wndPopupControl.RemoveAllItems();

	CXTPPopupItem* pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(25, 5, 170, 19), _T("Word 2000 Theme")));
	pItemText->SetTextAlignment(DT_LEFT);
	pItemText->CalculateHeight();
	pItemText->CalculateWidth();
	pItemText->SetHyperLink(FALSE);
	pItemText->SetTextColor(RGB(255,255,255));

	CXTPPopupItem * pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(4, 4, 20, 19)));
	pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_WORD2000), IMAGE_ICON, 16, 16, 0),xtpPopupItemIconNormal);

	pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(14, 40, 20, 49)));
	pItemIcon->SetIcons(IDB_LOGOOFFICE, 0, xtpPopupItemIconNormal);

	pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(109, 55, 132, 78)));
	pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_OPEN), IMAGE_ICON, 16, 16, 0),xtpPopupItemIconNormal);
	pItemIcon->CenterIcon();
	pItemIcon->SetButton(TRUE);

	pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(133, 55, 156, 78)));
	pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_SAVE), IMAGE_ICON, 16, 16, 0),xtpPopupItemIconNormal);
	pItemIcon->CenterIcon();
	pItemIcon->SetButton(TRUE);

	pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(151, 6, 164, 19)));
	pItemIcon->SetIcons(IDB_CLOSEOFFICE, 0,
		xtpPopupItemIconNormal|xtpPopupItemIconSelected|xtpPopupItemIconPressed);
	pItemIcon->SetID(ID_POPUP_CLOSE);

	pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(104, 30, 170, 45), _T("Parameters")));

	pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(65, 90, 105, 110), _T("OK")));
	pItemText->SetButton(TRUE);
	pItemText->SetTextAlignment(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pItemText->SetHyperLink(FALSE);
	pItemText->SetID(IDOK);

	m_wndPopupControl.SetTheme(xtpPopupThemeOffice2000);
	m_wndPopupControl.RedrawControl();
}

void CPopupControlDemoDlg::SetOfficeXPTheme()
{
	m_wndPopupControl.RemoveAllItems();

	CXTPPopupItem* pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(25, 5, 170, 19), _T("Word XP Theme")));
	pItemText->SetTextAlignment(DT_LEFT);
	pItemText->CalculateHeight();
	pItemText->CalculateWidth();
	pItemText->SetHyperLink(FALSE);
	pItemText->SetTextColor(RGB(255,255,255));

	CXTPPopupItem * pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(4, 4, 20, 19)));
	pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_WORD2000), IMAGE_ICON, 16, 16, 0),xtpPopupItemIconNormal);

	pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(14, 40, 20, 49)));
	pItemIcon->SetIcons(IDB_LOGOOFFICE, 0, xtpPopupItemIconNormal);

	pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(109, 55, 132, 78)));
	pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_OPEN), IMAGE_ICON, 16, 16, 0),xtpPopupItemIconNormal);
	pItemIcon->CenterIcon();
	pItemIcon->SetButton(TRUE);

	pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(133, 55, 156, 78)));
	pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_SAVE), IMAGE_ICON, 16, 16, 0),xtpPopupItemIconNormal);
	pItemIcon->CenterIcon();
	pItemIcon->SetButton(TRUE);

	pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(151, 6, 164, 19)));
	pItemIcon->SetIcons(IDB_CLOSEOFFICE, 0,
		xtpPopupItemIconNormal|xtpPopupItemIconSelected|xtpPopupItemIconPressed);
	pItemIcon->SetID(ID_POPUP_CLOSE);

	pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(104, 30, 170, 45), _T("Parameters")));

	pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(65, 90, 105, 110), _T("OK")));
	pItemText->SetButton(TRUE);
	pItemText->SetTextAlignment(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pItemText->SetHyperLink(FALSE);
	pItemText->SetID(IDOK);

	m_wndPopupControl.SetTheme(xtpPopupThemeOfficeXP);
	m_wndPopupControl.RedrawControl();

}

void CPopupControlDemoDlg::SetMSNTheme()
{
	m_wndPopupControl.RemoveAllItems();

	CXTPPopupItem* pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(25, 6, 170, 19), _T("PopupControlDemo")));
	pItemText->SetTextAlignment(DT_LEFT);
	pItemText->CalculateHeight();
	pItemText->CalculateWidth();
	pItemText->SetHyperLink(FALSE);

	pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(104, 27, 160, 45), _T("Options")));
	pItemText->SetTextAlignment(DT_RIGHT);
	pItemText->CalculateHeight();
	pItemText->CalculateWidth();

	pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(0, 50, 170, 100), _T("Please visit our website for additional product information including registration details.")));
	pItemText->SetTextAlignment(DT_CENTER|DT_WORDBREAK);
	pItemText->CalculateHeight();
	pItemText->SetID(ID_GOTO_SITE);

	CXTPPopupItem* pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(151, 6, 164, 19)));
	pItemIcon->SetIcons(IDB_CLOSEMSN, 0, xtpPopupItemIconNormal|xtpPopupItemIconSelected|xtpPopupItemIconPressed);
	pItemIcon->SetID(ID_POPUP_CLOSE);

	pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(7, 6, 20, 19)));
	pItemIcon->SetIcons(IDB_CAPTIONMSN, 0, xtpPopupItemIconNormal);

	pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(115, 102, 160, 120)));
	pItemIcon->SetIcons(IDB_LOGOMSN, 0, xtpPopupItemIconNormal);

	m_wndPopupControl.SetTheme(xtpPopupThemeMSN);
	m_wndPopupControl.RedrawControl();
}

void CPopupControlDemoDlg::SetOffice2003Theme()
{
	m_wndPopupControl.RemoveAllItems();

	// add text items.
	CXTPPopupItem* pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(46, 16, 270, 41), _T("Peter Parker")));
	pItemText->SetBold(TRUE);

	pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(46, 29, 270, 54), _T("RE: Week reports 12/04/2004 - 17/04/2004")));
	pItemText->SetHyperLink(FALSE);

	pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(46, 42, 270, 92), _T("See comments below.\nThanks.")));
	pItemText->SetHyperLink(FALSE);

	pItemText->SetTextColor(RGB(0, 61, 178));
	pItemText->SetTextAlignment(DT_LEFT|DT_WORDBREAK);

	// letter icon.
	CXTPPopupItem* pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(5, 12, 17, 24)));
	pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_LETTER), IMAGE_ICON, 32, 32, 0));
	pItemIcon->SetButton(FALSE);

	// close icon.
	pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(308, 14, 324, 30)));
	pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_CLOSE), IMAGE_ICON, 16, 16, 0));
	pItemIcon->SetButton(TRUE);
	pItemIcon->SetID(ID_POPUP_CLOSE);

	// flag icon.
	pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(4, 49, 20, 65)));
	pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_FLAG), IMAGE_ICON, 16, 16, 0));
	pItemIcon->SetButton(TRUE);

	// cross icon.
	pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(23, 49, 39, 65)));
	pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_CROSS), IMAGE_ICON, 16, 16, 0));
	pItemIcon->SetButton(TRUE);

	m_wndPopupControl.SetTheme(xtpPopupThemeOffice2003);
	m_wndPopupControl.RedrawControl();
}

void CPopupControlDemoDlg::SetBlueTheme()
{
	m_wndPopupControl.RemoveAllItems();

	m_wndPopupControl.AddItem(new CXTPPopupItem(
		CRect(0, 0, 170, 130), NULL,  RGB(50,50,255), RGB(255,255,255)));
	m_wndPopupControl.AddItem(new CXTPPopupItem(
		CRect(5, 25, 170 - 5, 130 - 5), NULL, RGB(70,70,250), RGB(200,200,200)));

	CXTPPopupItem* pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(104, 27, 170, 45), _T("more...")));

	pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(0, 70, 170, 100), _T("Please visit our website for additional product information including registration details.")));
	pItemText->SetTextAlignment(DT_CENTER|DT_WORDBREAK);
	pItemText->SetTextColor(RGB(255,255,255));
	pItemText->CalculateHeight();
	pItemText->SetID(ID_GOTO_SITE);

	CXTPPopupItem* pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(12, 30, 12, 47)));

	pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_LETTER2), IMAGE_ICON, 32, 32, 0));

	pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(5, 0, 170, 25), _T("Message for You")));
	pItemText->SetTextAlignment(DT_LEFT|DT_SINGLELINE| DT_VCENTER);
	pItemText->SetTextColor(RGB(255, 255, 255));
	pItemText->SetBold(TRUE);
	pItemText->SetHyperLink(FALSE);

	pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(151, 6, 164, 19)));
	pItemIcon->SetIcons(IDB_CLOSEMSN, 0, xtpPopupItemIconNormal|xtpPopupItemIconSelected|xtpPopupItemIconPressed);
	pItemIcon->SetID(ID_POPUP_CLOSE);

	m_wndPopupControl.SetTheme(new CXTPPopupPaintManager());
	m_wndPopupControl.RedrawControl();
}

void CPopupControlDemoDlg::SetBlackTheme()
{
	m_wndPopupControl.RemoveAllItems();

	m_wndPopupControl.AddItem(new CXTPPopupItem(
		CRect(0, 0, 170, 130), NULL, RGB(10,10,10), RGB(255,255,255)));
	m_wndPopupControl.AddItem(new CXTPPopupItem(
		CRect(5, 25, 170 - 5, 130 - 5), NULL, RGB(70,70,70), RGB(200,200,200)));

	CXTPPopupItem* pItemText = (CXTPPopupItem*)
		m_wndPopupControl.AddItem(new CXTPPopupItem(CRect(104, 27, 170, 45),
		_T("more...")));
	pItemText->SetTextColor(RGB(150,150,150));

	pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(0, 70, 170, 100), _T("Please visit our website for additional product information including registration details.")));
	pItemText->SetTextAlignment(DT_CENTER|DT_WORDBREAK);
	pItemText->SetTextColor(RGB(255,255,255));
	pItemText->CalculateHeight();
	pItemText->SetID(ID_GOTO_SITE);

	CXTPPopupItem* pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(12, 30, 12, 47)));

	pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_LETTER2), IMAGE_ICON, 32, 32, 0));

	pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(5, 0, 170, 25), _T("Message for You")));
	pItemText->SetTextAlignment(DT_LEFT|DT_SINGLELINE| DT_VCENTER);
	pItemText->SetTextColor(RGB(255, 255, 255));
	pItemText->SetBold(TRUE);
	pItemText->SetHyperLink(FALSE);

	pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(151, 6, 164, 19)));
	pItemIcon->SetIcons(IDB_CLOSEMSN, 0, xtpPopupItemIconNormal|xtpPopupItemIconSelected|xtpPopupItemIconPressed);
	pItemIcon->SetID(ID_POPUP_CLOSE);

	m_wndPopupControl.SetTheme(new CXTPPopupPaintManager());
	m_wndPopupControl.RedrawControl();
}

void CPopupControlDemoDlg::SetGreenTheme()
{
	m_wndPopupControl.RemoveAllItems();

	m_wndPopupControl.AddItem(new CXTPPopupItem(
		CRect(0, 0, 170, 130), NULL, RGB(30,120,30), RGB(255,255,0)));
	m_wndPopupControl.AddItem(new CXTPPopupItem(
		CRect(5, 25, 170 - 5, 130 - 5), NULL, RGB(70,130,70), RGB(255,255,0)));

	CXTPPopupItem* pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(104, 27, 170, 45), _T("more...")));

	pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(0, 70, 170, 100), _T("Please visit our website for additional product information including registration details.")));
	pItemText->SetTextAlignment(DT_CENTER|DT_WORDBREAK);
	pItemText->SetTextColor(RGB(255,255,0));
	pItemText->CalculateHeight();
	pItemText->SetID(ID_GOTO_SITE);

	CXTPPopupItem* pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(12, 30, 12, 47)));

	pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_LETTER2), IMAGE_ICON, 32, 32, 0));

	pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(5, 0, 170, 25), _T("Message for You")));
	pItemText->SetTextAlignment(DT_LEFT|DT_SINGLELINE| DT_VCENTER);
	pItemText->SetTextColor(RGB(255, 255, 255));
	pItemText->SetBold(TRUE);
	pItemText->SetHyperLink(FALSE);

	pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(151, 6, 164, 19)));
	pItemIcon->SetIcons(IDB_CLOSEMSN, 0, xtpPopupItemIconNormal|xtpPopupItemIconSelected|xtpPopupItemIconPressed);
	pItemIcon->SetID(ID_POPUP_CLOSE);

	m_wndPopupControl.SetTheme(new CXTPPopupPaintManager());
	m_wndPopupControl.RedrawControl();
}

void CPopupControlDemoDlg::SetRedTheme()
{
	m_wndPopupControl.RemoveAllItems();

	m_wndPopupControl.AddItem(new CXTPPopupItem(
		CRect(0, 0, 170, 130), NULL, RGB(255,50,50), RGB(255,255,0)));
	m_wndPopupControl.AddItem(new CXTPPopupItem(
		CRect(5, 25, 170 - 5, 130 - 5), NULL, RGB(230,70,70), RGB(255,255,0)));

	CXTPPopupItem* pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(104, 27, 170, 45), _T("more...")));

	pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(0, 70, 170, 100), _T("Please visit our website for additional product information including registration details.")));
	pItemText->SetTextAlignment(DT_CENTER|DT_WORDBREAK);
	pItemText->SetTextColor(RGB(255,255,0));
	pItemText->CalculateHeight();
	pItemText->SetID(ID_GOTO_SITE);

	CXTPPopupItem* pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(12, 30, 12, 47)));

	pItemIcon->SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_LETTER2), IMAGE_ICON, 32, 32, 0));

	pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(5, 0, 170, 25), _T("Message for You")));
	pItemText->SetTextAlignment(DT_LEFT|DT_SINGLELINE| DT_VCENTER);
	pItemText->SetTextColor(RGB(255, 255, 255));
	pItemText->SetBold(TRUE);
	pItemText->SetHyperLink(FALSE);

	pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(151, 6, 164, 19)));
	pItemIcon->SetIcons(IDB_CLOSEMSN, 0, xtpPopupItemIconNormal|xtpPopupItemIconSelected|xtpPopupItemIconPressed);
	pItemIcon->SetID(ID_POPUP_CLOSE);

	m_wndPopupControl.SetTheme(new CXTPPopupPaintManager());
	m_wndPopupControl.RedrawControl();
}

void CPopupControlDemoDlg::SetTooltipTheme()
{
	m_wndPopupControl.RemoveAllItems();

	m_wndPopupControl.AddItem(new CXTPPopupItem(
		CRect(0, 0, 220, 90), NULL, RGB(255,255,225), 0));

	CXTPPopupItem* pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(20, 30, 200, 100), _T("Please visit our website for additional product information including registration details.")));
	pItemText->SetTextAlignment(DT_CENTER|DT_WORDBREAK);
	pItemText->SetID(ID_GOTO_SITE);

	pItemText = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(5, 0, 170, 25), _T("Codejock Information")));
	pItemText->SetTextAlignment(DT_LEFT|DT_SINGLELINE| DT_VCENTER);
	pItemText->SetBold(TRUE);
	pItemText->SetHyperLink(FALSE);

	CXTPPopupItem* pItemIcon = (CXTPPopupItem*)m_wndPopupControl.AddItem(
		new CXTPPopupItem(CRect(220 - 20, 2, 220 - 2, 2 + 18)));
	pItemIcon->SetIcons(IDB_CLOSETOOLTIP, 0, xtpPopupItemIconNormal|xtpPopupItemIconSelected|xtpPopupItemIconPressed);
	pItemIcon->SetID(ID_POPUP_CLOSE);

	m_wndPopupControl.SetTheme(new CXTPPopupPaintManager());
	m_wndPopupControl.RedrawControl();
}

void CPopupControlDemoDlg::SetTheme(int nTheme)
{
	switch (nTheme)
	{
	case xtpPopupThemeOffice2000:
		SetOffice2000Theme();
		break;
	case xtpPopupThemeOfficeXP:
		SetOfficeXPTheme();
		break;
	case xtpPopupThemeOffice2003:
		SetOffice2003Theme();
		break;
	case xtpPopupThemeMSN:
		SetMSNTheme();
		break;
	case 4:
		SetBlueTheme();
		break;
	case 5:
		SetBlackTheme();
		break;
	case 6:
		SetGreenTheme();
		break;
	case 7:
		SetRedTheme();
		break;
	case 8:
		SetTooltipTheme();
		break;
	}
}

void CPopupControlDemoDlg::OnCheckAllowMove()
{
	UpdateData();
}

void CPopupControlDemoDlg::OnSelchangeComboTheme()
{
	UpdateData();

	SetTheme(m_nTheme);

	SetDefaults();
}

void CPopupControlDemoDlg::OnReleasedcaptureSliderTransparent(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	m_wndPopupControl.SetTransparency((BYTE)m_Slider.GetPos());
	*pResult = 0;
}

void CPopupControlDemoDlg::OnSelchangeComboAnimation()
{
	UpdateData();
	m_wndPopupControl.SetPopupAnimation((XTPPopupAnimation)m_nAnimation);
}

void CPopupControlDemoDlg::OnChangeEditAnimationdelay()
{
	UpdateData();
	m_wndPopupControl.SetAnimateDelay(m_uAnimDelay);
}

void CPopupControlDemoDlg::OnChangeEditShowdelay()
{
	UpdateData();
	m_wndPopupControl.SetShowDelay(m_uShowDelay);
}

LRESULT CPopupControlDemoDlg::OnPopUpNotify(WPARAM wParam, LPARAM lParam)
{
	// process notify from popup wnd

	if(wParam == XTP_PCN_ITEMCLICK)
	{
		CXTPPopupItem* pItem = (CXTPPopupItem*)lParam;
		ASSERT(pItem);

		m_wndPopupControl.Close();

		if(pItem->GetID() == ID_GOTO_SITE)
		{
			::ShellExecute(NULL, _T("open"), _T("http://www.codejock.com/"), NULL, NULL, SW_SHOW);
		}
	}
	else if(wParam == XTP_PCN_STATECHANGED)
	{
		CXTPPopupControl* pCntrol = (CXTPPopupControl*)lParam;
		ASSERT(pCntrol);

		if(pCntrol->GetPopupState() == xtpPopupStateClosed)
		{
			EnableItems(TRUE);
		}
	}
	return TRUE;
}
