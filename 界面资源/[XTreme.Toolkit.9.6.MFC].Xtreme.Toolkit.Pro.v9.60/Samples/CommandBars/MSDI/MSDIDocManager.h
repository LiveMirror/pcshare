// MSDIDocManager.h
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
* You may freely use and modify this code, but don't remove
* this copyright note.
*
* There is no warranty of any kind, express or implied, for this class.
* The author does not take the responsibility for any damage
* resulting from the use of it.
*
* Let me know if you find this code useful, and
* send me any modifications and bug reports.
*
* Copyright (C) 2002-03 by Michal Mecinski
*********************************************************/

#pragma once


// thread messages
#define MSDIM_EXIT_THREAD   (WM_USER + 1)
#define MSDIM_NEW_INSTANCE  (WM_USER + 2)
#define MSDIM_UPDATE_NOTIFY (WM_USER + 3)

// update nofitication codes
#define MSDIN_NEW_THREAD    1
#define MSDIN_EXIT_THREAD   2
#define MSDIN_DOC_TITLE     3

// maximum number of windows in menu
#define MSDI_MAX_WINDOWS    9


#define MSDI_ID_FIRST   AFX_IDM_FIRST_MDICHILD
#define MSDI_ID_LAST    (AFX_IDM_FIRST_MDICHILD+MSDI_MAX_WINDOWS-1)
//#define MSDI_ID_SELECT    (AFX_IDM_FIRST_MDICHILD+MSDI_MAX_WINDOWS)

#include <afxtempl.h>
#include <afxmt.h>

class CMSDIThread;


class CMSDIDocManager : public CDocManager
{
public:
	CMSDIDocManager();
	virtual ~CMSDIDocManager();

// Overrides
public:
	virtual void OnFileNew();
	virtual void OnFileOpen();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle,
			DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);
	virtual BOOL OnDDECommand(LPTSTR lpszCommand);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Implementation
protected:
	CTypedPtrList<CPtrList, CMSDIThread*> m_ThreadList;

	CCriticalSection m_CriticalSection;

	int m_nDocCnt;

protected:
	void OnExitThread(CMSDIThread* pThread);
	void KillAllThreads();

	friend class CMSDIApp;


// Public functions
public:
	// safely close the application
	void CloseAllFrames();


	// post message to all threads
	void PostMessageToAll(UINT message, WPARAM wParam=0, LPARAM lParam=0);

	// post update notify message
	void PostUpdateNotify(UINT nCode, LPARAM lParam=0)
	{
		PostMessageToAll(MSDIM_UPDATE_NOTIFY, nCode, lParam);
	}


	// access to the thread list and thread public data
	void Lock() { m_CriticalSection.Lock(); }
	void Unlock() { m_CriticalSection.Unlock(); }

	// use the following only between Lock() and Unlock()
	BOOL IsThreadListEmpty() { return m_ThreadList.IsEmpty(); }
	POSITION GetFirstThreadPosition() { return m_ThreadList.GetHeadPosition(); }
	CMSDIThread* GetNextThread(POSITION& pos) { return m_ThreadList.GetNext(pos); }
};
