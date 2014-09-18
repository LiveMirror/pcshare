// ReportSampleView.cpp : implementation of the CReportSampleView class
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
#include "ReportSample.h"

#include "MainFrm.h"
#include "ReportSampleDoc.h"
#include "ReportSampleView.h"
#include "ReportMultilinePaintManager.h"

#include "MessageRecord.h"
#include "TreeViewDlg.h"
#include "PerfomanceTestDlg.h"
#include "TaskListView.h"
#include "PropertiesView.h"

#include "float.h"

//#define XML_STATE

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLUMN_IMPORTANCE       0
#define COLUMN_ICON             1
#define COLUMN_ATTACHMENT       2
#define COLUMN_FROM             3
#define COLUMN_SUBJECT          4
#define COLUMN_SENT             5
#define COLUMN_SIZE             6
#define COLUMN_CHECK            7
#define COLUMN_PRICE            8
#define COLUMN_CREATED          9
#define COLUMN_RECIEVED         10
#define COLUMN_CONVERSATION     11
#define COLUMN_CONTACTS         12
#define COLUMN_MESSAGE          13
#define COLUMN_CC               14
#define COLUMN_CATEGORIES       15
#define COLUMN_AUTOFORWARD      16
#define COLUMN_DO_NOT_AUTOARCH  17
#define COLUMN_DUE_BY           18

#define COLUMN_MAIL_ICON    0
#define COLUMN_CHECK_ICON   2

/////////////////////////////////////////////////////////////////////////////
// CReportSampleView

IMPLEMENT_DYNCREATE(CReportSampleView, CXTPReportView)

BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
	//{{AFX_MSG_MAP(CReportSampleView)
	ON_WM_CREATE()
	ON_COMMAND(ID_ENABLE_PREVIEW, OnEnablePreview)
	ON_UPDATE_COMMAND_UI(ID_ENABLE_PREVIEW, OnUpdateEnablePreview)
	ON_COMMAND(ID_TEST_AUTOGROUPING, OnAutoGrouping)
	ON_UPDATE_COMMAND_UI(ID_TEST_AUTOGROUPING, OnUpdateAutoGrouping)
	ON_COMMAND(ID_TEST_GROUPBY, OnGroupBy)
	ON_UPDATE_COMMAND_UI(ID_TEST_GROUPBY, OnUpdateGroupBy)
	ON_COMMAND(ID_TEST_GROUPSHADE, OnGroupShade)
	ON_UPDATE_COMMAND_UI(ID_TEST_GROUPSHADE, OnUpdateGroupShade)
	ON_COMMAND(ID_TEST_TREEVIEWDIALOG, OnTreeViewDlg)
	ON_COMMAND(ID_REPORTCONTROL_AUTOMATICFORMATTINGSAMPLE, OnReportcontrolAutomaticformattingsample)
	ON_UPDATE_COMMAND_UI(ID_REPORTCONTROL_AUTOMATICFORMATTINGSAMPLE, OnUpdateReportcontrolAutomaticformattingsample)
	ON_COMMAND(ID_OPTIONS_ALLOWCOLUMNRESIZE, OnOptionsAllowcolumnresize)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ALLOWCOLUMNRESIZE, OnUpdateOptionsAllowcolumnresize)
	ON_COMMAND(ID_OPTIONS_ALLOWCOLUMNSREMOVE, OnOptionsAllowcolumnsremove)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ALLOWCOLUMNSREMOVE, OnUpdateOptionsAllowcolumnsremove)
	ON_COMMAND(ID_OPTIONS_MULTIPLESELECTION, OnOptionsMultipleselection)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_MULTIPLESELECTION, OnUpdateOptionsMultipleselection)
	ON_COMMAND(ID_OPTIONS_SHOWITEMSINGROUPS, OnOptionsShowitemsingroups)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_SHOWITEMSINGROUPS, OnUpdateOptionsShowitemsingroups)
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_COMMAND(ID_TEST_TASKLIST, OnTestTasklist)
	ON_COMMAND(ID_TEST_PERFOMANCE, OnTestPerfomance)
	ON_COMMAND(ID_TEST_PROPERTIES, OnTestProperties)
	ON_COMMAND(ID_OPTIONS_AUTOMATICCOLUMNSIZING, OnOptionsAutomaticcolumnsizing)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_AUTOMATICCOLUMNSIZING, OnUpdateOptionsAutomaticcolumnsizing)
	ON_COMMAND(ID_REPORTCONTROL_MULTILINESAMPLE, OnReportcontrolMultilinesample)
	ON_UPDATE_COMMAND_UI(ID_REPORTCONTROL_MULTILINESAMPLE, OnUpdateReportcontrolMultilinesample)
	//}}AFX_MSG_MAP
	// Standard printing commands

	ON_COMMAND_RANGE(ID_GRID_HORIZONTAL_NOGRIDLINES, ID_GRID_HORIZONTAL_SOLID, OnGridHorizontal)
	ON_UPDATE_COMMAND_UI_RANGE(ID_GRID_HORIZONTAL_NOGRIDLINES, ID_GRID_HORIZONTAL_SOLID, OnUpdateGridHorizontal)

	ON_COMMAND_RANGE(ID_GRID_VERTICAL_NOGRIDLINES, ID_GRID_VERTICAL_SOLID, OnGridVertical)
	ON_UPDATE_COMMAND_UI_RANGE(ID_GRID_VERTICAL_NOGRIDLINES, ID_GRID_VERTICAL_SOLID, OnUpdateGridVertical)

	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

	ON_NOTIFY(NM_CLICK, XTP_ID_REPORT_CONTROL, OnReportItemClick)
	ON_NOTIFY(XTP_NM_REPORT_HYPERLINK , XTP_ID_REPORT_CONTROL, OnReportHyperlinkClick)
	ON_NOTIFY(NM_RCLICK, XTP_ID_REPORT_CONTROL, OnReportItemRClick)
	ON_NOTIFY(NM_DBLCLK, XTP_ID_REPORT_CONTROL, OnReportItemDblClick)
	ON_NOTIFY(XTP_NM_REPORT_HEADER_RCLICK, XTP_ID_REPORT_CONTROL, OnReportColumnRClick)
	ON_NOTIFY(NM_KEYDOWN, XTP_ID_REPORT_CONTROL, OnReportKeyDown)
	ON_NOTIFY(LVN_BEGINDRAG, XTP_ID_REPORT_CONTROL, OnReportBeginDrag)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportSampleView construction/destruction

CReportSampleView::CReportSampleView()
{
	m_bAutomaticFormating = FALSE;
	m_pTaskFrame = 0;
	m_pPropertiesFrame = 0;
	m_bMultilineSample = FALSE;

}

CReportSampleView::~CReportSampleView()
{
}

BOOL CReportSampleView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CView::PreCreateWindow(cs))
		return FALSE;

	///cs.dwExStyle |= WS_EX_STATICEDGE;
	//cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;

}

/////////////////////////////////////////////////////////////////////////////
// CReportSampleView diagnostics

#ifdef _DEBUG
void CReportSampleView::AssertValid() const
{
	CView::AssertValid();
}

void CReportSampleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CReportSampleDoc* CReportSampleView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CReportSampleDoc)));
	return (CReportSampleDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CReportSampleView message handlers

int CReportSampleView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPReportView::OnCreate(lpCreateStruct) == -1)
		return -1;


	VERIFY(m_ilIcons.Create(16,16, ILC_COLOR24|ILC_MASK, 0, 1));
	CBitmap bmp;
	VERIFY(bmp.LoadBitmap(IDB_BMREPORT));
	m_ilIcons.Add(&bmp, RGB(255, 0, 255));

	m_wndReport.SetImageList(&m_ilIcons);

	//
	//  Add sample columns
	//


	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_IMPORTANCE, _T("Importance"), 18, FALSE, 1));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_ICON, _T("Message Class"), 18, FALSE, COLUMN_MAIL_ICON));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_ATTACHMENT, _T("Attachment"), 18, FALSE, 7));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_FROM, _T("From"), 100));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_SUBJECT, _T("Subject"), 180));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_RECIEVED, _T("Recieved"), 80));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_SIZE, _T("Size"), 55, FALSE));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_CHECK, _T("Checked"), 18, FALSE, COLUMN_CHECK_ICON));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_PRICE, _T("Price"), 80, TRUE, XTP_REPORT_NOICON, TRUE, FALSE));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_SENT, _T("Sent"), 150, TRUE, XTP_REPORT_NOICON, TRUE, FALSE));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_CREATED, _T("Created"), 80, TRUE, XTP_REPORT_NOICON, TRUE, FALSE));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_CONVERSATION, _T("Conversation"), 30, TRUE, XTP_REPORT_NOICON, TRUE, FALSE));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_CONTACTS, _T("Contacts"), 30, TRUE, XTP_REPORT_NOICON, TRUE, FALSE));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_MESSAGE, _T("Message"), 80, TRUE, XTP_REPORT_NOICON, TRUE, FALSE));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_CC, _T("CC"), 80, TRUE, XTP_REPORT_NOICON, TRUE, FALSE));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_CATEGORIES, _T("Categories"), 30, TRUE, XTP_REPORT_NOICON, TRUE, FALSE));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_AUTOFORWARD, _T("Auto Forward"), 30, TRUE, XTP_REPORT_NOICON, TRUE, FALSE));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_DO_NOT_AUTOARCH, _T("Do not autoarchive"), 30, TRUE, XTP_REPORT_NOICON, TRUE, FALSE));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_DUE_BY, _T("Due by"), 30, TRUE, XTP_REPORT_NOICON, TRUE, FALSE));




	//
	//  Add sample records
	//
	AddSampleRecords();

	// add sample hyperlinks


	//m_wndReport.GetRecords()->GetAt(6)->GetItem(COLUMN_FROM)->AddHyperlink(new CXTPReportHyperlink(7, 14));
	m_wndReport.GetRecords()->GetAt(3)->GetItem(COLUMN_FROM)->AddHyperlink(new CXTPReportHyperlink(12, 26));
	((CXTPReportRecordItem*)(m_wndReport.GetRecords()->GetAt(3)->GetItemPreview()))->AddHyperlink(new CXTPReportHyperlink(27, 7));
	((CXTPReportRecordItem*)(m_wndReport.GetRecords()->GetAt(3)->GetItemPreview()))->AddHyperlink(new CXTPReportHyperlink(110, 13));
	((CXTPReportRecordItem*)(m_wndReport.GetRecords()->GetAt(5)->GetItemPreview()))->AddHyperlink(new CXTPReportHyperlink(21, 26));
	((CXTPReportRecordItem*)(m_wndReport.GetRecords()->GetAt(5)->GetItemPreview()))->AddHyperlink(new CXTPReportHyperlink(123, 26));
	m_wndReport.GetRecords()->GetAt(8)->GetItem(COLUMN_FROM)->AddHyperlink(new CXTPReportHyperlink(0, 28));

	m_wndReport.GetRecords()->GetAt(3)->SetEditable(FALSE);

	OnGroupBy();

	OnAutoGrouping();
	m_wndReport.Populate();

	LoadReportState();

	return 0;
}

void CReportSampleView::OnDestroy()
{
	SaveReportState();

	if (m_pTaskFrame)
	{
		m_pTaskFrame->DestroyWindow();
	}
	if (m_pPropertiesFrame)
	{
		m_pPropertiesFrame->DestroyWindow();
	}

	CView::OnDestroy();
}


void CReportSampleView::OnEnablePreview()
{
	m_wndReport.EnablePreviewMode(!m_wndReport.IsPreviewMode());
	m_wndReport.Populate();
}

void CReportSampleView::OnUpdateEnablePreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndReport.IsPreviewMode());
}

void CReportSampleView::OnGroupBy()
{
	m_wndReport.ShowGroupBy(!m_wndReport.IsGroupByVisible());
}

void CReportSampleView::OnUpdateGroupBy(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndReport.IsGroupByVisible());
}

void CReportSampleView::OnGroupShade()
{
	m_wndReport.ShadeGroupHeadings(!m_wndReport.IsShadeGroupHeadingsEnabled());
}

void CReportSampleView::OnUpdateGroupShade(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndReport.IsShadeGroupHeadingsEnabled());
}

void CReportSampleView::OnAutoGrouping()
{
	if (m_wndReport.GetColumns()->GetGroupsOrder()->GetCount() == 0)
	{
		CXTPReportColumn* pColFrom = m_wndReport.GetColumns()->Find(COLUMN_FROM);

		m_wndReport.GetColumns()->GetGroupsOrder()->Clear();

		m_wndReport.GetColumns()->GetGroupsOrder()->Add(pColFrom);
	}
	else
	{
		m_wndReport.GetColumns()->GetGroupsOrder()->Clear();
	}
	m_wndReport.Populate();
}

void CReportSampleView::OnUpdateAutoGrouping(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndReport.GetColumns()->GetGroupsOrder()->GetCount() > 0);
}

void CReportSampleView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	if (m_wndSubList.GetSafeHwnd() == NULL)
	{
		m_wndSubList.SubclassDlgItem(IDC_COLUMNLIST, &pWnd->m_wndFieldChooser);
		m_wndReport.GetColumns()->GetReportHeader()->SetSubListCtrl(&m_wndSubList);
	}

	if (m_wndFilterEdit.GetSafeHwnd() == NULL)
	{
		m_wndFilterEdit.SubclassDlgItem(IDC_FILTEREDIT, &pWnd->m_wndFilterEdit);
		m_wndReport.GetColumns()->GetReportHeader()->SetFilterEditCtrl(&m_wndFilterEdit);
	}

}

void CReportSampleView::LoadReportState()
{
#ifdef XML_STATE
	CXTPPropExchangeXMLNode px(TRUE, 0, _T("ReportControl"));
	if (!px.LoadFromFile(_T("c:\\ReportControl.xml")))
		return;
	
	m_wndReport.DoPropExchange(&px);

#else	
	UINT nBytes = 0;
	LPBYTE pData = 0;

	if (!AfxGetApp()->GetProfileBinary(_T("ReportControl"), _T("State"), &pData, &nBytes))
		return;

	CMemFile memFile(pData, nBytes);
	CArchive ar (&memFile,CArchive::load);

	try
	{
		m_wndReport.SerializeState(ar);

	}
	catch (COleException* pEx)
	{
		pEx->Delete ();
	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
	}

	ar.Close();
	memFile.Close();
	delete[] pData;
#endif
}

void CReportSampleView::SaveReportState()
{
#ifdef XML_STATE
	
	CXTPPropExchangeXMLNode px(FALSE, 0, _T("ReportControl"));
	m_wndReport.DoPropExchange(&px);
	px.SaveToFile(_T("c:\\ReportControl.xml"));

#else
	CMemFile memFile;
	CArchive ar (&memFile,CArchive::store);

	m_wndReport.SerializeState(ar);

	ar.Flush();

	DWORD nBytes = (DWORD)memFile.GetPosition();
	LPBYTE pData = memFile.Detach();

	AfxGetApp()->WriteProfileBinary(_T("ReportControl"), _T("State"), pData, nBytes);

	ar.Close();
	memFile.Close();
	free(pData);
#endif

}


#define ID_REMOVE_ITEM  1
#define ID_SORT_ASC     2
#define ID_SORT_DESC        3
#define ID_GROUP_BYTHIS 4
#define ID_SHOW_GROUPBOX        5
#define ID_SHOW_FIELDCHOOSER 6
#define ID_COLUMN_BESTFIT       7
#define ID_COLUMN_ARRANGEBY 100
#define ID_COLUMN_ALIGMENT  200
#define ID_COLUMN_ALIGMENT_LEFT ID_COLUMN_ALIGMENT + 1
#define ID_COLUMN_ALIGMENT_RIGHT    ID_COLUMN_ALIGMENT + 2
#define ID_COLUMN_ALIGMENT_CENTER   ID_COLUMN_ALIGMENT + 3
#define ID_COLUMN_SHOW      500


CString LoadResourceString(UINT nID)
{
	CString str;
	VERIFY(str.LoadString(nID));
	return str;
}

void CReportSampleView::OnReportColumnRClick(NMHDR * pNotifyStruct, LRESULT * /*result*/)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
	ASSERT(pItemNotify->pColumn);
	CPoint ptClick = pItemNotify->pt;

	CMenu menu;
	VERIFY(menu.CreatePopupMenu());

	// create main menu items
	menu.AppendMenu(MF_STRING, ID_SORT_ASC, LoadResourceString(IDS_SORTASC));
	menu.AppendMenu(MF_STRING, ID_SORT_DESC, LoadResourceString(IDS_SORTDESC));
	menu.AppendMenu(MF_SEPARATOR, (UINT)-1, (LPCTSTR)NULL);
	menu.AppendMenu(MF_STRING, ID_GROUP_BYTHIS, LoadResourceString(IDS_GROUPBYFIELD));
	menu.AppendMenu(MF_STRING, ID_SHOW_GROUPBOX, LoadResourceString(IDS_GROUPBYBOX));
	menu.AppendMenu(MF_SEPARATOR, (UINT)-1, (LPCTSTR)NULL);
	menu.AppendMenu(MF_STRING, ID_REMOVE_ITEM, LoadResourceString(IDS_REMOVECOL));
	menu.AppendMenu(MF_STRING, ID_SHOW_FIELDCHOOSER, LoadResourceString(IDS_FIELDCHOOSER));
	menu.AppendMenu(MF_SEPARATOR, (UINT)-1, (LPCTSTR)NULL);
	menu.AppendMenu(MF_STRING, ID_COLUMN_BESTFIT, LoadResourceString(IDS_BESTFIT));

	if (m_wndReport.IsGroupByVisible())
	{
		menu.CheckMenuItem(ID_SHOW_GROUPBOX, MF_BYCOMMAND|MF_CHECKED);
	}
	if (m_wndReport.GetReportHeader()->IsShowItemsInGroups())
	{
		menu.EnableMenuItem(ID_GROUP_BYTHIS, MF_BYCOMMAND|MF_DISABLED);
	}


	CXTPReportColumns* pColumns = m_wndReport.GetColumns();
	CXTPReportColumn* pColumn = pItemNotify->pColumn;

	// create arrange by items
	CMenu menuArrange;
	VERIFY(menuArrange.CreatePopupMenu());
	int nColumnCount = pColumns->GetCount();
	for (int nColumn = 0; nColumn < nColumnCount; nColumn++)
	{
		CXTPReportColumn* pCol = pColumns->GetAt(nColumn);
		if (pCol && pCol->IsVisible())
		{
			CString sCaption = pCol->GetCaption();
			if (!sCaption.IsEmpty())
				menuArrange.AppendMenu(MF_STRING, ID_COLUMN_ARRANGEBY + nColumn, sCaption);
		}
	}

	menuArrange.AppendMenu(MF_SEPARATOR, 60, (LPCTSTR)NULL);

	menuArrange.AppendMenu(MF_STRING, ID_COLUMN_ARRANGEBY + nColumnCount,
		LoadResourceString(IDS_SHOWINGROUPS));
	menuArrange.CheckMenuItem(ID_COLUMN_ARRANGEBY + nColumnCount,
		MF_BYCOMMAND | ((m_wndReport.GetReportHeader()->IsShowItemsInGroups()) ? MF_CHECKED : MF_UNCHECKED)  );
	menu.InsertMenu(0, MF_BYPOSITION | MF_POPUP, (UINT_PTR) menuArrange.m_hMenu,
		LoadResourceString(IDS_ARRANGEBY));

	// create columns items
	CMenu menuColumns;
	VERIFY(menuColumns.CreatePopupMenu());
	for (nColumn = 0; nColumn < nColumnCount; nColumn++)
	{
		CXTPReportColumn* pCol = pColumns->GetAt(nColumn);
		CString sCaption = pCol->GetCaption();
		//if (!sCaption.IsEmpty())
		menuColumns.AppendMenu(MF_STRING, ID_COLUMN_SHOW + nColumn, sCaption);
		menuColumns.CheckMenuItem(ID_COLUMN_SHOW + nColumn,
				MF_BYCOMMAND | (pCol->IsVisible() ? MF_CHECKED : MF_UNCHECKED) );
	}

	menu.InsertMenu(0, MF_BYPOSITION | MF_POPUP, (UINT_PTR) menuColumns.m_hMenu,
		LoadResourceString(IDS_COLUMNS));

	//create Text alignment submenu
	CMenu menuAlign;
	VERIFY(menuAlign.CreatePopupMenu());

	menuAlign.AppendMenu(MF_STRING, ID_COLUMN_ALIGMENT_LEFT,
		LoadResourceString(IDS_ALIGNLEFT));
	menuAlign.AppendMenu(MF_STRING, ID_COLUMN_ALIGMENT_RIGHT,
		LoadResourceString(IDS_ALIGNRIGHT));
	menuAlign.AppendMenu(MF_STRING, ID_COLUMN_ALIGMENT_CENTER,
		LoadResourceString(IDS_ALIGNCENTER));

	int nAlignOption = 0;
	switch (pColumn->GetAlignment())
	{
	case DT_LEFT :
		nAlignOption = ID_COLUMN_ALIGMENT_LEFT;
		break;
	case DT_RIGHT :
		nAlignOption = ID_COLUMN_ALIGMENT_RIGHT;
		break;
	case DT_CENTER :
		nAlignOption = ID_COLUMN_ALIGMENT_CENTER;
		break;
	}

	menuAlign.CheckMenuItem(nAlignOption, MF_BYCOMMAND | MF_CHECKED);
	menu.InsertMenu(11, MF_BYPOSITION | MF_POPUP, (UINT_PTR) menuAlign.m_hMenu,
		LoadResourceString(IDS_ALIGNMENT));

	// track menu
#ifndef _XTP_INCLUDE_COMMANDBARS
	int nMenuResult = menu.TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN |TPM_RIGHTBUTTON, ptClick.x, ptClick.y, this, NULL);
#else
	int nMenuResult = CXTPCommandBars::TrackPopupMenu(&menu, TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTALIGN |TPM_RIGHTBUTTON, ptClick.x, ptClick.y, this, NULL);
#endif


	// arrange by items
	if (nMenuResult >= ID_COLUMN_ARRANGEBY && nMenuResult < ID_COLUMN_ALIGMENT)
	{
		for (int nColumn = 0; nColumn < nColumnCount; nColumn++)
		{
			CXTPReportColumn* pCol = pColumns->GetAt(nColumn);
			if (pCol && pCol->IsVisible())
			{
				if (nMenuResult == ID_COLUMN_ARRANGEBY + nColumn)
				{
					nMenuResult = ID_SORT_ASC;
					pColumn = pCol;
					break;
				}
			}
		}
		// group by item
		if (ID_COLUMN_ARRANGEBY + nColumnCount == nMenuResult)
		{
			m_wndReport.GetReportHeader()->ShowItemsInGroups(
				!m_wndReport.GetReportHeader()->IsShowItemsInGroups());
		}
	}

	// process Alignment options
	if (nMenuResult > ID_COLUMN_ALIGMENT && nMenuResult < ID_COLUMN_SHOW)
	{
		switch (nMenuResult)
		{
			case ID_COLUMN_ALIGMENT_LEFT :
				pColumn->SetAlignment(DT_LEFT);
				break;
			case ID_COLUMN_ALIGMENT_RIGHT :
				pColumn->SetAlignment(DT_RIGHT);
				break;
			case ID_COLUMN_ALIGMENT_CENTER  :
				pColumn->SetAlignment(DT_CENTER);
				break;
		}
	}

	// process column selection item
	if (nMenuResult >= ID_COLUMN_SHOW)
	{
		CXTPReportColumn* pCol = pColumns->GetAt(nMenuResult - ID_COLUMN_SHOW);
		if (pCol)
		{
			pCol->SetVisible(!pCol->IsVisible());
		}
	}

	// other general items
	switch (nMenuResult)
	{
		case ID_REMOVE_ITEM:
			pColumn->SetVisible(FALSE);
			m_wndReport.Populate();
			break;
		case ID_SORT_ASC:
		case ID_SORT_DESC:
			if (pColumn && pColumn->IsSortable())
			{
				pColumns->SetSortColumn(pColumn, nMenuResult == ID_SORT_ASC);
				m_wndReport.Populate();
			}
			break;

		case ID_GROUP_BYTHIS:
			if (pColumns->GetGroupsOrder()->IndexOf(pColumn) < 0)
			{
				pColumns->GetGroupsOrder()->Add(pColumn);
			}
			m_wndReport.ShowGroupBy(TRUE);
			m_wndReport.Populate();
			break;
		case ID_SHOW_GROUPBOX:
			m_wndReport.ShowGroupBy(!m_wndReport.IsGroupByVisible());
			break;
		case ID_SHOW_FIELDCHOOSER:
			OnShowFieldChooser();
			break;
		case ID_COLUMN_BESTFIT:
			m_wndReport.GetColumns()->GetReportHeader()->BestFit(pColumn);
			break;
	}

}

void CReportSampleView::OnReportBeginDrag(NMHDR * /*pNotifyStruct*/, LRESULT * /*result*/)
{
	int nCount = m_wndReport.GetSelectedRows()->GetCount();
	for (int i = nCount - 1; i >= 0; i--)
	{
		CXTPReportRow* pRow = m_wndReport.GetSelectedRows()->GetAt(i);
		if (pRow->IsGroupRow())
		{
			pRow->SetExpanded(TRUE);
			pRow->SelectChilds();
		}
	}

	COleDataSource ds;
	ds.DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_MOVE);
}


void CReportSampleView::OnReportItemRClick(NMHDR * pNotifyStruct, LRESULT * /*result*/)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow)
		return;

	if (pItemNotify->pRow->IsGroupRow())
	{
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU_CONTEXT_GROUP));

		// track menu
#ifndef _XTP_INCLUDE_COMMANDBARS
		int nMenuResult = menu.GetSubMenu(0)->TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN |TPM_RIGHTBUTTON, pItemNotify->pt.x, pItemNotify->pt.y, this, NULL);
#else
		int nMenuResult = CXTPCommandBars::TrackPopupMenu(menu.GetSubMenu(0), TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTALIGN |TPM_RIGHTBUTTON, pItemNotify->pt.x, pItemNotify->pt.y, this, NULL);
#endif

		// general items processing
		switch (nMenuResult)
		{
		case ID_POPUP_COLLAPSEALLGROUPS:
			pItemNotify->pRow->GetControl()->CollapseAll();
			break;
		case ID_POPUP_EXPANDALLGROUPS:
			pItemNotify->pRow->GetControl()->ExpandAll();
			break;
		}
	} else
	{
	}
}

void CReportSampleView::OnReportHyperlinkClick(NMHDR * pNotifyStruct, LRESULT * /*result*/)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;
	// if click on Hyperlink in Item
	if (pItemNotify->nHyperlink >= 0)
		TRACE(_T("Hyperlink Click : \n row %d \n col %d \n Hyperlink %d\n"),
				pItemNotify->pRow->GetIndex(), pItemNotify->pColumn->GetItemIndex(), pItemNotify->nHyperlink);
}


void CReportSampleView::OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * /*result*/)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	TRACE(_T("Click on row %d, col %d\n"),
			pItemNotify->pRow->GetIndex(), pItemNotify->pColumn->GetItemIndex());

	if (pItemNotify->pColumn->GetItemIndex() == COLUMN_CHECK)
	{
		m_wndReport.Populate();
	}
}

void CReportSampleView::OnReportKeyDown(NMHDR * pNotifyStruct, LRESULT * /*result*/)
{
	LPNMKEY lpNMKey = (LPNMKEY)pNotifyStruct;

	if (!m_wndReport.GetFocusedRow())
		return;

	if (lpNMKey->nVKey == VK_RETURN)
	{
		CMessageRecord* pRecord = DYNAMIC_DOWNCAST(CMessageRecord, m_wndReport.GetFocusedRow()->GetRecord());
		if (pRecord)
		{
			if (pRecord->SetRead())
			{
				m_wndReport.Populate();
			}
		}
	}
}

void CReportSampleView::OnReportItemDblClick(NMHDR * pNotifyStruct, LRESULT * /*result*/)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (pItemNotify->pRow)
	{
		TRACE(_T("Double Click on row %d\n"),
			pItemNotify->pRow->GetIndex());

		CMessageRecord* pRecord = DYNAMIC_DOWNCAST(CMessageRecord, pItemNotify->pRow->GetRecord());
		if (pRecord)
		{
			if (pRecord->SetRead())
			{
				m_wndReport.Populate();
			}
		}
	}
}

void CReportSampleView::OnShowFieldChooser()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrm)
	{
		BOOL bShow = !pMainFrm->m_wndFieldChooser.IsVisible();
		pMainFrm->ShowControlBar(&pMainFrm->m_wndFieldChooser, bShow, FALSE);
	}
}

void CReportSampleView::OnTreeViewDlg()
{
	CTreeViewDlg dlgTreeView;
	dlgTreeView.DoModal();
}


void CReportSampleView::OnReportcontrolAutomaticformattingsample()
{
	m_bAutomaticFormating = !m_bAutomaticFormating;
	m_wndReport.RedrawControl();

}

void CReportSampleView::OnUpdateReportcontrolAutomaticformattingsample(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bAutomaticFormating);

}


void CReportSampleView::AddSampleRecords()
{
	COleDateTime odtSent(COleDateTime::GetCurrentTime());
	COleDateTime odtCreated(COleDateTime::GetCurrentTime());
	COleDateTime odtRecieved(COleDateTime::GetCurrentTime());
	CString strMessage;
	CString strSubject;
	CString strEmpty(_T(" "));

	strMessage = _T("Simple Preview Text");
	strSubject = _T("Undeliverable Mail");
	m_wndReport.AddRecord(new CMessageRecord(msgImportanceNormal, FALSE, FALSE, _T("postmaster@mail.codejock.com"), strSubject, odtSent, 7, TRUE, 5,
		odtRecieved, odtCreated, strSubject, strEmpty, strMessage,
		strEmpty, strEmpty, strEmpty,
		strEmpty, strEmpty,
		strMessage));   odtSent -= 0.8; odtRecieved-= 0.8;

	strMessage = _T("Breaks words. Lines are automatically broken between words if a word would extend past the edge of the rectangle specified by the lpRect parameter. A carriage return-linefeed sequence also breaks the line.");
	strSubject = _T("Hi Mary Jane");
	m_wndReport.AddRecord(new CMessageRecord(msgImportanceNormal, FALSE, FALSE, _T("Peter Parker"), _T("RE: ") + strSubject, odtSent, 14, FALSE, 4.3,
		odtRecieved, odtCreated, strSubject, strEmpty, strMessage,
		strEmpty, strEmpty, strEmpty,
		strEmpty, strEmpty,
		strMessage)); odtSent -= 0.8;odtRecieved-= 0.8;

	strSubject = _T("");
	strMessage = _T("If you have several conditions to be tested together, and you know that one is more likely to pass or fail than the others, you can use a feature called 'short circuit evaluation' to speed the execution of your script. When JScript evaluates a logical expression, it only evaluates as many sub-expressions as required to get a result.");
	m_wndReport.AddRecord(new CMessageRecord(msgImportanceNormal, TRUE, FALSE, _T("James Howlett"), _T("RE:") + strSubject, odtSent, 24, FALSE, 56,
		odtRecieved, odtCreated, strSubject, strEmpty, strMessage,
		strEmpty, strEmpty, strEmpty,
		strEmpty, strEmpty,
		strMessage)); odtSent -= 0.8;odtRecieved-= 0.8;

	strMessage = _T("The MSDN Visual Studio 6.0 Library also provides information on the tools and technologies available with the Visual Studio suite.");
	strSubject = _T("error C2039: 'Serialize' : is not a member of 'CString'");
	m_wndReport.AddRecord(new CMessageRecord(msgImportanceLow, TRUE, TRUE, _T("Bruce Banner [bbanner@codetoolbox.com]"), _T("Re: ") + strSubject, odtSent, 14, TRUE, 12.2,
		odtRecieved, odtCreated, strSubject, strEmpty, strMessage,
		strEmpty, strEmpty, strEmpty,
		strEmpty, strEmpty,
		strMessage)); odtSent -= 0.8;odtRecieved-= 0.8;

	strMessage = _T("The Visual Studio Enterprise Edition includes all of the features available with the Visual Studio development tools. When installing the Visual Studio Enterprise Edition, you can elect to install enterprise features for use with these tools. In addition, Visual Basic and Visual C++ are available in separate enterprise editions.");
	strSubject = _T("Download GDI+ version 1.1");
	m_wndReport.AddRecord(new CMessageRecord(msgImportanceHigh, FALSE, FALSE, _T("QueryReply"), _T("Re: ") + strSubject, odtSent, 13, FALSE, 1.345,
		odtRecieved, odtCreated, strSubject, strEmpty, strMessage,
		strEmpty, strEmpty, strEmpty,
		strEmpty, strEmpty,
		strMessage)); odtSent -= 0.8;odtRecieved-= 0.8;

	strMessage = _T("The following table (email_example1@address.com) indicates whether a given feature is available with a given tool. If you (email_example2@address.com) acquired a separate enterprise edition of Visual Basic or Visual C++, you can use this table also to identify which of the Visual Studio enterprise features you received with your purchase. (Visual J++ and Visual InterDev are not offered as separate enterprise editions.) In addition, some of these features are available in professional editions with a subset of their enterprise edition functionality.");
	strSubject = _T("I don't understand:");
	m_wndReport.AddRecord(new CMessageRecord(msgImportanceLow, TRUE, FALSE, _T("James Howlett"), strSubject, odtSent, 24, FALSE, 5.4321,
		odtRecieved, odtCreated, strSubject, strEmpty, strMessage,
		strEmpty, strEmpty, strEmpty,
		strEmpty, strEmpty,
		strMessage)); odtSent -= 0.8;odtRecieved-= 0.8;

	strMessage = _T("Visual Studio contains many development tools for building your application.");
	strSubject = _T("Re: WIN32_FIND_DATA help ");
	m_wndReport.AddRecord(new CMessageRecord(msgImportanceNormal, FALSE, TRUE, _T("Matthew Murdock"), strSubject, odtSent, 14, TRUE, 45,
		odtRecieved, odtCreated, strSubject, strEmpty, strMessage,
		strEmpty, strEmpty, strEmpty,
		strEmpty, strEmpty,
		strMessage)); odtSent -= 8;odtRecieved-= 0.8;

	strMessage = _T("To learn more about how each development tool implements data access, see the following documentation topics.");
	strSubject = _T("Re: Licensing ");
	m_wndReport.AddRecord(new CMessageRecord(msgImportanceNormal, TRUE, FALSE, _T("Louis Lane"), strSubject, odtSent, 13, TRUE, 0.5,
		odtRecieved, odtCreated, strSubject, strEmpty, strMessage,
		strEmpty, strEmpty, strEmpty,
		strEmpty, strEmpty,
		strMessage)); odtSent -= 8;odtRecieved-= 0.8;

	strMessage = _T("Introduces the Data Access Guide, which discusses data access and provides links to information on related tools and technologies.");
	strSubject = _T("Undeliverable Mail");
	m_wndReport.AddRecord(new CMessageRecord(msgImportanceNormal, FALSE, FALSE, _T("postmaster@mail.codejock.com"), strSubject, odtSent, 7, TRUE, 10,
		odtRecieved, odtCreated, strSubject, strEmpty, strMessage,
		strEmpty, strEmpty, strEmpty,
		strEmpty, strEmpty,
		strMessage)); odtSent -= 80;odtRecieved-= 0.8;

	// new records
	CString strFrom;

	strFrom = _T("CodeToolBox");
	strSubject = _T("[CodeToolBox] Newsletter (10 May 2004)");
	strMessage = _T("An in-between week here at CodeToolBox. VSLive was last week, TechEd is in a couple of weeks; Last month's article winners have been awarded and this month's voting is just starting; Last week was Winter and this week is forecast to be Summer. It makes a developer want to just kick back and crank some code right? Right?");
	m_wndReport.AddRecord(new CMessageRecord(msgImportanceNormal, FALSE, FALSE, strFrom, strSubject, odtSent, 7, TRUE, 10,
		odtRecieved, odtCreated, strSubject, strEmpty, strMessage,
		strEmpty, strEmpty, strEmpty,
		strEmpty, strEmpty,
		strMessage));
	odtSent -= 7;

	strFrom = _T("Codejock Exchange");
	strSubject = _T("Good Answer! Capture message send from Net Send");
	strMessage = _T("Congratulations! Your answer has been accepted by SuperMan! You've earned 720 Expert Points and have been rewarded with a grade of A. These points will be added to your overall Expert Point total and to your Expert Point total in the Programming Topic Area.");
	m_wndReport.AddRecord(new CMessageRecord(msgImportanceNormal, FALSE, FALSE, strFrom, strSubject, odtSent, 8, FALSE, 720,
		odtRecieved, odtCreated, strSubject, strEmpty, strMessage,
		strEmpty, strEmpty, strEmpty,
		strEmpty, strEmpty,
		strMessage));
	odtSent -= 7; odtRecieved -= 14;

	strFrom = _T("CodeToolBox.com Team");
	strSubject = _T("CodeToolBox.com Sitewide update April 22nd, 2004");
	strMessage = _T("You have acquired this CodeToolBox.com update because you requested to be on the list. Honest! An explanation is at the bottom of this email, and also info on how to be extracted from the list.");
	m_wndReport.AddRecord(new CMessageRecord(msgImportanceNormal, FALSE, FALSE, strFrom, strSubject, odtSent, 46, FALSE, 28,
		odtRecieved, odtCreated, strSubject, strEmpty, strMessage,
		strEmpty, strEmpty, strEmpty,
		strEmpty, strEmpty,
		strMessage));
	odtSent -= 7;

	strFrom = _T("Codejock Exchange");
	strSubject = _T("Comment Added: returning a <ma> from a dll");
	strMessage = _T("A comment has been added to this question by: somebody. Click the following URL to read the new comment:");
	m_wndReport.AddRecord(new CMessageRecord(msgImportanceNormal, FALSE, FALSE, strFrom, strSubject, odtSent, 5, FALSE, 30,
		odtRecieved, odtCreated, strSubject, strEmpty, strMessage,
		strEmpty, strEmpty, strEmpty,
		strEmpty, strEmpty,
		strMessage));
	odtSent -= 7; odtRecieved -= 14;

	strFrom = _T("Bruce Wayne");
	strSubject = _T("Comment Added: memory leak in OnEndEdit() ?");
	strMessage = _T("A comment has been added to this question by: Robin. Click the following URL to read the new comment:");
	m_wndReport.AddRecord(new CMessageRecord(msgImportanceNormal, FALSE, FALSE, strFrom, strSubject, odtSent, 5, FALSE, 20,
		odtRecieved, odtCreated, strSubject, strEmpty, strMessage,
		strEmpty, strEmpty, strEmpty,
		strEmpty, strEmpty,
		strMessage));
	odtSent -= 7;

	strFrom = _T("Codejock Exchange");
	strSubject = _T("Good Answer!: stl containers");
	strMessage = _T("Congratulations! Your proposed answer has been accepted by TheBeast! You've earned 300 Expert points and have been rewarded with a grade of B.");
	m_wndReport.AddRecord(new CMessageRecord(msgImportanceNormal, FALSE, FALSE, strFrom, strSubject, odtSent, 4, FALSE, 2,
		odtRecieved, odtCreated, strSubject, strEmpty, strMessage,
		strEmpty, strEmpty, strEmpty,
		strEmpty, strEmpty,
		strMessage));
	odtSent -= 7; odtRecieved -= 14;

	strFrom = _T("Codejock Exchange");
	strSubject = _T("Codejock Exchange:  Lost Member Name or Password");
	strMessage = _T("You are receiving this email because you reported that you could not remember your Codejock Exchange Member Name or password.");
	m_wndReport.AddRecord(new CMessageRecord(msgImportanceNormal, FALSE, FALSE, strFrom, strSubject, odtSent, 7, FALSE, 1,
		odtRecieved, odtCreated, strSubject, strEmpty, strMessage,
		strEmpty, strEmpty, strEmpty,
		strEmpty, strEmpty,
		strMessage));
	odtSent -= 7;

	strFrom = _T("CodejockDirect");
	strSubject = _T("New volume/issues available on CodejockDirect");
	strMessage = _T("New volume/issues are now available for the following on CodejockDirect! * Mathematical and Computer Modelling");
	m_wndReport.AddRecord(new CMessageRecord(msgImportanceNormal, FALSE, FALSE, strFrom, strSubject, odtSent, 9, FALSE, 12,
		odtRecieved, odtCreated, strSubject, strEmpty, strMessage,
		strEmpty, strEmpty, strEmpty,
		strEmpty, strEmpty,
		strMessage));
	odtSent -= 7;

	strFrom = _T("Kyle Rayner");
	strSubject = _T("info");
	strMessage = _T("OR-Library is a collection of test data sets for a variety of problem areas. These test data sets can be accessed via emailing to CodeToolBox.com a message containing the name of the required file, or via ftp/WWW using the addresses given at the bottom of this file.");
	m_wndReport.AddRecord(new CMessageRecord(msgImportanceNormal, FALSE, FALSE, strFrom, strSubject, odtSent, 67, FALSE, 10,
		odtRecieved, odtCreated, strSubject, strEmpty, strMessage,
		strEmpty, strEmpty, strEmpty,
		strEmpty, strEmpty,
		strMessage));
	odtSent -= 7; odtRecieved -= 14;
}

void CReportSampleView::OnGridHorizontal(UINT nID)
{
	m_wndReport.SetGridStyle(FALSE, (XTPReportGridStyle)(nID - ID_GRID_HORIZONTAL_NOGRIDLINES));

}

void CReportSampleView::OnUpdateGridHorizontal(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(((int)pCmdUI->m_nID - ID_GRID_HORIZONTAL_NOGRIDLINES) == m_wndReport.GetGridStyle(FALSE));

}
void CReportSampleView::OnGridVertical(UINT nID)
{
	m_wndReport.SetGridStyle(TRUE, (XTPReportGridStyle)(nID - ID_GRID_VERTICAL_NOGRIDLINES));

}

void CReportSampleView::OnUpdateGridVertical(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(((int)pCmdUI->m_nID - ID_GRID_VERTICAL_NOGRIDLINES) == m_wndReport.GetGridStyle(TRUE));

}
void CReportSampleView::OnOptionsAllowcolumnresize()
{
	m_wndReport.GetReportHeader()->AllowColumnResize(!m_wndReport.GetReportHeader()->IsAllowColumnResize());
}

void CReportSampleView::OnUpdateOptionsAllowcolumnresize(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndReport.GetReportHeader()->IsAllowColumnResize());

}

void CReportSampleView::OnOptionsAllowcolumnsremove()
{
	m_wndReport.GetReportHeader()->AllowColumnRemove(!m_wndReport.GetReportHeader()->IsAllowColumnRemove());

}

void CReportSampleView::OnUpdateOptionsAllowcolumnsremove(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndReport.GetReportHeader()->IsAllowColumnRemove());

}

void CReportSampleView::OnOptionsMultipleselection()
{
	m_wndReport.SetMultipleSelection(!m_wndReport.IsMultipleSelection());

}

void CReportSampleView::OnUpdateOptionsMultipleselection(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndReport.IsMultipleSelection());

}

void CReportSampleView::OnOptionsShowitemsingroups()
{
	m_wndReport.GetReportHeader()->ShowItemsInGroups(!m_wndReport.GetReportHeader()->IsShowItemsInGroups());

}

void CReportSampleView::OnUpdateOptionsShowitemsingroups(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndReport.GetReportHeader()->IsShowItemsInGroups());
}

void CReportSampleView::OnOptionsAutomaticcolumnsizing()
{
	m_wndReport.GetReportHeader()->SetAutoColumnSizing(!m_wndReport.GetReportHeader()->IsAutoColumnSizing());

}

void CReportSampleView::OnUpdateOptionsAutomaticcolumnsizing(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndReport.GetReportHeader()->IsAutoColumnSizing());

}



void CReportSampleView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	m_wndReport.SetFocus();

}



void CReportSampleView::OnTestTasklist()
{
	if (m_pTaskFrame)
	{
		m_pTaskFrame->ActivateFrame();
		return;
	}
	CCreateContext contextT;
	// if no context specified, generate one from the
	// currently selected client if possible.
	contextT.m_pLastView       = NULL;
	contextT.m_pCurrentFrame   = NULL;
	contextT.m_pNewDocTemplate = NULL;
	contextT.m_pCurrentDoc     = NULL;
	contextT.m_pNewViewClass   = RUNTIME_CLASS(CTaskListView);


	m_pTaskFrame = new CTaskListFrame(this);

	DWORD dwStyle = WS_OVERLAPPEDWINDOW|FWS_ADDTOTITLE;

	m_pTaskFrame->LoadFrame(IDR_TASKLIST, dwStyle, 0, &contextT);
	m_pTaskFrame->InitialUpdateFrame(NULL, FALSE);

	m_pTaskFrame->ShowWindow(SW_SHOW);
}

void CReportSampleView::OnTestProperties()
{
	if (m_pPropertiesFrame)
	{
		m_pPropertiesFrame->ActivateFrame();
		return;
	}
	CCreateContext contextT;
	// if no context specified, generate one from the
	// currently selected client if possible.
	contextT.m_pLastView       = NULL;
	contextT.m_pCurrentFrame   = NULL;
	contextT.m_pNewDocTemplate = NULL;
	contextT.m_pCurrentDoc     = NULL;
	contextT.m_pNewViewClass   = RUNTIME_CLASS(CPropertiesView);


	m_pPropertiesFrame = new CPropertiesFrame(this);

	DWORD dwStyle = WS_OVERLAPPEDWINDOW|MFS_SYNCACTIVE;

	if (!m_pPropertiesFrame->Create(0, _T("Properties"), dwStyle, CRect(0, 0, 400, 350),
		this, 0, 0L, &contextT))
	{
		return ;   // will self destruct on failure normally
	}

	m_pPropertiesFrame->InitialUpdateFrame(NULL, FALSE);

	m_pPropertiesFrame->CenterWindow(this);
	m_pPropertiesFrame->ShowWindow(SW_SHOW);
}

void CReportSampleView::OnTestPerfomance()
{
	CPerfomanceTestDlg dlg;
	dlg.DoModal();
}


void CReportSampleView::OnReportcontrolMultilinesample()
{
	m_bMultilineSample = !m_bMultilineSample;
	if (m_bMultilineSample)
	{
		m_wndReport.SetPaintManager(new CReportMultilinePaintManager());
		m_wndReport.EnableToolTips(FALSE);
	}
	else
	{
		m_wndReport.SetPaintManager(new CXTPReportPaintManager());
		m_wndReport.EnableToolTips(TRUE);
	}

	m_wndReport.AdjustScrollBars();
}

void CReportSampleView::OnUpdateReportcontrolMultilinesample(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bMultilineSample);
}
