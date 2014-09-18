// EditCtrlForFlatTab.cpp : implementation file
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
#include "EditCtrlForFlatTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditCtrlForFlatTab

CEditCtrlForFlatTab::CEditCtrlForFlatTab()
{
}

CEditCtrlForFlatTab::~CEditCtrlForFlatTab()
{
}


BEGIN_MESSAGE_MAP(CEditCtrlForFlatTab, CEdit)
	//{{AFX_MSG_MAP(CEditCtrlForFlatTab)
	ON_WM_HSCROLL()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditCtrlForFlatTab message handlers

void CEditCtrlForFlatTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (m_ScrollBarH.m_hWnd  &&
		pScrollBar->GetSafeHwnd() == m_ScrollBarH.m_hWnd)
	{
		SCROLLINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE;
		m_ScrollBarH.GetScrollInfo(&si);

		int x = m_ScrollBarH.GetScrollPos();
		switch (nSBCode)
		{
			case SB_TOP:
				x = 0;
				break;
			case SB_BOTTOM:
				x = INT_MAX;
				break;
			case SB_LINEUP:
				x--;
				break;
			case SB_LINEDOWN:
				x++;
				break;
			case SB_PAGEUP:
				x -= si.nPage;
				break;
			case SB_PAGEDOWN:
				x += si.nPage;
				break;
			case SB_THUMBTRACK:
				x = nPos;
				break;
		}
		si.nPos = x;
		si.fMask = SIF_POS;
		m_ScrollBarH.SetScrollInfo(&si);
	}

	CEdit::OnHScroll(nSBCode, nPos, pScrollBar);
}

CScrollBar* CEditCtrlForFlatTab::GetScrollBarCtrl(int nBar) const
{
	if (nBar == SB_HORZ)
	{
		CScrollBar *pSB = (CScrollBar *)&m_ScrollBarH;
		SCROLLINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(si);
		if (m_ScrollBarH.m_hWnd == NULL)
		{
			pSB->Create(WS_CHILD, CRect(0, 0, 0, 0), (CWnd *)this, 0);
			si.fMask = SIF_ALL;
			if (::GetScrollInfo(m_hWnd, SB_HORZ, &si))
			{
				pSB->SetScrollInfo (&si);
			}
		}
		else
		{
			si.fMask = SIF_RANGE | SIF_POS;
			::SendMessage(m_hWnd, WM_SETREDRAW, FALSE, 0);
			::ShowScrollBar(m_hWnd, SB_HORZ, TRUE);
			::GetScrollInfo(m_hWnd, SB_HORZ, &si);
			::ShowScrollBar(m_hWnd, SB_HORZ, FALSE);
			::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0);
			pSB->SetScrollInfo (&si);
		}
		return pSB;
	}

	return CEdit::GetScrollBarCtrl(nBar);
}

int CEditCtrlForFlatTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}

void CEditCtrlForFlatTab::OnSize(UINT nType, int cx, int cy)
{
	CEdit::OnSize(nType, cx, cy);

	if (m_ScrollBarH.m_hWnd)
	{
		SCROLLINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(si);
		CRect r;
		GetClientRect(r);
		si.fMask = SIF_PAGE;
		si.nPage = r.Width() - 1;
		m_ScrollBarH.SetScrollInfo(&si);
	}
}

void CEditCtrlForFlatTab::OnUpdate()
{
	CXTFlatTabCtrl *pParent = (CXTFlatTabCtrl *)GetParent();
	ASSERT_KINDOF(CXTFlatTabCtrl, pParent);
	pParent->SyncScrollBar();
}

void CEditCtrlForFlatTab::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	OnUpdate();
}
