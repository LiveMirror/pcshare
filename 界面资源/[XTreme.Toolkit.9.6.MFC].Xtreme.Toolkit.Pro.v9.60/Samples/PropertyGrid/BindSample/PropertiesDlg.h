// PropertiesDlg.h : header file
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

#if !defined(AFX_PROPERTIESDLG_H__81AAD8BD_716B_4F0D_B846_8141CA301C6F__INCLUDED_)
#define AFX_PROPERTIESDLG_H__81AAD8BD_716B_4F0D_B846_8141CA301C6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPropertiesDlg dialog
class CPropertyGridEx : public CXTPPropertyGrid
{
public:
	HTREEITEM m_hItem;
};

class CPropertiesDlg : public CDialog
{
	class CPropertyTree : public CTreeCtrl
	{
	public:

		CPropertyTree::CPropertyTree()
		{
			m_imgList.Create(IDB_OPTIONS, 16, 4, RGB(0, 0xFF, 0));

		}

		void PreSubclassWindow()
		{
			ModifyStyle(TVS_SHOWSELALWAYS, TVS_DISABLEDRAGDROP | TVS_NOTOOLTIPS |TVS_SINGLEEXPAND);
			SetImageList(&m_imgList, TVSIL_NORMAL);

			CTreeCtrl::PreSubclassWindow();
		}
		CImageList m_imgList;
	};
// Construction
public:
	CPropertiesDlg(CWnd* pParent = NULL);   // standard constructor

	CPropertyTree m_wndProperties;
	CRect m_rcWorkspace;

	CPropertyGridEx* m_pSelected;

protected:
	void CreateGridGeneral(HTREEITEM hItemCategory);
	CPropertyGridEx m_wndGridGeneral;

	CString m_strAssemblyName;
	CString m_strOutputType;
	CString m_strDefaultNamespace;
	CString m_strStartupObject;
	CString m_strApplicationIcon;
	CString m_strProjectFile;
	CString m_strProjectFolder;
	CString m_strOutputFile;

	void CreateGridDesigner(HTREEITEM hItemCategory);
	CPropertyGridEx m_wndGridDesigner;

	CString m_strPageLayout;
	CString m_strTargetSchema;
	CString m_strClentScriptLanguage;

	void CreateGridBuild(HTREEITEM hItemCategory);
	CPropertyGridEx m_wndGridBuild;

	CString m_strCompilationConstants;
	BOOL m_bOptimizeCode;
	BOOL m_bCheckForOverflow;
	BOOL m_bAlowUnsafe;
	CString m_strWarningLevel;
	BOOL m_bTreatWarnings;




// Dialog Data
	//{{AFX_DATA(CPropertiesDlg)
	enum { IDD = IDD_BINDSAMPLE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPropertiesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGetdispinfoListOptions(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedListOptions(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTIESDLG_H__81AAD8BD_716B_4F0D_B846_8141CA301C6F__INCLUDED_)
