// XTPReportPaintManager.h: interface for the CXTPReportPaintManager class.
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
#if !defined(__XTPREPORTPAINTMANAGER_H__)
#define __XTPREPORTPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTPReportDefines.h"
#include "CommonPro.Include\XTPDrawHelpers.h"

class CXTPReportControl;
class CXTPReportRow;
class CXTPReportHeader;
class CXTPReportColumn;
class CXTPReportGroupRow;
class CXTPReportInplaceButton;
class CXTPReportHyperlink;

struct XTP_REPORTRECORDITEM_DRAWARGS;
struct XTP_REPORTRECORDITEM_METRICS;

//===========================================================================
// Summary:
//     Represents predefined grid line styles:
// Example:
//     <code>m_wndReport.SetGridStyle(FALSE, xtpGridLargeDots);</code>
// See Also: CXTPReportControl::SetGridStyle
//
// <KEYWORDS xtpGridNoLines, xtpGridSmallDots, xtpGridLargeDots, xtpGridDashes, xtpGridSolid>
//===========================================================================
enum XTPReportGridStyle
{
	xtpGridNoLines,     // empty line
	xtpGridSmallDots,   // line is drawn with small dots
	xtpGridLargeDots,   // line is drawn with large dots
	xtpGridDashes,      // line is drawn with dashes
	xtpGridSolid        // draws solid line
};

//===========================================================================
// Summary:
//     Represents predefined column styles.
// Example:
//     <code>m_wndReport.GetPaintManager()->m_columnStyle = xtpColumnFlat;</code>
// See Also: CXTPReportPaintManager::m_columnStyle, CXTPReportPaintManager
//
// <KEYWORDS xtpColumnShaded, xtpColumnFlat>
//===========================================================================
enum XTPReportColumnStyle
{
	xtpColumnShaded,    // Columns are grey shaded.
	xtpColumnFlat       // Flat style for drawing columns.
};


//-----------------------------------------------------------------------
// Summary:
//     Represent the style of lines used to draw a hierarchical tree structure.
// Example:
//     <code>m_wndReport.GetPaintManager()->m_treeStructureStyle = xtpTreeStructureSolid;</code>
// See Also: CXTPReportPaintManager::m_treeStructureStyle, CXTPReportPaintManager
//
// <KEYWORDS xtpTreeStructureNone, xtpTreeStructureSolid>
//-----------------------------------------------------------------------
enum XTPReportTreeStructureStyle
{
	xtpTreeStructureNone,           // No lines will be drawn from the parent node to each child node.
	xtpTreeStructureSolid           // Lines will be drawn from the parent node to each child node at the next level.
};

//===========================================================================
// Summary:
//    Utility class, handling most of the drawing activities. It stores
//    all settings, needed by a control to perform drawing operations:
//    fonts, colors, styles for all other classes of control.
//    It also implements all functions for drawing typical graphical
//    primitives, functions that directly work with device context.
// Remarks:
//    Could be overridden to provide another look and feel for the control.
//    Thus you have an easy way to change "skin" of your control; just provide
//    your own implementation of CXTPReportPaintManager and there's no need to touch
//    functionality all others classes of control
//
//    You create ReportPaintManager by calling its constructor,
//    further you can call getter and setter functions to change
//    every setting as you wish.
// See Also: CXTPReportControl overview, CXTPReportRow, CXTPReportGroupRow,
//           CXTPReportColumn, CXTPReportHeader
//===========================================================================
class _XTP_EXT_CLASS CXTPReportPaintManager : public CCmdTarget
{
	friend class CXTPReportControl;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	// See Also: RefreshMetrics
	//-----------------------------------------------------------------------
	CXTPReportPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager destructor, handles member items deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPReportPaintManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Initializes all drawings defaults (fonts, colors, etc.). Most
	//     of them are system defaults.
	// See Also: CXTPReportPaintManager
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns default row height.
	// Parameters:
	//     pDC   - Pointer to the used Device Context.
	//     pRow  - Pointer to CXTPReportRow object.
	// Returns:
	//     The height of the default rectangle where row's items will draw.
	// Example:
	//     <code>int nRowHeight = GetRowHeight(pDC, pRow)</code>
	//-----------------------------------------------------------------------
	virtual int GetRowHeight(CDC* pDC, CXTPReportRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the default column header height.
	// Returns:
	//     The current height of the rectangle where column's headers draw.
	//-----------------------------------------------------------------------
	virtual int GetHeaderHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns control background color.
	// Parameters:
	//     pControl - Pointer to the parent report control object.
	// Returns:
	//     Color that is used for filling client rect.
	//-----------------------------------------------------------------------
	virtual COLORREF GetControlBackColor(CXTPReportControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills the provided row with its background color.
	// Parameters:
	//     pDC     - Pointer to the used Device Context.
	//     pRow    - Pointer to the Row.
	//     rcRow   - Row area rectangle coordinates.
	// Example:
	// <code>
	// // paint row background
	// pPaintManager->FillRow(pDC, this, rcRow);
	// </code>
	//-----------------------------------------------------------------------
	virtual void FillRow(CDC* pDC, CXTPReportRow* pRow, CRect rcRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws control's grid.
	// Parameters:
	//     pDC       - Pointer to the used Device Context.
	//     bVertical - boolean value determines orientation of grid
	//     rc        - Row area rectangle coordinates.
	// Example:
	// <code>
	// // draw vertical grid in Row
	// CRect rcGridItem(rcRowRect);
	// pPaintManager->DrawGrid(pDC, TRUE, rcGridItem);
	//
	// // draw horizontal grid in Row
	// CPaintDC pDC(this);
	// CRect rcGridItem(rcRowRect);
	// pPaintManager->DrawGrid(pDC, FALSE, rcGridItem);
	// </code>
	//-----------------------------------------------------------------------
	virtual void DrawGrid(CDC* pDC, BOOL bVertical, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws group row.
	// Parameters:
	//     pDC   - Pointer to the used Device Context.
	//     pRow  - Pointer to the Group Row.
	//     rcRow - Group Row area rectangle coordinates.
	// Example:
	// <code>
	// CXTPReportPaintManager* pPaintManager = m_pControl->GetPaintManager();
	// pDC->SetBkMode(TRANSPARENT);
	// pPaintManager->DrawGroupRow(pDC, this, rcRow);
	// </code>
	//-----------------------------------------------------------------------
	virtual void DrawGroupRow(CDC* pDC, CXTPReportGroupRow* pRow, CRect rcRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills indent area by its background.
	// Parameters:
	//     pDC   - Pointer to the used Device Context.
	//     rcRow - Row area rectangle coordinates.
	// Example:
	// <code>
	// CXTPReportPaintManager* pPaintManager = m_pControl->GetPaintManager();
	// CRect rcRow(rcClient);
	// // Get default row height
	// int nRowHeight = m_pControl->GetRowHeight(FALSE)
	// rcRow.bottom = rcRow.top + nRowHeight;
	// pPaintManager->FillIndent(pDC, rcRow);
	// </code>
	//-----------------------------------------------------------------------
	virtual void FillIndent(CDC* pDC, CRect rcRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills item shade by its background if necessary.
	// Parameters:
	//     pDC    - Pointer to the used Device Context.
	//     rcItem - Item area rectangle coordinates.
	// Example:
	// <code>
	// CXTPReportPaintManager* pPaintManager = m_pControl->GetPaintManager();
	// CRect rcItem(m_rcRow);
	// pPaintManager->FillItemShade(pDC, rcItem);
	// </code>
	//-----------------------------------------------------------------------
	virtual void FillItemShade(CDC* pDC, CRect rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills column header control area by its background.
	// Parameters:
	//     pDC      -  Pointer to the used Device Context.
	//     rcHeader -  Header area rectangle coordinates.
	// Example:
	// <code>
	// CXTPReportPaintManager* pPaintManager = m_pControl->GetPaintManager();
	// CRect rcHeader(rcClient);
	// int nHeaderHeight = m_pControl->GetHeaderHeight();
	// rcHeader.bottom = rcHeader.top + nHeaderHeight;
	// pPaintManager->FillHeaderControl(pDC, rcHeader);
	// </code>
	//-----------------------------------------------------------------------
	virtual void FillHeaderControl(CDC* pDC, CRect rcHeader);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws column header with all related attributes (sort order, icon, etc).
	// Parameters:
	//     pDC           - Pointer to the used Device Context.
	//     pColumn       - Column header area rectangle coordinates.
	//     pHeader       - Pointer to report header.
	//     rcColumn      - Column area rectangle coordinates.
	//     bDrawExternal - whether draw Column on Header area, default is FALSE.
	// Example:
	// <code>
	// CXTPReportPaintManager* pPaintManager = m_pControl->GetPaintManager();
	// CXTPReportColumn* pColumn = CXTPReportColumn* pColumn = m_pColumns->GetAt(1);
	// CRect rcHeader(rcClient);
	// int nHeaderHeight = m_pControl->GetHeaderHeight();
	// rcHeader.bottom = rcHeader.top + nHeaderHeight;
	// GetPaintManager()->DrawColumn(pDC, pColumn, this, rcHeader);
	// </code>
	//-----------------------------------------------------------------------
	virtual void DrawColumn(CDC* pDC, CXTPReportColumn* pColumn, CXTPReportHeader* pHeader, CRect rcColumn, BOOL bDrawExternal = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws rows that were focused.
	// Parameters:
	//     pDC     - Pointer to the used Device Context.
	//     pHeader - Pointer to report header.
	//     rcRow   - Area rectangle coordinates.
	// Example:
	// <code>
	// CRect rcClient(GetParent()->GetClientRect());
	// CXTPReportHeader* pHeader = m_pColumns->GetReportHeader();
	// CXTPReportPaintManager* pPaintManager = m_pControl->GetPaintManager();
	// GetPaintManager()->DrawResizingRect(pDC, pHeader, rcClient);
	// </code>
	//-----------------------------------------------------------------------
	virtual void DrawFocusedRow(CDC* pDC, CRect rcRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws Group By box.
	// Parameters:
	//     pDC       - Pointer to the used Device Context.
	//     rcGroupBy - Reference to Group By box area rectangle coordinates.
	// Example:
	// <code>
	// CXTPReportPaintManager* pPaintManager = m_pControl->GetPaintManager();
	// pPaintManager->FillGroupByControl(pDC, rcGroupBy);
	// </code>
	//-----------------------------------------------------------------------
	virtual void FillGroupByControl(CDC* pDC, CRect& rcGroupBy);


	//-----------------------------------------------------------------------
	// Summary:
	//     Returns point to additional image list with Glyphs
	// Remarks:
	//     Additional image list usually used for bitmaps of collapsed icons, etc
	//     You call this member function to get a pointer to list and manipulate
	//     its content as you wish
	// Returns:
	//     Pointer to the instantiated object of CImageList class
	//-----------------------------------------------------------------------
	CImageList* GetGlyphsImageList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws Horizontal line.
	// Parameters:
	//     pDC - Pointer to the used Device Context.
	//     x   - Horizontal coordinate of the beginning of line.
	//     y   - Vertical coordinate of the beginning of line.
	//     cx  - Length of line.
	//     clr - Color of line.
	// Remarks:
	//     This member function is called in the control everywhere we need to
	//     draw a simple horizontal line
	//-----------------------------------------------------------------------
	void DrawHorizontalLine(CDC* pDC, int x, int y, int cx, COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws vertical line.
	// Parameters:
	//     pDC - Pointer to the used Device Context.
	//     x   - Horizontal coordinate of the beginning of line.
	//     y   - Vertical coordinate of the beginning of line.
	//     cy  - Length of line.
	//     clr - Color of line.
	// Remarks:
	//     This member function is called in the control everywhere we need to
	//     draw a simple vertical line
	//-----------------------------------------------------------------------
	void DrawVerticalLine(CDC* pDC, int x, int y, int cy, COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws triangle that shows the sort order of column.
	// Parameters:
	//     pDC        - Pointer to the used Device Context.
	//     rcTriangle - Triangle area rectangle coordinates.
	//     bToDown    - If TRUE the top of triangle turn downward, otherwise upward.
	// Remarks:
	//     You use this function to draw a triangle image on the header of the column
	//     that is sorted. Triangle represents the direction of sorting: ascending or descending.
	//     Sizes of the triangle depend on the rectangle area that is provided to draw
	// Example:
	// <code>
	// // draw ascendant triangle
	// CRect rcTriangle;
	// rcTriangle.CopyRect(rcHeader);
	// rcTriangle.DeflateRect(40, 5,5,5);
	// DrawTriangle(pDC, rcTriangle, TRUE);
	//
	// // draw descendant triangle
	// CRect rcTriangle;
	// rcTriangle.CopyRect(rcHeader);
	// rcTriangle.DeflateRect(40, 5,5,5);
	// DrawTriangle(pDC, rcTriangle, FALSE);
	// </code>
	//-----------------------------------------------------------------------
	void DrawTriangle(CDC* pDC, CRect rcTriangle, BOOL bToDown);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a column connector for Group By box.
	// Parameters:
	//     pDC    - Pointer to the used Device Context.
	//     ptFrom - Source point of the connector.
	//     ptTo   - Target point of the connector.
	// Remarks:
	//     You use this function to draw a line that represents a
	//     chain of columns in GroupBy box. It connects columns
	//     in GroupBy box altogether
	// Example:
	//     <code>DrawConnector(pDC, CPoint(rcItem.right - 5, rcItem.bottom), CPoint(x, rcItem.bottom + 4));</code>
	//-----------------------------------------------------------------------
	void DrawConnector(CDC* pDC, CPoint ptFrom, CPoint ptTo);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws collapsed bitmap.
	// Parameters:
	//     pDC      - Pointer to the used Device Context.
	//     pRow     - Pointer to the Row.
	//     rcBitmap - Bitmap area rectangle coordinates.
	// Returns:
	//     Intended value (right bound of drawn bitmap).
	//-----------------------------------------------------------------------
	int DrawCollapsedBitmap(CDC* pDC, CXTPReportRow* pRow, CRect rcBitmap);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws image of Report Control.
	// Parameters:
	//     pDC      - Pointer to the used Device Context.
	//     pControl - Pointer to report control which image will be drawn.
	//     rcColumn - Bitmap area rectangle coordinates.
	//     iImage   - Image index.
	// Remarks
	//     You use this function to draw a bitmap in the column rectangle.
	//     If the provided rectangle is too small, the bitmap will not be drawn.
	//     Generally, the column rectangle must be big enough to contain
	//     text (if presented) and bitmap
	// Returns:
	//     Right bound of drawn bitmap.
	// Example:
	// <code>
	// // draws bitmap with id ID_READ
	// (pPaintManager->GetGlyphsImageList(), pDC, rcColumn, ID_READ);
	// </code>
	//-----------------------------------------------------------------------
	int DrawBitmap(CDC* pDC, CXTPReportControl* pControl, CRect rcColumn, int iImage);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw standard glyphs of report control
	// Parameters:
	//     pDC      - Pointer to the used Device Context.
	//     rcColumn - Bitmap area rectangle coordinates.
	//     iImage   - Image index.
	// Returns:
	//     Right bound of drawn bitmap.
	//-----------------------------------------------------------------------
	int DrawGlyph(CDC* pDC, CRect rcColumn, int iImage);

	//-----------------------------------------------------------------------
	// Summary:
	//     Changes report control grid lines color.
	// Parameters:
	//     clrGridLine - New grid color.
	// Returns:
	//     Previous grid color
	// Example:
	// <code>
	// // Set new color, save old one
	// COLORREF clrNew;
	// clrNew = RGB(0,255,0);
	// COLORREF clrOld;
	// clrOld = SetGridColor(clrNew);
	// </code>
	//-----------------------------------------------------------------------
	COLORREF SetGridColor(COLORREF clrGridLine);

	//-----------------------------------------------------------------------
	// Summary:
	//     Changes the preview mode state for the control.
	// Parameters:
	//     bIsPreviewMode - New preview mode (TRUE or FALSE).
	// Remarks:
	//     Preview mode has two states: enable and disabled. When preview is
	//     enabled, the control tries to show additional bands with preview text
	// Example:
	// <code>
	// // enable preview mode
	// EnablePreviewMode(TRUE);
	//
	// // disable preview mode
	// EnablePreviewMode(FALSE);
	// </code>
	//-----------------------------------------------------------------------
	void EnablePreviewMode(BOOL bIsPreviewMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns preview mode state for the control.
	// Remarks:
	//     You use this member function to programmatically identify
	//     in which mode the control is
	// Returns:
	//     Current preview mode state.
	//-----------------------------------------------------------------------
	BOOL IsPreviewMode();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the default text font for the control items.
	// Returns:
	//     Pointer to the current font used for drawing test.
	//-----------------------------------------------------------------------
	CFont* GetTextFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Set count of max lines of the preview text.
	// Parameters:
	//     nMaxLines - New value of max lines of text for a preview item.
	// Remarks:
	//     You use this member function to bound the height of a textPreview item
	//     by setting the maximum rows of text for it
	// Returns:
	//     Previous value of max lines of text for a preview item.
	// Example:
	//     <code>int nOldMaxLines = SetMaxPreviewLines(4);</code>
	//-----------------------------------------------------------------------
	int SetMaxPreviewLines(int nMaxLines);

	//-----------------------------------------------------------------------
	// Summary:
	//     Get the count of max lines of  preview text.
	// Returns:
	//     Current value of max lines of text for a preview item.
	//-----------------------------------------------------------------------
	int GetMaxPreviewLines();

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates the line count for preview text, takes into account max lines.
	// Parameters:
	//     pDC     - Pointer to the used Device Context.
	//     rcText  - Reference to text area rectangle coordinates.
	//     strText - String of text.
	// Remarks:
	//     This function calculates the count of preview text lines based on the length
	//     of preview text string, preview text font and width of provided rectangle.
	//     If default max lines count less than count of calculate lines,
	//     returns default max lines
	// Returns:
	//     Returns the count of lines needed to draw preview text.
	//-----------------------------------------------------------------------
	int GetPreviewLinesCount(CDC* pDC, CRect& rcText, CString strText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Return visibility of grid line
	// Parameters:
	//     bVertical - determines which grid lines will be tested,
	//                 vertical or horizontal.
	// Returns:
	//     boolean value which identifies the visibility of the line
	//-----------------------------------------------------------------------
	BOOL IsGridVisible(BOOL bVertical);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets one of the predefined grid line styles
	// Parameters:
	//     bVertical - Determines which grid lines settings,
	//                 vertical or horizontal
	//     gridStyle - Grid style. Can be one of the values listed
	//                 in the remarks section.
	// Remarks:
	//     Grid style can be one of the following:
	//          * <b>xtpGridNoLines</b>   Empty line
	//          * <b>xtpGridSmallDots</b> Line drawn by small dots
	//          * <b>xtpGridLargeDots</b> Line drawn by large dots
	//          * <b>xtpGridDashes</b>    Line drawn by dashes
	//          * <b>xtpGridSolid</b>     Draws solid line
	// See Also: XTPReportGridStyle
	//-----------------------------------------------------------------------
	void SetGridStyle(BOOL bVertical, XTPReportGridStyle gridStyle);

	//--------------------------------------------------------------------------
	// Summary:
	//     Creates and sets default text font
	// Parameters:
	//     lf - Reference to LOGFONT structure
	// See Also: LOGFONT, SetCaptionFont
	//-----------------------------------------------------------------------
	void SetTextFont(LOGFONT& lf);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates and sets the default caption font
	// Parameters:
	//     lf - Reference to LOGFONT structure
	// See Also: LOGFONT, SetTextFont
	//-----------------------------------------------------------------------
	void SetCaptionFont(LOGFONT& lf);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the preview item indent.
	// Parameters:
	//     nLeft   - Left indent to be used
	//     nTop    - Top indent to be used
	//     nRight  - Right indent to be used
	//     nBottom - Bottom indent to be used
	//-----------------------------------------------------------------------
	void SetPreviewIndent(int nLeft, int nTop, int nRight, int nBottom);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw in-place button of the report
	// Parameters:
	//     pDC     - Pointer to the device context
	//     pButton - Pointer to in-place button to draw
	//-----------------------------------------------------------------------
	virtual void DrawInplaceButton(CDC* pDC, CXTPReportInplaceButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw tree elements of report control.
	// Parameters:
	//     pDrawArgs - pointer to structure with drawing arguments
	//     pMetrics  - Metrics of the item
	//     rcItem    - Item area rectangle coordinates.
	//-----------------------------------------------------------------------
	virtual void DrawTreeStructure(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pMetrics, CRect rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draw Item Caption. Override this method to draw custom caption.
	// Parameters:
	//     pDrawArgs - pointer to structure with drawing arguments
	//     pMetrics  - Metrics of the item
	//-----------------------------------------------------------------------
	virtual void DrawItemCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determines if paint manager supports
	//     variable height of row
	// Returns:
	//     TRUE if GetRowHeight retrieves same value for each row.
	// See Also: GetRowHeight
	//-----------------------------------------------------------------------
	BOOL IsFixedRowHeight();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draw hyperlink string using hyperlinks text settings.
	// Parameters:
	//     pnCurrDrawPos - pointer to current drawing position (is updated after each call of function)
	//     pDrawArgs     - pointer to structure with drawing arguments
	//     pHyperlink    - pointer to hyperlink object
	//     strText       - string of text
	//     rcLink        - link hot spot area rectangle coordinates
	//     nFlag         - Flag can allow next values:
	//                     DT_END_ELLIPSIS - truncate text by ellipses
	//                     DT_WORDBREAK - - truncate text by word end
	//     bSelected     - if Row is selected
	//
	// Remarks:
	//     Its member function is internally used by DrawTextLine and draws hyperlink
	//     chunk of text string, updates hyperlink's hot spot.
	// Returns:
	//     Position in CString where drawing of one link text was stopped
	//-----------------------------------------------------------------------
	int DrawLink(int* pnCurrDrawPos, XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, CXTPReportHyperlink* pHyperlink,
		CString strText, CRect rcLink, int nFlag);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draw a line of text with respect to the drawing rect
	// Parameters:
	//     pDrawArgs        - Pointer to structure with drawing arguments.
	//     strText          - String of text.
	//     rcItem           - Item area rectangle coordinates.
	//     nFlag            - Flag can allow next values:
	//                        DT_END_ELLIPSIS - truncate text by ellipses
	//                        DT_WORDBREAK - - truncate text by word end
	//     nCharCounter     - Reference to printed char counted, accepts value before, returns value after drawing
	//     nHyperlikCounter - Reference to printed hyperlinks counted, accepts value before, returns value after drawing
	//     bSelected        - If Row is selected
	// Remarks:
	//     Its member function is internally used by DrawText() and draws one line of text.
	//     It processes plain text/ hyperlink text for one line. For multi-line text
	//     process last line - the truncate by ellipses, otherwise truncate by word
	// Returns:
	//     Position in CString where drawing of one line of text was stopped
	//-----------------------------------------------------------------------
	void DrawTextLine(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, CString strText,
		CRect rcItem, int nFlag, int& nCharCounter, int& nHyperlikCounter);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draw simple string into rectangle
	// Parameters:
	//     pnCurrDrawPos - Pointer to current drawing position (is updated after each call of function)
	//     pDC           - Point to used Device Context
	//     strDraw       - String of text
	//     rcDraw        - Text area rectangle coordinates
	//     nFormat       - Flag can allow next values:
	//                     DT_END_ELLIPSIS - truncate text by ellipses
	//                     DT_WORDBREAK - - truncate text by word end
	// Remarks:
	//     Its member function is internally used by DrawTextLine and draws a plain chunk of
	//     text string.
	// Returns:
	//     Position in CString where drawing of one string of text line was stopped
	//-----------------------------------------------------------------------
	int DrawString(int* pnCurrDrawPos, CDC* pDC, CString strDraw, CRect rcDraw, UINT nFormat);


protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Generates a custom grid pattern
	// Parameters:
	//     pDC      -  Pointer to device context.
	//     pBrush   -  Pointer to brush object.
	//     rc       -  Rectangle area.
	//     pPattern -  Points to a short-integer array that contains the
	//                 initial bitmap bit values. If it is NULL, the new
	//                 bitmap is left uninitialized.
	//-----------------------------------------------------------------------
	void DrawGridPat(CDC* pDC, CBrush* pBrush, CRect rc, CONST VOID*pPattern);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws in-place button frame
	// Parameters:
	//     pDC     - Pointer to device context.
	//     pButton - Pointer to in-place button
	//-----------------------------------------------------------------------
	virtual void DrawInplaceButtonFrame(CDC* pDC, CXTPReportInplaceButton* pButton);

private:
	COLORREF MixColor(COLORREF clrLight, COLORREF clrDark, double dFactor);
	void Line(CDC* pDC, int x, int y, int cx, int cy, CPen* pPen);

public:
	CFont m_fontText;           // Report items default text.
	CFont m_fontBoldText;       // Report items default bold text.
	CFont m_fontCaption;        // Column header caption font.
	CFont m_fontPreview;        // Column header caption font.

public:
	CXTPPaintManagerColor m_clrHighlight;       // Background color of the highlighted row.
	CXTPPaintManagerColor m_clrGridLine;        // Grid lines color.
	CXTPPaintManagerColor m_clrWindowText;      // Report items default text color.
	CXTPPaintManagerColor m_clrHighlightText;   // Text color of the highlighted text.
	CXTPPaintManagerColor m_clrHeaderControl;   // Background color of the report header.
	CXTPPaintManagerColor m_clrCaptionText;     // Column header text color.
	CXTPPaintManagerColor m_clrControlBack;     // Background color of the control report area.
	CXTPPaintManagerColor m_clrGroupRowText;    // Row text color.
	CXTPPaintManagerColor m_clrGroupShadeBack;  // Group row background color when indentation shade is enabled.
	CXTPPaintManagerColor m_clrGroupShadeBorder;// Color for group border's shade.

	CXTPPaintManagerColor m_clrGroupBoxBack;    // Color of group box.
	CXTPPaintManagerColor m_clrControlDark;     // Dark background color of the report control (used on Group By area).
	CXTPPaintManagerColor m_clrControlLightLight;// Light background color used for drawing shades (used on column header area).
	CXTPPaintManagerColor m_clrHotDivider;      // Color of the column hot divider window (2 arrows).
	CXTPPaintManagerColor m_clrHyper;           // Hyperlink color.
	CXTPPaintManagerColor m_clrIndentControl;   // Color of the tree indentation area.
	CXTPPaintManagerColor m_clrItemShade;       // Color of the shade on sorted by column items.
	CXTPPaintManagerColor m_clrBtnFace;         // Standard button face color.
	CXTPPaintManagerColor m_clrBtnText;         // Standard button text color.
	CXTPPaintManagerColor m_clrPreviewText;     // Preview text color.


	BOOL m_bShadeGroupHeadings;     // Show or not show the indentation shade.
	BOOL m_bShadeSortColumn;        // Show or not show the items shade in the sorted by column.


	CString m_strNoItems;       // A string which contains customized "No Items" text for displaying in view when there are no visible rows.
	CString m_strNoGroupBy;     // A string which contains customized "No items" text message for displaying in Group By area when there are no items inside it.
	CString m_strNoFieldsAvailable; // A string which contains customized "No Fields Available" text message for displaying in Field Chooser.
	BOOL m_bHideSelection;      // TRUE when hiding report control's selection by drawing selected items as others, FALSE otherwise.
	BOOL m_bDrawSortTriangleAlways; // If TRUE, the sort triangle displayed in column headers when a column is sorted will always be displayed as long as the column size is large enough to drawn the triangle.  If FALSE, the triangle will be removed when the column size is too small to display the caption and triangle.

	XTPReportColumnStyle m_columnStyle; // Contains column drawing style.
	XTPReportTreeStructureStyle m_treeStructureStyle;   // Tree structure style
	CRect m_rcPreviewIndent;    // Preview indentation.
	int m_nTreeIndent;          // Tree indentation.

protected:
	int m_nRowHeight;           // Report row default height.
	int m_nHeaderHeight;        // Column header height.
	int m_nGroupGridLineHeight; // The height of the group grid line.

	CImageList m_ilGlyphs;      // Contains an additional image list for report control

	BOOL m_bIsPreviewMode;      // Show or not show the item preview.

	int m_nResizingWidth;       // The width of the column resizing marker area.

	int m_nMaxPreviewLines;     // Restrict maximum lines for preview text

	CSize m_szGlyph;            // Glyph size

	CBrush m_brushVeriticalGrid;    // Stores brush to draw vertical grid lines
	CBrush m_brushHorizontalGrid;   // Stores brush to draw horizontal grid lines

	BOOL m_bFixedRowHeight;     // TRUE if fixed rows used.

	XTPReportGridStyle m_verticalGridStyle;     // Stores current style for vertical grid lines
	XTPReportGridStyle m_horizontalGridStyle;   // Stores current style for horizontal grid lines

};

AFX_INLINE int CXTPReportPaintManager::GetHeaderHeight() {
	return m_nHeaderHeight;
}
AFX_INLINE BOOL CXTPReportPaintManager::IsGridVisible(BOOL bVertical) {
	return bVertical? (m_verticalGridStyle != xtpGridNoLines): (m_horizontalGridStyle != xtpGridNoLines);
}

AFX_INLINE void CXTPReportPaintManager::EnablePreviewMode(BOOL bIsPreviewMode) {
	m_bIsPreviewMode = bIsPreviewMode;
}
AFX_INLINE BOOL CXTPReportPaintManager::IsPreviewMode(){
	return m_bIsPreviewMode;
}
AFX_INLINE COLORREF CXTPReportPaintManager::SetGridColor(COLORREF clrGridLine){
	COLORREF clrOldColor = m_clrGridLine;
	m_clrGridLine.SetCustomValue(clrGridLine);
	return clrOldColor;
}

AFX_INLINE CFont* CXTPReportPaintManager::GetTextFont() {
	return &m_fontText;
}
AFX_INLINE int CXTPReportPaintManager::SetMaxPreviewLines(int nMaxLines) {
	int nOldMaxLines = m_nMaxPreviewLines;
	m_nMaxPreviewLines = nMaxLines;
	return nOldMaxLines;
}
AFX_INLINE int CXTPReportPaintManager::GetMaxPreviewLines() {
	return m_nMaxPreviewLines;
}
AFX_INLINE CImageList* CXTPReportPaintManager::GetGlyphsImageList() {
	return &m_ilGlyphs;
}
AFX_INLINE BOOL CXTPReportPaintManager::IsFixedRowHeight() {
	return m_bFixedRowHeight;
}
AFX_INLINE COLORREF CXTPReportPaintManager::MixColor(COLORREF clrLight, COLORREF clrDark, double dFactor) {
	return RGB(GetRValue(clrLight) - dFactor * (GetRValue(clrLight) - GetRValue(clrDark) ),
		GetGValue(clrLight) - dFactor * (GetGValue(clrLight) - GetGValue(clrDark)),
		GetBValue(clrLight) - dFactor * (GetBValue(clrLight) - GetBValue(clrDark)));
}
AFX_INLINE void CXTPReportPaintManager::Line(CDC* pDC, int x, int y, int cx, int cy, CPen* pPen) {
	CPen* pOldPen = pDC->SelectObject(pPen);
	pDC->MoveTo(x, y);
	pDC->LineTo(x + cx, y + cy);
	pDC->SelectObject(pOldPen);
}


#endif //#if !defined(__XTPREPORTPAINTMANAGER_H__)
