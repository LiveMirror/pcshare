// PageLookAndBehaviour.h : header file
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

#if !defined(AFX_PAGELOOKANDBEHAVIOUR_H__96C57C27_89FE_4794_BECC_84B865AE2B91__INCLUDED_)
#define AFX_PAGELOOKANDBEHAVIOUR_H__96C57C27_89FE_4794_BECC_84B865AE2B91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPageLookAndBehaviour dialog
class CXTPTaskPanel;

class CPageLookAndBehaviour : public CDialog
{
// Construction
public:
	CPageLookAndBehaviour();
	~CPageLookAndBehaviour();

// Dialog Data
	//{{AFX_DATA(CPageLookAndBehaviour)
	enum { IDD = IDD_PAGE_LOOKANDBEHAVIOUR };
	CComboBox   m_comboTheme;
	CComboBox   m_comboLayout;
	CComboBox   m_comboHotTrack;
	CComboBox   m_comboBehaviour;
	BOOL    m_bLargeIcons;
	BOOL    m_bDrawFocusRect;
	BOOL    m_bSelectOnFocus;
	BOOL    m_bToggleButtons;
	int     m_nControlMargins;
	int     m_nGroupInnerMargins;
	int     m_nGroupOuterMargins;
	int     m_nItemsMargins;
	BOOL    m_bDefaultMargins;
	BOOL    m_bAllowDrag;
	//}}AFX_DATA

	virtual void OnCancel();
	virtual void OnOK();

	CXTPTaskPanel* GetTaskPanel();
	void OnThemeChanged();

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageLookAndBehaviour)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageLookAndBehaviour)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboBehaviour();
	afx_msg void OnSelchangeComboTheme();
	afx_msg void OnSelchangeComboLayout();
	afx_msg void OnSelchangeComboHottrack();
	afx_msg void OnCheckLargeicons();
	afx_msg void OnCheckDrawfocusrect();
	afx_msg void OnCheckSelectonfocus();
	afx_msg void OnCheckTogglebuttons();
	afx_msg void OnReleasedcaptureSliderControl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderGroupinner(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderGroupouter(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderItems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckDefaultmargins();
	afx_msg void OnCheckAllowDrag();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGELOOKANDBEHAVIOUR_H__96C57C27_89FE_4794_BECC_84B865AE2B91__INCLUDED_)
