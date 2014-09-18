// TipOfTheDay.cpp : Defines the class behaviors for the application.
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
#include "TipOfTheDay.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "TipOfTheDayDoc.h"
#include "TipOfTheDayView.h"
#include "AboutDlg.h"
#include "SplitPath.h"

#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTipOfTheDayApp

BEGIN_MESSAGE_MAP(CTipOfTheDayApp, CWinApp)
	//{{AFX_MSG_MAP(CTipOfTheDayApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(IDC_TIPOFTHEDAY, OnTipoftheday)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTipOfTheDayApp construction

CTipOfTheDayApp::CTipOfTheDayApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTipOfTheDayApp object

CTipOfTheDayApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTipOfTheDayApp initialization

BOOL CTipOfTheDayApp::InitInstance()
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

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_TIPOFTTYPE,
		RUNTIME_CLASS(CTipOfTheDayDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTipOfTheDayView));
	AddDocTemplate(pDocTemplate);

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
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// This method handles the 'Tip of the Day' component.
	ShowTipAtStartup();

	return TRUE;
}

// App command to run the dialog
void CTipOfTheDayApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTipOfTheDayApp message handlers

void CTipOfTheDayApp::FindTipsFile(LPCTSTR lpszTipsFile)
{
	// if the tips.txt file does not exist, create one...
	if (ACCESS_S(lpszTipsFile, 0) == -1)
	{
		TRY
		{
			CStdioFile file(lpszTipsFile,
				CFile::modeCreate | CFile::modeWrite | CFile::typeText);

			file.WriteString(_T("Technical support is just a phone call away, feel free to contact us at anytime with your support or sales questions, we are always here to help! (989) 723-1442\n"));
			file.WriteString(_T("You can easily change the appearance of your application to look like Office XP with a single line of code.  Choose \"View\" then \"Office XP Look\" to see this in action!\n"));
			file.WriteString(_T("You can save thousands of hours in development time by using the many excellent controls found in the Xtreme Toolkit!\n"));

			file.Close();
		}
		CATCH( CFileException, e )
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
		}
		END_CATCH
	}
}

void CTipOfTheDayApp::ShowTipAtStartup()
{
	// This method handles the 'Tip of the Day' component.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (cmdInfo.m_bShowSplash)
	{
		CXTTipOfTheDay dlg;
		if (dlg.m_bStartup) {
			OnTipoftheday();
		}
	}
}

void CTipOfTheDayApp::OnTipoftheday()
{
	// construct the name of our tips file.  This is the file that will contain
	// our tip of the day messages.
	CSplitPath sp(m_pszHelpFilePath);

	CString strTipsFile;
	strTipsFile.Format(_T("%s%stips.txt"), sp.GetDrive(), sp.GetDir());

	FindTipsFile(strTipsFile);

	CXTTipOfTheDay dlg(strTipsFile);
	dlg.DoModal();
}
