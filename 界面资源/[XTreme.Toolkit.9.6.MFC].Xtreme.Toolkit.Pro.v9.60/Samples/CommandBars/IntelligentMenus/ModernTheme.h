// ModernTheme.h: interface for the CModernTheme class.
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

#if !defined(AFX_MODERNTHEME_H__8501B6CF_A950_445D_9480_F59722E5A000__INCLUDED_)
#define AFX_MODERNTHEME_H__8501B6CF_A950_445D_9480_F59722E5A000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CModernTheme : public CXTPNativeXPTheme
{
public:
	CModernTheme();
	virtual ~CModernTheme();

	void FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar);
	void FillDockBar(CDC* pDC, CXTPDockBar* pBar);

	void _Fill(CDC* pDC, CWnd* pBar);


};

#endif // !defined(AFX_MODERNTHEME_H__8501B6CF_A950_445D_9480_F59722E5A000__INCLUDED_)
