// XTPDockContext.h : interface for the CXTPDockContext class.
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
#if !defined(__XTPDOCKCONTEXT_H__)
#define __XTPDOCKCONTEXT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPCommandBarsDefines.h"

class CXTPToolBar;

//===========================================================================
// Summary:
//     CXTPDockContext is a stand alone class. It is used internally by
//     CXTPToolBar.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockContext
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockContext object
	// Parameters:
	//     pBar - Points to a CXTPToolBar object
	//-----------------------------------------------------------------------
	CXTPDockContext(CXTPToolBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockContext object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPDockContext(void);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user starts to move the toolbar.
	// Parameters:
	//     pt - Initial cursor position.
	//-----------------------------------------------------------------------
	virtual void StartDrag(CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user starts to resize the toolbar.
	// Parameters:
	//     nHitTest - Specifies the hit-test area code.
	//     pt       - Initial cursor position.
	//-----------------------------------------------------------------------
	virtual void StartResize(int nHitTest, CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to switch the docking position of the toolbar.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL ToggleDocking();


protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to start tracking loop
	//-------------------------------------------------------------------------
	virtual void Track();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to stretch child toolbar.
	// Parameters:
	//     pt - CPoint object specifies xy coordinates.
	//-----------------------------------------------------------------------
	void Stretch(CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to move child toolbar to the specified position
	// Parameters:
	//     pt - CPoint object specifies xy coordinates.
	//-----------------------------------------------------------------------
	void Move(CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to move child toolbar to resize child dialog bar.
	// Parameters:
	//     pt - CPoint object specifies xy coordinates.
	//-----------------------------------------------------------------------
	void Resize(CPoint pt);

private:
	static void AdjustRectangle(CRect& rect, CPoint pt);

protected:
	CXTPToolBar* m_pBar;                    // Child toolbar.
	CFrameWnd* m_pDockSite;                 // Frame site
	int m_nHitTest;                         // Hit Test code
	CPoint m_ptLast;                        // Last mouse position.

	XTPBarPosition m_uMRUDockPosition;      // Last docking position.

	CRect m_rectMRUDockPos;                 // Last docking rectangle.
	CPoint m_ptMRUFloatPos;                 // Last floating rectangle

	CRect m_rectDragDock;                   // Current docking rectangle
	CRect m_rectDragFrame;                  // Current floating rectangle.

private:
	friend class CXTPToolBar;
	friend class CXTPDockBar;
};

#endif //#if !defined(__XTPDOCKCONTEXT_H__)
