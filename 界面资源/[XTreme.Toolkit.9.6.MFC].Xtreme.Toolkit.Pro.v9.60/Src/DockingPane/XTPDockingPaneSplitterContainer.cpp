// XTPDockingPaneSplitterContainer.cpp : implementation of the CXTPDockingPaneSplitterContainer class.
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
#include "XTPResource.h"
#include "XTPDrawHelpers.h"
#include "XTPColorManager.h"

#include "XTPDockingPaneBase.h"
#include "XTPDockingPaneManager.h"
#include "XTPDockingPaneSplitterContainer.h"
#include "XTPDockingPanePaintManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CXTPDockingPaneSplitterWnd::CXTPDockingPaneSplitterWnd()
{
	m_hCursor = 0;
}
void CXTPDockingPaneSplitterWnd::Create(CXTPDockingPaneSplitterContainer* pContainer, CXTPDockingPaneBase* pFirst, CXTPDockingPaneBase* pSecond)
{
	m_pFirst = pFirst;
	m_pSecond = pSecond;
	m_pContainer = pContainer;

	//if (GetDockingPaneManager()->IsSplittersLocked())
	
	m_hCursor = AfxGetApp()->LoadCursor(pContainer->m_bHoriz?  XTP_IDC_HSPLITBAR:  XTP_IDC_VSPLITBAR);

	CWnd::Create(AfxRegisterWndClass(0, ::LoadCursor(0, IDC_ARROW)), _T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), pContainer->m_pParentFrame, 0);
}

CXTPDockingPaneSplitterWnd::~CXTPDockingPaneSplitterWnd()
{
	if (m_hWnd) DestroyWindow();
}

BEGIN_MESSAGE_MAP(CXTPDockingPaneSplitterWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

BOOL CXTPDockingPaneSplitterWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (GetDockingPaneManager()->IsSplittersLocked())
		return CWnd::OnSetCursor(pWnd, nHitTest, message);

	CRect rcAvail, rcUnion;
	if (!GetAvailableRect(rcAvail, rcUnion))
		return CWnd::OnSetCursor(pWnd, nHitTest, message);

	::SetCursor(m_hCursor);
	return TRUE;
}

void CXTPDockingPaneSplitterWnd::OnPaint()
{
	CPaintDC dc(this);

	if (m_pContainer)
		m_pContainer->GetPaintManager()->DrawSplitter(dc, this);
}
BOOL CXTPDockingPaneSplitterWnd::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

BOOL CXTPDockingPaneSplitterWnd::IsHorizontal()
{
	ASSERT(m_pContainer);
	return !m_pContainer->m_bHoriz;
}

void CXTPDockingPaneSplitterWnd::Reposition(CRect rc, CRect rcAvail)
{
	int nSplitterSize = m_pContainer->GetDockingPaneManager()->GetPaintManager()->m_nSplitterSize;

	if (m_pContainer->m_bHoriz)
	{
		double d = double(rc.left - rcAvail.left) / rcAvail.Width();
		int nSum = m_pFirst->m_szDocking.cx + m_pSecond->m_szDocking.cx + nSplitterSize;
		m_pFirst->m_szDocking.cx = GetExStyle() & WS_EX_LAYOUTRTL? nSum - int(d * nSum) - nSplitterSize: int(d * nSum);
		m_pSecond->m_szDocking.cx = nSum - m_pFirst->m_szDocking.cx - nSplitterSize;
	} else
	{
		double d = double(rc.top - rcAvail.top) / rcAvail.Height();
		int nSum = m_pFirst->m_szDocking.cy + m_pSecond->m_szDocking.cy + nSplitterSize;
		m_pFirst->m_szDocking.cy = int(d * nSum);
		m_pSecond->m_szDocking.cy = nSum - m_pFirst->m_szDocking.cy - nSplitterSize;
	}

}

void CXTPDockingPaneSplitterContainer::NormalizeDockingSize()
{
	if (m_bHoriz) m_szDocking.cx = m_rcWindow.Width();
		else m_szDocking.cy = m_rcWindow.Height();

	POSITION pos = m_lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane = (CXTPDockingPaneBase*)m_lstPanes.GetNext(pos);
		if (!pPane->IsEmpty())
		{
			if (m_bHoriz) pPane->m_szDocking.cx = pPane->m_rcWindow.Width();
				else pPane->m_szDocking.cy = pPane->m_rcWindow.Height();
		}
	}
}

BOOL CXTPDockingPaneSplitterWnd::GetAvailableRect(CRect& rcAvail, CRect& rcUnion)
{
	CXTPDockingPaneManager* pManager = GetDockingPaneManager();
	if (!pManager)
		return FALSE;

	int nSplitterSize = pManager->GetPaintManager()->m_nSplitterSize;

	BOOL bHoriz = m_pContainer->m_bHoriz;
	ASSERT(m_pFirst && m_pSecond);

	CRect rcFirst = m_pFirst->GetWindowRect();
	CRect rcSecond = m_pSecond->GetWindowRect();

	MINMAXINFO mmiFirst, mmiSecond;
	m_pFirst->GetMinMaxInfo(&mmiFirst);
	m_pSecond->GetMinMaxInfo(&mmiSecond);

	rcAvail.UnionRect(rcFirst, rcSecond);
	rcUnion = rcAvail;

	int nGap = pManager->m_nSplitterGap;
	if (bHoriz)
	{
		rcAvail.DeflateRect(max(mmiFirst.ptMinTrackSize.x, nGap), 0,
			max(mmiSecond.ptMinTrackSize.x + nSplitterSize, nGap), 0);

		if (rcUnion.Width() > mmiFirst.ptMaxTrackSize.x)
			rcAvail.right = min(rcAvail.right, rcUnion.left + mmiFirst.ptMaxTrackSize.x);

		if (rcUnion.Width() > mmiSecond.ptMaxTrackSize.x)
			rcAvail.left = max(rcAvail.left, rcUnion.right - mmiSecond.ptMaxTrackSize.x - nSplitterSize);

		if (rcAvail.left >= rcAvail.right)
			return FALSE;
	}
	else
	{
		rcAvail.DeflateRect(0, max(mmiFirst.ptMinTrackSize.y, nGap),
			0, max(mmiSecond.ptMinTrackSize.y + nSplitterSize, nGap));

		if (rcUnion.Height() > mmiFirst.ptMaxTrackSize.y)
			rcAvail.bottom = min(rcAvail.bottom, rcUnion.top + mmiFirst.ptMaxTrackSize.y);

		if (rcUnion.Height() > mmiSecond.ptMaxTrackSize.y)
			rcAvail.top = max(rcAvail.top, rcUnion.bottom - mmiSecond.ptMaxTrackSize.y - nSplitterSize);

		if (rcAvail.top >= rcAvail.bottom)
			return FALSE;
	}
	return TRUE;
}

void CXTPDockingPaneSplitterWnd::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	CXTPDockingPaneManager* pManager = GetDockingPaneManager();

	if (pManager->IsSplittersLocked())
		return;

	CXTPWindowRect rc(this);

	m_pContainer->NormalizeDockingSize();

	CRect rcAvail, rcUnion;
	if (!GetAvailableRect(rcAvail, rcUnion))
		return;

	BOOL bHoriz = m_pContainer->m_bHoriz;


	//point = rc.TopLeft();
	ClientToScreen(&point);

	if (pManager->IsSplitterTrackerUsed())
	{

		CXTPSplitterTracker tracker;
		BOOL bAccept = tracker.Track(this, rcAvail, rc, point, bHoriz);

		if (bAccept)
		{
			Reposition(rc, rcUnion);


			DELAYRECALCLAYOUT((CFrameWnd*)GetParent());
		}
	}
	else
	{
		CPoint ptOffset = bHoriz? CPoint(rc.left - point.x, 0):
			CPoint(0, rc.top - point.y);

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
				point += ptOffset;

				point.x = max(min(point.x, rcAvail.right), rcAvail.left);
				point.y = max(min(point.y, rcAvail.bottom), rcAvail.top);

				if (bHoriz)
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

				Reposition(rc, rcUnion);

				((CFrameWnd*)GetParent())->RecalcLayout();
			}
			else if (msg.message == WM_KEYDOWN &&  msg.wParam == VK_ESCAPE) break;
			else if (msg.message ==WM_LBUTTONUP) break;
			else ::DispatchMessage(&msg);
		}

		if (CWnd::GetCapture() == this) ReleaseCapture();
	}

}

CXTPDockingPaneManager* CXTPDockingPaneSplitterWnd::GetDockingPaneManager()
{
	return m_pContainer->GetDockingPaneManager();
}


CXTPDockingPaneSplitterContainer::CXTPDockingPaneSplitterContainer(CXTPDockingPaneLayout* pLayout)
	: CXTPDockingPaneBase(xtpPaneTypeSplitterContainer, pLayout)
{
	m_bRecalcLayout = FALSE;
}

void CXTPDockingPaneSplitterContainer::Init(CXTPDockingPaneBase* pPane, BOOL bHoriz, CFrameWnd* pFrame)
{
	ASSERT(pPane);

	m_bHoriz = bHoriz;
	m_pParentFrame = pFrame;
	_InsertPane(pPane);

	m_szDocking = pPane->m_szDocking;
}

void CXTPDockingPaneSplitterContainer::_UpdateSplitters()
{
	if (m_bRecalcLayout)
		return;

	while (!m_lstSpliters.IsEmpty())
		delete m_lstSpliters.RemoveTail();

	if (m_lstPanes.GetCount() > 1)
	{
		POSITION posPane = m_lstPanes.GetHeadPosition();
		CXTPDockingPaneBase* pFirstPane = NULL;

		// Finding first nonempty pane
		while (posPane)
		{
			pFirstPane = (CXTPDockingPaneBase*)m_lstPanes.GetNext(posPane);
			if (!pFirstPane->IsEmpty())
				break;
		}

		// Splitting
		while (posPane)
		{
			CXTPDockingPaneBase* pSecondPane = (CXTPDockingPaneBase*)m_lstPanes.GetNext(posPane);
			if (pSecondPane->IsEmpty())
				continue;

			CXTPDockingPaneSplitterWnd* pSplitter = OnCreateSplitter();
			pSplitter->Create(this, pFirstPane, pSecondPane);

			m_lstSpliters.AddTail(pSplitter);

			pFirstPane = pSecondPane;
		}
	}
}

CXTPDockingPaneSplitterWnd* CXTPDockingPaneSplitterContainer::OnCreateSplitter()
{
	return new CXTPDockingPaneSplitterWnd();
}

void CXTPDockingPaneSplitterContainer::_Replace(CXTPDockingPaneBase* pPane, CXTPDockingPaneBase* pReplace)
{
	ASSERT(pPane && pReplace);

	POSITION pos = m_lstPanes.Find(pPane);
	ASSERT(pos);

	m_lstPanes.InsertAfter(pos, pReplace);
	m_lstPanes.RemoveAt(pos);

	pReplace->m_pParentContainer = this;

	_UpdateSplitters();

	if (m_pParentContainer)
	{
		m_pParentContainer->OnChildConainerChanged(this);
	}
}

void CXTPDockingPaneSplitterContainer::_InsertPane(CXTPDockingPaneBase* pPane, CXTPDockingPaneBase* pNeighbour, BOOL bAfter)
{
	POSITION pos = pNeighbour? m_lstPanes.Find(pNeighbour): NULL;

	if (bAfter)
	{
		if (pos == NULL) pos = m_lstPanes.GetTailPosition();
		m_lstPanes.InsertAfter(pos, pPane);
	}
	else
	{
		if (pos == NULL) pos = m_lstPanes.GetHeadPosition();
		m_lstPanes.InsertBefore(pos, pPane);
	}
	pPane->m_pParentContainer = this;
	pPane->SetParentFrame(m_pParentFrame);

	DELAYRECALCLAYOUT(m_pParentFrame);

	_UpdateSplitters();

	if (m_pParentContainer)
	{
		m_pParentContainer->OnChildConainerChanged(this);
	}

	pPane->OnParentConainerChanged(this);

}

void CXTPDockingPaneSplitterContainer::RemovePane(CXTPDockingPaneBase* pPane)
{
	POSITION pos = m_lstPanes.Find(pPane);
	ASSERT(pos);

	m_lstPanes.RemoveAt(pos);
	_UpdateSplitters();

	DELAYRECALCLAYOUT(m_pParentFrame);

	if (m_pParentContainer)
	{
		m_pParentContainer->OnChildConainerChanged(this);
	}

	if (m_lstPanes.IsEmpty())
	{
		m_pParentContainer->RemovePane(this);
	}

	pPane->m_pParentContainer = NULL;
}

void CXTPDockingPaneSplitterContainer::OnChildConainerChanged(CXTPDockingPaneBase* pContainer)
{
	if (m_pParentContainer)
	{
		m_pParentContainer->OnChildConainerChanged(pContainer);
	}

	_UpdateSplitters();
	DELAYRECALCLAYOUT(m_pParentFrame);
}

void CXTPDockingPaneSplitterContainer::OnParentConainerChanged(CXTPDockingPaneBase* pContainer)
{
	m_pParentFrame = pContainer->m_pParentFrame;
	_UpdateSplitters();

	POSITION pos = m_lstPanes.GetHeadPosition();
	while (pos)
	{
		m_lstPanes.GetNext(pos)->OnParentConainerChanged(pContainer);
	}
}


CXTPDockingPaneSplitterContainer::~CXTPDockingPaneSplitterContainer(void)
{
	while (!m_lstSpliters.IsEmpty())
		delete m_lstSpliters.RemoveTail();
}

void CXTPDockingPaneSplitterContainer::OnSizeParent(CFrameWnd* pParent, CRect rect, LPVOID lParam)
{
	int nSplitterSize = GetDockingPaneManager()->GetPaintManager()->m_nSplitterSize;
	CXTPDockingPaneBase* pClient = GetDockingPaneManager()->GetClientPane();


	AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;

	m_pParentFrame = pParent;
	m_rcWindow = rect;

	BOOL bClientPane = !GetDockingPaneManager()->m_bHideClient;

	// getting list of nonempty panes
	CXTPDockingPaneBaseList lst;
	POSITION pos = m_lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane = (CXTPDockingPaneBase*)m_lstPanes.GetNext(pos);
		if (!pPane->IsEmpty())
			lst.AddTail(pPane);

		if (!bClientPane && pPane->ContainPane(pClient))
			bClientPane = TRUE;

	}

	if (!lst.IsEmpty() && bClientPane & GetDockingPaneManager()->m_bHideClient)
	{
		pClient->OnSizeParent(pParent, CXTPEmptyRect(), lParam);
	}


	if (lst.GetCount() == 0)
		return;

	if (lst.GetCount() == 1)
	{
		lst.GetHead()->OnSizeParent(pParent, rect, lParam);
		return;
	}

	if (m_bRecalcLayout)
		return;

	m_bRecalcLayout = TRUE;

	ASSERT(m_lstSpliters.GetCount() == lst.GetCount() - 1);

	int nTotalLength = 0;
	int nLengthAvail = 0;

	_AdjustPanesLength(GetDockingPaneManager(), lst, rect, m_bHoriz, TRUE, nTotalLength, nLengthAvail);

	pos = lst.GetHeadPosition();
	POSITION posSplitter = m_lstSpliters.GetHeadPosition();

	CRect rcPane(rect);
	while (pos)
	{
		CXTPDockingPaneBase* pPane = lst.GetNext(pos);

		int nLength = -pPane->m_nLength;
		if (pPane->m_nLength > 0)
		{
			nLength = nTotalLength == 0? 0: int((nLengthAvail * pPane->m_nLength) / nTotalLength);

			nTotalLength -= pPane->m_nLength;
			nLengthAvail = max(nLengthAvail - nLength, 0);
		}

		CRect rcSplitter;

		if (m_bHoriz)
		{
			rcPane.right = pos == NULL? rect.right: rcPane.left + nLength;

			pPane->OnSizeParent(pParent, rcPane, lParam);
			rcSplitter.SetRect(rcPane.right, rect.top, rcPane.right + nSplitterSize, rect.bottom);
			rcPane.left = rcPane.right + nSplitterSize;
		}  else
		{
			rcPane.bottom = pos == NULL? rect.bottom: rcPane.top + nLength;

			pPane->OnSizeParent(pParent, rcPane, lParam);
			rcSplitter.SetRect(rect.left, rcPane.bottom, rect.right, rcPane.bottom + nSplitterSize);
			rcPane.top = rcPane.bottom + nSplitterSize;
		}

		if (lpLayout->hDWP != NULL && posSplitter)
		{
			CXTPDockingPaneSplitterWnd* pSplitter = m_lstSpliters.GetNext(posSplitter);
			if (pSplitter)
			{
				rcSplitter.IntersectRect(rcSplitter, rect);
				pSplitter->SetWindowPos(&CWnd::wndBottom, rcSplitter.left, rcSplitter.top, rcSplitter.Width(), rcSplitter.Height(), 0);
				pSplitter->Invalidate(FALSE);
			}
		}
	}

	m_bRecalcLayout = FALSE;
}




POSITION CXTPDockingPaneSplitterContainer::ContainPane(CXTPDockingPaneBase* pPane)
{
	if (pPane == this)
		return (POSITION)TRUE;

	POSITION pos = m_lstPanes.GetHeadPosition();
	while (pos)
	{
		POSITION posContain = pos;

		if (m_lstPanes.GetNext(pos)->ContainPane(pPane))
			return posContain;
	}
	return NULL;
}

void CXTPDockingPaneSplitterContainer::FindPane(XTPDockingPaneType type, CXTPDockingPaneBaseList* pList)
{
	POSITION pos = m_lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane =  (CXTPDockingPaneBase*)m_lstPanes.GetNext(pos);
		if (pPane->IsEmpty())
			continue;

		if (pPane->GetType() == type)
		{
			pList->AddTail(pPane);
		}
		else
		{
			pPane->FindPane(type, pList);
		}
	}
}

BOOL CXTPDockingPaneSplitterContainer::IsEmpty()
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

void CXTPDockingPaneSplitterContainer::Copy(CXTPDockingPaneSplitterContainer* pClone, CXTPPaneToPaneMap* pMap)
{
	ASSERT(pClone);

	m_bHoriz = pClone->m_bHoriz;
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
	m_szDocking = pClone->m_szDocking;
	m_rcWindow = pClone->m_rcWindow;
}

CXTPDockingPaneBase* CXTPDockingPaneSplitterContainer::Clone(CXTPDockingPaneLayout* pLayout, CXTPPaneToPaneMap* pMap, DWORD /*dwIgnoredOptions*/)
{
	CXTPDockingPaneSplitterContainer* pPane = (CXTPDockingPaneSplitterContainer*)GetDockingPaneManager()->OnCreatePane(GetType(), pLayout);

	pPane->Copy(this, pMap);

	SAFE_CALLPTR(pMap, SetAt(this, pPane));

	return pPane;
}

BOOL CXTPDockingPaneSplitterContainer::_Before(CXTPDockingPaneBase* pPane, POSITION pos)
{
	POSITION p = m_lstPanes.GetHeadPosition();
	while (p)
	{
		if (p == pos) return FALSE;
		CXTPDockingPaneBase* pBase = m_lstPanes.GetNext(p);
		if (pBase == pPane) return TRUE;
	}

	ASSERT(FALSE);
	return FALSE;
}

void CXTPDockingPaneSplitterContainer::_AdjustPanesLength(CXTPDockingPaneManager* pManager, CXTPDockingPaneBaseList& lst, CRect rect, BOOL bHoriz, BOOL bApply, int& nTotalLength, int& nLengthAvail)
{
	nTotalLength = 0;
	nLengthAvail = 0;

	CXTPDockingPaneBase* pClientPane = NULL;
	CXTPDockingPaneBase* pClient = pManager->GetClientPane();
	int nSplitterSize = pManager->GetPaintManager()->m_nSplitterSize;

	POSITION pos = lst.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane = lst.GetNext(pos);
		pPane->m_nLength = bHoriz? pPane->m_szDocking.cx: pPane->m_szDocking.cy;

		MINMAXINFO mmi;
		pPane->GetMinMaxInfo(&mmi);
		pPane->m_nLength = max(GetMinSize(&mmi, bHoriz), min(GetMaxSize(&mmi, bHoriz), pPane->m_nLength));

		if (pClientPane == NULL && pPane->ContainPane(pClient))
		{
			if (!pManager->m_bHideClient || pPane->m_szDocking == CSize(0, 0))
			{
				pClientPane = pPane;
				if (bApply) pClientPane->m_szDocking = CSize(0, 0);
				pClientPane->m_nLength = 0;
			}
		}
		nTotalLength += pPane->m_nLength;
	}
	nLengthAvail = (bHoriz? rect.Width(): rect.Height()) - (nSplitterSize * ((int)lst.GetCount() - 1));


	if (pClientPane && nTotalLength < nLengthAvail)
	{
		pClientPane->m_nLength = nLengthAvail - nTotalLength;
		if (bApply)
		{
			if (bHoriz) pClientPane->m_szDocking.cx = pClientPane->m_nLength;
			else pClientPane->m_szDocking.cy = pClientPane->m_nLength;
		}
		nTotalLength = nLengthAvail;
	}

	if (nLengthAvail > 0)
	{
		pos = lst.GetHeadPosition();
		while (pos)
		{
			CXTPDockingPaneBase* pPane = lst.GetNext(pos);

			if ((pPane->m_nLength < 0) || (nTotalLength == 0))
				continue;

			MINMAXINFO mmi;
			pPane->GetMinMaxInfo(&mmi);

			int nLength = pPane->m_nLength;

			if (nLengthAvail * nLength / nTotalLength < GetMinSize(&mmi, bHoriz))
				pPane->m_nLength = -GetMinSize(&mmi, bHoriz);
			else if (nLengthAvail * nLength / nTotalLength > GetMaxSize(&mmi, bHoriz))
				pPane->m_nLength = -GetMaxSize(&mmi, bHoriz);

			if (pPane->m_nLength < 0)
			{
				nLengthAvail += pPane->m_nLength;
				nTotalLength -= nLength;

				if (nLengthAvail < 0)
				{
					pPane->m_nLength -= nLengthAvail;
					nLengthAvail = 0;
					break;
				}

				pos = lst.GetHeadPosition();
			}
		}
	}
}


CRect CXTPDockingPaneSplitterContainer::_CalculateResultDockingRect(BOOL bHoriz, CXTPDockingPaneBaseList& lst, CRect rect, CXTPDockingPaneBase* pPaneI)
{
	CXTPDockingPaneManager* pManager = pPaneI->GetDockingPaneManager();
	int nSplitterSize = pManager->GetPaintManager()->m_nSplitterSize;

	int nTotalLength = 0;
	int nLengthAvail = 0;

	_AdjustPanesLength(pManager, lst, rect, bHoriz, FALSE, nTotalLength, nLengthAvail);


	POSITION pos = lst.GetHeadPosition();

	CRect rcPane(rect);
	while (pos)
	{
		CXTPDockingPaneBase* pPane = lst.GetNext(pos);

		int nLength = -pPane->m_nLength;
		if (pPane->m_nLength > 0)
		{
			nLength = nTotalLength == 0? 0: int((nLengthAvail * pPane->m_nLength) / nTotalLength);

			nTotalLength -= pPane->m_nLength;
			nLengthAvail = max(nLengthAvail - nLength, 0);
		}

		if (bHoriz)
		{
			rcPane.right = pos == NULL? rect.right: rcPane.left + nLength;

			if (pPaneI == pPane)
				break;

			rcPane.left = rcPane.right + nSplitterSize;
		}  else
		{
			rcPane.bottom = pos == NULL? rect.bottom: rcPane.top + nLength;

			if (pPaneI == pPane)
				break;

			rcPane.top = rcPane.bottom + nSplitterSize;
		}
	}

	return rcPane;
}


CRect CXTPDockingPaneSplitterContainer::_CalculateResultDockingRectSelf(CXTPDockingPaneBase* pPaneI, XTPDockingPaneDirection direction, CXTPDockingPaneBase* pNeighbour)
{
	ASSERT(pNeighbour);

	CRect rect = m_rcWindow;
	m_pParentFrame->ClientToScreen(rect);

	BOOL bAfter = (direction == xtpPaneDockRight || direction == xtpPaneDockBottom);

	// getting list of nonempty panes
	CXTPDockingPaneBaseList lst;
	POSITION pos = m_lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane = (CXTPDockingPaneBase*)m_lstPanes.GetNext(pos);
		if (pPane->IsEmpty() || (pPane == pPaneI))
			continue;

		POSITION pos = lst.AddTail(pPane);

		if (pPane == pNeighbour)
		{
			if (bAfter) lst.InsertAfter(pos, pPaneI); else lst.InsertBefore(pos, pPaneI);
		}
	}

	return _CalculateResultDockingRect(m_bHoriz, lst, rect, pPaneI);
}


CRect CXTPDockingPaneSplitterContainer::_CalculateResultDockingRectChild(CXTPDockingPaneBase* pSplitter, CXTPDockingPaneBase* pPane, XTPDockingPaneDirection direction)
{
	BOOL bAfter = (direction == xtpPaneDockRight || direction == xtpPaneDockBottom);
	BOOL bHoriz = (direction == xtpPaneDockLeft || direction == xtpPaneDockRight);

	CXTPDockingPaneBaseList lst;
	lst.AddTail(pSplitter);

	if (bAfter) lst.AddTail(pPane); else lst.AddHead(pPane);

	CRect rect = pSplitter->GetWindowRect();

	return _CalculateResultDockingRect(bHoriz, lst, rect, pPane);

}



void CXTPDockingPaneSplitterContainer::GetMinMaxInfo(LPMINMAXINFO pMinMaxInfo)
{
	CXTPDockingPaneBase::GetMinMaxInfo(pMinMaxInfo);

	int nCount = 0;

	POSITION pos = m_lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane = m_lstPanes.GetNext(pos);
		if (pPane->IsEmpty())
			continue;

		nCount++;

		MINMAXINFO info;
		pPane->GetMinMaxInfo(&info);

		if (nCount == 1)
		{
			*pMinMaxInfo = info;

		}
		else
		{
			GetMinSize(pMinMaxInfo, m_bHoriz) += GetMinSize(&info, m_bHoriz);
			GetMaxSize(pMinMaxInfo, m_bHoriz) += GetMaxSize(&info, m_bHoriz);

			GetMinSize(pMinMaxInfo, !m_bHoriz) =  max(GetMinSize(pMinMaxInfo, !m_bHoriz), GetMinSize(&info, !m_bHoriz));
			GetMaxSize(pMinMaxInfo, !m_bHoriz) =  min(GetMaxSize(pMinMaxInfo, !m_bHoriz), GetMaxSize(&info, !m_bHoriz));
		}
	}

	if (nCount > 0)
	{
		GetMinSize(pMinMaxInfo, m_bHoriz) += GetPaintManager()->m_nSplitterSize * (nCount - 1);
		GetMaxSize(pMinMaxInfo, m_bHoriz) += GetPaintManager()->m_nSplitterSize * (nCount - 1);
	}

}
