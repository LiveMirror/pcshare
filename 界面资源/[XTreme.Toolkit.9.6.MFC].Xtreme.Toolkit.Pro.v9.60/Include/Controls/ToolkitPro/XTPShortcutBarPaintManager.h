// XTPShortcutBarPaintManager.h interface for the CXTPShortcutBarPaintManager class.
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
#if !defined(__XTPSHORTCUTBARPAINTMANAGER_H__)
#define __XTPSHORTCUTBARPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonPro.include/XTPColorManager.h"

//-----------------------------------------------------------------------
// Summary:
//     XTPShortcutBarPaintTheme is an enumeration used by CXTPShortcutBar to set the theme of the control.
// Example:
// <code>
//     m_wndShortcutBar.SetTheme(xtpShortcutThemeOffice2003);
// </code>
// See Also:
//     CXTPShortcutBar::GetCurrentTheme, CXTPShortcutBar::SetTheme, CXTPShortcutBar::SetCustomTheme
//
// <KEYWORDS xtpShortcutThemeOffice2000, xtpShortcutThemeOfficeXP, xtpShortcutThemeOffice2003>
//-----------------------------------------------------------------------
enum XTPShortcutBarPaintTheme
{
	xtpShortcutThemeOffice2000,   // Office 2000 style theme.
	xtpShortcutThemeOfficeXP,     // Office XP style theme.
	xtpShortcutThemeOffice2003    // Office 2003 style theme.
};

class CXTPShortcutBar;
class CXTPShortcutBarItem;
class CXTPShortcutBarPane;
class CXTPShortcutBarPaneItem;
class CXTPImageManagerIcon;

//===========================================================================
// Summary:
//     Standalone class used by CXTPShortcutBar as paint manager.
//===========================================================================
class _XTP_EXT_CLASS CXTPShortcutBarPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPShortcutBarPaintManager object
	//-----------------------------------------------------------------------
	CXTPShortcutBarPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPShortcutBarPaintManager object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPShortcutBarPaintManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the shortcut
	//     bar paint manager.
	// Remarks:
	//     This refreshes all the visual metrics of the shortcut bar such
	//     as the pane caption color, pane font, pane caption height, etc.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the specified item of the ShortcutBar item.
	// Parameters:
	//     pDC - Points to the client device context.
	//     pItem - Item to be drawn.
	//-----------------------------------------------------------------------
	virtual void DrawShortcutItem(CDC* pDC, CXTPShortcutBarItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the client area of the ShortcutBar control
	// Parameters:
	//     pDC - Points to the client device context.
	//     pShortcutBar - Points to the ShortcutBar control
	//-----------------------------------------------------------------------
	virtual void FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the frame of the ShortcutBar control.
	// Parameters:
	//     pDC - Points to the client device context.
	//     pShortcutBar - Points to the ShortcutBar control.
	// Remarks:
	//     This is the border that is drawn around the entire shortcut bar.
	//-----------------------------------------------------------------------
	virtual void DrawShortcutBarFrame(CDC* pDC, CXTPShortcutBar* pShortcutBar)  = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the specified item of the ShortcutBar control
	// Parameters:
	//     pDC - Points to the client device context.
	//     pItem - Item to be drawn.
	//-----------------------------------------------------------------------
	virtual void FillShortcutItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw gripper of the ShortcutBar control
	// Parameters:
	//     pDC - Points to the client device context.
	//     pShortcutBar - Points to the ShortcutBar control
	//     bDraw - TRUE to draw, FALSE to determine size of the gripper.
	// Returns:
	//     Size of the gripper.
	//-----------------------------------------------------------------------
	virtual int DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the caption of the specified pane.
	// Parameters:
	//     pDC - Points to the client device context.
	//     pPane - Shortcut Pane to be drawn
	//     bDraw - TRUE to draw pane, FALSE to determine size of the caption
	// Remarks:
	//     Panes of the shortcut bar are displayed in the shortcut bar's client
	//     area.  Each pane consists of a caption and one or more sub-panes.
	//     Each sub-pane has it's own caption and is drawn with the
	//     DrawPaneItemCaption method.
	//
	//     The caption of the shortcut bar pane is the bar on the top the pane.
	//     The caption of the pane is set when the pane is created in the
	//     CXTPShortcutBarPane::Create method.
	//
	//     The pane caption can be retrieved with CXTPShortcutBarPane::GetCaption
	//
	//     Shortcut bar pane's have sub-item's which also have a caption that is
	//     drawn with the DrawPaneItemCaption method.
	// Returns:
	//     Size of the caption.  Unless overridden, the default caption is 24 pixes.
	//-----------------------------------------------------------------------
	virtual int DrawPaneCaption(CDC* pDC, CXTPShortcutBarPane* pPane, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the caption of the specified shortcut
	//     pane item.  This is the sub-pane of a shortcut bar pane.
	// Parameters:
	//     pDC - Points to the client device context.
	//     pItem - Shortcut item pane to be drawn
	//     bDraw - bDraw - TRUE to draw, FALSE to determine size of the shortcut
	//             pane caption.
	// Remarks:
	//     Panes of the shortcut bar are displayed in the shortcut bar's client
	//     area.  Each pane consists of a caption and one or more sub-panes.
	//     Each sub-pane has it's own caption and is drawn with the
	//     DrawPaneItemCaption method.
	//
	//     The caption of the pane item is set when the pane item is created in the
	//     CXTPShortcutBarPane::AddItem method.
	//
	//     The sub-items of a pane can be retrieved with the CXTPShortcutBarPane::GetItem
	//     method.
	// Returns:
	//     Size of the pane item caption.  This is retrieved from m_nPaneItemHeight.
	//-----------------------------------------------------------------------
	virtual int DrawPaneItemCaption(CDC* pDC, CXTPShortcutBarPaneItem* pItem, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the expand button.
	// Parameters:
	//     pDC - Points to the client device context.
	//     rc - Client rectangle of the expand button.
	//-----------------------------------------------------------------------
	void DrawExpandButton(CDC* pDC, CRect rc);


	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the image of a Shortcut Bar item.
	// Parameters:
	//     pDC       - Points to the client device context.
	//     pt        - Location at which to draw the image within the specified
	//                 device context.
	//     sz        - Size of the image.
	//     pImage    - Points to a CXTPImageManagerIcon object.
	//     bSelected - TRUE if the shortcut bar item is selected\has focus.
	//                 I.e. When the user clicks on the item.
	//     bPressed  - TRUE if the shortcut bar item is currently pressed.
	//                 I.e. The user is clicking on the item.
	//     bChecked  - TRUE if the shortcut bar item is checked.  I.e.
	//                 toggle buttons.
	// Remarks:
	//     This member draws the image of a shortcut bar item.  The
	//     DrawShortcutItem method uses this method to draw shortcut bar
	//     images.
	//-----------------------------------------------------------------------
	virtual void DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bChecked) = 0;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a rectangle.  The rectangle is
	//     used to draw the shortcut items in the Office XP theme.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     rc     - Specifies the rectangle in logical units.
	//     nPen   - Specifies the color used to paint the rectangle.
	//     nBrush - Specifies the color used to fill the rectangle.
	//-----------------------------------------------------------------------
	void Rectangle(CDC* pDC, CRect rc, int nPen, int nBrush);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the "gripper" lines in the gripper.
	// Parameters:
	//     pDC  - Points to the client device context.
	//     x0   - Specifies the logical x-coordinate of the start position.
	//     y0   - Specifies the logical y-coordinate of the start position.
	//     x1   - Specifies the logical x-coordinate of the endpoint for the line.
	//     y1   - Specifies the logical y-coordinate of the endpoint for the line.
	//     nPen - Specifies the color used to paint the line.
	//-----------------------------------------------------------------------
	void Line(CDC* pDC, int x0, int y0, int x1, int y1, int nPen);


public:
	CFont m_fntText;            // Text font.
	CFont m_fntPaneCaption;     // Caption font.
	CFont m_fntPaneItemCaption; // Sub-caption font.

	int m_nPaneItemHeight;      // Height of sub-pane caption.
	HICON m_hExpandIcon;        // Icon of expand button.

	int m_nNormalIconWidth;     // Width of the icons.
	int m_nExpandedIconWidth;   // Width of the expanded icons.

	COLORREF m_clrPushedText;   // Color of Caption text when an CXTPShortcutBarItem is pressed.

};

namespace XTPShortcutBarPaintThemes
{

	//===========================================================================
	// Summary:
	//     CXTPShortcutBarOffice2003Theme is a CXTPShortcutBarPaintManager derived
	//     class that represents the Office 2003 shortcut bar theme.
	//===========================================================================
	class _XTP_EXT_CLASS CXTPShortcutBarOffice2003Theme : public CXTPShortcutBarPaintManager
	{
	public:
		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPShortcutBarOffice2003Theme object.
		//-------------------------------------------------------------------------
		CXTPShortcutBarOffice2003Theme();

	public:
		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill the client area of the ShortcutBar control.
		// Parameters:
		//     pDC - Points to the client device context.
		//     pShortcutBar - Points to the ShortcutBar control.
		//-----------------------------------------------------------------------
		void FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the frame of the ShortcutBar control.
		// Parameters:
		//     pDC - Points to the client device context.
		//     pShortcutBar - Points to the ShortcutBar control.
		// Remarks:
		//     This is the border that is drawn around the entire shortcut bar.
		//-----------------------------------------------------------------------
		void DrawShortcutBarFrame(CDC* pDC, CXTPShortcutBar* pShortcutBar);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the specified item of the ShortcutBar control.
		// Parameters:
		//     pDC - Points to the client device context.
		//     pItem - Item to be drawn.
		//-----------------------------------------------------------------------
		void FillShortcutItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw gripper of the ShortcutBar control
		// Parameters:
		//     pDC - Points to the client device context.
		//     pShortcutBar - Points to the ShortcutBar control
		//     bDraw - TRUE to draw, FALSE to determine size of the gripper.
		// Returns:
		//     Size of the gripper, which is 7 pixels for the Office 2003 theme.
		//-----------------------------------------------------------------------
		int DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the caption of the specified pane.
		// Parameters:
		//     pDC - Points to the client device context.
		//     pPane - Shortcut Pane to be drawn
		//     bDraw - TRUE to draw pane, FALSE to determine size of the caption
		// Remarks:
		//     Panes of the shortcut bar are displayed in the shortcut bar's client
		//     area.  Each pane consists of a caption and one or more sub-panes.
		//     Each sub-pane has it's own caption and is drawn with the
		//     DrawPaneItemCaption method.
		//
		//     The caption of the shortcut bar pane is the bar on the top the pane.
		//     The caption of the pane is set when the pane is created in the
		//     CXTPShortcutBarPane::Create method.
		//
		//     The pane caption can be retrieved with CXTPShortcutBarPane::GetCaption
		//
		//     Shortcut bar pane's have sub-item's which also have a caption that is
		//     drawn with the DrawPaneItemCaption method.
		// Returns:
		//     Size of the caption.  Which is 24 pixes.
		//-----------------------------------------------------------------------
		int DrawPaneCaption(CDC* pDC, CXTPShortcutBarPane* pPane, BOOL bDraw);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the caption of the specified shortcut
		//     pane item.  This is the sub-pane of a shortcut bar pane.
		// Parameters:
		//     pDC - Points to the client device context.
		//     pItem - Shortcut item pane to be drawn
		//     bDraw - TRUE to draw, FALSE to determine size of the shortcut
		//             pane caption.
		// Remarks:
		//     Panes of the shortcut bar are displayed in the shortcut bar's client
		//     area.  Each pane consists of a caption and one or more sub-panes.
		//     Each sub-pane has it's own caption and is drawn with the
		//     DrawPaneItemCaption method.
		//
		//     The caption of the pane item is set when the pane item is created in the
		//     CXTPShortcutBarPane::AddItem method.
		//
		//     The sub-items of a pane can be retrieved with the CXTPShortcutBarPane::GetItem
		//     method.
		// Returns:
		//     Size of the pane item caption.  This is retrieved from CXTPShortcutBarPaintManager::m_nPaneItemHeight.
		//-----------------------------------------------------------------------
		int DrawPaneItemCaption(CDC* pDC, CXTPShortcutBarPaneItem* pItem, BOOL bDraw);

		//-------------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of the shortcut
		//     bar paint manager.
		// Remarks:
		//     This refreshes all the visual metrics of the shortcut bar such
		//     as the pane caption color, pane font, pane caption height, etc.
		//
		//     If luna colors are enabled, the the shortcut bar will be colored
		//     to match the current system them if the XTPCurrentSystemTheme is
		//     xtpSystemThemeBlue, xtpSystemThemeSilver, or xtpSystemThemeOlive.
		//-------------------------------------------------------------------------
		void RefreshMetrics();

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the image of a Shortcut Bar item.
		// Parameters:
		//     pDC       - Points to the client device context.
		//     pt        - Location at which to draw the image within the specified
		//                 device context.
		//     sz        - Size of the image.
		//     pImage    - Points to a CXTPImageManagerIcon object.
		//     bSelected - TRUE if the shortcut bar item is selected\has focus.
		//                 I.e. When the user clicks on the item.
		//     bPressed  - TRUE if the shortcut bar item is currently pressed.
		//                 I.e. The user is clicking on the item.
		//     bChecked  - TRUE if the shortcut bar item is checked.  I.e.
		//                 toggle buttons.
		// Remarks:
		//     This member draws the image of a shortcut bar item.  The
		//     DrawShortcutItem method uses this method to draw shortcut bar
		//     images.
		//-----------------------------------------------------------------------
		void DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bChecked);

	protected:
		CXTPPaintManagerColorGradient m_grcSelected;            // Color gradient of currently selected item,
		CXTPPaintManagerColorGradient m_grcHot;                 // Color gradient of "hot" items.  And item is hot when the mouse cursor is positioned over the item.
		CXTPPaintManagerColorGradient m_grcPushed;              // Color gradient of item when clicked on.  This color is also used to color the currently selected item on mouse over.  I.e. When the selected item is "hot."
		CXTPPaintManagerColorGradient m_grcShortcutItem;        // Color gradient of shortcut bar item's.

	};


	//===========================================================================
	// Summary:
	//     CXTPShortcutBarOfficeXPTheme is a CXTPShortcutBarPaintManager derived
	//     class that represents the Office 2003 shortcut bar theme.
	//===========================================================================
	class _XTP_EXT_CLASS CXTPShortcutBarOfficeXPTheme : public CXTPShortcutBarPaintManager
	{
	public:
		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill the client area of the ShortcutBar control.
		// Parameters:
		//     pDC - Points to the client device context.
		//     pShortcutBar - Points to the ShortcutBar control.
		//-----------------------------------------------------------------------
		void FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the frame of the ShortcutBar control.
		// Parameters:
		//     pDC - Points to the client device context.
		//     pShortcutBar - Points to the ShortcutBar control.
		// Remarks:
		//     This is the border that is drawn around the entire shortcut bar.
		//-----------------------------------------------------------------------
		void DrawShortcutBarFrame(CDC* pDC, CXTPShortcutBar* pShortcutBar);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the specified item of the ShortcutBar control.
		// Parameters:
		//     pDC - Points to the client device context.
		//     pItem - Item to be drawn.
		//-----------------------------------------------------------------------
		void FillShortcutItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw gripper of the ShortcutBar control
		// Parameters:
		//     pDC - Points to the client device context.
		//     pShortcutBar - Points to the ShortcutBar control
		//     bDraw - TRUE to draw, FALSE to determine size of the gripper.
		// Returns:
		//     Size of the gripper, which is 7 pixels for the Office XP theme.
		//-----------------------------------------------------------------------
		int DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw);


		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the image of a Shortcut Bar item.
		// Parameters:
		//     pDC       - Points to the client device context.
		//     pt        - Location at which to draw the image within the specified
		//                 device context.
		//     sz        - Size of the image.
		//     pImage    - Points to a CXTPImageManagerIcon object.
		//     bSelected - TRUE if the shortcut bar item is selected\has focus.
		//                 I.e. When the user clicks on the item.
		//     bPressed  - TRUE if the shortcut bar item is currently pressed.
		//                 I.e. The user is clicking on the item.
		//     bChecked  - TRUE if the shortcut bar item is checked.  I.e.
		//                 toggle buttons.
		// Remarks:
		//     This member draws the image of a shortcut bar item.  The
		//     DrawShortcutItem method uses this method to draw shortcut bar
		//     images.
		//-----------------------------------------------------------------------
		void DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bChecked);

	};


	//===========================================================================
	// Summary:
	//     CXTPShortcutBarOffice2000Theme is a CXTPShortcutBarPaintManager derived
	//     class that represents the Office 2003 shortcut bar theme.
	//===========================================================================
	class _XTP_EXT_CLASS CXTPShortcutBarOffice2000Theme : public CXTPShortcutBarPaintManager
	{
	public:
		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill the client area of the ShortcutBar control.
		// Parameters:
		//     pDC - Points to the client device context.
		//     pShortcutBar - Points to the ShortcutBar control.
		//-----------------------------------------------------------------------
		void FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the frame of the ShortcutBar control.
		// Parameters:
		//     pDC - Points to the client device context.
		//     pShortcutBar - Points to the ShortcutBar control.
		// Remarks:
		//     This is the border that is drawn around the entire shortcut bar.
		//-----------------------------------------------------------------------
		void DrawShortcutBarFrame(CDC* pDC, CXTPShortcutBar* pShortcutBar);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the specified item of the ShortcutBar control.
		// Parameters:
		//     pDC - Points to the client device context.
		//     pItem - Item to be drawn.
		//-----------------------------------------------------------------------
		void FillShortcutItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw gripper of the ShortcutBar control
		// Parameters:
		//     pDC - Points to the client device context.
		//     pShortcutBar - Points to the ShortcutBar control
		//     bDraw - TRUE to draw, FALSE to determine size of the gripper.
		// Returns:
		//     Size of the gripper, which is 4 pixels for the Office 2000 theme.
		//-----------------------------------------------------------------------
		int DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw a rectangle.  The rectangle is
		//     used to draw the shortcut items.
		// Parameters:
		//     pDC    - Pointer to a valid device context
		//     rc     - Specifies the rectangle in logical units.
		//     nPen   - Specifies the color used to paint the rectangle.
		//     nBrush - Specifies the color used to fill the rectangle.
		// Remarks:
		//     Rectangle is used to draw the rectangle of the shortcut bar items
		//     in the Office XP theme.  This will draw the normal, pressed, and
		//     hot versions of the item.
		//-----------------------------------------------------------------------
		void Rectangle(CDC* pDC, CRect rc, int nPen, int nBrush);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the "gripper" lines in the gripper.
		// Parameters:
		//     pDC  - Points to the client device context.
		//     x0   - Specifies the logical x-coordinate of the start position.
		//     y0   - Specifies the logical y-coordinate of the start position.
		//     x1   - Specifies the logical x-coordinate of the endpoint for the line.
		//     y1   - Specifies the logical y-coordinate of the endpoint for the line.
		//     nPen - Specifies the color used to paint the line.
		//-----------------------------------------------------------------------
		void Line(CDC* pDC, int x0, int y0, int x1, int y1, int nPen);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the image of a Shortcut Bar item.
		// Parameters:
		//     pDC       - Points to the client device context.
		//     pt        - Location at which to draw the image within the specified
		//                 device context.
		//     sz        - Size of the image.
		//     pImage    - Points to a CXTPImageManagerIcon object.
		//     bSelected - TRUE if the shortcut bar item is selected\has focus.
		//                 I.e. When the user clicks on the item.
		//     bPressed  - TRUE if the shortcut bar item is currently pressed.
		//                 I.e. The user is clicking on the item.
		//     bChecked  - TRUE if the shortcut bar item is checked.  I.e.
		//                 toggle buttons.
		// Remarks:
		//     This member draws the image of a shortcut bar item.  The
		//     DrawShortcutItem method uses this method to draw shortcut bar
		//     images.
		//-----------------------------------------------------------------------
		void DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bChecked);

	};

}

using namespace XTPShortcutBarPaintThemes;

#endif // !defined(__XTPSHORTCUTBARPAINTMANAGER_H__)
