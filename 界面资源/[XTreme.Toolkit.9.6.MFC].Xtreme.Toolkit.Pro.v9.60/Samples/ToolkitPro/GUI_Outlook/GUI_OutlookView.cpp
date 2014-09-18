// GUI_OutlookView.cpp : implementation of the CGUI_OutlookView class
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
#include "GUI_Outlook.h"

#include "GUI_OutlookDoc.h"
#include "GUI_OutlookView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUI_OutlookView

IMPLEMENT_DYNCREATE(CGUI_OutlookView, CXTListView)

BEGIN_MESSAGE_MAP(CGUI_OutlookView, CXTListView)
	//{{AFX_MSG_MAP(CGUI_OutlookView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CXTListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CXTListView::OnFilePrint)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUI_OutlookView construction/destruction

CGUI_OutlookView::CGUI_OutlookView()
{
	// TODO: add construction code here

}

CGUI_OutlookView::~CGUI_OutlookView()
{
}

BOOL CGUI_OutlookView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= LVS_REPORT;

	return CXTListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_OutlookView drawing

void CGUI_OutlookView::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_OutlookView printing

BOOL CGUI_OutlookView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGUI_OutlookView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGUI_OutlookView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_OutlookView diagnostics

#ifdef _DEBUG
void CGUI_OutlookView::AssertValid() const
{
	CXTListView::AssertValid();
}

void CGUI_OutlookView::Dump(CDumpContext& dc) const
{
	CXTListView::Dump(dc);
}

CGUI_OutlookDoc* CGUI_OutlookView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGUI_OutlookDoc)));
	return (CGUI_OutlookDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGUI_OutlookView message handlers

static int arHeaderBmps[] = {
	IDB_HEADER_1,
	IDB_HEADER_3
};

static int arColWidths[] = {
	24,
	21,
	100,
	180,
	120
};

static CString arColLabels[] = {
	_T(" "),
	_T(" "),
	_T("From"),
	_T("Subject"),
	_T("Received")
};

int CGUI_OutlookView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the image list.
	if (!m_imageList.Create(IDB_ENV, 14, 1, RGB(0xFF,0x00,0xFF)))
		return -1;

	InitializeHeaderControl();
	InitializeListControl();
	m_flatHeader.SetTheme(0);

	SetExtendedStyle (LVS_EX_FULLROWSELECT);

	SetMinimumColSize(15);

	return 0;
}

bool CGUI_OutlookView::SortList(int /*nCol*/, bool /*bAscending*/)
{
	CXTSortClass csc (&GetListCtrl(), m_nSortedCol);
	csc.Sort (m_bAscending, xtSortString);
	return true;
}

void CGUI_OutlookView::InitializeHeaderControl()
{
	// Insert the columns.
	BuildColumns(_countof(arColWidths), arColWidths, arColLabels);

	// Subclass the flat header control.
	SubclassHeader();

	// add images to the header columns.
	int nCol;
	for (nCol = 0; nCol < _countof(arHeaderBmps); ++nCol) {
		m_flatHeader.SetBitmap(nCol, arHeaderBmps[nCol]/*, HDF_STRING*/);
	}

	// use sort arrows.
	m_flatHeader.ShowSortArrow(TRUE);
}

void CGUI_OutlookView::InitializeListControl()
{
	// build a date string to insert into the list control.
	CTime tm = CTime::GetCurrentTime();
	CString strDay[] = { _T("Sun"),_T("Mon"),_T("Tue"),_T("Wed"),_T("Thr"),_T("Fri"),_T("Sat") };
	CString strTime;

	strTime.Format(_T("%s %d/%d/%d"), strDay[tm.GetDayOfWeek()-1],
		tm.GetMonth(), tm.GetDay(), tm.GetYear());

	// set the image list for the list control.
	CListCtrl& rList = GetListCtrl();
	rList.SetImageList(&m_imageList, LVSIL_SMALL);

	// insert the first item.
	rList.InsertItem(0, NULL, 0);
	rList.SetItemText(0, 2, _T("Codejock Software"));
	rList.SetItemText(0, 3, _T("Xtreme Toolkit has arrived!"));
	rList.SetItemText(0, 4, strTime);

	// inset the second item.
	rList.InsertItem(1, NULL, 0);
	rList.SetItemText(1, 2, _T("Joe Programmer"));
	rList.SetItemText(1, 3, _T("How can I create an Outlook style application?"));
	rList.SetItemText(1, 4, strTime);

	EnableUserSortColor(true);

	// enable autosizing for columns.
	m_flatHeader.EnableAutoSize();

	// lock the first two colums from sizing operations.
	m_flatHeader.FreezeColumn(0);
	m_flatHeader.FreezeColumn(1);
}
