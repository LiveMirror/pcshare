// XTHelpers.h interface
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

#if !defined(__XTHELPERS_H__)
#define __XTHELPERS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Input:   TControl - Control type (must implement void SetItemState(TItemHandle hItem, UINT nBits, UINT nMask))
//          TItemHandle - Item handle type, passed verbatim to SetItemState()
// Summary: This function translates shell attributes into a (tree,list ) item state
template <class TControl, class TItemHandle>

	// Input:   pControl - Control that owns an item
	//          item - handle to identify the item, passed verbatim to SetItemState()
	//          dwAttributes - Shell attributes to translate
	//          dwCutBit - Control-specific bit to mark the item is rendered with a 'cut' feedback
	void MapShellFlagsToItemAttributes(TControl* pControl,TItemHandle hItem,DWORD dwAttributes,DWORD dwCutBit)
{
	// Display with 'cut' feedback if the item is ghosted (about to be moved to a different location or a hidden one)
	if (dwAttributes & SFGAO_GHOSTED)
	{
		pControl->SetItemState(hItem, dwCutBit, dwCutBit);
	}
	else
	{
		pControl->SetItemState(hItem, 0, dwCutBit);
	}

	UINT nImage; // assign proper overlay image (link, share)
	if (dwAttributes & SFGAO_LINK)
	{
		nImage = INDEXTOOVERLAYMASK(2);
	}
	else
	if (dwAttributes & SFGAO_SHARE)
	{
		nImage = INDEXTOOVERLAYMASK(1);
	}
	else
	{
		nImage = 0; // no overlay
	}
	// NB: we use here TVIS_OVERLAYMASK (as for the tree control) though
	// the list view control also calls this function and LVIS_OVERLAYMASK
	// should be used in that case. Reason for that is both of them refer
	// to results of INDEXTOOVERLAYMASK macro and hence they have to be the same,
	// namely INDEXTOOVERLAYMASK(15)
	pControl->SetItemState(hItem, nImage, TVIS_OVERLAYMASK);
}

// Helper class to handle deferred resizing/moving windows
class CXTDeferWindowPosHandler
{
	HDWP        m_hdwp;         // defer position handle
	const int   m_nNumWindows;  // max. number of windows
public:
	// Input:   nNumWidows - number of windows to reserve for
	// Summary: Constructor, takes a maxed out number of windows for resizing/moving
	CXTDeferWindowPosHandler(int nNumWidows)
	: m_hdwp(NULL)
	, m_nNumWindows(nNumWidows)
	{  }

	// Summary: Destructor ensures the windows are repositioned all at once
	~CXTDeferWindowPosHandler()
	{
		if (m_hdwp)
		{
			// initialization completed and no resource failure
			::EndDeferWindowPos(m_hdwp);
			m_hdwp = NULL;
		}
	}

	// Input:   pWnd - A window being moved/resized
	//          rect - Rectangle to move/resize the window into The window will be hidden if the
	//          rectangle is empty (or shown otherwise)
	// Summary: Defers moving/resizing window
	//          This function throws resource exceptions if Windows fails to allocate its internal structures
	void MoveWindow(CWnd* pWnd, CRect rect)
	{
		if (pWnd)
		{
			// ensure the handle is initialised properly
			if (m_hdwp == NULL)
			{
				m_hdwp = ::BeginDeferWindowPos(m_nNumWindows);
				if (m_hdwp == NULL)
				{
					// abort, low resources
					AfxThrowResourceException();
				}
			}

			UINT flags;
			if (rect.IsRectEmpty())
			{
				// ensure all coordinates are zero and hide the window
				rect.SetRectEmpty();
				flags = SWP_HIDEWINDOW;
			}
			else
			{
				flags = SWP_SHOWWINDOW; // ensure the window is shown at the given location
			}
			// now defere the request to move the window
			m_hdwp =::DeferWindowPos(m_hdwp, pWnd->m_hWnd, NULL,
					rect.left, rect.top, rect.Width(), rect.Height(),
					flags | SWP_NOZORDER | SWP_NOACTIVATE);
			if (m_hdwp == NULL)
			{
				// abort, low resources
				AfxThrowResourceException();
			}
		}
	}
};

// Safely selects object into device context
template <class T>
class CXTContextObjectHandler
{
	T* m_pObject;
	CDC* m_pDC;
public:
	CXTContextObjectHandler(CDC* pDC, T* object)
	: m_pDC(pDC), m_pObject(pDC->SelectObject(object))
	{  }

	~CXTContextObjectHandler()
	{
		if (m_pObject)
		{
			m_pDC->SelectObject(m_pObject);
		}
	}
};

// Background mode context handler
class CXTContextBkModeHandler
{
	int m_nMode;
	CDC* m_pDC;
public:
	CXTContextBkModeHandler(CDC* pDC, int nNewMode)
	: m_pDC(pDC), m_nMode(pDC->SetBkMode(nNewMode))
	{ }

	~CXTContextBkModeHandler()
	{
		m_pDC->SetBkMode(m_nMode);
	}

};

// Text color context handler
class CXTContextTextColorHandler
{
	COLORREF m_clrFore;
	CDC* m_pDC;
public:
	CXTContextTextColorHandler(CDC* pDC, int clrFore)
	: m_pDC(pDC), m_clrFore(pDC->SetTextColor(clrFore))
	{ }

	~CXTContextTextColorHandler()
	{
		m_pDC->SetTextColor(m_clrFore);
	}

};

// Background color context handler
class CXTContextBkColorHandler
{
	COLORREF m_clrBack;
	CDC* m_pDC;
public:
	CXTContextBkColorHandler(CDC* pDC, int clrBack)
	: m_pDC(pDC), m_clrBack(pDC->SetBkColor(clrBack))
	{ }

	~CXTContextBkColorHandler()
	{
		m_pDC->SetBkColor(m_clrBack);
	}
};

// Handles viewport origin
class CXTContextViewPortOrgHandler
{
	CPoint  m_pointOrigin;
	CDC* m_pDC;
public:
	CXTContextViewPortOrgHandler(CDC* pDC, const CPoint& pointOrigin)
	: m_pDC(pDC), m_pointOrigin(pDC->SetViewportOrg(pointOrigin))
	{ }

	~CXTContextViewPortOrgHandler()
	{
		m_pDC->SetViewportOrg(m_pointOrigin);
	}
};

#ifndef _XTP_INCLUDE_CONTROLS
// Input:   pDC - Device context to draw on
//          (0,0) in cdc-coordinates is the top-left corner of rc2
//          clrBorder - color to draw the line
//          frc1 - irst rectangle
//          rc2 - second rectangle
// Summary: Draws a line between the two adjacent rects
void _XT_EXT_CLASS DrawAdjacentBorder(CDC* pDC,COLORREF clrBorder,const CRect& rc1, const CRect& rc2);
#endif

inline int GetDimension(CSize size, bool bHorz)
{
	return (bHorz) ? size.cx : size.cy;
}

inline int GetDimension(CPoint pt, bool bHorz)
{
	return (bHorz) ? pt.x : pt.y;
}

inline void SetDimension(CPoint& pt, bool bHorz, int dimension)
{
	if (bHorz)
		pt.x = dimension;
	else
		pt.y = dimension;
}

inline void SetDimension(CSize& size, bool bHorz, int dimension)
{
	if (bHorz)
		size.cx = dimension;
	else
		size.cy = dimension;
}

// Input:   pDC - Pointer to a valid device context.
//          rcDest - Size of the destination bitmap.
//          bits - bits pointer.
// Summary: Renders bits as a bitmap on CDC using current text color
void RenderBits(CDC* pDC, const CRect& rcDest, LPCVOID bits);


// Input:   rcTarget - on exit, gets coordinates of computed rectangle
//          rcScreen - screen coordinates
//          rect - exclusion rect, generated rectangle shall not overlap it and be adjacent to it
//          tpmFlags - flags controlling placement of the generated rectangle
//          Recognized values are:
//          [ul]
//          [li]<b>TPM_LEFTALIGN</b>   align left coordinate of the generated rectangle with the left
//                 coordinate of exclusion rect[/li]
//          [li]<b>TPM_RIGHTALIGN</b>  align right coordinate of the generated rectangle with the right
//                 coordinate of exclusion rect[/li]
//          [li]<b>TPM_TOPALIGN</b>    align bottom coordinate of the generated rectangle with the top
//                 coordinate of exclusion rect[/li]
//          [li]<b>TPM_BOTTOMALIGN</b> align top coordinate of the generated rectangle with the bottom
//                 coordinate of exclusion rect[/li]
//          [li]<b>TPM_HORIZONTAL</b>  when aligning, prefer shifting horizontally (default)[/li]
//          [li]<b>TPM_VERTICAL</b>    when aligning, prefer shifting vertically[/li]
//          [/ul]
//          extent - extent of the rectangle to generate
// Summary: Calculates a rectangle that would be completely visible inside screen rectangle
//          The computed rectangle will be place side-by-side with the exclusion rect
void CalcPopupRect(CRect* rcTarget,const CRect& rcScreen,const CRect& rcExclusion,UINT tpmFlags,CSize extent);

// Input:   rc1 -
//          rc2 -
// Summary: Shifts the rc2 rectangle such that it shares a common border (adjacent border)
//          with the other rectangle (rc1). The rectangles must be positioned side-by-side
void AdjoinRect(const CRect& rc1, CRect& rc2);

#endif // #if !defined(__XTHELPERS_H__)
