// cntritem.cpp : implementation of the CWordPadCntrItem class
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
#include "wordpad.h"

#include "wordpdoc.h"
#include "wordpvw.h"
#include "cntritem.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWordPadCntrItem implementation

IMPLEMENT_SERIAL(CWordPadCntrItem, CRichEditCntrItem, 0)

CWordPadCntrItem::CWordPadCntrItem(REOBJECT *preo, CWordPadDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
}

BOOL CWordPadCntrItem::OnShowControlBars(CFrameWnd* pFrameWnd, BOOL bShow)
{
	CMainFrame* pFrame = DYNAMIC_DOWNCAST(CMainFrame, pFrameWnd);
	if (!pFrame)
		return COleClientItem::OnShowControlBars(pFrameWnd, bShow);


	BOOL bResult = FALSE;

	for (int i = 0; i < pFrame->GetCommandBars()->GetCount(); i++)
	{
		CXTPToolBar* pToolBar = pFrame->GetCommandBars()->GetAt(i);
		ASSERT(pToolBar);

		if (bShow)
		{

			if (pToolBar->m_nStateFlags & CControlBar::tempHide)
			{
				pToolBar->m_nStateFlags &= ~CControlBar::tempHide;
				pToolBar->SetVisible(TRUE);
				bResult = TRUE;
			}
		}
		else
		{
			if (pToolBar->IsVisible())
			{
				pToolBar->m_nStateFlags |= CControlBar::tempHide;
				pToolBar->SetVisible(FALSE);
				bResult = TRUE;
			}
		}
	}

	return COleClientItem::OnShowControlBars(pFrameWnd, bShow) || bResult;
}

/////////////////////////////////////////////////////////////////////////////
// CWordPadCntrItem diagnostics

#ifdef _DEBUG
void CWordPadCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CWordPadCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
