// PropertyGridDlg.cpp : implementation file
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
#include "PropertyGrid.h"
#include "PropertyGridDlg.h"

#include "AboutDlg.h"

#include "CustomItems.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPropertyGridDlg dialog

CPropertyGridDlg::CPropertyGridDlg(CWnd* pParent /*=NULL*/)
	: CPropertyGridDlgBase(CPropertyGridDlg::IDD, pParent)
	, m_bHelpVisible(TRUE)
	, m_bToolBarVisible(FALSE)
	, m_bCustomColors(FALSE)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nSort = 0;
	m_bCollapse = FALSE;
	m_bDoubleFont = FALSE;
	m_bWinXPTheme = FALSE;
	m_nTheme = 0;
	m_bTabItems = FALSE;

#ifdef _XTP_INCLUDE_CONTROLS
	// Enable/Disable XP GUI Mode
	XTAuxData().bXPMode = TRUE;
#endif

}

void CPropertyGridDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyGridDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyGridDlg)
	DDX_Control(pDX, IDC_COMBO_THEME, m_cmbTheme);
	DDX_Control(pDX, IDC_PLACEHOLDER, m_wndPlaceHolder);
	DDX_CBIndex(pDX, IDC_COMBO_THEME, m_nTheme);
	DDX_Check(pDX, IDC_CHK_TABITEMS, m_bTabItems);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHK_HELP, m_bHelpVisible);
	DDX_Check(pDX, IDC_CHK_TOOLBAR, m_bToolBarVisible);
	DDX_Check(pDX, IDC_CHK_DOUBLE, m_bDoubleFont);
	DDX_Radio(pDX, IDC_SORT_CATEGORIES, m_nSort);
	DDX_Check(pDX, IDC_CHK_CUSTOMCOLORS, m_bCustomColors);
}

BEGIN_MESSAGE_MAP(CPropertyGridDlg, CPropertyGridDlgBase)
	//{{AFX_MSG_MAP(CPropertyGridDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SWITCHSTATE, OnButtonSwitchstate)
	ON_CBN_SELCHANGE(IDC_COMBO_THEME, OnSelchangeComboTheme)
	ON_BN_CLICKED(IDC_CHK_TABITEMS, OnChkTabitems)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHK_TOOLBAR, OnClickedChkToolbar)
	ON_BN_CLICKED(IDC_CHK_HELP, OnClickedChkHelp)
	ON_BN_CLICKED(IDC_CHK_DOUBLE, OnClickedChkDouble)
	ON_BN_CLICKED(IDC_CHK_WINXPTHEME, OnClickedWinXPTheme)
	ON_BN_CLICKED(IDC_SORT_CATEGORIES, OnClickedSort)
	ON_BN_CLICKED(IDC_SORT_ALPHABETICAL, OnClickedSort)
	ON_BN_CLICKED(IDC_SORT_NOSORT, OnClickedSort)
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
	ON_BN_CLICKED(IDC_CHK_CUSTOMCOLORS, OnClickedCustomcolors)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyGridDlg message handlers

BOOL CPropertyGridDlg::OnInitDialog()
{
	CPropertyGridDlgBase::OnInitDialog();

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

	// get the size of the place holder, this will be used when creating the grid.
	CRect rc;
	m_wndPlaceHolder.GetWindowRect( &rc );
	ScreenToClient( &rc );

	// create the property grid.
	if ( m_wndPropertyGrid.Create( rc, this, IDC_PROPERTY_GRID ) )
	{
		LOGFONT lf;
		GetFont()->GetLogFont( &lf );

		// create document settings category.
		CXTPPropertyGridItem* pSettings        = m_wndPropertyGrid.AddCategory(_T("Document Settings"));

		// add child items to category.
		CXTPPropertyGridItem* pItemSaveOnClose = pSettings->AddChildItem(new CXTPPropertyGridItemBool(_T("SaveOnClose"), TRUE));
		pSettings->AddChildItem(new CXTPPropertyGridItemFont(_T("WindowFont"), lf));
		pSettings->AddChildItem(new CXTPPropertyGridItemSize(_T("WindowSize"), CSize(100, 100)));

		pSettings->Expand();
		pItemSaveOnClose->Select();

		// create global settings category.
		CXTPPropertyGridItem* pGlobals      = m_wndPropertyGrid.AddCategory(_T("Global Settings"));

		// add child items to category.
		CXTPPropertyGridItem* pItemGreeting = pGlobals->AddChildItem(new CXTPPropertyGridItem(_T("Greeting Text"), _T("Welcome to your application!")));
		pGlobals->AddChildItem(new CXTPPropertyGridItemNumber(_T("ItemsInMRUList"), 4));
		CXTPPropertyGridItem* pItemRate     = pGlobals->AddChildItem(new CXTPPropertyGridItemNumber(_T("MaxRepeatRate"), 10));
		pGlobals->AddChildItem(new CXTPPropertyGridItemColor(_T("ToolbarColor"), RGB(255, 192,128)));

		pItemGreeting->SetReadOnly(TRUE);
		pItemRate->SetDescription(_T("The rate in milliseconds that the text will repeat."));

		// create version category.
		CXTPPropertyGridItem* pVersion      = m_wndPropertyGrid.AddCategory(_T("Version"));

		// add child items to category.
		CXTPPropertyGridItem* pItemVersion  = pVersion->AddChildItem(new CXTPPropertyGridItem(_T("AppVerion"), _T("1.0")));
		CXTPPropertyGridItem* pItemLanguage = pVersion->AddChildItem(new CXTPPropertyGridItem(ID_ITEM_VERSION_LANGUAGE, _T("English (United States)")));

		pItemVersion->SetReadOnly(TRUE);
		pVersion->Expand();

		CXTPPropertyGridItemConstraints* pList = pItemLanguage->GetConstraints();

		pList->AddConstraint(_T("Neutral"));
		pList->AddConstraint(_T("Arabic"));
		pList->AddConstraint(_T("German"));
		pList->AddConstraint(_T("Chinese(Taiwan)"));
		pList->AddConstraint(_T("English (United Kingdom)"));
		pList->AddConstraint(_T("English (United States)"));
		pList->AddConstraint(_T("France"));
		pList->AddConstraint(_T("Russian"));

		pItemLanguage->SetFlags(xtpGridItemHasComboButton | xtpGridItemHasEdit);


		// create standard items category.
		CXTPPropertyGridItem* pStandard   = m_wndPropertyGrid.AddCategory(_T("Standard Items"));
		pStandard->AddChildItem(new CXTPPropertyGridItem(_T("String item")));
		pStandard->AddChildItem(new CXTPPropertyGridItemNumber(_T("Integer item")));
		pStandard->AddChildItem(new CXTPPropertyGridItemDouble(_T("Double item")));
		pStandard->AddChildItem(new CXTPPropertyGridItemColor(_T("Color item")));
		pStandard->AddChildItem(new CXTPPropertyGridItemBool(_T("Bool item")));
		pStandard->AddChildItem(new CXTPPropertyGridItemFont(_T("Font item"), lf));
		COleDateTime dates(1981, 1, 26, 0, 0, 0 );
		pStandard->AddChildItem(new CXTPPropertyGridItemDate(_T("Date item"), dates));
		pStandard->AddChildItem(new CXTPPropertyGridItemSize(_T("Size item")));

		CXTPPropertyGridItem* pItem = pStandard->AddChildItem(new CXTPPropertyGridItemEnum(_T("Enum item"), 2));
		pItem->GetConstraints()->AddConstraint(_T("Windows 98"), 1);
		pItem->GetConstraints()->AddConstraint(_T("Windows 2000"), 2);
		pItem->GetConstraints()->AddConstraint(_T("Windows XP"), 3);

		pItem = pStandard->AddChildItem(new CXTPPropertyGridItemFlags(_T("Flag item"), 1 + 2));
		pItem->GetConstraints()->AddConstraint(_T("All Windows"), 1 + 2 + 4);
		pItem->GetConstraints()->AddConstraint(_T("Windows 98"), 1);
		pItem->GetConstraints()->AddConstraint(_T("Windows 2000"), 2);
		pItem->GetConstraints()->AddConstraint(_T("Windows XP"), 4);



		// create custom items category.
		CXTPPropertyGridItem* pCustom   = m_wndPropertyGrid.AddCategory(_T("Custom Items"));

		// add child items to category.
		CXTPPropertyGridItem* pItemIcon = pCustom->AddChildItem(new CCustomItemIcon(_T("Icon"), m_hIcon));
		CXTPPropertyGridItem* pItemDock = pCustom->AddChildItem(new CCustomItemChilds(_T("DockPadding"), CRect(100, 20, 400, 50)));
		pCustom->AddChildItem(new CCustomItemColor(_T("CustomCombolist"), RGB(0xFF, 0x80, 0x40)));

		pItemIcon->SetDescription(_T("This sample shows how to override draw function"));
		pItemDock->SetDescription(_T("This sample shows how to add item with childs"));

		CXTPPropertyGridItem* pItemSpin = pCustom->AddChildItem(new CCustomItemSpin(_T("SpinButton")));
		pItemSpin->SetDescription(_T("This sample shows how to add new button type"));

		pCustom->AddChildItem(new CCustomItemFileBox(_T("File Box")));
		CCustomItemCheckBox* pItemCheckBox = (CCustomItemCheckBox*)pCustom->AddChildItem(new CCustomItemCheckBox(_T("Check Box")));
		pItemCheckBox->SetValue(_T("agree with conditions"));
		pItemCheckBox->SetBool(TRUE);

		CXTPPropertyGridItem* pItemMaskEdit = pCustom->AddChildItem(new CXTPPropertyGridItem(_T("Mask Edit"), _T("Phone No: (816) 220-0000")));
		pItemMaskEdit->SetMask(_T("Phone No: (000) 000-0000"), _T("Phone No: (___) ___-____"));


		CXTPPropertyGridItem* pItemPassword = pCustom->AddChildItem(new CXTPPropertyGridItem(_T("Password"), _T("Text")));
		pItemPassword->SetPasswordMask();


		COleDateTime date(1981, 1, 26, 0, 0, 0 );
		pCustom->AddChildItem(new CXTPPropertyGridItemDate(_T("Date"), date));


		pCustom->AddChildItem(new CCustomItemUpperCase(_T("UpperCase")));

		pCustom->AddChildItem(new CCustomItemSlider(_T("Slider")));


		// add multi level tree node.
		CXTPPropertyGridItem* pCategoryOne    = pCustom->AddChildItem(new CXTPPropertyGridItemCategory(_T("First Sub Category")));
		CXTPPropertyGridItem* pCategoryTwo    = pCategoryOne->AddChildItem(new CXTPPropertyGridItemCategory(_T("Second Sub Category 1")));
		pCategoryTwo->AddChildItem(new CXTPPropertyGridItem(_T("Third Level 1"), _T("")));
		pCategoryTwo->AddChildItem(new CXTPPropertyGridItem(_T("Third Level 2"), _T("")));
		CXTPPropertyGridItem* pCategoryTwo2   = pCategoryOne->AddChildItem(new CXTPPropertyGridItemCategory(_T("Second Sub Category 2")));
		pCategoryTwo2->AddChildItem(new CXTPPropertyGridItem(_T("Third Level 1"), _T("")));

		CXTPPropertyGridItem* pItemOne    = pCustom->AddChildItem(new CXTPPropertyGridItem(_T("First Level"), _T("")));
		CXTPPropertyGridItem* pItemTwo    = pItemOne->AddChildItem(new CXTPPropertyGridItem(_T("Second Level"), _T("")));
		CXTPPropertyGridItem* pItemThird     = pItemTwo->AddChildItem(new CXTPPropertyGridItem(_T("Third Level"), _T("")));
		pItemThird->AddChildItem(new CXTPPropertyGridItem(_T("Fourth Level 1"), _T("")));
		pItemThird->AddChildItem(new CXTPPropertyGridItem(_T("Fourth Level 2"), _T("")));

	}

#ifdef _XTP_INCLUDE_CONTROLS
	// Set control resizing.
	SetResize(IDC_PROPERTY_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_GBOX_APPEAR,       SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHK_TOOLBAR,       SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHK_HELP,          SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHK_DOUBLE,        SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHK_TABITEMS,      SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_COMBO_THEME,      SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_GBOX_SORT,         SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_SORT_CATEGORIES,   SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_SORT_ALPHABETICAL, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_SORT_NOSORT,       SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_GBOX_COLOR,        SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHK_CUSTOMCOLORS,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_SWITCHSTATE,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	// Load window placement
	AutoLoadPlacement(_T("CPropertyGridDlg"));
#endif

	m_cmbTheme.AddString(_T("Default"));
	m_cmbTheme.AddString(_T("NativeWinXP"));
	m_cmbTheme.AddString(_T("Office2003"));
	m_cmbTheme.AddString(_T("Cool"));
	m_cmbTheme.AddString(_T("Simple"));
	m_cmbTheme.AddString(_T("Delphi"));
	m_cmbTheme.AddString(_T("Whidbey"));
	m_cmbTheme.SetCurSel(0);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPropertyGridDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CPropertyGridDlgBase::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPropertyGridDlg::OnPaint()
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
		CPropertyGridDlgBase::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPropertyGridDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}




void CPropertyGridDlg::OnClickedChkToolbar()
{
	UpdateData();
	m_wndPropertyGrid.ShowToolBar(m_bToolBarVisible);
}

void CPropertyGridDlg::OnClickedChkHelp()
{
	UpdateData();
	m_wndPropertyGrid.ShowHelp(m_bHelpVisible);
}

void CPropertyGridDlg::OnClickedSort()
{
	UpdateData();
	m_wndPropertyGrid.SetPropertySort((XTPPropertyGridSortOrder)m_nSort);
}

LRESULT CPropertyGridDlg::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_PGN_SORTORDER_CHANGED)
	{
		m_nSort = m_wndPropertyGrid.GetPropertySort();
		UpdateData(FALSE);

		TRACE(_T("SortOrder Changed.\n"));
	}
	if (wParam == XTP_PGN_ITEMVALUE_CHANGED)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
		TRACE(_T("Value Changed. Caption = %s, ID = %i, Value = %s\n"), pItem->GetCaption(), pItem->GetID(), pItem->GetValue());
	}
	if (wParam == XTP_PGN_SELECTION_CHANGED)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
		if (pItem)
		{
			if (pItem->IsKindOf(RUNTIME_CLASS(CXTPPropertyGridItemColor)))
			{
				TRACE(_T("Color Item. Selection Changed. Item = %s\n"), pItem->GetCaption());
			}

			if (pItem->GetID() == ID_ITEM_VERSION_LANGUAGE)
			{
				//pItem->SetDescription(pItem->GetDescription() + "!");
			}

		}
	}
	if (wParam == XTP_PGN_EDIT_CHANGED)
	{
		CXTPPropertyGridInplaceEdit* pEdit = DYNAMIC_DOWNCAST(CXTPPropertyGridInplaceEdit, (CWnd*)lParam);
		if (pEdit && pEdit->GetItem())
		{
			// Custom Validation
			if (pEdit->GetItem()->GetID() == ID_ITEM_VERSION_LANGUAGE)
			{
				CString str;
				pEdit->CEdit::GetWindowText(str);

				if (str.GetLength() > 30)
				{
					MessageBeep((UINT)-1);
					pEdit->SetSel(0, -1);
					pEdit->ReplaceSel(str.Left(30));
				}
			}
			// Custom Validation
			if (pEdit->GetItem()->GetCaption() == _T("ItemsInMRUList"))
			{
				CString str;
				pEdit->CEdit::GetWindowText(str);

				int i = _ttoi(str);
				if (i > 20)
				{
					MessageBeep((UINT)-1);
					pEdit->SetSel(0, -1);
					pEdit->ReplaceSel(_T("20"));
				}
			}
		}
	}
	return 0;
}
void CPropertyGridDlg::OnClickedCustomcolors()
{
	UpdateData();

	if (m_bCustomColors)
	{
		m_wndPropertyGrid.SetCustomColors( RGB(200, 200, 200), 0, RGB(182, 210, 189), RGB(247, 243, 233), 0);
	}
	else m_wndPropertyGrid.SetStandardColors();
}

void CPropertyGridDlg::OnOK()
{
	CXTPPropertyGridItem* pItem = 0;

	pItem = m_wndPropertyGrid.FindItem(ID_ITEM_VERSION_LANGUAGE);
	TRACE(_T("%s - %s\n"), pItem->GetCaption(), pItem->GetValue());

	pItem = m_wndPropertyGrid.FindItem(_T("SaveOnClose"));
	TRACE(_T("%s - %s\n"), pItem->GetCaption(), pItem->GetValue());

	CPropertyGridDlgBase::OnOK();
}


typedef void (CXTPPropertyGridItem::*ITEMFUNCTIONPTR)();

void _DoCollapseExpand(CXTPPropertyGridItems* pItems, ITEMFUNCTIONPTR pFunction)
{
	for (int i = 0; i < pItems->GetCount(); i++)
	{
		CXTPPropertyGridItem* pItem = pItems->GetAt(i);
		if (pItem->HasChilds())
		{
			(pItem->*pFunction)();
			_DoCollapseExpand(pItem->GetChilds(), pFunction);
		}
	}
}

void CPropertyGridDlg::OnButtonSwitchstate()
{
	if (m_bCollapse)
	{
		_DoCollapseExpand(m_wndPropertyGrid.GetCategories(), &CXTPPropertyGridItem::Collapse);
	}
	else
	{
		_DoCollapseExpand(m_wndPropertyGrid.GetCategories(), &CXTPPropertyGridItem::Expand);
	}
	m_bCollapse = !m_bCollapse;
}

void CPropertyGridDlg::OnClickedChkDouble()
{
	UpdateData();

	LOGFONT lfIcon;
	VERIFY( ::SystemParametersInfo( SPI_GETICONTITLELOGFONT, sizeof( lfIcon ), &lfIcon, 0 ) );

	if (m_bDoubleFont) lfIcon.lfHeight *= 2;

	CFont fntNormal;
	fntNormal.CreateFontIndirect(&lfIcon );

	m_wndPropertyGrid.SetFont(&fntNormal);
}

void CPropertyGridDlg::OnClickedWinXPTheme()
{
	UpdateData();

	m_wndPropertyGrid.SetTheme(m_bWinXPTheme? xtpGridThemeNativeWinXP: xtpGridThemeDefault);
}

void CPropertyGridDlg::OnSelchangeComboTheme()
{
	UpdateData();
	m_wndPropertyGrid.SetTheme((XTPPropertyGridPaintTheme)m_nTheme);

}

void CPropertyGridDlg::OnChkTabitems()
{
	UpdateData();
	m_wndPropertyGrid.NavigateItems(m_bTabItems);
}
