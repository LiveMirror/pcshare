// LangManager.cpp: implementation of the CLanguageManger class.
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
#include "LangManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLanguageManger::CLanguageManger()
: m_wLangID(0)
, m_hModule(0)
{
	m_wLangID = (WORD)AfxGetApp()->GetProfileInt(_T("Settings"),
		_T("Current Language"), MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));

	InitLanguageDlls();
}

CLanguageManger::~CLanguageManger()
{
	if (m_hModule)
		::FreeLibrary(m_hModule);

	AfxGetApp()->WriteProfileInt(_T("Settings"),
		_T("Current Language"), m_wLangID);
}

struct LANGDEF
{
	UINT nID;
	WORD wLangID;
};

const LANGDEF languages[] =
{
	{ XTP_IDC_LANG_ARABIC,         MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_SAUDI_ARABIA) },
	{ XTP_IDC_LANG_CHINESE_PRC,    MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED) },
	{ XTP_IDC_LANG_CHINESE_TAIWAN, MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL) },
	{ XTP_IDC_LANG_CROATIAN,       MAKELANGID(LANG_CROATIAN, SUBLANG_DEFAULT) },
	{ XTP_IDC_LANG_CZECH,          MAKELANGID(LANG_CZECH, SUBLANG_DEFAULT) },
	{ XTP_IDC_LANG_DANISH,         MAKELANGID(LANG_DANISH, SUBLANG_DEFAULT) },
	{ XTP_IDC_LANG_DUTCH,          MAKELANGID(LANG_DUTCH, SUBLANG_DUTCH) },
	{ XTP_IDC_LANG_ENGLISH,        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) },
	{ XTP_IDC_LANG_ESTONIAN,       MAKELANGID(LANG_ESTONIAN, SUBLANG_DEFAULT) },
	{ XTP_IDC_LANG_FINNISH,        MAKELANGID(LANG_FINNISH, SUBLANG_DEFAULT) },
	{ XTP_IDC_LANG_FRENCH,         MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH) },
	{ XTP_IDC_LANG_GERMAN,         MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN) },
	{ XTP_IDC_LANG_GREEK,          MAKELANGID(LANG_GREEK, SUBLANG_DEFAULT) },
	{ XTP_IDC_LANG_HEBREW,         MAKELANGID(LANG_HEBREW, SUBLANG_DEFAULT) },
	{ XTP_IDC_LANG_HUNGARIAN,      MAKELANGID(LANG_HUNGARIAN, SUBLANG_DEFAULT) },
	{ XTP_IDC_LANG_ITALIAN,        MAKELANGID(LANG_ITALIAN, SUBLANG_ITALIAN) },
	{ XTP_IDC_LANG_JAPANESE,       MAKELANGID(LANG_JAPANESE, SUBLANG_DEFAULT) },
	{ XTP_IDC_LANG_KOREAN,         MAKELANGID(LANG_KOREAN, SUBLANG_DEFAULT) },
	{ XTP_IDC_LANG_LATVIAN,        MAKELANGID(LANG_LATVIAN, SUBLANG_DEFAULT) },
	{ XTP_IDC_LANG_LITHUANIAN,     MAKELANGID(LANG_LITHUANIAN, SUBLANG_LITHUANIAN) },
	{ XTP_IDC_LANG_NORWEGIAN,      MAKELANGID(LANG_NORWEGIAN, SUBLANG_NORWEGIAN_BOKMAL) },
	{ XTP_IDC_LANG_POLISH,         MAKELANGID(LANG_POLISH,  SUBLANG_DEFAULT) },
	{ XTP_IDC_LANG_PORTUGUESE_BR,  MAKELANGID(LANG_PORTUGUESE, SUBLANG_PORTUGUESE_BRAZILIAN) },
	{ XTP_IDC_LANG_PORTUGUESE,     MAKELANGID(LANG_PORTUGUESE, SUBLANG_PORTUGUESE) },
	{ XTP_IDC_LANG_ROMANIAN,       MAKELANGID(LANG_ROMANIAN, SUBLANG_DEFAULT) },
	{ XTP_IDC_LANG_RUSSIAN,        MAKELANGID(LANG_RUSSIAN, SUBLANG_DEFAULT) },
	{ XTP_IDC_LANG_SLOVAK,         MAKELANGID(LANG_SLOVAK, SUBLANG_DEFAULT) },
	{ XTP_IDC_LANG_SLOVENIAN,      MAKELANGID(LANG_SLOVENIAN, SUBLANG_DEFAULT) },
	{ XTP_IDC_LANG_SPANISH,        MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_MODERN) },
	{ XTP_IDC_LANG_SWEDISH,        MAKELANGID(LANG_SWEDISH, SUBLANG_DEFAULT) },
	{ XTP_IDC_LANG_THAI,           MAKELANGID(LANG_THAI, SUBLANG_DEFAULT) },
	{ XTP_IDC_LANG_UKRAINIAN,      MAKELANGID(LANG_UKRAINIAN, SUBLANG_DEFAULT) },
};

BOOL CLanguageManger::GetDllInfo(const WORD& wLangId, LANGUAGE_DLL& langDll)
{
	return m_mapLangDll.Lookup(wLangId, langDll);
}

WORD CLanguageManger::GetLangID(HMODULE hResDll) const
{
	typedef WORD (CALLBACK* PGETLANGID)();

	if (hResDll != NULL)
	{
		PGETLANGID pfnGetLangID = (PGETLANGID)::GetProcAddress(
				hResDll, "GetLangID");

		if (pfnGetLangID)
		{
			return (*pfnGetLangID)();
		}
	}
	return 0;
}

WORD CLanguageManger::GetLangID(UINT uCmdID) const
{
	for (int i = 0; i < _countof(languages); ++i)
	{
		if (uCmdID == languages[i].nID)
		{
			return languages[i].wLangID;
		}
	}
	return 0;
}

UINT CLanguageManger::GetCmdID(const WORD& wLangId) const
{
	for (int i = 0; i < _countof(languages); ++i)
	{
		if (wLangId == languages[i].wLangID)
		{
			return languages[i].nID;
		}
	}
	return 0;
}

CString CLanguageManger::GetDescription(const WORD& wLangId) const
{
	for (int i = 0; i < _countof(languages); ++i)
	{
		if (wLangId == languages[i].wLangID)
		{
			CString strBuffer;
			strBuffer.LoadString(languages[i].nID);
			return strBuffer;
		}
	}
	return _T("");
}

BOOL CLanguageManger::InitLanguageDlls()
{
	TCHAR szBuff[_MAX_PATH];
	::GetModuleFileName(NULL, szBuff, _countof(szBuff));

	TCHAR *p = _tcsrchr(szBuff, _T('\\'));
	if (p != NULL) {
		*p = _T('\0');
	}

	CString strDllPath;
	strDllPath.Format(_T("%s\\Translations\\*.dll"), szBuff);

	return InitLanguageDlls(strDllPath);
}

BOOL CLanguageManger::InitLanguageDlls(CString strDllPath)
{
	m_mapLangDll.RemoveAll();

	CFileFind ff;
	BOOL bContinue = ff.FindFile(strDllPath);

	while (bContinue)
	{
		bContinue = ff.FindNextFile();

		if (ff.IsDots())
			continue;

		if (ff.IsDirectory())
			continue;

		CString strDllPath(ff.GetFilePath());

		HMODULE hModule = ::LoadLibrary(strDllPath);
		if (hModule)
		{
			WORD wLangID = GetLangID(hModule);
			if (wLangID != 0)
			{
				LANGUAGE_DLL langDll;
				langDll.uCmdID = GetCmdID(wLangID);
				langDll.strDllPath = strDllPath;
				m_mapLangDll[wLangID] = langDll;
			}

			::FreeLibrary(hModule);
		}
	}

	ff.Close();

	return TRUE;
}

BOOL CLanguageManger::IsRTL() const
{
	return (
		(m_wLangID == MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_SAUDI_ARABIA)) ||
		(m_wLangID == MAKELANGID(LANG_HEBREW, SUBLANG_DEFAULT)));
}

WORD CLanguageManger::GetLangID() const
{
	return m_wLangID;
}

BOOL CLanguageManger::LoadLanguageDll(LPCTSTR lpszDllPath)
{
	if (m_hModule)
	{
		::FreeLibrary(m_hModule);
		m_hModule = 0;
	}

	m_hModule = ::LoadLibrary(lpszDllPath);
	m_wLangID = GetLangID(m_hModule);

	CXTPManageState::ResetModuleInfo(m_hModule, m_hModule);

	return (m_hModule != NULL);
}

BOOL CLanguageManger::LoadLanguageDll(const WORD& wLangId)
{
	LANGUAGE_DLL langDll;
	if (m_mapLangDll.Lookup(wLangId, langDll))
	{
		return LoadLanguageDll(langDll.strDllPath);
	}

	return FALSE;
}

CMapLangDll& CLanguageManger::GetLangMap()
{
	return m_mapLangDll;
}

void CLanguageManger::LayoutWindow(CWnd* pWnd, BOOL bRTLLayout)
{
	pWnd->ModifyStyleEx(bRTLLayout? 0: WS_EX_LAYOUTRTL, !bRTLLayout? 0: WS_EX_LAYOUTRTL);

	// walk through HWNDs to avoid creating temporary CWnd objects
	// unless we need to call this function recursively
	for (CWnd* pChild = pWnd->GetWindow(GW_CHILD); pChild != NULL;
		pChild = pChild->GetWindow(GW_HWNDNEXT))
	{

		// send to child windows after parent
		LayoutWindow(pChild, bRTLLayout);
	}
}

void CLanguageManger::SetRTLLayout(CWnd* pWnd, BOOL bRTLLayout)
{
	if ((BYTE)::GetVersion() < 4)
		return;

	LayoutWindow(pWnd, bRTLLayout);

	// then update the state of all floating windows owned by the parent
	HWND hWnd = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
	while (hWnd != NULL)
	{
		if (pWnd->m_hWnd == ::GetParent(hWnd))
			LayoutWindow(CWnd::FromHandle(hWnd), bRTLLayout);

		hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
	}
}
