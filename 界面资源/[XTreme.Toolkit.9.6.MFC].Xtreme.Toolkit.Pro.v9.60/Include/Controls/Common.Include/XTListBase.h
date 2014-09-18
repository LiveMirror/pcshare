// XTListBase.h interface for the CXTListCtrl class.
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
#if !defined(__XTLISTBASE_H__)
#define __XTLISTBASE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTListBase is a non-inherited class. It implements the extended
//     list control services provided by the Xtreme Toolkit list control classes.
//===========================================================================
class _XT_EXT_CLASS CXTListBase
{
public:
	// --------------------------------------------------------------------
	// Summary:
	//     ROWCOLOR structure is used by the CXTListBase class to
	//     store user defined background and text color settings for a
	//     particular row in a list control.
	// See Also:
	//     CRowColorList, SetRowColor, RemoveRowColors, Lookup
	// --------------------------------------------------------------------
	struct ROWCOLOR
	{
		int      iRow;   // List control row index.
		COLORREF crBack; // RGB value for text background color.
		COLORREF crText; // RGB value for text foreground color.
	};

	// ----------------------------------------------------------------------
	// Summary:
	//     List for maintaining ROWCOLOR structures.
	// Remarks:
	//     CList definition used by the CXTListBase class to maintain
	//     a list of ROWCOLOR structures representing each row that has
	//     a user defined color setting for the list control.
	// See Also:
	//     CXTListBase, CXTListBase::SetRowColor, RemoveRowColors, Lookup, ROWCOLOR
	// ----------------------------------------------------------------------
	typedef CList<ROWCOLOR,ROWCOLOR&> CRowColorList;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTListBase object
	//-----------------------------------------------------------------------
	CXTListBase();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTListBase object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTListBase();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get a pointer to the flat header control.
	// Returns:
	//     A pointer to a CXTFlatHeaderCtrl if subclassed, otherwise returns NULL.
	//-----------------------------------------------------------------------
	CXTFlatHeaderCtrl* GetFlatHeaderCtrl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the sort image for the specified column.
	// Parameters:
	//     nCol - Zero-based index of the column to set the sort image for.
	//     bAsc - TRUE if ascending, otherwise FALSE.
	// Returns:
	//     The zero-based index of the previously sorted column.
	//-----------------------------------------------------------------------
	virtual int SetSortImage(int nCol,BOOL bAsc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will return the column width stored in the
	//     registry.
	// Parameters:
	//     nCol - Zero-based index of the column.
	// Returns:
	//     The column width stored in the registry.
	//-----------------------------------------------------------------------
	virtual int GetStoredWidth(int nCol);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the column width from the registry
	//     settings.
	// Parameters:
	//     nCol - Zero-based index of the column.
	//-----------------------------------------------------------------------
	virtual void SetStoredWidth(int nCol);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function loads the column widths from the registry.
	//-----------------------------------------------------------------------
	virtual void LoadColumnWidths();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function saves the column widths to the registry.
	//-----------------------------------------------------------------------
	virtual void SaveColumnWidths();

	// ---------------------------------------------------------------------
	// Summary:
	//     Automatically sizes a specified column.
	// Parameters:
	//     nCol -  Zero\-based index of the column.
	// Remarks:
	//     This member function will automatically size a given column. Pass
	//     in a -1 to auto-fit all columns.
	// ---------------------------------------------------------------------
	virtual void AutoSizeColumn(int nCol=-1);

	// -------------------------------------------------------------------
	// Summary:
	//     Initializes the auto save feature.
	// Parameters:
	//     lpszSection -  Registry section name. If NULL, the default is
	//                    "Settings".
	// lpszEntry -    Registry entry name. If NULL, the default is "Column
	//                    Info".
	// lpszDefault -  Registry default value. If NULL, the default ID.
	// Remarks:
	//     This member function will initialize the auto save feature. If
	//     called, column widths will be saved to the system registry when
	//     the list control window is destroyed.
	// -------------------------------------------------------------------
	virtual void AutoSaveColumns(LPCTSTR lpszSection=NULL,LPCTSTR lpszEntry=NULL,LPCTSTR lpszDefault=NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to subclass the flat header control.
	// Parameters:
	//     bBoldFont - true to use a bold font in the header control.
	//-----------------------------------------------------------------------
	virtual bool SubclassHeader(bool bBoldFont=false);

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this member function in your derived class to perform custom
	//     sort routines.
	// Parameters:
	//     nCol       - Passed in from the control. Index of the column clicked.
	//     bAscending - Passed in from the control. true if sort order should
	//                  be ascending.
	//-----------------------------------------------------------------------
	virtual bool SortList(int nCol, bool bAscending);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to set extended styles for the list
	//     control, ie: LVS_EX_FULLROWSELECT, LVS_EX_GRIDLINES, etc. See MSDN
	//     documentation for a complete list of available styles.
	// Parameters:
	//     dwExStyle  - DWORD value that specifies the extended list-view control style. This
	//                  parameter can be a combination of Extended List-View Styles.
	//-----------------------------------------------------------------------
	virtual void SetExtendedStyle(DWORD dwExStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to return the list control extended
	//     style.
	// Returns:
	//     A DWORD value that represents the extended style of the list control.
	//-----------------------------------------------------------------------
	virtual DWORD GetExtendedStyle();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable column sizing.
	// Parameters:
	//     bDisable - true to disable column sizing. false to enable.
	//-----------------------------------------------------------------------
	virtual void DisableColumnSizing(bool bDisable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set a minimum column size.
	// Parameters:
	//     nSize - Size in pixels of the column's minimum size.
	//-----------------------------------------------------------------------
	void SetMinimumColSize(int nSize);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the minimum column size for the list
	//     control.
	// Returns:
	//     An integer value that represents the minimum column size for the list control.
	//-----------------------------------------------------------------------
	int GetMinimumColSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get the maximum column size for the list control.
	// Returns:
	//     An integer value that represents the maximum column size for the list control.
	//-----------------------------------------------------------------------
	int GetMaximumColSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine the row index and column
	//     index for a 'point'.
	// Parameters:
	//     point - Point to be tested.
	//     col   - Address of the column index
	// Returns:
	//     The row index, or -1 if 'point' is not over a row.
	//-----------------------------------------------------------------------
	virtual int HitTestEx(CPoint &point,int *col) const;

	// --------------------------------------------------------------------------
	// Summary:
	//     Call this member function to add a column after the right most
	//     column.
	// Parameters:
	//     lpszColHeading -  The column's heading.
	// nWidth -          Width of the column in pixels. If this parameter is \-1,
	//                       the column width is the same as the previous column.
	// nFormat -         The alignment of the column. It can be LVCFMT_LEFT,
	//                       LVCFMT_RIGHT, or LVCFMT_CENTER.
	// Returns:
	//     The index of the new column if successful, or -1 if unsuccessful.
	// --------------------------------------------------------------------------
	virtual int AddColumn(LPCTSTR lpszColHeading,int nWidth = -1,int nFormat=LVCFMT_LEFT);

	// ---------------------------------------------------------------------------
	// Summary:
	//     This member function will build the columns for the list control.
	// Parameters:
	//     nCols -         [in] Number of columns contained in the list control.
	//     nWidth -        [in] 'int' array containing the column widths.
	//     strColString -  [in] CString array containing the string for each
	//                     column.
	//     nColString -    [in] 'int' array containing the string resource ID for each
	//                     column.
	// Remarks:
	//     Call this member function to automatically generate the columns
	//     for the list control. This is done by passing in an array of
	//     column indexes specified by <i>nCols</i>, an array of column
	//     widths specified by <i>nWidth</i> and and array of column labels
	//     specified by <i>nColString</i> or <i>strColString</i>.
	// Example:
	// <code>
	// static int arColWidths[] = {
	//     24,
	//     21,
	//     100,
	//     180,
	//     120
	// };
	//
	// static CString arColLabels[] = {
	//     _T(" "),
	//     _T(" "),
	//     _T("From"),
	//     _T("Subject"),
	//     _T("Received")
	// };
	//
	// // Insert the columns.
	// BuildColumns(_countof(arColWidths), arColWidths, arColLabels);
	//     </code>
	// Returns:
	//     true if successful, or false if an error occurred.
	// ---------------------------------------------------------------------------
	virtual bool BuildColumns(int nCols, int* nWidth, int* nColString);
	virtual bool BuildColumns(int nCols, int* nWidth, CString* strColString); //<combine CXTListBase::BuildColumns@int@int*@int*>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets the number of columns in the list.
	// Returns:
	//     An integer value that represents the number of columns in the list.
	//-----------------------------------------------------------------------
	virtual int GetColumnCount();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function copies a row to a new location.
	// Parameters:
	//     nFrom - Index of the source row.
	//     nTo   - Index of the destination row.
	// Returns:
	//     The index of the source row specified by 'nFrom'.
	//-----------------------------------------------------------------------
	virtual int CopyRow(int nFrom,int nTo);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function moves a row to a new location.
	// Parameters:
	//     nFrom - Index of the source row
	//     nTo   - Index of the destination row
	// Returns:
	//     true if successful, or false if an error occurred.
	//-----------------------------------------------------------------------
	virtual bool MoveRow(int nFrom,int nTo);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is a utility function to retrieve the associated
	//     header control.
	// Returns:
	//     A CHeaderCtrl pointer to the associated header control.
	//-----------------------------------------------------------------------
	CHeaderCtrl *_xtGetHeaderCtrl() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the row, text, and background colors for
	//     the list control.
	// Parameters:
	//     clrText   - RGB value representing the row text color.
	//     clrTextBk - RGB value representing the row background color.
	//-----------------------------------------------------------------------
	virtual void SetRowColors(COLORREF clrText,COLORREF clrTextBk);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the text, and background colors for
	//     the specified row in the list control.
	// Parameters:
	//     iRow   - Index of the row to set color for.
	//     crText - RGB value representing the row text color.
	//     crBack - RGB value representing the row background color.
	//-----------------------------------------------------------------------
	virtual void SetRowColor(int iRow, COLORREF crText, COLORREF crBack);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the text, and background colors for
	//     the specified row in the list control.
	// Parameters:
	//     lpRowColor - Points to a ROWCOLOR that contains the RGB row
	//                  color information.
	//-----------------------------------------------------------------------
	virtual void SetRowColor(ROWCOLOR* lpRowColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove all user defined row colors
	//     from the list control.
	//-----------------------------------------------------------------------
	virtual void RemoveRowColors();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the specified row
	//     contains user defined color attributes.
	// Parameters:
	//     iRow - Index of the row to check.
	// Returns:
	//     A pointer to a ROWCOLOR structure if successful, otherwise
	//     NULL.
	//-----------------------------------------------------------------------
	virtual ROWCOLOR* Lookup(int iRow);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to modify the extended style for the
	//     list control.
	// Parameters:
	//     dwRemove - Specifies window styles to be removed during style
	//                modification.
	//     dwAdd    - Specifies window styles to be added during style
	//                modification.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL ModifyExtendedStyle(DWORD dwRemove, DWORD dwAdd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the RGB color value used
	//     for the column background color.
	// Returns:
	//     Returns an RGB value representing the column background color.
	//-----------------------------------------------------------------------
	COLORREF GetListBackColor() const;

	// ----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the RGB color value used for the
	//     list background color.
	// Parameters:
	//     crListBack :  An RGB value representing the list background color.
	// ----------------------------------------------------------------------
	void SetListBackColor(COLORREF crListBack);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the RGB color value used
	//     for the column text color.
	// Returns:
	//     Returns an RGB value representing the column text color.
	//-----------------------------------------------------------------------
	COLORREF GetListTextColor() const;

	// ---------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the RGB color value used for the
	//     list text color.
	// Parameters:
	//     crListText :  An RGB value representing the list text color.
	// ---------------------------------------------------------------------
	void SetListTextColor(COLORREF crListText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the RGB color value used
	//     for the sorted column background color.
	// Returns:
	//     Returns an RGB value representing the sorted column background color.
	//-----------------------------------------------------------------------
	COLORREF GetSortBackColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the RGB color value used
	//     for the sorted column background color.
	// Parameters:
	//     crSortBack - An RGB value representing the sorted column background color.
	//-----------------------------------------------------------------------
	void SetSortBackColor(COLORREF crSortBack);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the RGB color value used
	//     for the sorted column text color.
	// Returns:
	//     Returns an RGB value representing the sorted column text color.
	//-----------------------------------------------------------------------
	COLORREF GetSortTextColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the RGB color value used
	//     for the sorted column text color.
	// Parameters:
	//     crSortText - An RGB value representing the sorted column text color.
	//-----------------------------------------------------------------------
	void SetSortTextColor(COLORREF crSortText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable sorted column
	//     colors.
	// Parameters:
	//     bEnable - true to enable sorted column colors, false to disable.
	//-----------------------------------------------------------------------
	void EnableUserSortColor(bool bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if sorted column color
	//     is enabled.
	// Returns:
	//     true if sorted column colors are enabled, otherwise false.
	//-----------------------------------------------------------------------
	bool IsUserSortColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable user defined
	//     colors.
	// Parameters:
	//     bEnable - true to enable user defined colors, false to disable.
	//-----------------------------------------------------------------------
	void EnableUserListColor(bool bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if user defined colors
	//     are enabled.
	// Returns:
	//     true if user defined colors are enabled, otherwise false.
	//-----------------------------------------------------------------------
	bool IsUserListColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable user defined row
	//     colors.
	// Parameters:
	//     bEnable - true to enable user defined row colors, false to disable.
	//-----------------------------------------------------------------------
	void EnableUserRowColor(bool bEnable);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if user defined row colors
	//     are enabled.
	// Returns:
	//     true if user defined row colors are enabled, otherwise false.
	//-----------------------------------------------------------------------
	bool IsUserRowColor() const;

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to initialize the control.
	//-----------------------------------------------------------------------
	virtual void InitializeControl();

	// message implementation

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the list control pointer.
	// Parameters:
	//     pListCtrl - Points to the CListCtrl object to attach.
	// Remarks:
	//     This member function is called by the derived class to
	//     set the list control pointer for this implementation.
	//-----------------------------------------------------------------------
	void ImplAttach(CListCtrl *pListCtrl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Called from the derived class OnNotify method.
	// Parameters:
	//     wParam  - Identifies the control that sends the message if the
	//               message is from a control. Otherwise, wParam is 0.
	//     lParam  - Pointer to a notification message (NMHDR) structure
	//               that contains the notification code and additional
	//               information. For some notification messages, this
	//               parameter points to a larger structure that has the
	//               NMHDR structure as its first member.
	//     pResult - Pointer to an LRESULT variable in which to store the
	//               result code if the message is handled.
	// Remarks:
	//     The derived calls this member function
	//     to processes the message map for control notification, the
	//               derived class should call this member function from
	//               the CWnd::OnNotify override.
	// Returns:
	//     An application returns nonzero if it processes this message; otherwise
	//               0.
	//-----------------------------------------------------------------------
	virtual BOOL OnNotifyImpl(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//-------------------------------------------------------------------------
	// Summary:
	//     Updates colors and size values.
	// Remarks:
	//     This member function is called whenever there is a need to
	//     refresh the colors and size values that are set using system
	//     values. Usually called when the operating system color or
	//     settings change.
	//-------------------------------------------------------------------------
	void RefreshMetrics();

	//-------------------------------------------------------------------------
	// Summary:
	//     Called from the derived class PreSubclassWindow method.
	// Remarks:
	//     This member function is called by the derived class to allow other
	//     necessary subclassing to occur before the window is subclassed.
	//     Overriding this member function allows for dynamic subclassing
	//               of controls. It is an advanced overridable.
	//-------------------------------------------------------------------------
	void PreSubclassWindowImp();

	//-----------------------------------------------------------------------
	// Summary:
	//     Called from the derived class OnCreate method.
	// Parameters:
	//     lpCreateStruct - Points to a CREATESTRUCT structure that contains
	//               information about the CWnd object being created.
	// Remarks:
	//     The derived calls this member function
	//     when an application requests that the Windows window be created
	//     by calling the
	//               Create or CreateEx member function.
	// Returns:
	//     must return 0 to continue the creation of the CWnd object. If the application returns –1, the window will be destroyed.
	//-----------------------------------------------------------------------
	int OnCreateImpl(LPCREATESTRUCT lpCreateStruct);

protected:
//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_MSG
	afx_msg void OnCustomDrawImpl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroyImpl_Pre();
	afx_msg BOOL OnEraseBkgndImpl(CDC* pDC);
	afx_msg void OnPaintImpl();
	afx_msg void OnSettingChangeImpl(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnInitControl(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE


protected:
//{{AFX_CODEJOCK_PRIVATE
	// Needed in order to access protected functions.
	class CListCtrl_Friendly : public CXTListCtrlHelper { friend class CXTListBase; };
//}}AFX_CODEJOCK_PRIVATE

	int                 m_nMinColWidth; // Minimum width for columns.
	int                 m_nMaxColWidth; // Maximum width for columns.
	int                 m_nSortedCol;   // Index of the currently selected column.
	bool                m_bAutoFont;    // If true, the font will automatically be set for the control
	bool                m_bAutoSave;    // true if saving the column widths.
	bool                m_bListColor;   // true if user defined colors are used for background and text.
	bool                m_bRowColor;    // true if rows are drawn with user defined colors.
	bool                m_bSortColor;   // true if the sorted column is drawn in color.
	bool                m_bAscending;   // Flag to determine the sorting order.
	bool                m_bNoColSizing; // If true, column sizing is disabled.
	CString             m_strSection;   // Registry section name.
	CString             m_strEntry;     // Registry entry name.
	CString             m_strDefault;   // Registry default value.
	COLORREF            m_crListText;   // RGB value representing the row text color.
	COLORREF            m_crListBack;   // RGB value representing the row text background color.
	COLORREF            m_crSortText;   // RGB value for sort column text color.
	COLORREF            m_crSortBack;   // RGB value for sort column background color.
	CRowColorList     m_arRowColor;     // List of user defined text and background row colors.
	CXTFlatHeaderCtrl   m_flatHeader;   // Subclassed flat header control.
	CListCtrl_Friendly* m_pListCtrl;    // List control instance.
	bool m_bInitControl;                // true for initialization.
};

//===========================================================================

AFX_INLINE DWORD CXTListBase::GetExtendedStyle() {
	ASSERT(::IsWindow(m_pListCtrl->GetSafeHwnd())); return (DWORD)m_pListCtrl->SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
}
AFX_INLINE void CXTListBase::SetExtendedStyle(DWORD dwExStyle) {
	ASSERT(::IsWindow(m_pListCtrl->GetSafeHwnd())); m_pListCtrl->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, GetExtendedStyle()|dwExStyle);
}
AFX_INLINE void CXTListBase::SetMinimumColSize(int nSize) {
	m_nMinColWidth = nSize; GetFlatHeaderCtrl()->SetMinSize(nSize);
}
AFX_INLINE int CXTListBase::GetMinimumColSize() {
	return m_nMinColWidth;
}
AFX_INLINE int CXTListBase::GetMaximumColSize() {
	return m_nMaxColWidth;
}
AFX_INLINE void CXTListBase::DisableColumnSizing(bool bDisable) {
	m_bNoColSizing = bDisable;
}
AFX_INLINE void CXTListBase::SetListBackColor(COLORREF crListBack) {
	m_crListBack = crListBack;
}
AFX_INLINE void CXTListBase::SetListTextColor(COLORREF crListText) {
	m_crListText = crListText;
}
AFX_INLINE void CXTListBase::EnableUserListColor(bool bEnable) {
	m_bListColor = bEnable;
}
AFX_INLINE void CXTListBase::SetRowColors(COLORREF clrText, COLORREF clrTextBk) {
	SetListTextColor(clrText); SetListBackColor(clrTextBk); EnableUserListColor(true); m_pListCtrl->RedrawWindow();
}
AFX_INLINE COLORREF CXTListBase::GetListBackColor() const {
	return m_bListColor ? ((m_crListBack == (COLORREF)-1) ? ::GetSysColor(COLOR_WINDOW) : m_crListBack) : ::GetSysColor(COLOR_WINDOW);
}
AFX_INLINE COLORREF CXTListBase::GetListTextColor() const {
	return m_bListColor ? ((m_crListText == (COLORREF)-1) ? ::GetSysColor(COLOR_WINDOWTEXT) : m_crListText) : ::GetSysColor(COLOR_WINDOWTEXT);
}
AFX_INLINE COLORREF CXTListBase::GetSortBackColor() const {
	return (m_crSortBack == (COLORREF)-1)?GetListBackColor():m_crSortBack;
}
AFX_INLINE void CXTListBase::SetSortBackColor(COLORREF crSortBack) {
	m_crSortBack = crSortBack;
}
AFX_INLINE COLORREF CXTListBase::GetSortTextColor() const {
	return (m_crSortText == (COLORREF)-1)?GetListTextColor():m_crSortText;
}
AFX_INLINE void CXTListBase::SetSortTextColor(COLORREF crSortText) {
	m_crSortText = crSortText;
}
AFX_INLINE void CXTListBase::EnableUserSortColor(bool bEnable) {
	m_bSortColor = bEnable;
}
AFX_INLINE bool CXTListBase::IsUserSortColor() const {
	return m_bSortColor;
}
AFX_INLINE bool CXTListBase::IsUserListColor() const {
	return m_bListColor;
}
AFX_INLINE void CXTListBase::EnableUserRowColor(bool bEnable) {
	m_bRowColor = bEnable;
}
AFX_INLINE bool CXTListBase::IsUserRowColor() const {
	return m_bRowColor;
}

//{{AFX_CODEJOCK_PRIVATE
#define DECLATE_LIST_BASE(ClassName, List, Base)\
class _XT_EXT_CLASS ClassName : public List, public Base\
{\
public:\
	DWORD GetExtendedStyle() {\
		return Base::GetExtendedStyle();\
	}\
	void SetExtendedStyle(DWORD dwExStyle){\
		Base::SetExtendedStyle(dwExStyle);\
	}\
protected:\
	void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)  {\
		OnCustomDrawImpl(pNMHDR, pResult);\
	}\
	BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)   {\
		return OnNotifyImpl(wParam, lParam, pResult);\
	}\
	void OnSettingChange(UINT uFlags, LPCTSTR lpszSection)  {\
		OnSettingChangeImpl(uFlags, lpszSection);\
	}\
	void OnPaint() {\
		OnPaintImpl();\
	}   \
	BOOL OnEraseBkgnd(CDC* ){\
		return TRUE;\
	}   \
	void OnDestroy(){\
		OnDestroyImpl_Pre();\
		List::OnDestroy();\
	}\
	void PreSubclassWindow() {\
		PreSubclassWindowImp();\
	} \
	int OnCreate(LPCREATESTRUCT lpCreateStruct) {\
		if ((int)List::OnCreate(lpCreateStruct) == -1)\
			return -1;\
		return OnCreateImpl(lpCreateStruct);\
	}\
	LRESULT OnInitControl(WPARAM, LPARAM ) {\
		if (m_bInitControl)\
		{\
			m_bInitControl = false;\
			InitializeControl();\
		}\
		return 0;\
	}\
};

#define ON_LISTCTRL_REFLECT\
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)\
	ON_WM_DESTROY()\
	ON_WM_ERASEBKGND()\
	ON_WM_PAINT()\
	ON_WM_CREATE()\
	ON_MESSAGE(XTWM_INITIAL_UPDATE, OnInitControl)\
	ON_WM_SETTINGCHANGE
//}}AFX_CODEJOCK_PRIVATE

#endif // #if !defined(__XTLISTBASE_H__)
