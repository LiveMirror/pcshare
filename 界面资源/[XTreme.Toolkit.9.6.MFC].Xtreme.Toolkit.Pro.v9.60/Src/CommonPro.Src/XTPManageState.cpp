// XTPManageState.cpp: implementation of the CXTPManageState class.
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

#include "stdafx.h"
#include "XTPMacros.h"
#include "XTPResourceManager.h"

#include "XTPManageState.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//=============================================================================
// CXTPManageState
//=============================================================================

HMODULE CXTPManageState::m_hModXTInstance = NULL;
HMODULE CXTPManageState::m_hModXTResource = NULL;

#if defined( _AFXDLL )
CDynLinkLibrary* CXTPManageState::m_pDynamicExtension = NULL;
#endif


CXTPManageState::CXTPManageState()
{
	m_bSet = false;
	Redo();
}

CXTPManageState::~CXTPManageState()
{
	Undo();
	m_hModOldResource = NULL;
	m_hModOldInstance = NULL;
}

void CXTPManageState::SetModuleInfo(HMODULE hModRes, HMODULE hModInst)
{
	m_hModXTInstance = hModInst;
	m_hModXTResource = hModRes;
}

void CXTPManageState::ResetModuleInfo(HMODULE hModRes, HMODULE hModInst)
{
#if defined(_AFXDLL)
	if (m_hModXTInstance != hModInst)
	{
		SAFE_DELETE(m_pDynamicExtension);
		AFX_EXTENSION_MODULE ext = {FALSE, hModInst, hModRes, NULL, NULL};
		m_pDynamicExtension = new CDynLinkLibrary(ext, FALSE);
	}
#endif
	CXTPManageState::SetModuleInfo(hModRes, hModInst );
}

void CXTPManageState::Undo()
{
	if (!m_bSet)
		return;

	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	pModuleState->m_hCurrentResourceHandle = m_hModOldResource;
	pModuleState->m_hCurrentInstanceHandle = m_hModOldInstance;

	m_bSet = false;
}

void CXTPManageState::Redo()
{
	if (m_hModXTResource == NULL)
	{
		m_bSet = false;
		return;
	}

	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	m_hModOldResource = pModuleState->m_hCurrentResourceHandle;
	m_hModOldInstance = pModuleState->m_hCurrentInstanceHandle;

	pModuleState->m_hCurrentResourceHandle = m_hModXTResource;
	if (pModuleState->m_hCurrentInstanceHandle == NULL)
		pModuleState->m_hCurrentInstanceHandle = m_hModXTInstance;

	m_bSet = true;
}

HMODULE CXTPManageState::GetResourceHandle()
{
	return m_hModXTResource;
}

HMODULE CXTPManageState::GetInstanceHandle()
{
	return m_hModXTInstance;
}

//=============================================================================
// CXTPDialogState
//=============================================================================

CXTPDialogState::CXTPDialogState()
{
	// not needed for static link
	#if defined(_XT_STATICLINK) || defined(_XT_DYN_STATICLIB) || defined(_XT_NO_MANAGED_STATE)
	m_pState = NULL;
	#else
	m_pState = new CXTPManageState();
	#endif
}

CXTPDialogState::~CXTPDialogState()
{
	RevertResourceContext();
}

void CXTPDialogState::RevertResourceContext()
{
	SAFE_DELETE(m_pState);
}

//=============================================================================
// CXTPCriticalSection
//=============================================================================

CXTPCriticalSection::CXTPCriticalSection()
{
	::InitializeCriticalSection(&m_csMutex);
}

CXTPCriticalSection::~CXTPCriticalSection()
{
	::DeleteCriticalSection(&m_csMutex);
}

void CXTPCriticalSection::EnterCritical()
{
	::EnterCriticalSection(&m_csMutex);
}

void CXTPCriticalSection::LeaveCritical()
{
	::LeaveCriticalSection(&m_csMutex);
}

//=============================================================================
// CXTPLockGuard
//=============================================================================

CXTPLockGuard::CXTPLockGuard(CXTPCriticalSection& key)
: m_key(key)
{
	LockThread();
}

CXTPLockGuard::~CXTPLockGuard()
{
	UnLockThread();
}

void CXTPLockGuard::LockThread()
{
	m_key.EnterCritical();
}

void CXTPLockGuard::UnLockThread()
{
	m_key.LeaveCritical();
}
