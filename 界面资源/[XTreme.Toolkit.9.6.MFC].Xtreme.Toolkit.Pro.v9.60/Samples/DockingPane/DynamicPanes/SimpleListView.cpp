// SimpleListView.cpp : implementation file
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
#include "DynamicPanes.h"
#include "SimpleListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimpleListView

IMPLEMENT_DYNCREATE(CSimpleListView, CListView)

CSimpleListView::CSimpleListView()
{
}

CSimpleListView::~CSimpleListView()
{
}


BEGIN_MESSAGE_MAP(CSimpleListView, CListView)
	//{{AFX_MSG_MAP(CSimpleListView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimpleListView drawing

void CSimpleListView::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CSimpleListView diagnostics

#ifdef _DEBUG
void CSimpleListView::AssertValid() const
{
	CListView::AssertValid();
}

void CSimpleListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSimpleListView message handlers

void CSimpleListView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	if (m_imageList.GetSafeHandle())
		return;

	CListCtrl& wndListCtrl = GetListCtrl();

	if (!m_imageList.Create (32, 32, ILC_COLOR24 | ILC_MASK, 1, 20))
	{
		TRACE0 ("Failed to create image list.\n");
		return;
	}

	CWinApp* pWinApp = AfxGetApp();
	ASSERT_VALID(pWinApp);

	m_imageList.Add (pWinApp->LoadIcon (IDI_AQUAMAN));
	m_imageList.Add (pWinApp->LoadIcon (IDI_ATOM));
	m_imageList.Add (pWinApp->LoadIcon (IDI_BATMAN));
	m_imageList.Add (pWinApp->LoadIcon (IDI_FIRESTORM));
	m_imageList.Add (pWinApp->LoadIcon (IDI_FLASH));
	m_imageList.Add (pWinApp->LoadIcon (IDI_GREENARROW));
	m_imageList.Add (pWinApp->LoadIcon (IDI_GREENLANTERN));
	m_imageList.Add (pWinApp->LoadIcon (IDI_HAWKMAN));
	m_imageList.Add (pWinApp->LoadIcon (IDI_HAWKWOMAN));
	m_imageList.Add (pWinApp->LoadIcon (IDI_REDTORNADO));
	m_imageList.Add (pWinApp->LoadIcon (IDI_SUPERMAN));
	m_imageList.Add (pWinApp->LoadIcon (IDI_WONDERWOMAN));

	wndListCtrl.SetImageList (&m_imageList, LVSIL_NORMAL);
	wndListCtrl.ModifyStyle(0, LVS_AUTOARRANGE);


	for (int i = 0; i < 11; i++) wndListCtrl.InsertItem(LVIF_IMAGE, i, NULL, 0, 0, i, 0);

}
