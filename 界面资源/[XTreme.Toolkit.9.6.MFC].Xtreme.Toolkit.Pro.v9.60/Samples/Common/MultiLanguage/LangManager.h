// LangManager.h: interface for the CLanguageManger class.
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

#if !defined(__LANGMANAGER_H__)
#define __LANGMANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct LANGUAGE_DLL
{
	UINT    uCmdID;
	CString strDllPath;
};

typedef CMap<WORD,WORD,LANGUAGE_DLL,LANGUAGE_DLL&> CMapLangDll;

class CLanguageManger
{
public:
	CLanguageManger();
	virtual ~CLanguageManger();

protected:
	WORD        m_wLangID;
	HMODULE     m_hModule;
	CMapLangDll m_mapLangDll;

public:
	BOOL InitLanguageDlls();
	BOOL InitLanguageDlls(CString strDllPath);
	BOOL LoadLanguageDll(LPCTSTR lpszDllPath);
	BOOL LoadLanguageDll(const WORD& wLangId);
	BOOL IsRTL() const;
	BOOL GetDllInfo(const WORD& wLangId, LANGUAGE_DLL& langDll);
	WORD GetLangID() const;
	WORD GetLangID(UINT nCmdID) const;
	WORD GetLangID(HMODULE hResDll) const;
	void SetRTLLayout(CWnd* pWnd, BOOL bRTLLayout);
	UINT GetCmdID(const WORD& wLangId) const;
	CString GetDescription(const WORD& wLangId) const;
	CMapLangDll& GetLangMap();

protected:
	void LayoutWindow(CWnd* pWnd, BOOL bRTLLayout);
};

#endif // !defined(__LANGMANAGER_H__)
