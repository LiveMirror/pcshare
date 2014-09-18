// XTPReportInplaceEdit.cpp : implementation of the CXTPReportInplaceEdit class.
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

#include "XTPReportControl.h"
#include "XTPReportDefines.h"
#include "XTPReportInplaceControls.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CXTPReportInplaceControl::CXTPReportInplaceControl()
{
}

CXTPReportInplaceControl::~CXTPReportInplaceControl()
{
	SetItemArgs(0);
}

void CXTPReportInplaceControl::SetItemArgs(XTP_REPORTRECORDITEM_ARGS* pItemArgs)
{
	if (pRow)
	{
		pRow->InternalRelease();
		pRow = 0;
	}

	if (pItemArgs)
	{
		pItem = pItemArgs->pItem;
		pControl = pItemArgs->pControl;
		pRow = pItemArgs->pRow;
		pColumn = pItemArgs->pColumn;

		pRow->InternalAddRef();
		rcItem = pItemArgs->rcItem;
	}
	else
	{
		pItem = 0;
		pControl = 0;
		pRow = 0;
		pColumn = 0;
	}
}


CXTPReportInplaceEdit::CXTPReportInplaceEdit()
{
	m_pSelectedConstraint = NULL;
	m_clrText = 0;
}

CXTPReportInplaceEdit::~CXTPReportInplaceEdit()
{
}


BEGIN_MESSAGE_MAP(CXTPReportInplaceEdit, CEdit)
	//{{AFX_MSG_MAP(CXTPReportInplaceEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnEnKillfocus)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CXTPReportInplaceEdit::HideWindow()
{
	if (m_hWnd)
	{
		ShowWindow(SW_HIDE);
		SetItemArgs(0);
	}
}


void CXTPReportInplaceEdit::Create(XTP_REPORTRECORDITEM_ARGS* pItemArgs)
{
	SetItemArgs(pItemArgs);
	m_pSelectedConstraint = NULL;

	XTP_REPORTRECORDITEM_METRICS metrics;
	metrics.strText = pItem->GetCaption(pColumn);
	pItemArgs->pRow->FillMetrics(pColumn, pItem, &metrics);

	CRect rect = pItemArgs->rcItem;
	pItem->GetCaptionRect(pItemArgs, rect);
	rect.DeflateRect(2, 1, 1, 2);

	m_clrText = metrics.clrForeground;
	m_strValue = metrics.strText;

	if (!m_hWnd)
	{
		CEdit::Create(WS_CHILD|ES_AUTOHSCROLL, rect, pControl, 0);
	}

	SetFocus();
	SetFont(metrics.pFont);
	SetWindowText(m_strValue);

	if (rect.right > rect.left)
	{
		SetWindowPos(0, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER|SWP_SHOWWINDOW);
	} else
	{
		HideWindow();
	}

	SetMargins(0, 0);
}

void CXTPReportInplaceEdit::SetFont(CFont* pFont)
{
	m_fntEdit.DeleteObject();
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	m_fntEdit.CreateFontIndirect(&lf);

	CEdit::SetFont(&m_fntEdit);
}

void CXTPReportInplaceEdit::OnEnKillfocus()
{
	if (pControl && pItem)
	{
		pItem->OnValidateEdit((XTP_REPORTRECORDITEM_ARGS*)this);
		HideWindow();

	}
}

UINT CXTPReportInplaceEdit::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}

void CXTPReportInplaceEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (!pControl)
		return;

	if (nChar == VK_TAB) return;
	if (nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		pControl->EditItem(NULL);
		return;
	}
	if (pItem && pColumn && pItem->GetEditOptions(pColumn)->m_bConstraintEdit)
	{
		CXTPReportRecordItemEditOptions* pEditOptions = pItem->GetEditOptions(pColumn);

		CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
		int nCount = pConstraints->GetCount();
		if (nCount > 0)
		{

			CString str;
			GetWindowText(str);
			CXTPReportRecordItemConstraint* pConstraint = (m_pSelectedConstraint == NULL)?
				pEditOptions->FindConstraint(str): m_pSelectedConstraint;

			int nIndexStart, nIndex;
			nIndexStart = nIndex = (pConstraint == NULL? nCount - 1: pConstraint->GetIndex());

			CString strSeach ((TCHAR)nChar);

			do
			{

				nIndex = nIndex < nCount - 1? nIndex + 1: 0;
				CXTPReportRecordItemConstraint* pConstraint = pConstraints->GetAt(nIndex);

				CString str = pConstraint->m_strConstraint;

				if (strSeach.CompareNoCase(str.Left(1)) == 0)
				{
					m_pSelectedConstraint = pConstraint;
					SetWindowText(str);
					SetSel(0, -1);
					return;
				}

			} while (nIndex != nIndexStart);

			return;
		}
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CXTPReportInplaceEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CXTPReportControl* _pControl = pControl;

	if (nChar == VK_TAB && _pControl)
	{		
		_pControl->SetFocus();
		_pControl->SendMessage(WM_CHAR, nChar);
		return;

	}
	if (nChar == VK_ESCAPE)
	{
		SetWindowText(m_strValue);
		return ;
	}
	else if (nChar == VK_RETURN)
	{
		return ;
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CXTPReportInplaceEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && IsDialogMessage(pMsg))
		return TRUE;

	return CEdit::PreTranslateMessage(pMsg);
}

void CXTPReportInplaceEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (pRow && pItem)
	{
		MapWindowPoints(pControl, &point, 1);
		pRow->OnDblClick(point);
	}

	if (pItem)
	{
		CEdit::OnLButtonDblClk(nFlags, point);
	}
}

HBRUSH CXTPReportInplaceEdit::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	pDC->SetTextColor(m_clrText);

	return GetSysColorBrush(COLOR_WINDOW);
}

//////////////////////////////////////////////////////////////////////////
// CXTPReportInplaceButton

CXTPReportInplaceButton::CXTPReportInplaceButton(UINT nID)
{
	m_nID = nID;
	m_nWidth = 17;

	m_bPressed = m_bOver = FALSE;
}

void CXTPReportInplaceButton::Create(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CRect& rcButtons)
{
	m_bPressed = m_bOver = FALSE;
	SetItemArgs(pItemArgs);

	CRect rect(rcButtons.right - 1, rcButtons.top, rcButtons.right + m_nWidth - 1, rcButtons.bottom - 1);

	CXTPClientRect rcControl(pItemArgs->pControl);
	int nOffset = rcControl.right > rect.right? 0: rect.right - rcControl.right;

	rect.OffsetRect(-nOffset, 0);


	if (!m_hWnd)
	{
		CStatic::Create(NULL, SS_NOTIFY|WS_CHILD, rect, pItemArgs->pControl);
	}

	SetWindowPos(0, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER|SWP_SHOWWINDOW);

	pItemArgs->rcItem.right -= nOffset;
	rcButtons.right += m_nWidth;
}

BEGIN_MESSAGE_MAP(CXTPReportInplaceButton, CStatic)
	//{{AFX_MSG_MAP(CXTPReportInplaceButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CAPTURECHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTPReportInplaceButton::OnPaint()
{
	CPaintDC dc(this);

	if (pControl)
	{
		pControl->GetPaintManager()->DrawInplaceButton(&dc, this);
	}
}


void CXTPReportInplaceButton::OnLButtonDown(UINT, CPoint)
{
	m_bOver = m_bPressed = TRUE;
	Invalidate(FALSE);
	SetCapture();
}

void CXTPReportInplaceButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bPressed && pItem)
	{
		m_bPressed = FALSE;
		Invalidate(FALSE);
		ReleaseCapture();
		if (m_bOver)
		{
			pItem->OnInplaceButtonDown(this);
		}
	}
	CStatic::OnLButtonUp(nFlags, point);
}

void CXTPReportInplaceButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bPressed)
	{
		CXTPClientRect rect(this);
		if ((rect.PtInRect(point) && !m_bOver) ||
			(!rect.PtInRect(point) && m_bOver))
		{
			m_bOver = rect.PtInRect(point);
			Invalidate(FALSE);
		}
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CXTPReportInplaceButton::OnCaptureChanged(CWnd *pWnd)
{
	m_bPressed = FALSE;
	Invalidate(FALSE);

	CStatic::OnCaptureChanged(pWnd);
}


//////////////////////////////////////////////////////////////////////////
// CXTPReportInplaceList

CXTPReportInplaceList::CXTPReportInplaceList()
{
}


void CXTPReportInplaceList::Create(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CXTPReportRecordItemConstraints* pConstaints)
{
	SetItemArgs(pItemArgs);


	CRect rect(pItemArgs->rcItem);

	if (!m_hWnd)
	{
		CListBox::CreateEx(WS_EX_TOOLWINDOW | (pControl->GetExStyle() & WS_EX_LAYOUTRTL), _T("LISTBOX"), _T(""), LBS_NOTIFY |WS_CHILD|WS_BORDER|WS_VSCROLL, CRect(0, 0, 0, 0), pControl, 0);
		SetOwner(pControl);
	}

	SetFont(pControl->GetPaintManager()->GetTextFont());
	ResetContent();

	int dx = rect.right - rect.left + 1;

	CWindowDC dc(pControl);
	CXTPFontDC font(&dc, GetFont());
	int nThumbLength = GetSystemMetrics(SM_CXHTHUMB);

	CString strCaption = pItem->GetCaption(pColumn);
	DWORD dwData = pItem->GetSelectedConstraintData(pItemArgs);

	for (int i = 0; i < pConstaints->GetCount(); i++)
	{
		CXTPReportRecordItemConstraint* pConstaint = pConstaints->GetAt(i);
		CString str = pConstaint->m_strConstraint;
		int nIndex = AddString(str);
		SetItemDataPtr(nIndex, pConstaint);

		dx = max(dx, dc.GetTextExtent(str).cx + nThumbLength);

		if ((dwData == (DWORD)-1 && strCaption == str) || (dwData == pConstaint->m_dwData))
			SetCurSel(nIndex);
	}

	int nHeight = GetItemHeight(0);
	rect.top = rect.bottom;
	rect.bottom += nHeight * min(10, GetCount()) + 2;
	rect.left = rect.right - dx;

	pControl->ClientToScreen(&rect);

	CRect rcWork = CXTPDrawHelpers::GetWorkArea(pControl);
	if (rect.bottom > rcWork.bottom && rect.top > rcWork.CenterPoint().y)
	{
		rect.OffsetRect(0, - rect.Height() - pItemArgs->rcItem.Height());

	}

	SetFocus();

	SetWindowLong(m_hWnd, GWL_HWNDPARENT, 0 );
	ModifyStyle(WS_CHILD, WS_POPUP);
	SetWindowLongPtr(m_hWnd, GWL_HWNDPARENT, (LONG_PTR)pControl->m_hWnd );

	SetWindowPos(&CWnd::wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER|SWP_SHOWWINDOW|SWP_NOACTIVATE);

	CXTPMouseMonitor::SetupHook(this);

}

BEGIN_MESSAGE_MAP(CXTPReportInplaceList, CListBox)
	//{{AFX_MSG_MAP(CXTPReportInplaceList)
	ON_WM_MOUSEACTIVATE()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CXTPReportInplaceList::OnMouseActivate(CWnd* /*pDesktopWnd*/, UINT /*nHitTest*/, UINT /*message*/)
{
	return MA_NOACTIVATE;
}

void CXTPReportInplaceList::PostNcDestroy()
{
	CXTPMouseMonitor::SetupHook(NULL);
	SetItemArgs(NULL);

	CListBox::PostNcDestroy();
}


void CXTPReportInplaceList::OnKillFocus(CWnd* pNewWnd)
{
	CListBox::OnKillFocus(pNewWnd);
	DestroyWindow();
}

void CXTPReportInplaceList::OnLButtonUp(UINT, CPoint point)
{
	CXTPClientRect rc(this);

	if (rc.PtInRect(point)) Apply(); else Cancel();
}


void CXTPReportInplaceList::Cancel(void)
{
	GetOwner()->SetFocus();
}

void CXTPReportInplaceList::Apply(void)
{
	if (!pControl)
		return;

	int nIndex = GetCurSel();
	if (nIndex != LB_ERR)
	{
		CXTPReportRecordItemConstraint* pConstraint = (CXTPReportRecordItemConstraint*)GetItemDataPtr(nIndex);

		pItem->OnConstraintChanged(this, pConstraint);
		pControl->RedrawControl();

		pControl->SendMessageToParent(pRow, pItem, pColumn, XTP_NM_REPORT_VALUECHANGED, 0);

	}
	pControl->SetFocus();
}

void CXTPReportInplaceList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE) Cancel();
	else if (nChar == VK_RETURN || nChar == VK_F4) Apply();
	else CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPReportInplaceList::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_DOWN || nChar == VK_UP)
	{
		Apply();
		return;
	}

	CListBox::OnSysKeyDown(nChar, nRepCnt, nFlags);
}
