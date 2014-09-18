// XTPControlPopup.h : interface for the CXTPControlPopup class.
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
#if !defined(__XTPCONTROLPOPUP_H__)
#define __XTPCONTROLPOPUP_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPControl.h"

class CXTPCommandBar;
class CXTPControls;

//===========================================================================
// Summary:
//     CXTPControlPopup is a CXTPControl derived class.
//     It represents a popup-able control.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlPopup : public CXTPControl
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlPopup object
	//-----------------------------------------------------------------------
	CXTPControlPopup(void);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControlPopup object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPControlPopup(void);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a CXTPControlPopup object with the specified type.
	// Parameters:
	//     controlType - XTPControlType type of popup control. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     Popup type can be one of the following:
	//         * <b>xtpControlPopup</b> Popup type.
	//         * <b>xtpControlButtonPopup</b> Button popup.
	//         * <b>xtpControlSplitButtonPopup</b> Split button popup.
	// Returns:
	//     A pointer to a CXTPControlPopup object
	//-----------------------------------------------------------------------
	static CXTPControlPopup* AFX_CDECL CreateControlPopup(XTPControlType controlType);


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the child command bar.
	// Parameters:
	//     pBar - The child command bar.
	//-----------------------------------------------------------------------
	void SetCommandBar(CXTPCommandBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the child command bar.
	// Parameters:
	//     pMenu - The menu to be loaded as a child.
	//-----------------------------------------------------------------------
	void SetCommandBar(CMenu* pMenu);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the child command bar.
	// Returns:
	//     A pointer to the child command bar.
	//-----------------------------------------------------------------------
	CXTPCommandBar* GetCommandBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the control's popup state.
	// Returns:
	//     TRUE if the control is a popup.
	//-----------------------------------------------------------------------
	BOOL GetPopup();

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
	//     This method is called to check if pControlPopup contains in
	//     the control recursively.
	// Parameters:
	//     pControlPopup - Points to a CXTPControlPopup object needed to
	//                     check.
	// Returns:
	//     TRUE if contains; otherwise returns FALSE
	//-----------------------------------------------------------------------
	BOOL IsContainPopup(CXTPControlPopup* pControlPopup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to expand the parent popup bar.
	//-----------------------------------------------------------------------
	void ExpandCommandBar();

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
	//     This method is called when the user clicks the control.
	// Parameters:
	//     bKeyboard - TRUE if the control is selected using the keyboard.
	//     pt - Mouse cursor position.
	//-----------------------------------------------------------------------
	void OnClick(BOOL bKeyboard = FALSE, CPoint pt = CPoint(0, 0));

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called after the mouse hovers over the control.
	//-----------------------------------------------------------------------
	void OnMouseHover();

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
	//     This method is called to popup the control.
	// Parameters:
	//     bPopup - TRUE to set popup.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL OnSetPopup(BOOL bPopup);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the user releases the left mouse
	//     button.
	// Parameters:
	//     point - Specifies the x- and y-coordinate of the cursor.
	//-----------------------------------------------------------------------
	virtual void OnLButtonUp(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called when the user double-clicks the left mouse
	//     button.
	// Parameters:
	//     point - Specifies the x- and y-coordinates of the cursor.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL OnLButtonDblClk(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the user moves the mouse over the
	//     control.
	// Parameters:
	//     pDataObject - Points to a CXTPControl object
	//     point - Mouse position.
	//     dropEffect - DROPEFFECT enumerator.
	//-----------------------------------------------------------------------
	void OnCustomizeDragOver(CXTPControl* /*pDataObject*/, CPoint /*point*/, DROPEFFECT& /*dropEffect*/);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to compare controls.
	// Parameters:
	//     pOther - The control needed to compare with.
	// Returns:
	//     TRUE if the controls are identical.
	//-----------------------------------------------------------------------
	virtual BOOL Compare(CXTPControl* pOther);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to update the shadow of the control.
	//-----------------------------------------------------------------------
	virtual void UpdateShadow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Updates rectangle to set position where child popup bar become visible.
	// Parameters:
	//     rc - CRect object specifying size of area.
	//-----------------------------------------------------------------------
	virtual void AdjustExcludeRect(CRect& rc);

protected:
	BOOL            m_bPopup;           // TRUE if popuped.
	CXTPCommandBar* m_pCommandBar;      // Child popup bar.
	DWORD           m_dwCommandBarID;   // Child popup id.

public:

	DECLARE_XTP_CONTROL(CXTPControlPopup)
	friend class CXTPControls;
	friend class CXTPPopupBar;
	friend class CXTPCommandBar;

};

//////////////////////////////////////////////////////////////////////////


AFX_INLINE CXTPCommandBar* CXTPControlPopup::GetCommandBar() {
	return m_pCommandBar;
}
AFX_INLINE BOOL CXTPControlPopup::GetPopup() {
	return m_bPopup;
}

#endif //#if !defined(__XTPCONTROLPOPUP_H__)
