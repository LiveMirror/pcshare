// XTFlatControlsTheme.h: interface for the CXTFlatControlsTheme class.
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
#if !defined(__XTFLATCONTROLSTHEME_H__)
#define __XTFLATCONTROLSTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonPro.Include\XTPDrawHelpers.h"
#include "XTThemeManager.h"

class CXTFlatComboBox;
class CXTFlatEdit;

DECLARE_THEME_FACTORY(CXTFlatControlsTheme)

//===========================================================================
// Summary:
//     CXTTabCtrlTheme is used to draw the CXTTabCtrl object.  All themes
//     used for CXTTabCtrl should inherit from this base class.
//===========================================================================
class _XT_EXT_CLASS CXTFlatControlsTheme : public CXTThemeManagerStyle
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTFlatControlsTheme object.
	//-------------------------------------------------------------------------
	CXTFlatControlsTheme();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTFlatControlsTheme object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	~CXTFlatControlsTheme();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     This function refreshes the colors on the flat control.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw a rectangle around a ComboBox.
	// Parameters:
	//     pDC    - A CDC pointer that represents the current device
	//              context.
	//     pCombo - A pointer to a CXTFlatComboBox object.  Represents a flat combo box.
	//     eState - An int that is used to specify the state of the flat combo box.
	//              Possible values are: NORMAL, RAISED, and PRESSED.
	//-----------------------------------------------------------------------
	virtual void DrawFlatComboBox(CDC* pDC, CXTFlatComboBox* pCombo, int eState );

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw a rectangle around an EditBox.
	// Parameters:
	//     pDC    - A CDC pointer that represents the current device
	//              context.
	//     pEdit  - A pointer to an edit box control.
	//     eState - An int that is used to specify the state of the flat edit box.
	//-----------------------------------------------------------------------
	virtual void DrawFlatEdit(CDC* pDC, CXTFlatEdit* pEdit, int eState );

protected:

	friend class CXTThemeManager;  // Make the CXTThemeManager a friend of the CXTFlatControlsTheme class.

};

//===========================================================================
// Summary:
//     CXTFlatControlsThemeOfficeXP class is derived from CXTFlatControlsTheme.
//     This class is used to create the Office 2000 Theme for flat controls.
//===========================================================================
class CXTFlatControlsThemeOfficeXP : public CXTFlatControlsTheme
{
public:
	//--------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTFlatControlsThemeOfficeXP object.
	//--------------------------------------------------------------------
	CXTFlatControlsThemeOfficeXP();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This function is called to refresh the colors of the
	//     flat controls Office XP theme.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to draw a rectangle around a ComboBox.
	// Parameters:
	//     pDC    - A CDC pointer that represents the current device
	//              context.
	//     pCombo - A pointer to a CXTFlatComboBox object.  Represents a flat combo box.
	//     eState - An int that is used to specify the state of the flat combo box.
	//              Possible values are: NORMAL, RAISED, and PRESSED.
	//-----------------------------------------------------------------------
	void DrawFlatComboBox(CDC* pDC, CXTFlatComboBox* pCombo, int eState );

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to render the flat edit control using
	//     the specified theme.
	// Parameters:
	//     pDC    - A CDC pointer that represents the current device
	//              context.
	//     pEdit  - A pointer to an edit box control.
	//     eState - An int that is used to specify the state of the flat edit box.
	//-----------------------------------------------------------------------
	void DrawFlatEdit(CDC* pDC, CXTFlatEdit* pEdit, int eState );

public:
	CXTPPaintManagerColor m_clrHighlightText;   // Used to specify the color of highlighted text.
	CXTPPaintManagerColor m_clrPushedText;      // Used to specify the color of pushed text.
	CXTPPaintManagerColor m_clrHighlight;       // Used to specify the color a highlighted object.
	CXTPPaintManagerColor m_clrPushed;          // Used to specify the color a pushed object.
	CXTPPaintManagerColor m_clrFrameNormal;     // Used to specify the color a normal frame.
	CXTPPaintManagerColor m_clrFrameHighlight;  // Used to specify the color a highlighted frame.
	CXTPPaintManagerColor m_clrFrameEdit;       // Used to specify the color an edit frame.
	CXTPPaintManagerColor m_clrNormal;          // Used to specify the color a normal object.
};

//===========================================================================
// Summary:
//     CXTFlatControlsThemeOffice2003 class is derived from CXTFlatControlsThemeOfficeXP.
//     This class is used to create the Office 2003 Theme for flat controls.
//===========================================================================
class CXTFlatControlsThemeOffice2003 : public CXTFlatControlsThemeOfficeXP
{
public:
	//--------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTFlatControlsThemeOffice2003 object.
	//--------------------------------------------------------------------
	CXTFlatControlsThemeOffice2003();

public:
	//--------------------------------------------------------------------
	// Summary:
	//     This function is called to refresh the colors of
	//     the flat controls Office 2003 theme.
	//--------------------------------------------------------------------
	void RefreshMetrics();
};


/////////////////////////////////////////////////////////////////////////////

#endif // !defined(__XTFLATCONTROLSTHEME_H__)
