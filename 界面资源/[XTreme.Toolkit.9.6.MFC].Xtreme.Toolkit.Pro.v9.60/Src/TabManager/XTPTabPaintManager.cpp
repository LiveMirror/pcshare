// XTPTabPaintManager.cpp: implementation of the CXTPTabPaintManager class.
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

#include "XTPTabPaintManager.h"
#include "XTPTabManager.h"
#include "XTPColorManager.h"
#include "XTPDrawHelpers.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CXTPTabPaintManager


CXTPTabPaintManager::CXTPTabPaintManager()
{
	m_bUseStandardFont = TRUE;

	m_pColorSet = NULL;
	m_pAppearanceSet = NULL;

	SetAppearance(xtpTabAppearancePropertyPage);

	m_bHotTracking = FALSE;
	m_bBoldNormal = m_bBoldSelected = FALSE;
	m_bShowIcons = TRUE;

	m_bDisableLunaColors = FALSE;
	m_bOneNoteColors = FALSE;

	m_tabPosition = xtpTabPositionTop;
	m_tabLayout = xtpTabLayoutAutoSize;

	m_clientFrame = xtpTabFrameBorder;
	m_bStaticFrame = FALSE;

	m_bInvertGradient = FALSE;
	m_bFillBackground = TRUE;

	m_rcClientMargin.SetRect(0, 0, 0, 0);
	m_rcControlMargin.SetRect(0, 0, 0, 0);
	m_rcButtonMargin.SetRect(0, 0, 0, 0);

	m_szIcon = CSize(16, 16);

	m_bDrawTextPathEllipsis = FALSE;
	m_bDrawTextNoPrefix = TRUE;

	EnableAutomation();



	RefreshMetrics();
}

CXTPTabPaintManager::~CXTPTabPaintManager()
{
	delete m_pColorSet;
	delete m_pAppearanceSet;

}

CXTPTabPaintManager::CColorSet* CXTPTabPaintManager::SetColor(XTPTabColorStyle tabColor)
{
	CColorSet* pColorSet = NULL;

	m_tabColor = tabColor;
	if (tabColor == xtpTabColorVisualStudio) pColorSet = new CColorSetVisualStudio();
	else if (tabColor == xtpTabColorOffice2003) pColorSet = new CColorSetOffice2003();
	else if (tabColor == xtpTabColorWinXP) pColorSet = new CColorSetWinXP();
	else if (tabColor == xtpTabColorWhidbey) pColorSet = new CColorSetWhidbey();
	else pColorSet = new CColorSetDefault();

	return SetColorSet(pColorSet);
}
CXTPTabPaintManager::CColorSet* CXTPTabPaintManager::SetColorSet(CColorSet* pColorSet)
{
	if (pColorSet)
	{
		if (m_pColorSet) delete m_pColorSet;
		m_pColorSet = pColorSet;

		pColorSet->m_pPaintManager = this;
		pColorSet->RefreshMetrics();
	}
	OnPropertyChanged();

	return pColorSet;
}

CXTPTabPaintManager::CAppearanceSet* CXTPTabPaintManager::SetAppearance(XTPTabAppearanceStyle tabAppearance)
{
	CAppearanceSet* pAppearanceSet = NULL;

	m_tabAppearance = tabAppearance;
	if (tabAppearance == xtpTabAppearanceVisualStudio) pAppearanceSet = new CAppearanceSetVisualStudio();
	else if (tabAppearance == xtpTabAppearanceExcel) pAppearanceSet = new CAppearanceSetExcel();
	else if (tabAppearance == xtpTabAppearancePropertyPageFlat) pAppearanceSet = new CAppearanceSetPropertyPageFlat();
	else if (tabAppearance == xtpTabAppearanceFlat) pAppearanceSet = new CAppearanceSetFlat();
	else if (tabAppearance == xtpTabAppearanceStateButtons) pAppearanceSet = new CAppearanceSetStateButtons();
	else if (tabAppearance == xtpTabAppearancePropertyPageSelected) pAppearanceSet = new CAppearanceSetPropertyPageSelected();
	else if (tabAppearance == xtpTabAppearancePropertyPage2003) pAppearanceSet = new CAppearanceSetPropertyPage2003();
	else if (tabAppearance == xtpTabAppearanceVisio) pAppearanceSet = new CAppearanceSetVisio();
	else pAppearanceSet = new CAppearanceSetPropertyPage();

	return SetAppearanceSet(pAppearanceSet);

}
CXTPTabPaintManager::CAppearanceSet* CXTPTabPaintManager::SetAppearanceSet(CAppearanceSet* pAppearanceSet)
{
	if (pAppearanceSet)
	{
		if (m_pAppearanceSet) delete m_pAppearanceSet;
		m_pAppearanceSet = pAppearanceSet;

		pAppearanceSet->m_pPaintManager = this;
		pAppearanceSet->RefreshMetrics();

		SetColor(pAppearanceSet->GetDefaultColorSet());
	}
	OnPropertyChanged();

	return pAppearanceSet;

}


void CXTPTabPaintManager::SetFontIndirect(LOGFONT* pLogFont, BOOL bUseStandardFont /*= FALSE*/)
{
	m_bUseStandardFont = bUseStandardFont;

	if (!pLogFont)
		return;

	m_fntNormal.DeleteObject();
	m_fntBold.DeleteObject();
	m_fntVerticalNormal.DeleteObject();
	m_fntVerticalBold.DeleteObject();

	pLogFont->lfWeight = m_bBoldNormal? FW_BOLD: FW_NORMAL;
	VERIFY( m_fntNormal.CreateFontIndirect(pLogFont) );

	pLogFont->lfWeight = FW_BOLD;
	VERIFY( m_fntBold.CreateFontIndirect(pLogFont));

	STRCPY_S(pLogFont->lfFaceName, LF_FACESIZE, XTPDrawHelpers()->GetVerticalFontName(FALSE));
	pLogFont->lfOrientation = 900;
	pLogFont->lfEscapement  = 2700;
	pLogFont->lfHeight      = pLogFont->lfHeight < 0 ? __min(-11, pLogFont->lfHeight) : pLogFont->lfHeight;
	pLogFont->lfWeight = m_bBoldNormal? FW_BOLD: FW_NORMAL;
	VERIFY( m_fntVerticalNormal.CreateFontIndirect(pLogFont));

	pLogFont->lfWeight = FW_BOLD;
	VERIFY( m_fntVerticalBold.CreateFontIndirect(pLogFont));
}

void CXTPTabPaintManager::RefreshMetrics()
{
	if (m_bUseStandardFont)
	{
		LOGFONT lfIcon;
		VERIFY( ::SystemParametersInfo( SPI_GETICONTITLELOGFONT, sizeof( lfIcon ), &lfIcon, 0 ) );

		SetFontIndirect(&lfIcon, TRUE);
	}

	m_pColorSet->RefreshMetrics();
	m_pAppearanceSet->RefreshMetrics();
}

void CXTPTabPaintManager::DisableLunaColors(BOOL bDisableLunaColors)
{
	m_bDisableLunaColors = bDisableLunaColors;
	RefreshMetrics();
	OnPropertyChanged();
}
BOOL CXTPTabPaintManager::IsLunaColorsDisabled() {
	return m_bDisableLunaColors || XTPColorManager()->IsLunaColorsDisabled();
}

COLORREF CXTPTabPaintManager::GetOneNoteColor(XTPTabOneNoteColor tabColor)
{
	const COLORREF clrTable[] =
	{
		RGB(138, 168, 228), // xtpTabColorBlue    : Blue tab color used when OneNote colors enabled.
		RGB(255, 219, 117), // xtpTabColorYellow  : Yellow tab color used when OneNote colors enabled.
		RGB(189, 205, 159), // xtpTabColorGreen   : Green tab color used when OneNote colors enabled.
		RGB(240, 158, 159), // xtpTabColorRed     : Red tab color used when OneNote colors enabled.
		RGB(186, 166, 225), // xtpTabColorPurple  : Purple tab color used when OneNote colors enabled.
		RGB(154, 191, 180), // xtpTabColorCyan    : Cyan tab color used when OneNote colors enabled.
		RGB(247, 182, 131), // xtpTabColorOrange  : Orange tab color used when OneNote colors enabled.
		RGB(216, 171, 192)  // xtpTabColorMagenta : Magenta tab color used when OneNote colors enabled.
	};

	return clrTable[tabColor-xtpTabColorBlue];
}

void CXTPTabPaintManager::SetOneNoteColors(BOOL bOneNoteColors /* = TRUE */)
{
	m_bOneNoteColors = bOneNoteColors;
	RefreshMetrics();
}

void CXTPTabPaintManager::FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc)
{
	m_pColorSet->FillNavigateButton(pDC, pButton, rc);
}

void CXTPTabPaintManager::DrawTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient)
{
	m_pAppearanceSet->DrawTabControl(pTabManager, pDC, rcClient);
}

void CXTPTabPaintManager::DrawTabControlEx(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient)
{
	if (pTabManager->IsDrawStaticFrame())
	{
		pDC->Draw3dRect(rcClient, GetColorSet()->m_clrFrameBorder, GetColorSet()->m_clrFrameBorder);
		CAppearanceSet::DeflateRectEx(rcClient, CRect(1, 1, 1, 1), pTabManager->GetPosition());
		pDC->Draw3dRect(rcClient, GetColorSet()->m_clrControlFace, GetColorSet()->m_clrControlFace);
		CAppearanceSet::DeflateRectEx(rcClient, CRect(1, 0, 1, 1), pTabManager->GetPosition());
	}

	if (m_bFillBackground)
	{
		pDC->FillSolidRect(rcClient, GetColorSet()->m_clrControlFace);
	}

	CAppearanceSet::DeflateRectEx(rcClient, m_rcControlMargin, pTabManager->GetPosition());

	pDC->SetBkMode(TRANSPARENT);


	CRect rc = m_bFillBackground? m_pAppearanceSet->FillTabControl(pTabManager, pDC, rcClient): m_pAppearanceSet->GetHeaderRect(rcClient, pTabManager);

	int i;

	for (i = pTabManager->GetNavigateButtonCount() - 1; i >= 0; i--)
	{
		if (pTabManager->GetNavigateButton(i)->IsVisible())
		{
			if (pTabManager->IsHorizontalPosition())
				rc.right = min(rc.right, pTabManager->GetNavigateButton(i)->GetRect().left);
			else
				rc.bottom = min(rc.bottom, pTabManager->GetNavigateButton(i)->GetRect().top);
		}
	}


	CRgn rgn;
	rgn.CreateRectRgnIndirect(rc);
	pDC->SelectClipRgn(&rgn);

	m_pColorSet->SelectClipRgn(pDC, rcClient, pTabManager);

	CXTPTabManagerItem* pSelected = NULL;

	for (i = pTabManager->GetItemCount() - 1; i >= 0; i--)
	{
		CXTPTabManagerItem* pItem = pTabManager->GetItem(i);

		CRect rcItem = pItem->GetRect();

		if (CRect().IntersectRect(rcClient, rcItem) && pItem->IsVisible())
		{
			m_pAppearanceSet->DrawSingleButton(pDC, pItem);

			if (pItem->IsSelected())
				pSelected = pItem;
		}
	}

	if (pSelected)
	{
		m_pAppearanceSet->DrawSingleButton(pDC, pSelected);
	}

	pDC->SelectClipRgn(NULL);

	for (i = pTabManager->GetNavigateButtonCount() - 1; i >= 0; i--)
		pTabManager->GetNavigateButton(i)->Draw(pDC);

}
int CXTPTabPaintManager::_GetButtonLength(int nValue, int nMin, int nMax)
{
	nValue += m_rcButtonMargin.left +  m_rcButtonMargin.right;
	if (nMin > 0) nValue = max(nMin, nValue);
	if (nMax > 0) nValue = min(nMax, nValue);
	return nValue;
}

void CXTPTabPaintManager::StripMnemonics(CString& strClear)
{
#if _MSC_VER >= 1200 // MFC 5.0
	strClear.Replace (_T("&&"), _T("\001\001"));
	strClear.Remove (_T('&'));
	strClear.Replace (_T("\001\001"), _T("&"));
#else
	for (int i = 0; i < strClear.GetLength(); i++)
		if (strClear[i] == _T('&') && (i == strClear.GetLength() - 1 || strClear[i+1] != _T('&')))
		strClear = strClear.Left(i) + strClear.Mid(i + 1);
#endif
}

int CXTPTabPaintManager::GetPrefixTextExtent(CDC* pDC, CString str, UINT uFormat)
{
	if (uFormat & DT_NOPREFIX)
		return pDC->GetTextExtent(str).cx;

	CString strClear = str;
	StripMnemonics(strClear);

	return pDC->GetTextExtent(strClear).cx;
}

void CXTPTabPaintManager::DrawTextPathEllipsis(CDC* pDC, CString strItem, CRect rcItem, UINT uFormat)
{
	if (strItem.Find(_T('\\')) != -1)
	{
		pDC->DrawText(strItem, rcItem,  uFormat|DT_PATH_ELLIPSIS);
		return;
	}

	LPCTSTR strEllipsis = _T("...");
	int cxEllipsis = pDC->GetTextExtent(strEllipsis, 3).cx;
	int cxItem = GetPrefixTextExtent(pDC, strItem, uFormat);

	if (cxItem <= rcItem.Width())
	{
		pDC->DrawText(strItem, rcItem, uFormat);
		return;
	}
	if (rcItem.Width() <= cxEllipsis)
	{
		pDC->DrawText(strEllipsis, rcItem, uFormat);
		return;
	}
	int nWidth = rcItem.Width() - cxEllipsis;

	int nResultLength = strItem.GetLength() * nWidth / cxItem;
	CString strResultLeft = strItem.Left(nResultLength - nResultLength / 2);
	CString strResultRight = strItem.Right(nResultLength / 2);

	while (GetPrefixTextExtent(pDC, strResultLeft, uFormat) + GetPrefixTextExtent(pDC, strResultRight, uFormat) < nWidth)
	{
		nResultLength ++;
		strResultLeft = strItem.Left(nResultLength - nResultLength / 2);
		strResultRight = strItem.Right(nResultLength / 2);
	}

	while (GetPrefixTextExtent(pDC, strResultLeft, uFormat) + GetPrefixTextExtent(pDC, strResultRight, uFormat) > nWidth)
	{
		nResultLength --;
		strResultLeft = strItem.Left(nResultLength - nResultLength / 2);
		strResultRight = strItem.Right(nResultLength / 2);
	}

	pDC->DrawText(strResultLeft + strEllipsis + strResultRight, rcItem, uFormat);
}

int CXTPTabPaintManager::DrawSingleButtonIconAndText(CDC* pDC, CXTPTabManagerItem* pItem, CRect rcItem, BOOL bDraw)
{
	CXTPTabManager* pManager = pItem->GetTabManager();

	XTPTabLayoutStyle layout = pManager->GetLayout();

	BOOL bVertical = !pManager->IsHorizontalPosition();
	CString strItem = pItem->GetCaption();
	CXTPFontDC fnt(pDC, m_bBoldSelected && pItem->IsSelected() && pManager->IsActive()? GetBoldFont(bVertical): GetFont(bVertical));


	CSize szIcon(m_szIcon);
	BOOL bDrawIcon = pManager->DrawIcon(pDC, 0, pItem, FALSE, szIcon);
	BOOL bDrawText = layout != xtpTabLayoutCompressed || pItem->IsSelected();

	if (bDraw)
	{
		m_pColorSet->SetTextColor(pDC, pItem);

		if (bVertical)
		{
			rcItem.top += 6 + m_rcButtonMargin.left;
			if (pManager->GetPosition() == xtpTabPositionRight)
				rcItem.DeflateRect(m_rcButtonMargin.bottom, 0, m_rcButtonMargin.top, 0);
			else
				rcItem.DeflateRect(m_rcButtonMargin.top, 0, m_rcButtonMargin.bottom, 0);

			if (bDrawIcon)
			{
				CPoint pt(rcItem.CenterPoint().x - szIcon.cx/2, rcItem.top - 1);
				if (rcItem.Height() > szIcon.cy) pManager->DrawIcon(pDC, pt, pItem, TRUE, szIcon);
				rcItem.top += szIcon.cy + 2;
			}
			if (bDrawText && rcItem.bottom > rcItem.top + 8)
			{
				CSize szText = pDC->GetTextExtent(strItem);
				rcItem.left = rcItem.right - (rcItem.Width() - szText.cy - (pManager->GetPosition() == xtpTabPositionRight? 1: 0)) / 2;
				rcItem.right = rcItem.left + rcItem.Height();
				rcItem.bottom -= 2 + m_rcButtonMargin.right;

				if (!m_bDrawTextNoPrefix)
					StripMnemonics(strItem);

				if (m_bDrawTextPathEllipsis)
				{
					DrawTextPathEllipsis(pDC, strItem, rcItem, DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX);
				}
				else
				{
					pDC->DrawText(strItem, &rcItem, DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX| DT_END_ELLIPSIS);
				}
			}

		} else
		{
			rcItem.left += 6 + m_rcButtonMargin.left;

			if (pManager->GetPosition() == xtpTabPositionBottom)
				rcItem.DeflateRect(0, m_rcButtonMargin.bottom, 0, m_rcButtonMargin.top);
			else
				rcItem.DeflateRect(0, m_rcButtonMargin.top, 0, m_rcButtonMargin.bottom);

			if (bDrawIcon)
			{
				CPoint pt(rcItem.left - 1, rcItem.CenterPoint().y - szIcon.cy/2);
				if (rcItem.Width() > szIcon.cx)  pManager->DrawIcon(pDC, pt, pItem, TRUE, szIcon);
				rcItem.left += szIcon.cx + 2;
			}

			if (bDrawText && rcItem.right > rcItem.left)
			{
				rcItem.right -= 2 + m_rcButtonMargin.right;
				if (m_bDrawTextPathEllipsis)
				{
					DrawTextPathEllipsis(pDC, strItem, rcItem,  DT_SINGLELINE | DT_LEFT| DT_VCENTER | (m_bDrawTextNoPrefix? DT_NOPREFIX: 0));
				}
				else
				{
					pDC->DrawText(strItem, rcItem,  DT_SINGLELINE | DT_LEFT| DT_VCENTER | (m_bDrawTextNoPrefix? DT_NOPREFIX: 0) | DT_END_ELLIPSIS);
				}
			}
		}
		return 0;
	}
	else
	{

		CSize szItem, szMin, szMax;
		pManager->GetItemMetrics(&szItem, &szMin, &szMax);

		if (layout == xtpTabLayoutFixed)
		{
			return _GetButtonLength(szItem.cx, szMin.cx, szMax.cx);
		}

		if (layout == xtpTabLayoutCompressed)
		{
			if (!bDrawText)
				return _GetButtonLength(8 + (bVertical? szIcon.cy: szIcon.cx), szMin.cx, szMax.cx) ;

			CXTPFontDC fnt(pDC, m_bBoldSelected? GetBoldFont(bVertical): GetFont(bVertical));

			int nLength = 0;

			for (int i = 0; i < pManager->GetItemCount(); i++)
			{
				CRect rcItem(0, 0, 0, 0);
				pDC->DrawText(pManager->GetItem(i)->GetCaption(), rcItem, DT_SINGLELINE | DT_CALCRECT | DT_NOCLIP | (m_bDrawTextNoPrefix? DT_NOPREFIX: 0));
				nLength = max(nLength, rcItem.Width());
			}
			return _GetButtonLength(nLength + 12 + (!bDrawIcon? 0: bVertical? szIcon.cy: szIcon.cx), szMin.cx, szMax.cx);
		}

		pDC->DrawText(strItem, rcItem,   DT_SINGLELINE | DT_CALCRECT | DT_NOCLIP | (m_bDrawTextNoPrefix? DT_NOPREFIX: 0));
		return _GetButtonLength(rcItem.Width() + 12 + (!bDrawIcon? 0: bVertical? szIcon.cy: szIcon.cx), szMin.cx, szMax.cx);
	}
}


void CXTPTabPaintManager::AdjustClientRect(CXTPTabManager* pTabManager, CRect& rcClient)
{
	if (pTabManager->IsDrawStaticFrame())
		CAppearanceSet::DeflateRectEx(rcClient, CRect(2, 1, 2, 2), pTabManager->GetPosition());

	if (pTabManager->GetItemCount() > 0)
	{
		m_pAppearanceSet->AdjustClientRect(pTabManager, rcClient);
	}
}

CRect CXTPTabPaintManager::RepositionNavigateButtons(CXTPTabManager* pTabManager, CRect rcClient)
{
	CRect rcNavigateButtons = m_pAppearanceSet->GetHeaderRect(rcClient, pTabManager);
	CRect rcHeaderMargin = m_pAppearanceSet->GetHeaderMargin();

	if (pTabManager->IsHorizontalPosition())
	{
		rcNavigateButtons.left += rcHeaderMargin.left + rcHeaderMargin.right;
		rcNavigateButtons.right -= 1;
	}
	else
	{
		rcNavigateButtons.top += rcHeaderMargin.left + rcHeaderMargin.right ;
		rcNavigateButtons.bottom -= 1;
	}

	for (int i = pTabManager->GetNavigateButtonCount() - 1; i >= 0; i--)
		pTabManager->GetNavigateButton(i)->Reposition(rcNavigateButtons);

	return rcNavigateButtons;
}

void CXTPTabPaintManager::RepositionTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient)
{
	pTabManager->m_rcHeaderRect.SetRectEmpty();
	pTabManager->m_rcClient = pTabManager->m_rcControl = rcClient;

	m_pAppearanceSet->RepositionTabControl(pTabManager, pDC, rcClient);
	AdjustClientRect(pTabManager, pTabManager->m_rcClient);
}

void CXTPTabPaintManager::RepositionTabControlEx(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient)
{
	if (pTabManager->IsDrawStaticFrame())
	{
		CAppearanceSet::DeflateRectEx(rcClient, CRect(2, 1, 2, 2), pTabManager->GetPosition());
	}

	CAppearanceSet::DeflateRectEx(rcClient, m_rcControlMargin, pTabManager->GetPosition());

	pTabManager->m_rcHeaderRect = m_pAppearanceSet->GetHeaderRect(rcClient, pTabManager);

	if (pTabManager->GetItemCount() == 0)
		return;

	CRect rcHeaderMargin = m_pAppearanceSet->GetHeaderMargin();
	int nButtonHeight = m_pAppearanceSet->GetButtonHeight(pTabManager);

	int i;

	if (pTabManager->IsHorizontalPosition())
	{

		CRect rcItem(0, rcClient.top + rcHeaderMargin.top, 0, 0);

		if (pTabManager->GetPosition() == xtpTabPositionBottom)
		{
			rcItem.top = rcClient.bottom - nButtonHeight - rcHeaderMargin.top;
		}

		rcItem.bottom = rcItem.top + nButtonHeight;

		for (i = 0; i < pTabManager->GetItemCount(); i++)
		{
			CXTPTabManagerItem* pItem = pTabManager->GetItem(i);

			pItem->m_nLength = pItem->IsVisible()? m_pAppearanceSet->GetButtonLength(pDC, pItem): 0;
			pItem->m_rcItem = rcItem;
		}

		int nWidth = rcClient.Width() - (rcHeaderMargin.right + rcHeaderMargin.left);

		for (i = pTabManager->GetNavigateButtonCount() - 1; i >= 0; i--)
			pTabManager->GetNavigateButton(i)->AdjustWidth(nWidth);

		if (pTabManager->GetLayout() == xtpTabLayoutSizeToFit)
			SizeToFit(pTabManager, nWidth);

		if (pTabManager->m_nHeaderOffset < 0)
		{
			CRect rcNavigateButtons = RepositionNavigateButtons(pTabManager, rcClient);
			int nLength = pTabManager->GetItemsLength();

			if (nLength + pTabManager->m_nHeaderOffset < rcNavigateButtons.Width())
			{
				pTabManager->m_nHeaderOffset = min(0, rcNavigateButtons.Width() - nLength);
			}
		}

		int x = rcClient.left + rcHeaderMargin.left + pTabManager->GetHeaderOffset();
		for (i = 0; i < pTabManager->GetItemCount(); i++)
		{
			CXTPTabManagerItem* pItem = pTabManager->GetItem(i);

			pItem->m_rcItem.left = x;
			x = pItem->m_rcItem.right = x + pItem->m_nLength;
		}

		pTabManager->m_rcHeaderRect.right = RepositionNavigateButtons(pTabManager, rcClient).right;

	}
	else
	{
		CRect rcItem(rcClient.left + rcHeaderMargin.top, 0, 0, 0);

		if (pTabManager->GetPosition() == xtpTabPositionRight)
		{
			rcItem.left = rcClient.right - nButtonHeight - rcHeaderMargin.top;
		}

		rcItem.right = rcItem.left + nButtonHeight;

		for (i = 0; i < pTabManager->GetItemCount(); i++)
		{
			CXTPTabManagerItem* pItem = pTabManager->GetItem(i);

			pItem->m_nLength = pItem->IsVisible()? m_pAppearanceSet->GetButtonLength(pDC, pItem): 0;
			pItem->m_rcItem = rcItem;
		}

		int nWidth = rcClient.Height() - (rcHeaderMargin.right + rcHeaderMargin.left);

		for (i = pTabManager->GetNavigateButtonCount() - 1; i >= 0; i--)
			pTabManager->GetNavigateButton(i)->AdjustWidth(nWidth);

		if (pTabManager->GetLayout() == xtpTabLayoutSizeToFit)
			SizeToFit(pTabManager, nWidth);

		if (pTabManager->m_nHeaderOffset < 0)
		{
			CRect rcNavigateButtons = RepositionNavigateButtons(pTabManager, rcClient);
			int nLength = pTabManager->GetItemsLength();

			if (nLength + pTabManager->m_nHeaderOffset < rcNavigateButtons.Height())
			{
				pTabManager->m_nHeaderOffset = min(0, rcNavigateButtons.Height() - nLength);
			}
		}

		int y = rcClient.top + rcHeaderMargin.left + pTabManager->GetHeaderOffset();
		for (i = 0; i < pTabManager->GetItemCount(); i++)
		{
			CXTPTabManagerItem* pItem = pTabManager->GetItem(i);

			pItem->m_rcItem.top = y;
			y = pItem->m_rcItem.bottom = y + pItem->m_nLength;
		}

		pTabManager->m_rcHeaderRect.bottom = RepositionNavigateButtons(pTabManager, rcClient).bottom;

	}
}

void CXTPTabPaintManager::EnsureVisible(CXTPTabManager* pTabManager, CXTPTabManagerItem* pItem)
{
	CRect rcHeader = pTabManager->m_rcHeaderRect;
	CRect rcItem = pItem->GetRect();

	if (rcHeader.IsRectEmpty() || rcItem.IsRectEmpty())
		return;

	if (pTabManager->IsHorizontalPosition())
	{
		if (rcItem.left < rcHeader.left)
		{
			pTabManager->SetHeaderOffset(pTabManager->GetHeaderOffset() - rcItem.left + rcHeader.left + m_pAppearanceSet->GetHeaderMargin().left);
		}
		else if (rcItem.right > rcHeader.right)
		{
			pTabManager->SetHeaderOffset(pTabManager->GetHeaderOffset() - rcItem.right + rcHeader.right);
		}

	} else
	{
		if (rcItem.top < rcHeader.top)
		{
			pTabManager->SetHeaderOffset(pTabManager->GetHeaderOffset() - rcItem.top + rcHeader.top + m_pAppearanceSet->GetHeaderMargin().left);
		}
		else if (rcItem.bottom > rcHeader.bottom)
		{
			pTabManager->SetHeaderOffset(pTabManager->GetHeaderOffset() - rcItem.bottom + rcHeader.bottom);
		}

	}

}

class CXTPTabPaintManager::CSizeToFitSorter
{
public:

	static int _cdecl CompareRect(const void *arg1, const void *arg2 )
	{
		int nLength1 = s_pTabManager->GetItem(*(int*)arg1)->GetLength();
		int nLength2 = s_pTabManager->GetItem(*(int*)arg2)->GetLength();
		if (nLength1 > nLength2) return -1;
		if (nLength1 < nLength2) return 1;

		return 0;
	}
	static CXTPTabManager* s_pTabManager;
};
CXTPTabManager* CXTPTabPaintManager::CSizeToFitSorter::s_pTabManager;

void CXTPTabPaintManager::SizeToFit(CXTPTabManager* pTabManager, int nWidth)
{
	#define GetItemLength(nIndex) pTabManager->GetItem(pIndex[nIndex])->m_nLength

	int nLength = pTabManager->GetItemsLength();
	int nCount = pTabManager->GetItemCount();

	if (nWidth < nLength)
	{
		if (nCount == 1)
			pTabManager->GetItem(0)->m_nLength = nWidth;
		else
		{
			int* pIndex = new int[nCount];
			for (int c =  0; c < nCount; c++) pIndex[c] = c;

			CSizeToFitSorter::s_pTabManager = pTabManager;
			qsort(pIndex, pTabManager->GetItemCount(), sizeof(int), CSizeToFitSorter::CompareRect);

			int nDelta = nLength - nWidth;

			for (int i = 0; i < nCount;)
			{
				int nItemWidth = GetItemLength(i);

				int k = i + 1;
				while( k < nCount && GetItemLength(k) == nItemWidth) k++;

				if (k < nCount)
				{
					int nNextWidth = GetItemLength(k);
					int nCanFree = k  * (nItemWidth - nNextWidth);

					if (nCanFree >= nDelta)
					{
						for (int j = 0; j < k; j++)
							GetItemLength(j) -= nDelta / k;
						break;

					} else
					{
						nDelta -= nCanFree;
						for (int j = 0; j < k; j++)
							GetItemLength(j) =  nNextWidth;
					}
				} else
				{
					for (int j = 0; j < nCount; j++)
						GetItemLength(j) =  nWidth/nCount;
					break;
				}
				i = k;
			}

			delete pIndex;
		}
	}
}

XTPTabAppearanceStyle CXTPTabPaintManager::GetAppearance()
{
	return m_tabAppearance;
}

XTPTabColorStyle CXTPTabPaintManager::GetColor()
{
	return m_tabColor;
}

void CXTPTabPaintManager::SetPosition(XTPTabPosition tabPosition)
{
	m_tabPosition = tabPosition;
	OnPropertyChanged();

}

void CXTPTabPaintManager::SetLayout(XTPTabLayoutStyle tabLayout)
{
	m_tabLayout = tabLayout;
	OnPropertyChanged();
}

XTPTabPosition CXTPTabPaintManager::GetPosition()
{
	return m_tabPosition;
}

XTPTabLayoutStyle CXTPTabPaintManager::GetLayout()
{
	return m_tabLayout;
}


void CXTPTabPaintManager::OnPropertyChanged()
{
	for (int i = 0; i < m_arrObservers.GetSize(); i++)
	{
		m_arrObservers[i]->OnPropertyChanged();
	}
}


void CXTPTabPaintManager::AddObserver(CXTPTabManagerAtom* pObserver)
{
	for (int i = 0; i < m_arrObservers.GetSize(); i++)
	{
		if (m_arrObservers[i] == pObserver)
			return;
	}
	m_arrObservers.Add(pObserver);
}

//////////////////////////////////////////////////////////////////////////

