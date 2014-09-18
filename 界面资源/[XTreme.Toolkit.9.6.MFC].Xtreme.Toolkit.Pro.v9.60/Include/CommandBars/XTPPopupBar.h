// XTPPopupBar.h : interface for the CXTPPopupBar class.
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
#if !defined(__XTPPOPUPBAR_H__)
#define __XTPPOPUPBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPCommandBar.h"

class CXTPControlPopup;
class CXTPCommandBars;

//===========================================================================
// Summary:
//     CXTPPopupBar is a CXTPCommandBar derived class. It represents submenu of menu bar.
//===========================================================================
class _XTP_EXT_CLASS CXTPPopupBar : public CXTPCommandBar
{
	DECLARE_XTP_COMMANDBAR(CXTPPopupBar)

private:
	class CXTPControlExpandButton;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPopupBar object
	//-----------------------------------------------------------------------
	CXTPPopupBar(void);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPopupBar object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPPopupBar(void);


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a popup bar object.
	// Parameters:
	//     pCommandBars - Points to a CXTPCommandBars object
	// Returns:
	//     A pointer to a CXTPPopupBar object
	//-----------------------------------------------------------------------
	static CXTPPopupBar* AFX_CDECL CreatePopupBar(CXTPCommandBars* pCommandBars);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method adds the tear-off state to the popup bar.
	// Parameters:
	//     strCaption - Caption of the tear-off toolbar.
	//     nID        - Identifier of the toolbar to be created.
	//     nWidth     - Width of the toolbar.
	//-----------------------------------------------------------------------
	void SetTearOffPopup(CString strCaption, UINT nID, int nWidth = 400);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if Popupbar has tear-off state.
	// Parameters:
	//     strCaption - Caption of the tear-off toolbar.
	//     nID        - Identifier of the toolbar to be created.
	//     nWidth     - Width of the toolbar.
	// Returns:
	//     TRUE if tear-off; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL IsTearOffPopup(CString& strCaption, UINT& nID, int& nWidth);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the popup menu is currently visible\popped up.
	// Returns:
	//     TRUE is the popup menu is visible/displayed, FALSE if the popup menu
	//     is currently hidden.
	//-------------------------------------------------------------------------
	BOOL IsVisible();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method opens the popup bar.
	// Parameters:
	//     x         - Specifies the logical x-coordinate of the popup bar
	//     y         - Specifies the logical y-coordinate of the  position.
	//     rcExclude - Excluded area.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL Popup(int x, int y, LPCRECT rcExclude);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the control's popup.
	// Returns:
	//     A pointer to a CXTPControlPopup object.
	//-----------------------------------------------------------------------
	CXTPControlPopup* GetControlPopup() { return m_pControlPopup; }

	//-----------------------------------------------------------------------
	// Summary:
	//     This method expands the bar.
	//-----------------------------------------------------------------------
	void ExpandBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to destroy the window.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL DestroyWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to make popup bar looks like popup toolbar.
	// Parameters:
	//     bToolBarType - TRUE to make bar as popup toolbar
	//-----------------------------------------------------------------------
	void SetPopupToolBar(BOOL bToolBarType);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the default menu item for the specified popup bar.
	// Parameters:
	//     uItem  - Identifier or position of the new default menu item or
	//              - 1 for no default item. The meaning of this parameter
	//              depends on the value of fByPos.
	//     fByPos - Value specifying the meaning of uItem. If this parameter
	//              is FALSE, uItem is a menu item identifier. Otherwise,
	//              it is a menu item position.
	//-----------------------------------------------------------------------
	void SetDefaultItem(UINT uItem, BOOL fByPos = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the default menu item on the specified popup bar.
	// Parameters:
	//     gmdiFlags - Reserved. Should be NULL.
	//     fByPos    - Value specifying whether to retrieve the menu item's
	//                 identifier or its position. If this parameter is
	//                 FALSE, the identifier is returned. Otherwise, the
	//                 position is returned.
	// Returns:
	//     If the function succeeds, the return value is the identifier
	//     or position of the menu item. If the function fails, the return
	//     value is - 1.
	//-----------------------------------------------------------------------
	UINT GetDefaultItem(UINT gmdiFlags, BOOL fByPos = FALSE);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Redraw the popup bar.
	//-----------------------------------------------------------------------
	void DelayRedraw() { Redraw(); }

	//-----------------------------------------------------------------------
	// Summary:
	//     This method opens the popup bar.
	// Parameters:
	//     pControlPopup - Pointer to a CXTPControlPopup object
	//     bSelectFirst  - TRUE to select the first item.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL Popup(CXTPControlPopup* pControlPopup, BOOL bSelectFirst = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the size of the command bar.
	// Parameters:
	//      nLength - The requested dimension of the control bar, either horizontal or vertical, depending on dwMode.
	//      dwMode - see CControlBar::CalcDynamicLayout for list of supported flags.
	// Returns:
	//     Size of the command bar.
	//-----------------------------------------------------------------------
	CSize CalcDynamicLayout(int nLength, DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to change the tracking state.
	// Parameters:
	//     bMode        - TRUE to set the tracking mode; otherwise FALSE.
	//     bSelectFirst - TRUE to select the first item.
	//     bKeyboard    - TRUE if the item is popuped by the keyboard.
	// Returns:
	//     TRUE if the method was successful.
	// See Also: IsTrackingMode.
	//-----------------------------------------------------------------------
	BOOL SetTrackingMode(int bMode, BOOL bSelectFirst = TRUE, BOOL bKeyboard = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to select the specified control.
	// Parameters:
	//     nSelected - An integer index of the item to be selected.
	//     bKeyboard - TRUE if the item was selected using the keyboard.
	// Returns:
	//     TRUE if the method was successful; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetSelected(int nSelected, BOOL bKeyboard = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates the popup bar.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL Create();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the PopupBar position.
	// Parameters:
	//     sz - Size of the popup bar.
	//-----------------------------------------------------------------------
	void UpdateLocation(CSize sz);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates the control's shortcuts.
	//-----------------------------------------------------------------------
	virtual void UpdateShortcuts();

	//-----------------------------------------------------------------------
	// Summary:
	//     Update flags.
	//-----------------------------------------------------------------------
	virtual void UpdateFlags();

	//-----------------------------------------------------------------------
	// Summary:
	//     Update expanding state.
	//-----------------------------------------------------------------------
	void UpdateExpandingState();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the command bar in the given context.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//-----------------------------------------------------------------------
	void DrawCommandBar(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the controls array is changed.
	//-----------------------------------------------------------------------
	virtual void OnControlsChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to recalculate the command bar layout.
	//-----------------------------------------------------------------------
	void OnRecalcLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the parent command bar
	// Returns:
	//     A pointer to a CXTPCommandBar object
	//-----------------------------------------------------------------------
	CXTPCommandBar* GetParentCommandBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method make a copy of the command bar.
	// Parameters:
	//     pCommandBar - Command bar needed to be copied.
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	virtual void Copy(CXTPCommandBar* pCommandBar, BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function when a non-system key
	//     is pressed.
	// Parameters:
	//     nChar - Specifies the virtual key code of the given key.
	// Returns:
	//     TRUE if key handled, otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL OnKeyDown(UINT nChar);

	//-------------------------------------------------------------------------
	// Summary:
	//     Translates all messages in message queue.
	//-------------------------------------------------------------------------
	void PumpMessage();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the toolbars borders.
	// Parameters:
	//     rcBorders - Borders of the toolbar.
	//-----------------------------------------------------------------------
	void SetBorders(CRect rcBorders);

	//-----------------------------------------------------------------------
	// Summary:
	//     Disables shadow use.
	//-----------------------------------------------------------------------
	void DisableShadow();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPPopupBar)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	void AdjustScrolling(LPSIZE lpSize, BOOL bDown = FALSE, BOOL bInvalidate = FALSE);
	void TrackTearOff();
	void SwitchTearOffTracking(BOOL bShow, CPoint point);

	void Animate();
	XTPAnimationType GetAnimationType();

public:
	static double m_dMaxWidthDivisor;   // Maximum available width of popup

protected:
	CXTPControlPopup* m_pControlPopup;  // Control that popuped the bar.
	CPoint m_ptPopup;                   // Position of the bar.
	CRect m_rcExclude;                  // Excluded area.
	int  m_popupFlags;                  // Flags of the bar.
	BOOL m_bDynamicLayout;              // TRUE if DynamicLayout routine executed.
	BOOL m_bCollapsed;                  // TRUE if popupbar is collapsed.
	BOOL m_bExpanded;                   // TRUE if popupbar is expanded.
	BOOL m_bExpanding;                  // TRUE if popupbar is expanding.

	BOOL m_bTearOffPopup;               // TRUE if popupbar has tear-off option.
	CRect m_rcTearOffGripper;           // Tear-off gripper position.
	BOOL m_bTearOffSelected;            // TRUE if gripper selected.
	UINT_PTR m_nTearOffTimer;           // Tear-off timer.
	BOOL m_bTearOffTracking;            // Tear-off tracking mode.
	CSize m_szTearOffBar;               // Size of the bar.
	CXTPToolBar* m_pTearOffBar;         // Tear-off toolbar pointer.
	CString m_strTearOffCaption;        // Caption of the Tear-off toolbar.
	UINT m_nTearOffID;                  // ID of the Tear-off toolbar.
	int m_nTearOffWidth;                // Width of the Tear-off toolbar.
	BOOL m_bShowShadow;                 // TRUE to show the shadow.
	CRect m_rcBorders;                  // Borders of the bar.
	BOOL m_bCustomizeMenu;              // TRUE if popup is customized context menu.

private:

	struct SCROLLINFO
	{
		struct BTNSCROLL
		{
			void Init(CWnd* pParent, UINT nID)
			{
				m_pParent = pParent;
				m_nID = nID;
			}

			CRect rc;
			UINT nTimer;
			CWnd* m_pParent;
			UINT m_nID;

			void KillTimer() { m_pParent->KillTimer(nTimer); nTimer = 0;}
			void SetTimer() { if (nTimer == 0) nTimer = (UINT)m_pParent->SetTimer(m_nID, 80, 0); }
			BOOL OnMouseMove(CPoint point)
			{
				if (rc.PtInRect(point))
				{
					SetTimer();
					return TRUE;
				} else if (nTimer) KillTimer();
				return FALSE;
			}
		};

		BOOL bScroll;
		int nScrollFirst, nScrollLast;
		BTNSCROLL btnUp;
		BTNSCROLL btnDown;

		void Init(CWnd* pWnd, UINT nIDUp, UINT nIDDown)
		{
			btnUp.Init(pWnd, nIDUp);
			btnDown.Init(pWnd, nIDDown);
			bScroll = nScrollFirst = nScrollLast = 0;
		}
	} m_scrollInfo;



private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

	friend class CXTPControlExpandButton;
	friend class CXTPControlPopup;
	friend class CXTPCommandBars;
	friend class CXTPCommandBar;
	friend class CXTPCustomizeSheet;
};

//===========================================================================
// Summary:
//     CXTPPopupToolBar is a CXTPPopupBar derived class.
//===========================================================================
class _XTP_EXT_CLASS CXTPPopupToolBar : public CXTPPopupBar
{
	DECLARE_XTP_COMMANDBAR(CXTPPopupToolBar)

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPopupToolBar object
	//-----------------------------------------------------------------------
	CXTPPopupToolBar();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a popup toolbar object.
	// Parameters:
	//     pCommandBars - Points to a CXTPCommandBars object
	// Returns:
	//     A pointer to a CXTPPopupToolBar object
	//-----------------------------------------------------------------------
	static CXTPPopupToolBar* AFX_CDECL CreatePopupToolBar(CXTPCommandBars* pCommandBars);


protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the size of the command bar.
	// Parameters:
	//      nLength - The requested dimension of the control bar, either horizontal or vertical, depending on dwMode.
	//      dwMode - see CControlBar::CalcDynamicLayout for list of supported flags.
	// Returns:
	//     Size of the command bar.
	//-----------------------------------------------------------------------
	CSize CalcDynamicLayout(int nLength, DWORD dwMode);
};

//////////////////////////////////////////////////////////////////////////



AFX_INLINE void CXTPPopupBar::DisableShadow() {
	m_bShowShadow = FALSE;
}
AFX_INLINE void CXTPPopupBar::SetBorders(CRect rcBorders) {
	m_rcBorders = rcBorders;
}
AFX_INLINE BOOL CXTPPopupBar::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

#endif // #if !defined(__XTPPOPUPBAR_H__)
