// GUI_Outlook.h : main header file for the GUI_OUTLOOK application
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

#if !defined(AFX_GUI_OUTLOOK_H__62A35493_2F0F_4C47_BE10_A972BED2082D__INCLUDED_)
#define AFX_GUI_OUTLOOK_H__62A35493_2F0F_4C47_BE10_A972BED2082D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

inline COLORREF GetStaticFrameColor(BOOL bTopLeft)
{
	XTPPaintTheme theme = XTPPaintManager()->BaseTheme();
	const COLORREF clrFrame = theme == xtpThemeNativeWinXP? GetXtremeColor(XPCOLOR_STATICFRAME): theme == xtpThemeOffice2003?
		GetXtremeColor(XPCOLOR_FRAME): GetXtremeColor(bTopLeft?COLOR_3DSHADOW: COLOR_3DHIGHLIGHT);
	return clrFrame;
}

template<class BASE_CLASS>
class CStaticFrameWnd : public BASE_CLASS
{
public:

protected:

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_NCPAINT:
			{
				CWindowDC dc(this);
				CXTPWindowRect rc(this);

				dc.Draw3dRect(0, 0, rc.Width(), rc.Height(), GetStaticFrameColor(TRUE), GetStaticFrameColor(FALSE));
				return TRUE;
			}

		case WM_NCCALCSIZE:
			{
				NCCALCSIZE_PARAMS FAR* lpncsp = (NCCALCSIZE_PARAMS FAR*)lParam;

				// adjust non-client area for border space
				lpncsp->rgrc[0].left   += 1;
				lpncsp->rgrc[0].top    += 1;
				lpncsp->rgrc[0].right  -= 1;
				lpncsp->rgrc[0].bottom -= 1;

				return TRUE;
			}
		}

		return BASE_CLASS::WindowProc(message, wParam, lParam);
	}
};

/////////////////////////////////////////////////////////////////////////////
// CGUI_OutlookApp:
// See GUI_Outlook.cpp for the implementation of this class
//

class CGUI_OutlookApp : public CWinApp
{
public:
	CGUI_OutlookApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUI_OutlookApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGUI_OutlookApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUI_OUTLOOK_H__62A35493_2F0F_4C47_BE10_A972BED2082D__INCLUDED_)
