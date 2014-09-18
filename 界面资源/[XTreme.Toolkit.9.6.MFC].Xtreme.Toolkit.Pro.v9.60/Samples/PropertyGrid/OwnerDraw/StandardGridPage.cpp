// StandardGridPage.cpp : implementation file
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
#include "OwnerDraw.h"
#include "StandardGridPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_PROPERTY_GRID 100
/////////////////////////////////////////////////////////////////////////////
// CStandardGridPage property page

IMPLEMENT_DYNCREATE(CStandardGridPage, CPropertyPage)

CStandardGridPage::CStandardGridPage() : CPropertyPage(CStandardGridPage::IDD)
{
	//{{AFX_DATA_INIT(CStandardGridPage)
	m_bCustomColors = FALSE;
	//}}AFX_DATA_INIT
}

CStandardGridPage::~CStandardGridPage()
{
}

void CStandardGridPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStandardGridPage)
	DDX_Check(pDX, IDC_CHK_CUSTOMCOLORS, m_bCustomColors);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStandardGridPage, CPropertyPage)
	//{{AFX_MSG_MAP(CStandardGridPage)
	ON_BN_CLICKED(IDC_CHK_CUSTOMCOLORS, OnClickedCustomcolors)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStandardGridPage message handlers


BOOL CStandardGridPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// get the size of the place holder, this will be used when creating the grid.
	CWnd* pPlaceHolder = GetDlgItem(IDC_PLACEHOLDER);

	CRect rc;
	pPlaceHolder->GetWindowRect( &rc );
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

		pItemVersion->SetReadOnly(TRUE);
		pVersion->Expand();

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CStandardGridPage::OnClickedCustomcolors()
{
	UpdateData();

	if (m_bCustomColors)
	{
		m_wndPropertyGrid.SetCustomColors( 0xC8C8C8, 0, 0xB6D2BD, 0xF7F3E9, 0);
	}
	else m_wndPropertyGrid.SetStandardColors();
}
