// key.h : header file
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

/////////////////////////////////////////////////////////////////////////////
// CKey

class CKey
{
public:
	CKey() {m_hKey = NULL;}
	~CKey() {Close();}

// Attributes
public:
	HKEY m_hKey;
	BOOL SetStringValue(LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL);
	BOOL GetStringValue(CString& str, LPCTSTR lpszValueName = NULL);

// Operations
public:
	BOOL Create(HKEY hKey, LPCTSTR lpszKeyName);
	BOOL Open(HKEY hKey, LPCTSTR lpszKeyName);
	void Close();

// Overrides

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////
