// PaneAccelerators.h : header file
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

#if !defined(AFX_PANEACCELERATORS_H__B65C5A66_FA93_46C2_9BFA_36C6FBB17C14__INCLUDED_)
#define AFX_PANEACCELERATORS_H__B65C5A66_FA93_46C2_9BFA_36C6FBB17C14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPaneAccelerators window

class CPaneAccelerators : public CWnd
{
	class CAcceleratorsList : public CListCtrl
	{
		LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam);
	};

// Construction
public:
	CPaneAccelerators();

// Attributes
public:

// Operations
public:
	CAcceleratorsList m_wndListCtrl;

	CFrameWnd* m_pActiveFrame;
	LPACCEL m_lpAccel;
	int m_nAccelSize;

	CXTPToolBar m_wndToolBar;

	void Refresh(BOOL bForceAll = FALSE);
	void RefreshAccels(HACCEL& hAccel);

	void Insert();
	void Edit(int nIndex);
	void Delete(int nIndex);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaneAccelerators)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPaneAccelerators();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPaneAccelerators)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaneacelleratorsNew();
	afx_msg void OnUpdatePaneacelleratorsNew(CCmdUI* pCmdUI);
	afx_msg void OnPaneacelleratorsDelete();
	afx_msg void OnUpdatePaneacelleratorsDelete(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANEACCELERATORS_H__B65C5A66_FA93_46C2_9BFA_36C6FBB17C14__INCLUDED_)
