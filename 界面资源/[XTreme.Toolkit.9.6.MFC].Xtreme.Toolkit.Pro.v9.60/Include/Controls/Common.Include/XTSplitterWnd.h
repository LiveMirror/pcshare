// XTSplitterWnd.h : header file
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
#if !defined(__XTSPLITTERWND_H__)
#define __XTSPLITTERWND_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTSplitterTheme;

#include "Theme/XTThemeManager.h"

// --------------------------------------------------------------------
// Summary:
//     CXTSplitterWnd is a CSplitterWnd derived class. It
//     adds the ability to hide and show splitter panes based upon its
//     index.
// --------------------------------------------------------------------
class _XT_EXT_CLASS CXTSplitterWnd : public CSplitterWnd, public CXTThemeManagerStyleHost
{
	DECLARE_DYNAMIC(CXTSplitterWnd)
	DECLARE_THEME_HOST(CXTSplitterWnd, CXTSplitterTheme)

public:
	// ---------------------------------------------
	// Summary:
	//     Constructs a CXTSplitterWnd object
	// ---------------------------------------------
	CXTSplitterWnd();

	// ----------------------------------------------------------------
	// Summary:
	//     Destroys a CXTSplitterWnd object, handles cleanup and
	//     deallocation
	// ----------------------------------------------------------------
	virtual ~CXTSplitterWnd();


public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to show the column that was previously
	//     hidden.
	// See Also:
	//     HideColumn
	//-----------------------------------------------------------------------
	virtual void ShowColumn();

	//-----------------------------------------------------------------------
	// Summary:
	//    This member function will hide a column based upon its index.
	// Parameters:
	//    nColHide -  Index of the column to hide.
	// See Also:
	//     ShowColumn
	//-----------------------------------------------------------------------
	virtual void HideColumn(int nColHide);

	//-----------------------------------------------------------------------
	// Summary:
	//    This member function is called to show the row that was previously
	//    hidden.
	// See Also:
	//     HideRow
	//-----------------------------------------------------------------------
	virtual void ShowRow();

	//-----------------------------------------------------------------------
	// Summary:
	//    This member function will hide a row based upon its index.
	// Parameters:
	//     nRowHide -  Index of the row to hide.
	// See Also:
	//     ShowRow
	//-----------------------------------------------------------------------
	virtual void HideRow(int nRowHide);

	//-----------------------------------------------------------------------
	// Summary:
	//    This member function is called to switch, or swap, a splitter view
	//    with another.
	// Parameters:
	//    nRow -      Specifies a row.
	//    nCol -      Specifies a column.
	//    pNewView -  Specifies the view to switch the specified pane with.
	// Returns:
	//    TRUE if successful, otherwise returns FALSE.
	// See Also:
	//     ReplaceView
	//-----------------------------------------------------------------------
	virtual BOOL SwitchView(int nRow,int nCol,CView *pNewView);

	//-----------------------------------------------------------------------
	// Summary:
	//    This member function is called to replace an existing splitter view
	//    with another.
	// Parameters:
	//    nRow -        Specifies a row.
	//    nCol -        Specifies a column.
	//    pViewClass -  Specifies the runtime class to replace the specified pane with.
	//    pNewView -    Points to an already existing view.
	// Returns:
	//    A CView pointer to the view that was replaced, otherwise returns NULL.
	// See Also:
	//     SwitchView
	//-----------------------------------------------------------------------
	virtual CView* ReplaceView(int nRow,int nCol,CView* pNewView);
	virtual CView* ReplaceView(int nRow,int nCol,CRuntimeClass *pViewClass); //<COMBINE CXTSplitterWnd::ReplaceView@int@int@CView*>

	// -------------------------------------------------------------------------------
	// Summary:
	//     Call this member function to modify a splitter window's style.
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator.
	// Parameters:
	//     dwxStyle -  Specifies XT_SPLIT_ styles to be added during style
	//                 modification. See the Remarks section for a list of styles.
	// Remarks:
	//     The splitter window can be modified to use one or more of the
	//     following styles:
	//     * <b>XT_SPLIT_DOTTRACKER</b> The splitter window will use a
	//       dotted tracker rather than the splitter default.
	//     * <b>XT_SPLIT_NOFULLDRAG</b> Disable the "Show window contents
	//       while dragging" option even if it is set in Windows.
	//     * <b>XT_SPLIT_NOBORDER</b> The splitter window will not draw a
	//       border around the pane.
	//     * <b>XT_SPLIT_NOSIZE</b> Do not allow splitter window panes to
	//       be resized.
	// See Also:
	//     GetSplitterStyle
	// -------------------------------------------------------------------------------
	virtual void SetSplitterStyle(DWORD dwxStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set full drag state.
	// Parameters:
	//     bFullDrag -  TRUE to enable full drag.
	// Remarks:
	//     This member function will display the contents for child pane
	//     while the splitter is resized if bFullDrag is set to TRUE.
	// See Also:
	//     SetSplitterStyle
	//-----------------------------------------------------------------------
	void SetFullDrag(BOOL bFullDrag);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the current style for the splitter
	//     window.
	// Returns:
	//     The current style of the splitter window.
	// See Also:
	//     SetSplitterStyle
	//-----------------------------------------------------------------------
	virtual DWORD GetSplitterStyle();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable flat splitters.
	// Parameters:
	//     bFlatSplitter -  TRUE to enable flat splitters.
	// See Also:
	//     SetSplitterStyle
	//-----------------------------------------------------------------------
	virtual void EnableFlatLook(BOOL bFlatSplitter);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the index of the column that is
	//     currently hidden.
	// Returns:
	//     The index of the hidden column or -1 if no columns are hidden.
	// See Also:
	//     GetHiddenRowIndex
	//-----------------------------------------------------------------------
	int GetHiddenColIndex();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the index of the row that is
	//     currently hidden.
	// Returns:
	//     The index of the hidden row; or -1, if no rows are hidden.
	// See Also:
	//     GetHiddenColIndex
	//-----------------------------------------------------------------------
	int GetHiddenRowIndex();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets a pane to be the active one in the frame.
	// Remarks:
	//     This member function is called by the framework to set a pane as
	//     active when the user changes the focus to a pane within the frame
	//     window. You may explicitly call SetActivePane to change the focus
	//     to the specified view.<p/>
	//
	//    Specify pane by providing either row and column, or by providing
	//    <i>pWnd</i>.
	// Parameters:
	//     row  -  If pWnd is NULL, specifies the row in the pane that will
	//             be active.
	//     col  -  If pWnd is NULL, specifies the column in the pane that
	//             will be active.
	//     pWnd -  A pointer to a CWnd object. If NULL, the pane specified
	//             by row and col is set active.  If not NULL, specifies the
	//             pane that is set active.
	// See Also:
	//     CSplitterWnd::GetActivePane, CSplitterWnd::GetPane, CFrameWnd::SetActiveView
	//-----------------------------------------------------------------------
	virtual void SetActivePane(int row, int col, CWnd* pWnd = NULL);

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTSplitterWnd)
	virtual void SetSplitCursor(int ht);
	virtual void OnInvertTracker(const CRect& rect);
	virtual void StartTracking(int ht);
	virtual void StopTracking(BOOL bAccept);
	virtual void DrawTracker(const CRect& rect, CBrush* pBrush);
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTSplitterWnd)
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:

	int     m_nHiddenCol;       // Index of the hidden column.
	int     m_nHiddenRow;       // Index of the hidden row.
	BOOL    m_bFullDrag;        // TRUE if full window dragging is enabled.
	BOOL    m_bForceFullDrag;   // TRUE to use full drag always.
	BOOL    m_bFlatSplit;       // TRUE if the flat splitter style is used.
	DWORD   m_dwxStyle;         // The style of the splitter window. See SetSplitterStyle(...).
	CPoint  m_point;            // Previous cursor position.
	BOOL    m_bClipStyles;      // Clip styles before dragging.

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE DWORD CXTSplitterWnd::GetSplitterStyle() {
	return m_dwxStyle;
}
AFX_INLINE int CXTSplitterWnd::GetHiddenColIndex() {
	return m_nHiddenCol;
}
AFX_INLINE int CXTSplitterWnd::GetHiddenRowIndex() {
	return m_nHiddenRow;
}

// ---------------------------------------------------------------------
// Summary:
//     CXTSplitterWndEx is a CXTSplitterWnd derived class.
//     It can be used with the CXTOutBarCtrl to display a gap
//     between the top of the splitter and the toolbar area.
// ---------------------------------------------------------------------
class _XT_EXT_CLASS CXTSplitterWndEx : public CXTSplitterWnd
{
	DECLARE_DYNAMIC(CXTSplitterWndEx)

public:

	// -----------------------------------------------
	// Summary:
	//     Constructs a CXTSplitterWndEx object
	// -----------------------------------------------
	CXTSplitterWndEx();

	// ------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTSplitterWndEx object, handles cleanup and
	//     deallocation
	// ------------------------------------------------------------------
	virtual ~CXTSplitterWndEx();

public:
	// ------------------------------------------------------------------------------
	// Summary:
	//     Call this member function to show a top border for the splitter
	//     window, similar to Outlook. Enabled by default.
	// Parameters:
	//     bShowTopBorder -  Call this member function to show a top border for the
	//                       splitter window, similar to Outlook. Enabled by default.
	//     cyTopBorderGap -  Call this member function to show a top border for the
	//                       splitter window, similar to Outlook. Enabled by default.
	// ------------------------------------------------------------------------------
	virtual void ShowTopBorder(bool bShowTopBorder=true,int cyTopBorderGap=7);


protected:

	// -------------------------
	// Summary:
	//     Called to retrieve the client area minus the inside 3D borders.
	// Remarks:
	//     This member function is called by the splitter window to retrieve the
	//     size of the client area minus any 3D borders.
	// Parameters:
	//     rect -  Size of the client area minus borders.
	// -------------------------
	virtual void GetInsideRect(CRect& rect) const;

public:

	// -----------------
	// Summary:
	//     Call to redisplay the splitter window after adjusting row or column size.
	// Remarks:
	//     Call this member function to correctly redisplay the splitter window
	//     after you have adjusted row and column sizes with the SetRowInfo and
	//     SetColumnInfo member functions. If you change row and column sizes as
	//     part of the creation process before the splitter window is visible,
	//     it is not necessary to call this member function.<p/>
	//
	//     The framework calls this member function whenever the user resizes
	//     the splitter window or moves a split.
	// Example:
	//     See the example for CSplitterWnd::SetColumnInfo.
	// -----------------
	virtual void RecalcLayout();

	// ----------------------------
	// Summary:
	//     Renders an image of a split window.
	// Parameters:
	//     pDC     -  A pointer to the device context in which to draw. If pDC is NULL,
	//                then CWnd::RedrawWindow is called by the framework and no split
	//                window is drawn.
	//     nType   -  A value of the enum ESplitType. See remarks section for a list
	//                of values.
	//     rectArg -  A reference to a CRect object specifying the size and shape of
	//                the split windows.
	// Remarks:
	//     <i>nType</i> can be one of the following values:<p/>
	//
	//     * <b>splitBox</b>          The splitter drag box.
	//     * <b>splitBar</b>          The bar that appears between the two split windows.
	//     * <b>splitIntersection</b> The intersection of the split windows. This element
	//                                will not be called when running on Windows 95.
	//     * <b>splitBorder</b>       The split window borders.
	//
	//     This member function is called by the framework to draw and specify
	//     the exact characteristics of a splitter window.<p/>
	//
	//     Override OnDrawSplitter for advanced customization of the imagery for the
	//     various graphical components of a splitter window. The default imagery is
	//     similar to the splitter in Microsoft Works for Windows or Microsoft Windows
	//     95, in that the intersections of the splitter bars are blended together.<p/>
	//
	//     For more on dynamic splitter windows, see "Splitter Windows" in the
	//     articleMultiple Document Types, Views, and Frame Windows in Visual C++
	//     Programmer’s Guide, Technical Note 29, and the CSplitterWnd class overview.
	// See Also:
	//      CSplitterWnd::OnInvertTracker
	// ----------------------------
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg);

private:
	static void AFX_CDECL DeferClientPos(AFX_SIZEPARENTPARAMS* lpLayout,
		CWnd* pWnd, int x, int y, int cx, int cy, BOOL bScrollBar);

	static  void AFX_CDECL LayoutRowCol(CSplitterWnd::CRowColInfo* pInfoArray,
		int nMax, int nSize, int nSizeSplitter);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTSplitterWndEx)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	int  m_cyTopBorderGap; // Size in pixels of the top border.
	bool m_bShowTopBorder; // true to draw a top border line.

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTSplitterWndEx::ShowTopBorder(bool bShowTopBorder, int cyTopBorderGap) {
	m_bShowTopBorder = bShowTopBorder; m_cyTopBorderGap = cyTopBorderGap;
}

const DWORD XT_SPLIT_DOTTRACKER    = 0x0001; //<ALIAS CXTSplitterWnd::SetSplitterStyle@DWORD>
const DWORD XT_SPLIT_NOFULLDRAG    = 0x0002; //<ALIAS CXTSplitterWnd::SetSplitterStyle@DWORD>
const DWORD XT_SPLIT_NOBORDER      = 0x0004; //<ALIAS CXTSplitterWnd::SetSplitterStyle@DWORD>
const DWORD XT_SPLIT_NOSIZE        = 0x0008; //<ALIAS CXTSplitterWnd::SetSplitterStyle@DWORD>

#endif // !defined(__XTSPLITTERWND_H__)
