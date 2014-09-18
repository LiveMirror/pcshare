// EclipseHost.h : header file
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

#if !defined(AFX_ECLIPSEHOST_H__284ED544_8A69_42BB_8C9A_23E5BE2E009E__INCLUDED_)
#define AFX_ECLIPSEHOST_H__284ED544_8A69_42BB_8C9A_23E5BE2E009E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CEclipseHost window

#ifndef _XTP_INCLUDE_CONTROLS

class CXTListCtrlEx : public CListCtrl
{
public:
	bool BuildColumns(int nCols, int* nWidth, CString* strColString)
	{
		for (int i = 0; i < nCols; ++i)
		{
			if ( InsertColumn(i, strColString[i], LVCFMT_LEFT, nWidth[i], i) == -1 )
			{
				return false;
			}
		}

		return true;
	}
};

#else
#define CXTListCtrlEx CXTListCtrl
#endif


class CEclipseHost : public CWnd
{
// Construction
public:
	CEclipseHost();

// Attributes
public:

// Operations
public:
	void SetHostParams(UINT nID, HICON hIcon);
	BOOL Create(CWnd* pParentWnd);

	virtual CWnd* GetClientWnd() { return NULL; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEclipseHost)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEclipseHost();
	CFont m_fntNormal;
	BOOL m_bActive;
	CString m_strTitle;
	UINT m_nID;
	HICON m_hIcon;

	CXTPToolBar m_wndToolBar;
	CXTPToolBar m_wndHostBar;

	int m_nHeaderHeight;
	CSize m_szCaption;


	// Generated message map functions
protected:
	//{{AFX_MSG(CEclipseHost)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaneClose();
	afx_msg void OnPaneHide();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECLIPSEHOST_H__284ED544_8A69_42BB_8C9A_23E5BE2E009E__INCLUDED_)
