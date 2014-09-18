// CXTPReportDefines.h
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
#if !defined(__XTPREPORTDEFINES_H__)
#define __XTPREPORTDEFINES_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-----------------------------------------------------------------------
// Summary:
//     NULL terminated string used by CXTPReportControl which specifies
//     the window class name for the report control.
// Example:
// <code>
// BOOL CXTPReportControl::RegisterWindowClass()
// {
//     WNDCLASS wndcls;
//     HINSTANCE hInst = AfxGetInstanceHandle();
//
//     if (!(::GetClassInfo(hInst, XTPREPORTCTRL_CLASSNAME, &wndcls)))
//     {
//         // otherwise we need to register a new class
//         wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
//         wndcls.lpfnWndProc      = ::DefWindowProc;
//         wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
//         wndcls.hInstance        = hInst;
//         wndcls.hIcon            = NULL;
//         wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
//         wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
//         wndcls.lpszMenuName     = NULL;
//         wndcls.lpszClassName    = XTPREPORTCTRL_CLASSNAME;
//
//         if (!AfxRegisterClass(&wndcls))
//         {
//             AfxThrowResourceException();
//             return FALSE;
//         }
//     }
//
//     return TRUE;
// }
// </code>
// See Also:
//     CXTPReportControl::RegisterWindowClass
//-----------------------------------------------------------------------
const TCHAR XTPREPORTCTRL_CLASSNAME[] = _T("XTPReport");

//-----------------------------------------------------------------------
// Summary:
//     This constant indicates that a COLORREF is not defined.
// Example:
// <code>
// void CXTPReportRecordItem::GetItemMetrics(
//     XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
//     XTP_REPORTRECORDITEM_METRICS* pItemMetrics)
// {
//     if (m_clrBackground != XTP_REPORT_COLOR_DEFAULT)
//         pItemMetrics->clrBackground = m_clrBackground;
//
//     if (m_clrText != XTP_REPORT_COLOR_DEFAULT)
//         pItemMetrics->clrForeground = m_clrText;
//
//     if (m_pFontCaption != NULL)
//         pItemMetrics->pFont = m_pFontCaption;
//
//     else if (m_bBoldText)
//         pItemMetrics->pFont =
//             &pDrawArgs->pControl->GetPaintManager()->m_fontBoldText;
// }
// </code>
// See Also:
//    CXTPReportRecordItem, CXTPReportRow
//-----------------------------------------------------------------------
const COLORREF XTP_REPORT_COLOR_DEFAULT = (COLORREF)-1;

//-----------------------------------------------------------------------
// Summary:
//     This constant indicates that an icon ID is not defined.
// Example:
// <code>
// m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_PRICE, _T("Price"),
//     80, TRUE, XTP_REPORT_NOICON, TRUE, FALSE));
// </code>
// See Also:
//    CXTPReportColumn
//-----------------------------------------------------------------------
const int XTP_REPORT_NOICON = (int)-1;

//-----------------------------------------------------------------------
// Summary:
//     Defines message for header context menu
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_HEADER_RCLICK notification message is sent to inform
//     the owner window that the user has right clicked column of report control
//     The owner window of the color picker receives this notification
//     threw the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_HEADER_RCLICK, XTP_ID_REPORT_CONTROL, OnReportColumnRClick)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportColumnRClick(NMHDR * pNotifyStruct, LRESULT * /*result*/)
// {
//     XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
//     ASSERT(pItemNotify->pColumn);
//     CPoint ptClick = pItemNotify->pt;
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_COLUMNORDERCHANGED,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_NM_REPORT_SELCHANGED,
//     XTP_NM_REPORT_SORTORDERCHANGED, XTP_NM_REPORT_VALUECHANGED,
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_HEADER_RCLICK         (NM_FIRST-51)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for handling rows selection changed event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_SELCHANGED notification message is sent to inform
//     the owner window that the user select row of report control
//     The owner window of the color picker receives this notification
//     threw the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_SELCHANGED, XTP_ID_REPORT_CONTROL, OnReportSelChanged)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportSelChanged(NMHDR * pNotifyStruct, LRESULT * /*result*/)
// {
//     XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_COLUMNORDERCHANGED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_INPLACEBUTTONDOWN,
//     XTP_NM_REPORT_SORTORDERCHANGED, XTP_NM_REPORT_VALUECHANGED,
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_SELCHANGED            (NM_FIRST-52)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for handling row checking event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_CHECKED notification message is sent to inform
//     the owner window that the user click check box  of item of report control
//     The owner window of the color picker receives this notification
//     threw the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_CHECKED, XTP_ID_REPORT_CONTROL, OnReportCheckItem)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportCheckItem(NMHDR * pNotifyStruct, LRESULT * /*result*/)
// {
//     XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
//     ASSERT(pItemNotify != NULL);
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_REPORT_COLUMNORDERCHANGED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_NM_REPORT_SELCHANGED,
//     XTP_NM_REPORT_SORTORDERCHANGED, XTP_NM_REPORT_VALUECHANGED,
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_CHECKED               (NM_FIRST-53)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for handling hyperlink click event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_HYPERLINK notification message is sent to inform
//     the owner window that the user click hyperlink of item.
//     The owner window of the color picker receives this notification
//     threw the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_HYPERLINK, XTP_ID_REPORT_CONTROL, OnReportHyperlinkClick)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportHyperlinkClick(NMHDR * pNotifyStruct, LRESULT * /*result*/)
// {
//     XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
//
//     if (!pItemNotify->pRow || !pItemNotify->pColumn)
//         return;
//
//     // if click on Hyperlink in Item
//     if (pItemNotify->nHyperlink >= 0)
//     {
//         TRACE(_T("Hyperlink Click : \n row %d \n col %d \n Hyperlink %d.\n"),
//             pItemNotify->pRow->GetIndex(),
//             pItemNotify->pColumn->GetItemIndex(),
//             pItemNotify->nHyperlink);
//     }
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_COLUMNORDERCHANGED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_NM_REPORT_SELCHANGED,
//     XTP_NM_REPORT_SORTORDERCHANGED, XTP_NM_REPORT_VALUECHANGED,
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_HYPERLINK             (NM_FIRST-54)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for column changed event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_COLUMNORDERCHANGED notification message is sent to inform
//     the owner window that the user change the order of columns of report control.
//     The owner window of the color picker receives this notification
//     threw the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_COLUMNORDERCHANGED, XTP_ID_REPORT_CONTROL, OnReportColumnOrderChanged)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportColumnOrderChanged(NMHDR * pNotifyStruct, LRESULT * /*result*/)
// {
//     XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
//     ASSERT(pItemNotify != NULL);
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_NM_REPORT_SELCHANGED,
//     XTP_NM_REPORT_SORTORDERCHANGED, XTP_NM_REPORT_VALUECHANGED,
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_COLUMNORDERCHANGED    (NM_FIRST-55)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for sort changed event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_SORTORDERCHANGED notification message is sent to inform
//     the owner window that the user change click header of report control and change sort order.
//     The owner window of the color picker receives this notification
//     threw the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_SORTORDERCHANGED, XTP_ID_REPORT_CONTROL, OnReportSortOrderChanged)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportSortOrderChanged(NMHDR * pNotifyStruct, LRESULT * /*result*/)
// {
//     XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
//     ASSERT(pItemNotify != NULL);
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_COLUMNORDERCHANGED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_NM_REPORT_SELCHANGED,
//     XTP_NM_REPORT_VALUECHANGED,
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_SORTORDERCHANGED      (NM_FIRST-56)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for value changed event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_VALUECHANGED notification message is sent to inform
//     the owner window that the user change value of report cell
//     The owner window of the color picker receives this notification
//     threw the WM_COMMAND message.
// <code>
// BEGIN_MESSAGE_MAP(CPropertiesView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, ID_REPORT_CONTROL, OnReportValueChanged)
// END_MESSAGE_MAP()
//
// void CPropertiesView::OnReportValueChanged(NMHDR * pNotifyStruct, LRESULT * /*result*/)
// {
//     XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
//     ASSERT(pItemNotify != NULL);
//
//     switch (pItemNotify->pItem->GetItemData())
//     {
//         case ID_PROPERTY_MULTIPLESELECTION:
//             GetTargetReport()->SetMultipleSelection(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_PREVIEWMODE:
//             GetTargetReport()->EnablePreviewMode(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             GetTargetReport()->Populate();
//             break;
//         case ID_PROPERTY_GROUPBOXVISIBLE:
//             GetTargetReport()->ShowGroupBy(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_FOCUSSUBITEMS:
//             GetTargetReport()->FocusSubItems(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_ALLOWCOLUMNREMOVE:
//             GetTargetReport()->GetReportHeader()->AllowColumnRemove(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_ALLOWCOLUMNREORDER:
//             GetTargetReport()->GetReportHeader()->AllowColumnReorder(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_ALLOWCOLUMNRESIZE:
//             GetTargetReport()->GetReportHeader()->AllowColumnResize(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_FLATHEADER:
//             GetTargetReport()->GetPaintManager()->m_columnStyle =
//                 (CRecordPropertyBool::GetValue(pItemNotify))? xtpColumnFlat: xtpColumnShaded;
//             break;
//         case ID_PROPERTY_HIDESELECTION:
//             GetTargetReport()->GetPaintManager()->m_bHideSelection =
//                 (CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_TREEINDENT:
//             GetTargetReport()->GetPaintManager()->m_nTreeIndent =
//                 CRecordPropertyInt::GetValue(pItemNotify);
//             GetTargetReport()->RedrawControl();
//             break;
//     }
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_COLUMNORDERCHANGED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_NM_REPORT_SELCHANGED,
//     XTP_NM_REPORT_SORTORDERCHANGED,
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_VALUECHANGED          (NM_FIRST-57)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for in-place button click event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_INPLACEBUTTONDOWN notification message is sent to inform
//     the owner window that the user click in-place button of report control
//     The owner window of the color picker receives this notification
//     threw the WM_COMMAND message.
// Returns:
//     TRUE if message was processed; FALSE for default process.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_ID_REPORT_CONTROL, OnReportButtonClick)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportButtonClick(NMHDR * pNotifyStruct, LRESULT * /*result*/)
// {
//     XTP_NM_REPORTINPLACEBUTTON* pItemNotify = (XTP_NM_REPORTINPLACEBUTTON*) pNotifyStruct;
//     ASSERT(pItemNotify->pButton);
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_COLUMNORDERCHANGED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_SELCHANGED,
//     XTP_NM_REPORT_SORTORDERCHANGED, XTP_NM_REPORT_VALUECHANGED,
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_INPLACEBUTTONDOWN     (NM_FIRST-58)

//{{AFX_CODEJOCK_PRIVATE
#define XTP_TRACE
//}}AFX_CODEJOCK_PRIVATE


#endif //#if !defined(__XTPREPORTDEFINES_H__)
