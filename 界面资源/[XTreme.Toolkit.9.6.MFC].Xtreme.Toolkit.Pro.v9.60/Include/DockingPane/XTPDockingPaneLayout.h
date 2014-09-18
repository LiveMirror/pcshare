// XTPDockingPaneLayout.h : interface for the CXTPDockingPaneLayout class.
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
#if !defined(__XTPDOCKINGPANELAYOUT_H__)
#define __XTPDOCKINGPANELAYOUT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPDockingPaneDefines.h"
#include "CommonPro.Include/XTPXMLHelpers.h"


class CXTPDockingPaneBase;
class CXTPDockingPaneSplitterContainer;
class CXTPDockingPaneBase;
class CXTPDockingPaneAutoHidePanel;
class CXTPDockingPaneManager;
class CXTPPropExchange;

//===========================================================================
// Summary:
//     CXTPDockingPaneLayout is a stand alone class. It is used to manipulate
//     a pane's layouts.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneLayout
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneLayout object.
	// Parameters:
	//     pManager - Points to a CXTPDockingPaneManager object
	// Returns:
	//     A CXTPDockingPaneLayout object.
	//-----------------------------------------------------------------------
	CXTPDockingPaneLayout(CXTPDockingPaneManager* pManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneLayout object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPDockingPaneLayout();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to copy one layout to another.
	// Parameters:
	//     pLayout - Source Layout.
	//-----------------------------------------------------------------------
	void Copy(CXTPDockingPaneLayout* pLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to save the layout to the registry.
	// Parameters:
	//     strSection - Registry section.
	//-----------------------------------------------------------------------
	void Save(CString strSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to load the layout from the registry.
	// Parameters:
	//     strSection - Registry section.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL Load(CString strSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves list of layout's panes.
	// Returns:
	//     A list of the layout's panes
	// See Also:
	//     CXTPDockingPaneInfoList
	//-----------------------------------------------------------------------
	CXTPDockingPaneInfoList& GetPaneList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     ar - A CArchive object to serialize to or from.
	//-----------------------------------------------------------------------
	void Serialize(CArchive& ar);

#ifndef  _XTP_EXCLUDE_XML

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to save the layout to the file.
	// Parameters:
	//     strFileName - The full path and file name.
	//     strSection  - Registry section.
	//-----------------------------------------------------------------------
	void SaveToFile(CString strFileName, CString strSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to load the layout from the file.
	// Parameters:
	//     strFileName - The full path and file name.
	//     strSection  - Registry section.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL LoadFromFile(CString strFileName, CString strSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to save the layout to the XML node.
	// Parameters:
	//     xmlNode     - XML node to save.
	//     strSection  - Registry section.
	//-----------------------------------------------------------------------
	void SaveToNode(CXTPDOMNodePtr xmlNode, CString strSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to load the layout from the XML node.
	// Parameters:
	//     xmlNode     - XML node to load.
	//     strSection  - Registry section.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL LoadFromNode(CXTPDOMNodePtr xmlNode, CString strSection);

#endif

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if it is a user created layout.
	// Returns:
	//     TRUE if it is a user created layout.
	//-----------------------------------------------------------------------
	BOOL IsUserLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if layout is valid.
	// Returns:
	//     TRUE if Layout is valid
	//-----------------------------------------------------------------------
	BOOL IsValid();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to save/restore the settings of the pane.
	// Parameters:
	//     pPX - Points to a CXTPPropExchange object.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL DoPropExchange(CXTPPropExchange* pPX);

private:
	void Push(CXTPDockingPaneBase* pPane) { m_lstStack.AddTail(pPane); }
	void HidePane(CXTPDockingPaneBase* pPane);
	void OnSizeParent(CFrameWnd* pParent, CRect rect, LPVOID lParam);
	void DestroyPane(CXTPDockingPane* pPane);
	void Free();
	void _FreeEmptyPanes();
	XTPDockingPaneDirection _GetPaneDirection(CXTPDockingPaneBase* pPane);

	void _Save(CXTPPropExchange* pPX);
	BOOL _Load(CXTPPropExchange* pPX);

private:
	CXTPDockingPaneSplitterContainer* m_pTopContainer;
	CXTPDockingPaneBase* m_pClient;

	CXTPDockingPaneInfoList m_lstPanes;
	CXTPDockingPaneBaseList m_lstStack;

	XTP_DOCKINGPANE_INFO* FindPane(CXTPDockingPane* pPane);

	CXTPDockingPaneAutoHidePanel* m_wndPanels[4];

	BOOL m_bUserLayout;
	CXTPDockingPaneManager* m_pManager;


private:
	friend class CXTPDockingPaneManager;
	friend class CXTPDockingPaneBase;
	friend class CXTPDockingPane;
	friend class CXTPDockingPaneAutoHidePanel;
	friend class CDockingPaneSite;
};



AFX_INLINE BOOL CXTPDockingPaneLayout::IsUserLayout() {
	return m_bUserLayout;
}
AFX_INLINE CXTPDockingPaneInfoList& CXTPDockingPaneLayout::GetPaneList() {
	return m_lstPanes;
}

#endif // #if !defined(__XTPDOCKINGPANELAYOUT_H__)
