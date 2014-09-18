// TabbedView.h : main header file for the TABBEDVIEW application
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

#if !defined(AFX_TABBEDVIEW_H__2C45EC68_359D_4A10_A165_FB93F3AA4EE6__INCLUDED_)
#define AFX_TABBEDVIEW_H__2C45EC68_359D_4A10_A165_FB93F3AA4EE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTabbedViewApp:
// See TabbedView.cpp for the implementation of this class
//

class CTabbedViewApp : public CWinApp
{
public:
	CTabbedViewApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabbedViewApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTabbedViewApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


template<class BASE_CLASS>
class CNoFlickerWnd : public BASE_CLASS
{

protected:
	virtual LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam)
	{
		switch (message)
		{
		case WM_PAINT:
			{
				CPaintDC dc(this);

				// Get the client rect, and paint to a memory device context.
				// This will help reduce screen flicker.  Pass the memory
				// device context to the default window procedure to do
				// default painting.

				CXTPClientRect rc(this);
				CXTPBufferDC memDC(dc, rc);
				memDC.FillSolidRect(rc, GetXtremeColor(COLOR_WINDOW));

				return BASE_CLASS::DefWindowProc(WM_PAINT,
					(WPARAM)memDC.m_hDC, 0);
			}

		case WM_ERASEBKGND:
				return TRUE;
		}

		return BASE_CLASS::WindowProc(message, wParam, lParam);
	}
};

AFX_INLINE BOOL CreateImageList(CImageList& il, UINT nID)
{
	if (!il.Create(16, 16, ILC_MASK|ILC_COLOR24, 0, 1))
		return FALSE;
	CBitmap bmp;
	VERIFY(bmp.LoadBitmap(nID));

	il.Add(&bmp, CXTPImageManager::GetBitmapMaskColor(bmp, CPoint(0, 0)));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABBEDVIEW_H__2C45EC68_359D_4A10_A165_FB93F3AA4EE6__INCLUDED_)
