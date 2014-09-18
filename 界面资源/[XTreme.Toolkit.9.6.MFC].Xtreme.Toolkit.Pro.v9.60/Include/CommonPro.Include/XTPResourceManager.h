// XTPResourceManager.h: interface for the CXTPResourceManager class.
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
#if !defined(__XTPRESOURCEMANAGER_H__)
#define __XTPRESOURCEMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTPXMLHelpers.h"

#pragma warning(disable: 4275) // ID: Q134980
#pragma warning(disable: 4251) // ID: Q134980

//===========================================================================
// Summary:
//     Standalone class used to managed toolkit resources.
//===========================================================================
class _XTP_EXT_CLASS CXTPResourceManager : public CNoTrackObject
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//    Constructs a CXTPResourceManager object.
	//-------------------------------------------------------------------------
	CXTPResourceManager();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads a Windows string resource, identified by nID, into an existing CString object
	// Parameters:
	//     pString     - Output string pointer
	//     nIDResource - A Windows string resource ID.
	// Returns:
	//     Nonzero if resource load was successful; otherwise 0.
	//-----------------------------------------------------------------------
	virtual BOOL LoadString(CString* pString, UINT nIDResource);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads a Windows menu resource, identified by nID, into an existing CMenu object
	// Parameters:
	//     lpMenu      - Output menu pointer
	//     nIDResource - A Windows menu resource ID.
	// Returns:
	//     Nonzero if resource load was successful; otherwise 0.
	//-----------------------------------------------------------------------
	virtual BOOL LoadMenu(CMenu* lpMenu, UINT nIDResource);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to load the toolbar specified by CToolBar and nIDResource.
	// Parameters:
	//     pToolBar    - Result CToolBar pointer
	//     nIDResource - Resource ID of the toolbar to be loaded.
	// Returns:
	//     Nonzero if resource load was successful; otherwise 0.
	//-----------------------------------------------------------------------
	virtual BOOL LoadToolBar(CToolBar* pToolBar, UINT nIDResource);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads a Windows bitmap resource, identified by nID, into an existing CBitmap object
	// Parameters:
	//     pBitmap     - Result CBitmap pointer
	//     nIDResource -  A Windows bitmap resource ID.
	// Returns:
	//     Nonzero if resource load was successful; otherwise 0.
	//-----------------------------------------------------------------------
	virtual BOOL LoadBitmap(CBitmap* pBitmap, UINT nIDResource);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads a Windows dialog resource, identified by pszResource
	// Parameters:
	//     pszResource - A Windows dialog resource ID.
	// Returns:
	//     LPCDLGTEMPLATE pointer if resource load was successful; otherwise 0.
	//-----------------------------------------------------------------------
	virtual LPCDLGTEMPLATE LoadDialogTemplate(LPCTSTR pszResource);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Displays a message box on the screen
	// Parameters:
	//     nIDPrompt - A unique ID used to reference a string in the string table.
	//     nType     - The style of the message box.
	// Returns:
	//     See AfxMessageBox return values list
	//-----------------------------------------------------------------------
	int ShowMessageBox(UINT nIDPrompt, UINT nType = MB_OK);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set application defined Resource manager
	// Parameters:
	//     pResourceManager - Application defined Resource manager
	// Returns:
	//     TRUE if successful; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL SetResourceManager(CXTPResourceManager* pResourceManager);

public:
	static CProcessLocal<CXTPResourceManager> m_managerInstance; // Manager instance.

protected:
	BOOL m_bValid;                      // TRUE if Resource manager is valid.

protected:
};

//===========================================================================
// Summary:
//     Class used to managed toolkit resources from text file
//===========================================================================
class _XTP_EXT_CLASS CXTPResourceManagerText : public CXTPResourceManager
{
private:
	struct MENUITEMTEMPLATEINFO;
	struct MENUTEMPLATEINFO;
	struct DLGTEMPLATEINFO;
	struct DLGITEMTEMPLATEINFO;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPResourceManagerText object.
	// Parameters:
	//     strFileName - Text file to be used for resources
	//-----------------------------------------------------------------------
	CXTPResourceManagerText(CString strFileName);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads a Windows string resource, identified by nID, into an existing CString object
	// Parameters:
	//     pString     - Output string pointer
	//     nIDResource - A Windows string resource ID.
	// Returns:
	//     Nonzero if resource load was successful; otherwise 0.
	//-----------------------------------------------------------------------
	virtual BOOL LoadString(CString* pString, UINT nIDResource);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads a Windows menu resource, identified by nID, into an existing CMenu object
	// Parameters:
	//     lpMenu      - Output menu pointer
	//     nIDResource - A Windows menu resource ID.
	// Returns:
	//     Nonzero if resource load was successful; otherwise 0.
	//-----------------------------------------------------------------------
	virtual BOOL LoadMenu(CMenu* lpMenu, UINT nIDResource);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads a Windows dialog resource, identified by pszResource
	// Parameters:
	//     pszResource - A Windows dialog resource ID.
	// Returns:
	//     LPCDLGTEMPLATE pointer if resource load was successful; otherwise 0.
	//-----------------------------------------------------------------------
	virtual LPCDLGTEMPLATE LoadDialogTemplate(LPCTSTR pszResource);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used by class to load as string specified type.
	// Parameters:
	//     pString         - Output string pointer
	//     nIDResource     - A Windows string resource ID.
	//     strResourceType - Resource type
	// Returns:
	//     Nonzero if resource load was successful; otherwise 0.
	//-----------------------------------------------------------------------
	virtual BOOL LoadResource(CString* pString, UINT nIDResource, CString strResourceType);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates DLGTEMPLATE from string.
	// Parameters:
	//     strTemplate - Template string
	// Returns:
	//     DLGTEMPLATE pointer if successful; otherwise 0.
	//-----------------------------------------------------------------------
	LPCDLGTEMPLATE CreateDialogTemplate(CString strTemplate);

private:
	BOOL CreateMenu(CMenu& menu, CString strTemplate);
	LPCDLGTEMPLATE CreateDialogTemplate(DLGTEMPLATEINFO& dlgTemplate);

protected:
	CString m_strFileName;          // Text file path.
};

#ifndef  _XTP_EXCLUDE_XML

//===========================================================================
// Summary:
//     Class used to managed toolkit resources from XML file
//===========================================================================
class _XTP_EXT_CLASS CXTPResourceManagerXML : public CXTPResourceManagerText
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPResourceManagerXML object.
	// Parameters:
	//     strFileName - XML file path
	//-----------------------------------------------------------------------
	CXTPResourceManagerXML(CString strFileName);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPResourceManagerXML object, handles cleanup and de-
	//     allocation.
	//-------------------------------------------------------------------------
	~CXTPResourceManagerXML();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used by class to load as string specified type.
	// Parameters:
	//     pString         - Output string pointer
	//     nIDResource     - A Windows string resource ID.
	//     strResourceType - Resource type
	// Returns:
	//     Nonzero if resource load was successful; otherwise 0.
	//-----------------------------------------------------------------------
	virtual BOOL LoadResource(CString* pString, UINT nIDResource, CString strResourceType);

private:
	BOOL LoadDocument(CString strFileName, CXTPDOMElementPtr& rootPtr, BOOL bCheckRootName = FALSE);
};

#endif

//-----------------------------------------------------------------------
// Summary:
//     Call this function to access CXTPResourceManager members.
//     Since this class is designed as a single instance object you can
//     only access version info thru this method. You <b>cannot</b>
//     directly instantiate an object of type CXTPResourceManager.
// Example:
// <code>
// XTPResourceManager()->LoadString(&m_strNoFieldsAvailable,
//     XTP_IDS_REPORT_NOFIELDSAVAILABLE);
// </code>
//-----------------------------------------------------------------------
_XTP_EXT_CLASS CXTPResourceManager* AFX_CDECL XTPResourceManager();


#endif // !defined(__XTPRESOURCEMANAGER_H__)
