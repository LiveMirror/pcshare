// XTColorPageCustom.cpp : implementation file
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
#include <math.h>
#include "XTPDrawHelpers.h"
#include "XTPColorManager.h"
#include "XTPWinThemeWrapper.h"

#include "XTResource.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTFunctions.h"
#include "XTColorRef.h"
#include "XTWndHook.h"
#include "XTFlatComboBox.h"
#include "XTColorDialog.h"
#include "XTColorPageCustom.h"
#include "XTColorPageStandard.h"
#include "XTMemDC.h"
#include "XTVersionManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULT_LUMINANCE 0.50f

/////////////////////////////////////////////////////////////////////////////
// CXTColorBase

CXTColorBase::CXTColorBase()
{
	m_nLum         = 0.0;
	m_nSat         = 0.0;
	m_nHue         = 0.0;
	m_ptMousePos   = CPoint(0,0);
	m_clrColor     = RGB(0x00,0x00,0x00);
	m_bInitControl = true;

	RGBtoHSL(m_clrColor, &m_nHue, &m_nSat, &m_nLum);
}

CXTColorBase::~CXTColorBase()
{
}

CXTColorBase::FocusedControl CXTColorBase::m_eHasFocus = focusNone;

BEGIN_MESSAGE_MAP(CXTColorBase, CStatic)
	//{{AFX_MSG_MAP(CXTColorBase)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTWM_INITIAL_UPDATE, OnInitControl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTColorBase message handlers

LRESULT CXTColorBase::OnInitControl(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (m_bInitControl)
	{
		m_bInitControl = false;
		ModifyStyle(NULL, SS_NOTIFY);
	}

	return 0;
}

void CXTColorBase::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();

	// Initialize the control.
	PostMessage(XTWM_INITIAL_UPDATE);
}

int CXTColorBase::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize the control.
	SendMessage(XTWM_INITIAL_UPDATE);

	return 0;
}

void CXTColorBase::SetColor(COLORREF clr, bool bUpdate/*=true*/)
{
	UNUSED_ALWAYS(bUpdate);

	if (m_clrColor != (COLORREF)-1)
	{
		m_clrColor = clr;
		RGBtoHSL(m_clrColor, &m_nHue, &m_nSat, &m_nLum);
		Invalidate();
	}
}

void CXTColorBase::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
	SetCapture();
	UpdateCursorPos(point);

	if (GetFocus() != this) {
		SetFocus();
	}
}

void CXTColorBase::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	CStatic::OnLButtonUp(nFlags, point);
}

void CXTColorBase::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);

	if (nFlags & MK_LBUTTON)
	{
		UpdateCursorPos(point);

		if (GetFocus() != this) {
			SetFocus();
		}
	}
}

void CXTColorBase::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CStatic::OnLButtonDblClk(nFlags, point);

	UpdateCursorPos(point);

	CPropertyPage* pParentPage = (CPropertyPage*)GetParent();
	ASSERT_VALID(pParentPage);

	CPropertySheet* pParentSheet = (CPropertySheet*)pParentPage->GetParent();
	ASSERT_VALID(pParentSheet);

	pParentSheet->EndDialog(IDOK);
}

void CXTColorBase::UpdateCursorPos(CPoint point, BOOL bNotify)
{
	COLORREF color = m_dcPicker.GetPixel(point);

	if (color != (COLORREF)-1)
	{
		m_clrColor = color;

		if (bNotify)
		{
			GetParent()->SendMessage(XTWM_UPDATECOLOR,
				(WPARAM)(COLORREF)m_clrColor);
		}
	}

	m_ptMousePos = point;
}

COLORREF CXTColorBase::HLStoRGB(double h, double l, double s)
{
	return CXTColorRef::fromHSL(h, s, l);
}

void CXTColorBase::RGBtoHSL( COLORREF color, double *h, double *s, double *l)
{
	CXTColorRef(color).toHSL(*h, *s, *l);
}


/////////////////////////////////////////////////////////////////////////////
// CXTColorWnd

CXTColorWnd::CXTColorWnd()
{
}

CXTColorWnd::~CXTColorWnd()
{
}

BEGIN_MESSAGE_MAP(CXTColorWnd, CXTColorBase)
	//{{AFX_MSG_MAP(CXTColorWnd)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTColorWnd message handlers

void CXTColorWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);
	CXTMemDC memDC(&dc, rc);

	int cy = rc.Height();
	int cx = rc.Width();

	if (m_dcPicker.GetSafeHdc() == NULL)
	{
		// create bitmap
		CBitmap bmpPicker;
		bmpPicker.CreateCompatibleBitmap(&memDC, cx, cy);

		// create picker DC
		m_dcPicker.CreateCompatibleDC(&memDC);
		m_dcPicker.SelectObject(&bmpPicker);

		// fill color picker bitmap
		int x;
		for (x = 0; x < cx; x++)
		{
			int y;
			for (y = 0; y < cy; y++)
			{
				COLORREF clr = HLStoRGB((double)x/(double)cx,
					DEFAULT_LUMINANCE,  (double)y/(double)cy);

				m_dcPicker.SetPixelV(x, (cy-1)-y, clr);
			}
		}
	}

	memDC.BitBlt(0, 0, cx, cy, &m_dcPicker, 0, 0, SRCCOPY);
	DrawCrossHair(&memDC);
}

BOOL CXTColorWnd::OnEraseBkgnd(CDC* pDC)
{
	UNUSED_ALWAYS(pDC);
	return TRUE;
}

void CXTColorWnd::UpdateCursorPos(CPoint point, BOOL bNotify)
{
	CXTColorBase::UpdateCursorPos(point, bNotify);

	CClientDC dc(this);
	CRect rc;
	GetClientRect(&rc);
	CXTMemDC memDC(&dc, rc);

	memDC.BitBlt(0, 0, rc.Size().cx, rc.Size().cy, &m_dcPicker, 0, 0, SRCCOPY);
	DrawCrossHair(&memDC);
}

BOOL CXTColorWnd::PreTranslateMessage(MSG* pMsg)
{
	if (m_eHasFocus == focusColorWheel)
	{
		switch (pMsg->message)
		{
		case WM_KEYDOWN:
			{
				CRect rect;
				GetClientRect(&rect);

				TCHAR vkey = (TCHAR)pMsg->wParam;
				switch (vkey)
				{
				case VK_LEFT:
					m_ptMousePos.x--;
					if (m_ptMousePos.x < 0) {
						m_ptMousePos.x = 0;
					}
					UpdateCursorPos(m_ptMousePos);
					return TRUE;

				case VK_UP:
					m_ptMousePos.y--;
					if (m_ptMousePos.y < 0) {
						m_ptMousePos.y = 0;
					}
					UpdateCursorPos(m_ptMousePos);
					return TRUE;

				case VK_RIGHT:
					m_ptMousePos.x++;
					if (m_ptMousePos.x > rect.right) {
						m_ptMousePos.x = rect.right;
					}
					UpdateCursorPos(m_ptMousePos);
					return TRUE;

				case VK_DOWN:
					m_ptMousePos.y++;
					if (m_ptMousePos.y > rect.bottom) {
						m_ptMousePos.y = rect.bottom;
					}
					UpdateCursorPos(m_ptMousePos);
					return TRUE;
				case VK_RETURN:
				case VK_SPACE:
					CPropertyPage* pParentPage = (CPropertyPage*)GetParent();
					ASSERT_VALID(pParentPage);

					CPropertySheet* pParentSheet = (CPropertySheet*)pParentPage->GetParent();
					ASSERT_VALID(pParentSheet);

					pParentSheet->EndDialog(IDOK);
					break;
				}
			}
			break;
		}
	}

	return CStatic::PreTranslateMessage(pMsg);
}

void CXTColorWnd::DrawCrossHair(CDC* pDC)
{
	int x = m_ptMousePos.x;
	int y = m_ptMousePos.y;

	CPen pen1(PS_SOLID, 1, RGB(0xff,0xff,0xff));
	CPen pen2(PS_SOLID, 1, RGB(0x00,0x00,0x00));

	CPen* pOldPen = pDC->SelectObject((m_eHasFocus == focusColorWheel) ? &pen1 : &pen2);

	pDC->MoveTo(x-5, y-1);
	pDC->LineTo(x-10,y-1);
	pDC->MoveTo(x-5, y);
	pDC->LineTo(x-10,y);
	pDC->MoveTo(x-5, y+1);
	pDC->LineTo(x-10,y+1);

	pDC->MoveTo(x+5, y-1);
	pDC->LineTo(x+10,y-1);
	pDC->MoveTo(x+5, y);
	pDC->LineTo(x+10,y);
	pDC->MoveTo(x+5, y+1);
	pDC->LineTo(x+10,y+1);

	pDC->MoveTo(x-1,y-5);
	pDC->LineTo(x-1,y-10);
	pDC->MoveTo(x,  y-5);
	pDC->LineTo(x,  y-10);
	pDC->MoveTo(x+1,y-5);
	pDC->LineTo(x+1,y-10);

	pDC->MoveTo(x-1,y+5);
	pDC->LineTo(x-1,y+10);
	pDC->MoveTo(x,  y+5);
	pDC->LineTo(x,  y+10);
	pDC->MoveTo(x+1,y+5);
	pDC->LineTo(x+1,y+10);

	pDC->SelectObject(pOldPen);
}

void CXTColorWnd::SetColor(COLORREF clr, bool bUpdate)
{
	CXTColorBase::SetColor(clr, bUpdate);

	if (bUpdate)
	{
		CRect rc;
		GetClientRect(&rc);
		m_ptMousePos.x = (long)((double)rc.Width() * m_nHue);
		m_ptMousePos.y = rc.Height()-(long)((double)rc.Height() * m_nSat);
	}
}

void CXTColorWnd::SetHue(double h)
{
	m_nHue = h;
	CRect rc;
	GetClientRect(&rc);
	m_ptMousePos.x = (long)((double)rc.Width() * h);
	UpdateCursorPos(m_ptMousePos, FALSE);
}

void CXTColorWnd::SetSaturation(double s)
{
	m_nSat = s;
	CRect rc;
	GetClientRect(&rc);
	m_ptMousePos.y = rc.Height()-(long)((double)rc.Height() * s);
	UpdateCursorPos(m_ptMousePos, FALSE);
}

void CXTColorWnd::OnSetFocus(CWnd* pOldWnd)
{
	CXTColorBase::OnSetFocus(pOldWnd);
	m_eHasFocus = focusColorWheel;

	CClientDC dc(this);
	DrawCrossHair(&dc);

	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(10,10);

	if (!rect.PtInRect(m_ptMousePos))
		GetParent()->Invalidate();
}

void CXTColorWnd::OnKillFocus(CWnd* pNewWnd)
{
	CXTColorBase::OnKillFocus(pNewWnd);
	m_eHasFocus = focusNone;

	CClientDC dc(this);
	DrawCrossHair(&dc);

	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(10,10);

	if (!rect.PtInRect(m_ptMousePos))
		GetParent()->Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// CXTColorLum

CXTColorLum::CXTColorLum()
{
	m_nSliderPos = 0;
}

CXTColorLum::~CXTColorLum()
{
}

BEGIN_MESSAGE_MAP(CXTColorLum, CXTColorBase)
	//{{AFX_MSG_MAP(CXTColorLum)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTColorLum message handlers

void CXTColorLum::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);
	CXTMemDC memDC(&dc, rc);

	CXTColorPageCustom* pParent = DYNAMIC_DOWNCAST(CXTColorPageCustom, GetParent());
	if (pParent)
	{
		pParent->DrawThemedBack(&memDC, rc);
	}
	else
	{
		memDC.FillSolidRect(&rc,
			GetXtremeColor(COLOR_3DFACE));
	}

	DrawLuminanceBar(&memDC);
	DrawSliderArrow(&memDC);
}

BOOL CXTColorLum::OnEraseBkgnd(CDC* pDC)
{
	UNUSED_ALWAYS(pDC);
	return TRUE;
}

void CXTColorLum::GetLumBarRect(CRect& rect)
{
	GetClientRect(&rect);
	rect.DeflateRect(0,4);
	rect.right = rect.left + 14;
}

void CXTColorLum::SetColor(COLORREF clr, bool bUpdate)
{
	CXTColorBase::SetColor(clr, bUpdate);

	if (bUpdate)
	{
		CRect rcBar;
		GetLumBarRect(rcBar);

		// Set the slider position based on the current
		// luminance.
		m_ptMousePos.y = m_nSliderPos = rcBar.Height()-(long)((double)rcBar.Height() * m_nLum);
	}
}

void CXTColorLum::SetLuminance(double l)
{
	m_nLum = l;

	CRect rcBar;
	GetLumBarRect(rcBar);

	// Set the slider position based on the current
	// luminance.
	m_ptMousePos.y = m_nSliderPos = rcBar.Height()-(long)((double)rcBar.Height() * l);
	UpdateCursorPos(m_ptMousePos, FALSE);
}

void CXTColorLum::DrawLuminanceBar(CDC *pDC)
{
	CRect rcBar;
	GetLumBarRect(rcBar);

	int  cy = rcBar.Height();
	int  cx = rcBar.Width();

	if (m_dcPicker.GetSafeHdc() != NULL)
		m_dcPicker.DeleteDC();

	// create bitmap
	CBitmap bmpPicker;
	bmpPicker.CreateCompatibleBitmap(pDC, cx, cy);

	// create picker DC
	m_dcPicker.CreateCompatibleDC(pDC);
	m_dcPicker.SelectObject(&bmpPicker);

	// fill color picker bitmap
	int y;
	for (y = rcBar.top; y <= rcBar.bottom; y++)
	{
		COLORREF clr;
		if (y == rcBar.top) {
			clr = RGB(0x00,0x00,0x00);
		}
		else
		if (y == rcBar.bottom) {
			clr = RGB(0xff,0xff,0xff);
		}
		else {
			clr = HLStoRGB(m_nHue, (double)y/(double)cy, m_nSat);
		}

		m_dcPicker.FillSolidRect(rcBar.left, cy-y,
			rcBar.Width(), 1, clr);
	}

	if (XTOSVersionInfo()->IsWin95() || XTOSVersionInfo()->IsWin98())
	{
		m_dcPicker.FillSolidRect(rcBar.left, cy-3,
			rcBar.Width(), 3, RGB(0x00,0x00,0x00));
	}

	pDC->BitBlt(rcBar.left, rcBar.top, cx, cy,
		&m_dcPicker, 0, 0, SRCCOPY);
}

void CXTColorLum::DrawSliderArrow(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	rc.left += 17;

	int x = rc.left;
	int y = m_nSliderPos + 4;

	CRect rcBar;
	GetLumBarRect(rcBar);
	if (y > rcBar.bottom-1)
		y = rcBar.bottom-1;

	CPen pen1(PS_SOLID, 1, RGB(0x00,0x00,0x00));
	CPen pen2(PS_SOLID, 1, GetXtremeColor(COLOR_3DSHADOW));

	CPen* pOldPen = pDC->SelectObject((m_eHasFocus == focusLumination) ? &pen1 : &pen2);

	pDC->MoveTo(x+7, y-4);
	pDC->LineTo(x+9, y-4);
	pDC->MoveTo(x+5, y-3);
	pDC->LineTo(x+9, y-3);
	pDC->MoveTo(x+3, y-2);
	pDC->LineTo(x+9, y-2);
	pDC->MoveTo(x+1, y-1);
	pDC->LineTo(x+9, y-1);
	pDC->MoveTo(x,   y);
	pDC->LineTo(x+9, y);
	pDC->MoveTo(x+2, y+1);
	pDC->LineTo(x+9, y+1);
	pDC->MoveTo(x+4, y+2);
	pDC->LineTo(x+9, y+2);
	pDC->MoveTo(x+6, y+3);
	pDC->LineTo(x+9, y+3);
	pDC->MoveTo(x+8, y+4);
	pDC->LineTo(x+9, y+4);

	if (m_eHasFocus != focusLumination)
	{
		COLORREF clr = RGB(0x00,0x00,0x00);
		pDC->SetPixel(x,   y,   clr);
		pDC->SetPixel(x+1, y,   clr);
		pDC->SetPixel(x+1, y-1, clr);
		pDC->SetPixel(x+2, y-1, clr);
		pDC->SetPixel(x+3, y-2, clr);
		pDC->SetPixel(x+4, y-2, clr);
		pDC->SetPixel(x+5, y-3, clr);
		pDC->SetPixel(x+6, y-3, clr);
		pDC->SetPixel(x+7, y-4, clr);
		pDC->SetPixel(x+8, y-4, clr);
		pDC->SetPixel(x+8, y-3, clr);
		pDC->SetPixel(x+8, y-2, clr);
		pDC->SetPixel(x+8, y-1, clr);
		pDC->SetPixel(x+8, y,   clr);
		pDC->SetPixel(x+8, y+1, clr);
		pDC->SetPixel(x+8, y+2, clr);
		pDC->SetPixel(x+8, y+3, clr);
		pDC->SetPixel(x+8, y+4, clr);
		pDC->SetPixel(x+7, y+3, clr);
		pDC->SetPixel(x+6, y+3, clr);
		pDC->SetPixel(x+5, y+2, clr);
		pDC->SetPixel(x+4, y+2, clr);
		pDC->SetPixel(x+3, y+1, clr);
		pDC->SetPixel(x+2, y+1, clr);
	}

	pDC->SelectObject(pOldPen);
}

void CXTColorLum::UpdateCursorPos(CPoint point, BOOL bNotify)
{
	point.x  = 0;
	COLORREF color = m_dcPicker.GetPixel(point);

	if (color != (COLORREF)-1)
	{
		m_clrColor = color;
		if (bNotify)
		{
			GetParent()->SendMessage(XTWM_UPDATECOLOR,
				(WPARAM)(COLORREF)m_clrColor);
		}

		m_ptMousePos = point;
		m_nSliderPos = point.y;

		Invalidate(FALSE);
	}
}

BOOL CXTColorLum::PreTranslateMessage(MSG* pMsg)
{
	if (m_eHasFocus == focusLumination)
	{
		CRect rcBar;
		GetLumBarRect(rcBar);

		switch (pMsg->message)
		{
		case WM_KEYDOWN:
			{
				TCHAR vkey = (TCHAR)pMsg->wParam;
				switch (vkey)
				{
				case VK_UP:
					m_ptMousePos.y--;
					if (m_ptMousePos.y < /*4*/0) {
						m_ptMousePos.y = /*4*/0;
					}
					UpdateCursorPos(m_ptMousePos);
					return TRUE;

				case VK_DOWN:
					m_ptMousePos.y++;
					if (m_ptMousePos.y > rcBar.bottom) {
						m_ptMousePos.y = rcBar.bottom;
					}
					UpdateCursorPos(m_ptMousePos);
					return TRUE;
				case VK_RETURN:
				case VK_SPACE:
					CPropertyPage* pParentPage = (CPropertyPage*)GetParent();
					ASSERT_VALID(pParentPage);

					CPropertySheet* pParentSheet = (CPropertySheet*)pParentPage->GetParent();
					ASSERT_VALID(pParentSheet);

					pParentSheet->EndDialog(IDOK);
					break;
				}
			}
			break;
		}
	}

	return CStatic::PreTranslateMessage(pMsg);
}

void CXTColorLum::OnSetFocus(CWnd* pOldWnd)
{
	CXTColorBase::OnSetFocus(pOldWnd);
	m_eHasFocus = focusLumination;

	Invalidate(FALSE);
}

void CXTColorLum::OnKillFocus(CWnd* pNewWnd)
{
	CXTColorBase::OnKillFocus(pNewWnd);
	m_eHasFocus = focusNone;

	Invalidate(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CXTColorPageCustom

IMPLEMENT_DYNCREATE(CXTColorPageCustom, CXTThemePropertyPage)

BEGIN_MESSAGE_MAP(CXTColorPageCustom, CXTThemePropertyPage)
	//{{AFX_MSG_MAP(CXTColorPageCustom)
	ON_EN_CHANGE(XT_IDC_EDIT_RED, OnChangeEdit)
	ON_EN_CHANGE(XT_IDC_EDIT_GREEN, OnChangeEdit)
	ON_EN_CHANGE(XT_IDC_EDIT_BLUE, OnChangeEdit)
	ON_EN_CHANGE(XT_IDC_EDIT_LUM, OnChangeEditLum)
	ON_EN_CHANGE(XT_IDC_EDIT_HUE, OnChangeEditHue)
	ON_EN_CHANGE(XT_IDC_EDIT_SAT, OnChangeEditSat)
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTWM_UPDATECOLOR, OnUpdateColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTColorPageCustom construction/destruction

CXTColorPageCustom::CXTColorPageCustom(CXTColorDialog* pParentSheet)
: CXTThemePropertyPage(pParentSheet, CXTColorPageCustom::IDD)
{
	//{{AFX_DATA_INIT(CXTColorPageCustom)
	m_nR = 0;
	m_nB = 0;
	m_nG = 0;
	m_nH = 0;
	m_nL = 0;
	m_nS = 0;
	//}}AFX_DATA_INIT
}

CXTColorPageCustom::~CXTColorPageCustom()
{
}

void CXTColorPageCustom::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTColorPageCustom)
	DDX_Control(pDX, XT_IDC_CLR_WND, m_colorWnd);
	DDX_Control(pDX, XT_IDC_CLR_LUM, m_colorLum);
	DDX_Control(pDX, XT_IDC_TXT_SAT, m_txtSat);
	DDX_Control(pDX, XT_IDC_TXT_RED, m_txtRed);
	DDX_Control(pDX, XT_IDC_TXT_LUM, m_txtLum);
	DDX_Control(pDX, XT_IDC_TXT_HUE, m_txtHue);
	DDX_Control(pDX, XT_IDC_TXT_GREEN, m_txtGreen);
	DDX_Control(pDX, XT_IDC_TXT_BLUE, m_txtBlue);
	DDX_Control(pDX, XT_IDC_SPIN_SAT, m_spinSat);
	DDX_Control(pDX, XT_IDC_SPIN_RED, m_spinRed);
	DDX_Control(pDX, XT_IDC_SPIN_LUM, m_spinLum);
	DDX_Control(pDX, XT_IDC_SPIN_HUE, m_spinHue);
	DDX_Control(pDX, XT_IDC_SPIN_GREEN, m_spinGreen);
	DDX_Control(pDX, XT_IDC_SPIN_BLUE, m_spinBlue);
	DDX_Control(pDX, XT_IDC_EDIT_HUE, m_editHue);
	DDX_Control(pDX, XT_IDC_EDIT_GREEN, m_editGreen);
	DDX_Control(pDX, XT_IDC_EDIT_BLUE, m_editBlue);
	DDX_Control(pDX, XT_IDC_EDIT_LUM, m_editLum);
	DDX_Control(pDX, XT_IDC_EDIT_RED, m_editRed);
	DDX_Control(pDX, XT_IDC_EDIT_SAT, m_editSat);
	DDX_Text(pDX, XT_IDC_EDIT_RED, m_nR);
	DDV_MinMaxInt(pDX, m_nR, 0, 255);
	DDX_Text(pDX, XT_IDC_EDIT_BLUE, m_nB);
	DDV_MinMaxInt(pDX, m_nB, 0, 255);
	DDX_Text(pDX, XT_IDC_EDIT_GREEN, m_nG);
	DDV_MinMaxInt(pDX, m_nG, 0, 255);
	DDX_Text(pDX, XT_IDC_EDIT_HUE, m_nH);
	DDV_MinMaxInt(pDX, m_nH, 0, 255);
	DDX_Text(pDX, XT_IDC_EDIT_LUM, m_nL);
	DDV_MinMaxInt(pDX, m_nL, 0, 255);
	DDX_Text(pDX, XT_IDC_EDIT_SAT, m_nS);
	DDV_MinMaxInt(pDX, m_nS, 0, 255);
	//}}AFX_DATA_MAP
}

BOOL CXTColorPageCustom::OnInitDialog()
{
	CXTThemePropertyPage::OnInitDialog();

	// TODO: Add extra initialization here
	m_spinSat.SetBuddy(&m_editSat);
	m_spinSat.SetRange(0, 255);

	m_spinLum.SetBuddy(&m_editLum);
	m_spinLum.SetRange(0, 255);

	m_spinHue.SetBuddy(&m_editHue);
	m_spinHue.SetRange(0, 255);

	m_spinRed.SetBuddy(&m_editRed);
	m_spinRed.SetRange(0, 255);

	m_spinGreen.SetBuddy(&m_editGreen);
	m_spinGreen.SetRange(0, 255);

	m_spinBlue.SetBuddy(&m_editBlue);
	m_spinBlue.SetRange(0, 255);

	// initialize edit controls.
	m_editHue.Initialize(this);
	m_editGreen.Initialize(this);
	m_editBlue.Initialize(this);
	m_editLum.Initialize(this);
	m_editRed.Initialize(this);
	m_editSat.Initialize(this);

	// open Windows XP theme data for property page.
	if (!m_themeWrapper.ThemeDataOpen())
		m_themeWrapper.OpenThemeData(m_hWnd, L"TAB");

	// draw the following items transparent.
	MakeTextTransparent(XT_IDC_TXT_COLORS);
	MakeTextTransparent(XT_IDC_TXT_HUE);
	MakeTextTransparent(XT_IDC_TXT_SAT);
	MakeTextTransparent(XT_IDC_TXT_LUM);
	MakeTextTransparent(XT_IDC_TXT_RED);
	MakeTextTransparent(XT_IDC_TXT_GREEN);
	MakeTextTransparent(XT_IDC_TXT_BLUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTColorPageCustom::OnDrawPage(CDC* pDC)
{
	CXTThemePropertyPage::OnDrawPage(pDC);
}

BOOL CXTColorPageCustom::OnSetActive()
{
	COLORREF clr = m_pParentSheet->GetColor();
	m_colorWnd.SetColor(clr);
	m_colorLum.SetColor(clr);

	m_nR = GetRValue(clr);
	m_nG = GetGValue(clr);
	m_nB = GetBValue(clr);

	RGBtoHSL(clr, &m_nL, &m_nS, &m_nH);

	UpdateData(FALSE);

	return CPropertyPage::OnSetActive();
}

void CXTColorPageCustom::OnChangeEdit()
{
	UpdateData();
	COLORREF clr = (COLORREF)RGB(m_nR, m_nG, m_nB);

	if (clr != m_colorWnd.GetColor())
		m_colorWnd.SetColor(clr);

	OnUpdateColor((WPARAM)clr, 0);
}

void CXTColorPageCustom::OnChangeEditLum()
{
	UpdateData();
	m_colorLum.SetLuminance((double)m_nL/255);

	COLORREF clr = m_colorLum.GetColor();

	if (clr != m_colorWnd.GetColor())
		m_colorWnd.SetColor(clr, false);

	if (clr != m_pParentSheet->GetColor())
		m_pParentSheet->SetNewColor(clr, FALSE);

	m_nR = GetRValue(clr);
	m_nG = GetGValue(clr);
	m_nB = GetBValue(clr);

	UpdateData(FALSE);
}

void CXTColorPageCustom::OnChangeEditHue()
{
	UpdateData();
	m_colorWnd.SetHue((double)m_nH/255);

	COLORREF clr = m_colorWnd.GetColor();

	if (clr != m_colorLum.GetColor())
		m_colorLum.SetColor(clr, false);

	if (clr != m_pParentSheet->GetColor())
		m_pParentSheet->SetNewColor(clr, FALSE);

	m_nR = GetRValue(clr);
	m_nG = GetGValue(clr);
	m_nB = GetBValue(clr);

	UpdateData(FALSE);
}

void CXTColorPageCustom::OnChangeEditSat()
{
	UpdateData();
	m_colorWnd.SetSaturation((double)m_nS/255);

	COLORREF clr = m_colorWnd.GetColor();

	if (clr != m_colorLum.GetColor())
		m_colorLum.SetColor(clr, false);

	if (clr != m_pParentSheet->GetColor())
		m_pParentSheet->SetNewColor(clr, FALSE);

	m_nR = GetRValue(clr);
	m_nG = GetGValue(clr);
	m_nB = GetBValue(clr);

	UpdateData(FALSE);
}

LRESULT CXTColorPageCustom::OnUpdateColor(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(lParam);

	COLORREF clr = (COLORREF)wParam;
	if (clr != m_colorWnd.GetColor())
		m_colorWnd.SetColor(clr, false);

	if (clr != m_colorLum.GetColor())
		m_colorLum.SetColor(clr, true);

	if (clr != m_pParentSheet->GetColor())
		m_pParentSheet->SetNewColor(clr, FALSE);

	m_nR = GetRValue(clr);
	m_nG = GetGValue(clr);
	m_nB = GetBValue(clr);

	RGBtoHSL(clr, &m_nL, &m_nS, &m_nH);
	UpdateData(FALSE);

	return 0;
}

void CXTColorPageCustom::RGBtoHSL(COLORREF color, int* lum, int* sat, int* hue)
{
	double r = (double)GetRValue(color)/255;
	double g = (double)GetGValue(color)/255;
	double b = (double)GetBValue(color)/255;

	double maxcolor = __max(r, __max(g, b));
	double mincolor = __min(r, __min(g, b));

	double l = (maxcolor + mincolor)/2;

	if (maxcolor == mincolor)
	{
		*hue = 0;
		*sat = 0;
	}
	else
	{
		double s;
		double h;

		if (l < 0.5)
			s = (maxcolor-mincolor)/(maxcolor+mincolor);
		else
			s = (maxcolor-mincolor)/(2.0-maxcolor-mincolor);

		if (r == maxcolor)
			h = (g-b)/(maxcolor-mincolor);
		else if (g == maxcolor)
			h = 2.0+(b-r)/(maxcolor-mincolor);
		else
			h = 4.0+(r-g)/(maxcolor-mincolor);

		h /= 6.0;
		if(h < 0.0)
			h += 1;

		*hue = (int)((double)h * 255);
		*sat = (int)((double)s * 255);
	}

	*lum = (int)((double)l * 255);
}
