// XTPReportTip.cpp : implementation of the CXTPReportTip class.
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
#include "XTPDrawHelpers.h"

#include "XTPReportControl.h"
#include "XTPReportDefines.h"
#include "XTPReportTip.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPReportTip

CXTPReportTip::CXTPReportTip()
{
	m_pParentWnd = NULL;
	m_pItem = NULL;
	m_nRowIndex = -1;
}

CXTPReportTip::~CXTPReportTip()
{
}


BEGIN_MESSAGE_MAP(CXTPReportTip, CWnd)
	//{{AFX_MSG_MAP(CXTPReportTip)
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPReportTip message handlers

UINT CXTPReportTip::OnNcHitTest(CPoint point)
{
	UNREFERENCED_PARAMETER(point);
	return (UINT)HTTRANSPARENT;
}

BOOL CXTPReportTip::Create(CWnd * pParentWnd)
{
	ASSERT_VALID(pParentWnd);

	// Already created?
	if (m_hWnd)
		return TRUE;

	m_pParentWnd = pParentWnd;
	return CWnd::CreateEx(0, AfxRegisterWndClass(0, LoadCursor(0, IDC_ARROW)), NULL, WS_POPUP,
		CXTPEmptyRect(), pParentWnd, NULL);
}


void CXTPReportTip::Activate(BOOL bActive)
{
	if (bActive)
	{
		CString strText = m_strTooltipText;
		CWindowDC dc(this);
		CXTPFontDC font(&dc, &m_fntToolTip);
		CXTPWindowRect rc (this);
		rc.right = rc.left + dc.GetTextExtent(strText).cx + 6;

		CRect rcWork = CXTPDrawHelpers::GetWorkArea(m_pParentWnd);
		if (rc.right > rcWork.right)
			rc.OffsetRect(rcWork.right - rc.right, 0);
		if (rc.left < 0)
			rc.OffsetRect(-rc.left, 0);
		MoveWindow(rc);
		Invalidate(FALSE);

	}
	SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, (bActive? SWP_SHOWWINDOW: SWP_HIDEWINDOW|SWP_NOZORDER)|SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);

}

BOOL CXTPReportTip::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CXTPReportTip::OnPaint()
{
	CPaintDC dc(this);
	CXTPClientRect rc(this);

	COLORREF clrText = GetXtremeColor(COLOR_INFOTEXT);
	dc.FillSolidRect(rc, GetXtremeColor(COLOR_INFOBK));
	dc.Draw3dRect(rc, clrText, clrText);

	dc.SetTextColor(clrText);
	dc.SetBkMode(TRANSPARENT);

	CXTPFontDC font(&dc, &m_fntToolTip);
	CRect rcText(rc);
	rcText.left += 3;
	rcText.top -= 2;

	dc.DrawText(m_strTooltipText, rcText,  DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);

}
