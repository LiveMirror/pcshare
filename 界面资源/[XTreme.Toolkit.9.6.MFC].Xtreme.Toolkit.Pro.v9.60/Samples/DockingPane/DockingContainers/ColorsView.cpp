// ColorsView.cpp: implementation of the CColorsView class.
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
#include "DockingContainers.h"
#include "ColorsView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CColorsView::CColorsView()
{
	m_nToolBarID = IDR_COLORS_BAR;
}

CColorsView::~CColorsView()
{

}

BEGIN_MESSAGE_MAP(CColorsView, CPaneView)
	//{{AFX_MSG_MAP(CExplorerView)
	ON_COMMAND_RANGE(ID_COLORS_EXPLORE, ID_COLROS_SWITCH, OnColorsCommand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CWnd* CColorsView::OnCreateView()
{
	if (!m_wndColors.Create(NULL, WS_VISIBLE | WS_CHILD |SS_NOTIFY,
		CRect(0,0,0,0), this ))
	{
		TRACE0( "Unable to create tree control.\n" );
		return NULL;
	}

	return &m_wndColors;
}

void CColorsView::OnColorsCommand(UINT)
{
	// TODO: Add your command handler code here

}


/////////////////////////////////////////////////////////////////////////////
// CColorsSwatchWnd

CColorsSwatchWnd::CColorsSwatchWnd()
{
}

CColorsSwatchWnd::~CColorsSwatchWnd()
{
}


BEGIN_MESSAGE_MAP(CColorsSwatchWnd, CStatic)
	//{{AFX_MSG_MAP(CColorsSwatchWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorsSwatchWnd message handlers

BOOL CColorsSwatchWnd::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CColorsSwatchWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CXTPClientRect rc(this);

	CBitmap bmp;
	VERIFY(bmp.LoadBitmap(IDB_SWATCH));
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);

	CXTPCompatibleDC dcMem(&dc, &bmp);

	dc.StretchBlt(0, 0, rc.Width(), rc.Height(), &dcMem, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);

	/*dc.FillSolidRect(rc, GetSysColor(COLOR_3DFACE));
	dc.DrawState(CPoint(rc.CenterPoint().x - bmpInfo.bmWidth/2, rc.CenterPoint().y - bmpInfo.bmHeight/2),
		CSize(bmpInfo.bmWidth, bmpInfo.bmHeight), &bmp, 0);*/
}

void CColorsSwatchWnd::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	Invalidate(FALSE);

}
