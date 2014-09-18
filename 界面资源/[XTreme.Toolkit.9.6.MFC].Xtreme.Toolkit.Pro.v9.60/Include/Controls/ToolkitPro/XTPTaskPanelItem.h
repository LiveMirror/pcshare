// XTPTaskPanelItem.h interface for the CXTPTaskPanelItem class.
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
#if !defined(__XTPTASKPANELITEM_H__)
#define __XTPTASKPANELITEM_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPTaskPanel;
class CXTPTaskPanelPaintManager;
class CXTPTaskPanelItems;
class CXTPTaskPanelGroup;
class CXTPImageManagerIcon;
class CXTPPropExchange;

#include "XTPTaskPanelDefines.h"

//---------------------------------------------------------------------------
// Summary:
//     CXTPTaskPanelItem is a CCmdTarget derived class. It is used in TaskPanel control
//     as parent for CXTPTaskPanelGroupItem and CXTPTaskPanelGroup classes.
//---------------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTaskPanelItem : public CCmdTarget
{
	DECLARE_DYNCREATE(CXTPTaskPanelItem)

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelItem object
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTaskPanelItem object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTaskPanelItem();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get parent TaskPanel control item belongs to
	// Returns:
	//     Parent TaskPanel control.
	//-----------------------------------------------------------------------
	CXTPTaskPanel* GetTaskPanel();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the identifier of the item.
	// Parameters:
	//     nID - The new identifier of the item.
	//-----------------------------------------------------------------------
	void SetID(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the identifier of the item.
	// Returns:
	//     Identifier of the item.
	//-----------------------------------------------------------------------
	UINT GetID();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the caption of the item.
	// Parameters:
	//     lpstrCaption - The new caption of the item.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpstrCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Associates ToolTip text with the item.
	// Parameters:
	//     lpstrTooltip - The ToolTip text to display when the mouse cursor is over the item.
	//-----------------------------------------------------------------------
	void SetTooltip(LPCTSTR lpstrTooltip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the type of the item.
	// Returns:
	//     Type of the item.
	//-----------------------------------------------------------------------
	XTPTaskPanelItemType GetType();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the type of the item.
	// Parameters:
	//     typeItem - Type of the item
	//-----------------------------------------------------------------------
	void SetType(XTPTaskPanelItemType typeItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the item's caption
	// Returns:
	//     A CString object containing caption of the item.
	//-----------------------------------------------------------------------
	CString GetCaption();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the item's tooltip
	// Returns:
	//     The ToolTip text for the item.
	//-----------------------------------------------------------------------
	CString GetTooltip();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the state of the item.
	// Returns:
	//     TRUE if item is enabled; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL GetEnabled();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable/disable the item.
	// Parameters:
	//     bEnabled - TRUE to enable item.
	//-----------------------------------------------------------------------
	void SetEnabled(BOOL bEnabled);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the paint manager
	// Returns:
	//     A pointer to a CXTPTaskPanelPaintManager object
	//-----------------------------------------------------------------------
	CXTPTaskPanelPaintManager* GetPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if item is hot.
	// Returns:
	//     TRUE if item is hot; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsItemHot();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if item is pressed.
	// Returns:
	//     TRUE if item is pressed; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsItemPressed();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if item is focused.
	// Returns:
	//     TRUE if item is focused; otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL IsItemFocused();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if this item is currently dragging.
	// Returns:
	//     TRUE if this item is currently dragging.  FALSE if the item is
	//     not dragging.
	// See Also:
	//     IsItemDragOver
	//-----------------------------------------------------------------------
	BOOL IsItemDragging();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if another item is currently being
	//     dragged over this item.
	// Returns:
	//     TRUE if another item is being dragged over this item, FALSE otherwise.
	// See Also:
	//     IsItemDragging
	//-----------------------------------------------------------------------
	BOOL IsItemDragOver();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if this item is of type xtpTaskItemTypeGroup.
	// Returns:
	//     TRUE if the item is of type xtpTaskItemTypeGroup, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsGroup();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to redraw the parent TaskPanel control.
	//-----------------------------------------------------------------------
	void RedrawPanel();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set icon associated with item.
	// Parameters:
	//     nIndex - Index of the icon in the image list of parent TaskPanel control.
	//-----------------------------------------------------------------------
	void SetIconIndex(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves icon index.
	// Returns:
	//     Icon index of item.
	//-----------------------------------------------------------------------
	int GetIconIndex();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if item can be focused.
	// Returns:
	//     TRUE if item can be focused; otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsAcceptFocus();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the image of the item.
	// Parameters:
	//     nWidth - Width of the icon to be retrieved. (CXTPImageManagerIcon
	//              can have multiple image sizes for the same item.).
	//              If a 16x16 icon is to be retrieved, then pass in 16 for
	//              the width.
	// Returns:
	//     A pointer the CXTPImageManagerIcon image for this item.
	//-----------------------------------------------------------------------
	CXTPImageManagerIcon* GetImage(int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to cache a COleDataSource object that
	//     contains the group item being dragged.
	// Parameters:
	//     srcItem - COleDataSource object to cache.
	//     bCacheTextData - TRUE to cache text presentation of object.
	// Returns:
	//     TRUE if successful, FALSE if CacheGlobalData returns NULL.
	// Remarks:
	//     This places the item being dragged onto the clipboard.
	// See Also:
	//     CreateFromOleData, CacheGlobalData, CopyToClipboard,
	//     PasteFromClipboard, PrepareDrag
	//-----------------------------------------------------------------------
	BOOL PrepareDrag (COleDataSource& srcItem, BOOL bCacheTextData = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member restores a COleDataSource object.
	// Parameters:
	//     pDataObject -  COleDataSource object to restore.
	// Returns:
	//     Item that was stored in the COleDataSource object.
	// Remarks:
	//     This will restore the item that was cached using the PrepareDrag
	//     method.
	// See Also:
	//     PrepareDrag, CacheGlobalData, CopyToClipboard,
	//     PasteFromClipboard
	//-----------------------------------------------------------------------
	static CXTPTaskPanelItem* AFX_CDECL CreateFromOleData(COleDataObject* pDataObject);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to store information about the class and
	//     group item currently being dragged to HGLOBAL.
	// Returns:
	//     HGLOBAL object containing information about item being dragged, NULL
	//     if an OLE or Archive exception is thrown while caching the data.
	// See Also:
	//     PrepareDrag, CreateFromOleData, CopyToClipboard, PasteFromClipboard
	//-----------------------------------------------------------------------
	HGLOBAL CacheGlobalData();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to remove this item.
	//-------------------------------------------------------------------------
	void Remove();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to reposition the groups in the task panel.
	// Parameters:
	//     bRecalcOnly - TRUE to recalculate only without positioning.
	//-------------------------------------------------------------------------
	void RepositionPanel();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get a pointer to the array of items that this
	//     item belongs to.  This allows you to access sibling groups and\or
	//     items and determine the total number of siblings.
	// Returns:
	//     Pointer to array collection holding sibling items of this item.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItems* GetParentItems();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get a pointer to the group that this item belong to.
	// Returns:
	//     Group that this item belongs to.
	//-----------------------------------------------------------------------
	virtual CXTPTaskPanelGroup* GetItemGroup();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the index of this item within the
	//     array of items.
	// Returns:
	//     Index of item.
	//-----------------------------------------------------------------------
	int GetIndex();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if this item can be dropped.
	// Returns:
	//     TRUE if this item can be dropped, FALSE if it can't be dropped.
	//-----------------------------------------------------------------------
	BOOL IsAllowDrop();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable\disable dropping of this item.
	// Parameters:
	//     bAllowDrop - TRUE to allow this item to be dropped.  FALSE to
	//                  disabled dropping of this item.
	//-----------------------------------------------------------------------
	void AllowDrop(BOOL bAllowDrop);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable\disable dragging of this item.
	// Parameters:
	//     dragOptions - Drag options
	// Remarks:
	//     dragOptions parameter can be one or more of the following values:
	//          * <b>xtpTaskItemAllowDragCopyWithinGroup</b> To allow copy within group only
	//          * <b>xtpTaskItemAllowDragCopyWithinControl</b> To allow copy within task panel only
	//          * <b>xtpTaskItemAllowDragCopyOutsideControl</b> To allow copy outside task panel only
	//          * <b>xtpTaskItemAllowDragCopy</b> To allow copy operation
	//          * <b>xtpTaskItemAllowDragMoveWithinGroup</b> To allow move within group only
	//          * <b>xtpTaskItemAllowDragMoveWithinControl</b> To allow move within task panel only
	//          * <b>xtpTaskItemAllowDragMoveOutsideControl</b> To allow move outside task panel only
	//          * <b>xtpTaskItemAllowDragMove </b>   // To allow move operation
	//          * <b>xtpTaskItemAllowDragAll</b> To allow all drag operations
	// See Also: IsAllowDrag, XTPTaskPanelItemAllowDrag
	//-----------------------------------------------------------------------
	void AllowDrag(long nAllowDrag = xtpTaskItemAllowDragDefault);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if this item can be dragged.
	// Returns:
	//     TRUE if this item can be dragged, FALSE if it can't be dragged.
	// See Also: AllowDrag, XTPTaskPanelItemAllowDrag
	//-----------------------------------------------------------------------
	long IsAllowDrag();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the font used to display text for this item.
	// Returns:
	//     The font used to display text for this item.
	//-----------------------------------------------------------------------
	CFont* GetItemFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to save the location of the text caption
	//     within the item.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     rc      - CRect object specifying size of area.
	//     nFormat - Format of caption text. (DT_LEFT, DT_CENTER, DT_RIGHT, DT_VCENTER)
	// Remarks:
	//     CXTPTaskPanelPaintManager::DrawItemCaption calls this to save the
	//     bounding rectangle of the text within the item.
	// See Also:
	//     CXTPTaskPanelPaintManager::DrawItemCaption
	//-----------------------------------------------------------------------
	void SetTextRect(CDC* pDC, CRect rc, UINT nFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to retrieve the size and location of the caption
	//     text for the item.
	// Parameters:
	//     pFormat - Format of text. (DT_LEFT, DT_CENTER, DT_RIGHT, DT_VCENTER)
	// Returns:
	//     The size and location of the caption text within the item.
	//-----------------------------------------------------------------------
	CRect GetTextRect(UINT* pFormat = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the rectangle of the "tooltip zone"
	//     while the mouse is positioned over the group item.
	// Returns:
	//     0 in base class implementation, returns the rectangle of the "tooltip zone"
	//     while the mouse is positioned over the group item in CXTPTaskPanelGroupItem
	//     implementation.
	// Remarks:
	//     The "tooltip zone" is the area that a tooltip will be displayed
	//     while the mouse is positioned over the group item.
	//
	//     The member is overridden in CXTPTaskPanelGroupItem.
	// See Also:
	//     CXTPTaskPanelGroupItem::GetHitTestRect
	//-----------------------------------------------------------------------
	virtual CRect GetHitTestRect();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to copy this item to the clipboard.
	// See Also:
	//     PasteFromClipboard, CreateFromOleData, CacheGlobalData,
	//     PasteFromClipboard, PrepareDrag
	//-------------------------------------------------------------------------
	void CopyToClipboard();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to cat a pointer to the last item placed in the clipboard.
	// Returns:
	//     Pointer to last item placed in the clipboard.
	// See Also:
	//     CopyToClipboard, CreateFromOleData, CacheGlobalData, CopyToClipboard,
	//     PrepareDrag
	//-----------------------------------------------------------------------
	static CXTPTaskPanelItem* AFX_CDECL PasteFromClipboard();

protected:
	CXTPTaskPanel* m_pPanel;            // Parent TaskPanel class
	CString m_strCaption;               // Caption of the item.
	CString m_strTooltip;               // Tooltip of the item.
	UINT m_nID;                         // Identifier.

	XTPTaskPanelItemType m_typeItem;    // Type of the item.

	CXTPTaskPanelItems* m_pItems;       // Collection of child items.

	int m_nIconIndex;                   // Icon index.
	BOOL m_bEnabled;                    // TRUE if item is enabled.
	BOOL m_bAllowDrop;                  // TRUE if item can be dragged.
	long m_nAllowDrag;                  // TRUE if item can be dropped.

	CRect m_rcText;                     // Size and location of caption text of item.
	UINT m_nTextFormat;                 // Format of caption text of item.


	friend class CXTPTaskPanelItems;
};

AFX_INLINE CXTPTaskPanel* CXTPTaskPanelItem::GetTaskPanel() {
	ASSERT(m_pPanel != NULL);
	return m_pPanel;
}
AFX_INLINE CString CXTPTaskPanelItem::GetCaption() {
	return m_strCaption;
}
AFX_INLINE CString CXTPTaskPanelItem::GetTooltip() {
	return m_strTooltip;
}
AFX_INLINE CXTPTaskPanelItems* CXTPTaskPanelItem::GetParentItems() {
	return m_pItems;
}

#endif // !defined(__XTPTASKPANELITEM_H__)
