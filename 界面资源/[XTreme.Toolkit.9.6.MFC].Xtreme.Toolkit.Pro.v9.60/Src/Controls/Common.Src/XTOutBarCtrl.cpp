// XTOutBarCtrl.cpp : implementation of the CXTOutBarCtrl class.
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
#include "XTPColorManager.h"
#include "XTPDrawHelpers.h"

#include "XTResource.h"
#include "XTDefines.h"
#include "XTUtil.h"
#include "XTGlobal.h"
#include "XTVC50Helpers.h"
#include "XTWndHook.h"
#include "XTFlatComboBox.h"
#include "XTOutBarCtrl.h"
#include "XTMemDC.h"
#include "XTOutBarTheme.h"
#include "XTFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTOutBarEditItem

CXTOutBarEditItem::CXTOutBarEditItem()
{
	m_iIndex      = -1;
	m_pParentWnd  = NULL;
	m_bSmallIcons = false;
	m_bEscapeKey  = false;
	m_bIsFolder   = false;
}

CXTOutBarEditItem::~CXTOutBarEditItem()
{

}

IMPLEMENT_DYNAMIC(CXTOutBarEditItem, CXTEdit)

BEGIN_MESSAGE_MAP(CXTOutBarEditItem, CXTEdit)
	//{{AFX_MSG_MAP(CXTOutBarEditItem)
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTOutBarEditItem::OnKillFocus(CWnd* /*pNewWnd*/)
{
	PostMessage( WM_CLOSE );

	if ( !m_bEscapeKey )
	{
		GetWindowText( m_strText );

		if ( !m_strText.IsEmpty( ) )
		{
			m_pParentWnd->EndLabelEdit( this, m_bIsFolder );
		}
	}
}

BOOL CXTOutBarEditItem::PreTranslateMessage(MSG* pMsg)
{
	switch ( pMsg->wParam )
	{
	case VK_ESCAPE:
		{
			m_bEscapeKey = TRUE; // fall thru
		}
	case VK_RETURN:
		{
			PostMessage( WM_CLOSE );
			return TRUE;
		}
	}

	return CXTEdit::PreTranslateMessage(pMsg);
}

void CXTOutBarEditItem::PostNcDestroy()
{
	delete this;
}

BOOL CXTOutBarEditItem::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, UINT nIndex, bool bIsFolder, bool bSmallIcons)
{
	ASSERT_VALID( pParentWnd ); // must be valid.

	m_pParentWnd = ( CXTOutBarCtrl* )pParentWnd;
	ASSERT_KINDOF( CXTOutBarCtrl, m_pParentWnd ); // must be an outlook bar control.

	// get the inside size of the outlook bar.
	CRect rcInside;
	m_pParentWnd->GetInsideRect( rcInside );
	rcInside.DeflateRect(5,5);

	// adjust the size if necessary.
	m_rcOriginal  = rect;
	m_bSmallIcons = bSmallIcons;
	m_bIsFolder   = bIsFolder;
	m_iIndex      = nIndex;
	m_strText     = lpszText;

	if ( m_rcOriginal.Width( ) > rcInside.Width( ) )
	{
		if ( m_bSmallIcons != TRUE )
		{
			m_rcOriginal.left  = rcInside.left;
		}

		m_rcOriginal.right = rcInside.right;
	}

	if (!CXTEdit::Create(dwStyle, m_rcOriginal, pParentWnd, nID))
		return FALSE;

	if (!CXTEdit::Initialize(m_pParentWnd))
		return FALSE;

	SetFont( m_pParentWnd->GetFontX( ) );
	SetWindowText( m_strText );

	return TRUE;
}

void CXTOutBarEditItem::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// if this is a folder, do default processing.
	if ( m_bIsFolder )
	{
		CXTEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}

	// if the escape or return key has been pressed, abort edit.
	else if ( nChar == VK_ESCAPE || nChar == VK_RETURN )
	{
		if ( nChar == VK_ESCAPE )
		{
			m_bEscapeKey = TRUE;
		}

		m_pParentWnd->SetFocus( );
		return;
	}

	CXTEdit::OnChar(nChar, nRepCnt, nFlags);

	// get the inside size of the outlook bar.
	CRect rcInside;
	m_pParentWnd->GetInsideRect( rcInside );
	rcInside.DeflateRect(5,5);

	// get the edit box text
	CString strText;
	GetWindowText( strText );

	// get the font used by the outlook bar.
	CFont* pFont = m_pParentWnd->GetFontX( );
	ASSERT( pFont && pFont->GetSafeHandle( ) );

	// select the font used by the outlook bar.
	CWindowDC dc( NULL );

	// get the size of the text string.
	CFont* f = dc.SelectObject( pFont );
	CSize sz = dc.GetTextExtent( strText );
	dc.SelectObject( f );

	// construct the edit box size.
	CRect rcItem = m_rcOriginal;

	// calculate the width of the edit box.
	if ( m_bSmallIcons == TRUE )
	{
		rcItem.right = rcItem.left + sz.cx + 9;

		if ( rcItem.right < m_rcOriginal.right )
		{
			rcItem.right = m_rcOriginal.right;
		}
		else if ( rcItem.right > rcInside.right )
		{
			rcItem.right = rcInside.right;
		}
	}
	else
	{
		int x = ( rcInside.Width( )-sz.cx )/2;

		rcItem.left  = rcInside.left + x;
		rcItem.right = rcItem.left  + sz.cx;
		rcItem.InflateRect( 6,0 );

		if ( rcItem.Width( ) < m_rcOriginal.Width( ) )
		{
			rcItem = m_rcOriginal;
		}

		else if ( rcItem.Width( ) > rcInside.Width( ) )
		{
			rcItem.left  = rcInside.left;
			rcItem.right = rcInside.right;
		}
	}

	// resize the edit box.
	MoveWindow( &rcItem );
}

BOOL CXTOutBarEditItem::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	HCURSOR hCursor = ::LoadCursor( NULL, IDC_IBEAM );
	if ( hCursor != NULL )
	{
		::SetCursor( hCursor );
		return TRUE;
	}

	return CXTEdit::OnSetCursor(pWnd, nHitTest, message);
}

/////////////////////////////////////////////////////////////////////////////
// CXTOutBarCtrl

IMPLEMENT_THEME_HOST(CXTOutBarCtrl, CXTOutBarTheme)

CXTOutBarCtrl::CXTOutBarCtrl()
: CXTThemeManagerStyleHost(GetThemeFactoryClass())
, m_penBlack(PS_SOLID, 1, RGB(0,0,0))
{
	m_rcUpArrow.SetRectEmpty();
	m_rcDownArrow.SetRectEmpty();

	m_dwFlags               = OBS_XT_DEFAULT;
	m_pLargeImageList       = NULL;
	m_pSmallImageList       = NULL;
	m_sizeOffset.cx         = 4;
	m_sizeOffset.cy         = 3;
	m_sizeMargin.cx         = 5;
	m_sizeMargin.cy         = 5;
	m_nFolderHeight         = 22;
	m_nSelFolder            = 0;
	m_nLastFolderSelected   = -1;
	m_nFolderHilighted      = -1;
	m_nLastItemSelected     = -1;
	m_nItemHilighted        = -1;
	m_nLastDragItemDraw     = -1;
	m_nLastDragItemDrawType = -1;
	m_nFirstItem            = 0;
	m_nIconSpacingSmall     = 10;
	m_nIconSpacingLarge     = 8;
	m_nAnimationTickCount   = 10;
	m_nSelAnimCount         = 0;
	m_nSelAnimTiming        = 0;
	m_nHitInternal1         = 0;
	m_nHitInternal2         = 0;
	m_bUpArrow              = FALSE;
	m_bDownArrow            = FALSE;
	m_bUpPressed            = FALSE;
	m_bDownPressed          = FALSE;
	m_bLooping              = FALSE;
	m_bPressedHighlight     = FALSE;
	m_bIconPressed          = FALSE;

	SetFontX(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
}

CXTOutBarCtrl::~CXTOutBarCtrl()
{
	int iFolder;
	for (iFolder = 0; iFolder < m_arFolder.GetSize(); iFolder++)
	{
		m_nSelFolder = iFolder;
		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(iFolder);
		SAFE_DELETE( pBarFolder );
	}

	m_arFolder.RemoveAll();
	m_penBlack.DeleteObject();
	m_font.DeleteObject();

}

IMPLEMENT_DYNCREATE(CXTOutBarCtrl, CWnd)

BEGIN_MESSAGE_MAP(CXTOutBarCtrl, CWnd)
	//{{AFX_MSG_MAP(CXTOutBarCtrl)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_COMMAND(XT_IDC_LARGEICON, OnLargeIcon)
	ON_UPDATE_COMMAND_UI(XT_IDC_LARGEICON, OnUpdateLargeIcon)
	ON_COMMAND(XT_IDC_SMALLICON, OnSmallIcon)
	ON_UPDATE_COMMAND_UI(XT_IDC_SMALLICON, OnUpdateSmallIcon)
	ON_COMMAND(XT_IDC_REMOVEITEM, OnRemoveItem)
	ON_UPDATE_COMMAND_UI(XT_IDC_REMOVEITEM, OnUpdateRemoveItem)
	ON_COMMAND(XT_IDC_RENAMEITEM, OnRenameItem)
	ON_UPDATE_COMMAND_UI(XT_IDC_RENAMEITEM, OnUpdateRenameItem)
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CXTOutBarCtrl::DrawItemIcon( CDC* pDC, CPoint pt, CXTOutBarItem* pBarItem, CImageList* pImageList)
{
	GetTheme()->DrawItemIcon(pDC, pt, pBarItem, pImageList);
}

void CXTOutBarCtrl::DrawItemText( CDC* pDC, CRect rc, CXTOutBarItem* pBarItem, UINT nFormat)
{
	GetTheme()->DrawItemText(pDC, rc, pBarItem, nFormat);
}

void CXTOutBarCtrl::EndLabelEdit(CXTOutBarEditItem* pEdit, bool bIsFolder)
{
	ASSERT_VALID(pEdit); // must be valid.

	XT_OUTBAR_INFO obi;
	obi.bFolder   = bIsFolder;
	obi.nIndex    = pEdit->GetIndex();
	obi.lpszText  = pEdit->GetText();
	obi.nDragFrom = -1;
	obi.nDragTo   = -1;

	CWnd* pOwner = ( CWnd* )GetOwner( );
	ASSERT_VALID( pOwner );

	LRESULT lResult = 0;

	// give the owner a chance to process the message first.
	if ( bIsFolder )
	{
		lResult = pOwner->SendMessage( XTWM_OUTBAR_NOTIFY,
			OBN_XT_ONGROUPENDEDIT, ( LPARAM )&obi );
	}
	else
	{
		lResult = pOwner->SendMessage( XTWM_OUTBAR_NOTIFY,
			OBN_XT_ONLABELENDEDIT, ( LPARAM )&obi );
	}

	// If the return value is "0" then the owner has processed
	// the message, so just return.
	if ( lResult == 0 )
	{
		return;
	}

	ASSERT( m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount( ) );
	CXTOutBarFolder* pBarFolder = ( CXTOutBarFolder* )m_arFolder.GetAt( m_nSelFolder );
	if ( pBarFolder == NULL )
	{
		return;
	}

	CRect rc;
	bool  bUpdate = false;

	if ( bIsFolder )
	{
		bUpdate = ( pBarFolder->GetName( ) != obi.lpszText );

		if ( bUpdate )
		{
			GetFolderRect( obi.nIndex, rc );
			SetFolderText( obi.nIndex, obi.lpszText );
		}
	}
	else
	{
		ASSERT( obi.nIndex >= 0 && obi.nIndex < pBarFolder->GetItemCount( ) );
		CXTOutBarItem* pBarItem = pBarFolder->GetItemAt( obi.nIndex );
		if ( pBarItem == NULL )
		{
			return;
		}

		bUpdate = ( pBarItem->GetName( ) != obi.lpszText );

		if ( bUpdate )
		{
			GetInsideRect( rc);
			SetItemText( obi.nIndex, obi.lpszText );
		}
	}

	if ( bUpdate )
	{
		OnLabelChanged( &obi ); // handle in derived class.
		InvalidateRect( rc );
	}
}

void CXTOutBarCtrl::OnLabelChanged(const XT_OUTBAR_INFO* pObi)
{
	UNREFERENCED_PARAMETER( pObi );
}

void CXTOutBarCtrl::SetFolderText( const int iIndex, LPCTSTR lpszFolderName )
{
	if (m_arFolder.GetSize() != 0)
	{
		ASSERT((iIndex >= 0) && (iIndex < GetFolderCount()));
		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(iIndex);
		pBarFolder->SetName( lpszFolderName );
	}
}

void CXTOutBarCtrl::SetItemText( const int iIndex, LPCTSTR lpszItemName )
{
	if (m_arFolder.GetSize() != 0)
	{
		ASSERT(m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount());

		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(m_nSelFolder);

		CXTOutBarItem* pBarItem = pBarFolder->GetItemAt(iIndex);
		pBarItem->SetName( lpszItemName );
	}
}

BOOL CXTOutBarCtrl::Create(DWORD dwStyle, const RECT&  rect, CWnd* pParentWnd, UINT nID, const DWORD dwFlags)
{
	if(!CreateEx(NULL, dwStyle, rect, pParentWnd, nID, dwFlags)) {
		return FALSE;
	}

	return TRUE;
}

BOOL CXTOutBarCtrl::CreateEx(DWORD dwExStyle, DWORD dwStyle, const RECT&  rect, CWnd* pParentWnd, UINT nID, const DWORD dwFlags)
{
	ASSERT(dwStyle & WS_CHILD);
	ASSERT_VALID(pParentWnd);

	if(!CWnd::CreateEx(dwExStyle, NULL, NULL, dwStyle, rect, pParentWnd, nID)) {
		return FALSE;
	}

	m_dwFlags = dwFlags;

	return TRUE;
}

BOOL CXTOutBarCtrl::IsSmallIconView(const int iFolder/*=-1*/) const
{
	if (GetFolderCount() <= 0 || iFolder == -1)
		return ((m_dwFlags & OBS_XT_SMALLICON)==OBS_XT_SMALLICON);

	CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(iFolder);
	return pBarFolder->m_bSmallIcons;
}

void CXTOutBarCtrl::SetSmallIconView(const BOOL bSet, const int iFolder/*=-1*/)
{
	m_nFirstItem = 0;

	if (iFolder != -1)
	{
		if (iFolder >= 0 && iFolder < GetFolderCount())
		{
			CXTOutBarFolder* pbf = (CXTOutBarFolder*)m_arFolder.GetAt(iFolder);
			pbf->m_bSmallIcons = bSet;
		}
	}
	else
	{
		// do all current folders, and set flag so new folders
		// have the chosen style
		int nFolders = GetFolderCount();
		int i;
		for (i = 0; i < nFolders; i++)
		{
			CXTOutBarFolder* pbf = (CXTOutBarFolder*)m_arFolder.GetAt(i);
			pbf->m_bSmallIcons = bSet;
		}

		if (bSet && ! IsSmallIconView())
			m_dwFlags |= OBS_XT_SMALLICON;

		else if (! bSet && IsSmallIconView())
			m_dwFlags &= ~OBS_XT_SMALLICON;
	}

	CRect rc;
	GetInsideRect(rc);
	InvalidateRect(rc, false);
}

DWORD CXTOutBarCtrl::GetFlag() const
{
	return m_dwFlags;
}

void CXTOutBarCtrl::ModifyFlag(const DWORD& dwRemove, const DWORD& dwAdd, const bool bRedraw)
{
	if (dwRemove) {
		m_dwFlags &= ~dwRemove;
	}
	if (dwAdd) {
		m_dwFlags |= dwAdd;
	}
	if (GetSafeHwnd())
	{
		if (bRedraw == true)
		{
			Invalidate();
			UpdateWindow();
		}
	}
}

void CXTOutBarCtrl::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 3 && m_nLastItemSelected >= 0)
	{
		m_nSelAnimCount ++;
		if (m_nSelAnimCount > 10) m_nSelAnimCount = -1;
		if (m_nSelAnimCount == 0)
			DrawAnimItem(-1, 1, m_nLastItemSelected);
		if (m_nSelAnimCount == 1)
			DrawAnimItem(0, 0, m_nLastItemSelected);
		if (m_nSelAnimCount == 2)
			DrawAnimItem(1, 1, m_nLastItemSelected);
		if (m_nSelAnimCount == 3)
			DrawAnimItem(0, 0, m_nLastItemSelected);
	}
	CWnd::OnTimer(nIDEvent);
}

void CXTOutBarCtrl::DrawAnimItem(const int iOffsetX, const int iOffsetY, const int iIndex)
{
	CImageList* pImageList = GetFolderImageList(m_nSelFolder, IsSmallIconView(m_nSelFolder));

	CRect rc, irc;
	GetInsideRect(irc);
	GetItemRect(m_nSelFolder, m_nLastItemSelected, rc);

	if (m_arFolder.GetSize() != 0)
	{
		if (m_nLastItemSelected >= m_nFirstItem && irc.bottom > rc.bottom && irc.top < rc.top)
		{
			ASSERT(m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount());
			CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(m_nSelFolder);

			ASSERT(iIndex >= 0 && iIndex < pBarFolder->GetItemCount());
			CXTOutBarItem* pBarItem = (CXTOutBarItem*)pBarFolder->GetItemAt(iIndex);

			ASSERT(pBarItem && pImageList);

			CClientDC dc(this);

			if (IsSmallIconView(m_nSelFolder))
			{
				if (pImageList)
				{
					IMAGEINFO ii;
					pImageList->GetImageInfo(pBarItem->GetIndex(), &ii);
					CSize szImage = CRect(ii.rcImage).Size();
					CPoint pt;
					pt.x = rc.left + 2;
					pt.y = rc.top + (rc.Height() - szImage.cy) / 2;

					CRect rcBck(pt.x-1, pt.y-1, pt.x + szImage.cx+2, pt.y + szImage.cy+2);
					dc.FillSolidRect(rcBck, GetXtremeColor(COLOR_3DSHADOW));

					pt.x += iOffsetX;
					pt.y += iOffsetY;

					DrawItemIcon( &dc, pt, pBarItem, pImageList);
				}
			}
			else
			{
				if (pImageList)
				{
					IMAGEINFO ii;
					pImageList->GetImageInfo(pBarItem->GetIndex(), &ii);
					CSize szImage = CRect(ii.rcImage).Size();
					CPoint pt;
					pt.x = rc.left + (rc.Width() - szImage.cx) / 2;
					pt.y = rc.top;

					CRect rcBck(pt.x-1, pt.y-1, pt.x + szImage.cx+2, pt.y + szImage.cy+2);
					dc.FillSolidRect(rcBck, GetXtremeColor(COLOR_3DSHADOW));

					pt.x += iOffsetX;
					pt.y += iOffsetY;

					DrawItemIcon( &dc, pt, pBarItem, pImageList);
				}
			}
		}
	}
}

void CXTOutBarCtrl::OnPaint()
{
	// background is already filled in gray
	CPaintDC dc(this);

	// Get the client rect.
	CRect rectClient;
	GetClientRect(&rectClient);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTMemDC memDC(&dc, rectClient);

	CRect rc;
	GetInsideRect(rc);
	if (!GetFolderChild())
	{
		//memDC.FillSolidRect(rc, m_clrBack);
		GetTheme()->FillInsideRect(&memDC, rc);
	}

	int max = (int)m_arFolder.GetSize();
	CRect frc;
	int t;
	for (t = 0; t < max; t++)
	{
		GetFolderRect(t, frc);
		DrawFolder(&memDC, t, frc, xtMouseNormal);
	}
	if (!GetFolderChild())
	{
		int f,l;
		GetVisibleRange(m_nSelFolder, f,l);
		m_rcUpArrow.SetRect(0,0,GetSystemMetrics(SM_CXVSCROLL), GetSystemMetrics(SM_CXVSCROLL));
		m_rcDownArrow = m_rcUpArrow;
		m_rcUpArrow.OffsetRect(rc.right - 5 - GetSystemMetrics(SM_CXVSCROLL), rc.top +5);
		m_rcDownArrow.OffsetRect(rc.right - 5 - GetSystemMetrics(SM_CXVSCROLL), rc.bottom - 5 - GetSystemMetrics(SM_CXVSCROLL));

		if (f > 0)
		{
			if (m_bUpPressed)
			{
				DrawScrollButton( &memDC, m_rcUpArrow, DFC_SCROLL, DFCS_SCROLLUP|DFCS_PUSHED );
			}
			else
			{
				DrawScrollButton( &memDC, m_rcUpArrow, DFC_SCROLL, DFCS_SCROLLUP );
			}
			m_bUpArrow = TRUE;
			memDC.ExcludeClipRect(m_rcUpArrow);
		}
		else m_bUpArrow = FALSE;

		if (l < GetItemCount() - 1)
		{
			if (m_bDownPressed)
			{
				DrawScrollButton( &memDC, m_rcDownArrow, DFC_SCROLL, DFCS_SCROLLDOWN|DFCS_PUSHED );
			}
			else
			{
				DrawScrollButton( &memDC, m_rcDownArrow, DFC_SCROLL, DFCS_SCROLLDOWN );
			}
			m_bDownArrow = TRUE;
			memDC.ExcludeClipRect(m_rcDownArrow);
		}
		else m_bDownArrow = FALSE;

		PaintItems(&memDC, m_nSelFolder, rc);
	}

	//dc.BitBlt(rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), &memDC, 0,0, SRCCOPY);

	//memDC.RestoreDC(nSavedDC);

	if (m_nFolderHilighted >= 0)
	{
		int i = m_nFolderHilighted;
		m_nFolderHilighted = -1;
		HighlightFolder(&memDC, i);
	}
	if (m_nItemHilighted >= 0)
	{
		int i = m_nItemHilighted;
		m_nItemHilighted = -1;
		HighlightItem(&memDC, i, m_bPressedHighlight);
	}
}

BOOL CXTOutBarCtrl::OnEraseBkgnd(CDC* pDC)
{
	UNUSED_ALWAYS(pDC);
	return TRUE;
}

BOOL CXTOutBarCtrl::GetFolderRect(const int iIndex, CRect&  rect) const
{
	if (m_arFolder.GetSize() != 0)
	{
		int max = (int)m_arFolder.GetSize();
		ASSERT(iIndex >= 0 && iIndex < max);

		if (iIndex >= 0 && iIndex < max)
		{
			CRect rc;
			GetClientRect(rc);
			if (iIndex > m_nSelFolder)
				rect.SetRect(rc.left, rc.bottom - ((max - iIndex))* m_nFolderHeight, rc.right,
				rc.bottom - (max - iIndex - 1)* m_nFolderHeight);
			else
				rect.SetRect(rc.left, rc.top + iIndex* m_nFolderHeight, rc.right,
				rc.top + (1 + iIndex)* m_nFolderHeight);
			return TRUE;
		}
	}
	return FALSE;
}

void CXTOutBarCtrl::GetItemRect(const int iFolder, const int iIndex, CRect&  rect, CRect* pInsideRect /*= NULL*/)
{
	CRect rc;
	if (pInsideRect) rc = *pInsideRect; else GetInsideRect(rc);

	int top = rc.top;
	CSize sz(0,0);
	int y = 0;
	int t;
	for (t = 0; t < iIndex; t++)
	{
		sz = GetItemSize(iFolder, t, rectItemBoth);
		top += sz.cy;

		if (IsSmallIconView(iFolder))
		{
			top += m_nIconSpacingSmall;
		}
		else
		{
			top += m_nIconSpacingLarge;
		}
		if (t == m_nFirstItem - 1) y = top - rc.top;
	}
	sz = GetItemSize(iFolder, iIndex, rectItemBoth);
	rect.SetRect(rc.left, top, rc.left + sz.cx, top + sz.cy);

	rect.top -= y;
	rect.bottom -= y;

	rect.left += m_sizeMargin.cx;
	rect.top  += m_sizeMargin.cy;
	rect.bottom += m_sizeMargin.cy;

	if (!IsSmallIconView(iFolder))
	{
		rect.left = rc.left;
		rect.right = rc.right;
	}
}

void CXTOutBarCtrl::DrawFolder(CDC* pDC, const int iIndex, CRect rect, const XTMouseState eHilight)
{
	if (iIndex < m_arFolder.GetSize() && (iIndex >= 0))
	{
		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(iIndex);

		GetTheme()->DrawFolder(pDC, rect, pBarFolder, eHilight);
	}
}

int CXTOutBarCtrl::AddFolder( LPCTSTR lpszFolderName, const DWORD dwData )
{
	CXTOutBarFolder* pBarFolder = new CXTOutBarFolder(lpszFolderName, dwData, this);
	ASSERT(pBarFolder);

	pBarFolder->m_bSmallIcons = (m_dwFlags & OBS_XT_SMALLICON);

	return (int)m_arFolder.Add((void*)pBarFolder);
}

void CXTOutBarCtrl::GetInsideRect(CRect&  rect) const
{
	GetClientRect(rect);
	if (m_arFolder.GetSize() > 0)
	{
		int max = (int)m_arFolder.GetSize();
		rect.top += m_nFolderHeight* (m_nSelFolder + 1);
		rect.bottom -= (max - m_nSelFolder - 1) * m_nFolderHeight;
		return;
	}
}

void CXTOutBarCtrl::OnMouseLeave()
{
	OnMouseMove(0, CPoint(-1, -1));
}

void CXTOutBarCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	int iIndex;
	int ht = HitTestEx(point, iIndex);

	int nFolderHilighted = -1, nItemHilighted = -1;

	if (ht == hitFolder) nFolderHilighted = iIndex;
	if (ht == hitItem) nItemHilighted = iIndex;

	if ((m_nFolderHilighted != nFolderHilighted) || (m_nItemHilighted != nItemHilighted))
	{
		m_nFolderHilighted = nFolderHilighted;
		m_nItemHilighted = nItemHilighted;
		Invalidate(FALSE);

		if (nFolderHilighted != -1 || nItemHilighted != -1)
		{
			TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd};
			_TrackMouseEvent(&tme);
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

CXTOutBarCtrl::HitTestCode CXTOutBarCtrl::HitTestEx(const CPoint&  point, int& iIndex)
{
	if (m_arFolder.GetSize() != 0)
	{
		if (m_bUpArrow && m_rcUpArrow.PtInRect(point)) return hitUpScroll;
		if (m_bDownArrow && m_rcDownArrow.PtInRect(point)) return hitDnScroll;

		int max = (int)m_arFolder.GetSize(), t;

		CRect rc;
		for (t = 0; t < max; t++)
		{
			GetFolderRect(t, rc);
			if (rc.PtInRect(point))
			{
				iIndex = t;
				return hitFolder;
			}
		}
		GetInsideRect(rc);
		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(m_nSelFolder);
		max = pBarFolder->GetItemCount();
		for (t = m_nFirstItem; t < max; t++)
		{
			CRect rcItem;
			if (!IsSmallIconView(m_nSelFolder))
			{
				GetIconRect(m_nSelFolder, t, rcItem);
				if (rcItem.PtInRect(point))
				{
					iIndex = t;
					return hitItem;
				}
				else if (rcItem.top > rc.bottom) break;
				GetLabelRect(m_nSelFolder, t, rcItem);
				rcItem.top -= m_sizeOffset.cy;

				if (rcItem.PtInRect(point))
				{
					iIndex = t;
					return hitItem;
				}
				else if (rcItem.top > rc.bottom) break;
			}
			else
			{
				GetItemRect(m_nSelFolder, t, rcItem);
				if (rcItem.PtInRect(point))
				{
					iIndex = t;
					return hitItem;
				}
				else if (rcItem.top > rc.bottom) break;
			}
		}
	}
	return hitNone;
}

void CXTOutBarCtrl::HighlightFolder(CDC* pDC, const int iIndex)
{
	CWnd* pFocus = GetFocus();
	if (pFocus != NULL && pFocus != this && IsChild(pFocus)) {
		return;
	}

	if (m_nFolderHilighted == iIndex)
		return;

	CDC dcClient;
	if (!pDC)
	{
		dcClient.Attach(::GetDC(m_hWnd));
		pDC = &dcClient;
	}

	if (m_nFolderHilighted >= 0)
	{
		CRect rc;
		if (GetFolderRect(m_nFolderHilighted, rc))
		{
			DrawFolder(pDC, m_nFolderHilighted, rc, xtMouseNormal);
		}
	}

	if (iIndex >= 0)
	{
		CRect rc;
		if (GetFolderRect(iIndex, rc))
		{
			DrawFolder(pDC, iIndex, rc, xtMouseHover);
		}
	}
	if (dcClient.GetSafeHdc())
	{
		::ReleaseDC(m_hWnd, dcClient.Detach());
	}

	m_nFolderHilighted = iIndex;
}

void CXTOutBarCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (GetFocus() != this) SetFocus();

	int iIndex, ht = HitTestEx(point, iIndex);
	m_nLastDragItemDrawType = -1;

	CRect inRc;
	GetInsideRect(inRc);

	if (ht == hitFolder)
	{
		BOOL bHigh = TRUE;
		CRect rc;
		GetFolderRect(iIndex, rc);

		if (::GetCapture() == NULL)
		{
			SetCapture();
			ASSERT(this == GetCapture());
			CClientDC dc(this);
			DrawFolder(&dc, iIndex, rc, xtMouseSelect);
			AfxLockTempMaps();
			for (;;)
			{
				MSG msg;
				VERIFY(::GetMessage(&msg, NULL, 0, 0));

				if (CWnd::GetCapture() != this) break;

				switch (msg.message)
				{
				case WM_MOUSEMOVE:
					{
						CPoint pt(msg.lParam);
						int idx, ht1 = HitTestEx(pt, idx);
						if (ht1 == hitFolder && idx == iIndex)
						{
							if (!bHigh)
							{
								DrawFolder(&dc, iIndex, rc, xtMouseSelect);
								bHigh = TRUE;
							}
						}
						else
						{
							if (bHigh)
							{
								DrawFolder(&dc, iIndex, rc, xtMouseNormal);
								bHigh = FALSE;
							}
						}
					}
					break;

				case WM_LBUTTONUP:
					{
						ReleaseCapture();

						if (bHigh)
						{
							DrawFolder(&dc, iIndex, rc, xtMouseNormal);
							bHigh = FALSE;
						}
						CPoint pt(msg.lParam);
						int idx, ht1 = HitTestEx(pt, idx);
						if (ht1 == hitFolder && idx != m_nSelFolder)
							SetSelFolder(idx);
					}

					goto ExitLoop2;

				case WM_KEYDOWN:
					if (msg.wParam != VK_ESCAPE)
						break;

				default:
					DispatchMessage(&msg);
					break;
				}
			}

		ExitLoop2:
			ReleaseCapture();
			AfxUnlockTempMaps(FALSE);
		}
		if (bHigh) InvalidateRect(rc, FALSE);
	}
	else m_nLastFolderSelected = -1;

	if (ht == hitItem)
	{
		m_nLastDragItemDraw = -1;
		BOOL bHigh = TRUE, bDragging = FALSE;
		CRect rc;
		GetItemRect(m_nSelFolder, iIndex, rc);

		HCURSOR hCur = GetCursor();

		if (::GetCapture() == NULL)
		{
			SetCapture();
			ASSERT(this == GetCapture());
			CClientDC dc(this);
			HighlightItem(NULL, iIndex, TRUE);
			AfxLockTempMaps();
			for (;;)
			{
				MSG msg;
				VERIFY(::GetMessage(&msg, NULL, 0, 0));

				if (CWnd::GetCapture() != this) break;

				switch (msg.message)
				{
				case WM_MOUSEMOVE:
					{
						CPoint pt(msg.lParam);
						int idx, ht1 = HitTestEx(pt, idx);
						if (bDragging)
						{
							if (ht1 == hitItem)
							{
								DrawDragArrow(&dc, iIndex, idx);
								::SetCursor(XTAuxData().hcurDragMove);
								hCur = XTAuxData().hcurDragMove;
							}
							else
							{
								CRect rcItem;
								GetItemRect(m_nSelFolder, GetItemCount() - 1, rcItem);
								if (pt.y > rcItem.bottom && pt.y < inRc.bottom)
								{
									DrawDragArrow(&dc, iIndex, GetItemCount());
									::SetCursor(XTAuxData().hcurDragMove);
									hCur = XTAuxData().hcurDragMove;
								}
								else
								{
									DrawDragArrow(&dc, iIndex, -1);
									::SetCursor(XTAuxData().hcurDragNone);
									hCur = XTAuxData().hcurDragNone;
								}
							}
						}
						else
						{
							if (ht1 == hitItem && idx == iIndex)
							{
								if (!bHigh)
								{
									HighlightItem(NULL, iIndex, TRUE);
									bHigh = TRUE;
									m_bPressedHighlight = TRUE;
								}
							}
							else
							{
								if (ht1 == hitItem)
								{
									if (bHigh)
									{
										HighlightItem(NULL, iIndex, FALSE);
										bHigh = FALSE;
										m_bPressedHighlight = FALSE;
									}
								}
								else
								{
									if (m_dwFlags & OBS_XT_DRAGITEMS)
									{
										HighlightItem(NULL, iIndex, TRUE);
										bHigh = TRUE;
										bDragging = TRUE;

										XT_OUTBAR_INFO obi;
										obi.nIndex    = iIndex;
										obi.nDragFrom = -1;
										obi.nDragTo   = -1;
										obi.lpszText  = GetItemText(iIndex);
										obi.bFolder   = false;

										CWnd* pOwner = GetOwner();
										ASSERT_VALID(pOwner);

										pOwner->SendMessage(XTWM_OUTBAR_NOTIFY,
											OBN_XT_BEGINDRAG, (LPARAM)&obi);

										::SetCursor(XTAuxData().hcurDragMove);
										hCur = XTAuxData().hcurDragMove;

										m_bPressedHighlight = TRUE;
									}
								}
							}
						}
					}
					break;

				case WM_SETCURSOR:
					::SetCursor(hCur);
					break;

				case WM_LBUTTONUP:
					{
						ReleaseCapture();

						if (bHigh)
						{
							HighlightItem(NULL, -1);
							bHigh = FALSE;
						}
						CPoint pt(msg.lParam);
						int idx, ht1 = HitTestEx(pt, idx);
						if (!bDragging)
						{
							if (ht1 == hitItem && idx == iIndex)
							{
								CXTOutBarItem* pBarItem = GetBarFolderItem( m_nSelFolder, idx );
								if ( !pBarItem->IsEnabled( ) ) {
									return;
								}

								XT_OUTBAR_INFO obi;
								obi.nIndex    = idx;
								obi.nDragFrom = -1;
								obi.nDragTo   = -1;
								obi.lpszText  = GetItemText(idx);
								obi.bFolder   = false;

								CWnd* pOwner = GetOwner();
								ASSERT_VALID(pOwner);

								int nSelFolder = m_nSelFolder;

								LRESULT lResult = pOwner->SendMessage(XTWM_OUTBAR_NOTIFY,
									OBN_XT_ITEMCLICK, (LPARAM)&obi);

								if (lResult == TRUE && nSelFolder == GetSelFolder( ))
								{
									CXTOutBarFolder* pBarFolder = GetBarFolder( GetSelFolder( ) );
									pBarFolder->SetSelItem( idx );

									if (m_nSelAnimTiming > 0 && iIndex != m_nLastItemSelected && m_nLastItemSelected >= 0)
									{
										DrawAnimItem(0, 0, m_nLastItemSelected);
									}

									BOOL bInvalidate = FALSE;

									if ((m_dwFlags & OBS_XT_SELHIGHLIGHT) && (m_nLastItemSelected >= 0))
									{
										bInvalidate = TRUE;
									}

									m_nLastItemSelected = iIndex;

									if ((m_dwFlags & OBS_XT_SELHIGHLIGHT) && (m_nLastItemSelected >= 0))
									{
										bInvalidate = TRUE;
									}

									if (bInvalidate)
									{
										Invalidate(FALSE);
									}
								}
							}
						}
						else
						{
							if (ht1 == hitItem)
							{
								if (idx != iIndex)
								{
									int iItemSelected = m_nLastItemSelected;

									CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(m_nSelFolder);
									POSITION pos1 = pBarFolder->m_barItems.FindIndex(iIndex);

									CXTOutBarItem* pItemFrom = (CXTOutBarItem*)pBarFolder->m_barItems.GetAt(pos1);
									POSITION pos2 = pBarFolder->m_barItems.FindIndex(idx);

									XT_OUTBAR_INFO obi;
									obi.nIndex    = iIndex;
									obi.nDragFrom = iIndex;
									obi.nDragTo   = idx;
									obi.lpszText  = GetItemText(iIndex);
									obi.bFolder   = false;

									CWnd* pOwner = GetOwner();
									ASSERT_VALID(pOwner);

									LRESULT lResult = pOwner->SendMessage(XTWM_OUTBAR_NOTIFY,
										OBN_XT_DRAGITEM, (LPARAM)&obi);

									if (lResult == TRUE)
									{
										if (m_arFolder.GetSize() != 0)
										{
											pBarFolder->m_barItems.RemoveAt(pos1);
											pBarFolder->m_barItems.InsertBefore(pos2, pItemFrom);
										}

										int i = -1;
										if ( iItemSelected == iIndex )
										{
											i = idx;

											if ( idx > iIndex )
											{
												i--;
											}
										}

										else if ( ( ( iIndex <= m_nLastItemSelected ) && ( idx <= m_nLastItemSelected ) ) ||
												( ( iIndex > m_nLastItemSelected ) && ( idx > m_nLastItemSelected ) ) )
										{
											// do nothing...
										}

										else if ( m_nLastItemSelected != -1 )
										{
											i = m_nLastItemSelected;

											if ( idx > m_nLastItemSelected )
											{
												i--;
											}
											else
											{
												i++;
											}
										}

										if ( i >= 0 )
										{
											pBarFolder->SetSelItem( i );
											m_nLastItemSelected = i;
										}
									}
								}
							}
							else
							{
								CRect rcItem;
								GetItemRect(m_nSelFolder, GetItemCount() - 1, rcItem);
								if (pt.y > rcItem.bottom && pt.y < inRc.bottom)
								{
									CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt( m_nSelFolder );
									POSITION pos1 = pBarFolder->m_barItems.FindIndex(iIndex);

									CXTOutBarItem* pItemFrom = (CXTOutBarItem*)pBarFolder->m_barItems.GetAt( pos1 );
									POSITION pos2 = pBarFolder->m_barItems.FindIndex(pBarFolder->m_barItems.GetCount());

									XT_OUTBAR_INFO obi;
									obi.nIndex    = (int)pBarFolder->m_barItems.GetCount();
									obi.nDragFrom = iIndex;
									obi.nDragTo   = obi.nIndex;
									obi.lpszText  = GetItemText(iIndex);
									obi.bFolder   = false;

									CWnd* pOwner = GetOwner();
									ASSERT_VALID(pOwner);

									LRESULT lResult = pOwner->SendMessage(XTWM_OUTBAR_NOTIFY,
										OBN_XT_DRAGITEM, (LPARAM)&obi);

									if (lResult == TRUE)
									{
										pBarFolder->m_barItems.RemoveAt( pos1 );
										pBarFolder->m_barItems.InsertAfter( pos2, pItemFrom );
									}
								}
							}
						}
					}

					goto ExitLoop4;

				default:
					DispatchMessage(&msg);
					break;
				}
			}

		ExitLoop4:
			ReleaseCapture();
			AfxUnlockTempMaps(FALSE);
			if (bDragging)
			{
				Invalidate();
			}
		}
		m_bPressedHighlight = FALSE;
		if (bHigh) InvalidateRect(rc, FALSE);
	}
	else m_nItemHilighted = -1;

	if (ht == hitDnScroll)
	{
		m_bLooping = TRUE;
		BOOL bHigh = TRUE;

		if (::GetCapture() == NULL)
		{
			SetCapture();
			ASSERT(this == GetCapture());
			CClientDC dc(this);

			DrawScrollButton( &dc, m_rcDownArrow, DFC_SCROLL, DFCS_SCROLLDOWN|DFCS_PUSHED );

			SetTimer(2,300,NULL);
			AfxLockTempMaps();
			for (;;)
			{
				MSG msg;
				VERIFY(::GetMessage(&msg, NULL, 0, 0));

				if (CWnd::GetCapture() != this) break;

				switch (msg.message)
				{
				case WM_MOUSEMOVE:
					{
						CPoint pt(msg.lParam);
						if (m_rcDownArrow.PtInRect(pt))
						{
							if (bHigh == FALSE)
							{
								DrawScrollButton( &dc, m_rcDownArrow, DFC_SCROLL, DFCS_SCROLLDOWN|DFCS_PUSHED );
								bHigh = TRUE;
								m_bDownArrow = TRUE;
								m_bDownPressed = TRUE;
							}
						}
						else
						{
							if (bHigh == TRUE)
							{
								DrawScrollButton( &dc, m_rcDownArrow, DFC_SCROLL, DFCS_SCROLLDOWN );
								bHigh = FALSE;
								m_bDownArrow = FALSE;
								m_bDownPressed = FALSE;
							}
						}
					}
					break;

				case WM_LBUTTONUP:
					{
						ReleaseCapture();

						if (bHigh)
						{
							DrawScrollButton( &dc, m_rcDownArrow, DFC_SCROLL, DFCS_SCROLLDOWN );
							bHigh = FALSE;
						}
						m_bDownArrow = FALSE;
						CPoint pt(msg.lParam);
						if (m_rcDownArrow.PtInRect(pt))
						{
							CRect itrc;
							GetItemRect(m_nSelFolder, GetItemCount() - 1, itrc);
							CRect crc;
							GetInsideRect(crc);
							if (itrc.bottom > crc.bottom)
							{
								m_nFirstItem ++;
								InvalidateRect(crc, TRUE);
							}
						}
					}
					goto ExitLoop3;


				case WM_TIMER:
					{
						if (msg.wParam == 2)
						{
							if (bHigh)
							{
								CPoint pt(msg.pt);
								ScreenToClient(&pt);
								if (m_rcDownArrow.PtInRect(pt))
								{
									m_bDownPressed = TRUE;
									CRect itrc;
									GetItemRect(m_nSelFolder, GetItemCount() - 1, itrc);
									CRect crc;
									GetInsideRect(crc);
									if (itrc.bottom > crc.bottom)
									{
										m_nFirstItem ++;
										InvalidateRect(crc, TRUE);
									}
									else goto ExitLoop3;
								}
								else m_bDownPressed = FALSE;
							}
						}
						break;
					}

				case WM_KEYDOWN:
					if (msg.wParam != VK_ESCAPE)
						break;

				default:
					DispatchMessage(&msg);
					break;
				}
			}

		ExitLoop3:
			KillTimer(2);
			ReleaseCapture();
			AfxUnlockTempMaps(FALSE);
			m_bLooping = FALSE;
			m_bDownPressed = FALSE;
			m_bDownArrow = FALSE;
			CRect crc;
			GetInsideRect(crc);
			InvalidateRect(crc, TRUE);
		}
	}

	if (ht == hitUpScroll)
	{
		m_bLooping = TRUE;
		BOOL bHigh = TRUE;

		if (::GetCapture() == NULL)
		{
			SetCapture();
			ASSERT(this == GetCapture());
			CClientDC dc(this);
			DrawScrollButton( &dc, m_rcUpArrow, DFC_SCROLL, DFCS_SCROLLUP|DFCS_PUSHED );
			SetTimer(2,300,NULL);
			AfxLockTempMaps();
			for (;;)
			{
				MSG msg;
				VERIFY(::GetMessage(&msg, NULL, 0, 0));

				if (CWnd::GetCapture() != this) break;

				switch (msg.message)
				{
				case WM_MOUSEMOVE:
					{
						CPoint pt(msg.lParam);
						if (m_rcUpArrow.PtInRect(pt))
						{
							if (bHigh == FALSE)
							{
								DrawScrollButton( &dc, m_rcUpArrow, DFC_SCROLL, DFCS_SCROLLUP|DFCS_PUSHED );
								bHigh = TRUE;
								m_bUpArrow = TRUE;
								m_bUpPressed = TRUE;
							}
						}
						else
						{
							if (bHigh == TRUE)
							{
								DrawScrollButton( &dc, m_rcUpArrow, DFC_SCROLL, DFCS_SCROLLUP );
								bHigh = FALSE;
								m_bUpArrow = FALSE;
								m_bUpPressed = FALSE;
							}
						}
					}
					break;

				case WM_LBUTTONUP:
					{
						ReleaseCapture();

						if (bHigh)
						{
							DrawScrollButton( &dc, m_rcUpArrow, DFC_SCROLL, DFCS_SCROLLUP );
							bHigh = FALSE;
						}
						m_bUpArrow = FALSE;
						CPoint pt(msg.lParam);
						if (m_rcUpArrow.PtInRect(pt))
						{
							if (m_nFirstItem > 0 )
							{
								m_nFirstItem --;
								CRect crc;
								GetInsideRect(crc);
								InvalidateRect(crc, TRUE);
							}
						}
					}

					goto ExitLoop;

				case WM_TIMER:
					{
						if (msg.wParam == 2)
						{
							if (bHigh)
							{
								CPoint pt(msg.pt);
								ScreenToClient(&pt);
								if (m_rcUpArrow.PtInRect(pt))
								{
									m_bUpPressed = TRUE;
									if (m_nFirstItem > 0)
									{
										m_nFirstItem --;
										CRect crc;
										GetInsideRect(crc);
										InvalidateRect(crc, TRUE);
									}
									else goto ExitLoop;
								}
								else m_bUpPressed = FALSE;
							}
						}
						break;
					}
				case WM_KEYDOWN:
					if (msg.wParam != VK_ESCAPE) break;

				default:
					DispatchMessage(&msg);
					break;
				}
			}

		ExitLoop:
			KillTimer(2);
			ReleaseCapture();
			AfxUnlockTempMaps(FALSE);
			m_bLooping = FALSE;
			m_bUpPressed = FALSE;
			m_bUpArrow = FALSE;
			CRect crc;
			GetInsideRect(crc);
			InvalidateRect(crc, TRUE);
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

int CXTOutBarCtrl::InsertItem(const int iFolder, const int iIndex, LPCTSTR lpszItemName, const int iImage, const DWORD dwData)
{
	if (m_arFolder.GetSize() != 0)
	{
		ASSERT(iFolder >= 0 && iFolder < GetFolderCount());

		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(iFolder);

		return pBarFolder->InsertItem(iIndex, lpszItemName, iImage, dwData);
	}
	return -1;
}

int CXTOutBarCtrl::GetItemCount() const
{
	if (m_arFolder.GetSize() != 0)
	{
		ASSERT(m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount());
		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(m_nSelFolder);
		return  pBarFolder->GetItemCount();
	}
	return 0;
}

void CXTOutBarCtrl::SetSelFolder(const int iIndex)
{
	if (m_arFolder.GetSize() != 0)
	{
		ASSERT(iIndex >= 0 && iIndex < GetFolderCount());

		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(iIndex);

		CWnd* pOwner = (CWnd*)GetOwner();
		ASSERT_VALID(pOwner);

		XT_OUTBAR_INFO obi;
		obi.nIndex    = iIndex;
		obi.nDragFrom = -1;
		obi.nDragTo   = -1;
		obi.lpszText  = (LPCTSTR)pBarFolder->GetName();
		obi.bFolder   = true;

		LRESULT lResult = pOwner->SendMessage(XTWM_OUTBAR_NOTIFY,
			OBN_XT_FOLDERCHANGE, (LPARAM)&obi);

		if (lResult == TRUE)
		{
			CWnd* pWnd = GetFolderChild();
			if (pWnd) {
				pWnd->ShowWindow(SW_HIDE);
			}

			m_nLastItemSelected = -1;

			if (iIndex != m_nSelFolder && m_nSelFolder >= 0)
			{
				if ((m_dwFlags & OBS_XT_ANIMATION) && (m_nAnimationTickCount >= 0)) {
					AnimateFolderScroll(m_nSelFolder, iIndex);
				}
			}

			m_nSelFolder = iIndex;
			m_nFirstItem = 0;

			pWnd = GetFolderChild();
			if (pWnd)
			{
				CRect rc;
				GetInsideRect(rc);
				pWnd->MoveWindow(rc);
				pWnd->ShowWindow(SW_SHOW);
			}

			InvalidateRect(NULL);
		}
	}
}

int CXTOutBarCtrl::GetFolderCount() const
{
	return (int)m_arFolder.GetSize();
}

int CXTOutBarCtrl::GetSelFolder() const
{
	return m_nSelFolder;
}

void CXTOutBarCtrl::RemoveFolder(const int iIndex)
{
	if (m_arFolder.GetSize() != 0)
	{
		ASSERT(iIndex >= 0 && iIndex < GetFolderCount());
		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(iIndex);

		CWnd* pOwner = (CWnd*)GetOwner();
		ASSERT_VALID(pOwner);

		XT_OUTBAR_INFO obi;
		obi.nIndex    = iIndex;
		obi.nDragFrom = -1;
		obi.nDragTo   = -1;
		obi.lpszText  = (LPCTSTR)pBarFolder->GetName();
		obi.bFolder   = true;

		LRESULT lResult = pOwner->SendMessage(XTWM_OUTBAR_NOTIFY,
			OBN_XT_DELETEFOLDER, (LPARAM)&obi);

		if (lResult == TRUE)
		{
			if (m_nSelFolder >= iIndex) {
				m_nSelFolder = iIndex - 1;
			}
			if (m_nSelFolder < 0 && GetFolderCount() > 0) {
				m_nSelFolder = 0;
			}

			m_arFolder.RemoveAt(iIndex);
			SAFE_DELETE(pBarFolder);

			InvalidateRect(NULL);
		}
	}
}

CImageList* CXTOutBarCtrl::SetImageList(CImageList* pImageList, DWORD dwImageList)
{
	CImageList* pOldImageList = NULL;

	if (dwImageList & OBS_XT_SMALLICON) {
		pOldImageList = m_pSmallImageList;
		m_pSmallImageList = pImageList;
	}
	else if (dwImageList & OBS_XT_LARGEICON) {
		pOldImageList = m_pLargeImageList;
		m_pLargeImageList = pImageList;
	}

	return pOldImageList;
}

CImageList* CXTOutBarCtrl::GetImageList(DWORD dwImageList)
{
	if (dwImageList & OBS_XT_SMALLICON) {
		return m_pSmallImageList;
	}
	else if (dwImageList & OBS_XT_LARGEICON) {
		return m_pLargeImageList;
	}

	return NULL;
}

CImageList* CXTOutBarCtrl::SetFolderImageList(const int iFolder, CImageList* pImageList, DWORD dwImageList)
{
	if (m_arFolder.GetSize() != 0)
	{
		ASSERT(iFolder >= 0 && iFolder < GetFolderCount());
		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(iFolder);
		CImageList* pOldImageList = NULL;

		if (dwImageList & OBS_XT_SMALLICON) {
			pOldImageList = pBarFolder->GetSmallImageList();
			pBarFolder->SetSmallImageList(pImageList);
		}

		else if (dwImageList & OBS_XT_LARGEICON) {
			pOldImageList = pBarFolder->GetLargeImageList();
			pBarFolder->SetLargeImageList(pImageList);
		}

		return pOldImageList;
	}
	return NULL;
}

void CXTOutBarCtrl::PaintItems(CDC* pDC, const int iFolder, CRect rc)
{
	if (m_arFolder.GetSize() != 0)
	{
		ASSERT(iFolder >= 0 && iFolder < GetFolderCount());
		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(iFolder);

		int nItemCount = pBarFolder->GetItemCount();

		CRgn rgn;
		rgn.CreateRectRgnIndirect(&rc);
		pDC->SelectClipRgn(&rgn);

		for (int i = m_nFirstItem; i < nItemCount; i++)
		{
			CRect rcItem;
			GetItemRect(iFolder,i, rcItem, &rc);
			if (rcItem.top > rc.bottom)
				break;

			DrawItem(pDC, iFolder, rcItem, i);
		}

		pDC->SelectClipRgn(NULL);

		//rgn.DeleteObject();
	}
}

CSize CXTOutBarCtrl::GetItemSize(const int iFolder, const int iIndex, const RectItem iType)
{
	if (m_arFolder.GetSize() != 0)
	{
		if (iFolder < 0 && iFolder >= GetFolderCount())
			return CSize(0,0);

		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(iFolder);

		if (iIndex < 0 && iIndex >= pBarFolder->GetItemCount())
			return CSize(0,0);

		CXTOutBarItem* pBarItem = pBarFolder->GetItemAt(iIndex);

		CSize szImage(0,0);
		CSize szLabel(0,0);
		CSize szAll(0,0);

		if (pBarItem->GetIndex() >= 0)
		{
			if (iType != rectItemLabel)
			{
				CImageList* il = GetFolderImageList(iFolder, IsSmallIconView(iFolder));
				ASSERT(il);
				if (il)
				{
					IMAGEINFO ii;
					il->GetImageInfo(pBarItem->GetIndex(), &ii);
					szImage = CRect(ii.rcImage).Size();
				}
			}
		}
		if (pBarItem->GetName().GetLength())
		{
			if (iType != rectItemIcon)
			{
				CClientDC dc(this);
				CFont* oft = (CFont*)dc.SelectObject(GetFontX());
				if (IsSmallIconView(iFolder))
				{
					szLabel = dc.GetTextExtent(pBarItem->GetName(), pBarItem->GetName().GetLength());
				}
				else
				{
					CRect rc;
					GetInsideRect(rc);
					rc.bottom = rc.top;
					dc.DrawText(pBarItem->GetName(), pBarItem->GetName().GetLength(), rc,
						DT_CALCRECT | DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_END_ELLIPSIS);
					szLabel = rc.Size();
				}
				dc.SelectObject(oft);
			}
		}
		if (IsSmallIconView(iFolder))
		{
			if (iType == rectItemIcon) szAll = szImage;
			else if (iType == rectItemLabel) szAll = szLabel;
			else if (iType == rectItemBoth) szAll = CSize(szImage.cx + szLabel.cx + m_sizeOffset.cx, szImage.cy > szLabel.cy ? szImage.cy : szLabel.cy);
		}
		else
		{
			if (iType == rectItemIcon) szAll = szImage;
			else if (iType == rectItemLabel) szAll = szLabel;
			else if (iType == rectItemBoth)
			{
				szAll = CSize(szImage.cx > szLabel.cx ? szImage.cx : szLabel.cx, szLabel.cy + szImage.cy + m_sizeOffset.cy + m_nIconSpacingLarge);
			}
		}
		return szAll;
	}

	return CSize(0,0);
}

CImageList* CXTOutBarCtrl::GetFolderImageList(const int iIndex, const BOOL bSmall) const
{
	if (m_arFolder.GetSize() != 0)
	{
		ASSERT(iIndex >= 0 && iIndex < GetFolderCount());
		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(iIndex);
		if (bSmall)
		{
			if (pBarFolder->GetSmallImageList())
				return pBarFolder->GetSmallImageList();
			else
				return m_pSmallImageList;
		}
		if (pBarFolder->GetLargeImageList())
			return pBarFolder->GetLargeImageList();
		else
			return m_pLargeImageList;
	}
	return NULL;
}

void CXTOutBarCtrl::DrawScrollButton(CDC* pDC, CRect rect, UINT uType, UINT uState)
{
	pDC->DrawFrameControl( &rect, uType, uState );
}

void CXTOutBarCtrl::DrawItem(CDC* pDC, const int iFolder, CRect rc, const int iIndex, const BOOL bOnlyImage)
{
	if (m_arFolder.GetSize() != 0)
	{
		CImageList* pImageList = GetFolderImageList(iFolder, IsSmallIconView(iFolder));

		ASSERT(iFolder >= 0 && iFolder < GetFolderCount());
		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(iFolder);

		ASSERT(iIndex >= 0 && iIndex < pBarFolder->GetItemCount());
		CXTOutBarItem* pBarItem = pBarFolder->GetItemAt(iIndex);

		ASSERT(pBarItem && pImageList);

		CFont* oft = (CFont*)pDC->SelectObject(GetFontX());
		pDC->SetBkMode(TRANSPARENT);

		CRect rcFolder;
		GetFolderRect(iFolder, rcFolder);
		rc.left  = rcFolder.left +4;
		rc.right = rcFolder.right-4;

		// Exclude the up and down arrows from painting.
		if (m_bUpArrow)
			pDC->ExcludeClipRect(&m_rcUpArrow);
		if (m_bDownArrow)
			pDC->ExcludeClipRect(&m_rcDownArrow);

		// set the items text color.
		pDC->SetTextColor(GetTheme()->m_clrText);

		if ((m_dwFlags & OBS_XT_SELHIGHLIGHT) && (m_nLastItemSelected == iIndex) && (m_nLastItemSelected >= 0))
		{
			CRect rc;
			GetIconRect(m_nSelFolder, m_nLastItemSelected, rc);
			rc.InflateRect(1,1);

			GetTheme()->DrawIconFrame(pDC, rc, TRUE, FALSE);
		}


		if (pImageList)
		{
			IMAGEINFO ii;
			pImageList->GetImageInfo(pBarItem->GetIndex(), &ii);
			CSize szImage = CRect(ii.rcImage).Size();
			CPoint pt;

			if (IsSmallIconView(iFolder))
			{
				pt.x = rc.left + 2;
				pt.y = rc.top + (rc.Height() - szImage.cy) / 2;

				DrawItemIcon( pDC, pt, pBarItem, pImageList);

				if (!bOnlyImage)
				{
					rc.left  += (szImage.cx + m_sizeOffset.cx);

					DrawItemText(pDC, rc, pBarItem, DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
				}
			}
			else
			{
				pt.x = rc.left + (rc.Width() - szImage.cx) / 2;
				pt.y = rc.top;

				DrawItemIcon( pDC, pt, pBarItem, pImageList);

				if (!bOnlyImage)
				{
					rc.top += szImage.cy + m_sizeOffset.cy;
					rc.bottom += 4;
					DrawItemText(pDC, rc, pBarItem, DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_END_ELLIPSIS);
				}
			}
		}

		pDC->SelectObject(oft);
	}
}

BOOL CXTOutBarCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);
	int iIndex, ht = HitTestEx(pt, iIndex);
	if (ht == hitFolder)
	{
		::SetCursor(XTAuxData().hcurHand);
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CXTOutBarCtrl::GetVisibleRange(const int iFolder, int& iFirst, int& iLast)
{
	iFirst = m_nFirstItem;

	CRect rcInside;
	GetInsideRect( rcInside );

	if ( m_arFolder.GetSize() != 0 )
	{
		CXTOutBarFolder* pBarFolder = ( CXTOutBarFolder* )m_arFolder.GetAt( iFolder );
		if ( pBarFolder == NULL )
			return;

		int iCount = pBarFolder->GetItemCount();
		if ( iCount <= 0 )
		{
			iFirst = iLast = -1;
			return;
		}

		int iItem;
		for ( iItem = m_nFirstItem; iItem < iCount; ++ iItem )
		{
			CRect rcItem;
			GetItemRect( iFolder, iItem, rcItem );

			if (rcItem.bottom > rcInside.bottom)
			{
				iLast = iItem - 1;
				break;
			}
			else
			{
				iLast = iItem;
			}
		}
	}
}

void CXTOutBarCtrl::OnSize(UINT nType, int cx, int cy)
{
	m_bUpArrow = m_bDownArrow = FALSE;

	CWnd::OnSize(nType, cx, cy);

	int t, max = GetFolderCount();
	CRect rc;
	GetInsideRect(rc);

	for (t = 0; t < max; t++)
	{
		CWnd* pWnd = GetFolderChild(t);
		if (pWnd) pWnd->SetWindowPos(0, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER);
	}
}

void CXTOutBarCtrl::DrawIcon(CDC* pDC, int iIcon, int iFolder, bool bHilight)
{
	// Clear the icon background
	CRect rcIcon;
	GetIconRect(iFolder, iIcon, rcIcon);

	if (!IsSmallIconView(iFolder))
	{
		rcIcon.InflateRect(1,1);
	}
	else
	{
		rcIcon.left--;
		rcIcon.top--;
	}

	GetTheme()->FillInsideRect(pDC, rcIcon);

	// Should we highlight the icon?
	if (bHilight)
	{
		GetTheme()->DrawIconFrame(pDC, rcIcon, FALSE, m_bIconPressed);
	}

	// Draw the icon using the image list.
	ASSERT(iFolder >= 0 && iFolder < GetFolderCount());
	CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(iFolder);

	ASSERT(iIcon >= 0 && iIcon < pBarFolder->GetItemCount());
	CXTOutBarItem* pBarItem = pBarFolder->GetItemAt(iIcon);

	CImageList* pImageList = GetFolderImageList(iFolder, IsSmallIconView(iFolder));
	ASSERT(pBarItem && pImageList);

	CRect rc;
	GetItemRect(iFolder, iIcon, rc);

	CRect rcFolder;
	GetFolderRect(iFolder, rcFolder);
	rc.left  = rcFolder.left +4;
	rc.right = rcFolder.right-4;

	if (IsSmallIconView(iFolder))
	{
		if (pImageList)
		{
			IMAGEINFO ii;
			pImageList->GetImageInfo(pBarItem->GetIndex(), &ii);
			CSize szImage = CRect(ii.rcImage).Size();
			CPoint pt;
			pt.x = rc.left + 2;
			pt.y = rc.top + (rc.Height() - szImage.cy) / 2;

			DrawItemIcon( pDC, pt, pBarItem, pImageList);
		}
	}
	else
	{
		if (pImageList)
		{
			IMAGEINFO ii;
			pImageList->GetImageInfo(pBarItem->GetIndex(), &ii);
			CSize szImage = CRect(ii.rcImage).Size();
			CPoint pt;
			pt.x = rc.left + (rc.Width() - szImage.cx) / 2;
			pt.y = rc.top;// + (rc.Height() - szImage.cy) / 2;

			DrawItemIcon( pDC, pt, pBarItem, pImageList);
		}
	}
}

void CXTOutBarCtrl::HighlightItem(CDC* pDC, const int iIndex, const BOOL bPressed)
{
	static BOOL bOldPressed = FALSE;

	CWnd* pFocus = GetFocus();
	if (pFocus != NULL && pFocus != this && IsChild(pFocus)) {
		return;
	}

	if (m_nItemHilighted == iIndex && bOldPressed == bPressed) {
		return;
	}

	if ( iIndex != -1 )
	{
		CXTOutBarItem* pBarItem = GetBarFolderItem( m_nSelFolder, iIndex );
		if ( !pBarItem->IsEnabled( ) )
		{
			return;
		}
	}

	m_bIconPressed = FALSE;
	bOldPressed = bPressed;

	XT_OUTBAR_INFO obi;
	obi.nIndex    = iIndex;
	obi.nDragFrom = -1;
	obi.nDragTo   = -1;
	obi.lpszText  = GetItemText(iIndex);
	obi.bFolder   = false;

	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	LRESULT lResult = pOwner->SendMessage(XTWM_OUTBAR_NOTIFY,
		OBN_XT_ITEMHOVER, (LPARAM)&obi);

	if (!lResult)
		return;

	CDC dcClient;
	if (!pDC)
	{
		dcClient.Attach(::GetDC(m_hWnd));
		pDC = &dcClient;
	}

	CRect irc;
	GetInsideRect(irc);

	CRgn rgn;
	rgn.CreateRectRgnIndirect(&irc);
	pDC->SelectClipRgn(&rgn);

	// Exclude the up and down arrows from painting.
	if (m_bUpArrow)
		pDC->ExcludeClipRect(&m_rcUpArrow);
	if (m_bDownArrow)
		pDC->ExcludeClipRect(&m_rcDownArrow);

	if (m_nItemHilighted >= 0 && IsValidItem(m_nItemHilighted))
	{
		DrawIcon(pDC, m_nItemHilighted, m_nSelFolder, false);
	}
	if ((m_nSelAnimTiming > 0 && iIndex == m_nLastItemSelected) ||
		((m_dwFlags & OBS_XT_SELHIGHLIGHT) && (m_nLastItemSelected == iIndex)))
	{
		m_nItemHilighted = -1;
	}
	else
	{
		m_nItemHilighted = iIndex;
		m_bIconPressed = bPressed;

		if (m_nItemHilighted >= 0 && IsValidItem(m_nItemHilighted))
		{
			DrawIcon(pDC, m_nItemHilighted, m_nSelFolder, true);
		}
	}

	pDC->SelectClipRgn(NULL);
	rgn.DeleteObject();

	if (dcClient.GetSafeHdc())
	{
		::ReleaseDC(m_hWnd, dcClient.Detach());
	}
}

void CXTOutBarCtrl::GetIconRect(const int iFolder, const int iIndex, CRect&  rect, CRect* pInsideRect /*= NULL*/)
{
	CRect rc;
	if (pInsideRect) rc = *pInsideRect; else GetInsideRect(rc);
	int top = rc.top;
	CSize sz(0,0);
	int y = 0;
	int spacing = IsSmallIconView(iFolder) ? m_nIconSpacingSmall : m_nIconSpacingLarge;

	int t;
	for (t = 0; t < iIndex; t++)
	{
		sz = GetItemSize(iFolder, t, rectItemBoth);
		top += sz.cy;
		top += spacing;
		if (t == m_nFirstItem - 1) y = top - rc.top;
	}
	top += spacing;

	sz = GetItemSize(iFolder, iIndex, rectItemIcon);
	if (IsSmallIconView(iFolder))
	{
		rect.SetRect(rc.left, top, rc.left + sz.cx, top + sz.cy);
		rect.left += m_sizeMargin.cx + 2;
		rect.right += m_sizeMargin.cx + 2;
		rect.top -= m_sizeMargin.cy;
		rect.bottom -= m_sizeMargin.cy;

		rect.top -= y;
		rect.bottom -= y;
	}
	else
	{
		rect.SetRect(rc.left + (rc.Width() - sz.cx)/2, top, rc.left + (rc.Width() - sz.cx)/2 + sz.cx, top + sz.cy);
		rect.top -= y + 3;
		rect.bottom -= y + 2;
	}
}

void CXTOutBarCtrl::GetLabelRect(const int iFolder, const int iIndex, CRect&  rect)
{
	CRect rc;
	GetInsideRect(rc);
	int top = rc.top;
	CSize sz(0,0);
	int y = 0;
	int spacing = IsSmallIconView(iFolder) ? m_nIconSpacingSmall : m_nIconSpacingLarge;

	int t;
	for (t = 0; t < iIndex; t++)
	{
		sz = GetItemSize(iFolder, t, rectItemBoth);
		top += sz.cy;
		top += spacing;
		if (t == m_nFirstItem - 1) y = top - rc.top;
	}
	top += spacing;

	sz = GetItemSize(iFolder, iIndex, rectItemBoth);

	CSize szLabel = GetItemSize(iFolder, iIndex, rectItemLabel);

	if (IsSmallIconView(iFolder))
	{
		rect.SetRect(rc.left, top, rc.left + sz.cx, top + sz.cy);
		rect.left += m_sizeMargin.cx + 2;
		rect.right += m_sizeMargin.cx + 2;
		rect.top  += m_sizeMargin.cy - 5;
		rect.bottom += m_sizeMargin.cy - 5;
	}
	else
	{
		rect.SetRect(rc.left + (rc.Width() - sz.cx)/2, top, rc.left + (rc.Width() - sz.cx)/2 + sz.cx, top + sz.cy);
		rect.top -= y + 3;
		rect.bottom -= y + 2;

		rect.bottom -=  m_nIconSpacingLarge;
		rect.top    = rect.bottom - szLabel.cy;
	}
}

void CXTOutBarCtrl::StartGroupEdit(const int iIndex)
{
	CRect rc;
	GetFolderRect( iIndex, rc );
	rc.InflateRect( -2,-2 );

	if (m_arFolder.GetSize( ) != 0 )
	{
		ASSERT( iIndex >= 0 && iIndex < GetFolderCount( ) );
		CXTOutBarFolder* pBarFolder = ( CXTOutBarFolder* )m_arFolder.GetAt( iIndex );

		CXTOutBarEditItem* pEdit = new CXTOutBarEditItem;
		if ( pEdit->Create( pBarFolder->GetName( ), WS_CHILD|WS_VISIBLE|ES_CENTER|ES_AUTOHSCROLL,
			rc, this, 0xFF, iIndex, true, false ) )
		{
			pEdit->Initialize(this);
			pEdit->ModifyStyleEx( 0, WS_EX_CLIENTEDGE, SWP_FRAMECHANGED );
			pEdit->SetFocus( );
			pEdit->SetSel( 0,-1 );
		}
	}
}

void CXTOutBarCtrl::StartItemEdit(const int iIndex)
{
	if ( m_arFolder.GetSize( ) != 0 )
	{
		ASSERT( m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount( ) );
		CXTOutBarFolder* pBarFolder = ( CXTOutBarFolder* )m_arFolder.GetAt( m_nSelFolder );

		ASSERT( iIndex >= 0 && iIndex < pBarFolder->GetItemCount( ) );
		CXTOutBarItem* pBarItem = pBarFolder->GetItemAt( iIndex );
		if ( pBarItem == NULL )
		{
			return;
		}

		CRect rcLabel;
		GetLabelRect(m_nSelFolder, iIndex, rcLabel);

		DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL;

		bool bNoDown = false;
		if ( IsSmallIconView( m_nSelFolder ) )
		{
			bNoDown = true;

			CRect rcIcon;
			GetIconRect( m_nSelFolder, iIndex, rcIcon );
			rcLabel.OffsetRect( rcIcon.Width( )+1, -6 );
			rcLabel.InflateRect( 0,1 );
		}
		else
		{
			CRect rcInside;
			GetInsideRect( rcInside );

			CWindowDC dc( NULL );
			CFont* f = dc.SelectObject( GetFontX( ) );
			CSize sz = dc.GetTextExtent( pBarItem->GetName( ) );
			dc.SelectObject( f );

			int x = ( rcInside.Width( )-sz.cx )/2;

			rcLabel.left  = rcInside.left + x;
			rcLabel.right = rcLabel.left  + sz.cx;
			rcLabel.InflateRect( 6,2 );
			rcLabel.OffsetRect( 0,-1 );

			dwStyle |= ES_CENTER;
		}

		CXTOutBarEditItem* pEdit = new CXTOutBarEditItem;
		if ( pEdit->Create( pBarItem->GetName( ), dwStyle, rcLabel, this,
			0xFF, iIndex, false, bNoDown ) )
		{
			pEdit->Initialize(this);
			pEdit->SetFocus();
			pEdit->SetSel(0,-1);

			Invalidate();
		}
	}
}

void CXTOutBarCtrl::OnLargeIcon()
{
	SetSmallIconView(FALSE, m_nSelFolder);
	Invalidate();
}

void CXTOutBarCtrl::OnUpdateLargeIcon(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsSmallIconView(m_nSelFolder));
}

void CXTOutBarCtrl::OnSmallIcon()
{
	SetSmallIconView(TRUE, m_nSelFolder);
	Invalidate();
}

void CXTOutBarCtrl::OnUpdateSmallIcon(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!IsSmallIconView(m_nSelFolder));
}

void CXTOutBarCtrl::OnRemoveItem()
{
	if (m_nHitInternal1 == hitFolder)
	{
		RemoveFolder(m_nHitInternal2);
	}
	else if (m_nHitInternal1 == hitItem)
	{
		RemoveItem(m_nHitInternal2);
	}
}

void CXTOutBarCtrl::OnUpdateRemoveItem(CCmdUI* /*pCmdUI*/)
{

}

void CXTOutBarCtrl::OnRenameItem()
{
	if (m_nHitInternal1 == hitFolder)
	{
		StartGroupEdit(m_nHitInternal2);
	}
	else if (m_nHitInternal1 == hitItem)
	{
		StartItemEdit(m_nHitInternal2);
	}
}

void CXTOutBarCtrl::OnUpdateRenameItem(CCmdUI* /*pCmdUI*/)
{

}

void CXTOutBarCtrl::RemoveAllItems(int iFolder, bool bNofify/*=false*/)
{
	ASSERT( iFolder >= 0 && iFolder < GetFolderCount( ) );
	CXTOutBarFolder* pBarFolder = ( CXTOutBarFolder* )m_arFolder.GetAt( iFolder );
	if ( pBarFolder != NULL )
	{
		int iFolderCount = pBarFolder->GetItemCount();

		int iIndex;
		for ( iIndex = iFolderCount-1; iIndex >= 0; --iIndex )
		{
			CXTOutBarItem* pBarItem = pBarFolder->RemoveItemAt( iIndex );
			if ( pBarItem == NULL )
			{
				continue;
			}

			if ( bNofify )
			{
				XT_OUTBAR_INFO obi;
				obi.nIndex    = iIndex;
				obi.nDragFrom = -1;
				obi.nDragTo   = -1;
				obi.lpszText  = pBarItem->GetName( );
				obi.bFolder   = false;

				CWnd* pOwner = GetOwner( );
				ASSERT_VALID( pOwner );

				pOwner->SendMessage( XTWM_OUTBAR_NOTIFY,
					OBN_XT_DELETEITEM, ( LPARAM )&obi );
			}

			SAFE_DELETE( pBarItem );
		}

		Invalidate( );
		UpdateWindow( );
	}
}

void CXTOutBarCtrl::RemoveItem(const int iIndex)
{
	if (m_arFolder.GetSize() != 0)
	{
		ASSERT(m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount());
		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(m_nSelFolder);
		ASSERT(iIndex >= 0 && iIndex < pBarFolder->GetItemCount());

		if (IsValidItem(iIndex))
		{
			XT_OUTBAR_INFO obi;
			obi.nIndex    = iIndex;
			obi.nDragFrom = -1;
			obi.nDragTo   = -1;
			obi.lpszText  = GetItemText(iIndex);
			obi.bFolder   = false;

			CWnd* pOwner = GetOwner();
			ASSERT_VALID(pOwner);

			LRESULT lResult = pOwner->SendMessage(XTWM_OUTBAR_NOTIFY,
				OBN_XT_DELETEITEM, (LPARAM)&obi);

			if (lResult == TRUE)
			{
				CXTOutBarItem* pBarItem = pBarFolder->RemoveItemAt(iIndex);
				SAFE_DELETE(pBarItem);

				if (m_nLastItemSelected == iIndex)
					m_nLastItemSelected = -1;

				InvalidateRect(NULL);
			}
		}
	}
}

BOOL CXTOutBarCtrl::IsValidItem(const int iIndex) const
{
	if (m_arFolder.GetSize() != 0)
	{
		ASSERT(m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount());
		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(m_nSelFolder);
		return (iIndex >= 0 && iIndex < pBarFolder->GetItemCount());
	}
	return FALSE;
}

DWORD CXTOutBarCtrl::GetItemData(const int iIndex) const
{
	if (m_arFolder.GetSize() != 0)
	{
		ASSERT(m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount());
		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(m_nSelFolder);
		if (IsValidItem(iIndex))
		{
			CXTOutBarItem* i = pBarFolder->GetItemAt(iIndex);
			return i->GetData();
		}
	}
	return 0;
}

int CXTOutBarCtrl::GetItemImage(const int iIndex) const
{
	if (m_arFolder.GetSize() != 0)
	{
		ASSERT(m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount());
		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(m_nSelFolder);
		if (IsValidItem(iIndex))
		{
			CXTOutBarItem* i = pBarFolder->GetItemAt(iIndex);
			return i->GetIndex();
		}
	}
	return 0;
}

void CXTOutBarCtrl::SetItemData(const int iIndex, const DWORD dwData)
{
	if (m_arFolder.GetSize() != 0)
	{
		ASSERT(m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount());
		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(m_nSelFolder);
		if (IsValidItem(iIndex))
		{
			CXTOutBarItem* i = pBarFolder->GetItemAt(iIndex);
			i->SetData( dwData );
		}
	}
}

void CXTOutBarCtrl::SetItemImage(const int iIndex, const int iImage)
{
	if (m_arFolder.GetSize() != 0)
	{
		ASSERT(m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount());
		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(m_nSelFolder);
		if (IsValidItem(iIndex))
		{
			CXTOutBarItem* i = pBarFolder->GetItemAt(iIndex);
			i->SetIndex( iImage );
		}
	}
}

void CXTOutBarCtrl::DrawDragArrow(CDC* pDC, const int /*iFrom*/, const int iTo)
{
	if (iTo == m_nLastDragItemDraw) return;
	CRect rc1;
	if (m_nLastDragItemDraw >= 0)
	{
		GetDragItemRect(m_nLastDragItemDraw, rc1);
		if (m_nLastDragItemDrawType == 0) rc1.bottom = rc1.top + 5;
		else if (m_nLastDragItemDrawType == 1) { rc1.top -= 4; rc1.bottom = rc1.top + 9; }
		else if (m_nLastDragItemDrawType == 2) { rc1.top -= 4; rc1.bottom = rc1.top + 5; }
		InvalidateRect(rc1, TRUE);
		UpdateWindow();
	}
	m_nLastDragItemDraw = iTo;

	if (m_nLastDragItemDraw >= 0)
	{
		m_nLastDragItemDrawType = GetDragItemRect(m_nLastDragItemDraw, rc1);

		CPen* op = pDC->SelectObject(&m_penBlack);

		pDC->MoveTo(rc1.left, rc1.top);
		pDC->LineTo(rc1.right, rc1.top);

		if (m_nLastDragItemDrawType != 2)
		{
			pDC->MoveTo(rc1.left, rc1.top+1);
			pDC->LineTo(rc1.left + 7, rc1.top+1);
			pDC->MoveTo(rc1.left, rc1.top+2);
			pDC->LineTo(rc1.left + 5, rc1.top+2);
			pDC->MoveTo(rc1.left, rc1.top+3);
			pDC->LineTo(rc1.left + 3, rc1.top+3);
			pDC->MoveTo(rc1.left, rc1.top+4);
			pDC->LineTo(rc1.left + 1, rc1.top+4);

			pDC->MoveTo(rc1.right-1, rc1.top+1);
			pDC->LineTo(rc1.right - 7, rc1.top+1);
			pDC->MoveTo(rc1.right-1, rc1.top+2);
			pDC->LineTo(rc1.right - 5, rc1.top+2);
			pDC->MoveTo(rc1.right-1, rc1.top+3);
			pDC->LineTo(rc1.right - 3, rc1.top+3);
		}
		if (m_nLastDragItemDrawType != 0)
		{
			pDC->MoveTo(rc1.left, rc1.top-1);
			pDC->LineTo(rc1.left + 7, rc1.top-1);
			pDC->MoveTo(rc1.left, rc1.top-2);
			pDC->LineTo(rc1.left + 5, rc1.top-2);
			pDC->MoveTo(rc1.left, rc1.top-3);
			pDC->LineTo(rc1.left + 3, rc1.top-3);
			pDC->MoveTo(rc1.left, rc1.top-4);
			pDC->LineTo(rc1.left + 1, rc1.top-4);

			pDC->MoveTo(rc1.right-1, rc1.top-1);
			pDC->LineTo(rc1.right - 7, rc1.top-1);
			pDC->MoveTo(rc1.right-1, rc1.top-2);
			pDC->LineTo(rc1.right - 5, rc1.top-2);
			pDC->MoveTo(rc1.right-1, rc1.top-3);
			pDC->LineTo(rc1.right - 3, rc1.top-3);
			pDC->MoveTo(rc1.right-1, rc1.top-4);
			pDC->LineTo(rc1.right - 1, rc1.top-4);
		}

		pDC->SelectObject(op);
	}
}

int CXTOutBarCtrl::GetDragItemRect(const int iIndex, CRect&  rect)
{
	CRect rc, crc;
	GetInsideRect(crc);
	crc.InflateRect(-2, 0);

	GetItemRect(m_nSelFolder, iIndex < GetItemCount() ? iIndex : iIndex - 1, rc);
	int line = 0;
	if (iIndex < GetItemCount())
	{
		line = rc.top - 4;
	}
	else
	{
		line = rc.bottom + 6;
	}
	int tpe;
	if (iIndex == 0)
	{
		rect.SetRect(crc.left, line+2, crc.right, line + 7);
		tpe = 0;
	}
	else if (iIndex < GetItemCount())
	{
		rect.SetRect(crc.left, line - 9, crc.right, line);
		tpe = 1;
		if (IsSmallIconView(m_nSelFolder))
		{
			rect.top += 8;
			rect.bottom += 8;
		}
	}
	else
	{
		rect.SetRect(crc.left, line, crc.right, line + 5);
		tpe = 2;
	}
	return tpe;
}

void CXTOutBarCtrl::DrawPreviewBitmap(CWnd* pWnd, CDC* pDC, int nOffset)
{
	if (!pWnd || !::IsWindow(pWnd->m_hWnd))
		return;

	CPoint ovpt = pDC->SetViewportOrg(0, nOffset);
	BOOL bPrev = pWnd->ShowWindow(SW_SHOW);

	CXTPWindowRect rectWindow(pWnd);
	CClientDC dc(pWnd);
	CBitmap bitmapNonClient;

	VERIFY(bitmapNonClient.CreateCompatibleBitmap(&dc, rectWindow.Width(), rectWindow.Height()));

	CXTPCompatibleDC dcMemSourceNonClient(&dc, &bitmapNonClient);

	dcMemSourceNonClient.FillSolidRect(0, 0, rectWindow.Width(), rectWindow.Height(), GetXtremeColor(COLOR_WINDOW));
	pWnd->Print(&dcMemSourceNonClient, PRF_NONCLIENT);

	pDC->BitBlt(0,0,rectWindow.Width(), rectWindow.Height(), &dcMemSourceNonClient, 0,0, SRCCOPY);

	CPoint pLT(0,0);
	pWnd->ClientToScreen(&pLT);

	CPoint pt = pDC->GetViewportOrg();
	pDC->SetViewportOrg(pt.x + pLT.x - rectWindow.left, pt.y + pLT.y - rectWindow.top);
	pWnd->Print(pDC, PRF_CHILDREN | PRF_CLIENT | PRF_ERASEBKGND);

	if (!bPrev) pWnd->ShowWindow(SW_HIDE);
	pDC->SetViewportOrg(ovpt);

}

void CXTOutBarCtrl::AnimateFolderScroll(const int iFrom, const int iTo)
{
	if (m_arFolder.GetSize() == 0)
		return;

	ASSERT(iFrom >= 0 && iFrom < GetFolderCount());
	ASSERT(iTo >= 0 && iTo < GetFolderCount());

	CRect rc, frc;
	GetInsideRect(rc);
	GetFolderRect(iTo, frc);
	int nFolderHeight = frc.Height();

	CClientDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	int nTopItem = iTo > iFrom? iFrom: iTo;
	int nBottomItem = iTo > iFrom? iTo: iFrom;

	CBitmap bmpFrom, bmpTo;
	bmpFrom.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height() + nFolderHeight * (nBottomItem - nTopItem));
	bmpTo.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height() + nFolderHeight * (nBottomItem - nTopItem));


	CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(&bmpFrom);

	CRect rcFrom(0, 0, rc.Width(), rc.Height());
	GetTheme()->FillInsideRect(&memDC, rcFrom);

	CWnd* pWnd1 = GetFolderChild(nTopItem);
	CWnd* pWnd2 = GetFolderChild(nBottomItem);

	if (pWnd1)
	{
		DrawPreviewBitmap(pWnd1, &memDC, 0);
	}
	else PaintItems(&memDC, nTopItem, rcFrom);

	memDC.SelectObject(&bmpTo);

	CRect rcTo(rcFrom);
	for (int i = nTopItem + 1; i <= nBottomItem; i++)
	{
		CRect rcFolder(frc.left, rcTo.top, frc.right, rcTo.top + nFolderHeight);
		DrawFolder(&memDC, i, rcFolder, xtMouseNormal);
		rcTo.OffsetRect(0, nFolderHeight);
	}

	GetTheme()->FillInsideRect(&memDC, rcTo);


	if (pWnd2)
	{
		DrawPreviewBitmap(pWnd2, &memDC, rcTo.top);
	}
	else PaintItems(&memDC, nBottomItem, rcTo);

	if (iTo > iFrom)
	{
		CRect rcFrom, rcTo;
		GetFolderRect(iFrom, rcFrom);
		GetFolderRect(iTo, rcTo);

		for (int y = rcTo.top - nFolderHeight; y > rcFrom.bottom; y -= nFolderHeight)
		{
			memDC.SelectObject(&bmpFrom);
			dc.BitBlt(rc.left, rcFrom.bottom, rc.Width(), y - rcFrom.bottom, &memDC, 0, 0, SRCCOPY);

			memDC.SelectObject(&bmpTo);
			dc.BitBlt(rc.left, y, rc.Width(), rc.bottom - y + (iTo - iFrom) * nFolderHeight, &memDC, 0,0, SRCCOPY);
			Sleep(m_nAnimationTickCount);
		}
	}
	else
	{
		int nTopItem = iTo;
		int nBottomItem = iFrom;

		CRect rcFrom, rcTo;
		GetFolderRect(nTopItem, rcFrom);
		GetFolderRect(nBottomItem, rcTo);
		rcTo.OffsetRect(0, rc.Height());

		for (int y = rcFrom.bottom; y < rcTo.top - nFolderHeight ; y += nFolderHeight)
		{
			memDC.SelectObject(&bmpFrom);
			dc.BitBlt(rc.left, rcFrom.bottom, rc.Width(), y - rcFrom.bottom, &memDC, 0, 0, SRCCOPY);

			memDC.SelectObject(&bmpTo);
			dc.BitBlt(rc.left, y, rc.Width(), rc.bottom - y , &memDC, 0,0, SRCCOPY);
			Sleep(m_nAnimationTickCount);
		}
	}

	memDC.SelectObject(pOldBitmap);
}

CString CXTOutBarCtrl::GetItemText(const int iIndex)
{
	CString item = _T("");

	if (m_arFolder.GetSize() != 0)
	{
		ASSERT(m_nSelFolder >= 0 && m_nSelFolder < GetFolderCount());
		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(m_nSelFolder);
		if (IsValidItem(iIndex))
		{
			CXTOutBarItem* pBarItem = pBarFolder->GetItemAt(iIndex);
			if (pBarItem->GetName().GetLength())
				item = pBarItem->GetName();
		}
	}
	return item;
}

int CXTOutBarCtrl::AddFolderBar( LPCTSTR lpszFolderName, CWnd* pWndChild, const DWORD dwData )
{
	CXTOutBarFolder* pBarFolder = new CXTOutBarFolder( lpszFolderName, dwData, this );
	ASSERT( pBarFolder );

	pBarFolder->SetChild(pWndChild);
	return  (int)m_arFolder.Add((void*)pBarFolder);
}

CWnd* CXTOutBarCtrl::GetFolderChild(int iFolder)
{
	if (GetFolderCount())
	{
		if (iFolder < 0 && m_nSelFolder < 0)
			return NULL;

		if (iFolder < 0)
			iFolder = m_nSelFolder;

		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(iFolder);
		return pBarFolder->GetChild();
	}
	return NULL;
}

DWORD CXTOutBarCtrl::GetFolderData(int iFolder)
{
	if (iFolder < 0) iFolder = m_nSelFolder;
	CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(iFolder);
	return pBarFolder->GetData();
}

void CXTOutBarCtrl::SetAnimSelHighlight(const int iTime)
{
	if (iTime <= 0) KillTimer(3);
	else SetTimer(3, iTime, NULL);
	m_nSelAnimTiming = iTime;
}

void CXTOutBarCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	XT_OUTBAR_INFO obi = { 0 };

	int ht = HitTestEx(point, obi.nIndex);
	if (ht != hitItem)
	{
		obi.bFolder = true;

		if (ht != hitFolder)
		{
			obi.nIndex = -1;
		}
	}

	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	// allow users to add their own right click handlers.
	LRESULT lResult = pOwner->SendMessage(XTWM_OUTBAR_NOTIFY,
		OBN_XT_ITEMRCLICK, (LPARAM)&obi);

	if (lResult == TRUE)
	{

		m_nHitInternal1 = HitTestEx(point, m_nHitInternal2);

		CPoint pt = point;
		ClientToScreen(&pt);

		CMenu menu;
		VERIFY(XTPResourceManager()->LoadMenu(&menu, XT_IDM_POPUP));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		CString string;
		if (m_nHitInternal1 == hitItem)
		{
			if ((m_dwFlags & OBS_XT_REMOVEITEMS) || (m_dwFlags & OBS_XT_EDITITEMS))
			{
				pPopup->AppendMenu(MF_SEPARATOR);
				if (m_dwFlags & OBS_XT_REMOVEITEMS) {
					VERIFY(XTPResourceManager()->LoadString(&string, XT_IDC_REMOVEITEM));
					pPopup->AppendMenu(MF_STRING, XT_IDC_REMOVEITEM, string);
				}
				if (m_dwFlags & OBS_XT_EDITITEMS) {
					VERIFY(XTPResourceManager()->LoadString(&string, XT_IDC_RENAMEITEM));
					pPopup->AppendMenu(MF_STRING, XT_IDC_RENAMEITEM, string);
				}
			}
		}
		else if (m_nHitInternal1 == hitFolder)
		{
			if ((m_dwFlags & OBS_XT_REMOVEGROUPS) || (m_dwFlags & OBS_XT_EDITGROUPS))
			{
				pPopup->AppendMenu(MF_SEPARATOR);

				if (m_dwFlags & OBS_XT_REMOVEGROUPS) {
					VERIFY(XTPResourceManager()->LoadString(&string, XT_IDC_REMOVEGROUP));
					pPopup->AppendMenu(MF_STRING, XT_IDC_REMOVEITEM, string);
				}
				if (m_dwFlags & OBS_XT_EDITGROUPS) {
					VERIFY(XTPResourceManager()->LoadString(&string, XT_IDC_RENAMEGROUP));
					pPopup->AppendMenu(MF_STRING, XT_IDC_RENAMEITEM, string);
				}
			}
		}

		pPopup->CheckMenuItem( IsSmallIconView(m_nSelFolder) ?
			XT_IDC_SMALLICON : XT_IDC_LARGEICON, MF_CHECKED|MF_BYCOMMAND );

		XTFuncContextMenu(pPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, pWndPopupOwner, XT_IDR_TBAR_OUT);
	}

	CWnd::OnRButtonUp(nFlags, point);
}

void CXTOutBarCtrl::OnDestroy()
{
	CWnd* pOwner = (CWnd*)GetOwner();
	ASSERT_VALID(pOwner);

	// send notification for each folder / item that is deleted.
	int iFolder;
	for (iFolder = 0; iFolder < m_arFolder.GetSize(); iFolder++)
	{
		m_nSelFolder = iFolder;
		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(iFolder);

		XT_OUTBAR_INFO obif;
		obif.nIndex    = iFolder;
		obif.nDragFrom = -1;
		obif.nDragTo   = -1;
		obif.lpszText  = pBarFolder->GetName();
		obif.bFolder   = true;

		if (pBarFolder != NULL)
		{
			int iCount = (int)pBarFolder->m_barItems.GetCount();

			int iItem;
			for (iItem = 0; iItem < iCount; iItem++)
			{
				CXTOutBarItem* pBarItem = pBarFolder->GetItemAt(iItem);

				XT_OUTBAR_INFO obi;
				obi.nIndex    = iItem;
				obi.nDragFrom = -1;
				obi.nDragTo   = -1;
				obi.lpszText  = pBarItem->GetName();
				obi.bFolder   = false;

				pOwner->SendMessage(XTWM_OUTBAR_NOTIFY,
					OBN_XT_DELETEITEM, (LPARAM)&obi);
			}

			pOwner->SendMessage(XTWM_OUTBAR_NOTIFY,
				OBN_XT_DELETEFOLDER, (LPARAM)&obif);
		}
	}

	CWnd::OnDestroy();
}

void CXTOutBarCtrl::EnableItem(int iFolder, int iItem, bool bEnable)
{
	ASSERT( iFolder < m_arFolder.GetSize( ) );

	CXTOutBarItem* pBarItem = GetBarFolderItem( iFolder, iItem );
	if ( pBarItem )
	{
		pBarItem->EnableItem( bEnable );
	}
}

void CXTOutBarCtrl::SetSelItem(int iFolder, int iItem, BOOL bEnsureVisible/*=TRUE*/)
{
	if (iFolder >= 0 && iFolder < GetFolderCount())
	{
		// select the specified folder.
		SetSelFolder(iFolder);

		// get a pointer to the folder object.
		CXTOutBarFolder* pBarFolder = GetBarFolder( iFolder );

		int iItemCount = pBarFolder->GetItemCount();
		if (pBarFolder && iItem >= 0 && iItem < iItemCount)
		{
			// select the specified item.
			pBarFolder->SetSelItem( iItem );
			m_nLastItemSelected = iItem;

			if (bEnsureVisible)
			{
				int iFirst, iLast;
				GetVisibleRange(iFolder, iFirst, iLast);
				if (iLast != -1 && (iItem < iFirst || iItem > iLast))
				{
					m_nFirstItem = iItem-(iLast-iFirst);
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTOutBarItem
/////////////////////////////////////////////////////////////////////////////

CXTOutBarItem::CXTOutBarItem(LPCTSTR lpszName, const int nImageIndex, DWORD dwData)
: m_strName(lpszName)
, m_nIndex(nImageIndex)
, m_dwData(dwData)
, m_bSelected(false)
, m_bEnabled(true)
{

}

CXTOutBarItem::~CXTOutBarItem()
{

}

/////////////////////////////////////////////////////////////////////////////
// CXTOutBarFolder
/////////////////////////////////////////////////////////////////////////////

CXTOutBarFolder::CXTOutBarFolder(LPCTSTR lpszName, DWORD dwData, CXTOutBarCtrl* pControl)
: m_strName(lpszName)
, m_dwData(dwData)
, m_pLargeList(NULL)
, m_pSmallList(NULL)
, m_pChild(NULL)
, m_pControl(pControl)
{

}

CXTOutBarFolder::~CXTOutBarFolder()
{
	while(!m_barItems.IsEmpty()) {
		CXTOutBarItem* pBarItem = m_barItems.RemoveTail();
		SAFE_DELETE(pBarItem);
	}
}

int CXTOutBarFolder::InsertItem(int iIndex, LPCTSTR lpszName, const int nImage, const DWORD dwData)
{
	// if the iIndex is out of range, insert the item at the end
	// of the list.
	if(( iIndex < 0 ) || (iIndex > GetItemCount())) {
		iIndex = GetItemCount();
	}

	// allocate a new bar item.
	CXTOutBarItem* pBarItem = new CXTOutBarItem(lpszName, nImage, dwData);
	ASSERT(pBarItem);

	// insert the new item into the item array.
	InsertItemAt(iIndex, pBarItem);

	return iIndex;
}

CXTOutBarItem* CXTOutBarFolder::GetItemAt(int iIndex)
{
	POSITION pos = m_barItems.FindIndex(iIndex);
	return m_barItems.GetAt(pos);
}

void CXTOutBarFolder::InsertItemAt(int iIndex, CXTOutBarItem* pBarItem)
{
	POSITION pos = m_barItems.FindIndex(iIndex);
	m_barItems.InsertAfter(pos, pBarItem);
}

CXTOutBarItem* CXTOutBarFolder::RemoveItemAt(int iIndex)
{
	POSITION pos = m_barItems.FindIndex( iIndex );
	if ( pos != NULL )
	{
		CXTOutBarItem* pBarItem = m_barItems.GetAt( pos );
		if ( pBarItem != NULL )
		{
			m_barItems.RemoveAt( pos );
			return pBarItem;
		}
	}
	return NULL;
}

void CXTOutBarFolder::SetSelItem(int iItem)
{
	POSITION pos;
	for ( pos = m_barItems.GetHeadPosition( ); pos; m_barItems.GetNext( pos ) )
	{
		CXTOutBarItem* pBarItem = m_barItems.GetAt( pos );
		ASSERT( pBarItem != NULL );

		if ( pBarItem )
		{
			pBarItem->SelectItem( false );
		}
	}

	CXTOutBarItem* pBarItem = GetItemAt( iItem );
	if ( pBarItem != NULL )
	{
		pBarItem->SelectItem( true );
	}
}


CXTOutBarItem* CXTOutBarFolder::GetSelItem()
{
	POSITION pos;
	for ( pos = m_barItems.GetHeadPosition( ); pos; m_barItems.GetNext( pos ) )
	{
		CXTOutBarItem* pBarItem = m_barItems.GetAt( pos );
		if ( pBarItem == NULL )
		{
			return NULL;
		}

		if ( pBarItem && pBarItem->IsSelected( ) )
		{
			return pBarItem;
		}
	}
	return NULL;
}

int CXTOutBarFolder::GetSelIndex()
{
	int i;
	for ( i = 0; i < m_barItems.GetCount( ); ++i )
	{
		CXTOutBarItem* pBarItem = GetItemAt( i );
		if ( pBarItem == NULL )
		{
			return -1;
		}

		if ( pBarItem && pBarItem->IsSelected( ) )
		{
			return i;
		}
	}
	return -1;
}

void CXTOutBarCtrl::SetBackColor(COLORREF clrBack)
{
	GetTheme()->m_clrBack.SetCustomValue(clrBack);
}
void CXTOutBarCtrl::SetTextColor(COLORREF clrText)
{
	GetTheme()->m_clrText.SetCustomValue(clrText);
}


/////////////////////////////////////////////////////////////////////////////
// CXTToolBox
/////////////////////////////////////////////////////////////////////////////

CXTToolBox::CXTToolBox()
{
	m_bAnimating = false;
}

CXTToolBox::~CXTToolBox()
{
}

IMPLEMENT_DYNCREATE(CXTToolBox, CXTOutBarCtrl)

BEGIN_MESSAGE_MAP(CXTToolBox, CXTOutBarCtrl)
	//{{AFX_MSG_MAP(CXTToolBox)
	ON_WM_PAINT()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTToolBox message handlers

void CXTToolBox::GetItemRect(const int iFolder, const int iIndex, CRect& rect, CRect* pInsideRect /*= NULL*/)
{
	GetIconRect( iFolder, iIndex, rect, pInsideRect );
}

void CXTToolBox::GetIconRect(const int iFolder, const int iIndex, CRect& rcIcon, CRect* pInsideRect /*= NULL*/)
{
	CXTOutBarCtrl::GetIconRect( iFolder, iIndex, rcIcon, pInsideRect );

	CRect rcInside;
	if (pInsideRect) rcInside = *pInsideRect; else GetInsideRect(rcInside);

	rcIcon.left  = rcInside.left;
	rcIcon.right = rcInside.right;
	rcIcon.InflateRect( -2,4 );

	if ( ( m_bUpArrow   && iIndex == m_iFirst ) ||
		( m_bDownArrow && iIndex == m_iLast  ) ||
		( m_bDownArrow && iIndex == m_iLast-1) )
	{
		rcIcon.right -= XTAuxData().cxHThumb + 6;
	}
}

void CXTToolBox::DrawIcon(CDC* pDC, int iIcon, int iFolder, bool bHilight)
{
	CRect rcIcon;
	GetIconRect( iFolder, iIcon, rcIcon );

	pDC->Draw3dRect( rcIcon, GetTheme()->m_clrBack, GetTheme()->m_clrBack );

	// Should we highlight the icon?
	if ( bHilight )
	{
		if ( m_bIconPressed )
		{
			pDC->Draw3dRect( rcIcon,
				GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHILIGHT) );
		}
		else
		{
			pDC->Draw3dRect( rcIcon,
				GetXtremeColor(COLOR_3DHILIGHT), GetXtremeColor(COLOR_3DSHADOW) );
		}
	}

	// Draw the icon using the image list.
	ASSERT( iFolder >= 0 && iFolder < GetFolderCount( ) );
	CXTOutBarFolder* pBarFolder = ( CXTOutBarFolder* )m_arFolder.GetAt( iFolder );

	ASSERT( iIcon >= 0 && iIcon < pBarFolder->GetItemCount( ) );
	CXTOutBarItem* pBarItem = pBarFolder->GetItemAt(iIcon);

	CImageList* pImageList = GetFolderImageList( iFolder, IsSmallIconView( iFolder ) );
	ASSERT( pBarItem && pImageList );

	CRect rc;
	GetItemRect( iFolder, iIcon, rc );

	CRect rcFolder;
	GetFolderRect(iFolder, rcFolder);
	rc.left  = rcFolder.left +4;
	rc.right = rcFolder.right-4;

	if (pImageList)
	{
		IMAGEINFO ii;
		pImageList->GetImageInfo(pBarItem->GetIndex(), &ii);
		CSize szImage = CRect(ii.rcImage).Size();
		CPoint pt;
		pt.x = rc.left + 2;
		pt.y = rc.top + (rc.Height() - szImage.cy) / 2;

		DrawItemIcon( pDC, pt, pBarItem, pImageList);
	}
}

void CXTToolBox::DrawItem(CDC* pDC, const int iFolder, CRect rc, const int iIndex, const BOOL bOnlyImage)
{
	if (m_arFolder.GetSize() != 0)
	{
		CImageList* pImageList = GetFolderImageList(iFolder, IsSmallIconView(iFolder));

		ASSERT(iFolder >= 0 && iFolder < GetFolderCount());
		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(iFolder);

		ASSERT(iIndex >= 0 && iIndex < pBarFolder->GetItemCount());
		CXTOutBarItem* pBarItem = pBarFolder->GetItemAt(iIndex);

		ASSERT(pBarItem && pImageList);

		CFont* oft = (CFont*)pDC->SelectObject(GetFontX());
		COLORREF ocr = pDC->SetTextColor(GetXtremeColor(COLOR_WINDOW));
		int obk = pDC->SetBkMode(TRANSPARENT);

		CRect rcFolder;
		GetFolderRect(iFolder, rcFolder);
		rc.left  = rcFolder.left +4;
		rc.right = rcFolder.right-4;

		// Exclude the up and down arrows from painting.
		if (m_bUpArrow)
			pDC->ExcludeClipRect(&m_rcUpArrow);
		if (m_bDownArrow)
			pDC->ExcludeClipRect(&m_rcDownArrow);

		// set the items text color.
		pDC->SetTextColor(GetTheme()->m_clrText);

		if ( pBarItem && pBarItem->IsSelected( ) && !m_bAnimating && m_nLastItemSelected >= 0 )
		{
			CRect rc;
			GetIconRect(m_nSelFolder, m_nLastItemSelected, rc);
			pDC->FillSolidRect( rc, GetXtremeColor(COLOR_3DLIGHT) );
			pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHILIGHT));
		}

		if (pImageList)
		{
			IMAGEINFO ii;
			pImageList->GetImageInfo(pBarItem->GetIndex(), &ii);
			CSize szImage = CRect(ii.rcImage).Size();
			CPoint pt;
			pt.x = rc.left + 2;
			pt.y = rc.top + (rc.Height() - szImage.cy) / 2;

			DrawItemIcon( pDC, pt, pBarItem, pImageList);

			if (!bOnlyImage)
			{
				rc.left  += (szImage.cx + m_sizeOffset.cx);
				DrawItemText(pDC, rc, pBarItem, DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			}
		}

		pDC->SetTextColor(ocr);
		pDC->SelectObject(oft);
		pDC->SetBkMode(obk);
	}
}

void CXTToolBox::DrawScrollButton(CDC* pDC, CRect rect, UINT uType, UINT uState)
{
	rect.InflateRect(1,2,1,2);

	CRect rcInside;
	GetInsideRect( rcInside );
	rcInside.DeflateRect(2,2);

	rect.right = rcInside.right;
	rect.left  = rect.right - rect.Width();

	rect.DeflateRect(1,1,1,1);
	CXTOutBarCtrl::DrawScrollButton( pDC, rect, uType, uState );

	rect.DeflateRect(1,1,1,1);
	pDC->Draw3dRect( &rect, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DFACE) );

	rect.InflateRect(1,1,1,1);
	if ( ( uState & DFCS_PUSHED ) == DFCS_PUSHED )
	{
		pDC->Draw3dRect( &rect, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHILIGHT) );
	}
	else
	{
		pDC->Draw3dRect( &rect, GetXtremeColor(COLOR_3DHILIGHT), GetXtremeColor(COLOR_3DSHADOW) );
	}
}

void CXTToolBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	GetTheme()->m_clrBack.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	GetTheme()->m_clrText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));

	// Get the client rect.
	CRect rectClient;
	GetClientRect(&rectClient);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTMemDC memDC(&dc, rectClient);
	//int nSavedDC = memDC.SaveDC();

	CRect rc;
	GetInsideRect(rc);
	if (!GetFolderChild())
	{
		memDC.FillSolidRect(rc, GetTheme()->m_clrBack);
	}

	int max = (int)m_arFolder.GetSize();
	CRect frc;
	int t;
	for (t = 0; t < max; t++)
	{
		GetFolderRect(t, frc);
		DrawFolder(&memDC, t, frc, xtMouseNormal);
	}
	if (!GetFolderChild())
	{
		GetVisibleRange(m_nSelFolder, m_iFirst,m_iLast);
		m_rcUpArrow.SetRect(0,0,GetSystemMetrics(SM_CXVSCROLL), GetSystemMetrics(SM_CXVSCROLL));
		m_rcDownArrow = m_rcUpArrow;
		m_rcUpArrow.OffsetRect(rc.right - 5 - GetSystemMetrics(SM_CXVSCROLL), rc.top +5);
		m_rcDownArrow.OffsetRect(rc.right - 5 - GetSystemMetrics(SM_CXVSCROLL), rc.bottom - 5 - GetSystemMetrics(SM_CXVSCROLL));

		if (m_iFirst > 0)
		{
			if (m_bUpPressed)
			{
				DrawScrollButton( &memDC, m_rcUpArrow, DFC_SCROLL, DFCS_SCROLLUP|DFCS_PUSHED );
			}
			else
			{
				DrawScrollButton( &memDC, m_rcUpArrow, DFC_SCROLL, DFCS_SCROLLUP );
			}
			m_bUpArrow = TRUE;
			memDC.ExcludeClipRect(m_rcUpArrow);
		}
		else m_bUpArrow = FALSE;

		if (m_iLast < GetItemCount() - 1)
		{
			if (m_bDownPressed)
			{
				DrawScrollButton( &memDC, m_rcDownArrow, DFC_SCROLL, DFCS_SCROLLDOWN|DFCS_PUSHED );
			}
			else
			{
				DrawScrollButton( &memDC, m_rcDownArrow, DFC_SCROLL, DFCS_SCROLLDOWN );
			}
			m_bDownArrow = TRUE;
			memDC.ExcludeClipRect(m_rcDownArrow);
		}
		else m_bDownArrow = FALSE;

		PaintItems(&memDC, m_nSelFolder, rc);
	}

	//dc.BitBlt(rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), &memDC, 0,0, SRCCOPY);

	//memDC.RestoreDC(nSavedDC);

	if (m_nFolderHilighted >= 0)
	{
		int i = m_nFolderHilighted;
		m_nFolderHilighted = -1;
		HighlightFolder(&memDC, i);
	}
	if (m_nItemHilighted >= 0)
	{
		int i = m_nItemHilighted;
		m_nItemHilighted = -1;
		HighlightItem(&memDC, i, m_bPressedHighlight);
	}
}

void CXTToolBox::DrawFolder(CDC* pDC, const int iIndex, CRect rect, const XTMouseState eHilight)
{
	if (m_arFolder.GetSize() != 0)
	{
		pDC->FillSolidRect(rect, GetTheme()->m_clrBack);

		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(iIndex);

		int nSavedDC = pDC->SaveDC();

		CPen pen(PS_SOLID, 1, GetXtremeColor(COLOR_3DSHADOW));

		CRect rcInside;
		GetInsideRect( rcInside );

		CRect rcFolder( rect );
		rcFolder.left  = rcInside.left  + 2;
		rcFolder.right = rcInside.right - 2;
		rcFolder.DeflateRect( 0,1 );

		switch (eHilight)
		{
		case xtMouseNormal:
		case xtMouseHover:
			{
				pDC->FillSolidRect(rcFolder, GetXtremeColor(COLOR_3DFACE));
				pDC->Draw3dRect(rcFolder, GetXtremeColor(COLOR_3DHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
				pDC->SetBkMode(TRANSPARENT);
				pDC->SelectObject(GetFontX());

				rcFolder.DeflateRect( 4,0 );
				pDC->DrawText(pBarFolder->GetName(), rcFolder,
					DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			}
			break;

		case xtMouseSelect:
			{
				pDC->FillSolidRect(rcFolder, GetXtremeColor(COLOR_3DFACE));
				pDC->Draw3dRect(rcFolder, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHILIGHT));
				pDC->SetBkMode(TRANSPARENT);
				pDC->SelectObject(GetFontX());

				rcFolder.DeflateRect( 4,0 );
				pDC->DrawText(pBarFolder->GetName(), rcFolder,
					DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			}
			break;
		}

		pDC->RestoreDC(nSavedDC);
		pen.DeleteObject();
	}
}

void CXTToolBox::SetSelFolder(const int iIndex)
{
	if (m_arFolder.GetSize() != 0)
	{
		ASSERT(iIndex >= 0 && iIndex < GetFolderCount());

		CXTOutBarFolder* pBarFolder = (CXTOutBarFolder*)m_arFolder.GetAt(iIndex);

		CWnd* pOwner = (CWnd*)GetOwner();
		ASSERT_VALID(pOwner);

		XT_OUTBAR_INFO obi;
		obi.nIndex    = iIndex;
		obi.nDragFrom = -1;
		obi.nDragTo   = -1;
		obi.lpszText  = (LPCTSTR)pBarFolder->GetName();
		obi.bFolder   = true;

		LRESULT lResult = pOwner->SendMessage(XTWM_OUTBAR_NOTIFY,
			OBN_XT_FOLDERCHANGE, (LPARAM)&obi);

		if (lResult == TRUE)
		{
			CWnd* pWnd = GetFolderChild();
			if (pWnd) {
				pWnd->ShowWindow(SW_HIDE);
			}

			if (iIndex != m_nSelFolder && m_nSelFolder >= 0)
			{
				if ((m_dwFlags & OBS_XT_ANIMATION) && (m_nAnimationTickCount >= 0))
				{
					m_bAnimating = true;
					AnimateFolderScroll(m_nSelFolder, iIndex);
					m_bAnimating = false;
				}
			}

			m_nSelFolder = iIndex;
			m_nFirstItem = 0;
			m_nLastItemSelected = pBarFolder->GetSelIndex( );

			if ( m_nLastItemSelected != -1 )
			{
				pBarFolder->SetSelItem( m_nLastItemSelected );
			}

			pWnd = GetFolderChild();
			if (pWnd)
			{
				CRect rc;
				GetInsideRect(rc);
				pWnd->MoveWindow(rc);
				pWnd->ShowWindow(SW_SHOW);
			}

			InvalidateRect(NULL);
		}
	}
}

BOOL CXTToolBox::CreateEx(DWORD dwExStyle, DWORD dwStyle, const RECT&  rect, CWnd* pParentWnd, UINT nID, const DWORD dwFlags)
{
	if ( !CXTOutBarCtrl::CreateEx( dwExStyle, dwStyle, rect, pParentWnd, nID,
		dwFlags|OBS_XT_SELHIGHLIGHT|OBS_XT_ANIMATION ) )
	{
		return FALSE;
	}

	SetSmallIconView( TRUE );

	return TRUE;
}

void CXTToolBox::OnRButtonUp(UINT nFlags, CPoint point)
{
	// bypass base class.
	CWnd::OnRButtonUp(nFlags, point);
}
