// XTColorPopup.h : interface for the CXTColorPopup class.
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
#if !defined(__XTCOLORPOPUP_H__)
#define __XTCOLORPOPUP_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTColorPopup is a CXTColorSelectorCtrl derived class. It is used
//     to create a CXTColorPopup window that will allow a user to select
//     colors and add custom colors to a color list.
//===========================================================================
class _XT_EXT_CLASS CXTColorPopup : public CXTColorSelectorCtrl
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTColorPopup object.
	// Parameters:
	//     bAutoDelete - TRUE if the color picker window is to be self deleting.
	//     callerParam - Caller parameter; it will be reported back as an LPARAM of
	//     color popup notification messages.
	//-----------------------------------------------------------------------
	CXTColorPopup(BOOL bAutoDelete=FALSE, LPARAM callerParam = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTColorPopup object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTColorPopup();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will add a color to the user defined color
	//     list.
	// Parameters:
	//     clrColor - An RGB value that represents the user defined color
	//                to be displayed in
	//     the recent color list.
	//-----------------------------------------------------------------------
	static void AddUserColor(COLORREF clrColor);

	// ---------------------------------------------------------------------------
	// Summary:
	//     Resets the recently used color list.
	// Remarks:
	//     Call this member function to reset the user defined color list. Calling
	//     this function will remove all user defined colors from the MRU
	//     list.
	// ---------------------------------------------------------------------------
	static void ResetUserColors();

	// -----------------------------------------------------------------------------
	// Summary:
	//     This member function handles the creation of the color popup
	//     window.
	// Parameters:
	//     rect -        A reference to a <i>CRect</i> object that represents the
	//                   size of the color popup window.
	//     pParentWnd -  Points to the parent window for the color popup.
	//     dwPopup -     Style for the popup window. See the Remarks section for a
	//                   complete list of available styles.
	//     clrColor -    An <i>RGB</i> value that represents the currently selected
	//                   color for the popup window.
	//     clrDefault -  Specifies the default color for the color popup. If the
	//                   current style includes CPS_NOFILL, this parameter is
	//                   ignored.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:
	//     * <b>CPS_XT_NOFILL</b> The color picker will display a No Fill
	//           button rather than the default Automatic Color button.
	//     * <b>CPS_XT_EXTENDED</b> The color picker will display 40
	//           extended colors rather than the default 16 colors.
	//     * <b>CPS_XT_MORECOLORS</b> The color picker will display a
	//           More Colors button which will display a CXTColorDialog.
	//     * <b>CPS_XT_SHOW3DSELECTION</b> Displays the color selection
	//           box with a 3D raised border in CXTColorDialog.
	//     * <b>CPS_XT_SHOWHEXVALUE</b> Displays the hex equivalent of
	//           the selected color.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	// See Also:
	//     CXTColorPicker::ModifyCPStyle, CXTColorSelectorCtrl::Create
	// -----------------------------------------------------------------------------
	virtual BOOL Create(CRect& rect,CWnd* pParentWnd,DWORD dwPopup,COLORREF clrColor,COLORREF clrDefault=CLR_DEFAULT);

	// ---------------------------------------------------------------------
	// Summary:
	//     This member function adds a window that picker notifications are
	//     sent to.
	// Parameters:
	//     hwndListener -  A handle to the listener window. Messages will be
	//                     sent to it.
	// ---------------------------------------------------------------------
	void AddListener(HWND hwndListener);

	// --------------------------------------------------------------------------
	// Summary:
	//     Enables or disables popup window shadow.
	// Parameters:
	//     bDisplayShadow -  Flag that tells if the shadow is to be displayed.
	// Remarks:
	//     This member function enables or disables the drawing of popup shadows.
	//     Call this function after instantiating the color popup as a part
	//     \of an object's initialization. Do not call it after popup
	//     creation because doing so may cause unpredictable results.
	// --------------------------------------------------------------------------
	void DisplayShadow(BOOL bDisplayShadow = TRUE);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will finish the selection process for the color
	//     box or button in the color popup window.
	// Parameters:
	//     nCurSel - Current index of the selected color box or button
	//               in the color popup window.
	//-----------------------------------------------------------------------
	virtual void EndSelection(int nCurSel);

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTColorPopup)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTColorPopup)
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	//}}AFX_MSG
	#if _MSC_VER > 1200 //MFC 7.0
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
	#else
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	#endif //MFC 7.0
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:
	BOOL  m_bAutoDelete;        // TRUE if the popup window is to be self deleting.
	BOOL  m_bDisplayShadow;     // TRUE if the popup window will render its shadow.
	CRect m_rcExclude;          // Area to exclude from shadow display.
	const LPARAM m_callerParam; // Caller parameter that will be reported back as an LPARAM of all notification messages.

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE  void CXTColorPopup::AddUserColor(COLORREF clrColor) {
	m_arUserDefColors.Add( clrColor );
}
AFX_INLINE BOOL CXTColorPopup::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE void CXTColorPopup::ResetUserColors() {
	m_arUserDefColors.RemoveAll();
}
AFX_INLINE void CXTColorPopup::DisplayShadow( BOOL bDisplayShadow/* = TRUE*/) {
	m_bDisplayShadow = bDisplayShadow;
}

#endif //#if !defined(__XTCOLORPOPUP_H__)
