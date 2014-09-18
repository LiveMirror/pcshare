// XTResize.h: interface for the CXTResize class.
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
#if !defined(__XTRESIZE_H__)
#define __XTRESIZE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// -------------------------------------------------------------------
// Summary:
//     Enumeration used to determine attributes for resizing windows.
// Remarks:
//     XTResize type defines the constants used by the CXTResize
//     class to determine how a window will appear when it is resized.
// See Also:
//     CXTResize
//
// <KEYWORDS xtResizeNoSizeIcon, xtResizeNoHorisontal, xtResizeNoVertical, xtResizeNoMinsize, xtResizeNoClipChildren, xtResizeNoTransparentGroup>
// -------------------------------------------------------------------
enum XTResize
{
	xtResizeNoSizeIcon         = 0x01, // Do not add size icon.
	xtResizeNoHorisontal       = 0x02, // No horizontal resizing.
	xtResizeNoVertical         = 0x04, // No vertical resizing.
	xtResizeNoMinsize          = 0x08, // Do not require a minimum size.
	xtResizeNoClipChildren     = 0x10, // Do not set clip children style.
	xtResizeNoTransparentGroup = 0x20, // Do not set transparent style for group boxes.
};

// ---------------------------------------------------------------------
// Remarks:
//     XT_SIZING structure is used by the CXTResize class to
//     maintain information for a particular window that is being sized.
// See Also:
//     CXTResize
// ---------------------------------------------------------------------
struct XT_SIZING
{
	UINT            id;     // Control identifier of the window sized.
	XT_RESIZERECT   rrc;    // Size of the window sized.
};

class CXTResize;

//===========================================================================
// Summary:
//     CXTResizeItem is a stand alone helper class. It is used by CXTResize to
//     maintain information about each item to be sized or moved.
//===========================================================================
class _XT_EXT_CLASS CXTResizeItem
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTResizeItem object
	// Parameters:
	//     pWnd        - Pointer to the window to be sized or moved.
	//     rrcSizing   - Reference to a CXTResizeRect object.
	//     rcWindow    - Reference to a CRect object.
	//     bAutoDelete - TRUE if the window is to be deleted.
	//-----------------------------------------------------------------------
	CXTResizeItem(CWnd* pWnd,const CXTResizeRect& rrcSizing,CRect& rcWindow,BOOL bAutoDelete);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTResizeItem object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTResizeItem();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Called by CXTResize to make group box controls window style
	//     transparent.
	// Parameters:
	//     pXTResize - Points the resize manager.
	// Returns:
	//     True if the WS_EX_TRANSPARENT style was set for the window,
	//     otherwise false.
	//-----------------------------------------------------------------------
	virtual bool MakeTransparent(CXTResize* pXTResize);

public:
	BOOL m_bIsGroupBox;             // TRUE if the window is a group box.
	BOOL m_bAutoDelete;             // TRUE if the window is to be deleted.
	BOOL m_bInitialSize;            // Initial size/move has been completed.
	CWnd* m_pWnd;                   // A pointer to the window to be sized or moved.
	CXTResizeRect m_rrcSizing;      // Sizing option.
	CXTResizeRect m_rrcWindow;      // Last control size.
	CXTResizeRect m_rrcInitWindow;  // Initial control size.
};

//===========================================================================
// Summary:
//     The CXTSizeIcon class is a CStatic derived helper class. It is used
//     by CXTResize to display the sizing grip in the lower right corner of
//     a sizing window.
//===========================================================================
class _XT_EXT_CLASS CXTSizeIcon : public CScrollBar
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTSizeIcon object
	//-----------------------------------------------------------------------
	CXTSizeIcon();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTSizeIcon object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTSizeIcon();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTSizeIcon)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	HCURSOR m_hCursor; // Handle to the cursor displayed for the size icon

};

//===========================================================================
// Summary:
//     CXTResize is a base class. It is used by resizing dialogs, property
//     sheets, and form views. It acts as a manager to maintain size and location
//     of the dialog and dialog items.
//===========================================================================
class _XT_EXT_CLASS CXTResize
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTResize object
	// Parameters:
	//     pWnd   - Points to the parent or owner window object, of type CWnd, to which the
	//              resizing object belongs.
	//     nFlags - Flags that are to be passed to CXTResize that specify the attributes
	//              of the resizing property page. See Remarks section for a list of flags.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:
	//          * <b>xtResizeNoSizeIcon</b> Do not add size icon.
	//          * <b>xtResizeNoHorisontal</b> No horizontal resizing.
	//          * <b>xtResizeNoVertical</b> No vertical resizing.
	//          * <b>xtResizeNoMinsize</b> Do not require a minimum size.
	//          * <b>xtResizeNoClipChildren</b> Do not set clip children style.
	//          * <b>xtResizeNoTransparentGroup</b> Do not set transparent style for group boxes.
	//-----------------------------------------------------------------------
	CXTResize(CWnd* pWnd, const UINT nFlags = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTResize object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTResize();

public:

	//-----------------------------------------------------------------------
	// <COMBINE CXTResize::SetResize@CWnd*@const CXTResizeRect&@CRect>
	//-----------------------------------------------------------------------
	void SetResize(const UINT nID, XT_RESIZE left, XT_RESIZE top, XT_RESIZE right, XT_RESIZE bottom);

	//-----------------------------------------------------------------------
	// <COMBINE CXTResize::SetResize@CWnd*@const CXTResizeRect&@CRect>
	//-----------------------------------------------------------------------
	void SetResize(const UINT nID, const XT_RESIZERECT& rrcSizing);

	//-----------------------------------------------------------------------
	// <COMBINE CXTResize::SetResize@CWnd*@const CXTResizeRect&@CRect>
	//-----------------------------------------------------------------------
	void SetResize(const UINT nID, const HWND hWnd, const XT_RESIZERECT& rrcSizing);

	//-----------------------------------------------------------------------
	// <COMBINE CXTResize::SetResize@CWnd*@const CXTResizeRect&@CRect>
	//-----------------------------------------------------------------------
	void SetResize(const UINT nID, const HWND hWnd, const XT_RESIZEPOINT& rpTopLeft, const XT_RESIZEPOINT& rpBottomRight);

	//-----------------------------------------------------------------------
	// <COMBINE CXTResize::SetResize@CWnd*@const CXTResizeRect&@CRect>
	//-----------------------------------------------------------------------
	void SetResize(const UINT nID, const XT_RESIZEPOINT& rpTopLeft, const XT_RESIZEPOINT& rpBottomRight);

	//-----------------------------------------------------------------------
	// <COMBINE CXTResize::SetResize@CWnd*@const CXTResizeRect&@CRect>
	//-----------------------------------------------------------------------
	void SetResize(XT_SIZING arr[]);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the minimum size explicitly. Initial size
	//     is the default.
	// Parameters:
	//     sz - Specifies the minimum width and height the dialog can be sized to.
	//-----------------------------------------------------------------------
	void SetMinSize(CSize& sz);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the maximum size. No maximum is the default.
	// Parameters:
	//     sz - Specifies the maximum width and height the dialog can be sized to.
	//-----------------------------------------------------------------------
	void SetMaxSize(CSize& sz);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function saves the window placement to the registry.
	// Parameters:
	//     pszSection - Name of a section in the initialization file or a key in the Windows
	//                  registry where placement information is stored.
	//-----------------------------------------------------------------------
	void SavePlacement(LPCTSTR pszSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function loads saved window placement information from
	//     the registry.
	// Parameters:
	//     pszSection - Name of a section in the initialization file or a key in the Windows
	//                  registry where placement information is stored.
	//-----------------------------------------------------------------------
	void LoadPlacement(LPCTSTR pszSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function loads saved window placement information from
	//     the registry. This version is the same as LoadPlacement but there
	//     is no need for calling SavePlacement when the window is destroyed.
	//     This will be called automatically.
	// Parameters:
	//     pszSection - Name of a section in the initialization file or a key in the Windows
	//                  registry where placement information is stored.
	//-----------------------------------------------------------------------
	void AutoLoadPlacement(LPCTSTR pszSection);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     The SetResize function specifies how much each side of a control will
	//     move when the dialog is resized. If a control should be repositioned
	//     (e.g. an OK button) then all four sides should move by the same amount
	//     of pixels, as the dialog is resized. If a control should be resized
	//     just as much as the dialog (e.g. the list control in the file dialog),
	//     then the left and top sides shouldn't move, and the right and bottom
	//     sides should move by the same amount of pixels as the dialog.
	// Parameters:
	//     nID           - Specifies the control's ID.
	//     pWnd          - Points to the dialog item to be resized.
	//     hWnd          - HWND of the dialog item to be sized.
	//     rrcSizing     - How much the left, top, right, and bottom sides will move when
	//                     the dialog is resized.
	//     rcWindow      - Initial size of the dialog item.
	//     left          - How much the left side will move when the dialog is resized.
	//     top           - How much the top side will move when the dialog is resized.
	//     right         - How much the right side will move when the dialog is resized.
	//     bottom        - How much the bottom side will move when the dialog is resized.
	//     rpTopLeft     - How much the top and left sides will move when the dialog is resized.
	//     rpBottomRight - How much the bottom and right sides will move when the dialog is resized.
	//     arr           - Array of XT_SIZING structures that specify how much the left, top,
	//                     right, and bottom sides of the dialog item will move when the dialog
	//                     is resized.
	//-----------------------------------------------------------------------
	void SetResize(CWnd* pWnd, const CXTResizeRect& rrcSizing, CRect rcWindow);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to remove the specified dialog item
	//     from the list of items to be resized.
	// Parameters:
	//     nID - Specifies the control's ID.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL RemoveResize(const UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to purge the list that contains dialog
	//     items to be sized.
	//-----------------------------------------------------------------------
	void RemoveAllControls();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called from OnInitDialog or OnInitialUpdate
	//     to initialize the resize manager.
	//-----------------------------------------------------------------------
	void Init();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called from OnSize to move and resize the dialog
	//     items that are managed.
	//-----------------------------------------------------------------------
	void Size();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called when a property sheet in wizard mode
	//     has changed pages to alert the resize manager that the property sheet
	//     (common control) has moved the page back to its original size/position
	//     on the sheet.
	//-----------------------------------------------------------------------
	void Reset();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called from OnGetMinMaxInfo to get the
	//     maximized position or dimensions or the minimum or maximum tracking
	//     size. The maximized size is the size of the window when its borders
	//     are fully extended. The maximum tracking size of the window is the
	//     largest window size that can be achieved by using the borders to size
	//     the window. The minimum tracking size of the window is the smallest
	//     window size that can be achieved by using the borders to size the window.
	// Parameters:
	//     pMMI - Points to a MINMAXINFO structure that contains information about a
	//            window’s maximized size and position and its minimum and maximum
	//            tracking size. For more information about this structure, see the
	//            MINMAXINFO structure.
	//-----------------------------------------------------------------------
	void GetMinMaxInfo(MINMAXINFO* pMMI);

protected: // flags

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to determine if the specified flag
	//     has been set for the resize manager.
	// Parameters:
	//     eFlag - Flag to check, it can be one of the values in the Remarks section.
	// Remarks:
	//     Styles to be added can one of the following:
	//          * <b>xtResizeNoSizeIcon</b> Do not add size icon.
	//          * <b>xtResizeNoHorisontal</b> No horizontal resizing.
	//          * <b>xtResizeNoVertical</b> No vertical resizing.
	//          * <b>xtResizeNoMinsize</b> Do not require a minimum size.
	//          * <b>xtResizeNoClipChildren</b> Do not set clip children style.
	//          * <b>xtResizeNoTransparentGroup</b> Do not set transparent style
	//          for group boxes.
	// Returns:
	//     TRUE if the specified flag has been set, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL HasFlag(XTResize eFlag);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to set a specific flag for the resize
	//     manager.
	// Parameters:
	//     eFlag - Flag to check, it can be one of the values in the Remarks section.
	// Remarks:
	//     Styles to be added can one of the following:<p/>
	//     * <b>xtResizeNoSizeIcon</b> Do not add size icon.
	//     * <b>xtResizeNoHorisontal</b> No horizontal resizing.
	//     * <b>xtResizeNoVertical</b> No vertical resizing.
	//     * <b>xtResizeNoMinsize</b> Do not require a minimum size.
	//     * <b>xtResizeNoClipChildren</b> Do not set clip children style.
	//     * <b>xtResizeNoTransparentGroup</b> Do not set transparent style
	//       for group boxes.
	//-----------------------------------------------------------------------
	void SetFlag(XTResize eFlag);

protected: // helper methods

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by CXTResize to resize a child window.
	// Parameters:
	//     HDWP&    - [in] Handle to a multiple-window – position structure that contains size and position information for one or more windows. This structure is returned by BeginDeferWindowPos or by the most recent call to DeferWindowPos.
	//     CXTResizeItem* - [in] Pointer to a CXTResizeItem object that contains additional information about the window being sized.
	//     dx       - [in] Specifies the window's new width, in pixels.
	//     dy       - [in] Specifies the window's new height, in pixels.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL Defer(HDWP&, CXTResizeItem*, int dx, int dy);

protected:

	typedef CArray<CXTResizeItem*, CXTResizeItem*&> CResizeItemArray;   // Used by CXTResize to maintain a list of children that are sizable.

	UINT            m_nFlagsXX;     // flags passed from constructor
	CWnd*           m_pWnd;         // the associative relation to the window to be resized
	CRect           m_rcWindow;     // last dialog size
	CRect           m_rcInitWindow; // Initial dialog size
	CSize           m_szMin;        // smallest size allowed
	CSize           m_szMax;        // largest size allowed
	CString         m_strSection;   // section in registry where window placement information is saved.
	CXTSizeIcon     m_scSizeIcon;   // size icon window
	CResizeItemArray    m_arrItems;     // array of controls

	friend class CXTResizeItem;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTResize::SetMinSize(CSize& sz) {
	m_szMin = sz;
}
AFX_INLINE void CXTResize::SetMaxSize(CSize& sz) {
	m_szMax = sz;
}
AFX_INLINE BOOL CXTResize::HasFlag(XTResize eFlag) {
	return (m_nFlagsXX & eFlag) != 0;
}
AFX_INLINE void CXTResize::SetResize(const UINT nID, const HWND hWnd, const XT_RESIZEPOINT& rpTopLeft, const XT_RESIZEPOINT& rpBottomRight) {
	SetResize(nID, hWnd, CXTResizeRect(rpTopLeft.x, rpTopLeft.y, rpBottomRight.x, rpBottomRight.y));
}
AFX_INLINE void CXTResize::SetResize(const UINT nID, const XT_RESIZERECT& rrcSizing) {
	SetResize(nID,NULL,rrcSizing);
}
AFX_INLINE void CXTResize::SetResize(const UINT nID, const XT_RESIZEPOINT& rpTopLeft, const XT_RESIZEPOINT& rpBottomRight) {
	SetResize(nID, CXTResizeRect(rpTopLeft.x, rpTopLeft.y, rpBottomRight.x, rpBottomRight.y));
}
AFX_INLINE void CXTResize::SetResize(const UINT nID, XT_RESIZE left, XT_RESIZE top, XT_RESIZE right, XT_RESIZE bottom) {
	SetResize(nID, CXTResizeRect(left, top, right, bottom));
}

#endif // !defined(__XTRESIZE_H__)
