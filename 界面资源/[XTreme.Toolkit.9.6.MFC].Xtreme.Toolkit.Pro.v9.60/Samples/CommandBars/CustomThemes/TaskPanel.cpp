// TaskPanel.cpp : implementation file
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
#include "customthemes.h"
#include "TaskPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaskPanel

CTaskPanel::CTaskPanel()
{
}

CTaskPanel::~CTaskPanel()
{
}


BEGIN_MESSAGE_MAP(CTaskPanel, CTaskPanelBase)
	//{{AFX_MSG_MAP(CTaskPanel)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTaskPanel message handlers

BOOL CTaskPanel::Create(CWnd* pParentWnd)
{
#ifdef _XTP_INCLUDE_CONTROLS
	if (!CXTPTaskPanel::Create(WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0),  pParentWnd, 0))
		return FALSE;

	CXTPImageManager* pImageManager = XTPImageManager();

	pImageManager->InternalAddRef();
	SetImageManager(pImageManager);


	SetExpandable(FALSE);

	CXTPTaskPanelGroup* pGroup = AddGroup(0);
	pGroup->SetCaption(_T("New"));

	CXTPTaskPanelGroupItem* pItem = pGroup->AddLinkItem(0, ID_FILE_NEW);
	pItem->SetCaption(_T("Blank document"));


	pItem = pGroup->AddLinkItem(0);
	pItem->SetCaption(_T("XML document"));

	pItem = pGroup->AddLinkItem(0);
	pItem->SetCaption(_T("Web page"));

	pGroup = AddGroup(0);
	pGroup->SetCaption(_T("Templates"));

	pItem = pGroup->AddLinkItem(0, ID_FILE_OPEN);
	pItem->SetCaption(_T("Browse..."));

	SetAnimation(xtpTaskPanelAnimationNo);

#else
	if (!CWnd::Create(_T("STATIC"), _T("Task Panel"), WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|SS_CENTER, CRect(0, 0, 0, 0),  pParentWnd, 0))
		return FALSE;
#endif

	return TRUE;

}

void CTaskPanel::SetTheme(XTPPaintTheme theme)
{
#ifdef _XTP_INCLUDE_CONTROLS
	CXTPTaskPanel::SetTheme(theme == xtpThemeOfficeXP? xtpTaskPanelThemeOfficeXPPlain:
		theme == xtpThemeOffice2003? xtpTaskPanelThemeOffice2003Plain: theme == xtpThemeOffice2000? xtpTaskPanelThemeOffice2000Plain:
		xtpTaskPanelThemeNativeWinXPPlain);
#else
	UNREFERENCED_PARAMETER(theme);
#endif

}
