// MDITabWindow.cpp : Defines the class behaviors for the application.
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
#include "MDITabWindow.h"

#include "MainFrm.h"
#include "HelloFrm.h"
#include "HelloDoc.h"
#include "HelloVw.h"

//Added for Bounce document
#include "BncFrm.h"
#include "BncDoc.h"
#include "BncVw.h"

#include "AboutDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDITabWindowApp

BEGIN_MESSAGE_MAP(CMDITabWindowApp, CWinApp)
	//{{AFX_MSG_MAP(CMDITabWindowApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEWHELLO, OnNewHello)
	ON_COMMAND(ID_FILE_NEWBOUNCE, OnNewBounce)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDITabWindowApp construction

CMDITabWindowApp::CMDITabWindowApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMDITabWindowApp object

CMDITabWindowApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMDITabWindowApp initialization

BOOL CMDITabWindowApp::InitInstance()
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
		IDR_HELLOTYPE,
		RUNTIME_CLASS(CHelloDoc),
		RUNTIME_CLASS(CHelloFrame), // custom MDI child frame
		RUNTIME_CLASS(CHelloView));
	AddDocTemplate(pDocTemplate);

// Add Bounce template to list

	CMultiDocTemplate* pBounceTemplate;
	pBounceTemplate = new CMultiDocTemplate(
		IDR_BOUNCETYPE,
		RUNTIME_CLASS(CBounceDoc),
		RUNTIME_CLASS(CBounceFrame), // custom MDI child frame
		RUNTIME_CLASS(CBounceView));
	AddDocTemplate(pBounceTemplate);


	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}



// App command to run the dialog
void CMDITabWindowApp::OnAppAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMDITabWindowApp message handlers





/////////////////////////////////////////////////////////////////////////////
// other globals

// Color array maps colors to top-level Color menu

COLORREF NEAR colorArray[] =
{
	RGB (0, 0, 0),
	RGB (255, 0, 0),
	RGB (0, 255, 0),
	RGB (0, 0, 255),
	RGB (255, 255, 255)
};
/////////////////////////////////////////////////////////////////////////////
// CMDIApp commands

// The following two command handlers provides an
// alternative way to open documents by hiding the fact
// that the application has multiple templates. The
// default method uses a dialog with a listing of
// possible types to choose from.

void CMDITabWindowApp::OnNewHello()
{

// Searches template list for a document type
// containing the "Hello" string

	POSITION curTemplatePos = GetFirstDocTemplatePosition();

	while(curTemplatePos != NULL)
	{
		CDocTemplate* curTemplate =
			GetNextDocTemplate(curTemplatePos);
		CString str;
		curTemplate->GetDocString(str, CDocTemplate::docName);
		if(str == _T("Hello"))
		{
			curTemplate->OpenDocumentFile(NULL);
			return;
		}
	}
	AfxMessageBox(IDS_NOHELLOTEMPLATE);
}

void CMDITabWindowApp::OnNewBounce()
{
// Searches template list for a document type
// containing the "Bounce" string

	POSITION curTemplatePos = GetFirstDocTemplatePosition();

	while(curTemplatePos != NULL)
	{
		CDocTemplate* curTemplate =
			GetNextDocTemplate(curTemplatePos);
		CString str;
		curTemplate->GetDocString(str, CDocTemplate::docName);
		if(str == _T("Bounce"))
		{
			curTemplate->OpenDocumentFile(NULL);
			return;
		}
	}
	AfxMessageBox(IDS_NOBOUNCETEMPLATE);
}
