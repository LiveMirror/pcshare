// CoolGridPage.cpp : implementation file
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
#include "CoolGridPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCoolGridPage property page

IMPLEMENT_DYNCREATE(CCoolGridPage, CPropertyPage)

CCoolGridPage::CCoolGridPage() : CPropertyPage(CCoolGridPage::IDD)
{
	//{{AFX_DATA_INIT(CCoolGridPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCoolGridPage::~CCoolGridPage()
{
}

void CCoolGridPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCoolGridPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCoolGridPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCoolGridPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolGridPage message handlers

BOOL CCoolGridPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// get the size of the place holder, this will be used when creating the grid.
	CWnd* pPlaceHolder = GetDlgItem(IDC_PLACEHOLDER);

	CRect rc;
	pPlaceHolder->GetWindowRect( &rc );
	ScreenToClient( &rc );

	// create the property grid.
	if ( m_wndPropertyGrid.Create( rc, this, 100))
	{
		m_wndPropertyGrid.SetTheme(xtpGridThemeCool);

		LOGFONT lf;
		GetFont()->GetLogFont( &lf );

		// create document settings category.
		CXTPPropertyGridItem* pSettings        = m_wndPropertyGrid.AddCategory(_T("Document Settings"));
		CXTPPropertyGridItem* pSettingsGlobal = pSettings->AddChildItem(new CXTPPropertyGridItemCategory(_T("Global")));

		// add child items to category.
		CXTPPropertyGridItem* pItemSaveOnClose = pSettingsGlobal->AddChildItem(new CXTPPropertyGridItemBool(_T("SaveOnClose"), TRUE));

		CXTPPropertyGridItem* pSettingsAppearance = pSettings->AddChildItem(new CXTPPropertyGridItemCategory(_T("Appearance")));
		pSettingsAppearance->AddChildItem(new CXTPPropertyGridItemFont(_T("WindowFont"), lf));
		pSettingsAppearance->AddChildItem(new CXTPPropertyGridItemSize(_T("WindowSize"), CSize(100, 100)));

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
