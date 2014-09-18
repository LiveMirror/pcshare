// ACDSeeTheme.h
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

#pragma once

using namespace XTPPaintThemes;

class CACDSeeTheme : public CXTPDefaultTheme
{
public:
	CACDSeeTheme();
	~CACDSeeTheme();

	static void _FillRect(CDC* pDC, CWnd* pWnd, HBITMAP hbm);


protected:
	void FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar);
	void FillDockBar(CDC* pDC, CXTPDockBar* pBar);
	void RefreshMetrics();

private:
	BOOL InitBitmaps();


public:
	static HBITMAP m_hbmImageBackground;
	static HBITMAP m_hbmImageFill;

	static HBITMAP m_hbmTabCtrlBackground;
	static HBITMAP m_hbmTabCtrlSelected;

//  IDB_TABCTRLBACK
};
