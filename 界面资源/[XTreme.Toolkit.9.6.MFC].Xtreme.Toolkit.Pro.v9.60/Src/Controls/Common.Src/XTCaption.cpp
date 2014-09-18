// XTCaption.cpp : implementation of the CXTCaption class.
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

#include "XTPWinThemeWrapper.h"

#include "XTResource.h"
#include "XTDefines.h"
#include "XTUtil.h"
#include "XTGlobal.h"
#include "XTButton.h"
#include "XTCaption.h"
#include "XTCaptionPopupWnd.h"
#include "XTMemDC.h"
#include "XTPDrawHelpers.h"
#include "XTThemeManager.h"
#include "XTButtonTheme.h"
#include "XTButtonThemeOffice.h"
#include "XTFunctions.h"
#include "XTCaptionTheme.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTCaptionButton
/////////////////////////////////////////////////////////////////////////////

DWORD CXTCaptionButton::SetXButtonStyle(DWORD /*dwxStyle*/,BOOL bRedraw/*=TRUE*/)
{
	DWORD dwOldStyle = m_dwxStyle;

	CXTCaption* pCaption = DYNAMIC_DOWNCAST(CXTCaption, GetSafeHwnd()? GetParent(): 0);
	if (pCaption)
	{
		/*if ((dwxStyle & BS_XT_XPFLAT) != 0)
		{
			m_dwxStyle = (dwxStyle & ~BS_XT_FLAT);
			SetTheme(new CXTCaptionButtonThemeOfficeXP(pCaption));
		}
		else
		{
			m_dwxStyle = dwxStyle|BS_XT_FLAT;
			SetTheme(new CXTCaptionButtonThemeOffice(pCaption));
		}*/

		// set the button font and colors.
		SetColorFace(pCaption->m_clrFace);
		SetColorText(pCaption->m_clrText);
		SetColorShadow(GetXtremeColor(COLOR_3DDKSHADOW));

		// set the button font to match caption font.
		SetFont(pCaption->GetFont());

		// Redraw the button.
		if (bRedraw)
			Invalidate();
	}

	return dwOldStyle;
}

/////////////////////////////////////////////////////////////////////////////
// CXTCaption
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_THEME_HOST(CXTCaption, CXTCaptionTheme)

CXTCaption::CXTCaption()
: CXTThemeManagerStyleHost(GetThemeFactoryClass())
{
	m_bUserColors   = false;
	m_strCaption    = _T("");
	m_hIcon         = NULL;
	m_pChildWnd     = NULL;
	m_pParentView   = NULL;
	m_pSplitterWnd  = NULL;
	m_pPopupWnd     = NULL;
	m_nOffset       = 0;
	m_nBorder       = 0;
	m_sizeIcon.cx   = __min(XTAuxData().cxSmIcon, 16);
	m_sizeIcon.cy   = __min(XTAuxData().cySmIcon, 16);
	m_dwExStyle     = 0L;
	m_clrText       = ::GetSysColor(COLOR_BTNTEXT);
	m_clrBorder     = ::GetSysColor(COLOR_3DFACE);
	m_clrFace       = ::GetSysColor(COLOR_3DFACE);
}

CXTCaption::~CXTCaption()
{

}

IMPLEMENT_DYNAMIC(CXTCaption, CStatic)

BEGIN_MESSAGE_MAP(CXTCaption, CStatic)
	//{{AFX_MSG_MAP(CXTCaption)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(XT_IDC_BTN_CLOSE, OnCaptButton)
	ON_MESSAGE_VOID(CPWN_XT_PUSHPINBUTTON, OnPushPinButton)
	ON_MESSAGE_VOID(CPWN_XT_PUSHPINCANCEL, OnPushPinCancel)

END_MESSAGE_MAP()

void CXTCaption::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	if (m_hIcon != NULL)
		RedrawWindow();
}

void CXTCaption::OnPaint()
{
	// background is already filled in gray
	CPaintDC dc(this);

	// Get the client rect.
	CRect rectClient;
	GetClientRect(&rectClient);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTMemDC memDC(&dc, rectClient);

	// If the caption button is a valid window and visible exclude from painting...
	if (::IsWindow(m_btnCaption.m_hWnd) && m_btnCaption.IsWindowVisible())
	{
		CXTPWindowRect rcButton(&m_btnCaption);
		ScreenToClient(&rcButton);
		memDC.ExcludeClipRect(&rcButton);
	}

	// draw the background, text and icon.
	DrawCaptionBack(&memDC, rectClient);
	DrawCaptionText(&memDC);
	DrawCaptionIcon(&memDC, rectClient);
}

BOOL CXTCaption::OnEraseBkgnd(CDC* pDC)
{
	UNUSED_ALWAYS(pDC);
	return FALSE;
}

void CXTCaption::DrawCaptionBack(CDC* pDC, CRect& rcItem)
{
	GetTheme()->DrawCaptionBack(pDC, this, rcItem);
}

void CXTCaption::DrawCaptionText(CDC* pDC)
{
	GetTheme()->DrawCaptionText(pDC, this);
}

void CXTCaption::DrawCaptionIcon(CDC* pDC, CRect& rcItem)
{
	GetTheme()->DrawCaptionIcon(pDC, this, rcItem);
}

BOOL CXTCaption::Create(CWnd* pParentWnd, LPCTSTR lpszWindowName, DWORD dwExStyle, DWORD dwStyle, const CRect& rect, UINT nID)
{
	// Let the base class create the control.
	if( !CStatic::Create( NULL, dwStyle|WS_CLIPCHILDREN, rect, pParentWnd, nID ))
	{
		TRACE(_T("Unable to create caption.\n"));
		return FALSE;
	}

	SetFont(&XTAuxData().font);

	// save the style.
	m_dwExStyle = (dwExStyle & (CPWS_EX_GROOVE_EDGE | CPWS_EX_RAISED_EDGE | CPWS_EX_CLOSEBUTTON));
	ModifyStyleEx( 0, (dwExStyle & ~m_dwExStyle));

	// Save the window text.
	m_strCaption = lpszWindowName;

	if (HasCloseButton())
	{
		// Create the caption's close button.
		if (!m_btnCaption.Create(NULL, WS_VISIBLE|WS_CHILD|BS_ICON|BS_OWNERDRAW|BS_CENTER|BS_VCENTER,
			CRect(0,0,0,0), this, XT_IDC_BTN_CLOSE))
		{
			TRACE0( "Unable to create caption button.\n" );
			return -1;
		}

		// Create the image list used by frame buttons.
		m_ilButton.Create (XT_IDB_BTN_OUT, 16, 1, RGB(255,0,255));

		// and set the icon for the caption's close button
		m_btnCaption.SetIcon( CSize( 16, 15 ), m_ilButton.ExtractIcon( 2 ) );
	}
	else
	{
		// Create the button to be used with child window.
		if( !m_btnCaption.Create( m_strCaption, BS_ICON|BS_OWNERDRAW|BS_VCENTER,
			CRect(0,0,0,0), this, XT_IDC_BTN_CLOSE ))
		{
			TRACE0( "Unable to create caption button.\n" );
			return FALSE;
		}
	}
	OnThemeChanged();

	SetWindowPos(NULL, 0,0,0,0,
		SWP_NOSIZE|SWP_NOMOVE|SWP_FRAMECHANGED);

	return TRUE;
}

void CXTCaption::OnThemeChanged()
{
	CXTButtonTheme* pTheme = GetTheme()->GetButtonTheme(this);
	m_btnCaption.SetTheme(pTheme);

	m_btnCaption.SetXButtonStyle(0, FALSE);
}

void CXTCaption::ModifyCaptionStyle(int nBorderSize, CFont* pFont, LPCTSTR lpszWindText, HICON hIcon)
{
	ASSERT(::IsWindow(m_hWnd));

	if (nBorderSize != 0) {
		m_nBorder = nBorderSize;
	}

	if (pFont != NULL) {
		SetFont(pFont);

		// button font should match caption
		if (::IsWindow(m_btnCaption.m_hWnd))
			m_btnCaption.SetFont(GetFont());
	}

	if (lpszWindText != NULL) {
		m_strCaption = lpszWindText;
	}

	if (hIcon != NULL) {
		m_hIcon = hIcon;
	}

	Invalidate();
}

void CXTCaption::SetChildWindow(CWnd* pChild, CWnd* pNotifyWnd)
{
	ASSERT_VALID (pChild);      // must be valid.
	ASSERT_VALID (pNotifyWnd);  // must be valid.

	// save window pointers, and show caption button.
	m_pChildWnd  = pChild;
	SetOwner(pNotifyWnd);

	m_pParentView  = pChild->GetParent();
	ASSERT_KINDOF(CView, m_pParentView);

	m_pSplitterWnd = m_pParentView->GetParent();
	ASSERT_KINDOF(CSplitterWnd, m_pSplitterWnd);

	// get the size of the child and parent windows.
	m_pChildWnd   ->GetClientRect (&m_rcChild);
	m_pParentView ->GetClientRect (&m_rcParent);
	m_pSplitterWnd->GetClientRect (&m_rcSplitter);

	// save the size of the childs caption area.
	m_nOffset = (m_rcParent.Height() - m_rcChild.Height());

	// make the caption button visible.
	m_btnCaption.ShowWindow(SW_SHOW);
	m_btnCaption.UpdateWindow();

	// inflate the size of the parent to add a border, this will
	// also be the size of the popup window.
	int cx = ::GetSystemMetrics (SM_CXEDGE)*2;
	int cy = ::GetSystemMetrics (SM_CYEDGE)*2;
	m_rcParent.InflateRect(cx,cy);
}

void CXTCaption::KillChildWindow()
{
	// Hide the caption button.
	if (::IsWindow(m_btnCaption.m_hWnd))
	{
		m_btnCaption.ShowWindow(SW_HIDE);
		m_btnCaption.SetState(FALSE);
	}

	// Destroy the pop-up window.
	if (::IsWindow(m_pPopupWnd->GetSafeHwnd()))
	{
		m_pPopupWnd->DestroyWindow();
	}

	SAFE_DELETE( m_pPopupWnd );
}

BOOL CXTCaption::ShowPopupWindow()
{
	// already visible return TRUE.
	if (::IsWindowVisible(m_pPopupWnd->GetSafeHwnd()))
		return TRUE;

	// if not a window yet create it.
	if (!::IsWindow(m_pPopupWnd->GetSafeHwnd()))
	{
		m_pPopupWnd = new CXTCaptionPopupWnd;

		if (!m_pPopupWnd->Create(CRect(0,0,0,0), this, m_pChildWnd))
		{
			SAFE_DELETE(m_pPopupWnd);
			return FALSE;
		}
	}

	// view pointer is not valid.
	if (!::IsWindow(m_pParentView->GetSafeHwnd()))
		return FALSE;

	// splitter pointer is not valid.
	if (!::IsWindow(m_pSplitterWnd->GetSafeHwnd()))
		return FALSE;

	CRect rcPopup(m_rcParent);
	m_pParentView->ClientToScreen(&rcPopup);

	// get the current size of the splitter window.
	CRect rcSplitter;
	m_pSplitterWnd->GetClientRect(&rcSplitter);

	// check to see if the size has changed.
	if (rcSplitter.Height() > m_rcSplitter.Height()) {
		rcPopup.bottom += (rcSplitter.Height() - m_rcSplitter.Height());
	}
	else if (m_rcSplitter.Height() > rcSplitter.Height()) {
		rcPopup.bottom -= (m_rcSplitter.Height() - rcSplitter.Height());
	}

	// display the popup window.
	m_pPopupWnd->MoveWindow(&rcPopup);
	m_pPopupWnd->ShowWindow(SW_SHOW);
	m_pPopupWnd->RecalcLayout();

	// press the caption button.
	m_btnCaption.SetState(TRUE);

	return TRUE;
}

void CXTCaption::OnCaptButton()
{
	if (HasCloseButton())
	{
		CWnd* pOwner = GetOwner();
		ASSERT_VALID(pOwner);

		if (pOwner)
		{
			// Notify of selection change.
			pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
				BN_CLICKED), (LPARAM)m_hWnd );
		}
	}
	else
	{
		ShowPopupWindow();
	}
}

void CXTCaption::OnPushPinButton()
{
	KillChildWindow();

	// get the current size of the child's parent window and
	// adjust the size of the child accordingly.
	m_pParentView->GetClientRect (&m_rcParent);
	m_rcChild      = m_rcParent;
	m_rcChild.top += m_nOffset;

	// move the child window to its correct location.
	m_pChildWnd->MoveWindow (m_rcChild);

	CWnd* pNotifyWnd = GetOwner();
	ASSERT_VALID(pNotifyWnd);

	if (::IsWindow(pNotifyWnd->GetSafeHwnd()))
	{
		// Notify that push pin button was pressed.
		pNotifyWnd->SendMessage(CPWN_XT_PUSHPINBUTTON);
		Invalidate();
	}
}

void CXTCaption::OnPushPinCancel()
{
	m_btnCaption.SetState(FALSE);
	m_pPopupWnd->DestroyWindow();

	SAFE_DELETE (m_pPopupWnd);

	CWnd* pNotifyWnd = GetOwner();
	ASSERT_VALID(pNotifyWnd);

	if (::IsWindow(pNotifyWnd->GetSafeHwnd()))
	{
		// Notify that popup window has lost input focus.
		pNotifyWnd->SendMessage(CPWN_XT_PUSHPINCANCEL);
		Invalidate();
	}
}

void CXTCaption::UpdateCaption(LPCTSTR lpszWindowText, HICON hIcon)
{
	ASSERT(::IsWindow(m_hWnd));

	CRect rcClient;
	GetClientRect(&rcClient);
	CDC* pDC = GetDC();

	DrawCaptionBack(pDC, rcClient);

	if (lpszWindowText != NULL)
	{
		m_strCaption = lpszWindowText;
		UpdateCaptionText(pDC);
	}

	if (hIcon != NULL)
	{
		m_hIcon = hIcon;
		DrawCaptionIcon( pDC, rcClient );
	}

	ReleaseDC(pDC);
}

void CXTCaption::UpdateCaptionText(CDC* pDC)
{
	if (::IsWindow(m_btnCaption.m_hWnd))
	{
		m_btnCaption.SetWindowText(m_strCaption);
		m_btnCaption.MoveWindow(GetButtonRect());
	}

	DrawCaptionText(pDC);
}

CRect CXTCaption::GetTextRect() const
{
	if (m_strCaption.IsEmpty())
		return CXTPEmptyRect();

	CXTPClientRect rcClient(this);

	// Use a NULL window dc to get the size the caption button
	// should be using CDC::GetTextExtent(...).
	CWindowDC dc(NULL);
	CXTPFontDC fontDC(&dc, GetFont());

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);

	// Calculate the button area size.
	CSize size = dc.GetTextExtent(m_strCaption, m_strCaption.GetLength());
	size.cx += tm.tmAveCharWidth + 12;
	size.cy  = rcClient.Height();

	int iWidth = rcClient.Width()-(m_hIcon ?18 :0);
	if (size.cx > iWidth)
		size.cx = iWidth;

	return CRect(m_nBorder, m_nBorder, m_nBorder + size.cx, size.cy - m_nBorder);
}

CRect CXTCaption::GetButtonRect() const
{
	if (!::IsWindow(m_btnCaption.m_hWnd))
		return CXTPEmptyRect();

	if (!HasCloseButton())
		return GetTextRect();

	CSize szIcon(16,15);
	CXTPClientRect rcClient(this);

	CRect rcButton;
	rcButton.left   = (rcClient.Width() -szIcon.cx)-2;
	rcButton.top    = (rcClient.Height()-szIcon.cy)/2;
	rcButton.right  = rcButton.left + szIcon.cx;
	rcButton.bottom = rcButton.top  + szIcon.cy;

	return rcButton;
}

void CXTCaption::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CStatic::OnWindowPosChanged(lpwndpos);

	if (::IsWindow(m_btnCaption.m_hWnd))
		m_btnCaption.MoveWindow(GetButtonRect());
}

void CXTCaption::SetCaptionColors(COLORREF clrBorder, COLORREF clrFace, COLORREF clrText)
{
	ASSERT(::IsWindow(m_hWnd));

	m_bUserColors = true;

	m_clrBorder = clrBorder;
	m_clrFace   = clrFace;
	m_clrText   = clrText;

	if (::IsWindow(m_btnCaption.m_hWnd))
	{
		m_btnCaption.SetColorFace(m_clrFace);
		m_btnCaption.SetColorText(m_clrText);
		m_btnCaption.SetColorShadow(::GetSysColor(COLOR_3DDKSHADOW));
	}
}

void CXTCaption::SetOffice2003Colors(bool b2003Colors/*=true*/)
{
	m_bUserColors = b2003Colors;

	SetTheme(b2003Colors? new CXTCaptionThemeOffice2003(): 0);
}

void CXTCaption::OnSysColorChange()
{
	CStatic::OnSysColorChange();

	// update caption colors if not user defined.
	if (m_bUserColors == false)
	{
		m_clrText   = ::GetSysColor(COLOR_BTNTEXT);
		m_clrBorder = ::GetSysColor(COLOR_3DFACE);
		m_clrFace   = ::GetSysColor(COLOR_3DFACE);

		if (::IsWindow(m_btnCaption.m_hWnd))
		{
			m_btnCaption.SetColorFace(m_clrFace);
			m_btnCaption.SetColorText(m_clrText);
			m_btnCaption.SetColorShadow(::GetSysColor(COLOR_3DDKSHADOW));
		}
	}
}
