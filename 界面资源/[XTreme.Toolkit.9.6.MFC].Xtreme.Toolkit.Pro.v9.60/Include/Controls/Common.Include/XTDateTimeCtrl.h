// XTDateTimeCtrl.h interface for the CXTDateTimeCtrl class.
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
#if !defined(__XTDATETIMECTRL_H__)
#define __XTDATETIMECTRL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// class forwards
class CXTMonthCalCtrl;

//===========================================================================
// Summary:
//     CXTDateTimeCtrl is a CWnd derived class. A CXTDateTimeCtrl object encapsulates
//     the functionality of a date and time picker control.
// Remarks:
//     The date and time picker control (DTP control) provides a simple interface to exchange
//     date and time information with a user. This interface contains fields, and
//     each field displays a part of the date and time information stored
//     in the control. The user can change the information stored in the control
//     by changing the content of the string in a given field. The user can
//     also move from field to field using the mouse or the keyboard.
//
//     You can customize the date and time picker control by applying a variety
//     of styles to the object when you create it. See Date and Time Picker
//     Control Styles in the Platform SDK for more information about styles
//     specific to the date and time picker control. You can set the display
//     format of the DTP control using format styles. These format styles
//     are described in Preset DTP Display Formats in the Platform SDK.
//
//     The date and time picker control also uses notifications and callbacks,
//     which are described in Using CXTDateTimeCtrl in the Xtreme Toolkit online
//     help.
//===========================================================================
class _XT_EXT_CLASS CXTDateTimeCtrl : public CWnd
{
	DECLARE_DYNAMIC(CXTDateTimeCtrl)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTDateTimeCtrl object
	//-----------------------------------------------------------------------
	CXTDateTimeCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTDateTimeCtrl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTDateTimeCtrl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function creates the date and time picker control and
	//     attaches it to the CXTDateTimeCtrl object.
	// Parameters:
	//     dwStyle    - Specifies the combination of date time control
	//                  styles. See Date and Time Picker Control Styles
	//                  in the Platform SDK for more information about
	//                  date and time picker styles.
	//     rect       - A reference to a RECT structure, which is the
	//                  position and size of the date and time picker control.
	//     pParentWnd - A pointer to a CWnd object that is the parent
	//                  window of the date and time picker control. It
	//                  must not be NULL.
	//     nID        - Specifies the control ID of the date and time picker
	//                  control.
	// Returns:
	//     Nonzero if creation was successful, otherwise returns 0.
	//-----------------------------------------------------------------------
	BOOL Create(DWORD dwStyle,const RECT& rect,CWnd* pParentWnd,UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the color for a given portion of
	//     the month calendar within the date and time picker control.
	// Parameters:
	//     iColor - An int value specifying which color area of the month
	//              calendar to retrieve. For a list of values, see the
	//              'iColor' parameter for SetMonthCalColor.
	// Returns:
	//     A COLORREF value that represents the color setting for the specified
	//     portion of the month calendar control if successful. The function
	//     returns -1 if unsuccessful.
	//-----------------------------------------------------------------------
	COLORREF GetMonthCalColor(int iColor) const;

	// ------------------------------------------------------------------------------
	// Summary:
	//     This member function sets the color for a given portion of the
	//     month calendar within a date and time picker control.
	// Parameters:
	//     iColor -  An int value specifying which area of the month calendar control
	//               to set. This value can be one of the values listed in the
	//               \remarks section.
	// ref -     A RGB value representing the color that will be set for the
	//               specified area of the month calendar.
	// Remarks:
	//     The area of the calendar control that the color is to be set for
	//     can be one of the following values:
	//     * <b>MCSC_BACKGROUND</b> Set the background color displayed between
	//           months.
	//     * <b>MCSC_MONTHBK</b> Set the background color displayed within
	//           a month.
	//     * <b>MCSC_TEXT</b> Set the color used to display text within a
	//           month.
	//     * <b>MCSC_TITLEBK</b> Set the background color displayed in
	//           the calendar's title.
	//     * <b>MCSC_TITLETEXT</b> Set the color used to display text within
	//           the calendar's title.
	//     * <b>MCSC_TRAILINGTEXT</b> Set the color used to display header
	//           and trailing-day text. Header and trailing days are the days from
	//           the previous and following months that appear on the current
	//           calendar.
	// Returns:
	//     A COLORREF value that represents the previous color setting for
	//     the specified portion of the month calendar control if successful.
	//     Otherwise, the message returns -1.
	// ------------------------------------------------------------------------------
	COLORREF SetMonthCalColor(int iColor,COLORREF ref);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the display of a date and time picker
	//     control based on a given format string.
	// Parameters:
	//     pstrFormat - A pointer to a zero-terminated format string that
	//                  defines the desired display. Setting this parameter
	//                  to NULL will reset the control to the default format
	//                  string for the current style.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL SetFormat(LPCTSTR pstrFormat);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the date and time picker's child
	//     month calendar control.
	// Returns:
	//     A pointer to a DDX_XTMonthCalCtrl object;
	//     or NULL if unsuccessful or the window is not visible.
	//-----------------------------------------------------------------------
	CXTMonthCalCtrl* GetMonthCalCtrl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the font currently used by the date
	//     and time picker control's child month calendar control.
	// Parameters:
	//     hFont   - Handle to the font that will be set.
	//     bRedraw - Specifies whether or not the control should be redrawn
	//               immediately upon setting the font. Setting this
	//               parameter to TRUE causes the control to redraw itself.
	// Returns:
	//     A pointer to a CFont object, or returns NULL if unsuccessful.
	//-----------------------------------------------------------------------
	CFont* GetMonthCalFont() const;

	// -----------------------------------------------------------------------
	// Summary:
	//     This member function sets the font to be used by the date and time
	//     picker control's child month calendar control.
	// Parameters:
	//     hFont :    Handle to a HFONT object that represents the new font to
	//                use.
	// bRedraw :  TRUE to redraw the CXTDateTimeCtrl object.
	// -----------------------------------------------------------------------
	void SetMonthCalFont(HFONT hFont,BOOL bRedraw = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the minimum and maximum allowable system
	//     times for a date and time picker control.
	// Parameters:
	//     pMinRange - A pointer to a CTime or COleDateTime object containing the
	//                 earliest time allowed in the CXTDateTimeCtrl object.
	//     pMaxRange - A pointer to a CTime or COleDateTime object containing the
	//                 latest time allowed in the CXTDateTimeCtrl object.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL SetRange(const COleDateTime* pMinRange,const COleDateTime* pMaxRange);
	BOOL SetRange(const CTime* pMinRange,const CTime* pMaxRange); //<combine CXTDateTimeCtrl::SetRange@const COleDateTime*@const COleDateTime*>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the current minimum and maximum allowable
	//     system times for a date and time picker control.
	// Parameters:
	//     pMinRange - A pointer to a COleDateTime or CTime object containing the earliest
	//                 time allowed in the CXTDateTimeCtrl object.
	//     pMaxRange - A pointer to a COleDateTime or CTime object containing the latest
	//                 time allowed in the CXTDateTimeCtrl object.
	// Returns:
	//     A DWORD value containing flags that indicate which ranges are set.
	//-----------------------------------------------------------------------
	DWORD GetRange(COleDateTime* pMinRange,COleDateTime* pMaxRange) const;
	DWORD GetRange(CTime* pMinRange,CTime* pMaxRange) const; //<combine CXTDateTimeCtrl::GetRange@COleDateTime*@COleDateTime*@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the time in a date and time picker control.
	// Parameters:
	//     pTimeNew - A pointer to a CTime or SYSTEMTIME object containing the time that
	//                the control will be set to.
	//     timeNew - A reference to a COleDateTime object containing the
	//               time that the control will be set to.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL SetTime(const CTime* pTimeNew);
	BOOL SetTime(const COleDateTime& timeNew); //<combine CXTDateTimeCtrl::SetTime@const CTime*>
	BOOL SetTime(LPSYSTEMTIME pTimeNew = NULL); //<combine CXTDateTimeCtrl::SetTime@const CTime*>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the currently selected time from
	//     a date and time picker control and places it in a specified SYSTEMTIME
	//     structure.
	// Parameters:
	//     timeDest - A reference to a CTime or COleDateTime object that will receive
	//                the system time information.
	//     pTimeDest - A pointer to the SYSTEMTIME structure to receive
	//                 the system time information. Must not be NULL.
	// Returns:
	//     In the first version nonzero if the time is successfully written to the
	//     COleDateTime object, otherwise returns zero. In the second and third version
	//     a DWORD value equal to the dwFlag member set in the NMDATETIMECHANGE
	//     structure.
	//-----------------------------------------------------------------------
	BOOL GetTime(COleDateTime& timeDest) const;
	DWORD GetTime(CTime& timeDest) const; //<combine CXTDateTimeCtrl::GetTime@COleDateTime&@const>
	DWORD GetTime(LPSYSTEMTIME pTimeDest) const; //<combine CXTDateTimeCtrl::GetTime@COleDateTime&@const>

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTDateTimeCtrl::CXTDateTimeCtrl() {

}
AFX_INLINE BOOL CXTDateTimeCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE CFont* CXTDateTimeCtrl::GetMonthCalFont() const {
	ASSERT(::IsWindow(m_hWnd)); return CFont::FromHandle((HFONT) ::SendMessage(m_hWnd, DTM_GETMCFONT, 0, 0));
}
AFX_INLINE CXTMonthCalCtrl* CXTDateTimeCtrl::GetMonthCalCtrl() const {
	ASSERT(::IsWindow(m_hWnd)); return (CXTMonthCalCtrl*) CWnd::FromHandle((HWND) ::SendMessage(m_hWnd, DTM_GETMONTHCAL, 0, 0));
}
AFX_INLINE void CXTDateTimeCtrl::SetMonthCalFont(HFONT hFont, BOOL bRedraw /* = TRUE */) {
	ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, DTM_SETMCFONT, (WPARAM) hFont, MAKELONG(bRedraw, 0));
}
AFX_INLINE COLORREF CXTDateTimeCtrl::SetMonthCalColor(int iColor, COLORREF ref) {
	ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, DTM_SETMCCOLOR, (WPARAM) iColor, (LPARAM) ref);
}
AFX_INLINE DWORD CXTDateTimeCtrl::GetTime(LPSYSTEMTIME pTimeDest) const {
	ASSERT(::IsWindow(m_hWnd)); ASSERT(pTimeDest != NULL); return (DWORD) ::SendMessage(m_hWnd, DTM_GETSYSTEMTIME, 0, (LPARAM) pTimeDest);
}
AFX_INLINE COLORREF CXTDateTimeCtrl::GetMonthCalColor(int iColor) const {
	ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, DTM_GETMCCOLOR, (WPARAM) iColor, 0);
}
AFX_INLINE BOOL CXTDateTimeCtrl::SetFormat(LPCTSTR pstrFormat) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, DTM_SETFORMAT, 0, (LPARAM) pstrFormat);
}

// ----------------------------------------------------------------------------
// Summary:
//     The DDX_XTDateTimeCtrl function manages the transfer of
//     date and/or time data between a date and time picker control
//     (CXTDateTimeCtrl) in a dialog box or form view object and a
//     COleDateTime data member of the dialog box or form view object.
//
//     When DDX_XTDateTimeCtrl is called, <i>'value'</i> is set to
//     the current state of the date and time picker control, or the
//     control is set to <i>'value'</i>, depending on the direction of
//     the exchange.
// Parameters:
//     pDX -    A pointer to a CDataExchange object. The framework supplies
//              this object to establish the context of the data exchange,
//              including its direction. You do not need to delete this object.
//     nIDC -   The resource ID of the date and time picker control associated
//              with the member variable.
//     value -  A reference to a COleDateTime member variable, dialog box, form
//              view, or control view object with which data is exchanged.
// See Also:
//     CXTDateTimeCtrl
// ----------------------------------------------------------------------------
_XT_EXT_CLASS void AFXAPI DDX_XTDateTimeCtrl(CDataExchange* pDX,int nIDC,COleDateTime& value);

// ----------------------------------------------------------------------------
// Summary:
//     The DDX_XTDateTimeCtrl function manages the transfer of
//     date and/or time data between a date and time picker control
//     (CXTDateTimeCtrl) in a dialog box or form view object, and a CTime
//     data member of the dialog box or form view object.
//
//     When DDX_XTDateTimeCtrl is called, <i>'value'</i> is set to
//     the current state of the date and time picker control, or the
//     control is set to <i>'value'</i>, depending on the direction of
//     the exchange.
// Parameters:
//     pDX -    A pointer to a CDataExchange object. The framework supplies
//              this object to establish the context of the data exchange,
//              including its direction. You do not need to delete this object.
//     nIDC -   The resource ID of the date and time picker control associated
//              with the member variable.
//     value -  A reference to a CTime member variable, dialog box, form view,
//              or control view object with which data is exchanged.
// See Also:
//     CXTDateTimeCtrl
// ----------------------------------------------------------------------------
_XT_EXT_CLASS void AFXAPI DDX_XTDateTimeCtrl(CDataExchange* pDX,int nIDC,CTime& value);

// ----------------------------------------------------------------------------
// Summary:
//     Call DDV_XTMinMaxDateTime to verify that the time/date
//     value in the date and time picker control (CXTDateTimeCtrl)
//     associated with <i>'refValue'</i> falls between <i>'pMinRange'</i>
//     and <i>'pMaxRange'</i>.
// Parameters:
//     pDX -        A pointer to a CDataExchange object. The framework supplies
//                  this object to establish the context of the data exchange,
//                  including its direction. You do not need to delete this
//                  object.
//     refValue -   A reference to a CTime object associated with a member
//                  variable of the dialog box, form view, or control view
//                  object. This object contains the data to be validated.
//     pMinRange -  Minimum date/time value allowed.
//     pMaxRange -  Maximum date/time value allowed.
// See Also:
//     CXTDateTimeCtrl
// ----------------------------------------------------------------------------
_XT_EXT_CLASS void AFXAPI DDV_XTMinMaxDateTime(CDataExchange* pDX,CTime& refValue,const CTime* pMinRange,const CTime* pMaxRange);

// ----------------------------------------------------------------------------
// Summary:
//     Call DDV_XTMinMaxDateTime to verify that the time/date
//     value in the date and time picker control (CXTDateTimeCtrl)
//     associated with <i>'refValue'</i> falls between <i>'pMinRange'</i>
//     and <i>'pMaxRange'</i>.
// Parameters:
//     pDX -        A pointer to a CDataExchange object. The framework supplies
//                  this object to establish the context of the data exchange,
//                  including its direction. You do not need to delete this
//                  object.
//     refValue -   A reference to a COleDateTime object associated with a
//                  member variable of the dialog box, form view, or control
//                  view object. This object contains the data to be validated.
//     pMinRange -  Minimum date/time value allowed.
//     pMaxRange -  Maximum date/time value allowed.
// See Also:
//     CXTDateTimeCtrl
// ----------------------------------------------------------------------------
_XT_EXT_CLASS void AFXAPI DDV_XTMinMaxDateTime(CDataExchange* pDX,COleDateTime& refValue,const COleDateTime* pMinRange,const COleDateTime* pMaxRange);

//===========================================================================
// Summary:
//     CXTMonthCalCtrl is a CWnd derived class. A CXTMonthCalCtrl object encapsulates
//     the functionality of a month calendar control.
// Remarks:
//     The month calendar control provides the user with a simple calendar interface that the user
//     can select a date from. The user can change the display by:
//
//     * Scrolling backward and forward, from month to month.
//     * Clicking the Today text to display the current day (if the MCS_NOTODAY
//     style is not used).
//     * Picking a month or a year from a popup menu.
//
//     You can customize the month calendar control by applying a variety of
//     styles to the object when you create it. These styles are described
//     in Month Calendar Control Styles in the Platform SDK.
//
//     The month calendar control can display more than one month, and it can
//     indicate special days, such as holidays, by bolding the date.
//
//     For more information on using the month calendar control, see Using
//     CXTMonthCalCtrl in the Xtreme Toolkit online help.
//===========================================================================
class _XT_EXT_CLASS CXTMonthCalCtrl : public CWnd
{
	DECLARE_DYNAMIC(CXTMonthCalCtrl)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTMonthCalCtrl object
	//-----------------------------------------------------------------------
	CXTMonthCalCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTMonthCalCtrl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTMonthCalCtrl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function creates a month calendar control and attaches
	//     it to the DDX_XTMonthCalCtrl object.
	// Parameters:
	//     dwStyle    - Specifies the combination of Windows styles applied
	//                  to the month calendar control. See Month Calendar
	//                  Control Styles in the Platform SDK for more information
	//                  about the styles.
	//     rect       - A reference to a RECT structure. Contains the position
	//                  and size of the month calendar control.
	//     pt         - A reference to a POINT structure that identifies
	//                  the location of the month calendar control.
	//     pParentWnd - A pointer to a CWnd object that is the parent window
	//                  of the month calendar control. It must not be NULL.
	//     nID        - Specifies the control ID of the month calendar control.
	// Returns:
	//     Nonzero if initialization was successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL Create(DWORD dwStyle,const RECT& rect,CWnd* pParentWnd,UINT nID);
	BOOL Create(DWORD dwStyle,const POINT& pt,CWnd* pParentWnd,UINT nID); //<COMBINE CXTMonthCalCtrl::Create@DWORD@const RECT&@CWnd*@UINT>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the minimum size required to show
	//     a full month in a month calendar control.
	// Parameters:
	//     pRect - A pointer to a RECT structure that will receive
	//             bounding rectangle information. This parameter must be a valid
	//             address and cannot be NULL.
	// Returns:
	//     Nonzero and 'lpRect' receives the applicable bounding information. If
	//     unsuccessful, this member function returns zero.
	//-----------------------------------------------------------------------
	BOOL GetMinReqRect(RECT* pRect) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the scroll rate for a month calendar
	//     control.
	// Parameters:
	//     iDelta - The number of months to be set as the control's scroll
	//              rate. If this value is zero, the month delta is reset
	//              to the default, which is the number of months displayed
	//              in the control.
	// Returns:
	//     The previous scroll rate. If the scroll rate has not been previously
	//     set, the return value is zero.
	//-----------------------------------------------------------------------
	int SetMonthDelta(int iDelta);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the scroll rate for a month calendar
	//     control. The scroll rate is the number of months that the control
	//     moves its display when the user clicks a scroll button once.
	// Returns:
	//     An integer value that represents the scroll rate for the month
	//     calendar control.
	//-----------------------------------------------------------------------
	int GetMonthDelta() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the day of the week to be displayed in
	//     the leftmost column of the calendar.
	// Parameters:
	//     iDay   - An integer value representing which day is to be set
	//              as the first day of the week. This value must be one
	//              of the day numbers. See GetFirstDayOfWeek for a description
	//              of the day numbers.
	//     lpnOld - A pointer to an integer indicating the first day of
	//              the week that was previously set.
	// Returns:
	//     Nonzero if the previous first day of the week is set to a value other than
	//     that of  LOCALE_IFIRSTDAYOFWEEK, which is the day indicated in the control
	//     panel setting. Otherwise, this function returns zero.
	//-----------------------------------------------------------------------
	BOOL SetFirstDayOfWeek(int iDay,int* lpnOld = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the first day of the week to be displayed
	//     in the leftmost column of the calendar.
	// Parameters:
	//     pbLocal - A pointer to a BOOL value. If the value is nonzero,
	//               the control's setting does not match the setting in
	//               the control panel.
	// Returns:
	//     An integer value that represents the first day of the week.
	//     The days of the week are represented by integers, as follows:
	//     <code>
	//     Value   Day of the Week
	//     0       Monday
	//     1       Tuesday
	//     2       Wednesday
	//     3       Thursday
	//     4       Friday
	//     5       Saturday
	//     6       Sunday
	//     </code>
	//-----------------------------------------------------------------------
	int GetFirstDayOfWeek(BOOL* pbLocal = NULL) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the color of a specified area of a month
	//     calendar control.
	// Parameters:
	//     nRegion - The region of the month calendar control from which
	//               the color is retrieved. For a list of values, see the
	//               'nRegion' parameter of SetColor.
	// Returns:
	//     A COLORREF value specifying the color associated
	//     with the portion of the month calendar control, if successful.
	//     Otherwise, this member function returns -1.
	//-----------------------------------------------------------------------
	COLORREF GetColor(int nRegion) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the color of a specified area of a month
	//     calendar control.
	// Parameters:
	//     nRegion - An integer value specifying which month calendar color
	//               to set. See remarks section for a list of values.
	//     ref     - A COLORREF value for the new color setting for the specified
	//               portion of the month calendar control.
	// Remarks:
	//     nRegion values can be one of the following:
	//          * <b>MCSC_BACKGROUND</b> The background color displayed between
	//            months.
	//          * <b>MCSC_MONTHBK</b> The background color displayed within
	//            the month.
	//          * <b>MCSC_TEXT</b> The color used to display text within a month.
	//          * <b>MCSC_TITLEBK</b> The background color displayed in the
	//            calendar's title.
	//          * <b>MCSC_TITLETEXT</b> The color used to display text within
	//            the calendar's title.
	//          * <b>MCSC_TRAILINGTEXT</b> The color used to display header
	//            and trailing-day text. Header and trailing days are the days
	//            from the previous and following months that appear on the current
	//            calendar.
	// Returns:
	//     A COLORREF value that represents the previous
	//     color setting for the specified portion of the month calendar
	//     control, if successful. Otherwise this message returns -1.
	//-----------------------------------------------------------------------
	COLORREF SetColor(int nRegion,COLORREF ref);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function repaints the month calendar control to its
	//     minimum, one-month size.
	// Parameters:
	//     bRepaint - Specifies whether the control is to be repainted.
	//                By default, TRUE. If FALSE, no repainting occurs.
	// Returns:
	//     Nonzero if the month calendar control is sized to its minimum, otherwise
	//     returns zero.
	//-----------------------------------------------------------------------
	BOOL SizeMinReq(BOOL bRepaint = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the calendar control to the current day.
	// Parameters:
	//     refDateTime - A reference to a COleDateTime object that contains
	//                   the current date.
	//     pDateTime - A pointer to a CTime or SYSTEMTIME object containing the current
	//                 date information.
	//-----------------------------------------------------------------------
	void SetToday(const CTime* pDateTime);
	void SetToday(const COleDateTime& refDateTime); //<COMBINE CXTMonthCalCtrl::SetToday@const CTime*>
	void SetToday(const LPSYSTEMTIME pDateTime);    //<COMBINE CXTMonthCalCtrl::SetToday@const CTime*>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the date information for the date
	//     specified as "today" for a month calendar control.
	// Parameters:
	//     refTime - A reference to a CTime or COleDateTime object indicating the current day.
	//     pDateTime - A pointer to a SYSTEMTIME structure that
	//                 will receive the date information.  This parameter
	//                 must be a valid address and cannot be NULL.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL GetToday(CTime& refTime) const;
	BOOL GetToday(COleDateTime& refDateTime) const; //<COMBINE CXTMonthCalCtrl::GetToday@CTime&@const>
	BOOL GetToday(LPSYSTEMTIME pDateTime) const;    //<COMBINE CXTMonthCalCtrl::GetToday@CTime&@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the system time as indicated by the
	//     currently selected date.
	// Parameters:
	//     pDateTime - A pointer to a SYSTEMTIME structure that will receive
	//                 the currently selected date information. This parameter
	//                 must be a valid address and cannot be NULL.
	//     refDateTime - A reference to a CTime or a COleDateTime object. It receives the
	//                   current time.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL GetCurSel(CTime& refDateTime) const;
	BOOL GetCurSel(COleDateTime& refDateTime) const; //<COMBINE CXTMonthCalCtrl::GetCurSel@CTime&@const>
	BOOL GetCurSel(LPSYSTEMTIME pDateTime) const;    //<COMBINE CXTMonthCalCtrl::GetCurSel@CTime&@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the currently selected date for a month
	//     calendar control.
	// Parameters:
	//     pDateTime - Pointer to a SYSTEMTIME structure that contains the
	//                 date to be set as the current selection.
	//     refDateTime - A reference to a CTime or COleDateTime object indicating the
	//                   currently selected month calendar control.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL SetCurSel(const CTime& refDateTime);
	BOOL SetCurSel(const COleDateTime& refDateTime); //<COMBINE CXTMonthCalCtrl::SetCurSel@const CTime&>
	BOOL SetCurSel(const LPSYSTEMTIME pDateTime);    //<COMBINE CXTMonthCalCtrl::SetCurSel@const CTime&>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the display for days in a month calendar
	//     control.
	// Parameters:
	//     nMonths - Value indicating how many elements are in the array
	//               that 'pStates' points to.
	//     pStates - A pointer to a MONTHDAYSTATE array of values that define
	//               how the month calendar control will draw each day in its
	//               display. The MONTHDAYSTATE data type is a bit field, where
	//               each bit (1 through 31) represents the state of a day in a
	//               month. If a bit is on, the corresponding day will be displayed
	//               in bold; otherwise it will be displayed with no emphasis.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL SetDayState(int nMonths,LPMONTHDAYSTATE pStates);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the maximum number of days that can
	//     be selected in a month calendar control.
	// Parameters:
	//     nMax - Specifies the value that will be set to represent the
	//            maximum number of selectable days.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL SetMaxSelCount(int nMax);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the current maximum number of days
	//     that can be selected in a month calendar control.
	// Returns:
	//     An integer value that represents the total number of days that
	//     can be selected for the control.
	//-----------------------------------------------------------------------
	int GetMaxSelCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the minimum and maximum allowable dates for a month
	//     calendar control.
	// Parameters:
	//     pMinRange - A pointer to a CTime, COleDateTime or SYSTEMTIME object containing
	//                 the date at the lowest end of the range.
	//     pMaxRange - A pointer to a CTime, COleDateTime or SYSTEMTIME object containing the
	//                 date at the highest end of the range.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL SetRange(const CTime* pMinRange,const CTime* pMaxRange);
	BOOL SetRange(const COleDateTime* pMinRange,const COleDateTime* pMaxRange); //<COMBINE CXTMonthCalCtrl::SetRange@const CTime*@const CTime*>
	BOOL SetRange(const LPSYSTEMTIME pMinRange,const LPSYSTEMTIME pMaxRange);   //<COMBINE CXTMonthCalCtrl::SetRange@const CTime*@const CTime*>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the current minimum and maximum dates set
	//     in a month calendar control.
	// Parameters:
	//     pMinRange - A pointer to a CTime, COleDateTime or SYSTEMTIME object containing
	//                 the date at the lowest end of the range.
	//     pMaxRange - A pointer to a CTime, COleDateTime or SYSTEMTIME object containing the
	//                 date at the highest end of the range.
	// Returns:
	//     A DWORD that can be zero (no limits are set) or a combination
	//     of the following values that specify limit information:
	//
	//     * <b>GDTR_MAX</b> A maximum limit is set for the control; pMaxRange
	//       is valid and contains the applicable date information.
	//     * <b>GDTR_MIN</b> A minimum limit is set for the control; pMinRange
	//       is valid and contains the applicable date information.
	//
	//-----------------------------------------------------------------------
	DWORD GetRange(CTime* pMinRange,CTime* pMaxRange) const;
	DWORD GetRange(COleDateTime* pMinRange,COleDateTime* pMaxRange) const; //<COMBINE CXTMonthCalCtrl::GetRange@CTime*@CTime*@const>
	DWORD GetRange(LPSYSTEMTIME pMinRange,LPSYSTEMTIME pMaxRange) const;   //<COMBINE CXTMonthCalCtrl::GetRange@CTime*@CTime*@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves date information representing the
	//     high and low limits of a month calendar control's display.
	// Parameters:
	//     pMinRange   - A pointer to a SYSTEMTIME structure containing the
	//                   date at the lowest end of the range.
	//     refMinRange - A reference to a CTime or COleDateTime object
	//                   containing the minimum date allowed.
	//     pMaxRange - A pointer to a SYSTEMTIME structure containing the
	//                 date at the highest end of the range.
	//     refMaxRange - A reference to a CTime or COleDateTime object containing
	//                   the maximum date allowed.
	//     dwFlags     - Value specifying the scope of the range limits to
	//                   be retrieved. This value must be one of the following:
	//
	//     * <b>GMR_DAYSTATE</b> Include preceding and trailing months
	//       of visible range that are only partially displayed.
	//     * <b>GMR_VISIBLE</b> Include only those months that are entirely
	//       displayed.
	// Returns:
	//     An integer that represents the range, in months, spanned by the two
	//     limits indicated by 'refMinRange' and 'refMaxRange'.
	//-----------------------------------------------------------------------
	int GetMonthRange(CTime& refMinRange,CTime& refMaxRange,DWORD dwFlags) const;
	int GetMonthRange(COleDateTime& refMinRange,COleDateTime& refMaxRange,DWORD dwFlags) const; // <COMBINE CXTMonthCalCtrl::GetMonthRange@CTime&@CTime&@DWORD@const>
	int GetMonthRange(LPSYSTEMTIME pMinRange,LPSYSTEMTIME pMaxRange,DWORD dwFlags) const;       // <COMBINE CXTMonthCalCtrl::GetMonthRange@CTime&@CTime&@DWORD@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the selection for a month calendar
	//     control to a given date range.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	// Parameters:
	//     pMinRange - A pointer to a CTime, COleDateTime or SYSTEMTIME object containing
	//                 the date at the lowest end of the range.
	//     pMaxRange - A pointer to a CTime, COleDateTime or SYSTEMTIME object containing the
	//                 date at the highest end of the range.
	//-----------------------------------------------------------------------
	BOOL SetSelRange(const CTime& pMinRange,const CTime& pMaxRange);
	BOOL SetSelRange(const COleDateTime& pMinRange,const COleDateTime& pMaxRange); // <COMBINE CXTMonthCalCtrl::SetSelRange@const CTime&@const CTime&>
	BOOL SetSelRange(const LPSYSTEMTIME pMinRange,const LPSYSTEMTIME pMaxRange);   // <COMBINE CXTMonthCalCtrl::SetSelRange@const CTime&@const CTime&>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves date information that represents the
	//     upper and lower limits of the date range currently selected by the
	//     user.
	// Parameters:
	//     refMinRange - A reference to a CTime or COleDateTime object containing
	//                   the minimum date allowed.
	//     pMinRange - A pointer to a SYSTEMTIME structure containing
	//                 the date at the lowest end of the range.
	//     refMaxRange - A reference to a CTime or COleDateTime object containing
	//                   the maximum date allowed.
	//     pMaxRange - A pointer to a SYSTEMTIME structure containing the
	//                 date at the highest end of the range.
	// Returns:
	//     Nonzero if successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL GetSelRange(CTime& refMinRange,CTime& refMaxRange) const;
	BOOL GetSelRange(COleDateTime& refMinRange,COleDateTime& refMaxRange) const;  // <COMBINE CXTMonthCalCtrl::GetSelRange@CTime&@CTime&@const>
	BOOL GetSelRange(LPSYSTEMTIME pMinRange,LPSYSTEMTIME pMaxRange) const;        // <COMBINE CXTMonthCalCtrl::GetSelRange@CTime&@CTime&@const>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function determines which portion of a month calendar
	//     control is at a given point on the screen.
	// Parameters:
	//     pMCHitTest - A pointer to a MCHITTESTINFO structure containing
	//                  hit testing points for the month calendar control.
	// Returns:
	//     A DWORD value equal to the 'uHit' member of the MCHITTESTINFO
	//     structure.
	//-----------------------------------------------------------------------
	DWORD HitTest(PMCHITTESTINFO pMCHitTest);

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTMonthCalCtrl::CXTMonthCalCtrl() {

}
AFX_INLINE BOOL CXTMonthCalCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE DWORD CXTMonthCalCtrl::HitTest(PMCHITTESTINFO pMCHitTest) {
	ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, MCM_HITTEST, 0, (LPARAM) pMCHitTest);
}
AFX_INLINE BOOL CXTMonthCalCtrl::GetMinReqRect(RECT* pRect) const {
	ASSERT(m_hWnd != NULL); return (BOOL) ::SendMessage(m_hWnd, MCM_GETMINREQRECT, 0, (LPARAM) pRect);
}
AFX_INLINE int CXTMonthCalCtrl::SetMonthDelta(int iDelta) {
	ASSERT(m_hWnd != NULL); return (int) ::SendMessage(m_hWnd, MCM_SETMONTHDELTA, (WPARAM) iDelta, 0);
}
AFX_INLINE int CXTMonthCalCtrl::GetMonthDelta() const {
	ASSERT(m_hWnd != NULL); return (int) ::SendMessage(m_hWnd, MCM_GETMONTHDELTA, 0, 0);
}
AFX_INLINE COLORREF CXTMonthCalCtrl::GetColor(int nRegion) const {
	ASSERT(m_hWnd != NULL); return (COLORREF) ::SendMessage(m_hWnd, MCM_GETCOLOR, (WPARAM) nRegion, 0);
}
AFX_INLINE COLORREF CXTMonthCalCtrl::SetColor(int nRegion, COLORREF ref) {
	ASSERT(m_hWnd != NULL); return (COLORREF) ::SendMessage(m_hWnd, MCM_SETCOLOR, (WPARAM) nRegion, (LPARAM) ref);
}
AFX_INLINE BOOL CXTMonthCalCtrl::SetMaxSelCount(int nMax) {
	ASSERT(m_hWnd != NULL); return (BOOL) ::SendMessage(m_hWnd, MCM_SETMAXSELCOUNT, nMax, 0);
}
AFX_INLINE int CXTMonthCalCtrl::GetMaxSelCount() const {
	ASSERT(m_hWnd != NULL); return (int) ::SendMessage(m_hWnd, MCM_GETMAXSELCOUNT, 0, 0);
}
AFX_INLINE void CXTMonthCalCtrl::SetToday(const LPSYSTEMTIME pDateTime) {
	ASSERT(m_hWnd != NULL); ::SendMessage(m_hWnd, MCM_SETTODAY, 0, (LPARAM) pDateTime);
}
AFX_INLINE BOOL CXTMonthCalCtrl::GetToday(LPSYSTEMTIME pDateTime) const {
	ASSERT(m_hWnd != NULL); return (BOOL) ::SendMessage(m_hWnd, MCM_GETTODAY, 0, (LPARAM) pDateTime);
}
AFX_INLINE BOOL CXTMonthCalCtrl::SetCurSel(const LPSYSTEMTIME pDateTime) {
	ASSERT(m_hWnd != NULL); return (BOOL) ::SendMessage(m_hWnd, MCM_SETCURSEL, 0, (LPARAM) pDateTime);
}
AFX_INLINE BOOL CXTMonthCalCtrl::GetCurSel(LPSYSTEMTIME pDateTime) const {
	ASSERT(m_hWnd != NULL); return (BOOL) ::SendMessage(m_hWnd, MCM_GETCURSEL, 0, (LPARAM) pDateTime);
}

// ----------------------------------------------------------------------------
// Summary:
//     The DDX_XTMonthCalCtrl function manages the transfer of
//     date data between a month calendar control (CXTMonthCalCtrl) in a
//     dialog box, form view, or control view object, and a COleDateTime
//     data member, of the dialog box, form view, or control view object.
//
//     The control manages a date value only. The time fields in the time
//     object are set to reflect the creation time of the control
//     window, or whatever time was set in the control with a call to
//     CXTMonthCalCtrl::SetCurSel.
//
// When DDX_XTMonthCalCtrl is called, <i>'value'</i> is set to
//     the current state of the month calendar control.
// Parameters:
//     pDX -    A pointer to a CDataExchange object. The framework supplies
//              this object to establish the context of the data exchange,
//              including its direction. You do not need to delete this object.
//     nIDC -   The resource ID of the month calendar control associated with
//              the member variable.
//     value -  A reference to a COleDateTime member variable of the dialog
//              box, form view, or control view object with which data is
//              exchanged.
// See Also:
//     CXTMonthCalCtrl
// ----------------------------------------------------------------------------
_XT_EXT_CLASS void AFXAPI DDX_XTMonthCalCtrl(CDataExchange* pDX,    int nIDC,COleDateTime& value);

// ----------------------------------------------------------------------------
// Summary:
//     The DDX_XTMonthCalCtrl function manages the transfer of
//     date data between a month calendar control (CXTMonthCalCtrl), in a
//     dialog box, form view, or control view object, and a CTime data
//     member of the dialog box, form view, or control view object.
//
//     The control manages a date value only. The time fields in the time
//     object are set to reflect the creation time of the control
//     window, or whatever time was set in the control with a call to
//     CXTMonthCalCtrl::SetCurSel.
//
//     When DDX_XTMonthCalCtrl is called, <i>'value'</i> is set to
//     the current state of the month calendar control.
// Parameters:
//     pDX -    A pointer to a CDataExchange object. The framework supplies
//              this object to establish the context of the data exchange,
//              including its direction. You do not need to delete this object.
//     nIDC -   The resource ID of the month calendar control associated with
//              the member variable.
//     value -  A reference to a CTime member variable of the dialog box, form
//              view, or control view object with which data is exchanged.
// See Also:
//     CXTMonthCalCtrl
// ----------------------------------------------------------------------------
_XT_EXT_CLASS void AFXAPI DDX_XTMonthCalCtrl(CDataExchange* pDX,int nIDC,CTime& value);

// -------------------------------------------------------------------------------------
// Summary:
//     Call DDV_XTMinMaxMonth to verify that the time/date value
//     in the month calendar control (CXTMonthCalCtrl) associated with <i>'refValue'</i>
//     falls between <i>'pMinRange'</i> and <i>'pMaxRange'</i>.
// Parameters:
//     pDX -        A pointer to a CDataExchange object. The framework supplies
//                  this object to establish the context of the data exchange,
//                  including its direction.
//     refValue -   A reference to an object of type CTime associated with a
//                  member variable of the dialog box, form view, or control view
//                  object. This object contains the data to be validated. MFC
//                  passes this reference when DDV_XTMinMaxMonth is
//                  called.
//     pMinRange -  Minimum date/time value allowed.
//     pMaxRange -  Maximum date/time value allowed.
// See Also:
//     CXTMonthCalCtrl
// -------------------------------------------------------------------------------------
_XT_EXT_CLASS void AFXAPI DDV_XTMinMaxMonth(CDataExchange* pDX,CTime& refValue,const CTime* pMinRange,const CTime* pMaxRange);

// -------------------------------------------------------------------------------------
// Summary:
//     Call DDV_XTMinMaxMonth to verify that the time/date value
//     in the month calendar control (CXTMonthCalCtrl) associated with <i>'refValue'</i>
//     falls between <i>'pMinRange'</i> and <i>'pMaxRange'</i>.
// Parameters:
//     pDX -        A pointer to a CDataExchange object. The framework supplies
//                  this object to establish the context of the data exchange,
//                  including its direction.
//     refValue -   A reference to an object of type CTime associated with a
//                  member variable of the dialog box, form view, or control view
//                  object. This object contains the data to be validated. MFC
//                  passes this reference when DDV_XTMinMaxMonth is
//                  called.
//     pMinRange -  Minimum date/time value allowed.
//     pMaxRange -  Maximum date/time value allowed.
// See Also:
//     CXTMonthCalCtrl
// -------------------------------------------------------------------------------------
_XT_EXT_CLASS void AFXAPI DDV_XTMinMaxMonth(CDataExchange* pDX,COleDateTime& refValue,const COleDateTime* pMinRange,const COleDateTime* pMaxRange);

#endif // #if !defined(__XTDATETIMECTRL_H__)
