// XTPPreviewView.cpp : implementation file
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
#include "PrintPreview.h"
#include "XTPPreviewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPPreviewView

IMPLEMENT_DYNCREATE(CXTPPreviewView, CPreviewView)

CXTPPreviewView::CXTPPreviewView()
: m_pBtnNumPage(0)
{

}

BEGIN_MESSAGE_MAP(CXTPPreviewView, CPreviewView)
	//{{AFX_MSG_MAP(CXTPPreviewView)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_NUMPAGE, OnUpdateNumPageChange)
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CXTPPreviewView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPreviewView::OnCreate(lpCreateStruct) == -1)
		return -1;

	ASSERT_VALID (m_pToolBar); // dialog bar must be valid.

	// Create the toolbar that is used in the dialog bar.
	if (!m_wndToolBar.CreateToolBar( WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS, m_pToolBar) ||
		!m_wndToolBar.LoadToolBar( XTP_IDR_PREVIEW_TOOLBAR ) )
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar.SetOwner(this);

	// save a pointer to the numPage button.
	m_pBtnNumPage = FindToolbarButton(AFX_ID_PREVIEW_NUMPAGE);

	// hide the one and two page buttons.
	FindToolbarButton(AFX_IDS_ONEPAGE)->SetVisible(FALSE);
	FindToolbarButton(AFX_IDS_TWOPAGE)->SetVisible(FALSE);

	// display text for print and close buttons.
	FindToolbarButton(AFX_ID_PREVIEW_PRINT)->SetStyle(xtpButtonIconAndCaption);
	FindToolbarButton(AFX_ID_PREVIEW_CLOSE)->SetStyle(xtpButtonIconAndCaption);

	UpdateNumPageIcon();

	return 0;
}

CXTPControlButton* CXTPPreviewView::FindToolbarButton(UINT nCommandID)
{
	return ((CXTPControlButton*)m_wndToolBar.GetControls()->
		FindControl(xtpControlButton, nCommandID, TRUE, FALSE));
}

void CXTPPreviewView::UpdateNumPageIcon()
{
	// Get the current display state.
	UINT nPages = m_nZoomState == ZOOM_OUT ? m_nPages : m_nZoomOutPages;
	int nIcon = nPages != 1? AFX_IDS_ONEPAGE: AFX_IDS_TWOPAGE;

	if (m_pBtnNumPage->GetIconId() != nIcon)
	{
		m_pBtnNumPage->SetIconId(nIcon);
	}
}

void CXTPPreviewView::OnUpdateNumPageChange(CCmdUI* pCmdUI)
{
	CPreviewView::OnUpdateNumPageChange(pCmdUI);

	// Reload the toolbar
	UpdateNumPageIcon();
}

void CXTPPreviewView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CPreviewView::OnWindowPosChanged(lpwndpos);

	// If the toolbar is a window...
	if ( ::IsWindow( m_wndToolBar.m_hWnd ) )
	{
		// Get a pointer to the parent frame.
		CFrameWnd* pFrameWnd = m_pToolBar->GetDockingFrame( );
		ASSERT_VALID( pFrameWnd );

		// Get the size of the client area for the docking frame.
		CRect r;
		pFrameWnd->GetClientRect( &r );

		// Calculate the actual size of the toolbar.
		CSize size = m_wndToolBar.CalcDockingLayout(r.Width(), /*LM_HIDEWRAP| */ LM_HORZDOCK | LM_HORZ | LM_COMMIT);
		size.cx = r.Width( );

		// Set the height of the dialog bar equal to the
		// toolbar's height.
		m_pToolBar->m_sizeDefault.cy = size.cy;

		// Reposition the toolbar.
		m_wndToolBar.MoveWindow( 0, 0, size.cx, size.cy );
		m_wndToolBar.Invalidate(FALSE);
	}
}
