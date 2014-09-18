// PageDragAndDropClient.cpp : implementation file
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
#include "toolbox.h"
#include "PageDragAndDropClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageDragAndDropClient

CPageDragAndDropClient::CPageDragAndDropClient()
{
	m_imageManager.SetIcons(IDB_TRASH, 0, 0, CSize(48, 48));

	m_bDragging = FALSE;
}

CPageDragAndDropClient::~CPageDragAndDropClient()
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		m_arrItems[i]->InternalRelease();
	}
}


BEGIN_MESSAGE_MAP(CPageDragAndDropClient, CStatic)
	//{{AFX_MSG_MAP(CPageDragAndDropClient)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageDragAndDropClient message handlers

BOOL CPageDragAndDropClient::OnEraseBkgnd(CDC* )
{
	return TRUE;
}


CXTPTaskPanel* CPageDragAndDropClient::GetTaskPanel()
{
	CWnd* pSplitterWnd = GetParentFrame()->GetDlgItem(AFX_IDW_PANE_FIRST);
	ASSERT_KINDOF(CSplitterWnd, pSplitterWnd);

	return (CXTPTaskPanel*)pSplitterWnd->GetDlgItem(AFX_IDW_PANE_FIRST);
}

void CPageDragAndDropClient::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting

	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint, rc);

	dc.FillSolidRect(rc, GetSysColor(COLOR_3DFACE));

	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		CXTPTaskPanelGroupItem* pItem = m_arrItems[i];
		CRect rcItem = pItem->GetItemRect();

		GetTaskPanel()->GetPaintManager()->DrawGroupItemFrame(&dc, pItem, rcItem);

		CXTPImageManagerIcon* pImage = GetTaskPanel()->GetImageManager()->GetImage(pItem->GetIconIndex());
		if (pImage)
		{
			CPoint ptIcon((rcItem.right + rcItem.left - 16) / 2, (rcItem.top + rcItem.bottom - 16) / 2);
			pImage->Draw(&dc, ptIcon, pImage->GetIcon());
		}
	}


	CRect rcTrash(rc.right - 72, rc.bottom - 72, rc.right - 12, rc.bottom - 12);
	dc.FillSolidRect(rcTrash, GetSysColor(COLOR_3DFACE));
	dc.Draw3dRect(rcTrash, GetSysColor(COLOR_3DHIGHLIGHT), GetSysColor(COLOR_3DSHADOW));

	CXTPImageManagerIcon* pImage = m_imageManager.GetImage(0, 48);
	if (pImage)
	{
		CPoint ptImage((rcTrash.right + rcTrash.left - 48) / 2, (rcTrash.top + rcTrash.bottom - 48) / 2);
		pImage->Draw(&dc, ptImage, pImage->GetIcon());
	}
}

void CPageDragAndDropClient::PreSubclassWindow()
{
	Register(this);
	EnableToolTips();

	CStatic::PreSubclassWindow();
}

BOOL CPageDragAndDropClient::PtInTrash(CPoint point)
{
	CXTPClientRect rc(this);
	CRect rcTrash(rc.right - 72, rc.bottom - 72, rc.right - 12, rc.bottom - 12);

	return rcTrash.PtInRect(point);
}


DROPEFFECT CPageDragAndDropClient::OnDragOver(CWnd* /*pWnd*/, COleDataObject* pDataObject,
		DWORD dwKeyState, CPoint point)
{
	if (!pDataObject || !pDataObject->IsDataAvailable(CXTPTaskPanel::GetClipboardFormat()))
		return DROPEFFECT_NONE;

	if (PtInTrash(point))
		return DROPEFFECT_MOVE;

	if (!m_bDragging || ((dwKeyState & MK_CONTROL) == MK_CONTROL))
		return DROPEFFECT_COPY;

	return DROPEFFECT_MOVE;
}

BOOL CPageDragAndDropClient::OnDrop(CWnd* /*pWnd*/, COleDataObject* pDataObject,
		DROPEFFECT /*dropEffect*/, CPoint point)
{
	if (PtInTrash(point))
		return TRUE;

	CXTPTaskPanelGroupItem* pItemDrop = (CXTPTaskPanelGroupItem*)CXTPTaskPanelItem::CreateFromOleData(pDataObject);

	if (!pItemDrop)
		return FALSE;

	ASSERT_KINDOF(CXTPTaskPanelGroupItem, pItemDrop);

	m_arrItems.Add(pItemDrop);
	pItemDrop->GetItemRect() = CRect(point.x - 14, point.y - 14, point.x + 14, point.y + 14);

	Invalidate(FALSE);

	return TRUE;
}

int CPageDragAndDropClient::HitTest(CPoint point) const
{
	for (int i = (int)m_arrItems.GetSize() - 1; i >= 0; i--)
	{
		CXTPTaskPanelGroupItem* pItem = m_arrItems[i];

		if (pItem->GetItemRect().PtInRect(point))
			return i;
	}
	return -1;
}

void CPageDragAndDropClient::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	int nHit = HitTest(point);
	CXTPTaskPanelGroupItem* pItem = (nHit >= 0? m_arrItems[nHit]: NULL);

	if (!pItem)
		return;


	COleDataSource ds;
	if (!pItem->PrepareDrag(ds))
		return;

	m_bDragging = TRUE;

	DROPEFFECT dropEffect = ds.DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_MOVE);

	m_bDragging = FALSE;

	if (dropEffect == DROPEFFECT_MOVE)
	{
		pItem->InternalRelease();
		m_arrItems.RemoveAt(nHit);
	}

	Invalidate(FALSE);
}


INT_PTR CPageDragAndDropClient::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	// check child windows first by calling CControlBar
	INT_PTR nHit = CWnd::OnToolHitTest(point, pTI);
	if (nHit != -1)
		return nHit;

	int nItem = HitTest(point);
	CXTPTaskPanelGroupItem* pItem = (nItem >= 0? m_arrItems[nItem]: NULL);

	if (pItem)
	{
			int nHit = pItem->GetID();

			CString strTip = pItem->GetCaption();
			if (strTip.GetLength() == 0)
				return -1;

		#if _MSC_VER < 1200 // MFC 5.0
			if (pTI != NULL)
		#else
			if (pTI != NULL)
		#endif
			{
				pTI->hwnd = GetSafeHwnd();
				pTI->rect = pItem->GetItemRect();
				pTI->uId = nHit;
				pTI->lpszText = (LPTSTR) ::calloc(strTip.GetLength() + 1, sizeof(TCHAR));
				if( pTI->lpszText != NULL ) STRCPY_S(pTI->lpszText, strTip.GetLength() + 1, (LPCTSTR)strTip);
			}
			return TRUE;
	}
	return -1;
}
