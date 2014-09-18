// XTPPropertyGridItemExt.h
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
#if !defined(__XTPPROPERTYGRIDITEMEXT_H__)
#define __XTPPROPERTYGRIDITEMEXT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTPPropertyGridItem.h"

class CXTPPropertyGridItemDate;


//===========================================================================
// Summary:
//     Month calendar popup used for CXTPPropertyGridItemDate item.
//===========================================================================
class CXTPPropertyGridInplaceMonthCal : public CWnd
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridInplaceMonthCal object.
	// Parameters:
	//     pItem - Parent item
	//-----------------------------------------------------------------------
	CXTPPropertyGridInplaceMonthCal(CXTPPropertyGridItemDate* pItem)
	{
		m_pItem = pItem;
	}

//{{AFX_CODEJOCK_PRIVATE
	AFX_INLINE BOOL GetMinReqRect(RECT* pRect) const
	{
		ASSERT(m_hWnd != NULL); return (BOOL) ::SendMessage(m_hWnd, MCM_GETMINREQRECT, 0, (LPARAM) pRect);
	}

protected:

	void OnAccept();

	virtual void PostNcDestroy();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnSelect(NMHDR* pNMHDR, LRESULT* pResult);


	DECLARE_MESSAGE_MAP()
//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPPropertyGridItemDate* m_pItem;          // Parent item

};


//===========================================================================
// Summary:
//     CXTPPropertyGridItemDate is a CXTPPropertyGridItem derived class.
//     It is used to create Date item in a Property Grid control.
//
// Remarks:
//     When the in-place button for a date item is pressed, a
//     CXTPPropertyGridInplaceMonthCal object is created, which is a
//     User Interface that allows the user to easily select a date.
//
// Example:
//     This sample illustrates how to add an item of type Date to your grid.
// <code>
// CXTPPropertyGridItem* pStandard   = m_wndPropertyGrid.AddCategory(_T("Appointments"));
//
// //Create a Date object with the date 12/26/2004.  Note that no time information is entered
// //because this information is ignored.
// COleDateTime dates(2004, 12, 26, 0, 0, 0 );
//
// //Adds a date item to the property grid and set the date to the value stored in dates
// CXTPPropertyGridItemDate* pDate = (CXTPPropertyGridItemDate*)(pStandard->AddChildItem(new CXTPPropertyGridItemDate(_T("Date item"), dates)));
//
// //Set the date format to MM/DD/YYYY
// pDate->SetDateFormat("%m/%d/%Y");
//
// //Changes the date to 3/22/2001
// COleDateTime newDate(2003, 5, 12, 0, 0, 0 );
//
// //Changes the date of the Date item.
// pDate->SetDate(newDate);
//
// //Gets the currently set date
// TRACE(_T("Current Date= %d/%d/%d\n"), pDate->GetMonth(), pDate->GetDay(), pDate->GetYear());
// </code>
// See Also: SetDate, GetMonth, GetDay, GetYear, SetDateFormat
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemDate: public CXTPPropertyGridItem
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropertyGridItemDate object.
	// Parameters:
	//     strCaption - Caption of the item.
	//     nID        - Identifier of the item.
	//     oleDate    - COleDateTime object containing the initial
	//                  date of the item.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemDate(CString strCaption, COleDateTime& oleDate);
	CXTPPropertyGridItemDate(UINT nID, COleDateTime& oleDate);  // <COMBINE CXTPPropertyGridItemDate::CXTPPropertyGridItemDate@CString@COleDateTime&>

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the date of the item.
	// Parameters:
	//     oleDate - COleDateTime containing a date.  You must include
	//               the month, day, and year.  All time information
	//               should be set to zero.
	// Example:
	//     This sample illustrates how to set the currently selected
	//     date in and item of type CXTPPropertyGridItemDate.
	// <code>
	// CXTPPropertyGridItem* pStandard   = m_wndPropertyGrid.AddCategory(_T("Appointments"));
	//
	// //Create a Date object with the date 12/26/2004.  Note that no time information is entered
	// //because this information is ignored.
	// COleDateTime dates(2004, 12, 26, 0, 0, 0 );
	//
	// //Adds a date item to the property grid and set the date to the value stored in dates
	// CXTPPropertyGridItemDate* pDate = (CXTPPropertyGridItemDate*)(pStandard->AddChildItem(new CXTPPropertyGridItemDate(_T("Date item"), dates)));
	//
	// //Set the date format to MM/DD/YYYY
	// pDate->SetDateFormat("%m/%d/%Y");
	//
	// //Changes the date to 3/22/2001
	// COleDateTime newDate(2003, 5, 12, 0, 0, 0 );
	//
	// //Changes the date of the Date item.
	// pDate->SetDate(newDate);
	//
	// //Gets the currently set date
	// TRACE(_T("Current Date= %d/%d/%d\n"), pDate->GetMonth(), pDate->GetDay(), pDate->GetYear());
	// </code>
	//-----------------------------------------------------------------------
	void SetDate(const COleDateTime& oleDate);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to convert the time in the COleDateTime
	//     object to be represented as a SYSTEMTIME data structure.
	//     GetAsSystemTime stores the resulting time in the referenced
	//     sysTime object. The SYSTEMTIME data structure initialized by
	//     this function will have its wMilliseconds member set to zero.
	// Parameters:
	//     sysTime - SYSTEMTIME object to hold date.
	// Returns:
	//     TRUE if the COleDateTime date was successfully stored in the
	//     sysTime object.
	// Example:
	//     This sample code illustrates how to use the GetAsSystemTime member.
	// <code>
	// CXTPPropertyGridItem* pStandard   = m_wndPropertyGrid.AddCategory(_T("Appointments"));
	//
	// //Create a Date object with the date 12/26/2004.  Note that no time information is entered
	// //because this information is ignored.
	// COleDateTime dates(2004, 12, 26, 0, 0, 0 );
	//
	// //Adds a date item to the property grid and set the date to the value stored in dates
	// CXTPPropertyGridItemDate* pDate = (CXTPPropertyGridItemDate*)(pStandard->AddChildItem(new CXTPPropertyGridItemDate(_T("Date item"), dates)));
	//
	// //Creates a SYSTEMTIME object
	// SYSTEMTIME sysTime;
	//
	// //Stores the day, month, and year into the SYSTEMTIME structure, all other members will be 0.
	// if(pDate->GetAsSystemTime(sysTime))
	//     TRACE(_T("SysTime Current Date= %d/%d/%d\n"), sysTime.wMonth, sysTime.wDay, sysTime.wYear);
	// </code>
	//-----------------------------------------------------------------------
	BOOL GetAsSystemTime(SYSTEMTIME& sysTime);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to return the currently selected date.
	// Returns:
	//     The currently selected date.
	//-----------------------------------------------------------------------
	const COleDateTime& GetDate();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the Day of the currently selected date.
	// Returns:
	//     Day of the currently set date.
	//-----------------------------------------------------------------------
	long GetDay();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the Day of the date.
	// Parameters:
	//     nDay - New Day of date.
	//-----------------------------------------------------------------------
	void SetDay(long nDay);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the Month of the currently selected date.
	// Returns:
	//     Month of the currently set date.
	//-----------------------------------------------------------------------
	long GetMonth();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the Month of the date.
	// Parameters:
	//     nMonth - New Month of date.
	//-----------------------------------------------------------------------
	void SetMonth(long nMonth);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the Year of the currently selected date.
	// Returns:
	//     Year of the currently set date.
	//-----------------------------------------------------------------------
	long GetYear();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the Year of the date.
	// Parameters:
	//     nYear - New year of date.
	//-----------------------------------------------------------------------
	void SetYear(long nYear);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to parse the month, day, and year from
	//     the COleDateTime object.
	// Parameters:
	//     dt       - COleDateTime object containing the month, day, and year
	//     strValue - The date format.
	//                Format Code:
	//                <p>  %m - Month as decimal number (01 – 12)
	//                <p>  %d - Day of month as decimal number (01 – 31)
	//                <p>  %Y - Year with century, as decimal number
	//                     Format string for 05/22/2004 is "%m%d%Y"
	// Returns:
	//     TRUE if a valid date was extracted, FALSE if an invalid date
	//     or NULL date was found.
	//-----------------------------------------------------------------------
	BOOL ParseDateTime(COleDateTime& dt, CString strValue);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to change the date format.
	// Parameters:
	//     strFormat - The date format.
	//                 <p>  %m - Month as decimal number (01 – 12)
	//                 <p>  %d - Day of month as decimal number (01 – 31)
	//                 <p>  %Y - Year with century, as decimal number
	//                 Format string for 05/22/2004 is "%m%d%Y"
	// Example:
	//     This sample code illustrates how to change the date format of
	//     of type CXTPPropertyGridItemDate.
	// <code>
	// CXTPPropertyGridItem* pStandard   = m_wndPropertyGrid.AddCategory(_T("Appointments"));
	//
	// //Create a Date object with the date 12/26/2004.  Note that no time information is entered
	// //because this information is ignored.
	// COleDateTime dates(2004, 12, 26, 0, 0, 0 );
	//
	// //Adds a date item to the property grid and set the date to the value stored in dates
	// CXTPPropertyGridItemDate* pDate = (CXTPPropertyGridItemDate*)(pStandard->AddChildItem(new CXTPPropertyGridItemDate(_T("Date item"), dates)));
	//
	// //Set the date format to MM/DD/YYYY
	// pDate->SetDateFormat("%m/%d/%Y");
	// </code>
	//-----------------------------------------------------------------------
	void SetDateFormat(CString strFormat);

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses the in-place button.
	//     Override the method to show an item-specific dialog.
	// Remarks:
	//     The in-place button is the button that the user presses to
	//     display date picker.
	//-------------------------------------------------------------------------
	virtual void OnInplaceButtonDown();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to change an item's value.
	//     Override this method to add new functionality.
	//     You should call the base class version of this function from your
	//     override.
	// Parameters:
	//     strValue - New value of the item.
	//-----------------------------------------------------------------------
	virtual void SetValue(CString strValue);

private:
	void Init(COleDateTime& oleDate);

protected:
	COleDateTime m_oleDate;         // Currently selected date.  Only Month, Day, and Year are used.

	friend class CXTPPropertyGridInplaceMonthCal;
};

AFX_INLINE const COleDateTime& CXTPPropertyGridItemDate::GetDate() {
	return m_oleDate;
}

#ifdef __AFXCTL_H__
//{{AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     Picture item of ActiveX PropertyGrid
//===========================================================================
class _XTP_EXT_CLASS CXTPPropertyGridItemPicture: public CXTPPropertyGridItem
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a XTPPropertyGridItemPicture object.
	// Parameters:
	//     strCaption - Text caption of this item.
	//                  This is the text displayed in the left column of
	//                  the property grid.
	//-----------------------------------------------------------------------
	CXTPPropertyGridItemPicture(CString strCaption);

public:
	CPictureHolder& GetPicture();
	void SetPicturePath(LPCTSTR lpszPath);

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called when the user presses the in-place button.
	//     Override the method to show an item-specific dialog.
	// Remarks:
	//     The in-place button is the button that the user presses to
	//     display browse folder dialog so they can select a picture.
	// See Also: XTPPropertyGridItemFlags, CXTPPropertyGridItem::SetFlags
	//-------------------------------------------------------------------------
	virtual void OnInplaceButtonDown();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when an item is drawn. Override this function if
	//     needed.
	// Parameters:
	//     dc - Reference to the device context to be used for rendering an image
	//     of the item.
	//     rcValue - Bounding rectangle of the item.
	// Returns:
	//     TRUE if item is self-drawn.
	//-----------------------------------------------------------------------
	virtual BOOL OnDrawItemValue(CDC& dc, CRect rcValue);

protected:
	CPictureHolder m_olePicture;
	CString m_strPicturePath;

};


AFX_INLINE CPictureHolder& CXTPPropertyGridItemPicture::GetPicture() {
	return m_olePicture;
}
//}}AFX_CODEJOCK_PRIVATE
#endif

#endif // #if !defined(__XTPPROPERTYGRIDITEMEXT_H__)
