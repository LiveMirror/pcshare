// GnomeFrame.h : header file
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

#if !defined(AFX_GNOMEFRAME_H__5651AED2_19C1_469E_B327_FF7D5B10DFFC__INCLUDED_)
#define AFX_GNOMEFRAME_H__5651AED2_19C1_469E_B327_FF7D5B10DFFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define GNOME_HTMINIMIZE 1
#define GNOME_HTMAXIMIZE 2
#define GNOME_HTCLOSE 3

/////////////////////////////////////////////////////////////////////////////
// CGnomeFrame frame

class CGnomeFrame : public CXTPFrameWnd
{
	DECLARE_DYNCREATE(CGnomeFrame)
protected:
	CGnomeFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	CRect m_rcBorders;
	CBitmap m_bmpBottomLeft;
	CBitmap m_bmpBottomRight;

	CBitmap m_bmpMenuNormal;
	CBitmap m_bmpCaption;

	CBitmap m_bmpCloseNormal;
	CBitmap m_bmpMaximizeNormal;
	CBitmap m_bmpMinimizeNormal;

	CBitmap m_bmpClosePushed;
	CBitmap m_bmpMaximizePushed;
	CBitmap m_bmpMinimizePushed;

	CFont m_fntTitle;

	int m_nButtonDown;

// Operations
public:
	int GetSysButtonsHitTest(CPoint point);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGnomeFrame)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGnomeFrame();

	// Generated message map functions
	//{{AFX_MSG(CGnomeFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GNOMEFRAME_H__5651AED2_19C1_469E_B327_FF7D5B10DFFC__INCLUDED_)
