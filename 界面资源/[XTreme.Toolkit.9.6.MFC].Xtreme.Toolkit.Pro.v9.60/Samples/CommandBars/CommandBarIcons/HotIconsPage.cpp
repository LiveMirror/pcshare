// HotIconsPage.cpp : implementation file
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
#include "HotIconsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHotIconsPage dialog


CHotIconsPage::CHotIconsPage(CWnd* pParent /*=NULL*/)
	: CPropertiesTabPage(CHotIconsPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHotIconsPage)
	m_bDisabled = FALSE;
	m_bFaded = FALSE;
	m_bShadow = FALSE;
	//}}AFX_DATA_INIT
}


void CHotIconsPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHotIconsPage)
	DDX_Check(pDX, IDC_CHECK_DISABLED, m_bDisabled);
	DDX_Check(pDX, IDC_CHECK_FADED, m_bFaded);
	DDX_Check(pDX, IDC_CHECK_SHADOW, m_bShadow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHotIconsPage, CDialog)
	//{{AFX_MSG_MAP(CHotIconsPage)
	ON_BN_CLICKED(IDC_CHECK_FADED, OnCheckFaded)
	ON_BN_CLICKED(IDC_CHECK_SHADOW, OnCheckShadow)
	ON_BN_CLICKED(IDC_CHECK_DISABLED, OnCheckDisabled)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHotIconsPage message handlers


void CHotIconsPage::OnActivate()
{
	CXTPFrameWnd* pFrame = (CXTPFrameWnd*)AfxGetMainWnd();
	CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();

	ASSERT(pCommandBars);

	CXTPToolBar* pToolBar = pCommandBars->GetAt(1);
	ASSERT(pToolBar->GetTitle() == _T("Standard"));


	XTPImageManager()->RemoveAll();
	pToolBar->LoadToolBar(IDR_TOOLBAR_ICONS_NORMAL);

	XTPImageManager()->SetIcons(IDR_TOOLBAR_ICONS_HOT, xtpImageHot);


	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bUseFadedIcons = FALSE;

	CXTPCommandBarsOptions* pOptions = pCommandBars->GetCommandBarsOptions();
	pOptions->bLargeIcons = FALSE;


	UpdateContent();

}


void CHotIconsPage::OnCancel()
{

}


void CHotIconsPage::UpdateContent()
{
	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();

	m_bDisabled = pIconsInfo->bUseDisabledIcons;
	m_bFaded = pIconsInfo->bUseFadedIcons;
	m_bShadow = pIconsInfo->bIconsWithShadow;

	UpdateData(FALSE);
}

void CHotIconsPage::RedrawCommandBars()
{
	CXTPFrameWnd* pFrame = (CXTPFrameWnd*)AfxGetMainWnd();
	pFrame->RecalcLayout(FALSE);
	pFrame->GetCommandBars()->RedrawCommandBars();
	pFrame->RedrawWindow();

}

void CHotIconsPage::OnCheckFaded()
{
	UpdateData();

	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bUseFadedIcons = m_bFaded;

	RedrawCommandBars();
}

void CHotIconsPage::OnCheckShadow()
{
	UpdateData();

	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bIconsWithShadow = m_bShadow;

	RedrawCommandBars();
}

void CHotIconsPage::OnCheckDisabled()
{
	UpdateData();

	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bUseDisabledIcons = m_bDisabled;

	RedrawCommandBars();
}
