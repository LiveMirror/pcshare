// XTPDockingPaneMiniWnd.cpp : implementation of the CXTPDockingPaneMiniWnd class.
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
#include "XTPDrawHelpers.h"
#include "XTPTabManager.h"

#include "XTPDockingPaneMiniWnd.h"
#include "XTPDockingPane.h"
#include "XTPDockingPaneSplitterContainer.h"
#include "XTPDockingPaneTabbedContainer.h"
#include "XTPDockingPaneContext.h"
#include "XTPDockingPaneManager.h"
#include "XTPDockingPaneLayout.h"
#include "XTPDockingPaneResource.h"
#include "XTPDockingPaneAutoHidePanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL CXTPDockingPaneMiniWnd::m_bShowPinButton = TRUE;

#define TID_NCLEAVE 2345

#define TID_CHECKACTIVE 1
#define TID_SLIDEIN 2
#define TID_SLIDEOUT 3

IMPLEMENT_DYNAMIC(CXTPDockingPaneMiniWnd, CMiniFrameWnd)

CXTPDockingPaneMiniWnd::CXTPDockingPaneMiniWnd(CXTPDockingPaneLayout* pLayout)
	: CXTPDockingPaneBase(xtpPaneTypeMiniWnd, pLayout)
{
	m_pTopContainer = 0;

	m_arrCaptionButtons.Add(new CXTPDockingPaneCaptionButton(XTP_IDS_DOCKINGPANE_CLOSE, this));
	m_arrCaptionButtons.Add(new CXTPDockingPaneCaptionButton(XTP_IDS_DOCKINGPANE_AUTOHIDE, this));

	m_bCollapsed = FALSE;
	m_nExpandedHeight = 0;

	m_nStepsCount =
		CXTPDockingPaneAutoHideWnd::m_nAnimationDuration / CXTPDockingPaneAutoHideWnd::m_nAnimationInterval;
	m_nSlideStep = 0;
	m_bSlideOut = FALSE;
}

void CXTPDockingPaneMiniWnd::Init(CXTPDockingPaneBase* pPane, CRect rc)
{
	Create(rc);

	if (pPane->GetType() == xtpPaneTypeDockingPane)
	{
		CXTPDockingPaneTabbedContainer* pContainer = (CXTPDockingPaneTabbedContainer*)GetDockingPaneManager()->
			OnCreatePane(xtpPaneTypeTabbedContainer, m_pLayout);
		pContainer->Init((CXTPDockingPane*)pPane, this);

		pPane = pContainer;

	}

	m_pTopContainer = (CXTPDockingPaneSplitterContainer*)GetDockingPaneManager()->
		OnCreatePane(xtpPaneTypeSplitterContainer, m_pLayout);
	m_pTopContainer->Init(pPane, TRUE, this);

		//(pPane, TRUE, this, GetDockingPaneManager()->GetCurrentLayout());
	m_pTopContainer->m_pParentContainer = this;

	OnChildConainerChanged(m_pTopContainer);
	DelayRecalcLayout();
}

CXTPDockingPaneMiniWnd::~CXTPDockingPaneMiniWnd()
{
	DestroyWindow();
}

void CXTPDockingPaneMiniWnd::Copy(CXTPDockingPaneMiniWnd* pClone, CXTPPaneToPaneMap* pMap)
{
	ASSERT(pMap);

	m_pTopContainer = (CXTPDockingPaneSplitterContainer*)pClone->m_pTopContainer->Clone(m_pLayout, pMap);
	m_pTopContainer->m_pParentContainer  = this;
	m_pTopContainer->m_pParentFrame  = this;

	m_rcWindow = pClone->m_rcWindow;
	if (pClone->m_hWnd) ::GetWindowRect(pClone->m_hWnd, &m_rcWindow);

	m_bCollapsed = ((CXTPDockingPaneMiniWnd*)pClone)->m_bCollapsed;
	m_nExpandedHeight = ((CXTPDockingPaneMiniWnd*)pClone)->m_nExpandedHeight;

	if (m_bCollapsed)
	{
		m_rcWindow.bottom = m_rcWindow.top + GetDockingPaneManager()->GetPaintManager()->GetCaptionHeight() + 8;
	}

	if (!m_pLayout->IsUserLayout())
	{
		Create(m_rcWindow);
		m_pTopContainer->OnParentConainerChanged(m_pTopContainer);
		OnChildConainerChanged(m_pTopContainer);
	}
}


CXTPDockingPaneBase* CXTPDockingPaneMiniWnd::Clone(CXTPDockingPaneLayout* pLayout, CXTPPaneToPaneMap* pMap, DWORD /*dwIgnoredOptions*/)
{
	ASSERT(pMap);

	CXTPDockingPaneMiniWnd* pPane = (CXTPDockingPaneMiniWnd*)GetDockingPaneManager()->OnCreatePane(GetType(), pLayout);

	pPane->Copy(this, pMap);

	SAFE_CALLPTR(pMap, SetAt(this, pPane));

	return pPane;


}

void CXTPDockingPaneMiniWnd::RemovePane(CXTPDockingPaneBase* pPane)
{
	ASSERT(pPane == m_pTopContainer);

	pPane->m_pParentContainer = NULL;
	m_pTopContainer = NULL;

	if (m_hWnd) ShowWindow(SW_HIDE);

	if (m_bCollapsed)
		OnPinButtonClick();

}

void CXTPDockingPaneMiniWnd::OnChildConainerChanged(CXTPDockingPaneBase* pContainer)
{
	UNREFERENCED_PARAMETER(pContainer);
	ASSERT(m_pTopContainer);

	if (!m_hWnd)
		return;

	CXTPDockingPaneBaseList lst;

	m_pTopContainer->FindPane(xtpPaneTypeTabbedContainer, &lst);
	int nCount = (int)lst.GetCount();

	POSITION pos = lst.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane = lst.GetNext(pos);
		ASSERT(pPane->GetType() == xtpPaneTypeTabbedContainer);
		((CXTPDockingPaneTabbedContainer*)pPane)->ShowTitle(nCount != 1);
	}
	CString strTitle = (nCount != 1)? GetDockingPaneManager()->m_strFloatingFrameCaption: ((CXTPDockingPaneTabbedContainer*)lst.GetHead())->GetTitle();
	SetWindowText(strTitle);

	if ((GetStyle() & WS_VISIBLE) == 0 && nCount > 0 && m_hWnd)
		ShowWindow(SW_SHOW);

	if (nCount > 0)
	{
		BOOL bEnabled = FALSE;

		CXTPDockingPaneBaseList lst;
		m_pTopContainer->FindPane(xtpPaneTypeDockingPane, &lst);
		POSITION pos = lst.GetHeadPosition();
		while (pos)
		{
			CXTPDockingPaneBase* pPane = lst.GetNext(pos);

			if ((((CXTPDockingPane*)pPane)->GetOptions() & xtpPaneNoCloseable) == 0)
				bEnabled = TRUE;
		}

		if (!bEnabled || !m_bCloseEnabled)
		{
			CMenu* pSysMenu = GetSystemMenu(FALSE);
			pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | (bEnabled? MF_ENABLED: MF_DISABLED));
			m_bCloseEnabled = bEnabled;
		}
		MoveWindow(CXTPWindowRect(this));
	}
	SendMessage(WM_NCPAINT);
}


void CXTPDockingPaneMiniWnd::RecalcLayout(BOOL bNotify)
{
	m_nIdleFlags &= ~(idleLayout|idleNotify);

	UNREFERENCED_PARAMETER(bNotify);
	if (m_pTopContainer == NULL)
		return;

	if (m_pTopContainer->IsEmpty())
	{
		ShowWindow(SW_HIDE);

		if (m_bCollapsed)
			OnPinButtonClick();
	}
	else
	{
		//ShowWindow(SW_SHOW);

		AFX_SIZEPARENTPARAMS layout;
		layout.bStretch = TRUE;
		GetClientRect(&layout.rect);
		layout.hDWP = ::BeginDeferWindowPos(8); // reasonable guess

		m_pTopContainer->OnSizeParent(this, layout.rect, &layout);

		if (layout.hDWP == NULL || !::EndDeferWindowPos(layout.hDWP))
			TRACE0("Warning: DeferWindowPos failed - low system resources.\n");

		CWnd::GetWindowRect(&m_pTopContainer->m_rcWindow);
		ScreenToClient(&m_pTopContainer->m_rcWindow);
	}

	GetDockingPaneManager()->SyncPanesState();
}


BOOL CXTPDockingPaneMiniWnd::Create(CRect rc)
{
	DWORD dwStyle = WS_POPUP|WS_CAPTION|WS_SYSMENU|WS_BORDER|
		/*MFS_4THICKFRAME|*/MFS_SYNCACTIVE|MFS_BLOCKSYSMENU|WS_THICKFRAME;

	DWORD dwExStyle = 0;

	if (GetDockingPaneManager()->IsLayoutRTL())
	{
		dwExStyle =  WS_EX_LAYOUTRTL;
	}

	CRect rcWork = CXTPDrawHelpers::GetWorkArea(rc);
	if (!CRect().IntersectRect(rcWork, rc))
	{
		int nXOffset = (rc.right < rcWork.left) ? (rcWork.left - rc.left) :
			(rc.left > rcWork.right) ? (rcWork.right - rc.right) : 0;
		int nYOffset = (rc.bottom < rcWork.top) ? (rcWork.top - rc.top) :
			(rc.top > rcWork.bottom) ? (rcWork.bottom - rc.bottom) : 0;
		rc.OffsetRect(nXOffset, nYOffset);
	}


	if (!CMiniFrameWnd::CreateEx(dwExStyle,
		NULL, _T(""), dwStyle, rc, GetDockingPaneManager()->GetSite()))
	{
		return FALSE;
	}
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	pSysMenu->DeleteMenu(SC_MINIMIZE, MF_BYCOMMAND);
	pSysMenu->DeleteMenu(SC_MAXIMIZE, MF_BYCOMMAND);
	pSysMenu->DeleteMenu(SC_RESTORE, MF_BYCOMMAND);
	CString strHide;
	if (strHide.LoadString(AFX_IDS_HIDE))
	{
		pSysMenu->DeleteMenu(SC_CLOSE, MF_BYCOMMAND);
		pSysMenu->AppendMenu(MF_STRING|MF_ENABLED, SC_CLOSE, strHide);
	}
	m_bCloseEnabled = TRUE;

	if (IsThemed())
	{
		ModifyStyle(WS_BORDER, 0, SWP_FRAMECHANGED);
	}

	return TRUE;


}

BEGIN_MESSAGE_MAP(CXTPDockingPaneMiniWnd, CMiniFrameWnd)
	ON_WM_CLOSE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_DESTROY()

	ON_WM_NCHITTEST()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_NCMOUSEMOVE()
	ON_WM_TIMER()
	ON_MESSAGE_VOID(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_GETMINMAXINFO()

END_MESSAGE_MAP()

void CXTPDockingPaneMiniWnd::OnIdleUpdateCmdUI()
{
	if (!CWnd::GetSafeHwnd())
		return;

	if (IsThemed())
	{
		CWnd* pFocus = GetFocus();
		BOOL bActive = (pFocus->GetSafeHwnd() && (pFocus == this || IsChild(pFocus)));

		if (bActive != m_bActive)
		{
			m_bActive = bActive;
			SendMessage(WM_NCPAINT);
		}
	}


	CMiniFrameWnd::OnIdleUpdateCmdUI();
}


void CXTPDockingPaneMiniWnd::OnClose()
{
	if (m_pTopContainer)
	{
		CXTPDockingPaneBaseList lst;
		m_pTopContainer->FindPane(xtpPaneTypeDockingPane, &lst);
		POSITION pos = lst.GetHeadPosition();
		while (pos)
		{
			CXTPDockingPane* pPane = (CXTPDockingPane*)lst.GetNext(pos);

			if ((pPane->GetOptions() & xtpPaneNoCloseable) != 0)
				continue;

			if (GetDockingPaneManager()->_OnAction(xtpPaneActionClosing, pPane))
				continue;

			pPane->Close();

			GetDockingPaneManager()->_OnAction(xtpPaneActionClosed, pPane);
		}
	}
}

//void CXTPDockingPaneMiniWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
//{
//}

void CXTPDockingPaneMiniWnd::PostNcDestroy()
{
	// prevent auto deleting
}

void CXTPDockingPaneMiniWnd::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	UNREFERENCED_PARAMETER(point);
	if (nHitTest == HTCAPTION)
	{
		if (m_bCollapsed)
			OnPinButtonClick();
		else
			GetDockingPaneManager()->ToggleDocking(m_pTopContainer);
	}
}

void CXTPDockingPaneMiniWnd::OnDestroy()
{
	::GetWindowRect(m_hWnd, &m_rcWindow);
	CWnd::OnDestroy();
	// TODO: Add your message handler code here
}


BOOL CXTPDockingPaneMiniWnd::IsThemed()
{
	return GetDockingPaneManager()->IsThemedFloatingFrames();
}

//////////////////////////////////////////////////////////////////////////
// Themed methods.

UINT CXTPDockingPaneMiniWnd::OnNcHitTest(CPoint point)
{
	UINT nHitTest = CMiniFrameWnd::OnNcHitTest(point);

	if (!IsThemed())
		return nHitTest;

	if (nHitTest == HTNOWHERE)
	{
		int nPoint = point.y - CXTPWindowRect(this).top;
		if (nPoint < GetDockingPaneManager()->GetPaintManager()->GetCaptionHeight() + 4)
			return HTCAPTION;
	}

	return nHitTest;
}

void CXTPDockingPaneMiniWnd::RedrawPane()
{
	if (!IsThemed())
		return;

	if (CWnd::GetSafeHwnd())
		::InvalidateRect(CWnd::GetSafeHwnd(), 0, FALSE);

	SendMessage(WM_NCPAINT);
}

void CXTPDockingPaneMiniWnd::OnNcPaint()
{
	if (IsThemed())
	{
		CXTPWindowRect rc(this);
		rc.OffsetRect(-rc.TopLeft());
		CWindowDC dc(this);

		CXTPDockingPanePaintManager* pPaintManager = GetDockingPaneManager()->GetPaintManager();


		dc.SetBkMode(TRANSPARENT);
		pPaintManager->DrawFloatingFrame(dc, this, rc);
	}
	else
	{
		Default();
	}
}

void CXTPDockingPaneMiniWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	if (IsThemed())
	{
		lpncsp->rgrc[0].left   -= 1;
		lpncsp->rgrc[0].top    -= 1;
		lpncsp->rgrc[0].right  += 1;
		lpncsp->rgrc[0].bottom += 1;

		int nTitleFont = GetDockingPaneManager()->GetPaintManager()->GetCaptionHeight();
		lpncsp->rgrc[0].top    += nTitleFont + 2;
	}

	CMiniFrameWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CXTPDockingPaneMiniWnd::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	CMiniFrameWnd::OnNcMouseMove(nHitTest, point);

	if (IsThemed())
	{
		point.Offset(-CXTPWindowRect(this).TopLeft());

		if (m_arrCaptionButtons.CheckForMouseOver(point))
		{
			SetTimer (TID_NCLEAVE, 100, NULL);
		}

		Expand();
	}
}

void CXTPDockingPaneMiniWnd::Collapse()
{

	m_nSlideStep = m_nStepsCount =
		CXTPDockingPaneAutoHideWnd::m_nAnimationDuration / CXTPDockingPaneAutoHideWnd::m_nAnimationInterval;

	CWnd::GetWindowRect(&m_rcWindow);

	m_nExpandedHeight = m_rcWindow.Height();

	m_bCollapsed = TRUE;
	m_bSlideOut = FALSE;

	m_nDeactivationCount = 6;
	SetTimer(TID_CHECKACTIVE, 100, NULL);
}

void CXTPDockingPaneMiniWnd::OnCaptionButtonClick(CXTPDockingPaneCaptionButton* pButton)
{
	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_CLOSE)
	{
		OnClose();
	}
	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_AUTOHIDE)
	{
		OnPinButtonClick();
	}
}

void CXTPDockingPaneMiniWnd::OnPinButtonClick()
{
	if (!m_hWnd)
		return;

	if (!m_bCollapsed)
	{
		Collapse();
	} else
	{
		if (m_nStepsCount != m_nSlideStep)
		{
			CXTPWindowRect rcWindow(this);
			SetWindowPos(0, 0, 0, rcWindow.Width(), m_nExpandedHeight, SWP_NOZORDER|SWP_NOMOVE);
		}
		m_bCollapsed = FALSE;

		KillTimer(TID_CHECKACTIVE);
		KillTimer(TID_SLIDEOUT);

	}
	SendMessage(WM_NCPAINT);
}

void CXTPDockingPaneMiniWnd::DoSlideStep(BOOL bActivate)
{
	int nMinHeight = GetDockingPaneManager()->GetPaintManager()->GetCaptionHeight() + 8;

	CXTPWindowRect rc(this);
	rc.bottom = rc.top + max(nMinHeight, m_nSlideStep * m_nExpandedHeight / m_nStepsCount);



	SetWindowPos(&CWnd::wndTop, rc.left, rc.top, rc.Width(), rc.Height(), (!bActivate? SWP_NOZORDER|SWP_NOACTIVATE: SWP_NOACTIVATE));
	Invalidate(FALSE);
}


BOOL CXTPDockingPaneMiniWnd::IsCaptionButtonVisible(CXTPDockingPaneCaptionButton* pButton)
{
	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_CLOSE)
		return m_bCloseEnabled;

	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_AUTOHIDE)
	{
		pButton->SetState(m_bCollapsed? xtpPanePinPushed: 0);
		return m_bShowPinButton;
	}

	return TRUE;
}


void CXTPDockingPaneMiniWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	if (IsThemed())
	{
		CPoint ptClick(point);
		ptClick.Offset(-CXTPWindowRect(this).TopLeft());

		for (int i = 0; i < m_arrCaptionButtons.GetSize(); i++)
		{
			CXTPDockingPaneCaptionButton* pButton = m_arrCaptionButtons[i];
			if (pButton->PtInRect(ptClick))
			{
				if (pButton->Click(this, ptClick, FALSE))
				{
					OnNcMouseMove(nHitTest, point);
					OnCaptionButtonClick(pButton);
				}
				return;
			}
		}
	}

	if (nHitTest == HTCAPTION)
	{
		// special activation for floating toolbars
		ActivateTopParent();
		BringWindowToTop();

		CXTPDockingPaneContext* pContext = GetDockingPaneManager()->GetDockingContext();
		pContext->Drag(m_pTopContainer, point);

		return;
	}

	CMiniFrameWnd::OnNcLButtonDown(nHitTest, point);

}

void CXTPDockingPaneMiniWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TID_NCLEAVE)
	{
		CPoint pt;

		::GetCursorPos (&pt);

		if ( ::WindowFromPoint (pt) != m_hWnd )
		{
			PostMessage (WM_NCMOUSEMOVE);

			KillTimer(TID_NCLEAVE);
		}
	}

	if (nIDEvent == TID_SLIDEOUT && m_bSlideOut)
	{
		m_nSlideStep--;
		if (m_nSlideStep > -1) DoSlideStep();
		else
		{
			m_bSlideOut = FALSE;
			KillTimer(TID_SLIDEOUT);
		}
	}

	if (nIDEvent == TID_CHECKACTIVE)
	{
		CPoint pt;
		GetCursorPos(&pt);

		CWnd* pFocus = GetFocus();
		BOOL bActive = (pFocus->GetSafeHwnd() && (pFocus == this || IsChild(pFocus)));

		if (!bActive && !CXTPWindowRect(this).PtInRect(pt) && GetCapture() == NULL && !m_bSlideOut)
		{
			if (--m_nDeactivationCount <= 0)
			{
				if (m_nSlideStep == m_nStepsCount)
				{
					m_nExpandedHeight = CXTPWindowRect(this).Height();
				}

				m_bSlideOut = TRUE;
				SetTimer(TID_SLIDEOUT, CXTPDockingPaneAutoHideWnd::m_nAnimationInterval, NULL);

				KillTimer(TID_CHECKACTIVE);
			}
		}
	}

	CMiniFrameWnd::OnTimer(nIDEvent);
}



void CXTPDockingPaneMiniWnd::OnThemedChanged()
{
	if (!m_hWnd)
		return;

	ModifyStyle(IsThemed()? WS_BORDER: 0, IsThemed()? 0: WS_BORDER, SWP_FRAMECHANGED);

	if (!IsThemed() && m_bCollapsed)
	{
		OnPinButtonClick();
	}

	SetWindowPos(0, 0, 0, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);

	if (m_bCollapsed && IsThemed())
	{
		DoSlideStep();
	}
}



void CXTPDockingPaneMiniWnd::Expand()
{
	if (m_bCollapsed && m_nSlideStep <= 0)
	{
		m_nSlideStep = m_nStepsCount;
		DoSlideStep(TRUE);

		m_nDeactivationCount = 8;
		SetTimer(TID_CHECKACTIVE, 100, NULL);
	}
}

void CXTPDockingPaneMiniWnd::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CMiniFrameWnd::OnGetMinMaxInfo(lpMMI);

	if (m_pTopContainer && (!m_bCollapsed && !m_bSlideOut))
	{
		CXTPWindowRect rcWindow(this);
		CXTPClientRect rcClient(this);
		CSize szBorder = rcWindow.Size() - rcClient.Size();

		MINMAXINFO mInfo;
		m_pTopContainer->GetMinMaxInfo(&mInfo);

		lpMMI->ptMinTrackSize.x = max(lpMMI->ptMinTrackSize.x, mInfo.ptMinTrackSize.x + szBorder.cx);
		lpMMI->ptMinTrackSize.y = max(lpMMI->ptMinTrackSize.y, mInfo.ptMinTrackSize.y + szBorder.cy);

		lpMMI->ptMaxTrackSize.x = min(lpMMI->ptMaxTrackSize.x, mInfo.ptMaxTrackSize.x + szBorder.cx);
		lpMMI->ptMaxTrackSize.y = min(lpMMI->ptMaxTrackSize.y, mInfo.ptMaxTrackSize.y + szBorder.cy);
	}
}
