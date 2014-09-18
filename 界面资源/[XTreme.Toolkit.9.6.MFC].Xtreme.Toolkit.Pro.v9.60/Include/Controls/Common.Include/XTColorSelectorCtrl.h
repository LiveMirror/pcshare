// XTColorSelectorCtrl.h : interface for the CXTColorSelectorCtrl class.
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
#if !defined(__XTCOLORCTRL_H__)
#define __XTCOLORCTRL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Theme/XTThemeManager.h"

DECLARE_THEME_FACTORY(CXTColorSelectorCtrlTheme)

//===========================================================================
// Summary:
//    CXTColorSelectorCtrlTheme is derived from CXTThemeManagerStyle to
//    handle theme specific visualizations for the CXTColorSelectorCtrl
//    class.
//===========================================================================
class _XT_EXT_CLASS CXTColorSelectorCtrlTheme : public CXTThemeManagerStyle
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//    Constructs a CXTColorSelectorCtrlTheme object.
	//-----------------------------------------------------------------------
	CXTColorSelectorCtrlTheme();
};

class CXTColorDialog;

//===========================================================================
// Summary:
//     CXTColorSelectorCtrl is a CWnd derived class. It is used to create
//     a CXTColorSelectorCtrl control that will allow a user to select colors.
//===========================================================================
class _XT_EXT_CLASS CXTColorSelectorCtrl : public CWnd, public CXTThemeManagerStyleHost
{
	DECLARE_THEME_HOST(CXTColorSelectorCtrl, CXTColorSelectorCtrlTheme)

public:
	struct PICK_BUTTON;

protected:
	// ----------------------------------------------------------------------
	// Summary:
	//     COLOR_CELL structure is used by the CXTColorSelectorCtrl
	//     class to maintain information about a particular color cell.
	// See Also:
	//     CXTColorSelectorCtrl
	// ----------------------------------------------------------------------
	struct COLOR_CELL
	{
		UINT     nID;         // Command ID of the color cell.
		UINT     nIndex;      // Index of the color cell.
		bool     bChecked;    // true if the cell is checked.
		CRect    rect;        // Size of the color cell.
		DWORD    dwStyle;     // Windows style for the cell.
		TCHAR    szText[256]; // Tooltip text displayed for the color cell.
		COLORREF clr;         // An RGB value that represents the color of the cell.
	};

	// ----------------------------------------------------------------------
	// Summary:
	//     List for maintaining COLOR_CELL structures.
	// Remarks:
	//     CList definition used by the CXTColorSelectorCtrl class to maintain
	//     a list of COLOR_CELL structures representing each color item in
	//     the color selector control.
	// See Also:
	//     CXTColorSelectorCtrl, COLOR_CELL
	// ----------------------------------------------------------------------
	typedef CList<COLOR_CELL*,COLOR_CELL*> CColorCellList;

	// ---------------------------------------------------------------
	// Summary:
	//     Array of listener windows to send notifications to.
	// Remarks:
	//     CArray definition used by the CXTColorSelectorCtrl class to
	//     maintain a list of window handles that receive notification
	//     messages that are sent when an event occurs in the control.
	// See Also:
	//     CXTColorSelectorCtrl
	// ---------------------------------------------------------------
	typedef CArray<HWND, HWND> CListenerArray;


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTColorSelectorCtrl object
	//-----------------------------------------------------------------------
	CXTColorSelectorCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTColorSelectorCtrl object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTColorSelectorCtrl();
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to initialize the user defined color
	//     array for the color selector.
	// Parameters:
	//     arUserDefColors - Reference to a CUIntArray object that
	//                       represents an array of COLORREF values.
	//-----------------------------------------------------------------------
	static void SetUserColors(const CUIntArray& arUserDefColors);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return a reference to the user
	//     defined color array.
	// Returns:
	//     A reference to a CUIintArray object that represents an array
	//     of user defined COLORREF values.
	//-----------------------------------------------------------------------
	static CUIntArray& GetUserColors();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will return the index of the currently selected
	//     color and will initialize 'pColorCell' struct.
	// Parameters:
	//     pColorCell - Receives a pointer to the currently selected button.
	// Returns:
	//     The zero (0) based index of the currently selected button.
	//-----------------------------------------------------------------------
	int GetCurSel(COLOR_CELL* pColorCell);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will select a button based upon its index.
	// Parameters:
	//     nIndex - An integer value that represents the zero (0) based
	//     index of the button to be selected.
	//-----------------------------------------------------------------------
	void SetCurSel(int nIndex);

	// ----------------------------------------------------------------------------
	// Summary:
	//     This member function handles the creation of the color popup
	//     window.
	// Parameters:
	//     rect -        A reference to a <i>CRect</i> object that represents the
	//                   size of the color popup window.
	//     pParentWnd -  Points to the parent window for the color popup.
	//     dwPopup -     Style for the popup window. See the Remarks section for a
	//                   complete list of available styles.
	//     clrColor -    An <i>RGB</i> value that represents the currently selected
	//                   color for the popup window.
	//     clrDefault -  Specifies the default color for the color popup. If the
	//                   current style includes CPS_NOFILL this parameter is
	//                   ignored.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:
	//     * <b>CPS_XT_NOFILL</b> The color picker will display a No Fill
	//           button rather than the default Automatic Color button.
	//     * <b>CPS_XT_EXTENDED</b> The color picker will display 40
	//           extended colors rather than the default 16 colors.
	//     * <b>CPS_XT_MORECOLORS</b> The color picker will display a
	//           More Colors button which will display a CXTColorDialog.
	//     * <b>CPS_XT_USERCOLORS</b> The color picker will display an
	//           MRU (most recently used) color list for the control.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	// See Also:
	//     CXTColorPicker::ModifyCPStyle, CXTColorPopup::Create
	// ----------------------------------------------------------------------------
	BOOL Create(CRect rect,CWnd* pParentWnd,DWORD dwPopup,COLORREF clrColor,COLORREF clrDefault=CLR_DEFAULT);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function adds a window to send color picker notifications to.
	// Parameters:
	//     hwndListener - A handle to the listener window.  Messages will
	//                    be sent to it.
	//-----------------------------------------------------------------------
	void AddListener(HWND hwndListener);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function removes a window from notification list.
	// Parameters:
	//     hwndListener - A handle to the listener window to remove.
	//-----------------------------------------------------------------------
	void RemoveListener(HWND hwndListener);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to select a color cell.
	// Parameters:
	//     pColorCell - Points to an COLOR_CELL object.
	//-----------------------------------------------------------------------
	void SelectColorCell(COLOR_CELL* pColorCell);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to select a color cell.
	// Parameters:
	//     clr - Color of selected cell.
	//-----------------------------------------------------------------------
	void SelectColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to return an COLOR_CELL struct
	//     from the color cell array.
	// Parameters:
	//     iIndex - Index into the color cell array.
	// Returns:
	//     An COLOR_CELL object.
	//-----------------------------------------------------------------------
	COLOR_CELL* GetCellFromIndex(int iIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to set the size of the borders
	//     for the control.
	// Parameters:
	//     cxLeft   - Specifies the left position.
	//     cyTop    - Specifies the top.
	//     cxRight  - Specifies the right.
	//     cyBottom - Specifies the bottom.
	//-----------------------------------------------------------------------
	void SetBorders(int cxLeft = 0, int cyTop = 0, int cxRight = 0, int cyBottom = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the color selection dialog
	//     is visible.
	// Returns:
	//     True if the color selection dialog is open, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsColorDlgVisible();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the color selector.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device context.
	//-----------------------------------------------------------------------
	void DrawColorSelector(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the cell specified by
	//     'pColorCell'.
	// Parameters:
	//     pColorCell - An COLOR_CELL object.
	//     pDC        - A CDC pointer that represents the current device
	//                  context.
	//     bHilite    - TRUE to highlight the color cell.
	//     bPressed   - TRUE to press the color cell.
	//-----------------------------------------------------------------------
	void DrawColorCell(COLOR_CELL* pColorCell, CDC* pDC, BOOL bHilite, BOOL bPressed);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will finish the selection process for the color
	//     box or button in the color popup window.
	// Parameters:
	//     nCurSel - Current index of the selected color box or button
	//               in the color popup window.
	//-----------------------------------------------------------------------
	virtual void EndSelection(int nCurSel);

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function is called to determine if the specified color
	//     is already in the ColorDialog's MRU list or in the standard colors
	//     list.
	// Parameters:
	//     crFind -  A const COLORREF value.
	// Return -  TRUE if the color is found otherwise returns FALSE.
	// ----------------------------------------------------------------------
	BOOL LookUpColor(const COLORREF crFind);

	// ---------------------------------------------------------------------------
	// Summary:
	//     This member function is called to finish the selection process for
	//     the color box or button in the color popup window.
	// Parameters:
	//     nCurSel -      Current index of the selected color box or button in the
	//                    color popup window.
	//     callerParam -  User defined value to be passed to listeners.
	// Remarks:
	//     Override this method to pass user defined data to the listener
	//     windows that receive notification messages.
	// ---------------------------------------------------------------------------
	virtual void OnEndSelection(int nCurSel, LPARAM callerParam);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTColorSelectorCtrl)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTColorSelectorCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseLeave();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:

	int               m_nRows;           // Number of rows in the color popup window.
	int               m_nCols;           // Number of columns in the color popup window.
	int               m_nLastIndex;      // Index of the last button on the popup window.
	int               m_nBtnCount;       // Number of buttons in this window.
	int               m_nCurSel;         // Currently selected index.
	int               m_nPressed;        // Pressed button.
	CWnd*             m_pParentWnd;      // Points to the parent window for the popup window.
	CSize             m_sizeButton;      // cx and cy size for a color picker button.
	CRect             m_rcWnd;           // Rect for the popup window.
	CRect             m_rcBorders;       // Control borders
	DWORD             m_dwPopup;         // Color popup window style.
	CPoint            m_point;           // Last known cursor position.
	COLORREF          m_clrColor;        // An RGB value that represents the currently selected color.
	COLORREF          m_clrDefault;      // An RGB value that represents the default color for the popup window.
	CToolTipCtrl      m_tooltip;         // Tooltip control.
	CListenerArray    m_listeners;       // Array of listener windows to be sent notifications.
	CColorCellList m_arCells;         // Array of color items.
	static CUIntArray m_arUserDefColors; // Array of user defined colors.
	CXTColorDialog*   m_pColorDlg;       // Points to the color selection dialog when visible.


	friend class CXTPopupColorTearOff;

};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTColorSelectorCtrl::SetBorders(int cxLeft, int cyTop, int cxRight, int cyBottom) {
	m_rcBorders.left = cxLeft; m_rcBorders.top = cyTop; m_rcBorders.right = cxRight; m_rcBorders.bottom = cyBottom;
}
AFX_INLINE BOOL CXTColorSelectorCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

const DWORD CPS_XT_NOFILL          = 0x0001;  //<ALIAS CXTColorSelectorCtrl::Create@CRect@CWnd*@DWORD@COLORREF@COLORREF>
const DWORD CPS_XT_EXTENDED        = 0x0002;  //<ALIAS CXTColorSelectorCtrl::Create@CRect@CWnd*@DWORD@COLORREF@COLORREF>
const DWORD CPS_XT_MORECOLORS      = 0x0004;  //<ALIAS CXTColorSelectorCtrl::Create@CRect@CWnd*@DWORD@COLORREF@COLORREF>
//{{AFX_CODEJOCK_PRIVATE
const DWORD CPS_XT_PICKBOX         = 0x0008;  // Used internally
const DWORD CPS_XT_DEFAULT         = 0x0010;  // Used internally
//}}AFX_CODEJOCK_PRIVATE
const DWORD CPS_XT_USERCOLORS      = 0x0020;  //<ALIAS CXTColorSelectorCtrl::Create@CRect@CWnd*@DWORD@COLORREF@COLORREF>

#endif //__XTCOLORCTRL_H__
