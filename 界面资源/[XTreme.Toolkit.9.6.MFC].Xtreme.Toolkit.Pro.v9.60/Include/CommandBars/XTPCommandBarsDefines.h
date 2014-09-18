// XTPCommandBarDefines.h
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
#if !defined(__XTPCOMMANDBARDEFINES_H__)
#define __XTPCOMMANDBARDEFINES_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPControl;
class CXTPCommandBar;
class CXTPPopupBar;

//-----------------------------------------------------------------------
// Summary:
//     Position of the command bar
// Example:
// <code>
// CXTPToolBar* pToolBar = pCommandBars->Add("ToolBar", xtpBarTop);
// pToolBar->SetPosition(xtpBarBottom);
// </code>
// See Also:
//     CXTPCommandBars, CXTPToolBar, CXTPToolBar::SetPosition
//
// <KEYWORDS xtpBarTop, xtpBarBottom, xtpBarLeft, xtpBarRight, xtpBarFloating, xtpBarPopup, xtpBarListBox, xtpBarNone>
//-----------------------------------------------------------------------
enum XTPBarPosition
{
	xtpBarTop = 0,      // Docked at top.
	xtpBarBottom = 1,   // Docked at bottom.
	xtpBarLeft = 2,     // Docked at left.
	xtpBarRight = 3,    // Docked at right.
	xtpBarFloating = 4, // Floated.
	xtpBarPopup = 5,    // Popup.
	xtpBarListBox = 6,  // List box.
	xtpBarNone = 7      // None.
};

//-----------------------------------------------------------------------
// Summary:
//     Type of the command bar.
// Example:
// <code>
// CXTPCommandBar* pMenuBar = pCommandBars->GetMenuBar();
// ASSERT(pMenuBar->GetType() == xtpBarTypeMenuBar);
// </code>
// See Also: CXTPCommandBars, CXTPCommandBar, CXTPCommandBar::GetType
//
// <KEYWORDS xtpBarTypeMenuBar, xtpBarTypePopup>
//-----------------------------------------------------------------------
enum XTPBarType
{
	xtpBarTypeMenuBar, // Command bar is a menu bar.
	xtpBarTypeNormal,  // Command bar is a toolbar.
	xtpBarTypePopup    // Command bar is a popup.
};

//-----------------------------------------------------------------------
// Summary:
//     Type of the control.
// Example:
//     <code>pToolBar->GetControls()->Add(xtpControlButton, ID_FILE_NEW);</code>
// See Also: CXTPControl, CXTPControl::GetType, CXTPControls
//
// <KEYWORDS xtpControlError, xtpControlButton, xtpControlPopup, xtpControlButtonPopup, xtpControlSplitButtonPopup, xtpControlComboBox, xtpControlEdit, xtpControlCustom, xtpControlLabel>
//-----------------------------------------------------------------------
enum XTPControlType
{
	xtpControlError,            // Type is not defined.
	xtpControlButton,           // Button type.
	xtpControlPopup,            // Popup type.
	xtpControlButtonPopup,      // Button popup.
	xtpControlSplitButtonPopup, // Split button popup.
	xtpControlComboBox,         // Combo box button.
	xtpControlEdit,             // Edit control.
	xtpControlCustom,           // Custom control.
	xtpControlLabel             // Label control
};

//-----------------------------------------------------------------------
// Summary:
//     Flags of the control.
// Example:
//     <code>pControl->SetFlags(xtpFlagRightAlign);</code>
// See Also: CXTPControl, CXTPControl::SetFlags
//
// <KEYWORDS xtpFlagRightAlign, xtpFlagSkipFocus, xtpFlagLeftPopup, xtpFlagManualUpdate, xtpFlagNoMovable, xtpFlagControlStretched>
//-----------------------------------------------------------------------
enum XTPControlFlags
{
	xtpFlagRightAlign = 1,       // Control is right aligned.
	xtpFlagSkipFocus = 2,        // Control does not have focus.
	xtpFlagLeftPopup = 4,        // To Pop up child bar left.
	xtpFlagManualUpdate = 8,     // Control is manually updated.
	xtpFlagNoMovable = 16,       // To disable customization.
	xtpFlagControlStretched = 32 // Control is stretched in parent command bar.
};

//-----------------------------------------------------------------------
// Summary:
//     Control's hide flags, these are flags that specify why a control is hidden.
// Example:
//     <code>pControl->SetHideFlags(xtpHideGeneric);</code>
// See Also: CXTPControl, CXTPControl::SetHideFlags
//
// <KEYWORDS xtpNoHide, xtpHideGeneric, xtpHideWrap, xtpHideDockingPosition, xtpHideScroll, xtpHideCustomize, xtpHideExpand, xtpHideDocTemplate>
//-----------------------------------------------------------------------
enum XTPControlHideFlags
{
	xtpNoHide = 0,              // Control is visible.
	xtpHideGeneric = 1,         // Control is hidden by generic reason.  Developer set Visible = False.
	xtpHideWrap = 2,            // Control is hidden by wrap.  Toolbar is too small and the control is wrapped.
	xtpHideDockingPosition = 4, // Control is hidden by docking position.  If a toolbar has a combobox or edit control, when the toolbar is docked in the left or in the right side of frame, the Commandbar ComboBox and Edit controls becomes hidden.
	xtpHideScroll = 8,          // Control is hidden by scrolling.  There are too many controls in a popup and the control is hidden because the popup is scrolled.
	xtpHideCustomize = 16,      // Control is hidden by customize settings.  User removed it.  This is only possible when customization is enabled.  This flag is set if the user removes the control from the Add and Remove Buttons popup menu.
	xtpHideExpand = 32,         // If the control is rarely used and it is not visible because the intelligent menus option on and the command is a hidden command.
	xtpHideDocTemplate = 64     // Control is hidden because active template excluded from its list.
};

//-----------------------------------------------------------------------
// Summary:
//     Button's styles
// Remarks:
//     This styles can be applied to CXTPControlButton and CXTPControlPopup derived classes.
// Example:
//     <code>pControl->SetStyle(xtpButtonIconAndCaption);</code>
// See Also: CXTPControlButton, CXTPControl::SetStyle, CXTPControl::GetStyle
//
// <KEYWORDS xtpButtonAutomatic, xtpButtonCaption, xtpButtonIcon, xtpButtonIconAndCaption>
//-----------------------------------------------------------------------
enum XTPButtonStyle
{
	xtpButtonAutomatic = 0,     // Automatic style.
	xtpButtonCaption = 1,       // Button draw only caption.
	xtpButtonIcon = 2,          // Button draw only icon.
	xtpButtonIconAndCaption = 3 // Button draw icon and caption.
};

//-----------------------------------------------------------------------
// Summary:
//     Combobox's styles
// Example:
//     CXTPControlComboBox* pComboBox = pToolBar->GetControls()->Add(xtpControlComboBox, ID_GOTO_URL);
//     pComboBox->SetStyle(xtpComboLabel);
// See Also: CXTPControlComboBox, CXTPControlComboBox::SetStyle
//
// <KEYWORDS xtpComboNormal, xtpComboLabel>
//-----------------------------------------------------------------------
enum XTPComboStyle
{
	xtpComboNormal,     // Normal combo box.
	xtpComboLabel       // Combo box with label.
};

//-----------------------------------------------------------------------
// Summary:
//     Paint themes used by command bars.
// Example:
//     <code>XTPPaintManager()->SetTheme(xtpThemeWhidbey);</code>
// See Also:
//     CXTPCommandBars, ÑXTPPaintManager
//
// <KEYWORDS xtpThemeOfficeXP, xtpThemeOffice2000, xtpThemeOffice2003, xtpThemeNativeWinXP, xtpThemeWhidbey, xtpThemeCustom>
//-----------------------------------------------------------------------
enum XTPPaintTheme
{
	xtpThemeOfficeXP,       // Office XP theme.
	xtpThemeOffice2000,     // Office 2000 theme.
	xtpThemeOffice2003,     // Office 2003 theme.
	xtpThemeNativeWinXP,    // Windows XP themes support.
	xtpThemeWhidbey,        // Visual Studio 2005 theme.
	xtpThemeCustom          // Custom theme.
};

//-----------------------------------------------------------------------
// Summary:
//     Menubar system button flags.
// Example:
// <code>
// CXTPMenuBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar", IDR_MAINFRAME);
// pMenuBar->SetFlags(xtpFlagIgnoreSetMenuMessage);
// </code>
// See Also:
//     CXTPMenuBar, CXTPCommandBar, CXTPCommandBar::SetFlags
//
// <KEYWORDS xtpFlagHideMinimizeBox, xtpFlagHideMaximizeBox, xtpFlagIgnoreSetMenuMessage, xtpFlagUseMDIMenus, xtpFlagHideClose, xtpFlagHideMDIButtons, xtpFlagAddMDISysPopup>
//-----------------------------------------------------------------------
enum XTPMenuBarFlags
{
	xtpFlagHideMinimizeBox      = 0x0100L,          // To hide minimize box.
	xtpFlagHideMaximizeBox      = 0x0200L,          // To hide maximize box.
	xtpFlagIgnoreSetMenuMessage = 0x0400L,          // To ignore MDI menus.
	xtpFlagUseMDIMenus          = 0x0800L,          // To use MDI menus. Not longer used.
	xtpFlagHideClose            = 0x1000L,          // To hide close button.
	xtpFlagHideMDIButtons = xtpFlagHideMinimizeBox|xtpFlagHideMaximizeBox|xtpFlagHideClose,  // Specifies to hide all MDI buttons displayed on the Command Bar.
	xtpFlagAddMDISysPopup       = 0x2000L           // To add system MDI popup bar.  Specifies to add system MDI popup menu to the Menu Bar.  This will appears as an icon to the far left of the Menu Bar and will display the MDI menu bar options when clicked.
};

//-----------------------------------------------------------------------
// Summary:
//     Docking flags.
// Example:
// <code>
// CXTPMenuBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar", IDR_MAINFRAME);
// pMenuBar->EnableDocking(xtpFlagAlignTop|xtpFlagStretched);
// </code>
// See Also: CXTPToolBar, CXTPToolBar::EnableDocking
//
// <KEYWORDS xtpFlagAlignTop, xtpFlagAlignBottom, xtpFlagAlignLeft, xtpFlagAlignRight, xtpFlagAlignAny, xtpFlagFloating, xtpFlagHideWrap, xtpFlagStretched>
//-----------------------------------------------------------------------
enum XTPToolBarFlags
{
	xtpFlagAlignTop = 1,                // Allows docking at the top of the client area.
	xtpFlagAlignBottom = 2,             // Allows docking at the bottom of the client area.
	xtpFlagAlignLeft = 4,               // Allows docking on the left side of the client area.
	xtpFlagAlignRight = 8,              // Allows docking on the left side of the client area.
	xtpFlagAlignAny = xtpFlagAlignTop | xtpFlagAlignBottom | xtpFlagAlignLeft | xtpFlagAlignRight, // Allows docking on any side of the client area.
	xtpFlagFloating = 16,               // Allows floating.
	xtpFlagHideWrap = 32,               // Allow to hide wrapped controls.
	xtpFlagStretched = 64               // Bar is stretched.
};

//-----------------------------------------------------------------------
// Summary:
//     Enumerated type used to determine the animation effect of popup bars.
// Example:
//     <code> pCommandBars->GetCommandBarsOptions()->animationType = xtpAnimateSlide; </code>
// See Also: CXTPCommandBarsOptions, CXTPCommandBarsOptions::animationType
//
// <KEYWORDS xtpAnimateWindowsDefault, xtpAnimateRandom, xtpAnimateUnfold, xtpAnimateSlide, xtpAnimateFade, xtpAnimateNone>
//-----------------------------------------------------------------------
enum XTPAnimationType
{
	xtpAnimateWindowsDefault,   // As defined in the "Display" settings.
	xtpAnimateRandom,           // Any of the first three in random selection.
	xtpAnimateUnfold,           // Unfold top to bottom.
	xtpAnimateSlide,            // Slide in from left.
	xtpAnimateFade,             // Fade-in.
	xtpAnimateNone              // No animation.
};

//-----------------------------------------------------------------------
// Summary:
//     Docking direction enumerator.
// See Also: CXTPPopupBar, CXTPPopupBar::m_popupFlags
//
// <KEYWORDS xtpPopupRight, xtpPopupLeft, xtpPopupDown>
//-----------------------------------------------------------------------
enum XTPPopupDirection
{
	xtpPopupRight = 0,          // Popup Bar will be opened right of control.
	xtpPopupLeft = 1,           // Popup Bar will be opened left of control.
	xtpPopupDown = 2            // Popup Bar will be opened bottom of control.
};

//-----------------------------------------------------------------------
// Summary:
//     Special keys enumerator.
// Remarks:
//     See CXTPCommandBar::ProcessSpecialKey for details.
// See Also: CXTPCommandBar, CXTPCommandBar::ProcessSpecialKey
//
// <KEYWORDS xtpKeyNext, xtpKeyPrev, xtpKeyBack, xtpKeyPopup, xtpKeyEscape, xtpKeyReturn, xtpKeyHome, xtpKeyEnd>
//-----------------------------------------------------------------------
enum XTPSpecialKey
{
	xtpKeyNext,             // Select next key
	xtpKeyPrev,             // Select previous key
	xtpKeyBack,             // Close active popup key
	xtpKeyPopup,            // Open selected popup key
	xtpKeyEscape,           // Escape key
	xtpKeyReturn,           // Return key
	xtpKeyHome,             // Select firs key
	xtpKeyEnd               // Select last key
};

//-----------------------------------------------------------------------
// Summary:
//     Flags for idle routine
// Example:
//     <code>pToolBar->DelayRedraw();</code>
// See Also: CXTPCommandBar::DelayRedraw, CXTPCommandBar::DelayLayout
//
// <KEYWORDS xtpIdleLayout, xtpIdleRedraw>
//-----------------------------------------------------------------------
enum XTPIdleFlags
{
	xtpIdleLayout = 1,         // Delay recalculate layout of command bar
	xtpIdleRedraw = 2          // Delay redraw.command bar.
};


#include "XTPCommandBars.inl"
#include "CommonPro.Include\XTPPropExchange.inc"


//-----------------------------------------------------------------------
// Summary:
//     The WM_XTP_CONTROLSELECTED is sent to the CXTPCommandBars site
//     when the user selects a CXTPCommandBar item
// Parameters:
//     pControl - (CXTPControl*)wParam - pointer to selected item.
// Remarks:
//     pControl parameter is NULL when the user remove mouse pointer from selected item.
// Example:
//     Here is an example of how an application would process the WM_XTP_CONTROLSELECTED
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_MESSAGE(WM_XTP_CONTROLSELECTED, OnControlSelected)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CMainFrame::OnControlSelected(WPARAM wParam, LPARAM lParam)
// {
//     UNUSED_ALWAYS(lParam);
//     CXTPControl* pControl = (CXTPControl*)wParam;
//
//     return 0;
// }
// </code>
// See Also: CXTPControl, CXTPCommandBar
//-----------------------------------------------------------------------
const UINT WM_XTP_CONTROLSELECTED = (WM_USER + 305);

//-----------------------------------------------------------------------
// Summary:
//     The WM_XTP_DRAGCONTROLCHANGED is sent to the CXTPCommandBars site when user select button of toolbar
//     in customization mode
// Example:
//     Here is an example of how an application would process the WM_XTP_INITMENU
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_MESSAGE_VOID(WM_XTP_DRAGCONTROLCHANGED, OnDragControlChanged)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CMainFrame::OnDragControlChanged()
// {
//     CXTPControl* pControl = GetCommandBars()->GetDragControl();
// }
// </code>
// See Also: CXTPCommandBars::GetDragControl, CXTPCommandBars::SetCustomizeMode
//-----------------------------------------------------------------------
const UINT WM_XTP_DRAGCONTROLCHANGED = (WM_USER + 306);


//-----------------------------------------------------------------------
// Summary:
//     The WM_XTP_INITMENU is sent to the CXTPCommandBars site when menu is about to become active.
//     It occurs when the user clicks an item on the menu bar or presses a menu key.
//     This allows the application to modify the menu before it is displayed.
// Parameters:
//     pMenuBar - (CXTPMenuBar*)wParam - pointer to CXTPMenuBar object to be initialized.
// Returns:
//     If an application processes this message, it should return zero.
// Example:
//     Here is an example of how an application would process the WM_XTP_INITMENU
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_MESSAGE(WM_XTP_INITMENU, OnInitMenu)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// LRESULT CMainFrame::OnInitMenu(WPARAM wParam, LPARAM lParam)
// {
//     UNUSED_ALWAYS(lParam);
//     CXTPMenuBar* pMenuBar = (CXTPMenuBar*)wParam;
//
//     return 0;
// }
// </code>
// See Also: CXTPCommandBars, CXTPMenuBar
//-----------------------------------------------------------------------
const UINT WM_XTP_INITMENU = (WM_USER + 308);


//-----------------------------------------------------------------------
// Summary:
//     The WM_XTP_INITCOMMANDSPOPUP is sent to the CXTPCommandBars site when a pop-up menu
//     is about to become active
// Parameters:
//     pPopupBar - Pointer to drop-down menu.
// Example:
//     Here is an example of how an application would process the WM_XTP_INITCOMMANDSPOPUP
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_XTP_INITCOMMANDSPOPUP()
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CMainFrame::OnInitCommandsPopup(CXTPPopupBar* pPopupBar)
// {
//     // get the list of commands for the popup.
//     CXTPControls* pControls = pPopupBar->GetControls();
// }
// </code>
// See Also: WM_XTP_UNINITCOMMANDSPOPUP, CXTPPopupBar
//-----------------------------------------------------------------------
const UINT WM_XTP_INITCOMMANDSPOPUP = (WM_USER + 307);

//<ALIAS WM_XTP_INITCOMMANDSPOPUP>
#define ON_XTP_INITCOMMANDSPOPUP()
//{{AFX_CODEJOCK_PRIVATE
#undef ON_XTP_INITCOMMANDSPOPUP
#define ON_XTP_INITCOMMANDSPOPUP()\
	{ WM_XTP_INITCOMMANDSPOPUP, 0, 0, 0, AfxSig_vs, \
	(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(CXTPPopupBar*))&OnInitCommandsPopup } ,
//}}AFX_CODEJOCK_PRIVATE

//-----------------------------------------------------------------------
// Summary:
//     The WM_XTP_UNINITCOMMANDSPOPUP is sent to the CXTPCommandBars site when a pop-up
//     menu has been closed.
// Parameters:
//     pPopupBar - Pointer to drop-down menu.
// Example:
//     Here is an example of how an application would process the WM_XTP_UNINITCOMMANDSPOPUP
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_XTP_UNINITCOMMANDSPOPUP()
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CMainFrame::OnUninitCommandsPopup(CXTPPopupBar* pPopupBar)
// {
//     // get the list of commands for the popup.
//     CXTPControls* pControls = pPopupBar->GetControls();
// }
// </code>
// See Also: ON_XTP_INITCOMMANDSPOPUP
//-----------------------------------------------------------------------
const UINT WM_XTP_UNINITCOMMANDSPOPUP = (WM_USER + 309);

//<ALIAS WM_XTP_UNINITCOMMANDSPOPUP>
#define ON_XTP_UNINITCOMMANDSPOPUP()
//{{AFX_CODEJOCK_PRIVATE
#undef ON_XTP_UNINITCOMMANDSPOPUP
#define ON_XTP_UNINITCOMMANDSPOPUP()\
	{ WM_XTP_UNINITCOMMANDSPOPUP, 0, 0, 0, AfxSig_vs, \
	(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(CXTPPopupBar*))&OnUninitCommandsPopup } ,
//}}AFX_CODEJOCK_PRIVATE

//-----------------------------------------------------------------------
// Summary:
//     This structure is passed as parameter in CBN_XTP_EXECUTE message.
// Remarks:
//     See description of CBN_XTP_EXECUTE  for details.
// See Also: CBN_XTP_EXECUTE
//-----------------------------------------------------------------------
typedef struct
{
	NMHDR   hdr;            // NMHDR structure that contains additional information about this notification.
	CXTPControl* pControl;  // Pointer to control that caused the event.
}
NMXTPCONTROL, FAR* LPNMXTPCONTROL;

//-----------------------------------------------------------------------
// Summary:
//     The CBN_XTP_EXECUTE is sent to the CXTPCommandBars site when user click item of CXTPCommandBar object
// Parameters:
//     id -         ID of control was clicked.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     Use ON_COMMAND to map a single command to a member function if you don't need exact control that caused the event.
//     Use ON_COMMAND_RANGE or ON_XTP_EXECUTE_RANGE to map a range of command ids to one member function.
//     <p/>
//     If you don't set *pResult = 1 then WM_COMMAND also will be sent for this event.
// Example:
//     Here is an example of how an application would process the CBN_XTP_EXECUTE
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_XTP_EXECUTE(ID_TOOLS_TOOLITEM, OnToolsItem)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CMainFrame::OnToolsItem(NMHDR* pNMHDR, LRESULT* pResult)
// {
//      CXTPControl* pControl = ((NMXTPCONTROL*)pNMHDR)->pControl;
//
//      CToolRec* pTool = (CToolRec*)pControl->GetTag();
//
//      if (!pTool)
//          return;
//
//      CString strArg = pTool->m_strArg;
//      CString strDir = pTool->m_strDir;
//
//      // launch the process.
//      ::ShellExecute( NULL, _T( "open" ), pTool->m_strCmd, strArg, strDir, SW_SHOW);
//
//      *pResult = 1;
// }
// </code>
// See Also: CXTPControl, NMXTPCONTROL
//-----------------------------------------------------------------------
const UINT CBN_XTP_EXECUTE = 100;

//<ALIAS CBN_XTP_EXECUTE>
#define ON_XTP_EXECUTE(id, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_XTP_EXECUTE
#define ON_XTP_EXECUTE(id, memberFxn) \
	ON_NOTIFY(CBN_XTP_EXECUTE, id, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

//<ALIAS CBN_XTP_EXECUTE>
#define ON_XTP_EXECUTE_RANGE(id, idLast, memberFxn)
//{{AFX_CODEJOCK_PRIVATE
#undef ON_XTP_EXECUTE_RANGE
#define ON_XTP_EXECUTE_RANGE(id, idLast, memberFxn) \
	ON_NOTIFY_EX_RANGE(CBN_XTP_EXECUTE, id, idLast, memberFxn)
//}}AFX_CODEJOCK_PRIVATE

//-------------------------------------------------------------------------
// Summary:
//     Create control structure.
// Remarks:
//     This structure contains extended information about CXTPControl object to be created.
//     CXTPCommandBars sent XTP_COMMANDBARS_CREATECONTROL message to owner site when new CXTPControl object
//     is about to become created.
// See Also: WM_XTP_CREATECONTROL
//-------------------------------------------------------------------------
typedef struct
{
	UINT nID;                       // Identifier of the control to be created.
	CXTPControl* pControl;          // Control to be created.
	BOOL bToolBar;                  // TRUE if control is toolbar located.
	int nIndex;                     // Index of the control to be created.
	CMenu* pMenu;                   // A Pointer to CMenu object.
	CXTPCommandBar* pCommandBar;    // Pointer to the parent command bar class.
	CString strCaption;             // Caption of the control to be created.
	XTPControlType controlType;     // Type of the control.
	XTPButtonStyle buttonStyle;     // Button Style of the control.
}
XTP_COMMANDBARS_CREATECONTROL, FAR* LPCREATECONTROLSTRUCT;

//-----------------------------------------------------------------------
// Summary:
//     The WM_XTP_CREATECONTROL is sent to the CXTPCommandBars site when new CXTPControl object
//     is created.
// Parameters:
//     lpCreateControl - Pointer to XTP_COMMANDBARS_CREATECONTROL structure with information about control to be created.
// Returns:
//     If an application processes this message, it should return TRUE.
// Example:
//     Here is an example of how an application would process the WM_XTP_CREATECONTROL
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_XTP_CREATECONTROL()
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
// {
//      if (lpCreateControl->nID == ID_BDR_NONE)
//      {
//           lpCreateControl->controlType = xtpControlSplitButtonPopup;
//           return TRUE;
//      }
//      return FALSE
// }
// </code>
// See Also: XTP_COMMANDBARS_CREATECONTROL, CXTPControl
//-----------------------------------------------------------------------
const UINT WM_XTP_CREATECONTROL = (WM_USER + 303);

//<ALIAS WM_XTP_CREATECONTROL>
#define ON_XTP_CREATECONTROL()
//{{AFX_CODEJOCK_PRIVATE
#undef ON_XTP_CREATECONTROL
#define ON_XTP_CREATECONTROL()\
	{ WM_XTP_CREATECONTROL, 0, 0, 0, AfxSig_is, \
	(AFX_PMSG)(AFX_PMSGW)(int (AFX_MSG_CALL CWnd::*)(LPCREATECONTROLSTRUCT))&OnCreateControl } ,
//}}AFX_CODEJOCK_PRIVATE

//-------------------------------------------------------------------------
// Summary:
//     Create bar structure
// Remarks:
//     This structure contains extended information about bar to be created.
//     CXTPCommandBars sent XTP_COMMANDBARS_CREATEBAR message to owner site when new CXTPCommandBar object
//     is about to become created.
// See Also: WM_XTP_CREATECOMMANDBAR
//-------------------------------------------------------------------------
typedef struct
{
	CXTPCommandBar* pCommandBar;    // Created command bar.
	BOOL bPopup;                    // TRUE if it is a popup command bar,
	BOOL bExpandBar;                // TRUE if it is an expanded popup.
	BOOL bCustomBar;                // TRUE if it is a user defined toolbar.
	BOOL bTearOffBar;               // TRUE if it is a user tear-off popup.
	LPCTSTR lpcstrCaption;          // Caption of the command bar.
	UINT nID;                       // Identifier of the command bar.
}
XTP_COMMANDBARS_CREATEBAR, FAR* LPCREATEBARSTRUCT;

//-----------------------------------------------------------------------
// Summary:
//     The WM_XTP_CREATECOMMANDBAR is sent to the CXTPCommandBars site when new CXTPCommandBars object
//     is created.
// Parameters:
//     lpCreateBar - Pointer to XTP_COMMANDBARS_CREATEBAR structure with information about bar to be created.
// Returns:
//     If an application processes this message, it should return TRUE.
// Example:
//     Here is an example of how an application would process the WM_XTP_CREATECOMMANDBAR
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_XTP_CREATECOMMANDBAR()
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// int CMainFrame::OnCreateCommandBar(LPCREATEBARSTRUCT lpCreateBar)
// {
//     if (lpCreateBar->bPopup && _tcscmp(lpCreateBar->lpcstrCaption, _T("Find")) == 0)
//     {
//
//         CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());
//         pPopupBar->SetTearOffPopup(_T("Find Bar"), 1000);
//         lpCreateBar->pCommandBar = pPopupBar;
//         return TRUE;
//     }
//     return FALSE
// }
// </code>
// See Also: XTP_COMMANDBARS_CREATEBAR, CXTPCommandBar
//-----------------------------------------------------------------------
const UINT WM_XTP_CREATECOMMANDBAR = (WM_USER + 304);

//<ALIAS WM_XTP_CREATECOMMANDBAR>
#define ON_XTP_CREATECOMMANDBAR()
//{{AFX_CODEJOCK_PRIVATE
#undef ON_XTP_CREATECOMMANDBAR
#define ON_XTP_CREATECOMMANDBAR()\
	{ WM_XTP_CREATECOMMANDBAR, 0, 0, 0, AfxSig_is, \
	(AFX_PMSG)(AFX_PMSGW)(int (AFX_MSG_CALL CWnd::*)(LPCREATEBARSTRUCT))&OnCreateCommandBar} ,
//}}AFX_CODEJOCK_PRIVATE


//-----------------------------------------------------------------------
// Summary:
//     This macro generates the C++ header code necessary for a CXTPControl-derived class that can be serialized.
// Parameters:
//     class_name - The actual name of the class (not enclosed in quotation marks).
// Remarks:
//     Use the DECLARE_XTP_CONTROL macro in a .H module, then include that module in all .CPP modules
//     that need access to objects of this class.
//     <p/>
//     If DECLARE_XTP_CONTROL is included in the class declaration, then IMPLEMENT_XTP_CONTROL must be
//     included in the class implementation.
//     <p/>
//     The DECLARE_XTP_CONTROL macro includes all the functionality of DECLARE_SERIAL, DECLARE_DYNAMIC and DECLARE_DYNCREATE.
//
// Example:
// <code>
// class CBitmapControl: public CXTPControl
// {
//     DECLARE_XTP_CONTROL(CBitmapControl)
//
// public:
//     CBitmapControl();
//
// };
// </code>
// See Also: IMPLEMENT_XTP_CONTROL, DECLARE_XTP_COMMANDBAR
//-----------------------------------------------------------------------
#define DECLARE_XTP_CONTROL(class_name)
//{{AFX_CODEJOCK_PRIVATE
#undef DECLARE_XTP_CONTROL
#define DECLARE_XTP_CONTROL(class_name) \
	DECLARE_SERIAL(class_name)\
	virtual CXTPControl* Clone(BOOL bRecursive);
//}}AFX_CODEJOCK_PRIVATE

//-----------------------------------------------------------------------
// Summary:
//     This macro generates the C++ code necessary for a CXTPControl-derived class that can be serialized.
// Parameters:
//     class_name      - The actual name of the class (not enclosed in quotation marks).
//     base_class_name - The name of the base class (not enclosed in quotation marks).
// Remarks:
//     Use the IMPLEMENT_XTP_CONTROL macro in a .CPP module; then link the resulting object code only once.
// Example:
// <code>
// // BitmapControl.cpp
// #include "stdafx.h"
// #include "BitmapControl.h"
//
// IMPLEMENT_XTP_CONTROL(CBitmapControl, CXTPControl)
// </code>
// See Also: DECLARE_XTP_CONTROL, DECLARE_XTP_COMMANDBAR
//-----------------------------------------------------------------------
#define IMPLEMENT_XTP_CONTROL(class_name, base_class_name)
//{{AFX_CODEJOCK_PRIVATE
#undef IMPLEMENT_XTP_CONTROL
#define IMPLEMENT_XTP_CONTROL(class_name, base_class_name) \
	IMPLEMENT_SERIAL( class_name, base_class_name, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)\
	CXTPControl* class_name::Clone(BOOL bRecursive) {\
		class_name* pButton = (class_name*)class_name::CreateObject();\
		pButton->Copy(this, bRecursive);\
		return pButton;\
	}
//}}AFX_CODEJOCK_PRIVATE

//-----------------------------------------------------------------------
// Summary:
//     This macro generates the C++ header code necessary for a CXTPCommandBar-derived class that can be serialized.
// Parameters:
//     class_name - The actual name of the class (not enclosed in quotation marks).
// Remarks:
//     Use the DECLARE_XTP_COMMANDBAR macro in a .H module, then include that module in all .CPP modules
//     that need access to objects of this class.
//     <p/>
//     If DECLARE_XTP_COMMANDBAR is included in the class declaration, then IMPLEMENT_XTP_COMMANDBAR must be
//     included in the class implementation.
//     <p/>
//     The DECLARE_XTP_COMMANDBAR macro includes all the functionality of DECLARE_SERIAL, DECLARE_DYNAMIC and DECLARE_DYNCREATE.
//
// Example:
// <code>
// class CToolPopupBar: public CXTPPopupBar
// {
//     DECLARE_XTP_COMMANDBAR(CToolPopupBar)
//
// public:
//     CToolPopupBar();
//
// };
// </code>
// See Also: IMPLEMENT_XTP_COMMANDBAR, DECLARE_XTP_CONTROL
//-----------------------------------------------------------------------
#define DECLARE_XTP_COMMANDBAR(class_name)
//{{AFX_CODEJOCK_PRIVATE
#undef DECLARE_XTP_COMMANDBAR
#define DECLARE_XTP_COMMANDBAR(class_name) \
	DECLARE_SERIAL(class_name)\
	virtual CXTPCommandBar* Clone(BOOL bRecursive);
//}}AFX_CODEJOCK_PRIVATE

//-----------------------------------------------------------------------
// Summary:
//     This macro generates the C++ code necessary for a CXTPCommandBar-derived class that can be serialized.
// Parameters:
//     class_name      - The actual name of the class (not enclosed in quotation marks).
//     base_class_name - The name of the base class (not enclosed in quotation marks).
// Remarks:
//     Use the IMPLEMENT_XTP_COMMANDBAR macro in a .CPP module; then link the resulting object code only once.
// Example:
// <code>
// // ToolPopupBar.cpp
// #include "stdafx.h"
// #include "ToolPopupBar.h"
//
// IMPLEMENT_XTP_COMMANDBAR(CToolPopupBar, CXTPPopupBar)
// ...
// pCommandBars->SetPopupBarClass(RUNTIME_CLASS(CToolPopupBar));
// ...
// </code>
// See Also: DECLARE_XTP_COMMANDBAR, DECLARE_XTP_CONTROL
//-----------------------------------------------------------------------
#define IMPLEMENT_XTP_COMMANDBAR(class_name, base_class_name)
//{{AFX_CODEJOCK_PRIVATE
#undef IMPLEMENT_XTP_COMMANDBAR
#define IMPLEMENT_XTP_COMMANDBAR(class_name, base_class_name) \
	IMPLEMENT_SERIAL(class_name, base_class_name, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)\
	CXTPCommandBar* class_name::Clone(BOOL bRecursive) {\
		class_name* pCommandBar = (class_name*)class_name::CreateObject();\
		pCommandBar->Copy(this, bRecursive);\
		return pCommandBar;\
	}

#define CMDTARGET_RELEASE(x) if (x) { x->InternalRelease(); x = 0;}
#define CMDTARGET_ADDREF(x) if (x) { x->InternalAddRef(); }
#define SAFE_INVALIDATE(pWnd) if (pWnd && pWnd->GetSafeHwnd()) pWnd->Invalidate(FALSE);

// Internal defines
#define TRUE_POPUP 2
#define TRUE_KEYBOARD 2
#define FALSE_EXIT 2
#define LM_HIDEWRAP 0x80
#define LM_POPUP 0x100
#define _EMBOSSED_STYLE

// ActiveX commands
#define WM_XTP_COMMAND (WM_USER + 301)
#define WM_XTP_UPDATE_COMMAND (WM_USER + 302)
//}}AFX_CODEJOCK_PRIVATE


#endif //#if !defined(__XTPCOMMANDBARDEFINES_H__)
