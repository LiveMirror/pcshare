// EmbeddedFrame.cpp : implementation file
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
#include "CommandBarsDesigner.h"
#include "EmbeddedFrame.h"
#include "MainFrm.h"
#include "CommandBarsDesignerDoc.h"
#include "CommandBarsDesignerView.h"

#include "DialogPropertyPages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmbeddedFrame

IMPLEMENT_DYNCREATE(CEmbeddedFrame, CFrameWnd)

CEmbeddedFrame::CEmbeddedFrame()
{
	m_pHostView = NULL;
	m_pControls = new CXTPControls;
	m_bCustomizable = TRUE;
}

CEmbeddedFrame::~CEmbeddedFrame()
{
	delete m_pControls;
}


BEGIN_MESSAGE_MAP(CEmbeddedFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CEmbeddedFrame)
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XTP_DRAGCONTROLCHANGED, OnDragControlChanged)
	ON_COMMAND_RANGE(1, 8000, OnSimpleCommand)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmbeddedFrame message handlers

int CEmbeddedFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndView.Create(_T("STATIC"), NULL, WS_CHILD|WS_VISIBLE,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}


	if (!InitCommandBars())
		return -1;

	CXTPCommandBars* pCommandBars = GetCommandBars();

// Set Special Paint Manager
	pCommandBars->SetPaintManager(new CXTPOfficeTheme());
	m_theme = xtpThemeOfficeXP;


// Set Special Image Manager
	CXTPImageManager* pImagerManager = new CXTPImageManager();
	pImagerManager->SetMaskColor(RGB(0, 0xFF, 0));
	pCommandBars->SetImageManager(pImagerManager);

	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_EMBEDDEDFRAME);
	pMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage);

	pImagerManager->RemoveAll();

	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_EMBEDDEDFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}


	m_pControls->SetCommandBars(pCommandBars);

	BuildControls();


	pCommandBars->SetCustomizeMode(TRUE);

	return 0;
}

void CEmbeddedFrame::Clear()
{
	CXTPCommandBars* pCommandBars = GetCommandBars();

	for (int i = pCommandBars->GetCount() - 1; i >= 0; i--)
	{
		pCommandBars->Remove(pCommandBars->GetAt(i));
	}


	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_EMBEDDEDFRAME);
	pMenuBar->GetControls()->RemoveAll();

	::DestroyAcceleratorTable (m_hAccelTable);
	m_hAccelTable = 0;

	pCommandBars->GetImageManager()->RemoveAll();
	m_pControls->RemoveAll();
	m_resourceManager.FreeAll();

}


BOOL CEmbeddedFrame::AddCategory(CString strCategory, CMenu* pMenu)
{
	CXTPPaintManager::StripMnemonics(strCategory);

	int nCount = pMenu->GetMenuItemCount();

	for (int i = 0; i < nCount; i++)
	{
		if (pMenu->GetMenuItemID(i) > 0)
		{
			CXTPControl* pControl = m_pControls->AddMenuItem(pMenu, i);
			if (pControl) pControl->SetCategory(strCategory);
		}
	}

	return TRUE;
}


BOOL CEmbeddedFrame::BuildControls()
{
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_EMBEDDEDFRAME));

	int nCount = menu.GetMenuItemCount();

	for (int i = 0; i < nCount; i++)
	{
		CString strCategory;
		if (menu.GetMenuString(i, strCategory, MF_BYPOSITION) > 0)
		{
			CMenu* pMenu = menu.GetSubMenu(i);
			if (pMenu)
			{
				if (!AddCategory(strCategory, pMenu))
					return FALSE;
			}
		}

	}

	CString strBuiltInMenus;
	VERIFY(strBuiltInMenus.LoadString(XTP_IDS_BUILTINMENUS));

	CXTPCommandBar* pCommandBar = GetCommandBars()->GetMenuBar();

	for (int j = 0; j < pCommandBar->GetControls()->GetCount(); j++)
	{
		CXTPControl* pControl = m_pControls->AddClone(pCommandBar->GetControl(j));
		pControl->SetCategory(strBuiltInMenus);
	}
	return TRUE;
}


BOOL CEmbeddedFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	cs.style     |=  WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	cs.lpszClass = AfxRegisterWndClass( 0, NULL, NULL,
		AfxGetApp()->LoadIcon(IDI_FORM));
	ASSERT(cs.lpszClass);

	return TRUE;
}

UINT CEmbeddedFrame::OnNcHitTest(CPoint /*point*/)
{
	return HTCLIENT;
}



LRESULT CEmbeddedFrame::OnDragControlChanged(WPARAM, LPARAM)
{
	if (!AfxGetMainWnd())
		return 0;

	((CMainFrame*)AfxGetMainWnd())->m_pActiveCommandBar = NULL;
	((CMainFrame*)AfxGetMainWnd())->RefreshPanes();


	if (m_pHostView)
	{
		m_pHostView->GetDocument()->SetModifiedFlag();

	}
	return 0;
}

void CEmbeddedFrame::ReplaceControls(CXTPToolBar* pCommandBar, CXTPControls* pControls)
{
	class CCommamndBarFriendly : public CXTPToolBar
	{
		friend class CEmbeddedFrame;
	};

	if (((CCommamndBarFriendly*)pCommandBar)->m_pControls != NULL)
	{
		((CCommamndBarFriendly*)pCommandBar)->m_pControls->InternalRelease();
	}

	((CCommamndBarFriendly*)pCommandBar)->m_pControls = pControls;
	if (pControls) pControls->InternalAddRef();

}


void CEmbeddedFrame::Serialize(CArchive& ar)
{
	CXTPPropExchangeArchive px(ar);
	DoPropExchange(&px);

}

void CEmbeddedFrame::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	CXTPImageManager* pImageManager = pCommandBars->GetImageManager();

	ASSERT(m_pHostView);

	if (pPX->IsStoring())
	{

		for (int i = 0; i < pCommandBars->GetCount(); i++)
		{
			CXTPToolBar* pToolBar = pCommandBars->GetAt(i);

			pToolBar->GetControls()->ClearOriginalControls();
		}

// Save Docking Position and Controls
		CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Controls"), xtpBarFloating);
		ReplaceControls(pCommandBar, m_pControls);
		pCommandBar->SetBarID(0xFFEEFF);
		pCommandBar->SetVisible(FALSE);

		XTP_COMMANDBARS_PROPEXCHANGE_PARAM param;
		param.bSerializeControls = TRUE;
		param.bSaveOnlyCustomized = FALSE;
		pCommandBars->DoPropExchange(pPX, &param);

		pCommandBars->Remove(pCommandBar);

	} else
	{
		for (int i = pCommandBars->GetCount() - 1; i >= 0; i--)
		{
			pCommandBars->Remove(pCommandBars->GetAt(i));
		}

// Load Docking Position and Controls
		XTP_COMMANDBARS_PROPEXCHANGE_PARAM param;
		param.bSerializeControls = TRUE;
		pCommandBars->DoPropExchange(pPX, &param);


		CXTPToolBar* pCommandBar = pCommandBars->GetToolBar(0xFFEEFF);

		if (pCommandBar)
		{
			m_pControls->InternalRelease();
			m_pControls = pCommandBar->GetControls();
			m_pControls->InternalAddRef();

			pCommandBars->Remove(pCommandBar);
		}
		else
		{
			m_pControls->RemoveAll();

		}

		m_pControls->SetParent(NULL);

		m_pControls->Remove(m_pControls->FindControl(xtpControlError, XTP_ID_TOOLBAR_EXPAND, FALSE, FALSE));
		m_pControls->Remove(m_pControls->FindControl(xtpControlError, XTP_ID_TOOLBAR_HIDE, FALSE, FALSE));
		m_pControls->SetCommandBars(pCommandBars);

	}
	// Icons
	CXTPPropExchangeSection secImages(pPX->GetSection(_T("Images")));
	pImageManager->DoPropExchange(&secImages);

	// Accelerators:
	CXTPPropExchangeSection secShortcuts(pPX->GetSection(_T("Shortcuts")));
	XTPShortcutManager()->DoPropExchange(&secShortcuts, this);

	// Options:
	CXTPPropExchangeSection secOptions(pPX->GetSection(_T("Options")));
	PX_Enum(&secOptions, _T("Theme"), m_theme, xtpThemeOffice2000);
	if (pPX->IsLoading()) SetTheme(m_theme);
	pCommandBars->GetCommandBarsOptions()->DoPropExchange(&secOptions);

	// m_bCustomizable
	PX_Bool(&secOptions, _T("Customizable"), m_bCustomizable, TRUE);


	// Rectangle
	PX_Rect(&secOptions, _T("RectTracker"), m_pHostView->m_rectTracker.m_rect, CRect(10, 10, 310, 289));

	// Resource Manager
	CXTPPropExchangeSection secResourceManager(pPX->GetSection(_T("ResourceManager")));
	m_resourceManager.DoPropExchange(&secResourceManager);
}


void CEmbeddedFrame::ShowPropertyPage()
{
	CDialogPropertyPages dp(GetCommandBars());
	dp.DoModal();

}

void CEmbeddedFrame::SetTheme(XTPPaintTheme theme)
{
	m_theme = theme;

	CXTPPaintManager* pPaintManager = CXTPPaintManager::CreateTheme(theme);

	GetCommandBars()->SetPaintManager(pPaintManager);

}

void CEmbeddedFrame::OnSimpleCommand(UINT nID)
{
	CResourceInfo* pInfo = m_resourceManager.Find(nID);
	if (pInfo)
	{
		AfxMessageBox(pInfo->m_strCaption);
	}

}
