// TreeCtrlDlg.h : header file
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

#if !defined(AFX_TreeCtrlDLG_H__D52D23D4_9130_11D3_9983_00500487D199__INCLUDED_)
#define AFX_TreeCtrlDLG_H__D52D23D4_9130_11D3_9983_00500487D199__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlDlg dialog

class CTreeCtrlDlg : public CXTResizeDialog
{
// Construction
public:
	CTreeCtrlDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data
	//{{AFX_DATA(CTreeCtrlDlg)
	enum { IDD = IDD_TREECTRL_DIALOG };
	CXTFlatComboBox m_SizeCombo;
	CXTColorPicker  m_colorPicker;
	CXTTreeCtrl     m_TreeCtrl;
	CXTFontCombo    m_FontCombo;
	int     m_nIndex;
	BOOL    m_bBoldCheck;
	CString m_strFontSize;
	//}}AFX_DATA


	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeCtrlDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON       m_hIcon;
	COLORREF    m_clrTreeText;
	CXTLogFont  m_LogFont;
	CImageList  m_ImageList;

	// Generated message map functions
	//{{AFX_MSG(CTreeCtrlDlg)
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBoldCheck();
	afx_msg void OnApplyButton();
	afx_msg void OnSelendokSizeCombo();
	afx_msg void OnSelendokFontCombo();
	afx_msg void OnSelEndOK();
	afx_msg void OnEditchangeSizeCombo();
	afx_msg void OnRclickTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEndlabeleditTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TreeCtrlDLG_H__D52D23D4_9130_11D3_9983_00500487D199__INCLUDED_)
