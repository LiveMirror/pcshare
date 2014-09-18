// XTPControlPopup.cpp : implementation of the CXTPControlPopup class.
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

#include "XTPControlPopup.h"
#include "XTPCommandBar.h"
#include "XTPControls.h"
#include "XTPPopupBar.h"
#include "XTPShadowsManager.h"
#include "XTPPaintManager.h"
#include "XTPToolBar.h"
#include "XTPCommandBars.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_XTP_CONTROL(CXTPControlPopup, CXTPControl)

CXTPControlPopup::CXTPControlPopup(void)
{
	EnableAutomation();

	m_controlType = xtpControlPopup;
	m_bPopup = FALSE;
	m_pCommandBar = NULL;
	m_dwCommandBarID = 0;
}

CXTPControlPopup::~CXTPControlPopup(void)
{
	if (m_pCommandBar)
		m_pCommandBar->InternalRelease();

}

CXTPControlPopup* CXTPControlPopup::CreateControlPopup(XTPControlType controlType)
{
	ASSERT(IsPopupControlType(controlType));

	CXTPControlPopup* pControl = (CXTPControlPopup*)CXTPControlPopup::CreateObject();
	pControl->m_controlType = controlType;
	return pControl;
}

void CXTPControlPopup::OnClick(BOOL bKeyboard, CPoint pt)
{
	if (!m_bEnabled)
		return;

	if (IsCustomizeMode())
	{
		ASSERT(m_pParent);
		CXTPCommandBars* pCommandBars = m_pParent->GetCommandBars();
		ASSERT(pCommandBars);

		pCommandBars->SetDragControl(this);
	}

	if (m_bPopup && (GetParent()->GetPosition() != xtpBarPopup))
		m_pParent->SetTrackingMode(FALSE, FALSE);
	else
	{
		pt.Offset(-m_rcControl.TopLeft());

		if (!IsCustomizeMode() && m_controlType == xtpControlSplitButtonPopup && bKeyboard  && m_pParent->GetType() == xtpBarTypePopup)
		{
			OnExecute();
			return;
		}
		if (!IsCustomizeMode() && m_controlType == xtpControlSplitButtonPopup && !bKeyboard  && m_pParent->GetType() == xtpBarTypePopup && pt.x < m_rcControl.Width() - 20)
		{
			m_pParent->KillTimer(XTP_TID_HOVER);
			return;
		}
		if (!IsCustomizeMode() && m_controlType == xtpControlSplitButtonPopup && !bKeyboard && m_pParent->GetType() != xtpBarTypePopup && pt.x < m_rcControl.Width() - 11)
		{
			CRect rcActive = m_rcControl;
			rcActive.right -= 11;
			ClickToolBarButton(rcActive);

		} else if (!(IsCustomizeMode() && m_pCommandBar && !m_pCommandBar->IsCustomizable()))
		{
			m_pParent->SetPopuped(m_nIndex);
		}
	}

	if (IsCustomizeMode())
	{
		CustomizeStartDrag(pt);
	}
}

void CXTPControlPopup::OnLButtonUp(CPoint point)
{
	if (m_bEnabled && GetType() == xtpControlSplitButtonPopup && m_pParent->GetType() == xtpBarTypePopup)
	{
		if (m_rcControl.right > point.x + 20 )
			OnExecute();
	}
}

BOOL CXTPControlPopup::OnSetSelected(int bSelected)
{

	if (!CXTPControl::OnSetSelected(bSelected))
		return FALSE;

	if (m_controlType == xtpControlPopup || m_pParent->GetType() == xtpBarTypePopup)
	{
		if (m_bEnabled && bSelected && m_pParent->IsTrackingMode() == TRUE_POPUP && GetParent()->GetPosition() != xtpBarPopup)
		{
			m_pParent->SetPopuped(m_nIndex);
		}
		else if (!bSelected && m_bPopup && GetParent()->GetPosition() != xtpBarPopup)
		{
			m_pParent->SetPopuped(-1);
		}
	} else
	{
		if (m_bEnabled && bSelected == TRUE_KEYBOARD && m_pParent->IsTrackingMode() == TRUE_POPUP)
		{
			m_pParent->SetPopuped(m_nIndex);
		}

	}



	return TRUE;
}

void CXTPControlPopup::UpdateShadow()
{
	if (m_pParent->GetPosition() != xtpBarPopup && IsPopupControlType(m_controlType))
	{
		CXTPPopupBar* pPopupBar = DYNAMIC_DOWNCAST(CXTPPopupBar, m_pCommandBar);
		if (pPopupBar && pPopupBar->m_bShowShadow)
			XTPShadowsManager()->SetShadow(this);
	}
}

BOOL CXTPControlPopup::OnSetPopup(BOOL bPopup)
{
	m_bPopup = bPopup;
	if (bPopup)
	{
		ASSERT(m_pCommandBar);

		if (m_pCommandBar )
		{
			if (!m_pCommandBar->IsTrackingMode())
			{

				m_pCommandBar->Popup(this, GetSelected() == TRUE_KEYBOARD);
				UpdateShadow();
			}
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		m_pCommandBar->SetTrackingMode(FALSE);
	}

	DelayRedrawParent();
	return TRUE;
}

void CXTPControlPopup::ExpandCommandBar()
{
	CXTPPopupBar* pPopupBar = DYNAMIC_DOWNCAST(CXTPPopupBar, m_pCommandBar);
	if (m_bPopup && pPopupBar && m_controlType == xtpControlPopup)
	{
		pPopupBar->ExpandBar();
	}
}

void CXTPControlPopup::OnMouseHover()
{
	if (m_bEnabled && (m_pParent->IsTrackingMode() == TRUE_POPUP || GetParent()->GetPosition() == xtpBarPopup))
		GetParent()->SetPopuped(m_nIndex);
	else CXTPControl::OnMouseHover();
}

BOOL CXTPControlPopup::OnLButtonDblClk(CPoint point)
{
	CXTPPopupBar* pPopupBar = DYNAMIC_DOWNCAST(CXTPPopupBar, m_pCommandBar);
	if (pPopupBar && (m_controlType == xtpControlPopup || m_controlType == xtpControlButtonPopup))
	{
		if (!m_bPopup)
			m_pParent->OnLButtonDown(0, point);

		if (m_pCommandBar != pPopupBar)
			return FALSE;

		if (m_bPopup && pPopupBar->GetDefaultItem(0, TRUE) != (UINT)-1)
		{
			CXTPControl* pControl = pPopupBar->GetControl(pPopupBar->GetDefaultItem(0, TRUE));
			pControl->OnExecute();
			return TRUE;
		}

		if (m_bPopup && pPopupBar->m_bCollapsed)
		{
			pPopupBar->ExpandBar();
			return TRUE;
		}
	} else
	{
		return CXTPControl::OnLButtonDblClk(point);
	}
	return FALSE;
}



void CXTPControlPopup::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	ASSERT(DYNAMIC_DOWNCAST(CXTPControlPopup, pControl));

	CXTPControl::Copy(pControl, bRecursive);

	if (bRecursive && ((CXTPControlPopup*)pControl)->m_pCommandBar)
	{
		CXTPCommandBar* pCommandBar = ((CXTPControlPopup*)pControl)->m_pCommandBar->Clone(bRecursive);
		SetCommandBar(pCommandBar);
		pCommandBar->InternalRelease();

	} else
	{
		SetCommandBar(((CXTPControlPopup*)pControl)->m_pCommandBar);
	}

}

void CXTPControlPopup::SetCommandBar(CXTPCommandBar* pBar)
{
	if (m_pCommandBar)
		m_pCommandBar->InternalRelease();

	m_pCommandBar = pBar;

	if (m_pCommandBar)
		m_pCommandBar->InternalAddRef();

}

void CXTPControlPopup::SetCommandBar(CMenu* pMenu)
{
	if (!m_pCommandBar)
		m_pCommandBar = CXTPPopupBar::CreatePopupBar(m_pParent? m_pParent->GetCommandBars(): NULL);

	m_pCommandBar->LoadMenu(pMenu);
}

BOOL CXTPControlPopup::IsContainPopup(CXTPControlPopup* pControlPopup)
{
	if (!m_pCommandBar || !pControlPopup->GetCommandBar())
		return FALSE;

	if (m_pCommandBar == pControlPopup->GetCommandBar()) return TRUE;

	for (int i = 0; i < m_pCommandBar->GetControls()->GetCount(); i++)
	{
		CXTPControlPopup* pControlChild = DYNAMIC_DOWNCAST(CXTPControlPopup, m_pCommandBar->GetControl(i));
		if (pControlChild)
		{
			if (pControlPopup->IsContainPopup(pControlChild))
				return TRUE;

			if (pControlChild->IsContainPopup(pControlPopup))
				return TRUE;
		}

	}
	return FALSE;
}


BOOL CXTPControlPopup::Compare(CXTPControl* pOther)
{
	if (!CXTPControl::Compare(pOther))
		return FALSE;

	CXTPControlPopup* pPopup = DYNAMIC_DOWNCAST(CXTPControlPopup, pOther);
	if (!pPopup)
		return FALSE;

	if (m_pCommandBar != NULL && pPopup->m_pCommandBar != NULL)
	{
		if (!m_pCommandBar->GetControls()->Compare(pPopup->m_pCommandBar->GetControls()))
			return FALSE;
	}

	return TRUE;

}

void CXTPControlPopup::OnCustomizeDragOver(CXTPControl* pDataObject, CPoint /*point*/, DROPEFFECT& /*dropEffect*/)
{
	if (m_pCommandBar && m_pCommandBar->IsCustomizable())
	{
		CXTPControlPopup* pControlPopup = DYNAMIC_DOWNCAST(CXTPControlPopup, pDataObject);

		if (!(pControlPopup && pControlPopup->IsContainPopup(this)))
		{
			m_pParent->SetPopuped(m_nIndex);
		}
		else
		{
			m_pParent->SetPopuped(-1);
			m_pParent->SetSelected(-1);
		}
	}
}

void CXTPControlPopup::AdjustExcludeRect(CRect& rc)
{
	GetPaintManager()->AdjustExcludeRect(rc, this, IsVerticalPosition(GetParent()->GetPosition()));
}


