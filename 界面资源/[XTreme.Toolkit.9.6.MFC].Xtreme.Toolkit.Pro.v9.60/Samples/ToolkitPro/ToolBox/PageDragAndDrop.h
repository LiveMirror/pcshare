// PageDragAndDrop.h : header file
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

#if !defined(AFX_PAGEDRAGANDDROP_H__97381D7E_4FD5_48BB_AAA0_91D67C0D9FE5__INCLUDED_)
#define AFX_PAGEDRAGANDDROP_H__97381D7E_4FD5_48BB_AAA0_91D67C0D9FE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPageDragAndDrop dialog

#include "PageDragAndDropClient.h"

class CPageDragAndDrop : public CDialog
{
	DECLARE_DYNCREATE(CPageDragAndDrop)

// Construction
public:
	CPageDragAndDrop();
	~CPageDragAndDrop();

	virtual void OnCancel();
	virtual void OnOK();

// Dialog Data
	//{{AFX_DATA(CPageDragAndDrop)
	enum { IDD = IDD_PAGE_DRAGANDDROP };
	CPageDragAndDropClient  m_wndClient;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageDragAndDrop)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageDragAndDrop)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEDRAGANDDROP_H__97381D7E_4FD5_48BB_AAA0_91D67C0D9FE5__INCLUDED_)
