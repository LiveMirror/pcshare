#if !defined(AFX_MYPROCESSLISTVIEW_H__89065C61_8E1B_4489_8401_235D1126C607__INCLUDED_)
#define AFX_MYPROCESSLISTVIEW_H__89065C61_8E1B_4489_8401_235D1126C607__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyProcessListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyProcListView view

class CMyProcListView : public CXTListView
{
protected:
	CMyProcListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyProcListView)

// Attributes
public:

	void ReSizeWnd();
	virtual bool SortList(int nCol, bool bAscending);
// Operations
public:
	void InsertMyItem(LPPROCESSLIST	pItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyProcListView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	CImageList m_imagelist;
	virtual ~CMyProcListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Generated message map functions
protected:
	//{{AFX_MSG(CMyProcListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPROCESSLISTVIEW_H__89065C61_8E1B_4489_8401_235D1126C607__INCLUDED_)
