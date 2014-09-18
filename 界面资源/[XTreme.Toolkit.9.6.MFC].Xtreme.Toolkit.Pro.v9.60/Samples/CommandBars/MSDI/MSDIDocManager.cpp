// MSDIDocManager.cpp
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
#include "MSDIDocManager.h"
#include "MSDIThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMSDIDocManager

CMSDIDocManager::CMSDIDocManager()
{
	m_nDocCnt = 1;
}

CMSDIDocManager::~CMSDIDocManager()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMSDIDocManager diagnostics

#ifdef _DEBUG
void CMSDIDocManager::AssertValid() const
{
	CDocManager::AssertValid();
}

void CMSDIDocManager::Dump(CDumpContext& dc) const
{
	CDocManager::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMSDIDocManager implementation

void CMSDIDocManager::OnExitThread(CMSDIThread* pThread)
{
	m_CriticalSection.Lock();

	// remove the thread from the list
	POSITION pos = m_ThreadList.Find(pThread);
	ASSERT(pos != NULL);
	m_ThreadList.RemoveAt(pos);

	// check if no more threads
	if (m_ThreadList.IsEmpty())
		PostQuitMessage(0);

	HANDLE hThread = pThread->m_hThread;

	// prevent from CWndThread from closing the handle
	pThread->m_hThread = NULL;

	// let the thread exit
	pThread->m_eventTerminate.SetEvent();

	// wait until it terminates
	WaitForSingleObject(hThread, INFINITE);

	// free the thread
	CloseHandle(hThread);

	m_CriticalSection.Unlock();

	PostUpdateNotify(MSDIN_EXIT_THREAD);
}


void CMSDIDocManager::OnFileNew()
{
	OpenDocumentFile(NULL);
	return;
}

void CMSDIDocManager::OnFileOpen()
{
	CString newName;

	// allow selecting multiple files
	if (!DoPromptFileName(newName, AFX_IDS_OPENFILE,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, TRUE, NULL))
		return;

	CString strPath, strFile;

	// extract path part
	AfxExtractSubString(strPath, newName, 0, '\0');

	CString strLast = strPath.Right(1);
	if (strLast != '\\' && strLast != '/')
		strPath += '\\';

	for (int i=1; ; i++)
	{
		AfxExtractSubString(strFile, newName, i, '\0');

		if (strFile.IsEmpty())
		{
			if (i == 1) // no multiselect
				AfxGetApp()->OpenDocumentFile(newName);
			break;
		}

		// build full file path
		strFile = strPath + strFile;

		AfxGetApp()->OpenDocumentFile(strFile);
	}

}


CDocument* CMSDIDocManager::OpenDocumentFile(LPCTSTR lpszFileName)
{
	CMSDIThread* pThread = (CMSDIThread*)AfxGetThread();

	if (lpszFileName && pThread->IsKindOf(RUNTIME_CLASS(CMSDIThread)))
	{
		// get current thread's main window
		CFrameWnd* pWnd = (CFrameWnd*)pThread->GetMainWnd();

		if (pWnd)
		{
			CDocument* pDoc = pWnd->GetActiveDocument();

			// is the document empty?
			if (pDoc && pDoc->GetPathName().IsEmpty() && !pDoc->IsModified())
			{
				// replace it instead of creating a new one
				return pThread->m_pDocManager->OpenDocumentFile(lpszFileName);
			}
		}
	}

	CMSDIThread* pNewThread = (CMSDIThread*)RUNTIME_CLASS(CMSDIThread)->CreateObject();

	if (lpszFileName)   // pass file path to the new thread
		pNewThread->m_strFileOpen = lpszFileName;
	else                // number of the new document
		pNewThread->m_nDocCnt = m_nDocCnt++;

	m_CriticalSection.Lock();

	if (pNewThread->CreateThread())
		m_ThreadList.AddTail(pNewThread);

	m_CriticalSection.Unlock();

	PostUpdateNotify(MSDIN_NEW_THREAD);

	return NULL;
}


BOOL CMSDIDocManager::DoPromptFileName(CString& fileName, UINT nIDSTitle,
	DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate)
{
	CMSDIThread* pThread = (CMSDIThread*)AfxGetThread();
	ASSERT_KINDOF(CMSDIThread, pThread);

	CDocManager* pDocMgr = pThread->m_pDocManager;
	ASSERT_KINDOF(CDocManager, pDocMgr);

	return pDocMgr->DoPromptFileName(fileName, nIDSTitle, lFlags, bOpenFileDialog, pTemplate);
}


BOOL CMSDIDocManager::OnDDECommand(LPTSTR lpszCommand)
{
	CString strCommand = lpszCommand;

	if (strCommand.Left(7) == _T("[open(\""))
		strCommand = strCommand.Mid(7);
	else
		return FALSE;

	int i = strCommand.Find('"');
	if (i<0)
		return FALSE;

	strCommand.ReleaseBuffer(i);

	OpenDocumentFile(strCommand);

	return TRUE;
}


void CMSDIDocManager::KillAllThreads()
{
	m_CriticalSection.Lock();

	while (!m_ThreadList.IsEmpty())
	{
		HANDLE hArray[MAXIMUM_WAIT_OBJECTS];

		int nCnt = 0;

		POSITION pos = m_ThreadList.GetHeadPosition();

		while (pos && nCnt < MAXIMUM_WAIT_OBJECTS)
		{
			POSITION posPrev = pos;

			CMSDIThread* pThread = m_ThreadList.GetNext(pos);

			m_ThreadList.RemoveAt(posPrev);

			hArray[nCnt++] = pThread->m_hThread;

			pThread->m_hThread = NULL;
			pThread->m_eventTerminate.SetEvent();

			PostThreadMessage(pThread->m_nThreadID, WM_QUIT, 0, 0);
		}

		// unlock now to prevent deadlock
		m_CriticalSection.Unlock();

		// wait until all remaining threads terminate
		WaitForMultipleObjects(nCnt, hArray, TRUE, INFINITE);

		for (int i=0; i<nCnt; i++)
			CloseHandle(hArray[i]);

		// check once again if there are no threads remaining
		// or created in the meantime
		m_CriticalSection.Lock();
	}

	// there are no remaining threads now, we can safely exit
	m_CriticalSection.Unlock();
}

void CMSDIDocManager::CloseAllFrames()
{
	POSITION pos = m_ThreadList.GetHeadPosition();

	while (pos)
	{
		CWinThread* pThread = m_ThreadList.GetNext(pos);
		pThread->GetMainWnd()->PostMessage(WM_CLOSE);
	}
}

void CMSDIDocManager::PostMessageToAll(UINT message, WPARAM wParam, LPARAM lParam)
{
	m_CriticalSection.Lock();

	POSITION pos = m_ThreadList.GetHeadPosition();

	while (pos)
	{
		CWinThread* pThread = m_ThreadList.GetNext(pos);
		pThread->PostThreadMessage(message, wParam, lParam);
	}

	m_CriticalSection.Unlock();
}
