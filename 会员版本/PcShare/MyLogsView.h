#if !defined(AFX_MYLOGSVIEW_H__41D5BA7B_2B8B_4791_A929_7818B5A752F8__INCLUDED_)
#define AFX_MYLOGSVIEW_H__41D5BA7B_2B8B_4791_A929_7818B5A752F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyLogsView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyLogsView view

class CMyLogsView : public CXTListView
{
protected:
	CMyLogsView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyLogsView)

// Attributes
public:

// Operations
public:

	CImageList m_imagelist;
	void InsertItem(LPCTSTR Text);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyLogsView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyLogsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyLogsView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLogsDeleteItem();
	afx_msg void OnUpdateLogsDeleteItem(CCmdUI* pCmdUI);
	afx_msg void OnLogsEmpty();
	afx_msg void OnUpdateLogsEmpty(CCmdUI* pCmdUI);
	afx_msg void OnLogsSave();
	afx_msg void OnUpdateLogsSave(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnResizeWnd(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLOGSVIEW_H__41D5BA7B_2B8B_4791_A929_7818B5A752F8__INCLUDED_)
