// XTResizeRect.h: interface for the CXTResizeRect class.
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
#if !defined(__XTRESIZERECT_H__)
#define __XTRESIZERECT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------
// Summary:
//     The XT_RESIZE data type is defined as a <i>float</i> and is
//     used for storing single-precision floating point x and y window
//     coordinates.
// See Also:
//     CXTResizeRect, XT_RESIZERECT, CXTResizePoint, XT_RESIZEPOINT
// ----------------------------------------------------------------------
typedef float XT_RESIZE;

// ---------------------------------------------------------------------
// Summary:
//     XT_RESIZERECT structure is used by the CXTResizeRect class
//     for defining single -precision floating point coordinates of the
//     upper-left and lower-right corners of a rectangle.
// Example:
//     Alternate ways to initialize a XT_RESIZERECT structure:
// <code>
// XT_RESIZERECT rc1;
// rc1.left = 20;
// rc1.top = 30;
// rc1.right = 180;
// rc1.bottom = 230;
//
// XT_RESIZERECT rc2 = {20,30,180,230};
// </code>
// See Also:
//     CXTResizeRect, XT_RESIZE
// ---------------------------------------------------------------------
struct XT_RESIZERECT
{
	XT_RESIZE left;     // Specifies the x-coordinate of the upper-left corner of a rectangle.
	XT_RESIZE top;      // Specifies the y-coordinate of the upper-left corner of a rectangle.
	XT_RESIZE right;    // Specifies the x-coordinate of the lower-right corner of a rectangle.
	XT_RESIZE bottom;   // Specifies the y-coordinate of the lower-right corner of a rectangle.
};

//===========================================================================
// Summary:
//     CXTResizeRect is an XT_RESIZERECT structure derived class. The
//     CXTResizeRect class is similar to an XT_RESIZERECT structure. CXTResizeRect
//     also includes member functions to manipulate CXTResizeRect objects and
//     XT_RESIZERECT structures.
// See Also:
//     XT_RESIZERECT, CXTResizePoint, XT_RESIZEPOINT, XT_RESIZE
//===========================================================================
class _XT_EXT_CLASS CXTResizeRect: public XT_RESIZERECT
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTResizeRect object
	//-----------------------------------------------------------------------
	CXTResizeRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTResizeRect object
	// Parameters:
	//     rc - Refers to the RECT structure with the coordinates for CXTResizeRect.
	//-----------------------------------------------------------------------
	CXTResizeRect(const RECT& rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTResizeRect object
	// Parameters:
	//     rrc - Refers to the XT_RESIZERECT structure with the coordinates for
	//           CXTResizeRect.
	//-----------------------------------------------------------------------
	CXTResizeRect(const XT_RESIZERECT& rrc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTResizeRect object.
	// Parameters:
	//     l - Specifies the left position of CXTResizeRect.
	//     t - Specifies the top of CXTResizeRect.
	//     r - Specifies the right position of CXTResizeRect.
	//     b - Specifies the bottom of CXTResizeRect.
	//-----------------------------------------------------------------------
	CXTResizeRect(XT_RESIZE l,XT_RESIZE t,XT_RESIZE r,XT_RESIZE b);

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator copies the dimensions of a rectangle to CXTResizeRect.
	// Parameters:
	//     rc - Refers to a source rectangle. It can be a RECT or CRect.
	// Returns:
	//     A reference to a CXTResizeRect object.
	//-----------------------------------------------------------------------
	CXTResizeRect& operator = (const RECT& rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator copies the dimensions of a rectangle to CXTResizeRect.
	// Parameters:
	//     rrc - Refers to a source rectangle. It can be a XT_RESIZERECT or CXTResizeRect.
	// Returns:
	//     A reference to a CXTResizeRect object.
	//-----------------------------------------------------------------------
	CXTResizeRect& operator = (const XT_RESIZERECT& rrc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator adds the specified offsets to CXTResizeRect or inflates
	//     CXTResizeRect.
	// Parameters:
	//     rrc - Points to an XT_RESIZERECT structure or a CXTResizeRect object that
	//           contains the number of units to inflate each side of CXTResizeRect.
	// Returns:
	//     A reference to a CXTResizeRect object.
	//-----------------------------------------------------------------------
	CXTResizeRect& operator += (const XT_RESIZERECT& rrc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator adds the given offsets to CRect or inflates CRect.
	// Parameters:
	//     rrc - Points to an XT_RESIZERECT structure or a CXTResizeRect object that
	//           contains the number of units to inflate each side of the return value.
	// Returns:
	//     The resulting CXTResizeRect object.
	//-----------------------------------------------------------------------
	CXTResizeRect operator + (const XT_RESIZERECT& rrc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator creates the intersection of CXTResizeRect and a rectangle,
	//     and returns the resulting CXTResizeRect.
	// Parameters:
	//     rrc - Contains an XT_RESIZERECT or a CXTResizeRect.
	// Returns:
	//     A CXTResizeRect that is the intersection of CXTResizeRect and 'rrc'. The
	//     intersection is the largest rectangle that is contained in both rectangles.
	//-----------------------------------------------------------------------
	CXTResizeRect operator & (const XT_RESIZERECT& rrc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator determines whether CXTResizeRect is equal to a rectangle.
	// Parameters:
	//     rrc - Refers to a source rectangle. It can be an XT_RESIZERECT or a CXTResizeRect.
	// Returns:
	//     true if equal, otherwise returns false.
	//-----------------------------------------------------------------------
	bool operator == (const XT_RESIZERECT& rrc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator determines whether CXTResizeRect is not equal to a rectangle.
	// Parameters:
	//     rrc - Refers to a source rectangle. It can be an XT_RESIZERECT or a CXTResizeRect.
	// Returns:
	//     false if equal, otherwise returns true.
	//-----------------------------------------------------------------------
	bool operator != (const XT_RESIZERECT& rrc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator converts a CXTResizeRect to a CRect. When you use this
	//     function, you do not need the address-of (&) operator. This operator
	//     will be automatically used when you pass a CXTResizeRect object to
	//     a function that expects a CRect.
	//-----------------------------------------------------------------------
	operator CRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function determines if CXTResizeRect is normalized.
	// Returns:
	//     true if normalized, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsNormalized();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function calculates the width of a CXTResizeRect by subtracting
	//     the left value from the right value. The resulting value can be negative.
	// Returns:
	//     The width of a CXTResizeRect.
	//-----------------------------------------------------------------------
	XT_RESIZE Width();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function calculates the height of a CXTResizeRect by subtracting
	//     the top value from the bottom value. The resulting value can be negative.
	// Returns:
	//     The height of a CXTResizeRect.
	//-----------------------------------------------------------------------
	XT_RESIZE Height();
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTResizeRect::CXTResizeRect(const RECT& rc) {
	(operator =)(rc);
}
AFX_INLINE CXTResizeRect::CXTResizeRect(const XT_RESIZERECT& rrc) {
	(operator =)(rrc);
}
AFX_INLINE CXTResizeRect CXTResizeRect::operator + (const XT_RESIZERECT& rrc) {
	return CXTResizeRect(left + rrc.left, top + rrc.top, right + rrc.right, bottom += rrc.bottom);
}
AFX_INLINE bool CXTResizeRect::IsNormalized() {
	return ((left <= right) && (top <= bottom));
}
AFX_INLINE bool CXTResizeRect::operator == (const XT_RESIZERECT& rrc) {
	return left==rrc.left && top==rrc.top && right==rrc.right && bottom==rrc.bottom;
}
AFX_INLINE bool CXTResizeRect::operator != (const XT_RESIZERECT& rrc) {
	return !operator==(rrc);
}
AFX_INLINE CXTResizeRect::operator CRect() {
	return CRect((int) left, (int) top, (int) right, (int) bottom);
}
AFX_INLINE XT_RESIZE CXTResizeRect::Width() {
	return right-left;
}
AFX_INLINE XT_RESIZE CXTResizeRect::Height() {
	return bottom-top;
}

//-----------------------------------------------------------------------
// Summary:
//    CXTResizeRect constants used by the CXTResize class for defining the
//    resize attributes for a child in a resizable window.
// Parameters:
//    SZ_RESIZE - Resize.
//    SZ_REPOS - Reposition.
//    SZ_HORRESIZE - Horizontal resize.
//    SZ_HORREPOS - Horizontal reposition.
//    SZ_VERRESIZE - Vertical resize.
//    SZ_VERREPOS - Vertical reposition.
// Example:
// <code>
// // Set control resizing.
// SetResize(ID_WIZBACK, SZ_REPOS(1));
// SetResize(ID_WIZNEXT, SZ_REPOS(1));
// SetResize(ID_WIZFINISH, SZ_REPOS(1));
// SetResize(ID_WIZLINE, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
// </code>
// See Also:
//    CXTResize, CXTResizeDialog, CXTResizeFormView, CXTResizeGroupBox, CXTResizePropertyPage, CXTResizePropertySheet,  CXTResizePoint, CXTResizeRect
//-----------------------------------------------------------------------
#define SZ_RESIZE(x)    CXTResizeRect(0,0,x,x)
// <COMBINE SZ_RESIZE>
#define SZ_REPOS(x)     CXTResizeRect(x,x,x,x)
// <COMBINE SZ_RESIZE>
#define SZ_HORRESIZE(x) CXTResizeRect(0,0,x,0)
// <COMBINE SZ_RESIZE>
#define SZ_HORREPOS(x)  CXTResizeRect(x,0,x,0)
// <COMBINE SZ_RESIZE>
#define SZ_VERRESIZE(x) CXTResizeRect(0,0,0,x)
// <COMBINE SZ_RESIZE>
#define SZ_VERREPOS(x)  CXTResizeRect(0,x,0,x)

#endif // !defined(__XTRESIZERECT_H__)
