// XTPShortcutManager.h : interface for the CXTPShortcutManager class.
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
#if !defined(__XTPSHORTCUTMANAGER_H__)
#define __XTPSHORTCUTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPCommandBars;
class CXTPPropExchange;

//===========================================================================
// Summary:
//     CXTPShortcutManager is standalone class used to manipulate accelerators of frame.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutManager: public CNoTrackObject
{
public:

//{{AFX_CODEJOCK_PRIVATE
	class _XTP_EXT_CLASS CKeyHelper
	{
	public:

		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CKeyHelper object.
		//-----------------------------------------------------------------------
		CKeyHelper(LPACCEL lpAccel);

		//-------------------------------------------------------------------------
		// Summary:
		//     Destroys a CKeyHelper object, handles cleanup and deallocation
		//-------------------------------------------------------------------------
		virtual ~CKeyHelper();

	public:

		void Format (CString& str) const;
		int Priority();

		static BOOL EqualAccels(const ACCEL* pFirst, const ACCEL* pSecond)
		{
			return ((pFirst->fVirt | FNOINVERT) == (pSecond->fVirt | FNOINVERT)) && (pFirst->key == pSecond->key);
		}

	protected:
		void AddVirtKeyStr (CString& str, UINT uiVirtKey, BOOL bLast = FALSE) const;

	protected:
		LPACCEL m_lpAccel;
	};

	class _XTP_EXT_CLASS CKeyAssign : public CEdit
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CKeyAssign object.
		//-------------------------------------------------------------------------
		CKeyAssign();

		//-------------------------------------------------------------------------
		// Summary:
		//     Destroys a CKeyAssign object, handles cleanup and deallocation
		//-------------------------------------------------------------------------
		virtual ~CKeyAssign();

	public:
		BOOL IsKeyDefined() const;
		ACCEL const* GetAccel() const;
		void SetAccel(ACCEL& hAccel);
		void ResetKey();

	protected:
		void SetAccelFlag (BYTE bFlag, BOOL bOn);
		virtual BOOL PreTranslateMessage(MSG* pMsg);

	protected:
		BOOL        m_bKeyDefined;
		ACCEL       m_hAccel;
		CKeyHelper  m_keyHelper;

	};
//}}AFX_CODEJOCK_PRIVATE

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutManager object.
	//-------------------------------------------------------------------------
	CXTPShortcutManager();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves shortcut text for specified command.
	// Parameters:
	//     nCmd         - Command which shortcut text need to retrieve
	//     strShortcut  - Shortcut text reference.
	//     pCommandBars - Pointer to a CXTPCommandBars object.
	// Returns:
	//     TRUE if shortcut found.
	//-----------------------------------------------------------------------
	virtual BOOL FindDefaultAccelerator (UINT nCmd, CString& strShortcut, CXTPCommandBars* pCommandBars = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to retrieve text presentation of ACCEL.
	//     You can override this method.
	// Parameters:
	//     lpAccel - Accelerator need to format.
	//     pPriority - Returns priority of accelerator to be used to determine what accelerator used as default.
	//-----------------------------------------------------------------------
	virtual CString Format(LPACCEL lpAccel, int* pPriority);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to replace default shortcut manager.
	// Parameters:
	//     pShortcutManager - New shortcut manager pointe to be used
	//-----------------------------------------------------------------------
	void SetShortcutManager(CXTPShortcutManager* pShortcutManager);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to save the accelerators to the registry
	//     or .INI file.
	// Parameters:
	//     pFrame          - Frame which accelerators need to store
	//     lpszProfileName - Points to a null-terminated string that
	//                       specifies the name of a section in the
	//                       initialization file or a key in the Windows
	//                       registry where state information is stored.
	//-----------------------------------------------------------------------
	void SaveShortcuts(CFrameWnd* pFrame, LPCTSTR lpszProfileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to retrieve accelerators from the registry
	//     or .INI file.
	// Parameters:
	//     pFrame          - Frame which accelerators need to restore
	//     lpszProfileName - Points to a null-terminated string that
	//                       specifies the name of a section in the
	//                       initialization file or a key in the Windows
	//                       registry where state information is stored.
	//-----------------------------------------------------------------------
	void LoadShortcuts(CFrameWnd* pFrame, LPCTSTR lpszProfileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX    - A CXTPPropExchange object to serialize to or from.
	//     pFrame - Frame which accelerators need to serialize
	//----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX, CFrameWnd* pFrame);


public:
//{{AFX_CODEJOCK_PRIVATE
	void UpdateAcellTable(CFrameWnd* pFrame, LPACCEL lpAccel, int nSize);
	TCHAR ToUpper(TCHAR vkTCHAR);
	void SerializeShortcuts(CFrameWnd* pFrame, CArchive& ar);
	HACCEL CopyAccelTable(HACCEL hAccelTable);
	void AddShortcut(CFrameWnd* pFrame, long fVirt, long key, long cmd);
	ACCEL * GetShortcut(CFrameWnd* pFrame, int ID, ACCEL * pAccel);

public:
	static CProcessLocal<CXTPShortcutManager> m_managerInstance;
//}}AFX_CODEJOCK_PRIVATE
};

//-------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Summary:
//     Call this member function to access XTPShortcutManager members.
//     Since this class is designed as a single instance object you can
//     only access version info through this method. You <b>cannot</b>
//     directly instantiate an object of type CXTPShortcutManager.
// Example:
//     <code>XTPShortcutManager()->SaveShortcuts(this, _T("Shortcuts"));</code>
//---------------------------------------------------------------------------
_XTP_EXT_CLASS CXTPShortcutManager* AFX_CDECL XTPShortcutManager();

#endif //#if !defined(__XTPSHORTCUTMANAGER_H__)
