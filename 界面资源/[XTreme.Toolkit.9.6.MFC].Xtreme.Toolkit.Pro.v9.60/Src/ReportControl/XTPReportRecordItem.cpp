// XTPReportRecordItem.cpp : implementation of the CXTPReportRecordItem class.
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

#include "XTPReportRow.h"
#include "XTPReportColumns.h"
#include "XTPReportColumn.h"
#include "XTPReportControl.h"
#include "XTPReportPaintManager.h"
#include "XTPReportHyperlink.h"
#include "XTPReportRecord.h"
#include "XTPReportRecordItem.h"
#include "XTPReportInplaceControls.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef IDC_HAND
#define IDC_HAND MAKEINTRESOURCE(32649)
#endif

//////////////////////////////////////////////////////////////////////////
// XTP_REPORTRECORDITEM_ARGS

XTP_REPORTRECORDITEM_ARGS::XTP_REPORTRECORDITEM_ARGS()
{
	pControl = NULL;
	pRow = NULL;
	pColumn = NULL;
	pItem = NULL;
	rcItem.SetRectEmpty();
}

XTP_REPORTRECORDITEM_ARGS::XTP_REPORTRECORDITEM_ARGS(CXTPReportControl* pControl, CXTPReportRow* pRow, CXTPReportColumn* pColumn)
{
	ASSERT(pControl && pRow && pColumn);

	this->pControl = pControl;
	this->pRow = pRow;
	this->pColumn = pColumn;
	this->pItem = pRow->GetRecord()->GetItem(pColumn);
	this->rcItem = pRow->GetItemRect(pItem);
}
XTP_REPORTRECORDITEM_ARGS::~XTP_REPORTRECORDITEM_ARGS()
{

}

CXTPReportInplaceButtons::CXTPReportInplaceButtons()
{

}

//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemConstraint

CXTPReportRecordItemConstraint::CXTPReportRecordItemConstraint()
{
}

int CXTPReportRecordItemConstraint::GetIndex()
{
	return m_nIndex;
}

//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemConstraints

CXTPReportRecordItemConstraints::CXTPReportRecordItemConstraints()
{
}
CXTPReportRecordItemConstraints::~CXTPReportRecordItemConstraints()
{
	RemoveAll();
}


int CXTPReportRecordItemConstraints::GetCount()
{
	return (int)m_arrConstraints.GetSize();
}

CXTPReportRecordItemConstraint* CXTPReportRecordItemConstraints::GetAt(int nIndex)
{
	return m_arrConstraints.GetAt(nIndex);
}

void CXTPReportRecordItemConstraints::RemoveAll()
{
	for (int i = 0; i < GetCount(); i++)
		m_arrConstraints[i]->InternalRelease();
	m_arrConstraints.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemEditOptions

CXTPReportRecordItemEditOptions::CXTPReportRecordItemEditOptions()
{
	m_bAllowEdit = TRUE;
	m_bConstraintEdit = FALSE;
	m_pConstaints = new CXTPReportRecordItemConstraints();


}

CXTPReportRecordItemEditOptions::~CXTPReportRecordItemEditOptions()
{
	for (int j = 0; j < arrInplaceButtons.GetSize(); j++)
		delete arrInplaceButtons[j];

	m_pConstaints->InternalRelease();
}

void CXTPReportRecordItemEditOptions::AddComboButton()
{
	arrInplaceButtons.Add(new CXTPReportInplaceButton(XTP_ID_REPORT_COMBOBUTTON));
}

void CXTPReportRecordItemEditOptions::AddExpandButton()
{
	arrInplaceButtons.Add(new CXTPReportInplaceButton(XTP_ID_REPORT_EXPANDBUTTON));
}

CXTPReportRecordItemConstraint* CXTPReportRecordItemEditOptions::FindConstraint(DWORD_PTR dwData)
{
	for (int i = 0; i < m_pConstaints->GetCount(); i++)
	{
		CXTPReportRecordItemConstraint* pConstaint = m_pConstaints->GetAt(i);
		if (pConstaint->m_dwData == dwData)
			return pConstaint;
	}
	return NULL;
}

CXTPReportRecordItemConstraint* CXTPReportRecordItemEditOptions::FindConstraint(LPCTSTR lpszConstraint)
{
	for (int i = 0; i < m_pConstaints->GetCount(); i++)
	{
		CXTPReportRecordItemConstraint* pConstaint = m_pConstaints->GetAt(i);
		if (pConstaint->m_strConstraint == lpszConstraint)
			return pConstaint;
	}
	return NULL;
}

CXTPReportRecordItemConstraint* CXTPReportRecordItemEditOptions::AddConstraint(LPCTSTR lpszConstraint, DWORD_PTR dwData /*= 0*/)
{
	CXTPReportRecordItemConstraint* pConstaint = new CXTPReportRecordItemConstraint();

	pConstaint->m_strConstraint = lpszConstraint;
	pConstaint->m_dwData = dwData;
	pConstaint->m_nIndex = (int)m_pConstaints->m_arrConstraints.Add(pConstaint);

	return pConstaint;
}

//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItem

IMPLEMENT_DYNAMIC(CXTPReportRecordItem, CCmdTarget)

CXTPReportRecordItem::CXTPReportRecordItem()
	: m_strFormatString(_T("%s")), m_bEditable(TRUE)
{
	m_pFontCaption = NULL;
	m_bBoldText = FALSE;
	m_clrText = XTP_REPORT_COLOR_DEFAULT;
	m_clrBackground = XTP_REPORT_COLOR_DEFAULT;


	m_nIconIndex = XTP_REPORT_NOICON;

	m_pRecord = 0;
	m_dwData = 0;

	m_nSortPriority = -1;
	m_nGroupPriority = -1;

	m_bChecked = FALSE;
	m_bHasCheckbox = FALSE;
	m_pEditOptions = NULL;

	m_bFocusable = TRUE;
}


CXTPReportRecordItem::~CXTPReportRecordItem(void)
{
	for (int nItem = (int)m_arrHyperlinks.GetSize() - 1; nItem >= 0; nItem--)
	{
		CXTPReportHyperlink* pItem = m_arrHyperlinks.GetAt(nItem);
		pItem->InternalRelease();
	}

	if (m_pEditOptions)
	{
		m_pEditOptions->InternalRelease();
	}
}

void CXTPReportRecordItem::DoMouseButtonClick()
{
	mouse_event(GetSystemMetrics(SM_SWAPBUTTON)? MOUSEEVENTF_RIGHTDOWN: MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(GetSystemMetrics(SM_SWAPBUTTON)? MOUSEEVENTF_RIGHTUP: MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void CXTPReportRecordItem::GetCaptionRect(XTP_REPORTRECORDITEM_ARGS* pDrawArgs, CRect& rcItem)
{
	ASSERT(pDrawArgs->pControl);

	if (m_nIconIndex != XTP_REPORT_NOICON)
		rcItem.left = pDrawArgs->pControl->GetPaintManager()->DrawBitmap(NULL, pDrawArgs->pControl, rcItem, m_nIconIndex);

	if (m_bHasCheckbox)
		rcItem.left += 15;
}

void CXTPReportRecordItem::DrawCheckBox(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, CRect& rcItem)
{
	BOOL bEditable = IsEditable() && (pDrawArgs->pColumn == NULL || pDrawArgs->pColumn->IsEditable());

	CDC* pDC = pDrawArgs->pDC;
	int eState = (bEditable? 0: 2) + (m_bChecked? 1: 0);

	CXTPReportPaintManager* pPaintManager = pDrawArgs->pControl->GetPaintManager();

	rcItem.left += 2;
	rcItem.left = pPaintManager->DrawGlyph(pDC, rcItem, eState + 2);


}

void CXTPReportRecordItem::OnDrawCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pMetrics)
{
	ASSERT(pDrawArgs->pItem == this);
	pDrawArgs->pControl->GetPaintManager()->DrawItemCaption(pDrawArgs, pMetrics);
}


int CXTPReportRecordItem::Draw(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs)
{
	CDC* pDC = pDrawArgs->pDC;
	CRect& rcItem = pDrawArgs->rcItem;
	CXTPReportPaintManager* pPaintManager = pDrawArgs->pControl->GetPaintManager();

	CRgn rgn;
	rgn.CreateRectRgn(rcItem.left, rcItem.top - 1, rcItem.right, rcItem.bottom);

	if (!pDC->IsPrinting())
		pDC->SelectClipRgn(&rgn);

	XTP_REPORTRECORDITEM_METRICS* pMetrics = new XTP_REPORTRECORDITEM_METRICS();
	pMetrics->strText = GetCaption(pDrawArgs->pColumn);
	pDrawArgs->pRow->GetItemMetrics(pDrawArgs, pMetrics);

	ASSERT(pMetrics->pFont);
	ASSERT(pMetrics->clrForeground != XTP_REPORT_COLOR_DEFAULT);

	if (pMetrics->clrBackground != XTP_REPORT_COLOR_DEFAULT)
		pDC->FillSolidRect(rcItem, pMetrics->clrBackground);

	// draw tree inside item rect (see also HitTest function)
	if (pDrawArgs->pColumn && pDrawArgs->pColumn->IsTreeColumn())
	{
		int nTreeDepth = pDrawArgs->pRow->GetTreeDepth() - pDrawArgs->pRow->GetGroupLevel();
		if (nTreeDepth > 0)
			nTreeDepth++;
		rcItem.left += pDrawArgs->pControl->GetIndent(nTreeDepth);

		BOOL bHasChildren = pDrawArgs->pRow->HasChildren();

		pPaintManager->DrawTreeStructure(pDrawArgs, pMetrics, rcItem);

		int nIndent = pPaintManager->DrawCollapsedBitmap(bHasChildren? pDC: NULL, pDrawArgs->pRow, rcItem);
		rcItem.left = nIndent + 1;
	}

	pDC->SetTextColor(pMetrics->clrForeground);

	CFont* pOldFont = (CFont*)pDC->SelectObject(pMetrics->pFont);

	if (m_bHasCheckbox)
		DrawCheckBox(pDrawArgs, rcItem);

	// Do the draw bitmap pDC, rcItem, m_nIconIndex
	if (m_nIconIndex != XTP_REPORT_NOICON)
		rcItem.left = pPaintManager->DrawBitmap(pDC, pDrawArgs->pControl, rcItem, m_nIconIndex);

	OnDrawCaption(pDrawArgs, pMetrics);

	int nItemTextWidth = pDC->GetTextExtent(pMetrics->strText).cx;

	pDC->SelectObject(pOldFont);

	pMetrics->InternalRelease();

	if (!pDC->IsPrinting())
		pDC->SelectClipRgn(NULL);

	return nItemTextWidth;
}


void CXTPReportRecordItem::OnClick(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs)
{
	const CRect& rcItem = pClickArgs->rcItem;
	CXTPReportControl* pControl = pClickArgs->pControl;

	if (IsEditable() && m_bHasCheckbox && (pClickArgs->pColumn == NULL || pClickArgs->pColumn->IsEditable()))
	{
		CRect rcCheck (rcItem);
		rcCheck.right = rcCheck.left + 14;

		if (rcCheck.PtInRect(pClickArgs->ptClient))
		{
			m_bChecked = !m_bChecked;
			pControl->RedrawControl();

			pControl->SendMessageToParent(pClickArgs->pRow, this, pClickArgs->pColumn, XTP_NM_REPORT_CHECKED, &pClickArgs->ptClient);
		}
	}

	if (IsAllowEdit(pClickArgs) && pControl->IsEditOnClick())
	{
		pControl->EditItem(pClickArgs);

		CPoint pt(pClickArgs->ptClient);
		pControl->ClientToScreen(&pt);

		if (DYNAMIC_DOWNCAST(CEdit, CWnd::FromHandle(WindowFromPoint(pt))))
		{
			DoMouseButtonClick();
		}
	}

	pClickArgs->pControl->SendMessageToParent(pClickArgs->pRow, this, pClickArgs->pColumn, NM_CLICK, &pClickArgs->ptClient);

	// Determine Hyperlink Click
	int nHyperlink = HitTestHyperlink(pClickArgs->ptClient);
	if ( nHyperlink >= 0)
	{
		pClickArgs->pControl->SendMessageToParent(pClickArgs->pRow, this, pClickArgs->pColumn, XTP_NM_REPORT_HYPERLINK, &pClickArgs->ptClient, nHyperlink);
	}
}

int CXTPReportRecordItem::HitTestHyperlink(CPoint ptClick)
{
	int nHyperlinks = GetHyperlinksCount();
	CXTPReportHyperlink* pHyperlink;
	for(int nHyperlink = 0; nHyperlink < nHyperlinks; nHyperlink++)
	{
		pHyperlink = GetHyperlinkAt(nHyperlink);
		ASSERT(pHyperlink);
		if (pHyperlink->m_rcHyperSpot.PtInRect(ptClick))
			return nHyperlink;
	}
	return -1;
}

void CXTPReportRecordItem::OnDblClick(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs)
{
	CXTPReportControl* pControl = pClickArgs->pControl;

	if (IsAllowEdit(pClickArgs) && !pControl->IsEditOnClick() && pControl->GetInplaceEdit()->GetItem() != this)
	{
		pControl->EditItem(pClickArgs);

		CPoint pt(pClickArgs->ptClient);
		pControl->ClientToScreen(&pt);

		if (DYNAMIC_DOWNCAST(CEdit, CWnd::FromHandle(WindowFromPoint(pt))))
		{
			DoMouseButtonClick();
		}
	}

	pControl->SendMessageToParent(pClickArgs->pRow, this, pClickArgs->pColumn, NM_DBLCLK, &pClickArgs->ptClient, -1);
}

void CXTPReportRecordItem::OnMouseMove(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	int nHyperlink = HitTestHyperlink(point);
	if (nHyperlink >= 0)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
	}
}

void CXTPReportRecordItem::SetFont(CFont* pFont)
{
	m_pFontCaption = pFont;
}

void CXTPReportRecordItem::SetBold(BOOL bBold)
{
	m_bBoldText = bBold;
}

void CXTPReportRecordItem::SetTextColor(COLORREF clrText)
{
	m_clrText = clrText;
}

void CXTPReportRecordItem::SetBackgroundColor(COLORREF clrBackground)
{
	m_clrBackground = clrBackground;
}

int CXTPReportRecordItem::Compare(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem)
{
	if (m_nSortPriority != -1 || pItem->m_nSortPriority != -1)
		return m_nSortPriority - pItem->m_nSortPriority;

	return !pItem ? 0 : GetCaption(pColumn).Compare(pItem->GetCaption(pColumn));
}

CFont* CXTPReportRecordItem::GetFont()
{
	return m_pFontCaption;
}

CString CXTPReportRecordItem::GetGroupCaption(CXTPReportColumn* pColumn)
{
	if (!m_strGroupCaption.IsEmpty())
		return m_strGroupCaption;

	int nID = GetGroupCaptionID(pColumn);

	if (nID > 0)
	{
		CString str;
		if (str.LoadString(nID))
		{
			return str;
		}
	}

	return pColumn->GetCaption() + _T(": ") + GetCaption(pColumn);
}

int CXTPReportRecordItem::CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem)
{
	if (m_nGroupPriority != -1)
		return m_nGroupPriority - pItem->m_nGroupPriority;

	if (!m_strGroupCaption.IsEmpty())
		return m_strGroupCaption.Compare(pItem->m_strGroupCaption);

	int nID = GetGroupCaptionID(pColumn);

	if (nID > 0)
		return nID - pItem->GetGroupCaptionID(pColumn);

	return Compare(pColumn, pItem);
}

int CXTPReportRecordItem::GetGroupCaptionID(CXTPReportColumn* /*pColumn*/)
{
	return m_nGroupPriority;
}


void CXTPReportRecordItem::GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics)
{
	if (m_clrBackground != XTP_REPORT_COLOR_DEFAULT)
		pItemMetrics->clrBackground = m_clrBackground;

	if (m_clrText != XTP_REPORT_COLOR_DEFAULT)
		pItemMetrics->clrForeground = m_clrText;

	if (m_pFontCaption != NULL)
		pItemMetrics->pFont = m_pFontCaption;
	else if (m_bBoldText)
		pItemMetrics->pFont = &pDrawArgs->pControl->GetPaintManager()->m_fontBoldText;

}

int CXTPReportRecordItem::GetIndex()
{
	return m_pRecord? m_pRecord->IndexOf(this): -1;
}

BOOL CXTPReportRecordItem::IsEditable()
{
	if (this == NULL)
		return FALSE;

	return m_pRecord->m_bEditable && m_bEditable && IsFocusable();
}


BOOL CXTPReportRecordItem::IsFocusable()
{
	if (this == NULL)
		return FALSE;

	return m_bFocusable;
}

void CXTPReportRecordItem::OnBeginEdit(XTP_REPORTRECORDITEM_ARGS* pItemArgs)
{
	ASSERT(pItemArgs && pItemArgs->pColumn);

	CXTPReportControl* pControl = pItemArgs->pControl;
	CXTPReportInplaceEdit* pEdit = pControl->GetInplaceEdit();

	if (IsEditable())
	{
		CXTPReportRecordItemEditOptions* pEditOptions = GetEditOptions(pItemArgs->pColumn);

		CXTPReportInplaceButtons* pInpaceButtons = &pEditOptions->arrInplaceButtons;
		CRect rcButtons(pItemArgs->rcItem);

		for (int i = 0; i < pInpaceButtons->GetSize(); i++)
		{
			CXTPReportInplaceButton* pButton = pInpaceButtons->GetAt(i);
			pButton->Create(pItemArgs, rcButtons);

			pControl->GetInplaceButtons()->Add(pButton);
		}

		if (pEditOptions->m_bAllowEdit)
		{
			pEdit->Create(pItemArgs);
		}
	}
}

void CXTPReportRecordItem::OnCancelEdit(CXTPReportControl* pControl, BOOL bApply)
{
	CXTPReportInplaceButtons* pInpaceButtons = pControl->GetInplaceButtons();

	for (int i = 0; i < pInpaceButtons->GetSize(); i++)
	{
		CXTPReportInplaceButton* pButton = pInpaceButtons->GetAt(i);
		pButton->DestroyWindow();
		pButton->SetItemArgs(0);
	}
	pInpaceButtons->RemoveAll();

	CXTPReportInplaceList* pInpaceList = pControl->GetInplaceList();
	if (pInpaceList->GetSafeHwnd())
	{
		pInpaceList->DestroyWindow();
	}


	CXTPReportInplaceEdit* pEdit = pControl->GetInplaceEdit();
	pEdit->HideWindow();

	if (bApply && pEdit->GetSafeHwnd() && pEdit->GetItem() == this)
	{
		OnValidateEdit((XTP_REPORTRECORDITEM_ARGS*)pEdit);
	}
	pEdit->SetItemArgs(0);
}

void CXTPReportRecordItem::OnConstraintChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CXTPReportRecordItemConstraint* pConstraint)
{
	OnEditChanged(pItemArgs, pConstraint->m_strConstraint);
}

void CXTPReportRecordItem::OnValidateEdit(XTP_REPORTRECORDITEM_ARGS* pItemArgs)
{
	CXTPReportControl* pControl = pItemArgs->pControl;
	CXTPReportInplaceEdit* pEdit = pControl->GetInplaceEdit();

	if (pEdit->GetSafeHwnd() && pEdit->GetItem() == this && pItemArgs->pColumn)
	{
		BOOL bRedraw = FALSE;

		if (GetEditOptions(pItemArgs->pColumn)->m_bConstraintEdit)
		{
			if (pEdit->m_pSelectedConstraint)
			{
				OnConstraintChanged(pItemArgs, pEdit->m_pSelectedConstraint);
				bRedraw = TRUE;
			}
		}
		else
		{
			CString strValue;
			pEdit->GetWindowText(strValue);

			if (GetCaption(pItemArgs->pColumn) != strValue)
			{
				OnEditChanged(pItemArgs, strValue);
				bRedraw= TRUE;
			}
		}
		if (bRedraw)
		{
			pControl->RedrawControl();
			pControl->SendMessageToParent(pItemArgs->pRow, this, pItemArgs->pColumn, XTP_NM_REPORT_VALUECHANGED, 0);
		}
	}
}

BOOL CXTPReportRecordItem::IsAllowEdit(XTP_REPORTRECORDITEM_ARGS* pItemArgs)
{
	return pItemArgs->pControl->IsAllowEdit() && IsEditable() &&
		pItemArgs->pColumn && pItemArgs->pColumn->IsEditable();
}

BOOL CXTPReportRecordItem::OnChar(XTP_REPORTRECORDITEM_ARGS* pItemArgs, UINT nChar)
{
	CXTPReportControl* pControl = pItemArgs->pControl;

	if ((nChar == VK_SPACE) && IsEditable() && m_bHasCheckbox && (pItemArgs->pColumn == NULL || pItemArgs->pColumn->IsEditable()))
	{
		m_bChecked = !m_bChecked;
		pControl->RedrawControl();

		pControl->SendMessageToParent(pItemArgs->pRow, this, pItemArgs->pColumn, XTP_NM_REPORT_CHECKED, NULL);
		return TRUE;;
	}

	if (IsAllowEdit(pItemArgs))
	{
		pControl->EditItem(pItemArgs);

		CXTPReportInplaceEdit* pEdit = pControl->GetInplaceEdit();

		if (pEdit->GetSafeHwnd() && pEdit->GetItem() == this)
		{
			pEdit->SetFocus();
			pEdit->SetSel(0, -1);
			if (nChar != VK_TAB) pEdit->SendMessage(WM_CHAR, nChar);
		}
		return TRUE;

	}
	return FALSE;
}

CXTPReportRecordItemEditOptions* CXTPReportRecordItem::GetEditOptions(CXTPReportColumn* pColumn)
{
	if (m_pEditOptions)
		return m_pEditOptions;

	if (pColumn)
		return pColumn->GetEditOptions();

	return m_pEditOptions = new CXTPReportRecordItemEditOptions();
}

void CXTPReportRecordItem::OnInplaceButtonDown(CXTPReportInplaceButton* pButton)
{
	CXTPReportControl* pControl = pButton->pControl;

	XTP_NM_REPORTINPLACEBUTTON nm;
	nm.pButton = pButton;
	nm.pItem = this;

	if (pControl->SendNotifyMessage(XTP_NM_REPORT_INPLACEBUTTONDOWN, (NMHDR*)&nm) == TRUE)
		return;

	if (pButton->GetID() == XTP_ID_REPORT_COMBOBUTTON)
	{
		CXTPReportInplaceList* pList = pControl->GetInplaceList();

		XTP_REPORTRECORDITEM_ARGS itemArgs = *pButton;
		ASSERT(itemArgs.pItem == this);

		CXTPWindowRect rcButton(pButton);
		pControl->ScreenToClient(&rcButton);
		itemArgs.rcItem.right = rcButton.right;

		CXTPReportRecordItemEditOptions* pEditOptions = GetEditOptions(itemArgs.pColumn);

		if (pEditOptions->GetConstraints()->GetCount() > 0)
		{
			pList->Create(&itemArgs, pEditOptions->GetConstraints());
		}
	}
}

