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
#include "SplitterWindow.h"
#include "MainFrm.h"
#include "View1.h"
#include "View2.h"
#include "View3.h"
#include "View4.h"
#include "View5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_SPLIT_SWAPVIEWS, OnSwapViews)
	ON_COMMAND(ID_SPLIT_REPLACEVIEW, OnReplaceView)
	ON_UPDATE_COMMAND_UI(ID_SPLIT_REPLACEVIEW, OnUpdateReplaceView)
	ON_UPDATE_COMMAND_UI(ID_SPLIT_SWAPVIEWS, OnUpdateSwapViews)
	ON_COMMAND(ID_SPLIT_HIDEFIRSTROW, OnHideFirstRow)
	ON_UPDATE_COMMAND_UI(ID_SPLIT_HIDEFIRSTROW, OnUpdateHideFirstRow)
	ON_COMMAND(ID_SPLIT_HIDEFIRSTCOL, OnHideFirstCol)
	ON_UPDATE_COMMAND_UI(ID_SPLIT_HIDEFIRSTCOL, OnUpdateHideFirstCol)
	ON_COMMAND(ID_SPLIT_FULLDRAG, OnSplitNofulldrag)
	ON_UPDATE_COMMAND_UI(ID_SPLIT_FULLDRAG, OnUpdateSplitNofulldrag)
	ON_COMMAND(ID_SPLIT_DOTTRACKER, OnSplitDottracker)
	ON_UPDATE_COMMAND_UI(ID_SPLIT_DOTTRACKER, OnUpdateSplitDottracker)
	ON_COMMAND(ID_SPLIT_NOSIZE, OnSplitNosize)
	ON_UPDATE_COMMAND_UI(ID_SPLIT_NOSIZE, OnUpdateSplitNosize)
	ON_COMMAND(ID_SPLIT_FLATSPLITTER, OnSplitFlatsplitter)
	ON_UPDATE_COMMAND_UI(ID_SPLIT_FLATSPLITTER, OnUpdateSplitFlatsplitter)
	ON_COMMAND(ID_VIEW_XPMODE, OnViewXpmode)
	ON_UPDATE_COMMAND_UI(ID_VIEW_XPMODE, OnUpdateViewXpmode)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)

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
: m_pView1(0)
, m_pView2(0)
, m_pView3(0)
, m_pView4(0)
{
	m_bRowHidden = FALSE;
	m_bColHidden = FALSE;
	m_bSwapped   = FALSE;
	m_bReplaced  = FALSE;
	m_bFullDrag  = TRUE;
	m_bDotTrack  = FALSE;
	m_bNoSizing  = FALSE;
	m_bFlatSplit = TRUE;

	// Enable/Disable XP GUI Mode
	CXTRegistryManager regMgr;
	XTAuxData().bXPMode = regMgr.GetProfileInt(_T("Settings"), _T("bXPMode"), TRUE);
}

CMainFrame::~CMainFrame()
{
	CXTRegistryManager regMgr;
	regMgr.WriteProfileInt(_T("Settings"), _T("bXPMode"), XTAuxData().bXPMode);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
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

	XTPPaintManager()->SetTheme(XTAuxData().bXPMode? xtpThemeOfficeXP: xtpThemeOffice2000);

	CXTPCommandBars* pCommandBars = GetCommandBars();

	pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);

	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	// Load the previous state for command bars.
	LoadCommandBars(_T("CommandBars"));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.lpszClass = AfxRegisterWndClass( 0, NULL, NULL,
		AfxGetApp()->LoadIcon(IDR_MAINFRAME));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	// Create the splitter window with two columns
	if (!m_wndSplitter.CreateStatic(this, 2,2))
	{
		TRACE0("Failed to create splitter window\n");
		return FALSE;
	}

	if (!m_wndSplitter.CreateView(0,0, RUNTIME_CLASS(CView1),
		CSize(0, 0), pContext))
	{
		TRACE0("Failed to create CView1\n");
		return FALSE;
	}
	else
	{
		m_mapRowCol.SetAt(m_wndSplitter.IdFromRowCol(0,0),
			m_wndSplitter.GetPane(0,0));
	}

	if (!m_wndSplitter.CreateView(0,1, RUNTIME_CLASS(CView2),
		CSize(0, 0), pContext))
	{
		TRACE0("Failed to create CView2\n");
		return FALSE;
	}
	else
	{
		m_mapRowCol.SetAt(m_wndSplitter.IdFromRowCol(0,1),
			m_wndSplitter.GetPane(0,1));
	}

	if (!m_wndSplitter.CreateView(1,0, RUNTIME_CLASS(CView3),
		CSize(0, 0), pContext))
	{
		TRACE0("Failed to create CView3\n");
		return FALSE;
	}
	else
	{
		m_mapRowCol.SetAt(m_wndSplitter.IdFromRowCol(1,0),
			m_wndSplitter.GetPane(1,0));
	}

	if (!m_wndSplitter.CreateView(1,1, RUNTIME_CLASS(CView4),
		CSize(0, 0), pContext))
	{
		TRACE0("Failed to create CView4\n");
		return FALSE;
	}
	else
	{
		m_mapRowCol.SetAt(m_wndSplitter.IdFromRowCol(1,1),
			m_wndSplitter.GetPane(1,1));
	}

	CRect r;
	GetClientRect(&r);

	m_wndSplitter.SetRowInfo( 0, r.Height()/2, 0 );
	m_wndSplitter.SetColumnInfo( 0, r.Width()/2, 0 );

	return TRUE;
}


int _IdFromRowCol(int row, int col)
{
	return AFX_IDW_PANE_FIRST + row * 16 + col;
}

void CMainFrame::SynchronizePanes(CXTSplitterWnd& wndSplitter)
{
	ASSERT(::IsWindow(wndSplitter.m_hWnd));

	int iRowCount = wndSplitter.GetRowCount();
	int iColCount = wndSplitter.GetColumnCount();

	bool bRecalcLayout = false;

	for (int iRow = 0; iRow < iRowCount; ++iRow)
	{
		for (int iCol = 0; iCol < iColCount; ++iCol)
		{
			int idRowCol = _IdFromRowCol(
				m_bRowHidden ? 1 : iRow,
				m_bColHidden ? 1 : iCol );

			CWnd* pWndShow = NULL;
			if (m_mapRowCol.Lookup(idRowCol, pWndShow))
			{
				ASSERT_VALID(pWndShow);

				if (pWndShow->GetDlgCtrlID() != m_wndSplitter.IdFromRowCol(iRow,iCol))
				{
					// swap control ids.
					if (m_bRowHidden || m_bColHidden)
					{
						CWnd* pWndHide = m_wndSplitter.GetPane(iRow,iCol);
						ASSERT_VALID(pWndHide);

						pWndHide->ShowWindow(SW_HIDE);
						pWndHide->SetDlgCtrlID(pWndShow->GetDlgCtrlID());
					}

					pWndShow->ShowWindow(SW_SHOW);
					pWndShow->SetDlgCtrlID(m_wndSplitter.IdFromRowCol(iRow,iCol));

					bRecalcLayout = true;
				}
			}
		}
	}

	if (bRecalcLayout)
	{
		wndSplitter.RecalcLayout();
	}
}

void CMainFrame::OnSwapViews()
{
	m_wndSplitter.SwitchView(0,0, (CView*)m_wndSplitter.GetPane(0,1));
	m_bSwapped = !m_bSwapped;
}

void CMainFrame::OnUpdateSwapViews(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bSwapped);
	pCmdUI->Enable(!m_bRowHidden && !m_bColHidden);
}

void CMainFrame::OnReplaceView()
{
	if (m_wndSplitter.GetPane(1,0)->IsKindOf(RUNTIME_CLASS(CView3))) {
		m_wndSplitter.ReplaceView(1,0, RUNTIME_CLASS(CView5));
	}
	else {
		m_wndSplitter.ReplaceView(1,0, RUNTIME_CLASS(CView3));
	}
	m_bReplaced = !m_bReplaced;
}

void CMainFrame::OnUpdateReplaceView(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bReplaced);
	pCmdUI->Enable(!m_bRowHidden && !m_bColHidden);
}

void CMainFrame::OnHideFirstRow()
{
	m_bRowHidden = !m_bRowHidden;

	if (m_bRowHidden) {
		m_wndSplitter.HideRow(0);
	}
	else {
		m_wndSplitter.ShowRow();
	}

	// make sure the views are displayed in their correct location.
	SynchronizePanes(m_wndSplitter);
}

void CMainFrame::OnUpdateHideFirstRow(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bRowHidden);
}

void CMainFrame::OnHideFirstCol()
{
	m_bColHidden = !m_bColHidden;

	if (m_bColHidden) {
		m_wndSplitter.HideColumn(0);
	}
	else {
		m_wndSplitter.ShowColumn();
	}

	// make sure the views are displayed in their correct location.
	SynchronizePanes(m_wndSplitter);
}

void CMainFrame::OnUpdateHideFirstCol(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bColHidden);
}

void CMainFrame::OnSplitNofulldrag()
{
	m_bFullDrag = !m_bFullDrag;

	DWORD dw = m_wndSplitter.GetSplitterStyle();

	if (m_bFullDrag)
		dw &= ~XT_SPLIT_NOFULLDRAG;
	else
		dw |= XT_SPLIT_NOFULLDRAG;

	m_wndSplitter.SetSplitterStyle(dw);
}

void CMainFrame::OnUpdateSplitNofulldrag(CCmdUI* pCmdUI)
{
	BOOL bEnable = FALSE;

	// Get system settings for full drag.
	::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS,
		0, &bEnable, 0);

	pCmdUI->SetCheck(m_bFullDrag);
	pCmdUI->Enable(bEnable && !m_bDotTrack);
}

void CMainFrame::OnSplitDottracker()
{
	m_bDotTrack = !m_bDotTrack;

	DWORD dw = m_wndSplitter.GetSplitterStyle();

	if (m_bDotTrack)
		dw |= XT_SPLIT_DOTTRACKER;
	else
		dw &= ~XT_SPLIT_DOTTRACKER;

	m_wndSplitter.SetSplitterStyle(dw);
}

void CMainFrame::OnUpdateSplitDottracker(CCmdUI* pCmdUI)
{
	BOOL bEnable = FALSE;

	// Get system settings for full drag.
	::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS,
		0, &bEnable, 0);

	pCmdUI->SetCheck(m_bDotTrack);

	if (bEnable)
		pCmdUI->Enable(!m_bFullDrag);
}

void CMainFrame::OnSplitNosize()
{
	m_bNoSizing = !m_bNoSizing;

	DWORD dw = m_wndSplitter.GetSplitterStyle();

	if (m_bNoSizing)
		dw |= XT_SPLIT_NOSIZE;
	else
		dw &= ~XT_SPLIT_NOSIZE;

	m_wndSplitter.SetSplitterStyle(dw);
}

void CMainFrame::OnUpdateSplitNosize(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bNoSizing);
}

void CMainFrame::OnSplitFlatsplitter()
{
	m_bFlatSplit = !m_bFlatSplit;
	m_wndSplitter.EnableFlatLook(m_bFlatSplit);
}

void CMainFrame::OnUpdateSplitFlatsplitter(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bFlatSplit);
}

void CMainFrame::OnViewXpmode()
{
	XTAuxData().bXPMode = !XTAuxData().bXPMode;
	XTPPaintManager()->SetTheme(XTAuxData().bXPMode? xtpThemeOfficeXP: xtpThemeOffice2000);


	RedrawWindow( NULL, NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN );
}

void CMainFrame::OnUpdateViewXpmode(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(XTAuxData().bXPMode);
}

void CMainFrame::OnClose()
{
	// Save the current state for command bars.
	SaveCommandBars(_T("CommandBars"));
	// TODO: Add your message handler code here and/or call default

	CFrameWnd::OnClose();
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
	pPageCommands->AddCategories(IDR_MAINFRAME);

	// initialize the commands page page.
	pPageCommands->InsertAllCommandsCategory();
	pPageCommands->InsertBuiltInMenus(IDR_MAINFRAME);
	pPageCommands->InsertNewMenuCategory();

	// display the customize dialog.
	dlg.DoModal();
}
