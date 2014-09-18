// MSDIThread.h
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


#include <afxmt.h>

class CMSDIDocManager;
class CWindowDlg;


class CMSDIThread : public CWinThread
{
	DECLARE_DYNCREATE(CMSDIThread)
protected:
	CMSDIThread();
	virtual ~CMSDIThread();

// Attributes
public:

// Operations
public:
	void SetDocumentTitle(LPCTSTR lpszTitle);

// Overrides
	//{{AFX_VIRTUAL(CMSDIThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void FillWindowList();

	void UpdateWindowMenu();

	//{{AFX_MSG(CMSDIThread)
	//}}AFX_MSG
	afx_msg void OnUpdateNotify(WPARAM nCode, LPARAM lParam);
	afx_msg void OnWindowActivate(UINT uCmdID);
	afx_msg void OnWindowSelect();
	DECLARE_MESSAGE_MAP()

public:
	// initial parameters
	CString m_strFileOpen;
	int m_nDocCnt;

	// permission to terminate
	CEvent m_eventTerminate;

	// own document manager
	CDocManager* m_pDocManager;

protected:
	// window list dialog box
	CWindowDlg* m_pWndDlg;


public:
	// SHARED DATA
	// only modify and read from other threads within Lock/Unlock

	// document title
	CString m_strTitle;
};



class CControlWindowList : public CXTPControlButton
{
protected:

	virtual void OnCalcDynamicSize(DWORD dwMode);
	virtual BOOL IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint point, DROPEFFECT& dropEffect);

private:
	DECLARE_XTP_CONTROL(CControlWindowList)
};
