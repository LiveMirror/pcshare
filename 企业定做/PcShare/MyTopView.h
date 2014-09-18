#if !defined(AFX_MYTOPVIEW_H__437CCA85_58DE_447A_BAC8_942EF999274B__INCLUDED_)
#define AFX_MYTOPVIEW_H__437CCA85_58DE_447A_BAC8_942EF999274B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyTopView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyTopView view

#include "MyClientManaView.h"

class CMyTopView : public CView
{
protected:
	CMyTopView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyTopView)

	CXTPTabControl m_wndTabControl;

// Attributes
public:

	
	CWnd* AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon);

	CMyClientManaView* GetViewFromName(LPCTSTR sViewName);
	CMyClientManaView* GetCurView();
	CMyClientManaView* GetViewFromId(int nItem);
	CMyClientManaView* InsertView(LPCTSTR pName, BOOL IsAddView);

	void DeleteItemFromId(LPCTSTR sId);
	MyServerTran* FindItemFromId(LPCTSTR sId);

	void ResizeView();
	int  GetViewCount();
	int  GetCurSel();
	void EditView(LPCTSTR pName);
	void SetCurSelView(int nItem);
	void GetPaneInfo(TCHAR* pText);
	void DeleteView();

	CString GetCurViewName();
	CString GetViewNameFromId(int nItem);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTopView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyTopView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyTopView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTabEditItem();
	afx_msg void OnUpdateTabEditItem(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTabInsertItem(CCmdUI* pCmdUI);
	afx_msg void OnTabInsertItem();
	afx_msg void OnTabDeleteItem();
	afx_msg void OnUpdateTabDeleteItem(CCmdUI* pCmdUI);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg void OnSelectedChanged(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTOPVIEW_H__437CCA85_58DE_447A_BAC8_942EF999274B__INCLUDED_)
