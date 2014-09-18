// XTTreeBase.cpp : implementation file
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
#include <windowsx.h>
#include "XTPColorManager.h"

#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTVC50Helpers.h"
#include "XTTreeBase.h"
#include "XTMemDC.h"
#include "XTVersionManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef SCROLL_TIMER_PERIOD
#define SCROLL_TIMER_PERIOD 75
#endif

#ifndef HOVER_TIMER_PERIOD
#define HOVER_TIMER_PERIOD  85
#endif

#ifndef TVIS_UNDERLINE
#define TVIS_UNDERLINE      0x10000
#endif // TVIS_UNDERLINE

#ifndef COLOR_HOTLIGHT
#define COLOR_HOTLIGHT      26
#endif // COLOR_HOTLIGHT



/////////////////////////////////////////////////////////////////////////////
// CXTTreeBase

CXTTreeBase::CXTTreeBase()
: m_hSelect(NULL)
, m_bMultiSelect(false)
, m_bBandLabel(true)
, m_bActionDone(false)
, m_bTreeNotify(false)
, m_bOkToEdit(true)
, m_htiEdit(NULL)
, m_htiLast(NULL)
{

}

CXTTreeBase::~CXTTreeBase()
{
}

/////////////////////////////////////////////////////////////////////////////
// CXTTreeBase message handlers

void CXTTreeBase::OnLButtonDown(UINT nFlags, CPoint point)
{
	// If multiselect control, process possible left
	// click drag selection.
	if ( m_bMultiSelect )
	{
		UINT nHitFlags = 0;

		HTREEITEM hItemHit = m_pTreeCtrl->HitTest(point, &nHitFlags);
		HTREEITEM hItemSel = m_pTreeCtrl->GetSelectedItem();

		// if expanding/contracting call base class.
		if ( ( nHitFlags & (TVHT_ONITEMBUTTON|TVHT_ONITEMSTATEICON) ) != 0)
		{
			m_pTreeCtrl->Default();
			return;
		}

		if ( HasEditLabels( ) )
		{
			if( !( nFlags & ( MK_CONTROL|MK_SHIFT ) ) && ( nHitFlags & TVHT_ONITEMLABEL ) )
			{
				if ( m_bOkToEdit && hItemHit == hItemSel )
				{
					SelectAll(FALSE);
					SelectItem(hItemHit);

					m_pTreeCtrl->Default();
					return;
				}
			}

			if ( ( nHitFlags & TVHT_ONITEM ) == 0 ) {
				m_bOkToEdit = false;
			}

			else if ( GetFocusedItem() == hItemSel ) {
				m_bOkToEdit = true;
			}
		}

		OnButtonDown(TRUE, nFlags, point);
	}

	else
	{
		m_pTreeCtrl->Default();
	}
}

void CXTTreeBase::OnRButtonDown(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	// hittest to get the tree item under the cursor
	// and select it.
	UINT uFlags;
	HTREEITEM hItem = m_pTreeCtrl->HitTest(point, &uFlags);
	if (hItem != NULL && (uFlags & TVHT_ONITEM) != 0)
	{
		// if the item is not selected, clear previous
		// selections and select the item under cursor.
		if (!IsSelected(hItem))
		{
			SelectAll(FALSE);
			SelectItem(hItem);
		}
	}
	else
	{
		// clear previous selections.
		SelectAll(FALSE);
	}

	// get the owner of the tree control.
	HWND hWnd = m_pTreeCtrl->GetOwner()->m_hWnd;

	if (::IsWindow(hWnd))
	{
		// construct a NMHDR struct...
		NMHDR mHDR;
		mHDR.hwndFrom = m_pTreeCtrl->m_hWnd;
		mHDR.code     = NM_RCLICK;
		mHDR.idFrom   = m_pTreeCtrl->GetDlgCtrlID();

		// and send a WM_NOTIFY message to our owner.
		SendNotify( &mHDR );
	}
}

void CXTTreeBase::OnSetFocus(CWnd* /*pOldWnd*/)
{
	if (m_bMultiSelect)
	{
		//'emulated' selected items will remain greyed
		// if application gets covered
		HTREEITEM hItem = GetFirstSelectedItem();
		while (hItem)
		{
			RECT rect;
			m_pTreeCtrl->GetItemRect(hItem, &rect, TRUE);
			m_pTreeCtrl->InvalidateRect(&rect);
			hItem = GetNextSelectedItem(hItem);
		}
	}

	m_pTreeCtrl->Default();
}

void CXTTreeBase::OnKillFocus(CWnd* /*pNewWnd*/)
{
	m_pTreeCtrl->Default();

	if (m_bMultiSelect)
	{
		//'emulated' selected items may not get
		// refreshed to grey
		HTREEITEM hItem = GetFirstSelectedItem();
		while (hItem)
		{
			RECT rect;
			m_pTreeCtrl->GetItemRect(hItem, &rect, TRUE);
			m_pTreeCtrl->InvalidateRect(&rect);
			hItem = GetNextSelectedItem(hItem);
		}
	}
}

void CXTTreeBase::OnKeyDown(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
	if (!m_bMultiSelect) {
		m_pTreeCtrl->Default();
		return;
	}

	BOOL bCtrl  = (::GetKeyState(VK_CONTROL) < 0);
	BOOL bShift = (::GetKeyState(VK_SHIFT)   < 0);
	BOOL bDir   = FALSE;
	UINT uCount = GetSelectedCount();

	HTREEITEM hSel = NULL;
	switch (nChar)
	{
	case VK_UP:
		bDir = TRUE;
	case VK_DOWN:
		hSel = m_pTreeCtrl->GetSelectedItem();
		if (!m_hSelect) {
			m_hSelect = hSel;
		}
		if (!bCtrl && !bShift)
		{
			m_hSelect = NULL;   //reset

			if (uCount > 1) {
				SelectAll(FALSE);
			}
			else
			{
				// get the currently focused item.
				HTREEITEM htItem = GetFocusedItem();

				// if NULL try the first selected item.
				if (htItem == NULL)
					htItem = GetFirstSelectedItem();

				// if NULL try the root item.
				if (htItem == NULL)
				{
					htItem = m_pTreeCtrl->GetRootItem();

					// set focus to root.
					if (htItem != NULL) {
						FocusItem(htItem);
					}

					return;
				}

				// if not NULL set the item's state.
				if (htItem != NULL)
				{
					SetItemState(htItem, TVIS_FOCUSED|TVIS_SELECTED,
						TVIS_FOCUSED|TVIS_SELECTED);
				}
			}
		}
		break;
	}

	m_pTreeCtrl->Default();
	if (!hSel || (!bCtrl && !bShift)) {
		return;
	}

	HTREEITEM hNext = bDir ? m_pTreeCtrl->GetPrevVisibleItem(hSel) : m_pTreeCtrl->GetNextVisibleItem(hSel);
	if (!hNext) {
		hNext = hSel;
	}
	if (bShift) {
		SelectItems(m_hSelect, hNext, TRUE);
	}
	else if (bCtrl) {
		SetItemState(hNext, TVIS_FOCUSED, TVIS_FOCUSED);
	}
}

BOOL CXTTreeBase::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	*pResult = 0;

	// set m_htiEdit equal to pTVDispInfo->item.hItem.
	m_htiEdit = pTVDispInfo->item.hItem;

	return FALSE;   //pass to parent
}

BOOL CXTTreeBase::OnEndLabelEdit(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	*pResult = 0;

	// set m_htiEdit equal to NULL.
	m_htiEdit = NULL;

	return FALSE;   //pass to parent
}

BOOL CXTTreeBase::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (!m_bMultiSelect)
		return FALSE;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;

	// TODO: Add your control notification handler code here
	if ((pNMTreeView->action == TVE_COLLAPSE) || (pNMTreeView->action == TVE_COLLAPSERESET))
	{
		// clear selection of children, it would be confusing otherwise
		// - the notifications can be over-ridden to stop the de-selection
		// if required.
		// Unfortunately, the parent window can't over-ride this functionality
		// because MFC gives this class first crack.  So if changes are required
		// a derived class will have to be used..
		ASSERT(pNMTreeView->itemNew.hItem);

		// if a descendent item has focus the parent will get selected as a
		// consequence of collapsing the tree, so preserve
		// (if the parent was already selected it will gain focus, but
		// that's acceptable)
		BOOL bWasSel   = IsSelected(pNMTreeView->itemNew.hItem);
		BOOL bWasFocus = SelectChildren(pNMTreeView->itemNew.hItem, FALSE, TRUE);

		if (bWasFocus && !bWasSel) {
			FocusItem(pNMTreeView->itemNew.hItem); // give parent focus.
		}
	}

	return FALSE;   //pass to parent
}

HTREEITEM CXTTreeBase::GetNextItem( HTREEITEM hItem ) const
{
	HTREEITEM hti = NULL;

	if (m_pTreeCtrl->ItemHasChildren(hItem)) {
		hti = m_pTreeCtrl->GetChildItem(hItem);
	}

	if (hti == NULL)
	{
		while ((hti = m_pTreeCtrl->GetNextSiblingItem(hItem)) == NULL)
		{
			if ((hItem = m_pTreeCtrl->GetParentItem(hItem)) == NULL)
				return NULL;
		}
	}

	return hti;
}

HTREEITEM CXTTreeBase::GetPrevItem( HTREEITEM hItem ) const
{
	HTREEITEM hti = NULL;

	hti = m_pTreeCtrl->GetPrevSiblingItem(hItem);
	if( hti == NULL ) {
		hti = m_pTreeCtrl->GetParentItem(hItem);
	}
	else {
		hti = GetLastItem(hti);
	}

	return hti;
}

HTREEITEM CXTTreeBase::GetLastItem( HTREEITEM hItem ) const
{
	// Temporary used variable
	HTREEITEM htiNext;

	// Get the last item at the top level
	if( hItem == NULL ) {
		hItem = m_pTreeCtrl->GetRootItem();
	}

	while( m_pTreeCtrl->ItemHasChildren( hItem ) != NULL )
	{
		// Find the last child of hItem
		htiNext = m_pTreeCtrl->GetChildItem( hItem );
		while( htiNext != NULL )
		{
			hItem = htiNext;
			htiNext = m_pTreeCtrl->GetNextSiblingItem( htiNext );
		}
	}

	return hItem;

}

HTREEITEM CXTTreeBase::FindItemInBranch(LPCTSTR lpszSearch, BOOL bCaseSensitive /*= FALSE*/,
	BOOL bWholeWord /*= FALSE*/, HTREEITEM htiItem /*= NULL*/)
{
	HTREEITEM htiFound = NULL;

	if (!m_pTreeCtrl->ItemHasChildren(htiItem))
		return NULL;

	CString strSearch = lpszSearch;
	int iLen = strSearch.GetLength();
	if (iLen == 0) {
		return NULL;
	}

	if (!bCaseSensitive) {
		strSearch.MakeLower();
	}

	HTREEITEM htiChild = m_pTreeCtrl->GetChildItem(htiItem);
	while (htiChild != NULL)
	{
		if (m_pTreeCtrl->ItemHasChildren(htiChild))
		{
			htiFound = FindItemInBranch(lpszSearch, bCaseSensitive, bWholeWord, htiChild);
			if (htiFound != NULL)
				return htiFound;
		}

		CString strItemText = m_pTreeCtrl->GetItemText( htiChild );
		if (!bCaseSensitive) {
			strItemText.MakeLower();
		}

		int iIndex;
		while ((iIndex = strItemText.Find( strSearch )) != -1)
		{
			// Search string found
			if (bWholeWord)
			{
				// Check preceding char
				if (iIndex != 0)
				{
					if( _istalpha(strItemText[iIndex-1]) || strItemText[iIndex-1] == '_' )
					{
						// Not whole word
						strItemText = strItemText.Right(strItemText.GetLength() -
							iIndex - iLen );
						continue;
					}
				}

				// Check succeeding char
				if (strItemText.GetLength() > iIndex + iLen &&
					(_istalpha(strItemText[iIndex+iLen]) ||
					(strItemText[iIndex+iLen] == '_')))
				{
					// Not whole word
					strItemText = strItemText.Right( strItemText.GetLength() -
						iIndex - strSearch.GetLength() );
					continue;
				}
			}

			if (IsFindValid(htiChild)) {
				return htiChild;
			}

			else {
				break;
			}
		}

		htiChild = m_pTreeCtrl->GetNextSiblingItem(htiChild);
	}

	return NULL;
}

HTREEITEM CXTTreeBase::FindItem(LPCTSTR lpszSearch, BOOL bCaseSensitive /*= FALSE*/,
	BOOL bDownDir /*= TRUE*/, BOOL bWholeWord /*= FALSE*/, HTREEITEM hItem /*= NULL*/)
{
	CString str = lpszSearch;

	int lenSearchStr = str.GetLength();
	if (lenSearchStr == 0) {
		return NULL;
	}

	// For the first pass, set the current item equal to the selection
	HTREEITEM htiSel = hItem ? hItem : m_pTreeCtrl->GetSelectedItem();

	// If NULL, use root item.
	if (htiSel == NULL)
		htiSel = m_pTreeCtrl->GetRootItem();

	HTREEITEM htiCur = htiSel;
	CString strSearch = str;

	// make sure it ends if we started with no selection
	if ((htiCur == NULL) && (htiSel != NULL))
	{
		if (bDownDir) {
			htiCur = m_pTreeCtrl->GetRootItem();
		}
		else {
			htiCur = GetLastItem(NULL);
		}
	}

	if (!bCaseSensitive) {
		strSearch.MakeLower();
	}

	// For the first pass only, we check to see if it
	// is the item we're looking for.
	BOOL bFirstPass = TRUE;

	while( htiCur && (htiCur != htiSel || bFirstPass) )
	{
		bFirstPass = FALSE;

		CString strItemText = m_pTreeCtrl->GetItemText( htiCur );
		if (!bCaseSensitive) {
			strItemText.MakeLower();
		}

		int iIndex;
		while ((iIndex = strItemText.Find( strSearch )) != -1)
		{
			// Search string found
			if (bWholeWord)
			{
				// Check preceding char
				if (iIndex != 0)
				{
					if( _istalpha(strItemText[iIndex-1]) || strItemText[iIndex-1] == '_' )
					{
						// Not whole word
						strItemText = strItemText.Right(strItemText.GetLength() -
							iIndex - lenSearchStr );
						continue;
					}
				}

				// Check succeeding char
				if (strItemText.GetLength() > iIndex + lenSearchStr &&
					(_istalpha(strItemText[iIndex+lenSearchStr]) ||
					(strItemText[iIndex+lenSearchStr] == '_')))
				{
					// Not whole word
					strItemText = strItemText.Right( strItemText.GetLength() -
						iIndex - strSearch.GetLength() );
					continue;
				}
			}

			if (IsFindValid(htiCur)) {
				return htiCur;
			}
			else {
				break;
			}
		}

		htiCur = bDownDir ? GetNextItem( htiCur ) : GetPrevItem( htiCur );
		if( htiCur == NULL )
		{
			if (bDownDir) {
				htiCur = m_pTreeCtrl->GetRootItem();
			}
			else {
				htiCur = GetLastItem( NULL );
			}
		}
	}

	return NULL;
}

BOOL CXTTreeBase::IsFindValid( HTREEITEM )
{
	return TRUE;
}

void CXTTreeBase::SetItemFont(HTREEITEM hItem, LOGFONT& logfont)
{
	CLRFONT cf;
	if (!m_mapColorFont.Lookup(hItem, cf)) {
		cf.color = (COLORREF)-1;
	}

	cf.logfont = logfont;
	m_mapColorFont[hItem] = cf;
}

BOOL CXTTreeBase::GetItemFont(HTREEITEM hItem, LOGFONT * plogfont)
{
	CLRFONT cf;
	if (!m_mapColorFont.Lookup(hItem, cf)) {
		return FALSE;
	}

	if (cf.logfont.lfFaceName[0] == _T('\0')) {
		return FALSE;
	}

	*plogfont = cf.logfont;

	return TRUE;

}

void CXTTreeBase::SetItemBold(HTREEITEM hItem, BOOL bBold)
{
	SetItemState( hItem, bBold ? TVIS_BOLD: 0, TVIS_BOLD );
	m_pTreeCtrl->InvalidateRect(NULL);
}

BOOL CXTTreeBase::GetItemBold(HTREEITEM hItem)
{
	return GetItemState( hItem, TVIS_BOLD ) & TVIS_BOLD;
}

void CXTTreeBase::SetItemColor(HTREEITEM hItem, COLORREF color)
{
	CLRFONT cf;
	if (!m_mapColorFont.Lookup( hItem, cf)) {
		cf.logfont.lfFaceName[0] = _T('\0');
	}

	cf.color = color;
	m_mapColorFont[hItem] = cf;
	m_pTreeCtrl->InvalidateRect(NULL);
}

COLORREF CXTTreeBase::GetItemColor(HTREEITEM hItem)
{
	CLRFONT cf;
	if (!m_mapColorFont.Lookup(hItem, cf)) {
		return (COLORREF)-1;
	}
	return cf.color;
}


COLORREF CXTTreeBase::GetTreeBackColor() const
{
	// if the tree is disabled return disabled back color.
	if (!m_pTreeCtrl->IsWindowEnabled())
		return GetXtremeColor(COLOR_3DFACE);

#if _MSC_VER >= 1200 // MFC 6.0

	// is there a user defined color available?
	COLORREF crBack = m_pTreeCtrl->GetBkColor();
	if (crBack != (COLORREF)-1)
		return crBack;

#endif

	// return the system window color.
	return GetXtremeColor(COLOR_WINDOW);
}

COLORREF CXTTreeBase::GetTreeTextColor() const
{
	// if the tree is disabled return disabled text color.
	if (!m_pTreeCtrl->IsWindowEnabled())
		return GetXtremeColor(COLOR_GRAYTEXT);

#if _MSC_VER >= 1200 // MFC 6.0

	// is ther a user defined color available?
	COLORREF crText = m_pTreeCtrl->GetTextColor();
	if (crText != (COLORREF)-1)
		return crText;

#endif

	// return the system window text color.
	return GetXtremeColor(COLOR_WINDOWTEXT);
}

COLORREF CXTTreeBase::GetItemBackColor(UINT uState, bool bTreeHasFocus, DWORD dwStyle, COLORREF crBack) const
{
	// if the tree item is selected or drop highlighted.
	if (uState & TVIS_SELECTED || uState & TVIS_DROPHILITED)
	{
		// if the tree has focus and draw the text background hilite color.
		if (bTreeHasFocus)
		{
			if (m_pTreeCtrl->IsWindowEnabled())
				return GetXtremeColor(COLOR_HIGHLIGHT);
			else
				return GetXtremeColor(COLOR_GRAYTEXT);
		}

		// if the tree does not have focus and the TVS_SHOWSELALWAYS flag is
		// set draw the text background hilite color.
		else if (dwStyle & TVS_SHOWSELALWAYS)
		{
			if (m_pTreeCtrl->IsWindowEnabled())
				return GetXtremeColor(COLOR_3DFACE);
			else
				return GetXtremeColor(COLOR_GRAYTEXT);
		}
	}

	return crBack;
}

COLORREF CXTTreeBase::GetItemTextColor(UINT uState, bool bTreeHasFocus, DWORD dwStyle, COLORREF crText) const
{
	// if the tree item is selected or drop highlighted.
	if (uState & TVIS_SELECTED || uState & TVIS_DROPHILITED)
	{
		// if the tree has focus and draw the text hilite color.
		if (bTreeHasFocus)
		{
			if (m_pTreeCtrl->IsWindowEnabled())
				return GetXtremeColor(COLOR_HIGHLIGHTTEXT);
			else
				return GetXtremeColor(COLOR_3DFACE);
		}

		// if the tree does not have focus and the TVS_SHOWSELALWAYS flag is
		// set draw the text hilite color.
		else if (dwStyle & TVS_SHOWSELALWAYS)
		{
			if (m_pTreeCtrl->IsWindowEnabled())
				return GetXtremeColor(COLOR_WINDOWTEXT);
			else
				return GetXtremeColor(COLOR_3DFACE);
		}
	}

	return crText;
}


void CXTTreeBase::DoPaint(CDC& dc, BOOL bInternal)
{
	// Get the client rect.
	CRect rcClient;
	m_pTreeCtrl->GetClientRect(&rcClient);

	const COLORREF crTreeBack = GetTreeBackColor();
	const COLORREF crTreeText = GetTreeTextColor();

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTMemDC memDC(&dc, rcClient, crTreeBack);

	// Now let the window do its default painting...
	m_pTreeCtrl->DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );

	if (!bInternal)
		return;

	int nSavedDC = memDC.SaveDC();

	// check to see if a tree label is getting edited.
	BOOL bEditing = (HasEditLabels() && m_pTreeCtrl->GetEditControl());

	// if not editing, make sure the edit item is set to NULL.
	if (!bEditing)
		m_htiEdit = NULL;

	// check to see if the tree is enabled.
	BOOL bIsEnabled = m_pTreeCtrl->IsWindowEnabled();

	// check to see if the tree has focus.
	bool bTreeHasFocus = (CWnd::GetFocus() == m_pTreeCtrl);

	// get the style for the tree.
	DWORD dwStyle = m_pTreeCtrl->GetStyle();

	// get the visible count.
	HTREEITEM hItem = m_pTreeCtrl->GetFirstVisibleItem();
	int iVisibleCount = m_pTreeCtrl->GetVisibleCount()+1;

	while (hItem && iVisibleCount--)
	{
		// get the tree item rect.
		CRect rcItem;
		m_pTreeCtrl->GetItemRect(hItem, &rcItem, TRUE);

		// refresh the background.
		CRect rcFill = rcItem;
		rcFill.right = rcClient.right;
		memDC.FillSolidRect(&rcFill, crTreeBack);

		// get the state of the tree item.
		UINT uState = GetItemState(hItem, TVIS_SELECTED|TVIS_FOCUSED|TVIS_DROPHILITED|TVIS_UNDERLINE);
		BOOL bSelected = ((uState & (TVIS_SELECTED|TVIS_DROPHILITED)) != 0);

		// define the background and text colors.
		COLORREF crItemBack = GetItemBackColor(
			uState, bTreeHasFocus, dwStyle, crTreeBack);
		COLORREF crItemText = GetItemTextColor(
			uState, bTreeHasFocus, dwStyle, crTreeText);

		// if the item is getting edited paint white.
		if (m_htiEdit == hItem)
		{
			crItemBack = crTreeBack;
			crItemText = crTreeBack;
		}

		// create the LOGFONT used by the tree item.
		LOGFONT logfont;
		m_pTreeCtrl->GetFont()->GetLogFont(&logfont);

		CLRFONT cf;
		if (m_mapColorFont.Lookup(hItem, cf))
		{
			if (cf.color != (COLORREF)-1 && !bSelected && bIsEnabled) {
				crItemText = cf.color;
			}
			if (cf.logfont.lfFaceName[0] != _T('\0')) {
				logfont = cf.logfont;
			}
		}

		if (GetItemBold(hItem)) {
			logfont.lfWeight = FW_BOLD;
		}

		if (dwStyle & TVS_TRACKSELECT)
		{
			logfont.lfUnderline = (uState & TVIS_UNDERLINE) != 0;

			if (logfont.lfUnderline && !bSelected)
			{
				if (XTOSVersionInfo()->IsWin95() || XTOSVersionInfo()->IsWinNT4())
					crItemText = GetXtremeColor(COLOR_HIGHLIGHT);
				else
					crItemText = GetXtremeColor(COLOR_HOTLIGHT);
			}
		}

		// create a CFont object from the LOGFONT structure and
		// select it into the current device context.
		CFont fontDC;
		fontDC.CreateFontIndirect(&logfont);
		CFont* pOldFont = memDC.SelectObject(&fontDC);

		// get the text for the tree item and determine its size.
		CString strItem = m_pTreeCtrl->GetItemText(hItem);
		CSize sizeText = memDC.GetTextExtent(strItem);

		// if the text is wider than the tree item label, adjust accordingly.
		if (rcItem.Width() < sizeText.cx) {
			rcItem.right = rcItem.left + sizeText.cx + 2;
		}

		// set the font foreground and background colors.
		memDC.SetBkColor(crItemBack);
		memDC.SetTextColor(crItemText);

		// draw the label background if selected.
		if (bSelected) {
			memDC.FillSolidRect(&rcItem, crItemBack);
		}

		// draw the text and restore the device context.
		memDC.DrawText(strItem, &rcItem, DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_NOPREFIX);

		// draw the focus rect.
		if ((uState & TVIS_FOCUSED))
		{
			if (!bEditing && bIsEnabled && CWnd::GetFocus() == m_pTreeCtrl)
			{
				memDC.SetTextColor(crTreeText);
				memDC.DrawFocusRect(&rcItem);
			}
		}

		memDC.SelectObject( pOldFont );
		fontDC.DeleteObject();

		// move to the next visible item.
		hItem = m_pTreeCtrl->GetNextVisibleItem( hItem );
	}

	memDC.RestoreDC(nSavedDC);
}


HTREEITEM CXTTreeBase::GetPrevSelectedItem(HTREEITEM hItem) const
{
	for (hItem = m_pTreeCtrl->GetPrevVisibleItem(hItem);
		hItem != NULL;
		hItem = m_pTreeCtrl->GetPrevVisibleItem(hItem))
	{
		if (GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED) {
			return hItem;
		}
	}

	return NULL;
}

UINT CXTTreeBase::GetSelectedCount() const
{
	UINT nCount = 0;
	HTREEITEM hItem = GetFirstSelectedItem();
	while (hItem)
	{
		nCount++;
		hItem = GetNextSelectedItem(hItem);
	}
	return nCount;
}

BOOL CXTTreeBase::EnableMultiSelect(BOOL bMultiSelect)
{
	BOOL bReturn = m_bMultiSelect;
	m_bMultiSelect = bMultiSelect;

	if (!m_bMultiSelect)
	{
		HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
		if (hItem && !IsSelected(hItem)) {
			hItem = NULL;
		}
		SelectAllIgnore(FALSE, hItem);
		if (hItem) {
			SelectItem(hItem);
		}
	}

	return bReturn;
}

BOOL CXTTreeBase::SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask)
{
	ASSERT(hItem);

	if (!m_bMultiSelect) {
		return m_pTreeCtrl->SetItemState(hItem, nState, nStateMask);
	}

	HTREEITEM hFocus  = m_pTreeCtrl->GetSelectedItem();           // current focus
	BOOL bWasFocus    = (hFocus == hItem);
	BOOL bFocusWasSel = hFocus && IsSelected(hFocus); // selection state of current focus
	BOOL bWasSel      = IsSelected(hItem);            // select state of acting item

	UINT nS  = (nState & ~TVIS_FOCUSED);
	UINT nSM = (nStateMask & ~TVIS_FOCUSED);

	if (nStateMask & TVIS_FOCUSED)
	{
		//wanted to affect focus
		if (nState & TVIS_FOCUSED)
		{
			//wanted to set focus
			if (!bWasFocus && bFocusWasSel)
			{
				//because SelectItem would de-select the current 'real' selection
				// (the one with focus), need to make the tree ctrl think there is
				// no 'real' selection but still keep the the old item selected
				//it has to be done before the SelectItem call because
				// otherwise the TVN_SELCHANGING/ED notification handlers
				// wouldn't be able to get the proper list of selected items
				m_pTreeCtrl->SelectItem(NULL); //will cause notify, but can be taken as focus change
				m_pTreeCtrl->SetItemState(hFocus, TVIS_SELECTED, TVIS_SELECTED);
				m_pTreeCtrl->UpdateWindow();
			}

			if (!m_pTreeCtrl->SelectItem(hItem)) { //set focus (will consequently select, if not already focused)
				return FALSE;
			}
			if (nStateMask & TVIS_SELECTED)
			{
				//wanted to affect select state
				if (nState & TVIS_SELECTED)
				{
					//wanted to select, already done if wasn't focused
					if (!bWasFocus || bFocusWasSel)
					{
						nS &= ~TVIS_SELECTED;
						nSM &= ~TVIS_SELECTED;
					}
				}
				//else wanted to clear, base call will do that
			}
			else
			{
				//didn't want to affect select state
				if (!bWasSel)
				{
					//it wasn't previously selected, let base clear (correct)
					nS &= ~TVIS_SELECTED;
					nSM |= TVIS_SELECTED;
				}
				//else was already selected, no harm done
			}
		}
		else
		{
			//wanted to clear focus
			if (bWasFocus)
			{
				//it had the focus
				m_pTreeCtrl->SelectItem(NULL);  //clear focus
				if (!(nStateMask & TVIS_SELECTED))
				{
					//didn't want to affect select state
					if (bWasSel)
					{
						//it was selected, so restore
						ASSERT( !(nS & TVIS_SELECTED) );
						ASSERT( !(nSM & TVIS_SELECTED) );
						//set state here, to avoid double-notify
						m_pTreeCtrl->SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
						//let base do other states
					}
				}
				else if (nState & TVIS_SELECTED)
				{
					//wanted to select (but clear focus)
					if (bWasSel)
					{
						//if was selected, restore
						m_pTreeCtrl->SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
					}
					//don't want to notify, default did it
					nS &= ~TVIS_SELECTED;
					nSM &= ~TVIS_SELECTED;
				}
			}
		}
	}

	if (!nSM) {
		return TRUE;    //no other states to alter
	}

	if (nSM & TVIS_SELECTED)
	{
		//still need to alter selection state
		NMTREEVIEW nmtv;
		nmtv.hdr.hwndFrom  = m_pTreeCtrl->m_hWnd;
		nmtv.hdr.idFrom    = ::GetDlgCtrlID(m_pTreeCtrl->m_hWnd);
		nmtv.hdr.code      = TVN_SELCHANGING;
		nmtv.itemOld.mask  = nmtv.itemNew.mask = 0;
		nmtv.itemOld.hItem = nmtv.itemNew.hItem = NULL;

		TVITEM& item       = (nS & TVIS_SELECTED) ? nmtv.itemNew : nmtv.itemOld;
		item.mask          = TVIF_HANDLE|TVIF_PARAM;
		item.hItem         = hItem;
		item.lParam        = m_pTreeCtrl->GetItemData(hItem);

		if (SendNotify(&nmtv.hdr)) {
			return FALSE;   //sel-changing stopped
		}

		VERIFY( m_pTreeCtrl->SetItemState(hItem, nS, nSM) );
		nmtv.hdr.code = TVN_SELCHANGED;
		SendNotify(&nmtv.hdr);
		nS &= ~TVIS_SELECTED;
		nSM &= ~TVIS_SELECTED;
	}

	if (!nSM) {
		return TRUE;
	}

	return m_pTreeCtrl->SetItemState(hItem, nS, nSM);
}

UINT CXTTreeBase::GetItemState(HTREEITEM hItem, UINT nStateMask) const
{
	UINT n = m_pTreeCtrl->GetItemState(hItem, nStateMask & ~TVIS_FOCUSED);
	if (nStateMask & TVIS_FOCUSED)
	{
		if (m_pTreeCtrl->GetSelectedItem() == hItem) {
			n |= TVIS_FOCUSED;
		}
	}
	return n;
}

BOOL CXTTreeBase::SelectItem(HTREEITEM hItem)
{
	if (m_bMultiSelect)
	{
		return SetItemState(hItem,
			TVIS_SELECTED|TVIS_FOCUSED, TVIS_SELECTED|TVIS_FOCUSED);
	}
	else
	{
		return m_pTreeCtrl->SelectItem(hItem);
	}
}

BOOL CXTTreeBase::FocusItem(HTREEITEM hItem)
{
	ASSERT(m_bMultiSelect);

	BOOL bRet = FALSE;
	if (hItem) {
		bRet = SetItemState(hItem, TVIS_FOCUSED, TVIS_FOCUSED);
	}
	else
	{
		hItem = m_pTreeCtrl->GetSelectedItem();
		if (hItem) {
			bRet = SetItemState(hItem, 0, TVIS_FOCUSED);
		}
	}
	return bRet;
}

LRESULT CXTTreeBase::SendNotify(LPNMHDR pNMHDR)
{
	CWnd* pWndOwner = m_pTreeCtrl->GetOwner();
	if (!pWndOwner)
	{
		return 0;
	}

	bool bTreeNotify = m_bTreeNotify;

	LRESULT lResult = ::SendMessage( pWndOwner->m_hWnd, WM_NOTIFY,
		( WPARAM )pNMHDR->idFrom, ( LPARAM )pNMHDR );

	m_bTreeNotify = bTreeNotify;

	return lResult;
}

HTREEITEM CXTTreeBase::GetFirstSelectedItem() const
{
	HTREEITEM hItem = m_pTreeCtrl->GetRootItem();
	while (hItem)
	{
		if (IsSelected(hItem)) {
			break;
		}
		hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
	}
	return hItem;
}

HTREEITEM CXTTreeBase::GetNextSelectedItem(HTREEITEM hItem) const
{
	hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
	while (hItem)
	{
		if (IsSelected(hItem)) {
			break;
		}
		hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
	}
	return hItem;
}

void CXTTreeBase::SelectAll(BOOL bSelect /*= TRUE*/, HTREEITEM htItem/*=NULL*/)
{
	bSelect = !!bSelect;    //ensure 0 or 1
	UINT nState = bSelect ? TVIS_SELECTED : 0;

	if (htItem == NULL)
		htItem = m_pTreeCtrl->GetRootItem();

	while (htItem)
	{
		if (IsSelected(htItem) != bSelect) {
			SetItemState(htItem, nState, TVIS_SELECTED);
		}
		htItem = m_pTreeCtrl->GetNextVisibleItem(htItem);
	}
}

void CXTTreeBase::SelectAllIgnore(BOOL bSelect, HTREEITEM hIgnore)
{
	//special case to avoid multiple notifications for
	// the same item
	bSelect = !!bSelect;    //ensure 0 or 1
	UINT nState = bSelect ? TVIS_SELECTED : 0;
	HTREEITEM hItem = m_pTreeCtrl->GetRootItem();
	while (hItem)
	{
		if (hItem != hIgnore)
		{
			if (IsSelected(hItem) != bSelect) {
				SetItemState(hItem, nState, TVIS_SELECTED);
			}
		}
		hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
	}
}

void CXTTreeBase::SelectItems(HTREEITEM hFirst, HTREEITEM hLast, BOOL bOnly /*= TRUE*/)
{
	//locate (and select) either first or last
	// (so order is arbitrary)
	HTREEITEM hItem = m_pTreeCtrl->GetRootItem();
	while (hItem)
	{
		if ((hItem == hFirst) || (hItem == hLast))
		{
			if (hFirst != hLast)
			{
				if (!IsSelected(hItem)) {
					SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
				}
				hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
			}
			break;
		}

		if (bOnly && IsSelected(hItem)) {
			SetItemState(hItem, 0, TVIS_SELECTED);
		}

		hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
	}

	//select rest of range
	while (hItem)
	{
		if (!IsSelected(hItem)) {
			SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
		}
		if ((hItem == hFirst) || (hItem == hLast))
		{
			hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
			break;
		}
		hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
	}
	if (!bOnly) {
		return;
	}
	while (hItem)
	{
		if (IsSelected(hItem)) {
			SetItemState(hItem, 0, TVIS_SELECTED);
		}
		hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
	}
}

BOOL CXTTreeBase::OnButtonDown(BOOL bLeft, UINT nFlags, CPoint point)
{
	UINT      nHF   = 0;
	HTREEITEM hItem = NULL;

	BOOL bBase = !m_bMultiSelect;
	if (!bBase)
	{
		hItem = m_pTreeCtrl->HitTest(point, &nHF);
		if (hItem)
		{
			//base always handles expanding items
			//(doesn't really mean much to right button, but pass anyway)
			bBase = (nHF & (TVHT_ONITEMBUTTON/*|TVHT_ONITEMINDENT*/));
			if (!bBase && bLeft && (m_pTreeCtrl->GetStyle() & TVS_CHECKBOXES))
			{
				//when the tree has check-boxes, the default handler makes
				// a quick selection of the clicked item, then re-selects
				// the previously selected item - to cause a sel-changed
				// notification.  Fortunately it doesn't affect the multi-
				// selection, so can pass on.
				bBase = (nHF & TVHT_ONITEMSTATEICON);

#ifdef _MST_MULTI_CHECK
				//Use the above define if you want all selected items to
				// be checked the same when any one of them is checked
				// - Interestingly this doesn't happen in the listview control
				//  (LVS_EX_CHECKBOXES)
				if (bBase)
				{
					//the default selection notification would mess
					// the multi-selection up, so generate the notification
					// manually
					// (anyway, this is smoother than the selection flicker
					//  the default gives)
					NMTREEVIEW nmtv;
#ifdef TVN_CHKCHANGE
					nmtv.hdr.code = TVN_CHKCHANGE;
#else
					nmtv.hdr.code = TVN_SELCHANGED;
#endif
					nmtv.hdr.hwndFrom = m_hWnd;
					nmtv.hdr.idFrom = ::GetDlgCtrlID(m_hWnd);
					nmtv.itemOld.hItem = NULL;
					nmtv.itemNew.mask = TVIF_HANDLE|TVIF_PARAM;

					BOOL bChk = !GetCheck(hItem);
					if (IsSelected(hItem))
					{
						HTREEITEM h = GetFirstSelectedItem();
						while (h)
						{
							if (!GetCheck(h) == bChk) //! to ensure 0 or 1
							{
								SetCheck(h, bChk);
#ifdef TVN_CHKCHANGE
								//only send multiple check-change
								// notifications (not sel-changed)
								if (h != hItem) //clicked item will be done last
								{
									nmtv.itemNew.hItem = h;
									nmtv.itemNew.lParam = m_pTreeCtrl->GetItemData(h);
									SendNotify(&nmtv.hdr);
								}
#endif
							}
							h = GetNextSelectedItem(h);
						}
					}
					else {
						SetCheck(hItem, bChk);
					}
					//notify clicked item
					nmtv.itemNew.hItem = hItem;
					nmtv.itemNew.lParam = m_pTreeCtrl->GetItemData(hItem);
					SendNotify(&nmtv.hdr);
					return TRUE;
				}
#endif
			}
		}
	}

	if (bBase)
		return TRUE;

	if (!hItem || (nHF & (TVHT_ONITEMRIGHT|TVHT_NOWHERE|TVHT_ONITEMINDENT)))
	{
		//clicked in space, do rubber-banding
		DoBanding(nFlags, point);
		return TRUE;
	}

	ASSERT(nHF & (TVHT_ONITEM|TVHT_ONITEMSTATEICON));   //nothing else left

	DoPreSelection(hItem, bLeft, nFlags);
	DoAction(hItem, bLeft, nFlags, point);

	return m_bActionDone;  // as set in doaction
}

void CXTTreeBase::DoPreSelection(HTREEITEM hItem, BOOL bLeft, UINT nFlags)
{
	if (bLeft)
	{
		//if shift key down, select immediately
		if ((nFlags & MK_SHIFT))
		{
			if (!m_hSelect) {
				m_hSelect = m_pTreeCtrl->GetSelectedItem(); //focus
			}
			SelectItems(m_hSelect, hItem, !(nFlags & MK_CONTROL));
			SetItemState(hItem, TVIS_FOCUSED, TVIS_FOCUSED);    //focus changes to last clicked
		}
		else
		{
			if (!(nFlags & MK_CONTROL))
			{
				//if ctrl was down, then the selection is delayed until
				// mouse up, otherwise select the one item
				if (!IsSelected(hItem)) {
					SelectAllIgnore(FALSE, hItem);
				}
				SetItemState(hItem, TVIS_SELECTED|TVIS_FOCUSED, TVIS_SELECTED|TVIS_FOCUSED);
			}
			m_hSelect = NULL;   //reset when a non-shift operation occurs
		}
		return;
	}

	//right mouse
	if (nFlags & (MK_CONTROL|MK_SHIFT))
	{
		if (!(nFlags & MK_SHIFT)) {
			m_hSelect = hItem;
		}
		return;     //do nothing if shift or ctrl
	}
	if (!IsSelected(hItem)) {
		SelectAllIgnore(FALSE, hItem);
	}
	SetItemState(hItem, TVIS_SELECTED|TVIS_FOCUSED, TVIS_SELECTED|TVIS_FOCUSED);
}

void CXTTreeBase::DoAction(HTREEITEM hItem, BOOL bLeft, UINT nFlags, CPoint point)
{
	m_bActionDone = false;
	::SetCapture(m_pTreeCtrl->m_hWnd);
	ASSERT(::GetCapture() == m_pTreeCtrl->m_hWnd);

	MSG msg;
	UINT nDone = 0;
	CPoint pt;
	CSize sizeDrag(::GetSystemMetrics(SM_CXDRAG), ::GetSystemMetrics(SM_CYDRAG));

	while (!nDone && ::GetMessage(&msg, NULL, 0, 0))
	{
		if (::GetCapture() != m_pTreeCtrl->m_hWnd) {
			break;
		}
		switch (msg.message)
		{
			case WM_MOUSEMOVE:
				pt.x = GET_X_LPARAM(msg.lParam);
				pt.y = GET_Y_LPARAM(msg.lParam);
				if ((abs(pt.x - point.x) > sizeDrag.cx) ||
					((abs(pt.y - point.y) > sizeDrag.cy))) {
					nDone = 2;
				}
				//because we exit loop, button up will still be dispatched
				// which means WM_CONTEXTMENU will be sent after TVN_BEGINRDRAG
				// - this is the same behaviour as original tree
				break;

			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
				nDone = 1;
				break;

			default:
				::DispatchMessage(&msg);
				break;
		}

	}

	::ReleaseCapture();
	ASSERT(::GetCapture() != m_pTreeCtrl->m_hWnd);

	//construct tree notification info
	NMTREEVIEW nmtv;
	nmtv.hdr.hwndFrom   = m_pTreeCtrl->m_hWnd;
	nmtv.hdr.idFrom     = ::GetDlgCtrlID(m_pTreeCtrl->m_hWnd);
	nmtv.itemNew.mask   = TVIF_HANDLE|TVIF_PARAM;
	nmtv.itemNew.hItem  = hItem;
	nmtv.itemNew.lParam = m_pTreeCtrl->GetItemData(hItem);
	DWORD dwStyle = m_pTreeCtrl->GetStyle();

	if (nDone == 1)
	{
		//click
		if (!(nFlags & MK_SHIFT) && bLeft)
		{
			UINT nState = TVIS_SELECTED;
			if ((nFlags & MK_CONTROL)) {
				nState ^= (GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED);
			}
			else {
				SelectAllIgnore(FALSE, hItem);
			}
			SetItemState(hItem, TVIS_FOCUSED|nState, TVIS_FOCUSED|TVIS_SELECTED);
		}
		if (::GetFocus() != m_pTreeCtrl->m_hWnd) {
			::SetFocus(m_pTreeCtrl->m_hWnd);
		}

		nmtv.hdr.code = bLeft ? NM_CLICK : NM_RCLICK;
		SendNotify(&nmtv.hdr);
		m_bActionDone = true;
	}
	else if (nDone == 2)
	{
		//drag
		SetItemState(hItem, TVIS_FOCUSED|TVIS_SELECTED, TVIS_FOCUSED|TVIS_SELECTED);
		if (!(dwStyle & TVS_DISABLEDRAGDROP))
		{
			nmtv.hdr.code = bLeft ? TVN_BEGINDRAG : TVN_BEGINRDRAG;
			nmtv.ptDrag = point;
			SendNotify(&nmtv.hdr);
		}
		m_bActionDone = true;
	}
}

void CXTTreeBase::DoBanding(UINT nFlags, CPoint point)
{
	if (::GetFocus() != m_pTreeCtrl->m_hWnd) {
		::SetFocus(m_pTreeCtrl->m_hWnd);
	}

	::SetCapture(m_pTreeCtrl->m_hWnd);

	CTypedPtrList<CPtrList, HTREEITEM> list;
	if (nFlags & (MK_SHIFT|MK_CONTROL)) {
		GetSelectedList(list);
	}

	CClientDC dc(m_pTreeCtrl);
	CRect rectCli;
	m_pTreeCtrl->GetClientRect(&rectCli);

	MSG msg;
	BOOL bDone = FALSE;
	CPoint pt;
	CSize sizeDrag(::GetSystemMetrics(SM_CXDRAG), ::GetSystemMetrics(SM_CYDRAG));
	BOOL bDrag = FALSE;
	CSize sizeEdge(1, 1);

	UINT_PTR nTimer = m_pTreeCtrl->SetTimer(1, SCROLL_TIMER_PERIOD, NULL);  //for scroll
	CPoint ptScr(m_pTreeCtrl->GetScrollPos(SB_HORZ), m_pTreeCtrl->GetScrollPos(SB_VERT));
	CRect rect(0, 0, 0, 0);
	UINT h = 0;
	HTREEITEM hItem = m_pTreeCtrl->GetRootItem();
	if (hItem)
	{
		m_pTreeCtrl->GetItemRect(hItem, &rect, FALSE);
		ptScr.y *= (h = rect.Height());     //this assumes equal height items
	}
	point += ptScr;

	while (!bDone && ::GetMessage(&msg, NULL, 0, 0))
	{
		if (::GetCapture() != m_pTreeCtrl->m_hWnd) {
			break;
		}

		switch (msg.message)
		{
			case WM_TIMER:
				{
					if (msg.wParam == SCROLL_TIMER_PERIOD)
					{
						pt = msg.pt;
						m_pTreeCtrl->ScreenToClient(&pt);
						if (rectCli.PtInRect(pt))
						{
							::DispatchMessage(&msg);
							break;
						}
						msg.lParam = MAKELPARAM(pt.x, pt.y);
					}
					else
					{
						break;
					}
				}
				//fall through to mouse move

			case WM_MOUSEMOVE:
				pt.x = GET_X_LPARAM(msg.lParam);
				pt.y = GET_Y_LPARAM(msg.lParam);
				if (!bDrag)
				{
					if ((abs(pt.x - point.x) <= sizeDrag.cx) &&
						((abs(pt.y - point.y) <= sizeDrag.cy))) {
						break;
					}
					bDrag = TRUE;
					if (!(nFlags & (MK_CONTROL|MK_SHIFT))) {
						SelectAll(FALSE);
					}
					m_pTreeCtrl->UpdateWindow();
					rect.SetRect(point, point);
					dc.DrawDragRect(rect, sizeEdge, NULL, sizeEdge);
				}

				dc.DrawDragRect(rect, sizeEdge, NULL, sizeEdge);    //delete

				if (pt.y < rectCli.top) {
					::SendMessage(m_pTreeCtrl->m_hWnd, WM_VSCROLL, SB_LINEUP, 0);
				}
				else if (pt.y >= rectCli.bottom) {
					::SendMessage(m_pTreeCtrl->m_hWnd, WM_VSCROLL, SB_LINEDOWN, 0);
				}
				if (pt.x < rectCli.left) {
					::SendMessage(m_pTreeCtrl->m_hWnd, WM_HSCROLL, SB_LINELEFT, 0);
				}
				else if (pt.x >= rectCli.right) {
					::SendMessage(m_pTreeCtrl->m_hWnd, WM_HSCROLL, SB_LINERIGHT, 0);
				}

				ptScr = point;
				ptScr.x -= m_pTreeCtrl->GetScrollPos(SB_HORZ);
				ptScr.y -= m_pTreeCtrl->GetScrollPos(SB_VERT) * h;
				rect.SetRect(ptScr, pt);
				rect.NormalizeRect();
				UpdateSelectionForRect(rect, nFlags, list);
				dc.DrawDragRect(rect, sizeEdge, NULL, sizeEdge);    //draw
				break;

			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
				bDone = TRUE;
				break;

			case WM_KEYDOWN:
				if (LOWORD(msg.wParam) == VK_ESCAPE)
				{
					SelectAll(FALSE);
					bDone = TRUE;
					break;
				}
				//dispatch

			default:
				::DispatchMessage(&msg);
				break;
		}
	}
	m_pTreeCtrl->KillTimer(nTimer);
	::ReleaseCapture();

	if (bDrag) {
		dc.DrawDragRect(rect, sizeEdge, NULL, sizeEdge);
	}
	else {
		if (!(nFlags & (MK_CONTROL|MK_SHIFT))) {
			SelectAll(FALSE);
		}
	}
}

void CXTTreeBase::UpdateSelectionForRect(LPCRECT pRect, UINT nFlags, CTypedPtrList<CPtrList, HTREEITEM>& list)
{
	CRect rect;
	BOOL bSel;
	POSITION pos;

	HTREEITEM hItem = m_pTreeCtrl->GetRootItem();
	while (hItem)
	{
		bSel = IsSelected(hItem);
		m_pTreeCtrl->GetItemRect(hItem, &rect, m_bBandLabel);
		if (rect.IntersectRect(rect, pRect))
		{
			//item in rect
			pos = list.Find(hItem);
			if (!bSel && !pos) {
				SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
			}
			else if ((nFlags & MK_CONTROL) && pos) {
				SetItemState(hItem, 0, TVIS_SELECTED);
			}
			else if ((nFlags & MK_SHIFT) && pos) {
				list.RemoveAt(pos);     //if shift and in rect, don't lock anymore
			}
		}
		else
		{
			//item not in rect
			pos = list.Find(hItem);
			if (bSel && !pos) {
				SetItemState(hItem, 0, TVIS_SELECTED);
			}
			else if (pos) {
				SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
			}
		}
		hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
	}
	m_pTreeCtrl->UpdateWindow();
}

BOOL CXTTreeBase::SelectChildren(HTREEITEM hParent, BOOL bSelect /*= TRUE*/, BOOL bRecurse /*= TRUE*/)
{
	UINT nS = bSelect ? TVIS_SELECTED : 0;

	BOOL bFocusWasInHere = FALSE;

	HTREEITEM hItem = GetNextItem(hParent, TVGN_CHILD);
	while (hItem)
	{
		UINT nState = GetItemState(hItem, TVIS_SELECTED|TVIS_EXPANDED|TVIS_FOCUSED);
		if ((nState & TVIS_SELECTED) != nS) {
			SetItemState(hItem, nS, TVIS_SELECTED);
		}
		bFocusWasInHere |= (nState & TVIS_FOCUSED);
		if (bRecurse && (nState & TVIS_EXPANDED)) {
			bFocusWasInHere |= SelectChildren(hItem, bSelect, bRecurse);
		}
		hItem = m_pTreeCtrl->GetNextSiblingItem(hItem);
	}
	return bFocusWasInHere;
}

void CXTTreeBase::GetSelectedList(CTypedPtrList<CPtrList, HTREEITEM>& list) const
{
	list.RemoveAll();

	HTREEITEM hItem = GetFirstSelectedItem();
	while (hItem)
	{
		list.AddTail(hItem);
		hItem = GetNextSelectedItem(hItem);
	}
}

BOOL CXTTreeBase::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	NMHDR* pNMHDR = (NMHDR*)lParam;

	switch(pNMHDR->code)
	{
	case TTN_SHOW:
		{
			// get a pointer to the tooltip control.
			HWND hWnd = TreeView_GetToolTips( m_pTreeCtrl->m_hWnd );
			if ( hWnd != NULL )
			{
				// make sure the tooltip is at the top of the "Z" order, otherwise
				// it will appear behind popup windows....
				::SetWindowPos( hWnd, HWND_TOP, 0,0,0,0,
					SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE );
			}
		}
		break;
	}

	return m_pTreeCtrl->CTreeCtrl::OnNotify(wParam, lParam, pResult);
}

BOOL CXTTreeBase::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		// ensure that keystrokes are handled by the edit control.
		if (HasEditLabels() && m_pTreeCtrl->GetEditControl())
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);

			return TRUE;
		}

		// toggle expand / contract when return key is hit.
		if (pMsg->wParam == VK_RETURN)
		{
			HTREEITEM htItem = m_pTreeCtrl->GetSelectedItem();
			if (htItem != NULL)
			{
				m_pTreeCtrl->Expand(htItem, TVE_TOGGLE);
			}
		}
	}

	return m_pTreeCtrl->CTreeCtrl::PreTranslateMessage(pMsg);
}

BOOL CXTTreeBase::OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	// Remove the tree item from the map.
	m_mapColorFont.RemoveKey(pNMTreeView->itemOld.hItem);
	*pResult = 0;

	return FALSE;
}

bool CXTTreeBase::HasEditLabels() const
{
	return ((m_pTreeCtrl->GetStyle() & TVS_EDITLABELS) == TVS_EDITLABELS);
}

void CXTTreeBase::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	m_pTreeCtrl->Default();

	// MFCBUG: During label editing if the tree is resized while
	// there is an edit box open the edit box is not moved to the
	// new location of the tree item, but is left in it previous
	// location.

	if (HasEditLabels() && m_htiEdit)
	{
		CEdit* pEdit = m_pTreeCtrl->GetEditControl();
		if (pEdit && ::IsWindow(pEdit->m_hWnd))
		{
			CRect rcEdit;
			pEdit->GetWindowRect(&rcEdit);

			CRect rcItem;
			m_pTreeCtrl->GetItemRect(m_htiEdit, &rcItem, TRUE);

			rcItem.top   -= 1;
			rcItem.left  -= 3;
			rcItem.right  = rcItem.left + rcEdit.Width();
			rcItem.bottom = rcItem.top + rcEdit.Height();

			pEdit->MoveWindow(&rcItem);
		}
	}
}

void CXTTreeBase::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
	if (m_pTreeCtrl->GetStyle() & TVS_TRACKSELECT)
	{
		TV_HITTESTINFO hit;
		::ZeroMemory(&hit,sizeof(hit));

		hit.pt = point;
		m_pTreeCtrl->HitTest( &hit );

		if( hit.hItem )
		{
			if( hit.flags & TVHT_ONITEM )
				m_pTreeCtrl->SetItemState( hit.hItem, TVIS_UNDERLINE, TVIS_UNDERLINE );
			else
				m_pTreeCtrl->SetItemState( hit.hItem, 0, TVIS_UNDERLINE );
		}

		if( m_htiLast != NULL && m_htiLast != hit.hItem )
			m_pTreeCtrl->SetItemState( m_htiLast, 0, TVIS_UNDERLINE );

		m_htiLast = hit.hItem;

		UINT_PTR uID = m_pTreeCtrl->SetTimer( HOVER_TIMER_PERIOD, 55, NULL );
		ASSERT( uID == HOVER_TIMER_PERIOD );
	}

	m_pTreeCtrl->Default();
}

void CXTTreeBase::OnTimer(UINT_PTR nIDEvent)
{
	if (m_pTreeCtrl->GetStyle() & TVS_TRACKSELECT)
	{
		if( nIDEvent == HOVER_TIMER_PERIOD )
		{
			m_pTreeCtrl->KillTimer(HOVER_TIMER_PERIOD);

			CPoint pt(::GetMessagePos());
			CRect rc;
			m_pTreeCtrl->GetWindowRect(&rc);

			if( rc.PtInRect(pt) == FALSE )
			{
				HTREEITEM hti = m_pTreeCtrl->GetFirstVisibleItem();

				while( hti != NULL )
				{
					if( m_pTreeCtrl->GetItemState( hti, TVIS_UNDERLINE ) & TVIS_UNDERLINE )
						m_pTreeCtrl->SetItemState( hti, 0, TVIS_UNDERLINE );
					hti = m_pTreeCtrl->GetNextVisibleItem( hti );
				}
			}
			return;
		}
	}

	m_pTreeCtrl->Default();
}

void CXTTreeBase::OnNcMouseMove(UINT nHitTest, CPoint /*point*/)
{
	if (m_pTreeCtrl->GetStyle() & TVS_TRACKSELECT)
	{
		if( nHitTest == HTBORDER )
		{
			HTREEITEM hti = m_pTreeCtrl->GetFirstVisibleItem();
			if( m_pTreeCtrl->GetItemState( hti, TVIS_UNDERLINE) & TVIS_UNDERLINE )
				m_pTreeCtrl->SetItemState( hti, 0, TVIS_UNDERLINE );
		}
	}

	m_pTreeCtrl->Default();
}
