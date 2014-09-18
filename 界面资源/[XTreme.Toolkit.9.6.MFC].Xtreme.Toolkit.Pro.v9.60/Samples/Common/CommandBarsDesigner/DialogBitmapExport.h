// DialogBitmapExport.h : header file
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

#if !defined(AFX_DIALOGBITMAPEXPORT_H__C9DE3B76_7C91_44B7_8F0D_340B4EC3752B__INCLUDED_)
#define AFX_DIALOGBITMAPEXPORT_H__C9DE3B76_7C91_44B7_8F0D_340B4EC3752B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDialogBitmapExport;


#include "DialogBitmapImages.h"

class CResourceManager;
/////////////////////////////////////////////////////////////////////////////
// CDialogBitmapExport dialog

class CDialogBitmapExport : public CDialogBitmapImagesHost
{
	POSITION GetFirstIconPosition();
	CXTPImageManagerIcon* GetNextIcon(POSITION& pos);

// Construction
public:
	CDialogBitmapExport(CXTPImageManager* pImageManager,
		CResourceManager* pResourceManager);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogBitmapExport)
	enum { IDD = IDD_DIALOG_ICON_BITMAP };
	CComboBoxMask   m_comboMask;
	CComboBox   m_comboState;
	CComboBox   m_comboDevice;
	CComboBox   m_comboId;
	//}}AFX_DATA

	CDialogBitmapImages m_wndImages;
	CXTPImageManager* m_pImageManager;


public:
	void OnUpdateIcons();
	void OnImageChanged(CImage* pImage);
	BOOL CreateBitmap(BITMAPINFO* pBitmapInfo, BYTE*& pDest, CBitmap& bmp);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogBitmapExport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogBitmapExport)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboMask();
	afx_msg void OnDblclkComboMask();
	afx_msg void OnStateChanged();
	afx_msg void OnDeviceChanged();
	afx_msg void OnIdChanged();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGBITMAPEXPORT_H__C9DE3B76_7C91_44B7_8F0D_340B4EC3752B__INCLUDED_)
