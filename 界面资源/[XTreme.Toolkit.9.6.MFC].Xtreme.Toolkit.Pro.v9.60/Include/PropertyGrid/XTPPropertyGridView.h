// XTPPropertyGridView.h interface for the CXTPPropertyGridView class.
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
#if !defined(__XTPPROPERTYGRIDVIEW_H__)
#define __XTPPROPERTYGRIDVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTPPropertyGridPaintManager.h"
#include "CommonPro.Include/XTPDrawHelpers.h"

class CXTPPropertyGridView;
class CXTPPropertyGridPaintManager;
class CXTPPropertyGrid;

//===========================================================================
// Summary:
//     CXTPPropertyGridToolTip is a CWnd derived class.
//     It is an internal class used by Property Grid control
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridToolTip : public CWnd
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Activate Tooltip control.
	// Parameters:
	//     bActive - TRUE to activate.
	//-----------------------------------------------------------------------
	void Activate(BOOL bActive);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates Tooltip control.
	// Parameters:
	//     pParentWnd - Points to a CXTPPropertyGridView object.
	//-----------------------------------------------------------------------
	void Create(CXTPPropertyGridView* pParentWnd);

protected:
	CXTPPropertyGridView* m_pGrid;      // Parent window.

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CXTPPropertyGridView)
	afx_msg BOOL OnEraseBkgnd(CDC*);
	afx_msg void OnPaint();
	afx_msg UINT OnNcHitTest(CPoint point);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
};

AFX_INLINE BOOL CXTPPropertyGridToolTip::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


//===========================================================================
// Summary:
//     CXTPPropertyGridView is a CListBox derived class.
//     It is an internal class used by Property Grid control
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridView : public CListBox
{
	DECLARE_DYNAMIC(CXTPPropertyGridView)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridView object
	//-----------------------------------------------------------------------
	CXTPPropertyGridView();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridView object, handles cleanup and
	//     deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPropertyGridView();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the position of the divider.
	// Returns:
	//     Position of the divider
	//-----------------------------------------------------------------------
	int GetDividerPos();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method determines where a point lies in a specified item.
	// Parameters:
	//     point - Specifies the point to be tested.
	// Returns:
	//     A pointer to a CXTPPropertyGridItem object
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* ItemFromPoint(CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to refresh items in the list.
	//-----------------------------------------------------------------------
	void Refresh();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to remove all items in the list.
	//-----------------------------------------------------------------------
	void ResetContent();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves an item with the specified index.
	// Parameters:
	//     nIndex - Specifies the zero-based index of the item to retrieve.
	// Returns:
	//     A pointer to a CXTPPropertyGridItem object
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* GetItem(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add a new category to a Property
	//     Grid Control.
	// Parameters:
	//     strCaption - Name of the category to add.
	//     pCategory  - Points to a CXTPPropertyGridItem object.
	// Returns:
	//     A pointer to a CXTPPropertyGridItem object.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* AddCategory(CString strCaption, CXTPPropertyGridItem* pCategory = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to insert a new category to the Property Grid control.
	// Parameters:
	//     strCaption - Name of the category to add.
	//     nIndex     - Index of category to be inserted
	//     pCategory  - Points to a CXTPPropertyGridItem object.
	// Returns:
	//     The pointer to the item object of the newly inserted category.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItem* InsertCategory(int nIndex, CString strCaption, CXTPPropertyGridItem* pCategory = NULL);

private:

	CXTPPropertyGridItem* GetSelectedItem();
	void SetPropertySort(XTPPropertyGridSortOrder sort, BOOL bRrefresh = FALSE);
	int InsertItem(CXTPPropertyGridItem* pItem, int nIndex);
	int _DoExpand(CXTPPropertyGridItem* pItem, int nIndex);
	void _DoCollapse(CXTPPropertyGridItem* pItem);
	void _RefreshIndexes();
	void SwitchExpandState(int nItem);
	int HitTest(CPoint point);

	void ShowToolTip(CPoint pt);
	void _ShowToolTip(CRect rcBound, CRect rcText);
	CSize _GetTextExtent(CString str);
	void RelayToolTipEvent(UINT message);


public:
	CFont                         m_fontNormal;         // Normal font.
	CFont                         m_fontBold;           // Bold font.

	CXTPPaintManagerColor         m_clrLine;            // Line color.
	CXTPPaintManagerColor         m_clrFore;            // Color of the item's text.
	CXTPPaintManagerColor         m_clrCategoryFore;    // Color of the category text.
	CXTPPaintManagerColor         m_clrBack;            // Background color.
	CXTPPaintManagerColor         m_clrReadOnlyFore;    // Color of read-only text.
	CXTPPropertyGrid*             m_pGrid;              // Parent CXTPPropertyGrid class.

protected:
	BOOL                          m_bTracking;          // TRUE if grid in tracking mode.
	CRect                         m_rcToolTip;          // Position of the Tooltip.
	double                        m_dDivider;           // Divider position.
	HCURSOR                       m_hCursor;            // Handle of the cursor.
	CString                       m_strTipText;         // Tooltip text.
	XTPPropertyGridSortOrder      m_properetySort;      // Current sort order.
	CXTPPropertyGridItem*         m_pSelected;          // Current selected item.
	CXTPPropertyGridItems*        m_pCategories;        // Collection of the categories.
	CXTPPropertyGridToolTip       m_wndTip;             // Tooltip control.
	CXTPPropertyGridInplaceEdit   m_wndEdit;            // In-place edit control.
	CXTPPropertyGridInplaceButton m_wndButton;          // In-place button control.
	CXTPPropertyGridInplaceList   m_wndListBox;         // In-place list control.
	int                           m_nLockUpdate;        // TRUE to lock update.


protected:
//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_VIRTUAL(CXTPPropertyGridView)
	virtual void DrawItem(LPDRAWITEMSTRUCT);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT);

	virtual int CompareItem(LPCOMPAREITEMSTRUCT) { return 0;}
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPPropertyGridView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSelectionChanged();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChar(UINT nChar, UINT nRepCntr, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus (CWnd* pNewWnd);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	DECLARE_MESSAGE_MAP()

	friend class CXTPPropertyGridItem;
	friend class CXTPPropertyGridItems;
	friend class CXTPPropertyGrid;
	friend class CXTPPropertyGridInplaceEdit;
	friend class CPropertyGridCtrl;
	friend class CXTPPropertyGridToolTip;
};


#endif // #if !defined(__XTPPROPERTYGRIDVIEW_H__)
