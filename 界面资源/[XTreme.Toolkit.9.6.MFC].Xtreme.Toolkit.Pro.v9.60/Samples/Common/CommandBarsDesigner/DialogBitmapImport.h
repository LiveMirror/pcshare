// DialogBitmapImport.h : header file
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

#if !defined(AFX_DIALOGBITMAPIMPORT_H__16D0A13D_4EE9_4F7C_85B1_F57BE6CB3F4C__INCLUDED_)
#define AFX_DIALOGBITMAPIMPORT_H__16D0A13D_4EE9_4F7C_85B1_F57BE6CB3F4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CResourceManager;

#include "DialogBitmapImages.h"
/////////////////////////////////////////////////////////////////////////////
// CDialogBitmapImport dialog

class CDialogBitmapImport : public CDialogBitmapImagesHost
{
// Construction
public:
	CDialogBitmapImport(CXTPImageManager* pImageManager, LPCTSTR lpszFileName, CResourceManager* pResourceManager);   // standard constructor
	~CDialogBitmapImport();

// Dialog Data
	//{{AFX_DATA(CDialogBitmapImport)
	enum { IDD = IDD_DIALOG_ICON_BITMAP };
	CComboBoxMask   m_comboMask;
	CComboBox   m_comboState;
	CComboBox   m_comboDevice;
	CComboBox   m_comboId;
	//}}AFX_DATA

	CString m_strFileName;
	CXTPImageManager* m_pImageManager;
	CXTPImageManager* m_pParentImageManager;
	BOOL m_bAlpha;

	CDialogBitmapImages m_wndImages;

	void OnUpdateIcons();
	void OnImageChanged(CImage* pImage);

	void ReloadBitmap();
	CSize GetBitmapSize();

	CSize m_szBitmap;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogBitmapImport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogBitmapImport)
	virtual BOOL OnInitDialog();
	afx_msg void OnIdChanged();
	afx_msg void OnDeviceChanged();
	afx_msg void OnMaskChanged();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGBITMAPIMPORT_H__16D0A13D_4EE9_4F7C_85B1_F57BE6CB3F4C__INCLUDED_)
