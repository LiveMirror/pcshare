// XTPPaintManager.h : interface for the CXTPPaintManager class.
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
#if !defined(__XTPPAINTMANAGER_H__)
#define __XTPPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPCommandBarsDefines.h"
#include "CommonPro.include/XTPWinThemeWrapper.h"
#include "CommonPro.include/XTPColorManager.h"
#include "CommonPro.include/XTPDrawHelpers.h"

class CXTPControlButton;
class CXTPControlPopup;
class CXTPControl;
class CXTPToolBar;
class CXTPCommandBar;
class CXTPCommandBars;
class CXTPPopupBar;
class CXTPControlComboBox;
class CXTPTabControl;
class CXTPDockBar;
class CXTPControlEdit;
class CXTPImageManagerIcon;

class CXTPBufferDC;

//-----------------------------------------------------------------------
// Summary:
//     Special control with extended drawing.
// See Also: CXTPPaintManager::DrawSpecialControl
//
// <KEYWORDS xtpButtonExpandToolbar, xtpButtonExpandMenu, xtpButtonHideFloating, xtpButtonExpandFloating, xtpButtonCustomize>
//-----------------------------------------------------------------------
enum XTPSpecialControl
{
	xtpButtonExpandToolbar, // Control is option button of docking toolbar.
	xtpButtonExpandMenu,    // Control is expand button of popup bar.
	xtpButtonHideFloating,  // Control is hide button of floating toolbar.
	xtpButtonExpandFloating,// Control is option button of floating toolbar.
	xtpButtonCustomize      // Control is customize button of "Add or Remove" popup bar.
};

//------------------------------------------------------------------------
// Summary:
//     This structure collect icons options for command bars.
// Example:
//     <code>XTPPaintManager()->GetIconsInfo()->bIconsWithShadow = FALSE;</code>
// See Also: CXTPPaintManager::GetIconsInfo
//------------------------------------------------------------------------
struct XTP_COMMANDBARS_ICONSINFO
{
	BOOL bUseFadedIcons;    // If TRUE, Icons will appear faded.
	BOOL bUseDisabledIcons; // If TRUE, disabled icons will be used when the control is disabled.
	BOOL bIconsWithShadow;  // If TRUE, when the mouse pointer is moved over a command bar control, the icons will appear to "jump" away from the screen casting a shadow.
};

//===========================================================================
// Summary:
//     Paint manager of the command bars.
//===========================================================================
class _XTP_EXT_CLASS CXTPPaintManager : public CCmdTarget
{
	friend class CXTPPaintManager* XTPPaintManager();

	class CXTPPaintManagerDestructor;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC     - Pointer to a valid device context
	//     pButton - Points to a CXTPControl object
	//     bDraw   - TRUE to draw; FALSE to retrieve the size of the control.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	CSize DrawControl(CDC* pDC, CXTPControl* pButton, BOOL bDraw = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control placed on the PopupBar.
	// Parameters:
	//     pDC     - Pointer to a valid device context
	//     pButton - Points to a CXTPControl object
	//     bDraw   - TRUE to draw; FALSE to retrieve the size of the control.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawControlPopupParent(CDC* pDC, CXTPControl* pButton, BOOL bDraw) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control placed on the ToolBar.
	// Parameters:
	//     pDC     - Pointer to a valid device context
	//     pButton - Points to a CXTPControl object
	//     bDraw   - TRUE to draw; FALSE to retrieve the size of the control.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawControlToolBarParent(CDC* pDC, CXTPControl* pButton, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the frame for the SplitButton
	//     control
	// Parameters:
	//     pDC     - Pointer to a valid device context
	//     pButton - Points to a CXTPControl object
	//     rc      - Bounding rectangle to draw
	//-----------------------------------------------------------------------
	virtual void DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     pCombo - ComboBox to draw.
	//     bDraw  - TRUE to draw; FALSE to retrieve the size of the control.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawControlComboBox(CDC* pDC, CXTPControlComboBox* pCombo, BOOL bDraw) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC   - Pointer to a valid device context
	//     bDraw - TRUE to draw; FALSE to retrieve the size of the control.
	//     pEdit - Edit control to draw.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawControlEdit(CDC* pDC, CXTPControlEdit* pEdit, BOOL bDraw) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control placed on the ListBox.
	// Parameters:
	//     pDC       - Pointer to a valid device context
	//     pButton   - Points to a CXTPControl object
	//     rc        - Bounding rectangle to draw.
	//     bSelected - TRUE if the control is selected.
	//     bDraw     - TRUE to draw; FALSE to retrieve the size of the control.
	//     pCommandBars - CommandBars object which metrics need to use.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawListBoxControl(CDC* pDC, CXTPControl* pButton, CRect rc, BOOL bSelected, BOOL bDraw, CXTPCommandBars* pCommandBars = 0 ) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the command bar's face.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     pBar - Points to a CXTPCommandBar object to draw.
	//-----------------------------------------------------------------------
	virtual void FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the control's face
	// Parameters:
	//     pDC     - Pointer to a valid device context
	//     pButton - Points to a CXTPControl object to draw.
	//-----------------------------------------------------------------------
	virtual void DrawControlEntry(CDC* pDC, CXTPControl* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the control's face
	// Parameters:
	//     pDC         - Pointer to a valid device context
	//     rc          - Rectangle to draw.
	//     bEnabled    - TRUE if the control is enabled.
	//     bSelected   - TRUE if the control is selected.
	//     bPressed    - TRUE if the control is pushed.
	//     bChecked    - TRUE if the control is checked.
	//     bPopuped    - TRUE if the control is popuped.
	//     barPosition - Parent's bar position.
	//-----------------------------------------------------------------------
	virtual void DrawControlEntry(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked, BOOL bPopuped, XTPBarPosition barPosition) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the control's face
	// Parameters:
	//     pDC         - Pointer to a valid device context
	//     rc          - Rectangle to draw.
	//     bSelected   - TRUE if the control is selected.
	//     bPressed    - TRUE if the control is pushed.
	//     bEnabled    - TRUE if the control is enabled.
	//     bChecked    - TRUE if the control is checked.
	//     bPopuped    - TRUE if the control is popuped.
	//     bToolBar    - TRUE if the parent bar is a toolbar.
	//     barPosition - Parent's bar position.
	//-----------------------------------------------------------------------
	virtual void DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled = TRUE, BOOL bChecked = FALSE, BOOL bPopuped = FALSE, BOOL bToolBar = TRUE, XTPBarPosition barPosition = xtpBarPopup) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the command bar's gripper.
	// Parameters:
	//     pDC   - Pointer to a valid device context
	//     pBar  - Points to a CXTPCommandBar object
	//     bDraw - TRUE to draw; FALSE to retrieve the size of the gripper.
	// Returns:
	//     Size of the gripper.
	//-----------------------------------------------------------------------
	virtual CSize DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw = TRUE) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a command bar's separator.
	// Parameters:
	//     pDC      - Pointer to a valid device context
	//     pBar     - Points to a CXTPCommandBar object
	//     pControl - Points to a CXTPControl object
	//     bDraw    - TRUE to draw; FALSE to retrieve the size of the separator.
	// Returns:
	//     This method is called to draw a command bar's separator.
	//-----------------------------------------------------------------------
	virtual CSize DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw = TRUE) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine offset of popuped bar.
	// Parameters:
	//     rc        - Control's bounding rectangle.
	//     pControl  - Points to a CXTPControl object
	//     bVertical - TRUE if control docked vertically.
	//-----------------------------------------------------------------------
	virtual void AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL bVertical) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a single pane of the status bar.
	// Parameters:
	//     pDC     - Pointer to a valid device context
	//     pStatusBar - The status bar which paen need to draw
	//     nIndex - Index of the pane to draw.
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarPane(CDC* pDC, CStatusBar* pStatusBar, int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the status bar's gripper.
	// Parameters:
	//     pDC      - Pointer to a valid device context
	//     rcClient - Client rectangle of the status bar.
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarGripper(CDC* pDC, CRect rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill a dockbar.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     pBar - Points to a CXTPDockBar object
	//-----------------------------------------------------------------------
	virtual void FillDockBar(CDC* pDC, CXTPDockBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill a status bar.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     pBar - Points to a CXTPStatusBar object
	//-----------------------------------------------------------------------
	virtual void FillStatusBar(CDC* pDC, CStatusBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the kind of shadow to use.
	// Returns:
	//     TRUE to use alpha and solid shadow; otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL IsOfficeLikeShadow() { return FALSE; };

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the specified controls.
	// Parameters:
	//     pDC         - Pointer to a valid device context
	//     controlType - Special control enumerator.
	//     pButton     - Points to a CXTPControl object to draw.
	//     pBar        - Parent CXTPCommandBar object.
	//     bDraw       - TRUE to draw; FALSE to retrieve the size of the
	//                   control.
	//     lpParam     - Specified parameter.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType, CXTPControl* pButton, CXTPCommandBar* pBar, BOOL bDraw, LPVOID lpParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to retrieve the command bar's borders.
	// Parameters:
	//     pBar - Points to a CXTPCommandBar object that the borders need to get.
	// Returns:
	//     Borders of the command bar.
	//-----------------------------------------------------------------------
	virtual CRect GetCommandBarBorders(CXTPCommandBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a tear-off gripper of Popup Bar.
	// Parameters:
	//     pDC       - Pointer to a valid device context
	//     rcGripper - Bounding rectangle of the gripper.
	//     bSelected - True if the gripper is selected.
	//     bDraw     - TRUE to draw; FALSE to retrieve the size of the gripper.
	// Returns:
	//     Size of the gripper.
	//-----------------------------------------------------------------------
	virtual CSize DrawTearOffGripper(CDC* pDC, CRect rcGripper, BOOL bSelected, BOOL bDraw) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the image of the control.
	// Parameters:
	//     pDC           - Pointer to a valid device context
	//     pt            - Position to draw.
	//     sz            - Size of the image.
	//     pImage        - Points to a CXTPImageManagerIcon object
	//     bSelected     - TRUE if the control is selected.
	//     bPressed      - TRUE if the control is pushed.
	//     bEnabled      - TRUE if the control is enabled.
	//     bChecked      - TRUE if the control is checked.
	//     bPopuped      - TRUE if the control is popuped.
	//     bToolBarImage - TRUE if it is a toolbar image.
	//-----------------------------------------------------------------------
	virtual void DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bEnabled = TRUE, BOOL bChecked = FALSE, BOOL bPopuped = FALSE, BOOL bToolBarImage = TRUE) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to draw gripper of dialog bar.
	// Parameters:
	//     pDC   - Points to a valid device context.
	//     pBar  - Dialog Bar pointer
	//     bDraw - TRUE to draw gripper, FALSE to calculate size.
	// Returns:
	//     Size of gripper to be drawn.
	//-----------------------------------------------------------------------
	virtual CSize DrawDialogBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw) = 0;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates new PaintManager pointer.
	// Parameters:
	//     paintTheme - Theme to be created.
	// Returns:
	//     Pointer to new CXTPPaintManager object.
	// Example:
	//     <code>pCommandBars->SetPaintManager(CXTPPaintManager::CreateTheme(xtpThemeOfficeXP);</code>
	//-----------------------------------------------------------------------
	static CXTPPaintManager* AFX_CDECL CreateTheme(XTPPaintTheme paintTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to switch the visual theme of the Command Bars.
	// Parameters:
	//     paintTheme - Theme to be set. Can be any of the values listed in
	//                  the Remarks section.
	// Remarks:
	//     paintTheme can be one of following:
	//     * <b>xtpThemeOfficeXP</b> Enables Office XP theme.
	//     * <b>xtpThemeOffice2000</b> Enables Office 2000 theme.
	//     * <b>xtpThemeOffice2003</b> Enables Office 2003 theme.
	//     * <b>xtpThemeNativeWinXP</b> Enables Windows XP themes support.
	//     * <b>xtpThemeWhidbey</b> Enables Visual Studio 2005 theme.
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetTheme(XTPPaintTheme paintTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set a custom theme.
	// Parameters:
	//     pTheme - theme to be set.
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetCustomTheme(CXTPPaintManager* pTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the current visual theme.
	// Returns:
	//     The current theme.
	//-----------------------------------------------------------------------
	XTPPaintTheme GetCurrentTheme() { return m_themeCurrent;}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to destroy a paint manager object.
	//-----------------------------------------------------------------------
	static void AFX_CDECL Done();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the base theme of the manager.
	// Returns:
	//     Returns the base theme.
	//-----------------------------------------------------------------------
	virtual XTPPaintTheme BaseTheme() = 0;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the font of the command bar
	// Parameters:
	//     pBar - Points to a CXTPCommandBar object
	//     bBold - TRUE to retrieve BOLD font
	// Returns:
	//     A pointer to a CFont object
	//-----------------------------------------------------------------------
	virtual CFont* GetCommandBarFont(CXTPCommandBar* pBar, BOOL bBold = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a triangle.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pt0 - Specifies the logical coordinates of the first point.
	//     pt1 - Specifies the logical coordinates of the second point.
	//     pt2 - Specifies the logical coordinates of the third point.
	//     clr - Color to fill.
	//-----------------------------------------------------------------------
	static void AFX_CDECL Triangle(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2, COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the fonts to use.
	//-----------------------------------------------------------------------
	void UpdateFonts();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set command bars font.
	// Parameters:
	//     pLogFont         - New LOGFONT to use for text.
	//     bUseStandardFont - When calling SetCommandBarsFontIndirect, always
	//                        use FALSE, this will indicate that pLogFont
	//                        should be used as the new font.
	//-----------------------------------------------------------------------
	void SetCommandBarsFontIndirect(LOGFONT* pLogFont, BOOL bUseStandardFont = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the general font.
	// Returns:
	//     A Pointer to a CFont object
	//-----------------------------------------------------------------------
	CFont* GetRegularFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the general bold font.
	// Returns:
	//     A Pointer to a CFont object
	//-----------------------------------------------------------------------
	CFont* GetRegularBoldFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the icon font.
	// Returns:
	//     A pointer to a CFont object
	//-----------------------------------------------------------------------
	CFont* GetIconFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the icon's bold font.
	// Returns:
	//     A pointer to a CFont object
	//-----------------------------------------------------------------------
	CFont* GetIconBoldFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve caption of floating mini frame.
	// Returns:
	//     A pointer to a CFont object
	//-----------------------------------------------------------------------
	CFont* GetSmCaptionFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to use Tahoma font despite system metrics.
	// Parameters:
	//     bUseOfficeFont - TRUE to use office font.
	//-----------------------------------------------------------------------
	void UseOfficeFont(bool bUseOfficeFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method returns the common controls height.
	// Returns:
	//     Controls height.
	//-----------------------------------------------------------------------
	int GetControlHeight();

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a rectangle.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     rc     - Specifies the rectangle in logical units.
	//     nPen   - Specifies the color used to paint the rectangle.
	//     nBrush - Specifies the color used to fill the rectangle.
	//-----------------------------------------------------------------------
	void Rectangle(CDC* pDC, CRect rc, int nPen, int nBrush);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw a three-dimensional rectangle.
	// Parameters:
	//     pDC          - Pointer to a valid device context
	//     rc           - Specifies the rectangle in logical units.
	//     nTopLeft     - Specifies the color of the top and left sides
	//                    of the three-dimensional rectangle.
	//     nBottomRight - Specifies the color of the bottom and right sides
	//                    of the three-dimensional rectangle.
	//-----------------------------------------------------------------------
	void Draw3dRect(CDC* pDC, CRect rc, int nTopLeft, int nBottomRight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw a check mark.
	// Parameters:
	//     pDC        - Pointer to a valid device context
	//     rcCheck    - Specifies the rectangle in logical units.
	//     clr        - Color to fill.
	//     bLayoutRTL - TRUE to draw inverted check mark
	//-----------------------------------------------------------------------
	void DrawCheckMark(CDC* pDC, CRect rcCheck, COLORREF clr = 0, BOOL bLayoutRTL = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a line.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     p0  - Specifies the logical coordinates of the start position.
	//     p1  - Specifies the logical coordinates of the endpoint for the line.
	//-----------------------------------------------------------------------
	void Line(CDC* pDC, CPoint p0, CPoint p1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a line.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     x0   - Specifies the logical x-coordinate of the start position.
	//     y0   - Specifies the logical y-coordinate of the start position.
	//     x1   - Specifies the logical x-coordinate of the endpoint for the line.
	//     y1   - Specifies the logical y-coordinate of the endpoint for the line.
	//     nPen - Specifies the color used to paint the line.
	//-----------------------------------------------------------------------
	void Line(CDC* pDC, int x0, int y0, int x1, int y1, int nPen);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the pixel at the point.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     x    - Specifies the logical x-coordinate of the point to be set.
	//     y    - Specifies the logical y-coordinate of the point to be set.
	//     nPen -  Specifies the color used to paint the point
	//-----------------------------------------------------------------------
	void Pixel(CDC* pDC, int x, int y, int nPen);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gradient fills the rectangle.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     lpRect - Specifies the rectangle in logical units.
	//     crFrom - Start color.
	//     crTo   - Endpoint color.
	//     bHorz  - TRUE to fill horizontally.
	//-----------------------------------------------------------------------
	void GradientFill(CDC* pDC, LPRECT lpRect, COLORREF crFrom, COLORREF crTo, BOOL bHorz);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves specific icon's options.
	// See Also: XTP_COMMANDBARS_ICONSINFO
	//-----------------------------------------------------------------------
	XTP_COMMANDBARS_ICONSINFO* GetIconsInfo();

	//////////////////////////////////////////////////////////////////////////
	/// Animation Routines

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to animate the PopupBar.
	// Parameters:
	//     pDestDC       - Pointer to device context you must draw to.
	//     pSrcDC        - Device context that contains the bitmap you must
	//                     take.
	//     rc            - Bounding rectangle.
	//     animationType - Type of animation to perform.
	// See Also: SetAnimationDelay
	//-----------------------------------------------------------------------
	virtual void Animate(CDC* pDestDC, CDC* pSrcDC, CRect rc, XTPAnimationType animationType);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to animate the expanding process.
	// Parameters:
	//     pCommandBar - Command bar to aniimate.
	//     pDestDC     - Pointer to device context you must draw to.
	//     pSrcDC      - Device context that contains the bitmap you must
	//                   take.
	//-----------------------------------------------------------------------
	virtual void AnimateExpanding(CXTPCommandBar* pCommandBar, CDC* pDestDC, CDC* pSrcDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set animation delay.
	// Parameters:
	//     nAnimationSteps - Number of steps to take during animation.
	//     nAnimationTime  - Amount of time to rest, in milliseconds, between
	//                       each step.
	//-----------------------------------------------------------------------
	void SetAnimationDelay(int nAnimationSteps, int nAnimationTime);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function retrieves the current color of the specified XP
	//     display element. Display elements are the parts of a window
	//     and the display that appear on the system display screen.
	//     The function returns the red, green, blue (RGB) color value
	//     of the given element.  To provide a consistent look to all
	//     Windows applications, the operating system provides and
	//     maintains global settings that define the colors of various
	//     aspects of the display. These settings as a group are sometimes
	//     referred to as a "color scheme." To view the current color
	//     settings, click Control Panel, double-click the Display icon,
	//     and then click the "Appearance" tab.
	// Parameters:
	//     nIndex - Specifies the display element whose color is to be
	//              retrieved.  This parameter can be one of the
	//              following values displayed in the table below:
	// Remarks:
	//     <TABLE>
	//     <b>Constant</b>                   <b>Value</b>  <b>Description</b>
	//     --------------------------------  ============  ------------------------------------------------------------
	//     COLOR_SCROLLBAR                   0             Scroll bar color
	//     COLOR_BACKGROUND                  1             Desktop color
	//     COLOR_ACTIVECAPTION               2             Color of the title bar for the active window, Specifies the left side color in the color gradient of an active window's title bar if the gradient effect is enabled.
	//     COLOR_INACTIVECAPTION             3             Color of the title bar for the inactive window, Specifies the left side color in the color gradient of an inactive window's title bar if the gradient effect is enabled.
	//     COLOR_MENU                        4             Menu background color
	//     COLOR_WINDOW                      5             Window background color
	//     COLOR_WINDOWFRAME                 6             Window frame color
	//     COLOR_MENUTEXT                    7             Color of text on menus
	//     COLOR_WINDOWTEXT                  8             Color of text in windows
	//     COLOR_CAPTIONTEXT                 9             Color of text in caption, size box, and scroll arrow
	//     COLOR_ACTIVEBORDER                10            Border color of active window
	//     COLOR_INACTIVEBORDER              11            Border color of inactive window
	//     COLOR_APPWORKSPACE                12            Background color of multiple-document interface (MDI) applications
	//     COLOR_HIGHLIGHT                   13            Background color of items selected in a control
	//     COLOR_HIGHLIGHTTEXT               14            Text color of items selected in a control
	//     COLOR_BTNFACE                     15            Face color for three-dimensional display elements and for dialog box backgrounds.
	//     COLOR_BTNSHADOW                   16            Color of shading on the edge of command buttons
	//     COLOR_GRAYTEXT                    17            Grayed (disabled) text
	//     COLOR_BTNTEXT                     18            Text color on push buttons
	//     COLOR_INACTIVECAPTIONTEXT         19            Color of text in an inactive caption
	//     COLOR_BTNHIGHLIGHT                20            Highlight color for 3-D display elements
	//     COLOR_3DDKSHADOW                  21            Darkest shadow color for 3-D display elements
	//     COLOR_3DLIGHT                     22            Second lightest 3-D color after 3DHighlight, Light color for three-dimensional display elements (for edges facing the light source.)
	//     COLOR_INFOTEXT                    23            Color of text in ToolTips
	//     COLOR_INFOBK                      24            Background color of ToolTips
	//     COLOR_HOTLIGHT                    26            Color for a hot-tracked item. Single clicking a hot-tracked item executes the item.
	//     COLOR_GRADIENTACTIVECAPTION       27            Right side color in the color gradient of an active window's title bar. COLOR_ACTIVECAPTION specifies the left side color.
	//     COLOR_GRADIENTINACTIVECAPTION     28            Right side color in the color gradient of an inactive window's title bar. COLOR_INACTIVECAPTION specifies the left side color.
	//     XPCOLOR_TOOLBAR_FACE              30            XP toolbar background color.
	//     XPCOLOR_HIGHLIGHT                 31            XP menu item selected color.
	//     XPCOLOR_HIGHLIGHT_BORDER          32            XP menu item selected border color.
	//     XPCOLOR_HIGHLIGHT_PUSHED          33            XP menu item pushed color.
	//     XPCOLOR_HIGHLIGHT_CHECKED         36            XP menu item checked color.
	//     XPCOLOR_HIGHLIGHT_CHECKED_BORDER  37            An RGB value that represents the XP menu item checked border color.
	//     XPCOLOR_ICONSHADDOW               34            XP menu item icon shadow.
	//     XPCOLOR_GRAYTEXT                  35            XP menu item disabled text color.
	//     XPCOLOR_TOOLBAR_GRIPPER           38            XP toolbar gripper color.
	//     XPCOLOR_SEPARATOR                 39            XP toolbar separator color.
	//     XPCOLOR_DISABLED                  40            XP menu icon disabled color.
	//     XPCOLOR_MENUBAR_FACE              41            XP menu item text background color.
	//     XPCOLOR_MENUBAR_EXPANDED          42            XP hidden menu commands background color.
	//     XPCOLOR_MENUBAR_BORDER            43            XP menu border color.
	//     XPCOLOR_MENUBAR_TEXT              44            XP menu item text color.
	//     XPCOLOR_HIGHLIGHT_TEXT            45            XP menu item selected text color.
	//     XPCOLOR_TOOLBAR_TEXT              46            XP toolbar text color.
	//     XPCOLOR_PUSHED_TEXT               47            XP toolbar pushed text color.
	//     XPCOLOR_TAB_INACTIVE_BACK         48            XP inactive tab background color.
	//     XPCOLOR_TAB_INACTIVE_TEXT         49            XP inactive tab text color.
	//     XPCOLOR_HIGHLIGHT_PUSHED_BORDER   50            An RGB value that represents the XP border color for pushed in 3D elements.
	//     XPCOLOR_CHECKED_TEXT              45            XP color for text displayed in a checked button.
	//     XPCOLOR_3DFACE                    51            XP face color for three- dimensional display elements and for dialog box backgrounds.
	//     XPCOLOR_3DSHADOW                  52            XP shadow color for three-dimensional display elements (for edges facing away from the light source).
	//     XPCOLOR_EDITCTRLBORDER            53            XP color for the border color of edit controls.
	//     XPCOLOR_FRAME                     54            Office 2003 frame color.
	//     XPCOLOR_SPLITTER_FACE             55            XP splitter face color.
	//     XPCOLOR_LABEL                     56            Color for label control (xtpControlLabel)
	//     XPCOLOR_STATICFRAME               57            WinXP Static frame color
	//     </TABLE>
	// Returns:
	//     The red, green, blue (RGB) color value of the given element.
	//-----------------------------------------------------------------------
	COLORREF GetXtremeColor(UINT nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the colors for each element to the
	//     color in the specified color array.
	// Parameters:
	//     cElements    - Number of elements in array.
	//     lpaElements  - Array of elements.
	//     lpaRgbValues - Array of RGB values.
	//-----------------------------------------------------------------------
	void SetColors(int cElements, CONST INT *lpaElements, CONST COLORREF* lpaRgbValues);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the font exists in the system.
	// Parameters:
	//     strFaceName - Font needed to test.
	// Returns:
	//     TRUE if font exists; otherwise returns FALSE
	//-----------------------------------------------------------------------
	static bool AFX_CDECL FontExists(CString& strFaceName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the text
	// Parameters:
	//     pDC        - Pointer to a valid device context
	//     str        - Text to draw.
	//     rcText     - Bounding rectangle.
	//     bDraw      - TRUE to draw; FALSE to retrieve the size of the text.
	//     bVert      - Draws vertically.
	//     bCentered  - Draws centered aligned.
	//     bTriangled - Draws with triangle at the end.
	//     bEmbossed  - Draws text embossed.
	// Returns:
	//     Size of the text to be drawn.
	//-----------------------------------------------------------------------
	CSize DrawTextEx(CDC* pDC, CString str, CRect rcText, BOOL bDraw, BOOL bVert, BOOL bCentered, BOOL bTriangled, BOOL bEmbossed = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will search a string, strip off the mnemonic
	//     '&', and reformat the string.
	// Parameters:
	//     strClear - Text needed to strip.
	//-----------------------------------------------------------------------
	static void AFX_CDECL StripMnemonics(CString& strClear);


public:
//{{AFX_CODEJOCK_PRIVATE
	static void UpdateRTLCaption(CString& strCaption);
	XTPCurrentSystemTheme GetCurrentSystemTheme();
	void SetLunaTheme(XTPCurrentSystemTheme systemTheme);
//}}AFX_CODEJOCK_PRIVATE

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw frame of single cell of status bar.
	// Parameters:
	//     pDC - Points to a valid device context.
	//     rc - CRect object specifying size of area.
	//     bGripperPane - TRUE if pane is last cell of status bar
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarPaneBorder(CDC* pDC, CRect rc, BOOL bGripperPane);

private:
	CSize _DrawButtonText(CDC* pDC, CString strText, CRect rcText, BOOL bVert, BOOL bDraw, BOOL bEmbossed, BOOL bTriangled, CSize szButton, BOOL bDrawImage);
	void AlphaBlendU(PBYTE pDest, PBYTE pSrcBack, int cx, int cy, PBYTE pSrc, BYTE byAlpha);


public:
//{{AFX_CODEJOCK_PRIVATE
	COLORREF m_clrShadowFactor;
	COLORREF m_clrStatusTextColor;
//}}AFX_CODEJOCK_PRIVATE

protected:
	CFont m_fontRegular;                // Regular font.
	CFont m_fontRegularBold;            // Regular bold font.
	CFont m_fontVert;                   // Vertical font.
	CFont m_fontVertBold;               // Vertical font.

	CFont m_fontSmCaption;              // Small caption font.
	CFont m_fontIcon;                   // Icon font.
	CFont m_fontMarlettIcon;            // Marlett font.
	CFont m_fontIconBold;               // Bold font.

	int   m_nTextHeight;                // Text height of the regular font.
	BOOL  m_bUseOfficeFont;             // TRUE to use Tahoma font.
	BOOL  m_bUseStandardFont;           // TRUE to use system font.

	XTPPaintTheme m_themeCurrent;           // Current theme.
	static CXTPPaintManager* s_pInstance;   // Instance of the manager.

	XTP_COMMANDBARS_ICONSINFO m_iconsInfo;  // Specific icons options.


	int m_nAnimationSteps;                  // Steps of the animation.
	int m_nAnimationTime;                   // Time of the animation.

	CXTPPaintManagerColor m_arrColor[XPCOLOR_LAST + 1];  // Self colors array.
	XTPCurrentSystemTheme m_systemTheme;   // Current system theme.

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPaintManager object
	//-----------------------------------------------------------------------
	CXTPPaintManager(void);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPaintManager object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPaintManager(void);

public:
	//---------------------------------------------------------------------------
	// Summary:
	//     CLogFont is a self initializing LOGFONT derived class. It
	//     will allow you to create or copy a LOGFONT object, and defines the
	//     attributes of a font.
	//---------------------------------------------------------------------------
	struct _XTP_EXT_CLASS CLogFont : public LOGFONT
	{
		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs an CXTLogFont object.
		//-----------------------------------------------------------------------
		CLogFont();
	};

	//---------------------------------------------------------------------------
	// Summary:
	//     CNonClientMetrics is a self initializing NONCLIENTMETRICS derived
	//     class. It contains the scalable metrics associated with the
	//     non-client area of a non-minimized window.  This class is used by
	//     the SPI_GETNONCLIENTMETRICS and SPI_SETNONCLIENTMETRICS actions of
	//     SystemParametersInfo.
	//---------------------------------------------------------------------------
	struct _XTP_EXT_CLASS CNonClientMetrics : public NONCLIENTMETRICS
	{
		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CNonClientMetrics object
		//-----------------------------------------------------------------------
		CNonClientMetrics();
	};

	friend class CCommandBarsCtrl;
	friend class CXTPCommandBarsOptions;

};


//---------------------------------------------------------------------------
// Summary:
//     Call this member function to access CXTPPaintManager members.
//     Since this class is designed as a single instance object you can
//     only access version info through this method. You <b>cannot</b>
//     directly instantiate an object of type CXTPPaintManager.
// Example:
//     <code>XTPPaintManager()->GetIconsInfo()->bIconsWithShadow = FALSE;</code>
//---------------------------------------------------------------------------
CXTPPaintManager* XTPPaintManager();


AFX_INLINE CXTPPaintManager* XTPPaintManager() {
	return CXTPPaintManager::s_pInstance;
}


namespace XTPPaintThemes
{
	//===========================================================================
	// Summary:
	//     The CXTPOfficeTheme class is used to enable an Office XP style theme for Command Bars
	// See Also: CXTPPaintManager::SetTheme
	//===========================================================================
	class _XTP_EXT_CLASS CXTPOfficeTheme : public CXTPPaintManager
	{
	public:

		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPOfficeTheme object.
		//-----------------------------------------------------------------------
		CXTPOfficeTheme();

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill the command bar's face.
		// Parameters:
		//     pDC  - Pointer to a valid device context
		//     pBar - Points to a CXTPCommandBar object to draw.
		//-----------------------------------------------------------------------
		virtual void FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill the control's face
		// Parameters:
		//     pDC         - Pointer to a valid device context
		//     rc          - Rectangle to draw.
		//     bEnabled    - TRUE if the control is enabled.
		//     bSelected   - TRUE if the control is selected.
		//     bPressed    - TRUE if the control is pushed.
		//     bChecked    - TRUE if the control is checked.
		//     bPopuped    - TRUE if the control is popuped.
		//     barPosition - Parent's bar position.
		//-----------------------------------------------------------------------
		virtual void DrawControlEntry(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked, BOOL bPopuped, XTPBarPosition barPosition);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the command bar's gripper.
		// Parameters:
		//     pDC   - Pointer to a valid device context
		//     pBar  - Points to a CXTPCommandBar object
		//     bDraw - TRUE to draw; FALSE to retrieve the size of the gripper.
		// Returns:
		//     Size of the gripper.
		//-----------------------------------------------------------------------
		virtual CSize DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw = TRUE);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw a command bar's separator.
		// Parameters:
		//     pDC      - Pointer to a valid device context
		//     pBar     - Points to a CXTPCommandBar object
		//     pControl - Points to a CXTPControl object
		//     bDraw    - TRUE to draw; FALSE to retrieve the size of the separator.
		// Returns:
		//     This method is called to draw a command bar's separator.
		//-----------------------------------------------------------------------
		virtual CSize DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw = TRUE);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the control placed on the ListBox.
		// Parameters:
		//     pDC       - Pointer to a valid device context
		//     pButton   - Points to a CXTPControl object
		//     rc        - Bounding rectangle to draw.
		//     bSelected - TRUE if the control is selected.
		//     bDraw     - TRUE to draw; FALSE to retrieve the size of the control.
		//     pCommandBars - CommandBars object which metrics need to use.
		// Returns:
		//     Size of the control.
		//-----------------------------------------------------------------------
		virtual CSize DrawListBoxControl(CDC* pDC, CXTPControl* pButton, CRect rc, BOOL bSelected, BOOL bDraw, CXTPCommandBars* pCommandBars = 0);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to determine offset of popuped bar.
		// Parameters:
		//     rc       - Control's bounding rectangle.
		//     pControl - Points to a CXTPControl object
		//     bVertical - TRUE if control docked vertically.
		//-----------------------------------------------------------------------
		virtual void AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL bVertical);

		//-----------------------------------------------------------------------
		// Summary:
		//     Determines the kind of shadow to use.
		// Returns:
		//     TRUE to use alpha and solid shadow; otherwise returns FALSE
		//-----------------------------------------------------------------------
		virtual BOOL IsOfficeLikeShadow() { return TRUE; };

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw a tear-off gripper of Popup Bar.
		// Parameters:
		//     pDC       - Pointer to a valid device context
		//     rcGripper - Bounding rectangle of the gripper.
		//     bSelected - True if the gripper is selected.
		//     bDraw     - TRUE to draw; FALSE to retrieve the size of the gripper.
		// Returns:
		//     Size of the gripper.
		//-----------------------------------------------------------------------
		virtual CSize DrawTearOffGripper(CDC* pDC, CRect rcGripper, BOOL bSelected, BOOL bDraw);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the specified controls.
		// Parameters:
		//     pDC         - Pointer to a valid device context
		//     controlType - Special control enumerator.
		//     pButton     - Points to a CXTPControl object to draw.
		//     pBar        - Parent CXTPCommandBar object.
		//     bDraw       - TRUE to draw; FALSE to retrieve the size of the
		//                   control.
		//     lpParam     - Specified parameter.
		// Returns:
		//     Size of the control.
		// See Also: XTPSpecialControl
		//-----------------------------------------------------------------------
		virtual CSize DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType, CXTPControl* pButton, CXTPCommandBar* pBar, BOOL bDraw, LPVOID lpParam);

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to draw gripper of dialog bar.
		// Parameters:
		//     pDC   - Points to a valid device context.
		//     pBar  - Dialog Bar pointer
		//     bDraw - TRUE to draw gripper, FALSE to calculate size.
		// Returns:
		//     Size of gripper to be drawn.
		//-----------------------------------------------------------------------
		virtual CSize DrawDialogBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw);

	protected:

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw popup bar right gripper.
		// Parameters:
		//     pDC       - Points to a valid device context.
		//     x         - Specifies the logical x-coordinate of the upper-left corner of the rectangle.
		//     y         - Specifies the logical y-coordinate of the upper-left corner of the destination rectangle.
		//     cx        - Specifies the width of the rectangle.
		//     cy        - Specifies the height of the rectangle.
		//     bExpanded - TRUE if expanded.gripper.
		//-----------------------------------------------------------------------
		virtual void DrawPopupBarGripper(CDC* pDC, int x, int y, int cx, int cy, BOOL bExpanded = FALSE);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the control placed on the PopupBar.
		// Parameters:
		//     pDC     - Pointer to a valid device context
		//     pButton - Points to a CXTPControl object
		//     bDraw   - TRUE to draw; FALSE to retrieve the size of the control.
		// Returns:
		//     Size of the control.
		//-----------------------------------------------------------------------
		virtual CSize DrawControlPopupParent(CDC* pDC, CXTPControl* pButton, BOOL bDraw);


		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill the control's face
		// Parameters:
		//     pDC         - Pointer to a valid device context
		//     rc          - Rectangle to draw.
		//     bSelected   - TRUE if the control is selected.
		//     bPressed    - TRUE if the control is pushed.
		//     bEnabled    - TRUE if the control is enabled.
		//     bChecked    - TRUE if the control is checked.
		//     bPopuped    - TRUE if the control is popuped.
		//     bToolBar    - TRUE if the parent bar is a toolbar.
		//     barPosition - Parent's bar position.
		//-----------------------------------------------------------------------
		virtual void DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled = TRUE, BOOL bChecked = FALSE, BOOL bPopuped = FALSE, BOOL bToolBar = TRUE, XTPBarPosition barPosition = xtpBarPopup);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the image of the control.
		// Parameters:
		//     pDC           - Pointer to a valid device context
		//     pt            - Position to draw.
		//     sz            - Size of the image.
		//     pImage        - Points to a CXTPImageManagerIcon object
		//     bSelected     - TRUE if the control is selected.
		//     bPressed      - TRUE if the control is pushed.
		//     bEnabled      - TRUE if the control is enabled.
		//     bChecked      - TRUE if the control is checked.
		//     bPopuped      - TRUE if the control is popuped.
		//     bToolBarImage - TRUE if it is a toolbar image.
		//-----------------------------------------------------------------------
		virtual void DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bEnabled = TRUE, BOOL bChecked = FALSE, BOOL bPopuped = FALSE, BOOL bToolBarImage = TRUE);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the control.
		// Parameters:
		//     pDC    - Pointer to a valid device context
		//     bDraw  - TRUE to draw; FALSE to retrieve the size of the control.
		// Returns:
		//     Size of the control.
		//-----------------------------------------------------------------------
		virtual CSize DrawControlComboBox(CDC* pDC, CXTPControlComboBox*, BOOL bDraw);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the control.
		// Parameters:
		//     pDC   - Pointer to a valid device context
		//     bDraw - TRUE to draw; FALSE to retrieve the size of the control.
		//     pEdit - Edit control to draw.
		// Returns:
		//     Size of the control.
		//-----------------------------------------------------------------------
		virtual CSize DrawControlEdit(CDC* pDC, CXTPControlEdit* pEdit, BOOL bDraw);

		//-----------------------------------------------------------------------
		// Summary:
		//     Draws intersect rectangle of popup bar and its control.
		// Parameters:
		//     pDC       - Points to a valid device context.
		//     pPopupBar - Popup bar pointer.
		//     clr       - COLORREF specifies RGB color value.
		//-----------------------------------------------------------------------
		void FillIntersectRect(CDC* pDC, CXTPPopupBar* pPopupBar, COLORREF clr);

		//-----------------------------------------------------------------------
		// Summary:
		//     Calculates Office gripper of popup bar.
		// Parameters:
		//     pBar - Popup bar.
		// Returns:
		//     Width of gripper to draw.
		//-----------------------------------------------------------------------
		int GetGripperWidth(CXTPCommandBar* pBar);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw frame of single cell of status bar.
		// Parameters:
		//     pDC - Points to a valid device context.
		//     rc - CRect object specifying size of area.
		//     bGripperPane - TRUE if pane is last cell of status bar
		//-----------------------------------------------------------------------
		virtual void DrawStatusBarPaneBorder(CDC* pDC, CRect rc, BOOL bGripperPane);

		//-----------------------------------------------------------------------
		// Summary:
		//     Retrieves the base theme of the manager.
		// Returns:
		//     Returns the base theme.
		//-----------------------------------------------------------------------
		XTPPaintTheme BaseTheme() { return xtpThemeOfficeXP; }
	};

	//===========================================================================
	// Summary:
	//     The CXTPDefaultTheme class is used to enable an Office 2000 style theme for Command Bars
	// See Also: CXTPPaintManager::SetTheme
	//===========================================================================
	class _XTP_EXT_CLASS CXTPDefaultTheme : public CXTPPaintManager
	{
	public:
		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPDefaultTheme object.
		//-------------------------------------------------------------------------
		CXTPDefaultTheme();

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill the command bar's face.
		// Parameters:
		//     pDC  - Pointer to a valid device context
		//     pBar - Points to a CXTPCommandBar object to draw.
		//-----------------------------------------------------------------------
		virtual void FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill the control's face
		// Parameters:
		//     pDC         - Pointer to a valid device context
		//     rc          - Rectangle to draw.
		//     bEnabled    - TRUE if the control is enabled.
		//     bSelected   - TRUE if the control is selected.
		//     bPressed    - TRUE if the control is pushed.
		//     bChecked    - TRUE if the control is checked.
		//     bPopuped    - TRUE if the control is popuped.
		//     barPosition - Parent's bar position.
		//-----------------------------------------------------------------------
		virtual void DrawControlEntry(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked, BOOL bPopuped, XTPBarPosition barPosition);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the command bar's gripper.
		// Parameters:
		//     pDC   - Pointer to a valid device context
		//     pBar  - Points to a CXTPCommandBar object
		//     bDraw - TRUE to draw; FALSE to retrieve the size of the gripper.
		// Returns:
		//     Size of the gripper.
		//-----------------------------------------------------------------------
		virtual CSize DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw = TRUE);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw a command bar's separator.
		// Parameters:
		//     pDC      - Pointer to a valid device context
		//     pBar     - Points to a CXTPCommandBar object
		//     pControl - Points to a CXTPControl object
		//     bDraw    - TRUE to draw; FALSE to retrieve the size of the separator.
		// Returns:
		//     This method is called to draw a command bar's separator.
		//-----------------------------------------------------------------------
		virtual CSize DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw = TRUE);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to determine offset of popuped bar.
		// Parameters:
		//     rc       - Control's bounding rectangle.
		//     pControl - Points to a CXTPControl object
		//     bVertical - TRUE if control docked vertically.
		//-----------------------------------------------------------------------
		virtual void AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL bVertical);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to retrieve the command bar's borders.
		// Parameters:
		//     pBar - Points to a CXTPCommandBar object that the borders need to get.
		// Returns:
		//     Borders of the command bar.
		//-----------------------------------------------------------------------
		virtual CRect GetCommandBarBorders(CXTPCommandBar* pBar);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the control placed on the ListBox.
		// Parameters:
		//     pDC       - Pointer to a valid device context
		//     pButton   - Points to a CXTPControl object
		//     rc        - Bounding rectangle to draw.
		//     bSelected - TRUE if the control is selected.
		//     bDraw     - TRUE to draw; FALSE to retrieve the size of the control.
		//     pCommandBars - CommandBars object which metrics need to use.
		// Returns:
		//     Size of the control.
		//-----------------------------------------------------------------------
		virtual CSize DrawListBoxControl(CDC* pDC, CXTPControl* pButton, CRect rc, BOOL bSelected, BOOL bDraw, CXTPCommandBars* pCommandBars = 0);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the specified controls.
		// Parameters:
		//     pDC         - Pointer to a valid device context
		//     controlType - Special control enumerator.
		//     pButton     - Points to a CXTPControl object to draw.
		//     pBar        - Parent CXTPCommandBar object.
		//     bDraw       - TRUE to draw; FALSE to retrieve the size of the
		//                   control.
		//     lpParam     - Specified parameter.
		// Returns:
		//     Size of the control.
		// See Also: XTPSpecialControl
		//-----------------------------------------------------------------------
		virtual CSize DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType, CXTPControl* pButton, CXTPCommandBar* pBar, BOOL bDraw, LPVOID lpParam);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw a tear-off gripper of Popup Bar.
		// Parameters:
		//     pDC       - Pointer to a valid device context
		//     rcGripper - Bounding rectangle of the gripper.
		//     bSelected - True if the gripper is selected.
		//     bDraw     - TRUE to draw; FALSE to retrieve the size of the gripper.
		// Returns:
		//     Size of the gripper.
		//-----------------------------------------------------------------------
		virtual CSize DrawTearOffGripper(CDC* pDC, CRect rcGripper, BOOL bSelected, BOOL bDraw);

	protected:
		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the control placed on the PopupBar.
		// Parameters:
		//     pDC     - Pointer to a valid device context
		//     pButton - Points to a CXTPControl object
		//     bDraw   - TRUE to draw; FALSE to retrieve the size of the control.
		// Returns:
		//     Size of the control.
		//-----------------------------------------------------------------------
		virtual CSize DrawControlPopupParent(CDC* pDC, CXTPControl* pButton, BOOL bDraw);

		//-----------------------------------------------------------------------
		// Summary:
		//     Draws split button frame
		// Parameters:
		//     pDC      - Points to a valid device context.
		//     pButton  - Pointer to split button to draw.
		//     rcButton - Bounding rectangle to draw
		//-----------------------------------------------------------------------
		virtual void DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton, CRect rcButton);


		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill the control's face
		// Parameters:
		//     pDC         - Pointer to a valid device context
		//     rc          - Rectangle to draw.
		//     bSelected   - TRUE if the control is selected.
		//     bPressed    - TRUE if the control is pushed.
		//     bEnabled    - TRUE if the control is enabled.
		//     bChecked    - TRUE if the control is checked.
		//     bPopuped    - TRUE if the control is popuped.
		//     bToolBar    - TRUE if the parent bar is a toolbar.
		//     barPosition - Parent's bar position.
		//-----------------------------------------------------------------------
		virtual void DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled = TRUE, BOOL bChecked = FALSE, BOOL bPopuped = FALSE, BOOL bToolBar = TRUE, XTPBarPosition barPosition = xtpBarPopup);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the image of the control.
		// Parameters:
		//     pDC           - Pointer to a valid device context
		//     pt            - Position to draw.
		//     sz            - Size of the image.
		//     pImage        - Points to a CXTPImageManagerIcon object
		//     bSelected     - TRUE if the control is selected.
		//     bPressed      - TRUE if the control is pushed.
		//     bEnabled      - TRUE if the control is enabled.
		//     bChecked      - TRUE if the control is checked.
		//     bPopuped      - TRUE if the control is popuped.
		//     bToolBarImage - TRUE if it is a toolbar image.
		//-----------------------------------------------------------------------
		virtual void DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bEnabled = TRUE, BOOL bChecked = FALSE, BOOL bPopuped = FALSE, BOOL bToolBarImage = TRUE);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the control.
		// Parameters:
		//     pDC       - Pointer to a valid device context
		//     pComboBox - ComboBox to draw.
		//     bDraw     - TRUE to draw; FALSE to retrieve the size of the control.
		// Returns:
		//     Size of the control.
		//-----------------------------------------------------------------------
		virtual CSize DrawControlComboBox(CDC* pDC, CXTPControlComboBox* pComboBox, BOOL bDraw);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the control.
		// Parameters:
		//     pDC   - Pointer to a valid device context
		//     bDraw - TRUE to draw; FALSE to retrieve the size of the control.
		//     pEdit - Edit control to draw.
		// Returns:
		//     Size of the control.
		//-----------------------------------------------------------------------
		virtual CSize DrawControlEdit(CDC* pDC, CXTPControlEdit* pEdit, BOOL bDraw);

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to draw gripper of dialog bar.
		// Parameters:
		//     pDC   - Points to a valid device context.
		//     pBar  - Dialog Bar pointer
		//     bDraw - TRUE to draw gripper, FALSE to calculate size.
		// Returns:
		//     Size of gripper to be drawn.
		//-----------------------------------------------------------------------
		virtual CSize DrawDialogBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw frame of edit control.
		// Parameters:
		//     pDC - Points to a valid device context.
		//     rc - CRect object specifying size of area.
		//     bEnabled  - TRUE if control is enabled.
		//     bSelected - TRUE if control is selected.
		//-----------------------------------------------------------------------
		virtual void DrawControlEditFrame(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bSelected);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw button od combo box control.
		// Parameters:
		//     pDC - Points to a valid device context.
		//     rcBtn     - Button bounding rectangle.
		//     bEnabled  - TRUE if combo box is enabled
		//     bSelected - TRUE if combo box is selected
		//     bDropped  - TRUE TRUE if combo box is dropped.
		//-----------------------------------------------------------------------
		virtual void DrawControlComboBoxButton(CDC* pDC, CRect rcBtn, BOOL bEnabled, BOOL bSelected, BOOL bDropped);

		//-----------------------------------------------------------------------
		// Summary:
		//     Retrieves the base theme of the manager.
		// Returns:
		//     Returns the base theme.
		//-----------------------------------------------------------------------
		XTPPaintTheme BaseTheme() { return xtpThemeOffice2000; }

//{{AFX_CODEJOCK_PRIVATE
		void DrawShadedRect(CDC *pDC, CRect& rect);
		virtual void DrawText(CDC* pDC, CString strText, CRect& rcText, UINT nFlags, BOOL bDraw, BOOL bSelected, BOOL bEnabled);
//}}AFX_CODEJOCK_PRIVATE


	protected:
		int m_nPopupBarText;    // Popup bar text index.
	};

	//===========================================================================
	// Summary:
	//     The CXTPOffice2003Theme class is used to enable an Office 2003 style theme for Command Bars
	// See Also: CXTPPaintManager::SetTheme
	//===========================================================================
	class _XTP_EXT_CLASS CXTPOffice2003Theme : public CXTPOfficeTheme
	{
	public:
		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPOffice2003Theme object.
		//-----------------------------------------------------------------------
		CXTPOffice2003Theme();

		//-----------------------------------------------------------------------
		// Summary:
		//     Destroys a CXTPOffice2003Theme object, handles cleanup and deallocation
		//-----------------------------------------------------------------------
		virtual ~CXTPOffice2003Theme();

		//-----------------------------------------------------------------------
		// Summary:
		//     Loads bitmap specified by nIDResource and converts its colors.
		// Parameters:
		//     nIDResource - Specifies bitmap resource ID.
		// Returns:
		//     Handle to bitmap with loaded bitmap.
		//-----------------------------------------------------------------------
		static HBITMAP AFX_CDECL GetModernBitmap(UINT nIDResource);

		//-----------------------------------------------------------------------
		// Summary:
		//     Loads bitmap specified by nIDResource and adds it to ImageManager.
		// Parameters:
		//     nIDResource - Specifies bitmap resource ID.
		//     pCommands - Pointer to an array of IDs.
		//     nCount - Number of elements in the array pointed to by lpIDArray.
		//     szIcon - Size of the icons.
		// Returns:
		//     Returns TRUE if successful, FALSE otherwise
		// See also: GetModernBitmap, CXTPImageManager
		//-----------------------------------------------------------------------
		static BOOL AFX_CDECL LoadModernToolbarIcons(UINT nIDResource);
		static BOOL AFX_CDECL LoadModernToolbarIcons(UINT nIDResource, UINT* pCommands, int nCount, CSize szIcon = CSize(16, 15)); // <combine CXTPOffice2003Theme::LoadModernToolbarIcons@UINT>

		//-----------------------------------------------------------------------
		// Summary:
		//     This method creates a new clipping region that consists of the existing clipping region minus the corners rectangles.
		// Parameters:
		//     pDC - Pointer to a valid device context.
		//     rc - CRect object specifying size of area.
		//-----------------------------------------------------------------------
		void ExcludeCorners(CDC* pDC, CRect rc);

	protected:
		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of the manager.
		//-----------------------------------------------------------------------
		virtual void RefreshMetrics();

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw popup bar right gripper.
		// Parameters:
		//     pDC       - Points to a valid device context.
		//     x         - Specifies the logical x-coordinate of the upper-left corner of the rectangle.
		//     y         - Specifies the logical y-coordinate of the upper-left corner of the destination rectangle.
		//     cx        - Specifies the width of the rectangle.
		//     cy        - Specifies the height of the rectangle.
		//     bExpanded - TRUE if expanded.gripper.
		//-----------------------------------------------------------------------
		virtual void DrawPopupBarGripper(CDC* pDC, int x, int y, int cx, int cy, BOOL bExpanded = FALSE);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill a dockbar.
		// Parameters:
		//     pDC  - Pointer to a valid device context
		//     pBar - Points to a CXTPDockBar object
		//-----------------------------------------------------------------------
		virtual void FillDockBar(CDC* pDC, CXTPDockBar* pBar);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill the command bar's face.
		// Parameters:
		//     pDC  - Pointer to a valid device context
		//     pBar - Points to a CXTPCommandBar object to draw.
		//-----------------------------------------------------------------------
		virtual void FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw a command bar's separator.
		// Parameters:
		//     pDC      - Pointer to a valid device context
		//     pBar     - Points to a CXTPCommandBar object
		//     pControl - Points to a CXTPControl object
		//     bDraw    - TRUE to draw; FALSE to retrieve the size of the separator.
		// Returns:
		//     This method is called to draw a command bar's separator.
		//-----------------------------------------------------------------------
		virtual CSize DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw = TRUE);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the command bar's gripper.
		// Parameters:
		//     pDC   - Pointer to a valid device context
		//     pBar  - Points to a CXTPCommandBar object
		//     bDraw - TRUE to draw; FALSE to retrieve the size of the gripper.
		// Returns:
		//     Size of the gripper.
		//-----------------------------------------------------------------------
		virtual CSize DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw = TRUE);


		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill the control's face
		// Parameters:
		//     pDC         - Pointer to a valid device context
		//     rc          - Rectangle to draw.
		//     bSelected   - TRUE if the control is selected.
		//     bPressed    - TRUE if the control is pushed.
		//     bEnabled    - TRUE if the control is enabled.
		//     bChecked    - TRUE if the control is checked.
		//     bPopuped    - TRUE if the control is popuped.
		//     bToolBar    - TRUE if the parent bar is a toolbar.
		//     barPosition - Parent's bar position.
		//-----------------------------------------------------------------------
		virtual void DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled = TRUE, BOOL bChecked = FALSE, BOOL bPopuped = FALSE, BOOL bToolBar = TRUE, XTPBarPosition barPosition = xtpBarPopup);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the specified controls.
		// Parameters:
		//     pDC         - Pointer to a valid device context
		//     controlType - Special control enumerator.
		//     pButton     - Points to a CXTPControl object to draw.
		//     pBar        - Parent CXTPCommandBar object.
		//     bDraw       - TRUE to draw; FALSE to retrieve the size of the
		//                   control.
		//     lpParam     - Specified parameter.
		// Returns:
		//     Size of the control.
		// See Also: XTPSpecialControl
		//-----------------------------------------------------------------------
		virtual CSize DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType, CXTPControl* pButton, CXTPCommandBar* pBar, BOOL bDraw, LPVOID lpParam);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to retrieve the command bar's borders.
		// Parameters:
		//     pBar - Points to a CXTPCommandBar object that the borders need to get.
		// Returns:
		//     Borders of the command bar.
		//-----------------------------------------------------------------------
		virtual CRect GetCommandBarBorders(CXTPCommandBar* pBar);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw a tear-off gripper of Popup Bar.
		// Parameters:
		//     pDC       - Pointer to a valid device context
		//     rcGripper - Bounding rectangle of the gripper.
		//     bSelected - True if the gripper is selected.
		//     bDraw     - TRUE to draw; FALSE to retrieve the size of the gripper.
		// Returns:
		//     Size of the gripper.
		//-----------------------------------------------------------------------
		virtual CSize DrawTearOffGripper(CDC* pDC, CRect rcGripper, BOOL bSelected, BOOL bDraw);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the status bar's gripper.
		// Parameters:
		//     pDC      - Pointer to a valid device context
		//     rcClient - Client rectangle of the status bar.
		//-----------------------------------------------------------------------
		virtual void DrawStatusBarGripper(CDC* pDC, CRect rcClient);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw frame of single cell of status bar.
		// Parameters:
		//     pDC - Points to a valid device context.
		//     rc - CRect object specifying size of area.
		//     bGripperPane - TRUE if pane is last cell of status bar
		//-----------------------------------------------------------------------
		virtual void DrawStatusBarPaneBorder(CDC* pDC, CRect rc, BOOL bGripperPane);

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to draw gripper of dialog bar.
		// Parameters:
		//     pDC   - Points to a valid device context.
		//     pBar  - Dialog Bar pointer
		//     bDraw - TRUE to draw gripper, FALSE to calculate size.
		// Returns:
		//     Size of gripper to be drawn.
		//-----------------------------------------------------------------------
		virtual CSize DrawDialogBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw);

		//-----------------------------------------------------------------------
		// Summary:
		//     Retrieves the base theme of the manager.
		// Returns:
		//     Returns the base theme.
		//-----------------------------------------------------------------------
		XTPPaintTheme BaseTheme() { return xtpThemeOffice2003; }

		//-----------------------------------------------------------------------
		// Summary:
		//     Generates expand bitmap used for CXTPPopupBar.
		//-----------------------------------------------------------------------
		void CreateGradientCircle();

	protected:
		//{{AFX_CODEJOCK_PRIVATE
		void DrawExpandSymbols(CDC* pDC, BOOL bVertical, CRect rc, BOOL bHiddenExists, COLORREF clr);
		void RectangleEx(CDC* pDC, CRect rc, int nColorBorder, BOOL bHoriz, COLORREF clrLight, COLORREF clrDark);
		//}}AFX_CODEJOCK_PRIVATE

	public:
		COLORREF m_clrDockBarDark;      // Dark color of dockbar face
		COLORREF m_clrDockBarLight;     // Light color of dockbar face

		COLORREF m_clrCommandBarDark;   // Dark color of commandbar face
		COLORREF m_clrCommandBarLight;  // Light color of commandbar face

		COLORREF m_clrPopupDark;        // Dark color of popup control
		COLORREF m_clrPopupLight;       // Light color of popup control

		COLORREF m_crlToolbarShadow;    // Toolbar bottom shadow color
		COLORREF m_crlFloatingGripper;  // Gripper color of floating toolbar.

		COLORREF m_crlExpandNormalDark; // Expand button dark color.
		COLORREF m_crlExpandNormalLight;// Expand button light color.

		COLORREF m_clrMenuExpandedDark; // Dark color of popup bar gripper
		COLORREF m_clrMenuExpandedLight;// Light color of popup bar gripper

		COLORREF m_clrCircleDark;       // Expand button dark color.
		COLORREF m_clrCircleLight;      // Expand button light color.

		COLORREF m_clrStatusPane;       // Color of status bar cell..

		BOOL     m_bLunaTheme;          // TRUE if luna theme is active.
		CXTPWinThemeWrapper m_themeStatusBar;   // StatusBar theme helper.

		CImageList m_ilGradientCircle;  // Gradient circle holder.

	};

	//===========================================================================
	// Summary:
	//     The CXTPWhidbeyTheme class is used to enable a Visual Studio 2005 style theme for Command Bars
	// See Also: CXTPPaintManager::SetTheme
	//===========================================================================
	class _XTP_EXT_CLASS CXTPWhidbeyTheme : public CXTPOffice2003Theme
	{
	public:

		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPWhidbeyTheme object.
		//-----------------------------------------------------------------------
		CXTPWhidbeyTheme();

		//-----------------------------------------------------------------------
		// Summary:
		//     Destroys a CXTPWhidbeyTheme object, handles cleanup and deallocation
		//-----------------------------------------------------------------------
		virtual ~CXTPWhidbeyTheme();

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of the manager.
		//-----------------------------------------------------------------------
		void RefreshMetrics();
	};


	//===========================================================================
	// Summary:
	//     The CXTPNativeXPTheme class is used to enable a Windows XP style theme for Command Bars
	// See Also: CXTPPaintManager::SetTheme
	//===========================================================================
	class _XTP_EXT_CLASS CXTPNativeXPTheme : public CXTPDefaultTheme
	{
	public:

		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPNativeXPTheme object.
		//-----------------------------------------------------------------------
		CXTPNativeXPTheme();

		//-----------------------------------------------------------------------
		// Summary:
		//     Destroys a CXTPNativeXPTheme object, handles cleanup and deallocation
		//-----------------------------------------------------------------------
		~CXTPNativeXPTheme();

	protected:

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of the manager.
		//-----------------------------------------------------------------------
		virtual void RefreshMetrics();

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill the command bar's face.
		// Parameters:
		//     pDC  - Pointer to a valid device context
		//     pBar - Points to a CXTPCommandBar object to draw.
		//-----------------------------------------------------------------------
		virtual void FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill the control's face
		// Parameters:
		//     pDC         - Pointer to a valid device context
		//     rc          - Rectangle to draw.
		//     bSelected   - TRUE if the control is selected.
		//     bPressed    - TRUE if the control is pushed.
		//     bEnabled    - TRUE if the control is enabled.
		//     bChecked    - TRUE if the control is checked.
		//     bPopuped    - TRUE if the control is popuped.
		//     bToolBar    - TRUE if the parent bar is a toolbar.
		//     barPosition - Parent's bar position.
		//-----------------------------------------------------------------------
		virtual void DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled = TRUE, BOOL bChecked = FALSE, BOOL bPopuped = FALSE, BOOL bToolBar = TRUE, XTPBarPosition barPosition = xtpBarPopup);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the command bar's gripper.
		// Parameters:
		//     pDC   - Pointer to a valid device context
		//     pBar  - Points to a CXTPCommandBar object
		//     bDraw - TRUE to draw; FALSE to retrieve the size of the gripper.
		// Returns:
		//     Size of the gripper.
		//-----------------------------------------------------------------------
		virtual CSize DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw = TRUE);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill a dockbar.
		// Parameters:
		//     pDC  - Pointer to a valid device context
		//     pBar - Points to a CXTPDockBar object
		//-----------------------------------------------------------------------
		virtual void FillDockBar(CDC* pDC, CXTPDockBar* pBar);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill a status bar.
		// Parameters:
		//     pDC  - Pointer to a valid device context
		//     pBar - Points to a CXTPStatusBar object
		//-----------------------------------------------------------------------
		virtual void FillStatusBar(CDC* pDC, CStatusBar* pBar);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill a dockbar.
		// Parameters:
		//     pDC - Points to a valid device context.
		//     pWnd   - Client area to fill.
		//     pFrame - Parent frame window.
		//-----------------------------------------------------------------------
		void FillDockBarRect(CDC* pDC, CWnd* pWnd, CWnd* pFrame);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to determine offset of popuped bar.
		// Parameters:
		//     rc       - Control's bounding rectangle.
		//     pControl - Points to a CXTPControl object
		//     bVertical - TRUE if control docked vertically.
		//-----------------------------------------------------------------------
		void AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL bVertical);

		//-----------------------------------------------------------------------
		// Summary:
		//     Draws split button frame
		// Parameters:
		//     pDC      - Points to a valid device context.
		//     pButton  - Pointer to split button to draw.
		//     rcButton - Bounding rectangle to draw
		//-----------------------------------------------------------------------
		void DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton, CRect rcButton);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw frame of edit control.
		// Parameters:
		//     pDC - Points to a valid device context.
		//     rc - CRect object specifying size of area.
		//     bEnabled  - TRUE if control is enabled.
		//     bSelected - TRUE if control is selected.
		//-----------------------------------------------------------------------
		virtual void DrawControlEditFrame(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bSelected);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw button od combo box control.
		// Parameters:
		//     pDC - Points to a valid device context.
		//     rcBtn     - Button bounding rectangle.
		//     bEnabled  - TRUE if combo box is enabled
		//     bSelected - TRUE if combo box is selected
		//     bDropped  - TRUE TRUE if combo box is dropped.
		//-----------------------------------------------------------------------
		virtual void DrawControlComboBoxButton(CDC* pDC, CRect rcBtn, BOOL bEnabled, BOOL bSelected, BOOL bDropped);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw the status bar's gripper.
		// Parameters:
		//     pDC      - Pointer to a valid device context
		//     rcClient - Client rectangle of the status bar.
		//-----------------------------------------------------------------------
		virtual void DrawStatusBarGripper(CDC* pDC, CRect rcClient);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw frame of single cell of status bar.
		// Parameters:
		//     pDC - Points to a valid device context.
		//     rc - CRect object specifying size of area.
		//     bGripperPane - TRUE if pane is last cell of status bar
		//-----------------------------------------------------------------------
		virtual void DrawStatusBarPaneBorder(CDC* pDC, CRect rc, BOOL bGripperPane);

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to draw gripper of dialog bar.
		// Parameters:
		//     pDC   - Points to a valid device context.
		//     pBar  - Dialog Bar pointer
		//     bDraw - TRUE to draw gripper, FALSE to calculate size.
		// Returns:
		//     Size of gripper to be drawn.
		//-----------------------------------------------------------------------
		virtual CSize DrawDialogBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw);

		//-----------------------------------------------------------------------
		// Summary:
		//     Retrieves the base theme of the manager.
		// Returns:
		//     Returns the base theme.
		//-----------------------------------------------------------------------
		XTPPaintTheme BaseTheme() { return xtpThemeNativeWinXP; }

	protected:
		CXTPWinThemeWrapper m_themeRebar;           // Rebar theme helper
		CXTPWinThemeWrapper m_themeToolbar;         // Toolbar theme helper
		CXTPWinThemeWrapper m_themeCombo;           // ComboBox theme helper
		CXTPWinThemeWrapper m_themeStatusBar;       // StatusBar theme helper
		COLORREF            m_clrEdgeShadowColor;   // Color of toolbar bottom line
	};

	//===========================================================================
	// Summary:
	//     The CXTPReBarPaintManager template class is used to enable a ReBar theme for Command Bars
	// See Also: CXTPPaintManager::SetTheme
	//===========================================================================
	template <class TBase>
	class CXTPReBarPaintManager : public TBase
	{
	public:

		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CXTPReBarPaintManager object.
		//-----------------------------------------------------------------------
		CXTPReBarPaintManager()
		{
			RefreshMetrics();
		}

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to refresh the visual metrics of the manager.
		//-----------------------------------------------------------------------
		void RefreshMetrics()
		{
			TBase::RefreshMetrics();

			m_themeRebar.OpenThemeData(0, L"REBAR");
		}

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to fill the command bar's face.
		// Parameters:
		//     pDC  - Pointer to a valid device context
		//     pBar - Points to a CXTPCommandBar object to draw.
		//-----------------------------------------------------------------------
		void FillCommandBarEntry (CDC* pDC, CXTPCommandBar* pBar)
		{
			if (pBar->GetPosition() != xtpBarFloating && pBar->GetPosition() != xtpBarPopup)
			{
				if(m_themeRebar.IsAppThemed())
				{
					CXTPWindowRect rcClient(pBar->GetParent());
					pBar->ScreenToClient(&rcClient);

					m_themeRebar.DrawThemeBackground(pDC->GetSafeHdc(), 0, 0, &rcClient, 0);
				}
				else
					pDC->FillSolidRect(CXTPClientRect(pBar), GetSysColor(COLOR_3DFACE));
			}
			else
			{
				TBase::FillCommandBarEntry(pDC, pBar);
			}
		}

	protected:
		CXTPWinThemeWrapper m_themeRebar;  // ReBar theme helper.
	};

}
using namespace XTPPaintThemes;

//////////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPPaintManager::SetAnimationDelay(int nAnimationSteps, int nAnimationTime) {
	m_nAnimationSteps = nAnimationSteps;
	m_nAnimationTime = nAnimationTime;
}
AFX_INLINE CFont* CXTPPaintManager::GetRegularFont(){
	return this == NULL? CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT)): &m_fontRegular;
}
AFX_INLINE CFont* CXTPPaintManager::GetRegularBoldFont(){
	return &m_fontRegularBold;
}
AFX_INLINE CFont* CXTPPaintManager::GetIconFont() {
	return &m_fontIcon;
}
AFX_INLINE CFont* CXTPPaintManager::GetIconBoldFont() {
	return &m_fontIconBold;
}
AFX_INLINE CFont* CXTPPaintManager::GetSmCaptionFont() {
	return &m_fontSmCaption;
}

#endif // #if !defined(__XTPPAINTMANAGER_H__)
