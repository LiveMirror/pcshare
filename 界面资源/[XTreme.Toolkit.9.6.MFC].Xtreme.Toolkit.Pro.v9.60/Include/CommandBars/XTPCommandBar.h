// XTPCommandBar.h : interface for the CXTPCommandBar class.
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
#if !defined(__XTPCOMMANDBAR_H__)
#define __XTPCOMMANDBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <oleacc.h>
#include "XTPCommandBarsDefines.h"
#include "XTPHookManager.h"

class CXTPDockBar;
class CXTPControls;
class CXTPControl;
class CXTPControlPopup;
class CXTPCommandBars;
class CXTPCommandBar;
class CXTPPaintManager;
class CXTPImageManager;
class CXTPPropExchange;
struct XTP_COMMANDBARS_PROPEXCHANGE_PARAM;

//{{AFX_CODEJOCK_PRIVATE
#define XTP_TID_HOVER    112223
#define XTP_TID_EXPANDED_POPUP 112224
#define XTP_BOOL_DEFAULT 2
//}}AFX_CODEJOCK_PRIVATE



//===========================================================================
// Summary:
//     CXTPCommandBarList is a CList derived class. It is used as a collection
//     of CXTPCommandBar classes.
//===========================================================================
class _XTP_EXT_CLASS CXTPCommandBarList : public CList<CXTPCommandBar*, CXTPCommandBar*>
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCommandBarList object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	~CXTPCommandBarList();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to find the CommandBar with the specified
	//     identifier.
	// Parameters:
	//     nBarID - Identifier CommandBar to find.
	// Returns:
	//     A pointer to a CXTPCommandBar object.
	//-----------------------------------------------------------------------
	CXTPCommandBar* FindCommandBar(UINT nBarID);
};

//===========================================================================
// Summary:
//     CXTPCommandBar is a CWnd derived class. It represents the parent
//     class for command bars.
//===========================================================================
class _XTP_EXT_CLASS CXTPCommandBar : public CWnd, public CXTPHookManagerHookAble
{
private:
	typedef VOID (WINAPI* LPFNNOTIFYWINEVENT)( DWORD event, HWND hwnd, LONG idObject, LONG idChild);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCommandBar object
	//-----------------------------------------------------------------------
	CXTPCommandBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCommandBar object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPCommandBar();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the tracking state.
	// Returns:
	//     TRUE if the command bar is in the tracking mode.
	//-----------------------------------------------------------------------
	int IsTrackingMode() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the controls collection.
	// Returns:
	//     A CXTPControls pointer.
	//-----------------------------------------------------------------------
	CXTPControls* GetControls();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the type of the command bar.
	// Returns:
	//     The type of the command bar. It can be one of the following:
	//     * <b>xtpBarTypeMenuBar</b> Indicates the command bar is menu bar (CXTPMenuBar)
	//     * <b>xtpBarTypeNormal</b> Indicates the command bar is simple toolbar (CXTPToolBar)
	//     * <b>xtpBarTypePopup</b> Indicates the command bar is popupbar (CXTPPopupBar)
	// See Also: XTPBarType, GetPosition
	//-----------------------------------------------------------------------
	XTPBarType GetType();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the position of the command bar.
	// Returns:
	//     The position of the command bar.
	// See Also: XTPBarPosition, GetType
	//-----------------------------------------------------------------------
	XTPBarPosition GetPosition() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to recalculate the layout.
	//-----------------------------------------------------------------------
	void DelayLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to redraw the command bar.
	//-----------------------------------------------------------------------
	virtual void DelayRedraw();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to change the tracking state.
	// Parameters:
	//     bMode        - TRUE to set the tracking mode; otherwise FALSE.
	//     bSelectFirst - TRUE to select the first item.
	//     bKeyboard    - TRUE if the item is popuped by the keyboard.
	// Returns:
	//     TRUE if the method was successful.
	// See Also: IsTrackingMode.
	//-----------------------------------------------------------------------
	virtual BOOL SetTrackingMode(int bMode, BOOL bSelectFirst = TRUE, BOOL bKeyboard = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to convert menu items to command bar controls.
	// Parameters:
	//     pMenu           - Menu to be converted.
	//     bRemoveControls - TRUE to remove all controls first.
	// Returns:
	//     TRUE if the method was successful; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL LoadMenu(CMenu* pMenu, BOOL bRemoveControls = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to return the control at the specified index.
	// Parameters:
	//     nIndex - An integer index.
	// Returns:
	//     The CXTPControl pointer currently at this index.
	// See also: GetControls, CXTPControls::GetAt
	//-----------------------------------------------------------------------
	CXTPControl* GetControl(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to select the specified control.
	// Parameters:
	//     nSelected - An integer index of the item to be selected.
	//     bKeyboard - TRUE if the item was selected using the keyboard.
	// Returns:
	//     TRUE if the method was successful; otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL SetSelected(int nSelected, BOOL bKeyboard = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to popup the specified control.
	// Parameters:
	//     nPopuped  - An integer index of the item to be popuped.
	//     bKeyboard - TRUE if the item is popuped by the keyboard.
	// Returns:
	//     TRUE if the method was successful; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL SetPopuped(int nPopuped, BOOL bKeyboard = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set a title for this command bar.
	// Parameters:
	//     strTitle - Title to be set.
	//-----------------------------------------------------------------------
	void SetTitle(CString strTitle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get a title for this command bar.
	// Returns:
	//     The title of the command bar.
	//-----------------------------------------------------------------------
	CString GetTitle();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the command bar's object.
	// Returns:
	//     A CXTPCommandBars pointer (can be NULL).
	//-----------------------------------------------------------------------
	CXTPCommandBars* GetCommandBars() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the command bar's object.
	// Parameters:
	//     pCommandBars - CXTPCommandBars to be set.
	//-----------------------------------------------------------------------
	void SetCommandBars(CXTPCommandBars* pCommandBars);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the command bar's site.
	// Returns:
	//     Command bar's site (cannot be NULL).
	//-----------------------------------------------------------------------
	CWnd* GetSite();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the command bar's site as CFrameWnd.
	// Returns:
	//     The command bar's site (can be NULL).
	//-----------------------------------------------------------------------
	CFrameWnd* GetFrameSite();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set focused control.
	// Parameters:
	//     pControl - The control to be set.
	//-----------------------------------------------------------------------
	void SetFocusedControl(CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the command bar's flags.
	// Parameters:
	//     dwFlagsAdd    - Flags to be added.
	//     dwFlagsRemove - Flags to be removed.
	//-----------------------------------------------------------------------
	void SetFlags(DWORD dwFlagsAdd, DWORD dwFlagsRemove = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the controls from the button array.
	// Parameters:
	//     pButtons - Pointer to an array of command Ids. It cannot be NULL.
	//     nCount   - Number of elements in the array pointed to by pButtons.
	// Returns:
	//     Nonzero if successful; otherwise 0.
	// See Also: LoadToolBar, LoadMenu.
	//-----------------------------------------------------------------------
	BOOL SetButtons(UINT* pButtons, int nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to load the toolbar specified by nIDResource.
	// Parameters:
	//     nIDResource - Resource ID of the toolbar to be loaded.
	//     bLoadIcons  - TRUE if the toolbar icons are to be used by CXTPImageManager.
	// Returns:
	//     Nonzero if successful; otherwise 0.
	// See Also: LoadMenu, SetButtons.
	//-----------------------------------------------------------------------
	virtual BOOL LoadToolBar(UINT nIDResource, BOOL bLoadIcons = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the size of the actual bitmapped
	//     images to be added to a toolbar.
	// Parameters:
	//     size - Width and height, in pixels, of the toolbar's icons.
	//-----------------------------------------------------------------------
	void SetIconSize(CSize size);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the size of a toolbar icon.
	//-----------------------------------------------------------------------
	CSize GetIconSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the size of the buttons in the command bar.
	// Parameters:
	//     size - Width and height, in pixels, of the buttons.
	// Remarks:
	//     The button size must always be at least as large as the bitmap
	//     size it encloses. This function must be called only before adding
	//     any bitmaps to the command bar. If the application does not explicitly
	//     set the button size, it is set to the default size.
	//-----------------------------------------------------------------------
	void SetButtonSize(CSize size);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the size of the command bar button.
	// Returns:
	//     The width and height values of the command bar button.
	//-----------------------------------------------------------------------
	CSize GetButtonSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to show the caption below control's icon.
	// Parameters:
	//     bTextBelow - TRUE to show the caption below the icon.
	//-----------------------------------------------------------------------
	void ShowTextBelowIcons(BOOL bTextBelow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a Boolean value that indicates whether the caption is
	//     below the control's icon.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL IsTextBelowIcons();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to popup the command bar.
	// Parameters:
	//     pControlPopup - Points to a CXTPControlPopup object.
	//     bSelectFirst  - TRUE to select the first item.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Popup(CXTPControlPopup* pControlPopup, BOOL bSelectFirst = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the current focused control.
	// Returns:
	//     Window handle of the focused control.
	//-----------------------------------------------------------------------
	HWND GetTrackFocus();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set focus to the window.
	// Parameters:
	//     hwnd - Window handle of the focused control
	//-----------------------------------------------------------------------
	void SetTrackFocus(HWND hwnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the visibility state of the control.
	// Parameters:
	//     bVisible - TRUE to set control visible.
	//-----------------------------------------------------------------------
	virtual void SetVisible(BOOL bVisible);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a Boolean value that indicates whether the command bar
	//     is visible or hidden
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL IsVisible();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the position of the command bar
	// Parameters:
	//     barPosition - Position of the command bar
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL SetPosition(XTPBarPosition barPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the index of control with specified
	//     accelerator character.
	// Parameters:
	//     chAccel - Character to find.
	// Returns:
	//     Index of the control contained  character as accelerator.
	//-----------------------------------------------------------------------
	int FindAccel(TCHAR chAccel);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the index of the control with specified
	//     accelerator character.
	// Parameters:
	//     chAccel     - Character to find
	//     bSelectOnly - TRUE to select only.
	// Returns:
	//     Index of the control contained character as accelerator.
	//-----------------------------------------------------------------------
	int FindAccelEx(TCHAR chAccel, BOOL& bSelectOnly);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the customize mode of the command
	//     bars.
	// Returns:
	//     TRUE if command bars are in customized mode; otherwise returns
	//     FALSE
	//-----------------------------------------------------------------------
	BOOL IsCustomizeMode();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when a control is entered into the
	//     command bar.
	// Parameters:
	//     pDataObject - Points to a CXTPControl object
	//     point       - Point to test.
	// Returns:
	//     DROPEFFECT enumerator.
	//-----------------------------------------------------------------------
	DROPEFFECT OnCustomizeDragEnter(CXTPControl* pDataObject, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a control leaves the command bar.
	//-----------------------------------------------------------------------
	void OnCustomizeDragLeave();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when control is over the command bar.
	// Parameters:
	//     pDataObject - Points to a CXTPControl object
	//     point       - Point to test
	// Returns:
	//     DROPEFFECT enumerator.
	//-----------------------------------------------------------------------
	DROPEFFECT OnCustomizeDragOver(CXTPControl* pDataObject, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when control is dropped to the command bar.
	// Parameters:
	//     pDataObject - Points to a CXTPControl object
	//     dropEffect  - DROPEFFECT enumerator.
	//     ptDrop      - Point where user drops the control.
	//     ptDrag      - Point where user starts drag the control.
	//-----------------------------------------------------------------------
	void OnCustomizeDrop(CXTPControl* pDataObject, DROPEFFECT& dropEffect, CPoint ptDrop, CPoint ptDrag);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the customizable property of the
	//     command bar.
	// Returns:
	//     TRUE if the command bar is customizable; otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL IsCustomizable();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable/disable customization of the command bar.
	// Parameters:
	//     bEnable - TRUE to enable customization.
	//-----------------------------------------------------------------------
	void EnableCustomization(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to recalculate command bar layout.
	//-----------------------------------------------------------------------
	virtual void OnRecalcLayout();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to remove the tracking state of the command bar.
	//-----------------------------------------------------------------------
	void OnTrackLost();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to track the command bar.
	//-----------------------------------------------------------------------
	void OnTrackEnter();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the owner window of the command bar.
	// Returns:
	//     A pointer to a CWnd object
	//-----------------------------------------------------------------------
	CWnd* GetOwnerSite();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to redraw all parent command bars.
	//-----------------------------------------------------------------------
	void InvalidateParents();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the current behavior modification
	//     flags of the command bar.
	// Returns:
	//     Flags of the command bar that are currently in effect.
	//-----------------------------------------------------------------------
	DWORD GetFlags();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the identifier of the command bar.
	// Returns:
	//     Identifier of the command bar.
	//-----------------------------------------------------------------------
	UINT GetBarID();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when a child control becomes selected.
	// Parameters:
	//     bSelected - TRUE when control becomes selected.
	//     pControl  - Points to a CXTPControl object
	//-----------------------------------------------------------------------
	void OnControlSelected(BOOL bSelected, CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to set the description context.
	// Parameters:
	//     strDescription - Description string.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL SetStatusText(CString strDescription);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the 32-bit value associated with the command bar.
	// Parameters:
	//     dwData - Contains the new value to associate with the command bar.
	// See Also: GetCommandBarData
	//-----------------------------------------------------------------------
	void SetCommandBarData(DWORD_PTR dwData);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves the application-supplied 32-bit value
	//     associated with the command bar.
	// Returns:
	//     The 32-bit value associated with the command bar.
	// See Also: SetCommandBarData
	//-----------------------------------------------------------------------
	DWORD_PTR GetCommandBarData();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves the parent command bar.
	// Returns:
	//     A pointer to a CXTPCommandBar object
	//-----------------------------------------------------------------------
	virtual CXTPCommandBar* GetParentCommandBar() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the top-level parent command bar.
	// Returns:
	//     A pointer to the top-level CXTPCommandBar object
	//-----------------------------------------------------------------------
	CXTPCommandBar* GetRootParent();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the paint manager.
	// Returns:
	//     A pointer to a CXTPPaintManager object
	//-----------------------------------------------------------------------
	CXTPPaintManager* GetPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the image manager
	// Returns:
	//     A pointer to a CXTPImageManager object
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the width of the floated or popup toolbar.
	// Parameters:
	//     nWidth - Width of the floated or popuped toolbar.
	//-----------------------------------------------------------------------
	void SetWidth(int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the width of the floated or popup toolbar.
	// Returns:
	//     The width, in pixels, of the floated or popup toolbar.
	//-----------------------------------------------------------------------
	int GetWidth();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the specified paint manager
	// Parameters:
	//     pPaintManager - Points to a CXTPPaintManager object  to be set
	//-----------------------------------------------------------------------
	void SetPaintManager(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the specified image manager
	// Parameters:
	//     pImageManager - Points to a CXTPImageManager object to be set
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the CommandBar layout
	//     is Right-to-Left (RTL).
	// Returns:
	//     Returns TRUE if the Commandbar layout is RTL, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsLayoutRTL();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the CommandBar is a
	//     CXTPDialogBar.
	// Returns:
	//     Returns TRUE if the CommandBar is a CXTPDialogBar, otherwise FALSE
	// See Also: CXTPToolBar, CXTPDialogBar
	//-----------------------------------------------------------------------
	virtual BOOL IsDialogBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by WindowProc, or is called during
	//     message reflection.
	// Parameters:
	//     hWnd     - Window handle message belongs to.
	//     nMessage - Specifies the message to be sent.
	//     wParam   - Specifies additional message-dependent information.
	//     lParam   - Specifies additional message-dependent information.
	//     lResult  - The return value of WindowProc. Depends on the message;
	//                may be NULL.
	//-----------------------------------------------------------------------
	virtual int OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult);

	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function when a non-system key
	//     is pressed.
	// Parameters:
	//     nChar - Specifies the virtual key code of the given key.
	// Returns:
	//     TRUE if key handled, otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL OnKeyDown(UINT nChar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the size of the command bar.
	// Parameters:
	//      nLength - The requested dimension of the control bar, either horizontal or vertical, depending on dwMode.
	//      dwMode - see CControlBar::CalcDynamicLayout for list of supported flags.
	// Returns:
	//     Size of the command bar.
	//-----------------------------------------------------------------------
	virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to process special keys.
	// Parameters:
	//     key - Special keys enumerator. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     CXTPCommandBar maps key passed as parameter of OnKeyDown to XTPSpecialKey enumerator and call ProcessSpecialKey.<p/>
	//     key parameter can be one of the following:
	//         * <b>xtpKeyNext</b> Indicates the RIGHT or DOWN key was pressed  Next control must be selected
	//         * <b>xtpKeyPrev</b> Indicates the LEFT or UP key was pressed. Previous control must be selected.
	//         * <b>xtpKeyBack</b> Indicates the LEFT key was pressed for popup bar. Current submenu must be closed
	//         * <b>xtpKeyPopup</b> Indicates the RIGHT key was pressed for popup bar. Submenu must be opened.
	//         * <b>xtpKeyEscape</b> Indicates the ESCAPE key was pressed.
	//         * <b>xtpKeyReturn</b> Indicates the RETURN key was pressed
	//         * <b>xtpKeyHome</b> Indicates the HOME key was pressed. First control must be selected.
	//         * <b>xtpKeyEnd</b> Indicates the END key was pressed. Last control must be selected.
	// Returns:
	//     TRUE if key handled, otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL ProcessSpecialKey(XTPSpecialKey key);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the command bar in the given context.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//-----------------------------------------------------------------------
	virtual void DrawCommandBar(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the controls array is changed.
	//-----------------------------------------------------------------------
	virtual void OnControlsChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method makes a copy of the command bar.
	// Parameters:
	//     pCommandBar - Command bar needed to be copied.
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	virtual void Copy(CXTPCommandBar* pCommandBar, BOOL bRecursive = FALSE);

	//-------------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function to redraw the CommandBars.
	//-------------------------------------------------------------------------
	void Redraw();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine mouse hit code
	// Parameters:
	//     pt - Position to test
	// Returns:
	//     HTCLIENT if pointer belongs client area; HTNOWHERE otherwise.
	//-----------------------------------------------------------------------
	virtual int OnMouseHitTest(CPoint pt);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to freeze redrawing until UnlockRedraw call
	// See Also: UnlockRedraw
	//-------------------------------------------------------------------------
	void LockRedraw();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to unfreeze redrawing.
	// See Also: LockRedraw
	//-------------------------------------------------------------------------
	void UnlockRedraw();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves active MDI document template
	// Remarks:
	//     This method call CXTPCommandBars::GetActiveDocTemplate.
	// Returns:
	//     Active document template identifier.
	// See Also: CXTPCommandBars::GetActiveDocTemplate
	//-----------------------------------------------------------------------
	UINT GetActiveDocTemplate();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to assign self identifiers for serialization process.
	// Parameters:
	//     nID             - Identifier to assign
	//     pCommandBarList - List of CommandBars.
	//     pParam          - Address of a XTP_COMMANDBARS_PROPEXCHANGE_PARAM structure.
	//-----------------------------------------------------------------------
	virtual void GenerateCommandBarList(DWORD& nID, CXTPCommandBarList* pCommandBarList, XTP_COMMANDBARS_PROPEXCHANGE_PARAM* pParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called in serialization process.to restore popups from list of command bars.
	// Parameters:
	//     pCommandBarList - List of CommandBars.
	//-----------------------------------------------------------------------
	virtual void RestoreCommandBarList(CXTPCommandBarList* pCommandBarList);

protected:
//{{AFX_CODEJOCK_PRIVATE

	//{{AFX_VIRTUAL(CXTPCommandBar)
	virtual void OnFinalRelease();
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	public:
	virtual LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPCommandBar)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseLeave();
	afx_msg void OnPaint();
	afx_msg LRESULT OnPrint(WPARAM wParam, LPARAM);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT OnHelpHitTest(WPARAM, LPARAM lParam);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg LRESULT OnGetObject(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	void CustomizeFindDropIndex(CXTPControl* pDataObject, const CPoint& point, CRect& rcMarker, int& nDropIndex, BOOL& bDropAfter);
	void CustomizeDrawMarker(CDC* pDC);
	void UpdateDocTemplateControls();
	void OnUpdateCmdUI(CWnd* pTarget, BOOL bDisableIfNoHndler);

public:
	int               m_nStateFlags;         // State of the command bars.
	static int        s_nHoverDelay;         // Hover delay time
	static int        s_nExpandDelay;        // Expand delay time

protected:
	int              m_nMRUWidth;           // Recent width.
	int              m_nSelected;           // Selected control index.
	int              m_nPopuped;            // Popuped control index.
	HWND             m_hwndFocus;           // The handle of the window that currently has focus.
	UINT             m_nBarID;              // Command bar's identifier.
	BOOL             m_bVisible;            // TRUE if the command bar is visible.
	BOOL             m_bTracking;           // TRUE if the command bar is in tracking mode.
	CWnd*            m_pSite;               // Command bar's site.
	DWORD            m_nIdleFlags;          // Idle flags.
	DWORD            m_dwFlags;             // Command bar's flags.
	DWORD            m_dwStyle;             // Command bar's styles.
	CString          m_strTitle;            // Command bar's caption.
	XTPBarType       m_barType;             // Type of the command bar.
	CXTPControl*     m_pFocused;            // A pointer to focused control.
	CXTPControls*    m_pControls;           // A pointer to controls collection.
	XTPBarPosition   m_barPosition;         // Position of the command bar.
	CXTPCommandBars* m_pCommandBars;        // parent command bars class.
	CWnd*            m_pOwner;              // Owner window.
	CSize            m_szIcons;             // Size of the command bar's icons.
	CSize            m_szButtons;           // Size of the command bar's icons.
	BOOL             m_bTextBelow;          // TRUE if text is shown below the CommandBar buttons.
	BOOL             m_bExecOnRButton;      // TRUE if the control button is executed on a right-click
	BOOL             m_bIgnoreUpdateHandler;// TRUE to skip update handler for all child controls
	BOOL             m_bKeyboardSelect;     // TRUE if last item was selected by keyboard.
	BOOL             m_bCloseable;          // TRUE if the CommandBar can be closed and\or Hidden
	BOOL             m_bCustomizable;       // TRUE if command bar is customizable.
	CRect            m_rcMarker;            // Marker position.
	BOOL             m_bVerticalMarker;     // TRUE to draw marker vertically.
	DWORD_PTR        m_dwData;              // The 32-bit value associated with the command bar.
	UINT*            m_pReturnCmd;          // Return command holder.
	BOOL             m_bContextMenuPresent; // TRUE if the command bar is present in context menu.
	BOOL             m_bAnimatePopup;       // TRUE to animate popup bar.
	BOOL             m_nLockRedraw;         // TRUE if all drawing routines skipped
	CXTPPaintManager* m_pPaintManager;      // Selected paint manager for commandbar
	CXTPImageManager* m_pImageManager;      // Selected image manager for commandbar

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	DECLARE_XTP_COMMANDBAR(CXTPCommandBar)

	class CXTPCommandBarCmdUI;

	void NotifyFocusEvent(int nSelected);
	static LPFNNOTIFYWINEVENT m_pNotifyWinEvent;
	static LPFNLRESULTFROMOBJECT m_pLresultFromObject;

	BEGIN_INTERFACE_PART(Accessible, IAccessible)
		STDMETHOD(GetTypeInfoCount)(UINT FAR* pctinfo);
		STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo FAR* FAR* pptinfo);
		STDMETHOD(GetIDsOfNames)(REFIID riid, OLECHAR FAR* FAR* rgszNames, UINT cNames,
			LCID lcid, DISPID FAR* rgdispid);
		STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags,
			DISPPARAMS FAR* pdispparams, VARIANT FAR* pvarResult, EXCEPINFO FAR* pexcepinfo,
			UINT FAR* puArgErr);

		STDMETHOD(get_accParent)(IDispatch * FAR* ppdispParent);
		STDMETHOD(get_accChildCount)(long FAR* pChildCount);
		STDMETHOD(get_accChild)(VARIANT varChildIndex, IDispatch * FAR* ppdispChild);

		STDMETHOD(get_accName)(VARIANT varChild, BSTR* pszName);
		STDMETHOD(get_accValue)(VARIANT varChild, BSTR* pszValue);
		STDMETHOD(get_accDescription)(VARIANT varChild, BSTR FAR* pszDescription);
		STDMETHOD(get_accRole)(VARIANT varChild, VARIANT *pvarRole);
		STDMETHOD(get_accState)(VARIANT varChild, VARIANT *pvarState);
		STDMETHOD(get_accHelp)(VARIANT varChild, BSTR* pszHelp);
		STDMETHOD(get_accHelpTopic)(BSTR* pszHelpFile, VARIANT varChild, long* pidTopic);
		STDMETHOD(get_accKeyboardShortcut)(VARIANT varChild, BSTR* pszKeyboardShortcut);
		STDMETHOD(get_accFocus)(VARIANT FAR * pvarFocusChild);
		STDMETHOD(get_accSelection)(VARIANT FAR * pvarSelectedChildren);
		STDMETHOD(get_accDefaultAction)(VARIANT varChild, BSTR* pszDefaultAction);

		STDMETHOD(accSelect)(long flagsSelect, VARIANT varChild);
		STDMETHOD(accLocation)(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild);
		STDMETHOD(accNavigate)(long navDir, VARIANT varStart, VARIANT * pvarEndUpAt);
		STDMETHOD(accHitTest)(long xLeft, long yTop, VARIANT * pvarChildAtPoint);
		STDMETHOD(accDoDefaultAction)(VARIANT varChild);

		STDMETHOD(put_accName)(VARIANT varChild, BSTR szName);
		STDMETHOD(put_accValue)(VARIANT varChild, BSTR pszValue);
	END_INTERFACE_PART(Accessible)

	DECLARE_INTERFACE_MAP()


	friend class CXTPCommandBars;
	friend class CXTPDockBar;
	friend class CXTPCommandBarCmdUI;
	friend class CXTPControl;
	friend class CXTPControls;
	friend class CXTPControlPopup;
	friend class CXTPToolBar;
	friend class CXTPMouseManager;
	friend class CXTPPopupBar;
	friend class CXTPCustomizeSheet;

//}}AFX_CODEJOCK_PRIVATE
};

//////////////////////////////////////////////////////////////////////////


AFX_INLINE int CXTPCommandBar::IsTrackingMode() const{
	return m_bTracking;
}
AFX_INLINE CXTPControls* CXTPCommandBar::GetControls() {
	return m_pControls;
}
AFX_INLINE XTPBarType CXTPCommandBar::GetType() {
	return m_barType;
}
AFX_INLINE XTPBarPosition CXTPCommandBar::GetPosition() const{
	return m_barPosition;
}
AFX_INLINE void CXTPCommandBar::DelayLayout() {
	m_nIdleFlags |= xtpIdleLayout;
}

AFX_INLINE void CXTPCommandBar::DelayRedraw() {
	m_nIdleFlags |= xtpIdleRedraw;
}


AFX_INLINE void CXTPCommandBar::Redraw() {
	m_nIdleFlags &= ~xtpIdleRedraw;
	if ((m_nLockRedraw == 0) && m_hWnd) Invalidate(FALSE);
}
AFX_INLINE void CXTPCommandBar::SetTitle(CString strTitle) {
	m_strTitle = strTitle;
	Redraw();
}
AFX_INLINE CString CXTPCommandBar::GetTitle() {
	return m_strTitle;
}
AFX_INLINE void CXTPCommandBar::SetCommandBars(CXTPCommandBars* pCommandBars){
	m_pCommandBars = pCommandBars;
}
AFX_INLINE HWND CXTPCommandBar::GetTrackFocus() {
	return m_hwndFocus;
}
AFX_INLINE void CXTPCommandBar::SetTrackFocus(HWND hwnd) {
	m_hwndFocus = hwnd;
}
AFX_INLINE void CXTPCommandBar::SetVisible(BOOL /*bVisible*/) {
}
AFX_INLINE BOOL CXTPCommandBar::IsVisible() {
	return FALSE;
}
AFX_INLINE BOOL CXTPCommandBar::SetPosition(XTPBarPosition /*barPosition*/) {
	return FALSE;
}
AFX_INLINE CSize CXTPCommandBar::CalcDynamicLayout(int, DWORD /*nMode*/) {
	return CSize(0,0);
}
AFX_INLINE CXTPCommandBar* CXTPCommandBar::GetParentCommandBar() const {
	return NULL;
}
AFX_INLINE void CXTPCommandBar::OnRecalcLayout() {

}
AFX_INLINE BOOL CXTPCommandBar::IsCustomizable() {
	return m_bCustomizable;
}

AFX_INLINE void CXTPCommandBar::EnableCustomization(BOOL bEnable) {
	if (!IsDialogBar()) m_bCustomizable = bEnable;
}

AFX_INLINE void CXTPCommandBar::SetIconSize(CSize size) {
	m_szIcons = size;
}
AFX_INLINE void CXTPCommandBar::SetButtonSize(CSize size) {
	m_szButtons = size;
}

AFX_INLINE DWORD CXTPCommandBar::GetFlags() {
	return m_dwFlags;
}

AFX_INLINE UINT CXTPCommandBar::GetBarID() {
	return m_nBarID;
}
AFX_INLINE void CXTPCommandBar::SetCommandBarData(DWORD_PTR dwData) {
	m_dwData = dwData;
}
AFX_INLINE DWORD_PTR CXTPCommandBar::GetCommandBarData() {
	return m_dwData;
}
AFX_INLINE BOOL CXTPCommandBar::IsDialogBar() {
	return FALSE;
}
AFX_INLINE void CXTPCommandBar::LockRedraw() {
	m_nLockRedraw ++;
}
AFX_INLINE void CXTPCommandBar::UnlockRedraw() {
	m_nLockRedraw --;
	if (m_nLockRedraw == 0)
		Redraw();
}


//{{AFX_CODEJOCK_PRIVATE
#define foreach(variable, visible)\
	if (1) for (CXTPControl* variable = m_pControls->GetFirst(); variable; m_pControls->GetNext(variable)) if (!visible || (variable->IsVisible()))
//}}AFX_CODEJOCK_PRIVATE

#endif //#if !defined(__XTPCOMMANDBAR_H__)
