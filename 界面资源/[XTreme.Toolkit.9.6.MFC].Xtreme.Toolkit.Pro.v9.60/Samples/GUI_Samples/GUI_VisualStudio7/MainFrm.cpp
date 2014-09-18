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
#include "GUI_VisualStudio7.h"

#include "MainFrm.h"
#include "DynamicHelpView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLOR_MASK  RGB(0x00,0x80,0x80)

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_COMMAND(ID_WINDOW_MORE, OnMoreWindows)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_VIEW_SOLUTIONEXPLORER, ID_HELP_DYNAMICHELP, OnShowView)

	ON_COMMAND_RANGE(ID_FILE_NEW_PROJECT + 1, 200, OnEmptyCommandRange)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)

	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_XTP_CREATECONTROL()

	ON_XTP_EXECUTE(ID_EDIT_STATE, OnEditState)
	ON_UPDATE_COMMAND_UI(ID_EDIT_STATE, OnUpdateEditState)

	ON_MESSAGE(WM_XTP_PRETRANSLATEMOUSEMSG, OnWorkspaceMouseMsg)
	ON_COMMAND(ID_WINDOW_AUTOHIDEALL, OnWindowHideall)

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
	m_nState = 1;
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

	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);

	CXTPCommandBars* pCommandBars = GetCommandBars();

	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_GUI_VITYPE);
	pMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage);

	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}



////////////////////////
	XTPImageManager()->SetMaskColor((COLORREF)-1); // Auto

	CXTPToolBar* pWebBar = (CXTPToolBar*)pCommandBars->Add(_T("Web"), xtpBarTop);
	if (!pWebBar ||
		!pWebBar->LoadToolBar(IDR_TOOLBAR_WEB))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	XTPImageManager()->SetIcons(IDR_TOOLBAR_EXT);

	LoadCommandBars(_T("CommandBars"));

//////////////////////////////////////////////////////////////////////////
// Docking Pane

	GetDockingPaneManager()->InstallDockingPanes(this);
	GetDockingPaneManager()->SetTheme(xtpPaneThemeOffice);

	CXTPDockingPaneLayout layoutNormal(GetDockingPaneManager());

	if (layoutNormal.Load(_T("NormalLayout")))
	{
		GetDockingPaneManager()->SetLayout(&layoutNormal);
	}
	else
	{
		GetDockingPaneManager()->CreatePane(
			ID_WINDOWS_OUTPUT, CRect(0, 0, 150, 120), xtpPaneDockBottom);

		CXTPDockingPane* paneToolBox = GetDockingPaneManager()->CreatePane(
			ID_VIEW_TOOLBOX, CRect(0, 0, 150, 120), xtpPaneDockLeft);
		paneToolBox->Hide();

		CXTPDockingPane* paneClassView = GetDockingPaneManager()->CreatePane(
			ID_VIEW_CLASSVIEW, CRect(0, 0, 210, 140), xtpPaneDockRight);

		CXTPDockingPane* paneSolutionExplorer = GetDockingPaneManager()->CreatePane(
			ID_VIEW_SOLUTIONEXPLORER, CRect(0, 0, 210, 140), xtpPaneDockRight);

		CXTPDockingPane* paneResourceView = GetDockingPaneManager()->CreatePane(
			ID_VIEW_RESOURCEVIEW, CRect(0, 0, 210, 140), xtpPaneDockRight);

		GetDockingPaneManager()->AttachPane(paneSolutionExplorer, paneClassView);
		GetDockingPaneManager()->AttachPane(paneResourceView, paneClassView);

		paneClassView->Select();

		CXTPDockingPane* paneHelpView = GetDockingPaneManager()->CreatePane(
			ID_HELP_DYNAMICHELP, CRect(0, 0, 210, 140), xtpPaneDockBottom, paneClassView);

		CXTPDockingPane* paneProperties = GetDockingPaneManager()->CreatePane(
			ID_VIEW_PROPERTIESWINDOW, CRect(0, 0, 180, 140), xtpPaneDockBottom, paneClassView);

		GetDockingPaneManager()->AttachPane(paneProperties, paneHelpView);
	}

	int nIDIcons[] = {
		ID_VIEW_TOOLBOX, ID_VIEW_PROPERTIESWINDOW, ID_VIEW_SOLUTIONEXPLORER, ID_WINDOWS_OUTPUT,
		ID_VIEW_CLASSVIEW, 0, 0, 0, 0, 0, 0, 0, 0, 0, ID_VIEW_RESOURCEVIEW, ID_HELP_DYNAMICHELP
	};

	GetDockingPaneManager()->SetIcons(IDB_BITMAP_PANEICONS, nIDIcons,
		sizeof(nIDIcons)/sizeof(nIDIcons[0]), RGB(0, 255, 0));

	VERIFY(m_MTIClientWnd.Attach(this, TRUE));
	m_MTIClientWnd.GetPaintManager()->m_bShowIcons = FALSE;
	m_MTIClientWnd.EnableToolTips();

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

}
void CMainFrame::OnEmptyCommandRange(UINT)
{

}

CWnd* CMainFrame::CreateClassView()
{
	if (m_wndClassView.GetSafeHwnd() == 0)
	{
		if (!m_wndClassView.Create( WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
			CRect(0,0,0,0), this, 0xFF ))
		{
			TRACE0( "Unable to create tree control.\n" );
			return 0;
		}
		m_wndClassView.ModifyStyleEx(0, WS_EX_STATICEDGE);
		// load the tree images bitmap and add it to the image list.
		if (!m_ilClassView.Create(IDB_CLASSTREE, 16, 1, COLOR_MASK))
			return 0;

		// Set the image list for the tree control.
		m_wndClassView.SetImageList( &m_ilClassView, TVSIL_NORMAL );

		// Add the parent item
		HTREEITEM htItem = m_wndClassView.InsertItem(_T("XtremeDemo classes"));
		m_wndClassView.SetItemState( htItem, TVIS_BOLD, TVIS_BOLD );

		// Add children
		HTREEITEM hti = m_wndClassView.InsertItem(_T("CAboutDlg"), 3, 3, htItem);
		m_wndClassView.InsertItem(_T("CAboutDlg()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("DoDataExchange(CDataExchange *pDX)"), 5, 5, hti);

		hti = m_wndClassView.InsertItem(_T("CChildFrame"), 3, 3, htItem);
		m_wndClassView.InsertItem(_T("AssertValid()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("CChildFrame()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("~CChildFrame()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("Dump(CDumpContext &dc)"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("PreCreateWindow(CREATESTRUCT& cs)"), 4, 4, hti);

		hti = m_wndClassView.InsertItem(_T("CMainFrame"), 3, 3, htItem);
		m_wndClassView.InsertItem(_T("AssertValid()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("CMainFrame()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("~CMainFrame()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("Dump(CDumpContext &dc)"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("OnCreate(LPCREATESTRUCT lpCreateStruct)"), 5, 5, hti);
		m_wndClassView.InsertItem(_T("PreCreateWindow(CREATESTRUCT& cs)"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("m_wndStatusBar"), 7, 7, hti);
		m_wndClassView.InsertItem(_T("m_wndToolBar"), 7, 7, hti);

		hti = m_wndClassView.InsertItem(_T("CXtremeDemoApp"), 3, 3, htItem);
		m_wndClassView.InsertItem(_T("CXtremeDemoApp()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("InitInstance()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("OnAppAbout()"), 4, 4, hti);

		hti = m_wndClassView.InsertItem(_T("CXtremeDemoDoc"), 3, 3, htItem);
		m_wndClassView.InsertItem(_T("AssertValid()"), 5, 5, hti);
		m_wndClassView.InsertItem(_T("CXtremeDemoDoc()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("~CXtremeDemoDoc()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("Dump(CDumpContext &dc)"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("OnNewDocument()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("Serialize(CArchive& ar)"), 4, 4, hti);
		m_wndClassView.Expand(hti, TVE_EXPAND);

		hti = m_wndClassView.InsertItem(_T("CXtremeDemoView"), 3, 3, htItem);
		m_wndClassView.InsertItem(_T("AssertValid()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("CXtremeDemoView()"), 5, 5, hti);
		m_wndClassView.InsertItem(_T("~CXtremeDemoView()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("Dump(CDumpContext& dc)"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("GetDocument()"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("OnBeginPrinting(CDC* pDC, CPrintInfo *pInfo)"), 5, 5, hti);
		m_wndClassView.InsertItem(_T("OnDraw(CDC *pDC)"), 4, 4, hti);
		m_wndClassView.InsertItem(_T("OnEndPrinting(CDC *pDC, CPrintInfo *pInfo)"), 5, 5, hti);
		m_wndClassView.InsertItem(_T("OnPreparePrinting(CPrintInfo *pInfo)"), 5, 5, hti);
		m_wndClassView.InsertItem(_T("PreCreateWindow(CREATESTRUCT &cs)"), 4, 4, hti);

		hti = m_wndClassView.InsertItem(_T("Globals"), 1, 2, htItem);
		m_wndClassView.InsertItem(_T("theApp"), 6, 6, hti);
		m_wndClassView.Expand(hti, TVE_EXPAND);

		m_wndClassView.Expand(htItem, TVE_EXPAND);


	}
	return &m_wndClassView;
}

CWnd* CMainFrame::CreateSolutionView()
{
	if (m_wndSolutionView.GetSafeHwnd() == 0)
	{
		if (!m_wndSolutionView.Create( WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
			CRect(0,0,0,0), this, 0xFF ))
		{
			TRACE0( "Unable to create tree control.\n" );
			return 0;
		}
		m_wndSolutionView.ModifyStyleEx(0, WS_EX_STATICEDGE);

		// load the tree images bitmap and add it to the image list.
		if (!m_ilSolutionView.Create(IDB_FILETREE, 16, 1, COLOR_MASK))
			return 0;

		// Set the image list for the tree control.
		m_wndSolutionView.SetImageList( &m_ilSolutionView, TVSIL_NORMAL );

		// add the parent item, make it bold
		HTREEITEM htiParent = m_wndSolutionView.InsertItem(_T("Solution 'XtremeDemo': 1 project(s)"));
		HTREEITEM htiChild; // child item

		htiChild = m_wndSolutionView.InsertItem (_T("XtremeDemo Files"), 1, 1, htiParent);
		m_wndSolutionView.SetItemState (htiChild, TVIS_BOLD, TVIS_BOLD);

		// add the children of the parent item
		HTREEITEM hti = m_wndSolutionView.InsertItem(_T("Source Files"), 2, 3, htiChild);
		m_wndSolutionView.InsertItem(_T("ChildFrm.cpp"), 4, 4, hti);
		m_wndSolutionView.InsertItem(_T("MainFrm.cpp"), 4, 4, hti);
		m_wndSolutionView.InsertItem(_T("StdAfx.cpp"), 4, 4, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemo.cpp"), 4, 4, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemo.rc"), 4, 4, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemoDoc.cpp"), 4, 4, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemoView.cpp"), 4, 4, hti);
		m_wndSolutionView.Expand(hti, TVE_EXPAND);

		hti = m_wndSolutionView.InsertItem(_T("Header Files"), 2, 3, htiChild);
		m_wndSolutionView.InsertItem(_T("ChildFrm.h"), 5, 5, hti);
		m_wndSolutionView.InsertItem(_T("MainFrm.h"), 5, 5, hti);
		m_wndSolutionView.InsertItem(_T("Resource.rc"), 5, 5, hti);
		m_wndSolutionView.InsertItem(_T("StdAfx.h"), 5, 5, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemo.h"), 5, 5, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemoDoc.h"), 5, 5, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemoView.h"), 5, 5, hti);

		hti = m_wndSolutionView.InsertItem(_T("Resource Files"), 2, 3, htiChild);
		m_wndSolutionView.InsertItem(_T("Toolbar.bmp"), 5, 5, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemo.ico"), 5, 5, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemo.rc2"), 5, 5, hti);
		m_wndSolutionView.InsertItem(_T("XtremeDemoDoc.ico"), 5, 5, hti);

		m_wndSolutionView.InsertItem(_T("ReadMe.txt"), 5, 5, htiChild);
		m_wndSolutionView.InsertItem(_T("External Dependencies"), 2, 3, htiChild);

		m_wndSolutionView.Expand(htiParent, TVE_EXPAND);
		m_wndSolutionView.Expand(htiChild, TVE_EXPAND);


	}
	return &m_wndSolutionView;
}

CWnd* CMainFrame::CreateResourceView()
{
	if (m_wndResourceView.GetSafeHwnd() == 0)
	{
		if (!m_wndResourceView.Create( WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
			CRect(0,0,0,0), this, 0xFF ))
		{
			TRACE0( "Unable to create tree control.\n" );
			return 0;
		}
		m_wndResourceView.ModifyStyleEx(0, WS_EX_STATICEDGE);

		// load the tree images bitmap and add it to the image list.
		if (!m_ilResourceView.Create(IDB_RESTREE, 16, 1, COLOR_MASK))
			return 0;

		// Set the image list for the tree control.
		m_wndResourceView.SetImageList( &m_ilResourceView, TVSIL_NORMAL );

		// add the parent item
		HTREEITEM htItem = m_wndResourceView.InsertItem(_T("XtremeDemo resources"), 0, 1);
		m_wndResourceView.SetItemState(htItem, TVIS_BOLD, TVIS_BOLD);

		HTREEITEM hti = m_wndResourceView.InsertItem(_T("Accelerator"), 0, 1, htItem);
		m_wndResourceView.InsertItem(_T("IDR_MAINFRAME"), 2, 2, hti);
		m_wndResourceView.Expand(hti, TVE_EXPAND);

		hti = m_wndResourceView.InsertItem(_T("Dialog"), 0, 1, htItem);
		m_wndResourceView.InsertItem(_T("IDD_ABOUTBOX"), 3, 3, hti);
		m_wndResourceView.Expand(hti, TVE_EXPAND);

		hti = m_wndResourceView.InsertItem(_T("Icon"), 0, 1, htItem);
		m_wndResourceView.InsertItem(_T("IDR_MAINFRAME"), 4, 4, hti);
		m_wndResourceView.InsertItem(_T("IDR_XTREMETYPE"), 4, 4, hti);
		m_wndResourceView.Expand(hti, TVE_EXPAND);

		hti = m_wndResourceView.InsertItem(_T("Menu"), 0, 1, htItem);
		m_wndResourceView.InsertItem(_T("IDR_MAINFRAME"), 5, 5, hti);
		m_wndResourceView.InsertItem(_T("IDR_XTREMETYPE"), 5, 5, hti);

		hti = m_wndResourceView.InsertItem(_T("String Table"), 0, 1, htItem);
		m_wndResourceView.InsertItem(_T("String Table"), 6, 6, hti);

		hti = m_wndResourceView.InsertItem(_T("Toolbar"), 0, 1, htItem);
		m_wndResourceView.InsertItem(_T("IDR_MAINFRAME"), 7, 7, hti);

		hti = m_wndResourceView.InsertItem(_T("Version"), 0, 1, htItem);
		m_wndResourceView.InsertItem(_T("VS_VERSION_INFO"), 8, 8, hti);

		m_wndResourceView.Expand (htItem, TVE_EXPAND);


	}
	return &m_wndResourceView;
}

CWnd* CMainFrame::CreateOutputView()
{
	if (m_wndOutput.GetSafeHwnd() == 0)
	{
		m_wndOutput.Create(WS_HSCROLL|ES_AUTOHSCROLL|ES_MULTILINE, CRect(0, 0, 0, 0), this, 0 );
		m_wndOutput.ModifyStyleEx(0, WS_EX_STATICEDGE);
		m_wndOutput.SetWindowText(
			_T("'GUI_VisualStudio7.exe': Loaded 'C:\\Codejock.com\\.cvs\\ToolkitPro\\Samples\\GUI_Samples\\GUI_VisualStudio7\\Debug\\GUI_VisualStudio7.exe', Symbols loaded.\r\n")
			_T("'GUI_VisualStudio7.exe': Loaded 'D:\\WINDOWS\\system32\\ntdll.dll', No symbols loaded.\r\n")
			_T("'GUI_VisualStudio7.exe': Loaded 'D:\\WINDOWS\\system32\\kernel32.dll', No symbols loaded.\r\n")
			_T("'GUI_VisualStudio7.exe': Loaded 'D:\\WINDOWS\\system32\\mfc70d.dll', Symbols loaded.\r\n")
			_T("'GUI_VisualStudio7.exe': Loaded 'D:\\WINDOWS\\system32\\msvcr70d.dll', Symbols loaded.\r\n")
			_T("'GUI_VisualStudio7.exe': Loaded 'D:\\WINDOWS\\system32\\gdi32.dll', No symbols loaded.\r\n")
		);

		m_wndOutput.SetFont(XTPPaintManager()->GetRegularFont());
		m_wndOutput.SetMargins(10, 5);

	}

	return &m_wndOutput;
}


CWnd* CMainFrame::CreatePropertyView()
{
	if (m_wndPropertyGrid.GetSafeHwnd() == 0)
	{
		m_wndPropertyGrid.Create( CRect(0, 0, 0, 0), this, 0 );
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
		CXTPPropertyGridItem* pItemOne    = pCustom->AddChildItem(new CXTPPropertyGridItem(_T("First Level"), _T("")));
		CXTPPropertyGridItem* pItemTwo    = pItemOne->AddChildItem(new CXTPPropertyGridItem(_T("Second Level"), _T("")));
		CXTPPropertyGridItem* pItemThird     = pItemTwo->AddChildItem(new CXTPPropertyGridItem(_T("Third Level"), _T("")));
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
				case ID_VIEW_PROPERTIESWINDOW:  pPane->Attach(CreatePropertyView()); break;
				case ID_VIEW_CLASSVIEW: pPane->Attach(CreateClassView()); break;
				case ID_VIEW_SOLUTIONEXPLORER:  pPane->Attach(CreateSolutionView()); break;
				case ID_VIEW_RESOURCEVIEW:  pPane->Attach(CreateResourceView()); break;
				case ID_WINDOWS_OUTPUT: pPane->Attach(CreateOutputView()); break;
				case ID_HELP_DYNAMICHELP:
					{
						// attach the dynamic help view to the pane.
						CWnd* pView = pPane->AttachView(this, RUNTIME_CLASS(CDynamicHelpView));

						// send a WM_INITIALUPDATE message to the view.
						if (pView && ::IsWindow(pView->m_hWnd)) {
							pView->SendMessage(WM_INITIALUPDATE);
						}
						break;
					}
			}
		}
		return 1;
	}
	return 0;
}

void CMainFrame::OnShowView(UINT nID)
{
	GetDockingPaneManager()->ShowPane(nID);
}

void CMainFrame::OnDestroy()
{
	CMDIFrameWnd::OnDestroy();

	m_MTIClientWnd.Detach();
}


int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->bToolBar)
	{
		CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreateControl->pCommandBar);
		if (!pToolBar)
			return FALSE;

		if (lpCreateControl->nID == ID_VIEW_CLASSVIEW && pToolBar->GetBarID() == IDR_MAINFRAME)
		{
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}
		if (lpCreateControl->nID == ID_FILE_NEW_PROJECT && pToolBar->GetBarID() == IDR_MAINFRAME)
		{
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}
		if (lpCreateControl->nID == ID_PROJECT_ADDNEWITEM && pToolBar->GetBarID() == IDR_MAINFRAME)
		{
			lpCreateControl->controlType = xtpControlSplitButtonPopup;
			return TRUE;
		}

		if (lpCreateControl->nID == ID_EDIT_FIND_EX && pToolBar->GetBarID() == IDR_MAINFRAME)
		{
			CXTPControlComboBox* pComboFind = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
			pComboFind->SetDropDownListStyle();

			pComboFind->AddString(_T("Line1"));
			pComboFind->AddString(_T("Line2"));
			pComboFind->AddString(_T("Line3"));

			pComboFind->SetWidth(150);
			pComboFind->SetCaption(_T("Find"));
			pComboFind->SetStyle(xtpComboLabel);

			lpCreateControl->pControl = pComboFind;
			return TRUE;
		}

		if (lpCreateControl->nID == ID_EDIT_STATE && pToolBar->GetBarID() == IDR_MAINFRAME)
		{
			CXTPControlComboBox* pComboState = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
			pComboState->AddString(_T("Debug"));
			pComboState->AddString(_T("Release"));
			pComboState->AddString(_T("Configuration Manager"));
			pComboState->SetDropDownWidth(150);
			lpCreateControl->pControl = pComboState;
			return TRUE;
		}

		if (lpCreateControl->nID == ID_GOTO_URL && pToolBar->GetBarID() == IDR_TOOLBAR_WEB)
		{
			CXTPControlComboBox* pComboUrl = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
			pComboUrl->AddString(_T("http://www.codejock.com"));
			pComboUrl->AddString(_T("ms-help://MS.VSCC/vsccCommon/html/vsccCommon_Start_Page.htm"));
			pComboUrl->SetWidth(200);
			pComboUrl->SetDropDownWidth(300);
			pComboUrl->SetDropDownListStyle();
			pComboUrl->SetFlags(xtpFlagManualUpdate);
			pComboUrl->SetEditText(_T("http://www.codejock.com"));

			lpCreateControl->pControl = pComboUrl;
			return TRUE;
		}


	}
	return FALSE;

}


void CMainFrame::OnCustomize()
{
	CXTPCustomizeSheet cs(GetCommandBars());

	CXTPCustomizeOptionsPage pageOptions(&cs);
	cs.AddPage(&pageOptions);

	CXTPCustomizeCommandsPage* pCommands = cs.GetCommandsPage();
	pCommands->AddCategories(IDR_GUI_VITYPE, TRUE);

	pCommands->InsertAllCommandsCategory();
	pCommands->InsertBuiltInMenus(IDR_GUI_VITYPE);
	pCommands->InsertNewMenuCategory();

	cs.DoModal();
}


void CMainFrame::OnEditState(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlComboBox* pControl = (CXTPControlComboBox*)tagNMCONTROL->pControl;
	if (pControl->GetType() == xtpControlComboBox)
	{
		int nState = pControl->GetCurSel();
		if (nState != 2)
		{
			m_nState = nState;
		}
		else
		{
			MessageBox(_T("Configuration Manager"));
			pControl->SetCurSel(m_nState);
		}
		*pResult = TRUE; // Handled
	}
}

void CMainFrame::OnUpdateEditState(CCmdUI* pCmdUI)
{
	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmdUI->m_pOther;
	if (pToolBar && !XTPMouseManager()->IsTrackedLock())
	{
		CXTPControlComboBox* pStateCombo = (CXTPControlComboBox*)pToolBar->GetControls()->GetAt(pCmdUI->m_nIndex);

		if (pStateCombo->GetType() == xtpControlComboBox)
		{
			pStateCombo->SetCurSel(m_nState);
		}
	}

	pCmdUI->Enable(TRUE);
}


BOOL CMainFrame::ShowWindowEx(int nCmdShow)
{

#ifdef _XTP_INCLUDE_CONTROLS
	// Restore frame window size and position.
	CXTWindowPos wndPosition;
	wndPosition.LoadWindowPos(this);
	nCmdShow = wndPosition.showCmd;

#endif

	return ShowWindow(nCmdShow);

}

void CMainFrame::OnClose()
{
	SaveCommandBars(_T("CommandBars"));

	CXTPDockingPaneLayout layoutNormal(GetDockingPaneManager());
	GetDockingPaneManager()->GetLayout(&layoutNormal);
	layoutNormal.Save(_T("NormalLayout"));

#ifdef _XTP_INCLUDE_CONTROLS
	CXTWindowPos wndPosition;
	wndPosition.SaveWindowPos(this);
#endif


	CMDIFrameWnd::OnClose();
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



LRESULT CMainFrame::OnWorkspaceMouseMsg(WPARAM wParam,LPARAM lParam)
{
	CPoint point = CPoint((DWORD)lParam);

	CXTPTabManagerItem* pItem =  m_MTIClientWnd.HitTest(point);
	if (pItem)
	{
		if (wParam == WM_RBUTTONDOWN)
		{

			CWnd* pFrame = CWnd::FromHandle(pItem->GetHandle());
			MDIActivate(pFrame);

			m_MTIClientWnd.Refresh();

			CMenu menuPopup;
			VERIFY(menuPopup.LoadMenu(IDR_WORKSAPCE_POPUP));

			m_MTIClientWnd.WorkspaceToScreen(&point);
			CXTPCommandBars::TrackPopupMenu(menuPopup.GetSubMenu(0), 0, point.x, point.y, this);


			m_MTIClientWnd.Refresh();

			return TRUE;
		}


		return FALSE;
	}

	return FALSE;
}

void CMainFrame::OnWindowHideall()
{
	CXTPDockingPaneInfoList& lstPanes = m_paneManager.GetPaneList();
	POSITION pos = lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPane* pPane = lstPanes.GetNext(pos);
		if (!pPane->IsClosed() && !pPane->IsHidden())
			m_paneManager.HidePane(pPane);
	}

}

void CMainFrame::OnMoreWindows()
{
#ifdef _XTP_INCLUDE_CONTROLS
	CXTWindowList dlg(this);
	dlg.DoModal();
#endif
}
