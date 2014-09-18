// XTPPaintManager.cpp : implementation of the CXTPPaintManager class.
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
#include "XTPImageManager.h"
#include "XTPColorManager.h"
#include "XTPDrawHelpers.h"

#include "XTPPaintManager.h"
#include "XTPControlButton.h"
#include "XTPControlPopup.h"
#include "XTPToolBar.h"
#include "XTPCommandBars.h"
#include "XTPPopupBar.h"
#include "XTPControlComboBox.h"
#include "XTPShadowsManager.h"
#include "XTPControlEdit.h"
#include "XTPControls.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace XTPPaintThemes;

CXTPPaintManager* CXTPPaintManager::s_pInstance = NULL;

//////////////////////////////////////////////////////////////////////
// CXTPPaintManager::CXTPPaintManagerDestructor

static class CXTPPaintManager::CXTPPaintManagerDestructor
{
public:
	~CXTPPaintManagerDestructor()
	{
		CXTPPaintManager::Done();
	}

} __xtpPaintManagerDestructor;


///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPanePaintManager::CLogFont

CXTPPaintManager::CLogFont::CLogFont()
{
	::ZeroMemory(this, sizeof(LOGFONT));
}

///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPanePaintManager::CNonClientMetrics

CXTPPaintManager::CNonClientMetrics::CNonClientMetrics()
{
	::ZeroMemory(this, sizeof(NONCLIENTMETRICS));
	cbSize = sizeof(NONCLIENTMETRICS);
	VERIFY(::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS), this, 0));
}

CXTPPaintManager::CXTPPaintManager(void)
{
	m_nTextHeight = 0;
	m_bUseOfficeFont = FALSE;
	m_bUseStandardFont = TRUE;

	m_iconsInfo.bUseDisabledIcons = FALSE;
	m_iconsInfo.bUseFadedIcons = FALSE;
	m_iconsInfo.bIconsWithShadow = FALSE;

	m_nAnimationSteps = 10;
	m_nAnimationTime = 100;

	m_systemTheme = xtpSystemThemeDefault;

	RefreshMetrics();
}

XTPCurrentSystemTheme CXTPPaintManager::GetCurrentSystemTheme()
{
	if (m_systemTheme == xtpSystemThemeDefault)
		return XTPColorManager()->IsLunaColorsDisabled()? xtpSystemThemeUnknown:
			XTPColorManager()->GetCurrentSystemTheme();

	if (m_systemTheme == xtpSystemThemeAuto)
		return XTPColorManager()->GetWinThemeWrapperTheme();

	return m_systemTheme;
}
void CXTPPaintManager::SetLunaTheme(XTPCurrentSystemTheme systemTheme)
{
	m_systemTheme = systemTheme;
	RefreshMetrics();

}


COLORREF CXTPPaintManager::GetXtremeColor(UINT nIndex)
{
	if (nIndex > XPCOLOR_LAST) return nIndex;
	return m_arrColor[nIndex];
}

void CXTPPaintManager::SetColors(int cElements, CONST INT *lpaElements, CONST COLORREF *lpaRgbValues)
{
	for (int i = 0; i < cElements; i++)
		m_arrColor[lpaElements[i]].SetStandardValue(lpaRgbValues[i]);
}


CXTPPaintManager::~CXTPPaintManager(void)
{
}

XTP_COMMANDBARS_ICONSINFO* CXTPPaintManager::GetIconsInfo()
{
	return &m_iconsInfo;
}

void CXTPPaintManager::RefreshMetrics()
{
	UpdateFonts();

	CXTPColorManager* pColorManager = XTPColorManager();
	pColorManager->RefreshColors();

	for (int i = 0; i <= XPCOLOR_LAST; i++)
		m_arrColor[i].SetStandardValue(pColorManager->GetColor(i));

	m_clrShadowFactor = 0;
	m_clrStatusTextColor = GetXtremeColor(COLOR_BTNTEXT);
}

int CXTPPaintManager::GetControlHeight()
{
	return max(22, m_nTextHeight);
}

CXTPPaintManager* CXTPPaintManager::CreateTheme(XTPPaintTheme paintTheme)
{
	CXTPPaintManager* pInstance = NULL;

	if (paintTheme == xtpThemeOfficeXP) pInstance = new CXTPOfficeTheme();
	else if (paintTheme == xtpThemeOffice2003)  pInstance = new CXTPOffice2003Theme();
	else if (paintTheme == xtpThemeNativeWinXP) pInstance = new CXTPNativeXPTheme();
	else if (paintTheme == xtpThemeWhidbey) pInstance = new CXTPWhidbeyTheme();
	else pInstance = new CXTPDefaultTheme();

	ASSERT(pInstance);

	pInstance->m_themeCurrent = paintTheme;

	return pInstance;
}

void CXTPPaintManager::SetTheme(XTPPaintTheme paintTheme)
{
	if (s_pInstance != NULL)
		s_pInstance->InternalRelease();

	s_pInstance = CreateTheme(paintTheme);
}

void CXTPPaintManager::SetCustomTheme(CXTPPaintManager* pTheme)
{
	if (s_pInstance != NULL)
		s_pInstance->InternalRelease();

	s_pInstance = pTheme;

	s_pInstance->m_themeCurrent = xtpThemeCustom;
}


void CXTPPaintManager::Done()
{
	if (s_pInstance != NULL)
		s_pInstance->InternalRelease();
	s_pInstance = NULL;
}



void CXTPPaintManager::Triangle(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2, COLORREF clr)
{
	CXTPPenDC pen (*pDC, clr);
	CXTPBrushDC brush (*pDC, clr);

	CPoint pts[3];
	pts[0] = pt0;
	pts[1] = pt1;
	pts[2] = pt2;
	pDC->Polygon(pts, 3);
}
void CXTPPaintManager::Rectangle(CDC* pDC, CRect rc, int nPen, int nBrush)
{
	pDC->FillSolidRect(rc, GetXtremeColor(nBrush));
	pDC->Draw3dRect(rc, GetXtremeColor(nPen), GetXtremeColor(nPen));
}

void CXTPPaintManager::Draw3dRect(CDC* pDC, CRect rc, int nTopLeft, int nBottomRight)
{
	pDC->Draw3dRect(&rc, GetXtremeColor(nTopLeft), GetXtremeColor(nBottomRight));
}


void CXTPPaintManager::DrawCheckMark(CDC* pDC, CRect rc, COLORREF clr, BOOL bLayoutRTL /*= FALSE*/)
{
	CRect rcCheck(CPoint(rc.left + rc.Width()/2 - 10, rc.top + rc.Height()/2 - 10), CSize(20, 20));

	CXTPPenDC pen(*pDC, clr);

	if (bLayoutRTL)
	{
		pDC->MoveTo(rcCheck.left + 6, rcCheck.top + 7);
		pDC->LineTo(rcCheck.left + 10, rcCheck.top + 11);
		pDC->LineTo(rcCheck.left + 13, rcCheck.top + 8);

		pDC->MoveTo(rcCheck.left + 6, rcCheck.top + 8);
		pDC->LineTo(rcCheck.left + 10, rcCheck.top + 12);
		pDC->LineTo(rcCheck.left + 13, rcCheck.top + 9);
	}
	else
	{
		pDC->MoveTo(rcCheck.left + 6, rcCheck.top + 9);
		pDC->LineTo(rcCheck.left + 8, rcCheck.top + 11);
		pDC->LineTo(rcCheck.left + 13, rcCheck.top + 6);

		pDC->MoveTo(rcCheck.left + 6, rcCheck.top + 10);
		pDC->LineTo(rcCheck.left + 8, rcCheck.top + 12);
		pDC->LineTo(rcCheck.left + 13, rcCheck.top + 7);
	}

}
void CXTPPaintManager::Line(CDC* pDC, CPoint p0, CPoint p1)
{
	pDC->MoveTo(p0);
	pDC->LineTo(p1);
}
void CXTPPaintManager::Line(CDC* pDC, int x0, int y0, int x1, int y1, int nPen)
{
	CXTPPenDC pen(*pDC, GetXtremeColor(nPen));
	pDC->MoveTo(x0, y0);
	pDC->LineTo(x1, y1);
}
void CXTPPaintManager::Pixel(CDC* pDC, int x, int y, int nPen)
{
	pDC->SetPixel(x, y, GetXtremeColor(nPen));
}

void CXTPPaintManager::StripMnemonics(CString& strClear)
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

CSize CXTPPaintManager::DrawTextEx(CDC* pDC, CString str, CRect rcText, BOOL bDraw, BOOL bVert, BOOL bCentered, BOOL bTrangled, BOOL bEmbosssed)
{
	CString strClear(str);
	StripMnemonics(strClear);

	if (!bDraw)
	{
		CSize sz = pDC->GetTextExtent(strClear.IsEmpty()? _T(" "): strClear);
		if (bTrangled) sz.cx += 10;
		return bVert? CSize(sz.cy, sz.cx): sz;
	}

	if (!bVert)
	{
		int nWidth = pDC->GetTextExtent(strClear).cx + (bTrangled? 5: 0);

		if (bCentered)
		{
			rcText.left += (rcText.Width() - nWidth) /2;
		}

	#ifdef _EMBOSSED_STYLE
		if (bDraw && bEmbosssed)
		{
			COLORREF clrText = pDC->GetTextColor();
			pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));
			pDC->DrawText(str, rcText + CPoint(1, 1), DT_SINGLELINE|DT_VCENTER|DT_LEFT);
			pDC->SetTextColor(clrText);
		}
	#endif

		pDC->DrawText(str, &rcText, DT_SINGLELINE|DT_VCENTER|DT_LEFT);

		if (bTrangled)
		{
			CPoint pt = CPoint(rcText.left + nWidth, rcText.CenterPoint().y);
			Triangle(pDC, CPoint(pt.x -2 , pt.y - 1), CPoint(pt.x + 2, pt.y - 1), CPoint (pt.x, pt.y + 1), pDC->GetTextColor());
		}
	}
	else
	{
		CRect rc(rcText);
		CSize szText = pDC->GetTextExtent (strClear);
		szText.cx += (bTrangled? 5: 0);
		rcText.left = rcText.right - (rcText.Width() - szText.cy + 1) / 2;
		if (bCentered)
		{
			rcText.top = rcText.top + (rcText.Height() - szText.cx + 1) / 2;
		}
	#ifdef _EMBOSSED_STYLE
		if (bDraw && bEmbosssed)
		{
			COLORREF clrText = pDC->GetTextColor();
			pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));
			pDC->DrawText(strClear, rcText + CPoint(1, 1), DT_NOPREFIX|DT_SINGLELINE | DT_NOCLIP);
			pDC->SetTextColor(clrText);
		}
	#endif

		pDC->DrawText(strClear, &rcText, DT_NOPREFIX|DT_SINGLELINE | DT_NOCLIP);

		if (bTrangled)
		{
			CPoint pt = CPoint(rc.CenterPoint().x, rcText.top + szText.cx - 1);
			Triangle(pDC, CPoint(pt.x +2 , pt.y - 2), CPoint(pt.x + 2, pt.y + 2), CPoint (pt.x, pt.y), pDC->GetTextColor());
		}
	}
	return 0;
}

void CXTPPaintManager::UseOfficeFont(bool bUseOfficeFont)
{
	m_bUseOfficeFont = bUseOfficeFont;
	RefreshMetrics();
}

static int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX* pelf,
	NEWTEXTMETRICEX* lpntm, int FontType, LPVOID pThis)
{
	UNREFERENCED_PARAMETER( lpntm );
	UNREFERENCED_PARAMETER( FontType );

	CString strFontName = *( CString* )pThis;
	CString strFaceName = pelf->elfLogFont.lfFaceName;

	if ( strFaceName.CompareNoCase( strFontName ) == 0 )
	{
		return 0;
	}

	return 1;
}

bool CXTPPaintManager::FontExists(CString& strFaceName)
{
	// Enumerate all styles and charsets of all fonts:
	CLogFont lfEnum;

	lfEnum.lfFaceName[ 0 ] = 0;
	lfEnum.lfCharSet = DEFAULT_CHARSET;

	// See if lpszFaceName font exists, if so, then use it...
	CWindowDC dc( NULL );
	if ( ::EnumFontFamiliesEx(
		dc.m_hDC, &lfEnum, ( FONTENUMPROC )EnumFontFamExProc, ( LPARAM )&strFaceName, 0 ) == 0 )
	{
		return true;
	}

	return false;
}

void CXTPPaintManager::SetCommandBarsFontIndirect(LOGFONT* pLogFont, BOOL bUseStandardFont /*= FALSE*/)
{
	m_bUseStandardFont = bUseStandardFont;

	if (!pLogFont)
		return;

	m_fontRegular.DeleteObject();
	m_fontRegularBold.DeleteObject();
	m_fontVert.DeleteObject();
	m_fontVertBold.DeleteObject();

	// see if the Tahoma font was found.
	CString strOfficeFont(_T("Tahoma"));
	bool bTahomaExists = FontExists( strOfficeFont );

	// see if the system font is set to the Tahoma font, if so set the office font flag to true.
	bool bTahomaLF = strOfficeFont.CompareNoCase(pLogFont->lfFaceName) == 0;
	if (bTahomaLF) {
		m_bUseOfficeFont = true;
	}

	// set the office font if applicable.
	BOOL bUseSystemFont = (pLogFont->lfCharSet > SYMBOL_CHARSET);
	if (!bUseSystemFont && bUseStandardFont)
	{
		if ( m_bUseOfficeFont && bTahomaExists )
			STRCPY_S(pLogFont->lfFaceName, LF_FACESIZE, strOfficeFont);
	}

	m_fontRegular.CreateFontIndirect (pLogFont);

	int nWeight = pLogFont->lfWeight;

	pLogFont->lfWeight = FW_BOLD;
	m_fontRegularBold.CreateFontIndirect (pLogFont);


	pLogFont->lfOrientation = 900;
	pLogFont->lfEscapement = 2700;
	pLogFont->lfWeight = nWeight;
	STRCPY_S(pLogFont->lfFaceName, LF_FACESIZE, XTPDrawHelpers()->GetVerticalFontName(m_bUseOfficeFont));

	m_fontVert.CreateFontIndirect (pLogFont);

	pLogFont->lfWeight = FW_BOLD;
	m_fontVertBold.CreateFontIndirect (pLogFont);

	CWindowDC dc (NULL);
	CXTPFontDC font(&dc, &m_fontRegular);
	m_nTextHeight = dc.GetTextExtent(_T(" ")).cy + 6;
}

void CXTPPaintManager::UpdateFonts()
{

	CNonClientMetrics ncm;

	if (m_bUseStandardFont)
	{
		CLogFont lf;
		{
			CWindowDC dc (NULL);
			lf.lfCharSet = (BYTE)::GetTextCharsetInfo (dc, NULL, 0);
		}
		lf.lfHeight = ncm.lfMenuFont.lfHeight < 0? min(-11, ncm.lfMenuFont.lfHeight): ncm.lfMenuFont.lfHeight;
		lf.lfWeight = ncm.lfMenuFont.lfWeight;
		lf.lfItalic = ncm.lfMenuFont.lfItalic;

		STRCPY_S(lf.lfFaceName, LF_FACESIZE, ncm.lfMenuFont.lfFaceName);

		SetCommandBarsFontIndirect(&lf, TRUE);
	}

	m_fontSmCaption.DeleteObject();
	m_fontIcon.DeleteObject();
	m_fontMarlettIcon.DeleteObject();
	m_fontIconBold.DeleteObject();

	m_fontSmCaption.CreateFontIndirect (&ncm.lfSmCaptionFont);

	LOGFONT lfIcon;
	VERIFY( ::SystemParametersInfo( SPI_GETICONTITLELOGFONT, sizeof( lfIcon ), &lfIcon, 0 ) );
	VERIFY( m_fontIcon.CreateFontIndirect(&lfIcon  ) );

	LOGFONT lfMarlettIcon = {lfIcon.lfHeight, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _T("Marlett")};
	m_fontMarlettIcon.CreateFontIndirect(&lfMarlettIcon);

	lfIcon.lfWeight = FW_BOLD;
	VERIFY( m_fontIconBold.CreateFontIndirect(&lfIcon ) );

}

void CXTPPaintManager::DrawControlEntry(CDC* pDC, CXTPControl* pButton)
{
	BOOL bPopuped = IsPopupControlType(pButton->GetType()) && ((CXTPControlPopup*)pButton)->GetPopup();
	DrawControlEntry(pDC, pButton->GetRect(), pButton->GetEnabled(), pButton->GetSelected(), pButton->GetPressed(), pButton->GetChecked(), bPopuped, pButton->GetParent()->GetPosition());
}

CSize CXTPPaintManager::DrawControl(CDC* pDC, CXTPControl* pButton, BOOL bDraw)
{
	if (pButton->GetType() == xtpControlEdit)
	{
		CEdit* pEdit = ((CXTPControlEdit*)pButton)->GetEditCtrl();
		if (pEdit && pEdit->GetSafeHwnd() && pEdit->GetFont()!= &m_fontIcon)
		{
			pEdit->SetFont(&m_fontIcon, TRUE);
			pEdit->SetMargins(0, 0);
		}
		return DrawControlEdit(pDC, (CXTPControlEdit*)pButton, bDraw);
	}

	if (pButton->GetType() == xtpControlComboBox)
	{
		CEdit* pEdit = ((CXTPControlComboBox*)pButton)->GetEditCtrl();
		if (pEdit && pEdit->GetSafeHwnd() && pEdit->GetFont()!= &m_fontIcon)
		{
			pEdit->SetFont(&m_fontIcon, TRUE);
			pEdit->SetMargins(0, 0);
		}

		return DrawControlComboBox(pDC, (CXTPControlComboBox*)pButton, bDraw);
	}

	return (pButton->GetParent()->GetType() == xtpBarTypePopup)?
		DrawControlPopupParent(pDC, pButton, bDraw): DrawControlToolBarParent(pDC, pButton, bDraw);
}

void CXTPPaintManager::FillDockBar(CDC* pDC, CXTPDockBar* pBar)
{
	pDC->FillSolidRect(CXTPClientRect((CWnd*)pBar), GetXtremeColor(COLOR_3DFACE));
}

void CXTPPaintManager::FillStatusBar(CDC* pDC, CStatusBar* pBar)
{
	pDC->FillSolidRect(CXTPClientRect(pBar), GetXtremeColor(COLOR_3DFACE));
}

CFont* CXTPPaintManager::GetCommandBarFont(CXTPCommandBar* pBar, BOOL bBold)
{
	return (pBar->GetPosition() == xtpBarLeft || pBar->GetPosition() == xtpBarRight)? (bBold? &m_fontVertBold: &m_fontVert): (bBold? &m_fontRegularBold: &m_fontRegular);

}

CSize CXTPPaintManager::DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType, CXTPControl* pButton, CXTPCommandBar* pBar, BOOL bDraw, LPVOID lpParam)
{
	if (controlType == xtpButtonExpandToolbar)
	{
		if (!bDraw)
		{
			CSize szBar = *(CSize*)lpParam;
			if (!IsVerticalPosition(pBar->GetPosition()))
				pButton->SetRect(CRect(szBar.cx - (11 + 2), 2, szBar.cx - 2, szBar.cy - 2));
			else
				pButton->SetRect(CRect(2, szBar.cy - (11 + 2), szBar.cx - 2, szBar.cy - 2));
		} else
		{
			DrawControlEntry(pDC, pButton);
			COLORREF clrText = pDC->GetTextColor();
			BOOL bHiddenExists = *(BOOL*)lpParam;
			CRect rcButton = pButton->GetRect();
			if (!IsVerticalPosition(pBar->GetPosition()))
			{
				CPoint pt = CPoint(rcButton.left + rcButton.Width()/2, rcButton.bottom - 6);
				Triangle(pDC, CPoint(pt.x -2 , pt.y - 1), CPoint(pt.x + 2, pt.y - 1), CPoint (pt.x, pt.y + 1), clrText);
				if (bHiddenExists)
				{
					CXTPPenDC pen(*pDC, clrText);
					pDC->MoveTo(rcButton.left + 2, rcButton.top + 4); pDC->LineTo(rcButton.left + 4, rcButton.top + 6);     pDC->LineTo(rcButton.left + 1, rcButton.top + 9);
					pDC->MoveTo(rcButton.left + 3, rcButton.top + 4); pDC->LineTo(rcButton.left + 5, rcButton.top + 6);     pDC->LineTo(rcButton.left + 2, rcButton.top + 9);

					pDC->MoveTo(rcButton.left + 6, rcButton.top + 4); pDC->LineTo(rcButton.left + 8, rcButton.top + 6);     pDC->LineTo(rcButton.left + 5, rcButton.top + 9);
					pDC->MoveTo(rcButton.left + 7, rcButton.top + 4); pDC->LineTo(rcButton.left + 9, rcButton.top + 6);     pDC->LineTo(rcButton.left + 6, rcButton.top + 9);
				}
			} else
			{
				CPoint pt = CPoint(rcButton.left + 4, rcButton.top + 5);
				Triangle(pDC, CPoint(pt.x +2 , pt.y - 2), CPoint(pt.x + 2, pt.y + 2), CPoint (pt.x, pt.y), clrText);

				if (bHiddenExists)
				{
					CXTPPenDC pen(*pDC, clrText);
					pDC->MoveTo(rcButton.right - 8, rcButton.top + 2); pDC->LineTo(rcButton.right - 6, rcButton.top + 4);   pDC->LineTo(rcButton.right - 3, rcButton.top + 1);
					pDC->MoveTo(rcButton.right - 8, rcButton.top + 3); pDC->LineTo(rcButton.right - 6, rcButton.top + 5);   pDC->LineTo(rcButton.right - 3, rcButton.top + 2);

					pDC->MoveTo(rcButton.right - 8, rcButton.top + 6); pDC->LineTo(rcButton.right - 6, rcButton.top + 8);   pDC->LineTo(rcButton.right - 3, rcButton.top + 5);
					pDC->MoveTo(rcButton.right - 8, rcButton.top + 7); pDC->LineTo(rcButton.right - 6, rcButton.top + 9);   pDC->LineTo(rcButton.right - 3, rcButton.top + 6);
				}

			}
		}
	}
	else if (controlType == xtpButtonExpandFloating)
	{
		DrawControlEntry(pDC, pButton);
		CRect rcButton = pButton->GetRect();
		COLORREF clrText = pDC->GetTextColor();

		if (!pButton->GetSelected() && !((CXTPControlPopup*)pButton)->GetPopup())
			clrText = GetXtremeColor(COLOR_HIGHLIGHTTEXT);
		CPoint pt = CPoint(rcButton.left + rcButton.Width()/2, rcButton.top + rcButton.Height()/2);
		Triangle(pDC, CPoint(pt.x -3 , pt.y - 2), CPoint(pt.x + 3, pt.y - 2), CPoint (pt.x, pt.y + 1), clrText);
	}
	else if (controlType == xtpButtonHideFloating)
	{
		CFont font;
		font.CreatePointFont(MulDiv(80, 96, pDC->GetDeviceCaps(LOGPIXELSX)), _T("Marlett"));
		CFont* pFont = pDC->SelectObject(&font);
		DrawControlEntry(pDC, pButton);
		if (!pButton->GetPressed() && !pButton->GetSelected())
			pDC->SetTextColor(GetXtremeColor(COLOR_HIGHLIGHTTEXT))  ;

		pDC->DrawText(_T("r"), pButton->GetRect(), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		pDC->SelectObject(pFont);
	}
	else if (controlType == xtpButtonExpandMenu)
	{
		if (bDraw)
		{
			DrawControlEntry(pDC, pButton);
			CRect rcButton = pButton->GetRect();
			int nCenter = rcButton.CenterPoint().x;

			pDC->MoveTo(nCenter - 2, rcButton.top + 2); pDC->LineTo(nCenter, rcButton.top + 4);     pDC->LineTo(nCenter + 3, rcButton.top + 1);
			pDC->MoveTo(nCenter - 2, rcButton.top + 3); pDC->LineTo(nCenter, rcButton.top + 5);     pDC->LineTo(nCenter + 3, rcButton.top + 2);

			pDC->MoveTo(nCenter - 2, rcButton.top + 6); pDC->LineTo(nCenter, rcButton.top + 8);     pDC->LineTo(nCenter + 3, rcButton.top + 5);
			pDC->MoveTo(nCenter - 2, rcButton.top + 7); pDC->LineTo(nCenter, rcButton.top + 9);     pDC->LineTo(nCenter + 3, rcButton.top + 6);

		}
		return CSize(0, 12);
	}
	return 0;
}

CSize CXTPPaintManager::_DrawButtonText(CDC* pDC, CString strText, CRect rcText, BOOL bVert, BOOL bDraw, BOOL bEmbossed, BOOL bTriangled, CSize szButton, BOOL bDrawImage)
{
	if (!bVert)
	{
		rcText.left += (bDrawImage? szButton.cx - 1: 4);

		CSize sz = DrawTextEx(pDC, strText, &rcText, bDraw, bVert, FALSE, bTriangled, bEmbossed);

		return CSize(3 + sz.cx  + ((bDrawImage)? szButton.cx: 4),
			max(sz.cy + 6, szButton.cy));

	} else
	{
		rcText.top += (bDrawImage? szButton.cy: 4);

		CSize sz = DrawTextEx(pDC, strText, &rcText, bDraw, bVert, FALSE, bTriangled, bEmbossed);

		return CSize(max(sz.cx + 6, szButton.cx),
			3 + sz.cy  + ((bDrawImage)? szButton.cx: 4));

	}
}

void CXTPPaintManager::DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton, CRect rcButton)
{
	BOOL bControlPopup = DYNAMIC_DOWNCAST(CXTPControlPopup, pButton) != NULL;

	BOOL bPressed = pButton->GetPressed(), bSelected = pButton->GetSelected(), bEnabled = pButton->GetEnabled(), bChecked = pButton->GetChecked(),
		bPopuped = bControlPopup && ((CXTPControlPopup*)pButton)->GetPopup();

	DrawControlEntry(pDC, CRect(rcButton.left, rcButton.top, rcButton.right - (bPopuped? 0: 11), rcButton.bottom),
		bEnabled, bSelected, bPressed, bChecked, bPopuped, pButton->GetParent()->GetPosition());

	if ((bSelected || bPressed) && !bPopuped && bEnabled)
		DrawControlEntry(pDC, CRect(rcButton.right - 12, rcButton.top, rcButton.right, rcButton.bottom), TRUE, TRUE, FALSE, FALSE, FALSE, pButton->GetParent()->GetPosition());

	CPoint pt = CPoint(rcButton.right - 7, rcButton.CenterPoint().y);
	Triangle(pDC, CPoint(pt.x - 2 , pt.y - 1), CPoint(pt.x + 2, pt.y - 1), CPoint (pt.x, pt.y + 1), pDC->GetTextColor());
}

CSize CXTPPaintManager::DrawControlToolBarParent(CDC* pDC, CXTPControl* pButton, BOOL bDraw)
{
	CXTPFontDC font(pDC, GetCommandBarFont(pButton->GetParent(), pButton->IsItemDefault()));

	CRect rcButton = bDraw? pButton->GetRect(): CXTPEmptyRect();
	XTPControlType controlType = pButton->GetType();

	CSize szIcon = pButton->GetParent()->GetIconSize();
	CSize szButton = pButton->GetParent()->GetButtonSize();

	BOOL bControlButton = DYNAMIC_DOWNCAST(CXTPControlButton, pButton) != NULL;
	BOOL bControlPopup = DYNAMIC_DOWNCAST(CXTPControlPopup, pButton) != NULL;

	XTPButtonStyle buttonStyle = pButton->GetStyle();

	BOOL bVert = pButton->GetParent()->GetPosition() == xtpBarRight || pButton->GetParent()->GetPosition() == xtpBarLeft;

	BOOL bDrawImage = (buttonStyle == xtpButtonAutomatic && controlType != xtpControlPopup) ||
		(buttonStyle == xtpButtonIcon) || (buttonStyle == xtpButtonIconAndCaption);

	CXTPImageManagerIcon* pImage = (bDrawImage && pButton->GetIconId() != 0)? pButton->GetImage(szIcon.cx): NULL;
	bDrawImage = bDrawImage && (pImage != NULL);

	BOOL bDrawText = (pButton->GetCaption().GetLength() > 0) &&
		(IsCaptionStyle(buttonStyle) || (bControlButton && buttonStyle == xtpButtonAutomatic && pImage == NULL) ||
		(buttonStyle == xtpButtonAutomatic && controlType == xtpControlPopup) || (controlType == xtpControlLabel));

	BOOL bPressed = pButton->GetPressed(), bSelected = pButton->GetSelected(), bEnabled = pButton->GetEnabled(), bChecked = pButton->GetChecked(),
		bPopuped = bControlPopup && ((CXTPControlPopup*)pButton)->GetPopup();

	BOOL bEmbossed = !bEnabled && !IsOfficeLikeShadow();

	if (bDraw)
	{
		if (controlType != xtpControlSplitButtonPopup)
			DrawControlEntry(pDC, rcButton, bEnabled, bSelected, bPressed, bChecked, bPopuped, pButton->GetParent()->GetPosition());
		else
			DrawSplitButtonFrame(pDC, pButton, rcButton);
	}

	if (pButton->GetParent()->IsTextBelowIcons() && bDrawImage)
	{
		CXTPFontDC font (pDC, pButton->IsItemDefault()? &m_fontRegularBold: &m_fontRegular);
		BOOL bTriangled = controlType == xtpControlPopup;

		if (bDraw)
		{
			if (controlType == xtpControlSplitButtonPopup) rcButton.right -= 11;

			CPoint pt = CPoint(rcButton.CenterPoint().x - szIcon.cx / 2, rcButton.top + 4);
			DrawImage(pDC, pt, CSize(szIcon.cx, 0), pImage, bSelected, bPressed, bEnabled, bChecked, bPopuped);

			CRect rcText = rcButton;
			rcText.top += szButton.cy - 4;

			DrawTextEx(pDC, pButton->GetCaption(), rcText, TRUE, FALSE, TRUE, bTriangled, bEmbossed);
		}
		else
		{
			CSize szText = DrawTextEx(pDC, pButton->GetCaption(), CXTPEmptyRect(), FALSE, FALSE, TRUE, bTriangled, FALSE);

			szButton.cy += szText.cy;
			szButton.cx = max(szButton.cx, szText.cx + 10);

			if (pButton->GetType() == xtpControlSplitButtonPopup)
				szButton.cx += 11;

			szButton.cx = max(szButton.cx, szButton.cy);
		}
		return szButton;
	}

	if (bDraw && pImage)
	{
		szIcon.cy = pImage->GetHeight() * szIcon.cx / pImage->GetWidth();

		CPoint pt = !bVert? CPoint(rcButton.left + 3, rcButton.CenterPoint().y - szIcon.cy/2): CPoint(rcButton.CenterPoint().x - szIcon.cx/2, rcButton.top + 4);
		if (controlType == xtpControlSplitButtonPopup) pt.x = rcButton.left + 3;
		if (controlType == xtpControlPopup && bDrawImage && !bDrawText) pt.x = rcButton.left + 3;

		DrawImage(pDC, pt, szIcon, pImage, bSelected, bPressed, bEnabled, bChecked, bPopuped);
	}


	switch(controlType)
	{
	case xtpControlPopup:
		{
			if (!bDrawImage)
			{
				CSize sz = DrawTextEx(pDC, pButton->GetCaption(), rcButton, bDraw, bVert, TRUE, !bDraw || pButton->GetParent()->GetType() != xtpBarTypeMenuBar, bEmbossed);
				return CSize(sz.cx + 6, sz.cy + 6);
			}
			else
			{
				if (!bDrawText)
				{
					CPoint pt = CPoint(rcButton.right - 7, rcButton.CenterPoint().y);
					Triangle(pDC, CPoint(pt.x - 2 , pt.y - 1), CPoint(pt.x + 2, pt.y - 1), CPoint (pt.x, pt.y + 1), pDC->GetTextColor());
					return CSize(szButton.cy + 7, szButton.cy);
				}

				return _DrawButtonText(pDC, pButton->GetCaption(), rcButton, bVert, bDraw, bEmbossed, pButton->GetParent()->GetType() != xtpBarTypeMenuBar, szButton, bDrawImage);
			}
		}
	case xtpControlSplitButtonPopup:
		{
			if (!bDrawImage) szButton = CSize(23, 23);

			if (!bDrawText)
				return CSize(szButton.cx + 11, szButton.cy);

			rcButton.right -= 11;

			CSize sz = _DrawButtonText(pDC, pButton->GetCaption(), rcButton, bVert, bDraw, bEmbossed, FALSE, szButton, bDrawImage);
			return CSize(sz.cx + 11, sz.cy);

		}
	case xtpControlButtonPopup:
	case xtpControlButton:
	case xtpControlLabel:
		{
			if (!bDrawText)
				return szButton;

			if (!bDrawImage) szButton = CSize(23, 23);

			return _DrawButtonText(pDC, pButton->GetCaption(), rcButton, bVert, bDraw, bEmbossed, FALSE, szButton, bDrawImage);
		}
	}
	return 0;
}

CRect CXTPPaintManager::GetCommandBarBorders(CXTPCommandBar* /*pBar*/)
{
	return CRect(2, 2, 2, 2);
}

void CXTPPaintManager::GradientFill(CDC* pDC, LPRECT lpRect, COLORREF crFrom, COLORREF crTo, BOOL bHorz)
{
	XTPDrawHelpers()->GradientFill(pDC, lpRect, crFrom, crTo, bHorz);

}

void CXTPPaintManager::DrawStatusBarGripper(CDC* pDC, CRect rcClient)
{
	CXTPPenDC penBorder(*pDC, GetXtremeColor(COLOR_3DSHADOW));
	int i;
	for (i = 0; i < 3; i++ )
	{

		pDC->MoveTo(rcClient.Width() - 3 -i * 4, rcClient.Height() - 2);
		pDC->LineTo(rcClient.Width() - 1, rcClient.Height() - 4 - i * 4);

		pDC->MoveTo(rcClient.Width() - 4 -i * 4, rcClient.Height() - 2);
		pDC->LineTo(rcClient.Width() - 1, rcClient.Height() - 4 - i * 4 - 1);
	}

	CXTPPenDC penBorderWhite(*pDC, GetXtremeColor(COLOR_BTNHIGHLIGHT));
	for (i = 0; i < 3; i++ )
	{
		pDC->MoveTo(rcClient.Width() - 5 -i * 4, rcClient.Height() - 2);
		pDC->LineTo(rcClient.Width() - 1 , rcClient.Height() - 4 - i * 4 - 2);
	}

}

void CXTPPaintManager::AnimateExpanding(CXTPCommandBar* pCommandBar, CDC* pDestDC, CDC* pSrcDC)
{
	CXTPClientRect rcClientPost(pCommandBar);

	int nSteps = m_nAnimationSteps;
	int nAnimationTime = m_nAnimationTime;


	CXTPControls* pControls = pCommandBar->GetControls();

	for (int i = 0; i < nSteps; ++i)
	{
		double dPercent = double(i + 1) / nSteps;

		int nTopDest = 0;
		int nTopSrc = 0;
		BOOL bPrevExpanded = FALSE;
		int nWidth = rcClientPost.Width();

		for (int j = 0; j < pControls->GetCount(); ++j)
		{
			CXTPControl* pControl = pControls->GetAt(j);
			if (!pControl->IsVisible())
				continue;

			BOOL bExpanded = pControl->GetExpanded();

			if (bExpanded != bPrevExpanded)
			{
				int nTopSrcCurrent = pControls->GetAt(j)->GetRect().top;
				int nHeightDstr = int((bPrevExpanded? dPercent: 1) * (nTopSrcCurrent - nTopSrc));

				pDestDC->StretchBlt(0, nTopDest, nWidth, nHeightDstr, pSrcDC,
					0, nTopSrc, nWidth, nTopSrcCurrent - nTopSrc, SRCCOPY);

				nTopDest = nTopDest + nHeightDstr;
				nTopSrc = nTopSrcCurrent;
			}

			bPrevExpanded = bExpanded;
		}

		int nTopSrcCurrent = rcClientPost.Height();
		int nHeightDstr = int((bPrevExpanded? dPercent: 1) * (nTopSrcCurrent - nTopSrc));

		pDestDC->StretchBlt(0, nTopDest, nWidth, nHeightDstr, pSrcDC,
			0, nTopSrc, nWidth, nTopSrcCurrent - nTopSrc, SRCCOPY);

		Sleep(nAnimationTime / nSteps);
	}


}

void CXTPPaintManager::AlphaBlendU(PBYTE pDest, PBYTE pSrcBack, int cx, int cy, PBYTE pSrc, BYTE byAlpha)
{
	const BYTE byDiff = (BYTE)(255 - byAlpha);

	for (int i = 0; i < cx * cy; i++)
	{
		pDest[0]=(BYTE)((pSrcBack[0] * byDiff + pSrc[0] * byAlpha)>>8);
		pDest[1]=(BYTE)((pSrcBack[1] * byDiff + pSrc[1] * byAlpha)>>8);
		pDest[2]=(BYTE)((pSrcBack[2] * byDiff + pSrc[2] * byAlpha)>>8);

		pDest += 4;
		pSrcBack += 4;
		pSrc += 4;
	}
}


void CXTPPaintManager::Animate(CDC* pDestDC, CDC* pSrcDC, CRect rc, XTPAnimationType animationType)
{
	int nSteps = m_nAnimationSteps;
	int nAnimationTime = m_nAnimationTime;

	switch(animationType)
	{
		case xtpAnimateSlide:
			{
				for (int i = 0; i < nSteps; ++i)
				{
					int nHeight = rc.Height() * (i + 1) / nSteps;

					pDestDC->BitBlt(0, 0, rc.Width(), nHeight, pSrcDC,
						0, rc.Height() - nHeight, SRCCOPY);

					Sleep(nAnimationTime / nSteps);
				}
			}
			break;

		case xtpAnimateUnfold:
			{

				for (int i = 0; i < nSteps; ++i)
				{
					CRect rcDest(rc.right - rc.Width()  * (i + 1) / nSteps,  rc.bottom  - rc.Height() * (i + 1) / nSteps, rc.right, rc.bottom);

					pDestDC->BitBlt(0, 0, rcDest.Width(), rcDest.Height(), pSrcDC,
						rcDest.left, rcDest.top, SRCCOPY);

					Sleep(nAnimationTime / nSteps);
				}
			}
			break;

		case xtpAnimateFade:
			{
				const int cx = rc.Width();
				const int cy = rc.Height();

				BITMAPINFOHEADER BMI;
				// Fill in the header info.
				ZeroMemory (&BMI, sizeof (BMI));
				BMI.biSize = sizeof(BITMAPINFOHEADER);
				BMI.biWidth = cx;
				BMI.biHeight = cy;
				BMI.biPlanes = 1;
				BMI.biBitCount = 32;
				BMI.biCompression = BI_RGB;   // No compression

				BYTE * pSrcBits = NULL;
				HBITMAP hbmSrc = CreateDIBSection (NULL, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, 0, 0l);

				BYTE * pSrcBackBits = NULL;
				HBITMAP hbmSrcBack = CreateDIBSection (NULL, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBackBits, 0, 0l);

				BYTE * pDestBits = NULL;
				HBITMAP hbmDest = CreateDIBSection (NULL, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, 0, 0l);

				// Copy our source and destination bitmaps onto our DIBSections,
				// so we can get access to their bits using the BYTE *'s we passed into CreateDIBSection

				CDC dc;
				dc.CreateCompatibleDC(NULL);
				HBITMAP hbmpOld = (HBITMAP) ::SelectObject(dc, hbmSrc);
				::BitBlt(dc, 0, 0, cx, cy, pSrcDC->GetSafeHdc(), 0, 0, SRCCOPY);

				::SelectObject(dc, hbmSrcBack);
				::BitBlt(dc, 0, 0, cx, cy, pDestDC->GetSafeHdc (), 0, 0, SRCCOPY);

				DWORD dwTimePer = nAnimationTime / nSteps;

				::SelectObject(dc, hbmDest);
				for (int i = 1; i < nSteps; ++i)
				{
					DWORD dwTime = GetTickCount ();
					AlphaBlendU(pDestBits, pSrcBackBits, cx, cy, pSrcBits, (BYTE)(255 * i  / nSteps));
					pDestDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &dc, 0, 0, SRCCOPY);
					dwTime = GetTickCount () - dwTime;
					if (dwTime < dwTimePer)
					{
						Sleep(dwTimePer - dwTime);
					}
				}
				::SelectObject(dc, hbmpOld);
				DeleteObject(hbmSrc);
				DeleteObject(hbmSrcBack);
				DeleteObject(hbmDest);
			}

			break;
	}

}

void CXTPPaintManager::UpdateRTLCaption(CString& strCaption)
{
	StripMnemonics(strCaption);

	int iCount = strCaption.GetLength();
	if (iCount < 3)
		return;

	if (iCount > 3 && strCaption.Right(3) == _T("...") && _istalpha(strCaption[iCount - 4]))
	{
		strCaption = _T("...") + strCaption.Left(iCount - 3);
	}

	int nDigits = 0;
	while (nDigits < iCount - 2 && (_istdigit(strCaption[nDigits]) || strCaption[nDigits] == _T('&')))
		nDigits++;

	if (nDigits > 0 &&  strCaption[nDigits] == _T(' '))
	{
		int j = nDigits;
		while (j <  iCount - 2 && (strCaption[j] == _T(' ')))
			j++;

		if (_istalpha(strCaption[j]))
			strCaption = strCaption.Mid(nDigits + 1) + _T(' ') + strCaption.Left(nDigits);
	}
}

void CXTPPaintManager::DrawStatusBarPane(CDC* pDC, CStatusBar* pStatusBar, int nIndex)
{
	CRect rcItem;
	UINT nID, dwStyle;
	int cxWidth;

	pStatusBar->GetItemRect (nIndex, rcItem);
	pStatusBar->GetPaneInfo(nIndex, nID, dwStyle, cxWidth);

	BOOL bGripperPane = (nIndex == pStatusBar->GetCount() - 1);

	// WINBUG: CStatusBar::GetItemRect(...) does not return the correct size
	// of the last item in the status bar in most cases, so we need to calculate
	// the width ourself...

	if (bGripperPane)
	{
		if ((dwStyle & SBPS_STRETCH ) == 0)
		{
			rcItem.right = rcItem.left + cxWidth + (::GetSystemMetrics(SM_CXEDGE) * 3);
		}
		else
		{
			rcItem.right = CXTPClientRect(pStatusBar).right;

			if ((pStatusBar->GetStyle() & SBARS_SIZEGRIP) == SBARS_SIZEGRIP) {
				rcItem.right -= (::GetSystemMetrics(SM_CXSMICON) + ::GetSystemMetrics(SM_CXEDGE));
			}
		}
	}


	if ((dwStyle & SBPS_NOBORDERS) == 0)
		DrawStatusBarPaneBorder(pDC, rcItem, bGripperPane);


	if (!(dwStyle & SBPS_DISABLED))
	{
		CString str = pStatusBar->GetPaneText(nIndex);

		rcItem.DeflateRect(2, 1);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_clrStatusTextColor);
		pDC->DrawText(str, rcItem, DT_SINGLELINE | DT_VCENTER) ;
	}
}

void CXTPPaintManager::DrawStatusBarPaneBorder(CDC* pDC, CRect rc, BOOL /*bGripperPane*/)
{
	Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
}
