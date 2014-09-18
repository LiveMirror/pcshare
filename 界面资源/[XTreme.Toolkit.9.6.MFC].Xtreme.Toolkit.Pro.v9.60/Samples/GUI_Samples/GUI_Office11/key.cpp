// key.cpp : implementation file
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
#include "key.h"
#include <winreg.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKey

void CKey::Close()
{
	if (m_hKey != NULL)
	{
		LONG lRes = RegCloseKey(m_hKey);
		ASSERT(lRes == ERROR_SUCCESS);
		m_hKey = NULL;
	}
}

BOOL CKey::Create(HKEY hKey, LPCTSTR lpszKeyName)
{
	ASSERT(hKey != NULL);
	return (RegCreateKey(hKey, lpszKeyName, &m_hKey) == ERROR_SUCCESS);
}

BOOL CKey::Open(HKEY hKey, LPCTSTR lpszKeyName)
{
	ASSERT(hKey != NULL);
	return (RegOpenKey(hKey, lpszKeyName, &m_hKey) == ERROR_SUCCESS);
}

BOOL CKey::SetStringValue(LPCTSTR lpszValue, LPCTSTR lpszValueName)
{
	ASSERT(m_hKey != NULL);
	return (RegSetValueEx(m_hKey, lpszValueName, NULL, REG_SZ,
		(BYTE * const)lpszValue, (lstrlen(lpszValue)+1)*sizeof(TCHAR)) == ERROR_SUCCESS);
}

BOOL CKey::GetStringValue(CString& str, LPCTSTR lpszValueName)
{
	ASSERT(m_hKey != NULL);
	str.Empty();
	DWORD dw = 0;
	DWORD dwType = 0;
	LONG lRes = RegQueryValueEx(m_hKey, (LPTSTR)lpszValueName, NULL, &dwType,
		NULL, &dw);
	if (lRes == ERROR_SUCCESS)
	{
		ASSERT(dwType == REG_SZ);
		LPTSTR lpsz = str.GetBufferSetLength(dw);
		lRes = RegQueryValueEx(m_hKey, (LPTSTR)lpszValueName, NULL, &dwType, (BYTE*)lpsz, &dw);
		ASSERT(lRes == ERROR_SUCCESS);
		str.ReleaseBuffer();
		return TRUE;
	}
	return FALSE;
}
