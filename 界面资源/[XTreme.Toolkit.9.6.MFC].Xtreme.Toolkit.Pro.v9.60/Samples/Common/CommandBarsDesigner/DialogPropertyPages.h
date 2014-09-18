// DialogPropertyPages.h : header file
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

#if !defined(AFX_DIALOGPROPERTYPAGES_H__C600AAE7_629B_4C72_B50B_A05FC030EAAE__INCLUDED_)
#define AFX_DIALOGPROPERTYPAGES_H__C600AAE7_629B_4C72_B50B_A05FC030EAAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDialogPropertyPages dialog

class CDialogPropertyPages : public CDialog
{
	class COptionsList : public CTreeCtrl
	{
	public:

		COptionsList::COptionsList()
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
	CDialogPropertyPages(CXTPCommandBars* pCommandBars, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogPropertyPages)
	enum { IDD = IDD_DIALOG_PROPERTYPAGES };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CXTPPropertyGrid m_wndGridGeneral;
	CXTPPropertyGrid m_wndGridPaint;

	void CreateGridGeneral();
	void CreateGridPaint();

	CXTPCommandBars* m_pCommandBars;

	BOOL m_AlwaysShowFullMenus;
	BOOL m_bLargeIcons;
	BOOL m_bShowFullAfterDelay;
	BOOL m_bToolBarAccelTips;
	BOOL m_bToolBarScreenTips;
	CSize m_szIcons;
	CSize m_szLargeIcons;
	CSize m_szPopupIcons;
	BOOL m_bShowExpandButtonAlways;
	BOOL m_bCustomizable;

	BOOL m_bIconsWithShadow;
	BOOL m_bUseDisabledIcons;
	BOOL m_bUseFadedIcons;

	CWnd* m_pSelected;

	XTPPaintTheme m_theme;

	BOOL m_bSyncFloatingBars;
	BOOL m_bDblClickFloat;
	BOOL m_bShowTextBelowIcons;
	BOOL m_bAltDragCustomization;
	BOOL m_bAutoHideUnusedPopups;
	XTPAnimationType m_nAnimationType;



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogPropertyPages)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	COptionsList m_wndList;
	CRect m_rcWorkspace;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogPropertyPages)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetdispinfoListOptions(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedListOptions(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg LRESULT OnGridNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGPROPERTYPAGES_H__C600AAE7_629B_4C72_B50B_A05FC030EAAE__INCLUDED_)
