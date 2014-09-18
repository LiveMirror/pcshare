// XTPHookManager.h : interface for the CXTPHookManager class.
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
#if !defined(__XTPHOOKMANAGER_H__)
#define __XTPHOOKMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPCommandBarsDefines.h"


//===========================================================================
// Summary:
//     HookAble interface.
// See Also: CXTPHookManager, CXTPHookManager::SetHook
//===========================================================================
class CXTPHookManagerHookAble
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by WindowProc, or is called during message reflection.
	// Parameters:
	//     hWnd - Window handle that the message belongs to.
	//     nMessage - Specifies the message to be sent.
	//     wParam - Specifies additional message-dependent information.
	//     lParam - Specifies additional message-dependent information.
	//     lResult - The return value of WindowProc. Depends on the message; may be NULL.
	// Returns:
	//     TRUE if message was processed.
	//-----------------------------------------------------------------------
	virtual int OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult) = 0;
};


//===========================================================================
// Summary:
//     CXTPHookManager is standalone class. It is used to hook a CWnd object
//     in order to intercept and act upon window messages that are received.
// Remarks:
//     To access CXTPHookManager methods use XTPHookManager function
// See Also:
//     XTPHookManager, CXTPHookManagerHookAble
//===========================================================================
class _XTP_EXT_CLASS CXTPHookManager
{
private:
	class CHookSink;

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPHookManager object.
	//-----------------------------------------------------------------------
	CXTPHookManager(void);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPHookManager object, handles cleanup and de-
	//     allocation.
	//-----------------------------------------------------------------------
	~CXTPHookManager(void);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will hook a window so that its messages are
	//     intercepted before they are passed on to the specified window.
	// Parameters:
	//     hWnd  - A handle to a window that represents that represents the window to hook.
	//     pHook - Hookable class that will receive messages
	//-----------------------------------------------------------------------
	void SetHook(HWND hWnd, CXTPHookManagerHookAble* pHook );

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes a hook associated with a window.
	// Parameters:
	//     hWnd  - A handle to a window that hooks need to remove
	//     pHook - Hookable class that hooks need to remove
	//-----------------------------------------------------------------------
	void RemoveHook(HWND hWnd, CXTPHookManagerHookAble* pHook );

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes all hooks associated with a window.
	// Parameters:
	//     hWnd  - A handle to a window that hooks need to remove
	//     pHook - Hookable class that hooks need to remove
	//-----------------------------------------------------------------------
	void RemoveAll(CXTPHookManagerHookAble* pHook); // <combine CXTPHookManager::RemoveHook>
	void RemoveAll(HWND hWnd);  // <combine CXTPHookManager::RemoveHook>

	//-----------------------------------------------------------------------
	// Summary:
	//     Searches collection of Hookable interfaces that receive hooks of specified window.
	// Parameters:
	//     hWnd  - A handle to a window need to test
	// Returns:
	//     CHookSink pointer if found; otherwise returns NULL;
	//-----------------------------------------------------------------------
	CHookSink* Lookup(HWND hWnd);

private:
	static LRESULT CALLBACK HookWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	void RemoveAll();
	static CXTPHookManager s_managerInstance;
	CMap<HWND, HWND, LPVOID, LPVOID> m_mapHooks;

	friend class CXTPHookManager* XTPHookManager();
	friend class CHookSink;
};


//---------------------------------------------------------------------------
// Summary:
//     Call this member function to access CXTPHookManager members.
//     Since this class is designed as a single instance object you can
//     only access version info through this method. You <b>cannot</b>
//     directly instantiate an object of type CXTPHookManager.
// Example:
//     <code>XTPHookManager()->SetHook(hWnd, this);</code>
//---------------------------------------------------------------------------
CXTPHookManager* XTPHookManager();



//{{AFX_CODEJOCK_PRIVATE

class _XTP_EXT_CLASS CXTPKeyboardManager : public CNoTrackObject
{
public:

	CXTPKeyboardManager(void);
	~CXTPKeyboardManager();

	void SetupKeyboardHook(BOOL bSetup = TRUE);
	void SetupMessageHook(BOOL bSetup = TRUE);
	void HookKeyboard(CXTPHookManagerHookAble* pHook);
	void UnhookKeyboard(CXTPHookManagerHookAble* pHook);
	BOOL IsKeyboardHooked() { return m_lstKeyboardHooks.GetCount() > 0; }

protected:

	BOOL ProcessKeyboardHooks(UINT, WPARAM wParam, LPARAM lParam = 0);
	static LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK CallWndProc(int code, WPARAM wParam, LPARAM lParam);

protected:
	HHOOK m_hHookKeyboard;
	HHOOK m_hHookMessage;

#ifdef _AFXDLL
	AFX_MODULE_STATE* m_pModuleState;
#endif

	static CThreadLocal<CXTPKeyboardManager> _xtpKeyboardThreadState;

	CList<CXTPHookManagerHookAble*, CXTPHookManagerHookAble*> m_lstKeyboardHooks;

	friend CXTPKeyboardManager* XTPKeyboardManager();
	friend class CXTPCommandBars;

};

//}}AFX_CODEJOCK_PRIVATE


AFX_INLINE CXTPKeyboardManager* XTPKeyboardManager() {
	return CXTPKeyboardManager::_xtpKeyboardThreadState.GetData();
}

AFX_INLINE CXTPHookManager* XTPHookManager() {
	return &CXTPHookManager::s_managerInstance;
}

#endif //#if !defined(__XTPHOOKMANAGER_H__)
