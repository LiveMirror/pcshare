// XTPPropertyGridDefines.h
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
#if !defined(__XTPPROPERTYGRIDDEFINES_H__)
#define __XTPPROPERTYGRIDDEFINES_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-----------------------------------------------------------------------
// Summary:
//     The XTWM_PROPERTYGRID_NOTIFY message is sent to the CXTPPropertyGrid owner window
//     whenever an action occurs within the CXTPPropertyGrid
//
// <code>XTWM_PROPERTYGRID_NOTIFY
// nGridAction = (int) wParam;              // Property grid action
// pItem = (CXTPPropertyGridItem*) lParam;  // pointer to a CXTPPropertyGridItem object</code>
//
//     When the user performs an action in the property grid, the XTWM_PROPERTYGRID_NOTIFY message is
//     sent to the property grid's owner window.
// Parameters:
//     nGridAction -  Value of wParam specifies an value that indicates the user's
//                    request .
//     pItem - The value of lParam points to a CXTPPropertyGridItem object that contains information for the
//             specified item. This pointer should <b>never</b> be NULL.
// Returns:
//     If the application is to process this message, the return value should be TRUE, otherwise the
//     return value is FALSE.
// Remarks:
//     nGridAction parameter can be one of the following values:
//          * <b>XTP_PGN_SORTORDER_CHANGED</b> The sort order has changed in the property grid.
//          * <b>XTP_PGN_SELECTION_CHANGED</b> The selection has changed in the property grid.
//          * <b>XTP_PGN_ITEMVALUE_CHANGED</b> The value has changed for pItem in the property grid.
//          * <b>XTP_PGN_EDIT_CHANGED</b> The edit value has changed in the property grid.
//          * <b>XTP_PGN_INPLACEBUTTONDOWN</b> Item's in-place button (combo or expand) down.
//          * <b>XTP_PGN_ITEMEXPANDCHANGED</b> User expand or collapse item.
//          * <b>XTP_PGN_DBLCLICK</b> The user double-clicks the left mouse button in the property grid.
//          * <b>XTP_PGN_RCLICK</b> The user pressed the right mouse button in the property grid.
//          * <b>XTP_PGN_VERB_CLICK</b> The user click verb in the property grid.
//
// Example:
//    Here is an example of how an application would process the XTWM_PROPERTYGRID_NOTIFY
//    message.
//
// <code>
// int nGridAction = (int)wParam;
//
// // Cast the lParam to an CXTPPropertyGridItem* object pointer.
// CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
// ASSERT(pItem);
//
// switch (nGridAction)
// {
//     case XTP_PGN_SORTORDER_CHANGED:
//           {
//              m_nSort = m_wndXTPPropertyGrid.GetPropertySort();
//              UpdateData(FALSE);
//           }
//         break;
//     case XTP_PGN_ITEMVALUE_CHANGED:
//           {
//              TRACE(_T("Value Changed. Caption = %s, ID = %i, Value = %s\n"),
//                  pItem->GetCaption(), pItem->GetID(), pItem->GetValue());
//           }
//         break;
//     case XTP_PGN_SELECTION_CHANGED:
//           {
//              TRACE(_T("Selection Changed. Item = %s\n"), pItem->GetCaption());
//           }
//         break;
// }
// return FALSE;
// </code>
//-----------------------------------------------------------------------
const UINT XTPWM_PROPERTYGRID_NOTIFY = (WM_APP + 2533);


const UINT XTP_PGN_SORTORDER_CHANGED  = 1;  //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_SELECTION_CHANGED  = 2;  //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_ITEMVALUE_CHANGED  = 3;  //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_EDIT_CHANGED      = 4;   //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_INPLACEBUTTONDOWN = 5;   //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_DRAWITEM = 6;            //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_ITEMEXPANDCHANGED = 7;   //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_DBLCLICK = 8;            //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_RCLICK = 9;              //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>
const UINT XTP_PGN_VERB_CLICK = 10;         //<ALIAS XTPWM_PROPERTYGRID_NOTIFY>

//-----------------------------------------------------------------------
// Summary:
//     Apply this style to the property grid to use the owner draw feature.
// See Also:
//     CXTPPropertyGrid::Create
// Example:
//     The following example illustrates using XTP_PGS_OWNERDRAW:
// <code>
// m_wndPropertyGrid.ModifyStyle(0, XTP_PGS_OWNERDRAW);
// </code>
//-----------------------------------------------------------------------
const UINT XTP_PGS_OWNERDRAW = 0x0010L;


#endif //#define __XTPPROPERTYGRIDDEFINES_H__
