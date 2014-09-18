// XTPCustomizeToolbarsPage.h : interface for the CXTPCustomizeToolbarsPage class.
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
#if !defined(__XTPCUSTOMIZETOOLBARSPAGE_H__)
#define __XTPCUSTOMIZETOOLBARSPAGE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPCustomizeSheet;

//===========================================================================
// Summary:
//     CXTPCustomizeToolbarsPage is a CPropertyPage derived class.
//     It represents the Toolbars page of the Customize dialog.
//===========================================================================
class _XTP_EXT_CLASS CXTPCustomizeToolbarsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CXTPCustomizeToolbarsPage)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCustomizeToolbarsPage object
	// Parameters:
	//     pSheet - Points to a CXTPCustomizeSheet object that this page
	//     belongs to.
	//-----------------------------------------------------------------------
	CXTPCustomizeToolbarsPage(CXTPCustomizeSheet* pSheet = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCustomizeToolbarsPage object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	~CXTPCustomizeToolbarsPage();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPCustomizeToolbarsPage)
	virtual void DoDataExchange(CDataExchange* pDX);
	BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPCustomizeToolbarsPage)
	afx_msg void OnSelectionChanged();
	afx_msg void OnChkChange();
	afx_msg void OnNewToolbar();
	afx_msg void OnRenameToolbar();
	afx_msg void OnDeleteToolbar();
	afx_msg void OnResetToolbar();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	void RefreshToolbarsList();

public:
//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_DATA(CXTPCustomizeToolbarsPage)
	enum { IDD = XTP_IDD_PAGE_TOOLBARS };
	CXTPCustomizeSheet* m_pSheet;
	CCheckListBox m_checkList;
	CButton m_btnReset;
	CButton m_btnRename;
	CButton m_btnDelete;
	CButton m_btnNew;
	//}}AFX_DATA
//}}AFX_CODEJOCK_PRIVATE

};

#endif // !defined(__XTPCUSTOMIZETOOLBARSPAGE_H__)
