// CommandBarsDesignerView.cpp : implementation of the CCommandBarsDesignerView class
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
#include "CommandBarsDesigner.h"

#include "CommandBarsDesignerDoc.h"
#include "CommandBarsDesignerView.h"
#include "CommandBarsDesignerDesignerView.h"

#include "EmbeddedFrame.h"
#include "MainFrm.h"
#include "DialogResourceSymbols.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_TABCONTROL 100

/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerView

IMPLEMENT_DYNCREATE(CCommandBarsDesignerView, CView)

BEGIN_MESSAGE_MAP(CCommandBarsDesignerView, CView)
	//{{AFX_MSG_MAP(CCommandBarsDesignerView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCONTROL, OnSelectedChanged)
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerView construction/destruction

CCommandBarsDesignerView::CCommandBarsDesignerView()
{
}

CCommandBarsDesignerView::~CCommandBarsDesignerView()
{
}

BOOL CCommandBarsDesignerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if (!CView::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;

}

/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerView drawing

void CCommandBarsDesignerView::OnDraw(CDC*)
{
}

/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerView diagnostics

#ifdef _DEBUG
void CCommandBarsDesignerView::AssertValid() const
{
	CView::AssertValid();
}

void CCommandBarsDesignerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCommandBarsDesignerDoc* CCommandBarsDesignerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCommandBarsDesignerDoc)));
	return (CCommandBarsDesignerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerView message handlers

BOOL CCommandBarsDesignerView::AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon)
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

int CCommandBarsDesignerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndTabControl.Create(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, IDC_TABCONTROL);
	m_wndTabControl.GetPaintManager()->SetAppearance(xtpTabAppearanceStateButtons);
	m_wndTabControl.GetPaintManager()->SetPosition(xtpTabPositionBottom);
	m_wndTabControl.GetPaintManager()->m_bShowIcons = TRUE;

	AddView(RUNTIME_CLASS(CCommandBarsDesignerDesignerView), _T("Designer"), 3);
	AddView(RUNTIME_CLASS(CCommandBarsDesignerEditView), _T("XML"), 0);

	if (!IsXMLEngineAvailable())
	{
		m_wndTabControl.GetItem(1)->SetEnabled(FALSE);
	}

	return 0;
}

BOOL CCommandBarsDesignerView::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CCommandBarsDesignerView::OnPaint()
{
	CPaintDC dc(this);
}

void CCommandBarsDesignerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_wndTabControl.GetSafeHwnd())
	{
		m_wndTabControl.MoveWindow(0, 0, cx, cy);
	}

}

void CCommandBarsDesignerView::OnInitialUpdate()
{
	CCommandBarsDesignerDesignerView* pView =  DYNAMIC_DOWNCAST(CCommandBarsDesignerDesignerView,
		CWnd::FromHandlePermanent(m_wndTabControl.GetItem(0)->GetHandle()));

	pView->OnInitialUpdate();
}


void CCommandBarsDesignerView::UpdateDocTitle()
{
	GetDocument()->UpdateFrameCounts();
}

void CCommandBarsDesignerView::OnSelectedChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNUSED_ALWAYS(pNMHDR);
	*pResult = 0;

	UpdateDocTitle();

	CFrameWnd* pFrame = GetParentFrame();
	CView* pView = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetSelectedItem()->GetHandle()));
	ASSERT_KINDOF(CView, pView);

	if (m_wndTabControl.GetItemCount() == 2)
	{

		if (pView->IsKindOf(RUNTIME_CLASS(CCommandBarsDesignerEditView)))
		{
			GetEmbeddedFrame()->GetCommandBars()->SetDragControl(0);


			CXTPPropExchangeXMLNode px(FALSE, 0, _T("DesignerFile"));
			if (!px.OnBeforeExchange())
			{
				m_wndTabControl.SetCurSel(0);
				return;
			}
			GetEmbeddedFrame()->DoPropExchange(&px);

			CString str = px.GetXML(TRUE);
			((CEditView*)pView)->SetWindowText(str);
			((CEditView*)pView)->GetEditCtrl().SetModify(FALSE);
		}
		else
		{
			CCommandBarsDesignerEditView* pEditView =  DYNAMIC_DOWNCAST(CCommandBarsDesignerEditView,
				CWnd::FromHandlePermanent(m_wndTabControl.GetItem(1)->GetHandle()));

			if (pEditView->GetEditCtrl().GetModify())
			{
				CString str;
				pEditView->GetWindowText(str);

				CXTPPropExchangeXMLNode px(TRUE, NULL, _T("DesignerFile"));
				if (px.LoadFromString(str))
				{
					GetEmbeddedFrame()->DoPropExchange(&px);
					OnInitialUpdate();

					((CMainFrame*)AfxGetMainWnd())->RefreshPanes();
				}
			}
		}
	}

	pFrame->SetActiveView(pView);
}


int CCommandBarsDesignerView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CCommandBarsDesignerView::OnUpdateFrameTitle()
{

}

void CCommandBarsDesignerView::Serialize(CArchive& ar)
{
	GetEmbeddedFrame()->Serialize(ar);


}

CEmbeddedFrame* CCommandBarsDesignerView::GetEmbeddedFrame()
{
	CCommandBarsDesignerDesignerView* pView =  DYNAMIC_DOWNCAST(CCommandBarsDesignerDesignerView,
		CWnd::FromHandlePermanent(m_wndTabControl.GetItem(0)->GetHandle()));

	ASSERT(pView);
	if (pView)
		return pView->m_pFrame;


	return 0;
}
