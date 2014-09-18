// XTComboBoxEx.h : interface for the CXTComboBoxEx class.
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
#if !defined(__XTCOMBOBOXEX_H__)
#define __XTCOMBOBOXEX_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     The CXTComboBoxEx class is a CComboBox derived class. It extends the
//     combo box control by providing support for image lists.
// Remarks:
//     By using CXTComboBoxEx
//     to create combo box controls, you no longer need to implement your own
//     image drawing code. Instead, use CXTComboBoxEx to access images from
//     an image list.
//===========================================================================
class _XT_EXT_CLASS CXTComboBoxEx : public CComboBox
{
	DECLARE_DYNAMIC(CXTComboBoxEx)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTComboBoxEx object
	//-----------------------------------------------------------------------
	CXTComboBoxEx();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTComboBoxEx object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTComboBoxEx();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function creates the combo box and attaches it to the
	//     CXTComboBoxEx object.
	// Parameters:
	//     dwStyle    - Specifies the combination of combo box styles applied
	//                  to the combo box.
	//     rect       - A reference to a CRect object or RECT structure that
	//                  specifies the position and size of the combo box.
	//     pParentWnd - A pointer to a CWnd object that is the parent window
	//                  of the combo box (usually a CDialog). It must not
	//                  be NULL.
	//     nID        - Specifies the combo box’s control ID.
	// Returns:
	//     Nonzero if the operation was successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	//{{AFX_CODEJOCK_PRIVATE
#if _MSC_VER > 1200 //MFC 7.0
	using CComboBox::DeleteItem;
#endif //MFC 7.0
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function removes an item from a ComboBoxEx control.
	// Parameters:
	//     iIndex - Zero-based index of the item to be removed.
	// Returns:
	//     The number of items remaining in the control. If 'iIndex' is
	//     invalid, the function returns CB_ERR.
	//-----------------------------------------------------------------------
	int DeleteItem(int iIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves item information for a given
	//     ComboBoxEx item.
	// Parameters:
	//     pCBItem - A pointer to a COMBOBOXEXITEM structure that will
	//               receive the item information.
	// Returns:
	//     Nonzero if the operation was successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL GetItem(COMBOBOXEXITEM* pCBItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function inserts a new item in a ComboBoxEx control.
	// Parameters:
	//     pCBItem - A pointer to a COMBOBOXEXITEM structure that will
	//               receive the item information.  This structure contains
	//               callback flag values for the item.
	//     iItem          - Item index.
	//     nStringID      - String resource.
	//     lpszItem       - Item string.
	//     iIndent        - Amount in pixels the image is to be indented.
	//     iImage         - Image index.
	//     iSelectedImage - Selected image index.
	//     mask           - Style mask.
	// Returns:
	//     The index at which the new item was inserted if successful;
	//     otherwise returns -1.
	//-----------------------------------------------------------------------
	int InsertItem(const COMBOBOXEXITEM* pCBItem);
	int InsertItem(int iItem, UINT nStringID, int iIndent=0, int iImage=0, int iSelectedImage=0, UINT mask=CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE|CBEIF_INDENT);// <combine CXTComboBoxEx::InsertItem@const COMBOBOXEXITEM*>
	int InsertItem(int iItem, LPCTSTR lpszItem, int iIndent=0, int iImage=0, int iSelectedImage=0, UINT mask=CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE|CBEIF_INDENT); // <combine CXTComboBoxEx::InsertItem@const COMBOBOXEXITEM*>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets the attributes for an item in a
	//     ComboBoxEx control.
	// Parameters:
	//     pCBItem - A pointer to a COMBOBOXEXITEM structure that will
	//               receive the item information.
	// Returns:
	//     Nonzero if the operation was successful, otherwise returns zero.
	//-----------------------------------------------------------------------
	BOOL SetItem(const COMBOBOXEXITEM* pCBItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the extended styles control.
	// Returns:
	//     The DWORD value that contains the extended styles that are used for
	//     the combo box control.
	//-----------------------------------------------------------------------
	DWORD GetExtendedStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function sets extended styles within a combo box control.
	// Parameters:
	//     dwExMask   - A DWORD value that indicates which styles in 'dwExStyles'
	//                  are to be affected. Only the extended styles in
	//                  'dwExMask' will be changed. All other styles will
	//                  be maintained as is. If this parameter is zero,
	//                  then all of the styles in 'dwExStyles' will be affected.
	//     dwExStyles - A DWORD value that contains the combo box control extended styles
	//                  to set for the control.
	// Returns:
	//     A DWORD value that contains the extended styles previously
	//     used for the control.
	//-----------------------------------------------------------------------
	DWORD SetExtendedStyle(DWORD dwExMask, DWORD dwExStyles);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine if the text in the combo
	//     has changed by typing.
	// Returns:
	//     Nonzero if the user has typed in the control's edit box, otherwise
	//     returns zero.
	//-----------------------------------------------------------------------
	BOOL HasEditChanged();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get a pointer to the edit control for
	//     a combo box.
	// Returns:
	//     A pointer to a CEdit object.
	//-----------------------------------------------------------------------
	CEdit* GetEditCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get a pointer to a combo box control
	//     within a CXTComboBoxEx object.
	// Returns:
	//     A pointer to a CComboBox object.
	//-----------------------------------------------------------------------
	CComboBox* GetComboBoxCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve a pointer to the image list
	//     used by a CXTComboBoxEx control.
	// Returns:
	//     A pointer to a CImageList object. If it fails, this member function
	//     returns NULL.
	//-----------------------------------------------------------------------
	CImageList* GetImageList() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets an image list for a ComboBoxEx control.
	// Parameters:
	//     pImageList - A pointer to a CImageList object containing the
	//                  images to use with the CXTComboBoxEx control.
	// Returns:
	//     A pointer to a CImageList object containing the images previously
	//     used by the CXTComboBoxEx control.  Returns NULL if no image
	//     list was previously set.
	//-----------------------------------------------------------------------
	CImageList* SetImageList(CImageList* pImageList);

//{{AFX_CODEJOCK_PRIVATE
	int Dir(UINT attr, LPCTSTR lpszWildCard);                // Un-supported base class member function.
	int FindString(int nIndexStart, LPCTSTR lpszFind) const; // Un-supported base class member function.
	int AddString(LPCTSTR lpszString);                       // Un-supported base class member function.
	BOOL SetEditSel(int nStartChar, int nEndChar);           // Un-supported base class member function.
	int InsertString(int nIndex, LPCTSTR lpszString);        // Un-supported base class member function.
//}}AFX_CODEJOCK_PRIVATE

protected:
//{{AFX_CODEJOCK_PRIVATE
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTComboBoxEx::CXTComboBoxEx() {

}
AFX_INLINE DWORD CXTComboBoxEx::GetExtendedStyle() const {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, CBEM_GETEXTENDEDSTYLE, 0, 0);
}
AFX_INLINE DWORD CXTComboBoxEx::SetExtendedStyle(DWORD dwExMask, DWORD dwExStyles) {
	ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, CBEM_SETEXTENDEDSTYLE, (DWORD) dwExMask, (LPARAM) dwExStyles);
}
AFX_INLINE BOOL CXTComboBoxEx::HasEditChanged() {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, CBEM_HASEDITCHANGED, 0, 0);
}
AFX_INLINE CEdit* CXTComboBoxEx::GetEditCtrl() {
	ASSERT(::IsWindow(m_hWnd)); return (CEdit*) CEdit::FromHandle((HWND) ::SendMessage(m_hWnd, CBEM_GETEDITCONTROL, 0, 0));
}
AFX_INLINE CComboBox* CXTComboBoxEx::GetComboBoxCtrl() {
	ASSERT(::IsWindow(m_hWnd)); return (CComboBox*) CComboBox::FromHandle((HWND) ::SendMessage(m_hWnd, CBEM_GETCOMBOCONTROL, 0, 0));
}
AFX_INLINE CImageList* CXTComboBoxEx::SetImageList(CImageList* pImageList) {
	ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, CBEM_SETIMAGELIST, 0, (LPARAM)pImageList->GetSafeHandle()));
}
AFX_INLINE CImageList* CXTComboBoxEx::GetImageList() const {
	ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, CBEM_GETIMAGELIST, 0, 0));
}
AFX_INLINE BOOL CXTComboBoxEx::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
//---------------------------------------------------------------------------
// While CXTComboBoxEx derives from CComboBox, there are some
// CB_messages the underlying ComboBoxEx control doesn't support.
//---------------------------------------------------------------------------

AFX_INLINE int CXTComboBoxEx::Dir(UINT attr, LPCTSTR lpszWildCard) {
	UNUSED_ALWAYS(attr); UNUSED_ALWAYS(lpszWildCard); ASSERT(FALSE); return CB_ERR;
}
AFX_INLINE int CXTComboBoxEx::FindString(int nIndexStart, LPCTSTR lpszFind) const {
	UNUSED_ALWAYS(nIndexStart); UNUSED_ALWAYS(lpszFind); ASSERT(FALSE); return CB_ERR;
}
AFX_INLINE int CXTComboBoxEx::AddString(LPCTSTR lpszString) {
	UNUSED_ALWAYS(lpszString); ASSERT(FALSE); return CB_ERR;
}
AFX_INLINE BOOL CXTComboBoxEx::SetEditSel(int nStartChar, int nEndChar) {
	UNUSED_ALWAYS(nStartChar); UNUSED_ALWAYS(nEndChar); ASSERT(FALSE); return FALSE;
}
AFX_INLINE int CXTComboBoxEx::InsertString(int nIndex, LPCTSTR lpszString) {
	UNUSED_ALWAYS(nIndex); UNUSED_ALWAYS(lpszString); ASSERT(FALSE); return CB_ERR;
}

#endif // #if !defined(__XTCOMBOBOXEX_H__)
