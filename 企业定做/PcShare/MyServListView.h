#if !defined(AFX_MYSERVICESVIEW_H__D13EF556_250E_4D85_9EF1_3C36E7155EDB__INCLUDED_)
#define AFX_MYSERVICESVIEW_H__D13EF556_250E_4D85_9EF1_3C36E7155EDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyServicesView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyServListView view

class CMyServListView : public CXTListView
{
protected:
	CMyServListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyServListView)

// Attributes
public:

	void ReStart();

// Operations
public:
	void UpDateItem(int nItem,LPMYSERVICES pServices);
	void InsertItem(LPMYSERVICES pServices);
	void DeleteAllItems();

	void ReSizeWnd();
	virtual bool SortList(int nCol, bool bAscending);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyServListView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyServListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CImageList m_imagelist;
	// Generated message map functions
protected:
	//{{AFX_MSG(CMyServListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSERVICESVIEW_H__D13EF556_250E_4D85_9EF1_3C36E7155EDB__INCLUDED_)
