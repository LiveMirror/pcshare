// GUI_VisualStudio7.cpp : Defines the class behaviors for the application.
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
#include "ChildFrm.h"
#include "GUI_VisualStudio7Doc.h"
#include "GUI_VisualStudio7View.h"

#include "StartPageView.h"
#include "BrowserView.h"

#include "AboutDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUI_VisualStudio7App

BEGIN_MESSAGE_MAP(CGUI_VisualStudio7App, CWinApp)
	//{{AFX_MSG_MAP(CGUI_VisualStudio7App)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_WINDOW_CLOSE_ALL, OnWindowCloseAll)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_NEW_PROJECT, OnFileNew)

	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)

	ON_XTP_EXECUTE(ID_GOTO_URL, OnGotoURL)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUI_VisualStudio7App construction

CGUI_VisualStudio7App::CGUI_VisualStudio7App()
{
	m_bFirstTime = TRUE;
	EnableManifestEditor();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGUI_VisualStudio7App object

CGUI_VisualStudio7App theApp;

/////////////////////////////////////////////////////////////////////////////
// CGUI_VisualStudio7App initialization

BOOL CGUI_VisualStudio7App::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#if _MSC_VER <= 1200 // MFC 6.0 or earlier
#ifdef _AFXDLL
	Enable3dControls();         // Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();   // Call this when linking to MFC statically
#endif
#endif // MFC 6.0 or earlier

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Codejock Software Sample Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	m_pDocTemplate = new CMultiDocTemplate(
		IDR_GUI_VITYPE,
		RUNTIME_CLASS(CGUI_VisualStudio7Doc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CGUI_VisualStudio7View));
	AddDocTemplate(m_pDocTemplate);

	m_pDocTemplateStartPage = new CMultiDocTemplate(
		IDR_GUI_VITYPE,
		RUNTIME_CLASS(CGUI_VisualStudio7Doc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CStartPageView));
	AddDocTemplate(m_pDocTemplateStartPage);

	m_pDocTemplateBrowserPage = new CMultiDocTemplate(
		IDR_GUI_VITYPE,
		RUNTIME_CLASS(CGUI_VisualStudio7Doc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CBrowserView));
	AddDocTemplate(m_pDocTemplateBrowserPage);



	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindowEx(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

void CGUI_VisualStudio7App::OnFileNew()
{
	if (m_bFirstTime == TRUE)
	{
		CDocument* pDoc = m_pDocTemplateStartPage->OpenDocumentFile(NULL);
		pDoc->SetTitle(_T("Start Page"));
	}
	else
	{
		m_pDocTemplate->OpenDocumentFile(NULL);
	}
}

// App command to run the dialog
void CGUI_VisualStudio7App::OnAppAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_VisualStudio7App message handlers

void CGUI_VisualStudio7App::OnGotoURL(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlComboBox* pControl = (CXTPControlComboBox*)tagNMCONTROL->pControl;
	if (pControl->GetType() == xtpControlComboBox)
	{
		CGUI_VisualStudio7Doc* pDoc = (CGUI_VisualStudio7Doc*)m_pDocTemplateBrowserPage->OpenDocumentFile(NULL);
		POSITION pos = pDoc->GetFirstViewPosition();
		CBrowserView* pView = (CBrowserView*)pDoc->GetNextView(pos);

		CString strURL = pControl->GetEditText();

	#if _MSC_VER >= 1200 // MFC 5.0
		pView->Navigate(strURL);
	#endif
		pDoc->SetTitle(strURL);
	}
	*pResult = 1;
}
void CGUI_VisualStudio7App::OnWindowCloseAll()
{
	CloseAllDocuments(FALSE);

}
