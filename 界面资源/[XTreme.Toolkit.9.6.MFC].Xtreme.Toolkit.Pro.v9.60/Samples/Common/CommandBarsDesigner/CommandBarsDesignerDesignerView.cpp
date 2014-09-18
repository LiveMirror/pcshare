// CommandBarsDesignerDesignerView.cpp : implementation file
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
#include "commandbarsdesigner.h"
#include "commandbarsdesignerDoc.h"
#include "CommandBarsDesignerDesignerView.h"
#include "EmbeddedFrame.h"
#include "MainFrm.h"
#include "DialogResourceSymbols.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerDesignerView

IMPLEMENT_DYNCREATE(CCommandBarsDesignerDesignerView, CView)

BEGIN_MESSAGE_MAP(CCommandBarsDesignerDesignerView, CView)
	//{{AFX_MSG_MAP(CCommandBarsDesignerDesignerView)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_COMMAND(ID_COMMANDBARS_SETTINGS, OnCommandbarsSettings)
	ON_COMMAND(ID_COMMANDBARS_RESOURCESYMBOLS, OnCommandbarsResourcesymbols)
	ON_COMMAND(ID_FILE_EXPORT_XMLDOCUMENT, OnFileExportXmldocument)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_XMLDOCUMENT, OnUpdateFileExportXmldocument)
	ON_COMMAND(ID_DESIGN_MODE, OnDesignMode)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_MODE, OnUpdateDesignMode)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerDesignerView construction/destruction

CCommandBarsDesignerDesignerView::CCommandBarsDesignerDesignerView()
{
	m_pFrame  = NULL;
}

CCommandBarsDesignerDesignerView::~CCommandBarsDesignerDesignerView()
{
}

BOOL CCommandBarsDesignerDesignerView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerDesignerView drawing

void CCommandBarsDesignerDesignerView::OnDraw(CDC* pDC)
{
	CCommandBarsDesignerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	//CRect rcFrame;
	//m_pFrame->GetWindowRect(&rcFrame);
	//ScreenToClient(&rcFrame);

	m_rectTracker.Draw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerDesignerView printing

BOOL CCommandBarsDesignerDesignerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCommandBarsDesignerDesignerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCommandBarsDesignerDesignerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerDesignerView diagnostics

#ifdef _DEBUG
void CCommandBarsDesignerDesignerView::AssertValid() const
{
	CView::AssertValid();
}

void CCommandBarsDesignerDesignerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCommandBarsDesignerDoc* CCommandBarsDesignerDesignerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCommandBarsDesignerDoc)));
	return (CCommandBarsDesignerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCommandBarsDesignerDesignerView message handlers


int CCommandBarsDesignerDesignerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_pFrame == NULL)
	{

		m_pFrame = new CEmbeddedFrame();

		VERIFY(m_pFrame->LoadFrame(IDR_EMBEDDEDFRAME,
			WS_CHILD|WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, AfxGetMainWnd(), NULL));

		((CEmbeddedFrame*)m_pFrame)->m_pHostView = this;
	}

	return 0;
}


void CCommandBarsDesignerDesignerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	ASSERT(m_pFrame);

	m_pFrame->SetParent(this);
	CRect rc (m_rectTracker.m_rect);
	m_pFrame->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW|SWP_NOZORDER|SWP_NOACTIVATE);

	OnUpdateFrameTitle();
}
void CCommandBarsDesignerDesignerView::OnUpdateFrameTitle()
{
	m_pFrame->SetWindowText(GetDocument()->GetTitle());

}

BOOL CCommandBarsDesignerDesignerView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// convert cursor position to client co-ordinates
	CPoint point;
	GetCursorPos(&point);
	pWnd->ScreenToClient(&point);

	int nHit = m_rectTracker.HitTest(point);
	switch(nHit) {
		case CRectTracker::hitRight:
			::SetCursor(::LoadCursor(0, IDC_SIZEWE));
			return TRUE;
		case CRectTracker::hitBottom:
			::SetCursor(::LoadCursor(0, IDC_SIZENS));
			return TRUE;
		case CRectTracker::hitBottomRight:
			::SetCursor(::LoadCursor(0, IDC_SIZENWSE));
			return TRUE;
	}

	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CCommandBarsDesignerDesignerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	int nHit = m_rectTracker.HitTest(point);
	if (nHit != CRectTracker::hitMiddle)
	{
		ASSERT(m_pFrame);

		if (m_rectTracker.Track(this, point))
		{
			CRect rc (m_rectTracker.m_rect);
			m_pFrame->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER);
			Invalidate(TRUE);

		}
	}
	CView::OnLButtonDown(nFlags, point);
}

void CCommandBarsDesignerDesignerView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: Add your specialized code here and/or call the base class

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);


	((CMainFrame*)AfxGetMainWnd())->RefreshPanes();
}

void CCommandBarsDesignerDesignerView::OnCommandbarsSettings()
{
	m_pFrame->ShowPropertyPage();

}

void CCommandBarsDesignerDesignerView::OnCommandbarsResourcesymbols()
{
	CDialogResourceSymbols ds(m_pFrame);
	ds.DoModal();

}

void CCommandBarsDesignerDesignerView::OnFileExportXmldocument()
{
	CString strFilter = _T("XML Document(*.xml)|*.xml|All files (*.*)|*.*||");

	CFileDialog fd(FALSE, _T("xml"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter);
	if (fd.DoModal() != IDOK)
		return;

	CXTPPropExchangeXMLNode px(FALSE, 0, _T("DesignerFile"));
	m_pFrame->DoPropExchange(&px);

	px.SaveToFile(fd.GetFileName());
}

void CCommandBarsDesignerDesignerView::OnUpdateFileExportXmldocument(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsXMLEngineAvailable());
}


void CCommandBarsDesignerDesignerView::OnDesignMode()
{
	m_pFrame->GetCommandBars()->SetCustomizeMode(!m_pFrame->GetCommandBars()->IsCustomizeMode());
}

void CCommandBarsDesignerDesignerView::OnUpdateDesignMode(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_pFrame->GetCommandBars()->IsCustomizeMode()? TRUE: FALSE);

}
