// PaneNew.cpp : implementation file
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
#include "PaneNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaneNew

CPaneNew::CPaneNew()
{
}

CPaneNew::~CPaneNew()
{
}


BEGIN_MESSAGE_MAP(CPaneNew, CWnd)
	//{{AFX_MSG_MAP(CPaneNew)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPaneNew message handlers

BOOL CPaneNew::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

static struct
{
	UINT nID;
	LPCTSTR lpUrl;
	LPCTSTR lpDesc;
}
tasks[] =
{
	ID_FILE_NEW, _T("Blank Page"), _T("Add a new page to the current notebook. A new page is represented by a new tab at the end of the vertical series of tabs."),
	ID_INSERT_SUBPAGE, _T("Subpage"), _T("Add a new page immediately following the current page and that shares the same title as the current page. A subpage is represented by a small tab to indicate that it is grouped with another page."),
	ID_INSERT_NEWSECTION, _T("Section"), _T("Create a section file in the current folder on your computer. Sections are represented by tabs across the top of the pages."),
	ID_WINDOW_NEW, _T("Quick note"), _T("Open a small OneNote window where you can make notes that are automatically saved in the Quick notes section.")
};

void CPaneNew::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting

	CXTPClientRect rc(this);

	CXTPBufferDC dc(dcPaint, rc);
	dc.FillSolidRect(rc, m_clrOffice3DFace);


	CXTPFontDC font(&dc, XTPPaintManager()->GetRegularFont());
	int nTextHeight = dc.GetTextExtent(_T(" ")).cy;

	int nTop = 20;


	for (int i = 0; i < _countof(tasks); i++)
	{
		dc.DrawState(CPoint(17, nTop - 2), CSize(0), XTPImageManager()->GetImage(tasks[i].nID)->GetIcon(), DST_ICON|DSS_NORMAL, (CBrush*)NULL);
		dc.SetTextColor(RGB(0, 0, 255));
		CRect rcUrl(37, nTop, rc.Width() - 10, nTop + nTextHeight);
		dc.DrawText(tasks[i].lpUrl, rcUrl, DT_SINGLELINE);

		nTop = nTop + nTextHeight + 3;
		dc.SetTextColor(GetSysColor(COLOR_BTNTEXT));

		CRect rcDesc(37, nTop, rc.Width() - 10, nTop);
		dc.DrawText(tasks[i].lpDesc, rcDesc, DT_WORDBREAK|DT_CALCRECT);
		dc.DrawText(tasks[i].lpDesc, rcDesc, DT_WORDBREAK);

		nTop = rcDesc.bottom + 13;
	}
}

void CPaneNew::RefreshColors()
{
	XTPCurrentSystemTheme systemTheme = XTPDrawHelpers()->GetCurrentSystemTheme();

	switch (systemTheme)
	{
		case xtpSystemThemeBlue:
			m_clrOffice3DFace = RGB(221, 236, 254);
			break;
		case xtpSystemThemeOlive:
			m_clrOffice3DFace = RGB(243, 242, 231);
			break;
		case xtpSystemThemeSilver:
			m_clrOffice3DFace = RGB(238, 238, 244);
			break;
		default:
			m_clrOffice3DFace = XTPColorManager()->LightColor(GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_WINDOW), 50);
	}

}

int CPaneNew::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	RefreshColors();

	return 0;
}

void CPaneNew::OnSysColorChange()
{
	CWnd::OnSysColorChange();

	RefreshColors();

}

void CPaneNew::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	Invalidate(FALSE);

}
