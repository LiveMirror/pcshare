// XTVersionManager.h: interface for the CXTOSVersionInfo class.
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
#if !defined(__XTVERSIONMANAGER_H__)
#define __XTVERSIONMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTOSVersionInfo;
// -----------------------------------------------------------------------
// Summary:
//     The XTOSVersionInfo function is used for access to the
//     CXTOSVersionInfo class.
// Remarks:
//     Call this member function to access CXTOSVersionInfo members. Since
//     this class is designed as a single instance object you can only
//     access its members through this method. You cannot directly
//     instantiate an object of type CXTOSVersionInfo.
// Returns:
//     A pointer to the one and only CXTOSVersionInfo instance.
// Example:
// <code>
// bool bIsWinNT = XTOSVersionInfo()-\>IsWinNT4();
// </code>
// See Also:
//     CXTOSVersionInfo
// -----------------------------------------------------------------------
_XT_EXT_CLASS CXTOSVersionInfo* AFXAPI XTOSVersionInfo();

//===========================================================================
// Summary:
//     CXTOSVersionInfo is a OSVERSIONINFO derived class. This class
//     wraps the Win32 API GetVersionEx(...), used to get the current
//     Windows OS version. CXTOSVersionInfo is a single instance, or
//     "singleton" object, that is accessed with the Get() method.
//===========================================================================
class _XT_EXT_CLASS CXTOSVersionInfo : public OSVERSIONINFO
{
private:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTOSVersionInfo object.
	//-----------------------------------------------------------------------
	CXTOSVersionInfo();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to check whether or not the
	//     operating system is Windows 3.1.
	// Returns:
	//     true if the OS is Windows 3.1, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWin31() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to check whether or not the
	//     operating system is Windows 95.
	// Returns:
	//     true if the OS is Windows 95, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWin95() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to check whether or not the
	//     operating system is Windows 98.
	// Returns:
	//     true if the OS is Windows 98, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWin98() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to check whether or not the
	//     operating system is Windows ME.
	// Returns:
	//     true if the OS is Windows ME, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWinME() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to check whether or not the
	//     operating system is Windows NT 4.
	// Returns:
	//     true if the OS is Windows NT 4, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWinNT4() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to check whether or not the
	//     operating system is Windows 2000.
	// Returns:
	//     true if the OS is Windows 2000, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWin2K() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to check whether or not the
	//     operating system is Windows XP.
	// Returns:
	//     true if the OS is Windows XP, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWinXP() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to check whether or not the
	//     operating system is greater than or equal to Windows 3.1.
	// Returns:
	//     true if the OS is greater than or equal to Windows 3.1,
	//     otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWin31OrGreater() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to check whether or not the
	//     operating system is of the Windows 9x family, and if it is
	//     Windows 95 or a later version.
	// Returns:
	//     true if the OS is of the Windows 9x family, and is Windows 95
	///    or a later version, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWin95OrGreater() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to check whether or not the
	//     operating system is of the Windows 9x family, and if it is
	//     Windows 98 or a later version.
	// Returns:
	//     true if the OS is of the Windows 9x family, and is Windows 98
	//     or a later version, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWin98OrGreater() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to check whether or not the
	//     operating system is of the Windows 9x family, and if it is
	//     Windows ME or a later version.
	// Returns:
	//     true if the OS is of the Windows 9x family, and is Windows ME
	//     or a later version, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWinMEOrGreater() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to check whether or not the
	//     operating system is of the Windows NT family, and if it is
	//     Windows NT 4 or a later version.
	// Returns:
	//     true if the OS is of the Windows NT family, and is Windows NT
	//     4 or a later version, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWinNT4OrGreater() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to check whether or not the
	//     operating system is of the Windows NT family, and if it is
	//     Windows 2000 or a later version.
	// Returns:
	//     true if the OS is of the Windows NT family, and is Windows
	//     2000 or a later version, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWin2KOrGreater() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to check whether or not the
	//     operating system is of the Windows NT family, and if it is
	//     Windows XP or a later version.
	// Returns:
	//     true if the OS is of the Windows NT family, and is Windows XP
	//     or a later version, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsWinXPOrGreater() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve a CString object that
	//     represents the current version of the Xtreme Toolkit.  The
	//     string returned is formatted like so: "Xtreme Toolkit v1.94".
	// Parameters:
	//     bVerNumOnly - true to return the version number only, minus
	//                   "Xtreme Toolkit v".
	// Returns:
	//     A NULL terminated string that indicates the version of the
	//     Xtreme Toolkit.
	//-----------------------------------------------------------------------
	CString GetLibVersion(bool bVerNumOnly=false);

private:

	//-----------------------------------------------------------------------
	// Summary:
	//     Checks to see if the OS is greater or equal to the specified
	//     version number
	// Returns:
	//     true if the OS is greater than or equal to the specified
	//     version.
	//-----------------------------------------------------------------------
	bool GreaterThanEqualTo(const DWORD maj, const DWORD min) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Checks to see if the OS is equal to the specified version
	//     number
	// Returns:
	//     true if the OS is equal to the specified version.
	//-----------------------------------------------------------------------
	bool EqualTo(const DWORD maj, const DWORD min) const;

private:
	static CXTOSVersionInfo m_instance;  // singleton instantiates on demand.


	friend _XT_EXT_CLASS CXTOSVersionInfo* AFXAPI XTOSVersionInfo();
};

#endif // !defined(__XTVERSIONMANAGER_H__)
