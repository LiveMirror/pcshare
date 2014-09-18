// XTPSoundManager.h : interface for the CXTPSoundManager class.
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
#if !defined(__XTPSOUNDMANAGER_H__)
#define __XTPSOUNDMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//-----------------------------------------------------------------------
// Summary:
//     Enumeration used to play system sound.
// Example:
//     <code>XTPSoundManager()->PlaySystemSound(xtpSoundMenuCommand);</code>
// See Also: CXTPSoundManager
//
// <KEYWORDS xtpSoundNone, xtpSoundMenuCommand, xtpSoundMenuPopup, xtpSoundMenuTerminate>
//-----------------------------------------------------------------------
enum XTPSoundManagerState
{
	xtpSoundNone,           // No Sound (default)
	xtpSoundMenuCommand,    // Menu command system sound
	xtpSoundMenuPopup,      // Menu popup system sound
	xtpSoundMenuTerminate,  // Stop sound thread (internal usage)
};

//===========================================================================
// Summary:
//     CXTPSoundManager is standalone class. It used to play system sound.
// See Also: XTPSoundManagerState, XTPSoundManager
//===========================================================================
class _XTP_EXT_CLASS CXTPSoundManager
{
	friend class CXTPSoundManager* XTPSoundManager();

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPSoundManager object.
	//-------------------------------------------------------------------------
	CXTPSoundManager(void);
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSoundManager object, handles cleanup and deallocation
	//-------------------------------------------------------------------------
	~CXTPSoundManager(void);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to play system sound
	// Parameters:
	//     state - System sound to play. Can be any of the values listed in
	//             the remarks section.
	// Remarks:
	//     State parameter can be one of the following:
	//     * <b>xtpSoundMenuCommand</b> Play menu command system sound
	//     * <b>xtpSoundMenuPopup</b> Play menu popup system sound
	//-----------------------------------------------------------------------
	void PlaySystemSound(XTPSoundManagerState state);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to disable all system sounds for application
	// Parameters:
	//     bEnable - FALSE to disable system sounds.
	// Remarks:
	//     Call this method to force system settings and disable all sounds.
	//-----------------------------------------------------------------------
	void EnableSystemSounds(BOOL bEnable);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to stop all sounds and terminate sound thread.
	//-------------------------------------------------------------------------
	void StopThread();

private:
	void StartThread();
	static DWORD WINAPI SoundThreadProc(LPVOID);

private:
	XTPSoundManagerState m_soundState;
	BOOL m_bSystemSounds;
	HANDLE m_hThread;

	static CXTPSoundManager s_managerInstance;

};

//---------------------------------------------------------------------------
// Summary:
//     Call this member function to access CXTPSoundManager members.
//     Since this class is designed as a single instance object you can
//     only access version info through this method. You <b>cannot</b>
//     directly instantiate an object of type CXTPSoundManager.
// Example:
//     <code>XTPSoundManager()->PlaySystemSound(xtpSoundMenuCommand);</code>
//---------------------------------------------------------------------------
CXTPSoundManager* XTPSoundManager();



AFX_INLINE CXTPSoundManager* XTPSoundManager() {
	return &CXTPSoundManager::s_managerInstance;
}


#endif //#if !defined(__XTPSOUNDMANAGER_H__)
