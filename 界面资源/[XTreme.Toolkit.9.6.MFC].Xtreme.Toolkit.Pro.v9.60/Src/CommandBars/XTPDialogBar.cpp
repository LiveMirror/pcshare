// XTPDialogBar.cpp : implementation of the CXTPDialogBar class.
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

#include "XTPCommandBarsResource.h"
#include "XTPDialogBar.h"
#include "XTPControls.h"
#include "XTPControl.h"
#include "XTPPaintManager.h"
#include "XTPMouseManager.h"
#include "XTPControlPopup.h"
#include "XTPControlButton.h"
#include "XTPPopupBar.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_XTP_COMMANDBAR(CXTPDialogBar, CXTPToolBar)


class CXTPDialogBar::CXTPControlButtonHide: public CXTPControlButton
{
public:
	CXTPControlButtonHide()
	{
		m_dwFlags = xtpFlagManualUpdate | xtpFlagSkipFocus | xtpFlagNoMovable;
	}
	void Draw(CDC* pDC);
	void SetBeginGroup(BOOL /*bBeginGroup*/) {}
	void OnExecute()
	{
		ASSERT(m_pParent);
		((CXTPToolBar*)m_pParent)->SetVisible(FALSE);
	}
};

void CXTPDialogBar::CXTPControlButtonHide::Draw(CDC* pDC)
{
	CFont font;
	font.CreatePointFont(MulDiv(80, 96, pDC->GetDeviceCaps(LOGPIXELSX)), _T("Marlett"));
	CFont* pFont = pDC->SelectObject(&font);
	GetPaintManager()->DrawControlEntry(pDC, this);
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(_T("r"), m_rcControl, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pDC->SelectObject(pFont);
}

class CXTPDialogBar::CXTPControlCaptionPopup: public CXTPControlPopup
{
public:
	CXTPControlCaptionPopup()
	{
		m_dwFlags = xtpFlagLeftPopup | xtpFlagManualUpdate | xtpFlagSkipFocus | xtpFlagNoMovable;
	}
	void Draw(CDC* pDC);

	void AdjustExcludeRect(CRect& rc)
	{
		GetPaintManager()->AdjustExcludeRect(rc, this, FALSE);
	}
	BOOL OnSetPopup(BOOL bPopup)
	{
		if (m_bCaptionPopup)
			return CXTPControlPopup::OnSetPopup(bPopup);
		return TRUE;
	}

public:
	BOOL m_bCaptionPopup;
};

void CXTPDialogBar::CXTPControlCaptionPopup::Draw(CDC* pDC)
{
	CXTPPaintManager* pPaintManager = GetPaintManager();
	CXTPFontDC font(pDC, pPaintManager->GetSmCaptionFont());

	CRect rcButton = GetRect();
	BOOL bPressed = GetPressed(), bSelected = GetSelected(), bChecked = GetChecked(),
		bPopuped = GetPopup();

	pDC->SetBkMode(TRANSPARENT);

	if (m_bCaptionPopup)
	{
		pPaintManager->DrawControlEntry(pDC, rcButton, TRUE, bSelected, bPressed, bChecked, bPopuped, xtpBarTop);
	} else
	{
		pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
	}

	CRect rcText(rcButton);
	rcText.DeflateRect(2, 0, m_bCaptionPopup? 15: 2, 0);
	pDC->DrawText(GetCaption(), rcText, DT_SINGLELINE | DT_VCENTER);

	COLORREF clrText = pDC->GetTextColor();

	if (m_bCaptionPopup)
	{
		CPoint pt = CPoint(rcButton.right - 9, rcButton.top + rcButton.Height()/2);
		pPaintManager->Triangle(pDC, CPoint(pt.x -4 , pt.y - 2), CPoint(pt.x + 4, pt.y - 2), CPoint (pt.x, pt.y + 2), clrText);
	}
}

class CXTPDialogBar::CXTPCaptionPopupBar: public CXTPPopupBar
{
public:
	CXTPCaptionPopupBar()
	{
		m_popupFlags = xtpPopupDown;
	}
	void UpdateFlags()
	{
		CXTPPopupBar::UpdateFlags();
		m_popupFlags = xtpPopupDown|xtpPopupLeft;

		m_ptPopup.x = (m_popupFlags & xtpPopupDown)? m_rcExclude.left: m_rcExclude.right;
		m_ptPopup.y = (m_popupFlags & xtpPopupDown)? m_rcExclude.bottom: m_rcExclude.top;


	}
};

/////////////////////////////////////////////////////////////////////////////
// CXTPDialogBar

CXTPDialogBar::CXTPDialogBar()
{
	m_rcBorders = CRect(3, 3, 3, 3);
	m_bResizable = TRUE;
	m_bCaptionVisible = TRUE;

	m_szDockingClient = m_szFloatingClient = CSize(200, 200);
	m_szMinTrackRect = CSize(50, 50);
	m_bCaptionPopup = TRUE;

	m_hwndChild = 0;

	m_pPopupBar = new CXTPCaptionPopupBar();
	m_nCaptionHeight = 0;

	m_bCustomizable = FALSE;

	EnableAutomation();
}

CXTPDialogBar::~CXTPDialogBar()
{
	m_pPopupBar->InternalRelease();

}


BEGIN_MESSAGE_MAP(CXTPDialogBar, CXTPToolBar)
	//{{AFX_MSG_MAP(CXTPDialogBar)
	ON_WM_NCHITTEST()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPDialogBar message handlers

CSize CXTPDialogBar::CalcDockingLayout(int nLength, DWORD dwMode, int /*nWidth*/)
{
	return CalcSize(nLength, dwMode, m_szDockingClient);
}


CRect CXTPDialogBar::GetMargins()
{
	CRect rcBorder = m_rcBorders;

	CClientDC dc(this);
	CXTPFontDC font(&dc, GetPaintManager()->GetCommandBarFont(this));
	CSize szGripper = GetPaintManager()->DrawCommandBarGripper(&dc, this, FALSE);
	int nTitleSize = szGripper.cy;

	rcBorder.top += nTitleSize;
	return rcBorder;
}

CSize CXTPDialogBar::CalcSize(int nLength, DWORD dwMode, CSize szClient)
{
	CRect rcBorder = m_rcBorders;
	CSize sizeResult = szClient;

	sizeResult.cx += rcBorder.left + rcBorder.right;
	sizeResult.cy += rcBorder.top + rcBorder.bottom;

	if (!m_bCaptionVisible)
	{
		return sizeResult;
	}

	CClientDC dc(this);
	CXTPFontDC font(&dc, GetPaintManager()->GetCommandBarFont(this));
	CSize szGripper = GetPaintManager()->DrawCommandBarGripper(&dc, this, FALSE);
	int nTitleSize = szGripper.cy;
	m_nCaptionHeight = szGripper.cy + rcBorder.top;

	sizeResult.cy += nTitleSize;



	if (dwMode & LM_STRETCH)
	{
		if (dwMode & LM_VERTDOCK) sizeResult.cy = max(nLength, sizeResult.cy); else sizeResult.cx = max(nLength, sizeResult.cx);
	}
	if (dwMode & LM_HORZ) sizeResult.cx = min(nLength, sizeResult.cx); else sizeResult.cy = min(nLength, sizeResult.cy);


	int nRightBorder = sizeResult.cx - rcBorder.right;

	CXTPControl* pHide = m_pControls->FindControl(XTP_ID_TOOLBAR_HIDE);
	if (!pHide && IsCloseable())
	{
		pHide = (CXTPControlPopup*)m_pControls->Add(new CXTPControlButtonHide(), XTP_ID_TOOLBAR_HIDE, _T(""), -1, TRUE);
	}
	if (pHide && !IsCloseable())
	{
		pHide->SetHideFlags(xtpHideGeneric);
	}
	else if (pHide)
	{
		pHide->SetRect(CRect(CPoint(nRightBorder - 16, m_rcBorders.top + nTitleSize/2 - 8), CSize(16, 16)));
		nRightBorder -= 16;
	}

	CXTPControlCaptionPopup* pCaption = (CXTPControlCaptionPopup*)m_pControls->FindControl(XTP_ID_DIALOGBAR_CAPTION);
	if (!pCaption)
	{
		pCaption = (CXTPControlCaptionPopup*)m_pControls->Add(new CXTPControlCaptionPopup(), XTP_ID_DIALOGBAR_CAPTION, _T(""), -1, TRUE);
		pCaption->SetCommandBar(m_pPopupBar);
	}
	pCaption->m_bCaptionPopup = m_bCaptionPopup && (m_pPopupBar->GetControls()->GetCount() != 0);

	int nCaptionHeight = 16;
	int nCaptionTop = m_rcBorders.top + nTitleSize/2 - nCaptionHeight / 2;
	pCaption->SetRect(CRect(m_rcBorders.left + szGripper.cx, nCaptionTop, nRightBorder, nCaptionTop + nCaptionHeight));
	pCaption->SetCaption(m_strCaption.IsEmpty()? GetTitle(): m_strCaption);


	return sizeResult;
}

CSize CXTPDialogBar::CalcDynamicLayout(int /*nLength*/, DWORD dwMode)
{
	return CalcSize(32000, dwMode, m_szFloatingClient);
}


UINT CXTPDialogBar::OnNcHitTest(CPoint point)
{
	if (!m_bResizable)
		return CXTPCommandBar::OnNcHitTest(point);

	CRect rectWindow;
	GetWindowRect(&rectWindow);
	rectWindow.InflateRect(-3, -3);

	if (m_barPosition != xtpBarFloating)
	{
		if (m_barPosition == xtpBarBottom)
		{
			if (point.y <= rectWindow.top) return HTTOP;
		}
		else
		{
			if ((point.y >= rectWindow.bottom) && (((m_dwFlags & xtpFlagStretched) == 0) || !IsVerticalPosition(GetPosition()))) return HTBOTTOM;
		}

		if (m_barPosition == xtpBarRight)
		{
			if (point.x <= rectWindow.left) return HTLEFT;
		}
		else
		{
			if ((point.x >= rectWindow.right) && (((m_dwFlags & xtpFlagStretched) == 0) || IsVerticalPosition(GetPosition()))) return HTRIGHT;
		}
	}
	else
	{
		if ((point.y < rectWindow.top) && (point.x < rectWindow.left)) return HTTOPLEFT;
		if ((point.y < rectWindow.top) && (point.x >= rectWindow.right)) return HTTOPRIGHT;
		if ((point.y >= rectWindow.bottom) && (point.x < rectWindow.left)) return HTBOTTOMLEFT;
		if ((point.y >= rectWindow.bottom) && (point.x >= rectWindow.right)) return HTBOTTOMRIGHT;
		if (point.y < rectWindow.top) return HTTOP;
		if (point.y >= rectWindow.bottom) return HTBOTTOM;
		if (point.x < rectWindow.left) return HTLEFT;
		if (point.x >= rectWindow.right) return HTRIGHT;
	}

	return CXTPCommandBar::OnNcHitTest(point);
}


void CXTPDialogBar::OnResize(CRect rcWindow, int nHitTest)
{
	CRect rcMargins = GetMargins();

	if (rcWindow.Width() < m_szMinTrackRect.cx || rcWindow.Height() < m_szMinTrackRect.cy)
		return;

	if (GetPosition() == xtpBarFloating)
	{
		m_szFloatingClient = CSize(rcWindow.Width() - rcMargins.left - rcMargins.right, rcWindow.Height() - rcMargins.top - rcMargins.bottom);

		CalcDynamicLayout(0, 0);
		MoveWindow(rcWindow);
		Invalidate(FALSE);
	}
	else
	{
		CSize szDockingClient = CSize(rcWindow.Width() - rcMargins.left - rcMargins.right, rcWindow.Height() - rcMargins.top - rcMargins.bottom);

		if (nHitTest == HTRIGHT || nHitTest == HTLEFT)
			m_szDockingClient.cx = szDockingClient.cx; else m_szDockingClient.cy = szDockingClient.cy;

		OnRecalcLayout();
	}
}

int CXTPDialogBar::OnMouseHitTest(CPoint pt)
{
	CXTPWindowRect rc(this);
	rc.bottom = rc.top + m_nCaptionHeight;

	return rc.PtInRect(pt)? HTCLIENT: HTERROR;
}

void CXTPDialogBar::MoveChildWindow(CRect rc)
{
	if (m_hwndChild)
	{
		CRect rcMargins = GetMargins();
		CRect rcChild(rcMargins.left, rcMargins.top, rc.Width() - rcMargins.right, rc.Height() - rcMargins.bottom);

		::SetWindowPos(m_hwndChild, 0, rcChild.left, rcChild.top, rcChild.Width(), rcChild.Height(), SWP_NOZORDER|SWP_NOACTIVATE);
		::InvalidateRect(m_hwndChild, 0, FALSE);
	}
}

void CXTPDialogBar::OnSize(UINT nType, int cx, int cy)
{
	CXTPToolBar::OnSize(nType, cx, cy);

	MoveChildWindow(CRect(0, 0, cx, cy));
}

void CXTPDialogBar::SetChild(CWnd* pChildWnd)
{
	m_hwndChild = pChildWnd->m_hWnd;

	MoveChildWindow(CXTPClientRect(this));
	DelayLayout();
}

void CXTPDialogBar::SetCaption(LPCTSTR strCaption)
{
	m_strCaption = strCaption;
	DelayLayout();
}

void CXTPDialogBar::SetCaptionPopup(int nMenuResourceID)
{
	CMenu menu;
	VERIFY(menu.LoadMenu(nMenuResourceID));
	m_pPopupBar->LoadMenu(menu.GetSubMenu(0));
}

void CXTPDialogBar::SetSize(CSize sz)
{
	m_szDockingClient = m_szFloatingClient = sz;
	DelayLayout();
}

CString CXTPDialogBar::GetCaption()
{
	return m_strCaption;
}

void CXTPDialogBar::SetCaptionVisible(BOOL bVisible)
{
	m_bCaptionVisible = bVisible;
	DelayLayout();
}



