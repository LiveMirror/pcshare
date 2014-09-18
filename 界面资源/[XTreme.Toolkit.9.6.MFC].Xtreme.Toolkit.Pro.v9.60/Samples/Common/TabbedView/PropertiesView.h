// PropertiesView.h : header file
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

#if !defined(AFX_PROPERTIESVIEW_H__7D1C72CB_5118_4583_AB59_CFFB96189CE1__INCLUDED_)
#define AFX_PROPERTIESVIEW_H__7D1C72CB_5118_4583_AB59_CFFB96189CE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPropertiesView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CPropertiesView : public CFormView
{
protected:
	CPropertiesView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPropertiesView)

// Form Data
public:
	//{{AFX_DATA(CPropertiesView)
	enum { IDD = IDD_PROPERTIES_VIEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CComboBox   m_comboAppearance;
	CComboBox   m_comboPosition;
	CComboBox   m_comboColor;
	int     m_nAppearance;
	int     m_nColor;
	int     m_nLayout;
	int     m_nPosition;
	BOOL m_bLunaColors;
	BOOL m_bOneNoteColors;

	void Update();


// Attributes
public:


// Operations
public:
	virtual CXTPTabPaintManager* GetPaintManager();
	void UpdateControls();
	void Reposition();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertiesView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPropertiesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	afx_msg void OnSelchangeComboColor();
	afx_msg void OnSelchangeComboPosition();
	afx_msg void OnSelchangeComboAppearance();
	afx_msg void OnCheckLunaColors();
	afx_msg void OnCheckOnenoteColors();
	afx_msg void OnSelchangeComboLayout();

	// Generated message map functions
	//{{AFX_MSG(CPropertiesView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTIESVIEW_H__7D1C72CB_5118_4583_AB59_CFFB96189CE1__INCLUDED_)
