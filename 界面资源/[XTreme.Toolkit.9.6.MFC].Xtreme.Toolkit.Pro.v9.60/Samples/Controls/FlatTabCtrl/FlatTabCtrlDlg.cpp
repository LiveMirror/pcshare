// FlatTabCtrlDlg.cpp : implementation file
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
#include "FlatTabCtrl.h"
#include "FlatTabCtrlDlg.h"
#include "AboutDlg.h"
#include "TabManipulationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlatTabCtrlDlg dialog

CFlatTabCtrlDlg::CFlatTabCtrlDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CFlatTabCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlatTabCtrlDlg)
	m_bIsBottom = TRUE;
	m_bHasArrows = TRUE;
	m_bHasHomeEnd = FALSE;
	m_bHasHScroll = TRUE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pFlatTabCtrl = NULL;
	m_dwTabStyle = -1;
	m_pManipDlg = NULL;
}

CFlatTabCtrlDlg::~CFlatTabCtrlDlg()
{
	delete m_pManipDlg;
	delete m_pFlatTabCtrl;
}

void CFlatTabCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlatTabCtrlDlg)
	DDX_Control(pDX, IDC_BORDER_TAB, m_staticBorder);
	DDX_Control(pDX, IDC_FTS_XT_HASARROWS, m_chkHasArrows);
	DDX_Check(pDX, IDC_FTS_XT_BOTTOM, m_bIsBottom);
	DDX_Check(pDX, IDC_FTS_XT_HASARROWS, m_bHasArrows);
	DDX_Check(pDX, IDC_FTS_XT_HASHOMEEND, m_bHasHomeEnd);
	DDX_Check(pDX, IDC_FTS_XT_HSCROLL, m_bHasHScroll);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFlatTabCtrlDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CFlatTabCtrlDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_FTS_XT_BOTTOM, UpdateTabsStyle)
	ON_BN_CLICKED(IDC_MANIPULATE, OnManipulate)
	ON_BN_CLICKED(IDC_FTS_XT_HASARROWS, UpdateTabsStyle)
	ON_BN_CLICKED(IDC_FTS_XT_HASHOMEEND, UpdateTabsStyle)
	ON_BN_CLICKED(IDC_FTS_XT_HSCROLL, UpdateTabsStyle)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlatTabCtrlDlg message handlers

void CFlatTabCtrlDlg::GetTabRect()
{
	CXTPWindowRect rcBorder(&m_staticBorder);
	ScreenToClient(&rcBorder);
	
	// get the size of the thumb box in a horizontal scroll bar.
	int cy = ::GetSystemMetrics(SM_CXHTHUMB); // SM_CYHSCROLL
	
	CRect rcTabCtrl = rcBorder;
	rcTabCtrl.DeflateRect(2, 2);
	
	CRect rcCtrl = rcBorder;
	rcCtrl.DeflateRect(2, 1, 2, cy + 1);
	
	// Create  the flat tab control.
	m_rcTabCtrl = rcTabCtrl;
}

BOOL CFlatTabCtrlDlg::OnInitDialog()
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
	SetResize(IDC_BORDER_TAB,        SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_FTS_XT_BOTTOM,     SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_FTS_XT_HASARROWS,  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_FTS_XT_HASHOMEEND, SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_FTS_XT_HSCROLL,    SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_MANIPULATE,        SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDOK,                  SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,              SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	// set the background color for our flicker free templates.
	m_sheet1.SetBackColor(GetXtremeColor(COLOR_WINDOW));
	m_sheet2.SetBackColor(GetXtremeColor(COLOR_WINDOW));
	m_sheet3.SetBackColor(GetXtremeColor(COLOR_WINDOW));
	m_sheet4.SetBackColor(GetXtremeColor(COLOR_WINDOW));
	m_sheet5.SetBackColor(GetXtremeColor(COLOR_WINDOW));
	m_sheet6.SetBackColor(GetXtremeColor(COLOR_WINDOW));

	// create the tabs
	GetTabRect();
	UpdateTabsStyle();

	// Load window placement
	LoadPlacement(_T("CFlatTabCtrlDlg"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFlatTabCtrlDlg::OnDestroy()
{
	// Save window placement
	SavePlacement(_T("CFlatTabCtrlDlg"));

	CXTResizeDialog::OnDestroy();
}

void CFlatTabCtrlDlg::SelectTabView(int nTab)
{
	m_sheet1.ShowWindow((nTab==0)?SW_SHOW:SW_HIDE);
	m_sheet2.ShowWindow((nTab==1)?SW_SHOW:SW_HIDE);
	m_sheet3.ShowWindow((nTab==2)?SW_SHOW:SW_HIDE);
	m_sheet4.ShowWindow((nTab==3)?SW_SHOW:SW_HIDE);
	m_sheet5.ShowWindow((nTab==4)?SW_SHOW:SW_HIDE);
	m_sheet6.ShowWindow((nTab==5)?SW_SHOW:SW_HIDE);
	m_pSheet7->ShowWindow((nTab==6)?SW_SHOW:SW_HIDE);
}

void CFlatTabCtrlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFlatTabCtrlDlg::OnPaint()
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
HCURSOR CFlatTabCtrlDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CFlatTabCtrlDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	if(IDC_FLAT_TABCTRL == (UINT)wParam)
	{
		NMHDR* pNMHDR = (NMHDR*)lParam;

		switch(pNMHDR->code)
		{
		case TCN_SELCHANGING:
			break;

		case TCN_SELCHANGE:
			// For unmanaged views, you can use something like this:
//          SelectTabView(m_pFlatTabCtrl->GetCurSel());
			break;
		}
	}

	return CXTResizeDialog::OnNotify(wParam, lParam, pResult);
}

void CFlatTabCtrlDlg::UpdateTabsStyle()
{
	int dwNewStyle = FTS_XT_TOOLTIPS;
	UpdateData();

	if (m_bHasHomeEnd)
	{
		dwNewStyle |= FTS_XT_HASHOMEEND;
		m_bHasArrows = TRUE;
		UpdateData(FALSE);
	}

	m_chkHasArrows.EnableWindow(!m_bHasHomeEnd);

	if (m_bIsBottom)
		dwNewStyle |= FTS_XT_BOTTOM;

	if (m_bHasArrows)
		dwNewStyle |= FTS_XT_HASARROWS;

	if (m_bHasHScroll)
		dwNewStyle |= FTS_XT_HSCROLL;

	if (dwNewStyle != m_dwTabStyle)
	{
		m_dwTabStyle = dwNewStyle;
		CreateTabs();
	}
}

void CFlatTabCtrlDlg::CreateTabs()
{
	BOOL bResize = FALSE;

	SetRedraw(false);
	RemoveResize(IDC_FLAT_TABCTRL);

	int iDefaultSel = 0;
	if (::IsWindow(m_pFlatTabCtrl->GetSafeHwnd()))
	{
		bResize = TRUE;
		iDefaultSel = m_pFlatTabCtrl->GetCurSel();
		m_pFlatTabCtrl->DestroyWindow();
	}

	SAFE_DELETE(m_pFlatTabCtrl);
	m_pFlatTabCtrl = new CXTFlatTabCtrl;

	// Create  the flat tab control.
	if (!m_pFlatTabCtrl->Create(WS_CHILD|WS_VISIBLE|m_dwTabStyle,
		m_rcTabCtrl, this, IDC_FLAT_TABCTRL))
	{
		TRACE0( "Failed to create flattab control\n" );
		return;
	}

	// set the tab control to have a static edge border.
	m_pFlatTabCtrl->ModifyStyleEx(NULL, WS_EX_STATICEDGE);

	CreateViews();

	// Insert tabs into the flat tab control.
	m_pFlatTabCtrl->InsertItem(0, _T("Build"),           &m_sheet1);
	m_pFlatTabCtrl->InsertItem(1, _T("Debug"),           &m_sheet2);
	m_pFlatTabCtrl->InsertItem(2, _T("Find in Files 1"), &m_sheet3);
	m_pFlatTabCtrl->InsertItem(3, _T("Find in Files 2"), &m_sheet4);
	m_pFlatTabCtrl->InsertItem(4, _T("Results"),         &m_sheet5);
	m_pFlatTabCtrl->InsertItem(5, _T("SQL Debugging"),   &m_sheet6);
	m_pFlatTabCtrl->InsertItem(6, _T("Form View"),       m_pSheet7);

	m_pFlatTabCtrl->SetCurSel(iDefaultSel);

	m_pFlatTabCtrl->SetWindowPos(&wndTop, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_FRAMECHANGED);

	// enable resizing for the flat tab control.
	SetResize(IDC_FLAT_TABCTRL, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	if (bResize) {
		CXTResize::Size();
	}
	
	SetRedraw(true);
	RedrawWindow(0,0, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
}

int CFlatTabCtrlDlg::CreateViews()
{
	CRect rectDummy(0, 0, 0, 0);
	// Create the sheet1 list box.
	if ( ::IsWindow( m_sheet1.m_hWnd ) )
	{
		m_sheet1.DestroyWindow();
	}
	if (!m_sheet1.Create(WS_CHILD |
		LVS_REPORT | LVS_NOCOLUMNHEADER, rectDummy, m_pFlatTabCtrl, IDC_SHEET1 ))
	{
		TRACE0( "Failed to create sheet1.\n" );
		return -1;
	}
	ListView_SetExtendedListViewStyleEx(m_sheet1.m_hWnd,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	// Create the sheet2 list box.
	if ( ::IsWindow( m_sheet2.m_hWnd ) )
	{
		m_sheet2.DestroyWindow();
	}
	if (!m_sheet2.Create( WS_CHILD | WS_TABSTOP | WS_CLIPCHILDREN |
		WS_HSCROLL | WS_VSCROLL | ES_MULTILINE, rectDummy, m_pFlatTabCtrl, IDC_SHEET2 ))
	{
		TRACE0( "Failed to create sheet2.\n" );
		return -1;
	}
	m_sheet2.SetFont(m_sheet1.GetFont());

	// Define the default style for the output list boxes.
	DWORD dwStyle = WS_CHILD | LBS_NOINTEGRALHEIGHT | WS_TABSTOP | WS_VSCROLL;

	// Create the sheet3 list box.
	if ( ::IsWindow( m_sheet3.m_hWnd ) )
	{
		m_sheet3.DestroyWindow();
	}
	if (!m_sheet3.Create(
		WS_CHILD | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | WS_VSCROLL,
		rectDummy, m_pFlatTabCtrl, IDC_SHEET3 ))
	{
		TRACE0( "Failed to create sheet3.\n" );
		return -1;
	}

	// Create the sheet4 list box.
	if ( ::IsWindow( m_sheet4.m_hWnd ) )
	{
		m_sheet4.DestroyWindow();
	}
	if (!m_sheet4.Create( dwStyle, rectDummy, m_pFlatTabCtrl, IDC_SHEET4 ))
	{
		TRACE0( "Failed to create sheet4.\n" );
		return -1;
	}

	// Create the sheet5 list box.
	if ( ::IsWindow( m_sheet5.m_hWnd ) )
	{
		m_sheet5.DestroyWindow();
	}
	if (!m_sheet5.Create( dwStyle, rectDummy, m_pFlatTabCtrl, IDC_SHEET5 ))
	{
		TRACE0( "Failed to create sheet5.\n" );
		return -1;
	}

	// Create the sheet6 list box.
	if ( ::IsWindow( m_sheet6.m_hWnd ) )
	{
		m_sheet6.DestroyWindow();
	}
	if (!m_sheet6.Create( dwStyle, rectDummy, m_pFlatTabCtrl, IDC_SHEET6 ))
	{
		TRACE0( "Failed to create sheet6.\n" );
		return -1;
	}

	// views must be created dynamically because they are destroyed during
	// window destruction
	if ( m_pSheet7 && ::IsWindow( m_pSheet7->m_hWnd ) )
	{
		m_pSheet7->DestroyWindow();
	}
	CCreateContext cc;
	ZeroMemory(&cc, sizeof(cc));
	m_pSheet7 = (CSampleFormView*)RUNTIME_CLASS(CSampleFormView)->CreateObject();
	if (!m_pSheet7  || !((CWnd *)m_pSheet7)->Create(
		NULL, NULL, WS_CHILD, rectDummy, m_pFlatTabCtrl, IDC_SHEET7, &cc))
	{
		TRACE0( "Failed to create sheet7.\n" );
		return -1;
	}

	// Insert text into the list box.
	m_sheet1.InsertColumn(0, _T("Column"), LVCFMT_LEFT, 512);
	m_sheet1.InsertItem(0, _T("--------------------Configuration: FlatTabCtrl - Win32 Debug--------------------"));
	m_sheet1.InsertItem(1, _T("Compiling resources..."));
	m_sheet1.InsertItem(2, _T("Compiling..."));
	m_sheet1.InsertItem(3, _T("StdAfx.cpp"));
	m_sheet1.InsertItem(5, _T("Compiling..."));
	m_sheet1.InsertItem(6, _T("AboutDlg.cpp"));
	m_sheet1.InsertItem(7, _T("FlatTabCtrl.cpp"));
	m_sheet1.InsertItem(8, _T("FlatTabCtrlDlg.cpp"));
	m_sheet1.InsertItem(9, _T("Generating Code..."));
	m_sheet1.InsertItem(10, _T("Linking..."));
	m_sheet1.InsertItem(11, _T(""));
	m_sheet1.InsertItem(12, _T("FlatTabCtrl.exe - 0 error(s), 0 warning(s)"));

//  m_sheet2.AddString(_T("Debug Tab Output..."));

	m_sheet2.SetWindowText(
		_T("Loaded 'C:\\WINNT\\System32\\ntdll.dll', no matching symbolic information found.\r\n")
		_T("Loaded symbols for 'C:\\WINNT\\system32\\MFC42D.DLL'\r\n")
		_T("Loaded symbols for 'C:\\WINNT\\system32\\MSVCRTD.DLL'\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\KERNEL32.DLL', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\GDI32.DLL', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\USER32.DLL', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\ADVAPI32.DLL', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\rpcrt4.dll', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\SHELL32.DLL', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\shlwapi.dll', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\comctl32.dll', no matching symbolic information found.\r\n")
		_T("Loaded symbols for 'C:\\WINNT\\system32\\MFCO42D.DLL'\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\OLE32.DLL', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\OLEAUT32.DLL', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\lpk.dll', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\usp10.dll', no matching symbolic information found.\r\n")
		_T("XTTOOLKIT_DLL.DLL Initializing!\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\indicdll.dll', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\imm32.dll', no matching symbolic information found.\r\n")
		_T("The program 'D:\\prj\\XT\\Toolkit\\Bin\\FlatTabCtrl.exe' has exited with code 0 (0x0).\r\n"));

///sheet3.AddString(_T("Find in Files 1 Tab Output..."));
	HTREEITEM file;
	file = m_sheet3.InsertItem(_T("D:\\Toolkit\\Samples\\FlatTabCtrl\\EditCtrlForFlatTab.cpp"));
	m_sheet3.InsertItem(_T("Line 92:            pSB->Create(WS_CHILD, CRect(0, 0, 0, 0), (CWnd *)this, 0)"), 0, 0, file);
	file = m_sheet3.InsertItem(_T("D:\\Toolkit\\Samples\\FlatTabCtrl\\FlatTabCtrl.rc"));
	m_sheet3.InsertItem(_T("Line 190:STYLE WS_CHILD"), 0, 0, file);
	file = m_sheet3.InsertItem(_T("D:\\Toolkit\\Samples\\FlatTabCtrl\\FlatTabCtrlDlg.cpp"));
	m_sheet3.InsertItem(_T("Line 92:            pSB->Create(WS_CHILD, CRect(0, 0, 0, 0), (CWnd *)this, 0)"), 0, 0, file);
	m_sheet3.InsertItem(_T("Line 265:   if (!m_pFlatTabCtrl->Create(WS_CHILD|WS_VISIBLE|m_dwTabStyle,"), 0, 0, file);
	m_sheet3.InsertItem(_T("Line 293:   if (!m_sheet1.Create(WS_CHILD |  "), 0, 0, file);
	m_sheet3.InsertItem(_T("Line 303:   if (!m_sheet2.Create( WS_CHILD | WS_TABSTOP | WS_CLIPCHILDREN |"), 0, 0, file);
	m_sheet3.InsertItem(_T("Line 312:   DWORD dwStyle = WS_CHILD | LBS_NOINTEGRALHEIGHT | WS_TABSTOP | WS_VSCROLL;"), 0, 0, file);
	m_sheet3.InsertItem(_T("Line 316:       WS_CHILD | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | WS_VSCROLL, "), 0, 0, file);
	m_sheet3.InsertItem(_T("Line 348:        !((CWnd *)m_pSheet7)->Create(NULL, NULL, WS_CHILD, "), 0, 0, file);
	file = m_sheet3.InsertItem(_T("D:\\Toolkit\\Samples\\FlatTabCtrl\\SampleFormView.cpp"));
	m_sheet3.InsertItem(_T("Line 79:    m_ScrollBarH.Create(WS_CHILD, CRect(0, 0, 0, 0), this, 0);"), 0, 0, file);

	m_sheet4.AddString(_T("Find in Files 2 Tab Output..."));
	m_sheet5.AddString(_T("Results Tab Output..."));
	m_sheet6.AddString(_T("SQL Debugging Tab Output..."));

	// set the font for the list boxes
	m_sheet4.SetFont(&XTAuxData().font);
	m_sheet5.SetFont(&XTAuxData().font);
	m_sheet6.SetFont(&XTAuxData().font);

	return TRUE;
}

void CFlatTabCtrlDlg::OnManipulate()
{
	if (m_pManipDlg == NULL)
	{
		m_pManipDlg = new CTabManipulationDlg(m_pFlatTabCtrl);
		m_pManipDlg->Create(CTabManipulationDlg::IDD, this);
	}
	m_pManipDlg->UpdateDlgStatus();
	m_pManipDlg->ShowWindow(SW_SHOW);
	m_pManipDlg->SetFocus();
}
