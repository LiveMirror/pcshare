// XTPColorManager.h: interface for the CXTPColorManager class.
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
#if !defined(__XTPCOLORMANAGER_H__)
#define __XTPCOLORMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//-----------------------------------------------------------------------
// Summary:
//     Enumeration used to determine the current Windows XP theme in use
// See Also:
//     CXTPColorManager::GetCurrentSystemTheme
// Example:
//     <code>XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();</code>
//
// <KEYWORDS xtpSystemThemeUnknown, xtpSystemThemeBlue, xtpSystemThemeOlive, xtpSystemThemeSilver, xtpSystemThemeAuto, xtpSystemThemeDefault>
//-----------------------------------------------------------------------
enum XTPCurrentSystemTheme
{
	xtpSystemThemeUnknown, // No known theme in use
	xtpSystemThemeBlue,    // Blue theme in use
	xtpSystemThemeOlive,   // Olive theme in use
	xtpSystemThemeSilver,  // Silver theme in use
	xtpSystemThemeAuto,    // Use OS theme.
	xtpSystemThemeDefault  // Default theme (xtpSystemThemeAuto)
};


//{{AFX_CODEJOCK_PRIVATE
#ifndef COLOR_GRADIENTACTIVECAPTION
#define COLOR_GRADIENTACTIVECAPTION           27
#endif

#ifndef COLOR_GRADIENTINACTIVECAPTION
#define COLOR_GRADIENTINACTIVECAPTION         28
#endif
//}}AFX_CODEJOCK_PRIVATE

const UINT XPCOLOR_BASE                     = 30; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_TOOLBAR_FACE             = XPCOLOR_BASE; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_HIGHLIGHT                = 31; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_HIGHLIGHT_BORDER         = 32; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_HIGHLIGHT_PUSHED         = 33; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_HIGHLIGHT_CHECKED        = 36; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_HIGHLIGHT_CHECKED_BORDER = 37; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_ICONSHADDOW              = 34; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_GRAYTEXT                 = 35; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_TOOLBAR_GRIPPER          = 38; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_SEPARATOR                = 39; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_DISABLED                 = 40; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_MENUBAR_FACE             = 41; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_MENUBAR_EXPANDED         = 42; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_MENUBAR_BORDER           = 43; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_MENUBAR_TEXT             = 44; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_HIGHLIGHT_TEXT           = 45; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_TOOLBAR_TEXT             = 46; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_PUSHED_TEXT              = 47; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_TAB_INACTIVE_BACK        = 48; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_TAB_INACTIVE_TEXT        = 49; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_HIGHLIGHT_PUSHED_BORDER  = 50; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_CHECKED_TEXT             = XPCOLOR_HIGHLIGHT_TEXT; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_3DFACE                   = 51; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_3DSHADOW                 = 52; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_EDITCTRLBORDER           = 53; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_FRAME                    = 54; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_SPLITTER_FACE            = 55; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_LABEL                    = 56; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_STATICFRAME              = 57; //<ALIAS CXTPColorManager::GetColor>
const UINT XPCOLOR_LAST                     = 57; //<ALIAS CXTPColorManager::GetColor>

//===========================================================================
// Summary:
//     PaintManagers used COLORREF wrapper.
//===========================================================================
class _XTP_EXT_CLASS CXTPPaintManagerColor
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPaintManagerColor object.
	//-----------------------------------------------------------------------
	CXTPPaintManagerColor()
	{
		m_clrStandardValue = m_clrCustomValue = (COLORREF)-1;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if default color used
	// Returns:
	//     TRUE if default color used
	//-----------------------------------------------------------------------
	BOOL IsDefaultValue() {
		return (m_clrCustomValue == (COLORREF)-1) && (m_clrStandardValue == (COLORREF)-1);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves paintmanager's defined color
	//-----------------------------------------------------------------------
	COLORREF GetStandardColor() {
		return m_clrStandardValue;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets application defined color
	// Parameters:
	//     clr - Application defined color
	//-----------------------------------------------------------------------
	void SetCustomValue(COLORREF clr) {
		m_clrCustomValue = clr;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets paintmanager's color
	// Parameters:
	//     clr - PaintManager's defined color
	//-----------------------------------------------------------------------
	void SetStandardValue(COLORREF clr) {
		m_clrStandardValue = clr;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Resets application defined color
	//-----------------------------------------------------------------------
	void SetDefaultValue() {
		m_clrCustomValue = (COLORREF)-1;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts a CXTPPaintManagerColor object to an COLORREF.
	//-----------------------------------------------------------------------
	operator COLORREF() {
		return  (m_clrCustomValue == (COLORREF)-1)? m_clrStandardValue: m_clrCustomValue;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator assigns an COLORREF to the application defined color
	// Parameters:
	//     clr - Application defined color
	// See Also: SetStandardValue, SetCustomValue
	//-----------------------------------------------------------------------
	const CXTPPaintManagerColor& operator= (COLORREF clr) {
		SetCustomValue(clr);
		return *this;
	}

protected:
	COLORREF m_clrStandardValue;   // Standard color
	COLORREF m_clrCustomValue;     // Application defined color

};


//===========================================================================
// Summary:
//     Helper class used by CXTPColorManager to display gradient color.
// See Also: CXTPPaintManagerColor
//===========================================================================
class _XTP_EXT_CLASS CXTPPaintManagerColorGradient
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the gradient colors for the
	//     structure.
	// Parameters:
	//     clrLight - RGB light color value
	//     clrDark  - RGB dark color value
	//-----------------------------------------------------------------------
	void SetStandardValue(COLORREF clrLight, COLORREF clrDark);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set application defined values for the structure
	// Parameters:
	//     clrLight - RGB light color value
	//     clrDark  - RGB dark color value
	//-----------------------------------------------------------------------
	void SetCustomValue(COLORREF clrLight, COLORREF clrDark);

	//-----------------------------------------------------------------------
	// Summary:
	//     Overloaded assignment operator sets the value of the dark and
	//     light colors equal to the specified COLORREF value.
	// Parameters:
	//     clr - RGB color value
	//-----------------------------------------------------------------------
	void SetStandardValue(const COLORREF& clr);

public:
	CXTPPaintManagerColor clrLight;     // Light color.
	CXTPPaintManagerColor clrDark;      // Dark color.
};

//===========================================================================
// Summary:
//     CXTPColorManager is a stand alone class. This object is used for manipulation of
//     RGB values.
// See Also: XTPColorManager
//===========================================================================
class _XTP_EXT_CLASS CXTPColorManager : public CCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Application-defined callback function used with the SetGetSysColorPtr function.
	// See Also: SetGetSysColorPtr
	//-----------------------------------------------------------------------
	typedef DWORD (__stdcall *PFNGETSYSCOLOR)(int nIndex);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This function will refresh the display element colors defined
	//     in the CXTPColorManager object.
	// Parameters:
	//     bInit - TRUE when called for initialization.
	//-----------------------------------------------------------------------
	void RefreshColors(BOOL bInit = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the current color of the specified XP display element.
	// Parameters:
	//     nIndex - Specifies the display element whose color is to be
	//              retrieved.  This parameter can be one of the
	//              following values displayed in the table below:
	// Remarks:
	//     Display elements are the parts of a window
	//     and the display that appear on the system display screen.
	//     The function returns the red, green, blue (RGB) color value
	//     of the given element.  To provide a consistent look to all
	//     Windows applications, the operating system provides and
	//     maintains global settings that define the colors of various
	//     aspects of the display. These settings as a group are sometimes
	//     referred to as a "color scheme." To view the current color
	//     settings, click Control Panel, double-click the Display icon,
	//     and then click the "Appearance" tab.
	//<TABLE>
	//  <b>Constant</b>                   <b>Value</b>  <b>Description</b>
	//  --------------------------------  ============  ------------------------------------------------------------
	//  COLOR_SCROLLBAR                   0             Scroll bar color
	//  COLOR_BACKGROUND                  1             Desktop color
	//  COLOR_ACTIVECAPTION               2             Color of the title bar for the active window, Specifies the left side color in the color gradient of an active window's title bar if the gradient effect is enabled.
	//  COLOR_INACTIVECAPTION             3             Color of the title bar for the inactive window, Specifies the left side color in the color gradient of an inactive window's title bar if the gradient effect is enabled.
	//  COLOR_MENU                        4             Menu background color
	//  COLOR_WINDOW                      5             Window background color
	//  COLOR_WINDOWFRAME                 6             Window frame color
	//  COLOR_MENUTEXT                    7             Color of text on menus
	//  COLOR_WINDOWTEXT                  8             Color of text in windows
	//  COLOR_CAPTIONTEXT                 9             Color of text in caption, size box, and scroll arrow
	//  COLOR_ACTIVEBORDER                10            Border color of active window
	//  COLOR_INACTIVEBORDER              11            Border color of inactive window
	//  COLOR_APPWORKSPACE                12            Background color of multiple-document interface (MDI) applications
	//  COLOR_HIGHLIGHT                   13            Background color of items selected in a control
	//  COLOR_HIGHLIGHTTEXT               14            Text color of items selected in a control
	//  COLOR_BTNFACE                     15            Face color for three-dimensional display elements and for dialog box backgrounds.
	//  COLOR_BTNSHADOW                   16            Color of shading on the edge of command buttons
	//  COLOR_GRAYTEXT                    17            Grayed (disabled) text
	//  COLOR_BTNTEXT                     18            Text color on push buttons
	//  COLOR_INACTIVECAPTIONTEXT         19            Color of text in an inactive caption
	//  COLOR_BTNHIGHLIGHT                20            Highlight color for 3-D display elements
	//  COLOR_3DDKSHADOW                  21            Darkest shadow color for 3-D display elements
	//  COLOR_3DLIGHT                     22            Second lightest 3-D color after 3DHighlight, Light color for three-dimensional display elements (for edges facing the light source.)
	//  COLOR_INFOTEXT                    23            Color of text in ToolTips
	//  COLOR_INFOBK                      24            Background color of ToolTips
	//  COLOR_HOTLIGHT                    26            Color for a hot-tracked item. Single clicking a hot-tracked item executes the item.
	//  COLOR_GRADIENTACTIVECAPTION       27            Right side color in the color gradient of an active window's title bar. COLOR_ACTIVECAPTION specifies the left side color.
	//  COLOR_GRADIENTINACTIVECAPTION     28            Right side color in the color gradient of an inactive window's title bar. COLOR_INACTIVECAPTION specifies the left side color.
	//  XPCOLOR_TOOLBAR_FACE              30            XP toolbar background color.
	//  XPCOLOR_HIGHLIGHT                 31            XP menu item selected color.
	//  XPCOLOR_HIGHLIGHT_BORDER          32            XP menu item selected border color.
	//  XPCOLOR_HIGHLIGHT_PUSHED          33            XP menu item pushed color.
	//  XPCOLOR_HIGHLIGHT_CHECKED         36            XP menu item checked color.
	//  XPCOLOR_HIGHLIGHT_CHECKED_BORDER  37            An RGB value that represents the XP menu item checked border color.
	//  XPCOLOR_ICONSHADDOW               34            XP menu item icon shadow.
	//  XPCOLOR_GRAYTEXT                  35            XP menu item disabled text color.
	//  XPCOLOR_TOOLBAR_GRIPPER           38            XP toolbar gripper color.
	//  XPCOLOR_SEPARATOR                 39            XP toolbar separator color.
	//  XPCOLOR_DISABLED                  40            XP menu icon disabled color.
	//  XPCOLOR_MENUBAR_FACE              41            XP menu item text background color.
	//  XPCOLOR_MENUBAR_EXPANDED          42            XP hidden menu commands background color.
	//  XPCOLOR_MENUBAR_BORDER            43            XP menu border color.
	//  XPCOLOR_MENUBAR_TEXT              44            XP menu item text color.
	//  XPCOLOR_HIGHLIGHT_TEXT            45            XP menu item selected text color.
	//  XPCOLOR_TOOLBAR_TEXT              46            XP toolbar text color.
	//  XPCOLOR_PUSHED_TEXT               47            XP toolbar pushed text color.
	//  XPCOLOR_TAB_INACTIVE_BACK         48            XP inactive tab background color.
	//  XPCOLOR_TAB_INACTIVE_TEXT         49            XP inactive tab text color.
	//  XPCOLOR_HIGHLIGHT_PUSHED_BORDER   50            An RGB value that represents the XP border color for pushed in 3D elements.
	//  XPCOLOR_CHECKED_TEXT              45            XP color for text displayed in a checked button.
	//  XPCOLOR_3DFACE                    51            XP face color for three- dimensional display elements and for dialog box backgrounds.
	//  XPCOLOR_3DSHADOW                  52            XP shadow color for three-dimensional display elements (for edges facing away from the light source).
	//  XPCOLOR_EDITCTRLBORDER            53            XP color for the border color of edit controls.
	//  XPCOLOR_FRAME                     54            Office 2003 frame color.
	//  XPCOLOR_SPLITTER_FACE             55            XP splitter face color.
	//  XPCOLOR_LABEL                     56            Color for label control (xtpControlLabel)
	//  XPCOLOR_STATICFRAME               57            WinXP Static frame color
	//</TABLE>
	// Returns:
	//     The red, green, blue (RGB) color value of the given element.
	//-----------------------------------------------------------------------
	COLORREF GetColor(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to blend the light and dark colors
	//     by the amount specified in nDelta.
	// Parameters:
	//     clrLight - RGB light color
	//     clrDark  - RGB dark color
	//     nDelta   - Amount saturation to blend colors.
	// Returns:
	//     An RGB color value representing the two blended colors.
	//-----------------------------------------------------------------------
	COLORREF LightColor(COLORREF clrLight, COLORREF clrDark, int nDelta);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to disable the luna (Blue, Olive
	//     and Green) colors for Office 2003 theme for Windows XP and
	//     later operating systems.
	// Parameters:
	//     bDisable - TRUE to disable luna colors, otherwise FALSE.
	//-----------------------------------------------------------------------
	void DisableLunaColors(BOOL bDisable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if Luna colors for Office 2003 style is disabled.
	// Returns:
	//     TRUE if the luna colors are disabled, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsLunaColorsDisabled();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the color for the specified index.
	//     See GetColor for a complete list of index values.
	// Parameters:
	//     nIndex - Color index.
	//     clrValue - RGB color value to set.
	// See Also: SetColors
	//-----------------------------------------------------------------------
	void SetColor(int nIndex, COLORREF clrValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the colors for each element to the
	//     color in the specified color array.
	// Parameters:
	//     cElements    - Number of elements in array.
	//     lpaElements  - Array of elements.
	//     lpaRgbValues - Array of RGB values.
	// See Also: SetColor
	//-----------------------------------------------------------------------
	void SetColors(int cElements, const int* lpaElements, const COLORREF* lpaRgbValues);

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
	// See Also: SetLunaTheme
	//-----------------------------------------------------------------------
	XTPCurrentSystemTheme GetCurrentSystemTheme();

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
	XTPCurrentSystemTheme GetWinThemeWrapperTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to force system theme.
	// Parameters:
	//     systemTheme - theme to use. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     theme can be one of the following:
	//     * <b>xtpSystemThemeUnknown</b> Disables <i>luna</i> theme support.
	//     * <b>xtpSystemThemeBlue</b> Enables blue theme
	//     * <b>xtpSystemThemeOlive</b> Enables olive theme
	//     * <b>xtpSystemThemeSilver</b> Enables silver theme
	//-----------------------------------------------------------------------
	void SetLunaTheme(XTPCurrentSystemTheme systemTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method replace standard GetSysColor to application defined.
	// Parameters:
	//     pfnGetSysColor - Pointer to the PFNGETSYSCOLOR procedure
	//-----------------------------------------------------------------------
	void SetGetSysColorPtr(PFNGETSYSCOLOR pfnGetSysColor);

private:
	CXTPColorManager();
	void RefreshSysColors();
	void RefreshXPColors();
	void RefreshGradientColors();
	float ColorWidth(int nLength, int nWidth);
	float ColorDelta(COLORREF clrA, COLORREF clrB);
	float Length(COLORREF clrA, COLORREF clrB);
	BOOL LongColor(COLORREF clrMain, COLORREF clrSub, BOOL bCalcLength, float fDistance);
	COLORREF MixColor(COLORREF clrMain, COLORREF clrSub, float fDistance);
	COLORREF AdjustColor(COLORREF clrMain, COLORREF clrSub, float fDistance);
	double GetRDelta(COLORREF clr);
	double GetGDelta(COLORREF clr);
	double GetBDelta(COLORREF clr);

	// singleton, instantiate on demand.
	static CXTPColorManager& AFX_CDECL Instance();

public:
	CXTPPaintManagerColorGradient grcCaption;               // Caption gradient color.
	CXTPPaintManagerColorGradient grcDockBar;               // Dockbar gradient color.
	CXTPPaintManagerColorGradient grcShortcutBarGripper;    // Shortcut bar gripper.
	CXTPPaintManagerColorGradient grcToolBar;               // ToolBar gradient color.

private:
	BOOL m_bInit;
	BOOL m_bDisableLunaColors;
	COLORREF m_arrStandardColor[XPCOLOR_LAST + 1];
	COLORREF m_arrCustomColor[XPCOLOR_LAST + 1];
	PFNGETSYSCOLOR m_pfnGetSysColor;
	XTPCurrentSystemTheme m_systemTheme;


	friend CXTPColorManager* XTPColorManager();
};


//---------------------------------------------------------------------------
// Summary:
//     Call this member function to access CXTPColorManager members.
//     Since this class is designed as a single instance object you can
//     only access version info thru this method. You <b>cannot</b>
//     directly instantiate an object of type CXTPColorManager.
// Example:
// <code>
// COLORREF cr = XTPColorManager()->GetColor(nIndex);
// </code>
//---------------------------------------------------------------------------
CXTPColorManager* XTPColorManager();

//---------------------------------------------------------------------------
// Summary:
//     Call this member as a convenient way to call
//     XTPColorManager()->GetColor, see CXTPColorManager::GetColor for
//     more details.
// Parameters:
//     nIndex - Specifies the display element whose color is to be
//              retrieved.
// Returns:
//     The red, green, blue (RGB) color value of the given element.
// Example:
// <code>
// if ( eState == xtMouseNormal )
// {
//     pDC->Draw3dRect( rcItem,
//         GetXtremeColor(COLOR_3DFACE),
//         GetXtremeColor(COLOR_3DFACE) );
// }
// </code>
// See Also:
//     CXTPColorManager::GetColor
//---------------------------------------------------------------------------
COLORREF GetXtremeColor(UINT nIndex);

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE COLORREF GetXtremeColor(UINT nIndex) {
	return XTPColorManager()->GetColor(nIndex);
}
AFX_INLINE CXTPColorManager* XTPColorManager() {
	return &CXTPColorManager::Instance();
}

#endif // !defined(__XTPCOLORMANAGER_H__)
