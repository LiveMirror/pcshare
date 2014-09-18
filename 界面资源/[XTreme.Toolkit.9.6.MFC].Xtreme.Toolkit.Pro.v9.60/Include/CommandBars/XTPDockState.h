// XTPDockState.h : interface for the CXTPDockState class.
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
#if !defined(__XTPDOCKSTATE_H__)
#define __XTPDOCKSTATE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPToolbar.h"

//===========================================================================
// Summary:
//     CXTPDockState is a stand alone class. It is used internally by CXTPToolBar.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockState
{
	friend class CXTPCommandBars;

private:
	//-----------------------------------------------------------------------
	// Summary:
	//     TOOLBARINFO definition
	//-----------------------------------------------------------------------
	typedef CXTPToolBar::CToolBarInfo TOOLBARINFO;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Maps the point from the stored coordinates to the device.
	// Parameters:
	//     pt - Specifies the point to be tested
	//-----------------------------------------------------------------------
	void ScalePoint(CPoint& pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Maps the point from the stored coordinates to the device.
	// Parameters:
	//     rect - A reference to a CRect object
	//-----------------------------------------------------------------------
	void ScaleRectPos(CRect& rect);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to retrieve the version number of the stored bar state
	//-----------------------------------------------------------------------
	DWORD GetVersion();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockState object.
	//-------------------------------------------------------------------------
	CXTPDockState(void);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockState object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	~CXTPDockState(void);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to save the state information to the registry or .INI file
	// Parameters:
	//     lpszProfileName - Points to a null-teminated string that specifies the name of a section
	//                       in the initialization file or a key in the Windows registry where state information is stored.
	//-----------------------------------------------------------------------
	void SaveState(LPCTSTR lpszProfileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to retrieve state information from the registry or .INI file.
	// Parameters:
	//     lpszProfileName - Points to a null-teminated string that specifies the name of a section in the initialization
	//                       file or a key in the Windows registry where state information is stored.
	//-----------------------------------------------------------------------
	void LoadState(LPCTSTR lpszProfileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes options from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	// See Also: CXTPCommandBars::DoPropExchange
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

private:
	void SaveDockBarState(LPCTSTR lpszProfileName);
	CSize GetScreenSize();
	void SetScreenSize(CSize& size);
	void Clear();


private:
	DWORD m_dwVersion;
	CArray<TOOLBARINFO*, TOOLBARINFO*> m_arrBarInfo;
	BOOL m_bScaling;
	CRect m_rectDevice;
	CSize m_sizeLogical;
	CRect m_rectClip;

};

AFX_INLINE DWORD CXTPDockState::GetVersion() {
	return m_dwVersion;
}


#endif //#if !defined(__XTPDOCKSTATE_H__)
