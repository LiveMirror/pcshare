// XTBrowseEdit.h : interface for the CXTBrowseEdit class.
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
#if !defined(__XTBROWSEEDIT_H__)
#define __XTBROWSEEDIT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTBrowseEdit;

// --------------------------------------------------------------------
// Summary:
//     CXTBrowseButton is a CXTButton derived class that is used
//     by CXTBrowseEdit to create the browse button that is used to
//     display file dialogs and popup menus.
// --------------------------------------------------------------------
class _XT_EXT_CLASS CXTBrowseButton : public CXTButton
{
	DECLARE_DYNAMIC(CXTBrowseButton)

public:

	// ----------------------------------------------
	// Summary:
	//     Constructs a CXTBrowseButton object
	// ----------------------------------------------
	CXTBrowseButton();

	// -----------------------------------------------------------------
	// Summary:
	//     Destroys a CXTBrowseButton object, handles cleanup and
	//     deallocation
	// -----------------------------------------------------------------
	virtual ~CXTBrowseButton();

public:

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function is called to create the browse button used by
	//     the edit control.
	// Parameters:
	//     pEditWnd -      [in] Parent edit control.
	//     dwBStyle -      [in] Search type
	//     nID -           [in] Resource id for push button
	//     nMenu -         [in] Popup menu id
	//     nSubMenuIndx -  [in] Index of popup submenu.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	// ----------------------------------------------------------------------
	virtual BOOL Create(CXTBrowseEdit* pEditWnd, DWORD dwBStyle, UINT nID, UINT nMenu = (UINT)-1, UINT nSubMenuIndx = (UINT)-1);

	// ----------------------------------------------------------------------------
	// Summary:
	//     This member function will set the browse style for the button.
	// Parameters:
	//     dwxStyle -  Specifies the browse style. The button style can be defined
	//                 as one or more of the styles listed in the Remarks section.
	// Remarks:
	//     The CXTBrowseButton can be set to use one or more of the following
	//     styles:
	//     * <b>BES_XT_CHOOSEDIR</b> Display the choose folder dialog.
	//     * <b>BES_XT_CHOOSEFILE</b> Display the choose file dialog.
	//     * <b>BES_XT_POPUPMENU</b> Display a user defined context menu.
	// See Also:
	//     SetXButtonStyle
	// ----------------------------------------------------------------------------
	void SetBrowseStyle(DWORD dwxStyle);



protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTItemEdit)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnClicked();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTBrowseButton)
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

protected:

	DWORD           m_dwBStyle;     // Search type
	UINT            m_nID;          // Resource id for push button
	UINT            m_nMenu;        // Popup menu id
	UINT            m_nSubMenuIndx; // Index of popup submenu.
	CXTBrowseEdit*  m_pEditWnd;     // Parent edit control.
	CXTIconHandle   m_hPopupIcon;   // Popup menu icon.

	friend class CXTBrowseEdit;
};

// --------------------------------------------------------------------
// Summary:
//     CXTBrowseEdit is a CXTEdit derived class. This class also
//     has a push button associated with it. The control can be used to
//     search for directories and files or activate a popup menu.
// --------------------------------------------------------------------
class _XT_EXT_CLASS CXTBrowseEdit : public CXTEdit
{
	DECLARE_DYNAMIC(CXTBrowseEdit)

public:

	// --------------------------------------------
	// Summary:
	//     Constructs a CXTBrowseEdit object
	// --------------------------------------------
	CXTBrowseEdit();

	// ---------------------------------------------------------------
	// Summary:
	//     Destroys a CXTBrowseEdit object, handles cleanup and
	//     deallocation
	// ---------------------------------------------------------------
	virtual ~CXTBrowseEdit();

public:

	// -----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if the browse edit control
	//     is in the middle of a browse operation.
	// Returns:
	//     true if the control is displaying a File Open dialog or popup menu.
	// -----------------------------------------------------------------------
	bool IsBrowsing();

	// -------------------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the gap between the browse button
	//     and the edit window.
	// Parameters:
	//     nGap -  [in] Gap, in pixels, between the browse button and the edit window.
	// -------------------------------------------------------------------------------
	void SetGap(int nGap);

	// ------------------------------------------------------------------------------
	// Summary:
	//     This member function will set a CFileDialog derived class object to
	//     be the file open dialog.
	// Parameters:
	//     pFileDialog -  [in] Points to the CFileDialog object that will replace the
	//                    standard File Open dialog.
	// ------------------------------------------------------------------------------
	virtual void SetFileDialog(CFileDialog* pFileDialog);

	// -------------------------------------------------------------------------------
	// Summary:
	//     This member function will set the File Open dialog style.
	// Parameters:
	//     bOpenFileDialog -  [in] Set to TRUE to construct a File Open dialog box, or
	//                        FALSE to construct a File Save as dialog box.
	// -------------------------------------------------------------------------------
	virtual void SetDlgOpenFile(BOOL bOpenFileDialog=TRUE);

	// -----------------------------------------------------------------------------
	// Summary:
	//     This member function sets the default extension for the File Open
	//     dialog.
	// Parameters:
	//     strDefExt -  [in] Points to a NULL terminated string that represents the
	//                  default file extension to be used with the File Open dialog.
	// -----------------------------------------------------------------------------
	virtual void SetDlgDefExt(LPCTSTR strDefExt=NULL);

	// ------------------------------------------------------------------------------
	// Summary:
	//     This member function sets the default file name for the File Open
	//     dialog.
	// Parameters:
	//     strFileName -  [in] Points to a NULL terminated string that represents the
	//                    default file name to be used with the File Open dialog.
	// ------------------------------------------------------------------------------
	virtual void SetDlgFileName(LPCTSTR strFileName=NULL);

	// ---------------------------------------------------------------------
	// Summary:
	//     This member function sets the style flags for the File Open
	//     dialog.
	// Parameters:
	//     dwFlags -  [in] The desired OFN_ styles for the File Open dialog.
	// ---------------------------------------------------------------------
	virtual void SetDlgStyle(DWORD dwFlags=OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT);

	// ----------------------------------------------------------------------------
	// Summary:
	//     This member function sets the file filter for the File Open
	//     dialog.
	// Parameters:
	//     strFilter -  [in] Points to a NULL terminated string that represents the
	//                  \file filter used by the File Open dialog.
	// ----------------------------------------------------------------------------
	virtual void SetDlgFilter(LPCTSTR strFilter=NULL);

	// --------------------------------------------------------------------------
	// Summary:
	//     This member function sets the title for the directory dialog.
	// Parameters:
	//     strTitle -  [in] Points to a NULL terminated string the represents the
	//                 title of the "browse for directory" dialog.
	// --------------------------------------------------------------------------
	virtual void SetDlgTitle(LPCTSTR strTitle=NULL);

	// -----------------------------------------------------------------------------
	// Summary:
	//     This member function is called to initialize the browse edit
	//     control.
	// Parameters:
	//     pParentWnd -    [in] Pointer to the parent of the edit control.
	//     dwBStyle -      [in] A <i>DWORD</i> value that represents the type of
	//                     search to perform. It can be any one of the styles listed
	//                     in the remarks section.
	//     nMenu -         [in] If <i>dwBStyle</i> contains the BES_XT_POPUPMENU flag,
	//                     then <i>nMenu</i> represents the resource ID of a popup
	//                     menu. Otherwise this value is ignored.
	//     nSubMenuIndx -  [in] Index of submenu to display.
	// Remarks:
	//     The Initialize function must be called once for each browse edit
	//     control to complete initialization. This should be done after you
	//     have subclassed or created the browse edit control.<p/>
	//
	//    The browse edit control can be defined to use any of the following
	//     styles:
	//     * <b>BES_XT_CHOOSEDIR</b> Display the choose folder dialog.
	//     * <b>BES_XT_CHOOSEFILE</b> Display the choose file dialog.
	//     * <b>BES_XT_POPUPMENU</b> Display a user defined context menu.
	//
	//     The single argument version of Initialize will call the CXTEdit
	//     base class to initialize the edit portion of the control only,
	//     generally you would not want to use this override for standard
	//     initialization.
	// Example:
	//  <code>
	//  BOOL CBrowseEditDlg::OnInitDialog()
	//  {
	//      CXTResizeDialog::OnInitDialog();
	//      ...
	//      // Initialize browse edit controls.
	//      m_edit1.Initialize(this, BES_XT_POPUPMENU, IDR_POPUP);
	//      m_edit2.Initialize(this, BES_XT_CHOOSEDIR);
	//      m_edit3.Initialize(this, BES_XT_CHOOSEFILE);
	//
	//      return TRUE;  // return TRUE  unless you set the focus to a control
	// }
	// </code>
	// Returns:
	//     TRUE if initialization was successful, otherwise FALSE.
	// -----------------------------------------------------------------------------
	virtual bool Initialize(CWnd* pParentWnd, DWORD dwBStyle, UINT nMenu=0, int nSubMenuIndx=0);
	virtual bool Initialize(CWnd* pParentWnd); //<COMBINE CXTBrowseEdit::Initialize@CWnd*@DWORD@UINT@int>

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function is called whenever the browse button is
	//     pressed, and can be overridden to perform custom browse functions.
	// ----------------------------------------------------------------------
	virtual void OnBrowse();

	//-------------------------------------------------------------------------
	// Summary:
	//     This member function is called by the browse edit control to
	//     display a directory chooser dialog.
	//-------------------------------------------------------------------------
	virtual void ChooseDirectory();

	//-------------------------------------------------------------------------
	// Summary:
	//     This member function is called by the browse edit control to
	//     display a File Open dialog.
	//-------------------------------------------------------------------------
	virtual void ChooseFile();

	// ---------------------------------------------------------------------
	// Summary:
	//     This member function is called by the browse edit control to
	//     create and track a pop-up menu. The menu selection is then copied
	//     to the edit control.
	// ---------------------------------------------------------------------
	virtual void PopupMenu();

	// -------------------------------------------------------------------------
	// Summary:
	//     This member function is called by the browse edit control to position
	//     the browse button in relation to the edit control.
	// -------------------------------------------------------------------------
	virtual void PositionBrowseButton();

protected:
//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_MSG(CXTBrowseEdit)
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//}}AFX_CODEJOCK_PRIVATE

protected:

	int              m_nGap;            // Distance between the button and edit control.
	bool             m_bBrowsing;       // true if in browse operation.
	BOOL             m_bOpenFileDialog; // TRUE for Open File dialog, FALSE for Save as. See SetDlgOpenFile.
	UINT             m_nMenu;           // Popup menu ID.
	UINT             m_nSubMenuIndx;    // Index of a popup submenu.
	DWORD            m_dwFlags;         // File dialog styles.
	DWORD            m_dwBStyle;        // Search type.
	CString          m_strDefExt;       // Default file extension.
	CString          m_strFileName;     // Default file name.
	CString          m_strFilter;       // Default file filter.
	CString          m_strTitle;        // Directory dialog title.
	CFileDialog*     m_pFileDialog;     // Points to a valid CFileDialog object.
	CXTBrowseButton  m_btnBrowse;       // Pointer to a push button.


};

//////////////////////////////////////////////////////////////////////
AFX_INLINE bool CXTBrowseEdit::Initialize(CWnd* pParentWnd) {
	return CXTEdit::Initialize(pParentWnd);
}
AFX_INLINE void CXTBrowseEdit::SetGap(int nGap) {
	ASSERT(nGap >= 0); m_nGap = nGap;
}
AFX_INLINE void CXTBrowseEdit::SetFileDialog(CFileDialog *pFileDialog/*=NULL*/) {
	m_pFileDialog = pFileDialog;
}
AFX_INLINE void CXTBrowseEdit::SetDlgOpenFile(BOOL bOpenFileDialog/*=TRUE*/) {
	m_bOpenFileDialog = bOpenFileDialog;
}
AFX_INLINE void CXTBrowseEdit::SetDlgDefExt(LPCTSTR strDefExt/*=NULL*/) {
	m_strDefExt = strDefExt;
}
AFX_INLINE void CXTBrowseEdit::SetDlgFileName(LPCTSTR strFileName/*=NULL*/) {
	m_strFileName = strFileName;
}
AFX_INLINE void CXTBrowseEdit::SetDlgStyle(DWORD dwFlags/*=OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT*/) {
	m_dwFlags = dwFlags;
}
AFX_INLINE void CXTBrowseEdit::SetDlgFilter(LPCTSTR strFilter/*=NULL*/) {
	m_strFilter = strFilter;
}
AFX_INLINE void CXTBrowseEdit::SetDlgTitle(LPCTSTR strTitle/*=NULL*/) {
	m_strTitle = strTitle;
}
AFX_INLINE bool CXTBrowseEdit::IsBrowsing() {
	return m_bBrowsing;
}

const DWORD BES_XT_CHOOSEDIR       = 0x0001;  //<ALIAS CXTBrowseEdit::Initialize@CWnd*@DWORD@UINT@int>
const DWORD BES_XT_CHOOSEFILE      = 0x0002;  //<ALIAS CXTBrowseEdit::Initialize@CWnd*@DWORD@UINT@int>
const DWORD BES_XT_POPUPMENU       = 0x0004;  //<ALIAS CXTBrowseEdit::Initialize@CWnd*@DWORD@UINT@int>

//===========================================================================
// Summary:
//     CXTItemEdit is a CXTBrowseEdit derived class.  It is used to create
//     a CXTItemEdit window that can be used as an "in-place" edit field that
//     can be dynamically created for controls such as a list box.
//===========================================================================
class _XT_EXT_CLASS CXTItemEdit : public CXTBrowseEdit
{
	DECLARE_DYNAMIC(CXTItemEdit)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTItemEdit object that can be used as an "in-place" edit
	//     field and can be dynamically created for controls such as a list box
	// Parameters:
	//     pParent       - Points to the parent window.
	//     rect          - Size of the edit item.
	//     strWindowText - Text to be initially displayed in the edit field.
	//     dwBStyle      - Specifies the browse edit style for the in-place
	//                     edit field. See CXTBrowseEdit::SetBrowseStyle
	//                     for available styles.
	//     bAutoDelete   - Set to true if the object is to be self deleting.
	//-----------------------------------------------------------------------
	CXTItemEdit(CWnd* pParent, const CRect& rect, CString& strWindowText,
		DWORD dwBStyle=BES_XT_CHOOSEDIR, bool bAutoDelete=true);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTItemEdit object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTItemEdit();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called whenever the control loses focus.
	//     This will destroy the window, and notify the parent via WM_COMMAND
	//     that the editing has been completed. The two possible commands are:
	//     ON_BEN_XT_LABELEDITEND and ON_BEN_XT_LABELEDITCANCEL
	//-----------------------------------------------------------------------
	virtual void EndLabelEdit();

protected:

//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_VIRTUAL(CXTItemEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTItemEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//}}AFX_CODEJOCK_PRIVATE

public:
	bool     m_bModified;     // true if the item was modified.
	bool     m_bAutoDelete;   // true if self deleting.
	bool     m_bEscapeKey;    // true if the edit window was closed with the escape key.
	CString& m_strWindowText; // The edit controls text.

protected:
	bool m_bClosePosted;    // Used internally to determine if a WM_CLOSE message has been sent.

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTBrowseButton::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTBrowseButton::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
	return CButton::Create(lpszCaption, dwStyle, rect, pParentWnd, nID);

}

#endif // #if !defined(__XTBROWSEEDIT_H__)
