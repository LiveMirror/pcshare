// HyperLinkDlg.h : header file
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

#if !defined(AFX_HYPERLINKDLG_H__9EB41119_66FF_4F2B_9686_7900E42EEFEC__INCLUDED_)
#define AFX_HYPERLINKDLG_H__9EB41119_66FF_4F2B_9686_7900E42EEFEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHyperLinkDlg dialog

class CHyperLinkDlg : public CXTResizeDialog
{
// Construction
public:
	CHyperLinkDlg(CWnd* pParent = NULL);    // standard constructor

// Dialog Data
	//{{AFX_DATA(CHyperLinkDlg)
	enum { IDD = IDD_HYPERLINK_DIALOG };
	CXTHyperLink    m_hyperLink;
	CXTColorPicker  m_cpLink;
	CXTColorPicker  m_cpHover;
	CXTColorPicker  m_cpVisited;
	BOOL    m_bUnderline;
	BOOL    m_bVisited;
	CString m_strURL;
	CString m_strEditTxt;
	BOOL    m_bShellExecute;
	//}}AFX_DATA
	COLORREF m_clrLink;
	COLORREF m_clrHover;
	COLORREF m_clrVisited;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHyperLinkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHyperLinkDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnChangeEditUrl();
	afx_msg void OnChkUnderline();
	afx_msg void OnChkVisited();
	afx_msg void OnDestroy();
	afx_msg void OnChangeEditTxt();
	afx_msg void OnHypText();
	afx_msg void OnChkShell();
	//}}AFX_MSG
	afx_msg void OnSelEndOkColor();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HYPERLINKDLG_H__9EB41119_66FF_4F2B_9686_7900E42EEFEC__INCLUDED_)
