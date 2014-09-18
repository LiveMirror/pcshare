// DialogPropertyPages.cpp : implementation file
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
#include "commandbarsdesigner.h"
#include "DialogPropertyPages.h"
#include "EmbeddedFrame.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogPropertyPages dialog
LPCTSTR lpThemes[] = { _T("xtpThemeOfficeXP"), _T("xtpThemeOffice2000"), _T("xtpThemeOffice2003") , _T("xtpThemeNativeWinXP"), _T("xtpThemeWhidbey") };


CDialogPropertyPages::CDialogPropertyPages(CXTPCommandBars* pCommandBars, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPropertyPages::IDD, pParent)
{
	m_pCommandBars = pCommandBars;
	//{{AFX_DATA_INIT(CDialogPropertyPages)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	CXTPCommandBarsOptions* pOptions = pCommandBars->GetCommandBarsOptions();

	m_AlwaysShowFullMenus = pOptions->bAlwaysShowFullMenus;
	m_bLargeIcons = pOptions->bLargeIcons;
	m_bShowFullAfterDelay = pOptions->bShowFullAfterDelay;
	m_bToolBarAccelTips = pOptions->bToolBarAccelTips;
	m_bToolBarScreenTips = pOptions->bToolBarScreenTips;
	m_szIcons = pOptions->szIcons;
	m_szLargeIcons = pOptions->szLargeIcons;
	m_szPopupIcons = pOptions->szPopupIcons;
	m_bShowExpandButtonAlways = pOptions->bShowExpandButtonAlways;

	m_bSyncFloatingBars = pOptions->bSyncFloatingBars;
	m_bDblClickFloat = pOptions->bDblClickFloat;
	m_bShowTextBelowIcons = pOptions->bShowTextBelowIcons;
	m_bAltDragCustomization = pOptions->bAltDragCustomization;
	m_bAutoHideUnusedPopups = pOptions->bAutoHideUnusedPopups;
	m_nAnimationType = pOptions->animationType;

	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = pCommandBars->GetPaintManager()->GetIconsInfo();
	m_bIconsWithShadow = pIconsInfo->bIconsWithShadow;
	m_bUseDisabledIcons = pIconsInfo->bUseDisabledIcons;
	m_bUseFadedIcons = pIconsInfo->bUseFadedIcons;

	m_theme = ((CEmbeddedFrame*)pCommandBars->GetSite())->m_theme;
	m_bCustomizable = ((CEmbeddedFrame*)pCommandBars->GetSite())->m_bCustomizable;


	m_pSelected = NULL;
}


void CDialogPropertyPages::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPropertyPages)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogPropertyPages, CDialog)
	//{{AFX_MSG_MAP(CDialogPropertyPages)
	ON_NOTIFY(TVN_GETDISPINFO, IDC_LIST_OPTIONS, OnGetdispinfoListOptions)
	ON_NOTIFY(TVN_SELCHANGED, IDC_LIST_OPTIONS, OnSelchangedListOptions)
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPropertyPages message handlers

BOOL CDialogPropertyPages::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_wndList.SubclassDlgItem(IDC_LIST_OPTIONS, this);

	GetDlgItem(IDC_WORKSPACE)->GetWindowRect(m_rcWorkspace);
	ScreenToClient(m_rcWorkspace);

	HTREEITEM hItem = m_wndList.InsertItem(_T("Configuration Properties"), I_IMAGECALLBACK , I_IMAGECALLBACK );
	HTREEITEM hItemGeneral = m_wndList.InsertItem(_T("General"), 1, 0, hItem);
	HTREEITEM hItemPaint = m_wndList.InsertItem(_T("Paint Manager"), 1, 0, hItem);

	CreateGridGeneral();
	CreateGridPaint();

	m_wndList.SetItemData(hItemGeneral, (DWORD_PTR)&m_wndGridGeneral);
	m_wndList.SetItemData(hItemPaint, (DWORD_PTR)&m_wndGridPaint);
	m_wndGridPaint.ShowWindow(SW_HIDE);

	m_wndList.Expand(hItem, TVE_EXPAND);
	m_wndList.SelectItem(hItemGeneral);

	m_pSelected = &m_wndGridGeneral;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDialogPropertyPages::CreateGridGeneral()
{
	m_wndGridGeneral.Create(m_rcWorkspace, this, 1000);


	CXTPPropertyGridItem* pCategoryOptions = m_wndGridGeneral.AddCategory(_T("Options"));

	pCategoryOptions->AddChildItem(new CXTPPropertyGridItemBool(_T("Customizable"), m_bCustomizable, &m_bCustomizable));
	pCategoryOptions->AddChildItem(new CXTPPropertyGridItemBool(_T("Always Show Full Menus"), m_AlwaysShowFullMenus, &m_AlwaysShowFullMenus));
	pCategoryOptions->AddChildItem(new CXTPPropertyGridItemBool(_T("Show Full Menus After Delay"), m_bShowFullAfterDelay, &m_bShowFullAfterDelay));
	pCategoryOptions->AddChildItem(new CXTPPropertyGridItemBool(_T("Show ScreenTips on Toolbars"), m_bToolBarScreenTips, &m_bToolBarScreenTips));
	pCategoryOptions->AddChildItem(new CXTPPropertyGridItemBool(_T("Show Shortcut Keys in ScreenTips"), m_bToolBarAccelTips, &m_bToolBarAccelTips));
	pCategoryOptions->AddChildItem(new CXTPPropertyGridItemBool(_T("Show Expand Buttons Always"), m_bShowExpandButtonAlways, &m_bShowExpandButtonAlways));

	pCategoryOptions->AddChildItem(new CXTPPropertyGridItemBool(_T("Large Icons"), m_bLargeIcons, &m_bLargeIcons));
	pCategoryOptions->AddChildItem(new CXTPPropertyGridItemSize(_T("Icons Size"), m_szIcons, &m_szIcons));
	pCategoryOptions->AddChildItem(new CXTPPropertyGridItemSize(_T("Large Icons Size"), m_szLargeIcons, &m_szLargeIcons));
	pCategoryOptions->AddChildItem(new CXTPPropertyGridItemSize(_T("Popup Icons Size"), m_szPopupIcons, &m_szPopupIcons));

	CXTPPropertyGridItem* pItemAnimation = pCategoryOptions->AddChildItem(new CXTPPropertyGridItemEnum(_T("Menu Animation"), m_nAnimationType, (int*)&m_nAnimationType));
	pItemAnimation->GetConstraints()->AddConstraint(_T("xtpAnimateWindowsDefault"), xtpAnimateWindowsDefault);
	pItemAnimation->GetConstraints()->AddConstraint(_T("xtpAnimateRandom"), xtpAnimateRandom);
	pItemAnimation->GetConstraints()->AddConstraint(_T("xtpAnimateUnfold"), xtpAnimateUnfold);
	pItemAnimation->GetConstraints()->AddConstraint(_T("xtpAnimateSlide"), xtpAnimateSlide);
	pItemAnimation->GetConstraints()->AddConstraint(_T("xtpAnimateFade"), xtpAnimateFade);
	pItemAnimation->GetConstraints()->AddConstraint(_T("xtpAnimateNone"), xtpAnimateNone);


	pCategoryOptions->AddChildItem(new CXTPPropertyGridItemBool(_T("Sync Floating Toolbars"), m_bSyncFloatingBars, &m_bSyncFloatingBars));
	pCategoryOptions->AddChildItem(new CXTPPropertyGridItemBool(_T("Float Toolbars by DblClick "), m_bDblClickFloat, &m_bDblClickFloat));
	pCategoryOptions->AddChildItem(new CXTPPropertyGridItemBool(_T("Show Text Below Icons"), m_bShowTextBelowIcons, &m_bShowTextBelowIcons));
	pCategoryOptions->AddChildItem(new CXTPPropertyGridItemBool(_T("AltDrag Customization"), m_bAltDragCustomization, &m_bAltDragCustomization));
	pCategoryOptions->AddChildItem(new CXTPPropertyGridItemBool(_T("Auto Hide Unused Popups"), m_bAutoHideUnusedPopups, &m_bAutoHideUnusedPopups));


	pCategoryOptions->Expand();

}

void CDialogPropertyPages::CreateGridPaint()
{
	m_wndGridPaint.Create(m_rcWorkspace, this, 1001);


	CXTPPropertyGridItem* pCategoryOptions = m_wndGridPaint.AddCategory(_T("Options"));

	CXTPPropertyGridItem* pItemTheme = pCategoryOptions->AddChildItem(new CXTPPropertyGridItemEnum(ID_GRID_ITEM_THEME, m_theme, (int*)&m_theme));
	CXTPPropertyGridItemConstraints* pConstrants = pItemTheme->GetConstraints();
	for (int i = 0; i < _countof(lpThemes); i++)
		pConstrants->AddConstraint(lpThemes[i], i);



	pCategoryOptions->AddChildItem(new CXTPPropertyGridItemBool(ID_GRID_ITEM_ICONS_SHADOW, m_bIconsWithShadow, &m_bIconsWithShadow));
	pCategoryOptions->AddChildItem(new CXTPPropertyGridItemBool(_T("Generate disabled icons"), m_bUseDisabledIcons, &m_bUseDisabledIcons));
	pCategoryOptions->AddChildItem(new CXTPPropertyGridItemBool(ID_GRID_ITEM_ICONS_FADED, m_bUseFadedIcons, &m_bUseFadedIcons));


	pCategoryOptions->Expand();

}

void CDialogPropertyPages::OnGetdispinfoListOptions(NMHDR* pNMHDR, LRESULT* pResult)
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	if (pTVDispInfo->item.mask & (TVIF_SELECTEDIMAGE | TVIF_IMAGE))
	{
		UINT nState = m_wndList.GetItemState(pTVDispInfo->item.hItem, TVIF_STATE);
		pTVDispInfo->item.iSelectedImage = pTVDispInfo->item.iImage = nState & TVIS_EXPANDED? 3: 2;
	}

	*pResult = 0;
}

void CDialogPropertyPages::OnSelchangedListOptions(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if (pNMTreeView->itemNew.lParam == 0)
		return;

	if (pNMTreeView->itemNew.hItem)
	{

		CWnd* pPage = (CWnd*)pNMTreeView->itemNew.lParam;

		if (pPage != m_pSelected)
		{
			ASSERT(pPage);
			ASSERT_KINDOF(CXTPPropertyGrid, pPage);

			pPage->ShowWindow(SW_SHOW);
			pPage->EnableWindow();
			pPage->ModifyStyle(0, WS_TABSTOP);

			if (m_pSelected)
			{
				m_pSelected->ShowWindow(SW_HIDE);
			}
			m_pSelected = pPage;
		}
	}

	*pResult = 0;
}

void CDialogPropertyPages::OnOK()
{
	CXTPCommandBarsOptions* pOptions = m_pCommandBars->GetCommandBarsOptions();

	pOptions->bAlwaysShowFullMenus = m_AlwaysShowFullMenus;
	pOptions->bLargeIcons = m_bLargeIcons;
	pOptions->bShowFullAfterDelay = m_bShowFullAfterDelay;
	pOptions->bToolBarAccelTips = m_bToolBarAccelTips;
	pOptions->bToolBarScreenTips = m_bToolBarScreenTips;
	pOptions->szIcons = m_szIcons;
	pOptions->szLargeIcons = m_szLargeIcons;
	pOptions->szPopupIcons = m_szPopupIcons;
	pOptions->bShowExpandButtonAlways = m_bShowExpandButtonAlways;

	pOptions->bSyncFloatingBars = m_bSyncFloatingBars;
	pOptions->bDblClickFloat = m_bDblClickFloat;
	pOptions->bShowTextBelowIcons = m_bShowTextBelowIcons;
	pOptions->bAltDragCustomization = m_bAltDragCustomization;
	pOptions->bAutoHideUnusedPopups = m_bAutoHideUnusedPopups;
	pOptions->animationType = m_nAnimationType;


	((CEmbeddedFrame*)m_pCommandBars->GetSite())->SetTheme(m_theme);
	((CEmbeddedFrame*)m_pCommandBars->GetSite())->m_bCustomizable = m_bCustomizable;

	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = m_pCommandBars->GetPaintManager()->GetIconsInfo();
	pIconsInfo->bIconsWithShadow = m_bIconsWithShadow;
	pIconsInfo->bUseDisabledIcons = m_bUseDisabledIcons;
	pIconsInfo->bUseFadedIcons = m_bUseFadedIcons;


	m_pCommandBars->RedrawCommandBars();
	m_pCommandBars->RecalcFrameLayout();

	CDialog::OnOK();
}

BOOL CDialogPropertyPages::PreTranslateMessage(MSG* pMsg)
{

	return CDialog::PreTranslateMessage(pMsg);
}


LRESULT CDialogPropertyPages::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_PGN_ITEMVALUE_CHANGED)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;

		switch (pItem->GetID())
		{
			case ID_GRID_ITEM_THEME:
				{
					CString strTheme = pItem->GetValue();
					for (int i = 0; i < _countof(lpThemes); i++)
					if (strTheme == lpThemes[i])
					{
						switch (i)
						{
							case xtpThemeOffice2000:
							case xtpThemeOffice2003:
							case xtpThemeWhidbey:
								((CXTPPropertyGridItemBool*)m_wndGridPaint.FindItem(ID_GRID_ITEM_ICONS_SHADOW))->SetBool(FALSE);
								((CXTPPropertyGridItemBool*)m_wndGridPaint.FindItem(ID_GRID_ITEM_ICONS_FADED))->SetBool(FALSE);
								break;
							case xtpThemeOfficeXP:
								((CXTPPropertyGridItemBool*)m_wndGridPaint.FindItem(ID_GRID_ITEM_ICONS_SHADOW))->SetBool(TRUE);
								((CXTPPropertyGridItemBool*)m_wndGridPaint.FindItem(ID_GRID_ITEM_ICONS_FADED))->SetBool(TRUE);
								break;
							case xtpThemeNativeWinXP:
								((CXTPPropertyGridItemBool*)m_wndGridPaint.FindItem(ID_GRID_ITEM_ICONS_SHADOW))->SetBool(FALSE);
								((CXTPPropertyGridItemBool*)m_wndGridPaint.FindItem(ID_GRID_ITEM_ICONS_FADED))->SetBool(TRUE);
						}
						break;
					}
				}

		}
	}
	return FALSE;
}
