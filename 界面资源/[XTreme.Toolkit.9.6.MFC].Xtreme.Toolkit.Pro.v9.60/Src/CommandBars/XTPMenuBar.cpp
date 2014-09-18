// XTPMenuBar.cpp : implementation of the CXTPMenuBar class.
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
#include "XTPImageManager.h"

#include "XTPMenuBar.h"
#include "XTPControls.h"
#include "XTPControl.h"
#include "XTPHookManager.h"
#include "XTPControlButton.h"
#include "XTPControlPopup.h"
#include "XTPPaintManager.h"
#include "XTPCommandBarsResource.h"
#include "XTPCommandBars.h"
#include "XTPMouseManager.h"
#include "XTPPopupBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TID_REFRESH 12223

#ifndef OIC_WINLOGO
#define OIC_WINLOGO         32517
#endif



class CXTPMenuBar::CXTPControlMDIButton : public CXTPControlButton
{
public:
	CXTPControlMDIButton()
	{
		m_dwFlags |= xtpFlagRightAlign | xtpFlagSkipFocus | xtpFlagManualUpdate | xtpFlagNoMovable;
	}
	CSize GetSize(CDC* /*pDC*/)  { return CSize(16, 16);};
	void SetBeginGroup(BOOL /*bBeginGroup*/) {}
	void Draw(CDC* pDC);
	void OnExecute()
	{
		HWND hWndChild = ((CXTPMenuBar*)m_pParent)->GetActiveMdiChildWnd();
		ASSERT(hWndChild);

		UINT nId = m_nId == XTP_ID_MENUBAR_CLOSE? SC_CLOSE:
			m_nId == XTP_ID_MENUBAR_RESTORE? SC_RESTORE: SC_MINIMIZE;

		::PostMessage(hWndChild, WM_SYSCOMMAND, nId, 0);
	}
};

class CXTPMenuBar::CXTPControlMDISysMenuPopup : public CXTPControlPopup
{
	class CXTPSysMenuPopupBar : public CXTPPopupBar
	{
		void UpdateShortcuts()
		{
		}

	};

public:
	CXTPControlMDISysMenuPopup(HWND hWndActiveChild, HMENU hDocMenu)
	{
		m_dwFlags |= xtpFlagManualUpdate | xtpFlagNoMovable;
		m_controlType = xtpControlPopup;
		m_strCaption = _T("-");
		m_hIcon = 0;

		m_pCommandBar = new CXTPSysMenuPopupBar();
		m_pCommandBar->EnableCustomization(FALSE);

		SetMDISysMenu(hWndActiveChild, hDocMenu);

	}
	void SetMDISysMenu(HWND hWndActiveChild, HMENU hDocMenu)
	{
		m_hDocMenu = hDocMenu;

		HICON hIcon = (HICON)::SendMessage(hWndActiveChild, WM_GETICON, ICON_SMALL, 0);
		if( hIcon == NULL ) hIcon = (HICON)::SendMessage(hWndActiveChild, WM_GETICON, ICON_BIG, 0 );
		if(hIcon == NULL) hIcon = (HICON)(ULONG_PTR)::GetClassLongPtr(hWndActiveChild, GCL_HICONSM );
		if (hIcon == NULL) hIcon = AfxGetApp()->LoadOEMIcon(OIC_WINLOGO);

		if (m_hIcon)
		{
			DestroyIcon(m_hIcon);
			m_hIcon = 0;
		}

		if (hIcon)
			m_hIcon = CXTPImageManagerIcon::ScaleToFit(hIcon, CXTPImageManagerIcon::GetExtent(hIcon), 16);

		m_pCommandBar->GetControls()->RemoveAll();

		CMenu* pMenu = CMenu::FromHandle(hDocMenu);

		BOOL bSeparator = FALSE;
		for (int i = 0; i < (int)pMenu->GetMenuItemCount(); i++)
		{
			UINT nItemID = pMenu->GetMenuItemID(i);
			if (nItemID == 0)
				bSeparator = TRUE;
			else
			{
				CXTPControl* pControl = NULL;
				if (nItemID >= 0xF000 && nItemID < 0xF1F0)
				{
					pControl = (CXTPControl*)CXTPControlButton::CreateObject();
					pControl->SetFlags(xtpFlagManualUpdate);
					m_pCommandBar->GetControls()->Add(pControl, nItemID);

					CString strCaption;
					pMenu->GetMenuString(i, strCaption, MF_BYPOSITION);
					pControl->SetCaption(strCaption);

					CString strDescription;
					strDescription.LoadString(ID_COMMAND_FROM_SC(nItemID));
					pControl->SetDescription(strDescription);

					pControl->SetItemDefault((int)::GetMenuDefaultItem(pMenu->m_hMenu, GMDI_USEDISABLED, TRUE) == i);


					pControl->SetEnabled(!(pMenu->GetMenuState (i, MF_BYPOSITION) & MF_DISABLED));
				}
				else
				{
					pControl = m_pCommandBar->GetControls()->AddMenuItem(pMenu, i);;

				}

				if (pControl && bSeparator)
				{
					pControl->SetBeginGroup(TRUE);
					bSeparator = FALSE;
				}
			}
		}


	}
	~CXTPControlMDISysMenuPopup()
	{
		if (m_hIcon) DestroyIcon(m_hIcon);
	}
	CSize GetSize(CDC* /*pDC*/)  { return CSize(16, 16);};
	void Draw(CDC* pDC)
	{
		if (m_hIcon)
		{
			pDC->DrawState (GetRect().TopLeft(), CSize(16, 16), m_hIcon, (UINT)DSS_NORMAL, HBRUSH(0));
		}
	}
	void UpdateShadow()
	{

	}
	void AdjustExcludeRect(CRect& rc)
	{
		rc.InflateRect(1, 1);
	}

public:
	HMENU m_hDocMenu;
	HICON m_hIcon;
};

void CXTPMenuBar::CXTPControlMDIButton::Draw(CDC* pDC)
{
	CFont font;
	font.CreatePointFont(MulDiv(80, 96, pDC->GetDeviceCaps(LOGPIXELSX)), _T("Marlett"));
	CFont* pFont = pDC->SelectObject(&font);
	GetPaintManager()->DrawControlEntry(pDC, this);
	pDC->DrawText(m_strParameter, m_rcControl, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pDC->SelectObject(pFont);
}


IMPLEMENT_XTP_COMMANDBAR(CXTPMenuBar, CXTPToolBar)

CXTPMenuBar::CXTPMenuBar(void)
{
	m_barType = xtpBarTypeMenuBar;
	m_bMdiApp = FALSE;
	m_hWndActiveChild = 0;
	m_nIDResource = 0;
	m_nIDResourceCurrent = 0;

	m_bDirtyMenu = FALSE;
	m_hMenuDefault = NULL;
}

CXTPMenuBar::~CXTPMenuBar(void)
{
	FreeMDIMenus();

	if (m_hMenuDefault)
	{
		DestroyMenu(m_hMenuDefault);
	}
}


void CXTPMenuBar::SetupHook()
{
	m_bMdiApp = GetSite()->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd));


	CWnd* pSite = GetSite();
	XTPHookManager()->SetHook(pSite->GetSafeHwnd(), this);

	CMDIFrameWnd* pMDIFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, pSite);
	if (pMDIFrame)
	{
		XTPHookManager()->SetHook(pMDIFrame->m_hWndMDIClient, this);
	}
}

HWND CXTPMenuBar::GetActiveMdiChildWnd(BOOL* bMaximized)
{
	BOOL bMax = FALSE;
	if( !m_bMdiApp ) return NULL;
	CMDIFrameWnd * pFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, GetSite());
	ASSERT_VALID( pFrame );
	HWND hWndMdiArea = pFrame->m_hWndMDIClient;

	ASSERT(hWndMdiArea && ::IsWindow(hWndMdiArea) );

	HWND hWnd = (HWND) ::SendMessage(hWndMdiArea, WM_MDIGETACTIVE, 0, (LPARAM)&bMax);
	ASSERT(hWnd == NULL || ::IsWindow( hWnd ));
	if (bMaximized) *bMaximized = bMax;
	return hWnd;
}


void CXTPMenuBar::SyncActiveMdiChild()
{
	HWND hWndActiveChild = GetActiveMdiChildWnd();
	if( m_hWndActiveChild == hWndActiveChild )
		return;

	if( m_hWndActiveChild != NULL )
		XTPHookManager()->RemoveHook(m_hWndActiveChild, this );
	m_hWndActiveChild = hWndActiveChild;
	if( m_hWndActiveChild != NULL )
		XTPHookManager()->SetHook(m_hWndActiveChild, this );
}

void CXTPMenuBar::RefreshMenu()
{
	SyncActiveMdiChild();

	if (IsVisible())
	{
		RefreshSysButtons();
	}
}


void CXTPMenuBar::AddSysButton(CXTPControl* pControl, int nId, CString strParameter, int nBefore /*= -1*/)
{
	m_pControls->Add(pControl, nId, strParameter, nBefore, TRUE);
}

void CXTPMenuBar::RefreshSysButtons()
{
	BOOL bMax = FALSE;
	HWND hWndActiveChild = GetActiveMdiChildWnd(&bMax);
	DWORD dwStyle = hWndActiveChild? GetWindowLong(hWndActiveChild, GWL_STYLE): 0;
	CXTPControl*  pButton;


	pButton = m_pControls->FindControl(XTP_ID_MENUBAR_SYSMENU);

	if (bMax && (m_dwFlags & xtpFlagAddMDISysPopup))
	{
		HMENU hDocMenuButton = pButton? ((CXTPControlMDISysMenuPopup*)pButton)->m_hDocMenu: 0;

		HMENU hDocMenu =
			::GetSystemMenu(hWndActiveChild, FALSE);
		if (hDocMenu && ::IsMenu(hDocMenu))
		{
			if  (hDocMenuButton != hDocMenu)
			{
				if (pButton)
				{
					((CXTPControlMDISysMenuPopup*)pButton)->SetMDISysMenu(hWndActiveChild, hDocMenu);
					DelayRedraw();
				}
				else
				{
					AddSysButton(new CXTPControlMDISysMenuPopup(hWndActiveChild, hDocMenu), XTP_ID_MENUBAR_SYSMENU, _T(""), 0);
				}
			}
		}
		else
		{
			if (pButton) m_pControls->Remove(pButton);

		}

	}
	else if (pButton) m_pControls->Remove(pButton);


	pButton = (CXTPControlButton* )m_pControls->FindControl(XTP_ID_MENUBAR_MINIMIZE);
	if (!pButton && bMax && (dwStyle & WS_MINIMIZEBOX) && (!(m_dwFlags & xtpFlagHideMinimizeBox))) AddSysButton(new CXTPControlMDIButton(), XTP_ID_MENUBAR_MINIMIZE, _T("0"));
	else if (pButton && !bMax) m_pControls->Remove(pButton);

	pButton = (CXTPControlButton* )m_pControls->FindControl(XTP_ID_MENUBAR_RESTORE);
	if (!pButton && bMax && (dwStyle & WS_MAXIMIZEBOX) && (!(m_dwFlags & xtpFlagHideMaximizeBox))) AddSysButton(new CXTPControlMDIButton(), XTP_ID_MENUBAR_RESTORE, _T("2"));
	else if (pButton && !bMax) m_pControls->Remove(pButton);

	pButton = (CXTPControlButton* )m_pControls->FindControl(XTP_ID_MENUBAR_CLOSE);
	if (!pButton && bMax && (!(m_dwFlags & xtpFlagHideClose))) AddSysButton(new CXTPControlMDIButton(), XTP_ID_MENUBAR_CLOSE, _T("r"));
	else if (pButton && !bMax) m_pControls->Remove(pButton);
}

void CXTPMenuBar::FreeMDIMenus()
{
	POSITION pos = m_mapMDIMenus.GetStartPosition();
	while (pos)
	{
		UINT nIDCommand;
		CXTPControls* pControls;
		m_mapMDIMenus.GetNextAssoc(pos, nIDCommand, pControls);
		pControls->InternalRelease();
	}
	m_mapMDIMenus.RemoveAll();
}

void CXTPMenuBar::CleanupMDIMenus()
{
	m_bDirtyMenu = FALSE;

	if (m_mapMDIMenus.GetCount() > 0)
	{
		SwitchMDIMenu(m_hMenuDefault);
		m_bDirtyMenu = TRUE;
	}

	POSITION pos = m_mapMDIMenus.GetStartPosition();
	while (pos)
	{
		UINT nIDCommand;
		CXTPControls* pControls;
		m_mapMDIMenus.GetNextAssoc(pos, nIDCommand, pControls);

		if (!pControls->IsChanged())
		{
			pControls->InternalRelease();
			m_mapMDIMenus.RemoveKey(nIDCommand);
		}
	}

}

void CXTPMenuBar::SwitchMDIMenu(HMENU hMenu)
{
	UINT nIDResource = GetActiveDocTemplate();

	if (hMenu == m_hMenuDefault)
	{
		nIDResource = m_nIDResource;
	}

	if ((nIDResource == 0) || (m_nIDResourceCurrent == 0) || (m_nIDResourceCurrent == nIDResource))
		return;

	CMenu* pMenu = CMenu::FromHandle(hMenu);
	if (!pMenu || pMenu->GetMenuItemCount() == 0)
		return;

	if (GetCommandBars()->IsCustomizeAvail())
	{
		CXTPControls* pMDIControls = 0;
		if (m_mapMDIMenus.Lookup(m_nIDResourceCurrent, pMDIControls))
		{
			m_mapMDIMenus.RemoveKey(m_nIDResourceCurrent);
			pMDIControls->InternalRelease();
		}

		m_mapMDIMenus.SetAt(m_nIDResourceCurrent, m_pControls);
		m_pControls->SetParent(0);

		if (!m_mapMDIMenus.Lookup(nIDResource, pMDIControls) )
		{
			m_pControls = new CXTPControls();
			m_pControls->SetParent(this);
			LoadMenu(pMenu);
			m_pControls->CreateOriginalControls();
		}
		else
		{
			m_pControls = pMDIControls;
			m_pControls->SetParent(this);

			if (m_pControls->GetOriginalControls() == 0)
			{
				m_pControls->m_pOriginalControls = new CXTPControls();
				m_pControls->m_pOriginalControls->LoadMenu(pMenu);
			}

			m_mapMDIMenus.RemoveKey(nIDResource);
		}
	}
	else
	{
		LoadMenu(pMenu);
	}

	OnRecalcLayout();

	m_nIDResourceCurrent = nIDResource;
}

int CXTPMenuBar::OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult)
{

	switch (nMessage)
	{
		case WM_SYSCOMMAND:
			if (IsVisible() && !XTPMouseManager()->IsMouseLocked() && (GetKeyState(VK_SHIFT) >= 0) && !IsCustomizeMode())
			if (((wParam & 0xFFF0)  == SC_KEYMENU) && ( lParam == 0 ))
			{
				SetTrackingMode(TRUE, FALSE);
				SetSelected(m_pControls->GetNext(-1, +1, TRUE, TRUE), TRUE);
				return TRUE;
			}
			break;

		case WM_MDISETMENU:
			if (IsVisible())
			{
				BOOL bAccepMenus = ((m_dwFlags & xtpFlagIgnoreSetMenuMessage) == 0) && (wParam || m_hMenuDefault);

				if (bAccepMenus)
				{
					SwitchMDIMenu(wParam? (HMENU)wParam: m_hMenuDefault);
				}

				GetSite()->SetMenu(0);
				RefreshMenu();

				wParam = 0;
				return TRUE;
			}
			else if (GetSite()->GetMenu() == 0)
			{
				wParam = 0;
				return TRUE;

			}
			break;

		case WM_MDICREATE:
		case WM_MDIDESTROY:
		case WM_MDIACTIVATE:
			PostMessage( WM_TIMER, TID_REFRESH, 0 );
			break;

		case WM_SIZE:
		case WM_WINDOWPOSCHANGED:
			if (hWnd == m_hWndActiveChild)
				PostMessage( WM_TIMER, TID_REFRESH, 0 );

	}
	return CXTPToolBar::OnHookMessage(hWnd, nMessage, wParam, lParam, lResult);
}

BOOL CXTPMenuBar::LoadMenuBar(UINT nIDResource)
{
	ASSERT(m_pControls->GetCount() == 0);

	if (nIDResource != 0)
	{
		CMenu menu;
		if (!menu.LoadMenu(nIDResource))
			return FALSE;

		if (!LoadMenu(&menu))
			return FALSE;

		m_nIDResourceCurrent = m_nIDResource = nIDResource;
	}

	SetupHook();

	UINT nIDMenuBarIcons[] = {SC_RESTORE, SC_MINIMIZE, SC_MAXIMIZE, SC_CLOSE};

	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars)
		pCommandBars->GetImageManager()->SetIcons(XTP_IDB_MENUBAR_ICONS, nIDMenuBarIcons, 4, CSize(16, 16));

	m_hMenuDefault = ::GetMenu(GetSite()->GetSafeHwnd());
	GetSite()->SetMenu(0);

	return TRUE;
}


BEGIN_MESSAGE_MAP(CXTPMenuBar, CXTPToolBar)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CXTPMenuBar::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TID_REFRESH)
		RefreshMenu();

	CXTPToolBar::OnTimer(nIDEvent);
}

void CXTPMenuBar::Reset()
{
	if (((m_mapMDIMenus.GetCount() > 0) || (GetControls()->GetOriginalControls() == NULL)) && m_nIDResourceCurrent)
	{
		CString strMessage, strTitle = GetTitle(), strReset;

		VERIFY(XTPResourceManager()->LoadString(&strReset, XTP_IDS_CONFIRM_RESET));
		strMessage.Format(strReset,  (LPCTSTR)strTitle);

		if (AfxMessageBox(strMessage, MB_ICONWARNING|MB_OKCANCEL) == IDOK)
		{
			CMenu menu;
			VERIFY(menu.LoadMenu(m_nIDResourceCurrent));
			LoadMenu(&menu);
		}
	}
	else if (GetControls()->GetOriginalControls() != NULL)
	{
		CXTPToolBar::Reset();
	}

	RefreshMenu();
	OnInitialUpdate();
	OnRecalcLayout();
}

void CXTPMenuBar::Copy(CXTPCommandBar* pCommandBar, BOOL bRecursive)
{
	CXTPToolBar::Copy(pCommandBar, bRecursive);

	m_nIDResource= ((CXTPMenuBar*)pCommandBar)->m_nIDResource;

	FreeMDIMenus();

	POSITION pos = ((CXTPMenuBar*)pCommandBar)->m_mapMDIMenus.GetStartPosition();
	while (pos)
	{
		UINT nIDCommand;
		CXTPControls* pControls;
		((CXTPMenuBar*)pCommandBar)->m_mapMDIMenus.GetNextAssoc(pos, nIDCommand, pControls);

		m_mapMDIMenus.SetAt(nIDCommand, pControls->Duplicate(TRUE));
	}

}
