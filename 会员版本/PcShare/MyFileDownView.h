#if !defined(AFX_MYFILEDOWNVIEW_H__79A49660_8750_4EF0_A248_E27E887B2C0C__INCLUDED_)
#define AFX_MYFILEDOWNVIEW_H__79A49660_8750_4EF0_A248_E27E887B2C0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFileDownView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyFileDownView view

class CMyFileDownView : public CView
{
protected:
	CMyFileDownView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyFileDownView)

// Attributes
public:

	CXTPTabControl m_wndTabControl;
	CWnd* AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon);
	CWnd* InsertView(LPCTSTR pName);
	CWnd* GetCurView();
	CWnd* GetViewFromName(LPCTSTR sViewName);
	CString GetViewNameFromId(int nItem);
	CWnd* GetViewFromId(int nItem);
	int  GetViewCount();
	void CheckFileDownView(LPCTSTR pId);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFileDownView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyFileDownView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyFileDownView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg void OnSelectedChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnAddNewFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDlDeleteView(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFILEDOWNVIEW_H__79A49660_8750_4EF0_A248_E27E887B2C0C__INCLUDED_)
