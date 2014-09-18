// DialogResourceSymbols.h : header file
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

#if !defined(AFX_DIALOGRESOURCESYMBOLS_H__7BC9099D_CC46_44C5_B6C9_640FC2D42E80__INCLUDED_)
#define AFX_DIALOGRESOURCESYMBOLS_H__7BC9099D_CC46_44C5_B6C9_640FC2D42E80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDialogResourceSymbols dialog

//   for owner/self draw controls (except for the optional DeleteItem)

class CDialogResourceSymbolsList : public CListBox
{
public:
	void DrawItem(LPDRAWITEMSTRUCT);
	void MeasureItem(LPMEASUREITEMSTRUCT)
	{

	}
	int CompareItem(LPCOMPAREITEMSTRUCT);

	int m_nCaptionWidth;
	int m_nUsedWidth;
};


class CDialogResourceSymbols : public CDialog
{
// Construction
public:
	CDialogResourceSymbols(CEmbeddedFrame* pFrame, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogResourceSymbols)
	enum { IDD = IDD_DIALOG_RESOURCE_SYMBOLS };
	CListBox    m_wndListUsage;
	CDialogResourceSymbolsList  m_wndList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogResourceSymbols)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CEmbeddedFrame* m_pFrame;

	void RefreshMainList();
	BOOL FillUsageList(UINT nID, BOOL bCheckOnly);

	// Generated message map functions
	//{{AFX_MSG(CDialogResourceSymbols)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonImport();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonDelete();
	afx_msg void OnSelchangeList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGRESOURCESYMBOLS_H__7BC9099D_CC46_44C5_B6C9_640FC2D42E80__INCLUDED_)
