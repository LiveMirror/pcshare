// XTMemDC.h interface for the CXTMemDC class.
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
#if !defined(__XTMEMDC_H__)
#define __XTMEMDC_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTMemDC is a CDC derived class. CXTMemDC is an extension of CDC
//     that helps eliminate screen flicker when windows are resized, by
//     painting to an off screen bitmap. The class then uses CDC::BitBlt
//     to copy the bitmap back into the current device context after all
//     items have been painted.
//===========================================================================
class _XT_EXT_CLASS CXTMemDC : public CDC
{
	DECLARE_DYNAMIC(CXTMemDC)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTMemDC object used for flicker free drawing.
	// Parameters:
	//     pDC      - A Pointer to the current device context.
	//     rect     - Represents the size of the area to paint.
	//     clrColor - An RGB value that represents the current system
	//                face color of three dimensional display elements.
	//-----------------------------------------------------------------------
	CXTMemDC(CDC* pDC,const CRect& rect,COLORREF clrColor=GetSysColor(COLOR_3DFACE));

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTMemDC object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTMemDC();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to set the valid flag to false
	//     so that the off screen device context will not be drawn.
	//-----------------------------------------------------------------------
	void Discard();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets content from the given DC
	//-----------------------------------------------------------------------
	void FromDC();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves a reference to the CBitmap
	//     object associated with the memory device context.
	// Returns:
	//     A reference to the CBitmap object associated with the memory
	//     device context.
	//-----------------------------------------------------------------------
	CBitmap& GetBitmap();

protected:
	CDC*    m_pDC;          // Saves the CDC passed in constructor.
	BOOL    m_bValid;       // Flag used for autodraw in destructor.
	CRect   m_rc;           // Rectangle of the drawing area.
	CBitmap m_bitmap;       // Off screen bitmap.
	HBITMAP m_hOldBitmap;   // Original GDI object.
};

#endif // #if !defined(__XTMEMDC_H__)
