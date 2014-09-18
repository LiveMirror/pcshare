// XTWndHook.cpp : implementation of the CXTWndHook class.
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

#include "StdAfx.h"
#include "XTResource.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTWndHook.h"

#ifndef _XTP_INCLUDE_CONTROLS
#include "XTToolBarCtrl.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTToolBar.h"
#include "XTStandardDefines.h"
#include "XTMenuBar.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTWndHook implementation
/////////////////////////////////////////////////////////////////////////////
CXTWndHook::CXTWndHook()
{
	m_pNextHook   = NULL;
	m_pOldWndProc = NULL;
	m_hWnd        = NULL;
}

CXTWndHook::~CXTWndHook()
{
	if (IsWindowHooked()) {
		HookWindow((HWND)NULL);
	}
}

CXTWindowMap CXTWindowMap::m_mapHooks;

IMPLEMENT_DYNAMIC(CXTWndHook, CObject)

BOOL CXTWndHook::HookWindow(HWND hWnd)
{
	ASSERT_VALID(this);

	// Hook the window
	if (hWnd)
	{
		ASSERT(m_hWnd==NULL);
		ASSERT(::IsWindow(hWnd));
		CXTWindowMap::m_mapHooks.Add(hWnd, this);
	}

	// Unhook the window
	else if (m_hWnd)
	{
		CXTWindowMap::m_mapHooks.Remove(this);
		m_pOldWndProc = NULL;
	}

	m_hWnd = hWnd;
	return TRUE;
}

LRESULT CXTWndHook::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	ASSERT(m_pOldWndProc);
	return m_pNextHook ? m_pNextHook->WindowProc(message, wParam, lParam) :
		::CallWindowProc(m_pOldWndProc, m_hWnd, message, wParam, lParam);
}

LRESULT CXTWndHook::Default()
{
	// MFC stores current MSG in thread state
	MSG& curMsg = AfxGetThreadState()->m_lastSentMsg;
	// Note: must explicitly call CXTWndHook::WindowProc to avoid infinite
	// recursion on virtual function
	return CXTWndHook::WindowProc(curMsg.message, curMsg.wParam, curMsg.lParam);
}
////////////////////////////////////////////////////////////////
// CXTWindowMap implementation
////////////////////////////////////////////////////////////////
CXTWindowMap::CXTWindowMap()
{

}

CXTWindowMap::~CXTWindowMap()
{

}

LRESULT CALLBACK CXTWindowMap::HookWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Set up MFC message state just in case anyone wants it
	// This is just like AfxCallWindowProc, but we can't use that because
	// a CXTWndHook is not a CWnd.
	MSG& curMsg = AfxGetThreadState()->m_lastSentMsg;
	MSG  oldMsg = curMsg;
	curMsg.hwnd = hWnd;
	curMsg.message = message;
	curMsg.wParam  = wParam;
	curMsg.lParam  = lParam;

	// Get hook object for this window. Get from hook map
	CXTWndHook* pXTWndHook = m_mapHooks.Lookup(hWnd);
	ASSERT(pXTWndHook);

	LRESULT lResult;

	// Window is being destroyed: unhook all hooks (for this window)
	// and pass message to original window proc
	if (message==WM_NCDESTROY)
	{
		WNDPROC wndproc = pXTWndHook->m_pOldWndProc;
		m_mapHooks.RemoveAll(hWnd);
		lResult = ::CallWindowProc(wndproc, hWnd, message, wParam, lParam);

	}

	// pass to message hook
	else {
		lResult = pXTWndHook->WindowProc(message, wParam, lParam);
	}

	curMsg = oldMsg;
	return lResult;
}

void CXTWindowMap::Add(HWND hWnd, CXTWndHook* pXTWndHook)
{
	ASSERT(hWnd && ::IsWindow(hWnd));

	// Add to front of list
	pXTWndHook->m_pNextHook = Lookup(hWnd);
	SetAt(hWnd, pXTWndHook);

	// If this is the first hook added, subclass the window
	if (pXTWndHook->m_pNextHook==NULL)
	{
		pXTWndHook->m_pOldWndProc = (WNDPROC)GetWindowLongPtr(
			hWnd, GWL_WNDPROC);
		SetWindowLongPtr(hWnd, GWL_WNDPROC, (LONG_PTR)HookWndProc);
	}

	// just copy wndproc from next hook
	else {
		pXTWndHook->m_pOldWndProc = pXTWndHook->m_pNextHook->m_pOldWndProc;
	}

	ASSERT(pXTWndHook->m_pOldWndProc);
}

void CXTWindowMap::Remove(CXTWndHook* pUnHook)
{
	HWND hWnd = pUnHook->m_hWnd;
	ASSERT(hWnd && ::IsWindow(hWnd));

	CXTWndHook* pHook = Lookup(hWnd);
	ASSERT(pHook);
	if (pHook==pUnHook)
	{
		// hook to remove is the one in the hash table: replace w/next
		if (pHook->m_pNextHook) {
			SetAt(hWnd, pHook->m_pNextHook);
		}

		// This is the last hook for this window: restore wnd proc
		else {
			RemoveKey(hWnd);
			SetWindowLongPtr(hWnd, GWL_WNDPROC, (LONG_PTR)pHook->m_pOldWndProc);
		}
	}
	else
	{
		// Hook to remove is in the middle: just remove from linked list
		while (pHook->m_pNextHook!=pUnHook) {
			pHook = pHook->m_pNextHook;
		}

		ASSERT(pHook && pHook->m_pNextHook==pUnHook);
		pHook->m_pNextHook = pUnHook->m_pNextHook;
	}
}

void CXTWindowMap::RemoveAll(HWND hWnd)
{
	CXTWndHook* pXTWndHook;
	while ((pXTWndHook = Lookup(hWnd))!=NULL)
		pXTWndHook->HookWindow((HWND)NULL);
}

CXTWndHook* CXTWindowMap::Lookup(HWND hWnd)
{
	CXTWndHook* pFound = NULL;
	if (!CMapPtrToPtr::Lookup(hWnd, (void*&)pFound))
		return NULL;
	ASSERT_KINDOF(CXTWndHook, pFound);
	return pFound;
}

IMPLEMENT_DYNAMIC(CXTWindowMap, CMapPtrToPtr)

