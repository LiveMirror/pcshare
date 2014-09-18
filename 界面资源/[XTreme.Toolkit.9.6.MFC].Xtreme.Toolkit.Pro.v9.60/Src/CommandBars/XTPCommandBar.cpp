// XTPCommandBar.cpp : implementation of the CXTPCommandBar class.
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
#include "afxpriv.h"
#include "XTPDrawHelpers.h"
#include "XTPImageManager.h"

#include "XTPCommandBar.h"
#include "XTPControls.h"
#include "XTPControl.h"
#include "XTPMouseManager.h"
#include "XTPHookManager.h"
#include "XTPPopupBar.h"
#include "XTPControlPopup.h"
#include "XTPPaintManager.h"
#include "XTPShortcutManager.h"
#include "XTPCommandBarsResource.h"
#include "XTPCustomizeTools.h"
#include "XTPToolBar.h"
#include "XTPCommandBars.h"
#include "XTPCustomizeSheet.h"

CXTPCommandBar::LPFNNOTIFYWINEVENT CXTPCommandBar::m_pNotifyWinEvent = 0;
LPFNLRESULTFROMOBJECT CXTPCommandBar::m_pLresultFromObject = 0;



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef CBRS_GRIPPER
#define CBRS_GRIPPER                    0x00400000L
#endif

#ifndef WM_GETOBJECT
#define WM_GETOBJECT 0x003D
#endif

#ifndef EVENT_OBJECT_FOCUS
#define EVENT_OBJECT_FOCUS 0x8005
#endif

#ifndef OBJID_MENU
#define OBJID_MENU 0xFFFFFFFD
#endif

int CXTPCommandBar::s_nHoverDelay = 300;
int CXTPCommandBar::s_nExpandDelay = 800;

//////////////////////////////////////////////////////////////////////////
// CXTPCommandBarList


CXTPCommandBarList::~CXTPCommandBarList()
{
	while (!IsEmpty())
	{
		RemoveHead()->InternalRelease();
	}
}

CXTPCommandBar* CXTPCommandBarList::FindCommandBar(UINT nBarID)
{
	POSITION pos = GetHeadPosition();

	while (pos)
	{
		CXTPCommandBar* pBar = GetNext(pos);
		if (pBar->GetBarID() == nBarID)
			return pBar;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// CXTPCommandBar

IMPLEMENT_XTP_COMMANDBAR(CXTPCommandBar, CWnd)

CXTPCommandBar::CXTPCommandBar()
{
	EnableAutomation();


	m_pCommandBars = 0;
	m_nIdleFlags = 0;
	m_barType = xtpBarTypeNormal;

	m_pControls = (CXTPControls*) CXTPControls::CreateObject();
	m_pControls->SetParent(this);

	m_barPosition = xtpBarNone;
	m_bTracking = FALSE;

	m_nSelected = -1;
	m_nPopuped = -1;
	m_hwndFocus = 0;
	m_pFocused = NULL;
	m_dwFlags = 0;
	m_bVisible = TRUE;
	m_pOwner = NULL;

	m_nBarID = 0;
	m_pSite = 0;

	m_szIcons = 0;
	m_szButtons = 0;

	m_nMRUWidth = 32767;
	m_dwStyle = 0;
	m_bCustomizable = TRUE;

	m_nStateFlags = 0;

	m_rcMarker.SetRectEmpty();
	m_dwData = 0;
	m_pReturnCmd = NULL;

	m_bContextMenuPresent = TRUE;
	m_bTextBelow = XTP_BOOL_DEFAULT;
	m_bExecOnRButton = FALSE;
	m_bIgnoreUpdateHandler = FALSE;

	m_pPaintManager = NULL;
	m_pImageManager = NULL;

	m_bKeyboardSelect = FALSE;

	if (m_pNotifyWinEvent == 0)
	{
		HMODULE hLib = GetModuleHandle(_T("USER32"));
		if (hLib)
		{
			m_pNotifyWinEvent = (LPFNNOTIFYWINEVENT) GetProcAddress(hLib, "NotifyWinEvent");
		}
	}

	m_nLockRedraw = 0;

}

CXTPCommandBar::~CXTPCommandBar()
{

	if (IsTrackingMode()) SetTrackingMode(FALSE);

	XTPHookManager()->RemoveAll(this);
	XTPMouseManager()->RemoveSelected(this);

	if (m_pControls)
	{
		m_pControls->InternalRelease();
	}
	if (m_pPaintManager)
		m_pPaintManager->InternalRelease();

	if (m_pImageManager)
		m_pImageManager->InternalRelease();


}

void CXTPCommandBar::OnFinalRelease()
{
	if (m_hWnd != NULL)
		DestroyWindow();

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CXTPCommandBar, CWnd)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_MESSAGE_VOID(WM_INITIALUPDATE, OnInitialUpdate)
	ON_WM_CREATE()
	ON_WM_DESTROY()

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_MESSAGE(WM_PRINT, OnPrint)

	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)

	ON_MESSAGE(WM_HELPHITTEST, OnHelpHitTest)
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()

	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_NCMOUSEMOVE()

	ON_MESSAGE(WM_GETOBJECT, OnGetObject)

END_MESSAGE_MAP()


// CXTPCommandBar message handlers

int CXTPCommandBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_barPosition != xtpBarListBox)
	{
		CXTPCommandBars* pCommandBars = GetCommandBars() ;
		if (pCommandBars)
		{
			pCommandBars->GetDropSource()->Register(this);
		}
	}

	if (m_barType != xtpBarTypePopup)
	{
		EnableToolTips();


	}

	return 0;
}
void CXTPCommandBar::OnDestroy()
{
	if (m_barPosition != xtpBarListBox)
	{
		CXTPCommandBars* pCommandBars = GetCommandBars();
		if (pCommandBars)
		{
			pCommandBars->GetDropSource()->UnRegister(this);
		}
	}

	if (IsTrackingMode()) SetTrackingMode(FALSE);

	CWnd::OnDestroy();
}

void CXTPCommandBar::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
	BOOL bActive = IsTrackingMode() || XTPMouseManager()->IsTopParentActive(m_hWnd);

	if (!m_hWnd || XTPMouseManager()->IsTrackedLock(this) ||
		(!bActive && GetPosition() != xtpBarPopup) || XTPMouseManager()->IsMouseLocked())
		return;

	CXTPCommandBars* pCommandBars = GetCommandBars ();
	if (pCommandBars && pCommandBars->IsCustomizeMode())
	{
		CXTPControl* pControl = pCommandBars->m_pDragSelected;
		if (pControl)
			pControl->OnCustomizeMouseMove(point);
		return;
	}


	BOOL bLockSelected = ((GetPosition() != xtpBarPopup) || m_nPopuped != -1) || m_bKeyboardSelect;
	int nSelected = (IsTrackingMode() && bLockSelected )? m_nSelected: -1;

	foreach (pControl, TRUE)
	{
		if (pControl->GetRect().PtInRect(point))
		{
			nSelected = pControl->GetIndex();
			XTPMouseManager()->TrackMouseLeave(*this);
			pControl->OnMouseMove(point);
		}
	}

	SetSelected(nSelected);
}

void CXTPCommandBar::OnMouseLeave()
{
	if (!XTPMouseManager()->IsMouseLocked() && !(IsTrackingMode() && (GetPosition() != xtpBarPopup || m_nPopuped != -1)))
		SetSelected(-1);
}
void CXTPCommandBar::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	OnMouseLeave();
	CWnd::OnNcMouseMove(nHitTest, point);
}

void CXTPCommandBar::OnTrackEnter()
{
	SetSelected(-1);
}



void CXTPCommandBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	CXTPControl* pControl = m_pControls->HitTest(point);
	if (pControl)
	{

		if (!IsCustomizeMode() && m_bCustomizable && ((pControl->GetFlags() & xtpFlagNoMovable) == 0)  &&
			DYNAMIC_DOWNCAST(CXTPToolBar, this) && (GetKeyState(VK_MENU) < 0))
		{
			CXTPCommandBars* pCommandBars = GetCommandBars();
			if (pCommandBars && pCommandBars->IsCustomizeAvail() && pCommandBars->GetCommandBarsOptions()->bAltDragCustomization)
			{
				pCommandBars->SetQuickCustomizeMode(TRUE);

				SetPopuped(-1);
				SetSelected(-1);
				pControl->CustomizeStartDrag(point);

				pCommandBars->SetQuickCustomizeMode(FALSE);
				pCommandBars->RecalcFrameLayout();
				XTPMouseManager()->SendTrackLost();
				return;
			}
		}


		if (!(IsCustomizeMode() && (!m_bCustomizable || pControl->GetFlags() & xtpFlagNoMovable)))
			pControl->OnClick(FALSE, point);
		return;
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CXTPCommandBar::OnLButtonDblClk(UINT /*nFlags*/, CPoint point)
{
	if (IsCustomizeMode())
		return;

	CXTPControl* pControl = m_pControls->HitTest(point);
	if (pControl)
	{
		pControl->OnLButtonDblClk(point);
	}
}

void CXTPCommandBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (IsCustomizeMode())
		return;

	CXTPControl* pControl = m_pControls->HitTest(point);
	if (pControl)
	{
		pControl->OnLButtonUp(point);
		return;
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CXTPCommandBar::OnRButtonUp(UINT nFlags, CPoint point)
{
	CXTPControl* pControl = m_pControls->HitTest(point);

	if (pControl && !IsCustomizeMode())
	{
		if (GetRootParent()->m_bExecOnRButton)
		{
			pControl->OnLButtonUp(point);
		}
		else
		{
			pControl->OnRButtonUp(point);
		}
		return;
	}
	CWnd::OnRButtonUp(nFlags, point);
}


BOOL CXTPCommandBar::SetSelected(int nSelected, BOOL bKeyboard)
{
	if (m_nSelected == nSelected)
		return FALSE;

	m_bKeyboardSelect = bKeyboard;

	if (!GetSafeHwnd())
		return FALSE;

	LockRedraw();

	if (m_nSelected != -1)
		GetControl(m_nSelected)->OnSetSelected(FALSE);

	if ((m_nSelected = nSelected) != -1)
	{
		if (bKeyboard) NotifyFocusEvent(m_nSelected);
		GetControl(nSelected)->OnSetSelected(bKeyboard? TRUE_KEYBOARD: TRUE);
		XTPMouseManager()->SetSelected(this);
		if (!bKeyboard) SetTimer(XTP_TID_HOVER, s_nHoverDelay, NULL);
	}
	else
	{
		KillTimer(XTP_TID_HOVER);
	}

	UnlockRedraw();

	return TRUE;
}


void CXTPCommandBar::NotifyFocusEvent(int nSelected)
{
	if (m_pNotifyWinEvent != NULL)
	{
		m_pNotifyWinEvent(EVENT_OBJECT_FOCUS , m_hWnd,  OBJID_MENU , nSelected + 1);
	}
}

BOOL CXTPCommandBar::SetPopuped(int nPopuped, BOOL bKeyboard)
{
	if (m_nPopuped == nPopuped)
		return FALSE;

	LockRedraw();

	if (m_nPopuped != -1)
	{
		CXTPControl* pControl = GetControl(m_nPopuped);
		m_nPopuped = -1;
		pControl->OnSetPopup(FALSE);
	}

	if (nPopuped != -1)
	{
		CXTPControlPopup* pControlPopup = DYNAMIC_DOWNCAST(CXTPControlPopup, GetControl(nPopuped));
		if (!pControlPopup || !pControlPopup->GetEnabled())
		{
			UnlockRedraw();
			return FALSE;
		}

		m_nPopuped = nPopuped;
		SetTrackingMode(TRUE_POPUP, FALSE, bKeyboard);
		SetSelected(nPopuped, bKeyboard);

		VERIFY(((CXTPControl*)pControlPopup)->OnSetPopup(TRUE));

		CXTPCommandBars* pCommandBars = GetCommandBars();
		if (pCommandBars && pCommandBars->GetCommandBarsOptions()->bShowFullAfterDelay)
		{
			SetTimer(XTP_TID_EXPANDED_POPUP, s_nExpandDelay, NULL);
		}

	}

	UnlockRedraw();
	return TRUE;
}


LRESULT CXTPCommandBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM)
{
	if (GetSafeHwnd() && (GetStyle() & WS_VISIBLE))
	{
		CWnd* pTarget = GetOwnerSite();

		UpdateDocTemplateControls();

		if (pTarget != NULL)
			OnUpdateCmdUI(pTarget, (BOOL)wParam);
	}
	return 0L;
}

class CXTPCommandBar::CXTPCommandBarCmdUI : public CCmdUI
{
public: // re-implementations only
	virtual void Enable(BOOL bOn);
	virtual void SetCheck(int nCheck);
	virtual void SetText(LPCTSTR lpszText);
};

void CXTPCommandBar::CXTPCommandBarCmdUI::Enable(BOOL bOn)
{
	m_bEnableChanged = TRUE;

	CXTPControl* pControl = CXTPControl::FromUI(this);
	ASSERT(pControl != NULL);

	pControl->SetEnabled(bOn);
}

void CXTPCommandBar::CXTPCommandBarCmdUI::SetCheck(int nCheck)
{
	ASSERT(nCheck >= 0 && nCheck <= 2); // 0=>off, 1=>on, 2=>indeterminate

	CXTPControl* pControl = CXTPControl::FromUI(this);
	ASSERT(pControl != NULL);

	pControl->SetChecked(nCheck);
}

void CXTPCommandBar::CXTPCommandBarCmdUI::SetText(LPCTSTR lpszCaption)
{
	CXTPControl* pControl = CXTPControl::FromUI(this);
	ASSERT(pControl != NULL);

	pControl->SetCaption(lpszCaption);
}


void CXTPCommandBar::OnInitialUpdate()
{
	// update the indicators before becoming visible
	OnIdleUpdateCmdUI(TRUE, 0L);
}

void CXTPCommandBar::OnUpdateCmdUI(CWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CXTPCommandBars* pCommandBars = GetCommandBars();


	BOOL bIgnoreUpdateHandler = GetRootParent()->m_bIgnoreUpdateHandler;

	CXTPCommandBarCmdUI state;
	state.m_pOther = this;

	for (state.m_nIndex = 0; (int)state.m_nIndex < m_pControls->GetCount(); state.m_nIndex++)
	{
		CXTPControl* pControl = m_pControls->GetAt(state.m_nIndex);

		if (pControl->m_bPressed)
		{
			pControl->m_bPressed = FALSE;
			DelayRedraw();
		}

		if (bIgnoreUpdateHandler || (pControl->m_dwFlags & xtpFlagManualUpdate))
			continue;

		state.m_nID = pControl->GetID();

		if (CWnd::OnCmdMsg(0,
			MAKELONG((int)CN_UPDATE_COMMAND_UI, WM_COMMAND+WM_REFLECT_BASE),
			&state, NULL))
			continue;

		// allow the toolbar itself to have update handlers
		if (CWnd::OnCmdMsg(state.m_nID, (int)CN_UPDATE_COMMAND_UI, &state, NULL))
			continue;

		if (pCommandBars && pCommandBars->IsCustomizeMode()) state.Enable(TRUE);
		else
		// allow the owner to process the update
			state.DoUpdate(pTarget, bDisableIfNoHndler);
	}
	if (m_nIdleFlags & xtpIdleRedraw)
	{
		Invalidate(FALSE);
		m_nIdleFlags &= ~xtpIdleRedraw;
	}
	if (m_nIdleFlags & xtpIdleLayout)
	{
		OnRecalcLayout();
		m_nIdleFlags &= ~xtpIdleLayout;
	}

	// update the dialog controls added to the toolbar
	UpdateDialogControls(pTarget, bDisableIfNoHndler);

}

BOOL CXTPControls::LoadMenu(CMenu* pMenu)
{
	BOOL bSeparator = FALSE;
	for (int i = 0; i < ::GetMenuItemCount(pMenu->m_hMenu); i++)
	{
		// Check to see if the item is a separator, we don't want
		// to use GetMenuItemID(i) because it may not equal zero.

		MENUITEMINFO info = { sizeof(MENUITEMINFO), MIIM_TYPE|MIIM_STATE};
		::GetMenuItemInfo(pMenu->m_hMenu, i, TRUE, &info);

		if (((info.fType & MFT_SEPARATOR) == MFT_SEPARATOR) || (pMenu->GetMenuItemID(i) == 0))
			bSeparator = TRUE;
		else
		{
			CXTPControl* pControl = AddMenuItem(pMenu, i);

			if (!pControl)
				continue;

			if (bSeparator)
			{
				pControl->SetBeginGroup(TRUE);
				bSeparator = FALSE;
			}
			if (info.fState & MFS_CHECKED)
			{
				pControl->SetChecked(TRUE);
			}
			if (info.fState & MFS_DISABLED)
			{
				pControl->SetEnabled(FALSE);
			}
		}
	}
	return TRUE;
}

BOOL CXTPCommandBar::LoadMenu(CMenu* pMenu, BOOL bRemoveControls)
{
	if (!pMenu || !::IsMenu(pMenu->m_hMenu))
		return FALSE;

	if (bRemoveControls)
		m_pControls->RemoveAll();

	if (!m_pControls->LoadMenu(pMenu))
		return FALSE;

	CFrameWnd* pFrame = GetParentFrame();
	if (pFrame) pFrame->DelayRecalcLayout();
	return TRUE;
}





BOOL CXTPCommandBar::ProcessSpecialKey(XTPSpecialKey key)
{
	switch (key)
	{
		case xtpKeyNext:
			SetSelected(m_pControls->GetNext(m_nSelected, +1), TRUE);
			return TRUE;

		case xtpKeyPrev:
			SetSelected(m_pControls->GetNext(m_nSelected, -1), TRUE);
			return TRUE;

		case xtpKeyHome:
			SetSelected(m_pControls->GetNext(-1, +1), TRUE);
			return TRUE;

		case xtpKeyEnd:
			SetSelected(m_pControls->GetNext(m_pControls->GetCount(), -1), TRUE);
			return TRUE;

		case xtpKeyPopup:
			if (m_nSelected != -1 && m_nSelected != m_nPopuped)
			{
				GetControl(m_nSelected)->OnSetSelected(TRUE_KEYBOARD);
				return SetPopuped(m_nSelected);
			}
			break;

		case xtpKeyBack:
			if (m_nPopuped != -1)
			{
				SetPopuped(-1);
				SetTrackingMode(TRUE, FALSE);
				return TRUE;
			}
			break;

		case xtpKeyEscape:

			if (GetParentCommandBar() != NULL)
				GetParentCommandBar()->SetTrackingMode(TRUE, FALSE);

			SetTrackingMode(FALSE);
			return TRUE;

		case xtpKeyReturn:
			if (m_nSelected != -1)
			{
				GetControl(m_nSelected)->OnSetSelected(TRUE_KEYBOARD);
				GetControl(m_nSelected)->OnClick(TRUE);
			}
			return TRUE;

	}
	return FALSE;
}


BOOL CXTPCommandBar::OnKeyDown(UINT nChar)
{
	if (IsCustomizeMode())
	{
		return FALSE;
	}

	switch (nChar)
	{
		case VK_TAB: return ProcessSpecialKey(GetKeyState(VK_SHIFT) >= 0? xtpKeyNext: xtpKeyPrev);
		case VK_RETURN: return ProcessSpecialKey(xtpKeyReturn);
	}

	if (m_pFocused)
	{
		BOOL bResult = m_pFocused->OnKeyDown(nChar);

		if ((bResult == TRUE) || (bResult == FALSE_EXIT))
			return bResult;
	}

	BOOL bHoriz = m_barPosition == xtpBarTop || m_barPosition == xtpBarBottom || m_barPosition == xtpBarFloating;
	BOOL bLayoutRTL = IsLayoutRTL();

	switch (nChar)
	{
		case VK_ESCAPE: return ProcessSpecialKey(xtpKeyEscape);
		case VK_LEFT: return ProcessSpecialKey(bHoriz? (bLayoutRTL? xtpKeyNext: xtpKeyPrev): (bLayoutRTL ? xtpKeyPopup: xtpKeyBack));
		case VK_RIGHT: return ProcessSpecialKey(bHoriz? (bLayoutRTL? xtpKeyPrev: xtpKeyNext): (!bLayoutRTL? xtpKeyPopup : xtpKeyBack));
		case VK_UP: return ProcessSpecialKey(bHoriz? xtpKeyBack: xtpKeyPrev);
		case VK_DOWN: return ProcessSpecialKey(bHoriz? xtpKeyPopup: xtpKeyNext);
		case VK_HOME: return ProcessSpecialKey(xtpKeyHome);
		case VK_END: return ProcessSpecialKey(xtpKeyEnd);
	}

	nChar = MapVirtualKey(nChar, 2);

	TCHAR chAccel = XTPShortcutManager()->ToUpper((TCHAR)nChar);
	if( chAccel)
	{
		BOOL bSelectOnly = FALSE;
		int nIndex = GetKeyState(VK_CONTROL) < 0? -1: FindAccelEx(chAccel, bSelectOnly);
		if (nIndex != -1)
		{
			SetSelected(nIndex, TRUE);

			if (!bSelectOnly)
			{
				GetControl(nIndex)->OnClick(TRUE);
			}
			return TRUE;
		}
		if (nChar != VK_SHIFT && nChar != VK_CONTROL) MessageBeep(0);
	}

	return TRUE;
}

BOOL CXTPCommandBar::Popup(CXTPControlPopup* /*pControlPopup*/, BOOL)
{
	return FALSE;
}

BOOL CXTPCommandBar::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

int CXTPCommandBar::OnHookMessage(HWND /*hWnd*/, UINT nMessage, WPARAM& wParam, LPARAM& /*lParam*/, LRESULT& /*lResult*/)
{
	if (IsTrackingMode())
	{
		switch (nMessage)
		{
			case WM_KEYDOWN:
				return OnKeyDown((UINT)wParam);
		}
	}
	return FALSE;
}

void CXTPCommandBar::OnTrackLost()
{
	SetTrackingMode(FALSE);
}

BOOL CXTPCommandBar::SetTrackingMode(int bMode, BOOL bSelectFirst, BOOL bKeyboard)
{
	if (m_bTracking && bMode || m_bTracking == bMode)
	{
		m_bTracking = bMode;
		return FALSE;
	}

	m_bTracking = bMode;

	if (bMode)
	{
		if (GetType() == xtpBarTypeMenuBar)
			GetSite()->SendMessage(WM_XTP_INITMENU, (WPARAM)this);

		m_bAnimatePopup = !bSelectFirst && !bKeyboard;

		if (m_hwndFocus == 0)
			m_hwndFocus = ::GetFocus()? ::GetFocus(): GetSite()->GetSafeHwnd();
		::HideCaret(m_hwndFocus);

		XTPMouseManager()->SetTrack(this, FALSE);

		XTPKeyboardManager()->HookKeyboard(this);

		::SetCursor(::LoadCursor(NULL, IDC_ARROW));

		if (bSelectFirst)
		{
			SetSelected(m_pControls->GetNext(-1, +1), TRUE);
		}

	} else
	{
		m_bAnimatePopup = FALSE;

		XTPMouseManager()->RemoveTrack(this);

		XTPKeyboardManager()->UnhookKeyboard(this);

		SetPopuped(-1);
		if (bSelectFirst) SetSelected(-1);

		if (m_hwndFocus)
		{
			::ShowCaret(m_hwndFocus);
			m_hwndFocus = 0;
		}

		Redraw();

		if (!XTPMouseManager()->IsTrackedLock())
			XTPMouseManager()->SetForceExpanded(FALSE);

		if (m_hWnd && GetFocus() == this && GetParent())
		{
			CWnd* pWndNext = GetParent()->GetNextDlgTabItem(this);
			if (pWndNext != NULL)
				pWndNext->SetFocus();
		}

	}

	return TRUE;
}



CXTPControl* CXTPCommandBar::GetControl(int nIndex)
{
	//Ensure the index falls within the correct range.
	if ((nIndex >= 0) && (nIndex < m_pControls->GetCount()))
	{
		return m_pControls->GetAt(nIndex);
	}
	return NULL;
}

void CXTPCommandBar::OnTimer(UINT_PTR nIDEvent)
{
	CWnd::OnTimer(nIDEvent);

	if (nIDEvent == XTP_TID_HOVER )
	{
		KillTimer(XTP_TID_HOVER);

		if (m_nSelected != -1)
			GetControl(m_nSelected)->OnMouseHover();
	}
	if (nIDEvent == XTP_TID_EXPANDED_POPUP )
	{
		KillTimer(XTP_TID_EXPANDED_POPUP);

		if (m_nPopuped != -1 && m_nSelected == m_nPopuped)
		{
			CXTPControlPopup* pControl = DYNAMIC_DOWNCAST(CXTPControlPopup, GetControl(m_nSelected));
			if (pControl && pControl->IsCursorOver())
			{
				pControl->ExpandCommandBar();
			}
		}
	}
}

void CXTPCommandBar::OnControlsChanged()
{
	m_nSelected = -1;
	m_nPopuped = -1;
	DelayLayout();
}

void CXTPCommandBar::OnPaint()
{
	CPaintDC paintDC(this);
	CXTPBufferDC dc(paintDC, CXTPClientRect(this));
	DrawCommandBar(&dc);
}

LRESULT CXTPCommandBar::OnPrint(WPARAM wParam, LPARAM)
{
	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (pDC)
	{
		DrawCommandBar(pDC);
	}
	return TRUE;
}

void CXTPCommandBar::DrawCommandBar(CDC* pDC)
{
	CXTPPaintManager* pPaintManager = GetPaintManager();
	CXTPCommandBars* pCommandBars = GetCommandBars();

	pPaintManager->FillCommandBarEntry(pDC, this);

	if (m_dwStyle & CBRS_GRIPPER)
		pPaintManager->DrawCommandBarGripper(pDC, this);

	BOOL bFirst = TRUE;
	foreach (pControl, TRUE)
	{
		if (pControl->GetBeginGroup() && !bFirst)
		{
			pPaintManager->DrawCommandBarSeparator(pDC, this, pControl);
		}
		bFirst = FALSE;

		CXTPFontDC font(pDC, pPaintManager->GetCommandBarFont(this, pControl->IsItemDefault()));

		pControl->Draw(pDC);

		if (pCommandBars && pCommandBars->IsCustomizeMode() && (pCommandBars->m_pDragSelected == pControl))
		{
			CRect rc = pControl->GetRect();
			pDC->Draw3dRect(rc, 0, 0);
			rc.DeflateRect(1, 1);
			pDC->Draw3dRect(rc, 0, 0);
		}
	}

	if (pCommandBars && pCommandBars->IsCustomizeMode() && !m_rcMarker.IsRectEmpty())
	{
		CustomizeDrawMarker(pDC);
	}
}

void CXTPCommandBar::CustomizeDrawMarker(CDC* pDC)
{

	CBrush brush(GetXtremeColor(COLOR_3DFACE));
	CBrush* pBrush = (CBrush*)pDC->SelectObject(&brush);

	if (m_bVerticalMarker)
	{
		pDC->PatBlt(m_rcMarker.left, m_rcMarker.top, 1, m_rcMarker.Height(), PATINVERT);
		pDC->PatBlt(m_rcMarker.left + 1, m_rcMarker.top + 1, 1, m_rcMarker.Height() - 2, PATINVERT);
		pDC->PatBlt(m_rcMarker.left + 2, m_rcMarker.top + 2, m_rcMarker.Width() - 4, 2, PATINVERT);
		pDC->PatBlt(m_rcMarker.right - 1, m_rcMarker.top, 1, m_rcMarker.Height(), PATINVERT);
		pDC->PatBlt(m_rcMarker.right - 2, m_rcMarker.top + 1, 1, m_rcMarker.Height() - 2, PATINVERT);

	} else
	{
		pDC->PatBlt(m_rcMarker.left, m_rcMarker.top, m_rcMarker.Width(), 1, PATINVERT);
		pDC->PatBlt(m_rcMarker.left + 1, m_rcMarker.top + 1, m_rcMarker.Width() - 2, 1, PATINVERT);
		pDC->PatBlt(m_rcMarker.left + 2, m_rcMarker.top + 2, 2, m_rcMarker.Height() - 4, PATINVERT);
		pDC->PatBlt(m_rcMarker.left, m_rcMarker.bottom - 1, m_rcMarker.Width(), 1, PATINVERT);
		pDC->PatBlt(m_rcMarker.left + 1, m_rcMarker.bottom - 2, m_rcMarker.Width() - 2, 1, PATINVERT);
	}

	pDC->SelectObject(pBrush);
}


int CXTPCommandBar::FindAccel(TCHAR chAccel)
{
	foreach (pControl, TRUE)
	{
		CString strCaption = pControl->GetCaption();
		int nAmdIndex = strCaption.Find(_T('&'));
		if( nAmdIndex == -1 || nAmdIndex ==  strCaption.GetLength() - 1 )
			continue;
		TCHAR ch = XTPShortcutManager()->ToUpper(strCaption[nAmdIndex+1]);
		if( chAccel == ch )
		{
			return pControl->GetIndex();
		}
	}
	return -1;
}


int CXTPCommandBar::FindAccelEx(TCHAR chAccel, BOOL& bSelectOnly)
{
	int nItem = m_pControls->GetNext(m_nSelected, +1);
	if (nItem == -1)
		return -1;

	bSelectOnly = FALSE;

	int nStart = nItem;

	int nAmpCondidate = -1, nCondidate = -1;

	do
	{
		CXTPControl* pControl = GetControl(nItem);
		ASSERT(pControl);

		CString strCaption = pControl->GetCaption();
		if (!strCaption.IsEmpty())
		{
			int nAmpIndex = strCaption.Find(_T('&'));

			if( nAmpIndex > -1 &&  nAmpIndex < strCaption.GetLength() - 1 )
			{
				TCHAR ch = XTPShortcutManager()->ToUpper(strCaption[nAmpIndex+1]);
				if( chAccel == ch )
				{
					if (nAmpCondidate == -1)
						nAmpCondidate = pControl->GetIndex();
					else
					{
						bSelectOnly = TRUE;
						return nAmpCondidate;
					}
				}
			} else
			{
				TCHAR ch = XTPShortcutManager()->ToUpper(strCaption[0]);
				if( chAccel == ch )
				{
					if (nCondidate == -1)
						nCondidate = pControl->GetIndex();
					else
					{
						bSelectOnly = TRUE;
					}
				}
			}
		}
		nItem = m_pControls->GetNext(nItem, +1, TRUE, FALSE, FALSE);
	} while (nItem != nStart);

	return nAmpCondidate != -1? nAmpCondidate: nCondidate;
}



void CXTPCommandBar::SetFocusedControl(CXTPControl* pControl)
{
	ASSERT(pControl == NULL || pControl->m_pParent == this);
	m_pFocused = pControl;
}


CFrameWnd* CXTPCommandBar::GetFrameSite()
{
	CXTPCommandBars* pCommandBars = GetCommandBars();

	return pCommandBars? DYNAMIC_DOWNCAST(CFrameWnd, pCommandBars->GetSite()): 0;
}

CWnd* CXTPCommandBar::GetSite()
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars)
		return pCommandBars->GetSite();

	CXTPCommandBar* pRoot = GetRootParent();

	if (pRoot->m_pSite) return pRoot->m_pSite;
	if (pRoot->m_hWnd) return pRoot->GetOwner();

	return AfxGetMainWnd();
}

CWnd* CXTPCommandBar::GetOwnerSite()
{
	CXTPCommandBar* pRoot = GetRootParent();

	if (pRoot->m_pOwner)
		return pRoot->m_pOwner;

	return pRoot->GetSite();
}

CXTPCommandBar* CXTPCommandBar::GetRootParent()
{
	CXTPCommandBar* pRoot = this;
	while (TRUE)
	{
		CXTPCommandBar* pCommandBar = pRoot->GetParentCommandBar();
		if (pCommandBar == NULL)
			return pRoot;
		pRoot = pCommandBar;
	}
	return NULL;
}

CXTPCommandBars* CXTPCommandBar::GetCommandBars() const
{
	const CXTPCommandBar* pCommandBar = this;
	do
	{
		if (pCommandBar->m_pCommandBars)
			return pCommandBar->m_pCommandBars;

		pCommandBar = pCommandBar->GetParentCommandBar();

	} while (pCommandBar != NULL);

	return NULL;
}

BOOL CXTPCommandBar::IsCustomizeMode()
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	return pCommandBars? pCommandBars->IsCustomizeMode(): FALSE;
}


DROPEFFECT CXTPCommandBar::OnCustomizeDragEnter(CXTPControl* pDataObject, CPoint point)
{
	m_rcMarker.SetRectEmpty();

	return OnCustomizeDragOver(pDataObject, point);
}

void CXTPCommandBar::OnCustomizeDragLeave()
{
	if (!m_rcMarker.IsRectEmpty())
	{
		m_rcMarker.SetRectEmpty();
		if (GetSafeHwnd()) Invalidate(FALSE);
	}
}

void CXTPCommandBar::CustomizeFindDropIndex(CXTPControl* pDataObject, const CPoint& point,
	CRect& rcMarker, int& nDropIndex, BOOL& bDropAfter)
{

	CXTPControl* pControlMarker = NULL;
	int nOffset = -1;
	int nCount = m_pControls->GetCount();

	BOOL bVert =  (m_barPosition == xtpBarLeft || m_barPosition == xtpBarRight || m_barType == xtpBarTypePopup);

	for (int i = 0; i < nCount; i++)
	{
		CXTPControl* pControl = GetControl(i);

		if (!pControl->IsVisible() || pControl->m_dwFlags & xtpFlagNoMovable)
			continue;

		CRect rc = pControl->GetRect();

		if (pControl->GetWrap())
		{
			nOffset = -1;
			if (!bVert && rc.top > point.y) break;
			else if (bVert && rc.right < point.x) break;
		}

		if (bVert)
		{
			if (nOffset == -1 || nOffset > abs(rc.CenterPoint().y - point.y))
			{
				nOffset = abs(rc.CenterPoint().y - point.y);
				pControlMarker = pControl;

				bDropAfter = rc.CenterPoint().y < point.y;

				if (bDropAfter)
				{
					if (i >= nCount - 1 || GetControl(i + 1)->GetBeginGroup())
						rcMarker.SetRect(rc.left, rc.bottom - 6, rc.right, rc.bottom );
					else
						rcMarker.SetRect(rc.left, rc.bottom, rc.right, rc.bottom + 6);
				} else
				{
					rcMarker.SetRect(rc.left, rc.top, rc.right, rc.top + 6);

				}

			}

		}
		else
		{
			if (nOffset == -1 || nOffset > abs(rc.CenterPoint().x - point.x))
			{
				nOffset = abs(rc.CenterPoint().x - point.x);
				pControlMarker = pControl;

				bDropAfter = rc.CenterPoint().x < point.x;

				if (bDropAfter)
				{
					if (i >= nCount - 1 || GetControl(i + 1)->GetBeginGroup() || GetControl(i + 1)->GetWrap())
						rcMarker.SetRect(rc.right - 6, rc.top, rc.right, rc.bottom);
					else
						rcMarker.SetRect(rc.right, rc.top, rc.right + 6, rc.bottom);
				} else
				{
					rcMarker.SetRect(rc.left, rc.top, rc.left + 6, rc.bottom);
				}
			}
		}
		m_bVerticalMarker = bVert;
	}

	if (pControlMarker == NULL)
	{
		CRect rc;
		GetClientRect(&rc);
		rcMarker.SetRect(rc.left + 2, rc.top + 2, rc.left + 8, rc.bottom - 2);
		bDropAfter = TRUE;
		nDropIndex = -1;
		m_bVerticalMarker = FALSE;
	}
	else
	{
		nDropIndex = pControlMarker->GetIndex();
	}

	if (pDataObject == pControlMarker)
		rcMarker.SetRectEmpty();
}


DROPEFFECT CXTPCommandBar::OnCustomizeDragOver(CXTPControl* pDataObject, CPoint point)
{
	if (!IsCustomizable() || !(GetStyle() & WS_VISIBLE))
		return DROPEFFECT_NONE;

	ASSERT(pDataObject);

	CRect rcMarker;
	int nDropIndex;
	BOOL bDropAfter;
	CustomizeFindDropIndex (pDataObject, point, rcMarker, nDropIndex, bDropAfter);

	DROPEFFECT dropEffect = (DROPEFFECT)-1;

	CXTPControl* pControl = m_pControls->HitTest(point);
	if (pControl)
	{
		if (GetParentCommandBar() == NULL && m_nPopuped != pControl->GetIndex())
		{
			XTPMouseManager()->SendTrackLost();
		}

		pControl->OnCustomizeDragOver(pDataObject, point, dropEffect);
	}

	if (!pDataObject->IsCustomizeDragOverAvail(this, point, dropEffect))
		return dropEffect;

	m_rcMarker = rcMarker;

	Invalidate(FALSE);

	return dropEffect;
}

void CXTPCommandBar::OnCustomizeDrop(CXTPControl* pDataObject, DROPEFFECT& dropEffect, CPoint ptDrop, CPoint ptDrag)
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	ASSERT(pCommandBars);

	CRect rcMarker;
	int nDropIndex;
	BOOL bDropAfter;

	CustomizeFindDropIndex (pDataObject, ptDrop, rcMarker, nDropIndex, bDropAfter);

	if (nDropIndex == -1 || pDataObject != GetControl(nDropIndex))
	{
		if (bDropAfter)  nDropIndex++;

		SetPopuped(-1);
		SetSelected(-1);

		BOOL bBeginGroup = FALSE;
		if (bDropAfter == FALSE && nDropIndex >= 0 && nDropIndex < m_pControls->GetCount() )
		{
			bBeginGroup = GetControl(nDropIndex)->GetBeginGroup();
			if (bBeginGroup)
			{
				GetControl(nDropIndex)->SetBeginGroup(FALSE);
			}
		}

		CXTPControl* pClone = m_pControls->AddClone(pDataObject, nDropIndex);
		pClone->SetBeginGroup(bBeginGroup);
		pCommandBars->SetDragControl(pClone);

	}
	else
	{
		if (abs(ptDrag.x - ptDrop.x) > 4 || abs(ptDrag.y - ptDrop.y) > 4)
		{
			CXTPControl* pControl = GetControl(nDropIndex);
			ASSERT(pDataObject == pControl);
			pControl->SetBeginGroup(bDropAfter && nDropIndex != 0);
		}

		dropEffect = DROPEFFECT_CANCEL;
	}
	OnRecalcLayout();

	m_rcMarker.SetRectEmpty();
	Invalidate(FALSE);
}

void CXTPCommandBar::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (IsCustomizeMode() && m_bCustomizable)
	{
		CXTPControl* pControl = m_pControls->HitTest(point);
		if (pControl && pControl->IsVisible() && !(pControl->m_dwFlags & xtpFlagNoMovable))
		{

			SetPopuped(-1);
			SetSelected(-1);

			CXTPCommandBars* pCommandBars = GetCommandBars();
			ASSERT(pCommandBars);
			CXTPCustomizeDropSource* pDropSource = pCommandBars->GetDropSource();
			ASSERT(pDropSource);

			pCommandBars->SetDragControl(pControl);

			if (pDropSource->GetSheet())
			{
				ClientToScreen(&point);
				pDropSource->GetSheet()->ContextMenu(point);
			}
		}
		return;
	}

	CWnd::OnRButtonDown(nFlags, point);
}

void CXTPCommandBar::InvalidateParents()
{
	Invalidate(FALSE);
	CXTPCommandBar* pCommandBar = GetParentCommandBar();
	if (pCommandBar) pCommandBar->InvalidateParents();
}

void CXTPCommandBar::OnControlSelected(BOOL bSelected, CXTPControl* pControl)
{
	SetStatusText(bSelected? pControl->GetDescription(): _T(""));
	GetSite()->SendMessage(WM_XTP_CONTROLSELECTED, bSelected? (WPARAM)pControl: 0);

}

BOOL CXTPCommandBar::SetStatusText(CString strDescription)
{
	CWnd* pOwner = GetSite();

	if (strDescription.IsEmpty())
	{
		pOwner->SendMessage(WM_POPMESSAGESTRING, AFX_IDS_IDLEMESSAGE);
	}
	else
	{
		pOwner->SendMessage(WM_SETMESSAGESTRING, 0, (LPARAM)(LPCTSTR)strDescription);
	}
	return TRUE;
}



void CXTPCommandBar::SetPaintManager(CXTPPaintManager* pPaintManager)
{
	if (m_pPaintManager)
		m_pPaintManager->InternalRelease();

	m_pPaintManager = pPaintManager;

}

CXTPPaintManager* CXTPCommandBar::GetPaintManager()
{
	if (m_pPaintManager)
		return m_pPaintManager;

	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars)
		return pCommandBars->GetPaintManager();

	CXTPCommandBar* pRoot = GetRootParent();
	if (pRoot->m_pPaintManager)
		return pRoot->m_pPaintManager;

	return XTPPaintManager();
}

void CXTPCommandBar::SetImageManager(CXTPImageManager* pImageManager)
{
	if (m_pImageManager)
		m_pImageManager->InternalRelease();

	m_pImageManager = pImageManager;

}

CXTPImageManager* CXTPCommandBar::GetImageManager()
{
	if (m_pImageManager)
		return m_pImageManager;

	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars)
		return pCommandBars->GetImageManager();

	CXTPCommandBar* pRoot = GetRootParent();
	if (pRoot->m_pImageManager)
		return pRoot->m_pImageManager;

	return XTPImageManager();
}


void CXTPCommandBar::Copy(CXTPCommandBar* pCommandBar, BOOL bRecursive)
{
	m_barType = pCommandBar->m_barType;
	m_barPosition = pCommandBar->m_barPosition;
	m_nBarID = pCommandBar->m_nBarID;
	m_dwFlags = pCommandBar->m_dwFlags;
	m_dwStyle = pCommandBar->m_dwStyle;
	m_strTitle = pCommandBar->m_strTitle;
	m_bCustomizable = pCommandBar->m_bCustomizable;
	m_nMRUWidth = pCommandBar->m_nMRUWidth;
	m_szIcons = pCommandBar->m_szIcons;
	m_szButtons = pCommandBar->m_szButtons;
	m_pCommandBars = pCommandBar->m_pCommandBars;

	if (m_pControls)
		m_pControls->InternalRelease();

	m_pControls = pCommandBar->m_pControls->Duplicate(bRecursive);
	m_pControls->SetParent(this);
}

UINT CXTPCommandBar::GetActiveDocTemplate()
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (!pCommandBars)
		return 0;

	return pCommandBars->GetActiveDocTemplate();
}

void CXTPCommandBar::UpdateDocTemplateControls()
{
	UINT nIDResource = GetActiveDocTemplate();

	if (nIDResource == 0)
		return;

	BOOL bRecalcLayout = FALSE;
	BOOL bValue;

	foreach(pControl, FALSE)
	{
		if (pControl->m_mapDocTemplatesAssigned.GetCount() > 0)
		{
			BOOL bVisible = pControl->m_mapDocTemplatesAssigned.Lookup(nIDResource, bValue);
			if (pControl->SetHideFlag(xtpHideDocTemplate, !bVisible)) bRecalcLayout = TRUE;

		}
		if (pControl->m_mapDocTemplatesExcluded.GetCount() > 0)
		{
			BOOL bVisible = !pControl->m_mapDocTemplatesExcluded.Lookup(nIDResource, bValue);
			if (pControl->SetHideFlag(xtpHideDocTemplate, !bVisible)) bRecalcLayout = TRUE;
		}
	}

	if (bRecalcLayout)
		m_nIdleFlags |= xtpIdleLayout;
}

void CXTPCommandBar::SetFlags(DWORD dwFlagsAdd, DWORD dwFlagsRemove)
{
	m_dwFlags |= dwFlagsAdd;
	m_dwFlags &= ~dwFlagsRemove;
}


LRESULT CXTPCommandBar::OnHelpHitTest(WPARAM, LPARAM lParam)
{
	ASSERT_VALID(this);

	INT_PTR nID = OnToolHitTest((DWORD)lParam, NULL);
	if (nID != -1)
		return HID_BASE_COMMAND + nID;

	nID = _AfxGetDlgCtrlID(m_hWnd);
	return nID != 0 ? HID_BASE_CONTROL + nID : 0;
}

UINT CXTPCommandBar::OnNcHitTest(CPoint point)
{
	CFrameWnd* pFrame = GetFrameSite();
	if (pFrame && pFrame->m_bHelpMode)
	{
		ScreenToClient(&point);

		CXTPControl* pControl = m_pControls->HitTest(point);
		if (pControl && pControl->GetID() <= 0 && IsPopupControlType(pControl->GetType()))
			return HTMENU;

		return HTCLIENT;
	}

	return CWnd::OnNcHitTest(point);

}

void CXTPCommandBar::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	CFrameWnd* pFrame = GetFrameSite();
	if (pFrame && pFrame->m_bHelpMode)
	{
		ScreenToClient(&point);
		OnLButtonDown(0, point);
		return;
	}
	CWnd::OnNcLButtonDown(nHitTest, point);
}

void CXTPCommandBar::SetWidth(int nWidth)
{
	m_nMRUWidth = nWidth;
}

int CXTPCommandBar::GetWidth()
{
	return m_nMRUWidth;
}

BOOL CXTPCommandBar::IsLayoutRTL()
{
	return m_hWnd && GetExStyle() & WS_EX_LAYOUTRTL;
}

int CXTPCommandBar::OnMouseHitTest(CPoint pt)
{
	return CXTPWindowRect(this).PtInRect(pt)? HTCLIENT: HTNOWHERE;
}



STDMETHODIMP_(ULONG) CXTPCommandBar::XAccessible::AddRef()
{
	METHOD_PROLOGUE_EX_(CXTPCommandBar, Accessible)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CXTPCommandBar::XAccessible::Release()
{
	METHOD_PROLOGUE_EX_(CXTPCommandBar, Accessible)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CXTPCommandBar::XAccessible::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CXTPCommandBar, Accessible)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CXTPCommandBar::XAccessible::GetTypeInfoCount(
	unsigned int*)
{
	return E_NOTIMPL;
}

STDMETHODIMP CXTPCommandBar::XAccessible::GetTypeInfo(
	unsigned int, LCID, ITypeInfo**)
{
	return E_NOTIMPL;
}

STDMETHODIMP CXTPCommandBar::XAccessible::GetIDsOfNames(
	REFIID, LPOLESTR*, unsigned int, LCID, DISPID*)
{
	return E_NOTIMPL;
}

STDMETHODIMP CXTPCommandBar::XAccessible::get_accParent(IDispatch * FAR* ppdispParent)
{
	*ppdispParent = NULL;
	return S_OK;
}

STDMETHODIMP CXTPCommandBar::XAccessible::get_accChildCount(long FAR* /*pChildCount*/)
{
	TRACE(_T("get_accChildCount\n"));
	return E_NOTIMPL;
}
STDMETHODIMP CXTPCommandBar::XAccessible::get_accChild(VARIANT /*varChildIndex*/, IDispatch * FAR* ppdispChild)
{
	*ppdispChild = NULL;
	TRACE(_T("get_accChild\n"));
	return S_OK;
}

STDMETHODIMP CXTPCommandBar::XAccessible::get_accName(VARIANT varChild, BSTR* pszName)
{
	TRACE(_T("get_accName\n"));

	METHOD_PROLOGUE_EX_(CXTPCommandBar, Accessible);

	if (varChild.vt == VT_I4 && varChild.lVal > 0  && varChild.lVal <= pThis->GetControls()->GetCount())
	{
		*pszName = pThis->GetControl(varChild.lVal - 1)->GetCaption().AllocSysString();
		return S_OK;
	}
	return E_NOTIMPL;
}

STDMETHODIMP CXTPCommandBar::XAccessible::get_accValue(VARIANT /*varChild*/, BSTR* /*pszValue*/)
{
	TRACE(_T("get_accValue\n"));
	return E_NOTIMPL;
}

STDMETHODIMP CXTPCommandBar::XAccessible::get_accDescription(VARIANT /*varChild*/, BSTR FAR* /*pszDescription*/)
{
	TRACE(_T("get_accDescription\n"));
	return E_NOTIMPL;
}

STDMETHODIMP CXTPCommandBar::XAccessible::get_accRole(VARIANT varChild, VARIANT *pvarRole)
{
	TRACE(_T("get_accRole\n"));

	pvarRole->vt = VT_EMPTY;

	if (varChild.vt == VT_I4 && varChild.lVal > 0)
	{
		pvarRole->vt = VT_I4;
		pvarRole->lVal = ROLE_SYSTEM_MENUITEM;
	}
	return S_OK;
}

STDMETHODIMP CXTPCommandBar::XAccessible::get_accState(VARIANT varChild, VARIANT *pvarState)
{
	TRACE(_T("get_accState\n"));

	METHOD_PROLOGUE_EX_(CXTPCommandBar, Accessible);

	if (varChild.vt == VT_I4 && varChild.lVal > 0 && varChild.lVal <= pThis->GetControls()->GetCount())
	{
		pvarState->vt = VT_I4;
		pvarState->lVal = STATE_SYSTEM_FOCUSABLE|STATE_SYSTEM_SELECTABLE|
			(pThis->m_nSelected == varChild.lVal - 1? STATE_SYSTEM_FOCUSED|STATE_SYSTEM_SELECTED: 0);

		return S_OK;

	}

	return E_NOTIMPL;
}
STDMETHODIMP CXTPCommandBar::XAccessible::get_accHelp(VARIANT /*varChild*/, BSTR* /*pszHelp*/)
{
	TRACE(_T("get_accHelp\n"));
	return E_NOTIMPL;
}
STDMETHODIMP CXTPCommandBar::XAccessible::get_accHelpTopic(BSTR* /*pszHelpFile*/, VARIANT /*varChild*/, long* /*pidTopic*/)
{
	TRACE(_T("get_accHelpTopic\n"));
	return E_NOTIMPL;
}
STDMETHODIMP CXTPCommandBar::XAccessible::get_accKeyboardShortcut(VARIANT /*varChild*/, BSTR* /*pszKeyboardShortcut*/)
{
	TRACE(_T("get_accKeyboardShortcut\n"));
	return E_NOTIMPL;
}
STDMETHODIMP CXTPCommandBar::XAccessible::get_accFocus(VARIANT FAR * /*pvarFocusChild*/)
{
	TRACE(_T("get_accFocus\n"));
	return E_NOTIMPL;
}
STDMETHODIMP CXTPCommandBar::XAccessible::get_accSelection(VARIANT FAR * /*pvarSelectedChildren*/)
{
	TRACE(_T("get_accSelection\n"));
	return E_NOTIMPL;
}
STDMETHODIMP CXTPCommandBar::XAccessible::get_accDefaultAction(VARIANT /*varChild*/, BSTR* /*pszDefaultAction*/)
{
	TRACE(_T("get_accDefaultAction\n"));
	return E_NOTIMPL;
}

STDMETHODIMP CXTPCommandBar::XAccessible::accSelect(long /*flagsSelect*/, VARIANT /*varChild*/)
{
	TRACE(_T("get_accChild\n"));
	return E_NOTIMPL;
}
STDMETHODIMP CXTPCommandBar::XAccessible::accLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild)
{
	METHOD_PROLOGUE_EX_(CXTPCommandBar, Accessible);

	CRect rc;
	pThis->GetWindowRect(&rc);

	if (varChild.vt == VT_I4 && varChild.lVal > 0 && varChild.lVal <= pThis->GetControls()->GetCount())
	{
		CXTPControl* pControl = pThis->GetControl(varChild.lVal - 1);
		CRect rcControl =  pControl->GetRect();

		rc = CRect(CPoint(rc.left + rcControl.left, rc.top + rcControl.top), rcControl.Size());

	}

	*pxLeft = rc.left;
	*pyTop = rc.top;
	*pcxWidth = rc.Width();
	*pcyHeight = rc.Height();


	TRACE(_T("accLocation\n"));
	return S_OK;
}
STDMETHODIMP CXTPCommandBar::XAccessible::accNavigate(long /*navDir*/, VARIANT /*varStart*/, VARIANT * /*pvarEndUpAt*/)
{
	TRACE(_T("accNavigate\n"));
	return E_NOTIMPL;
}
STDMETHODIMP CXTPCommandBar::XAccessible::accHitTest(long /*xLeft*/, long /*yTop*/, VARIANT * /*pvarChildAtPoint*/)
{
	TRACE(_T("accHitTest\n"));
	return E_NOTIMPL;
}
STDMETHODIMP CXTPCommandBar::XAccessible::accDoDefaultAction(VARIANT /*varChild*/)
{
	TRACE(_T("accDoDefaultAction\n"));
	return E_NOTIMPL;
}

STDMETHODIMP CXTPCommandBar::XAccessible::put_accName(VARIANT /*varChild*/, BSTR /*szName*/)
{
	TRACE(_T("put_accName\n"));
	return E_NOTIMPL;
}
STDMETHODIMP CXTPCommandBar::XAccessible::put_accValue(VARIANT /*varChild*/, BSTR /*pszValue*/)
{
	TRACE(_T("put_accValue\n"));
	return E_NOTIMPL;
}
STDMETHODIMP CXTPCommandBar::XAccessible::Invoke(
	DISPID /*dispid*/, REFIID, LCID, unsigned short /*wFlags*/,
	DISPPARAMS* /*pDispParams*/, VARIANT* /*pvarResult*/,
	EXCEPINFO*, unsigned int*)
{
	return E_NOTIMPL;
}

LRESULT CXTPCommandBar::OnGetObject(WPARAM wParam, LPARAM)
{
	LPUNKNOWN lpUnknown = GetInterface(&IID_IAccessible);
	if (!lpUnknown)
		return 0;

	if (m_pLresultFromObject == 0)
	{
		HMODULE hModule = LoadLibrary(_T("oleacc.dll"));
		m_pLresultFromObject =  (LPFNLRESULTFROMOBJECT) GetProcAddress(hModule, "LresultFromObject");
	}

	return m_pLresultFromObject != 0? m_pLresultFromObject(IID_IAccessible, wParam, lpUnknown): 0;;
}





BEGIN_INTERFACE_MAP(CXTPCommandBar, CCmdTarget)
	INTERFACE_PART(CXTPCommandBar, IID_IAccessible, Accessible)
END_INTERFACE_MAP()
