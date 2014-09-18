// XTGlobal.cpp : implementation of the CXTAuxData struct.
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
#include "XTPVC80Helpers.h"  // Visual Studio 2005 helper functions

#include "XTResource.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTPColorManager.h"
#include "XTRegistryManager.h"
#include "XTVersionManager.h"
#include "XTVC50Helpers.h"

#ifndef _XTP_INCLUDE_CONTROLS
#include "XTVersion.h"
#include "XTWndHook.h"
#endif

#ifdef _XTP_INCLUDE_COMMANDBARS
#include "XTPPaintManager.h"
#include "XTPCommandBars.h"
#include "XTPPopupBar.h"
#include "XTPImageManager.h"
#endif

#include "XTThemeManager.h"


#include <shlwapi.h>  // used for DLLVERSIONINFO

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef XT_IDC_HSPLITBAR
#define XT_IDC_HSPLITBAR 9970
#endif

#ifndef XT_IDC_VSPLITBAR
#define XT_IDC_VSPLITBAR 9971
#endif

//=============================================================================
// CXTNonClientMetrics
//=============================================================================

CXTNonClientMetrics::CXTNonClientMetrics()
{
	::ZeroMemory(this, sizeof(NONCLIENTMETRICS));
	cbSize = sizeof(NONCLIENTMETRICS);

	VERIFY(::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS), this, 0));
}

//=============================================================================
// CXTFontManager
//=============================================================================

CXTLogFont::CXTLogFont()
: dwType(0)
{
	::ZeroMemory(this, sizeof(CXTLogFont));
}

CXTLogFont::CXTLogFont(LOGFONT& logfont)
: dwType(0)
{
	MEMCPY_S((void*)&*this, (const void*)&logfont, (DWORD)sizeof(LOGFONT));
}

void CXTLogFont::operator = (LOGFONT& logfont)
{
	MEMCPY_S((void*)&*this, (const void*)&logfont, (DWORD)sizeof(LOGFONT));
}


//////////////////////////////////////////////////////////////////////////
// CXTAuxDataTheme

CXTAuxDataTheme::operator BOOL()
{
	return XTThemeManager()->GetTheme() == xtThemeDefault? FALSE: TRUE;
}

BOOL CXTAuxDataTheme::operator= (BOOL bXPMode)
{
	XTThemeManager()->SetTheme(bXPMode? xtThemeOfficeXP: xtThemeDefault);
	return bXPMode;
}

//=============================================================================
// CXTAuxData
//=============================================================================

CXTAuxData::CXTAuxData() : strINIFileName(CXTRegistryManager::GetINIFileName())
{
	iComCtlVersion   = -1;
	hInstance        = NULL;
	hcurDragCopy     = NULL;
	hcurDragMove     = NULL;
	hcurDragNone     = NULL;
	hcurHand         = NULL;
	hcurHandNone     = NULL;
	hcurHSplitBar    = NULL;
	hcurVSplitBar    = NULL;
	hcurMove         = NULL;
	bUseOfficeFont   = FALSE;
	bUseSolidShadows = FALSE;
	bMenuShadows     = TRUE;

	bWin95 = XTOSVersionInfo()->IsWin95();
	bWin98 = XTOSVersionInfo()->IsWin98();
	bWinNT = XTOSVersionInfo()->IsWinNT4();
	bWin2K = XTOSVersionInfo()->IsWin2K();
	bWinXP = XTOSVersionInfo()->IsWinXP();

	// Internet Explorer 4.0 or higher.
	GetComCtlVersion();
	VERIFY(IsComCtlValid());

	UpdateSysColors();
	UpdateSysMetrics();

	// statically linked
#if defined (_XT_STATICLINK) || defined (_XT_DYN_STATICLIB)
#if defined (_XT_USES_RESOURCE_DLL)
	#pragma message(__LOC__ "You must call 'CXTPManageState::SetModuleInfo()' to initialize resource handle." )
#else
	HMODULE hModule = ::GetModuleHandle(0);
	CXTPManageState::SetModuleInfo(hModule, hModule);
#endif // defined (_XT_USES_RESOURCE_DLL)
#endif // defined (_XT_STATICLINK) || defined (_XT_DYN_STATICLIB)

	// Load cursors and fonts.
	LoadSysCursors();
	LoadSysFonts();
}

CXTAuxData::~CXTAuxData()
{
	FreeSysCursors();
	FreeSysFonts();
}

// If LoadCursors() returns FALSE and your application is linking to the
// static version of the library you most likely need to include the following
// line in your .rc2 file: #include "XTResource.rc".  This name will vary
// depending on the library you are linking to.

BOOL CXTAuxData::LoadSysCursors()
{
	XT_MANAGE_STATE // switch to library resources/module state

	if (hInstance == NULL)
		hInstance = CXTPManageState::GetResourceHandle();

	FreeSysCursors();

	hcurDragNone = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XT_IDC_DRAGNONE));

	if (hcurDragNone == NULL)
		return FALSE;

	hcurDragCopy = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XT_IDC_DRAGCOPY));

	if (hcurDragCopy == NULL)
		return FALSE;

	hcurDragMove = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XT_IDC_DRAGMOVE));

	if (hcurDragMove == NULL)
		return FALSE;

	hcurHand = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XT_IDC_HAND));

	if (hcurHand == NULL)
		return FALSE;

	hcurHandNone = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XT_IDC_HANDNONE));

	if (hcurHandNone == NULL)
		return FALSE;

	hcurHSplitBar = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XT_IDC_HSPLITBAR));

	if (hcurHSplitBar == NULL)
		return FALSE;

	hcurVSplitBar = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XT_IDC_VSPLITBAR));

	if (hcurVSplitBar == NULL)
		return FALSE;

	hcurMove = ::LoadCursor(NULL,
		MAKEINTRESOURCE(IDC_SIZEALL));

	if (hcurMove == NULL)
		return FALSE;

	return TRUE;
}

void CXTAuxData::FreeSysCursors()
{
	// Free the cursors used by the Xtreme Toolkit.
	if (hcurDragNone != NULL) {
		::DestroyCursor(hcurDragNone);
	}
	if (hcurDragCopy != NULL) {
		::DestroyCursor(hcurDragCopy);
	}
	if (hcurDragMove != NULL) {
		::DestroyCursor(hcurDragMove);
	}
	if (hcurHand != NULL) {
		::DestroyCursor(hcurHand);
	}
	if (hcurHandNone != NULL) {
		::DestroyCursor(hcurHandNone);
	}
	if (hcurHSplitBar != NULL) {
		::DestroyCursor(hcurHSplitBar);
	}
	if (hcurVSplitBar != NULL) {
		::DestroyCursor(hcurVSplitBar);
	}
}

void CXTAuxData::UpdateSysColors()
{
	XTPColorManager()->RefreshColors();

	// Initialize standard color for windows components.
	clr3DFace                   = GetXtremeColor(COLOR_3DFACE);
	clr3DShadow                 = GetXtremeColor(COLOR_3DSHADOW);
	clr3DDkShadow               = GetXtremeColor(COLOR_3DDKSHADOW);
	clr3DHilight                = GetXtremeColor(COLOR_3DHILIGHT);
	clr3DLight                  = GetXtremeColor(COLOR_3DLIGHT);
	clrBtnText                  = GetXtremeColor(COLOR_BTNTEXT);
	clrGrayText                 = GetXtremeColor(COLOR_GRAYTEXT);
	clrHighlight                = GetXtremeColor(COLOR_HIGHLIGHT);
	clrHighlightText            = GetXtremeColor(COLOR_HIGHLIGHTTEXT);
	clrMenu                     = GetXtremeColor(COLOR_MENU);
	clrMenuText                 = GetXtremeColor(COLOR_MENUTEXT);
	clrWindow                   = GetXtremeColor(COLOR_WINDOW);
	clrWindowFrame              = GetXtremeColor(COLOR_WINDOWFRAME);
	clrWindowText               = GetXtremeColor(COLOR_WINDOWTEXT);
	clrActiveCaption            = GetXtremeColor(COLOR_ACTIVECAPTION);
	clrInActiveCaption          = GetXtremeColor(COLOR_INACTIVECAPTION);
	clrGradActiveCapt           = GetXtremeColor(COLOR_GRADIENTACTIVECAPTION);
	clrGradInActiveCapt         = GetXtremeColor(COLOR_GRADIENTINACTIVECAPTION);
	clrActiveCaptText           = GetXtremeColor(COLOR_CAPTIONTEXT);
	clrInactiveCaptText         = GetXtremeColor(COLOR_INACTIVECAPTIONTEXT);

	// Initialize special colors for XP style interfaces.
	clrXPBarFace                = GetXtremeColor(XPCOLOR_TOOLBAR_FACE);
	clrXPHighlight              = GetXtremeColor(XPCOLOR_HIGHLIGHT);
	clrXPHighlightBorder        = GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER);
	clrXPHighlightPushed        = GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED);
	clrXPIconShadow             = GetXtremeColor(XPCOLOR_ICONSHADDOW);
	clrXPGrayText               = GetXtremeColor(XPCOLOR_GRAYTEXT);
	clrXPHighlightChecked       = GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED);
	clrXPHighlightCheckedBorder = GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED_BORDER);
	clrXPGripper                = GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER);
	clrXPSeparator              = GetXtremeColor(XPCOLOR_SEPARATOR);
	clrXPDisabled               = GetXtremeColor(XPCOLOR_DISABLED);
	clrXPMenuTextBack           = GetXtremeColor(XPCOLOR_MENUBAR_FACE);
	clrXPMenuExpanded           = GetXtremeColor(XPCOLOR_MENUBAR_EXPANDED);
	clrXPMenuBorder             = GetXtremeColor(XPCOLOR_MENUBAR_BORDER);
	clrXPMenuText               = GetXtremeColor(XPCOLOR_MENUBAR_TEXT);
	clrXPHighlightText          = GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT);
	clrXPBarText                = GetXtremeColor(XPCOLOR_TOOLBAR_TEXT);
	clrXPBarTextPushed          = GetXtremeColor(XPCOLOR_PUSHED_TEXT);
	clrXPTabInactiveBack        = GetXtremeColor(XPCOLOR_TAB_INACTIVE_BACK);
	clrXPTabInactiveText        = GetXtremeColor(XPCOLOR_TAB_INACTIVE_TEXT);
}

void CXTAuxData::UpdateSysMetrics()
{
	cxSmIcon   = ::GetSystemMetrics(SM_CXSMICON);
	cySmIcon   = ::GetSystemMetrics(SM_CYSMICON);
	cxHThumb   = ::GetSystemMetrics(SM_CXHTHUMB);
	cyVThumb   = ::GetSystemMetrics(SM_CYVTHUMB);
	cyMenuItem = ::GetSystemMetrics(SM_CYMENU);
	cxSize     = 18; //::GetSystemMetrics(SM_CXSIZE);
	cySize     = 18; //::GetSystemMetrics(SM_CYSIZE);
}

BOOL CXTAuxData::LoadSysFonts(CString strHorzFaceName/*=_T("")*/, CString strVertFaceName/*=_T("")*/)
{
	CString strFaceTahoma(_T("Tahoma"));
	CString strFaceArial(_T("Arial"));

	// Free up any resources previously allocated.
	FreeSysFonts();

	CWindowDC dc(NULL);

	CXTLogFont lfMenuFont;
	CXTNonClientMetrics ncm;

	lfMenuFont.lfCharSet = (BYTE)::GetTextCharsetInfo (dc, NULL, 0);
	lfMenuFont.lfHeight  = ncm.lfMenuFont.lfHeight;
	lfMenuFont.lfWeight  = ncm.lfMenuFont.lfWeight;
	lfMenuFont.lfItalic  = ncm.lfMenuFont.lfItalic;

	STRCPY_S(lfMenuFont.lfFaceName, LF_FACESIZE, ncm.lfMenuFont.lfFaceName);

	if (!strHorzFaceName.IsEmpty())
	{
		if (FontExists(strHorzFaceName))
		{
			STRCPY_S(lfMenuFont.lfFaceName, LF_FACESIZE, strHorzFaceName);
			STRCPY_S(ncm.lfStatusFont.lfFaceName, LF_FACESIZE, strHorzFaceName);
			STRCPY_S(ncm.lfSmCaptionFont.lfFaceName, LF_FACESIZE, strHorzFaceName);
		}
	}

	// check to see if we can use the "Tahoma" font.
	else if (bUseOfficeFont && !(ncm.lfMenuFont.lfCharSet > SYMBOL_CHARSET))
	{
		if (FontExists(strFaceTahoma))
		{
			STRCPY_S(lfMenuFont.lfFaceName, LF_FACESIZE, strFaceTahoma);
		}
	}

	// create the menu fonts.
	if (!CreateSysFont(lfMenuFont, font))
		return FALSE;

	if (!CreateSysFont(lfMenuFont, fontBold, FW_BOLD))
		return FALSE;

	if (!CreateSysFont(lfMenuFont, fontULine, -1, 1))
		return FALSE;

	// If the "Tahoma" font is not used see if we can use "Arial" for vertical
	// fonts because it displays better than MS Sans Serif on older operating
	// systems...

	if (!strVertFaceName.IsEmpty())
	{
		if (FontExists(strVertFaceName))
		{
			STRCPY_S(lfMenuFont.lfFaceName, LF_FACESIZE, strVertFaceName);
		}
	}

	else if (strFaceTahoma.CompareNoCase(ncm.lfMenuFont.lfFaceName) != 0)
	{
		if (FontExists(strFaceArial))
		{
			STRCPY_S(lfMenuFont.lfFaceName, LF_FACESIZE, strFaceArial);
		}
	}

	// create the vertical menu fonts.
	if (!CreateSysFont(lfMenuFont, fontVert, -1, -1, 900, 2700))
		return FALSE;

	if (!CreateSysFont(lfMenuFont, fontVertBold, FW_BOLD, -1, 900, 2700))
		return FALSE;

	// create the icon title fonts.
	CXTLogFont lfIconTitleFont;
	VERIFY(::SystemParametersInfo(SPI_GETICONTITLELOGFONT,
		sizeof(LOGFONT), &lfIconTitleFont, 0));

	if (!CreateSysFont(lfIconTitleFont, fontIconTitle))
		return FALSE;

	if (!CreateSysFont(lfIconTitleFont, fontIconTitleBold, FW_BOLD))
		return FALSE;

	// Create the status bar font.
	if (!CreateSysFont(ncm.lfStatusFont, fontStatus))
		return FALSE;

	// Create the small caption font.
	if (!CreateSysFont(ncm.lfSmCaptionFont, fontSmCaption))
		return FALSE;

	// Create the marlett icon font.
	LOGFONT lfMarlettIcon = {lfIconTitleFont.lfHeight, 0, 0, 0, 0, 0, 0, 0,
		DEFAULT_CHARSET, 0, 0, 0, 0, _T("Marlett")};

	if (!CreateSysFont(lfMarlettIcon, fontMarlettIcon))
		return FALSE;

	return TRUE;
}

void CXTAuxData::FreeSysFonts()
{
	if (font.GetSafeHandle())
		font.DeleteObject();
	if (fontBold.GetSafeHandle())
		fontBold.DeleteObject();
	if (fontULine.GetSafeHandle())
		fontULine.DeleteObject();
	if (fontVert.GetSafeHandle())
		fontVert.DeleteObject();
	if (fontVertBold.GetSafeHandle())
		fontVertBold.DeleteObject();
	if (fontIconTitle.GetSafeHandle())
		fontIconTitle.DeleteObject();
	if (fontIconTitleBold.GetSafeHandle())
		fontIconTitleBold.DeleteObject();
	if (fontMarlettIcon.GetSafeHandle())
		fontMarlettIcon.DeleteObject();
	if (fontStatus.GetSafeHandle())
		fontStatus.DeleteObject();
	if (fontSmCaption.GetSafeHandle())
		fontSmCaption.DeleteObject();
}

void CXTAuxData::UseOfficeFont(BOOL bOfficeFont)
{
	bUseOfficeFont = bOfficeFont;
	LoadSysFonts();
}

int CALLBACK CXTAuxData::EnumFontFamExProc(ENUMLOGFONTEX* pelf,
	NEWTEXTMETRICEX*, int, LPVOID pThis)
{
	CString strFontName = *(CString*)pThis;
	return (strFontName.CompareNoCase(pelf->elfLogFont.lfFaceName) != 0);
}

BOOL CXTAuxData::FontExists(CString& strFaceName)
{
	// Enumerate all styles and character sets of all fonts:
	CXTLogFont lfEnum;
	lfEnum.lfFaceName[0] = 0;
	lfEnum.lfCharSet = DEFAULT_CHARSET;

	// returns TRUE if the specified font exists.
	CWindowDC dc(NULL);
	return (::EnumFontFamiliesEx(dc.m_hDC, &lfEnum,
		(FONTENUMPROC)EnumFontFamExProc, (LPARAM)&strFaceName, 0) == 0);
}

BOOL CXTAuxData::CreateSysFont(const CXTLogFont& logFont, CFont& font, long lfWeight/*=-1*/, char lfUnderline/*=-1*/, long lfOrientation/*=-1*/, long lfEscapement/*=-1*/)
{
	// delete the font if it has already been created.
	if (font.GetSafeHandle()) {
		font.DeleteObject();
	}

	// construct the log font.
	CXTLogFont lf(logFont);

	// set font weight.
	if (lfWeight != -1) {
		lf.lfWeight = lfWeight;
	}

	// set font orientation.
	if (lfOrientation != -1) {
		lf.lfOrientation = lfOrientation;
	}

	// set font escapement.
	if (lfEscapement != -1) {
		lf.lfEscapement = lfEscapement;
	}

	// set font underline style.
	if (lfUnderline != -1) {
		lf.lfUnderline = lfUnderline;
	}

	// create the font using the log font we constructed.
	return font.CreateFontIndirect(&lf);
}

BOOL CXTAuxData::SetGlobalFont(LPCTSTR lpszFaceName, LPCTSTR lpszVertFaceName/*=NULL*/)
{
	return LoadSysFonts(lpszFaceName, lpszVertFaceName);
}

BOOL CXTAuxData::SetGlobalFont(CFont* pHorzFont, CFont* pVertFont/*=NULL*/)
{
	if (pHorzFont != NULL && pHorzFont->GetSafeHandle() != NULL)
	{
		CXTLogFont lf;
		pHorzFont->GetLogFont(&lf);

		// create the menu fonts.
		if (!CreateSysFont(lf, font))
			return FALSE;

		if (!CreateSysFont(lf, fontBold, FW_BOLD))
			return FALSE;

		if (!CreateSysFont(lf, fontULine, -1, 1))
			return FALSE;

		if (!CreateSysFont(lf, fontStatus))
			return FALSE;
	}

	if (pVertFont != NULL && pVertFont->GetSafeHandle() != NULL)
	{
		CXTLogFont lf;
		pVertFont->GetLogFont(&lf);

		if (!CreateSysFont(lf, fontVert, -1, -1, 900, 2700))
			return FALSE;

		if (!CreateSysFont(lf, fontVertBold, FW_BOLD, -1, 900, 2700))
			return FALSE;
	}

	return TRUE;
}


DWORD CXTAuxData::GetComCtlVersion()
{
	//-----------------------------------------------------------------------------
	// Summary: DLLVERSIONINFOEX is a stand alone helper structure class.  It is used
	//          to store version information for a specified module.
	//-----------------------------------------------------------------------------
	struct DLLVERSIONINFOEX
	{
		DWORD cbSize;           // Size of the structure, in bytes. This member must be filled in before calling the function.
		DWORD dwMajorVersion;   // Major version of the DLL. If the DLL's version is 4.0.950, this  value will be 4.
		DWORD dwMinorVersion;   // Minor version of the DLL. If the DLL's version is 4.0.950, this  value will be 0.
		DWORD dwBuildNumber;    // Build number of the DLL. If the DLL's version is 4.0.950, this  value will be 950.

		// Identifies the platform for which the DLL was built. This can
		// be one of the following values:
		// DLLVER_PLATFORM_WINDOWS - The DLL was built for all Windows platforms.
		// DLLVER_PLATFORM_NT      - The DLL was built specifically for Windows NT.
		DWORD dwPlatformID;
	};

	typedef HRESULT (CALLBACK* PFNDLLGETVERSIONPROC)(DLLVERSIONINFOEX*);


	// return cached version if already determined...
	if (iComCtlVersion != -1)
		return iComCtlVersion;

	// otherwise determine comctl32.dll version via DllGetVersion
	HINSTANCE hInst = ::LoadLibrary(_T("COMCTL32.DLL"));
	ASSERT(hInst != NULL);

	PFNDLLGETVERSIONPROC pfn = (PFNDLLGETVERSIONPROC)
		::GetProcAddress(hInst, "DllGetVersion");

	if (pfn != NULL)
	{
		DLLVERSIONINFOEX dvi;
		::ZeroMemory(&dvi, sizeof(dvi));
		dvi.cbSize = sizeof(dvi);

		if (SUCCEEDED((*pfn)(&dvi)))
		{
			ASSERT(dvi.dwMajorVersion <= 0xFFFF);
			ASSERT(dvi.dwMinorVersion <= 0xFFFF);
			DWORD dwVersion = MAKELONG(dvi.dwMinorVersion, dvi.dwMajorVersion);

			iComCtlVersion = dwVersion;
		}
	}

	::FreeLibrary(hInst);

	return iComCtlVersion != -1? iComCtlVersion : 0;
}

BOOL CXTAuxData::IsComCtlValid() const
{
	if (iComCtlVersion < VERSION_IE401)
	{
#ifndef _XTP_INCLUDE_CONTROLS
		XT_MANAGE_STATE
		HINSTANCE hInstance = AfxGetInstanceHandle();

		TCHAR szMessage[256];
		::LoadString(hInstance, XT_IDS_INVALID_VER, szMessage, _countof(szMessage));
		::MessageBox(NULL, szMessage, _T("Comctl32.dll"), MB_ICONSTOP|MB_OK);
		::TerminateProcess(::GetCurrentProcess(), 1);
#endif
		return FALSE;
	}

	return TRUE;
}

CString CXTAuxData::GetXTVersion(bool bVerNumOnly/*=false*/)
{
	CString strVersion;
	if ( bVerNumOnly )
	{
#ifndef _XTP_INCLUDE_CONTROLS
		strVersion.Format( _T( "%d.%02d" ),
			_XTLIB_VERSION_MAJOR, _XTLIB_VERSION_MINOR );
#else
		strVersion.Format( _T( "%d.%02d" ),
			_XTPLIB_VERSION_MAJOR, _XTPLIB_VERSION_MINOR );
#endif
	}
	else
	{
#ifndef _XTP_INCLUDE_CONTROLS
		strVersion.Format( _T( "Xtreme Toolkit v%d.%02d" ),
			_XTLIB_VERSION_MAJOR, _XTLIB_VERSION_MINOR );
#else
		strVersion.Format( _T( "Xtreme Toolkit v%d.%02d" ),
			_XTPLIB_VERSION_MAJOR, _XTPLIB_VERSION_MINOR );
#endif
	}
	return strVersion;
}

void CXTAuxData::SetINIFileName(LPCTSTR lpszINIFullPath)
{
	strINIFileName = lpszINIFullPath;
}

LPCTSTR CXTAuxData::GetINIFileName()
{
	return strINIFileName;
}

void CXTAuxData::InitResources(HINSTANCE hInst)
{
	// Set the resource handle used by the library.
	CXTPManageState::SetModuleInfo(hInst, hInst);
	hInstance = hInst;

	LoadSysCursors(); // Load cursors.
}

_XT_EXT_CLASS CXTAuxData& AFXAPI XTAuxData()
{
	static CXTAuxData instance;
	return instance;
}
