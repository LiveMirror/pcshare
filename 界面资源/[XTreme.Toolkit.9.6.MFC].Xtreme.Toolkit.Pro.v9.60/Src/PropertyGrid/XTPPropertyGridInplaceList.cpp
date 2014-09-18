// XTPPropertyGridInplaceList.cpp : implementation of the CXTPPropertyGridInplaceList class.
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

#include "XTPPropertyGridInplaceEdit.h"
#include "XTPPropertyGridInplaceButton.h"
#include "XTPPropertyGridInplaceList.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGridInplaceList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceList

CXTPPropertyGridInplaceList::CXTPPropertyGridInplaceList()
: m_pItem(0)
{
}

CXTPPropertyGridInplaceList::~CXTPPropertyGridInplaceList()
{
}

IMPLEMENT_DYNAMIC(CXTPPropertyGridInplaceList, CListBox)

BEGIN_MESSAGE_MAP(CXTPPropertyGridInplaceList, CListBox)
	//{{AFX_MSG_MAP(CXTPPropertyGridInplaceList)
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTPPropertyGridInplaceList::Create(CXTPPropertyGridItem* pItem, CRect rect)
{
	ASSERT(pItem && pItem->GetGrid());

	CRect rcValue = pItem->GetValueRect();

	CWnd* pParent = (CWnd*)pItem->GetGrid();

	DestroyWindow();

	if (!m_hWnd)
	{
		CListBox::CreateEx(WS_EX_TOOLWINDOW |pParent->GetExStyle() & WS_EX_LAYOUTRTL, _T("LISTBOX"), _T(""), LBS_NOTIFY |WS_CHILD|WS_BORDER|WS_VSCROLL, CRect(0, 0, 0, 0), pParent, 0);
		SetOwner(pParent);
	}
	SetFont(pParent->GetFont());

	ResetContent();

	CXTPPropertyGridItemConstraints* pList = pItem->GetConstraints();


	int nIndex = 0;
	int dx = rect.right - rcValue.left;

	CWindowDC dc(pParent);
	CXTPFontDC font(&dc, pParent->GetFont());
	SetItemHeight(0, dc.GetTextExtent(_T(" ")).cy + 3);
	
	int nThumbLength = GetSystemMetrics(SM_CXHTHUMB);

	for (int i = 0; i < pList->GetCount(); i++)
	{
		CString str = pList->GetAt(i);
		AddString(str);

		dx = __max(dx, dc.GetTextExtent(str).cx + nThumbLength * 2);

		if (pItem->GetValue() == str)
			SetCurSel(nIndex);
		nIndex++;
	}

	int nHeight = GetItemHeight(0);
	rect.top = rect.bottom;
	rect.bottom += nHeight * __min(10, GetCount()) + 2;
	rect.left = rect.right - __min(dx, rect.Width() - XTP_PGI_EXPAND_BORDER);

	pParent->ClientToScreen(&rect);

	CRect rcWork = CXTPDrawHelpers::GetWorkArea(rcValue);
	if (rect.bottom > rcWork.bottom && rect.top > rcWork.CenterPoint().y)
	{
		rect.OffsetRect(0, - rect.Height() - rcValue.Height() - 3);
	}
	if (rect.left < rcWork.left) rect.OffsetRect(rcWork.left - rect.left, 0);
	if (rect.right > rcWork.right) rect.OffsetRect(rcWork.right - rect.right, 0);


	SetFocus();

#ifdef XTP_SITENOTIFY_ONFOCUS
	XTP_SITENOTIFY_ONFOCUS(pParent, pParent, TRUE)
#endif

	SetWindowLong(m_hWnd, GWL_HWNDPARENT, 0 );
	ModifyStyle(WS_CHILD, WS_POPUP);
	SetWindowLongPtr(m_hWnd, GWL_HWNDPARENT, (LONG_PTR)pParent->m_hWnd );

	SetWindowPos(0, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER|SWP_SHOWWINDOW|SWP_NOACTIVATE);

	CXTPMouseMonitor::SetupHook(this);

	m_pItem = pItem;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceList message handlers

void CXTPPropertyGridInplaceList::OnKillFocus(CWnd* pNewWnd)
{
	CListBox::OnKillFocus(pNewWnd);

	ShowWindow(SW_HIDE);
	CXTPMouseMonitor::SetupHook(NULL);

#ifdef XTP_SITENOTIFY_ONFOCUS
	XTP_SITENOTIFY_ONFOCUS(GetOwner(), GetOwner(), FALSE)
#endif
}

int CXTPPropertyGridInplaceList::OnMouseActivate(CWnd* /*pDesktopWnd*/, UINT /*nHitTest*/, UINT /*message*/)
{
	return MA_NOACTIVATE;
}
void CXTPPropertyGridInplaceList::PostNcDestroy()
{
	CXTPMouseMonitor::SetupHook(NULL);

	CListBox::PostNcDestroy();
}

void CXTPPropertyGridInplaceList::OnLButtonUp(UINT, CPoint point)
{
	CXTPClientRect rc(this);

	if (rc.PtInRect(point)) Apply(); else Cancel();
}


void CXTPPropertyGridInplaceList::Cancel(void)
{
	GetOwner()->SetFocus();
}

void CXTPPropertyGridInplaceList::Apply(void)
{
	int nIndex = GetCurSel();
	if (nIndex != LB_ERR)
	{
		CXTPPropertyGridItemConstraints* pList = m_pItem->GetConstraints();
		pList->SetCurrent(nIndex);

		CString str;
		GetText(nIndex, str);

		if (str != m_pItem->GetValue())
			m_pItem->OnValueChanged(str);

	}
	GetOwner()->SetFocus();
}

void CXTPPropertyGridInplaceList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE) Cancel();
	else if (nChar == VK_RETURN || nChar == VK_F4) Apply();
	else CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPPropertyGridInplaceList::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_DOWN || nChar == VK_UP)
	{
		Apply();
		return;
	}

	CListBox::OnSysKeyDown(nChar, nRepCnt, nFlags);
}


void CXTPPropertyGridInplaceList::DestroyItem()
{
	// reset variables to defaults.
	m_pItem = NULL;

	CXTPMouseMonitor::SetupHook(NULL);

	// destroy the window.
	//DestroyWindow( );
	if (::IsWindow(m_hWnd)) ShowWindow(SW_HIDE);
}
