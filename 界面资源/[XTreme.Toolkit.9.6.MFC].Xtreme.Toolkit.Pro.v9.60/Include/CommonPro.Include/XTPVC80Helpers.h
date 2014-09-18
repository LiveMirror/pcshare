// XTPVC80Helpers.h : Visual Studio 2005 helpers
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
#if !defined(__XTPVC80HELPERS_H__)
#define __XTPVC80HELPERS_H__

#if (_MSC_VER >= 1000)
#pragma once
#endif // _MSC_VER >= 1000

#include <io.h>
#include <stdio.h>
#include <tchar.h>

AFX_INLINE void MEMCPY_S(void *dest, const void *src, size_t count) {
	#if (_MSC_VER > 1310) // VS2005
	memcpy_s(dest, count, src, count);
	#else
	memcpy(dest, src, count);
	#endif
}
AFX_INLINE UINT RAND_S() {
	#if (_MSC_VER > 1310) && defined(_CRT_RAND_S) // VS2005
	unsigned int randVal = 0;
	rand_s(&randVal);
	return randVal;
	#else
	return (UINT)rand();
	#endif
}
AFX_INLINE void STRCPY_S(TCHAR* strDestination, size_t sizeInWords, const TCHAR* strSource) {
	#if (_MSC_VER > 1310) // VS2005
	_tcscpy_s(strDestination, sizeInWords, strSource);
	#else
	_tcscpy(strDestination, strSource);UNREFERENCED_PARAMETER(sizeInWords);
	#endif
}

AFX_INLINE void STRNCPY_S(TCHAR* strDestination, size_t sizeInWords, const TCHAR* strSource, size_t count) {
	#if (_MSC_VER > 1310) // VS2005
	_tcsncpy_s(strDestination, sizeInWords, strSource, count);
	#else
	_tcsncpy(strDestination, strSource, count);UNREFERENCED_PARAMETER(sizeInWords);
	#endif
}

AFX_INLINE TCHAR* STRTOK_S(TCHAR* strToken, const TCHAR* strDelimit, TCHAR** context) {
	#if (_MSC_VER > 1310) // VS2005
	return _tcstok_s(strToken, strDelimit, context);
	#else
	UNREFERENCED_PARAMETER(context);
	return _tcstok(strToken, strDelimit);
	#endif
}

AFX_INLINE int SPRINTF_S(TCHAR *buffer, size_t count, const TCHAR *format, ...) {
	va_list args;
	va_start(args, format);
	#if (_MSC_VER > 1310) // VS2005
	int result = _vstprintf_s(buffer, count, format, args);
	#else
	int result = _vstprintf(buffer, format, args);UNREFERENCED_PARAMETER(count);
	#endif
	va_end(args);
	return result;
}
AFX_INLINE void MEMMOVE_S(void *dest, const void *src, size_t count) {
	#if (_MSC_VER > 1310) // VS2005
	memmove_s(dest, count, src, count);
	#else
	memmove(dest, src, count);
	#endif
}
AFX_INLINE void SPLITPATH_S(const TCHAR* szFullPath, TCHAR* szDrive, TCHAR* szDir, TCHAR* szFileName, TCHAR* szExt) {
	#if (_MSC_VER > 1310) // VS2005
	_tsplitpath_s(szFullPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFileName, _MAX_FNAME, szExt, _MAX_EXT);
	#else
	_tsplitpath(szFullPath, szDrive, szDir, szFileName, szExt);
	#endif
}
AFX_INLINE FILE* FOPEN_S(const TCHAR* szFileName, const TCHAR* szMode) {
	FILE* stream;
	#if (_MSC_VER > 1310) // VS2005
	_tfopen_s(&stream, szFileName, szMode);
	#else
	stream = _tfopen(szFileName, szMode);
	#endif
	return stream;
}
AFX_INLINE void ITOW_S(int value, wchar_t* buffer, size_t sizeInWords, int radix) {
	#if (_MSC_VER > 1310) // VS2005
	_itow_s(value, buffer, sizeInWords, radix);
	#else
	_itow(value, buffer, radix);UNREFERENCED_PARAMETER(sizeInWords);
	#endif
}
AFX_INLINE wchar_t* WCSLWR_S(wchar_t *str, size_t sizeInWords) {
	#if (_MSC_VER > 1310) // VS2005
	_wcslwr_s(str, sizeInWords);
	return str;
	#else
	UNREFERENCED_PARAMETER(sizeInWords);
	return _wcslwr(str);
	#endif
}
AFX_INLINE void MBSTOWCS_S(LPWSTR dest, LPCTSTR src, size_t sizeInWords) {
#ifdef _UNICODE
	STRCPY_S(dest, sizeInWords, src);
#else
#if (_MSC_VER > 1310) // VS2005
	mbstowcs_s(&sizeInWords, dest, sizeInWords, src, sizeInWords);
#else
	mbstowcs(dest, src, sizeInWords);
#endif
#endif

}

AFX_INLINE void WCSTOMBS_S(LPSTR dest, LPCTSTR src, size_t sizeInWords) {
#ifndef _UNICODE
	STRCPY_S(dest, sizeInWords, src);
#else
#if (_MSC_VER > 1310) // VS2005
	wcstombs_s(&sizeInWords, dest, sizeInWords, src, sizeInWords);
#else
	wcstombs(dest, src, sizeInWords);
#endif
#endif

}

#if (_MSC_VER > 1310) // VS2005
#define SCANF_S _stscanf_s
#define SCANF_PARAM_S(x, count) x, count
#define ACCESS_S _taccess_s
#else
#define SCANF_S _stscanf
#define SCANF_PARAM_S(x, count) x
#define ACCESS_S _taccess
#endif
//}}AFX_CODEJOCK_PRIVATE


//////////////////////////////////////////////////////////////////////

#endif // #if !defined(__XTPVC80HELPERS_H__)
