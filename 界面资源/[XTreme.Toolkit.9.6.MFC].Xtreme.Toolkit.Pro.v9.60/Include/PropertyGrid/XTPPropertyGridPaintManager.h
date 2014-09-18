// XTPPropertyGridPaintManager.h interface for the CXTPPropertyGridPaintManager class.
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
#if !defined(_XTPPROPERTYGRIDPAINTMANAGER_H__)
#define _XTPPROPERTYGRIDPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonPro.include/XTPWinThemeWrapper.h"

class CXTPPropertyGrid;
class CXTPPropertyGridInplaceButton;
class CXTPPropertyGridItem;

//-----------------------------------------------------------------------
// Summary:
//     Visual theme enumeration
// Example:
//     <code> m_wndPropertyGrid.SetTheme(xtpGridThemeWhidbey); </code>
// See Also: CXTPPropertyGrid::SetTheme, CXTPPropertyGrid::GetCurrentTheme
//
// <KEYWORDS xtpGridThemeDefault, xtpGridThemeNativeWinXP, xtpGridThemeOffice2003, xtpGridThemeCool, xtpGridThemeSimple, xtpGridThemeDelphi, xtpGridThemeWhidbey>
//-----------------------------------------------------------------------
enum XTPPropertyGridPaintTheme
{
	xtpGridThemeDefault,     // Default theme.
	xtpGridThemeNativeWinXP, // WinXP style theme.
	xtpGridThemeOffice2003,  // Office 2003 style theme.
	xtpGridThemeCool,        // Cool theme.
	xtpGridThemeSimple,      // Visual Basic style theme.
	xtpGridThemeDelphi,      // Delphi style theme.
	xtpGridThemeWhidbey      // Visual Studio 2005 "Whidbey" style theme.
};


//===========================================================================
// Summary:
//     Standalone class used by CXTPPropertyGrid as the paint manager.
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridPaintManager
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridPaintManager object.
	// Parameters:
	//     pGrid - Points to a CXTPPropertyGrid object
	//-----------------------------------------------------------------------
	CXTPPropertyGridPaintManager(CXTPPropertyGrid* pGrid);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridPaintManager object, handles cleanup
	//     and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridPaintManager();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw property grid background.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//-----------------------------------------------------------------------
	virtual void FillPropertyGrid(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw view background.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     bClient - TRUE to draw client area; FALSE to draw frame.
	//-----------------------------------------------------------------------
	virtual void FillPropertyGridView(CDC* pDC, BOOL bClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw single item of the grid.
	// Parameters:
	//     lpDrawItemStruct - A long pointer to a DRAWITEMSTRUCT structure
	//     that contains information about the type of drawing required.
	//-----------------------------------------------------------------------
	virtual void DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct);

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this method and fill in the MEASUREITEMSTRUCT structure
	//     to inform Windows of the list-box dimensions.
	// Parameters:
	//     lpMeasureItemStruct - Specifies a long pointer to a MEASUREITEMSTRUCT
	//     structure.
	//-----------------------------------------------------------------------
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw in-place button of the grid.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pButton - Points to a CXTPPropertyGridInplaceButton object
	//-----------------------------------------------------------------------
	virtual void FillInplaceButton(CDC* pDC, CXTPPropertyGridInplaceButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if a verb is at a specific point.
	// Parameters:
	//     pDC     - Reference to a valid device context
	//     rcVerbs - Bounding rectangle of all verbs.
	//     pt      - Point to test.
	// Returns:
	//     Index of verb within collection if the point was on a verb,
	//     otherwise -1.
	//-----------------------------------------------------------------------
	int HitTestVerbs(CDC* pDC, CRect rcVerbs, CPoint pt);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw expand buttons of the grid.
	// Parameters:
	//     dc - Reference to a valid device context
	//     pItem - Points to a CXTPPropertyGridItem object
	//     rcCaption - Caption button rectangle.
	//-----------------------------------------------------------------------
	virtual void DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to draw the bounding rectangle for a
	//     category caption.
	// Parameters:
	//     pDC - Reference to a valid device context
	//     rc  - Bounding rectangle of Category’s caption.
	//-----------------------------------------------------------------------
	virtual void DrawCategoryCaptionBackground(CDC* pDC, CRect rc);

protected:
	CXTPPropertyGrid* m_pGrid;                      // Parent grid class.
	BOOL m_bThemed;                                 // TRUE to use WinXP themes.
	CFont m_fntVerbUnderline;                       // Font used to display the caption of verbs with underline
	CFont m_fntVerbNormal;                          // Font used to display the caption of verbs without underline


	CXTPWinThemeWrapper m_themeTree;                // Themes for Tree controls.
	CXTPWinThemeWrapper m_themeButton;              // Themes for button controls
	CXTPWinThemeWrapper m_themeCombo;               // Themes for combo button.

	COLORREF m_clrFace;                             // Face color.
	COLORREF m_clrShadow;                           // Shadow color.
	COLORREF m_clrVerbFace;                         // Text color of verbs.
};

namespace XTPPropertyGridPaintThemes
{

	//===========================================================================
	// Summary:
	//     CXTPPropertyGridOffice2003Theme is a CXTPPropertyGridPaintManager derived
	//     class.  This represents an Office 2003 style theme.
	//===========================================================================
	class _XTP_EXT_CLASS CXTPPropertyGridOffice2003Theme : public CXTPPropertyGridPaintManager
	{
	public:

		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPPropertyGridOffice2003Theme object.
		// Parameters:
		//     pGrid - Points to a CXTPPropertyGrid object
		//-----------------------------------------------------------------------
		CXTPPropertyGridOffice2003Theme(CXTPPropertyGrid* pGrid);

		//-------------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of the
		//     property grid.
		//-------------------------------------------------------------------------
		virtual void RefreshMetrics();


	protected:
		BOOL m_bLunaTheme;  // TRUE to use luna colors, FALSE otherwise
	};

	//===========================================================================
	// Summary:
	//     CXTPPropertyGridNativeXPTheme is a CXTPPropertyGridPaintManager derived
	//     class.  This represents a Native Windows XP style theme.
	//===========================================================================
	class _XTP_EXT_CLASS CXTPPropertyGridNativeXPTheme : public CXTPPropertyGridPaintManager
	{
	public:

		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPPropertyGridNativeXPTheme object.
		// Parameters:
		//     pGrid - Points to a CXTPPropertyGrid object
		//-----------------------------------------------------------------------
		CXTPPropertyGridNativeXPTheme(CXTPPropertyGrid* pGrid);

		//-------------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of the
		//     property grid.
		//-------------------------------------------------------------------------
		virtual void RefreshMetrics();
	};


	//===========================================================================
	// Summary:
	//     CXTPPropertyGridCoolTheme is a CXTPPropertyGridPaintManager derived
	//     class.  This represents a Cool style theme.
	//===========================================================================
	class _XTP_EXT_CLASS CXTPPropertyGridCoolTheme : public CXTPPropertyGridPaintManager
	{
	public:
		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPPropertyGridCoolTheme object.
		// Parameters:
		//     pGrid - Points to a CXTPPropertyGrid object
		//-----------------------------------------------------------------------
		CXTPPropertyGridCoolTheme(CXTPPropertyGrid* pGrid);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw single item of the grid.
		// Parameters:
		//     lpDrawItemStruct - A long pointer to a DRAWITEMSTRUCT structure
		//     that contains information about the type of drawing required.
		//-----------------------------------------------------------------------
		virtual void DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct);

		//-------------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of the
		//     property grid.
		//-------------------------------------------------------------------------
		void RefreshMetrics();
	};


	//===========================================================================
	// Summary:
	//     CXTPPropertyGridSimpleTheme is a CXTPPropertyGridPaintManager derived
	//     class.  This represents a Simple style theme.
	//===========================================================================
	class _XTP_EXT_CLASS CXTPPropertyGridSimpleTheme : public CXTPPropertyGridPaintManager
	{
	public:
		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPPropertyGridSimpleTheme object.
		// Parameters:
		//     pGrid - Points to a CXTPPropertyGrid object
		//-----------------------------------------------------------------------
		CXTPPropertyGridSimpleTheme(CXTPPropertyGrid* pGrid);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw single item of the grid.
		// Parameters:
		//     lpDrawItemStruct - A long pointer to a DRAWITEMSTRUCT structure
		//     that contains information about the type of drawing required.
		//-----------------------------------------------------------------------
		virtual void DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct);

		//-------------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of the
		//     property grid.
		//-------------------------------------------------------------------------
		void RefreshMetrics();
	};


	//===========================================================================
	// Summary:
	//     CXTPPropertyGridDelphiTheme is a CXTPPropertyGridPaintManager derived
	//     class.  This represents a Delphi style theme.
	//===========================================================================
	class _XTP_EXT_CLASS CXTPPropertyGridDelphiTheme : public CXTPPropertyGridPaintManager
	{
	public:
		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPPropertyGridDelphiTheme object.
		// Parameters:
		//     pGrid - Points to a CXTPPropertyGrid object
		//-----------------------------------------------------------------------
		CXTPPropertyGridDelphiTheme(CXTPPropertyGrid* pGrid);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw single item of the grid.
		// Parameters:
		//     lpDrawItemStruct - A long pointer to a DRAWITEMSTRUCT structure
		//     that contains information about the type of drawing required.
		//-----------------------------------------------------------------------
		virtual void DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct);

		//-------------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of the
		//     property grid.
		//-------------------------------------------------------------------------
		virtual void RefreshMetrics();

	};


	//===========================================================================
	// Summary:
	//     CXTPPropertyGridWhidbeyTheme is a CXTPPropertyGridPaintManager derived
	//     class.  This represents a Visual Studio 2005 "Whidbey" style theme.
	//===========================================================================
	class _XTP_EXT_CLASS CXTPPropertyGridWhidbeyTheme : public CXTPPropertyGridPaintManager
	{
	public:

		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPPropertyGridWhidbeyTheme object.
		// Parameters:
		//     pGrid - Points to a CXTPPropertyGrid object
		//-----------------------------------------------------------------------
		CXTPPropertyGridWhidbeyTheme(CXTPPropertyGrid* pGrid);

	protected:

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to draw the bounding rectangle for a
		//     category caption.
		// Parameters:
		//     pDC - Reference to a valid device context
		//     rc  - Bounding rectangle of Category’s caption.
		//-----------------------------------------------------------------------
		void DrawCategoryCaptionBackground(CDC* pDC, CRect rc);

		//-------------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of the
		//     property grid.
		//-------------------------------------------------------------------------
		void RefreshMetrics();

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw expand buttons of the grid.
		// Parameters:
		//     dc        - Reference to a valid device context
		//     pItem     - Points to a CXTPPropertyGridItem object
		//     rcCaption - Caption button rectangle.
		//-----------------------------------------------------------------------
		void DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption);
	};

}
using namespace XTPPropertyGridPaintThemes;


#endif // !defined(_XTPPROPERTYGRIDPAINTMANAGER_H__)
