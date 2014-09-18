// XTPTaskPanelItems.h interface for the CXTPTaskPanelItems class.
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
#if !defined(__XTPTASKPANELITEMS_H__)
#define __XTPTASKPANELITEMS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPTaskPanelGroup;
class CXTPTaskPanelItem;
class CXTPTaskPanel;
class CXTPPropExchange;

//===========================================================================
// Summary:
//     CXTPTaskPanelItems is a CCmdTarget derived class. It is used as collection of
//     CXTPTaskPanelItem classes.
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelItems : public CCmdTarget
{
	DECLARE_DYNCREATE(CXTPTaskPanelItems)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelItems object.
	// Parameters:
	//     pPanel - Pointer to the task panel to add the items to.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItems(CXTPTaskPanel* pPanel = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTaskPanelItems object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPTaskPanelItems();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to add a new item to the collection.
	// Parameters:
	//     pItem - Points to a CXTPTaskPanelItem to be added.
	//     nID - Identifier of the item.
	// Returns:
	//     A pointer to a CXTPTaskPanelItem object added.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem* Add(CXTPTaskPanelItem* pItem, UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to insert an item at a specific position
	//     within the collection of items.
	// Parameters:
	//     nIndex - Position in collection\array to insert the new item.
	//     pItem  - Pointer to a valid task panel item\group.
	// Remarks:
	//     To move an item, use the MOVE function.
	// Returns:
	//     A pointer to the newly inserted item.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem* InsertAt(int nIndex, CXTPTaskPanelItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to return the item at the specified index
	// Parameters:
	//     nIndex - Specifies the zero-based index of the item to retrieve.
	// Returns:
	//     A pointer to a CXTPTaskPanelItem object.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem* GetAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to remove all items in the collection.
	// Parameters:
	//     bReposition - TRUE to reposition items after remove.
	// See Also: CXTPTaskPanel::Reposition
	//-----------------------------------------------------------------------
	void Clear(BOOL bReposition = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to find an item by its ID
	// Parameters:
	//     nID - Identifier of item to be found.
	// Returns:
	//     A pointer to item if successful; otherwise returns NULL.
	//-----------------------------------------------------------------------
	CXTPTaskPanelItem* Find(UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to remove the specified item.
	// Parameters:
	//     pItem - Item to be removed.
	//-----------------------------------------------------------------------
	void Remove(CXTPTaskPanelItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to remove the specified item by its index.
	// Parameters:
	//     nIndex - Specifies the zero-based index of the item to remove.
	//-----------------------------------------------------------------------
	void RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get index of item in the collection of items.
	// Parameters:
	//     pItem - Item to be tested
	// Returns:
	//     Index of item in collection.
	//-----------------------------------------------------------------------
	int GetIndex(CXTPTaskPanelItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get count of items
	// Returns:
	//     Number of items.
	//-----------------------------------------------------------------------
	int GetCount();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get a pointer to the owner
	// Returns:
	//     A pointer to a CCmdTarget object owned by this collection
	//-----------------------------------------------------------------------
	CCmdTarget* GetOwner();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to move a Group\Item to the specified location
	//     within the collection of items.
	// Parameters:
	//     pItem  - Group\Item to be moved.
	//     nIndex - New position of the item within the collection of items.
	// Remarks:
	//     A Group\Item will be displayed in the order that they are added
	//     to the collection of items.  They are stored in a zero based array
	//     of items.  The items position within the array can be changed by
	//     specifying an index to move the item to.
	//-----------------------------------------------------------------------
	void Move(CXTPTaskPanelItem* pItem, int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);


	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves default runtime class of items in collection.
	//-----------------------------------------------------------------------
	virtual CRuntimeClass* GetDefaultItemClass();

protected:
	CXTPTaskPanel* m_pPanel;                                    // Parent TaskPanel control.
	CArray<CXTPTaskPanelItem*, CXTPTaskPanelItem*> m_arrItems;  // Array of items.
	CCmdTarget* m_pOwner;                                       // Owner of items. Can be TaskPanel or Group.

	friend class CXTPTaskPanel;
	friend class CXTPTaskPanelGroup;
};


//===========================================================================
// Summary:
//     CXTPTaskPanelGroupItems is a CXTPTaskPanelItems derived class.
//     It is used as collection of items in the group.
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelGroupItems : public CXTPTaskPanelItems
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs CXTPTaskPanelGroupItems object
	// Parameters:
	//     pPanel - Points to a CXTPTaskPanel object
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroupItems(CXTPTaskPanel* pPanel);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves default runtime class of items in collection.
	//-----------------------------------------------------------------------
	virtual CRuntimeClass* GetDefaultItemClass();

};

//===========================================================================
// Summary:
//     CXTPTaskPanelGroups is a CXTPTaskPanelItems derived class.
//     It is used as a collection of groups in the control.
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelGroups : public CXTPTaskPanelItems
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelGroups object
	// Parameters:
	//     pPanel - Points to a CXTPTaskPanel object
	//-----------------------------------------------------------------------
	CXTPTaskPanelGroups(CXTPTaskPanel* pPanel);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves default runtime class of items in collection.
	//-----------------------------------------------------------------------
	virtual CRuntimeClass* GetDefaultItemClass();

};


AFX_INLINE int CXTPTaskPanelItems::GetCount()
{
	return (int)m_arrItems.GetSize();
}
AFX_INLINE CXTPTaskPanelItem* CXTPTaskPanelItems::GetAt(int nIndex)
{
	return nIndex >= 0 && nIndex < m_arrItems.GetSize()? m_arrItems.GetAt(nIndex): NULL;
}
AFX_INLINE CCmdTarget* CXTPTaskPanelItems::GetOwner()
{
	return m_pOwner;
}

#endif // !defined(__XTPTASKPANELITEMS_H__)
