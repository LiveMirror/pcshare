// XTResizePropertySheet.h: interface for the CXTResizePropertySheet class.
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
#if !defined(__XTRESIZEPROPERTYSHEET_H__)
#define __XTRESIZEPROPERTYSHEET_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTResizePropertySheet is a multiple inheritance class derived from
//     CPropertySheet and CXTResize. CXTResizePropertySheet is used to create
//     a resizable CPropertySheet type object that allows its dialog items
//     to be resized or moved dynamically.
//===========================================================================
class _XT_EXT_CLASS CXTResizePropertySheet : public CPropertySheet, public CXTResize
{
	DECLARE_DYNCREATE(CXTResizePropertySheet)

public:
	//----------------------------------------------------------------------
	// Summary:
	//     Default constructor, constructs a CXTResizePropertySheet object
	//----------------------------------------------------------------------
	CXTResizePropertySheet();

	//----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTResizePropertySheet object
	// Parameters:
	//     nCaption    - ID of the caption to be used for the property sheet.
	//     pParent     - Points to the parent window of the property sheet. If NULL, the parent
	//                   window will be the main window of the application.
	//     nSelectPage - The index of the page that will initially be on top. Default is the
	//                   first page added to the sheet.
	//     nFlags      - Flags that are to be passed to CXTResize that specify the attributes
	//                   of the resizing property page. They can be one or more of the values in the Remarks section.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:<p/>
	//     * <b>xtResizeNoSizeIcon</b> Do not add size icon.
	//     * <b>xtResizeNoHorisontal</b> No horizontal resizing.
	//     * <b>xtResizeNoVertical</b> No vertical resizing.
	//     * <b>xtResizeNoMinsize</b> Do not require a minimum size.
	//     * <b>xtResizeNoClipChildren</b> Do not set clip children style.
	//     * <b>xtResizeNoTransparentGroup</b> Do not set transparent style
	//       for group boxes.
	//
	//----------------------------------------------------------------------
	CXTResizePropertySheet(const UINT nCaption, CWnd* pParent = 0, const UINT nSelectPage = 0, const UINT nFlags = 0);

	//----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTResizePropertySheet object
	// Parameters:
	//     pszCaption  - Points to a string containing the caption to be used for the property
	//                   sheet. It cannot be NULL.
	//     pParent     - Points to the parent window of the property sheet. If NULL, the parent
	//                   window will be the main window of the application.
	//     nSelectPage - The index of the page that will initially be on top. The default is the
	//                   first page added to the sheet.
	//     nFlags      - Flags that are to be passed to CXTResize that specify the attributes
	//                   of the resizing property page.  They can be one or more of the values in the Remarks section.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:<p/>
	//     * <b>xtResizeNoSizeIcon</b> Do not add size icon.
	//     * <b>xtResizeNoHorisontal</b> No horizontal resizing.
	//     * <b>xtResizeNoVertical</b> No vertical resizing.
	//     * <b>xtResizeNoMinsize</b> Do not require a minimum size.
	//     * <b>xtResizeNoClipChildren</b> Do not set clip children style.
	//     * <b>xtResizeNoTransparentGroup</b> Do not set transparent style
	//       for group boxes.
	//
	//----------------------------------------------------------------------
	CXTResizePropertySheet(LPCTSTR pszCaption,CWnd* pParent = 0,const UINT nSelectPage = 0,const UINT nFlags = 0);

protected:

	//----------------------------------------------------------------------
	// Summary:
	//     This member function determines if the property sheet is in wizard mode.
	// Returns:
	//     TRUE if the property sheet is a wizard, otherwise returns FALSE.
	//----------------------------------------------------------------------
	BOOL IsWizard();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTResizePropertySheet)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO *lpMMI);
	afx_msg void OnDestroy();
	afx_msg BOOL OnPageChanging(NMHDR* pNotifyStruct, LRESULT* pResult);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

public:
	CXTNoFlickerWnd <CTabCtrl> m_tabCtrl;  // Tab control.


	friend CXTResizePropertyPage;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTResizePropertySheet::IsWizard() {
	return (m_psh.dwFlags & PSH_WIZARD);
}

#endif // !defined(__XTRESIZEPROPERTYSHEET_H__)
