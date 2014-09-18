// XTPManageState.h: interface for the CXTPManageState class.
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
#if !defined(__XTPMANAGESTATE_H__)
#define __XTPMANAGESTATE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//=============================================================================
// Summary:
//     Macro for using the CXTPManageState class - see class for more details
//=============================================================================
#if defined(_XT_STATICLINK) || defined(_XT_DYN_STATICLIB) || defined(_XT_NO_MANAGED_STATE)

	#define XT_MANAGE_STATE
	#define XT_MANAGE_STATE_BEGIN
	#define XT_MANAGE_STATE_END
	#define XT_UNDO_STATE
	#define XT_REDO_STATE
#else
	#define XT_MANAGE_STATE CXTPManageState state;
	#define XT_MANAGE_STATE_BEGIN { CXTPManageState state;
	#define XT_MANAGE_STATE_END }
	#define XT_UNDO_STATE state.Undo();
	#define XT_REDO_STATE state.Redo();
#endif // #if defined(_XT_STATICLINK) || defined(_XT_DYN_STATICLIB) || defined(_XT_NO_MANAGED_STATE)
//}}AFX_CODEJOCK_PRIVATE

//=============================================================================
// Summary:
//     CXTPManageState is used internally by the toolkit to switch module
//     states.  Specifically, the class enables the toolkit to read its
//     own .dll resources, and not that of the host application (or .dll).
//     Future versions of the class may handle other such details
//=============================================================================
class _XTP_EXT_CLASS CXTPManageState
{
	friend class CXTPDialogState;

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPManageState object
	//-------------------------------------------------------------------------
	CXTPManageState();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPManageState object, handles clean up and deallocation
	//-------------------------------------------------------------------------
	~CXTPManageState();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member function to UNDO managed state
	//-------------------------------------------------------------------------
	void Undo();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member function to REDO managed state
	//-------------------------------------------------------------------------
	void Redo();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the resource and instance
	//     handles for the calling module.
	// Parameters:
	//     hModInst - Handle to modules resource.
	//     hModRes - Handle to modules instance.
	//-------------------------------------------------------------------------
	static void AFX_CDECL SetModuleInfo(HMODULE hModRes,HMODULE hModInst);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member function to reset the resource and instance
	//     handles for the calling module.
	// Parameters:
	//     hModInst - Handle to modules resource.
	//     hModRes - Handle to modules instance.
	//-------------------------------------------------------------------------
	static void AFX_CDECL ResetModuleInfo(HMODULE hModRes, HMODULE hModInst);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the library resource handle.
	// Returns:
	//     Returns the resource handle defined for the library.
	//-------------------------------------------------------------------------
	static HMODULE AFX_CDECL GetResourceHandle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the library instance handle.
	// Returns:
	//     Returns the instance handle defined for the library.
	//-------------------------------------------------------------------------
	static HMODULE AFX_CDECL GetInstanceHandle();

protected:

	bool            m_bSet;             // true to set
	HMODULE         m_hModOldResource;  // Handle to previous resource.
	HMODULE         m_hModOldInstance;  // Handle to previous instance.
	static HMODULE  m_hModXTResource;   // Handle to toolkit resource.
	static HMODULE  m_hModXTInstance;   // Handle to toolkit instance.

private:

	#if defined(_AFXDLL)
	static CDynLinkLibrary* m_pDynamicExtension;
	#endif
};

//=============================================================================
// Summary:
//     CXTPDialogState is a stand alone class. It is an internal class used
//     by toolkit dialogs to manage resource context.
//=============================================================================
class _XTP_EXT_CLASS CXTPDialogState
{
protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDialogState object
	//-------------------------------------------------------------------------
	CXTPDialogState();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDialogState object, handles cleanup and deallocation
	//-------------------------------------------------------------------------
	~CXTPDialogState();

	//-------------------------------------------------------------------------
	// Summary:
	//     Restores the application resource context.  Should be used within
	//     the derived class's constructor.
	//-------------------------------------------------------------------------
	void RevertResourceContext();

private:

	CXTPManageState* m_pState; // CXTPManageState pointer.
};

//{{AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary: Critical Section Wrapper
//===========================================================================
class _XTP_EXT_CLASS CXTPCriticalSection
{
public:
	CXTPCriticalSection();
	~CXTPCriticalSection();

public:
	void EnterCritical();
	void LeaveCritical();

private:
	CRITICAL_SECTION m_csMutex;
};


class _XTP_EXT_CLASS CXTPLockGuard
{
public:
	CXTPLockGuard(CXTPCriticalSection& key);
	~CXTPLockGuard();

public:
	void LockThread();
	void UnLockThread();

private:
	CXTPCriticalSection& m_key;
};

//}}AFX_CODEJOCK_PRIVATE



#endif // !defined(__XTPMANAGESTATE_H__)
