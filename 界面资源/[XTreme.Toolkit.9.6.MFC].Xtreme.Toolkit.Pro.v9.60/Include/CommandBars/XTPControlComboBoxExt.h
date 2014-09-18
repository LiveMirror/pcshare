// XTPControlComboBoxExt.h
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
#if !defined(__XTPCONTROLCOMBOBOXEXT_H__)
#define __XTPCONTROLCOMBOBOXEXT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//-----------------------------------------------------------------------
// Summary:
//     The XTP_FN_SETFORMAT notification sent by CXTPControlFontComboBox to set current RichEdit selection format
// Example:
//     Here is an example of how an application would process the XTP_FN_SETFORMAT
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CApplicationView, CRichEditView)
//     //{{AFX_MSG_MAP(CApplicationView)
//     ON_NOTIFY(XTP_FN_SETFORMAT, ID_EDIT_SIZE, OnSetCharFormat)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CApplicationView::OnSetCharFormat(NMHDR* pNMHDR, LRESULT* pResult)
// {
//     CHARFORMAT& cf = ((NMXTPCHARHDR*)pNMHDR)->cf;
//     SetCharFormat(cf);
//     *pResult = 1;
// }
// </code>
// See Also: XTP_FN_GETFORMAT, CXTPControlFontComboBox
//-----------------------------------------------------------------------
const XTP_FN_SETFORMAT    = 0x1000;

//-----------------------------------------------------------------------
// Summary:
//     The XTP_FN_GETFORMAT notification sent by CXTPControlFontComboBox to get current RichEdit selection format
// Example:
//     Here is an example of how an application would process the XTP_FN_GETFORMAT
//     message.
// <code>
// BEGIN_MESSAGE_MAP(CApplicationView, CRichEditView)
//     //{{AFX_MSG_MAP(CApplicationView)
//     ON_NOTIFY(XTP_FN_GETFORMAT, ID_EDIT_SIZE, OnGetCharFormat)
//     //}}AFX_MSG_MAP
// END_MESSAGE_MAP()
//
// void CApplicationView::OnGetCharFormat(NMHDR* pNMHDR, LRESULT* pResult)
// {
//     CHARFORMAT& cf = ((NMXTPCHARHDR*)pNMHDR)->cf;
//     cf = GetCharFormatSelection();
//     *pResult = 1;
// }
// </code>
// See Also: XTP_FN_SETFORMAT, CXTPControlFontComboBox
//-----------------------------------------------------------------------
const XTP_FN_GETFORMAT    = 0x1001;


//-----------------------------------------------------------------------
// Summary:
//     Structure used as parameter for XTP_FN_SETFORMAT and XTP_FN_GETFORMAT messages.
// Example: See Example of XTP_FN_SETFORMAT
// See Also: XTP_FN_SETFORMAT, XTP_FN_GETFORMAT, CXTPControlFontComboBox
//-----------------------------------------------------------------------
struct NMXTPCHARHDR : public NMXTPCONTROL
{
//{{AFX_CODEJOCK_PRIVATE
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a NMXTPCHARHDR object
	//-------------------------------------------------------------------------
	NMXTPCHARHDR() {cf.cbSize = sizeof(CHARFORMAT);}
//}}AFX_CODEJOCK_PRIVATE


	CHARFORMAT cf;          // Character formatting in a rich edit control.
};


//===========================================================================
// Summary:
//     CXTPControlSizeComboBox is a CXTPControlComboBox derived class. It
//     represents the size picker.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlSizeComboBox : public CXTPControlComboBox
{
	DECLARE_XTP_CONTROL(CXTPControlSizeComboBox)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlSizeComboBox object
	//-----------------------------------------------------------------------
	CXTPControlSizeComboBox();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControlSizeComboBox object, handles cleanup
	//     and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPControlSizeComboBox();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set the current size.
	// Parameters:
	//     nTwips - Size, in twips, to be set
	//-----------------------------------------------------------------------
	void SetTwipSize(int nTwips);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to popup the control.
	// Parameters:
	//     bPopup - TRUE to set popup.
	// Returns:
	//     TRUE if successful; otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL OnSetPopup(BOOL bPopup);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve the current user selected size.
	// Returns:
	//     The current size.
	//-----------------------------------------------------------------------
	int GetTwipSize();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the control is executed.
	//-----------------------------------------------------------------------
	virtual void OnExecute();

private:
	void EnumFontSizes(CDC& dc, LPCTSTR pFontName);

	static BOOL CALLBACK AFX_EXPORT EnumSizeCallBack(LOGFONT FAR* /*lplf*/,
		LPNEWTEXTMETRIC lpntm, int FontType, LPVOID lpv);

	void InsertSize(int nSize);
	void TwipsToPointString(LPTSTR lpszBuf, int nTwips);


private:
	int m_nLogVert;
	int m_nTwipsLast;
};

//===========================================================================
// Summary:
//     CXTPControlFontComboBoxList is a CXTPControlComboBoxList derived
//     class. It represents the font picker popup.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlFontComboBoxList : public CXTPControlComboBoxList
{
	DECLARE_XTP_COMMANDBAR(CXTPControlFontComboBoxList)

public:
//{{AFX_CODEJOCK_PRIVATE
	class CFontDesc;
	class CFontDescHolder;
//}}AFX_CODEJOCK_PRIVATE

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes this object from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//----------------------------------------------------------------------
	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to make a copy of the command bar.
	// Parameters:
	//     pCommandBar - Command bar needed to be copied.
	//     bRecursive - TRUE to copy recursively.
	//-----------------------------------------------------------------------
	void Copy(CXTPCommandBar* pCommandBar, BOOL bRecursive = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates a list box.
	//-----------------------------------------------------------------------
	void CreateListBox();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to add all fonts to list.
	// Parameters:
	//     dwStyleListBox - Flags to be used in EnumFontFamiliesEx. Currently not supported.
	//-----------------------------------------------------------------------
	void EnumFontFamiliesEx(BOOL dwStyleListBox = 1);

protected:
	DWORD m_dwStyleListBox;     // Style of the control.

private:
	friend class CXTPControlFontComboBox;

};

//===========================================================================
// Summary:
//     CXTPControlFontComboBox is a CXTPControlComboBox derived class. It
//     represents the font picker control.
//===========================================================================
class _XTP_EXT_CLASS CXTPControlFontComboBox : public CXTPControlComboBox
{
	DECLARE_XTP_CONTROL(CXTPControlFontComboBox)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPControlFontComboBox object
	// Parameters:
	//     dwStyleListBox - Flags to be used in list box. Currently not supported, must be default.
	//-----------------------------------------------------------------------
	CXTPControlFontComboBox(DWORD dwStyleListBox = 1);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPControl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPControlFontComboBox();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set current font of the picker.
	// Parameters:
	//     cf - CHARFORMAT structure contains information about character
	//          formatting in a rich edit control.
	//-----------------------------------------------------------------------
	void SetCharFormat(CHARFORMAT& cf);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called when the control is executed.
	//-----------------------------------------------------------------------
	virtual void OnExecute();

private:
	void MatchFont(LPCTSTR lpszName, BYTE nCharSet);


private:
	int m_nFontHeight;
};


#endif //#if !defined(__XTPCONTROLCOMBOBOXEXT_H__)
