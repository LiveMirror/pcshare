// TreeCtrlDlg.cpp : implementation file
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
#include "TreeCtrl.h"
#include "TreeCtrlDlg.h"
#include "AboutDlg.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlDlg dialog

CTreeCtrlDlg::CTreeCtrlDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CTreeCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTreeCtrlDlg)
	m_nIndex = 0;
	m_bBoldCheck = FALSE;
	m_strFontSize = _T("");
	//}}AFX_DATA_INIT

	// Enable/Disable XP GUI Mode
	XTAuxData().bXPMode = TRUE;

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_clrTreeText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_strFontSize = _T("8");
}

void CTreeCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTreeCtrlDlg)
	DDX_Control(pDX, IDC_SIZE_COMBO, m_SizeCombo);
	DDX_Control(pDX, IDC_COLOR_PICKER, m_colorPicker);
	DDX_Control(pDX, IDC_TREE_CTRL, m_TreeCtrl);
	DDX_Control(pDX, IDC_FONT_COMBO, m_FontCombo);
	DDX_CBIndex(pDX, IDC_FONT_COMBO, m_nIndex);
	DDX_Check(pDX, IDC_BOLD_CHECK, m_bBoldCheck);
	DDX_CBString(pDX, IDC_SIZE_COMBO, m_strFontSize);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTreeCtrlDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CTreeCtrlDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BOLD_CHECK, OnBoldCheck)
	ON_BN_CLICKED(IDC_APPLY_BUTTON, OnApplyButton)
	ON_CBN_SELENDOK(IDC_SIZE_COMBO, OnSelendokSizeCombo)
	ON_CBN_SELENDOK(IDC_FONT_COMBO, OnSelendokFontCombo)
	ON_CPN_XT_SELENDOK(IDC_COLOR_PICKER, OnSelEndOK)
	ON_CBN_EDITCHANGE(IDC_SIZE_COMBO, OnEditchangeSizeCombo)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_CTRL, OnRclickTreeCtrl)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE_CTRL, OnEndlabeleditTreeCtrl)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlDlg message handlers

BOOL CTreeCtrlDlg::OnInitDialog()
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
	SetResize(IDC_TREE_CTRL,    SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_TXT_FONT,     SZ_TOP_RIGHT,    SZ_TOP_RIGHT);
	SetResize(IDC_FONT_COMBO,   SZ_TOP_RIGHT,    SZ_TOP_RIGHT);
	SetResize(IDC_TXT_SIZE,     SZ_TOP_RIGHT,    SZ_TOP_RIGHT);
	SetResize(IDC_SIZE_COMBO,   SZ_TOP_RIGHT,    SZ_TOP_RIGHT);
	SetResize(IDC_TXT_COLOR,    SZ_TOP_RIGHT,    SZ_TOP_RIGHT);
	SetResize(IDC_COLOR_PICKER, SZ_TOP_RIGHT,    SZ_TOP_RIGHT);
	SetResize(IDC_BOLD_CHECK,   SZ_TOP_RIGHT,    SZ_TOP_RIGHT);
	SetResize(IDC_TXT_INFO,     SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,         SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_APPLY_BUTTON, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	// Load window placement
	LoadPlacement(_T("CTreeCtrlDlg"));

	m_colorPicker.SetColor( m_clrTreeText );

	m_SizeCombo.SetCurSel(0);

	m_TreeCtrl.EnableMultiSelect();

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_TREE_IMAGES);

	m_ImageList.Create(16, 16, ILC_COLOR24|ILC_MASK, 2, 1);
	m_ImageList.Add(&bitmap, RGB(0,255,0));

	m_TreeCtrl.SetImageList(&m_ImageList, TVSIL_NORMAL);

	HTREEITEM hti = m_TreeCtrl.InsertItem(_T("Root Item"), 0, 1);
	m_TreeCtrl.SetItemBold(hti);
	m_TreeCtrl.FocusItem(hti);

	int i;
	for(i = 0; i < 10; ++i )
	{
		m_TreeCtrl.InsertItem(_T("Child Item"), 0, 1, hti);
	}
	m_TreeCtrl.Expand(hti, TVE_EXPAND);
	m_TreeCtrl.SetFont(&XTAuxData().font);

	// initialize cool menus
//  m_coolMenu.HookWindow(m_hWnd);
//  m_coolMenu.LoadToolbar(IDR_POPUP_TREE);

	// select the tree control's font
	LOGFONT lf;
	m_TreeCtrl.GetFont()->GetLogFont(&lf);

	m_FontCombo.InitControl( lf.lfFaceName, 285 );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTreeCtrlDlg::OnDestroy()
{
	// Save window placement
	SavePlacement(_T("CTreeCtrlDlg"));

	CXTResizeDialog::OnDestroy();
}

void CTreeCtrlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTreeCtrlDlg::OnPaint()
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
HCURSOR CTreeCtrlDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTreeCtrlDlg::OnSelendokFontCombo()
{
	m_FontCombo.GetSelFont( m_LogFont );
}

void CTreeCtrlDlg::OnSelendokSizeCombo()
{
	UpdateData();
}

void CTreeCtrlDlg::OnBoldCheck()
{
	UpdateData();
}

void CTreeCtrlDlg::OnSelEndOK()
{
	// a color selection was made, update the
	// appropriate member data.
	m_clrTreeText = m_colorPicker.GetColor();
}

void CTreeCtrlDlg::OnApplyButton()
{
	HTREEITEM hItem = m_TreeCtrl.GetFirstSelectedItem();
	while( hItem != NULL )
	{
		HDC hDC = ::GetDC(m_TreeCtrl.m_hWnd);
		m_LogFont.lfHeight = -MulDiv(_ttoi(m_strFontSize), GetDeviceCaps(hDC, LOGPIXELSY), 72);
		m_TreeCtrl.SetItemFont ( hItem, m_LogFont );
		m_TreeCtrl.SetItemBold ( hItem, m_bBoldCheck );
		m_TreeCtrl.SetItemColor( hItem, m_clrTreeText );
		m_TreeCtrl.SendMessage( WM_ERASEBKGND, (WPARAM)hDC, (LPARAM)0 );
		m_TreeCtrl.Invalidate();
		::ReleaseDC(m_TreeCtrl.m_hWnd, hDC);

		hItem = m_TreeCtrl.GetNextSelectedItem( hItem );
	}
}

void CTreeCtrlDlg::OnEditchangeSizeCombo()
{
	UpdateData();
}

void CTreeCtrlDlg::OnRclickTreeCtrl(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	CPoint point;
	GetCursorPos(&point);

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP_TREE));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);

	*pResult = 0;
}

void CTreeCtrlDlg::OnEditCut()
{
	// TODO: Add your command handler code here
	AfxMessageBox(_T("You selected Cut!"));
}

void CTreeCtrlDlg::OnEditCopy()
{
	// TODO: Add your command handler code here
	AfxMessageBox(_T("You selected Copy!"));
}

void CTreeCtrlDlg::OnEditPaste()
{
	// TODO: Add your command handler code here
	AfxMessageBox(_T("You selected Paste!"));
}

void CTreeCtrlDlg::OnEndlabeleditTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	// TODO: Add your control notification handler code here
	if (pTVDispInfo->item.pszText)
	{
		m_TreeCtrl.SetItemText(pTVDispInfo->item.hItem,
			pTVDispInfo->item.pszText);
	}

	*pResult = 0;
}

BOOL CTreeCtrlDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->wParam == VK_RETURN ||
		pMsg->wParam == VK_ESCAPE)
	{
		// don't allow dialog to process return or escape keys.
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);

		return TRUE;
	}

	return CXTResizeDialog::PreTranslateMessage(pMsg);
}
