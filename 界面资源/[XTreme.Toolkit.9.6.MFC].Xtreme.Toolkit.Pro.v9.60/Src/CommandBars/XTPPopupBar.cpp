// XTPPopupBar.cpp : implementation of the CXTPPopupBar class.
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
#include "XTPDrawHelpers.h"
#include "XTPColorManager.h"

#include "XTPPopupBar.h"
#include "XTPPaintManager.h"
#include "XTPControls.h"
#include "XTPControl.h"
#include "XTPControlButton.h"
#include "XTPShortcutManager.h"
#include "XTPControlPopup.h"
#include "XTPShadowsManager.h"
#include "XTPCommandBarsResource.h"
#include "XTPToolBar.h"
#include "XTPCommandBars.h"
#include "XTPMouseManager.h"
#include "XTPSoundManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

double CXTPPopupBar::m_dMaxWidthDivisor = 1.0/3.0;



#define TID_HOVERUP 5001
#define TID_HOVERDOWN 5002
#define TID_TEAROFFSELECTED 5003


class CXTPPopupBar::CXTPControlExpandButton: public CXTPControlButton
{
public:
	CXTPControlExpandButton()
	{
		m_controlType = xtpControlButton;
		m_dwFlags = xtpFlagManualUpdate | xtpFlagNoMovable | xtpFlagSkipFocus;
	}
	void Draw(CDC* pDC);
	virtual void OnExecute();
	virtual void OnMouseHover();
};

void CXTPPopupBar::CXTPControlExpandButton::Draw(CDC* pDC)
{
	GetPaintManager()->DrawSpecialControl(pDC, xtpButtonExpandMenu, this, m_pParent, TRUE, NULL);
}

void CXTPPopupBar::CXTPControlExpandButton::OnExecute()
{
	((CXTPPopupBar*)m_pParent)->ExpandBar();
}

void CXTPPopupBar::CXTPControlExpandButton::OnMouseHover()
{
	CXTPCommandBars* pCommandBars = m_pParent->GetCommandBars();
	ASSERT(pCommandBars);

	if (pCommandBars->GetCommandBarsOptions()->bShowFullAfterDelay)
	{
		OnExecute();
	}
}


IMPLEMENT_XTP_COMMANDBAR(CXTPPopupBar, CXTPCommandBar)

CXTPPopupBar::CXTPPopupBar(void)
{
	m_scrollInfo.Init(this, TID_HOVERUP, TID_HOVERDOWN);

	m_barType = xtpBarTypePopup;
	m_barPosition = xtpBarPopup;

	m_pControlPopup = 0;
	m_ptPopup = 0;
	m_rcExclude.SetRectEmpty();
	m_popupFlags = xtpPopupRight;
	m_bDynamicLayout = FALSE;

	m_bTearOffPopup = FALSE;
	m_bTearOffTracking = FALSE;
	m_nTearOffID = 0;
	m_nTearOffWidth = 0;

	m_bShowShadow = TRUE;
	m_rcBorders = CRect(2, 4, 2, 4);
	m_bExpanding = FALSE;

	m_nMRUWidth = 0;
	m_bCustomizeMenu = FALSE;

}

CXTPPopupBar* CXTPPopupBar::CreatePopupBar(CXTPCommandBars* pCommandBars)
{
	CXTPPopupBar* pPopupBar = (CXTPPopupBar*)CXTPCommandBars::m_pPopupBarClass->CreateObject();
	pPopupBar->SetCommandBars(pCommandBars);
	return pPopupBar;
}

CXTPPopupBar::~CXTPPopupBar(void)
{
}

void CXTPPopupBar::SetTearOffPopup(CString strCaption, UINT nID, int nWidth)
{
	if (nID == 0)
	{
		m_bTearOffPopup = FALSE;
	}
	else
	{
		m_bTearOffPopup = TRUE;
		m_strTearOffCaption = strCaption;
		m_nTearOffID = nID;
		m_nTearOffWidth = nWidth;
	}
}
BOOL CXTPPopupBar::IsTearOffPopup(CString& strCaption, UINT& nID, int& nWidth)
{
	strCaption = m_strTearOffCaption;
	nID = m_nTearOffID;
	nWidth = m_nTearOffWidth;

	return m_bTearOffPopup;
}

BOOL CXTPPopupBar::IsVisible()
{
	return (m_hWnd != 0) && IsWindowVisible();
}

void CXTPPopupBar::ExpandBar()
{
	if (m_bExpanded == FALSE && m_bCollapsed == TRUE)
	{
		m_bExpanding = TRUE;
		m_bExpanded = TRUE;
		SetPopuped(-1);
		SetSelected(-1);
		OnRecalcLayout();
		XTPMouseManager()->SetForceExpanded(TRUE);
		XTPMouseManager()->IgnoreLButtonUp();
	}
}

BOOL CXTPPopupBar::Create()
{
	if (m_hWnd)
		return TRUE;

	CString strPopupClass = AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(0, IDC_ARROW));

	int nLayoutRTL = GetSite()->GetExStyle() & WS_EX_LAYOUTRTL;

	if(!CreateEx(WS_EX_TOOLWINDOW|nLayoutRTL, strPopupClass, 0, WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0,0,0,0), GetDesktopWindow(), 0))
		return FALSE;

	SetOwner(GetSite());

	return TRUE;
}

#ifndef SPI_GETMENUANIMATION
#define SPI_GETMENUANIMATION                0x1002
#endif

#ifndef SPI_GETMENUFADE
#define SPI_GETMENUFADE                     0x1012
#endif


XTPAnimationType CXTPPopupBar::GetAnimationType()
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars)
	{
		if (GetWindow(GW_CHILD))
			return xtpAnimateNone;

		XTPAnimationType animationType = pCommandBars->GetCommandBarsOptions()->animationType;

		switch(animationType)
		{
			case xtpAnimateWindowsDefault:
			{
				BOOL bEnabled = FALSE;
				if (!SystemParametersInfo(SPI_GETMENUANIMATION, 0,  &bEnabled, 0))
					return xtpAnimateNone;

				if (!bEnabled)
					return xtpAnimateNone;

				BOOL bFadeAnimation = FALSE;

				if (!SystemParametersInfo(SPI_GETMENUFADE, 0,  &bFadeAnimation, 0))
					return xtpAnimateSlide;

				animationType = bFadeAnimation? xtpAnimateFade: xtpAnimateSlide;
			}
			break;

			// choose any animation based on a random number
			case xtpAnimateRandom:
			{
				switch(RAND_S() % 3)
				{
					case 0: animationType = xtpAnimateFade; break;
					case 1: animationType = xtpAnimateSlide; break;
					default: animationType = xtpAnimateUnfold; break;
				}
			}
			break;
		}

		return animationType;
	}
	return xtpAnimateNone;

}

void CXTPPopupBar::OnControlsChanged()
{

	if (!m_bDynamicLayout && GetSafeHwnd() && IsTrackingMode())
	{
		CXTPClientRect rcClientPre(this);

		XTPShadowsManager()->RemoveShadow(this);

		UpdateShortcuts();
		UpdateFlags();

		UpdateLocation(CalcDynamicLayout(0, 0));

		if (m_bExpanding)
		{
			XTPAnimationType animationType = GetAnimationType();
			if (IsAnimateType(animationType))
			{
				CWindowDC paintDC(this);
				CXTPClientRect rcClientPost(this);

				CXTPBufferDC animationDC(paintDC, rcClientPost);
				DrawCommandBar(&animationDC);

				// Animation
				GetPaintManager()->AnimateExpanding(this, &paintDC, &animationDC);
			}
			XTPSoundManager()->PlaySystemSound(xtpSoundMenuPopup);
		}

		if (m_barPosition == xtpBarPopup && m_bShowShadow)
			XTPShadowsManager()->SetShadow(this, m_rcExclude);

		if (m_pControlPopup)
			m_pControlPopup->UpdateShadow();

		Redraw();

		m_bExpanding = FALSE;
	}

}

void CXTPPopupBar::OnRecalcLayout()
{
	OnControlsChanged();
}

CSize CXTPPopupBar::CalcDynamicLayout(int, DWORD /*nMode*/)
{
	m_bDynamicLayout = TRUE;

	CClientDC dc(this);
	CXTPFontDC font(&dc, GetPaintManager()->GetCommandBarFont(this));

	if (m_barType == xtpBarTypeNormal)
	{
		CRect rcBorders(m_rcBorders);
		CSize szTearOffGripper(0);
		if (m_bTearOffPopup)
		{
			szTearOffGripper = GetPaintManager()->DrawTearOffGripper(&dc, 0, FALSE, FALSE);
			rcBorders.top += szTearOffGripper.cy;
		}

		UpdateShortcuts();

		CSize sz = m_pControls->CalcDynamicSize(&dc, 0, LM_COMMIT | LM_HORZ | LM_POPUP, rcBorders);

		m_rcTearOffGripper.SetRect(rcBorders.left, rcBorders.top - szTearOffGripper.cy, sz.cx - rcBorders.left, rcBorders.top);

		m_bDynamicLayout = FALSE;

		return sz;
	}


	RECT rcWork;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcWork, 0);
	CRect rcBorders = GetPaintManager()->GetCommandBarBorders(this);

	int nSeparator = GetPaintManager()->DrawCommandBarSeparator(NULL, this, NULL, FALSE).cy;

	CSize szTearOffGripper(0);

	if (m_bTearOffPopup)
	{
		szTearOffGripper = GetPaintManager()->DrawTearOffGripper(&dc, 0, FALSE, FALSE);
		rcBorders.top += szTearOffGripper.cy;
	}

	foreach (pControl, FALSE)
	{
		pControl->OnCalcDynamicSize(0);
	}

	UpdateExpandingState();


	CXTPControl* pButtonExpand = m_pControls->FindControl(XTP_ID_POPUPBAR_EXPAND);
	CSize szButtonExpand(0);
	if (m_bCollapsed)
	{
		szButtonExpand = GetPaintManager()->DrawSpecialControl(&dc, xtpButtonExpandMenu, NULL, this, FALSE, NULL);
		rcBorders.bottom += szButtonExpand.cy;

		if (!pButtonExpand)
			pButtonExpand = m_pControls->Add(new CXTPControlExpandButton(), XTP_ID_POPUPBAR_EXPAND, _T(""), -1, TRUE);
		pButtonExpand->SetHideFlags(xtpHideGeneric);
	}
	else if (pButtonExpand) pButtonExpand->SetHideFlags(xtpHideGeneric);

	CRect rcControl;
	int yPos = rcBorders.top, nWidth = 0;


	UpdateShortcuts();


	BOOL bFirst = TRUE;
	foreach (pControl, FALSE)
	{
		pControl->SetHideFlags(pControl->GetHideFlags() & ~xtpHideScroll);
		if (!pControl->IsVisible())
			continue;

		if (pControl->GetBeginGroup() && !bFirst)
			yPos += nSeparator;

		bFirst = FALSE;

		CXTPFontDC font(&dc, GetPaintManager()->GetCommandBarFont(this, pControl->IsItemDefault()));

		CSize szControl = pControl->GetSize(&dc);

		if (m_dMaxWidthDivisor > 0 && szControl.cx > (m_dMaxWidthDivisor * rcWork.right))
			szControl.cx = int(m_dMaxWidthDivisor * rcWork.right);

		pControl->SetRect(CRect(rcBorders.left, yPos, rcBorders.left + szControl.cx, yPos + szControl.cy));
		yPos += szControl.cy;
		pControl->SetWrap(FALSE);

		if (nWidth < szControl.cx) nWidth = szControl.cx;
	}

	foreach (pControl, TRUE)
	{
		CRect rc = pControl->GetRect();
		pControl->SetRect(CRect(rc.left, rc.top, nWidth + rcBorders.left, rc.bottom));
	}

	if (m_bCollapsed)
	{
		pButtonExpand->SetHideFlags(xtpNoHide);
		pButtonExpand->SetRect(CRect(rcBorders.left, yPos, nWidth + rcBorders.left, yPos + szButtonExpand.cy));
	}

	m_rcTearOffGripper.SetRect(rcBorders.left, rcBorders.top - szTearOffGripper.cy, nWidth + rcBorders.left, rcBorders.top);

	nWidth = max(22, nWidth);
	yPos = (yPos == rcBorders.top? 22: yPos);

	m_bDynamicLayout = FALSE;

	return CSize(nWidth + rcBorders.left + rcBorders.right, yPos + rcBorders.bottom);
}



void CXTPPopupBar::UpdateLocation(CSize sz)
{

	RECT rcWork = m_rcExclude.IsRectEmpty()? CXTPDrawHelpers::GetWorkArea(): CXTPDrawHelpers::GetWorkArea(m_rcExclude);

	rcWork.bottom -= 5;  // for shadow

	CPoint ptCenterRect = m_rcExclude.CenterPoint();
	BOOL bScroll = FALSE;
	if (m_popupFlags == (xtpPopupLeft | xtpPopupDown) && m_rcExclude.Width() != 0)
		m_ptPopup.x = m_rcExclude.right - sz.cx;     else
	if (m_popupFlags & xtpPopupLeft && m_rcExclude.Width() != 0)
		m_ptPopup.x = m_rcExclude.left - sz.cx;

	if ((m_popupFlags & xtpPopupLeft) && m_rcExclude.Width() == 0)
	{
		m_ptPopup.x -= sz.cx;
	}

	if (m_rcExclude.Height() == 0 && m_ptPopup.y > rcWork.bottom)
	{
		m_ptPopup.y = m_ptPopup.y - sz.cy;
	}
	else if (m_ptPopup.y + sz.cy > rcWork.bottom)
	{
		if (m_rcExclude.Height() == 0)
		{
			m_ptPopup.y = (m_ptPopup.y < (rcWork.bottom - rcWork.top)/2 ? rcWork.bottom : m_ptPopup.y)- sz.cy;
		}
		else
		{
			if (m_popupFlags & xtpPopupDown )
			{
				m_ptPopup.y = m_rcExclude.top - sz.cy;
				if (m_ptPopup.y < rcWork.top)
				{
					bScroll = TRUE;
					if (ptCenterRect.y > (rcWork.bottom - rcWork.top)/2)
					{
						m_ptPopup.y = rcWork.top;
						sz.cy = m_rcExclude.top - rcWork.top;
					} else
					{
						m_ptPopup.y = m_rcExclude.bottom;
						sz.cy = rcWork.bottom - m_ptPopup.y;
					}
				}
			} else
			{
				m_ptPopup.y = rcWork.bottom - sz.cy;
			}
		}

		if (m_ptPopup.y < rcWork.top) m_ptPopup.y = rcWork.top;

		if (m_ptPopup.y + sz.cy > rcWork.bottom)
		{
			ASSERT(m_ptPopup.y == rcWork.top);
			sz.cy = rcWork.bottom - m_ptPopup.y;
			bScroll = TRUE;
		}


	}

	if (m_ptPopup.x + sz.cx > rcWork.right )
	{
		if (m_rcExclude.Width() == 0 && m_ptPopup.x > rcWork.right)
			m_ptPopup.x = m_ptPopup.x - sz.cx;
		else if (m_rcExclude.Width() == 0)
			m_ptPopup.x = rcWork.right - sz.cx;
		else
		{
			m_ptPopup.x = (m_popupFlags & xtpPopupDown ? rcWork.right: m_rcExclude.left) - sz.cx ;
			m_popupFlags |= xtpPopupLeft;
		}
	} else
	if (m_ptPopup.x < rcWork.left)
	{
		if (m_rcExclude.Width() != 0)
		{
			m_ptPopup.x = m_popupFlags & xtpPopupDown  ? rcWork.left : m_rcExclude.right;
			m_popupFlags &= ~xtpPopupLeft;
		}
		else if (m_popupFlags & xtpPopupLeft)
		{
			m_ptPopup.x = 0;
			m_popupFlags &= ~xtpPopupLeft;

		}
	}

	m_scrollInfo.bScroll = bScroll;
	if (bScroll)
	{
		m_scrollInfo.nScrollFirst = 0;
		ASSERT(GetType() == xtpBarTypePopup);
		AdjustScrolling(&sz);
	}

	SetWindowPos(&CWnd::wndTopMost, m_ptPopup.x, m_ptPopup.y, sz.cx, sz.cy, SWP_NOACTIVATE | /*SWP_NOZORDER | */SWP_SHOWWINDOW);
}

void CXTPPopupBar::AdjustScrolling(LPSIZE lpSize, BOOL bDown, BOOL bInvalidate)
{
	CSize sz = lpSize? *lpSize: CXTPWindowRect(this).Size();

	m_scrollInfo.btnDown.rc.SetRectEmpty();
	m_scrollInfo.btnUp.rc.SetRectEmpty();
	int nCount = m_pControls->GetCount();
	CRect rcBorders = GetPaintManager()->GetCommandBarBorders(this);

	int nSeparator = GetPaintManager()->DrawCommandBarSeparator(NULL, this, NULL, FALSE).cy;

	CSize szTearOffGripper(0);
	if (m_bTearOffPopup)
	{
		szTearOffGripper = GetPaintManager()->DrawTearOffGripper(NULL, 0, FALSE, FALSE);
		rcBorders.top += szTearOffGripper.cy;
	}

	CXTPControl* pButtonExpand = m_pControls->FindControl(XTP_ID_POPUPBAR_EXPAND);;
	CSize szButtonExpand(0);

	if (m_bCollapsed)
	{
		szButtonExpand = GetPaintManager()->DrawSpecialControl(NULL, xtpButtonExpandMenu, NULL, this, FALSE, NULL);
		rcBorders.bottom += szButtonExpand.cy;

		if (!pButtonExpand)
			pButtonExpand = m_pControls->Add(new CXTPControlExpandButton(), XTP_ID_POPUPBAR_EXPAND, _T(""), -1, TRUE);
		pButtonExpand->SetHideFlags(xtpHideGeneric);
	}
	else if (pButtonExpand) pButtonExpand->SetHideFlags(xtpHideGeneric);


	int nScrollHeight = 16;
	int i;

	if (!bDown)
	{
		if (m_scrollInfo.nScrollFirst == 1)
			m_scrollInfo.nScrollFirst = 0;

		int nPos = rcBorders.top;
		if (m_scrollInfo.nScrollFirst != 0)
		{
			nPos += nScrollHeight;
			m_scrollInfo.btnDown.rc.SetRect(rcBorders.left, rcBorders.top, sz.cx - rcBorders.right, nPos);
		}

		ASSERT(m_scrollInfo.nScrollFirst < nCount && m_scrollInfo.nScrollFirst >= 0);

		for (i = m_scrollInfo.nScrollFirst; i < nCount ;i++)
		{
			CXTPControl* pControl = GetControl(i);
			pControl->SetHideFlags(pControl->GetHideFlags() & ~xtpHideScroll);

			if (!pControl->IsVisible())
				continue;

			int nItemHeight = pControl->GetRect().Height();
			if (pControl->GetBeginGroup()) nPos += nSeparator;

			if (sz.cy - (nPos + nItemHeight) < nScrollHeight + rcBorders.bottom)
			{
				break;
			}
			pControl->SetRect(CRect(rcBorders.left, nPos, sz.cx - rcBorders.right, nPos + nItemHeight));

			nPos += nItemHeight;
		}
		for (int j = i; j < nCount; j++)
		{
			GetControl(j)->SetHideFlags(GetControl(j)->GetHideFlags() | xtpHideScroll);
		}

		m_scrollInfo.nScrollLast = i;

		if (i != nCount)
		{
			m_scrollInfo.btnUp.rc.SetRect(rcBorders.left, sz.cy - rcBorders.bottom - nScrollHeight, sz.cx - rcBorders.right, sz.cy - rcBorders.bottom);
		}
	}
	else
	{
		if (m_scrollInfo.nScrollLast == nCount - 1)
			m_scrollInfo.nScrollLast = nCount;

		int nPos = sz.cy - rcBorders.bottom;
		if (m_scrollInfo.nScrollLast != nCount)
		{
			nPos -= nScrollHeight;
			m_scrollInfo.btnUp.rc.SetRect(rcBorders.left, sz.cy - rcBorders.bottom - nScrollHeight, sz.cx - rcBorders.right, sz.cy - rcBorders.bottom);
		}

		ASSERT(m_scrollInfo.nScrollLast <= nCount && m_scrollInfo.nScrollLast > 0);

		for (i = m_scrollInfo.nScrollLast - 1 ; i >= 0 ;i--)
		{
			CXTPControl* pControl = GetControl(i);
			pControl->SetHideFlags(pControl->GetHideFlags() & ~xtpHideScroll);

			if (!pControl->IsVisible())
				continue;

			int nItemHeight = pControl->GetRect().Height();

			if (nPos - nItemHeight < nScrollHeight)
			{
				break;
			}
			pControl->SetRect(CRect(rcBorders.left, nPos - nItemHeight, sz.cx - rcBorders.right, nPos));

			nPos -= nItemHeight;
			if (pControl->GetBeginGroup()) nPos -= nSeparator;
		}

		for (int j = i; j >= 0; j--)
		{
			GetControl(j)->SetHideFlags(GetControl(j)->GetHideFlags() | xtpHideScroll);
		}

		m_scrollInfo.nScrollFirst = i + 1;

		if (i != -1)
		{
			m_scrollInfo.btnDown.rc.SetRect(rcBorders.left, rcBorders.top, sz.cx - rcBorders.right,  rcBorders.top + nScrollHeight);
		}
	}
	if (m_bCollapsed)
	{
		pButtonExpand->SetHideFlags(xtpNoHide);
		pButtonExpand->SetRect(CRect(rcBorders.left, sz.cy - rcBorders.bottom, sz.cx - rcBorders.right, sz.cy - rcBorders.bottom +  szButtonExpand.cy));
	}
	m_rcTearOffGripper.SetRect(rcBorders.left, rcBorders.top - szTearOffGripper.cy, sz.cx - rcBorders.right, rcBorders.top);

	if (bInvalidate) Redraw();
}

void CXTPPopupBar::UpdateShortcuts()
{
	if (GetFrameSite() == NULL)
		return;

	foreach (pControl, TRUE)
	{
		CString strShortcut;
		if (pControl->GetID() != 0)
		{
			XTPShortcutManager()->FindDefaultAccelerator(pControl->GetID(), strShortcut, GetCommandBars());
			pControl->m_strShortcutTextAuto = strShortcut;
		}
	}
}

void CXTPPopupBar::UpdateFlags()
{
	if (m_pControlPopup)
	{
		BOOL bLayoutRTL = IsLayoutRTL();


		CXTPCommandBar* pParent = GetParentCommandBar();
		ASSERT(pParent);

		m_popupFlags = xtpPopupRight;

		if (m_pControlPopup->m_dwFlags & xtpFlagLeftPopup || bLayoutRTL) m_popupFlags |= xtpPopupLeft;

		if (!IsVerticalPosition(pParent->GetPosition()))
			m_popupFlags |= xtpPopupDown;   else
		{
			if (pParent->GetPosition() == xtpBarRight || (pParent->GetType() == xtpBarTypePopup &&
				((CXTPPopupBar*)pParent)->m_popupFlags & xtpPopupLeft))
				m_popupFlags = xtpPopupLeft;
		}

		m_rcExclude = m_pControlPopup->GetRect();

		if (m_pControlPopup->GetType() == xtpControlComboBox && pParent->GetType() != xtpBarTypePopup)
		{
			m_popupFlags = xtpPopupDown;
		}
		else m_pControlPopup->AdjustExcludeRect(m_rcExclude);

		pParent->ClientToScreen(m_rcExclude);

		m_ptPopup.x = (m_popupFlags & xtpPopupDown)? m_rcExclude.left: m_rcExclude.right;
		m_ptPopup.y = (m_popupFlags & xtpPopupDown)? m_rcExclude.bottom: m_rcExclude.top;
	}
}


void CXTPPopupBar::UpdateExpandingState()
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (!pCommandBars)
		return;

	if (pCommandBars->GetCommandBarsOptions()->bAlwaysShowFullMenus)
	{
		foreach (pControl, FALSE)
		{
			pControl->SetExpanded(FALSE);
		}
		m_bExpanded = TRUE;
		return;
	}

	m_bCollapsed = FALSE;

	BOOL bCollapsedExists = FALSE;
	foreach (pControl, FALSE)
	{
		if (pCommandBars->IsControlHidden(pControl))
		{
			pControl->SetExpanded(TRUE);
		}
		else
		{
			pControl->SetExpanded(FALSE);

			if (pControl->GetID() != XTP_ID_POPUPBAR_EXPAND && (pControl->GetHideFlags() | xtpHideExpand | xtpHideScroll) == (xtpHideExpand|xtpHideScroll))
				bCollapsedExists = TRUE;
		}
	}

	foreach (pControl, FALSE)
	{
		pControl->SetHideFlags(pControl->GetHideFlags() & ~xtpHideExpand);

		if (pControl->GetExpanded())
		{
			if (!m_bExpanded && bCollapsedExists)
			{
				pControl->SetHideFlags(pControl->GetHideFlags() | xtpHideExpand);
				m_bCollapsed = TRUE;
			}
		}
	}
}

void CXTPPopupBar::Animate()
{
	CXTPCommandBar* pParentCommandBar = GetParentCommandBar();

	BOOL bAnimate = pParentCommandBar? pParentCommandBar->m_bAnimatePopup: TRUE;

	if (bAnimate)
	{
		if (IsCustomizeMode() || GetWindow(GW_CHILD))
			bAnimate = FALSE;
	}


	if (bAnimate)
	{
		XTPAnimationType animationType = GetAnimationType();
		if (IsAnimateType(animationType))
		{

			if (m_pControlPopup)
			{
				m_pControlPopup->RedrawParent();
				m_pControlPopup->GetParent()->UpdateWindow();
			}

			GetSite()->UpdateWindow();

			CPaintDC paintDC(this);
			CXTPClientRect rcClient(this);

			CXTPBufferDC animationDC(paintDC, rcClient);
			DrawCommandBar(&animationDC);

			// Animation
			GetPaintManager()->Animate(&paintDC, &animationDC, rcClient, animationType);
		}
	}

	if (pParentCommandBar) pParentCommandBar->m_bAnimatePopup = FALSE;

}



BOOL CXTPPopupBar::Popup(CXTPControlPopup* pControlPopup, BOOL bSelectFirst)
{
	m_pControlPopup = pControlPopup;

	if (!Create())
		return FALSE;

	LockRedraw();

	CWnd* pWndOwner = GetOwnerSite();
	if (pWndOwner) pWndOwner->SendMessage(WM_XTP_INITCOMMANDSPOPUP, 0, (LPARAM)this);

	m_bTearOffSelected = FALSE;

	m_bCollapsed = FALSE;
	m_bExpanded = IsCustomizeMode() || XTPMouseManager()->IsForceExpanded();

	UpdateFlags();

	CSize sz = CalcDynamicLayout(0, 0);
	UpdateLocation(sz);

	SetTrackingMode(TRUE, bSelectFirst);

	m_nIdleFlags &= ~(xtpIdleLayout|xtpIdleRedraw);
	OnIdleUpdateCmdUI(TRUE, 0L);

	Animate();
	XTPSoundManager()->PlaySystemSound(xtpSoundMenuPopup);

	if (m_barPosition == xtpBarPopup && m_bShowShadow)
	{
		XTPShadowsManager()->RemoveShadow(this);
		XTPShadowsManager()->SetShadow(this, m_rcExclude);
	}

	UnlockRedraw();

	return TRUE;
}

BOOL CXTPPopupBar::Popup(int x, int y, LPCRECT rcExclude)
{
	m_ptPopup = CPoint(x, y);
	if (rcExclude) m_rcExclude = *rcExclude;
	return Popup(NULL);
}

BEGIN_MESSAGE_MAP(CXTPPopupBar, CXTPCommandBar)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CXTPPopupBar::SwitchTearOffTracking(BOOL bShow, CPoint point)
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	ASSERT(pCommandBars);

	ShowWindow(bShow? SW_SHOWNA: SW_HIDE);

	if (!bShow)
	{
		if (m_pTearOffBar == NULL)
		{
			m_pTearOffBar = pCommandBars->GetToolBar(m_nTearOffID);
			if (m_pTearOffBar != NULL)
			{
				m_pTearOffBar->SetPosition(xtpBarFloating);
				m_pTearOffBar->SetVisible(TRUE);
				GetFrameSite()->RecalcLayout();
			}
		}
		if (m_pTearOffBar == NULL)
		{

			m_pTearOffBar = pCommandBars->AddCustomBar(m_strTearOffCaption, m_nTearOffID, TRUE);
			m_pTearOffBar->m_nMRUWidth = m_nTearOffWidth;

			foreach(pControl, FALSE)
			{
				if (pControl->GetID() != XTP_ID_POPUPBAR_EXPAND)
				{
					CXTPControl* pControlClone = m_pTearOffBar->GetControls()->AddClone(pControl);
					pControlClone->SetHideFlags(pControlClone->GetHideFlags() & ~(xtpHideScroll|xtpHideExpand));
				}
			}

		}
		ASSERT(m_pTearOffBar);

		m_szTearOffBar = m_pTearOffBar->CalcDynamicLayout(-1, LM_MRUWIDTH | LM_HORZ | LM_COMMIT);
		CPoint pt(point.x - m_szTearOffBar.cx / 2, point.y - 10);
		m_pTearOffBar->MoveWindow(pt.x, pt.y, m_szTearOffBar.cx, m_szTearOffBar.cy);
		m_pTearOffBar->OnInitialUpdate();
		m_pTearOffBar->Invalidate(FALSE);


	}
	else
	{
		if (m_pTearOffBar)
		{
			m_pTearOffBar->SetVisible(FALSE);
			m_pTearOffBar = 0;
		}

	}
}

void CXTPPopupBar::TrackTearOff()
{
	m_pTearOffBar = NULL;
	CRect rcGripper = m_rcTearOffGripper;
	ClientToScreen(&rcGripper);

	BOOL bShow = TRUE;
	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return;

	// set capture to the window which received this message
	SetCapture();
	ASSERT(this == CWnd::GetCapture());

	CPoint pt(0, 0);
	// get messages until capture lost or cancelled/accepted
	while (CWnd::GetCapture() == this)
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

		if (msg.message == WM_LBUTTONUP)
			break;
		else if (msg.message == WM_MOUSEMOVE && pt != msg.pt)
		{
			pt = msg.pt;
			BOOL bState = !(pt.x > rcGripper.right + 10 || pt.x < rcGripper.left - 10 ||
				pt.y > rcGripper.bottom + 10 || pt.y < rcGripper.top - 10 );
			if (bState != bShow)
			{
				bShow = bState;
				SwitchTearOffTracking(bState, pt);
			}

			if (m_pTearOffBar != NULL)
			{
				CPoint point(pt.x - m_szTearOffBar.cx / 2, pt.y - 10);
				m_pTearOffBar->MoveWindow(point.x, point.y, m_szTearOffBar.cx, m_szTearOffBar.cy);

			}

		}
		else if (msg.message == WM_KEYDOWN)
		{
			if (msg.wParam == VK_ESCAPE)
			{
				break;
			}
		} else
			DispatchMessage(&msg);

	}

	ReleaseCapture();
	if (m_pTearOffBar)
	{
		XTPMouseManager()->SendTrackLost();
	}
}


void CXTPPopupBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bTearOffPopup && m_rcTearOffGripper.PtInRect(point) && !IsCustomizeMode())
	{
		SetCursor(LoadCursor(NULL, IDC_SIZEALL));

		XTPMouseManager()->LockMouseMove();
		m_bTearOffTracking = TRUE;
		TrackTearOff();
		m_bTearOffTracking = FALSE;
		XTPMouseManager()->UnlockMouseMove();

	}
	CXTPCommandBar::OnLButtonDown(nFlags, point);
}

BOOL CXTPPopupBar::DestroyWindow()
{
	CWnd* pWndParent = GetSite();

	CWnd* pWnd = GetWindow(GW_CHILD);
	while (pWnd)
	{
		CWnd* pWndNext = pWnd->GetWindow(GW_HWNDNEXT);

		pWnd->ShowWindow(SW_HIDE);
		pWnd->SetParent(pWndParent);

		pWnd = pWndNext;
	}
	return CWnd::DestroyWindow();
}

BOOL CXTPPopupBar::SetTrackingMode(int bMode, BOOL bSelectFirst, BOOL bKeyboard)
{
	if (!CXTPCommandBar::SetTrackingMode(bMode, bSelectFirst, bKeyboard))
		return FALSE;

	if (!bMode)
	{
		DestroyWindow();

		CXTPCommandBar* pParentCommandBar = GetParentCommandBar();
		if (pParentCommandBar)
			pParentCommandBar->SetPopuped(-1);

		CWnd* pWnd = GetSite();

		if (pWnd && ::IsWindow(pWnd->m_hWnd))
		{
			pWnd->UpdateWindow();
			pWnd->SendMessage(WM_XTP_UNINITCOMMANDSPOPUP, 0, (LPARAM)this);
		}

		m_pControlPopup = 0;
	}

	return TRUE;
}


void CXTPPopupBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if (GetParentCommandBar() != NULL && CXTPClientRect(this).PtInRect(point))
	{
		// restore selection
		GetParentCommandBar()->SetSelected(m_pControlPopup->GetIndex());
	}

	if (m_scrollInfo.bScroll)
	{
		if (m_scrollInfo.btnUp.OnMouseMove(point) ||
			m_scrollInfo.btnDown.OnMouseMove(point))
		{
			SetPopuped(-1);
		}
	}

	if (m_bTearOffPopup && !IsCustomizeMode() && m_rcTearOffGripper.PtInRect(point) && !m_bTearOffSelected)
	{
		SetSelected(-1);
		SetPopuped(-1);
		m_nTearOffTimer = SetTimer(TID_TEAROFFSELECTED, 80, NULL);
	}

	CXTPCommandBar::OnMouseMove(nFlags, point);
}

CXTPCommandBar* CXTPPopupBar::GetParentCommandBar() const
{
	return m_pControlPopup? m_pControlPopup->GetParent(): NULL;
}

int CXTPPopupBar::OnMouseActivate(CWnd* /*pDesktopWnd*/, UINT /*nHitTest*/, UINT /*message*/)
{
	return MA_NOACTIVATE;
}

void CXTPPopupBar::DrawCommandBar(CDC* pDC)
{
	CXTPCommandBar::DrawCommandBar(pDC);

	if (m_scrollInfo.bScroll)
	{
		if (!m_scrollInfo.btnUp.rc.IsRectEmpty())
		{
			CPoint pt = m_scrollInfo.btnUp.rc.CenterPoint();
			GetPaintManager()->Triangle(pDC, CPoint(pt.x - 3, pt.y), CPoint(pt.x + 3, pt.y), CPoint(pt.x, pt.y + 3), GetXtremeColor(COLOR_BTNTEXT));
		}
		if (!m_scrollInfo.btnDown.rc.IsRectEmpty())
		{
			CPoint pt = m_scrollInfo.btnDown.rc.CenterPoint();
			GetPaintManager()->Triangle(pDC, CPoint(pt.x - 3, pt.y), CPoint(pt.x + 3, pt.y), CPoint(pt.x, pt.y - 3), GetXtremeColor(COLOR_BTNTEXT));
		}
	}

	if (m_bTearOffPopup && m_pControls->GetCount() > 0)
	{
		GetPaintManager()->DrawTearOffGripper(pDC, m_rcTearOffGripper, m_bTearOffSelected, TRUE);
	}
}


BOOL CXTPPopupBar::SetSelected(int nSelected, BOOL bKeyboard)
{
	if (!CXTPCommandBar::SetSelected(nSelected, bKeyboard))
		return FALSE;

	if (m_scrollInfo.bScroll && m_nSelected != -1)
	{
		if (GetControl(m_nSelected)->GetHideFlags() & xtpHideScroll)
		{
			if (m_nSelected >= m_scrollInfo.nScrollLast)
			{
				m_scrollInfo.nScrollLast = m_nSelected + 1;
				AdjustScrolling(NULL, TRUE, TRUE);
			} else
			{
				m_scrollInfo.nScrollFirst = m_nSelected;
				AdjustScrolling(NULL, FALSE, TRUE);
			}
		}
	}

	return TRUE;
}

void CXTPPopupBar::OnTimer(UINT_PTR nIDEvent)
{
	int nCount = m_pControls->GetCount();

	switch (nIDEvent)
	{
		case TID_HOVERUP:
			if (m_scrollInfo.nScrollLast == nCount)
			{
				m_scrollInfo.btnUp.KillTimer();
				return;
			}
			m_scrollInfo.nScrollLast++;
			AdjustScrolling(NULL, TRUE, TRUE);

			break;

		case TID_HOVERDOWN:
			if (m_scrollInfo.nScrollFirst == 0)
			{
				m_scrollInfo.btnDown.KillTimer() ;
				return;
			}
			m_scrollInfo.nScrollFirst--;
			AdjustScrolling(NULL, FALSE, TRUE);
			break;

		case TID_TEAROFFSELECTED:
			{
				CPoint pt;
				GetCursorPos(&pt);
				ScreenToClient(&pt);
				if (!m_bTearOffSelected && m_rcTearOffGripper.PtInRect(pt))
				{
					m_bTearOffSelected = TRUE;
					Invalidate(FALSE);
				}
				if (m_bTearOffSelected && !m_rcTearOffGripper.PtInRect(pt) && !m_bTearOffTracking)
				{
					m_bTearOffSelected = FALSE;
					Invalidate(FALSE);
					KillTimer(m_nTearOffTimer);
				}
			}

			break;
	}

	CXTPCommandBar::OnTimer(nIDEvent);
}

BOOL CXTPPopupBar::OnKeyDown(UINT nChar)
{
	if (m_bCollapsed)
	{
		if (nChar == VK_DOWN)
		{
			if (GetKeyState(VK_CONTROL) < 0 || m_pControls->GetNext(m_nSelected, +1) <= m_nSelected)
			{
				ExpandBar();
			}
		}
	}
	return CXTPCommandBar::OnKeyDown(nChar);
}

void CXTPPopupBar::SetDefaultItem(UINT uItem, BOOL fByPos)
{
	CXTPControl* pControl = fByPos? m_pControls->GetAt(uItem): m_pControls->FindControl(xtpControlError, uItem, TRUE, FALSE);
	if (pControl)
	{
		pControl->SetItemDefault(TRUE);
	}
}

UINT CXTPPopupBar::GetDefaultItem(UINT /*gmdiFlags*/, BOOL fByPos)
{
	foreach (pControl, TRUE)
	{
		if (pControl->IsItemDefault())
		{
			return fByPos? pControl->GetIndex(): pControl->GetID();
		}
	}
	return (UINT)-1;
}

void CXTPPopupBar::Copy(CXTPCommandBar* pCommandBar, BOOL bRecursive)
{
	ASSERT_KINDOF(CXTPPopupBar, pCommandBar);

	CXTPCommandBar::Copy(pCommandBar, bRecursive);

	m_bTearOffPopup = ((CXTPPopupBar*)pCommandBar)->m_bTearOffPopup;
	m_strTearOffCaption = ((CXTPPopupBar*)pCommandBar)->m_strTearOffCaption;
	m_nTearOffID = ((CXTPPopupBar*)pCommandBar)->m_nTearOffID;
	m_nTearOffWidth = ((CXTPPopupBar*)pCommandBar)->m_nTearOffWidth;

	m_bShowShadow = ((CXTPPopupBar*)pCommandBar)->m_bShowShadow;
	m_rcBorders = ((CXTPPopupBar*)pCommandBar)->m_rcBorders;
}

void CXTPPopupBar::SetPopupToolBar(BOOL bToolBarType)
{
	m_barType = bToolBarType? xtpBarTypeNormal: xtpBarTypePopup;
}

////////////////////////////////////////////////////////////////////////////
//

IMPLEMENT_XTP_COMMANDBAR(CXTPPopupToolBar, CXTPPopupBar)

CXTPPopupToolBar::CXTPPopupToolBar()
{
	m_barType = xtpBarTypeNormal;
}

CSize CXTPPopupToolBar::CalcDynamicLayout(int, DWORD /*nMode*/)
{
	return CXTPPopupBar::CalcDynamicLayout(0, 0);
}
CXTPPopupToolBar* CXTPPopupToolBar::CreatePopupToolBar(CXTPCommandBars* pCommandBars)
{
	CXTPPopupToolBar* pPopupBar = (CXTPPopupToolBar*)CXTPPopupToolBar::CreateObject();
	pPopupBar->SetCommandBars(pCommandBars);
	return pPopupBar;
}
