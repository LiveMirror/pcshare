// XTPPopupControl.cpp: implementation of the CXTPPopupControl class.
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
#include "XTPImageManager.h"

#include "XTPPopupControl.h"
#include "XTPPopupItem.h"
#include "XTPPopupPaintManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TID_EXPANDING  0x128L
#define TID_COLLAPSING 0x129L
#define TID_SHOWDELAY  0x130L

//this define from new SDK implementation
//not supported by Visual C++ 6.0
#ifndef LWA_ALPHA
#define LWA_ALPHA               0x00000002
#endif
#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED           0x00080000
#endif
typedef BOOL (WINAPI *PFNSETLAYEREDWINDOWATTRIBUTES) (HWND hwnd,COLORREF crKey,BYTE bAlpha,DWORD dwFlags);
//end



/////////////////////////////////////////////////////////////////////////////
// CXTPPopupControl

CXTPPopupControl::CXTPPopupControl()
{
	//set default paint manager
	m_pPaintManager     = new CXTPPopupThemeOffice2000();
	m_paintTheme = xtpPopupThemeOffice2000;

	//init handels and flags for mouse operation
	m_pSelected         = NULL;
	m_pPressed          = NULL;
	m_bCapture          = FALSE;

	//default state popup window
	m_popupAnimation    = xtpPopupAnimationNone;
	m_popupState        = xtpPopupStateClosed;

	//select def transparency value
	m_nTransparency     = 255L;
	m_pfnSetLayeredWindowAttributes = NULL;

	//init animation vars
	m_nAnimationInterval = 16;
	m_uShowDelay         = 5000L;
	m_uAnimationDelay    = 256L;
	m_nStep = 0;

	//popup pos&size init
	m_szPopup           = CSize(170, 130);

	CRect rcDeskWnd;
	//get desctop parameters
	::SystemParametersInfo(SPI_GETWORKAREA,0,&rcDeskWnd,0);
	//set position
	m_ptPopup = rcDeskWnd.BottomRight();


	//init layered function  (for Win98 compatible)
	HMODULE hLib = GetModuleHandle(_T("USER32"));
	if (hLib)
	{
		m_pfnSetLayeredWindowAttributes = (PVOID) ::GetProcAddress(hLib, "SetLayeredWindowAttributes");
	}

	m_bAllowMove = FALSE;
	m_pImageManager = new CXTPImageManager;

}

CXTPPopupControl::~CXTPPopupControl()
{
	//Destroy CWnd object
	Close();

	//clear all items
	RemoveAllItems();

	//delete paint manager
	if (m_pPaintManager)
		delete m_pPaintManager;

	if (m_pImageManager)
		m_pImageManager->InternalRelease();

}

BOOL CXTPPopupControl::Create(CWnd *pParentWnd)
{
	//if hwnd already exist - return TRUE;
	if (GetSafeHwnd())
		return TRUE;

	//init extended wnd style (for Win98 compatible)
	DWORD dwExStyle = m_pfnSetLayeredWindowAttributes &&
		(m_nTransparency < 255 || m_popupAnimation == xtpPopupAnimationFade)? WS_EX_LAYERED: 0;

	//Create popup Wnd
	if (!CreateEx(dwExStyle | WS_EX_TOOLWINDOW|WS_EX_TOPMOST,
		AfxRegisterWndClass(NULL, ::LoadCursor(NULL, IDC_ARROW)),
		NULL, WS_POPUP, CRect(0, 0, 0, 0), pParentWnd, NULL))
		return FALSE;



	//Set begining state for creating window
	m_popupState = xtpPopupStateClosed;

	return TRUE;
}

void CXTPPopupControl::SetTheme(CXTPPopupPaintManager* pPaintManager)
{
	//delete old theme object
	delete m_pPaintManager;

	//store point to new theme object
	m_pPaintManager = pPaintManager;

	//redraw all controls
	RedrawControl();
}

void CXTPPopupControl::SetTheme(XTPPopupPaintTheme theme)
{
	//set new theme
	switch(theme)
	{
		case xtpPopupThemeMSN:
			SetTheme(new CXTPPopupThemeMSN());
			break;
		case xtpPopupThemeOffice2003:
			SetTheme(new CXTPPopupThemeOffice2003());
			break;
		case xtpPopupThemeOfficeXP:
			SetTheme(new CXTPPopupThemeOfficeXP());
			break;
		case xtpPopupThemeOffice2000:
			SetTheme(new CXTPPopupThemeOffice2000());
			break;
		default:
			//error case!!!
			SetTheme(new CXTPPopupPaintManager());
	}
	m_paintTheme = theme;
}

void CXTPPopupControl::RedrawControl()
{
	if(m_hWnd)
		//call WM_PAINT message
		Invalidate(FALSE);
}

CXTPPopupItem* CXTPPopupControl::AddItem(CXTPPopupItem* pItem)
{
	//inser item to item's queue
	pItem->m_nIndex = (int)m_arrItems.Add(pItem);

	//init control handler
	pItem->m_pControl = this;

	//notify to item about adding inside CXTPPopupControl
	pItem->OnItemInserted();

	return pItem;
}

void CXTPPopupControl::RemoveAllItems()
{
	//dealocate memory for all items
	for(int i = 0; i < GetItemCount(); i++)
		m_arrItems[i]->InternalRelease();

	//clear item's array
	m_arrItems.RemoveAll();

	m_pImageManager->RemoveAll();

	//reset selected and pressed pointers
	m_pSelected = NULL;
	m_pPressed = NULL;
}

void CXTPPopupControl::RemoveItem(CXTPPopupItem * pItem)
{
	ASSERT(pItem);

	CXTPPopupItem *pCurrItem = NULL;

	//find item pointer in item's queue
	for(int i = 0; i < GetItemCount(); i++)
	{
		pCurrItem = GetItem(i);

		//if poitters equal
		if(pCurrItem == pItem && pCurrItem)
		{
			//deallocate memory
			m_arrItems[i]->InternalRelease();

			//remove pointer from item's queue
			m_arrItems.RemoveAt(i);

			//if pointer was selected - reset selected pointer
			if(m_pSelected == pCurrItem)
				m_pSelected = NULL;

			//if pointer was pressed - reset pressed pointer
			if(m_pPressed == pCurrItem)
				m_pPressed = NULL;

			//redraw all valid items
			RedrawControl();

			break;
		}
	}
}

CXTPPopupItem* CXTPPopupControl::GetItem(int nIndex)
{
	//return item on an index
	return m_arrItems[nIndex];
}

int CXTPPopupControl::GetItemCount()
{
	//return count of valid items
	return (int)m_arrItems.GetSize();
}

CXTPPopupItem* CXTPPopupControl::HitTest(CPoint pt)
{
	//look over a item's pointers in item's queue
	for(int i = GetItemCount() - 1; i >= 0; i--)
	{
		CXTPPopupItem* pItem = GetItem(i);

		//test item rect to XY location
		if(pItem->GetRect().PtInRect(pt))
			//if OK return pointer
			return pItem;
	}
	return NULL;
}

BOOL CXTPPopupControl::SetLayeredWindowAttributes(BYTE bAlpha)
{
	//set memer transparent function for compatible with Win98
	if(m_pfnSetLayeredWindowAttributes)
	{
		//if pointer to transparent func - valid
		return ((PFNSETLAYEREDWINDOWATTRIBUTES)m_pfnSetLayeredWindowAttributes)
			(m_hWnd, 0x00, bAlpha, LWA_ALPHA);
	}
	return FALSE;
}


XTPPopupState CXTPPopupControl::GetPopupState()
{
	//return current popup state
	return m_popupState;
}

void CXTPPopupControl::SetPopupState(XTPPopupState popupState)
{
	//set new popup state
	m_popupState = popupState;

	//if popup object is destroyed
	if (!m_hWnd)
		return;

	//else if CWnd object exist
	// - notify to parent window about change state
	Notify(XTP_PCN_STATECHANGED, (LPARAM)this);
}


void CXTPPopupControl::UpdateState(BOOL bInit)
{
	//Get current popup wnd rect
	CRect rc = m_stateCurrent.rcPopup;

	//set current pos and size
	SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(),
		SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE |
		SWP_NOSENDCHANGING | SWP_SHOWWINDOW);

	//redraw all items
	RedrawControl();

	//if mouse is capture - window is opaque (return from UpdateState proc)
	//else, will be need to set current transparent value
	if (!m_bCapture)
		//if transparent wiil be change ore init state - set transparent value
		if (m_stateCurrent.nTransparency != m_stateTarget.nTransparency || bInit)
			SetLayeredWindowAttributes((BYTE)m_stateCurrent.nTransparency);

	UpdateWindow();
}

BOOL CXTPPopupControl::Close()
{
	//set close state
	SetPopupState(xtpPopupStateClosed);

	//reset capture flag
	m_bCapture = FALSE;

	//destroy m_hWnd object
	BOOL bResult = DestroyWindow();

	//reset selected and pressed state
	m_pSelected = m_pPressed = NULL;

	return bResult;
}

void CXTPPopupControl::Hide()
{
	//if popup state "Show" - collapsing window
	if (m_popupState == xtpPopupStateShow)
	{
		KillTimer(TID_SHOWDELAY);
		OnCollapsing();
	}
}

void CXTPPopupControl::OnShow()
{
	//set SHOW state
	SetPopupState(xtpPopupStateShow);

	m_stateCurrent.rcPopup = CRect(CPoint(m_ptPopup.x - m_szPopup.cx, m_ptPopup.y - m_szPopup.cy), m_szPopup);
	m_stateCurrent.nTransparency = m_nTransparency;


	//start SHOW timer
	if (m_uShowDelay > 0)
	{
		SetTimer(TID_SHOWDELAY, m_uShowDelay, NULL);
	}
}

void CXTPPopupControl::OnCollapsing()
{
	//set COLLAPSING state
	SetPopupState(xtpPopupStateCollapsing);

	//if no-animation mode
	if(m_popupAnimation == xtpPopupAnimationNone || m_uAnimationDelay <= 0)
	{
		//close & destroy popup window
		Close();
		return;
	}
	//if Fage animation mode
	else if(m_popupAnimation == xtpPopupAnimationFade)
	{
		//set target transparensy value
		m_stateTarget.nTransparency = 0;
	}
	//if Slide animation mode
	else if(m_popupAnimation == xtpPopupAnimationSlide)
	{
		//set rectangular of target
		m_stateTarget.rcPopup = CRect(m_ptPopup.x - m_szPopup.cx, m_ptPopup.y, m_ptPopup.x, m_ptPopup.y);
	}
	//if Unfold animation mode
	else if(m_popupAnimation == xtpPopupAnimationUnfold)
	{
		//set rectangular of target
		m_stateTarget.rcPopup = CRect(m_ptPopup, CSize(0));
	}

	//calc step
	m_nStep = max(1, m_uAnimationDelay/m_nAnimationInterval);

	//set collapsing timer
	SetTimer(TID_COLLAPSING, m_nAnimationInterval, NULL);

	//update view state
	UpdateState(TRUE);
}

void CXTPPopupControl::OnExpanding(BOOL bUpdateCurrent)
{
	//set Expanding state
	SetPopupState(xtpPopupStateExpanding);

	//reinit target rect
	m_stateTarget.rcPopup = CRect(CPoint(m_ptPopup.x - m_szPopup.cx, m_ptPopup.y - m_szPopup.cy), m_szPopup);
	m_stateTarget.nTransparency = m_nTransparency;

	//if updating flag is set
	if (bUpdateCurrent)
	{
		//reinit curent state object
		m_stateCurrent = m_stateTarget;

		//if no-animation mode
		if(m_popupAnimation == xtpPopupAnimationNone || m_uAnimationDelay <= 0)
		{
			//update view
			UpdateState(TRUE);
			//show popup
			OnShow();
			//return from proc
			return;
		}
		//if Fage animation mode
		else if(m_popupAnimation == xtpPopupAnimationFade)
		{
			//set target transparensy value
			m_stateCurrent.nTransparency = 0;
		}
		//if Slide animation mode
		if(m_popupAnimation == xtpPopupAnimationSlide)
		{
			//set rectangular of target
			m_stateCurrent.rcPopup = CRect(m_ptPopup.x - m_szPopup.cx, m_ptPopup.y, m_ptPopup.x, m_ptPopup.y);
		}
		//if Unfold animation mode
		else if(m_popupAnimation == xtpPopupAnimationUnfold)
		{
			//set rectangular of target
			m_stateCurrent.rcPopup = CRect(m_ptPopup, CSize(0));
		}
		//calc step
		m_nStep = max(1, m_uAnimationDelay/m_nAnimationInterval);
	}
	else
	{
		//calc step
		m_nStep = max(1, m_uAnimationDelay/m_nAnimationInterval - m_nStep);
	}

	//set expanding timer
	SetTimer(TID_EXPANDING, m_nAnimationInterval, NULL);

	//update view state
	UpdateState(TRUE);
}

BOOL CXTPPopupControl::Show(CWnd* pParent)
{
	//create popup wnd
	if (!Create(pParent))
		return FALSE;

	//check popup state - return if popup state is wrong
	if (m_popupState != xtpPopupStateClosed)
		return FALSE;


	//set expanding
	OnExpanding(TRUE);

	return TRUE;
}

#define MOVETO(A, B, Step) if (A != B) A += max(1, abs(A - B)/Step) * (A > B? -1: 1);

void CXTPPopupControl::Animate(int nStep)
{
	// if step == 0 set current state to target state
	if(nStep < 1)
	{
		m_stateCurrent = m_stateTarget;
	}
	else
	{
		//move
		MOVETO(m_stateCurrent.rcPopup.top, m_stateTarget.rcPopup.top, nStep);
		MOVETO(m_stateCurrent.rcPopup.left, m_stateTarget.rcPopup.left, nStep);
		MOVETO(m_stateCurrent.rcPopup.right, m_stateTarget.rcPopup.right, nStep);
		MOVETO(m_stateCurrent.rcPopup.bottom, m_stateTarget.rcPopup.bottom, nStep);
		MOVETO(m_stateCurrent.nTransparency, m_stateTarget.nTransparency, nStep);
	}
	//update view state
	UpdateState();
}


BEGIN_MESSAGE_MAP(CXTPPopupControl, CWnd)
	//{{AFX_MSG_MAP(CXTPPopupControl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CXTPPopupControl message handlers

int CXTPPopupControl::OnMouseActivate(CWnd* /*pDesktopWnd*/, UINT /*nHitTest*/, UINT /*message*/)
{
	return MA_NOACTIVATE;
}

void CXTPPopupControl::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		//if expand ore collapsing state
		case TID_EXPANDING:
		case TID_COLLAPSING:
			//animate from current to target
			Animate(m_nStep);
			m_nStep--;

			//if end step
			if (m_nStep <= 0)
			{
				//kill timer event
				KillTimer(nIDEvent);

				//change popup state
				if (nIDEvent == TID_EXPANDING)
					OnShow();
				else
					Close();
			}

			break;

		//if popup wnd shown
		case TID_SHOWDELAY:
			//if mouse cursor is not capture
			if (!m_bCapture)
			{
				//kill timer event
				KillTimer(TID_SHOWDELAY);
				//set collapsing state
				OnCollapsing();
			}
			break;

	}

	CWnd::OnTimer(nIDEvent);
}


BOOL CXTPPopupControl::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPPopupControl::OnPaint()
{
	//Get context CDC object
	CPaintDC dcPaint(this);

	//init client rect
	CRect rc(0, 0, m_szPopup.cx, m_szPopup.cy);
	//init temp buffer CDC object
	CXTPBufferDC dc(dcPaint, rc);

	//draw background
	m_pPaintManager->DrawBackground(&dc, rc);

	//draw all valid items
	for(int i = 0; i < GetItemCount(); i++)
	{
		CXTPPopupItem* pItem = GetItem(i);
		pItem->Draw(&dc);
	}
}

void CXTPPopupControl::Notify(WPARAM wParam, LPARAM lParam)
{
	//get parent CWnd object
	CWnd * pParent = GetOwner();
	if(pParent)
		//send message to paprent
		pParent->SendMessage(XTPWM_POPUPCONTROL_NOTIFY, wParam, lParam);

}

void CXTPPopupControl::OnClick(CXTPPopupItem* pItem)
{
	Notify(XTP_PCN_ITEMCLICK, (LPARAM)pItem);
}

void CXTPPopupControl::TrackMove()
{
	SetCapture();

	CPoint ptStart;
	GetCursorPos(&ptStart);

	CXTPWindowRect rcStart(this);

	while (GetCapture() == this)
	{
		MSG msg;

		while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
				break;
			DispatchMessage(&msg);
		}

		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage((int)msg.wParam);
			break;
		}

		if (msg.message == WM_LBUTTONUP) break;
		else if (msg.message == WM_MOUSEMOVE)
		{
			CPoint pt(msg.pt);
			CRect rc(rcStart);
			rc.OffsetRect(pt - ptStart);

			CRect rcDeskWnd = CXTPDrawHelpers::GetWorkArea();

			if (rc.left < rcDeskWnd.left) rc.OffsetRect(rcDeskWnd.left - rc.left, 0);
			if (rc.top < rcDeskWnd.top) rc.OffsetRect(0, rcDeskWnd.top - rc.top);
			if (rc.right > rcDeskWnd.right) rc.OffsetRect(rcDeskWnd.right - rc.right,0);
			if (rc.bottom > rcDeskWnd.bottom) rc.OffsetRect(0, rcDeskWnd.bottom - rc.bottom);

			MoveWindow(rc);
		}
		else if (msg.message == WM_KEYDOWN)
		{
			if (msg.wParam == VK_ESCAPE)
				break;
		} else
			DispatchMessage(&msg);

	}
	ReleaseCapture();


	m_stateTarget.rcPopup = m_stateCurrent.rcPopup = CXTPWindowRect(this);
	m_ptPopup = m_stateCurrent.rcPopup.BottomRight();


}

void CXTPPopupControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	//test point to pressed controll
	CXTPPopupItem* pPressed = HitTest(point);

	if (m_bAllowMove && (!pPressed || (!pPressed->GetID() && !pPressed->IsButton() && (pPressed->GetCaption().IsEmpty() || !pPressed->IsHyperLink()))))
	{
		TrackMove();
		return;
	}

	//if success test
	if(pPressed)
	{
		m_pPressed = pPressed;
		//set capture
		SetCapture();
		//redraw all valide controls
		RedrawControl();
	}


	CWnd::OnLButtonDown(nFlags, point);
}

void CXTPPopupControl::OnLButtonUp(UINT nFlags, CPoint point)
{
	//if there is pressed control
	if (m_pPressed)
	{
		//store popup pointer
		CXTPPopupItem* pPressed = m_pPressed;
		m_pPressed = NULL;

		//free mouse event
		ReleaseCapture();

		//if selected pointer equal pressed pointer - it is clik on item
		if(pPressed == m_pSelected)
		{
			OnClick(pPressed);
		}
		//else reset mouse vars
		else
		{
			OnMouseMove(0,point);
		}

		//redraw all valid items
		RedrawControl();
	}


	CWnd::OnLButtonUp(nFlags, point);
}

void CXTPPopupControl::OnCaptureChanged(CWnd *pWnd)
{
	//if m_pPressed - reset pointer
	if (m_pPressed)
	{
		m_pPressed = NULL;
		RedrawControl();
	}

	CWnd::OnCaptureChanged(pWnd);
}

void CXTPPopupControl::OnMouseMove(UINT nFlags, CPoint point)
{
	CXTPClientRect rc(this);

	//test client rect if no-pressed
	BOOL bInRect = rc.PtInRect(point) || m_pPressed != NULL;

	//if test successfull and already not capture
	if(bInRect && !m_bCapture)
	{
		//set capture
		m_bCapture = TRUE;

		//opaque window
		SetLayeredWindowAttributes(255);

		//capture mouse leave event
		TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, 0};
		_TrackMouseEvent(&tme);
	}
	//else if test fail and there is pressed and selected control
	if(!bInRect && m_bCapture && m_pPressed == NULL)
	{
		//free capture
		m_bCapture = FALSE;
		//set current transparent
		SetLayeredWindowAttributes((BYTE)m_nTransparency);
	}

	//if collapsing state - expand popup window
	if(m_popupState == xtpPopupStateCollapsing)
	{
		//kill collapsing timer
		KillTimer(TID_COLLAPSING);

		if (m_popupAnimation == xtpPopupAnimationFade)
		{
			OnShow();
		}
		else
		{
			OnExpanding(FALSE);
		}
	}

	//test point to controled items
	CXTPPopupItem* pSelected = HitTest(point);

	//if detect new selected item ore lose selection (NULL)
	if(pSelected != m_pSelected)
	{
		//select new item ore set NULL
		m_pSelected = (m_pPressed == 0 || m_pPressed == pSelected || pSelected == NULL)? pSelected: NULL;

		//redraw all items
		RedrawControl();
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CXTPPopupControl::OnMouseLeave()
{
	//reset mouse vars
	OnMouseMove(0,CPoint(-1, -1));
}

CXTPImageManager* CXTPPopupControl::GetImageManager()
{
	return m_pImageManager;
}

void CXTPPopupControl::SetImageManager(CXTPImageManager* pImageManager)
{
	if (m_pImageManager)
		m_pImageManager->InternalRelease();

	m_pImageManager = pImageManager;

	RedrawControl();
}
