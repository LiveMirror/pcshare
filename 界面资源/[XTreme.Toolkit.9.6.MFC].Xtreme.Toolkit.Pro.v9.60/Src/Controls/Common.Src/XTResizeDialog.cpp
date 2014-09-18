// XTResizeDialog.cpp: implementation of the CXTResizeDialog class.
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
#include "XTResizeDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable: 4355) // 'this' : used in base member initializer list

/////////////////////////////////////////////////////////////////////////////
// CXTResizeDialog

IMPLEMENT_DYNCREATE(CXTResizeDialog, CDialog)

BEGIN_MESSAGE_MAP(CXTResizeDialog, CDialog)
	//{{AFX_MSG_MAP(CXTResizeDialog)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTResizeDialog construction/destruction

CXTResizeDialog::CXTResizeDialog()
	: CXTResize(NULL, 0)
{
}

CXTResizeDialog::CXTResizeDialog(const UINT nID, CWnd* pParent, const UINT nFlags) :
	CDialog(nID, pParent),
	CXTResize(this, nFlags)
{
	m_nDialogID = nID;
}

BOOL CXTResizeDialog::OnInitDialog()
{
	// Only show the resize grip if the frame is a resizing frame
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	if ((dwStyle & WS_THICKFRAME) == 0) {
		SetFlag(xtResizeNoSizeIcon);
	}

	CDialog::OnInitDialog();
	CXTResize::Init();

	// Enable tool tips for this dialog
	EnableToolTips();

	return TRUE;
}

void CXTResizeDialog::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	CXTResize::GetMinMaxInfo(pMMI);
}

void CXTResizeDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CXTResize::Size();
}

void CXTResizeDialog::OnDestroy()
{
	// Save window placement if AutoLoadPlacement was used.
	if (!m_strSection.IsEmpty()) {
		SavePlacement(m_strSection);
	}

	CDialog::OnDestroy();
}
