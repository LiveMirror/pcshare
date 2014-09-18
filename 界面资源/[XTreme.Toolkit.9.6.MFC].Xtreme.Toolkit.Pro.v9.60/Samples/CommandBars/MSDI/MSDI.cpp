// MSDI.cpp
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

/*********************************************************
* Multithreaded SDI Application
* Version: 1.4
* Date: January 3, 2002
* Autor: Michal Mecinski
* E-mail: mimec@mimec.w.pl
* WWW: http://www.mimec.w.pl
*
* Copyright (C) 2002-03 by Michal Mecinski
*********************************************************/

#include "stdafx.h"
#include <afxpriv.h>
#include <dde.h>
#include "MSDI.h"

#include "MSDIDocManager.h"
#include "MSDIThread.h"


#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMSDIApp

BEGIN_MESSAGE_MAP(CMSDIApp, CWinApp)
	//{{AFX_MSG_MAP(CMSDIApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	// Handle thread messages
	ON_THREAD_MESSAGE(MSDIM_EXIT_THREAD, OnExitThread)
	ON_THREAD_MESSAGE(MSDIM_NEW_INSTANCE, OnNewInstance)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMSDIApp construction

CMSDIApp::CMSDIApp()
{
	m_pFrameWnd = NULL;
	EnableManifestEditor();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMSDIApp object

CMSDIApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMSDIApp initialization

BOOL CMSDIApp::InitInstance()
{
	if (!ParseCommandLine())
		return FALSE;

	if (IsAlreadyRunning(_T("Unique App Name")))
		return FALSE;

	SetRegistryKey(_T("Codejock Software Sample Applications"));

	LoadStdProfileSettings();

	m_pDocManager = new CMSDIDocManager;

	EnableShellOpen();

	if (!ProcessShellCommand())
		return FALSE;

	return TRUE;
}


// App command to run the dialog
void CMSDIApp::OnAppAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
/////////////////////////////////////////////////////////////////////////////
// CMSDIApp message handlers

int CMSDIApp::Run()
{
	return CWinThread::Run();
}

int CMSDIApp::ExitInstance()
{
	if (m_pFrameWnd)
		m_pFrameWnd->DestroyWindow();

	if (m_pDocManager)
		((CMSDIDocManager*)m_pDocManager)->KillAllThreads();

	if (!(m_fCommand & CMD_UNREGSERVER))
		SaveStdProfileSettings();

	return 0;
}


void CMSDIApp::OnExitThread(WPARAM pThread, LPARAM)
{
	((CMSDIDocManager*)m_pDocManager)->OnExitThread((CMSDIThread*)pThread);
}

void CMSDIApp::OnNewInstance(WPARAM nFileOpen, LPARAM)
{
	if (nFileOpen)
	{
		TCHAR szBuf[MAX_PATH];

		if (GlobalGetAtomName((ATOM)nFileOpen, szBuf, sizeof(szBuf)))
			OpenDocumentFile(szBuf);

		GlobalDeleteAtom((ATOM)nFileOpen);
	}
	else
		OnFileNew();
}


BOOL CMSDIApp::ParseCommandLine()
{
	for (int i=1; i < __argc; i++)
	{
		LPCSTR lpszParam = __argv[i];

		if (lpszParam[0] == '-' || lpszParam[0] == '/')
		{
			lpszParam++;
			if (!strcmp(lpszParam, "dde"))
				m_fCommand |= CMD_DDE;
			else if (!stricmp(lpszParam, "nosplash"))
				m_fCommand |= CMD_NOSPLASH;
			else if (!stricmp(lpszParam, "regserver"))
				m_fCommand = CMD_REGSERVER;
			else if (!stricmp(lpszParam, "unregserver"))
				m_fCommand = CMD_UNREGSERVER;
		}
		else if (!(m_fCommand & CMD_OPEN))
		{
			m_strFileOpen = lpszParam;
			m_fCommand |= CMD_OPEN;
		}
	}

	if (m_fCommand & CMD_UNREGSERVER)
		Unregister();
	else
		Register();

	return !(m_fCommand & (CMD_REGSERVER | CMD_UNREGSERVER));
}

BOOL CMSDIApp::ProcessShellCommand()
{
	if (m_fCommand & CMD_DDE)
		return TRUE;

	if (m_fCommand & CMD_OPEN)
		OpenDocumentFile(m_strFileOpen);
	else
		OnFileNew();

	if (((CMSDIDocManager*)m_pDocManager)->IsThreadListEmpty())
		return FALSE;

	return TRUE;
}


BOOL CMSDIApp::IsAlreadyRunning(LPCTSTR lpszName)
{
	ASSERT(lpszName!=NULL);

	LPCTSTR lpszClassName = AfxRegisterWndClass(0, NULL, NULL, NULL);

	HWND hWnd = FindWindow(lpszClassName, lpszName);

	if (hWnd)
	{
		ATOM nFileOpen = NULL;

		if (m_fCommand & CMD_OPEN)
			nFileOpen = GlobalAddAtom(m_strFileOpen);

		DWORD dwThreadID = GetWindowThreadProcessId(hWnd, NULL);

		::PostThreadMessage(dwThreadID, MSDIM_NEW_INSTANCE, (WPARAM)nFileOpen, 0);

		return TRUE;
	}

	m_pFrameWnd = (CFrameWnd*)RUNTIME_CLASS(CFrameWnd)->CreateObject();

	m_pFrameWnd->Create(lpszClassName, lpszName);

	return FALSE;
}


BOOL CMSDIApp::Register()
{
	CString strShort;
	AfxGetModuleShortFileName(m_hInstance, strShort);

	CString strDescr;
	strDescr.LoadString(IDR_MAINFRAME);

	CString strExt, strID, strName;
	AfxExtractSubString(strExt, strDescr, CDocTemplate::filterExt, '\n');
	AfxExtractSubString(strID, strDescr, CDocTemplate::regFileTypeId, '\n');
	AfxExtractSubString(strName, strDescr, CDocTemplate::regFileTypeName, '\n');

	SetRegKey(strExt, strID);
	SetRegKey(strID, strName);
	SetRegKey(strID + _T("\\DefaultIcon"), strShort + _T(",1"));

	SetRegKey(strID + _T("\\shell\\open\\command"), strShort + _T(" /dde"));
	SetRegKey(strID + _T("\\shell\\open\\ddeexec"), _T("[open(\"%1\")]"));

	return TRUE;
}

BOOL CMSDIApp::Unregister()
{
	CWinApp::Unregister();

	CString strDescr;
	strDescr.LoadString(IDR_MAINFRAME);

	CString strExt, strID;
	AfxExtractSubString(strExt, strDescr, CDocTemplate::filterExt, '\n');
	AfxExtractSubString(strID, strDescr, CDocTemplate::regFileTypeId, '\n');

	DelRegTree(HKEY_CLASSES_ROOT, strExt);
	DelRegTree(HKEY_CLASSES_ROOT, strID);

	return TRUE;
}

void CMSDIApp::SetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue)
{
	RegSetValue(HKEY_CLASSES_ROOT, lpszKey, REG_SZ,
		lpszValue, (DWORD)_tcslen(lpszValue));
}


BOOL CMSDIApp::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	CMSDIThread* pThread = (CMSDIThread*)AfxGetThread();

	if (pThread->IsKindOf(RUNTIME_CLASS(CMSDIThread)))
	{
		if (pThread->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}
	return CWinApp::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMSDIApp::OnAppExit()
{
	((CMSDIDocManager*)m_pDocManager)->CloseAllFrames();
}
