// XTWindowList.h : header file
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(_XTWINDOWLIST_H__)
#define _XTWINDOWLIST_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTWindowList dialog
//===========================================================================
class _XT_EXT_CLASS CXTWindowList : public CXTResizeDialog
{
	DECLARE_DYNAMIC(CXTWindowList)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTWindowList object.
	// Parameters:
	//     pMDIFrameWnd - Pointer to applications main window.
	//-----------------------------------------------------------------------
	CXTWindowList(CMDIFrameWnd* pMDIFrameWnd);


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the dialogs icon.
	// Parameters:
	//     hIcon - Icon handle to set.
	//-----------------------------------------------------------------------
	void SetWindowIcon(HICON hIcon) {
		m_hIcon = hIcon;
	}

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add a MDI child to the window list.
	// Parameters:
	//     pMDIChildWnd - Pointer to a MDI child window.
	//-----------------------------------------------------------------------
	virtual void InsertMDIChild(CMDIChildWnd* pMDIChildWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the MDI child windows.
	// Parameters:
	//     hWnd - Handle to a MDI child window.
	// Returns:
	//     A CString object representing the windows text.
	//-----------------------------------------------------------------------
	virtual CString GetChildWndText(HWND hWnd) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to toggle restore / minimize state for selected MDI children.
	//-----------------------------------------------------------------------
	void RestoreAndMinimize();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable / disable button controls for the window list dialog
	//     depending on selection.
	//-----------------------------------------------------------------------
	void SetEnableButtons();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to populate the window list with currently open MDI children.
	//-----------------------------------------------------------------------
	virtual void FillMDIWindowList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to select the active MDI child window.
	//-----------------------------------------------------------------------
	virtual void SelectMDIActive();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTWindowList)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTWindowList)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLBox();
	afx_msg void OnBtnActivate();
	afx_msg void OnBtnSave();
	afx_msg void OnBtnClosewindows();
	afx_msg void OnBtnTilehorz();
	afx_msg void OnBtnTilevert();
	afx_msg void OnBtnCascade();
	afx_msg void OnBtnMinimize();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_DATA(CXTWindowList)
	enum { IDD = XT_IDD_WINDOWLIST };
	CButton  m_btnMinimize;
	CButton  m_btnCascade;
	CButton  m_btnOk;
	CButton  m_btnTileVert;
	CButton  m_btnTileHorz;
	CButton  m_btnSave;
	CButton  m_btnClose;
	CButton  m_btnActivate;
	//}}AFX_DATA
//}}AFX_CODEJOCK_PRIVATE

	HWND                        m_hWndMDIClient; // Handle to MDI client area.
	HICON                       m_hIcon;         // Resize dialogs handle
	CMDIFrameWnd*               m_pMDIFrameWnd;  // Pointer to applications MDI frame.
	CXTListBox  m_lboxWindows;   // Flicker free list box to display MDI children.

};

#endif // !defined(_XTWINDOWLIST_H__)
