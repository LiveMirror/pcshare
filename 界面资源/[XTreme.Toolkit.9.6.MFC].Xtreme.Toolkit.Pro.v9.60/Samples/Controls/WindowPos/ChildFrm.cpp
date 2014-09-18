// ChildFrm.cpp : implementation of the CChildFrame class
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
#include "WindowPos.h"

#include "ChildFrm.h"
#include "WindowPosDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	m_strWindowPos = _T("");
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::ActivateFrame(int nCmdShow)
{
	// Let the frame activate first...
	CMDIChildWnd::ActivateFrame(nCmdShow);

	// Create the format string for saving the window position to
	// the system registry.  You can increment this number for each
	// document type that you want to save position for.
	int iDocType = 0;

	CView* pView = GetActiveView();
	if (pView)
	{
		CDocument* pDocument = pView->GetDocument();
		if (pDocument)
		{
			if (pDocument->IsKindOf(RUNTIME_CLASS(CWindowPosDoc))) {
				iDocType = 1;
			}
//          else if (pDocument->IsKindOf(RUNTIME_CLASS(CMyDoc2))) {
//              iDocType = 2;
//          }
//          else if (pDocument->IsKindOf(RUNTIME_CLASS(CMyDoc3))) {
//              iDocType = 3;
//          }
		}
	}

	m_strWindowPos.Format(IDS_REG_MDI_WNDPOS, iDocType);

	// then restore the previous window position.
	m_wndPosition.LoadWindowPos(this, m_strWindowPos);
}

void CChildFrame::OnDestroy()
{
	// Save frame window size and position.
	m_wndPosition.SaveWindowPos(this, m_strWindowPos);

	CMDIChildWnd::OnDestroy();
}
