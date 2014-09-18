// ACDSeeTabCtrl.cpp : implementation file
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
#include "GUI_ACDSee.h"
#include "ACDSeeTabCtrl.h"

#include "ACDSeeTheme.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CACDSeeTabCtrl, CTabCtrl);
/////////////////////////////////////////////////////////////////////////////
// CACDSeeTabCtrl

CACDSeeTabCtrl::CACDSeeTabCtrl()
{
}

CACDSeeTabCtrl::~CACDSeeTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CACDSeeTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CACDSeeTabCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACDSeeTabCtrl message handlers

BOOL CACDSeeTabCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CACDSeeTabCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CXTPClientRect rc(this);

	CXTPBufferDC memDC(dc, rc);
	CACDSeeTheme::_FillRect(&memDC, this, CACDSeeTheme::m_hbmTabCtrlBackground);

	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );

}


void CACDSeeTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDraw)
{
	CDC* pDC = CDC::FromHandle(lpDraw->hDC);
	CRect rcItem(lpDraw->rcItem);

	BOOL bSelected = lpDraw->itemState & ODS_SELECTED;

	if (bSelected)
	{
		CXTPCompatibleDC dc(pDC, CBitmap::FromHandle(CACDSeeTheme::m_hbmTabCtrlSelected));
		for (int x = rcItem.left; x < rcItem.right; x += 128)
			pDC->BitBlt(x, rcItem.top , min(128, rcItem.right - x), rcItem.Height(), &dc, 0, 0, SRCCOPY);
	}

	TCHAR strTitle[256];
	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT|TCIF_IMAGE;
	tcItem.pszText = strTitle;
	tcItem.cchTextMax = 256;
	GetItem(lpDraw->itemID, &tcItem);

	GetItemRect(lpDraw->itemID, &rcItem);
	if (bSelected) rcItem.OffsetRect(1, 1);

	pDC->SetTextColor(GetSysColor(COLOR_BTNTEXT));
	pDC->SetBkMode(TRANSPARENT);
	CRect rcText(rcItem);
	rcText.left += 7 + 16;
	pDC->DrawText(strTitle, rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	CImageList* pList = GetImageList();
	if (pList)
	{
		pList->Draw(pDC, tcItem.iImage, CPoint(rcItem.left + 3, rcItem.CenterPoint().y - 7), 0);
	}
	//pDC->DrawText(strTitle, rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE);


}
