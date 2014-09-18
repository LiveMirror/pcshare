#if !defined(AFX_MYKEYMEDITVIEW_H__B056F894_6C86_4540_B0C0_BEBC63E65783__INCLUDED_)
#define AFX_MYKEYMEDITVIEW_H__B056F894_6C86_4540_B0C0_BEBC63E65783__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyKeyMEditView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyKeyMEditView view

class CMyKeyMEditView : public CEditView
{
protected:
	CMyKeyMEditView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyKeyMEditView)

// Attributes
public:

	TCHAR m_SaveFile[256];
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyKeyMEditView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyKeyMEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyKeyMEditView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChange();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnFileSave();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnEmpty();
	afx_msg void OnUpdateEmpty(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnDataArrived(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYKEYMEDITVIEW_H__B056F894_6C86_4540_B0C0_BEBC63E65783__INCLUDED_)
