// XTWndHook.h interface for the CXTWndHook class.
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
#if !defined(__XTWNDHOOK_H__)
#define __XTWNDHOOK_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// Summary:
//     CXTWndHook is a CWnd derived class. It is used to hook a CWnd object
//     in order to intercept and act upon window messages that are received.
//===========================================================================
class _XT_EXT_CLASS CXTWndHook : public CObject
{
	DECLARE_DYNAMIC(CXTWndHook)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTWndHook object
	//-----------------------------------------------------------------------
	CXTWndHook();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTWndHook object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTWndHook();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will check whether or not the current window has
	//     been hooked.
	// Returns:
	//     TRUE if the window has been hooked, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsWindowHooked();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will hook a window so that its messages are
	//     intercepted before they are passed on to the specified window.
	// Parameters:
	//     pWnd - Points to a CWnd object that represents the window to hook.
	//            If set to NULL, the window hook is removed.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL HookWindow(CWnd* pWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will install a new window procedure that directs
	//     messages to the CXTWndHook.
	// Parameters:
	//     hWnd - A handle to a window that represents the window whose window
	//            procedure is to be hooked. If set to NULL, the WNDPROC is removed.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL HookWindow(HWND hWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function provides a Windows procedure (WindowProc) for
	//     a CXTWndHook object. It dispatches messages through the window's message
	//     map.
	// Parameters:
	//     message - Specifies the Windows message to be processed.
	//     wParam  - Provides additional information used in processing the message.
	//               The parameter value depends on the message.
	//     lParam  - Provides additional information used in processing the message.
	//               The parameter value depends on the message.
	// Returns:
	//     An LRESULT value. The return value depends on the message.
	//-----------------------------------------------------------------------
	virtual LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function calls the default window procedure. The default
	//     window procedure provides default processing for any window message
	//     that an application does not process. This member function ensures
	//     that every message is processed.
	// Returns:
	//     An LRESULT value. The return value depends on the message.
	//-----------------------------------------------------------------------
	LRESULT Default();

protected:

	HWND        m_hWnd;         // The window hooked.
	WNDPROC     m_pOldWndProc;  // The original window procedure.
	CXTWndHook* m_pNextHook;    // Next, in chain of hooks, for this window.

	friend class CXTWindowMap;
};


//===========================================================================
// Summary:
//     CXTWindowMap is a CMapPtrToPtr derived class. It is used to create
//     a CXTWindowMap class object.
//===========================================================================
class _XT_EXT_CLASS CXTWindowMap : private CMapPtrToPtr
{
	DECLARE_DYNAMIC(CXTWindowMap)

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTWindowMap object
	//-----------------------------------------------------------------------
	CXTWindowMap();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTWindowMap object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTWindowMap();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function adds a hook to the map, that is, it associates
	//     the hook with the window.
	// Parameters:
	//     hWnd       - Valid window handle.
	//     pXTWndHook - A pointer to a CXTWndHook object.
	//-----------------------------------------------------------------------
	void Add(HWND hWnd,CXTWndHook* pXTWndHook);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function removes a hook from the map.
	// Parameters:
	//     pXTWndHook - A pointer to a CXTWndHook object.
	//-----------------------------------------------------------------------
	void Remove(CXTWndHook* pXTWndHook);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function removes all the hooks for a window.
	// Parameters:
	//     hWnd - Valid window handle.
	//-----------------------------------------------------------------------
	void RemoveAll(HWND hWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is a subclassed window procedure for message
	//     hooks and replaces AfxWndProc or whatever else was there before.
	// Parameters:
	//     hWnd    - Valid window handle.
	//     message - Specifies the Windows message to be processed.
	//     wParam  - Provides additional information used in processing the message.
	//               The parameter value depends on the message.
	//     lParam  - Provides additional information used in processing the message.
	//               The parameter value depends on the message.
	// Returns:
	//     An LRESULT object
	//-----------------------------------------------------------------------
	static LRESULT CALLBACK HookWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function finds the first hook associated with the window.
	// Parameters:
	//     hWnd - Valid window handle.
	// Returns:
	//     A CXTWndHook pointer.
	//-----------------------------------------------------------------------
	CXTWndHook* Lookup(HWND hWnd);

protected:
	static CXTWindowMap m_mapHooks; // Self instance

	friend class CXTWndHook;
};


//////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTWndHook::IsWindowHooked() {
	return (m_hWnd != NULL && ::IsWindow(m_hWnd));;
}
AFX_INLINE BOOL CXTWndHook::HookWindow(CWnd* pWnd) {
	return HookWindow(pWnd->GetSafeHwnd());
}

#endif // #if !defined(__XTWNDHOOK_H__)
