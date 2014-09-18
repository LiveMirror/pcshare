// XTPPopupPaintManager.h: interface for the CXTPPopupPaintManager class.
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
#if !defined(_XTPPOPUPPAINTMANAGER_H__)
#define _XTPPOPUPPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonPro.include/XTPDrawHelpers.h"

class CXTPPopupItem;
class CXTPPopupItemText;
class CXTPPopupItemIcon;

//===========================================================================
// Summary:
//     CXTPPopupPaintManager is a class used to perform specialized
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPPopupPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPopupPaintManager object.
	//-----------------------------------------------------------------------
	CXTPPopupPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPopupPaintManager object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPopupPaintManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function will draw the background of popup window.
	// Parameters:
	//     pDC - Points to the device context.
	//     rcClient - Size of area to fill.
	//-----------------------------------------------------------------------
	virtual void DrawBackground(CDC* pDC, CRect rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function will draw the controlled item object
	//     of popup window.
	// Parameters:
	//     pDC - Points to the device context.
	//     pItem - point to CXTPPopupItem object.
	//-----------------------------------------------------------------------
	void DrawItem(CDC* pDC, CXTPPopupItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to draw the rectangle of a button.
	// Parameters:
	//     pDC   - Points to the device context.
	//     pItem - Pointer to a CXTPPopupItem.
	//-----------------------------------------------------------------------
	void DrawButton(CDC* pDC, CXTPPopupItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets font for text used in Popup control.
	// Parameters:
	//     hFont - Font handle to use.
	//-----------------------------------------------------------------------
	void SetFont(HFONT hFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to refresh draw metrics.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

public:
	CXTPPaintManagerColorGradient m_clrBackground;      //gradient background color
	CXTPPaintManagerColorGradient m_clrFrame;           //3d frame color

	CFont m_fntText;                        //default font of text
	COLORREF m_clrText;                     //default color of text

	COLORREF m_clrButtonSelected;           //color of selected button
	COLORREF m_clrButtonPressed;            //color of pressed button
	CXTPPaintManagerColorGradient m_clrButtonSelectedBorder;    //3d border color of selected button
	CXTPPaintManagerColorGradient m_clrButtonPressedBorder; //3d border color of pressed button
};

//===========================================================================
// Summary:
//     CXTPPopupThemeOffice2000 is a class used to perform Office2000
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPPopupThemeOffice2000 : public CXTPPopupPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPopupThemeOffice2000 object.
	//-----------------------------------------------------------------------
	CXTPPopupThemeOffice2000();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPopupThemeOffice2000 object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPopupThemeOffice2000();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to refresh draw metrics.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function will draw the background of popup window.
	// Parameters:
	//     pDC - Points to the device context.
	//     rcClient - Size of area to fill.
	//-----------------------------------------------------------------------
	void DrawBackground(CDC* pDC, CRect rcClient);
};

//===========================================================================
// Summary:
//     CXTPPopupThemeOfficeXP is a class used to perform OfficeXP
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPPopupThemeOfficeXP : public CXTPPopupThemeOffice2000
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPopupThemeOfficeXP object.
	//-----------------------------------------------------------------------
	CXTPPopupThemeOfficeXP();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPopupThemeOfficeXP object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPopupThemeOfficeXP();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to refresh draw metrics.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function will draw the background of popup window.
	// Parameters:
	//     pDC - Points to the device context.
	//     rcClient - Size of area to fill.
	//-----------------------------------------------------------------------
	void DrawBackground(CDC* pDC, CRect rcClient);
};

//===========================================================================
// Summary:
//     CXTPPopupThemeOffice2003 is a class used to perform Office2003
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPPopupThemeOffice2003 : public CXTPPopupThemeOfficeXP
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPopupThemeOffice2003 object.
	//-----------------------------------------------------------------------
	CXTPPopupThemeOffice2003();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPopupThemeOffice2003 object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPopupThemeOffice2003();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to refresh draw metrics.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function will draw the background of popup window.
	// Parameters:
	//     pDC - Points to the device context.
	//     rcClient - Size of area to fill.
	//-----------------------------------------------------------------------
	void DrawBackground(CDC* pDC, CRect rcClient);

public:
	CXTPPaintManagerColorGradient m_clrGripper; // Color of the popup gripper for the xtpPopupThemeOffice2003 paint theme.
};

//===========================================================================
// Summary:
//     CXTPPopupThemeMSN is a class used to perform MSN Messenger
//     drawing tasks.
//===========================================================================
class _XTP_EXT_CLASS CXTPPopupThemeMSN : public CXTPPopupThemeOffice2000
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPopupThemeMSN object.
	//-----------------------------------------------------------------------
	CXTPPopupThemeMSN();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPopupThemeMSN object.
	//-----------------------------------------------------------------------
	virtual ~CXTPPopupThemeMSN();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to refresh draw metrics.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function will draw the background of popup window.
	// Parameters:
	//     pDC - Points to the device context.
	//     rcClient - Size of area to fill.
	//-----------------------------------------------------------------------
	void DrawBackground(CDC* pDC, CRect rcClient);
};


#endif // !defined(_XTPPOPUPPAINTMANAGER_H__)
