// XTPPreviewView.h : header file
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

#if !defined(_XTPPREVIEWVIEW_H__)
#define _XTPPREVIEWVIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXTPPreviewView view

class CXTPPreviewView : public CPreviewView
{
	DECLARE_DYNCREATE(CXTPPreviewView)

protected:
	CXTPPreviewView();

	CXTPToolBar  m_wndToolBar;
	CXTPControl* m_pBtnNumPage;

	//{{AFX_VIRTUAL(CXTPPreviewView)
	//}}AFX_VIRTUAL

	CXTPControlButton* FindToolbarButton(UINT nCommandID);

	//{{AFX_MSG(CXTPPreviewView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void UpdateNumPageIcon();
	afx_msg void OnUpdateNumPageChange(CCmdUI* pCmdUI) ;
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_XTPPREVIEWVIEW_H__)
