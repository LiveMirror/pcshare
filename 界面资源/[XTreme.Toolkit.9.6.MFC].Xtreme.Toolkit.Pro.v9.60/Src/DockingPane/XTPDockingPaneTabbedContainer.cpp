// XTPDockingPaneTabbedContainer.cpp : implementation of the CXTPDockingPaneTabbedContainer class.
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
#include "XTPResourceManager.h"
#include "XTPVC80Helpers.h"
#include "XTPDrawHelpers.h"
#include "XTPTabManager.h"

#include "XTPDockingPaneTabbedContainer.h"
#include "XTPDockingPane.h"
#include "XTPDockingPaneContext.h"
#include "XTPDockingPaneManager.h"
#include "XTPDockingPanePaintManager.h"
#include "XTPDockingPaneLayout.h"
#include "XTPDockingPaneAutoHidePanel.h"
#include "XTPDockingPaneMiniWnd.h"
#include "XTPDockingPaneResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CTabManagerDropTarget

class CXTPDockingPaneTabbedContainer::CContainerDropTarget : public COleDropTarget
{
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* /*pDataObject*/, DWORD /*dwKeyState*/, CPoint point)
	{
		CXTPDockingPaneTabbedContainer* pControl = (CXTPDockingPaneTabbedContainer*)pWnd;
		ASSERT_VALID(pControl);

		CXTPTabManagerItem* pItem = pControl->CXTPTabManager::HitTest(point);

		if (pItem)
		{
			CXTPDockingPane* pPane = pControl->GetItemPane(pItem->GetIndex());
			if (pControl->GetSelected() != pPane)
			{
				pControl->SelectPane(pPane, FALSE);
				pControl->m_pParentFrame->RecalcLayout();
			}
		}

		return DROPEFFECT_NONE;
	}
};

// CXTPDockingPaneTabbedContainer

void CXTPDockingPaneTabbedContainer::RedrawControl()
{
	if (!m_bLockReposition)
	{
		if (GetSafeHwnd()) Invalidate(FALSE);
	}
}
void CXTPDockingPaneTabbedContainer::Reposition()
{
	if (!m_bLockReposition)
	{
		GetDockingPaneManager()->RedrawPanes();
	}

}
CXTPTabPaintManager* CXTPDockingPaneTabbedContainer::GetPaintManager()
{
	return CXTPDockingPaneBase::GetPaintManager()->GetTabPaintManager();
}

void CXTPDockingPaneTabbedContainer::SetPaintManager(CXTPTabPaintManager* /*pPaintManager*/)
{

}

BOOL CXTPDockingPaneTabbedContainer::DrawIcon(CDC* pDC, CPoint pt, CXTPTabManagerItem* pItem, BOOL bDraw, CSize& szIcon)
{
	if (!pItem)
		return TRUE;

	if (!CXTPDockingPaneBase::GetPaintManager()->GetTabPaintManager()->m_bShowIcons)
		return FALSE;

	CXTPImageManagerIcon* pImage = ((CXTPDockingPane*)pItem->GetData())->GetIcon(szIcon.cx);

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



IMPLEMENT_DYNAMIC(CXTPDockingPaneTabbedContainer, CWnd)

CXTPDockingPaneTabbedContainer::CXTPDockingPaneTabbedContainer(CXTPDockingPaneLayout* pLayout)
	: CXTPDockingPaneBase(xtpPaneTypeTabbedContainer, pLayout)
{

	m_pSelectedPane = 0;
	m_pTrackingPane = 0;

	m_bLockReposition = FALSE;

	m_bActive = FALSE;
	m_bTitleVisible = TRUE;

	m_arrCaptionButtons.Add(new CXTPDockingPaneCaptionButton(XTP_IDS_DOCKINGPANE_CLOSE, this));
	m_arrCaptionButtons.Add(new CXTPDockingPaneCaptionButton(XTP_IDS_DOCKINGPANE_AUTOHIDE, this));

	m_pDropTarget = new CContainerDropTarget();

}

CXTPDockingPaneCaptionButton* CXTPDockingPaneTabbedContainer::GetCloseButton() {
	return FindCaptionButton(XTP_IDS_DOCKINGPANE_CLOSE);
}
CXTPDockingPaneCaptionButton* CXTPDockingPaneTabbedContainer::GetPinButton() {
	return FindCaptionButton(XTP_IDS_DOCKINGPANE_AUTOHIDE);
}


void CXTPDockingPaneTabbedContainer::Init(CXTPDockingPane* pPane, CFrameWnd* pFrame)
{
	ASSERT(pPane);

	m_pParentFrame = pFrame;
	m_szDocking = pPane->m_szDocking;
	m_rcWindow = pPane->m_rcWindow;

	_InsertPane(pPane);
	SelectPane(pPane);
}

CXTPDockingPaneTabbedContainer::~CXTPDockingPaneTabbedContainer()
{
	if (m_hWnd) DestroyWindow();

	delete m_pDropTarget;
}

void CXTPDockingPaneTabbedContainer::SetParentFrame(CFrameWnd* pFrame)
{
	m_pParentFrame = pFrame;
	if (m_hWnd)
	{
		if (GetParent() != pFrame) CWnd::SetParent(pFrame);
	}
}

BOOL CXTPDockingPaneTabbedContainer::IsTabsVisible()
{
	return m_hWnd && !DYNAMIC_DOWNCAST(CXTPDockingPaneAutoHideWnd, GetParent())
		&& (GetItemCount() > 1 || CXTPDockingPaneBase::GetPaintManager()->m_bDrawSingleTab);
}


void CXTPDockingPaneTabbedContainer::OnSizeParent(CFrameWnd* pParent, CRect rect, LPVOID lParam)
{
	AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;

	ASSERT(!IsEmpty());

	SetParentFrame(pParent);
	m_rcWindow = rect;

	if (lpLayout == 0 || lpLayout->hDWP != NULL)
	{
		//MoveWindow(&rect);
		SetWindowPos(&CWnd::wndBottom, rect.left, rect.top, rect.Width(), rect.Height(), 0);
		Invalidate(FALSE);

		m_bLockReposition += 1;
		_UpdateTabs();
		m_bLockReposition -= 1;

		GetPinButton()->SetState(IsHide()? xtpPanePinVisible|xtpPanePinPushed: DYNAMIC_DOWNCAST(CXTPDockingPaneMiniWnd, pParent) == 0? xtpPanePinVisible: 0);

		CXTPDockingPaneBase::GetPaintManager()->AdjustClientRect(this, rect, TRUE);

		POSITION pos = m_lstPanes.GetHeadPosition();
		while(pos)
		{
			CXTPDockingPane* pPane = m_lstPanes.GetNext(pos);
			pPane->ShowWindow(m_pSelectedPane == pPane);
			pPane->OnSizeParent(pParent, rect, lParam);
		}

	}
}



void CXTPDockingPaneTabbedContainer::_UpdateTabs()
{
	if (!m_hWnd)
		return;

	m_bLockReposition += 1;

	DeleteAllItems();

	POSITION pos = m_lstPanes.GetHeadPosition();
	while(pos)
	{
		CXTPDockingPane* pPane = m_lstPanes.GetNext(pos);

		CXTPTabManagerItem* pItem = AddItem(GetItemCount());
		if (m_pSelectedPane == pPane) SetSelectedItem(pItem);

		pItem->SetCaption(pPane->GetShortTitle());
		pItem->SetColor(pPane->GetItemColor());
		pItem->SetTooltip(pPane->GetTitle());

		pItem->SetData((DWORD_PTR)pPane);
	}
	//////////////////////////////////////////////////////////////////////////

	m_arrCaptionButtons.CheckForMouseOver(CPoint(-1, -1));

	m_bLockReposition -= 1;
}

void CXTPDockingPaneTabbedContainer::_InsertPane(CXTPDockingPane* pPane)
{
	ASSERT(m_pLayout);
	if (!m_hWnd && !m_pLayout->IsUserLayout())
	{
		Create(AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(0, IDC_ARROW)), _T(""), WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), m_pParentFrame, 0);
		EnableToolTips();
		m_pDropTarget->Register(this);


	}

	m_lstPanes.AddTail(pPane);

	pPane->SetParentContainer(this);
	pPane->SetParentFrame(m_pParentFrame);

	DELAYRECALCLAYOUT(m_pParentFrame);

	SelectPane(pPane);

	_UpdateTabs();

	SAFE_CALLPTR(m_pParentContainer, OnChildConainerChanged(this));

}

void CXTPDockingPaneTabbedContainer::RemovePane(CXTPDockingPaneBase* pPane)
{
	ASSERT(pPane->GetContainer() == this);

	POSITION pos = m_lstPanes.Find((CXTPDockingPane*)pPane);
	ASSERT(pos);

	m_lstPanes.RemoveAt(pos);
	_UpdateTabs();

	pPane->SetParentContainer(NULL);

	m_pParentContainer->OnChildConainerChanged(this);

	if (m_lstPanes.IsEmpty())
	{
		DestroyWindow();
	}

	DELAYRECALCLAYOUT(m_pParentFrame);

	if (pPane == m_pSelectedPane)
	{
		SelectPane(m_lstPanes.GetCount() > 0? m_lstPanes.GetTail(): 0);
	}
}


BEGIN_MESSAGE_MAP(CXTPDockingPaneTabbedContainer, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()



// CXTPDockingPaneTabbedContainer message handlers


void CXTPDockingPaneTabbedContainer::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint, rc);

	CXTPDockingPaneBase::GetPaintManager()->DrawPane(dc, this, rc);
}


void CXTPDockingPaneTabbedContainer::ShowTitle(BOOL bShow)
{
	m_bTitleVisible = bShow;
	DELAYRECALCLAYOUT(m_pParentFrame);
}

BOOL CXTPDockingPaneTabbedContainer::IsTitleVisible()
{
	return m_bTitleVisible &&
		m_pSelectedPane && ((m_pSelectedPane->GetOptions() & xtpPaneNoCaption) == 0);
}


CString CXTPDockingPaneTabbedContainer::GetTitle()
{
	return m_pSelectedPane? m_pSelectedPane->GetTitle(): _T("");
}

void CXTPDockingPaneTabbedContainer::OnIdleUpdate()
{
	if (!GetSafeHwnd())
		return;

	CWnd* pFocus = GetFocus();
	BOOL bActive = (pFocus->GetSafeHwnd() && (pFocus == this || IsChild(pFocus) ||
		(pFocus->GetOwner()->GetSafeHwnd() && IsChild(pFocus->GetOwner()))));

	if (bActive != m_bActive)
	{
		m_bActive = bActive;
		Invalidate(FALSE);
	}

	if (m_pSelectedPane && m_pSelectedPane->GetChild())
	{
		CWnd* pWnd = m_pSelectedPane->GetChild();
		if (pWnd && pWnd->GetSafeHwnd())
		{
			pWnd->SendMessage(WM_IDLEUPDATECMDUI, 0, 0);
		}
	}
}

#define DOCKINGPANE_HITCAPTION -2

int CXTPDockingPaneTabbedContainer::HitTest(CPoint point)
{
	CXTPClientRect rc(this);

	if (IsTitleVisible())
	{
		if (rc.PtInRect(point))
		{
			CXTPDockingPaneBase::GetPaintManager()->AdjustCaptionRect(this, rc);
			if (!rc.PtInRect(point))
			{
				return DOCKINGPANE_HITCAPTION;
			}
		}
	}

	if (IsTabsVisible())
	{
		CXTPTabManagerItem* pItem =  CXTPTabManager::HitTest(point);
		return pItem? pItem->GetIndex(): -1;
	}
	return -1;
}

void CXTPDockingPaneTabbedContainer::SelectPane(CXTPDockingPane* pPane, BOOL bSetFocus)
{
	if (m_pSelectedPane == pPane)
		return;

	m_pSelectedPane = pPane;
	_UpdateTabs();

	DELAYRECALCLAYOUT(m_pParentFrame);
	SAFE_CALLPTR(m_pParentContainer, OnChildConainerChanged(this));
	if (bSetFocus)
	{
		SAFE_CALLPTR(pPane, SetFocus());
	}
}

void CXTPDockingPaneTabbedContainer::OnRButtonDown(UINT /*nFlags*/, CPoint point)
{
	int nHit = HitTest(point);
	if (nHit >= 0)
	{
		SelectPane(GetItemPane(nHit));
		m_pParentFrame->RecalcLayout();
	}

	if (m_pSelectedPane) m_pSelectedPane->SetFocus(); else SetFocus();
	GetDockingPaneManager()->UpdatePanes();

	GetDockingPaneManager()->GetSite()->SendMessage(XTPWM_DOCKINGPANE_NOTIFY, XTP_DPN_RCLICK, (LPARAM)(CXTPDockingPaneBase*)this);
}

void CXTPDockingPaneTabbedContainer::OnCaptionLButtonDown(CPoint point)
{
	if (m_pSelectedPane) m_pSelectedPane->SetFocus(); else SetFocus();
	GetDockingPaneManager()->UpdatePanes();

	CXTPDockingPaneContext* pContext = GetDockingPaneManager()->GetDockingContext();
	pContext->Drag(this, point);
}

void CXTPDockingPaneTabbedContainer::OnCaptionButtonClick(CXTPDockingPaneCaptionButton* pButton)
{
	CXTPDockingPane* pPane = m_pSelectedPane;

	CXTPDockingPaneManager* pManager = GetDockingPaneManager();

	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_CLOSE)
	{
		if (pManager->m_bCloseGroupOnButtonClick)
		{
			POSITION pos = m_lstPanes.GetTailPosition();
			while (pos)
			{
				CXTPDockingPane* pPane = (CXTPDockingPane*)m_lstPanes.GetPrev(pos);

				if ((pPane->GetOptions() & xtpPaneNoCloseable) != 0)
					continue;

				if (!pManager->_OnAction(xtpPaneActionClosing, pPane))
				{
					pPane->Close();
					pManager->_OnAction(xtpPaneActionClosed, pPane);
				}
			}

		}
		else
		{
			if (!pManager->_OnAction(xtpPaneActionClosing, pPane))
			{
				pPane->Close();
				pManager->_OnAction(xtpPaneActionClosed, pPane);
			}
		}
	}

	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_AUTOHIDE)
	{
		if (!IsHide())
		{
			if (!pManager->_OnAction(xtpPaneActionPinning, pPane))
			{
				pPane->Hide();
				pManager->_OnAction(xtpPaneActionPinned, pPane);
			}

		}
		else GetDockingPaneManager()->ToggleDocking(this);
	}

}

void CXTPDockingPaneTabbedContainer::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pSelectedPane) m_pSelectedPane->SetFocus(); else SetFocus();
	GetDockingPaneManager()->UpdatePanes();

	for (int i = 0; i < m_arrCaptionButtons.GetSize(); i++)
	{
		CXTPDockingPaneCaptionButton* pButton = m_arrCaptionButtons[i];
		if (pButton->PtInRect(point))
		{
			if (pButton->Click(this, point) && m_pSelectedPane)
			{
				OnCaptionButtonClick(pButton);
			}
			return;
		}
	}

	if (IsTabsVisible() && PerformClick(m_hWnd, point, TRUE))
		return;

	int nHit = HitTest(point);
	if (nHit == DOCKINGPANE_HITCAPTION && !IsHide())
	{
		ClientToScreen(&point);
		OnCaptionLButtonDown(point);
	}
	else if (nHit >=0)
	{
		m_lstRects.RemoveAll();
		for (int j = 0; j < GetItemCount(); j++)
		{
			CRect rc(GetItem(j)->GetRect());
			m_lstRects.Add(rc);
		}


		m_pTrackingPane = GetItemPane(nHit);
		SelectPane(m_pTrackingPane);
		m_pTrackingPane->SetFocus();

		m_pParentFrame->RecalcLayout();

		PerformMouseMove(m_hWnd, point);

		SetCapture();

		Invalidate(FALSE);
	}
	CWnd::OnLButtonDown(nFlags, point);
}

CXTPDockingPane* CXTPDockingPaneTabbedContainer::GetItemPane(int nIndex)
{
	return (CXTPDockingPane*)GetItem(nIndex)->GetData();
}

void CXTPDockingPaneTabbedContainer::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (PerformClick(m_hWnd, point, TRUE))
		return;

	UNREFERENCED_PARAMETER(nFlags);

	for (int i = 0; i < m_arrCaptionButtons.GetSize(); i++)
	{
		if (m_arrCaptionButtons.GetAt(i)->PtInRect(point))
			return;
	}

	int nHit = HitTest(point);
	if (nHit == DOCKINGPANE_HITCAPTION)
	{
		CXTPDockingPane* pSelected = GetSelected();
		if (IsHide() &&pSelected && ((pSelected->GetOptions() & xtpPaneNoHideable) != 0))
			return;

		GetDockingPaneManager()->ToggleDocking(this);
	}
	else if (nHit >=0)
	{
		CXTPDockingPane* pPane = GetItemPane(nHit);
		GetDockingPaneManager()->ToggleDocking(pPane);
	}

}


void CXTPDockingPaneTabbedContainer::_Swap(CXTPDockingPane* p1, CXTPDockingPane* p2)
{
	POSITION pos1 = m_lstPanes.Find(p1);
	POSITION pos2 = m_lstPanes.Find(p2);
	ASSERT(pos1 && pos2);

	m_lstPanes.SetAt(pos1, p2);
	m_lstPanes.SetAt(pos2, p1);
}

void CXTPDockingPaneTabbedContainer::OnMouseMove(UINT nFlags, CPoint point)
{
	PerformMouseMove(m_hWnd, point);

	if (m_pTrackingPane)
	{
		for (int i = 0; i < m_lstRects.GetSize(); i++)
		{
			if (GetItemPane(i) != m_pTrackingPane && m_lstRects[i].PtInRect(point))
			{
				_Swap(GetItemPane(i), m_pTrackingPane);

				m_pParentFrame->RecalcLayout();
				return;
			}
		}


		if (CXTPTabManager::HitTest(point) == NULL)
		{
			ASSERT(m_pSelectedPane == m_pTrackingPane );
			m_pTrackingPane = NULL;
			ReleaseCapture();

			ClientToScreen(&point);

			CXTPDockingPaneContext* pContext = GetDockingPaneManager()->GetDockingContext();
			pContext->Drag(m_pSelectedPane, point);
		}
	} else
	{
		m_arrCaptionButtons.CheckForMouseOver(point);
	}

	CWnd::OnMouseMove(nFlags, point);
}
void CXTPDockingPaneTabbedContainer::OnMouseLeave()
{
	OnMouseMove(0,CPoint(-1, -1));
}
void CXTPDockingPaneTabbedContainer::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_pTrackingPane)
	{
		m_pTrackingPane = NULL;
		if (GetCapture() == this) ReleaseCapture();
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CXTPDockingPaneTabbedContainer::OnCaptureChanged(CWnd *pWnd)
{
	m_pTrackingPane = NULL;
	CWnd::OnCaptureChanged(pWnd);
}

void CXTPDockingPaneTabbedContainer::OnParentConainerChanged(CXTPDockingPaneBase* pContainer)
{
	m_pParentFrame = pContainer->m_pParentFrame;
	m_bTitleVisible = TRUE;
	if (pContainer->GetType() == xtpPaneTypeAutoHidePanel)
	{
		if (m_hWnd) ShowWindow(SW_HIDE); //?
	}
}

void CXTPDockingPaneTabbedContainer::FindPane(XTPDockingPaneType type, CXTPDockingPaneBaseList* pList)
{
	POSITION pos = m_lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane =  m_lstPanes.GetNext(pos);
		if (pPane->GetType() == type)
		{
			pList->AddTail(pPane);
		}
	}
}

POSITION CXTPDockingPaneTabbedContainer::ContainPane(CXTPDockingPaneBase* pPane)
{
	if (pPane == this)
		return (POSITION)TRUE;

	POSITION pos = m_lstPanes.GetHeadPosition();
	while (pos)
	{
		POSITION posContain = pos;

		if (m_lstPanes.GetNext(pos)->ContainPane(pPane))
			return posContain;
	}
	return NULL;
}

BOOL CXTPDockingPaneTabbedContainer::CanAttach(CRect& rcClient, CPoint pt)
{
	CRect rcTitle(rcClient);

	if (m_bTitleVisible)
	{
		rcClient.top += CXTPDockingPaneBase::GetPaintManager()->GetCaptionHeight() + 3;
	} else
	{
		SAFE_CALLPTR(m_pParentFrame, GetWindowRect(rcTitle));
	}

	rcTitle.bottom = rcClient.top;
	if (rcTitle.PtInRect(pt))
		return TRUE;

	if (IsTabsVisible())
	{
		CRect rcTabs(rcClient);
		rcClient.bottom -= CXTPDockingPaneBase::GetPaintManager()->GetTabsHeight();
		rcTabs.top = rcClient.bottom;

		if (rcTabs.PtInRect(pt))
			return TRUE;
	}

	return FALSE;
}


BOOL CXTPDockingPaneTabbedContainer::IsEmpty()
{
	return m_lstPanes.IsEmpty();
}

void CXTPDockingPaneTabbedContainer::Copy(CXTPDockingPaneTabbedContainer* pClone, CXTPPaneToPaneMap* pMap, DWORD dwIgnoredOptions)
{
	ASSERT(pClone);

	m_bActive = FALSE;
	m_pParentFrame = GetDockingPaneManager()->GetSite();
	m_pSelectedPane = m_pTrackingPane = NULL;

	m_rcWindow = pClone->m_rcWindow;
	m_szDocking = pClone->m_szDocking;
	m_bTitleVisible = TRUE;
	m_bLockReposition = FALSE;

	CXTPDockingPane* pSelected = NULL;
	if (pMap)
	{
		POSITION pos = pClone->m_lstPanes.GetHeadPosition();
		while (pos)
		{
			CXTPDockingPane* pPane = (CXTPDockingPane*)pClone->m_lstPanes.GetNext(pos);
			CXTPDockingPane* pNewPane = (CXTPDockingPane*)pPane->Clone(m_pLayout, pMap);
			if (pClone->GetSelected() == pPane) pSelected = pNewPane;
			_InsertPane(pNewPane);
		}
	}
	else
	{

		POSITION pos = pClone->m_lstPanes.GetHeadPosition();
		while (pos)
		{
			CXTPDockingPane* pPane = (CXTPDockingPane*)pClone->m_lstPanes.GetNext(pos);

			ASSERT(pPane->GetType() == xtpPaneTypeDockingPane);

			if ((((CXTPDockingPane*)pPane)->GetOptions() & dwIgnoredOptions) == 0)
			{
				if (pClone->GetSelected() == pPane && pSelected == NULL) pSelected = pPane;

				pPane->m_pParentContainer->RemovePane(pPane);
				_InsertPane(pPane);
			}
		}
	}
	if (pSelected) SelectPane(pSelected, FALSE);


}

CXTPDockingPaneBase* CXTPDockingPaneTabbedContainer::Clone(CXTPDockingPaneLayout* pLayout, CXTPPaneToPaneMap* pMap, DWORD dwIgnoredOptions)
{
	CXTPDockingPaneTabbedContainer* pPane = (CXTPDockingPaneTabbedContainer*)
		GetDockingPaneManager()->OnCreatePane(GetType(), pLayout);

	pPane->Copy(this, pMap, dwIgnoredOptions);

	SAFE_CALLPTR(pMap, SetAt(this, pPane));

	return pPane;

}

BOOL CXTPDockingPaneTabbedContainer::IsHide()
{
	return DYNAMIC_DOWNCAST(CXTPDockingPaneAutoHideWnd, GetParentFrame()) != 0;
}

void CXTPDockingPaneTabbedContainer::Show()
{
	if (m_pSelectedPane &&m_pParentContainer &&
		m_pParentContainer->GetType() == xtpPaneTypeAutoHidePanel)
	{
		((CXTPDockingPaneAutoHidePanel*)m_pParentContainer)->ShowPane(m_pSelectedPane);
	}
	else if (DYNAMIC_DOWNCAST(CXTPDockingPaneMiniWnd, GetParentFrame()))
	{
		((CXTPDockingPaneMiniWnd*)GetParentFrame())->Expand();
	}

}


BOOL CXTPDockingPaneTabbedContainer::IsCaptionButtonVisible(CXTPDockingPaneCaptionButton* pButton)
{
	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_CLOSE)
		return m_pSelectedPane && ((m_pSelectedPane->GetOptions() & xtpPaneNoCloseable) == 0);

	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_AUTOHIDE)
		return m_pSelectedPane && ((m_pSelectedPane->GetOptions() & xtpPaneNoHideable) == 0) && (pButton->GetState() & xtpPanePinVisible);

	return TRUE;
}


INT_PTR CXTPDockingPaneTabbedContainer::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{

	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	// check child windows first by calling CControlBar
	INT_PTR nHit = CWnd::OnToolHitTest(point, pTI);
	if (nHit != -1)
		return nHit;

	for (int i = 0; i < m_arrCaptionButtons.GetSize(); i++)
	{
		CXTPDockingPaneCaptionButton* pButton = m_arrCaptionButtons[i];
		if (pButton->PtInRect(point))
		{
			nHit = (INT_PTR)pButton->GetID();
			CString strTip;
			XTPResourceManager()->LoadString(&strTip, (UINT)nHit);

			if (strTip.GetLength() == 0)
				return -1;

		#if _MSC_VER < 1200 // MFC 5.0
			if (pTI != NULL && pTI->cbSize >= sizeof(TOOLINFO))
		#else
			if (pTI != NULL && pTI->cbSize >= sizeof(AFX_OLDTOOLINFO))
		#endif
			{
				pTI->hwnd = m_hWnd;
				pTI->rect = pButton->GetRect();
				pTI->uId = nHit;
				pTI->lpszText = (LPTSTR) ::calloc(strTip.GetLength() + 1, sizeof(TCHAR));
				if( pTI->lpszText != NULL ) STRCPY_S(pTI->lpszText, strTip.GetLength() + 1, (LPCTSTR)strTip);
			}
			return nHit;
		}
	}

	return PerformToolHitTest(m_hWnd, CXTPTabManager::HitTest(point), pTI);
}

void CXTPDockingPaneTabbedContainer::AdjustMinMaxInfoClientRect(LPMINMAXINFO pMinMaxInfo, BOOL bCaptionOnly)
{
	CRect rc(0, 0, 1000, 1000);
	if (bCaptionOnly)
	{
		CXTPDockingPaneBase::GetPaintManager()->AdjustCaptionRect(this, rc);
	}
	else
	{
		CXTPDockingPaneBase::GetPaintManager()->AdjustClientRect(this, rc, FALSE);
	}

	pMinMaxInfo->ptMinTrackSize.x += 1000 - rc.Width();
	pMinMaxInfo->ptMinTrackSize.y += 1000 - rc.Height();

	pMinMaxInfo->ptMaxTrackSize.x += 1000 - rc.Width();
	pMinMaxInfo->ptMaxTrackSize.y += 1000 - rc.Height();

}

void CXTPDockingPaneTabbedContainer::GetMinMaxInfo(LPMINMAXINFO pMinMaxInfo)
{
	CXTPDockingPaneBase::GetMinMaxInfo(pMinMaxInfo);

	if (IsEmpty())
		return;

	POSITION pos = m_lstPanes.GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane = m_lstPanes.GetNext(pos);

		MINMAXINFO info;
		pPane->GetMinMaxInfo(&info);

		pMinMaxInfo->ptMinTrackSize.x = max(pMinMaxInfo->ptMinTrackSize.x, info.ptMinTrackSize.x);
		pMinMaxInfo->ptMinTrackSize.y = max(pMinMaxInfo->ptMinTrackSize.y, info.ptMinTrackSize.y);

		pMinMaxInfo->ptMaxTrackSize.x = min(pMinMaxInfo->ptMaxTrackSize.x, info.ptMaxTrackSize.x);
		pMinMaxInfo->ptMaxTrackSize.y = min(pMinMaxInfo->ptMaxTrackSize.y, info.ptMaxTrackSize.y);
	}
	AdjustMinMaxInfoClientRect(pMinMaxInfo);
}

