// XTPDrawHelpers.h: interface for the CXTPDrawHelpers class.
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
#if !defined(__XTPDRAWHELPERS_H__)
#define __XTPDRAWHELPERS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "XTPColorManager.h"

//===========================================================================
// Summary:
//     CXTPTransparentBitmap is a helper class used to extract the
//     transparent color from a transparent BitMap.  Also, this class
//     is used to convert a transparent BitMap into a transparent icon.
//===========================================================================
class _XTP_EXT_CLASS CXTPTransparentBitmap
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor.  Construct a new CXTPTransparentBitmap from a
	//     handle to an existing BitMap.
	// Parameters:
	//     hBitmap - Handle to an existing BitMap.
	//-----------------------------------------------------------------------
	CXTPTransparentBitmap(HBITMAP hBitmap);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the transparent color of the BitMap.
	// Returns:
	//     -1 if the BitMap is NULL.
	//     Otherwise, a COLORREF that contains the transparent color of the BitMap.
	//-----------------------------------------------------------------------
	COLORREF GetTransparentColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create an icon based on the BitMap.
	// Returns:
	//     NULL if the BitMap is NULL.
	//     NULL if the width or height of the BitMap is 0.
	//     Otherwise, a handle to the icon created from the BitMap.
	//-----------------------------------------------------------------------
	HICON ConvertToIcon() const;

protected:

	HBITMAP m_hBitmap;  // A handle to a BtiMap.
};

//===========================================================================
// Summary:
//     CXTPClientCursorPos is a helper class used to retrieve the cursor
//     position in client coordinates.
//===========================================================================
class _XTP_EXT_CLASS CXTPClientCursorPos : public CPoint
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPClientCursorPos object used to retrieve the
	//     cursor position in client coordinates.
	// Parameters:
	//     pWnd - Pointer to the window that contains the client area to
	//            get the cursor position for.
	//-----------------------------------------------------------------------
	CXTPClientCursorPos(CWnd* pWnd);
};

//===========================================================================
// Summary:
//     CXTPEmptyRect is a helper class used to instantiate an empty
//     CRect object.
//===========================================================================
class _XTP_EXT_CLASS CXTPEmptyRect : public CRect
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPEmptyRect object used to instantiate an
	//     empty CRect object.
	//-----------------------------------------------------------------------
	CXTPEmptyRect();
};

//===========================================================================
// Summary:
//     CXTPWindowRect is a helper class used to retrieve the screen
//     size for a specified window.
//===========================================================================
class _XTP_EXT_CLASS CXTPWindowRect : public CRect
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPWindowRect object used to retrieve the
	//     screen size for the specified window.
	// Parameters:
	//     hWnd - Handle to the window to retrieve the screen size for.
	//-----------------------------------------------------------------------
	CXTPWindowRect(HWND hWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPWindowRect object used to retrieve the
	//     screen size for the specified window.
	// Parameters:
	//     pWnd - Points to the window to retrieve the screen size for.
	//-----------------------------------------------------------------------
	CXTPWindowRect(const CWnd* pWnd);
};

//===========================================================================
// Summary:
//     CXTPClientRect is a helper class used to retrieve the client
//     area for a specified window.
//===========================================================================
class _XTP_EXT_CLASS CXTPClientRect : public CRect
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPClientRect object used to retrieve the
	//     client area for the specified window.
	// Parameters:
	//     hWnd - Handle to the window to retrieve the client area for.
	//-----------------------------------------------------------------------
	CXTPClientRect(HWND hWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPClientRect object used to retrieve the
	//     client area for the specified window.
	// Parameters:
	//     pWnd - Points to the window to retrieve the client area for.
	//-----------------------------------------------------------------------
	CXTPClientRect(const CWnd* pWnd);
};

//===========================================================================
// Summary:
//     CXTPBufferDC is a helper class used to create a memory device
//     context used to draw to an off-screen bitmap.  When destroyed, the
//     class selects the previous bitmap back into the device context to
//     handle GDI resource cleanup.
//===========================================================================
class _XTP_EXT_CLASS CXTPBufferDC : public CDC
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPBufferDC object used to create a memory
	//     device context used to draw to an off-screen bitmap.
	// Parameters:
	//     hDestDC - Handle to the destination device context the memory
	//               device is BitBlt to.
	//     rcPaint - Size of the area to paint.
	//-----------------------------------------------------------------------
	CXTPBufferDC(HDC hDestDC, const CRect rcPaint);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPBufferDC object, handles cleanup and de-
	//     allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPBufferDC();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve a CDC pointer to the destination
	//     device context.
	//-----------------------------------------------------------------------
	CDC* GetDestDC();

protected:

	HDC     m_hDestDC;    // Handle to the destination device context.
	CBitmap m_bitmap;     // Bitmap in memory device context
	CRect   m_rect;       // Size of the area to paint.
	HGDIOBJ m_hOldBitmap; // Handle to the previously selected bitmap.
};

//===========================================================================
// Summary:
//     CXTPFontDC is a helper class used to select a font into the
//     device context specified by hDC.  When destroyed, the class
//     selects the previous font back into the device context to
//     handle GDI resource cleanup.
//===========================================================================
class _XTP_EXT_CLASS CXTPFontDC
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFontDC object and selects the specified font
	//     into the device context specified by pDC.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pFont - Pointer to a CFont object to select into the device
	//             context.
	//-----------------------------------------------------------------------
	CXTPFontDC(CDC* pDC, CFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFontDC object, handles cleanup and de-
	//     allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPFontDC();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to change the font selected by the
	//     device context.
	// Parameters:
	//     pFont - Pointer to a CFont object to select into the device
	//             context.
	//-----------------------------------------------------------------------
	void SetFont(CFont* pFont);

protected:

	HDC     m_hDC;      // Device context handle.
	HGDIOBJ m_hOldFont; // Handle to the previously selected font.
};

//===========================================================================
// Summary:
//     CXTPPenDC is a helper class used to create a pen using
//     the color specified by crColor.  The pen is then selected
//     into the device context specified by hDC.  When destroyed, the
//     class selects the previous pen back into the device context to
//     handle GDI resource cleanup.
//===========================================================================
class _XTP_EXT_CLASS CXTPPenDC
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPenDC object and creates a pen using
	//     the color specified by crColor.  The pen is then selected
	//     into the device context specified by hDC.
	// Parameters:
	//     hDC     - Handle to a valid device context.
	//     crColor - RGB value used to create pen.
	//-----------------------------------------------------------------------
	CXTPPenDC(HDC hDC, COLORREF crColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPenDC object, handles cleanup and de-
	//     allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPenDC();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to change the color used by the
	//     device context pen.
	// Parameters:
	//     crColor - RGB value to change the pen color to.
	//-----------------------------------------------------------------------
	void Color(COLORREF crColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the color used by the
	//     device context pen.
	// Returns:
	//     An RGB value that represents the selected pen color.
	//-----------------------------------------------------------------------
	COLORREF Color();

protected:

	CPen m_pen;     // Pen selected into device context.
	HDC  m_hDC;     // Device context handle.
	HPEN m_hOldPen; // Handle to the previously selected pen.
};

//===========================================================================
// Summary:
//     CXTPBrushDC is a helper class used to create a  brush using
//     the color specified by crColor.  The brush is then selected
//     into the device context specified by hDC.  When destroyed, the
//     class selects the previous brush back into the device context to
//     handle GDI resource cleanup.
//===========================================================================
class _XTP_EXT_CLASS CXTPBrushDC
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPBrushDC object and creates a brush using
	//     the color specified by crColor.  The brush is then selected
	//     into the device context specified by hDC.
	// Parameters:
	//     hDC     - Handle to a valid device context.
	//     crColor - RGB value used to create brush.
	//-----------------------------------------------------------------------
	CXTPBrushDC(HDC hDC, COLORREF crColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPBrushDC object, handles cleanup and de-
	//     allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPBrushDC();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to change the color used by the
	//     device context brush.
	// Parameters:
	//     crColor - RGB value to change the brush color to.
	//-----------------------------------------------------------------------
	void Color(COLORREF crColor);

protected:

	CBrush m_brush;     // Brush selected into device context.
	HDC    m_hDC;       // Device context handle.
	HBRUSH m_hOldBrush; // Handle to the previously selected brush.
};

//===========================================================================
// Summary:
//     CXTPCompatibleDC is a helper class used to create a memory device
//     context (DC) compatible with the device specified by pDC.  When
//     destroyed, the class selects the previous bitmap back into the
//     device context to handle GDI resource cleanup.
//===========================================================================
class _XTP_EXT_CLASS CXTPCompatibleDC : public CDC
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCompatibleDC object and creates a memory
	//     device context (DC) compatible with the device specified by
	//     pDC.  The bitmap specified by pBitmap is then selected into
	//     the device context.
	// Parameters:
	//     pDC     - Points to a valid device context.
	//     pBitmap - Points to the previously selected bitmap.
	//-----------------------------------------------------------------------
	CXTPCompatibleDC(CDC* pDC, CBitmap* pBitmap);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCompatibleDC object, handles cleanup and de-
	//     allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPCompatibleDC();

protected:

	CBitmap* m_pBmp; // Pointer to the previously selected bitmap.
};

//===========================================================================
// Summary:
//     CXTPSplitterTracker is a stand alone class. It is used
//     to track a splitter rectangle.
//===========================================================================
class _XTP_EXT_CLASS CXTPSplitterTracker
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPSplitterTracker object.
	// Parameters:
	//     bSolid - TRUE to use solid tracker; FALSE for HalftoneBrush.
	//-----------------------------------------------------------------------
	CXTPSplitterTracker(BOOL bSolid = FALSE);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to track size of the splitter.
	// Parameters:
	//     pTrackWnd - Pointer to a CWnd object.
	//     rcAvail - Available rectangle of tracking.
	//     rectTracker - Current rectangle of tracking.
	//     point - Starting point.
	//     bHoriz - TRUE to track horizontally.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL Track(CWnd* pTrackWnd, CRect rcAvail, CRect& rectTracker, CPoint point, BOOL bHoriz);

	//-----------------------------------------------------------------------
	// Summary:
	//     Controls the accumulation of bounding-rectangle information for the specified device context.
	// Parameters:
	//     rcBoundRect - A CRect object that specifies the boundaries of the bounding rectangle.
	//-----------------------------------------------------------------------
	void SetBoundRect(CRect rcBoundRect);

private:
	//-----------------------------------------------------------------------
	// Summary:
	//     This function is called by the framework during the resizing of splitter windows.
	//     This function inverts the contents of the Splitter Tracker rectangle.
	//     Inversion is a logical NOT operation and flips the bits of each pixel.
	// Parameters:
	//     rc - A CRect object that specifies the XY position of the Splitter Tracker.
	//-----------------------------------------------------------------------
	void OnInvertTracker(CRect rc);

private:
	CDC* m_pDC;
	BOOL m_bSolid;
	CRect m_rcBoundRect;
};

//===========================================================================
// Summary:
//     The class CXTPMouseMonitor is a helper class that is used to
//     monitor mouse messages for in-place controls.  This class is used
//     in XTPPropertyGridInPlaceList and XTPReportInplaceControls.
//===========================================================================
class _XTP_EXT_CLASS CXTPMouseMonitor
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set up a hook to monitor mouse messages.
	// Parameters:
	//     pWndMonitor - A pointer to a CWnd object.  Used to determine
	//                   which monitor currently contains the mouse cursor.
	//-----------------------------------------------------------------------
	static void SetupHook(CWnd* pWndMonitor);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     The MouseProc hook procedure is an application-defined or
	//     library-defined callback function used with the SetWindowsHookEx
	//     function. The system calls this function whenever an application
	//     calls the GetMessage or PeekMessage function and there is a mouse
	//     message to be processed.
	// Parameters:
	//     nCode  - Specifies a code the hook procedure uses to determine
	//          how to process the message. This parameter can be one of the
	//          following values: HC_ACTION and HC_NOREMOVE.
	//          HC_ACTION   - The wParam and lParam parameters contain information about a mouse message.
	//          HC_NOREMOVE - The wParam and lParam parameters contain information about a mouse message,
	//                        and the mouse message has not been removed from the message queue. (An
	//                        application called the PeekMessage function, specifying the PM_NOREMOVE flag.)
	//     wParam - Specifies the identifier of the mouse message.
	//     lParam - Pointer to a MOUSEHOOKSTRUCT structure.
	// Returns:
	//     If nCode is less than zero, the hook procedure must return the value returned by CallNextHookEx.
	//     If nCode is greater than or equal to zero, and the hook procedure did not process the message,
	//     it is highly recommended that you call CallNextHookEx and return the value it returns; otherwise,
	//     other applications that have installed WH_MOUSE hooks will not receive hook notifications and may
	//     behave incorrectly as a result. If the hook procedure processed the message, it may return a nonzero
	//     value to prevent the system from passing the message to the target window procedure.
	//-----------------------------------------------------------------------
	static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
	static HHOOK m_hHookMouse;      // A handle to a hook.
	static CWnd* m_pWndMonitor;     // A pointer to the in-place control.
};


//===========================================================================
// Summary:
//     CXTPDrawHelpers is a helper class used to perform specialized
//     drawing tasks.  You can use this class to perform such tasks as
//     gradient fills and blending colors.
//===========================================================================
class _XTP_EXT_CLASS CXTPDrawHelpers
{
protected:
	//===========================================================================
	// Summary:
	//     Function pointer used to access the Windows API GradientFill function.
	//===========================================================================
	typedef BOOL (__stdcall *PFNGRADIENTFILL)(HDC, PTRIVERTEX, ULONG, PVOID, ULONG, ULONG);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDrawHelpers object.
	//-----------------------------------------------------------------------
	CXTPDrawHelpers();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXCSplitPath object, handles cleanup and de-
	//     allocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPDrawHelpers();

private:
	HMODULE m_hMsImgDll; // Handle to MsImg32.dll.

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to perform a gradient fill using
	//     the specified COLORREF values crFrom and crTo.
	// Parameters:
	//     pDC    - Points to the device context
	//     lpRect - Size of area to fill
	//     crFrom - RGB value to start from
	//     crTo   - RGB value to fill to
	//     bHorz  - TRUE if fill is horizontal.
	//-----------------------------------------------------------------------
	void GradientFill(CDC* pDC, LPRECT lpRect, COLORREF crFrom, COLORREF crTo, BOOL bHorz);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to perform a gradient fill using
	//     the specified CCXTPPaintManagerColorGradient structure.
	// Parameters:
	//     pDC    - Points to the device context
	//     lpRect - Size of area to fill
	//     grc    - CXTPPaintManagerColorGradient struct containing start from and
	//              fill to RGB color values.
	//     bHorz  - TRUE if fill is horizontal.
	//-----------------------------------------------------------------------
	void GradientFill(CDC* pDC, LPRECT lpRect, CXTPPaintManagerColorGradient& grc, BOOL bHorz);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to exclude the pixels for each corner of the
	//     area specified by rc.
	// Parameters:
	//     pDC - Points to the device context
	//     rc  - Area to exclude corners from
	//-----------------------------------------------------------------------
	void ExcludeCorners(CDC* pDC, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to blur the color value for the
	//     points specified by pts
	// Parameters:
	//     pDC    - Points to the device context
	//     pts    - Array of points to blur
	//     nCount - Number of points in array.
	//-----------------------------------------------------------------------
	void BlurPoints(CDC* pDC, LPPOINT pts, int nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to blend the colors specified by
	//     crA and crB.
	// Parameters:
	//     crA      - RGB value to blend
	//     crB      - RGB value to blend
	//     fAmountA - The amount that crA saturates crB.
	// Returns:
	//     An RGB value that represents the blended color.
	//-----------------------------------------------------------------------
	COLORREF BlendColors(COLORREF crA, COLORREF crB, float fAmountA);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the specified font
	//     exists.
	// Parameters:
	//     strFaceName - A NULL terminated string that represents the
	//                   font face name.
	// Returns:
	//     TRUE if the font was found, otherwise FALSE.
	//-----------------------------------------------------------------------
	static BOOL AFX_CDECL FontExists(CString& strFaceName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves DEFAULT_GUI_FONT font name.
	// Returns:
	//     Face name of DEFAULT_GUI_FONT font.
	//-----------------------------------------------------------------------
	static CString AFX_CDECL GetDefaultFontName();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves vertical font name
	// Parameters:
	//     bUseOfficeFont - TRUE to use "Tahoma" font if exists.
	// Returns:
	//     Face name of vertical font.
	//-----------------------------------------------------------------------
	CString GetVerticalFontName(BOOL bUseOfficeFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw a triangle onto the
	//     specified device context.
	// Parameters:
	//     pDC - Points to the device context
	//     pt0 - First point of the triangle
	//     pt1 - Second point of the triangle
	//     pt2 - Third point of the triangle
	//     clr - An RGB value that represents the fill color.
	//-----------------------------------------------------------------------
	static void AFX_CDECL Triangle(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2, COLORREF clr);
	static void AFX_CDECL Triangle(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2); // <combine CXTPDrawHelpers::Triangle@CDC*@CPoint@CPoint@CPoint@COLORREF>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the system display
	//     is set to low resolution.
	// Parameters:
	//     hDC - Handle to a device context, if NULL the desktop window
	//           device context is used.
	// Returns:
	//     TRUE if the system display is set to low resolution,
	//     otherwise FALSE.
	//-----------------------------------------------------------------------
	static BOOL AFX_CDECL IsLowResolution(HDC hDC = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine the current Windows XP
	//     theme in use.
	// Returns:
	//     A XTPCurrentSystemTheme enumeration that represents the
	//     current Windows theme in use, can be one of the following
	//     values:
	//     * <b>xtpSystemThemeUnknown</b> Indicates no known theme in use
	//     * <b>xtpSystemThemeBlue</b> Indicates blue theme in use
	//     * <b>xtpSystemThemeOlive</b> Indicates olive theme in use
	//     * <b>xtpSystemThemeSilver</b> Indicates silver theme in use
	//-----------------------------------------------------------------------
	XTPCurrentSystemTheme GetCurrentSystemTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if GradientFill from msimg32.dll available.
	// Returns:
	//     TRUE if GradientFill can be used
	//-----------------------------------------------------------------------
	BOOL IsFastFillAvailable();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to fill rectangle and triangle structures
	//     with color that smoothly fades from one side to the other.
	// Parameters:
	//     hdc         - Handle to a device context.
	//     pVertex     - Pointer to an array of TRIVERTEX structures that
	//                   each define a triangle vertex.
	//     dwNumVertex - The number of vertices.
	//     pMesh       - Array of GRADIENT_TRIANGLE structures in triangle
	//                   mode, or an array of GRADIENT_RECT structures in
	//                   rectangle mode.
	//     dwNumMesh   - The number of elements (triangles or rectangles)
	//                   in pMesh.
	//     dwMode      - Specifies gradient fill mode. For a list of possible
	//                   values, see GradientFill in the Platform SDK.
	// Returns:
	//     TRUE if successful; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GradientFill(HDC hdc, PTRIVERTEX pVertex, ULONG dwNumVertex, PVOID pMesh, ULONG dwNumMesh, ULONG dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function retrieves work area for the display monitor that
	//     the mouse cursor is currently positioned over.
	// Parameters:
	//     rect  - Reference to a CRect class that specifies the
	//             coordinates of the rectangle of interest in virtual
	//             screen coordinates.
	//     pWnd   - Pointer to the window of interest.
	// Returns:
	//     If the mouse cursor is positioned over a display monitor, the
	//     return value is a CRect object that specifies the work area
	//     rectangle of the display monitor, expressed in virtual-screen
	//     coordinates.  Otherwise, returns the size of the work area on
	//     the primary display monitor.
	//-----------------------------------------------------------------------
	static CRect AFX_CDECL GetWorkArea();
	static CRect AFX_CDECL GetWorkArea(const CRect& rect); //<COMBINE CXTPDrawHelpers::GetWorkArea>
	static CRect AFX_CDECL GetWorkArea(const CWnd* pWnd);  //<COMBINE CXTPDrawHelpers::GetWorkArea>

private:

	PFNGRADIENTFILL m_pfnFastGradientFill;

	// private members used for draw routines.
	void GradientFillFast(CDC* pDC, LPRECT lpRect, COLORREF crFrom, COLORREF crTo, BOOL bHorz);
	void GradientFillSlow(CDC* pDC, LPRECT lpRect, COLORREF crFrom, COLORREF crTo, BOOL bHorz);

	// singleton
	static CXTPDrawHelpers m_instance;          // The only instance of the CXTPDrawHelpers class.
	friend CXTPDrawHelpers* XTPDrawHelpers();   // Used to access the CXTPDrawHelpers object.
};


//---------------------------------------------------------------------------
// Summary:
//     Call this member function to access CXTPDrawHelpers members.
//     Since this class is designed as a single instance object you can
//     only access its members through this method. You <b>cannot</b>
//     directly instantiate an object of type CXTPDrawHelpers.
// Example:
//     <code>BOOL bLowRes = XTPDrawHelpers()->IsLowResolution();</code>
//---------------------------------------------------------------------------
CXTPDrawHelpers* XTPDrawHelpers();




AFX_INLINE CXTPDrawHelpers* XTPDrawHelpers() {
	return &CXTPDrawHelpers::m_instance;
}
AFX_INLINE BOOL CXTPDrawHelpers::IsFastFillAvailable() {
	return m_pfnFastGradientFill != NULL;
}
AFX_INLINE void CXTPDrawHelpers::Triangle(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2) {
	CPoint pts[] = {pt0, pt1, pt2};
	pDC->Polygon(pts, 3);
}
AFX_INLINE void CXTPSplitterTracker::SetBoundRect(CRect rcBoundRect) {
	m_rcBoundRect = rcBoundRect;
}

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(__XTPDRAWHELPERS_H__)
