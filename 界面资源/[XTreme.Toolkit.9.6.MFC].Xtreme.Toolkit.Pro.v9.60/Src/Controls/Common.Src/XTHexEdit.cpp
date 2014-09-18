// XTHexEdit.cpp : implementation of the CXTHexEdit class.
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
#include <ctype.h>
#include <afxole.h>
#include <afxdisp.h>
#include "XTPVC80Helpers.h"  // Visual Studio 2005 helper functions
#include "XTPResourceManager.h"
#include "XTPColorManager.h"

#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTHexEdit.h"
#include "XTMemDC.h"
#include "XTResource.h"
#include "XTWndHook.h"
#include "XTFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const CLIPFORMAT CF_BINARY = (CLIPFORMAT)
	::RegisterClipboardFormat( _T("BinaryData") );

static const BYTE chHexTable[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
#define TOHEX(a, b) {*b++ = chHexTable[a >> 4];*b++ = chHexTable[a&0xf];}

/////////////////////////////////////////////////////////////////////////////
// CXTHexEdit
/////////////////////////////////////////////////////////////////////////////
CXTHexEdit::CXTHexEdit()
{
	m_pData             = NULL;
	m_nLength           = 0;
	m_nTopIndex         = 0;
	m_nCurrentAddress   = 0;
	m_nSelStart         = 0xffffffff;
	m_nSelEnd           = 0xffffffff;
	m_nBytePerRow       = 8;
	m_bDynamicBPR       = FALSE;
	m_nLinesPerPage     = 1;
	m_nLineHeight       = 0;
	m_nNullWidth        = 0;
	m_nOffHex           = 0;
	m_nOffAscii         = 0;
	m_nOffAddress       = 0;
	m_bShowHex          = TRUE;
	m_bShowAscii        = TRUE;
	m_bShowAddress      = TRUE;
	m_bAddressIsWide    = TRUE;
	m_bShowCaret        = TRUE;
	m_bUpdate           = TRUE;
	m_bAllowDeletes     = true;
	m_ptEditPos.x       = 0;
	m_ptEditPos.y       = 0;
	m_eEditMode         = editNone;
	m_szCaret           = CSize(0, 0);
	m_crBack            = GetXtremeColor(COLOR_WINDOW);
	m_crText            = GetXtremeColor(COLOR_WINDOWTEXT);
	m_crDisabledBack    = GetXtremeColor(COLOR_3DFACE);
	m_crDisabledText    = GetXtremeColor(COLOR_GRAYTEXT);
	m_crHighlightBack   = GetXtremeColor(COLOR_HIGHLIGHT);
	m_crHighlightText   = GetXtremeColor(COLOR_HIGHLIGHTTEXT);
	m_crDisabledHighlightText = GetXtremeColor(COLOR_HIGHLIGHTTEXT);
	m_crDisabledHighlightBack = GetXtremeColor(COLOR_3DSHADOW);

	_AFX_THREAD_STATE* pState = AfxGetThreadState();
	if (!pState->m_bNeedTerm) {
		AfxOleInit();
	}

	// create the default font used by the hex edit control.
	m_fontHex.CreateFont(-12, 0,0,0,0,0,0,0,0,0,0,0,0, _T("Courier New"));
}

CXTHexEdit::~CXTHexEdit()
{
	if (m_fontHex.GetSafeHandle())
		m_fontHex.DeleteObject();

	if(m_pData)
	{
		free(m_pData);
		m_pData = NULL;
	}
}

IMPLEMENT_DYNAMIC(CXTHexEdit, CEdit)

BEGIN_MESSAGE_MAP(CXTHexEdit, CEdit)
	//{{AFX_MSG_MAP(CXTHexEdit)
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_GETDLGCODE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_WM_NCPAINT()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTHexEdit::RestoreDefaultFont()
{
	if (m_fontHex.GetSafeHandle())
		m_fontHex.DeleteObject();

	// create the default font used by the hex edit control.
	m_fontHex.CreateFont(-12, 0,0,0,0,0,0,0,0,0,0,0,0, _T("Courier New"));
}

void CXTHexEdit::SetHexFont(CFont* pFont)
{
	ASSERT(pFont->GetSafeHandle());

	if (pFont->GetSafeHandle())
	{
		LOGFONT lf;
		pFont->GetLogFont(&lf);
		SetHexFont(&lf);
	}
}

void CXTHexEdit::SetHexFont(LOGFONT* pLogFont)
{
	ASSERT(pLogFont != NULL);

	if (m_fontHex.GetSafeHandle())
		m_fontHex.DeleteObject();

	m_fontHex.CreateFontIndirect(pLogFont);
}

COLORREF CXTHexEdit::GetActualBackColor()
{
	DWORD dwStyle = ::GetWindowLong( m_hWnd, GWL_STYLE );

	if ( !IsWindowEnabled( ) || ( dwStyle & ES_READONLY ) == ES_READONLY)
	{
		return m_crDisabledBack;
	}

	return m_crBack;
}

COLORREF CXTHexEdit::GetActualTextColor()
{
	if ( !IsWindowEnabled( ) )
	{
		return m_crDisabledText;
	}

	return m_crText;
}

COLORREF CXTHexEdit::GetActualHighlightBackColor()
{
	if ( !IsWindowEnabled( ) )
	{
		return m_crDisabledHighlightBack;
	}

	return m_crHighlightBack;
}

COLORREF CXTHexEdit::GetActualHighlightTextColor()
{
	if ( !IsWindowEnabled( ) )
	{
		return m_crDisabledHighlightText;
	}

	return m_crHighlightText;
}

void CXTHexEdit::OnPaint()
{
	// background is already filled in gray
	CPaintDC dc(this);

	// Get the client rect.
	CRect rectClient;
	GetClientRect(&rectClient);


	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTMemDC memDC( &dc, rectClient, GetActualBackColor( ) );

	// Save the current device context, we will restore this just
	// before we loose scope.
	int nSavedDC = memDC.SaveDC();

	ASSERT(m_nCurrentAddress >= 0);
	ASSERT(m_nTopIndex >= 0);

	memDC.SelectObject(&m_fontHex);

	int height = 0;
	int x = rectClient.TopLeft().x;
	int y = rectClient.TopLeft().y;

	TCHAR buf[256];

	memDC.SetBoundsRect(&rectClient, DCB_DISABLE);
	memDC.SetTextColor( GetActualTextColor( ) );

	// Get char dimensions
	if(m_bUpdate)
	{
		const int h_delta = ::GetScrollPos(m_hWnd, SB_HORZ) * m_nNullWidth;

		memDC.GetCharWidth('0', '0', &m_nNullWidth);
		CSize sz = memDC.GetTextExtent(_T("0"), 1);
		m_nLineHeight = sz.cy;

		const int width_address = (m_bAddressIsWide ? 9 : 5) * m_nNullWidth;
		m_nOffHex = m_bShowAddress ? width_address : 0;

		m_nOffAscii = m_nOffHex;
		if (m_bShowHex) m_nOffAscii += m_nBytePerRow * 3 * m_nNullWidth;

		m_nOffAddress = -h_delta;
		m_nOffHex    -= h_delta;
		m_nOffAscii  -= h_delta;

		m_nLinesPerPage = rectClient.Height() / m_nLineHeight;
		m_bUpdate = FALSE;
		UpdateScrollbars();
	}

	if(m_pData)
	{
		height = rectClient.Height() / m_nLineHeight;
		height *= m_nLineHeight;

		if(m_bShowAddress)
		{
			y = 0;
			CRect rcd = rectClient;
			rcd.left = m_nOffAddress;

			// draw address
			int i;
			for (i = m_nTopIndex;
				(i < (m_nLength + 1)) && (rcd.TopLeft().y < height);
				i+= m_nBytePerRow)
			{
				const int width = m_bAddressIsWide ? 8 : 4;
				SPRINTF_S(buf, _countof(buf), _T("%0*lX"), width, m_dwBaseAddress + i);
				memDC.DrawText(buf, width, rcd,
					DT_LEFT|DT_TOP|DT_SINGLELINE|DT_NOPREFIX);
				rcd.TopLeft().y += m_nLineHeight;
			}
		}
		if(m_bShowHex)
		{
			y = 0;
			CRect rcd = rectClient;
			rcd.left = x = m_nOffHex;

			if(m_nSelStart != 0xffffffff && (m_eEditMode == editHigh || m_eEditMode == editLow))
			{
				int  i;
				int  n = 0;
				int  selStart = m_nSelStart, selEnd = m_nSelEnd;
				if(selStart > selEnd)
					selStart ^= selEnd ^= selStart ^= selEnd;

				for (i = m_nTopIndex; (i < selStart) && (y < height); i++)
				{
					LPTSTR p = &buf[0];

					TOHEX(m_pData[i], p);

					*p++ = ' ';
					memDC.TextOut(x, y, buf, 3);
					x += m_nNullWidth * 3;
					n++;
					if(n == m_nBytePerRow)
					{
						n = 0;
						x = m_nOffHex;
						y += m_nLineHeight;
					}
				}

				memDC.SetTextColor(GetActualHighlightTextColor());
				memDC.SetBkColor(GetActualHighlightBackColor());
				for (; (i < selEnd) && (i < m_nLength) && (y < height); i++)
				{
					LPTSTR p = &buf[0];

					TOHEX(m_pData[i], p);

					*p++ = ' ';
					memDC.TextOut(x, y, buf, 3);
					x += m_nNullWidth * 3;
					n++;
					if(n == m_nBytePerRow)
					{
						n = 0;
						x = m_nOffHex;
						y += m_nLineHeight;
					}
				}
				memDC.SetTextColor(GetActualTextColor());
				memDC.SetBkColor(GetActualBackColor());
				for (; (i < m_nLength) && (y < height); i++)
				{
					LPTSTR p = &buf[0];

					TOHEX(m_pData[i], p);

					*p++ = ' ';
					memDC.TextOut(x, y, buf, 3);
					x += m_nNullWidth * 3;
					n++;
					if(n == m_nBytePerRow)
					{
						n = 0;
						x = m_nOffHex;
						y += m_nLineHeight;
					}
				}
			}
			else
			{
				int i;
				for (i = m_nTopIndex; (i < m_nLength) && (rcd.TopLeft().y < height);)
				{
					LPTSTR p = &buf[0];

					int n;
					for (n = 0; (n < m_nBytePerRow) && (i < m_nLength); n++)
					{
						TOHEX(m_pData[i], p);
						*p++ = ' ';
						i++;
					}

					while(n < m_nBytePerRow)
					{
						*p++ = ' '; *p++ = ' '; *p++ = ' ';
						n++;
					}

					memDC.DrawText(buf, m_nBytePerRow*3, rcd, DT_LEFT|DT_TOP|DT_SINGLELINE|DT_NOPREFIX);
					rcd.TopLeft().y += m_nLineHeight;
				}
			}
		}
		if(m_bShowAscii)
		{
			y = 0;
			CRect rcd = rectClient;
			rcd.left = x = m_nOffAscii;
			if(m_nSelStart != 0xffffffff && m_eEditMode == editAscii)
			{
				int  i;
				int  n = 0;
				int  selStart = m_nSelStart, selEnd = m_nSelEnd;
				if(selStart > selEnd)
					selStart ^= selEnd ^= selStart ^= selEnd;

				for (i = m_nTopIndex; (i < selStart) && (y < height); i++)
				{
					buf[0] = isprint(m_pData[i]) ? m_pData[i] : '.';
					memDC.TextOut(x, y, buf, 1);
					x += m_nNullWidth;
					n++;
					if(n == m_nBytePerRow)
					{
						n = 0;
						x = m_nOffAscii;
						y += m_nLineHeight;
					}
				}
				memDC.SetTextColor(GetActualHighlightTextColor());
				memDC.SetBkColor(GetActualHighlightBackColor());
				for (; (i < selEnd) && (y < height); i++)
				{
					buf[0] = isprint(m_pData[i]) ? m_pData[i] : '.';
					memDC.TextOut(x, y, buf, 1);
					x += m_nNullWidth;
					n++;
					if(n == m_nBytePerRow)
					{
						n = 0;
						x = m_nOffAscii;
						y += m_nLineHeight;
					}
				}
				memDC.SetTextColor(GetActualTextColor());
				memDC.SetBkColor(GetActualBackColor());
				for (; (i < m_nLength) && y < height; i++)
				{
					buf[0] = isprint(m_pData[i]) ? m_pData[i] : '.';
					memDC.TextOut(x, y, buf, 1);
					x += m_nNullWidth;
					n++;
					if(n == m_nBytePerRow)
					{
						n = 0;
						x = m_nOffAscii;
						y += m_nLineHeight;
					}
				}
			}
			else
			{
				int i;
				for (i = m_nTopIndex; (i < m_nLength) && (rcd.TopLeft().y < height);)
				{
					LPTSTR p = &buf[0];

					int n;
					for (n = 0; (n < m_nBytePerRow) && (i < m_nLength); n++)
					{
						*p++ = isprint(m_pData[i]) ? m_pData[i] : '.';
						i++;
					}

					memDC.DrawText(buf, n, rcd, DT_LEFT|DT_TOP|DT_SINGLELINE|DT_NOPREFIX);
					rcd.TopLeft().y += m_nLineHeight;
				}
			}
		}
	}

	// Restore the device context.
	memDC.RestoreDC(nSavedDC);
}

void CXTHexEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);
	if(m_pData && !IsSelected())
	{
		if(m_ptEditPos.x == 0 && m_bShowAddress)
		{
			CreateAddressCaret();
		}
		else
		{
			CreateEditCaret();
		}
		RepositionCaret(m_nCurrentAddress);
	}
}

void CXTHexEdit::OnKillFocus(CWnd* pNewWnd)
{
	DestroyCaret();
	CEdit::OnKillFocus(pNewWnd);
}

void CXTHexEdit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UNUSED_ALWAYS(pScrollBar);
	UNUSED_ALWAYS(nPos);

	DoVScroll(nSBCode, false);
}

void CXTHexEdit::DoVScroll(UINT nSBCode, bool bMoveCaret)
{
	if(!m_pData)
	{
		return;
	}

	// Turn off redraw while scrolling, this will cause paint problems
	// with our control because we are painting off screen to reduce
	// flicker.  The default implementation assumes that WM_ERASEBKGND
	// has repainted the background, which in our case does not happen.

	SetRedraw(false);
	const int oa = m_nTopIndex;
	SCROLLINFO  si;
	ZeroMemory(&si, sizeof(SCROLLINFO));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE|SIF_RANGE|SIF_POS;
	if (GetStyle() & WS_VSCROLL)
	{
		VERIFY(::GetScrollInfo(m_hWnd, SB_VERT, &si));
	}
	else
	{
		si.nPage = si.nPos = si.nMin = si.nMax = 0;
	}

	switch(nSBCode)
	{
		case SB_LINEDOWN:
			if (si.nPos < (si.nMax - (int)si.nPage + 1))
			{
				m_nTopIndex += m_nBytePerRow;
			}
			break;

		case SB_LINEUP:
			if(m_nTopIndex > 0)
			{
				m_nTopIndex -= m_nBytePerRow;
				if (m_nTopIndex < 0) m_nTopIndex = 0;
			}
			break;

		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			if (si.nPos >= si.nMax) si.nPos = si.nMax;
			m_nTopIndex = si.nPos * m_nBytePerRow;
			break;

		case SB_PAGEUP:
			if (oa == 0)
			{
				m_nCurrentAddress = 0;
			}
			else
			{
				m_nTopIndex -= m_nBytePerRow * m_nLinesPerPage;
				if (m_nTopIndex < 0)
					m_nTopIndex = 0;
			}
			break;

		case SB_THUMBTRACK:
			si.fMask = SIF_TRACKPOS;

			// Call GetScrollInfo to get current tracking
			// position in si.nTrackPos

			if (!::GetScrollInfo(m_hWnd, SB_VERT, &si) ) {
				return ; // GetScrollInfo failed
			}

			m_nTopIndex = si.nTrackPos * m_nBytePerRow;
			break;
	}

	if(bMoveCaret)
	{
		m_nCurrentAddress += (m_nTopIndex - oa);
	}

	// bug fix by John M. Drescher jdrescher@geocities.com
	if ( m_nCurrentAddress < 0 )
	{
		m_nCurrentAddress = 0;
	}

	UpdateScrollbars();
	RepositionCaret(m_nCurrentAddress);

	SetRedraw(true);

	// repaint the list box
	Invalidate();
	UpdateWindow();

	// repaint the scroll bar.
	::SetWindowPos(m_hWnd, NULL, 0,0,0,0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
}

BOOL CXTHexEdit::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CEdit::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}
BOOL CXTHexEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if (!CEdit::Create(dwStyle, rect, pParentWnd, nID))
	{
		TRACE0( "Failed to create hex edit control.\n" );
		return FALSE;
	}
	SetFont(&m_fontHex);
	return TRUE;
}

BOOL CXTHexEdit::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	if (!CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext))
	{
		TRACE0( "Failed to create hex edit control.\n" );
		return FALSE;
	}

	SetFont(&m_fontHex);

	return TRUE;
}

BOOL CXTHexEdit::OnEraseBkgnd(CDC* pDC)
{
	UNUSED_ALWAYS(pDC);
	return TRUE;
}

void CXTHexEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	if(!m_pData)
	{
		return;
	}

	if(nFlags & MK_SHIFT)
	{
		m_nSelStart = m_nCurrentAddress;
	}
	CPoint pt = CalcPos(point.x, point.y);
	if(pt.x > -1)
	{
		m_ptEditPos = pt;
		pt.x *= m_nNullWidth;
		pt.y *= m_nLineHeight;

		if(pt.x == 0 && m_bShowAddress)
		{
			CreateAddressCaret();
		}
		else
		{
			CreateEditCaret();
		}

		SetCaretPos(pt);
		if(nFlags & MK_SHIFT)
		{
			m_nSelEnd = m_nCurrentAddress;
			if(m_eEditMode == editHigh || m_eEditMode == editLow)
			{
				m_nSelEnd++;
			}
			RedrawWindow();
		}
	}
	if(!(nFlags & MK_SHIFT))
	{
		if(::DragDetect(m_hWnd, point))
		{
			m_nSelStart = m_nCurrentAddress;
			m_nSelEnd   = m_nSelStart;
			SetCapture();
		}
		else
		{
			BOOL bsel = m_nSelStart != 0xffffffff;

			m_nSelStart = 0xffffffff;
			m_nSelEnd   = 0xffffffff;
			if(bsel)
			{
				RedrawWindow();
			}
		}
	}
	if(!IsSelected())
	{
		if (m_bShowCaret)
			ShowCaret();
	}
}

// in: pixels
// out: character coordinates + change of m_nCurrrentAddress
CPoint CXTHexEdit::CalcPos(int x, int y)
{
	if (y < 0  ||  x < 0)
		return CPoint(-1, -1);

	int lastVisRow = m_nLength / m_nBytePerRow - (m_nTopIndex / m_nBytePerRow);
	int row = y / m_nLineHeight;
	y = row + (m_nTopIndex / m_nBytePerRow);


//  x += (m_nNullWidth - 1);
	x /= m_nNullWidth;

	//In address area
	const int right_of_address = (m_nOffAddress / m_nNullWidth) + // one past last
		(!m_bShowAddress? -1 : (m_bAddressIsWide ? 8 : 4));
	if(x < right_of_address)
	{
		//Set first byte in line as current.
		m_nCurrentAddress = m_nTopIndex + (m_nBytePerRow * row);
		m_eEditMode = editNone;
		return CPoint(0, row);
	}

	//In hex area
	const int right_of_hex_area = right_of_address +
		(!m_bShowHex? 0 : (m_nBytePerRow * 3)); //3 = 3 chars per byte (e.g. cc<space>)
	if(m_bShowHex && x < right_of_hex_area)
	{
		if ((x % 3) == 2)
		{
			++x;
		}
		const int col = (x - right_of_address) / 3;
		m_nCurrentAddress = m_nTopIndex +
			(m_nBytePerRow * row) + col;

		if (m_nCurrentAddress > m_nLength)  // past end?
		{
			x = m_nLength % m_nBytePerRow;
			row = lastVisRow;

			x *= 3;
			x += (right_of_address + 1);

			m_nCurrentAddress = m_nLength;
		}

		m_eEditMode = ((x % 3) == 0)? editHigh : editLow;

		return CPoint(x, row);
	}

	const int right_of_ascii_area = right_of_hex_area + m_nBytePerRow + 1;
	if(m_bShowAscii && x < right_of_ascii_area)
	{
		if (x == right_of_hex_area) ++x; // advance from margin
		const int col = x - right_of_hex_area - 1;
		m_nCurrentAddress = m_nTopIndex + (m_nBytePerRow * row) + col;

		if (m_nCurrentAddress > m_nLength)  // past end?
		{
			x = m_nLength % m_nBytePerRow;
			row = lastVisRow;
			x += (right_of_hex_area + 1);

			m_nCurrentAddress = m_nLength;
		}

		m_eEditMode = editAscii;
		return CPoint(x, row);
	}

	return CPoint(-1,-1);
}

void CXTHexEdit::CreateAddressCaret()
{
	DestroyCaret();
	m_szCaret = CSize(m_nNullWidth * (m_bAddressIsWide ? 8 : 4),
		m_nLineHeight);
	CreateSolidCaret(m_szCaret.cx, m_szCaret.cy);
}

void CXTHexEdit::CreateEditCaret()
{
	DestroyCaret();
	m_szCaret = CSize(m_nNullWidth, m_nLineHeight);
	CreateSolidCaret(m_szCaret.cx, m_szCaret.cy);
}

void CXTHexEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_pData)
	{
		return;
	}

	if(nFlags & MK_LBUTTON && m_nSelStart != 0xffffffff)
	{
		CRect rc;
		GetClientRect(&rc);
		if(!rc.PtInRect(point))
		{
			if(point.y < 0)
			{
				DoVScroll(SB_LINEUP, true);
				point.y = 0;
			}
			else if(point.y > rc.Height())
			{
				DoVScroll(SB_LINEDOWN, true);
				point.y = rc.Height() -1;
			}
		}

		//
		// we are selecting
		//
		int  seo = m_nSelEnd;
		CPoint pt = CalcPos(point.x, point.y);
		if(pt.x > -1)
		{
			m_nSelEnd = m_nCurrentAddress;
			if(m_eEditMode == editHigh || m_eEditMode == editLow)
			{
				m_nSelEnd++;
			}
		}
		if(IsSelected())
		{
			DestroyCaret();
		}

		if(seo != m_nSelEnd)
		{
			RedrawWindow();
		}
	}
}

void CXTHexEdit::UpdateScrollbars()
{
	DWORD dwStyle = GetStyle();
	SCROLLINFO si;

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE|SIF_RANGE|SIF_POS;

	if (dwStyle & WS_VSCROLL)
	{
		bool bDisable = false;
		si.nMin = 0;
		si.nMax = m_nLength / m_nBytePerRow - 1; // - m_nLinesPerPage + 1;
		if (si.nMax < m_nLinesPerPage - 1)
		{
			si.nMax = 1;
			si.nPage = 1;
			bDisable = true;
			if (m_nTopIndex)
			{
				m_nTopIndex = 0;
				RepositionCaret(m_nCurrentAddress);
			}
		}
		else
		{
			si.nPage = m_nLinesPerPage - 1;

			while (m_nTopIndex / m_nBytePerRow > (si.nMax - m_nLinesPerPage + 2)  &&
				m_nTopIndex > 0)
			{
				m_nTopIndex -= m_nBytePerRow;
				RepositionCaret(m_nCurrentAddress);
			}
		}
		if (m_nTopIndex < 0) m_nTopIndex = 0;
		si.nPos = m_nTopIndex / m_nBytePerRow;

		ModifyStyle(0, WS_VSCROLL);
		if (!bDisable)
		{
			::SetScrollInfo(m_hWnd, SB_VERT, &si, TRUE);
			::EnableScrollBar(m_hWnd, SB_VERT, ESB_ENABLE_BOTH);
		}
		else
		{
			si.nMax = si.nMin + 1;
			si.nPage = si.nPos = 0;
			::SetScrollInfo(m_hWnd, SB_VERT, &si, TRUE);
			::EnableScrollBar(m_hWnd, SB_VERT, ESB_DISABLE_BOTH);
		}
	}

	if (dwStyle & WS_HSCROLL)
	{
		CRect rc;
		GetClientRect(&rc);

		si.nMin = 0;
		si.nMax =((m_bShowAddress ? (m_bAddressIsWide ? 9 : 5) : 0) +
				(m_bShowHex ? m_nBytePerRow * 3 : 0) +
				(m_bShowAscii ? m_nBytePerRow : 0) );
		si.nPage = __min(rc.Width() / m_nNullWidth, si.nMax);
		si.nPos = 0;

		si.fMask &= ~SIF_POS;
		::SetScrollInfo(m_hWnd, SB_HORZ, &si, TRUE);
		if(si.nMax * m_nNullWidth >= (int)rc.Width())
		{
			::EnableScrollBar(m_hWnd, SB_HORZ, ESB_ENABLE_BOTH);
		}
	}
}

void CXTHexEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(IsSelected())
	{
		ReleaseCapture();
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CXTHexEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	DWORD dwStyle = ::GetWindowLong( m_hWnd, GWL_STYLE );

	if ( !IsWindowEnabled( ) || ( dwStyle & ES_READONLY ) == ES_READONLY)
	{
		return;
	}

	nFlags;nRepCnt;
	if(!m_pData)
	{
		return;
	}

	if(m_nCurrentAddress < 0)   //Shouldn't happen
	{
		return;
	}

	if(nChar == '\t')
	{
		return;
	}

	if(::GetKeyState(VK_CONTROL) < 0)
	{
		switch(nChar)
		{
			case 0x03:
				if(IsSelected())
				{
					OnEditCopy();
				}
				return;
			case 0x16:
				OnEditPaste();
				return;
			case 0x18:
				if(IsSelected())
				{
					OnEditCut();
				}
				return;
			case 0x1a:
				OnEditUndo();
				return;
		}
	}

	if(nChar == 0x08) // backspace
	{
		if (GetAllowDeletes())
		{
			if(IsSelected())
			{
				OnEditClear();
			}
			else if(m_nCurrentAddress > 0)
			{
				m_nCurrentAddress--;
				SelDelete(m_nCurrentAddress, m_nCurrentAddress+1);
				RepositionCaret(m_nCurrentAddress);
				RedrawWindow();
			}
			else
			{
				MessageBeep(0);
			}
		}
		return;
	}

	// can't go past end of buffer
	if ( (UINT)m_nCurrentAddress >= m_nMaxLength )
	{
		MessageBeep(0);
		return;
	}

	if (m_eEditMode != editNone)
	{
		// see if need to grow the buffer
		if (m_nCurrentAddress >= m_nLength)
		{
			int iNewLength = (m_nLength+1);
			LPBYTE p = (LPBYTE) calloc(iNewLength, 1);

			MEMCPY_S(p, m_pData, m_nLength);

			free(m_pData);
			m_pData = p;
			SetSel(-1, -1);
			m_nLength = iNewLength;
			m_bUpdate = TRUE;
		}
	}

	SetSel(-1, -1);
	switch(m_eEditMode)
	{
		case editNone:
			return;

		case editHigh:
		case editLow:
			if ( isxdigit(nChar) )
			{
				UINT b = nChar - '0';
				if (b > 9)
				{
					b = 10 + nChar - 'a';
				}

				if(m_eEditMode == editHigh)
				{
					m_pData[m_nCurrentAddress] = (unsigned char)((m_pData[m_nCurrentAddress] & 0x0f) | (b << 4));
				}
				else
				{
					m_pData[m_nCurrentAddress] = (unsigned char)((m_pData[m_nCurrentAddress] & 0xf0) | b);
				}
				Move(1,0);
			}
			break;

		case editAscii:
			m_pData[m_nCurrentAddress] = (unsigned char)nChar;
			Move(1,0);
			break;
	}
	RedrawWindow();
}

void CXTHexEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	nFlags; nRepCnt;

	BOOL bShift = (::GetKeyState(VK_SHIFT) < 0);
	switch(nChar)
	{
		case VK_DOWN:
			if(bShift)
			{
				if(!IsSelected())
				{
					m_nSelStart = m_nCurrentAddress;
				}
				Move(0,1);
				m_nSelEnd = m_nCurrentAddress;
				if(m_eEditMode == editHigh || m_eEditMode == editLow)
				{
					m_nSelEnd++;
				}
				RedrawWindow();
				break;
			}
			else
			{
				SetSel(-1, -1);
			}
			Move(0,1);
			break;
		case VK_UP:
			if(bShift)
			{
				if(!IsSelected())
				{
					m_nSelStart = m_nCurrentAddress;
				}
				Move(0,-1);
				m_nSelEnd   = m_nCurrentAddress;
				RedrawWindow();
				break;
			}
			else
			{
				SetSel(-1, -1);
			}
			Move(0,-1);
			break;
		case VK_LEFT:
			if(bShift)
			{
				if(!IsSelected())
				{
					m_nSelStart = m_nCurrentAddress;
				}
				Move(-1,0);
				m_nSelEnd   = m_nCurrentAddress;
				RedrawWindow();
				break;
			}
			else
			{
				SetSel(-1, -1);
			}
			Move(-1,0);
			break;
		case VK_RIGHT:
			if(bShift)
			{
				if(!IsSelected())
				{
					m_nSelStart = m_nCurrentAddress;
				}
				Move(1,0);
				m_nSelEnd   = m_nCurrentAddress;
				if(m_eEditMode == editHigh || m_eEditMode == editLow)
				{
					m_nSelEnd++;
				}
				RedrawWindow();
				break;
			}
			else
			{
				SetSel(-1, -1);
			}
			Move(1,0);
			break;
		case VK_PRIOR:
			if(bShift)
			{
				if(!IsSelected())
				{
					m_nSelStart = m_nCurrentAddress;
				}
				DoVScroll(SB_PAGEUP, true);
				Move(0,0);
				m_nSelEnd   = m_nCurrentAddress;
				RedrawWindow();
				break;
			}
			else
			{
				SetSel(-1, -1);
			}
			DoVScroll(SB_PAGEUP, true);
			Move(0,0);
			break;
		case VK_NEXT:
			if(bShift)
			{
				if(!IsSelected())
				{
					m_nSelStart = m_nCurrentAddress;
				}
				DoVScroll(SB_PAGEDOWN, true);
				Move(0,0);
				m_nSelEnd   = m_nCurrentAddress;
				RedrawWindow();
				break;
			}
			else
			{
				SetSel(-1, -1);
			}
			DoVScroll(SB_PAGEDOWN, true);
			Move(0,0);
			break;
		case VK_HOME:
			if (m_eEditMode == editLow)
			{
				m_eEditMode = editHigh;
			}
			if(bShift)
			{
				if(!IsSelected())
				{
					m_nSelStart = m_nCurrentAddress;
				}
				if(::GetKeyState(VK_CONTROL) < 0)
				{
					DoVScroll(SB_THUMBTRACK, true);
					Move(0,0);
				}
				else
				{
					m_nCurrentAddress /= m_nBytePerRow;
					m_nCurrentAddress *= m_nBytePerRow;
					Move(0,0);
				}
				m_nSelEnd = m_nCurrentAddress;
				RedrawWindow();
				break;
			}
			else
			{
				SetSel(-1, -1);
			}
			if(::GetKeyState(VK_CONTROL) < 0)
			{
				m_nTopIndex = m_nCurrentAddress = 0;
				UpdateScrollbars();
				RepositionCaret(m_nCurrentAddress);
				RedrawWindow();
			}
			else
			{
				m_nCurrentAddress /= m_nBytePerRow;
				m_nCurrentAddress *= m_nBytePerRow;
				Move(0,0);
			}
			break;
		case VK_END:
			if (m_eEditMode == editLow)
			{
				m_eEditMode = editHigh;
			}
			if(bShift)
			{
				if(!IsSelected())
				{
					m_nSelStart = m_nCurrentAddress;
				}
				if(::GetKeyState(VK_CONTROL) < 0)
				{
					m_nCurrentAddress = m_nLength-1;
					DoVScroll(SB_PAGEDOWN, false);
					Move(0,0);
				}
				else
				{
					m_nCurrentAddress /= m_nBytePerRow;
					m_nCurrentAddress *= m_nBytePerRow;
					m_nCurrentAddress += m_nBytePerRow - 1;
					if(m_nCurrentAddress > m_nLength)
					{
						m_nCurrentAddress = m_nLength;
					}
					Move(0,0);
				}
				m_nSelEnd   = m_nCurrentAddress;
				RedrawWindow();
				break;
			}
			else
			{
				SetSel(-1, -1);
			}
			if(::GetKeyState(VK_CONTROL) < 0)
			{
				m_nCurrentAddress = m_nLength;
				DoVScroll(SB_PAGEDOWN, false);
				Move(0,0);
			}
			else
			{
				m_nCurrentAddress /= m_nBytePerRow;
				m_nCurrentAddress *= m_nBytePerRow;
				m_nCurrentAddress += m_nBytePerRow - 1;
				if(m_nCurrentAddress > m_nLength)
					m_nCurrentAddress = m_nLength;
				Move(0,0);
			}
			break;
		case VK_INSERT:
			SelInsert(m_nCurrentAddress, __max(1, m_nSelEnd-m_nSelStart));
			RepositionCaret(m_nCurrentAddress);
			RedrawWindow();
			break;
		case VK_DELETE:
			if (GetAllowDeletes())
			{
				if(IsSelected())
				{
					OnEditClear();
				}
				else
				{
					if (m_nCurrentAddress < m_nLength)
					{
						SelDelete(m_nCurrentAddress, m_nCurrentAddress+1);
						RedrawWindow();
					}
					else
					{
						MessageBeep(0);
					}
				}
			}
			break;
		case '\t':
		{
			int next_win_sel = 0;
			switch(m_eEditMode)
			{
				case editNone:
					m_eEditMode = editHigh;
					break;
				case editHigh:
				case editLow:
					if (bShift)
					{
						next_win_sel = GW_HWNDPREV;
					}
					else
					{
						m_eEditMode = editAscii;
					}
					break;
				case editAscii:
					if (bShift)
					{
						m_eEditMode = editHigh;
					}
					else
					{
						next_win_sel = GW_HWNDNEXT;
					}
					break;
			}
			if (next_win_sel)
			{
				CWnd *pWnd = this;
				for (;;)
				{
					pWnd = pWnd->GetNextWindow(next_win_sel);
					if (pWnd == this)
					{
						break;
					}
					if (pWnd == NULL)
					{
						if (next_win_sel == GW_HWNDNEXT)
						{
							pWnd = GetNextWindow(GW_HWNDFIRST);
						}
						else
						{
							pWnd = GetNextWindow(GW_HWNDLAST);
						}
					}
					if (pWnd->GetStyle() & WS_TABSTOP)
					{
						pWnd->SetFocus();
						break;
					}
				}
			}
			else
			{
				Move(0,0);
			}
			break;
		}
	}
}

void CXTHexEdit::Move(int x, int y)
{
	switch(m_eEditMode)
	{
		case editNone:
			return;

		case editHigh:
			if(x != 0)
				m_eEditMode = editLow;
			if(x == -1)
				m_nCurrentAddress --;
			m_nCurrentAddress += y* m_nBytePerRow;
			break;

		case editLow:
			if(x != 0)
				m_eEditMode = editHigh;
			if(x == 1)
				m_nCurrentAddress++;
			m_nCurrentAddress += y* m_nBytePerRow;
			break;

		case editAscii:
			{
				m_nCurrentAddress += x;
				m_nCurrentAddress += y*m_nBytePerRow;
			}
			break;
	}

	if(m_nCurrentAddress < 0)
	{
		m_nCurrentAddress = 0;
	}

	if (m_nCurrentAddress >= m_nLength)
	{
		m_nCurrentAddress = m_nLength;
	}

	if ((x | y) != 0  &&  m_nCurrentAddress < m_nTopIndex)
	{
		DoVScroll(SB_LINEUP, false);
	}
	if ((x | y) != 0  &&
		m_nCurrentAddress >= m_nTopIndex + m_nLinesPerPage*m_nBytePerRow)
	{
		DoVScroll(SB_LINEDOWN, false);
	}
	//ScrollIntoView(m_nCurrentAddress);
	RepositionCaret(m_nCurrentAddress);
}

void CXTHexEdit::SetSel(int nSelStart, int nSelEnd)
{
#ifdef _DEBUG
	_CrtCheckMemory();
#endif

	if(GetFocus() == this)
		DestroyCaret();
	m_nSelStart = nSelStart;
	m_nSelEnd = nSelEnd;
	if (! IsWindow(m_hWnd) )
		return;
	RedrawWindow();
	if(GetFocus() == this)
	{
		if(m_ptEditPos.x == 0 && m_bShowAddress)
		{
			CreateAddressCaret();
		}
		else
		{
			CreateEditCaret();
		}
		RepositionCaret(m_nCurrentAddress);

		if (m_bShowCaret)
			ShowCaret();
	}
}

void CXTHexEdit::RepositionCaret(int p)
{
	if(GetFocus() != this)
		return;

	int x, y;

	y = (p - m_nTopIndex) / m_nBytePerRow;
	x = (p - m_nTopIndex) % m_nBytePerRow;

	switch(m_eEditMode)
	{
		case editNone:
			CreateAddressCaret();
			x = m_nOffAddress;
			break;
		case editHigh:
			CreateEditCaret();
			x *= m_nNullWidth*3;
			x += m_nOffHex;
			break;
		case editLow:
			CreateEditCaret();
			x *= m_nNullWidth*3;
			x += m_nNullWidth;
			x += m_nOffHex;
			break;
		case editAscii:
			CreateEditCaret();
			x *= m_nNullWidth;
			x += m_nOffAscii;
			break;
	}
	m_ptEditPos.x = x;
	m_ptEditPos.y = y*m_nLineHeight;
	CRect rc;
	GetClientRect(&rc);
	CRect rcCarret(m_ptEditPos, m_szCaret);
	CRect rcInt;
	rcInt.IntersectRect(rcCarret, rc);
	if(rcInt == rcCarret)
	{
		SetCaretPos(m_ptEditPos);

		if (m_bShowCaret)
			ShowCaret();
	}
}

void CXTHexEdit::ScrollIntoView(int p)
{
	if(p < m_nTopIndex || p > m_nTopIndex + m_nLinesPerPage*m_nBytePerRow)
	{
		m_nTopIndex = (p/m_nBytePerRow) * m_nBytePerRow;
		m_nTopIndex -= (m_nLinesPerPage / 3) * m_nBytePerRow;
		if(m_nTopIndex < 0)
		{
			m_nTopIndex = 0;
		}

		UpdateScrollbars();
		RedrawWindow();
	}
}

void CXTHexEdit::OnContextMenu(CWnd*, CPoint point)
{
	if (point.x == -1 && point.y == -1)
	{
		//keystroke invocation
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CMenu menu;
	XTPResourceManager()->LoadMenu(&menu, XT_IDM_POPUP);

	CMenu* pPopup = menu.GetSubMenu(1);
	ASSERT(pPopup != NULL);

	const int DISABLE_FLAGS = MF_GRAYED|MF_DISABLED|MF_BYCOMMAND;

	pPopup->EnableMenuItem(ID_EDIT_UNDO, DISABLE_FLAGS);

	if (!GetAllowDeletes())
	{
		pPopup->EnableMenuItem(ID_EDIT_CLEAR, DISABLE_FLAGS);
		pPopup->EnableMenuItem(ID_EDIT_CUT,   DISABLE_FLAGS);
	}

	if(!IsSelected())
	{
		pPopup->EnableMenuItem(ID_EDIT_CLEAR, DISABLE_FLAGS);
		pPopup->EnableMenuItem(ID_EDIT_CUT,   DISABLE_FLAGS);
		pPopup->EnableMenuItem(ID_EDIT_COPY,  DISABLE_FLAGS);
	}

	if (OpenClipboard())
	{
		if (!IsClipboardFormatAvailable(CF_TEXT)  &&
			!IsClipboardFormatAvailable(CF_BINARY))
		{
			pPopup->EnableMenuItem(ID_EDIT_PASTE, DISABLE_FLAGS);
		}
		::CloseClipboard();
	}

	XTFuncContextMenu(pPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this, XT_IDR_TBAR_HEXEDIT);
}

void CXTHexEdit::OnEditClear()
{
	DWORD dwStyle = ::GetWindowLong( m_hWnd, GWL_STYLE );

	if ( !IsWindowEnabled( ) || ( dwStyle & ES_READONLY ) == ES_READONLY)
	{
		return;
	}

	if (!GetAllowDeletes())
		return;

	NormalizeSel();
	m_nCurrentAddress = m_nSelStart;
	SelDelete(m_nSelStart, m_nSelEnd);
	RepositionCaret(m_nCurrentAddress);
	RedrawWindow();
}

void CXTHexEdit::OnEditCopy()
{
	if (!OpenClipboard()) return;

	NormalizeSel();

	::EmptyClipboard();

	const int dwLen = m_nSelEnd - m_nSelStart + 1;
	HGLOBAL hMemb = ::GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT, dwLen);

	// copy binary
	ASSERT(hMemb != NULL);
	LPBYTE  p = (BYTE*)::GlobalLock(hMemb);
	MEMCPY_S(p, m_pData+m_nSelStart, dwLen);
	::GlobalUnlock(hMemb);

	HGLOBAL hMema = NULL;
	if(m_eEditMode != editAscii)
	{
		hMema = ::GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT,
			dwLen * 3);

		// copy ascii
		p = (BYTE*)::GlobalLock(hMema);
		int i;
		for (i = 0; i < dwLen;)
		{
			TOHEX(m_pData[m_nSelStart+i], p);
			*p++ = ' ';
			i++;
		}

		::GlobalUnlock(hMema);
	}
	else
	{
		hMema = ::GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT, dwLen);

		// copy ascii
		p = (BYTE*)::GlobalLock(hMema);
		MEMCPY_S(p, m_pData+m_nSelStart, dwLen);

		for (int i = 0; i < dwLen; p++, i++)
		{
			if(!isprint(*p))
			{
				*p = '.';
			}
		}
		::GlobalUnlock(hMema);
	}

	::SetClipboardData(CF_TEXT, hMema);
	::SetClipboardData(CF_BINARY, hMemb);

	VERIFY(CloseClipboard());
}

void CXTHexEdit::OnEditCut()
{
	DWORD dwStyle = ::GetWindowLong( m_hWnd, GWL_STYLE );

	if ( !IsWindowEnabled( ) || ( dwStyle & ES_READONLY ) == ES_READONLY)
	{
		return;
	}

	OnEditCopy();
	SelDelete(m_nSelStart, m_nSelEnd);
	RedrawWindow();
}

void CXTHexEdit::OnEditPaste()
{
	DWORD dwStyle = ::GetWindowLong( m_hWnd, GWL_STYLE );

	if ( !IsWindowEnabled( ) || ( dwStyle & ES_READONLY ) == ES_READONLY)
	{
		return;
	}

	if (!OpenClipboard()) return;

	HGLOBAL hmem = NULL;
	if (::IsClipboardFormatAvailable(CF_BINARY))
	{
		hmem = ::GetClipboardData(CF_BINARY);
	}
	else if (::IsClipboardFormatAvailable(CF_TEXT))
	{
		hmem = ::GetClipboardData(CF_TEXT);
	}

	if(hmem)
	{
		LPBYTE  p = (BYTE*)::GlobalLock(hmem);
		DWORD   dwLen = (DWORD)::GlobalSize(hmem);
		int     insert;
		int     oa = m_nCurrentAddress;

		NormalizeSel();
		if(m_nSelStart == 0xffffffff)
		{
			if(m_eEditMode == editLow)
			{
				m_nCurrentAddress++;
			}
			insert = m_nCurrentAddress;
			SelInsert(m_nCurrentAddress, dwLen);
		}
		else
		{
			insert = m_nSelStart;
			SelDelete(m_nSelStart, m_nSelEnd);
			SelInsert(insert, dwLen);
			SetSel(-1, -1);
		}

		if (insert + dwLen > m_nMaxLength)
		{
			dwLen = m_nMaxLength - insert;
		}

		MEMCPY_S(m_pData + insert, p, dwLen);

#ifdef _DEBUG
		_CrtCheckMemory();
#endif

		m_nCurrentAddress = oa;
		RedrawWindow();
		::GlobalUnlock(hmem);
	}

	::CloseClipboard();
}

void CXTHexEdit::OnEditSelectAll()
{
	m_nSelStart = 0;
	m_nSelEnd = m_nLength;
	DestroyCaret();
	RedrawWindow();
}

void CXTHexEdit::OnEditUndo()
{
	// TODO: Add your command handler code here

}

void CXTHexEdit::NormalizeSel()
{
	if(m_nSelStart > m_nSelEnd)
	{
		m_nSelStart ^= m_nSelEnd ^= m_nSelStart ^= m_nSelEnd;
	}
	if (m_nSelEnd > m_nLength)
	{
		m_nSelEnd = m_nLength;
	}
}

void CXTHexEdit::SelDelete(int nSelStart, int nSelEnd)
{
	if (!GetAllowDeletes())
		return;

#ifdef _DEBUG
	_CrtCheckMemory();
#endif

	NormalizeSel();

	const int nSelSize  = (nSelEnd-nSelStart);
	const int newLength = m_nLength - nSelSize;
	if (newLength < 0)
		return;

	MEMMOVE_S(m_pData + nSelStart, m_pData + nSelEnd,
		m_nLength - nSelEnd);

	SetSel(-1, -1);

	m_nLength = newLength;
	if(m_nCurrentAddress > m_nLength)
	{
		m_nCurrentAddress = m_nLength;
		RepositionCaret(m_nCurrentAddress);
	}
	m_bUpdate = TRUE;
}

void CXTHexEdit::SelInsert(int nSelStart, int nLength)
{
	if ((UINT)(m_nLength + nLength) > m_nMaxLength)
	{
		nLength = m_nMaxLength - m_nLength;
	}
	if (nLength <= 0)
	{
		MessageBeep(0);
		return;
	}
	LPBYTE p = (LPBYTE) calloc(m_nLength + nLength, 1);
	MEMCPY_S(p, m_pData, nSelStart);
	MEMCPY_S(p+nSelStart+nLength, m_pData+nSelStart, (m_nLength-nSelStart));
	free(m_pData);
	m_pData = p;
	SetSel(-1, -1);
	m_nLength = m_nLength+nLength;
	m_bUpdate = TRUE;
}

void CXTHexEdit::SetData(LPBYTE p, int nLength, int nMaxLength)
{
	if(m_pData)
		free(m_pData);

	m_pData = (LPBYTE) malloc(nLength);
	MEMCPY_S(m_pData, p, nLength);

	SetSel(-1, -1);
	m_nMaxLength = nMaxLength < 0? -1 : __max(nMaxLength, nLength);
	m_nLength = nLength;
	m_nCurrentAddress = 0;
	m_ptEditPos.x = m_ptEditPos.y = 0;
	m_eEditMode = editHigh;
	m_nTopIndex = 0;
	m_bUpdate = TRUE;

	if (m_hWnd)
	{
		UpdateScrollbars();
		RepositionCaret(m_nCurrentAddress);
		RedrawWindow();

		// this will force the scroll bars to update if this is before the
		// initial update
		if (GetStyle() & WS_VSCROLL)
		{
			PostMessage(WM_VSCROLL, (WPARAM)-1, 0);
		}
	}
}

void CXTHexEdit::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UNUSED_ALWAYS(nPos);
	UNUSED_ALWAYS(pScrollBar);

	SCROLLINFO si;
	ZeroMemory(&si, sizeof(SCROLLINFO));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE|SIF_RANGE|SIF_POS|SIF_TRACKPOS;
	VERIFY(::GetScrollInfo(m_hWnd, SB_HORZ, &si));

	int newpos = si.nPos;

	switch(nSBCode)
	{
		case SB_LINELEFT:
			--newpos;
			break;

		case SB_LINERIGHT:
			++newpos;
			break;

		case SB_PAGELEFT:
			newpos -= si.nPage;
			break;

		case SB_PAGERIGHT:
			newpos += si.nPage;
			break;

		case SB_THUMBTRACK:
			newpos = si.nTrackPos;
			break;
	}

	if (newpos < 0)
	{
		newpos = 0;
	}
	else if (newpos > si.nMax)
	{
		newpos = si.nMax;
	}

	if (newpos != si.nPos)
	{
		m_bUpdate = true;
		::SetScrollPos(m_hWnd, SB_HORZ, newpos, TRUE);
		RedrawWindow();
		RepositionCaret(m_nCurrentAddress);
	}
}

void CXTHexEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	UNUSED_ALWAYS(nFlags);
	UNUSED_ALWAYS(point);
}

DWORD CXTHexEdit::SetAddressBase(DWORD dwBase)
{
	DWORD dwOldAddress = m_dwBaseAddress;
	m_dwBaseAddress = dwBase;
	m_bUpdate = TRUE;

	return dwOldAddress;
}

void CXTHexEdit::OnSize(UINT nType, int cx, int cy)
{
	SetRedraw(false);
	CEdit::OnSize(nType, cx, cy);

	if(m_bDynamicBPR)
	{
		RecalcBPR();
	}

	m_bUpdate = TRUE;
	UpdateScrollbars();
	SetRedraw(true);
	Invalidate();
	UpdateWindow();
}

void CXTHexEdit::RecalcBPR()
{
	CDC* dc = GetDC();
	HGDIOBJ hOldFont = dc->SelectObject(&m_fontHex);

	dc->GetCharWidth('0', '0', &m_nNullWidth);
	CRect rc;

	GetClientRect(rc);
	int cx = rc.Width();

	if(m_bShowAddress)
		cx -= m_nOffHex;

	if(m_bShowAscii && m_bShowHex)
	{
		m_nBytePerRow = ((cx * 2) / 3) / (3 * m_nNullWidth);    // 2/3rd is hex, 1/3rd is ascii
	}
	else if(m_bShowHex)
	{
		m_nBytePerRow = cx / (3 * m_nNullWidth);
	}
	else if(m_bShowAscii)
	{
		m_nBytePerRow = cx / m_nNullWidth;
	}
	m_bUpdate = TRUE;

	if(m_nBytePerRow <= 0)
		m_nBytePerRow = 1;

	dc->SelectObject(hOldFont);
	ReleaseDC(dc);
}

void CXTHexEdit::OnNcPaint()
{
	// Use the OnNcPaint() handler to prevent the caret
	// from being shown when the control is first displayed

	CEdit::OnNcPaint(); // Be sure and call the parent class
	//HideCaret();  // Hide the caret
}

BOOL CXTHexEdit::OnMouseWheel(UINT /*nFlags*/, short zDelta, CPoint /*pt*/)
{
	OnVScroll(zDelta < 0? SB_LINEDOWN : SB_LINEUP, 0, NULL);
	return TRUE;
}

void CXTHexEdit::EnableScrollBars(bool /*bVertical*/, bool /*bHorizontal*/)
{
	XT_ASSERT_MSG(FALSE,
		"CXTHexEdit::EnableScrollBars has been deprecated.\n"
		"\n"
		"You should now set scroll bars for the control through WS_HSCROLL/WS_VSCROLL and/or the resource editor."
		);
}

int CXTHexEdit::GetData(LPBYTE pData, int nLength)
{
	MEMCPY_S(pData, m_pData, __min(nLength, m_nLength));
	return m_nLength;
}
