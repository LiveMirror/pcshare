// HeroesView.h : header file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// ©1998-2005 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEROESVIEW_H__6FD5D13E_6D06_4391_A898_5B2302A5B4A7__INCLUDED_)
#define AFX_HEROESVIEW_H__6FD5D13E_6D06_4391_A898_5B2302A5B4A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template<class BASE_CLASS>
class CNoFlickerWnd : public BASE_CLASS
{
public:
	CNoFlickerWnd()
	{
		m_bSkipEraseBkgnd = FALSE;
	}

	// This member function is called to set the background fill
	// color for the flicker free control.
	//
	inline void SetBackColor(
		// An RGB value.
		COLORREF crBack) {m_crBack = crBack;}

	// This member function is called to retrieve the background fill color
	// for the flicker free control. Returns an RGB value.
	//
	inline COLORREF GetBackColor() {return m_crBack;}

protected:

	// An RGB value.
	COLORREF m_crBack;
	BOOL m_bSkipEraseBkgnd;

	// This method provides a CE procedure (WindowProc) for a CWnd object.
	// It dispatches messages through the window message map. The return value
	// depends on the message.
	//
	virtual LRESULT WindowProc(
		// Specifies the Windows message to be processed.
		UINT message,
		// Provides additional information used in processing the message. The
		// parameter value depends on the message.
		WPARAM wParam,
		// Provides additional information used in processing the message. The
		// parameter value depends on the message.
		LPARAM lParam)
	{
		switch (message)
		{
		case WM_PAINT:
			{
				CPaintDC dc(this);

				// Get the client rect, and paint to a memory device context.  This
				// will help reduce screen flicker.  Pass the memory device context
				// to the default window procedure to do default painting.

				CRect r;
				GetClientRect(&r);
				CXTPBufferDC memDC(dc, r);
				memDC.FillSolidRect(r, GetSysColor(COLOR_WINDOW));

				return BASE_CLASS::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
			}

		case WM_WINDOWPOSCHANGING:
			m_bSkipEraseBkgnd = TRUE;
			break;

		case WM_WINDOWPOSCHANGED:
			m_bSkipEraseBkgnd = FALSE;
			break;

		case WM_ERASEBKGND:
			if (m_bSkipEraseBkgnd)
			{
				return TRUE;
			}
			break;
		}

		return BASE_CLASS::WindowProc(message, wParam, lParam);
	}
};


/////////////////////////////////////////////////////////////////////////////
// CHeroesView view

class CHeroesView : public CWnd
{
public:
	CHeroesView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHeroesView)

// Attributes
public:

	CNoFlickerWnd<CListCtrl>  m_listCtrl;
	CImageList m_imageList;

	CXTPToolBar m_wndToolBar;

	int m_nFilter;

// Operations
public:
	void InsertItem(int nItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeroesView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CHeroesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	afx_msg void OnFilterColor(UINT nID);
	afx_msg void OnUpdateFilterColor(CCmdUI* pCmd);

	// Generated message map functions
protected:
	//{{AFX_MSG(CHeroesView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEROESVIEW_H__6FD5D13E_6D06_4391_A898_5B2302A5B4A7__INCLUDED_)
