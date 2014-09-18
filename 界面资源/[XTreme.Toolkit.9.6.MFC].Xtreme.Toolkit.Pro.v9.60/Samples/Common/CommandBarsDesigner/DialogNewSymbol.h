// DialogNewSymbol.h : header file
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

#if !defined(AFX_DIALOGNEWSYMBOL_H__C1517853_612F_49CD_8653_D3A8D12CF835__INCLUDED_)
#define AFX_DIALOGNEWSYMBOL_H__C1517853_612F_49CD_8653_D3A8D12CF835__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CResourceManager;
/////////////////////////////////////////////////////////////////////////////
// CDialogNewSymbol dialog

class CDialogNewSymbol : public CDialog
{
// Construction
public:
	CDialogNewSymbol(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogNewSymbol)
	enum { IDD = IDD_DIALOG_RESOURCE_NEW };
	CString m_strName;
	UINT    m_nValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogNewSymbol)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CResourceManager* m_pResourceManager;

protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogNewSymbol)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGNEWSYMBOL_H__C1517853_612F_49CD_8653_D3A8D12CF835__INCLUDED_)
