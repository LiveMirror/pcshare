// mainfrm.cpp : implementation of the CMainFrame class
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

#include "wordpad.h"
#include "mainfrm.h"
#include "wordpdoc.h"
#include "wordpvw.h"
#include "strings.h"
#include "colorlis.h"

#include "ControlSelector.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_COMMAND(ID_HELP, OnHelpFinder)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_CHAR_COLOR, OnCharColor)
	ON_COMMAND(ID_PEN_TOGGLE, OnPenToggle)
	ON_WM_FONTCHANGE()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	ON_WM_DEVMODECHANGE()
	ON_COMMAND(ID_HELP_INDEX, OnHelpFinder)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	// Global help commands
//  ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, OnHelpFinder)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FORMATBAR, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RULER, OnUpdateControlBarMenu)
	ON_MESSAGE(WPM_BARSTATE, OnBarState)
	ON_REGISTERED_MESSAGE(CWordPadApp::m_nOpenMsg, OnOpenMsg)
	ON_COMMAND_EX(ID_VIEW_STATUS_BAR, OnBarCheck)
	ON_COMMAND_EX(ID_VIEW_TOOLBAR, OnBarCheck)
	ON_COMMAND_EX(ID_VIEW_FORMATBAR, OnBarCheck)
	ON_COMMAND_EX(ID_VIEW_RULER, OnBarCheck)

	ON_XTP_CREATECONTROL()
	ON_XTP_CREATECOMMANDBAR()
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)

	ON_COMMAND_RANGE(ID_BDR_OUTSIDE, ID_BDR_INSIDE_VERT, OnBorders)
	ON_UPDATE_COMMAND_UI_RANGE(ID_BDR_OUTSIDE, ID_BDR_INSIDE_VERT, OnUpdateBorders)

	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)



END_MESSAGE_MAP()

static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
};

#define IDR_COLORTEXT_POPUP 1000

enum Borders {borderTop = 1, borderLeft = 2, borderRight = 4, borderBottom = 8, borderInsideVert = 16, borderInsideHorz = 32,
	borderOutside = borderBottom | borderTop | borderLeft | borderRight,
	borderInside = borderInsideVert | borderInsideHorz,
	borderAll = borderInside | borderOutside};



/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_hIconDoc = theApp.LoadIcon(IDI_ICON_DOC);
	m_hIconText = theApp.LoadIcon(IDI_ICON_TEXT);
	m_hIconWrite = theApp.LoadIcon(IDI_ICON_WRITE);

	m_dwBorders = 0;
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	WNDCLASS wndcls;

	BOOL bRes = CFrameWnd::PreCreateWindow(cs);
	HINSTANCE hInst = AfxGetInstanceHandle();

	// see if the class already exists
	if (!::GetClassInfo(hInst, szWordPadClass, &wndcls))
	{
		// get default stuff
		::GetClassInfo(hInst, cs.lpszClass, &wndcls);
		wndcls.style &= ~(CS_HREDRAW|CS_VREDRAW);
		// register a new class
		wndcls.lpszClassName = szWordPadClass;
		wndcls.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDR_MAINFRAME));
		ASSERT(wndcls.hIcon != NULL);
		if (!AfxRegisterClass(&wndcls))
			AfxThrowResourceException();
	}
	cs.lpszClass = szWordPadClass;
	CRect rect = theApp.m_rectInitialFrame;
	if (rect.Width() > 0 && rect.Height() > 0)
	{
		// make sure window will be visible
		CDisplayIC dc;
		CRect rectDisplay(0, 0, dc.GetDeviceCaps(HORZRES),
			dc.GetDeviceCaps(VERTRES));
		if (rectDisplay.PtInRect(rect.TopLeft()) &&
			rectDisplay.PtInRect(rect.BottomRight()))
		{
			cs.x = rect.left;
			cs.y = rect.top;
			cs.cx = rect.Width();
			cs.cy = rect.Height();
		}
	}
	return bRes;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!CreateStatusBar())
		return -1;

	if (!InitCommandBars())
		return -1;

	CXTPPaintManager::SetTheme(xtpThemeOffice2003);


	CXTPCommandBar* pMenuBar = GetCommandBars()->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);
	pMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage);

	if (!CreateToolBars())
		return -1;

	EnableDocking(CBRS_ALIGN_ANY);

	if (!CreateRulerBar())
		return -1;

	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndFormatBar.EnableDocking(CBRS_ALIGN_TOP|CBRS_ALIGN_BOTTOM);
	//DockControlBar(&m_wndToolBar);
	//DockControlBar(&m_wndFormatBar);

	CWnd* pView = GetDlgItem(AFX_IDW_PANE_FIRST);
	if (pView != NULL)
	{
		pView->SetWindowPos(&wndBottom, 0, 0, 0, 0,
			SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE);
	}

	CXTPOffice2003Theme::LoadModernToolbarIcons(IDR_MAINFRAME1);
	CXTPOffice2003Theme::LoadModernToolbarIcons(IDB_FORMATBAR);

	LoadCommandBars(_T("CommandBars2"));

	return 0;
}

BOOL CMainFrame::CreateToolBars()
{
	XTPImageManager()->SetMaskColor(RGB(0, 255, 0));

	CXTPCommandBars* pCommandBars = GetCommandBars();

	CXTPToolBar* pStandardBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pStandardBar ||
		!pStandardBar->LoadToolBar(IDR_MAINFRAME1))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;
	}

	CXTPToolBar* pFormatBar = (CXTPToolBar*)pCommandBars->Add(_T("Formating"), xtpBarTop);
	if (!pFormatBar ||
		!pFormatBar->LoadToolBar(IDB_FORMATBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;
	}

	DockRightOf(pFormatBar, pStandardBar);


	return TRUE;
}

BOOL CMainFrame::CreateRulerBar()
{
	if (!m_wndRulerBar.Create(this,
		WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_HIDE_INPLACE, ID_VIEW_RULER))
	{
		TRACE0("Failed to create ruler\n");
		return FALSE;      // fail to create
	}
	return TRUE;
}

BOOL CMainFrame::CreateStatusBar()
{
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return FALSE;      // fail to create
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Operations

HICON CMainFrame::GetIcon(int nDocType)
{
	switch (nDocType)
	{
		case RD_WINWORD6:
		case RD_WORDPAD:
		case RD_EMBEDDED:
		case RD_RICHTEXT:
			return m_hIconDoc;
		case RD_TEXT:
		case RD_OEMTEXT:
			return m_hIconText;
		case RD_WRITE:
			return m_hIconWrite;
	}
	return m_hIconDoc;
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

void CMainFrame::OnFontChange()
{
	//m_wndFormatBar.SendMessage(CWordPadApp::m_nPrinterChangedMsg);
}

void CMainFrame::OnDevModeChange(LPTSTR lpDeviceName)
{
	theApp.NotifyPrinterChanged();
	CFrameWnd::OnDevModeChange(lpDeviceName); //sends message to descendants
}

void CMainFrame::OnSysColorChange()
{
	CFrameWnd::OnSysColorChange();
	m_wndRulerBar.SendMessage(WM_SYSCOLORCHANGE);
}

void CMainFrame::ActivateFrame(int nCmdShow)
{
	CFrameWnd::ActivateFrame(nCmdShow);
	// make sure and display the toolbar, ruler, etc while loading a document.
	OnIdleUpdateCmdUI();
	UpdateWindow();
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	theApp.m_bMaximized = (nType == SIZE_MAXIMIZED);
	if (nType == SIZE_RESTORED)
		GetWindowRect(theApp.m_rectInitialFrame);
}

LONG CMainFrame::OnBarState(UINT wParam, LONG lParam)
{
	if (lParam == -1)
		return 0L;
	ASSERT(lParam != RD_EMBEDDED);
	if (wParam == 0)
	{
		//CDockState& ds = theApp.GetDockState(lParam);
		//ds.Clear(); // empty out the dock state
		//GetDockState(ds);
	}
	else
	{
		if (IsTextType(lParam))
		{
			// in text mode hide the ruler and format bar so that it is the default
			CWnd* pBar = GetControlBar(ID_VIEW_RULER);
			if (pBar != NULL)
				pBar->ShowWindow(SW_HIDE);
		}
		HICON hIcon = GetIcon((int)lParam);
		SendMessage(WM_SETICON, TRUE, (LPARAM)hIcon);
		//SetDockState(theApp.GetDockState(lParam));
	}
	return 0L;
}

void CMainFrame::OnMove(int x, int y)
{
	CFrameWnd::OnMove(x, y);
	WINDOWPLACEMENT wp;
	wp.length = sizeof(wp);
	GetWindowPlacement(&wp);
	theApp.m_rectInitialFrame = wp.rcNormalPosition;
	CView* pView = GetActiveView();
	if (pView != NULL)
		pView->SendMessage(WM_MOVE);
}

LONG CMainFrame::OnOpenMsg(UINT, LONG lParam)
{
	TCHAR szAtomName[256];
	szAtomName[0] = NULL;
	GlobalGetAtomName((ATOM)lParam, szAtomName, 256);
	CWordPadDoc* pDoc = (CWordPadDoc*)GetActiveDocument();
	if (szAtomName[0] != NULL && pDoc != NULL)
	{
		if (lstrcmpi(szAtomName, pDoc->GetPathName()) == 0)
			return TRUE;
	}
	return FALSE;
}

void CMainFrame::OnHelpFinder()
{
	theApp.WinHelp(0, HELP_FINDER);
}

void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
	TCHAR szFileName[_MAX_PATH];
	::DragQueryFile(hDropInfo, 0, szFileName, _MAX_PATH);
	::DragFinish(hDropInfo);
	theApp.OpenDocumentFile(szFileName);
}

void CMainFrame::OnCharColor()
{
	/*CColorMenu colorMenu;
	CRect rc;
	int index = m_wndFormatBar.CommandToIndex(ID_CHAR_COLOR);
	m_wndFormatBar.GetItemRect(index, &rc);
	m_wndFormatBar.ClientToScreen(rc);
	colorMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,rc.left,rc.bottom, this);*/
}

void CMainFrame::OnPenToggle()
{

}

BOOL CMainFrame::OnQueryNewPalette()
{
	CView* pView = GetActiveView();
	if (pView != NULL)
		return (BOOL)pView->SendMessage(WM_QUERYNEWPALETTE);
	return FALSE;
}

void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd)
{
	CView* pView = GetActiveView();
	if (pView != NULL)
		pView->SendMessage(WM_PALETTECHANGED, (WPARAM)pFocusWnd->GetSafeHwnd());
}



int CMainFrame::OnCreateCommandBar(LPCREATEBARSTRUCT lpCreatePopup)
{
	if (lpCreatePopup->bTearOffBar && lpCreatePopup->nID == IDR_COLORTEXT_POPUP)
	{
		CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreatePopup->pCommandBar);
		ASSERT(pToolBar);

		// Prevent docking.
		pToolBar->EnableDocking(xtpFlagFloating);
		// Prevent customization
		pToolBar->EnableCustomization(FALSE);
		return TRUE;
	}
	return FALSE;
}
int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	CXTPToolBar* pToolBar = lpCreateControl->bToolBar? DYNAMIC_DOWNCAST(CXTPToolBar, lpCreateControl->pCommandBar): NULL;

	if (lpCreateControl->nID == ID_COMBO_FONT && pToolBar && pToolBar->GetBarID() == IDB_FORMATBAR)
	{
		CXTPControlComboBox* pComboFont = new CXTPControlFontComboBox();
		pComboFont->SetDropDownListStyle();

		lpCreateControl->pControl = pComboFont;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_BDR_NONE && pToolBar)
	{
		CXTPControlPopup* pPopup = CXTPControlPopup::CreateControlPopup(xtpControlSplitButtonPopup);

		CXTPPopupToolBar* pPopupBar = CXTPPopupToolBar::CreatePopupToolBar(GetCommandBars());

		pPopupBar->LoadToolBar(IDR_BORDERS);
		pPopupBar->SetWidth(23 * 5 + 6);
		pPopupBar->SetTearOffPopup(_T("Borders"), IDR_BORDERS, 23 * 5 + 6);

		pPopup->SetCommandBar(pPopupBar);
		pPopupBar->InternalRelease();

		lpCreateControl->pControl = pPopup;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_COMBO_SIZE && pToolBar && pToolBar->GetBarID() == IDB_FORMATBAR)
	{
		CXTPControlComboBox* pComboSize = new CXTPControlSizeComboBox();
		pComboSize->SetDropDownListStyle();

		lpCreateControl->pControl = pComboSize;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_BUTTON_TEXT && pToolBar && pToolBar->GetBarID() == IDB_FORMATBAR)
	{
		CXTPControlPopupColor* pPopupColor = new CXTPControlPopupColor();

		CXTPPopupBar* pColorBar = (CXTPPopupBar*)CXTPPopupToolBar::CreateObject();

		pColorBar->GetControls()->Add(new CXTPControlButtonColor(), XTP_IDS_AUTOMATIC);
		pColorBar->GetControls()->Add(new CXTPControlColorSelector(), ID_SELECTOR_TEXT);
		pColorBar->GetControls()->Add(new CXTPControlButtonColor(), XTP_IDS_MORE_COLORS);

		pPopupColor->SetCommandBar(pColorBar);

		pColorBar->SetTearOffPopup(_T("Text Color"), IDR_COLORTEXT_POPUP, 0);
		pColorBar->EnableCustomization(FALSE);

		pColorBar->InternalRelease();

		lpCreateControl->pControl = pPopupColor;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_EDIT_UNDO && pToolBar)
	{
		CXTPControlPopup* pButtonUndo = CXTPControlPopup::CreateControlPopup(xtpControlSplitButtonPopup);

		CXTPPopupToolBar* pUndoBar = CXTPPopupToolBar::CreatePopupToolBar(GetCommandBars());
		pUndoBar->EnableCustomization(FALSE);
		pUndoBar->SetBorders(CRect(2, 2, 2, 2));
		pUndoBar->DisableShadow();


		CXTPControlListBox* pControlListBox = (CXTPControlListBox*)pUndoBar->GetControls()->Add(new CXTPControlListBox(), ID_EDIT_UNDO);
		pControlListBox->SetWidth(140);
		pControlListBox->SetLinesMinMax(1, 6);
		pControlListBox->SetMultiplSel(TRUE);

		CXTPControlStatic* pControlListBoxInfo = (CXTPControlStatic*)pUndoBar->GetControls()->Add(new CXTPControlStatic(), ID_EDIT_UNDO);
		pControlListBoxInfo->SetWidth(140);


		pButtonUndo->SetCommandBar(pUndoBar);
		pUndoBar->InternalRelease();

		lpCreateControl->pControl = pButtonUndo;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_INSERT_COLUMNS)
	{
		CXTPControlPopup* pButtonColumns = CXTPControlPopup::CreateControlPopup(xtpControlButtonPopup);
		pButtonColumns->SetFlags(xtpFlagManualUpdate);

		CXTPPopupToolBar* pColumnsBar = CXTPPopupToolBar::CreatePopupToolBar(GetCommandBars());
		pColumnsBar->EnableCustomization(FALSE);
		pColumnsBar->SetBorders(CRect(2, 2, 2, 2));

		pColumnsBar->DisableShadow();


		pColumnsBar->GetControls()->Add(new CControlColumns(), ID_INSERT_COLUMNS);


		CXTPControlStatic* pControlColumnsInfo = (CXTPControlStatic*)pColumnsBar->GetControls()->Add(new CXTPControlStatic(), ID_INSERT_COLUMNS);
		pControlColumnsInfo->SetFlags(xtpFlagManualUpdate);

		pButtonColumns->SetCommandBar(pColumnsBar);
		pColumnsBar->InternalRelease();

		lpCreateControl->pControl = pButtonColumns;
		return TRUE;
	}

	if (lpCreateControl->nID == ID_INSERT_TABLE)
	{
		CXTPControlPopup* pButtonTable = CXTPControlPopup::CreateControlPopup(xtpControlButtonPopup);
		pButtonTable->SetFlags(xtpFlagManualUpdate);

		CXTPPopupToolBar* pTableBar = CXTPPopupToolBar::CreatePopupToolBar(GetCommandBars());
		pTableBar->EnableCustomization(FALSE);
		pTableBar->SetBorders(CRect(2, 2, 2, 2));

		pTableBar->DisableShadow();


		pTableBar->GetControls()->Add(new CControlTable(), ID_INSERT_TABLE);


		CXTPControlStatic* pControlTableInfo = (CXTPControlStatic*)pTableBar->GetControls()->Add(new CXTPControlStatic(), ID_INSERT_TABLE);
		pControlTableInfo->SetFlags(xtpFlagManualUpdate);

		pButtonTable->SetCommandBar(pTableBar);
		pTableBar->InternalRelease();

		lpCreateControl->pControl = pButtonTable;
		return TRUE;

	}

	return FALSE;
}

void CMainFrame::OnCustomize()
{
	CXTPCustomizeSheet cs(GetCommandBars());

	CXTPCustomizeOptionsPage pageOptions(&cs);
	cs.AddPage(&pageOptions);

	CXTPCustomizeCommandsPage* pCommands = cs.GetCommandsPage();
	pCommands->AddCategories(IDR_MAINFRAME);
	pCommands->GetControls(_T("File"))->Add(xtpControlButton, ID_FILE_OPEN, _T(""), 1);

	pCommands->InsertAllCommandsCategory();
	pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
	pCommands->InsertNewMenuCategory();

	cs.DoModal();
}

void CMainFrame::OnClose()
{
	SaveCommandBars(_T("CommandBars2"));

	CFrameWnd::OnClose();
}



void CMainFrame::SwitchBorders(DWORD dwState)
{
	if (m_dwBorders & dwState) m_dwBorders &= ~dwState; else m_dwBorders |= dwState;

}


void CMainFrame::OnBorders(UINT nID)
{
	switch (nID)
	{
		case ID_BDR_TOP: SwitchBorders(borderTop); break;
		case ID_BDR_LEFT: SwitchBorders(borderLeft); break;
		case ID_BDR_RIGHT: SwitchBorders(borderRight); break;
		case ID_BDR_BOTTOM: SwitchBorders(borderBottom); break;
		case ID_BDR_NONE: m_dwBorders = 0; break;
		case ID_BDR_OUTSIDE: SwitchBorders(borderOutside); break;
		case ID_BDR_ALL: SwitchBorders(borderAll); break;
		case ID_BDR_INSIDE_VERT: SwitchBorders(borderInsideVert); break;
		case ID_BDR_INSIDE_HORZ: SwitchBorders(borderInsideHorz); break;
		case ID_BDR_INSIDE: SwitchBorders(borderInside); break;
	}

}

void CMainFrame::OnUpdateBorders(CCmdUI* pCmdUI)
{
	switch (pCmdUI->m_nID)
	{
		case ID_BDR_TOP: pCmdUI->SetCheck(m_dwBorders & borderTop? TRUE: FALSE); break;
		case ID_BDR_LEFT: pCmdUI->SetCheck(m_dwBorders & borderLeft? TRUE: FALSE); break;
		case ID_BDR_RIGHT: pCmdUI->SetCheck(m_dwBorders & borderRight? TRUE: FALSE); break;
		case ID_BDR_BOTTOM: pCmdUI->SetCheck(m_dwBorders & borderBottom? TRUE: FALSE); break;
		case ID_BDR_OUTSIDE : pCmdUI->SetCheck((m_dwBorders & borderOutside) == borderOutside? TRUE: FALSE); break;
		case ID_BDR_ALL : pCmdUI->SetCheck((m_dwBorders & borderAll) == borderAll? TRUE: FALSE); break;
		case ID_BDR_INSIDE_VERT: pCmdUI->SetCheck(m_dwBorders & borderInsideVert? TRUE: FALSE); break;
		case ID_BDR_INSIDE_HORZ: pCmdUI->SetCheck(m_dwBorders & borderInsideHorz? TRUE: FALSE); break;
		case ID_BDR_INSIDE: pCmdUI->SetCheck((m_dwBorders & borderInside) == borderInside? TRUE: FALSE); break;
	}
}
