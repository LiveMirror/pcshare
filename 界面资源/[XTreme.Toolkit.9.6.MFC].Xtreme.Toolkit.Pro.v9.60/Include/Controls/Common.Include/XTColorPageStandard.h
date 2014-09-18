// XTColorPageStandard.h : header file
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
#if !defined(__XTCOLORSTANDARD_H__)
#define __XTCOLORSTANDARD_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTColorHex is a CStatic derived class. It displays a color selection
//     grid used by CXTColorPageStandard.
//===========================================================================
class _XT_EXT_CLASS CXTColorHex : public CStatic
{
	// ----------------------------------------------------------------------
	// Summary:
	//     HEXCOLOR_CELL structure is used by the CXTColorHex class
	//     to maintain information about a particular color cell.
	// See Also:
	//     CXTColorHex
	// ----------------------------------------------------------------------
	struct HEXCOLOR_CELL
	{
		int      direction[4]; // Array that indicates which cell index is to the left, top, right, and bottom of the color cell.
		BOOL     bSmall;       // TRUE if the color cell is a standard selection rectangle.
		CPoint*  pPoint;       // Represents the current cursor position.
		COLORREF clr;          // An RGB value.
	};

	// ----------------------------------------------------------------------
	// Summary:
	//     List for maintaining HEXCOLOR_CELL structures.
	// Remarks:
	//     CList definition used by the CXTColorHex control to maintain
	//     a list of HEXCOLOR_CELL structures representing each color cell in
	//     the hex color control.
	// See Also:
	//     CXTColorHex, HEXCOLOR_CELL
	// ----------------------------------------------------------------------
	typedef CList<HEXCOLOR_CELL*,HEXCOLOR_CELL*> CHexColorCellList;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTColorHex object
	//-----------------------------------------------------------------------
	CXTColorHex();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTColorHex object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTColorHex();

public:

	// ----------------------------------------------------------------------
	// Summary:
	//     Retrieves a COLORREF value from a specified point.
	// Parameters:
	//     point -  XY location of the color to retrieve RGB information for.
	// Remarks:
	//     Call this member function to retrieve RGB information for the
	//     color found at the location specified by point. Returns a COLORREF
	//     value.
	// ----------------------------------------------------------------------
	COLORREF ColorFromPoint(CPoint point);

	// ----------------------------------------------------------------------
	// Summary:
	//     Retrieves HEXCOLOR_CELL information for a color cell.
	// Parameters:
	//     clr -  An RGB value that represents the color.
	// Remarks:
	//     Call this member function to get a pointer to the HEXCOLOR_CELL
	//     structure that is represented by 'clr'.
	// Returns:
	//     A pointer to an HEXCOLOR_CELL struct.
	// ----------------------------------------------------------------------
	HEXCOLOR_CELL* GetColorCell(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to select the color specified by 'iIndex'.
	// Parameters:
	//     iIndex - Index into m_arCells list.
	//    clr - An RGB value that represents the color.
	//-----------------------------------------------------------------------
	void SetSelectedColor(int iIndex);
	void SetSelectedColor(COLORREF clr); //<combine CXTColorHex::SetSelectedColor@int>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the currently selected color.
	// Returns:
	//     An RGB color value that represents the selected color.
	//-----------------------------------------------------------------------
	COLORREF GetSelectedColor() const;

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the class to draw a single color cell.
	// Parameters:
	//     pDC   - A CDC pointer that represents the current device context.
	//     point - XY position of the starting point for the first pixel drawn.
	//     clr   - An RGB value that represents the color of the cell to draw.
	//     l     - Index of the cell to be selected when VK_LEFT is pressed.
	//     u     - Index of the cell to be selected when VK_UP is pressed.
	//     r     - Index of the cell to be selected when VK_RIGHT is pressed.
	//     d     - Index of the cell to be selected when VK_DOWN is pressed.
	//-----------------------------------------------------------------------
	void DrawCell(CDC* pDC, CPoint point, COLORREF clr, int l, int u, int r, int d);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the class to draw a single large
	//     color cell.
	// Parameters:
	//     pDC   - A CDC pointer that represents the current device context.
	//     point - XY position of the starting point for the first pixel drawn.
	//     clr   - An RGB value that represents the color of the cell to draw.
	//     l     - Index of the cell to be selected when VK_LEFT is pressed.
	//     u     - Index of the cell to be selected when VK_UP is pressed.
	//     r     - Index of the cell to be selected when VK_RIGHT is pressed.
	//     d     - Index of the cell to be selected when VK_DOWN is pressed.
	//-----------------------------------------------------------------------
	void DrawLargeCell(CDC* pDC, CPoint point, COLORREF clr, int l, int u, int r, int d);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the selection window.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device context.
	//-----------------------------------------------------------------------
	void DrawColorSelector(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to select a color cell if any
	//     are selected.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device context.
	//-----------------------------------------------------------------------
	void DrawSelectCell(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to select a large color cell if any
	//     are selected.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device context.
	//-----------------------------------------------------------------------
	void DrawLargeSelectCell(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function updates the color selection based on the XY
	//     coordinates specified by 'point'.
	// Parameters:
	//     point - Current location of the color to select.
	//-----------------------------------------------------------------------
	void UpdateSelection(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to select a cell when a new
	//     selection is made.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device context.
	//-----------------------------------------------------------------------
	void SelectColorCell(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to determine if the specified
	//      color is defined.
	// Parameters:
	//     cr - A COLORREF value to check.
	// Returns:
	//     Returns true if the color specified by cr is defined in the
	//      color array otherwise returns false.
	//-----------------------------------------------------------------------
	virtual bool IsValidColor(COLORREF cr) const;

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTColorHex)
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTColorHex)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnInitControl(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:

	CDC                 m_dcPicker;     // Background device context.
	bool                m_bInitControl; // true for initialization.
	BOOL                m_bLBtnDown;    // TRUE when the left mouse button is pressed.
	BOOL                m_bSmallCell;   // TRUE when a small color cell is selected.
	CPoint              m_ptCurrent;    // Holds the last known selection point.
	COLORREF            m_clrColor;     // A COLORREF value that contains the RGB information for the current color.
	CHexColorCellList   m_arCells;      // Array of HEXCOLOR_CELL structs that represent displayed color cells.

	static const COLORREF clrSelArray[];// Hex colors.

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE COLORREF CXTColorHex::GetSelectedColor() const {
	return m_clrColor;
}

//===========================================================================
// Summary:
//     CXTColorPageStandard is derived from CXTThemePropertyPage, it is
//     used to create a CXTColorPageStandard dialog.
//===========================================================================
class _XT_EXT_CLASS CXTColorPageStandard : public CXTThemePropertyPage
{
	DECLARE_DYNCREATE(CXTColorPageStandard)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTColorPageStandard object
	// Parameters:
	//     pParentSheet - Points to the parent property sheet.
	//-----------------------------------------------------------------------
	CXTColorPageStandard(CXTColorDialog* pParentSheet=NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTColorPageStandard object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTColorPageStandard();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to set the selected color for
	//     the page.
	// Parameters:
	//     clr - An RGB value that represents the color.
	//-----------------------------------------------------------------------
	void SetColor(COLORREF clr);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This functions draws the themed background and the transparent
	//     text on the themed property page.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device
	//           context.
	//-----------------------------------------------------------------------
	virtual void OnDrawPage(CDC* pDC);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTColorPageStandard)
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTColorPageStandard)
	afx_msg LRESULT OnUpdateColor(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	//{{AFX_DATA(CXTColorPageStandard)
	enum { IDD = XT_IDD_COLORSTANDARD };
	//}}AFX_DATA
//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTColorHex m_colorHex;                // ColorHex control.


	friend class CXTColorHex;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTColorPageStandard::SetColor(COLORREF clr) {
	m_colorHex.SetSelectedColor(clr);
}

#endif // !defined(__XTCOLORSTANDARD_H__)
