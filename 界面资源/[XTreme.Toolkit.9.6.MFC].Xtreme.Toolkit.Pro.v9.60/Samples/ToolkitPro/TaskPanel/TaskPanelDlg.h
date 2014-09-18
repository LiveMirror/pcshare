// TaskPanelDlg.h : header file
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

#if !defined(AFX_TASKPANELDLG_H__3572B7BA_2CD7_4E44_B782_53CCF5EB89AB__INCLUDED_)
#define AFX_TASKPANELDLG_H__3572B7BA_2CD7_4E44_B782_53CCF5EB89AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTaskPanelDlg dialog

class CTaskPanelDlg : public CDialog
{
// Construction
public:
	CTaskPanelDlg(CWnd* pParent = NULL);   // standard constructor


	CBrush m_brushBack;
	COLORREF m_clrBack;
	void UpdateColors();

	inline void SetItem(CXTPTaskPanelGroupItem* pItem) {m_pItem = pItem;}
	CXTPTaskPanelGroupItem* m_pItem;

	virtual void OnCancel() {
		//////////////////////////////////////////////////////////////////////////


	}
	virtual void OnOK() {

	}

// Dialog Data
	//{{AFX_DATA(CTaskPanelDlg)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskPanelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaskPanelDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKPANELDLG_H__3572B7BA_2CD7_4E44_B782_53CCF5EB89AB__INCLUDED_)
