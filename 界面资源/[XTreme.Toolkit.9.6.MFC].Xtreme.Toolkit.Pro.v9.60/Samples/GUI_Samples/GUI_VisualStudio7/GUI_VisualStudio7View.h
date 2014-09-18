// GUI_VisualStudio7View.h : interface of the CGUI_VisualStudio7View class
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

#if !defined(AFX_GUI_VISUALSTUDIO7VIEW_H__6D08DC7C_2CBA_4CD9_9626_48D5990F8CB7__INCLUDED_)
#define AFX_GUI_VISUALSTUDIO7VIEW_H__6D08DC7C_2CBA_4CD9_9626_48D5990F8CB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGUI_VisualStudio7View : public CEditView
{
protected: // create from serialization only
	CGUI_VisualStudio7View();
	DECLARE_DYNCREATE(CGUI_VisualStudio7View)

// Attributes
public:
	CGUI_VisualStudio7Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUI_VisualStudio7View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL


// Implementation
public:
	virtual ~CGUI_VisualStudio7View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	void OnEditFind(NMHDR* pNMHDR, LRESULT* pResult);
	void OnUpdateEditFind(CCmdUI* pCmd);

// Generated message map functions
protected:
	//{{AFX_MSG(CGUI_VisualStudio7View)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GUI_VisualStudio7View.cpp
inline CGUI_VisualStudio7Doc* CGUI_VisualStudio7View::GetDocument()
	{ return (CGUI_VisualStudio7Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUI_VISUALSTUDIO7VIEW_H__6D08DC7C_2CBA_4CD9_9626_48D5990F8CB7__INCLUDED_)
