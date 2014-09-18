// TaskListView.cpp : implementation file
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
#include "TaskListView.h"
#include "ReportSampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CTaskListFrame


BEGIN_MESSAGE_MAP(CTaskListFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CTaskListFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

#define COLUMN_TYPE             0
#define COLUMN_IMPORTANCE       1
#define COLUMN_ATTACHMENT       2
#define COLUMN_STATUS           3
#define COLUMN_SUBJECT          4
#define COLUMN_DUE_DATE         5
#define COLUMN_COMPLETE         6
#define COLUMN_CATEGORIES       7


int CTaskListFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}


#ifdef _XTP_INCLUDE_COMMANDBARS

	if (!InitCommandBars())
		return -1;

	CXTPCommandBars* pCommandBars = GetCommandBars();

	pCommandBars->SetMenu(_T("Menu Bar"), IDR_TASKLIST);

#endif

	return 0;
}

void CTaskListFrame::OnDestroy()
{
	((CReportSampleView*)m_pOwnerView)->m_pTaskFrame = NULL;

	CFrameWnd::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////
// CTaskRecord

CTaskRecord::CTaskRecord(
		BOOL bSharedTask, TaskImportance taskImportance, BOOL bHasAttachment,
		TaskStatus taskStatus, CString strSubject,
		COleDateTime odtDueDate, int nComplete, CString strCategories)
{
	AddItem(new CTaskItemType(bSharedTask));
	AddItem(new CTaskItemImportance(taskImportance));
	AddItem(new CTaskItemAttachment(bHasAttachment));
	AddItem(new CTaskItemStatus(taskStatus));
	AddItem(new CXTPReportRecordItemText(strSubject));
	AddItem(new CTaskItemDueDate(odtDueDate));
	AddItem(new CTaskItemComplete(nComplete));
	AddItem(new CXTPReportRecordItemText(strCategories));
}

void CTaskRecord::GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics)
{
	if (((CTaskItemStatus*)GetItem(3))->m_taskStatus == taskStatusCompleted)
	{

		static CFont m_fontStriked;

		if (!m_fontStriked.GetSafeHandle())
		{
			LOGFONT lf;
			pDrawArgs->pControl->GetPaintManager()->m_fontText.GetLogFont(&lf);
			lf.lfStrikeOut = TRUE;

			m_fontStriked.CreateFontIndirect(&lf);
		}

		pItemMetrics->clrForeground = RGB(128, 128, 128);
		pItemMetrics->pFont = &m_fontStriked;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTaskListView

IMPLEMENT_DYNCREATE(CTaskListView, CXTPReportView)

CTaskListView::CTaskListView()
{
}

CTaskListView::~CTaskListView()
{
}


BEGIN_MESSAGE_MAP(CTaskListView, CXTPReportView)
	//{{AFX_MSG_MAP(CTaskListView)
	ON_WM_CREATE()
	ON_COMMAND(ID_REPORTCONTROL_ALLOWEDIT, OnReportcontrolAllowedit)
	ON_UPDATE_COMMAND_UI(ID_REPORTCONTROL_ALLOWEDIT, OnUpdateReportcontrolAllowedit)
	ON_COMMAND(ID_REPORTCONTROL_EDITONCLICK, OnReportcontrolEditonclick)
	ON_UPDATE_COMMAND_UI(ID_REPORTCONTROL_EDITONCLICK, OnUpdateReportcontrolEditonclick)
	ON_COMMAND(ID_REPORTCONTROL_FOCUSSUBITEMS, OnReportcontrolFocussubitems)
	ON_UPDATE_COMMAND_UI(ID_REPORTCONTROL_FOCUSSUBITEMS, OnUpdateReportcontrolFocussubitems)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskListView drawing

/////////////////////////////////////////////////////////////////////////////
// CTaskListView diagnostics

#ifdef _DEBUG
void CTaskListView::AssertValid() const
{
	CView::AssertValid();
}

void CTaskListView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTaskListView message handlers

int CTaskListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPReportView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndReport.GetImageManager()->SetIcons(IDB_BMREPORT, 0, 0, CSize(16, 16));

	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_TYPE, _T("Type"), 18, FALSE, 0));
	CXTPReportColumn* pColumnImportance = m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_IMPORTANCE, _T("Importants"), 18, FALSE, 1));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_ATTACHMENT, _T("Attachments"), 18, FALSE, 7));
	CXTPReportColumn* pColumnStatus = m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_STATUS, _T("Status"), 100));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_SUBJECT, _T("Subject"), 200));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_DUE_DATE, _T("Due Date"), 100));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_COMPLETE, _T("% Complete"), 100));
	m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_CATEGORIES, _T("Categories"), 80));

	pColumnStatus->GetEditOptions()->AddConstraint(_T("Not Started"), taskStatusNotStarted);
	pColumnStatus->GetEditOptions()->AddConstraint(_T("In Progress"), taskStatusInProgress);
	pColumnStatus->GetEditOptions()->AddConstraint(_T("Completed"), taskStatusCompleted);
	pColumnStatus->GetEditOptions()->AddConstraint(_T("Waiting on someone else"), taskStatusWaiting);
	pColumnStatus->GetEditOptions()->AddConstraint(_T("Deferred"), taskStatusDeferred);
	pColumnStatus->GetEditOptions()->m_bConstraintEdit = TRUE;
	pColumnStatus->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumnStatus->GetEditOptions()->AddComboButton();

	pColumnImportance->GetEditOptions()->AddConstraint(_T("Low"), taskImportanceLow);
	pColumnImportance->GetEditOptions()->AddConstraint(_T("Normal"), taskImportanceNormal);
	pColumnImportance->GetEditOptions()->AddConstraint(_T("High"), taskImportanceHigh);
	pColumnImportance->GetEditOptions()->m_bAllowEdit = FALSE;
	pColumnImportance->GetEditOptions()->AddComboButton();


	COleDateTime dtNone;
	COleDateTime dtNow = COleDateTime::GetCurrentTime();

	m_wndReport.AddRecord(new CTaskRecord(TRUE, taskImportanceNormal, TRUE, taskStatusInProgress, _T("Support Email: About tooltips"), dtNone, 70, _T("")));
	m_wndReport.AddRecord(new CTaskRecord(TRUE, taskImportanceNormal, TRUE, taskStatusNotStarted, _T("Support Email: Docking Pane ToggleDocking"), dtNone, 0, _T("")));
	m_wndReport.AddRecord(new CTaskRecord(TRUE, taskImportanceHigh, TRUE, taskStatusNotStarted, _T("Feature Request: New Event for CommandBars"), dtNow, 0, _T("")));
	m_wndReport.AddRecord(new CTaskRecord(TRUE, taskImportanceHigh, TRUE, taskStatusCompleted, _T("Support Email: Help Taskpanel And ShortcutBar"), dtNow, 100, _T("")));
	m_wndReport.AddRecord(new CTaskRecord(TRUE, taskImportanceNormal, FALSE, taskStatusCompleted, _T("Support Email: RE: Docking Pane Window Overlapping Issues"), dtNow, 100, _T("")));
	m_wndReport.AddRecord(new CTaskRecord(TRUE, taskImportanceNormal, FALSE, taskStatusCompleted, _T("Support Email: CXTPPropertyGridItem"), dtNone, 100, _T("")));
	m_wndReport.AddRecord(new CTaskRecord(TRUE, taskImportanceNormal, TRUE, taskStatusCompleted, _T("Support Email: Toolbar Oddity"), dtNone, 100, _T("")));
	m_wndReport.AddRecord(new CTaskRecord(TRUE, taskImportanceNormal, TRUE, taskStatusCompleted, _T("Support Email: CXTPTabControl"), dtNone, 100, _T("")));
	m_wndReport.AddRecord(new CTaskRecord(FALSE, taskImportanceNormal, TRUE, taskStatusInProgress, _T("Support Email: Menus go wrong if another form has topmost setting"), dtNone, 10, _T("")));
	m_wndReport.AddRecord(new CTaskRecord(FALSE, taskImportanceNormal, TRUE, taskStatusInProgress, _T("Support Email: Update Xtreme Suite from 8.6 to 8.7"), dtNone, 0, _T("")));
	m_wndReport.AddRecord(new CTaskRecord(TRUE, taskImportanceLow, TRUE, taskStatusInProgress, _T("Support Email: Bug in Tree-View on Label Edit"), dtNone, 0, _T("")));


	m_wndReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndReport.AllowEdit(TRUE);
	m_wndReport.FocusSubItems(TRUE);
	m_wndReport.Populate();
	m_wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpGridSolid);

	m_wndReport.SetFocus();

	return 0;
}


void CTaskListView::OnReportcontrolAllowedit()
{
	m_wndReport.AllowEdit(!m_wndReport.IsAllowEdit());

}

void CTaskListView::OnUpdateReportcontrolAllowedit(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndReport.IsAllowEdit()? TRUE: FALSE);

}

void CTaskListView::OnReportcontrolEditonclick()
{
	m_wndReport.EditOnClick(!m_wndReport.IsEditOnClick());

}

void CTaskListView::OnUpdateReportcontrolEditonclick(CCmdUI* pCmdUI)
{

	pCmdUI->SetCheck(m_wndReport.IsAllowEdit() && m_wndReport.IsEditOnClick()? TRUE: FALSE);

	if (!m_wndReport.IsAllowEdit())
		pCmdUI->Enable(FALSE);

}

void CTaskListView::OnReportcontrolFocussubitems()
{
	m_wndReport.FocusSubItems(!m_wndReport.IsFocusSubItems());

}

void CTaskListView::OnUpdateReportcontrolFocussubitems(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndReport.IsFocusSubItems()? TRUE: FALSE);
}
