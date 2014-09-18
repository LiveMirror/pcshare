#if !defined(AFX_MYBOTTOMVIEW_H__28634AE9_EBA7_4AF4_A4FA_AA891CF131AD__INCLUDED_)
#define AFX_MYBOTTOMVIEW_H__28634AE9_EBA7_4AF4_A4FA_AA891CF131AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyBottomView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyFileManaView view

class CMyFileManaView : public CView
{
protected:
	CMyFileManaView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyFileManaView)

// Attributes
public:

	CXTPTabControl m_wndTabControl;
	CWnd* AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFileManaView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyFileManaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyFileManaView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void OnSelectedChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBOTTOMVIEW_H__28634AE9_EBA7_4AF4_A4FA_AA891CF131AD__INCLUDED_)
