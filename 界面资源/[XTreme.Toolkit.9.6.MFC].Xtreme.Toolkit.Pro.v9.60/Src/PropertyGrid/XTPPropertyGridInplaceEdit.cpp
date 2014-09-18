// PropertyGripInplaceEdit.cpp : implementation of the CPropertyGripInplaceEdit class.
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

#include "XTPPropertyGridInplaceEdit.h"
#include "XTPPropertyGridInplaceButton.h"
#include "XTPPropertyGridInplaceList.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGridInplaceEdit.h"
#include "XTPPropertyGrid.h"
#include "XTPPropertyGridDefines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceEdit

CXTPPropertyGridInplaceEdit::CXTPPropertyGridInplaceEdit()
: m_pItem(0)
, m_pGrid(0)
{
	m_clrBack = 0;
}

CXTPPropertyGridInplaceEdit::~CXTPPropertyGridInplaceEdit()
{
}

IMPLEMENT_DYNAMIC(CXTPPropertyGridInplaceEdit, CEdit)

BEGIN_MESSAGE_MAP(CXTPPropertyGridInplaceEdit, CXTMaskEditT<CEdit>)
	ON_MASKEDIT_REFLECT()
	//{{AFX_MSG_MAP(CXTPPropertyGridInplaceEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnEnKillfocus)
	ON_CONTROL_REFLECT(EN_SETFOCUS, OnEnSetfocus)
	ON_CONTROL_REFLECT(EN_CHANGE, OnEnChange)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_CHAR()
	ON_WM_SYSKEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceEdit message handlers

void CXTPPropertyGridInplaceEdit::SetValue(CString strValue)
{
	m_strValue = strValue;
}

void CXTPPropertyGridInplaceEdit::HideWindow()
{
	if (m_hWnd)
	{
		ShowWindow(SW_HIDE);
	}
}

void CXTPPropertyGridInplaceEdit::Create(CXTPPropertyGridItem* pItem, CRect rect)
{
	ASSERT(pItem && pItem->GetGrid());
	m_pGrid = pItem->GetGrid();
	m_pItem = pItem;

	if (m_hWnd && (m_pItem->m_bPassword ||
		(!m_pItem->m_bPassword && ((GetStyle() & ES_PASSWORD) != 0))))
	{
		DestroyWindow();
	}

	if (!m_hWnd)
	{
		CEdit::Create(WS_CHILD|ES_AUTOHSCROLL| (m_pItem->m_bPassword? ES_PASSWORD: 0) , rect, m_pGrid, 0);

		if (m_pGrid->GetExStyle() & WS_EX_LAYOUTRTL)
		{
			ModifyStyleEx(0, WS_EX_LAYOUTRTL);
		}
	}
	if (m_pItem->m_bPassword)
		CEdit::SetPasswordChar(pItem->m_chPrompt);

	SetFont(m_pGrid->GetFont());
	SetWindowText(m_strValue);
	SetWindowPos(0, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER|SWP_SHOWWINDOW);

	SetMargins(m_pGrid->GetExStyle() & WS_EX_LAYOUTRTL? 2: 3, 0);


	SetUseMask(m_pItem->m_bUseMask);
	if (m_pItem->m_bUseMask)
	{
		SetEditMask(m_pItem->m_strMask, m_pItem->m_strLiteral, m_pItem->m_strValue);
		SetPromptChar(m_pItem->m_chPrompt);
	}



}

HBRUSH CXTPPropertyGridInplaceEdit::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	CXTPPropertyGridView* pGrid = (CXTPPropertyGridView*)m_pGrid;

	pDC->SetTextColor(GetStyle() & ES_READONLY? pGrid->m_clrReadOnlyFore: pGrid->m_clrFore);

	COLORREF clr = pGrid->m_clrBack;

	if (clr != m_clrBack || !m_brBack.GetSafeHandle())
	{
		m_brBack.DeleteObject();
		m_brBack.CreateSolidBrush(clr);
		m_clrBack = clr;

	}

	pDC->SetBkColor(m_clrBack);


	return m_brBack;
}

void CXTPPropertyGridInplaceEdit::OnEnSetfocus()
{
	if (!m_pGrid)
		return;

	m_pGrid->Invalidate(FALSE);

#ifdef XTP_SITENOTIFY_ONFOCUS
	XTP_SITENOTIFY_ONFOCUS(this, m_pGrid, TRUE)
#endif

}

void CXTPPropertyGridInplaceEdit::OnEnKillfocus()
{
	if (!m_pGrid)
		return;

	if (m_pItem)
	{
		m_pItem->OnValidateEdit();
		if (m_pGrid) m_pGrid->Invalidate(FALSE);
	}

#ifdef XTP_SITENOTIFY_ONFOCUS
	XTP_SITENOTIFY_ONFOCUS(this, m_pGrid, FALSE)
#endif
}

void CXTPPropertyGridInplaceEdit::OnEnChange()
{
	ASSERT(m_pItem);

	CXTPPropertyGrid* pCtrl = (CXTPPropertyGrid*)m_pGrid->GetParent();
	ASSERT_KINDOF(CXTPPropertyGrid, pCtrl);

	CWnd* pOwner = pCtrl->GetOwner();
	ASSERT(pOwner);
	pOwner->SendMessage(XTPWM_PROPERTYGRID_NOTIFY, XTP_PGN_EDIT_CHANGED, (LPARAM)this);

}

UINT CXTPPropertyGridInplaceEdit::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}

void CXTPPropertyGridInplaceEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_TAB) return;
	if (nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		m_pGrid->SetFocus();
		return;
	}
	if (m_pItem && !m_pItem->GetReadOnly() && m_pItem->GetConstraintEdit())
	{
		CXTPPropertyGridItemConstraints* pList = m_pItem->GetConstraints();
		if (!pList->IsEmpty())
		{

			CString str;
			GetWindowText(str);
			int nIndex = pList->FindConstraint(str);
			int nIndexStart = nIndex == -1? pList->GetCount() - 1: nIndex;

			CString strSeach ((TCHAR)nChar);

			do
			{

				nIndex = nIndex < pList->GetCount() - 1? nIndex + 1: 0;
				CString str = pList->GetAt(nIndex);

				if (strSeach.CompareNoCase(str.Left(1)) == 0)
				{
					SetWindowText(str);
					pList->SetCurrent(nIndex);
					SetSel(0, -1);
					return;
				}

			} while (nIndex != nIndexStart);

			return;
		}
	}

	CXTMaskEditT<CEdit>::OnChar(nChar, nRepCnt, nFlags);
}
void CXTPPropertyGridInplaceEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_TAB && m_pGrid)
	{
		((CXTPPropertyGrid*)m_pGrid->GetParent())->OnNavigate(
			xtpGridUIInplaceEdit, GetKeyState(VK_SHIFT) >= 0, m_pItem);
		return;
	}
	else if (nChar == VK_ESCAPE)
	{
		SetWindowText(m_strValue);
		return ;
	}
	else if (nChar == VK_RETURN)
	{
		return ;
	}
	else if (nChar == VK_F4 )
	{
		if (m_pItem && m_pItem->GetFlags() & xtpGridItemHasComboButton)
			m_pItem->OnInplaceButtonDown();
	}
	else if (nChar == VK_DOWN || nChar == VK_UP)
	{
		if (m_pItem != NULL && !m_pItem->GetReadOnly())
		{
			if (SelectConstraint(nChar == VK_DOWN? +1: -1, FALSE))
			{
				SetSel(0, -1);
				return ;
			}
		}
	}

	CXTMaskEditT<CEdit>::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPPropertyGridInplaceEdit::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_DOWN || nChar == VK_UP)
	{
		if (m_pItem && m_pItem->GetFlags() & xtpGridItemHasComboButton)
			m_pItem->OnInplaceButtonDown();
	}

	CXTMaskEditT<CEdit>::OnSysKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CXTPPropertyGridInplaceEdit::SelectConstraint(int nDirection, BOOL bCircle)
{
	CXTPPropertyGridItemConstraints* pList = m_pItem->GetConstraints();
	if (pList->IsEmpty())
		return FALSE;

	CString str;
	GetWindowText(str);
	int nIndex = pList->FindConstraint(str);

	if (nIndex == -1)
		return FALSE;

	nIndex += nDirection;
	if (nIndex >= pList->GetCount()) nIndex = bCircle? 0: (ULONG)pList->GetCount() - 1;
	if (nIndex < 0) nIndex = bCircle? (ULONG)pList->GetCount() - 1: 0;

	SetWindowText(pList->GetAt(nIndex));
	pList->SetCurrent(nIndex);

	return TRUE;
}

void CXTPPropertyGridInplaceEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_pItem != NULL && !m_pItem->GetReadOnly() && SelectConstraint(+1, TRUE))
	{
		m_pGrid->SetFocus();
		return;
	}
	CXTMaskEditT<CEdit>::OnLButtonDblClk(nFlags, point);
}

void CXTPPropertyGridInplaceEdit::DestroyItem()
{
	// reset variables to defaults.
	m_pItem = NULL;
	m_pGrid = NULL;
	m_strValue.Empty();
	m_brBack.DeleteObject();

	// destroy the window.
	//DestroyWindow( );
	if (::IsWindow(m_hWnd)) ShowWindow(SW_HIDE);
}


BOOL CXTPPropertyGridInplaceEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && IsDialogMessage(pMsg))
		return TRUE;
	return CXTMaskEditT<CEdit>::PreTranslateMessage(pMsg);
}
