// NotificationsView.cpp : implementation of the CNotificationsView class
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
#include "Notifications.h"

#include "NotificationsDoc.h"
#include "NotificationsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNotificationsView

IMPLEMENT_DYNCREATE(CNotificationsView, CView)

BEGIN_MESSAGE_MAP(CNotificationsView, CView)
	//{{AFX_MSG_MAP(CNotificationsView)
	ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullscreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FULLSCREEN, OnUpdateViewFullscreen)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNotificationsView construction/destruction

CNotificationsView::CNotificationsView()
{
	// TODO: add construction code here

}

CNotificationsView::~CNotificationsView()
{
}

BOOL CNotificationsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CNotificationsView drawing

void CNotificationsView::OnDraw(CDC* /*pDC*/)
{
	CNotificationsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CNotificationsView printing

BOOL CNotificationsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CNotificationsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CNotificationsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CNotificationsView diagnostics

#ifdef _DEBUG
void CNotificationsView::AssertValid() const
{
	CView::AssertValid();
}

void CNotificationsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNotificationsDoc* CNotificationsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNotificationsDoc)));
	return (CNotificationsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNotificationsView message handlers

void CNotificationsView::OnViewFullscreen()
{
	CWnd* pWnd = GetParent();
	if ((pWnd->GetStyle() & WS_MAXIMIZE) != 0)
		pWnd->ShowWindow(SW_NORMAL);
	else pWnd->ShowWindow(SW_MAXIMIZE);

}

void CNotificationsView::OnUpdateViewFullscreen(CCmdUI* pCmdUI)
{
	CWnd* pWnd = GetParent();

	CString strCaption = (pWnd->GetStyle() & WS_MAXIMIZE) != 0? _T("Normal"): _T("Maximized");

	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmdUI->m_pOther;
	if (pToolBar)
	{
		CXTPControl* pControl = pToolBar->GetControls()->GetAt(pCmdUI->m_nIndex);
		if (pControl->GetCaption() != strCaption)
		{
			pControl->SetCaption(strCaption);
			pToolBar->DelayLayout();
		}
	}
}
