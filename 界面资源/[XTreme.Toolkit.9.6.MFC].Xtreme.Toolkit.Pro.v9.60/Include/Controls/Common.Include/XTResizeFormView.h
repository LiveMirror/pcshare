// XTResizeFormView.h: interface for the CXTResizeFormView class.
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
#if !defined(__XTRESIZEFORMVIEW_H__)
#define __XTRESIZEFORMVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTResizeFormView is a multiple inheritance class derived from CFormView
//     and CXTResize. CXTResizeFormView is used to create a resizable CFormView
//     type object that allows its form items to be resized or moved dynamically.
//===========================================================================
class _XT_EXT_CLASS CXTResizeFormView : public CFormView, public CXTResize
{
	DECLARE_DYNCREATE(CXTResizeFormView)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTResizeFormView object
	// Parameters:
	//     nID    - Contains the ID number of a dialog template resource.
	//     nFlags - Flags that are to be passed to CXTResize that specify the attributes
	//              of the resizing property page. They can be one or more of the values
	//              in the Remarks section:
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
	CXTResizeFormView(const UINT nID = 0,const UINT nFlags = 0);

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTResizeFormView)
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTResizeFormView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO *lpMMI);
	afx_msg void OnDestroy();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

};

#endif // !defined(__XTRESIZEFORMVIEW_H__)
