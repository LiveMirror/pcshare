// PaneSearch.cpp : implementation file
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
#include "GUI_OneNote.h"
#include "PaneSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaneSearch

CPaneSearch::CPaneSearch()
{
}

CPaneSearch::~CPaneSearch()
{
}


BEGIN_MESSAGE_MAP(CPaneSearch, CWnd)
	//{{AFX_MSG_MAP(CPaneSearch)
	ON_WM_CREATE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CPaneSearch::RefreshColors()
{
	XTPCurrentSystemTheme systemTheme = XTPDrawHelpers()->GetCurrentSystemTheme();

	switch (systemTheme)
	{
		case xtpSystemThemeBlue:
			m_clrOffice3DFace = RGB(221, 236, 254);
			m_clrOffice3DFaceDark = RGB(74, 122, 201);
			break;
		case xtpSystemThemeOlive:
			m_clrOffice3DFace = RGB(243, 242, 231);
			m_clrOffice3DFaceDark = RGB(190, 198, 152);
			break;
		case xtpSystemThemeSilver:
			m_clrOffice3DFace = RGB(238, 238, 244);
			m_clrOffice3DFaceDark = RGB(177, 176, 195);
			break;
		default:

			m_clrOffice3DFaceLight = XTPColorManager()->LightColor(GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_WINDOW), 0xCD);
			m_clrOffice3DFaceDark = GetSysColor(COLOR_3DFACE);
			m_clrOffice3DFace = XTPColorManager()->LightColor(GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_WINDOW), 50);
	}

}

/////////////////////////////////////////////////////////////////////////////
// CPaneSearch message handlers

int CPaneSearch::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	RefreshColors();

	return 0;
}

void CPaneSearch::OnSysColorChange()
{
	CWnd::OnSysColorChange();

	RefreshColors();

}

void CPaneSearch::OnPaint()
{
		CPaintDC dcPaint(this); // device context for painting

	CXTPClientRect rc(this);

	CXTPBufferDC dc(dcPaint, rc);
	XTPDrawHelpers()->GradientFill(&dc, rc, m_clrOffice3DFace, m_clrOffice3DFaceDark, FALSE);

}

BOOL CPaneSearch::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CPaneSearch::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	Invalidate(FALSE);

}
