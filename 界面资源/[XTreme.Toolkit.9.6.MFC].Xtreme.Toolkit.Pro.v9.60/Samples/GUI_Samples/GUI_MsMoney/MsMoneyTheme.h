// MsMoneyTheme.h: interface for the CMsMoneyTheme class.
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

#if !defined(AFX_MSMONEYTHEME_H__CE425EF4_949E_4119_A4AD_BBE0F0FC7C8D__INCLUDED_)
#define AFX_MSMONEYTHEME_H__CE425EF4_949E_4119_A4AD_BBE0F0FC7C8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMsMoneyTheme : public CXTPDefaultTheme
{
public:
	CMsMoneyTheme(COLORREF clrGradientLight, COLORREF clrGradientDark, COLORREF clrLineLight, COLORREF clrLineDark);
	virtual ~CMsMoneyTheme();

	virtual void FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar);
	virtual CSize DrawControlPopupParent(CDC* pDC, CXTPControl* pButton, BOOL bDraw);
	virtual CSize DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw);
	virtual void AdjustExcludeRect(CRect& rc, CXTPControl* pControl);
	virtual CSize DrawControlToolBarParent(CDC* pDC, CXTPControl* pButton, BOOL bDraw);
	virtual CRect GetCommandBarBorders(CXTPCommandBar* pBar);
	virtual CSize DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw);
	virtual void DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bEnabled = TRUE, BOOL bChecked = FALSE, BOOL bPopuped = FALSE, BOOL bToolBarImage = TRUE);
	virtual CSize DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType, CXTPControl* pButton, CXTPCommandBar* pBar, BOOL bDraw, LPVOID lpParam);

private:
	COLORREF m_clrGradientLight;
	COLORREF m_clrGradientDark;
	COLORREF m_clrLineLight;
	COLORREF m_clrLineDark;

};

#endif // !defined(AFX_MSMONEYTHEME_H__CE425EF4_949E_4119_A4AD_BBE0F0FC7C8D__INCLUDED_)
