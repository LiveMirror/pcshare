// GUI_OneNoteView.h : interface of the CGUI_OneNoteView class
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

#if !defined(AFX_GUI_ONENOTEVIEW_H__DC8615E5_0F07_4609_95E8_A6C9C78D4648__INCLUDED_)
#define AFX_GUI_ONENOTEVIEW_H__DC8615E5_0F07_4609_95E8_A6C9C78D4648__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGUI_OneNoteCntrItem;

class CGUI_OneNoteView : public CRichEditView
{
protected: // create from serialization only
	CGUI_OneNoteView();
	DECLARE_DYNCREATE(CGUI_OneNoteView)

// Attributes
public:
	CGUI_OneNoteDoc* GetDocument();

	COLORREF m_clr;
	COLORREF m_clrBack;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUI_OneNoteView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGUI_OneNoteView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;

#endif

	afx_msg void OnEmptyCommand(UINT);
	afx_msg void OnGetCharFormat(NMHDR* pNMHDR, LRESULT* pRes);
	afx_msg void OnSetCharFormat(NMHDR* pNMHDR, LRESULT* pRes);
	afx_msg void OnUpdateComboFont(CCmdUI* pCmd);
	afx_msg void OnUpdateComboSize(CCmdUI* pCmd);
	afx_msg void OnUpdateSetCheck(CCmdUI* pCmdUI);

	afx_msg void OnUpdateSelectorText(CCmdUI* pCmd);
	afx_msg void OnSelectorText(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateText(CCmdUI* pCmd);
	afx_msg void OnButtonText();

	afx_msg void OnUpdateSelectorBack(CCmdUI* pCmd);
	afx_msg void OnSelectorBack(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnUpdateBack(CCmdUI* pCmd);
	afx_msg void OnButtonBack();

	afx_msg void OnTextAuto();
	afx_msg void OnUpdateTextAuto(CCmdUI* pCmd);

	afx_msg void OnBackNone();
	afx_msg void OnUpdateBackNone(CCmdUI* pCmd);

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGUI_OneNoteView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GUI_OneNoteView.cpp
inline CGUI_OneNoteDoc* CGUI_OneNoteView::GetDocument()
	{ return (CGUI_OneNoteDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUI_ONENOTEVIEW_H__DC8615E5_0F07_4609_95E8_A6C9C78D4648__INCLUDED_)
