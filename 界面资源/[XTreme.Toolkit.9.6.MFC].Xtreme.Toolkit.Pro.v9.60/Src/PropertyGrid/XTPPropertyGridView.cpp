// XTPPropertyGridView.cpp : implementation of the CXTPPropertyGridView class.
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
#include "XTPVC80Helpers.h"
#include "XTPResource.h"

#include "XTPPropertyGridInplaceEdit.h"
#include "XTPPropertyGridInplaceButton.h"
#include "XTPPropertyGridInplaceList.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGrid.h"
#include "XTPPropertyGridDefines.h"
#include "XTPPropertyGridPaintManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const UINT PGV_HIT_SPLITTER = 0x100;

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridToolTip

BEGIN_MESSAGE_MAP(CXTPPropertyGridToolTip, CWnd)
	//{{AFX_MSG_MAP(CXTPPropertyGridView)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

UINT CXTPPropertyGridToolTip::OnNcHitTest(CPoint point)
{
	UNREFERENCED_PARAMETER(point);
	return (UINT)HTTRANSPARENT;
}
void CXTPPropertyGridToolTip::Create(CXTPPropertyGridView* pParentWnd)
{
	CWnd::CreateEx(0, AfxRegisterWndClass(0, LoadCursor(0, IDC_ARROW)), _T(""), WS_POPUP, CXTPEmptyRect(), pParentWnd, 0);
	m_pGrid = pParentWnd;

}

void CXTPPropertyGridToolTip::Activate(BOOL bActive)
{
	if (bActive)
	{
		CString strText;
		GetWindowText(strText);
		CWindowDC dc(this);
		CXTPFontDC font(&dc, &m_pGrid->m_fontNormal);
		CXTPWindowRect rc (this);
		rc.right = rc.left + dc.GetTextExtent(strText).cx + 8;

		CRect rcWork = CXTPDrawHelpers::GetWorkArea(m_pGrid);
		if (rc.right > rcWork.right)
			rc.OffsetRect(rcWork.right - rc.right, 0);
		MoveWindow(rc);
	}
	ShowWindow(bActive? SW_SHOWNA: SW_HIDE);
}

BOOL CXTPPropertyGridToolTip::OnEraseBkgnd(CDC*)
{
	return TRUE;
}
void CXTPPropertyGridToolTip::OnPaint()
{
	CPaintDC dc(this);
	CXTPClientRect rc(this);

	COLORREF clrText = GetXtremeColor(COLOR_INFOTEXT);
	dc.FillSolidRect(rc, GetXtremeColor(COLOR_INFOBK));
	dc.Draw3dRect(rc, clrText, clrText);

	CString strText;
	GetWindowText(strText);
	dc.SetTextColor(clrText);
	dc.SetBkMode(TRANSPARENT);

	CXTPFontDC font(&dc, &m_pGrid->m_fontNormal);
	CRect rcText(rc);
	rcText.left += 4;

	dc.DrawText(strText, rcText,  DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);

}

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridView

CXTPPropertyGridView::CXTPPropertyGridView()
{
	m_properetySort = xtpGridSortCategorized;

	m_dDivider = .5;
	m_bTracking = FALSE;

	LOGFONT lfIcon;
	VERIFY( ::SystemParametersInfo( SPI_GETICONTITLELOGFONT, sizeof( lfIcon ), &lfIcon, 0 ) );
	lfIcon.lfWeight = FW_NORMAL;
	lfIcon.lfItalic = FALSE;
	VERIFY( m_fontNormal.CreateFontIndirect(&lfIcon  ) );

	lfIcon.lfWeight = FW_BOLD;
	VERIFY( m_fontBold.CreateFontIndirect(&lfIcon  ) );

	m_pSelected = NULL;
	m_pCategories = new CXTPPropertyGridItems();
	m_pCategories->m_pGrid = this;

	m_hCursor = AfxGetApp()->LoadCursor( XTP_IDC_HSPLITBAR);
	m_nLockUpdate = 0;
	m_pGrid = NULL;
}

CXTPPropertyGridView::~CXTPPropertyGridView()
{
	m_pCategories->Clear();
	m_pCategories->InternalRelease();
}

IMPLEMENT_DYNAMIC(CXTPPropertyGridView, CListBox)

BEGIN_MESSAGE_MAP(CXTPPropertyGridView, CListBox)
	//{{AFX_MSG_MAP(CXTPPropertyGridView)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelectionChanged)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()

	ON_WM_KEYUP()
	ON_WM_SYSKEYUP()

	ON_WM_CHAR()
	ON_WM_GETDLGCODE()

	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()

	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridView message handlers
void CXTPPropertyGridView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (GetScrollBarCtrl(SB_VERT) == pScrollBar)
	{
		CListBox::OnVScroll(nSBCode, nPos, pScrollBar);

		Invalidate(FALSE);

		CWnd* pWnd = GetWindow(GW_CHILD);
		while (pWnd)
		{
			pWnd->Invalidate(FALSE);
			pWnd = pWnd->GetWindow(GW_HWNDNEXT);
		}
	}
	else
	{
		CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
	}
}

BOOL CXTPPropertyGridView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	BOOL bResult = CWnd::OnMouseWheel(nFlags, zDelta, pt);

	Invalidate(FALSE);

	CWnd* pWnd = GetWindow(GW_CHILD);
	while (pWnd)
	{
		pWnd->Invalidate(FALSE);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	return bResult;
}


void CXTPPropertyGridView::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	m_pGrid->m_pPaintManager->MeasureItem(lpMeasureItemStruct);
}


void CXTPPropertyGridView::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);

	CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lpDrawItemStruct->itemData;

	if (pItem == NULL)
		return;

	if ((int)lpDrawItemStruct->itemID == GetCount() - 1)
	{
		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		CXTPClientRect rc(this);
		rc.top = lpDrawItemStruct->rcItem.bottom;
		pDC->FillSolidRect(rc, m_clrBack);
	}

	if (!((GetParent()->GetStyle() & XTP_PGS_OWNERDRAW) &&
		(GetParent()->GetOwner()->SendMessage(XTPWM_PROPERTYGRID_NOTIFY, XTP_PGN_DRAWITEM, (LPARAM)lpDrawItemStruct) == TRUE)))
	{
		m_pGrid->m_pPaintManager->DrawItem(lpDrawItemStruct);
	}
}

BOOL CXTPPropertyGridView::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CXTPPropertyGridView::OnPaint()
{
	CPaintDC dc(this);
	CXTPClientRect rc(this);
	CXTPBufferDC buffer(dc, rc);

	ASSERT(m_pGrid);
	m_pGrid->m_pPaintManager->FillPropertyGridView(&buffer, TRUE);

	CWnd::DefWindowProc(WM_PAINT, (WPARAM)buffer.m_hDC, 0);
}

void CXTPPropertyGridView::OnNcPaint()
{
	CWindowDC dc(this);

	CWnd::DefWindowProc(WM_PRINT, (WPARAM)dc.m_hDC, PRF_NONCLIENT);

	ASSERT(m_pGrid);
	m_pGrid->m_pPaintManager->FillPropertyGridView(&dc, FALSE);
}

CXTPPropertyGridItem* CXTPPropertyGridView::InsertCategory(int nIndex, CString strCaption, CXTPPropertyGridItem* pCategory)
{
	ASSERT(nIndex >= 0 && nIndex <= m_pCategories->GetCount());

	if (nIndex < 0 || nIndex > m_pCategories->GetCount())
		nIndex = m_pCategories->GetCount();

	if (pCategory == NULL)
		pCategory = new CXTPPropertyGridItem(strCaption);

	SetPropertySort(xtpGridSortCategorized);

	pCategory->m_pGrid = this;
	pCategory->m_bCategory = TRUE;
	pCategory->m_nFlags = 0;
	pCategory->m_pChilds->m_pGrid = this;

	m_pCategories->InsertAt(nIndex, pCategory);

	if (m_hWnd)
	{
		int nInsertAt = (nIndex >= m_pCategories->GetCount() - 1)? GetCount(): m_pCategories->GetAt(nIndex + 1)->m_nIndex;
		InsertItem(pCategory, nInsertAt);
	}
	_RefreshIndexes();


	return pCategory;

}

CXTPPropertyGridItem* CXTPPropertyGridView::AddCategory(CString strCaption, CXTPPropertyGridItem* pCategory)
{
	return InsertCategory(m_pCategories->GetCount(), strCaption, pCategory);
}

void CXTPPropertyGridView::Refresh()
{
	_RefreshIndexes();
	SetPropertySort(m_properetySort, TRUE);

	if (GetSafeHwnd())
		GetParent()->Invalidate(FALSE);
}

void CXTPPropertyGridView::ResetContent()
{
	if (m_pSelected)
	{
		m_pSelected->OnDeselect();
		m_pSelected = NULL;
	}

	if (m_hWnd)
	{
		for (int i = 0; i < GetCount(); i++)
		{
			CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)GetItemDataPtr(i);
			ASSERT(pItem);
			pItem->SetVisible(FALSE);

		}

		CListBox::ResetContent();

		SetFont(&m_fontNormal, FALSE);

		CWindowDC dc(this);
		CXTPFontDC font(&dc, &m_fontNormal);
		SetItemHeight(0, dc.GetTextExtent(_T(" ")).cy + 4);
	}
}

void CXTPPropertyGridView::SetPropertySort(XTPPropertyGridSortOrder sort, BOOL bRrefresh)
{
	if (m_nLockUpdate > 0)
	{
		m_properetySort = sort;
		return;
	}

	if (sort == m_properetySort && !bRrefresh)
		return;

	if (!m_hWnd)
		return;

	CXTPPropertyGridItem* pSelected = GetSelectedItem();
	ResetContent();

	if (sort == xtpGridSortCategorized)
	{
		for (int i = 0; i < m_pCategories->GetCount(); i++)
		{
			CXTPPropertyGridItem* pCategory = m_pCategories->GetAt(i);
			InsertItem(pCategory, GetCount());
		}
	}
	else if (sort == xtpGridSortAlphabetical || sort == xtpGridSortNoSort)
	{
		CXTPPropertyGridItems lstItems;
		int i;
		for (i = 0; i < m_pCategories->GetCount(); i++)
		{
			CXTPPropertyGridItem* pCategory = m_pCategories->GetAt(i);
			lstItems.AddTail(pCategory->m_pChilds);
		}

		if (sort != xtpGridSortNoSort)
			lstItems.Sort();

		for (i = 0; i < lstItems.GetCount(); i++)
		{
			CXTPPropertyGridItem* pItem = lstItems.GetAt(i);
			InsertItem(pItem, GetCount());
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	_RefreshIndexes();

	if (pSelected)
	{
		pSelected->Select();
	}
	OnSelectionChanged();

	if (sort != m_properetySort)
	{
		m_properetySort = sort;
		((CXTPPropertyGrid*)GetParent())->OnSortChanged();
	}
}



int CXTPPropertyGridView::GetDividerPos()
{
	CXTPWindowRect rcWindow(this);
	return int(m_dDivider * rcWindow.Width());
}

int  CXTPPropertyGridView::InsertItem(CXTPPropertyGridItem* pItem, int nIndex)
{
	if (m_nLockUpdate > 0) return 0;
	if (!m_hWnd) return 0;

	pItem->OnBeforeInsert();
	nIndex = (int)::SendMessage(m_hWnd, LB_INSERTSTRING, nIndex, (LPARAM)pItem);

	ASSERT(nIndex != -1);
	SetItemDataPtr(nIndex, pItem);

	if (GetStyle() & LBS_OWNERDRAWVARIABLE)
	{
		MEASUREITEMSTRUCT measureItemStruct = {ODT_LISTBOX, 0, nIndex, 0, 0, (ULONG_PTR)pItem};
		MeasureItem(&measureItemStruct);
		if (measureItemStruct.itemHeight != 0)
		{
			SetItemHeight(nIndex, measureItemStruct.itemHeight);
		}
	}

	pItem->SetVisible(TRUE);

	int nItemsInserted = 1;

	if (pItem->m_bExpanded)
	{
		nItemsInserted += _DoExpand(pItem, nIndex);
	}

	return nItemsInserted;
}

int CXTPPropertyGridView::_DoExpand(CXTPPropertyGridItem* pItem, int nIndex)
{
	int nStart = nIndex;

	for (int i = 0; i < pItem->GetChilds()->GetCount(); i++)
	{
		CXTPPropertyGridItem* pChild = pItem->GetChilds()->GetAt(i);

		nIndex += InsertItem(pChild, nIndex + 1);

	}
	return nIndex - nStart;
}

void CXTPPropertyGridView::_RefreshIndexes()
{
	if (m_hWnd)
	{
		for (int i = 0; i < GetCount(); i++)
		{
			CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)GetItemDataPtr(i);
			ASSERT(pItem);
			pItem->m_nIndex = i;
		}
	}
}

void CXTPPropertyGridView::_DoCollapse(CXTPPropertyGridItem* pItem)
{
	ASSERT(pItem);
	ASSERT(pItem->m_bExpanded);
	int nIndex = pItem->m_nIndex + 1;

	while(nIndex < GetCount())
	{
		CXTPPropertyGridItem* pChild = (CXTPPropertyGridItem*)GetItemDataPtr(nIndex);
		ASSERT(pChild);
		if (!pChild || !pChild->HasParent(pItem))
			break;

		pChild->SetVisible(FALSE);
		DeleteString(nIndex);
	}
	_RefreshIndexes();
}

CXTPPropertyGridItem* CXTPPropertyGridView::GetItem(int nIndex)
{
	if (nIndex < 0 || nIndex >= GetCount())
		return 0;

	CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)GetItemDataPtr(nIndex);
	
	if ((ULONG_PTR)pItem == (ULONG_PTR)(-1))
		return NULL;

	ASSERT(pItem);

	return pItem;
}

void CXTPPropertyGridView::SwitchExpandState(int nItem)
{
	CXTPPropertyGridItem* pItem = GetItem(nItem);
	if (!pItem) return;

	if(pItem->m_bExpanded) pItem->Collapse(); else pItem->Expand();
}

void CXTPPropertyGridView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (HitTest(point) == PGV_HIT_SPLITTER)
	{
		SetFocus();
		SetCapture();
		if (m_pSelected) m_pSelected->OnValidateEdit();

		m_bTracking = TRUE;
		return;
	}


	CXTPPropertyGridItem* pItem = ItemFromPoint(point);
	if (pItem)
	{
		SetFocus();

		if (GetFocus() != this)
			return;

		if ((ItemFromPoint(point) == pItem) && pItem->OnLButtonDown(nFlags, point))
			return;
	}

	CListBox::OnLButtonDown(nFlags, point);
}

CXTPPropertyGridItem* CXTPPropertyGridView::ItemFromPoint(CPoint point)
{
	BOOL bOutside = FALSE;
	int nIndex = CListBox::ItemFromPoint(point, bOutside);
	if (nIndex != -1 && !bOutside)
	{
		return GetItem(nIndex);
	}
	return NULL;
}

void CXTPPropertyGridView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bTracking)
	{
		CXTPWindowRect rcWindow(this);
		m_dDivider = (double)point.x / rcWindow.Width();
		m_dDivider = __max(m_dDivider, .1);
		m_dDivider = __min(m_dDivider, .85);

		if (m_pSelected) m_pSelected->OnSelect();
		Invalidate(FALSE);
	}
	else
	{

		ShowToolTip(point);
	}

	CListBox::OnMouseMove(nFlags, point);
}

void CXTPPropertyGridView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bTracking)
	{
		ReleaseCapture();
		m_bTracking = FALSE;
	}

	CListBox::OnLButtonUp(nFlags, point);
}

void CXTPPropertyGridView::OnCaptureChanged(CWnd *pWnd)
{
	m_bTracking = FALSE;

	CListBox::OnCaptureChanged(pWnd);
}

void CXTPPropertyGridView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CListBox::OnLButtonDblClk(nFlags, point);

	CXTPPropertyGridItem* pItem = ItemFromPoint(point);

	if (pItem)
	{
		pItem->OnLButtonDblClk();


		if (pItem == ItemFromPoint(point))
		{
			GetParent()->GetOwner()->SendMessage(XTPWM_PROPERTYGRID_NOTIFY, XTP_PGN_DBLCLICK, (LPARAM)pItem);
		}
	}
}

void CXTPPropertyGridView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CListBox::OnRButtonDown(nFlags, point);

	CXTPPropertyGridItem* pItem = ItemFromPoint(point);

	if (pItem)
	{
		pItem->OnRButtonDown();


		if (pItem == ItemFromPoint(point))
		{
			GetParent()->GetOwner()->SendMessage(XTPWM_PROPERTYGRID_NOTIFY, XTP_PGN_RCLICK, (LPARAM)pItem);
		}
	}
}

CXTPPropertyGridItem* CXTPPropertyGridView::GetSelectedItem()
{
	return m_hWnd? GetItem(GetCurSel()): 0;
}

void CXTPPropertyGridView::OnChar(UINT nChar, UINT nRepCntr, UINT nFlags)
{
	if (m_bTracking) return;

	CXTPPropertyGridItem* pItem = GetSelectedItem();

	if (nChar == '+' && pItem && pItem->HasChilds() && !pItem->m_bExpanded)
	{
		pItem->Expand();
		return;
	}
	if (nChar == '-' && pItem && pItem->HasChilds() && pItem->m_bExpanded)
	{
		pItem->Collapse();
		return;
	}
	if (nChar == VK_TAB)
	{
		m_pGrid->OnNavigate(xtpGridUIView, GetKeyState(VK_SHIFT) >= 0, pItem);
		return;
	}
	if (pItem && (nChar != VK_RETURN || !pItem->HasChilds()))
	{
		if (pItem->OnChar(nChar))
			return;
	}

	CWnd::OnChar(nChar, nRepCntr, nFlags);
}

UINT CXTPPropertyGridView::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}
void CXTPPropertyGridView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

#ifdef XTP_SITENOTIFY_KEY
	XTP_SITENOTIFY_KEY(this, this, FALSE, nChar)
#endif
	CListBox::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CXTPPropertyGridView::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

#ifdef XTP_SITENOTIFY_KEY
	XTP_SITENOTIFY_KEY(this, this, FALSE, nChar)
#endif
	CListBox::OnSysKeyUp(nChar, nRepCnt, nFlags);
}

void CXTPPropertyGridView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

#ifdef XTP_SITENOTIFY_KEY
	XTP_SITENOTIFY_KEY(this, this, TRUE, nChar)
#endif


	CXTPPropertyGridItem* pItem = GetSelectedItem();

	if (nChar == VK_RIGHT)
	{
		if (GetKeyState(VK_CONTROL) < 0)
		{
			CXTPWindowRect rcWindow(this);
			m_dDivider = m_dDivider + 2.0 / rcWindow.Width();
			m_dDivider = __max(m_dDivider, .1);
			m_dDivider = __min(m_dDivider, .85);

			if (m_pSelected) m_pSelected->OnSelect();
			Invalidate(FALSE);

			return;
		}
		if (pItem && pItem->HasChilds() && !pItem->m_bExpanded)
		{
			pItem->Expand();
			return;
		}
	}
	else if (nChar == VK_LEFT)
	{
		if (GetKeyState(VK_CONTROL) < 0)
		{
			CXTPWindowRect rcWindow(this);
			m_dDivider = m_dDivider - 2.0 / rcWindow.Width();
			m_dDivider = __max(m_dDivider, .1);
			m_dDivider = __min(m_dDivider, .85);

			if (m_pSelected) m_pSelected->OnSelect();
			Invalidate(FALSE);

			return;
		}
		if (pItem && pItem->HasChilds() && pItem->m_bExpanded)
		{
			pItem->Collapse();
			return;
		}
	}
	if (nChar == VK_RETURN )
	{
		SwitchExpandState(GetCurSel());
		return ;
	}
	if (nChar == VK_F4 )
	{
		if (m_pSelected && m_pSelected->GetFlags() & xtpGridItemHasComboButton)
			m_pSelected->OnInplaceButtonDown();
	}


	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPPropertyGridView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
#ifdef XTP_SITENOTIFY_KEY
	XTP_SITENOTIFY_KEY(this, this, TRUE, nChar)
#endif

	if (nChar == VK_DOWN || nChar == VK_UP)
	{
		if (m_pSelected && m_pSelected->GetFlags() & xtpGridItemHasComboButton)
			m_pSelected->OnInplaceButtonDown();
	}

	CListBox::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CXTPPropertyGridView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	static BOOL bRelay = FALSE;
	if (m_wndTip.GetSafeHwnd() && m_wndTip.IsWindowVisible() && !bRelay)
	{
		bRelay = TRUE;
		RelayToolTipEvent(message);
		bRelay = FALSE;
	}


	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}


void CXTPPropertyGridView::OnSelectionChanged()
{
	CXTPPropertyGridItem* pItem = GetSelectedItem();

	if (m_pSelected) m_pSelected->OnDeselect();
	if (pItem) pItem->OnSelect();

	m_pSelected = pItem;

	if (m_pSelected) ((CXTPPropertyGrid*)GetParent())->OnSelectionChanged(m_pSelected);
}

int CXTPPropertyGridView::HitTest(CPoint point)
{
	CXTPWindowRect rcWindow(this);

	int nWidth =  int(m_dDivider * rcWindow.Width());
	if (point.x > nWidth - 4 && point.x <= nWidth + 2 && GetCount() > 0)
		return PGV_HIT_SPLITTER;

	return -1;
}

BOOL CXTPPropertyGridView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest == HTCLIENT)
	{
		CPoint point;

		GetCursorPos(&point);
		ScreenToClient(&point);

		if (HitTest(point) == PGV_HIT_SPLITTER)
		{
			SetCursor( m_hCursor );
			return TRUE;
		}
	}

	return CListBox::OnSetCursor(pWnd, nHitTest, message);
}

void CXTPPropertyGridView::OnSize(UINT nType, int cx, int cy)
{
	CListBox::OnSize(nType, cx, cy);

	OnSelectionChanged();
}


// ToolTips routings

void CXTPPropertyGridView::_ShowToolTip(CRect rcBound, CRect rcText)
{
	m_wndTip.SetWindowText(m_strTipText);
	m_wndTip.MoveWindow(rcText);
	m_wndTip.Activate(TRUE);

	m_rcToolTip = rcBound;

	TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, 0};
	_TrackMouseEvent (&tme);
}

CSize CXTPPropertyGridView::_GetTextExtent(CString str)
{
	CWindowDC dc(this);
	CXTPFontDC font(&dc, GetFont());
	return dc.GetTextExtent(str);
}

void CXTPPropertyGridView::ShowToolTip(CPoint pt)
{
	if (!m_wndTip.GetSafeHwnd())
	{
		m_wndTip.Create(this);
	}

	CXTPPropertyGridItem* pItem = ItemFromPoint(pt);

	if (!pItem || pItem->IsCategory())
		return;

	CXTPWindowRect rcWindow(this);
	CRect rc = pItem->GetItemRect();

	CRect rcCaption(rc.left, rc.top, rc.left + int(m_dDivider * rcWindow.Width()), rc.bottom);

	if (rcCaption.PtInRect(pt))
	{
		if (m_strTipText == pItem->GetCaption())
			return;

		m_strTipText = pItem->GetCaption();

		ClientToScreen(&rcCaption);

		CRect rcCaptionText(rcCaption);
		rcCaptionText.left += pItem->m_nIndent * XTP_PGI_EXPAND_BORDER + 3 - 3 -1;
		rcCaptionText.top--;

		if (_GetTextExtent(m_strTipText).cx + 3 > rcCaptionText.Width())
		{
			if (GetExStyle() & WS_EX_LAYOUTRTL)
			{
				rcCaptionText.left = rcCaption.right - _GetTextExtent(m_strTipText).cx  - 7 - (pItem->m_nIndent * XTP_PGI_EXPAND_BORDER);

			}

			_ShowToolTip(rcCaption, rcCaptionText);
		}
	}
	else
	{
		if (m_strTipText == pItem->GetViewValue())
			return;

		m_strTipText = pItem->GetViewValue();

		if (GetFocus() && GetFocus()->IsKindOf(RUNTIME_CLASS(CXTPPropertyGridInplaceEdit)))
		{
			if (((CXTPPropertyGridInplaceEdit*)(GetFocus()))->GetItem() == pItem)
				return;
		}

		CRect rcValue(rc.left + int(m_dDivider * rcWindow.Width()), rc.top, rc.right, rc.bottom);
		ClientToScreen(&rcValue);

		CRect rcValueText = pItem->GetValueRect();
		rcValueText.InflateRect(1, 2, 0, 1);
		ClientToScreen(&rcValueText);

		if (_GetTextExtent(m_strTipText).cx + 3 > rcValueText.Width())
		{
			if (GetExStyle() & WS_EX_LAYOUTRTL)
			{
				rcValueText.left = rcValueText.right - 8 - _GetTextExtent(m_strTipText).cx;

			}
			_ShowToolTip(rcValue, rcValueText);
		}
	}
}


void CXTPPropertyGridView::RelayToolTipEvent(UINT message)
{
	if (m_wndTip.GetSafeHwnd() && m_wndTip.IsWindowVisible())
	{
		CRect rc;
		m_wndTip.GetWindowRect(rc);

		CPoint pt;
		GetCursorPos(&pt);

		if (!m_rcToolTip.PtInRect(pt))
		{
			m_strTipText = "";
			m_wndTip.Activate(FALSE);
		}

		switch (message)
		{
			case WM_MOUSEWHEEL:
				m_strTipText = "";
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			case WM_MOUSELEAVE:
				m_wndTip.Activate(FALSE);
		}
	}

}


void CXTPPropertyGridView::OnSetFocus(CWnd* pOldWnd)
{
	CListBox::OnSetFocus(pOldWnd);

#ifdef XTP_SITENOTIFY_ONFOCUS
	XTP_SITENOTIFY_ONFOCUS(this, this, TRUE)
#endif
}
void CXTPPropertyGridView::OnKillFocus (CWnd* pNewWnd)
{
	CListBox::OnKillFocus(pNewWnd);

#ifdef XTP_SITENOTIFY_ONFOCUS
	XTP_SITENOTIFY_ONFOCUS(this, this, FALSE)
#endif
}