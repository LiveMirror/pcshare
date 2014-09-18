// XTPDockBar.cpp : implementation of the CXTPDockBar class.
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
#include <afxpriv.h>
#include "XTPDrawHelpers.h"

#include "XTPDockBar.h"
#include "XTPToolBar.h"
#include "XTPCommandBars.h"
#include "XTPDockContext.h"
#include "XTPPaintManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CXTPDockBar

IMPLEMENT_DYNAMIC(CXTPDockBar, CWnd)

CXTPDockBar::CXTPDockBar(CXTPCommandBars* pCommandBars)
{
	m_arrBars.Add(NULL);
	m_dwStyle = 0;
	m_pCommandBars = pCommandBars;
}

CXTPDockBar::~CXTPDockBar()
{
}


BEGIN_MESSAGE_MAP(CXTPDockBar, CWnd)
	ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
	ON_WM_PAINT()
	ON_MESSAGE(WM_PRINT, OnPrint)
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



// CXTPDockBar message handlers

BOOL CXTPDockBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID)
{
	ASSERT(pParentWnd != NULL);
	ASSERT_KINDOF(CFrameWnd, pParentWnd);

	// save the xtp
	m_dwStyle = (dwStyle & CBRS_ALL);

	return CWnd::Create(AfxRegisterWndClass(0, LoadCursor(0, IDC_ARROW)), NULL, dwStyle, CXTPEmptyRect(), pParentWnd, nID);
}

BOOL CXTPDockBar::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	// force clipsliblings (otherwise will cause repaint problems)
	cs.style |= WS_CLIPSIBLINGS;
	return TRUE;
}



CSize CXTPDockBar::CalcFixedLayout(BOOL /*bStretch*/, BOOL /*bHorz*/, AFX_SIZEPARENTPARAMS* /*lpLayout*/)
{
	return 0;
}


struct CXTPDockBar::XTP_DOCK_INFO
{
	XTP_DOCK_INFO(CXTPToolBar* p = 0, CRect rc = 0, int n = 0) { pBar = p; rcMRUPos = rcBar = rc; nIndex = n; }
	CXTPToolBar* pBar;
	CRect rcBar;
	CRect rcMRUPos;
	int nIndex;
	int nMinWidth;
	int nTotlaMinWidth;
};

class CXTPDockBar::CXTPDockInfoArray : public CArray<XTP_DOCK_INFO, XTP_DOCK_INFO&>
{
public:
	void Sort()
	{
		qsort(GetData(), GetSize(), sizeof(XTP_DOCK_INFO),CompareFunc);
	}
	static int _cdecl CompareFunc(const void* elem1, const void* elem2)
	{
		return ((XTP_DOCK_INFO*)elem1)->rcBar.left < ((XTP_DOCK_INFO*)elem2)->rcBar.left ? -1 : 1;
	}
	void _swap(LONG& A, LONG& B)
	{
		int C = B;
		B = A;
		A = C;
	}
	void InvertRects()
	{
		for (int nPos = 0; nPos < GetSize(); nPos++ )
		{
			CRect& rect = ElementAt(nPos).rcBar;
			_swap(rect.left, rect.top);
			_swap(rect.right, rect.bottom);
		}
	}
};

int CXTPDockBar::_GetMode(BOOL bHorz, CXTPToolBar* pBar)
{
	DWORD dwMode = bHorz? LM_HORZ | LM_HORZDOCK: LM_VERTDOCK;

	if (pBar->GetFlags() & xtpFlagStretched) dwMode |= LM_STRETCH;
	if ((!m_pCommandBars->IsCustomizeMode() || m_pCommandBars->IsQuickCustomizeMode()) && pBar->GetFlags() & xtpFlagHideWrap)
		return dwMode | LM_HIDEWRAP;

	return dwMode;
}

int CXTPDockBar::_AdjustRow(CToolBarArray& arrRow, CPoint pt, int nLength, BOOL bHorz, AFX_SIZEPARENTPARAMS* lpLayout, int& nRemove)
{
	CXTPDockInfoArray arrInfo;

	int nPos;

	// Step 1. Getting maximum available size;
	for (nPos = 0; nPos < arrRow.GetSize(); nPos++)
	{
		CXTPToolBar* pBar = arrRow[nPos];
		CSize sizeBar = pBar->CalcDockingLayout(nLength, _GetMode(bHorz, pBar));

		CPoint p = bHorz? CPoint(pBar->m_pDockContext->m_rectMRUDockPos.left, pt.y):
			CPoint(pt.x, pBar->m_pDockContext->m_rectMRUDockPos.top);

		XTP_DOCK_INFO dockInfo(pBar, CRect(p, sizeBar), nPos);
		arrInfo.Add(dockInfo);
	}

	ASSERT(arrInfo.GetSize() == arrRow.GetSize());

	if (!bHorz) arrInfo.InvertRects();
	arrInfo.Sort();

	// Step 2. if Total length is more than available, fill empty area.
	int nIndex = -1;
	int nLen = 0; BOOL bMove = TRUE;
	for (nPos = 0; nPos < arrInfo.GetSize(); nPos++ )
	{
		CRect& rect = arrInfo[nPos].rcBar;

		bMove = (rect.left < nLen && (nIndex < arrInfo[nPos].nIndex || bMove));
		if (bMove) rect.OffsetRect(nLen - rect.left, 0);
		nLen = rect.right;

		nIndex = arrInfo[nPos].nIndex;
	}
	nLen = nLength; nIndex = -1; bMove = TRUE;
	for (nPos = (int)arrInfo.GetSize() - 1; nPos >= 0; nPos--)
	{
		CRect& rect = arrInfo[nPos].rcBar;
		bMove = (rect.right - nLen > 0 && (nIndex < arrInfo[nPos].nIndex || bMove));
		if (bMove) rect.OffsetRect(nLen - rect.right, 0);

		nLen = rect.left;
		nIndex = arrInfo[nPos].nIndex;
	}
	nLen = 0;
	for (nPos = 0; nPos < arrInfo.GetSize(); nPos++ )
	{
		CRect& rect = arrInfo[nPos].rcBar;
		if (rect.left < nLen)
			rect.OffsetRect(nLen - rect.left, 0);

		nLen = rect.left + rect.Width();
	}

	// Step 3. if Total length is more than available, make it expandable.
	if (nLen > nLength)
	{
		int nSum = 0;
		for (nPos = 0; nPos < arrInfo.GetSize(); nPos++ )
		{
			CXTPToolBar* pBar = arrInfo[nPos].pBar;

			if (!(_GetMode(bHorz, pBar) & LM_HIDEWRAP))
				arrInfo[nPos].nMinWidth = nLength;
			else
			{
				CSize sz = pBar->CalcDockingLayout(1, _GetMode(bHorz, pBar));
				arrInfo[nPos].nMinWidth = bHorz? sz.cx: sz.cy;
			}

			arrInfo[nPos].nTotlaMinWidth = nSum;
			nSum += arrInfo[nPos].nMinWidth;
		}

		int nLen = nLength;
		for (nPos = (int)arrInfo.GetSize() - 1; nPos >= 0; nPos--)
		{
			CRect& rect = arrInfo[nPos].rcBar;
			int nLeft = bHorz? arrInfo[nPos].rcMRUPos.left: arrInfo[nPos].rcMRUPos.top;

			if (nLeft >rect.left) nLeft = rect.left;
			if (nLeft < nLen - rect.Width()) nLeft = nLen - rect.Width();

			if (nLeft < arrInfo[nPos].nTotlaMinWidth) nLeft = arrInfo[nPos].nTotlaMinWidth;
			if (nLen - nLeft < arrInfo[nPos].nMinWidth) nLeft = nLen - arrInfo[nPos].nMinWidth;

			if ((nLen - nLeft < arrInfo[nPos].nMinWidth || nLeft < arrInfo[nPos].nTotlaMinWidth)
				&& arrInfo.GetSize() != 1)
			{
				nRemove = arrInfo[arrInfo.GetSize() - 1].nIndex;
				return 0;
			}

			rect.right = nLen;
			nLen = rect.left = max(0, nLeft);
		}
	}

	if (!bHorz) arrInfo.InvertRects();


	int nWidth = 0;
	// Calculate total width
	for (nPos = 0; nPos < arrInfo.GetSize(); nPos++)
	{
		CXTPToolBar* pBar = arrInfo[nPos].pBar;
		CRect& rect = arrInfo[nPos].rcBar;
		CSize sizeBar = pBar->CalcDockingLayout(bHorz? rect.Width(): rect.Height(), _GetMode(bHorz, pBar));
		nWidth = max(nWidth, bHorz? sizeBar.cy: sizeBar.cx);
	}

	// Step 4. Move it.
	for (nPos = 0; nPos < arrInfo.GetSize(); nPos++)
	{
		CXTPToolBar* pBar = arrInfo[nPos].pBar;
		CRect& rect = arrInfo[nPos].rcBar;

		CSize sz = pBar->CalcDockingLayout(bHorz? rect.Width(): rect.Height(), _GetMode(bHorz, pBar) | LM_COMMIT, nWidth );
		rect = CRect(rect.TopLeft(), sz);

		pBar->m_pDockContext->m_uMRUDockPosition= GetPosition();

		AfxRepositionWindow(lpLayout, pBar->m_hWnd, &rect);
		pBar->Invalidate(FALSE);
	}
	return nWidth;
}

int CXTPDockBar::AdjustRow(CToolBarArray& arrRow, CPoint pt, int nLength, BOOL bHorz, AFX_SIZEPARENTPARAMS* lpLayout)
{
	int nTotalWidth = 0;
	CToolBarArray arrNext;

	while (TRUE)
	{
		int nRemove = -1;
		ASSERT(arrRow.GetSize() > 0);
		int nWidth = _AdjustRow(arrRow, pt, nLength, bHorz, lpLayout, nRemove);

		if (nWidth == 0)
		{
			ASSERT(nRemove != -1);
			arrNext.Add(arrRow[nRemove]);
			arrRow.RemoveAt(nRemove);
		}
		else
		{
			nTotalWidth += nWidth;
			if (bHorz) pt.y += nWidth; else pt.x += nWidth;

			if (arrNext.GetSize() > 0)
			{
				arrRow.Copy(arrNext);
				arrNext.RemoveAll();
				continue;
			}
			return nTotalWidth;
		}
	}
	return 0;
}


CSize CXTPDockBar::CalcDynamicLayout(int nLength, DWORD nMode, AFX_SIZEPARENTPARAMS* lpLayout)
{
	BOOL bHorz = nMode & LM_HORZ;
	BOOL bStretch = nMode & LM_STRETCH;

	CSize sizeFixed (bStretch && bHorz ? 32767 : 0, bStretch && !bHorz ? 32767 : 0);

	CRect rectLayout = lpLayout->rect;
	BOOL bLayoutQuery = (lpLayout->hDWP == NULL);

	// prepare for layout
	AFX_SIZEPARENTPARAMS layout;
	layout.hDWP = bLayoutQuery ? 0 : ::BeginDeferWindowPos((int)m_arrBars.GetSize());

	CPoint pt(0, 0);
	// layout all the control bars
	for (int nPos = 0; nPos < m_arrBars.GetSize(); nPos++)
	{
		CToolBarArray lst;
		CXTPToolBar* pBar = GetDockedCommandBar(nPos);

		while (pBar)
		{
			if (pBar->IsVisible()) lst.Add(pBar);
			pBar = GetDockedCommandBar(++nPos);
		}

		int nWidth = 0;

		if (lst.GetSize() != 0)
		{
			nWidth = AdjustRow(lst, pt, nLength, bHorz, &layout);
			lst.RemoveAll();
		}
		if (pBar == NULL && nWidth != 0)
		{
			// end of row because pBar == NULL
			if (bHorz)
			{
				pt.y += nWidth;
				sizeFixed.cy = max(sizeFixed.cy, pt.y);
			}
			else
			{
				pt.x += nWidth;
				sizeFixed.cx = max(sizeFixed.cx, pt.x);
			}
		}
	}
	if (!bLayoutQuery)
	{
		// move and resize all the windows at once!
		if (layout.hDWP == NULL || !::EndDeferWindowPos(layout.hDWP))
			TRACE0("Warning: DeferWindowPos failed - low system resources.\n");
	}

	return sizeFixed;
}



LRESULT CXTPDockBar::OnSizeParent(WPARAM, LPARAM lParam)
{
	AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;

	DWORD dwStyle = GetStyle();
	if (dwStyle & WS_VISIBLE)
	{
		// align the control bar
		CRect rect;
		rect.CopyRect(&lpLayout->rect);

		CSize sizeAvail = rect.Size();  // maximum size available

		// get maximum requested size
		DWORD dwMode = lpLayout->bStretch ? LM_STRETCH : 0;
		if ((m_dwStyle & CBRS_SIZE_DYNAMIC) && m_dwStyle & CBRS_FLOATING)
			dwMode |= LM_HORZ | LM_MRUWIDTH;
		else if (dwStyle & CBRS_ORIENT_HORZ)
			dwMode |= LM_HORZ | LM_HORZDOCK;
		else
			dwMode |=  LM_VERTDOCK;


		int nLength = (dwStyle & CBRS_ORIENT_HORZ? sizeAvail.cx: sizeAvail.cy) /*- 2 * GetSystemMetrics(SM_CXBORDER)*/;

		CSize size = CalcDynamicLayout(nLength, dwMode, lpLayout);

		size.cx = min(size.cx, sizeAvail.cx);
		size.cy = min(size.cy, sizeAvail.cy);

		if (dwStyle & CBRS_ORIENT_HORZ)
		{
			lpLayout->sizeTotal.cy += size.cy;
			lpLayout->sizeTotal.cx = max(lpLayout->sizeTotal.cx, size.cx);
			if (dwStyle & CBRS_ALIGN_TOP)
				lpLayout->rect.top += size.cy;
			else if (dwStyle & CBRS_ALIGN_BOTTOM)
			{
				rect.top = rect.bottom - size.cy;
				lpLayout->rect.bottom -= size.cy;
			}
		}
		else if (dwStyle & CBRS_ORIENT_VERT)
		{
			lpLayout->sizeTotal.cx += size.cx;
			lpLayout->sizeTotal.cy = max(lpLayout->sizeTotal.cy, size.cy);
			if (dwStyle & CBRS_ALIGN_LEFT)
				lpLayout->rect.left += size.cx;
			else if (dwStyle & CBRS_ALIGN_RIGHT)
			{
				rect.left = rect.right - size.cx;
				lpLayout->rect.right -= size.cx;
			}
		}
		else
		{
			ASSERT(FALSE);      // can never happen
		}

		rect.right = rect.left + size.cx;
		rect.bottom = rect.top + size.cy;

		// only resize the window if doing layout and not just rect query
		if (lpLayout->hDWP != NULL)
			AfxRepositionWindow(lpLayout, m_hWnd, &rect);
		Invalidate(FALSE);
	}
	return 0;
}




void CXTPDockBar::OnPaint()
{
	CPaintDC dc(this);
	m_pCommandBars->GetPaintManager()->FillDockBar(&dc, this);
}

LRESULT CXTPDockBar::OnPrint(WPARAM wParam, LPARAM lParam)
{
	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (pDC)
	{
		m_pCommandBars->GetPaintManager()->FillDockBar(pDC, this);

		if (lParam & PRF_CHILDREN)
		{
			CWnd* pWnd = GetWindow(GW_CHILD);
			while (pWnd)
			{
				if (pWnd->IsWindowVisible())
				{
					CRect rc;
					GetWindowRect(&rc);
					pWnd->ScreenToClient(&rc);
					pDC->SetViewportOrg(-rc.left, -rc.top);
					pWnd->Print(pDC, (DWORD)lParam);
				}
				pWnd = pWnd->GetWindow(GW_HWNDNEXT);
			}
		}
	}

	return TRUE;
}


BOOL CXTPDockBar::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPDockBar::AdjustStretchBars()
{
	for (int nPos = 0; nPos < m_arrBars.GetSize(); nPos++)
	{
		CXTPToolBar* pBar = GetDockedCommandBar(nPos);
		if (pBar && pBar->GetFlags() & xtpFlagStretched)
		{
			if (nPos > 0 && GetDockedCommandBar(nPos - 1) != NULL)
			{
				m_arrBars.InsertAt(nPos, (CXTPToolBar*)NULL);
				nPos ++;
			}
			if (nPos < m_arrBars.GetSize() -1 && GetDockedCommandBar(nPos + 1) != NULL)
			{
				m_arrBars.InsertAt(nPos + 1, (CXTPToolBar*)NULL);
				nPos ++;
			}
		}
	}
}

void CXTPDockBar::DockCommandBar(CXTPToolBar* pBar, LPCRECT lpRect)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pBar);
	ASSERT_KINDOF(CXTPToolBar, pBar);

	CRect rectBar;
	pBar->GetWindowRect(&rectBar);
	if (pBar->m_pDockBar == this && (lpRect == NULL || rectBar == *lpRect))
	{
		// already docked and no change in position
		return;
	}

	int nPos = -1;
	if (lpRect != NULL)
	{
		// insert into appropriate row
		CRect rect(lpRect);
		ScreenToClient(&rect);
		CPoint ptMid(rect.left + rect.Width()/2, rect.top + rect.Height()/2);
		nPos = Insert(pBar, rect, ptMid);

		pBar->m_pDockContext->m_rectMRUDockPos = rect;
	}
	else
	{
		// always add on current row, then create new one
		m_arrBars.Add(pBar);
		m_arrBars.Add(NULL);

		pBar->m_pDockContext->m_rectMRUDockPos.SetRectEmpty();
	}

	// attach it to the docking site
	if (pBar->GetParent() != this)
		pBar->SetParent(this);
	if (pBar->m_pDockBar == this)
		pBar->m_pDockBar->RemoveCommandBar(pBar, nPos);
	else if (pBar->m_pDockBar != NULL)
		pBar->m_pDockBar->RemoveCommandBar(pBar, -1);
	pBar->m_pDockBar = this;

	AdjustStretchBars();

	// get parent frame for recalc layout
	GetDockingFrame()->DelayRecalcLayout();
}

BOOL CXTPDockBar::RemoveCommandBar(CXTPToolBar* pBar, int nPosExclude)
{
	ASSERT_VALID(this);
	ASSERT(pBar != NULL);
	int nPos = FindBar(pBar, nPosExclude);
	ASSERT(nPos > 0);

	m_arrBars.RemoveAt(nPos);
	if (m_arrBars[nPos-1] == NULL && m_arrBars[nPos] == NULL)
		m_arrBars.RemoveAt(nPos);

	pBar->m_pDockBar = NULL;

	// get parent frame for recalc layout/frame destroy
	CFrameWnd* pFrameWnd = GetDockingFrame();
	pFrameWnd->DelayRecalcLayout();


	return FALSE;
}



CXTPToolBar* CXTPDockBar::GetDockedCommandBar(int nPos) const
{
	CXTPToolBar* pResult = (CXTPToolBar*)m_arrBars[nPos];
	ASSERT(pResult == 0 || HIWORD(pResult) != 0); // XTPLibrary don't use placeholder
	return pResult;
}
int CXTPDockBar::GetDockedCount() const
{
	int nCount = 0;
	for (int i = 0; i < m_arrBars.GetSize(); i++)
	{
		if (GetDockedCommandBar(i) != NULL)
			nCount++;
	}
	return nCount;
}

int CXTPDockBar::GetDockedVisibleCount() const
{
	int nCount = 0;
	for (int i = 0; i < m_arrBars.GetSize(); i++)
	{
		CXTPToolBar* pBar = GetDockedCommandBar(i);
		if (pBar != NULL && pBar->IsVisible())
			nCount++;
	}
	return nCount;
}

int CXTPDockBar::FindBar(CXTPToolBar* pBar, int nPosExclude)
{
	for (int nPos = 0; nPos< m_arrBars.GetSize(); nPos++)
	{
		if (nPos != nPosExclude && m_arrBars[nPos] == pBar)
			return nPos;
	}
	return -1;
}

int CXTPDockBar::Insert(CXTPToolBar* pBarIns, CRect /*rect*/, CPoint ptMid)
{
	ASSERT_VALID(this);
	ASSERT(pBarIns != NULL);

	int nPos = 0;
	int nPosInsAfter = 0;
	int nWidth = 0;
	int nTotalWidth = 0;
	//int nPosInsBefore = 0;
	BOOL bHorz = m_dwStyle & CBRS_ORIENT_HORZ;

	for (nPos = 0; nPos < m_arrBars.GetSize(); nPos++)
	{
		CXTPToolBar* pBar = GetDockedCommandBar(nPos);
		if (pBar && !pBar->IsVisible())
			continue;

		if (pBar != NULL)
		{
			CRect rectBar;
			pBar->GetWindowRect(&rectBar);
			ScreenToClient(&rectBar);
			nWidth = max(nWidth,
				bHorz ? rectBar.bottom : rectBar.right);
		}
		else // end of row because pBar == NULL
		{
			if ((bHorz ? ptMid.y : ptMid.x) < nWidth)
			{
				if (nPos == 0 || ((bHorz ? ptMid.y : ptMid.x) == nTotalWidth)) // first section
					m_arrBars.InsertAt(nPosInsAfter+1, (CXTPToolBar*)NULL);
				m_arrBars.InsertAt(nPosInsAfter+1, pBarIns);

				return nPosInsAfter+1;
			}
			nTotalWidth = nWidth;
			nWidth = 0;
			nPosInsAfter = nPos;
		}
	}

	// create a new row
	m_arrBars.InsertAt(nPosInsAfter+1, (CXTPToolBar*)NULL);
	m_arrBars.InsertAt(nPosInsAfter+1, pBarIns);

	return nPosInsAfter+1;
}


XTPBarPosition CXTPDockBar::GetPosition()
{
	if (m_dwStyle & CBRS_TOP) return xtpBarTop;
	if (m_dwStyle & CBRS_BOTTOM) return xtpBarBottom;
	if (m_dwStyle & CBRS_LEFT) return xtpBarLeft;
	return xtpBarRight;
}

BOOL CXTPDockBar::IsVerticalPosition()
{
	return !(m_dwStyle & CBRS_ORIENT_HORZ);
}

void CXTPDockBar::OnRButtonDown(UINT /*nFlags*/, CPoint point)
{
	ClientToScreen(&point);
	m_pCommandBars->ContextMenu(point);
}


void CXTPDockBar::GetVisibleToolbars(int nPos, CToolBarArray& arrBars)
{
	ASSERT(nPos != -1);
	ASSERT(m_arrBars[nPos]);

	while (m_arrBars[nPos] != NULL && nPos > 0) nPos --;
	nPos ++;

	CXTPDockInfoArray arrInfo;

	while (m_arrBars[nPos] != NULL)
	{
		CXTPToolBar* pBar = m_arrBars[nPos++];
		if (pBar->IsVisible() && pBar->GetType() == xtpBarTypeNormal)
		{
			XTP_DOCK_INFO dockInfo(pBar, CXTPWindowRect(pBar), nPos);
			arrInfo.Add(dockInfo);
		}
	}

	if (m_dwStyle & CBRS_LEFT || m_dwStyle & CBRS_RIGHT) arrInfo.InvertRects();
	arrInfo.Sort();

	for (int i = 0; i < arrInfo.GetSize(); i++)
	{
		arrBars.Add(arrInfo[i].pBar);
	}

}
