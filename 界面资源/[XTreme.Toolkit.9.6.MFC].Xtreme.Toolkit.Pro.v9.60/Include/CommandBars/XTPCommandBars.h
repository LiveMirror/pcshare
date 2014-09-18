// XTPCommandBars.h : interface for the CXTPCommandBars class.
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
#if !defined(__XTPCOMMANDBARS_H__)
#define __XTPCOMMANDBARS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPCommandBarsDefines.h"
#include "XTPControl.h"
#include "XTPControls.h"
#include "XTPToolBar.h"
#include "XTPMenuBar.h"


class CXTPDockBar;
class CXTPToolBar;
class CXTPControl;
class CXTPPopupBar;
class CXTPDockState;
class CXTPCommandBar;
class CXTPCustomizeDropSource;
class CXTPCommandBars;
class CXTPCommandBarList;
class CXTPImageManager;
class CXTPPaintManager;
class CXTPControls;
class CXTPMenuBar;
class CXTPReBar;


//-----------------------------------------------------------------------
// Summary:
//     XTP_COMMANDBARS_PROPEXCHANGE_PARAM is a
//     structure used to store the CommandBar's save settings.  This is
//     a set of State options that will be used to change how and what
//     will be saved and loaded when loading and saving XML files.
// Remarks:
//     When saving your XML string to a file, be sure to use the .XML
//     file extension.  If you open up your .XML file in your favorite
//     web browser it will automatically format the XML string into the
//     correct format.
// See Also: CXTPCommandBars::DoPropExchange, CXTPPropExchangeXMLNode
// Example:
// <code>
// XTP_COMMANDBARS_PROPEXCHANGE_PARAM param;
// param.bSerializeControls = TRUE;
// // Serialize to XML
// CXTPPropExchangeXMLNode px(FALSE, 0, _T("Settings"));
// CXTPPropExchangeSection pxCommandBars(px.GetSection(_T("CommandBars")));
// GetCommandBars()->DoPropExchange(&pxCommandBars, &param);
// px.SaveToFile(m_strIniFileName);
// </code>
//-----------------------------------------------------------------------
struct XTP_COMMANDBARS_PROPEXCHANGE_PARAM
{

//{{AFX_CODEJOCK_PRIVATE
	//-------------------------------------------------------------------------
	// Summary:
	//     Creates a XTP_COMMANDBARS_PROPEXCHANGE_PARAM object which is a
	//     structure used to store the CommandBar's save settings.  This is
	//     a set of State options that will be used to change how and what
	//     will be saved and loaded when loading and saving XML files.
	//     The CXTPPropExchangeXMLNode class is used to create the XML file.
	// Remarks:
	//     When saving your XML string to a file, be sure to use the .XML
	//     file extension.  If you open up your .XML file in your favorite
	//     web browser it will automatically format the XML string into the
	//     correct format.
	// See Also: CXTPCommandBars::DoPropExchange, CXTPPropExchangeXMLNode
	//-------------------------------------------------------------------------
	XTP_COMMANDBARS_PROPEXCHANGE_PARAM()
	{
		pCommandBars = NULL;
		bSerializeControls = FALSE;
		bSaveOnlyCustomized = TRUE;
		bLoadSilent = FALSE;
		bSaveOriginalControls = TRUE;
		bSerializeLayout = TRUE;
		bSerializeOptions = FALSE;
		bSerializeImages = FALSE;
		bSerializeDesignerControls = FALSE;
	}
//}}AFX_CODEJOCK_PRIVATE

	BOOL bSerializeControls;            // If True, the controls in the CommandBars will be saved.  If False, no controls will be saved,
	                                    // regardless of the other settings.  The default is True
	BOOL bSaveOnlyCustomized ;          // If True, only CommandBars that have been customized will be saved.  The default is True
	BOOL bLoadSilent;                   // If True, no message boxes will be displayed when the CommandBar layout is loaded.
	                                    // i.e.  The message box that asks for conformation before the CommandBar layout is reset.
	BOOL bSaveOriginalControls;         // If True, the original state of the CommandBars is saved along with the customized state
	                                    // For example, if a button on a toolbar is moved, then both the original and modified states will be saved.
	                                    // The Default is True
	BOOL bSerializeImages;              // If True, the images stored in the ImageManager will be saved.  The default is False
	BOOL bSerializeOptions;             // If True, the setting on the Keyboard and Options pages of the Customize dialog will
	                                    // be saved.  This includes shortcut keys and CommandBar options like large icons and full menus.
	                                    // The default is False
	BOOL bSerializeLayout;              // If True, the layout of the CommandBars will be saved.  The Layout includes information such as bar position, location, and size.  The Default is True
	BOOL bSerializeDesignerControls;    // In the CommandBar Designer you can export a XCB file to a XML file and Load it in your project.
	                                    // If SerializeDesignerControls is True, then the DesignerControls property will be filled with
	                                    // Control items from the CommandBar Designer's controls pane.  This allows you to load the XML file instead
	                                    // of loading the XCB file using the LoadDesignerBars method.  NOTE: DesignerControls are the controls displayed in the customization dialog.
	                                    // The CXTPPropExchangeXMLNode class is used to create the XML file.
	                                    // The default is False
	CXTPCommandBars* pCommandBars;      // Pointer to the CommandBars object that the XTP_COMMANDBARS_PROPEXCHANGE_PARAM setting apply to.
};

//===========================================================================
// Summary:
//     Options of the command bars.
// See Also:
//     CXTPCommandBars::GetCommandBarsOptions
// Example:
//     <code>pCommandBars->GetCommandBarsOptions()->animationType = xtpAnimateFade;</code>
//===========================================================================
class _XTP_EXT_CLASS CXTPCommandBarsOptions : public CCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs an CXTPCommandBarsOptions object
	//-----------------------------------------------------------------------
	CXTPCommandBarsOptions();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes options from or to an archive.
	// Parameters:
	//     ar - A CArchive object to serialize to or from.
	//-----------------------------------------------------------------------
	void Serialize(CArchive& ar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes options from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//     bUserOptionsOnly - TRUE to serialize user's options only.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX, BOOL bUserOptionsOnly = FALSE);


public:
	BOOL bAlwaysShowFullMenus;                          // TRUE to show full menus always.
	BOOL bShowFullAfterDelay;                           // TRUE to show full menus after short delay.
	BOOL bToolBarScreenTips;                            // TRUE to show toolbar's tooltips
	BOOL bToolBarAccelTips;                             // TRUE to add accelerators to tooltips.
	BOOL bLargeIcons;                                   // TRUE to use large toolbars icons.
	BOOL bSyncFloatingBars;                             // TRUE to hide floating bars when site frame deactivated.
	BOOL bShowExpandButtonAlways;                       // TRUE to show expand buttons always.
	BOOL bDblClickFloat;                                // TRUE to make toolbar floating be double-clicking.
	BOOL bShowTextBelowIcons;                           // TRUE to show captions below icons.
	BOOL bAltDragCustomization;                         // Allows users to change a toolbar button's position by dragging it while holding down ALT.
	BOOL bAutoHideUnusedPopups;                         // TRUE to hide unused popup controls.  This is similar to how the Windows Start menu hides infrequently used popups.
	BOOL bDirtyState;                                   // TRUE if the user has done any type of customization to menus or toolbars. (I.e. Moving a toolbar or adding a new button).  After a load or save of the BarStae this is set to FALSE.

	XTPAnimationType animationType;                     // Animation type.

	CSize szIcons;                                      // Size of the normal icons.
	CSize szLargeIcons;                                 // Size of the large icons.
	CSize szPopupIcons;                                 // Size of icons displayed in popup menus.

	CMap<UINT, UINT, BOOL, BOOL> m_mapHiddenCommands;   // Hidden command array.
	CMap<UINT, UINT, BOOL, BOOL> m_mapUsedCommands;     // Used commands array.

private:
	CXTPCommandBars* m_pCommandBars;


	friend class CXTPCommandBars;
};

//===========================================================================
// Summary:
//     CXTPCommandBars is standalone class. It allows developer manipulate
//     toolbars and popupbars
// See Also: CXTPCommandBar, CXTPToolBar, CXTPCommandBarsOptions
//===========================================================================
class _XTP_EXT_CLASS CXTPCommandBars : public CCmdTarget
{

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCommandBars object
	//-----------------------------------------------------------------------
	CXTPCommandBars();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCommandBars object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPCommandBars();


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to create menu bar.
	// Parameters:
	//     strName - Title of menu bar.
	//     nID - menu identifier to be loaded.
	// Returns:
	//     A CXTPMenuBar object if successful; otherwise NULL.
	// See Also: Add
	//-----------------------------------------------------------------------
	CXTPMenuBar* SetMenu(CString strName, UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to add a new toolbar.
	// Parameters:
	//     strName - Title of toolbar.
	//     barPosition - Position of toolbar to be created.
	//     pToolbarClass - Runtime class of toolbar to create
	// Remarks:
	//     CommandBars will create new toolbar with pToolbarClass runtime class if it specified,
	//     if not m_pToolBarClass will be used.<p/>
	//     Call SetToolBarClass to set default runtime class for toolbars.
	// Returns:
	//     A CXTPToolBar object if successful; otherwise NULL.
	// See Also: SetMenu, SetToolBarClass
	//-----------------------------------------------------------------------
	CXTPToolBar* Add(CString strName, XTPBarPosition barPosition, CRuntimeClass* pToolbarClass = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the active menu bar.
	// Returns:
	//     A CXTPMenuBar pointer associated with the menu bar.
	//-----------------------------------------------------------------------
	CXTPMenuBar* GetMenuBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the toolbar with the specified identifier.
	// Parameters:
	//     nID - An integer identifier.
	// Returns:
	//     The CXTPToolBar pointer
	//-----------------------------------------------------------------------
	CXTPToolBar* GetToolBar(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the site of the command bars.
	// Returns:
	//     A CFrameWnd pointer representing the site of the command bars.
	//-----------------------------------------------------------------------
	CWnd* GetSite();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the dock bar at the specified position.
	// Parameters:
	//     barPosition - Position of the dockbar.
	// Returns:
	//     A CXTPDockBar pointer.
	//-----------------------------------------------------------------------
	CXTPDockBar* GetDockBar(XTPBarPosition barPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to redraw the command bars.
	//-----------------------------------------------------------------------
	void RedrawCommandBars();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to recalculate site layout.
	// Parameters:
	//     bDelay - TRUE to delay dock site layout until remaining framework
	//              items have been initialized completely.
	//-----------------------------------------------------------------------
	void RecalcFrameLayout(BOOL bDelay = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to find the specified control.
	// Parameters:
	//     type - Type of the control to find.
	//     nId - Control's identifier.
	//     bVisible - Control is visible.
	//     bRecursive - To find in the nested command bars.
	// Returns:
	//     The CXTPControl object if successful; otherwise NULL.
	//-----------------------------------------------------------------------
	CXTPControl* FindControl(XTPControlType type, UINT nId, BOOL bVisible, BOOL bRecursive);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to display a floating pop-up menu at the specified
	//     location.
	// Parameters:
	//     pMenu        - Menu to be popped up
	//     pPopupBar    - Points to a CXTPPopupBar object
	//     nFlags       - Reserved, currently not used.
	//     x            - Specifies the horizontal position in screen coordinates
	//                    of the pop-up menu.
	//     y            - Specifies the vertical position in screen coordinates
	//                    of the top of the menu on the screen.
	//     pWnd         - Identifies the window that owns the pop-up menu.
	//                    This window receives all WM_COMMAND messages from
	//                    the menu.
	//     rcExclude    - Reserved, currently not used.
	//     pWndOwner    - Points to a CWnd object
	//     pCommandBars - Points to a CXTPCommandBars object.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	// Example:
	// <code>
	// void CCustomThemesView::OnRButtonDown(UINT /*nFlags*/, CPoint point)
	// {
	//     ClientToScreen(&point);
	//
	//     CMenu menu;
	//     VERIFY(menu.LoadMenu(IDR_MAINFRAME));
	//
	//     // get a pointer to the application window.
	//     CXTPMDIFrameWnd* pMainWnd = DYNAMIC_DOWNCAST(
	//         CXTPMDIFrameWnd, AfxGetMainWnd());
	//
	//     if (!pMainWnd)
	//         return;
	//
	//     // get a pointer to the CXTPCommandBars object.
	//     CXTPCommandBars* pCommandBars = pMainWnd->GetCommandBars();
	//
	//     if (!pCommandBars)
	//         return;
	//
	//     // display context menu.
	//     CXTPCommandBars::TrackPopupMenu(&menu, TPM_RIGHTBUTTON,
	//         point.x, point.y, pMainWnd, 0, 0, pCommandBars);
	// }
	// </code>
	//-----------------------------------------------------------------------
	static BOOL AFX_CDECL TrackPopupMenu(CMenu* pMenu, UINT nFlags, int x, int y, CWnd* pWnd, LPCRECT rcExclude = NULL, CWnd* pWndOwner = NULL, CXTPCommandBars* pCommandBars = NULL);
	static BOOL AFX_CDECL TrackPopupMenu(CXTPPopupBar* pPopupBar, UINT nFlags, int x, int y, CWnd* pWnd, LPCRECT rcExclude = NULL, CWnd* pWndOwner = NULL); //<COMBINE CXTPCommandBars::TrackPopupMenu@CMenu*@UINT@int@int@CWnd*@LPCRECT@CWnd*@CXTPCommandBars*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the options of the command bars.
	// Returns:
	//     Command Bars options.
	//-----------------------------------------------------------------------
	CXTPCommandBarsOptions* GetCommandBarsOptions();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to save the state information to the registry
	//     or .INI file.
	// Parameters:
	//     lpszProfileName - Points to a null-terminated string that
	//                       specifies the name of a section in the
	//                       initialization file or a key in the Windows
	//                       registry where state information is stored.
	//    bOnlyCustomized  - TRUE to save only the state for customized
	//                       command bar items.
	// See Also: LoadBarState, DoPropExchange
	//-----------------------------------------------------------------------
	void SaveBarState(LPCTSTR lpszProfileName, BOOL bOnlyCustomized = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to retrieve state information from the registry
	//     or .INI file.
	// Parameters:
	//     lpszProfileName - Points to a null-terminated string that
	//                       specifies the name of a section in the initialization
	//                       file or a key in the Windows registry where
	//                       state information is stored.
	//     bSilent         - TRUE to disable user notifications when command bars are restore to their original state.
	// See Also: SaveBarState, DoPropExchange
	//-----------------------------------------------------------------------
	void LoadBarState(LPCTSTR lpszProfileName, BOOL bSilent = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to save the command bars options.
	// Parameters:
	//     lpszProfileName - Points to a null-terminated string that
	//                       specifies the name of a section in the initialization
	//                       file or a key in the Windows registry where
	//                       state information is stored.
	// See Also: LoadOptions, DoPropExchange
	//-----------------------------------------------------------------------
	void SaveOptions(LPCTSTR lpszProfileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to restore the options of the command bars.
	// Parameters:
	//     lpszProfileName - Points to a null-terminated string that
	//                       specifies the name of a section in the initialization
	//                       file or a key in the Windows registry where
	//                       state information is stored.
	// See Also: SaveOptions, DoPropExchange
	//-----------------------------------------------------------------------
	void LoadOptions(LPCTSTR lpszProfileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to either read or write command bars from
	//     or to an archive.
	// Parameters:
	//     ar                 - A CArchive object to serialize to or from.
	//     bSerializeControls - TRUE to store controls information.
	//     bOnlyCustomized    - TRUE to store each commandbar only if it was customized by user.
	//     bSilent            - TRUE to disable user notifications when command bars are restore to their original state.
	// See Also: DoPropExchange
	//-----------------------------------------------------------------------
	void SerializeBarState(CArchive& ar, BOOL bSerializeControls = FALSE, BOOL bOnlyCustomized = TRUE, BOOL bSilent = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an storage.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//     pParam - Exchange options.
	// Example:
	// <code>
	// XTP_COMMANDBARS_PROPEXCHANGE_PARAM param;
	// param.bSerializeControls = TRUE;
	// // Serialize to XML
	// CXTPPropExchangeXMLNode px(FALSE, 0, _T("Settings"));
	// CXTPPropExchangeSection pxCommandBars(px.GetSection(_T("CommandBars")));
	// GetCommandBars()->DoPropExchange(&pxCommandBars, &param);
	// px.SaveToFile(m_strIniFileName);
	// </code>
	// See Also: CXTPPropExchange, XTP_COMMANDBARS_PROPEXCHANGE_PARAM
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX, XTP_COMMANDBARS_PROPEXCHANGE_PARAM* pParam = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to load designer command bars from
	//     an archive.
	// Parameters:
	//     ar - A CArchive object to serialize to or from.
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//-----------------------------------------------------------------------
	void LoadDesignerBars(CArchive& ar);
	void LoadDesignerBars(CXTPPropExchange* pPX); // <combine CXTPCommandBars::LoadDesignerBars@CArchive&>

	//# Intelligent menus routines

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function adds the specified commands to the list of
	//     menu items to hide until activated by clicking on the chevron.
	// Parameters:
	//     pCommands - An array of command IDs, of menu items, to hide.
	//     nCount    - Size of the array passed in.
	// Remarks:
	//     The command will not be displayed in popup menus until the user
	//     clicks on the "expand menu" chevron or after a short delay when
	//     bShowFullAfterDelay is TRUE.  Note:  bAlwaysShowFullMenus must set
	//     to FALSE to hide the hidden commands
	//-----------------------------------------------------------------------
	void HideCommands(const UINT* pCommands, int nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function adds the specified command to the list of
	//     menu items to hide until activated by clicking on the chevron.
	// Parameters:
	//     nCommand - The command ID of a menu item to hide.
	//-----------------------------------------------------------------------
	void HideCommand(UINT nCommand);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve if the command is hidden
	//     until activated by clicking on the chevron.  I.e. Infrequently
	//     used commands.
	// Parameters:
	//     nCommand - Command id to check.
	// Remarks:
	//     The command will not be displayed in popup menus until the user
	//     clicks on the "expand menu" chevron or after a short delay when
	//     bShowFullAfterDelay is TRUE.  Note:  bAlwaysShowFullMenus must set
	//     to FALSE to hide the hidden commands
	// Returns:
	//     TRUE if the command item is hidden; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsCommandHidden(UINT nCommand);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve if the control is hidden and of type
	//     CXTPControlPopup when bAutoHideUnusedPopups is TRUE.
	// Parameters:
	//     pControl - Control pointer to check.
	// Remarks:
	//     If bAutoHideUnusedPopups is FALSE, then this member performs the same
	//     action as IsCommandHidden. Override this member to provide custom intelligent menu scheme.
	// Returns:
	//     TRUE if the control item is hidden; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsControlHidden(CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the command activated by user.
	// Parameters:
	//     nCommand - Command id to set
	//-----------------------------------------------------------------------
	void SetCommandUsed(UINT nCommand);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to restore the state of the hidden
	//     commands.
	//-----------------------------------------------------------------------
	void ResetUsageData();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves active MDI document template
	// Returns:
	//     Active document template identifier.
	//-----------------------------------------------------------------------
	virtual UINT GetActiveDocTemplate();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the application’s command bars into and out of print-preview mode.
	// Paramets:
	//     bPreview - Specifies whether or not to place the application in print-preview mode. Set to TRUE to place in print preview, FALSE to cancel preview mode.
	//-----------------------------------------------------------------------
	void OnSetPreviewMode (BOOL bPreview);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to process frame messages.
	//     CXTPFrameWndBase derived classes call this method automatically.
	// Parameters:
	//     pMsg - Points to an MSG object
	// Returns:
	//     TRUE if the message was handled; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL PreTranslateFrameMessage(MSG* pMsg);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to process frame messages.
	//     CXTPFrameWndBase derived classes call this method automatically.
	// Parameters:
	//     message - Specifies the message to be sent.
	//     wParam  - Specifies additional message-dependent information.
	//     lParam  - Specifies additional message-dependent information.
	//     pResult - The return value of WindowProc. Depends on the message;
	//               may be NULL.
	// Returns:
	//     TRUE if the message was handled; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL OnFrameWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method shows the context menu for toolbars.
	// Parameters:
	//     point - Position of the menu to show.
	//-----------------------------------------------------------------------
	virtual void ContextMenu(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method creates a context menu for toolbars.
	// Returns:
	//     A CXTPPopupBar object containing list of toolbars.
	//-----------------------------------------------------------------------
	virtual CXTPPopupBar* GetToolbarsPopup();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to create an expand bar.
	// Parameters:
	//     pToolBar - Points to a CXTPToolBar object
	//     pExpandBar - Points to a CXTPCommandBar object
	//-----------------------------------------------------------------------
	virtual void GetHiddenControls(CXTPToolBar* pToolBar, CXTPCommandBar* pExpandBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to create customized controls.
	// Parameters:
	//     pToolBar   - Points to a CXTPToolBar object
	//     pExpandBar - Points to a CXTPCommandBar object
	//-----------------------------------------------------------------------
	virtual void GetAddOrRemovePopup(CXTPToolBar* pToolBar, CXTPCommandBar* pExpandBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to set the site window.
	// Parameters:
	//     pFrame - Points to a CWnd object
	//-----------------------------------------------------------------------
	void SetSite(CWnd* pFrame);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve dock bar in specified point.
	// Parameters:
	//     pt - Point needed to test.
	//     pMainDock - CXTPDockBar class currently docked in.
	// Returns:
	//     A pointer to a CXTPDockBar object.
	//-----------------------------------------------------------------------
	CXTPDockBar* CanDock(CPoint pt, CXTPDockBar* pMainDock = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member creates command bars.
	// Returns:
	//     A pointer to a CXTPCommandBars object
	//-----------------------------------------------------------------------
	static CXTPCommandBars* AFX_CDECL CreateCommandBars();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to create dock bars.
	//-----------------------------------------------------------------------
	void EnableDocking(void);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get toolbars count.
	// Returns:
	//     Number of the toolbars.
	//-----------------------------------------------------------------------
	int GetCount();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve a toolbar with the specified index.
	// Parameters:
	//     nIndex - Index of the toolbar.
	// Returns:
	//     A pointer to a CXTPToolBar object
	//-----------------------------------------------------------------------
	CXTPToolBar* GetAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the  customize mode of the command
	//     bars.
	// Returns:
	//     TRUE if command bars in customize mode; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL IsCustomizeMode() { return m_bCustomizeMode;}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set customize mode of the command bars.
	// Parameters:
	//     bMode - TRUE to set customize mode.
	//-----------------------------------------------------------------------
	void SetCustomizeMode(BOOL bMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the  customize mode of the command
	//     bars.
	// Returns:
	//     TRUE if command bars in customize mode; otherwise returns FALSE
	// See Also: SetQuickCustomizeMode
	//-----------------------------------------------------------------------
	BOOL IsQuickCustomizeMode() { return m_bQuickCustomizeMode;}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set customize mode of the command bars.
	// Parameters:
	//     bMode - TRUE to set customize mode.
	// See Also: IsQuickCustomizeMode
	//-----------------------------------------------------------------------
	void SetQuickCustomizeMode(BOOL bMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the CXTPCustomizeDropSource helper
	//     class for customization.
	// Returns:
	//     A pointer to a CXTPCustomizeDropSource object
	//-----------------------------------------------------------------------
	CXTPCustomizeDropSource* GetDropSource();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when customized control changed.
	// Parameters:
	//     pControl - Points to a CXTPControl object
	//-----------------------------------------------------------------------
	void SetDragControl(CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the current customized control.
	// Returns:
	//     A pointer to a CXTPControl object
	//-----------------------------------------------------------------------
	CXTPControl* GetDragControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve if the customize is avail.
	// Returns:
	//     TRUE if customize avail; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL IsCustomizeAvail();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove the toolbar.
	// Parameters:
	//     pToolBar - Points to a CXTPToolBar object to remove.
	//-----------------------------------------------------------------------
	void Remove(CXTPToolBar* pToolBar);
	
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove all toolbars.
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to dock a toolbar to the specified dockbar.
	// Parameters:
	//     pBar     - Points to a CXTPToolBar object
	//     lpRect   - Position to dock.
	//     pDockBar - Dock bar needed to be docked.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL DockCommandBar(CXTPToolBar* pBar, LPRECT lpRect, CXTPDockBar* pDockBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the specified paint manager.
	// Parameters:
	//     pPaintManager - Points to a CXTPPaintManager object
	//-----------------------------------------------------------------------
	void SetPaintManager(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the specified image manager.
	// Parameters:
	//     pImageManager - Points to a CXTPPaintManager object
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the paint manager of the command
	//     bars.
	// Returns:
	//     A pointer to a CXTPPaintManager object
	//-----------------------------------------------------------------------
	CXTPPaintManager* GetPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the image manager of the command
	//     bars.
	// Returns:
	//     A pointer to a CXTPImageManager object
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to show/hide the toolbar.
	// Parameters:
	//     nIndex - Index of the toolbar to show or hide.
	//-----------------------------------------------------------------------
	void ToggleVisible(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set customization file path
	// Parameters:
	//     lpszFile - Path to customization file information
	// Remarks:
	//     CommandBars can't store to registry customization information greater than 16kb, so it
	//     creates file with user customization information.
	//     If file not specified current directory will be used to save customization.
	// Example:
	// <code>
	// if (GetAppDataFolder(strAppDataFolder))
	// {
	//     CreateDirectory(strAppDataFolder + _T("\\") + AfxGetApp()->m_pszAppName, NULL);
	//
	//      // Set customization path as "\\Documents and Settings\\user\\Application Data\\CustomThemes\\CommandBars"
	//      GetCommandBars()->SetCustomizationDataFileName(strAppDataFolder + _T("\\") + AfxGetApp()->m_pszAppName + _T("\\") + lpszProfileName);
	// }
	// </code>
	//-----------------------------------------------------------------------
	void SetCustomizationDataFileName(LPCTSTR lpszFile);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method finds rebar of the frame
	// Returns:
	//     CXTPReBar pointer if rebar was found; NULL otherwise.
	//-----------------------------------------------------------------------
	CXTPReBar* GetFrameReBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set default toolbar class
	// Parameters:
	//     pToolBarClass - Runtime class of toolbars will be added.
	// Example:
	// <code>
	// class CSystemToolBar : public CXTPToolBar
	// {
	//     DECALRE_DYNCREATE(CSystemToolBar)
	//
	//     ....
	//
	// }
	//
	// pCommandBars->SetToolBarClass(RUNTIME_CLASS(CSystemToolBar));
	//
	// CXTPToolBar* pToolBar = pCommandBars->Add("New ToolBar", xtpBarTop);
	// ASSERT_KINDOF(CSystemToolBar, pToolBar);
	// </code>
	// See Also: SetMenuBarClass, SetPopupBarClass
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetToolBarClass(CRuntimeClass* pToolBarClass);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set default menubar class
	// Parameters:
	//     pMenuBarClass - Runtime class of menu will be added.
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetMenuBarClass(CRuntimeClass* pMenuBarClass);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set default popupbar class
	// Parameters:
	//     pPopupBarClass - Runtime class of popups will be added.
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetPopupBarClass(CRuntimeClass* pPopupBarClass);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to float a CXTPToolBar.
	// Parameters:
	//     pBar - Pointer to a valid CXTPToolBar object.
	// Returns:
	//     TRUE if the CXTPToolBar was successfully floated, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL FloatCommandBar(CXTPToolBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to dock a CXTPToolBar to a specified position
	// Parameters:
	//     pBar        - Toolbar to dock.
	//     barPosition - XTPBarPosition to dock the toolbar.
	// Returns:
	//     TRUE if the toolbar was successfully docked, FALSE if the bar
	//     could not be docked.
	//-----------------------------------------------------------------------
	BOOL DockCommandBar(CXTPToolBar* pBar, XTPBarPosition barPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to store state information about the
	//     frame window's control bars in a CXTPDockState object.
	// Parameters:
	//     state - Contains the current state of the frame window's control
	//             bars upon return.
	//-----------------------------------------------------------------------
	void GetDockState(CXTPDockState& state);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to apply state information stored in a
	//     CXTPDockState object to the frame window's control bars.
	// Parameters:
	//     state - Apply the stored state to the frame window's control bars.
	//-----------------------------------------------------------------------
	void SetDockState(CXTPDockState& state);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when new toolbar was added.
	// Parameters:
	//     pToolBar - Pointer to new toolbar.
	//-----------------------------------------------------------------------
	virtual void OnToolBarAdded(CXTPToolBar* pToolBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when new toolbar was removed
	// Parameters:
	//     pToolBar - Pointer to toolbar was removed.
	//-----------------------------------------------------------------------
	virtual void OnToolBarRemoved(CXTPToolBar* pToolBar);

private:
	void SaveDockBarsState(LPCTSTR lpszProfileName);
	void LoadDockBarsState(LPCTSTR lpszProfileName);
	void SerializeDockBarsState(CXTPPropExchange* pPX);
	void GenerateCommandBarList(CXTPCommandBarList* pCommandBarList, XTP_COMMANDBARS_PROPEXCHANGE_PARAM* pParam);
	void SerializeCommandBars(CArchive& ar);
	BOOL SaveCommandBarList(CXTPPropExchange* pPX, CXTPCommandBarList* pCommandBarList);
	BOOL LoadCommandBarList(CXTPPropExchange* pPX, CXTPCommandBarList* pCommandBarList);
	void RestoreCommandBarList(CXTPCommandBarList* pCommandBarList, BOOL bSilent = FALSE);
	CXTPToolBar* AddCustomBar(LPCTSTR lpcstrCaption, UINT nID, BOOL bTearOffBar = FALSE);
	void _GetAddOrRemovePopup(CXTPToolBar* pToolBar, CXTPCommandBar* pExpandBar);
	void _LoadControlsPart(CFile& file, XTP_COMMANDBARS_PROPEXCHANGE_PARAM* pParam);
	BOOL GetControlsFileName(CString& strFileName, LPCTSTR lpszProfileName);
	CDocTemplate* FindDocTemplate(CMDIChildWnd* pChild);
	CWnd* GetForegroundFrame();

public:
	CXTPControls* m_pDesignerControls;      // Designer controls if used.
	HACCEL m_hDesignerAccels;               // Designed accelerators.
	BOOL m_bIgnoreShiftMenuKey;             // if set menus will ignore Shift+Alt key combinations

protected:
	CXTPCustomizeDropSource* m_pDropSource; // Customize helper class.

	CXTPPaintManager* m_pPaintManager;      // Paint manager.
	CXTPImageManager* m_pImageManager;      // Image manager

	CXTPControl* m_pDragSelected;           // Customize control.
	BOOL m_bCustomizeMode;                  // Customize mode.
	BOOL m_bCustomizeAvail;                 // TRUE if customize available.
	int  m_nDefaultMenuID;                  // Default menu resource.

	BOOL m_bQuickCustomizeMode;             // TRUE if command bars in quick customization mode
	CString m_strControlsFileName;          // File name to be used to store user customization.

	CXTPCommandBarsOptions* m_pOptions;     // Command bars' options

	CList <CXTPToolBar*, CXTPToolBar*> m_lstBars;   // Toolbars list.
	CXTPDockBar* m_pDocks[4];               // DockBars list
	CWnd* m_pFrame;                         // Parent frame of command bars
	CXTPMenuBar* m_pMenuBar;                // Active menu bar.

	static CRuntimeClass* m_pToolBarClass;  // Default toolbar class
	static CRuntimeClass* m_pMenuBarClass;  // Default menubar class
	static CRuntimeClass* m_pPopupBarClass; // Default popupbar class

private:
	BOOL m_bRecalcLayout;


private:
	DECLARE_DYNCREATE(CXTPCommandBars)

	friend class CXTPCommandBar;
	friend class CXTPToolBar;
	friend class CXTPDockContext;
	friend class CXTPDockState;
	friend class CXTPControlToolbars;
	friend class CXTPControl;
	friend class CCommandBarsCtrl;
	friend class CXTPCustomizeToolbarsPage;
	friend class CXTPPopupBar;
	friend class CXTPCommandBarsOptions;



};

//////////////////////////////////////////////////////////////////////////


AFX_INLINE CXTPMenuBar* CXTPCommandBars::GetMenuBar() {
	return m_pMenuBar;
}
AFX_INLINE CWnd* CXTPCommandBars::GetSite() {
	return m_pFrame;
}
AFX_INLINE CXTPDockBar* CXTPCommandBars::GetDockBar(XTPBarPosition xtpPosition) {
	return m_pDocks[xtpPosition];
}
AFX_INLINE int CXTPCommandBars::GetCount() {
	return (int)m_lstBars.GetCount();
}
AFX_INLINE CXTPCustomizeDropSource* CXTPCommandBars::GetDropSource() {
	return m_pDropSource;
}
AFX_INLINE CXTPControl* CXTPCommandBars::GetDragControl() {
	return  m_pDragSelected;
}
AFX_INLINE BOOL CXTPCommandBars::IsCustomizeAvail() {
	return m_bCustomizeAvail;
}
AFX_INLINE CXTPCommandBarsOptions* CXTPCommandBars::GetCommandBarsOptions() {
	return m_pOptions;
}
AFX_INLINE void CXTPCommandBars::SetCustomizationDataFileName(LPCTSTR lpszFile) {
	m_strControlsFileName = lpszFile;
}
AFX_INLINE void CXTPCommandBars::SetToolBarClass(CRuntimeClass* pToolBarClass) {
	m_pToolBarClass = pToolBarClass;
}
AFX_INLINE void CXTPCommandBars::SetMenuBarClass(CRuntimeClass* pMenuBarClass) {
	m_pMenuBarClass = pMenuBarClass;
}
AFX_INLINE void CXTPCommandBars::SetPopupBarClass(CRuntimeClass* pPopupBarClass) {
	m_pPopupBarClass = pPopupBarClass;
}

#endif //#if !defined(__XTPCOMMANDBARS_H__)
