// XTPControlButton.cpp : implementation of the CXTPControlButton class.
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
#include "XTPControlButton.h"
#include "XTPCommandBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPControlButton

IMPLEMENT_XTP_CONTROL(CXTPControlButton, CXTPControl)

CXTPControlButton::CXTPControlButton()
{
	EnableAutomation();

	m_controlType = xtpControlButton;
}

CXTPControlButton::~CXTPControlButton()
{
}

void CXTPControlButton::OnLButtonUp(CPoint /*point*/)
{
	if (m_bEnabled && m_pParent->GetType() == xtpBarTypePopup)
	{
		OnExecute();
	}
}

void CXTPControlButton::OnClick(BOOL bKeyboard, CPoint pt)
{
	if (!m_bEnabled) return;

	if (IsCustomizeMode())
	{
		m_pParent->SetPopuped(-1);
		m_pParent->SetSelected(-1);
		CustomizeStartDrag(pt);
		return;
	}

	if (!bKeyboard)
	{
		if (m_pParent->GetType() != xtpBarTypePopup)
			ClickToolBarButton();
	}
	else
	{
		OnExecute();
	}
}

void CXTPControlButton::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	ASSERT(DYNAMIC_DOWNCAST(CXTPControlButton, pControl));

	CXTPControl::Copy(pControl, bRecursive);
}






