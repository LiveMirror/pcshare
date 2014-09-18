// XTResizeDialog.h: interface for the CXTResizeDialog class.
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
#if !defined(__XTRESIZEDIALOG_H__)
#define __XTRESIZEDIALOG_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTResizeDialog is a multiple inheritance class derived from CDialog
//     and CXTResize. CXTResizeDialog is used to create a resizable CDialog
//     type object that allows its dialog items to be resized or moved dynamically.
//===========================================================================
class _XT_EXT_CLASS CXTResizeDialog : public CDialog, public CXTResize
{
	DECLARE_DYNCREATE(CXTResizeDialog)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTResizeDialog object
	//-----------------------------------------------------------------------
	CXTResizeDialog();

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTResizeDialog object
	// Parameters:
	//     nID     - Contains the ID number of a dialog box template resource.
	//     pParent - Points to the parent or owner window object of type CWnd, which
	//               the dialog object belongs to. If it is NULL, the dialog object’s parent
	//               window is set to the main application window.
	//     nFlags  - Flags that are to be passed to CXTResize that specify the attributes
	//               of the resizing property page. They can be one or more of the values
	//               listed in the Remarks section.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:
	//     * <b>xtResizeNoSizeIcon</b> Do not add size icon.
	//     * <b>xtResizeNoHorisontal</b> No horizontal resizing.
	//     * <b>xtResizeNoVertical</b> No vertical resizing.
	//     * <b>xtResizeNoMinsize</b> Do not require a minimum size.
	//     * <b>xtResizeNoClipChildren</b> Do not set clip children style.
	//     * <b>xtResizeNoTransparentGroup</b> Do not set transparent style
	//       for group boxes.
	//-----------------------------------------------------------------------
	CXTResizeDialog(const UINT nID,CWnd* pParent = 0,const UINT nFlags = 0);

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTResizeDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO *lpMMI);
	afx_msg void OnDestroy();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	UINT m_nDialogID; // Contains the ID number of a dialog box template resource

};

#endif // !defined(__XTRESIZEDIALOG_H__)
