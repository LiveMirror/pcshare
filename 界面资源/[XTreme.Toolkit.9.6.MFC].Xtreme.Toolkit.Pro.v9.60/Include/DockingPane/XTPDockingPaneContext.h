// XTPDockingPaneContext.h : interface for the CXTPDockingPaneContext class.
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
#if !defined(__XTPDOCKINGPANECONTEXT_H__)
#define __XTPDOCKINGPANECONTEXT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPDockingPaneBase.h"

class CXTPDockingPane;
class CXTPDockingPaneManager;
class CXTPDockingPaneContext;

//-----------------------------------------------------------------------
// Summary:
//     XTPDockingPaneStickerType is an enumeration that is used to indicate
//     which docking stickers are to be drawn on the screen when a
//     docking pane is dragged.
// Remarks:
//     m_bUseAlphaContext must be TRUE to display docking stickers.
// See Also:
//     CXTPDockingPaneContext::CreateNewSticker,
//     CXTPDockingPaneContextStickerWnd::m_typeSticker, CXTPDockingPaneContextStickerWnd::m_selectedSticker
//
// <KEYWORDS xtpPaneStickerNone, xtpPaneStickerLeft, xtpPaneStickerRight, xtpPaneStickerTop, xtpPaneStickerBottom, xtpPaneStickerCenter, xtpPaneStickerClient, xtpPaneStickerPane>
//-----------------------------------------------------------------------
enum XTPDockingPaneStickerType
{
	xtpPaneStickerNone = 0,         // No docking stickers are displayed
	xtpPaneStickerLeft = 1,         // Display the docking sticker indicating the pane can be docked to the left.
	xtpPaneStickerRight = 2,        // Display the docking sticker indicating the pane can be docked to the right.
	xtpPaneStickerTop = 4,          // Display the docking sticker indicating the pane can be docked on the top.
	xtpPaneStickerBottom = 8,       // Display the docking sticker indicating the pane can be docked on the bottom.
	xtpPaneStickerCenter = 16,      // Display the docking sticker indicating the pane can be docked in the center.
	xtpPaneStickerClient = xtpPaneStickerLeft + xtpPaneStickerRight + xtpPaneStickerTop + xtpPaneStickerBottom,  // Display the left, top, bottom, and right docking stickers on the client frame.
	xtpPaneStickerPane = xtpPaneStickerClient + xtpPaneStickerCenter            // Display full sticker
};

//===========================================================================
// Summary:
//     CXTPDockingPaneContextAlphaWnd is CWnd derived class represents alpha context helper window
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneContextAlphaWnd : public CWnd
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneContextAlphaWnd object.
	//-----------------------------------------------------------------------
	CXTPDockingPaneContextAlphaWnd();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneContextAlphaWnd object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPDockingPaneContextAlphaWnd();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CXTPDockingPaneContextAlphaWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

	friend class CXTPDockingPaneContext;
};

//===========================================================================
// Summary:
//     CXTPDockingPaneContextStickerWnd is CWnd derived class represents sticker window.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneContextStickerWnd : public CWnd
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneContextStickerWnd object.
	//-----------------------------------------------------------------------
	CXTPDockingPaneContextStickerWnd();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneContextStickerWnd object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPDockingPaneContextStickerWnd();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw sticker
	// Parameters:
	//     pDC - Device context pointer
	//-----------------------------------------------------------------------
	void OnDraw(CDC* pDC);


//{{AFX_CODEJOCK_PRIVATE
public:
	XTPDockingPaneStickerType HitTest(CPoint pt);
	struct SPRITEINFO;

protected:
	void DrawSprite(CDC* pDC, UINT nID, SPRITEINFO* pSpriteInfo, BOOL bClientBitmap = TRUE);
	void DrawTransparent(CDC* pDC , const CPoint& ptDest, const CSize& sz, CBitmap* pBitmap);

protected:
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTPDockingPaneContextStickerWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

public:
	XTPDockingPaneStickerType m_typeSticker;      // Type of the sticker.
	XTPDockingPaneStickerType m_selectedSticker;  // Selected sticker part.

	friend class CXTPDockingPaneContext;

};

//===========================================================================
// Summary:
//     CXTPDockingPaneContext is a stand alone class. It is used internally
//     for docking and sizing panes.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneContext
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneContext object
	//-----------------------------------------------------------------------
	CXTPDockingPaneContext();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneContext object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPDockingPaneContext();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to drag a pane.
	// Parameters:
	//     pPane - Points to a CXTPDockingPaneBase object
	//     point - Initial mouse position.
	//-----------------------------------------------------------------------
	virtual void Drag(CXTPDockingPaneBase* pPane, CPoint point);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to track the size of the pane.
	//-----------------------------------------------------------------------
	virtual void Track();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called then user move mouse cursor in track mode.
	// Parameters:
	//     pt - New mouse cursor position
	//-----------------------------------------------------------------------
	virtual void Move(CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to initialize a loop.
	//-----------------------------------------------------------------------
	virtual void InitLoop();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called when a loop is canceled.
	//-----------------------------------------------------------------------
	virtual void CancelLoop();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the focus rectangle.
	// Parameters:
	//     bRemoveRect - TRUE to delete focus rect.
	//-----------------------------------------------------------------------
	virtual void DrawFocusRect(BOOL bRemoveRect = FALSE);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to adjust the rectangle.
	// Parameters:
	//     rect - Rectangle to be changed.
	//     pt   - Position of the rectangle to be changed.
	//-----------------------------------------------------------------------
	static void AFX_CDECL AdjustRectangle(CRect& rect, CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to create a context menu.
	// Parameters:
	//     pWnd - A pointer to a CXTPDockingPaneContextAlphaWnd object.
	//-----------------------------------------------------------------------
	void CreateContextWindow(CXTPDockingPaneContextAlphaWnd* pWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to refresh the docking stickers.
	//-----------------------------------------------------------------------
	void UpdateDockingStickers();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove the docking stickers.
	//-----------------------------------------------------------------------
	void DestroyDockingStickers();

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates new sticker window
	// Parameters:
	//     rc          - Boundary rectangle of the sticker
	//     typeSticker - Type of the sticker to create
	// Returns:
	//     New Sticker window
	//-----------------------------------------------------------------------
	CXTPDockingPaneContextStickerWnd* CreateNewSticker(CRect rc, XTPDockingPaneStickerType typeSticker);

protected:
//{{AFX_CODEJOCK_PRIVATE
	void _CreateRectangleRgn(CRgn& rgnResult, CRect rc);
	void _CreateRgn(CRgn& rgnResult, CRect rc, BOOL bTabbedRgn, BOOL bRemove = FALSE);
	virtual void FindContainer(CPoint pt);
	BOOL CanDock(CRect rc, CPoint pt, CXTPDockingPaneBase* pPane, BOOL bInside = TRUE);

	void RegionFromBitmap(CRgn* pRgn, CDC* pDC, CRect rc);
	BOOL IsAllowDockingTo(CXTPDockingPaneBase* pPane, XTPDockingPaneDirection direction);
	BOOL IsAllowAttachTo(CXTPDockingPaneBase* pPane);
//}}AFX_CODEJOCK_PRIVATE

private:
	int StickerToOffset(XTPDockingPaneStickerType sticker);
	void IncludeRgnPart(CRgn* pRgn, int x1, int y, int x2);

protected:

	CXTPDockingPaneContextAlphaWnd m_wndContext;        // Alpha context control.
	CXTPDockingPaneContextAlphaWnd m_wndAttachedTab;    // Attached tab control.

	PVOID m_pfnSetLayeredWindowAttributes;              // Pointer to SetLayeredWindowAttributes method.

	BOOL m_bUseDockingStickers;                         // TRUE to use docking stickers.
	BOOL m_bUseAlphaContext;                            // TRUE to use alpha context.

	CRect m_rectStickerPane;                            // Area occupied by the sticker pane.
	CXTPDockingPaneBase* m_pStickerPane;                // Sticker Pane.

	CList<CXTPDockingPaneContextStickerWnd*, CXTPDockingPaneContextStickerWnd*> m_lstStickers;  //List of the stickers
	CPoint m_ptLast;                                    // Holds the last selection point.
	CPoint m_ptSticky;                                  // Position of the sticky control.
	CRect m_rectDragFrame;                              // Area occupied by the drag frame.
	CXTPDockingPaneManager* m_pManager;                 // Parent pane manager.
	CXTPDockingPaneBase* m_pPane;                       // Pointer to the associated pane.

protected:
//{{AFX_CODEJOCK_PRIVATE
	CXTPDockingPaneBase* m_pContainer;
	CRect m_rectContainer;
	BOOL  m_bAttachLast;
	BOOL  m_bAttach;
	BOOL  m_bFloatable;
	CRect m_rectLast;
	CSize m_sizeLast;
	BOOL  m_bDitherLast;
	CRgn  m_rgnStickers[5];
	XTPDockingPaneDirection m_containDirection;
	CDC* m_pDC;
//}}AFX_CODEJOCK_PRIVATE

private:
	friend class CXTPDockingPaneManager;
	friend class CXTPDockingPaneContextAlphaWnd;
};

#endif // #if !defined(__XTPDOCKINGPANECONTEXT_H__)
