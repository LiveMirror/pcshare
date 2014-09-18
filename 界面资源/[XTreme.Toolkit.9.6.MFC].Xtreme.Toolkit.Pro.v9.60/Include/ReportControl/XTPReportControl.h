// CXTPReportControl.h: interface for the XTPReportControl class.
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
#if !defined(__XTPREPORTCONTROL_H__)
#define __XTPREPORTCONTROL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTPReportDefines.h"
#include "XTPReportRow.h"
#include "XTPReportRows.h"
#include "XTPReportPaintManager.h"
#include "XTPReportTip.h"

struct XTP_REPORTRECORDITEM_DRAWARGS;
struct XTP_REPORTRECORDITEM_METRICS;
class CXTPReportRecords;
class CXTPReportRecordItem;
class CXTPReportColumns;
class CXTPReportGroups;
class CXTPReportColumn;
class CXTPReportRecord;
class CXTPReportRows;
class CXTPReportPaintManager;
class CXTPReportColumnOrder;
class CXTPImageManager;
class CXTPReportInplaceEdit;
class CXTPReportInplaceButtons;
class CXTPReportInplaceList;
class CXTPPropExchange;

struct XTP_REPORTRECORDITEM_ARGS;


//===========================================================================
// Summary:
//     Enumeration of operational mouse modes.
// Remarks:
//     ReportControl has several
//     Mouse states that handled by control. This enumeration helps to
//     clearly identify each of these
//         - Sends Notifications:
//         - Sends Messages:
// See Also: CXTPReportControl::SetMouseMode, CXTPReportControl::GetMouseMode
//
// <KEYWORDS xtpReportMouseNothing, xtpReportMouseOverColumnDivide, xtpReportMousePrepareDragColumn, xtpReportMouseDraggingColumn>
//===========================================================================
enum XTPReportMouseMode
{
	xtpReportMouseNothing,            // User is just watching to the list.
	xtpReportMouseOverColumnDivide,   // User watching, but mouse is under column divider.
	xtpReportMousePrepareDragColumn,  // User holds mouse button pressed on the column header.
	xtpReportMouseDraggingColumn      // User is dragging column header.
};


//===========================================================================
// Summary:
//     The CXTPReportControl class provides an implementation of
//     the Report control.
//
// Remarks:
//     A "report control" is a window that displays a hierarchical list
//     of items, such as emails in the inbox. Each item is called a CXTPReportRow
//     and consists of its properties and corresponding CXTPReportRecord,
//     which contains all the corresponding data (mostly text).
//     Each Row item (as well as Record) can have a list of sub-items
//     associated with it. By clicking a Row item, the user can expand and
//     collapse the associated list of sub-items.
//
//     The CXTPReportRecords collection holds all the CXTPReportRecord objects
//     that are assigned to the Report control. It could be accessible via
//     GetRecords() method. The records in this collection
//     are referred to as the root records. Any record that is subsequently
//     added to a root record is referred to as a child record. Because each
//     CXTPReportRecord can contain a collection of other CXTPReportRecord
//     objects, you might find it difficult to determine your location in the
//     tree structure when you iterate through the collection.
//
//     Record nodes can be expanded to display the next level of child records.
//     The user can expand the CXTPReportRecord by clicking the plus-sign (+)
//     button, if one is displayed, or you can expand the CXTPReportRecord by
//     calling the CXTPReportRecord::SetExpanded method.
//     To expand all the child records, call the ExpandAll method.
//     You can collapse the child CXTPReportRecord level by calling the
//     CXTPReportRecord::SetExpanded(FALSE) method, or the user can press
//     the minus-sign (-) button, if one is displayed. You can also call
//     CollapseAll method to collapse all child records.
//
//     Each record contains an array of record items which are implemented
//     with CXTPReportRecordItem and its descendants. You can create your own
//     types of items simply by inheritance from the base record item class.
//
//     Each record item has an association with corresponding CXTPReportColumn
//     item. The item will be shown below the corresponding column header
//     depending on its position in report control columns array. If a column
//     has not an associated item in the record, there will be an empty item
//     shown in the corresponding cell.
//
//     Columns array is represented by CXTPReportColumns collection and could
//     be accessed via GetColumns() method.
//
//     As a finalization of adding data to the report control, which means
//     adding columns and records, Populate() method should be called. It
//     performs population of control rows with data - creates a rows tree if
//     necessary, rebuilds groups if grouping if enabled, and sorts rows
//     on a specified manner. See Also an example below.
//
//     Handling notification messages sent by the control to the parent
//     window is allowed with ON_NOTIFY handler. The control is using
//     SendMessageToParent function to send notifications. See below for
//     the example of how messages could be handled in a parent window:
//
// <code>
// ON_NOTIFY(NM_CLICK, ID_REPORT_CONTROL, OnReportItemClick)
// ON_NOTIFY(NM_RCLICK, ID_REPORT_CONTROL, OnReportItemRClick)
// ON_NOTIFY(NM_DBLCLK, ID_REPORT_CONTROL, OnReportItemDblClick)
// ON_NOTIFY(XTP_NM_SHOWFIELDCHOOSER, ID_REPORT_CONTROL, OnShowFieldChooser)
// ON_NOTIFY(XTP_NM_HEADER_RCLICK, ID_REPORT_CONTROL, OnReportColumnRClick)
// ON_NOTIFY(NM_KEYDOWN, ID_REPORT_CONTROL, OnReportKeyDown)
// </code>
//
//     You can also change the appearance of the CXTPReportControl control
//     by setting some of its display and style properties.
//
//     Also Report control has an ability to store and restore its
//     settings, which includes all columns with their settings, and some
//     required control's settings. It is implemented via standard MFC
//     serialization and available with the member function
//     SerializeState(CArchive& ar);
//
// See Also:
//     CXTPReportView, CXTPReportHeader, CXTPReportRow, CXTPReportRecord, CXTPReportColumn,
//     CXTPReportRecords, CXTPReportRows, CXTPReportColumns,
//     CXTPReportSubListControl, CXTPReportFilterEditControl
//===========================================================================
class _XTP_EXT_CLASS CXTPReportControl : public CWnd
{
	DECLARE_DYNCREATE(CXTPReportControl)

	friend class CXTPReportSubListControl;
	friend class CXTPReportRow;
	friend class CXTPReportHeader;
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportControl object.
	// Remarks:
	//     You construct a CXTPReportControl object in two steps.
	//     First, call the constructor CXTPReportControl and then call
	//     Create method, which initializes the window.
	//
	// Example:
	// <code>
	// // Declare a local CXTPReportControl object.
	// CXTPReportControl myReport;
	//
	// // Declare a dynamic CXTPReportControl object.
	// CXTPReportControl* pMyReport = new CXTPReportControl();
	//
	// if (!myReport.Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|WM_VSCROLL, CRect(0, 0, 0, 0), this, ID_REPORT_CONTROL))
	// {
	//     TRACE(_T("Failed to create view window\n"));
	// }
	// </code>
	// See Also: Create
	//-----------------------------------------------------------------------
	CXTPReportControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPReportControl object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPReportControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to create a report control.
	// Parameters:
	//     dwStyle     - Specifies the window style attributes.
	//                   It could be a combination of standard window styles.
	//     rect        - The size and position of the window, in client
	//                   coordinates of pParentWnd.
	//     pParentWnd  - Specifies the report control parent window.
	//     nID         - Specifies the report control identifier.
	//     pContext    - The create context of the window.
	// Remarks:
	//     You construct a CXTPReportControl object in two steps.
	//     First, call the constructor CXTPReportControl and then call
	//     Create method, which initializes the window.
	// Example:
	//     See the example for CXTPReportControl::CXTPReportControl
	// Returns:
	//     Nonzero if successful; otherwise 0.
	// See Also: CXTPReportControl::CXTPReportControl
	//-----------------------------------------------------------------------
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     (Re)Stores control configuration to/from the provided archive stream.
	// Parameters:
	//     ar - Archive stream for serializing.
	//-----------------------------------------------------------------------
	void SerializeState(CArchive& ar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes configuration to/from the provided archive stream.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds new column to the end of the columns array.
	// Parameters:
	//     pColumn - Pointer to the specified column class object.
	// Remarks:
	//     Call this member function if you want to add a new column
	//     to a report control. It will be added to the end of the columns array.
	// Example:
	// <code>
	// // this function adds a column with "Subject" caption and 250 pixels initial width
	// void AddSubjectColumn(CXTPReportControl* pReportCtrl)
	// {
	//     pReportCtrl->AddColumn(new CXTPReportColumn(1, _T("Subject"), 250));
	// }
	// </code>
	// See Also: CXTPReportColumn overview
	//-----------------------------------------------------------------------
	CXTPReportColumn* AddColumn(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds record with data and associated row.
	// Parameters:
	//     pRecord - Record data with items.
	// Remarks:
	//     Call this member function if you want to add a data record to the
	//     report control's internal storage.
	// Example:
	// <code>
	// // this function adds 2 empty records to a report control
	// void Add2Empties(CXTPReportControl* pReportCtrl)
	// {
	//     pReportCtrl->AddRecord(new CXTPReportRecord());
	//     pReportCtrl->AddRecord(new CXTPReportRecord());
	// }
	// </code>
	// Returns:
	//     Pointer to the recently added record object.
	// See Also: CXTPReportRecord overview, GetRecords
	//-----------------------------------------------------------------------
	CXTPReportRecord* AddRecord(CXTPReportRecord* pRecord);

	//-----------------------------------------------------------------------
	// Summary:
	//     Prevents the control from redrawing until the EndUpdate
	//     method is called.
	// Remarks:
	//     If you want to add items one at a time using the AddRecord method,
	//     or to make some another operations in a single sequence,
	//     you can use the BeginUpdate method to prevent the control
	//     from repainting the CXTPReportControl each time an item is added.
	//     Once you have completed the task of adding items to the control,
	//     call the EndUpdate method to enable the CXTPReportControl to repaint.
	//     This way of adding items can prevent flickered drawing of
	//     the CXTPReportControl when a large number of items are being
	//     added to the control.
	// Example:
	// <code>
	// // This function collapses all rows for the specified report control
	// void CollapseAll(CXTPReportControl* pReportCtrl)
	// {
	//     pReportCtrl->BeginUpdate();
	//     for (int i = pReportCtrl->GetRows()->GetCount() - 1; i >= 0; i --)
	//         pReportCtrl->GetRows()->GetAt(i)->SetExpanded(FALSE);
	//
	//     pReportCtrl->EndUpdate();
	// }
	// </code>
	// See Also: EndUpdate, RedrawControl, AddRecord
	//-----------------------------------------------------------------------
	void BeginUpdate();

	//-----------------------------------------------------------------------
	// Summary:
	//     Resumes drawing of the report control after drawing is
	//     suspended by the BeginUpdate method.
	// Remarks:
	//     If you want to add items one at a time using the AddRecord method,
	//     or to make some other operations in a single sequence,
	//     you can use the BeginUpdate method to prevent the control
	//     from repainting the CXTPReportControl each time an item is added.
	//     Once you have completed the task of adding items to the control,
	//     call the EndUpdate method to enable the CXTPReportControl to repaint.
	//     This way of adding items can prevent flickered drawing of
	//     the CXTPReportControl when a large number of items are being
	//     added to the control.
	// Example:  See example for CXTPReportControl::BeginUpdate method.
	// See Also: BeginUpdate, RedrawControl
	//-----------------------------------------------------------------------
	void EndUpdate();

	//-----------------------------------------------------------------------
	// Summary:
	//     Initiates report control redrawing.
	// Remarks:
	//     Call this member function if you want to initialize the
	//     report control redrawing. The control will be redrawn taking
	//     into account its latest state.
	// See Also: BeginUpdate, EndUpdate
	//-----------------------------------------------------------------------
	void RedrawControl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Set or clear grid drawing style.
	// Parameters:
	//     bVertical - TRUE for changing vertical grid style,
	//                 FALSE for changing horizontal grid style.
	//     gridStyle - New grid style. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     Call this member function if you want to change a style
	//     of report grid lines.
	//
	//     Possible grid line styles are the following:
	//     * <b>xtpGridNoLines</b>  Empty line
	//     * <b>xtpGridSmallDots</b> Line is drawn with small dots
	//     * <b>xtpGridLargeDots</b> Line is drawn with large dots
	//     * <b>xtpGridDashes</b> Line is drawn with dashes
	//     * <b>xtpGridSolid</b> Draws solid line
	//
	// See Also: XTPReportGridStyle overview, GetGridStyle, SetGridColor
	//-----------------------------------------------------------------------
	void SetGridStyle(BOOL bVertical, XTPReportGridStyle gridStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns current grid drawing mode.
	// Parameters:
	//     bVertical - TRUE for vertical grid style,
	//                 FALSE for horizontal grid style.
	// Remarks:
	//     Call this member function if you want to retrieve current
	//     grid lines drawing style for the report control.
	// Returns:
	//     Current grid drawing style.
	// See Also: XTPReportGridStyle overview, SetGridStyle, SetGridColor
	//-----------------------------------------------------------------------
	XTPReportGridStyle GetGridStyle(BOOL bVertical);

	//-----------------------------------------------------------------------
	// Summary:
	//     Change color of GridLines.
	// Parameters:
	//     clrGridLine - New Grid Lines color.
	// Remarks:
	//     Call this member function if you want to change a color of
	//     report control grid lines.
	// Returns:
	//     Old Grid Lines color.
	// See Also: SetGridStyle, GetGridStyle
	//-----------------------------------------------------------------------
	COLORREF SetGridColor(COLORREF clrGridLine);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the collection of the data records.
	// Returns:
	//     The data records collection.
	// Remarks:
	//     Call this member function if you want to retrieve an access
	//     to the collection of report records. You may then perform
	//     standard operations on the collection like adding, removing, etc.
	// See Also: CXTPReportRecords overview, AddRecord
	//-----------------------------------------------------------------------
	CXTPReportRecords* GetRecords();


	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the collection of the report rows
	// Remarks:
	//     Use this member function to retrieve an access to the collection
	//     of report rows, representing current control view.
	//
	//     Note that rows collection could be rebuilt automatically
	//     on executing Populate method.
	// Returns:
	//     The report rows collection.
	// Example:
	//     See example for CXTPReportControl::BeginUpdate method.
	// See Also: CXTPReportRows overview, Populate
	//-----------------------------------------------------------------------
	CXTPReportRows* GetRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Interface for accessing list columns.
	// Remarks:
	//     Use this member function to retrieve an access to the collection
	//     of report columns.
	// Returns:
	//     The report Columns collection.
	// Example:
	// <code>
	// // this function adds a column with "Subject" caption and 250 pixels initial width
	// void AddSubjectColumn(CXTPReportControl* pReportCtrl)
	// {
	//     pReportCtrl->GetColumns()->Add(new CXTPReportColumn(1, _T("Subject"), 250));
	// }
	// </code>
	// See Also: CXTPReportColumns overview
	//-----------------------------------------------------------------------
	CXTPReportColumns* GetColumns();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the currently used control's Paint Manager.
	// Remarks:
	//     Call this member function to get the paint manager object used
	//     for drawing a report control window.
	// Returns:
	//     Pointer to the paint manager object.
	// See Also: CXTPReportPaintManager overview
	//-----------------------------------------------------------------------
	CXTPReportPaintManager* GetPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set custom paint manager.
	// Parameters:
	//     pPaintManager - A pointer to the custom paint manager.
	//-----------------------------------------------------------------------
	void SetPaintManager(CXTPReportPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs control population, creating view from the data.
	// Remarks:
	//     Call this member function to populate control's Rows collection
	//     with the data containing in Records collection.
	//     It automatically creates Tree View references if necessary
	//     (for example, in Grouping mode).
	//     , it is the main function which should be called for
	//     (re)populating all data changes that you have made into the
	//     Records collection.
	// See Also: CXTPReportPaintManager overview
	//-----------------------------------------------------------------------
	virtual void Populate();

	//-----------------------------------------------------------------------
	// Summary:
	//     Ensures that a report control row is at least partially visible.
	// Parameters:
	//     pCheckRow - A pointer to the row that is to be visible.
	// Remarks:
	//     Ensures that a report row item is at least partially visible.
	//     The list view control is scrolled if necessary.
	// See Also: MoveDown, MoveUp, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	void EnsureVisible(CXTPReportRow* pCheckRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines which report row item, if any, is at a specified position.
	// Parameters:
	//     pt - A point to test.
	// Returns:
	//     The row item at the position specified by pt, if any,
	//     or NULL otherwise.
	//-----------------------------------------------------------------------
	CXTPReportRow* HitTest(CPoint pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the collection of the selected rows.
	// Remarks:
	//     Use this member function to retrieve an access to the collection
	//     of currently selected report rows.
	// Returns:
	//     The selected rows collection.
	// See Also: CXTPReportSelectedRows overview.
	//-----------------------------------------------------------------------
	CXTPReportSelectedRows* GetSelectedRows();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the index of the row that currently has focus
	//     in the report control's view.
	// Returns:
	//     Returns pointer to the focused row, or NULL otherwise.
	// See Also: SetFocusedRow
	//-----------------------------------------------------------------------
	CXTPReportRow* GetFocusedRow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Makes the provided row as currently focused.
	// Parameters:
	//     pRow - The row to set as focused to.
	//     bSelectBlock - TRUE if select rows block from focused row to pRow.
	//     bIgnoreSelection - TRUE if select new focused row.
	// See Also: GetFocusedRow
	//-----------------------------------------------------------------------
	void SetFocusedRow(CXTPReportRow* pRow, BOOL bIgnoreSelection = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row down.
	// Parameters:
	//     bSelectBlock - TRUE when selecting a new focused row, FALSE otherwise.
	//     bIgnoreSelection - TRUE if ignore current rows selection.
	// Remarks:
	//     Call this member function if you want to move the currently
	//     focused row down by 1.
	// See Also: MoveUp, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	void MoveDown(BOOL bSelectBlock, BOOL bIgnoreSelection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row up.
	// Parameters:
	//     bSelectBlock     - TRUE when selecting a new focused row, FALSE otherwise.
	//     bIgnoreSelection - TRUE if ignore current rows selection.
	// Remarks:
	//     Call this member function if you want to move the currently
	//     focused row up by 1.
	// See Also: MoveDown, MovePageDown, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	void MoveUp(BOOL bSelectBlock, BOOL bIgnoreSelection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row down at one page.
	// Parameters:
	//     bSelectBlock     - TRUE when selecting rows up to new focused row,
	//                        FALSE otherwise.
	//     bIgnoreSelection - TRUE if ignore current rows selection.
	// Remarks:
	//     Call this member function if you want to move currently
	//     focused row down by 1 page (an count of rows visible in a view).
	// See Also: MoveDown, MoveUp, MovePageUp, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	void MovePageDown(BOOL bSelectBlock, BOOL bIgnoreSelection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row up at one page.
	// Parameters:
	//     bSelectBlock - TRUE when selecting rows up to new focused row,
	//                    FALSE otherwise.
	//     bIgnoreSelection - TRUE to ignore the current rows selection.
	// Remarks:
	//     Call this member function if you want to move currently
	//     row up by 1 page (and count of rows visible in a view).
	// See Also: MoveDown, MoveUp, MovePageDown, MoveFirst, MoveLast
	//-----------------------------------------------------------------------
	void MovePageUp(BOOL bSelectBlock, BOOL bIgnoreSelection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row to the beginning.
	// Parameters:
	//     bSelectBlock     - TRUE when selecting rows up to new focused row,
	//                        FALSE otherwise.
	//     bIgnoreSelection - TRUE if ignore current rows selection.
	// Remarks:
	//     Call this member function if you want to move the currently
	//     focused row to the top of the rows list.
	// See Also: MoveDown, MoveUp, MovePageDown, MovePageUp, MoveLast
	//-----------------------------------------------------------------------
	void MoveFirst(BOOL bSelectBlock, BOOL bIgnoreSelection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Moves the focused row to the end.
	// Parameters:
	//     bSelectBlock     - TRUE when selecting rows up to new focused row,
	//                        FALSE otherwise.
	//     bIgnoreSelection - TRUE if ignore current rows selection.
	// Remarks:
	//     Call this member function if you want to move the currently
	//     focused row to the bottom of the rows list.
	// See Also: MoveDown, MoveUp, MovePageDown, MovePageUp, MoveFirst
	//-----------------------------------------------------------------------
	void MoveLast(BOOL bSelectBlock, BOOL bIgnoreSelection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns rows from the visible report area.
	// Parameters:
	//     nStartRow - Row index to start calculating from.
	//     bMoveDown - Rows moving direction.
	// Remarks:
	//     Call this member function if you want to calculate the amount
	//     of currently visible rows between nStartRow and the top/bottom
	//     of the current view.
	// Returns:
	//     Visible row index starting from.
	// See Also: MovePageDown, MovePageUp
	//-----------------------------------------------------------------------
	int GetReportAreaRows(int nStartRow, BOOL bMoveDown);

	//-----------------------------------------------------------------------
	// Summary:
	//     Force provided row to the top.
	// Parameters:
	//     nIndex - An index of the row to force.
	// Remarks:
	//     The system scrolls the report control view until either the
	//     item specified by nIndex appears at the top of the view
	//     the maximum scroll range has been reached.
	// See Also: GetReportAreaRows, EnsureVisible
	//-----------------------------------------------------------------------
	void SetTopRow(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set horizontal scroll position.
	// Parameters:
	//     nOffset - Horizontal scroll position.
	// Remarks:
	//     This method takes effect only if auto column sizing is disable
	// See Also: CXTPReportHeader::SetAutoColumnSizing
	//-----------------------------------------------------------------------
	void SetLeftOffset(int nOffset);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enable/disable preview mode for the control.
	// Parameters:
	//     bIsPreviewMode - TRUE for enabling preview mode,
	//                      FALSE for disabling.
	// Remarks:
	//     Call this member function if you want to hide or show the
	//     row preview item.
	// See Also: IsPreviewMode
	//-----------------------------------------------------------------------
	void EnablePreviewMode(BOOL bIsPreviewMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns current preview mode.
	// Remarks:
	//     Call this member function if you want to determine whether the
	//     row preview item is shown or not.
	// Returns:
	//     TRUE when preview mode is enabled,
	//     FALSE when preview mode is disabled.
	// See Also: EnablePreviewMode
	//-----------------------------------------------------------------------
	BOOL IsPreviewMode();

	//-----------------------------------------------------------------------
	// Summary:
	//     Shows/hides Group By control area.
	// Parameters:
	//     bEnable - TRUE for showing GroupBy area,
	//               FALSE for hiding GroupBy area.
	// Remarks:
	//     Call this member function if you want to hide or show
	//     Group By area in the report header.
	// See Also: IsGroupByVisible
	//-----------------------------------------------------------------------
	void ShowGroupBy(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns current Group By area mode.
	// Remarks:
	//     Call this member function if you want to determine whether
	//     Group By area in the report header is visible or not.
	// Returns:
	//     TRUE when Group By area is shown on the control,
	//     when Group By area is hidden.
	// See Also: ShowGroupBy
	//-----------------------------------------------------------------------
	BOOL IsGroupByVisible();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to hide/show column headers.
	// Parameters:
	//     bShow - TRUE is column headers will be displayed, FALSE to hide column headers.
	// See Also: IsHeaderVisible
	//-----------------------------------------------------------------------
	void ShowHeader(BOOL bShow = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine is column headers are currently visible.
	// Returns:
	//     TRUE is column headers are visible, FALSE is column headers are hidden.
	// See Also: ShowHeader
	//-----------------------------------------------------------------------
	BOOL IsHeaderVisible();

	//-----------------------------------------------------------------------
	// Summary:
	//     Enable/disable group shade.
	// Parameters:
	//     bEnable - TRUE for enabling group items shade,
	//               FALSE for disabling.
	// Remarks:
	//     Call this member function if you want to hide or show
	//     group rows' headings.
	// See Also: IsShadeGroupHeadingsEnabled
	//-----------------------------------------------------------------------
	void ShadeGroupHeadings(BOOL bEnable = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns current group shade mode.
	// Remarks:
	//     Call this member function if you want to determine whether
	//     group rows' headings are enabled or not.
	// Returns:
	//     TRUE when group items shading is enabled,
	//     FALSE when it is disabled.
	// See Also: ShadeGroupHeadings
	//-----------------------------------------------------------------------
	BOOL IsShadeGroupHeadingsEnabled();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to remove all items and column in specified position.
	// Parameters:
	//     nIndex - Index of item to remove.
	//-----------------------------------------------------------------------
	void ReleaseItem(int nIndex);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get a pointer to the image manager of Report Control.
	// Returns:
	//     Pointer to the image manager of Report Control.
	// See Also: SetImageManager
	//-----------------------------------------------------------------------
	CXTPImageManager* GetImageManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set new image manager.
	// Parameters:
	//     pImageManager - Image manager to be set
	// Example:
	// <code>
	// CXTPImageManager* pImageManager = new CXTPImageManager();
	// pImageManager->SetIcons(IDR_MAINFRAME);
	// m_wndReport.SetImageManager(pImageManager);
	// </code>
	// See Also: GetImageManager
	//-----------------------------------------------------------------------
	void SetImageManager(CXTPImageManager* pImageManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Initiates ImageList of Paint Manager.
	// Parameters:
	//     pImageList - Image list.
	// Remarks:
	//    You use this function to set up your own ImageList
	//    with set bitmaps that represent various states of rows
	//    and depict any other information.
	// Note:
	//    Recommended to use SetImageManager/GetImageManager methods instead.
	// Example:
	// <code>
	// CImageList listIcons;
	// listIcons.Create(16,16, ILC_COLOR24|ILC_MASK, 0, 1));
	// CBitmap bmp;
	// // load bitmap by id
	// bmp.LoadBitmap(IDB_BMREPORT);
	// ilIcons.Add(&bmp, RGB(255, 0, 255));
	// m_wndReport.SetImageList(&lIcons);
	// </code>
	// See Also: GetImageManager
	//-----------------------------------------------------------------------
	void SetImageList(CImageList* pImageList);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Returns a pointer to the associated report header object.
	// Remarks:
	//     Call this member function if you want to retrieve access
	//     to the report header object properties and methods.
	// Returns:
	//     A pointer to the associated report header.
	// See Also: CXTPReportHeader overview
	//-----------------------------------------------------------------------
	CXTPReportHeader* GetReportHeader();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns indentation for the tree view row of the specified depth level.
	// Parameters:
	//     nLevel - Tree depth level.
	// Remarks:
	//     Calculates row indentation in pixels based on the provided
	//     indentation level.
	// Returns:
	//     Row indentation in pixels.
	// See Also: Populate
	//-----------------------------------------------------------------------
	int GetIndent(int nLevel);

	//-----------------------------------------------------------------------
	// Summary:
	//     Notifies parent control of some event that has happened.
	// Parameters:
	//     pRow       - Specified row of event if used.
	//     pItem      - Specified item of event if used.
	//     pColumn    - Specified column of event if used.
	//     nMessage   - A message to sent to parent window.
	//     pPoint     - A point where the message was sent from in
	//                        client coordinates.
	//     nHyperlink - Hyperlink order number, where the message was sent
	//                  from (-1 if message was not send from the hyperlink).
	// Remarks:
	//     Sends a message to the parent in the form of a WM_NOTIFY message
	//     with a specific structure attached.
	// Returns:
	//     The result of the message processing;
	//     its value depends on the message sent. (see CWnd::SendMessage)
	// See Also: CXTPReportControl overview, SendNotifyMessage
	//-----------------------------------------------------------------------
	LRESULT SendMessageToParent(CXTPReportRow* pRow, CXTPReportRecordItem* pItem, CXTPReportColumn* pColumn, UINT nMessage, CPoint* pPoint, int nHyperlink = -1) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sends the specified message to the window.
	// Parameters:
	//     nMessage - The message to be sent.
	//     pNMHDR - Notify header
	// Returns:
	//     Nonzero if successful; otherwise returns zero.
	//-----------------------------------------------------------------------
	LRESULT SendNotifyMessage(UINT nMessage, NMHDR* pNMHDR = NULL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Collapses all rows.
	// Remarks:
	//     The CollapseAll method collapses all the CXTPReportRow objects,
	//     including all the child rows, that are in the report control.
	// See Also: ExpandAll, CXTPReportRow::SetExpanded
	//-----------------------------------------------------------------------
	void CollapseAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Expands all rows.
	// Remarks:
	//     The ExpandAll method expands all the CXTPReportRow objects,
	//     including all the child rows, that are in the report control.
	// See Also: CollapseAll, CXTPReportRow::SetExpanded
	//-----------------------------------------------------------------------
	void ExpandAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the report allows multiple selections.
	// Returns:
	//     TRUE if the report allows multiple selections.
	// See Also: SetMultipleSelection, GetSelectedRows
	//-----------------------------------------------------------------------
	BOOL IsMultipleSelection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Changes allowing multiple selections for the control.
	// Parameters:
	//     bMultipleSelection - TRUE for enabling, FALSE for disabling.
	// Remarks:
	//     Sets the flag that determines whether the report allows multiple selections.
	// See Also: IsMultipleSelection, GetSelectedRows
	//-----------------------------------------------------------------------
	void SetMultipleSelection(BOOL bMultipleSelection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables/disables showing tooltips for the control.
	// Parameters:
	//     bEnable - TRUE for enabling, FALSE for disabling.
	// Remarks:
	//     Call this member function to enable or disable tooltips
	//     show for the report control window.
	//-----------------------------------------------------------------------
	void EnableToolTips(BOOL bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether groups are skipped when navigating
	//     the ReportControl with the Up and Down keys.
	// Parameters:
	//     bSkipFocus - If TRUE, when navigating the rows with the Up and Down
	//                  keys in the ReportControl the group headings will be skipped
	//                  and the next non-group heading row will be selected.
	//                  If FALSE, when navigating the rows with the Up and Down
	//                  keys in the ReportControlall rows will be selected,
	//                  even group headings.
	//-----------------------------------------------------------------------
	void SkipGroupsFocus(BOOL bSkipFocus);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine is groups are skipped when navigating the
	//     with the Up and Down arrow keys.
	// Returns:
	//     TRUE if groups are skipped, FALSE is groups receive focus when navigating
	//     the report control with the Up and Down arrow keys.
	//-----------------------------------------------------------------------
	BOOL IsSkipGroupsFocusEnabled();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve indentation of the header
	// Returns:
	//     Header indent.
	//-----------------------------------------------------------------------
	int GetHeaderIndent();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve Report bounding rectangle.
	// Returns:
	//     Report bounding rectangle.
	//-----------------------------------------------------------------------
	CRect GetReportRectangle();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts scroll bars depending on currently visible rows.
	//-----------------------------------------------------------------------
	void AdjustScrollBars();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve a pointer to the currently selected column.
	// Returns:
	//     A pointer to the currently selected item.
	//-----------------------------------------------------------------------
	CXTPReportColumn* GetFocusedColumn();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set selected column.
	// Parameters:
	//     pColumn - column to be selected.
	//-----------------------------------------------------------------------
	void SetFocusedColumn(CXTPReportColumn* pColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether each individual CXTPReportRecordItem
	//     will show focus when the item in a row is clicked.
	// Parameters:
	//     bFocusSubItems - If TRUE, when a ReportRecordItem is clicked, the
	//                      entire row will become highlighted except the individual
	//                      item that was clicked.
	//                      If FALSE, the entire row will become highlighted when
	//                      an item is clicked, including the item that was clicked.
	// See Also: IsFocusSubItems
	//-----------------------------------------------------------------------
	void FocusSubItems(BOOL bFocusSubItems);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if individual CXTPReportRecordItem(s) will
	//     receive focus when that item in the CXTPReportRow that they belong to is clicked.
	// Returns:
	//     TRUE if individual items can receive focus, FALSE if only the entire row can receive focus.
	// See Also: FocusSubItems
	//-----------------------------------------------------------------------
	BOOL IsFocusSubItems();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the CXTPReportRecordItem(s)are editable.
	// Returns:
	//     TRUE is the CXTPReportRecordItem(s) are editable, FALSE otherwise.
	// See Also: AllowEdit, EditOnClick, IsEditOnClick
	//-----------------------------------------------------------------------
	BOOL IsAllowEdit();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to allow the text in all CXTPReportRecordItem(s) to be edited.  This will
	//     add an edit box to the item where the text can be edited.
	// Parameters:
	//     bAllowEdit - TRUE to add an edit box to the CXTPReportRecordItem(s) so they are editable.
	//                  FALSE to remove the edit box and not allow them to be edited.
	// See Also: IsAllowEdit, EditOnClick, IsEditOnClick
	//-----------------------------------------------------------------------
	void AllowEdit(BOOL bAllowEdit);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify whether a CXTPReportRecordItem can be edited by
	//     single-clicking on the item.
	// Parameters:
	//     bEditOnClick - If TRUE, when the CXTPReportRecordItem is single-clicked,
	//                    the item will become editable.  The entire ReportControl
	//                    or the specific CXTPReportRecordItem must have the bAllowEdit
	//                    property set to TRUE for this to work.
	//                    If FALSE, the item must be double-clicked to become editable.
	// See Also: AllowEdit, IsAllowEdit, IsEditOnClick
	//-----------------------------------------------------------------------
	void EditOnClick(BOOL bEditOnClick);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine whether items are edited with a single-click.
	// Returns:
	//     TRUE it items can be edited with a single-click, FALSE is a
	//     double-click is needed to edit items.
	// See Also: AllowEdit, IsAllowEdit, EditOnClick
	//-----------------------------------------------------------------------
	BOOL IsEditOnClick();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when user start edit item.
	// Parameters:
	//     pItemArgs - Arguments of item to be edit.
	// Remarks:
	//     Call this method with NULL as parameter to stop item edit.
	//-----------------------------------------------------------------------
	void EditItem(XTP_REPORTRECORDITEM_ARGS* pItemArgs);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves in-place edit pointer
	// Returns:in-place
	//     Pointer to in-place edit control
	//-----------------------------------------------------------------------
	CXTPReportInplaceEdit* GetInplaceEdit();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves in-place buttons of report control
	// Returns:
	//     Collection of in-place buttons.
	//-----------------------------------------------------------------------
	CXTPReportInplaceButtons* GetInplaceButtons();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves in-place list of report controls
	// Returns:
	//     Pointer to in-place list.
	//-----------------------------------------------------------------------
	CXTPReportInplaceList* GetInplaceList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine if the ReportControl has focus.
	// Returns:
	//     TRUE if the ReportControl has focus, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL HasFocus();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the currently active item, which
	//     is the item that has focus.
	// Returns:
	//     A pointer to the currently active(focused) CXTPReportRecordItem.
	//-----------------------------------------------------------------------
	CXTPReportRecordItem* GetActiveItem();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specifies the indentation placed before the
	//     text of each child node in a hierarchical tree structure.
	// Parameters:
	//     nIndent - Indentation used when displaying child nodes.
	//-----------------------------------------------------------------------
	void SetTreeIndent(int nIndent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable virtual mode of the control
	// Parameters:
	//     pVirtualRecord - record to be used as virtual for all rows.
	//     nCount - Count of virtual records.
	// Example:
	// <code>
	// class CVirtualRecord : public CXTPReportRecord
	// {
	// public:
	//     CVirtualRecord()
	//     {
	//         AddItem(new CXTPReportRecordItem());
	//         AddItem(new CXTPReportRecordItem());
	//         AddItem(new CXTPReportRecordItem());
	//     }
	//
	//     void GetItemMetrics (XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics)
	//     {
	//         // Draw virtual record
	//     }
	// }
	// ...
	// m_wndReport.SetVirtualMode(new CVirtualRecord(), 540);
	// </code>
	// See Also: IsVirtualMode
	//-----------------------------------------------------------------------
	void SetVirtualMode(CXTPReportRecord* pVirtualRecord, int nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if control in virtual mode
	// Returns;
	//     TRUE if virtual mode is enabled; FALSE otherwise.
	// See Also: IsVirtualMode
	//-----------------------------------------------------------------------
	BOOL IsVirtualMode();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the text from the associated filter edit control.
	// Returns:
	//     Text string entered by user inside filter edit control.
	// See Also: SetFilterText
	//-----------------------------------------------------------------------
	virtual CString GetFilterText();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set new filter for control.
	// Parameters:
	//     strFilterText - Filter text to be applyed for control.
	// Remarks:
	//     You must call Populate method to update rows.
	// See Also: GetFilterText, Populate
	//-----------------------------------------------------------------------
	void SetFilterText(CString strFilterText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Register the window class if it has not already been registered.
	// Returns:
	//     TRUE if the window class was successfully registered.
	//-----------------------------------------------------------------------
	BOOL RegisterWindowClass();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Sets new operational control mouse mode.
	// Parameters:
	//     nMode - New mouse mode. For available values, see XTPReportMouseMode enum.
	// See Also: XTPReportMouseMode overview, GetMouseMode
	//-----------------------------------------------------------------------
	void SetMouseMode(XTPReportMouseMode nMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the current control mouse mode.
	// Returns:
	//     Current control mouse mode.
	//     For available values, see XTPReportMouseMode enum.
	// See Also: XTPReportMouseMode overview, SetMouseMode
	//-----------------------------------------------------------------------
	XTPReportMouseMode GetMouseMode() const;

	//-----------------------------------------------------------------------
	// <COMBINE CXTPReportControl::SetFocusedRow@CXTPReportRow*@BOOL>
	//-----------------------------------------------------------------------
	void SetFocusedRow(CXTPReportRow* pRow, BOOL bSelectBlock, BOOL bIgnoreSelection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Performs all drawing logic.
	// Parameters:
	//     pDC - Provided DC to draw control image with.
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws all rows on the provided DC.
	// Parameters:
	//     pDC - Provided DC to draw rows image with.
	//     rcClient - A rectangle to draw rows image into.
	//-----------------------------------------------------------------------
	void DrawRows(CDC* pDC, CRect& rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns changed status flag of the control. Used for caching control image drawing.
	// Returns:
	//     TRUE if the internal control state was changed since last drawing, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets new changed status flag for the control. Used for caching control image drawing.
	// Parameters:
	//     bChanged - TRUE when something was changed in the control contents and control needs to be republished.
	//-----------------------------------------------------------------------
	void SetChanged(BOOL bChanged = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sorts rows corresponding to the sort order taken from columns.
	// Parameters:
	//     pRows - A rows collection to sort.
	//-----------------------------------------------------------------------
	void SortRows(CXTPReportRows* pRows);

	//-----------------------------------------------------------------------
	// Summary:
	//     Update field chooser control with its content.
	//-----------------------------------------------------------------------
	void UpdateSubList();

	//-----------------------------------------------------------------------
	// Summary:
	//     Builds rows tree based on provided data record.
	// Parameters:
	//     pTree      - Rows tree to add items into.
	//     pParentRow - Parent tree row.
	//     pRecords   - Records collection for transferring to rows.
	// Remarks:
	//     Builds rows tree based on provided data record.
	//     Recursively calls itself when build nested branches of rows
	//-----------------------------------------------------------------------
	void BuildTree(CXTPReportRows* pTree, CXTPReportRow* pParentRow, CXTPReportRecords* pRecords);

	//-----------------------------------------------------------------------
	// Summary:
	//     Collapses all children of the specified row.
	// Parameters:
	//     pRow - A row to collapse.
	//-----------------------------------------------------------------------
	void _DoCollapse(CXTPReportRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Expands all children of the specified row.
	// Parameters:
	//     nIndex - An index to insert rows from.
	//     pRow   - A row to expand.
	// Returns:
	//     A count of the newly added rows or void.
	//-----------------------------------------------------------------------
	int _DoExpand(int nIndex, CXTPReportRow* pRow);
	void _DoExpand(CXTPReportRow* pRow); // <COMBINE CXTPReportControl::_DoExpand@int@CXTPReportRow*>

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts the specified row at the specified position.
	// Parameters:
	//     nIndex - A position to insert row at.
	//     pRow   - A row to insert.
	// Remarks:
	//     Inserts the specified row to the rows array at the specified position
	//     with all its children expanded. Use _DoExpand() to expand
	//     all child items
	// Returns:
	//     A number of the inserted rows.
	//-----------------------------------------------------------------------
	int InsertRow(int nIndex, CXTPReportRow* pRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Recalculates indexes of all rows in the specified collection.
	//-----------------------------------------------------------------------
	void _RefreshIndexes();

	//-----------------------------------------------------------------------
	// Summary:
	//     Checks a record for filter text.
	// Parameters:
	//     pRecord         - A record to apply filter to.
	//     strFilterText   - Filter string text.
	//     bIncludePreview - Include preview item in filtering or not.
	// Remarks:
	//     This member function represents filter functionality. First, it parses
	//     the input text of a filter string by tokens; second it enumerates all visible
	//     columns to find text matching to the filter string. Returns TRUE if
	//     matching found, FALSE otherwise
	// Returns:
	//     TRUE if record is filtered with the specified filter,
	//     FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL ApplyFilter(CXTPReportRecord* pRecord, CString strFilterText, BOOL bIncludePreview);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called when a user selects a row
	// Remarks:
	//     When user selects row ReportControl, it sends notification to the
	//     parent window by calling OnSelectionChanged()
	// See Also: XTP_NM_SELCHANGED, SendNotifyMessage()
	//-----------------------------------------------------------------------
	virtual void OnSelectionChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns drawing metrics for the row.
	// Parameters:
	//     pDrawArgs    - Pointer to the provided draw arguments structure for calculating metrics.
	//     pItemMetrics - Pointer to the metrics structure to fill.
	//-----------------------------------------------------------------------
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS*, XTP_REPORTRECORDITEM_METRICS* ) {
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to create new CXTPReportRow object. Overwrite it
	//     to use derived CXTPReportRow class.
	// Returns:
	//     New CXTPReportRow object.
	//-----------------------------------------------------------------------
	virtual CXTPReportRow* CreateRow();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to create new CXTPReportGroupRow object. Overwrite it
	//     to use derived CXTPReportGroupRow class.
	// Returns:
	//     New CXTPReportGroupRow object.
	//-----------------------------------------------------------------------
	virtual CXTPReportGroupRow* CreateGroupRow();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when user start drag row
	// Parameters:
	//     point - Drag point
	// Remarks:
	//     Use can override this method or catch LVN_BEGINDRAG message to proceed drag
	//     operations.
	//-----------------------------------------------------------------------
	virtual void OnBeginDrag(CPoint point);


protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Adjust control indentation properties depending on current tree depth.
	// See Also: GetIndent
	//-----------------------------------------------------------------------
	void AdjustIndentation();

	//-----------------------------------------------------------------------
	// Summary:
	//     Adjusts main control areas depending on current control size.
	//-----------------------------------------------------------------------
	void AdjustLayout();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPReportControl)
	virtual void PreSubclassWindow();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPReportControl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) ;
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint ptDblClick);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnSysColorChange();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus (CWnd* pNewWnd);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnChar(UINT nChar, UINT nRepCntr, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	static UINT GetMouseScrollLines();
	void RelayToolTipEvent(UINT message);
	CXTPReportColumn* GetNextFocusableColumn(CXTPReportRow* pRow, int nColumnIndex, int nDirection);
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:
	BOOL m_bSubclassFromCreate;     // For identifying where the window is sub-classed.

	BOOL m_bChanged;                // Internal member for storing changed flag.
	BOOL m_bRefreshIndexes;         // TRUE when it is required to refresh items indexes.

	int m_nLockUpdateCount;         // A counter of the update locks. An image will be redrawn only when the lock counter is equal to zero.

	CRect m_rcGroupByArea;          // The area occupied by Group By item.
	CRect m_rcHeaderArea;           // The area occupied by the header.
	CRect m_rcReportArea;           // The area occupied by rows.

	CXTPReportRows* m_pRows;        // Virtual list rows container. Used for changing order, etc.
	CXTPReportRows* m_pPlainTree;   // Tree rows container.
	CXTPReportRecords* m_pRecords;  // List records container.
	CXTPReportColumns* m_pColumns;  // List columns container.

	CXTPReportPaintManager* m_pPaintManager;    // Paint manager.
	int m_nTopRow;                              // Current top row in the visible area.
	int m_nLeftOffset;                          // Scroll position

	int m_nFocusedRow;                          // Pointer to the currently focused row.
	CXTPReportSelectedRows* m_pSelectedRows;    // Container for the currently selected rows.

	CXTPReportRow* m_pHotRow;                   // Hot row

	int m_nRowsPerWheel;                        // Amount of rows to scroll by mouse wheel.

	CXTPReportTip m_wndTip;                     // Tip window.
	CBitmap m_bmpCache;                         // Cached window bitmap.

	BOOL m_bGroupByEnabled;                     // TRUE if Group By box is enabled

	XTPReportMouseMode m_mouseMode;             // Current mouse operation mode
	BOOL m_bMultipleSelection;                  // TRUE if multiple selection enabled.
	BOOL m_bShowTooltips;                       // TRUE if showing tooltips enabled.

	CXTPImageManager* m_pImageManager;          // Contains image list for report control
	BOOL m_bSkipGroupsFocus;                    // TRUE if group rows are skipped when navigating rows with the Up and Down arrow keys

	BOOL m_bFocusSubItems;                      // TRUE if sub-items can receive focus.
	BOOL m_bEditOnClick;                        // TRUE if sub-items become editable on a single-click
	BOOL m_bAllowEdit;                          // TRUE if sub-items can be edited.
	BOOL m_bHeaderVisible;                      // TRUE if column headers are visible.
	CXTPReportColumn* m_pFocusedColumn;         // Pointer to the currently focused CXTPReportColumn.

	CXTPReportRecordItem* m_pActiveItem;        // Pointer to the currently focused CXTPReportRecordItem.
	CXTPReportInplaceEdit* m_pInplaceEdit;      // In-place edit pointer
	CXTPReportInplaceButtons* m_pInplaceButtons;// In-place buttons pointer
	CXTPReportInplaceList* m_pInplaceList;      // In-place list pointer
	BOOL m_bVScrollBarVisible;                  // TRUE if vertical scroll bar is visible
	CPoint m_pointDrag;                         // Drag position
	BOOL m_bPrepareDrag;                        // TRUE if user click the report control and doesn't release button.
	CXTPReportRows m_arrScreenRows;             // Rows currently presented on screen.
	CString m_strFilterText;                    // Filter text.

	friend class CReportControlCtrl;
};

//{{AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     Internal report update helper.
//===========================================================================
class CXTPReportControlUpdateContext
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReportControlUpdateContext object.
	// Parameters:
	//     pControl - Pointer to a Report Control object.
	//-----------------------------------------------------------------------
	CXTPReportControlUpdateContext(CXTPReportControl* pControl)
	{
		m_pControl = pControl;
		pControl->BeginUpdate();
	}

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPReportControlUpdateContext object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	~CXTPReportControlUpdateContext()
	{
		m_pControl->EndUpdate();
	}
protected:
	CXTPReportControl* m_pControl;
};
//}}AFX_CODEJOCK_PRIVATE

//////////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPReportPaintManager* CXTPReportControl::GetPaintManager() {
	return m_pPaintManager;
}
AFX_INLINE CXTPReportSelectedRows* CXTPReportControl::GetSelectedRows() {
	return m_pSelectedRows;
}
AFX_INLINE CXTPReportRows* CXTPReportControl::GetRows() {
	return m_pRows;
}

AFX_INLINE CXTPReportRecords* CXTPReportControl::GetRecords() {
	return m_pRecords;
}
AFX_INLINE CXTPReportColumns* CXTPReportControl::GetColumns() {
	return m_pColumns;
}
AFX_INLINE XTPReportMouseMode CXTPReportControl::GetMouseMode() const {
	return m_mouseMode;
}
AFX_INLINE void CXTPReportControl::SetGridStyle(BOOL bVertical, XTPReportGridStyle gridStyle) {
	m_pPaintManager->SetGridStyle(bVertical, gridStyle);
	AdjustScrollBars();
}
AFX_INLINE XTPReportGridStyle CXTPReportControl::GetGridStyle(BOOL bVertical) {
	return bVertical? m_pPaintManager->m_verticalGridStyle: m_pPaintManager->m_horizontalGridStyle;
}
AFX_INLINE COLORREF CXTPReportControl::SetGridColor(COLORREF clrGridLine) {
	return m_pPaintManager->SetGridColor(clrGridLine);
}
AFX_INLINE void CXTPReportControl::EnablePreviewMode(BOOL bIsPreviewMode) {
	m_pPaintManager->EnablePreviewMode(bIsPreviewMode);
}
AFX_INLINE BOOL CXTPReportControl::IsPreviewMode(){
	return m_pPaintManager->IsPreviewMode();
}
AFX_INLINE BOOL CXTPReportControl::IsChanged() {
	return m_bChanged;
}
AFX_INLINE void CXTPReportControl::SetChanged(BOOL bChanged) {
	m_bChanged = bChanged;
}
AFX_INLINE void CXTPReportControl::ShowGroupBy(BOOL bEnable) {
	ASSERT(!IsVirtualMode());
	m_bGroupByEnabled = bEnable;
	AdjustLayout();
	AdjustScrollBars();
}
AFX_INLINE BOOL CXTPReportControl::IsGroupByVisible() {
	return m_bGroupByEnabled;
}

AFX_INLINE void CXTPReportControl::ShowHeader(BOOL bShow /*= TRUE*/) {
	m_bHeaderVisible = bShow;
	AdjustLayout();
	AdjustScrollBars();
}

AFX_INLINE BOOL CXTPReportControl::IsHeaderVisible() {
	return m_bHeaderVisible;
}

AFX_INLINE void CXTPReportControl::ShadeGroupHeadings(BOOL bEnable) {
	if (m_pPaintManager)
		m_pPaintManager->m_bShadeGroupHeadings = bEnable;
	AdjustScrollBars();
}

AFX_INLINE BOOL CXTPReportControl::IsShadeGroupHeadingsEnabled() {
	return m_pPaintManager ? m_pPaintManager->m_bShadeGroupHeadings : FALSE;
}
AFX_INLINE CRect CXTPReportControl::GetReportRectangle() {
	return m_rcReportArea;
}
AFX_INLINE BOOL CXTPReportControl::IsMultipleSelection() {
	return m_bMultipleSelection;
}
AFX_INLINE void CXTPReportControl::SetMultipleSelection(BOOL bMultipleSelection) {
	m_bMultipleSelection = bMultipleSelection;
	SetFocusedRow(GetFocusedRow());
}
AFX_INLINE void CXTPReportControl::EnableToolTips(BOOL bEnable) {
	m_bShowTooltips = bEnable;
}
AFX_INLINE void CXTPReportControl::SkipGroupsFocus(BOOL bSkipFocus) {
	m_bSkipGroupsFocus = bSkipFocus;
}
AFX_INLINE BOOL CXTPReportControl::IsSkipGroupsFocusEnabled() {
	return m_bSkipGroupsFocus;
}
AFX_INLINE CXTPImageManager* CXTPReportControl::GetImageManager() {
	return m_pImageManager;
}
AFX_INLINE CXTPReportColumn* CXTPReportControl::GetFocusedColumn() {
	return m_pFocusedColumn;
}
AFX_INLINE void CXTPReportControl::FocusSubItems(BOOL bFocusSubItems) {
	m_bFocusSubItems = bFocusSubItems;
	m_pFocusedColumn = NULL;
}
AFX_INLINE BOOL CXTPReportControl::IsAllowEdit() {
	return m_bAllowEdit;
}
AFX_INLINE CXTPReportInplaceEdit* CXTPReportControl::GetInplaceEdit() {
	return m_pInplaceEdit;
}
AFX_INLINE CXTPReportInplaceButtons* CXTPReportControl::GetInplaceButtons() {
	return m_pInplaceButtons;
}
AFX_INLINE CXTPReportInplaceList* CXTPReportControl::GetInplaceList() {
	return m_pInplaceList;
}
AFX_INLINE CXTPReportRecordItem* CXTPReportControl::GetActiveItem() {
	return m_pActiveItem;
}
AFX_INLINE void CXTPReportControl::AllowEdit(BOOL bAllowEdit) {
	m_bAllowEdit = bAllowEdit;
}
AFX_INLINE BOOL CXTPReportControl::IsFocusSubItems() {
	return m_bFocusSubItems;
}
AFX_INLINE void CXTPReportControl::EditOnClick(BOOL bEditOnClick) {
	m_bEditOnClick = bEditOnClick;
}
AFX_INLINE BOOL CXTPReportControl::IsEditOnClick() {
	return m_bEditOnClick;
}
AFX_INLINE BOOL CXTPReportControl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE CString CXTPReportControl::GetFilterText() {
	return m_strFilterText;
}
AFX_INLINE void CXTPReportControl::SetFilterText(CString strFilterText) {
	m_strFilterText = strFilterText;
}

#endif //#if !defined(__XTPREPORTCONTROL_H__)
