// XTPPropExchange.cpp: implementation of the CXTPPropExchange class.
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
#include "XTPPropExchange.h"
#include "XTPVC80Helpers.h"
#include "XTPVC50Helpers.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

enum VARTYPE_EX
{
	VT_EX_RECT = 100,
	VT_EX_SIZE = 101
};


//////////////////////////////////////////////////////////////////////////
// CXTPPropExchangeEnumerator

CXTPPropExchangeEnumerator::CXTPPropExchangeEnumerator(CXTPPropExchange* pContainer, LPCTSTR lpszSection)
{
	m_pContainer = pContainer;
	m_strSectionName = lpszSection;
	m_nCount = 0;
}

CXTPPropExchangeEnumerator::~CXTPPropExchangeEnumerator() 
{

}

POSITION CXTPPropExchangeEnumerator::GetPosition(DWORD dwCount /*= 0*/, BOOL bCompactMode /*= TRUE*/) 
{
	m_nCount = dwCount;
	
	if (!m_pContainer)
		return NULL;

	if (!bCompactMode)
	{
		PX_DWord(m_pContainer, _T("Count"), m_nCount, 0);
	}
	else
	{
		if (m_pContainer->IsStoring())	
			m_pContainer->WriteCount(m_nCount);
		else
			m_nCount = m_pContainer->ReadCount();		
	}

	return m_nCount == 0? NULL: (POSITION)1;
}

CXTPPropExchange* CXTPPropExchangeEnumerator::GetNext(POSITION& pos) 
{
	CString strSection;
	strSection.Format(_T("%s%i"), m_strSectionName, (DWORD)(DWORD_PTR)pos - 1);

	CXTPPropExchange* pSection = m_pContainer->GetSection(strSection);
	
	pos ++;		
	if ((DWORD)(DWORD_PTR)pos > m_nCount) pos = NULL;
	
	return pSection;
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPPropExchange::CXTPPropExchange()
{
	m_bLoading = FALSE;
	m_dwData = 0;
	m_nSchema = _XTP_SCHEMA_CURRENT;
	m_bChildSection = FALSE;
	m_bInitialized = FALSE;
	m_bEmptySection = FALSE;
	m_bUseDefaultOnLoad = TRUE;
}

CXTPPropExchange::~CXTPPropExchange()
{

}

void CXTPPropExchange::InitSection(CXTPPropExchange* pRootSection)
{
	ASSERT(m_bLoading == pRootSection->m_bLoading);

	m_bChildSection = TRUE;
	m_nSchema = pRootSection->m_nSchema;
	m_dwData = pRootSection->m_dwData;
	m_bEmptySection = pRootSection->m_bEmptySection;
	m_bUseDefaultOnLoad = pRootSection->m_bUseDefaultOnLoad;
}



void CXTPPropExchange::EmptySection()
{
	m_bEmptySection = TRUE;
}


DWORD CXTPPropExchange::GetSizeOfVarType(VARTYPE vt)
{
	switch (vt)
	{
	case VT_BOOL: return sizeof(BOOL);
	case VT_UI1: return sizeof(BYTE);
	case VT_I2: return 2;
	case VT_I4: return 4;
	case VT_R4:
		ASSERT(sizeof(float) == 4);
		return 4;
	case VT_R8:
		ASSERT(sizeof(double) == 8);
		return 8;
	case VT_EX_RECT: return sizeof(RECT);
	case VT_EX_SIZE: return sizeof(SIZE);
	case VT_CY: return sizeof(CURRENCY);
	case VT_BSTR:return sizeof(BSTR);
	}

	return 0;
}

void CXTPPropExchange::Write (LPCTSTR pszPropName, const void* lpBuf, UINT nCount)
{
	ASSERT(IsStoring());
	ASSERT(lpBuf != NULL);
	ExchangeBlobProp(pszPropName, (BYTE*&)lpBuf, (DWORD&)nCount);
}

UINT CXTPPropExchange::Read (LPCTSTR pszPropName, void* lpBuf, UINT nCount)
{
	ASSERT(IsLoading());
	ASSERT(lpBuf != NULL);
	if(!ExchangeBlobProp(pszPropName, (BYTE*&)lpBuf, (DWORD&)nCount))
		return 0;

	return nCount;
}

void CXTPPropExchange::PreformatString(CString& strValue)
{
	if (m_bLoading)
	{
		if (strValue.Find(_T('\\')) == -1)
			return;

		REPLACE_S(strValue, _T("\\\\"), _T("\001\001"));
		REPLACE_S(strValue, _T("\\n"), _T("\n"));
		REPLACE_S(strValue, _T("\\r"), _T("\r"));
		REPLACE_S(strValue, _T("\\t"), _T("\t"));
		REPLACE_S(strValue, _T("\001\001"), _T("\\"));
	}
	else
	{
		REPLACE_S(strValue, _T("\\"), _T("\\\\"));
		REPLACE_S(strValue, _T("\n"), _T("\\n"));
		REPLACE_S(strValue, _T("\r"), _T("\\r"));
		REPLACE_S(strValue, _T("\t"), _T("\\t"));
	}
}

BOOL CXTPPropExchange::ExchangePropStringHelper(CString& strValue, VARTYPE vtProp,
	void* pvProp, const void* pvDefault)
{
	if (m_bLoading)
	{
		switch (vtProp)
		{
		case VT_UI1:
		case VT_I2:
		case VT_I4:
		case VT_BOOL:
			{
				int dwValue = _ttoi(strValue);
				return CopyPropValue(vtProp, pvProp, &dwValue);
			}
		case VT_LPSTR:
		case VT_BSTR:
		case VT_EX_SIZE:
		case VT_EX_RECT:
			{
				if (vtProp == VT_EX_SIZE)
				{
					SIZE sz;
					if (SCANF_S(strValue, _T("%i, %i"), &sz.cx, &sz.cy) != 2)
						return CopyPropValue(vtProp, pvProp, pvDefault);

					*((LPSIZE)pvProp) = sz;
				}
				else if (vtProp == VT_EX_RECT)
				{
					RECT rc;
					if (SCANF_S(strValue, _T("%i, %i, %i, %i"), &rc.left, &rc.top, &rc.right, &rc.bottom) != 4)
						return CopyPropValue(vtProp, pvProp, pvDefault);

					*((LPRECT)pvProp) = rc;
				}
				else
				{
					PreformatString(strValue);
					*(CString*)pvProp = strValue;
				}
				return TRUE;
			}
		}

	}
	else
	{
		switch (vtProp)
		{
		case VT_UI1:
		case VT_I2:
		case VT_I4:
		case VT_BOOL:
			{
				int nValue = 0;
				memcpy(&nValue, pvProp, GetSizeOfVarType(vtProp));

				strValue.Format(_T("%i"), nValue);

			}
			return TRUE;

		case VT_LPSTR:
		case VT_BSTR:
			{
				strValue = *(CString*)pvProp;
				PreformatString(strValue);
			}
			return TRUE;

		case VT_R4:
		case VT_R8:
			ASSERT(FALSE);
			return FALSE;

		case VT_EX_SIZE:
			{
				strValue.Format(_T("%i, %i"), ((LPSIZE)pvProp)->cx, ((LPSIZE)pvProp)->cy);
			}
			return TRUE;

		case VT_EX_RECT:
			{
				strValue.Format(_T("%i, %i, %i, %i"), ((LPRECT)pvProp)->left, ((LPRECT)pvProp)->top, ((LPRECT)pvProp)->right, ((LPRECT)pvProp)->bottom);
			}
			return TRUE;
		}
	}
	return FALSE;		
}



BOOL CXTPPropExchange::ExchangeBlobProp(LPCTSTR pszPropName, BYTE*& pData, DWORD& nBytes)
{
	if (IsStoring())
	{
		CString str;
		if (nBytes > 0)
		{
			LPTSTR lpsz = new TCHAR[nBytes*2+1];
			for (UINT i = 0; i < nBytes; i++)
			{
				lpsz[i*2] = (TCHAR)((pData[i] & 0x0F) + 'A'); //low nibble
				lpsz[i*2+1] = (TCHAR)(((pData[i] >> 4) & 0x0F) + 'A'); //high nibble
			}
			lpsz[i*2] = 0;

			str = lpsz;
			delete[] lpsz;
		}
		PX_String(this, pszPropName, str, _T(""));
	}
	else
	{
		CString str;
		PX_String(this, pszPropName, str, _T(""));

		if (str.GetLength() >0)
		{
			int nLen = str.GetLength();
			if ((nLen == 0) || (nLen % 2 != 0) )
				return FALSE;

			DWORD nCount = nLen/2;
			if (pData == NULL)
			{
				pData = (BYTE*)malloc(nCount);
				nBytes = nCount;
			}

			if (nBytes < nCount)
				return FALSE;

			for (int i = 0; i < nLen; i += 2)
			{
				(pData)[i/2] = (BYTE)
					(((str[i+1] - 'A') << 4) + (str[i] - 'A'));
			}
		}
	}
	return TRUE;

}


BOOL CXTPPropExchange::IsSamePropValue(VARTYPE vtProp, const void* pv1, const void* pv2)
{
	if (pv1 == pv2)
		return TRUE;

	if ((pv1 == NULL) || (pv2 == NULL))
		return FALSE;

	BOOL bSame = FALSE;

	switch (vtProp)
	{
	case VT_BSTR:
		bSame = ((CString*)pv1)->Compare(*(CString*)pv2) == 0;
		break;
	case VT_LPSTR:
		bSame = ((CString*)pv1)->Compare((LPCTSTR)pv2) == 0;
		break;

	case VT_BOOL:
	case VT_UI1:
	case VT_I2:
	case VT_I4:
	case VT_CY:
	case VT_R4:
	case VT_R8:
	case VT_EX_RECT:
	case VT_EX_SIZE:
		bSame = memcmp(pv1, pv2, GetSizeOfVarType(vtProp)) == 0;
		break;
	}

	return bSame;
}

BOOL CXTPPropExchange::CopyPropValue(VARTYPE vtProp, void* pvDest, const void * pvSrc)
{
	ASSERT(AfxIsValidAddress(pvDest, 1));

	if (pvSrc != NULL)
	{
		ASSERT(AfxIsValidAddress(pvSrc, 1, FALSE));

		switch (vtProp)
		{
		case VT_UI1:
			*(BYTE*)pvDest = *(BYTE*)pvSrc;
			break;
		case VT_I2:
			*(short*)pvDest = *(short*)pvSrc;
			break;
		case VT_I4:
			*(long*)pvDest = *(long*)pvSrc;
			break;
		case VT_BOOL:
			*(BOOL*)pvDest = *(BOOL*)pvSrc;
			break;
		case VT_BSTR:
			*(CString*)pvDest = *(CString*)pvSrc;
			break;
		case VT_LPSTR:
			*(CString*)pvDest = (LPCTSTR)pvSrc;
			break;
		case VT_CY:
			*(CY*)pvDest = *(CY*)pvSrc;
			break;
		case VT_R4:
			*(float*)pvDest = *(float*)pvSrc;
			break;
		case VT_R8:
			*(double*)pvDest = *(double*)pvSrc;
			break;
		case VT_EX_RECT:
			*(RECT*)pvDest = *(RECT*)pvSrc;
		case VT_EX_SIZE:
			*(SIZE*)pvDest = *(SIZE*)pvSrc;
		default:
			return FALSE;
		}
	}
	return pvSrc != NULL;
}

CRuntimeClass* CXTPPropExchange::FindRuntimeClass(LPCSTR szClassName)
{

	CRuntimeClass* pClass;
	// search app specific classes
	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	AfxLockGlobals(CRIT_RUNTIMECLASSLIST);
	for (pClass = pModuleState->m_classList; pClass != NULL;
	pClass = pClass->m_pNextClass)
	{
		if (lstrcmpA(szClassName, pClass->m_lpszClassName) == 0)
		{
			AfxUnlockGlobals(CRIT_RUNTIMECLASSLIST);
			return pClass;
		}
	}
	AfxUnlockGlobals(CRIT_RUNTIMECLASSLIST);
#ifdef _AFXDLL
	// search classes in shared DLLs
	AfxLockGlobals(CRIT_DYNLINKLIST);
	for (CDynLinkLibrary* pDLL = pModuleState->m_libraryList; pDLL != NULL;
	pDLL = pDLL->m_pNextDLL)
	{
		for (pClass = pDLL->m_classList; pClass != NULL;
		pClass = pClass->m_pNextClass)
		{
			if (lstrcmpA(szClassName, pClass->m_lpszClassName) == 0)
			{
				AfxUnlockGlobals(CRIT_DYNLINKLIST);
				return pClass;
			}
		}
	}
	AfxUnlockGlobals(CRIT_DYNLINKLIST);
#endif
	ASSERT(FALSE);
	return 0;
}

BOOL CXTPPropExchange::ExchangeRuntimeClass(LPCTSTR pszPropName, CRuntimeClass*& pClass, CRuntimeClass* pDefaultClass)
{
	USES_CONVERSION;

	CString strClassName;

	if (!IsLoading())
	{
		strClassName = CString(pClass->m_lpszClassName);
		PX_String(this, pszPropName, strClassName, CString(pDefaultClass->m_lpszClassName));
	}
	else
	{
		if (!PX_String(this, pszPropName, strClassName))
		{
			pClass =  pDefaultClass;
			return TRUE;
		}

		pClass = FindRuntimeClass(T2A((LPTSTR)(LPCTSTR)strClassName));
		if (!pClass)
			return FALSE;
	}
	return TRUE;
}

BOOL CXTPPropExchange::ExchangeObjectInstance(CObject*& pObject, CRuntimeClass* pObjectRuntime)
{
	if (!IsLoading())
	{
		CRuntimeClass* pClass = pObject->GetRuntimeClass();
		PX_RuntimeClass(this, _T("Class"), pClass, pObjectRuntime);
	}
	else
	{
		CRuntimeClass* pClass = NULL;
		PX_RuntimeClass(this, _T("Class"), pClass, pObjectRuntime);
		if (!pClass)
			return FALSE;

		pObject = (CObject*)AfxDynamicDownCast(pObjectRuntime, pClass->CreateObject());
		if (!pObject)
			return FALSE;
	}
	return TRUE;
}
void CXTPPropExchange::WriteCount (DWORD dwCount)
{
	PX_DWord(this, _T("Count"), dwCount, 0);
}

DWORD CXTPPropExchange::ReadCount()
{
	DWORD nCount = 0;
	PX_DWord(this, _T("Count"), nCount, 0);
	return nCount;
}

CArchive* CXTPPropExchange::GetArchive(LPCTSTR pszPropName)
{
	if (IsStoring())
	{

		CMemFile* pMemFile = new CMemFile();
		CArchive* pArchive = new CArchive(pMemFile, CArchive::store);

		return pArchive;
	}

	DWORD nBytes = 0;
	BYTE* pData = NULL;
	PX_Blob(this, pszPropName, pData, nBytes);

	if (nBytes == 0)
		return NULL;

	CMemFile* pMemFile = new CMemFile(pData, nBytes);
	CArchive* pArchive = new CArchive(pMemFile, CArchive::load);

	return pArchive;
}

void CXTPPropExchange::ReleaseArchive(LPCTSTR pszPropName, CArchive* pArchive)
{
	CMemFile* pFile = DYNAMIC_DOWNCAST(CMemFile, pArchive->GetFile());
	ASSERT(pFile);

	BYTE* pData = NULL;
	if (IsStoring())
	{
		pArchive->Flush();

		DWORD nBytes = (DWORD)pFile->GetPosition();
		pData = pFile->Detach();
		PX_Blob(this, pszPropName, pData, nBytes);

	}
	else
	{
		pData = pFile->Detach();
	}

	pArchive->Abort();
	pFile->Close();
	free(pData);

	delete pFile;
	delete pArchive;
}

void CXTPPropExchange::ExchangeSchema()
{
	PX_Long(this, _T("Schema"), (long&)m_nSchema, _XTP_SCHEMA_CURRENT);
}


CXTPPropExchangeEnumerator* CXTPPropExchange::GetEnumerator(LPCTSTR lpszSection)
{
	return new CXTPPropExchangeEnumerator(this, lpszSection);
}

//////////////////////////////////////////////////////////////////////////
// CXTPPropExchangeArchive

CXTPPropExchangeArchive::CXTPPropExchangeArchive(CArchive& ar) :
	m_ar((CSelfArchive&)ar)
{
	ASSERT_POINTER(&ar, CArchive);
	m_bLoading = m_ar.IsLoading();
}

CXTPPropExchange* CXTPPropExchangeArchive::GetSection(LPCTSTR /*pszSection*/)
{
	CXTPPropExchangeArchive* pSection = new CXTPPropExchangeArchive(m_ar);
	pSection->InitSection(this);
	return pSection;
}


BOOL CXTPPropExchangeArchive::ExchangeRuntimeClass(LPCTSTR /*pszPropName*/, CRuntimeClass*& pClass, CRuntimeClass* pDefaultClass)
{
	if (!OnBeforeExchange())
		return FALSE;

	if (!IsLoading())
	{
		m_ar.WriteClass(pClass);
	}
	else
	{
		pClass = m_ar.ReadClass(pDefaultClass, &m_nSchema);

		if (!pClass)
			return FALSE;
	}
	return TRUE;
}

BOOL CXTPPropExchangeArchive::ExchangeObjectInstance(CObject*& pObject, CRuntimeClass* pObjectRuntime)
{
	if (!CXTPPropExchange::ExchangeObjectInstance(pObject, pObjectRuntime))
		return FALSE;

	if (IsLoading())
	{
		m_ar.CheckCount();
		m_ar.m_pLoadArray->InsertAt(m_ar.m_nMapCount++, pObject);
	}
	else
	{
		m_ar.MapObject(NULL);
		m_ar.CheckCount();
		m_ar.m_pStoreMap->SetAt(pObject, (LPVOID)(DWORD_PTR)m_ar.m_nMapCount++);

	}

	return TRUE;
}

BOOL CXTPPropExchangeArchive::ExchangeProp(LPCTSTR pszPropName, VARTYPE vtProp,
		void* pvProp, const void* pvDefault)
{
	if (!OnBeforeExchange())
		return FALSE;

	UNUSED(pszPropName);     // unused in release builds
	UNUSED(pvDefault);       // unused in release builds

	ASSERT(AfxIsValidString(pszPropName));
	ASSERT(AfxIsValidAddress(pvProp, 1, FALSE));
	ASSERT((pvDefault == NULL) || AfxIsValidAddress(pvDefault, 1, FALSE));

	if (m_bLoading)
	{
		switch (vtProp)
		{
		case VT_UI1:
			m_ar >> *(BYTE*)pvProp;
			break;
		case VT_I2:
			m_ar >> *(WORD*)pvProp;
			break;
		case VT_I4:
			m_ar >> *(long*)pvProp;
			break;
		case VT_BOOL:
			m_ar >> *(BOOL*)pvProp;
			break;
		case VT_LPSTR:
		case VT_BSTR:
			m_ar >> *(CString*)pvProp;
			break;
		case VT_CY:
			m_ar >> ((CY*)pvProp)->Lo;
			m_ar >> ((CY*)pvProp)->Hi;
			break;
		case VT_R4:
			m_ar >> *(float*)pvProp;
			break;
		case VT_R8:
			m_ar >> *(double*)pvProp;
			break;
		case VT_EX_RECT:
			m_ar >> *(RECT*)pvProp;
			break;
		case VT_EX_SIZE:
			m_ar >> *(SIZE*)pvProp;
			break;
		}
	}
	else
	{
		switch (vtProp)
		{
		case VT_UI1:
			m_ar << *(BYTE*)pvProp;
			break;
		case VT_I2:
			m_ar << *(WORD*)pvProp;
			break;
		case VT_I4:
			m_ar << *(long*)pvProp;
			break;
		case VT_BOOL:
			m_ar << *(BOOL*)pvProp;
			break;
		case VT_LPSTR:
		case VT_BSTR:
			m_ar << *(CString*)pvProp;
			break;
		case VT_CY:
			m_ar << ((CY*)pvProp)->Lo;
			m_ar << ((CY*)pvProp)->Hi;
			break;
		case VT_R4:
			m_ar << *(float*)pvProp;
			break;
		case VT_R8:
			m_ar << *(double*)pvProp;
			break;
		case VT_EX_RECT:
			m_ar << *(RECT*)pvProp;
			break;
		case VT_EX_SIZE:
			m_ar << *(SIZE*)pvProp;
			break;
		}
	}

	return TRUE;
}

void CXTPPropExchangeArchive::WriteCount (DWORD dwCount)
{
	m_ar.WriteCount(dwCount);
}
DWORD CXTPPropExchangeArchive::ReadCount()
{
	return (DWORD)m_ar.ReadCount();
}
void CXTPPropExchangeArchive::Write (LPCTSTR /*pszPropName*/, const void* lpBuf, UINT nCount)
{
	ASSERT(lpBuf);
	m_ar.Write(lpBuf, nCount);
}

UINT CXTPPropExchangeArchive::Read (LPCTSTR /*pszPropName*/, void* lpBuf, UINT nCount)
{
	ASSERT(lpBuf);
	return m_ar.Read(lpBuf, nCount);
}

BOOL CXTPPropExchangeArchive::ExchangeBlobProp(LPCTSTR /*pszPropName*/, BYTE*& pData, DWORD& nBytes)
{
	if (!OnBeforeExchange())
		return FALSE;

	if (IsStoring())
	{
		WriteCount(nBytes);
		m_ar.Write(pData, nBytes);
	}
	else
	{
		DWORD nCount = ReadCount();

		if (pData == NULL)
		{
			pData = (BYTE*)malloc(nCount);
			nBytes = nCount;
		}

		if (nBytes < nCount)
			return FALSE;

		return m_ar.Read(pData, nCount) == nCount;
	}

	return TRUE;
}

CArchive* CXTPPropExchangeArchive::GetArchive(LPCTSTR /*pszPropName*/)
{
	return &m_ar;
}

void CXTPPropExchangeArchive::ReleaseArchive(LPCTSTR /*pszPropName*/, CArchive* /*pArchive*/)
{

}


//////////////////////////////////////////////////////////////////////////
// CXTPPropExchangeRegistry

CXTPPropExchangeRegistry::CXTPPropExchangeRegistry(BOOL bLoading, HKEY hParentKey, LPCTSTR lpszSection)
{
	ASSERT(lpszSection != NULL);

	m_bLoading = bLoading;
	m_hParentKey = hParentKey;
	m_strSectionName = lpszSection;
	m_hSectionKey = NULL;
}

CXTPPropExchangeRegistry::~CXTPPropExchangeRegistry()
{
	if (m_hSectionKey)
	{
		RegCloseKey(m_hSectionKey);
	}
}

CXTPPropExchange* CXTPPropExchangeRegistry::GetSection(LPCTSTR lpszSection)
{
	OnBeforeExchange();

	CXTPPropExchangeRegistry* pSection = new CXTPPropExchangeRegistry(IsLoading(), m_hSectionKey, lpszSection);
	pSection->InitSection(this);
	return pSection;
}

BOOL CXTPPropExchangeRegistry::OnBeforeExchange()
{
	if (m_hParentKey == NULL)
		return FALSE;

	if (!m_bInitialized)
	{
		ASSERT(m_hSectionKey == 0);

		VERIFY(RegCreateKeyEx(m_hParentKey, m_strSectionName, 0, REG_NONE, REG_OPTION_NON_VOLATILE, m_bLoading? KEY_READ: KEY_WRITE, NULL, &m_hSectionKey, NULL) == ERROR_SUCCESS);

		m_bInitialized = TRUE;
	}
	return m_hSectionKey != NULL;
}

void CXTPPropExchangeRegistry::EmptySection()
{
	CXTPPropExchange::EmptySection();

	if (m_hSectionKey)
	{
		RegCloseKey(m_hSectionKey);
		m_hSectionKey = 0;
	}

	AfxDelRegTreeHelper(m_hParentKey, m_strSectionName);
	m_bInitialized = FALSE;
}


BOOL CXTPPropExchangeRegistry::ExchangeProp(LPCTSTR pszPropName, VARTYPE vtProp,
				void* pvProp, const void* pvDefault /*= NULL*/)
{
	if (m_bLoading)
	{
		if (!m_bUseDefaultOnLoad)
			pvDefault = NULL;

		if (!OnBeforeExchange())
			return CopyPropValue(vtProp, pvProp, pvDefault);

		ASSERT(m_hSectionKey);

		switch (vtProp)
		{
		case VT_UI1:
		case VT_I2:
		case VT_I4:
		case VT_BOOL:
			{

				DWORD dwValue;
				DWORD dwType;
				DWORD dwCount = sizeof(DWORD);
				LONG lResult = RegQueryValueEx(m_hSectionKey, (LPTSTR)pszPropName, NULL, &dwType,
					(LPBYTE)&dwValue, &dwCount);

				if (lResult == ERROR_SUCCESS)
				{
					ASSERT(dwType == REG_DWORD);
					ASSERT(dwCount == sizeof(DWORD));

					return CopyPropValue(vtProp, pvProp, &dwValue);

				}
				else
				{
					return CopyPropValue(vtProp, pvProp, pvDefault);
				}
			}
		case VT_LPSTR:
		case VT_BSTR:
		case VT_EX_SIZE:
		case VT_EX_RECT:
			{
				CString strValue;
				DWORD dwType, dwCount;
				LONG lResult = RegQueryValueEx(m_hSectionKey, (LPTSTR)pszPropName, NULL, &dwType,
					NULL, &dwCount);
				if (lResult == ERROR_SUCCESS)
				{
					ASSERT(dwType == REG_SZ);
					lResult = RegQueryValueEx(m_hSectionKey, (LPTSTR)pszPropName, NULL, &dwType,
						(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
					strValue.ReleaseBuffer();
				}
				if (lResult == ERROR_SUCCESS)
				{
					ASSERT(dwType == REG_SZ);

					if (vtProp == VT_EX_SIZE)
					{
						SIZE sz;
						if (SCANF_S(strValue, _T("%i, %i"), &sz.cx, &sz.cy) != 2)
							return CopyPropValue(vtProp, pvProp, pvDefault);

						*((LPSIZE)pvProp) = sz;
					}
					else if (vtProp == VT_EX_RECT)
					{
						RECT rc;
						if (SCANF_S(strValue, _T("%i, %i, %i, %i"), &rc.left, &rc.top, &rc.right, &rc.bottom) != 4)
							return CopyPropValue(vtProp, pvProp, pvDefault);

						*((LPRECT)pvProp) = rc;
					}
					else
					{
						*(CString*)pvProp = strValue;
					}

				}
				else
				{
					return CopyPropValue(vtProp, pvProp, pvDefault);
				}
				return TRUE;
			}
		}
	}
	else
	{
		if (!OnBeforeExchange())
			return FALSE;

		if (IsSamePropValue(vtProp, pvProp, pvDefault))
		{
			if (m_hSectionKey && !m_bEmptySection)
				RegDeleteValue(m_hSectionKey, pszPropName);
			return TRUE;
		}

		LONG lResult = -1;

		switch (vtProp)
		{
		case VT_UI1:
		case VT_I2:
		case VT_I4:
		case VT_BOOL:
			lResult = RegSetValueEx(m_hSectionKey, pszPropName, NULL, REG_DWORD, (LPBYTE)pvProp, GetSizeOfVarType(vtProp));
			break;

		case VT_LPSTR:
		case VT_BSTR:
			{
				CString& strValue = *(CString*)pvProp;
				lResult = RegSetValueEx(m_hSectionKey, pszPropName, NULL, REG_SZ,
					(LPBYTE)(LPCTSTR)strValue, (strValue.GetLength() + 1) * sizeof(TCHAR));
			}
			break;

		case VT_R4:
		case VT_R8:
			lResult = RegSetValueEx(m_hSectionKey, pszPropName, NULL, REG_BINARY, (LPBYTE)pvProp, GetSizeOfVarType(vtProp));
			break;

		case VT_EX_SIZE:
			{
				CString strValue;
				strValue.Format(_T("%i, %i"), ((LPSIZE)pvProp)->cx, ((LPSIZE)pvProp)->cy);
				lResult = RegSetValueEx(m_hSectionKey, pszPropName, NULL, REG_SZ,
					(LPBYTE)(LPCTSTR)strValue, (strValue.GetLength() + 1) * sizeof(TCHAR));
			}
			break;

		case VT_EX_RECT:
				CString strValue;
				strValue.Format(_T("%i, %i, %i, %i"), ((LPRECT)pvProp)->left, ((LPRECT)pvProp)->top, ((LPRECT)pvProp)->right, ((LPRECT)pvProp)->bottom);
				lResult = RegSetValueEx(m_hSectionKey, pszPropName, NULL, REG_SZ,
					(LPBYTE)(LPCTSTR)strValue, (strValue.GetLength() + 1) * sizeof(TCHAR));
			break;

		}

		return lResult == ERROR_SUCCESS;
	}

	return FALSE;
}

BOOL CXTPPropExchangeRegistry::ExchangeBlobProp(LPCTSTR pszPropName, BYTE*& pData, DWORD& nBytes)
{
	if (!OnBeforeExchange())
		return FALSE;

	ASSERT(m_hSectionKey != 0);

	if (IsStoring())
	{
		LONG lResult = ::RegSetValueEx(m_hSectionKey, pszPropName, NULL, REG_BINARY,
			pData, nBytes);
		return lResult == ERROR_SUCCESS;

	}
	else
	{
		DWORD dwType, dwCount;
		LONG lResult = RegQueryValueEx(m_hSectionKey, (LPTSTR)pszPropName, NULL, &dwType,
			NULL, &dwCount);

		if (lResult != ERROR_SUCCESS)
			return FALSE;

		ASSERT(dwType == REG_BINARY);


		if (pData == NULL)
		{
			pData = (BYTE*)malloc(dwCount);
			nBytes = dwCount;
		}

		if (nBytes < dwCount)
			return FALSE;

		lResult = RegQueryValueEx(m_hSectionKey, (LPTSTR)pszPropName, NULL, &dwType,
				(LPBYTE)pData, &dwCount);
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// CXTPPropExchangeXMLNode

#ifndef _XTP_EXCLUDE_XML

CXTPPropExchangeXMLNode::CXTPPropExchangeXMLNode(BOOL bLoading, CXTPDOMNodePtr xmlParentNode, LPCTSTR lpszSection)
{
	m_bLoading = bLoading;
	m_strSectionName = lpszSection;
	m_xmlParentNode = xmlParentNode;
	m_xmlSectionNode = NULL;
	m_bCompactMode = TRUE;

	if (xmlParentNode)
	{
		xmlParentNode->get_ownerDocument(&m_xmlDocPtr);

		if (m_xmlDocPtr == 0)
		{
			m_xmlDocPtr = xmlParentNode;
		}
	}
}

BOOL CXTPPropExchangeXMLNode::CreateDocumentInstance()
{
	if (m_xmlDocPtr == NULL)
	{
		if (!SUCCEEDED(m_xmlDocPtr.CreateInstance(CLSID_XTPDOMDocument)))
		{
			TRACE(_T("Warning: CreateDocumentInstance failed in CXTPPropExchangeXMLNode --\n"));
			TRACE(_T("\tperhaps AfxOleInit() has not been called, or msxml3.dll not found."));
			
			return FALSE;
		}
	}
	return TRUE;

}

BOOL CXTPPropExchangeXMLNode::LoadFromString(LPCTSTR lpszState)
{
	return LoadFromString((BSTR)_bstr_t(lpszState));
}

BOOL CXTPPropExchangeXMLNode::LoadFromString(BSTR lpszState)
{
	if (!CreateDocumentInstance())
		return FALSE;

	ASSERT(m_xmlDocPtr != 0);

	VARIANT_BOOL bResult;
	HRESULT hr = m_xmlDocPtr->loadXML(lpszState, &bResult);
	if (FAILED(hr) || bResult == VARIANT_FALSE)
		return FALSE;

	m_xmlParentNode = m_xmlDocPtr;

	return TRUE;
}


BOOL CXTPPropExchangeXMLNode::LoadFromFile(CString strFileName)
{
	if (ACCESS_S(strFileName, 0) == -1)
		return FALSE;

	if (!CreateDocumentInstance())
		return FALSE;

	ASSERT(m_xmlDocPtr != 0);

	VARIANT_BOOL bResult;
	HRESULT hr = m_xmlDocPtr->load(COleVariant(strFileName), &bResult);
	if (FAILED(hr) || bResult == VARIANT_FALSE)
	{
#ifdef _DEBUG
		XTPXML::IXMLDOMParseErrorPtr parseError = 0;
		if (FAILED(m_xmlDocPtr->get_parseError(&parseError)))
			return FALSE;
		
		BSTR bstrReason = 0;
		if (FAILED(parseError->get_reason(&bstrReason)))
			return FALSE;
		
		CString strReason(bstrReason);
		TRACE(_T("Warning: CXTPPropExchangeXMLNode::LoadFromFile failed. ParseError.Reason = %s"), (LPCTSTR)strReason);
		
		SysFreeString(bstrReason);		
#endif
		return FALSE;
	}

	m_xmlParentNode = m_xmlDocPtr;

	return TRUE;
}

CString CXTPPropExchangeXMLNode::GetXML(BOOL bFormatted)
{
	const LPCTSTR lpszCrLf = _T("\r\n");

#define ADDINDENT(str, i, strOffset)\
	INSERT_S(str, i, lpszCrLf);\
	INSERT_S(str, i + 2, strOffset);\
	i += strOffset.GetLength() + 2;

	CString str;
	if (!OnBeforeExchange())
		return str;

	ASSERT(m_xmlDocPtr != 0);

	BSTR bstrXML = 0;

	if (FAILED(m_xmlDocPtr->get_xml(&bstrXML)))
		return str;

	str = bstrXML;
	SysFreeString(bstrXML);

	if (bFormatted)
	{
		CString strOffset = _T('\t');

		for (int i = 1; i < str.GetLength(); i++)
		{
			if (((str[i] == _T('>')) && (str[i - 1] == _T('/'))) || ((str[i] == _T('<')) && (str[i + 1] == _T('/'))))
			{
				DELETE_S(strOffset, strOffset.GetLength() - 1);
				if (str[i] == _T('<') && str[i - 1] == _T('>'))
				{
					ADDINDENT(str, i, strOffset);
				}
			}
			else if (str[i] == _T('<'))
			{
				ADDINDENT(str, i, strOffset);
				strOffset += _T('\t');

			}
		}
	}

	return str;

}
void CXTPPropExchangeXMLNode::SaveToFile(CString strFileName, BOOL bFormatted)
{
#define WRITEINDENT(str, i, nBegin, nOffset)\
	file.WriteString(str.Mid(nBegin, nIndex - nBegin));\
	file.WriteString(_T("\n"));\
	for (int j = 0; j < nOffset; j++) file.WriteString(_T("\t"));\
	nBegin = nIndex;


	CString str;
	if (!OnBeforeExchange())
		return;

	ASSERT(m_xmlDocPtr != 0);

	if (!bFormatted)
	{
		m_xmlDocPtr->save(COleVariant(strFileName));
	}
	else
	{
		BSTR bstrXML = 0;

		if (FAILED(m_xmlDocPtr->get_xml(&bstrXML)))
			return;

		str = bstrXML;
		SysFreeString(bstrXML);

		CStdioFile file(strFileName, CFile::modeCreate|CFile::modeWrite);

		int nBegin = 0, nOffset = 1, nIndex = 1, nLength = str.GetLength();
		if (nLength > 2 && str[nLength - 2] == '\r')
			nLength -= 2;

		for (; nIndex < nLength; nIndex++)
		{
			if (((str[nIndex] == _T('>')) && (str[nIndex - 1] == _T('/'))) || ((str[nIndex] == _T('<')) && (str[nIndex + 1] == _T('/'))))
			{
				nOffset --;
				if (str[nIndex] == _T('<') && str[nIndex - 1] == _T('>'))
				{
					WRITEINDENT(str, i, nBegin, nOffset);
				}
			}
			else if (str[nIndex] == _T('<'))
			{
				WRITEINDENT(str, i, nBegin, nOffset);
				nOffset ++;
			}
		}
		file.WriteString(str.Mid(nBegin, nIndex - nBegin));
	}

}

CXTPPropExchange* CXTPPropExchangeXMLNode::GetSection(LPCTSTR lpszSection)
{
	OnBeforeExchange();

	CXTPPropExchangeXMLNode* pSection = new CXTPPropExchangeXMLNode(IsLoading(), m_xmlSectionNode, lpszSection);
	pSection->m_bCompactMode = m_bCompactMode;
	pSection->InitSection(this);
	return pSection;
}

BOOL CXTPPropExchangeXMLNode::OnBeforeExchange()
{
	if (!m_bInitialized)
	{
		m_bInitialized = TRUE;

		if (m_xmlDocPtr == NULL && IsRootSection())
		{
			ASSERT(m_xmlParentNode == 0);

			if (!CreateDocumentInstance())
				return FALSE;

			if (m_xmlDocPtr == 0)
				return FALSE;

			m_xmlParentNode = m_xmlDocPtr;
			ASSERT(m_xmlParentNode != 0);
		}

		if ((m_xmlDocPtr == NULL) || (m_xmlParentNode == NULL))
			return FALSE;

		ASSERT(m_xmlSectionNode == 0);

		m_xmlSectionNode = GetProfileNode(m_strSectionName);

		if (m_xmlSectionNode != NULL && IsRootSection())
		{
			PX_Bool(this, _T("CompactMode"), m_bCompactMode, FALSE);
		}
	}

	if ((m_xmlDocPtr == NULL) || (m_xmlParentNode == NULL))
		return FALSE;

	return m_xmlSectionNode != NULL;
}

void CXTPPropExchangeXMLNode::EmptySection()
{
	CXTPPropExchange::EmptySection();

	OnBeforeExchange();

	if (m_xmlSectionNode != 0 && m_xmlParentNode != 0)
	{
		m_xmlParentNode->removeChild(m_xmlSectionNode, NULL);
		m_xmlSectionNode = 0;
		m_bInitialized = FALSE;
	}
}

void CXTPPropExchangeXMLNode::SetCompactMode(BOOL bCompactMode)
{
	m_bCompactMode = bCompactMode;
}


CXTPDOMNodePtr CXTPPropExchangeXMLNode::GetProfileNode(CString strSection)
{
	ASSERT(m_xmlParentNode != 0);
	CXTPDOMNodePtr xmlNodePtr;
	CXTPDOMElementPtr xmlElementPtr;

	HRESULT hr = m_xmlParentNode->selectSingleNode(_bstr_t(strSection), &xmlNodePtr);

	if (FAILED(hr) || xmlNodePtr == NULL)
	{
		m_xmlDocPtr->createElement(_bstr_t(strSection), &xmlElementPtr);
		ASSERT(xmlElementPtr != 0);
		if (xmlElementPtr)
		{
			m_xmlParentNode->appendChild(xmlElementPtr, &xmlNodePtr);
		}
	}

	return xmlNodePtr;
}

CXTPDOMNodePtr CXTPPropExchangeXMLNode::GetEntryNode(LPCTSTR lpszEntry, BOOL bCreate)
{
	ASSERT(m_xmlSectionNode != 0);

	CXTPDOMNodePtr xmlNodeEntryPtr;

	if (m_bCompactMode)
	{
		XTPXML::IXMLDOMNamedNodeMapPtr map;
		m_xmlSectionNode->get_attributes(&map);
		HRESULT hr = map->getNamedItem(_bstr_t(lpszEntry), &xmlNodeEntryPtr);

		if (FAILED(hr) || xmlNodeEntryPtr == NULL)
		{
			if (bCreate)
			{
				XTPXML::IXMLDOMAttributePtr xmlElementPtr;
				m_xmlDocPtr->createAttribute(_bstr_t(lpszEntry), &xmlElementPtr);
				if (xmlElementPtr)
				{
					map->setNamedItem(xmlElementPtr, &xmlNodeEntryPtr);
				}
			}
			else
			{
				return 0;
			}
		}
	}
	else
	{
		CXTPDOMNodePtr xmlNodeSectionPtr = m_xmlSectionNode;

		HRESULT hr = xmlNodeSectionPtr->selectSingleNode(_bstr_t(lpszEntry), &xmlNodeEntryPtr);
		if (FAILED(hr) || xmlNodeEntryPtr == NULL)
		{
			if (bCreate)
			{

				CXTPDOMElementPtr xmlElementPtr;
				m_xmlDocPtr->createElement(_bstr_t(lpszEntry), &xmlElementPtr);
				ASSERT(xmlElementPtr != 0);
				if (xmlElementPtr)
				{
					xmlNodeSectionPtr->appendChild(xmlElementPtr, &xmlNodeEntryPtr);
				}
			}
			else
			{
				return 0;
			}
		}

	}
	ASSERT(xmlNodeEntryPtr != 0);
	return xmlNodeEntryPtr;
}


BOOL CXTPPropExchangeXMLNode::ExchangeProp(LPCTSTR pszPropName, VARTYPE vtProp,
	void* pvProp, const void* pvDefault)
{
	if (m_bLoading)
	{
		if (!m_bUseDefaultOnLoad)
			pvDefault = NULL;

		if (!OnBeforeExchange())
			return CopyPropValue(vtProp, pvProp, pvDefault);

		CXTPDOMNodePtr xmlEntryPtr = GetEntryNode(pszPropName, FALSE);
		if (xmlEntryPtr== 0)
			return CopyPropValue(vtProp, pvProp, pvDefault);

		BSTR bstrValue = NULL;
		xmlEntryPtr->get_text(&bstrValue);
		CString strValue(bstrValue);
		if (bstrValue) SysFreeString(bstrValue);

		return ExchangePropStringHelper(strValue, vtProp, pvProp, pvDefault);
	}
	else
	{
		if (IsSamePropValue(vtProp, pvProp, pvDefault))
			return TRUE;

		if (!OnBeforeExchange())
			return FALSE;

		CXTPDOMNodePtr xmlEntryPtr = GetEntryNode(pszPropName, TRUE);
		ASSERT(xmlEntryPtr != 0);

		CString strValue;
		if (!ExchangePropStringHelper(strValue, vtProp, pvProp, pvDefault))
			return FALSE;

		xmlEntryPtr->put_text(bstr_t(strValue));
	}
	return TRUE;

}

BOOL CXTPPropExchangeXMLNode::IsPropertyExists(LPCTSTR pszPropName)
{
	ASSERT(IsLoading());

	if (!OnBeforeExchange())
		return FALSE;

	if (GetEntryNode(pszPropName, FALSE) != NULL)
		return TRUE;
	
	return FALSE;
}


class CXTPPropExchangeXMLNode::CXMLEnumerator : public CXTPPropExchangeEnumerator
{
public:
	CXMLEnumerator::CXMLEnumerator(CXTPPropExchangeXMLNode* pContainer, LPCTSTR lpszSection)
		: CXTPPropExchangeEnumerator(pContainer, lpszSection)
	{
	}

public:	
	CXTPDOMNodePtr GetProfileNode(POSITION pos)
	{
		CXTPDOMNodePtr xmlNodePtr;

		CString strNode;
		strNode.Format(_T("%s[%i]"), m_strSectionName, pos);

		if (SUCCEEDED(((CXTPPropExchangeXMLNode*)m_pContainer)->m_xmlSectionNode->selectSingleNode(_bstr_t(strNode), &xmlNodePtr)))
			return xmlNodePtr;

		return 0;

	}

	POSITION GetPosition(DWORD dwCount = 0, BOOL /*bCompactMode*/ = TRUE) 
	{
		m_nCount = dwCount;

		if (!m_pContainer)
			return NULL;

		if (m_pContainer->IsLoading())
		{
			if (!m_pContainer->OnBeforeExchange())
				return NULL;

			m_xmNextNode = GetProfileNode(0);
			if (m_xmNextNode == 0)
				return NULL;			
		}
		
		return (POSITION)1;		
	}
	
	CXTPPropExchange* GetNext(POSITION& pos)
	{
		CXTPPropExchangeXMLNode* pSection = (CXTPPropExchangeXMLNode*)m_pContainer->GetSection(m_strSectionName);

		CXTPDOMNodePtr xmlNodePtr;
		CXTPDOMElementPtr xmlElementPtr;
			

		if (m_pContainer->IsStoring())
		{
			ASSERT((DWORD)(DWORD_PTR)pos <= m_nCount);			

			pSection->m_xmlDocPtr->createElement(_bstr_t(m_strSectionName), &xmlElementPtr);
			ASSERT(xmlElementPtr != 0);
			if (xmlElementPtr)
			{
				((CXTPPropExchangeXMLNode*)m_pContainer)->m_xmlSectionNode->appendChild(xmlElementPtr, &xmlNodePtr);
			}

			pos ++;			
			if ((DWORD)(DWORD_PTR)pos > m_nCount) pos = NULL;
		}
		else
		{
			ASSERT(m_xmNextNode != NULL);
			
			xmlNodePtr = m_xmNextNode;

			m_xmNextNode = GetProfileNode(pos);			
			if (m_xmNextNode == NULL) pos = NULL; else pos ++;				
			
		}

		pSection->m_bInitialized = TRUE;
		pSection->m_xmlSectionNode = xmlNodePtr;
		
		return pSection;
	}
protected:
	CXTPDOMNodePtr m_xmNextNode;
};



CXTPPropExchangeEnumerator* CXTPPropExchangeXMLNode::GetEnumerator(LPCTSTR lpszSection)
{
	if (IsStoring() || !IsPropertyExists(_T("Count")))
		return new CXMLEnumerator(this, lpszSection);
	
	return new CXTPPropExchangeEnumerator(this, lpszSection);	
}

#endif

//////////////////////////////////////////////////////////////////////////
// CXTPPropExchangeIniFile

CXTPPropExchangeIniFile::CXTPPropExchangeIniFile(BOOL bLoading, LPCTSTR lpszParentSection, LPCTSTR lpszSection)
{
	ASSERT(lpszSection != NULL);

	m_bLoading = bLoading;
	
	if (lpszParentSection == NULL)
	{
		m_strSectionName = lpszSection;
	}
	else
	{
		m_strSectionName = CString(lpszParentSection) + _T("\\") + lpszSection;
	}

}

CXTPPropExchangeIniFile::~CXTPPropExchangeIniFile()
{
	if (IsRootSection() && !m_strFileName.IsEmpty())
	{
		::DeleteFile((LPTSTR)(LPCTSTR)m_strFileName);
	}

}

CXTPPropExchange* CXTPPropExchangeIniFile::GetSection(LPCTSTR lpszSection)
{
	OnBeforeExchange();

	CXTPPropExchangeIniFile* pSection = new CXTPPropExchangeIniFile(IsLoading(), m_strSectionName, lpszSection);
	pSection->InitSection(this);
	pSection->m_strFileName = m_strFileName;
	return pSection;
}

BOOL CXTPPropExchangeIniFile::OnBeforeExchange()
{
	if (!m_bInitialized)
	{
		if (m_strFileName.IsEmpty())
		{
			TCHAR lpszTempPath[MAX_PATH];
			TCHAR lpszTempFileName[MAX_PATH];
			
			if (GetTempPath(MAX_PATH, lpszTempPath) == 0)
				return FALSE;

			if (GetTempFileName(lpszTempPath, _T("xtp"), 0, lpszTempFileName) == 0)
				return FALSE;
			
			m_strFileName = lpszTempFileName;
		}

		m_bInitialized = TRUE;
	}
	return !m_strFileName.IsEmpty();
}


BOOL CXTPPropExchangeIniFile::ExchangeProp(LPCTSTR pszPropName, VARTYPE vtProp,
				void* pvProp, const void* pvDefault /*= NULL*/)
{
	if (m_bLoading)
	{
		if (!m_bUseDefaultOnLoad)
			pvDefault = NULL;

		if (!OnBeforeExchange())
			return CopyPropValue(vtProp, pvProp, pvDefault);

		TCHAR lpszValue[4096];
		if (GetPrivateProfileString(m_strSectionName, pszPropName, NULL, lpszValue, 4096, m_strFileName) == 0)
			return CopyPropValue(vtProp, pvProp, pvDefault);

		CString strValue(lpszValue);
		return ExchangePropStringHelper(strValue, vtProp, pvProp, pvDefault);
	}
	else
	{
		if (!OnBeforeExchange())
			return FALSE;

		if (IsSamePropValue(vtProp, pvProp, pvDefault))
			return TRUE;

		CString strValue;
		if (!ExchangePropStringHelper(strValue, vtProp, pvProp, pvDefault))
			return FALSE;

		if (!WritePrivateProfileString (m_strSectionName, pszPropName, strValue, m_strFileName))
			return FALSE;
	}

	return TRUE;
}


void CXTPPropExchangeIniFile::SaveToFile(CString strFileName)
{
	ASSERT(IsRootSection() && IsStoring());

	::DeleteFile(strFileName);
	::CopyFile(m_strFileName, strFileName, FALSE);
}

BOOL CXTPPropExchangeIniFile::LoadFromFile(CString strFileName)
{
	ASSERT(IsRootSection() && IsLoading());

	if (ACCESS_S(strFileName, 0) == -1)
		return FALSE;
	
	if (!OnBeforeExchange())
		return FALSE;

	return ::CopyFile(strFileName, m_strFileName, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// PX_ functions

BOOL AFX_CDECL PX_Byte(CXTPPropExchange* pPX, LPCTSTR pszPropName, BYTE& nValue, BYTE nDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&nValue, short);

	return  pPX->ExchangeProp(pszPropName, VT_UI1, &nValue, &nDefault);
}

BOOL AFX_CDECL PX_Short(CXTPPropExchange* pPX, LPCTSTR pszPropName, short& sValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&sValue, short);

	return  pPX->ExchangeProp(pszPropName, VT_I2, &sValue);
}

BOOL AFX_CDECL PX_Short(CXTPPropExchange* pPX, LPCTSTR pszPropName, short& sValue,
	short sDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&sValue, short);

	return pPX->ExchangeProp(pszPropName, VT_I2, &sValue, &sDefault);
}

BOOL AFX_CDECL PX_UShort(CXTPPropExchange* pPX, LPCTSTR pszPropName, USHORT& usValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&usValue, USHORT);

	return pPX->ExchangeProp(pszPropName, VT_I2, &usValue);
}

BOOL AFX_CDECL PX_UShort(CXTPPropExchange* pPX, LPCTSTR pszPropName, USHORT& usValue,
	USHORT usDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&usValue, USHORT);

	return pPX->ExchangeProp(pszPropName, VT_I2, &usValue, &usDefault);
}

BOOL AFX_CDECL PX_Long(CXTPPropExchange* pPX, LPCTSTR pszPropName, long& lValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&lValue, long);

	return pPX->ExchangeProp(pszPropName, VT_I4, &lValue);
}

BOOL AFX_CDECL PX_Long(CXTPPropExchange* pPX, LPCTSTR pszPropName, long& lValue,
	long lDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&lValue, long);

	return pPX->ExchangeProp(pszPropName, VT_I4, &lValue, &lDefault);
}

BOOL AFX_CDECL PX_DWord(CXTPPropExchange* pPX, LPCTSTR pszPropName, DWORD& lValue,
	DWORD lDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&lValue, long);

	return pPX->ExchangeProp(pszPropName, VT_I4, &lValue, &lDefault);
}


BOOL AFX_CDECL PX_Int(CXTPPropExchange* pPX, LPCTSTR pszPropName, int& nValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&nValue, int);

	return pPX->ExchangeProp(pszPropName, VT_I4, &nValue);
}

BOOL AFX_CDECL PX_Int(CXTPPropExchange* pPX, LPCTSTR pszPropName, int& nValue,
	int nDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&nValue, int);

	return pPX->ExchangeProp(pszPropName, VT_I4, &nValue, &nDefault);
}

BOOL AFX_CDECL PX_ULong(CXTPPropExchange* pPX, LPCTSTR pszPropName, ULONG& ulValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&ulValue, ULONG);

	return pPX->ExchangeProp(pszPropName, VT_I4, &ulValue);
}

BOOL AFX_CDECL PX_ULong(CXTPPropExchange* pPX, LPCTSTR pszPropName, ULONG& ulValue,
	ULONG ulDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&ulValue, ULONG);

	return pPX->ExchangeProp(pszPropName, VT_I4, &ulValue, &ulDefault);
}

BOOL AFX_CDECL PX_Bool(CXTPPropExchange* pPX, LPCTSTR pszPropName, BOOL& bValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&bValue, BOOL);

	return pPX->ExchangeProp(pszPropName, VT_BOOL, &bValue);
}

BOOL AFX_CDECL PX_Bool(CXTPPropExchange* pPX, LPCTSTR pszPropName, BOOL& bValue,
	BOOL bDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&bValue, BOOL);

	return pPX->ExchangeProp(pszPropName, VT_BOOL, &bValue, &bDefault);
}

BOOL AFX_CDECL PX_String(CXTPPropExchange* pPX, LPCTSTR pszPropName, CString& strValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&strValue, CString);

	return pPX->ExchangeProp(pszPropName, VT_LPSTR, &strValue);
}

BOOL AFX_CDECL PX_String(CXTPPropExchange* pPX, LPCTSTR pszPropName, CString& strValue,
	LPCTSTR lpszDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&strValue, CString);

	return pPX->ExchangeProp(pszPropName, VT_LPSTR, &strValue, lpszDefault);
}

BOOL AFX_CDECL PX_Float(CXTPPropExchange* pPX, LPCTSTR pszPropName, float& floatValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&floatValue, float);

	return pPX->ExchangeProp(pszPropName, VT_R4, &floatValue);
}

BOOL AFX_CDECL PX_Float(CXTPPropExchange* pPX, LPCTSTR pszPropName, float& floatValue,
	float floatDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&floatValue, float);

	return pPX->ExchangeProp(pszPropName, VT_R4, &floatValue, &floatDefault);
}

BOOL AFX_CDECL PX_Double(CXTPPropExchange* pPX, LPCTSTR pszPropName, double& doubleValue)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&doubleValue, double);

	return pPX->ExchangeProp(pszPropName, VT_R8, &doubleValue);
}

BOOL AFX_CDECL PX_Double(CXTPPropExchange* pPX, LPCTSTR pszPropName, double& doubleValue,
	double doubleDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT_POINTER(&doubleValue, double);

	return pPX->ExchangeProp(pszPropName, VT_R8, &doubleValue, &doubleDefault);
}

BOOL AFX_CDECL PX_Size(CXTPPropExchange* pPX, LPCTSTR pszPropName, SIZE& szValue, SIZE szDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));

	return pPX->ExchangeProp(pszPropName, VT_EX_SIZE, &szValue, &szDefault);
}

BOOL AFX_CDECL PX_Point(CXTPPropExchange* pPX, LPCTSTR pszPropName, POINT& ptValue, POINT ptDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));
	ASSERT(sizeof(POINT) == sizeof(SIZE));

	return pPX->ExchangeProp(pszPropName, VT_EX_SIZE, &ptValue, &ptDefault);
}

BOOL AFX_CDECL PX_Rect(CXTPPropExchange* pPX, LPCTSTR pszPropName, RECT& rcValue, RECT rcDefault)
{
	ASSERT_POINTER(pPX, CXTPPropExchange);
	ASSERT(AfxIsValidString(pszPropName));

	return pPX->ExchangeProp(pszPropName, VT_EX_RECT, &rcValue, &rcDefault);
}

BOOL AFX_CDECL PX_RuntimeClass(CXTPPropExchange* pPX, LPCTSTR pszPropName, CRuntimeClass*& pClass, CRuntimeClass* pDefaultClass)
{
	return pPX->ExchangeRuntimeClass(pszPropName, pClass, pDefaultClass);
}

void AFX_CDECL PX_Blob(CXTPPropExchange* pPX, LPCTSTR pszPropName, BYTE*& pData, DWORD& nBytes)
{
	pPX->ExchangeBlobProp(pszPropName, pData, nBytes);
}


BOOL AFX_CDECL PX_Serialize(CXTPPropExchange* pPX, LPCTSTR pszPropName, CObject* pObject)
{
	CArchive* pArchive = pPX->GetArchive(pszPropName);

	if (!pArchive)
		return FALSE;

	BOOL bResult = FALSE;
	try
	{
		pObject->Serialize(*pArchive);
		bResult = TRUE;
	}
	catch (COleException* pEx)
	{
		pEx->Delete ();

	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
	}

	pPX->ReleaseArchive(pszPropName, pArchive);

	return bResult;
}
