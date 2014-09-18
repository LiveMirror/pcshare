// ReportSampleView.h : interface of the CReportSampleView class
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

#if !defined(AFX_REPORTDEMOVIEW_H__EE95B23A_1835_4ABB_B336_2C161F0CB650__INCLUDED_)
#define AFX_REPORTDEMOVIEW_H__EE95B23A_1835_4ABB_B336_2C161F0CB650__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CReportSampleDoc;

class CReportSampleView : public CXTPReportView
{
protected: // create from serialization only
	CReportSampleView();
	DECLARE_DYNCREATE(CReportSampleView)

// Attributes
public:
	CReportSampleDoc* GetDocument();

	CXTPReportSubListControl m_wndSubList;
	CXTPReportFilterEditControl m_wndFilterEdit;

	CImageList m_ilIcons;

	BOOL m_bAutomaticFormating;
	BOOL m_bMultilineSample;

	void LoadReportState();
	void SaveReportState();

	CFrameWnd* m_pTaskFrame;
	CFrameWnd* m_pPropertiesFrame;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportSampleView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CReportSampleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void AddSampleRecords();

// Generated message map functions
protected:
	//{{AFX_MSG(CReportSampleView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEnablePreview();
	afx_msg void OnUpdateEnablePreview(CCmdUI* pCmdUI);
	afx_msg void OnAutoGrouping();
	afx_msg void OnUpdateAutoGrouping(CCmdUI* pCmdUI);
	afx_msg void OnGroupBy();
	afx_msg void OnUpdateGroupBy(CCmdUI* pCmdUI);
	afx_msg void OnGroupShade();
	afx_msg void OnUpdateGroupShade(CCmdUI* pCmdUI);
	afx_msg void OnTreeViewDlg();
	afx_msg void OnReportcontrolAutomaticformattingsample();
	afx_msg void OnUpdateReportcontrolAutomaticformattingsample(CCmdUI* pCmdUI);
	afx_msg void OnOptionsAllowcolumnresize();
	afx_msg void OnUpdateOptionsAllowcolumnresize(CCmdUI* pCmdUI);
	afx_msg void OnOptionsAllowcolumnsremove();
	afx_msg void OnUpdateOptionsAllowcolumnsremove(CCmdUI* pCmdUI);
	afx_msg void OnOptionsMultipleselection();
	afx_msg void OnUpdateOptionsMultipleselection(CCmdUI* pCmdUI);
	afx_msg void OnOptionsShowitemsingroups();
	afx_msg void OnUpdateOptionsShowitemsingroups(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDestroy();
	afx_msg void OnTestTasklist();
	afx_msg void OnTestPerfomance();
	afx_msg void OnTestProperties();
	afx_msg void OnOptionsAutomaticcolumnsizing();
	afx_msg void OnUpdateOptionsAutomaticcolumnsizing(CCmdUI* pCmdUI);
	afx_msg void OnReportcontrolMultilinesample();
	afx_msg void OnUpdateReportcontrolMultilinesample(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnGridHorizontal(UINT);
	afx_msg void OnUpdateGridHorizontal(CCmdUI* pCmdUI);

	afx_msg void OnGridVertical(UINT);
	afx_msg void OnUpdateGridVertical(CCmdUI* pCmdUI);
	afx_msg void OnReportHyperlinkClick(NMHDR * pNotifyStruct, LRESULT * /*result*/);

	afx_msg void OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg void OnReportItemRClick(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg void OnReportColumnRClick(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg void OnReportItemDblClick(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg void OnShowFieldChooser();
	afx_msg void OnReportKeyDown(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg void OnReportBeginDrag(NMHDR * pNotifyStruct, LRESULT * result);
	DECLARE_MESSAGE_MAP()
public:
};

#ifndef _DEBUG  // debug version in ReportSampleView.cpp
inline CReportSampleDoc* CReportSampleView::GetDocument()
	{ return (CReportSampleDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTDEMOVIEW_H__EE95B23A_1835_4ABB_B336_2C161F0CB650__INCLUDED_)
