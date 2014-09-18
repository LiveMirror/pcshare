// XTTreeCtrl.cpp : implementation file
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
#include "XTGlobal.h"
#include "XTVC50Helpers.h"
#include "XTTreeCtrlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CXTTreeView

CXTTreeView::CXTTreeView()
{
	m_pTreeCtrl = (CTreeCtrl_Friendly*)&GetTreeCtrl();

}

CXTTreeView::~CXTTreeView()
{
}

IMPLEMENT_DYNCREATE(CXTTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CXTTreeView, CTreeView)
	//{{AFX_MSG_MAP(CXTTreeView)
	ON_TREECTRL_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CXTTreeCtrl

CXTTreeCtrl::CXTTreeCtrl()
{
	m_pTreeCtrl = (CTreeCtrl_Friendly*)this;
}

CXTTreeCtrl::~CXTTreeCtrl()
{
}

IMPLEMENT_DYNAMIC(CXTTreeCtrl, CTreeCtrl)

BEGIN_MESSAGE_MAP(CXTTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CXTTreeCtrl)
	ON_TREECTRL_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTTreeCtrl message handlers
