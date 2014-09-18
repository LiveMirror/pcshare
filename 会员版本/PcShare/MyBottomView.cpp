// MyBottomView.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyBottomView.h"
#include "MyLogsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyBottomView

IMPLEMENT_DYNCREATE(CMyBottomView, CView)

CMyBottomView::CMyBottomView()
{
	pLogView = NULL;
}

CMyBottomView::~CMyBottomView()
{
}

#define IDC_TABCONTROL_BOTTOM2	0x9954

BEGIN_MESSAGE_MAP(CMyBottomView, CView)
	//{{AFX_MSG_MAP(CMyBottomView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_NOFILETRAN, OnNoFileTran)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCONTROL_BOTTOM2, OnSelectedChanged)
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyBottomView drawing

void CMyBottomView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMyBottomView diagnostics

#ifdef _DEBUG
void CMyBottomView::AssertValid() const
{
	CView::AssertValid();
}

void CMyBottomView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyBottomView message handlers

BOOL CMyBottomView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if(!CView::PreCreateWindow(cs))
	{
		return FALSE;
	}
	cs.style &= ~WS_BORDER;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return TRUE;
}

CWnd* CMyBottomView::AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon)
{
	CCreateContext contextT;
	contextT.m_pCurrentDoc     = NULL;
	contextT.m_pNewViewClass   = pViewClass;
	contextT.m_pNewDocTemplate = NULL;

	CWnd* pWnd;
	TRY
	{
		pWnd = (CWnd*) pViewClass->CreateObject();
		if (pWnd == NULL)
		{
			AfxThrowMemoryException();
		}
	}
	CATCH_ALL(e)
	{
		TRACE0( "Out of memory creating a view.\n" );
		// Note: DELETE_EXCEPTION(e) not required
		return NULL;
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

	CXTPTabManagerItem* pItem = m_wndTabControl.InsertItem(nTab, lpszTitle, pWnd->GetSafeHwnd(), nIcon);
	pItem->SetData((DWORD) pWnd);
	pWnd->SendMessage(WM_INITIALUPDATE);
	pWnd->SetOwner(this);
	return pWnd;
}

int CMyBottomView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	m_wndTabControl.Create(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, IDC_TABCONTROL_BOTTOM2);
	m_wndTabControl.GetPaintManager()->SetAppearance(xtpTabAppearancePropertyPage2003);
	m_wndTabControl.GetPaintManager()->m_bHotTracking = TRUE;
	m_wndTabControl.GetPaintManager()->m_bShowIcons = TRUE;
	m_wndTabControl.GetPaintManager()->DisableLunaColors(FALSE);

	pLogView = AddView(RUNTIME_CLASS(CMyLogsView), _T("日志信息"), 10);
	return 0;
}

BOOL CMyBottomView::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CMyBottomView::OnPaint()
{
	CPaintDC dc(this);
}

void CMyBottomView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	if(m_wndTabControl.GetSafeHwnd())
	{
		m_wndTabControl.MoveWindow(0, 0, cx, cy);
	}
}

void CMyBottomView::OnSelectedChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	if(m_wndTabControl.GetItemCount() <= 0)
	{
		return;
	}

	UNUSED_ALWAYS(pNMHDR);
	*pResult = 0;
	CView* pView = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetSelectedItem()->GetHandle()));
	ASSERT_KINDOF(CView, pView);
	GetParentFrame()->SetActiveView(pView);
}

int CMyBottomView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CMyBottomView::SetCurSelView(int nItem)
{
	m_wndTabControl.SetCurSel(nItem);
}

LRESULT CMyBottomView::OnNoFileTran(WPARAM wParam, LPARAM lParam)
{
	SetCurSelView(0);
	return TRUE;
}