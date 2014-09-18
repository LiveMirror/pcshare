// XTPDockingPaneAutoHidePanel.cpp : implementation of the CXTPDockingPaneAutoHidePanel class.
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
#include "XTPVC80Helpers.h"
#include "XTPDrawHelpers.h"
#include "XTPColorManager.h"
#include "XTPImageManager.h"
#include "XTPResource.h"
#include "XTPTabManager.h"

#include "XTPDockingPane.h"
#include "XTPDockingPaneManager.h"
#include "XTPDockingPaneTabbedContainer.h"
#include "XTPDockingPaneAutoHidePanel.h"
#include "XTPDockingPaneLayout.h"
#include "XTPDockingPaneSplitterContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TID_CHECKACTIVE 1
#define TID_CLOSE 4
#define TID_SLIDEIN 2
#define TID_SLIDEOUT 3

double CXTPDockingPaneAutoHideWnd::m_dAnimationDelay = -1;
int CXTPDockingPaneAutoHideWnd::m_nAnimationInterval = 16;                 // smother animation
int CXTPDockingPaneAutoHideWnd::m_nAnimationDuration = 128;                // fast slide completion
BOOL CXTPDockingPaneAutoHidePanel::m_bCloseActiveWindow = TRUE;
DWORD CXTPDockingPaneAutoHideWnd::m_nMouseHoverDelay = HOVER_DEFAULT;
UINT CXTPDockingPaneAutoHideWnd::m_nInactiveCollapseDelay = 100;

void CXTPDockingPaneAutoHideWnd::GetMinMaxInfo (LPMINMAXINFO pMinMaxInfo, BOOL bIncludeSplitter /*= TRUE*/)
{
	ZeroMemory(pMinMaxInfo, sizeof(MINMAXINFO));
	pMinMaxInfo->ptMaxTrackSize = CPoint(32000, 32000);

	if (!m_pPane || !m_pPane->m_pSelectedPane)
		return;

	m_pPane->m_pSelectedPane->GetMinMaxInfo(pMinMaxInfo);
	m_pPane->AdjustMinMaxInfoClientRect(pMinMaxInfo, TRUE);

	if (!bIncludeSplitter)
		return;
	
	if (m_direction == xtpPaneDockLeft || m_direction == xtpPaneDockRight)
	{
		pMinMaxInfo->ptMinTrackSize.x += 4; pMinMaxInfo->ptMaxTrackSize.x += 4;
	}
	else
	{
		pMinMaxInfo->ptMinTrackSize.y += 4; pMinMaxInfo->ptMaxTrackSize.y += 4;
	}
}

CXTPDockingPaneAutoHideWnd::CXTPDockingPaneAutoHideWnd(CXTPDockingPaneAutoHidePanel* pPanel, CXTPDockingPaneTabbedContainer* pPane)
	: m_pPanel(pPanel), m_pPane(pPane)
{
	ASSERT(pPane);
	ASSERT(pPane->m_pSelectedPane);

	m_bTracking = FALSE;

	m_direction = m_pPanel->GetDirection();
	m_bHoriz = (m_direction == xtpPaneDockTop || m_direction == xtpPaneDockBottom);
	CXTPDockingPaneManager* pManager = GetDockingPaneManager();

	CFrameWnd* pSite = pManager->GetSite();
	CXTPDockingPaneBase* pTopPane = pManager->GetTopPane();

	CRect rcClient = pTopPane->GetWindowRect();
	
	MINMAXINFO mmInfo;
	GetMinMaxInfo(&mmInfo);

	CSize szDocking = CSize(
		max(mmInfo.ptMinTrackSize.x, min(mmInfo.ptMaxTrackSize.x, pPane->m_pSelectedPane->m_szDocking.cx)),
		max(mmInfo.ptMinTrackSize.y, min(mmInfo.ptMaxTrackSize.y, pPane->m_pSelectedPane->m_szDocking.cy)));

	
	switch(pManager->GetRTLDirection(m_direction))
	{
		case xtpPaneDockLeft: rcClient.right = rcClient.left + szDocking.cx; break;
		case xtpPaneDockRight: rcClient.left = rcClient.right - szDocking.cx; break;
		case xtpPaneDockTop: rcClient.bottom = rcClient.top + szDocking.cy; break;
		case xtpPaneDockBottom: rcClient.top = rcClient.bottom - szDocking.cy; break;
	}

	pSite->ScreenToClient(&rcClient);

	CRect rcAvail, rc;
	BOOL bResizable = GetAvailableRect(rcAvail, rc);

	HCURSOR hCursor = !bResizable || pManager->IsSplittersLocked()? ::LoadCursor(0, IDC_ARROW):
		AfxGetApp()->LoadCursor(!m_bHoriz?  XTP_IDC_HSPLITBAR:  XTP_IDC_VSPLITBAR);
	m_rcWindow = rcClient;

	pSite->UpdateWindow();
	pSite->SetRedraw(FALSE);

	CreateEx(pSite->GetExStyle() & WS_EX_LAYOUTRTL, AfxRegisterWndClass(0, hCursor), _T(""),
		WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, rcClient, pSite, 0);

	pSite->SetRedraw(TRUE);


	m_nSlideStep = 0;
	m_nStepsCount = 1;

	if (m_dAnimationDelay != 0 && m_nAnimationInterval != 0)
	{
		if (m_dAnimationDelay > 0 )
		{
			// keep the animation delay behaviour relative to 50 ms frame interval as per original
			// implementation
			m_nStepsCount = int((m_bHoriz? m_rcWindow.Width(): m_rcWindow.Height()) * m_dAnimationDelay * 50 / m_nAnimationInterval);
		}
		else
		{
			m_nStepsCount = m_nAnimationDuration/m_nAnimationInterval;
		}
	}
	if (m_nStepsCount < 1) m_nStepsCount = 1;


	m_bSlideOut = FALSE;
	if (m_nStepsCount > 1)
		SetTimer(TID_SLIDEIN, m_nAnimationInterval, NULL);
	else DoSlideStep();

	SetTimer(TID_CHECKACTIVE, m_nInactiveCollapseDelay, NULL);

	m_nDeactivationCount = 0;

}

CXTPDockingPaneManager* CXTPDockingPaneAutoHideWnd::GetDockingPaneManager()
{
	ASSERT(m_pPanel);
	return m_pPanel->GetDockingPaneManager();
}


void CXTPDockingPaneAutoHideWnd::DoSlideStep()
{
	CRect rc(m_rcWindow);

	switch(m_direction)
	{
		case xtpPaneDockLeft: rc.right = rc.left + (m_nSlideStep + 1) * m_rcWindow.Width() / m_nStepsCount; break;
		case xtpPaneDockRight: rc.left = rc.right - (m_nSlideStep + 1) * m_rcWindow.Width() / m_nStepsCount; break;
		case xtpPaneDockTop: rc.bottom = rc.top + (m_nSlideStep + 1) * m_rcWindow.Height() / m_nStepsCount; break;
		case xtpPaneDockBottom: rc.top = rc.bottom - (m_nSlideStep + 1) * m_rcWindow.Height() / m_nStepsCount; break;
	}
	SetWindowPos(&CWnd::wndTop, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
	Invalidate(FALSE);
}

void CXTPDockingPaneAutoHideWnd::RecalcLayout(BOOL bNotify)
{
	UNREFERENCED_PARAMETER(bNotify);

	if (!m_pPane || !m_pPane->GetSafeHwnd())
		return;

	m_pPane->ShowWindow(SW_SHOW);

	AFX_SIZEPARENTPARAMS layout;
	layout.bStretch = TRUE;
	GetClientRect(&layout.rect);
	CRect rcClient(layout.rect);
	layout.hDWP = ::BeginDeferWindowPos(8); // reasonable guess

	CRect rcPane(CPoint(0, 0), m_rcWindow.Size());

	switch(m_direction)
	{
		case xtpPaneDockLeft:
			rcPane.OffsetRect( - m_rcWindow.Width() + rcClient.Width(), 0);
			rcPane.right -= 4;
			break;
		case xtpPaneDockTop:
			rcPane.OffsetRect(0, - m_rcWindow.Height() + rcClient.Height());
			rcPane.bottom -=4;
			break;
		case xtpPaneDockRight: rcPane.left +=4; break;
		case xtpPaneDockBottom: rcPane.top += 4; break;
	}

	m_pPane->OnSizeParent(this, rcPane, &layout);

	if (layout.hDWP == NULL || !::EndDeferWindowPos(layout.hDWP))
		TRACE0("Warning: DeferWindowPos failed - low system resources.\n");
}

IMPLEMENT_DYNAMIC(CXTPDockingPaneAutoHideWnd, CMiniFrameWnd)

BEGIN_MESSAGE_MAP(CXTPDockingPaneAutoHideWnd, CMiniFrameWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


void CXTPDockingPaneAutoHideWnd::CloseWindow()
{
	if (!IsWindow(m_hWnd))
		return;

	ShowWindow(SW_HIDE);
	if (m_pPane && m_pPane->GetSafeHwnd() && m_pPane->GetParent() == this)
	{
		m_pPane->ShowWindow(SW_HIDE);
		m_pPane->SetParentFrame(GetDockingPaneManager()->GetSite());
	}
	m_pPane = 0;

	if (m_pPanel && m_pPanel->m_pActiveWnd == this)
		m_pPanel->m_pActiveWnd = 0;

	PostMessage(WM_CLOSE);
}
void CXTPDockingPaneAutoHideWnd::PostNcDestroy()
{
	if (m_pPanel && m_pPanel->m_pActiveWnd == this)
		m_pPanel->m_pActiveWnd = 0;

	delete this;
}

void CXTPDockingPaneAutoHideWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TID_CLOSE)
	{
		CloseWindow();
		return;
	}

	if (!m_pPane)
		return;

	if (nIDEvent == TID_CHECKACTIVE)
	{
		if (m_bTracking)
			return;

		CPoint pt;
		GetCursorPos(&pt);
		if (!m_pPane->IsActive() && !CXTPWindowRect(this).PtInRect(pt) && !CXTPWindowRect(m_pPanel).PtInRect(pt))
		{
			if (!m_bSlideOut)
			{
				if (--m_nDeactivationCount <= 0)
				{
					m_nDeactivationCount = 0;

					if (!CXTPDockingPaneAutoHidePanel::m_bCloseActiveWindow || GetDockingPaneManager()->_OnAction(xtpPaneActionCollapsing, m_pPane->GetSelected()))
					{
						m_nDeactivationCount = 6;
					}
					else
					{
						m_bSlideOut = TRUE;
						SetTimer(TID_SLIDEOUT, m_nAnimationInterval, NULL);
					}
				}
			}
		}
		else
		{
			m_nDeactivationCount = 6;
			if (m_bSlideOut)
			{
				m_bSlideOut = FALSE;
				KillTimer(TID_SLIDEOUT);
				SetTimer(TID_SLIDEIN, m_nAnimationInterval, NULL);
			}
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

			GetDockingPaneManager()->_OnAction(xtpPaneActionCollapsed, m_pPane->GetSelected());

			CloseWindow();
		}
	}
	if (nIDEvent == TID_SLIDEIN && !m_bSlideOut)
	{
		m_nSlideStep ++;
		if  (m_nSlideStep < m_nStepsCount) DoSlideStep();
		else
		{
			m_nSlideStep = m_nStepsCount - 1;
			KillTimer(TID_SLIDEIN);

			GetDockingPaneManager()->_OnAction(xtpPaneActionExpanded, m_pPane->GetSelected());
		}
	}

	CWnd::OnTimer(nIDEvent);
}

void CXTPDockingPaneAutoHideWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bSlideOut)
	{
		OnTimer(TID_CHECKACTIVE);
	}

	CMiniFrameWnd::OnMouseMove(nFlags, point);
}


void CXTPDockingPaneAutoHideWnd::OnClose()
{
	CMiniFrameWnd::OnClose();
}

BOOL CXTPDockingPaneAutoHideWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPDockingPaneAutoHideWnd::OnPaint()
{
	CPaintDC dc(this);

	if (!m_pPane)
		return;

	CXTPDockingPanePaintManager* pPaintManager = m_pPane->CXTPDockingPaneBase::GetPaintManager();

	CXTPClientRect rc(this);
	dc.FillSolidRect(rc, pPaintManager->GetXtremeColor(XPCOLOR_3DFACE));


	switch(m_direction)
	{
		case xtpPaneDockLeft:
			dc.Draw3dRect(rc.right - 1, rc.top, 1, rc.Height(), pPaintManager->GetXtremeColor(COLOR_BTNTEXT), pPaintManager->GetXtremeColor(COLOR_BTNTEXT));
			dc.Draw3dRect(rc.right - 2, rc.top, 1, rc.Height(), pPaintManager->GetXtremeColor(COLOR_3DSHADOW), pPaintManager->GetXtremeColor(COLOR_3DSHADOW));
			break;
		case xtpPaneDockTop:
			dc.Draw3dRect(rc.left, rc.bottom - 1, rc.Width(), 1, pPaintManager->GetXtremeColor(COLOR_BTNTEXT), pPaintManager->GetXtremeColor(COLOR_BTNTEXT));
			dc.Draw3dRect(rc.left, rc.bottom - 2, rc.Width(), 1, pPaintManager->GetXtremeColor(COLOR_3DSHADOW), pPaintManager->GetXtremeColor(COLOR_3DSHADOW));
			break;
		case xtpPaneDockRight:
			dc.Draw3dRect(rc.left + 1, rc.top, 1, rc.Height(), pPaintManager->GetXtremeColor(COLOR_3DHIGHLIGHT), pPaintManager->GetXtremeColor(COLOR_3DHIGHLIGHT));
			break;
		case xtpPaneDockBottom:
			dc.Draw3dRect(rc.left, rc.top + 1, rc.Width(), 1, pPaintManager->GetXtremeColor(COLOR_3DHIGHLIGHT), pPaintManager->GetXtremeColor(COLOR_3DHIGHLIGHT));
			break;
	}
}

void CXTPDockingPaneAutoHideWnd::Reposition(CRect rc, CRect rcClient)
{
	CFrameWnd* pSite = GetDockingPaneManager()->GetSite();

	switch(GetDockingPaneManager()->GetRTLDirection(m_direction))
	{
	case xtpPaneDockLeft:  rc.left = rcClient.left; break;
	case xtpPaneDockRight: rc.right = rcClient.right; break;
	case xtpPaneDockTop: rc.top = rcClient.top; break;
	case xtpPaneDockBottom: rc.bottom = rcClient.bottom; break;
	}
	if (!m_bHoriz) m_pPane->m_pSelectedPane->m_szDocking.cx = rc.Width();
	else m_pPane->m_pSelectedPane->m_szDocking.cy = rc.Height();

	pSite->ScreenToClient(&rc);
	m_rcWindow = rc;
	MoveWindow(m_rcWindow);
	Invalidate(FALSE);
}

BOOL CXTPDockingPaneAutoHideWnd::GetAvailableRect(CRect& rcAvail, CRect& rc)
{
	CXTPDockingPaneManager* pManager = GetDockingPaneManager();
	if (!pManager)
		return FALSE;

	CRect rcClient = pManager->GetTopPane()->GetWindowRect();	

	int nGap = pManager->m_nSplitterGap + 4;

	rcAvail = rcClient;

	MINMAXINFO mmInfo;
	GetMinMaxInfo(&mmInfo, m_direction == xtpPaneDockRight || m_direction == xtpPaneDockBottom);

	switch(m_direction)
	{
		case xtpPaneDockLeft:
			rcAvail.DeflateRect(max(mmInfo.ptMinTrackSize.x, nGap), 0, nGap, 0); 
		
			if (rcAvail.right - rcClient.left> mmInfo.ptMaxTrackSize.x)
				rcAvail.right = mmInfo.ptMaxTrackSize.x + rcClient.left;
		
			if (rcAvail.left >= rcAvail.right)
				return FALSE;
			
			rc.left = rc.right - 4;
			break;
	
		case xtpPaneDockRight:
			rcAvail.DeflateRect(nGap, 0, max(mmInfo.ptMinTrackSize.x, nGap), 0); 
		
			if (rcClient.right - rcAvail.left > mmInfo.ptMaxTrackSize.x)
				rcAvail.left = rcClient.right - mmInfo.ptMaxTrackSize.x;
		
			if (rcAvail.left >= rcAvail.right)
				return FALSE;
			
			rc.right = rc.left + 4;
			break;
	
		case xtpPaneDockTop:
			rcAvail.DeflateRect(0, max(mmInfo.ptMinTrackSize.y, nGap), 0, nGap);

			if (rcAvail.bottom - rcClient.top> mmInfo.ptMaxTrackSize.y)
				rcAvail.bottom = mmInfo.ptMaxTrackSize.y + rcClient.top;

			if (rcAvail.top >= rcAvail.bottom)
				return FALSE;
			
			rc.top = rc.bottom - 4;
			break;

		case xtpPaneDockBottom:
			rcAvail.DeflateRect(0, nGap, 0, max(mmInfo.ptMinTrackSize.y, nGap));

			if (rcClient.bottom - rcAvail.top> mmInfo.ptMaxTrackSize.y)
				rcAvail.top = rcClient.bottom - mmInfo.ptMaxTrackSize.y;

			if (rcAvail.top >= rcAvail.bottom)
				return FALSE;
			
			rc.bottom = rc.top + 4;
			break;
	}
	return TRUE;
}

void CXTPDockingPaneAutoHideWnd::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	CXTPDockingPaneManager* pManager = GetDockingPaneManager();

	if (!pManager || pManager->IsSplittersLocked())
		return;

	CXTPWindowRect rc(this);
	CRect rcClient = pManager->GetTopPane()->GetWindowRect();	

	CRect rcAvail;
	if (!GetAvailableRect(rcAvail, rc))
		return;

	m_bTracking = TRUE;

	if (pManager->IsSplitterTrackerUsed())
	{
		if (GetExStyle() & WS_EX_LAYOUTRTL)
			point = rc.TopLeft();
		else ClientToScreen(&point);


		CXTPSplitterTracker tracker;

		if (tracker.Track(this, rcAvail, rc, point, !m_bHoriz))
		{
			Reposition(rc, rcClient);
		}
	} else
	{
		SetCapture();

		while (CWnd::GetCapture() == this)
		{
			MSG msg;

			while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
			{
				if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
					return;
				DispatchMessage(&msg);
			}

			if (!GetMessage(&msg, NULL, 0, 0))
				break;

			if (msg.message == WM_MOUSEMOVE)
			{
				point = CPoint(msg.lParam);
				ClientToScreen(&point);

				point.x = max(min(point.x, rcAvail.right), rcAvail.left);
				point.y = max(min(point.y, rcAvail.bottom), rcAvail.top);

				if (!m_bHoriz)
				{
					if (rc.left == point.x)
						continue;
					rc.OffsetRect(point.x - rc.left, 0);
				}
				else
				{
					if (rc.top == point.y)
						continue;
					rc.OffsetRect(0, point.y - rc.top);
				}

				Reposition(rc, rcClient);
			}
			else if (msg.message == WM_KEYDOWN &&  msg.wParam == VK_ESCAPE) break;
			else if (msg.message ==WM_LBUTTONUP) break;
			else ::DispatchMessage(&msg);
		}

		if (CWnd::GetCapture() == this) ReleaseCapture();
	}

	m_bTracking =  FALSE;
}


//////////////////////////////////////////////////////////////////////////
// CPanelDropTarget

class CXTPDockingPaneAutoHidePanel::CPanelDropTarget : public COleDropTarget
{
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* /*pDataObject*/, DWORD /*dwKeyState*/, CPoint point)
	{
		CXTPDockingPaneAutoHidePanel* pPanel = (CXTPDockingPaneAutoHidePanel*)pWnd;
		ASSERT_VALID(pPanel);

		CXTPDockingPane* pPane = pPanel->HitTest(point);
		if (pPane)
		{
			if (!(pPanel->m_pActiveWnd && pPanel->m_pActiveWnd->GetPane()->GetSelected() == pPane))
			{
				pPanel->CloseActiveWindow();
				pPanel->ShowPane(pPane, FALSE);
			}
		}

		return DROPEFFECT_NONE;
	}
};

//////////////////////////////////////////////////////////////////////////
// CAutoHidePanelTabManager

class CXTPDockingPaneAutoHidePanel::CAutoHidePanelTabManager : public CXTPTabManager
{
public:
	CAutoHidePanelTabManager()
	{
		FindNavigateButton(xtpTabNavigateButtonLeft)->SetFlags(xtpTabNavigateButtonNone);
		FindNavigateButton(xtpTabNavigateButtonRight)->SetFlags(xtpTabNavigateButtonNone);
	}

	virtual void Reposition()
	{

	}
	virtual BOOL DrawIcon(CDC* pDC, CPoint pt, CXTPTabManagerItem* pItem, BOOL bDraw, CSize& szIcon)
	{
		if (!pItem)
			return TRUE;

		if (GetPanel()->GetPaintManager()->GetPanelPaintManager()->m_bShowIcons == FALSE)
			return FALSE;


		CXTPImageManagerIcon* pImage = ((CXTPDockingPane*)pItem->GetData())->GetIcon(szIcon.cx);

		if (!pImage)
			return FALSE;

		if (!bDraw)
		{
			return TRUE;
		}
		pImage->Draw(pDC, pt, !pItem->IsEnabled()? pImage->GetDisabledIcon(): pItem->IsHighlighted()? pImage->GetHotIcon():
		pItem->IsSelected()? pImage->GetCheckedIcon(): pImage->GetIcon(), szIcon);

		return TRUE;

	}

	virtual void RedrawControl()
	{
		if (GetPanel()->m_hWnd) GetPanel()->Invalidate(FALSE);
	}
	virtual CXTPTabPaintManager* GetPaintManager()
	{
		return GetPanel()->GetPaintManager()->GetPanelPaintManager();

	}
	CXTPTabManagerItem* Add()
	{
		return AddItem(GetItemCount());
	}
	XTPTabPosition GetPosition()
	{
		switch (GetPanel()->GetDirection())
		{
			case xtpPaneDockLeft: return xtpTabPositionRight;
			case xtpPaneDockRight: return xtpTabPositionLeft;
			case xtpPaneDockTop: return xtpTabPositionBottom;
			case xtpPaneDockBottom: return xtpTabPositionTop;
		}
		return xtpTabPositionTop;
	}
	void PerformMouseMove(HWND hwnd, CPoint pt)
	{
		CXTPTabManager::PerformMouseMove(hwnd, pt);
	}


	CXTPDockingPaneAutoHidePanel* GetPanel();

public:
	CAutoHidePanelTabManagersArray* m_pArray;
	CRect m_rcManager;

};

class CXTPDockingPaneAutoHidePanel::CAutoHidePanelTabManagersArray : public CArray<CAutoHidePanelTabManager*, CAutoHidePanelTabManager*>
{
public:

	~CAutoHidePanelTabManagersArray()
	{
		FreeAll();
	}

	void FreeAll()
	{
		for (int i = 0; i < GetSize(); i++)
		{
			delete GetAt(i);
		}
		RemoveAll();
	}
	CAutoHidePanelTabManager* AddManager()
	{
		CAutoHidePanelTabManager* pManager = new CAutoHidePanelTabManager();
		Add(pManager);
		pManager->m_pArray = this;
		return pManager;
	}

	void Draw(CDC* pDC)
	{
		CXTPTabPaintManager* pPaintManager = m_pAutoHidePanel->GetPaintManager()->GetPanelPaintManager();
		for (int i = 0; i < GetSize(); i++)
		{
			CAutoHidePanelTabManager* pManager = GetAt(i);
			pPaintManager->DrawTabControl(pManager, pDC, pManager->m_rcManager);
		}
	}



public:
	CXTPDockingPaneAutoHidePanel* m_pAutoHidePanel;
};



CXTPDockingPaneAutoHidePanel* CXTPDockingPaneAutoHidePanel::CAutoHidePanelTabManager::GetPanel()
{
	return m_pArray->m_pAutoHidePanel;
}






//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneAutoHidePanel

int CXTPDockingPaneAutoHidePanel::GetPanelHeight()
{
	CXTPTabPaintManager::CAppearanceSet* pAppearanceSet = GetPaintManager()->GetPanelPaintManager()->GetAppearanceSet();

	return pAppearanceSet->GetButtonHeight(NULL) +
		pAppearanceSet->m_rcHeaderMargin.bottom + pAppearanceSet->m_rcHeaderMargin.top;
}

void CXTPDockingPaneAutoHidePanel::RefreshManagersArray(CDC* pDC, CRect rcClient)
{
	BOOL bIndent = (m_direction == xtpPaneDockLeft || m_direction == xtpPaneDockRight)
		&& (m_pLayout->m_wndPanels[xtpPaneDockTop] != NULL && !m_pLayout->m_wndPanels[xtpPaneDockTop]->IsEmpty());

	if (bIndent)
	{
		rcClient.top += GetPanelHeight();
	}

	m_pTabManagers->FreeAll();

	CRect rcMargins = GetPaintManager()->GetPanelPaintManager()->GetAppearanceSet()->GetHeaderMargin();

	POSITION pos = m_lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneTabbedContainer* pContainer = (CXTPDockingPaneTabbedContainer*)m_lstPanes.GetNext(pos);
		if (pContainer->IsEmpty())
			continue;

		CAutoHidePanelTabManager* pManager = m_pTabManagers->AddManager();

		POSITION posPane = pContainer->GetPanes().GetHeadPosition();
		while(posPane)
		{
			CXTPDockingPane* pPane = pContainer->GetPanes().GetNext(posPane);

			CXTPTabManagerItem* pItem = pManager->Add();

			pItem->SetData((DWORD_PTR)pPane);
			if (pContainer->GetSelected() == pPane) pManager->SetSelectedItem(pItem);

			pItem->SetCaption(pPane->GetShortTitle());
			pItem->SetColor(pPane->GetItemColor());

		}
		pManager->GetPaintManager()->RepositionTabControl(pManager, pDC, rcClient);
		pManager->m_rcManager = rcClient;

		if (m_direction == xtpPaneDockLeft || m_direction == xtpPaneDockRight)
		{
			rcClient.top += pManager->GetItemsLength() + rcMargins.left + rcMargins.right + 8;
		} else
		{
			rcClient.left += pManager->GetItemsLength() + rcMargins.left + rcMargins.right + 8;

		}
	}
}

CXTPDockingPaneAutoHidePanel::CXTPDockingPaneAutoHidePanel(CXTPDockingPaneLayout* pLayout)
	: CXTPDockingPaneBase(xtpPaneTypeAutoHidePanel, pLayout)
{
	m_direction = xtpPaneDockLeft;
	m_pActiveWnd = 0;
	m_pTabManagers = new CAutoHidePanelTabManagersArray();
	m_pTabManagers->m_pAutoHidePanel =  this;

	m_pDropTarget = new CPanelDropTarget();
}

CXTPDockingPaneAutoHidePanel::~CXTPDockingPaneAutoHidePanel()
{
	if (m_pActiveWnd)
	{
		m_pActiveWnd->m_pPane = 0;
		m_pActiveWnd->m_pPanel = 0;
		CloseActiveWindow(FALSE);
	}

	if (m_hWnd)
		DestroyWindow();

	delete m_pTabManagers;
	delete m_pDropTarget;
}

void CXTPDockingPaneAutoHidePanel::_InsertPane(CXTPDockingPaneBase* pPane)
{
	m_lstPanes.AddTail(pPane);

	pPane->SetParentContainer(this);
	pPane->SetParentFrame(m_pParentFrame);

	DELAYRECALCLAYOUT(m_pParentFrame);

	pPane->OnParentConainerChanged(this);

	if (pPane->GetType() == xtpPaneTypeTabbedContainer)
	{
		CWnd* pFocus = GetFocus();
		if (((CXTPDockingPaneTabbedContainer*)pPane)->GetSafeHwnd() && pFocus->GetSafeHwnd() && (pFocus == ((CXTPDockingPaneTabbedContainer*)pPane) || ((CXTPDockingPaneTabbedContainer*)pPane)->IsChild(pFocus)))
			GetDockingPaneManager()->GetSite()->SetFocus();
	}

	if (m_hWnd) Invalidate(FALSE);
}

void CXTPDockingPaneAutoHidePanel::RemovePane(CXTPDockingPaneBase* pPane)
{
	POSITION pos = m_lstPanes.Find(pPane);
	ASSERT(pos);

	m_lstPanes.RemoveAt(pos);
	if (m_hWnd) Invalidate(FALSE);
	DELAYRECALCLAYOUT(m_pParentFrame);

	if (m_lstPanes.IsEmpty())
	{
		if (m_hWnd) DestroyWindow();
	}

	pPane->m_pParentContainer = NULL;
}

AFX_STATIC CString XTPRegisterWndClass(UINT nClassStyle,
	HCURSOR hCursor, HBRUSH hbrBackground = 0, HICON hIcon = 0)
{
	CString strNameThread = AfxGetThreadState()->m_szTempClassName;

	CString strName = AfxRegisterWndClass(nClassStyle, hCursor, hbrBackground, hIcon);

	STRCPY_S(AfxGetThreadState()->m_szTempClassName, 96, strNameThread);

	return strName;
}

void CXTPDockingPaneAutoHidePanel::Create(CWnd* pParent)
{
	CWnd::Create(XTPRegisterWndClass(0, LoadCursor(0, IDC_ARROW)), NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CXTPEmptyRect(), pParent, 0);
	m_pDropTarget->Register(this);
}
void CXTPDockingPaneAutoHidePanel::OnSizeParentEx(CWnd* pParent, CRect& rect, LPVOID lParam)
{
	AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;

	if (IsEmpty())
	{
		if (m_hWnd) DestroyWindow();
		return;
	}

	if (!m_hWnd)
		Create(pParent);

	CloseActiveWindow();

	int nHeight = GetPanelHeight();

	CRect rcClient(rect);
	switch(m_direction)
	{
		case xtpPaneDockLeft: rcClient.right = rect.left =  rect.left + nHeight; break;
		case xtpPaneDockRight: rcClient.left = rect.right =  rect.right - nHeight; break;
		case xtpPaneDockTop: rcClient.bottom = rect.top =  rect.top + nHeight; break;
		case xtpPaneDockBottom: rcClient.top = rect.bottom =  rect.bottom - nHeight; break;
	}

	CClientDC dc(this);
	CRect rcPanel(rcClient);
	rcPanel.OffsetRect(-rcPanel.TopLeft());
	RefreshManagersArray(&dc, rcPanel);

	if (lpLayout->hDWP != NULL)
	{
		MoveWindow(rcClient);
		Invalidate(FALSE);
	}
}


BEGIN_MESSAGE_MAP(CXTPDockingPaneAutoHidePanel, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

void CXTPDockingPaneAutoHidePanel::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint, rc);

	CXTPPaintManagerColorGradient& clr = GetPaintManager()->GetPanelPaintManager()->GetColorSet()->m_clrAutoHideFace;

	if (m_direction == xtpPaneDockRight || clr.clrDark == clr.clrLight)
		dc.FillSolidRect(rc, clr.clrDark);
	else
	{
		CRect rcFill(rc);
		rcFill.right = rcFill.left + max(rcFill.Width(), GetSystemMetrics(SM_CXFULLSCREEN) /2);
		XTPDrawHelpers()->GradientFill(&dc, rcFill, clr, TRUE);
	}

	m_pTabManagers->Draw(&dc);
}

CXTPDockingPane* CXTPDockingPaneAutoHidePanel::HitTest(CPoint point)
{
	for (int i = 0; i < m_pTabManagers->GetSize(); i++)
	{
		CXTPTabManagerItem* pItem = m_pTabManagers->GetAt(i)->HitTest(point);
		if (pItem)
		{
			return (CXTPDockingPane*)pItem->GetData();
		}

	}
	return NULL;
}

void CXTPDockingPaneAutoHidePanel::CloseActiveWindow(BOOL bDelayed)
{
	if (!m_bCloseActiveWindow)
		return;

	if (m_pActiveWnd)
	{
		if (m_pActiveWnd->m_pPane)
		{
			if (GetDockingPaneManager()->_OnAction(xtpPaneActionCollapsing, m_pActiveWnd->m_pPane->GetSelected()))
				return;

			GetDockingPaneManager()->_OnAction(xtpPaneActionCollapsed,
				m_pActiveWnd->m_pPane->GetSelected());
		}

		if (bDelayed)
		{
			// let the window close itself
			m_pActiveWnd->m_pPane = 0;
			m_pActiveWnd->SetTimer(TID_CLOSE, 50, NULL);
		} else
		{
			m_pActiveWnd->CloseWindow();
		}

		m_pActiveWnd = 0;
	}
}

void CXTPDockingPaneAutoHidePanel::ShowPane(CXTPDockingPane* pPane, BOOL bSetFocus)
{
	if (GetDockingPaneManager()->_OnAction(xtpPaneActionExpanding, pPane))
		return;

	CXTPDockingPaneTabbedContainer* pContainer = ((CXTPDockingPaneTabbedContainer*)pPane->m_pParentContainer);

	ASSERT(pContainer);
	ASSERT(pContainer->GetType() == xtpPaneTypeTabbedContainer);

	pContainer->SelectPane(pPane, bSetFocus);

	Invalidate(FALSE);

	CXTPDockingPaneAutoHideWnd* pActiveWnd = new CXTPDockingPaneAutoHideWnd(this, pContainer);
	if (bSetFocus) pContainer->m_pSelectedPane->SetFocus();

	m_pParentFrame->RecalcLayout();
	m_pActiveWnd = pActiveWnd;
}

void CXTPDockingPaneAutoHidePanel::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	CXTPDockingPane* pPane = HitTest(point);
	if (pPane)
	{
		if (m_pActiveWnd && m_pActiveWnd->m_pPane->GetSelected() == pPane)
		{
			pPane->SetFocus();

		} else
		{
			CloseActiveWindow();
			ShowPane(pPane);
		}
	}
}

void CXTPDockingPaneAutoHidePanel::OnRButtonDown(UINT /*nFlags*/, CPoint point)
{
	CXTPDockingPane* pPane = HitTest(point);
	if (pPane)
	{
		GetDockingPaneManager()->GetSite()->SendMessage(XTPWM_DOCKINGPANE_NOTIFY, XTP_DPN_RCLICK, (LPARAM)(CXTPDockingPaneBase*)pPane);
	}
}

void CXTPDockingPaneAutoHidePanel::OnMouseMove(UINT nFlags, CPoint point)
{
	for (int i = 0; i <  m_pTabManagers->GetSize(); i++)
	{
		m_pTabManagers->GetAt(i)->PerformMouseMove(m_hWnd, point);
	}



	CXTPDockingPane* pPane = HitTest(point);
	if (pPane)
	{
		TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_HOVER|TME_LEAVE, m_hWnd, CXTPDockingPaneAutoHideWnd::m_nMouseHoverDelay};
		_TrackMouseEvent(&tme);

	}
	CWnd::OnMouseMove(nFlags, point);
}

void CXTPDockingPaneAutoHidePanel::OnMouseLeave()
{
	OnMouseMove(0,CPoint(-1, -1));
}

LRESULT CXTPDockingPaneAutoHidePanel::OnMouseHover(WPARAM, LPARAM lParam)
{
	CPoint point(lParam);
	CXTPDockingPane* pPane = HitTest(point);
	if (pPane)
	{
		if (!IsTopParentActive() || (m_pActiveWnd && m_pActiveWnd->m_pPane->GetSelected() == pPane))
			return 1;

		CloseActiveWindow();
		ShowPane(pPane, FALSE);
	}

	return 1;
}


void CXTPDockingPaneAutoHidePanel::Copy(CXTPDockingPaneAutoHidePanel* pClone, CXTPPaneToPaneMap* pMap)
{
	ASSERT(pClone);

	m_direction = pClone->m_direction;
	m_pParentFrame = GetDockingPaneManager()->GetSite();

	POSITION pos = pClone->m_lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane = (CXTPDockingPaneBase*)pClone->m_lstPanes.GetNext(pos);
		if (!pPane->IsEmpty() || pMap)
		{
			_InsertPane(pPane->Clone(m_pLayout, pMap));
		}
	}
}

CXTPDockingPaneBase* CXTPDockingPaneAutoHidePanel::Clone(CXTPDockingPaneLayout* pLayout, CXTPPaneToPaneMap* pMap, DWORD /*dwIgnoredOptions*/)
{
	CXTPDockingPaneAutoHidePanel* pPane = (CXTPDockingPaneAutoHidePanel*)GetDockingPaneManager()->
		OnCreatePane(GetType(), pLayout);

	pPane->Copy(this, pMap);

	SAFE_CALLPTR(pMap, SetAt(this, pPane));

	return pPane;

}

void CXTPDockingPaneAutoHidePanel::OnChildConainerChanged(CXTPDockingPaneBase* /*pContainer*/)
{
	if (m_pActiveWnd )
	{
		if (!m_bCloseActiveWindow)
		{
			CFrameWnd* pFrameWnd = GetParentFrame();
			if (pFrameWnd)
				pFrameWnd->RecalcLayout();
			
			m_pActiveWnd->RecalcLayout(TRUE);
		}
		else
		{		
			CloseActiveWindow(GetDockingPaneManager()->m_bAttachingPane);
		}
	}
}

BOOL CXTPDockingPaneAutoHidePanel::IsEmpty()
{
	POSITION pos = m_lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane =  (CXTPDockingPaneBase*)m_lstPanes.GetNext(pos);
		if (!pPane->IsEmpty())
			return FALSE;
	}
	return TRUE;
}
