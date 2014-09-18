// XTResizePoint.h: interface for the CXTResizePoint class.
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
#if !defined(__XTRESIZEPOINT_H__)
#define __XTRESIZEPOINT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// -----------------------------------------------------------------
// Summary:
//     XT_RESIZEPOINT structure is used by the CXTResizePoint
//     class for defining single -precision floating point x- and
//     y-coordinates of a point.
// See Also:
//     CXTResizePoint
// -----------------------------------------------------------------
struct XT_RESIZEPOINT
{
	XT_RESIZE x; // Specifies the x-coordinate of a point
	XT_RESIZE y; // Specifies the y-coordinate of a point
};

//===========================================================================
// Summary:
//     CXTResizePoint is an XT_RESIZEPOINT structure derived class. The
//     CXTResizePoint class is similar to the XT_RESIZEPOINT structure.
//     It also includes member functions to manipulate CXTResizePoint and
//     XT_RESIZEPOINT structures. A CXTResizePoint object can be used wherever
//     an XT_RESIZEPOINT structure is used.
//===========================================================================
class _XT_EXT_CLASS CXTResizePoint : public XT_RESIZEPOINT
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTResizePoint object
	//-----------------------------------------------------------------------
	CXTResizePoint();

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTResizePoint object
	// Parameters:
	//     cx - Specifies the x-coordinate of a point.
	//     cy - Specifies the y-coordinate of a point.
	//-----------------------------------------------------------------------
	CXTResizePoint(XT_RESIZE cx,XT_RESIZE cy);
};

//-----------------------------------------------------------------------
// Summary:
//    CXTResizePoint constants used by the CXTResize class for defining the
//    anchor position for a child in a resizable window.
// Parameters:
//    SZ_TOP_LEFT - Specifies a top left position.
//    SZ_TOP_CENTER - Specifies a top center position.
//    SZ_TOP_RIGHT - Specifies a top right position.
//    SZ_MIDDLE_LEFT - Specifies a middle left position.
//    SZ_MIDDLE_CENTER - Specifies a middle center position.
//    SZ_MIDDLE_RIGHT - Specifies a middle right position.
//    SZ_BOTTOM_LEFT - Specifies a bottom left position.
//    SZ_BOTTOM_CENTER - Specifies a bottom center position.
//    SZ_BOTTOM_RIGHT - Specifies a bottom right position.
// Example:
// <code>
// // Set control resizing.
// SetResize(IDC_EDIT_MASKEDIT,   SZ_TOP_LEFT,     SZ_TOP_RIGHT);
// SetResize(IDC_TXT_MASK,        SZ_TOP_CENTER,   SZ_TOP_RIGHT);
// SetResize(IDC_LBOX_MASK,       SZ_TOP_CENTER,   SZ_TOP_RIGHT);
// SetResize(IDC_BTN_UPDATE,      SZ_TOP_CENTER,   SZ_TOP_CENTER);
// SetResize(IDC_EDIT_MASK,       SZ_TOP_LEFT,     SZ_TOP_CENTER);
// SetResize(IDC_EDIT_PROMPT,     SZ_TOP_LEFT,     SZ_TOP_CENTER);
// SetResize(IDC_EDIT_LITERAL,    SZ_TOP_LEFT,     SZ_TOP_CENTER);
// SetResize(IDC_EDIT_DEFAULT,    SZ_TOP_LEFT,     SZ_TOP_CENTER);
// SetResize(IDC_GBOX_PROPERTIES, SZ_TOP_LEFT,     SZ_TOP_CENTER);
// SetResize(IDCANCEL,            SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
// SetResize(IDOK,                SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
// </code>
// See Also:
//    CXTResize, CXTResizeDialog, CXTResizeFormView, CXTResizeGroupBox, CXTResizePropertyPage, CXTResizePropertySheet,  CXTResizePoint, CXTResizeRect
//-----------------------------------------------------------------------
#define SZ_BOTTOM_CENTER CXTResizePoint(.5, 1)
//<COMBINE SZ_BOTTOM_CENTER>
#define SZ_BOTTOM_LEFT   CXTResizePoint( 0, 1)
//<COMBINE SZ_BOTTOM_CENTER>
#define SZ_BOTTOM_RIGHT  CXTResizePoint( 1, 1)
//<COMBINE SZ_BOTTOM_CENTER>
#define SZ_MIDDLE_CENTER CXTResizePoint(.5,.5)
//<COMBINE SZ_BOTTOM_CENTER>
#define SZ_MIDDLE_LEFT   CXTResizePoint( 0,.5)
//<COMBINE SZ_BOTTOM_CENTER>
#define SZ_MIDDLE_RIGHT  CXTResizePoint( 1,.5)
//<COMBINE SZ_BOTTOM_CENTER>
#define SZ_TOP_LEFT      CXTResizePoint( 0, 0)
//<COMBINE SZ_BOTTOM_CENTER>
#define SZ_TOP_RIGHT     CXTResizePoint( 1, 0)
//<COMBINE SZ_BOTTOM_CENTER>
#define SZ_TOP_CENTER    CXTResizePoint(.5, 0)

#endif // !defined(__XTRESIZEPOINT_H__)
