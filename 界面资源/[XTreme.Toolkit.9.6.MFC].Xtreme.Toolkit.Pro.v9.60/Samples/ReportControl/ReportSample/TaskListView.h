// TaskListView.h : header file
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

#if !defined(AFX_TASKLISTVIEW_H__5E52F174_9488_44B0_B8E7_48BC345422A2__INCLUDED_)
#define AFX_TASKLISTVIEW_H__5E52F174_9488_44B0_B8E7_48BC345422A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MainFrm.h"

enum TaskImportance
{
	taskImportanceNormal,
	taskImportanceHigh,
	taskImportanceLow
};

enum TaskStatus
{
	taskStatusNotStarted,
	taskStatusInProgress,
	taskStatusCompleted,
	taskStatusWaiting,
	taskStatusDeferred,
};



class CTaskRecord : public CXTPReportRecord
{
protected:

	class CTaskItemType : public CXTPReportRecordItem
{
	public:
		CTaskItemType(BOOL /*bSharedTask*/)
		{
			SetFocusable(FALSE);
		}
	};

	class CTaskItemImportance : public CXTPReportRecordItem
	{
	public:
		CTaskItemImportance(TaskImportance taskImportance)
		{
			SetImportance(taskImportance);
		}
		void OnConstraintChanged(XTP_REPORTRECORDITEM_ARGS* /*pItemArgs*/, CXTPReportRecordItemConstraint* pConstraint)
		{
			SetImportance((TaskImportance)pConstraint->m_dwData);
		}
		DWORD GetSelectedConstraintData(XTP_REPORTRECORDITEM_ARGS* /*pItemArgs*/)
		{
			return m_taskImportance;
		}

		void SetImportance(TaskImportance taskImportance )
		{
			m_taskImportance = taskImportance;

				SetIconIndex(taskImportance == taskImportanceHigh ? 6 :
			taskImportance == taskImportanceLow ? 9: -1);

			SetGroupPriority(
				taskImportance == taskImportanceHigh? IDS_GROUP_IMPOPRTANCE_HIGH:
			taskImportance == taskImportanceLow?    IDS_GROUP_IMPOPRTANCE_LOW: IDS_GROUP_IMPOPRTANCE_NORMAL);

			SetSortPriority(GetGroupPriority());
		}

	public:
		TaskImportance m_taskImportance;
	};

	class CTaskItemAttachment : public  CXTPReportRecordItem
	{
	public:
		CTaskItemAttachment::CTaskItemAttachment(BOOL bHasAttachment)
		{
			SetIconIndex(bHasAttachment ? 8 : -1);
			SetGroupPriority(bHasAttachment? IDS_GROUP_ATTACHMENTS_TRUE: IDS_GROUP_ATTACHMENTS_FALSE);
			SetSortPriority(GetGroupPriority());
			SetFocusable(FALSE);
		}
	};
	class CTaskItemDueDate : public CXTPReportRecordItemDateTime
	{
	public:
		CTaskItemDueDate(COleDateTime dtDateTime)
			: CXTPReportRecordItemDateTime(dtDateTime)
		{
			m_strFormatString = _T("%a %m/%d/%Y");
		}
		CString GetCaption(CXTPReportColumn* /*pColumn*/)
		{
			if ((DATE)m_odtValue == 0)
				return _T("None");

			return m_odtValue.Format(m_strFormatString);
		}
	};

	class CTaskItemStatus : public CXTPReportRecordItem
	{
	public:
		CTaskItemStatus(TaskStatus taskStatus)
		{
			m_taskStatus = taskStatus;
		}

		CString GetCaption(CXTPReportColumn* pColumn)
		{
			CXTPReportRecordItemConstraint* pConstraint = pColumn->GetEditOptions()->FindConstraint(m_taskStatus);
			ASSERT(pConstraint);
			return pConstraint->m_strConstraint;
		}

		void OnEditChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, LPCTSTR szText)
		{
			CXTPReportRecordItemConstraint* pConstraint = pItemArgs->pColumn->GetEditOptions()->FindConstraint(szText);
			if (pConstraint)
			{
				m_taskStatus = (TaskStatus)pConstraint->m_dwData;

				CXTPReportRecordItem* pItemComplete = ((CTaskRecord*)GetRecord())->GetItem(6);

				if (m_taskStatus == taskStatusCompleted &&
					((CTaskItemComplete*)pItemComplete)->GetValue() != 100)
				{
					((CTaskItemComplete*)pItemComplete)->SetValue(100);
				}
				if (m_taskStatus != taskStatusCompleted &&
					((CTaskItemComplete*)pItemComplete)->GetValue() == 100)
				{
					((CTaskItemComplete*)pItemComplete)->SetValue(0);
				}
			}
		}
		TaskStatus m_taskStatus;
	};

	class CTaskItemComplete : public CXTPReportRecordItemNumber
	{
	public:
		CTaskItemComplete(int nComplete)
			: CXTPReportRecordItemNumber(nComplete, _T("%2.0f%%"))
		{

		}
		void OnEditChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, LPCTSTR szText)
		{
			CXTPReportRecordItemNumber::OnEditChanged(pItemArgs, szText);
			if (m_dValue > 100)
				m_dValue = 100;

			int nStatus = (int)GetValue();

			CXTPReportRecordItem* pItemStatus = ((CTaskRecord*)GetRecord())->GetItem(3);

			if (nStatus == 100 &&
				((CTaskItemStatus*)pItemStatus)->m_taskStatus !=taskStatusCompleted)
			{
				((CTaskItemStatus*)pItemStatus)->m_taskStatus = taskStatusCompleted;
			}
			if (nStatus != 100 &&
				((CTaskItemStatus*)pItemStatus)->m_taskStatus ==taskStatusCompleted)
			{
				((CTaskItemStatus*)pItemStatus)->m_taskStatus = taskStatusInProgress;
			}
		}

	};


public:
	CTaskRecord(
		BOOL bSharedTask, TaskImportance taskImportance, BOOL bHasAttachment,
		TaskStatus taskStatus, CString strSubject,
		COleDateTime odtDueDate, int nComplete, CString strCategories);

	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);
};



class CTaskListFrame : public CXTPFrameWnd
{
public:
	CTaskListFrame(CView* pView)
	{
		m_pOwnerView = pView;

	}

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CView* m_pOwnerView;

// Generated message map functions
protected:
	//{{AFX_MSG(CTaskListFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CTaskListView view

class CTaskListView : public CXTPReportView
{
protected:
	CTaskListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTaskListView)

// Attributes

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskListView)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTaskListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CTaskListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnReportcontrolAllowedit();
	afx_msg void OnUpdateReportcontrolAllowedit(CCmdUI* pCmdUI);
	afx_msg void OnReportcontrolEditonclick();
	afx_msg void OnUpdateReportcontrolEditonclick(CCmdUI* pCmdUI);
	afx_msg void OnReportcontrolFocussubitems();
	afx_msg void OnUpdateReportcontrolFocussubitems(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKLISTVIEW_H__5E52F174_9488_44B0_B8E7_48BC345422A2__INCLUDED_)
