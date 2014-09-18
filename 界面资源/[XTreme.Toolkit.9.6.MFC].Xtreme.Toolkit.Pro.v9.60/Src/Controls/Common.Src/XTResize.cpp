// XTResize.cpp: implementation of the CXTResize class.
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
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTResizeRect.h"
#include "XTResizePoint.h"
#include "XTResize.h"
#include "XTResizeGroupBox.h"
#include "XTPDrawHelpers.h"
#include "XTMemDC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef OBM_SIZE
#define OBM_SIZE 32766
#endif

#define ENTRY_WINDOWPLACEMENT _T("WindowPlacement")

#define IDC_SIZEICON 0x7FFF

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTResize::CXTResize(CWnd* pWnd, const UINT nFlags) :
	m_pWnd(pWnd),
	m_nFlagsXX(nFlags),
	m_szMin(0,0),
	m_szMax(0,0),
	m_rcWindow(0,0,0,0),
	m_strSection(_T(""))
{

}

CXTResize::~CXTResize()
{
	RemoveAllControls();
}

//////////////////////////////////////////////////////////////////////

BOOL CXTResize::RemoveResize(const UINT nID)
{
	// search for an item with the given id
	int iItem;
	for (iItem = (int)m_arrItems.GetSize(); iItem--; )
	{
		CWnd* pWnd = m_arrItems[iItem]->m_pWnd;
		ASSERT_VALID(pWnd);

		if (pWnd->GetDlgCtrlID() == (int) nID)
		{
			CXTResizeItem* pItem = m_arrItems[iItem];
			if (pItem != NULL)
			{
				m_arrItems.RemoveAt(iItem);
				SAFE_DELETE( pItem );
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CXTResize::RemoveAllControls()
{
	// free allocated memory
	int iIndex;
	for (iIndex = 0; iIndex < m_arrItems.GetSize(); ++iIndex)
	{
		CXTResizeItem* pItem = m_arrItems.GetAt(iIndex);
		SAFE_DELETE( pItem );
	}

	// empty array
	m_arrItems.RemoveAll();
}

void CXTResize::SetResize(const UINT nID, const HWND hWnd, const XT_RESIZERECT& rrcSizing)
{
	CXTResizeItem* pItem = 0;

	// search for an item with the given id
	int i;
	for ( i = (int)m_arrItems.GetSize(); i--; )
	{
		CWnd* pWnd = m_arrItems[i]->m_pWnd;
		if (pWnd && ::IsWindow( pWnd->m_hWnd ) && ( pWnd->GetDlgCtrlID( ) == ( int )nID ) )
		{
			pItem = m_arrItems[i];
		}
	}

	// if we didn't find it then create one
	if (pItem == 0)
	{
		HWND hWndChild = hWnd;
		if ( hWndChild == NULL )
		{
			CWnd* pWnd = m_pWnd->GetDlgItem( nID );
			if ( pWnd &&  ::IsWindow( pWnd->m_hWnd ) )
			{
				hWndChild = pWnd->m_hWnd;
			}
		}

		bool bDelete = false;

		CWnd* pWnd = m_pWnd->FromHandlePermanent( hWndChild );
		if ( pWnd == NULL )
		{
			// there is no permanent window, create one
			pWnd = new CWnd; // CXTResizeItem handles the delete
			pWnd->Attach( hWndChild );

			bDelete = true;
		}

		CRect rcWindow;
		pWnd->GetWindowRect( rcWindow );
		m_pWnd->ScreenToClient( rcWindow );

		pItem = new CXTResizeItem(pWnd, rrcSizing, rcWindow, bDelete);
		pItem->MakeTransparent(this);

		m_arrItems.Add( pItem );
	}
	else
	{
		// the item already exists, just add the new sizing option
		pItem->m_rrcSizing += rrcSizing;
	}
	// we should only allow sizing within the rect {0,0,1,1}
	ASSERT((CXTResizeRect(0,0,1,1) & pItem->m_rrcSizing) == pItem->m_rrcSizing);
}

void CXTResize::Init()
{
	if(m_pWnd == NULL)
		return;

	// clear the control list when initializing.  The reason for this is we may have a
	// static window that calls Init() multiple times for example CDialog::DoModal().
	RemoveAllControls();

	// get the dialog size
	m_pWnd->GetWindowRect(m_rcInitWindow);
	m_rcWindow = m_rcInitWindow;

	if (!HasFlag(xtResizeNoMinsize))
	{
		// set minimum size to current window size
		m_szMin.cx = m_rcWindow.Width();
		m_szMin.cy = m_rcWindow.Height();
	}

	// set the clip children style to prevent flickering
	if (!HasFlag(xtResizeNoClipChildren))
	{
		m_pWnd->ModifyStyle(0, WS_CLIPCHILDREN);
	}

	// add the size icon
	if (!HasFlag(xtResizeNoSizeIcon))
	{
		// Create the size icon if it already doesn't exist.
		if (!::IsWindow(m_scSizeIcon.m_hWnd))
		{
			CXTPClientRect rcIcon(m_pWnd);
			rcIcon.OffsetRect(-1,-1);
			rcIcon.left = rcIcon.right  - XTAuxData().cxSmIcon;
			rcIcon.top  = rcIcon.bottom - XTAuxData().cySmIcon;

			m_scSizeIcon.Create(WS_CHILD | WS_VISIBLE | SBS_SIZEGRIP,
				rcIcon, m_pWnd, IDC_SIZEICON);
		}

		m_scSizeIcon.SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0,
			SWP_NOSIZE|SWP_NOMOVE|SWP_FRAMECHANGED);

		SetResize(IDC_SIZEICON, SZ_REPOS(1));
	}
}

void CXTResize::Size()
{
	if (m_arrItems.GetSize() > 0)
	{
		CRect rcWindow;
		m_pWnd->GetWindowRect(rcWindow);

		int dx = rcWindow.Width()  - m_rcWindow.Width();
		int dy = rcWindow.Height() - m_rcWindow.Height();

		BOOL bDefered = FALSE;
		HDWP hDWP = ::BeginDeferWindowPos((int)m_arrItems.GetSize());

		int i;
		for (i = 0; i < (int)m_arrItems.GetSize(); i++)
		{
			if (Defer(hDWP, m_arrItems[i], dx, dy))
				bDefered = TRUE;
		}

		if (bDefered)
			::EndDeferWindowPos(hDWP);

		// refresh group box items.
		for (i = 0; i < m_arrItems.GetSize(); i++)
		{
			if (m_arrItems[i]->m_bIsGroupBox)
				m_arrItems[i]->m_pWnd->Invalidate();
		}

		m_rcWindow = rcWindow;
	}
}

void CXTResize::Reset()
{
	int iSize = (int)m_arrItems.GetSize();
	int i;
	for (i = 0; i < iSize; i++)
	{
		CXTResizeItem* pItem = m_arrItems[i];
		pItem->m_bInitialSize = FALSE;
		pItem->m_rrcWindow = pItem->m_rrcInitWindow;
	}
}

BOOL CXTResize::Defer(HDWP& hDWP, CXTResizeItem* pItem, int dx, int dy)
{
	HWND hWnd = pItem->m_pWnd->m_hWnd;
	if (::IsWindow(hWnd))
	{
		if (!pItem->m_bInitialSize)
		{
			CRect rcWindow;
			m_pWnd->GetWindowRect(rcWindow);

			dx = rcWindow.Width()  - m_rcInitWindow.Width();
			dy = rcWindow.Height() - m_rcInitWindow.Height();

			pItem->m_bInitialSize = TRUE;
		}

		CXTResizeRect rrcItem = pItem->m_rrcWindow;

		rrcItem.left   += dx * pItem->m_rrcSizing.left;
		rrcItem.top    += dy * pItem->m_rrcSizing.top;
		rrcItem.right  += dx * pItem->m_rrcSizing.right;
		rrcItem.bottom += dy * pItem->m_rrcSizing.bottom;

		if (rrcItem != pItem->m_rrcWindow)
		{
			int x  = (int) rrcItem.left;
			int y  = (int) rrcItem.top;
			int cx = (int) rrcItem.Width();
			int cy = (int) rrcItem.Height();

			// Set positioning flags
			UINT uFlags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER;

			CRect rOld;
			CRect rNew = rrcItem;

			// get the size of the dialog item in client coordinates.
			pItem->m_pWnd->GetWindowRect(&rOld);
			m_pWnd->ScreenToClient(&rOld);

			// if the x-y coordinates have not changed, there is no reason
			// to move the dialog item.
			if (rNew.TopLeft() == rOld.TopLeft())
				uFlags |= SWP_NOMOVE;

			// if the cx-cy size has not changed, there is no reason to
			// size the dialog item.  If size has changed, don't
			// copy bits of the client area (i.e. have them invalidated/redrawn)
			if (rNew.Size() == rOld.Size())
				uFlags |= SWP_NOSIZE;
			else
				uFlags |= SWP_NOCOPYBITS;

			hDWP = ::DeferWindowPos(hDWP, hWnd, 0, x, y, cx, cy, uFlags);
			if (hDWP ==  NULL)
			{
				TRACE(_T("DeferWindowPos failed for ID %i\n"), GetDlgCtrlID(hWnd));
				return FALSE;
			}
			pItem->m_rrcWindow = rrcItem;
		}
	}
	return TRUE;
}

void CXTResize::GetMinMaxInfo(MINMAXINFO* pMMI)
{
	if (!m_rcWindow.IsRectNull())
	{
		if (HasFlag(xtResizeNoHorisontal))
		{
			pMMI->ptMaxTrackSize.x = pMMI->ptMaxSize.x = m_rcWindow.Width();
		}
		if (HasFlag(xtResizeNoVertical))
		{
			pMMI->ptMaxTrackSize.y = pMMI->ptMaxSize.y = m_rcWindow.Height();
		}
	}
	if (m_szMin.cx != 0) pMMI->ptMinTrackSize.x = m_szMin.cx;
	if (m_szMin.cy != 0) pMMI->ptMinTrackSize.y = m_szMin.cy;
	if (m_szMax.cx != 0) pMMI->ptMaxTrackSize.x = m_szMax.cx;
	if (m_szMax.cy != 0) pMMI->ptMaxTrackSize.y = m_szMax.cy;
}

void CXTResize::AutoLoadPlacement(LPCTSTR pszSection)
{
	m_strSection = pszSection;
	ASSERT(!m_strSection.IsEmpty());
	LoadPlacement(m_strSection);
}

void CXTResize::LoadPlacement(LPCTSTR pszSection)
{
	UINT nBytes = 0;
	BYTE* pBytes = 0;
	AfxGetApp()->GetProfileBinary(pszSection, ENTRY_WINDOWPLACEMENT, &pBytes, &nBytes);
	if (nBytes == sizeof(WINDOWPLACEMENT))
	{
		m_pWnd->SetWindowPlacement((WINDOWPLACEMENT*) pBytes);
	}
	if (pBytes && nBytes) delete[] pBytes;

}

void CXTResize::SavePlacement(LPCTSTR pszSection)
{
	WINDOWPLACEMENT wp;
	m_pWnd->GetWindowPlacement(&wp);
	AfxGetApp()->WriteProfileBinary(pszSection, ENTRY_WINDOWPLACEMENT, (BYTE*) &wp, sizeof(wp));
}

void CXTResize::SetResize(XT_SIZING arr[])
{
	int i;
	for (i=0; i < sizeof(arr)/sizeof(arr[0]); i++)
	{
		XT_SIZING& sz = arr[i];
		SetResize(sz.id, sz.rrc.left, sz.rrc.top, sz.rrc.right, sz.rrc.bottom);
	}
}

void CXTResize::SetFlag(XTResize eFlag)
{
	m_nFlagsXX &= (eFlag^0xFFFFFFFF);
	m_nFlagsXX |= eFlag;
}

void CXTResize::SetResize(CWnd* pWnd, const CXTResizeRect& rrcSizing, CRect rcWindow)
{
	CXTResizeItem *pItem = new CXTResizeItem(pWnd, rrcSizing, rcWindow, FALSE);
	m_arrItems.Add(pItem);
}

//////////////////////////////////////////////////////////////////////
// CXTResizeItem
//////////////////////////////////////////////////////////////////////

CXTResizeItem::CXTResizeItem(CWnd* pWnd, const CXTResizeRect& rrcSizing, CRect& rcWindow, BOOL bAutoDelete)
: m_pWnd(pWnd)
, m_rrcSizing(rrcSizing)
, m_rrcWindow(rcWindow)
, m_rrcInitWindow(rcWindow)
, m_bAutoDelete(bAutoDelete)
, m_bInitialSize(FALSE)
, m_bIsGroupBox(FALSE)
{

}

CXTResizeItem::~CXTResizeItem()
{
	if (m_bAutoDelete)
	{
		m_pWnd->Detach();
		SAFE_DELETE(m_pWnd);
	}
}

bool CXTResizeItem::MakeTransparent(CXTResize* pXTResize)
{
	if (pXTResize->HasFlag(xtResizeNoTransparentGroup))
	{
		return false;
	}

	if (m_pWnd && ::IsWindow(m_pWnd->m_hWnd))
	{
		TCHAR szClassName[8];
		::GetClassName(m_pWnd->m_hWnd, szClassName, 8);

		// not a CButton derived class.
		if (_tcsicmp(szClassName, _T("Button")) != 0)
		{
			return false;
		}

		// get the window style if not a group box, return.
		DWORD dwStyle = ::GetWindowLong(m_pWnd->m_hWnd, GWL_STYLE);
		if ((dwStyle & (BS_GROUPBOX|WS_TABSTOP)) == BS_GROUPBOX)
		{
			// we don’t want CXTResizeGroupBox transparent.
			if (m_pWnd->IsKindOf(RUNTIME_CLASS(CXTResizeGroupBox)))
			{
				return false;
			}

			// Get the extended style for the group box.
			DWORD dwStyleEx = ::GetWindowLong( m_pWnd->m_hWnd, GWL_EXSTYLE );

			// add the WS_EX_TRANSPARENT flag to the group box.
			::SetWindowLong( m_pWnd->m_hWnd, GWL_EXSTYLE,
				dwStyleEx|WS_EX_TRANSPARENT );

			// apply the style for the window.
			::SetWindowPos( m_pWnd->m_hWnd, NULL, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|
				SWP_NOZORDER|SWP_NOACTIVATE|SWP_FRAMECHANGED );

			m_bIsGroupBox = TRUE;

			return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////
// CXTSizeIcon
//////////////////////////////////////////////////////////////////////

CXTSizeIcon::CXTSizeIcon()
{
	// Load the size cursor
	m_hCursor = ::LoadCursor(NULL, IDC_SIZENWSE);
}

CXTSizeIcon::~CXTSizeIcon()
{
	if (m_hCursor != NULL)
		::DestroyCursor(m_hCursor);
}

BEGIN_MESSAGE_MAP(CXTSizeIcon, CScrollBar)
	//{{AFX_MSG_MAP(CXTSizeIcon)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTSizeIcon message handlers

BOOL CXTSizeIcon::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// Set the cursor to the size cursor.
	if (nHitTest == HTCLIENT && m_hCursor != NULL)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}

	return CScrollBar::OnSetCursor(pWnd, nHitTest, message);
}
