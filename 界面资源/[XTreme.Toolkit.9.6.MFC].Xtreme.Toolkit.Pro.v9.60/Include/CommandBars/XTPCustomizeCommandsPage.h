// XTPCustomizeCommandsPage.h : interface for the CXTPCustomizeCommandsPage class.
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
#if !defined(__XTPCUSTOMIZECOMMANDSPAGE_H__)
#define __XTPCUSTOMIZECOMMANDSPAGE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPCustomizeSheet;
class CXTPControl;
class CXTPControls;
class CXTPCommandBar;


//-----------------------------------------------------------------------
// Summary:
//     This structure stores categories for CXTPCustomizeCommandsPage and CXTPCustomizeKeyboardPage option
//     pages
// See Also: CXTPCustomizeCommandsPage, CXTPCustomizeKeyboardPage
//-----------------------------------------------------------------------
struct XTP_COMMANDBARS_CATEGORYINFO
{
//{{AFX_CODEJOCK_PRIVATE
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a XTP_COMMANDBARS_CATEGORYINFO struct
	// Parameters:
	//     strCategory  - Category caption
	//     pCommandBars - Pointer to a CXTPCommandBars object.
	//-----------------------------------------------------------------------
	XTP_COMMANDBARS_CATEGORYINFO(CString strCategory, CXTPCommandBars* pCommandBars);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a XTP_COMMANDBARS_CATEGORYINFO, handles cleanup
	//     and deallocation.
	//-------------------------------------------------------------------------
	~XTP_COMMANDBARS_CATEGORYINFO();
//}}AFX_CODEJOCK_PRIVATE

	CString strCategory;        // Category caption
	CXTPControls* pControls;    // Controls of this category.
};


//----------------------------------------------------------------------
// Summary:
//     This array is used by the CXTPCustomizeCommandsPage class for
//     maintaining a list of XTP_COMMANDBARS_CATEGORYINFO structures.
// See Also:
//     CXTPCustomizeCommandsPage
//----------------------------------------------------------------------
typedef CArray<XTP_COMMANDBARS_CATEGORYINFO*, XTP_COMMANDBARS_CATEGORYINFO*> CXTPCommandBarsCategoryArray;

//===========================================================================
// Summary:
//     CXTPCustomizeCommandsPage is a CPropertyPage derived class.
//     It represents the Commands page of the Customize dialog.
//===========================================================================
class _XTP_EXT_CLASS CXTPCustomizeCommandsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CXTPCustomizeCommandsPage)

private:
	//-----------------------------------------------------------------------
	// Summary:
	//     List of the commands
	//-----------------------------------------------------------------------
	class CCommandsListBox : public CListBox
	{
	protected:
		//{{AFX_CODEJOCK_PRIVATE
		DECLARE_MESSAGE_MAP()

		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		virtual void DrawItem(LPDRAWITEMSTRUCT);
		virtual LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam);
		//}}AFX_CODEJOCK_PRIVATE

		friend class CXTPCustomizeCommandsPage;

	private:
		CXTPCustomizeCommandsPage* m_pCommandsPage;
	};

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCustomizeCommandsPage object
	// Parameters:
	//     pSheet - Points to a CXTPCustomizeSheet object
	//-----------------------------------------------------------------------
	CXTPCustomizeCommandsPage(CXTPCustomizeSheet* pSheet = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCustomizeCommandsPage object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	~CXTPCustomizeCommandsPage();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Adds categories brunch from menu resource.
	// Parameters:
	//     nIDResource   - Menu resource from where categories will be built.
	//     bListSubItems - TRUE to add sub menus to categories.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL AddCategories(UINT nIDResource,  BOOL bListSubItems = FALSE);

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
	//     Adds a new category from a CMenu object.
	// Parameters:
	//     strCategory   - Category to be added.
	//     pMenu         - Points to a CMenu object
	//     bListSubItems - TRUE to add sub menus to the category.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL AddCategory(CString strCategory, CMenu* pMenu, BOOL bListSubItems = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds a new category from a toolbar resource.
	// Parameters:
	//     strCategory - Category to be added.
	//     nIDResource - Toolbar resource identifier.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL AddToolbarCategory(CString strCategory, UINT nIDResource);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method adds a new empty category in the given index.
	// Parameters:
	//     strCategory - Category to be added.
	//     nIndex      - Index to insert.
	// Returns:
	//     A pointer to a CXTPControls object
	//-----------------------------------------------------------------------
	CXTPControls* InsertCategory(CString strCategory, int nIndex = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds single "New menu" category.
	// Parameters:
	//     nIDResource - "New Menu" category string resource.
	//     nIndex      - Index to insert
	//-----------------------------------------------------------------------
	void InsertNewMenuCategory(UINT nIDResource = XTP_IDS_NEWMENU, int nIndex = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds "All Commands" category and adds all controls already
	//     created in other categories here.
	// Parameters:
	//     nIDResource - "All commands" category string resource.
	//     nIndex      - Index to insert
	//-----------------------------------------------------------------------
	void InsertAllCommandsCategory(UINT nIDResource = XTP_IDS_ALLCOMMANDS, int nIndex = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds built-in menus category.
	// Parameters:
	//     nIDResourceMenu     - Menu resource identifier.
	//     nIDResourceCategory - "Built-in" string identifier.
	//     nIndex              - Index to insert.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL InsertBuiltInMenus(UINT nIDResourceMenu, UINT nIDResourceCategory = XTP_IDS_BUILTINMENUS, int nIndex = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the control list of the given category.
	// Parameters:
	//     strCategory - Category to retrieve.
	// Returns:
	//     A pointer to a CXTPControls object.
	//-----------------------------------------------------------------------
	CXTPControls* GetControls(CString strCategory);


protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPCustomizeCommandsPage)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPCustomizeCommandsPage)
	afx_msg void OnCategoriesSelectionChanged();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE


private:
	XTP_COMMANDBARS_CATEGORYINFO* FindCategory(CString strCategory);
	XTP_COMMANDBARS_CATEGORYINFO* GetCategoryInfo(int nIndex);
	BOOL _LoadToolBar(CXTPCommandBar* pCommandBar, UINT nIDResource);

protected:
//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_DATA(CXTPCustomizeCommandsPage)
	enum { IDD = XTP_IDD_PAGE_COMMANDS };
	CCommandsListBox    m_lstCommands;
	CListBox    m_lstCategories;
	//}}AFX_DATA
//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPCustomizeSheet* m_pSheet;                   // Parent Customize dialog
	CXTPCommandBarsCategoryArray m_arrCategories;   // Array of categories.

private:
	friend class CCommandsListBox;


};

#endif // !defined(__XTPCUSTOMIZECOMMANDSPAGE_H__)
