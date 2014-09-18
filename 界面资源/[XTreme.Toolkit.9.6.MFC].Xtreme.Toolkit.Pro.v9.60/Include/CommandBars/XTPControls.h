// XTPControls.h : interface for the CXTPControls class.
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
#if !defined(__XTPCONTROLS_H__)
#define __XTPCONTROLS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "XTPCommandBarsDefines.h"

class CXTPCommandBar;
class CXTPControl;
class CXTPCommandBars;
class CXTPCommandBarList;
class CXTPPropExchange;
struct XTP_COMMANDBARS_PROPEXCHANGE_PARAM;

//===========================================================================
// Summary:
//     CXTPControls is a CCmdTarget derived class. It represents a collection
//     of the controls.
//===========================================================================
class _XTP_EXT_CLASS CXTPControls : public CCmdTarget
{
	DECLARE_DYNCREATE(CXTPControls)

private:
	struct XTPBUTTONINFO;       // Internal helper structure.

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Protected constructor used by dynamic creation
	//-----------------------------------------------------------------------
	CXTPControls();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControls object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPControls();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to return the control at the specified index.
	// Parameters:
	//     nIndex - An integer index.
	// Returns:
	//     The CXTPControl pointer currently at this index.
	//-----------------------------------------------------------------------
	CXTPControl* GetAt(int nIndex);


	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to add a new control.
	// Parameters:
	//     controlType  - The type of the control to be added.
	//     nId          - Identifier of the control to be added.
	//     strParameter - Parameter of the control to be added.
	//     nBefore      - Index of the control to be inserted.
	//     bTemporary   - TRUE if this control is temporary.
	// Returns:
	//     A pointer to the added control.
	//-----------------------------------------------------------------------
	CXTPControl* Add(XTPControlType controlType, int nId, CString strParameter = _T(""), int nBefore = -1, BOOL bTemporary = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to add a new control.
	// Parameters:
	//     pControl     - A pointer to the control to be added.
	//     nId          - Identifier of the control to be added.
	//     strParameter - Parameter of the control to be added.
	//     nBefore      - Index of the control to be inserted.
	//     bTemporary   - TRUE if this control is temporary.
	// Returns:
	//     A pointer to the added control.
	//-----------------------------------------------------------------------
	CXTPControl* Add(CXTPControl* pControl, int nId, CString strParameter = _T(""), int nBefore = -1, BOOL bTemporary = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to add a new control.
	// Parameters:
	//     pMenu  - Menu that contains the control.
	//     nIndex - Index of the control to be added.
	// Returns:
	//     Pointer to the added control.
	//-----------------------------------------------------------------------
	CXTPControl* AddMenuItem(CMenu* pMenu, int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove a control.
	// Parameters:
	//     pControl - Control to be removed.
	//-----------------------------------------------------------------------
	void Remove(CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove a control.
	// Parameters:
	//     nIndex - Index of the control to be removed.
	//-----------------------------------------------------------------------
	void Remove(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to remove all controls.
	//-----------------------------------------------------------------------
	void RemoveAll(void);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the count of the controls
	// Returns:
	//     The count of the controls.
	//-----------------------------------------------------------------------
	int GetCount();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to find a control by its identifier.
	// Parameters:
	//     nId - Identifier of the control to find.
	// Returns:
	//     A pointer to the CXTPControl object if successful; otherwise returns NULL.
	//-----------------------------------------------------------------------
	CXTPControl* FindControl(int nId);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to find the specified control.
	// Parameters:
	//     type       - The type of the control to find.
	//     nId        - The control's identifier.
	//     bVisible   - TRUE if the control is visible.
	//     bRecursive - TRUE to find in the nested command bars.
	// Returns:
	//     Pointer to the CXTPControl object if successful; otherwise returns NULL.
	//-----------------------------------------------------------------------
	CXTPControl* FindControl(XTPControlType type, int nId, BOOL bVisible, BOOL bRecursive);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to convert menu items to command bar controls.
	// Parameters:
	//     pMenu           - Menu to be converted.
	// Returns:
	//     TRUE if the method was successful; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL LoadMenu(CMenu* pMenu);


public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the first control.
	// Returns:
	//     A pointer to a CXTPControl object
	//-----------------------------------------------------------------------
	CXTPControl* GetFirst();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the next control.
	// Parameters:
	//     pControl - Current control.
	//-----------------------------------------------------------------------
	void GetNext(CXTPControl*& pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method determines where a point lies in a specified control.
	// Parameters:
	//     point - Specifies the point to be tested.
	// Returns:
	//     A pointer to a CXTPControl control that occupies the specified
	//     point or NULL if no control occupies the point.
	//-----------------------------------------------------------------------
	CXTPControl* HitTest(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to change the type of the control.
	// Parameters:
	//     pControl - Points to a CXTPControl object
	//     type - Type needed to be changed.
	// Returns:
	//     A pointer to a CXTPControl object
	//-----------------------------------------------------------------------
	CXTPControl* SetControlType(CXTPControl* pControl, XTPControlType type);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to change the type of the control.
	// Parameters:
	//     nIndex - Index of the control
	//     type - Type needed to be changed.
	// Returns:
	//     A pointer to a CXTPControl object
	//-----------------------------------------------------------------------
	CXTPControl* SetControlType(int nIndex, XTPControlType type);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to add the copy of the control.
	// Parameters:
	//     pClone - Points to a CXTPControl object needed to copy.
	//     nBefore - Index of the control to be inserted.
	//     bRecursive - TRUE to copy recursively.
	// Returns:
	//     A pointer to an added CXTPControl object
	//-----------------------------------------------------------------------
	CXTPControl* AddClone(CXTPControl* pClone, int nBefore = -1, BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called internally to reposition controls and
	//     determine their size.
	// Parameters:
	//     pDC      - Pointer to a valid device context
	//     nLength  - Length of the parent bar.
	//     dwMode   - Mode of the parent bar.
	//     rcBorder - Border to be set.
	//     nWidth   - Width to be set.
	// Returns:
	//     Total size of the controls.
	//-----------------------------------------------------------------------
	CSize CalcDynamicSize(CDC* pDC, int nLength, DWORD dwMode, const CRect& rcBorder, int nWidth = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the number of visible controls.
	// Parameters:
	//     bIgnoreWrap - TRUE to ignore controls that are wrapped.
	// Returns:
	//     Number of visible controls.
	//-----------------------------------------------------------------------
	int GetVisibleCount(BOOL bIgnoreWrap = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to attach the specified command bars object.
	// Parameters:
	//     pCommandBars - Command bars object need to attach.
	// Remaks:
	//     You can call this member if you create plain toolbar and want to set CommandBars for it.
	//-----------------------------------------------------------------------
	void SetCommandBars(CXTPCommandBars* pCommandBars);

	//-----------------------------------------------------------------------
	// Summary:
	//     Finds the next control in the given direction.
	// Parameters:
	//     nIndex - Current control index.
	//     nDirection - Direction to find.
	//     bKeyboard - TRUE to skip controls with xtpFlagSkipFocus flag.
	//     bSkipTemporary - TRUE to skip all controls with Temporary flag
	//     bSkipCollapsed - TRUE to sekip all controls  with xtpHideExpand hide flag.
	// Returns:
	//     Index of the next control.
	//-----------------------------------------------------------------------
	long GetNext(long nIndex, int nDirection = +1, BOOL bKeyboard = TRUE, BOOL bSkipTemporary = FALSE, BOOL bSkipCollapsed = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Makes a copy of all controls.
	// Parameters:
	//     bRecursive - TRUE to copy recursively.
	// Returns:
	//     A pointer to a CXTPControls object.
	//-----------------------------------------------------------------------
	CXTPControls* Duplicate(BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the parent command bars object.
	// Returns:
	//     A pointer to a CXTPCommandBars object
	//-----------------------------------------------------------------------
	CXTPCommandBars* GetCommandBars();

	//-----------------------------------------------------------------------
	// Summary:
	//     Move the control to a specified position.
	// Parameters:
	//     pControl - Points to a CXTPControl object
	//     nBefore  - Index to be moved.
	//-----------------------------------------------------------------------
	void MoveBefore(CXTPControl* pControl, int nBefore);

	//-----------------------------------------------------------------------
	// Summary:
	//     Attaches the parent command bar.
	// Parameters:
	//     pParent - Points to a CXTPCommandBar object
	//-----------------------------------------------------------------------
	void SetParent(CXTPCommandBar* pParent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates original controls set.
	//-----------------------------------------------------------------------
	void CreateOriginalControls();

	//-----------------------------------------------------------------------
	// Summary:
	//     Removes the original controls set.
	//-----------------------------------------------------------------------
	void ClearOriginalControls();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves original controls set.
	// Returns:
	//     A pointer to a CXTPControls object
	//-----------------------------------------------------------------------
	CXTPControls* GetOriginalControls();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to compare the controls.
	// Parameters:
	//     pControls - Points to a CXTPControls object
	// Returns:
	//     TRUE if the controls are identical.
	//-----------------------------------------------------------------------
	BOOL Compare(CXTPControls* pControls);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the changed status flag of the control.
	// Returns:
	//     TRUE if the internal control state was changed since last drawing,
	//     FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

protected:


private:
	void GenerateCommandBarList(DWORD& nID, CXTPCommandBarList* pCommandBarList, XTP_COMMANDBARS_PROPEXCHANGE_PARAM* pParam);
	void RestoreCommandBarList(CXTPCommandBarList* pCommandBarList);
	void RefreshIndexes();
	void AddTail(CXTPControl* pControl);
	CSize _CalcSize(XTPBUTTONINFO* pData, BOOL bCommit = FALSE, BOOL bVert = FALSE);
	int _WrapToolBar(XTPBUTTONINFO* pData, int nWidth, DWORD& dwMode);
	void _SizeFloatableBar(XTPBUTTONINFO* pData, int nLength, DWORD dwMode);
	void _AdjustBorders(XTPBUTTONINFO* pData, CSize& sizeResult, DWORD dwMode, CRect rcBorder);
	void _CenterControlsInRow(XTPBUTTONINFO* pData, int nFirst, int nLast, int, BOOL, CSize sizeResult, CRect );
	void _MoveRightAlligned(XTPBUTTONINFO* pData, CSize sizeResult, CRect rcBorder, DWORD dwMode);

private:
	CXTPCommandBar* m_pParent;
	CArray<CXTPControl*, CXTPControl*> m_arrControls;
	CXTPCommandBars* m_pCommandBars;

	CXTPControls* m_pOriginalControls;

public:


	friend class CXTPCommandBar;
	friend class CXTPCommandBars;
	friend class CXTPToolBar;
	friend class CXTPMenuBar;

};

//////////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPControl* CXTPControls::GetAt(int nIndex) {
	ASSERT(nIndex < m_arrControls.GetSize());
	return (nIndex >= 0 && nIndex < m_arrControls.GetSize())? m_arrControls.GetAt(nIndex): NULL;
}
AFX_INLINE int CXTPControls::GetCount() {
	return (int)m_arrControls.GetSize();
}
AFX_INLINE CXTPControls* CXTPControls::GetOriginalControls() {
	return m_pOriginalControls;
}


#endif // #if !defined(__XTPCONTROLS_H__)
