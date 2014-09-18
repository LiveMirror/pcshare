// TabCtrlDlg.h : header file
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

#if !defined(AFX_TABCTRLDLG_H__F53ED4B3_D634_4811_B102_B29D5913AA04__INCLUDED_)
#define AFX_TABCTRLDLG_H__F53ED4B3_D634_4811_B102_B29D5913AA04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CEditListBoxEx helper class to handle sizing for CXTTabCtrl.

class CEditListBoxEx : public CXTEditListBox
{
public:
	virtual void Initialize(bool bAutoFont=true);

protected:
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlEx class

class CTabCtrlEx : public CXTTabCtrl
{
	DECLARE_DYNAMIC(CTabCtrlEx)

// Construction / destruction
public:

	// Constructs a CTabCtrlEx object.
	CTabCtrlEx();

	// Destroys a CTabCtrlEx object, handles cleanup and deallocation.
	virtual ~CTabCtrlEx();

// Member variables
protected:

// Member functions
public:

	//{{AFX_VIRTUAL(CTabCtrlEx)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTabCtrlEx)
	afx_msg void OnCloseTab();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlDlg dialog

class CTabCtrlDlg : public CXTResizeDialog
{
// Construction
public:
	CTabCtrlDlg(CWnd* pParent = NULL);  // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabCtrlDlg)
	enum { IDD = IDD_TABCTRL_DIALOG };
	CTabCtrlEx  m_tabCtrl;
	BOOL    m_bAutoCondense;
	BOOL    m_bVertical;
	BOOL    m_bMultiLine;
	BOOL    m_bXPMode;
	//}}AFX_DATA
	BOOL    m_bCloseButton;
	BOOL    m_bArrowButtons;


	CXTNoFlickerWnd <CButton> m_chkVertical;
	CXTNoFlickerWnd <CButton> m_chkMultiline;
	CXTNoFlickerWnd <CButton> m_chkAutoCondense;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabCtrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON           m_hIcon;
	CEditListBoxEx  m_listBox;
	CImageList      m_imageList;

	int InitTabCtrl();
	int InitEditList();
	int AddTabViews();
	void ToggleStyle(int style, BOOL bEnable);
	void ForceRefreshTabCtrl();

	// Generated message map functions
	//{{AFX_MSG(CTabCtrlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnAutocondense();
	afx_msg void OnVertical();
	afx_msg void OnMultiline();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnXpmode();
	//}}AFX_MSG
	afx_msg void OnLabelEditEnd();
	afx_msg void OnLabelEditCancel();
	afx_msg void OnNewItem();
	afx_msg void OnDeleteItem();
	afx_msg void OnMoveItemUp();
	afx_msg void OnMoveItemDown();
	afx_msg void OnChkButtons();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCTRLDLG_H__F53ED4B3_D634_4811_B102_B29D5913AA04__INCLUDED_)
