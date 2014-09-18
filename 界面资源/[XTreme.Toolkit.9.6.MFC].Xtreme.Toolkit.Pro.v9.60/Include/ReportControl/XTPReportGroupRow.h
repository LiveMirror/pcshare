// XTPReportGroupItem.h: interface for the CXTPReportGroupItem class.
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
#if !defined(__XTPREPORTGROUPROW_H__)
#define __XTPREPORTGROUPROW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTPReportRow.h"

//===========================================================================
// Summary:
//    This class represents a 'group row' of the report control view -
//    that means a row which has no associated Record item, but
//    has a special text caption instead.
// Remarks:
//    CXTPReportGroupRow has a slightly different reaction on mouse events and another
//    draw logic.
//
//    All other behavior of the group row is similar to its parent.
//
// See Also: CXTPReportRow overview, CXTPReportControl overview
//===========================================================================
class _XTP_EXT_CLASS CXTPReportGroupRow : public CXTPReportRow
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     A constructor of the CXTPReportGroupRow object.
	//-----------------------------------------------------------------------
	CXTPReportGroupRow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the group text caption.
	// Remarks:
	//     Call this member function to retrieve a text caption of this
	//     group row item.
	// Returns:
	//     Group text caption.
	// See Also: CXTPReportGroupRow::CXTPReportGroupRow
	//-----------------------------------------------------------------------
	virtual CString GetCaption();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to specify the text that is displayed as the group
	//     caption when the column that the ReportRecordItem is in has been grouped.
	// Parameters:
	//     lpszCaption - Caption to be set
	// Remarks:
	//     When a column that the ReportRecordItem belong to has been grouped,
	//     the caption of the column is displayed as
	//     the group caption.  If you specify a caption with CXTPReportGroupRow::SetCaption
	//     for the ReportRecordItem, the m_strGroupText text
	//     will be displayed instead of the caption of the column.
	//     You can use CXTPReportGroupRow::SetCaption to "sub group" your ReportRecordItems when
	//     the column they belong to has been grouped.  The ReportRecordItems
	//     will be "sub grouped" by the value in m_strGroupText when the
	//     column they belong to is grouped.
	//-----------------------------------------------------------------------
	void SetCaption(LPCTSTR lpszCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the row using the specified device context.
	// Parameters:
	//     pDC     - A pointer to the device context in which the drawing occurs.
	//     rcRow   - Position of the row in client window coordinates.
	// See Also: CXTPReportRow::Draw
	//-----------------------------------------------------------------------
	void Draw(CDC* pDC, CRect rcRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Process mouse single clicks.
	// Parameters:
	//     ptClicked - Point coordinates where the message was fired from.
	// See Also: CXTPReportRow::OnClick
	//-----------------------------------------------------------------------
	void OnClick(CPoint ptClicked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Process mouse double clicks.
	// Parameters:
	//     ptClicked - Point coordinates where the message was fired from.
	// Remarks:
	//     Group row adds a collapse/expand functionality if double-
	//     clicked on a row.
	// See Also: CXTPReportRow::OnDblClick
	//-----------------------------------------------------------------------
	void OnDblClick(CPoint ptClicked);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if this is a group row or not.
	// Returns:
	//     TRUE because this is a group row.
	// See Also: CXTPReportRow::IsGroupRow
	//-----------------------------------------------------------------------
	BOOL IsGroupRow();


protected:
	CString m_strGroupText;     // group text label

	friend class CXTPReportPaintManager;
};

AFX_INLINE BOOL CXTPReportGroupRow::IsGroupRow() {
	return TRUE;
}

#endif //#if !defined(__XTPREPORTGROUPROW_H__)
