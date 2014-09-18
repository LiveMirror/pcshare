// CSplitPath : header file
/////////////////////////////////////////////////////////////////////////////
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

#ifndef __SPLITPATH_H
#define __SPLITPATH_H

/////////////////////////////////////////////////////////////////////////////
// CSplitPath class

class CSplitPath
{
// Construction / destruction
public:

	// Constructs a CSplitPath object.
	CSplitPath(LPCTSTR lpszPathBuffer=NULL);

	// Destroys a CSplitPath object, handles cleanup and deallocation.
	virtual ~CSplitPath();

// Member variables
protected:
	TCHAR m_szDrive[_MAX_DRIVE];
	TCHAR m_szDir[_MAX_DIR];
	TCHAR m_szFName[_MAX_FNAME];
	TCHAR m_szExt[_MAX_EXT];

	// Member functions
public:
	void SplitPath(LPCTSTR lpszPathBuffer);
	CString GetDrive() const;
	CString GetDir() const;
	CString GetFName() const;
	CString GetExt() const;
	CString GetFullPath() const;
	CString GetFullName() const;
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE CString CSplitPath::GetDrive() const {
	return CString(m_szDrive);
}
AFX_INLINE CString CSplitPath::GetDir() const {
	return CString(m_szDir);
}
AFX_INLINE CString CSplitPath::GetFName() const {
	return CString(m_szFName);
}
AFX_INLINE CString CSplitPath::GetExt() const {
	return CString(m_szExt);
}
AFX_INLINE CString CSplitPath::GetFullPath() const {
	return GetDrive() + GetDir();
}
AFX_INLINE CString CSplitPath::GetFullName() const {
	return GetFName() + GetExt();
}

/////////////////////////////////////////////////////////////////////////////

#endif // __SPLITPATH_H
