// ViewType3.cpp : implementation file
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
#include "MDIMenus.h"
#include "ViewType3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewType3

IMPLEMENT_DYNCREATE(CViewType3, CViewType3Base)

CViewType3::CViewType3()
{
	//{{AFX_DATA_INIT(CViewType3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CViewType3::~CViewType3()
{
}

void CViewType3::DoDataExchange(CDataExchange* pDX)
{
	CViewType3Base::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewType3)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewType3, CViewType3Base)
	//{{AFX_MSG_MAP(CViewType3)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_XTP_EXECUTE(ID_GOTO_URL, OnGotoURL)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewType3 diagnostics

#ifdef _DEBUG
void CViewType3::AssertValid() const
{
	CViewType3Base::AssertValid();
}

void CViewType3::Dump(CDumpContext& dc) const
{
	CViewType3Base::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewType3 message handlers


void CViewType3::OnGotoURL(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlComboBox* pControl = (CXTPControlComboBox*)tagNMCONTROL->pControl;
	if (pControl->GetType() == xtpControlComboBox)
	{
		CString strURL = pControl->GetEditText();

	#if _MSC_VER >= 1200 // MFC 5.0
		Navigate(strURL);
	#endif
		GetDocument()->SetTitle(strURL);
	}
	*pResult = 1;
}

void CViewType3::OnInitialUpdate() 
{
	CViewType3Base::OnInitialUpdate();

#if _MSC_VER >= 1200 // MFC 5.0
	Navigate(_T("about:blank"));
#endif
}