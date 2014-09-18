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
#include "GUI_Eclipse.h"

#include <math.h>
#include "MainFrm.h"
#include "AppearanceSetEclipse.h"

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
	ON_COMMAND(ID_RUN_EXTERNALTOOLS, OnEmptyCommand)
	ON_COMMAND(ID_VIEW_BASETHEME_OFFICE2000, OnViewBasethemeOffice2000)
	ON_COMMAND(ID_VIEW_BASETHEME_OFFICEXP, OnViewBasethemeOfficexp)
	ON_COMMAND(ID_VIEW_BASETHEME_OFFICE2003, OnViewBasethemeOffice2003)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)

	ON_XTP_CREATECONTROL()
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
	// TODO: add member initialization code here

}

CMainFrame::~CMainFrame()
{
}


void CreateGreyBitmap(UINT nID, CBitmap& bmp)
{
	if (!bmp.LoadBitmap(nID))
		return;

	CXTPCompatibleDC dc(NULL, &bmp);

	BITMAP bitmap;
	bmp.GetObject(sizeof (BITMAP), &bitmap);

	COLORREF clrMask = dc.GetPixel(0, 0);

	for(int i = 0; i < bitmap.bmWidth; i++)
	for(int j = 0; j < bitmap.bmHeight; j++)
	{
		COLORREF clr = dc.GetPixel(i, j);

		if (clr != clrMask)
		{
			double dGray = GetRValue(clr) * 0.299 + GetGValue(clr) * 0.587 + GetBValue(clr) * 0.114;
			int nGray = (BYTE)(pow(dGray / 255.0, 0.9) * 255.0);

			dc.SetPixel(i, j, RGB(nGray, nGray, nGray));
		}
	}
}
void CMainFrame::AddIcons(UINT nID)
{
	CBitmap bmp;
	CreateGreyBitmap(nID, bmp);
	XTPImageManager()->SetIcons(nID, bmp);

	CBitmap bmpHot;
	bmpHot.LoadBitmap(nID);
	XTPImageManager()->SetIcons(nID, bmpHot, xtpImageHot);
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

	CXTPPaintManager::SetCustomTheme(new CEclipseCommandBarsPaintManager<CXTPOfficeTheme>);

	CXTPCommandBars* pCommandBars = GetCommandBars();

	pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);


	CXTPToolBar* pStandardBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pStandardBar ||
		!pStandardBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	XTPPaintManager()->GetIconsInfo()->bUseFadedIcons = FALSE;
	XTPPaintManager()->GetIconsInfo()->bUseDisabledIcons = TRUE;
	XTPPaintManager()->GetIconsInfo()->bIconsWithShadow = FALSE;
	CXTPImageManager::m_dDisabledBrightnessFactor = 0.3;


	AddIcons(IDR_MAINFRAME);
	AddIcons(IDR_EXTENDED);


	m_MTIClientWnd.Attach(this);
	m_MTIClientWnd.SetPaintManager(new CEclipseTabPaintManager());

	//////////////////////////////////////////////////////////////////////////

	m_paneManager.InstallDockingPanes(this);

	// Create docking panes.
	CXTPDockingPane* pPaneNavigator = m_paneManager.CreatePane(
		ID_PANE_NAVIGATOR, CRect(0, 0,220, 120), xtpPaneDockLeft);

	CXTPDockingPane* pPaneTasks = m_paneManager.CreatePane(
		ID_PANE_TASKS, CRect(0, 0,220, 120), xtpPaneDockTop, pPaneNavigator);

	CXTPDockingPane* pPaneOutline = m_paneManager.CreatePane(
		ID_PANE_OUTLINE, CRect(0, 0,220, 120), xtpPaneDockLeft, pPaneNavigator);

	m_paneManager.AttachPane(pPaneTasks, pPaneNavigator);
	m_paneManager.AttachPane(pPaneOutline, pPaneNavigator);

	m_paneManager.CreatePane(ID_PANE_BOOKMARKS, CRect(0, 0,220, 120), xtpPaneDockBottom, pPaneNavigator);

	pPaneNavigator->Select();

	m_paneManager.SetCustomTheme(new CEclipseDockingPanePaintManager<CXTPDockingPaneOfficeTheme>());

	m_ilIcons.Create(IDB_PANEICONS, 16, 1, RGB(0, 0xFF, 0));

	int nIDPanes[] = {ID_PANE_NAVIGATOR, ID_PANE_BOOKMARKS, ID_PANE_TASKS, ID_PANE_OUTLINE};
	m_paneManager.SetIcons(IDB_PANEICONS, nIDPanes, 4, RGB(0, 255, 0));

	RecalcLayout();

	// Load the previous state for command bars.
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




void CMainFrame::OnEmptyCommand()
{
	// TODO: Add your command handler code here

}


int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->bToolBar)
	{
		CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreateControl->pCommandBar);
		if (!pToolBar)
			return FALSE;

		if (lpCreateControl->nID == ID_RUN_EXTERNALTOOLS)
		{
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}

		if (lpCreateControl->nID == ID_FILE_NEW_PROJECT)
		{
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}

		if (lpCreateControl->nID == ID_EDIT_SEARCH)
		{
			lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
			return TRUE;
		}

	}
	return FALSE;
}


LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		// get a pointer to the docking pane being shown.
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
		if (!pPane->IsValid())
		{
			switch(pPane->GetID())
			{
			case ID_PANE_NAVIGATOR:
				if (!::IsWindow(m_wndNavigator.m_hWnd))
				{
					if (!m_wndNavigator.Create(this))
					{
						TRACE0( "Error creating pane edit control.\n" );
					}
					m_wndNavigator.SetHostParams(pPane->GetID(), m_ilIcons.ExtractIcon(0));
				}

				pPane->Attach(&m_wndNavigator);

				break;
			case ID_PANE_OUTLINE:
				if (!::IsWindow(m_wndOutline.m_hWnd))
				{
					if (!m_wndOutline.Create(this))
					{
						TRACE0( "Error creating pane edit control.\n" );
					}
					m_wndOutline.SetHostParams(pPane->GetID(), m_ilIcons.ExtractIcon(3));
				}

				pPane->Attach(&m_wndOutline);

				break;
			case ID_PANE_TASKS:
				if (!::IsWindow(m_wndTasks.m_hWnd))
				{
					if (!m_wndTasks.Create(this))
					{
						TRACE0( "Error creating pane edit control.\n" );
					}
					m_wndTasks.SetHostParams(pPane->GetID(), m_ilIcons.ExtractIcon(2));
				}

				pPane->Attach(&m_wndTasks);

				break;
			case ID_PANE_BOOKMARKS:
				if (!::IsWindow(m_wndBookmarks.m_hWnd))
				{
					if (!m_wndBookmarks.Create(this))
					{
						TRACE0( "Error creating pane edit control.\n" );
					}
					m_wndBookmarks.SetHostParams(pPane->GetID(), m_ilIcons.ExtractIcon(1));
				}

				pPane->Attach(&m_wndBookmarks);

				break;
			}
		}

		return TRUE; // handled
	}
	return FALSE;
}



void CMainFrame::OnThemeChanged()
{
	XTPPaintManager()->GetIconsInfo()->bUseFadedIcons = FALSE;
	XTPPaintManager()->GetIconsInfo()->bUseDisabledIcons = TRUE;
	XTPPaintManager()->GetIconsInfo()->bIconsWithShadow = FALSE;

	m_paneManager.GetPaintManager()->RefreshMetrics();
	m_MTIClientWnd.GetPaintManager()->RefreshMetrics();

	GetCommandBars()->RedrawCommandBars();
	RedrawWindow(0, 0, RDW_INVALIDATE|RDW_ALLCHILDREN);

}

void CMainFrame::OnViewBasethemeOffice2000()
{
	CXTPPaintManager::SetCustomTheme(new CEclipseCommandBarsPaintManager<CXTPDefaultTheme>);

	m_paneManager.SetCustomTheme(new CEclipseDockingPanePaintManager<CXTPDockingPaneDefaultTheme>);

	m_paneManager.GetPaintManager()->GetPanelPaintManager()->SetAppearance(xtpTabAppearanceStateButtons);
	m_paneManager.GetPaintManager()->GetPanelPaintManager()->SetColor(xtpTabColorDefault);

	OnThemeChanged();

}

void CMainFrame::OnViewBasethemeOfficexp()
{
	CXTPPaintManager::SetCustomTheme(new CEclipseCommandBarsPaintManager<CXTPOfficeTheme>);

	m_paneManager.SetCustomTheme(new CEclipseDockingPanePaintManager<CXTPDockingPaneOfficeTheme>);

	m_paneManager.GetPaintManager()->GetPanelPaintManager()->SetAppearance(xtpTabAppearanceFlat);

	OnThemeChanged();

}


void CMainFrame::OnViewBasethemeOffice2003()
{
	CXTPPaintManager::SetCustomTheme(new CEclipseCommandBarsPaintManager<CXTPOffice2003Theme>);

	m_paneManager.SetCustomTheme(new CEclipseDockingPanePaintManager<CXTPDockingPaneOffice2003Theme>);

	m_paneManager.GetPaintManager()->GetPanelPaintManager()->SetAppearance(xtpTabAppearancePropertyPageFlat);
	m_paneManager.GetPaintManager()->GetPanelPaintManager()->SetColor(xtpTabColorOffice2003);

	OnThemeChanged();
}

void CMainFrame::OnClose()
{
	// Save the current state for command bars.
	SaveCommandBars(_T("CommandBars"));
	// TODO: Add your message handler code here and/or call default

	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnCustomize()
{
	// get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars == NULL)
		return;

	// instanciate the customize dialog
	CXTPCustomizeSheet dlg(pCommandBars);

	// add the options page to the customize dialog.
	CXTPCustomizeOptionsPage pageOptions(&dlg);
	dlg.AddPage(&pageOptions);

	// add the commands page to the customize dialog.
	CXTPCustomizeCommandsPage* pPageCommands = dlg.GetCommandsPage();
	pPageCommands->AddCategories(IDR_GUI_ECTYPE);

	// initialize the commands page page.
	pPageCommands->InsertAllCommandsCategory();
	pPageCommands->InsertBuiltInMenus(IDR_GUI_ECTYPE);
	pPageCommands->InsertNewMenuCategory();

	// display the customize dialog.
	dlg.DoModal();
}


void CMainFrame::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
{

// Toggle CommandBars
	GetCommandBars()->OnSetPreviewMode(bPreview);

// Toggle Tab Client
	m_MTIClientWnd.ShowWorkspace(!bPreview);

// Toggle Docking Panes.
	m_paneManager.OnSetPreviewMode(bPreview);


	CMDIFrameWnd::OnSetPreviewMode(bPreview, pState);
}
