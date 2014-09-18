// XTPControlListBox.h : interface for the CXTPControlListBox class.
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
#if !defined(__XTPCONTROLLISTBOX_H__)
#define __XTPCONTROLLISTBOX_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//-----------------------------------------------------------------------
// Summary:
//     The XTP_LBN_SELCHANGE is sent to active view when the selection of CXTPControlListBox
//     is changed
// Example:
//     Here is an example of how an application would process the XTP_LBN_SELCHANGE
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_NOTIFY(XTP_LBN_SELCHANGE, ID_EDIT_UNDO, OnListBoxControlSelChange)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CMainFrame::OnListBoxControlSelChange(NMHDR* pNMHDR, LRESULT* pResult)
// {
//     CXTPControlListBox* pControlListBox = DYNAMIC_DOWNCAST(CXTPControlListBox, ((NMXTPCONTROL*)pNMHDR)->pControl);
//     if (pControlListBox)
//     {
//          *pResult = 1;
//     }
// }
// </code>
// See Also: CXTPControlListBox, XTP_LBN_POPUP
//-----------------------------------------------------------------------
const UINT XTP_LBN_SELCHANGE   = 0x1003;

//-----------------------------------------------------------------------
// Summary:
//     The XTP_LBN_POPUP is sent to active view when the CXTPControlListBox is about to become visible.
// Example:
//     Here is an example of how an application would process the XTP_LBN_POPUP
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
//     //{{AFX_MSG_MAP(CMainFrame)
//     ON_NOTIFY(XTP_LBN_POPUP, ID_EDIT_UNDO, OnListBoxControlPopup)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CMainFrame::OnListBoxControlPopup(NMHDR* pNMHDR, LRESULT* pResult)
// {
//     CXTPControlListBox* pControlListBox = DYNAMIC_DOWNCAST(CXTPControlListBox, ((NMXTPCONTROL*)pNMHDR)->pControl);
//     if (pControlListBox)
//     {
//          *pResult = 1;
//     }
// }
// </code>
// See Also: CXTPControlListBox, XTP_LBN_SELCHANGE
//-----------------------------------------------------------------------
const UINT XTP_LBN_POPUP    = 0x1004;

//===========================================================================
// Summary:
//     CXTPControlComboBox is a CXTPControl derived class.
//     It represents a list box control.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlListBox : public CXTPControl
{
private:
	class CListBoxExt;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlListBox object
	//-----------------------------------------------------------------------
	CXTPControlListBox();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControlListBox object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPControlListBox();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the width of the control
	// Parameters:
	//     nWidth - The width of the control
	//-----------------------------------------------------------------------
	void SetWidth(int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the total minimum and maximum lines.
	// Parameters:
	//     nMin - Minimum lines to be used.
	//     nMax - Maximum lines to be used
	//-----------------------------------------------------------------------
	void SetLinesMinMax(int nMin, int nMax);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve a pointer to the list control.
	// Returns:
	//     Retrieves list control.
	//-----------------------------------------------------------------------
	CListBox* GetListCtrl ();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the multiple selection style of the
	//     control.
	// Parameters:
	//     bMultiplSel - TRUE to set multiple selection style.
	//-----------------------------------------------------------------------
	void SetMultiplSel(BOOL bMultiplSel);

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
	//     This method is called to determine the size of the control.
	// Parameters:
	//     pDC - Pointer to a valid device context
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize GetSize(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to assign a parent command bar object.
	// Parameters:
	//     pParent - Points to a CXTPCommandBar object
	//-----------------------------------------------------------------------
	virtual void SetParent(CXTPCommandBar* pParent);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//-----------------------------------------------------------------------
	virtual void Draw(CDC* pDC);

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
	//     This method is called when the control becomes selected.
	// Parameters:
	//     bSelected - TRUE if the control becomes selected.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL OnSetSelected(int bSelected);

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
	//     CXTPControl
	//-----------------------------------------------------------------------
	virtual void OnCalcDynamicSize(DWORD dwMode);

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
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);


protected:
	CListBoxExt* m_pListBox;            // List box control.
	int          m_nListBoxLinesMin;    // Minimum lines.
	int          m_nListBoxLinesMax;    // Maximum lines.
	BOOL         m_bMultiplSel;         // Multiple selection style.
	int          m_nWidth;              // Width of the control.
	BOOL         m_bMouseLocked;        // TRUE to lock mouse events.


	DECLARE_XTP_CONTROL(CXTPControlListBox)

	friend class CListBoxExt;
};

//===========================================================================
// Summary:
//     CXTPControlStatic is a CXTPControl derived class.
//     It represents a static text control.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlStatic : public CXTPControl
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlStatic object
	//-----------------------------------------------------------------------
	CXTPControlStatic();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the width of the control
	// Parameters:
	//     nWidth - The width of the control
	//-----------------------------------------------------------------------
	void SetWidth(int nWidth);

protected:
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
	//     This method is called to copy the control.
	// Parameters:
	//     pControl - Points to a source CXTPControl object
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	virtual void Copy(CXTPControl* pControl, BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);


private:
	int m_nWidth;

private:
	DECLARE_XTP_CONTROL(CXTPControlStatic)

};


#endif // !defined(__XTPCONTROLLISTBOX_H__)
