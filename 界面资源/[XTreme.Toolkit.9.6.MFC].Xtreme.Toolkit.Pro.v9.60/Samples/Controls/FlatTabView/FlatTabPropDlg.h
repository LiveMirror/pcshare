// FlatTabPropDlg.h : header file
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

#if !defined(AFX_FLATTABPROPDLG_H__704EA15D_CD56_4018_B28F_E63CFFC02737__INCLUDED_)
#define AFX_FLATTABPROPDLG_H__704EA15D_CD56_4018_B28F_E63CFFC02737__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFlatTabPropDlg dialog

class CFlatTabViewView;
class CTabManipulationDlg;
class CFlatTabPropDlg : public CDialog
{
// Construction
public:
	CFlatTabPropDlg(CXTFlatTabCtrl* pFlatTabCtrl, CFlatTabViewView* pParent);   // standard constructor
	virtual ~CFlatTabPropDlg();

// Dialog Data
	//{{AFX_DATA(CFlatTabPropDlg)
	enum { IDD = IDD_FLATTAB_PROP };
	BOOL&   m_FTS_XT_BOTTOM;
	BOOL&   m_FTS_XT_HASARROWS;
	BOOL&   m_FTS_XT_HASHOMEEND;
	BOOL&   m_FTS_XT_HSCROLL;
	//}}AFX_DATA

	CFlatTabViewView*    m_pParentWnd;
	CXTFlatTabCtrl*      m_pFlatTabCtrl;
	CTabManipulationDlg* m_pManipDlg;
	int&                 m_dwTabStyle;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlatTabPropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFlatTabPropDlg)
	afx_msg void UpdateTabsStyle();
	afx_msg void OnManipulate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLATTABPROPDLG_H__704EA15D_CD56_4018_B28F_E63CFFC02737__INCLUDED_)
