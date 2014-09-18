// XTPReBar.cpp : implementation file
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
#include "XTPVC80Helpers.h"
#include "XTPDrawHelpers.h"

#include "XTPReBar.h"
#include "XTPToolBar.h"
#include "XTPDockBar.h"


#ifndef VERSION_IE401
#define VERSION_IE401   MAKELONG(72, 4)
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const LPCTSTR CXTPReBar::m_lpszStateInfoEntry  = _T("RebarStateInfo (v1.01)");
const LPCTSTR CXTPReBar::m_lpszStateInfoFormat = _T("wID=%04X,cx=%d,fStyle=%08X");

//////////////////////////////////////////////////////////////////////////
//

#if _MSC_VER < 1200 // MFC 6.0

/////////////////////////////////////////////////////////////////////////////
// CXTPReBarBase

#include <shlwapi.h>

IMPLEMENT_DYNAMIC(CXTPReBarBase, CControlBar)


BEGIN_MESSAGE_MAP(CXTPReBarBase, CControlBar)
	//{{AFX_MSG_MAP(CXTPReBarBase)
	ON_WM_NCCREATE()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_NOTIFY_REFLECT(RBN_HEIGHTCHANGE, OnHeightChange)
	ON_NOTIFY_REFLECT(RBN_ENDDRAG, OnHeightChange)
	ON_MESSAGE(RB_SHOWBAND, OnShowBand)
	ON_MESSAGE_VOID(WM_RECALCPARENT, OnRecalcParent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CXTPReBarBase::CXTPReBarBase()
	: m_iComCtlVersion(-1)
{
	m_cxLeftBorder = m_cxRightBorder = m_cyTopBorder = m_cyBottomBorder = 0;
	GetComCtlVersion();
}

void CXTPReBarBase::GetComCtlVersion()
{
	// otherwise determine comctl32.dll version via DllGetVersion
	HINSTANCE hInst = ::LoadLibraryA("COMCTL32.DLL");
	ASSERT(hInst != NULL);

	DLLGETVERSIONPROC pfn = (DLLGETVERSIONPROC)
		::GetProcAddress(hInst, "DllGetVersion");

	if (pfn != NULL)
	{
		DLLVERSIONINFO dvi;
		memset(&dvi, 0, sizeof(dvi));
		dvi.cbSize = sizeof(dvi);

		if (SUCCEEDED((*pfn)(&dvi)))
		{
			ASSERT(dvi.dwMajorVersion <= 0xFFFF);
			ASSERT(dvi.dwMinorVersion <= 0xFFFF);
			DWORD dwVersion = MAKELONG(dvi.dwMinorVersion, dvi.dwMajorVersion);

			m_iComCtlVersion = dwVersion;
		}
	}

	FreeLibrary(hInst);
}

void CXTPReBarBase::OnRecalcParent()
{
	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT(pFrameWnd != NULL);
	pFrameWnd->RecalcLayout();
}

void CXTPReBarBase::OnHeightChange(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	// gives us access to protected member m_bInRecalcLayout.
	class CFriendFrameWnd : public CFrameWnd
	{
		friend class CXTPReBarBase;
	};

	// does the CXTPReBarBase have a frame?
	CFriendFrameWnd* pFrameWnd = (CFriendFrameWnd*)GetParentFrame();
	if (pFrameWnd != NULL)
	{
		// it does -- tell it to recalc its layout
		if (!pFrameWnd->m_bInRecalcLayout)
			pFrameWnd->RecalcLayout();
		else
			PostMessage(WM_RECALCPARENT);
	}
	*pResult = 0;
}

LRESULT CXTPReBarBase::OnShowBand(WPARAM wParam, LPARAM)
{
	LRESULT lResult = Default();
	if (lResult)
	{
		// keep window visible state in sync with band visible state
		REBARBANDINFO rbBand;
		rbBand.cbSize = sizeof(rbBand);
		rbBand.fMask = RBBIM_CHILD|RBBIM_STYLE;
		VERIFY(DefWindowProc(RB_GETBANDINFO, wParam, (LPARAM)&rbBand));
		CControlBar* pBar = DYNAMIC_DOWNCAST(CControlBar, CWnd::FromHandlePermanent(rbBand.hwndChild));
		BOOL bWindowVisible;
		if (pBar != NULL)
			bWindowVisible = pBar->IsVisible();
		else
			bWindowVisible =  (::GetWindowLong(rbBand.hwndChild, GWL_STYLE) & WS_VISIBLE) != 0;
		BOOL bBandVisible = (rbBand.fStyle & RBBS_HIDDEN) == 0;
		if (bWindowVisible != bBandVisible)
			VERIFY(::ShowWindow(rbBand.hwndChild, bBandVisible ? SW_SHOW : SW_HIDE));
	}
	return lResult;
}

BOOL CXTPReBarBase::_AddBar(CWnd* pBar, REBARBANDINFO* pRBBI)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pBar != NULL);
	ASSERT(::IsWindow(pBar->m_hWnd));

	pRBBI->cbSize = sizeof(REBARBANDINFO);
	pRBBI->fMask |= RBBIM_CHILD | RBBIM_CHILDSIZE;
	pRBBI->hwndChild = pBar->m_hWnd;

	CSize size;
	CControlBar* pTemp = DYNAMIC_DOWNCAST(CControlBar, pBar);
	if (pTemp != NULL)
	{
		size = pTemp->CalcFixedLayout(FALSE, m_dwStyle & CBRS_ORIENT_HORZ);
	}
	else
	{
		CRect rect;
		pBar->GetWindowRect(&rect);
		size = rect.Size();
	}
	//WINBUG: COMCTL32.DLL is off by 4 pixels in its sizing logic.  Whatever
	//  is specified as the minimum size, the system rebar will allow that band
	//  to be 4 actual pixels smaller!  That's why we add 4 to the size here.
	ASSERT(m_iComCtlVersion != -1);
	pRBBI->cxMinChild = size.cx + (m_iComCtlVersion < VERSION_IE401 ? 4 : 0);
	pRBBI->cyMinChild = size.cy;
	BOOL bResult = (BOOL)DefWindowProc(RB_INSERTBAND, (WPARAM)-1, (LPARAM)pRBBI);

	CFrameWnd* pFrameWnd = GetParentFrame();
	if (pFrameWnd != NULL)
		pFrameWnd->RecalcLayout();

	return bResult;
}

BOOL CXTPReBarBase::AddBar(CWnd* pBar, LPCTSTR pszText, CBitmap* pbmp, DWORD dwStyle)
{
	REBARBANDINFO rbBand;
	rbBand.fMask = RBBIM_STYLE;
	rbBand.fStyle = dwStyle;
	if (pszText != NULL)
	{
		rbBand.fMask |= RBBIM_TEXT;
		rbBand.lpText = const_cast<LPTSTR>(pszText);
	}
	if (pbmp != NULL)
	{
		rbBand.fMask |= RBBIM_BACKGROUND;
		rbBand.hbmBack = (HBITMAP)*pbmp;
	}
	return _AddBar(pBar, &rbBand);
}

BOOL CXTPReBarBase::AddBar(CWnd* pBar, COLORREF clrFore, COLORREF clrBack, LPCTSTR pszText, DWORD dwStyle)
{
	REBARBANDINFO rbBand;
	rbBand.fMask = RBBIM_STYLE | RBBIM_COLORS;
	rbBand.fStyle = dwStyle;
	rbBand.clrFore = clrFore;
	rbBand.clrBack = clrBack;
	if (pszText != NULL)
	{
		rbBand.fMask |= RBBIM_TEXT;
		rbBand.lpText = const_cast<LPTSTR>(pszText);
	}
	return _AddBar(pBar, &rbBand);
}

CSize CXTPReBarBase::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	// the union of the band rectangles is the total bounding rect
	int nCount = DefWindowProc(RB_GETBANDCOUNT, 0, 0);
	REBARBANDINFO rbBand;
	rbBand.cbSize = sizeof(rbBand);
	int nTemp;

	// sync up hidden state of the bands
	for (nTemp = nCount; nTemp--; )
	{
		rbBand.fMask = RBBIM_CHILD|RBBIM_STYLE;
		VERIFY(DefWindowProc(RB_GETBANDINFO, nTemp, (LPARAM)&rbBand));
		CControlBar* pBar = DYNAMIC_DOWNCAST(CControlBar, CWnd::FromHandlePermanent(rbBand.hwndChild));
		BOOL bWindowVisible;
		if (pBar != NULL)
			bWindowVisible = pBar->IsVisible();
		else
			bWindowVisible =  (::GetWindowLong(rbBand.hwndChild, GWL_STYLE) & WS_VISIBLE) != 0;
		BOOL bBandVisible = (rbBand.fStyle & RBBS_HIDDEN) == 0;
		if (bWindowVisible != bBandVisible)
			VERIFY(DefWindowProc(RB_SHOWBAND, nTemp, bWindowVisible));
	}

	// determine bounding rect of all visible bands
	CRect rectBound; rectBound.SetRectEmpty();
	for (nTemp = nCount; nTemp--; )
	{
		rbBand.fMask = RBBIM_STYLE;
		VERIFY(DefWindowProc(RB_GETBANDINFO, nTemp, (LPARAM)&rbBand));
		if ((rbBand.fStyle & RBBS_HIDDEN) == 0)
		{
			CRect rect;
			VERIFY(DefWindowProc(RB_GETRECT, nTemp, (LPARAM)&rect));
			rectBound |= rect;
		}
	}

	// add borders as part of bounding rect
	if (!rectBound.IsRectEmpty())
	{
		CRect rect; rect.SetRectEmpty();
		CalcInsideRect(rect, bHorz);
		rectBound.right -= rect.Width();
		rectBound.bottom -= rect.Height();
	}

	return CSize((bHorz && bStretch) ? 32767 : rectBound.Width(),
		(!bHorz && bStretch) ? 32767 : rectBound.Height());
}

CSize CXTPReBarBase::CalcDynamicLayout(int /*nLength*/, DWORD dwMode)
{
	if (dwMode & LM_HORZDOCK)
		ASSERT(dwMode & LM_HORZ);
	return CalcFixedLayout(dwMode & LM_STRETCH, dwMode & LM_HORZ);
}

BOOL CXTPReBarBase::Create(CWnd* pParentWnd, DWORD dwCtrlStyle, DWORD dwStyle, UINT nID)
{
	ASSERT_VALID(pParentWnd);   // must have a parent
	ASSERT (!((dwStyle & CBRS_SIZE_FIXED) && (dwStyle & CBRS_SIZE_DYNAMIC)));

	// save the style
	m_dwStyle = (dwStyle & CBRS_ALL);
	if (nID == AFX_IDW_REBAR)
		m_dwStyle |= CBRS_HIDE_INPLACE;

	dwStyle &= ~CBRS_ALL;
	dwStyle |= CCS_NOPARENTALIGN|CCS_NOMOVEY|CCS_NODIVIDER|CCS_NORESIZE|RBS_VARHEIGHT;
	dwStyle |= dwCtrlStyle;

	// initialize common controls
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(icex);
	icex.dwICC = ICC_COOL_CLASSES;
	VERIFY(InitCommonControlsEx(&icex));

	// create the HWND
	CRect rect; rect.SetRectEmpty();
	if (!CWnd::Create(REBARCLASSNAME, NULL, dwStyle, rect, pParentWnd, nID))
		return FALSE;

	// Note: Parent must resize itself for control bar to be resized

	return TRUE;
}

void CXTPReBarBase::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler)
{
	UpdateDialogControls(pTarget, bDisableIfNoHandler);
}

BOOL CXTPReBarBase::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!CControlBar::OnNcCreate(lpCreateStruct))
		return FALSE;

	// if the owner was set before the rebar was created, set it now
	if (m_hWndOwner != NULL)
		DefWindowProc(RB_SETPARENT, (WPARAM)m_hWndOwner, 0);

	return TRUE;
}


void CXTPReBarBase::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS* lpncsp)
{
	// calculate border space (will add to top/bottom, subtract from right/bottom)
	CRect rect; rect.SetRectEmpty();
	BOOL bHorz = (m_dwStyle & CBRS_ORIENT_HORZ) != 0;
	CControlBar::CalcInsideRect(rect, bHorz);

	// adjust non-client area for border space
	lpncsp->rgrc[0].left += rect.left;
	lpncsp->rgrc[0].top += rect.top;
	lpncsp->rgrc[0].right += rect.right;
	lpncsp->rgrc[0].bottom += rect.bottom;
}

void CXTPReBarBase::OnNcPaint()
{
	EraseNonClient();
}

static HWND _xtpAfxChildWindowFromPoint(HWND hWnd, POINT pt)
{
	ASSERT(hWnd != NULL);

	// check child windows
	::ClientToScreen(hWnd, &pt);
	HWND hWndChild = ::GetWindow(hWnd, GW_CHILD);
	for (; hWndChild != NULL; hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT))
	{
		if ((UINT)(WORD)::GetDlgCtrlID(hWndChild) != (WORD)-1 &&
			(::GetWindowLong(hWndChild, GWL_STYLE) & WS_VISIBLE))
		{
			// see if point hits the child window
			CRect rect;
			::GetWindowRect(hWndChild, rect);
			if (rect.PtInRect(pt))
				return hWndChild;
		}
	}

	return NULL;    // not found
}

INT_PTR CXTPReBarBase::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	HWND hWndChild = _xtpAfxChildWindowFromPoint(m_hWnd, point);
	CWnd* pWnd = CWnd::FromHandlePermanent(hWndChild);
	if (pWnd == NULL)
		return CControlBar::OnToolHitTest(point, pTI);

	ASSERT(pWnd->m_hWnd == hWndChild);
	return pWnd->OnToolHitTest(point, pTI);
}

LRESULT CXTPReBarBase::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// special handling for certain messages (forwarding to owner/parent)
	switch (message)
	{
	case WM_POPMESSAGESTRING:
	case WM_SETMESSAGESTRING:
		return GetOwner()->SendMessage(message, wParam, lParam);
	}
	return CControlBar::WindowProc(message, wParam, lParam);
}

#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPReBar

CXTPReBar::CXTPReBar()
{
}

CXTPReBar::~CXTPReBar()
{
}

IMPLEMENT_DYNAMIC(CXTPReBar, CXTPReBarBase)

BOOL CXTPReBar::AddToolBar(CXTPToolBar* pToolBar, DWORD dwStyle)
{
	CSize sz = pToolBar->CalcDockingLayout(32000, LM_HORZDOCK|LM_HORZ | LM_COMMIT);
	pToolBar->MoveWindow(0, 0, sz.cx, sz.cy);

	if (pToolBar->GetDockBar())
	{
		pToolBar->GetDockBar()->RemoveCommandBar(pToolBar, -1);
		pToolBar->EnableDocking(0);
		pToolBar->ModifyBarStyle(CBRS_GRIPPER, 0);
	}


	if (!AddBar(pToolBar, 0, 0, dwStyle))
		return FALSE;

	UINT nID = ( UINT )pToolBar->GetBarID();

	int nCount = (int)DefWindowProc(RB_GETBANDCOUNT, 0, 0);
	REBARBANDINFO rbBand;
	rbBand.cbSize = sizeof(rbBand);

	rbBand.fMask = RBBIM_ID;
	rbBand.wID = nID;

	VERIFY(DefWindowProc(RB_SETBANDINFO, nCount - 1, (LPARAM)&rbBand));


	return TRUE;
}

void CXTPReBar::DeleteToolBar(CXTPToolBar* pToolBar)
{
	int nBand = FindBand(pToolBar);

	if (nBand != -1)
	{
		DefWindowProc(RB_DELETEBAND, nBand, 0);
	}
}

CXTPToolBar* CXTPReBar::GetToolBar(int nBand)
{
	REBARBANDINFO rbBand;
	rbBand.cbSize = sizeof(rbBand);
	rbBand.fMask = RBBIM_CHILD|RBBIM_STYLE|RBBIM_SIZE;

	VERIFY(DefWindowProc(RB_GETBANDINFO, nBand, (LPARAM)&rbBand));

	return DYNAMIC_DOWNCAST(CXTPToolBar, CWnd::FromHandlePermanent(rbBand.hwndChild));
}

int CXTPReBar::FindBand(CXTPToolBar* pToolBar)
{
	int nCount = (int)DefWindowProc(RB_GETBANDCOUNT, 0, 0);

	// sync up hidden state of the bands
	for (int nTemp = nCount; nTemp--; )
	{
		if (GetToolBar(nTemp) == pToolBar)
			return nTemp;
	}
	return -1;


}

BEGIN_MESSAGE_MAP(CXTPReBar, CXTPReBarBase)
	//{{AFX_MSG_MAP(CXTPReBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(RBN_CHILDSIZE, OnChildSize)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CXTPReBar::OnPaint()
{
	// background is already filled in gray
	CPaintDC dc(this);

	// Get the client rect.
	CXTPClientRect rectClient(this);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTPBufferDC memDC(dc, rectClient);
	memDC.FillSolidRect(rectClient, GetSysColor(COLOR_3DFACE));

	// Now let the window do its default painting...
	CWnd::DefWindowProc( WM_ERASEBKGND, (WPARAM)memDC.m_hDC, 0 );
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
}

BOOL CXTPReBar::OnEraseBkgnd(CDC* pDC)
{
	UNUSED_ALWAYS(pDC);
	return TRUE;
}

CSize CXTPReBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CSize sz = CXTPReBarBase::CalcFixedLayout(bStretch, bHorz);
	Invalidate(FALSE);

	int nCount = (int)DefWindowProc(RB_GETBANDCOUNT, 0, 0);

	// sync up hidden state of the bands
	for (int nTemp = nCount; nTemp--; )
	{
		CXTPToolBar* pToolBar = GetToolBar(nTemp);

		if (pToolBar)
		{
			CXTPWindowRect rc(pToolBar);

			CSize szMin = pToolBar->CalcDockingLayout(32000, LM_HORZDOCK|LM_HORZ);
			CSize sz = pToolBar->CalcDockingLayout(rc.Width(), LM_HORZDOCK|LM_HORZ | LM_COMMIT | (pToolBar->GetType() == xtpBarTypeMenuBar? LM_STRETCH: LM_HIDEWRAP));

			REBARBANDINFO rbbi;
			rbbi.cbSize     = sizeof( rbbi );
			rbbi.fMask      = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE;
			rbbi.cxIdeal    = sz.cx;
			rbbi.cxMinChild = szMin.cx;
			rbbi.cyMinChild = sz.cy;
			VERIFY(DefWindowProc(RB_SETBANDINFO, nTemp, (LPARAM)&rbbi ) );

			pToolBar->Invalidate(FALSE);
		}

	}

	return sz;
}


void CXTPReBar::OnChildSize(NMHDR * pNotifyStruct, LRESULT* result )
{
	OnHeightChange(pNotifyStruct, result);
}


void CXTPReBar::LoadState( LPCTSTR lpszProfileName )
{
	// This function restores index, width and style from the registry for
	// each band in the rebar.

	CString strValue = AfxGetApp()->GetProfileString( lpszProfileName,
		m_lpszStateInfoEntry );
	if ( !strValue.IsEmpty() )
	{
		REBARBANDINFO rbbi;
		rbbi.cbSize = sizeof( rbbi );
		rbbi.fMask  = RBBIM_STYLE | RBBIM_SIZE | RBBIM_ID;

		int nCount = (int)DefWindowProc(RB_GETBANDCOUNT, 0, 0);
		for ( int nBand = 0; nBand < nCount; nBand++ )
		{
			CString strBandState;
			VERIFY( AfxExtractSubString( strBandState, strValue, nBand, _T('\n') ) );

			UINT nID, cx, nStyle;
			int nResult = SCANF_S( strBandState, m_lpszStateInfoFormat, &nID, &cx, &nStyle );
			ASSERT( nResult == 3 );

			DefWindowProc(RB_MOVEBAND, DefWindowProc(RB_IDTOINDEX, nID, 0), nBand );
			VERIFY(DefWindowProc(RB_GETBANDINFO, nBand, (LPARAM)&rbbi ) );
			rbbi.cx     = cx;
			rbbi.fStyle = ( rbbi.fStyle & ~( RBBS_HIDDEN | RBBS_BREAK ) ) | nStyle;
			VERIFY(DefWindowProc(RB_SETBANDINFO, nBand, (LPARAM)&rbbi ) );

			CXTPToolBar* pToolBar = GetToolBar(nBand);

			if (pToolBar && (nStyle & RBBS_HIDDEN))
			{
				pToolBar->SetVisible(FALSE);
			}

		}
	}
}

void CXTPReBar::SaveState( LPCTSTR lpszProfileName )
{
	// This function saves index, width and style in the registry for each
	// band in the rebar, so that it could be possible to restore all these
	// settings when the user runs the program next time.

	CString strValue;

	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof( rbbi );
	rbbi.fMask  = RBBIM_STYLE | RBBIM_SIZE | RBBIM_ID;

	int nCount = (int)DefWindowProc(RB_GETBANDCOUNT, 0, 0);
	for ( int nBand = 0; nBand < nCount; nBand++ )
	{
		VERIFY(DefWindowProc(RB_GETBANDINFO,nBand, (LPARAM)&rbbi ) );

		CString strBandState;
		strBandState.Format( m_lpszStateInfoFormat, rbbi.wID, rbbi.cx, rbbi.fStyle );
		strValue += ( strValue.IsEmpty() ? _T("") : _T("\n") ) + strBandState;
	}

	VERIFY( AfxGetApp()->WriteProfileString( lpszProfileName,
		m_lpszStateInfoEntry, strValue ) );
}
