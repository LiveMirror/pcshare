// XTResizePropertySheet.cpp: implementation of the CXTResizePropertySheet class.
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
#include "XTDefines.h"
#include "XTUtil.h"
#include "XTGlobal.h"
#include "XTMemDC.h"
#include "XTResizeRect.h"
#include "XTResizePoint.h"
#include "XTPWinThemeWrapper.h"
#include "XTResize.h"
#include "XTResizePropertyPage.h"
#include "XTResizePropertySheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable: 4355) // 'this' : used in base member initializer list

// Wizard mode ID's not in AFX headers
#define ID_WIZLINE      (ID_WIZFINISH + 1)

/////////////////////////////////////////////////////////////////////////////
// CXTResizePropertySheet

IMPLEMENT_DYNCREATE(CXTResizePropertySheet, CPropertySheet)

BEGIN_MESSAGE_MAP(CXTResizePropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CXTResizePropertySheet)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT_EX(PSN_SETACTIVE, OnPageChanging)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTResizePropertySheet construction/destruction

CXTResizePropertySheet::CXTResizePropertySheet() // add default constructor, PCVA - Yair 3/6/2003
: CPropertySheet()
, CXTResize(this, 0)
{

}

CXTResizePropertySheet::CXTResizePropertySheet(const UINT nCaption, CWnd* pParent, const UINT nSelectPage, const UINT nFlags)
: CPropertySheet(nCaption, pParent, nSelectPage)
, CXTResize(this, nFlags)
{

}

CXTResizePropertySheet::CXTResizePropertySheet(LPCTSTR pszCaption, CWnd* pParent, const UINT nSelectPage, const UINT nFlags)
: CPropertySheet(pszCaption, pParent, nSelectPage)
, CXTResize(this, nFlags)
{

}

BOOL CXTResizePropertySheet::OnInitDialog()
{
	// Modify the window style to include WS_THICKFRAME for resizing.
	::SetWindowLong(m_hWnd,
		GWL_STYLE, GetStyle()|WS_THICKFRAME);

	EnableToolTips();

	CPropertySheet::OnInitDialog();

	// subclass our "flicker-free" tab control.
	m_tabCtrl.SubclassWindow(GetTabControl()->m_hWnd);

	// Do this last so that any prop pages are moved accordingly
	if (!HasFlag(xtResizeNoSizeIcon) && !IsWizard())
	{
		// the size icon is too close to the buttons, so inflate the sheet
		CRect rc;
		GetWindowRect(rc);
		rc.InflateRect(1,1,2,2);
		MoveWindow(rc);
	}

	// add sizing entries to the system menu
	CMenu* pSysMenu = (CMenu*)GetSystemMenu(FALSE);
	if(pSysMenu)
	{
		CString szMaximize, szMinimize, szSize, szRestore;
		// try to get the strings from the topmost window
		CWnd* pwndTop;
		for (pwndTop = this; pwndTop->GetParent(); pwndTop = pwndTop->GetParent());

		CMenu* pTopSysMenu = (CMenu*)pwndTop->GetSystemMenu(FALSE);
		if(pTopSysMenu)
		{
			pTopSysMenu->GetMenuString(SC_MAXIMIZE, szMaximize, MF_BYCOMMAND);
			pTopSysMenu->GetMenuString(SC_MINIMIZE, szMinimize, MF_BYCOMMAND);
			pTopSysMenu->GetMenuString(SC_SIZE, szSize, MF_BYCOMMAND);
			pTopSysMenu->GetMenuString(SC_RESTORE, szRestore, MF_BYCOMMAND);
		}
		// if we din't get the strings then set them to the English defaults
		if (szMaximize.IsEmpty()) szMaximize = _T("Ma&ximize");
		if (szMinimize.IsEmpty()) szMinimize = _T("Mi&nimize");
		if (szSize.IsEmpty()) szSize = _T("&Size");
		if (szRestore.IsEmpty()) szRestore = _T("&Restore");

		pSysMenu->InsertMenu(1, MF_BYPOSITION | MF_SEPARATOR, 0, (LPCTSTR) 0);
		pSysMenu->InsertMenu(1, MF_BYPOSITION | MF_STRING, SC_MAXIMIZE, szMaximize);
		pSysMenu->InsertMenu(1, MF_BYPOSITION | MF_STRING, SC_MINIMIZE, szMinimize);
		pSysMenu->InsertMenu(1, MF_BYPOSITION | MF_STRING, SC_SIZE, szSize);
		pSysMenu->InsertMenu(0, MF_BYPOSITION | MF_STRING, SC_RESTORE, szRestore);
	}

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	if ((dwStyle & WS_THICKFRAME) == 0) {
		SetFlag(xtResizeNoSizeIcon);
	}

	CXTResize::Init();

	// Check which buttons are available in sheet or wizard
	if (IsWizard())
	{
		SetResize(ID_WIZBACK, SZ_REPOS(1));
		SetResize(ID_WIZNEXT, SZ_REPOS(1));
		SetResize(ID_WIZFINISH, SZ_REPOS(1));
		SetResize(ID_WIZLINE, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	}
	else
	{
		SetResize(IDOK, SZ_REPOS(1));
		SetResize(ID_APPLY_NOW, SZ_REPOS(1));
		SetResize(AFX_IDC_TAB_CONTROL, SZ_RESIZE(1));
	}
	SetResize(IDCANCEL, SZ_REPOS(1));
	SetResize(IDHELP, SZ_REPOS(1));

	// set page sizings
	CRect rcPage;
	GetActivePage()->GetWindowRect(rcPage);
	ScreenToClient(rcPage);
	int i;
	for (i=0; i<GetPageCount(); i++)
	{
		SetResize(GetPage(i), SZ_RESIZE(1), rcPage);
	}


	return TRUE;
}

void CXTResizePropertySheet::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	CXTResize::GetMinMaxInfo(pMMI);
}

void CXTResizePropertySheet::OnSize(UINT nType, int cx, int cy)
{
	CPropertySheet::OnSize(nType, cx, cy);
	CXTResize::Size();
}

BOOL CXTResizePropertySheet::OnPageChanging(NMHDR* /*pNotifyStruct*/, LRESULT* /*pResult*/)
{
	// When in wizard mode, the property sheet seems to resize/move the page
	// back to it's original size/position on the sheet.  We need to let the
	// size manager know that this has occurred, so that it can move the pages
	// back to the correct position
	if (IsWizard())
	{
		CXTResize::Reset();
	}

	// Update new wizard page, the active page changes after this notification
	CXTResize::Size();

	// and continue routing...
	return FALSE;
}

void CXTResizePropertySheet::OnDestroy()
{
	// Save window placement if AutoLoadPlacement was used.
	if (!m_strSection.IsEmpty()) {
		SavePlacement(m_strSection);
	}

	CPropertySheet::OnDestroy();
}
