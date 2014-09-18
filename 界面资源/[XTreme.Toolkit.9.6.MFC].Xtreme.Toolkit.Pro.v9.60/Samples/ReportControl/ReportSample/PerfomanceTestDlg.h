// PerfomanceTestDlg.h : header file
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

#if !defined(AFX_PERFOMANCETESTDLG_H__FA589373_5B94_4F22_9C64_D962A64C557B__INCLUDED_)
#define AFX_PERFOMANCETESTDLG_H__FA589373_5B94_4F22_9C64_D962A64C557B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPerfomanceTestDlg dialog

class CPerfomanceTestDlg : public CDialog
{
// Construction
public:
	CPerfomanceTestDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPerfomanceTestDlg)
	enum { IDD = IDD_DIALOG_PERFOMANCE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CXTPReportControl m_wndReport;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPerfomanceTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPerfomanceTestDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PERFOMANCETESTDLG_H__FA589373_5B94_4F22_9C64_D962A64C557B__INCLUDED_)
