// GUI_WinZipView.cpp : implementation of the CGUI_WinZipView class
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
#include "GUI_WinZip.h"

#include "GUI_WinZipDoc.h"
#include "GUI_WinZipView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUI_WinZipView

IMPLEMENT_DYNCREATE(CGUI_WinZipView, CListViewBase)

BEGIN_MESSAGE_MAP(CGUI_WinZipView, CListViewBase)
	//{{AFX_MSG_MAP(CGUI_WinZipView)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUI_WinZipView construction/destruction

CGUI_WinZipView::CGUI_WinZipView()
{
	// TODO: add construction code here

}

CGUI_WinZipView::~CGUI_WinZipView()
{
}

BOOL CGUI_WinZipView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= LVS_REPORT|LVS_AUTOARRANGE;
	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_WinZipView drawing

void CGUI_WinZipView::OnDraw(CDC* /*pDC*/)
{
	CGUI_WinZipDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

struct col_data
{
	LPCTSTR name;
	int width;
	int fmt;
};

static col_data columns[] =
{
	{ _T("Name"),       200, LVCFMT_LEFT},
	{ _T("Type"),       100, LVCFMT_LEFT},
	{ _T("Modified"),   120, LVCFMT_LEFT},
	{ _T("Size"),        80, LVCFMT_RIGHT},
	{ _T("Ratio"),       70, LVCFMT_RIGHT},
	{ _T("Packed"),     100, LVCFMT_RIGHT},
	{ _T("Attributes"),  90, LVCFMT_LEFT},
	{ _T("Path"),       200, LVCFMT_LEFT}
};

struct file_info
{
	LPCSTR info[_countof(columns)];
};

static file_info infoarr[] =
{
	// Name                 Type                    Modified        Size        Ratio   Packed     Attributes  Path
	{ "GUI_WinZip.aps",    "APS file",             "18/06/2002",   "134908",   "66%",  "45965",    "A",        "" },
	{ "GUI_WinZip.clw",    "CLW file",             "18/06/2002",   "4474",     "78%",  "1029",     "A",        "" },
	{ "GUI_WinZip.cpp",    "C++ Source File",      "06/06/2002",   "4364",     "62%",  "1679",     "RA",       "" },
	{ "GUI_WinZip.dsp",    "Project File",         "18/06/2002",   "5231",     "76%",  "1276",     "A",        "" },
	{ "GUI_WinZip.dsw",    "Project Workspace",    "07/06/2002",   "626",      "61%",  "246",      "RA",       "" },
	{ "GUI_WinZip.h",      "C Header File",        "06/06/2002",   "1400",     "55%",  "640",      "RA",       "" },
	{ "GUI_WinZip.ico",    "Icon",                 "06/06/2002",   "1078",     "73%",  "298",      "RA",       "res" },
	{ "GUI_WinZip.obj",    "Intermediate file",    "18/06/2002",   "24855",    "69%",  "7785",     "A",        "Debug" },
	{ "GUI_WinZip.pch",    "Intermediate file",    "18/06/2002",   "8326332",  "68%",  "689319",   "A",        "Debug" },
	{ "GUI_WinZip.rc",     "Resource Template",    "18/06/2002",   "16370",    "78%",  "3644",     "A",        "" },
	{ "GUI_WinZip.rc2",    "RC2 file",             "06/06/2002",   "402",      "57%",  "175",      "RA",       "res" },
	{ "GUI_WinZip.res",    "RES file",             "18/06/2002",   "93564",    "62%",  "35772",    "A",        "Debug" },
	{ "GUI_WinZip.Tags.WW","WW file",              "18/06/2002",   "1044",     "81%",  "208",      "A",        "" },
	{ "GUI_WinZipD.pdb",   "Intermediate file",    "18/06/2002",   "508928",   "77%",  "20292",    "A",        "Debug" },
	{ "GUI_WinZipDoc.cpp", "C++ Source File",      "06/06/2002",   "1822",     "67%",  "619",      "RA",       "" },
	{ "GUI_WinZipDoc.h",   "C Header File",        "06/06/2002",   "1519",     "57%",  "666",      "RA",       "" },
	{ "GUI_WinZipDoc.ico", "Icon",                 "06/06/2002",   "1078",     "68%",  "352",      "RA",       "res" },
	{ "GUI_WinZipDoc.obj", "Intermediate file",    "18/06/2002",   "16481",    "69%",  "5272",     "A",        "Debug" },
	{ "GUI_WinZipView.cpp","C++ Source File",      "18/06/2002",   "3680",     "65%",  "1295",     "A",        "" },
	{ "GUI_WinZipView.h",  "C Header File",        "18/06/2002",   "1995",     "58%",  "838",      "A",        "" },
	{ "GUI_WinZipView.obj","Intermediate file",    "18/06/2002",   "27203",    "69%",  "8468",     "A",        "Debug" },
	{ "highclr1.bmp",      "Bitmap Image",         "06/06/2002",   "35334",    "67%",  "11879",    "RA",       "res" },
	{ "highclr2.bmp",      "Bitmap Image",         "06/06/2002",   "35334",    "65%",  "12594",    "RA",       "res" },
	{ "highclrsm1.bmp",    "Bitmap Image",         "07/06/2002",   "5814",     "31%",  "4053",     "RA",       "res" },
	{ "highclrsm2.bmp",    "Bitmap Image",         "07/06/2002",   "5814",     "36%",  "3758",     "RA",       "res" },
	{ "MainFrm.cpp",       "C++ Source File",      "18/06/2002",   "8535",     "75%",  "2201",     "A",        "" },
	{ "MainFrm.h",         "C Header File",        "17/06/2002",   "2153",     "63%",  "807",      "RA",       "" },
	{ "MainFrm.obj",       "Intermediate file",    "18/06/2002",   "41818",    "72%",  "11972",    "A",        "Debug" },
	{ "manifest.xml",      "XML Document",         "17/06/2002",   "599",      "46%",  "329",      "RA",       "res" },
	{ "ReadMe.txt",        "Readme Document",      "06/06/2002",   "4407",     "64%",  "1590",     "RA",       "" },
	{ "resource.h",        "C Header File",        "17/06/2002",   "1713",     "70%",  "516",      "A",        "" },
	{ "StdAfx.cpp",        "C++ Source File",      "06/06/2002",   "212",      "32%",  "145",      "RA",       "" },
	{ "StdAfx.h",          "C Header File",        "06/06/2002",   "1148",     "52%",  "557",      "RA",       "" },
	{ "StdAfx.obj",        "Intermediate file",    "18/06/2002",   "113867",   "58%",  "48738",    "A",        "Debug" },
	{ "Toolbar.bmp",       "Bitmap Image",         "06/06/2002",   "1078",     "61%",  "424",      "RA",       "res" },
	{ "vc60.idb",          "Intermediate file",    "18/06/2002",   "427008",   "75%",  "8543",     "A",        "Debug" },
	{ "vc60.pdb",          "Intermediate file",    "18/06/2002",   "651264",   "69%",  "2461",     "A",        "Debug" }
};


////////

void CGUI_WinZipView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	CListCtrl& listCtrl = GetListCtrl();
	int i;
	for (i = 0; i < _countof(columns); ++i)
	{
		listCtrl.InsertColumn(i, columns[i].name, columns[i].fmt,
			columns[i].width);
	}

	::SendMessage(listCtrl.GetSafeHwnd(), LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);

	int j;
	for (i = 0; i < _countof(infoarr); ++i)
	{
		CString strItem1 = infoarr[i].info[0];
		listCtrl.InsertItem(i, strItem1);

		for (j = 1; j < _countof(columns); ++j)
		{
			CString strItem2 = infoarr[i].info[j];
			listCtrl.SetItem(i, j, LVIF_TEXT, strItem2, 0, 0, 0, 0);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_WinZipView printing

BOOL CGUI_WinZipView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGUI_WinZipView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGUI_WinZipView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_WinZipView diagnostics

#ifdef _DEBUG
void CGUI_WinZipView::AssertValid() const
{
	CListView::AssertValid();
}

void CGUI_WinZipView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CGUI_WinZipDoc* CGUI_WinZipView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGUI_WinZipDoc)));
	return (CGUI_WinZipDoc*)m_pDocument;
}
#endif //_DEBUG
