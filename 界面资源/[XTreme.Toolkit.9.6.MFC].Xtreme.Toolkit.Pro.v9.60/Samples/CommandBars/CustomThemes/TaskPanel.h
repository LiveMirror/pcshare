// TaskPanel.h : header file
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

#if !defined(AFX_TASKPANEL_H__007E5FF9_A62F_498D_B59D_F53A8C30BD37__INCLUDED_)
#define AFX_TASKPANEL_H__007E5FF9_A62F_498D_B59D_F53A8C30BD37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _XTP_INCLUDE_CONTROLS
	#define CTaskPanelBase CXTPTaskPanel
#else
	#define CTaskPanelBase CWnd
#endif

class CTaskPanel : public CTaskPanelBase
{
// Construction
public:
	CTaskPanel();

// Attributes
public:
	BOOL Create(CWnd* pParentWnd);

	void SetTheme(XTPPaintTheme theme);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskPanel)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTaskPanel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTaskPanel)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKPANEL_H__007E5FF9_A62F_498D_B59D_F53A8C30BD37__INCLUDED_)
