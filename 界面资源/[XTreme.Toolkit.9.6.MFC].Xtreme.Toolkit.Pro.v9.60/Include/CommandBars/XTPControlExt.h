// XTPControlExt.h
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
#if !defined(__XTPCONTROLEXT_H__)
#define __XTPCONTROLEXT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "XTPCommandBarsDefines.h"
#include "XTPControlButton.h"

class CXTPTabClientWnd;

//===========================================================================
// Summary:
//     CXTPControlWindowList is a CXTPControlButton derived class.
//     It is used internally to create window list items.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlWindowList : public CXTPControlButton
{
protected:

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
	//     CXTPControlComboBox, CXTPControlCustom, CXTPControlEdit,
	//     CXTPControl, CXTPControlWorkspaceActions, CXTPControlToolbars,
	//     CXTPControlOleItems, CXTPControlRecentFileList, CXTPControlSelector,
	//     CXTPControlListBox
	//-----------------------------------------------------------------------
	virtual void OnCalcDynamicSize(DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to check if the user can drop the control.
	// Parameters:
	//     pCommandBar - Points to a CXTPCommandBar object
	//     point       - Mouse position.
	//     dropEffect  - DROPEFFECT enumerator.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint point, DROPEFFECT& dropEffect);

public:
	static UINT m_nFistMDIChild;        // First MDI child id.
	static UINT m_nItemCount;           // Maximum allowed items.

private:
	DECLARE_XTP_CONTROL(CXTPControlWindowList)
};


//===========================================================================
// Summary:
//     CXTPControlWorkspaceActions is a CXTPControlButton derived class. It used internally to
//     add workspace actions to window menu.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlWorkspaceActions : public CXTPControlButton
{
protected:

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
	//     CXTPControlComboBox, CXTPControlCustom, CXTPControlEdit,
	//     CXTPControlWindowList, CXTPControl, CXTPControlToolbars,
	//     CXTPControlOleItems, CXTPControlRecentFileList, CXTPControlSelector,
	//     CXTPControlListBox
	//-----------------------------------------------------------------------
	virtual void OnCalcDynamicSize(DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds action command to parent controls.
	// Parameters:
	//     pClient - Pointer to MDI tabbed client
	//     nID - Specifies the object's child-window ID.
	//     nIndex  - Index to add.
	//-----------------------------------------------------------------------
	void AddCommand(CXTPTabClientWnd* pClient, UINT nID, int& nIndex);

private:
	DECLARE_XTP_CONTROL(CXTPControlWorkspaceActions)
};

//===========================================================================
// Summary:
//     CXTPControlToolbars is a CXTPControlButton derived class.
//     It is used internally to create toolbars list items.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlToolbars : public CXTPControlButton
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Represents single toolbar item.
	//-----------------------------------------------------------------------
	class CXTPControlToolbar : public CXTPControlButton
	{
		//----------------------------------------------------------------------
		// Summary:
		//     This method is called when the control is executed.
		//----------------------------------------------------------------------
		virtual void OnExecute();
	};

protected:

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
	//     CXTPControlComboBox, CXTPControlCustom, CXTPControlEdit,
	//     CXTPControlWindowList, CXTPControlWorkspaceActions, CXTPControl,
	//     CXTPControlOleItems, CXTPControlRecentFileList, CXTPControlSelector,
	//     CXTPControlListBox
	//-----------------------------------------------------------------------
	virtual void OnCalcDynamicSize(DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to check if the user can drop the control.
	// Parameters:
	//     pCommandBar - Points to a CXTPCommandBar object
	//     point       - Mouse position.
	//     dropEffect  - DROPEFFECT enumerator.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint point, DROPEFFECT& dropEffect);

private:
	DECLARE_XTP_CONTROL(CXTPControlToolbars)
};

//===========================================================================
// Summary:
//     CXTPControlOleItems is a CXTPControlButton derived class.
//     It is used internally to create ole items.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlOleItems : public CXTPControlButton
{
protected:

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
	//     CXTPControlComboBox, CXTPControlCustom, CXTPControlEdit,
	//     CXTPControlWindowList, CXTPControlWorkspaceActions, CXTPControlToolbars,
	//     CXTPControl, CXTPControlRecentFileList, CXTPControlSelector,
	//     CXTPControlListBox
	//-----------------------------------------------------------------------
	virtual void OnCalcDynamicSize(DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to check if the user can drop the control.
	// Parameters:
	//     pCommandBar - Points to a CXTPCommandBar object
	//     point - Mouse position.
	//     dropEffect - DROPEFFECT enumerator.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint point, DROPEFFECT& dropEffect);

private:
	DECLARE_XTP_CONTROL(CXTPControlOleItems)
};

//===========================================================================
// Summary:
//     CXTPControlRecentFileList is a CXTPControlButton derived class.
//     It is used internally to create recent items list.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlRecentFileList : public CXTPControlButton
{
protected:

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
	//     CXTPControlComboBox, CXTPControlCustom, CXTPControlEdit,
	//     CXTPControlWindowList, CXTPControlWorkspaceActions, CXTPControlToolbars,
	//     CXTPControlOleItems, CXTPControl, CXTPControlSelector,
	//     CXTPControlListBox
	//-----------------------------------------------------------------------
	virtual void OnCalcDynamicSize(DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to check if the user can drop the control.
	// Parameters:
	//     pCommandBar - Points to a CXTPCommandBar object
	//     point - Mouse position.
	//     dropEffect - DROPEFFECT enumerator.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint point, DROPEFFECT& dropEffect);

private:
	DECLARE_XTP_CONTROL(CXTPControlRecentFileList)
};

//-----------------------------------------------------------------------
// Summary:
//     XTP_SLN_SELCHANGE is sent to the parent commandbar when the
//     number of selected items has changed in the control selector.
// Remarks:
//     The number of selected items changes as the mouse is moved
//     over the controls in the control selector.
//
//           XTP_SLN_SELCHANGE is sent in CXTPControlSelector::SetItemsActive.
// Example:
//     Here is an example of how an application would process the XTP_LBN_SELCHANGE
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CExtendedControlsView, CEditView)
//     //{{AFX_MSG_MAP(CExtendedControlsView)
//     ON_NOTIFY(XTP_SLN_SELCHANGE, ID_INSERT_COLUMNS, OnColumnsSelChange)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CExtendedControlsView::OnColumnsSelChange(NMHDR* pNMHDR, LRESULT* pRes)
// {
//     ASSERT(pNMHDR != NULL);
//     ASSERT(pRes != NULL);
//
//     CControlColumns* pControlColumns = DYNAMIC_DOWNCAST(CControlColumns,
//         ((NMXTPCONTROL*)pNMHDR)->pControl);
//     if (pControlColumns)
//     {
//
//         CXTPControlStatic* pInfo = FindInfoControl(pControlColumns);
//         if (pInfo)
//         {
//             int nCount = pControlColumns->m_szItemsActive.cx;
//
//             CString str = _T("Cancel");
//             if (nCount != 0)
//             {
//                 str.Format(_T("%i Columns"), nCount);
//             }
//
//             pInfo->SetWidth(pControlColumns->m_szItem.cx *
//                 pControlColumns->m_szItemsVisible.cx);
//
//
//             pInfo->SetCaption(str);
//             pInfo->DelayRedrawParent();
//         }
//
//         *pRes = 1;
//     }
// }
// </code>
// See Also:
//     CXTPControlSelector::SetItemsActive
//-----------------------------------------------------------------------
const UINT XTP_SLN_SELCHANGE    = 0x1006;


//===========================================================================
// Summary:
//     CXTPControlSelector is a CXTPControl derived class that represents
//     a control selector.
// Remarks:
//     A control selector is an x by y grid of controls of which can be
//     selected by the user.  This control is like the "Insert Table" and
//     "Insert Column" controls seen in Microsoft Word.
//
//          When the control selector is created, m_szItemsCount holds the number
//          items that will initially be shown.  By clicking and dragging inside
//          the control selector, up to m_szItemsMax items can be shown.  For example,
//          if m_szItemsCount = CSize(5, 4) and m_szItemsMax = CSize(12, 12), then
//          the control selector will always display a 5x4 grid of items unless the
//          mouse is clicked and dragged to expand the grid up to 12x12.
//
//          The size of the items is set by m_szItem, all items in the grid will
//          be sized according to m_szItem.
//
//          The current number of active items (highlighted items) is stored in
//          m_szItemsActive.  OnLButtonUp will store the number of selected items
//          in m_szResult.
//
//          When the number of selected (active\highlighted) items changes,
//          XTP_SLN_SELCHANGE is send in a message to the parent commandbar.  The
//          message is sent while in the SetItemsActive function.
//
//          The Office 2003 GUI sample illustrates how to use this class.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlSelector : public CXTPControl
{
	DECLARE_XTP_CONTROL(CXTPControlSelector)
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlSelector object.
	//-------------------------------------------------------------------------
	CXTPControlSelector();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine the size of the control.
	// Parameters:
	//     pDC - Pointer to a valid device context
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	CSize GetSize(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	void Draw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the mouse cursor moves.
	// Parameters:
	//     point - Specifies the x- and y-coordinate of the cursor.
	//-----------------------------------------------------------------------
	void OnMouseMove(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the control becomes selected.
	// Parameters:
	//     bSelected - TRUE if the control becomes selected.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL OnSetSelected(int bSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the user releases the left mouse button.
	// Parameters:
	//     point - Specifies the x- and y-coordinate of the cursor.
	//-----------------------------------------------------------------------
	void OnLButtonUp(CPoint point);

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
	//     CXTPControlComboBox, CXTPControlCustom, CXTPControlEdit,
	//     CXTPControlWindowList, CXTPControlWorkspaceActions, CXTPControlToolbars,
	//     CXTPControlOleItems, CXTPControlRecentFileList, CXTPControl,
	//     CXTPControlListBox
	//-----------------------------------------------------------------------
	virtual void OnCalcDynamicSize(DWORD dwMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to copy the control.
	// Parameters:
	//     pControl - Points to a source CXTPControl object
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	void Copy(CXTPControl* pControl, BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to set the number of active items.
	// Parameters:
	//     szActive - Number of active items (on screen).  If m_szItemsActive
	//                is not equal to szActive, then SetItemsActive will
	//                set m_szItemsActive = szActive.
	//     bNotify  - TRUE to send XTP_SLN_SELCHANGE message to parent
	//                commandbar even if m_szItemsActive = szActive.  This
	//                is necessary when the number of items has changed and
	//                m_szItemsActive = szActive, which occurs when the mouse
	//                is clicked and dragged to select items more items than
	//                are currently visible.
	// Remarks:
	//     Active items are the items that are currently highlighted in the
	//     control selector.
	// See Also: XTP_SLN_SELCHANGE
	//-----------------------------------------------------------------------
	void SetItemsActive(CSize szActive, BOOL bNotify = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Them member is called when a control item in the control
	//     selector is drawn.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     rcItem  - Bounding rectangle of item being drawn.
	//     clrText - Color to use when drawing.
	// Remarks:
	//     Override this member to draw your own custom control.  In
	//     the Office 2003 GUI sample, this is used to draw the column
	//     lines in the column selector control.
	//-----------------------------------------------------------------------
	virtual void OnDrawItem(CDC* pDC, CRect rcItem, COLORREF clrText);

public:
	CSize m_szItemsCount;   // Number of items to draw in the control selector's view.  By clicking and dragging the mouse, the number of items drawn can be up to m_szItemsMax.
	CSize m_szItemsMax;     // Maximum number of items to draw in the control selector.
	CSize m_szItemsActive;  // Number of items currently "active", this is current number of items that are highlighted as the mouse moves across the control selector.
	CSize m_szItem;         // Size of the items.
	CSize m_szResult;       // Number of items that were selected from the control selector.
	CSize m_szItemsVisible; // Number of items currently visible. Item range is from m_szItemsCount to m_szItemsMax.

private:
	BOOL  m_bLockUpdate;
};


//===========================================================================
// Summary:
//     CXTPControlLabel is a CXTPControl derived class.
//     It represents a label control.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlLabel : public CXTPControl
{
	DECLARE_XTP_CONTROL(CXTPControlLabel)
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlLabel Object.
	//-------------------------------------------------------------------------
	CXTPControlLabel()
	{
		m_dwFlags = xtpFlagSkipFocus|xtpFlagNoMovable;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the state of the control.
	// Returns:
	//     TRUE if the control is selected; otherwise FALSE.  Controls of
	//     type CXTPControlLabel can not receive focus so this will always
	//     return FALSE.
	//-----------------------------------------------------------------------
	int GetSelected() {
		return FALSE;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine of the control is a label.
	// Returns:
	//     TRUE if the control is a label, FALSE otherwise.  Controls of
	//     type CXTPControlLabel will always return TRUE.  All others will
	//     be FALSE.
	//-----------------------------------------------------------------------
	BOOL IsLabel() {
		return TRUE;
	}

};


#endif // #if !defined(__XTPCONTROLEXT_H__)
