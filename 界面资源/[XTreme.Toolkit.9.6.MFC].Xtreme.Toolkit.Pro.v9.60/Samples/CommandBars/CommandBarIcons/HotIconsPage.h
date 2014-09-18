// HotIconsPage.h : header file
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

#if !defined(AFX_HOTICONSPAGE_H__5778BA0D_4B3D_4EA0_98ED_3AB524D60BBB__INCLUDED_)
#define AFX_HOTICONSPAGE_H__5778BA0D_4B3D_4EA0_98ED_3AB524D60BBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PropertiesTabCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CHotIconsPage dialog

class CHotIconsPage : public CPropertiesTabPage
{
// Construction
public:
	CHotIconsPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHotIconsPage)
	enum { IDD = IDD_PAGE_HOTICONS };
		// NOTE: the ClassWizard will add data members here
	BOOL    m_bDisabled;
	BOOL    m_bFaded;
	BOOL    m_bShadow;
	//}}AFX_DATA

	void UpdateContent();
	void RedrawCommandBars();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHotIconsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void OnActivate();
	virtual void OnCancel();

	// Generated message map functions
	//{{AFX_MSG(CHotIconsPage)
	afx_msg void OnCheckFaded();
	afx_msg void OnCheckShadow();
	afx_msg void OnCheckDisabled();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOTICONSPAGE_H__5778BA0D_4B3D_4EA0_98ED_3AB524D60BBB__INCLUDED_)
