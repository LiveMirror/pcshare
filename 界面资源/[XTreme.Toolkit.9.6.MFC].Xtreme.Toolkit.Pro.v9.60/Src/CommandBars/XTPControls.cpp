// XTPControls.cpp : implementation of the CXTPControls class.
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

#include "XTPControls.h"
#include "XTPCommandBar.h"
#include "XTPPopupBar.h"
#include "XTPControl.h"
#include "XTPControlButton.h"
#include "XTPControlPopup.h"
#include "XTPControlComboBox.h"
#include "XTPControlEdit.h"
#include "XTPControlCustom.h"
#include "XTPControlExt.h"
#include "XTPCommandBarsResource.h"
#include "XTPToolBar.h"
#include "XTPCommandBars.h"
#include "XTPPaintManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommandBarControls

IMPLEMENT_DYNCREATE(CXTPControls, CCmdTarget)

CXTPControls::CXTPControls()
{
	EnableAutomation();


	m_pParent = 0;
	m_pCommandBars = 0;

	m_pOriginalControls = 0;
}

CXTPControls::~CXTPControls()
{
	RemoveAll();

	CMDTARGET_RELEASE(m_pOriginalControls);
}

CXTPCommandBars* CXTPControls::GetCommandBars()
{
	return m_pParent? m_pParent->GetCommandBars(): m_pCommandBars;
}
void CXTPControls::SetCommandBars(CXTPCommandBars* pCommandBars)
{
	m_pCommandBars = pCommandBars;
}

void CXTPControls::RefreshIndexes()
{
	for (int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		CXTPControl* pControl = m_arrControls.GetAt(nIndex);
		pControl->m_nIndex = nIndex;
		pControl->m_pControls = this;
		pControl->SetParent(m_pParent);
	}
	if (m_pParent)
	{
		m_pParent->OnControlsChanged();
	}
}

void CXTPControls::SetParent(CXTPCommandBar* pParent)
{
	m_pParent = pParent;
	if (m_arrControls.GetSize() > 0) RefreshIndexes();
}


CXTPControl* CXTPControls::AddMenuItem(CMenu* pMenu, int nIndex)
{
	CMenu* pPopupMenu = pMenu->GetSubMenu(nIndex);

	CXTPControl* pControl = NULL;
	UINT nID = pMenu->GetMenuItemID(nIndex);

	BOOL bDefaultItem = (int)::GetMenuDefaultItem(pMenu->m_hMenu, GMDI_USEDISABLED, TRUE) == nIndex;

	if (nID == AFX_IDM_FIRST_MDICHILD)
		return NULL;

	CString strCaption;
	pMenu->GetMenuString(nIndex, strCaption, MF_BYPOSITION);

	CWnd* pSite = m_pCommandBars? m_pCommandBars->GetSite():
		m_pParent? (m_pParent->m_pOwner? m_pParent->m_pOwner: m_pParent->GetSite()): 0;

	XTPControlType controlType = pPopupMenu? xtpControlPopup: xtpControlButton;

	XTPButtonStyle buttonStyle = xtpButtonAutomatic;

	if (pSite)
	{
		XTP_COMMANDBARS_CREATECONTROL cs;
		cs.nID = nID;
		cs.pControl = NULL;
		cs.bToolBar = FALSE;
		cs.pCommandBar = m_pParent;
		cs.pMenu = pMenu;
		cs.nIndex = nIndex;
		cs.strCaption = strCaption;
		cs.controlType = controlType;
		cs.buttonStyle = buttonStyle;

		if (pSite->SendMessage(WM_XTP_CREATECONTROL, 0, (LPARAM)&cs) != 0)
		{
			pControl = cs.pControl;
			controlType = cs.controlType;
			strCaption = cs.strCaption;
			nID = cs.nID;
			buttonStyle = cs.buttonStyle;
		}
	}

	if (pControl == NULL)
	{
		switch (controlType)
		{
			case xtpControlButton:
				if (nID == XTP_ID_WINDOWLIST) pControl = (CXTPControl*)CXTPControlWindowList::CreateObject();
				else if (nID == XTP_ID_WORKSPACE_ACTIONS) pControl = (CXTPControl*)CXTPControlWorkspaceActions::CreateObject();
				else if (nID == ID_VIEW_TOOLBAR) pControl = (CXTPControl*)CXTPControlToolbars::CreateObject();
				else if (nID == ID_OLE_VERB_FIRST && strCaption == _T("<<OLE VERBS GO HERE>>")) pControl = (CXTPControl*)CXTPControlOleItems::CreateObject();
				else if (nID == ID_FILE_MRU_FILE1) pControl = (CXTPControl*)CXTPControlRecentFileList::CreateObject();
				else pControl = (CXTPControl*)CXTPControlButton::CreateObject();
				break;
			case xtpControlComboBox:
				pControl = (CXTPControl*)CXTPControlComboBox::CreateObject();
				break;
			case xtpControlEdit:
				pControl = (CXTPControl*)CXTPControlEdit::CreateObject();
				break;

			case xtpControlPopup:
			case xtpControlSplitButtonPopup:
			case xtpControlButtonPopup:
				pControl = CXTPControlPopup::CreateControlPopup(controlType);
				CXTPPopupBar* pPopupBar = 0;
				CString strCaptionPopup(strCaption);
				CXTPPaintManager::StripMnemonics(strCaptionPopup);

				if (pSite)
				{
					XTP_COMMANDBARS_CREATEBAR cs;
					ZeroMemory(&cs, sizeof(cs));
					cs.bPopup = TRUE;
					cs.lpcstrCaption = strCaptionPopup;
					cs.nID = nID;

					if (pSite->SendMessage(WM_XTP_CREATECOMMANDBAR, 0, (LPARAM)&cs) != 0)
					{
						pPopupBar = DYNAMIC_DOWNCAST(CXTPPopupBar, cs.pCommandBar);
						strCaptionPopup = cs.lpcstrCaption;
					}
				}

				if (pPopupBar == NULL) pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());
				pPopupBar->SetTitle(strCaptionPopup);



				CMenu menu;
				if (!pPopupMenu && nID > 0 && menu.LoadMenu(nID))
				{
					pPopupMenu = menu.GetSubMenu(0);
				}
				if (pPopupMenu) pPopupBar->LoadMenu(pPopupMenu, FALSE);

				((CXTPControlPopup*)pControl)->m_pCommandBar = pPopupBar;

				break;
		}

		ASSERT(pControl);
		pControl->m_controlType = controlType;
		pControl->SetStyle(buttonStyle);
	}

	pControl->SetID(nID);
	pControl->SetCaption(strCaption);
	pControl->SetItemDefault(bDefaultItem);

	AddTail(pControl);
	RefreshIndexes();

	return pControl;
}


CXTPControl* CXTPControls::Add(XTPControlType controlType, int nId, CString strParameter, int nBefore, BOOL bTemporary)
{
	CXTPControl* pControl = NULL;
	switch (controlType)
	{
		case xtpControlCustom:
			return Add(new CXTPControlCustom(), nId, strParameter, nBefore, bTemporary);

		case xtpControlButton:
			if (nId == XTP_ID_WINDOWLIST) pControl = (CXTPControl*)CXTPControlWindowList::CreateObject();
			else if (nId == XTP_ID_WORKSPACE_ACTIONS) pControl = (CXTPControl*)CXTPControlWorkspaceActions::CreateObject();
			else if (nId == ID_VIEW_TOOLBAR) pControl = (CXTPControl*)CXTPControlToolbars::CreateObject();
			else if (nId == XTP_ID_FILE_MRU) pControl = (CXTPControl*)CXTPControlRecentFileList::CreateObject();
			else
			pControl = (CXTPControl*)CXTPControlButton::CreateObject();
			pControl->SetID(nId);
			break;
		case xtpControlComboBox:
			pControl = (CXTPControl*)CXTPControlComboBox::CreateObject();
			pControl->SetID(nId);
			break;
		case xtpControlLabel:
			pControl = (CXTPControl*)CXTPControlLabel::CreateObject();
			pControl->SetID(nId);
			break;
		case xtpControlEdit:
			pControl = (CXTPControl*)CXTPControlEdit::CreateObject();
			pControl->SetID(nId);
			break;
		case xtpControlPopup:
		case xtpControlSplitButtonPopup:
		case xtpControlButtonPopup:
			CXTPControlPopup* pControlPopup = (CXTPControlPopup*)CXTPControlPopup::CreateObject();
			CXTPPopupBar* pPopupBar = 0;

			CMenu menu;
			if (nId > 0)
			{
				CString strCaption;
				CMenu* pPopupMenu = NULL;

				if (menu.LoadMenu(nId))
				{
					pPopupMenu = menu.GetSubMenu(0);
					menu.GetMenuString(0, strCaption, MF_BYPOSITION);
				}

				CWnd* pSite = m_pCommandBars? m_pCommandBars->GetSite():
					m_pParent? (m_pParent->m_pOwner? m_pParent->m_pOwner: m_pParent->GetSite()): 0;

				if (pSite)
				{
					XTP_COMMANDBARS_CREATEBAR cs;
					ZeroMemory(&cs, sizeof(cs));
					cs.bPopup = TRUE;
					cs.lpcstrCaption = strCaption;
					cs.nID = nId;

					if (pSite->SendMessage(WM_XTP_CREATECOMMANDBAR, 0, (LPARAM)&cs) != 0)
					{
						pPopupBar = DYNAMIC_DOWNCAST(CXTPPopupBar, cs.pCommandBar);
					}
				}
				if (pPopupBar == NULL) pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());
				pControlPopup->m_strCaption =strCaption;
				if (pPopupMenu)
				{
					pPopupBar->LoadMenu(pPopupMenu);
				}

			}

			if (pPopupBar == NULL) pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());
			pControlPopup->m_pCommandBar = pPopupBar;

			if (controlType != xtpControlPopup)
				pControlPopup->SetID(nId);
			pControl = pControlPopup;
	}

	ASSERT(pControl);

	pControl->m_controlType = controlType;
	pControl->m_strParameter = strParameter;
	pControl->m_bTemporary = bTemporary;

	if (nBefore < 0 || nBefore >= m_arrControls.GetSize())
		nBefore = (int)m_arrControls.GetSize();

	m_arrControls.InsertAt(nBefore, pControl);
	RefreshIndexes();

	return pControl;
}

CXTPControl* CXTPControls::Add(CXTPControl* pControl, int nId, CString strParameter, int nBefore, BOOL bTemporary)
{
	ASSERT(pControl);
	pControl->m_strParameter = strParameter;
	pControl->m_bTemporary = bTemporary;
	pControl->SetID(nId);

	if (nBefore < 0 || nBefore >= m_arrControls.GetSize())
		nBefore = (int)m_arrControls.GetSize();

	m_arrControls.InsertAt(nBefore, pControl);
	RefreshIndexes();

	return pControl;
}

CXTPControl* CXTPControls::AddClone(CXTPControl* pClone, int nBefore, BOOL bRecursive)
{
	CXTPControl* pControl = pClone->Clone(bRecursive);
	ASSERT(pControl);

	if (nBefore < 0 || nBefore >= m_arrControls.GetSize())
		nBefore = (int)m_arrControls.GetSize();

	m_arrControls.InsertAt(nBefore, pControl);
	RefreshIndexes();

	return pControl;
}



void CXTPControls::RemoveAll(void)
{
	for (int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		CXTPControl* pControl = m_arrControls.GetAt(nIndex);
		pControl->m_pControls = NULL;
		pControl->m_pParent = NULL;
		pControl->InternalRelease();
	}
	m_arrControls.RemoveAll();
}


void CXTPControls::AddTail(CXTPControl* pControl)
{
	m_arrControls.InsertAt(m_arrControls.GetSize(), pControl);
}

CXTPControl* CXTPControls::GetFirst()
{
	return m_arrControls.GetSize() > 0? m_arrControls.GetAt(0): 0;
}

void CXTPControls::GetNext(CXTPControl*& pControl)
{
	pControl = (!pControl || pControl->m_nIndex >= GetCount() - 1)? 0: GetAt(pControl->m_nIndex + 1);
}

CXTPControl* CXTPControls::FindControl(int nId)
{
	return FindControl(xtpControlError, nId, FALSE, FALSE);
}

CXTPControl* CXTPControls::FindControl(XTPControlType type, int nId, BOOL bVisible, BOOL bRecursive)
{
	for (int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		CXTPControl* pControl = m_arrControls.GetAt(nIndex);
		if ((type == xtpControlError || pControl->GetType() == type) &&
			(!bVisible || pControl->IsVisible()) &&
			(nId == -1 || nId == pControl->GetID()))
			return pControl;

		if (bRecursive && IsPopupControlType(pControl->GetType()))
		{
			CXTPCommandBar* pBar = ((CXTPControlPopup*)pControl)->GetCommandBar();
			if (pBar)
			{
				pControl = pBar->GetControls()->FindControl(type, nId, bVisible, bRecursive);
				if (pControl != NULL) return pControl;
			}
		}
	}
	return NULL;
}

void CXTPControls::Remove(CXTPControl* pControl)
{
	if (pControl)
	{
		if (pControl->GetSelected())
			m_pParent->SetSelected(-1);

		ASSERT(pControl->m_pControls == this);

		//Ensure the index falls within the correct range.
		if ((pControl->m_nIndex >= 0) && (pControl->m_nIndex < m_arrControls.GetSize()))
		{
			m_arrControls.RemoveAt(pControl->m_nIndex);

			pControl->m_pControls = NULL;
			pControl->m_pParent = NULL;
			pControl->InternalRelease();
			RefreshIndexes();
		}
	}
}

void CXTPControls::Remove(int nIndex)
{
	CXTPControl* pControl = GetAt(nIndex);
	Remove(pControl);
}

CXTPControl* CXTPControls::SetControlType(int nIndex, XTPControlType type)
{
	return SetControlType(GetAt(nIndex), type);
}


CXTPControl* CXTPControls::SetControlType(CXTPControl* pControl, XTPControlType type)
{
	ASSERT(pControl->m_pControls == this);
	ASSERT(pControl);

	if (IsPopupControlType(pControl->GetType()) && IsPopupControlType(type))
	{
		pControl->m_controlType = type;
		return pControl;
	}

	CXTPControl* pNew = Add(type, 0, _T(""), pControl->GetIndex() + 1, FALSE);
	pNew->CXTPControl::Copy(pControl, FALSE);
	pNew->m_controlType = type;

	Remove(pControl);

	RefreshIndexes();

	return pNew;

}


long CXTPControls::GetNext(long nIndex, int nDirection, BOOL bKeyboard, BOOL bSkipTemporary, BOOL bSkipCollapsed)
{
	ASSERT(nDirection == +1 || nDirection == -1);
	if (GetCount() == 0) return -1;

	long nNext = nIndex + nDirection;

	if (nDirection == -1 && nIndex == -1)
	{
		nNext = GetCount() - 1;
	}

	int nSkipVisible = xtpHideScroll | (!bSkipCollapsed? xtpHideExpand: 0);

	BOOL bCircle = FALSE;

	while (nNext != nIndex)
	{
		if (nNext >= GetCount())
		{
			if ((nIndex == -1 && nDirection == +1) || bCircle) return -1;
			nNext = 0;
			bCircle = TRUE;
		}
		if (nNext < 0)
		{
			if ((nIndex == -1 && nDirection == -1) || bCircle) return -1;
			nNext = GetCount() - 1;
			bCircle = TRUE;
		}

		if (((GetAt(nNext)->m_dwHideFlags & ~nSkipVisible) == xtpNoHide )&& (!bKeyboard || !(GetAt(nNext)->m_dwFlags & xtpFlagSkipFocus)))
		{
			if (!bSkipTemporary || !GetAt(nNext)->m_bTemporary)
				return nNext;
		}
		nNext += nDirection;
	}
	return nNext;
}

CXTPControl* CXTPControls::HitTest(CPoint point)
{
	for (CXTPControl* pControl = GetFirst(); pControl; GetNext(pControl))
	{
		if (pControl->IsVisible() )
		{
			if (pControl->m_rcControl.PtInRect(point))
				return pControl;
		}

	}
	return NULL;
}


struct CXTPControls::XTPBUTTONINFO  // struct for CalcDynamicSize ;
{
	CRect rcControl;
	CRect rcRow;
	CSize szControl;
	BOOL bVisible;
	BOOL bWrap;
	BOOL bHide;
	BOOL bBeginGroup;

	void Attach(CDC* pDC, CXTPControl* pControl)
	{
		bHide = bWrap = FALSE;
		rcRow.SetRectEmpty();

		szControl = pControl->GetSize(pDC);
		rcControl = pControl->GetRect();
		bBeginGroup = pControl->GetBeginGroup();
		bVisible = (pControl->GetHideFlags() & (~xtpHideWrap)) == xtpNoHide;
	}
	void Detach(CXTPControl* pControl)
	{
		pControl->SetHideWrap(bHide);
		pControl->SetRawRect(rcRow);
		if (!bHide)
		pControl->SetRect(rcControl);
		pControl->SetWrap(bWrap);
	}

};

CSize CXTPControls::_CalcSize(XTPBUTTONINFO* pData, BOOL /*bCommit*/, BOOL bVert)
{

	CPoint cur(0,0);
	CSize sizeResult(0,0);
	int nRaw = 0;
	BOOL bFirst = TRUE;
	for (int i = 0; i < GetCount(); i++)
	{
		XTPBUTTONINFO* pControl = &pData[i];

		if (!pControl->bVisible || pControl->bHide)
			continue;

		CSize szControl = pControl->szControl;

		if (bVert)
		{
			if (pControl->bBeginGroup && !pControl->bWrap && !bFirst)
				cur.y += 6;


			if (pControl->bWrap)
			{
				cur.x -= nRaw + (pControl->bBeginGroup? 6: 0);
				nRaw = cur.y = 0;
			}
			nRaw = max(nRaw, szControl.cx);

			pControl->rcControl.SetRect(cur.x - szControl.cx, cur.y, cur.x , cur.y + szControl.cy);

			sizeResult.cx = max(szControl.cx - cur.x, sizeResult.cx);
			sizeResult.cy = max(cur.y + szControl.cy, sizeResult.cy);

			cur.y += szControl.cy;

		} else
		{

			if (pControl->bBeginGroup && !pControl->bWrap && !bFirst)
				cur.x += 6;

			if (pControl->bWrap)
			{
				cur.y += nRaw + (pControl->bBeginGroup? 6: 0);
				nRaw = cur.x = 0;
			}
			nRaw = max(nRaw, szControl.cy);

			pControl->rcControl.SetRect(cur.x, cur.y, cur.x + szControl.cx, cur.y + szControl.cy);

			sizeResult.cx = max(cur.x + szControl.cx, sizeResult.cx);
			sizeResult.cy = max(cur.y + szControl.cy, sizeResult.cy);

			cur.x += szControl.cx;
		}
		bFirst = FALSE;
	}
	return sizeResult;
}

int CXTPControls::_WrapToolBar(XTPBUTTONINFO* pData, int nWidth, DWORD& dwMode)
{

	int nResult = 0, x = 0;
	BOOL bVert = dwMode & LM_VERTDOCK;
	BOOL bFirst = TRUE;

	for (int i = 0; i < GetCount(); i++)
	{
		XTPBUTTONINFO* pControl = &pData[i];
		pControl->bHide = FALSE;
		pControl->bWrap = FALSE;

		if (!pControl->bVisible)
			continue;

		int dx = bVert? pControl->szControl.cy: pControl->szControl.cx;

		if (pControl->bBeginGroup && !bFirst)
			dx += 6;

		if (x + dx > nWidth)
		{
			if (dwMode & LM_HIDEWRAP && !bFirst)
			{
				for (int j = i; j < GetCount(); j++)
					pData[j].bHide = TRUE;
				dwMode |= LM_STRETCH;
				return nResult;
			}

			for (int j = i; j >= 0  && pData[j].bWrap == FALSE; j--)
			{
				// Find last separator
				if (pData[j].bBeginGroup && pData[j].bVisible)
				{
					i = j;
					break;
				}
			}
			x = bVert? pData[i].szControl.cy: pData[i].szControl.cx;

			pData[i].bWrap = TRUE;
			if (!(dwMode & LM_HIDEWRAP)) dwMode |= LM_STRETCH;
			nResult++;
		}
		else
			x += dx;

		bFirst = FALSE;

	}
	return nResult + 1;
}

void  CXTPControls::_SizeFloatableBar(XTPBUTTONINFO* pData, int nLength, DWORD dwMode)
{
	if (! (dwMode & LM_LENGTHY))
	{
		int nMin, nMax, nTarget, nCurrent, nMid;

		// Wrap ToolBar as specified
		nMax = nLength;
		nTarget = _WrapToolBar(pData, nMax, dwMode);

		// Wrap ToolBar vertically
		nMin = 0;
		nCurrent = _WrapToolBar(pData, nMin, dwMode);

		if (nCurrent != nTarget)
		{
			while (nMin < nMax)
			{
				nMid = (nMin + nMax) / 2;
				nCurrent = _WrapToolBar(pData, nMid, dwMode);

				if (nCurrent == nTarget)
					nMax = nMid;
				else
				{
					if (nMin == nMid)
					{
						_WrapToolBar(pData, nMax, dwMode);
						break;
					}
					nMin = nMid;
				}
			}
		}
		CSize size = _CalcSize(pData);
		_WrapToolBar(pData, size.cx, dwMode);
	}
	else
	{
		CSize sizeMax, sizeMin, sizeMid;

		// Wrap ToolBar vertically
		_WrapToolBar(pData, 0, dwMode);
		sizeMin = _CalcSize(pData);

		// Wrap ToolBar horizontally
		_WrapToolBar(pData, 32767, dwMode);
		sizeMax = _CalcSize(pData);

		while (sizeMin.cx < sizeMax.cx)
		{
			sizeMid.cx = (sizeMin.cx + sizeMax.cx) / 2;
			_WrapToolBar(pData, sizeMid.cx, dwMode);
			sizeMid = _CalcSize(pData);

			if (nLength < sizeMid.cy)
			{
				if (sizeMin == sizeMid)
				{
					_WrapToolBar(pData, sizeMax.cx, dwMode);
					return;
				}
				sizeMin = sizeMid;
			}
			else if (nLength > sizeMid.cy)
				sizeMax = sizeMid;
			else
				return;
		}
	}
}
void CXTPControls::_CenterControlsInRow(XTPBUTTONINFO* pData, int nFirst, int nLast, int nHeight, BOOL bVert, CSize sizeResult, CRect rcBorder)
{
	for (int i = nFirst; i < nLast; i++)
	{
		XTPBUTTONINFO* pControl = &pData[i];
		CRect rc = pControl->rcControl;

		if (bVert)
		{
			pControl->rcRow.SetRect(rc.right - nHeight, rcBorder.top, rc.right, sizeResult.cy  + rcBorder.top);
			pControl->rcControl.OffsetRect(-(nHeight - rc.Width())/2, 0);
		}
		else
		{
			pControl->rcRow.SetRect(rcBorder.left, rc.top, sizeResult.cx + rcBorder.left, rc.top + nHeight);
			pControl->rcControl.OffsetRect(0, (nHeight - rc.Height())/2);
		}
	}

}

void CXTPControls::_AdjustBorders(XTPBUTTONINFO* pData, CSize& sizeResult, DWORD dwMode, CRect rcBorder)
{
	if (dwMode & LM_COMMIT)
	{
		BOOL bVert = dwMode & LM_VERTDOCK;
		int nFirstInRow = 0;
		int nHeight = 0;
		int nBarHeight = 0;

		for (int i = 0; i < GetCount(); i++)
		{
			XTPBUTTONINFO* pControl = &pData[i];

			if (!pControl->bVisible || pControl->bHide)
				continue;

			pControl->rcControl.OffsetRect(rcBorder.left, rcBorder.top);
			if (bVert)  pControl->rcControl.OffsetRect(sizeResult.cx, 0);

			if (pControl->bWrap)
			{
				_CenterControlsInRow(pData, nFirstInRow, i, nHeight, bVert, sizeResult, rcBorder);
				nFirstInRow = i;
				nHeight = 0;
			}

			nBarHeight = bVert? sizeResult.cx: sizeResult.cy;
			nHeight = max(nHeight, bVert? pControl->rcControl.Width(): pControl->rcControl.Height());

			//nHeight = max(nHeight, bVert? pControl->rcControl.Width(): pControl->rcControl.Height());
		}
		_CenterControlsInRow(pData, nFirstInRow, GetCount(), nFirstInRow == 0? nBarHeight: nHeight, bVert, sizeResult, rcBorder );
	}

	sizeResult += CSize(rcBorder.left + rcBorder.right, rcBorder.top + rcBorder.bottom);
}

void CXTPControls::_MoveRightAlligned(XTPBUTTONINFO* pData, CSize sizeResult, CRect rcBorder, DWORD dwMode)
{
	BOOL bVert = dwMode & LM_VERTDOCK;
	int nRight = bVert ? sizeResult.cy - rcBorder.bottom : sizeResult.cx - rcBorder.right;

	int i;
	for (i = GetCount() - 1; i >= 0; i--)
	{
		XTPBUTTONINFO* pControl = &pData[i];

		if (!pControl->bVisible || pControl->bHide)
			continue;

		if (!(GetAt(i)->m_dwFlags & xtpFlagRightAlign))
			break;

		CRect rc = pControl->rcControl;
		if (!bVert)
		{
			if (nRight - rc.Width() < rcBorder.right)
				break;

			pControl->rcControl.SetRect(nRight - rc.Width(), rc.top, nRight, rc.bottom);
			nRight -= rc.Width();
		} else
		{
			if (nRight - rc.Height() < rcBorder.top)
				break;

			pControl->rcControl.SetRect(rc.left, nRight - rc.Height(), rc.right, nRight);
			nRight -= rc.Height();
		}
		if (pControl->bWrap)
			break;
	}

	int nStretchAvail = -1;
	int nStretchedCount = 0;
	BOOL bSkipRightAligned = FALSE;

	nRight = bVert ? sizeResult.cy - rcBorder.bottom : sizeResult.cx - rcBorder.right;
	int nLastInRow = GetCount() - 1;

	for (i = GetCount() - 1; i >= 0; i--)
	{
		XTPBUTTONINFO* pControl = &pData[i];

		if (pControl->bVisible && !pControl->bHide)
		{
			if (!bSkipRightAligned && (GetAt(i)->m_dwFlags & xtpFlagRightAlign))
			{
				nRight = pControl->rcControl.left;
				nLastInRow = i - 1;
			}
			else if (GetAt(i)->m_dwFlags & xtpFlagControlStretched)
			{
				nStretchedCount++;
				bSkipRightAligned = TRUE;
			}

			if ((nStretchAvail == -1) && (nLastInRow >= i))
			{
				nStretchAvail = nRight - (!bVert? pControl->rcControl.right: pControl->rcControl.bottom);
			}
		}

		if ((pControl->bVisible && !pControl->bHide && pControl->bWrap) || (i == 0))
		{
			if (nStretchedCount > 0 && nStretchAvail > 0)
			{
				int nOffset = 0;
				for (int j = i; j <= nLastInRow; j++)
				{
					XTPBUTTONINFO* pControl = &pData[j];

					if (!pControl->bVisible || pControl->bHide)
						continue;

					CRect rc = pControl->rcControl;
					if (!bVert)
						pControl->rcControl.SetRect(nOffset + rc.left, rc.top, nOffset + rc.right, rc.bottom);
					else
						pControl->rcControl.SetRect(rc.left, nOffset + rc.top, rc.right, nOffset + rc.bottom);

					if ((GetAt(j)->m_dwFlags & xtpFlagControlStretched) && (nStretchedCount > 0))
					{
						int nStretchedOffset = nStretchAvail / nStretchedCount;
						if (!bVert) pControl->rcControl.right += nStretchedOffset; else pControl->rcControl.bottom += nStretchedOffset;

						nStretchAvail -= nStretchedOffset;
						nStretchedCount--;
						nOffset += nStretchedOffset;
					}
				}

			}

			nRight = bVert ? sizeResult.cy - rcBorder.bottom : sizeResult.cx - rcBorder.right;
			bSkipRightAligned = TRUE;
			nLastInRow = i - 1;
			nStretchAvail = -1;
			nStretchedCount = 0;
		}
	}
}

int CXTPControls::GetVisibleCount(BOOL bIgnoreWraps)
{
	int nFlags = bIgnoreWraps? 0: xtpHideWrap;
	int nCount = 0;
	for (int i = 0; i < m_arrControls.GetSize(); i++)
	{
		if ((m_arrControls.GetAt(i)->GetHideFlags() & ~nFlags) == xtpNoHide)  nCount ++;
	}
	return nCount;
}


CSize CXTPControls::CalcDynamicSize(CDC* pDC, int nLength, DWORD dwMode, const CRect& rcBorder, int nWidth)
{
	CSize sizeResult(0,0);

	for (int i = GetCount() - 1; i >=0; i--)
	{
		GetAt(i)->OnCalcDynamicSize(dwMode);
	}

	if (GetVisibleCount() > 0)
	{

		XTPBUTTONINFO* pData = new XTPBUTTONINFO[GetCount()];
		for (int j = 0; j < GetCount(); j++)
			pData[j].Attach(pDC, GetAt(j));


		if (dwMode & LM_MRUWIDTH)
		{
			nLength = m_pParent->m_nMRUWidth;
		}
		if (dwMode & LM_POPUP)
		{
			_WrapToolBar(pData, /*0*/m_pParent->m_nMRUWidth, dwMode);
			CSize sz = _CalcSize(pData, FALSE, FALSE);
			_WrapToolBar(pData, sz.cx , dwMode);

		} else
		if (dwMode & LM_HORZDOCK)
		{
			_WrapToolBar(pData, nLength - rcBorder.right - rcBorder.left, dwMode);
		}
		else if (dwMode & LM_VERTDOCK)
		{
			_WrapToolBar(pData, nLength - rcBorder.top - rcBorder.bottom, dwMode);
		}
		else
		{
			_SizeFloatableBar(pData, nLength - ((dwMode & LM_LENGTHY)? rcBorder.top + rcBorder.bottom: rcBorder.right + rcBorder.left), dwMode);
		}


		sizeResult = _CalcSize(pData, dwMode & LM_COMMIT, dwMode & LM_VERTDOCK);

		if (nWidth > 0)
		{
			if (dwMode & LM_HORZDOCK)
				sizeResult.cy = max(nWidth - rcBorder.top - rcBorder.bottom, sizeResult.cy);
			else
				sizeResult.cx = max(nWidth - rcBorder.right - rcBorder.left, sizeResult.cx);
		}

		if (dwMode & LM_STRETCH)
		{
			if (dwMode & LM_VERTDOCK)
				sizeResult.cy = max(nLength - rcBorder.top - rcBorder.bottom, sizeResult.cy);
			else
				sizeResult.cx = max(nLength - rcBorder.right - rcBorder.left, sizeResult.cx);

		}
		_AdjustBorders(pData, sizeResult, dwMode, rcBorder);

		if (dwMode & LM_COMMIT)
		{
			//if (!(dwMode & (LM_HORZDOCK | LM_VERTDOCK))) m_pParent->m_nMRUWidth = sizeResult.cx;
			_MoveRightAlligned(pData, sizeResult, rcBorder, dwMode);
			for (int i = 0; i < GetCount(); i++) pData[i].Detach(GetAt(i));
		}
		delete[] pData;
	} else
	{
		sizeResult = CSize (23, 22) + CSize(rcBorder.left + rcBorder.right, rcBorder.top + rcBorder.bottom);;
		if (dwMode & LM_HORZDOCK) sizeResult.cy = max(nWidth, sizeResult.cy);
		else sizeResult.cx = max(nWidth, sizeResult.cx);
	}

	return sizeResult;


}

CXTPControls* CXTPControls::Duplicate(BOOL bRecursive)
{
	CXTPControls* pControls = new CXTPControls;
	for (int i = 0; i < GetCount(); i++)
	{
		if (!GetAt(i)->m_bTemporary)
			pControls->AddClone(GetAt(i), -1, bRecursive);
	}

	if (m_pOriginalControls)
	{
		pControls->m_pOriginalControls = m_pOriginalControls->Duplicate(bRecursive);
	}

	return pControls;
}


void CXTPControls::ClearOriginalControls()
{
	CMDTARGET_RELEASE(m_pOriginalControls);
}


void CXTPControls::CreateOriginalControls()
{
	ClearOriginalControls();

	m_pOriginalControls = Duplicate(TRUE);
}

void CXTPControls::MoveBefore(CXTPControl* pControl, int nBefore)
{
	ASSERT(pControl);
	ASSERT(pControl->GetControls() == this);

	if (pControl->GetIndex() < nBefore) nBefore--;

	m_arrControls.RemoveAt(pControl->GetIndex());
	m_arrControls.InsertAt(nBefore, pControl);
	RefreshIndexes();
}

BOOL CXTPControls::Compare(CXTPControls* pOther)
{
	int j = 0, i = 0;
	while (i < GetCount() || j < pOther->GetCount())
	{
		while (i < GetCount() && GetAt(i)->m_bTemporary) i++;
		while (j < pOther->GetCount() && pOther->GetAt(j)->m_bTemporary) j++;

		if (i < GetCount() && j < pOther->GetCount())
		{
			if (!GetAt(i)->Compare(pOther->GetAt(j)))
				return FALSE;
		} else
		{
			if (i < GetCount() || j < pOther->GetCount())
				return FALSE;
		}

		i++; j++;
	}

	return TRUE;
}

BOOL CXTPControls::IsChanged()
{
	if (!m_pOriginalControls)
		return TRUE;

	return !m_pOriginalControls->Compare(this);
}

