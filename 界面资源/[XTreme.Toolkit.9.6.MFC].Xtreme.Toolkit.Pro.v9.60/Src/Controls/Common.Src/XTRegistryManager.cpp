// XTRegistryManager.cpp : implementation file
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
#include <atlbase.h>

#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTRegistryManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTRegistryManager

CString CXTRegistryManager::m_strINIFileName = _T("");

CXTRegistryManager::CXTRegistryManager(HKEY hKeyBase/*=HKEY_CURRENT_USER*/)
: m_pszRegistryKey(NULL)
, m_pszProfileName(NULL)
{
	ASSERT(hKeyBase == HKEY_CURRENT_USER || hKeyBase == HKEY_LOCAL_MACHINE);
	m_hKeyBase = hKeyBase;
}

CXTRegistryManager::~CXTRegistryManager()
{

}

BOOL CXTRegistryManager::GetProfileInfo()
{
	if (m_pszRegistryKey == NULL ||
		m_pszProfileName == NULL)
	{
		CWinApp* pWinApp = AfxGetApp();
		if (pWinApp != NULL)
		{
			m_pszRegistryKey = pWinApp->m_pszRegistryKey;
			m_pszProfileName = pWinApp->m_pszProfileName;
		}

		// If this fails, you need to call SetRegistryKey(...); in your
		// applications CWinApp::InitInstance() override, or you have called
		// this method before you call SetRegistryKey..  Calling SetRegistryKey
		// will initialize m_pszProfileName and m_pszRegistryKey for CWinApp.

		// NOTE: If you want to write to an INI file instead of the system
		// registry, call CXTRegistryManager::SetINIFileName(...) prior to
		// making this call.

		if (m_pszRegistryKey == NULL ||
			m_pszProfileName == NULL)
		{
			return FALSE;
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

HKEY CXTRegistryManager::GetAppRegistryKey()
{
	if (!GetProfileInfo())
		return 0;

	HKEY hAppKey = NULL;
	HKEY hSoftKey = NULL;
	HKEY hCompanyKey = NULL;
	if (::RegOpenKeyEx(m_hKeyBase, _T("Software"), 0, KEY_WRITE|KEY_READ,
		&hSoftKey) == ERROR_SUCCESS)
	{
		DWORD dw;
		if (::RegCreateKeyEx(hSoftKey, m_pszRegistryKey, 0, REG_NONE,
			REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
			&hCompanyKey, &dw) == ERROR_SUCCESS)
		{
			::RegCreateKeyEx(hCompanyKey, m_pszProfileName, 0, REG_NONE,
				REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
				&hAppKey, &dw);
		}
	}
	if (hSoftKey != NULL)
		::RegCloseKey(hSoftKey);
	if (hCompanyKey != NULL)
		::RegCloseKey(hCompanyKey);

	return hAppKey;
}

HKEY CXTRegistryManager::GetSectionKey(LPCTSTR lpszSection)
{
	ASSERT(lpszSection != NULL);

	HKEY hSectionKey = NULL;
	HKEY hAppKey = GetAppRegistryKey();
	if (hAppKey == NULL)
		return NULL;

	DWORD dw;
	::RegCreateKeyEx(hAppKey, lpszSection, 0, REG_NONE,
		REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
		&hSectionKey, &dw);
	::RegCloseKey(hAppKey);
	return hSectionKey;
}

BOOL CXTRegistryManager::WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);

	if (m_strINIFileName.IsEmpty())
	{
		if (!GetProfileInfo())
			return FALSE;

		HKEY hSecKey = GetSectionKey (lpszSection);
		if (hSecKey == NULL)
			return FALSE;
		LONG lResult = ::RegSetValueEx(hSecKey, lpszEntry, NULL, REG_DWORD,
			(LPBYTE)&nValue, sizeof(nValue));
		::RegCloseKey(hSecKey);
		return lResult == ERROR_SUCCESS;
	}
	else
	{
		ASSERT(m_strINIFileName.IsEmpty() == FALSE);

		TCHAR szT[16];
		wsprintf(szT, _T("%d"), nValue);
		return ::WritePrivateProfileString(lpszSection, lpszEntry, szT,
			m_strINIFileName);
	}
}

BOOL CXTRegistryManager::WriteProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE pData, UINT nBytes)
{
	ASSERT(lpszSection != NULL);
	if (m_strINIFileName.IsEmpty())
	{
		if (!GetProfileInfo())
			return FALSE;

		LONG lResult;
		HKEY hSecKey = GetSectionKey (lpszSection);
		if (hSecKey == NULL)
			return FALSE;
		lResult = ::RegSetValueEx(hSecKey, lpszEntry, NULL, REG_BINARY,
			pData, nBytes);
		::RegCloseKey(hSecKey);
		return lResult == ERROR_SUCCESS;
	}

	// convert to string and write out
	LPTSTR lpsz = new TCHAR[nBytes*2+1];
	UINT i;
	for (i = 0; i < nBytes; i++)
	{
		lpsz[i*2] = (TCHAR)((pData[i] & 0x0F) + 'A'); //low nibble
		lpsz[i*2+1] = (TCHAR)(((pData[i] >> 4) & 0x0F) + 'A'); //high nibble
	}
	lpsz[i*2] = 0;

	ASSERT(m_strINIFileName.IsEmpty() == FALSE);

	BOOL bResult = WriteProfileString(lpszSection, lpszEntry, lpsz);
	delete[] lpsz;
	return bResult;
}

BOOL CXTRegistryManager::WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
{
	ASSERT(lpszSection != NULL);
	if (m_strINIFileName.IsEmpty())
	{
		if (!GetProfileInfo())
			return FALSE;

		LONG lResult;
		if (lpszEntry == NULL) //delete whole section
		{
			HKEY hAppKey = GetAppRegistryKey();
			if (hAppKey == NULL)
				return FALSE;
			lResult = ::RegDeleteKey(hAppKey, lpszSection);
			::RegCloseKey(hAppKey);
		}
		else if (lpszValue == NULL)
		{
			HKEY hSecKey = GetSectionKey (lpszSection);
			if (hSecKey == NULL)
				return FALSE;
			// necessary to cast away const below
			lResult = ::RegDeleteValue(hSecKey, (LPTSTR)lpszEntry);
			::RegCloseKey(hSecKey);
		}
		else
		{
			HKEY hSecKey = GetSectionKey (lpszSection);
			if (hSecKey == NULL)
				return FALSE;
			lResult = ::RegSetValueEx(hSecKey, lpszEntry, NULL, REG_SZ,
				(LPBYTE)lpszValue, (lstrlen(lpszValue)+1)*sizeof(TCHAR));
			::RegCloseKey(hSecKey);
		}
		return lResult == ERROR_SUCCESS;
	}
	else
	{
		ASSERT(m_strINIFileName.IsEmpty() == FALSE);
		ASSERT(m_strINIFileName.GetLength() < 4095); // can't read in bigger
		return ::WritePrivateProfileString(lpszSection, lpszEntry, lpszValue,
			m_strINIFileName);
	}
}

UINT CXTRegistryManager::GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);
	if (m_strINIFileName.IsEmpty()) // use registry
	{
		if (!GetProfileInfo())
			return nDefault;

		HKEY hSecKey = GetSectionKey (lpszSection);
		if (hSecKey == NULL)
			return nDefault;
		DWORD dwValue;
		DWORD dwType;
		DWORD dwCount = sizeof(DWORD);
		LONG lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			(LPBYTE)&dwValue, &dwCount);
		::RegCloseKey(hSecKey);
		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_DWORD);
			ASSERT(dwCount == sizeof(dwValue));
			return (UINT)dwValue;
		}
		return nDefault;
	}
	else
	{
		ASSERT(m_strINIFileName.IsEmpty() == FALSE);
		return ::GetPrivateProfileInt(lpszSection, lpszEntry, nDefault,
			m_strINIFileName);
	}
}

BOOL CXTRegistryManager::GetProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, BYTE** ppData, UINT* pBytes)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);
	ASSERT(ppData != NULL);
	ASSERT(pBytes != NULL);

	*ppData = NULL;
	*pBytes = 0;

	if (m_strINIFileName.IsEmpty())
	{
		if (!GetProfileInfo())
			return FALSE;

		HKEY hSecKey = GetSectionKey (lpszSection);
		if (hSecKey == NULL)
			return FALSE;

		DWORD dwType, dwCount;
		LONG lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			NULL, &dwCount);
		*pBytes = dwCount;
		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_BINARY);
			*ppData = new BYTE[*pBytes];
			lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
				*ppData, &dwCount);
		}
		::RegCloseKey(hSecKey);
		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_BINARY);
			return TRUE;
		}
		else
		{
			delete [] *ppData;
			*ppData = NULL;
		}
		return FALSE;
	}
	else
	{
		ASSERT(m_strINIFileName.IsEmpty() == FALSE);

		CString str = GetProfileString(lpszSection, lpszEntry, NULL);
		if (str.IsEmpty())
			return FALSE;
		ASSERT(str.GetLength()%2 == 0);
		int nLen = str.GetLength();
		*pBytes = nLen/2;
		*ppData = new BYTE[*pBytes];
		int i;
		for (i=0;i<nLen;i+=2)
		{
			(*ppData)[i/2] = (BYTE)
				(((str[i+1] - 'A') << 4) + (str[i] - 'A'));
		}
		return TRUE;
	}
}

CString CXTRegistryManager::GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);
	if (m_strINIFileName.IsEmpty())
	{
		if (!GetProfileInfo())
			return lpszDefault;

		HKEY hSecKey = GetSectionKey (lpszSection);
		if (hSecKey == NULL)
			return lpszDefault;
		CString strValue;
		DWORD dwType, dwCount;
		LONG lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			NULL, &dwCount);
		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_SZ);
			lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
				(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
			strValue.ReleaseBuffer();
		}
		::RegCloseKey(hSecKey);
		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_SZ);
			return strValue;
		}
		return lpszDefault;
	}
	else
	{
		TCHAR chNil = '\0';

		ASSERT(m_strINIFileName.IsEmpty() == FALSE);

		if (lpszDefault == NULL) {
			lpszDefault = &chNil;    // don't pass in NULL
		}

		TCHAR szT[4096];
		DWORD dw = ::GetPrivateProfileString(lpszSection, lpszEntry,
			lpszDefault, szT, _countof(szT), m_strINIFileName);
		ASSERT(dw < 4095);
		return szT;
	}
}

int CXTRegistryManager::EnumValues(LPCTSTR lpszSection, CMap<CString, LPCTSTR, DWORD, DWORD&> * mapItems,
									CStringArray * arrayNames)
{
	ASSERT(lpszSection != NULL);

	HKEY hKey = GetSectionKey (lpszSection);

	int index = 0;

	TCHAR szValue[512];
	DWORD dwLen = 512;
	DWORD dwType;

	while (ERROR_NO_MORE_ITEMS != ::RegEnumValue(hKey, index++, szValue, &dwLen,
		NULL, &dwType, NULL, NULL))
	{
		if(mapItems) mapItems->SetAt(szValue, dwType);
		if (arrayNames) arrayNames->Add(szValue);
		dwLen = 512;
	}

	return --index;
}

int CXTRegistryManager::EnumKeys(LPCTSTR lpszSection, CStringArray & arrayKeys)
{
	ASSERT(lpszSection != NULL);

	HKEY hKey = GetSectionKey (lpszSection);

	int index = 0;

	TCHAR szValue[512];
	DWORD dwLen = 512;

	while (ERROR_NO_MORE_ITEMS != ::RegEnumKeyEx(hKey, index++, szValue, &dwLen,
		NULL, NULL, NULL, NULL))
	{
		arrayKeys.Add(szValue);
		dwLen = 512;
	}

	return --index;
}

bool CXTRegistryManager::DeleteKey(LPCTSTR lpszSection, LPCTSTR lpszKey)
{
	CRegKey regkey;

	HKEY hAppKey = GetAppRegistryKey();
	regkey.Create(hAppKey, lpszSection);
	::RegCloseKey(hAppKey);

	return (ERROR_SUCCESS == regkey.RecurseDeleteKey(lpszKey));
}

bool CXTRegistryManager::DeleteValue(LPCTSTR lpszSection, LPCTSTR lpszValue)
{
	CRegKey regkey;

	HKEY hAppKey = GetAppRegistryKey();
	regkey.Create(hAppKey, lpszSection);
	::RegCloseKey(hAppKey);

	return (ERROR_SUCCESS == regkey.DeleteValue(lpszValue));
}

BOOL CXTRegistryManager::WriteProfilePoint(LPCTSTR lpszSection, LPCTSTR lpszEntry, CPoint * pValue)
{
	ASSERT(lpszSection != NULL);
	if (m_strINIFileName.IsEmpty())
	{
		if (!GetProfileInfo())
			return FALSE;

		LONG lResult;
		HKEY hSecKey = GetSectionKey (lpszSection);
		if (hSecKey == NULL)
			return FALSE;
		lResult = ::RegSetValueEx(hSecKey, lpszEntry, NULL, REG_BINARY,
			(LPBYTE)pValue, sizeof(CPoint));
		::RegCloseKey(hSecKey);
		return lResult == ERROR_SUCCESS;
	}

	// convert to string and write out
	LPBYTE pData = (LPBYTE) pValue;
	UINT nBytes = sizeof(CPoint);
	LPTSTR lpsz = new TCHAR[nBytes*2+1];
	UINT i;
	for (i = 0; i < nBytes; i++)
	{
		lpsz[i*2] = (TCHAR)((pData[i] & 0x0F) + 'A'); //low nibble
		lpsz[i*2+1] = (TCHAR)(((pData[i] >> 4) & 0x0F) + 'A'); //high nibble
	}
	lpsz[i*2] = 0;

	ASSERT(m_strINIFileName.IsEmpty() == FALSE);

	BOOL bResult = WriteProfileString(lpszSection, lpszEntry, lpsz);
	delete[] lpsz;
	return bResult;
}

BOOL CXTRegistryManager::GetProfilePoint(LPCTSTR lpszSection, LPCTSTR lpszEntry, CPoint * ptResult)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);

	if (m_strINIFileName.IsEmpty())
	{
		if (!GetProfileInfo())
			return FALSE;

		HKEY hSecKey = GetSectionKey (lpszSection);
		if (hSecKey == NULL)
			return FALSE;

		DWORD dwType, dwCount;
		LONG lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			NULL, &dwCount);

		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_BINARY);
			lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
				(LPBYTE)ptResult, &dwCount);
		}
		::RegCloseKey(hSecKey);
		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_BINARY);
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		ASSERT(m_strINIFileName.IsEmpty() == FALSE);

		CString str = GetProfileString(lpszSection, lpszEntry, NULL);
		if (str.IsEmpty())
			return FALSE;

		ASSERT(str.GetLength()%2 == 0);
		int nLen = str.GetLength();

		LPBYTE pData = (LPBYTE) ptResult;

		pData = new BYTE[nLen/2];
		int i;
		for (i=0;i<nLen;i+=2)
		{
			(pData)[i/2] = (BYTE)
				(((str[i+1] - 'A') << 4) + (str[i] - 'A'));
		}

		return TRUE;
	}
}

BOOL CXTRegistryManager::WriteProfileRect(LPCTSTR lpszSection, LPCTSTR lpszEntry, CRect * pValue)
{
	ASSERT(lpszSection != NULL);
	if (m_strINIFileName.IsEmpty())
	{
		if (!GetProfileInfo())
			return FALSE;

		LONG lResult;
		HKEY hSecKey = GetSectionKey (lpszSection);
		if (hSecKey == NULL)
			return FALSE;
		lResult = ::RegSetValueEx(hSecKey, lpszEntry, NULL, REG_BINARY,
			(LPBYTE)pValue, sizeof(CRect));
		::RegCloseKey(hSecKey);
		return lResult == ERROR_SUCCESS;
	}

	// convert to string and write out
	LPBYTE pData = (LPBYTE) pValue;
	UINT nBytes = sizeof(CRect);
	LPTSTR lpsz = new TCHAR[nBytes*2+1];
	UINT i;
	for (i = 0; i < nBytes; i++)
	{
		lpsz[i*2] = (TCHAR)((pData[i] & 0x0F) + 'A'); //low nibble
		lpsz[i*2+1] = (TCHAR)(((pData[i] >> 4) & 0x0F) + 'A'); //high nibble
	}
	lpsz[i*2] = 0;

	ASSERT(m_strINIFileName.IsEmpty() == FALSE);

	BOOL bResult = WriteProfileString(lpszSection, lpszEntry, lpsz);
	delete[] lpsz;
	return bResult;
}

BOOL CXTRegistryManager::GetProfileRect(LPCTSTR lpszSection, LPCTSTR lpszEntry, CRect * rcResult)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);

	if (m_strINIFileName.IsEmpty())
	{
		if (!GetProfileInfo())
			return FALSE;

		HKEY hSecKey = GetSectionKey (lpszSection);
		if (hSecKey == NULL)
			return FALSE;

		DWORD dwType, dwCount;
		LONG lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			NULL, &dwCount);

		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_BINARY);
			lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
				(LPBYTE)rcResult, &dwCount);
		}
		::RegCloseKey(hSecKey);
		if (lResult == ERROR_SUCCESS)
		{
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		ASSERT(m_strINIFileName.IsEmpty() == FALSE);

		CString str = GetProfileString(lpszSection, lpszEntry, NULL);
		if (str.IsEmpty())
			return FALSE;

		ASSERT(str.GetLength()%2 == 0);
		int nLen = str.GetLength();

		LPBYTE pData = (LPBYTE) rcResult;

		pData = new BYTE[nLen/2];
		int i;
		for (i=0;i<nLen;i+=2)
		{
			(pData)[i/2] = (BYTE)
				(((str[i+1] - 'A') << 4) + (str[i] - 'A'));
		}

		return TRUE;
	}
}

BOOL CXTRegistryManager::WriteProfileSize(LPCTSTR lpszSection, LPCTSTR lpszEntry, CSize * pValue)
{
	ASSERT(lpszSection != NULL);
	if (m_strINIFileName.IsEmpty())
	{
		if (!GetProfileInfo())
			return FALSE;

		LONG lResult;
		HKEY hSecKey = GetSectionKey (lpszSection);
		if (hSecKey == NULL)
			return FALSE;
		lResult = ::RegSetValueEx(hSecKey, lpszEntry, NULL, REG_BINARY,
			(LPBYTE)pValue, sizeof(CSize));
		::RegCloseKey(hSecKey);
		return lResult == ERROR_SUCCESS;
	}

	LPBYTE pData = (LPBYTE) pValue;
	UINT nBytes = sizeof(CSize);
	LPTSTR lpsz = new TCHAR[nBytes*2+1];
	UINT i;
	for (i = 0; i < nBytes; i++)
	{
		lpsz[i*2] = (TCHAR)((pData[i] & 0x0F) + 'A'); //low nibble
		lpsz[i*2+1] = (TCHAR)(((pData[i] >> 4) & 0x0F) + 'A'); //high nibble
	}
	lpsz[i*2] = 0;

	ASSERT(m_strINIFileName.IsEmpty() == FALSE);

	BOOL bResult = WriteProfileString(lpszSection, lpszEntry, lpsz);
	delete[] lpsz;
	return bResult;
}

BOOL CXTRegistryManager::GetProfileSize(LPCTSTR lpszSection, LPCTSTR lpszEntry, CSize * szResult)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);

	if (m_strINIFileName.IsEmpty())
	{
		if (!GetProfileInfo())
			return FALSE;

		HKEY hSecKey = GetSectionKey (lpszSection);
		if (hSecKey == NULL)
			return FALSE;

		DWORD dwType, dwCount;
		LONG lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			NULL, &dwCount);

		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_BINARY);
			lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
				(LPBYTE)szResult, &dwCount);
		}
		::RegCloseKey(hSecKey);
		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_BINARY);
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		ASSERT(m_strINIFileName.IsEmpty() == FALSE);

		CString str = GetProfileString(lpszSection, lpszEntry, NULL);
		if (str.IsEmpty())
			return FALSE;

		ASSERT(str.GetLength()%2 == 0);
		int nLen = str.GetLength();

		LPBYTE pData = (LPBYTE) szResult;

		pData = new BYTE[nLen/2];
		int i;
		for (i=0;i<nLen;i+=2)
		{
			(pData)[i/2] = (BYTE)
				(((str[i+1] - 'A') << 4) + (str[i] - 'A'));
		}

		return TRUE;
	}
}

BOOL CXTRegistryManager::WriteProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double * pValue)
{
	ASSERT(lpszSection != NULL);
	if (m_strINIFileName.IsEmpty())
	{
		if (!GetProfileInfo())
			return FALSE;

		LONG lResult;
		HKEY hSecKey = GetSectionKey (lpszSection);
		if (hSecKey == NULL)
			return FALSE;
		lResult = ::RegSetValueEx(hSecKey, lpszEntry, NULL, REG_BINARY,
			(LPBYTE)pValue, sizeof(*pValue));
		::RegCloseKey(hSecKey);
		return lResult == ERROR_SUCCESS;
	}

	LPBYTE pData = (LPBYTE) pValue;
	UINT nBytes = sizeof(double);
	LPTSTR lpsz = new TCHAR[nBytes*2+1];
	UINT i;
	for (i = 0; i < nBytes; i++)
	{
		lpsz[i*2] = (TCHAR)((pData[i] & 0x0F) + 'A'); //low nibble
		lpsz[i*2+1] = (TCHAR)(((pData[i] >> 4) & 0x0F) + 'A'); //high nibble
	}
	lpsz[i*2] = 0;

	ASSERT(m_strINIFileName.IsEmpty() == FALSE);

	BOOL bResult = WriteProfileString(lpszSection, lpszEntry, lpsz);
	delete[] lpsz;
	return bResult;

}

BOOL CXTRegistryManager::GetProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double * dResult)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);

	if (m_strINIFileName.IsEmpty())
	{
		if (!GetProfileInfo())
			return FALSE;

		HKEY hSecKey = GetSectionKey (lpszSection);
		if (hSecKey == NULL)
			return FALSE;

		DWORD dwType, dwCount;
		LONG lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			NULL, &dwCount);

		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_BINARY);
			lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
				(LPBYTE)dResult, &dwCount);
		}

		::RegCloseKey(hSecKey);

		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_BINARY);
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		ASSERT(m_strINIFileName.IsEmpty() == FALSE);

		ASSERT(m_strINIFileName.IsEmpty() == FALSE);

		CString str = GetProfileString(lpszSection, lpszEntry, NULL);
		if (str.IsEmpty())
			return FALSE;

		ASSERT(str.GetLength()%2 == 0);
		int nLen = str.GetLength();

		LPBYTE pData = (LPBYTE) dResult;

		pData = new BYTE[nLen/2];
		int i;
		for (i=0;i<nLen;i+=2)
		{
			(pData)[i/2] = (BYTE)
				(((str[i+1] - 'A') << 4) + (str[i] - 'A'));
		}

		return TRUE;
	}
}

BOOL CXTRegistryManager::WriteProfileDword(LPCTSTR lpszSection, LPCTSTR lpszEntry, DWORD * pValue)
{
	ASSERT(lpszSection != NULL);
	if (m_strINIFileName.IsEmpty())
	{
		if (!GetProfileInfo())
			return FALSE;

		LONG lResult;
		HKEY hSecKey = GetSectionKey (lpszSection);
		if (hSecKey == NULL)
			return FALSE;
		lResult = ::RegSetValueEx(hSecKey, lpszEntry, NULL, REG_DWORD,
			(LPBYTE)pValue, sizeof(*pValue));
		::RegCloseKey(hSecKey);
		return lResult == ERROR_SUCCESS;
	}

	LPBYTE pData = (LPBYTE) pValue;
	UINT nBytes = sizeof(DWORD);
	LPTSTR lpsz = new TCHAR[nBytes*2+1];
	UINT i;
	for (i = 0; i < nBytes; i++)
	{
		lpsz[i*2] = (TCHAR)((pData[i] & 0x0F) + 'A'); //low nibble
		lpsz[i*2+1] = (TCHAR)(((pData[i] >> 4) & 0x0F) + 'A'); //high nibble
	}
	lpsz[i*2] = 0;

	ASSERT(m_strINIFileName.IsEmpty() == FALSE);

	BOOL bResult = WriteProfileString(lpszSection, lpszEntry, lpsz);
	delete[] lpsz;
	return bResult;
}

BOOL CXTRegistryManager::GetProfileDword(LPCTSTR lpszSection, LPCTSTR lpszEntry, DWORD * dwResult)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);

	if (m_strINIFileName.IsEmpty())
	{
		if (!GetProfileInfo())
			return FALSE;

		HKEY hSecKey = GetSectionKey (lpszSection);
		if (hSecKey == NULL)
			return FALSE;

		DWORD dwType, dwCount;
		LONG lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
			NULL, &dwCount);

		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_DWORD);
			lResult = ::RegQueryValueEx(hSecKey, (LPTSTR)lpszEntry, NULL, &dwType,
				(LPBYTE)dwResult, &dwCount);
		}

		::RegCloseKey(hSecKey);

		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_DWORD);
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		ASSERT(m_strINIFileName.IsEmpty() == FALSE);

		CString str = GetProfileString(lpszSection, lpszEntry, NULL);
		if (str.IsEmpty())
			return FALSE;

		ASSERT(str.GetLength()%2 == 0);
		int nLen = str.GetLength();

		LPBYTE pData = (LPBYTE) dwResult;

		pData = new BYTE[nLen/2];
		int i;
		for (i=0;i<nLen;i+=2)
		{
			(pData)[i/2] = (BYTE)
				(((str[i+1] - 'A') << 4) + (str[i] - 'A'));
		}

		return TRUE;
	}
}

BOOL CXTRegistryManager::WriteProfileColor(LPCTSTR lpszSection, LPCTSTR lpszEntry, COLORREF * pValue)
{
	return WriteProfileDword(lpszSection, lpszEntry, pValue);
}

BOOL CXTRegistryManager::GetProfileColor(LPCTSTR lpszSection, LPCTSTR lpszEntry, COLORREF * rgbResult)
{
	return GetProfileDword(lpszSection, lpszEntry, rgbResult);
}
