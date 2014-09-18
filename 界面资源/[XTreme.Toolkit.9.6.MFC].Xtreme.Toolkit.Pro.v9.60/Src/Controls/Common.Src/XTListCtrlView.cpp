// XTListCtrlView.cpp : implementation of the CXTListView and CXTListCtrl class.
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

#include "StdAfx.h"
#include "XTResource.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTVC50Helpers.h"
#include "XTPWinThemeWrapper.h"
#include "XTFlatHeaderCtrl.h"
#include "XTListCtrlView.h"
#include "XTMemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTListView
/////////////////////////////////////////////////////////////////////////////
CXTListView::CXTListView()
{
	ImplAttach(&GetListCtrl());
}

CXTListView::~CXTListView()
{

}

IMPLEMENT_DYNCREATE(CXTListView, CListView)

BEGIN_MESSAGE_MAP(CXTListView, CListView)
	//{{AFX_MSG_MAP(CXTListView)
	ON_LISTCTRL_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTListCtrl
/////////////////////////////////////////////////////////////////////////////

CXTListCtrl::CXTListCtrl()
{
	ImplAttach(this);
}

CXTListCtrl::~CXTListCtrl()
{
}


IMPLEMENT_DYNAMIC(CXTListCtrl, CListCtrl)

BEGIN_MESSAGE_MAP(CXTListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CXTListCtrl)
	ON_LISTCTRL_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
