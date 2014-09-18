#if !defined(AFX_MYTLNTEDITVIEW_H__5C8351DA_3B66_41DC_820C_95D3C6E2D244__INCLUDED_)
#define AFX_MYTLNTEDITVIEW_H__5C8351DA_3B66_41DC_820C_95D3C6E2D244__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyTlntEditView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyTlntEditView view

class CMyTlntEditView : public CEditView
{
protected:
	CMyTlntEditView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyTlntEditView)

// Attributes
public:

	CString m_strSendBuf;
	HANDLE m_InputEvent;
	BOOL m_IsExit;
	BOOL WaitInput(CString& m_Str);
	void CloseInput();

	int nCount;
	
	BOOL bIsSafeChar(UINT nChar);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTlntEditView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyTlntEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyTlntEditView)
	afx_msg void OnChange();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnFileSave();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnDataArrived(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	// 增加两款字体 [9/16/2007 zhaiyinwei]
	CFont m_ChineseFont;
	CFont m_RussianFont;
	BOOL  m_bSetFont;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTLNTEDITVIEW_H__5C8351DA_3B66_41DC_820C_95D3C6E2D244__INCLUDED_)
