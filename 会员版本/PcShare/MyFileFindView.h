#if !defined(AFX_MYFILEFINDVIEW_H__F9519774_5982_4BD5_8315_F513F6C02296__INCLUDED_)
#define AFX_MYFILEFINDVIEW_H__F9519774_5982_4BD5_8315_F513F6C02296__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFileFindView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyFileFindView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMyFileFindView : public CFormView
{
protected:
	CMyFileFindView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyFileFindView)

// Form Data
public:
	//{{AFX_DATA(CMyFileFindView)
	enum { IDD = IDD_DIALOG_FILE_FIND };
	CString	m_Dir;
	CString	m_File;
	//}}AFX_DATA

	void SetPath(LPCTSTR sPath);
	afx_msg void OnFileFind();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFileFindView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyFileFindView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMyFileFindView)
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFILEFINDVIEW_H__F9519774_5982_4BD5_8315_F513F6C02296__INCLUDED_)
