// XTPCommandBars.inl inlines of the CXTPCommandBars class.
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
#if !defined(__XTPCOMMANDBARSINLINES_H__)
#define __XTPCOMMANDBARSINLINES_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPCommandBarsResource.h"

//-----------------------------------------------------------------------
// Summary:
//     Call this member to determine if the specified bar position is left,
//     top, right, or bottom.
// Parameters:
//     barPosition - Bar position.
// Returns:
//     TRUE if the specified bar position is left, top, right, or bottom.
//     FALSE if the position is anything else.
//-----------------------------------------------------------------------
AFX_INLINE BOOL IsDockingPosition(XTPBarPosition barPosition)
{
	return barPosition == xtpBarLeft || barPosition == xtpBarTop ||
		barPosition == xtpBarRight || barPosition == xtpBarBottom;
}

//-----------------------------------------------------------------------
// Summary:
//     Call this member to determine if the specified button style is button caption
//     or button icon and caption.
// Parameters:
//     buttonStyle - Button style.
// Returns:
//     TRUE if the specified button style is button caption or button icon and caption.
//     FALSE otherwise.
//-----------------------------------------------------------------------
AFX_INLINE BOOL IsCaptionStyle(XTPButtonStyle buttonStyle)
{
	return buttonStyle == xtpButtonCaption || buttonStyle == xtpButtonIconAndCaption;
}

//-----------------------------------------------------------------------
// Summary:
//     Call this member to determine if the specified control type is popup control,
//     a button popup control, or a split button popup control.
// Parameters:
//     controlType - Control type.
// Returns:
//     TRUE if the specified control type is popup control, a button popup control,
//     or a split button popup control.  FALSE otherwise.
//-----------------------------------------------------------------------
AFX_INLINE BOOL IsPopupControlType(XTPControlType controlType)
{
	return controlType == xtpControlPopup || controlType == xtpControlButtonPopup || controlType == xtpControlSplitButtonPopup;
}

//-----------------------------------------------------------------------
// Summary:
//     Call this member to determine if the specified bar position is
//     left, right, or is a popup toolbar.
// Parameters:
//     barPosition - Bar position.
// Returns:
//     TRUE if the specified bar position is left, right, or is a
//     popup toolbar.  FALSE otherwise.
//-----------------------------------------------------------------------
AFX_INLINE BOOL IsVerticalPosition(XTPBarPosition barPosition)
{
	return barPosition == xtpBarLeft || barPosition == xtpBarRight || barPosition == xtpBarPopup;
}

//-----------------------------------------------------------------------
// Summary:
//     Call this member to determine if the menu animation type is
//     set to fade, slide, or unfold.
// Parameters:
//     animationType - Animation type.
// Returns:
//     TRUE if the animation type is set to fade, slide, or unfold.
//-----------------------------------------------------------------------
AFX_INLINE BOOL IsAnimateType(XTPAnimationType animationType)
{
	return animationType == xtpAnimateFade || animationType == xtpAnimateSlide || animationType == xtpAnimateUnfold;
}

AFX_INLINE BOOL IsCustomizeCommand(int nID) 
{
	return (nID >= XTP_ID_CUSTOMIZE_RESET && nID <= XTP_ID_CUSTOMIZE_BEGINAGROUP);
}

#endif
