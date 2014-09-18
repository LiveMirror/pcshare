// XTOutBarCtrl.h interface for the CXTOutBarCtrl class.
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
#if !defined(__XTOUTBARCTRL_H__)
#define __XTOUTBARCTRL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Theme/XTOutBarTheme.h"

// forwards
class CXTOutBarItem;
class CXTOutBarFolder;
class CXTOutBarEditItem;
class CXTOutBarTheme;

const DWORD OBS_XT_SMALLICON       = 0x0001;   //<ALIAS CXTOutBarCtrl::ModifyFlag@const DWORD&@const DWORD&@const bool>
const DWORD OBS_XT_LARGEICON       = 0x0002;   //<ALIAS CXTOutBarCtrl::ModifyFlag@const DWORD&@const DWORD&@const bool>
const DWORD OBS_XT_EDITGROUPS      = 0x0004;   //<ALIAS CXTOutBarCtrl::ModifyFlag@const DWORD&@const DWORD&@const bool>
const DWORD OBS_XT_EDITITEMS       = 0x0008;   //<ALIAS CXTOutBarCtrl::ModifyFlag@const DWORD&@const DWORD&@const bool>
const DWORD OBS_XT_REMOVEGROUPS    = 0x0010;   //<ALIAS CXTOutBarCtrl::ModifyFlag@const DWORD&@const DWORD&@const bool>
const DWORD OBS_XT_REMOVEITEMS     = 0x0020;   //<ALIAS CXTOutBarCtrl::ModifyFlag@const DWORD&@const DWORD&@const bool>
const DWORD OBS_XT_ADDGROUPS       = 0x0040;   //<ALIAS CXTOutBarCtrl::ModifyFlag@const DWORD&@const DWORD&@const bool>
const DWORD OBS_XT_DRAGITEMS       = 0x0080;   //<ALIAS CXTOutBarCtrl::ModifyFlag@const DWORD&@const DWORD&@const bool>
const DWORD OBS_XT_ANIMATION       = 0x0100;   //<ALIAS CXTOutBarCtrl::ModifyFlag@const DWORD&@const DWORD&@const bool>
const DWORD OBS_XT_SELHIGHLIGHT    = 0x0200;   //<ALIAS CXTOutBarCtrl::ModifyFlag@const DWORD&@const DWORD&@const bool>
const DWORD OBS_XT_DEFAULT         = 0x00FC;   //<ALIAS CXTOutBarCtrl::ModifyFlag@const DWORD&@const DWORD&@const bool>

// ---------------------------------------------------------------------
// Summary:
//     XT_OUTBAR_INFO structure is used by by the CXTOutBarCtrl
//     class to store and information for an Outlook bar folder or item.
// See Also:
//     CXTOutBarCtrl, CXTOutBarCtrl::OnLabelChanged
// ---------------------------------------------------------------------
struct XT_OUTBAR_INFO
{
	int     nIndex;    // Index of the item.
	int     nDragTo;   // Ending drag index.
	int     nDragFrom; // Starting drag index.
	bool    bFolder;   // true if the item is a folder
	LPCTSTR lpszText;  // Item text.
};

//===========================================================================
// Summary:
//     CXTOutBarCtrl is a CWnd derived class. It is used to create a shortcut
//     bar window similar to the shortcut bar seen in Outlook™.
//===========================================================================
class _XT_EXT_CLASS CXTOutBarCtrl : public CWnd, public CXTThemeManagerStyleHost
{
	DECLARE_DYNCREATE(CXTOutBarCtrl)
	DECLARE_THEME_HOST(CXTOutBarCtrl, CXTOutBarTheme)

public:
	// ----------------------------------------------------------------------
	// Summary:
	//     Enumeration to determine an item type.
	// Remarks:
	//     HitTestCode type defines the constants used to determine the
	//     type of item that was found during a HitTestEx operation.
	// See Also:
	//     CXTOutBarCtrl, CXTOutBarCtrl::HitTestEx
	//
	// <KEYWORDS hitNone, hitFolder, hitItem, hitUpScroll, hitDnScroll>
	// ----------------------------------------------------------------------
	enum HitTestCode
	{
		hitNone      =-1, // No item was found.
		hitFolder    = 0, // The item found was a folder.
		hitItem      = 1, // The item found was a shortcut.
		hitUpScroll  = 2, // The item found was a up arrow.
		hitDnScroll  = 3  // The item found was a down arrow.
	};

	// ----------------------------------------------------------------------
	// Summary:
	//     Enumeration to determine an item's size.
	// Remarks:
	//     RectItem type defines the constants used to determine the
	//     size or the item or one of its components (label or icon). This is
	//     used to determine how the control should be rendered.
	// See Also:
	//     CXTOutBarCtrl, CXTOutBarCtrl::GetItemSize
	//
	// <KEYWORDS rectItemIcon, rectItemLabel, rectItemBoth>
	// ----------------------------------------------------------------------
	enum RectItem
	{
		rectItemIcon = 1,  // Area is the size of an item icon.
		rectItemLabel = 2, // Area is the size of an item label.
		rectItemBoth = 3   // Area is the size of both the item icon and label combined.
	};

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTOutBarCtrl object
	//-----------------------------------------------------------------------
	CXTOutBarCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTOutBarCtrl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTOutBarCtrl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set an animation effect for the currently
	//     selected item. It is not to be used with OBS_XT_SELHIGHLIGHT flag.
	// Parameters:
	//     iTime - Specifies the time in milliseconds that the selected item will animate.
	//-----------------------------------------------------------------------
	virtual void SetAnimSelHighlight(const int iTime);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the item data that was set for
	//     the specified folder.
	// Parameters:
	//     iFolder - The index of the folder to retrieve item data for. If -1, the currently
	//               selected folder item data is returned.
	// Returns:
	//     A DWORD value.
	//-----------------------------------------------------------------------
	virtual DWORD GetFolderData(int iFolder = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the CWnd object that has been set
	//     for the folder specified by 'iFolder'.
	// Parameters:
	//     iFolder - Index of the folder to retrieve the CWnd object for, if -1 the currently
	//               selected folder CWnd object is used.
	// Returns:
	//     If 'iFolder' is -1, the child of the currently selected folder is returned.
	//     If no object has been set for the folder, the return value is NULL.
	//-----------------------------------------------------------------------
	virtual CWnd* GetFolderChild(int iFolder = -1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add a folder with a CWnd child nested
	//     inside of it. You can insert a folder with any CWnd object, such as
	//     a tree control (see the OutlookBar sample).
	// Parameters:
	//     lpszFolderName - Name of the folder to add.
	//     pWndChild      - Points to a valid CWnd object. The object must be created before
	//                      inserting.
	//     dwData         - Item data (lParam) for the folder.
	// Returns:
	//     The integer value that represents the index of the added folder.
	//-----------------------------------------------------------------------
	virtual int AddFolderBar(LPCTSTR lpszFolderName,CWnd* pWndChild,const DWORD dwData = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the text of the specified item for
	//     the currently selected folder.
	// Parameters:
	//     iIndex - Index of the item to retrieve the text for.
	// Returns:
	//     A CString object containing the retrieved text.
	//-----------------------------------------------------------------------
	virtual CString GetItemText(const int iIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the tick count, in milliseconds, between
	//     each animation frame in folder scrolling. If you set a value of -1,
	//     or minor, no animation will be played. Animation requires the OBS_XT_ANIMATION
	//     flag be set.
	// Parameters:
	//     lValue - Specifies the time in milliseconds between animation. A value of
	//              -1 will disable animation playback.
	//-----------------------------------------------------------------------
	virtual void SetAnimationTickCount(const long lValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the current animation tick count.
	// Returns:
	//     An integer value representing the current tick count.
	//-----------------------------------------------------------------------
	virtual int GetAnimationTickCount();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the image index, in the image list,
	//     for the 'iIndex' item of the currently selected folder.
	// Parameters:
	//     iIndex - Index of the item in the currently selected folder.
	//     iImage - Index of the image, in the image list, to use for the specified item.
	//-----------------------------------------------------------------------
	virtual void SetItemImage(const int iIndex,const int iImage);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the item data (lParam) for the specified
	//     item in the currently selected folder.
	// Parameters:
	//     iIndex - Index of the item to set item data for.
	//     dwData - Item data (lParam) to set.
	//-----------------------------------------------------------------------
	virtual void SetItemData(const int iIndex,const DWORD dwData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the index of the image associated
	//     with the specified item in the currently selected folder.
	// Parameters:
	//     iIndex - Index of the item to retrieve the image index for.
	// Returns:
	//     An integer value that represents the index of the desired image.
	//-----------------------------------------------------------------------
	virtual int  GetItemImage(const int iIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the item data (lParam) for the specified
	//     item in the currently selected folder.
	// Parameters:
	//     iIndex - Index of the item to retrieve item data for.
	// Returns:
	//     A DWORD value.
	//-----------------------------------------------------------------------
	virtual DWORD GetItemData(const int iIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove all items from the folder specified
	//     by 'iFolder'.
	// Parameters:
	//     iFolder - Index of the folder to insert the item into.
	//     bNofify - true to send an XTWM_OUTBAR_NOTIFY message.
	//-----------------------------------------------------------------------
	virtual void RemoveAllItems(int iFolder,bool bNofify=false);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove the specified item from the currently
	//     selected folder.
	// Parameters:
	//     iIndex - Index of the item to remove.
	//-----------------------------------------------------------------------
	virtual void RemoveItem(const int iIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the text for the specified item in the
	//     currently selected folder.
	// Parameters:
	//     iIndex       - Index of the item to set the text for.
	//     lpszItemName - Points to a NULL terminated string.
	//-----------------------------------------------------------------------
	virtual void SetItemText(const int iIndex,LPCTSTR lpszItemName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to begin local editing of the specified item
	//     in the currently selected folder.
	// Parameters:
	//     iIndex - Index of the item to begin editing for.
	//-----------------------------------------------------------------------
	virtual void StartItemEdit(const int iIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the text label for the specified folder.
	// Parameters:
	//     iIndex         - Index of the folder to set the text label for.
	//     lpszFolderName - Points to a NULL terminated string.
	//-----------------------------------------------------------------------
	virtual void SetFolderText(const int iIndex,LPCTSTR lpszFolderName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to begin editing of the specified folder
	//     item's label.
	// Parameters:
	//     iIndex - Index of the folder to begin editing.
	//-----------------------------------------------------------------------
	virtual void StartGroupEdit(const int iIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get a pointer to the image list for the
	//     specified folder.
	// Parameters:
	//     iIndex - Index of the folder to retrieve the image list for.
	//     bSmall - TRUE to return the small image list. FALSE to return the large image
	//              list.
	// Returns:
	//     A CImageList pointer representing the image list for the folder specified
	//     by 'iIndex'.
	//-----------------------------------------------------------------------
	virtual CImageList* GetFolderImageList(const int iIndex,const BOOL bSmall) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the global image list for the OutlookBar
	//     control.
	// Parameters:
	//     dwImageList - If OBS_XT_SMALLICON, the small image list is returned; if OBS_XT_LARGEICON,
	//                   the large image list is returned.
	// Returns:
	//     A CImageList pointer representing the global image list for the OutlookBar
	//     control.
	//-----------------------------------------------------------------------
	virtual CImageList* GetImageList(DWORD dwImageList);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the image list for the specified folder.
	// Parameters:
	//     iFolder     - Index of the folder to set the image list for.
	//     pImageList  - Points to the new image list.
	//     dwImageList - If OBS_XT_SMALLICON, the small image list is set, if OBS_XT_LARGEICON,
	//                   the large image list is set.
	// Returns:
	//     A pointer to the previously set image list, or NULL if no previous
	//     image list exists.
	//-----------------------------------------------------------------------
	virtual CImageList* SetFolderImageList(const int iFolder,CImageList* pImageList,DWORD dwImageList);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the main image list. You can link different
	//     image lists to the folders using the SetFolderImageList function. If a
	//     folder has been linked to an image list with the SetFolderImageList function,
	//     it will own the linked image list. Otherwise, it will use the image list
	//     set with this function.
	// Parameters:
	//     pImageList  - Points to the new image list.
	//     dwImageList - If OBS_XT_SMALLICON, the small image list is set; if OBS_XT_LARGEICON,
	//     the large image list is set.
	// Returns:
	//     A pointer to the previously set image list, or NULL if no previous image
	//     list exists.
	//-----------------------------------------------------------------------
	virtual CImageList* SetImageList(CImageList* pImageList,DWORD dwImageList);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove the specified folder and its items.
	// Parameters:
	//     iIndex - Index of the folder to remove.
	//-----------------------------------------------------------------------
	virtual void RemoveFolder(const int iIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will get the index of the currently selected folder.
	// Returns:
	//     An integer value representing the index of the currently selected folder.
	//-----------------------------------------------------------------------
	virtual int GetSelFolder() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will get the total number of folders found in
	//     the Outlook bar.
	// Returns:
	//     An integer value representing the number of folders in the Outlook bar.
	//-----------------------------------------------------------------------
	virtual int GetFolderCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the selected folder for the Outlook bar.
	// Parameters:
	//     iIndex - Index of the new selected folder.
	//-----------------------------------------------------------------------
	virtual void SetSelFolder(const int iIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the number of items found in the currently
	//     selected folder.
	// Returns:
	//     An integer value representing the number of items in the current folder.
	//-----------------------------------------------------------------------
	virtual int GetItemCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to insert an item into the specified folder.
	// Parameters:
	//     iFolder      - Index of the folder to insert the item into.
	//     iIndex       - Index or position of the item to insert into the folder.
	//     lpszItemName - A NULL terminated string that represents the item label. This value
	//                    cannot be set to NULL.
	//     iImage       - Index into the folder's image list.
	//     dwData       - User defined item data that you can assign to the item. Use GetItemData
	//                    and SetItemData to access and change this data.
	// Returns:
	//     The index of the newly inserted item.
	//-----------------------------------------------------------------------
	virtual int InsertItem(const int iFolder,const int iIndex,LPCTSTR lpszItemName,const int iImage = -1,const DWORD dwData = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add a folder to the Outlook bar control.
	// Parameters:
	//     lpszFolderName - A NULL terminated string that represents the folder's label.
	//     dwData         - User defined item data for the folder.
	// Returns:
	//     The index of the newly inserted folder.
	//-----------------------------------------------------------------------
	virtual int AddFolder(LPCTSTR lpszFolderName,const DWORD dwData);

	// --------------------------------------------------------------------------------------------
	// Summary:
	//     Call this member function to modify an Outlook bar style.
	// Parameters:
	//     dwRemove -  Specifies OBS_XT_ styles to be removed during
	//                 style modification.
	//     dwAdd -     Specifies OBS_XT_ styles to be added during style
	//                 modification.
	//     bRedraw -   true to redraw the Outlook bar.
	// Remarks:
	//     Call this member function to modify an Outlook bar style. Styles
	//     to be added or removed can be combined by using the bitwise OR (|)
	//     operator.
	//
	//     The desired styles for the Outlook bar can be one or more of the
	//     following:<p/>
	//
	//     * <b>OBS_XT_SMALLICON</b> Sets small icon mode.
	//     * <b>OBS_XT_LARGEICON</b> Sets large icon mode.
	//     * <b>OBS_XT_EDITGROUPS</b> Enables folder local editing
	//           (renaming).
	//     * <b>OBS_XT_EDITITEMS</b> Enables item local editing
	//           (renaming).
	//     * <b>OBS_XT_REMOVEGROUPS</b> Enables the "Remove" command for
	//           folders in context menu.
	//     * <b>OBS_XT_REMOVEITEMS</b> Enables the "Remove" command for
	//           items in context menu.
	//     * <b>OBS_XT_ADDGROUPS</b> Enables folder insertion.
	//     * <b>OBS_XT_DRAGITEMS</b> Enables item dragging to rearrange
	//           position.
	//     * <b>OBS_XT_ANIMATION</b> Enables animation while changing
	//           folder selection.
	//     * <b>OBS_XT_SELHIGHLIGHT</b> Enables dimmed highlight of last
	//           pressed item.
	//     * <b>OBS_XT_DEFAULT</b> Same as (OBS_XT_DRAGITEMS | OBS_XT_EDITGROUPS
	//           | OBS_XT_EDITITEMS | OBS_XT_REMOVEGROUPS | OBS_XT_REMOVEITEMS
	//           | OBS_XT_ADDGROUPS).
	// See Also:
	//     GetFlag, Create, CreateEx
	// --------------------------------------------------------------------------------------------
	virtual void ModifyFlag(const DWORD& dwRemove,const DWORD& dwAdd,const bool bRedraw = false);

	// ----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the current style set for the
	//     Outlook bar.
	// Returns:
	//     A DWORD value representing the current style of the Outlook
	//     bar.
	// Remarks:
	//     See ModifyFlag for a list of available styles.
	// See Also:
	//     ModifyFlag, Create, CreateEx
	// ----------------------------------------------------------------------
	virtual DWORD GetFlag() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the size of the icons displayed in the
	//     Outlook bar control for the specified folder.
	// Parameters:
	//     bSet    - TRUE to display small icons, or FALSE to display large icons.
	//     iFolder - Index of the folder to set the icon size for. If -1, all folder icons
	//               are set.
	//-----------------------------------------------------------------------
	virtual void SetSmallIconView(const BOOL bSet,const int iFolder=-1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the current state of the icon display
	//     for the Outlook bar control.
	// Parameters:
	//     iFolder - Index of the folder to check. If -1, the currently selected folder
	//               is checked.
	// Returns:
	//     TRUE if small icons are displayed, and FALSE if large icons are displayed.
	//-----------------------------------------------------------------------
	virtual BOOL IsSmallIconView(const int iFolder=-1) const;

	// -----------------------------------------------------------------------------------------------
	// Summary:
	//     Call this member function to create the Outlook bar control.
	// Parameters:
	//     dwStyle -     Style for the Outlook bar. It usually includes the WS_CHILD|WS_VISIBLE
	//                   flags.
	//     rect -        Size of the Outlook bar.
	//     pParentWnd -  Parent of the control.
	//     nID -         Identifier of the Outlook bar control.
	//     dwFlag -      Specifies the style flags for the control. See ModifyFlag
	//                   for a list of available styles.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	// See Also:
	//     CreateEx, ModifyFlag
	// -----------------------------------------------------------------------------------------------
	virtual BOOL Create(DWORD dwStyle,const RECT& rect,CWnd* pParentWnd,UINT nID,const DWORD dwFlag = OBS_XT_DEFAULT);

	// --------------------------------------------------------------------------------------------------
	// Summary:
	//     Call this member function to create the Outlook bar control.
	// Parameters:
	//     dwExStyle -   Extended style for the Outlook bar such as WS_EX_STATICEDGE.
	//                   It can be NULL.
	//     dwStyle -     Style for the Outlook bar. It usually includes the WS_CHILD|WS_VISIBLE
	//                   flags.
	//     rect -        Size of the Outlook bar.
	//     pParentWnd -  Parent of the control.
	//     nID -         Identifier of the Outlook bar control.
	//     dwFlag -      Specifies the style flags for the control. See ModifyFlag
	//                   for a list of available styles.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	// See Also:
	//     Create, ModifyFlag
	// --------------------------------------------------------------------------------------------------
	virtual BOOL CreateEx(DWORD dwExStyle, DWORD dwStyle,const RECT& rect,CWnd* pParentWnd,UINT nID,const DWORD dwFlag = OBS_XT_DEFAULT);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the font used by the Outlook bar control.
	// Parameters:
	//     pFont - Points to the font to be used by the Outlook bar.
	//-----------------------------------------------------------------------
	virtual void SetFontX(CFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the font used by the Outlook
	//     bar control.
	// Returns:
	//     A pointer to a CFont object representing the font used by the Outlook bar
	//     control.
	//-----------------------------------------------------------------------
	virtual CFont* GetFontX();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return a pointer to the CXTOutBarFolder data
	//     that is associated with the specified folder.
	// Parameters:
	//     iFolder - Index of the folder to retrieve.
	// Returns:
	//     A pointer to a CXTOutBarFolder object.
	//-----------------------------------------------------------------------
	virtual CXTOutBarFolder* GetBarFolder(const int iFolder);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return a pointer to the CXTOutBarItem data
	//     that is associated with the specified folder and item.
	// Parameters:
	//     iFolder - Index of the folder to retrieve.
	//     iIndex  - Index of the item to retrieve.
	// Returns:
	//     A pointer to a CXTOutBarItem object.
	//-----------------------------------------------------------------------
	virtual CXTOutBarItem* GetBarFolderItem(const int iFolder,const int iIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the background color for the Outlook
	//     bar control.
	// Parameters:
	//     clrBack - An RGB value that represents the background color.
	//-----------------------------------------------------------------------
	virtual void SetBackColor(COLORREF clrBack);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the text color for items in the Outlook
	//     bar control.
	// Parameters:
	//     clrText - An RGB value that represents the text item color.
	//-----------------------------------------------------------------------
	virtual void SetTextColor(COLORREF clrText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the size of the label for the
	//     specified item.
	// Parameters:
	//     iFolder - Index of the folder where the item is located.
	//     iIndex  - Index of the item.
	//     rect    - Address of a CRect object that will receive the label size.
	//-----------------------------------------------------------------------
	virtual void GetLabelRect(const int iFolder,const int iIndex,CRect& rect);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the size of the icon for the
	//     specified item.
	// Parameters:
	//     iFolder     - Index of the folder where the item is located.
	//     iIndex      - Index of the item.
	//     rect        - Address of a CRect object that will receive the icon
	//                   size.
	//     pInsideRect - Pointer to a CRect object that represents the size
	//                   of the client area for the Outlook bar. If NULL
	//                   GetInsideRect will be used to determine the client
	//                   area size.
	//-----------------------------------------------------------------------
	virtual void GetIconRect(const int iFolder,const int iIndex,CRect& rect, CRect* pInsideRect = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the size of the client area for
	//     the Outlook bar. This is the inside area that contains the folders.
	// Parameters:
	//     rect - Address of a CRect object that will receive the size.
	//-----------------------------------------------------------------------
	virtual void GetInsideRect(CRect& rect) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the size of the specified item.
	//     The size includes the area occupied by the item's label and icon.
	// Parameters:
	//     iFolder     - Index of the folder where the item is located.
	//     iIndex      - Index of the item.
	//     rect        - Address of a CRect object that will receive the icon
	//                   size.
	//     pInsideRect - Pointer to a CRect object that represents the size
	//                   of the client area for the Outlook bar. If NULL
	//                   GetInsideRect will be used to determine the client
	//                   area size.
	//-----------------------------------------------------------------------
	virtual void GetItemRect(const int iFolder,const int iIndex,CRect& rect, CRect* pInsideRect = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the size of the specified folder.
	// Parameters:
	//     iIndex - Index of the folder item.
	//     rect   - Address of a CRect object that will receive the folder size.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL GetFolderRect(const int iIndex,CRect& rect) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the selected folder and item
	//     for the Outlook bar.
	// Parameters:
	//     iFolder        - Index of the folder that contains the new selection.
	//     iItem          - Index of the new item to select.
	//     bEnsureVisible - TRUE to ensure the item is visible when selected.
	//-----------------------------------------------------------------------
	virtual void SetSelItem(int iFolder, int iItem, BOOL bEnsureVisible=TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will return the index of the currently selected
	//     item for the currently selected folder.
	// Returns:
	//     An integer value representing the index of the selected item.
	//-----------------------------------------------------------------------
	int GetCurSel();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the currently selected item for the currently
	//     selected folder.
	// Parameters:
	//     iItem    - Index of the item to select.
	//     bPressed - true if the item is to be pressed when selected.
	//-----------------------------------------------------------------------
	void SetCurSel(int iItem,const BOOL bPressed=false);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable a folder item.
	// Parameters:
	//     iFolder - Index of the folder that owns the item.
	//     iItem   - Index of the item to enable or disable.
	//     bEnable - true to enable item, false to disable.
	//-----------------------------------------------------------------------
	void EnableItem(int iFolder,int iItem,bool bEnable);


protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTOutBarCtrl to draw the up or down scroll button.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     rect   - Size of area to render.
	//     uType  - Specifies the type of frame control to draw. See the
	//              uType parameter in DrawFrameControl in the Platform SDK for
	//              a list of this parameter's possible values.
	//     uState - Specifies the initial state of the frame control.
	//              Can be one or more of the values described for the uState parameter
	//              in DrawFrameControl in the Platform SDK. Use the nState value
	//              DFCS_ADJUSTRECT to adjust the bounding rectangle to exclude
	//              the surrounding edge of the push button.
	//-----------------------------------------------------------------------
	virtual void DrawScrollButton(CDC* pDC, CRect rect, UINT uType, UINT uState);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTOutBarCtrl to draw a folder item.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//     iFolder    - Index of the parent folder.
	//     rc         - Size of the area to render.
	//     iIndex     - Index of the item to render.
	//     bOnlyImage - TRUE if only the image is to be drawn.
	//-----------------------------------------------------------------------
	virtual void DrawItem(CDC* pDC, const int iFolder, CRect rc, const int iIndex, const BOOL bOnlyImage = false);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTOutBarCtrl to draw the drag arrow
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     iFrom - Index of the item before the arrow.
	//     iTo   - Index of the item after the arrow.
	//-----------------------------------------------------------------------
	virtual void DrawDragArrow(CDC* pDC, const int iFrom, const int iTo);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTOutBarCtrl to render an item animated.
	// Parameters:
	//     iOffsetX - X distance to offset icon animation.
	//     iOffsetY - Y distance to offset icon animation.
	//     iIndex   - Index of item to animate.
	//-----------------------------------------------------------------------
	virtual void DrawAnimItem(const int iOffsetX, const int iOffsetY, const int iIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTOutBarCtrl to render a folder.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     iIndex   - Index of the folder to render.
	//     rect     - Size of the area to render.
	//     eHilight - Current mouse over state.
	// See Also: XTMouseState
	//-----------------------------------------------------------------------
	virtual void DrawFolder(CDC* pDC, const int iIndex, CRect rect, const XTMouseState eHilight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTOutBarCtrl to render an item's icon.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     iIcon    - Index of the icon to render.
	//     iFolder  - Index of the parent folder.
	//     bHilight - true if the icon is highlighted.
	//-----------------------------------------------------------------------
	virtual void DrawIcon(CDC* pDC, int iIcon, int iFolder, bool bHilight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTOutBarCtrl to render all items for a folder.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     iFolder - Index of the parent folder.
	//     rc      - Size of area to render.
	//-----------------------------------------------------------------------
	virtual void PaintItems(CDC* pDC, const int iFolder, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTOutBarCtrl to get the area that is visible to the user.
	// Parameters:
	//     iFolder - [in] Index of the folder to get the range for.
	//     iFirst  - [out] Index of the first item that is visible.
	//     iLast   - [out] Index of the last item that is visible.
	//-----------------------------------------------------------------------
	virtual void GetVisibleRange(const int iFolder, int& iFirst, int& iLast);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTOutBarCtrl to get the size of the
	//     item dragged.
	// Parameters:
	//     iIndex - Index of the item
	//     rect   - Size of the item dragged.
	// Returns:
	//     Style of last drag item that was rendered.
	//-----------------------------------------------------------------------
	virtual int GetDragItemRect(const int iIndex, CRect& rect);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTOutBarCtrl to get the size of a folder item.
	// Parameters:
	//     iFolder - Index of parent folder.
	//     iIndex  - Index of item.
	//     iType   - The pare of the item to get the size for.
	// Returns:
	//     A CSize object representing the item size.
	//-----------------------------------------------------------------------
	virtual CSize GetItemSize(const int iFolder, const int iIndex, const RectItem iType);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTOutBarCtrl to render folder animation when a new folder is selected.
	// Parameters:
	//     iFrom - Index of currently selected folder.
	//     iTo   - Index of the new folder to select.
	//-----------------------------------------------------------------------
	virtual void AnimateFolderScroll(const int iFrom, const int iTo);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTOutBarCtrl to draw an item highlighted.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     iIndex   - Index of the item to render.
	//     bPressed - TRUE if the item is selected.
	//-----------------------------------------------------------------------
	virtual void HighlightItem(CDC* pDC, const int iIndex, const BOOL bPressed = false);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTOutBarCtrl to render a folder highlighted.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     iIndex - Index of the folder to highlight.
	//-----------------------------------------------------------------------
	virtual void HighlightFolder(CDC* pDC, const int iIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called to determine the item from a given xy coordinate.
	// Parameters:
	//     point  - [in] xy coordinate to test.
	//     iIndex - [out] Index of the item at the given xy coordinates.
	// Returns:
	//     Item type see HitTestCode.
	//-----------------------------------------------------------------------
	virtual HitTestCode HitTestEx(const CPoint& point, int& iIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called to determine if the specified item is valid.
	// Parameters:
	//     iIndex - Index of the item to test.
	// Returns:
	//     TRUE if the item is valid, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL IsValidItem(const int iIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTOutBarCtrl to end an item or folder label edit.
	// Parameters:
	//     pEdit     - Pointer to the edit control.
	//     bIsFolder - true if the item edited is a folder.
	//-----------------------------------------------------------------------
	virtual void EndLabelEdit(CXTOutBarEditItem* pEdit, bool bIsFolder);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTOutBarCtrl when an item or folders label changes.
	// Parameters:
	//     pObi - Pointer to a XT_OUTBAR_INFO structure.
	//-----------------------------------------------------------------------
	virtual void OnLabelChanged(const XT_OUTBAR_INFO* pObi);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTOutBarCtrl to draw an item icon.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//     pt         - xy location of the icon to render.
	//     pBarItem   - Pointer to a CXTOutBarItem object.
	//     pImageList - Pointer to the CXTOutBarCtrl image list.
	//-----------------------------------------------------------------------
	virtual void DrawItemIcon( CDC* pDC, CPoint pt, CXTOutBarItem* pBarItem, CImageList* pImageList);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTOutBarCtrl to draw an items label.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rc       - Size of text to render.
	//     pBarItem - Pointer to a CXTOutBarItem object.
	//     nFormat  - Draw text flags, see CXTOutBarTheme::DrawItemText.
	//-----------------------------------------------------------------------
	virtual void DrawItemText( CDC* pDC, CRect rc, CXTOutBarItem* pBarItem, UINT nFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTOutBarCtrl to render a folder that contains
	//     a CWnd object when a new folder is selected.
	// Parameters:
	//     pWnd    - Pointer to the child of the folder.
	//     pDC     - Pointer to a valid device context.
	//     nOffset - Amount in pixels to offset from the top.
	//-----------------------------------------------------------------------
	void DrawPreviewBitmap(CWnd* pWnd, CDC* pDC, int nOffset);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CXTOutBarCtrl)
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLargeIcon();
	afx_msg void OnUpdateLargeIcon(CCmdUI* pCmdUI);
	afx_msg void OnSmallIcon();
	afx_msg void OnUpdateSmallIcon(CCmdUI* pCmdUI);
	afx_msg void OnRemoveItem();
	afx_msg void OnUpdateRemoveItem(CCmdUI* pCmdUI);
	afx_msg void OnRenameItem();
	afx_msg void OnUpdateRenameItem(CCmdUI* pCmdUI);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam = NULL);


protected:

	int m_nFolderHeight;           // Size in pixels of the folder caption bar.
	int m_nSelFolder;              // Index of the currently selected folder.
	int m_nFolderHilighted;        // Index of the currently highlighted folder.
	int m_nItemHilighted;          // Index of the currently highlighted item.
	int m_nLastFolderSelected;     // Index of the last folder that was selected.
	int m_nLastItemSelected;       // Index of the last item that was selected.
	int m_nFirstItem;              // Index of the first visible item.
	int m_nIconSpacingLarge;       // Amount in pixels of the gap between large icons.
	int m_nIconSpacingSmall;       // Amount in pixels of the gap between small icons.
	int m_nHitInternal1;           // Used internally to test folder and item indexes.
	int m_nHitInternal2;           // Used internally to test folder and item indexes.
	int m_nLastDragItemDraw;       // Insertion index for drag and drop operations.
	int m_nLastDragItemDrawType;   // Style of last drag item that was rendered.
	int m_nSelAnimCount;           // Selected item animation sequence count
	int m_nSelAnimTiming;          // Specifies the time in milliseconds that the selected item will animate.
	int m_nAnimationTickCount;     // Specifies the time in milliseconds between animation.
	CPen m_penBlack;               // Pen used to render drag arrow.
	BOOL m_bUpArrow;               // TRUE when the up scroll arrow is displayed.
	BOOL m_bDownArrow;             // TRUE when the down scroll arrow is displayed.
	BOOL m_bUpPressed;             // TRUE when the up scroll arrow is selected.
	BOOL m_bDownPressed;           // TRUE when the down scroll arrow is selected.
	BOOL m_bIconPressed;           // TRUE when the selected item's icon should be rendered selected.
	BOOL m_bLooping;               // TRUE when scrolling to continue iteration.
	BOOL m_bPressedHighlight;      // TRUE when the selected item should be rendered.
	CFont m_font;                  // Font used by the outlook bar.
	DWORD m_dwFlags;               // Style for the Outlook bar.
	CRect m_rcUpArrow;             // Size and location of the up scroll arrow.
	CRect m_rcDownArrow;           // Size and location of the down scroll arrow.
	CSize m_sizeOffset;            // Amount in pixels of the gap between an item icon and text.
	CSize m_sizeMargin;            // Amount in pixels of the gap between the border and item text and icon.
	CPtrArray m_arFolder;          // Array of folders added to the outlook bar.
	CImageList* m_pLargeImageList; // Points to the large image list for the outlook bar.
	CImageList* m_pSmallImageList; // Points to the small image list for the outlook bar.


	friend class CXTOutBarEditItem;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTOutBarCtrl::GetCurSel() {
	return m_nItemHilighted;
}
AFX_INLINE void CXTOutBarCtrl::SetCurSel(int iItem, const BOOL bPressed/*=false*/) {
	HighlightItem(NULL, iItem, bPressed);
}
AFX_INLINE BOOL CXTOutBarCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTOutBarCtrl::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam) {
	return CWnd::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam);
}

// ----------------------------------------------------------------------
// Summary:
//     CXTOutBarItem is a stand alone helper class. It is used by the
//     CXTOutBarCtrl to maintain information about each folder item
//     specified in the Outlook bar control.
// ----------------------------------------------------------------------
class _XT_EXT_CLASS CXTOutBarItem
{
public:

	// --------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTOutBarItem object
	// Parameters:
	//     lpszName -     [in] A NULL terminated string that represents the
	//                    item name.
	// nImageIndex -  [in] An index into the folders image list.
	// dwData -       [in] User item data (lParam).
	// --------------------------------------------------------------------
	CXTOutBarItem(LPCTSTR lpszName,const int nImageIndex,DWORD dwData);

	// ------------------------------------------------------------
	// Summary:
	//     Destroys a CXTOutBarItem object, handles cleanup and
	//     deallocation
	// ------------------------------------------------------------
	virtual ~CXTOutBarItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the zero-based index of the folder
	//     item.
	// Returns:
	//     An integer value that represents the zero-based index of the folder item.
	//-----------------------------------------------------------------------
	int GetIndex();

	// ----------------------------------------------------------------
	// Summary:
	//     Call this member function to set the zero-based index of the
	//     folder item.
	// Parameters:
	//     iIndex -  [in] New index of the folder item.
	// ----------------------------------------------------------------
	void SetIndex(int iIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the user specified item data (lParam)
	//     for the folder item.
	// Returns:
	//     A DWORD value that represents the user data.
	//-----------------------------------------------------------------------
	DWORD GetData();

	// --------------------------------------------------------------------------
	// Summary:
	//     Call this member function to set a user data (lParam) value for
	//     the folder item.
	// Parameters:
	//     dwData -  [in] Specifies the user data (lparam) value to be associated
	//               with the folder item.
	// --------------------------------------------------------------------------
	void SetData(DWORD dwData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the label of the folder item.
	// Returns:
	//     A CString object that contains the folder item label.
	//-----------------------------------------------------------------------
	CString GetName();

	// ---------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the label of the folder item.
	// Parameters:
	//     strName -  [in] A NULL terminated string that represents the item
	//                label.
	// ---------------------------------------------------------------------
	void SetName(CString strName);

	// --------------------------------------------------------------
	// Summary:
	//     Call this member function to set the items selected state.
	// Parameters:
	//     bSelected -  [in] true to set the item state to selected.
	// --------------------------------------------------------------
	void SelectItem(bool bSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to see if the item is selected.
	// Returns:
	//     true if the item is selected, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsSelected();

	// -----------------------------------------------------------------
	// Summary:
	//     Call this member function to toggle the enabled state for the
	//     \outlook bar item.
	// Parameters:
	//     bEnable -  [in] true to enable, false to disable.
	// -----------------------------------------------------------------
	void EnableItem(bool bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the item is enabled.
	// Returns:
	//     true if the item is enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsEnabled();

protected:

	int     m_nIndex;       // Item index.
	bool    m_bEnabled;     // true if the icon is enabled.
	bool    m_bSelected;    // true if selected.
	DWORD   m_dwData;       // User data.
	CString m_strName;      // label of the folder item.

	friend class CXTOutBarCtrl;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTOutBarItem::SelectItem(bool bSelected) {
	m_bSelected = bSelected;
}
AFX_INLINE bool CXTOutBarItem::IsSelected() {
	return m_bSelected;
}
AFX_INLINE void CXTOutBarItem::EnableItem(bool bEnable) {
	m_bEnabled = bEnable;
}
AFX_INLINE bool CXTOutBarItem::IsEnabled() {
	return m_bEnabled;
}

// --------------------------------------------------------------------
// Summary:
//     CXTOutBarFolder is a stand alone helper class. It is used by
//     the CXTOutBarCtrl to maintain information about each folder
//     specified in the Outlook bar control.
// --------------------------------------------------------------------
class _XT_EXT_CLASS CXTOutBarFolder
{
public:

	// ----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTOutBarFolder object.
	// Parameters:
	//     lpszName -  [in] A NULL terminated string that represents the item
	//                 name.
	// dwData -    [in] User defined item data (lParam).
	// pControl -  [in] Pointer to the parent CXTOutBarCtrl object.
	// Remarks:
	//     The CXTOutBarFolder object is constructed by the CXTOutBarCtrl to
	//     when a new folder to the control.
	// See Also:
	//     CXTOutBarCtrl::AddFolder, CXTOutBarCtrl::AddFolderBar
	// ----------------------------------------------------------------------
	CXTOutBarFolder(LPCTSTR lpszName,DWORD dwData, CXTOutBarCtrl* pControl);

	// --------------------------------------------------------------
	// Summary:
	//     Destroys a CXTOutBarFolder object, handles cleanup and
	//     deallocation
	// --------------------------------------------------------------
	virtual ~CXTOutBarFolder();

	// ---------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the number of folders found
	//     in the Outlook bar.
	// Returns:
	//     The number of folders in the Outlook bar.
	// ---------------------------------------------------------------------
	int GetItemCount();

	// --------------------------------------------------------------------
	// Summary:
	//     Call this member function to insert an item into the Outlook bar
	//     folder.
	// Parameters:
	//     iIndex -    [in] Index of the item.
	// lpszName -  [in] A NULL terminated string that represents the
	//                 label of the item.
	// nImage -    [in] Index into the folder's image list.
	// dwData -    [in] User item data (lParam).
	// Returns:
	//     An integer value.
	// --------------------------------------------------------------------
	int InsertItem(int iIndex,LPCTSTR lpszName,const int nImage,const DWORD dwData);

	// ---------------------------------------------------------------------
	// Summary:
	//     Retrieves a pointer to a CXTOutBarItem at the given index.
	// Parameters:
	//     iIndex -  [in] Index of the folder item to retrieve.
	// Remarks:
	//     Call this member function to retrieve a pointer to the CXTOutBarItem
	//     at the specified index.
	// Returns:
	//     A pointer to a CXTOutBarItem object if successful, otherwise NULL.
	// ---------------------------------------------------------------------
	CXTOutBarItem* GetItemAt(int iIndex);

	// -----------------------------------------------------------------------
	// Summary:
	//     Call this member function to insert a folder item into the
	//     location specified by 'iIndex'.
	// Parameters:
	//     iIndex -    [in] Zero\-based index of where to insert the new item.
	// pBarItem -  [in] Points to a valid CXTOutBarItem object.
	// -----------------------------------------------------------------------
	void InsertItemAt(int iIndex,CXTOutBarItem* pBarItem);

	// -------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove the specified item from the
	//     folder.
	// Parameters:
	//     iIndex -  Index of the item to remove.
	// Returns:
	//     A pointer to the removed item.
	// -------------------------------------------------------------------
	CXTOutBarItem* RemoveItemAt(int iIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the label of the folder item.
	// Returns:
	//     A CString object.
	//-----------------------------------------------------------------------
	CString GetName();

	// -------------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the label for the folder item.
	// Parameters:
	//     strName -  [in] A NULL terminated string that represents the folder's
	//                new label.
	// -------------------------------------------------------------------------
	void SetName(CString strName);

	// ---------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the user defined item data
	//     (lParam) for the folder.
	// Returns:
	//     A DWORD value that represents the item data if defined, otherwise
	//     NULL.
	// ---------------------------------------------------------------------
	DWORD GetData();

	// --------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the user item data (lParam) for
	//     the folder.
	// Parameters:
	//     dwData -  [in] User item data (lParam).
	// --------------------------------------------------------------------
	void SetData(DWORD dwData);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return a pointer to the large image list
	//     for the folder.
	// Returns:
	//     A pointer to CImageList object if successful, otherwise returns NULL.
	//-----------------------------------------------------------------------
	CImageList* GetLargeImageList();

	// -----------------------------------------------------------------
	// Summary:
	//     Call this member function to set the large image list for the
	//     folder.
	// Parameters:
	//     pLargeList -  [in] Points to a CImageList object.
	// -----------------------------------------------------------------
	void SetLargeImageList(CImageList* pLargeList);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return a pointer to the small image list
	//     for the folder.
	// Returns:
	//     A CImageList object if successful, otherwise returns NULL.
	//-----------------------------------------------------------------------
	CImageList* GetSmallImageList();

	// -----------------------------------------------------------------
	// Summary:
	//     Call this member function to set the small image list for the
	//     folder.
	// Parameters:
	//     pSmallList -  [in] Points to a CImageList object.
	// -----------------------------------------------------------------
	void SetSmallImageList(CImageList* pSmallList);

	// ----------------------------------------------------------------------
	// Summary:
	//     Retrieves a pointer to the child window.
	// Remarks:
	//     Call this member function to retrieve a CWnd pointer to the child
	//     object that is associated with this folder item.
	// Returns:
	//     A CWnd pointer to the child associated with the folder, or NULL if
	//     no objects were found.
	// See Also:
	//     SetChild, CXTOutBarCtrl::AddFolderBar
	// ----------------------------------------------------------------------
	CWnd* GetChild();

	// --------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the CWnd child to be associated
	//     with this folder item.
	// Parameters:
	//     pChild -  [in] Points to a valid CWnd object.
	// --------------------------------------------------------------------
	void SetChild(CWnd* pChild);

	// --------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the currently selected item for
	//     the folder.
	// Parameters:
	//     iItem -  [in] Index of the item to select
	// --------------------------------------------------------------------
	void SetSelItem(int iItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return a pointer to the currently selected
	//     item.
	// Returns:
	//     A pointer to a CXTOutBarItem object if successful, otherwise returns NULL.
	//-----------------------------------------------------------------------
	CXTOutBarItem* GetSelItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the index for the currently selected
	//     item.
	// Returns:
	//     The index of the currently selected item if successful, otherwise returns -1.
	//-----------------------------------------------------------------------
	int GetSelIndex();

	// ----------------------------------------------------------------------
	// Summary:
	//     Retrieves a pointer to the parent CXTOutBarCtrl window.
	// Remarks:
	//     Call this member to retrieve a pointer to the folder's CXTOutBarCtrl
	//     parent window.
	// Returns:
	//     A pointer to a CXTOutBarCtrl object if successful, otherwise NULL.
	// ----------------------------------------------------------------------
	CXTOutBarCtrl* GetControl();

protected:

	BOOL                            m_bSmallIcons;  // TRUE to use small icons.
	CWnd*                           m_pChild;       // Child window pointer.
	DWORD                           m_dwData;       // Application data.
	CString                         m_strName;      // Folder label.
	CImageList*                     m_pLargeList;   // Image list for large icons.
	CImageList*                     m_pSmallList;   // Image list for small icons.
	CList<CXTOutBarItem*, CXTOutBarItem*> m_barItems;     // Folder items.
	CXTOutBarCtrl*                  m_pControl;     // Pointer to the parent CXTOutBarCtrl object.

	friend class CXTOutBarCtrl;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTOutBarCtrl::SetFontX(CFont* pFont) {
	ASSERT_VALID(pFont); LOGFONT lf; pFont->GetLogFont(&lf); m_font.DeleteObject(); m_font.CreateFontIndirect(&lf);
}
AFX_INLINE CFont* CXTOutBarCtrl::GetFontX() {
	return &m_font;
}
AFX_INLINE void CXTOutBarCtrl::SetAnimationTickCount(const long lValue) {
	m_nAnimationTickCount = lValue;
}
AFX_INLINE int CXTOutBarCtrl::GetAnimationTickCount() {
	return m_nAnimationTickCount;
}
AFX_INLINE CXTOutBarFolder* CXTOutBarCtrl::GetBarFolder(const int iFolder) {
	return (CXTOutBarFolder*)m_arFolder.GetAt(iFolder);
}
AFX_INLINE CXTOutBarItem* CXTOutBarCtrl::GetBarFolderItem(const int iFolder, const int iIndex) {
	return GetBarFolder(iFolder)->GetItemAt(iIndex);
}


//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTOutBarItem::GetIndex() {
	return m_nIndex;
}
AFX_INLINE void CXTOutBarItem::SetIndex(int iIndex) {
	m_nIndex = iIndex;
}
AFX_INLINE DWORD CXTOutBarItem::GetData() {
	return m_dwData;
}
AFX_INLINE void CXTOutBarItem::SetData(DWORD dwData) {
	m_dwData = dwData;
}
AFX_INLINE CString CXTOutBarItem::GetName() {
	return m_strName;
}
AFX_INLINE void CXTOutBarItem::SetName(CString strName) {
	m_strName = strName;
}

//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTOutBarFolder::GetItemCount() {
	return (int)m_barItems.GetCount();
}
AFX_INLINE CString CXTOutBarFolder::GetName() {
	return m_strName;
}
AFX_INLINE void CXTOutBarFolder::SetName(CString strName) {
	m_strName = strName;
}
AFX_INLINE DWORD CXTOutBarFolder::GetData() {
	return m_dwData;
}
AFX_INLINE void CXTOutBarFolder::SetData(DWORD dwData) {
	m_dwData = dwData;
}
AFX_INLINE CImageList* CXTOutBarFolder::GetLargeImageList() {
	return m_pLargeList;
}
AFX_INLINE void CXTOutBarFolder::SetLargeImageList(CImageList* pLargeList) {
	m_pLargeList = pLargeList;
}
AFX_INLINE CImageList* CXTOutBarFolder::GetSmallImageList() {
	return m_pSmallList;
}
AFX_INLINE void CXTOutBarFolder::SetSmallImageList(CImageList* pSmallList) {
	m_pSmallList = pSmallList;
}
AFX_INLINE CWnd* CXTOutBarFolder::GetChild() {
	return m_pChild;
}
AFX_INLINE void CXTOutBarFolder::SetChild(CWnd* pChild) {
	m_pChild = pChild;
}

AFX_INLINE CXTOutBarCtrl* CXTOutBarFolder::GetControl() {
	return m_pControl;
}
//////////////////////////////////////////////////////////////////////

//===========================================================================
// Summary:
//     CXTOutBarEditItem is a helper class derived from CXTEdit. It is used by
//     CXTOutBarCtrl, an Outlook like control, to create an in-place edit box
//     that is used to modify a folder or an item within the control.
//===========================================================================
class _XT_EXT_CLASS CXTOutBarEditItem : public CXTEdit
{
	DECLARE_DYNAMIC(CXTOutBarEditItem)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTOutBarEditItem object
	//-----------------------------------------------------------------------
	CXTOutBarEditItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTOutBarEditItem object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTOutBarEditItem();

protected:

	int             m_iIndex;       // Index of the folder or item.
	bool            m_bEscapeKey;   // true if the escape key was pressed.
	bool            m_bSmallIcons;  // true if the folder is using small icons.
	bool            m_bIsFolder;    // true if the edit box is for a folder.
	CRect           m_rcOriginal;   // Original size of the edit box when it was first created.
	CString         m_strText;      // Original string of the edit box when it was first created, and the new text on edit completion.
	CXTOutBarCtrl*  m_pParentWnd;   // Parent Outlook bar control.

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the index of the folder or item
	//     currently being edited.
	// Returns:
	//     An integer value that represents the index of the folder or item.
	//-----------------------------------------------------------------------
	int GetIndex() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the text associated with this edit
	//     control.
	// Returns:
	//     A CString object that represents the text originally
	//     set for the edit control. If called after the edit is destroyed, it
	//     returns the modified text value.
	//-----------------------------------------------------------------------
	CString GetText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     The Outlook bar control calls this member to create an in-place edit
	//     control.
	// Parameters:
	//     lpszText    - NULL terminated string to be displayed in the edit control.
	//     dwStyle     - Window style for the edit control.
	//     rect        - Size of the edit control.
	//     pParentWnd  - Owner window.
	//     nID         - Control ID.
	//     nIndex      - Folder or item index.
	//     bIsFolder   - true if the edit is for a folder item.
	//     bSmallIcons - true if the parent folder is displaying small icons.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Create(LPCTSTR lpszText,DWORD dwStyle,const RECT& rect,CWnd* pParentWnd,UINT nID,UINT nIndex,bool bIsFolder,bool bSmallIcons);

	//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_VIRTUAL(CXTOutBarEditItem)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
	//}}AFX_CODEJOCK_PRIVATE

protected:

	//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_MSG(CXTOutBarEditItem)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTOutBarEditItem::GetIndex() const {
	return m_iIndex;
}
AFX_INLINE CString CXTOutBarEditItem::GetText() const {
	return m_strText;
}
AFX_INLINE BOOL CXTOutBarEditItem::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTOutBarEditItem::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
	return CEdit::Create(dwStyle, rect, pParentWnd, nID);
}

//===========================================================================
// Summary:
//     CXTToolBox is a CXTOutBarCtrl derived class. It is used to create a
//     toolbox control similar to the toolbox control seen in VisualStudio.NET.
//===========================================================================
class _XT_EXT_CLASS CXTToolBox : public CXTOutBarCtrl
{
	DECLARE_DYNCREATE(CXTToolBox)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTToolBox object
	//-----------------------------------------------------------------------
	CXTToolBox();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTToolBox object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTToolBox();

protected:

	int  m_iFirst;      // Index of the first visible item.
	int  m_iLast;       // Index of the last visible item.
	bool m_bAnimating;  // true if the folder selection is changing.

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to create the toolbox control.
	// Parameters:
	//     dwExStyle  - Extended style for the toolbox, such as WS_EX_STATICEDGE. It can be
	//                  NULL.
	//     dwStyle    - Style for the toolbox. It usually includes the WS_CHILD|WS_VISIBLE
	//                  flags.
	//     rect       - Size of the toolbox.
	//     pParentWnd - Parent of the control.
	//     nID        - Identifier of the toolbox control.
	//     dwFlag     - Specifies the style flags for the control. See CXTOutBarCtrl::ModifyFlag
	//                  for a list of available styles.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL CreateEx(DWORD dwExStyle,DWORD dwStyle,const RECT& rect,CWnd* pParentWnd,UINT nID,const DWORD dwFlag = OBS_XT_DEFAULT);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the size of the icon for the
	//     specified item.
	// Parameters:
	//     iFolder - Index of the folder where the item is located.
	//     iIndex  - Index of the item.
	//     rect    - Address of a CRect object that will receive the icon size.
	//     pInsideRect - Pointer to a CRect object that represents the size
	//                   of the client area for the Outlook bar. If NULL
	//                   GetInsideRect will be used to determine the client
	//                   area size.
	//-----------------------------------------------------------------------
	virtual void GetIconRect(const int iFolder,const int iIndex,CRect& rect, CRect* pInsideRect = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the size of the specified item.
	//     The size includes the area occupied by the item's label and icon.
	// Parameters:
	//     iFolder - Index of the folder where the item is located.
	//     iIndex  - Index of the item.
	//     rect    - Address of a CRect object that will receive the item size.
	//     pInsideRect - Pointer to a CRect object that represents the size
	//                   of the client area for the Outlook bar. If NULL
	//                   GetInsideRect will be used to determine the client
	//                   area size.
	//-----------------------------------------------------------------------
	virtual void GetItemRect(const int iFolder,const int iIndex,CRect& rect, CRect* pInsideRect = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the selected folder for the toolbox.
	// Parameters:
	//     iIndex - Index of the new selected folder.
	//-----------------------------------------------------------------------
	virtual void SetSelFolder(const int iIndex);

protected:

	//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_VIRTUAL(CXTToolBox)
	//}}AFX_VIRTUAL
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTToolBox to render an item's icon.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     iIcon    - Index of the icon to render.
	//     iFolder  - Index of the parent folder.
	//     bHilight - true if the icon is highlighted.
	//-----------------------------------------------------------------------
	virtual void DrawIcon(CDC* pDC, int iIcon, int iFolder, bool bHilight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTToolBox to draw a folder item.
	// Parameters:
	//     pDC        - Pointer to a valid device context.
	//     iFolder    - Index of the parent folder.
	//     rc         - Size of the area to render.
	//     iIndex     - Index of the item to render.
	//     bOnlyImage - TRUE if only the image is to be drawn.
	//-----------------------------------------------------------------------
	virtual void DrawItem(CDC* pDC, const int iFolder, CRect rc, const int iIndex, const BOOL bOnlyImage);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTToolBox to render a folder.
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     iIndex   - Index of the folder to render.
	//     rect     - Size of the area to render.
	//     eHilight - Current mouse over state.
	// See Also: XTMouseState
	//-----------------------------------------------------------------------
	virtual void DrawFolder(CDC* pDC, const int iIndex, CRect rect, const XTMouseState eHilight);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called by the CXTToolBox to draw the up or down scroll button.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     rect   - Size of area to render.
	//     uType  - Specifies the type of frame control to draw. See the
	//              uType parameter in DrawFrameControl in the Platform SDK for
	//              a list of this parameter's possible values.
	//     uState - Specifies the initial state of the frame control.
	//              Can be one or more of the values described for the uState parameter
	//              in DrawFrameControl in the Platform SDK. Use the nState value
	//              DFCS_ADJUSTRECT to adjust the bounding rectangle to exclude
	//              the surrounding edge of the push button.
	//-----------------------------------------------------------------------
	virtual void DrawScrollButton(CDC* pDC, CRect rect, UINT uType, UINT uState);

	//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_MSG(CXTToolBox)
	afx_msg void OnPaint();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam = NULL);
};

AFX_INLINE BOOL CXTToolBox::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam) {
	return CWnd::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam);
}

#endif // #if !defined(__XTOUTBARCTRL_H__)
