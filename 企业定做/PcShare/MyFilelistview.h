#if !defined(AFX_MYDIRLISTVIEW_H__A878D52C_2AD5_4932_8A1D_6948F8C863EC__INCLUDED_)
#define AFX_MYDIRLISTVIEW_H__A878D52C_2AD5_4932_8A1D_6948F8C863EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyDirListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyDirListView view

class CMyDirListView : public CXTListView
{
protected:
	CMyDirListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyDirListView)

// Attributes
public:

// Operations
public:
	void SetMyCoulmn(BOOL IsDisk);

	BOOL m_IsDiskList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDirListView)
	public:
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyDirListView();

	// Generated message map functions
protected:

	virtual bool SortList(int nCol, bool bAscending);
	CImageList m_imagelist;
	CList <LPMYICONINFO, LPMYICONINFO> m_List;
	void FindMyImage(LPCTSTR ItemName,int &nImage);

	//{{AFX_MSG(CMyDirListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnInsertMyItem(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDIRLISTVIEW_H__A878D52C_2AD5_4932_8A1D_6948F8C863EC__INCLUDED_)
