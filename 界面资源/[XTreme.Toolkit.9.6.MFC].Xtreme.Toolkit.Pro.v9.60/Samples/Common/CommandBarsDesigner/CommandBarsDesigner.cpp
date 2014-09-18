// CommandBarsDesigner.cpp : Defines the class behaviors for the application.
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
#include "ChildFrm.h"
#include "CommandBarsDesignerDoc.h"
#include "CommandBarsDesignerView.h"

#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerApp

BEGIN_MESSAGE_MAP(CCommandBarsDesignerApp, CWinApp)
	//{{AFX_MSG_MAP(CCommandBarsDesignerApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerApp construction

CCommandBarsDesignerApp::CCommandBarsDesignerApp()
{
	m_bRunAutomated = FALSE;
	EnableManifestEditor();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCommandBarsDesignerApp object

CCommandBarsDesignerApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {D87BE4B3-AF03-44B7-A307-84820060A4C5}
static const CLSID clsid =
{ 0xd87be4b3, 0xaf03, 0x44b7, { 0xa3, 0x7, 0x84, 0x82, 0x0, 0x60, 0xa4, 0xc5 } };

/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerApp initialization
BOOL IsRegisterShellCommand()
{
	for (int i = 1; i < __argc; i++)
	{
		LPCTSTR pszParam = __targv[i];
		if (pszParam[0] == '-' || pszParam[0] == '/')
		{
			++pszParam;
			if (_tcscmp(pszParam, _T("Regserver")) == 0)
				return TRUE;
		}
	}

	return FALSE;
}

BOOL CCommandBarsDesignerApp::InitInstance()
{
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

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
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCommandBarsDesignerDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CCommandBarsDesignerView));
	AddDocTemplate(m_pDocTemplate);

	// Connect the COleTemplateServer to the document template.
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template.
	m_server.ConnectTemplate(clsid, m_pDocTemplate, FALSE);

	// Register all OLE server factories as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleTemplateServer::RegisterAll();
		// Note: MDI applications register all server objects without regard
		//  to the /Embedding or /Automation on the command line.

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	m_bRunAutomated = cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated;

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;


	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// Check to see if launched as OLE server
	if (m_bRunAutomated)
	{
		// Application was run with /Embedding or /Automation.  Don't show the
		//  main window in this case.
		return TRUE;
	}

	// When a server application is launched stand-alone, it is a good idea
	//  to update the system registry in case it has been damaged.
	m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
	COleObjectFactory::UpdateRegistryAll();

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	if (IsRegisterShellCommand())
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	pMainFrame->RefreshPanes();

	return TRUE;
}



// App command to run the dialog
void CCommandBarsDesignerApp::OnAppAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerApp message handlers


CString StripChars(CString str)
{
	str.MakeUpper();
	XTPStringRemove(str, _T('&'));
	XTPStringRemove(str, _T(' '));
	int i = 0;
	while (i < str.GetLength())
	{
		if (!isupper(str[i])) XTPStringRemove(str, str[i]); else i++;
	}
	return str;
}

void XTPStringRemove(CString& str, TCHAR chRemove)
{
#if _MSC_VER < 1200 // MFC 5.0
	class CXTPString: public CString
	{
		friend void XTPStringRemove(CString& str, TCHAR chRemove);
	};
	((CXTPString*)&str)->CopyBeforeWrite();

	LPTSTR pstrSource = ((CXTPString*)&str)->m_pchData;
	LPTSTR pstrDest = ((CXTPString*)&str)->m_pchData;
	LPTSTR pstrEnd = ((CXTPString*)&str)->m_pchData + ((CXTPString*)&str)->GetData()->nDataLength;

	while (pstrSource < pstrEnd)
	{
		if (*pstrSource != chRemove)
		{
			*pstrDest = *pstrSource;
			pstrDest = _tcsinc(pstrDest);
		}
		pstrSource = _tcsinc(pstrSource);
	}
	*pstrDest = '\0';
	int nCount = pstrSource - pstrDest;
	((CXTPString*)&str)->GetData()->nDataLength -= nCount;
#else
	str.Remove(chRemove);
#endif

}

int XTPStringDelete(CString& str, int nIndex, int nCount)
{
#if _MSC_VER < 1200 // MFC 5.0

	class CXTPString: public CString
	{
		friend int XTPStringDelete(CString& str, int nIndex, int nCount);
	};

	if (nIndex < 0)
		nIndex = 0;
	int nNewLength = ((CXTPString*)&str)->GetData()->nDataLength;
	if (nCount > 0 && nIndex < nNewLength)
	{
		((CXTPString*)&str)->CopyBeforeWrite();
		int nBytesToCopy = nNewLength - (nIndex + nCount) + 1;

		MEMCPY_S(((CXTPString*)&str)->m_pchData + nIndex,
			((CXTPString*)&str)->m_pchData + nIndex + nCount, nBytesToCopy * sizeof(TCHAR));
		((CXTPString*)&str)->GetData()->nDataLength = nNewLength - nCount;
	}

	return nNewLength;
#else
	return str.Delete(nIndex, nCount);
#endif
}

COLORREF GetStaticFrameColor()
{
	return GetXtremeColor(XPCOLOR_STATICFRAME);
}

BOOL IsRunAutomated()
{
	return theApp.m_bRunAutomated;
}
