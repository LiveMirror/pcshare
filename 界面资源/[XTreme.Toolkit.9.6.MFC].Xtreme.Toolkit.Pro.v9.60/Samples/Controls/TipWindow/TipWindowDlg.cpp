// TipWindowDlg.cpp : implementation file
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
#include "TipWindow.h"
#include "TipWindowDlg.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTipWindowDlg dialog

CTipWindowDlg::CTipWindowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTipWindowDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTipWindowDlg)
	m_strTipText = _T("This is where you would display your tip text message...\nThis is where you would display the second line of your tip text message.");
	m_strTipTitle = _T("Tip Message Title");
	m_nMillisecs = 5000;
	m_nDelayMillisecs = 0;
	m_cx = 1;
	m_cy = 1;
	m_nLineSpacing = 1;
	m_bThickBorder = FALSE;
	m_bAlphaShadow = TRUE;
	m_bDropShadow = TRUE;
	m_bMoveTipBelowCursor = FALSE;
	//}}AFX_DATA_INIT

	m_tipWindow.SetMargins(CSize(m_cx,m_cy));
	m_tipWindow.SetLineSpace(m_nLineSpacing);

	m_clrTipBack = ::GetSysColor(COLOR_INFOBK);
	m_clrTipText = ::GetSysColor(COLOR_INFOTEXT);

	UpdateStyle();

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTipWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTipWindowDlg)
	DDX_Control(pDX, IDC_LIST, m_cListCtrl);
	DDX_Control(pDX, IDC_EDIT_CY, m_editCY);
	DDX_Control(pDX, IDC_EDIT_CX, m_editCX);
	DDX_Control(pDX, IDC_SPIN_CY, m_spinCY);
	DDX_Control(pDX, IDC_SPIN_CX, m_spinCX);
	DDX_Control(pDX, IDC_CLR_TEXT, m_cpTipText);
	DDX_Control(pDX, IDC_CLR_BACK, m_cpTipBack);
	DDX_Control(pDX, IDC_BTN_SHOWTIP, m_btnShowTip);
	DDX_Text(pDX, IDC_EDIT_TIPTEXT, m_strTipText);
	DDX_Text(pDX, IDC_EDIT_TIPTITLE, m_strTipTitle);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT, m_nMillisecs);
	DDX_Text(pDX, IDC_EDIT_DELAY, m_nDelayMillisecs);
	DDX_Text(pDX, IDC_EDIT_CX, m_cx);
	DDX_Text(pDX, IDC_EDIT_CY, m_cy);
	DDX_Text(pDX, IDC_EDIT_LINESPACE, m_nLineSpacing);
	DDX_Check(pDX, IDC_CHK_THICKBORDER, m_bThickBorder);
	DDX_Check(pDX, IDC_CHK_ALPHASHADOWING, m_bAlphaShadow);
	DDX_Check(pDX, IDC_CHK_SHADOW, m_bDropShadow);
	DDX_Check(pDX, IDC_CHK_MOVEBELOWCURSOR, m_bMoveTipBelowCursor);
	//}}AFX_DATA_MAP
	DDX_XTColorPicker(pDX, IDC_CLR_TEXT, m_clrTipText);
	DDX_XTColorPicker(pDX, IDC_CLR_BACK, m_clrTipBack);
}

BEGIN_MESSAGE_MAP(CTipWindowDlg, CDialog)
	//{{AFX_MSG_MAP(CTipWindowDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SHOWTIP, OnBtnShowTip)
	ON_EN_CHANGE(IDC_EDIT_TIPTEXT, OnChangeEditTiptext)
	ON_EN_CHANGE(IDC_EDIT_TIPTITLE, OnChangeEditTiptitle)
	ON_BN_CLICKED(IDC_CHK_ALPHASHADOWING, OnChkAlphaShadowing)
	ON_BN_CLICKED(IDC_CHK_THICKBORDER, OnChkThickBorder)
	ON_BN_CLICKED(IDC_CHK_SHADOW, OnChkShadow)
	//}}AFX_MSG_MAP
	ON_CPN_XT_SELENDOK(IDC_CLR_TEXT, OnSelEndOkTextColor)
	ON_CPN_XT_SELENDOK(IDC_CLR_BACK, OnSelEndOkBackColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTipWindowDlg message handlers

BOOL CTipWindowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_tipWindow.Create(this);
	m_tipWindow.ShowWindow(SW_HIDE);

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

	// TODO: Add extra initialization here
	m_cpTipText.SetColor (m_clrTipText);
	m_cpTipText.SetDefaultColor (m_clrTipText);

	m_cpTipBack.SetColor (m_clrTipBack);
	m_cpTipBack.SetDefaultColor (m_clrTipBack);

	m_spinCY.SetBuddy(&m_editCY);
	m_spinCY.SetRange(0,15);

	m_spinCX.SetBuddy(&m_editCX);
	m_spinCX.SetRange(0,25);

	m_cListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES);

	m_cListCtrl.InsertColumn(0, _T("First name"), LVCFMT_LEFT, 50, 0);
	m_cListCtrl.InsertColumn(1, _T("Last name"), LVCFMT_LEFT, 50, 1);
	m_cListCtrl.InsertColumn(2, _T("Address"), LVCFMT_LEFT, 50, 2);
	m_cListCtrl.InsertColumn(3, _T("City"), LVCFMT_LEFT, 50, 3);

	m_cListCtrl.InsertItem(0, _T("Michael"));
	m_cListCtrl.SetItemText(0, 1, _T("abcdefghijklmnopqrstuvwxyz"));
	m_cListCtrl.SetItemText(0, 2, _T("Street 1234"));
	m_cListCtrl.SetItemText(0, 3, _T("Munich\n\n\n\n(Germany)"));

	m_cListCtrl.InsertItem(0, _T("Robert\nRaab\n123"));
	m_cListCtrl.SetItemText(0, 1, _T("Einstein\njunior"));
	m_cListCtrl.SetItemText(0, 2, _T("Road ABCDEFG"));
	m_cListCtrl.SetItemText(0, 3, _T("Miami"));

	m_cListCtrl.InsertItem(0, _T("Sandra"));
	m_cListCtrl.SetItemText(0, 1, _T("Test"));
	m_cListCtrl.SetItemText(0, 2, _T("1234567890\nabcdefghij"));
	m_cListCtrl.SetItemText(0, 3, _T("Nowhere city"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTipWindowDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTipWindowDlg::OnPaint()
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
HCURSOR CTipWindowDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTipWindowDlg::OnBtnShowTip()
{
	UpdateData();

	if (::IsWindow(m_tipWindow.m_hWnd))
	{
		//Do not destroy this window -> hide it
		m_tipWindow.HideTipWindow();
	}

	CXTStringHelper sTemp(m_strTipText);
	sTemp.Replace(_T("\\n"), _T("\n"));
	m_tipWindow.SetTipText(m_strTipTitle, sTemp);

	CRect rect;
	m_btnShowTip.GetWindowRect(&rect);
	m_tipWindow.SetMargins (CSize(m_cx,m_cy));
	m_tipWindow.SetLineSpace (m_nLineSpacing);
	m_tipWindow.ShowTipWindow(CPoint(rect.left-5, rect.bottom+5), m_dwTipStyle, m_nDelayMillisecs, m_nMillisecs);
}

void CTipWindowDlg::OnChangeEditTiptext()
{
	UpdateData();
}

void CTipWindowDlg::OnChangeEditTiptitle()
{
	UpdateData();
}

void CTipWindowDlg::OnSelEndOkTextColor()
{
	UpdateData();

	m_tipWindow.SetTipColors(m_clrTipBack,
		m_clrTipText);
}

void CTipWindowDlg::OnSelEndOkBackColor()
{
	UpdateData();

	m_tipWindow.SetTipColors(m_clrTipBack,
		m_clrTipText);
}

void CTipWindowDlg::UpdateStyle()
{
	m_dwTipStyle = 0L;

	if (m_bAlphaShadow) {
		m_dwTipStyle |= TWS_XT_ALPHASHADOW;
	}
	if (m_bThickBorder) {
		m_dwTipStyle |= TWS_XT_THICKBORDER;
	}
	if (m_bDropShadow) {
		m_dwTipStyle |= TWS_XT_DROPSHADOW;
	}
}

void CTipWindowDlg::OnChkAlphaShadowing()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	UpdateStyle();
}

void CTipWindowDlg::OnChkThickBorder()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	UpdateStyle();
}

void CTipWindowDlg::OnChkShadow()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	UpdateStyle();
}


BOOL CTipWindowDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_MOUSEMOVE && pMsg->wParam == 0)
	{
		int id = ::GetDlgCtrlID(pMsg->hwnd);
		CPoint point;
		point.x = LOWORD(pMsg->lParam);
		point.y = HIWORD(pMsg->lParam);
		if(id == IDC_LIST)
			MouseOverListCtrl(point);
		MouseOverCtrl(point, pMsg->hwnd);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CTipWindowDlg::MouseOverCtrl(CPoint point, HWND hWnd)
{
	CWnd* pWnd = FromHandle(hWnd);
	if(!pWnd)
		return;
	int id = pWnd->GetDlgCtrlID();

	if(id == IDC_EDIT_DELAY || id == IDC_CHK_MOVEBELOWCURSOR)
	{
		//Show the tip window
		UpdateData();

		CRect rect;
		pWnd->GetClientRect(&rect);

		//Calculate the client coordinates of the dialog window
		pWnd->ClientToScreen(&rect);
		pWnd->ClientToScreen(&point);
		ScreenToClient(&rect);
		ScreenToClient(&point);

		if(id == IDC_EDIT_DELAY)
			m_tipWindow.SetTipText(m_strTipTitle, _T("Please enter tip window delay time (ms)\n\n------- tabbed text output -------\na:\t123\nab:\t1234\nabc:\t12345\nabcd:\t123456"));
		if(id == IDC_CHK_MOVEBELOWCURSOR)
			m_tipWindow.SetTipText(m_strTipTitle, _T("Check this box to move the\ntip window of the list control\nbelow the cursor."));

		m_tipWindow.SetMargins (CSize(m_cx,m_cy));
		m_tipWindow.SetLineSpace (m_nLineSpacing);
		m_tipWindow.ShowTipWindow(rect, point, m_dwTipStyle, m_nDelayMillisecs, 0, FALSE, TRUE);
	}
}

void CTipWindowDlg::MouseOverListCtrl(CPoint point)
{
	//point = client coordinates inside the listctrl
	int nItem, nCol;

	if ((nItem = m_cListCtrl.HitTestEx(point, &nCol)) != -1)
	{

	#if _MSC_VER >= 1200 // MFC 5.0
		int nOriginalCol = m_cListCtrl.GetHeaderCtrl()->OrderToIndex(nCol);
		CString sItemText = m_cListCtrl.GetItemText(nItem, nOriginalCol);
		int iTextWidth = m_cListCtrl.GetStringWidth(sItemText) + 5; //5 pixed extra size

		CRect rect;
		m_cListCtrl.GetSubItemRect(nItem, nOriginalCol, LVIR_LABEL, rect);
		if(iTextWidth > rect.Width())
		{
			rect.top--;

			sItemText.Replace(_T("\r\n"), _T("\n")); //Avoid ugly outputted rectangle character in the tip window
			m_tipWindow.SetMargins(CSize(1,1));
			m_tipWindow.SetLineSpace(0);
			m_tipWindow.SetTipText(_T(""), sItemText);
			//Calculate the client coordinates of the dialog window
			m_cListCtrl.ClientToScreen(&rect);
			m_cListCtrl.ClientToScreen(&point);
			ScreenToClient(&rect);
			ScreenToClient(&point);

			//Show the tip window
			UpdateData();
			m_tipWindow.ShowTipWindow(rect, point, m_dwTipStyle, m_nDelayMillisecs, 0, false, m_bMoveTipBelowCursor);
		}
	#endif

	}
}
