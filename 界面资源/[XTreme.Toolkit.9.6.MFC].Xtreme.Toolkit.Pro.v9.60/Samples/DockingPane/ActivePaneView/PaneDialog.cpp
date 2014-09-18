// PaneDialog.cpp : implementation file
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
#include "ActivePaneView.h"
#include "PaneDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaneDialog dialog

CPaneDialog::CPaneDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPaneDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPaneDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPaneDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPaneDialog)
	DDX_Control(pDX, IDC_TXT_DIALOG, m_txtDialog);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPaneDialog, CDialog)
	//{{AFX_MSG_MAP(CPaneDialog)
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_COMMAND(ID_VIEW_ONE, OnViewOne)
	ON_COMMAND(ID_VIEW_TWO, OnViewTwo)
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaneDialog message handlers

void CPaneDialog::OnNcPaint()
{
	CWindowDC dc(this);

	CXTPWindowRect rect(this);

	int cx = rect.Width();
	int cy = rect.Height();

	const COLORREF clrFrame =  ::GetSysColor(COLOR_3DSHADOW);
	dc.Draw3dRect(0, 0, cx, cy, clrFrame, clrFrame);
}

void CPaneDialog::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	// adjust non-client area for border space
	lpncsp->rgrc[0].left   += 1;
	lpncsp->rgrc[0].top    += 1;
	lpncsp->rgrc[0].right  -= 1;
	lpncsp->rgrc[0].bottom -= 1;
}

BOOL CPaneDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_txtDialog.SetFont(&theApp.m_font);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPaneDialog::OnViewOne()
{
	// alert user.
	AfxMessageBox(_T("Dialog Pane - No. 1"));

	// set input focus
	SetFocus();
}

void CPaneDialog::OnViewTwo()
{
	// alert user.
	AfxMessageBox(_T("Dialog Pane - No. 2"));

	// set input focus
	SetFocus();
}

void CPaneDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDown(nFlags, point);

	// set input focus
	SetFocus();
}

void CPaneDialog::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CDialog::OnWindowPosChanged(lpwndpos);

	// TODO: Add your message handler code here
	if (::IsWindow(m_txtDialog.m_hWnd))
	{
		CXTPWindowRect rect(&m_txtDialog);
		ScreenToClient(&rect);

		int cx = rect.Width();
		int cy = rect.Height();

		rect.top    = (lpwndpos->cy-cy)/2;
		rect.bottom = rect.top + cy;
		rect.left   = (lpwndpos->cx-cx)/2;
		rect.right  = rect.left + cx;

		m_txtDialog.MoveWindow(&rect);
		m_txtDialog.Invalidate();
	}
}
