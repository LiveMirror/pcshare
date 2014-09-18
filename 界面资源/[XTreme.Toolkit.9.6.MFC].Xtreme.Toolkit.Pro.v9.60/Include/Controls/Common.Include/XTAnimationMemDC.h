// XTAnimationMemDC.h : header file
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
#if !defined(__XTANIMATIONMEMDC_H__)
#define __XTANIMATIONMEMDC_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ---------------------------------------------------------------
// Summary:
//     Enumeration used to determine animation style.
// Remarks:
//     XTAnimationType type defines constants used by the
//     CXTAnimationMemDC class to determine the animation
//     effect for the current device context.
// See Also:
//     CXTAnimationMemDC, CXTAnimationMemDC::Animate, CXTAnimationMemDC::SetCustomAnimation
//
// <KEYWORDS xtWindowsDefault, xtRandom, xtUnfold, xtSlide, xtFade, xtNone>
// -------------------------------------------------------------------------------
enum XTAnimationType
{
	xtWindowsDefault,  // Uses Windows default animation settings.
	xtRandom,          // Animates a window with a random animation type.
	xtUnfold,          // Animates a window to unfold top to bottom.
	xtSlide,           // Animates a window to slide in from left.
	xtFade,            // Animates a window with a fade-in effect.
	xtNone             // No animation.
};

// -------------------------------------------------------------------------------------
// Summary:
//     CXTAnimationMemDC is a CXTMemDC derived class. This class
//     can be used to render animation effects for the current device
//     context.
// Remarks:
//     By calling Animate the device context is drawn on the
//     screen with a defined animation type. See XTAnimationType for
//     a list of available animation styles.<p/>
//     Animations are similar to the menu animation effect as seen in
//     Microsoft® Windows® or Microsoft® Office products.
// Example:
//     See Animate for an example that demonstrates the use of CXTAnimationMemDC.
// See Also:
//     LPFNANIMATION, XTAnimationType, Animate, SetCustomAnimation
// -------------------------------------------------------------------------------------
class _XT_EXT_CLASS CXTAnimationMemDC : public CXTMemDC
{
public:
	// -------------------------------------------------------------------
	// Summary:
	//     Function pointer used to define a custom animation effect.
	// Parameters:
	//     rc -              Area to display.
	//     pDestDC -         Destination device context.
	//     pSrcDC -          Source device context.
	//     nType -           Enumerated XTAnimationType to determine
	//                       animation effect.
	//     nSteps -          Number of steps to complete animation.
	//     nAnimationTime -  Amount of time in milliseconds between each step.
	// Example:
	//     See CXTAnimationMemDC::SetCustomAnimation for an example that
	//     demonstrates the use of LPFNANIMATION.
	// See Also:
	//     CXTAnimationMemDC::SetCustomAnimation, XTAnimationType
	// -------------------------------------------------------------------
	typedef void (AFX_CDECL * LPFNANIMATION)(CRect rc, CDC* pDestDC, CDC* pSrcDC, int nType, int nSteps, int nAnimationTime);

public:

	// ---------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTAnimationMemDC object
	// Parameters:
	//     pDC -       [in] A pointer to a CDC object.
	//     rect -      [in] An address of a CRect object.
	//     clrColor -  [in] An RGB value that represents the current system face
	//                 color of three dimensional display elements.
	// Remarks:
	//     Call this member function to construct a CXTAnimationMemDC object
	//     using the specified device context.
	// Example:
	//     See Animate for an example that demonstrates the use of
	//     CXTAnimationMemDC.
	// See Also:
	//     LPFNANIMATION, XTAnimationType, Animate, SetCustomAnimation
	// ---------------------------------------------------------------------
	CXTAnimationMemDC(CDC* pDC, const CRect& rect, COLORREF clrColor=GetSysColor(COLOR_3DFACE));

	// -------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTAnimationMemDC object, handles cleanup and
	//     deallocation.
	// -------------------------------------------------------------------
	virtual ~CXTAnimationMemDC();

	// ----------------------------------------------------------------------------
	// Summary:
	//     This member function performs the animation
	// Parameters:
	//     nType -           [in] Type of animation to perform.
	//     nSteps -          [in] Number of steps to take during animation.
	//     nAnimationTime -  [in] Amount of time to rest, in milliseconds, between
	//                       each step.
	// Remarks:
	//     Call this member function from your WM_PAINT handler to render the
	//     animation background.
	// Example:
	//     The following example demonstrates the use of Animate.
	// <code>
	// void CMainFrame::DoAnimationTest(CDC* pDC, XTAnimationType anim)
	// {
	//     CRect r(100, 100, 300, 200);
	//     CXTMemDC background(pDC, r, (COLORREF)-1);
	//     background.FromDC();
	//     {
	//         CXTAnimationMemDC memDC(pDC, r, NULL);
	//
	//         r -= r.TopLeft();
	//
	//         memDC.Rectangle(r);
	//         memDC.MoveTo(0, 0);
	//         memDC.LineTo(r.BottomRight());
	//         memDC.SelectStockObject(GRAY_BRUSH);
	//         memDC.FloodFill(r.left + 10, r.top + 10, RGB(0, 0, 0));
	//         memDC.Animate(anim, 10, 1000);
	//     }
	//
	//     \::Sleep(2000);
	// }
	// </code>
	// See Also:
	//     LPFNANIMATION, XTAnimationType, SetCustomAnimation
	// ----------------------------------------------------------------------------
	void Animate(int nType = xtWindowsDefault, int nSteps = 10, int nAnimationTime = 1000);

	// ------------------------------------------------------------------------------
	// Summary:
	//     This member implements default animation effects <i>Fade</i>, <i>Slide</i>
	//     and <i>Unfold</i>.
	// Parameters:
	//     rc -              [in] Bounding rectangle.
	//     pDestDC -         [in] Pointer to device context you must draw to.
	//     pSrcDC -          [in] Device context that contains the bitmap you must
	//                       take.
	//     nType -           [in] Type of animation to perform. For custom animation
	//                       you must use numbers greater than <i>xtNone</i>.
	//     nSteps -          [in] Number of steps to take during animation.
	//     nAnimationTime -  [in] Amount of time to rest, in milliseconds, between
	//                       each step.
	// Remarks:
	//     You can add new animation effects to call SetCustomAnimation
	//     member.
	// Example:
	//     See SetCustomAnimation for an example that demonstrates the use of
	//     DefaultAnimation.
	// See Also:
	//     LPFNANIMATION, XTAnimationType, Animate, SetCustomAnimation
	// ------------------------------------------------------------------------------
	static void AFX_CDECL DefaultAnimation(CRect rc, CDC* pDestDC, CDC* pSrcDC, int nType, int nSteps, int nAnimationTime);

	// -----------------------------------------------------------------------
	// Summary:
	//     Call this member function to setup new Animation effects.
	// Parameters:
	//     pCustom -  [in] Function pointer that holds the address of a custom
	//                animation function.
	// Remarks:
	//     You can call DefaultAnimation in your custom animation method to
	//     handle default animation effects.
	// Example:
	//     The following example demonstrates the use of SetCustomAnimation.
	// <code>
	// int CMainFrame::OnCreate()
	// {
	//     // custom animation procedure.
	//     CXTAnimationMemDC::SetCustomAnimation(CustomAnimation);
	//     ...
	// }
	// ...
	// void CMainFrame::CustomAnimation(CRect rc, CDC* pDestDC,
	//     CDC* pSrcDC, int nType, int nSteps, int nAnimationTime)
	// {
	//     if (nType == xtUnfold)
	//     {
	//         // do custom animation
	//     }
	//     else
	//     {
	//         CXTAnimationMemDC::DefaultAnimation(rc, pDestDC,
	//             pSrcDC, nType, nSteps, nAnimationTime);
	//     }
	// }
	//     </code>
	// See Also:
	//     LPFNANIMATION, XTAnimationType, Animate, DefaultAnimation
	// -----------------------------------------------------------------------
	static void SetCustomAnimation(LPFNANIMATION pCustom);

private:

	// -------------------------------------------------------------------
	// Summary:
	//     Private LPFNANIMATION pointer that points to the address of a
	//     user defined animation method.
	// -------------------------------------------------------------------
	static LPFNANIMATION m_pCustomAnimation;

	// ---------------------------------------------------------
	// Summary:
	//     Private method is used internally for alpha-blending.
	// ---------------------------------------------------------
	static void AlphaBlendU(PBYTE, PBYTE, int, int, PBYTE, BYTE);
};

#endif // !defined(__XTANIMATIONMEMDC_H__)
