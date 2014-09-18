// PageDragAndDropClient.h : header file
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

#if !defined(AFX_PAGEDRAGANDDROPCLIENT_H__FB67509E_7603_4797_939B_A82B573267B0__INCLUDED_)
#define AFX_PAGEDRAGANDDROPCLIENT_H__FB67509E_7603_4797_939B_A82B573267B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPageDragAndDropClient window

class CPageDragAndDropClient : public CStatic, COleDropTarget
{
// Construction
public:
	CPageDragAndDropClient();

// Attributes
public:
	CXTPImageManager m_imageManager;

	void PASCAL operator delete(void* p) {
		CStatic::operator delete(p);
	}
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
		DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
		DROPEFFECT dropEffect, CPoint point);

	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	int HitTest(CPoint point) const;

	BOOL PtInTrash(CPoint point);

	CArray<CXTPTaskPanelGroupItem*, CXTPTaskPanelGroupItem*> m_arrItems;
	CXTPTaskPanel* GetTaskPanel();

	BOOL m_bDragging;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageDragAndDropClient)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPageDragAndDropClient();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPageDragAndDropClient)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEDRAGANDDROPCLIENT_H__FB67509E_7603_4797_939B_A82B573267B0__INCLUDED_)
