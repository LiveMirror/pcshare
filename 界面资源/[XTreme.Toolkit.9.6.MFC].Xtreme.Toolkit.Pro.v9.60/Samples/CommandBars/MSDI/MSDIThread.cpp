// MSDIThread.cpp
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
#include "MSDI.h"
#include "MSDIThread.h"
#include "MSDIDocManager.h"

#include "MainFrm.h"
#include "SomeDoc.h"
#include "SomeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMSDIThread

IMPLEMENT_DYNCREATE(CMSDIThread, CWinThread)

CMSDIThread::CMSDIThread()
{
	m_pDocManager = NULL;
	m_pWndDlg = NULL;

	m_eventTerminate.ResetEvent();
}

CMSDIThread::~CMSDIThread()
{
}


BOOL CMSDIThread::InitInstance()
{
	m_pDocManager = new CDocManager();

	// moved from CMSDIApp::InitInstance

	CSingleDocTemplate* pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSomeDoc),
		RUNTIME_CLASS(CMainFrame),
		RUNTIME_CLASS(CSomeView));

	m_pDocManager->AddDocTemplate(pDocTemplate);

	if (m_strFileOpen.IsEmpty())
	{
		m_pDocManager->OnFileNew();

		if (m_pMainWnd)
		{
			CDocument* pDoc = ((CFrameWnd*)m_pMainWnd)->GetActiveDocument();
			if (pDoc)
			{
				// append document number like in MDI
				CString str;
				str.Format(_T("%s%d"), pDoc->GetTitle(), m_nDocCnt);
				pDoc->SetTitle(str);
			}
		}
	}
	else
		m_pDocManager->OpenDocumentFile(m_strFileOpen);

	if (!m_pMainWnd)
		return FALSE;

	m_pMainWnd->BringWindowToTop();

	return TRUE;
}

int CMSDIThread::ExitInstance()
{
	if (m_pMainWnd)
		m_pMainWnd->DestroyWindow();

	delete m_pDocManager;

	AfxGetApp()->PostThreadMessage(MSDIM_EXIT_THREAD, (WPARAM)this, 0);

	// wait for permission to terminate
	WaitForSingleObject(m_eventTerminate, INFINITE);

	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CMSDIThread, CWinThread)
	//{{AFX_MSG_MAP(CMSDIThread)
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(MSDIM_UPDATE_NOTIFY, OnUpdateNotify)
	ON_COMMAND_RANGE(MSDI_ID_FIRST, MSDI_ID_LAST, OnWindowActivate)
	ON_COMMAND(MSDI_ID_SELECT, OnWindowSelect)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWindowDlg dialog used for Window Select

class CWindowDlg : public CDialog
{
// Construction
public:
	CWindowDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWindowDlg)
	enum { IDD = IDD_WINDOW };
	CListBox    m_lbWndList;
	//}}AFX_DATA

	int m_nResult;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindowDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWindowDlg)
	virtual void OnOK();
	afx_msg void OnDblclkWindowsList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CWindowDlg dialog

CWindowDlg::CWindowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWindowDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWindowDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindowDlg)
	DDX_Control(pDX, IDC_WINDOW_LIST, m_lbWndList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWindowDlg, CDialog)
	//{{AFX_MSG_MAP(CWindowDlg)
	ON_LBN_DBLCLK(IDC_WINDOW_LIST, OnDblclkWindowsList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindowDlg message handlers

void CWindowDlg::OnOK()
{
	m_nResult = m_lbWndList.GetCurSel();

	CDialog::OnOK();
}

void CWindowDlg::OnDblclkWindowsList()
{
	OnOK();
}


/////////////////////////////////////////////////////////////////////////////
// CMSDIThread message handlers

void CMSDIThread::OnUpdateNotify(WPARAM nCode, LPARAM /*lParam*/)
{
	switch (nCode)
	{
	// no need to react on MSDIN_NEW_THREAD because
	// MSDIN_DOC_TITLE is always sent after it
	case MSDIN_EXIT_THREAD:
	case MSDIN_DOC_TITLE:
		// the window dialog will automatically reopen with updated list
		if (m_pWndDlg && m_pWndDlg->m_hWnd && m_pWndDlg->m_lbWndList.m_hWnd)
			FillWindowList();
		break;
	}
}


void CMSDIThread::SetDocumentTitle(LPCTSTR lpszTitle)
{
	CMSDIDocManager* pDocMgr = (CMSDIDocManager*)AfxGetApp()->m_pDocManager;

	pDocMgr->Lock();

	// change the shared data safely
	m_strTitle = lpszTitle;

	pDocMgr->Unlock();

	// send notification message to all threads
	pDocMgr->PostUpdateNotify(MSDIN_DOC_TITLE);
}


void CMSDIThread::OnWindowActivate(UINT uCmdID)
{
	CMSDIDocManager* pDocMgr = (CMSDIDocManager*)AfxGetApp()->m_pDocManager;

	pDocMgr->Lock();

	POSITION pos = pDocMgr->GetFirstThreadPosition();

	int nCnt = uCmdID - MSDI_ID_FIRST;

	while (pos)
	{
		CMSDIThread* pThread = pDocMgr->GetNextThread(pos);

		if (nCnt == 0)
		{
			// restore if it was maximized and switch to foreground
			pThread->GetMainWnd()->ShowWindow(SW_RESTORE);
			pThread->GetMainWnd()->SetForegroundWindow();
			break;
		}

		nCnt--;
	}

	pDocMgr->Unlock();
}


void CMSDIThread::OnWindowSelect()
{
	CWindowDlg dlg;

	m_pWndDlg = &dlg;

	// fill the list after the window is created
	PostThreadMessage(MSDIM_UPDATE_NOTIFY, MSDIN_DOC_TITLE, 0);

	int nRes = (int)dlg.DoModal();

	m_pWndDlg = NULL;

	if (nRes==IDOK && dlg.m_nResult>=0)
		OnWindowActivate(MSDI_ID_FIRST + dlg.m_nResult);
}


void CMSDIThread::FillWindowList()
{
	CMSDIDocManager* pDocMgr = (CMSDIDocManager*)AfxGetApp()->m_pDocManager;

	pDocMgr->Lock();

	POSITION pos = pDocMgr->GetFirstThreadPosition();

	m_pWndDlg->m_lbWndList.ResetContent();

	int nCnt = 0;

	while (pos)
	{
		CMSDIThread* pThread = pDocMgr->GetNextThread(pos);

		m_pWndDlg->m_lbWndList.AddString(pThread->m_strTitle);

		if (pThread == this)
			m_pWndDlg->m_lbWndList.SetCurSel(nCnt);

		nCnt++;
	}

	m_pWndDlg->m_lbWndList.SetFocus();

	pDocMgr->Unlock();
}


IMPLEMENT_XTP_CONTROL( CControlWindowList, CXTPControlButton)

void CControlWindowList::OnCalcDynamicSize(DWORD /*dwMode*/)
{
	ASSERT(m_pControls->GetAt(m_nIndex) == this);

	while (m_nIndex + 1 < m_pControls->GetCount())
	{
		CXTPControl* pControl = m_pControls->GetAt(m_nIndex + 1);
		if (pControl->GetID() >= (int)MSDI_ID_FIRST && pControl->GetID() <= (int)MSDI_ID_LAST)
		{
			m_pControls->Remove(pControl);
		}
		else break;
	}

	if (m_pParent->IsCustomizeMode())
	{
		m_dwHideFlags = 0;
		return;
	}

	m_dwHideFlags |= xtpHideGeneric;

	CMSDIDocManager* pDocMgr = (CMSDIDocManager*)AfxGetApp()->m_pDocManager;

	// obtain access to the thread list
	pDocMgr->Lock();

	POSITION pos = pDocMgr->GetFirstThreadPosition();
	int nCnt = 0;

	while (pos && nCnt < MSDI_MAX_WINDOWS)
	{
		CMSDIThread* pThread = pDocMgr->GetNextThread(pos);

		// title can be accessed safely now
		CString strItem;
		strItem.Format(_T("&%d %s"), nCnt+1, pThread->m_strTitle);

		CXTPControl* pControl = m_pControls->Add(xtpControlButton, MSDI_ID_FIRST + nCnt, _T(""), m_nIndex + 1 + nCnt, TRUE);

		pControl->SetCaption(strItem);
		pControl->SetBeginGroup(nCnt == 0);
		pControl->SetChecked(pThread == AfxGetThread()? TRUE: FALSE);

		pControl->SetFlags(xtpFlagManualUpdate);

		nCnt++;
	}

	pDocMgr->Unlock();
}

BOOL CControlWindowList::IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint /*point*/, DROPEFFECT& dropEffect)
{
	if (pCommandBar->GetType() != xtpBarTypePopup)
	{
		dropEffect = DROPEFFECT_NONE;
		return FALSE;
	}
	return TRUE;
}
