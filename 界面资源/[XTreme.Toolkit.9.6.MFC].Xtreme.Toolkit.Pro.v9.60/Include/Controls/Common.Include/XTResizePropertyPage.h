// XTResizePropertyPage.h: interface for the CXTResizePropertyPage class.
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
#if !defined(__XTRESIZEPROPERTYPAGE_H__)
#define __XTRESIZEPROPERTYPAGE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTResizePropertyPage is a multiple inheritance class derived from
//     CPropertyPage and CXTResize. CXTResizePropertyPage is used to create
//     a resizable CPropertyPage type object that allows its dialog items to
//     be resized or moved dynamically.
//===========================================================================
class _XT_EXT_CLASS CXTResizePropertyPage : public CPropertyPage, public CXTResize
{
	DECLARE_DYNCREATE(CXTResizePropertyPage)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTResizePropertyPage object
	// Parameters:
	//     nTemplate - ID of the template used for this page.
	//     nCaption  - ID of the name to be placed in the tab for this page. If 0, the name
	//                 will be taken from the dialog template for this page.
	//     nFlags    - Flags that are to be passed to CXTResize that specify the attributes
	//                 of the resizing property page. They can be one or more of the values in the Remarks section.
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
	//-----------------------------------------------------------------------
	CXTResizePropertyPage(const UINT nTemplate = 0,const UINT nCaption = 0,const UINT nFlags = 0);

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();

	//{{AFX_MSG(CXTResizePropertyPage)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO *lpMMI);
	afx_msg void OnDestroy();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

public:
	DWORD m_nDialogID; // ID of the template used for this page

};

#endif // !defined(__XTRESIZEPROPERTYPAGE_H__)
