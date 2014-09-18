// XTPControl.h : interface for the CXTPControl class.
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
#if !defined(__XTPCONTROL_H__)
#define __XTPCONTROL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPCommandBarsDefines.h"
#include "CommonPro.include/XTPManageState.h"

class CXTPControls;
class CXTPCommandBar;
class CXTPImageManager;
class CXTPImageManagerIcon;
class CXTPPaintManager;
class CXTPPropExchange;


//===========================================================================
// Summary:
//     CXTPControl is a CCmdTarget derived class. It represents the parent
//     class for the command bar's controls.
//===========================================================================
class _XTP_EXT_CLASS CXTPControl : public CCmdTarget
{
protected:
//{{AFX_CODEJOCK_PRIVATE
	class CDocTemplateMap : public CMap<UINT, UINT, BOOL, BOOL>
	{
	public:
		void Copy(CDocTemplateMap& map);
	};
//}}AFX_CODEJOCK_PRIVATE


protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControl object
	//-----------------------------------------------------------------------
	CXTPControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPControl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the style of the control.
	// Parameters:
	//     buttonStyle - The style to be set. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     buttonStyle parameter can be one of the following:
	//         * <b>xtpButtonAutomatic</b> Indicates the default style.
	//         * <b>xtpButtonCaption</b> Indicates caption drawing only.
	//         * <b>xtpButtonIcon</b> Indicates icon drawing only.
	//         * <b>xtpButtonIconAndCaption</b> Indicates icon and caption drawing.
	//-----------------------------------------------------------------------
	void SetStyle(XTPButtonStyle buttonStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the control's style.
	// Returns:
	//     The style of the control.
	//-----------------------------------------------------------------------
	XTPButtonStyle GetStyle();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the caption of the control.
	// Parameters:
	//     strCaption - Caption to be set.
	// See Also: SetPrompt, SetDescription, SetTooltip
	//-----------------------------------------------------------------------
	void SetCaption(CString strCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the caption of the control.
	// Parameters:
	//     nIDCaption - Caption resource identifier.
	//-----------------------------------------------------------------------
	void SetCaption(UINT nIDCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the control's caption.
	// Returns:
	//     The caption of the control.
	//-----------------------------------------------------------------------
	CString GetCaption();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the shortcut string to the control.
	// Parameters:
	//     strShortcutText - Shortcut to be set.
	//-----------------------------------------------------------------------
	void SetShortcutText(CString strShortcutText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the control's shortcut.
	// Returns:
	//     The shortcut of the control.
	//-----------------------------------------------------------------------
	CString GetShortcutText();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the description of the control.
	// Parameters:
	//     strDescription - Description to be set.
	// See Also: SetCaption, SetPrompt, SetTooltip
	//-----------------------------------------------------------------------
	void SetDescription(CString strDescription);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the control's description.
	// Returns:
	//     The description of the control.
	//-----------------------------------------------------------------------
	CString GetDescription();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the tooltip of the control.
	// Parameters:
	//     strTooltip - Tooltip to be set.
	// See Also: SetCaption, SetDescription, SetPrompt
	//-----------------------------------------------------------------------
	void SetTooltip(CString strTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the control's tooltip.
	// Parameters:
	//     pPoint    - Points to a POINT structure to receive xy coordinates.
	//     lpRectTip - Points to a RECT structure to receive size information.
	//     nHit      - Specifies the hit-test area code.
	// Returns:
	//     Tooltip of the control.
	//-----------------------------------------------------------------------
	virtual CString  GetTooltip(LPPOINT pPoint = 0, LPRECT lpRectTip = 0, int* nHit = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the parameter of the control.
	// Parameters:
	//     strParameter - Parameter to be set.
	// Remarks:
	//     This method sets CString value associated with the control, use SetTag to set numeric parameter.
	// See Also: SetTag, GetParameter
	//-----------------------------------------------------------------------
	void SetParameter(CString strParameter);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the control's parameter.
	// Returns:
	//     The parameter of the control.
	// See Also: GetTag, SetParameter
	//-----------------------------------------------------------------------
	CString GetParameter();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the identifier of the control.
	// Parameters:
	//     nId - Identifier to be set.
	//-----------------------------------------------------------------------
	void SetID(int nId);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the control's identifier.
	// Returns:
	//     The identifier of the control.
	//-----------------------------------------------------------------------
	int GetID();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the index of the control.
	// Returns:
	//     The index of the control.
	//-----------------------------------------------------------------------
	long GetIndex();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the control icon's identifier.
	// Parameters:
	//     nId - Icon's identifier to be set.
	//-----------------------------------------------------------------------
	void SetIconId(int nId);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the icon's identifier.
	// Returns:
	//     An icon's identifier of the control.
	//-----------------------------------------------------------------------
	int GetIconId();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to begin a new group using a separator.
	// Parameters:
	//     bBeginGroup - TRUE if the control starts a new group.
	//-----------------------------------------------------------------------
	virtual void SetBeginGroup(BOOL bBeginGroup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the control has a separator.
	// Returns:
	//     TRUE if the control starts a new group; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetBeginGroup();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable or disable the control.
	//     If the control does not have xtpFlagManualUpdate flag, you must call
	//     the Enable member of CCmdUI in the ON_UPDATE_COMMAND_UI handler.
	// Parameters:
	//     bEnabled - TRUE if the control is enabled.
	// See Also: GetEnabled, SetChecked
	//-----------------------------------------------------------------------
	virtual void SetEnabled(BOOL bEnabled);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the state of the control.
	// Returns:
	//     TRUE if the control is enabled; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetEnabled();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to check the control.
	//     If the control does not have xtpFlagManualUpdate flag, you must call
	//     the SetCheck member of CCmdUI in the ON_UPDATE_COMMAND_UI handler.
	// Parameters:
	//     bChecked - TRUE if the control is checked.
	// See Also: GetChecked, SetEnabled
	//-----------------------------------------------------------------------
	void SetChecked(BOOL bChecked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the state of the control.
	// Returns:
	//     TRUE if the control is checked; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetChecked();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the state of the control.
	// Returns:
	//     TRUE if the control is selected; otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual int GetSelected();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to press the control.
	// Parameters:
	//     bPressed - TRUE if the control is pressed.
	//-----------------------------------------------------------------------
	void SetPressed(BOOL bPressed);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the state of the control.
	// Returns:
	//     TRUE if the control is pressed; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetPressed();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to set the bounding rectangle of the control.
	// Parameters:
	//     rcControl - Bounding rectangle of the control.
	//-----------------------------------------------------------------------
	virtual void SetRect(CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the bounding rectangle of the control.
	// Returns:
	//     The bounding rectangle of the control.
	//-----------------------------------------------------------------------
	CRect GetRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the parent command bar.
	// Returns:
	//     The parent command bar object.
	//-----------------------------------------------------------------------
	CXTPCommandBar* GetParent();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the control is visible.
	// Returns:
	//     TRUE if the control is visible.
	//-----------------------------------------------------------------------
	BOOL IsVisible();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to change the state of the control.
	// Parameters:
	//     bVisible - TRUE if the control is visible.
	//-----------------------------------------------------------------------
	void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the parent controls collection.
	// Returns:
	//     A CXTPControls pointer to the parent controls.
	//-----------------------------------------------------------------------
	CXTPControls* GetControls();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the control's type
	// Returns:
	//     The type of the control. It can be one of the following:
	//         * <b>xtpControlButton</b> Indicates the control is simple button (CXTPControlButton)
	//         * <b>xtpControlPopup</b> Indicates the control is simple popup button. (CXTPControlPopup)
	//         * <b>xtpControlButtonPopup</b> Indicates the control is popup button with icon (CXTPControlPopup)
	//         * <b>xtpControlSplitButtonPopup</b> Indicates the control is split button popup.(CXTPControlPopup)
	//         * <b>xtpControlComboBox</b> Indicates the control is combo box (CXTPControlComboBox)
	//         * <b>xtpControlEdit</b> Indicates the control is edit control (CXTPControlEdit)
	//         * <b>xtpControlLabel</b> Indicates the control is label (CXTPControlLabel)
	//-----------------------------------------------------------------------
	XTPControlType GetType();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the flags of the control.
	// Parameters:
	//     dwFlags - control flags.
	// Remarks:
	//     Flags to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:
	//         * <b>xtpFlagRightAlign</b> Indicates the control is right aligned.
	//         * <b>xtpFlagSkipFocus</b> Indicates the control does not have focus.
	//         * <b>xtpFlagLeftPopup</b> Indicates the child bar should pop-up on the left.
	//         * <b>xtpFlagManualUpdate</b> Indicates the control is manually updated.
	//         * <b>xtpFlagNoMovable</b> Indicates the control's customization is disabled.
	//         * <b>xtpFlagControlStretched</b> Indicates the control is stretched in the parent command bar.
	// See Also: GetFlags, XTPControlFlags
	//-----------------------------------------------------------------------
	void SetFlags(DWORD dwFlags);

	//----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve control special flags.
	// Returns:
	//     Controls special flags.
	// See Also: SetFlags, XTPControlFlags
	//----------------------------------------------------------------------
	DWORD GetFlags();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the 32-bit value associated with the command bar.
	// Parameters:
	//     dwTag - Contains the new value to associate with the command bar.
	//-----------------------------------------------------------------------
	void SetTag(DWORD_PTR dwTag);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves the application-supplied 32-bit value
	//     associated with the command bar.
	// Returns:
	//     The 32-bit value associated with the command bar.
	//-----------------------------------------------------------------------
	DWORD_PTR GetTag();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to compare controls.
	// Parameters:
	//     pOther - The control need compare with.
	// Returns:
	//     TRUE if the controls are identical.
	//-----------------------------------------------------------------------
	virtual BOOL Compare(CXTPControl* pOther);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the child command bar.
	// Returns:
	//     A pointer to the child command bar.
	//-----------------------------------------------------------------------
	virtual CXTPCommandBar* GetCommandBar();

	//----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable closing of the
	//     sub menus when a control is clicked in the menu.
	// Parameters:
	//     bCloseOnClick - TRUE if the sub menu will close when a control
	//                     is clicked, FALSE if the menu will remain open
	//                     after a control is clicked.
	// See Also: GetCloseSubMenuOnClick
	//----------------------------------------------------------------------
	void SetCloseSubMenuOnClick(BOOL bCloseOnClick);

	//----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if sub menus will close
	//     as soon as a control is clicked.
	// Returns:
	//     TRUE if the sub menu will close when a control is clicked, FALSE
	//     if the menu will remain open after a control is clicked.
	// See Also: SetCloseSubMenuOnClick
	//----------------------------------------------------------------------
	BOOL GetCloseSubMenuOnClick();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine of the control is a label.
	// Returns:
	//     TRUE if the control is a label, FALSE otherwise.  Controls of
	//     type CXTPControlLabel will always return TRUE.  All others will
	//     be FALSE.
	// See Also: CXTPControlLabel
	//-----------------------------------------------------------------------
	virtual BOOL IsLabel();

public:

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to hide the control.
	// Parameters:
	//     bHide - TRUE to set hide state.
	//----------------------------------------------------------------------
	void SetHideWrap(BOOL bHide);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to hide the control.
	// Parameters:
	//     dwFlags - Reasons to hide.
	// Remarks:
	//     HideFlags will tell how a control was hidden if it is hidden
	//     or why it is not visible.  Most of these flags are read only
	//     and should not be set be the developer.
	//     There is only one situation where you should set these flags
	//     manually.  The xtpHideCustomize is the only flag that should
	//     ever be manually set. you would do this when you want the
	//     control to initially be hidden but still exist in customize
	//     popup of toolbar.  This will cause the control to appear in
	//     the Add and Remove Buttons popup without a check mark indicating
	//     that it is currently hidden.  This flag will automatically
	//     be set when a command is hidden this way.
	// See Also: XTPControlHideFlags
	//----------------------------------------------------------------------
	virtual void SetHideFlags(DWORD dwFlags);

	//----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve hidden flags.
	// Returns:
	//     Flags of the control visibility.
	// Remarks:
	//     HideFlags will tell how a control was hidden if it is hidden
	//     or why it is not visible.  Most of these flags are read only
	//     and should not be set be the developer.
	//     There is only one situation where you should set these flags
	//     manually.  The xtpHideCustomize is the only flag that should
	//     ever be manually set. you would do this when you want the
	//     control to initially be hidden but still exist in customize
	//     popup of toolbar.  This will cause the control to appear in
	//     the Add and Remove Buttons popup without a check mark indicating
	//     that it is currently hidden.  This flag will automatically
	//     be set when a command is hidden this way.
	// See Also: XTPControlHideFlags
	//----------------------------------------------------------------------
	DWORD GetHideFlags();

	//----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the position of the control's row.
	// Returns:
	//     Bounding rectangle of the control's row.
	//----------------------------------------------------------------------
	CRect GetRawRect();

	//----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve if the control is wrapped.
	// Returns:
	//     TRUE if the control is wrapped; otherwise returns FALSE
	// See Also: SetWrap
	//----------------------------------------------------------------------
	BOOL GetWrap();

	//----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the row of the control.
	// Parameters:
	//     rcRaw - Rectangle of the control's row.
	//----------------------------------------------------------------------
	void SetRawRect(CRect rcRaw);

	//----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the wrap.
	// Parameters:
	//     bWrap - TRUE to set wrap.
	// See Also: GetWrap
	//----------------------------------------------------------------------
	void SetWrap(BOOL bWrap);

	//----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve if the control is expanded.
	// Returns:
	//     TRUE if the control is expanded; otherwise returns FALSE
	//----------------------------------------------------------------------
	BOOL GetExpanded();

	//----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the mouse pointer is over
	//     the control.
	// Returns:
	//     TRUE if the mouse cursor is over the control; otherwise returns
	//     FALSE
	//----------------------------------------------------------------------
	BOOL IsCursorOver();

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called when the control is executed.
	//----------------------------------------------------------------------
	virtual void OnExecute();

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine the size of the control.
	// Parameters:
	//     pDC - Pointer to a valid device context
	// Returns:
	//     Size of the control.
	//----------------------------------------------------------------------
	virtual CSize GetSize(CDC* pDC);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called before recalculating the parent command
	//     bar size to calculate the dimensions of the control.
	// Parameters:
	//     dwMode - Flags used to determine the height and width of the
	//              dynamic command bar. See Remarks section for a list of
	//              values.
	// Remarks:
	//     The following predefined flags are used to determine the height and
	//     width of the dynamic command bar. Use the bitwise-OR (|) operator to
	//     combine the flags.<p/>
	//
	//     * <b>LM_STRETCH</b> Indicates whether the command bar should be
	//                stretched to the size of the frame. Set if the bar is
	//                not a docking bar (not available for docking). Not set
	//                when the bar is docked or floating (available for
	//                docking). If set, LM_STRETCH returns dimensions based
	//                on the LM_HORZ state. LM_STRETCH works similarly to
	//                the the bStretch parameter used in CalcFixedLayout;
	//                see that member function for more information about
	//                the relationship between stretching and orientation.
	//     * <b>LM_HORZ</b> Indicates that the bar is horizontally or
	//                vertically oriented. Set if the bar is horizontally
	//                oriented, and if it is vertically oriented, it is not
	//                set. LM_HORZ works similarly to the the bHorz
	//                parameter used in CalcFixedLayout; see that member
	//                function for more information about the relationship
	//                between stretching and orientation.
	//     * <b>LM_MRUWIDTH</b> Most Recently Used Dynamic Width. Uses the
	//                remembered most recently used width.
	//     * <b>LM_HORZDOCK</b> Horizontal Docked Dimensions. Returns the
	//                dynamic size with the largest width.
	//     * <b>LM_VERTDOCK</b> Vertical Docked Dimensions. Returns the dynamic
	//                size with the largest height.
	//     * <b>LM_COMMIT</b> Resets LM_MRUWIDTH to current width of
	//                floating command bar.
	//
	//     The framework calls this member function to calculate the dimensions
	//     of a dynamic command bar.<p/>
	//
	//     Override this member function to provide your own layout in classes
	//     you derive from CXTPControl. XTP classes derived from CXTPControl,
	//     such as CXTPControlComboBox, override this member function to provide
	//     their own implementation.
	// See Also:
	//     CXTPControlComboBox, CXTPControlCustom, CXTPControlEdit,
	//     CXTPControlWindowList, CXTPControlWorkspaceActions, CXTPControlToolbars,
	//     CXTPControlOleItems, CXTPControlRecentFileList, CXTPControlSelector,
	//     CXTPControlListBox
	//----------------------------------------------------------------------
	virtual void OnCalcDynamicSize(DWORD dwMode) { UNREFERENCED_PARAMETER(dwMode); };

	//----------------------------------------------------------------------
	// Summary:
	//     Call this method to redraw the parent command bar.
	//----------------------------------------------------------------------
	void DelayRedrawParent();

	//----------------------------------------------------------------------
	// Summary:
	//     Call this method to recalculate the parent command bar size.
	//----------------------------------------------------------------------
	void DelayLayoutParent();

	//----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the category of the control.
	// Parameters:
	//     strCategory - Category of the control.
	//----------------------------------------------------------------------
	void SetCategory(CString strCategory);

	//----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the category of the control.
	// Returns:
	//     Category of the control.
	//----------------------------------------------------------------------
	CString GetCategory();

	//----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the image manager.
	// Returns:
	//     A pointer to a CXTPImageManager object.
	//----------------------------------------------------------------------
	CXTPImageManager* GetImageManager();

	//----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the paint manager.
	// Returns:
	//     A pointer to a CXTPPaintManager object.
	//----------------------------------------------------------------------
	CXTPPaintManager* GetPaintManager();

	//----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the image of the control.
	// Parameters:
	//     nWidth - Width of the image to retrieve.
	// Returns:
	//     A pointer to a CXTPImageManagerIcon object.
	//----------------------------------------------------------------------
	virtual CXTPImageManagerIcon* GetImage(int nWidth = 16);

	//----------------------------------------------------------------------
	// Summary:
	//     Call this method to assign a document template to the control.
	//     Assigned control will be shown only when the child MDI window is activated
	//     with the assigned document template.
	// Parameters:
	//     nIDResource - document template identifier.
	// See Also: AssignDocTemplate
	//----------------------------------------------------------------------
	void AssignDocTemplate(UINT nIDResource);

	//----------------------------------------------------------------------
	// Summary:
	//     Call this method to exclude the document template from control.
	//     Control will be shown only when the child MDI window is activated
	//     with the document template different from excluded.
	// Parameters:
	//     nIDResource - document template identifier.
	// See Also: ExcludeDocTemplate
	//----------------------------------------------------------------------
	void ExcludeDocTemplate(UINT nIDResource);

	//----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve if control is default menu item.
	// Returns:
	//     TRUE if control is default menu item.
	// See Also: SetItemDefault
	//----------------------------------------------------------------------
	BOOL IsItemDefault();

	//----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the default menu item.
	// Parameters:
	//     bDefault - TRUE to set control default menu item.
	// Remarks:
	//     A default menu item will appear in bold text.
	// See Also: IsItemDefault
	//----------------------------------------------------------------------
	void SetItemDefault(BOOL bDefault);

	//----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if control is temporary.
	//     Temporary controls will not be saved to the system registry when
	//     the application is closed (If the CommandBars layout is saved).
	// Returns:
	//     TRUE if control is temporary.
	//----------------------------------------------------------------------
	BOOL IsTemporary();

	//----------------------------------------------------------------------
	// Summary:
	//     Call this member to reset the icon and caption of a control
	//     to its default settings before it was customized by the user.
	// Remarks:
	//     The caption and icon of a control is customized while in customization
	//     mode.
	// See Also:
	//     SetCustomIcon, m_strCustomCaption
	//----------------------------------------------------------------------
	void Reset();

	//----------------------------------------------------------------------
	// Summary:
	//     Call this member to load a string from a STRINGTABLE resource
	//     to set the tooltip and description of a control.
	// Parameters:
	//     strPrompt -  String from resource file that contains the
	//                  tooltip and description of the item.  The Caption
	//                  will be set to use the tooltip text.  The tooltip
	//                  and description must be separated by "\n" if both
	//                  are included.  You can omit the description and
	//                  both the tooltip and description will be set to
	//                  the same value.
	// Remarks:
	//     Resource strings are in the format "ToolTip\\nDescription".
	//
	//     Note: The caption is set to the value of the tooltip when this
	//           member is called.
	// See Also: SetCaption, SetDescription, SetTooltip
	//----------------------------------------------------------------------
	void SetPrompt(const CString& strPrompt);

	// -------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve CXTPControl from CCmdUI structure.
	// Parameters:
	//     pCmdUI :  Address of a CCmdUI structure.
	// Returns:
	//     CXTPControl from CCmdUI structure.
	// -------------------------------------------------------------------
	static CXTPControl* AFX_CDECL FromUI(CCmdUI* pCmdUI);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set a custom icon to this control.
	// Parameters:
	//     hIcon - HICON object containing icon to set to control.
	// See Also: Reset
	//-----------------------------------------------------------------------
	void SetCustomIcon(HICON hIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

protected:

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to assign the order of the control in
	//     the controls list.
	// Parameters:
	//     nIndex - Index of the control.
	//----------------------------------------------------------------------
	void SetIndex(int nIndex) { m_nIndex = nIndex; }

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to assign expanded state to the control.
	// Parameters:
	//     bExpanded - TRUE if the control is expanded.
	//----------------------------------------------------------------------
	void SetExpanded(BOOL bExpanded);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called when a non-system key is pressed.
	// Parameters:
	//     nChar - Specifies the virtual key code of the given key.
	// Returns:
	//     TRUE if key handled, otherwise returns FALSE
	//----------------------------------------------------------------------
	virtual BOOL OnKeyDown(UINT nChar);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to assign a parent command bar object.
	// Parameters:
	//     pParent - Points to a CXTPCommandBar object
	//----------------------------------------------------------------------
	virtual void SetParent(CXTPCommandBar* pParent) { m_pParent = pParent;}

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to redraw the parent command bar.
	//----------------------------------------------------------------------
	void RedrawParent();

	//----------------------------------------------------------------------
	// Summary:
	//     The ScreenToClient function converts the screen coordinates
	//     of a specified point on the screen
	// Parameters:
	//     point - Pointer to a CPoint object that specifies the screen
	//     coordinates to be converted.
	//----------------------------------------------------------------------
	void ScreenToClient(CPoint* point);

	//----------------------------------------------------------------------
	// Summary:
	//     This helper method is called when the user clicks to the toolbar's
	//     button.
	// Parameters:
	//     rcActiveRect - Rectangle of the control.
	//----------------------------------------------------------------------
	void ClickToolBarButton(CRect rcActiveRect = CRect(0, 0, 0, 0));

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to copy the control.
	// Parameters:
	//     pControl   - Points to a source CXTPControl object
	//     bRecursive - TRUE to copy recursively.
	//----------------------------------------------------------------------
	virtual void Copy(CXTPControl* pControl, BOOL bRecursive = FALSE);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to popup the control.
	// Parameters:
	//     bPopup - TRUE to set popup.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//----------------------------------------------------------------------
	virtual BOOL OnSetPopup(BOOL bPopup);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user clicks the control.
	// Parameters:
	//     bKeyboard - TRUE if the control is selected using the keyboard.
	//     pt        - Mouse cursor position.
	//----------------------------------------------------------------------
	virtual void OnClick(BOOL bKeyboard = FALSE, CPoint pt = CPoint(0, 0));

	//----------------------------------------------------------------------
	// Summary:
	//     This member is called when the user releases the left mouse button.
	// Parameters:
	//     point - Specifies the x- and y-coordinate of the cursor.
	//----------------------------------------------------------------------
	virtual void OnLButtonUp(CPoint point);

	//----------------------------------------------------------------------
	// Summary:
	//     This member is called when the user releases the right mouse button.
	// Parameters:
	//     point - Specifies the x- and y-coordinate of the cursor.
	//----------------------------------------------------------------------
	virtual void OnRButtonUp(CPoint point);

	//----------------------------------------------------------------------
	// Summary:
	//     This member is called when the user double-clicks the left mouse button.
	// Parameters:
	//     point - Specifies the x- and y-coordinates of the cursor.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//----------------------------------------------------------------------
	virtual BOOL OnLButtonDblClk(CPoint point);

	//----------------------------------------------------------------------
	// Summary:
	//     This member is called when the mouse cursor moves.
	// Parameters:
	//     point - Specifies the x- and y-coordinate of the cursor.
	//----------------------------------------------------------------------
	virtual void OnMouseMove(CPoint point);

	//----------------------------------------------------------------------
	// Summary:
	//     Called after the mouse hovers over the control.
	//----------------------------------------------------------------------
	virtual void OnMouseHover();

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called when the control becomes selected.
	// Parameters:
	//     bSelected - TRUE if the control becomes selected.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//----------------------------------------------------------------------
	virtual BOOL OnSetSelected(int bSelected);

	//----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if command bars is in customize
	//     mode.
	// Returns:
	//     TRUE if command bars is in Customize mode; otherwise returns
	//     FALSE
	//----------------------------------------------------------------------
	BOOL IsCustomizeMode();

	//----------------------------------------------------------------------
	// Summary:
	//     This member is called when the user starts to drag the control in customize mode.
	// Parameters:
	//     point - Specifies the x- and y-coordinate of the cursor.
	// See Also: IsCustomizeMode, CustomizeStartResize
	//----------------------------------------------------------------------
	virtual void CustomizeStartDrag(CPoint point);

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called when user starts resize control in customize mode
	// Parameters:
	//     point - Specifies the x- and y-coordinate of the cursor.
	// Returns:
	//     TRUE if control process this event.
	// See Also: IsCustomizeMode, CustomizeStartDrag, GetCustomizeMinWidth
	//----------------------------------------------------------------------
	virtual BOOL CustomizeStartResize(CPoint point);

	//----------------------------------------------------------------------
	// Summary:
	//     This member is called when the user moves the mouse over the
	//     control.
	// Parameters:
	//     pDataObject - Points to a CXTPControl object
	//     point       - Mouse position.
	//     dropEffect  - DROPEFFECT enumerator.
	//----------------------------------------------------------------------
	virtual void OnCustomizeDragOver(CXTPControl* pDataObject, CPoint point, DROPEFFECT& dropEffect);

	//----------------------------------------------------------------------
	// Summary:
	//     This member checks if the user can drop the control.
	// Parameters:
	//     pCommandBar - Points to a CXTPCommandBar object
	//     point       - Mouse position.
	//     dropEffect  - DROPEFFECT enumerator.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE
	//----------------------------------------------------------------------
	virtual BOOL IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint point, DROPEFFECT& dropEffect);

	//----------------------------------------------------------------------
	// Summary:
	//     This member checks if the user can resize control.
	// Returns:
	//     TRUE if resize available.
	//----------------------------------------------------------------------
	virtual BOOL IsCustomizeResizeAllow();

	//----------------------------------------------------------------------
	// Summary:
	//     Retrieves available minimum width of control.
	// Remarks:
	//     This method is called in CustomizeStartResize to retrieve dimension of available rectangles
	//     of resized control.
	// Returns:
	//     Returns zero by default.
	// See Also: CXTPControlComboBox::GetCustomizeMinWidth, CXTPControlEdit::GetCustomizeMinWidth
	//----------------------------------------------------------------------
	virtual int GetCustomizeMinWidth();

	//----------------------------------------------------------------------
	// Summary:
	//     This member is called when the mouse cursor moves in customized mode
	// Parameters:
	//     point - Specifies the x- and y-coordinate of the cursor.
	//----------------------------------------------------------------------
	void OnCustomizeMouseMove(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the width of the control
	// Parameters:
	//     nWidth - The width of the control
	// Remarks:
	//     Implemented for CXTPControlEdit and CXTPControlComboBox only
	//-----------------------------------------------------------------------
	virtual void SetWidth(int nWidth);

	//----------------------------------------------------------------------
	// Summary:
	//     Call this member to set/remove hide flags.
	// Parameters:
	//     dwFlag - XTPControlHideFlags type of flag to be set/remove
	//     bSet   - TRUE to set flag; FALSE to remove.
	// Remarks:
	//     SetHideFlag will tell how a control was hidden if it is hidden
	//     or why it is not visible.  Most of these flags are read only
	//     and should not be set be the developer.
	// Returns:
	//     TRUE if flags was changed
	// See Also: XTPControlHideFlags, SetHideFlags
	//----------------------------------------------------------------------
	BOOL SetHideFlag(XTPControlHideFlags dwFlag, BOOL bSet);

private:
	void Copy(CXTPControl* pControl);
	void OnInvertTracker(CDC* pDC, CRect rect);

public:
	BOOL m_bDoubleGripper;          // TRUE if control has double gripper.

protected:
	DWORD_PTR m_nTag;               // Control tag.
	int m_nIndex;                   // Index of the control.
	int m_nId;                      // Identifier of the control.
	int m_nIconId;                  // Identifier of the control's image.
	int m_nCustomIconId;            // Identifier of the control's custom image.
	BOOL m_bWrap;                   // TRUE if the control is wrapped.
	BOOL m_bBeginGroup;             // TRUE if the control starts new group.
	BOOL m_bEnabled;                // TRUE if the control is enabled.
	BOOL m_bChecked;                // TRUE if the control is checked.
	BOOL m_bSelected;               // TRUE if the control is selected.
	BOOL m_bPressed;                // TRUE if the control is pushed.
	BOOL m_bTemporary;              // TRUE if the control is temporary.
	CRect m_rcRaw;                  // Bounding rectangle of the control's row.
	CRect m_rcControl;              // Bounding rectangle of the control.
	DWORD m_dwHideFlags;            // Hidden flags.
	DWORD m_dwFlags;                // Flags of the control.
	CString m_strCaption;           // Caption of the control.
	CString m_strCustomCaption;     // User defined caption.
	CString m_strShortcutText;      // Shortcut text.
	CString m_strShortcutTextAuto;  // Shortcut text.
	CString m_strTooltipText;       // Tooltip text.
	CString m_strDescriptionText;   // Description text.
	CString m_strParameter;         // Parameter text.
	CXTPControls* m_pControls;      // Parent control collection
	XTPControlType m_controlType;   // Type of the control.
	CXTPCommandBar* m_pParent;      // The Parent command bar.
	BOOL m_bExpanded;               // TRUE if the control is expanded.

	CString m_strCategory;          // Category of the control.
	BOOL m_bDefaultItem;            // TRUE if the item is default popup item;

	CDocTemplateMap m_mapDocTemplatesAssigned;  // Assigned templates.
	CDocTemplateMap m_mapDocTemplatesExcluded;  // Excluded templates.

	XTPButtonStyle m_buttonStyle;   // Button Style.
	XTPButtonStyle m_buttonCustomStyle;         // User defined style of button.

	BOOL m_bCloseSubMenuOnClick;    // TRUE is sub-menus are closed as soon as a control is clicked.


private:

	DECLARE_XTP_CONTROL(CXTPControl)

	friend class CXTPControls;
	friend class CXTPCommandBar;
	friend class CXTPToolBar;
	friend class CXTPPopupBar;
	friend class CXTPCustomizeSheet;
	friend class CXTPCustomizeDropSource;
};

//////////////////////////////////////////////////////////////////////////


AFX_INLINE long CXTPControl::GetIndex() {
	return m_nIndex;
}
AFX_INLINE void CXTPControl::SetShortcutText(CString strShortcutText) {
	m_strShortcutText = strShortcutText;
}
AFX_INLINE CString CXTPControl::GetShortcutText() {
	return m_strShortcutText.IsEmpty()? m_strShortcutTextAuto: m_strShortcutText;
}
AFX_INLINE void CXTPControl::SetDescription(CString strDescription) {
	m_strDescriptionText = strDescription;
}
AFX_INLINE CString CXTPControl::GetDescription() {
	return m_strDescriptionText;
}
AFX_INLINE CString CXTPControl::GetTooltip(LPPOINT /*pPoint = 0*/, LPRECT /*lpRectTip = 0*/, int* /*nHit = 0*/) {
	return m_strTooltipText;
}
AFX_INLINE void CXTPControl::SetTooltip(CString strTooltip) {
	m_strTooltipText = strTooltip;
}
AFX_INLINE CString CXTPControl::GetCaption() {
	return m_strCustomCaption.IsEmpty()? m_strCaption: m_strCustomCaption;
}
AFX_INLINE void CXTPControl::SetParameter(CString strParameter) {
	m_strParameter = strParameter;
}
AFX_INLINE CString CXTPControl::GetParameter() {
	return m_strParameter;
}
AFX_INLINE int CXTPControl::GetID () {
	return m_nId;
}
AFX_INLINE void CXTPControl::SetIconId(int nId) {
	if (m_nIconId != nId) {m_nIconId = nId; DelayRedrawParent();}
}
AFX_INLINE int CXTPControl::GetIconId() {
	return m_nCustomIconId != 0? m_nCustomIconId: m_nIconId <=0? m_nId: m_nIconId;
}
AFX_INLINE void CXTPControl::SetBeginGroup(BOOL bBeginGroup) {
	m_bBeginGroup = bBeginGroup; DelayLayoutParent();
}
AFX_INLINE BOOL CXTPControl::GetBeginGroup() {
	return m_bBeginGroup;
}
AFX_INLINE BOOL CXTPControl::GetEnabled() {
	return m_bEnabled;
}
AFX_INLINE void CXTPControl::SetEnabled(BOOL bEnabled) {
	if (m_bEnabled != bEnabled) {m_bEnabled = bEnabled; DelayRedrawParent();}
}
AFX_INLINE BOOL CXTPControl::GetChecked() {
	return m_bChecked;
}
AFX_INLINE void CXTPControl::SetChecked(BOOL bChecked) {
	if (m_bChecked != bChecked) {m_bChecked = bChecked; DelayRedrawParent();}
}
AFX_INLINE int CXTPControl::GetSelected() {
	return m_bSelected;
}
AFX_INLINE BOOL CXTPControl::GetPressed() {
	return m_bPressed;
}
AFX_INLINE void CXTPControl::SetPressed(BOOL bPressed) {
	m_bPressed = bPressed;
}
AFX_INLINE void CXTPControl::SetRect(CRect rcControl) {
	m_rcControl = rcControl;
}
AFX_INLINE CRect CXTPControl::GetRect() {
	return m_rcControl;
}
AFX_INLINE CXTPCommandBar* CXTPControl::GetParent() {
	return m_pParent;
}
AFX_INLINE BOOL CXTPControl::IsVisible() {
	return m_dwHideFlags == xtpNoHide;
}
AFX_INLINE void CXTPControl::SetHideFlags(DWORD dwFlags) {
	m_dwHideFlags = dwFlags;
}
AFX_INLINE BOOL CXTPControl::SetHideFlag(XTPControlHideFlags dwFlag, BOOL bHide) {
	DWORD dwHideFlags = m_dwHideFlags;
	if (bHide) SetHideFlags (m_dwHideFlags | dwFlag); else SetHideFlags(m_dwHideFlags & ~dwFlag);
	return dwHideFlags != m_dwHideFlags;
}
AFX_INLINE void CXTPControl::SetVisible(BOOL bVisible) {
	DWORD dwHideFlags = m_dwHideFlags;
	if (!bVisible) SetHideFlags(m_dwHideFlags | xtpHideGeneric); else SetHideFlags(m_dwHideFlags & ~xtpHideGeneric);
	if (dwHideFlags != m_dwHideFlags) DelayLayoutParent();
}
AFX_INLINE void CXTPControl::SetHideWrap(BOOL bHide) {
	SetHideFlag(xtpHideWrap, bHide);
}
AFX_INLINE DWORD CXTPControl::GetHideFlags() {
	return m_dwHideFlags;
}
AFX_INLINE CRect CXTPControl::GetRawRect() {
	return m_rcRaw;
}
AFX_INLINE void CXTPControl::SetRawRect(CRect rcRaw) {
	m_rcRaw = rcRaw;
}
AFX_INLINE BOOL CXTPControl::GetWrap() {
	return m_bWrap;
}
AFX_INLINE void CXTPControl::SetWrap(BOOL bWrap) {
	m_bWrap = bWrap;
}
AFX_INLINE CXTPControls* CXTPControl::GetControls() {
	return m_pControls;
}

AFX_INLINE BOOL CXTPControl::IsTemporary() {
	return m_bTemporary;
}
AFX_INLINE XTPControlType CXTPControl::GetType() {
	return m_controlType;
}
AFX_INLINE BOOL CXTPControl::GetExpanded() {
	return m_bExpanded;
}
AFX_INLINE void CXTPControl::SetCategory(CString strCategory) {
	m_strCategory = strCategory;
}
AFX_INLINE CString CXTPControl::GetCategory() {
	return m_strCategory;
}
AFX_INLINE void CXTPControl::SetTag(DWORD_PTR dwTag) {
	m_nTag = dwTag;
}
AFX_INLINE DWORD_PTR CXTPControl::GetTag() {
	return m_nTag;
}
AFX_INLINE void CXTPControl::AssignDocTemplate(UINT nIDResource) {
	m_mapDocTemplatesAssigned.SetAt(nIDResource, TRUE);
}
AFX_INLINE void CXTPControl::ExcludeDocTemplate(UINT nIDResource) {
	m_mapDocTemplatesExcluded.SetAt(nIDResource, TRUE);
}
AFX_INLINE void CXTPControl::OnClick(BOOL /*bKeyboard = FALSE*/, CPoint /*pt = CPoint(0, 0)*/) {
}
AFX_INLINE void CXTPControl::SetStyle(XTPButtonStyle buttonStyle) {
	m_buttonStyle = buttonStyle;
	DelayLayoutParent();
}
AFX_INLINE XTPButtonStyle CXTPControl::GetStyle() {
	return m_buttonCustomStyle != xtpButtonAutomatic? m_buttonCustomStyle: m_buttonStyle;
}
AFX_INLINE CXTPCommandBar* CXTPControl::GetCommandBar() {
	return NULL;
}
AFX_INLINE void CXTPControl::SetCloseSubMenuOnClick(BOOL bCloseOnClick) {
	m_bCloseSubMenuOnClick = bCloseOnClick;
}

AFX_INLINE BOOL CXTPControl::GetCloseSubMenuOnClick() {
	return m_bCloseSubMenuOnClick;
}
AFX_INLINE BOOL CXTPControl::IsLabel() {
	return FALSE;
}
AFX_INLINE int CXTPControl::GetCustomizeMinWidth() {
	return 0;
}
AFX_INLINE void CXTPControl::SetWidth(int /*nWidth*/) {
}
AFX_INLINE BOOL CXTPControl::OnKeyDown(UINT /*nChar*/) {
	return FALSE;
}
AFX_INLINE BOOL CXTPControl::OnSetPopup(BOOL /*bPopup*/) {
	return FALSE;
}
AFX_INLINE void CXTPControl::OnLButtonUp(CPoint /*point*/) {
}
AFX_INLINE void CXTPControl::OnRButtonUp(CPoint /*point*/) {
}
AFX_INLINE void CXTPControl::OnMouseMove(CPoint /*point*/) {
}

#endif // #if !defined(__XTPCONTROL_H__)
