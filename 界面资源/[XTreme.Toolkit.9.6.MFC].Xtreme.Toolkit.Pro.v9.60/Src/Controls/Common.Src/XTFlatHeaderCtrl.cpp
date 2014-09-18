// XTFlatHeaderCtrl.cpp : implementation of the CXTHeaderCtrl class.
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
#include "XTPWinThemeWrapper.h"
#include "XTPColorManager.h"
#include "XTPDrawHelpers.h"

#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTResource.h"
#include "XTVC50Helpers.h"
#include "XTMemDC.h"
#include "XTWndHook.h"
#include "XTHeaderTheme.h"
#include "XTFlatHeaderCtrl.h"
#include "XTFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_THEME_HOST(CXTHeaderCtrl, CXTHeaderTheme)

/////////////////////////////////////////////////////////////////////////////
// CXTHeaderCtrl

CXTHeaderCtrl::CXTHeaderCtrl()
: CXTThemeManagerStyleHost(GetThemeFactoryClass())
, m_iMinSize(0)
, m_nPos(0)
, m_iOverIndex(-1)
, m_nSortedCol(-1)
, m_bRTL(DetermineRTL())
, m_bAutoSize(false)
, m_bEnableMenus(TRUE)
, m_bAscending(TRUE)
, m_bLBtnDown(FALSE)
, m_bPainted(FALSE)
, m_popupMenuID(0)
, m_pt(CPoint(0,0))
{

}

CXTHeaderCtrl::~CXTHeaderCtrl()
{
}

BEGIN_MESSAGE_MAP(CXTHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CXTHeaderCtrl)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_SETCURSOR()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT_EX(HDN_ITEMCHANGING, OnItemchanging)
	ON_COMMAND(XT_IDC_SORTASC, OnSortAsc)
	ON_COMMAND(XT_IDC_SORTDSC, OnSortDsc)
	ON_COMMAND(XT_IDC_ALIGNLEFT, OnAlignLeft)
	ON_COMMAND(XT_IDC_ALIGNCENTER, OnAlignCenter)
	ON_COMMAND(XT_IDC_ALIGNRIGHT, OnAlignRight)
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CXTHeaderCtrl, CHeaderCtrl)

/////////////////////////////////////////////////////////////////////////////
// CXTHeaderCtrl message handlers

void CXTHeaderCtrl::OnThemeChanged()
{
	RecalcLayout();
}

BOOL CXTHeaderCtrl::IsThemeValid() const
{
	return (GetTheme() != NULL);
}

void CXTHeaderCtrl::ApplyFieldWidths(int iNewWidth)
{
	CListCtrl* pListCtrl = ( CListCtrl* )GetParent();
	ASSERT_VALID( pListCtrl );

	int iItemCount   = GetItemCount();
	int iFrozenCount = (int)m_arFrozenCols.GetCount();
	int iThawedCount = iItemCount - iFrozenCount;

	if (iThawedCount <= 0)
		return;

	int iWidth = iNewWidth/iThawedCount;

	int iItem;
	for (iItem = 0; iItem < iItemCount; iItem++)
	{
		if (IsColFrozen(iItem))
			continue;

		if (iWidth > m_iMinSize)
			pListCtrl->SetColumnWidth( iItem, iWidth );

		iNewWidth -= iWidth;
	}
}

void CXTHeaderCtrl::ResizeColumnsToFit()
{
	FitFieldWidths(0);
}

void CXTHeaderCtrl::FitFieldWidths(int iNewWidth)
{
	if (iNewWidth <= 0)
	{
		CXTPWindowRect rc(this);
		iNewWidth = rc.Width();
	}

	// adjust for vertical scroll bar.
	DWORD dwStyle = ::GetWindowLong(::GetParent(m_hWnd), GWL_STYLE);
	if ((dwStyle & WS_VSCROLL) == WS_VSCROLL)
		iNewWidth -= ::GetSystemMetrics(SM_CXVSCROLL);

	// adjust for frozen columns.
	iNewWidth -= GetFrozenColWidth();

	ApplyFieldWidths( iNewWidth );
}

void CXTHeaderCtrl::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
	if ( m_bAutoSize )
	{
		int iCount = GetItemCount();
		if (iCount > 0)
		{
			// is the window size changing?
			CXTPWindowRect rc(this);
			if (rc.Width() != lpwndpos->cx)
			{
				FitFieldWidths(lpwndpos->cx);
			}
		}
	}
	else
	{
		CHeaderCtrl::OnWindowPosChanging(lpwndpos);
	}
}

void CXTHeaderCtrl::EnableAutoSize(bool bEnable/*=true*/)
{
	m_bAutoSize = bEnable;
}

void CXTHeaderCtrl::FreezeColumn(int iCol)
{
	m_arFrozenCols.AddTail(iCol);
}

void CXTHeaderCtrl::ThawColumn(int iCol)
{
	for (POSITION pos = m_arFrozenCols.GetHeadPosition(); pos; m_arFrozenCols.GetNext(pos))
	{
		int iNext = m_arFrozenCols.GetAt(pos);
		if (iNext == iCol)
		{
			m_arFrozenCols.RemoveAt(pos);
			break;
		}
	}
}

void CXTHeaderCtrl::ThawAllColumns()
{
	m_arFrozenCols.RemoveAll();
}

BOOL CXTHeaderCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if ( m_arFrozenCols.GetCount() )
	{
		CPoint pt;
		::GetCursorPos(&pt);

		CPoint ptClient = pt;
		ScreenToClient(&ptClient);

		HDHITTESTINFO hti;
		::ZeroMemory(&hti, sizeof(hti));
		hti.pt.x = ptClient.x;
		hti.pt.y = ptClient.y;

		int nIndex = (int)::SendMessage(GetSafeHwnd(),
			HDM_HITTEST, 0L, (LPARAM)&hti);

		if ( nIndex > -1 )
		{
			// if we are over one of the frozen columns, we can stop
			if ( IsColFrozen( nIndex ) )
			{
				::SetCursor(afxData.hcurArrow);
				return TRUE;
			}

			else
			{
				// determine if the current index is adjacent to a frozen column index.
				// if columns are resized by dragging to the right, test for the frozen column on the left.
				// if columns are resized by dragging to the left, test for the frozen column on the right.

				int iAdjIndex = nIndex + (m_bRTL ? 1 : -1);
				if ((iAdjIndex > -1) && IsColFrozen( iAdjIndex ))
				{
					CRect r;
					Header_GetItemRect(m_hWnd, nIndex, &r);
					int nMidPoint = (r.left + (r.Width()/2));

					// if columns resize to the right and the point is the right half of the header item...
					if (!m_bRTL && (ptClient.x <= nMidPoint))
					{
						::SetCursor(afxData.hcurArrow);
						return TRUE;
					}

					// if columns resize to the left and the point is the left half of the header item...
					else if (m_bRTL && (ptClient.x >= nMidPoint))
					{
						::SetCursor(afxData.hcurArrow);
						return TRUE;
					}
				}
			}
		}
	}

	return CHeaderCtrl::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CXTHeaderCtrl::OnItemchanging(NMHDR* pNMHDR, LRESULT* pResult)
{
	HD_NOTIFY* pHDN = ( HD_NOTIFY* )pNMHDR;
	ASSERT( pHDN != NULL );

	LRESULT lResult = FALSE;

	// if sizing is disabled for this column, keep the user from resizing
	if (m_arFrozenCols.GetCount() && IsColFrozen(pHDN->iItem))
		lResult = TRUE;

	// if the column is smaller than the minimum size, keep the user from resizing
	if (pHDN->pitem->mask & HDI_WIDTH && pHDN->pitem->cxy < m_iMinSize)
		lResult = TRUE;

	*pResult = lResult;

	return BOOL(*pResult);
}

bool CXTHeaderCtrl::IsColFrozen(int iCol)
{
	for (POSITION pos = m_arFrozenCols.GetHeadPosition(); pos; m_arFrozenCols.GetNext(pos))
	{
		int iNext = m_arFrozenCols.GetAt(pos);
		if (iNext == iCol)
		{
			return true;
		}
	}

	return false;
}

bool CXTHeaderCtrl::DetermineRTL()
{
	CWindowDC dc(NULL);

	// determine if columns are resized by dragging them right (most languages) or
	// left (RTL languages like Arabic & Hebrew).

	UINT nAlign = dc.GetTextAlign();
	ASSERT(nAlign != GDI_ERROR);

	// will only be true for RTL languages, text is laid out right to left and
	// columns resize to the left
	if ((nAlign != GDI_ERROR) && (nAlign & TA_RTLREADING)) {
		return true;
	}

	return false;
}

int CXTHeaderCtrl::GetFrozenColWidth()
{
	int iFrozenWidth = 0;

	for (POSITION pos = m_arFrozenCols.GetHeadPosition(); pos; m_arFrozenCols.GetNext(pos))
	{
		int iCol = m_arFrozenCols.GetAt(pos);

		CRect r;
		Header_GetItemRect(m_hWnd, iCol, &r);
		iFrozenWidth += r.Width();
	}

	return iFrozenWidth;
}

void CXTHeaderCtrl::SetMinSize(int iMinSize)
{
	m_iMinSize = iMinSize;
}

void CXTHeaderCtrl::OnPaint()
{
	CPaintDC dc(this);

	if (IsThemeValid())
	{
		CXTPBufferDC memDC(dc.m_hDC, CXTPClientRect(this));
		GetTheme()->DrawHeader(&memDC, this);
	}
	else
	{
		CHeaderCtrl::DefWindowProc(WM_PAINT, (WPARAM)dc.m_hDC, 0);
	}
}

BOOL CXTHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

void CXTHeaderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLBtnDown = TRUE;
	CHeaderCtrl::OnLButtonDown(nFlags, point);
}

void CXTHeaderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bLBtnDown = FALSE;
	CHeaderCtrl::OnLButtonUp(nFlags, point);
}

void CXTHeaderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (IsThemeValid() && GetTheme()->UseWinXPThemes(this))
	{
		SetTimer(1, 10, NULL);
	}

	CHeaderCtrl::OnMouseMove(nFlags, point);
}

int CXTHeaderCtrl::HitTest(CPoint pt, UINT* pFlags/*=NULL*/) const
{
	HDHITTESTINFO hti;
	hti.pt.x = pt.x;
	hti.pt.y = pt.y;

	int iItem = (int)::SendMessage(m_hWnd, HDM_HITTEST, 0, (LPARAM)(&hti));

	if (pFlags != NULL)
		*pFlags = hti.flags;

	return iItem;
}

BOOL CXTHeaderCtrl::ItemPressed(int iItem)
{
	if (m_bLBtnDown)
	{
		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point);

		UINT uFlags;
		if (HitTest(point, &uFlags) == iItem)
			return ((uFlags & HHT_ONHEADER) == HHT_ONHEADER);
	}

	return FALSE;
}

void CXTHeaderCtrl::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		CPoint pt;
		::GetCursorPos(&pt);

		int iOverIndex = HitTest(pt);
		if (iOverIndex == -1)
		{
			KillTimer(1);

			if (m_bPainted == TRUE)
			{
				RedrawWindow();
			}

			m_bPainted = false;
			return;
		}

		if (iOverIndex != m_iOverIndex)
		{
			m_iOverIndex = iOverIndex;
			m_bPainted = false;
		}

		if (!m_bPainted)
		{
			RedrawWindow();
			m_bPainted = true;
		}
	}

	CHeaderCtrl::OnTimer(nIDEvent);
}

LRESULT CXTHeaderCtrl::OnLayout(WPARAM wParam, LPARAM lParam)
{
	if (IsThemeValid())
	{
		return (LRESULT)GetTheme()->Layout(
			(LPHDLAYOUT)lParam, this);
	}

	return CHeaderCtrl::DefWindowProc(HDM_LAYOUT, wParam, lParam);
}

int CXTHeaderCtrl::SetSortImage(int iSortCol, BOOL bSortAsc)
{
	ASSERT(iSortCol < GetItemCount());

	int nPrevCol = m_nSortedCol;
	m_nSortedCol   = iSortCol;
	m_bAscending   = bSortAsc;

	RedrawWindow();
	return nPrevCol;
}

int CXTHeaderCtrl::GetSortedCol(BOOL* pbSortAsc/*=NULL*/)
{
	if (pbSortAsc)
		*pbSortAsc = m_bAscending;

	return m_nSortedCol;
}

BOOL CXTHeaderCtrl::RecalcLayout()
{
	if (!::IsWindow(m_hWnd))
		return FALSE;

	HD_LAYOUT hdl;
	WINDOWPOS wp;
	CXTPClientRect rcClient(this);

	hdl.prc   = &rcClient;
	hdl.pwpos = &wp;

	if (Header_Layout(m_hWnd, &hdl))
	{
		// Set the size, position, and visibility of the header window.
		::SetWindowPos(m_hWnd, wp.hwndInsertAfter, wp.x, wp.y,
			wp.cx, wp.cy, wp.flags | SWP_SHOWWINDOW);

		CWnd* pWndParent = GetParent();
		if (pWndParent)
		{
			CXTPWindowRect rcWindow(pWndParent);

			int cx = rcWindow.Width();
			int cy = rcWindow.Height();

			pWndParent->SetWindowPos(NULL, 0,0,cx++,cy++, SWP_NOMOVE);
			pWndParent->SetWindowPos(NULL, 0,0,cx--,cy--, SWP_NOMOVE);

			pWndParent->RedrawWindow(0, 0,
				RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN|RDW_FRAME);

			return TRUE;
		}
	}

	return FALSE;
}

void CXTHeaderCtrl::OnDestroy()
{
	if (IsThemeValid())
		GetTheme()->CleanUp(this);

	CHeaderCtrl::OnDestroy();
}

void CXTHeaderCtrl::SetBitmap(int iCol, UINT uBitmapID, DWORD dwRemove/*=NULL*/)
{
	if (dwRemove)
	{
		HD_ITEM hdi;
		hdi.mask = HDI_FORMAT;
		GetItem(iCol, &hdi);
		hdi.fmt &= ~dwRemove;
		SetItem(iCol, &hdi);
	}

	SetBitmap(iCol, uBitmapID, FALSE, RGB(192,192,192));
}

void CXTHeaderCtrl::SetBitmap(int iCol, UINT uBitmapID, BOOL bRemove, COLORREF crMask)
{
	if (IsThemeValid())
	{
		GetTheme()->SetBitmap(iCol, uBitmapID, bRemove, crMask, this);
	}
	else
	{
		if (bRemove)
		{
			HD_ITEM hdi;

			hdi.mask = HDI_BITMAP | HDI_FORMAT;
			GetItem(iCol, &hdi);

			hdi.fmt &= ~HDF_BITMAP;

			if (hdi.hbm != NULL)
			{
				::DeleteObject(hdi.hbm);
				hdi.hbm = NULL;
			}

			SetItem(iCol, &hdi);
		}
		else
		{
			HBITMAP hBitmap = (HBITMAP)::LoadImage (AfxGetResourceHandle(),
				MAKEINTRESOURCE(uBitmapID), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);
			ASSERT(hBitmap != NULL);

			if (hBitmap != NULL)
			{
				HD_ITEM hdi;

				hdi.mask = HDI_FORMAT;
				GetItem(iCol, &hdi);

				hdi.mask |= HDI_BITMAP;
				hdi.fmt  |= HDF_BITMAP;
				hdi.hbm  = hBitmap;
				SetItem(iCol, &hdi);
			}
		}
	}
}

void CXTHeaderCtrl::OnSysColorChange()
{
	CHeaderCtrl::OnSysColorChange();

	if (IsThemeValid())
	{
		GetTheme()->RefreshMetrics();
	}
}

void CXTHeaderCtrl::InitializeHeader(BOOL bBoldFont)
{
	SetFont(bBoldFont ? &XTAuxData().fontBold : &XTAuxData().font);
	RedrawWindow();
}

void CXTHeaderCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	CPoint pt = m_pt = point;
	ClientToScreen(&pt);

	// If no sort headers are defined for the parent control or popup menus
	// has been disabled, call the base class and return.
	CWnd* pParentWnd = GetParent();
	if (pParentWnd->GetStyle() & LVS_NOSORTHEADER || m_bEnableMenus == FALSE)
	{
		CHeaderCtrl::OnRButtonDown(nFlags, point);
		return;
	}

	// No menu was defined use default
	if (!m_popupMenuID)
	{
		// Get the index to the header item under the cursor.
		int iIndex = HitTest(m_pt);

		if (iIndex != -1)
		{
			CMenu menu;
			VERIFY(XTPResourceManager()->LoadMenu(&menu, XT_IDM_POPUP));

			CMenu* pPopup = menu.GetSubMenu(2);
			ASSERT(pPopup != NULL);

			if (m_nSortedCol == iIndex && m_bAscending == TRUE)
				pPopup->CheckMenuItem(XT_IDC_SORTASC, MF_CHECKED | MF_BYCOMMAND);

			else if (m_nSortedCol == iIndex && m_bAscending == FALSE)
				pPopup->CheckMenuItem(XT_IDC_SORTDSC, MF_CHECKED | MF_BYCOMMAND);

			CWnd* pParentWnd = GetParent();
			if (pParentWnd && (
				pParentWnd->IsKindOf(RUNTIME_CLASS(CListCtrl)) ||
				pParentWnd->IsKindOf(RUNTIME_CLASS(CListView))))
			{
				LVCOLUMN lvc;
				lvc.mask = LVCF_FMT;

				ListView_GetColumn(pParentWnd->m_hWnd, iIndex, &lvc);

				switch (lvc.fmt & LVCFMT_JUSTIFYMASK)
				{
				case LVCFMT_LEFT:
					pPopup->CheckMenuItem(XT_IDC_ALIGNLEFT, MF_CHECKED | MF_BYCOMMAND);
					break;
				case LVCFMT_CENTER:
					pPopup->CheckMenuItem(XT_IDC_ALIGNCENTER, MF_CHECKED | MF_BYCOMMAND);
					break;
				case LVCFMT_RIGHT:
					pPopup->CheckMenuItem(XT_IDC_ALIGNRIGHT, MF_CHECKED | MF_BYCOMMAND);
					break;
				}
			}

			XTFuncContextMenu(pPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this, XT_IDR_TBAR_HDR);
		}
	}
	else
	{
		CMenu menu;
		VERIFY(menu.LoadMenu(m_popupMenuID));

		CMenu* pPopup = menu.GetSubMenu(m_nPos);
		ASSERT(pPopup != NULL);

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
			pt.x, pt.y, GetOwner());
	}
}

void CXTHeaderCtrl::SetMenuID(UINT popupMenuID, int nPos)
{
	m_popupMenuID  = popupMenuID;
	m_nPos         = nPos;
}

void CXTHeaderCtrl::SendNotify(int iIndex)
{
	CWnd* pParentWnd = GetParent();
	if (!pParentWnd || !::IsWindow(pParentWnd->m_hWnd))
		return;

	if (pParentWnd->IsKindOf(RUNTIME_CLASS(CListCtrl)) ||
		pParentWnd->IsKindOf(RUNTIME_CLASS(CListView)))
	{
		TCHAR  lpBuffer[256];

		HDITEM hdi;
		hdi.mask       = HDI_TEXT|HDI_BITMAP|HDI_FORMAT|HDI_IMAGE|HDI_LPARAM|HDI_ORDER|HDI_WIDTH;
		hdi.pszText    = lpBuffer;
		hdi.cchTextMax = 256;

		GetItem(iIndex, &hdi);

		NMHEADER nmh;
		nmh.hdr.hwndFrom = m_hWnd;
		nmh.hdr.idFrom   = GetDlgCtrlID();
		nmh.hdr.code     = HDN_ITEMCLICK;
		nmh.iItem        = iIndex;
		nmh.iButton      = 1;
		nmh.pitem        = &hdi;

		// send message to the parent's owner window.
		pParentWnd->SendMessage(WM_NOTIFY,
			(WPARAM)(int)nmh.hdr.idFrom, (LPARAM)(NMHEADER*)&nmh);

		// then forward to the descendants.
		pParentWnd->SendMessageToDescendants(WM_NOTIFY,
			(WPARAM)(int)nmh.hdr.idFrom, (LPARAM)(NMHEADER*)&nmh);
	}
}

void CXTHeaderCtrl::OnSortAsc()
{
	int iIndex = HitTest(m_pt);
	if (iIndex != -1)
	{
		if (m_nSortedCol != iIndex || m_bAscending == FALSE)
		{
			m_bAscending = TRUE;
			m_nSortedCol = iIndex;
			SendNotify(iIndex);
		}
	}
}

void CXTHeaderCtrl::OnSortDsc()
{
	int iIndex = HitTest(m_pt);
	if (iIndex != -1)
	{
		if (m_nSortedCol != iIndex || m_bAscending == TRUE)
		{
			m_bAscending = FALSE;
			m_nSortedCol = iIndex;
			SendNotify(iIndex);
		}
	}
}

BOOL CXTHeaderCtrl::SetAlingment(int nFlag)
{
	int iIndex = HitTest(m_pt);

	if (iIndex != -1)
	{
		CWnd* pParentWnd = GetParent();
		if (pParentWnd && (
			pParentWnd->IsKindOf(RUNTIME_CLASS(CListCtrl)) ||
			pParentWnd->IsKindOf(RUNTIME_CLASS(CListView))))
		{
			LVCOLUMN lvc;
			lvc.mask = LVCF_FMT;

			ListView_GetColumn(pParentWnd->m_hWnd, iIndex, &lvc);
			lvc.fmt &= ~LVCFMT_JUSTIFYMASK;
			lvc.fmt |= nFlag;
			ListView_SetColumn(pParentWnd->m_hWnd, iIndex, &lvc);
			ListView_SetWorkAreas(pParentWnd->m_hWnd, 0, NULL);
		}
	}

	return FALSE;
}

void CXTHeaderCtrl::OnAlignLeft()
{
	SetAlingment(LVCFMT_LEFT);
}

void CXTHeaderCtrl::OnAlignCenter()
{
	SetAlingment(LVCFMT_CENTER);
}

void CXTHeaderCtrl::OnAlignRight()
{
	SetAlingment(LVCFMT_RIGHT);
}

BOOL CXTHeaderCtrl::HasSortArrow()
{
	if (IsThemeValid())
		return GetTheme()->HasSortArrow();
	return FALSE;
}

void CXTHeaderCtrl::ShowSortArrow(BOOL bSortArrow)
{
	if (IsThemeValid())
	{
		DWORD dwStyle = GetTheme()->GetDrawStyle() & ~XTTHEME_SORTARROW;

		if (bSortArrow)
			dwStyle |= XTTHEME_SORTARROW;

		GetTheme()->SetDrawStyle(dwStyle, this);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTFlatHeaderCtrl

IMPLEMENT_DYNAMIC(CXTFlatHeaderCtrl, CXTHeaderCtrl)

CXTFlatHeaderCtrl::CXTFlatHeaderCtrl()
{
	SetTheme(new CXTHeaderThemeOfficeXP());
}

CXTFlatHeaderCtrl::~CXTFlatHeaderCtrl()
{

}
