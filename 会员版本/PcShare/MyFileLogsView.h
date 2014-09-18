#if !defined(AFX_MYFILELOGSVIEW_H__DE6BE493_0FBC_4795_B090_16801EEA63DE__INCLUDED_)
#define AFX_MYFILELOGSVIEW_H__DE6BE493_0FBC_4795_B090_16801EEA63DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFileLogsView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyFileLogsView view

class CMyFileLogsView : public CXTListView
{
protected:
	CMyFileLogsView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyFileLogsView)

// Attributes
public:

// Operations
public:

	CImageList m_imagelist;
	void InsertItem(LPCTSTR Text, LPCTSTR pTitle);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFileLogsView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyFileLogsView();
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

#endif // !defined(AFX_MYFILELOGSVIEW_H__DE6BE493_0FBC_4795_B090_16801EEA63DE__INCLUDED_)
