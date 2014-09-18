// TabbedViewView.cpp : implementation of the CTabbedViewView class
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
#include "TabbedView.h"

#include "TabbedViewDoc.h"
#include "TabbedViewView.h"

#include "FileTreeView.h"
#include "ClassTreeView.h"
#include "ResourceTreeView.h"
#include "PropertiesView.h"
#include "EditControlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_TABCONTROL 100

/////////////////////////////////////////////////////////////////////////////
// CTabbedViewView

IMPLEMENT_DYNCREATE(CTabbedViewView, CView)

BEGIN_MESSAGE_MAP(CTabbedViewView, CView)
	//{{AFX_MSG_MAP(CTabbedViewView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCONTROL, OnSelectedChanged)
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabbedViewView construction/destruction

CTabbedViewView::CTabbedViewView()
{
}

CTabbedViewView::~CTabbedViewView()
{
}

BOOL CTabbedViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if (!CView::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;

}

/////////////////////////////////////////////////////////////////////////////
// CTabbedViewView drawing

void CTabbedViewView::OnDraw(CDC*)
{
}

/////////////////////////////////////////////////////////////////////////////
// CTabbedViewView diagnostics

#ifdef _DEBUG
void CTabbedViewView::AssertValid() const
{
	CView::AssertValid();
}

void CTabbedViewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTabbedViewDoc* CTabbedViewView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTabbedViewDoc)));
	return (CTabbedViewDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTabbedViewView message handlers

BOOL CTabbedViewView::AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon)
{
	CCreateContext contextT;
	contextT.m_pCurrentDoc     = GetDocument();
	contextT.m_pNewViewClass   = pViewClass;
	contextT.m_pNewDocTemplate = GetDocument()->GetDocTemplate();

	CWnd* pWnd;
	TRY
	{
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
		{
			AfxThrowMemoryException();
		}
	}
	CATCH_ALL(e)
	{
		TRACE0( "Out of memory creating a view.\n" );
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL

	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;
	dwStyle &= ~WS_BORDER;

	int nTab = m_wndTabControl.GetItemCount();

	// Create with the right size (wrong position)
	CRect rect(0,0,0,0);
	if (!pWnd->Create(NULL, NULL, dwStyle,
		rect, &m_wndTabControl, (AFX_IDW_PANE_FIRST + nTab), &contextT))
	{
		TRACE0( "Warning: couldn't create client tab for view.\n" );
		// pWnd will be cleaned up by PostNcDestroy
		return NULL;
	}
	m_wndTabControl.InsertItem(nTab, lpszTitle, pWnd->GetSafeHwnd(), nIcon);

	pWnd->SendMessage(WM_INITIALUPDATE);
	pWnd->SetOwner(this);

	return TRUE;


}

int CTabbedViewView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndTabControl.Create(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, IDC_TABCONTROL);
	m_wndTabControl.GetPaintManager()->SetAppearance(xtpTabAppearancePropertyPage2003);
	m_wndTabControl.GetPaintManager()->m_bHotTracking = TRUE;
	m_wndTabControl.GetPaintManager()->m_bShowIcons = TRUE;
	m_wndTabControl.GetPaintManager()->DisableLunaColors(FALSE);

	m_wndTabControl.GetImageManager()->SetIcons(IDB_TAB_ICONS, NULL, 0, CSize(16, 16), xtpImageNormal);

	AddView(RUNTIME_CLASS(CEditControlView), _T("Edit Control"), 3);
	AddView(RUNTIME_CLASS(CFileTreeView), _T("File Tree"), 0);
	AddView(RUNTIME_CLASS(CClassTreeView), _T("Class Tree"), 4);
	AddView(RUNTIME_CLASS(CResourceTreeView), _T("Resource Tree"), 2);
	AddView(RUNTIME_CLASS(CPropertiesView), _T("Properties"), 1);

	return 0;
}

BOOL CTabbedViewView::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CTabbedViewView::OnPaint()
{
	CPaintDC dc(this);
}

void CTabbedViewView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_wndTabControl.GetSafeHwnd())
	{
		m_wndTabControl.MoveWindow(0, 0, cx, cy);
	}

}

void CTabbedViewView::UpdateDocTitle()
{
	GetDocument()->UpdateFrameCounts();
}

void CTabbedViewView::OnSelectedChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNUSED_ALWAYS(pNMHDR);
	*pResult = 0;

	UpdateDocTitle();

	CFrameWnd* pFrame = GetParentFrame();
	CView* pView = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetSelectedItem()->GetHandle()));
	ASSERT_KINDOF(CView, pView);

	pFrame->SetActiveView(pView);
}


int CTabbedViewView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
