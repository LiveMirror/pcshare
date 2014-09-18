// XTPCustomizeSheet.h : interface for the CXTPCustomizeSheet class.
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
#if !defined(__XTPCUSTOMIZESHEET_H__)
#define __XTPCUSTOMIZESHEET_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTPCommandBarsDefines.h"
#include "CommonPro.include/XTPManageState.h"

class CXTPCommandBars;
class CXTPCustomizeToolbarsPage;
class CXTPCustomizeCommandsPage;
class CXTPImageManager;

//===========================================================================
// Summary:
//     CXTPCustomizeSheet is a CPropertySheet derived class. It represents
//     the Customize dialog.
//===========================================================================
class _XTP_EXT_CLASS CXTPCustomizeSheet : public CPropertySheet
{

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCustomizeSheet object
	// Parameters:
	//     pCommandBars - Points to a CXTPCommandBars object
	//     iSelectPage  - Page initially selected.
	//-----------------------------------------------------------------------
	CXTPCustomizeSheet(CXTPCommandBars* pCommandBars, UINT iSelectPage = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCustomizeSheet object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCustomizeSheet();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get a pointer to the context menu that
	//     is displayed in customization mode when a user right-clicks
	//     on a control.
	// Returns:
	//     Retrieves the context menu.
	//-----------------------------------------------------------------------
	CXTPPopupBar* GetContextMenu();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves collection of custom icons that can be selected for
	//     a control during customization mode.  Custom icons are displayed
	//     in the "Change Button Image" popup of the context menu that is
	//     displayed when a user right-clicks on a control.
	// Remarks:
	//     Icons must be numbered from 1.  When adding icons, the numbering
	//     must start with 1 and increment by 1 for each icon added.  So the
	//     numbering would look like this: 1,2,3,4,...N.
	//
	//          The toolkit provide 42 "built-in" icons that you can choose from
	//          and they are automatically displayed in the "Change Button Icon"
	//          popup menu if you do not remove them.  If you do not remove the
	//          "built-in" icons that are provided with the toolkit then you should
	//          start your numbering at 43.  If you start at 1 and you did not
	//          remove the "bulilt-in" icons, then you will overwrite some of
	//          the "built-in" icons.  So you would number your icons
	//          43,44,45,...N.  So you could even keep some of the "built-in"
	//          icons and overwrite the ones you don't need with your own
	//          custom icons.
	//-----------------------------------------------------------------------
	CXTPImageManager* GetCustomIcons();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the Commands PropertyPage
	// Returns:
	//     A pointer to a CXTPCustomizeCommandsPage object
	//-----------------------------------------------------------------------
	CXTPCustomizeCommandsPage* GetCommandsPage();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the parent Command Bars class
	// Returns:
	//     A pointer to a CXTPCommandBars object
	//-----------------------------------------------------------------------
	CXTPCommandBars* GetCommandBars();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to show customize dialog.
	//-----------------------------------------------------------------------
	INT_PTR DoModal();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method shows the context menu for toolbars
	// Parameters:
	//     point - Position of the menu to show
	//-----------------------------------------------------------------------
	void ContextMenu(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds the supplied page with the rightmost tab in the property sheet.
	// Parameters:
	//     pPage - Points to the page to be added to the property sheet. Cannot be NULL.
	//-----------------------------------------------------------------------
	void AddPage(CPropertyPage* pPage);

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPCustomizeSheet)
	BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPCustomizeSheet)
	afx_msg void OnCustomizeDefaultStyle();
	afx_msg void OnCustomizeImageAndText();
	afx_msg void OnCustomizeTextOnly();
	afx_msg void OnCustomizeBeginGroup();
	afx_msg void OnCustomizeDelete();
	afx_msg void OnCustomizeCopyImage();
	afx_msg void OnCustomizePasteImage();
	afx_msg void OnCustomizeResetImage();
	afx_msg void OnCustomizeEditImage();
	afx_msg void OnCustomizeReset();
	afx_msg LRESULT OnCustomizeName(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateCustomizeDefaultStyle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCustomizeImageAndText(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCustomizeTextOnly(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCustomizeBeginGroup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCustomizeName(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCustomizeCopyImage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCustomizePasteImage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCustomizeResetImage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCustomizeReset(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCustomizeEditImage(CCmdUI* pCmdUI);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnInitCommandsPopup(CXTPPopupBar* pPopupBar);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL FilterMouse(WPARAM wParam, LPARAM lParam);
	BOOL FilterKey(WPARAM wParam, LPARAM lParam);
	void CreateContextMenu();

protected:
	CXTPCustomizeToolbarsPage* m_pPageToolBars;     // Toolbar page.
	CXTPCustomizeCommandsPage* m_pPageCommands;     // Commands page.
	CWnd* m_pWndParent;                             // Parent window.
	CXTPCommandBars* m_pCommandBars;                // Customized comamndBars.
	CXTPPopupBar* m_pContextMenu;                   // Context menu that is displayed in customization mode when a
	                                                // user right-clicks on a control.
	CXTPImageManager* m_pCustomIcons;               // Icons displayed in the "Change Button Image" popup of the
	                                                // context menu that is displayed when a user right-clicks on a control.

private:
	DECLARE_DYNAMIC(CXTPCustomizeSheet)

	class CCustomizeButton;
	class CCustomizeEdit;
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPCustomizeCommandsPage* CXTPCustomizeSheet::GetCommandsPage() {
	return m_pPageCommands;
}
AFX_INLINE CXTPCommandBars* CXTPCustomizeSheet::GetCommandBars() {
	return m_pCommandBars;
}
AFX_INLINE CXTPPopupBar* CXTPCustomizeSheet::GetContextMenu() {
	return m_pContextMenu;
}
AFX_INLINE CXTPImageManager* CXTPCustomizeSheet::GetCustomIcons() {
	return m_pCustomIcons;
}



#endif // #if !defined(__XTPCUSTOMIZESHEET_H__)
