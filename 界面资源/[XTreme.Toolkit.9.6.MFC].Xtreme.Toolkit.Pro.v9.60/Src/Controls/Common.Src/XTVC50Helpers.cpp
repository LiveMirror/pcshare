// XTVC50Helpers.cpp : Visual C++ 5.0 helpers
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
#include "XTGlobal.h"
#include "XTVC50Helpers.h"
#include "XTPColorManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if (_MSC_VER <= 1100) // Using Visual C++ 5.0

/////////////////////////////////////////////////////////////////////////////
// CXTStringHelper

CXTStringHelper::CXTStringHelper()
{
}

CXTStringHelper::CXTStringHelper(CString strIn) : CString(strIn)
{
}

int CXTStringHelper::Find(TCHAR ch) const
{
	return CString::Find(ch);
}

int CXTStringHelper::Find(LPCTSTR lpszSub) const
{
	return CString::Find(lpszSub);
}

const CString& CXTStringHelper::operator =(const CString& stringSrc)
{
	return CString::operator =(stringSrc);
}

const CString& CXTStringHelper::operator =(TCHAR ch)
{
	return CString::operator =(ch);
}

const CString& CXTStringHelper::operator =(const unsigned char* psz)
{
	return CString::operator =(psz);
}

const CString& CXTStringHelper::operator =(LPCWSTR lpsz)
{
	return CString::operator =(lpsz);
}

const CString& CXTStringHelper::operator =(LPCSTR lpsz)
{
	return CString::operator =(lpsz);
}

int CXTStringHelper::Find(LPCTSTR lpszSub, int nStart) const
{
	ASSERT(AfxIsValidString(lpszSub));

	int nLength = GetData()->nDataLength;
	if (nStart > nLength)
		return -1;

	// find first matching substring
	LPTSTR lpsz = _tcsstr(m_pchData + nStart, lpszSub);

	// return -1 for not found, distance from beginning otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

int CXTStringHelper::Insert(int nIndex, TCHAR ch)
{
	CopyBeforeWrite();

	if (nIndex < 0)
		nIndex = 0;

	int nNewLength = GetData()->nDataLength;
	if (nIndex > nNewLength)
		nIndex = nNewLength;
	nNewLength++;

	if (GetData()->nAllocLength < nNewLength)
	{
		CStringData* pOldData = GetData();
		LPTSTR pstr = m_pchData;
		AllocBuffer(nNewLength);
		memcpy(m_pchData, pstr, (pOldData->nDataLength+1)*sizeof(TCHAR));
		CString::Release(pOldData);
	}

	// move existing bytes down
	memmove(m_pchData + nIndex + 1,
		m_pchData + nIndex, (nNewLength-nIndex)*sizeof(TCHAR));
	m_pchData[nIndex] = ch;
	GetData()->nDataLength = nNewLength;

	return nNewLength;
}

int CXTStringHelper::Insert(int nIndex, LPCTSTR pstr)
{
	if (nIndex < 0)
		nIndex = 0;

	int nInsertLength = SafeStrlen(pstr);
	int nNewLength = GetData()->nDataLength;
	if (nInsertLength > 0)
	{
		CopyBeforeWrite();
		if (nIndex > nNewLength)
			nIndex = nNewLength;
		nNewLength += nInsertLength;

		if (GetData()->nAllocLength < nNewLength)
		{
			CStringData* pOldData = GetData();
			LPTSTR pstr = m_pchData;
			AllocBuffer(nNewLength);
			memcpy(m_pchData, pstr, (pOldData->nDataLength+1)*sizeof(TCHAR));
			CString::Release(pOldData);
		}

		// move existing bytes down
		memmove(m_pchData + nIndex + nInsertLength,
			m_pchData + nIndex,
			(nNewLength-nIndex-nInsertLength+1)*sizeof(TCHAR));
		memmove(m_pchData + nIndex,
			pstr, nInsertLength*sizeof(TCHAR));
		GetData()->nDataLength = nNewLength;
	}

	return nNewLength;
}

int CXTStringHelper::Remove(TCHAR chRemove)
{
	CopyBeforeWrite();

	LPTSTR pstrSource = m_pchData;
	LPTSTR pstrDest = m_pchData;
	LPTSTR pstrEnd = m_pchData + GetData()->nDataLength;

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
	GetData()->nDataLength -= nCount;

	return nCount;
}

int CXTStringHelper::Replace(TCHAR chOld, TCHAR chNew)
{
	int nCount = 0;

	// short-circuit the nop case
	if (chOld != chNew)
	{
		// otherwise modify each character that matches in the string
		CopyBeforeWrite();
		LPTSTR psz = m_pchData;
		LPTSTR pszEnd = psz + GetData()->nDataLength;
		while (psz < pszEnd)
		{
			// replace instances of the specified character only
			if (*psz == chOld)
			{
				*psz = chNew;
				nCount++;
			}
			psz = _tcsinc(psz);
		}
	}
	return nCount;
}

int CXTStringHelper::Replace(LPCTSTR lpszOld, LPCTSTR lpszNew)
{
	// can't have empty or NULL lpszOld

	int nSourceLen = SafeStrlen(lpszOld);
	if (nSourceLen == 0)
		return 0;
	int nReplacementLen = SafeStrlen(lpszNew);

	// loop once to figure out the size of the result string
	int nCount = 0;
	LPTSTR lpszStart = m_pchData;
	LPTSTR lpszEnd = m_pchData + GetData()->nDataLength;
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
		CopyBeforeWrite();

		// if the buffer is too small, just
		//   allocate a new buffer (slow but sure)
		int nOldLength = GetData()->nDataLength;
		int nNewLength =  nOldLength + (nReplacementLen-nSourceLen)*nCount;
		if (GetData()->nAllocLength < nNewLength || GetData()->nRefs > 1)
		{
			CStringData* pOldData = GetData();
			LPTSTR pstr = m_pchData;
			AllocBuffer(nNewLength);
			memcpy(m_pchData, pstr, pOldData->nDataLength*sizeof(TCHAR));
			CString::Release(pOldData);
		}
		// else, we just do it in-place
		lpszStart = m_pchData;
		lpszEnd = m_pchData + GetData()->nDataLength;

		// loop again to actually do the work
		while (lpszStart < lpszEnd)
		{
			while ((lpszTarget = _tcsstr(lpszStart, lpszOld)) != NULL)
			{
				int nBalance = nOldLength - (lpszTarget - m_pchData + nSourceLen);
				memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen,
					nBalance * sizeof(TCHAR));
				memcpy(lpszTarget, lpszNew, nReplacementLen*sizeof(TCHAR));
				lpszStart = lpszTarget + nReplacementLen;
				lpszStart[nBalance] = '\0';
				nOldLength += (nReplacementLen - nSourceLen);
			}
			lpszStart += lstrlen(lpszStart) + 1;
		}
		ASSERT(m_pchData[nNewLength] == '\0');
		GetData()->nDataLength = nNewLength;
	}

	return nCount;
}

int CXTStringHelper::Delete(int nIndex, int nCount)
{
	if (nIndex < 0)
		nIndex = 0;
	int nNewLength = GetData()->nDataLength;
	if (nCount > 0 && nIndex < nNewLength)
	{
		CopyBeforeWrite();
		int nBytesToCopy = nNewLength - (nIndex + nCount) + 1;

		memcpy(m_pchData + nIndex,
			m_pchData + nIndex + nCount, nBytesToCopy * sizeof(TCHAR));
		GetData()->nDataLength = nNewLength - nCount;
	}

	return nNewLength;
}

CXTStringHelper CXTStringHelper::Left(int nCount) const
{
	if (nCount < 0)
		nCount = 0;
	if (nCount >= GetData()->nDataLength)
		return *this;

	CXTStringHelper dest;
	AllocCopy(dest, nCount, 0, 0);
	return dest;
}

/////////////////////////////////////////////////////////////////////////////
// CXTHeaderCtrlHelper

int CXTHeaderCtrlHelper::GetItemCount() const
{
	ASSERT(::IsWindow(m_hWnd));
	return (int)::SendMessage(m_hWnd, HDM_GETITEMCOUNT, 0, 0L);
}

BOOL CXTHeaderCtrlHelper::GetItemRect(int nIndex, LPRECT lpRect) const
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(lpRect != NULL);
	return (BOOL)::SendMessage(m_hWnd, HDM_GETITEMRECT, nIndex, (LPARAM)lpRect);
}

BOOL CXTHeaderCtrlHelper::GetOrderArray(LPINT piArray, int iCount /* = -1 */)
{
	ASSERT(::IsWindow(m_hWnd));

	// if -1 was passed, find the count ourselves

	int nCount = iCount;
	if (nCount == -1)
	{
		nCount = GetItemCount();

		if (nCount == -1)
			return FALSE;
	}

	ASSERT(AfxIsValidAddress(piArray, iCount * sizeof(int)));

	return (BOOL) ::SendMessage(m_hWnd, HDM_GETORDERARRAY,
		(WPARAM) iCount, (LPARAM) piArray);
}


/////////////////////////////////////////////////////////////////////////////
// CXTListCtrlHelper

POSITION CXTListCtrlHelper::GetFirstSelectedItemPosition() const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	return (POSITION)(1+GetNextItem(-1, LVIS_SELECTED));
}

int CXTListCtrlHelper::GetNextSelectedItem(POSITION& pos) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	int nOldPos = (int)pos-1; pos = (POSITION)(1+GetNextItem(nOldPos, LVIS_SELECTED)); return nOldPos;
}

/////////////////////////////////////////////////////////////////////////////
// CXTListViewHelper

POSITION CXTListViewHelper::GetFirstSelectedItemPosition() const
{
	ASSERT(::IsWindow(GetListCtrl().GetSafeHwnd()));
	return (POSITION)(1+GetListCtrl().GetNextItem(-1, LVIS_SELECTED));
}

int CXTListViewHelper::GetNextSelectedItem(POSITION& pos) const
{
	ASSERT(::IsWindow(GetListCtrl().GetSafeHwnd()));
	int nOldPos = (int)pos-1; pos = (POSITION)(1+GetListCtrl().GetNextItem(nOldPos, LVIS_SELECTED)); return nOldPos;
}

#else

/////////////////////////////////////////////////////////////////////////////
// CXTListViewHelper

POSITION CXTListViewHelper::GetFirstSelectedItemPosition() const
{
	return GetListCtrl().GetFirstSelectedItemPosition();
}

int CXTListViewHelper::GetNextSelectedItem(POSITION& pos) const
{
	return GetListCtrl().GetNextSelectedItem(pos);
}


#endif//#if (_MSC_VER <= 1100)
