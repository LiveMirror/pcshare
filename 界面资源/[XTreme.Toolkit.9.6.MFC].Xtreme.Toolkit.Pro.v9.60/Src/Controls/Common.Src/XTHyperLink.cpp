// XTHyperLink.cpp : implementation of the CXTHyperLink class.
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
#include "XTPResourceManager.h"

#include "XTResource.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTMemDC.h"
#include "XTPColorManager.h"
#include "XTHyperLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTHyperLink
/////////////////////////////////////////////////////////////////////////////
CXTHyperLink::CXTHyperLink()
: m_clrBack( ( COLORREF )-1 )
, m_bTipEnabled( true )
{
	m_hcurHand     = XTAuxData().hcurHand;
	m_clrLink      = RGB(0x00, 0x00, 0x80); // dark blue
	m_clrHover     = RGB(0x00, 0x00, 0xFF); // blue
	m_clrVisited   = RGB(0x80, 0x00, 0x80); // purple
	m_bUnderline   = true;
	m_bMouseOver   = false;
	m_bVisited     = false;
	m_bInitControl = true;
	m_bShellExec   = true;

	m_strLink.Empty();
}

CXTHyperLink::~CXTHyperLink()
{
}

IMPLEMENT_DYNAMIC(CXTHyperLink, CStatic)

BEGIN_MESSAGE_MAP(CXTHyperLink, CStatic)
	//{{AFX_MSG_MAP(CXTHyperLink)
	ON_WM_SETCURSOR()
	ON_WM_NCHITTEST()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_WINDOWPOSCHANGED()
	ON_CONTROL_REFLECT_EX(STN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTWM_INITIAL_UPDATE, OnInitControl)
END_MESSAGE_MAP()

BOOL CXTHyperLink::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if ( m_hcurHand == NULL ) {
		m_hcurHand = XTAuxData().hcurHand;
	}

	// if the hand cursor is not NULL and the mouse is over text,
	// set the hand cursor.
	if ( m_hcurHand && IsLinkHot() )
	{
		::SetCursor( m_hcurHand );
		return TRUE;
	}

	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

UINT CXTHyperLink::OnNcHitTest(CPoint point)
{
	UNUSED_ALWAYS(point);
	return HTCLIENT;
}

LRESULT CXTHyperLink::OnInitControl(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (m_bInitControl)
	{
		m_bInitControl = false;

		// only use the window text if this is a text link and
		// the URL has not already been initialized first.
		if (IsTextControl() && m_strLink.IsEmpty()) {
			GetWindowText(m_strLink);
		}

		ModifyStyle(NULL, SS_NOTIFY);
		if ( m_strTipText.IsEmpty( ) )
		{
			m_strTipText = m_strLink;
		}

		m_toolTip.Create(this);
		m_toolTip.AddTool(this, m_strTipText);
		m_toolTip.Activate( m_bTipEnabled );

		// create the font used by the hyperlink control.
		LOGFONT lf;
		GetFont()->GetLogFont(&lf);
		lf.lfUnderline = m_bUnderline;
		m_font.CreateFontIndirect(&lf);

		SetFont(&m_font);
	}

	return 0;
}

void CXTHyperLink::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();

	// Initialize the control.
	PostMessage(XTWM_INITIAL_UPDATE);
}

int CXTHyperLink::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize the control.
	SendMessage(XTWM_INITIAL_UPDATE);

	return 0;
}

void CXTHyperLink::ReportError(int iErrorNo)
{
	CString strMessage;
	switch( iErrorNo )
	{
	case 0:                      XTPResourceManager()->LoadString(&strMessage, XT_IDS_SE_0 );                   break;
	case SE_ERR_FNF:             XTPResourceManager()->LoadString(&strMessage, XT_IDS_SE_ERR_FNF );             break;
	case SE_ERR_PNF:             XTPResourceManager()->LoadString(&strMessage, XT_IDS_SE_ERR_PNF );             break;
	case SE_ERR_ACCESSDENIED:    XTPResourceManager()->LoadString(&strMessage, XT_IDS_SE_ERR_ACCESSDENIED );    break;
	case SE_ERR_OOM:             XTPResourceManager()->LoadString(&strMessage, XT_IDS_SE_ERR_OOM );             break;
	case ERROR_BAD_FORMAT:       XTPResourceManager()->LoadString(&strMessage, XT_IDS_SE_ERR_BAD_FORMAT );      break;
	case SE_ERR_SHARE:           XTPResourceManager()->LoadString(&strMessage, XT_IDS_SE_ERR_SHARE );           break;
	case SE_ERR_ASSOCINCOMPLETE: XTPResourceManager()->LoadString(&strMessage, XT_IDS_SE_ERR_ASSOCINCOMPLETE ); break;
	case SE_ERR_DDETIMEOUT:      XTPResourceManager()->LoadString(&strMessage, XT_IDS_SE_ERR_DDETIMEOUT );      break;
	case SE_ERR_DDEFAIL:         XTPResourceManager()->LoadString(&strMessage, XT_IDS_SE_ERR_DDEFAIL );         break;
	case SE_ERR_DDEBUSY:         XTPResourceManager()->LoadString(&strMessage, XT_IDS_SE_ERR_DDEBUSY );         break;
	case SE_ERR_NOASSOC:         XTPResourceManager()->LoadString(&strMessage, XT_IDS_SE_ERR_NOASSOC );         break;
	case SE_ERR_DLLNOTFOUND:     XTPResourceManager()->LoadString(&strMessage, XT_IDS_SE_ERR_DLLNOTFOUND );     break;
	default:
		{
			CString str;
			XTPResourceManager()->LoadString(&str, XT_IDS_SE_ERR_UNKOWN);
			strMessage.Format(str, iErrorNo);
		}
	}

	MessageBeep( MB_ICONEXCLAMATION );
	AfxMessageBox( strMessage, MB_ICONEXCLAMATION | MB_OK );
}

bool CXTHyperLink::GotoURL(LPCTSTR lpszLink, int nShowCmd)
{
	if (!lpszLink)
		lpszLink = m_strLink;

	// open the URL
	int iRetVal = (int)(INT_PTR)::ShellExecute(NULL, _T("open"),
		lpszLink, NULL, NULL, nShowCmd);

	// alert user if there was an error.
	if (iRetVal <= HINSTANCE_ERROR)
		ReportError(iRetVal);

	return (iRetVal > HINSTANCE_ERROR);
}

HBRUSH CXTHyperLink::CtlColor(CDC* pDC, UINT nCtlColor)
{
	UNUSED_ALWAYS(nCtlColor);
	ASSERT(nCtlColor == CTLCOLOR_STATIC);

	if (IsTextControl())
	{
		HBRUSH hbr = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);

		// draw the current state color.
		if (m_bMouseOver) {
			pDC->SetTextColor(m_clrHover);
		}
		else
		if (m_bVisited) {
			pDC->SetTextColor(m_clrVisited);
		}
		else {
			pDC->SetTextColor(m_clrLink);
		}

		pDC->SetBkMode(TRANSPARENT);
		return hbr;
	}

	return NULL;
}

void CXTHyperLink::OnMouseMove(UINT nFlags, CPoint point)
{
	if (IsTextControl() && IsLinkHot()) {
		SetTimer(1, 10, NULL);
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CXTHyperLink::OnTimer(UINT_PTR nIDEvent)
{
	if (IsTextControl())
	{
		if (!IsLinkHot() && m_bMouseOver)
		{
			KillTimer(1);
			m_bMouseOver = false;
		}
		else if(!m_bMouseOver)
		{
			m_bMouseOver = true;
		}

		Invalidate();
		UpdateWindow();
	}

	CStatic::OnTimer(nIDEvent);
}

BOOL CXTHyperLink::PreTranslateMessage(MSG* pMsg)
{
	if (!m_bInitControl && m_bTipEnabled)
	{
		m_toolTip.RelayEvent(pMsg);
	}
	return CStatic::PreTranslateMessage(pMsg);
}

BOOL CXTHyperLink::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

void CXTHyperLink::OnPaint()
{
	// if the WS_EX_TRANSPARENT style is set, we have to use
	// default paint routines otherwise our background will still
	// be painted.

	if ((::GetWindowLong(m_hWnd, GWL_EXSTYLE)
		& WS_EX_TRANSPARENT) != 0)
	{
		Default();
	}

	else
	{
		CPaintDC dc(this);

		// Get the client rect, and paint to a memory device context.  This will
		// help reduce screen flicker. Pass the memory device context to the
		// default window procedure do default painting.

		CRect r;
		GetClientRect( &r );
		CXTMemDC memDC( &dc, r, GetBackColor( ) );

		CStatic::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
	}
}

void CXTHyperLink::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CStatic::OnWindowPosChanged(lpwndpos);

	// if we are resized, redraw hyperlink.
	Invalidate();
	UpdateWindow();
}

BOOL CXTHyperLink::OnClicked()
{
	if (IsLinkHot())
	{
		if (m_bShellExec)
		{
			m_bVisited = GotoURL(m_strLink, SW_SHOW);
			return TRUE;
		}
		else
		{
			m_bVisited = true;
		}
	}

	return FALSE;   // continue routing
}

bool CXTHyperLink::IsLinkHot()
{
	// if this isn't a text control, just return true
	if (!IsTextControl())
		return true;

	// get the text for the hyperlink control.
	CString strText;
	GetWindowText(strText);

	// get the size of the text.
	CWindowDC dc(NULL);
	CFont* pFont = dc.SelectObject(GetFont());
	CSize size = dc.GetTextExtent(strText);
	dc.SelectObject(pFont);

	// get the total size of the hyperlink
	CRect r;
	GetWindowRect(&r);

	// construct the actual size of the text.
	CRect rHot = r;
	switch (GetStyle() & (SS_LEFT|SS_CENTER|SS_RIGHT))
	{
	case SS_LEFT:
		{
			rHot.right = rHot.left + size.cx;
		}
		break;

	case SS_CENTER:
		{
			int cx = (r.Width() - size.cx) / 2;
			rHot.right -= cx;
			rHot.left  += cx;
		}
		break;

	case SS_RIGHT:
		{
			rHot.left = rHot.right - size.cx;
		}
		break;
	}

	// get the cursor location, if it is over the
	// text, return true, this means the hyperlink
	// should appear hot.
	CPoint pt;
	::GetCursorPos(&pt);
	if (rHot.PtInRect(pt))
	{
		return true;
	}

	return false;
}

void CXTHyperLink::EnableToolTips(bool bEnable/*=true*/)
{
	m_bTipEnabled = bEnable;

	// if the tooltip control is already created, activate or
	// deactivate it.
	if ( ::IsWindow( m_toolTip.m_hWnd ) )
	{
		m_toolTip.Activate( bEnable );
	}
}

void CXTHyperLink::SetTipText(LPCTSTR lpszTipText)
{
	m_strTipText = lpszTipText;

	// if the tooltip control is already created, update the
	// tip text.
	if ( ::IsWindow( m_toolTip.m_hWnd ) )
	{
		m_toolTip.UpdateTipText( m_strTipText, this );
	}
}
