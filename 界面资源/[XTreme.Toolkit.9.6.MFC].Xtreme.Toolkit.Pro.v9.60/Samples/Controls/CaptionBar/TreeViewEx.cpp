// TreeViewEx.cpp : implementation of the CTreeViewEx class
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
#include "CaptionBar.h"

#include "ListViewExDoc.h"
#include "TreeViewEx.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeViewEx

IMPLEMENT_DYNCREATE(CTreeViewEx, CView)

BEGIN_MESSAGE_MAP(CTreeViewEx, CView)
	//{{AFX_MSG_MAP(CTreeViewEx)
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_BN_CLICKED(IDC_CAPT_BUTTON, OnCaptButton)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeViewEx construction/destruction

CTreeViewEx::CTreeViewEx()
{

}

CTreeViewEx::~CTreeViewEx()
{
	m_ilTreeIcons.DeleteImageList();
}

/////////////////////////////////////////////////////////////////////////////
// CTreeViewEx drawing

void CTreeViewEx::OnDraw(CDC*)
{
}

/////////////////////////////////////////////////////////////////////////////
// CTreeViewEx printing

BOOL CTreeViewEx::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTreeViewEx::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTreeViewEx::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTreeViewEx::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to m_wndTreeCtrl.
}

/////////////////////////////////////////////////////////////////////////////
// CTreeViewEx diagnostics

#ifdef _DEBUG
void CTreeViewEx::AssertValid() const
{
	CView::AssertValid();
}

void CTreeViewEx::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CListViewExDoc* CTreeViewEx::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CListViewExDoc)));
	return (CListViewExDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTreeViewEx message handlers

int CTreeViewEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the caption.
	if (!m_wndCaption.Create( this, _T("Folder List"), CPWS_EX_RAISED_EDGE | CPWS_EX_CLOSEBUTTON,
		WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, CRect(0,0,0,0), IDC_CAPT_BUTTON ))
	{
		TRACE0( "Unable to create caption.\n" );
		return -1;
	}

	// create the tree control.
	if (!m_wndTreeCtrl.Create (WS_VISIBLE|TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS,
		CRect(0,0,0,0), this, IDC_TREE_VIEW))
	{
		TRACE0( "Unable to create tree control.\n" );
		return -1;
	}

	// create the image lists used by the caption and tree controls.
	if (!InitializeImageLists())
		return -1;

	// fill the tree control.
	if (!InitilalizeTreeControl())
		return -1;

	return 0;
}

void CTreeViewEx::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CView::OnWindowPosChanged(lpwndpos);

	// Move the caption and tree control to their correct locations.
	if( ::IsWindow( m_wndCaption.m_hWnd ) )
	{
		::SetWindowPos( m_wndCaption.m_hWnd, NULL, 0, 0, lpwndpos->cx, 19,
			SWP_NOZORDER | SWP_NOACTIVATE );
	}
	if( ::IsWindow( m_wndTreeCtrl.m_hWnd ) )
	{
		::SetWindowPos( m_wndTreeCtrl.m_hWnd, NULL, 0, 19, lpwndpos->cx, lpwndpos->cy-19,
			SWP_NOZORDER | SWP_NOACTIVATE );
	}
}

void CTreeViewEx::OnCaptButton()
{
	CMainFrame* pFrameWnd = (CMainFrame*)GetParentFrame();
	ASSERT_KINDOF (CMainFrame, pFrameWnd);
	pFrameWnd->CloseTreeViewPane();
}

BOOL CTreeViewEx::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CView::PreCreateWindow( cs ))
		return FALSE;

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	return TRUE;
}

BOOL CTreeViewEx::InitilalizeTreeControl()
{
	// Set the tree controls image list.
	m_wndTreeCtrl.SetImageList (&m_ilTreeIcons, TVSIL_NORMAL);

	// TODO: Add items to the tree control.
	HTREEITEM htItem = m_wndTreeCtrl.InsertItem(_T("Parent Item"));

	// Add children
	int i;
	for(i = 1; i < 5; i++ ) {
		m_wndTreeCtrl.InsertItem (_T("Child of Parent Item"), 0, 0, htItem, TVI_LAST);
	}

	m_wndTreeCtrl.Expand(htItem, TVE_EXPAND);
	m_wndTreeCtrl.EnableMultiSelect(FALSE);

	return TRUE;
}

BOOL CTreeViewEx::InitializeImageLists()
{
	// Create the image list used by the tree control.
	if (!m_ilTreeIcons.Create(16, 16, ILC_MASK|ILC_COLOR24, 2, 1))
		return FALSE;

	// TODO: Load your icons that you want displayed in the tree
	// control here, then add them to the image list.
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_CAPTIOTYPE);
	m_ilTreeIcons.Add(hIcon);

	return TRUE;
}
