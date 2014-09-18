// MainFrm.cpp : implementation of the CMainFrame class
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
#include "MultiLanguage.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_LANGAUGE_FILE 1001
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP

	ON_COMMAND_EX_RANGE(IDC_LANG_ARABIC, IDC_LANG_UKRAINIAN, OnLangaugeFile)
	ON_UPDATE_COMMAND_UI_RANGE(IDC_LANG_ARABIC, IDC_LANG_UKRAINIAN, OnUpdateLangaugeFile)

	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_XTP_INITCOMMANDSPOPUP()

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	VERIFY(GetVersionEx(&osvi));

	m_bIsWin2000OrLater = (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion > 4);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create Status bar.
	// Important: All control bars including the Status Bar
	// must be created before CommandBars....
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Initialize the command bars
	if (!InitCommandBars())
		return -1;

	XTPImageManager()->SetMaskColor(XTPImageManager()->GetBitmapMaskColor(IDR_MAINFRAME));

	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars == NULL)
	{
		TRACE0("Failed to create command bars object.\n");
		return -1;      // fail to create
	}

	// Add the menu bar
	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(
		_T("Menu Bar"), IDR_MULTILTYPE);
	if(pMenuBar == NULL)
	{
		TRACE0("Failed to create menu bar.\n");
		return -1;      // fail to create
	}
	pMenuBar->SetFlags(xtpFlagAddMDISysPopup);
	pMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage);

	// Create ToolBar
	CXTPToolBar* pToolBar = (CXTPToolBar*)
		pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pToolBar || !pToolBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	// Create ToolBar
	CXTPToolBar* pLangBar = (CXTPToolBar*)
		pCommandBars->Add(_T("Language"), xtpBarTop);
	if (!pLangBar || !pLangBar->LoadToolBar(IDR_LANGBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	// Set Office 2003 Theme
	CXTPPaintManager::SetTheme(xtpThemeOffice2003);

	// Initialize the docking pane manager and set the
	// initial them for the docking panes.  Do this only after all
	// control bars objects have been created and docked.
	m_paneManager.InstallDockingPanes(this);
	// Set Office 2003 Theme
	m_paneManager.SetTheme(xtpPaneThemeOffice2003);

	// Create docking panes.
	CXTPDockingPane* pwndPane1 = m_paneManager.CreatePane(
		IDR_PANE_OPTIONS, CRect(0, 0,200, 120), xtpPaneDockLeft);
	m_paneManager.CreatePane(
		IDR_PANE_PROPERTIES, CRect(0, 0,200, 120), xtpPaneDockBottom, pwndPane1);

	// Set the icons for the docking pane tabs.
	int nIDIcons[] = {IDR_PANE_OPTIONS, IDR_PANE_PROPERTIES};
		m_paneManager.SetIcons(IDB_BITMAP_ICONS, nIDIcons,
		_countof(nIDIcons), RGB(0, 255, 0));

	// Load the previous state for docking panes.
	CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	if (layoutNormal.Load(_T("NormalLayout")))
	{
		m_paneManager.SetLayout(&layoutNormal);
	}

	// Load the previous state for toolbars and menus.
	LoadCommandBars(_T("CommandBars"), TRUE);

	// Load previously selected language.
	OnLangaugeFile(m_langManager.GetCmdID(m_langManager.GetLangID()));

	return 0;
}

void CMainFrame::OnClose()
{
	// Save the current state for toolbars and menus.
	SaveCommandBars(_T("CommandBars"));

	// Save the current state for docking panes.
	CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	m_paneManager.GetLayout(&layoutNormal);
	layoutNormal.Save(_T("NormalLayout"));

	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnCustomize()
{
	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars != NULL)
	{
		// Instantiate the customize dialog object.
		CXTPCustomizeSheet dlg(pCommandBars);

		// Add the options page to the customize dialog.
		CXTPCustomizeOptionsPage pageOptions(&dlg);
		dlg.AddPage(&pageOptions);

		// Add the commands page to the customize dialog.
		CXTPCustomizeCommandsPage* pCommands = dlg.GetCommandsPage();
		pCommands->AddCategories(IDR_MULTILTYPE);

		// Use the command bar manager to initialize the
		// customize dialog.
		pCommands->InsertAllCommandsCategory();
		pCommands->InsertBuiltInMenus(IDR_MULTILTYPE);
		pCommands->InsertNewMenuCategory();

		// Display the dialog.
		dlg.DoModal();
	}
}


CWnd* CMainFrame::CreatePropertyView()
{
	if (m_wndPropertyGrid.GetSafeHwnd() == 0)
	{
		m_wndPropertyGrid.Create( CRect(0, 0, 0, 0), this, 0 );
		m_wndPropertyGrid.SetTheme(xtpGridThemeOffice2003);
		//m_wndPropertyGrid.ShowToolBar(TRUE);
		m_wndPropertyGrid.SetOwner(this);

		CFont font;
		font.CreatePointFont(60, _T("Marlett"));

		LOGFONT lf;
		font.GetLogFont(&lf);

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
		CXTPPropertyGridItem* pItemLanguage = pVersion->AddChildItem(new CXTPPropertyGridItem(_T("Language"), _T("English (United States)")));

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

		// create custom items category.
		CXTPPropertyGridItem* pCustom   = m_wndPropertyGrid.AddCategory(_T("Custom Items"));

		// add multi level tree node.
		CXTPPropertyGridItem* pItemOne   = pCustom->AddChildItem(new CXTPPropertyGridItem(_T("First Level"), _T("")));
		CXTPPropertyGridItem* pItemTwo   = pItemOne->AddChildItem(new CXTPPropertyGridItem(_T("Second Level"), _T("")));
		CXTPPropertyGridItem* pItemThird = pItemTwo->AddChildItem(new CXTPPropertyGridItem(_T("Third Level"), _T("")));
		pItemThird->AddChildItem(new CXTPPropertyGridItem(_T("Fourth Level 1"), _T("")));
		pItemThird->AddChildItem(new CXTPPropertyGridItem(_T("Fourth Level 2"), _T("")));
	}

	return &m_wndPropertyGrid;
}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

		if (!pPane->IsValid())
		{
			switch (pPane->GetID())
			{
			case IDR_PANE_PROPERTIES:
				{

					pPane->Attach(CreatePropertyView());
					break;
				}
			case IDR_PANE_OPTIONS:
				{
					if (m_wndOptions.GetSafeHwnd() == 0)
					{
						m_wndOptions.Create(_T("\n\nOptions"),
							WS_CHILD|WS_CLIPCHILDREN|
							WS_CLIPSIBLINGS|SS_CENTER,
							CRect(0, 0, 0, 0), this, 0);
					}
					pPane->Attach(&m_wndOptions);
					break;
				}
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnInitCommandsPopup(CXTPPopupBar* pCommandBar)
{
	if (pCommandBar->GetControls()->GetCount() > 0 &&
		pCommandBar->GetControl(0)->GetID() == ID_LANGAUGE_NOFOUND)
	{
		while (pCommandBar->GetControls()->GetCount() > 1)
			pCommandBar->GetControls()->Remove(1);

		CMapLangDll& mapLangDll = m_langManager.GetLangMap();
		for (POSITION pos = mapLangDll.GetStartPosition(); pos;)
		{
			WORD wLangID;
			LANGUAGE_DLL langDll;
			mapLangDll.GetNextAssoc(pos, wLangID, langDll);

			CXTPControl* pControl = pCommandBar->GetControls()->Add(
				xtpControlButton, langDll.uCmdID, _T(""), -1, TRUE);

			if (pControl)
			{
				CString strDescription = m_langManager.GetDescription(wLangID);
				if (strDescription.IsEmpty())
				{
					strDescription.Format(_T("(dll %d corrupt or missing) "), m_langManager.GetLangID());
					int iIndex = langDll.strDllPath.ReverseFind(_T('\\'))+1;
					strDescription += langDll.strDllPath.Right(langDll.strDllPath.GetLength()-iIndex);
				}

				pControl->SetCaption(strDescription);
				pControl->SetParameter(langDll.strDllPath);
				pCommandBar->GetControl(0)->SetVisible(FALSE);
			}
		}
	}
}

void CMainFrame::ResetCommandBars()
{
	m_bInRecalcLayout = TRUE;

	CXTPCommandBars* pCommandBars = GetCommandBars();
	for (int i = 0; i < pCommandBars->GetCount(); i++)
	{
		CXTPToolBar* pToolBar = pCommandBars->GetAt(i);
		if (pToolBar->IsBuiltIn())
		{
			if (pToolBar->GetType() != xtpBarTypeMenuBar)
			{
				pToolBar->LoadToolBar(pToolBar->GetBarID(), FALSE);
			}
			else
			{
				CMenu menu;
				menu.LoadMenu(IDR_MULTILTYPE);

				pToolBar->LoadMenu(&menu);
				pToolBar->GetControls()->CreateOriginalControls();
				((CXTPMenuBar*)pToolBar)->RefreshMenu();
			}
		}

		if (pToolBar->GetBarID() == IDR_LANGBAR)
		{
			CXTPControls* pCommandList = pToolBar->GetControls();
			for (int iIndex = pCommandList->GetCount()-1; iIndex >= 0; --iIndex)
			{
				CXTPControl* pButton = pCommandList->GetAt(iIndex);
				if (pButton)
				{
					WORD wLangID = m_langManager.GetLangID(pButton->GetID());
					pButton->SetCaption(m_langManager.GetDescription(wLangID));

					LANGUAGE_DLL langDll;
					if (!m_langManager.GetDllInfo(wLangID, langDll))
						pCommandList->Remove(iIndex);
				}
			}
		}
	}

	m_bInRecalcLayout = FALSE;
	RecalcLayout();
	AfxGetApp()->OnIdle(0);

	RedrawWindow(NULL, NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
}

BOOL CMainFrame::OnLangaugeFile(UINT nID)
{
	LANGUAGE_DLL langDll;
	if (m_langManager.GetDllInfo(m_langManager.GetLangID(nID), langDll))
	{
		if (m_langManager.LoadLanguageDll(langDll.strDllPath))
		{
			BOOL bRTLLayout = ((::GetWindowLong(m_hWnd, GWL_EXSTYLE)
				& WS_EX_LAYOUTRTL) == WS_EX_LAYOUTRTL);

			if (bRTLLayout != m_langManager.IsRTL())
			{
				m_langManager.SetRTLLayout(this, m_langManager.IsRTL());

				XTPImageManager()->DrawReverted(m_langManager.IsRTL());
			}

			ResetCommandBars();
			return TRUE;
		}
	}
	return FALSE;
}


BOOL CMainFrame::IsUnicodeAndLayoutSupport()
{
#ifdef _UNICODE
	return m_bIsWin2000OrLater;
#else
	return FALSE;
#endif	
}

void CMainFrame::OnUpdateLangaugeFile(CCmdUI* pCmdUI)
{
	if (!IsUnicodeAndLayoutSupport())
	{
		switch (pCmdUI->m_nID)
		{
		case IDC_LANG_ARABIC:
		case IDC_LANG_CHINESE_PRC:
		case IDC_LANG_CHINESE_TAIWAN:
		case IDC_LANG_GREEK:
		case IDC_LANG_HEBREW:
		case IDC_LANG_JAPANESE:
		case IDC_LANG_KOREAN:
		case IDC_LANG_THAI:
			{
				pCmdUI->Enable(FALSE);
				return;
			}
		}

	}

	WORD wLangID = m_langManager.GetLangID(pCmdUI->m_nID);

	LANGUAGE_DLL langDll;
	if (m_langManager.GetDllInfo(wLangID, langDll))
	{
		pCmdUI->SetCheck(wLangID == m_langManager.GetLangID());
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}


void CMainFrame::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
{

	// Toggle CommandBars
	GetCommandBars()->OnSetPreviewMode(bPreview);

// Toggle Docking Panes.
	m_paneManager.OnSetPreviewMode(bPreview);

	CMDIFrameWnd::OnSetPreviewMode(bPreview, pState);
}