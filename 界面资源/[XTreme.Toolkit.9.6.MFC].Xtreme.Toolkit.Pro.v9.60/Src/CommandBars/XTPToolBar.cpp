// XTPToolBar.cpp : implementation of the CXTPToolBar class.
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
#include "XTPResourceManager.h"
#include "XTPImageManager.h"
#include "XTPColorManager.h"
#include "XTPDrawHelpers.h"

#include "XTPCommandBarsResource.h"
#include "XTPToolBar.h"
#include "XTPCommandBars.h"
#include "XTPDockBar.h"
#include "XTPControls.h"
#include "XTPControl.h"
#include "XTPPaintManager.h"
#include "XTPMouseManager.h"
#include "XTPControlPopup.h"
#include "XTPPopupBar.h"
#include "XTPControlButton.h"
#include "XTPDockContext.h"
#include "XTPShadowsManager.h"
#include "XTPShortcutManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef CBRS_GRIPPER
#define CBRS_GRIPPER                    0x00400000L
#endif

#define _XTP_CBRS_ALL            0x0040FFFFL

IMPLEMENT_XTP_COMMANDBAR(CXTPToolBar, CXTPCommandBar)

class CXTPToolBar::CXTPControlButtonHide: public CXTPControlButton
{
public:
	CXTPControlButtonHide()
	{
		m_dwFlags = xtpFlagManualUpdate | xtpFlagSkipFocus | xtpFlagNoMovable;
	}
	void Draw(CDC* pDC);
	void SetBeginGroup(BOOL /*bBeginGroup*/) {}
	void OnExecute()
	{
		ASSERT(m_pParent);
		((CXTPToolBar*)m_pParent)->SetVisible(FALSE);
	}
};

void CXTPToolBar::CXTPControlButtonHide::Draw(CDC* pDC)
{
	GetPaintManager()->DrawSpecialControl(pDC, xtpButtonHideFloating, this, m_pParent, TRUE, NULL);
}

class CXTPToolBar::CXTPControlButtonCustomize: public CXTPControlButton
{
public:
	CXTPControlButtonCustomize()
	{
		m_pControl = NULL;
		m_pToolBar = NULL;
	}
	void Draw(CDC* pDC)
	{
		GetPaintManager()->DrawSpecialControl(pDC, xtpButtonCustomize, this, m_pParent, TRUE, NULL);
	}
	CSize GetSize(CDC* pDC)
	{
		return GetPaintManager()->DrawSpecialControl(pDC, xtpButtonCustomize, this, m_pParent, FALSE, NULL);
	}
	virtual void OnExecute()
	{
		ASSERT(m_pParent);
		CXTPCommandBars* pCommandBars = m_pParent->GetCommandBars();
		ASSERT(pCommandBars);
		pCommandBars->GetCommandBarsOptions()->bDirtyState = TRUE;

		if (m_pControl)
		{
			if (m_pControl->GetHideFlags() & xtpHideCustomize)
				m_pControl->SetHideFlags(m_pControl->GetHideFlags() & ~xtpHideCustomize);
			else m_pControl->SetHideFlags(m_pControl->GetHideFlags() | xtpHideCustomize);

			SetChecked((m_pControl->GetHideFlags() & xtpHideCustomize) == 0);
			m_pControl->GetParent()->OnRecalcLayout();
			m_pParent->InvalidateParents();
		}
		if (m_pToolBar)
		{
			XTPMouseManager()->SendTrackLost();
			m_pToolBar->Reset();
		}
	}

	CXTPControl* m_pControl;
	CXTPToolBar* m_pToolBar;
};


class CXTPToolBar::CXTPControlExpandButton: public CXTPControlPopup
{
public:
	CXTPControlExpandButton()
	{
		m_controlType = xtpControlButtonPopup;
		m_bHiddenExists = FALSE;
		m_dwFlags = xtpFlagLeftPopup | xtpFlagManualUpdate | xtpFlagNoMovable;
	}
	void Draw(CDC* pDC);
	BOOL OnSetPopup(BOOL bPopup);
	void SetRect(CRect rcControl) { m_rcControl = rcControl; m_bHiddenExists = IsHiddenExists();};
	void SetBeginGroup(BOOL /*bBeginGroup*/) {}
	BOOL IsHiddenExists();
private:
	BOOL m_bHiddenExists;
};

BOOL CXTPToolBar::CXTPControlExpandButton::IsHiddenExists()
{
	for (int i = 0; i < m_pParent->GetControls()->GetCount(); i++)
	{
		if (m_pParent->GetControl(i)->GetHideFlags() == xtpHideWrap)
			return TRUE;
	}
	return FALSE;
}

void CXTPToolBar::CXTPControlExpandButton::Draw(CDC* pDC)
{
	GetPaintManager()->DrawSpecialControl(pDC, m_pParent->GetPosition() == xtpBarFloating?
		xtpButtonExpandFloating: xtpButtonExpandToolbar, this, m_pParent, TRUE, &m_bHiddenExists);
}

BOOL CXTPToolBar::CXTPControlExpandButton::OnSetPopup(BOOL bPopup)
{
	m_bPopup = bPopup;
	if (bPopup)
	{
		if (m_pCommandBar)
			m_pCommandBar->InternalRelease();

		m_pCommandBar = NULL;
		CWnd* pSite = m_pParent->GetSite();
		if (pSite)
		{
			XTP_COMMANDBARS_CREATEBAR cs;
			ZeroMemory(&cs, sizeof(cs));
			cs.bExpandBar = TRUE;

			if (pSite->SendMessage(WM_XTP_CREATECOMMANDBAR, 0, (LPARAM)&cs) != 0)
			{
				m_pCommandBar = DYNAMIC_DOWNCAST(CXTPPopupBar, cs.pCommandBar);
			}
		}
		if (m_pCommandBar == NULL)  m_pCommandBar = CXTPPopupToolBar::CreatePopupToolBar(m_pParent->GetCommandBars());
		m_pCommandBar->EnableCustomization(FALSE);

		CXTPCommandBars* pCommandBars = m_pParent->GetCommandBars();
		ASSERT(pCommandBars);

		pCommandBars->GetHiddenControls((CXTPToolBar*)m_pParent, m_pCommandBar);

		if (pCommandBars->GetCommandBarsOptions()->bShowExpandButtonAlways || pCommandBars->IsCustomizeAvail())
		{
			CXTPControlPopup* pControl = (CXTPControlPopup*)m_pCommandBar->GetControls()->Add(xtpControlPopup, 0, _T(""), -1, FALSE);

			CString strCaption;
			XTPResourceManager()->LoadString(&strCaption, XTP_IDS_ADDORREMOVE);
			pControl->SetCaption(strCaption);

			pControl->SetBeginGroup(m_pCommandBar->GetControls()->GetCount() > 1);

			pCommandBars->GetAddOrRemovePopup((CXTPToolBar*)m_pParent, pControl->GetCommandBar());
		} else
		{
			((CXTPPopupToolBar*)m_pCommandBar)->SetWidth(150);

		}

		m_pCommandBar->Popup(this);

	} else
	{
		ASSERT(m_pCommandBar);
		m_pCommandBar->SetTrackingMode(FALSE);

	}
	DelayRedrawParent();
	return TRUE;
}


CXTPToolBar::CXTPToolBar(void)
{
	m_pDockBar = 0;
	m_pDockContext = 0;
	m_bBuiltIn = TRUE;
	m_bTearOff = FALSE;

	m_dwStyle = CBRS_GRIPPER;

	m_bCloseable = TRUE;
	m_bTemporary = FALSE;

	m_bPreviewMode = m_bPreviewVisible = FALSE;
}

BOOL CXTPToolBar::CreateToolBar(DWORD dwStyle, CWnd* pParentWnd, UINT nID)
{
	m_dwStyle = dwStyle & _XTP_CBRS_ALL;
	dwStyle = dwStyle & ~_XTP_CBRS_ALL;

	dwStyle = dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	if (!CWnd::CreateEx(0, AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW)), NULL, dwStyle, CRect(0, 0, 0, 0), pParentWnd, nID))
		return FALSE;

	m_barPosition = xtpBarTop;

	if (m_dwStyle & CBRS_TOOLTIPS)
		EnableToolTips();

	return TRUE;

}

void CXTPToolBar::EnableDocking(DWORD dwFlags)
{
	if (m_pDockContext == 0)
	{
		m_pDockContext = new CXTPDockContext(this);
	}
	m_dwFlags = dwFlags;
}

CXTPToolBar::~CXTPToolBar(void)
{
	if (m_pDockContext)
	{
		delete m_pDockContext;
	}

}



BOOL CXTPCommandBar::SetButtons(UINT* pButtons, int nCount)
{
	m_pControls->RemoveAll();

	BOOL bSeparator = FALSE;
	for (int i = 0; i < nCount; i++)
	{
		if (pButtons[i] == 0)
			bSeparator = TRUE;
		else
		{
			XTPControlType controlType = xtpControlButton;
			XTPButtonStyle buttonStyle = xtpButtonAutomatic;
			CXTPControl* pControl = NULL;
			UINT nID = pButtons[i];

			if (GetSite())
			{
				XTP_COMMANDBARS_CREATECONTROL cs;
				cs.nID = nID;
				cs.pControl = NULL;
				cs.bToolBar = TRUE;
				cs.pMenu = NULL;
				cs.nIndex = i;
				cs.strCaption = GetTitle();
				cs.controlType = controlType;
				cs.pCommandBar = this;
				cs.buttonStyle = buttonStyle;

				if (GetSite()->SendMessage(WM_XTP_CREATECONTROL, 0, (LPARAM)&cs) != 0)
				{
					pControl = cs.pControl;
					controlType = cs.controlType;
					buttonStyle = cs.buttonStyle;
					nID = cs.nID;
				}
			}

			if (pControl == NULL)
			{
				pControl = m_pControls->Add(controlType, nID);
				if (pControl)
				{
					pControl->SetStyle(buttonStyle);
					if (controlType == xtpControlPopup) pControl->SetIconId(nID);
				}
			}
			else m_pControls->Add(pControl, nID);

			if (bSeparator)
			{
				pControl->SetBeginGroup(TRUE);
				bSeparator = FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CXTPToolBar::LoadToolBar(UINT nIDResource, BOOL bLoadIcons)
{
	m_nBarID = nIDResource;

	if (!CXTPCommandBar::LoadToolBar(nIDResource, bLoadIcons))
		return FALSE;

	m_pControls->CreateOriginalControls();

	return TRUE;
}

BOOL CXTPCommandBar::LoadToolBar(UINT nIDResource, BOOL bLoadIcons)
{
	LPCTSTR lpszResourceName = MAKEINTRESOURCE(nIDResource);

	struct CToolBarData
	{
		WORD wVersion;
		WORD wWidth;
		WORD wHeight;
		WORD wItemCount;
		WORD* items()
		{ return (WORD*)(this+1); }
	};

	m_pControls->RemoveAll();
	ASSERT(m_pControls->GetCount() == 0);

	ASSERT_VALID(this);
	ASSERT(lpszResourceName != NULL);


	// determine location of the bitmap in resource fork
	HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_TOOLBAR);
	HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_TOOLBAR);
	if (hRsrc == NULL)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
	if (hGlobal == NULL)
		return FALSE;

	CToolBarData* pData = (CToolBarData*)LockResource(hGlobal);
	if (pData == NULL)
		return FALSE;

	ASSERT(pData->wVersion == 1);

	int i;
	UINT* pItems = new UINT[pData->wItemCount];

	for (i = 0; i < pData->wItemCount; i++)
		pItems[i] = pData->items()[i];

	BOOL bResult = SetButtons(pItems, pData->wItemCount);

	if (bLoadIcons)
	{
		CXTPImageManager* pImageManager = GetImageManager();

		if (!pImageManager->SetIcons(nIDResource, pItems,
			pData->wItemCount, CSize(pData->wWidth, pData->wHeight)))
			bResult = FALSE;

		if (pData->wWidth != 16)
		{
			SetIconSize(CSize(pData->wWidth, pData->wHeight));
		}
	}

	delete[] pItems;

	UnlockResource(hGlobal);
	FreeResource(hGlobal);


	return bResult;
}


BOOL CXTPToolBar::SetPosition(XTPBarPosition barPosition)
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (!pCommandBars)
		return FALSE;

	if (IsDockingPosition(barPosition))
	{
		if (!pCommandBars->DockCommandBar(this, barPosition))
			return FALSE;

	} else if (barPosition == xtpBarFloating)
	{
		if (!pCommandBars->FloatCommandBar(this))
			return FALSE;

	} else  return FALSE;

	ASSERT(m_barPosition == barPosition);

	return TRUE;
}


BEGIN_MESSAGE_MAP(CXTPToolBar, CXTPCommandBar)
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONDOWN()

	ON_WM_NCCREATE()
	ON_WM_NCACTIVATE()
	ON_MESSAGE(WM_FLOATSTATUS, OnFloatStatus)
	ON_WM_MOUSEACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



BOOL CXTPToolBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	return CXTPCommandBar::OnSetCursor(pWnd, nHitTest, message);
}

INT_PTR CXTPCommandBar::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{

	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	// check child windows first by calling CControlBar
	INT_PTR nHit = CWnd::OnToolHitTest(point, pTI);
	if (nHit != -1)
		return nHit;

	if (XTPMouseManager()->IsTrackedLock(this) || XTPMouseManager()->IsMouseLocked())
		return -1;

	if (IsTrackingMode() == TRUE_POPUP && GetPosition() != xtpBarPopup)
		return -1;

	BOOL bToolBarAccelTips = FALSE;
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars)
	{
		if (!pCommandBars->GetCommandBarsOptions()->bToolBarScreenTips)
			return -1;

		bToolBarAccelTips = pCommandBars->GetCommandBarsOptions()->bToolBarAccelTips;
	}

	foreach (pControl, TRUE)
	{
		CRect rect = pControl->GetRect();
		if (rect.PtInRect(point))
		{
			INT_PTR nHit = pControl->GetID();

			CString strTip = pControl->GetTooltip(&point, &rect, &nHit);
			if (strTip.GetLength() == 0)
				return -1;

			if (bToolBarAccelTips)
			{
				CString strShortcut;
				if (XTPShortcutManager()->FindDefaultAccelerator((UINT)nHit, strShortcut, GetCommandBars()))
				{
					if (strShortcut.GetLength() > 0)
					strTip += _T(" (") + strShortcut +_T(")");
				}
			}

		#if _MSC_VER < 1200 // MFC 5.0
			if (pTI != NULL && pTI->cbSize >= sizeof(TOOLINFO))
		#else
			if (pTI != NULL && pTI->cbSize >= sizeof(AFX_OLDTOOLINFO))
		#endif
			{
				pTI->hwnd = m_hWnd;
				pTI->rect = rect;
				pTI->uId = nHit;
				pTI->lpszText = (LPTSTR) ::calloc(strTip.GetLength() + 1, sizeof(TCHAR));
				if( pTI->lpszText != NULL ) STRCPY_S(pTI->lpszText, strTip.GetLength() + 1, (LPCTSTR)strTip);
			}
			// found matching rect, return the ID of the button
			return nHit != 0 ? nHit : -1;
		}
	}
	return -1;
}



void CXTPToolBar::OnRButtonDown(UINT nFlags, CPoint point)
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (!pCommandBars) return;

	if (IsCustomizeMode())
	{
		CXTPCommandBar::OnRButtonDown(nFlags, point);
		return;
	}

	SetTrackingMode(FALSE);
	SetSelected(-1);
	Invalidate(FALSE);

	ClientToScreen(&point);

	pCommandBars->ContextMenu(point);
}


BOOL CXTPToolBar::Create(CWnd* pParentWnd, BOOL bFloat)
{

	if (!IsWindow(m_hWnd))
	{
		DWORD dwStyle = WS_POPUP| MFS_SYNCACTIVE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

		CPoint pt = m_pDockContext? m_pDockContext->m_ptMRUFloatPos: CPoint(100, 100);

		int nLayoutRTL = GetSite()->GetExStyle() & WS_EX_LAYOUTRTL;

		if (!CWnd::CreateEx(nLayoutRTL, AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW)), m_strTitle, dwStyle, CRect(pt, CSize(0, 0)), pParentWnd, 0))
			return FALSE;

		ShowWindow(SW_SHOWNA);

	}
	if (!bFloat)
	{
		ModifyStyle(WS_POPUP, WS_CHILD);
	}
	else
	{
		SetWindowLong(m_hWnd, GWL_HWNDPARENT, 0 );
		ModifyStyle(WS_CHILD, WS_POPUP| MFS_SYNCACTIVE);
		SetWindowLongPtr(m_hWnd, GWL_HWNDPARENT, (LONG_PTR)pParentWnd->m_hWnd );
	}

	return TRUE;
}


CSize CXTPToolBar::CalcDockingLayout(int nLength, DWORD dwMode, int nWidth)
{
	CClientDC dc(this);
	CXTPFontDC font(&dc, GetPaintManager()->GetCommandBarFont(this));
	CXTPControlPopup* pPopupExp = NULL;

	CRect rcBorder = GetPaintManager()->GetCommandBarBorders(this);

	if (m_dwStyle & CBRS_GRIPPER)
		rcBorder.TopLeft() += GetPaintManager()->DrawCommandBarGripper(NULL, this, FALSE);


	CXTPControl* pHide = m_pControls->FindControl(XTP_ID_TOOLBAR_HIDE);
	if (pHide)
	{
		pHide->SetHideFlags(xtpHideGeneric);
	}

	BOOL bShowExpandButtonAlways = TRUE;
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars) bShowExpandButtonAlways = pCommandBars->GetCommandBarsOptions()->bShowExpandButtonAlways;

	BOOL bCommit = TRUE;
	CSize sz = 0;
	if (m_barType != xtpBarTypeMenuBar && m_pDockContext)
	{

		pPopupExp = (CXTPControlPopup* )m_pControls->FindControl(XTP_ID_TOOLBAR_EXPAND);
		if (!pPopupExp)
		{
			pPopupExp = (CXTPControlPopup*)m_pControls->Add(new CXTPControlExpandButton(), XTP_ID_TOOLBAR_EXPAND, _T(""), -1, TRUE);
		}
		pPopupExp->SetHideFlags(xtpHideGeneric);

		if (bShowExpandButtonAlways)
		{
			if (dwMode & LM_HORZDOCK)
				rcBorder.right += 11; else rcBorder.bottom += 11;
		} else
		{
			if (dwMode & LM_COMMIT)
			{
				sz = m_pControls->CalcDynamicSize(&dc, nLength, dwMode, rcBorder, nWidth);
				if (((CXTPControlExpandButton*)pPopupExp)->IsHiddenExists())
				{
					if (dwMode & LM_HORZDOCK)
						rcBorder.right += 11; else rcBorder.bottom += 11;

				}  else
				{
					bCommit = FALSE;
					pPopupExp = NULL;
				}
			} else
			{
				sz = m_pControls->CalcDynamicSize(&dc, nLength, dwMode, rcBorder, nWidth);
				CSize szMin = m_pControls->CalcDynamicSize(&dc, 1, dwMode, rcBorder, nWidth);

				if (m_pControls->GetVisibleCount() > 1)
				{
					if (dwMode & LM_HORZDOCK)
						sz.cx = max(sz.cx, szMin.cx + 11); else
						sz.cy = max(sz.cy, szMin.cy + 11);
				}


				pPopupExp = NULL;
				bCommit = FALSE;
			}

		}
	}

	if (bCommit)
		sz = m_pControls->CalcDynamicSize(&dc, nLength, dwMode, rcBorder, nWidth);

	if (pPopupExp)
	{
		pPopupExp->SetHideFlags(xtpNoHide);
		GetPaintManager()->DrawSpecialControl(0, xtpButtonExpandToolbar, pPopupExp, this, FALSE, &sz);
	}

	return sz;
}

CSize CXTPToolBar::CalcDynamicLayout(int nLength, DWORD nMode)
{
	ASSERT(!(nMode & LM_HORZDOCK || nMode & LM_VERTDOCK));

	CClientDC dc(this);
	CXTPFontDC font(&dc, GetPaintManager()->GetCommandBarFont(this));

	int nTitleSize = GetPaintManager()->DrawCommandBarGripper(&dc, this, FALSE).cy;

	CRect rcBorder(3, 3 + nTitleSize + 1, 3, 3);

	CXTPControlPopup* pPopupExp = NULL;

	CXTPControl* pHide = NULL;

	if (m_barType != xtpBarTypeMenuBar && IsCloseable())
	{
		pHide = m_pControls->FindControl(XTP_ID_TOOLBAR_HIDE);
		if (!pHide)
		{
			pHide = (CXTPControlPopup*)m_pControls->Add(new CXTPControlButtonHide(), XTP_ID_TOOLBAR_HIDE, _T(""), -1, TRUE);
		}

		pHide->SetHideFlags(xtpHideGeneric);
	}

	if (m_barType != xtpBarTypeMenuBar)
	{
		pPopupExp = (CXTPControlPopup* )m_pControls->FindControl(XTP_ID_TOOLBAR_EXPAND);
		if (!pPopupExp)
		{
			pPopupExp = (CXTPControlPopup*)m_pControls->Add(new CXTPControlExpandButton(), XTP_ID_TOOLBAR_EXPAND, _T(""), -1, TRUE);
		}
		pPopupExp->SetHideFlags(xtpHideGeneric);
	}


	CSize sz = m_pControls->CalcDynamicSize(&dc, nLength, nMode, rcBorder);

	int nLeft = sz.cx - 3;
	if (pHide)
	{
		pHide->SetHideFlags(xtpNoHide);
		pPopupExp->SetWrap(TRUE);
		pHide->SetRect(CRect(nLeft - nTitleSize, 3,nLeft, 3 + nTitleSize));
		nLeft -= nTitleSize;
	}

	BOOL bShowExpandButtonAlways = TRUE;
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars) bShowExpandButtonAlways = pCommandBars->GetCommandBarsOptions()->bShowExpandButtonAlways;

	if (pPopupExp && nLeft - nTitleSize > 0 && bShowExpandButtonAlways)
	{
		pPopupExp->SetWrap(TRUE);
		pPopupExp->SetHideFlags(xtpNoHide);
		pPopupExp->SetRect(CRect(nLeft - nTitleSize, 3, nLeft, 3 + nTitleSize));
	}

	return sz;
}

void CXTPToolBar::OnRecalcLayout()
{
	if (m_barPosition != xtpBarFloating)
	{
		CXTPCommandBars* pCommandBars = GetCommandBars();
		if (pCommandBars)
			pCommandBars->RecalcFrameLayout();
	}
	else
	{
		CSize sz = CalcDynamicLayout(-1, LM_MRUWIDTH | LM_HORZ | LM_COMMIT);
		CRect rc;
		GetWindowRect(&rc);
		MoveWindow(rc.left, rc.top, sz.cx, sz.cy);
		Invalidate(FALSE);
	}
}

BOOL CXTPToolBar::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!CXTPCommandBar::OnNcCreate(lpCreateStruct))
		return FALSE;

	if (GetStyle() & MFS_SYNCACTIVE)
	{
		// syncronize activation state with top level parent
		CWnd* pParentWnd = GetTopLevelParent();
		ASSERT(pParentWnd != NULL);
		CWnd* pActiveWnd = GetForegroundWindow();
		BOOL bActive = (pParentWnd == pActiveWnd) ||
			(pParentWnd->GetLastActivePopup() == pActiveWnd &&
			pActiveWnd->SendMessage(WM_FLOATSTATUS, FS_SYNCACTIVE) != 0);

		// the WM_FLOATSTATUS does the actual work
		SendMessage(WM_FLOATSTATUS, bActive ? FS_ACTIVATE : FS_DEACTIVATE);
	}


	return TRUE;
}


LRESULT CXTPToolBar::OnFloatStatus(WPARAM wParam, LPARAM)
{
	// FS_SYNCACTIVE is used to detect MFS_SYNCACTIVE windows
	LRESULT lResult = ((GetStyle() & MFS_SYNCACTIVE) && (wParam & FS_SYNCACTIVE));

	if (m_barPosition != xtpBarFloating || !m_bVisible)
		return lResult;

	BOOL bSyncActivate = TRUE;
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars) bSyncActivate = pCommandBars->GetCommandBarsOptions()->bSyncFloatingBars;

	if (bSyncActivate)
	{
		if ((wParam & FS_DEACTIVATE) && !IsTopParentActive())
		{
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
		}
		if ((wParam & FS_ACTIVATE) && !IsWindowVisible() && IsTopParentActive())
		{
			// check if parent invisible or minimized window
			DWORD dwStyle = ::GetWindowLong(::GetForegroundWindow(), GWL_STYLE);
			if ((dwStyle & WS_VISIBLE) && !(dwStyle & WS_MINIMIZE))
				SetWindowPos(NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

		}
	}

	if (wParam & (FS_SHOW|FS_HIDE))
	{
		SetWindowPos(NULL, 0, 0, 0, 0,
			((wParam & FS_SHOW) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW) | SWP_NOZORDER |
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	}
	if (wParam & (FS_ENABLE|FS_DISABLE))
		EnableWindow((wParam & FS_ENABLE) != 0);

	return lResult;
}

BOOL CXTPToolBar::IsFloatingFrameFocused()
{
	if (!IsTopParentActive())
		return FALSE;

	CWnd* pWnd = CWnd::GetFocus();

	while (pWnd && IsWindow(pWnd->GetSafeHwnd()))
	{
		pWnd = pWnd->GetParentFrame();

		if (DYNAMIC_DOWNCAST(CMiniFrameWnd, pWnd))
			return TRUE;
	}
	return FALSE;
}

int CXTPToolBar::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	if (m_barPosition == xtpBarFloating || IsFloatingFrameFocused())
		return MA_NOACTIVATE;

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

BOOL CXTPToolBar::OnNcActivate(BOOL bActive)
{
	return bActive? FALSE: CWnd::OnNcActivate(bActive);
}

void CXTPToolBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_barPosition == xtpBarFloating)
		SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);


	if (m_pControls->HitTest(point) == NULL)
	{
		XTPMouseManager()->SendTrackLost();

		if (m_pDockContext && ((m_dwFlags & (xtpFlagAlignAny|xtpFlagFloating)) != 0))
		{
			ClientToScreen(&point);
			m_pDockContext->StartDrag(point);
		}
	}
	else
	{
		CXTPCommandBar::OnLButtonDown(nFlags, point);
	}
}

void CXTPToolBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_pControls->HitTest(point) == NULL)
	{
		if (m_pDockContext)
		{
			m_pDockContext->ToggleDocking();
			return;
		}
	}

	CXTPCommandBar::OnLButtonDblClk(nFlags, point);
}

UINT CXTPToolBar::OnNcHitTest(CPoint point)
{
	if (m_barPosition != xtpBarFloating)
		return CXTPCommandBar::OnNcHitTest(point);

	CRect rectWindow;
	GetWindowRect(&rectWindow);
	rectWindow.InflateRect(-3, -3);

	if (point.y < rectWindow.top) return HTTOP;
	if (point.y >= rectWindow.bottom) return HTBOTTOM;
	if (point.x < rectWindow.left) return HTLEFT;
	if (point.x >= rectWindow.right) return HTRIGHT;

	return CXTPCommandBar::OnNcHitTest(point);
}

void CXTPToolBar::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	if (!m_pDockContext)
		return;

	if (nHitTest >= HTSIZEFIRST && nHitTest <= HTSIZELAST)
	{
		ActivateTopParent();
		SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
		XTPMouseManager()->SendTrackLost();
		m_pDockContext->StartResize(nHitTest, point);
		return;
	}

	CXTPCommandBar::OnNcLButtonDown(nHitTest, point);
}



void CXTPToolBar::OnClose()
{
}

void CXTPToolBar::SetVisible(BOOL bVisible)
{
	m_bVisible = bVisible;

	if (!bVisible)
	{
		SetTrackingMode(FALSE);
	}

	ShowWindow(bVisible? SW_SHOWNA: SW_HIDE);

	if (m_barPosition != xtpBarFloating)
	{
		CFrameWnd* pFrame = GetFrameSite();
		if (pFrame) pFrame->RecalcLayout(FALSE);
	}

	if (m_bTearOff && !bVisible)
		GetCommandBars()->Remove(this);
}




LRESULT CXTPToolBar::OnSizeParent(WPARAM, LPARAM lParam)
{
	AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;

	if (GetStyle() & WS_VISIBLE)
	{
		// align the control bar
		CRect rect;
		rect.CopyRect(&lpLayout->rect);

		CSize sizeAvail = rect.Size();  // maximum size available

		// get maximum requested size
		DWORD dwMode = LM_HORZDOCK|LM_HORZ | LM_COMMIT;
		if (GetFlags() & xtpFlagStretched) dwMode |= LM_STRETCH;
		if (GetFlags() & xtpFlagHideWrap) dwMode |= LM_HIDEWRAP;



		if (lpLayout->hDWP == NULL) dwMode &= ~LM_COMMIT;

		CSize size = CalcDockingLayout(sizeAvail.cx, dwMode);

		size.cx = min(size.cx, sizeAvail.cx);
		size.cy = min(size.cy, sizeAvail.cy);

		lpLayout->sizeTotal.cy += size.cy;
		lpLayout->sizeTotal.cx = max(lpLayout->sizeTotal.cx, size.cx);
		lpLayout->rect.top += size.cy;

		rect.right = rect.left + size.cx;
		rect.bottom = rect.top + size.cy;

		// only resize the window if doing layout and not just rect query
		if (lpLayout->hDWP != NULL)
			AfxRepositionWindow(lpLayout, m_hWnd, &rect);
	}
	return 0;
}

CSize CXTPCommandBar::GetIconSize()
{
	if (m_szIcons != CSize(0))
		return m_szIcons;

	CSize sz = CSize(16, 16);

	CXTPCommandBars* pCommandBars = GetCommandBars();

	if (!pCommandBars)
		return sz;

	if (GetType() != xtpBarTypePopup)
	{
		sz = pCommandBars->m_pOptions->szIcons == CSize(0)? sz: pCommandBars->m_pOptions->szIcons;
		if (pCommandBars->m_pOptions->bLargeIcons)
		{
			sz = pCommandBars->m_pOptions->szLargeIcons == CSize(0)? CSize(sz.cx * 2, sz.cy * 2): pCommandBars->m_pOptions->szLargeIcons;
		}
	} else
	{
		return pCommandBars->m_pOptions->szPopupIcons;
	}
	return sz;
}

CSize CXTPCommandBar::GetButtonSize()
{
	if (m_szButtons != CSize(0)) return m_szButtons;

	CSize szIcon = GetIconSize();
	return CSize(szIcon.cx  + 7, szIcon.cy  + 7);
}

void CXTPCommandBar::ShowTextBelowIcons(BOOL bTextBelow) {
	m_bTextBelow = bTextBelow;
	DelayLayout();
}

BOOL CXTPCommandBar::IsTextBelowIcons()
{
	if (m_bTextBelow != XTP_BOOL_DEFAULT)
		return m_bTextBelow;

	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars)
	{
		return pCommandBars->m_pOptions->bShowTextBelowIcons;
	}
	return FALSE;
}


int CXTPToolBar::_FindNearest(CXTPControls* pControls, CXTPControl* pFind, int nPos, BOOL /*bVisible*/)
{
	if (pFind == NULL)
		return -1;

	int nResult = -1;

	for (int i = nPos; i < pControls->GetCount(); i++)
	{
		CXTPControl* pControl = pControls->GetAt(i);

		if (pControl->GetID() == pFind->GetID() && pControl->GetType() == pFind->GetType())
		{
			return i;
		}
	}
	return nResult;
}


void CXTPToolBar::Reset()
{
	if (m_bBuiltIn && m_nBarID != 0 && IsCustomizable())
	{
		CString strMessage, strTitle = GetTitle(), strReset;

		VERIFY(XTPResourceManager()->LoadString(&strReset, XTP_IDS_CONFIRM_RESET));
		strMessage.Format(strReset, strTitle);

		if (AfxMessageBox(strMessage, MB_ICONWARNING|MB_OKCANCEL) == IDOK)
		{

			if (m_pControls->m_pOriginalControls != NULL)
			{
				m_pControls->RemoveAll();

				CXTPControls* pControls = m_pControls->m_pOriginalControls;
				for (int i = 0; i < pControls->GetCount(); i++)
				{
					CXTPControl* pControl = pControls->GetAt(i);
					if (!pControl->m_bTemporary)
						m_pControls->AddClone(pControl, -1, TRUE);
				}

			} else
			{
				LoadToolBar(m_nBarID, FALSE);
			}

			OnInitialUpdate();
			OnRecalcLayout();
		}
	}

}
void CXTPToolBar::BuildCustomizePopup(CXTPCommandBar* pExpandBar)
{

	CXTPControls* pOriginalControls = m_pControls->m_pOriginalControls? m_pControls->m_pOriginalControls->Duplicate(TRUE): new CXTPControls();
	ASSERT(pOriginalControls);

	int nOriginal = 0;
	int nToolBar = 0;

	while(nToolBar < m_pControls->GetCount())
	{
		if ((GetControl(nToolBar)->GetHideFlags() & xtpHideCustomize) != 0)
		{
			int nSelected = m_nSelected;
			int nPopuped = m_nPopuped;
			GetControls()->Remove(nToolBar);
			if (nPopuped != -1) m_nPopuped = nPopuped - 1;
			if (nSelected != -1) m_nSelected = nSelected - 1;
		} else
			nToolBar++;

	}

	nToolBar = 0;

	while (nOriginal < pOriginalControls->GetCount() || nToolBar < m_pControls->GetCount())
	{
		CXTPControl* pOriginal = nOriginal < pOriginalControls->GetCount()? pOriginalControls->GetAt(nOriginal): NULL;
		CXTPControl* pControl = nToolBar < m_pControls->GetCount()? m_pControls->GetAt(nToolBar): NULL;


		int nNearest = _FindNearest(m_pControls, pOriginal, nToolBar, TRUE);
		int nNearestOriginal = _FindNearest(pOriginalControls, pControl, nOriginal);

		if (nNearest == -1 && pOriginal)
		{
			int nSelected = m_nSelected;
			int nPopuped = m_nPopuped;
			pControl = m_pControls->AddClone(pOriginal, nToolBar);
			if (nPopuped != -1) m_nPopuped = nPopuped + 1;
			if (nSelected != -1) m_nSelected = nSelected + 1;
			nNearestOriginal = nToolBar;

			pControl->SetHideFlags(xtpHideCustomize);
			nOriginal++;
			nToolBar++;
		}
		else
		{
			if (nNearest == nToolBar) nOriginal ++;
			else if (nNearestOriginal != -1) pOriginalControls->Remove(nNearestOriginal);
			nToolBar ++;
		}
		ASSERT(pControl);
		if (pControl && (pControl->GetID() == XTP_ID_TOOLBAR_HIDE || pControl->GetID() == XTP_ID_TOOLBAR_EXPAND))
		{
			continue;
		}
		if (pControl && ((pControl->GetHideFlags() & (xtpHideGeneric|xtpHideDocTemplate)) != 0))
		{
			continue;
		}

		CXTPControlButtonCustomize* pButton = (CXTPControlButtonCustomize*)pExpandBar->GetControls()->Add(new CXTPControlButtonCustomize(), 0);
		pButton->SetCaption(pControl->GetCaption());

		XTPControlType controlType = pControl->GetType();
		if ((controlType == xtpControlPopup) || (controlType == xtpControlButtonPopup) || (controlType == xtpControlSplitButtonPopup) || (controlType == xtpControlButton))
			pButton->SetIconId(pControl->GetIconId());

		CString strShortcut;
		XTPShortcutManager()->FindDefaultAccelerator(pControl->GetID(), strShortcut, GetCommandBars());
		pButton->SetShortcutText(strShortcut);
		pButton->SetEnabled(nNearestOriginal != -1);
		pButton->SetChecked((pControl->GetHideFlags() & xtpHideCustomize) == 0);
		pButton->m_pControl = pControl;
	}

	pOriginalControls->InternalRelease();

	if (m_pControls->m_pOriginalControls && !m_bTearOff && m_bBuiltIn)
	{
		CXTPControlButtonCustomize* pButton = (CXTPControlButtonCustomize*)pExpandBar->GetControls()->Add(new CXTPControlButtonCustomize(), 0);

		CString strCaption;
		XTPResourceManager()->LoadString(&strCaption, XTP_IDS_CUSTOMIZE_RESET);

		pButton->SetCaption(strCaption);
		pButton->SetBeginGroup(TRUE);
		pButton->m_bDoubleGripper = TRUE;
		pButton->m_pToolBar = this;
	}
}


void CXTPToolBar::OnSetPreviewMode(BOOL bPreview)
{
	if (m_bPreviewMode == bPreview)
		return;

	m_bPreviewMode = bPreview;

	if (bPreview)
	{
		m_bPreviewVisible = m_bVisible;
		if (m_bVisible)
		{
			m_bVisible = FALSE;
			ShowWindow(SW_HIDE);
		}
	}
	else if (m_bPreviewVisible)
	{
		m_bVisible = TRUE;
		ShowWindow(SW_SHOWNA);

		if (GetType() == xtpBarTypeMenuBar)
		{
			CFrameWnd* pFrameWnd = GetFrameSite();
			if (pFrameWnd && pFrameWnd->GetSafeHwnd()) pFrameWnd->OnUpdateFrameMenu(0);
		}

	}
}

void CXTPToolBar::Copy(CXTPCommandBar* pCommandBar, BOOL bRecursive)
{
	CXTPCommandBar::Copy(pCommandBar, bRecursive);

	m_bBuiltIn = ((CXTPToolBar*)pCommandBar)->m_bBuiltIn;
	m_bTearOff = ((CXTPToolBar*)pCommandBar)->m_bTearOff;
	m_bCloseable = ((CXTPToolBar*)pCommandBar)->m_bCloseable;

}

BOOL CXTPToolBar::OnKeyDown(UINT nChar)
{
	if (m_pDockContext == NULL && nChar == VK_TAB && GetParent())
	{
		CWnd* pWndNext = GetParent()->GetNextDlgTabItem(this,
			(GetKeyState(VK_SHIFT) < 0));

		if (pWndNext != NULL && pWndNext != this)
		{
			pWndNext->SetFocus();
			SetTrackingMode(FALSE);

			return TRUE;
		}
	}
	return CXTPCommandBar::OnKeyDown(nChar);
}

void CXTPToolBar::OnSetFocus(CWnd* pOldWnd)
{
	if (!m_hwndFocus && pOldWnd) m_hwndFocus = pOldWnd->GetSafeHwnd();
	CXTPCommandBar::OnSetFocus(pOldWnd);

	if (m_pDockContext == NULL)
	{
		SetTrackingMode(TRUE, TRUE);
	}

}
