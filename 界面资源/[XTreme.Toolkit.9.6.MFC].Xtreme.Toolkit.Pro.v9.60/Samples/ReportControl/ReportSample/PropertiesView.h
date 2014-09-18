// PropertiesView.h
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

#if !defined(AFX_PROPERTIESFRAME_H__19FCF03D_2F7A_4F06_B2FA_DD2F8BF25F24__INCLUDED_)
#define AFX_PROPERTIESFRAME_H__19FCF03D_2F7A_4F06_B2FA_DD2F8BF25F24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertiesFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertiesFrame frame

class CPropertiesFrame : public CXTPFrameWndBase<CMiniFrameWnd>
{
public:
	CPropertiesFrame(CView* pOwnerView);           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	CView* m_pOwnerView;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertiesFrame)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPropertiesFrame();

	// Generated message map functions
	//{{AFX_MSG(CPropertiesFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CPropertiesView view

class CPropertiesView : public CView
{
protected:
	CPropertiesView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPropertiesView)

// Attributes
public:
	CXTPReportControl m_wndReport;
// Operations
public:
	CXTPReportControl* GetTargetReport();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertiesView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPropertiesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropertiesView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	afx_msg void OnPropertyChanged(NMHDR * pNotifyStruct, LRESULT * /*result*/);

	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTIESFRAME_H__19FCF03D_2F7A_4F06_B2FA_DD2F8BF25F24__INCLUDED_)
