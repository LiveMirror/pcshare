// IconsInfoPage.cpp : implementation file
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
#include "CommandBarIcons.h"
#include "IconsInfoPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIconsInfoPage dialog


CIconsInfoPage::CIconsInfoPage(CWnd* pParent /*=NULL*/)
	: CPropertiesTabPage(CIconsInfoPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIconsInfoPage)
	m_bDisabled = FALSE;
	m_bFaded = FALSE;
	m_bShadow = FALSE;
	m_bLargeImages = FALSE;
	//}}AFX_DATA_INIT
}

void CIconsInfoPage::UpdateContent()
{
	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();

	m_bDisabled = pIconsInfo->bUseDisabledIcons;
	m_bFaded = pIconsInfo->bUseFadedIcons;
	m_bShadow = pIconsInfo->bIconsWithShadow;

	CXTPFrameWnd* pFrame = (CXTPFrameWnd*)AfxGetMainWnd();
	CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();

	ASSERT(pCommandBars);
	CXTPCommandBarsOptions* pOptions = pCommandBars->GetCommandBarsOptions();
	m_bLargeImages = pOptions->bLargeIcons;

	UpdateData(FALSE);
}


void CIconsInfoPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIconsInfoPage)
	DDX_Check(pDX, IDC_CHECK_DISABLED, m_bDisabled);
	DDX_Check(pDX, IDC_CHECK_FADED, m_bFaded);
	DDX_Check(pDX, IDC_CHECK_SHADOW, m_bShadow);
	DDX_Check(pDX, IDC_CHECK_LARGEIMAGES, m_bLargeImages);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIconsInfoPage, CDialog)
	//{{AFX_MSG_MAP(CIconsInfoPage)
	ON_BN_CLICKED(IDC_CHECK_FADED, OnCheckFaded)
	ON_BN_CLICKED(IDC_CHECK_SHADOW, OnCheckShadow)
	ON_BN_CLICKED(IDC_CHECK_DISABLED, OnCheckDisabled)
	ON_BN_CLICKED(IDC_CHECK_LARGEIMAGES, OnCheckLargeimages)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconsInfoPage message handlers
void CIconsInfoPage::RedrawCommandBars()
{
	CXTPFrameWnd* pFrame = (CXTPFrameWnd*)AfxGetMainWnd();
	pFrame->RecalcLayout(FALSE);
	pFrame->GetCommandBars()->RedrawCommandBars();
	pFrame->RedrawWindow();
}

void CIconsInfoPage::OnCheckFaded()
{
	UpdateData();

	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bUseFadedIcons = m_bFaded;

	RedrawCommandBars();
}

void CIconsInfoPage::OnCheckShadow()
{
	UpdateData();

	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bIconsWithShadow = m_bShadow;

	RedrawCommandBars();
}

void CIconsInfoPage::OnCheckDisabled()
{
	UpdateData();

	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bUseDisabledIcons = m_bDisabled;

	RedrawCommandBars();
}

void CIconsInfoPage::OnCancel()
{

}


void CIconsInfoPage::OnActivate()
{
	CXTPFrameWnd* pFrame = (CXTPFrameWnd*)AfxGetMainWnd();
	CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();

	ASSERT(pCommandBars);

	CXTPToolBar* pToolBar = pCommandBars->GetAt(1);
	ASSERT(pToolBar->GetTitle() == _T("Standard"));


	XTPImageManager()->RemoveAll();
	pToolBar->LoadToolBar(IDR_MAINFRAME);
	XTPImageManager()->SetIcons(IDR_MENU_ICONS);

	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bUseDisabledIcons = TRUE;

	CXTPCommandBarsOptions* pOptions = pCommandBars->GetCommandBarsOptions();
	pOptions->szLargeIcons = CSize (24, 24);
	pOptions->bLargeIcons = FALSE;

	XTPImageManager()->SetIcons(IDR_MAINFRAME_LARGE);

	UpdateContent();
}

void CIconsInfoPage::OnCheckLargeimages()
{
	UpdateData();

	CXTPFrameWnd* pFrame = (CXTPFrameWnd*)AfxGetMainWnd();
	CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();

	ASSERT(pCommandBars);
	CXTPCommandBarsOptions* pOptions = pCommandBars->GetCommandBarsOptions();

	// Set global icon's size
	pOptions->bLargeIcons = m_bLargeImages;

	RedrawCommandBars();
}
