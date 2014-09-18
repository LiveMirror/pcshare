// XTPControlExt.cpp : implementation file.
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
#include "ControlUndo.h"

#include "DynamicPopups.h"

#include "DynamicPopupsDoc.h"
#include "DynamicPopupsView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_XTP_CONTROL( CControlUndo, CXTPControlButton)

void CControlUndo::OnCalcDynamicSize(DWORD /*dwMode*/)
{
	if (GetParent()->GetType() !=xtpBarTypePopup)
		return;

	ASSERT(m_pControls->GetAt(m_nIndex) == this);

	while (m_nIndex + 1 < m_pControls->GetCount())
	{
		CXTPControl* pControl = m_pControls->GetAt(m_nIndex + 1);
		if (pControl->GetID() >= FIRST_UNDO_COMMAND && pControl->GetID() < LAST_UNDO_COMMAND)
		{
			m_pControls->Remove(pControl);
		}
		else break;
	}

	m_dwHideFlags = 0;

	if (m_pParent->IsCustomizeMode())
		return;

	CMDIFrameWnd* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, AfxGetMainWnd());
	ASSERT(pFrame);

	CFrameWnd* pChildFrame =  pFrame->GetActiveFrame();
	if (pChildFrame == NULL)
		return;

	CDynamicPopupsView* pView = (CDynamicPopupsView*)pChildFrame->GetActiveView();
	if (pView == NULL)
		return;

	m_dwHideFlags |= xtpHideGeneric;

	int nCount = pView->m_wndList.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CString strTitle;
		pView->m_wndList.GetText(i, strTitle);

		CXTPControl* pControl = m_pControls->Add(xtpControlButton, FIRST_UNDO_COMMAND + i, _T(""), m_nIndex + 1 + i, TRUE);

		pControl->SetCaption(strTitle);
		pControl->SetFlags(xtpFlagManualUpdate);

		if (GetBeginGroup() && (i == 0))
			pControl->SetBeginGroup(TRUE);

	}

}


BOOL CControlUndo::IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint /*point*/, DROPEFFECT& dropEffect)
{
	if (pCommandBar->GetType() != xtpBarTypePopup)
	{
		dropEffect = DROPEFFECT_NONE;
		return FALSE;
	}
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
//

IMPLEMENT_XTP_CONTROL( CControlComboBoxUndo, CXTPControlComboBox)

BOOL CControlComboBoxUndo::OnSetPopup(BOOL bPopup)
{
	if (bPopup)
	{
		CString str = GetText();

		ResetContent();

		CMDIFrameWnd* pFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, AfxGetMainWnd());
		ASSERT(pFrame);

		CFrameWnd* pChildFrame =  pFrame->GetActiveFrame();
		if (pChildFrame == NULL)
			return CXTPControlComboBox::OnSetPopup(bPopup);;

		CDynamicPopupsView* pView = (CDynamicPopupsView*)pChildFrame->GetActiveView();
		if (pView == NULL)
			return CXTPControlComboBox::OnSetPopup(bPopup);;

		int nCount = pView->m_wndList.GetCount();
		for (int i = 0; i < nCount; i++)
		{
			CString strTitle;
			pView->m_wndList.GetText(i, strTitle);

			AddString(strTitle);

		}

		int nIdx = FindStringExact(-1, str);
		if (nIdx != CB_ERR) SetCurSel(nIdx);
	}


	return CXTPControlComboBox::OnSetPopup(bPopup);
}
