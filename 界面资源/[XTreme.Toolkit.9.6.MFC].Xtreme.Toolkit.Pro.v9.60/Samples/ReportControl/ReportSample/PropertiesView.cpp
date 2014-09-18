// PropertiesFrame.cpp : implementation file
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
#include "reportsample.h"
#include "PropertiesView.h"
#include "ReportSampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_PROPERTY_MULTIPLESELECTION 1
#define ID_PROPERTY_PREVIEWMODE 2
#define ID_PROPERTY_GROUPBOXVISIBLE 3
#define ID_PROPERTY_FOCUSSUBITEMS 4
#define ID_PROPERTY_ALLOWCOLUMNREMOVE 5
#define ID_PROPERTY_ALLOWCOLUMNREORDER 6
#define ID_PROPERTY_ALLOWCOLUMNRESIZE 7
#define ID_PROPERTY_FLATHEADER 8
#define ID_PROPERTY_HIDESELECTION 9
#define ID_PROPERTY_TREEINDENT 10

class CRecordPropertyGroup : public CXTPReportRecord
{
public:
	CRecordPropertyGroup(CString strCaption)
	{
		AddItem(new CXTPReportRecordItemText(strCaption));

		CXTPReportRecordItem* pItem = AddItem(new CXTPReportRecordItemText(_T("")));
		pItem->SetEditable(FALSE);

		AddItem(new CXTPReportRecordItemText(_T("")));
	}
};

class CRecordPropertyInt : public CXTPReportRecord
{
public:
	CRecordPropertyInt(UINT nID, CString strCaption, int nValue)
	{
		AddItem(new CXTPReportRecordItemText(strCaption));

		CXTPReportRecordItem* pItem = AddItem(new CXTPReportRecordItemNumber(nValue));
		pItem->SetItemData(nID);

		AddItem(new CXTPReportRecordItemText(_T("int")));
	}
	static int GetValue(XTP_NM_REPORTRECORDITEM* pItemNotify)
	{
		return (int)((CXTPReportRecordItemNumber*)pItemNotify->pItem)->GetValue();
	}
};


class CRecordPropertyBool : public CXTPReportRecord
{
protected:
	class CPropertyItemBool : public CXTPReportRecordItem
	{
	public:
		CPropertyItemBool(BOOL bValue)
		{
			m_bValue = bValue;
			GetEditOptions(NULL)->AddConstraint(_T("True"), TRUE);
			GetEditOptions(NULL)->AddConstraint(_T("False"), FALSE);
			GetEditOptions(NULL)->m_bConstraintEdit = TRUE;
			GetEditOptions(NULL)->AddComboButton();
		}

		CString GetCaption(CXTPReportColumn* /*pColumn*/)
		{
			CXTPReportRecordItemConstraint* pConstraint = GetEditOptions(NULL)->FindConstraint(m_bValue);
			ASSERT(pConstraint);
			return pConstraint->m_strConstraint;
		}

		void OnConstraintChanged(XTP_REPORTRECORDITEM_ARGS* /*pItemArgs*/, CXTPReportRecordItemConstraint* pConstraint)
		{
			m_bValue = (BOOL)pConstraint->m_dwData;
		}
		BOOL GetValue()
		{
			return m_bValue;
		}
	protected:
		BOOL m_bValue;

	};
public:
	CRecordPropertyBool(UINT nID, CString strCaption, BOOL bValue)
	{
		AddItem(new CXTPReportRecordItemText(strCaption));

		CXTPReportRecordItem* pItem = AddItem(new CPropertyItemBool(bValue));
		pItem->SetItemData(nID);

		AddItem(new CXTPReportRecordItemText(_T("bool")));
	}
	static BOOL GetValue(XTP_NM_REPORTRECORDITEM* pItemNotify)
	{
		return ((CPropertyItemBool*)pItemNotify->pItem)->GetValue();
	}
};


/////////////////////////////////////////////////////////////////////////////
// CPropertiesFrame

CPropertiesFrame::CPropertiesFrame(CView* pOwnerView)
{
	m_pOwnerView = pOwnerView;
}

CPropertiesFrame::~CPropertiesFrame()
{
}


BEGIN_MESSAGE_MAP(CPropertiesFrame, CMiniFrameWnd)
	//{{AFX_MSG_MAP(CPropertiesFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertiesFrame message handlers

int CPropertiesFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMiniFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!InitCommandBars())
		return -1;

	CXTPCommandBars* pCommandBars = GetCommandBars();

	CXTPToolBar* pToolBar = pCommandBars->Add(_T("Options"), xtpBarTop);
	pToolBar->LoadToolBar(IDR_MAINFRAME, FALSE);
	pToolBar->SetCloseable(FALSE);


	return 0;
}

void CPropertiesFrame::OnDestroy()
{
	((CReportSampleView*)m_pOwnerView)->m_pPropertiesFrame = NULL;

	CMiniFrameWnd::OnDestroy();
}


/////////////////////////////////////////////////////////////////////////////
// CPropertiesView

IMPLEMENT_DYNCREATE(CPropertiesView, CView)

CPropertiesView::CPropertiesView()
{
}

CPropertiesView::~CPropertiesView()
{
}

#define ID_REPORT_CONTROL 100

BEGIN_MESSAGE_MAP(CPropertiesView, CView)
	//{{AFX_MSG_MAP(CPropertiesView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, ID_REPORT_CONTROL, OnPropertyChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertiesView drawing

void CPropertiesView::OnDraw(CDC*)
{
}

/////////////////////////////////////////////////////////////////////////////
// CPropertiesView diagnostics

#ifdef _DEBUG
void CPropertiesView::AssertValid() const
{
	CView::AssertValid();
}

void CPropertiesView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPropertiesView message handlers


CXTPReportControl* CPropertiesView::GetTargetReport()
{
	CView* pView = ((CPropertiesFrame*)GetParent())->m_pOwnerView;

	return &((CReportSampleView*)pView)->GetReportCtrl();
}


int CPropertiesView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndReport.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|WM_VSCROLL, CRect(0, 0, 0, 0), this, ID_REPORT_CONTROL))
	{
		TRACE(_T("Failed to create view window\n"));
		return -1;
	}
	m_wndReport.GetReportHeader()->AllowColumnRemove(FALSE);

	CXTPReportColumn* pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Name"), 200));
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetEditable(FALSE);

	m_wndReport.AddColumn(new CXTPReportColumn(1, _T("Value"), 150));

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(2, _T("Type"), 100));
	pColumn->SetEditable(FALSE);

	CXTPReportRecord* pRecordControl = m_wndReport.AddRecord(new CRecordPropertyGroup(_T("Report Control")));

	pRecordControl->SetExpanded(TRUE);

	CXTPReportControl* pTargetReport = GetTargetReport();

	pRecordControl->GetChilds()->Add(
		new CRecordPropertyBool(ID_PROPERTY_MULTIPLESELECTION, _T("Multiple Selection"), pTargetReport->IsMultipleSelection()));

	pRecordControl->GetChilds()->Add(
		new CRecordPropertyBool(ID_PROPERTY_PREVIEWMODE, _T("Preview Mode"), pTargetReport->IsPreviewMode()));

	pRecordControl->GetChilds()->Add(
		new CRecordPropertyBool(ID_PROPERTY_GROUPBOXVISIBLE, _T("Group Box Visible"), pTargetReport->IsGroupByVisible()));

	pRecordControl->GetChilds()->Add(
		new CRecordPropertyBool(ID_PROPERTY_FOCUSSUBITEMS, _T("Focus Sub Items"), pTargetReport->IsFocusSubItems()));

	CXTPReportRecord* pRecordHeader = pRecordControl->GetChilds()->Add(
		new CRecordPropertyGroup(_T("Report Header")));


	pRecordHeader->GetChilds()->Add(
		new CRecordPropertyBool(ID_PROPERTY_ALLOWCOLUMNREMOVE, _T("Allow Column Remove"), pTargetReport->GetReportHeader()->IsAllowColumnRemove()));

	pRecordHeader->GetChilds()->Add(
		new CRecordPropertyBool(ID_PROPERTY_ALLOWCOLUMNREORDER, _T("Allow Column Reorder"), pTargetReport->GetReportHeader()->IsAllowColumnReorder()));

	pRecordHeader->GetChilds()->Add(
		new CRecordPropertyBool(ID_PROPERTY_ALLOWCOLUMNRESIZE, _T("Allow Column Resize"), pTargetReport->GetReportHeader()->IsAllowColumnResize()));

	CXTPReportRecord* pRecordPaintManager = pRecordControl->GetChilds()->Add(
		new CRecordPropertyGroup(_T("Report Paint Manager")));


	pRecordPaintManager->GetChilds()->Add(
		new CRecordPropertyBool(ID_PROPERTY_FLATHEADER, _T("Flat Header"), pTargetReport->GetPaintManager()->m_columnStyle == xtpColumnFlat));

	pRecordPaintManager->GetChilds()->Add(
		new CRecordPropertyBool(ID_PROPERTY_HIDESELECTION, _T("Hide Selection"), pTargetReport->GetPaintManager()->m_bHideSelection));

	pRecordPaintManager->GetChilds()->Add(
		new CRecordPropertyInt(ID_PROPERTY_TREEINDENT, _T("Tree Indent"), pTargetReport->GetPaintManager()->m_nTreeIndent));

	m_wndReport.GetPaintManager()->m_columnStyle = xtpColumnFlat;
	m_wndReport.AllowEdit(TRUE);
	m_wndReport.EditOnClick(FALSE);
	m_wndReport.SetMultipleSelection(FALSE);
	m_wndReport.SetTreeIndent(10);
	m_wndReport.GetReportHeader()->AllowColumnSort(FALSE);

	m_wndReport.Populate();



	return 0;
}

void CPropertiesView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_wndReport.GetSafeHwnd())
	{
		m_wndReport.MoveWindow(0, 0, cx, cy);
	}

}

BOOL CPropertiesView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CPropertiesView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	m_wndReport.SetFocus();

}

void CPropertiesView::OnPropertyChanged(NMHDR * pNotifyStruct, LRESULT * /*result*/)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	switch (pItemNotify->pItem->GetItemData())
	{
		case ID_PROPERTY_MULTIPLESELECTION:
			GetTargetReport()->SetMultipleSelection(CRecordPropertyBool::GetValue(pItemNotify));
			break;
		case ID_PROPERTY_PREVIEWMODE:
			GetTargetReport()->EnablePreviewMode(CRecordPropertyBool::GetValue(pItemNotify));
			GetTargetReport()->Populate();
			break;
		case ID_PROPERTY_GROUPBOXVISIBLE:
			GetTargetReport()->ShowGroupBy(CRecordPropertyBool::GetValue(pItemNotify));
			break;
		case ID_PROPERTY_FOCUSSUBITEMS:
			GetTargetReport()->FocusSubItems(CRecordPropertyBool::GetValue(pItemNotify));
			break;
		case ID_PROPERTY_ALLOWCOLUMNREMOVE:
			GetTargetReport()->GetReportHeader()->AllowColumnRemove(CRecordPropertyBool::GetValue(pItemNotify));
			break;
		case ID_PROPERTY_ALLOWCOLUMNREORDER:
			GetTargetReport()->GetReportHeader()->AllowColumnReorder(CRecordPropertyBool::GetValue(pItemNotify));
			break;
		case ID_PROPERTY_ALLOWCOLUMNRESIZE:
			GetTargetReport()->GetReportHeader()->AllowColumnResize(CRecordPropertyBool::GetValue(pItemNotify));
			break;
		case ID_PROPERTY_FLATHEADER:
			GetTargetReport()->GetPaintManager()->m_columnStyle = (CRecordPropertyBool::GetValue(pItemNotify))? xtpColumnFlat: xtpColumnShaded;
			break;
		case ID_PROPERTY_HIDESELECTION:
			GetTargetReport()->GetPaintManager()->m_bHideSelection = (CRecordPropertyBool::GetValue(pItemNotify));
			break;
		case ID_PROPERTY_TREEINDENT:
			GetTargetReport()->GetPaintManager()->m_nTreeIndent = CRecordPropertyInt::GetValue(pItemNotify);
			GetTargetReport()->RedrawControl();
			break;
	}

}
