// EclipseHost.cpp : implementation file
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
#include "gui_eclipse.h"
#include "EclipseHost.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEclipseHost

CEclipseHost::CEclipseHost()
{
	LOGFONT lfIcon;
	VERIFY( ::SystemParametersInfo( SPI_GETICONTITLELOGFONT, sizeof( lfIcon ), &lfIcon, 0 ) );

	lfIcon.lfWeight = FW_NORMAL;
	VERIFY( m_fntNormal.CreateFontIndirect(&lfIcon  ) );

	m_bActive = FALSE;
	m_nID = 0;
	m_hIcon = 0;

	m_nHeaderHeight = m_szCaption.cy =  25;
}

CEclipseHost::~CEclipseHost()
{
}
void CEclipseHost::SetHostParams(UINT nID, HICON hIcon)
{
	m_nID = nID;
	m_strTitle.LoadString(nID);
	m_hIcon = hIcon;

	CBitmap bmp;
	if (bmp.LoadBitmap(nID))
	{
		m_wndToolBar.CreateToolBar(WS_CHILD|WS_VISIBLE|CBRS_TOOLTIPS, this, AFX_IDW_TOOLBAR);
		m_wndToolBar.LoadToolBar(nID);

		CMainFrame::AddIcons(nID);
	}

	m_wndHostBar.CreateToolBar(WS_CHILD|WS_VISIBLE|CBRS_TOOLTIPS, this, AFX_IDW_TOOLBAR + 1);
	m_wndHostBar.LoadToolBar(ID_PANE_HOST);

}

BEGIN_MESSAGE_MAP(CEclipseHost, CWnd)
	//{{AFX_MSG_MAP(CEclipseHost)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_PANE_CLOSE, OnPaneClose)
	ON_COMMAND(ID_PANE_HIDE, OnPaneHide)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CEclipseHost message handlers

void CEclipseHost::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{

	if (point.y < m_szCaption.cy)
	{
		CXTPDockingPaneTabbedContainer* pContainer = DYNAMIC_DOWNCAST(CXTPDockingPaneTabbedContainer, GetParent());
		ASSERT(pContainer);

		if (pContainer)
		{
			ClientToScreen(&point);
			pContainer->OnCaptionLButtonDown(point);
		}
	}
}

void CEclipseHost::OnLButtonDblClk(UINT /*nFlags*/, CPoint point)
{
	if (point.y < m_szCaption.cy)
	{
		CXTPDockingPaneTabbedContainer* pContainer = DYNAMIC_DOWNCAST(CXTPDockingPaneTabbedContainer, GetParent());
		ASSERT(pContainer);

		if (pContainer)
		{
			pContainer->GetDockingPaneManager()->ToggleDocking(pContainer);
		}
	}
}

void CEclipseHost::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (!GetClientWnd() || !m_wndHostBar.GetSafeHwnd())
		return;

	CSize szToolBar(0);

	if (m_wndToolBar.GetSafeHwnd())
		szToolBar = m_wndToolBar.CalcDockingLayout(32000, LM_HORZDOCK|LM_HORZ | LM_COMMIT);

	CSize szHostBar = m_wndHostBar.CalcDockingLayout(32000, LM_HORZDOCK|LM_HORZ | LM_COMMIT);


	m_nHeaderHeight = m_szCaption.cy = szHostBar.cy + 1;
	m_szCaption.cx = cx - szToolBar.cx - szHostBar.cx;

	m_wndHostBar.MoveWindow(cx - szHostBar.cx, 0, szHostBar.cx, szHostBar.cy);

	CClientDC dc(this);
	CSize sz = dc.GetTextExtent(m_strTitle);

	if (m_wndToolBar.GetSafeHwnd())
	{

		if (szHostBar.cx + szToolBar.cx < cx - 5 - 16 - sz.cx)
		{
			m_wndToolBar.MoveWindow(cx - szToolBar.cx - szHostBar.cx, 0, szToolBar.cx, szToolBar.cy);

		} else
		{
			szToolBar = m_wndToolBar.CalcDockingLayout(cx, LM_HORZDOCK|LM_HORZ | LM_COMMIT);

			m_szCaption.cx = cx - szHostBar.cx;
			m_nHeaderHeight = m_szCaption.cy + szToolBar.cy;

			m_wndToolBar.MoveWindow(0, m_szCaption.cy, cx, szToolBar.cy);

		}
		m_wndToolBar.Invalidate(FALSE);
	}
	m_wndHostBar.Invalidate(FALSE);

	CWnd* pWnd = GetClientWnd();
	pWnd->MoveWindow(0, m_nHeaderHeight, cx, cy - m_nHeaderHeight);
	pWnd->Invalidate(FALSE);

	Invalidate(FALSE);

}



void CEclipseHost::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting

	CXTPClientRect rc(this);

	CXTPBufferDC dc(dcPaint, rc);

	CXTPFontDC fnt(&dc, &m_fntNormal);

	dc.FillSolidRect(rc, GetXtremeColor(COLOR_WINDOW));


	CRect rcTitle(rc.left, rc.top, rc.right, rc.top + m_szCaption.cy);
	CRect rcFill(rc.left, rc.top, rc.left + m_szCaption.cx, rc.top + m_szCaption.cy);

	dc.FillSolidRect(rcTitle, XTPPaintManager()->GetXtremeColor(XPCOLOR_TOOLBAR_FACE));

	if (m_bActive)
	{
		XTPDrawHelpers()->GradientFill(&dc, rcFill, GetXtremeColor(COLOR_ACTIVECAPTION), XTPPaintManager()->GetXtremeColor(XPCOLOR_TOOLBAR_FACE), TRUE);
	}

	dc.FillSolidRect(rcTitle.left, rcTitle.bottom - 1, rcTitle.Width(), 1, RGB(132, 130, 132));

	if (m_hIcon)
		dc.DrawState(CPoint(rcTitle.left + 5, rcTitle.CenterPoint().y - 8), CSize(16, 16), m_hIcon, 0, (CBrush*)0);

	CRect rcText(rcTitle.left + 5 + 16 + 5, rcTitle.top, rcFill.right, rcTitle.bottom);

	dc.SetTextColor(!m_bActive? 0: GetXtremeColor(COLOR_CAPTIONTEXT));
	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(m_strTitle, rcText, DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

}

BOOL CEclipseHost::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}


LRESULT CEclipseHost::OnIdleUpdateCmdUI(WPARAM, LPARAM)
{
	CWnd* pFocus = GetFocus();
	BOOL bActive = (pFocus->GetSafeHwnd() && (pFocus == this || IsChild(pFocus)));

	if (bActive != m_bActive)
	{
		m_bActive = bActive;
		Invalidate(FALSE);
	}

	return 0L;
}

BOOL CEclipseHost::Create(CWnd* pParentWnd)
{
	return CreateEx(0, _T("STATIC"), _T(""),
		SS_NOTIFY|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0,200, 120), pParentWnd, 0);
}

void CEclipseHost::OnPaneClose()
{
	CXTPDockingPaneTabbedContainer* pContainer = DYNAMIC_DOWNCAST(CXTPDockingPaneTabbedContainer, GetParent());

	if (pContainer && pContainer->GetSelected())
	{
		pContainer->GetSelected()->Close();
	}

}

void CEclipseHost::OnPaneHide()
{
	CXTPDockingPaneTabbedContainer* pContainer = DYNAMIC_DOWNCAST(CXTPDockingPaneTabbedContainer, GetParent());

	if (pContainer && pContainer->GetSelected())
	{
		if (!pContainer->IsHide()) pContainer->GetSelected()->Hide();
			else pContainer->GetDockingPaneManager()->ToggleDocking(pContainer);
	}
}
