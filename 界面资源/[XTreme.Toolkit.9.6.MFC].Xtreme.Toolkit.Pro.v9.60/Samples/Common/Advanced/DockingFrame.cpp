// DockingFrame.cpp : implementation file
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
#include "advanced.h"
#include "DockingFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDockingFrame

IMPLEMENT_DYNCREATE(CDockingFrame, CFrameWnd)

CDockingFrame::CDockingFrame()
{
}

CDockingFrame::~CDockingFrame()
{
}


BEGIN_MESSAGE_MAP(CDockingFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CDockingFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_INSERT, OnButtonInsert)
	ON_COMMAND(ID_BUTTON_CLEAR, OnButtonClear)
	ON_COMMAND(ID_BUTTON_DELETE, OnButtonDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDockingFrame message handlers

int CDockingFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;



	if (!m_wndView.CreateEx(WS_EX_STATICEDGE, _T("LISTBOX"), NULL,
		WS_CHILD|WS_VISIBLE|LBS_NOINTEGRALHEIGHT|WS_VSCROLL, CRect(0, 0, 0, 0),  this, AFX_IDW_PANE_FIRST))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	m_wndView.SetFont(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
	CString strText;
	int iItem;
	for (iItem = 1; iItem < 51; ++iItem)
	{
		strText.Format(_T("ListBox Item %d"), iItem);
		m_wndView.AddString(strText);   }


	if (!InitCommandBars())
		return -1;

	CXTPCommandBars* pCommandBars = GetCommandBars();
	pCommandBars->GetCommandBarsOptions()->bShowExpandButtonAlways = FALSE;

	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Docking"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_DOCKINGFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	return 0;
}

void CDockingFrame::OnButtonInsert()
{
	int nIndex = m_wndView.GetCurSel();
	CString strText;
	strText.Format(_T("ListBox Item %d"), m_wndView.GetCount());

	nIndex = nIndex == -1? m_wndView.GetCount(): nIndex ;
	m_wndView.InsertString(nIndex, strText);

}

void CDockingFrame::OnButtonClear()
{
	m_wndView.ResetContent();

}

void CDockingFrame::OnButtonDelete()
{
	int nIndex = m_wndView.GetCurSel();
	if (nIndex != -1) m_wndView.DeleteString(nIndex);

}
