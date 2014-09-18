// XTPVC50Helpers.h : Visual C++ 5.0 helpers
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
#if !defined(__XTPVC50HELPERS_H__)
#define __XTPVC50HELPERS_H__

#if (_MSC_VER >= 1000)
#pragma once
#endif // _MSC_VER >= 1000

#if (_MSC_VER <= 1100)

class CXTPStringHelper
{
	class CXTPString : public CString {
		friend class CXTPStringHelper;
	};

public:
	static int Remove(CString& strRemove, TCHAR chRemove)
	{
		CXTPString& str = (CXTPString&) strRemove;

		str.CopyBeforeWrite();

		LPTSTR pstrSource = str.m_pchData;
		LPTSTR pstrDest = str.m_pchData;
		LPTSTR pstrEnd = str.m_pchData + str.GetData()->nDataLength;

		while (pstrSource < pstrEnd)
		{
			if (*pstrSource != chRemove)
			{
				*pstrDest = *pstrSource;
				pstrDest = _tcsinc(pstrDest);
			}
			pstrSource = _tcsinc(pstrSource);
		}
		*pstrDest = '\0';
		int nCount = pstrSource - pstrDest;
		str.GetData()->nDataLength -= nCount;

		return nCount;
	}

	static int Find(CString& strFind, LPCTSTR lpszSub, int nStart)
	{
		CXTPString& str = (CXTPString&) strFind;

		ASSERT(AfxIsValidString(lpszSub));

		int nLength = str.GetData()->nDataLength;
		if (nStart > nLength)
			return -1;

		// find first matching substring
		LPTSTR lpsz = _tcsstr(str.m_pchData + nStart, lpszSub);

		// return -1 for not found, distance from beginning otherwise
		return (lpsz == NULL) ? -1 : (int)(lpsz - str.m_pchData);
	}
	static int Find(CString& strFind, TCHAR ch, int nStart)
	{
		CXTPString& str = (CXTPString&) strFind;

		int nLength = str.GetData()->nDataLength;
		if (nStart >= nLength)
			return -1;

		// find first single character
		LPTSTR lpsz = _tcschr(str.m_pchData + nStart, (_TUCHAR)ch);

		// return -1 if not found and index otherwise
		return (lpsz == NULL) ? -1 : (int)(lpsz - str.m_pchData);
	}

	static int Replace(CString& strReplace, LPCTSTR lpszOld, LPCTSTR lpszNew)
	{
		CXTPString& str = (CXTPString&) strReplace;
		// can't have empty or NULL lpszOld

		int nSourceLen = str.SafeStrlen(lpszOld);
		if (nSourceLen == 0)
			return 0;
		int nReplacementLen = str.SafeStrlen(lpszNew);

		// loop once to figure out the size of the result string
		int nCount = 0;
		LPTSTR lpszStart = str.m_pchData;
		LPTSTR lpszEnd = str.m_pchData + str.GetData()->nDataLength;
		LPTSTR lpszTarget;
		while (lpszStart < lpszEnd)
		{
			while ((lpszTarget = _tcsstr(lpszStart, lpszOld)) != NULL)
			{
				nCount++;
				lpszStart = lpszTarget + nSourceLen;
			}
			lpszStart += lstrlen(lpszStart) + 1;
		}

		// if any changes were made, make them
		if (nCount > 0)
		{
			str.CopyBeforeWrite();

			// if the buffer is too small, just
			//   allocate a new buffer (slow but sure)
			int nOldLength = str.GetData()->nDataLength;
			int nNewLength =  nOldLength + (nReplacementLen-nSourceLen)*nCount;
			if (str.GetData()->nAllocLength < nNewLength || str.GetData()->nRefs > 1)
			{
				CStringData* pOldData = str.GetData();
				LPTSTR pstr = str.m_pchData;
				str.AllocBuffer(nNewLength);
				memcpy(str.m_pchData, pstr, pOldData->nDataLength*sizeof(TCHAR));
				CXTPString::Release(pOldData);
			}
			// else, we just do it in-place
			lpszStart = str.m_pchData;
			lpszEnd = str.m_pchData + str.GetData()->nDataLength;

			// loop again to actually do the work
			while (lpszStart < lpszEnd)
			{
				while ((lpszTarget = _tcsstr(lpszStart, lpszOld)) != NULL)
				{
					int nBalance = nOldLength - (lpszTarget - str.m_pchData + nSourceLen);
					memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen,
						nBalance * sizeof(TCHAR));
					memcpy(lpszTarget, lpszNew, nReplacementLen*sizeof(TCHAR));
					lpszStart = lpszTarget + nReplacementLen;
					lpszStart[nBalance] = '\0';
					nOldLength += (nReplacementLen - nSourceLen);
				}
				lpszStart += lstrlen(lpszStart) + 1;
			}
			ASSERT(str.m_pchData[nNewLength] == '\0');
			str.GetData()->nDataLength = nNewLength;
		}

		return nCount;
	}

	static int Delete(CString& strSource,int nIndex, int nCount)
	{
		CXTPString& str = (CXTPString&) strSource;

		if (nIndex < 0)
			nIndex = 0;
		int nNewLength = str.GetData()->nDataLength;
		if (nCount > 0 && nIndex < nNewLength)
		{
			str.CopyBeforeWrite();
			int nBytesToCopy = nNewLength - (nIndex + nCount) + 1;

			memcpy(str.m_pchData + nIndex,
				str.m_pchData + nIndex + nCount, nBytesToCopy * sizeof(TCHAR));
			str.GetData()->nDataLength = nNewLength - nCount;
		}

		return nNewLength;
	}

	static int Insert(CString& strSource, int nIndex, LPCTSTR pstr)
	{
		if (nIndex < 0)
			nIndex = 0;

		CXTPString& str = (CXTPString&) strSource;

		int nInsertLength = str.SafeStrlen(pstr);
		int nNewLength = str.GetData()->nDataLength;
		if (nInsertLength > 0)
		{
			str.CopyBeforeWrite();
			if (nIndex > nNewLength)
				nIndex = nNewLength;
			nNewLength += nInsertLength;

			if (str.GetData()->nAllocLength < nNewLength)
			{
				CStringData* pOldData = str.GetData();
				LPTSTR pstr = str.m_pchData;
				str.AllocBuffer(nNewLength);
				memcpy(str.m_pchData, pstr, (pOldData->nDataLength+1)*sizeof(TCHAR));
				CXTPString::Release(pOldData);
			}

			// move existing bytes down
			memmove(str.m_pchData + nIndex + nInsertLength,
				str.m_pchData + nIndex,
				(nNewLength-nIndex-nInsertLength+1)*sizeof(TCHAR));
			memmove(str.m_pchData + nIndex,
				pstr, nInsertLength*sizeof(TCHAR));
			str.GetData()->nDataLength = nNewLength;
		}

		return nNewLength;
	}
};

#endif

AFX_INLINE int REPLACE_S(CString& str, LPCTSTR lpszOld, LPCTSTR lpszNew) {
	#if (_MSC_VER <= 1100) // Using Visual C++ 5.0
		return CXTPStringHelper::Replace(str, lpszOld, lpszNew);
	#else
		return str.Replace(lpszOld, lpszNew);
	#endif
}

AFX_INLINE int FIND_S(CString& str, LPCTSTR lpszSub, int nStart) {
	#if (_MSC_VER <= 1100) // Using Visual C++ 5.0
		return CXTPStringHelper::Find(str, lpszSub, nStart);
	#else
		return str.Find(lpszSub, nStart);
	#endif
}

AFX_INLINE int FIND_S(CString& str, TCHAR ch, int nStart) {
	#if (_MSC_VER <= 1100) // Using Visual C++ 5.0
		return CXTPStringHelper::Find(str, ch, nStart);
	#else
		return str.Find(ch, nStart);
	#endif
}

AFX_INLINE int REMOVE_S(CString& str, TCHAR chRemove) {
	#if (_MSC_VER <= 1100) // Using Visual C++ 5.0
		return CXTPStringHelper::Remove(str, chRemove);
	#else
		return str.Remove(chRemove);
	#endif
}
AFX_INLINE int INSERT_S(CString& str, int nIndex, LPCTSTR pstr) {
	#if (_MSC_VER <= 1100) // Using Visual C++ 5.0
		return CXTPStringHelper::Insert(str, nIndex, pstr);
	#else
		return str.Insert(nIndex, pstr);
	#endif
}
AFX_INLINE int DELETE_S(CString& str, int nIndex, int nCount = 1) {
	#if (_MSC_VER <= 1100) // Using Visual C++ 5.0
		return CXTPStringHelper::Delete(str, nIndex, nCount);
	#else
		return str.Delete(nIndex, nCount);
	#endif
}



#if (_MSC_VER <= 1100)

AFX_INLINE LONG AFXAPI AfxDelRegTreeHelper(HKEY hParentKey, const CString& strKeyName)
{
	TCHAR   szSubKeyName[256];
	HKEY    hCurrentKey;
	DWORD   dwResult;

	if ((dwResult = RegOpenKey(hParentKey, strKeyName, &hCurrentKey)) ==
		ERROR_SUCCESS)
	{
		// Remove all subkeys of the key to delete
		while ((dwResult = RegEnumKey(hCurrentKey, 0, szSubKeyName, 255)) ==
				ERROR_SUCCESS)
		{
			if ((dwResult = AfxDelRegTreeHelper(hCurrentKey, szSubKeyName)) != ERROR_SUCCESS)
				break;
		}

		// If all went well, we should now be able to delete the requested key
		if ((dwResult == ERROR_NO_MORE_ITEMS) || (dwResult == ERROR_BADKEY))
		{
			dwResult = RegDeleteKey(hParentKey, strKeyName);
		}
	}

	RegCloseKey(hCurrentKey);
	return dwResult;
}

#endif
//}}AFX_CODEJOCK_PRIVATE

//////////////////////////////////////////////////////////////////////

#endif // #if !defined(__XTPVC50HELPERS_H__)
