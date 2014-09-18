#if !defined(AFX_MYREGLISTCTRL_H__6B713253_3FD2_40E9_8B35_2C3083A3E75D__INCLUDED_)
#define AFX_MYREGLISTCTRL_H__6B713253_3FD2_40E9_8B35_2C3083A3E75D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyRegListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyRegtListCtrl view

class CMyRegtListCtrl : public CListView
{
protected:
	CMyRegtListCtrl();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyRegtListCtrl)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyRegtListCtrl)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyRegtListCtrl();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	CImageList		m_ListImageList;
	//{{AFX_MSG(CMyRegtListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnEditLable(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYREGLISTCTRL_H__6B713253_3FD2_40E9_8B35_2C3083A3E75D__INCLUDED_)
