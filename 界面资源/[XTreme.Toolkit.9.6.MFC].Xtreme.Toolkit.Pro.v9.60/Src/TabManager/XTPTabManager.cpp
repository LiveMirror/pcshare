// XTPTabManager.cpp: implementation of the CXTPTabManager class.
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
#include "XTPVC80Helpers.h"
#include "XTPDrawHelpers.h"

#include "XTPTabManager.h"
#include "XTPTabPaintManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// CXTPTabManagerItem

CXTPTabManagerItem::CXTPTabManagerItem()
{
	EnableAutomation();


	m_bVisible = TRUE;
	m_bEnabled = TRUE;


	m_hIcon = 0;
	m_hWnd = 0;
	m_clrItem = 0;
	m_nIndex = -1;
	m_nImage = -1;
	m_dwData = 0;
	m_rcItem.SetRectEmpty();
}

void CXTPTabManagerItem::SetCaption(LPCTSTR lpszCaption)
{
	if (m_strCaption != lpszCaption)
	{
		m_strCaption = lpszCaption;
		m_pTabManager->Reposition();
	}
}
void CXTPTabManagerItem::SetColor(COLORREF clr)
{
	m_clrItem = clr;
	m_pTabManager->RedrawControl();
}

CString CXTPTabManagerItem::GetCaption()
{
	return m_pTabManager->GetItemCaption(this);
}

CString CXTPTabManagerItem::GetTooltip() const
{
	return m_pTabManager->GetItemTooltip((CXTPTabManagerItem* const)this);
}

COLORREF CXTPTabManagerItem::GetColor()
{
	return m_pTabManager->GetItemColor(this);
}
void CXTPTabManagerItem::SetHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}

void CXTPTabManagerItem::Remove()
{
	m_pTabManager->DeleteItem(m_nIndex);
}
void CXTPTabManagerItem::Select()
{
	m_pTabManager->OnItemClick(this);
}
HWND CXTPTabManagerItem::GetHandle()
{
	return m_hWnd;
}
BOOL CXTPTabManagerItem::IsVisible()
{
	return m_bVisible;
}

void CXTPTabManagerItem::SetVisible(BOOL bVisible)
{
	if (m_bVisible != bVisible)
	{
		m_bVisible = bVisible;
		m_pTabManager->Reposition();
	}
}

BOOL CXTPTabManagerItem::IsEnabled()
{
	return m_bEnabled;
}

void CXTPTabManagerItem::SetEnabled(BOOL bEnabled)
{
	if (m_bEnabled != bEnabled)
	{
		m_bEnabled = bEnabled;
		m_pTabManager->Reposition();
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPTabManagerNavigateButton

CXTPTabManagerNavigateButton::CXTPTabManagerNavigateButton(CXTPTabManager* pManager, UINT nID, XTPTabNavigateButtonFlags dwFlags)
{
	m_pManager = pManager;
	m_nID = nID;
	m_dwFlags = dwFlags;
	m_szButton = CSize(14, 15);

	m_rcButton.SetRectEmpty();

	m_bEnabled = TRUE;
	m_bPressed = FALSE;
	m_bHighlighted = FALSE;
}

void CXTPTabManagerNavigateButton::AdjustWidth(int& nWidth)
{
	if (m_dwFlags == xtpTabNavigateButtonAlways)
	{
		if ((m_pManager->GetPosition() == xtpTabPositionTop) || (m_pManager->GetPosition() == xtpTabPositionBottom))
		{
			nWidth -= m_szButton.cx;
		} else
		{
			nWidth -= m_szButton.cy;
		}
	}
}

void CXTPTabManagerNavigateButton::Reposition(CRect& rcNavigateButtons)
{
	if (m_dwFlags == xtpTabNavigateButtonAlways || m_dwFlags == xtpTabNavigateButtonAutomatic)
	{
		if (m_pManager->IsHorizontalPosition())
		{
			m_rcButton.SetRect(rcNavigateButtons.right - m_szButton.cx, rcNavigateButtons.CenterPoint().y + m_szButton.cy / 2 - m_szButton.cy, rcNavigateButtons.right, rcNavigateButtons.CenterPoint().y + m_szButton.cy / 2);
			rcNavigateButtons.right -= m_szButton.cx;
		} else
		{
			m_rcButton.SetRect(rcNavigateButtons.CenterPoint().x - m_szButton.cx / 2, rcNavigateButtons.bottom - m_szButton.cy, rcNavigateButtons.CenterPoint().x - m_szButton.cx / 2 + m_szButton.cx, rcNavigateButtons.bottom);
			rcNavigateButtons.bottom -= m_szButton.cy;
		}
	} else
		m_rcButton.SetRectEmpty();
}

void CXTPTabManagerNavigateButton::Draw(CDC* pDC)
{
	if (!m_rcButton.IsRectEmpty())
	{
		CRect rc(m_rcButton);
		m_pManager->GetPaintManager()->FillNavigateButton(pDC, this, rc);

		COLORREF clr = pDC->GetTextColor();
		CXTPPenDC pen(*pDC, clr);

		if (IsEnabled())
		{
			CXTPBrushDC brush(*pDC, clr);
			DrawEntry(pDC, rc);
		} else
		{
			CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
			DrawEntry(pDC, rc);
			pDC->SelectObject(pOldBrush);
		}
	}
}

void CXTPTabManagerNavigateButton::PerformClick(HWND hWnd, CPoint pt)
{
	if ( (::GetCapture() != NULL ) || !m_bEnabled)
		return;

	::SetCapture(hWnd);

	BOOL  bClick = FALSE;

	DWORD dwStart = GetTickCount();
	while ( TRUE )
	{
		if (m_bEnabled && GetTickCount() - dwStart > 20)
		{
			dwStart = GetTickCount();
			OnExecute(TRUE);
		}

		BOOL bPressed = m_rcButton.PtInRect(pt);

		if ( bPressed != m_bPressed )
		{
			m_bPressed = bPressed;
			m_pManager->RedrawControl();
		}
		MSG msg;

		if (!::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
			continue;

		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if ( ::GetCapture() != hWnd )
		{
			DispatchMessage (&msg);
			goto ExitLoop;
		}
		switch ( msg.message )
		{
			case WM_MOUSEMOVE:
				pt = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));
				break;

			case WM_LBUTTONUP:
				bClick = m_bPressed;
				goto ExitLoop;

			case WM_KEYDOWN:
				if ( msg.wParam != VK_ESCAPE )
					break;

			case WM_CANCELMODE:
			case WM_RBUTTONDOWN:
				goto ExitLoop;

			default:
				DispatchMessage (&msg);
				break;
		}
	}

ExitLoop:
	ReleaseCapture();
	m_bPressed = FALSE;
	PerformMouseMove(hWnd, pt);
	m_pManager->RedrawControl();

	if (bClick)
	{
		OnExecute(FALSE);
	}
}


void CXTPTabManagerNavigateButton::PerformMouseMove(HWND hWnd, CPoint pt)
{
	if (!m_rcButton.PtInRect(pt) && m_bHighlighted)
	{
		m_bHighlighted = FALSE;
		m_pManager->RedrawControl();
	}
	if (!m_bHighlighted && m_rcButton.PtInRect (pt) && IsEnabled())
	{
		m_bHighlighted = TRUE;
		m_pManager->RedrawControl();

		TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, hWnd};
		_TrackMouseEvent(&tme);
	}
}

void CXTPTabManagerNavigateButton::OnExecute(BOOL bTick)
{
	if (!bTick)
	{
		m_pManager->OnNavigateButtonClick(m_nID);
	}
}

//////////////////////////////////////////////////////////////////////////

class CXTPTabManager::CNavigateButtonArrow : public CXTPTabManagerNavigateButton
{
protected:
	CNavigateButtonArrow(CXTPTabManager* pManager, XTPTabNavigateButton nID, XTPTabNavigateButtonFlags dwFlags)
		: CXTPTabManagerNavigateButton(pManager, nID, dwFlags)
	{
	}
	virtual void Reposition(CRect& rcNavigateButtons);

};

class CXTPTabManager::CNavigateButtonArrowLeft : public CNavigateButtonArrow
{
public:
	CNavigateButtonArrowLeft(CXTPTabManager* pManager, XTPTabNavigateButtonFlags dwFlags)
		: CNavigateButtonArrow(pManager, xtpTabNavigateButtonLeft, dwFlags)
	{
	}
protected:
	virtual void DrawEntry(CDC* pDC, CRect rc);
	virtual void Reposition(CRect& rcNavigateButtons);
	virtual void OnExecute(BOOL bTick);
};

class CXTPTabManager::CNavigateButtonArrowRight : public CNavigateButtonArrow
{
public:
	CNavigateButtonArrowRight(CXTPTabManager* pManager, XTPTabNavigateButtonFlags dwFlags)
		: CNavigateButtonArrow(pManager, xtpTabNavigateButtonRight, dwFlags)
	{
	}
protected:
	virtual void DrawEntry(CDC* pDC, CRect rc);
	virtual void Reposition(CRect& rcNavigateButtons);
	virtual void OnExecute(BOOL bTick);
};

class CXTPTabManager::CNavigateButtonClose : public CXTPTabManagerNavigateButton
{
public:
	CNavigateButtonClose(CXTPTabManager* pManager, XTPTabNavigateButtonFlags dwFlags)
		: CXTPTabManagerNavigateButton(pManager, xtpTabNavigateButtonClose, dwFlags)
	{
	}
protected:
	virtual void DrawEntry(CDC* pDC, CRect rc);
};


void CXTPTabManager::CNavigateButtonClose::DrawEntry(CDC* pDC, CRect rc)
{
	pDC->MoveTo(rc.left + 3, rc.top + 3);
	pDC->LineTo(rc.right - 4, rc.bottom - 5);
	pDC->MoveTo(rc.left + 4, rc.top + 3);
	pDC->LineTo(rc.right - 3, rc.bottom - 5);

	pDC->MoveTo(rc.left + 3, rc.bottom  - 6);
	pDC->LineTo(rc.right - 4, rc.top + 2);
	pDC->MoveTo(rc.left + 4, rc.bottom  - 6);
	pDC->LineTo(rc.right - 3, rc.top + 2);
}

void CXTPTabManager::CNavigateButtonArrowRight::DrawEntry(CDC* pDC, CRect rc)
{
	CPoint pt = rc.CenterPoint();

	if (m_pManager->IsHorizontalPosition())
		CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x - 2, pt.y - 5), CPoint(pt.x + 2, pt.y - 1), CPoint(pt.x - 2, pt.y + 3));
	else
		CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x - 5, pt.y - 2), CPoint(pt.x - 1, pt.y + 2), CPoint(pt.x + 3, pt.y - 2));
}
void CXTPTabManager::CNavigateButtonArrow::Reposition(CRect& rcNavigateButtons)
{
	if (m_dwFlags == xtpTabNavigateButtonAutomatic)
	{
		if (m_pManager->GetLayout() == xtpTabLayoutSizeToFit)
		{
			m_rcButton.SetRectEmpty();
			return;
		}

		CRect rc = m_pManager->GetAppearanceSet()->GetHeaderMargin();

		if (!(m_pManager->GetItemsLength() + m_pManager->GetHeaderOffset() - rc.left - 1> m_pManager->GetRectLength(rcNavigateButtons)  || m_pManager->GetHeaderOffset() < 0))
		{
			m_rcButton.SetRectEmpty();
			return;
		}
	}

	CXTPTabManagerNavigateButton::Reposition(rcNavigateButtons);

}
void CXTPTabManager::CNavigateButtonArrowRight::Reposition(CRect& rcNavigateButtons)
{
	m_bEnabled = m_pManager->GetItemsLength() + m_pManager->GetHeaderOffset() > m_pManager->GetRectLength(rcNavigateButtons) - 28;

	CNavigateButtonArrow::Reposition(rcNavigateButtons);
}

void CXTPTabManager::CNavigateButtonArrowRight::OnExecute(BOOL bTick)
{
	if (bTick)
	{
		m_pManager->SetHeaderOffset(m_pManager->GetHeaderOffset() - 20);
	}
}


void CXTPTabManager::CNavigateButtonArrowLeft::DrawEntry(CDC* pDC, CRect rc)
{
	CPoint pt = rc.CenterPoint();

	if (m_pManager->IsHorizontalPosition())
		CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x + 2, pt.y - 5), CPoint(pt.x - 2, pt.y - 1), CPoint(pt.x + 2, pt.y + 3));
	else
		CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x - 5, pt.y + 2), CPoint(pt.x - 1, pt.y - 2), CPoint(pt.x + 3, pt.y + 2));
}
void CXTPTabManager::CNavigateButtonArrowLeft::Reposition(CRect& rcNavigateButtons)
{
	m_bEnabled = m_pManager->GetHeaderOffset() < 0;

	CNavigateButtonArrow::Reposition(rcNavigateButtons);
}

void CXTPTabManager::CNavigateButtonArrowLeft::OnExecute(BOOL bTick)
{
	if (bTick)
	{
		m_pManager->SetHeaderOffset(m_pManager->GetHeaderOffset() + 20);
	}
}



//////////////////////////////////////////////////////////////////////
// CXTPTabManager

CXTPTabManager::CXTPTabManager()
{

	m_pSelected = 0;
	m_pHighlighted = 0;

	m_nHeaderOffset = 0;
	m_nItemColor = 0;

	m_bAllowReorder = TRUE;

	m_szItemNormal = CSize(70, 18);
	m_szItemMin = m_szItemMax = CSize(0, 0);

	m_bActive = TRUE;

	m_rcHeaderRect.SetRectEmpty();
	m_rcControl.SetRectEmpty();
	m_rcClient.SetRectEmpty();


	m_arrNavigateButtons.Add(new CNavigateButtonArrowLeft(this, xtpTabNavigateButtonAutomatic));
	m_arrNavigateButtons.Add(new CNavigateButtonArrowRight(this, xtpTabNavigateButtonAutomatic));
	m_arrNavigateButtons.Add(new CNavigateButtonClose(this, xtpTabNavigateButtonNone));
}

CXTPTabManager::~CXTPTabManager()
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
		m_arrItems[i]->InternalRelease();

	for (int j = 0; j < m_arrNavigateButtons.GetSize(); j++)
		delete m_arrNavigateButtons[j];
}

void CXTPTabManager::SetActive(BOOL bActive)
{
	if (m_bActive != bActive)
	{
		m_bActive = bActive;
		Reposition();
	}
}

void CXTPTabManager::EnableTabThemeTexture(HWND hWnd, BOOL bEnable)
{
	m_themeTabControl.OpenTheme(hWnd, L"TAB");
	m_themeTabControl.EnableThemeDialogTexture(hWnd, bEnable? ETDT_ENABLETAB: ETDT_DISABLE|ETDT_USETABTEXTURE);

}

CXTPTabManagerNavigateButton* CXTPTabManager::FindNavigateButton(UINT nID)
{
	for (int i = 0; i < m_arrNavigateButtons.GetSize(); i++)
	{
		if (m_arrNavigateButtons[i]->m_nID == nID)
			return m_arrNavigateButtons[i];
	}

	return NULL;
}

void CXTPTabManager::SetHeaderOffset(int nOffset)
{
	if (nOffset > 0) nOffset = 0;

	if (nOffset != m_nHeaderOffset)
	{
		m_nHeaderOffset = nOffset;
		Reposition();
	}
}

int CXTPTabManager::GetItemsLength()
{
	int nLength = 0;

	for (int i = 0; i < GetItemCount(); i++)
		nLength += GetItem(i)->m_nLength;

	return nLength;
}


void CXTPTabManager::DeleteAllItems()
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
		m_arrItems[i]->InternalRelease();

	m_arrItems.RemoveAll();

	m_pHighlighted = m_pSelected = NULL;

	OnItemsChanged();
}

BOOL CXTPTabManager::DeleteItem(int nItem)
{
	if (nItem < 0 || nItem >= GetItemCount())
		return FALSE;

	BOOL bSelected = m_pSelected == m_arrItems[nItem];
	m_arrItems[nItem]->InternalRelease();

	m_arrItems.RemoveAt(nItem);

	if (bSelected)
	{
		SetCurSel(nItem);
	}


	OnItemsChanged();

	return TRUE;
}

CXTPTabManagerItem* CXTPTabManager::AddItem(int nItem, CXTPTabManagerItem* pItem /*= NULL*/)
{
	if (!pItem)
		pItem = new CXTPTabManagerItem();

	pItem->m_pTabManager = this;

	if (nItem < 0 || nItem > GetItemCount())
		nItem = GetItemCount();

	m_arrItems.InsertAt(nItem, pItem);
	pItem->m_clrItem = xtpTabColorBlue + (m_nItemColor++ % 8);

	OnItemsChanged();

	return pItem;
}

void CXTPTabManager::OnItemsChanged()
{
	for (int i = 0; i < GetItemCount(); i++)
	{
		GetItem(i)->m_nIndex = i;
	}

	Reposition();
}

CString CXTPTabManager::GetItemCaption(CXTPTabManagerItem* pItem)
{
	return pItem->m_strCaption;
}
HICON CXTPTabManager::GetItemIcon(CXTPTabManagerItem* pItem)
{
	return pItem->m_hIcon;
}

void CXTPTabManager::ShowIcons(BOOL bShowIcons)
{
	GetPaintManager()->m_bShowIcons = bShowIcons;
	Reposition();
}

COLORREF CXTPTabManager::GetItemColor(CXTPTabManagerItem* pItem)
{
	COLORREF nColor = pItem->m_clrItem;

	if (nColor >= xtpTabColorBlue && nColor <= xtpTabColorMagenta)
		return CXTPTabPaintManager::GetOneNoteColor((XTPTabOneNoteColor)nColor);

	return nColor;
}

void CXTPTabManager::SetCurSel(int nIndex)
{
	if (GetItemCount() != 0)
	{
		nIndex = nIndex < 0? 0: nIndex >= GetItemCount()? GetItemCount() - 1: nIndex;

		SetSelectedItem(GetItem(nIndex));
	}
	else
	{
		SetSelectedItem(NULL);
	}
}

int CXTPTabManager::GetCurSel()
{
	if (m_pSelected)
	{
		return m_pSelected->GetIndex();
	}
	return -1;
}

void CXTPTabManager::SetSelectedItem(CXTPTabManagerItem* pItem)
{
	if (m_pSelected != pItem)
	{
		m_pSelected = pItem;
		Reposition();
		EnsureVisible(pItem);
	}
}

void CXTPTabManager::EnsureVisible(CXTPTabManagerItem* pItem)
{
	if (!pItem)
		return;

	GetPaintManager()->EnsureVisible(this, pItem);
}

void CXTPTabManager::SetPosition(XTPTabPosition tabPosition)
{
	GetPaintManager()->m_tabPosition = tabPosition;
	Reposition();
}

void CXTPTabManager::SetLayoutStyle(XTPTabLayoutStyle tabLayout)
{
	GetPaintManager()->m_tabLayout = tabLayout;
	Reposition();
}


CXTPTabManagerItem* CXTPTabManager::HitTest(CPoint point) const
{
	if (!m_rcControl.PtInRect(point))
		return NULL;

	if (!m_rcHeaderRect.IsRectEmpty() && !m_rcHeaderRect.PtInRect(point))
		return NULL;

	for (int i = 0; i < GetItemCount(); i++)
	{
		CXTPTabManagerItem* pItem = GetItem(i);

		if (pItem->GetRect().PtInRect(point) && pItem->IsEnabled() && pItem->IsVisible())
		{
			return pItem;
		}
	}
	return NULL;
}

XTPTabPosition CXTPTabManager::GetPosition()
{
	return GetPaintManager()->m_tabPosition;
}
XTPTabLayoutStyle CXTPTabManager::GetLayout()
{
	return GetPaintManager()->m_tabLayout;
}

void CXTPTabManager::MoveItem(CXTPTabManagerItem* pItem, int nIndex)
{
	int nOldIndex = pItem->GetIndex();
	ASSERT(nOldIndex >= 0 && pItem->GetTabManager() == this);
	ASSERT(nIndex >= 0 && nIndex < GetItemCount());

	m_arrItems.RemoveAt(nOldIndex);
	m_arrItems.InsertAt(nIndex, pItem);

	OnItemsChanged();
}

void CXTPTabManager::ReOrder(HWND hWnd, CPoint pt, CXTPTabManagerItem* pItem)
{
	CXTPTabManagerItem* pSelected = m_pSelected;
	m_pSelected = pItem;

	if (GetPaintManager()->m_bHotTracking)
		m_pHighlighted = pItem;

	Reposition();

	CArray<CRect, CRect&> arrRects;

	for (int j = 0; j < GetItemCount(); j++)
	{
		CRect rc = GetItem(j)->GetRect();
		arrRects.Add(rc);
	}

	::SetCapture(hWnd);

	while ( TRUE )
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if ( ::GetCapture() != hWnd )
		{
			DispatchMessage (&msg);
			goto ExitLoop;
		}
		switch ( msg.message )
		{
			case WM_MOUSEMOVE:
				{
					pt = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));

					for (int i = 0; i < arrRects.GetSize(); i++)
					{
						if (i != pItem->GetIndex() && arrRects[i].PtInRect(pt))
						{

							CXTPTabManagerItem* p = pItem;
							m_arrItems[pItem->GetIndex()] = m_arrItems[i];
							m_arrItems[i] = p;

							OnItemsChanged();
							break;
						}
					}
				}

				break;

			case WM_KEYDOWN:
				if ( msg.wParam != VK_ESCAPE )
					break;
			case WM_CANCELMODE:
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
				goto ExitLoop;

			default:
				DispatchMessage (&msg);
				break;
		}
	}

ExitLoop:
	ReleaseCapture();
	PerformMouseMove(hWnd, pt);

	m_pSelected = pSelected;
	OnItemClick(pItem);
}

void CXTPTabManager::OnItemClick(CXTPTabManagerItem* pItem)
{
	SetSelectedItem(pItem);
}

BOOL CXTPTabManager::PerformClick(HWND hWnd, CPoint pt, BOOL bNoItemClick)
{
	for (int i = 0; i < m_arrNavigateButtons.GetSize(); i++)
	{
		if (m_arrNavigateButtons[i]->m_rcButton.PtInRect(pt))
		{
			m_arrNavigateButtons[i]->PerformClick(hWnd, pt);
			return TRUE;
		}
	}

	if (bNoItemClick)
		return FALSE;

	CXTPTabManagerItem* pItem = HitTest(pt);

	if (pItem)
	{
		if (IsAllowReorder())
		{
			ReOrder(hWnd, pt, pItem);
		}
		else
		{
			OnItemClick(pItem);
		}
		return TRUE;
	}

	return FALSE;
}


void CXTPTabManager::PerformMouseMove(HWND hWnd, CPoint pt)
{
	if (!CWnd::FromHandle(hWnd)->IsTopParentActive())
		return;

	if (IsMouseLocked())
		return;

	if (GetPaintManager()->m_bHotTracking)
	{

		CXTPTabManagerItem* pItem = HitTest(pt);

		if (pItem != m_pHighlighted)
		{
			m_pHighlighted = pItem;
			RedrawControl();

			if (pItem)
			{
				TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, hWnd};
				_TrackMouseEvent(&tme);
			}
		}
	}

	for (int i = 0; i < m_arrNavigateButtons.GetSize(); i++)
		m_arrNavigateButtons[i]->PerformMouseMove(hWnd, pt);
}

CString CXTPTabManager::GetItemTooltip(CXTPTabManagerItem* pItem) const
{
	return pItem->m_strToolTip;
}

INT_PTR CXTPTabManager::PerformToolHitTest(HWND hWnd, CXTPTabManagerItem* pItem, TOOLINFO* pTI) const
{
	if (pItem)
	{
		CString strTip = GetItemTooltip(pItem);
		if (strTip.IsEmpty())
			return -1;

	#if _MSC_VER < 1200 // MFC 5.0
		if (pTI != NULL && pTI->cbSize >= sizeof(TOOLINFO))
	#else
		if (pTI != NULL && pTI->cbSize >= sizeof(AFX_OLDTOOLINFO))
	#endif
		{
			pTI->hwnd = hWnd;
			pTI->rect = pItem->GetRect();
			pTI->uId = pItem->GetIndex();
			pTI->lpszText = (LPTSTR) ::calloc(strTip.GetLength() + 1, sizeof(TCHAR));
			if( pTI->lpszText != NULL ) STRCPY_S(pTI->lpszText, strTip.GetLength() + 1, (LPCTSTR)strTip);
		}
		// found matching rect, return the ID of the button
		return pItem->GetIndex();
	}
	return -1;
}

void CXTPTabManager::GetItemMetrics(CSize* lpszNormal, CSize* lpszMin /*= NULL*/, CSize* lpszMax /*= NULL*/)
{
	if (lpszNormal) *lpszNormal = m_szItemNormal;
	if (lpszMin) *lpszMin = m_szItemMin;
	if (lpszMax) *lpszMax = m_szItemMax;
}

void CXTPTabManager::SetItemMetrics(CSize szNormal, CSize szMin /*= CSize(0, 0)*/, CSize szMax /*= CSize(0, 0)*/)
{
	m_szItemNormal = szNormal;
	m_szItemMin = szMin;
	m_szItemMax = szMax;
	Reposition();
}

BOOL CXTPTabManager::IsDrawStaticFrame()
{
	return GetPaintManager()->m_bStaticFrame;
}


CXTPTabPaintManager::CColorSet* CXTPTabManager::SetColor(XTPTabColorStyle tabColor)
{
	CXTPTabPaintManager::CColorSet* pColorSet = GetPaintManager()->SetColor(tabColor);
	Reposition();
	return pColorSet;
}

CXTPTabPaintManager::CColorSet* CXTPTabManager::SetColorSet(CXTPTabPaintManager::CColorSet* pColorSet)
{
	GetPaintManager()->SetColorSet(pColorSet);
	Reposition();
	return pColorSet;
}
CXTPTabPaintManager::CAppearanceSet* CXTPTabManager::SetAppearance(XTPTabAppearanceStyle tabAppearance)
{
	CXTPTabPaintManager::CAppearanceSet* pAppearanceSet = GetPaintManager()->SetAppearance(tabAppearance);
	Reposition();
	return pAppearanceSet;
}
CXTPTabPaintManager::CAppearanceSet* CXTPTabManager::SetAppearanceSet(CXTPTabPaintManager::CAppearanceSet* pAppearanceSet)
{
	GetPaintManager()->SetAppearanceSet(pAppearanceSet);
	Reposition();
	return pAppearanceSet;
}

