#if !defined(AFX_MYCWNDLISTVIEW_H__545F4F16_FCB9_4EA5_A4AB_50DA93E6F3E4__INCLUDED_)
#define AFX_MYCWNDLISTVIEW_H__545F4F16_FCB9_4EA5_A4AB_50DA93E6F3E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyCwndListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyCwndListView view

class CMyCwndListView : public CXTListView
{
protected:
	CMyCwndListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyCwndListView)

// Attributes
public:

	void ReSizeWnd();
// Operations
public:

	void InsertMyItem(LPWINDOWITEM pItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCwndListView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	virtual bool SortList(int nCol, bool bAscending);

	CImageList m_imagelist;
// Implementation
protected:
	virtual ~CMyCwndListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyCwndListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCWNDLISTVIEW_H__545F4F16_FCB9_4EA5_A4AB_50DA93E6F3E4__INCLUDED_)
