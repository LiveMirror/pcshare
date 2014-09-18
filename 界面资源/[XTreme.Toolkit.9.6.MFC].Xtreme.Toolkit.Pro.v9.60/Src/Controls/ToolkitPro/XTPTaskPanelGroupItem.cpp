// XTPTaskPanelGroupItem.cpp : implementation of the CXTPTaskPanelGroupItem class.
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
#include "XTPDrawHelpers.h"
#include "XTPPropExchange.h"

#include "XTPTaskPanelGroupItem.h"
#include "XTPTaskPanelItems.h"
#include "XTPTaskPanelPaintManager.h"
#include "XTPTaskPanel.h"
#include "XTPTaskPanelGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelGroupItem

IMPLEMENT_SERIAL(CXTPTaskPanelGroupItem, CXTPTaskPanelItem, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPTaskPanelGroupItem::CXTPTaskPanelGroupItem()
{
	m_typeItem = xtpTaskItemTypeText;
	m_rcMargins = CXTPTaskPanelPaintManager::rectDefault;

	m_bBold = FALSE;
	m_hWnd = 0;
	m_szItem = 0;
	m_bSelected = FALSE;

	EnableAutomation();
}

CXTPTaskPanelGroupItem::~CXTPTaskPanelGroupItem()
{
	if (m_typeItem == xtpTaskItemTypeControl && m_hWnd && ::IsWindow(m_hWnd))
	{
		::ShowWindow(m_hWnd, SW_HIDE);
	}
}

CRect CXTPTaskPanelGroupItem::OnReposition(CRect rc)
{
	if (GetItemGroup()->GetItemLayout() == xtpTaskItemLayoutImages)
	{
		return m_rcItem = rc;
	}

	CXTPTaskPanelPaintManager* pPaintManager = GetPaintManager();
	CRect rcMargins = pPaintManager->GetItemOuterMargins(this);

	rc.DeflateRect(rcMargins);

	if ((m_szItem == CSize(0)) && (m_typeItem != xtpTaskItemTypeControl))
	{
		CClientDC dc((CWnd*)m_pPanel);
		m_rcItem =  GetPaintManager()->DrawGroupItem(&dc, this, rc, FALSE);
	} else
	{
		m_rcItem.SetRect(rc.left, rc.top, rc.left + m_szItem.cx, rc.top + m_szItem.cy);
	}

	return m_rcItem;
}

void CXTPTaskPanelGroupItem::OnDrawItem(CDC* pDC, CRect rc)
{
	if (m_typeItem != xtpTaskItemTypeControl)
		GetPaintManager()->DrawGroupItem(pDC, this, rc, TRUE);
	else
	{
		if (!IsWindowVisible(m_hWnd) && m_bmpPreview.GetSafeHandle())
		{
			pDC->DrawState(rc.TopLeft(), rc.Size(), &m_bmpPreview, DST_BITMAP);
		}
	}
}

CXTPTaskPanelGroup* CXTPTaskPanelGroupItem::GetItemGroup()
{
	return m_pItems? (CXTPTaskPanelGroup*) m_pItems->GetOwner(): NULL;
}

void CXTPTaskPanelGroupItem::SetBold(BOOL bBold)
{
	m_bBold = bBold;
	RepositionPanel();
}
BOOL CXTPTaskPanelGroupItem::IsBold()
{
	return m_bBold;
}

void CXTPTaskPanelGroupItem::SetControlHandle(HWND hWnd)
{
	m_hWnd = hWnd;
	SetType(xtpTaskItemTypeControl);

	CRect rc;
	::GetClientRect(hWnd, rc);
	m_szItem.cy = rc.Height();
}


void CXTPTaskPanelGroupItem::GetPreviewBitmap(CWnd* pWnd, CBitmap& bmp)
{
	bmp.DeleteObject();
	if (!pWnd || !::IsWindow(pWnd->m_hWnd))
		return;


	{
		CXTPWindowRect rectWindow(pWnd);
		CClientDC dc(pWnd);
		CBitmap bitmapNonClient;

		VERIFY(bmp.CreateCompatibleBitmap(&dc, rectWindow.Width(), rectWindow.Height()));
		VERIFY(bitmapNonClient.CreateCompatibleBitmap(&dc, rectWindow.Width(), rectWindow.Height()));

		CXTPCompatibleDC dcMemDestination(&dc, &bmp);
		CXTPCompatibleDC dcMemSourceNonClient(&dc, &bitmapNonClient);

		dcMemSourceNonClient.FillSolidRect(0, 0, rectWindow.Width(), rectWindow.Height(), GetBackColor());
		pWnd->Print(&dcMemSourceNonClient, PRF_NONCLIENT);

		dcMemDestination.BitBlt(0,0,rectWindow.Width(), rectWindow.Height(), &dcMemSourceNonClient, 0,0, SRCCOPY);

		CPoint pLT(0,0);
		pWnd->ClientToScreen(&pLT);
		dcMemDestination.SetViewportOrg(pLT.x - rectWindow.left, pLT.y - rectWindow.top);

		pWnd->Print(&dcMemDestination, PRF_CHILDREN | PRF_CLIENT | PRF_ERASEBKGND);
	}

}


void CXTPTaskPanelGroupItem::OnAnimate(int /*nStep*/)
{
	if (!(m_typeItem == xtpTaskItemTypeControl && m_hWnd && ::IsWindow(m_hWnd)))
		return;

	CXTPTaskPanelGroup* pGroup = GetItemGroup();
	if (!pGroup) return;

	CRect rc = pGroup->GetClientRect();
	rc.OffsetRect(m_rcItem.left - rc.left, m_rcItem.top - m_pPanel->GetScrollOffset());


	if (pGroup->IsExpanding() || !pGroup->IsExpanded()
		|| (pGroup->GetClientRect().Height() != pGroup->GetTargetClientRect().Height())
		)
	{
		if (IsWindowVisible(m_hWnd) || m_bmpPreview.GetSafeHandle() == 0)
		{
			GetPreviewBitmap(CWnd::FromHandle(m_hWnd), m_bmpPreview);
			::ShowWindow(m_hWnd, SW_HIDE);
		}
	}
	else
	{
		CRect rcMargins = GetPaintManager()->GetItemOuterMargins(this);
		CRect rcGroupMargin = GetPaintManager()->GetGroupInnerMargins(pGroup);

		int nWidth = rc.Width() - rcGroupMargin.left - rcGroupMargin.right -
			rcMargins.right - rcMargins.left;

		if (m_szItem.cx != 0) nWidth = min(m_szItem.cx, nWidth);

		SetWindowPos(m_hWnd, 0, rc.left, rc.top, nWidth, m_rcItem.Height(), SWP_NOZORDER|SWP_SHOWWINDOW|SWP_NOACTIVATE);

		m_bmpPreview.DeleteObject();
	}
}

void CXTPTaskPanelGroupItem::SetSize(CSize szItem)
{
	m_szItem = szItem;
	RepositionPanel();
}

COLORREF CXTPTaskPanelGroupItem::GetBackColor()
{
	XTP_TASKPANEL_GROUPCOLORS* pColors = GetPaintManager()->GetGroupColors(GetItemGroup()->IsSpecialGroup());
	return pColors->clrClient;
}



BOOL CXTPTaskPanelGroupItem::IsItemSelected()
{
	if (m_pPanel && m_pPanel->IsSingleSelection())
		return m_pPanel->GetFocusedItem() == this;

	return m_bSelected;
}

void CXTPTaskPanelGroupItem::SetItemSelected(BOOL bSelected)
{
	if (m_pPanel && m_pPanel->IsSingleSelection())
	{
		if (bSelected) m_pPanel->SetFocusedItem(this);
		return;
	}

	if (bSelected != m_bSelected)
	{
		m_bSelected = bSelected;
		RedrawPanel();
	}
}

CRect CXTPTaskPanelGroupItem::GetHitTestRect()
{
	CXTPTaskPanelGroup* pGroup = GetItemGroup();
	CRect rcCaption = pGroup->GetCaptionRect();

	CRect rcItem = GetItemRect();
	int nOffset = m_pPanel->GetScrollOffset() - rcCaption.bottom + pGroup->GetScrollOffsetPos();

	rcItem.OffsetRect(0, -nOffset);
	return rcItem;
}

void CXTPTaskPanelGroupItem::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPTaskPanelItem::DoPropExchange(pPX);

	BOOL bSelected = IsItemSelected();

	PX_Rect(pPX, _T("Margins"), m_rcMargins, CXTPTaskPanelPaintManager::rectDefault);
	PX_Bool(pPX, _T("Bold"), m_bBold, FALSE);
	PX_Size(pPX, _T("ItemSize"), m_szItem, CSize(0, 0));
	PX_Bool(pPX, _T("Selected"), bSelected, FALSE);

	if (pPX->IsLoading())
	{
		SetItemSelected(bSelected);
	}
}



