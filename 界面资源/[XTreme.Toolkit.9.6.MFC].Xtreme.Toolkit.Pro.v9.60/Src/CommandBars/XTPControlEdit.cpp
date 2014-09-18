// XTPControlEdit.cpp : implementation of the CXTPControlEdit class.
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
#include "XTPColorManager.h"

#include "XTPControlEdit.h"
#include "XTPCommandBar.h"
#include "XTPToolBar.h"
#include "XTPCommandBars.h"
#include "XTPPaintManager.h"
#include "XTPMouseManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CXTPControlEdit

IMPLEMENT_XTP_CONTROL(CXTPControlEdit, CXTPControl)

CXTPControlEdit::CXTPControlEdit(void)
{
	EnableAutomation();

	m_controlType = xtpControlEdit;
	m_nWidth = 100;
	m_nLabelWidth = 0;
	m_bLabel = TRUE;
	m_strEditText = _T("");
	m_pEdit = NULL;
	m_bReadOnly = FALSE;
	m_bDelayReposition = FALSE;
}

CString CXTPControlEdit::GetEditText()
{
	if (m_pEdit && m_pEdit->GetSafeHwnd())
	{
		m_pEdit->GetWindowText(m_strEditText);
	}
	return m_strEditText;
}

void CXTPControlEdit::SetEditText(LPCTSTR lpszText)
{
	if (m_pEdit == NULL)
	{
		m_pEdit = CreateEditControl();
		m_pEdit->m_pControl = this;
	}
	if (m_pEdit && m_pEdit->GetSafeHwnd() && GetEditText() != lpszText)
	{
		m_pEdit->SetWindowText(lpszText);
	}
	else m_strEditText = lpszText;
}

CXTPControlEdit::~CXTPControlEdit(void)
{
	if (m_pEdit)
	{
		m_pEdit->m_pControl = NULL;
		delete m_pEdit;
	}
}
BOOL CXTPControlEdit::OnKeyDown(UINT nChar)
{
	if (nChar == VK_ESCAPE && m_pEdit)
	{
		if (::GetFocus() == *m_pEdit)
		{
			::SetFocus(m_pParent->GetTrackFocus());
			m_pParent->Invalidate(FALSE);
			m_pParent->OnIdleUpdateCmdUI(TRUE, FALSE);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE_EXIT;
}

CXTPControlEditCtrl* CXTPControlEdit::CreateEditControl()
{
	return new CXTPControlEditCtrl();
}

void CXTPControlEdit::SetRect(CRect rcControl)
{
	if (!m_pEdit)
	{
		m_pEdit = CreateEditControl();
		m_pEdit->m_pControl = this;
	}

	if (m_rcControl == rcControl && !(m_pEdit && !m_pEdit->GetSafeHwnd()) && (m_bDelayReposition == FALSE))
	{
		if (!(m_pEdit && m_pEdit->GetSafeHwnd() && m_pEdit->GetParent() != m_pParent))
			return;
	}

	m_rcControl = rcControl;

	if (m_pEdit)
	{
		if (!m_pEdit->GetSafeHwnd())
		{
			m_pEdit->CreateEdit(WS_CHILD|ES_AUTOHSCROLL, m_pParent);
			m_pEdit->SetFont(GetPaintManager()->GetIconFont(), FALSE);
			if (!m_strEditText.IsEmpty()) m_pEdit->SetWindowText(m_strEditText);
			if (!m_bEnabled) m_pEdit->EnableWindow(FALSE);
		}

		m_pEdit->SetParent(m_pParent);
		m_pEdit->SetMargins(0, 0);
		m_pEdit->SetReadOnly(m_bReadOnly);
		rcControl.DeflateRect(m_nLabelWidth + 1 + 3, 3, 3, 3);
		m_pEdit->MoveWindow(rcControl);
		m_pEdit->SetWindowPos(0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | ((m_dwHideFlags != xtpNoHide)? SWP_HIDEWINDOW: SWP_SHOWWINDOW));
	}
	m_bDelayReposition = FALSE;
}

void CXTPControlEdit::SetReadOnly(BOOL bReadOnly /* = TRUE */)
{
	m_bReadOnly = bReadOnly;

	if (m_pEdit && m_pEdit->GetSafeHwnd())
	{
		m_pEdit->SetReadOnly(bReadOnly);
	}
}

BOOL CXTPControlEdit::GetReadOnly()
{
	return m_bReadOnly;
}

void CXTPControlEdit::SetEnabled(BOOL bEnabled)
{
	if (bEnabled != m_bEnabled)
	{
		m_bEnabled = bEnabled;
		if (m_pEdit && m_pEdit->GetSafeHwnd())
			m_pEdit->EnableWindow(bEnabled);

		DelayRedrawParent();
	}
}
void CXTPControlEdit::SetParent(CXTPCommandBar* pParent)
{
	CXTPControl::SetParent(pParent);

	if (!pParent && m_pEdit && m_pEdit->GetSafeHwnd())
	{
		m_pEdit->DestroyWindow();
	}
}

BOOL  CXTPControlEdit::OnSetSelected(int bSelected)
{
	if (!CXTPControl::OnSetSelected(bSelected))
		return FALSE;

	if (!bSelected && m_pEdit && ::GetFocus() == *m_pEdit)
	{
		::SetFocus(m_pParent->GetTrackFocus());
		m_pParent->Invalidate(FALSE);
	}

	return TRUE;
}

void CXTPControlEdit::OnClick(BOOL bKeyboard, CPoint pt)
{
	if (IsCustomizeMode())
	{
		m_pParent->SetPopuped(-1);
		m_pParent->SetSelected(-1);
		CustomizeStartDrag(pt);
	}
	else
	{
		if (bKeyboard && CWnd::GetFocus() == m_pEdit)
		{
			OnExecute();
		}
		else m_pEdit->SetFocus();
	}

}
void CXTPControlEdit::OnMouseHover()
{
	if (!XTPMouseManager()->IsMouseLocked())
		CXTPControl::OnMouseHover();
}


void CXTPControlEdit::SetHideFlags(DWORD dwFlags)
{
	if (m_dwHideFlags != dwFlags)
	{
		m_dwHideFlags = dwFlags;

		if (m_pEdit && m_pEdit->GetSafeHwnd())
			m_pEdit->SetWindowPos(0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | ((m_dwHideFlags != xtpNoHide)? SWP_HIDEWINDOW: SWP_SHOWWINDOW));
		DelayLayoutParent();
	}
}
void CXTPControlEdit::OnCalcDynamicSize(DWORD dwMode)
{
	if (dwMode & LM_VERTDOCK) SetHideFlags(m_dwHideFlags | xtpHideDockingPosition);
	else SetHideFlags(m_dwHideFlags & ~xtpHideDockingPosition);
}



void CXTPControlEdit::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	ASSERT_KINDOF(CXTPControlEdit, pControl);

	m_nWidth = ((CXTPControlEdit*)pControl)->m_nWidth;
	m_strEditText = ((CXTPControlEdit*)pControl)->GetEditText();
	m_bLabel = ((CXTPControlEdit*)pControl)->m_bLabel;
	SetReadOnly(((CXTPControlEdit*)pControl)->m_bReadOnly);

	CXTPControl::Copy(pControl, bRecursive);
}

BOOL CXTPControlEdit::Compare(CXTPControl* pOther)
{
	if (!CXTPControl ::Compare(pOther))
		return FALSE;

	if (!pOther->IsKindOf(RUNTIME_CLASS(CXTPControlEdit)))
		return FALSE;

	if (m_nWidth != ((CXTPControlEdit*)pOther)->m_nWidth)
		return FALSE;

	return TRUE;
}

void CXTPControlEdit::OnSetFocus(CWnd* pOldWnd)
{
	if (m_pParent->GetTrackFocus() == 0)
		m_pParent->SetTrackFocus(pOldWnd? pOldWnd->GetSafeHwnd(): m_pParent->GetSite()->GetSafeHwnd());

	m_pParent->SetPopuped(-1);

	m_pParent->SetTrackingMode(TRUE, FALSE);
	m_pParent->SetSelected(m_nIndex);
	XTPMouseManager()->LockMouseMove();
	m_pParent->SetFocusedControl(this);
}

void CXTPControlEdit::OnKillFocus()
{
	XTPMouseManager()->UnlockMouseMove();
	m_pParent->SetFocusedControl(NULL);
}

// mini hack.
BEGIN_MESSAGE_MAP(CXTPControlEditCtrl, CEdit)
	ON_WM_SETFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


void CXTPControlEditCtrl::OnContextMenu(CWnd *pWnd, CPoint point)
{
	UNREFERENCED_PARAMETER(pWnd);

	if (!m_pControl->IsCustomizeMode() &&
		m_pControl->GetParent()->GetPosition() != xtpBarPopup)
	{
		ShowContextMenu(m_pControl, point);
	}
}


void CXTPControlEditCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pControl->IsCustomizeMode())
	{
		if (m_pControl->GetParent()->IsCustomizable() && ((m_pControl->GetFlags() & xtpFlagNoMovable) == 0))
			m_pControl->OnClick();
	}
	else CEdit::OnLButtonDown(nFlags, point);
}

void CXTPControlEditCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_pControl->IsCustomizeMode())
	{
		ClientToScreen(&point);
		m_pControl->GetParent()->ScreenToClient(&point);

		m_pControl->GetParent()->OnRButtonDown(nFlags, point);
		return;
	}

	if (m_pControl->GetParent()->GetPosition() != xtpBarPopup)
	{
		if (!ShowContextMenu(m_pControl, point))
			CXTPEdit::OnRButtonDown(nFlags, point);
	}
}

void CXTPControlEditCtrl::OnDestroy()
{
	if (m_pControl) GetWindowText(m_pControl->m_strEditText);
	CEdit::OnDestroy();
}


void CXTPControlEditCtrl::OnSetFocus(CWnd* pOldWnd)
{
	m_pControl->OnSetFocus(pOldWnd);
	CEdit::OnSetFocus(pOldWnd);
	::PostMessage(m_hWnd, EM_SETSEL, 0, -1);
}

void CXTPControlEditCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	m_pControl->OnKillFocus();
}

void CXTPControlEditCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CEdit::OnMouseMove(nFlags, point);

	MapWindowPoints(m_pControl->GetParent(), &point, 1);
	m_pControl->GetParent()->OnMouseMove(nFlags, point);
}

void CXTPControlEditCtrl::OnPaint()
{
	if (m_bRichEditClass)
	{
		Default();
	}
	else
	{
		CPaintDC dcPaint(this);
		CRect rc; GetClientRect(rc);
		CXTPBufferDC dc(dcPaint, rc);

		CWnd::DefWindowProc(WM_PAINT, (WPARAM)dc.m_hDC, 0);
	}
}

BOOL CXTPControlEditCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
	if (m_bRichEditClass)
		return (BOOL)Default();

	return TRUE;
}

HBRUSH CXTPControlEdit::OnCtlColor(CDC* pDC, UINT /*nCtlColor*/) 
{ 
	if (GetEnabled())
	{
		pDC->SetBkColor(GetSysColor(COLOR_WINDOW));
		return GetSysColorBrush(COLOR_WINDOW);
	}
	return 0;
}

HBRUSH CXTPControlEditCtrl::CtlColor(CDC* pDC, UINT nCtlColor)
{
	return m_pControl->OnCtlColor(pDC, nCtlColor);
}




