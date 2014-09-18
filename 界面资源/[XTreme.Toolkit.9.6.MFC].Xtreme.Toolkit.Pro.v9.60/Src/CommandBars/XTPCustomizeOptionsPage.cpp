// XTPCustomizeOptionsPage.cpp : implementation of the CXTPCustomizeOptionsPage class.
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
#include "XTPResourceManager.h"

#include "XTPCommandBarsResource.h"
#include "XTPCustomizeOptionsPage.h"
#include "XTPCustomizeSheet.h"
#include "XTPToolBar.h"
#include "XTPCommandBars.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPCustomizeOptionsPage property page


CXTPCustomizeOptionsPage::CXTPCustomizeOptionsPage(CXTPCustomizeSheet* pSheet) : CPropertyPage(CXTPCustomizeOptionsPage::IDD)
{
	CXTPCommandBars* pCommandBars = pSheet->GetCommandBars();
	CXTPCommandBarsOptions* pOptions = pCommandBars->GetCommandBarsOptions();

	m_bAlwaysShowFullMenus = pOptions->bAlwaysShowFullMenus;
	m_bShowFullAfterDelay = pOptions->bShowFullAfterDelay;
	m_bLargeIcons = pOptions->bLargeIcons;
	m_bToolBarScreenTips = pOptions->bToolBarScreenTips;
	m_bToolBarAccelTips = pOptions->bToolBarAccelTips;

	m_nAnimationType = pOptions->animationType;

	//{{AFX_DATA_INIT(CXTPCustomizeOptionsPage)
	//}}AFX_DATA_INIT
	m_pSheet = pSheet;

	//RevertResourceContext();
}

CXTPCustomizeOptionsPage::~CXTPCustomizeOptionsPage()
{
}

CXTPCommandBars* CXTPCustomizeOptionsPage::GetCommandBars()
{
	ASSERT(m_pSheet && m_pSheet->GetCommandBars());
	return m_pSheet->GetCommandBars();
}

void CXTPCustomizeOptionsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, XTP_IDC_COMBO_ANIMATION, m_comboAnimationType);
	DDX_CBIndex(pDX, XTP_IDC_COMBO_ANIMATION, m_nAnimationType);

	//{{AFX_DATA_MAP(CXTPCustomizeOptionsPage)
	DDX_Check(pDX, XTP_IDC_CHECK_FULL_MENUS, m_bAlwaysShowFullMenus);
	DDX_Check(pDX, XTP_IDC_CHECK_AFTERDELAY, m_bShowFullAfterDelay);
	DDX_Check(pDX, XTP_IDC_CHECK_LARGEICONS, m_bLargeIcons);
	DDX_Check(pDX, XTP_IDC_CHECK_SCREENTTIPS, m_bToolBarScreenTips);
	DDX_Check(pDX, XTP_IDC_CHECK_SHORTCUTS, m_bToolBarAccelTips);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXTPCustomizeOptionsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CXTPCustomizeOptionsPage)
	ON_BN_CLICKED(XTP_IDC_CHECK_FULL_MENUS, OnCheckFullMenus)
	ON_BN_CLICKED(XTP_IDC_CHECK_AFTERDELAY, OnCheckAfterdelay)
	ON_BN_CLICKED(XTP_IDC_CHECK_LARGEICONS, OnCheckLargeicons)
	ON_BN_CLICKED(XTP_IDC_CHECK_SCREENTTIPS, OnCheckScreenttips)
	ON_BN_CLICKED(XTP_IDC_CHECK_SHORTCUTS, OnCheckShortcuts)
	ON_BN_CLICKED(XTP_IDC_BUTTON_RESETDATA, OnResetData)
	ON_CBN_SELCHANGE(XTP_IDC_COMBO_ANIMATION, OnAnimationChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPCustomizeOptionsPage message handlers

void CXTPCustomizeOptionsPage::AddComboString( UINT nIDResource )
{
	CString str;
	XTPResourceManager()->LoadString(&str, nIDResource);

	m_comboAnimationType.AddString(str);
}


BOOL CXTPCustomizeOptionsPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	GetDlgItem(XTP_IDC_CHECK_AFTERDELAY)->EnableWindow(!m_bAlwaysShowFullMenus);
	GetDlgItem(XTP_IDC_CHECK_SHORTCUTS)->EnableWindow(m_bToolBarScreenTips);

	// Add strings to animation combo box.
	AddComboString( XTP_IDS_ANIMATION_SYSDEF );
	AddComboString( XTP_IDS_ANIMATION_RANDOM );
	AddComboString( XTP_IDS_ANIMATION_UNFOLD );
	AddComboString( XTP_IDS_ANIMATION_SLIDE );
	AddComboString( XTP_IDS_ANIMATION_FADE );
	AddComboString( XTP_IDS_ANIMATION_NONE );

	// Ensure combo selection is made.
	UpdateData( FALSE );


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTPCustomizeOptionsPage::OnCheckFullMenus()
{
	UpdateData();
	GetDlgItem(XTP_IDC_CHECK_AFTERDELAY)->EnableWindow(!m_bAlwaysShowFullMenus);

	GetCommandBars()->GetCommandBarsOptions()->bAlwaysShowFullMenus = m_bAlwaysShowFullMenus;
}


void CXTPCustomizeOptionsPage::OnAnimationChanged()
{
	UpdateData();
	if (m_nAnimationType != CB_ERR)
	{
		GetCommandBars()->GetCommandBarsOptions()->animationType = (XTPAnimationType)m_nAnimationType;
	}
}

void CXTPCustomizeOptionsPage::OnCheckAfterdelay()
{
	UpdateData();
	GetCommandBars()->GetCommandBarsOptions()->bShowFullAfterDelay = m_bShowFullAfterDelay;
}

void CXTPCustomizeOptionsPage::OnCheckLargeicons()
{
	UpdateData();
	CXTPCommandBars* pCommandBars = m_pSheet->GetCommandBars();
	pCommandBars->GetCommandBarsOptions()->bLargeIcons = m_bLargeIcons;

	pCommandBars->RedrawCommandBars();
}

void CXTPCustomizeOptionsPage::OnCheckScreenttips()
{
	UpdateData();
	GetDlgItem(XTP_IDC_CHECK_SHORTCUTS)->EnableWindow(m_bToolBarScreenTips);
	GetCommandBars()->GetCommandBarsOptions()->bToolBarScreenTips = m_bToolBarScreenTips;

}

void CXTPCustomizeOptionsPage::OnCheckShortcuts()
{
	UpdateData();
	GetCommandBars()->GetCommandBarsOptions()->bToolBarAccelTips = m_bToolBarAccelTips;
}

void CXTPCustomizeOptionsPage::OnResetData()
{
	if (XTPResourceManager()->ShowMessageBox(XTP_IDS_CONFIRM_RESETDATA, MB_ICONWARNING|MB_YESNO ) == IDYES )
	{
		GetCommandBars()->ResetUsageData();
	}
}
