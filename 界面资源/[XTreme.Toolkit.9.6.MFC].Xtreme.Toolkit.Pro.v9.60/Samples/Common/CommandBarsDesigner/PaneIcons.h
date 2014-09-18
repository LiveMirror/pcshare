// PaneIcons.h : header file
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

#if !defined(AFX_PANEICONS_H__4A0FC97D_89AF_4870_8769_C6CDEB879851__INCLUDED_)
#define AFX_PANEICONS_H__4A0FC97D_89AF_4870_8769_C6CDEB879851__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EmbeddedFrame.h"
/////////////////////////////////////////////////////////////////////////////
// CPaneIcons window

class CPaneIcons;

struct CControlIconsViewItem
{
	CControlIconsViewItem(int y = 0, int x = 0, XTPImageState _imageState = xtpImageNormal, CXTPImageManagerIcon* _pIcon = NULL)
	{
		ptPosition.x = x;
		ptPosition.y = y;
		imageState = _imageState;
		pIcon = _pIcon;
	}

	CXTPImageManagerIconHandle& GetIcon()
	{
		ASSERT(pIcon);
		return imageState == xtpImageNormal? pIcon->GetIcon(): imageState == xtpImageHot? pIcon->GetHotIcon():
			imageState == xtpImageChecked? pIcon->GetCheckedIcon(): pIcon->GetDisabledIcon();
	}

	void Draw(CDC* pDC, BOOL bSelected, int x, int y, CSize sz);

	POINT ptPosition;
	XTPImageState imageState;
	CXTPImageManagerIcon* pIcon;

};


class CControlIconsView : public CWnd
{
public:
	CControlIconsView();

// Attributes
	CControlIconsViewItem* m_pSelected;
	CList<CControlIconsViewItem, CControlIconsViewItem&> m_lstItems;
	CPaneIcons* m_pPaneIcons;

// Operations
public:
	CControlIconsViewItem* HitTest(CPoint pt);
	void DeleteAllItems()
	{
		m_lstItems.RemoveAll();
	}
	void RefreshIcons(CXTPImageManagerIconSet* pIconSet);
	void Edit();
	int GetScrollOffset(int nBar);
	void UpdateScrollBars();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlIconsView)
	public:
	LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CControlIconsView();
	CXTPImageManagerIconSet* m_pIconSet;
	BOOL m_bScrollVisible[2];

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CControlIconsView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



class CPaneIcons : public CWnd
{
// Construction
public:
	CPaneIcons();

// Attributes
public:
	CControlIconsView m_wndIconsView;

	CXTPToolBar m_wndToolBar;
	CComboBox m_wndCombo;

	CFont m_fntRegular;

	CEmbeddedFrame* m_pActiveFrame;
	CXTPImageManagerIconSet* m_pActiveIcons;
	CXTPControl* m_pLastControl;

	CImageList m_ilIcons;

// Operations
public:
	void RefreshIcons();
	void Refresh(BOOL bForce);

	void RemoveIcons(CXTPImageManagerIconSet* pIconSet, CXTPImageManagerIcon* pIconRemove);

	CXTPImageManager* GetImageManager();
	CString GetStringID(int nID);
	void SetModified();
	void RefreshAll();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaneIcons)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPaneIcons();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPaneIcons)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaneiconsClear();
	afx_msg void OnUpdatePaneiconsClear(CCmdUI* pCmdUI);
	afx_msg void OnPaneiconsNew();
	afx_msg void OnUpdatePaneiconsNew(CCmdUI* pCmdUI);
	afx_msg void OnPaneiconsEdit();
	afx_msg void OnUpdatePaneiconsEdit(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePaneiconsImportAndExport(CCmdUI* pCmdUI);
	afx_msg void OnImportandexportExporttoxml();
	afx_msg void OnImportandexportImportfromxml();
	afx_msg void OnImportandexportExporttobitmap();
	afx_msg void OnImportandexportImportfrombitmap();
	afx_msg void OnUpdateXMLCommand(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnComboChanged();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANEICONS_H__4A0FC97D_89AF_4870_8769_C6CDEB879851__INCLUDED_)
