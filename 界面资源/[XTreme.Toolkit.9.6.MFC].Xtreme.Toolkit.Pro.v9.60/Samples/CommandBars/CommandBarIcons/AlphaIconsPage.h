// AlphaIconsPage.h : header file
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

#if !defined(AFX_ALPHAICONSPAGE_H__)
#define AFX_ALPHAICONSPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PropertiesTabCtrl.h"


/////////////////////////////////////////////////////////////////////////////
// CAlphaIconsPage dialog

class CAlphaIconsPage : public CPropertiesTabPage
{
// Construction
public:
	CAlphaIconsPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlphaIconsPage)
	enum { IDD = IDD_PAGE_ALPHAICONS };
	BOOL    m_bDisabled;
	BOOL    m_bFaded;
	BOOL    m_bShadow;
	BOOL    m_bLargeImages;
	BOOL    m_bResample;
	//}}AFX_DATA


	void UpdateContent();
	void RedrawCommandBars();
	void ResampleImages();

	virtual void OnActivate();
	virtual void OnCancel();
	void OnCheckResample();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlphaIconsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlphaIconsPage)
	afx_msg void OnCheckFaded();
	afx_msg void OnCheckShadow();
	afx_msg void OnCheckDisabled();
	afx_msg void OnCheckLargeimages();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALPHAICONSPAGE_H__)
