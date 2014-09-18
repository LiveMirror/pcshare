// DialogBitmapImages.h : header file
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

#if !defined(AFX_DIALOGBITMAPIMAGES_H__95DB1801_C68B_4A6E_AE44_8FD7373BE65A__INCLUDED_)
#define AFX_DIALOGBITMAPIMAGES_H__95DB1801_C68B_4A6E_AE44_8FD7373BE65A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct CImage;
class CResourceManager;
class CDialogBitmapImagesHost;

class CComboBoxMask : public CComboBox
{
	virtual LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam);
};

struct CImage
{
public:
	XTPImageState imageState;
	CXTPImageManagerIcon* pIcon;
	UINT nID;
	int nIndex;
	CDialogBitmapImagesHost* pParent;

public:
	CXTPImageManagerIconHandle& GetHandle();

};

class CDialogBitmapImagesHost : public CDialog
{
public:
	CDialogBitmapImagesHost(UINT nIDTemplate, CResourceManager* pResourceManager);

public:
	virtual void OnImageChanged(CImage* /*pImage*/) {

	}

public:
	CResourceManager* m_pResourceManager;

	XTPImageState m_imageState;
	CArray<CImage, CImage&> m_arrImages;
	CImage* m_pSelected;
	CSize m_szIcons;
	COLORREF m_clrMask;



};




/////////////////////////////////////////////////////////////////////////////
// CDialogBitmapImages window

class CDialogBitmapImages : public CWnd
{
// Construction
public:
	CDialogBitmapImages();

// Attributes
public:

// Operations
public:
	CImage* HitTest(CPoint point);
	CPoint GetTopLeftPoint();

	void OnImageCountChanged();

	BOOL m_bScrollVisible[2];
	BOOL m_bAllowEdit;



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogBitmapImages)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDialogBitmapImages();
	int GetScrollOffset(int nBar);

	// Generated message map functions
protected:
	//{{AFX_MSG(CDialogBitmapImages)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGBITMAPIMAGES_H__95DB1801_C68B_4A6E_AE44_8FD7373BE65A__INCLUDED_)
