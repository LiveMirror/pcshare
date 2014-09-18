// XTPPropertyGrid.cpp : implementation of the CXTPPropertyGrid class.
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
#include "XTPResourceManager.h"
#include "XTPVC80Helpers.h"
#include "XTPResource.h"
#include "XTPPropertyGridResource.h"
#include "XTPDrawHelpers.h"

#include "XTPPropertyGridInplaceEdit.h"
#include "XTPPropertyGridInplaceButton.h"
#include "XTPPropertyGridInplaceList.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGrid.h"
#include "XTPPropertyGridDefines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int SPLITTER_HEIGHT = 3;
const int TOOLBAR_HEIGHT  = 25;

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridUpdateContext

CXTPPropertyGridUpdateContext::CXTPPropertyGridUpdateContext()
{
}

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridVerb

CXTPPropertyGridVerb::CXTPPropertyGridVerb()
{
}

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridVerbs

CXTPPropertyGridVerbs::CXTPPropertyGridVerbs()
{

}

CXTPPropertyGridVerbs::~CXTPPropertyGridVerbs()
{
	for (int i = 0; i < m_arrVerbs.GetSize(); i++)
		m_arrVerbs[i]->InternalRelease();
}

void CXTPPropertyGridVerbs::RemoveAll()
{
	if (IsEmpty())
		return;

	for (int i = 0; i < m_arrVerbs.GetSize(); i++)
		m_arrVerbs[i]->InternalRelease();

	m_arrVerbs.RemoveAll();

	m_pGrid->OnVerbsChanged();
}

int CXTPPropertyGridVerbs::GetCount()
{
	return (int)m_arrVerbs.GetSize();
}

void CXTPPropertyGridVerbs::Add(LPCTSTR strCaption, UINT nID)
{
	CXTPPropertyGridVerb* pVerb = new CXTPPropertyGridVerb();

	pVerb->m_nID = nID;
	pVerb->m_strCaption = strCaption;
	pVerb->m_pVerbs = this;
	pVerb->m_rcPart.SetRectEmpty();
	pVerb->m_ptClick = CPoint(0);

	pVerb->m_nIndex = (UINT)m_arrVerbs.Add(pVerb);

	m_pGrid->OnVerbsChanged();
}


/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridToolBar

CXTPPropertyGridToolBar::CXTPPropertyGridToolBar()
{
	m_cxLeftBorder = m_cxRightBorder = m_cyBottomBorder = m_cyTopBorder = 0;
}

BEGIN_MESSAGE_MAP(CXTPPropertyGridToolBar, CToolBar)
	//{{AFX_MSG_MAP(CToolBar)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCCALCSIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTPPropertyGridToolBar::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CXTPPropertyGridToolBar::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS* /*lpncsp*/)
{
}

void CXTPPropertyGridToolBar::OnPaint()
{
	CPaintDC dc(this);

	CXTPClientRect rc(this);
	CXTPBufferDC memDC(dc, rc);
	memDC.FillSolidRect(rc,GetXtremeColor(COLOR_3DFACE));

	CToolBar::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
}

void CXTPPropertyGridToolBar::OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL /*bDisableIfNoHndler*/)
{
}


/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGrid

CXTPPropertyGrid::CXTPPropertyGrid()
: m_bHelpVisible(TRUE)
, m_bToolBarVisible(FALSE)
, m_nHelpHeight(58)
{
	m_pView = 0;

	m_hCursorSplit = AfxGetApp()->LoadCursor( XTP_IDC_VSPLITBAR);

	m_hCursorHand = ::LoadCursor(0, MAKEINTRESOURCE(32649));

	if (m_hCursorHand == 0)
		m_hCursorHand = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(XTP_IDC_HAND));


	m_pPaintManager = new CXTPPropertyGridPaintManager(this);
	m_themeCurrent = xtpGridThemeDefault;

	m_pVerbs = new CXTPPropertyGridVerbs;
	m_pVerbs->m_pGrid = this;

	m_nVerbsHeight = 25;
	m_nFocusedVerb = -1;
	m_bVerbActivate = FALSE;
	m_bVerbsVisible = FALSE;

	m_bTabItems = FALSE;
}


CXTPPropertyGrid::~CXTPPropertyGrid()
{
	delete m_pPaintManager;

	if (m_pView)
	{
		delete m_pView;
	}
	m_pVerbs->InternalRelease();
}


CXTPPropertyGridView* CXTPPropertyGrid::CreateView()
{
	return new CXTPPropertyGridView();
}

CXTPPropertyGridView& CXTPPropertyGrid::GetGridView()
{
	if (m_pView == 0)
	{
		m_pView = CreateView();
		m_pView->m_pGrid = this;
	}

	return *m_pView;
}

IMPLEMENT_DYNAMIC(CXTPPropertyGrid, CWnd)

BEGIN_MESSAGE_MAP(CXTPPropertyGrid, CWnd)
	//{{AFX_MSG_MAP(CXTPPropertyGrid)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_COMMAND(XTP_IDC_SORT_ALPHABETIC, OnSortAlphabetic)
	ON_COMMAND(XTP_IDC_SORT_CATEGORIZED, OnSortCategorized)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

UINT CXTPPropertyGrid::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}


BOOL CXTPPropertyGrid::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwListStyle /*= LBS_OWNERDRAWFIXED| LBS_NOINTEGRALHEIGHT*/)
{
	LPCTSTR pszCreateClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW, ::LoadCursor(NULL, IDC_ARROW));
	CXTPEmptyRect rc;

	if (!CreateEx(0, pszCreateClass, _T(""), WS_TABSTOP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_CHILD|WS_GROUP, rc, pParentWnd, nID))
		return FALSE;

	if (!GetGridView().CreateEx(WS_EX_STATICEDGE, _T("LISTBOX"), NULL, WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_CHILD|WS_VISIBLE|LBS_NOTIFY|WS_VSCROLL|WS_TABSTOP|dwListStyle, rc, this, 0))
		return FALSE;

	GetGridView().ResetContent();

	SetWindowPos(0, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW | SWP_NOZORDER);
	SetStandardColors();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGrid message handlers

BOOL CXTPPropertyGrid::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CXTPPropertyGrid::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint, rc);

	m_pPaintManager->FillPropertyGrid(&dc);


}

CWnd* CXTPPropertyGrid::GetNextGridTabItem(BOOL bForward)
{
	CWnd* pParent = this;
	if (!pParent || !pParent->GetParent())
	{
		ASSERT(FALSE);
		return 0 ;
	}
	CWnd* pNextItem = pParent->GetParent()->GetNextDlgTabItem(pParent, bForward);

	if (pNextItem == pParent)
		return 0;

	return pNextItem;
}

void CXTPPropertyGrid::OnNavigate(XTPPropertyGridUI nUIElement, BOOL bForward, CXTPPropertyGridItem* pItem)
{
	int nNextElement = bForward? +1: -1;
	int nUI = nUIElement + nNextElement;

	if (pItem == NULL)
		pItem = GetGridView().GetSelectedItem();

	while (nUI != nUIElement)
	{
		if (nUI == xtpGridUIViewPrev)
		{
			nUI += nNextElement;

			if (m_bTabItems)
			{
				int nIndex = !bForward && pItem? pItem->m_nIndex - 1: 0;
				if (nIndex >= 0)
				{
					GetGridView().SetFocus();
					GetGridView().SetCurSel(nIndex);
					GetGridView().OnSelectionChanged();

					if (bForward)
						return;

					nUI = xtpGridUIInplaceButton;
					pItem = GetGridView().GetSelectedItem();
				}
			}
		}
		if (nUI == xtpGridUIView)
		{
			GetGridView().SetFocus();
			return;
		}
		if (nUI == xtpGridUIInplaceEdit)
		{
			if (pItem && pItem->GetFlags() & xtpGridItemHasEdit)
			{
				pItem->OnSelect();
				pItem->GetInplaceEdit().SetFocus();
				pItem->GetInplaceEdit().SetSel(0, -1);
				return;
			}
			nUI += nNextElement;
		}
		if (nUI == xtpGridUIInplaceButton)
		{
			if (pItem && pItem->GetInplaceButton().GetSafeHwnd() &&  pItem->GetInplaceButton().IsWindowVisible() && pItem->GetInplaceButton().GetItem() == pItem)
			{
				pItem->GetInplaceButton().SetFocus();
				return;
			}
			nUI += nNextElement;
		}
		if (nUI == xtpGridUIViewNext)
		{
			if (m_bTabItems)
			{
				int nIndex = bForward? (pItem? pItem->m_nIndex + 1: 0): GetGridView().GetCount() - 1;
				if (nIndex < GetGridView().GetCount())
				{
					GetGridView().SetFocus();
					GetGridView().SetCurSel(nIndex);
					GetGridView().OnSelectionChanged();
					if (bForward)
						return;
					pItem = GetGridView().GetSelectedItem();
				}
			}

			nUI += nNextElement;
		}
		if (nUI == xtpGridUIVerb)
		{
			if (IsVerbsVisible())
			{
				m_nFocusedVerb = bForward? 0: m_pVerbs->GetCount() - 1;
				m_bVerbActivate = TRUE;
				SetFocus();
				m_bVerbActivate = FALSE;
				Invalidate(FALSE);
				return;
			}
			nUI += nNextElement;
		}
		if (nUI == xtpGridUIParentNext || nUI == xtpGridUIParentPrev)
		{
			CWnd* pWndNext = GetNextGridTabItem(!bForward);

			if (pWndNext != NULL)
			{
				pWndNext->SetFocus();
				return;
			}

			if (nUI == xtpGridUIParentNext)
				nUI = xtpGridUIViewPrev; else nUI = xtpGridUIVerb;
		}
		if (nUI < xtpGridUIParentPrev || nUI > xtpGridUIParentNext)
		{
			ASSERT(FALSE);
			return;
		}
	}
}

void CXTPPropertyGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_nFocusedVerb != -1 && IsVerbsVisible())
	{
		if (m_nFocusedVerb >= m_pVerbs->GetCount())
			m_nFocusedVerb = m_pVerbs->GetCount() -1;

		BOOL bForward = GetKeyState(VK_SHIFT) >= 0;

		if ((nChar == VK_TAB) && bForward && (m_nFocusedVerb == m_pVerbs->GetCount() -1))
		{
			OnNavigate(xtpGridUIVerb, TRUE, NULL);
			return;

		}
		else if ((nChar == VK_TAB) && !bForward && (m_nFocusedVerb == 0))
		{
			OnNavigate(xtpGridUIVerb, FALSE, NULL);
			return;
		}
		else if (nChar == VK_RIGHT || nChar == VK_DOWN || ((nChar == VK_TAB) && bForward))
		{
			m_nFocusedVerb ++;
			if (m_nFocusedVerb >= m_pVerbs->GetCount())
				m_nFocusedVerb = 0;
			Invalidate(FALSE);
		}
		else if (nChar == VK_LEFT || nChar == VK_UP || nChar == VK_TAB)
		{
			m_nFocusedVerb --;
			if (m_nFocusedVerb < 0)
				m_nFocusedVerb = m_pVerbs->GetCount() -1;
			Invalidate(FALSE);
		}
		else if (nChar == VK_RETURN)
		{
			CRect rcPart = m_pVerbs->GetAt(m_nFocusedVerb)->GetPart();
			OnVerbClick(m_nFocusedVerb, CPoint(rcPart.left, rcPart.bottom));
		}
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPPropertyGrid::OnKillFocus(CWnd* pNewWnd)
{
	if (m_nFocusedVerb != -1)
	{
		m_nFocusedVerb = -1;
		Invalidate(FALSE);
	}
	CWnd::OnKillFocus(pNewWnd);
}

void CXTPPropertyGrid::OnSetFocus(CWnd* pOldWnd)
{
	if (!m_bVerbActivate)
	{
		if (pOldWnd && pOldWnd == GetNextGridTabItem(FALSE) && GetKeyState(VK_SHIFT) < 0)
		{
			OnNavigate(xtpGridUIParentNext, FALSE, NULL);
		}
		else if (pOldWnd && pOldWnd == GetNextGridTabItem(TRUE) && GetKeyState(VK_SHIFT) >= 0)
		{
			OnNavigate(xtpGridUIParentPrev, TRUE, NULL);
		}
		else
		{
			GetGridView().SetFocus();
		}
	}
	else
	{
		CWnd::OnSetFocus(pOldWnd);
	}
}

void CXTPPropertyGrid::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (!GetGridView().GetSafeHwnd())
		return;

	Reposition(cx, cy);
}

int CXTPPropertyGrid::HitTest(CPoint pt)
{
	CXTPClientRect rc(this);

	if (m_bHelpVisible)
	{
		CRect rcSplitter(CPoint(rc.left, rc.bottom - SPLITTER_HEIGHT - m_nHelpHeight), CSize(rc.Width(), SPLITTER_HEIGHT));

		if (rcSplitter.PtInRect(pt))
			return xtpGridHitHelpSplitter;

		rc.bottom -= SPLITTER_HEIGHT + m_nHelpHeight;
	}
	if (IsVerbsVisible())
	{
		CRect rcSplitter(CPoint(rc.left, rc.bottom - SPLITTER_HEIGHT - m_nVerbsHeight), CSize(rc.Width(), SPLITTER_HEIGHT));

		if (rcSplitter.PtInRect(pt))
			return xtpGridHitVerbsSplitter;

		CRect rcVerbs(rc);
		rcVerbs.top = rc.bottom - m_nVerbsHeight;

		if (rcVerbs.PtInRect(pt))
		{
			CWindowDC dc(this);
			int nIndex = m_pPaintManager->HitTestVerbs(&dc, rcVerbs, pt);
			if (nIndex != -1)
			{
				return xtpGridHitFirstVerb + nIndex;
			}
		}
	}

	return xtpGridHitError;
}

BOOL CXTPPropertyGrid::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest == HTCLIENT)
	{
		CPoint point;

		GetCursorPos(&point);
		ScreenToClient(&point);

		int nHitTest = HitTest(point);

		if (nHitTest == xtpGridHitHelpSplitter || nHitTest == xtpGridHitVerbsSplitter)
		{
			SetCursor( m_hCursorSplit );
			return TRUE;
		}
		if ((nHitTest != -1) && (nHitTest >= xtpGridHitFirstVerb))
		{
			SetCursor( m_hCursorHand );
			return TRUE;
		}
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CXTPPropertyGrid::OnInvertTracker(CRect rect)
{
	ASSERT_VALID(this);
	ASSERT(!rect.IsRectEmpty());
	ASSERT((GetStyle() & WS_CLIPCHILDREN) == 0);

	// pat-blt without clip children on
	CDC* pDC = GetDC();

	CBrush brush(GetXtremeColor(COLOR_3DFACE));
	CBrush* pBrush = (CBrush*)pDC->SelectObject(&brush);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);
	pDC->SelectObject(pBrush);

	ReleaseDC(pDC);
}

void CXTPPropertyGrid::OnSelectionChanged(CXTPPropertyGridItem* pItem)
{
	CXTPClientRect rc(this);
	CRect rcHelp(rc);
	rcHelp.top = rc.bottom - m_nHelpHeight;

	if (pItem) GetOwner()->SendMessage(XTPWM_PROPERTYGRID_NOTIFY, XTP_PGN_SELECTION_CHANGED, (LPARAM)pItem);

	InvalidateRect(rcHelp, FALSE);
}

void CXTPPropertyGrid::OnVerbsChanged()
{
	if (m_bVerbsVisible != IsVerbsVisible() && GetGridView().m_nLockUpdate == 0)
	{
		CXTPClientRect rc(this);
		Reposition(rc.Width(), rc.Height());
	}
	else if (GetSafeHwnd() && GetGridView().m_nLockUpdate == 0)
	{
		Invalidate(FALSE);
	}
}

void CXTPPropertyGrid::OnVerbClick(int nIndex, CPoint pt)
{
	ClientToScreen(&pt);
	ASSERT(nIndex < m_pVerbs->GetCount());

	CXTPPropertyGridVerb* pVerb = m_pVerbs->GetAt(nIndex);
	pVerb->m_ptClick = pt;

	GetOwner()->SendMessage(XTPWM_PROPERTYGRID_NOTIFY, XTP_PGN_VERB_CLICK, (LPARAM)pVerb);
}

void CXTPPropertyGrid::OnLButtonUp(UINT nFlags, CPoint point)
{
	if ((m_nFocusedVerb != -1) && (m_nFocusedVerb == HitTest(point) - xtpGridHitFirstVerb))
	{
		OnVerbClick(m_nFocusedVerb, point);
	}
	else
	{
		CWnd::OnLButtonUp(nFlags, point);
	}

}

void CXTPPropertyGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	int nHitTest = HitTest(point);

	if ((nHitTest != -1) && (nHitTest >= xtpGridHitFirstVerb))
	{
		m_nFocusedVerb = nHitTest - xtpGridHitFirstVerb;
		m_bVerbActivate = TRUE;
		SetFocus();
		m_bVerbActivate = FALSE;
		Invalidate(FALSE);
		return;

	}
	SetFocus();

	if ((nHitTest == xtpGridHitHelpSplitter) || (nHitTest == xtpGridHitVerbsSplitter))
	{
		SetCapture();
		CXTPClientRect rc(this);
		BOOL bHelpHitTest = nHitTest == xtpGridHitHelpSplitter;

		CRect rcAvail = bHelpHitTest? CRect(0, 20 + TOOLBAR_HEIGHT, rc.right, rc.bottom):
			CRect(0, 20 + TOOLBAR_HEIGHT, rc.right, rc.bottom - (IsHelpVisible()? m_nHelpHeight + SPLITTER_HEIGHT: 0));

		ModifyStyle(WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 0, FALSE);

		m_rectTracker.SetRect(0, point.y, rc.Width(), point.y + 3);

		OnInvertTracker(m_rectTracker);

		BOOL bAccept = FALSE;
		while (GetCapture() == this)
		{
			MSG msg;
			if (!GetMessage(&msg, NULL, 0, 0))
				break;

			if (msg.message == WM_MOUSEMOVE)
			{
				point = CPoint(msg.lParam);
				point.y = __min(point.y, rcAvail.bottom - 20);
				point.y = __max((int)point.y, int(rcAvail.top));

				if (m_rectTracker.top != point.y)
				{
					OnInvertTracker(m_rectTracker);
					m_rectTracker.OffsetRect(0, point.y - m_rectTracker.top);
					OnInvertTracker(m_rectTracker);
				}
			}
			else if (msg.message == WM_KEYDOWN &&  msg.wParam == VK_ESCAPE) break;
			else if (msg.message ==WM_LBUTTONUP)
			{
				bAccept = TRUE;
				break;
			}
			else  ::DispatchMessage(&msg);
		}

		ReleaseCapture();

		if (bAccept)
		{
			if (bHelpHitTest) m_nHelpHeight = rcAvail.bottom - m_rectTracker.top -2; else
				m_nVerbsHeight = rcAvail.bottom - m_rectTracker.top -2;
			Reposition(rc.Width(), rc.Height());
		}
		Invalidate(FALSE);
		ModifyStyle(0, WS_CLIPCHILDREN|WS_CLIPSIBLINGS, FALSE);
		return;
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CXTPPropertyGrid::Reposition(int cx, int cy)
{
	if (GetGridView().m_nLockUpdate > 0)
		return;

	if (GetSafeHwnd() == 0)
		return;

	CRect rcView(0, 0, cx, cy);

	if (m_bHelpVisible)
	{
		rcView.bottom -= m_nHelpHeight + SPLITTER_HEIGHT;
	}

	if (IsVerbsVisible())
	{
		rcView.bottom -= m_nVerbsHeight + SPLITTER_HEIGHT;
	}

	m_bVerbsVisible = IsVerbsVisible();

	if (m_bToolBarVisible)
	{
		ASSERT(m_wndToolbar.GetSafeHwnd());
		CRect rcToolBar(1, 1, cx - 1, TOOLBAR_HEIGHT - 1);
		m_wndToolbar.MoveWindow(rcToolBar);

		rcView.top += TOOLBAR_HEIGHT;
	}

	GetGridView().MoveWindow(rcView);
	Invalidate(FALSE);
}

void CXTPPropertyGrid::CreateToolbar()
{
#if _MSC_VER < 1200
	m_wndToolbar.Create(this, TBSTYLE_FLAT|WS_CHILD|WS_VISIBLE|CBRS_TOOLTIPS|WS_BORDER, 0);
#else
	m_wndToolbar.CreateEx(this, TBSTYLE_FLAT, WS_BORDER|WS_CHILD|WS_VISIBLE|CBRS_ALIGN_TOP|CBRS_TOOLTIPS);
#endif

	VERIFY(XTPResourceManager()->LoadToolBar(&m_wndToolbar, XTP_IDR_TBAR_PROPGRID));
}

void CXTPPropertyGrid::ShowToolBar(BOOL bShow)
{
	if (bShow && !m_wndToolbar.GetSafeHwnd())
	{
		CreateToolbar();
	}
	else
	{
		if (m_wndToolbar.GetSafeHwnd())
			m_wndToolbar.ShowWindow(bShow? SW_SHOW: SW_HIDE);
	}

	m_bToolBarVisible = bShow;

	CXTPClientRect rc(this);
	Reposition(rc.Width(), rc.Height());
	RefreshToolBarButtons();
}

void CXTPPropertyGrid::RefreshToolBarButtons()
{
	if (m_bToolBarVisible)
	{
		m_wndToolbar.GetToolBarCtrl().SetState(XTP_IDC_SORT_CATEGORIZED, TBSTATE_ENABLED | (GetGridView().m_properetySort == xtpGridSortCategorized? TBSTATE_CHECKED: 0));
		m_wndToolbar.GetToolBarCtrl().SetState(XTP_IDC_SORT_ALPHABETIC, TBSTATE_ENABLED | (GetGridView().m_properetySort == xtpGridSortAlphabetical? TBSTATE_CHECKED: 0));
	}
}

void CXTPPropertyGrid::ShowHelp(BOOL bShow)
{
	m_bHelpVisible = bShow;

	CXTPClientRect rc(this);
	Reposition(rc.Width(), rc.Height());

}
void CXTPPropertyGrid::OnSortAlphabetic()
{
	SetPropertySort(xtpGridSortAlphabetical);
}

void CXTPPropertyGrid::OnSortCategorized()
{
	SetPropertySort(xtpGridSortCategorized);
}

void CXTPPropertyGrid::OnSortChanged()
{
	RefreshToolBarButtons();
	OnSelectionChanged(NULL);
	GetOwner()->SendMessage(XTPWM_PROPERTYGRID_NOTIFY, XTP_PGN_SORTORDER_CHANGED, (LPARAM)GetDlgCtrlID());
}

void CXTPPropertyGrid::SetStandardColors()
{
	m_clrHelpBack.SetDefaultValue();
	m_clrHelpFore.SetDefaultValue();
	GetGridView().m_clrLine.SetDefaultValue();
	GetGridView().m_clrBack.SetDefaultValue();
	GetGridView().m_clrFore.SetDefaultValue();
	GetGridView().m_clrReadOnlyFore.SetDefaultValue();
	GetGridView().m_clrCategoryFore.SetDefaultValue();

	m_pPaintManager->RefreshMetrics();

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW| RDW_INVALIDATE | RDW_ERASE);
}

void CXTPPropertyGrid::SetCustomColors(
	COLORREF clrHelpBack, COLORREF clrHelpFore, COLORREF clrViewLine,
	COLORREF clrViewBack, COLORREF clrViewFore, COLORREF clrCategoryFore )
{
	m_clrHelpBack.SetCustomValue(clrHelpBack);
	m_clrHelpFore.SetCustomValue(clrHelpFore);
	GetGridView().m_clrLine.SetCustomValue(clrViewLine);
	GetGridView().m_clrBack.SetCustomValue(clrViewBack);
	GetGridView().m_clrFore.SetCustomValue(clrViewFore);
	GetGridView().m_clrCategoryFore.SetCustomValue(clrCategoryFore);

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW| RDW_INVALIDATE | RDW_ERASE);
}

void CXTPPropertyGrid::ResetContent()
{
	m_pVerbs->RemoveAll();
	GetGridView().ResetContent();

	GetGridView().m_nLockUpdate++;
	GetGridView().m_pCategories->Clear();
	GetGridView().m_nLockUpdate--;
}

CXTPPropertyGridItems* CXTPPropertyGrid::GetCategories()
{
	return GetGridView().m_pCategories;
}

CXTPPropertyGridItem* CXTPPropertyGrid::AddCategory(int nID)
{
	CString strCaption;
	VERIFY(strCaption.LoadString(nID));
	CXTPPropertyGridItem* pItem = GetGridView().AddCategory(strCaption);
	pItem->SetID(nID);
	return pItem;
}

CXTPPropertyGridItem* CXTPPropertyGrid::InsertCategory(int nIndex, int nID)
{
	CString strCaption;
	VERIFY(strCaption.LoadString(nID));
	CXTPPropertyGridItem* pItem = GetGridView().InsertCategory(nIndex, strCaption);
	pItem->SetID(nID);
	return pItem;
}


void CXTPPropertyGrid::SetFont(CFont* pFont)
{

	LOGFONT lf;
	if (pFont->GetLogFont(&lf))
	{
		GetGridView().m_fontNormal.DeleteObject();
		GetGridView().m_fontBold.DeleteObject();

		lf.lfWeight = FW_NORMAL;
		VERIFY( GetGridView().m_fontNormal.CreateFontIndirect(&lf));

		lf.lfWeight = FW_BOLD;
		VERIFY( GetGridView().m_fontBold.CreateFontIndirect(&lf));

		if (GetGridView().m_wndEdit.GetSafeHwnd())
		{
			GetGridView().m_wndEdit.SetFont(&GetGridView().m_fontNormal);
		}

		Refresh();
	}
}

void CXTPPropertyGrid::Refresh()
{
	GetGridView().SetPropertySort(GetPropertySort(), TRUE);
}


void CXTPPropertyGrid::SaveExpandingState(CXTPPropertyGridItems* pItems, CMap<UINT, UINT, BOOL, BOOL&>* pState, UINT& nSelected)
{
	for (int nItem = 0; nItem < pItems->GetCount(); nItem++)
	{
		CXTPPropertyGridItem* pItem = pItems->GetAt(nItem);

		if (pItem->GetID() != 0)
		{
			if (pItem->HasChilds())
			{
				BOOL bExpanded = pItem->IsExpanded();
				pState->SetAt(pItem->GetID(), bExpanded);
			}

			if (pItem->IsSelected()) nSelected = pItem->GetID();
		}
		SaveExpandingState(pItem->GetChilds(), pState, nSelected);
	}
}

void CXTPPropertyGrid::RestoreExpandingState(CXTPPropertyGridItems* pItems, CMap<UINT, UINT, BOOL, BOOL&>* pState)
{
	for (int nItem = 0; nItem < pItems->GetCount(); nItem++)
	{
		CXTPPropertyGridItem* pItem = pItems->GetAt(nItem);

		BOOL bExpanded;
		if (pItem->GetID() != 0 && pState->Lookup(pItem->GetID(), bExpanded))
		{
			if (bExpanded) pItem->Expand(); else pItem->Collapse();
		}
		RestoreExpandingState(pItem->GetChilds(), pState);
	}
}

void CXTPPropertyGrid::BeginUpdate(CXTPPropertyGridUpdateContext& context)
{
	GetGridView().m_nLockUpdate = 1;

	context.m_nSelected = 0;
	SaveExpandingState(GetCategories(), &context.m_mapState, context.m_nSelected);
	context.m_propertySort = GetPropertySort();

	ResetContent();
}

void CXTPPropertyGrid::EndUpdate(CXTPPropertyGridUpdateContext& context)
{
	RestoreExpandingState(GetCategories(), &context.m_mapState);
	GetGridView().m_nLockUpdate = 0;

	GetGridView().SetPropertySort(context.m_propertySort, TRUE);
	if (context.m_nSelected > 0)
	{
		for (int i = 0; i < GetGridView().GetCount(); i++)
		{
			CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)GetGridView().GetItemDataPtr(i);
			if (pItem && pItem->GetID() == context.m_nSelected)
			{
				pItem->Select();
				break;
			}
		}
	}

	CXTPClientRect rc(this);
	Reposition(rc.Width(), rc.Height());

}



void CXTPPropertyGrid::SetVerbsHeight(int nHeight)
{
	ASSERT(nHeight > 0);
	m_nVerbsHeight = nHeight;

	if (GetSafeHwnd())
	{
		CXTPClientRect rc(this);
		Reposition(rc.Width(), rc.Height());
	}

}

void CXTPPropertyGrid::SetHelpHeight(int nHeight)
{

	ASSERT(nHeight > 0);
	m_nHelpHeight = nHeight;

	if (GetSafeHwnd())
	{
		CXTPClientRect rc(this);
		Reposition(rc.Width(), rc.Height());
	}
}

void CXTPPropertyGrid::SetViewDivider(double dDivider)
{
	ASSERT(dDivider > 0 && dDivider < 1);
	GetGridView().m_dDivider = dDivider;

	if (GetSafeHwnd())
	{
		CXTPClientRect rc(this);
		Reposition(rc.Width(), rc.Height());
	}
}

CXTPPropertyGridItem* CXTPPropertyGrid::GetSelectedItem()
{
	return GetGridView().GetSelectedItem();
}

void CXTPPropertyGrid::SetTheme(XTPPropertyGridPaintTheme paintTheme)
{
	ASSERT(m_pPaintManager);
	delete m_pPaintManager;

	if (paintTheme == xtpGridThemeDefault) m_pPaintManager = new CXTPPropertyGridPaintManager(this);
	else if (paintTheme == xtpGridThemeCool) m_pPaintManager = new CXTPPropertyGridCoolTheme(this);
	else if (paintTheme == xtpGridThemeDelphi) m_pPaintManager = new CXTPPropertyGridDelphiTheme(this);
	else if (paintTheme == xtpGridThemeNativeWinXP) m_pPaintManager = new CXTPPropertyGridNativeXPTheme(this);
	else if (paintTheme == xtpGridThemeOffice2003) m_pPaintManager = new CXTPPropertyGridOffice2003Theme(this);
	else if (paintTheme == xtpGridThemeSimple) m_pPaintManager = new CXTPPropertyGridSimpleTheme(this);
	else if (paintTheme == xtpGridThemeWhidbey) m_pPaintManager = new CXTPPropertyGridWhidbeyTheme(this);
	else { ASSERT(FALSE); }

	m_themeCurrent = paintTheme;

	if (GetSafeHwnd()) RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW| RDW_INVALIDATE | RDW_ERASE);
}

void CXTPPropertyGrid::SetCustomTheme(CXTPPropertyGridPaintManager* pPaintManager)
{
	ASSERT(m_pPaintManager);
	delete m_pPaintManager;

	m_pPaintManager = pPaintManager;

	if (GetSafeHwnd()) RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW| RDW_INVALIDATE | RDW_ERASE);
}

void CXTPPropertyGrid::OnSysColorChange()
{
	CWnd::OnSysColorChange();

	m_pPaintManager->RefreshMetrics();
}


