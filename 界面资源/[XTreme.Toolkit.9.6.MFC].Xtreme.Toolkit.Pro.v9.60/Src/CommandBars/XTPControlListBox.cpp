// XTPControlListBox.cpp : implementation of the CXTPControlListBox class.
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
#include "XTPPropExchange.h"

#include "XTPControl.h"
#include "XTPCommandBar.h"
#include "XTPPaintManager.h"
#include "XTPMouseManager.h"
#include "XTPControlListBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

class CXTPControlListBox::CListBoxExt : public CListBox
{
	DECLARE_MESSAGE_MAP()
		friend class CXTPControlListBox;
public:
	CXTPControlListBox* m_pControl;

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	void OnSelChanged();
};



IMPLEMENT_XTP_CONTROL(CXTPControlListBox, CXTPControl)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPControlListBox::CXTPControlListBox()
{
	m_pListBox = new CListBoxExt;
	m_pListBox->m_pControl = this;

	m_pListBox->CreateEx(0, _T("LISTBOX"), NULL, WS_POPUP|WS_VSCROLL|WS_CLIPCHILDREN|LBS_NOTIFY, CRect(0,0,0,0),  0,  0);

	m_nListBoxLinesMin = 1;
	m_nListBoxLinesMax = 12;
	m_nWidth = 100;
	m_bMultiplSel = FALSE;

	m_bMouseLocked = FALSE;
}

CXTPControlListBox::~CXTPControlListBox()
{
	if (m_pListBox)
	{
		delete m_pListBox;
	}

}
CListBox* CXTPControlListBox::GetListCtrl()
{
	ASSERT_VALID(this);
	return m_pListBox;
}

void CXTPControlListBox::SetWidth(int nWidth)
{
	m_nWidth = nWidth;
}

void CXTPControlListBox::SetMultiplSel(BOOL bMultiplSel)
{
	m_bMultiplSel = bMultiplSel;

	m_pListBox->DestroyWindow();
	m_pListBox->CreateEx(0, _T("LISTBOX"), NULL, WS_POPUP|WS_VSCROLL|WS_CLIPCHILDREN|LBS_NOTIFY|(m_bMultiplSel? LBS_MULTIPLESEL: 0), CRect(0,0,0,0),  0,  0);
}


void CXTPControlListBox::SetLinesMinMax(int nMin, int nMax)
{
	m_nListBoxLinesMin = nMin;
	m_nListBoxLinesMax = nMax;
}

void CXTPControlListBox::SetRect(CRect rcControl)
{
	ASSERT_VALID(this);

	if (m_rcControl == rcControl && m_pListBox->GetParent() == m_pParent)
	{
		return;
	}

	m_rcControl = rcControl;

	m_pListBox->EnableWindow(m_bEnabled);

	m_pListBox->ModifyStyle(WS_POPUP, WS_CHILD);
	m_pListBox->SetParent(m_pParent);
	m_pListBox->MoveWindow(rcControl);
	m_pListBox->SetWindowPos(0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | ((m_dwHideFlags != xtpNoHide)? SWP_HIDEWINDOW: SWP_SHOWWINDOW));
}

void CXTPControlListBox::SetParent(CXTPCommandBar* pParent)
{
	if (pParent != m_pParent && (pParent && pParent->GetSafeHwnd()))
	{
		m_pListBox->ModifyStyle(WS_POPUP, WS_CHILD);
		m_pListBox->SetParent(pParent);
	}
	CXTPControl::SetParent(pParent);
}

void CXTPControlListBox::SetEnabled(BOOL bEnabled)
{
	ASSERT_VALID(this);

	if (bEnabled != m_bEnabled)
	{
		m_bEnabled = bEnabled;
		m_pListBox->EnableWindow(bEnabled);
	}
}

CSize CXTPControlListBox::GetSize(CDC* /*pDC*/)
{
	ASSERT_VALID(this);

	m_pListBox->SetFont(GetPaintManager()->GetIconFont(), FALSE);

	int nItemHeight = m_pListBox->GetItemHeight(0);
	int nHeight = min (m_nListBoxLinesMax, max(m_nListBoxLinesMin, m_pListBox->GetCount())) * nItemHeight;
	int nWidth = m_nWidth;

	return CSize(nWidth, nHeight);
}

void CXTPControlListBox::Draw(CDC* /*pDC*/)
{

}

BOOL  CXTPControlListBox::OnSetSelected(int bSelected)
{
	if (!CXTPControl::OnSetSelected(bSelected))
		return FALSE;

	m_pParent->SetFocusedControl(bSelected? this: NULL);

	if (bSelected && !m_bMouseLocked)
	{
		XTPMouseManager()->LockMouseMove();
		m_bMouseLocked = TRUE;
	}
	else if (!bSelected && m_bMouseLocked)
	{
		XTPMouseManager()->UnlockMouseMove();
		m_bMouseLocked = FALSE;

	}

	return TRUE;
}

BOOL CXTPControlListBox::OnKeyDown(UINT nChar)
{
	ASSERT_VALID(this);

	int nSel = m_pListBox->GetCurSel();

	m_pListBox->SendMessage(WM_KEYDOWN, nChar, 0);

	if (nSel != m_pListBox->GetCurSel())
	{
		m_pListBox->OnSelChanged();
	}


	return TRUE;
}

void CXTPControlListBox::OnClick(BOOL bKeyboard , CPoint pt)
{
	if (bKeyboard)
	{
		OnExecute();
	}
	else CXTPControl::OnClick(bKeyboard, pt);
}

void CXTPControlListBox::OnCalcDynamicSize(DWORD dwMode)
{
	CXTPControl::OnCalcDynamicSize(dwMode);

	if (m_bMultiplSel)
	{
		m_pListBox->SelItemRange(FALSE, 0, m_pListBox->GetCount() - 1);
	} else
	{
		m_pListBox->SetCurSel(-1);
	}

	NMXTPCONTROL fnm;
	fnm.hdr.code = XTP_LBN_POPUP;
	fnm.hdr.idFrom = GetID();
	fnm.hdr.hwndFrom = 0;
	fnm.pControl = this;

	GetParent()->GetSite()->SendMessage(WM_XTP_COMMAND, GetID(), (LPARAM)&fnm);

}


void CXTPControlListBox::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	ASSERT_KINDOF(CXTPControlListBox, pControl);

	CXTPControl::Copy(pControl, bRecursive);

	m_nWidth = ((CXTPControlListBox*)pControl)->m_nWidth;
	SetMultiplSel(((CXTPControlListBox*)pControl)->m_bMultiplSel);
	m_nListBoxLinesMax = ((CXTPControlListBox*)pControl)->m_nListBoxLinesMax;
	m_nListBoxLinesMin = ((CXTPControlListBox*)pControl)->m_nListBoxLinesMin;


	CListBox* pListBox = ((CXTPControlListBox*)pControl)->GetListCtrl();
	DWORD dwCount = pListBox->m_hWnd? pListBox->GetCount(): 0;
	m_pListBox->ResetContent();

	for (UINT i = 0; i < dwCount; i++)
	{
		CString str;
		pListBox->GetText(i, str);
		m_pListBox->AddString(str);
	}

}

void CXTPControlListBox::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPControl::DoPropExchange(pPX);
	CString str;

	PX_Int(pPX, _T("Width"), m_nWidth, 100);
	PX_Bool(pPX, _T("MultiplSel"), m_bMultiplSel, FALSE);
	PX_Int(pPX, _T("ListBoxLinesMax"), m_nListBoxLinesMax, 12);
	PX_Int(pPX, _T("ListBoxLinesMin"), m_nListBoxLinesMin, 1);

	CXTPPropExchangeSection secItems(pPX->GetSection(_T("Items")));

	if (pPX->IsStoring())
	{
		DWORD dwCount = m_pListBox->m_hWnd? m_pListBox->GetCount(): 0;
		secItems->WriteCount(dwCount);
		for (UINT i = 0; i < dwCount; i++)
		{
			m_pListBox->GetText(i, str);

			CString strSection;
			strSection.Format(_T("Item%i"), i);
			PX_String(&secItems, strSection, str, _T(""));
		}

	}
	else
	{
		SetMultiplSel(m_bMultiplSel);

		DWORD nNewCount = secItems->ReadCount();
		for (DWORD i = 0; i < nNewCount; i++)
		{
			CString strSection;
			strSection.Format(_T("Item%i"), i);

			PX_String(&secItems, strSection, str, _T(""));
			m_pListBox->AddString(str);
		}

	}

}



// mini hack.
BEGIN_MESSAGE_MAP(CXTPControlListBox::CListBoxExt, CListBox)
	{ WM_LBUTTONDOWN, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(AFX_PMSGW) (static_cast< void (AFX_MSG_CALL CWnd::*)(UINT, CPoint) > (CXTPControlListBox::CListBoxExt::OnLButtonDown)) },
	{ WM_LBUTTONUP, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(AFX_PMSGW) (static_cast< void (AFX_MSG_CALL CWnd::*)(UINT, CPoint) > (CXTPControlListBox::CListBoxExt::OnLButtonUp)) },
	{ WM_LBUTTONDBLCLK, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(AFX_PMSGW) (static_cast< void (AFX_MSG_CALL CWnd::*)(UINT, CPoint) > (CXTPControlListBox::CListBoxExt::OnLButtonDblClk)) },
	{ WM_MOUSEMOVE, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(AFX_PMSGW) (static_cast< void (AFX_MSG_CALL CWnd::*)(UINT, CPoint) > (CXTPControlListBox::CListBoxExt::OnMouseMove)) },
	{ WM_ERASEBKGND, 0, 0, 0, AfxSig_bD, (AFX_PMSG)(AFX_PMSGW) (static_cast< BOOL (AFX_MSG_CALL CWnd::*)(CDC*) > (CXTPControlListBox::CListBoxExt::OnEraseBkgnd)) },
	{ WM_PAINT, 0, 0, 0, AfxSig_vv, (AFX_PMSG)(AFX_PMSGW) (static_cast< void (AFX_MSG_CALL CWnd::*)(void) > (CXTPControlListBox::CListBoxExt::OnPaint)) },

END_MESSAGE_MAP()


void CXTPControlListBox::CListBoxExt::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/)
{
}
void CXTPControlListBox::CListBoxExt::OnLButtonUp(UINT /*nFlags*/, CPoint /*point*/)
{
	m_pControl->OnExecute();
}

void CXTPControlListBox::CListBoxExt::OnLButtonDblClk(UINT /*nFlags*/, CPoint /*point*/)
{
}


void CXTPControlListBox::CListBoxExt::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPBufferDC dc(dcPaint, CXTPClientRect(this));

	CWnd::DefWindowProc(WM_PAINT, (WPARAM)dc.m_hDC, 0);
}

BOOL CXTPControlListBox::CListBoxExt::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}


void CXTPControlListBox::CListBoxExt::OnMouseMove(UINT nFlags, CPoint point)
{

	BOOL bOutside;
	UINT nItem = ItemFromPoint(point,  bOutside);

	if (!bOutside)
	{
		SetCurSel(nItem);
		SetSel(nItem, TRUE);

		OnSelChanged();
	}

	MapWindowPoints(m_pControl->GetParent(), &point, 1);
	m_pControl->GetParent()->OnMouseMove(nFlags, point);

}

void CXTPControlListBox::CListBoxExt::OnSelChanged()
{
	if (m_pControl->m_bMultiplSel)
	{
		int nSel = GetCurSel();

		if (nSel != LB_ERR)
		{
			SelItemRange(FALSE, nSel + 1, GetCount() - 1);
			if (nSel != 0) SelItemRange(TRUE, 0, nSel);
			else  SetSel(0, TRUE);

		}
		else
		{
			SelItemRange(FALSE, 0, GetCount() - 1);
		}
	}

	NMXTPCONTROL fnm;
	fnm.hdr.code = XTP_LBN_SELCHANGE;
	fnm.hdr.idFrom = m_pControl->GetID();
	fnm.hdr.hwndFrom = 0;
	fnm.pControl = m_pControl;

	m_pControl->GetParent()->GetSite()->SendMessage(WM_XTP_COMMAND, m_pControl->GetID(), (LPARAM)&fnm);


}




///////////////////////////////////////////////////////////////////
// CXTPControlListBoxInfo

IMPLEMENT_XTP_CONTROL(CXTPControlStatic, CXTPControl)

CXTPControlStatic::CXTPControlStatic()
{
	m_nWidth = 0;

}
void CXTPControlStatic::SetWidth(int nWidth)
{
	m_nWidth = nWidth;

}


CSize CXTPControlStatic::GetSize(CDC* pDC)
{
	BOOL bVert = GetParent()->GetPosition() == xtpBarRight || GetParent()->GetPosition() == xtpBarLeft;

	CXTPEmptyRect rcText;

	CSize sz = GetPaintManager()->DrawTextEx(pDC, GetCaption(), &rcText, FALSE, bVert, FALSE, FALSE);

	if (bVert) return CSize(max(22, sz.cx), max(m_nWidth, sz.cy));

	return CSize(max(m_nWidth, sz.cx), max(22, sz.cy));

}
void CXTPControlStatic::Draw(CDC* pDC)
{
	BOOL bVert = GetParent()->GetPosition() == xtpBarRight || GetParent()->GetPosition() == xtpBarLeft;

	CRect rcText = GetRect();
	GetPaintManager()->DrawControlEntry(pDC, rcText, GetEnabled(), FALSE, FALSE, FALSE, FALSE, GetParent()->GetPosition());
	GetPaintManager()->DrawTextEx(pDC, GetCaption(), &rcText, TRUE, bVert, TRUE, FALSE, !GetEnabled());
}


void CXTPControlStatic::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	ASSERT_KINDOF(CXTPControlStatic, pControl);

	CXTPControl::Copy(pControl, bRecursive);

	m_nWidth = ((CXTPControlStatic*)pControl)->m_nWidth;

}
void CXTPControlStatic::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPControl::DoPropExchange(pPX);

	PX_Int(pPX, _T("Width"), m_nWidth, 0);
}
