// XTPCustomizeKeyboardPage.cpp : implementation of the CXTPCustomizeKeyboardPage class.
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
#include "XTPIntel80Helpers.h"

#include "XTPCommandBarsResource.h"
#include "XTPShortcutManager.h"
#include "XTPToolBar.h"
#include "XTPPaintManager.h"
#include "XTPControls.h"
#include "XTPControl.h"
#include "XTPControlPopup.h"
#include "XTPCommandBars.h"
#include "XTPCustomizeSheet.h"
#include "XTPCustomizeCommandsPage.h"
#include "XTPCustomizeKeyboardPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPCustomizeKeyboardPage property page

CXTPCustomizeKeyboardPage::CXTPCustomizeKeyboardPage(CXTPCustomizeSheet* pSheet) : CPropertyPage(CXTPCustomizeKeyboardPage::IDD)
{
	//{{AFX_DATA_INIT(CXTPCustomizeKeyboardPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSheet = pSheet;
	m_strDesc   = _T("");
	m_iCategory = 0;

	//RevertResourceContext();
}

CXTPCustomizeKeyboardPage::~CXTPCustomizeKeyboardPage()
{
	for (int i = 0; i < m_arrCategories.GetSize(); i++)
	{
		delete m_arrCategories[i];
	}
}

void CXTPCustomizeKeyboardPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPCustomizeKeyboardPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, XTP_IDC_EDIT_SHORTCUTKEY, m_editShortcutKey);
	DDX_Control(pDX, XTP_IDC_LBOX_KEYS, m_lboxKeys);
	DDX_Control(pDX, XTP_IDC_LBOX_ACCEL_COMMANDS, m_lboxCommands);
	DDX_Control(pDX, XTP_IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Text(pDX, XTP_IDC_TXT_DESC_OVERVIEW, m_strDesc);
	DDX_CBIndex(pDX, XTP_IDC_COMBO_CATEGORY, m_iCategory);}


BEGIN_MESSAGE_MAP(CXTPCustomizeKeyboardPage, CPropertyPage)
	//{{AFX_MSG_MAP(CXTPCustomizeKeyboardPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(XTP_IDC_COMBO_CATEGORY, OnSelchangeCategory)
	ON_LBN_SELCHANGE(XTP_IDC_LBOX_ACCEL_COMMANDS, OnSelchangeCommands)
	ON_LBN_SELCHANGE(XTP_IDC_LBOX_KEYS, OnSelchangeCurKeys)
	ON_BN_CLICKED(XTP_IDC_BTN_ASSIGN, OnAssign)
	ON_BN_CLICKED(XTP_IDC_BTN_ACCEL_RESET, OnResetAll)
	ON_BN_CLICKED(XTP_IDC_BTN_REMOVE, OnRemove)
	ON_EN_CHANGE(XTP_IDC_EDIT_SHORTCUTKEY, OnChangeShortcutKey)
END_MESSAGE_MAP()

XTP_COMMANDBARS_CATEGORYINFO* CXTPCustomizeKeyboardPage::FindCategory(CString strCategory)
{
	for (int i = 0; i < m_arrCategories.GetSize(); i++)
	{
		if (m_arrCategories[i]->strCategory == strCategory)
			return m_arrCategories[i];
	}
	return NULL;
}
CXTPControls* CXTPCustomizeKeyboardPage::InsertCategory(CString strCategory, int nIndex)
{
	XTP_COMMANDBARS_CATEGORYINFO* pInfo = FindCategory(strCategory);
	if (!pInfo)
	{
		pInfo = new XTP_COMMANDBARS_CATEGORYINFO(strCategory, m_pSheet->GetCommandBars());
		m_arrCategories.InsertAt(nIndex == -1? m_arrCategories.GetSize(): nIndex, pInfo);
	}
	return pInfo->pControls;
}


BOOL CXTPCustomizeKeyboardPage::AddCategory(CString strCategory, CMenu* pMenu, BOOL bListSubItems)
{
	CXTPPaintManager::StripMnemonics(strCategory);

	CXTPControls* pCategoryControls = InsertCategory(strCategory);

	int nCount = pMenu->GetMenuItemCount();

	for (int i = 0; i < nCount; i++)
	{
		if (pMenu->GetMenuItemID(i) > 0)
		{
			CXTPControlPopup* pControl = DYNAMIC_DOWNCAST(CXTPControlPopup,
				pCategoryControls->AddMenuItem(pMenu, i));

			if (pControl && bListSubItems)
			{
				CXTPControls* pControls = pControl->GetCommandBar()->GetControls();
				for (int j = 0; j < pControls->GetCount(); j++)
				{
					pCategoryControls->AddClone(pControls->GetAt(j));
				}
			}
		}
	}

	return TRUE;
}

BOOL CXTPCustomizeKeyboardPage::AddCategories(UINT nIDResource, BOOL bListSubItems)
{
	CMenu menu;
	if (!XTPResourceManager()->LoadMenu(&menu, nIDResource))
		return FALSE;

	int nCount = menu.GetMenuItemCount();

	for (int i = 0; i < nCount; i++)
	{
		CString strCategory;
		if (menu.GetMenuString(i, strCategory, MF_BYPOSITION) > 0)
		{
			CMenu* pMenu = menu.GetSubMenu(i);
			if (pMenu)
			{
				if (!AddCategory(strCategory, pMenu, bListSubItems))
					return FALSE;
			}
		}

	}
	return TRUE;
}

BOOL CXTPCustomizeKeyboardPage::AddCategories(CXTPControls* pControls)
{
	for (int i = 0; i < pControls->GetCount(); i++)
	{
		CXTPControl* pControl = pControls->GetAt(i);
		CString strCategory = pControl->GetCategory();

		if (!strCategory.IsEmpty() && pControl->GetID() > 0 &&
			pControl->GetType() == xtpControlButton )
		{
			CXTPControls* pCategoryControls = InsertCategory(strCategory);
			pCategoryControls->AddClone(pControl);
		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPCustomizeKeyboardPage message handlers

BOOL CXTPCustomizeKeyboardPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	for (int i = 0; i < m_arrCategories.GetSize(); i++)
	{
		int nIndex = m_comboCategory.AddString(m_arrCategories[i]->strCategory);
		m_comboCategory.SetItemDataPtr(nIndex, m_arrCategories[i]->pControls);

		if (i == 0) m_comboCategory.SetCurSel(0);
	}

	OnSelchangeCategory();


	return FALSE;
}

void CXTPCustomizeKeyboardPage::ReloadList()
{
	int iIndex = m_comboCategory.GetCurSel( );
	if( iIndex == LB_ERR )
		return;

	CXTPControls* pControls =  (CXTPControls*)m_comboCategory.GetItemDataPtr( iIndex );
	if ( pControls == NULL)
		return;

	m_lboxCommands.ResetContent();

	int iItem;
	for ( iItem = 0; iItem < pControls->GetCount(); ++iItem )
	{
		CXTPControl* pControl = pControls->GetAt(iItem);
		UINT nID = pControl->GetID();

		if ( nID != 0 && !IsPopupControlType(pControl->GetType()))
		{
			if ( ( nID < ID_FILE_MRU_FIRST ) || ( nID > ID_FILE_MRU_LAST ) )
			{
				CString str = pControl->GetCaption();
				CXTPPaintManager::StripMnemonics(str);
				int iIndex = m_lboxCommands.AddString( str );
				m_lboxCommands.SetItemDataPtr( iIndex, pControl );
			}
		}
	}

	if ( m_lboxCommands.GetCount( ) )
		m_lboxCommands.SetCurSel( 0 );
}

void CXTPCustomizeKeyboardPage::OnChangeShortcutKey()
{
	EnableControls( );
}

void CXTPCustomizeKeyboardPage::OnSelchangeCurKeys()
{
	EnableControls( );
}

void CXTPCustomizeKeyboardPage::OnSelchangeCategory()
{
	UpdateData( );
	ReloadList( );
	OnSelchangeCommands( );
	EnableControls( );
}

HACCEL CXTPCustomizeKeyboardPage::GetFrameAccelerator()
{
	CFrameWnd* pFrame = DYNAMIC_DOWNCAST(CFrameWnd, m_pSheet->GetCommandBars()->GetSite());
	if (!pFrame)
		return NULL;

	return pFrame->m_hAccelTable;
}


void CXTPCustomizeKeyboardPage::OnSelchangeCommands()
{
	ASSERT(m_pSheet);

	m_lboxKeys.ResetContent( );

	int iIndex = m_lboxCommands.GetCurSel( );
	if ( iIndex == -1 )
	{
		return;
	}

	CXTPControl* pControl = (CXTPControl*)m_lboxCommands.GetItemDataPtr( iIndex );
	ASSERT(pControl);

	HACCEL hAccelTable = GetFrameAccelerator();
	if (hAccelTable)
	{
		int nAccelSize = ::CopyAcceleratorTable (hAccelTable, NULL, 0);

		LPACCEL lpAccel = new ACCEL [nAccelSize];
		::CopyAcceleratorTable (hAccelTable, lpAccel, nAccelSize);

		for (int i = 0; i < nAccelSize; i ++)
		{
			if (lpAccel [i].cmd == pControl->GetID())
			{
				CString str;
				CXTPShortcutManager::CKeyHelper helper (&lpAccel [i]);
				helper.Format (str);

				int nIndex = m_lboxKeys.AddString(str);
				m_lboxKeys.SetItemData(nIndex, i);
			}
		}
		delete[] lpAccel;
	}

	m_strDesc = pControl->GetDescription();

	UpdateData( FALSE );
	EnableControls( );
}

void CXTPCustomizeKeyboardPage::UpdateAcellTable(LPACCEL lpAccel, int nSize)
{
	XTPShortcutManager()->UpdateAcellTable(DYNAMIC_DOWNCAST(CFrameWnd, m_pSheet->GetCommandBars()->GetSite()), lpAccel, nSize);
}

void CXTPCustomizeKeyboardPage::OnAssign()
{
	int iIndex = m_lboxCommands.GetCurSel( );
	if ( iIndex == -1 )
		return;

	CXTPControl* pControl = (CXTPControl*)m_lboxCommands.GetItemDataPtr( iIndex );

	const ACCEL* pAccel = m_editShortcutKey.GetAccel();
	ASSERT(pAccel);

	HACCEL hAccelTable = GetFrameAccelerator();

	int nAccelSize = ::CopyAcceleratorTable (hAccelTable, NULL, 0);

	LPACCEL lpAccel = new ACCEL [nAccelSize + 1];
	::CopyAcceleratorTable (hAccelTable, lpAccel, nAccelSize);

	int nIndex = nAccelSize;

	for (int i = 0; i < nAccelSize; i++)
	{
		if (CXTPShortcutManager::CKeyHelper::EqualAccels(&lpAccel[i], pAccel))
		{
			nIndex = i;

			if ( XTPResourceManager()->ShowMessageBox( XTP_IDS_CONFIRM_REASSIGN, MB_ICONWARNING|MB_YESNO ) != IDYES )
			{
				delete[] lpAccel;
				return;
			}
			break;
		}
	}

	lpAccel[nIndex] = *pAccel;
	lpAccel[nIndex].cmd = (WORD)pControl->GetID();

	UpdateAcellTable(lpAccel, nAccelSize + (nIndex == nAccelSize ? 1 : 0));

	delete[] lpAccel;

	OnSelchangeCommands( );
	m_editShortcutKey.SetWindowText( _T( "" ) );
	EnableControls( );
}

void CXTPCustomizeKeyboardPage::EnableControls()
{
	CString strText;
	m_editShortcutKey.GetWindowText( strText );

	CWnd* pWnd = GetDlgItem( XTP_IDC_BTN_ASSIGN );
	if ( pWnd )
	{
		pWnd->EnableWindow( !strText.IsEmpty( ) && m_editShortcutKey.IsKeyDefined( ) );
	}

	int iSel = m_lboxKeys.GetCurSel( );

	pWnd = GetDlgItem( XTP_IDC_BTN_REMOVE );
	if ( pWnd )
	{
		pWnd->EnableWindow( iSel != LB_ERR );
	}
}

void CXTPCustomizeKeyboardPage::OnResetAll()
{
	USES_PROTECTED_ACCESS(CXTPCustomizeKeyboardPage, CFrameWnd, UINT, m_nIDHelp)

	CXTPCommandBars* pCommandBars = m_pSheet->GetCommandBars();
	CFrameWnd* pFrame = DYNAMIC_DOWNCAST(CFrameWnd, pCommandBars->GetSite());
	ASSERT(pFrame);

	UINT nIDHelp = PROTECTED_ACCESS(CFrameWnd, pFrame, m_nIDHelp);

	if (nIDHelp == 0 && pCommandBars->m_hDesignerAccels == 0)
		return;

	if ( XTPResourceManager()->ShowMessageBox( XTP_IDS_CONFIRM_KEYBOARD, MB_ICONWARNING|MB_YESNO ) == IDYES )
	{

		HACCEL hAccelTable = pFrame->m_hAccelTable;

		if (pCommandBars->m_hDesignerAccels == 0)
		{
			pFrame->m_hAccelTable = 0;
			pFrame->LoadAccelTable(MAKEINTRESOURCE(nIDHelp));
		} else
		{
			pFrame->m_hAccelTable = XTPShortcutManager()->CopyAccelTable(pCommandBars->m_hDesignerAccels);
		}

		if (hAccelTable) ::DestroyAcceleratorTable (hAccelTable);

		OnSelchangeCommands( );
		EnableControls( );
	}
}

void CXTPCustomizeKeyboardPage::OnRemove()
{
	int iIndex = m_lboxKeys.GetCurSel( );
	if ( iIndex == LB_ERR )
		return;

	HACCEL hAccelTable = GetFrameAccelerator();
	if (!hAccelTable)
		return;

	int nAccelSize = ::CopyAcceleratorTable (hAccelTable, NULL, 0);

	LPACCEL lpAccel = new ACCEL [nAccelSize];
	::CopyAcceleratorTable (hAccelTable, lpAccel, nAccelSize);

	int j = (int)m_lboxKeys.GetItemData(iIndex);
	ASSERT( j >= 0 && j < nAccelSize );

	if (j >= 0 && j < nAccelSize)
	{
		if (j != nAccelSize - 1)
			lpAccel[j] = lpAccel[nAccelSize - 1];

		UpdateAcellTable(lpAccel, nAccelSize - 1);
	}

	delete[] lpAccel;


	OnSelchangeCommands( );
	EnableControls( );
}

BOOL CXTPCustomizeKeyboardPage::OnSetActive()
{
	UpdateData( FALSE );
	ReloadList( );
	EnableControls( );

	return CPropertyPage::OnSetActive();
}
