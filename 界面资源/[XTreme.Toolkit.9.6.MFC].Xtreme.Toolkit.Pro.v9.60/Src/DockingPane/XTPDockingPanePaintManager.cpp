// XTPDockingPanePaintManager.cpp : implementation of the CXTPDockingPanePaintManager class.
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
#include "Tmschema.h"
#include "XTPVC80Helpers.h"
#include "XTPColorManager.h"
#include "XTPDrawHelpers.h"
#include "XTPTabManager.h"
#include "XTPTabPaintManager.h"

#include "XTPDockingPanePaintManager.h"
#include "XTPDockingPaneManager.h"
#include "XTPDockingPaneTabbedContainer.h"
#include "XTPDockingPaneAutoHidePanel.h"
#include "XTPDockingPane.h"
#include "XTPDockingPaneResource.h"
#include "XTPDockingPaneMiniWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL CXTPDockingPaneCaptionButton::IsVisible()
{
	return m_pPane->IsCaptionButtonVisible(this);
}



void AFX_CDECL CXTPDockingPaneCaptionButton::DrawPinnButton(CDC* pDC, CPoint pt, BOOL bPinned)
{
	if (bPinned)
	{
		pDC->MoveTo(pt.x - 4, pt.y); pDC->LineTo(pt.x - 1, pt.y);
		pDC->MoveTo(pt.x - 1, pt.y - 3); pDC->LineTo(pt.x - 1, pt.y + 4);
		pDC->MoveTo(pt.x - 1, pt.y + 2); pDC->LineTo(pt.x + 3, pt.y + 2);
		pDC->LineTo(pt.x + 3, pt.y - 2); pDC->LineTo(pt.x - 1, pt.y - 2);
		pDC->MoveTo(pt.x - 1, pt.y + 1); pDC->LineTo(pt.x + 3, pt.y + 1);
	}
	else
	{
		pt.y +=2;
		pDC->MoveTo(pt.x - 3, pt.y - 1); pDC->LineTo(pt.x + 4, pt.y - 1);
		pDC->MoveTo(pt.x, pt.y - 1); pDC->LineTo(pt.x, pt.y + 3);
		pDC->MoveTo(pt.x - 2, pt.y - 1); pDC->LineTo(pt.x - 2, pt.y - 6);
		pDC->LineTo(pt.x + 2, pt.y - 6); pDC->LineTo(pt.x + 2, pt.y - 1);
		pDC->MoveTo(pt.x + 1, pt.y - 1); pDC->LineTo(pt.x + 1, pt.y - 6);
	}
}

void CXTPDockingPaneCaptionButton::InvalidateRect()
{
	m_pPane->RedrawPane();
}

DWORD CXTPDockingPaneCaptionButton::Click (CWnd* pWnd, CPoint pt, BOOL bClient)
{
	if ( (::GetCapture() != NULL ))
		return FALSE;

	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());

	BOOL  bClick = FALSE;
	CRect rcBtn(m_rcButton);

	while ( TRUE )
	{

		BOOL bNewDown = ::PtInRect (rcBtn, pt) != 0;
		if ( bNewDown != m_bPressed)
		{
			m_bPressed = bNewDown;

			InvalidateRect();
		}
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if ( CWnd::GetCapture() != pWnd )
		{
			DispatchMessage (&msg);
			goto ExitLoop;
		}
		switch ( msg.message )
		{
		case WM_NCMOUSEMOVE:
			break;
		case WM_MOUSEMOVE:
			pt = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));
			if (!bClient)
			{
				pWnd->ClientToScreen (&pt);
				pt -= CXTPWindowRect(pWnd).TopLeft();
			}
			break;

		case WM_LBUTTONUP:
			bClick = m_bPressed;
			goto ExitLoop;

		case WM_KEYDOWN:
			if ( msg.wParam != VK_ESCAPE )
				break;
		case WM_CANCELMODE:
		case WM_RBUTTONDOWN:
			goto ExitLoop;

		default:
			DispatchMessage (&msg);
			break;
		}
	}
ExitLoop:
	ReleaseCapture();

	m_bPressed = FALSE;
	CheckForMouseOver (pt);

	InvalidateRect();

	return bClick;
}

void CXTPDockingPaneCaptionButton::SetRect (CRect rc)
{
	ASSERT(m_pPane);
	if (rc != m_rcButton)
	{
		m_rcButton = rc;
		m_pPane->OnCaptionButtonRectChanged(this);
	}
}

BOOL CXTPDockingPaneCaptionButton::CheckForMouseOver (CPoint pt)
{
	if (!PtInRect (pt) && m_bSelected)
	{
		m_bSelected = FALSE;
		InvalidateRect();
	}

	if (!m_bSelected && PtInRect (pt) )
	{
		TRACKMOUSEEVENT tme = { sizeof (TRACKMOUSEEVENT), TME_LEAVE, m_pPane->GetSafeHwnd()};
		_TrackMouseEvent(&tme);

		m_bSelected = TRUE;

		InvalidateRect();
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPanePaintManager


CXTPDockingPanePaintManager::CXTPDockingPanePaintManager(void)
: m_bUseOfficeFont(false)
, m_strOfficeFont(_T("Tahoma"))
{
	m_pTabPaintManager = new CXTPTabPaintManager();
	m_pTabPaintManager->SetLayout(xtpTabLayoutSizeToFit);
	m_pTabPaintManager->SetPosition(xtpTabPositionBottom);
	m_pTabPaintManager->m_clientFrame = xtpTabFrameSingleLine;
	m_pTabPaintManager->m_bBoldSelected = FALSE;

	m_pPanelPaintManager = new CXTPTabPaintManager();
	m_pPanelPaintManager->SetLayout(xtpTabLayoutCompressed);
	m_pPanelPaintManager->m_clientFrame = xtpTabFrameNone;
	m_pPanelPaintManager->m_bBoldSelected = FALSE;
	m_pPanelPaintManager->m_bInvertGradient = TRUE;
	m_pPanelPaintManager->m_bFillBackground = FALSE;

	m_bDrawSingleTab = FALSE;
	m_nSplitterSize = 4;

	m_rcCaptionMargin.SetRect(0, 2, 0, 1);
	m_nCaptionFontGap = 5;

	m_bShowCaption = TRUE;
	m_bUseStandardFont = TRUE;

	m_bHighlighActiveCaption = TRUE;
	m_systemTheme = xtpSystemThemeDefault;
	m_bUseBoldCaptionFont = TRUE;

	RefreshMetrics();

}

XTPCurrentSystemTheme CXTPDockingPanePaintManager::GetCurrentSystemTheme()
{
	if (m_systemTheme == xtpSystemThemeDefault)
		return XTPColorManager()->IsLunaColorsDisabled()? xtpSystemThemeUnknown:
			XTPColorManager()->GetCurrentSystemTheme();

	if (m_systemTheme == xtpSystemThemeAuto)
		return XTPColorManager()->GetWinThemeWrapperTheme();

	return m_systemTheme;
}

void CXTPDockingPanePaintManager::SetLunaTheme(XTPCurrentSystemTheme systemTheme)
{
	m_systemTheme = systemTheme;
	RefreshMetrics();

}

COLORREF CXTPDockingPanePaintManager::GetXtremeColor(UINT nIndex)
{
	if (nIndex > XPCOLOR_LAST) return nIndex;
	return m_arrColor[nIndex];
}

void CXTPDockingPanePaintManager::SetColors(int cElements, CONST INT *lpaElements, CONST COLORREF *lpaRgbValues)
{
	for (int i = 0; i < cElements; i++)
		m_arrColor[lpaElements[i]] = lpaRgbValues[i];
}



CXTPDockingPanePaintManager::~CXTPDockingPanePaintManager(void)
{
	m_pTabPaintManager->InternalRelease();
	m_pPanelPaintManager->InternalRelease();
}

CXTPTabPaintManager* CXTPDockingPanePaintManager::SetTabPaintManager(CXTPTabPaintManager* pManager)
{
	m_pTabPaintManager->InternalRelease();
	m_pTabPaintManager = pManager;

	m_pTabPaintManager->SetLayout(xtpTabLayoutSizeToFit);
	m_pTabPaintManager->SetPosition(xtpTabPositionBottom);
	m_pTabPaintManager->m_bBoldSelected = FALSE;

	pManager->RefreshMetrics();
	return pManager;
}

CXTPTabPaintManager* CXTPDockingPanePaintManager::SetPanelPaintManager(CXTPTabPaintManager* pManager)
{
	delete m_pPanelPaintManager;
	m_pPanelPaintManager = pManager;

	m_pPanelPaintManager->SetLayout(xtpTabLayoutCompressed);
	m_pPanelPaintManager->m_clientFrame = xtpTabFrameNone;
	m_pPanelPaintManager->m_bBoldSelected = FALSE;
	m_pPanelPaintManager->m_bInvertGradient = TRUE;
	m_pPanelPaintManager->m_bFillBackground = FALSE;


	pManager->RefreshMetrics();
	return pManager;
}


void CXTPDockingPanePaintManager::UseOfficeFont(bool bUseOfficeFont)
{
	m_bUseOfficeFont = bUseOfficeFont;
	RefreshMetrics();
}

void CXTPDockingPanePaintManager::SetCaptionFontIndirect(LOGFONT* pLogFont, BOOL bUseStandardFont /*= FALSE*/)
{
	m_bUseStandardFont = bUseStandardFont;

	if (!pLogFont)
		return;

	m_fntTitle.DeleteObject();
	VERIFY(m_fntTitle.CreateFontIndirect(pLogFont));

	CWindowDC dc(CWnd::GetDesktopWindow());
	CXTPFontDC font(&dc, &m_fntTitle);
	m_nTitleHeight = max(13, dc.GetTextExtent(_T(" ")).cy) + m_nCaptionFontGap;

}

void CXTPDockingPanePaintManager::RefreshMetrics()
{

	CXTPColorManager* pColorManager = XTPColorManager();
	pColorManager->RefreshColors();

	for (int i = 0; i <= XPCOLOR_LAST; i++)
		m_arrColor[i] = pColorManager->GetColor(i);

	if (m_bUseStandardFont)
	{
		NONCLIENTMETRICS ncm = { sizeof(NONCLIENTMETRICS) };
		VERIFY(::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));
		if (!m_bUseBoldCaptionFont)
			ncm.lfSmCaptionFont.lfWeight = FW_NORMAL;

		SetCaptionFontIndirect(&ncm.lfSmCaptionFont, TRUE);
	}

	m_pTabPaintManager->RefreshMetrics();
	m_pPanelPaintManager->RefreshMetrics();

	CWindowDC dc(CWnd::GetDesktopWindow());
	CXTPFontDC font(&dc, m_pTabPaintManager->GetFont(FALSE));
	m_nTabsHeight = max(24, dc.GetTextExtent(_T(" ")).cy + 10);
}

void CXTPDockingPanePaintManager::Line(CDC* pDC, int x0, int y0, int x1, int y1, int nPen)
{
	CXTPPenDC pen(*pDC, GetXtremeColor(nPen));
	pDC->MoveTo(x0, y0);
	pDC->LineTo(x1, y1);
}

void CXTPDockingPanePaintManager::Line(CDC* pDC, int x0, int y0, int x1, int y1)
{
	pDC->MoveTo(x0, y0);
	pDC->LineTo(x1, y1);
}


void CXTPDockingPanePaintManager::Rectangle(CDC* pDC, CRect rc, int nBrush, int nPen)
{
	pDC->FillSolidRect(rc, GetXtremeColor(nBrush));
	pDC->Draw3dRect(rc, GetXtremeColor(nPen), GetXtremeColor(nPen));
}

void CXTPDockingPanePaintManager::Pixel(CDC* pDC, int x, int y, int nPen)
{
	pDC->SetPixel(x, y, GetXtremeColor(nPen));
}


void CXTPDockingPanePaintManager::DrawCaptionButtons(CDC* pDC, CXTPDockingPaneCaptionButtons* pButtons, CRect& rcCaption, COLORREF clr, int nButtonSize, int nButtonGap)
{
	CRect rcButton(CPoint(rcCaption.right - nButtonSize , rcCaption.CenterPoint().y - nButtonSize / 2), CSize(nButtonSize, nButtonSize));

	for (int i = 0; i < pButtons->GetSize(); i++)
	{
		CXTPDockingPaneCaptionButton* pButton = pButtons->GetAt(i);

		if (pButton->IsVisible())
		{
			pButton->SetRect(rcButton);
			DrawCaptionButton(pDC, pButton, clr);

			rcButton.OffsetRect(- nButtonSize - nButtonGap, 0);
		} else
		{
			pButton->SetRect(CXTPEmptyRect());
		}
	}
	rcCaption.right = rcButton.right;
}


void CXTPDockingPanePaintManager::DrawPane(CDC& dc, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	dc.FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));

	if (pPane->IsTitleVisible() && m_bShowCaption)
	{
		dc.SetBkMode(TRANSPARENT);
		DrawCaption(dc, pPane, rc);
	}

	AdjustCaptionRect(pPane, rc);

	if (pPane->IsTabsVisible())
	{
		m_pTabPaintManager->DrawTabControl(pPane, &dc, rc);
	}
}


void CXTPDockingPanePaintManager::AdjustCaptionRect(CXTPDockingPaneTabbedContainer* pPane, CRect& rc)
{
	if (pPane->IsTitleVisible() && m_bShowCaption)
	{
		rc.top += m_nTitleHeight + m_rcCaptionMargin.top + m_rcCaptionMargin.bottom;
	}
}

void CXTPDockingPanePaintManager::AdjustClientRect(CXTPDockingPaneTabbedContainer* pPane, CRect& rect, BOOL bApply)
{
	CPoint pt = rect.TopLeft();
	AdjustCaptionRect(pPane, rect);


	if (pPane->IsTabsVisible())
	{
		if (bApply)
		{
			CClientDC dc(pPane);

			CRect rcTabs(rect);
			rcTabs.OffsetRect(-pt);

			m_pTabPaintManager->RepositionTabControl(pPane, &dc, rcTabs);
		}
		m_pTabPaintManager->AdjustClientRect(pPane, rect);
	}
}

void CXTPDockingPanePaintManager::DrawSplitter(CDC& dc, CXTPDockingPaneSplitterWnd* pSplitter)
{
	dc.FillSolidRect(CXTPClientRect((CWnd*)pSplitter), GetXtremeColor(XPCOLOR_3DFACE));
}


///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneOfficeTheme

CXTPDockingPaneOfficeTheme::CXTPDockingPaneOfficeTheme()
{
	m_pPanelPaintManager->SetAppearance(xtpTabAppearanceFlat);
	m_pTabPaintManager->SetAppearance(xtpTabAppearanceVisualStudio);
	m_bGradientCaption = FALSE;	
}

void CXTPDockingPaneOfficeTheme::RefreshMetrics()
{
	CXTPDockingPaneGripperedTheme::RefreshMetrics();

	m_clrActiveCaption.SetStandardValue(GetXtremeColor(COLOR_ACTIVECAPTION), GetXtremeColor(27));
	m_clrInactiveCaption.SetStandardValue(GetXtremeColor(COLOR_INACTIVECAPTION), GetSysColor(28));
	m_clrInactiveCaptionText = GetXtremeColor(COLOR_INACTIVECAPTIONTEXT);

	OSVERSIONINFO osvi = { sizeof(OSVERSIONINFO) };
	GetVersionEx(&osvi);

	if ((osvi.dwMajorVersion < 4) || ((osvi.dwMajorVersion == 4) && (osvi.dwMinorVersion == 0)))
	{
		// Windows 95
		m_clrInactiveCaption.SetStandardValue(GetXtremeColor(COLOR_INACTIVECAPTION));
		m_clrActiveCaption.SetStandardValue(GetXtremeColor(COLOR_ACTIVECAPTION));
	}
}


void CXTPDockingPaneOfficeTheme::DrawCaptionPart(CDC& dc, CXTPDockingPaneBase* pPane, CRect rcCaption, CString strTitle, BOOL bActive)
{
	COLORREF clrButton = GetXtremeColor(bActive? COLOR_HIGHLIGHTTEXT: COLOR_BTNTEXT), clrText;

	if (m_bGradientCaption)
	{
		XTPDrawHelpers()->GradientFill(&dc, rcCaption, bActive? m_clrActiveCaption: m_clrInactiveCaption, TRUE);
		clrText = bActive? GetXtremeColor(COLOR_HIGHLIGHTTEXT): m_clrInactiveCaptionText;
	}
	else
	{	
		if (bActive)
		{
			dc.FillSolidRect(rcCaption, GetXtremeColor(COLOR_ACTIVECAPTION));
		} else
		{
			CXTPPenDC pen(dc, GetXtremeColor(XPCOLOR_3DSHADOW));
			dc.MoveTo(rcCaption.left + 1, rcCaption.top); dc.LineTo(rcCaption.right - 1 , rcCaption.top);
			dc.MoveTo(rcCaption.left, rcCaption.top + 1); dc.LineTo(rcCaption.left, rcCaption.bottom - 1);
			dc.MoveTo(rcCaption.left + 1, rcCaption.bottom - 1); dc.LineTo(rcCaption.right - 1 ,rcCaption.bottom - 1);
			dc.MoveTo(rcCaption.right -1 , rcCaption.top + 1 ); dc.LineTo(rcCaption.right - 1, rcCaption.bottom - 1);
		}
		clrText = GetXtremeColor(bActive? COLOR_HIGHLIGHTTEXT: COLOR_BTNTEXT);		
	}

	rcCaption.right -= 2;
	DrawCaptionButtons(&dc, pPane->GetCaptionButtons(), rcCaption, clrButton, 14, 2);

	// Title text
	if (!strTitle.IsEmpty())
	{
		CRect rcText(rcCaption.left + 4, rcCaption.top + 1, rcCaption.right, rcCaption.bottom);

		dc.SetTextColor(clrText);
		CXTPFontDC font(&dc, &m_fntTitle);
		dc.DrawText(strTitle, rcText, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER | DT_NOPREFIX);
	}
}


///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneDefaultTheme

CXTPDockingPaneDefaultTheme::CXTPDockingPaneDefaultTheme()
{
	m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
	m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
}


void CXTPDockingPaneDefaultTheme::DrawCaptionButton(CDC* pDC, CXTPDockingPaneCaptionButton* pButton, COLORREF clrButton)
{
	CRect rc = pButton->GetRect();

	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_CLOSE)
	{
		pDC->DrawFrameControl(rc, DFC_CAPTION, DFCS_CAPTIONCLOSE | (pButton->IsPressed()? DFCS_PUSHED: 0));
		return;
	}

	pDC->DrawFrameControl(&rc, DFC_BUTTON, DFCS_ADJUSTRECT|DFCS_BUTTONPUSH | (pButton->IsPressed() ? DFCS_PUSHED : 0));

	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_AUTOHIDE)
	{
		CXTPPenDC pen(*pDC, clrButton);

		if (pButton->GetState() & xtpPanePinPushed)
		{
			CPoint pt(rc.left - 1, rc.CenterPoint().y - 1);
			if (pButton->IsPressed()) pt += CPoint(1, 1);

			POINT pts[] = {
				{pt.x, pt.y}, {pt.x + 2, pt.y}, {pt.x + 2, pt.y - 3},
				{pt.x + 4, pt.y - 1}, {pt.x + 5, pt.y - 2}, {pt.x + 6, pt.y - 2},
				{pt.x + 6, pt.y + 2}, {pt.x + 5, pt.y + 2}, {pt.x + 4, pt.y + 1},
				{pt.x + 2, pt.y + 3}, {pt.x + 2, pt.y} };
			pDC->Polyline(&pts[0], sizeof(pts) / sizeof(POINT));
		}
		else
		{
			CPoint pt(rc.CenterPoint().x - 1, rc.bottom -1);
			if(pButton->IsPressed()) pt+= CPoint(1, 1);

			POINT pts[] = {
				{ pt.x, pt.y }, { pt.x, pt.y - 2 }, { pt.x - 3, pt.y - 2 },
				{ pt.x - 3, pt.y - 3 }, { pt.x - 1, pt.y - 6 }, { pt.x - 2, pt.y - 7 },
				{ pt.x - 2, pt.y - 8 }, { pt.x + 2, pt.y - 8 }, { pt.x + 2, pt.y - 7 },
				{ pt.x + 1, pt.y - 6 }, { pt.x + 3, pt.y - 3 }, { pt.x + 3, pt.y - 2 },
				{ pt.x, pt.y - 2 }};
			pDC->Polyline(&pts[0], sizeof(pts) / sizeof(POINT));
		}
	}
}

void CXTPDockingPaneDefaultTheme::DrawFloatingFrame(CDC& dc, CXTPDockingPaneMiniWnd* pPane, CRect rc)
{
	dc.Draw3dRect(rc, GetXtremeColor(COLOR_3DFACE), 0);
	rc.DeflateRect(1, 1);
	dc.Draw3dRect(rc, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
	rc.DeflateRect(1, 1);
	dc.Draw3dRect(rc, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DFACE));

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - 3);

	dc.FillSolidRect(rcCaption, GetXtremeColor(COLOR_3DFACE));

	rcCaption.DeflateRect(1, 2, 1, 2);

	BOOL bActive = m_bHighlighActiveCaption && pPane->IsActive();

	dc.FillSolidRect(rcCaption, GetXtremeColor(bActive? COLOR_ACTIVECAPTION: COLOR_INACTIVECAPTION));

	rcCaption.right -= 2;
	DrawCaptionButtons(&dc, pPane->GetCaptionButtons(), rcCaption, GetXtremeColor(COLOR_BTNTEXT), 12, 2);

	CString strTitle;
	pPane->GetWindowText(strTitle);

	if (!strTitle.IsEmpty())
	{
		CRect rcText(rcCaption.left + 4, rcCaption.top + 1, rcCaption.right, rcCaption.bottom);

		dc.SetTextColor(GetXtremeColor(bActive? COLOR_CAPTIONTEXT: COLOR_INACTIVECAPTIONTEXT));
		CXTPFontDC font(&dc, &m_fntTitle);
		dc.DrawText(strTitle, rcText, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER | DT_NOPREFIX);
	}
}


void CXTPDockingPaneDefaultTheme::DrawCaption(CDC& dc, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 2, 0, rc.Height() - nTitleHeight - 1);

	BOOL bActive = m_bHighlighActiveCaption && pPane->IsActive();

	dc.FillSolidRect(rc.left, rc.top, rc.Width(), rcCaption.bottom + 2, GetXtremeColor(COLOR_3DFACE));
	dc.FillSolidRect(rcCaption, GetXtremeColor(bActive? COLOR_ACTIVECAPTION: COLOR_INACTIVECAPTION));

	rcCaption.right -= 2;
	DrawCaptionButtons(&dc, pPane->GetCaptionButtons(), rcCaption, GetXtremeColor(COLOR_BTNTEXT), 12, 2);

	if (pPane->GetSelected())
	{
		CRect rcText(rcCaption.left + 4, rcCaption.top + 1, rcCaption.right, rcCaption.bottom);

		dc.SetTextColor(GetXtremeColor(bActive? COLOR_CAPTIONTEXT: COLOR_INACTIVECAPTIONTEXT));
		CXTPFontDC font(&dc, &m_fntTitle);
		dc.DrawText(pPane->GetSelected()->GetTitle(), rcText, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER | DT_NOPREFIX);
	}
}
///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneGripperedTheme

void CXTPDockingPaneGripperedTheme::DrawCaptionButton(CDC* pDC, CXTPDockingPaneCaptionButton* pButton, COLORREF clrButton)
{
	ASSERT (pDC != NULL);
	CRect rc = pButton->GetRect();

	if (pButton->IsPressed())
	{
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_BTNTEXT), GetXtremeColor(COLOR_BTNHILIGHT));
		rc.OffsetRect(1, 1);
	}
	else if (pButton->IsSelected())
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_BTNHILIGHT), GetXtremeColor(COLOR_BTNTEXT));

	CXTPPenDC pen(*pDC, clrButton);
	CPoint pt = rc.CenterPoint();

	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_CLOSE)
	{
		pDC->MoveTo(pt.x - 4, pt.y - 3); pDC->LineTo(pt.x + 3 , pt.y + 4);
		pDC->MoveTo(pt.x - 4, pt.y + 3); pDC->LineTo(pt.x + 3 , pt.y - 4);

		pDC->MoveTo(pt.x - 3, pt.y - 3); pDC->LineTo(pt.x + 4 , pt.y + 4);
		pDC->MoveTo(pt.x - 3, pt.y + 3); pDC->LineTo(pt.x + 4 , pt.y - 4);

	}
	else if (pButton->GetID() == XTP_IDS_DOCKINGPANE_AUTOHIDE)
	{
		CXTPDockingPaneCaptionButton::DrawPinnButton(pDC, pt, pButton->GetState() & xtpPanePinPushed);
	}
}


void CXTPDockingPaneGripperedTheme::DrawCaptionPart(CDC& dc, CXTPDockingPaneBase* pPane, CRect rcCaption, CString strTitle, BOOL)
{
	COLORREF clrText = GetXtremeColor(COLOR_BTNTEXT);

	rcCaption.right -= 2;
	DrawCaptionButtons(&dc, pPane->GetCaptionButtons(), rcCaption, clrText, 14, 2);

	int nLeft = 4;
	if (!strTitle.IsEmpty())
	{
		CRect rcText(rcCaption.left + 4, rcCaption.top + 1, rcCaption.right, rcCaption.bottom);
		dc.SetTextColor(clrText);
		CXTPFontDC font(&dc, &m_fntTitle);
		dc.DrawText(strTitle, rcText, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER | DT_NOPREFIX);

		CXTPEmptyRect rcCalc;
		dc.DrawText(strTitle, rcCalc, DT_CALCRECT|DT_SINGLELINE| DT_NOPREFIX);

		nLeft = rcCalc.Width() + rcCaption.left + 8;
	}
	rcCaption.right -= 2;
	if (rcCaption.right  > nLeft )
	{
		int nCenter = rcCaption.CenterPoint().y;
		dc.Draw3dRect(nLeft, nCenter - 3, rcCaption.right - nLeft, 3, GetXtremeColor( COLOR_BTNHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
		dc.Draw3dRect(nLeft, nCenter + 1, rcCaption.right - nLeft, 3, GetXtremeColor( COLOR_BTNHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
	}
}

void CXTPDockingPaneGripperedTheme::DrawFloatingFrame(CDC& dc, CXTPDockingPaneMiniWnd* pPane, CRect rc)
{
	dc.Draw3dRect(rc, GetXtremeColor(COLOR_3DFACE), 0);
	rc.DeflateRect(1, 1);
	dc.Draw3dRect(rc, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
	rc.DeflateRect(1, 1);
	dc.Draw3dRect(rc, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DFACE));

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - 3);

	dc.FillSolidRect(rcCaption, GetXtremeColor(COLOR_3DFACE));

	CString strTitle;
	pPane->GetWindowText(strTitle);

	rcCaption.DeflateRect(1, 2, 1, 2);

	BOOL bActive = m_bHighlighActiveCaption && pPane->IsActive();
	DrawCaptionPart(dc, pPane, rcCaption, strTitle, bActive);
}

void CXTPDockingPaneGripperedTheme::DrawCaption(CDC& dc, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, m_rcCaptionMargin.top, 0, rc.Height() - nTitleHeight - 1);
	dc.FillSolidRect(rc.left, rc.top, rc.Width(), rcCaption.bottom + 2, GetXtremeColor(XPCOLOR_3DFACE));

	CString strTitle = pPane->GetSelected()? pPane->GetSelected()->GetTitle(): _T("");
	BOOL bActive = m_bHighlighActiveCaption && pPane->IsActive();
	DrawCaptionPart(dc, pPane, rcCaption, strTitle, bActive);

}

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneExplorerTheme

CXTPDockingPaneExplorerTheme::CXTPDockingPaneExplorerTheme()
{
	m_bUseBoldCaptionFont = FALSE;
	m_rcCaptionMargin.SetRect(0, 0, 0, 0);
	m_nCaptionFontGap = 9;

	RefreshMetrics();
}

void CXTPDockingPaneExplorerTheme::DrawCaptionButton(CDC* pDC, CXTPDockingPaneCaptionButton* pButton, COLORREF clrButton)
{
	if (!m_themeToolbar.IsAppThemed())
	{
		CXTPDockingPaneGripperedTheme::DrawCaptionButton(pDC, pButton, clrButton);
		return;
	}
	
	
	ASSERT (pDC != NULL);
	CRect rc = pButton->GetRect();
	
	if (pButton->IsPressed())
	{
		m_themeToolbar.DrawThemeBackground(pDC->GetSafeHdc(), TP_BUTTON, TS_PRESSED, &rc, 0);
	}
	else if (pButton->IsSelected())
	{
		m_themeToolbar.DrawThemeBackground(pDC->GetSafeHdc(), TP_BUTTON, TS_HOT, &rc, 0);
	}


	CXTPPenDC pen(*pDC, clrButton);
	CPoint pt = rc.CenterPoint();

	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_CLOSE)
	{
		pDC->MoveTo(pt.x - 4, pt.y - 3); pDC->LineTo(pt.x + 3 , pt.y + 4);
		pDC->MoveTo(pt.x - 4, pt.y + 3); pDC->LineTo(pt.x + 3 , pt.y - 4);

		pDC->MoveTo(pt.x - 3, pt.y - 3); pDC->LineTo(pt.x + 4 , pt.y + 4);
		pDC->MoveTo(pt.x - 3, pt.y + 3); pDC->LineTo(pt.x + 4 , pt.y - 4);
	}
	else if (pButton->GetID() == XTP_IDS_DOCKINGPANE_AUTOHIDE)
	{
		CXTPDockingPaneCaptionButton::DrawPinnButton(pDC, pt, pButton->GetState() & xtpPanePinPushed);
	}
}

void CXTPDockingPaneExplorerTheme::DrawCaptionPart(CDC& dc, CXTPDockingPaneBase* pPane, CRect rcCaption, CString strTitle, BOOL)
{
	if(m_themeRebar.IsAppThemed())
	{
		CWnd* pWnd = pPane->GetType() == xtpPaneTypeTabbedContainer? (CWnd*)(CXTPDockingPaneTabbedContainer*)pPane:
			pPane->GetType() == xtpPaneTypeMiniWnd? (CWnd*)(CXTPDockingPaneMiniWnd*)pPane: 0;

		ASSERT(pWnd);
		CWnd* pParent = pPane->GetType() == xtpPaneTypeMiniWnd? pWnd: pWnd->GetParentFrame();

		CRect rc, rcParent, rcClient, rcClipRect;
		pWnd->GetClientRect(&rcClient);
		rcClipRect = rcClient;

		if (pParent)
		{
			pParent->GetWindowRect(rcParent);
			pWnd->GetWindowRect(rc);

			rcClient.TopLeft() += rcParent.TopLeft() - rc.TopLeft();
			rcClient.BottomRight() += rcParent.BottomRight() - rc.BottomRight();
		}

		m_themeRebar.DrawThemeBackground(dc, 0, 0, &rcClient, &rcCaption);
	}
	
	COLORREF clrText = GetXtremeColor(COLOR_BTNTEXT);

	rcCaption.right -= 2;
	DrawCaptionButtons(&dc, pPane->GetCaptionButtons(), rcCaption, clrText, 16, 2);

	if (!strTitle.IsEmpty())
	{
		CRect rcText(rcCaption.left + 4, rcCaption.top + 1, rcCaption.right, rcCaption.bottom);
		dc.SetTextColor(clrText);
		CXTPFontDC font(&dc, &m_fntTitle);
		dc.DrawText(strTitle, rcText, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER | DT_NOPREFIX);
	}
}

void CXTPDockingPaneExplorerTheme::RefreshMetrics()
{
	CXTPDockingPaneGripperedTheme::RefreshMetrics();
	
	m_themeRebar.OpenThemeData(0, L"REBAR");
	m_themeToolbar.OpenThemeData(0, L"TOOLBAR");

	if (m_themeRebar.IsAppThemed())
	{
		m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
		m_pPanelPaintManager->SetColor(xtpTabColorWinXP);
		m_pPanelPaintManager->m_bHotTracking = TRUE;
		
		m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
		m_pTabPaintManager->SetColor(xtpTabColorWinXP);
		m_pTabPaintManager->m_bHotTracking = TRUE;
	}
	else
	{
		m_pPanelPaintManager->SetAppearance(xtpTabAppearanceFlat);
		m_pPanelPaintManager->m_bHotTracking = FALSE;
		
		m_pTabPaintManager->SetAppearance(xtpTabAppearanceVisualStudio);
		m_pTabPaintManager->m_bHotTracking = FALSE;

	}
}

///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneVisioTheme

CXTPDockingPaneVisioTheme::CXTPDockingPaneVisioTheme()
{
	RefreshMetrics();

	m_pPanelPaintManager->SetAppearance(xtpTabAppearanceFlat);

	m_pTabPaintManager->SetAppearance(xtpTabAppearanceVisio);
	m_pTabPaintManager->SetPosition(xtpTabPositionTop);

	m_nCaptionFontGap = 2;
}



void CXTPDockingPaneVisioTheme::RefreshMetrics()
{
	CXTPDockingPanePaintManager::RefreshMetrics();
}


void CXTPDockingPaneVisioTheme::DrawCaptionButtonFrame(CDC* pDC, CXTPDockingPaneCaptionButton* pButton, COLORREF& clrText)
{
	if (pButton->IsSelected())
	{
		Rectangle(pDC, pButton->GetRect(), XPCOLOR_HIGHLIGHT,  XPCOLOR_HIGHLIGHT_BORDER);
		clrText = GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT);
	}
	if (pButton->IsPressed())
	{
		Rectangle(pDC, pButton->GetRect(), XPCOLOR_HIGHLIGHT_PUSHED,  XPCOLOR_HIGHLIGHT_BORDER);
		clrText = GetXtremeColor(XPCOLOR_PUSHED_TEXT);
	}
}

void CXTPDockingPaneVisioTheme::AdjustCaptionRect(CXTPDockingPaneTabbedContainer* pPane, CRect& rc)
{
	CXTPDockingPaneOfficeTheme::AdjustCaptionRect(pPane, rc);
	rc.DeflateRect(3, 0, 3, 3);

	if (!pPane->IsTitleVisible() || !m_bShowCaption)
	{
		rc.DeflateRect(0, 3, 0, 0);
	}
	if (!pPane->IsTabsVisible() && m_pTabPaintManager->m_clientFrame != xtpTabFrameNone)
	{
		rc.DeflateRect(1, 1);
	}
}

void CXTPDockingPaneVisioTheme::DrawPane(CDC& dc, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	BOOL bActive = m_bHighlighActiveCaption && pPane->IsActive();
	dc.FillSolidRect(rc, GetXtremeColor(bActive? XPCOLOR_HIGHLIGHT: XPCOLOR_TOOLBAR_FACE));
	dc.SetBkMode(TRANSPARENT);

	CRect rcClient(rc);

	if (pPane->IsTitleVisible() && m_bShowCaption)
	{
		dc.FillSolidRect(rc.left, rc.top, rc.Width(), 1, GetXtremeColor(XPCOLOR_3DFACE));
		DrawCaption(dc, pPane, rc);

		rcClient.top ++;
	}

	AdjustCaptionRect(pPane, rc);

	if (pPane->IsTabsVisible())
	{
		m_pTabPaintManager->DrawTabControl(pPane, &dc, rc);
	}
	else if (m_pTabPaintManager->m_clientFrame != xtpTabFrameNone)
	{
		dc.Draw3dRect(rc.left - 1, rc.top - 1, rc.Width() + 2, rc.Height() + 2, m_pTabPaintManager->GetColorSet()->m_clrFrameBorder, m_pTabPaintManager->GetColorSet()->m_clrFrameBorder);
	}


	Pixel(&dc, rcClient.left, rcClient.top, XPCOLOR_3DFACE);
	Pixel(&dc, rcClient.right - 1, rcClient.top, XPCOLOR_3DFACE);
	Pixel(&dc, rcClient.left, rcClient.bottom -1, XPCOLOR_3DFACE);
	Pixel(&dc, rcClient.right - 1, rcClient.bottom -1, XPCOLOR_3DFACE);
}

void CXTPDockingPaneVisioTheme::DrawCaptionButton(CDC* pDC, CXTPDockingPaneCaptionButton* pButton, COLORREF clrText)
{
	CRect rcButton = pButton->GetRect();
	DrawCaptionButtonFrame(pDC, pButton, clrText);
	CXTPPenDC pen(*pDC, clrText);

	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_CLOSE)
	{
		CPoint ptClose = rcButton.CenterPoint();

		Line(pDC, ptClose.x - 4, ptClose.y - 3, ptClose.x + 3, ptClose.y + 4);
		Line(pDC, ptClose.x - 3, ptClose.y - 3, ptClose.x + 4, ptClose.y + 4);

		Line(pDC, ptClose.x - 4, ptClose.y + 3, ptClose.x + 3, ptClose.y - 4);
		Line(pDC, ptClose.x - 3, ptClose.y + 3, ptClose.x + 4, ptClose.y - 4);
	}

	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_AUTOHIDE)
	{
		CXTPDockingPaneCaptionButton::DrawPinnButton(pDC, rcButton.CenterPoint(),
			pButton->GetState() & xtpPanePinPushed);
	}
}

void CXTPDockingPaneVisioTheme::DrawFloatingFrame(CDC& dc, CXTPDockingPaneMiniWnd* pPane, CRect rc)
{
	BOOL bActive = m_bHighlighActiveCaption && pPane->IsActive();
	COLORREF clrFrame = GetXtremeColor(bActive? XPCOLOR_HIGHLIGHT: XPCOLOR_TOOLBAR_FACE);

	dc.Draw3dRect(rc, clrFrame, 0);
	rc.DeflateRect(1, 1);
	dc.Draw3dRect(rc, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
	rc.DeflateRect(1, 1);
	dc.Draw3dRect(rc, clrFrame, clrFrame);

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - 3);

	dc.FillSolidRect(rcCaption, clrFrame);

	rcCaption.right -= 2;
	DrawCaptionButtons(&dc, pPane->GetCaptionButtons(), rcCaption, GetXtremeColor(COLOR_BTNTEXT), 16, 0);

	CString strTitle;
	pPane->GetWindowText(strTitle);


	int nRight = rcCaption.right;


	COLORREF clrText = GetXtremeColor(COLOR_BTNTEXT);
	int nLeft = 4;
	if (!strTitle.IsEmpty())
	{
		CRect rcText(rcCaption.left + 4, rcCaption.top + 1, nRight - 2, rcCaption.bottom);
		dc.SetTextColor(clrText);
		CXTPFontDC font(&dc, &m_fntTitle);
		dc.DrawText(strTitle, rcText, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER | DT_NOPREFIX);

		CRect rcCalc(0, 0, 0, 0);
		dc.DrawText(strTitle, rcCalc, DT_CALCRECT|DT_SINGLELINE| DT_NOPREFIX);

		nLeft = rcCalc.Width() + rcCaption.left + 14;
	}
	nRight -= 4;

	if (nRight - nLeft > 0)
	{
		int nCenter = rcCaption.CenterPoint().y;
		Line(&dc, nLeft, nCenter - 3, nRight, nCenter - 3, !bActive? XPCOLOR_TOOLBAR_GRIPPER: COLOR_BTNTEXT);
		Line(&dc, nLeft, nCenter - 1, nRight, nCenter - 1, !bActive? XPCOLOR_TOOLBAR_GRIPPER: COLOR_BTNTEXT);
		Line(&dc, nLeft, nCenter + 1, nRight, nCenter + 1, !bActive? XPCOLOR_TOOLBAR_GRIPPER: COLOR_BTNTEXT);
		Line(&dc, nLeft, nCenter + 3, nRight, nCenter + 3, !bActive? XPCOLOR_TOOLBAR_GRIPPER: COLOR_BTNTEXT);
	}
}

void CXTPDockingPaneVisioTheme::DrawCaption(CDC& dc, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	BOOL bActive = m_bHighlighActiveCaption && pPane->IsActive();
	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 2, 1, rc.Height() - nTitleHeight - 3);

	DrawCaptionButtons(&dc, pPane->GetCaptionButtons(), rcCaption, 0, 16, 0);

	int nRight = rcCaption.right;


	COLORREF clrText = GetXtremeColor(COLOR_BTNTEXT);
	int nLeft = 4;
	if (pPane->GetSelected())
	{
		CRect rcText(rcCaption.left + 4, rcCaption.top + 1, nRight - 2, rcCaption.bottom);
		dc.SetTextColor(clrText);
		CXTPFontDC font(&dc, &m_fntTitle);
		dc.DrawText(pPane->GetSelected()->GetTitle(), rcText, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER | DT_NOPREFIX);

		CRect rcCalc(0, 0, 0, 0);
		dc.DrawText(pPane->GetSelected()->GetTitle(), rcCalc, DT_CALCRECT|DT_SINGLELINE| DT_NOPREFIX);

		nLeft = rcCalc.Width() + rcCaption.left + 14;
	}
	nRight -= 4;

	if (nRight - nLeft > 0)
	{
		int nCenter = rcCaption.CenterPoint().y;
		Line(&dc, nLeft, nCenter - 3, nRight, nCenter - 3, !bActive? XPCOLOR_TOOLBAR_GRIPPER: COLOR_BTNTEXT);
		Line(&dc, nLeft, nCenter - 1, nRight, nCenter - 1, !bActive? XPCOLOR_TOOLBAR_GRIPPER: COLOR_BTNTEXT);
		Line(&dc, nLeft, nCenter + 1, nRight, nCenter + 1, !bActive? XPCOLOR_TOOLBAR_GRIPPER: COLOR_BTNTEXT);
		Line(&dc, nLeft, nCenter + 3, nRight, nCenter + 3, !bActive? XPCOLOR_TOOLBAR_GRIPPER: COLOR_BTNTEXT);
	}
}



///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneOffice2003Theme


CXTPDockingPaneOffice2003Theme::CXTPDockingPaneOffice2003Theme()
{
	m_bDrawGripper = TRUE;
	m_bRoundedCaption = TRUE;
	m_nCaptionFontGap = 7;

	m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPageFlat);
	m_pPanelPaintManager->SetColor(xtpTabColorOffice2003);
	m_pPanelPaintManager->m_bHotTracking = TRUE;

	m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPageFlat);
	m_pTabPaintManager->SetColor(xtpTabColorOffice2003);
	m_pTabPaintManager->m_bHotTracking = TRUE;

	RefreshMetrics();
}

CXTPDockingPaneOffice2003Theme::~CXTPDockingPaneOffice2003Theme()
{

}


void CXTPDockingPaneOffice2003Theme::RefreshMetrics()
{
	CXTPDockingPaneOfficeTheme::RefreshMetrics();

	m_clrNormalCaptionLight = XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 0xcd);
	m_clrNormalCaptionDark = GetXtremeColor(COLOR_3DFACE);

	m_clrActiveCaptionLight = m_clrActiveCaptionDark = GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED);
	m_clrNormalCaptionText = m_clrActiveCaptionText = GetXtremeColor(COLOR_BTNTEXT);

	m_bLunaTheme = FALSE;

	XTPCurrentSystemTheme systemTheme = GetCurrentSystemTheme();

	switch (systemTheme)
	{
	case xtpSystemThemeBlue:
		{
			m_clrNormalCaptionDark = RGB(129, 169, 226);
			m_clrNormalCaptionLight = RGB(221, 236, 254);

			m_arrColor[XPCOLOR_3DFACE] = RGB(216, 231, 252);
			INT nElements[] =      { XPCOLOR_TOOLBAR_GRIPPER, XPCOLOR_SEPARATOR,  XPCOLOR_DISABLED,   XPCOLOR_MENUBAR_FACE, XPCOLOR_MENUBAR_BORDER, XPCOLOR_HIGHLIGHT,  XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED, XPCOLOR_HIGHLIGHT_CHECKED, XPCOLOR_TOOLBAR_FACE, XPCOLOR_PUSHED_TEXT };
			COLORREF clrValues[] = { RGB(39, 65, 118),        RGB(106, 140, 203), RGB(109, 150, 208), RGB(246, 246, 246),   RGB(0, 45, 150),        RGB(255, 238, 194), RGB(0, 0, 128),           RGB(0, 0, 128),                  RGB(0, 0, 128),                   RGB(254, 128, 62),        RGB(255, 192, 111),         RGB(158, 190, 245),   0};
			SetColors(sizeof(nElements)/sizeof(INT), nElements, clrValues);

			m_bLunaTheme = TRUE;

		}
		break;

	case xtpSystemThemeOlive:
		{
			m_clrNormalCaptionDark = RGB(183, 198, 145);
			m_clrNormalCaptionLight = RGB(244, 247, 222);

			m_arrColor[XPCOLOR_3DFACE] = RGB(226, 231, 191);
			INT nElements[] =      { XPCOLOR_TOOLBAR_GRIPPER, XPCOLOR_SEPARATOR,  XPCOLOR_DISABLED,   XPCOLOR_MENUBAR_FACE, XPCOLOR_MENUBAR_BORDER, XPCOLOR_HIGHLIGHT,  XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED,  XPCOLOR_HIGHLIGHT_CHECKED, XPCOLOR_TOOLBAR_FACE, XPCOLOR_PUSHED_TEXT };
			COLORREF clrValues[] = { RGB(81, 94, 51),         RGB(96, 128, 88),    RGB(159, 174, 122), RGB(244, 244, 238),   RGB(117, 141, 94),      RGB(255, 238, 194), RGB(63, 93, 56),          RGB(63, 93, 56),                  RGB(63, 93, 56),                 RGB(254, 128, 62),          RGB(255, 192, 111),        RGB(217, 217, 167),   0};
			SetColors(sizeof(nElements)/sizeof(INT), nElements, clrValues);

			m_bLunaTheme = TRUE;
		}
		break;

	case xtpSystemThemeSilver:
		{

			m_clrNormalCaptionDark = RGB(153, 151, 181);
			m_clrNormalCaptionLight = RGB(243, 244, 250);

			m_arrColor[XPCOLOR_3DFACE] = RGB(223, 223, 234);

			INT nElements[] =      { XPCOLOR_TOOLBAR_GRIPPER, XPCOLOR_SEPARATOR,  XPCOLOR_DISABLED,   XPCOLOR_MENUBAR_FACE, XPCOLOR_MENUBAR_BORDER, XPCOLOR_HIGHLIGHT,  XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED,  XPCOLOR_HIGHLIGHT_CHECKED, XPCOLOR_TOOLBAR_FACE, XPCOLOR_PUSHED_TEXT };
			COLORREF clrValues[] = { RGB(84, 84, 117),        RGB(110, 109, 143), RGB(168, 167, 190), RGB(253, 250, 255),   RGB(124, 124, 148),     RGB(255, 238, 194), RGB(75, 75, 111),          RGB(75, 75, 111),                  RGB(75, 75, 111),              RGB(254, 128, 62),         RGB(255, 192, 111),        RGB(215, 215, 229),   0};
			SetColors(sizeof(nElements)/sizeof(INT), nElements, clrValues);

			m_bLunaTheme = TRUE;
		}
	}

	if (m_bLunaTheme)
	{
		m_clrActiveCaptionLight = RGB(255, 213, 140);
		m_clrActiveCaptionDark = RGB(255, 166, 76);

	}


}

void CXTPDockingPaneOffice2003Theme::DrawCaptionButtonFrame(CDC* pDC, CXTPDockingPaneCaptionButton* pButton, COLORREF& clrText)
{
	CRect rc = pButton->GetRect();

	if (pButton->IsSelected())
	{
		if (!m_bLunaTheme)
		{
			Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT,  XPCOLOR_HIGHLIGHT_BORDER);
			clrText = GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT);
		} else
		{
			XTPDrawHelpers()->GradientFill(pDC, rc, RGB(255, 242, 200), RGB(255, 212, 151), FALSE);
			pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER), GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
			clrText = 0;
		}
	}
	if (pButton->IsPressed())
	{
		if (!m_bLunaTheme)
		{
			Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED,  XPCOLOR_HIGHLIGHT_BORDER);
			clrText = GetXtremeColor(XPCOLOR_PUSHED_TEXT);
		}
		else
		{
			XTPDrawHelpers()->GradientFill(pDC, rc, RGB(254, 142, 75), RGB(255, 207, 139), FALSE);
			pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER), GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
			clrText = 0;
		}
	}
}



void CXTPDockingPaneOffice2003Theme::DrawFloatingFrame(CDC& dc, CXTPDockingPaneMiniWnd* pPane, CRect rc)
{
	COLORREF clrFrame = GetXtremeColor(XPCOLOR_MENUBAR_BORDER);

	dc.Draw3dRect(rc, clrFrame, clrFrame);
	rc.DeflateRect(1, 1);
	dc.Draw3dRect(rc, clrFrame, clrFrame);
	rc.DeflateRect(1, 1);

	dc.Draw3dRect(rc, clrFrame, clrFrame);
	XTPDrawHelpers()->ExcludeCorners(&dc, rc);
	dc.Draw3dRect(rc, m_clrNormalCaptionDark, m_clrNormalCaptionDark);

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - 3);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, rcCaption.Width(), rcCaption.Height());
	CXTPCompatibleDC dcCache(&dc, &bmp);

	COLORREF clrText = m_clrNormalCaptionText;

	if (m_bHighlighActiveCaption && pPane->IsActive())
	{
		XTPDrawHelpers()->GradientFill(&dcCache, CRect(0, 0, rcCaption.Width(), rcCaption.Height()), m_clrActiveCaptionLight, m_clrActiveCaptionDark, FALSE);
		clrText = m_clrActiveCaptionText;
	}
	else
	{
		XTPDrawHelpers()->GradientFill(&dcCache, CRect(0, 0, rcCaption.Width(), rcCaption.Height()), m_clrNormalCaptionLight, m_clrNormalCaptionDark, FALSE);
	}

	dc.BitBlt(rcCaption.left, rcCaption.top, rcCaption.Width(), rcCaption.Height(), &dcCache, 0, 0, SRCCOPY);

	dc.SetPixel(rcCaption.left, rcCaption.bottom, m_clrNormalCaptionDark);
	dc.SetPixel(rcCaption.right - 1, rcCaption.bottom, m_clrNormalCaptionDark);

	rcCaption.left += 1;
	rcCaption.right -= 5;
	DrawCaptionButtons(&dc, pPane->GetCaptionButtons(), rcCaption, clrText, 16, 0);

	if (m_bDrawGripper && (rcCaption.right > rcCaption.left + 7))
	{		
		for (int y = 5; y < rcCaption.Height() - 3; y += 4)
		{
			dc.FillSolidRect(rcCaption.left + 4 + 2, y + 1, 2, 2, GetXtremeColor(COLOR_BTNHIGHLIGHT));
			dc.FillSolidRect(rcCaption.left + 3 + 2, y, 2, 2, GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}
		rcCaption.left += 8;
	}



	CString strTitle;
	pPane->GetWindowText(strTitle);

	if (!strTitle.IsEmpty())
	{
		CRect rcText(rcCaption.left + 6, rcCaption.top + 1, rcCaption.right - 2, rcCaption.bottom);

		dc.SetTextColor(clrText);
		CXTPFontDC font(&dc, &m_fntTitle);
		dc.DrawText(strTitle, rcText, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER | DT_NOPREFIX);
	}

}


void CXTPDockingPaneOffice2003Theme::DrawCaptionButton(CDC* pDC, CXTPDockingPaneCaptionButton* pButton, COLORREF clrText)
{
	CRect rcButton = pButton->GetRect();
	DrawCaptionButtonFrame(pDC, pButton, clrText);
	CXTPPenDC pen(*pDC, clrText);

	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_CLOSE)
	{
		CPoint ptClose = rcButton.CenterPoint();
		

		Line(pDC, ptClose.x - 4, ptClose.y - 3, ptClose.x + 3, ptClose.y + 4);
		Line(pDC, ptClose.x - 3, ptClose.y - 3, ptClose.x + 4, ptClose.y + 4);

		Line(pDC, ptClose.x - 4, ptClose.y + 3, ptClose.x + 3, ptClose.y - 4);
		Line(pDC, ptClose.x - 3, ptClose.y + 3, ptClose.x + 4, ptClose.y - 4);
	}

	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_AUTOHIDE)
	{
		CXTPDockingPaneCaptionButton::DrawPinnButton(pDC, rcButton.CenterPoint(),
			pButton->GetState() & xtpPanePinPushed);
	}
}


void CXTPDockingPaneOffice2003Theme::DrawCaption(CDC& dc, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - 3);

	CRect rcFill(rcCaption); 
	
	if (m_bRoundedCaption)
	{	
		rcFill.DeflateRect(0, 1, 0, 0);
		dc.FillSolidRect(rcCaption, GetXtremeColor(XPCOLOR_3DFACE));

		XTPDrawHelpers()->ExcludeCorners(&dc, rcFill);
	}
	COLORREF clrText = m_clrNormalCaptionText;

	if (m_bHighlighActiveCaption && pPane->IsActive())
	{
		XTPDrawHelpers()->GradientFill(&dc, rcFill, m_clrActiveCaptionLight, m_clrActiveCaptionDark, FALSE);
		clrText = m_clrActiveCaptionText;
	}
	else
	{
		XTPDrawHelpers()->GradientFill(&dc, rcFill, m_clrNormalCaptionLight, m_clrNormalCaptionDark, FALSE);
	}
	if (!m_bRoundedCaption)
	{
		dc.FillSolidRect(rcCaption.left, rcCaption.top, 1, rcCaption.Height(), GetXtremeColor(XPCOLOR_FRAME));
		dc.FillSolidRect(rcCaption.left, rcCaption.top, rcCaption.Width(), 1, GetXtremeColor(XPCOLOR_FRAME));
		dc.FillSolidRect(rcCaption.right - 1, rcCaption.top, 1, rcCaption.Height(), GetXtremeColor(XPCOLOR_FRAME));
	}

	rcCaption.right -= 4;
	DrawCaptionButtons(&dc, pPane->GetCaptionButtons(), rcCaption, clrText, 16, 0);

	if (m_bDrawGripper && rcCaption.right > rcCaption.left + 7)
	{	
		for (int y = 5; y < rcCaption.Height() - 3; y += 4)
		{
			dc.FillSolidRect(CRect(4 + 2, y + 1, 6 + 2, y + 3), GetXtremeColor(COLOR_BTNHIGHLIGHT));
			dc.FillSolidRect(CRect(3 + 2, y, 5 + 2, y + 2), GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}
		rcCaption.left += 8;
	}

	if (pPane->GetSelected())
	{
		CRect rcText(rcCaption.left + 6, rcCaption.top + 1, rcCaption.right - 2, rcCaption.bottom);

		dc.SetTextColor(clrText);
		CXTPFontDC font(&dc, &m_fntTitle);
		dc.DrawText(pPane->GetSelected()->GetTitle(), rcText, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER | DT_NOPREFIX);
	}
}


///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneShortcutBar2003Theme


CXTPDockingPaneShortcutBar2003Theme::CXTPDockingPaneShortcutBar2003Theme()
{
	m_bDrawGripper = FALSE;
	m_bRoundedCaption = FALSE;
	m_nCaptionFontGap = 3;
	RefreshMetrics();
}

CXTPDockingPaneShortcutBar2003Theme::~CXTPDockingPaneShortcutBar2003Theme()
{ 
}


void CXTPDockingPaneShortcutBar2003Theme::RefreshMetrics()
{
	CXTPDockingPaneOffice2003Theme::RefreshMetrics();

	CWindowDC dc(CWnd::GetDesktopWindow());

	if (m_bUseStandardFont)
	{
		LOGFONT lf = { sizeof(LOGFONT)};
		
		VERIFY(::SystemParametersInfo(SPI_GETICONTITLELOGFONT,
			sizeof(LOGFONT), &lf, 0));
		
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Arial"));
		lf.lfWeight = FW_BOLD;
		lf.lfHeight = 20;

		SetCaptionFontIndirect(&lf);
	}

	if (m_bLunaTheme)
	{
		m_clrNormalCaptionDark = XTPColorManager()->grcCaption.clrDark;
		m_clrNormalCaptionLight = XTPColorManager()->grcCaption.clrLight;
		m_clrNormalCaptionText = RGB(255,255, 255);
	}
	else
	{
		m_clrNormalCaptionLight = XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_WINDOW), 0xcd);
		m_clrNormalCaptionDark = GetXtremeColor(COLOR_3DSHADOW);
	}
}


//////////////////////////////////////////////////////////////////////////
///

CXTPDockingPaneNativeXPTheme::CXTPDockingPaneNativeXPTheme()
{
	m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
	m_pPanelPaintManager->SetColor(xtpTabColorWinXP);
	m_pPanelPaintManager->m_bHotTracking = TRUE;

	m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
	m_pTabPaintManager->SetColor(xtpTabColorWinXP);
	m_pTabPaintManager->m_bHotTracking = TRUE;

	m_bGradientCaption = TRUE;
	m_nCaptionFontGap = 7;

	RefreshMetrics();
}

void CXTPDockingPaneNativeXPTheme::RefreshMetrics()
{
	CXTPDockingPaneOfficeTheme::RefreshMetrics();

	m_themeWindow.OpenThemeData(0, L"WINDOW");
	m_themeExplorer.OpenTheme(0, L"EXPLORERBAR");
}

void CXTPDockingPaneNativeXPTheme::DrawCaptionButton(CDC* pDC, CXTPDockingPaneCaptionButton* pButton, COLORREF clrButton)
{
	if (m_themeExplorer.IsAppThemed() && (pButton->GetID() == XTP_IDS_DOCKINGPANE_CLOSE || pButton->GetID() == XTP_IDS_DOCKINGPANE_AUTOHIDE))
	{
		BOOL bActive = TRUE;
		if (pButton->GetPane()->GetType() == xtpPaneTypeTabbedContainer)
			bActive = ((CXTPDockingPaneTabbedContainer*)pButton->GetPane())->IsActive();
		if (pButton->GetPane()->GetType() == xtpPaneTypeMiniWnd)
			bActive = ((CXTPDockingPaneMiniWnd*)pButton->GetPane())->IsActive();

		bActive = m_bHighlighActiveCaption && bActive;

		if (pButton->GetID() == XTP_IDS_DOCKINGPANE_CLOSE)
		{
			int nState = bActive? pButton->IsPressed()? EBHC_PRESSED: EBHC_HOT: EBHC_NORMAL;
			m_themeExplorer.DrawThemeBackground(*pDC, EBP_HEADERCLOSE, nState, pButton->GetRect(), 0);
		}
		else
		{
			int nState = bActive? pButton->IsPressed()? EBHP_PRESSED: pButton->IsSelected()? EBHP_HOT: EBHP_NORMAL: EBHP_NORMAL;
			m_themeExplorer.DrawThemeBackground(*pDC, EBP_HEADERPIN, (pButton->GetState() & xtpPanePinPushed? 3: 0) + nState, pButton->GetRect(), 0);
		}
	} else
	{
		CXTPDockingPaneOfficeTheme::DrawCaptionButton(pDC, pButton, clrButton);
	}
}


void CXTPDockingPaneNativeXPTheme::DrawFloatingFrame(CDC& dc, CXTPDockingPaneMiniWnd* pPane, CRect rc)
{
	if (!m_themeWindow.IsAppThemed())
	{
		CXTPDockingPaneOfficeTheme::DrawFloatingFrame(dc, pPane, rc);
		return;
	}
	BOOL bActive = m_bHighlighActiveCaption && pPane->IsActive();

	int nState = bActive? CS_ACTIVE: CS_INACTIVE;

	m_themeWindow.DrawThemeBackground(dc, WP_SMALLFRAMELEFT, nState, CRect(rc.left, rc.top, rc.left + 3, rc.bottom), 0);
	m_themeWindow.DrawThemeBackground(dc, WP_SMALLFRAMERIGHT, nState, CRect(rc.right - 3, rc.top, rc.right, rc.bottom), 0);
	m_themeWindow.DrawThemeBackground(dc, WP_SMALLFRAMEBOTTOM, nState, CRect(rc.left, rc.bottom - 3, rc.right, rc.bottom), 0);

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - 3 - 2);

	m_themeWindow.DrawThemeBackground(dc, WP_SMALLCAPTION, nState, rcCaption, 0);

	COLORREF clrText = GetXtremeColor(bActive? COLOR_CAPTIONTEXT: COLOR_INACTIVECAPTIONTEXT);

	rcCaption.DeflateRect(3, 0, 5, 0);
	DrawCaptionButtons(&dc, pPane->GetCaptionButtons(), rcCaption, clrText, 16, 3);

	CString strTitle;
	pPane->GetWindowText(strTitle);

	if (!strTitle.IsEmpty())
	{
		CRect rcText(rcCaption.left + 4, rcCaption.top + 1, rcCaption.right, rcCaption.bottom);

		dc.SetTextColor(clrText);
		CXTPFontDC font(&dc, &m_fntTitle);
		dc.DrawText(strTitle, rcText, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER | DT_NOPREFIX);
	}
}

void CXTPDockingPaneNativeXPTheme::DrawCaption(CDC& dc,CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	if (!m_themeWindow.IsAppThemed())
	{
		CXTPDockingPaneOfficeTheme::DrawCaption(dc, pPane, rc);
		return;
	}
	BOOL bActive = m_bHighlighActiveCaption && pPane->IsActive();

	CRect rcCaption(0, 0, rc.right, m_nTitleHeight + 1);
	CXTPDockingPane* pSelected = pPane->GetSelected();

	// Drawing Frame
	m_themeWindow.DrawThemeBackground(dc, WP_SMALLCAPTION, bActive? CS_ACTIVE: CS_INACTIVE, rcCaption, 0);

	COLORREF clrText = GetXtremeColor(bActive? COLOR_CAPTIONTEXT: COLOR_INACTIVECAPTIONTEXT);

	rcCaption.right -= 2;
	DrawCaptionButtons(&dc, pPane->GetCaptionButtons(), rcCaption, clrText, 16, 3);

	// Drawing Title
	if (pPane->GetSelected())
	{
		CRect rcText(rcCaption.left + 4, rcCaption.top + 1, rcCaption.right, rcCaption.bottom);

		dc.SetTextColor(clrText);
		CXTPFontDC font(&dc, &m_fntTitle);
		dc.DrawText(pSelected->GetTitle(), rcText, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER | DT_NOPREFIX);
	}
}


//////////////////////////////////////////////////////////////////////////
//

class CXTPDockingPaneWhidbeyTheme::CColorSetWhidbey : public CXTPTabPaintManager::CColorSetOffice2003
{
public:
	CColorSetWhidbey(BOOL bHighlightSelected)
	{
		m_bHighlightSelected = bHighlightSelected;

	}
	void RefreshMetrics();

	void FillFramePartRect(CDC* pDC, CRect rc, CWnd* pWnd)
	{
		COLORREF clrLight = m_clrHeaderFace.clrLight, clrDark = m_clrHeaderFace.clrDark;

		CWnd* pFrame = pWnd->GetParentFrame();

		if (pFrame)
		{
			CRect rcFrame;
			pFrame->GetWindowRect(&rcFrame);
			pWnd->ScreenToClient(&rcFrame);

			float fWidth = (float)max(rcFrame.Width(), GetSystemMetrics(SM_CXFULLSCREEN)/ 2);

			COLORREF clr1 = XTPDrawHelpers()->BlendColors(clrLight, clrDark, float(rc.left - rcFrame.left) / fWidth);
			COLORREF clr2 = XTPDrawHelpers()->BlendColors(clrLight, clrDark, float(rc.right - rcFrame.left) / fWidth);

			XTPDrawHelpers()->GradientFill(pDC, rc, clr1, clr2, TRUE);
		}

	}

	void FillHeader(CDC* pDC, CRect rc, CXTPTabManager* pTabManager)
	{
		FillFramePartRect(pDC, rc, (CXTPDockingPaneTabbedContainer*)pTabManager);
	}

	COLORREF FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem)
	{
		if (m_bGradientButton)
			return CColorSetOffice2003::FillPropertyButton(pDC, rcItem, pItem);

		return CColorSet::FillPropertyButton(pDC, rcItem, pItem);

	}
protected:
	BOOL m_bGradientButton;
	BOOL m_bHighlightSelected;

};

void CXTPDockingPaneWhidbeyTheme::CColorSetWhidbey::RefreshMetrics()
{
	CColorSet::RefreshMetrics();

	if (m_bHighlightSelected)
	{
		m_clrButtonSelected = RGB(255, 255, 255);
	}
	m_clrHeaderFace.SetStandardValue(GetSysColor(COLOR_3DFACE), XTPColorManager()->LightColor(GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_WINDOW), 0xcd));

	m_clrAutoHideFace.SetStandardValue(m_clrHeaderFace.clrDark, m_clrHeaderFace.clrLight);

	m_bGradientButton = FALSE;
	m_bLunaTheme = FALSE;

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
	case xtpSystemThemeSilver:
		m_clrButtonNormal = RGB(242, 242, 247);
	case xtpSystemThemeBlue:
	case xtpSystemThemeOlive:
		m_bGradientButton = TRUE;
	}

	m_clrAutoHideFace.SetStandardValue(m_clrHeaderFace.clrDark, m_clrHeaderFace.clrLight);
}


void CXTPDockingPaneWhidbeyTheme::DrawSplitter(CDC& dc, CXTPDockingPaneSplitterWnd* pSplitter)
{
	CXTPClientRect rc((CWnd*)pSplitter);
	((CColorSetWhidbey*)m_pTabPaintManager->GetColorSet())->FillFramePartRect(&dc, rc, (CWnd*)pSplitter);
}


CXTPDockingPaneWhidbeyTheme::CXTPDockingPaneWhidbeyTheme()
{
	CXTPTabPaintManager::CAppearanceSetPropertyPageFlat* pAppearanceFlat = (CXTPTabPaintManager::CAppearanceSetPropertyPageFlat*)
		m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPageFlat);

	m_pTabPaintManager->m_bDisableLunaColors = TRUE;
	pAppearanceFlat->m_bBlurPoints = FALSE;
	m_pTabPaintManager->SetColorSet(new CColorSetWhidbey(TRUE));
	m_pTabPaintManager->m_bHotTracking = FALSE;
	m_pTabPaintManager->m_clientFrame = xtpTabFrameNone;

	m_pPanelPaintManager->m_bDisableLunaColors = TRUE;
	pAppearanceFlat = (CXTPTabPaintManager::CAppearanceSetPropertyPageFlat*)m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPageFlat);
	pAppearanceFlat->m_bBlurPoints = FALSE;
	m_pPanelPaintManager->SetColorSet(new CColorSetWhidbey(FALSE));
	m_pPanelPaintManager->m_bHotTracking = FALSE;
	m_pPanelPaintManager->SetLayout(xtpTabLayoutAutoSize);

	m_rcCaptionMargin.SetRect(0, 0, 0, 0);
	m_bGradientCaption = TRUE;
	m_bUseBoldCaptionFont = FALSE;

	m_systemTheme = xtpSystemThemeUnknown;
	RefreshMetrics();
}

void CXTPDockingPaneWhidbeyTheme::RefreshMetrics()
{
	CXTPDockingPaneOfficeTheme::RefreshMetrics();

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
	case xtpSystemThemeBlue:
	case xtpSystemThemeOlive:
		m_clrInactiveCaption.SetStandardValue(RGB(216, 215, 198), RGB(238, 239, 229));
		m_clrInactiveCaptionText = 0;
		break;
	case xtpSystemThemeSilver:
		m_clrInactiveCaption.SetStandardValue(RGB(224, 224, 235), RGB(242, 242, 246));
		m_clrInactiveCaptionText = 0;
		m_clrActiveCaption.SetStandardValue(RGB(160, 158, 186), RGB(224, 225, 235));
		break;
	}

}


