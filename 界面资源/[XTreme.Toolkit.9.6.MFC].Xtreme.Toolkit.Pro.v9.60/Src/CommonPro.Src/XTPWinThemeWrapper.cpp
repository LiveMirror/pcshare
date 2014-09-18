// XTPWinThemeWrapper.cpp: implementation of the CXTPWinThemeWrapper class.
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
#include <shlwapi.h>

#include "uxtheme.h"
#include "schemadef.h"
#include "tmschema.h"
#include "XTPWinThemeWrapper.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable: 4571) // warning C4571: catch(...) blocks compiled with /EHs do not catch or re-throw Structured Exceptions


#define TRY_POINTER_CALL(proc)\
	try\
	{\
		if ( m_sData.m_hThemeDll != NULL && m_sData.m_ptrWrappers[xtpWrapper##proc] == NULL )\
		{\
			m_sData.m_ptrWrappers[xtpWrapper##proc] = ::GetProcAddress(\
				m_sData.m_hThemeDll, xtpWrapperProc##proc);\
		}\
		PFN##proc ptr = (PFN##proc)m_sData.m_ptrWrappers[xtpWrapper##proc];\
		if ( ptr )\
		{
#define POINTER_CALL (*ptr)
#define CATCH_POINTER_CALL()\
		}\
	}\
	catch (...)\
	{\
	}


BOOL CXTPWinThemeWrapper::m_bDllDetached = FALSE;
CXTPWinThemeWrapper::CSharedData CXTPWinThemeWrapper::m_sData;


CXTPWinThemeWrapper::CSharedData::CSharedData()
{
	m_dwComCtlVersion = 0;
	ZeroMemory(&m_ptrWrappers, sizeof(m_ptrWrappers));

	m_hThemeDll = ::LoadLibrary(_T("UxTheme.dll"));

	HMODULE hComCtlDll = ::LoadLibrary(_T("comctl32.dll"));

	if (hComCtlDll)
	{
		m_dwComCtlVersion = CXTPWinThemeWrapper::GetModuleVersion(hComCtlDll);
		::FreeLibrary( hComCtlDll );
	}
}

//===========================================================================
// CXTPWinThemeWrapper class
//===========================================================================

CXTPWinThemeWrapper::CXTPWinThemeWrapper()
: m_hTheme(NULL)
{

}

CXTPWinThemeWrapper::~CXTPWinThemeWrapper()
{
	CloseThemeData();
}

void CXTPWinThemeWrapper::SetThemeWrapperPtr(int nThemeWrapper, LPVOID lpWrapperPtr)
{
	m_sData.m_ptrWrappers[nThemeWrapper] = lpWrapperPtr;
}


void CXTPWinThemeWrapper::CloseThemeData()
{
	if (m_hTheme && !m_bDllDetached)
	{
		TRY_POINTER_CALL(CloseThemeData)
			POINTER_CALL(m_hTheme);
		CATCH_POINTER_CALL()
	}

	m_hTheme = NULL;
}

HRESULT CXTPWinThemeWrapper::DrawThemeBackground(HDC hDC, int iPartID, int iStateID, const RECT *pRect, const RECT *pClipRect)
{
	TRY_POINTER_CALL(DrawThemeBackground);
		return POINTER_CALL(m_hTheme, hDC, iPartID, iStateID, pRect, pClipRect);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::DrawThemeBackgroundEx(HDC hDC, int iPartID, int iStateID, const RECT *pRect, const DTBGOPTS *pOptions)
{
	TRY_POINTER_CALL(DrawThemeBackgroundEx)
		return POINTER_CALL(m_hTheme, hDC, iPartID, iStateID, pRect, pOptions);
	CATCH_POINTER_CALL()

	return E_FAIL;
}


HRESULT CXTPWinThemeWrapper::DrawThemeEdge(HDC hDC, int iPartID, int iStateID, const RECT *pDestRect, UINT uEdge, UINT uFlags, RECT *pContentRect)
{
	TRY_POINTER_CALL(DrawThemeEdge)
		return POINTER_CALL(m_hTheme, hDC, iPartID, iStateID, pDestRect, uEdge, uFlags, pContentRect);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::DrawThemeIcon(HDC hDC, int iPartID, int iStateID, const RECT *pRect, HIMAGELIST himl, int iImageIndex)
{
	TRY_POINTER_CALL(DrawThemeIcon)
		return POINTER_CALL(m_hTheme, hDC, iPartID, iStateID, pRect, himl, iImageIndex);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::DrawThemeParentBackground(HWND hWnd, HDC hDC, RECT *pRect)
{
	TRY_POINTER_CALL(DrawThemeParentBackground)
		return POINTER_CALL(hWnd, hDC, pRect);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::DrawThemeText(HDC hDC, int iPartID, int iStateID, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, const RECT *pRect)
{
	TRY_POINTER_CALL(DrawThemeText)
		return POINTER_CALL(m_hTheme, hDC, iPartID, iStateID, pszText, iCharCount, dwTextFlags, dwTextFlags2, pRect);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::EnableThemeDialogTexture(HWND hWnd, DWORD dwFlags)
{
	TRY_POINTER_CALL(EnableThemeDialogTexture)
		return POINTER_CALL(hWnd, dwFlags);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::EnableTheming(BOOL fEnable)
{
	TRY_POINTER_CALL(EnableTheming)
		return POINTER_CALL(fEnable);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetCurrentThemeName(LPWSTR pszThemeFileName, int dwMaxNameChars, LPWSTR pszColorBuff, int cchMaxColorChars, LPWSTR pszSizeBuff, int cchMaxSizeChars)
{
	TRY_POINTER_CALL(GetCurrentThemeName)
		return POINTER_CALL(pszThemeFileName, dwMaxNameChars, pszColorBuff, cchMaxColorChars, pszSizeBuff, cchMaxSizeChars);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

DWORD CXTPWinThemeWrapper::ThemeAppProperties()
{
	TRY_POINTER_CALL(ThemeAppProperties)
		return POINTER_CALL();
	CATCH_POINTER_CALL()

	return 0L;
}

HRESULT CXTPWinThemeWrapper::GetThemeBackgroundContentRect(HDC hDC, int iPartID, int iStateID, const RECT *pBoundingRect, RECT *pContentRect)
{
	TRY_POINTER_CALL(GetThemeBackgroundContentRect)
		return POINTER_CALL(m_hTheme, hDC, iPartID, iStateID, pBoundingRect, pContentRect);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemeBackgroundExtent(HDC hDC, int iPartID, int iStateID, const RECT *pContentRect, RECT *pExtentRect)
{
	TRY_POINTER_CALL(GetThemeBackgroundExtent)
		return POINTER_CALL(m_hTheme, hDC, iPartID, iStateID, pContentRect, pExtentRect);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemeBackgroundRegion(HDC hDC, int iPartID, int iStateID, const RECT *pRect, HRGN *pRegion)
{
	TRY_POINTER_CALL(GetThemeBackgroundRegion)
		return POINTER_CALL(m_hTheme, hDC, iPartID, iStateID, pRect, pRegion);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemeBool(int iPartID, int iStateID, int iPropID, BOOL *pfVal)
{
	TRY_POINTER_CALL(GetThemeBool)
		return POINTER_CALL(m_hTheme, iPartID, iStateID, iPropID, pfVal);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemeColor(int iPartID, int iStateID, int iPropID, COLORREF *pColor)
{
	TRY_POINTER_CALL(GetThemeColor)
		return POINTER_CALL(m_hTheme, iPartID, iStateID, iPropID, pColor);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemeDocumentationProperty(LPCWSTR pszThemeName, LPCWSTR pszPropertyName, LPWSTR pszValueBuff, int cchMaxValChars)
{
	TRY_POINTER_CALL(GetThemeDocumentationProperty)
		return POINTER_CALL(pszThemeName, pszPropertyName, pszValueBuff, cchMaxValChars);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemeEnumValue(int iPartID, int iStateID, int iPropID, int *piVal)
{
	TRY_POINTER_CALL(GetThemeEnumValue)
		return POINTER_CALL(m_hTheme, iPartID, iStateID, iPropID, piVal);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemeFilename(int iPartID, int iStateID, int iPropID, LPWSTR pszThemeFilename, int cchMaxBuffChars)
{
	TRY_POINTER_CALL(GetThemeFilename)
		return POINTER_CALL(m_hTheme, iPartID, iStateID, iPropID, pszThemeFilename, cchMaxBuffChars);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemeFont(HDC hDC, int iPartID, int iStateID, int iPropID, LOGFONT *pFont)
{
	TRY_POINTER_CALL(GetThemeFont)
		return POINTER_CALL(m_hTheme, hDC, iPartID, iStateID, iPropID, pFont);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemeInt(int iPartID, int iStateID, int iPropID, int *piVal)
{
	TRY_POINTER_CALL(GetThemeInt)
		return POINTER_CALL(m_hTheme, iPartID, iStateID, iPropID, piVal);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemeIntList(int iPartID, int iStateID, int iPropID, INTLIST *pIntList)
{
	TRY_POINTER_CALL(GetThemeIntList)
		return POINTER_CALL(m_hTheme, iPartID, iStateID, iPropID, pIntList);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemeMargins(HDC hDC, int iPartID, int iStateID, int iPropID, RECT *pRect, MARGINS *pMargins)
{
	TRY_POINTER_CALL(GetThemeMargins)
		return POINTER_CALL(m_hTheme, hDC, iPartID, iStateID, iPropID, pRect, pMargins);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemeMetric(HDC hDC, int iPartID, int iStateID, int iPropID, int *piVal)
{
	TRY_POINTER_CALL(GetThemeMetric)
		return POINTER_CALL(m_hTheme, hDC, iPartID, iStateID, iPropID, piVal);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemePartSize(HDC hDC, int iPartID, int iStateID, RECT *pRect, THEMESIZE eSize, SIZE *pSize)
{
	TRY_POINTER_CALL(GetThemePartSize)
		return POINTER_CALL(m_hTheme, hDC, iPartID, iStateID, pRect, eSize, pSize);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemePosition(int iPartID, int iStateID, int iPropID, POINT *pPoint)
{
	TRY_POINTER_CALL(GetThemePosition)
		return POINTER_CALL(m_hTheme, iPartID, iStateID, iPropID, pPoint);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemePropertyOrigin(int iPartID, int iStateID, int iPropID, PROPERTYORIGIN *pOrigin)
{
	TRY_POINTER_CALL(GetThemePropertyOrigin)
		return POINTER_CALL(m_hTheme, iPartID, iStateID, iPropID, pOrigin);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemeRect(int iPartID, int iStateID, int iPropID, RECT *pRect)
{
	TRY_POINTER_CALL(GetThemeRect)
		return POINTER_CALL(m_hTheme, iPartID, iStateID, iPropID, pRect);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemeString(int iPartID, int iStateID, int iPropID, LPWSTR pszBuff, int cchMaxBuffChars)
{
	TRY_POINTER_CALL(GetThemeString)
		return POINTER_CALL(m_hTheme, iPartID, iStateID, iPropID, pszBuff, cchMaxBuffChars);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

BOOL CXTPWinThemeWrapper::GetThemeSysBool(int iBoolID)
{
	TRY_POINTER_CALL(GetThemeSysBool)
		return POINTER_CALL(m_hTheme, iBoolID);
	CATCH_POINTER_CALL()

	return FALSE;
}

COLORREF CXTPWinThemeWrapper::GetThemeSysColor(int iColorID)
{
	TRY_POINTER_CALL(GetThemeSysColor)
		return POINTER_CALL(m_hTheme, iColorID);
	CATCH_POINTER_CALL()

	return (COLORREF)-1;
}

HBRUSH CXTPWinThemeWrapper::GetThemeSysColorBrush(int iColorID)
{
	TRY_POINTER_CALL(GetThemeSysColorBrush)
		return POINTER_CALL(m_hTheme, iColorID);
	CATCH_POINTER_CALL()

	return NULL;
}

HRESULT CXTPWinThemeWrapper::GetThemeSysFont(int iFontID, LOGFONT *pLF)
{
	TRY_POINTER_CALL(GetThemeSysFont)
		return POINTER_CALL(m_hTheme, iFontID, pLF);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemeSysInt(int iIntID, int *piValue)
{
	TRY_POINTER_CALL(GetThemeSysInt)
		return POINTER_CALL(m_hTheme, iIntID, piValue);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

int CXTPWinThemeWrapper::GetThemeSysSize(int iSizeID)
{
	TRY_POINTER_CALL(GetThemeSysSize)
		return POINTER_CALL(m_hTheme, iSizeID);
	CATCH_POINTER_CALL()

	return 0;
}

HRESULT CXTPWinThemeWrapper::GetThemeSysString(int iStringID, LPWSTR pszStringBuff, int cchMaxStringChars)
{
	TRY_POINTER_CALL(GetThemeSysString)
		return POINTER_CALL(m_hTheme, iStringID, pszStringBuff, cchMaxStringChars);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemeTextExtent(HDC hDC, int iPartID, int iStateID, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, const RECT *pBoundingRect, RECT *pExtentRect)
{
	TRY_POINTER_CALL(GetThemeTextExtent)
		return POINTER_CALL(m_hTheme, hDC, iPartID, iStateID, pszText, iCharCount, dwTextFlags, pBoundingRect, pExtentRect);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HRESULT CXTPWinThemeWrapper::GetThemeTextMetrics(HDC hDC, int iPartID, int iStateID, TEXTMETRIC *ptm)
{
	TRY_POINTER_CALL(GetThemeTextMetrics)
		return POINTER_CALL(m_hTheme, hDC, iPartID, iStateID, ptm);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

HTHEME CXTPWinThemeWrapper::GetWindowTheme(HWND hWnd)
{
	TRY_POINTER_CALL(GetWindowTheme)
		return POINTER_CALL(hWnd);
	CATCH_POINTER_CALL()

	return NULL;
}

HRESULT CXTPWinThemeWrapper::HitTestThemeBackground(HDC hDC, int iPartID, int iStateID, DWORD dwOptions, const RECT *pRect, HRGN hRgn, POINT ptTest, WORD *pwHitTestCode)
{
	TRY_POINTER_CALL(HitTestThemeBackground)
		return POINTER_CALL(m_hTheme, hDC, iPartID, iStateID, dwOptions, pRect, hRgn, ptTest, pwHitTestCode);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

BOOL CXTPWinThemeWrapper::IsAppThemed()
{
	if (!m_hTheme)
		return FALSE;

	TRY_POINTER_CALL(IsAppThemed)
		return POINTER_CALL();
	CATCH_POINTER_CALL()

	return FALSE;
}

BOOL CXTPWinThemeWrapper::IsThemeActive()
{
	TRY_POINTER_CALL(IsThemeActive)
		return POINTER_CALL();
	CATCH_POINTER_CALL()

	return FALSE;
}

BOOL CXTPWinThemeWrapper::IsThemeBackgroundPartiallyTransparent(int iPartID, int iStateID)
{
	TRY_POINTER_CALL(IsThemeBackgroundPartiallyTransparent)
		return POINTER_CALL(m_hTheme, iPartID, iStateID);
	CATCH_POINTER_CALL()

	return FALSE;
}

BOOL CXTPWinThemeWrapper::IsThemeDialogTextureEnabled()
{
	TRY_POINTER_CALL(IsThemeDialogTextureEnabled)
		return POINTER_CALL();
	CATCH_POINTER_CALL()

	return FALSE;
}

BOOL CXTPWinThemeWrapper::IsThemePartDefined(int iPartID, int iStateID)
{
	TRY_POINTER_CALL(IsThemePartDefined)
		return POINTER_CALL(m_hTheme, iPartID, iStateID);
	CATCH_POINTER_CALL()

	return FALSE;
}

void CXTPWinThemeWrapper::OpenThemeData(HWND hWnd, LPCWSTR pszClassList)
{
	CloseThemeData();

	TRY_POINTER_CALL(OpenThemeData)
		m_hTheme = POINTER_CALL(hWnd, pszClassList);
	CATCH_POINTER_CALL()
}

void CXTPWinThemeWrapper::SetThemeAppProperties(DWORD dwFlags)
{
	TRY_POINTER_CALL(SetThemeAppProperties)
		POINTER_CALL(dwFlags);
	CATCH_POINTER_CALL()
}

HRESULT CXTPWinThemeWrapper::SetWindowTheme(HWND hWnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList)
{
	TRY_POINTER_CALL(SetWindowTheme)
		return POINTER_CALL(hWnd, pszSubAppName, pszSubIdList);
	CATCH_POINTER_CALL()

	return E_FAIL;
}

BOOL CXTPWinThemeWrapper::ThemeDataOpen() const
{
	return (m_hTheme != NULL);
}

DWORD CXTPWinThemeWrapper::GetModuleVersion(HMODULE hModule)
{
	typedef HRESULT ( CALLBACK* DLLGETVERSIONPROC )( DLLVERSIONINFO* );

	DLLGETVERSIONPROC pDllGetVersion =
		( DLLGETVERSIONPROC )::GetProcAddress( hModule, "DllGetVersion" );

	DWORD dwVersion = 0L;
	if ( pDllGetVersion != NULL )
	{
		DLLVERSIONINFO dvi;
		::ZeroMemory( &dvi, sizeof( dvi ) );
		dvi.cbSize = sizeof( dvi );

		HRESULT hr = ( *pDllGetVersion )( &dvi );
		if ( SUCCEEDED( hr ) )
		{
			ASSERT( dvi.dwMajorVersion <= 0xFFFF );
			ASSERT( dvi.dwMinorVersion <= 0xFFFF );
			dwVersion = MAKELONG( dvi.dwMinorVersion, dvi.dwMajorVersion );
		}
	}

	return dwVersion;
}

BOOL CXTPWinThemeWrapper::IsAppThemeReady()
{
	// If we could not load the UxTheme.dll then we cannot
	// use Windows XP themes.  This should only be NULL on
	// operatings systems older than Windows XP.

	if (m_sData.m_hThemeDll == NULL)
		return FALSE;

	// This will fail when using Windows "classic" or any non
	// theme mode the system themes are not available to use.

	if (!IsThemeActive())
		return FALSE;

	// This will fail when our application is not displaying
	// any controls that "could be themed" using Windows XP
	// visual styles.  This DOES NOT check to see if the application
	// has a manifest file it only checks to see if the application
	// has controls that "could be themed" using visual styles.

	if (!IsAppThemed())
		return FALSE;

	// By loading the comctl32.dll, we can determine if the application
	// has a manifest file because the application will load comctl32.dll
	// version 6.0 (or greater).  If it does not contain a manifest file
	// the application will load version 5.0 or earlier.

	// Our application is able to display Windows XP themes.
	if (HIWORD(m_sData.m_dwComCtlVersion) > 5)
		return TRUE;

	return FALSE;
}
