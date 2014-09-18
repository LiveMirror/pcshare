// XTPControlCustom.h : interface for the CXTPControlCustom class.
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
#if !defined(__XTPCONTROLCUSTOM_H__)
#define __XTPCONTROLCUSTOM_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPControlButton.h"
#include "XTPHookManager.h"

class CXTPCommandBars;

//-----------------------------------------------------------------------
// Summary:
//     Vertical options enumerator.
// Example:
// <code>
// CXTPControlCustom* pControl = (CXTPControlCustom*)pToolBar->GetControls()->Add(xtpControlCustom, ID_VIEW_LOGO);
// pControl->SetVerticalPositionOption(xtpVerticalShow);
// </code>
// See Also: CXTPControlCustom::SetVerticalPositionOption
//
// <KEYWORDS xtpVerticalHide, xtpVerticalShow, xtpVerticalButton>
//-----------------------------------------------------------------------
enum XTPControlCustomVerticalOptions
{
	xtpVerticalHide,  // To hide control if parent toolbar vertically docked.
	xtpVerticalShow,  // To show control if parent toolbar vertically docked.
	xtpVerticalButton // To show control as button.
};


//===========================================================================
// Summary:
//     CXTPControlCustom is a CXTPControlButton derived class. It represents
//     the control with some child window.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlCustom : public CXTPControlButton, public CXTPHookManagerHookAble
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlCustom object
	//-----------------------------------------------------------------------
	CXTPControlCustom();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControlCustom object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPControlCustom();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to create new CXTPControlCustom object with pWndChild as child window,
	// Parameters:
	//     pWndChild - Pointer to the child window.
	// Returns:
	//     Pointer to new CXTPControlCustom object.
	//-----------------------------------------------------------------------
	static CXTPControlCustom* AFX_CDECL CreateControlCustom(CWnd* pWndChild);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to attach the window to the control.
	// Parameters:
	//     pWnd - Points to a child CWnd object.
	//-----------------------------------------------------------------------
	void SetControl(CWnd* pWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the child window of the control.
	// Returns:
	//     A pointer to a CWnd object.
	//-----------------------------------------------------------------------
	CWnd* GetControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the child window size.
	// Parameters:
	//     szControl - Size of the child window.
	//-----------------------------------------------------------------------
	void SetSize(CSize szControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method sets the margins of the child window.
	// Parameters:
	//     cxRight - Right border gap.
	//     cxTop - Top border gap.
	//     cxLeft - Left border gap.
	//     cxBottom - Bottom border gap.
	//-----------------------------------------------------------------------
	void SetBorders(int cxRight, int cxTop, int cxLeft, int cxBottom);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set vertical options to the control.
	// Parameters:
	//     options - XTPControlCustomVerticalOptions type to be set
	//               Can be any of the values listed in the Remarks section.
	// Remarks:
	//     Vertical options refer to how the control will be displayed
	//     when the toolbar it docked vertically. It can be one of the following:
	//     * <b>xtpVerticalHide</b> To hide control
	//     * <b>xtpVerticalShow</b> To show control
	//     * <b>xtpVerticalButton</b> To show control as button.
	// See Also: XTPControlCustomVerticalOptions
	//-----------------------------------------------------------------------
	void SetVerticalPositionOption(XTPControlCustomVerticalOptions options);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the bounding rectangle of the control.
	// Parameters:
	//     rcControl - Bounding rectangle of the control.
	//-----------------------------------------------------------------------
	virtual void SetRect(CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine the size of the control.
	// Parameters:
	//     pDC - Pointer to a valid device context
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize GetSize(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable or disable the control.
	//     If the control does not have flags xtpFlagManualUpdate, you must call
	//     the Enable member of CCmdUI in the ON_UPDATE_COMMAND_UI handler.
	// Parameters:
	//     bEnabled - TRUE if the control is enabled.
	// See Also: GetEnabled, SetChecked
	//-----------------------------------------------------------------------
	virtual void SetEnabled(BOOL bEnabled);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to hide the control.
	// Parameters:
	//     dwFlags - Reasons to hide.
	// See Also: XTPControlHideFlags
	//-----------------------------------------------------------------------
	virtual void SetHideFlags(DWORD dwFlags);

	//-----------------------------------------------------------------------
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
	//     CXTPControlComboBox, CXTPControl, CXTPControlEdit,
	//     CXTPControlWindowList, CXTPControlWorkspaceActions, CXTPControlToolbars,
	//     CXTPControlOleItems, CXTPControlRecentFileList, CXTPControlSelector,
	//     CXTPControlListBox
	//-----------------------------------------------------------------------
	virtual void OnCalcDynamicSize(DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to copy the control.
	// Parameters:
	//     pControl - Points to a source CXTPControl object
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	virtual void Copy(CXTPControl* pControl, BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user clicks the control.
	// Parameters:
	//     bKeyboard - TRUE if the control is selected using the keyboard.
	//     pt - Mouse cursor position.
	//-----------------------------------------------------------------------
	virtual void OnClick(BOOL bKeyboard = FALSE, CPoint pt = CPoint(0, 0));

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check if the user can drop the control.
	// Parameters:
	//     pCommandBar - Points to a CXTPCommandBar object
	//     point - Mouse position.
	//     dropEffect - DROPEFFECT enumerator.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint /*point*/, DROPEFFECT& dropEffect);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by WindowProc, or is called
	//     during message reflection.
	// Parameters:
	//     hWnd     - Window handle that the message belongs to.
	//     nMessage - Specifies the message to be sent.
	//     wParam   - Specifies additional message-dependent information.
	//     lParam   - Specifies additional message-dependent information.
	//     lResult  - The return value of WindowProc. Depends on the message;
	//                may be NULL.
	//-----------------------------------------------------------------------
	int OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult);

private:
	CWnd* FindChildWindow(CXTPCommandBars* pCommandBars, UINT nID);

protected:
	CWnd* m_pControlWnd;                                // The child window.
	CRect m_rcBorders;                                  // Borders.
	BOOL  m_bControlVisible;                            // TRUE if child window is visible.
	CSize m_szControl;                                  // Size of the control.
	XTPControlCustomVerticalOptions m_verticalOptions;  // Vertical options.
	int   m_nControlID;                                 // Id of the custom control

public:

	DECLARE_XTP_CONTROL(CXTPControlCustom)
};



#endif // #if !defined(__XTPCONTROLCUSTOM_H__)
