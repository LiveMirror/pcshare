// IconsInfoPage.h : header file
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

#if !defined(AFX_ICONSINFOPAGE_H__91423EE2_15B0_4D1C_AF35_872F59E76F1D__INCLUDED_)
#define AFX_ICONSINFOPAGE_H__91423EE2_15B0_4D1C_AF35_872F59E76F1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PropertiesTabCtrl.h"


/////////////////////////////////////////////////////////////////////////////
// CIconsInfoPage dialog

class CIconsInfoPage : public CPropertiesTabPage
{
// Construction
public:
	CIconsInfoPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIconsInfoPage)
	enum { IDD = IDD_PAGE_ICONSINFO };
	BOOL    m_bDisabled;
	BOOL    m_bFaded;
	BOOL    m_bShadow;
	BOOL    m_bLargeImages;
	//}}AFX_DATA


	void UpdateContent();
	void RedrawCommandBars();

	virtual void OnActivate();
	virtual void OnCancel();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIconsInfoPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIconsInfoPage)
	afx_msg void OnCheckFaded();
	afx_msg void OnCheckShadow();
	afx_msg void OnCheckDisabled();
	afx_msg void OnCheckLargeimages();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONSINFOPAGE_H__91423EE2_15B0_4D1C_AF35_872F59E76F1D__INCLUDED_)
