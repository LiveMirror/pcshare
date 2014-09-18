// XTPMouseManager.cpp : implementation of the CXTPMouseManager class.
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
#include "XTPDrawHelpers.h"

#include "XTPMouseManager.h"
#include "XTPCommandBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CThreadLocal<CXTPMouseManager> CXTPMouseManager::_xtpMouseThreadState;

#define TID_MOUSELEAVE 113341

CXTPMouseManager::CXTPMouseManager(void)
{
	m_hHookMouse = 0;
	m_nLock = 0;
	m_hwndLeave = 0;
	m_bForceExpanded = FALSE;
	m_pSelected = 0;
	m_bIgnoreLButtonUp = FALSE;

#ifdef _AFXDLL
	m_pModuleState = 0;
#endif
	SetupHook(TRUE);
}

CXTPMouseManager::~CXTPMouseManager(void)
{
	SetupHook(FALSE);
}

void CXTPMouseManager::DeliverMessage(CXTPCommandBar* pCapture, WPARAM wParam, POINT pt)
{
	if (!pCapture->GetSafeHwnd())
		return;

	switch(wParam)
	{
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			pCapture->ScreenToClient(&pt);
			pCapture->OnMouseMove(0, pt);
			break;

		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_NCMBUTTONDOWN:
			pCapture->OnTrackLost();
			break;
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			if (!IsMouseLocked())
				pCapture->OnTrackLost();
	}
}



BOOL CXTPMouseManager::PreTranslateMouseEvents(WPARAM wParam, POINT point)
{
	CWnd* pWndMouse = CWnd::WindowFromPoint(point);
	CTrackArray& arrTracked = m_arrTracked;

	for (int i = 0; i < arrTracked.GetSize(); i++)
	{
		CXTPCommandBar* pCommandBar = arrTracked[i];

		if (!pCommandBar->GetSafeHwnd())
			continue;

		int nHtCode = pCommandBar->OnMouseHitTest(point);

		if ((nHtCode != HTERROR) && ((nHtCode == HTCLIENT) || pCommandBar->IsChild(pWndMouse)))
		{
			return FALSE;
		}
	}

	if (arrTracked.GetSize() >0 && arrTracked[0]->IsCustomizeMode())
	{
		CTrustedArray& arrTrusted = m_arrTrusted;
		for (int i = 0; i < arrTrusted.GetSize(); i++)
		{
			if (IsWindow(arrTrusted[i]) && CXTPWindowRect(arrTrusted[i]).PtInRect(point))
			{
				return FALSE;
			}
		}

	}

	for (int j = 0; j < arrTracked.GetSize(); j++)
	{
		DeliverMessage(arrTracked.GetAt(j), wParam, point);
	}

	return FALSE;

}




LRESULT CALLBACK CXTPMouseManager::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	CXTPMouseManager* pMouseManager = XTPMouseManager();

	if (nCode != HC_ACTION)
		return CallNextHookEx(pMouseManager->m_hHookMouse, nCode, wParam, lParam);

	PMOUSEHOOKSTRUCT pHook = (PMOUSEHOOKSTRUCT)lParam;
	CPoint point = pHook->pt;

	static CPoint ptMouse;
	CTrackArray& arrTracked = pMouseManager->m_arrTracked;

	if (wParam == WM_LBUTTONDOWN)
		pMouseManager->m_bIgnoreLButtonUp = FALSE;

	if (wParam == WM_LBUTTONUP && pMouseManager->m_bIgnoreLButtonUp)
	{
		pMouseManager->m_bIgnoreLButtonUp = FALSE;
		return TRUE;
	}

	if (arrTracked.GetSize() != 0 && (wParam == WM_MOUSEMOVE || wParam == WM_NCMOUSEMOVE) && ptMouse == point)
		return TRUE;

	ptMouse = point;

	if (pMouseManager->m_arrTracked.GetSize() == 0)
		return CallNextHookEx(pMouseManager->m_hHookMouse, nCode, wParam, lParam);

	SAFE_MANAGE_STATE(pMouseManager->m_pModuleState);

	if (pMouseManager->PreTranslateMouseEvents(wParam, point))
	{
		return 1;
	}

	return CallNextHookEx(pMouseManager->m_hHookMouse, nCode, wParam, lParam);

}

void CXTPMouseManager::SetupHook(BOOL bHook)
{
	if (bHook && m_hHookMouse == 0)
	{
		m_hHookMouse = SetWindowsHookEx(WH_MOUSE, MouseProc, 0, GetCurrentThreadId ());
	}
	if (!bHook && m_hHookMouse)
	{
		UnhookWindowsHookEx(m_hHookMouse);
		m_hHookMouse = 0;
	}
}

void CXTPMouseManager::SetTrack(CXTPCommandBar* pTrack, BOOL /*bCaptureMouse*/)
{
#ifdef _AFXDLL
	m_pModuleState = AfxGetModuleState( );
#endif

	if (m_arrTracked.GetSize() == 0 && m_pSelected && m_pSelected != pTrack)
		m_pSelected->OnTrackEnter();

	if (m_arrTracked.Find(pTrack) == -1)
		m_arrTracked.Add(pTrack);
}
void CXTPMouseManager::RemoveTrack(CXTPCommandBar* pTrack)
{
	int nIndex = m_arrTracked.Find(pTrack);
	if (nIndex != -1)
	{
		m_arrTracked.RemoveAt(nIndex);
	}
	if (m_arrTracked.GetSize() == 0)
	{
		CPoint pt;
		GetCursorPos(&pt);
		SetCursorPos(pt.x, pt.y);
	}
	m_bIgnoreLButtonUp = FALSE;
}


void CXTPMouseManager::SendTrackLost()
{
	if (m_arrTracked.GetSize() > 0)
		m_arrTracked[0]->OnTrackLost();
}

BOOL CXTPMouseManager::IsTopParentActive(HWND hWnd)
{
	HWND hwndForeground = ::GetForegroundWindow();

	HWND hWndT;
	while ((hWndT = AfxGetParentOwner(hWnd)) != NULL)
	{
		hWnd = hWndT;
	}

	return hwndForeground ==
		::GetLastActivePopup(hWnd);
}

void CALLBACK CXTPMouseManager::TrackMouseTimerProc (HWND hWnd, UINT /*uMsg*/, UINT idEvent, DWORD /*dwTime*/)
{
	RECT    rect;
	POINT   pt;

	if (!IsWindow(hWnd))
	{
		KillTimer (hWnd, idEvent);
		XTPMouseManager()->m_hwndLeave = 0;
		return;
	}
	GetWindowRect(hWnd, &rect);
	::GetCursorPos (&pt);

	BOOL bTopParentActive = GetParent(hWnd) == 0 || IsTopParentActive(hWnd);
	if (!::PtInRect (&rect, pt) || !bTopParentActive)
	{
		KillTimer (hWnd, idEvent);
		XTPMouseManager()->m_hwndLeave = 0;

		::PostMessage (hWnd,WM_MOUSELEAVE, 0, 0);
	}


}

void CXTPMouseManager::TrackMouseLeave(HWND hwnd)
{
	if (m_hwndLeave == hwnd) return;
	if (m_hwndLeave && hwnd != m_hwndLeave && IsWindow(m_hwndLeave))
		SendMessage(m_hwndLeave, WM_MOUSELEAVE, 0, 0);
	m_hwndLeave = hwnd;
	::SetTimer (hwnd, TID_MOUSELEAVE, 50,
			(TIMERPROC) TrackMouseTimerProc);
}


void CXTPMouseManager::AddTrustedWindow(HWND hWnd)
{
	m_arrTrusted.Add(hWnd);
}
void CXTPMouseManager::RemoveTrustedWindow(HWND hWnd)
{
	m_arrTrusted.Remove(hWnd);
}
