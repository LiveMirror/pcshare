// XTPControlExt.cpp : implementation file.
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
#include "XTPIntel80Helpers.h"
#include "XTPColorManager.h"
#include "XTPPropExchange.h"

#include "XTPControlExt.h"
#include "XTPMouseManager.h"
#include "XTPControls.h"
#include "XTPToolBar.h"
#include "XTPCommandBarsResource.h"
#include "XTPPopupBar.h"
#include "XTPControlPopup.h"
#include "XTPCommandBars.h"
#include "XTPTabManager.h"
#include "XTPTabClientWnd.h"
#include "XTPPaintManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT CXTPControlWindowList::m_nFistMDIChild = AFX_IDM_FIRST_MDICHILD;
UINT CXTPControlWindowList::m_nItemCount = 9;

//////////////////////////////////////////////////////////////////////////
// CXTPControlWindowList

IMPLEMENT_XTP_CONTROL( CXTPControlWindowList, CXTPControlButton)

void CXTPControlWindowList::OnCalcDynamicSize(DWORD /*dwMode*/)
{
	ASSERT(m_pControls->GetAt(m_nIndex) == this);

	while (m_nIndex + 1 < m_pControls->GetCount())
	{
		CXTPControl* pControl = m_pControls->GetAt(m_nIndex + 1);
		if (pControl->GetID() >= (int)m_nFistMDIChild)
		{
			m_pControls->Remove(pControl);
		}
		else break;
	}

	CMDIFrameWnd* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, m_pParent->GetSite());
	HWND hWndClient = pFrame? pFrame->m_hWndMDIClient: NULL;

	if (m_pParent->IsCustomizeMode() || hWndClient == NULL)
	{
		m_dwHideFlags = 0;
		return;
	}

	m_dwHideFlags |= xtpHideGeneric;

	int nIndex = m_nIndex + 1;

	TCHAR lpTitle[256];
	HWND hWndActive = (HWND)::SendMessage(hWndClient, WM_MDIGETACTIVE, 0, 0);

	if (hWndActive)
	{
		int nID = GetDlgCtrlID(hWndActive);

		if (nID < 0x800F && nID >= 0x8000)
			m_nFistMDIChild = 0x8000;

		if (nID < 0xF && nID >= 1)
			m_nFistMDIChild = 1;
	}

	int nID = m_nFistMDIChild;

	HWND hWndChild = ::GetDlgItem(hWndClient, nID);
	while (hWndChild)
	{
		if (::IsWindowVisible(hWndChild))
		{
			::GetWindowText(hWndChild, lpTitle, 256);

			CXTPControl* pControl = m_pControls->Add(xtpControlButton, nID, _T(""), nIndex, TRUE);

			CString strTitle;
			strTitle.Format(_T("&%i %s"), nIndex - m_nIndex, lpTitle);

			pControl->SetCaption(strTitle);
			pControl->SetBeginGroup(nIndex == m_nIndex + 1);
			pControl->SetChecked(hWndActive == hWndChild);
			pControl->SetFlags(xtpFlagManualUpdate);

			nIndex++;

			if (nIndex - m_nIndex > (int)m_nItemCount)
				break;
		}

		hWndChild = ::GetDlgItem(hWndClient, ++nID);
	}
}

BOOL CXTPControlWindowList::IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint /*point*/, DROPEFFECT& dropEffect)
{
	if (pCommandBar->GetType() != xtpBarTypePopup)
	{
		dropEffect = DROPEFFECT_NONE;
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// CXTPControlToolbars

void CXTPControlToolbars::CXTPControlToolbar::OnExecute()
{
	CXTPCommandBars* pCommandBars = GetParent()->GetCommandBars();
	XTPMouseManager()->SendTrackLost();

	if (pCommandBars) pCommandBars->ToggleVisible((int)GetTag());
}

IMPLEMENT_XTP_CONTROL( CXTPControlToolbars, CXTPControlButton)

void CXTPControlToolbars::OnCalcDynamicSize(DWORD /*dwMode*/)
{
	while (m_nIndex + 1 < m_pControls->GetCount())
	{
		CXTPControl* pControl = m_pControls->GetAt(m_nIndex + 1);
		if (pControl->GetParameter() == _T("CXTPControlToolbar"))
		{
			m_pControls->Remove(pControl);
		}
		else break;
	}

	if (m_pParent->IsCustomizeMode())
	{
		m_dwHideFlags = 0;
		return;
	}

	m_dwHideFlags |= xtpHideGeneric;


	CXTPCommandBars* pCommandBars = GetParent()->GetCommandBars();
	if (!pCommandBars)
		return;

	CXTPToolBar* pBar = NULL;
	CList <CXTPToolBar*, CXTPToolBar*>& lstBars = pCommandBars->m_lstBars;
	int nIndex = 0;
	int nInsert = m_nIndex + 1;

	POSITION pos = lstBars.GetHeadPosition();
	while (pos)
	{
		pBar = lstBars.GetNext(pos);

		if (pBar->GetType() == xtpBarTypeNormal && pBar->IsContextMenuPresent() && pBar->IsCloseable())
		{
			CXTPControl* pButton =  m_pControls->Add(new CXTPControlToolbar(), GetID(), _T("CXTPControlToolbar"), nInsert, TRUE);
			pButton->SetTag(nIndex);

			pButton->SetCaption(pBar->GetTitle());
			pButton->SetFlags(xtpFlagManualUpdate);
			pButton->SetChecked(pBar->IsVisible());
			nInsert++;
		}
		nIndex++;
	}
}

BOOL CXTPControlToolbars::IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint /*point*/, DROPEFFECT& dropEffect)
{
	if (pCommandBar->GetType() != xtpBarTypePopup)
	{
		dropEffect = DROPEFFECT_NONE;
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// CXTPControlOleItems

IMPLEMENT_XTP_CONTROL( CXTPControlOleItems, CXTPControlButton)


void CXTPControlOleItems::OnCalcDynamicSize(DWORD /*dwMode*/)
{

	ASSERT(m_pControls->GetAt(m_nIndex) == this);

	if (m_nIndex + 1 < m_pControls->GetCount())
	{
		CXTPControl* pControl = m_pControls->GetAt(m_nIndex + 1);
		if (pControl->GetParameter() == _T("OLEMENUPOPUP"))
		{
			m_pControls->Remove(pControl);
		}
	}

	if (m_pParent->IsCustomizeMode())
	{
		m_dwHideFlags = 0;
		return;
	}

	m_dwHideFlags |= xtpHideGeneric;


	CFrameWnd* pActiveFrame = m_pParent->GetFrameSite();
	if (!pActiveFrame)
		return;

	CFrameWnd* pFrame = pActiveFrame->GetActiveFrame();

	if (pFrame == NULL || pFrame->GetActiveView() == NULL)
		return;

	CView* pView = pFrame->GetActiveView();

	CDocument* pDocument = pView->GetDocument();
	if (pDocument == NULL || DYNAMIC_DOWNCAST(COleDocument, pDocument) == NULL)
		return;

		// check for single selection
	COleClientItem* pItem = ((COleDocument*)pDocument)->GetPrimarySelectedItem(pView);
	if (pItem == NULL || pItem->GetType() == OT_STATIC)
	{
		// no selection, or is 'static' item
		return;
	}
	int nConvertID = 0;


	CMenu mnuOlePopup;
	mnuOlePopup.CreatePopupMenu();
	// update the menu
	AfxOleSetEditMenu(pItem, &mnuOlePopup, 0,
		ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, nConvertID);

	if (mnuOlePopup.GetMenuItemCount() != 1)
		return;

	CMenu* pMenu = mnuOlePopup.GetSubMenu(0);
	if (pMenu == NULL)
		return;

	CXTPPopupBar* pBar = CXTPPopupBar::CreatePopupBar(m_pParent->GetCommandBars());
	pBar->LoadMenu(pMenu);

	CXTPControlPopup* pPopup =  (CXTPControlPopup*)m_pControls->Add(xtpControlPopup, 0, _T("OLEMENUPOPUP"), m_nIndex + 1, TRUE);

	pPopup->SetCommandBar(pBar);
	pBar->InternalRelease();

	CString strTitle;
	mnuOlePopup.GetMenuString(0, strTitle, MF_BYPOSITION);
	pPopup->SetCaption(strTitle);
}

BOOL CXTPControlOleItems::IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint /*point*/, DROPEFFECT& dropEffect)
{
	if (pCommandBar->GetType() != xtpBarTypePopup)
	{
		dropEffect = DROPEFFECT_NONE;
		return FALSE;
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// CXTPControlRecentFileList

IMPLEMENT_XTP_CONTROL( CXTPControlRecentFileList, CXTPControlButton)


void CXTPControlRecentFileList::OnCalcDynamicSize(DWORD /*dwMode*/)
{
	USES_PROTECTED_ACCESS(CXTPControlRecentFileList, CWinApp, CRecentFileList*, m_pRecentFileList)

	CRecentFileList* pRecentFileList = PROTECTED_ACCESS(CWinApp, AfxGetApp(), m_pRecentFileList);

	if (!pRecentFileList)
		return;

	ASSERT(pRecentFileList->m_arrNames != NULL);

	while (m_nIndex + 1 < m_pControls->GetCount())
	{
		CXTPControl* pControl = m_pControls->GetAt(m_nIndex + 1);
		if (pControl->GetID() >= ID_FILE_MRU_FILE1 && pControl->GetID() <= ID_FILE_MRU_FILE16)
		{
			m_pControls->Remove(pControl);
		}
		else break;
	}

	if (m_pParent->IsCustomizeMode())
	{
		m_dwHideFlags = 0;
		SetEnabled(TRUE);
		return;
	}

	if (pRecentFileList->m_arrNames[0].IsEmpty())
	{
		m_dwHideFlags = 0;
		SetEnabled(FALSE);

		return;
	}
	m_dwHideFlags |= xtpHideGeneric;

	TCHAR szCurDir[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, szCurDir);
	int nCurDir = lstrlen(szCurDir);
	ASSERT(nCurDir >= 0);
	szCurDir[nCurDir] = '\\';
	szCurDir[++nCurDir] = '\0';

	CString strName;
	CString strTemp;
	for (int iMRU = 0; iMRU < pRecentFileList->m_nSize; iMRU++)
	{
		if (!pRecentFileList->GetDisplayName(strName, iMRU, szCurDir, nCurDir))
			break;

		// double up any '&' characters so they are not underlined
		LPCTSTR lpszSrc = strName;
		LPTSTR lpszDest = strTemp.GetBuffer(strName.GetLength()*2);
		while (*lpszSrc != 0)
		{
			if (*lpszSrc == '&')
				*lpszDest++ = '&';
			if (_istlead(*lpszSrc))
				*lpszDest++ = *lpszSrc++;
			*lpszDest++ = *lpszSrc++;
		}
		*lpszDest = 0;
		strTemp.ReleaseBuffer();

		// insert mnemonic + the file name
		TCHAR buf[10];
		wsprintf(buf, _T("&%d "), (iMRU+1+pRecentFileList->m_nStart) % 10);

		CXTPControl* pControl = m_pControls->Add(xtpControlButton, iMRU + ID_FILE_MRU_FILE1, _T(""), m_nIndex + iMRU + 1 , TRUE);

		pControl->SetCaption(CString(buf) + strTemp);
		pControl->SetFlags(xtpFlagManualUpdate);
		pControl->SetBeginGroup(iMRU == 0 && m_nIndex != 0);

	}
}



BOOL CXTPControlRecentFileList::IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint /*point*/, DROPEFFECT& dropEffect)
{
	if (pCommandBar->GetType() != xtpBarTypePopup)
	{
		dropEffect = DROPEFFECT_NONE;
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// CXTPControlSelector

IMPLEMENT_XTP_CONTROL(CXTPControlSelector, CXTPControl)

CXTPControlSelector::CXTPControlSelector()
{
	m_bLockUpdate = FALSE;

	m_szItemsCount = CSize(5, 4);
	m_szItemsMax = CSize(12, 12);
	m_szItem = CSize(28, 28);

}

void CXTPControlSelector::OnDrawItem(CDC* /*pDC*/, CRect /*rcItem*/, COLORREF /*clrText*/)
{

}


void CXTPControlSelector::Draw(CDC* pDC)
{
	CXTPPaintManager* pPaintManager = GetPaintManager();

	CRect rcControl = GetRect();

	for (int x = 0; x < m_szItemsVisible.cx; x++)
	for (int y = 0; y < m_szItemsVisible.cy; y++)
	{
		CRect rcItem(rcControl.left + x * m_szItem.cx, rcControl.top + y * m_szItem.cy, rcControl.left + (x + 1)* m_szItem.cx, rcControl.top + (y + 1) * m_szItem.cy);
		rcItem.DeflateRect(1, 1, 1, 1);

		COLORREF clrText = pPaintManager->GetXtremeColor(XPCOLOR_TOOLBAR_TEXT);

		if (x < m_szItemsActive.cx && y < m_szItemsActive.cy)
		{
			pDC->FillSolidRect(rcItem, GetXtremeColor(COLOR_HIGHLIGHT));
			clrText = GetXtremeColor(COLOR_HIGHLIGHTTEXT);
		} else
		{
			pDC->FillSolidRect(rcItem, GetXtremeColor(COLOR_WINDOW));
		}

		pDC->Draw3dRect(rcItem, pPaintManager->GetXtremeColor(XPCOLOR_MENUBAR_BORDER), pPaintManager->GetXtremeColor(XPCOLOR_MENUBAR_BORDER));

		OnDrawItem(pDC, rcItem, clrText);
	}

}

CSize CXTPControlSelector::GetSize(CDC* /*pDC*/)
{
	return CSize (m_szItem.cx * m_szItemsVisible.cx, m_szItem.cy * m_szItemsVisible.cy);
}

void CXTPControlSelector::OnLButtonUp(CPoint /*point*/)
{
	if (m_szItemsActive != CSize(0))
	{
		m_szResult = m_szItemsActive;
		OnExecute();
	}
}

BOOL CXTPControlSelector::OnSetSelected(int bSelected)
{
	if (!bSelected)
	{
		CPoint point;
		GetCursorPos(&point);
		GetParent()->ScreenToClient(&point);

		CRect rcControl = GetRect();
		CSize szItemsVisible = m_szItemsVisible;

		if (GetKeyState(VK_LBUTTON) < 0)
		{
			if (point.x >= rcControl.right)
			{
				szItemsVisible.cx = min(m_szItemsMax.cx, (point.x - rcControl.left) / m_szItem.cx + 1);
			}
			if (point.y >= rcControl.bottom)
			{
				szItemsVisible.cy = min(m_szItemsMax.cy, (point.y - rcControl.top) / m_szItem.cy + 1);
			}
		}

		if (szItemsVisible != m_szItemsVisible)
		{
			m_szItemsVisible = szItemsVisible;
			SetItemsActive(m_szItemsActive, TRUE);

			m_bLockUpdate = TRUE;
			GetParent()->OnRecalcLayout();
			m_bLockUpdate = FALSE;
			OnMouseMove(point);


		}
		else
		{
			SetItemsActive(0);
		}
	}

	return CXTPControl::OnSetSelected(bSelected);
}

void CXTPControlSelector::OnCalcDynamicSize(DWORD dwMode)
{
	CXTPControl::OnCalcDynamicSize(dwMode);

	if (!m_bLockUpdate)
	{
		m_szItemsActive = CSize(0);
		m_szItemsVisible = m_szItemsCount;
	}
	SetItemsActive(m_szItemsActive, TRUE);
}

void CXTPControlSelector::SetItemsActive(CSize szActive, BOOL bNotify)
{
	if (szActive != m_szItemsActive || bNotify)
	{
		m_szItemsActive = szActive;
		RedrawParent();

		NMXTPCONTROL fnm;
		fnm.hdr.code = XTP_SLN_SELCHANGE;
		fnm.hdr.idFrom = GetID();
		fnm.hdr.hwndFrom = 0;
		fnm.pControl = this;

		GetParent()->GetSite()->SendMessage(
			WM_XTP_COMMAND, GetID(), (LPARAM)&fnm);
	}
}

void CXTPControlSelector::OnMouseMove(CPoint point)
{
	CSize szActive(0);

	CRect rcControl = GetRect();

	for (int x = 0; x < m_szItemsVisible.cx; x++)
	{
		if (point.x > rcControl.left + x * m_szItem.cx)
			szActive.cx = x + 1;
	}
	for (int y = 0; y < m_szItemsVisible.cy; y++)
	{
		if (point.y > rcControl.top + y * m_szItem.cy)
			szActive.cy = y + 1;
	}

	SetItemsActive(szActive);
}


void CXTPControlSelector::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	ASSERT_KINDOF(CXTPControlSelector, pControl);

	CXTPControl::Copy(pControl, bRecursive);

	m_szItemsCount = ((CXTPControlSelector*)pControl)->m_szItemsCount;
	m_szItemsMax = ((CXTPControlSelector*)pControl)->m_szItemsMax;
	m_szItem = ((CXTPControlSelector*)pControl)->m_szItem;

}

void CXTPControlSelector::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPControl::DoPropExchange(pPX);

	PX_Size(pPX, _T("ItemsCount"), m_szItemsCount, CSize(0, 0));
	PX_Size(pPX, _T("ItemsMax"), m_szItemsMax, CSize(0, 0));
	PX_Size(pPX, _T("ItemSize"), m_szItem, CSize(0, 0));
}



//////////////////////////////////////////////////////////////////////////
// CXTPControlWorkspaceActions

IMPLEMENT_XTP_CONTROL( CXTPControlWorkspaceActions, CXTPControlButton)

void CXTPControlWorkspaceActions::AddCommand(CXTPTabClientWnd* pClient, UINT nID, int& nIndex)
{
	if (pClient->IsWorkspaceCommandEnabled(nID))
	{
		m_pControls->Add(xtpControlButton, nID, _T(""), nIndex++, TRUE);
	}
}

void CXTPControlWorkspaceActions::OnCalcDynamicSize(DWORD /*dwMode*/)
{
	ASSERT(m_pControls->GetAt(m_nIndex) == this);

	if (!m_pParent)
		return;

	while (m_nIndex + 1 < m_pControls->GetCount())
	{
		CXTPControl* pControl = m_pControls->GetAt(m_nIndex + 1);
		if (pControl->GetID() >= XTP_ID_WORKSAPCE_MOVEPREVIOUS && pControl->GetID() <= XTP_ID_WORKSAPCE_NEWVERTICAL)
		{
			m_pControls->Remove(pControl);
		}
		else break;
	}

	CMDIFrameWnd* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd,m_pParent->GetSite());
	HWND hWndClient = pFrame? pFrame->m_hWndMDIClient: NULL;

	CXTPTabClientWnd* pClient = DYNAMIC_DOWNCAST(CXTPTabClientWnd, CWnd::FromHandlePermanent(hWndClient));

	if (pClient == NULL)
		return;

	m_dwHideFlags |= xtpHideGeneric;

	int nIndex = m_nIndex + 1;

	AddCommand(pClient, XTP_ID_WORKSAPCE_NEWHORIZONTAL, nIndex);
	AddCommand(pClient, XTP_ID_WORKSAPCE_NEWVERTICAL, nIndex);
	AddCommand(pClient, XTP_ID_WORKSAPCE_MOVEPREVIOUS, nIndex);
	AddCommand(pClient, XTP_ID_WORKSAPCE_MOVENEXT, nIndex);

	if (nIndex != m_nIndex + 1)
		m_pControls->GetAt(m_nIndex + 1)->SetBeginGroup(GetBeginGroup());
}


//////////////////////////////////////////////////////////////////////////
//

IMPLEMENT_XTP_CONTROL( CXTPControlLabel, CXTPControl)
