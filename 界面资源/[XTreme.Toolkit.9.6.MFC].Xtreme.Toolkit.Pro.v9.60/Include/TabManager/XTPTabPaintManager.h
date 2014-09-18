// XTPTabPaintManager.h: interface for the CXTPTabPaintManager class.
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
#if !defined(__XTPTABPAINTMANAGER_H__)
#define __XTPTABPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPTabManager;
class CXTPTabManagerAtom;
class CXTPTabManagerItem;
class CXTPTabManagerNavigateButton;

#include "CommonPro.include/XTPColorManager.h"
#include "CommonPro.include/XTPWinThemeWrapper.h"


//-----------------------------------------------------------------------
// Summary:
//     Indicates that a transparent color should be used.
// Remarks:
//     If an objects COLORREF value is XTP_TABMANAGER_COLOR_NONE it
//     is used to indicate that the object should not be drawn.  This
//     is used in the tab appearance and color functions to indicate
//     when tab borders, highlighting, shadowing, etc should be drawn.
//
//      Setting a member of CXTPTabPaintManager::CColorSet to
//      XTP_TABMANAGER_COLOR_NONE will indicate that the member should
//      not be drawn.
// See Also: CXTPTabPaintManager::CColorSet
//-----------------------------------------------------------------------
const COLORREF XTP_TABMANAGER_COLOR_NONE = (COLORREF)(-1);

//-----------------------------------------------------------------------
// Summary:
//     Indicates a special "shaded" fill type.
// Remarks:
//     To get the "shaded" effect, the object will be filled with
//     one pixel COLOR_3DFACE, one pixel COLOR_3DHIGHLIGHT, one
//     pixel COLOR_3DFACE....
//
//           Setting the clrLight parameter to XTP_TABMANAGER_COLOR_SHADED
//           in the used in CXTPTabPaintManager::CColorSet::GradientFill
//           member will cause the object to be filled with the special shaded fill.
//
// NOTE:
//     This is used in xtpTabAppearanceStateButtons to colorize
//     the button face.  The special fill was used to color of the currently
//     selected tab.  The special fill can be applied to any
//     color member.
// See Also: CXTPTabPaintManager::CColorSet::GradientFill
//-----------------------------------------------------------------------
const COLORREF XTP_TABMANAGER_COLOR_SHADED = (COLORREF)(-2);


//-----------------------------------------------------------------------
// Summary:
//     XTPTabOneNoteColor is an enumeration used to indicate which OneNote
//     COLORREF value to retrieve.
// Remarks:
//     This enumerator is used by CXTPTabPaintManager::GetOneNoteColor to
//     retrieve a specified OneNote RGB color value.
// Example:
//      See WM_XTP_GETTABCOLOR for an example.
// See Also:
//     CXTPTabPaintManager::GetOneNoteColor, CXTPTabManager::GetItemColor,
//     CXTPDockingPane::GetItemColor
//
// <KEYWORDS xtpTabColorBlue, xtpTabColorYellow, xtpTabColorGreen, xtpTabColorRed, xtpTabColorPurple, xtpTabColorCyan, xtpTabColorOrange, xtpTabColorMagenta>
//-----------------------------------------------------------------------
enum XTPTabOneNoteColor
{
	xtpTabColorBlue    = 0x1000000, // Blue tab color used when OneNote colors enabled.
	xtpTabColorYellow  = 0x1000001, // Yellow tab color used when OneNote colors enabled.
	xtpTabColorGreen   = 0x1000002, // Green tab color used when OneNote colors enabled.
	xtpTabColorRed     = 0x1000003, // Red tab color used when OneNote colors enabled.
	xtpTabColorPurple  = 0x1000004, // Purple tab color used when OneNote colors enabled.
	xtpTabColorCyan    = 0x1000005, // Cyan tab color used when OneNote colors enabled.
	xtpTabColorOrange  = 0x1000006, // Orange tab color used when OneNote colors enabled.
	xtpTabColorMagenta = 0x1000007  // Magenta tab color used when OneNote colors enabled.
};

//-----------------------------------------------------------------------
// Summary:
//     XTPTabWorkspacePart is an enumeration used to indicate which
//     part of the MDI TabWorkspace needs to be redrawn.
// Remarks:
//     The enumerators are used by CXTPTabPaintManager::CAppearanceSet::DrawWorkspacePart
//     when redrawing the MDITabClient TabWorkspace when grouping is enabled.
// See Also: CXTPTabPaintManager::CAppearanceSet::DrawWorkspacePart
//
// <KEYWORDS xtpTabWorkspacePartBorder, xtpTabWorkspacePartWidth, xtpTabWorkspacePartVSplitter, xtpTabWorkspacePartHSplitter>
//-----------------------------------------------------------------------
enum XTPTabWorkspacePart
{
	xtpTabWorkspacePartBorder,      // Indicates that the border of the MDITabClient TabWorkspace should be drawn.
	xtpTabWorkspacePartWidth,       // Indicates that the border width of the MDITabClient TabWorkspace should be recalculated.  I.e. When adding or removing a static frame.
	xtpTabWorkspacePartVSplitter,   // Indicates that the vertical splitter of the MDITabClient TabWorkspace should be drawn.
	xtpTabWorkspacePartHSplitter    // Indicates that the horizontal splitter of the MDITabClient TabWorkspace should be drawn.
};


//-----------------------------------------------------------------------
// Summary:
//     XTPTabAppearanceStyle is an enumeration used to set the appearance of tabs.
// Example:
//     <code>m_wndTabControl.SetAppearance(xtpTabAppearancePropertyPage2003);</code>
// See Also: CXTPTabManager::SetAppearance, CXTPTabPaintManager::SetAppearance
//
// <KEYWORDS xtpTabAppearancePropertyPage, xtpTabAppearancePropertyPageSelected, xtpTabAppearancePropertyPageFlat, xtpTabAppearancePropertyPage2003, xtpTabAppearanceStateButtons, xtpTabAppearanceVisualStudio, xtpTabAppearanceFlat, xtpTabAppearanceExcel, xtpTabAppearanceVisio>
//-----------------------------------------------------------------------
enum XTPTabAppearanceStyle
{
	xtpTabAppearancePropertyPage,           // Gives your tabs an Office 2000 appearance.
	xtpTabAppearancePropertyPageSelected,   // Gives your tabs an Office 2000 selected appearance.
	xtpTabAppearancePropertyPageFlat,       // Gives your tabs an Office 2000 Flat appearance.
	xtpTabAppearancePropertyPage2003,       // Gives your tabs an Office 2003 appearance.
	xtpTabAppearanceStateButtons,           // Gives your tabs a State Button appearance.
	xtpTabAppearanceVisualStudio,           // Gives your tabs a Visual Studio appearance.
	xtpTabAppearanceFlat,                   // Gives your tabs Flat appearance.
	xtpTabAppearanceExcel,                  // Gives your tabs an Excel appearance.
	xtpTabAppearanceVisio                   // Gives your tabs a Visio appearance.
};

//-----------------------------------------------------------------------
// Summary:
//     XTPTabColorStyle is an enumeration used to set the color style of tabs.
// Example:
//     <code>m_wndTabControl.SetColor(xtpTabColorWhidbey);</code>
// See Also:
//     CXTPTabManager::SetColor, CXTPTabPaintManager::SetColor,
//     CXTPTabManager::GetColor, CXTPTabPaintManager::GetColor
//
// <KEYWORDS xtpTabColorDefault, xtpTabColorVisualStudio, xtpTabColorOffice2003, xtpTabColorWinXP, xtpTabColorWhidbey>
//-----------------------------------------------------------------------
enum XTPTabColorStyle
{
	xtpTabColorDefault = 1,         // Tabs will use the default color for the currently set Appearance.
	xtpTabColorVisualStudio = 2,    // Tabs will use the Visual Studio color style for the currently set Appearance.
	xtpTabColorOffice2003 = 4,      // Tabs will use the Office 2003 color style for the currently set Appearance.
	xtpTabColorWinXP = 8,           // Tabs will use the Windows XP color style for the currently set Appearance.
	xtpTabColorWhidbey = 16,        // Tabs will use the Visual Studio 2005 "Whidbey" color style for the currently set Appearance.
};

//-----------------------------------------------------------------------
// Summary:
//     XTPTabPosition is an enumeration used to set the position of tabs.
// Example:
//     <code>m_wndTabControl.SetPosition(xtpTabPositionBottom);</code>
// See Also: CXTPTabManager::SetPosition, CXTPTabManager::GetPosition
//
// <KEYWORDS xtpTabPositionTop, xtpTabPositionLeft, xtpTabPositionBottom, xtpTabPositionRight>
//-----------------------------------------------------------------------
enum XTPTabPosition
{
	xtpTabPositionTop,          // Tabs will be drawn on the Top.
	xtpTabPositionLeft,         // Tabs will be drawn on the Left.
	xtpTabPositionBottom,       // Tabs will be drawn on the Bottom.
	xtpTabPositionRight         // Tabs will be drawn on the Right.
};

//-----------------------------------------------------------------------
// Summary:
//     XTPTabLayoutStyle is an enumeration used to set the layout of tabs.
// Example:
//     <code>m_wndTabControl.SetLayoutStyle(xtpTabLayoutCompressed);</code>
// See Also: CXTPTabManager::SetLayoutStyle, CXTPTabManager::GetLayout
//
// <KEYWORDS xtpTabLayoutAutoSize, xtpTabLayoutSizeToFit, xtpTabLayoutFixed, xtpTabLayoutCompressed>
//-----------------------------------------------------------------------
enum XTPTabLayoutStyle
{
	xtpTabLayoutAutoSize,       // Tabs will be automatically sized based on the caption and image size.  With this flag set, tabs will appear in their normal size.
	xtpTabLayoutSizeToFit,      // Tabs are sized to fit within the tab panel.  All tabs will be compressed and forced to fit into the tab panel.
	xtpTabLayoutFixed,          // All tabs will be set to a fixed size within the tab panel.
	xtpTabLayoutCompressed      // Tabs will be compressed within the tab panel.  This will compress the size of the tabs, but all tabs will not be forced into the tab panel.
};

//-----------------------------------------------------------------------
// Summary:
//     XTPTabClientFrame is an enumeration used to set the client frame style drawn around the client of tabs.
// Example:
//     <code>m_wndTabControl.GetPaintManager()->m_clientFrame = xtpTabFrameBorder;</code>
// See Also: CXTPTabManager, CXTPTabPaintManager
//
// <KEYWORDS xtpTabFrameBorder, xtpTabFrameSingleLine, xtpTabFrameNone>
//-----------------------------------------------------------------------
enum XTPTabClientFrame
{
	xtpTabFrameBorder,      // With this flag set, a border will appear around the client area.
	xtpTabFrameSingleLine,  // With this flag set, only a single line is used as a border around the client area.
	xtpTabFrameNone         // With this flag set, no border will appear around the client area.
};

//-----------------------------------------------------------------------
// Summary:
//     XTPTabNavigateButton is an enumeration used to identify navigation
//     buttons within a tab control.
// Remarks:
//     This enumerator is used by the CXTPTabManagerNavigateButton class to
//     identify navigation buttons.
// Example:
//      See XTPTabNavigateButtonFlags for an example.
// See Also:
//     XTPTabNavigateButtonFlags, CXTPTabManager, CXTPTabManagerNavigateButton
//
// <KEYWORDS xtpTabNavigateButtonLeft, xtpTabNavigateButtonRight, xtpTabNavigateButtonClose>
//-----------------------------------------------------------------------
enum XTPTabNavigateButton
{
	xtpTabNavigateButtonLeft,   // Left tab navigation button.
	xtpTabNavigateButtonRight,  // Right tab navigation button.
	xtpTabNavigateButtonClose   // Close tab navigation button.
};


//-----------------------------------------------------------------------
// Summary:
//     XTPTabNavigateButtonFlags is an enumeration used to indicate
//     when a navigate button will be displayed.  This style is
//     applied to one button.  Navigate buttons include the left,
//     right, and close buttons that appear in the tab header area.
//
// Remarks:
//     If automatic is selected, then the button will appear only
//     when needed.  I.e. When the XTPTabLayoutStyle is set to
//     xtpTabLayoutAutoSize, all tab might not fit in the tab header
//     area.  When there are more tabs than can fit in the header, the
//     button will automatically be displayed.
//
// Example:
//     This example code illustrates how to specify when the tab navigation
//     buttons are displayed.
// <code>
// //Gets a reference to the tab manager
// CXTPTabManager* pManager = GetManager();
// if (pManager)
// {
//     //Finds the left navigation button and specifies that it is always displayed
//     pManager->FindNavigateButton(xtpTabNavigateButtonLeft)->SetFlags(xtpTabNavigateButtonAlways);
//     //Finds the right navigation button and specifies that it is never displayed
//     pManager->FindNavigateButton(xtpTabNavigateButtonRight)->SetFlags(xtpTabNavigateButtonNone);
//     //Finds the close navigation button and specifies that it is always displayed
//     pManager->FindNavigateButton(xtpTabNavigateButtonClose)->SetFlags(xtpTabNavigateButtonAlways);
// }
// //Called to recalculate tab area and reposition components
// pManager->Reposition();
// </code>
// See Also: CXTPTabManagerNavigateButton, CXTPTabManagerNavigateButton::SetFlags,
//           CXTPTabManagerNavigateButton::GetFlags
//
// <KEYWORDS xtpTabNavigateButtonNone, xtpTabNavigateButtonAutomatic, xtpTabNavigateButtonAlways>
//-----------------------------------------------------------------------
enum XTPTabNavigateButtonFlags
{
	xtpTabNavigateButtonNone,       // Never display the navigate button.
	xtpTabNavigateButtonAutomatic,  // Automatically display the navigate button.
	xtpTabNavigateButtonAlways,     // Always display the navigate button.
};


//-------------------------------------------------------------------------
// Summary:
//     CXTPTabPaintManager is a CCmdTarget derived class that is used to
//     store the color and appearance information of the tabs in the
//     tab manager.
//-------------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTabPaintManager : public CCmdTarget
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     CColorSet is a class that represents the colors of all the
	//     components (tab button, tab button border, etc) of the tab manager.
	// Remarks:
	//    CColorSet colorizes the different parts that make up a tab, tab header,
	//    tab client, and tab manager control.
	//
	//    Colors should be updated in the RefreshMetrics member's for each
	//    CColorSet object.
	//-------------------------------------------------------------------------
	class _XTP_EXT_CLASS CColorSet
	{
	public:

		//-----------------------------------------------------------------------
		// Summary:
		//     Colors used for Excel tab button colorization.
		// Remarks:
		//     Members of COLORSET_TAB_EXCEL are only used when the
		//     XTPTabAppearanceStyle is set to xtpTabAppearanceExcel.
		//
		//     Both the m_csExcelSelected and m_csExcelNormal color sets are
		//     used when the xtpTabAppearanceExcel appearance is applied.
		//
		//     By default, the "selected" excel style tab will display the
		//     clrLeftInnerBorder and clrTopInnerBorder in addition to the
		//     outer borders to indicate that the tab is selected.
		//
		//     The following color sets customize members of COLORSET_TAB_EXCEL
		//     in addition to the color members of CColorSet:
		//     * <b>CColorSetOffice2003</b>  Office 2003 Color Set.
		//     * <b>CColorSetDefault</b>     Default Color Set.
		//
		// See Also: m_csExcelSelected, m_csExcelNormal
		//-----------------------------------------------------------------------
		struct COLORSET_TAB_EXCEL
		{
			CXTPPaintManagerColor clrLeftInnerBorder;       // Color of the left inner border of the tab button.
			CXTPPaintManagerColor clrTopInnerBorder;        // Color of the top inner border of the tab button.
			CXTPPaintManagerColor clrRightInnerBorder;      // Color of the right inner border of the tab button.
			CXTPPaintManagerColor clrLeftOuterBorder;       // Color of the left outer border of the tab button.
			CXTPPaintManagerColor clrRightOuterBorder;      // Color of the right outer border of the tab button.
			CXTPPaintManagerColor clrTopOuterBorder;        // Color of the top outer border of the tab button.
		};

		//-----------------------------------------------------------------------
		// Summary:
		//     Colors used for Office 2000 tab button colorization.
		// Remarks:
		//     The tab buttons and the tab client area cast a shadow. The shadow consists
		//     of a normal and dark color.  One or both of these colors can be used
		//     depending on the selected XTPTabAppearanceStyle.
		//
		//     Members of COLORSET_TAB_PROPERTYPAGE are only used when the
		//     XTPTabAppearanceStyle is set to xtpTabAppearancePropertyPage,
		//     xtpTabAppearancePropertyPageFlat, xtpTabAppearancePropertyPageSelected,
		//     and xtpTabAppearancePropertyPage2003.
		//
		//     The following color sets customize members of COLORSET_TAB_PROPERTYPAGE
		//     in addition to the color members of CColorSet:
		//     * <b>CColorSetOffice2003</b> Office 2003 Color Set.
		//     * <b>CColorSetWinXP</b>      Windows XP Color Set.
		//     * <b>CColorSetDefault</b>    Default Color Set.
		//
		// See Also: m_csPropertyPage
		//-----------------------------------------------------------------------
		struct COLORSET_TAB_PROPERTYPAGE
		{
			CXTPPaintManagerColor clrHighlight;      // Color of tab button on mouse over when hot tracking is enabled.
			CXTPPaintManagerColor clrShadow;         // Color of the tab button's shadow.
			CXTPPaintManagerColor clrDarkShadow;     // Color of the tab button's dark shadow.
		};

		//-----------------------------------------------------------------------
		// Summary:
		//     Colors used for Office 2003 tab button colorization.
		// Remarks:
		//     Members of COLORSET_TAB_PROPERTYPAGE2003 are only used when the
		//     XTPTabAppearanceStyle is set to xtpTabAppearancePropertyPage2003.
		//
		//     The following color sets customize members of COLORSET_TAB_PROPERTYPAGE2003
		//     in addition to the color members of CColorSet:
		//     * <b>CColorSetOffice2003</b> Office 2003 Color Set.
		//     * <b>CColorSetWhidbey</b>    Visual Studio 2005 "Whidbey" Color Set.
		//     * <b>CColorSetDefault</b>    Default Color Set.
		//
		// See Also: m_csPropertyPage2003
		//-----------------------------------------------------------------------
		struct COLORSET_TAB_PROPERTYPAGE2003
		{
			CXTPPaintManagerColor clrDarkShadow;            // Color of the tab buttons shadow.
			CXTPPaintManagerColor clrFrameBorder;           // Color of the XTPTabClientFrame.
			CXTPPaintManagerColor clrBorderLeftHighlight;   // Color of the left border of a tab button.
			CXTPPaintManagerColor clrBorderTopHighlight;    // Color of the top border of a tab button.
		};

		//-----------------------------------------------------------------------
		// Summary:
		//     Colors used for "State Button" tab button colorization.
		// Remarks:
		//
		//     Members of COLORSET_TAB_STATEBUTTON are only used when the
		//     XTPTabAppearanceStyle is set to xtpTabAppearanceStateButtons.
		//
		//     The following color sets customize members of COLORSET_TAB_STATEBUTTON
		//     in addition to the color members of CColorSet:
		//     * <b>CColorSetOffice2003</b> Office 2003 Color Set.
		//     * <b>CColorSetDefault</b>    Default Color Set.
		//
		// See Also: m_csStateButton
		//-----------------------------------------------------------------------
		struct COLORSET_TAB_STATEBUTTON
		{
			CXTPPaintManagerColorGradient clrFace;    // Color of the currently selected tab button.
			CXTPPaintManagerColorGradient clrBorder;  // Border color of the currently selected tab button.
		};

	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CColorSet object.
		//-------------------------------------------------------------------------
		CColorSet();

		//-------------------------------------------------------------------------
		// Summary:
		//     Destroys a CColorSet object, handles cleanup and deallocation.
		//-------------------------------------------------------------------------
		virtual ~CColorSet();

	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     This member is called to refresh the visual metrics of the tabs.
		// Remarks:
		//     All of the color members are refreshed when this is called.
		//     This member can be override this member to change the colors of
		//     the color members.
		//-------------------------------------------------------------------------
		virtual void RefreshMetrics();

	public:

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to change the color of text in the tab.
		// Parameters:
		//     pDC   - Pointer to a valid device context.
		//     pItem - Change text color of this tab.
		//-----------------------------------------------------------------------
		virtual void SetTextColor(CDC* pDC, CXTPTabManagerItem* pItem);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to fill the header of the TabClient of the tab manager.
		// Parameters:
		//     pDC         - Pointer to a valid device context.
		//     rc          - Bounding rectangle of the  tab header.
		//     pTabManager - Fill the header of this CXTPTabManager object.
		// Remarks:
		//     The TabClient header is the bounding rectangle with tab tab buttons.
		//     The bounding rectangle can be retrieved with the CXTPTabPaintManager::CAppearanceSet::GetHeaderRect,
		//     and CXTPTabManager::GetHeaderRect members.
		// See Also: CXTPTabPaintManager::CAppearanceSet::GetHeaderRect, CXTPTabManager::GetHeaderRect
		//-----------------------------------------------------------------------
		virtual void FillHeader(CDC* pDC, CRect rc, CXTPTabManager* pTabManager);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to fill the TabClient area of the tab manager.
		// Parameters:
		//     pDC         - Pointer to a valid device context.
		//     rc          - Bounding rectangle of the tab client area.
		//     pTabManager - Fill the tab client area of this CXTPTabManager object.
		// Remarks:
		//     The TabClient bounding rectangle can be retrieved with the
		//     CXTPTabPaintManager::CAppearanceSet::GetClientRect,
		//     and CXTPTabManager::GetClientRect members.
		// Returns:
		//     If a tab is selected then the color from CXTPTabManager::GetItemColor
		//     for the currently selected tab is returned,
		//     otherwise m_clrButtonSelected is returned.
		//-----------------------------------------------------------------------
		virtual COLORREF FillClient(CDC* pDC, CRect rc, CXTPTabManager* pTabManager);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to fill a tab button when the XTPTabAppearanceStyle is
		//     xtpTabAppearanceStateButtons.
		// Parameters:
		//     pDC    - Pointer to a valid device context.
		//     rcItem - Bounding rectangle of the tab button to fill.
		//     pItem  - Tab button to draw.
		// Remarks:
		//     This member is only used to fill buttons when the XTPTabAppearanceStyle is
		//     xtpTabAppearanceStateButtons, for all other buttons, FillPropertyButton is
		//     used.
		// See Also: FillPropertyButton
		//-----------------------------------------------------------------------
		virtual void FillStateButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to fill a tab button.
		// Parameters:
		//     pDC    - Pointer to a valid device context.
		//     rcItem - Bounding rectangle of the tab button to fill.
		//     pItem  - Tab button to draw.
		// Returns:
		//     COLORREF returned from CXTPTabManager::GetItemColor(pItem).
		// Remarks:
		//     This member is used to fill all tab buttons except when the XTPTabAppearanceStyle is
		//     xtpTabAppearanceStateButtons.  xtpTabAppearanceStateButtons require
		//     the FillStateButton member.
		// See Also: FillStateButton
		//-----------------------------------------------------------------------
		virtual COLORREF FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to protect client area to be drawn.
		// Parameters:
		//     pDC         - Pointer to a valid device context.
		//     rcClient    - Bounding rectangle of the TabClient area.  This will
		//                   be the clipping region for the device context.
		//     pTabManager - Pointer to the CXTPTabManager.
		//-----------------------------------------------------------------------
		virtual void SelectClipRgn(CDC* pDC, CRect rcClient, CXTPTabManager* pTabManager);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to fill the tab navigation buttons.
		// Parameters:
		//     pDC     - Pointer to a valid device context.
		//     pButton - Tab navigation button to fill.
		//     rc      - Bounding rectangle of the tab navigation button.
		// Remarks:
		//     This member must be overridden and takes care of filling the
		//     tab navigation buttons that are in the header of the TabClient.
		//     The XTPTabColorStyle CColorSet classes override this to perform
		//     actions such as painting the highlighting, pressed, and normal
		//     versions of the tab navigation buttons.
		//
		// See Also: CXTPTabManager::GetNavigateButton, CXTPTabManagerNavigateButton
		//-----------------------------------------------------------------------
		virtual void FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc) = 0;

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to get the color of a tab.
		// Parameters:
		//     pItem - Get color of this Tab.
		// Returns:
		//     COLORREF of pItem.
		// Remarks:
		//     This is used in many of the CColorSet "fill" functions.
		// See Also: CXTPTabManager::GetItemColor, XTPTabColorStyle, SetColor, GetColorSet, SetColorSet
		//-----------------------------------------------------------------------
		virtual COLORREF GetItemColor(CXTPTabManagerItem* pItem);

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to do a gradient fill of a rectangle
		//     with color that smoothly fades from top to bottom.
		//
		// Parameters:
		//     pDC      - Pointer to a valid device context.
		//     rc       - Rectangle to fill with gradient.
		//     clrLight - COLORREF to start gradient from.
		//     clrDark  - COLORREF to fill to.
		//     position - XTPTabPosition of the tab buttons.
		//
		//Remarks:
		//     The color will smoothly fade from clrLight to clrDark, from
		//     top to bottom.
		//
		//     If clrLight is XTP_TABMANAGER_COLOR_SHADED, then the
		//     rectangle will be filled with a special "shaded" fill
		//     type and clrDark will be ignored.  See description of
		//     XTP_TABMANAGER_COLOR_SHADED for more information on this
		//     fill type.
		//
		// See Also: XTP_TABMANAGER_COLOR_SHADED
		//-----------------------------------------------------------------------
		void GradientFill(CDC* pDC, CRect rc, COLORREF clrLight, COLORREF clrDark, XTPTabPosition position);

		//-----------------------------------------------------------------------
		// Summary:
		//     The member is called to determine if this ColorSet supports
		//     Windows XP Themes and if a Windows XP Theme is enabled.
		// Returns:
		//     Always returns FALSE in the base class implementation.
		// Remarks:
		//     This member is overridden in CXTPTabPaintManager::CColorSetWinXP.
		// See Also: CXTPTabPaintManager::CColorSetWinXP::IsAppThemed
		//-----------------------------------------------------------------------
		virtual BOOL IsAppThemed();

	public:

		CXTPPaintManagerColorGradient m_clrHeaderFace;      // Color set of the tab header background.
		CXTPPaintManagerColorGradient m_clrAutoHideFace;    // Color set of the auto-hide panel background.  This is the tab header for docking pane tabs when the pane is hidden.
		CXTPPaintManagerColor         m_clrFrameBorder;     // Color of border placed around the tab buttons and tab client.
		CXTPPaintManagerColor         m_clrControlFace;     // Color of the control foreground.  This is the area that the tab header and tab client are drawn on.
		CXTPPaintManagerColor         m_clrHighlight;       // Color of the tab on mouse over when hot tracking is enabled.
		CXTPPaintManagerColor         m_clrButtonSelected;  // Color of the currently selected tab.
		CXTPPaintManagerColor         m_clrButtonNormal;    // Color of normal tabs.  Normal tabs are tabs that are not selected or disabled.
		CXTPPaintManagerColor         m_clrNormalText;      // Color of text in a normal tab.  Normal tabs are tabs that are not selected or disabled.
		CXTPPaintManagerColor         m_clrInActiveText;    // Color of text for tabs in all non-active groups of tabs in MDITabClient.
		CXTPPaintManagerColor         m_clrSelectedText;    // Color of text in a "selected" tab button.  The "selected" tab button is the tab with focus.  I.e. The tab becomes active when it is clicked.
		CXTPPaintManagerColor         m_clrHighlightText;   // Color of text on mouse over when hot tracking is enabled.
		CXTPPaintManagerColor         m_clrDisabledText;    // Color of text when a tab is disabled.  A disabled tab cannot receive focus.
		COLORSET_TAB_STATEBUTTON      m_csStateButton;      // Color set of state button style tabs.
		COLORSET_TAB_PROPERTYPAGE     m_csPropertyPage;     // Color set of visual studio style tabs.
		COLORSET_TAB_PROPERTYPAGE2003 m_csPropertyPage2003; // Color set of Office 2003 style tabs.
		COLORSET_TAB_EXCEL            m_csExcelSelected;    // Color set of Selected (focused) Excel style tabs.
		COLORSET_TAB_EXCEL            m_csExcelNormal;      // Color set of Normal Excel style tabs.

	protected:
		CXTPTabPaintManager*          m_pPaintManager;      // Self Paint Manager pointer.

		friend class CXTPTabPaintManager;
	};

	//-------------------------------------------------------------------------
	// Summary:
	//     CColorSetDefault is a CColorSet derived class that represents the
	//     default tab color set.
	// Remarks:
	//     To use the default color set, SetColor is used to apply
	//     the xtpTabColorDefault XTPTabColorStyle.
	//
	//     The following appearances (XTPTabAppearanceStyle) support CColorSetDefault:
	//     * <b>xtpTabAppearancePropertyPage</b>         Office 2000 appearance.
	//     * <b>xtpTabAppearancePropertyPageSelected</b> Office 2000 selected appearance.
	//     * <b>xtpTabAppearancePropertyPageFlat</b>     Office 2000 Flat appearance.
	//     * <b>xtpTabAppearancePropertyPage2003</b>     Office 2003 appearance.
	//     * <b>xtpTabAppearanceStateButtons</b>         State Button appearance.
	//     * <b>xtpTabAppearanceVisualStudio</b>         Visual Studio appearance.
	//     * <b>xtpTabAppearanceFlat</b>                 Flat appearance.
	//     * <b>xtpTabAppearanceExcel</b>                Excel appearance.
	//     * <b>xtpTabAppearanceVisio</b>                Visio appearance.
	//
	// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
	//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
	//-------------------------------------------------------------------------
	class _XTP_EXT_CLASS CColorSetDefault : public CColorSet
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     This member is called to refresh the visual metrics of the tabs.
		// Remarks:
		//     All of the color members are refreshed when this is called.
		//     This member can be override this member to change the colors of
		//     the color members.
		//-------------------------------------------------------------------------
		virtual void RefreshMetrics();

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to fill the tab navigation buttons.
		// Parameters:
		//     pDC     - Pointer to a valid device context.
		//     pButton - Tab navigation button to fill.
		//     rc      - Bounding rectangle of the tab navigation button.
		// Remarks:
		//     This member takes care of filling the tab navigation buttons
		//     that are in the header of the TabClient.
		//     The XTPTabColorStyle CColorSet classes override this to perform
		//     actions such as painting the highlighting, pressed, and normal
		//     versions of the tab navigation buttons.
		//
		// See Also: CXTPTabManager::GetNavigateButton, CXTPTabManagerNavigateButton
		//-----------------------------------------------------------------------
		void FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc);
	};

	//-------------------------------------------------------------------------
	// Summary:
	//     CColorSetVisualStudio is a CColorSetDefault derived class that represents the
	//     Visual Studio tab color set.
	// Remarks:
	//     To use the Visual Studio color set, SetColor is used to apply
	//     the xtpTabColorVisualStudio XTPTabColorStyle.
	//
	//     The following appearances (XTPTabAppearanceStyle) support CColorSetVisualStudio:
	//     * <b>xtpTabAppearancePropertyPageFlat</b>   Office 2000 Flat appearance.
	//     * <b>xtpTabAppearanceStateButtons</b>       State Button appearance.
	//     * <b>xtpTabAppearanceVisualStudio</b>       Visual Studio appearance.
	//     * <b>xtpTabAppearanceFlat</b>               Flat appearance.
	//     * <b>xtpTabAppearanceExcel</b>              Excel appearance.
	//
	// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
	//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
	//-------------------------------------------------------------------------
	class _XTP_EXT_CLASS CColorSetVisualStudio : public CColorSetDefault
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     This member is called to refresh the visual metrics of the tabs.
		// Remarks:
		//     All of the color members are refreshed when this is called.
		//     This member can be override this member to change the colors of
		//     the color members.
		//-------------------------------------------------------------------------
		virtual void RefreshMetrics();

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to fill the tab navigation buttons.
		// Parameters:
		//     pDC     - Pointer to a valid device context.
		//     pButton - Tab navigation button to fill.
		//     rc      - Bounding rectangle of the tab navigation button.
		// Remarks:
		//     This member takes care of filling the tab navigation buttons
		//     that are in the header of the TabClient.
		//     The XTPTabColorStyle CColorSet classes override this to perform
		//     actions such as painting the highlighting, pressed, and normal
		//     versions of the tab navigation buttons.
		//
		// See Also: CXTPTabManager::GetNavigateButton, CXTPTabManagerNavigateButton
		//-----------------------------------------------------------------------
		void FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc);

	};

	//-------------------------------------------------------------------------
	// Summary:
	//     CColorSetOffice2003 is a CColorSetDefault derived class that represents the
	//     Office 2003 tab color set.
	// Remarks:
	//     To use the Office 2003 color set, SetColor is used to apply
	//     the xtpTabColorOffice2003 XTPTabColorStyle.
	//
	//     The following appearances (XTPTabAppearanceStyle) support CColorSetOffice2003:
	//     * <b>xtpTabAppearancePropertyPage</b>         Office 2000 appearance.
	//     * <b>xtpTabAppearancePropertyPageSelected</b> Office 2000 selected appearance.
	//     * <b>xtpTabAppearancePropertyPageFlat</b>     Office 2000 Flat appearance.
	//     * <b>xtpTabAppearancePropertyPage2003</b>     Office 2003 appearance.
	//     * <b>xtpTabAppearanceStateButtons</b>         State Button appearance.
	//     * <b>xtpTabAppearanceVisualStudio</b>         Visual Studio appearance.
	//     * <b>xtpTabAppearanceFlat</b>                 Flat appearance.
	//     * <b>xtpTabAppearanceExcel</b>                Excel appearance.
	//     * <b>xtpTabAppearanceVisio</b>                Visio appearance.
	//
	// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
	//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
	//-------------------------------------------------------------------------
	class _XTP_EXT_CLASS CColorSetOffice2003 : public CColorSetDefault
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     This member is called to refresh the visual metrics of the tabs.
		// Remarks:
		//     All of the color members are refreshed when this is called.
		//     This member can be override this member to change the colors of
		//     the color members.
		//     A different color set will be used for each luna color if used.
		//-------------------------------------------------------------------------
		virtual void RefreshMetrics();

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to fill a tab button.
		// Parameters:
		//     pDC    - Pointer to a valid device context.
		//     rcItem - Bounding rectangle of the tab button to fill.
		//     pItem  - Tab button to draw.
		// Returns:
		//     COLORREF returned from CXTPTabManager::GetItemColor(pItem).
		// Remarks:
		//     This member is used to fill all tab buttons except when the XTPTabAppearanceStyle is
		//     xtpTabAppearanceStateButtons.  xtpTabAppearanceStateButtons require
		//     the FillStateButton member.
		//
		//     Tab buttons will use CXTPTabPaintManager::CColorSet::GradientFill to
		//     fill the buttons.  The gradient will use the color of the tab item
		//     blended with RGB(255,255,255) to produce a "light color", the gradient will
		//     start with the "light color" and fill to the normal color of the tab item.
		//
		// See Also: FillStateButton
		//-----------------------------------------------------------------------
		virtual COLORREF FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to fill the tab navigation buttons.
		// Parameters:
		//     pDC     - Pointer to a valid device context.
		//     pButton - Tab navigation button to fill.
		//     rc      - Bounding rectangle of the tab navigation button.
		// Remarks:
		//     This member takes care of filling the tab navigation buttons
		//     that are in the header of the TabClient.
		//     The XTPTabColorStyle CColorSet classes override this to perform
		//     actions such as painting the highlighting, pressed, and normal
		//     versions of the tab navigation buttons.
		//
		// See Also: CXTPTabManager::GetNavigateButton, CXTPTabManagerNavigateButton, Rectangle
		//-----------------------------------------------------------------------
		virtual void FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member draws a rectangle with a border.
		// Parameters:
		//     pDC    - Pointer to a valid device context.
		//     rc     - Specifies the rectangle in logical units.
		//     nPen   - Specifies the color used to paint the rectangle.
		//     nBrush - Specifies the color used to fill the rectangle.
		// Remarks:
		//     This member draws a rectangle using the current pen. The interior
		//     of the rectangle is filled using the current brush.
		//
		//     This member is called by FillNavigateButton to fill the
		//     tab navigation buttons when luna colors are disabled.
		// See Also: FillNavigateButton
		//-----------------------------------------------------------------------
		void Rectangle(CDC* pDC, CRect rc, int nPen, int nBrush);

	protected:
		CXTPPaintManagerColor m_clrButtonHighlighted;       // Tab button color on mouse over when hot tracking is enabled.
		CXTPPaintManagerColor m_clrNavigateButtonBorder;    // Border color of tab navigation buttons on mouse over.
		BOOL     m_bLunaTheme;              // TRUE if the current system theme is xtpSystemThemeSilver, xtpSystemThemeOlive, or xtpSystemThemeOlive, FALSE otherwise.

	};

	//-------------------------------------------------------------------------
	// Summary:
	//     CColorSetWinXP is a CColorSetDefault derived class that represents the
	//     Windows XP tab color set.
	// Remarks:
	//     To use the Windows XP color set, SetColor is used to apply
	//     the xtpTabColorWinXP XTPTabColorStyle.
	//
	//     The following appearances (XTPTabAppearanceStyle) support CColorSetWinXP:
	//     * <b>xtpTabAppearancePropertyPage</b>         Office 2000 appearance.
	//     * <b>xtpTabAppearancePropertyPageSelected</b> Office 2000 selected appearance.
	//     * <b>xtpTabAppearanceStateButtons</b>         State Button appearance.
	//
	// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
	//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
	//-------------------------------------------------------------------------
	class _XTP_EXT_CLASS CColorSetWinXP : public CColorSetDefault
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     This member is called to refresh the visual metrics of the tabs.
		// Remarks:
		//     All of the color members are refreshed when this is called.
		//     This member can be override this member to change the colors of
		//     the color members.
		//-------------------------------------------------------------------------
		virtual void RefreshMetrics();

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to fill a tab button.
		// Parameters:
		//     pDC    - Pointer to a valid device context.
		//     rcItem - Bounding rectangle of the tab button to fill.
		//     pItem  - Tab button to draw.
		// Returns:
		//     XTP_TABMANAGER_COLOR_NONE
		// Remarks:
		//     This member is used to fill all tab buttons except when the XTPTabAppearanceStyle is
		//     xtpTabAppearanceStateButtons.  xtpTabAppearanceStateButtons require
		//     the FillStateButton member.
		//
		//          If IsAppThemed is FALSE, then CColorSetDefault::FillPropertyButton
		//          is used.
		// See Also: FillStateButton
		//-----------------------------------------------------------------------
		virtual COLORREF FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to fill the TabClient area of the tab manager.
		// Parameters:
		//     pDC         - Pointer to a valid device context.
		//     rc          - Bounding rectangle of the tab client area.
		//     pTabManager - Fill the tab client area of this CXTPTabManager object.
		// Remarks:
		//     The TabClient bounding rectangle can be retrieved with the
		//     CXTPTabPaintManager::CAppearanceSet::GetClientRect,
		//     and CXTPTabManager::GetClientRect members.
		// Returns:
		//     If IsAppThemed is TRUE, then 0 is returned.
		//
		//     If IsAppThemed is FALSE and a tab is selected then the color from CXTPTabManager::GetItemColor
		//     for the currently selected tab is returned, otherwise CXTPTabPaintManager::CColorSet::m_clrButtonSelected is returned.
		//-----------------------------------------------------------------------
		virtual COLORREF FillClient(CDC* pDC, CRect rc, CXTPTabManager* pTabManager);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to protect client area to be drawn.
		// Parameters:
		//     pDC         - Pointer to a valid device context.
		//     rcClient    - Bounding rectangle of the TabClient area.  This will
		//                   be the clipping region for the device context.
		//     pTabManager - Pointer to the CXTPTabManager.
		//-----------------------------------------------------------------------
		virtual void SelectClipRgn(CDC* pDC, CRect rcClient, CXTPTabManager* pTabManager);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to fill the tab navigation buttons.
		// Parameters:
		//     pDC     - Pointer to a valid device context.
		//     pButton - Tab navigation button to fill.
		//     rc      - Bounding rectangle of the tab navigation button.
		// Remarks:
		//     This member takes care of filling the tab navigation buttons
		//     that are in the header of the TabClient.
		//     The XTPTabColorStyle CColorSet classes override this to perform
		//     actions such as painting the highlighting, pressed, and normal
		//     versions of the tab navigation buttons.
		//
		//     If IsAppThemed is FALSE, then CColorSetDefault::FillNavigationButton
		//     is used.
		//
		// See Also: CXTPTabManager::GetNavigateButton, CXTPTabManagerNavigateButton
		//-----------------------------------------------------------------------
		virtual void FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to fill a tab button when the XTPTabAppearanceStyle is
		//     xtpTabAppearanceStateButtons.
		// Parameters:
		//     pDC    - Pointer to a valid device context.
		//     rcItem - Bounding rectangle of the tab button to fill.
		//     pItem  - Tab button to draw.
		// Remarks:
		//     This member is only used to fill buttons when the XTPTabAppearanceStyle is
		//     xtpTabAppearanceStateButtons, for all other buttons, FillPropertyButton is
		//     used.
		//
		//     If IsAppThemed is FALSE and/or the tab button is not selected, then
		//     CColorSetDefault::FillStateButton is used.
		//
		// See Also: FillPropertyButton
		//-----------------------------------------------------------------------
		virtual void FillStateButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem);

		//-----------------------------------------------------------------------
		// Summary:
		//     The member is called to determine if this ColorSet supports
		//     Windows XP Themes and if a Windows XP Theme is enabled.
		// Returns:
		//     TRUE if Windows XP themes are enabled and a Windows XP theme
		//     is currently used.
		// Remarks:
		//     IsAppThemed will return FALSE if running an operating system
		//     before Windows XP i.e. (98, 2000, NT) or if the Standard/Clasic
		//     themes are used with Windows XP.
		//
		//     This member is called internally to determine how to colorize
		//     the tab buttons.  If FALSE, CXTPTabPaintManager::CColorSetDefault
		//     is used to colorize the tab buttons.  If TRUE, then Windows XP
		//     colorization is used to colorize the tabs.
		// See Also: CXTPTabPaintManager::IsAppThemed
		//-----------------------------------------------------------------------
		virtual BOOL IsAppThemed();

	private:
		typedef void (AFX_CDECL* LPFNDRAWROTATEDBITS)(int cx, int cy, UINT* pSrcBits, UINT* pDestBits);

	private:
		void DrawRotatedButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem, BOOL bSwap, LPFNDRAWROTATEDBITS pfnRotatedProc);
		static void AFX_CDECL DrawRotatedBitsBottom(int cx, int cy, UINT* pSrcBits, UINT* pDestBits);
		static void AFX_CDECL DrawRotatedBitsRight(int cx, int cy, UINT* pSrcBits, UINT* pDestBits);
		static void AFX_CDECL DrawRotatedBitsLeft(int cx, int cy, UINT* pSrcBits, UINT* pDestBits);

	protected:
		CXTPWinThemeWrapper m_themeTabButton;   // Internally used helper to draw PropertyPage Buttons using Windows XP color set.
		CXTPWinThemeWrapper m_themeToolbar;     // Internally used helper to draw State Button using Windows XP color set.
	};

	//-------------------------------------------------------------------------
	// Summary:
	//     CColorSetWhidbey is a CColorSetOffice2003 derived class that represents the
	//     Visual Studio 2005 "Whidbey" tab color set.
	// Remarks:
	//     To use the Visual Studio 2005 "Whidbey" color set, SetColor is used to apply
	//     the xtpTabColorWhidbey XTPTabColorStyle.
	//
	//     The following appearances (XTPTabAppearanceStyle) support CColorSetWhidbey:
	//     * <b>xtpTabAppearancePropertyPage2003</b> Office 2003 appearance.
	//     * <b>xtpTabAppearanceStateButtons</b>     State Button appearance.
	//
	// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
	//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
	//-------------------------------------------------------------------------
	class _XTP_EXT_CLASS CColorSetWhidbey : public CColorSetOffice2003
	{
	public:
		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CColorSetWhidbey object.
		//-----------------------------------------------------------------------
		CColorSetWhidbey();

	public:

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to refresh the visual metrics of the tabs.
		// Remarks:
		//     All of the color members are refreshed when this is called.
		//     This member can be override this member to change the colors of
		//     the color members.
		//-----------------------------------------------------------------------
		void RefreshMetrics();

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to fill a tab button.
		// Parameters:
		//     pDC    - Pointer to a valid device context.
		//     rcItem - Bounding rectangle of the tab button to fill.
		//     pItem  - Tab button to draw.
		// Returns:
		//     COLORREF returned from CXTPTabManager::GetItemColor(pItem).
		// Remarks:
		//     This member is used to fill all tab buttons except when the XTPTabAppearanceStyle is
		//     xtpTabAppearanceStateButtons.  xtpTabAppearanceStateButtons require
		//     the FillStateButton member.
		//
		//     If m_bGradientButton is TRUE, then CColorSetOffice2003::FillPropertyButton
		//     is used to fill the tab button with a gradient fill. See
		//     CColorSetOffice2003::FillPropertyButton for details.
		//
		//     If m_bGradientButton is FALSE then CColorSet::FillPropertyButton is used.
		//
		// See Also: m_bGradientButton
		//-----------------------------------------------------------------------
		COLORREF FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem);

	protected:
		BOOL m_bGradientButton;         // TRUE if luna colors are enabled.  This will be TRUE when using Windows XP and the current system theme is xtpSystemThemeSilver, xtpSystemThemeOlive, or xtpSystemThemeOlive, FALSE otherwise.
	};


	//////////////////////////////////////////////////////////////////////////
	/// CAppearanceSet

	//-------------------------------------------------------------------------
	// Summary:
	//     CAppearanceSet is a class that represents the appearance of all the
	//     components (tab button, tab caption font, button size, tab margins, etc)
	//     of the tab manager.
	// Remarks:
	//    CAppearanceSet is the base class for all appearances.  To create a custom
	//    appearance, and new class can be derived from the base class or any of
	//    the appearances.
	//
	//    The appearance should be updated in the RefreshMetrics member's for each
	//    CAppearanceSet object.
	// See Also: XTPTabAppearanceStyle, SetAppearance, SetAppearanceSet, GetAppearance,
	//           GetAppearanceSet, SetColor, GetColor, GetColorSet, SetColorSet, CColorSet
	//-------------------------------------------------------------------------
	class _XTP_EXT_CLASS CAppearanceSet
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CAppearanceSet object.
		//-------------------------------------------------------------------------
		CAppearanceSet();

		//-------------------------------------------------------------------------
		// Summary:
		//     Destroys a CAppearanceSet object, handles cleanup and deallocation.
		//-------------------------------------------------------------------------
		virtual ~CAppearanceSet();

	public:

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to refresh the appearance metrics.
		// Remarks:
		//     This member is used to refresh the appearance metrics such
		//     as font, button height, margin size, etc.  Override this
		//     member in derived classes to change the appearance metrics.
		// See Also: CXTPTabPaintManager::SetFontIndirect, GetHeaderMargin, GetClientMargin, GetButtonLength, GetButtonHeight
		//-----------------------------------------------------------------------
		virtual void RefreshMetrics();

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to get the currently used color set.
		// Returns:
		//     Currently used custom color set.
		// See Also: XTPTabColorStyle, CXTPTabPaintManager::GetColor, CXTPTabPaintManager::SetColorSet, CXTPTabPaintManager::SetColor
		//-----------------------------------------------------------------------
		CColorSet* GetColorSet()
		{
			return m_pPaintManager->m_pColorSet;
		}

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to get the bounding rectangle to the tab
		//     header area.
		// Parameters:
		//     rcControl   - Bounding rectangle of the tab header.
		//                   See CXTPTabManager::GetControlRect.
		//     pTabManager - Pointer to tab manager.
		// Returns:
		//     Bounding rectangle of tab header area.
		// See Also: GetClientRect, GetHeaderMargin, GetClientMargin, CXTPTabManager::GetControlRect
		//-----------------------------------------------------------------------
		virtual CRect GetHeaderRect(CRect rcControl, CXTPTabManager* pTabManager);

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to get the bounding rectangle to the tab
		//     client area.
		// Parameters:
		//     rcControl   - Bounding rectangle of the tab manager control.
		//                   See CXTPTabManager::GetControlRect.
		//     pTabManager - Pointer to tab manager.
		// Returns:
		//     Bounding rectangle of tab client area.
		// See Also: GetHeaderRect, GetHeaderMargin, GetClientMargin, CXTPTabManager::GetControlRect
		//-----------------------------------------------------------------------
		virtual CRect GetClientRect(CRect rcControl, CXTPTabManager* pTabManager);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to get the top, left, right, and bottom
		//     tab header margins.
		// Returns:
		//     CRect struct containing the left, top, right, and bottom margins
		//     of the tab header.  This is the margin (space) placed around the
		//     tab buttons inside the tab header.
		//
		// Remarks:
		//     The CRect structure will contain the margins of the tab header.
		//     m_rcHeaderMargin contains these margins.
		//
		//     This is the margin placed around the tab buttons in the tab header.
		//     This is not placed around each tab button, but all the tab buttons.
		//
		// Example:
		// <code>
		// // Set the top, left, right, and bottom tab header margins to 5 pixels.  This will
		// // place a margin of 5 pixels around the tab buttons in the tab header.
		// m_wndTabControl.GetPaintManager()->GetAppearanceSet()->m_rcHeaderMargin.SetRect(5,5,5,5);
		//
		// // Set the top, left, right, and bottom tab control margins to 4 pixels.  This will
		// // place a margin of 4 pixels around the entire tab control (Includes tab header and tab client area).
		// m_wndTabControl.GetPaintManager()->m_rcControlMargin.SetRect(4,4,4,4);
		//
		// // Set the top, left, right, and bottom tab button margins to 2 pixels.  This will
		// // place a margin of 2 pixels around the text and icon in the tab buttons.
		// m_wndTabControl.GetPaintManager()->m_rcButtonMargin.SetRect(2,2,2,2);
		//
		// // Set the top, left, right, and bottom tab client margins to 10 pixels.  This will
		// // place a margin of 10 pixels around the tab client area.
		// m_wndTabControl.GetPaintManager()->m_rcClientMargin.SetRect(10,10,10,10);
		// </code>
		// See Also: m_rcHeaderMargin, GetHeaderRect
		//-----------------------------------------------------------------------
		virtual CRect GetHeaderMargin();

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to get the top, left, right, and bottom
		//     tab client area margins.
		// Returns:
		//     CRect struct containing the left, top, right, and bottom margins
		//     of the tab client area.  This is the margin (space) placed around the
		//     tab client area inside the tab control.
		//
		// Remarks:
		//     The CRect structure will contain the margins of the tab client area.
		//     CXTPTabPaintManager::m_rcClientMargin contains these margins.
		//
		//     This is the margin placed around the tab client area in the tab control.
		//
		// Example:
		// <code>
		// // Set the top, left, right, and bottom tab header margins to 5 pixels.  This will
		// // place a margin of 5 pixels around the tab buttons in the tab header.
		// m_wndTabControl.GetPaintManager()->GetAppearanceSet()->m_rcHeaderMargin.SetRect(5,5,5,5);
		//
		// // Set the top, left, right, and bottom tab control margins to 4 pixels.  This will
		// // place a margin of 4 pixels around the entire tab control (Includes tab header and tab client area).
		// m_wndTabControl.GetPaintManager()->m_rcControlMargin.SetRect(4,4,4,4);
		//
		// // Set the top, left, right, and bottom tab button margins to 2 pixels.  This will
		// // place a margin of 2 pixels around the text and icon in the tab buttons.
		// m_wndTabControl.GetPaintManager()->m_rcButtonMargin.SetRect(2,2,2,2);
		//
		// // Set the top, left, right, and bottom tab client margins to 10 pixels.  This will
		// // place a margin of 10 pixels around the tab client area.
		// m_wndTabControl.GetPaintManager()->m_rcClientMargin.SetRect(10,10,10,10);
		// </code>
		// See Also: CXTPTabPaintManager::m_rcClientMargin, GetClientRect
		//-----------------------------------------------------------------------
		virtual CRect GetClientMargin();

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to get the total button length including
		//     the left and right button margin.
		// Parameters:
		//     pDC   - Pointer to a valid device context.
		//     pItem - Pointer to tab button to get the length of.
		// Returns:
		//     Tab button length including the left and right button margin.
		// Remarks:
		//     This members calls CXTPTabPaintManager::DrawSingleButtonIconAndText
		//     with bDraw = FALSE to get the total button length including margins.
		// See Also: CXTPTabPaintManager::m_rcButtonMargin, GetClientMargin, GetHeaderMargin, CXTPTabPaintManager::DrawSingleButtonIconAndText, CXTPTabManager::GetItemMetrics, GetButtonHeight
		//-----------------------------------------------------------------------
		virtual int GetButtonLength(CDC* pDC, CXTPTabManagerItem* pItem);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to determine the total height of the tab buttons
		//     including the top and bottom margins and icon size.
		// Parameters:
		//     pTabManager - Pointer to the tab manager to get the button height.
		// Returns:
		//     The height of the tab buttons.
		// Remarks:
		//     This member add the size of the left and right margins and the
		//     icon size to the size of CXTPTabPaintManager.CAppearanceSet.m_nButtonHeight,
		//     which is the current height of the tabs without margins and icon size.
		// See Also: CXTPTabPaintManager.CAppearanceSet.m_nButtonHeight, CXTPTabPaintManager::m_rcButtonMargin, GetClientMargin, GetHeaderMargin, CXTPTabPaintManager::DrawSingleButtonIconAndText, CXTPTabManager::GetItemMetrics, GetButtonLength
		//-----------------------------------------------------------------------
		virtual int GetButtonHeight(CXTPTabManager* pTabManager);

	public:

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to draw the tab control.
		// Parameters:
		//     pTabManager - Pointer to the tab manager to draw.
		//     pDC         - Pointer to a valid device context.
		//     rcControl   - Bounding rectangle of the client area.
		// Remarks:
		//     This member is called in various Draw and Paint functions.
		//
		//     Unless overridden, this member will just call
		//     CXTPTabPaintManager::DrawTabControlEx.
		// See Also: CXTPTabPaintManager::DrawTabControlEx.
		//-----------------------------------------------------------------------
		virtual void DrawTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member fills in the tab header and tab client area.  It
		//     will then draw tab client frame.
		// Parameters:
		//     pTabManager - Pointer to tab manager to fill.
		//     pDC         - Pointer to a valid device context.
		//     rcControl   - Bounding rectangle of the tab manager control.
		//                   See CXTPTabManager::GetControlRect.
		// Returns:
		//     Tab header bounding rectangle.  This value is returned from GetHeaderRect.
		// Remarks:
		//     If CXTPTabPaintManager::m_bFillBackground is TRUE, then the tab client
		//     space is filled using CXTPTabPaintManager::CColorSet::m_clrControlFace color.
		//     CXTPTabPaintManager::m_bFillBackground is only FALSE when drawing
		//     DockingPanePanel AutoHide tabs because there is no client area to draw.
		// See Also: GetHeaderRect, CXTPTabPaintManager::m_bFillBackground, XTPTabClientFrame, CXTPTabPaintManager::m_clientFrame
		//-----------------------------------------------------------------------
		virtual CRect FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to move the tab buttons of the tab control to their
		//     correct positions.
		// Parameters:
		//     pTabManager - Pointer to the tab manager the tab button are on.
		//     pDC         - Pointer to a valid device context.
		//     rcClient    - Bounding rectangle of the tab client area.
		// Remarks:
		//     CXTPTabPaintManager::RepositionTabControl and CXTPTabPaintManager::AdjustClientRect
		//     are called when the window is resized.  When DrawTabControl is called
		//     the changes will be used when drawing the tab control.
		//
		//     This member is called by CXTPTabPaintManager::RepositionTabControl.
		//     Unless overridden this member will just call CXTPTabPaintManager::RepositionTabControlEx.
		//-----------------------------------------------------------------------
		virtual void RepositionTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to adjust the tab client rectangle.  It will
		//     return the new tab client rectangle with margins included.
		// Parameters:
		//     pTabManager - Pointer to the tab manager to adjust the tab client rectangle.
		//     rcClient    - [out] CRect to store tab client rectangle.
		// Remarks:
		//     CXTPTabPaintManager::AdjustClientRect and CXTPTabPaintManager::RepositionTabControl
		//     are called when the window is resized.  When CXTPTabPaintManager::DrawTabControl
		//     is called the changes will be used when drawing the tab control.
		//
		//     This member will adjust the tab client rectangle based on the
		//     currently set XTPTabClientFrame.
		//
		//     Then CXTPTabPaintManager::AdjustClientRect calls this member
		//     if there is one or more tab in the tab manager.
		// See Also:
		//     CXTPTabPaintManager::AdjustClientRect
		//-----------------------------------------------------------------------
		virtual void AdjustClientRect(CXTPTabManager* pTabManager, CRect& rcClient);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw a single tab button in the tab
		//     client header area.
		// Parameters:
		//     pDC   - Pointer to a valid device context.
		//     pItem - Tab button to draw.
		// Remarks:
		//     This method is overridden in derived classes and will draw a
		//     single tab button.  This method only draws the button, no text
		//     or icon is added.
		//
		//     DrawSingleButton calls CXTPTabPaintManager::DrawSingleButtonIconAndText
		//     after the button is drawn.  This is called to draw the button's
		//     icon and text.
		// See Also:: CXTPTabPaintManager::DrawTabControlEx, CXTPTabPaintManager::DrawSingleButtonIconAndText
		//-----------------------------------------------------------------------
		virtual void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to draw the different parts that make up
		//     the TabMDIClient TabWorkSpace.
		// Parameters:
		//     pDC    - Pointer to a valid device context.
		//     lpRect - Size of splitter.
		//     workspacePart  - XTPTabWorkspacePart to draw.
		// Remarks:
		//     This is used when drawing the TabWorkspace of MDI tabs when
		//     tab grouping is enabled.
		//
		//     lpRect is the size of the splitter that is placed between the
		//     vertical or horizontal tab groups if workspacePart is
		//     XTPTabWorkspacePartHSplitter or XTPTabWorkspacePartVSplitter.  And
		//     the splitter will be drawn with lpRect.
		//
		//     If workspacePart is XTPTabWorkspacePartWidth, then this is the size
		//     of the splitter.
		// See Also: XTPTabWorkspacePart
		//-----------------------------------------------------------------------
		virtual void DrawWorkspacePart(CDC* pDC, LPRECT lpRect, XTPTabWorkspacePart workspacePart);

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to retrieve all the supported XTPTabColorStyle
		//     for this appearance.
		// Returns:
		//     xtpTabColorDefault | xtpTabColorVisualStudio | xtpTabColorOffice2003 | xtpTabColorWinXP | xtpTabColorWhidbey
		// See Also: GetDefaultColorSet
		//-----------------------------------------------------------------------
		virtual int GetSupportedColorSets() { return xtpTabColorDefault | xtpTabColorVisualStudio | xtpTabColorOffice2003 | xtpTabColorWinXP | xtpTabColorWhidbey; }

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to get the default XTPTabColorStyle for this appearance.
		// Returns:
		//     xtpTabColorDefault
		// See Also: GetSupportedColorSets
		//-----------------------------------------------------------------------
		virtual XTPTabColorStyle GetDefaultColorSet() { return xtpTabColorDefault; }

	public:

//{{AFX_CODEJOCK_PRIVATE
		// Internal Draw helpers.
		COLORREF FillButton(CDC* pDC, CXTPTabManagerItem* pItem, LPPOINT pts, int nCount, XTPTabPosition tabPosition, BOOL bAbsoulute = FALSE);
		void DrawPolyLine(CDC* pDC, COLORREF clr, LPPOINT pts, int nCount);
		void DrawSingleLineBorder(CDC* pDC, CRect rc, XTPTabPosition position, COLORREF clrTopLeft, COLORREF clrBottomRight);
		static void AFX_CDECL InflateRectEx(CRect& rc, CRect rcInflate, XTPTabPosition position);
		static void AFX_CDECL DeflateRectEx(CRect& rc, CRect rcDeflate, XTPTabPosition position);
//}}AFX_CODEJOCK_PRIVATE

	public:
		CRect m_rcHeaderMargin;                 // CRect struct containing the left, top, right, and bottom margins of the tab header.  This is the margin (space) placed around the tab buttons inside the tab header.

	protected:
		int m_nButtonHeight;                    // Current tab button height (without margins).
		CXTPTabPaintManager* m_pPaintManager;   // Self paint manager pointer.

		friend class CXTPTabPaintManager;

	};

	//-------------------------------------------------------------------------
	// Summary:
	//     CAppearanceSetPropertyPage is a CAppearanceSet derived class that represents the
	//     Office 2000 tab appearance.
	// Remarks:
	//     To use the Office 2000 appearance, SetAppearance is used to apply
	//     the xtpTabAppearancePropertyPage XTPTabAppearanceStyle.
	//
	//     CAppearanceSetPropertyPage supports the following XTPTabColorStyle:
	//     * <b>xtpTabColorDefault</b>    Default color style.
	//     * <b>xtpTabColorOffice2003</b> Office 2003 color style.
	//     * <b>xtpTabColorWinXP</b>      Windows XP color style.
	//
	// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
	//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
	//-------------------------------------------------------------------------
	class _XTP_EXT_CLASS CAppearanceSetPropertyPage : public CAppearanceSet
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CAppearanceSetPropertyPage object.
		//-------------------------------------------------------------------------
		CAppearanceSetPropertyPage()
		{
			m_rcHeaderMargin.SetRect(2, 3, 6, 0);
		}

		//-------------------------------------------------------------------------
		// Summary:
		//     This member is called to refresh the appearance metrics.
		// Remarks:
		//     This member is used to refresh the appearance metrics such
		//     as font, button height, margin size, etc.  Override this
		//     member to change the appearance metrics.
		// See Also:
		//     CXTPTabPaintManager::SetFontIndirect, CXTPTabPaintManager::CAppearanceSet::GetHeaderMargin,
		//     CXTPTabPaintManager::CAppearanceSet::GetHeaderMargin::GetClientMargin,
		//     CXTPTabPaintManager::CAppearanceSet::GetHeaderMargin::GetButtonLength,
		//     CXTPTabPaintManager::CAppearanceSet::GetHeaderMargin::GetButtonHeight
		//-------------------------------------------------------------------------
		virtual void RefreshMetrics();

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw a single tab button in the tab
		//     client header area.
		// Parameters:
		//     pDC   - Pointer to a valid device context.
		//     pItem - Tab button to draw.
		// Remarks:
		//     This method will draw a single tab button.  This method only
		//     draws the button, no text or icon is added.
		//
		//          DrawSingleButton calls CXTPTabPaintManager::DrawSingleButtonIconAndText
		//          after the button is drawn.  This is called to draw the button's
		//          icon and text.
		// See Also:: CXTPTabPaintManager::DrawTabControlEx, CXTPTabPaintManager::DrawSingleButtonIconAndText
		//-----------------------------------------------------------------------
		virtual void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member fills in the tab header and tab client area.  It
		//     will then draw tab client frame.
		// Parameters:
		//     pTabManager - Pointer to tab manager to fill.
		//     pDC         - Pointer to a valid device context.
		//     rcControl   - Bounding rectangle of the tab manager control.
		//                   See CXTPTabManager::GetControlRect.
		// Returns:
		//     Tab header bounding rectangle.  This value is returned from GetHeaderRect.
		// Remarks:
		//     If CXTPTabPaintManager::m_bFillBackground is TRUE, then the tab client
		//     space is filled using CXTPTabPaintManager::CColorSet::m_clrControlFace color.
		//     CXTPTabPaintManager::m_bFillBackground is only FALSE when drawing
		//     DockingPanePanel AutoHide tabs because there is no client area to draw.
		// See Also: CXTPTabPaintManager::CAppearanceSet::GetHeaderRect, CXTPTabPaintManager::m_bFillBackground, XTPTabClientFrame, CXTPTabPaintManager::m_clientFrame
		//-----------------------------------------------------------------------
		virtual CRect FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to retrieve all the supported XTPTabColorStyle
		//     for this appearance.
		// Returns:
		//     xtpTabColorDefault | xtpTabColorOffice2003 | xtpTabColorWinXP
		// See Also: GetDefaultColorSet
		//-----------------------------------------------------------------------
		virtual int GetSupportedColorSets() { return xtpTabColorDefault | xtpTabColorOffice2003 | xtpTabColorWinXP; }

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to get the default XTPTabColorStyle for this appearance.
		// Returns:
		//     xtpTabColorDefault
		// See Also: GetSupportedColorSets
		//-----------------------------------------------------------------------
		virtual XTPTabColorStyle GetDefaultColorSet() { return xtpTabColorDefault; }

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to get the top, left, right, and bottom
		//     tab client area margins.
		// Returns:
		//     CRect struct containing the left, top, right, and bottom margins
		//     of the tab client area.  This is the margin (space) placed around the
		//     tab client area inside the tab control.
		//
		// Remarks:
		//     The CRect structure will contain the margins of the tab client area.
		//     CXTPTabPaintManager::m_rcClientMargin contains these margins.
		//
		//     This is the margin placed around the tab client area in the tab control.
		//
		// Example:
		// <code>
		// // Set the top, left, right, and bottom tab header margins to 5 pixels.  This will
		// // place a margin of 5 pixels around the tab buttons in the tab header.
		// m_wndTabControl.GetPaintManager()->GetAppearanceSet()->m_rcHeaderMargin.SetRect(5,5,5,5);
		//
		// // Set the top, left, right, and bottom tab control margins to 4 pixels.  This will
		// // place a margin of 4 pixels around the entire tab control (Includes tab header and tab client area).
		// m_wndTabControl.GetPaintManager()->m_rcControlMargin.SetRect(4,4,4,4);
		//
		// // Set the top, left, right, and bottom tab button margins to 2 pixels.  This will
		// // place a margin of 2 pixels around the text and icon in the tab buttons.
		// m_wndTabControl.GetPaintManager()->m_rcButtonMargin.SetRect(2,2,2,2);
		//
		// // Set the top, left, right, and bottom tab client margins to 10 pixels.  This will
		// // place a margin of 10 pixels around the tab client area.
		// m_wndTabControl.GetPaintManager()->m_rcClientMargin.SetRect(10,10,10,10);
		// </code>
		// See Also: CXTPTabPaintManager::m_rcClientMargin, CXTPTabPaintManager::CAppearanceSet::GetHeaderMargin,
		//-----------------------------------------------------------------------
		virtual CRect GetClientMargin();
	};

	//-------------------------------------------------------------------------
	// Summary:
	//     CAppearanceSetPropertyPageSelected is a CAppearanceSetPropertyPage derived
	//     class that represents the "Office 2000 Selected" tab appearance.
	// Remarks:
	//     To use the Office 2000 Selected appearance, SetAppearance is used to apply
	//     the xtpTabAppearancePropertyPageSelected XTPTabAppearanceStyle.
	//
	//     CAppearanceSetPropertyPageSelected supports the following XTPTabColorStyle:
	//     * <b>xtpTabColorDefault</b>    Default color style.
	//     * <b>xtpTabColorOffice2003</b> Office 2003 color style.
	//     * <b>xtpTabColorWinXP</b>      Windows XP color style.
	//
	// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
	//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
	//-------------------------------------------------------------------------
	class _XTP_EXT_CLASS CAppearanceSetPropertyPageSelected : public CAppearanceSetPropertyPage
	{
	public:

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw a single tab button in the tab
		//     client header area.
		// Parameters:
		//     pDC   - Pointer to a valid device context.
		//     pItem - Tab button to draw.
		// Remarks:
		//     This method will draw a single tab button.  This method only
		//     draws the button, no text or icon is added.
		//
		//     DrawSingleButton calls CXTPTabPaintManager::DrawSingleButtonIconAndText
		//     after the button is drawn.  This is called to draw the button's
		//     icon and text.
		//
		//     If the tab button is selected or highlighted then
		//     CXTPTabPaintManager::CAppearanceSetPropertyPageSelected::DrawSingleButton
		//     is called instead of CXTPTabPaintManager::DrawSingleButtonIconAndText.
		//
		// See Also:: CXTPTabPaintManager::DrawTabControlEx, CXTPTabPaintManager::DrawSingleButtonIconAndText
		//-----------------------------------------------------------------------
		virtual void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);
	};

	//-------------------------------------------------------------------------
	// Summary:
	//     CAppearanceSetPropertyPage2003 is a CAppearanceSet derived
	//     class that represents the Office 2003 tab appearance.
	// Remarks:
	//     To use the Office 2003 appearance, SetAppearance is used to apply
	//     the xtpTabAppearancePropertyPage2003 XTPTabAppearanceStyle.
	//
	//     CAppearanceSetPropertyPage2003 supports the following XTPTabColorStyle:
	//     * <b>xtpTabColorDefault</b>    Default color style.
	//     * <b>xtpTabColorOffice2003</b> Office 2003 color style.
	//     * <b>xtpTabColorWhidbey</b>    Visual Studio 2005 "Whidbey" color style.
	//
	//     By default m_bDoubleHighlightedBorder is FALSE.  If TRUE then a double
	//     border is drawn on mouse over when hot tracking is enabled.
	//
	// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
	//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
	//-------------------------------------------------------------------------
	class _XTP_EXT_CLASS CAppearanceSetPropertyPage2003 : public CAppearanceSet
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CAppearanceSetPropertyPage2003 object.
		//-------------------------------------------------------------------------
		CAppearanceSetPropertyPage2003()
		{
			m_rcHeaderMargin.SetRect(2, 3, 6, 0);
			m_bDoubleHighlightedBorder = FALSE;
		}

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to get the top, left, right, and bottom
		//     tab header margins.
		// Returns:
		//     CRect struct containing the left, top, right, and bottom margins
		//     of the tab header.  This is the margin (space) placed around the
		//     tab buttons inside the tab header.
		// Remarks:
		//     The CRect structure will contain the margins of the tab header.
		//     CXTPTabPaintManager::CAppearanceSet::m_rcHeaderMargin contains
		//     these margins.
		//
		// See Also: CXTPTabPaintManager::CAppearanceSet::m_rcHeaderMargin, CXTPTabPaintManager::GetHeaderRect
		//-----------------------------------------------------------------------
		virtual CRect GetHeaderMargin()
		{
			return CRect(m_rcHeaderMargin.left + m_nButtonHeight, m_rcHeaderMargin.top, m_rcHeaderMargin.right, m_rcHeaderMargin.bottom);
		}

		//-------------------------------------------------------------------------
		// Summary:
		//     This member is called to refresh the appearance metrics.
		// Remarks:
		//     This member is used to refresh the appearance metrics such
		//     as font, button height, margin size, etc.  Override this
		//     member to change the appearance metrics.
		// See Also: CXTPTabPaintManager::SetFontIndirect, GetHeaderMargin,
		//           CXTPTabPaintManager::CAppearanceSet::GetHeaderMargin::GetClientMargin,
		//           CXTPTabPaintManager::CAppearanceSet::GetHeaderMargin::GetButtonLength,
		//           CXTPTabPaintManager::CAppearanceSet::GetHeaderMargin::GetButtonHeight
		//-------------------------------------------------------------------------
		virtual void RefreshMetrics();

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw a single tab button in the tab
		//     client header area.
		// Parameters:
		//     pDC   - Pointer to a valid device context.
		//     pItem - Tab button to draw.
		// Remarks:
		//     This method will draw a single tab button.  This method only
		//     draws the button, no text or icon is added.
		//
		//     DrawSingleButton calls CXTPTabPaintManager::DrawSingleButtonIconAndText
		//     after the button is drawn.  This is called to draw the button's
		//     icon and text.
		//
		//     If m_bDoubleHighlightedBorder = TRUE, then a double border
		//     is drawn on mouse over when hot tracking is enabled.
		//
		// See Also:: CXTPTabPaintManager::DrawTabControlEx, CXTPTabPaintManager::DrawSingleButtonIconAndText, m_bDoubleHighlightedBorder
		//-----------------------------------------------------------------------
		virtual void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member fills in the tab header and tab client area.  It
		//     will then draw tab client frame.
		// Parameters:
		//     pTabManager - Pointer to tab manager to fill.
		//     pDC         - Pointer to a valid device context.
		//     rcControl   - Bounding rectangle of the tab manager control.
		//                   See CXTPTabManager::GetControlRect.
		// Returns:
		//     Tab header bounding rectangle.  This value is returned from GetHeaderRect.
		// Remarks:
		//     If CXTPTabPaintManager::m_bFillBackground is TRUE, then the tab client
		//     space is filled using CXTPTabPaintManager::CColorSet::m_clrControlFace color.
		//     CXTPTabPaintManager::m_bFillBackground is only FALSE when drawing
		//     DockingPanePanel AutoHide tabs because there is no client area to draw.
		// See Also: CXTPTabPaintManager::CAppearanceSet::GetHeaderRect, CXTPTabPaintManager::m_bFillBackground, XTPTabClientFrame, CXTPTabPaintManager::m_clientFrame
		//-----------------------------------------------------------------------
		virtual CRect FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to retrieve all the supported XTPTabColorStyle
		//     for this appearance.
		// Returns:
		//     xtpTabColorDefault | xtpTabColorOffice2003 | xtpTabColorWhidbey
		// See Also: GetDefaultColorSet
		//-----------------------------------------------------------------------
		virtual int GetSupportedColorSets() { return xtpTabColorDefault | xtpTabColorOffice2003 | xtpTabColorWhidbey; }

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to get the default XTPTabColorStyle for this appearance.
		// Returns:
		//     xtpTabColorOffice2003
		// See Also: GetSupportedColorSets
		//-----------------------------------------------------------------------
		virtual XTPTabColorStyle GetDefaultColorSet() { return xtpTabColorOffice2003; }

	public:

		BOOL m_bDoubleHighlightedBorder;    // TRUE to draw a double border on mouse over when hot tracking is enabled.

	};

	//-------------------------------------------------------------------------
	// Summary:
	//     CAppearanceSetStateButtons is a CAppearanceSet derived
	//     class that represents the "State Button" tab appearance.
	// Remarks:
	//     To use the "State Button" appearance, SetAppearance is used to apply
	//     the xtpTabAppearanceStateButtons XTPTabAppearanceStyle.
	//
	//     CAppearanceSetStateButtons supports the following XTPTabColorStyle:
	//     * <b>xtpTabColorDefault</b>      Default color style.
	//     * <b>xtpTabColorVisualStudio</b> Visual Studio color style.
	//     * <b>xtpTabColorOffice2003</b>   Office 2003 color style.
	//     * <b>xtpTabColorWinXP</b>        Windows XP color style.
	//     * <b>xtpTabColorWhidbey</b>      Visual Studio 2005 "Whidbey" color style.
	//
	// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
	//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
	//-------------------------------------------------------------------------
	class _XTP_EXT_CLASS CAppearanceSetStateButtons : public CAppearanceSet
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CAppearanceSetStateButtons object.
		//-------------------------------------------------------------------------
		CAppearanceSetStateButtons()
		{
			m_rcHeaderMargin.SetRect(2, 2, 4, 2);
		}

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw a single tab button in the tab
		//     client header area.
		// Parameters:
		//     pDC   - Pointer to a valid device context.
		//     pItem - Tab button to draw.
		// Remarks:
		//     This method will draw a single tab button.  This method only
		//     draws the button, no text or icon is added.
		//
		//     DrawSingleButton calls CXTPTabPaintManager::DrawSingleButtonIconAndText
		//     after the button is drawn.  This is called to draw the button's
		//     icon and text.
		// See Also:: CXTPTabPaintManager::DrawTabControlEx, CXTPTabPaintManager::DrawSingleButtonIconAndText
		//-----------------------------------------------------------------------
		virtual void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member fills in the tab header and tab client area.  It
		//     will then draw tab client frame.
		// Parameters:
		//     pTabManager - Pointer to tab manager to fill.
		//     pDC         - Pointer to a valid device context.
		//     rcControl   - Bounding rectangle of the tab manager control.
		//                   See CXTPTabManager::GetControlRect.
		// Returns:
		//     Tab header bounding rectangle.  This value is returned from GetHeaderRect.
		// Remarks:
		//     If CXTPTabPaintManager::m_bFillBackground is TRUE, then the tab client
		//     space is filled using CXTPTabPaintManager::CColorSet::m_clrControlFace color.
		//     CXTPTabPaintManager::m_bFillBackground is only FALSE when drawing
		//     DockingPanePanel AutoHide tabs because there is no client area to draw.
		// See Also: CXTPTabPaintManager::CAppearanceSet::GetHeaderRect, CXTPTabPaintManager::m_bFillBackground, XTPTabClientFrame, CXTPTabPaintManager::m_clientFrame
		//-----------------------------------------------------------------------
		virtual CRect FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to get the default XTPTabColorStyle for this appearance.
		// Returns:
		//     xtpTabColorVisualStudio
		// See Also: GetSupportedColorSets
		//-----------------------------------------------------------------------
		virtual XTPTabColorStyle GetDefaultColorSet() { return xtpTabColorVisualStudio; }

	};

	//-------------------------------------------------------------------------
	// Summary:
	//     CAppearanceSetVisualStudio is a CAppearanceSet derived
	//     class that represents the Visual Studio tab appearance.
	// Remarks:
	//     To use the Visual Studio appearance, SetAppearance is used to apply
	//     the xtpTabAppearanceVisualStudio XTPTabAppearanceStyle.
	//
	//     CAppearanceSetVisualStudio supports the following XTPTabColorStyle:
	//     * <b>xtpTabColorDefault</b>      Default color style.
	//     * <b>xtpTabColorVisualStudio</b> Visual Studio color style.
	//     * <b>xtpTabColorOffice2003</b>   Office 2003 color style.
	//     * <b>xtpTabColorWinXP</b>        Windows XP color style.
	//     * <b>xtpTabColorWhidbey</b>      Visual Studio 2005 "Whidbey" color style.
	//
	// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
	//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
	//-------------------------------------------------------------------------
	class _XTP_EXT_CLASS CAppearanceSetVisualStudio : public CAppearanceSet
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CAppearanceSetVisualStudio object.
		//-------------------------------------------------------------------------
		CAppearanceSetVisualStudio()
		{
			m_rcHeaderMargin.SetRect(4, 2, 4, 0);
		}

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw a single tab button in the tab
		//     client header area.
		// Parameters:
		//     pDC   - Pointer to a valid device context.
		//     pItem - Tab button to draw.
		// Remarks:
		//     This method will draw a single tab button.  This method only
		//     draws the button, no text or icon is added.
		//
		//     DrawSingleButton calls CXTPTabPaintManager::DrawSingleButtonIconAndText
		//     after the button is drawn.  This is called to draw the button's
		//     icon and text.
		// See Also:: CXTPTabPaintManager::DrawTabControlEx, CXTPTabPaintManager::DrawSingleButtonIconAndText
		//-----------------------------------------------------------------------
		virtual void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member fills in the tab header and tab client area.  It
		//     will then draw tab client frame.
		// Parameters:
		//     pTabManager - Pointer to tab manager to fill.
		//     pDC         - Pointer to a valid device context.
		//     rcControl   - Bounding rectangle of the tab manager control.
		//                   See CXTPTabManager::GetControlRect.
		// Returns:
		//     Tab header bounding rectangle.  This value is returned from GetHeaderRect.
		// Remarks:
		//     If CXTPTabPaintManager::m_bFillBackground is TRUE, then the tab client
		//     space is filled using CXTPTabPaintManager::CColorSet::m_clrControlFace color.
		//     CXTPTabPaintManager::m_bFillBackground is only FALSE when drawing
		//     DockingPanePanel AutoHide tabs because there is no client area to draw.
		// See Also: CXTPTabPaintManager::CAppearanceSet::GetHeaderRect, CXTPTabPaintManager::m_bFillBackground, XTPTabClientFrame, CXTPTabPaintManager::m_clientFrame
		//-----------------------------------------------------------------------
		virtual CRect FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to retrieve all the supported XTPTabColorStyle
		//     for this appearance.
		// Returns:
		//     xtpTabColorDefault |xtpTabColorVisualStudio | xtpTabColorOffice2003
		// See Also: GetDefaultColorSet
		//-----------------------------------------------------------------------
		virtual int GetSupportedColorSets() { return xtpTabColorDefault | xtpTabColorVisualStudio | xtpTabColorOffice2003; }

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to get the default XTPTabColorStyle for this appearance.
		// Returns:
		//     xtpTabColorVisualStudio
		// See Also: GetSupportedColorSets
		//-----------------------------------------------------------------------
		virtual XTPTabColorStyle GetDefaultColorSet() { return xtpTabColorVisualStudio; }
	};

	//-------------------------------------------------------------------------
	// Summary:
	//     CAppearanceSetFlat is a CAppearanceSet derived
	//     class that represents the "Flat" tab appearance.
	// Remarks:
	//     To use the Flat appearance, SetAppearance is used to apply
	//     the xtpTabAppearanceFlat XTPTabAppearanceStyle.
	//
	//     CAppearanceSetFlat supports the following XTPTabColorStyle:
	//     * <b>xtpTabColorDefault</b>      Default color style.
	//     * <b>xtpTabColorVisualStudio</b> Visual Studio color style.
	//     * <b>xtpTabColorOffice2003</b>   Office 2003 color style.
	//
	// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
	//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
	//-------------------------------------------------------------------------
	class _XTP_EXT_CLASS CAppearanceSetFlat : public CAppearanceSet
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CAppearanceSetFlat object.
		//-------------------------------------------------------------------------
		CAppearanceSetFlat()
		{
			m_rcHeaderMargin.SetRect(2, 2, 4, 0);
		}

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw a single tab button in the tab
		//     client header area.
		// Parameters:
		//     pDC   - Pointer to a valid device context.
		//     pItem - Tab button to draw.
		// Remarks:
		//     This method will draw a single tab button.  This method only
		//     draws the button, no text or icon is added.
		//
		//     DrawSingleButton calls CXTPTabPaintManager::DrawSingleButtonIconAndText
		//     after the button is drawn.  This is called to draw the button's
		//     icon and text.
		// See Also:: CXTPTabPaintManager::DrawTabControlEx, CXTPTabPaintManager::DrawSingleButtonIconAndText
		//-----------------------------------------------------------------------
		virtual void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member fills in the tab header and tab client area.  It
		//     will then draw tab client frame.
		// Parameters:
		//     pTabManager - Pointer to tab manager to fill.
		//     pDC         - Pointer to a valid device context.
		//     rcControl   - Bounding rectangle of the tab manager control.
		//                   See CXTPTabManager::GetControlRect.
		// Returns:
		//     Tab header bounding rectangle.  This value is returned from GetHeaderRect.
		// Remarks:
		//     If CXTPTabPaintManager::m_bFillBackground is TRUE, then the tab client
		//     space is filled using CXTPTabPaintManager::CColorSet::m_clrControlFace color.
		//     CXTPTabPaintManager::m_bFillBackground is only FALSE when drawing
		//     DockingPanePanel AutoHide tabs because there is no client area to draw.
		// See Also: CXTPTabPaintManager::CAppearanceSet::GetHeaderRect, CXTPTabPaintManager::m_bFillBackground, XTPTabClientFrame, CXTPTabPaintManager::m_clientFrame
		//-----------------------------------------------------------------------
		virtual CRect FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to retrieve all the supported XTPTabColorStyle
		//     for this appearance.
		// Returns:
		//     xtpTabColorDefault |xtpTabColorVisualStudio | xtpTabColorOffice2003
		// See Also: GetDefaultColorSet
		//-----------------------------------------------------------------------
		virtual int GetSupportedColorSets() { return xtpTabColorDefault |xtpTabColorVisualStudio | xtpTabColorOffice2003; }

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to get the default XTPTabColorStyle for this appearance.
		// Returns:
		//     xtpTabColorVisualStudio
		// See Also: GetSupportedColorSets
		//-----------------------------------------------------------------------
		virtual XTPTabColorStyle GetDefaultColorSet() { return xtpTabColorVisualStudio; }
	};

	//-------------------------------------------------------------------------
	// Summary:
	//     CAppearanceSetPropertyPageFlat is a CAppearanceSetFlat derived
	//     class that represents the Office 2000 Flat tab appearance.
	// Remarks:
	//     To use the Office 2000 Flat appearance, SetAppearance is used to apply
	//     the xtpTabAppearancePropertyPageFlat XTPTabAppearanceStyle.
	//
	//     CAppearanceSetPropertyPageSelected supports the following XTPTabColorStyle:
	//     * <b>xtpTabColorDefault</b>      Default color style.
	//     * <b>xtpTabColorVisualStudio</b> Visual Studio color style.
	//     * <b>xtpTabColorOffice2003</b>   Office 2003 color style.
	//
	// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
	//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
	//-------------------------------------------------------------------------
	class _XTP_EXT_CLASS CAppearanceSetPropertyPageFlat : public CAppearanceSetFlat
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CAppearanceSetPropertyPageFlat object.
		// Remarks:
		//     By default, m_bBlurPoints is TRUE.  This will smooth the edges
		//     of the tabs.
		//-------------------------------------------------------------------------
		CAppearanceSetPropertyPageFlat()
		{
			m_bBlurPoints = TRUE;
		}

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw a single tab button in the tab
		//     client header area.
		// Parameters:
		//     pDC   - Pointer to a valid device context.
		//     pItem - Tab button to draw.
		// Remarks:
		//     This method will draw a single tab button.  This method only
		//     draws the button, no text or icon is added.
		//
		//     DrawSingleButton calls CXTPTabPaintManager::DrawSingleButtonIconAndText
		//     after the button is drawn.  This is called to draw the button's
		//     icon and text.
		//
		//     If m_bBlurPoints is TRUE, then the corners of the tabs will be
		//     smoothed.
		//
		// See Also:: CXTPTabPaintManager::DrawTabControlEx, CXTPTabPaintManager::DrawSingleButtonIconAndText, m_bBlurPoints
		//-----------------------------------------------------------------------
		virtual void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to get the default XTPTabColorStyle for this appearance.
		// Returns:
		//     xtpTabColorDefault
		// See Also: GetSupportedColorSets
		//-----------------------------------------------------------------------
		virtual XTPTabColorStyle GetDefaultColorSet() { return xtpTabColorDefault; }

	public:

		BOOL m_bBlurPoints;         // TRUE to smooth corners of flat tabs.
	};

	//-------------------------------------------------------------------------
	// Summary:
	//     CAppearanceSetExcel is a CAppearanceSetFlat derived
	//     class that represents the Excel tab appearance.
	// Remarks:
	//     To use the Excel appearance, SetAppearance is used to apply
	//     the xtpTabAppearanceExcel XTPTabAppearanceStyle.
	//
	//     CAppearanceSetExcel supports the following XTPTabColorStyle:
	//     * <b>xtpTabColorDefault</b>      Default color style.
	//     * <b>xtpTabColorVisualStudio</b> Visual Studio color style.
	//     * <b>xtpTabColorOffice2003</b>   Office 2003 color style.
	//
	// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
	//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
	//-------------------------------------------------------------------------
	class _XTP_EXT_CLASS CAppearanceSetExcel : public CAppearanceSetFlat
	{
	public:

		//-------------------------------------------------------------------------
		// Summary:
		//     Constructs a CAppearanceSetExcel object.
		//-------------------------------------------------------------------------
		CAppearanceSetExcel()
		{
			m_rcHeaderMargin.SetRect(2, 3, 2, 0);
		}

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to get the top, left, right, and bottom
		//     tab header margins.
		// Returns:
		//     CRect struct containing the left, top, right, and bottom margins
		//     of the tab header.  This is the margin (space) placed around the
		//     tab buttons inside the tab header.
		// Remarks:
		//     The CRect structure will contain the margins of the tab header.
		//     CXTPTabPaintManager::CAppearanceSet::m_rcHeaderMargin contains
		//     these margins.
		//
		// See Also: CXTPTabPaintManager::CAppearanceSet::m_rcHeaderMargin, CXTPTabPaintManager::GetHeaderRect
		//-----------------------------------------------------------------------
		virtual CRect GetHeaderMargin()
		{
			return CRect(m_rcHeaderMargin.left + m_nButtonHeight / 2, m_rcHeaderMargin.top, m_rcHeaderMargin.right + m_nButtonHeight / 2, m_rcHeaderMargin.bottom);
		}

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw a single tab button in the tab
		//     client header area.
		// Parameters:
		//     pDC   - Pointer to a valid device context.
		//     pItem - Tab button to draw.
		// Remarks:
		//     This method will draw a single tab button.  This method only
		//     draws the button, no text or icon is added.
		//
		//     DrawSingleButton calls CXTPTabPaintManager::DrawSingleButtonIconAndText
		//     after the button is drawn.  This is called to draw the button's
		//     icon and text.
		// See Also:: CXTPTabPaintManager::DrawTabControlEx, CXTPTabPaintManager::DrawSingleButtonIconAndText
		//-----------------------------------------------------------------------
		virtual void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);
	};

	//-------------------------------------------------------------------------
	// Summary:
	//     CAppearanceSetVisio is a CAppearanceSet derived
	//     class that represents the Visio tab appearance.
	// Remarks:
	//     To use the Visio appearance, SetAppearance is used to apply
	//     the xtpTabAppearanceVisio XTPTabAppearanceStyle.
	//
	//     CAppearanceSetVisio supports the following XTPTabColorStyle:
	//     * <b>xtpTabColorDefault</b>     Default color style.
	//     * <b>xtpTabColorOffice2003</b>  Office 2003 color style.
	//
	// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
	//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
	//-------------------------------------------------------------------------
	class _XTP_EXT_CLASS CAppearanceSetVisio : public CAppearanceSet
	{
	public:

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to adjust the tab client rectangle.  It will
		//     return the new tab client rectangle with margins included.
		// Parameters:
		//     pTabManager - Pointer to the tab manager to adjust the tab client rectangle.
		//     rcClient    - [out] CRect to store tab client rectangle.
		// Remarks:
		//     CXTPTabPaintManager::AdjustClientRect and CXTPTabPaintManager::RepositionTabControl
		//     are called when the window is resized.  When CXTPTabPaintManager::DrawTabControl
		//     is called the changes will be used when drawing the tab control.
		//
		//     This member will adjust the tab client rectangle based on the
		//     currently set XTPTabClientFrame.
		//
		//     Then CXTPTabPaintManager::AdjustClientRect calls this member
		//     if there is one or more tab in the tab manager.
		// See Also:
		//     CXTPTabPaintManager::AdjustClientRect
		//-----------------------------------------------------------------------
		void AdjustClientRect(CXTPTabManager* pTabManager, CRect& rcClient);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to move the tab buttons of the tab control to their
		//     correct positions.
		// Parameters:
		//     pTabManager - Pointer to the tab manager the tab button are on.
		//     pDC         - Pointer to a valid device context.
		//     rcClient    - Bounding rectangle of the tab client area.
		// Remarks:
		//     CXTPTabPaintManager::RepositionTabControl and CXTPTabPaintManager::AdjustClientRect
		//     are called when the window is resized.  When DrawTabControl is called
		//     the changes will be used when drawing the tab control.
		//
		//     This member is called in CXTPTabPaintManager::RepositionTabControl.
		//     This member calls CXTPTabPaintManager::RepositionTabControlEx.
		//-----------------------------------------------------------------------
		void RepositionTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient);

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to get the bounding rectangle to the tab
		//     header area.
		// Parameters:
		//     rcControl   - Bounding rectangle of the tab header.
		//                   See CXTPTabManager::GetControlRect.
		//     pTabManager - Pointer to a CXTPTabManager.
		// Returns:
		//     Bounding rectangle of tab header area.
		// See Also: GetClientRect, CXTPTabManager::GetControlRect, CXTPTabManager::CAppearanceSet::GetHeaderMargin, CXTPTabManager::CAppearanceSet::GetClientMargin
		//-----------------------------------------------------------------------
		CRect GetHeaderRect(CRect rcControl, CXTPTabManager* pTabManager);

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to get the bounding rectangle to the tab
		//     client area.
		// Parameters:
		//     rcControl   - Bounding rectangle of the tab control.
		//     pTabManager - Pointer to a CXTPTabManager.
		// Returns:
		//     Bounding rectangle of tab client area.
		// See Also: GetClientRect, CXTPTabManager::GetControlRect
		//-----------------------------------------------------------------------
		CRect GetClientRect(CRect rcControl, CXTPTabManager* pTabManager);

		//-----------------------------------------------------------------------
		// Summary:
		//     This method is called to draw a single tab button in the tab
		//     client header area.
		// Parameters:
		//     pDC   - Pointer to a valid device context.
		//     pItem - Tab button to draw.
		// Remarks:
		//     This method will draw a single tab button.  This method only
		//     draws the button, no text or icon is added.
		//
		//     DrawSingleButton calls CXTPTabPaintManager::DrawSingleButtonIconAndText
		//     after the button is drawn.  This is called to draw the button's
		//     icon and text.
		// See Also:: CXTPTabPaintManager::DrawTabControlEx, CXTPTabPaintManager::DrawSingleButtonIconAndText
		//-----------------------------------------------------------------------
		void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);

		//-----------------------------------------------------------------------
		// Summary:
		//     This member is called to draw the tab control.
		// Parameters:
		//     pTabManager - Pointer to the tab manager to draw.
		//     pDC         - Pointer to a valid device context.
		//     rcControl   - Bounding rectangle of the client area.
		// Remarks:
		//     This member is called in various Draw and Paint functions.
		//
		//     This member will be called by CXTPTabPaintManager::DrawTabControl
		//     when the tab control needs to be drawn.
		//-----------------------------------------------------------------------
		void DrawTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

		//-----------------------------------------------------------------------
		// Summary:
		//     Call this member to retrieve all the supported XTPTabColorStyle
		//     for this appearance.
		// Returns:
		//     xtpTabColorDefault | xtpTabColorOffice2003
		// See Also: GetDefaultColorSet
		//-----------------------------------------------------------------------
		int GetSupportedColorSets() { return xtpTabColorDefault | xtpTabColorOffice2003; }
	};


public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabPaintManager object.
	//-------------------------------------------------------------------------
	CXTPTabPaintManager();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabPaintManager object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPTabPaintManager();


public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to apply one of the "built-in" color sets to
	//     the tabs.
	// Parameters:
	//     tabColor - XTPTabColorStyle to apply to the tabs.
	// Returns:
	//     Pointer to the CXTPTabPaintManager::CColorSet object applied.
	// See Also: XTPTabColorStyle, SetColorSet, GetColor, GetColorSet, XTPTabAppearanceStyle,
	//           SetAppearanceSet, SetAppearance, GetAppearance, GetAppearanceSet
	//-----------------------------------------------------------------------
	CColorSet* SetColor(XTPTabColorStyle tabColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to apply a custom color set to the tabs.
	// Parameters:
	//     pColorSet - Pointer to custom CXTPTabPaintManager::CColorSet
	//                 appearance set.
	// Returns:
	//     Pointer to the newly set custom color set.
	// See Also:
	//     XTPTabColorStyle, SetColor, GetColor, GetColorSet, XTPTabAppearanceStyle,
	//     SetAppearanceSet, SetAppearance, GetAppearance, GetAppearanceSet
	//-----------------------------------------------------------------------
	CColorSet* SetColorSet(CColorSet* pColorSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to apply one of the "built-in" appearances to
	//     the tabs.
	// Parameters:
	//     tabAppearance - XTPTabAppearanceStyle to apply to the tabs.
	// Returns:
	//     Pointer to the CXTPTabPaintManager::CAppearanceSet object applied.
	// Remarks:
	//     Use SetAppearance to apply a "Built-In" theme such as
	//     xtpTabAppearanceExcel.  To apply a custom them, use the
	//     SetAppearanceSet member.
	// See Also: XTPTabAppearanceStyle, SetAppearanceSet, GetAppearance, GetAppearanceSet,
	//           SetColor, GetColor, GetColorSet, SetColorSet
	//-----------------------------------------------------------------------
	CAppearanceSet* SetAppearance(XTPTabAppearanceStyle tabAppearance);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to apply a custom appearance set.
	// Parameters:
	//     pAppearanceSet - Pointer to custom CXTPTabPaintManager::CAppearanceSet
	//                      appearance set.
	// Remarks:
	//     An appearance set specifies how the tabs will look.  This
	//     is the actual physical structure of the tab buttons.  For example,
	//     tab buttons with an Excel appearance style will have a
	//     half hexagon shape.  SetColorSet is used to set custom
	//     colors.
	//
	//     See the GUI_Eclipse sample for an example of how to create
	//     a custom appearance set and apply it to the tabs.
	// Returns:
	//     Pointer to newly set custom appearance set.
	// See Also: XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
	//           SetColor, GetColor, GetColorSet, SetColorSet
	//-----------------------------------------------------------------------
	CAppearanceSet* SetAppearanceSet(CAppearanceSet* pAppearanceSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the currently set custom appearance set.
	// Returns:
	//     Pointer to the currently used custom appearance set.
	// Remarks:
	//     An appearance set specifies how the tabs will look.  This
	//     is the actual physical structure of the tab buttons.  For example,
	//     tab buttons with an Excel appearance style will have a
	//     half hexagon shape.  SetColorSet is used to set
	//     custom colors.
	//
	//     See the GUI_Eclipse sample for an example of how to create
	//     a custom appearance set and apply it to the tabs.
	// See Also: GetAppearanceSet, SetAppearanceSet, SetAppearance
	//-----------------------------------------------------------------------
	CAppearanceSet* GetAppearanceSet();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the currently used custom color set.
	// Returns:
	//     Currently used custom color set.
	// See Also: XTPTabColorStyle, GetColor, SetColorSet, SetColor
	//-----------------------------------------------------------------------
	CColorSet* GetColorSet();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine the currently set appearance style.
	// Returns:
	//     The currently set XTPTabAppearanceStyle.
	// Remarks:
	//     The appearance is the physical structure of the tab buttons.
	// See Also: XTPTabAppearanceStyle, GetAppearanceSet, SetAppearanceSet, SetAppearance
	//-----------------------------------------------------------------------
	XTPTabAppearanceStyle GetAppearance();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the currently used tab color style.
	// Returns:
	//     Currently used color style.
	// Remarks:
	//     Color styles are used to colorize the tabs.  For example,
	//     applying the xtpTabColorWinXP color style will give the
	//     tabs Windows XP colorization.
	// See Also: XTPTabColorStyle, SetColor, GetColorSet, SetColorSet
	//-----------------------------------------------------------------------
	XTPTabColorStyle GetColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine is luna colors are disabled.
	// Returns:
	//     TRUE if luna colors are disabled, FALSE is luna colors are
	//     enabled.
	// See Also: DisableLunaColors
	//-----------------------------------------------------------------------
	BOOL IsLunaColorsDisabled();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable/disable luna colors.
	// Parameters:
	//     bDisableLunaColors - TRUE to disable luna colors, FALSE to
	//                          enable luna colors.
	// Remarks:
	//     Luna colors are the Windows XP colors (Silver, Blue, Olive).
	//     CXTPDrawHelpers::GetCurrentSystemTheme can be called to get
	//     the currently used system theme.
	// See Also: IsLunaColorsDisabled, XTPCurrentSystemTheme,
	//           CXTPDrawHelpers::GetCurrentSystemTheme
	//-----------------------------------------------------------------------
	void DisableLunaColors(BOOL bDisableLunaColors);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the COLORREF value for the specified OneNote color.
	// Parameters:
	//     tabColor - XTPTabOneNoteColor flag indicating which OneNote color
	//                to retrieve. See remarks section for available flags.
	// Remarks:
	//     The tab manager calls this member function to retrieve the RGB
	//     color for the value specified by <i>tabColor</i>. <i>tabColor</i>
	//     can be one of the following values:
	//     * <b>xtpTabColorBlue</b>    Blue tab color used when OneNote
	//                                 colors enabled.
	//     * <b>xtpTabColorYellow</b>  Yellow tab color used when OneNote
	//                                 colors enabled.
	//     * <b>xtpTabColorGreen</b>   Green tab color used when OneNote
	//                                 colors enabled.
	//     * <b>xtpTabColorRed</b>     Red tab color used when OneNote
	//                                 colors enabled.
	//     * <b>xtpTabColorPurple</b>  Purple tab color used when OneNote
	//                                 colors enabled.
	//     * <b>xtpTabColorCyan</b>    Cyan tab color used when OneNote
	//                                 colors enabled.
	//     * <b>xtpTabColorOrange</b>  Orange tab color used when OneNote
	//                                 colors enabled.
	//     * <b>xtpTabColorMagenta</b> Magenta tab color used when OneNote
	//                                 colors enabled.
	// Example:
	//     See WM_XTP_GETTABCOLOR for an example.
	// See Also:
	//     XTPTabOneNoteColor
	//-----------------------------------------------------------------------
	static COLORREF AFX_CDECL GetOneNoteColor(XTPTabOneNoteColor tabColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable\disable OneNote colorization for
	//     the tabs.
	// Parameters:
	//     bOneNoteColors - TRUE to enable OneNote colors, FALSE to
	//                      disable OneNote colors.
	// Remarks:
	//     OneNote colors are similar to the tab colorization of tabs in
	//     Microsoft OneNote.
	//-----------------------------------------------------------------------
	void SetOneNoteColors(BOOL bOneNoteColors);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the tab position.
	// Parameters:
	//     tabPosition - New XTPTabPosition to apply to tab manager.
	// Remarks:
	//     The position refers to where the tab buttons are located. The
	//     tab buttons can be positioned on the top, bottom, left, or right
	//     side of the tab client area.
	// See Also: GetPosition
	//-----------------------------------------------------------------------
	void SetPosition(XTPTabPosition tabPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the tab layout style.
	// Parameters:
	//     tabLayout - New XTPTabLayoutStyle to apply to the tabs.
	// Remarks:
	//     The layout refers to how the tab buttons are sized within the
	//     tab client header.  The tab buttons can be in a compressed,
	//     fixed, auto size, or size to fit layout.
	// See Also: GetLayout
	//-----------------------------------------------------------------------
	void SetLayout(XTPTabLayoutStyle tabLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the position of the tab buttons.
	// Returns:
	//     XTPTabPosition of tab buttons.
	// Remarks:
	//     The position refers to where the tab buttons are located. The
	//     tab buttons can be positioned on the top, bottom, left, or right
	//     side of the tab client area.
	// See Also: SetPosition
	//-----------------------------------------------------------------------
	XTPTabPosition GetPosition();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the currently set tab layout.
	// Returns:
	//     Currently used XTPTabLayoutStyle.
	// Remarks:
	//     The layout refers to how the tab buttons are sized within the
	//     tab client header.  The tab buttons can be in a compressed,
	//     fixed, auto size, or size to fit layout.
	// See Also: SetLayout
	//-----------------------------------------------------------------------
	XTPTabLayoutStyle GetLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the font for the tabs.
	// Parameters:
	//     pLogFont - New LOGFONT to use for text in the tab buttons.
	//     bUseStandardFont - When calling SetFontIndirect, always
	//                        use FALSE, this will indicate that pLogFont
	//                        should be used as the new tab font.
	//
	//                        TRUE is used when SetFontIndirect
	//                        is called internally when the system font
	//                        is changed.
	//
	// Remarks:
	//     When bUseStandardFont is TRUE, the standard system "icon" font
	//     will be used.  The standard "icon" font is Tahoma 8pt.
	//-----------------------------------------------------------------------
	void SetFontIndirect(LOGFONT* pLogFont, BOOL bUseStandardFont = FALSE);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to draw the tab control.
	// Parameters:
	//     pTabManager - Pointer to the tab manager to draw.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Bounding rectangle of the client area.
	// Remarks:
	//     This member is called in various Draw and Paint functions.
	//
	//     This member will call CXTPTabPaintManager::CAppearanceSet::DrawTabControl
	//     for the currently set appearance. CXTPTabPaintManager::CAppearanceSet::DrawTabControl
	//     will then call CXTPTabPaintManager::DrawTabControlEx.
	//-----------------------------------------------------------------------
	virtual void DrawTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to adjust the client rectangle.  It will
	//     return the new client rectangle with margins included.
	// Parameters:
	//     pTabManager - Pointer to the tab manager to adjust the client rectangle.
	//     rcClient    - [out] CRect to store client rectangle.
	// Remarks:
	//     AdjustClientRect and RepositionTabControl are called when the window
	//     is resized.  When DrawTabControl is called the changes will be
	//     used when drawing the tab control.
	//
	//     This member will adjust the client rectangle of the entire tab
	//     manager if IsDrawStaticFrame is TRUE to make room to draw the static
	//     frame.
	//
	//     Then CXTPTabPaintManager::CAppearanceSet::AdjustClientRect is called
	//     if there is one or more tabs in the tab manager.
	// See Also:
	//     CXTPTabPaintManager::CAppearanceSet::AdjustClientRect
	//-----------------------------------------------------------------------
	virtual void AdjustClientRect(CXTPTabManager* pTabManager, CRect& rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to move the tab buttons of the tab control to their
	//     correct positions.
	// Parameters:
	//     pTabManager - Pointer to the tab manager the tab button are on.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Client rectangle of control.
	// Remarks:
	//     RepositionTabControl and AdjustClientRect are called when the window
	//     is resized.  When DrawTabControl is called the changes will be
	//     used when drawing the tab control.
	//
	//     CXTPTabPaintManager::CAppearanceSet::RepositionTabControl is then
	//     called for the currently set appearance.
	//-----------------------------------------------------------------------
	virtual void RepositionTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to reposition the tab navigation buttons.
	// Parameters:
	//     pTabManager - Pointer to the tab manager the navigation button are on.
	//     rcClient    - Client rectangle of control.
	// Returns:
	//     Tab header rectangle including header margins.
	// Remarks:
	//     This member is called in CXTPTabPaintManager::RepositionTabControlEx
	//     to reposition the tab navigation buttons.
	//-----------------------------------------------------------------------
	CRect RepositionNavigateButtons(CXTPTabManager* pTabManager, CRect rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to fill the tab navigation buttons.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Tab navigation button to fill.
	//     rc      - Bounding rectangle of the tab navigation button.
	// Remarks:
	//     This member calls m_pColorSet->FillNavigateButton.
	//
	//          The XTPTabColorStyle CColorSet classes override this to perform
	//          actions such as painting the highlighting, pressed, and normal
	//          versions of the tab navigation buttons.
	//
	// See Also: CXTPTabPaintManager::CColorSet::FillNavigateButton, CXTPTabManager::GetNavigateButton, CXTPTabManagerNavigateButton
	//-----------------------------------------------------------------------
	void FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the tab button's text and icon.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pItem  - Tab button to draw text and icon on.
	//     rcItem - Bounding rectangle of the tab.
	//     bDraw  - TRUE to draw the text and icon, FALSE to calculate
	//              the button width (no drawing is done).
	// Returns:
	//     Returns 0 if bDraw is TRUE and drawing is successful.
	//
	//          Returns the button width including the left and right button margin
	//          if bDraw is FALSE.
	//
	// Remarks:
	//     This method only draws the text and icon on a tab button.
	//
	//          CXTPTabPaintManager::CAppearanceSet::DrawSingleButton calls
	//          CXTPTabPaintManager::DrawSingleButtonIconAndText
	//          after the button is drawn.  This is called to draw the button's
	//          icon and text.
	//
	// See Also: CXTPTabPaintManager::DrawTabControlEx, CXTPTabPaintManager::CAppearanceSet::DrawSingleButton
	//-----------------------------------------------------------------------
	virtual int DrawSingleButtonIconAndText(CDC* pDC, CXTPTabManagerItem* pItem, CRect rcItem, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to ensure that a tab is visible.
	// Parameters:
	//     pTabManager - Pointer to the tab manager the tab in on.
	//     pItem       - CXTPTabManagerItem tab to make visible.
	// See Also: CXTPTabManager::SetHeaderOffset, CXTPTabManager::GetHeaderOffset
	//-----------------------------------------------------------------------
	void EnsureVisible(CXTPTabManager* pTabManager, CXTPTabManagerItem* pItem);

	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called to refresh the visual metrics of the
	//     tab main manager.
	// Remarks:
	//     This member checks if m_bUseStandardFont is TRUE, if it is,
	//     then it updates the font with the standard system "icon" font
	//     which is Tahoma 8pt.
	//     This member also calls the RefreshMetrics member for the
	//     currently used CColorSet and CAppearanceSet.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member adds a control to the list of controls that receive
	//     notification when a property of paint manager has changed.
	// Parameters:
	//     pObserver - List of controls that receive
	//     notification when a property of paint manager has changed.
	// Remarks:
	//     This member is not used in the MFC version.  This is only used
	//     in the ActiveX version and does nothing in MFC.
	//-----------------------------------------------------------------------
	void AddObserver(CXTPTabManagerAtom* pObserver);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the CFont used for displaying tab
	//     caption text.
	// Parameters:
	//     bVertical - TRUE if the XTPTabPosition is vertical, FALSE if
	//                 the XTPTabPosition is horizontal.
	// Returns:
	//     CFont used to display caption text in the tab buttons.
	// See Also: GetBoldFont
	//-----------------------------------------------------------------------
	CFont* GetFont(BOOL bVertical = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the CFont used for displaying tab
	//     caption text when bold.
	// Parameters:
	//     bVertical - TRUE if the XTPTabPosition is vertical, FALSE if
	//                 the XTPTabPosition is horizontal.
	// Returns:
	//     CFont used to display bold caption text in the tab buttons.
	// Remarks:
	//     Bold font is used to display caption text when a tab is
	//     selected and m_bBoldSelected = TRUE.
	//
	//          If m_bBoldNormal = TRUE, then the tabs will use this font
	//          even when not selected.
	// See Also: m_bBoldNormal, m_bBoldSelected, GetFont
	//-----------------------------------------------------------------------
	CFont* GetBoldFont(BOOL bVertical = FALSE);

protected:
//{{AFX_CODEJOCK_PRIVATE
	class CSizeToFitSorter;
//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to change the tab button size when the
	//     XTPTabLayoutStyle is xtpTabLayoutSizeToFit.
	// Parameters:
	//     pTabManager - Pointer to tab manager.
	//     nWidth      - Total width that all tab buttons must fit in.
	// Remarks:
	//     When calculating nWidth, the left and right rcHeaderMargin and
	//     tab navigation button widths should be subtracted from the
	//     TabClient width.
	// See Also: XTPTabLayoutStyle, SetLayout, GetLayout
	//-----------------------------------------------------------------------
	void SizeToFit(CXTPTabManager* pTabManager, int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to draw the tab control.
	// Parameters:
	//     pTabManager - Pointer to the tab manager to draw.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Bounding rectangle of the client area.
	// Remarks:
	//     DrawTabControl calls CXTPTabPaintManager::CAppearanceSet::DrawTabControl
	//     which will then call DrawTabControlEx if it has not been overridden.
	// See Also:
	//     CXTPTabPaintManager::CAppearanceSet::DrawTabControl
	//-----------------------------------------------------------------------
	void DrawTabControlEx(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to move the tab buttons of the tab control to their
	//     correct positions.
	// Parameters:
	//     pTabManager - Pointer to the tab manager the tab button are on.
	//     pDC         - Pointer to a valid device context.
	//     rcClient    - Client rectangle of control
	// Remarks:
	//     RepositionTabControl calls CXTPTabPaintManager::CAppearanceSet::RepositionTabControl
	//     which will then call RepositionTabControlEx if it has not been overridden.
	//
	//     RepositionTabControl and AdjustClientRect are called when the window
	//     is resized.  When DrawTabControl is called the changes will be
	//     used when drawing the tab control.
	// See Also:
	//     CXTPTabPaintManager::CAppearanceSet::RepositionTabControl
	//-----------------------------------------------------------------------
	void RepositionTabControlEx(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient);

	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called when a property of the tab paint manager
	//     has changed.
	// Remarks:
	//     This member is called when the value of a property is changed.
	//     For example, when hot tracking in enabled/disabled.
	//-------------------------------------------------------------------------
	virtual void OnPropertyChanged();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to get the total button length including
	//     the left and right button margin.
	// Parameters:
	//     nValue - Width of tab button.
	//     nMin   - Minimum width of tab button.
	//     nMax   - Maximum width of tab button.
	// Returns:
	//     Tab button length including the left and right button margin.
	// See Also: CXTPTabManager::GetItemMetrics
	//-----------------------------------------------------------------------
	int _GetButtonLength(int nValue, int nMin, int nMax);


	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to place an ellipse "..." in the middle
	//     of a string.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     strItem - String item to place an ellipse in.
	//     rcItem  - Bounding rectangle of tab button.  This is the rectangle
	//               (in logical coordinates) in which the text is to be formatted.
	//     uFormat - See nFormat parameter of CDC::DrawText.
	//-----------------------------------------------------------------------
	void DrawTextPathEllipsis(CDC* pDC, CString strItem, CRect rcItem, UINT uFormat);

private:
	int GetPrefixTextExtent(CDC* pDC, CString str, UINT uFormat);
	void StripMnemonics(CString& strClear);

public:
	BOOL              m_bHotTracking;               // If TRUE, tab hot tracking will be enabled.
	BOOL              m_bShowIcons;                 // If TRUE, tab icons will be drawn is icons were added
	BOOL              m_bBoldSelected;              // If TRUE, the text of selected tabs will be displayed in bold font
	BOOL              m_bBoldNormal;                // If TRUE, bold font will be used for all tab both selected and non-selected.
	BOOL              m_bDisableLunaColors;         // If FALSE, Tabs will not use LunaColors, is the current theme uses LunaColors
	BOOL              m_bOneNoteColors;             // If TRUE, tabs will use OneNote colorization
	XTPTabClientFrame m_clientFrame;                // Frame style of tab client area.
	BOOL              m_bStaticFrame;               // If TRUE, a static frame will be drawn around the entire tab area
	CRect             m_rcClientMargin;             // Margin around tab client area.
	CRect             m_rcControlMargin;            // Margin around entire tab control (tab client header and tab client area).
	CRect             m_rcButtonMargin;             // Margin around the text in tab button.
	BOOL              m_bInvertGradient;            // If TRUE, the colors passed into CXTPTabPaintManager::CColorSet::GradientFill will be swapped before the rectangle is filled with the gradient.
	BOOL              m_bFillBackground;            // If TRUE, then the tab client space is filled using CXTPTabPaintManager.CColorSet.m_clrControlFace color.  This is only FALSE when drawing DockingPanePanel AutoHide tabs because there is no client area to draw.
	CSize             m_szIcon;                     // Size of icon displayed in tab button.
	BOOL              m_bDrawTextPathEllipsis;      // If TRUE, when the size of a tab button is too small to display the caption the middle of the caption will be replaced with an ellipse "...", If FALSE, the the end of the caption will be replaced with the ellipse.
	BOOL              m_bDrawTextNoPrefix;          // If FALSE, then an ampersand '&' in the tab button caption will be ignored and a double ampersand '&&' will display a single ampersand '&'.  If TRUE, the the caption will be displayed unmodified.

protected:
	CAppearanceSet*   m_pAppearanceSet;             // Currently set CAppearanceSet.
	CColorSet*        m_pColorSet;                  // Currently used CColorSet.
	CArray<CXTPTabManagerAtom*, CXTPTabManagerAtom*> m_arrObservers; // List of controls that receive notification when a property of paint manager has changed.
	XTPTabAppearanceStyle m_tabAppearance;          // Currently set appearance style.
	XTPTabColorStyle  m_tabColor;                   // Currently used color style.
	XTPTabPosition    m_tabPosition;                // Currently set tab button position.  The position refers to where the tab buttons are located. The tab buttons can be positioned on the top, bottom, left, or right side of the tab client area.
	XTPTabLayoutStyle m_tabLayout;                  // Currently set tab button layout.  The layout refers to how the tab buttons are sized within the tab client header.
	CFont             m_fntNormal;                  // Font used to display text in tab buttons.
	CFont             m_fntBold;                    // Font used to display bold text in the tab buttons.  I.e. When a tab is selected and m_bBoldSelected = TRUE.
	CFont             m_fntVerticalNormal;          // Font used to display tab button caption when the tab position is vertical.
	CFont             m_fntVerticalBold;            // Font used to display bold text in the tab buttons when the tab position is vertical.  I.e. When a tab is selected and m_bBoldSelected = TRUE.
	BOOL              m_bUseStandardFont;           // TRUE to use standard system "icon" font which is Tahoma 8pt.

	friend class CAppearanceSet;
	friend class CColorSet;
	friend class CXTPTabManager;


};


AFX_INLINE void CXTPTabPaintManager::CColorSet::SelectClipRgn(CDC* /*pDC*/, CRect /*rcClient*/, CXTPTabManager* /*pTabManager*/) {
}
//////////////////////////////////////////////////////////////////////////

AFX_INLINE CRect CXTPTabPaintManager::CAppearanceSet::GetHeaderMargin() {
	return m_rcHeaderMargin;
}
AFX_INLINE CRect CXTPTabPaintManager::CAppearanceSet::FillTabControl(CXTPTabManager* /*pTabManager*/, CDC* /*pDC*/, CRect /*rcControl*/) {
	return 0;
}
AFX_INLINE void CXTPTabPaintManager::CAppearanceSet::DrawSingleButton(CDC* /*pDC*/, CXTPTabManagerItem* /*pItem*/) {

}
AFX_INLINE void CXTPTabPaintManager::CAppearanceSet::DrawTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl) {
	m_pPaintManager->DrawTabControlEx(pTabManager, pDC, rcControl);
}
AFX_INLINE void CXTPTabPaintManager::CAppearanceSet::RepositionTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient) {
	m_pPaintManager->RepositionTabControlEx(pTabManager, pDC, rcClient);
}

//////////////////////////////////////////////////////////////////////////

AFX_INLINE CFont* CXTPTabPaintManager::GetFont(BOOL bVertical) {
	return bVertical? &m_fntVerticalNormal: &m_fntNormal;
}
AFX_INLINE CFont* CXTPTabPaintManager::GetBoldFont(BOOL bVertical) {
	return bVertical? &m_fntVerticalBold: &m_fntBold;
}
AFX_INLINE CXTPTabPaintManager::CAppearanceSet* CXTPTabPaintManager::GetAppearanceSet() {
	return m_pAppearanceSet;
}
AFX_INLINE CXTPTabPaintManager::CColorSet* CXTPTabPaintManager::GetColorSet() {
	return m_pColorSet;
}


#endif // !defined(__XTPTABPAINTMANAGER_H__)
