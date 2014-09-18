// XTPagerCtrl.h interface for the CXTPagerCtrl class.
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
#if !defined(__XTPAGERCTRL_H__)
#define __XTPAGERCTRL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// Summary:
//     CXTPagerCtrl is a CWnd derived class. It is used to contain and scroll
//     another window, and wraps the windows pager API.
//===========================================================================
class _XT_EXT_CLASS CXTPagerCtrl : public CWnd
{
	DECLARE_DYNAMIC(CXTPagerCtrl)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPagerCtrl object
	//-----------------------------------------------------------------------
	CXTPagerCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPagerCtrl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPagerCtrl();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function creates a pager child window and attaches it to
	//     this CWnd object. Returns nonzero if successful, otherwise returns zero.
	// Parameters:
	//     dwStyle    - Specifies the window style attributes. See the Remarks section for a list of style to use.
	//     rect       - The size and position of the window, in client coordinates of
	//                  'pParentWnd'.
	//     pParentWnd - The parent window.
	//     nID        - The ID of the child window.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:<p/>
	//     * <b>PGS_AUTOSCROLL</b> The pager control will scroll when the
	//       user hovers the mouse over one of the scroll buttons.
	//     * <b>PGS_DRAGNDROP</b> The contained window can be a drag-and-drop
	//       target. The pager control will automatically scroll if an item
	//       is dragged from outside the pager over one of the scroll buttons.
	//     * <b>PGS_HORZ</b> Creates a pager control that can be scrolled
	//       horizontally. This style and the PGS_VERT style are mutually
	//       exclusive and cannot be combined.
	//     * <b>PGS_VERT</b> Creates a pager control that can be scrolled
	//       vertically. This is the default direction; no direction style
	//       is specified. This style and the PGS_HORZ style are mutually
	//       exclusive and cannot be combined.
	//-----------------------------------------------------------------------
	virtual BOOL Create(DWORD dwStyle,const RECT& rect,CWnd* pParentWnd,UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the contained window for the pager control.
	//     This command will not change the parent of the contained window. It
	//     only assigns a window handle to the pager control for scrolling. In
	//     most cases, the contained window will be a child window. If this is
	//     the case, the contained window should be a child of the pager control.
	// Parameters:
	//     hwndChild - Handle to the window to be contained.
	//-----------------------------------------------------------------------
	void SetChild(HWND hwndChild);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function forces the pager control to recalculate the
	//     size of the contained window. Using this command will result in a
	//     PGN_CALCSIZE notification being sent.
	//-----------------------------------------------------------------------
	void RecalcSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function enables or disables mouse forwarding for the
	//     pager control. When mouse forwarding is enabled, the pager control
	//     forwards WM_MOUSEMOVE messages to the contained window.
	// Parameters:
	//     bForward - BOOL value that determines if mouse forwarding is enabled or
	//                disabled. If this value is nonzero, mouse forwarding is enabled.
	//                If this value is zero, mouse forwarding is disabled.
	//-----------------------------------------------------------------------
	void ForwardMouse(BOOL bForward);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the current background color for the pager
	//     control.
	// Parameters:
	//     clr - COLORREF value that contains the new background color of the
	//           pager control.
	// Returns:
	//     A COLORREF value that contains the previous background color.
	//-----------------------------------------------------------------------
	COLORREF SetBkColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the current background color for
	//     the pager control.
	// Returns:
	//     A COLORREF value that contains the current background color.
	//-----------------------------------------------------------------------
	COLORREF GetBkColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the current border size for the pager
	//     control.
	// Parameters:
	//     iBorder - New size of the border, in pixels. This value should not be
	//               larger than the pager button or less than zero. If 'iBorder'
	//               is too large, the border will be drawn the same size as the
	//               button. If 'iBorder' is negative, the border size will be set
	//               to zero.
	// Returns:
	//     An integer value that contains the previous border size in pixels.
	//-----------------------------------------------------------------------
	int SetBorder(int iBorder);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the current border size for the pager
	//     control.
	// Returns:
	//     An integer value that contains the current border size in pixels.
	//-----------------------------------------------------------------------
	int GetBorder();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the scroll position for the pager control.
	// Parameters:
	//     iPos - Integer value that contains the new scroll position in pixels.
	//-----------------------------------------------------------------------
	void SetPos(int iPos);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the current scroll position of the
	//     pager control.
	// Returns:
	//     An integer value that contains the current scroll position in pixels.
	//-----------------------------------------------------------------------
	int GetPos();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the current button size for the pager
	//     control.
	// Parameters:
	//     iSize - Integer value that contains the new button size in pixels.
	// Returns:
	//     An integer value that contains the previous button size in pixels.
	//-----------------------------------------------------------------------
	int SetButtonSize(int iSize);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the current button size for the pager
	//     control.
	// Returns:
	//     An integer value that contains the current button size in pixels.
	//-----------------------------------------------------------------------
	int GetButtonSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the state of the specified button
	//     in a pager control.
	// Parameters:
	//     iButton - Indicates which button to retrieve the state for. See the description
	//               for 'iButton' in PGM_GETBUTTONSTATE for a list of possible values.
	// Returns:
	//     The state of the button specified in 'iButton'. See the return value
	//     description in PGM_GETBUTTONSTATE for a list of possible values.
	//-----------------------------------------------------------------------
	DWORD GetButtonState(int iButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves a pager control's IDropTarget interface
	//     pointer.
	// Parameters:
	//     ppdt - Address of an IDropTarget pointer that receives the interface
	//            pointer. It is the caller's responsibility to call Release on this
	//            pointer when it is no longer needed.
	//-----------------------------------------------------------------------
	void GetDropTarget(IDropTarget **ppdt);


protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPagerCtrl)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPagerCtrl)
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPagerCtrl::SetChild(HWND hwndChild) {
	ASSERT(::IsWindow(m_hWnd)); Pager_SetChild(m_hWnd, hwndChild);
}
AFX_INLINE void CXTPagerCtrl::RecalcSize() {
	ASSERT(::IsWindow(m_hWnd)); Pager_RecalcSize(m_hWnd);
}
AFX_INLINE void CXTPagerCtrl::ForwardMouse(BOOL bForward) {
	ASSERT(::IsWindow(m_hWnd)); Pager_ForwardMouse(m_hWnd, bForward);
}
AFX_INLINE COLORREF CXTPagerCtrl::SetBkColor(COLORREF clr) {
	ASSERT(::IsWindow(m_hWnd)); return Pager_SetBkColor(m_hWnd, clr);
}
AFX_INLINE COLORREF CXTPagerCtrl::GetBkColor() {
	ASSERT(::IsWindow(m_hWnd)); return Pager_GetBkColor(m_hWnd);
}
AFX_INLINE int CXTPagerCtrl::SetBorder(int iBorder) {
	ASSERT(::IsWindow(m_hWnd)); return Pager_SetBorder(m_hWnd, iBorder);
}
AFX_INLINE int CXTPagerCtrl::GetBorder() {
	ASSERT(::IsWindow(m_hWnd)); return Pager_GetBorder(m_hWnd);
}
AFX_INLINE void CXTPagerCtrl::SetPos(int iPos) {
	ASSERT(::IsWindow(m_hWnd)); Pager_SetPos(m_hWnd, iPos);
}
AFX_INLINE int CXTPagerCtrl::GetPos() {
	ASSERT(::IsWindow(m_hWnd)); return Pager_GetPos(m_hWnd);
}
AFX_INLINE int CXTPagerCtrl::SetButtonSize(int iSize) {
	ASSERT(::IsWindow(m_hWnd)); return Pager_SetButtonSize(m_hWnd, iSize);
}
AFX_INLINE int CXTPagerCtrl::GetButtonSize() {
	ASSERT(::IsWindow(m_hWnd)); return Pager_GetButtonSize(m_hWnd);
}
AFX_INLINE DWORD CXTPagerCtrl::GetButtonState(int iButton) {
	ASSERT(::IsWindow(m_hWnd)); return Pager_GetButtonState(m_hWnd, iButton);
}
AFX_INLINE void CXTPagerCtrl::GetDropTarget(IDropTarget **ppdt) {
	ASSERT(::IsWindow(m_hWnd)); Pager_GetDropTarget(m_hWnd, ppdt);
}
AFX_INLINE BOOL CXTPagerCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

#endif // #if !defined(__XTPAGERCTRL_H__)
