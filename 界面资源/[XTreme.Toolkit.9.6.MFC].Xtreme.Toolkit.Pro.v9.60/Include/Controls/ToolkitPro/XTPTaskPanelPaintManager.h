// XTPTaskPanelPaintManager.h interface for the CXTPTaskPanelPaintManager class.
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
#if !defined(__XTPTASKPANELPAINTMANAGER_H__)
#define __XTPTASKPANELPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPTaskPanel;
class CXTPTaskPanelGroupItem;
class CXTPTaskPanelScrollButton;

#include "CommonPro.include/XTPColorManager.h"

//-----------------------------------------------------------------------
// Summary:
//     Structure used to store task panel group colorization information for
//     various  parts of the group.
// Example:
// <code>
//      m_wndTaskPanel.GetPaintManager()->GetGroupColors(FALSE)->clrClientLink = RGB(0, 0, 255);
// </code>
// See Also:
//     CXTPTaskPanelPaintManager::GetGroupColors, CXTPTaskPanelPaintManager::m_groupNormal,
//     CXTPTaskPanelPaintManager::m_groupSpecial
//-----------------------------------------------------------------------
struct XTP_TASKPANEL_GROUPCOLORS
{
	CXTPPaintManagerColorGradient clrHead;       // Color gradient used for the background color of group headers.
	CXTPPaintManagerColor clrClient;             // Background color of the client area of the group.  This is the group area under group header.
	CXTPPaintManagerColor clrClientBorder;       // Color of the client group border.  This is the group area under group header.
	double dHeadGradientFactor;                  // Value used in calculating the gradient of the group header.  Is used to increase or decrease the color as the backcolor of the header is filled in.
	CXTPPaintManagerColor clrHeadTextNormal;     // Color of normal text in the group header
	CXTPPaintManagerColor clrHeadTextHot;        // Color of "hot" text in the group header, this is the color of text on mouse over
	CXTPPaintManagerColor clrClientText;         // Color of text within the client or "body" of the group.  This is the area under the group header.
	CXTPPaintManagerColor clrClientLink;         // Color of links within the client or "body" of the group.  This is the area under the group header.
	CXTPPaintManagerColor clrClientLinkHot;      // Color of "hot" links within the client or "body" of the group.  This is the color of link text on mouse over.
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration used to indicate which type of gripper is drawn
//     on task panel group headers.  This can be used when creating custom
//     themes or you can override the predefined themes to change there
//     grippers if drawn in the group headers.
// Remarks:
//     Depending on the theme and XTPTaskPanelGripper selection, the gripper
//     can appear "hot" on mouse over, and display a different gripper
//     then when the group is expanded and closed.
// Example:
// <code>
//      m_wndTaskPanel.GetPaintManager()->m_eGripper = xtpTaskPanelGripperTriangle;
// </code>
// See Also:
//     CXTPTaskPanelPaintManager::m_eGripper
//
// <KEYWORDS xtpTaskPanelGripperNone, xtpTaskPanelGripperBitmap, xtpTaskPanelGripperPlain, xtpTaskPanelGripperTriangle>
//-----------------------------------------------------------------------
enum XTPTaskPanelGripper
{
	xtpTaskPanelGripperNone,            // Do not draw a gripper in task panel group headers.
	xtpTaskPanelGripperBitmap,          // Display a bitmap in task panel group header, I.e. Arrows in WinXP theme.
	xtpTaskPanelGripperPlain,           // Display the default "classic" gripper in task panel group headers.
	xtpTaskPanelGripperTriangle         // Display a triangle gripper in task panel group headers.
};


//===========================================================================
// Summary:
//     Standalone class used by CXTPTaskPanel as paint manager.
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelPaintManager
{
//{{AFX_CODEJOCK_PRIVATE
typedef BOOL (WINAPI  *AlphaBlendProc) (HDC,int,int,int,int,HDC,int,int,int,int,BLENDFUNCTION);
//}}AFX_CODEJOCK_PRIVATE

public:
	static const CRect rectDefault; // System selected default margins.  All parameters
	                                // of this CRETC are set to CW_USEDEFAULT, which tells
	                                // the system to select the default position for the object
	                                // upper-left corner and ignores the y parameter.  This CRECT
	                                // is used when calculating the margins of the task panel,
	                                // group items margin, group outer margin, and group inner
	                                // margin.  If the margins have not been modified, then this
	                                // CRECT is used to set the margins.

public:
	//-----------------------------------------------------------------------
	//Summary:
	//     Constructs a CXTPTaskPanelPaintManager object
	//-----------------------------------------------------------------------
	CXTPTaskPanelPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTaskPanelPaintManager object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPTaskPanelPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill background of task panel control.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pTaskPanel - Points to a CXTPTaskPanel object
	//-----------------------------------------------------------------------
	virtual void FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a single group of the control.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pGroup - Points to a CXTPTaskPanelGroup object
	//-----------------------------------------------------------------------
	virtual void DrawGroupClient(CDC* pDC, CXTPTaskPanelGroup* pGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the caption of the group.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pGroup - Points to a CXTPTaskPanelGroup object
	//     bDraw - TRUE to draw; FALSE to retrieve height of caption.
	// Returns:
	//     Height of the caption.
	//-----------------------------------------------------------------------
	virtual int DrawGroupCaption(CDC* pDC, CXTPTaskPanelGroup* pGroup, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw single item of control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     pItem - Points to a CXTPTaskPanelGroupItem object.
	//     rc - Rectangle of item to draw.
	//     bDraw - TRUE to draw; FALSE to retrieve height of item
	// Returns:
	//     Height of item.
	//-----------------------------------------------------------------------
	virtual CRect DrawGroupItem(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves group colors.
	// Parameters:
	//     bSpecial - TRUE if the special group color set should be returned,
	//                FASLE if the normal group color set should be returned.
	// Remarks:
	//     If a group is special, it will appear in a different color than
	//     the other groups.  Groups marked as special will be displayed
	//     differently than normal groups drawing the users attention to
	//     that particular group.  This is an excellent way to show the
	//     user the "default" or important items.
	// Returns:
	//     The color set stored in m_groupSpecial if bSpecial is TRUE else
	//     the m_groupNormal color set is returned.
	// See Also:
	//     CXTPTaskPanelGroup::IsSpecialGroup, CXTPTaskPanelGroup::SetSpecialGroup
	//-----------------------------------------------------------------------
	XTP_TASKPANEL_GROUPCOLORS* GetGroupColors(BOOL bSpecial);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member draws a rectangle around a group item when selected,
	//     pressed or highlighted.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pItem - Points to a CXTPTaskPanelGroupItem object.
	//     rc    - Bounding rectangle group to draw.
	//-----------------------------------------------------------------------
	virtual void DrawGroupItemFrame(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the outer margins of a group item.
	// Parameters:
	//     pItem - Points to a CXTPTaskPanelGroupItem object.
	// Returns:
	//     Outer margins of the specified group item.
	// See Also:
	//     GetItemInnerMargins, GetGroupOuterMargins, GetGroupInnerMargins,
	//     GetControlMargins
	//-----------------------------------------------------------------------
	virtual CRect GetItemOuterMargins(CXTPTaskPanelGroupItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the inner margins of a group item.
	// Parameters:
	//     pItem - Points to a CXTPTaskPanelGroupItem object.
	// Returns:
	//     Inner margins of the specified group item.
	// See Also:
	//     GetItemOuterMargins, GetGroupOuterMargins, GetGroupInnerMargins,
	//     GetControlMargins
	//-----------------------------------------------------------------------
	virtual CRect GetItemInnerMargins(CXTPTaskPanelGroupItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the outer margins of a group.
	// Parameters:
	//     pGroup - Points to a CXTPTaskPanelGroup object.
	// Returns:
	//     Outer margins of the specified group.
	// See Also:
	//     GetItemOuterMargins, GetItemInnerMargins, GetGroupInnerMargins,
	//     GetControlMargins
	//-----------------------------------------------------------------------
	virtual CRect GetGroupOuterMargins(CXTPTaskPanelGroup* pGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the inner margins of a group.
	// Parameters:
	//     pGroup - Points to a CXTPTaskPanelGroup object.
	// Returns:
	//     Inner margins of the specified group.
	// See Also:
	//     GetItemOuterMargins, GetItemInnerMargins, GetGroupOuterMargins,
	//     GetControlMargins
	//-----------------------------------------------------------------------
	virtual CRect GetGroupInnerMargins(CXTPTaskPanelGroup* pGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the default margins used for all groups.
	// Parameters:
	//     pPanel - Pointer to a CXTPTaskPanel control
	// Returns:
	//     Default margins for all groups.
	// See Also:
	//     GetItemOuterMargins, GetItemInnerMargins, GetGroupOuterMargins,
	//     GetGroupInnerMargins
	//-----------------------------------------------------------------------
	virtual CRect GetControlMargins(CXTPTaskPanel* pPanel);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to return the spacing used between groups.
	// Parameters:
	//     pPanel - Pointer to a task panel control.
	// Returns:
	//     The amount of spacing placed between task panel groups.
	//-----------------------------------------------------------------------
	virtual int GetGroupSpacing(CXTPTaskPanel* pPanel);

	//-----------------------------------------------------------------------
	// Summary:
	//     The member is called to draw a group scroll button.
	// Parameters:
	//     pDC           - Pointer to a valid device context
	//     pScrollButton - Pointer to the scroll button to draw.
	// Remarks:
	//     DrawScrollButton only draws group scroll buttons, it does not draw
	//     group caption scroll buttons.  DrawCaptionGripper draws the scroll
	//     button for the group caption.
	// See Also:
	//     DrawCaptionGripper
	//-----------------------------------------------------------------------
	virtual void DrawScrollButton(CDC* pDC, CXTPTaskPanelScrollButton* pScrollButton);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the font used to display task panel
	//     item text.  Can use custom font or the standard item font.
	// Parameters:
	//     pLogFont              - Font used to display task panel item text.
	//     bUseStandardItemsFont - TRUE to use standard item font, FALSE to
	//                             use font specified in pLogFont.
	//-----------------------------------------------------------------------
	void SetItemsFontIndirect(LOGFONT* pLogFont, BOOL bUseStandardItemsFont = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the font used to display text in task panel
	//     group caption.  Can use custom font or the standard caption font.
	// Parameters:
	//     pLogFont                - Font to use for task panel group caption text.
	//     bUseStandardCaptionFont - TRUE to use standard caption font.  FALSE to
	//                               use font specified in pLogFont.
	//-----------------------------------------------------------------------
	void SetCaptionFontIndirect(LOGFONT* pLogFont, BOOL bUseStandardCaptionFont = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the font used to display task panel item text.
	// Parameters:
	//     pItem - Pointer to a valid task panel item.
	// Returns:
	//     Font used to display task panel item text.
	//-----------------------------------------------------------------------
	CFont* GetItemFont(CXTPTaskPanelItem* pItem);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to a group item when the layout is xtpTaskItemLayoutImagesWithText.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Points to a CXTPTaskPanelGroupItem object.
	//     rc    - Rectangle of group item to draw.
	//     bDraw - TRUE to draw the item, FALSE to retrieve
	//             the item's bounding rectangle (only the text is drawn).
	// Returns:
	//     Returns 0 if bDraw is TRUE and drawing is successful.
	//
	//     Returns the item's rectangle including the inner margins of the item.
	//     So the left and right margins of the item will be the left and right
	//     margins of the item + the left and right inner margins of the item.
	//
	//     DrawGroupItem calls this method to draw items with a layout if type
	//     xtpTaskItemLayoutImagesWithText.
	// See Also:
	//     DrawGroupItem, DrawGroupItemImageWithTextBelow, DrawGroupItemImage, GetItemInnerMargins
	//-----------------------------------------------------------------------
	CRect DrawGroupItemImageWithText(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to a group item when the layout is xtpTaskItemLayoutImagesWithTextBelow.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Points to a CXTPTaskPanelGroupItem object.
	//     rc    - Rectangle of group item to draw.
	//     bDraw - TRUE to draw the item, FALSE to retrieve
	//             the item's bounding rectangle (only the text is drawn).
	// Returns:
	//     Returns 0 if bDraw is TRUE and drawing is successful.
	//
	//     Returns the item's rectangle including the inner margins of the item.
	//     So the left and right margins of the item will be the left and right
	//     margins of the item + the left and right inner margins of the item.
	//
	//     DrawGroupItem calls this method to draw items with a layout if type
	//     xtpTaskItemLayoutImagesWithTextBelow.
	// See Also:
	//     DrawGroupItem, DrawGroupItemImageWithText, DrawGroupItemImage, GetItemInnerMargins
	//-----------------------------------------------------------------------
	CRect DrawGroupItemImageWithTextBelow(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to a group item when the layout is xtpTaskItemLayoutImages.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Points to a CXTPTaskPanelGroupItem object.
	//     rc    - Rectangle of group item to draw.
	//     bDraw - TRUE to draw the item, FALSE to retrieve
	//             the item's bounding rectangle (no drawing is done).
	// Returns:
	//     Returns 0 if bDraw is TRUE and drawing is successful.
	//
	//     Returns the item's rectangle including the inner margins of the item.
	//     So the left and right margins of the item will be the left and right
	//     margins of the item + the left and right inner margins of the item.
	//
	//     DrawGroupItem calls this method to draw items with a layout if type
	//     xtpTaskItemLayoutImages.
	// See Also:
	//     DrawGroupItem, DrawGroupItemImageWithText, DrawGroupItemImageWithTextBelow, GetItemInnerMargins
	//-----------------------------------------------------------------------
	CRect DrawGroupItemImage(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc, BOOL bDraw);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the face of the group.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pGroup - Points to a CXTPTaskPanelGroup object
	//     rc - Rectangle of group to be draw.
	//-----------------------------------------------------------------------
	virtual void DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the items of the group.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pGroup - Points to a CXTPTaskPanelGroup object.
	//     rc - Rectangle of group to be draw.
	//-----------------------------------------------------------------------
	virtual void DrawGroupClientItems(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw gripper of the caption.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     pGroup - Points to a CXTPTaskPanelGroup object
	//     rc     - Rectangle of group to be drawn.
	// Returns:
	//     Rectangle of caption after draw.
	// Remarks:
	//     The caption gripper is the button that allows the group to be
	//     expanded and collapsed.
	// See Also:
	//     DrawCaptionGripperBitmap, DrawCaptionGripperSigns
	//-----------------------------------------------------------------------
	virtual CRect DrawCaptionGripper(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get width of caption's gripper.
	// Parameters:
	//     pGroup - Points to a CXTPTaskPanelGroup object
	//     rc - Rectangle of group to be drawn.
	// Returns:
	//     Caption's gripper width.
	// Remarks:
	//     The caption gripper is the button that allows the group to be
	//     expanded and collapsed.
	//-----------------------------------------------------------------------
	virtual int GetCaptionGripperWidth(CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw text caption of the task panel item.
	// Parameters:
	//     pDC     - Pointer to a valid device context
	//     pItem   - Points to a CXTPTaskPanelItem object.
	//     rcText  - Rectangle of item to draw.
	//     nFormat - [in] Specifies the method of formatting the text.
	// Remarks:
	//     The nFormat parameter can be one or more of the following values:
	//        * <b>DT_BOTTOM</b>          Justifies the text to the bottom of the rectangle. This value is used
	//                                    only with the DT_SINGLELINE value.
	//        * <b>DT_CALCRECT</b>        Determines the width and height of the rectangle.
	//        * <b>DT_CENTER</b>          Centers text horizontally in the rectangle.
	//        * <b>DT_EDITCONTROL</b>     Duplicates the text-displaying characteristics of a multiline edit
	//                                    control. Specifically, the average character width is calculated in the same manner as for an edit control, and the function does not display a partially visible last line.
	//        * <b>DT_END_ELLIPSIS</b>    For displayed text, if the end of a string does not fit in the rectangle, it is truncated and ellipses are added. If a word that is not at the end of the string goes beyond the limits of the rectangle, it is truncated without ellipses.
	//                                    The string is not modified unless the DT_MODIFYSTRING flag is specified.
	//        * <b>DT_EXPANDTABS</b>      Expands tab characters. The default number of characters per tab is eight. The DT_WORD_ELLIPSIS, DT_PATH_ELLIPSIS, and DT_END_ELLIPSIS values cannot be used with the DT_EXPANDTABS value.
	//        * <b>DT_EXTERNALLEADING</b> Includes the font external leading in line height. Normally, external leading is not included in the height of a line of text.
	//        * <b>DT_HIDEPREFIX</b>      Ignores the ampersand (&) prefix character in the text. The letter that follows will not be underlined, but other mnemonic-prefix characters are still processed.
	//        * <b>DT_INTERNAL</b>        Uses the system font to calculate text metrics.
	//        * <b>DT_LEFT</b>            Aligns text to the left.
	//        * <b>DT_MODIFYSTRING</b>    Modifies the specified string to match the displayed text. This value has no effect unless DT_END_ELLIPSIS or DT_PATH_ELLIPSIS is specified.
	//        * <b>DT_NOCLIP</b>          Draws without clipping.
	//        * <b>DT_NOFULLWIDTHCHARBREAK</b> Prevents a line break at a DBCS (double-wide character string), so that the line breaking rule is equivalent to SBCS strings. For example, this can be used in Korean windows, for more readability of icon labels. This value has no effect unless DT_WORDBREAK is specified.
	//        * <b>DT_NOPREFIX</b>        Turns off processing of prefix characters.
	//        * <b>DT_PATH_ELLIPSIS</b>   For displayed text, replaces characters in the middle of the string with ellipses so that the result fits in the specified rectangle. If the string contains backslash (\) characters, DT_PATH_ELLIPSIS preserves as much as possible of the text after the last backslash.
	//                                    The string is not modified unless the DT_MODIFYSTRING flag is specified.
	//        * <b>DT_PREFIXONLY</b>      Draws only an underline at the position of the character following the ampersand (&) prefix character. Does not draw any other characters in the string.
	//        * <b>DT_RIGHT</b>           Aligns text to the right.
	//        * <b>DT_RTLREADING</b>      Layout in right-to-left reading order for bi-directional text when the font selected into the hdc is a Hebrew or Arabic font. The default reading order for all text is left-to-right.
	//        * <b>DT_SINGLELINE</b>      Displays text on a single line only. Carriage returns and line feeds do not break the line.
	//        * <b>DT_TABSTOP</b>         Sets tab stops. Bits 158 (high-order byte of the low-order word) of the uFormat parameter specify the number of characters for each tab. The default number of characters per tab is eight. The DT_CALCRECT, DT_EXTERNALLEADING, DT_INTERNAL, DT_NOCLIP, and DT_NOPREFIX values cannot be used with the DT_TABSTOP value.
	//        * <b>DT_TOP</b>             Justifies the text to the top of the rectangle.
	//        * <b>DT_VCENTER</b>         Centers text vertically. This value is used only with the DT_SINGLELINE value.
	//        * <b>DT_WORDBREAK</b>       Breaks words. Lines are automatically broken between words if a word would extend past the edge of the rectangle specified by the lpRect parameter. A carriage return-line feed sequence also breaks the line.
	//                                    If this is not specified, output is on one line.
	//        * <b>DT_WORD_ELLIPSIS</b>   Truncates any word that does not fit in the rectangle and adds ellipses.
	//     <p/>
	//     DrawGroupCaption uses this method to draw the group caption with
	//     nFormat = DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS|DT_VCENTER|DT_NOPREFIX
	//
	//     DrawGroupItemImageWithText and DrawGroupItemImageWithTextBelow use
	//     DrawItemCaption to draw the group item caption.
	//-----------------------------------------------------------------------
	virtual void DrawItemCaption(CDC* pDC, CXTPTaskPanelItem* pItem, CRect rcText, UINT nFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the icon of the task panel item.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     pItem  - Points to the item to draw the image on.
	//     ptIcon - Top left corner to start drawing icon.
	//     pImage - Pointer to the image to draw.
	//     szIcon - Size of the image to draw.
	// Remarks:
	//     DrawItemImage is called by DrawGroupItemImageWithText,
	//     DrawGroupItemImageWithTextBelow, and DrawGroupCaption to draw the
	//     image for the item.
	//-----------------------------------------------------------------------
	virtual void DrawItemImage(CDC* pDC, CXTPTaskPanelItem* pItem, CPoint ptIcon, CXTPImageManagerIcon* pImage, CSize szIcon);

	// Helper functions
protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to blend from one color to another color
	//     by the amount specified in nAlpha.
	// Parameters:
	//     clrFrom - RGB Start color
	//     clrTo   - Blend to this RGB color.
	//     nAlpha  - The amount of transparency.
	// Returns:
	//     COLORREF containing blended color.
	// Remarks:
	//     This function takes a blended color intensity which is
	//     (clrFrom * nAlpha + clrTo * (255 - nAlpha)) / 255
	//-----------------------------------------------------------------------
	COLORREF BlendColor(COLORREF clrFrom, COLORREF clrTo, int nAlpha);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the group caption expand button bitmap.
	// Parameters:
	//     pDC       - Pointer to a valid device context
	//     bSpecial  - TRUE if the group is special, FALSE otherwise.
	//     bExpanded - TRUE to draw the bitmap when group is expanded.
	//     bHot      - TRUE to draw the bitmap when the group id "hot".
	//                 This is when the mouse pointer is positioned over the gripper.
	//     rc - CRect object specifying size of area.
	// Returns:
	//     Bounding rectangle of group caption - gripper bitmap width.
	// See Also:
	//     CXTPTaskPanelGroup::SetSpecialGroup, CXTPTaskPanelGroup::IsSpecialGroup
	//-----------------------------------------------------------------------
	CRect DrawCaptionGripperBitmap(CDC* pDC, BOOL bSpecial, BOOL bExpanded, BOOL bHot, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is used to draw the ">>" and "<<" of the group caption
	//     expand button for the xtpTaskPanelThemeOffice2000 theme.
	// Parameters:
	//     pDC       - Pointer to a valid device context
	//     pt - CPoint object specifies x y coordinates.
	//     clr - COLORREF specifies RGB color value.
	//     bExpanded - TRUE to draw the expanded gripper ">>", FLASE to draw the
	//                 retracted gripper "<<".
	//-----------------------------------------------------------------------
	void DrawCaptionGripperSigns(CDC* pDC, CPoint pt, COLORREF clr, BOOL bExpanded);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method loads the group caption expand button bitmaps for the
	//     xtpTaskPanelThemeNativeWinXPPlain and xtpTaskPanelThemeNativeWinXP
	//     themes.
	// Remarks:
	//     If the current operating system is Windows Xp and the current theme
	//     is xtpSystemThemeBlue, xtpSystemThemeOlive, or xtpSystemThemeSilver
	//     then the gripper bitmap is loaded from the operating system.  For
	//     all other OS and themes, CreateGripperIcon is used to create an
	//     emulated version of the Windows Xp gripper.
	// See Also:
	//     CreateGripperIcon
	//-------------------------------------------------------------------------
	void CreateGripperBitmaps();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method creates expand button icons for the xtpTaskPanelThemeNativeWinXPPlain
	//     and xtpTaskPanelThemeNativeWinXP themes when the current operation system used
	//     is not Windows Xp or if the OS is WinXP and Classic themes are used.
	// Parameters:
	//     hIcon          - Specifies icon handle.
	//     nIDResource    - Specifies icon resource ID.
	//     clrGripperBack - Background color of the expand button.
	// See Also:
	//     CreateGripperBitmaps
	//-----------------------------------------------------------------------
	void CreateGripperIcon(HICON& hIcon, UINT nIDResource, COLORREF clrGripperBack);

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to free the memory used by the group caption bitmaps.
	//-------------------------------------------------------------------------
	void FreeBitmaps();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is only used when drawing hot, pressed and selected group
	//     items and scroll buttons when m_bOfficeHighlight is TRUE.
	// Parameters:
	//     pDC      - Pointer to a valid device context
	//     rc       - CRect object specifying size of area.
	//     clrPen   - COLORREF specifying pen RGB color value.
	//     clrBrush - COLORREF specifying brush RGB color value.
	//-----------------------------------------------------------------------
	void Rectangle(CDC* pDC, CRect rc, COLORREF clrPen, COLORREF clrBrush);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to retrieve the highlight offset of
	//     the text of an item.
	// Parameters:
	//     pItem - Points to a CXTPTaskPanelGroupItem object.
	// Returns:
	//     If the item is selected, offset is (1,1), else offset is (0, 0).
	//-----------------------------------------------------------------------
	virtual CPoint GetHighlightOffset(CXTPTaskPanelGroupItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the "drag arrow" while dragging an
	//     item.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pItem  - Pointer to the group item that the drag arrow (line) is
	//              drawn above if dragging up, and the group item that the
	//              drag arrow (line) is drawn below is dragging down.
	//     rc     - CRect object specifying size of area.
	//     bAfter - TRUE to draw the drag arrow at the bottom of pItem, this
	//              is done when dragging in the down direction.  FALSE to
	//              draw the drag arrow before pItem.
	// Remarks:
	//     The "drag arrow" is the line that is placed above or below a group
	//     item when dragging an item.  The drag arrow is draw for all themes
	//     except xtpTaskPanelThemeToolbox.
	// See Also:
	//     CXTPTaskPanelToolboxTheme::DrawGroupCaption
	//-----------------------------------------------------------------------
	virtual void DrawDragArrow(CDC* pDC, CXTPTaskPanelItem* pItem, CRect rc, BOOL bAfter = FALSE);

private:
	void PremultplyAlpha(HBITMAP& bmp);
	void ConvertToHSL(COLORREF clr, double& h, double& s, double& l);
	double ConvertHueToRGB(double temp1, double temp2, double temp3);
	COLORREF ConvertFromHSL(double h, double s, double l);
	void Triangle(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2, COLORREF clr);
	void DrawScrollArrow(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2, BOOL bEnabled);

public:
	BOOL m_bEmulateEplorerTheme;        // TRUE to use Explorer theme under WinXP and Office 2000 under older OS.
	XTPTaskPanelGripper m_eGripper;     // Gripper of the manager.

	HICON m_hGripperNormal;             // Icon of normal group header gripper
	HICON m_hGripperSpecial;            // Icon of special group header gripper.

	HBITMAP m_bmpGrippers[8];           // Bitmaps of alpha grippers.

	BOOL m_bUseBitmapGrippers;          // TRUE to use alpha grippers.
	BOOL m_bBoldCaption;                // TRUE to display caption text in bold font, FALSE to display caption text in normal type.

	CRect m_rcGroupOuterMargins;        // Outer margins of the group.
	CRect m_rcGroupInnerMargins;        // Inner margins of the group.
	CRect m_rcItemOuterMargins;         // Outer margins of the group items.
	CRect m_rcItemInnerMargins;         // Inner margins of the group items.
	CRect m_rcControlMargins;           // Default margins for all groups.
	int   m_nGroupSpacing;              // Amount of space placed between groups in the task panel.

	BOOL m_bOfficeHighlight;            // TRUE to highlight group items and scroll buttons using the Office XP style (XPCOLOR_HIGHLIGHT, XPCOLOR_HIGHLIGHT_CHECKED,XPCOLOR_HIGHLIGHT_PUSHED)
	BOOL m_bInvertDragRect;             // Only used when theme is set to xtpTaskPanelThemeToolbox.
	BOOL m_bCaptionScrollButton;        // TRUE if group caption scroll button is visible, FALSE if it is hidden.
	BOOL m_bEmbossedDisabledText;       // TRUE to draw disabled text embossed.

protected:

	CFont m_fntCaption;                                 // Caption font.
	CFont m_fntIcon;                                    // Normal Items font.
	CFont m_fntIconBold;                                // Bold items font.
	CFont m_fntIconHot;                                 // Hot items font.
	CFont m_fntIconHotBold;                             // Bold items font.

	CXTPPaintManagerColorGradient m_clrBackground;      // Background color of Task Panel.

	XTP_TASKPANEL_GROUPCOLORS m_groupNormal;            // Normal colors set.
	XTP_TASKPANEL_GROUPCOLORS m_groupSpecial;           // Special colors set.

	CXTPPaintManagerColor m_clrHighlightHot;            // Color of items when they are "hot." This the color of the item on mouseover if hot tracking is enabled.
	CXTPPaintManagerColor m_clrHighlightSelected;       // Color of items when they are selected. This is when the item has focus or is clicked on.
	CXTPPaintManagerColor m_clrHighlightPressed;        // Color of items when they are pressed.  This is when the item is clicked on.
	CXTPPaintManagerColorGradient m_clrHighlightBorder; // Border color of items when they are "hot."  This the border color of the item on mouseover if hot tracking is enabled.

	BOOL m_bOfficeBorder;                               // TRUE to use office borders.
	BOOL m_bLeftRoundedCorner;                          // TRUE to round left corner of the group caption.
	BOOL m_bRightRoundedCorner;                         // TRUE to round right corner of the group caption.

	int m_nCaptionHeight;                               // Caption height in pixels.
	AlphaBlendProc m_pAlphaBlend;                       // Alpha blend function.
	HMODULE m_hLib;                                     // HANDLE to msimg32.dll.

	BOOL m_bUseStandardCaptionFont;                     // TRUE to use system font for group caption text.
	BOOL m_bUseStandardItemsFont;                       // TRUE to use system font for item caption text.

	friend class CXTPTaskPanelColorSet;

};

namespace XTPTaskPanelPaintThemes
{

	//===========================================================================
	// Summary:
	//     Class that represents an Office 2003 style theme.
	// See Also:
	//     CXTPTaskPanel::SetTheme
	//===========================================================================
	class _XTP_EXT_CLASS CXTPTaskPanelOffice2003Theme : public CXTPTaskPanelPaintManager
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPTaskPanelOffice2003Theme object.
		//-------------------------------------------------------------------------
		CXTPTaskPanelOffice2003Theme();

		//-------------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of task panel.
		//-------------------------------------------------------------------------
		void RefreshMetrics();
	};

	//===========================================================================
	// Summary:
	//     Class that represents an Explorer style theme.
	// See Also:
	//     CXTPTaskPanel::SetTheme
	//===========================================================================
	class _XTP_EXT_CLASS CXTPTaskPanelExplorerTheme : public CXTPTaskPanelPaintManager
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPTaskPanelExplorerTheme object.
		//-------------------------------------------------------------------------
		CXTPTaskPanelExplorerTheme();

		//-------------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of task panel.
		//-------------------------------------------------------------------------
		void RefreshMetrics();

		BOOL m_bExplorerTheme;  // TRUE to use Explorer theme under WinXP and to use office 2000 under older OSes.
	};


	//===========================================================================
	// Summary:
	//     Base class theme that all other themes are derived from.  Use this
	//     class when creating a custom theme.
	// See Also:
	//     CXTPTaskPanel::SetTheme, CXTPTaskPanel::SetCustomTheme
	//===========================================================================
	class _XTP_EXT_CLASS CXTPTaskPanelPaintManagerPlain : public CXTPTaskPanelPaintManager
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPTaskPanelPaintManagerPlain object.
		//-------------------------------------------------------------------------
		CXTPTaskPanelPaintManagerPlain();

		//-------------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of task panel.
		//-------------------------------------------------------------------------
		void RefreshMetrics();

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the face of the group.
		// Parameters:
		//     pDC    - Pointer to a valid device context
		//     pGroup - Points to a CXTPTaskPanelGroup object
		//     rc     - Rectangle of group to be draw.
		//-----------------------------------------------------------------------
		void DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill background of task panel control.
		// Parameters:
		//     pDC - Pointer to a valid device context
		//     pTaskPanel - Points to a CXTPTaskPanel object
		//-----------------------------------------------------------------------
		void FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the caption of the group.
		// Parameters:
		//     pDC - Pointer to a valid device context
		//     pGroup - Points to a CXTPTaskPanelGroup object
		//     bDraw - TRUE to draw; FALSE to retrieve height of caption.
		// Returns:
		//     Height of the caption.
		//-----------------------------------------------------------------------
		int DrawGroupCaption(CDC* pDC, CXTPTaskPanelGroup* pGroup, BOOL bDraw);

	protected:
		BOOL m_bRoundedFrame;           // TRUE to draw a rounded frame around all of the groups in the task panel.  Can bee seen in the Native WinXP theme.  Default is TRUE.
		BOOL m_bOfficeCaption;          // FALSE to draw a rounded rectangle around caption.  TRUE to not draw a rectangle around caption, caption will not appear in a rectrangle. Default is FALSE.
	};

	//===========================================================================
	// Summary:
	//     The CXTPTaskPanelExplorerThemePlain class is used to enable a classic Explorer style theme.
	// See Also:
	//     CXTPTaskPanel::SetTheme
	//===========================================================================
	class _XTP_EXT_CLASS CXTPTaskPanelExplorerThemePlain : public CXTPTaskPanelPaintManagerPlain
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPTaskPanelExplorerThemePlain object.
		//-------------------------------------------------------------------------
		CXTPTaskPanelExplorerThemePlain();

		//-------------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of task panel.
		//-------------------------------------------------------------------------
		void RefreshMetrics();

		BOOL m_bExplorerTheme;  // TRUE to use Explorer theme under WinXP and to use Office 2000 under older OSes.

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw gripper of the caption.
		// Parameters:
		//     pDC    - Pointer to a valid device context
		//     pGroup - Points to a CXTPTaskPanelGroup object
		//     rc     - Rectangle of group to be drawn.
		// Returns:
		//     Rectangle of caption after draw.
		// Remarks:
		//     The caption gripper is the button that allows the group to be
		//     expanded and collapsed.
		//
		//     If the current operating system is Windows Xp and the current theme
		//     is xtpSystemThemeBlue, xtpSystemThemeOlive, or xtpSystemThemeSilver
		//     then the gripper bitmap is loaded from the operating system.  The WinXp
		//     gripper is created with CXTPTaskPanelPaintManager::CreateGripperBitmaps. For
		//     all other OS and themes, CXTPTaskPanelPaintManager::CreateGripperIcon
		//     is used to create an emulated version of the Windows Xp gripper.
		//
		//     If m_bExplorerTheme is FALSE, then CXTPTaskPanelPaintManager::DrawCaptionGripper
		//     is used to draw the gripper, so no bitmap is used.
		//
		//     If m_bExplorerTheme is TRUE, then CXTPTaskPanelPaintManager::DrawCaptionGripperBitmap
		//     is used to draw the gripper, this load a bitmap as mentioned before.
		//
		// See Also:
		//     CXTPTaskPanelPaintManager::DrawCaptionGripperBitmap,
		//     CXTPTaskPanelPaintManager::DrawCaptionGripperSigns
		//-----------------------------------------------------------------------
		CRect DrawCaptionGripper(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);
	};

	//===========================================================================
	// Summary:
	//     The CXTPTaskPanelOfficeXPThemePlain class is used to enable a classic Office XP style theme.
	// See Also:
	//     CXTPTaskPanel::SetTheme
	//===========================================================================
	class _XTP_EXT_CLASS CXTPTaskPanelOfficeXPThemePlain : public CXTPTaskPanelPaintManagerPlain
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPTaskPanelOfficeXPThemePlain object.
		//-------------------------------------------------------------------------
		CXTPTaskPanelOfficeXPThemePlain();

		//-------------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of task panel.
		//-------------------------------------------------------------------------
		void RefreshMetrics();

	};

	//===========================================================================
	// Summary:
	//     The CXTPTaskPanelOffice2003ThemePlain class is used to enable a classic Office 2003 style theme.
	// See Also:
	//     CXTPTaskPanel::SetTheme
	//===========================================================================
	class _XTP_EXT_CLASS CXTPTaskPanelOffice2003ThemePlain : public CXTPTaskPanelOfficeXPThemePlain
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPTaskPanelOffice2003ThemePlain object.
		//-------------------------------------------------------------------------
		CXTPTaskPanelOffice2003ThemePlain();

		//-------------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of task panel.
		//-------------------------------------------------------------------------
		void RefreshMetrics();
	};


	//===========================================================================
	// Summary:
	//     The CXTPTaskPanelToolboxTheme class is used to enable a Toolbox style theme.
	// See Also:
	//     CXTPTaskPanel::SetTheme
	//===========================================================================
	class _XTP_EXT_CLASS CXTPTaskPanelToolboxTheme : public CXTPTaskPanelPaintManagerPlain
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPTaskPanelToolboxTheme object.
		//-------------------------------------------------------------------------
		CXTPTaskPanelToolboxTheme();

		//-------------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of task panel.
		//-------------------------------------------------------------------------
		void RefreshMetrics();

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the caption of the group.
		// Parameters:
		//     pDC    - Pointer to a valid device context
		//     pGroup - Points to a CXTPTaskPanelGroup object
		//     bDraw  - TRUE to draw; FALSE to retrieve height of caption.
		// Returns:
		//     Height of the caption.
		//-----------------------------------------------------------------------
		int DrawGroupCaption(CDC* pDC, CXTPTaskPanelGroup* pGroup, BOOL bDraw);

	protected:

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to draw the background of the group caption.
		// Parameters:
		//     pDC    - Pointer to a valid device context.
		//     pGroup - Fill group caption of this group.
		//     rc     - Bounding rectangle of the group caption.
		// Returns:
		//     Bounding rectangle of text within group caption.
		// Remarks:
		//     This member will draw the normal, hot, and pressed background for
		//     the group caption.
		//-----------------------------------------------------------------------
		virtual CRect DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

	protected:
		int m_nCaptionTextFormat;   // Format flags of caption text.  See CXTPTaskPanelPaintManager::DrawItemCaption for a list of format flags.

	};

	//===========================================================================
	// Summary:
	//     The CXTPTaskPanelToolboxWhidbeyTheme class is used to enable a VS.NET Whidbey style theme.
	// See Also:
	//     CXTPTaskPanel::SetTheme
	//===========================================================================
	class _XTP_EXT_CLASS CXTPTaskPanelToolboxWhidbeyTheme : public CXTPTaskPanelToolboxTheme
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPTaskPanelToolboxWhidbeyTheme object.
		//-------------------------------------------------------------------------
		CXTPTaskPanelToolboxWhidbeyTheme();

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to draw the background of the group caption.
		// Parameters:
		//     pDC    - Pointer to a valid device context.
		//     pGroup - Fill group caption of this group.
		//     rc     - Bounding rectangle of the group caption.
		// Returns:
		//     Bounding rectangle of text within group caption.
		// Remarks:
		//     This member will draw the Whidbey color gradient in the group
		//     caption rectangle.
		//-----------------------------------------------------------------------
		CRect DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the face of the group.
		// Parameters:
		//     pDC    - Pointer to a valid device context
		//     pGroup - Points to a CXTPTaskPanelGroup object.
		//     rc     - Rectangle of group to be draw.
		//-----------------------------------------------------------------------
		void DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill background of task panel control.
		// Parameters:
		//     pDC - Pointer to a valid device context
		//     pTaskPanel - Points to a CXTPTaskPanel object
		//-----------------------------------------------------------------------
		void FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel);
	};

	//===========================================================================
	// Summary:
	//     The CXTPTaskPanelListViewTheme class is used to enable a List View style theme.
	// See Also:
	//     CXTPTaskPanel::SetTheme
	//===========================================================================
	class _XTP_EXT_CLASS CXTPTaskPanelListViewTheme : public CXTPTaskPanelToolboxTheme
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPTaskPanelListViewTheme object.
		//-------------------------------------------------------------------------
		CXTPTaskPanelListViewTheme();

		//-------------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of task panel.
		//-------------------------------------------------------------------------
		void RefreshMetrics();
	};

	//===========================================================================
	// Summary:
	//     The CXTPTaskPanelListViewOfficeXPTheme class is used to enable a List View Office XP style theme.
	// See Also:
	//     CXTPTaskPanel::SetTheme
	//===========================================================================
	class _XTP_EXT_CLASS CXTPTaskPanelListViewOfficeXPTheme : public CXTPTaskPanelListViewTheme
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPTaskPanelListViewOfficeXPTheme object.
		//-------------------------------------------------------------------------
		CXTPTaskPanelListViewOfficeXPTheme();

		//-------------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of task panel.
		//-------------------------------------------------------------------------
		void RefreshMetrics();
	};

	//===========================================================================
	// Summary:
	//     The CXTPTaskPanelListViewOffice2003Theme class is used to enable a List View Office 2003 style theme.
	// See Also:
	//     CXTPTaskPanel::SetTheme
	//===========================================================================
	class _XTP_EXT_CLASS CXTPTaskPanelListViewOffice2003Theme : public CXTPTaskPanelListViewOfficeXPTheme
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPTaskPanelListViewOffice2003Theme object.
		//-------------------------------------------------------------------------
		CXTPTaskPanelListViewOffice2003Theme();

		//-------------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of task panel.
		//-------------------------------------------------------------------------
		void RefreshMetrics();

		CXTPPaintManagerColorGradient m_grcHot;         // Color gradient used to colorize "hot" items for this theme.
		CXTPPaintManagerColorGradient m_grcPushed;      // Color gradient used to colorize "pushed" items for this theme.

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to draw the background of the group caption.
		// Parameters:
		//     pDC    - Pointer to a valid device context.
		//     pGroup - Fill group caption of this group.
		//     rc     - Bounding rectangle of the group caption.
		// Returns:
		//     Bounding rectangle of group caption.
		// Remarks:
		//     This member will draw the normal, hot, and pressed background for
		//     the group caption.  It will then fill the group caption with the
		//     Office 2003 color gradient.
		//-----------------------------------------------------------------------
		CRect DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);
	};


	//===========================================================================
	// Summary:
	//     CXTPTaskPanelShortcutBarOffice2003Theme is a CXTPTaskPanelListViewOffice2003Theme
	//     derived class used to enable an Office 2003 ShortcutBar style theme.
	// See Also:
	//     CXTPTaskPanel::SetTheme
	//===========================================================================
	class CXTPTaskPanelShortcutBarOffice2003Theme : public CXTPTaskPanelListViewOffice2003Theme
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPTaskPanelShortcutBarOffice2003Theme object.
		//-------------------------------------------------------------------------
		CXTPTaskPanelShortcutBarOffice2003Theme();

		//-------------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of task panel.
		//-------------------------------------------------------------------------
		void RefreshMetrics();

	protected:

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to draw the background of the group caption.
		// Parameters:
		//     pDC    - Pointer to a valid device context.
		//     pGroup - Fill group caption of this group.
		//     rc     - Bounding rectangle of the group caption.
		// Returns:
		//     Bounding rectangle of text within group caption.
		// Remarks:
		//     This member will draw the normal, hot, and pressed background for
		//     the group caption.  It will then fill the group caption with the
		//     Office 2003 color gradient.
		//
		//     This task panel theme looks similar to the ShortcutBar Office 2003
		//     theme.
		//-----------------------------------------------------------------------
		CRect DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the face of the group.
		// Parameters:
		//     pDC - Pointer to a valid device context
		//     pGroup - Points to a CXTPTaskPanelGroup object
		//     rc - Rectangle of group to be draw.
		//-----------------------------------------------------------------------
		void DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

		//-----------------------------------------------------------------------
		//     This method is called to fill background of task panel control.
		// Parameters:
		//     pDC - Pointer to a valid device context
		//     pTaskPanel - Points to a CXTPTaskPanel object
		//-----------------------------------------------------------------------
		void FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel);

	};

}

using namespace XTPTaskPanelPaintThemes;


AFX_INLINE XTP_TASKPANEL_GROUPCOLORS* CXTPTaskPanelPaintManager::GetGroupColors(BOOL bSpecial) {
	return bSpecial? &m_groupSpecial: &m_groupNormal;
}


#endif // !defined(__XTPTASKPANELPAINTMANAGER_H__)
