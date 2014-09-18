// XTPCommandBars.cpp : implementation of the CXTPCommandBars class.
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
#include "XTPResourceManager.h"
#include "XTPColorManager.h"
#include "XTPImageManager.h"
#include "XTPTabManager.h"
#include "XTPIntel80Helpers.h"
#include "XTPPropExchange.h"

#include "XTPDockBar.h"
#include "XTPMenuBar.h"
#include "XTPShortcutManager.h"
#include "XTPPaintManager.h"
#include "XTPHookManager.h"
#include "XTPMouseManager.h"
#include "XTPControls.h"
#include "XTPControlComboBox.h"
#include "XTPCommandBarsResource.h"
#include "XTPCustomizeTools.h"
#include "XTPCommandBars.h"
#include "XTPSoundManager.h"
#include "XTPControlExt.h"
#include "XTPTabClientWnd.h"
#include "XTPReBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRuntimeClass* CXTPCommandBars::m_pToolBarClass = RUNTIME_CLASS(CXTPToolBar);
CRuntimeClass* CXTPCommandBars::m_pMenuBarClass = RUNTIME_CLASS(CXTPMenuBar);
CRuntimeClass* CXTPCommandBars::m_pPopupBarClass = RUNTIME_CLASS(CXTPPopupBar);



//////////////////////////////////////////////////////////////////////////
// CXTPCommandBarsOptions

CXTPCommandBarsOptions::CXTPCommandBarsOptions()
{
	bAlwaysShowFullMenus = TRUE; bShowFullAfterDelay = TRUE;
	bToolBarScreenTips = TRUE; bToolBarAccelTips = FALSE;
	bLargeIcons = FALSE;
	bShowExpandButtonAlways = TRUE;

	szIcons = szLargeIcons = CSize(0);
	szPopupIcons = CSize(16, 16);

	bSyncFloatingBars = TRUE;
	bDblClickFloat = FALSE;
	bShowTextBelowIcons = FALSE;

	animationType = xtpAnimateWindowsDefault;
	bAltDragCustomization = TRUE;

	bAutoHideUnusedPopups = TRUE;

	bDirtyState = FALSE;

	EnableAutomation();

}





CXTPCommandBars* CXTPCommandBars::CreateCommandBars()
{
	CXTPCommandBars* pCommandBars = (CXTPCommandBars*)CXTPCommandBars::CreateObject();
	return pCommandBars;
}


// CXTPCommandBars

IMPLEMENT_DYNCREATE(CXTPCommandBars, CCmdTarget)

CXTPCommandBars::CXTPCommandBars()
: m_pFrame (0)
, m_nDefaultMenuID(0)
{
	ZeroMemory(m_pDocks, 4 * sizeof(CXTPDockBar*));
	m_pMenuBar = 0;
	m_pDragSelected = 0;

	m_bQuickCustomizeMode = m_bCustomizeMode = FALSE;
	m_bCustomizeAvail = FALSE;
	m_pDropSource = new CXTPCustomizeDropSource(this);

	m_pPaintManager = NULL;
	m_pImageManager = NULL;

	m_pOptions = new CXTPCommandBarsOptions;
	m_pOptions->m_pCommandBars = this;

	m_pDesignerControls = 0;
	m_hDesignerAccels = 0;

	m_bIgnoreShiftMenuKey = FALSE;

	EnableAutomation();

	m_bRecalcLayout = FALSE;

}

CXTPCommandBars::~CXTPCommandBars()
{
	if (m_pDragSelected)
	{
		m_pDragSelected->InternalRelease();
		m_pDragSelected = NULL;
	}
	if (m_pDesignerControls)
		m_pDesignerControls->InternalRelease();


	while (!m_lstBars.IsEmpty())
		m_lstBars.RemoveHead()->InternalRelease();

	for (int i = 0; i < 4; i ++)
	{
		if (m_pDocks[i])
		{
			delete(m_pDocks[i]);
		}
	}

	delete m_pDropSource;

	if (m_pPaintManager)
		m_pPaintManager->InternalRelease();
	if (m_pImageManager)
		m_pImageManager->InternalRelease();


	if (m_hDesignerAccels)
	{
		::DestroyAcceleratorTable(m_hDesignerAccels);
	}
	m_pOptions->InternalRelease();

	XTPSoundManager()->StopThread();
}


CXTPMenuBar* CXTPCommandBars::SetMenu(CString strName, UINT nID)
{
	CXTPMenuBar* pMenuBar = (CXTPMenuBar*) m_pMenuBarClass->CreateObject();
	ASSERT(pMenuBar);
	if (!pMenuBar)
		return NULL;

	pMenuBar->m_pCommandBars = this;
	pMenuBar->EnableDocking(xtpFlagAlignAny | xtpFlagFloating | xtpFlagStretched);

	pMenuBar->SetTitle(strName);
	pMenuBar->m_nBarID = XTP_IDR_MENUBAR;

	if( !pMenuBar->SetPosition(xtpBarTop) ||
		!pMenuBar->LoadMenuBar(nID))
	{
		delete pMenuBar;
		return NULL;
	}

	if (nID > 0)
	{
		pMenuBar->m_pControls->CreateOriginalControls();
		m_nDefaultMenuID = nID;
	}

	m_lstBars.AddTail(pMenuBar);

	return m_pMenuBar = pMenuBar;
}

void CXTPCommandBars::RecalcFrameLayout(BOOL bDelay)
{
	if (m_bRecalcLayout)
		return;

	CFrameWnd* pFrame = DYNAMIC_DOWNCAST(CFrameWnd, GetSite());
	if (pFrame)
	{
		if (bDelay) pFrame->DelayRecalcLayout(FALSE); else pFrame->RecalcLayout(FALSE);
	}
}

void CXTPCommandBars::OnToolBarAdded(CXTPToolBar* pToolBar)
{
	ASSERT(pToolBar);

	CXTPReBar* pReBar = GetFrameReBar();
	if (pReBar)
	{
		pReBar->AddToolBar(pToolBar, RBBS_GRIPPERALWAYS|RBBS_BREAK);
	}
}

void CXTPCommandBars::OnToolBarRemoved(CXTPToolBar* pToolBar)
{
	ASSERT(pToolBar);

	if (pToolBar == m_pMenuBar)
	{
		m_pMenuBar = NULL;
	}

	CXTPReBar* pReBar = GetFrameReBar();
	if (pReBar)
	{
		pReBar->DeleteToolBar(pToolBar);
	}
}

CXTPReBar* CXTPCommandBars::GetFrameReBar()
{
	return DYNAMIC_DOWNCAST(CXTPReBar, GetSite()->GetDlgItem(AFX_IDW_REBAR));
}

CXTPToolBar* CXTPCommandBars::AddCustomBar(LPCTSTR lpcstrCaption, UINT nID, BOOL bTearOffBar)
{
	ASSERT(nID != 0);

	CXTPToolBar* pCommandBar = (CXTPToolBar*) m_pToolBarClass->CreateObject();;
	pCommandBar->m_pCommandBars = this;

	XTP_COMMANDBARS_CREATEBAR cs;
	ZeroMemory(&cs, sizeof(cs));
	cs.bCustomBar = TRUE;
	cs.bTearOffBar = bTearOffBar;
	cs.lpcstrCaption = lpcstrCaption;
	cs.pCommandBar = pCommandBar;
	cs.nID = nID;

	m_pFrame->SendMessage(WM_XTP_CREATECOMMANDBAR, 0, (LPARAM)&cs);

	ASSERT(pCommandBar == cs.pCommandBar);

	if (pCommandBar->m_pDockContext == NULL)
	{
		pCommandBar->EnableDocking();
	}



	ASSERT(pCommandBar);
	pCommandBar->m_strTitle = lpcstrCaption;

	if (!pCommandBar->SetPosition(bTearOffBar? xtpBarFloating: xtpBarTop))
	{
		delete pCommandBar;
		return NULL;
	}

	m_lstBars.AddTail(pCommandBar);

	pCommandBar->SetBarID(nID);
	pCommandBar->m_bBuiltIn = FALSE;
	pCommandBar->m_bTearOff = bTearOffBar;

	if (!bTearOffBar) RecalcFrameLayout();

	OnToolBarAdded(pCommandBar);

	return pCommandBar;
}

CXTPToolBar* CXTPCommandBars::Add(CString strName, XTPBarPosition xtpPosition, CRuntimeClass* pToolbarClass )
{
	if (!pToolbarClass)
		pToolbarClass = m_pToolBarClass;

	ASSERT(pToolbarClass && pToolbarClass->IsDerivedFrom(RUNTIME_CLASS(CXTPToolBar)));
	ASSERT( IsDockingPosition(xtpPosition) || xtpPosition == xtpBarFloating);

	CXTPToolBar* pCommandBar = (CXTPToolBar*) pToolbarClass->CreateObject();
	pCommandBar->m_pCommandBars = this;
	pCommandBar->EnableDocking();

	ASSERT(pCommandBar);
	pCommandBar->m_strTitle = strName;

	if (!pCommandBar->SetPosition(xtpPosition))
	{
		delete pCommandBar;
		return NULL;
	}

	m_lstBars.AddTail(pCommandBar);


	return pCommandBar;
}

void CXTPCommandBars::EnableDocking(void)
{
	const DWORD dwDockBarMap[4][2] =
	{
		{ AFX_IDW_DOCKBAR_TOP,      CBRS_TOP   },
		{ AFX_IDW_DOCKBAR_BOTTOM,   CBRS_BOTTOM },
		{ AFX_IDW_DOCKBAR_LEFT,     CBRS_LEFT   },
		{ AFX_IDW_DOCKBAR_RIGHT,    CBRS_RIGHT  },
	};

	CWnd* pParentFrame = GetSite();
	ASSERT(pParentFrame);

	for (int i = 0; i < 4; i++)
	{
		ASSERT(!m_pDocks[i]);

		m_pDocks[i] = new CXTPDockBar(this);
		if (!m_pDocks[i]->Create(pParentFrame,
			WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE |
			dwDockBarMap[i][1], dwDockBarMap[i][0]))
		{
			AfxThrowResourceException();
		}
	}

}

CXTPDockBar* CXTPCommandBars::CanDock(CPoint pt, CXTPDockBar* pMainDock)
{
	CRect rc;
	if (pMainDock)
	{
		pMainDock->GetWindowRect(&rc);
		if (pMainDock->m_dwStyle & CBRS_ORIENT_HORZ &&
			rc.top - 20 < pt.y && rc.bottom + 20 > pt.y && rc.left - 20 < pt.x && rc.right + 20 > pt.x)
			return pMainDock;
		if (pMainDock->m_dwStyle & CBRS_ORIENT_VERT &&
			rc.left - 20 < pt.x && rc.right + 20 > pt.x && rc.top - 20 < pt.y && rc.bottom + 20 > pt.y)
			return pMainDock;
	}
	for (int i = 0; i < 4; i++)
	{
		m_pDocks[i]->GetWindowRect(&rc);
		if (m_pDocks[i]->m_dwStyle & CBRS_ORIENT_HORZ &&
			rc.top - 20 < pt.y && rc.bottom + 20 > pt.y && rc.left - 20 < pt.x && rc.right + 20 > pt.x)
			return m_pDocks[i];
		if (m_pDocks[i]->m_dwStyle & CBRS_ORIENT_VERT &&
			rc.left - 20 < pt.x && rc.right + 20 > pt.x  && rc.top - 20 < pt.y && rc.bottom + 20 > pt.y)
			return m_pDocks[i];
	}
	return NULL;
}

BOOL CXTPCommandBars::DockCommandBar(CXTPToolBar* pBar, LPRECT lpRect, CXTPDockBar* pDockBar)
{
	ASSERT(pDockBar);

	if( !pBar->Create(m_pFrame, FALSE))
		return FALSE;

	pDockBar->DockCommandBar(pBar, lpRect);
	pBar->m_barPosition = pDockBar->GetPosition();

	return TRUE;
}

BOOL CXTPCommandBars::DockCommandBar(CXTPToolBar* pBar, XTPBarPosition xtpPosition)
{
	CXTPDockBar* pDockBar =  GetDockBar(xtpPosition);
	ASSERT(pDockBar);

	return DockCommandBar(pBar, NULL, pDockBar);
}

BOOL CXTPCommandBars::FloatCommandBar(CXTPToolBar* pBar)
{
	if( !pBar->Create(m_pFrame, TRUE))
	{
		return FALSE;
	}
	if (pBar->m_pDockBar != NULL)
	{
		pBar->m_pDockBar->RemoveCommandBar(pBar);
		pBar->m_pDockBar = NULL;

	}
	pBar->m_barPosition = xtpBarFloating;

	return TRUE;
}

void CXTPCommandBars::RedrawCommandBars()
{
	m_bRecalcLayout = TRUE;

	POSITION pos = m_lstBars.GetHeadPosition();
	while (pos)
	{
		CXTPToolBar* pBar = m_lstBars.GetNext(pos);
		if (pBar->IsVisible())
		{
			pBar->OnRecalcLayout();
		}
	}

	CMDIFrameWnd* pMDIFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, GetSite());
	if (pMDIFrame)
	{
		CXTPTabClientWnd* pClient = DYNAMIC_DOWNCAST(CXTPTabClientWnd, CWnd::FromHandle(pMDIFrame->m_hWndMDIClient));
		if (pClient)
			pClient->Refresh();
	}

	m_bRecalcLayout = FALSE;
	RecalcFrameLayout();
}

CWnd* CXTPCommandBars::GetForegroundFrame()
{
	CWnd* pWnd = CWnd::GetForegroundWindow();

	while (pWnd != NULL)
	{
		if (pWnd->IsFrameWnd() && !DYNAMIC_DOWNCAST(CMiniFrameWnd, pWnd))
			return pWnd;

		if (!pWnd || !IsWindow(pWnd->m_hWnd))
			return NULL;

		pWnd = pWnd->GetParentFrame();
	}

	return pWnd;
}


BOOL CXTPCommandBars::PreTranslateFrameMessage(MSG* pMsg)
{
	if (m_pMenuBar && m_pMenuBar->IsVisible() && !XTPMouseManager()->IsMouseLocked() && !XTPMouseManager()->IsTrackedLock(0)
		&& (pMsg->message == WM_SYSKEYDOWN) && (HIWORD(pMsg->lParam) & KF_ALTDOWN)
			&& GetForegroundFrame() == GetSite())
	{
		if (!m_bIgnoreShiftMenuKey || !(::GetKeyState(VK_SHIFT) & 0x8000))
		{

			TCHAR chAccel = (TCHAR)MapVirtualKey((UINT)pMsg->wParam, 2);
			if( chAccel)
			{
				BOOL bSelectOnly;
				int nIndex = m_pMenuBar->FindAccelEx(chAccel, bSelectOnly);
				if (nIndex != -1)
				{
					if (bSelectOnly)
					{
						m_pMenuBar->SetTrackingMode(TRUE, FALSE);
						m_pMenuBar->SetSelected(nIndex, TRUE);
					}
					else
					{
						if (DYNAMIC_DOWNCAST(CXTPControlPopup, m_pMenuBar->GetControl(nIndex)))
							m_pMenuBar->SetPopuped(nIndex, TRUE);
						else m_pMenuBar->GetControl(nIndex)->OnExecute();
					}
					return TRUE;
				}
			}
		}
	}


	if (XTPMouseManager()->IsTrackedLock(0) &&
		(pMsg->message == WM_CONTEXTMENU || pMsg->message == WM_SYSKEYUP))
	{
		return TRUE;
	}

	if (pMsg->message == WM_MOUSEWHEEL && XTPKeyboardManager()->IsKeyboardHooked())
	{
		XTPKeyboardManager()->ProcessKeyboardHooks(WM_MOUSEWHEEL, pMsg->wParam, pMsg->lParam);
		return TRUE;

	}

	return FALSE;
}

BOOL CXTPCommandBars::OnFrameWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (message == WM_IDLEUPDATECMDUI)
	{
		POSITION pos = m_lstBars.GetHeadPosition();
		while(pos)
		{
			CXTPCommandBar* pBar = m_lstBars.GetNext(pos);

			if (pBar->GetPosition() == xtpBarFloating)
			{
				pBar->OnIdleUpdateCmdUI(TRUE, TRUE);
			}
		}
	}
	if (message == WM_SYSCOLORCHANGE)
	{
		XTPColorManager()->RefreshColors();
		GetImageManager()->RefreshAll();
		GetPaintManager()->RefreshMetrics();

		RecalcFrameLayout();
		GetSite()->RedrawWindow(FALSE);
	}
	if (message == WM_SETTINGCHANGE)
	{
		GetPaintManager()->RefreshMetrics();

		RecalcFrameLayout();
		GetSite()->RedrawWindow(FALSE);
	}
	if (message == WM_CLOSE)
	{
		if (m_pDropSource->m_pSheet != NULL)
		{
			return TRUE;
		}
	}
	if (message == WM_COMMAND)
	{
		CWnd* pWnd = CWnd::GetFocus();
		if (pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CXTPEdit)) && ((CXTPEdit*)pWnd)->OnCommand(wParam, lParam))
		{
			if (pResult != NULL)
				*pResult = 1;
			return TRUE;
		}
	}
	if (XTPMouseManager()->IsTrackedLock())
	{
		switch(message)
		{
		case WM_ACTIVATEAPP:
			if (!wParam)
				XTPMouseManager()->SendTrackLost();
			return FALSE;
		case WM_SYSCOMMAND:
			if ((wParam == 0 || LOWORD(wParam) != 0) && wParam != SC_KEYMENU && !IsCustomizeMode())
				XTPMouseManager()->SendTrackLost();
			return FALSE;
		}
	}

	return FALSE;
}

CXTPControl* CXTPCommandBars::FindControl(XTPControlType type, UINT nId, BOOL bVisible, BOOL bRecursive)
{

	POSITION pos = m_lstBars.GetTailPosition();
	while(pos)
	{
		CXTPToolBar* pBar =  m_lstBars.GetPrev(pos);

		if (!bVisible || pBar->IsVisible())
		{
			CXTPControl* pControl = pBar->GetControls()->FindControl(type, nId, bVisible, bRecursive);
			if (pControl != NULL ) return pControl;
		}
	}
	return NULL;
}

void CXTPPopupBar::PumpMessage()
{

	while ( IsTrackingMode() )
	{
		MSG msg;
		if (::GetMessage(&msg, NULL, NULL, NULL))
		{
			if (!IsTrackingMode())
			{
				::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
				break;
			}

			if (msg.message != WM_KICKIDLE && !AfxGetThread()->PreTranslateMessage(&msg))
			{
				// process this message
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		} else
			break;
	}
}


BOOL CXTPCommandBars::TrackPopupMenu(CXTPPopupBar* pPopup, UINT nFlags, int x, int y, CWnd* pWnd, LPCRECT rcExclude, CWnd* pWndOwner)
{
	ASSERT(pPopup);

	pPopup->m_pSite = pWnd;
	pPopup->m_pOwner = pWndOwner;

	UINT nReturn = TRUE;

	if (nFlags & TPM_RETURNCMD)
	{
		pPopup->m_pReturnCmd = &nReturn;
		nReturn = 0;
	}
	pPopup->m_bExecOnRButton = nFlags & TPM_RIGHTBUTTON;
	pPopup->m_bIgnoreUpdateHandler = nFlags & TPM_NONOTIFY;

	if (nFlags & TPM_RIGHTALIGN)
	{
		pPopup->m_popupFlags = xtpPopupLeft;
	}

	if (!pPopup->m_bCustomizeMenu)
	{
		XTPMouseManager()->SendTrackLost();
		XTPMouseManager()->IgnoreLButtonUp();
		ReleaseCapture();
	}

	if (!pPopup->Popup(x, y, rcExclude))
	{
		return FALSE;
	}

	pPopup->PumpMessage();

	return nReturn;
}

BOOL CXTPCommandBars::TrackPopupMenu(CMenu* pMenu, UINT nFlags, int x, int y, CWnd* pWnd, LPCRECT rcExclude, CWnd* pWndOwner, CXTPCommandBars* pCommandBars)
{
	CXTPPopupBar* pPopup = CXTPPopupBar::CreatePopupBar(pCommandBars);

	pPopup->m_pSite = pWnd;
	pPopup->m_pOwner = pWndOwner;
	pPopup->m_pCommandBars = pCommandBars;


	if (!pPopup->LoadMenu(pMenu))
	{
		pPopup->InternalRelease();
		return FALSE;
	}

	BOOL bResult = TrackPopupMenu(pPopup, nFlags, x, y, pWnd, rcExclude, pWndOwner);

	pPopup->InternalRelease();

	return bResult;
}




CXTPPopupBar* CXTPCommandBars::GetToolbarsPopup()
{
	CXTPPopupBar* pPopup = CXTPPopupBar::CreatePopupBar(this);

	int nIndex = 0;

	POSITION pos = m_lstBars.GetHeadPosition();
	while (pos)
	{
		CXTPToolBar* pBar = m_lstBars.GetNext(pos);

		if (pBar->GetType() == xtpBarTypeNormal && pBar->m_bContextMenuPresent && pBar->IsCloseable())
		{
			CXTPControl* pButton =  pPopup->GetControls()->Add(new CXTPControlToolbars::CXTPControlToolbar(), 0, _T(""), -1, TRUE);
			pButton->SetTag(nIndex);
			pButton->SetCaption(pBar->GetTitle());
			pButton->SetFlags(xtpFlagManualUpdate);
			pButton->SetChecked(pBar->IsVisible());
		}
		nIndex++;
	}

	if (m_bCustomizeAvail && nIndex > 0)
	{

		CXTPControl* pControlCustomize = pPopup->GetControls()->Add(xtpControlButton, XTP_ID_CUSTOMIZE);
		pControlCustomize->SetBeginGroup(TRUE);
	}

	return pPopup;
}


void CXTPCommandBars::ContextMenu(CPoint point)
{
	if (IsCustomizeMode())
		return;

	CXTPPopupBar* pPopup = GetToolbarsPopup();

	if (pPopup->GetControls()->GetCount() > 0)
	{
		pPopup->m_popupFlags = GetSite()->GetExStyle() & WS_EX_LAYOUTRTL? xtpPopupLeft: xtpPopupRight;

		pPopup->Popup(point.x, point.y, NULL);

		while ( pPopup->IsTrackingMode() )
		{
			AfxGetThread()->PumpMessage();
		}
	}
	pPopup->InternalRelease();
}

CXTPToolBar* CXTPCommandBars::GetAt(int nIndex)
{
	POSITION pos = m_lstBars.FindIndex(nIndex);
	if (pos == NULL) return NULL;
	return m_lstBars.GetAt(pos);
}

void CXTPCommandBars::ToggleVisible(int nIndex)
{
	ASSERT(nIndex >=0 && nIndex < m_lstBars.GetCount());

	CXTPToolBar* pBar = GetAt(nIndex);
	if (pBar)
	{
		pBar->SetVisible(!pBar->IsVisible());
	}
}

CXTPToolBar* CXTPCommandBars::GetToolBar(UINT nID)
{
	if (nID == 0)
		return NULL;
	POSITION pos = m_lstBars.GetHeadPosition();
	while (pos != NULL)
	{
		CXTPToolBar* pBar = m_lstBars.GetNext(pos);
		ASSERT(pBar != NULL);
		if (pBar->m_nBarID == nID)
			return pBar;
	}
	return NULL;
}

void CXTPCommandBars::GetHiddenControls(CXTPToolBar* pToolBar, CXTPCommandBar* pExpandBar)
{
	if (pToolBar->m_pDockBar == 0)  return;

	CXTPDockBar* pDock = pToolBar->m_pDockBar;
	int nPos = pDock->FindBar(pToolBar);
	if (nPos == -1) return;

	CXTPDockBar::CToolBarArray arrBars;
	pDock->GetVisibleToolbars(nPos, arrBars);
	for (int i = 0; i < arrBars.GetSize(); i++)
	{
		CXTPToolBar* pToolBar = arrBars[i];

		for (int i = 0; i < pToolBar->GetControls()->GetCount(); i++)
		{
			CXTPControl* pControl = pToolBar->GetControl(i);

			if (pControl->GetHideFlags() == xtpHideWrap)
			{
				CXTPControl* pControlClone = pExpandBar->GetControls()->AddClone(pControl);
				pControlClone->SetHideFlags(0);
				pControlClone->SetBeginGroup(FALSE);
			}
		}
	}
}

void CXTPCommandBars::_GetAddOrRemovePopup(CXTPToolBar* pToolBar, CXTPCommandBar* pExpandBar)
{
	CXTPControlPopup* pButton = (CXTPControlPopup*)pExpandBar->GetControls()->Add(xtpControlPopup, 0);
	pButton->SetFlags(xtpFlagManualUpdate);
	pButton->SetCaption(pToolBar->GetTitle());

	BOOL bEnabled = m_bCustomizeAvail && pToolBar->m_bBuiltIn && pToolBar->m_bCustomizable;

	pButton->SetEnabled(bEnabled);
	if (bEnabled)
	{
		pToolBar->BuildCustomizePopup(pButton->GetCommandBar());
	}
}

void CXTPCommandBars::GetAddOrRemovePopup(CXTPToolBar* pToolBar, CXTPCommandBar* pExpandBar)
{
	if (pToolBar->m_pDockBar == 0)
	{
		_GetAddOrRemovePopup(pToolBar, pExpandBar);
	}
	else
	{

		CXTPDockBar* pDock = pToolBar->m_pDockBar;
		int nPos = pDock->FindBar(pToolBar);
		if (nPos == -1)
		{
			_GetAddOrRemovePopup(pToolBar, pExpandBar);
		} else
		{
			CXTPDockBar::CToolBarArray arrBars;
			pDock->GetVisibleToolbars(nPos, arrBars);
			for (int i = 0; i < arrBars.GetSize(); i++)
			{
				_GetAddOrRemovePopup(arrBars[i], pExpandBar);
			}
		}
	}

	if (m_bCustomizeAvail)
	{
		CXTPControl* pControlCustomize = pExpandBar->GetControls()->Add(xtpControlButton, XTP_ID_CUSTOMIZE);
		pControlCustomize->SetBeginGroup(TRUE);
	}
}


void CXTPCommandBars::SetQuickCustomizeMode(BOOL bMode)
{
	m_pOptions->bDirtyState = TRUE;
	m_bCustomizeMode = m_bQuickCustomizeMode = bMode;
	SetDragControl(NULL);
}

void CXTPCommandBars::SetCustomizeMode(BOOL bMode)
{
	m_pOptions->bDirtyState = TRUE;
	m_bCustomizeMode = bMode;
	SetDragControl(NULL);

	POSITION pos = m_lstBars.GetHeadPosition();
	while(pos)
	{
		CXTPCommandBar* pBar = m_lstBars.GetNext(pos);
		pBar->OnIdleUpdateCmdUI(TRUE, TRUE);
	}
	RecalcFrameLayout();
}

void CXTPCommandBars::SetDragControl(CXTPControl* pControl)
{
	if (m_pDragSelected != pControl)
	{

		CXTPControl* pDragSelected = m_pDragSelected;
		m_pDragSelected = pControl;

		if (pDragSelected != NULL)
		{
			SAFE_INVALIDATE(pDragSelected->GetParent());
			pDragSelected->InternalRelease();
		}
		if (m_pDragSelected)
		{
			SAFE_INVALIDATE(m_pDragSelected->GetParent());
			m_pDragSelected->InternalAddRef();
		}
	}

	CWnd* pWnd = m_pDropSource->m_pSheet? (CWnd*)m_pDropSource->m_pSheet: m_pFrame;
	if (pWnd && pWnd->GetSafeHwnd())
	{
		pWnd->SendMessage(WM_XTP_DRAGCONTROLCHANGED);
	}
}

void CXTPCommandBars::RemoveAll()
{
	m_bRecalcLayout = TRUE;

	while (!m_lstBars.IsEmpty())
	{
		CXTPToolBar* pToolBar = m_lstBars.GetHead();

		if (pToolBar->GetSafeHwnd())
		{
			pToolBar->ShowWindow(SW_HIDE);
			pToolBar->m_bVisible = FALSE;
		}

		if (pToolBar->m_pDockBar != NULL)
		{
			pToolBar->m_pDockBar->RemoveCommandBar(pToolBar);
			pToolBar->m_pDockBar = NULL;			
		}

		OnToolBarRemoved(pToolBar);
		
		m_lstBars.RemoveHead();
		pToolBar->InternalRelease();
	}
	
	m_bRecalcLayout = FALSE;
	RecalcFrameLayout(TRUE);
	
}

void CXTPCommandBars::Remove(CXTPToolBar* pToolBar)
{
	ASSERT(pToolBar);

	POSITION pos = m_lstBars.GetHeadPosition();
	while (pos != NULL)
	{
		CXTPToolBar* pBar = m_lstBars.GetAt(pos);

		if (pBar == pToolBar)
		{
			if (pToolBar->GetSafeHwnd())
			{	
				pToolBar->ShowWindow(SW_HIDE);
			}
			pToolBar->m_bVisible = FALSE;

			if (pToolBar->m_pDockBar != NULL)
			{
				pToolBar->m_pDockBar->RemoveCommandBar(pToolBar);
				pToolBar->m_pDockBar = NULL;			
				RecalcFrameLayout();
			}

			OnToolBarRemoved(pToolBar);
			m_lstBars.RemoveAt(pos);
			pToolBar->InternalRelease();
			break;
		}
		m_lstBars.GetNext(pos);
	}
}

void CXTPCommandBars::SetSite(CWnd* pFrame)
{
	m_pFrame = pFrame;

	if (pFrame->GetSafeHwnd())
	{
		AFX_CMDHANDLERINFO info;
		info.pTarget = NULL;

		m_bCustomizeAvail = pFrame->OnCmdMsg(XTP_ID_CUSTOMIZE, CN_COMMAND, this, &info);
	}
}



void CXTPCommandBars::HideCommands(const UINT* pCommands, int nCount)
{
	ASSERT(pCommands);
	for (int i = 0; i < nCount; i++)
		HideCommand(pCommands[i]);
}

void CXTPCommandBars::HideCommand(UINT nCommand)
{
	m_pOptions->m_mapHiddenCommands.SetAt(nCommand, TRUE);
}


BOOL CXTPCommandBars::IsControlHidden(CXTPControl* pControl)
{
	BOOL bCommandHidden = IsCommandHidden(pControl->GetID());

	if (m_pOptions->bAutoHideUnusedPopups && !bCommandHidden && IsPopupControlType(pControl->GetType()))
	{
		CXTPCommandBar* pCommandBar = pControl->GetCommandBar();
		if (!pCommandBar)
			return FALSE;

		int nCount = pCommandBar->GetControls()->GetCount();

		if (nCount == 0)
			return FALSE;

		for (int i = 0; i < nCount; i++)
		{
			if (!IsControlHidden(pCommandBar->GetControl(i)))
				return FALSE;
		}

		return TRUE;

	}
	return bCommandHidden;
}

BOOL CXTPCommandBars::IsCommandHidden(UINT nCommand)
{
	BOOL bCommand;

	if (!m_pOptions->m_mapHiddenCommands.Lookup(nCommand, bCommand))
		return FALSE;

	if (m_pOptions->m_mapUsedCommands.Lookup(nCommand, bCommand))
		return FALSE;

	return TRUE;
}

void CXTPCommandBars::SetCommandUsed(UINT nCommand)
{
	if (IsCommandHidden(nCommand))
	{
		m_pOptions->m_mapUsedCommands.SetAt(nCommand, TRUE);
	}
}
void CXTPCommandBars::ResetUsageData()
{
	m_pOptions->m_mapUsedCommands.RemoveAll();
}


AFX_STATIC_DATA const TCHAR _xtpCommandBarOptionsSection[]  = _T("%s-Options");

void PX_UintMap(CXTPPropExchange* pPX, LPCTSTR pszPropName, CMap<UINT, UINT, BOOL, BOOL>& mapValue)
{
	if (pPX->IsAllowBlobValues())
	{
		PX_Serialize(pPX, pszPropName, &mapValue);
		return;
	}

	CXTPPropExchangeSection secMap(pPX->GetSection(pszPropName));

	UINT nID;
	if (pPX->IsStoring())
	{
		secMap->WriteCount((DWORD)mapValue.GetCount());
		BOOL bValue; int i = 0;
		POSITION pos = mapValue.GetStartPosition();
		while (pos)
		{
			CString strSection;
			strSection.Format(_T("Item%i"), i++);

			mapValue.GetNextAssoc(pos, nID, bValue);

			PX_Long(&secMap, strSection, (long&)nID, 0);
		}
	}
	else
	{
		mapValue.RemoveAll();

		int nCount = secMap->ReadCount();
		for (int i = 0; i < nCount; i++)
		{
			CString strSection;
			strSection.Format(_T("Item%i"), i);
			PX_Long(&secMap, strSection, (long&)nID, 0);

			mapValue.SetAt(nID, TRUE);
		}
	}
}

void CXTPCommandBarsOptions::DoPropExchange(CXTPPropExchange* pPX, BOOL bUserOptionsOnly)
{
	pPX->UseDefaultOnLoad(FALSE);

	XTP_COMMANDBARS_ICONSINFO* pInfo = m_pCommandBars->GetPaintManager()->GetIconsInfo();
	DWORD dwSchema = _XTP_SCHEMA_CURRENT;

	PX_DWord(pPX, _T("Schema"), dwSchema, _XTP_SCHEMA_CURRENT);
	PX_Bool(pPX, _T("AlwaysShowFullMenus"), bAlwaysShowFullMenus, TRUE);
	PX_Bool(pPX, _T("ShowFullAfterDelay"), bShowFullAfterDelay, TRUE);
	PX_Bool(pPX, _T("ToolBarScreenTips"), bToolBarScreenTips, TRUE);
	PX_Bool(pPX, _T("ToolBarAccelTips"), bToolBarAccelTips, FALSE);
	PX_Bool(pPX, _T("LargeIcons"), bLargeIcons, FALSE);

	if ((dwSchema > _XTP_SCHEMA_875) && !(pPX->IsStoring() && pPX->IsAllowDefaultValues() && m_mapUsedCommands.IsEmpty()))
		PX_UintMap(pPX,_T("UsedCommands"), m_mapUsedCommands );

	if ((dwSchema > _XTP_SCHEMA_875) || bUserOptionsOnly)
		PX_Enum(pPX, _T("Animation"), animationType, xtpAnimateWindowsDefault);


	if (!bUserOptionsOnly)
	{
		PX_Bool(pPX, _T("SyncFloatingBars"), bSyncFloatingBars, TRUE);
		PX_Bool(pPX, _T("ShowExpandButtonAlways"), bShowExpandButtonAlways, TRUE);
		PX_Size(pPX, _T("IconsSize"), szIcons, CSize(0, 0));
		PX_Size(pPX, _T("LargeIconsSize"), szLargeIcons, CSize(0, 0));

		if (!(pPX->IsStoring() && pPX->IsAllowDefaultValues() && m_mapHiddenCommands.IsEmpty()))
			PX_UintMap(pPX,_T("HiddenCommands"), m_mapHiddenCommands );

		PX_Bool(pPX, _T("IconsWithShadow"), pInfo->bIconsWithShadow, FALSE);
		PX_Bool(pPX, _T("UseDisabledIcons"), pInfo->bUseDisabledIcons, TRUE);
		PX_Bool(pPX, _T("UseFadedIcons"), pInfo->bUseFadedIcons, TRUE);

		if (dwSchema > _XTP_SCHEMA_875)
		{
			PX_Bool(pPX, _T("SyncFloatingBars"), bSyncFloatingBars, TRUE);
			PX_Bool(pPX, _T("ShowExpandButtonAlways"), bShowExpandButtonAlways, TRUE);
			PX_Bool(pPX, _T("DblClickFloat"), bDblClickFloat, FALSE);
			PX_Bool(pPX, _T("ShowTextBelowIcons"), bShowTextBelowIcons, FALSE);
			PX_Bool(pPX, _T("AltDragCustomization"), bAltDragCustomization, TRUE);
			PX_Bool(pPX, _T("AutoHideUnusedPopups"), bAutoHideUnusedPopups, TRUE);
			PX_Size(pPX, _T("PopupIconsSize"), szPopupIcons, CSize(16, 16));
		}
	}

}
void CXTPCommandBarsOptions::Serialize(CArchive& ar)
{
	CXTPPropExchangeArchive px(ar);
	DoPropExchange(&px);
}


void CXTPCommandBars::SaveOptions(LPCTSTR lpszProfileName)
{
	TCHAR szSection[256];
	wsprintf(szSection, _xtpCommandBarOptionsSection, lpszProfileName);

	HKEY  hKey = AfxGetApp()->GetAppRegistryKey();
	CXTPPropExchangeRegistry px(FALSE, hKey, szSection);
	m_pOptions->DoPropExchange(&px, TRUE);

	RegCloseKey(hKey);
}


void CXTPCommandBars::LoadOptions(LPCTSTR lpszProfileName)
{
	TCHAR szSection[256];
	wsprintf(szSection, _xtpCommandBarOptionsSection, lpszProfileName);

	HKEY  hKey = AfxGetApp()->GetAppRegistryKey();
	CXTPPropExchangeRegistry px(TRUE, hKey, szSection);
	m_pOptions->DoPropExchange(&px, TRUE);

	RegCloseKey(hKey);
}

void CXTPCommandBars::SetPaintManager(CXTPPaintManager* pPaintManager)
{
	if (m_pPaintManager)
		m_pPaintManager->InternalRelease();

	m_pPaintManager = pPaintManager;

}
void CXTPCommandBars::SetImageManager(CXTPImageManager* pImageManager)
{
	ASSERT(m_pImageManager == NULL);
	m_pImageManager = pImageManager;
}

CXTPPaintManager* CXTPCommandBars::GetPaintManager()
{
	if (m_pPaintManager)
		return m_pPaintManager;
	return XTPPaintManager();

}
CXTPImageManager* CXTPCommandBars::GetImageManager()
{
	if (m_pImageManager)
		return m_pImageManager;
	return XTPImageManager();
}

void CXTPCommandBars::LoadDesignerBars(CArchive& ar)
{
	CXTPPropExchangeArchive px(ar);
	LoadDesignerBars(&px);
}

void CXTPCommandBars::LoadDesignerBars(CXTPPropExchange* pPX)
{
	try
	{
		//////////////////////////////////////////////////////////////////////////
		// Load Docking Position and Controls
		XTP_COMMANDBARS_PROPEXCHANGE_PARAM param;
		param.bSerializeControls = TRUE;
		param.bSerializeDesignerControls = TRUE;
		param.bSerializeImages = TRUE;
		param.bSerializeOptions = TRUE;

		DoPropExchange(pPX, &param);

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


CDocTemplate* CXTPCommandBars::FindDocTemplate(CMDIChildWnd* pChild)
{
	USES_PROTECTED_ACCESS(CXTPCommandBars, CMDIChildWnd, HMENU, m_hMenuShared)

	CMultiDocTemplate* pTemplate = DYNAMIC_DOWNCAST(CMultiDocTemplate, pChild->GetActiveDocument()->GetDocTemplate());
	if (!pTemplate)
		return NULL;

	HMENU hMenuShared = PROTECTED_ACCESS(CMDIChildWnd, pChild, m_hMenuShared);

	if ((hMenuShared == 0) || (pTemplate->m_hMenuShared == hMenuShared))
		return pTemplate;

	POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();

	while(pos)
	{
		CMultiDocTemplate* pTemplate =
			DYNAMIC_DOWNCAST(CMultiDocTemplate, AfxGetApp()->GetNextDocTemplate(pos));

		if (pTemplate && pTemplate->m_hMenuShared == hMenuShared)
			return pTemplate;
	}


	return NULL;
}


UINT CXTPCommandBars::GetActiveDocTemplate()
{
	#pragma warning(disable:4510)
	#pragma warning(disable:4610)

	USES_PROTECTED_ACCESS(CXTPCommandBars, CDocTemplate, UINT, m_nIDResource)
	USES_PROTECTED_ACCESS(CXTPCommandBars, CFrameWnd, UINT, m_nIDHelp)

	CMDIFrameWnd* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, GetSite());

	if (!pFrame)
		return 0;

	UINT nIDResource = (WORD)m_nDefaultMenuID;

	CMDIChildWnd* pChild = pFrame->MDIGetActive();
	if (pChild)
	{
		if (pChild->GetActiveDocument())
		{
			CDocTemplate* pDocTemplate = FindDocTemplate(pChild);
			if (!pDocTemplate)
				return 0;
			nIDResource = PROTECTED_ACCESS(CDocTemplate, pDocTemplate, m_nIDResource);
		} else
		{
			nIDResource = PROTECTED_ACCESS(CFrameWnd, pChild, m_nIDHelp);
		}
	}
	return nIDResource;
}


void CXTPCommandBars::OnSetPreviewMode (BOOL bPreview)
{
	int nCount = GetCount();
	for (int i = 0; i < nCount; i++)
	{

		CXTPToolBar* pBar = GetAt(i);
		pBar->OnSetPreviewMode(bPreview);
	}	
}