// ControlTools.cpp: implementation of the CControlTools class.
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
#include "resource.h"
#include "ControlTools.h"
#include "CustomizePageTools.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_XTP_CONTROL( CControlTools, CXTPControlButton)


CControlTools::CControlTools()
{

}

CControlTools::~CControlTools()
{

}

void CControlTools::OnCalcDynamicSize(DWORD /*dwMode*/)
{

	ASSERT(m_pControls->GetAt(m_nIndex) == this);

	while (m_nIndex + 1 < m_pControls->GetCount())
	{
		CXTPControl* pControl = m_pControls->GetAt(m_nIndex + 1);
		if (pControl->GetID() == ID_TOOLS_TOOLITEM)
		{
			m_pControls->Remove(pControl);
		}
		else break;
	}

	if (m_pParent->IsCustomizeMode())
	{
		m_dwHideFlags = 0;
		SetEnabled(TRUE);
		return;
	}
	if (CCustomizePageTools::m_lstTools.IsEmpty())
	{
		m_dwHideFlags = 0;
		SetEnabled(FALSE);

		return;
	}

	m_dwHideFlags |= xtpHideGeneric;

	int nIndex = m_nIndex + 1;

	POSITION pos = CCustomizePageTools::m_lstTools.GetHeadPosition();
	while (pos)
	{
		CToolRec& tool = CCustomizePageTools::m_lstTools.GetNext(pos);

		CXTPControl* pControl = m_pControls->Add(xtpControlButton, ID_TOOLS_TOOLITEM, _T(""), nIndex, TRUE);

		pControl->SetBeginGroup(nIndex == m_nIndex + 1 && GetBeginGroup());
		pControl->SetCaption(tool.m_strTitle);
		pControl->SetFlags(xtpFlagManualUpdate);
		pControl->SetTag((DWORD_PTR)&tool);
		pControl->SetIconId(tool.m_nIconID);

		nIndex++;
	}
}


BOOL CControlTools::IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint /*point*/, DROPEFFECT& dropEffect)
{
	if (pCommandBar->GetType() != xtpBarTypePopup)
	{
		dropEffect = DROPEFFECT_NONE;
		return FALSE;
	}
	return TRUE;
}
