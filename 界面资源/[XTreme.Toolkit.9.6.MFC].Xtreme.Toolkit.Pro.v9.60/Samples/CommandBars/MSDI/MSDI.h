// MSDI.h
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

#include "resource.h"

// Command line flags
#define CMD_DDE         0x01
#define CMD_OPEN        0x02
#define CMD_NOSPLASH    0x04
#define CMD_REGSERVER   0x08
#define CMD_UNREGSERVER 0x10


class CMSDIApp : public CWinApp
{
public:
	CMSDIApp();

// Overrides
	//{{AFX_VIRTUAL(CSomeApp)
	public:
	virtual BOOL InitInstance();
	virtual int Run();
	virtual int ExitInstance();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMSDIApp)
	afx_msg void OnAppAbout();
	afx_msg void OnAppExit();
	//}}AFX_MSG
protected:
	afx_msg void OnExitThread(WPARAM pThread, LPARAM);
	afx_msg void OnNewInstance(WPARAM nFileOpen, LPARAM);
	DECLARE_MESSAGE_MAP()

protected:
	BOOL ParseCommandLine();
	BOOL ProcessShellCommand();

	BOOL IsAlreadyRunning(LPCTSTR lpszName);

	BOOL Register();
	BOOL Unregister();

	void SetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue);

	int m_fCommand;
	CString m_strFileOpen;

	CFrameWnd* m_pFrameWnd;
};

//{{AFX_INSERT_LOCATION}}
