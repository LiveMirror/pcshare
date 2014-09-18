// cntritem.h : interface of the CDrawItem class
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
#include "drawcli.h"

#include "drawdoc.h"
#include "drawobj.h"
#include "drawvw.h"
#include "cntritem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawItem implementation

IMPLEMENT_SERIAL(CDrawItem, COleClientItem, 0)

CDrawItem::CDrawItem(CDrawDoc* pContainer, CDrawOleObj* pDrawObj)
	: COleClientItem(pContainer)
{
	m_pDrawObj = pDrawObj;
}

CDrawItem::~CDrawItem()
{
	if (m_pDrawObj != NULL)
		m_pDrawObj->m_pClientItem = NULL;
}

void CDrawItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam)
{
	ASSERT_VALID(this);

	COleClientItem::OnChange(nCode, dwParam);

	switch(nCode)
	{
	case OLE_CHANGED_STATE:
	case OLE_CHANGED_ASPECT:
		m_pDrawObj->Invalidate();
		break;
	case OLE_CHANGED:
		UpdateExtent(); // extent may have changed
		m_pDrawObj->Invalidate();
		break;
	}
}

BOOL CDrawItem::OnChangeItemPosition(const CRect& rectPos)
{
	ASSERT_VALID(this);

	CDrawView* pView = GetActiveView();
	ASSERT_VALID(pView);
	CRect rect = rectPos;
	pView->ClientToDoc(rect);

	if (rect != m_pDrawObj->m_position)
	{
		// invalidate old rectangle
		m_pDrawObj->Invalidate();

		// update to new rectangle
		m_pDrawObj->m_position = rect;
		GetExtent(&m_pDrawObj->m_extent);

		// and invalidate new rectangle
		m_pDrawObj->Invalidate();

		// mark document as dirty
		GetDocument()->SetModifiedFlag();
	}
	return COleClientItem::OnChangeItemPosition(rectPos);
}

void CDrawItem::OnGetItemPosition(CRect& rPosition)
{
	ASSERT_VALID(this);

	// update to extent of item if m_position is not initialized
	if (m_pDrawObj->m_position.IsRectEmpty())
		UpdateExtent();

	// copy m_position, which is in document coordinates
	CDrawView* pView = GetActiveView();
	ASSERT_VALID(pView);
	rPosition = m_pDrawObj->m_position;
	pView->DocToClient(rPosition);
}

void CDrawItem::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	// Call base class first to read in COleClientItem data.
	// Note: this sets up the m_pDocument pointer returned from
	//  CDrawItem::GetDocument, therefore it is a good idea
	//  to call the base class Serialize first.
	COleClientItem::Serialize(ar);

	// now store/retrieve data specific to CDrawItem
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

BOOL CDrawItem::UpdateExtent()
{
	CSize size;
	if (!GetExtent(&size) || size == m_pDrawObj->m_extent)
		return FALSE;       // blank

	// if new object (i.e. m_extent is empty) setup position
	if (m_pDrawObj->m_extent == CSize(0, 0))
	{
		m_pDrawObj->m_position.right =
			m_pDrawObj->m_position.left + MulDiv(size.cx, 10, 254);
		m_pDrawObj->m_position.bottom =
			m_pDrawObj->m_position.top - MulDiv(size.cy, 10, 254);
	}
	// else data changed so scale up rect as well
	else if (!IsInPlaceActive() && size != m_pDrawObj->m_extent)
	{
		m_pDrawObj->m_position.right = m_pDrawObj->m_position.left +
			MulDiv(m_pDrawObj->m_position.Width(), size.cx, m_pDrawObj->m_extent.cx);
		m_pDrawObj->m_position.bottom = m_pDrawObj->m_position.top +
			MulDiv(m_pDrawObj->m_position.Height(), size.cy, m_pDrawObj->m_extent.cy);
	}

	m_pDrawObj->m_extent = size;
	m_pDrawObj->Invalidate();   // redraw to the new size/position
	return TRUE;
}

void CDrawItem::OnActivate()
{
	// allow only one inplace active item per frame
	CView* pView = GetActiveView();
	ASSERT_VALID(pView);
	COleClientItem* pItem = GetDocument()->GetInPlaceActiveItem(pView);
	if (pItem != NULL && pItem != this)
		pItem->Close();

	COleClientItem::OnActivate();
}

void CDrawItem::OnDeactivateUI(BOOL bUndoable)
{
	COleClientItem::OnDeactivateUI(bUndoable);

	// hide the object if it is not an outside-in object
	DWORD dwMisc = 0;
	m_lpObject->GetMiscStatus(GetDrawAspect(), &dwMisc);
	if (dwMisc & OLEMISC_INSIDEOUT)
		DoVerb(OLEIVERB_HIDE, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CDrawItem diagnostics

#ifdef _DEBUG
void CDrawItem::AssertValid() const
{
	COleClientItem::AssertValid();
}

void CDrawItem::Dump(CDumpContext& dc) const
{
	COleClientItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////

BOOL CDrawItem::OnShowControlBars(CFrameWnd* pFrameWnd, BOOL bShow)
{
	CMainFrame* pFrame = DYNAMIC_DOWNCAST(CMainFrame, pFrameWnd);
	if (!pFrame)
		return COleClientItem::OnShowControlBars(pFrameWnd, bShow);


	BOOL bResult = FALSE;

#ifdef _XTP_INCLUDE_COMMANDBARS

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
#endif

	return COleClientItem::OnShowControlBars(pFrameWnd, bShow) || bResult;
}
