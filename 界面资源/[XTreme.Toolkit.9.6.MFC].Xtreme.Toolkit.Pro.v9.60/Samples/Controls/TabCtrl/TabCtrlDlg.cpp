// TabCtrlDlg.cpp : implementation file
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
#include "TabCtrl.h"
#include "TabCtrlDlg.h"
#include "AboutDlg.h"
#include "View1.h"
#include "View2.h"
#include "View3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditListBoxEx dialog

const int BTN_OFFSET = 22;

BEGIN_MESSAGE_MAP(CEditListBoxEx, CXTEditListBox)
	//{{AFX_MSG_MAP(CEditListBoxEx)
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CEditListBoxEx::Initialize(bool bAutoFont/*=true*/)
{
	CXTListBox::Initialize(bAutoFont);

	// get a pointer to the parent window.
	m_pParentWnd = GetParent();
	ASSERT_VALID(m_pParentWnd); // must be valid.

	// Create the edit toolbar group.
	if (!HasToolbar())
		return;

	// Create the toolbar.
	if (!CreateEditGroup(bAutoFont))
		return;

	// force WM_WINDOWPOSCHANGING to be called to adjust the
	// size of the edit list control, which will also cause
	// WM_WINDOWPOSCHANGED to be called to position the toolbar.
	::SetWindowPos( GetSafeHwnd(), NULL, 0, 0, 0, 0,
		SWP_FRAMECHANGED|SWP_NOSIZE|SWP_NOMOVE);
}

void CEditListBoxEx::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
	if (HasToolbar() && ::IsWindow(m_editGroup.m_hWnd))
	{
		lpwndpos->cy -= BTN_OFFSET;
		lpwndpos->y  += BTN_OFFSET;
	}

	CXTListBox::OnWindowPosChanging(lpwndpos);
}

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlDlg dialog

CTabCtrlDlg::CTabCtrlDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CTabCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabCtrlDlg)
	m_bAutoCondense = TRUE;
	m_bVertical = FALSE;
	m_bMultiLine = FALSE;
	m_bXPMode = FALSE;
	//}}AFX_DATA_INIT
	m_bCloseButton = FALSE;
	m_bArrowButtons = TRUE;

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTabCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabCtrlDlg)
	DDX_Control(pDX, IDC_TAB_CTRL, m_tabCtrl);
	DDX_Check(pDX, IDC_AUTOCONDENSE, m_bAutoCondense);
	DDX_Check(pDX, IDC_VERTICAL, m_bVertical);
	DDX_Check(pDX, IDC_MULTILINE, m_bMultiLine);
	DDX_Control(pDX, IDC_VERTICAL, m_chkVertical);
	DDX_Control(pDX, IDC_MULTILINE, m_chkMultiline);
	DDX_Control(pDX, IDC_AUTOCONDENSE, m_chkAutoCondense);
	DDX_Check(pDX, IDC_XPMODE, m_bXPMode);

	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_ARROWS, m_bArrowButtons);
	DDX_Check(pDX, IDC_CLOSE, m_bCloseButton);
}

BEGIN_MESSAGE_MAP(CTabCtrlDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CTabCtrlDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_AUTOCONDENSE, OnAutocondense)
	ON_BN_CLICKED(IDC_VERTICAL, OnVertical)
	ON_BN_CLICKED(IDC_MULTILINE, OnMultiline)
	ON_WM_WINDOWPOSCHANGED()
	ON_BN_CLICKED(IDC_XPMODE, OnXpmode)
	//}}AFX_MSG_MAP
	ON_LBN_XT_LABELEDITEND(IDC_EDIT_LIST, OnLabelEditEnd)
	ON_LBN_XT_LABELEDITCANCEL(IDC_EDIT_LIST, OnLabelEditCancel)
	ON_LBN_XT_NEWITEM(IDC_EDIT_LIST, OnNewItem)
	ON_LBN_XT_DELETEITEM(IDC_EDIT_LIST, OnDeleteItem)
	ON_LBN_XT_MOVEITEMUP(IDC_EDIT_LIST, OnMoveItemUp)
	ON_LBN_XT_MOVEITEMDOWN(IDC_EDIT_LIST, OnMoveItemDown)

	ON_BN_CLICKED(IDC_ARROWS, OnChkButtons)
	ON_BN_CLICKED(IDC_CLOSE, OnChkButtons)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlDlg message handlers

BOOL CTabCtrlDlg::OnInitDialog()
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

	VERIFY(InitTabCtrl()  != -1);
	VERIFY(InitEditList() != -1);
	VERIFY(AddTabViews()  != -1);

	// Set control resizing.
	SetResize(IDC_TAB_CTRL,     SZ_TOP_LEFT,    SZ_BOTTOM_RIGHT);
	SetResize(IDOK,             SZ_TOP_RIGHT,   SZ_TOP_RIGHT);
	SetResize(IDCANCEL,         SZ_TOP_RIGHT,   SZ_TOP_RIGHT);
	SetResize(IDC_AUTOCONDENSE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_VERTICAL,     SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_MULTILINE,    SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_XPMODE,       SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_ARROWS,       SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CLOSE,        SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	// Load window placement
	LoadPlacement(_T("CTabCtrlDlg"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTabCtrlDlg::OnDestroy()
{
	// Save window placement
	SavePlacement(_T("CTabCtrlDlg"));

	CXTResizeDialog::OnDestroy();
}

void CTabCtrlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTabCtrlDlg::OnPaint()
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
HCURSOR CTabCtrlDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/////////////////////////////////////////////////////////////////////////////
// Tab control initialization
/////////////////////////////////////////////////////////////////////////////

int CTabCtrlDlg::InitTabCtrl()
{
	// you can use this to change the tab orientation

	/*
	m_tabCtrl.ModifyTabStyle(NULL, TCS_BOTTOM);
	*/

	// Have the tab control send WM_INITIALUPDATE to views.
	m_tabCtrl.SendInitialUpdate(TRUE);

	// you can use something like the following snippet to remove an image
	// from a tab that's already been created...

	/*
	TCITEM tci;
	tci.mask = TCIF_IMAGE;
	tci.iImage = -1;
	m_tabCtrl.SetItem(1, &tci);
	*/

	m_tabCtrl.SetPadding(CSize(6, 3));

	// Create the image list used by the tab control.
	if (!m_imageList.Create( IDB_IMAGELIST, 16, 1, RGB( 0x00,0xFF,0x00 )))
	{
		TRACE0("Failed to create image list.\n");
		return -1;
	}

	// Set the tab controls image list.
	m_tabCtrl.SetImageList(&m_imageList);

	// Set the tab to auto condense (shrink) its tabs when sized.
	m_tabCtrl.SetAutoCondense(m_bAutoCondense);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_tabCtrl.ModifyStyle(0L,
		WS_CLIPCHILDREN|WS_CLIPSIBLINGS);

	ForceRefreshTabCtrl();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Edit list box initialization
/////////////////////////////////////////////////////////////////////////////

int CTabCtrlDlg::InitEditList()
{
	if (!m_listBox.CreateEx(WS_EX_CLIENTEDGE, _T("listbox"), NULL,
		WS_CHILD|WS_VISIBLE|LBS_NOINTEGRALHEIGHT|WS_VSCROLL,
		CRect(0,0,0,0), &m_tabCtrl, IDC_EDIT_LIST))
	{
		TRACE0("Failed to create listbox control.\n");
		return -1;
	}

	m_listBox.SetOwner(this);

	// Set the font for the listbox control.
	m_listBox.SetFont (&XTAuxData().font);

	CString strText;
	int iItem;
	for (iItem = 1; iItem < 11; ++iItem)
	{
		strText.Format(_T("ListBox Item %d"), iItem);
		m_listBox.AddString(strText);
	}

	// initialize list box.
	m_listBox.Initialize();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Tab view's added to tab control
/////////////////////////////////////////////////////////////////////////////

int CTabCtrlDlg::AddTabViews()
{
	// add views to the tab control.
	if (!m_tabCtrl.AddView(_T("View One - HTML"), RUNTIME_CLASS(CView1), NULL, NULL, 0, 0))
	{
		TRACE0("Failed to create CView1.\n");
		return -1;
	}

	if (!m_tabCtrl.AddView(_T("View Two"), RUNTIME_CLASS(CView2), NULL, NULL, 1, 1))
	{
		TRACE0("Failed to create CView2.\n");
		return -1;
	}

	if (!m_tabCtrl.AddView(_T("View Three"), RUNTIME_CLASS(CView3), NULL, NULL, 2, 2))
	{
		TRACE0("Failed to create CView3.\n");
		return -1;
	}

	if (!m_tabCtrl.AddControl(_T("View Four - Edit List"), &m_listBox, 3, 3))
	{
		TRACE0("Failed to create edit list.\n");
		return -1;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Edit list box message handling
/////////////////////////////////////////////////////////////////////////////

void CTabCtrlDlg::OnLabelEditEnd()
{
	TRACE0("CTabCtrlDlg::OnLabelEditEnd() called.\n");
}

void CTabCtrlDlg::OnLabelEditCancel()
{
	TRACE0("CTabCtrlDlg::OnLabelEditCancel() called.\n");
}

void CTabCtrlDlg::OnNewItem()
{
	TRACE0("CTabCtrlDlg::OnNewItem() called.\n");
}

void CTabCtrlDlg::OnDeleteItem()
{
	TRACE0("CTabCtrlDlg::OnDeleteItem() called.\n");
}

void CTabCtrlDlg::OnMoveItemUp()
{
	TRACE0("CTabCtrlDlg::OnMoveItemUp() called.\n");
}

void CTabCtrlDlg::OnMoveItemDown()
{
	TRACE0("CTabCtrlDlg::OnMoveItemDown() called.\n");
}

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlDlg message handlers
/////////////////////////////////////////////////////////////////////////////

void CTabCtrlDlg::OnAutocondense()
{
	UpdateData();
	m_tabCtrl.SetAutoCondense(m_bAutoCondense);
	ForceRefreshTabCtrl();
}

void CTabCtrlDlg::OnVertical()
{
	UpdateData();
	ToggleStyle(TCS_VERTICAL, m_bVertical);
	GetDlgItem(IDC_MULTILINE)->EnableWindow(!m_bVertical);
	OnMultiline();
}

void CTabCtrlDlg::OnMultiline()
{
	UpdateData();
	ToggleStyle(TCS_MULTILINE, m_bMultiLine | m_bVertical);
}

void CTabCtrlDlg::ToggleStyle(int style, BOOL bEnable)
{
	if (bEnable)
	{
		m_tabCtrl.ModifyTabStyle(0, style);
	}
	else
	{
		m_tabCtrl.ModifyTabStyle(style, 0);
	}
	ForceRefreshTabCtrl();
}

void CTabCtrlDlg::ForceRefreshTabCtrl()
{
	::SetWindowPos(m_tabCtrl.m_hWnd, NULL, 0,0,0,0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
	m_tabCtrl.Invalidate(FALSE);

	UpdateData();
	BOOL bEnable = !m_bVertical && m_bXPMode && !m_bAutoCondense && !m_bMultiLine;

	GetDlgItem(IDC_ARROWS)->EnableWindow(bEnable);
	GetDlgItem(IDC_CLOSE)->EnableWindow(bEnable);
	GetDlgItem(IDC_MULTILINE)->EnableWindow(!m_bAutoCondense);
}

void CTabCtrlDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CXTResizeDialog::OnWindowPosChanged(lpwndpos);

	if (::IsWindow(m_chkVertical.m_hWnd))
	{
		m_chkVertical.Invalidate();
		m_chkVertical.UpdateWindow();
	}

	if (::IsWindow(m_chkMultiline.m_hWnd))
	{
		m_chkMultiline.Invalidate();
		m_chkMultiline.UpdateWindow();
	}

	if (::IsWindow(m_chkAutoCondense.m_hWnd))
	{
		m_chkAutoCondense.Invalidate();
		m_chkAutoCondense.UpdateWindow();
	}
}

void CTabCtrlDlg::OnXpmode()
{
	UpdateData();

	m_tabCtrl.SetTheme(m_bXPMode? xtThemeOfficeXP: xtThemeDefault);

	if ( m_bXPMode )
	{
		m_tabCtrl.m_bXPBorder = false;
		m_listBox.GetEditGroup().SetXButtonStyle(BS_XT_XPFLAT);
	}
	else
	{
		m_listBox.GetEditGroup().SetXButtonStyle(BS_XT_FLAT);
	}

	RedrawWindow( NULL, NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN );

	ForceRefreshTabCtrl();
}

void CTabCtrlDlg::OnChkButtons()
{
	UpdateData();

	m_tabCtrl.ShowNavButtons((m_bCloseButton? xtNavBtnClose: 0) |
		(m_bArrowButtons? xtNavBtnArrows: 0));

	ForceRefreshTabCtrl();
}

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlEx

CTabCtrlEx::CTabCtrlEx()
{
	// TODO: add construction code here.

}

CTabCtrlEx::~CTabCtrlEx()
{
	// TODO: add destruction code here.

}

IMPLEMENT_DYNAMIC(CTabCtrlEx, CXTTabCtrl)

BEGIN_MESSAGE_MAP(CTabCtrlEx, CXTTabCtrl)
	//{{AFX_MSG_MAP(CTabCtrlEx)
	ON_COMMAND(XT_IDC_TAB_CLOSE, OnCloseTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlEx message handlers

void CTabCtrlEx::OnCloseTab()
{
	if ( GetItemCount( ) == 1 )
	{
		AfxMessageBox( IDS_CANNOTDEL );
	}
	else
	{
		DeleteView( GetCurSel( ) );
	}
}
