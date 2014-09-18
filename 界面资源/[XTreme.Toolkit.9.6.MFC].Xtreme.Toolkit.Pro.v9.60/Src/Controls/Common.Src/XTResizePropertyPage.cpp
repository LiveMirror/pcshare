// XTResizePropertyPage.cpp: implementation of the CXTResizePropertyPage class.
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

#include "stdafx.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTResizeRect.h"
#include "XTResizePoint.h"
#include "XTPWinThemeWrapper.h"
#include "XTResize.h"
#include "XTResizePropertyPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable: 4355) // 'this' : used in base member initializer list

/////////////////////////////////////////////////////////////////////////////
// CXTResizePropertyPage

IMPLEMENT_DYNCREATE(CXTResizePropertyPage, CPropertyPage)

BEGIN_MESSAGE_MAP(CXTResizePropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CXTResizePropertyPage)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTResizePropertyPage construction/destruction

CXTResizePropertyPage::CXTResizePropertyPage(const UINT nTemplate, const UINT nCaption, const UINT nFlags) :
	CPropertyPage(nTemplate, nCaption),
	CXTResize(this, nFlags | xtResizeNoSizeIcon)
{
	m_nDialogID = nTemplate;
}

BOOL CXTResizePropertyPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	CXTResize::Init();

	EnableToolTips();

	return TRUE;
}

void CXTResizePropertyPage::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	CXTResize::GetMinMaxInfo(pMMI);
}

void CXTResizePropertyPage::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);
	CXTResize::Size();
}

void CXTResizePropertyPage::OnDestroy()
{
	// Save window placement if AutoLoadPlacement was used.
	if (!m_strSection.IsEmpty()) {
		SavePlacement(m_strSection);
	}

	CPropertyPage::OnDestroy();
}
