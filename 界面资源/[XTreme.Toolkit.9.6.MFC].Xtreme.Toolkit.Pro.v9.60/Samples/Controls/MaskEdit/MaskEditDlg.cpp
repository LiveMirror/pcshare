// MaskEditDlg.cpp : implementation file
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
#include "MaskEdit.h"
#include "MaskEditDlg.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaskEditDlg dialog

CMaskEditDlg::CMaskEditDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CMaskEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMaskEditDlg)
	m_strEditMask = _T("");
	m_strLiteral = _T("");
	m_strDefault = _T("");
	m_bDefault = FALSE;
	m_strPrompt = _T("_");
	//}}AFX_DATA_INIT

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMaskEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaskEditDlg)
	DDX_Control(pDX, IDC_EDIT_PROMPT, m_editPrompt);
	DDX_Control(pDX, IDC_BTN_HELP, m_btnHelp);
	DDX_Control(pDX, IDC_EDIT_LITERAL, m_editLiteral);
	DDX_Control(pDX, IDC_EDIT_MASK, m_editMask);
	DDX_Control(pDX, IDC_EDIT_DEFAULT, m_editDefault);
	DDX_Control(pDX, IDC_LBOX_MASK, m_lboxMask);
	DDX_Control(pDX, IDC_EDIT_MASKEDIT, m_editMaskEdit);
	DDX_Control(pDX, IDC_GBOX_PROPERTIES, m_grpProperties);
	DDX_Text(pDX, IDC_EDIT_MASK, m_strEditMask);
	DDX_Text(pDX, IDC_EDIT_LITERAL, m_strLiteral);
	DDX_Text(pDX, IDC_EDIT_DEFAULT, m_strDefault);
	DDX_Check(pDX, IDC_CHK_DEFAULT, m_bDefault);
	DDX_Text(pDX, IDC_EDIT_PROMPT, m_strPrompt);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMaskEditDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CMaskEditDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_LBOX_MASK, OnSelchangeLboxMask)
	ON_BN_CLICKED(IDC_CHK_DEFAULT, OnChkDefault)
	ON_BN_CLICKED(IDC_BTN_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_BTN_HELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

struct MASK_ITEM
{
	LPCTSTR lpszMask;
	LPCTSTR lpszLiteral;
	LPCTSTR lpszDefault;
};

static MASK_ITEM _maskArray[] =
{
	{ _T("Phone No: (000) 000-0000"),    _T("Phone No: (___) ___-____"),    _T("Phone No: (800) 555-1212")    },
	{ _T("SSN: 000-00-0000"),            _T("SSN: ___-__-____"),            _T("SSN: 123-45-6789")            },
	{ _T("IP Address: 000.000.000.000"), _T("IP Address: ___.___.___.___"), _T("IP Address: 192.168.100.100") },
	{ _T("Currency: $0,000.00"),         _T("Currency: $_,___.__"),         _T("Currency: $1,250.29")         },
	{ _T("Hex: 0xHHHHHHHH"),             _T("Hex: 0x________"),             _T("Hex: 0x0012FCE4")             },
	{ _T("Date: 00/00/00"),              _T("Date: __/__/__"),              _T("Date: 08/26/02")              },
	{ _T("Time: 00:00"),                 _T("Time: 00:00"),                 _T("Time: 14:25")                 },
	{ _T("Upper Case: >>>>>>>"),         _T("Upper Case: _______"),         _T("Upper Case: ABCDEFG")         },
	{ _T("Lower Case: <<<<<<<"),         _T("Lower Case: _______"),         _T("Lower Case: abcdefg")         },
	{ _T("Alpha: LLLLLLL"),              _T("Alpha: _______"),              _T("Alpha: AbCdEfG")              },
	{ _T("Alpha-Numeric: AAAAAAA"),      _T("Alpha-Numeric: _______"),      _T("Alpha-Numeric: AbC1234")      },
};

/////////////////////////////////////////////////////////////////////////////
// CMaskEditDlg message handlers

BOOL CMaskEditDlg::OnInitDialog()
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

	// initialize edit controls.
	m_editLiteral.Initialize(this);
	m_editMask.Initialize(this);
	m_editDefault.Initialize(this);
	m_editMaskEdit.Initialize(this);

	// initialize the list box.
	m_lboxMask.Initialize();

	// Add masks to the list box.
	int i;
	for ( i = 0; i < _countof(_maskArray); ++i )
	{
		m_lboxMask.AddString( _maskArray[ i ].lpszMask );
	}

	m_lboxMask.SetCurSel( 0 );
	OnSelchangeLboxMask();

	// Set control resizing.
	SetResize(IDC_EDIT_MASKEDIT,   SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_TXT_MASK,        SZ_TOP_CENTER,   SZ_TOP_RIGHT);
	SetResize(IDC_LBOX_MASK,       SZ_TOP_CENTER,   SZ_TOP_RIGHT);
	SetResize(IDC_BTN_UPDATE,      SZ_TOP_CENTER,   SZ_TOP_CENTER);
	SetResize(IDC_EDIT_MASK,       SZ_TOP_LEFT,     SZ_TOP_CENTER);
	SetResize(IDC_EDIT_PROMPT,     SZ_TOP_LEFT,     SZ_TOP_CENTER);
	SetResize(IDC_EDIT_LITERAL,    SZ_TOP_LEFT,     SZ_TOP_CENTER);
	SetResize(IDC_EDIT_DEFAULT,    SZ_TOP_LEFT,     SZ_TOP_CENTER);
	SetResize(IDC_GBOX_PROPERTIES, SZ_TOP_LEFT,     SZ_TOP_CENTER);
	SetResize(IDCANCEL,            SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK,                SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	// prevent vertical resizing.
	SetFlag(xtResizeNoVertical);

	// Load window placement
	LoadPlacement(_T("CMaskEditDlg"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMaskEditDlg::OnDestroy()
{
	// Save window placement
	SavePlacement(_T("CMaskEditDlg"));

	CXTResizeDialog::OnDestroy();
}

void CMaskEditDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMaskEditDlg::OnPaint()
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
HCURSOR CMaskEditDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMaskEditDlg::OnSelchangeLboxMask()
{
	// get the current selection from the list box.
	int iSel = m_lboxMask.GetCurSel( );

	// if a valid selection has been made
	if ( LB_ERR != iSel )
	{
		// update the edit fields
		m_strEditMask = _maskArray[ iSel ].lpszMask;
		m_strLiteral  = _maskArray[ iSel ].lpszLiteral;
		m_strDefault  = _maskArray[ iSel ].lpszDefault;

		// set the edit mask
		m_editMaskEdit.SetEditMask(
			m_strEditMask, m_strLiteral, m_bDefault ? m_strDefault : m_strLiteral );

		m_editMaskEdit.SetPromptChar( '_' );
		m_strPrompt = '_';

		// enable / disable default edit
		m_editDefault.EnableWindow( m_bDefault );

		UpdateData( FALSE );
	}
}

void CMaskEditDlg::OnChkDefault()
{

	OnUpdate();
}

void CMaskEditDlg::RemoveSpaces( CString& strIn )
{
	CString strBuffer;
	int i;
	for ( i = 0; i < strIn.GetLength(); ++i )
	{
		TCHAR ch = strIn.GetAt( i );
		if ( ch != ' ' )
		{
			strBuffer += ch;
		}
	}
	strIn = strBuffer;
}

void CMaskEditDlg::OnUpdate()
{
	UpdateData( );

	int iLen1 = m_strEditMask.GetLength();
	int iLen2 = m_strLiteral.GetLength();
	int iLen3 = m_strDefault.GetLength();

	if ( iLen1 != iLen2 || iLen1 != iLen3 )
	{
		AfxMessageBox( _T("The fields for Mask, Literal and Default must be the same length.") );
		OnSelchangeLboxMask();
		return;
	}

	TCHAR ch = '_';
	if ( m_strPrompt.GetLength( ) )
	{
		ch = m_strPrompt.GetAt( 0 );
	}

	// if the prompt character is a space, we have to remove
	// it from the mask, literal and default strings.
	if ( ch == ' ' )
	{
		RemoveSpaces( m_strEditMask );
		RemoveSpaces( m_strLiteral );
		RemoveSpaces( m_strDefault );
	}

	m_strLiteral.Replace(
		m_editMaskEdit.GetPromptChar( ), ch );

	// set the edit mask
	m_editMaskEdit.SetEditMask(
		m_strEditMask, m_strLiteral, m_bDefault ? m_strDefault : m_strLiteral );

	m_editMaskEdit.SetPromptChar( ch );
	m_strPrompt = ch;

	m_editDefault.EnableWindow( m_bDefault );

	UpdateData( FALSE );
}

void CMaskEditDlg::OnHelp()
{
	if ( ::IsWindow( m_dlg.m_hWnd ) )
	{
		m_dlg.DestroyWindow();
	}

	if ( m_dlg.Create( IDD_MASKHELP ) )
	{
		m_dlg.ShowWindow( SW_SHOW );
		m_dlg.UpdateWindow();
	}
}
