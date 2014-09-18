// XTPTaskPanelGroup.cpp : implementation of the CXTPTaskPanelGroup class.
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
#include "XTPImageManager.h"
#include "XTPPropExchange.h"

#include "XTPTaskPanelGroup.h"
#include "XTPTaskPanelGroupItem.h"
#include "XTPTaskPanelItems.h"
#include "XTPTaskPanel.h"
#include "XTPTaskPanelPaintManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelGroup

IMPLEMENT_SERIAL(CXTPTaskPanelGroup, CXTPTaskPanelItem, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPTaskPanelGroup::CXTPTaskPanelGroup(CXTPTaskPanel* pPanel)
{
	m_typeItem = xtpTaskItemTypeGroup;

	m_bSpecial = FALSE;

	m_rcMarginsOuter = CXTPTaskPanelPaintManager::rectDefault;
	m_rcMarginsInner = CXTPTaskPanelPaintManager::rectDefault;


	m_rcGroupTarget.SetRectEmpty();
	m_rcGroupCurrent.SetRectEmpty();

	m_pItems = new CXTPTaskPanelGroupItems(pPanel);
	m_pItems->m_pOwner = this;

	m_nCaptionHeight = 0;
	m_nExpandedClientHeight = 0;
	m_bCaptionVisible = TRUE;

	m_bExpandable = TRUE;
	m_bExpanded = TRUE;
	m_bExpanding = FALSE;
	m_itemLayout = xtpTaskItemLayoutDefault;

	m_nScrollOffset = 0;
	m_nItemsInRow = 1;

	m_szItemIcon = CSize(0);
	m_szClientBitmap = CSize(0);

	EnableAutomation();
}

CXTPTaskPanelGroup::~CXTPTaskPanelGroup()
{
	m_pItems->InternalRelease();
}


CXTPTaskPanelGroupItem* CXTPTaskPanelGroup::AddLinkItem(UINT nID, int nImage)
{
	CXTPTaskPanelGroupItem* pItem = (CXTPTaskPanelGroupItem*)m_pItems->Add(new CXTPTaskPanelGroupItem(), nID);
	pItem->SetIconIndex(nImage);
	pItem->SetType(xtpTaskItemTypeLink);

	return pItem;
}
CXTPTaskPanelGroupItem* CXTPTaskPanelGroup::AddTextItem(CString strText)
{
	CXTPTaskPanelGroupItem* pItem = (CXTPTaskPanelGroupItem*)m_pItems->Add(new CXTPTaskPanelGroupItem(), 0);
	pItem->SetCaption(strText);


	return pItem;
}

CXTPTaskPanelGroupItem* CXTPTaskPanelGroup::AddControlItem(HWND hWnd)
{
	CXTPTaskPanelGroupItem* pItem = (CXTPTaskPanelGroupItem*)m_pItems->Add(new CXTPTaskPanelGroupItem(), 0);
	pItem->SetControlHandle(hWnd);
	m_pPanel->Reposition();
	return pItem;
}


CXTPTaskPanelGroupItem* CXTPTaskPanelGroup::GetAt(int nIndex)
{
	return (CXTPTaskPanelGroupItem*)m_pItems->GetAt(nIndex);
}
CXTPTaskPanelGroupItem* CXTPTaskPanelGroup::FindItem(int nID)
{
	return (CXTPTaskPanelGroupItem*)m_pItems->Find(nID);
}

BOOL CXTPTaskPanelGroup::IsDirty()
{
	return m_rcGroupCurrent != m_rcGroupTarget;
}

int CXTPTaskPanelGroup::CalcInsideHeight()
{
	CRect rc;
	m_pPanel->GetClientRect(&rc);
	int nInsideHeight = rc.Height();

	CXTPTaskPanelPaintManager* pPaintManager = GetPaintManager();

	int nGroupSpacing = pPaintManager->GetGroupSpacing(m_pPanel);
	CRect rcMargins = pPaintManager->GetControlMargins(m_pPanel);

	int nGroupCount = m_pPanel->GetGroups()->GetCount();
	for (int i = 0; i < nGroupCount; i++)
	{
		CXTPTaskPanelGroup* pGroup = (CXTPTaskPanelGroup*)m_pPanel->GetGroups()->GetAt(i);
		CRect rcMarginsOuter = pPaintManager->GetGroupOuterMargins(pGroup);

		nInsideHeight -= pGroup->m_nCaptionHeight + rcMarginsOuter.top + rcMarginsOuter.bottom;
		if (pGroup->IsExpanded() && pGroup != m_pPanel->GetActiveGroup())
			nInsideHeight -= pGroup->m_rcGroupTarget.Height() - pGroup->m_nCaptionHeight;
	}
	nInsideHeight -= nGroupSpacing * (nGroupCount - 1);
	nInsideHeight -= rcMargins.top + rcMargins.bottom;

	return nInsideHeight;
}

CSize CXTPTaskPanelGroup::GetItemIconSize()
{
	return m_szItemIcon == CSize(0)? GetTaskPanel()->GetItemIconSize(): m_szItemIcon;

}

void CXTPTaskPanelGroup::SetIconSize(CSize szItemIcon)
{
	m_szItemIcon =  szItemIcon;
	GetTaskPanel()->Reposition();
}


int CXTPTaskPanelGroup::OnCalcClientHeight()
{
	CRect rc(m_rcGroupTarget.left, 0, m_rcGroupTarget.right, 0);
	CRect rcItems(rc);
	CXTPTaskPanelPaintManager* pPaintManager = GetPaintManager();
	CRect rcMarginsInner = pPaintManager->GetGroupInnerMargins(this);

	rcItems.DeflateRect(rcMarginsInner);
	m_nItemsInRow = 1;

	if (GetItemLayout() == xtpTaskItemLayoutImages)
	{
		CSize szIcon = GetItemIconSize();
		CRect rcInnerMargins(6, 6, 6, 6);
		CSize szButton(szIcon.cx + rcInnerMargins.right + rcInnerMargins.left,
			szIcon.cy + rcInnerMargins.top + rcInnerMargins.bottom);

		int nRowIcons = max(1, rcItems.Width() / szButton.cx), x = 0, y = 0;

		for (int i = 0; i < m_pItems->GetCount(); i++)
		{
			CXTPTaskPanelGroupItem* pItem = GetAt(i);

			CRect rcItem(CPoint(rcItems.left + x * szButton.cx, rcItems.top + y * szButton.cy), szButton);
			pItem->OnReposition(rcItem);

			if (++x == nRowIcons && i != m_pItems->GetCount() - 1)
			{
				x = 0;
				y ++;
			}
		}
		rcItems.bottom = rcItems.top + rcMarginsInner.bottom + szButton.cy * (y + 1);
		m_nItemsInRow = nRowIcons;

		m_nScrollOffset -= m_nScrollOffset % m_nItemsInRow;
	}
	else
	{
		for (int i = 0; i < m_pItems->GetCount(); i++)
		{
			CXTPTaskPanelGroupItem* pItem = GetAt(i);

			CRect rcItem = pItem->OnReposition(rcItems);
			rcItems.top = rcItem.bottom + pPaintManager->GetItemOuterMargins(pItem).bottom;
		}
		rcItems.bottom = rcItems.top + rcMarginsInner.bottom;
	}

	if (!m_pPanel->IsExplorerBehaviour() && (m_pPanel->GetActiveGroup() == this || IsExpanding()))
	{
		int nInsideHeight = CalcInsideHeight();

		int nButtonHeight = max(32, GetItemIconSize().cy + rcMarginsInner.bottom);

		rcItems.bottom = rc.top + max(nButtonHeight + rcMarginsInner.top + 2, nInsideHeight);
	}

	return rcItems.bottom - rc.top;
}


CRect CXTPTaskPanelGroup::OnReposition(CRect rc, BOOL bRecalcOnly)
{
	if (m_pPanel->IsExplorerBehaviour())
		m_nScrollOffset = 0;

	CXTPTaskPanelPaintManager* pPaintManager = GetPaintManager();
	CRect rcMarginsOuter = pPaintManager->GetGroupOuterMargins(this);

	rc.DeflateRect(rcMarginsOuter);

	m_rcGroupTarget = rc;

	if (m_pPanel)
	{
		CClientDC dc(m_pPanel);
		m_nCaptionHeight = GetPaintManager()->DrawGroupCaption(&dc, this, FALSE);
	}

	m_nExpandedClientHeight = OnCalcClientHeight();

	int nClientHeight = IsExpanded()? m_nExpandedClientHeight: 0;

	m_rcGroupTarget.bottom = m_rcGroupTarget.top + m_nCaptionHeight + nClientHeight;

	m_rcGroupCurrent.left = m_rcGroupTarget.left;
	m_rcGroupCurrent.right = m_rcGroupTarget.right;

	if (!bRecalcOnly || !m_pPanel->m_bAnimation) OnAnimate(0);
	else if (!IsDirty()) OnAnimate(0);


	return m_rcGroupTarget;
}

#define MOVETO(A, B, Step) if (A != B) A += max(1, abs(A - B)/Step) * (A > B? -1: 1);

void CXTPTaskPanelGroup::OnAnimate(int nStep)
{
	if (nStep < 1)
	{
		m_rcGroupCurrent = m_rcGroupTarget;
	}
	else
	{
		MOVETO(m_rcGroupCurrent.top, m_rcGroupTarget.top, nStep);
		MOVETO(m_rcGroupCurrent.bottom, m_rcGroupTarget.bottom, nStep);
	}

	if (!IsDirty() && m_bExpanding)
	{
		m_bExpanding = FALSE;
		m_pPanel->NotifyOwner(XTP_TPN_GROUPEXPANDED, (LPARAM)this);
	}

	for (int i = 0; i < m_pItems->GetCount(); i++)
	{
		CXTPTaskPanelGroupItem* pItem = GetAt(i);

		pItem->OnAnimate(nStep);
	}
}

void CXTPTaskPanelGroup::OnPaint(CDC* pDC)
{
	GetPaintManager()->DrawGroupCaption(pDC, this, TRUE);

	GetPaintManager()->DrawGroupClient(pDC, this);
}

CRect CXTPTaskPanelGroup::GetCaptionRect()
{
	int nCaptionRight = m_rcGroupCurrent.right;
	if (m_pPanel->GetScrollButton(TRUE)->pGroupCaption == this)
		nCaptionRight = m_pPanel->GetScrollButton(TRUE)->rcButton.left - GetPaintManager()->GetGroupSpacing(m_pPanel) - 1;
	if (m_pPanel->GetScrollButton(FALSE)->pGroupCaption == this)
		nCaptionRight = m_pPanel->GetScrollButton(FALSE)->rcButton.left - GetPaintManager()->GetGroupSpacing(m_pPanel) - 1;

	return CRect(m_rcGroupCurrent.left, m_rcGroupCurrent.top, nCaptionRight, m_rcGroupCurrent.top + m_nCaptionHeight);
}

CRect CXTPTaskPanelGroup::GetTargetCaptionRect()
{
	int nCaptionRight = m_rcGroupTarget.right;
	if (m_pPanel->GetScrollButton(TRUE)->pGroupCaption == this)
		nCaptionRight = m_pPanel->GetScrollButton(TRUE)->rcButton.left - GetPaintManager()->GetGroupSpacing(m_pPanel) - 1;
	if (m_pPanel->GetScrollButton(FALSE)->pGroupCaption == this)
		nCaptionRight = m_pPanel->GetScrollButton(FALSE)->rcButton.left - GetPaintManager()->GetGroupSpacing(m_pPanel) - 1;

	return CRect(m_rcGroupTarget.left, m_rcGroupTarget.top, nCaptionRight, m_rcGroupTarget.top + m_nCaptionHeight);
}

CRect CXTPTaskPanelGroup::GetClientRect()
{
	return CRect(m_rcGroupCurrent.left, m_rcGroupCurrent.top  + m_nCaptionHeight, m_rcGroupCurrent.right, m_rcGroupCurrent.bottom);
}
CRect CXTPTaskPanelGroup::GetTargetClientRect()
{
	return CRect(m_rcGroupTarget.left, m_rcGroupTarget.top  + m_nCaptionHeight, m_rcGroupTarget.right, m_rcGroupTarget.bottom);
}

BOOL CXTPTaskPanelGroup::IsAcceptFocus()
{
	return CXTPTaskPanelItem::IsAcceptFocus() && IsExpandable();
}

void CXTPTaskPanelGroup::SetSpecialGroup(BOOL bSpecial)
{
	m_bSpecial = bSpecial;
	RedrawPanel();
}
BOOL CXTPTaskPanelGroup::IsSpecialGroup()
{
	return m_bSpecial;
}

void CXTPTaskPanelGroup::SetExpandable(BOOL bExpandable)
{
	m_bExpandable = bExpandable;

	RedrawPanel();
}
BOOL CXTPTaskPanelGroup::IsExpandable()
{
	return m_bExpandable && m_pPanel->m_bExpandable && m_bEnabled;
}

void CXTPTaskPanelGroup::SetExpanded(BOOL bExpanded)
{
	if (m_pPanel)
		m_pPanel->ExpandGroup(this, bExpanded);
}
BOOL CXTPTaskPanelGroup::IsExpanded()
{
	return m_pPanel? m_pPanel->IsGroupExpanded(this): FALSE;
}

BOOL CXTPTaskPanelGroup::IsExpanding()
{
	return m_bExpanding;
}
void CXTPTaskPanelGroup::ShowCaption(BOOL bShow)
{
	if (m_bCaptionVisible != bShow)
	{
		m_bCaptionVisible = bShow;
		RepositionPanel();
	}
}

BOOL CXTPTaskPanelGroup::IsCaptionVisible()
{
	return m_bCaptionVisible;
}

void CXTPTaskPanelGroup::SetClientBitmap(UINT nID, COLORREF clrTransparent, BOOL bAlpha /*= FALSE*/)
{
	m_ilClient.DeleteImageList();

	CBitmap bmp;
	bmp.LoadBitmap(nID);

	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);
	CSize szBitmap(bmpInfo.bmWidth, bmpInfo.bmHeight);


	if (bAlpha)
	{
		m_ilClient.Create(szBitmap.cx, szBitmap.cy, ILC_COLOR32|ILC_MASK, 0, 1);
		m_ilClient.Add(&bmp, (CBitmap*)NULL);
	}
	else
	{
		m_ilClient.Create(szBitmap.cx, szBitmap.cy, ILC_COLOR24|ILC_MASK, 0, 1);
		m_ilClient.Add(&bmp, clrTransparent);
	}
	m_szClientBitmap = szBitmap;
}
void CXTPTaskPanelGroup::OnFillClient(CDC* pDC, CRect rc)
{
	if (m_ilClient.GetSafeHandle() && m_ilClient.GetImageCount() == 1)
	{
		CPoint pt(rc.right - m_szClientBitmap.cx, rc.bottom - m_szClientBitmap.cy);
		m_ilClient.Draw(pDC, 0, pt, ILD_NORMAL);

	}
}

CRect CXTPTaskPanelGroup::GetHitTestRect()
{
	CRect rc = GetCaptionRect();
	rc.OffsetRect(0, -m_pPanel->GetScrollOffset());

	return rc;
}

CXTPTaskPanelGroupItem* CXTPTaskPanelGroup::HitTest(CPoint pt, CRect* lpRect)
{
	if (!IsExpanded())
		return NULL;

	int nOffset = m_pPanel->GetScrollOffset() - m_nCaptionHeight - m_rcGroupCurrent.top + GetScrollOffsetPos();
	pt.y += nOffset;

	for (int i = 0; i < m_pItems->GetCount(); i++)
	{
		CXTPTaskPanelGroupItem* pItem = GetAt(i);
		CRect rcItem = pItem->GetItemRect();

		if (rcItem.PtInRect(pt))
		{
			if (lpRect)
			{
				rcItem.OffsetRect(0, -nOffset);
				*lpRect = rcItem;
			}
			return pItem;
		}
	}
	return NULL;
}

XTPTaskPanelItemLayout CXTPTaskPanelGroup::GetItemLayout()
{
	if (m_itemLayout != xtpTaskItemLayoutDefault)
		return m_itemLayout;

	return m_pPanel->GetItemLayout();
}

void CXTPTaskPanelGroup::SetItemLayout(XTPTaskPanelItemLayout itemLayout)
{
	if (itemLayout != m_itemLayout)
	{
		m_itemLayout = itemLayout;
		RepositionPanel();
	}
}


void CXTPTaskPanelGroup::SetSelectedItem(CXTPTaskPanelGroupItem* pSelectedItem)
{
	int nCount = GetItems()->GetCount();

	for (int i = 0; i < nCount; i++)
	{
		CXTPTaskPanelGroupItem* pItem = GetAt(i);
		pItem->SetItemSelected(pSelectedItem == pItem);
	}
}

BOOL CXTPTaskPanelGroup::IsChildItemDragOver()
{
	return m_pPanel && m_pPanel->m_pItemDragOver &&
		m_pPanel->m_pItemDragOver->GetParentItems() == m_pItems;
}

int CXTPTaskPanelGroup::GetItemCount()
{
	return m_pItems->GetCount();
}

BOOL CXTPTaskPanelGroup::IsScrollButtonEnabled(BOOL bScrollButtonUp)
{
	if (GetItemCount() == 0)
		return FALSE;

	if (bScrollButtonUp)
		return m_nScrollOffset != 0;

	CRect rcItem(GetAt(GetItemCount() - 1)->GetItemRect());

	return rcItem.bottom - GetScrollOffsetPos() > m_rcGroupTarget.Height() - m_nCaptionHeight;
}

int CXTPTaskPanelGroup::GetScrollOffsetPos()
{
	if (m_nScrollOffset > 0 && m_nScrollOffset < GetItemCount())
	{
		return GetAt(m_nScrollOffset)->GetItemRect().top - GetPaintManager()->GetGroupInnerMargins(this).top;
	}
	return 0;
}

void CXTPTaskPanelGroup::Scroll(int nDelta)
{
	m_nScrollOffset += nDelta;

	if (m_nScrollOffset < 0)
		m_nScrollOffset = 0;

	if (m_nScrollOffset > GetItemCount() - 1)
		m_nScrollOffset = GetItemCount() - 1;

	m_pPanel->UpdateScrollButtons();
}

void CXTPTaskPanelGroup::EnsureVisible(CXTPTaskPanelGroupItem* pItem)
{
	if (m_pPanel->IsExplorerBehaviour() || GetItemCount() == 0 || !IsExpanded())
		return;

	if (!pItem || pItem->GetItemGroup() != this)
		return;

	int nIndex = pItem->GetIndex();

	if (m_nScrollOffset > nIndex)
	{
		m_nScrollOffset = nIndex - (nIndex % m_nItemsInRow);
		m_pPanel->UpdateScrollButtons();
		return;
	}

	CRect rcItem(pItem->GetItemRect());
	if (rcItem.bottom - GetScrollOffsetPos() > m_rcGroupTarget.Height() - m_nCaptionHeight)
	{
		int nTopMargin = GetPaintManager()->GetGroupInnerMargins(this).top;
		if (rcItem.top - GetScrollOffsetPos() <= nTopMargin)
			return;

		int nScrollOffset = m_nScrollOffset + 1;
		for (; nScrollOffset <= nIndex; nScrollOffset++)
		{
			int nScrollOffsetPos = GetAt(nScrollOffset)->GetItemRect().top - nTopMargin;

			if (rcItem.bottom - nScrollOffsetPos <= m_rcGroupTarget.Height() - m_nCaptionHeight)
				break;

			if (rcItem.top - nScrollOffsetPos <= nTopMargin)
				break;
		}
		m_nScrollOffset = nScrollOffset;
		m_pPanel->UpdateScrollButtons();
	}
}

void CXTPTaskPanelGroup::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPTaskPanelItem::DoPropExchange(pPX);

	BOOL bExpanded = IsExpanded();

	PX_Bool(pPX, _T("CaptionVisible"), m_bCaptionVisible, TRUE);
	PX_Bool(pPX, _T("Expandable"), m_bExpandable, TRUE);
	PX_Bool(pPX, _T("Expanded"), bExpanded, FALSE);
	PX_Bool(pPX, _T("Special"), m_bSpecial, FALSE);
	PX_Enum(pPX, _T("Layout"), m_itemLayout, xtpTaskItemLayoutDefault);
	PX_Rect(pPX, _T("MarginsInner"), m_rcMarginsInner, CXTPTaskPanelPaintManager::rectDefault);
	PX_Rect(pPX, _T("MarginsOuter"), m_rcMarginsOuter, CXTPTaskPanelPaintManager::rectDefault);
	PX_Size(pPX, _T("IconSize"), m_szItemIcon, CSize(0));

	if (pPX->IsLoading())
	{
		m_pItems->m_pPanel = m_pPanel;
		SetExpanded(bExpanded);
	}

	CXTPPropExchangeSection secItems(pPX->GetSection(_T("Items")));
	m_pItems->DoPropExchange(&secItems);
}



/////////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelGroup message handlers


