// XTFontCombo.h interface for the CXTFontCombo class.
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
#if !defined(__XTFONTCOMBO_H__)
#define __XTFONTCOMBO_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// Summary:
//     CXTFontCombo is a CXTFlatComboBox derived class. It is used to create
//     a combo box that displays a drop list of available fonts for your system.
//     The fonts are displayed in their various styles.
//===========================================================================
class _XT_EXT_CLASS CXTFontCombo : public CXTFlatComboBox
{
	DECLARE_DYNAMIC(CXTFontCombo)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTFontCombo object
	//-----------------------------------------------------------------------
	CXTFontCombo();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTFontCombo object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTFontCombo();

public:
	// -------------------------------------------------------------------
	// Summary:
	//     Retrieves the selected font.
	// Parameters:
	//     lf -           Reference to an CXTLogFont structure.
	//     strFaceName -  A reference to a valid CString object to receive the
	//                    logfont face name.
	// Remarks:
	//     The first version of GetSelFont will retrieve the selected font
	//     name. The second version will get the CXTLogFont for the currently
	//     selected item.
	// Returns:
	//     true if successful, otherwise returns false.
	// -------------------------------------------------------------------
	virtual bool GetSelFont(CXTLogFont& lf);
	virtual bool GetSelFont(CString& strFaceName); //<combine CXTFontCombo::GetSelFont@CXTLogFont&>

	// ----------------------------------------------------------------------
	// Summary:
	//     Sets the selected font.
	// Parameters:
	//     lf -           Reference to an CXTLogFont object.
	//     strFaceName -  A NULL terminated string that represents the logfont
	//                    face name.
	// Remarks:
	//     The first version of SetSelFont will set the selected font by
	//     using its face name. The second version will set the selected font
	//     by using a CXTLogFont object.
	// Returns:
	//     true if successful, otherwise returns false.
	// ----------------------------------------------------------------------
	virtual bool SetSelFont(CXTLogFont& lf);
	virtual bool SetSelFont(CString strFaceName); //<combine CXTFontCombo::SetSelFont@CXTLogFont&>

	// --------------------------------------------------------------------------
	// Summary:
	//     Sets the font style for the CXTFontCombo control.
	// Parameters:
	//     eStyle -  Specifies the style for the font list box. Styles can be any
	//               one of the values in the Remarks section.
	// Remarks:
	//     Call this member function to set the font display style for the
	//     font list box. There are three styles to choose from that include
	//     displaying the font in the default GUI font, displaying the font
	//     in its own font style, or displaying both the font name in the
	//     default GUI font and a sample to the right.
	//
	//     The style can be any of the following values:
	//
	//     * <b>xtFontGUI</b> Display font name with GUI font style.
	//     * <b>xtFontSample</b> Display font name with its own font style.
	//     * <b>xtFontBoth</b> Display font name with GUI font style,
	//           then a sample display to the right.
	// --------------------------------------------------------------------------
	void SetListStyle(XTFontStyle eStyle);

	// ------------------------------------------------------------------------
	// Summary:
	//     Initializes the CXTFontCombo control.
	// Parameters:
	//     lpszFaceName -  A NULL terminated string that represents the logfont
	//                     face name.
	// nWidth -        The minimum allowable width of the list box portion of
	//                     the combo box in pixels.
	// bEnable -       TRUE to enable auto completion, otherwise FALSE.
	// Remarks:
	//     Call this member function to initialize the font list box and
	//     populate it with a list of available fonts.
	// ------------------------------------------------------------------------
	virtual void InitControl(LPCTSTR lpszFaceName=NULL,UINT nWidth=0,BOOL bEnable=TRUE );

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTFontCombo)
	virtual void DrawItem(LPDRAWITEMSTRUCT);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTFontCombo)
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	int         m_cyHScroll;    // Represents system metrics for SM_CYHSCROLL.
	int         m_cyEdge;       // Represents system metrics for SM_CYEDGE.
	XTFontStyle      m_eStyle;       // Enumerated style indicating how to display the font list.
	CString     m_strSymbol;    // String displayed for symbol characters.
	CImageList  m_ilFontType;   // true type font image list.

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTFontCombo::SetListStyle(XTFontStyle eStyle) {
	m_eStyle = eStyle;
}

#endif // #if !defined(__XTFONTCOMBO_H__)
