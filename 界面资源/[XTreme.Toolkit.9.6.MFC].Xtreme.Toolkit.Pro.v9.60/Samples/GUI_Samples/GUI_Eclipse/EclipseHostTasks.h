// EclipseHostTasks.h : header file
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

#if !defined(AFX_ECLIPSEHOSTTASKS_H__970BA2F3_3122_436D_B677_471ECD127A49__INCLUDED_)
#define AFX_ECLIPSEHOSTTASKS_H__970BA2F3_3122_436D_B677_471ECD127A49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EclipseHost.h"
/////////////////////////////////////////////////////////////////////////////
// CEclipseHostTasks window

class CEclipseHostTasks : public CEclipseHost
{
// Construction
public:
	CEclipseHostTasks();

// Attributes
public:
	CXTListCtrlEx m_listCtrl;
	CImageList  m_ilTabImages;

	void InitListControl();

	virtual CWnd* GetClientWnd() { return &m_listCtrl; }
	void OnEmptyCommand();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEclipseHostTasks)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEclipseHostTasks();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEclipseHostTasks)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	void OnUpdateTasksDelete(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECLIPSEHOSTTASKS_H__970BA2F3_3122_436D_B677_471ECD127A49__INCLUDED_)
