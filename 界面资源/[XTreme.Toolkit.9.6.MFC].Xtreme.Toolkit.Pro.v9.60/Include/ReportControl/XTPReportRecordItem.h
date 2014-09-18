// XTPReportRecordItem.h: interface for the CXTPReportRecordItem class.
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
#if !defined(__XTPREPORTRECORDITEM_H__)
#define __XTPREPORTRECORDITEM_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTPReportHyperlink.h"


class CXTPReportControl;
class CXTPReportRow;
class CXTPReportRecordItem;
class CXTPReportHyperlink;
class CXTPReportColumn;
class CXTPReportPaintManager;
class CXTPReportRecord;
class CXTPReportInplaceButton;

//===========================================================================
// Summary:
//     Basic set of parameters transferred to item handlers.
//     This structure groups together essential parameters used in
//     items processing activities. It is parent for all other structures:
//     XTP_REPORTRECORDITEM_DRAWARGS, XTP_REPORTRECORDITEM_CLICKARGS
// Example:
// <code>
// XTP_REPORTRECORDITEM_ARGS itemArgs(pControl, pRow, pColumn);
// pItem->OnBeginEdit(&itemArgs);
// </code>
// See Also: CXTPReportRecordItem::OnBeginEdit
//===========================================================================
struct _XTP_EXT_CLASS XTP_REPORTRECORDITEM_ARGS
{
//{{AFX_CODEJOCK_PRIVATE
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a XTP_REPORTRECORDITEM_ARGS object
	// Parameters:
	//     pControl - Parent report control pointer
	//     pRow     - Item's row pointer
	//     pColumn  - Item's column pointer
	//-----------------------------------------------------------------------
	XTP_REPORTRECORDITEM_ARGS();
	XTP_REPORTRECORDITEM_ARGS(CXTPReportControl* pControl, CXTPReportRow* pRow, CXTPReportColumn* pColumn); // <COMBINE XTP_REPORTRECORDITEM_ARGS::XTP_REPORTRECORDITEM_ARGS>

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGrid object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~XTP_REPORTRECORDITEM_ARGS();
//}}AFX_CODEJOCK_PRIVATE

	CXTPReportControl* pControl;    // Pointer to the main Report control.
	CXTPReportRow* pRow;            // Pointer to the associated row.
	CXTPReportColumn* pColumn;      // Report column at click position, if any, NULL otherwise.
	CXTPReportRecordItem* pItem;    // Pointer to the associated item.
	CRect rcItem;                   // Item position in control client coordinates.
};

//===========================================================================
// Summary:
//     A set of parameters transferred to Draw item handler.
//     It inherits essential parameters from XTP_REPORTRECORDITEM_ARGS.
// Remarks:
//     This structure is used in almost all functions concerning drawing
//     activities
// Example:
// <code>
// // fill structure
// XTP_REPORTRECORDITEM_DRAWARGS drawArgs;
// drawArgs.pDC = pDC;
// drawArgs.pControl = m_pControl;
// drawArgs.pRow = pRow;
// // call function
// pItem->Draw(&drawArgs);
// </code>
// See Also: CXTPReportPaintManager
//===========================================================================
struct _XTP_EXT_CLASS XTP_REPORTRECORDITEM_DRAWARGS : public XTP_REPORTRECORDITEM_ARGS
{
	CDC* pDC;                       // Pointer to control drawing context.
	int nTextAlign;                 // Text alignment mode DT_LEFT||DT_RIGHT||DT_CENTER
};

//===========================================================================
// Summary:
//     A set of parameters transferred to mouse click item handlers.
//     It inherits essential parameters from XTP_REPORTRECORDITEM_ARGS.
// Remarks:
//     XTP_REPORTRECORDITEM_CLICKARGS is used in most functions involved
//     in mouse events processing
// See Also: CXTPReportControl, CXTPReportRecordItem::OnClick
//===========================================================================
struct _XTP_EXT_CLASS XTP_REPORTRECORDITEM_CLICKARGS : public XTP_REPORTRECORDITEM_ARGS
{
	CPoint ptClient;                // Coordinates of the mouse click point.
};

//===========================================================================
// Summary:
//     This structure is sent to Main window in a WM_NOTIFY message from Item
//     and provides all parameters that are needed in processing control specific
//     notifications by the main window
// Example:
// <code>
// BEGIN_MESSAGE_MAP(CPropertiesView, CView)
//     ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, ID_REPORT_CONTROL, OnPropertyChanged)
// END_MESSAGE_MAP()
//
// void CPropertiesView::OnPropertyChanged(NMHDR * pNotifyStruct, LRESULT * /*result*/)
// {
//     XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
//
//     switch (pItemNotify->pItem->GetItemData())
//     {
//         // Some code.
//     }
// }
// </code>
// See Also: XTP_NM_REPORTINPLACEBUTTON
//===========================================================================
struct XTP_NM_REPORTRECORDITEM
{
	NMHDR hdr;                          // Standard structure, containing information about a notification message.
	CXTPReportRow* pRow;                // Pointer to the row associated with the notification.
	CXTPReportRecordItem* pItem;        // Pointer to the record item associated with the notification.
	CXTPReportColumn* pColumn;          // Pointer to the column associated with the notification.
	int   nHyperlink;                   // Index of clicked Hyperlink, if any, or -1 otherwise.
	POINT pt;                           // Point where the message has happened.
};


//-----------------------------------------------------------------------
// Summary:
//     This structure is sent to Main window in a WM_NOTIFY message from Item
//     and provides all parameters that are needed in processing control specific
//     notifications by the main window
// Remarks:
//     Use this structure to get in-place button which produce XTP_NM_REPORT_INPLACEBUTTONDOWN message.
// See Also: XTP_NM_REPORT_INPLACEBUTTONDOWN
//-----------------------------------------------------------------------
struct XTP_NM_REPORTINPLACEBUTTON : public XTP_NM_REPORTRECORDITEM
{
	CXTPReportInplaceButton* pButton;           // Pointer to in-place button
};

//-----------------------------------------------------------------------
// Summary:
//     Record item drawing metrics.
//     Its helper structures group together parameters to store general
//     drawing metrics
//-----------------------------------------------------------------------
struct XTP_REPORTRECORDITEM_METRICS : public CCmdTarget
{
	CFont* pFont;                       // Drawing font.
	COLORREF clrForeground;             // Item foreground color.
	COLORREF clrBackground;             // Item background color.
	CString strText;                    // Item text.

};


//===========================================================================
// Summary:
//     CXTPReportRecordItemConstraint is a CCmdTarget derived class. It
//     represents a single item constraints.
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRecordItemConstraint : public CCmdTarget
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemConstraint object.
	//-------------------------------------------------------------------------
	CXTPReportRecordItemConstraint();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the index of this constraint
	//     within the collection of constraints.
	// Returns:
	//     Index of this constraint.
	//-----------------------------------------------------------------------
	int GetIndex();

public:
	CString m_strConstraint;    // Caption text of constraint.  This is the
	                            // text displayed for this constraint.
	DWORD_PTR   m_dwData;       // The 32-bit value associated with the item.

protected:
	int m_nIndex;               // Index of constraint.

private:
	friend class CXTPReportRecordItemConstraints;
	friend class CXTPReportRecordItemEditOptions;
};

//===========================================================================
// Summary:
//     CXTPReportRecordItemConstraints is a CCmdTarget derived class. It represents the item
//     constraints collection.
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRecordItemConstraints : public CCmdTarget
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportRecordItemConstraints object.
	//-------------------------------------------------------------------------
	CXTPReportRecordItemConstraints();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropertyGridItemConstraints object, handles cleanup and deallocation
	//-------------------------------------------------------------------------
	~CXTPReportRecordItemConstraints();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine the total number of constraints in the list.
	// Returns:
	//     Returns the total number of constraints added to the ReportRecordItem and\or ReportColumn.
	//-----------------------------------------------------------------------
	int GetCount();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to remove all constraints from the list of constraints.
	//-------------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve a constraint at nIndex.
	// Parameters:
	//     nIndex - Position in constraint collection.
	// Returns:
	//     Pointer to the constraint at nIndex in the collection of constraints.
	//-----------------------------------------------------------------------
	CXTPReportRecordItemConstraint* GetAt(int nIndex);

protected:
	CArray<CXTPReportRecordItemConstraint*, CXTPReportRecordItemConstraint*> m_arrConstraints;  // Collection of constraints

private:

	friend class CXTPReportRecordItemEditOptions;

};


//===========================================================================
// Summary:
//     This class represents collection of the in-place buttons
//     of the single item of the report control.
//===========================================================================
class _XTP_EXT_CLASS CXTPReportInplaceButtons : public CArray<CXTPReportInplaceButton*, CXTPReportInplaceButton*>
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportInplaceButtons object
	//-----------------------------------------------------------------------
	CXTPReportInplaceButtons();
};


//===========================================================================
// Summary:
//     This class represents edit options of the single item or column
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRecordItemEditOptions : public CCmdTarget
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportRecordItemEditOptions object.
	//-------------------------------------------------------------------------
	CXTPReportRecordItemEditOptions();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPReportRecordItemEditOptions object, handles
	//     cleanup and deallocation
	//-------------------------------------------------------------------------
	~CXTPReportRecordItemEditOptions();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds new constraint to constraint list.
	// Parameters:
	//     lpszConstraint - Caption of the constraint to be added.
	//     dwData         - The 32-bit value associated with the constraint.
	//-----------------------------------------------------------------------
	CXTPReportRecordItemConstraint* AddConstraint(LPCTSTR lpszConstraint, DWORD_PTR dwData = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Finds constraint by its value or caption
	// Parameters:
	//     dwData - The 32-bit value associated with the constraint.
	//     lpszConstraint - caption of the constraint
	// Returns:
	//     Constraint pointer if found or NULL  if the constraint
	//     is not found.
	//-----------------------------------------------------------------------
	CXTPReportRecordItemConstraint* FindConstraint(DWORD_PTR dwData);
	CXTPReportRecordItemConstraint* FindConstraint(LPCTSTR lpszConstraint); // <COMBINE CXTPReportRecordItemEditOptions::FindConstraint@DWORD_PTR>

	//-------------------------------------------------------------------------
	// Summary:
	//     Retrieves constraints pointer.
	// Returns:
	//     Constraint pointer of the edit options.
	//-------------------------------------------------------------------------
	CXTPReportRecordItemConstraints* GetConstraints() {
		return m_pConstaints;
	}

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to add a combo button to the CXTPReportRecordItem.
	// Remarks:
	//     This adds a drop-down combo button to the ReportRecordItem.  When
	//     the button is pressed, it will display all of the data items that
	//     were added as CXTPReportRecordItemConstraints.
	//-------------------------------------------------------------------------
	void AddComboButton();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this member to add a expand button to the CXTPReportRecordItem.
	// Remarks:
	//     A small button with three ellipses is displayed and you can use this
	//     button to display your own custom dialog.
	//-------------------------------------------------------------------------
	void AddExpandButton();

public:
	BOOL                             m_bAllowEdit;          // TRUE to add an edit box to this item.
	BOOL                             m_bConstraintEdit;     // If TRUE, then you can only choose from the list of constraints added, If FALSE, then you can type a custom response not listed in the list of constraints.
	CXTPReportRecordItemConstraints* m_pConstaints;         // Constraint list.
	CXTPReportInplaceButtons         arrInplaceButtons;     // Array of in-place buttons.

};

//===========================================================================
// Summary:
//     Class for working with single list cell.
//     Base class for extended list cells. It determines the behavior
//     of all specific record items.
//     You don't use its class directly in the control.
//     If you wish to add your own Record Item to the control, you must inherit
//     it from CXTPReportRecordItem. Most member functions in this class
//     are virtual.
// See Also:
//     CXTPReportRecordItemDateTime, CXTPReportRecordItemNumber,
//     CXTPReportRecordItemPreview, CXTPReportRecordItemText,
//     CXTPReportRecordItemVariant
//===========================================================================
class _XTP_EXT_CLASS CXTPReportRecordItem : public CCmdTarget
{
	DECLARE_DYNAMIC(CXTPReportRecordItem)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPReportRecordItem default constructor
	//-----------------------------------------------------------------------
	CXTPReportRecordItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPReportRecordItem default destructor
	//-----------------------------------------------------------------------
	virtual ~CXTPReportRecordItem(void);

	//-----------------------------------------------------------------------
	// Summary:
	//     Should be overridden by descendants for drawing itself.
	// Parameters:
	//     pDrawArgs - structure which contains drawing arguments:
	// Remarks:
	//     Call this member function to draw an item. Actually this function
	//     only prepares and calls needed drawing functions from PaintManager.
	//     Thus if you wish to change the look of your report item, you must just provide
	//     your own implementation of PaintManager
	// Example:
	// <code>
	// // fill structure
	// XTP_REPORTRECORDITEM_DRAWARGS drawArgs;
	// drawArgs.pDC = pDC;
	// drawArgs.pControl = m_pControl;
	// drawArgs.pRow = this;
	// // call function
	// Draw(&drawArgs);
	// </code>
	//
	// See Also: XTP_REPORTRECORDITEM_DRAWARGS
	//-----------------------------------------------------------------------
	virtual int Draw(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Should be overridden by descendants for drawing itself.
	// Parameters:
	//     pDrawArgs - structure which contain drawing arguments.
	//     pMetrics - structure which contain metrics of the item.
	//-----------------------------------------------------------------------
	virtual void OnDrawCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes single mouse clicks.
	// Parameters:
	//     pClickArgs - structure which contains mouse click arguments
	// Remarks:
	//     Usually this function is called by ReportRow's OnClick function
	// See Also: XTP_REPORTRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnClick(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes single mouse clicks.
	// Parameters:
	//     pClickArgs - structure which contains mouse click arguments:
	// Remarks:
	//     Usually this function is called by ReportRow's OnDblClick function
	// See Also: XTP_REPORTRECORDITEM_CLICKARGS
	//-----------------------------------------------------------------------
	virtual void OnDblClick(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Processes the move mouse event.
	// Parameters:
	//     point  - mouse point
	//     nFlags - additional flags.
	// Remarks:
	//     Usually this function is called by ReportRow's OnDblClick function
	//-----------------------------------------------------------------------
	virtual void OnMouseMove(UINT nFlags, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns item text caption.
	// Parameters:
	//     pColumn - Corresponded column of the item.
	// Remarks:
	//     Can be overridden by descendants.
	// Returns:
	//     Item text caption (empty string for base record item class).
	//-----------------------------------------------------------------------
	virtual CString GetCaption(CXTPReportColumn* pColumn) {
		UNREFERENCED_PARAMETER(pColumn); return m_strCaption;
	};

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves caption text bounding rectangle
	// Parameters:
	//     pDrawArgs - structure which contain drawing arguments.
	//     rcItem    - Bounding rectangle of the item
	//-----------------------------------------------------------------------
	virtual void GetCaptionRect(XTP_REPORTRECORDITEM_ARGS* pDrawArgs, CRect& rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns group caption.
	// Parameters:
	//     pColumn - point to the column
	// Returns:
	//     Text of Group Caption.
	//-----------------------------------------------------------------------
	virtual CString GetGroupCaption(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns resource id of string containing text
	// Parameters:
	//     pColumn - Point to the column
	// Returns:
	//     Integer value of resource id
	//-----------------------------------------------------------------------
	virtual int GetGroupCaptionID(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares group captions.
	// Parameters:
	//     pColumn - Point to the column
	//     pItem   - Point to the item
	// Returns:
	//     Zero if the items' values are identical,
	//     < 0 if this item value is less than provided,
	//     or > 0 if this item value is greater than provided.
	//-----------------------------------------------------------------------
	virtual int CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Compares this item with the provided one.
	//     Provides default behavior for descendants.
	// Parameters:
	//     pColumn - Corresponded column of the items.
	//     pItem - points to the compared Item
	// Returns:
	//     Zero if the items' values are identical,
	//     < 0 if this item value is less than provided,
	//     or > 0 if this item value is greater than provided.
	//-----------------------------------------------------------------------
	virtual int Compare(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets item text font.
	// Parameters:
	//     pFont - New font for caption.
	//-----------------------------------------------------------------------
	virtual void SetFont(CFont* pFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get item text font.
	// Returns:
	//     Pointer to the current text font.
	//-----------------------------------------------------------------------
	virtual CFont* GetFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets item text color.
	// Parameters:
	//     clrText - New color.
	//-----------------------------------------------------------------------
	virtual void SetTextColor(COLORREF clrText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets/clears bold flag for the item font.
	// Parameters:
	//     bBold - If TRUE set font to bold.
	// Remarks:
	//     By using this function you can easily change font to bold or to normal.
	//-----------------------------------------------------------------------
	virtual void SetBold(BOOL bBold = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets item text background color.
	// Parameters:
	//     clrBackground - New background color.
	//-----------------------------------------------------------------------
	virtual void SetBackgroundColor(COLORREF clrBackground);

	//-----------------------------------------------------------------------
	// Summary:
	//     Gets item caption format string.
	// Returns:
	//     String object, containing current format string
	//-----------------------------------------------------------------------
	virtual CString GetFormatString();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets item caption format string.
	// Parameters:
	//     strFormat - New format string.
	// Remarks:
	//     If the format string is set, a caption text will be formatted
	//     accordingly to this format string before drawing it; convenient
	//     in many cases (drawing date for example). Format string is C - like
	//     style, see sprintf() C function or CString.Format() member function
	// Returns:
	//     Old caption format string.
	//-----------------------------------------------------------------------
	virtual CString SetFormatString(CString strFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to programmatically check if item is
	//     editable.
	// Returns:
	//     BOOLEAN value represents current editable flag
	//-----------------------------------------------------------------------
	virtual BOOL IsEditable();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets editable flag.
	// Parameters:
	//     bEditable - new editable flag (default is TRUE).
	// Returns:
	//     Old item editable state.
	//-----------------------------------------------------------------------
	virtual BOOL SetEditable(BOOL bEditable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds new Hyperlink.
	// Parameters:
	//     pHyperlink - pointer to hyperlink.
	// Remarks:
	//     Each record item can have a number of hyperlinks.
	//     To add new hyperlink to record item you firs create create
	//     the new instance of hyperlink class.
	// Returns:
	//     zero-based id of new Hyperlink.
	// Example:
	//     <code>AddHyperlink(new CXTPReportHyperlink(27, 7);</code>
	//
	// See Also: CXTPReportHyperlink.
	//-----------------------------------------------------------------------
	int AddHyperlink(CXTPReportHyperlink* pHyperlink);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a count of hyperlinks in record item
	// Remarks:
	//     Each record item can have a number of hyperlinks. To process
	//     all of them, you get the count of hyperlinks that are processed by an item
	//     using GetHyperlinksCount() member function
	// Returns:
	//     Count of hyperlinks in item text
	//-----------------------------------------------------------------------
	int GetHyperlinksCount();

	//-----------------------------------------------------------------------
	// Summary:
	//     Return Hyperlink by id.
	// Parameters:
	//     nHyperlink - zero-based index of hyperlink.
	// Remarks:
	//     To process a hyperlink, you get a pointer to it by calling GetHyperlinkAt();
	// Returns:
	//     Pointer to Hyperlink.
	// Example:
	// <code>
	// int nHyperlinks = GetHyperlinksCount();
	// CXTPReportHyperlink* pHyperlink;
	// for(int nHyperlink = 0; nHyperlink < nHyperlinks; nHyperlink++)
	// {
	//     pHyperlink = GetHyperlinkAt(nHyperlink);
	//     // YOUR PROCESSING HERE
	//     // ...
	//     //
	// }
	// </code>
	// See Also: CXTPReportHyperlink.
	//-----------------------------------------------------------------------
	CXTPReportHyperlink* GetHyperlinkAt(int nHyperlink);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills XTP_REPORTRECORDITEM_METRICS structure
	// Parameters:
	//     pDrawArgs    - Draw arguments for calculating item metrics.
	//     pItemMetrics - Pointer to the metrics item to fill with values.
	// Remarks:
	//     Calculates preview item metrics based on provided draw arguments.
	//     Could be overridden by descendants.
	// See Also: XTP_REPORTRECORDITEM_DRAWARGS, XTP_REPORTRECORDITEM_METRICS
	//-----------------------------------------------------------------------
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if this item is a preview item.
	// Returns:
	//     TRUE for the preview item, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsPreviewItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns item index.
	// Returns:
	//     Item numeric index.
	//-----------------------------------------------------------------------
	int GetIndex();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns an ID of the associated resource bitmap.
	// Returns:
	//     An ID of the associated resource bitmap.
	//-----------------------------------------------------------------------
	int GetIconIndex();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets new associated bitmap ID.
	// Parameters:
	//     nIconIndex - ID of the bitmap from the application resources.
	// Returns:
	//     Returns an ID of old resource bitmap.
	//-----------------------------------------------------------------------
	int SetIconIndex(int nIconIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves sort priority of the item.
	// See Also: GetSortPriority, SetSortPriority, SetGroupPriority, GetGroupPriority
	//-----------------------------------------------------------------------
	int GetSortPriority();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the sort priority of the item.
	// Parameters:
	//     nSortPriority - Priority will be used for sort routines.
	// See Also:
	//     GetSortPriority, SetSortPriority, SetGroupPriority, GetGroupPriority
	//-----------------------------------------------------------------------
	void SetSortPriority(int nSortPriority);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves sort priority of the item.
	// See Also: GetSortPriority, SetSortPriority, SetGroupPriority, GetGroupPriority
	//-----------------------------------------------------------------------
	int GetGroupPriority();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the group priority of the item.
	// Parameters:
	//     nGroupPriority - Priority will be used for sort routines.
	// See Also: GetSortPriority, SetSortPriority, SetGroupPriority, GetGroupPriority
	//-----------------------------------------------------------------------
	void SetGroupPriority(int nGroupPriority);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the group caption of the item.
	// Parameters:
	//     strCaption - Caption of group to set.
	//-----------------------------------------------------------------------
	void SetGroupCaption(CString strCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets caption of the item.
	// Parameters:
	//     strCaption - Caption of item to set.
	//-----------------------------------------------------------------------
	void SetCaption(CString strCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check the item.
	// Parameters:
	//     bChecked - TRUE if item is checked.
	// See Also:
	//     IsChecked, HasCheckbox
	//-----------------------------------------------------------------------
	void SetChecked(BOOL bChecked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the item is checked
	// See Also: SetChecked, HasCheckbox
	//-----------------------------------------------------------------------
	BOOL IsChecked();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds check box area for item.
	// Parameters:
	//     bHasCheckbox - TRUE if item has check box area.
	// See Also: SetChecked, IsChecked
	//-----------------------------------------------------------------------
	void HasCheckbox(BOOL bHasCheckbox);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieved parent record object.
	// Returns:
	//     Pointer to parent record object.
	//-----------------------------------------------------------------------
	CXTPReportRecord* GetRecord () const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves the application-supplied 32-bit value
	//     associated with the item.
	// Returns:
	//     The 32-bit value associated with the item.
	//-----------------------------------------------------------------------
	DWORD_PTR GetItemData();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the 32-bit value associated with the item.
	// Parameters:
	//     dwData - Contains the new value to associate with the item.
	//-----------------------------------------------------------------------
	void SetItemData(DWORD_PTR dwData);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws check box bitmap
	// Parameters:
	//     pDrawArgs - Pointer to structure with drawing arguments
	//     rcItem    - Item area rectangle coordinates.
	//-----------------------------------------------------------------------
	virtual void DrawCheckBox(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, CRect& rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determine if the click occurred on the hyperlink
	// Parameters:
	//     ptClick - point of the mouse click
	// Returns:
	//     Zero-based index of clicked link, -1 otherwise
	//-----------------------------------------------------------------------
	int HitTestHyperlink(CPoint ptClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if a CXTPReportRecordItem can receive focus.
	// Returns:
	//     TRUE is the CXTPReportRecordItem can receive focus, FALSE if
	//     it can not receive focus.
	//-----------------------------------------------------------------------
	BOOL IsFocusable();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether the CXTPReportRecordItem can have focus.
	// Parameters:
	//     bFocusable - TRUE to allow the item to have focus, FALSE to specify the
	//                  item can not have focus.
	// Remarks:
	//     This will override the CXTPReportControl::FocusSubItems setting which only
	//     allows individual items to receive focus.  The SetFocusable member specifies
	//     whether the item can receive focus, if FALSE, the item can not be edited
	//     and the combo or expand buttons will not be displayed.
	//-----------------------------------------------------------------------
	void SetFocusable(BOOL bFocusable);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when user begin edit item.
	// Parameters:
	//     pItemArgs - Pointer to structure with items arguments.
	//-----------------------------------------------------------------------
	virtual void OnBeginEdit(XTP_REPORTRECORDITEM_ARGS* pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when user cancel edit item.
	// Parameters:
	//     pControl - Parent report control.
	//     bApply   - TRUE to save value of in-place edit.
	//-----------------------------------------------------------------------
	virtual void OnCancelEdit(CXTPReportControl* pControl, BOOL bApply);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when user press key.
	// Parameters:
	//     pItemArgs - Pointer to structure with items arguments.
	//     nChar     - Pressed character code
	// Returns:
	//     TRUE if item proceeded key.
	//-----------------------------------------------------------------------
	virtual BOOL OnChar(XTP_REPORTRECORDITEM_ARGS* pItemArgs, UINT nChar);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//    This method is called to save value from in-place edit
	// Parameters:
	//    pItemArgs - Pointer to structure with items arguments.
	//-----------------------------------------------------------------------
	virtual void OnValidateEdit(XTP_REPORTRECORDITEM_ARGS* pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//    This method is called when value of in-place edit control changed
	// Parameters:
	//    pItemArgs - Pointer to structure with items arguments.
	//    szText    - New in-place edit text.
	//-----------------------------------------------------------------------
	virtual void OnEditChanged(XTP_REPORTRECORDITEM_ARGS* /*pItemArgs*/, LPCTSTR /*szText*/) {}

	//-----------------------------------------------------------------------
	// Summary:
	//    This method is called when user select specified constraint in in-place list
	// Parameters:
	//    pItemArgs   - Pointer to structure with items arguments
	//    pConstraint - Selected constraint
	//-----------------------------------------------------------------------
	virtual void OnConstraintChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CXTPReportRecordItemConstraint* pConstraint);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves associated value with selected constraint.
	// Parameters:
	//     pItemArgs - Pointer to structure with items arguments
	// Returns:
	//     DWORD value associated with selected constraint.
	//-----------------------------------------------------------------------
	virtual DWORD GetSelectedConstraintData(XTP_REPORTRECORDITEM_ARGS* pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when user press in-place button of the item.
	// Parameters:
	//     pButton - In-place button of the item.
	//-----------------------------------------------------------------------
	virtual void OnInplaceButtonDown(CXTPReportInplaceButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves edit options of item.
	// Parameters:
	//     pColumn - Column of the item.
	// Returns:
	//     Pointer to CXTPReportRecordItemEditOptions class.
	//-----------------------------------------------------------------------
	CXTPReportRecordItemEditOptions* GetEditOptions(CXTPReportColumn* pColumn);

private:
	BOOL IsAllowEdit(XTP_REPORTRECORDITEM_ARGS* pItemArgs);
	void DoMouseButtonClick();

protected:

	CFont* m_pFontCaption;      // Storage for item caption font.

	COLORREF m_clrText;         // Storage for item text color.
	COLORREF m_clrBackground;   // Storage for item background color.
	BOOL m_bBoldText;           // Is bold text.

	CString m_strFormatString;  // Stores string for extra formatting of item caption.

	BOOL m_bEditable;           // Stores editable flag.
	CArray< CXTPReportHyperlink*, CXTPReportHyperlink* > m_arrHyperlinks; // Array of the Item's hyperlinks

	CXTPReportRecord* m_pRecord;    // Pointer to the associated record item, if any, or NULL otherwise.

	int m_nIconIndex;               // ID of the bitmap from the application resources.

	int m_nSortPriority;            // Sort priority
	int m_nGroupPriority;           // Group priority

	CString m_strGroupCaption;      // Caption of the group
	CString m_strCaption;           // Caption of the item
	BOOL m_bFocusable;              // TRUE if item accept focus.

	BOOL m_bChecked;                // TRUE if item checked.
	BOOL m_bHasCheckbox;            // TRUE if item has check box.
	DWORD_PTR m_dwData;             // The 32-bit value associated with the item.

	CXTPReportRecordItemEditOptions* m_pEditOptions; // Edit options of the item.


	friend class CXTPReportRecord;
	friend class CXTPReportControl;
};

AFX_INLINE CString CXTPReportRecordItem::GetFormatString() {
	return m_strFormatString;
}

AFX_INLINE CString CXTPReportRecordItem::SetFormatString(CString strFormat) {
	CString sOldString(m_strFormatString);
	m_strFormatString = strFormat;
	return sOldString;
}


AFX_INLINE BOOL CXTPReportRecordItem::SetEditable(BOOL bEditable) {
	BOOL bOldEditable = m_bEditable;
	m_bEditable = bEditable;
	return bOldEditable;
}

AFX_INLINE int CXTPReportRecordItem::AddHyperlink(CXTPReportHyperlink* pHyperlink){
	return (int)m_arrHyperlinks.Add(pHyperlink);
}

AFX_INLINE int CXTPReportRecordItem::GetHyperlinksCount(){
	return (int)m_arrHyperlinks.GetSize();
}

AFX_INLINE CXTPReportHyperlink* CXTPReportRecordItem::GetHyperlinkAt(int nHyperlink){
	return m_arrHyperlinks.GetAt(nHyperlink);
}

AFX_INLINE BOOL CXTPReportRecordItem::IsPreviewItem() {
	return FALSE;
}
AFX_INLINE int CXTPReportRecordItem::GetIconIndex() {
	return m_nIconIndex;
}
AFX_INLINE int CXTPReportRecordItem::SetIconIndex(int nIconIndex){
	int nOldID = GetIconIndex(); m_nIconIndex = nIconIndex; return nOldID;
}
AFX_INLINE int CXTPReportRecordItem::GetSortPriority() {
	return m_nSortPriority;
}
AFX_INLINE void CXTPReportRecordItem::SetSortPriority(int nSortPriority) {
	m_nSortPriority = nSortPriority;
}
AFX_INLINE int CXTPReportRecordItem::GetGroupPriority() {
	return m_nGroupPriority;
}
AFX_INLINE void CXTPReportRecordItem::SetGroupPriority(int nGroupPriority) {
	m_nGroupPriority = nGroupPriority;
}
AFX_INLINE void CXTPReportRecordItem::SetGroupCaption(CString strCaption) {
	m_strGroupCaption = strCaption;
}
AFX_INLINE void CXTPReportRecordItem::SetCaption(CString strCaption) {
	m_strCaption = strCaption;
}
AFX_INLINE void CXTPReportRecordItem::SetChecked(BOOL bChecked) {
	m_bChecked = bChecked;
}

AFX_INLINE BOOL CXTPReportRecordItem::IsChecked() {
	return m_bChecked;
}
AFX_INLINE void CXTPReportRecordItem::HasCheckbox(BOOL bHasCheckbox) {
	m_bHasCheckbox = bHasCheckbox;
}
AFX_INLINE CXTPReportRecord* CXTPReportRecordItem::GetRecord () const {
	return m_pRecord;
}
AFX_INLINE void CXTPReportRecordItem::SetFocusable(BOOL bFocusable) {
	m_bFocusable = bFocusable;
}
AFX_INLINE DWORD_PTR CXTPReportRecordItem::GetItemData() {
	return m_dwData;
}
AFX_INLINE void CXTPReportRecordItem::SetItemData(DWORD_PTR dwData) {
	m_dwData = dwData;
}
AFX_INLINE DWORD CXTPReportRecordItem::GetSelectedConstraintData(XTP_REPORTRECORDITEM_ARGS* /*pItemArgs*/) {
	return DWORD(-1);
}

#endif //#if !defined(__XTPREPORTRECORDITEM_H__)
