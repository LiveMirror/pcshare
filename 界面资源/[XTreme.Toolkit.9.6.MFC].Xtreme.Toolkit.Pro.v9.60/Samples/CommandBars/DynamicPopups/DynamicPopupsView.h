// DynamicPopupsView.h : interface of the CDynamicPopupsView class
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

#if !defined(AFX_DYNAMICPOPUPSVIEW_H__3B5FF861_58E0_4E5D_9291_99C0843FB382__INCLUDED_)
#define AFX_DYNAMICPOPUPSVIEW_H__3B5FF861_58E0_4E5D_9291_99C0843FB382__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDynamicPopupsView : public CFormView
{
protected: // create from serialization only
	CDynamicPopupsView();
	DECLARE_DYNCREATE(CDynamicPopupsView)

public:
	//{{AFX_DATA(CDynamicPopupsView)
	enum{ IDD = IDD_DYNAMICPOPUPS_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CListBox    m_wndList;
	CString m_strText;

// Attributes
public:
	CDynamicPopupsDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynamicPopupsView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDynamicPopupsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDynamicPopupsView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();
	afx_msg void OnEmptyCommand();
	afx_msg void OnUndoClick(UINT nID);
	afx_msg void OnComboAdd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEmptyUpdateCommand(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DynamicPopupsView.cpp
inline CDynamicPopupsDoc* CDynamicPopupsView::GetDocument()
	{ return (CDynamicPopupsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNAMICPOPUPSVIEW_H__3B5FF861_58E0_4E5D_9291_99C0843FB382__INCLUDED_)
