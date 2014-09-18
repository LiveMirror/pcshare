// ChildFrm.cpp : implementation of the CChildFrame class
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
#include "MDIPanes.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_TOOLS_PROPERTIES, OnToolsProperties)
	//}}AFX_MSG_MAP

	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
END_MESSAGE_MAP()

XTPDockingPanePaintTheme CChildFrame::m_themeCurrent = xtpPaneThemeOffice;

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here

}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

#ifdef _XTP_INCLUDE_COMMANDBARS
	if (!InitCommandBars())
		return -1;

	CXTPCommandBars* pCommandBars = GetCommandBars();
	pCommandBars->GetCommandBarsOptions()->bShowExpandButtonAlways = false;

	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_CHILD_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}
	CXTPControlButton* pControl = (CXTPControlButton*)pCommandBar->GetControls()->FindControl(ID_TOOLS_PROPERTIES);
	pControl->SetStyle(xtpButtonIconAndCaption);

#endif


	// control bars objects have been created and docked.
	m_paneManager.InstallDockingPanes(this);

	// Create docking panes.
	m_paneManager.CreatePane(
		IDR_PANE_PROPERTIES, CRect(0, 0,210, 120), xtpPaneDockLeft);

	int nID = IDR_PANE_PROPERTIES;
	m_paneManager.SetIcons(IDB_BITMAP_ICONS, &nID, 1);

	m_paneManager.SetTheme(m_themeCurrent);


	return 0;
}


void CChildFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	CMDIChildWnd::OnUpdateFrameTitle(bAddToTitle);

	CXTPDockingPane* pPane = m_paneManager.FindPane(IDR_PANE_PROPERTIES);
	if (pPane)
	{
		CString strTitle;
		GetWindowText(strTitle);
		pPane->SetTitle(_T("Properties - ") + strTitle + _T("\nProperties"));
	}
}

LRESULT CChildFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

#ifdef _XTP_INCLUDE_PROPERTYGRID

		if (!pPane->IsValid())
		{
			ASSERT (pPane->GetID() == IDR_PANE_PROPERTIES);

			if (!m_wndPropertyGrid.m_hWnd)
			{
				m_wndPropertyGrid.Create(CRect(0, 0, 0, 0), this, 1000);
				m_wndPropertyGrid.SetOwner(this);

				CFont font;
				font.CreatePointFont(180, _T("Arial"));

				LOGFONT lf;
				font.GetLogFont( &lf );

				CXTPPropertyGridItem* pSettings  = m_wndPropertyGrid.AddCategory(_T("Settings"));
				pSettings->AddChildItem(new CXTPPropertyGridItem(_T("Text"), _T("www.codejock.com")));
				pSettings->AddChildItem(new CXTPPropertyGridItemFont(_T("Font"), lf));
				pSettings->AddChildItem(new CXTPPropertyGridItemColor(_T("Color"), RGB(0,0, 255)));

				pSettings->Expand();

			}
			pPane->Attach(&m_wndPropertyGrid);
		}

		return TRUE;
#endif

	}
	return FALSE;
}

void CChildFrame::OnToolsProperties()
{
	m_paneManager.ShowPane(IDR_PANE_PROPERTIES);

}
