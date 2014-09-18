// XTButton.cpp : implementation of the CXTButton class.
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
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTFunctions.h"
#include "XTMemDC.h"
#include "XTHelpers.h"
#include "uxtheme.h"
#include "schemadef.h"
#include "tmschema.h"
#include "XTPWinThemeWrapper.h"
#include "XTThemeManager.h"
#include "XTButtonTheme.h"
#include "XTButtonThemeOffice.h"
#include "XTPColorManager.h"
#include "XTPDrawHelpers.h"
#include "XTPImageManager.h"
#include "XTFunctions.h"
#include "XTButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTButton
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CXTButton, CButton)

IMPLEMENT_THEME_HOST(CXTButton, CXTButtonTheme)


CXTButton::CXTButton()
: CXTThemeManagerStyleHost(GetThemeFactoryClass())
, m_nBorderGap(4)
, m_nImageGap(8)
, m_bHilite(false)
, m_bUserPosition(false)
, m_bLBtnDown(false)
, m_bPainted(false)
, m_bChecked(false)
, m_dwxStyle(BS_XT_FLAT)
, m_sizeImage(CSize(0,0))
, m_ptImage(CPoint(0,0))
, m_ptText(CPoint(0,0))
, m_pIcon(NULL)
{

}


CXTButton::~CXTButton()
{
	CleanUpGDI();

	SAFE_DELETE(m_pIcon);
}

HICON CXTButton::GetNormalIcon()
{
	return m_pIcon? (HICON)m_pIcon->GetIcon(): NULL;
}

HICON CXTButton::GetSelectedIcon()
{
	return m_pIcon? (HICON)m_pIcon->GetHotIcon(): NULL;
}


BEGIN_MESSAGE_MAP(CXTButton, CButton)
	//{{AFX_MSG_MAP(CXTButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	NoteButtonDepressed(true);
	CButton::OnLButtonDown(nFlags, point);
}

void CXTButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	NoteButtonDepressed(false);
	CButton::OnLButtonUp(nFlags, point);
}

void CXTButton::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		CRect rc;
		GetWindowRect(rc);

		CPoint pt;
		GetCursorPos(&pt);

		if (m_bLBtnDown == TRUE || (!rc.PtInRect(pt)))
		{
			KillTimer(1);

			m_bHilite = FALSE;

			if (m_bPainted == TRUE) {
				InvalidateRect (NULL);
			}

			m_bPainted = FALSE;
			return;
		}

		// On mouse over, show raised button.
		else if (GetTheme()->CanHilite(this) && !m_bPainted)
		{
			m_bHilite = TRUE;
			InvalidateRect(NULL);
			m_bPainted = true;
		}
	}

	CButton::OnTimer(nIDEvent);
}

void CXTButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	GetTheme()->DrawItem(lpDIS, this);
}

void CXTButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	POINT pt;

	::GetCursorPos(&pt);
	::MapWindowPoints(NULL,(HWND)(m_hWnd),&pt,1);
	::SendMessage((HWND)(m_hWnd),WM_LBUTTONDOWN,0,MAKELPARAM(pt.x,pt.y));

	CButton::OnLButtonDblClk(nFlags, point);
}

void CXTButton::PreSubclassWindow()
{
	CButton::PreSubclassWindow();

	// Set the style to BS_OWNERDRAW.
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle|BS_OWNERDRAW);
}

int CXTButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Set the style to BS_OWNERDRAW.
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle|BS_OWNERDRAW);

	return 0;
}

CSize CXTButton::GetImageSize() const
{
	if (!GetTheme()->IsIconVisible())
		return CSize(0,0);

	return m_sizeImage;
}

CPoint CXTButton::GetImagePoint() const
{
	if (!GetTheme()->IsIconVisible())
		return CPoint(0,0);

	return m_ptImage;
}

CPoint CXTButton::GetTextPoint() const
{
	if (!GetTheme()->IsIconVisible())
		return CPoint(0,0);

	return m_ptText;
}

void CXTButton::CleanUpGDI()
{
	if (m_pIcon)
	{
		m_pIcon->Refresh();
	}
}

BOOL CXTButton::SetIcon(CSize size, UINT nID, UINT nHotID/*=0*/, BOOL bRedraw/*=TRUE*/)
{
	return CXTButton::SetIcon(
		size, MAKEINTRESOURCE(nID), MAKEINTRESOURCE(nHotID), bRedraw);
}

BOOL CXTButton::SetIcon(CSize size, LPCTSTR lpszID, LPCTSTR lpszHotID/*=NULL*/, BOOL bRedraw/*=TRUE*/)
{
	HICON hIcon = NULL;
	HICON hIconHot = NULL;

	// Free previous resources (if any).
	CleanUpGDI();

	// Find the resource for the normal icon and load the image.
	HINSTANCE hInst = AfxFindResourceHandle(lpszID, RT_GROUP_ICON);
	hIcon = (HICON)::LoadImage(hInst, lpszID,
		IMAGE_ICON, size.cx, size.cy, LR_DEFAULTCOLOR);

	// Return false if the icon handle is NULL.
	if (!hIcon)
	{
		TRACE1( "Failed to load Icon resource %s.\n", lpszID );
		return FALSE;
	}

	// If we are using a pushed image as well...
	if (lpszHotID)
	{
		// Find the resource for the pushed icon and load the image.
		hInst = AfxFindResourceHandle(lpszHotID, RT_GROUP_ICON);
		hIconHot = (HICON)::LoadImage(hInst, lpszHotID,
			IMAGE_ICON, size.cx, size.cy, LR_DEFAULTCOLOR);

		// Return false if the icon handle is NULL.
		if (!hIconHot)
		{
			TRACE0( "Failed to load Icon resource.\n" );
			return FALSE;
		}
	}

	return CXTButton::SetIcon(size, hIcon, hIconHot, bRedraw);
}

BOOL CXTButton::SetIcon(CSize size, HICON hIcon, HICON hIconHot/*=NULL*/, BOOL bRedraw/*=TRUE*/)
{
	ASSERT(hIcon);

	// Save the image size and set the icon
	// handles to NULL.
	m_sizeImage = size != CSize(0)? size: CXTPImageManagerIcon::GetExtent(hIcon);

	// Construct the icon manager.
	SAFE_DELETE(m_pIcon);

	m_pIcon = new CXTPImageManagerIcon(0, size.cx, size.cy);
	m_pIcon->SetIcon(hIcon);

	if (hIconHot)
		m_pIcon->SetHotIcon(hIconHot);

	// Redraw the button.
	if (::IsWindow(m_hWnd) && bRedraw)
		Invalidate();

	return TRUE;
}

BOOL CXTButton::SetBitmap(CSize size, UINT nID, BOOL bRedraw/*=TRUE*/)
{
	// Free previous resources (if any).
	CleanUpGDI();

	CBitmap bmpIcon;

	BOOL bAlphaBitmap = CXTPImageManager::IsAlphaBitmapResource(nID);

	if (bAlphaBitmap)
	{
		HBITMAP hBmp = CXTPImageManagerIcon::LoadAlphaBitmap(nID);

		if (!hBmp)
			return FALSE;

		bmpIcon.Attach(hBmp);

		// Construct the icon manager.
		SAFE_DELETE(m_pIcon);

		m_pIcon = new CXTPImageManagerIcon(0, 0, 0);
		m_pIcon->SetIcon(CXTPImageManagerIconHandle(bmpIcon));

		// Save the image size and set the icon
		// handles to NULL.
		m_sizeImage = size != CSize(0)? size: CSize(m_pIcon->GetWidth(), m_pIcon->GetHeight());

		// Redraw the button.
		if (::IsWindow(m_hWnd) && bRedraw)
			Invalidate();
	}
	else
	{
		if (!bmpIcon.LoadBitmap(nID))
			return FALSE;

		// convert the bitmap to a transparent icon.
		HICON hIcon = CXTPTransparentBitmap(bmpIcon).ConvertToIcon();

		BOOL bResult =  CXTButton::SetIcon(size, hIcon, NULL, bRedraw);

		DestroyIcon(hIcon);

		return bResult;
	}
	return TRUE;
}

BOOL CXTButton::SetTextAndImagePos(CPoint ptImage, CPoint ptText, BOOL bRedraw/*=TRUE*/)
{
	m_ptImage = ptImage;
	m_ptText  = ptText;

	// Redraw the button.
	if (::IsWindow(m_hWnd) && bRedraw)
		Invalidate();

	m_bUserPosition = TRUE;
	return TRUE;
}

DWORD CXTButton::SetXButtonStyle(DWORD dwxStyle, BOOL bRedraw/*=TRUE*/)
{
	DWORD dwOldStyle = m_dwxStyle;
	m_dwxStyle = dwxStyle;

	if ((m_dwxStyle & BS_XT_XPFLAT) == BS_XT_XPFLAT)
		SetTheme(new CXTButtonThemeOfficeXP());
	else if ((m_dwxStyle & BS_XT_FLAT) == BS_XT_FLAT)
		SetTheme(new CXTButtonTheme());
	else SetTheme(0);

	// Redraw the button.
	if (::IsWindow(m_hWnd) && bRedraw)
		Invalidate();

	return dwOldStyle;
}

void CXTButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (GetTheme()->CanHilite(this))
	{
		SetTimer(1, 10, NULL);
		OnTimer(1);
	}

	CButton::OnMouseMove(nFlags, point);
}

void CXTButton::OnPaint()
{
	// background is already filled in gray
	CPaintDC dc(this);

	// Get the client rect.
	CRect rectClient;
	GetClientRect(&rectClient);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTMemDC memDC(&dc, rectClient);

	// Now let the window do its default painting...
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
}

BOOL CXTButton::OnEraseBkgnd(CDC* pDC)
{
	UNUSED_ALWAYS(pDC);
	return FALSE;
}

void CXTButton::NoteButtonDepressed(BOOL bDepressed)
{
	if (m_bLBtnDown != bDepressed)
	{
		m_bLBtnDown = bDepressed;
		Invalidate();
	}
}


BOOL CXTButton::IsThemeValid() const
{
	return (GetTheme() != NULL);
}

CXTPImageManagerIcon* CXTButton::GetIcon() const
{
	return m_pIcon;
}

CString CXTButton::GetButtonText(BOOL bRemoveAmpersand)
{
	CString strText;
	GetWindowText(strText);

	if (bRemoveAmpersand)
		XTFuncStripMnemonics(strText);

	return strText;
}

void CXTButton::SetAlternateColors(COLORREF clr3DFace, COLORREF clr3DHilight, COLORREF clr3DShadow, COLORREF clrBtnText)
{
	if (IsThemeValid())
	{
		GetTheme()->SetAlternateColors(
			clr3DFace, clr3DHilight, clr3DShadow, clrBtnText);
	}
}

void CXTButton::SetColorFace(COLORREF clrFace)
{
	if (IsThemeValid())
	{
		GetTheme()->SetColorFace(clrFace);
	}
}

COLORREF CXTButton::GetColorFace()
{
	if (IsThemeValid())
	{
		return GetTheme()->GetColorFace();
	}
	return XTAuxData().clr3DFace;
}

void CXTButton::SetColorHilite(COLORREF clrHilite)
{
	if (IsThemeValid())
	{
		GetTheme()->SetColorHilite(clrHilite);
	}
}

void CXTButton::SetColorShadow(COLORREF clrShadow)
{
	if (IsThemeValid())
	{
		GetTheme()->SetColorShadow(clrShadow);
	}
}

void CXTButton::SetColorText(COLORREF clrText)
{
	if (IsThemeValid())
	{
		GetTheme()->SetColorText(clrText);
	}
}

void CXTButton::SetXPFlatColors(COLORREF clrBorder, COLORREF clrHighlight, COLORREF clrPressed)
{
	if (IsThemeValid())
	{
		CXTButtonTheme* pButtonTheme = GetTheme();
		if (pButtonTheme->GetTheme() == xtThemeOfficeXP)
		{
			((CXTButtonThemeOfficeXP*)pButtonTheme)->SetColorHilite(clrBorder);
			((CXTButtonThemeOfficeXP*)pButtonTheme)->SetColorShadow(clrBorder);
			((CXTButtonThemeOfficeXP*)pButtonTheme)->SetBackHiliteColor(clrHighlight);
			((CXTButtonThemeOfficeXP*)pButtonTheme)->SetBackPushedColor(clrPressed);
		}
	}
}

void CXTButton::ResetImage()
{
	if (IsThemeValid())
	{
		if (GetTheme()->IsIconVisible()) {
			GetTheme()->ShowIcon(FALSE);
		}
		else {
			GetTheme()->ShowIcon(TRUE);
		}
		Invalidate();
	}
}

CFont* CXTButton::SetFontEx(CFont* pFont)
{
	CFont* pOldFont = GetFont();
	CButton::SetFont(pFont);
	return pOldFont;
}

void CXTButton::OnSysColorChange()
{
	CButton::OnSysColorChange();
	if (IsThemeValid())
	{
		GetTheme()->RefreshMetrics();
	}
}
