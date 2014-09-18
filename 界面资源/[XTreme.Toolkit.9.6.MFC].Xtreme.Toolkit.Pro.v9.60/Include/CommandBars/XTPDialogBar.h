// XTPToolBar.h : interface for the CXTPToolBar class.
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
#if !defined(__XTPDIALOGBAR_H__)
#define __XTPDIALOGBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPPopupBar;

#include "XTPToolBar.h"

//===========================================================================
// Summary:
//     CXTPDialogBar is a CXTPToolBar derived class. It represents dockable container for child window
//===========================================================================
class _XTP_EXT_CLASS CXTPDialogBar : public CXTPToolBar
{
	class CXTPControlButtonHide;
	class CXTPControlCaptionPopup;
	class CXTPCaptionPopupBar;

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDialogBar object
	//-------------------------------------------------------------------------
	CXTPDialogBar();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDialogBar object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPDialogBar();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify the child window displayed under the
	//     dialog bar.
	// Parameters:
	//     pChild - Pointer to a CWnd object that is displayed under the
	//              dialog bar.
	//-----------------------------------------------------------------------
	void SetChild(CWnd* pChild);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the text that will be displayed in the
	//     dialog bar's caption popup.
	// Parameters:
	//     strCaption - Text that is displayed in the dialog bar's caption popup.
	// Remarks:
	//     This is the text displayed in the dialog bar's caption popup.  The
	//     caption popup can be used to indicate what is being displayed in the
	//     child window.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR strCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the text displayed in the caption popup.
	// Returns:
	//     The text displayed in the caption popup.
	//-----------------------------------------------------------------------
	CString GetCaption();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to hide the caption popup control (and close button).
	// Parameters:
	//     bVisible - TRUE to hide the caption popup control, FALSE to show it.
	// Remarks:
	//     If bVisible is TRUE, only the commandbar is displayed, the caption
	//     popup and close button are hidden.
	//-----------------------------------------------------------------------
	void SetCaptionVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to tell the caption popup which CMenu popup to display
	//     when the the caption popup is dropped down.
	// Parameters:
	//     nMenuResourceID - Id of the CMenu resource that contains the popup
	//                       menu that should be displayed when the caption popup
	//                       is clicked/displayed.
	// Remarks:
	//     If there is more than one popup on the CMenu resource, then only the
	//     first popup on the CMenu will be used.
	//-----------------------------------------------------------------------
	void SetCaptionPopup(int nMenuResourceID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve a pointer to the CXTPPopupBar that is
	//     displayed when the caption popup control is clicked/dropped down.
	// Returns:
	//     Pointer to the CXTPPopupBar that is displayed when the caption
	//     popup control is clicked/dropped down.
	//-----------------------------------------------------------------------
	CXTPPopupBar* GetCaptionPopup();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether the dialog bar will be re-sizable.
	// Parameters:
	//     bResizable - TRUE if the dialog bar can be resized, FALSE to lock
	//                  the dialog bar.
	// Remarks:
	//     By default, bResizable is TRUE.  If FALSE, no splitter is displayed
	//     when the mouse pointer is positioned over the dialog bar borders.
	//     You can use the SetSize member function to change the size of the
	//     dialog bar programmatically.
	//-----------------------------------------------------------------------
	void SetResizable(BOOL bResizable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the width and height of the dialog bar child window.
	// Parameters:
	//     sz - The width and height of the dialog bar's child window.
	// Remarks:
	//     The size is only used to size the child window when the dialog
	//     bar is floating, then both the specified width and height are used.
	//     If the bar is dock on the left or right, only the width is used,
	//     the height will depend on how much of the client area the dialog bar
	//     can occupy.  If the bar is docked on the top or bottom, then only
	//     the height is used to size the child window.
	//-----------------------------------------------------------------------
	void SetSize(CSize sz);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method calculates the dimensions of a toolbar.
	// Parameters:
	//     nLength - Length of the bar.
	//     dwMode - Mode to dock.
	//     nWidth - Width of the bar.
	// Returns:
	//     Size of the docked toolbar.
	//-----------------------------------------------------------------------
	CSize CalcDockingLayout(int nLength, DWORD dwMode, int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method calculates the dimensions of a toolbar.
	// Parameters:
	//     nLength - The requested dimension of the control bar, either horizontal or vertical, depending on dwMode.
	//      dwMode - see CControlBar::CalcDynamicLayout for list of supported flags.
	// Returns:
	//     Size of the docked toolbar.
	//-----------------------------------------------------------------------
	CSize CalcDynamicLayout(int nLength, DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the CommandBar is a
	//     CXTPDialogBar.
	// Remarks:
	//     This will always return TRUE.
	// Returns:
	//     Returns TRUE if the CommandBar is a CXTPDialogBar, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsDialogBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when user resize dialog bar.
	// Parameters:
	//     rcWindow - CRect object specifying the size of the window.
	//     nHitTest - Specifies the hit-test area code.
	//-----------------------------------------------------------------------
	void OnResize(CRect rcWindow, int nHitTest);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method calculates the dimensions of a dialog bar.
	// Parameters:
	//     nLength - Length of the bar.
	//     dwMode - Mode to dock.
	//     szClient - Client size of dialog bar
	// Returns:
	//     Size of the dialog bar.
	//-----------------------------------------------------------------------
	CSize CalcSize(int nLength, DWORD dwMode, CSize szClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine mouse hit code
	// Parameters:
	//     pt - Position to test
	// Returns:
	//     HTCLIENT if pointer belongs client area; HTNOWHERE otherwise.
	//-----------------------------------------------------------------------
	int OnMouseHitTest(CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves client margins of dialog bar.
	// Returns:
	//     Rectangle contained top, left, right, bottom margins of dialog bar.
	//-----------------------------------------------------------------------
	CRect GetMargins();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to reposition child window in dialog bar.
	// Parameters:
	//     rc - CRect object specifying size of area.
	//-----------------------------------------------------------------------
	void MoveChildWindow(CRect rc);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPDialogBar)
	UINT OnNcHitTest(CPoint point);
	void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	CRect m_rcBorders;          // Borders size.
	BOOL m_bResizable;          // TRUE if the child window and dialog bar are re-sizable.
	CSize m_szMinTrackRect;     // Minimum tracking rectangle
	BOOL m_bCaptionVisible;     // TRUE if the caption popup control is visible.
	BOOL m_bCaptionPopup;       // TRUE to use caption as popup control.
	int m_nCaptionHeight;       // Caption height.
	CSize m_szDockingClient;    // Docking client rectangle
	CSize m_szFloatingClient;   // Floating client rectangle
	HWND m_hwndChild;           // Child window handle.
	CString m_strCaption;       // Text that is displayed in the caption popup control.
	CXTPPopupBar* m_pPopupBar;  // Pointer to caption popup bar.


protected:

	DECLARE_XTP_COMMANDBAR(CXTPDialogBar)

	friend class CXTPToolBar;
	friend class CXTPDockContext;


};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTPDialogBar::IsDialogBar() {
	return TRUE;
}
AFX_INLINE CXTPPopupBar* CXTPDialogBar::GetCaptionPopup() {
	return m_pPopupBar;
}
AFX_INLINE void CXTPDialogBar::SetResizable(BOOL bResizable) {
	m_bResizable = bResizable;
}


#endif  //#if !defined(__XTPDIALOGBAR_H__)
