// XTResizeGroupBox.cpp : implementation file
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
#include "XTResizeGroupBox.h"
#include "XTGlobal.h"
#include "XTMemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTResizeGroupBox

CXTResizeGroupBox::CXTResizeGroupBox()
{
}

CXTResizeGroupBox::~CXTResizeGroupBox()
{
}

IMPLEMENT_DYNAMIC(CXTResizeGroupBox, CButton)

BEGIN_MESSAGE_MAP(CXTResizeGroupBox, CButton)
	//{{AFX_MSG_MAP(CXTResizeGroupBox)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTResizeGroupBox message handlers

void CXTResizeGroupBox::Exclude(CDC* pDC, CRect& rcClient)
{
	// get a pointer to the parent.
	CWnd* pWndParent = GetParent();
	if (!pWndParent)
		return;

	// get a pointer to the parents first child.
	CWnd* pWnd = pWndParent->GetWindow(GW_CHILD);
	if (pWnd == NULL)
		return;

	// iterate thru all children and exclude those children that
	// are located inside the group box.
	CWnd* pChildWnd = pWnd->GetWindow(GW_HWNDFIRST);
	while (pChildWnd != NULL)
	{
		// make sure we do not exclude ourself
		if (pChildWnd != this)
		{
			CString str;
			pChildWnd->GetWindowText(str);

			CRect rc;
			pChildWnd->GetWindowRect(&rc);
			ScreenToClient(&rc);

			// if the parent's child is located in our group box, exclude
			// it from painting.
			if (rcClient.PtInRect(rc.TopLeft()) ||
				rcClient.PtInRect(rc.BottomRight()))
			{
				pDC->ExcludeClipRect(&rc);
			}
		}

		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}
}

BOOL CXTResizeGroupBox::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return FALSE;
}

void CXTResizeGroupBox::OnPaint()
{
	CPaintDC dc(this);

	// get the client area size.
	CRect rcClient;
	GetClientRect(&rcClient);

	// exclude controls that we "group"
	Exclude(&dc, rcClient);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTMemDC memDC(&dc, rcClient);

	// Let the window do its default painting...
	CButton::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
}
