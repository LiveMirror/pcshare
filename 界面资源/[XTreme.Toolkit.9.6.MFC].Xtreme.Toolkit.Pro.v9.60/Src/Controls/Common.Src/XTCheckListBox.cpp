// XTCheckListBox.cpp : implementation of the CXTCheckListBox class.
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

#include "StdAfx.h"
#include "XTPVC80Helpers.h"  // Visual Studio 2005 helper functions

#include "XTResource.h"
#include "XTDefines.h"
#include "XTUtil.h"
#include "XTGlobal.h"
#include "XTButton.h"
#include "XTEditListBox.h"
#include "XTCheckListBox.h"
#include "XTMemDC.h"
#include "XTPWinThemeWrapper.h"

#ifndef LAYOUT_BITMAPORIENTATIONPRESERVED
#define LAYOUT_BITMAPORIENTATIONPRESERVED  0x00000008
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTCheckListState

CXTCheckListBox::CCheckListState::CCheckListState(UINT uBitmapID)
{
	CBitmap bitmap;
	VERIFY(bitmap.LoadBitmap(uBitmapID));

	BITMAP bm;
	bitmap.GetObject(sizeof (BITMAP), &bm);
	m_sizeCheck.cx = bm.bmWidth / 3;
	m_sizeCheck.cy = bm.bmHeight;
	m_hbitmapCheck = (HBITMAP)bitmap.Detach();
}

CXTCheckListBox::CCheckListState::~CCheckListState()
{
	if (m_hbitmapCheck != NULL)
		::DeleteObject(m_hbitmapCheck);
}

/////////////////////////////////////////////////////////////////////////////
// CHECK_DATA

struct CXTCheckListBox::CHECK_DATA
{
public:
	int m_nCheck;
	BOOL m_bEnabled;
	DWORD_PTR m_dwUserData;

	CHECK_DATA()
	{
		m_nCheck = 0;
		m_bEnabled = TRUE;
		m_dwUserData = 0;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CXTCheckListBox

CXTCheckListBox::CXTCheckListBox(UINT uBitmapID/*=XT_IDB_CHECKLISTBOX*/)
: m_checkListState(uBitmapID)
, m_cyText(0)
, m_nStyle(0)
{

}

CXTCheckListBox::~CXTCheckListBox()
{

}

IMPLEMENT_DYNAMIC(CXTCheckListBox, CXTListBox)

BEGIN_MESSAGE_MAP(CXTCheckListBox, CXTListBox)
	//{{AFX_MSG_MAP(CXTCheckListBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_MESSAGE(LB_ADDSTRING, OnLBAddString)
	ON_MESSAGE(LB_FINDSTRING, OnLBFindString)
	ON_MESSAGE(LB_FINDSTRINGEXACT, OnLBFindStringExact)
	ON_MESSAGE(LB_GETITEMDATA, OnLBGetItemData)
	ON_MESSAGE(LB_GETTEXT, OnLBGetText)
	ON_MESSAGE(LB_INSERTSTRING, OnLBInsertString)
	ON_MESSAGE(LB_SELECTSTRING, OnLBSelectString)
	ON_MESSAGE(LB_SETITEMDATA, OnLBSetItemData)
	ON_MESSAGE(LB_SETITEMHEIGHT, OnLBSetItemHeight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTCheckListBox::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if (!(dwStyle & LBS_OWNERDRAWVARIABLE)) //must be one or the other
	{
		dwStyle |= LBS_OWNERDRAWFIXED;
	}

	return CXTListBox::Create(dwStyle, rect, pParentWnd, nID);
}

void CXTCheckListBox::SetCheckStyle(UINT nStyle)
{
	ASSERT(
		nStyle == 0               ||
		nStyle == BS_CHECKBOX     ||
		nStyle == BS_AUTOCHECKBOX ||
		nStyle == BS_AUTO3STATE   ||
		nStyle == BS_3STATE );

	m_nStyle = nStyle;
}

void CXTCheckListBox::SetCheck(int nIndex, int nCheck)
{
	ASSERT(::IsWindow(m_hWnd));

	if (nCheck == 2)
	{
		if (m_nStyle == BS_CHECKBOX || m_nStyle == BS_AUTOCHECKBOX)
			return;
	}

	LRESULT lResult = DefWindowProc(LB_GETITEMDATA, nIndex, 0);
	if (lResult != LB_ERR)
	{

		CHECK_DATA* pState = (CHECK_DATA*)lResult;

		if (pState == NULL)
			pState = new CHECK_DATA;

		pState->m_nCheck = nCheck;
		VERIFY(DefWindowProc(LB_SETITEMDATA, nIndex, (LPARAM)pState) != LB_ERR);

		InvalidateCheck(nIndex);
	}
}

int CXTCheckListBox::GetCheck(int nIndex)
{
	ASSERT(::IsWindow(m_hWnd));

	LRESULT lResult = DefWindowProc(LB_GETITEMDATA, nIndex, 0);
	if (lResult != LB_ERR)
	{
		CHECK_DATA* pState = (CHECK_DATA*)lResult;
		if (pState != NULL)
			return pState->m_nCheck;
	}
	return 0; // The default
}

void CXTCheckListBox::Enable(int nIndex, BOOL bEnabled)
{
	ASSERT(::IsWindow(m_hWnd));

	LRESULT lResult = DefWindowProc(LB_GETITEMDATA, nIndex, 0);
	if (lResult != LB_ERR)
	{
		CHECK_DATA* pState = (CHECK_DATA*)lResult;

		if (pState == NULL)
			pState = new CHECK_DATA;

		pState->m_bEnabled = bEnabled;
		VERIFY(DefWindowProc(LB_SETITEMDATA, nIndex, (LPARAM)pState) != LB_ERR);

		InvalidateItem(nIndex);
	}
}

int CXTCheckListBox::IsEnabled(int nIndex)
{
	ASSERT(::IsWindow(m_hWnd));

	LRESULT lResult = DefWindowProc(LB_GETITEMDATA, nIndex, 0);
	if (lResult != LB_ERR)
	{
		CHECK_DATA* pState = (CHECK_DATA*)lResult;
		if (pState != NULL)
			return pState->m_bEnabled;
	}
	return TRUE; // The default
}

CRect CXTCheckListBox::OnGetCheckPosition(CRect, CRect rectCheckBox)
{
	return rectCheckBox;
}

void CXTCheckListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	// Must be LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS
	ASSERT((GetStyle() & (LBS_OWNERDRAWVARIABLE|LBS_HASSTRINGS)) ==
		(LBS_OWNERDRAWVARIABLE|LBS_HASSTRINGS));

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect rcItem(lpDIS->rcItem);
	rcItem.left+=2;

	if (((LONG)(lpDIS->itemID) >= 0) &&
		(lpDIS->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)))
	{
		int cyItem = GetItemHeight(lpDIS->itemID);
		BOOL fDisabled = !IsWindowEnabled() || !IsEnabled(lpDIS->itemID);

		COLORREF newTextColor = fDisabled ?
			RGB(0x80, 0x80, 0x80) : GetSysColor(COLOR_WINDOWTEXT);  // light gray
		COLORREF oldTextColor = pDC->SetTextColor(newTextColor);

		COLORREF newBkColor = GetSysColor(COLOR_WINDOW);
		COLORREF oldBkColor = pDC->SetBkColor(newBkColor);

		if (newTextColor == newBkColor)
			newTextColor = RGB(0xC0, 0xC0, 0xC0);   // dark gray

		if (!fDisabled && ((lpDIS->itemState & ODS_SELECTED) != 0))
		{
			pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
			pDC->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
		}

		if (m_cyText == 0)
			VERIFY(cyItem >= CalcMinimumItemHeight());

		CString strText;
		GetText(lpDIS->itemID, strText);

		pDC->ExtTextOut(rcItem.left+2, rcItem.top,
			ETO_OPAQUE, &rcItem, strText, strText.GetLength(), NULL);

		pDC->SetTextColor(oldTextColor);
		pDC->SetBkColor(oldBkColor);
	}

	if ((lpDIS->itemAction & ODA_FOCUS) != 0)
		pDC->DrawFocusRect(&rcItem);
}



int CXTCheckListBox::GetFontHeight()
{
	CWindowDC dc(NULL); // get the screen device context

	// select the control's font into the window dc.
	CFont* pOldFont = dc.SelectObject(GetFont());

	// get the text metrics for the device context that
	// is using the control's font, this will give us the
	// height in pixels that's what we will use for the
	// list item's height.
	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);

	// restore the dc with it's original font.
	dc.SelectObject(pOldFont);

	// return the desired row height.
	return tm.tmHeight;
}

void CXTCheckListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	lpMIS->itemHeight = GetFontHeight();
}


bool CXTCheckListBox::PreDrawItemThemed(CDC* pDC, DRAWITEMSTRUCT &drawItem, int nCheck, int cyItem)
{
	bool bRet = false;
	CXTPWinThemeWrapper themeHelper;

	themeHelper.OpenThemeData(m_hWnd, L"BUTTON");

	// Draw the check boxes using the theme API's only if the app is themed.
	if (themeHelper.IsAppThemeReady())
	{

		int nState = (nCheck == 1? CBS_CHECKEDNORMAL: nCheck == 2? CBS_UNCHECKEDDISABLED: CBS_UNCHECKEDNORMAL);

		SIZE size;

		if (SUCCEEDED(themeHelper.GetThemePartSize(pDC->m_hDC, BP_CHECKBOX, nState, NULL, TS_TRUE, &size)))
		{
			CRect rectCheck = drawItem.rcItem;
			rectCheck.left += 1;
			rectCheck.top += 1 + max(0, (cyItem - size.cy) / 2);
			rectCheck.right = rectCheck.left + size.cx;
			rectCheck.bottom = rectCheck.top + size.cy;

			CRect rectItem = drawItem.rcItem;
			rectItem.right = rectItem.left + size.cx + 2;

			CRect rectCheckBox = OnGetCheckPosition(rectItem, rectCheck);

			if (SUCCEEDED(themeHelper.DrawThemeBackground(pDC->m_hDC, BP_CHECKBOX, nState, &rectCheckBox, NULL)))
			{
				bRet = true;
			}
		}
	}
	return bRet;
}

void CXTCheckListBox::PreDrawItemNonThemed(CDC* pDC, DRAWITEMSTRUCT &drawItem, int nCheck, int cyItem)
{
	CDC bitmapDC;
	if (bitmapDC.CreateCompatibleDC(pDC))
	{

#if _MSC_VER >= 1200 // MFC 6.0
		// Change Compatible DC to LTR since the bitmap is LTR
		DWORD dwLayoutBitmapDC = bitmapDC.GetLayout();
		bitmapDC.SetLayout(0);
#endif

		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(bitmapDC.m_hDC, m_checkListState.m_hbitmapCheck);

		CRect rectCheck = drawItem.rcItem;
		rectCheck.left += 1;
		rectCheck.top += 1 + max(0, (cyItem - m_checkListState.m_sizeCheck.cy) / 2);
		rectCheck.right = rectCheck.left + m_checkListState.m_sizeCheck.cx;
		rectCheck.bottom = rectCheck.top + m_checkListState.m_sizeCheck.cy;

		CRect rectItem = drawItem.rcItem;
		rectItem.right = rectItem.left + m_checkListState.m_sizeCheck.cx + 2;
		CRect rectCheckBox = OnGetCheckPosition(rectItem, rectCheck);

		pDC->FillSolidRect(rectItem, pDC->GetBkColor());

#if _MSC_VER >= 1200 // MFC 6.0
		DWORD dwLayoutDC = pDC->GetLayout();

		// Change destination DC layout to preserve bitmap orientation
		pDC->SetLayout(dwLayoutDC | LAYOUT_BITMAPORIENTATIONPRESERVED);
#endif

		pDC->BitBlt(rectCheckBox.left, rectCheckBox.top,
			m_checkListState.m_sizeCheck.cx, m_checkListState.m_sizeCheck.cy, &bitmapDC,
			m_checkListState.m_sizeCheck.cx  * nCheck, 0, SRCCOPY);

#if _MSC_VER >= 1200 // MFC 6.0
		// Restore DC layout
		pDC->SetLayout(dwLayoutDC);

		bitmapDC.SetLayout(dwLayoutBitmapDC);
#endif
		::SelectObject(bitmapDC.m_hDC, hOldBitmap);
	}
}


void CXTCheckListBox::PreDrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	DRAWITEMSTRUCT drawItem;
	MEMCPY_S(&drawItem, lpDrawItemStruct, sizeof(DRAWITEMSTRUCT));

	if ((((LONG)drawItem.itemID) >= 0) &&
		((drawItem.itemAction & (ODA_DRAWENTIRE | ODA_SELECT)) != 0))
	{
		int cyItem = GetItemHeight(drawItem.itemID);

		CDC* pDC = CDC::FromHandle(drawItem.hDC);

		COLORREF newBkColor = GetSysColor(COLOR_WINDOW);

		BOOL fDisabled = !IsWindowEnabled() || !IsEnabled(drawItem.itemID);
		if ((drawItem.itemState & ODS_SELECTED) && !fDisabled)
			newBkColor = GetSysColor(COLOR_HIGHLIGHT);

		COLORREF oldBkColor = pDC->SetBkColor(newBkColor);

		int nCheck = GetCheck(drawItem.itemID);
		bool bThemed = false;

		if (XTAuxData().iComCtlVersion >= 6)
		{
			bThemed = PreDrawItemThemed(pDC, drawItem, nCheck, cyItem);
		}

		if (!bThemed)
		{
			PreDrawItemNonThemed(pDC, drawItem, nCheck, cyItem);
		}

		pDC->SetBkColor(oldBkColor);
	}

	if (drawItem.itemData != 0 && drawItem.itemData != (UINT)LB_ERR)
	{
		CHECK_DATA* pState = (CHECK_DATA*)drawItem.itemData;
		drawItem.itemData = pState->m_dwUserData;
	}
	drawItem.rcItem.left = drawItem.rcItem.left + m_checkListState.m_sizeCheck.cx + 2;

	DrawItem(&drawItem);
}

/*
void CXTCheckListBox::PreDrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	DRAWITEMSTRUCT drawItem;

	MEMCPY_S(&drawItem, lpDIS, sizeof(DRAWITEMSTRUCT));

	if ((((LONG)drawItem.itemID) >= 0) &&
		((drawItem.itemAction & (ODA_DRAWENTIRE | ODA_SELECT)) != 0))
	{
		int cyItem = GetItemHeight(drawItem.itemID);

		CDC* pDC = CDC::FromHandle(drawItem.hDC);

		COLORREF newBkColor = GetSysColor(COLOR_WINDOW);
		COLORREF oldBkColor = pDC->SetBkColor(newBkColor);

		CDC bitmapDC;
		if (bitmapDC.CreateCompatibleDC(pDC))
		{
			int nCheck = GetCheck(drawItem.itemID);
			HBITMAP hOldBitmap = (HBITMAP)::SelectObject(bitmapDC.m_hDC, m_checkListState.m_hbitmapCheck);

			CRect rectCheck = drawItem.rcItem;
			rectCheck.left += 1;
			rectCheck.top += 1 + __max(0, (cyItem - m_checkListState.m_sizeCheck.cy) / 2);
			rectCheck.right = rectCheck.left + m_checkListState.m_sizeCheck.cx;
			rectCheck.bottom = rectCheck.top + m_checkListState.m_sizeCheck.cy;

			CRect rectItem = drawItem.rcItem;
			rectItem.right = rectItem.left + m_checkListState.m_sizeCheck.cx + 2;

			CRect rectCheckBox = OnGetCheckPosition(rectItem, rectCheck);

			CBrush brush(newBkColor);
			pDC->FillRect(rectItem, &brush);

			pDC->BitBlt(rectCheckBox.left, rectCheckBox.top,
				m_checkListState.m_sizeCheck.cx, m_checkListState.m_sizeCheck.cy, &bitmapDC,
				m_checkListState.m_sizeCheck.cx  * nCheck, 0, SRCCOPY);

			::SelectObject(bitmapDC.m_hDC, hOldBitmap);
			brush.DeleteObject();
		}
		pDC->SetBkColor(oldBkColor);
		bitmapDC.DeleteDC();
	}

	if (drawItem.itemData != 0 && drawItem.itemData != (UINT)LB_ERR)
	{
		CHECK_DATA* pState = (CHECK_DATA*)drawItem.itemData;
		drawItem.itemData = pState->m_dwUserData;
	}
	drawItem.rcItem.left = drawItem.rcItem.left + m_checkListState.m_sizeCheck.cx + 2;

	DrawItem(&drawItem);
}*/

void CXTCheckListBox::PreMeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	int cyItem = CalcMinimumItemHeight();

	MEASUREITEMSTRUCT measureItem;

	MEMCPY_S(&measureItem, lpMeasureItemStruct, sizeof(MEASUREITEMSTRUCT));

	measureItem.itemHeight = cyItem;
	measureItem.itemWidth  = (UINT)-1;

	// WINBUG: Windows95 and Windows NT disagree on what this value
	// should be.  According to the docs, they are both wrong
	if (GetStyle() & LBS_OWNERDRAWVARIABLE)
	{
		LRESULT lResult = DefWindowProc(LB_GETITEMDATA, measureItem.itemID, 0);
		if (lResult != LB_ERR)
			measureItem.itemData = (UINT)lResult;
		else
			measureItem.itemData = 0;

		// WINBUG: This is only done in the LBS_OWNERDRAWVARIABLE case
		// because Windows 95 does not initialize itemData to zero in the
		// case of LBS_OWNERDRAWFIXED list boxes (it is stack garbage).
		if (measureItem.itemData != 0 && measureItem.itemData != (UINT)LB_ERR)
		{
			CHECK_DATA* pState = (CHECK_DATA*)measureItem.itemData;
			measureItem.itemData = pState->m_dwUserData;
		}
	}

	MeasureItem(&measureItem);

	lpMeasureItemStruct->itemHeight = max(measureItem.itemHeight,(UINT) cyItem);
	lpMeasureItemStruct->itemWidth = measureItem.itemWidth;
}

int CXTCheckListBox::PreCompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
	COMPAREITEMSTRUCT compareItem;

	MEMCPY_S(&compareItem, lpCompareItemStruct, sizeof(COMPAREITEMSTRUCT));

	if (compareItem.itemData1 != 0 && compareItem.itemData1 != (UINT)LB_ERR)
	{
		CHECK_DATA* pState = (CHECK_DATA*)compareItem.itemData1;
		compareItem.itemData1 = pState->m_dwUserData;
	}
	if (compareItem.itemData2 != 0 && compareItem.itemData2 != (UINT)LB_ERR)
	{
		CHECK_DATA* pState = (CHECK_DATA*)compareItem.itemData2;
		compareItem.itemData2 = pState->m_dwUserData;
	}
	return CompareItem(&compareItem);
}

void CXTCheckListBox::PreDeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct)
{
	DELETEITEMSTRUCT deleteItem;

	MEMCPY_S(&deleteItem, lpDeleteItemStruct, sizeof(DELETEITEMSTRUCT));

	// WINBUG: The following if block is required because Windows NT
	// version 3.51 does not properly fill out the LPDELETEITEMSTRUCT.
	if (deleteItem.itemData == 0)
	{
		LRESULT lResult = DefWindowProc(LB_GETITEMDATA, deleteItem.itemID, 0);
		if (lResult != LB_ERR)
			deleteItem.itemData = (UINT)lResult;
	}

	if (deleteItem.itemData != 0 && deleteItem.itemData != (UINT)LB_ERR)
	{
		CHECK_DATA* pState = (CHECK_DATA*)deleteItem.itemData;
		deleteItem.itemData = pState->m_dwUserData;
		delete pState;
	}
	DeleteItem(&deleteItem);
}

BOOL CXTCheckListBox::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	switch (message)
	{
	case WM_DRAWITEM:
		ASSERT(pResult == NULL);       // no return value expected
		PreDrawItem((LPDRAWITEMSTRUCT)lParam);
		break;
	case WM_MEASUREITEM:
		ASSERT(pResult == NULL);       // no return value expected
		PreMeasureItem((LPMEASUREITEMSTRUCT)lParam);
		break;
	case WM_COMPAREITEM:
		ASSERT(pResult != NULL);       // return value expected
		*pResult = PreCompareItem((LPCOMPAREITEMSTRUCT)lParam);
		break;
	case WM_DELETEITEM:
		ASSERT(pResult == NULL);       // no return value expected
		PreDeleteItem((LPDELETEITEMSTRUCT)lParam);
		break;
	default:
		return CXTListBox::OnChildNotify(message, wParam, lParam, pResult);
	}
	return TRUE;
}

void CXTCheckListBox::PreSubclassWindow()
{
	CXTListBox::PreSubclassWindow();
#ifdef _DEBUG
	// CCheckListBoxes must be owner drawn
	ASSERT(GetStyle() & (LBS_OWNERDRAWFIXED | LBS_OWNERDRAWVARIABLE));
#endif
}

int CXTCheckListBox::CalcMinimumItemHeight()
{
	int nResult;

	if ((GetStyle() & (LBS_HASSTRINGS | LBS_OWNERDRAWFIXED)) ==
		(LBS_HASSTRINGS | LBS_OWNERDRAWFIXED))
	{
		CClientDC dc(this);
		CFont* pOldFont = dc.SelectObject(GetFont());
		TEXTMETRIC tm;
		VERIFY (dc.GetTextMetrics ( &tm ));
		dc.SelectObject(pOldFont);

		m_cyText = tm.tmHeight;
		nResult = max(m_checkListState.m_sizeCheck.cy + 1, m_cyText);
	}
	else
	{
		nResult = m_checkListState.m_sizeCheck.cy + 1;
	}

	return nResult;
}

void CXTCheckListBox::InvalidateCheck(int nIndex)
{
	CRect rect;

	GetItemRect(nIndex, rect);
	rect.right = rect.left + m_checkListState.m_sizeCheck.cx + 2;
	InvalidateRect(rect, FALSE);
}

void CXTCheckListBox::InvalidateItem(int nIndex)
{
	CRect rect;
	GetItemRect(nIndex, rect);
	InvalidateRect(rect, FALSE);
}

int CXTCheckListBox::CheckFromPoint(CPoint point, BOOL& bInCheck)
{
	// assume did not hit anything
	bInCheck = FALSE;
	int nIndex = -1;

	if ((GetStyle() & (LBS_OWNERDRAWFIXED|LBS_MULTICOLUMN)) == LBS_OWNERDRAWFIXED)
	{
		// optimized case for ownerdraw fixed, single column
		int cyItem = GetItemHeight(0);
		if (point.y < cyItem * GetCount())
		{
			nIndex = GetTopIndex() + point.y / cyItem;
			if (point.x < m_checkListState.m_sizeCheck.cx + 2)
				++bInCheck;
		}
	}
	else
	{
		// general case for ownerdraw variable or multiple column
		int i;
		for (i = GetTopIndex(); i < GetCount(); i++)
		{
			CRect itemRect;
			GetItemRect(i, &itemRect);
			if (itemRect.PtInRect(point))
			{
				nIndex = i;
				if (point.x < itemRect.left + m_checkListState.m_sizeCheck.cx + 2)
					++bInCheck;
				break;
			}
		}
	}
	return nIndex;
}

void CXTCheckListBox::SetSelectionCheck( int nCheck )
{
	int* piSelectedItems;
	int nSelectedItems;
	int iSelectedItem;

	nSelectedItems = GetSelCount();
	if( nSelectedItems > 0 )
	{
		piSelectedItems = (int*)_alloca( nSelectedItems*sizeof( int ) );
		GetSelItems( nSelectedItems, piSelectedItems );
		for( iSelectedItem = 0; iSelectedItem < nSelectedItems; iSelectedItem++ )
		{
			if( IsEnabled( piSelectedItems[iSelectedItem] ) )
			{
				SetCheck( piSelectedItems[iSelectedItem], nCheck );
				InvalidateCheck( piSelectedItems[iSelectedItem] );
			}
		}
	}
}

void CXTCheckListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect itemRect;
	CRect clientRect;

	GetClientRect(clientRect);

	int nIndex;
	for (nIndex = GetTopIndex(); nIndex < GetCount(); nIndex++)
	{
		GetItemRect(nIndex, &itemRect);
		if (!clientRect.PtInRect(itemRect.TopLeft()))
			break;
		if (itemRect.PtInRect(point) && IsEnabled(nIndex))
		{
			if (m_nStyle != BS_CHECKBOX && m_nStyle != BS_3STATE)
			{
				if (point.x - itemRect.left < m_checkListState.m_sizeCheck.cx + 2)
				{
					CWnd* pParent = GetParent();
					ASSERT_VALID(pParent);
					int nModulo = (m_nStyle == BS_AUTO3STATE) ? 3 : 2;
					int nCheck = GetCheck(nIndex);
					nCheck = (nCheck == nModulo) ? nCheck - 1 : nCheck;
					SetCheck(nIndex, (nCheck + 1) % nModulo);
					InvalidateCheck(nIndex);
					CXTListBox::OnLButtonDown(nFlags, point);

					// Inform of check
					pParent->SendMessage(WM_COMMAND,
						MAKEWPARAM(GetDlgCtrlID(), CLBN_CHKCHANGE),
						(LPARAM)m_hWnd);
					return;

				}
			}
			else
				return; // Swallow LButtons for disabled items
		}
	}

	// do default listbox selection logic
	CXTListBox::OnLButtonDown( nFlags, point );
}

void CXTCheckListBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CRect itemRect;
	CRect clientRect;

	GetClientRect(clientRect);

	int nIndex;
	for (nIndex = GetTopIndex(); nIndex < GetCount(); nIndex++)
	{
		GetItemRect(nIndex, &itemRect);
		if (!clientRect.PtInRect(itemRect.TopLeft()))
			break;
		if (itemRect.PtInRect(point) && IsEnabled(nIndex))
		{
			if (m_nStyle != BS_CHECKBOX && m_nStyle != BS_3STATE)
			{
				CWnd* pParent = GetParent();
				ASSERT_VALID(pParent);
				int nModulo = (m_nStyle == BS_AUTO3STATE) ? 3 : 2;
				int nCheck = GetCheck(nIndex);
				nCheck = (nCheck == nModulo) ? nCheck - 1 : nCheck;
				SetCheck(nIndex, (nCheck + 1) % nModulo);
				InvalidateCheck(nIndex);
				CXTListBox::OnLButtonDown(nFlags, point);

				// Inform of check
				pParent->SendMessage(WM_COMMAND,
					MAKEWPARAM(GetDlgCtrlID(), CLBN_CHKCHANGE),
					(LPARAM)m_hWnd);
				return;

			}
			else
				return; // Swallow LButtons for disabled items
		}
	}

	CXTListBox::OnLButtonDblClk(nFlags, point);
}

void CXTCheckListBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SPACE)
	{
		int nIndex = GetCaretIndex();
		CWnd* pParent = GetParent();
		ASSERT_VALID(pParent);

		if (nIndex != LB_ERR)
		{
			if (m_nStyle != BS_CHECKBOX && m_nStyle != BS_3STATE)
			{
				if ((GetStyle() & LBS_MULTIPLESEL) != 0)
				{
					if (IsEnabled(nIndex))
					{
						BOOL bSelected = GetSel(nIndex);
						if (bSelected)
						{
							int nModulo = (m_nStyle == BS_AUTO3STATE) ? 3 : 2;
							int nCheck = GetCheck(nIndex);
							nCheck = (nCheck == nModulo) ? nCheck - 1 : nCheck;
							SetCheck(nIndex, (nCheck + 1) % nModulo);

							// Inform of check
							pParent->SendMessage(WM_COMMAND,
								MAKEWPARAM(GetDlgCtrlID(), CLBN_CHKCHANGE),
								(LPARAM)m_hWnd);
						}
						SetSel(nIndex, !bSelected);
					}
					else
						SetSel(nIndex, FALSE); // unselect disabled items

					return;
				}
				else
				{
					// If there is a selection, the space bar toggles that check,
					// all other keys are the same as a standard listbox.

					if (IsEnabled(nIndex))
					{
						int nModulo = (m_nStyle == BS_AUTO3STATE) ? 3 : 2;
						int nCheck = GetCheck(nIndex);
						nCheck = (nCheck == nModulo) ? nCheck - 1 : nCheck;
						int nNewCheck = (nCheck+1)%nModulo;
						SetCheck(nIndex, nNewCheck);

						InvalidateCheck(nIndex);

						if( GetStyle()&LBS_EXTENDEDSEL )
						{
							// The listbox is a multi-select listbox, and the user
							// clicked on a selected check, so change the check on all
							// of the selected items.
							SetSelectionCheck( nNewCheck );
						}

						// Inform of check
						pParent->SendMessage(WM_COMMAND,
							MAKEWPARAM(GetDlgCtrlID(), CLBN_CHKCHANGE),
							(LPARAM)m_hWnd);
					}
					else
						SetSel(nIndex, FALSE); // unselect disabled items

					return;
				}
			}
		}
	}
	CXTListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

int CXTCheckListBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTListBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	if ((GetStyle() & (LBS_OWNERDRAWFIXED | LBS_HASSTRINGS))
		== (LBS_OWNERDRAWFIXED | LBS_HASSTRINGS))
		SetItemHeight(0, CalcMinimumItemHeight());

	return 0;
}

LRESULT CXTCheckListBox::OnSetFont(WPARAM , LPARAM)
{
	Default();

	if ((GetStyle() & (LBS_OWNERDRAWFIXED | LBS_HASSTRINGS))
		== (LBS_OWNERDRAWFIXED | LBS_HASSTRINGS))
		SetItemHeight(0, CalcMinimumItemHeight());

	return 0;
}

LRESULT CXTCheckListBox::OnLBAddString(WPARAM wParam, LPARAM lParam)
{
	CHECK_DATA* pState = NULL;

	if (!(GetStyle() & LBS_HASSTRINGS))
	{
		pState = new CHECK_DATA;

		pState->m_dwUserData = lParam;
		lParam = (LPARAM)pState;
	}

	LRESULT lResult = DefWindowProc(LB_ADDSTRING, wParam, lParam);

	if (lResult == LB_ERR && pState != NULL)
		delete pState;

	return lResult;
}

LRESULT CXTCheckListBox::OnLBFindString(WPARAM wParam, LPARAM lParam)
{
	if (GetStyle() & LBS_HASSTRINGS)
		return DefWindowProc(LB_FINDSTRING, wParam, lParam);

	int nIndex = (int)wParam;
	if (nIndex == -1) nIndex = 0;

	for(; nIndex < GetCount(); nIndex++)
		if ((UINT_PTR)lParam == GetItemData(nIndex))
			return nIndex;

		return LB_ERR;
}

LRESULT CXTCheckListBox::OnLBFindStringExact(WPARAM wParam, LPARAM lParam)
{
	if (GetStyle() & (LBS_HASSTRINGS | LBS_SORT))
		return DefWindowProc(LB_FINDSTRINGEXACT, wParam, lParam);

	int nIndex = (int)wParam;
	if (nIndex == -1) nIndex = 0;

	for(; nIndex < GetCount(); nIndex++)
		if ((UINT_PTR)lParam == GetItemData(nIndex))
			return nIndex;

		return LB_ERR;
}

LRESULT CXTCheckListBox::OnLBGetItemData(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = DefWindowProc(LB_GETITEMDATA, wParam, lParam);

	if (lResult != LB_ERR)
	{
		CHECK_DATA* pState = (CHECK_DATA*)lResult;

		if (pState == NULL)
			return 0; // default

		lResult = pState->m_dwUserData;
	}
	return lResult;
}

LRESULT CXTCheckListBox::OnLBGetText(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = DefWindowProc(LB_GETTEXT, wParam, lParam);

	if (GetStyle() & LBS_HASSTRINGS)
		return lResult;

	if (lResult != LB_ERR)
	{
		CHECK_DATA* pState = (CHECK_DATA*)lParam;

		if (pState != NULL)
			lParam = pState->m_dwUserData;
	}
	return lResult;
}

LRESULT CXTCheckListBox::OnLBInsertString(WPARAM wParam, LPARAM lParam)
{
	CHECK_DATA* pState = NULL;

	if (!(GetStyle() & LBS_HASSTRINGS))
	{
		pState = new CHECK_DATA;
		pState->m_dwUserData = lParam;
		lParam = (LPARAM)pState;
	}

	LRESULT lResult = DefWindowProc(LB_INSERTSTRING, wParam, lParam);

	if (lResult == LB_ERR && pState != NULL)
		delete pState;

	return lResult;
}

LRESULT CXTCheckListBox::OnLBSelectString(WPARAM wParam, LPARAM lParam)
{
	if (GetStyle() & LBS_HASSTRINGS)
		return DefWindowProc(LB_SELECTSTRING, wParam, lParam);

	int nIndex = (int)wParam;
	if (nIndex == -1) nIndex = 0;

	for(; nIndex < GetCount(); nIndex++)
		if ((UINT_PTR)lParam == GetItemData(nIndex))
		{
			SetCurSel(nIndex);
			return nIndex;
		}

		return LB_ERR;
}

LRESULT CXTCheckListBox::OnLBSetItemData(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = DefWindowProc(LB_GETITEMDATA, wParam, 0);

	if (lResult != LB_ERR)
	{
		CHECK_DATA* pState = (CHECK_DATA*)lResult;

		if (pState == NULL)
			pState = new CHECK_DATA;

		pState->m_dwUserData = lParam;
		lResult = DefWindowProc(LB_SETITEMDATA, wParam, (LPARAM)pState);

		if (lResult == LB_ERR)
			delete pState;
	}
	return lResult;
}

LRESULT CXTCheckListBox::OnLBSetItemHeight(WPARAM wParam, LPARAM lParam)
{
	int nHeight = max(CalcMinimumItemHeight(),(int)LOWORD(lParam));
	return DefWindowProc(LB_SETITEMHEIGHT, wParam, MAKELPARAM(nHeight,0));
}
