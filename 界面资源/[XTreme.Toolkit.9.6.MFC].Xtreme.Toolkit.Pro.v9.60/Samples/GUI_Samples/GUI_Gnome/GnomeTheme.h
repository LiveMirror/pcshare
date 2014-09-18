// GnomeTheme.h: interface for the CGnomeTheme class.
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

#if !defined(AFX_GNOMETHEME_H__7001C63A_D7DB_40D4_93B5_2FFE7FD846DA__INCLUDED_)
#define AFX_GNOMETHEME_H__7001C63A_D7DB_40D4_93B5_2FFE7FD846DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define GNOMECOLOR_3DFACE RGB(230, 230, 230)
#define GNOMECOLOR_3DSHADOW RGB(153, 153, 153)
#define GNOMECOLOR_BTNHIGHLIGHT RGB(255, 255, 255)


class CGnomeTheme : public CXTPDefaultTheme
{
public:
	CGnomeTheme();
	virtual ~CGnomeTheme();

	virtual void FillStatusBar(CDC* pDC, CXTPStatusBar* pBar);
	virtual void FillDockBar(CDC* pDC, CXTPDockBar* pBar);
	virtual void DrawStatusBarGripper(CDC* pDC, CRect rcClient);
	virtual void DrawStatusBarPane(CDC* pDC, CRect& rc, DWORD dwStyle, CString str);

	virtual void FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar);
	CSize DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw);

	CSize DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw);
	void DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, BOOL bToolBar, XTPBarPosition barPosition = xtpBarPopup);

	void AdjustExcludeRect(CRect& rc, CXTPControl* pControl);


};

#endif // !defined(AFX_GNOMETHEME_H__7001C63A_D7DB_40D4_93B5_2FFE7FD846DA__INCLUDED_)
