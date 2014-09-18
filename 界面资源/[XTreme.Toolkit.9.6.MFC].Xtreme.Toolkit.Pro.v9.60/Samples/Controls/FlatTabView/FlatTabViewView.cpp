// FlatTabViewView.cpp : implementation of the CFlatTabViewView class
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
#include "FlatTabView.h"

#include "FlatTabViewDoc.h"
#include "FlatTabViewView.h"
#include "SampleFormView.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlatTabViewView

IMPLEMENT_DYNCREATE(CFlatTabViewView, CView)

BEGIN_MESSAGE_MAP(CFlatTabViewView, CView)
	//{{AFX_MSG_MAP(CFlatTabViewView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlatTabViewView construction/destruction

CFlatTabViewView::CFlatTabViewView()
: m_pSheet7(0)
, m_pFlatTabCtrl(0)
{
	// TODO: add construction code here
	m_FTS_XT_BOTTOM = TRUE;
	m_FTS_XT_HASARROWS = TRUE;
	m_FTS_XT_HASHOMEEND = FALSE;
	m_FTS_XT_HSCROLL = TRUE;
	m_dwTabStyle    = -1;
}

CFlatTabViewView::~CFlatTabViewView()
{
	SAFE_DELETE( m_pFlatTabCtrl );
}

BOOL CFlatTabViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CView::PreCreateWindow(cs))
		return FALSE;

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= WS_CLIPCHILDREN;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFlatTabViewView drawing

void CFlatTabViewView::OnDraw(CDC*)
{
}

/////////////////////////////////////////////////////////////////////////////
// CFlatTabViewView printing

BOOL CFlatTabViewView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFlatTabViewView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFlatTabViewView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFlatTabViewView diagnostics

#ifdef _DEBUG
void CFlatTabViewView::AssertValid() const
{
	CView::AssertValid();
}

void CFlatTabViewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFlatTabViewDoc* CFlatTabViewView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFlatTabViewDoc)));
	return (CFlatTabViewDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFlatTabViewView message handlers

int CFlatTabViewView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create the tabs
	UpdateTabsStyle();

	// set the background color for our flicker free templates.
	m_sheet1.SetBackColor(GetXtremeColor(COLOR_WINDOW));
	m_sheet2.SetBackColor(GetXtremeColor(COLOR_WINDOW));
	m_sheet3.SetBackColor(GetXtremeColor(COLOR_WINDOW));
	m_sheet4.SetBackColor(GetXtremeColor(COLOR_WINDOW));
	m_sheet5.SetBackColor(GetXtremeColor(COLOR_WINDOW));
	m_sheet6.SetBackColor(GetXtremeColor(COLOR_WINDOW));

	return 0;
}

void CFlatTabViewView::UpdateTabsStyle()
{
	int dwNewStyle = 0;
	UpdateData();
	if (m_FTS_XT_HASHOMEEND)
	{
		dwNewStyle |= FTS_XT_HASHOMEEND;
		m_FTS_XT_HASARROWS = TRUE;
		UpdateData(FALSE);
	}

	if (m_FTS_XT_BOTTOM)     dwNewStyle |= FTS_XT_BOTTOM;
	if (m_FTS_XT_HASARROWS)  dwNewStyle |= FTS_XT_HASARROWS;
	if (m_FTS_XT_HSCROLL)    dwNewStyle |= FTS_XT_HSCROLL;
	dwNewStyle |= FTS_XT_TOOLTIPS;
	if (dwNewStyle != m_dwTabStyle)
	{
		m_dwTabStyle = dwNewStyle;
		CreateTabs();
	}
}

void CFlatTabViewView::CreateTabs()
{
	SetRedraw(false);

	int iDefaultSel = 0;
	if (m_pFlatTabCtrl)
	{
		m_pFlatTabCtrl->GetWindowRect(m_rcTabs);
		ScreenToClient(m_rcTabs);

		iDefaultSel = m_pFlatTabCtrl->GetCurSel();
		m_pFlatTabCtrl->DestroyWindow();
		SAFE_DELETE( m_pFlatTabCtrl );
	}

	m_pFlatTabCtrl = new CXTFlatTabCtrl;
	CChildFrame* pParentFrame = ( CChildFrame* )GetParentFrame( );
	if ( pParentFrame && pParentFrame->IsKindOf( RUNTIME_CLASS( CChildFrame ) ) )
	{
		pParentFrame->m_pFlatTabCtrl = m_pFlatTabCtrl;
		pParentFrame->m_pFlatTabView = this;
	}

	// Create  the flat tab control.
	if (!m_pFlatTabCtrl->Create(WS_CHILD|WS_VISIBLE|m_dwTabStyle,
		m_rcTabs, this, IDC_FLAT_TABCTRL))
	{
		TRACE0( "Failed to create flattab control\n" );
		return;
	}

	CreateViews();

	// Insert tabs into the flat tab control.
	m_pFlatTabCtrl->InsertItem(0, _T("Build"),           &m_sheet1);
	m_pFlatTabCtrl->InsertItem(1, _T("Debug"),           &m_sheet2);
	m_pFlatTabCtrl->InsertItem(2, _T("Find in Files 1"), &m_sheet3);
	m_pFlatTabCtrl->InsertItem(3, _T("Find in Files 2"), &m_sheet4);
	m_pFlatTabCtrl->InsertItem(4, _T("Results"),         &m_sheet5);
	m_pFlatTabCtrl->InsertItem(5, _T("SQL Debugging"),   &m_sheet6);
	m_pFlatTabCtrl->InsertItem(6, _T("Form View"),       m_pSheet7);

	// Insert the Test Scroll View into the CXTFlatTabCtrl
	m_pFlatTabCtrl->InsertItem(7, _T("Test View"),       m_pTestView);

	m_pFlatTabCtrl->SetCurSel(iDefaultSel);

	m_pFlatTabCtrl->SetWindowPos(&wndTop, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_FRAMECHANGED);

	SetRedraw(true);

	RedrawWindow(0,0, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
}

int CFlatTabViewView::CreateViews()
{
	CRect rectDummy(0, 0, 0, 0);

	// Create the sheet1 list box.
	if ( ::IsWindow( m_sheet1.m_hWnd ) )
	{
		m_sheet1.DestroyWindow();
	}
	if (!m_sheet1.Create(WS_CHILD |
		LVS_REPORT | LVS_NOCOLUMNHEADER, rectDummy, m_pFlatTabCtrl, IDC_SHEET1 ))
	{
		TRACE0( "Failed to create sheet1.\n" );
		return -1;
	}
	ListView_SetExtendedListViewStyleEx(m_sheet1.m_hWnd,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	// Create the sheet2 list box.
	if ( ::IsWindow( m_sheet2.m_hWnd ) )
	{
		m_sheet2.DestroyWindow();
	}
	if (!m_sheet2.Create( WS_CHILD | WS_TABSTOP | WS_CLIPCHILDREN |
		WS_HSCROLL | WS_VSCROLL | ES_MULTILINE, rectDummy, m_pFlatTabCtrl, IDC_SHEET2 ))
	{
		TRACE0( "Failed to create sheet2.\n" );
		return -1;
	}
	m_sheet2.SetFont(m_sheet1.GetFont());

	// Define the default style for the output list boxes.
	DWORD dwStyle = WS_CHILD | LBS_NOINTEGRALHEIGHT | WS_TABSTOP | WS_VSCROLL;

	// Create the sheet3 list box.
	if ( ::IsWindow( m_sheet3.m_hWnd ) )
	{
		m_sheet3.DestroyWindow();
	}
	if (!m_sheet3.Create(
		WS_CHILD | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | WS_VSCROLL,
		rectDummy, m_pFlatTabCtrl, IDC_SHEET3 ))
	{
		TRACE0( "Failed to create sheet3.\n" );
		return -1;
	}

	// Create the sheet4 list box.
	if ( ::IsWindow( m_sheet4.m_hWnd ) )
	{
		m_sheet4.DestroyWindow();
	}
	if (!m_sheet4.Create( dwStyle, rectDummy, m_pFlatTabCtrl, IDC_SHEET4 ))
	{
		TRACE0( "Failed to create sheet4.\n" );
		return -1;
	}

	// Create the sheet5 list box.
	if ( ::IsWindow( m_sheet5.m_hWnd ) )
	{
		m_sheet5.DestroyWindow();
	}
	if (!m_sheet5.Create( dwStyle, rectDummy, m_pFlatTabCtrl, IDC_SHEET5 ))
	{
		TRACE0( "Failed to create sheet5.\n" );
		return -1;
	}

	// Create the sheet6 list box.
	if ( ::IsWindow( m_sheet6.m_hWnd ) )
	{
		m_sheet6.DestroyWindow();
	}
	if (!m_sheet6.Create( dwStyle, rectDummy, m_pFlatTabCtrl, IDC_SHEET6 ))
	{
		TRACE0( "Failed to create sheet6.\n" );
		return -1;
	}

	// views must be created dynamically because they are destroyed during
	// window destruction
	if ( m_pSheet7 && ::IsWindow( m_pSheet7->m_hWnd ) )
	{
		m_pSheet7->DestroyWindow();
	}
	CCreateContext cc;
	ZeroMemory(&cc, sizeof(cc));
	m_pSheet7 = (CSampleFormView*)RUNTIME_CLASS(CSampleFormView)->CreateObject();
	if (!m_pSheet7  || !((CWnd *)m_pSheet7)->Create(
		NULL, NULL, WS_CHILD, rectDummy, m_pFlatTabCtrl, IDC_SHEET7, &cc))
	{
		TRACE0( "Failed to create sheet7.\n" );
		return -1;
	}

	// Create the Test Scroll View
	m_pTestView = (CTestScrollView*)RUNTIME_CLASS(CTestScrollView)->CreateObject();
	if (!m_pTestView  || !((CWnd *)m_pTestView)->Create( NULL, NULL, WS_CHILD |
		WS_VSCROLL|WS_HSCROLL, rectDummy, m_pFlatTabCtrl, IDC_SHEET7 + 1, &cc))
	{
		TRACE0( "Failed to create m_pTestView.\n" );
		return -1;
	}

	// Insert text into the list box.
	m_sheet1.InsertColumn(0, _T("Column"), LVCFMT_LEFT, 512);
	m_sheet1.InsertItem(0, _T("--------------------Configuration: FlatTabCtrl - Win32 Debug--------------------"));
	m_sheet1.InsertItem(1, _T("Compiling resources..."));
	m_sheet1.InsertItem(2, _T("Compiling..."));
	m_sheet1.InsertItem(3, _T("StdAfx.cpp"));
	m_sheet1.InsertItem(5, _T("Compiling..."));
	m_sheet1.InsertItem(6, _T("AboutDlg.cpp"));
	m_sheet1.InsertItem(7, _T("FlatTabCtrl.cpp"));
	m_sheet1.InsertItem(8, _T("FlatTabCtrlDlg.cpp"));
	m_sheet1.InsertItem(9, _T("Generating Code..."));
	m_sheet1.InsertItem(10, _T("Linking..."));
	m_sheet1.InsertItem(11, _T(""));
	m_sheet1.InsertItem(12, _T("FlatTabCtrl.exe - 0 error(s), 0 warning(s)"));

//  m_sheet2.AddString(_T("Debug Tab Output..."));

	m_sheet2.SetWindowText(
		_T("Loaded 'C:\\WINNT\\System32\\ntdll.dll', no matching symbolic information found.\r\n")
		_T("Loaded symbols for 'C:\\WINNT\\system32\\MFC42D.DLL'\r\n")
		_T("Loaded symbols for 'C:\\WINNT\\system32\\MSVCRTD.DLL'\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\KERNEL32.DLL', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\GDI32.DLL', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\USER32.DLL', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\ADVAPI32.DLL', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\rpcrt4.dll', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\SHELL32.DLL', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\shlwapi.dll', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\comctl32.dll', no matching symbolic information found.\r\n")
		_T("Loaded symbols for 'C:\\WINNT\\system32\\MFCO42D.DLL'\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\OLE32.DLL', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\OLEAUT32.DLL', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\lpk.dll', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\usp10.dll', no matching symbolic information found.\r\n")
		_T("XTTOOLKIT_DLL.DLL Initializing!\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\indicdll.dll', no matching symbolic information found.\r\n")
		_T("Loaded 'C:\\WINNT\\system32\\imm32.dll', no matching symbolic information found.\r\n")
		_T("The program 'D:\\prj\\XT\\Toolkit\\Bin\\FlatTabCtrl.exe' has exited with code 0 (0x0).\r\n"));

///sheet3.AddString(_T("Find in Files 1 Tab Output..."));
	HTREEITEM file;
	file = m_sheet3.InsertItem(_T("D:\\Toolkit\\Samples\\FlatTabCtrl\\EditCtrlForFlatTab.cpp"));
	m_sheet3.InsertItem(_T("Line 92:            pSB->Create(WS_CHILD, CRect(0, 0, 0, 0), (CWnd *)this, 0)"), 0, 0, file);
	file = m_sheet3.InsertItem(_T("D:\\Toolkit\\Samples\\FlatTabCtrl\\FlatTabCtrl.rc"));
	m_sheet3.InsertItem(_T("Line 190:STYLE WS_CHILD"), 0, 0, file);
	file = m_sheet3.InsertItem(_T("D:\\Toolkit\\Samples\\FlatTabCtrl\\FlatTabCtrlDlg.cpp"));
	m_sheet3.InsertItem(_T("Line 92:            pSB->Create(WS_CHILD, CRect(0, 0, 0, 0), (CWnd *)this, 0)"), 0, 0, file);
	m_sheet3.InsertItem(_T("Line 265:   if (!m_pFlatTabCtrl->Create(WS_CHILD|WS_VISIBLE|m_dwTabStyle,"), 0, 0, file);
	m_sheet3.InsertItem(_T("Line 293:   if (!m_sheet1.Create(WS_CHILD |  "), 0, 0, file);
	m_sheet3.InsertItem(_T("Line 303:   if (!m_sheet2.Create( WS_CHILD | WS_TABSTOP | WS_CLIPCHILDREN |"), 0, 0, file);
	m_sheet3.InsertItem(_T("Line 312:   DWORD dwStyle = WS_CHILD | LBS_NOINTEGRALHEIGHT | WS_TABSTOP | WS_VSCROLL;"), 0, 0, file);
	m_sheet3.InsertItem(_T("Line 316:       WS_CHILD | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | WS_VSCROLL, "), 0, 0, file);
	m_sheet3.InsertItem(_T("Line 348:        !((CWnd *)m_pSheet7)->Create(NULL, NULL, WS_CHILD, "), 0, 0, file);
	file = m_sheet3.InsertItem(_T("D:\\Toolkit\\Samples\\FlatTabCtrl\\SampleFormView.cpp"));
	m_sheet3.InsertItem(_T("Line 79:    m_ScrollBarH.Create(WS_CHILD, CRect(0, 0, 0, 0), this, 0);"), 0, 0, file);

	m_sheet4.AddString(_T("Find in Files 2 Tab Output..."));
	m_sheet5.AddString(_T("Results Tab Output..."));
	m_sheet6.AddString(_T("SQL Debugging Tab Output..."));

	// set the font for the list boxes
	m_sheet4.SetFont(&XTAuxData().font);
	m_sheet5.SetFont(&XTAuxData().font);
	m_sheet6.SetFont(&XTAuxData().font);

	return TRUE;
}

void CFlatTabViewView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_pFlatTabCtrl && ::IsWindow(m_pFlatTabCtrl->m_hWnd))
	{
		m_pFlatTabCtrl->SetWindowPos(NULL, 0,0,cx,cy, SWP_FRAMECHANGED);
	}
}

void CFlatTabViewView::OnActivateView(BOOL /*bActivate*/, CView* /*pActivateView*/, CView* /*pDeactiveView*/)
{
	UpdateActiveView();
}

BOOL CFlatTabViewView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (IDC_FLAT_TABCTRL == (UINT)wParam)
	{
		NMHDR* pNMHDR = (NMHDR*)lParam;

		switch(pNMHDR->code)
		{
		case TCN_SELCHANGING:
			break;

		case TCN_SELCHANGE:
			UpdateActiveView();
			break;
		}
	}

	return CView::OnNotify(wParam, lParam, pResult);
}

// This function ensures that the child pane is the actual active view (and
// not this container)
void CFlatTabViewView::UpdateActiveView()
{
	// any tab selected?
	const int iSel = m_pFlatTabCtrl->GetCurSel();
	if (iSel >= 0)
	{
		// is the corresponding child window a view?
		CWnd * const pWndActive = m_pFlatTabCtrl->GetItemWindow(iSel);
		if (pWndActive->GetRuntimeClass()->IsDerivedFrom(RUNTIME_CLASS(CView)))
		{
			// yes - make it the active view
			GetParentFrame()->SetActiveView((CView *)pWndActive);
		}
	}
}
