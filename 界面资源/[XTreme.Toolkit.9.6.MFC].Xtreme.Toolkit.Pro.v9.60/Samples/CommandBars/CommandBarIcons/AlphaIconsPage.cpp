// AlphaIconsPage.cpp : implementation file
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
#include "AlphaIconsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlphaIconsPage dialog


CAlphaIconsPage::CAlphaIconsPage(CWnd* pParent /*=NULL*/)
	: CPropertiesTabPage(CAlphaIconsPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlphaIconsPage)
	m_bFaded = FALSE;
	m_bShadow = FALSE;
	m_bLargeImages = FALSE;
	m_bResample = FALSE;
	//}}AFX_DATA_INIT
}

void CAlphaIconsPage::UpdateContent()
{
	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();

	m_bFaded = pIconsInfo->bUseFadedIcons;
	m_bShadow = pIconsInfo->bIconsWithShadow;

	CXTPFrameWnd* pFrame = (CXTPFrameWnd*)AfxGetMainWnd();
	CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();

	ASSERT(pCommandBars);
	CXTPCommandBarsOptions* pOptions = pCommandBars->GetCommandBarsOptions();
	m_bLargeImages = pOptions->bLargeIcons;


	UpdateData(FALSE);
}


void CAlphaIconsPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlphaIconsPage)
	DDX_Check(pDX, IDC_CHECK_FADED, m_bFaded);
	DDX_Check(pDX, IDC_CHECK_SHADOW, m_bShadow);
	DDX_Check(pDX, IDC_CHECK_LARGEIMAGES, m_bLargeImages);
	DDX_Check(pDX, IDC_CHECK_RESAMPLE, m_bResample);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlphaIconsPage, CDialog)
	//{{AFX_MSG_MAP(CAlphaIconsPage)
	ON_BN_CLICKED(IDC_CHECK_FADED, OnCheckFaded)
	ON_BN_CLICKED(IDC_CHECK_SHADOW, OnCheckShadow)
	ON_BN_CLICKED(IDC_CHECK_LARGEIMAGES, OnCheckLargeimages)
	ON_BN_CLICKED(IDC_CHECK_RESAMPLE, OnCheckResample)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlphaIconsPage message handlers
void CAlphaIconsPage::RedrawCommandBars()
{
	CXTPFrameWnd* pFrame = (CXTPFrameWnd*)AfxGetMainWnd();
	pFrame->RecalcLayout(FALSE);
	pFrame->GetCommandBars()->RedrawCommandBars();
	pFrame->RedrawWindow();
}

void CAlphaIconsPage::OnCheckFaded()
{
	UpdateData();

	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bUseFadedIcons = m_bFaded;

	RedrawCommandBars();
}

void CAlphaIconsPage::OnCheckShadow()
{
	UpdateData();

	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bIconsWithShadow = m_bShadow;

	RedrawCommandBars();
}

void CAlphaIconsPage::OnCheckDisabled()
{
	UpdateData();

	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bUseDisabledIcons = m_bDisabled;

	RedrawCommandBars();
}

void CAlphaIconsPage::OnCancel()
{

}


void CAlphaIconsPage::OnActivate()
{
	CXTPFrameWnd* pFrame = (CXTPFrameWnd*)AfxGetMainWnd();
	CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();

	ASSERT(pCommandBars);

	CXTPToolBar* pToolBar = pCommandBars->GetAt(1);
	ASSERT(pToolBar->GetTitle() == _T("Standard"));


	XTPImageManager()->RemoveAll();
	pToolBar->LoadToolBar(IDR_ICONS_MENU_32);

	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bUseDisabledIcons = TRUE;

	CXTPCommandBarsOptions* pOptions = pCommandBars->GetCommandBarsOptions();
	pOptions->szIcons = CSize(16, 16);
	pOptions->szLargeIcons = CSize (42, 35);
	pOptions->bLargeIcons = FALSE;

	XTPImageManager()->SetIcons(IDR_ICONS_TOOLBAR_32);


	if (!XTPImageManager()->IsAlphaIconsSupported())
	{
		m_bResample = TRUE;
		GetDlgItem(IDC_CHECK_RESAMPLE)->EnableWindow(FALSE);
		UpdateData(FALSE);
	}

	ResampleImages();

	UpdateContent();
}

void CAlphaIconsPage::ResampleImages()
{
	if (m_bResample)
	{
		CBitmap bmpResample;
		bmpResample.Attach(CXTPImageManager::ResampleAlphaLayer(IDR_ICONS_MENU_32, RGB(0, 0xFF, 0)));
		XTPImageManager()->SetIcons(IDR_ICONS_MENU_32, bmpResample);

		bmpResample.DeleteObject();
		bmpResample.Attach(CXTPImageManager::ResampleAlphaLayer(IDR_ICONS_TOOLBAR_32, RGB(0, 0xFF, 0)));
		XTPImageManager()->SetIcons(IDR_ICONS_TOOLBAR_32, bmpResample);
	} else
	{
		XTPImageManager()->SetIcons(IDR_ICONS_MENU_32);
		XTPImageManager()->SetIcons(IDR_ICONS_TOOLBAR_32);
	}

	RedrawCommandBars();

}

void CAlphaIconsPage::OnCheckResample()
{
	UpdateData();
	ResampleImages();
}

void CAlphaIconsPage::OnCheckLargeimages()
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
