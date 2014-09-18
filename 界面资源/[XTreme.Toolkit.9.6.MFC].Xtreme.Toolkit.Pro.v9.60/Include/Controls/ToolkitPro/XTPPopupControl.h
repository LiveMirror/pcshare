// XTPPopupControl.h: interface for the CXTPPopupControl class.
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
#if !defined(_XTPPOPUPCONTROL_H__)
#define _XTPPOPUPCONTROL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPPopupItem;
class CXTPPopupPaintManager;
class CXTPImageManager;

//-----------------------------------------------------------------------
// Summary:
//     Enumeration used to determine the popup window theme in use
// Example:
//     <code>m_wndPopupControl.SetTheme(xtpPopupThemeOffice2000);</code>
// See Also: CXTPPopupControl::SetTheme, CXTPPopupControl::GetTheme
//
// <KEYWORDS xtpPopupThemeOffice2000, xtpPopupThemeOfficeXP, xtpPopupThemeOffice2003, xtpPopupThemeMSN, xtpPopupThemeCustom>
//-----------------------------------------------------------------------
enum XTPPopupPaintTheme
{
	xtpPopupThemeOffice2000,    // The theme like to a Office2000
	xtpPopupThemeOfficeXP,      // The theme like to a OfficeXP
	xtpPopupThemeOffice2003,    // The theme like to a Office2003
	xtpPopupThemeMSN,           // The theme like to a MSN Messenger
	xtpPopupThemeCustom         // The custom theme
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration used to determine the popup window animation in use
// Example:
//     <code>m_wndPopupControl.SetPopupAnimation(xtpPopupAnimationFade);</code>
// See Also: CXTPPopupControl::SetPopupAnimation, CXTPPopupControl::GetPopupAnimation
//
// <KEYWORDS xtpPopupAnimationNone, xtpPopupAnimationFade, xtpPopupAnimationSlide, xtpPopupAnimationUnfold>
//-----------------------------------------------------------------------
enum XTPPopupAnimation
{
	xtpPopupAnimationNone,      // Animation is not used
	xtpPopupAnimationFade,      // Fade method animation
	xtpPopupAnimationSlide,     // Slide method animation
	xtpPopupAnimationUnfold     // Unfold method animation
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration used to determine a current popup window state
// Example:
// <code>
// if(pControl->GetPopupState() == xtpPopupStateClosed)
// {
//     EnableItems(TRUE);
// }
// </code>
// See Also: CXTPPopupControl::GetPopupState, CXTPPopupControl::SetPopupState
//
// <KEYWORDS xtpPopupStateClosed, xtpPopupStateExpanding, xtpPopupStateShow, xtpPopupStateCollapsing>
//-----------------------------------------------------------------------
enum XTPPopupState
{
	xtpPopupStateClosed,        // The popup window is closed
	xtpPopupStateExpanding,     // The popup window is expanding
	xtpPopupStateShow,          // The popup window is shown
	xtpPopupStateCollapsing     // The popup window is collapsing
};

//-----------------------------------------------------------------------
// Summary:
//     The XTPWM_POPUPCONTROL_NOTIFY message is sent to the CXTPPopupControl owner window
//     whenever an action occurs within the CXTPPopupControl
// Parameters:
//     nAction -  Value of wParam specifies a Popup Control value that indicates the user's
//                request. Can be any of the values listed in the Remarks section.
// Remarks:
//     nAction parameter can be one of the following:
//     * <b>XTP_PCN_ITEMCLICK</b> Indicates the user clicked on the popup control item.
//     * <b>XTP_PCN_STATECHANGED</b> Indicates the state of control changed.
// Returns:
//     If the application is to process this message, the return value should be TRUE, otherwise the
//     return value is FALSE.
// Example:
//     Here is an example of how an application would process the XTPWM_POPUPCONTROL_NOTIFY
//     message.
// <code>
//
// BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_MESSAGE(XTPWM_POPUPCONTROL_NOTIFY, OnPopUpNotify)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CMainFrame::OnPopUpNotify(WPARAM wParam, LPARAM lParam)
// {
//      if(wParam == XTP_PCN_ITEMCLICK)
//      {
//          CXTPPopupItem* pItem = (CXTPPopupItem*)lParam;
//          ASSERT(pItem);
//
//          m_wndPopupControl.Close();
//
//          if(pItem->GetID() == ID_GOTO_SITE)
//          {
//              ::ShellExecute(NULL, _T("open"), _T("http://www.codejock.com/"), NULL, NULL, SW_SHOW);
//          }
//      }
//      else if(wParam == XTP_PCN_STATECHANGED)
//      {
//          CXTPPopupControl* pControl = (CXTPPopupControl*)lParam;
//          ASSERT(pControl);
//
//          if(pControl->GetPopupState() == xtpPopupStateClosed)
//          {
//              EnableItems(TRUE);
//          }
//      }
//      return TRUE;
//  }
// </code>
// See Also:
//     XTP_PCN_ITEMCLICK, XTP_PCN_STATECHANGED, CXTPPopupControl
//-----------------------------------------------------------------------
const UINT XTPWM_POPUPCONTROL_NOTIFY = (WM_APP + 2591);

//-----------------------------------------------------------------------
// Summary:
//     XTP_PCN_ITEMCLICK is used to indicate that the user clicked on a popup control item.
// Remarks:
//     XTP_PCN_ITEMCLICK is sent in the XTPWM_POPUPCONTROL_NOTIFY message to the
//     owner window when the user has clicked on a popup control item.
// Example:
//   See example of XTPWM_POPUPCONTROL_NOTIFY
// See Also:
//     XTPWM_POPUPCONTROL_NOTIFY, XTP_PCN_STATECHANGED
//-----------------------------------------------------------------------
const UINT XTP_PCN_ITEMCLICK = 1;

//-----------------------------------------------------------------------
// Summary:
//     XTP_PCN_STATECHANGED is used to indicate that the state of popup control changed
// Remarks:
//     XTP_PCN_STATECHANGED is sent in the XTPWM_POPUPCONTROL_NOTIFY message to the
//     owner window when the state of popup control changed.
//     It can be one of the following states:
//         * <b>xtpPopupStateClosed</b> Indicates the the popup window is closed
//         * <b>xtpPopupStateExpanding</b> Indicates the popup window is expanding
//         * <b>xtpPopupStateShow</b> Indicates the popup window is shown
//         * <b>xtpPopupStateCollapsing</b> Indicates the popup window is collapsing
// Example:
//   See example of XTPWM_POPUPCONTROL_NOTIFY
// See Also:
//     XTPWM_POPUPCONTROL_NOTIFY, XTP_PCN_ITEMCLICK
//-----------------------------------------------------------------------
const UINT XTP_PCN_STATECHANGED = 2;

//===========================================================================
// Summary:
//     CXTPPopupControl is a class for control of popup window
//===========================================================================
class _XTP_EXT_CLASS CXTPPopupControl : public CWnd
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Popup state descriptor
	// See Also: CXTPPopupControl::m_stateTarget, CXTPPopupControl::m_stateCurrent
	//-----------------------------------------------------------------------
	struct POPUPSTATE
	{
		CRect rcPopup;                              // Popup position
		int nTransparency;                          // Popup transparency
	};


public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPopupControl object.
	//-----------------------------------------------------------------------
	CXTPPopupControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPopupControl object, all handles cleanup and
	//     deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPopupControl();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the position of a popup
	//     window.
	// Parameters:
	//     ptPopup  - a CPoint object containing XY location.
	//-----------------------------------------------------------------------
	void SetPopupPos(CPoint ptPopup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the position of a  popup
	//     window.
	// Returns:
	//     A CPoint object containing XY location.
	//-----------------------------------------------------------------------
	CPoint GetPopupPos();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the size of a popup window.
	// Parameters:
	//     szPopup  - A CSize object containing XY size.
	//-----------------------------------------------------------------------
	void SetPopupSize(CSize szPopup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the size of a popup window.
	// Returns:
	//     A CSize object containing XY size.
	//-----------------------------------------------------------------------
	CSize GetPopupSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member for create and starting expanding state
	//     of a popup window
	// Parameters:
	//     pParent - points to a CWnd object that represents the parent
	//     window.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL Show(CWnd* pParent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member for starting collapsing state of a popup
	//     window with the posterior destroy.
	//-----------------------------------------------------------------------
	void Hide();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to destroy a popup window.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL Close();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the transparency value of a
	//     popup window.
	// Parameters:
	//     bTransp - value of transparency (0...255).
	//     If bTransp = 0 the popup window is full transparent, otherwise
	//     if bTransp = 255 the popup window is full opaque.
	//-----------------------------------------------------------------------
	void SetTransparency(BYTE bTransp);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the transparency value a popup
	//     window.
	// Returns:
	//     Transparency value of a popup window.
	//-----------------------------------------------------------------------
	BYTE GetTransparency();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the value of show delay a
	//     popup window.
	// Parameters:
	//     dwShowDelay - value of show delay in milliseconds.
	//-----------------------------------------------------------------------
	void SetShowDelay(DWORD dwShowDelay);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the value of show delay a
	//     popup window.
	// Returns:
	//     Transparency value of show delay in milliseconds.
	//-----------------------------------------------------------------------
	DWORD GetShowDelay();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the value of animate delay
	//     a popup window.
	// Parameters:
	//     dwAnimateDelay - value of animate delay in milliseconds .
	//-----------------------------------------------------------------------
	void SetAnimateDelay(DWORD dwAnimateDelay);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the value of animate
	//     delay a popup window.
	// Returns:
	//     Transparency value of animate delay in milliseconds.
	//-----------------------------------------------------------------------
	DWORD GetAnimateDelay();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the popup animation method.
	// Parameters:
	//     popupAnimation - method in use, see XTPPopupAnimation enum.
	//-----------------------------------------------------------------------
	void SetPopupAnimation(XTPPopupAnimation popupAnimation);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the popup animation method.
	// Returns:
	//     Method in use, see XTPPopupAnimation enum.
	//-----------------------------------------------------------------------
	XTPPopupAnimation GetPopupAnimation();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the current state of a popup
	//     window.
	// Returns:
	//     Current state, see XTPPopupState enum.
	//-----------------------------------------------------------------------
	XTPPopupState GetPopupState();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the paint manager object used
	//     for drawing a popup window.
	// Parameters:
	//     pPaintManager - point of paint manager object.
	//-----------------------------------------------------------------------
	void SetTheme(CXTPPopupPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the paint manager object used
	//     for drawing a popup window.
	// Returns:
	//     Point of paint manager object.
	//-----------------------------------------------------------------------
	CXTPPopupPaintManager* GetPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the theme of popup window
	//     drawing.
	// Parameters:
	//     theme - theme of popup window drawing, see XTPPopupPaintTheme
	//     enum.
	// See Also: GetTheme, XTPPopupPaintTheme
	//-----------------------------------------------------------------------
	void SetTheme(XTPPopupPaintTheme theme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the theme that the popup window is
	//     currently using.
	// Returns:
	//     The currently used theme of the popup window drawing.
	// See Also: SetTheme, XTPPopupPaintTheme
	//-----------------------------------------------------------------------
	XTPPopupPaintTheme GetTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to redraw all controlled items of a
	//     popup window.
	//-----------------------------------------------------------------------
	void RedrawControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to install new controlled item in
	//     a popup window.
	// Parameters:
	//     pItem - point to CXTPPopupItem object which will be installed
	//     in a popup window.
	// Returns:
	//     Point to the installed CXTPPopupItem object.
	//-----------------------------------------------------------------------
	CXTPPopupItem* AddItem(CXTPPopupItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove all controlled items from
	//     a popup window.
	//-----------------------------------------------------------------------
	void RemoveAllItems();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove controlled item from
	//     a popup window.
	// Parameters:
	//     pItem - point to CXTPPopupItem object which will be installed
	//     in a popup window.
	//-----------------------------------------------------------------------
	void RemoveItem(CXTPPopupItem * pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get count of controlled items
	//     a popup window.
	// Returns:
	//     Count of controlled items.
	//-----------------------------------------------------------------------
	int GetItemCount();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get a controlled item on an index
	// Parameters:
	//     nIndex - index of a controlled item.
	// Returns:
	//     Point to CXTPPopupItem object.
	//-----------------------------------------------------------------------
	CXTPPopupItem* GetItem(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to testing a point for controlled
	//     items of a popup window.
	// Parameters:
	//     pt - a CPoint object containing XY location for testing.
	// Returns:
	//     Point to CXTPPopupItem object if test is successful, otherwise
	//     returns NULL.
	//-----------------------------------------------------------------------
	CXTPPopupItem* HitTest(CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether the user can click and drag
	//     the popup window when it is displayed.
	// Parameters:
	//     bAllowMove - TRUE to allow the user can click and drag the popup
	//                  window anywhere on the screen.
	//                  If FALSE, the popup window cannot be moved by the user.
	//-----------------------------------------------------------------------
	void AllowMove(BOOL bAllowMove);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a popup window
	// Parameters:
	//     pParentWnd - Pointer to the parent window.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Create(CWnd* pParentWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when user click on controlled item
	// Parameters:
	//     pItem - Item has been clicked.
	//-----------------------------------------------------------------------
	virtual void OnClick(CXTPPopupItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to start expanding animation
	// Parameters:
	//     bUpdateCurrent - TRUE to update current state.
	//-----------------------------------------------------------------------
	virtual void OnExpanding(BOOL bUpdateCurrent);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to start collapsing animation
	//-------------------------------------------------------------------------
	virtual void OnCollapsing();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to show control
	//-------------------------------------------------------------------------
	virtual void OnShow();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to notify parent about action.
	// Parameters:
	//     wParam - Occured action
	//     lParam - Extended parameter.
	//-----------------------------------------------------------------------
	virtual void Notify(WPARAM wParam, LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method animates popup control
	// Parameters:
	//     nStep - Current step of animation.
	//-----------------------------------------------------------------------
	void Animate(int nStep);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates position in animation according current animation step
	// Parameters:
	//     bInit - Initial step of animation.
	//-----------------------------------------------------------------------
	void UpdateState(BOOL bInit = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when current state of control changed.
	// Parameters:
	//     popupState - Current state of control.
	//-----------------------------------------------------------------------
	void SetPopupState(XTPPopupState popupState);

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates current control transparency
	// Parameters:
	//     bAlpha - Current alpha level of window
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL SetLayeredWindowAttributes(BYTE bAlpha);

	//-------------------------------------------------------------------------
	// Summary:
	//     Loop of control moving.
	//-------------------------------------------------------------------------
	void TrackMove();


	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set new image manager.
	// Parameters:
	//     pImageManager - Points to a CXTPImageManager object to be set
	// Example:
	// <code>
	//     CXTPImageManager* pImageManager = new CXTPImageManager();
	//     pImageManager->SetIcons(IDR_MAINFRAME);
	//     m_wndPopupControl.SetImageManager(pImageManager);
	// </code>
	// See Also:
	//     GetImageManager
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get a pointer to the image manager of popup
	//      control.
	// Returns:
	//     Pointer to the image manager of popup control.
	// Remarks:
	//     The image manager is used to hold all of the icons displayed in the
	//     popup control.
	// See Also:
	//     SetImageManager
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager();


protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPPopupControl)
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:

	CXTPPopupPaintManager* m_pPaintManager;         // point to CXTPPopupPaintManager object
	CArray<CXTPPopupItem*, CXTPPopupItem*> m_arrItems; // array for controlled items
	CXTPPopupItem* m_pSelected;                     // point to selected item
	CXTPPopupItem* m_pPressed;                      // point to pressed item
	XTPPopupAnimation m_popupAnimation;             // current animation method
	XTPPopupState m_popupState;                     // current popup state
	CSize m_szPopup;                                // current XY location of a popup window
	CPoint m_ptPopup;                               // current XY size of a popup window
	LONG m_nTransparency;                           // current value transparency of a popup window
	UINT m_uShowDelay;                              // value of show delay of a popup window
	UINT m_uAnimationDelay;                         // value of animation delay of a popup window
	UINT m_nAnimationInterval;                      // value of animation interval of a popup window
	int  m_nStep;                                   // value of step animation of a popup window
	XTPPopupPaintTheme m_paintTheme;                // Currently used theme.
	BOOL m_bAllowMove;                              // If TRUE, the user can click and drag the popup window anywhere on the screen.  If FALSE, the popup window cannot be moved by the user.

	POPUPSTATE  m_stateTarget;                      // Target descriptor
	POPUPSTATE  m_stateCurrent;                     // Current descriptor
	CXTPImageManager* m_pImageManager;              // Images of popup items.

private:
	PVOID       m_pfnSetLayeredWindowAttributes;    // point to Transparency proc in USER32.dll module
	BOOL        m_bCapture;                         // capture flag of mouse event

	friend class CXTPPopupItem;
	friend class CPopupControlCtrl;
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPPopupPaintManager* CXTPPopupControl::GetPaintManager() {
	return m_pPaintManager;
}
AFX_INLINE void CXTPPopupControl::SetPopupSize(CSize szPopup) {
	m_szPopup = szPopup;
}
AFX_INLINE CSize CXTPPopupControl::GetPopupSize() {
	return m_szPopup;
}
AFX_INLINE void CXTPPopupControl::SetPopupPos(CPoint ptPopup) {
	m_ptPopup = ptPopup;
}
AFX_INLINE CPoint CXTPPopupControl::GetPopupPos() {
	return m_ptPopup;
}
AFX_INLINE void CXTPPopupControl::SetShowDelay(DWORD dwShowDelay) {
	m_uShowDelay = dwShowDelay;
}
AFX_INLINE DWORD CXTPPopupControl::GetShowDelay() {
	return m_uShowDelay;
}
AFX_INLINE void CXTPPopupControl::SetAnimateDelay(DWORD dwAnimateDelay) {
	m_uAnimationDelay = dwAnimateDelay;
}
AFX_INLINE DWORD CXTPPopupControl::GetAnimateDelay() {
	return m_uAnimationDelay;
}
AFX_INLINE BYTE CXTPPopupControl::GetTransparency() {
	return (BYTE)m_nTransparency;
}
AFX_INLINE void CXTPPopupControl::SetTransparency(BYTE nTransparency) {
	m_nTransparency = nTransparency;
}
AFX_INLINE XTPPopupAnimation CXTPPopupControl::GetPopupAnimation() {
	return m_popupAnimation;
}
AFX_INLINE void CXTPPopupControl::SetPopupAnimation(XTPPopupAnimation popupAnimation) {
	m_popupAnimation = popupAnimation;
}
AFX_INLINE XTPPopupPaintTheme CXTPPopupControl::GetTheme() {
	return m_paintTheme;
}
AFX_INLINE void CXTPPopupControl::AllowMove(BOOL bAllowMove) {
	m_bAllowMove = bAllowMove;
}
AFX_INLINE BOOL CXTPPopupControl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

#endif // !defined(_XTPPOPUPCONTROL_H__)
