// XTPColorManager.cpp: implementation of the CXTPColorManager class.
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
#include "XTPVC80Helpers.h"
#include "XTPWinThemeWrapper.h"

#include "XTPColorManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//===========================================================================
// CXTPPaintManagerColorGradient class
//===========================================================================
void CXTPPaintManagerColorGradient::SetStandardValue(COLORREF _clrLight, COLORREF _clrDark)
{
	clrLight.SetStandardValue(_clrLight);
	clrDark.SetStandardValue(_clrDark);
}

void CXTPPaintManagerColorGradient::SetStandardValue(const COLORREF& clr)
{
	SetStandardValue(clr, clr);
}

void CXTPPaintManagerColorGradient::SetCustomValue(COLORREF _clrLight, COLORREF _clrDark)
{
	clrLight.SetCustomValue(_clrLight);
	clrDark.SetCustomValue(_clrDark);
}


//===========================================================================
// CXTPColorManager class
//===========================================================================
#pragma warning( disable  : 4244 )

#define clrToolBar GetColor(XPCOLOR_TOOLBAR_FACE)
#define clrWindow GetColor(COLOR_WINDOW)
#define clrHighlight GetColor(COLOR_HIGHLIGHT)
#define pow2(x) pow(x, 2)
#define NORMVALUE(a, b) a = (a < 0) ? 0 : (a > b) ? b : a
//#define UNIQUEVALUE _T("7GWBBA98PRVEBP49JESYMATY1EMQU")

CXTPColorManager::CXTPColorManager()
{
	m_bDisableLunaColors = FALSE;
	m_bInit = FALSE;
	m_systemTheme = xtpSystemThemeAuto;
	m_pfnGetSysColor = 0;

	for (int i = 0; i <= XPCOLOR_LAST; i++)
		m_arrCustomColor[i] = (COLORREF)-1;

	RefreshColors();

}

CXTPColorManager& AFX_CDECL CXTPColorManager::Instance()
{
	static CXTPColorManager instance;
	return instance;
}


float CXTPColorManager::ColorWidth(int nLength, int nWidth)
{
	if (nLength > 220) nLength = 220;
	return (2.0 - (float)nLength / 220) * nWidth;
}


float CXTPColorManager::ColorDelta(COLORREF clrA, COLORREF clrB)
{
	return pow2(GetRDelta(clrA) - GetRDelta(clrB)) +
		pow2(GetGDelta(clrA) - GetGDelta(clrB)) +
		pow2(GetBDelta(clrA) - GetBDelta(clrB));
}

float CXTPColorManager::Length(COLORREF clrA, COLORREF clrB)
{
	return sqrt(ColorDelta(clrA, clrB));
}

BOOL CXTPColorManager::LongColor(COLORREF clrMain, COLORREF clrSub, BOOL bCalcLength, float fDistance)
{
	if (bCalcLength)
		fDistance =  ColorWidth(Length(clrMain, 0), fDistance);

	return pow2(fDistance) < ColorDelta(clrMain, clrSub);
}

COLORREF CXTPColorManager::MixColor(COLORREF clrMain, COLORREF clrSub, float fDistance)
{
	float fMainLength =  Length(clrMain, 0);

	float fGate = ColorWidth(fMainLength, fDistance);

	BOOL bReverse = TRUE;

	if (Length(clrSub, 0) > fMainLength)
	{
		if (fMainLength > 442 - fGate)
			bReverse = FALSE;

	}
	else
	{
		if (fMainLength > fGate)
			bReverse = FALSE;
	}

	float fSubRed = GetRDelta(clrSub);
	float fSubGreen = GetGDelta(clrSub);
	float fSubBlue= GetBDelta(clrSub);

	float fMainRed = GetRDelta(clrMain);
	float fMainGreen = GetGDelta(clrMain);
	float fMainBlue= GetBDelta(clrMain);

	if (bReverse)
	{
		fSubRed = 195.0 - fSubRed;
		fSubGreen = 390.0 - fSubGreen;
		fSubBlue = 65.0 - fSubBlue;

		fMainRed = 195.0 - fMainRed;
		fMainGreen = 390.0 - fMainGreen;
		fMainBlue = 65.0 - fMainBlue;
	}

	float A = __max(0.001, sqrt(pow2(fSubRed) + pow2(fSubGreen) + pow2(fSubBlue)));
	float B = fMainRed * fSubRed + fMainGreen * fSubGreen + fMainBlue * fSubBlue;
	float X = B / pow2(A);

	float fR = X * fSubRed - fMainRed;
	float fG = X * fSubGreen - fMainGreen;
	float fB = X * fSubBlue - fMainBlue;

	float Y = sqrt(pow2(fGate) - (pow2(fR) + pow2(fG) + pow2(fB)));

	float Z =(B / A - Y);

	fR = Z * fSubRed / A;
	fG = Z * fSubGreen / A;
	fB = Z * fSubBlue / A;

	if (bReverse)
	{
		fR = 195.0 - fR;
		fG = 390.0 - fG;
		fB = 65.0  - fB;
	}

	NORMVALUE(fR, 195.0);
	NORMVALUE(fG, 390.0);
	NORMVALUE(fB, 65.0);

	int nR = .5 + fR * (255.0/195.0);
	int nG = .5 + fG * (255.0/390.0);
	int nB = .5 + fB * (255.0/65.0);

	return RGB(NORMVALUE(nR, 255), NORMVALUE(nG, 255), NORMVALUE(nB, 255));
}

COLORREF CXTPColorManager::AdjustColor(COLORREF clrMain, COLORREF clrSub, float fDistance)
{
	float Z = sqrt(
		pow2(GetRDelta(clrMain) - GetRDelta(clrSub)) +
		pow2(GetGDelta(clrMain) - GetGDelta(clrSub)) +
		pow2(GetBDelta(clrMain) - GetBDelta(clrSub)));

	float Q = (Z - ColorWidth(Length(0, clrMain), fDistance)) / Z;

	float fR = Q * (GetRDelta(clrMain) - GetRDelta(clrSub)) + GetRDelta(clrSub);
	float fG = Q * (GetGDelta(clrMain) - GetGDelta(clrSub)) + GetGDelta(clrSub);
	float fB = Q * (GetBDelta(clrMain) - GetBDelta(clrSub)) + GetBDelta(clrSub);

	NORMVALUE(fR, 195.0);
	NORMVALUE(fG, 390.0);
	NORMVALUE(fB, 65.0);

	int nR = .5 + fR * (255.0/195.0);
	int nG = .5 + fG * (255.0/390.0);
	int nB = .5 + fB * (255.0/65.0);

	return RGB(NORMVALUE(nR, 255), NORMVALUE(nG, 255), NORMVALUE(nB, 255));
}

COLORREF CXTPColorManager::LightColor(COLORREF clrLight, COLORREF clrDark, int nDelta)
{
	int nParam = (nDelta < 100 ? 100 : 1000);

	int nR = (GetRValue(clrDark) * (nParam - nDelta) + nParam / 2 +  GetRValue(clrLight) * nDelta) / nParam;
	int nG = (GetGValue(clrDark) * (nParam - nDelta) + nParam / 2 +  GetGValue(clrLight) * nDelta) / nParam;
	int nB = (GetBValue(clrDark) * (nParam - nDelta) + nParam / 2 +  GetBValue(clrLight) * nDelta) / nParam;

	return RGB(nR, nG, nB);
}

COLORREF CXTPColorManager::GetColor(int nIndex)
{
	if (nIndex > XPCOLOR_LAST) return nIndex;
	return m_arrCustomColor[nIndex] == (COLORREF)-1? m_arrStandardColor[nIndex]: m_arrCustomColor[nIndex];
}

void CXTPColorManager::RefreshSysColors()
{
	m_arrStandardColor[0] = 0;
	for (int i = 1; i < 28; i++) m_arrStandardColor[i] = (m_pfnGetSysColor != 0? (*m_pfnGetSysColor)(i): GetSysColor(i));
}

void CXTPColorManager::SetGetSysColorPtr(PFNGETSYSCOLOR pfnGetSysColor)
{
	m_pfnGetSysColor = pfnGetSysColor;
	RefreshColors();
}

void CXTPColorManager::RefreshXPColors()
{
	COLORREF clr3DFace = GetColor(COLOR_3DFACE);
	COLORREF clr3DShadow = GetColor(COLOR_3DSHADOW);

	m_arrStandardColor[XPCOLOR_TOOLBAR_FACE]             = clr3DFace;
	m_arrStandardColor[XPCOLOR_HIGHLIGHT]                = GetColor(COLOR_WINDOW);
	m_arrStandardColor[XPCOLOR_HIGHLIGHT_PUSHED]         = GetColor(COLOR_HIGHLIGHT);
	m_arrStandardColor[XPCOLOR_MENUBAR_FACE]             = GetColor(COLOR_WINDOW);
	m_arrStandardColor[XPCOLOR_GRAYTEXT]                 = GetColor(COLOR_GRAYTEXT);
	m_arrStandardColor[XPCOLOR_TOOLBAR_GRIPPER]          = clr3DShadow;
	m_arrStandardColor[XPCOLOR_SEPARATOR]                = clr3DShadow;
	m_arrStandardColor[XPCOLOR_MENUBAR_BORDER]           = clr3DShadow;
	m_arrStandardColor[XPCOLOR_DISABLED]                 = clr3DShadow;
	m_arrStandardColor[XPCOLOR_HIGHLIGHT_CHECKED]        = GetColor(COLOR_WINDOW);
	m_arrStandardColor[XPCOLOR_HIGHLIGHT_BORDER]         = GetColor(COLOR_HIGHLIGHT);
	m_arrStandardColor[XPCOLOR_HIGHLIGHT_CHECKED_BORDER] = GetColor(COLOR_HIGHLIGHT);
	m_arrStandardColor[XPCOLOR_MENUBAR_TEXT]             = GetColor(COLOR_WINDOWTEXT);
	m_arrStandardColor[XPCOLOR_HIGHLIGHT_TEXT]           = GetColor(COLOR_MENUTEXT);
	m_arrStandardColor[XPCOLOR_TOOLBAR_TEXT]             = GetColor(COLOR_BTNTEXT);
	m_arrStandardColor[XPCOLOR_PUSHED_TEXT]              = GetColor(COLOR_HIGHLIGHTTEXT);
	m_arrStandardColor[XPCOLOR_TAB_INACTIVE_BACK]        = clr3DFace;
	m_arrStandardColor[XPCOLOR_TAB_INACTIVE_TEXT]        = GetColor(COLOR_BTNTEXT);
	m_arrStandardColor[XPCOLOR_MENUBAR_EXPANDED]         = clr3DShadow;

	m_arrStandardColor[XPCOLOR_HIGHLIGHT_PUSHED_BORDER]  = GetColor(COLOR_HIGHLIGHT);
	m_arrStandardColor[XPCOLOR_ICONSHADDOW]              = clr3DShadow;
	m_arrStandardColor[XPCOLOR_3DFACE]                   = clr3DFace;
	m_arrStandardColor[XPCOLOR_3DSHADOW]                 = clr3DShadow;
	m_arrStandardColor[XPCOLOR_EDITCTRLBORDER]           = clr3DFace;
	m_arrStandardColor[XPCOLOR_FRAME]                    = clr3DShadow;
	m_arrStandardColor[XPCOLOR_SPLITTER_FACE]            = clr3DFace;
	m_arrStandardColor[XPCOLOR_LABEL]                    = clr3DFace;
	m_arrStandardColor[XPCOLOR_STATICFRAME]              = clr3DShadow;
}

void CXTPColorManager::RefreshGradientColors()
{
	XTPCurrentSystemTheme systemTheme = GetCurrentSystemTheme();

	grcCaption.SetStandardValue(GetColor(COLOR_3DSHADOW), GetColor(COLOR_3DSHADOW));
	grcDockBar.SetStandardValue(GetColor(COLOR_3DFACE), LightColor(GetColor(COLOR_3DFACE), GetColor(COLOR_WINDOW), 0xcd));
	grcShortcutBarGripper.SetStandardValue(GetColor(COLOR_3DFACE), GetColor(COLOR_3DSHADOW));
	grcToolBar.SetStandardValue(LightColor(GetColor(COLOR_3DFACE), GetColor(COLOR_WINDOW), 0xcd), GetColor(COLOR_3DFACE));

	if (!m_bDisableLunaColors)
	{
		switch (systemTheme)
		{
		case xtpSystemThemeBlue:
			{
				m_arrStandardColor[XPCOLOR_FRAME] = RGB(0, 45, 150);
				grcCaption.SetStandardValue(RGB(89, 135, 214), RGB(0, 45, 150));
				grcDockBar.SetStandardValue(RGB(158, 190, 245), RGB(196, 218, 250));
				grcShortcutBarGripper.SetStandardValue(RGB(89, 135, 214), RGB(0, 45, 150));
				grcToolBar.SetStandardValue(RGB(221, 236, 254), RGB(129, 169, 226));
			}
			break;

		case xtpSystemThemeOlive:
			{
				m_arrStandardColor[XPCOLOR_FRAME] = RGB(96, 128, 88);;
				grcCaption.SetStandardValue(RGB(175, 192, 130), RGB(99, 122, 68));
				grcDockBar.SetStandardValue(RGB(217, 217, 167), RGB(242, 241, 228));
				grcShortcutBarGripper.SetStandardValue(RGB(120, 142, 111), RGB(73,91, 67));
				grcToolBar.SetStandardValue(RGB(244, 247, 222), RGB(183, 198, 145));
			}
			break;

		case xtpSystemThemeSilver:
			{
				m_arrStandardColor[XPCOLOR_FRAME] = RGB(124, 124, 148);
				grcCaption.SetStandardValue(RGB(168, 167, 191), RGB(112, 111, 145));
				grcDockBar.SetStandardValue(RGB(215, 215, 229), RGB(243, 243, 247));
				grcShortcutBarGripper.SetStandardValue(RGB(168, 167, 191),RGB(119, 118, 151));
				grcToolBar.SetStandardValue(RGB(243, 244, 250), RGB(153, 151, 181));
			}
			break;
		}
	}
}

void CXTPColorManager::RefreshColors(BOOL bInit)
{

	if (bInit && m_bInit)
		return;

	m_bInit = TRUE;

	struct MUTECOLOR
	{
		int nIndex;
		int clrMain;
		int clrSub;
		int nDistance;
	} ;

	BOOL bSimpleColors = FALSE;

	CDC *pDC = CWnd::GetDesktopWindow()->GetDC();
	if (pDC)
	{
		int nColors = pDC->GetDeviceCaps(BITSPIXEL);
		CWnd::GetDesktopWindow()->ReleaseDC(pDC);

		bSimpleColors = (nColors > 0 && nColors <= 8);
	}

	RefreshSysColors();
	RefreshXPColors();

	if (bSimpleColors)
	{
		RefreshGradientColors();
		return;
	}

	const MUTECOLOR IndexTable[] =
	{
		{ XPCOLOR_TOOLBAR_FACE,     COLOR_WINDOW,         COLOR_3DFACE,    165  },
		{ XPCOLOR_HIGHLIGHT,        COLOR_HIGHLIGHT,      COLOR_WINDOW,    0x1E },
		{ XPCOLOR_HIGHLIGHT_PUSHED, COLOR_HIGHLIGHT,      COLOR_WINDOW,    0x32 },
		{ XPCOLOR_MENUBAR_FACE,     COLOR_3DFACE,         COLOR_WINDOW,    0x8f },
		{ XPCOLOR_GRAYTEXT,         COLOR_GRAYTEXT,       COLOR_WINDOW,    0x46 },
		{ XPCOLOR_TOOLBAR_GRIPPER,  COLOR_3DSHADOW,       COLOR_WINDOW,    0x4b },
		{ XPCOLOR_SEPARATOR,        COLOR_3DSHADOW,       COLOR_WINDOW,    0x46 },
		{ XPCOLOR_MENUBAR_BORDER,   XPCOLOR_TOOLBAR_TEXT, COLOR_BTNSHADOW, 0x14 },
		{ XPCOLOR_DISABLED,         COLOR_3DSHADOW,       COLOR_WINDOW,    0x5A },
		{ XPCOLOR_MENUBAR_EXPANDED, COLOR_3DFACE,         COLOR_3DSHADOW,  90 }
	};

	int i;
	for (i = 0; i < sizeof(IndexTable) / sizeof(IndexTable[0]); i++)
	{
		m_arrStandardColor[IndexTable[i].nIndex] = LightColor(GetColor(IndexTable[i].clrMain),
			GetColor(IndexTable[i].clrSub), IndexTable[i].nDistance);

	}

	const MUTECOLOR LongTable[] =
	{
		{ XPCOLOR_HIGHLIGHT,                XPCOLOR_TOOLBAR_FACE,          XPCOLOR_HIGHLIGHT,                50  },
		{ XPCOLOR_HIGHLIGHT_BORDER,         XPCOLOR_TOOLBAR_FACE,          COLOR_HIGHLIGHT,                  100 },
		{ XPCOLOR_HIGHLIGHT_PUSHED,         XPCOLOR_TOOLBAR_FACE,          XPCOLOR_HIGHLIGHT_PUSHED,         30  },
		{ XPCOLOR_GRAYTEXT,                 XPCOLOR_MENUBAR_FACE,          XPCOLOR_GRAYTEXT,                 80  },
		{ XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED,      XPCOLOR_HIGHLIGHT_CHECKED_BORDER, 100 },
		{ XPCOLOR_TOOLBAR_GRIPPER,          XPCOLOR_TOOLBAR_FACE,          XPCOLOR_TOOLBAR_GRIPPER,          85  },
		{ XPCOLOR_SEPARATOR,                XPCOLOR_TOOLBAR_FACE,          XPCOLOR_SEPARATOR,                50  },
		{ XPCOLOR_MENUBAR_BORDER,           XPCOLOR_PUSHED_TEXT,           XPCOLOR_MENUBAR_BORDER,           100 },
		{ XPCOLOR_DISABLED,                 XPCOLOR_TOOLBAR_FACE,          XPCOLOR_DISABLED,                 80  },
		{ XPCOLOR_MENUBAR_TEXT,             XPCOLOR_MENUBAR_FACE,          XPCOLOR_MENUBAR_TEXT,             180 },
		{ XPCOLOR_HIGHLIGHT_TEXT,           XPCOLOR_HIGHLIGHT,             XPCOLOR_HIGHLIGHT_TEXT,           180 },
		{ XPCOLOR_TOOLBAR_TEXT,             XPCOLOR_TOOLBAR_FACE,          XPCOLOR_TOOLBAR_TEXT,             180 },
		{ XPCOLOR_PUSHED_TEXT,              XPCOLOR_HIGHLIGHT_PUSHED,      XPCOLOR_PUSHED_TEXT,              180 }
	};

	if (LongColor(GetColor(COLOR_3DFACE), GetColor(XPCOLOR_TOOLBAR_FACE), 1, 35))
	{
		m_arrStandardColor[XPCOLOR_TOOLBAR_FACE] = AdjustColor(GetColor(COLOR_3DFACE), GetColor(XPCOLOR_TOOLBAR_FACE), 35);
	}

	for (i = 0; i < sizeof(LongTable) / sizeof(LongTable[0]); i++)
	{
		if (! LongColor(GetColor(LongTable[i].clrMain),GetColor(LongTable[i].clrSub) , 1, LongTable[i].nDistance))
		{
			m_arrStandardColor[LongTable[i].nIndex] = MixColor(GetColor(LongTable[i].clrMain),GetColor(LongTable[i].clrSub), LongTable[i].nDistance);
		}
	}

	if (LongColor(GetColor(XPCOLOR_MENUBAR_FACE), GetColor(XPCOLOR_GRAYTEXT), 1, 145))
	{

		m_arrStandardColor[XPCOLOR_GRAYTEXT] = AdjustColor(GetColor(XPCOLOR_MENUBAR_FACE), GetColor(XPCOLOR_GRAYTEXT), 145);
	}

	m_arrStandardColor[XPCOLOR_HIGHLIGHT_CHECKED] =
		RGB( GetRValue(clrWindow) * 40 / 100 + GetRValue(clrHighlight) * 10 / 100 + GetRValue(clrToolBar) * 50 / 100,
		GetGValue(clrWindow) * 40 / 100 + GetGValue(clrHighlight) * 10 / 100 + GetGValue(clrToolBar) * 50 / 100,
		GetBValue(clrWindow) * 40 / 100 + GetBValue(clrHighlight) * 10 / 100 + GetBValue(clrToolBar) * 50 / 100);


	COLORREF clrBtn = GetColor(COLOR_3DFACE);
	int r =  GetRValue(clrBtn), g = GetGValue(clrBtn), b = GetBValue(clrBtn);
	int nMax = __max(__max(r, g),  b);
	if (nMax == 0)
		m_arrStandardColor[XPCOLOR_TAB_INACTIVE_BACK] = RGB(35, 35, 35); else
	{
		int nDelta = __min(35, 255 - nMax) + nMax;
		m_arrStandardColor[XPCOLOR_TAB_INACTIVE_BACK] = RGB(r * nDelta / nMax, g * nDelta / nMax, b * nDelta / nMax);
	}

	COLORREF clrShadow = GetColor(COLOR_3DSHADOW);
	r =  GetRValue(clrShadow), g = GetGValue(clrShadow), b = GetBValue(clrShadow );
	nMax = __max(__max(r, g),  b);

	if (clrBtn == 0) m_arrStandardColor[XPCOLOR_TAB_INACTIVE_TEXT] = RGB(0x7f, 0x7f, 0x7f);
	else if (nMax == 0) m_arrStandardColor[XPCOLOR_TAB_INACTIVE_TEXT] = RGB(0, 0, 0);
	else
	{
		int nDelta = __max(0, nMax - 43);
		m_arrStandardColor[XPCOLOR_TAB_INACTIVE_TEXT] = RGB(r * nDelta / nMax, g * nDelta / nMax, b * nDelta / nMax);
	}

	COLORREF clrBackground = m_arrStandardColor[XPCOLOR_HIGHLIGHT];
	m_arrStandardColor[XPCOLOR_ICONSHADDOW] = RGB( GetRValue(clrBackground) * .75, GetGValue(clrBackground ) * .75, GetBValue(clrBackground) * .75);

	m_arrStandardColor[COLOR_3DLIGHT] = RGB(min(GetRValue(clrBtn) + 15, 255), min(GetGValue(clrBtn) + 15, 255), min(GetBValue(clrBtn) + 15, 255));
	m_arrStandardColor[XPCOLOR_EDITCTRLBORDER] = GetColor(XPCOLOR_TOOLBAR_FACE);

	RefreshGradientColors();

	CXTPWinThemeWrapper wrpTreeView;
	wrpTreeView.OpenTheme(0, L"TREEVIEW");
	if (wrpTreeView.IsAppThemed())
	{
		wrpTreeView.GetThemeColor(0, 0, TMT_BORDERCOLOR, &m_arrStandardColor[XPCOLOR_STATICFRAME]);
	}
}

void CXTPColorManager::SetColors(int cElements, CONST INT *lpaElements, CONST COLORREF *lpaRgbValues)
{
	for (int i = 0; i < cElements; i++)
		m_arrCustomColor[lpaElements[i]] = lpaRgbValues[i];
}

void CXTPColorManager::SetColor(int nIndex, COLORREF clrValue)
{
	m_arrCustomColor[nIndex] = clrValue;
}

double CXTPColorManager::GetRDelta(COLORREF clr)
{
	return GetRValue(clr) * (195.0/255.0);
}

double CXTPColorManager::GetGDelta(COLORREF clr)
{
	return GetGValue(clr) * (390.0/255.0);
}

double CXTPColorManager::GetBDelta(COLORREF clr)
{
	return GetBValue(clr) * (65.0/255.0);
}

void CXTPColorManager::DisableLunaColors(BOOL bDisable)
{
	m_bDisableLunaColors = bDisable;
}

BOOL CXTPColorManager::IsLunaColorsDisabled()
{
	return m_bDisableLunaColors;
}

void CXTPColorManager::SetLunaTheme(XTPCurrentSystemTheme systemTheme)
{
	m_systemTheme = systemTheme;
}

XTPCurrentSystemTheme CXTPColorManager::GetWinThemeWrapperTheme()
{
	WCHAR pszThemeFileName[MAX_PATH];
	WCHAR pszColorBuff[MAX_PATH];

	// get the name of the current theme in use.
	if ( CXTPWinThemeWrapper().GetCurrentThemeName( pszThemeFileName, MAX_PATH,
		pszColorBuff, MAX_PATH, NULL, 0) != S_OK )
	{
		return xtpSystemThemeUnknown;
	}

	// search for "luna.msstyles" string in theme name.
	if (wcsstr(WCSLWR_S(pszThemeFileName, MAX_PATH), L"luna.msstyles"))
	{
		if (wcscmp(WCSLWR_S(pszColorBuff, MAX_PATH), L"normalcolor") == 0)
			return xtpSystemThemeBlue;

		if (wcscmp(WCSLWR_S(pszColorBuff, MAX_PATH), L"homestead") == 0)
			return xtpSystemThemeOlive;

		if (wcscmp(WCSLWR_S(pszColorBuff, MAX_PATH), L"metallic") == 0)
			return xtpSystemThemeSilver;
	}

	return xtpSystemThemeUnknown;
}

XTPCurrentSystemTheme CXTPColorManager::GetCurrentSystemTheme()
{
	if (m_systemTheme != xtpSystemThemeAuto && m_systemTheme != xtpSystemThemeDefault)
		return m_systemTheme;

	return GetWinThemeWrapperTheme();
}



