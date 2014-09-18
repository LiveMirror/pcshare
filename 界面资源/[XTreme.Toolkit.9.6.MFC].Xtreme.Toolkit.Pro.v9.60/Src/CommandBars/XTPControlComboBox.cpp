// XTPControlComboBox.cpp : implementation of the CXTPControlComboBox class.
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
#include "XTPPropExchange.h"

#include "XTPControlComboBox.h"
#include "XTPCommandBar.h"
#include "XTPToolBar.h"
#include "XTPCommandBars.h"
#include "XTPPaintManager.h"
#include "XTPMouseManager.h"
#include "XTPControls.h"

#ifdef _UNICODE
BOOL CXTPEdit::m_bRichEditClass = TRUE;
#else
BOOL CXTPEdit::m_bRichEditClass = FALSE;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CXTPEdit

IMPLEMENT_DYNCREATE(CXTPEdit, CEdit)

BOOL CXTPEdit::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* /*pHandlerInfo*/)
{
	if (nID == ID_EDIT_PASTE && nCode == CN_UPDATE_COMMAND_UI)
	{
		CCmdUI* pCmdUI = (CCmdUI*)pExtra;
		pCmdUI->Enable(::IsClipboardFormatAvailable(CF_TEXT));
	} else if ((nID == ID_EDIT_CUT || nID == ID_EDIT_COPY) && nCode == CN_UPDATE_COMMAND_UI)
	{
		CCmdUI* pCmdUI = (CCmdUI*)pExtra;
		int nStartChar, nEndChar;
		GetSel(nStartChar, nEndChar);
		pCmdUI->Enable(nStartChar != nEndChar);
	} else

		return FALSE;

	return TRUE;
}

BOOL CXTPEdit::OnCommand(WPARAM wParam, LPARAM)
{
	UINT nID = LOWORD(wParam);

	if (nID == ID_EDIT_PASTE) Paste();
	else if (nID == ID_EDIT_COPY) Copy();
	else if (nID == ID_EDIT_CUT) Cut();
	else return FALSE;

	return TRUE;
}

BOOL CXTPEdit::ShowContextMenu(CXTPControl* pControl, CPoint point)
{
	CString strPrompt;
	if (!strPrompt.LoadString(ID_EDIT_CUT))
		return FALSE;

	BOOL bFocus = GetFocus() == this;

	if (!bFocus) SetSel(0, -1);

	XTPMouseManager()->SendTrackLost();

	ClientToScreen(&point);

	CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(pControl->GetParent()->GetCommandBars());
	pPopupBar->GetControls()->Add(xtpControlButton, ID_EDIT_CUT);
	pPopupBar->GetControls()->Add(xtpControlButton, ID_EDIT_COPY);
	pPopupBar->GetControls()->Add(xtpControlButton, ID_EDIT_PASTE);


	int nCommand = CXTPCommandBars::TrackPopupMenu(pPopupBar, TPM_RETURNCMD, point.x, point.y, this, NULL, this);

	if (nCommand > 0)
	{
		if (bFocus || nCommand == ID_EDIT_PASTE)
			SetFocus();
		this->OnCommand(nCommand, 0);
	}

	pPopupBar->InternalRelease();

	pControl->GetParent()->Invalidate(FALSE);
	pControl->GetParent()->UpdateWindow();

	return TRUE;
}

BOOL CXTPEdit::CreateEdit(DWORD dwStyle, CWnd* pParentWnd)
{
	if (m_bRichEditClass)
	{
		if (AfxInitRichEdit())
			return CWnd::Create(_T("RICHEDIT"), 0, dwStyle, CRect(0, 0, 0, 0), pParentWnd, 0);
	}

	return Create(dwStyle, CRect(0, 0, 0, 0), pParentWnd, 0);
}



// CXTPControlComboBox

IMPLEMENT_XTP_CONTROL(CXTPControlComboBox, CXTPControlPopup)

CXTPControlComboBox::CXTPControlComboBox(void)
{
	EnableAutomation();

	m_controlType = xtpControlComboBox;
	m_bDropDown = FALSE;
	m_comboStyle = xtpComboNormal;
	m_nWidth = 100;
	m_nLabelWidth = 0;
	m_pEdit = NULL;
	m_strEditText = _T("");
	m_nLastSel = -1;
	m_bDelayReposition = FALSE;

	m_pCommandBar = new CXTPControlComboBoxList();
	((CXTPControlComboBoxList*)m_pCommandBar)->CreateListBox();
}

CString CXTPControlComboBox::GetEditText()
{
	if (m_pEdit && m_pEdit->GetSafeHwnd())
	{
		m_pEdit->GetWindowText(m_strEditText);
	}
	return m_strEditText;
}

void CXTPControlComboBox::SetEditText(LPCTSTR lpszText,BOOL bMatchExact)
{
	int idx = (bMatchExact) ? FindStringExact(-1,lpszText) :
	FindString(-1, lpszText);
	SetCurSel( (idx==CB_ERR) ? -1 : idx);

	if (m_pEdit && m_pEdit->GetSafeHwnd() && GetEditText() != lpszText)
	{
		m_pEdit->SetWindowText(lpszText);
	}
	else m_strEditText = lpszText;
}

CXTPControlComboBox::~CXTPControlComboBox(void)
{
	if (m_pEdit)
	{
		delete m_pEdit;
	}
}
void CXTPControlComboBox::OnSelChanged()
{
	if (m_pEdit && m_pEdit->GetSafeHwnd()) m_pEdit->SetWindowText(GetText()); else m_strEditText = GetText();
	if (m_pParent && m_pParent->GetSafeHwnd()) m_pParent->Invalidate(FALSE);
}

BOOL CXTPControlComboBox::OnKeyDown(UINT nChar)
{
	if (m_pEdit)
	{
		if (nChar == VK_ESCAPE)
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

		if (nChar == VK_F4)
		{
			OnClick(FALSE);
		}

		return FALSE_EXIT;
	}
	return TRUE;
}

BOOL CXTPControlComboBox::GetDropDownListStyle()
{
	return m_pEdit != NULL;
}

CXTPControlComboBoxEditCtrl* CXTPControlComboBox::CreateEditControl()
{
	return new CXTPControlComboBoxEditCtrl();
}

void CXTPControlComboBox::SetDropDownListStyle(BOOL bSet)
{
	m_bDropDown = bSet;

	if (bSet && m_pEdit == NULL)
	{
		m_pEdit = CreateEditControl();
		m_pEdit->m_pControl = this;
	}
	else if (!bSet && m_pEdit != NULL)
	{
		delete m_pEdit;
		m_pEdit = NULL;
	}
}

void CXTPControlComboBox::SetRect(CRect rcControl)
{
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
		rcControl.DeflateRect(m_nLabelWidth + 1 + 3, 3, m_nThumbWidth + 3, 3);
		m_pEdit->MoveWindow(rcControl);
		m_pEdit->SetWindowPos(0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | ((m_dwHideFlags != xtpNoHide)? SWP_HIDEWINDOW: SWP_SHOWWINDOW));
	}
	m_bDelayReposition = FALSE;
}
void CXTPControlComboBox::SetEnabled(BOOL bEnabled)
{
	if (bEnabled != m_bEnabled)
	{
		m_bEnabled = bEnabled;
		if (m_pEdit && m_pEdit->GetSafeHwnd())
			m_pEdit->EnableWindow(bEnabled);

		DelayRedrawParent();
	}
}

void CXTPControlComboBox::SetParent(CXTPCommandBar* pParent)
{
	if (pParent != m_pParent && pParent)
	{
		CWnd* pSite = pParent->GetSite();
		if (pSite)
			SetWindowLongPtr(m_pCommandBar->GetSafeHwnd(), GWL_HWNDPARENT, (LONG_PTR)pSite->GetSafeHwnd());
	}
	if (!pParent && m_pEdit && m_pEdit->GetSafeHwnd())
	{
		m_pEdit->DestroyWindow();

	}
	CXTPControlPopup::SetParent(pParent);
}
BOOL  CXTPControlComboBox::OnSetSelected(int bSelected)
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
void CXTPControlComboBox::OnClick(BOOL bKeyboard , CPoint pt)
{
	if (IsCustomizeMode())
	{
		m_pParent->SetPopuped(-1);
		m_pParent->SetSelected(-1);
		CustomizeStartDrag(pt);
	}
	else if (bKeyboard && m_pEdit)
	{
		if (CWnd::GetFocus() == m_pEdit)
		{
			OnExecute();
		} else
		m_pEdit->SetFocus();
	}
	else CXTPControlPopup::OnClick(bKeyboard, pt);
}
void CXTPControlComboBox::OnMouseHover()
{
	if (!XTPMouseManager()->IsMouseLocked())
	{
		if (m_pCommandBar->IsKindOf(RUNTIME_CLASS(CXTPControlComboBoxList)))
			CXTPControl::OnMouseHover(); else CXTPControlPopup::OnMouseHover();
	}
}


void CXTPControlComboBox::SetHideFlags(DWORD dwFlags)
{
	if (m_dwHideFlags != dwFlags)
	{
		m_dwHideFlags = dwFlags;

		if (m_pEdit && m_pEdit->GetSafeHwnd())
			m_pEdit->SetWindowPos(0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | ((m_dwHideFlags != xtpNoHide)? SWP_HIDEWINDOW: SWP_SHOWWINDOW));

		DelayLayoutParent();
	}
}
void CXTPControlComboBox::OnCalcDynamicSize(DWORD dwMode)
{
	if (dwMode & LM_VERTDOCK) SetHideFlags(m_dwHideFlags | xtpHideDockingPosition);
	else SetHideFlags(m_dwHideFlags & ~xtpHideDockingPosition);
}



void CXTPControlComboBox::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	ASSERT(DYNAMIC_DOWNCAST(CXTPControlComboBox, pControl));

	m_comboStyle = ((CXTPControlComboBox*)pControl)->m_comboStyle;
	m_nWidth = ((CXTPControlComboBox*)pControl)->m_nWidth;
	m_strEditText = ((CXTPControlComboBox*)pControl)->GetEditText();
	SetDropDownListStyle(((CXTPControlComboBox*)pControl)->m_pEdit != NULL);

	CXTPControlPopup::Copy(pControl, bRecursive);
}

BOOL CXTPControlComboBox::Compare(CXTPControl* pOther)
{
	if (!CXTPControlPopup::Compare(pOther))
		return FALSE;

	if (!pOther->IsKindOf(RUNTIME_CLASS(CXTPControlComboBox)))
		return FALSE;

	if (m_nWidth != ((CXTPControlComboBox*)pOther)->m_nWidth)
		return FALSE;

	return TRUE;
}

CString CXTPControlComboBox::GetText()
{
	if (!::IsWindow(m_pCommandBar->m_hWnd))
		return _T("");

	int nSel = ((CListBox*)m_pCommandBar)->GetCurSel();
	CString str;
	if (nSel >= 0)
	((CListBox*)m_pCommandBar)->GetText(nSel, str);
	return str;
}

int CXTPControlComboBox::GetCurSel()
{
	return m_pCommandBar->IsKindOf(RUNTIME_CLASS(CXTPControlComboBoxList)) && m_pCommandBar->m_hWnd?
		((CListBox*)m_pCommandBar)->GetCurSel(): -1;
}

void CXTPControlComboBox::SetCurSel(int nIndex)
{
	if (GetCurSel() != nIndex)
	{
		((CListBox*)m_pCommandBar)->SetCurSel(nIndex);
		OnSelChanged();
	}
}

void CXTPControlComboBox::OnSetFocus(CWnd* pOldWnd)
{
	if (m_pParent->GetTrackFocus() == 0)
		m_pParent->SetTrackFocus(pOldWnd? pOldWnd->GetSafeHwnd(): m_pParent->GetSite()->GetSafeHwnd());

	m_pParent->SetPopuped(-1);

	m_pParent->SetTrackingMode(TRUE, FALSE);
	m_pParent->SetSelected(m_nIndex);
	XTPMouseManager()->LockMouseMove();
	m_pParent->SetFocusedControl(this);
}

void CXTPControlComboBox::OnKillFocus()
{
	XTPMouseManager()->UnlockMouseMove();
	m_pParent->SetFocusedControl(NULL);
}

BOOL CXTPControlComboBox::GetDroppedState()
{
	return m_pCommandBar->IsTrackingMode() && ((CXTPPopupBar*)m_pCommandBar)->GetControlPopup() == this;
}

BOOL CXTPControlComboBox::OnSetPopup(BOOL bPopup)
{
	if (m_pEdit == NULL)
	{
		if (bPopup)
		{
			m_nLastSel = GetCurSel();
		}
		else
		{
			SetCurSel(m_nLastSel);
		}
	}

	return CXTPControlPopup::OnSetPopup(bPopup);
}

void CXTPControlComboBox::OnExecute()
{
	m_nLastSel = GetCurSel();

	CXTPControlPopup::OnExecute();
}


//////////////////////////////////////////////////////////////////////////
// CXTPControlComboBoxList

BEGIN_MESSAGE_MAP(CXTPControlComboBoxList, CXTPPopupBar)
	ON_WM_LBUTTONUP()
	ON_WM_NCPAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


IMPLEMENT_XTP_COMMANDBAR(CXTPControlComboBoxList, CXTPPopupBar)

void CXTPControlComboBoxList::CreateListBox()
{
	CreateEx(WS_EX_STATICEDGE|WS_EX_TOOLWINDOW, _T("LISTBOX"), _T(""), WS_POPUP|WS_VSCROLL|WS_BORDER|WS_CLIPCHILDREN, CRect(0,0,0,0), 0,  0);
}

void CXTPControlComboBoxList::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/)
{
	// do nothing.
}


int CXTPControlComboBoxList::OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult)
{
	if (nMessage == WM_MOUSEWHEEL)
	{
		CWnd::DefWindowProc(WM_MOUSEWHEEL, wParam, lParam);
		return TRUE;
	}
	return CXTPCommandBar::OnHookMessage(hWnd, nMessage, wParam, lParam, lResult);
}


void CXTPControlComboBoxList::OnNcPaint()
{
	CXTPPaintManager* pPaintManager = GetPaintManager();

	Default();
	CWindowDC dc(this);
	CXTPWindowRect rc(this);
	rc.OffsetRect(-rc.TopLeft());
	dc.Draw3dRect(rc, pPaintManager->GetXtremeColor(XPCOLOR_MENUBAR_BORDER), pPaintManager->GetXtremeColor(XPCOLOR_MENUBAR_BORDER));

	rc.DeflateRect(1, 1);
	dc.Draw3dRect(rc, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_WINDOW));

}

BOOL CXTPControlComboBoxList::OnEraseBkgnd(CDC* pDC)
{
	return CWnd::OnEraseBkgnd(pDC);
}

void CXTPControlComboBoxList::OnLButtonUp(UINT /*nFlags*/, CPoint /*point*/)
{
	((CXTPControlComboBox*)m_pControlPopup)->OnSelChanged();
	m_pControlPopup->OnExecute();
}

BOOL CXTPControlComboBoxList::SetTrackingMode(int bMode, BOOL bSelectFirst, BOOL bKeyboard)
{
	if (!CXTPCommandBar::SetTrackingMode(bMode, bSelectFirst, bKeyboard))
		return FALSE;

	if (!bMode)
	{
		XTPMouseManager()->UnlockMouseMove();
		ShowWindow(SW_HIDE);

		if (GetParentCommandBar())
			GetParentCommandBar()->SetPopuped(-1);

		m_pControlPopup = 0;

	} else
		XTPMouseManager()->LockMouseMove();

	return TRUE;
}

BOOL CXTPControlComboBoxList::OnKeyDown(UINT nChar)
{
	ASSERT(m_pControlPopup);

	if (nChar == VK_ESCAPE) return CXTPCommandBar::OnKeyDown(nChar);
	if (nChar == VK_TAB) return FALSE;
	if (nChar == VK_RETURN)
	{
		m_pControlPopup->OnExecute();
		return TRUE;
	}
	int nSel = ((CListBox*)this)->GetCurSel();

	CWnd::DefWindowProc(WM_KEYDOWN, (WPARAM)nChar, 0);

	if (nSel != ((CListBox*)this)->GetCurSel())
	{
		((CXTPControlComboBox*)m_pControlPopup)->OnSelChanged();
	}
	return TRUE;
}

void CXTPControlComboBoxList::OnMouseMove(UINT nFlags, CPoint point)
{
	BOOL bOutside;
	UINT nItem = ((CListBox*)(this))->ItemFromPoint(point,  bOutside);

	if (!bOutside && ((CListBox*)(this))->GetCurSel() != (int)nItem)
		((CListBox*)(this))->SetCurSel(nItem);

	CXTPPopupBar::OnMouseMove(nFlags, point);
}



BOOL CXTPControlComboBoxList::Popup(CXTPControlPopup* pControlPopup, BOOL /*bSelectFirst*/)
{
	SetFont(GetPaintManager()->GetIconFont());

	m_pControlPopup = pControlPopup;

	CWnd* pWndOwner = GetOwnerSite();
	if (pWndOwner) pWndOwner->SendMessage(WM_XTP_INITCOMMANDSPOPUP, 0, (LPARAM)this);

	SetTrackingMode(TRUE, FALSE);

	UpdateFlags();

	CRect rcControl = pControlPopup->GetRect();

	int nItemHeight = ((CListBox*)this)->GetItemHeight(0);
	int nHeight = min (12, max(1, ((CListBox*)this)->GetCount())) * nItemHeight + 4;
	int nWidth = m_nMRUWidth <= 0? rcControl.Width(): m_nMRUWidth;
	UpdateLocation(CSize(nWidth, nHeight));

	EnableWindow();

	return TRUE;
}

void CXTPControlComboBoxList::DrawCommandBar(CDC* pDC)
{
	pDC->FillSolidRect(CXTPClientRect(this), GetXtremeColor(COLOR_WINDOW));
	CWnd::DefWindowProc(WM_PAINT, (WPARAM)pDC->m_hDC, 0);
}

//////////////////////////////////////////////////////////////////////////
// CXTPControlComboBoxEditCtrl

BEGIN_MESSAGE_MAP(CXTPControlComboBoxEditCtrl, CXTPEdit)
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


void CXTPControlComboBoxEditCtrl::OnContextMenu(CWnd *pWnd, CPoint point)
{
	UNREFERENCED_PARAMETER(pWnd);

	if (!m_pControl->IsCustomizeMode() &&
		m_pControl->GetParent()->GetPosition() != xtpBarPopup)
	{
		ShowContextMenu(m_pControl, point);
	}
}

void CXTPControlComboBoxEditCtrl::OnRButtonDown(UINT nFlags, CPoint point)
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

void CXTPControlComboBoxEditCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pControl->IsCustomizeMode())
	{
		if (m_pControl->GetParent()->IsCustomizable() && ((m_pControl->GetFlags() & xtpFlagNoMovable) == 0))
			m_pControl->OnClick();
	}
	else CEdit::OnLButtonDown(nFlags, point);
}

void CXTPControlComboBoxEditCtrl::OnDestroy()
{
	GetWindowText(m_pControl->m_strEditText);
	CEdit::OnDestroy();
}


void CXTPControlComboBoxEditCtrl::OnSetFocus(CWnd* pOldWnd)
{
	m_pControl->OnSetFocus(pOldWnd);
	CEdit::OnSetFocus(pOldWnd);
	::PostMessage(m_hWnd, EM_SETSEL, 0, -1);
}

void CXTPControlComboBoxEditCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	m_pControl->OnKillFocus();
}

void CXTPControlComboBoxEditCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CEdit::OnMouseMove(nFlags, point);

	MapWindowPoints(m_pControl->GetParent(), &point, 1);
	m_pControl->GetParent()->OnMouseMove(nFlags, point);
}

void CXTPControlComboBoxEditCtrl::OnPaint()
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

		dc.FillSolidRect(rc, IsWindowEnabled()? GetXtremeColor(COLOR_WINDOW): GetXtremeColor(COLOR_3DFACE));
		CWnd::DefWindowProc(WM_PAINT, (WPARAM)dc.m_hDC, 0);
	}
}

BOOL CXTPControlComboBoxEditCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
	if (m_bRichEditClass)
		return (BOOL)Default();

	return TRUE;
}

HBRUSH CXTPControlComboBoxEditCtrl::CtlColor(CDC* pDC, UINT nCtlColor)
{
	return m_pControl->OnCtlColor(pDC, nCtlColor);
}

void CXTPControlComboBoxList::Copy(CXTPCommandBar* pCommandBar, BOOL bRecursive)
{
	CXTPPopupBar::Copy(pCommandBar, bRecursive);

	if (!m_hWnd)
		CreateListBox();

	CListBox* pListBox = (CListBox*)(CXTPControlComboBoxList*)pCommandBar;
	DWORD dwCount = pListBox->m_hWnd? pListBox->GetCount(): 0;

	for (UINT i = 0; i < dwCount; i++)
	{
		CString str;
		pListBox->GetText(i, str);
		((CListBox*)this)->AddString(str);
	}

}

void CXTPControlComboBoxList::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPopupBar::DoPropExchange(pPX);

	CListBox* pThis = (CListBox*)this;
	CString str;

	CXTPPropExchangeSection secItems(pPX->GetSection(_T("Items")));

	if (pPX->IsStoring())
	{
		if (m_hWnd)
		{
			DWORD dwCount = pThis->GetCount();
			secItems->WriteCount(dwCount);

			for (int i = 0; i < (int)dwCount; i++)
			{
				pThis->GetText(i, str);

				CString strSection;
				strSection.Format(_T("Item%i"), i);
				PX_String(&secItems, strSection, str, _T(""));

			}
		}
		else
		{
			pPX->WriteCount(0);
		}
	}
	else
	{
		if (!m_hWnd)
			CreateListBox();

		DWORD nNewCount = secItems->ReadCount();
		for (DWORD i = 0; i < nNewCount; i++)
		{
			CString strSection;
			strSection.Format(_T("Item%i"), i);

			PX_String(&secItems, strSection, str, _T(""));
			pThis->AddString(str);
		}
	}
}


int CXTPControlComboBox::GetDropDownWidth()
{
	return m_pCommandBar->GetWidth();
}

int CXTPControlComboBox::GetCustomizeMinWidth()
{
	return m_nLabelWidth + m_nThumbWidth + 5;

}


