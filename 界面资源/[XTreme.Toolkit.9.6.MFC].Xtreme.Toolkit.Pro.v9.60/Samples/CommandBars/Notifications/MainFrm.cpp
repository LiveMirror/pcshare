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
#include "Notifications.h"

#include "MainFrm.h"

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
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP

	ON_XTP_CREATECOMMANDBAR()
	ON_XTP_INITCOMMANDSPOPUP()
	ON_XTP_CREATECONTROL()

	ON_UPDATE_COMMAND_UI(ID_WINDOW_GOTO, OnUpdateGoto)
	ON_XTP_EXECUTE(ID_WINDOW_GOTO, OnGoto)
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
{
	// TODO: add member initialization code here

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

	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars == NULL)
	{
		TRACE0("Failed to create command bars object.\n");
		return -1;      // fail to create
	}

	// Add the menu bar

	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(
		_T("Menu Bar"), IDR_MAINFRAME);
	if(pMenuBar == NULL)
	{
		TRACE0("Failed to create menu bar.\n");
		return -1;      // fail to create
	}

	// Create ToolBar
	CXTPToolBar* pToolBar = (CXTPToolBar*)
		pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pToolBar || !pToolBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	CXTPPaintManager::SetTheme(xtpThemeNativeWinXP);


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

int CMainFrame::OnCreateCommandBar(LPCREATEBARSTRUCT lpCreatePopup)
{
	if (lpCreatePopup->bPopup && _tcscmp(lpCreatePopup->lpcstrCaption, _T("Edit")) == 0)
	{
		CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());
		pPopupBar->SetTearOffPopup(_T("Edit Bar"), 1000);
		lpCreatePopup->pCommandBar = pPopupBar;
		return TRUE;
	}

	return FALSE;
}

int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->nID == ID_VIEW_FULLSCREEN )
	{
		CXTPControlButton* pButton = (CXTPControlButton*)CXTPControlButton::CreateObject();
		pButton->SetStyle(xtpButtonIconAndCaption);
		lpCreateControl->pControl = pButton;
		return TRUE;
	}
	if (lpCreateControl->nID == ID_WINDOW_GOTO)
	{
		CXTPControlComboBox* pComboState = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
		pComboState->SetDropDownWidth(150);
		lpCreateControl->pControl = pComboState;
		return TRUE;
	}

	return FALSE;
}

void CMainFrame::OnInitCommandsPopup(CXTPPopupBar* pCommandBar)
{
	// get the list of commands for the popup.
	CXTPControls* pCommandList = pCommandBar->GetControls();

	// Remove "New" menu item from the File menu.
	CXTPControl* pCommandNew = pCommandList->FindControl(
		xtpControlButton, ID_FILE_NEW, TRUE, FALSE);
	if (pCommandNew)
	{
		pCommandList->Remove(pCommandNew);
	}

	// Rename menu item "Save" to "Save Document".
	CXTPControl* pCommandSave = pCommandList->FindControl(
		xtpControlButton, ID_FILE_SAVE, TRUE, FALSE);
	if (pCommandSave)
	{
		CWnd* pWnd = MDIGetActive();
		CString strCaption;
		if (pWnd) pWnd->GetWindowText(strCaption);

		pCommandSave->SetCaption("&Save " + strCaption);
	}

	// Rename menu item "Print..." to "Print Document...".
	CXTPControl* pCommandPrint = pCommandList->FindControl(
		xtpControlButton, ID_FILE_PRINT, TRUE, FALSE);
	if (pCommandPrint)
	{
		CWnd* pWnd = MDIGetActive();
		CString strCaption;
		if (pWnd) pWnd->GetWindowText(strCaption);

		pCommandPrint->SetCaption("&Print " + strCaption +"...");
	}

	// Sample of dynamic creation. Recommended to make it in OnCreateControl handler.
	CXTPControl* pControlIcons = pCommandList->FindControl(
		xtpControlButton, ID_VIEW_ICONS, TRUE, FALSE);
	if (pControlIcons)
	{
		CXTPControlPopup* pPopup = (CXTPControlPopup*)pControlIcons->GetControls()->SetControlType(pControlIcons, xtpControlPopup);
		pPopup->SetFlags(xtpFlagManualUpdate);

		CXTPPopupToolBar* pPopupBar = CXTPPopupToolBar::CreatePopupToolBar(GetCommandBars());
		pPopupBar->LoadToolBar(IDR_MAINFRAME);
		pPopupBar->SetWidth(200);

		pPopup->SetCommandBar(pPopupBar);
		pPopupBar->InternalRelease();

	}



	// Combo box's List bar.
	if (pCommandBar->GetPosition() == xtpBarListBox)
	{
		CXTPControlComboBox* pControl = DYNAMIC_DOWNCAST(CXTPControlComboBox, pCommandBar->GetControlPopup());
		ASSERT(pControl != NULL);

		pControl->ResetContent();

		int nID = AFX_IDM_FIRST_MDICHILD;

		HWND hWndChild = ::GetDlgItem(m_hWndMDIClient, nID);
		while (hWndChild)
		{
			TCHAR lpText[256];
			::GetWindowText(hWndChild, lpText, 256);
			pControl->AddString(lpText);


			hWndChild = ::GetDlgItem(m_hWndMDIClient, ++nID);
		}

	}
}



void CMainFrame::OnUpdateGoto(CCmdUI* pCmdUI)
{

	pCmdUI->Enable(MDIGetActive() != NULL);

}

void CMainFrame::OnClose()
{
	// Save the current state for toolbars and menus.
	SaveCommandBars(_T("CommandBars"));

	CMDIFrameWnd::OnClose();
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
		pCommands->AddCategories(IDR_NOTIFTYPE);

		// Use the command bar manager to initialize the
		// customize dialog.
		pCommands->InsertAllCommandsCategory();
		pCommands->InsertBuiltInMenus(IDR_NOTIFTYPE);
		pCommands->InsertNewMenuCategory();

		// Dispaly the dialog.
		dlg.DoModal();
	}
}

void CMainFrame::OnGoto(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlComboBox* pControl = (CXTPControlComboBox*)tagNMCONTROL->pControl;
	if (pControl->GetType() == xtpControlComboBox)
	{
		CString strText = pControl->GetText();
		if (!strText.IsEmpty())
		{
			int nID = AFX_IDM_FIRST_MDICHILD;

			HWND hWndChild = ::GetDlgItem(m_hWndMDIClient, nID);
			while (hWndChild)
			{
				CWnd* pWnd = CWnd::FromHandle(hWndChild);
				CString strCaption;
				pWnd->GetWindowText(strCaption);

				if (strCaption == strText)
				{
					MDIActivate(pWnd);
					break;
				}

				hWndChild = ::GetDlgItem(m_hWndMDIClient, ++nID);
			}

		}

		*pResult = TRUE;
	}
}
