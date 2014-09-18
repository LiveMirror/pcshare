// XTSplitterTheme.cpp: implementation of the ÑXTSplitterTheme class.
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
#include "XTPWinThemeWrapper.h"
#include "XTPColorManager.h"
#include "XTPDrawHelpers.h"

#include "XTThemeManager.h"
#include "XTSplitterTheme.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_THEME_FACTORY(CXTSplitterTheme, CXTSplitterTheme, CXTSplitterThemeOffice2003)

CXTSplitterTheme::CXTSplitterTheme()
{
}

void CXTSplitterTheme::RefreshMetrics()
{
	m_clrSplitterFace = GetSysColor(COLOR_3DFACE);
	m_clrSplitterBorders = GetSysColor(COLOR_3DSHADOW);
}

void CXTSplitterThemeOffice2003::RefreshMetrics()
{
	CXTSplitterTheme::RefreshMetrics();

	if (!XTPColorManager()->IsLunaColorsDisabled())
	{
		XTPCurrentSystemTheme systemTheme = XTPDrawHelpers()->GetCurrentSystemTheme();

		switch (systemTheme)
		{
		case xtpSystemThemeBlue:
			m_clrSplitterFace =  RGB(216, 231, 252);
			m_clrSplitterBorders = RGB(158, 190, 245);
			break;

		case xtpSystemThemeOlive:
			m_clrSplitterFace = RGB(226, 231, 191);
			m_clrSplitterBorders = RGB(171, 192, 138);
			break;

		case xtpSystemThemeSilver:
			m_clrSplitterFace = RGB(223, 223, 234);
			m_clrSplitterBorders = RGB(177, 176, 195);
		}

	}
}
