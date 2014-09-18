// XTShellSettings.h: interface for the CXTShellSettings class.
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
#if !defined(__XTSHELLSETTINGS_H__)
#define __XTSHELLSETTINGS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTShellSettings is a SHELLFLAGSTATE struct derived class. CXTShellSettings
//     extends the SHELLFLAGSTATE struct for easy access to the shell flag
//     state settings.
//===========================================================================
class _XT_EXT_CLASS CXTShellSettings : public SHELLFLAGSTATE
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTShellSettings object
	//-----------------------------------------------------------------------
	CXTShellSettings();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTShellSettings object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTShellSettings();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Show All Files option is
	//     enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool ShowAllFiles();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Hide File Extensions for
	//     Known File Types option is disabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool ShowExtensions();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks to see if the Display Delete Confirmation
	//     Dialog box in the Recycle Bin is enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool NoConfirmRecycle();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Do Not Show Hidden Files
	//     option is selected.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool ShowSysFiles();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Display Compressed Files
	//     and Folders with Alternate Color option is enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool ShowCompColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Double-Click to Open an
	//     Item option is enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool DoubleClickOpen();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Active Desktop – View as
	//     Web Page option is enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool HTMLDesktop();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Classic Style option is
	//     enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool Win95Classic();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Allow All Uppercase Names
	//     option is enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool AllowUpperCase();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Show File Attributes in
	//     Detail View option is enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool ShowAttributes();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Show Map Network Drive
	//     Button in Toolbar option is enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool ShowMapNetworkDrive();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function checks whether or not the Show Info Tips for Items
	//     in Folders & Desktop option is enabled.
	// Returns:
	//     true if enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool ShowInfoTips();
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE bool CXTShellSettings::ShowAllFiles() {
	return fShowAllObjects != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::ShowExtensions() {
	return fShowExtensions != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::NoConfirmRecycle() {
	return fNoConfirmRecycle != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::ShowSysFiles() {
	return fShowSysFiles != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::ShowCompColor() {
	return fShowCompColor != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::DoubleClickOpen() {
	return fDoubleClickInWebView != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::HTMLDesktop() {
	return fDesktopHTML != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::Win95Classic() {
	return fWin95Classic != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::AllowUpperCase() {
	return fDontPrettyPath != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::ShowAttributes() {
	return fShowAttribCol != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::ShowMapNetworkDrive() {
	return fMapNetDrvBtn != 0 ? true : false;
}
AFX_INLINE bool CXTShellSettings::ShowInfoTips() {
	return fShowInfoTip != 0 ? true : false;
}

#endif // !defined(__XTSHELLSETTINGS_H__)
