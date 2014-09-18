// XTPTabControl.cpp : implementation file
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

#include "XTPTabControl.h"
#include "XTPDrawHelpers.h"
#include "XTPTabPaintManager.h"
#include "XTPImageManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CTabManagerDropTarget

class CXTPTabControl::CTabControlDropTarget : public COleDropTarget
{
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* /*pDataObject*/, DWORD /*dwKeyState*/, CPoint point)
	{
		CXTPTabControl* pControl = (CXTPTabControl*)pWnd;
		ASSERT_VALID(pControl);

		CXTPTabManagerItem* pItem = pControl->HitTest(point);

		if (pItem)
		{
			pControl->SetSelectedItem(pItem);
		}

		return DROPEFFECT_NONE;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CXTPTabControl

CXTPTabControl::CXTPTabControl()
{
	m_pPaintManager = new CXTPTabPaintManager();
	m_pPaintManager->DisableLunaColors(TRUE);
	m_pPaintManager->m_bDrawTextNoPrefix = FALSE;

	m_hwndClient = 0;
	m_bAllowReorder = FALSE;

	m_pDropTarget = new CTabControlDropTarget();
	m_pImageManager = new CXTPImageManager();
}

CXTPTabControl::~CXTPTabControl()
{
	m_pPaintManager->InternalRelease();
	m_pImageManager->InternalRelease();
	delete m_pDropTarget;
}

BOOL CXTPTabControl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if (!CWnd::Create(AfxRegisterWndClass(0, LoadCursor(0, IDC_ARROW)), NULL, dwStyle, rect, pParentWnd, nID))
		return FALSE;

	m_pDropTarget->Register(this);

	return TRUE;
}

CXTPTabManagerItem* CXTPTabControl::InsertItem(int nItem, LPCTSTR lpszItem, HWND hwndChild /*= NULL*/, int nImage /*= -1*/)
{
	CXTPTabManagerItem* pItem = AddItem(nItem);
	pItem->SetCaption(lpszItem);
	pItem->SetHandle(hwndChild);
	pItem->SetImageIndex(nImage);

	if (hwndChild)
		::ShowWindow(hwndChild, SW_HIDE);

	if (nItem == 0)
	{
		SetCurSel(0);
	}

	return pItem;
}
void CXTPTabControl::RedrawControl()
{
	if (GetSafeHwnd()) Invalidate(FALSE);
}

void CXTPTabControl::Reposition()
{
	if (!GetSafeHwnd())
		return;

	CXTPClientRect rc(this);
	CClientDC dc(this);

	GetPaintManager()->RepositionTabControl(this, &dc, rc);
	GetPaintManager()->AdjustClientRect(this, rc);

	if (m_hwndClient && ::IsWindow(m_hwndClient))
	{
		::MoveWindow(m_hwndClient, rc.left, rc.top, rc.Width(), rc.Height(), TRUE);
	}

	Invalidate(FALSE);
}

void CXTPTabControl::SetSelectedItem(CXTPTabManagerItem* pItem)
{
	if (m_pSelected != pItem)
	{
		if (m_hwndClient)
			::ShowWindow(m_hwndClient, SW_HIDE);

		if (pItem)
		{
			m_hwndClient = pItem->GetHandle();
		}
		else
		{
			m_hwndClient = 0;
		}

		if (m_hwndClient)
			::ShowWindow(m_hwndClient, SW_SHOWNA);
	}

	CXTPTabManager::SetSelectedItem(pItem);


	NMHDR nmhdr;
	nmhdr.hwndFrom = GetSafeHwnd();
	nmhdr.idFrom   = GetDlgCtrlID();
	nmhdr.code     = TCN_SELCHANGE;

	CWnd* pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
	{
		pOwner->SendMessage(WM_NOTIFY, nmhdr.idFrom, (LPARAM)&nmhdr);
	}
}

void CXTPTabControl::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();

	Reposition();
}

void CXTPTabControl::SetImageList(CImageList* pImageList)
{
	if (pImageList)
	{
		m_pImageManager->SetIcons(*pImageList, 0, 0, 0, xtpImageNormal);
	}
	Reposition();
}



BEGIN_MESSAGE_MAP(CXTPTabControl, CWnd)
	//{{AFX_MSG_MAP(CXTPTabControl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPTabControl message handlers

BOOL CXTPTabControl::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPTabControl::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint, rc);

	GetPaintManager()->DrawTabControl(this, &dc, rc);
}

void CXTPTabControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	Reposition();
}


void CXTPTabControl::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	PerformClick(m_hWnd, point);
}

void CXTPTabControl::OnNavigateButtonClick(UINT nID)
{
	nID;
	ASSERT(nID == xtpTabNavigateButtonClose);

	if (m_pSelected)
	{
		DeleteItem(m_pSelected->GetIndex());
	}
}


void CXTPTabControl::OnSysColorChange()
{
	CWnd::OnSysColorChange();

	XTPColorManager()->RefreshColors();
	GetPaintManager()->RefreshMetrics();

	Reposition();

}

INT_PTR CXTPTabControl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	return PerformToolHitTest(m_hWnd, HitTest(point), pTI);
}

void CXTPTabControl::OnMouseMove(UINT nFlags, CPoint point)
{
	PerformMouseMove(m_hWnd, point);

	CWnd::OnMouseMove(nFlags, point);
}

void CXTPTabControl::OnMouseLeave()
{
	PerformMouseMove(m_hWnd,CPoint(-1, -1));
}

BOOL CXTPTabControl::DrawIcon(CDC* pDC, CPoint pt, CXTPTabManagerItem* pItem, BOOL bDraw, CSize& szIcon)
{
	if (!pItem)
		return GetPaintManager()->m_bShowIcons;

	if (pItem->GetImageIndex() == -1 || GetPaintManager()->m_bShowIcons == FALSE)
		return FALSE;

	CXTPImageManagerIcon* pImage = m_pImageManager->GetImage(pItem->GetImageIndex(), szIcon.cx);

	if (!pImage)
		return FALSE;

	if (!bDraw)
	{
		return TRUE;
	}

	pImage->Draw(pDC, pt, !pItem->IsEnabled()? pImage->GetDisabledIcon(): pItem->IsHighlighted()? pImage->GetHotIcon():
		pItem->IsSelected()? pImage->GetCheckedIcon(): pImage->GetIcon(), szIcon);

	return TRUE;
}

void CXTPTabControl::SetPaintManager(CXTPTabPaintManager* pPaintManager)
{
	delete m_pPaintManager;
	m_pPaintManager = pPaintManager;
	m_pPaintManager->RefreshMetrics();
	Reposition();
}
