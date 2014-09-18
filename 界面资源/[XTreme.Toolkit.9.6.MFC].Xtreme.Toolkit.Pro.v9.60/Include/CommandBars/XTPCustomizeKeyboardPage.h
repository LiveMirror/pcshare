// XTPCustomizeKeyboardPage.h : interface for the CXTPCustomizeKeyboardPage class.
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
#if !defined(__XTPCUSTOMIZEKEYBOARDPAGE_H__)
#define __XTPCUSTOMIZEKEYBOARDPAGE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonPro.include/XTPManageState.h"

class CXTPCustomizeSheet;

//===========================================================================
// Summary:
//     CXTPCustomizeKeyboardPage is a CPropertyPage derived class.
//     It represents the Keyboard page of the Customize dialog.
//===========================================================================
class _XTP_EXT_CLASS CXTPCustomizeKeyboardPage : public CPropertyPage
{

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCustomizeKeyboardPage object
	// Parameters:
	//     pSheet - Points to a CXTPCustomizeSheet object that this page
	//     belongs to.
	//-----------------------------------------------------------------------
	CXTPCustomizeKeyboardPage(CXTPCustomizeSheet* pSheet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCustomizeKeyboardPage object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	~CXTPCustomizeKeyboardPage();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Finds the category.
	// Parameters:
	//     strCategory - Category needed to find.
	//-----------------------------------------------------------------------
	XTP_COMMANDBARS_CATEGORYINFO* FindCategory(CString strCategory);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method adds a new empty category in the given index.
	// Parameters:
	//     strCategory - Category to be added.
	//     nIndex - Index to insert.
	// Returns:
	//     A pointer to a CXTPControls object
	//-----------------------------------------------------------------------
	CXTPControls* InsertCategory(CString strCategory, int nIndex = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds categories brunch from menu resource.
	// Parameters:
	//     nIDResource - Menu resource from where categories will be built.
	//     bListSubItems - TRUE to add sub menus to categories.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL AddCategories(UINT nIDResource, BOOL bListSubItems = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new category from a CMenu object.
	// Parameters:
	//     strCategory - Category to be added.
	//     pMenu - Points to a CMenu object
	//     bListSubItems - TRUE to add sub menus to categories.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL AddCategory(CString strCategory, CMenu* pMenu, BOOL bListSubItems = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds categories brunch from Controls.
	// Parameters:
	//     pControls - Points to a CXTPControls object
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL AddCategories(CXTPControls* pControls);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the page to reload the list
	//     items.
	//-----------------------------------------------------------------------
	void ReloadList();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the page to enable the
	//     assignment buttons.
	//-----------------------------------------------------------------------
	void EnableControls();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves frame accelerators.
	//-----------------------------------------------------------------------
	HACCEL GetFrameAccelerator();

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates frame accelerators.
	// Parameters:
	//     lpAccel - Accelerator table to set.
	//     nSize - Number of items.
	//-----------------------------------------------------------------------
	void UpdateAcellTable(LPACCEL lpAccel, int nSize);


protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPCustomizeKeyboardPage)
	virtual BOOL OnSetActive();
	virtual void DoDataExchange(CDataExchange* pDX);
	BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPCustomizeKeyboardPage)
	afx_msg void OnSelchangeCategory();
	afx_msg void OnSelchangeCommands();
	afx_msg void OnSelchangeCurKeys();
	afx_msg void OnChangeShortcutKey();
	afx_msg void OnAssign();
	afx_msg void OnResetAll();
	afx_msg void OnRemove();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE


public:
//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_DATA(CXTPCustomizeCommandsPage)
	enum { IDD = XTP_IDD_PAGE_KEYBOARD };
	CListBox        m_lboxKeys;
	CListBox        m_lboxCommands;
	CComboBox       m_comboCategory;
	CString         m_strTitle;
	CString         m_strDesc;
	int             m_iCategory;
	CXTPShortcutManager::CKeyAssign m_editShortcutKey;
	//}}AFX_DATA
//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPCommandBarsCategoryArray  m_arrCategories;  // Array of categories
	CXTPCustomizeSheet* m_pSheet;                   // Parent Customize dialog

};

#endif // !defined(__XTPCUSTOMIZEKEYBOARDPAGE_H__)
