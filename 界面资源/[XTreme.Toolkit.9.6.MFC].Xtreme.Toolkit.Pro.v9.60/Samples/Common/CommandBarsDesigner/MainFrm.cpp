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
#include "CommandBarsDesigner.h"


#include "MainFrm.h"


#include "CommandBarsDesignerDoc.h"
#include "CommandBarsDesignerView.h"
#include "ChildFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_PROPERTIES, OnViewProperties)
	ON_COMMAND(ID_VIEW_ACCELERATORS, OnViewAccelerators)
	ON_COMMAND(ID_VIEW_CONTROLSPANE, OnViewControlspane)
	ON_COMMAND(ID_VIEW_ICONSPANE, OnViewIconspane)
	ON_COMMAND(ID_VIEW_TOOLBARSPANE, OnViewToolbarspane)
	ON_COMMAND(ID_FILE_IMPORT_XMLDOCUMENT, OnFileImportXmldocument)
	ON_UPDATE_COMMAND_UI(ID_FILE_IMPORT_XMLDOCUMENT, OnUpdateFileImportXmldocument)
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static int nIDPaneIcons[] =
{
	IDR_PANE_TOOLBARS, IDR_PANE_PROPERTIES, IDR_PANE_CONTROLS, IDR_PANE_ACCELERATORS, IDR_PANE_ICONS
};
static UINT nIDCommandsIcons[] =
{
	ID_VIEW_TOOLBARSPANE, ID_VIEW_PROPERTIES, ID_VIEW_CONTROLSPANE, ID_VIEW_ACCELERATORS, ID_VIEW_ICONSPANE
};


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_pActiveCommandBars = NULL;
	m_pActiveCommandBar = NULL;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!InitCommandBars())
		return -1;

	CXTPPaintManager::SetTheme(xtpThemeWhidbey);


	CXTPCommandBars* pCommandBars = GetCommandBars();

	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);
	pMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage|xtpFlagHideMaximizeBox|xtpFlagHideMinimizeBox);

	XTPImageManager()->SetMaskColor(RGB(0, 0xFF, 0));

	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	XTPImageManager()->SetIcons(IDB_BITMAP_PANEICONS, nIDCommandsIcons, _countof(nIDCommandsIcons), CSize(16, 16));


	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeWhidbey);

	// Create docking panes.
	CXTPDockingPane* pPaneToolbars = m_paneManager.CreatePane(
		IDR_PANE_TOOLBARS, CRect(0, 0,200, 120), xtpPaneDockRight);

	CXTPDockingPane* pPaneControls = m_paneManager.CreatePane(
		IDR_PANE_CONTROLS, CRect(0, 0,200, 120), xtpPaneDockRight);

	m_paneManager.AttachPane(pPaneControls, pPaneToolbars);


	m_paneManager.CreatePane(
		IDR_PANE_PROPERTIES, CRect(0, 0,200, 120), xtpPaneDockBottom, pPaneToolbars);

	CXTPDockingPane* pPaneAccelerators = m_paneManager.CreatePane(
		IDR_PANE_ACCELERATORS, CRect(0, 0,200, 120), xtpPaneDockLeft);

	CXTPDockingPane* pPaneIcons= m_paneManager.CreatePane(
		IDR_PANE_ICONS, CRect(0, 0,200, 120), xtpPaneDockLeft, pPaneAccelerators);

	m_paneManager.AttachPane(pPaneAccelerators, pPaneIcons);
	pPaneAccelerators->Hide();


	m_paneManager.SetIcons(IDB_BITMAP_PANEICONS, nIDPaneIcons, _countof(nIDPaneIcons), RGB(0, 255, 0));


	VERIFY(m_MTIClientWnd.Attach(this));

	// Load the previous state for docking panes.
	CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	if (layoutNormal.Load(_T("NormalLayout")))
	{
		if (layoutNormal.GetPaneList().GetCount() == 5)
		{
			m_paneManager.SetLayout(&layoutNormal);
		}
	}
	m_paneManager.SetAlphaDockingContext(TRUE);
	m_paneManager.SetShowDockingContextStickers(TRUE);

	// Load the previous state for toolbars and menus.
	LoadCommandBars(_T("CommandBars"));

	return 0;
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


void CMainFrame::OnDestroy()
{
	//m_MTIClientWnd.Detach();

	CMDIFrameWnd::OnDestroy();
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
				case IDR_PANE_TOOLBARS:
				{
					if (m_paneToolbars.GetSafeHwnd() == 0)
					{
						m_paneToolbars.Create(_T("STATIC"), NULL, WS_CHILD, CRect(0, 0, 0, 0), this, 0);
					}

					pPane->Attach(&m_paneToolbars);
					break;
				}
				case IDR_PANE_PROPERTIES:
				{
					if (m_paneProperties.GetSafeHwnd() == 0)
					{
						m_paneProperties.Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);
						XTPMouseManager()->AddTrustedWindow(m_paneProperties);
					}

					pPane->Attach(&m_paneProperties);
					break;
				}
				case IDR_PANE_CONTROLS:
				{
					if (m_paneControls.GetSafeHwnd() == 0)
					{
						m_paneControls.Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);
					}

					pPane->Attach(&m_paneControls);
					break;
				}
				case IDR_PANE_ACCELERATORS:
				{
					if (m_paneAccelerators.GetSafeHwnd() == 0)
					{
						m_paneAccelerators.Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);
					}

					pPane->Attach(&m_paneAccelerators);
					break;
				}
				case IDR_PANE_ICONS:
				{
					if (m_paneIcons.GetSafeHwnd() == 0)
					{
						m_paneIcons.Create(_T("STATIC"), NULL, WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);
					}

					pPane->Attach(&m_paneIcons);
					break;
				}
			}
			RefreshPanes();

		}

		return TRUE;
	}
	return FALSE;
}


void CMainFrame::RefreshPanes(BOOL bForceAll)
{
	if (!AfxGetMainWnd())
		return;

	if (m_pActiveCommandBars != GetActiveCommandBars())
	{
		m_pActiveCommandBar = NULL;
		m_pActiveCommandBars = GetActiveCommandBars();
	}

	if (m_paneToolbars.GetSafeHwnd()) m_paneToolbars.Refresh();
	if (m_paneProperties.GetSafeHwnd()) m_paneProperties.Refresh(m_pActiveCommandBar);
	if (m_paneControls.GetSafeHwnd()) m_paneControls.Refresh(bForceAll);
	if (m_paneAccelerators.GetSafeHwnd()) m_paneAccelerators.Refresh(bForceAll);
	if (m_paneIcons.GetSafeHwnd()) m_paneIcons.Refresh(bForceAll);

}

CEmbeddedFrame* CMainFrame::GetActiveEmbeddedFrame()
{
	CChildFrame* pChildFrame = DYNAMIC_DOWNCAST(CChildFrame, GetActiveFrame());
	if (!pChildFrame)
		return NULL;

	CCommandBarsDesignerDesignerView* pView = DYNAMIC_DOWNCAST(CCommandBarsDesignerDesignerView, pChildFrame->GetActiveView());
	if (!pView)
		return NULL;

	return  DYNAMIC_DOWNCAST(CEmbeddedFrame, pView->m_pFrame);
}

CXTPCommandBars* CMainFrame::GetActiveCommandBars()
{

	CEmbeddedFrame* pFrame = GetActiveEmbeddedFrame();

	return pFrame? pFrame->GetCommandBars(): NULL;
}

void CMainFrame::OnClose()
{
	CXTPDockingPaneLayout layoutNormal(&m_paneManager);
	m_paneManager.GetLayout(&layoutNormal);
	layoutNormal.Save(_T("NormalLayout"));

	// Save the current state for toolbars and menus.
	SaveCommandBars(_T("CommandBars"));

	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnViewProperties()
{
	m_paneManager.ShowPane(IDR_PANE_PROPERTIES);
}

void CMainFrame::OnViewAccelerators()
{
	m_paneManager.ShowPane(IDR_PANE_ACCELERATORS);
}

void CMainFrame::OnViewControlspane()
{
	m_paneManager.ShowPane(IDR_PANE_CONTROLS);
}

void CMainFrame::OnViewIconspane()
{
	m_paneManager.ShowPane(IDR_PANE_ICONS);
}

void CMainFrame::OnViewToolbarspane()
{
	m_paneManager.ShowPane(IDR_PANE_TOOLBARS);
}

void CMainFrame::OnCustomize()
{
	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars != NULL)
	{
		// Instanciate the customize dialog object.
		CXTPCustomizeSheet dlg(pCommandBars);

		// Add the options page to the customize dialog.
		CXTPCustomizeOptionsPage pageOptions(&dlg);
		dlg.AddPage(&pageOptions);

		// Add the commands page to the customize dialog.
		CXTPCustomizeCommandsPage* pCommands = dlg.GetCommandsPage();
		pCommands->AddCategories(IDR_MAINFRAME);

		// Use the command bar manager to initialize the
		// customize dialog.
		pCommands->InsertAllCommandsCategory();
		pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
		pCommands->InsertNewMenuCategory();

		// Display the dialog.
		dlg.DoModal();
	}
}

void CMainFrame::OnFileImportXmldocument()
{
	CString strFilter = _T("XML Document(*.xml)|*.xml|All files (*.*)|*.*||");

	CFileDialog fd(TRUE, _T("xml"), NULL, OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, strFilter);
	if (fd.DoModal() != IDOK)
		return;

	CXTPPropExchangeXMLNode px(TRUE, NULL, _T("DesignerFile"));
	if (!px.LoadFromFile(fd.GetPathName()))
		return;
	
	if (!px.OnBeforeExchange())
	{
		AfxMessageBox(_T("<DesignerFile> node not found"));
		return;
	}
	
	CDocument* pDocument = ((CCommandBarsDesignerApp*)AfxGetApp())->m_pDocTemplate->OpenDocumentFile(NULL);

	POSITION pos = pDocument->GetFirstViewPosition();
	CCommandBarsDesignerView* pView = (CCommandBarsDesignerView*)pDocument->GetNextView(pos);

	CEmbeddedFrame* pFrame = pView->GetEmbeddedFrame();

	pFrame->DoPropExchange(&px);

	pView->OnInitialUpdate();

	RefreshPanes(TRUE);

}

void CMainFrame::OnUpdateFileImportXmldocument(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsXMLEngineAvailable());
}