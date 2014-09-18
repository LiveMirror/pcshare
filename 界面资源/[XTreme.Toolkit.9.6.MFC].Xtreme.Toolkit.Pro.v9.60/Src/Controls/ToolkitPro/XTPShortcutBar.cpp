// XTPShortcutBarItem.cpp : implementation of the CXTPShortcutBar class.
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
#include "XTPDrawHelpers.h"
#include "XTResource.h"
#include "XTPCommandBarsResource.h"
#include "XTPImageManager.h"

#include "XTPToolBar.h"
#include "XTPCommandBars.h"
#include "XTPPopupBar.h"
#include "XTPControls.h"
#include "XTPControlButton.h"
#include "XTPControlPopup.h"
#include "XTPMouseManager.h"

#include "XTPShortcutBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define XTP_ID_SHORTCUT_SELECTED 8000
#define XTP_ID_SHORTCUT_CUSTOMIZE 8001


CXTPShortcutBarItem::CXTPShortcutBarItem(CXTPShortcutBar* pShortcutBar, UINT nID, CWnd* pWnd)
{

	CString strCaption;
	if (strCaption.LoadString(nID))
	{
		m_strCaption = m_strTooltip = strCaption;

		int nShortCutPos = strCaption.Find ('\n');
		if ( nShortCutPos != -1 )
		{
			m_strTooltip = strCaption.Mid(nShortCutPos + 1);
			m_strCaption = strCaption.Left (nShortCutPos);
		}
	}


	m_pShortcutBar = pShortcutBar;
	m_bVisible = TRUE;

	m_rcItem.SetRectEmpty();
	m_hwndChild = pWnd->GetSafeHwnd();
	m_nID = nID;
	m_bExpandButton = FALSE;
	m_dwData = 0;

	EnableAutomation();


}
CXTPShortcutBarItem::CXTPShortcutBarItem(CXTPShortcutBar* pShortcutBar)
{
	m_pShortcutBar = pShortcutBar;
	m_bExpandButton = TRUE;
	m_nID = -1;
	XTPResourceManager()->LoadString(&m_strCaption, XTP_IDS_SHORTCUT_CONFIGURE);
	m_strTooltip = m_strCaption;
	m_hwndChild = NULL;
	m_bVisible = TRUE;

	m_dwData = 0;

	EnableAutomation();

}

CXTPImageManagerIcon* CXTPShortcutBarItem::GetImage(int nWidth)
{
	return m_pShortcutBar->GetImageManager()->GetImage(GetID(), nWidth);
}

void CXTPShortcutBarItem::SetCaption(LPCTSTR strCaption)
{
	m_strCaption = strCaption;
	if (m_pShortcutBar->GetSafeHwnd()) m_pShortcutBar->Invalidate(FALSE);
}

void CXTPShortcutBarItem::SetTooltip(LPCTSTR strTooltip)
{
	m_strTooltip = strTooltip;
	m_pShortcutBar->Reposition();
}

void CXTPShortcutBarItem::SetID(int nID)
{
	m_nID = nID;
}

int CXTPShortcutBarItem::GetID()
{
	return m_nID;
}
void CXTPShortcutBarItem::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;
	m_pShortcutBar->Reposition();
}
BOOL CXTPShortcutBarItem::IsVisible()
{
	return m_bVisible;
}
BOOL CXTPShortcutBarItem::IsHidden()
{
	return m_bHidden;
}
/////////////////////////////////////////////////////////////////////////////
// CXTPShortcutBar

CXTPShortcutBar::CXTPShortcutBar()
{
	m_pPaintManager = new CXTPShortcutBarOffice2003Theme;
	m_paintTheme = xtpShortcutThemeOffice2003;

	m_nExpandedLines = m_nDesiredExpandedLines = 1;

	m_szItem = CSize(22, 32);
	m_rcClient.SetRectEmpty();

	// Add expand button
	m_arrItems.Add(new CXTPShortcutBarItem(this));

	m_hSizeCursor = AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(IDC_SIZENS));

	m_bTrackingSize = FALSE;
	m_pPressedItem = m_pSelectedItem = m_pHotItem = NULL;

	m_hwndClient = NULL;

	m_nMinClientHeight = 200;

	m_bClientPaneVisible = TRUE;
	m_pImageManager = NULL;

	m_hHandCursor = ::LoadCursor(0, MAKEINTRESOURCE(32649));

	if (m_hHandCursor == 0)
		m_hHandCursor = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(XT_IDC_HAND));

}

CXTPShortcutBar::~CXTPShortcutBar()
{
	delete m_pPaintManager;

	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		m_arrItems[i]->InternalRelease();
	}

	if (m_pImageManager)
	{
		m_pImageManager->InternalRelease();
	}
}

CXTPImageManager* CXTPShortcutBar::GetImageManager()
{
	return m_pImageManager? m_pImageManager: XTPImageManager();
}

void CXTPShortcutBar::SetImageManager(CXTPImageManager* pImageManager)
{
	if (m_pImageManager)
		m_pImageManager->InternalRelease();

	m_pImageManager = pImageManager;

	Reposition();
}

BOOL CXTPShortcutBar::Create(DWORD dwStyle, const RECT&  rect, CWnd* pParentWnd, UINT nID)
{
	if (!CreateEx(WS_EX_STATICEDGE,::AfxRegisterWndClass(0, ::LoadCursor(0, IDC_ARROW)),
		NULL, dwStyle, rect, pParentWnd, nID))
	{
		return FALSE;
	}

	return TRUE;
}

void CXTPShortcutBar::SetTheme(XTPShortcutBarPaintTheme paintTheme)
{
	delete m_pPaintManager;

	if (paintTheme == xtpShortcutThemeOfficeXP) m_pPaintManager = new CXTPShortcutBarOfficeXPTheme();
	else if(paintTheme == xtpShortcutThemeOffice2003)   m_pPaintManager = new CXTPShortcutBarOffice2003Theme();
	else m_pPaintManager = new CXTPShortcutBarOffice2000Theme();

	m_paintTheme = paintTheme;

	if (m_hWnd)
	{
		Reposition();
		RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN|RDW_FRAME);
	}
}

void CXTPShortcutBar::SetCustomTheme(CXTPShortcutBarPaintManager* pPaintManager)
{
	delete m_pPaintManager;
	m_pPaintManager = pPaintManager;

	Reposition();
	RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
}


void CXTPShortcutBar::SetExpandedLinesCount(int nLinesCount)
{
	m_nDesiredExpandedLines = nLinesCount;
	Reposition();
}

CXTPShortcutBarItem* CXTPShortcutBar::AddItem(UINT nID, CWnd* pWnd)
{
	CXTPShortcutBarItem* pItem = new CXTPShortcutBarItem(this, nID, pWnd);
	m_arrItems.InsertAt(m_arrItems.GetSize() - 1, pItem);

	Reposition();

	return pItem;
}



int CXTPShortcutBar::GetVisibleItemsCount()
{
	int nCount = 0;
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		if (m_arrItems[i]->m_bVisible) nCount++;

	}
	return nCount;
}

void CXTPShortcutBar::Reposition()
{
	if (!m_hWnd)
		return;

	static BOOL bReposition = FALSE;

	if (bReposition)
		return;

	bReposition = TRUE;


	CXTPClientRect rcShortcutBar(this);
	int nWidth = rcShortcutBar.Width();
	int nHeight = rcShortcutBar.Height();
	int nGripperHeight = m_pPaintManager->DrawGripper(NULL, this, FALSE);

	for (int nItem =  0; nItem < m_arrItems.GetSize(); nItem++)
		m_arrItems[nItem]->m_bHidden = FALSE;

	int nVisibleItemsCount = GetVisibleItemsCount();

	int nClientHeight = nHeight;


	if (!m_bClientPaneVisible)
	{
		m_nExpandedLines = nVisibleItemsCount;
		nGripperHeight = 0;

		m_rcGripper.SetRectEmpty();
		m_rcClient.SetRectEmpty();

		nHeight = min(nHeight, m_nExpandedLines * m_szItem.cy);
		if (nHeight - m_nExpandedLines * m_szItem.cy < 0)
		{
			m_nExpandedLines = nHeight/ m_szItem.cy;
			nHeight = m_nExpandedLines * m_szItem.cy;
		}
		nHeight--;
	}
	else
	{
		m_nExpandedLines = m_nDesiredExpandedLines;

		if (nHeight - m_nExpandedLines * m_szItem.cy - nGripperHeight < m_nMinClientHeight)
		{
			m_nExpandedLines = (nHeight - nGripperHeight - m_nMinClientHeight) / m_szItem.cy;

		}

		if (m_nExpandedLines > nVisibleItemsCount)
			m_nExpandedLines = nVisibleItemsCount;
		if (m_nExpandedLines < 1)
			m_nExpandedLines = 1;

		m_rcGripper.SetRect(0, nHeight - m_nExpandedLines * m_szItem.cy - nGripperHeight, nWidth, nHeight - m_nExpandedLines * m_szItem.cy);
		m_rcClient.SetRect(0, 0, nWidth, m_rcGripper.top);
	}





	int nExpandedLines = m_nExpandedLines;

	int nExpandedItem;
	for (nExpandedItem = 0; nExpandedItem < m_arrItems.GetSize(); nExpandedItem++)
	{

		CXTPShortcutBarItem* pItem = m_arrItems[nExpandedItem];
		if (!pItem->m_bVisible)
			continue;

		if (nExpandedLines < 2) break;

		pItem->m_bExpanded = TRUE;
		pItem->m_rcItem.SetRect(0, nHeight - nExpandedLines * m_szItem.cy, nWidth, nHeight - (nExpandedLines - 1) * m_szItem.cy);

		nExpandedLines--;
		nVisibleItemsCount--;
	}

	int nCollapsedCount = min(nVisibleItemsCount, nWidth / m_szItem.cx) - 1;
	int nCollapsedPos = nWidth - m_szItem.cx * (nCollapsedCount + 1);

	for (int nCollapsedItem = nExpandedItem; nCollapsedItem < m_arrItems.GetSize(); nCollapsedItem++)
	{
		CXTPShortcutBarItem* pItem = m_arrItems[nCollapsedItem];
		if (!pItem->m_bVisible)
			continue;
		pItem->m_bExpanded = FALSE;

		if (pItem->IsItemExpandButton())
			pItem->m_rcItem.SetRect(nWidth - m_szItem.cx, nClientHeight - m_szItem.cy, nWidth, nClientHeight);
		else if (nCollapsedCount > 0)
			pItem->m_rcItem.SetRect(nCollapsedPos, nClientHeight - m_szItem.cy, nCollapsedPos + m_szItem.cx, nClientHeight);
		else
			pItem->m_bHidden = TRUE;

		nCollapsedPos += m_szItem.cx;
		nCollapsedCount--;
	}


	if (m_hwndClient)
	{
		::MoveWindow(m_hwndClient, m_rcClient.left, m_rcClient.top, m_rcClient.Width(), m_rcClient.Height(), TRUE);
	}

	Invalidate(FALSE);

	bReposition = FALSE;

	OnRepositionDone();
}

void CXTPShortcutBar::OnRepositionDone()
{
	m_wndToolTip.DestroyWindow();
	m_wndToolTip.Create(this);

	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		CXTPShortcutBarItem* pItem = m_arrItems[i];
		if (pItem->m_bVisible && !pItem->m_bHidden && !pItem->IsExpanded())
		{
			m_wndToolTip.AddTool(this, pItem->GetTooltip(), pItem->GetItemRect(), i + 1);
		}
	}

	m_wndToolTip.Activate(TRUE);
}


CXTPShortcutBarItem* CXTPShortcutBar::HitTest(CPoint pt)
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		CXTPShortcutBarItem* pItem = m_arrItems[i];
		if (pItem->m_bVisible && !pItem->m_bHidden && pItem->m_rcItem.PtInRect(pt))
			return m_arrItems[i];
	}
	return NULL;
}

void CXTPShortcutBar::SetHotItem(CXTPShortcutBarItem* pItem)
{
	if (m_pHotItem == pItem)
		return;

	m_pHotItem = pItem;
	Invalidate(FALSE);

	if (m_pHotItem)
	{
		TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, 0};
		_TrackMouseEvent(&tme);
	}

}

void CXTPShortcutBar::SelectItem(CXTPShortcutBarItem* pItem)
{
	if (m_pSelectedItem == pItem)
		return;

	if (pItem->IsItemExpandButton())
	{
		OnExpandButtonDown(pItem);
		return;
	}

	if (GetOwner()->SendMessage(XTPWM_SHORTCUTBAR_NOTIFY, XTP_SBN_SELECTION_CHANGING, (LPARAM)pItem) == -1)
		return;


	if (pItem->m_hwndChild)
	{
		if (m_hwndClient)
		{
			::ShowWindow(m_hwndClient, SW_HIDE);
		}

		m_hwndClient = pItem->m_hwndChild;
		::ShowWindow(m_hwndClient, SW_SHOW);
	}

	m_pSelectedItem = pItem;


	Reposition();
}

void CXTPShortcutBar::OnExpandButtonDown(CXTPShortcutBarItem* pExpandButton)
{


	CXTPPopupBar* pPopup = CXTPPopupBar::CreatePopupBar(NULL);

	if (m_bClientPaneVisible)
	{
		pPopup->GetControls()->Add(xtpControlButton, XTP_ID_SHORTCUT_SHOW_MORE);
		pPopup->GetControls()->Add(xtpControlButton, XTP_ID_SHORTCUT_SHOW_FEWER);
	}

	CXTPControlPopup* pArrOrRemove = (CXTPControlPopup*)pPopup->GetControls()->Add(xtpControlPopup, 0);

	CString strCaption;
	XTPResourceManager()->LoadString(&strCaption, XTP_IDS_ADDORREMOVE);
	pArrOrRemove->SetCaption(strCaption);

	for (int j = 0; j < m_arrItems.GetSize() - 1; j++)
	{
		CXTPShortcutBarItem* pItem = m_arrItems[j];

		CXTPControl* pControl = pArrOrRemove->GetCommandBar()->GetControls()->Add(xtpControlButton, XTP_ID_SHORTCUT_CUSTOMIZE);
		pControl->SetIconId(pItem->GetID());
		pControl->SetCaption(pItem->GetCaption());
		pControl->SetTag((DWORD_PTR)pItem);
		pControl->SetChecked(pItem->m_bVisible);
		pControl->SetFlags(xtpFlagManualUpdate);
	}



	BOOL bBeginGroup = TRUE;
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		CXTPShortcutBarItem* pItem = m_arrItems[i];
		if (pItem->m_bVisible && pItem->m_bHidden)
		{
			CXTPControl* pControl = pPopup->GetControls()->Add(xtpControlButton, XTP_ID_SHORTCUT_SELECTED);
			pControl->SetIconId(pItem->GetID());
			pControl->SetCaption(pItem->GetCaption());
			pControl->SetTag((DWORD_PTR)pItem);

			pControl->SetFlags(xtpFlagManualUpdate);

			pControl->SetBeginGroup(bBeginGroup);

			bBeginGroup = FALSE;
		}
	}

	CPoint pt(pExpandButton->GetItemRect().right, pExpandButton->GetItemRect().CenterPoint().y);
	ClientToScreen(&pt);

	CXTPCommandBars::TrackPopupMenu(pPopup, 0, pt.x, pt.y, this);

	pPopup->InternalRelease();

	SetHotItem(NULL);

}


BEGIN_MESSAGE_MAP(CXTPShortcutBar, CWnd)
	//{{AFX_MSG_MAP(CXTPShortcutBar)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_NCPAINT()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_SYSCOLORCHANGE()
	ON_COMMAND(XTP_ID_SHORTCUT_SHOW_MORE, OnShowMoreButtons)
	ON_COMMAND(XTP_ID_SHORTCUT_SHOW_FEWER, OnShowFewerButtons)
	ON_UPDATE_COMMAND_UI(XTP_ID_SHORTCUT_SHOW_MORE, OnUpdateShowMoreButtons)
	ON_UPDATE_COMMAND_UI(XTP_ID_SHORTCUT_SHOW_FEWER, OnUpdateShowFewerButtons)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_XTP_COMMAND, OnExpandPopupExecute)

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPShortcutBar message handlers

void CXTPShortcutBar::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting
	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint, rc);

	m_pPaintManager->FillShortcutBar(&dc, this);

	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		CXTPShortcutBarItem* pItem = m_arrItems[i];
		if (!pItem->m_bVisible || pItem->m_bHidden)
			continue;
		m_pPaintManager->DrawShortcutItem(&dc, pItem);
	}
	if (m_bClientPaneVisible)
		m_pPaintManager->DrawGripper(&dc, this, TRUE);
}

BOOL CXTPShortcutBar::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPShortcutBar::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	Reposition();
}


int CXTPShortcutBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	Reposition();

	return 0;
}

void CXTPShortcutBar::OnNcPaint()
{
	CWindowDC dc(this); // device context for painting
	m_pPaintManager->DrawShortcutBarFrame(&dc, this);
}

BOOL CXTPShortcutBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	if (m_bTrackingSize || m_rcGripper.PtInRect(pt))
	{
		::SetCursor(m_hSizeCursor);
		return TRUE;
	}

	if (HitTest(pt) != NULL && m_hHandCursor)
	{
		::SetCursor(m_hHandCursor);
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}



void CXTPShortcutBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if (XTPMouseManager()->IsTrackedLock(0))
		return;

	if (m_bTrackingSize)
	{
		CXTPClientRect rc(this);
		m_nDesiredExpandedLines = (rc.Height() - point.y) / m_szItem.cy;
		Reposition();
	}
	else
	{
		CXTPShortcutBarItem* pItem = HitTest(point);
		if (pItem != m_pHotItem)
		{
			SetHotItem(pItem);
		}

	}

	CWnd::OnMouseMove(nFlags, point);
}


void CXTPShortcutBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_rcGripper.PtInRect(point))
	{
		m_bTrackingSize = TRUE;
		SetCapture();
		return;
	}

	CXTPShortcutBarItem* pItem = HitTest(point);

	if (pItem)
	{
		m_pPressedItem = pItem;
		Invalidate(FALSE);
		SetCapture();
		return;
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CXTPShortcutBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bTrackingSize = FALSE;

	CXTPShortcutBarItem* pPressedItem = m_pPressedItem;
	m_pPressedItem = NULL;
	ReleaseCapture();

	if (pPressedItem)
	{
		if (m_pHotItem == pPressedItem)
		{
			SelectItem(pPressedItem);
		}
		Invalidate(FALSE);
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CXTPShortcutBar::OnCaptureChanged(CWnd *pWnd)
{
	m_bTrackingSize = FALSE;

	if (m_pPressedItem)
	{
		m_pPressedItem = NULL;
		Invalidate(FALSE);
	}

	CWnd::OnCaptureChanged(pWnd);
}

void CXTPShortcutBar::OnRButtonDown(UINT nFlags, CPoint point)
{
	OnMouseMove(nFlags, point);

	GetOwner()->SendMessage(XTPWM_SHORTCUTBAR_NOTIFY, XTP_SBN_RCLICK, MAKELONG(point.x, point.y));

	GetCursorPos(&point);
	ScreenToClient(&point);
	OnMouseMove(nFlags, point);

	CWnd::OnRButtonDown(nFlags, point);
}

void CXTPShortcutBar::OnRButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonUp(nFlags, point);

}


void CXTPShortcutBar::OnMouseLeave()
{
	if (!XTPMouseManager()->IsTrackedLock(0))
		SetHotItem(NULL);
}


void CXTPShortcutBar::OnSysColorChange()
{
	CWnd::OnSysColorChange();

	m_pPaintManager->RefreshMetrics();
	Reposition();

}

CXTPShortcutBarItem* CXTPShortcutBar::FindItem(int nID)
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		if (m_arrItems[i]->GetID() == nID)
			return m_arrItems[i];
	}
	return NULL;
}

void CXTPShortcutBar::LoadState(LPCTSTR lpszProfileName)
{
	m_nDesiredExpandedLines = AfxGetApp()->GetProfileInt(lpszProfileName, _T("DesiredExpandedLines"), m_nDesiredExpandedLines);


	CDWordArray arrHiddenItems;

	LPBYTE lpbData;
	UINT dwDataSize;
	if (AfxGetApp()->GetProfileBinary(lpszProfileName, _T("Hidden"), &lpbData, &dwDataSize))
	{
		try
		{
			CMemFile file(lpbData, dwDataSize);
			CArchive ar (&file, CArchive::load);
			arrHiddenItems.Serialize(ar);

			for (int i = 0; i < arrHiddenItems.GetSize(); i++)
			{
				CXTPShortcutBarItem* pItem = FindItem(arrHiddenItems[i]);
				if (pItem && !pItem->IsItemExpandButton()) pItem->m_bVisible = FALSE;
			}

		}
		catch (CMemoryException* pEx)
		{
			pEx->Delete ();
		}
		catch (CArchiveException* pEx)
		{
			pEx->Delete ();
		}

		delete[] lpbData;
	}

	Reposition();
}

void CXTPShortcutBar::SaveState(LPCTSTR lpszProfileName)
{
	AfxGetApp()->WriteProfileInt(lpszProfileName, _T("DesiredExpandedLines"), m_nDesiredExpandedLines);

	CDWordArray arrHiddenItems;

	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		if (!m_arrItems[i]->IsVisible() && !m_arrItems[i]->IsItemExpandButton())
			arrHiddenItems.Add(m_arrItems[i]->GetID());
	}

	try
	{
		CMemFile file;
		CArchive ar (&file, CArchive::store);
		arrHiddenItems.Serialize(ar);
		ar.Flush ();

		DWORD dwDataSize = (DWORD)file.GetPosition();
		LPBYTE lpbData = file.Detach();

		AfxGetApp()->WriteProfileBinary(lpszProfileName, _T("Hidden"), lpbData, dwDataSize);

		ar.Close();
		file.Close();
		free (lpbData);
	}
	catch (CMemoryException* pEx)
	{
		pEx->Delete ();
	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
	}

}
int CXTPShortcutBar::GetItemCount()
{
	return (int)m_arrItems.GetSize();
}

CXTPShortcutBarItem* CXTPShortcutBar::GetItem(int nIndex)
{
	if (nIndex >= 0 && nIndex < m_arrItems.GetSize())
		return m_arrItems[nIndex];
	return NULL;
}

void CXTPShortcutBar::RemoveItem(int nID)
{
	m_pHotItem = NULL; m_pPressedItem = NULL;

	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		CXTPShortcutBarItem* pItem = m_arrItems[i];
		if (pItem->GetID() == nID)
		{
			if (pItem == m_pSelectedItem) m_pSelectedItem = NULL;

			m_arrItems.RemoveAt(i);
			pItem->InternalRelease();
			Reposition();
			return;
		}
	}
}
void CXTPShortcutBar::RemoveAllItems()
{
	m_pHotItem = NULL; m_pPressedItem = NULL; m_pSelectedItem = NULL;

	for (int i = (int)m_arrItems.GetSize() - 1; i >= 0 ; i--)
	{
		CXTPShortcutBarItem* pItem = m_arrItems[i];

		if (!pItem->IsItemExpandButton())
		{
			m_arrItems.RemoveAt(i);
			pItem->InternalRelease();
		}
	}

	Reposition();
}


LRESULT CXTPShortcutBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_wndToolTip && m_wndToolTip.GetSafeHwnd())
	{
		if (!XTPMouseManager()->IsTrackedLock(0))
		if ((message == WM_MOUSEMOVE || message == WM_NCMOUSEMOVE || message == WM_LBUTTONDOWN ||
			message == WM_LBUTTONUP || message == WM_RBUTTONUP))
		{
			MSG msg;
			msg.message = message;
			msg.lParam = lParam;
			msg.wParam = wParam;
			msg.hwnd = m_hWnd;

			m_wndToolTip.RelayEvent(&msg);
		}
	}

	return CWnd::WindowProc(message, wParam, lParam);
}


void CXTPShortcutBar::OnShowMoreButtons()
{
	m_nDesiredExpandedLines = m_nExpandedLines + 1;
	Reposition();
}
void CXTPShortcutBar::OnShowFewerButtons()
{
	m_nDesiredExpandedLines = m_nExpandedLines - 1;
	Reposition();
}

void CXTPShortcutBar::OnUpdateShowMoreButtons(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetVisibleItemsCount() != m_nExpandedLines);

}
void CXTPShortcutBar::OnUpdateShowFewerButtons(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nExpandedLines > 1);
}

LRESULT CXTPShortcutBar::OnExpandPopupExecute(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_ID_SHORTCUT_SELECTED)
	{
		CXTPControl* pControl = ((NMXTPCONTROL*)lParam)->pControl;
		SelectItem((CXTPShortcutBarItem*)pControl->GetTag());
		return 1;
	}
	if (wParam == XTP_ID_SHORTCUT_CUSTOMIZE)
	{
		CXTPControl* pControl = ((NMXTPCONTROL*)lParam)->pControl;
		CXTPShortcutBarItem* pItem = (CXTPShortcutBarItem*)pControl->GetTag();

		pItem->m_bVisible = !pItem->m_bVisible;
		Reposition();


		return 1;
	}
	return 0;
}






