// Custom Items.cpp : implementation file
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

#include "StdAfx.h"
#include "CustomItems.h"


////////////////////////////////////////////////////////////////////////////////////////////////

CCustomItemIcon::CCustomItemIcon(CString strCaption, HICON hIcon)
	: CXTPPropertyGridItem(strCaption)
{
	m_hIcon = hIcon? CopyIcon(hIcon): 0;
	m_nFlags = xtpGridItemHasExpandButton;
}

CCustomItemIcon::~CCustomItemIcon(void)
{
	if (m_hIcon)
		DestroyIcon(m_hIcon);
}

BOOL CCustomItemIcon::OnDrawItemValue(CDC& dc, CRect rcValue)
{
	if (m_hIcon)
	{
		COLORREF clr = dc.GetTextColor();
		CRect rcSample(rcValue.left - 2, rcValue.top + 1, rcValue.left + 18, rcValue.bottom - 1);
		DrawIconEx(dc, rcSample.left, rcSample.top, m_hIcon, rcSample.Width(), rcSample.Height(), 0, 0, DI_NORMAL);
		dc.Draw3dRect(rcSample, clr, clr);
	}

	CRect rcText(rcValue);
	rcText.left += 25;

	dc.DrawText( _T("(Icon)"), rcText,  DT_SINGLELINE|DT_VCENTER);

	return TRUE;
}

void CCustomItemIcon::OnInplaceButtonDown()
{
	const TCHAR szFilters[]=
		_T("Icon files (*.ico)|*.ico|All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("ico"), _T("*.ico"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK)
	{
		if (m_hIcon)
			DestroyIcon(m_hIcon);
		m_hIcon = (HICON)LoadImage(NULL, dlg.GetPathName(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE|LR_DEFAULTSIZE );


		OnValueChanged(_T(""));
		((CWnd*)m_pGrid)->Invalidate(FALSE);
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCustomItemSpinInplaceButton, CSpinButtonCtrl)
	ON_NOTIFY_REFLECT(UDN_DELTAPOS, OnDeltapos)
END_MESSAGE_MAP()


void CCustomItemSpinInplaceButton::OnDeltapos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	m_pItem->OnValidateEdit();
	long nValue = m_pItem->GetNumber() + pNMUpDown->iDelta;

	CString str;
	str.Format(_T("%i"), nValue);
	m_pItem->OnValueChanged(str);

	*pResult = 1;
}

CCustomItemSpin::CCustomItemSpin(CString strCaption)
	: CXTPPropertyGridItemNumber(strCaption)
{
	m_wndSpin.m_pItem = this;
}
void CCustomItemSpin::OnDeselect()
{
	CXTPPropertyGridItemNumber::OnDeselect();

	if (m_wndSpin.m_hWnd) m_wndSpin.ShowWindow(SW_HIDE);
}

void CCustomItemSpin::OnSelect()
{
	CXTPPropertyGridItem::OnSelect();

	if (!m_bReadOnly)
	{
		CRect rc = GetItemRect();
		rc.left = rc.right - 15;
		if (!m_wndSpin.m_hWnd)
		{
			m_wndSpin.Create(UDS_ARROWKEYS|WS_CHILD, rc, (CWnd*)m_pGrid, 0);
			m_wndSpin.SetRange(0, 100);
		}
		m_wndSpin.MoveWindow(rc);
		m_wndSpin.ShowWindow(SW_SHOW);
	}
}
CRect CCustomItemSpin::GetValueRect()
{
	CRect rcValue(CXTPPropertyGridItem::GetValueRect());
	rcValue.right -= 17;
	return rcValue;
}


////////////////////////////////////////////////////////////////////////////////////////////////

class CCustomItemChilds::CCustomItemChildsAll : public CXTPPropertyGridItemNumber
{
public:
	CCustomItemChildsAll(CString strCaption) : CXTPPropertyGridItemNumber(strCaption) {}
	virtual void OnValueChanged(CString strValue)
	{
		SetValue(strValue);

		CCustomItemChilds* pParent = ((CCustomItemChilds*)m_pParent);
		CRect& rc = pParent->m_rcValue;
		rc.left = rc.right = rc.top = rc.bottom = GetNumber();
		pParent->OnValueChanged(pParent->RectToString(rc));
	}
};

class CCustomItemChilds::CCustomItemChildsPad : public CXTPPropertyGridItemNumber
{
public:
	CCustomItemChildsPad(CString strCaption, LONG& nPad) : CXTPPropertyGridItemNumber(strCaption), m_nPad(nPad) {}
	virtual void OnValueChanged(CString strValue)
	{
		SetValue(strValue);

		CCustomItemChilds* pParent = ((CCustomItemChilds*)m_pParent);
		m_nPad = GetNumber();
		pParent->m_itemAll->SetNumber(0);
		pParent->OnValueChanged(pParent->RectToString(pParent->m_rcValue));

	}
	LONG& m_nPad;
};

CCustomItemChilds::CCustomItemChilds(CString strCaption, CRect rcValue)
	: CXTPPropertyGridItem(strCaption)
{
	m_rcValue = rcValue;
	m_strValue = RectToString(rcValue);
	m_nFlags = 0;
}

void CCustomItemChilds::OnAddChildItem()
{
	m_itemAll = (CCustomItemChildsAll*)AddChildItem(new CCustomItemChildsAll(_T("All")));
	m_itemLeft = (CCustomItemChildsPad*)AddChildItem(new CCustomItemChildsPad(_T("Left"), m_rcValue.left));
	m_itemTop = (CCustomItemChildsPad*)AddChildItem(new CCustomItemChildsPad(_T("Top"), m_rcValue.top));
	m_itemRight = (CCustomItemChildsPad*)AddChildItem(new CCustomItemChildsPad(_T("Right"), m_rcValue.right));
	m_itemBottom = (CCustomItemChildsPad*)AddChildItem(new CCustomItemChildsPad(_T("Bottom"), m_rcValue.bottom));

	UpdateChilds();
}

void CCustomItemChilds::UpdateChilds()
{
	m_itemLeft->SetNumber(m_rcValue.left);
	m_itemRight->SetNumber(m_rcValue.right);
	m_itemTop->SetNumber(m_rcValue.top);
	m_itemBottom->SetNumber(m_rcValue.bottom);
}

void CCustomItemChilds::SetValue(CString strValue)
{
	CXTPPropertyGridItem::SetValue(strValue);
	UpdateChilds();
}

CString CCustomItemChilds::RectToString(CRect rc)
{
	CString str;
	str.Format(_T("%i; %i; %i; %i"), rc.left, rc.top, rc.right, rc.bottom);
	return str;
}


///////////////////////////////////////////////////////////////////////////////

#ifdef _XTP_INCLUDE_CONTROLS

class CCustomItemColorPopup: public CXTColorPopup
{
	friend class CCustomItemColor;
public:
	CCustomItemColorPopup() : CXTColorPopup(TRUE) {}
private:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnSelEndOK(WPARAM wParam, LPARAM lParam);

	CCustomItemColor* m_pItem;
};

BEGIN_MESSAGE_MAP(CCustomItemColorPopup, CXTColorPopup)
	ON_MESSAGE(CPN_XT_SELENDOK, OnSelEndOK)
END_MESSAGE_MAP()


LRESULT CCustomItemColorPopup::OnSelEndOK(WPARAM wParam, LPARAM /*lParam*/)
{
	m_pItem->OnValueChanged(m_pItem->RGBToString((COLORREF)wParam));
	return 0;
}

#endif

CCustomItemColor::CCustomItemColor(CString strCaption, COLORREF clr)
	: CXTPPropertyGridItemColor(strCaption, clr)
{
	m_nFlags = xtpGridItemHasComboButton|xtpGridItemHasEdit;
	SetColor(clr);
}


void CCustomItemColor::OnInplaceButtonDown()
{
#ifdef _XTP_INCLUDE_CONTROLS
	CCustomItemColorPopup *pColorPopup = new CCustomItemColorPopup();

	CRect rcItem= GetItemRect();
	m_pGrid->ClientToScreen(&rcItem);
	rcItem.left = rcItem.right - 158; // small hack. need to add CPS_XT_LEFTALLIGN

	pColorPopup->Create(rcItem, m_pGrid, CPS_XT_USERCOLORS|CPS_XT_EXTENDED|CPS_XT_MORECOLORS|CPS_XT_SHOW3DSELECTION|CPS_XT_SHOWHEXVALUE, GetColor(), GetColor());
	pColorPopup->SetOwner(m_pGrid);
	pColorPopup->SetFocus();
	pColorPopup->AddListener(pColorPopup->GetSafeHwnd());
	pColorPopup->m_pItem = this;
#else

	CColorDialog dlg( m_clrValue, 0, m_pGrid );
	if ( dlg.DoModal( ) == IDOK )
	{
		OnValueChanged( RGBToString( dlg.GetColor( ) ) );
		m_pGrid->Invalidate( FALSE );
	}

#endif

}


///////////////////////////////////////////////////////////////////////////////

CCustomItemFileBox::CCustomItemFileBox(CString strCaption)
	: CXTPPropertyGridItem(strCaption)
{
	m_nFlags = xtpGridItemHasExpandButton|xtpGridItemHasEdit;
}
void CCustomItemFileBox::OnInplaceButtonDown()
{
	CFileDialog dlg( TRUE, NULL,  GetValue(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, NULL, m_pGrid);
	if ( dlg.DoModal( ) == IDOK )
	{
		OnValueChanged( dlg.GetPathName());
		m_pGrid->Invalidate( FALSE );
	}
};

///////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CInplaceCheckBox, CButton)
	ON_MESSAGE(BM_SETCHECK, OnCheck)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

HBRUSH CInplaceCheckBox::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	class CGridView : public CXTPPropertyGridView
	{
		friend class CInplaceCheckBox;
	};

	CGridView* pGrid = (CGridView*)m_pItem->m_pGrid;

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

LRESULT CInplaceCheckBox::OnCheck(WPARAM wParam, LPARAM lParam)
{
	m_pItem->m_bValue = (wParam == BST_CHECKED);
	m_pItem->OnValueChanged(m_pItem->GetValue());

	return CButton::DefWindowProc(BM_SETCHECK, wParam, lParam);
}

CCustomItemCheckBox::CCustomItemCheckBox(CString strCaption)
	: CXTPPropertyGridItem(strCaption)
{
	m_wndCheckBox.m_pItem = this;
	m_nFlags = 0;
	m_bValue = FALSE;
}

void CCustomItemCheckBox::OnDeselect()
{
	CXTPPropertyGridItem::OnDeselect();

	if (m_wndCheckBox.m_hWnd) m_wndCheckBox.DestroyWindow();
}

void CCustomItemCheckBox::OnSelect()
{
	CXTPPropertyGridItem::OnSelect();

	if (!m_bReadOnly)
	{
		CRect rc = GetValueRect();
		rc.left -= 15;
		rc.right = rc.left + 15;

		if (!m_wndCheckBox.m_hWnd)
		{
			m_wndCheckBox.Create(NULL, WS_CHILD|BS_AUTOCHECKBOX|BS_FLAT, rc, (CWnd*)m_pGrid, 0);

		}
		if (m_wndCheckBox.GetCheck() != m_bValue) m_wndCheckBox.SetCheck(m_bValue);
		m_wndCheckBox.MoveWindow(rc);
		m_wndCheckBox.ShowWindow(SW_SHOW);
	}
}

CRect CCustomItemCheckBox::GetValueRect()
{
	CRect rcValue(CXTPPropertyGridItem::GetValueRect());
	rcValue.left += 17;
	return rcValue;
}

BOOL CCustomItemCheckBox::OnDrawItemValue(CDC& dc, CRect rcValue)
{
	CRect rcText(rcValue);

	if (m_wndCheckBox.GetSafeHwnd() == 0 && m_bValue)
	{
		CRect rcCheck(rcText.left , rcText.top, rcText.left + 13, rcText.bottom -1);
		dc.DrawFrameControl(rcCheck, DFC_MENU, DFCS_MENUCHECK);
	}

	rcText.left += 17;
	dc.DrawText( GetValue(), rcText,  DT_SINGLELINE|DT_VCENTER);
	return TRUE;
}


BOOL CCustomItemCheckBox::GetBool()
{
	return m_bValue;
}
void CCustomItemCheckBox::SetBool(BOOL bValue)
{
	m_bValue = bValue;

	if (m_wndCheckBox.GetSafeHwnd())
		m_wndCheckBox.SetCheck(bValue);

}



IMPLEMENT_DYNAMIC(CInplaceUpperCase, CXTPPropertyGridInplaceEdit)

BEGIN_MESSAGE_MAP(CInplaceUpperCase, CXTPPropertyGridInplaceEdit)
	//{{AFX_MSG_MAP(CXTPPropertyGridInplaceEdit)
	//}}AFX_MSG_MAP
	ON_WM_CHAR()
END_MESSAGE_MAP()

void CInplaceUpperCase::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CString strChar((TCHAR)nChar), strUpper((TCHAR)nChar);
	strUpper.MakeUpper();

	if (strChar != strUpper) ReplaceSel(strUpper, TRUE);
	else CXTPPropertyGridInplaceEdit::OnChar(nChar, nRepCnt, nFlags);
}


////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CInplaceSlider, CSliderCtrl)
	ON_WM_CTLCOLOR_REFLECT()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
END_MESSAGE_MAP()

HBRUSH CInplaceSlider::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	class CGridView : public CXTPPropertyGridView
	{
		friend class CInplaceSlider;
	};

	CGridView* pGrid = (CGridView*)m_pItem->m_pGrid;

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

void CInplaceSlider::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW lpCustDraw = (LPNMCUSTOMDRAW)pNMHDR;
	if(lpCustDraw->dwDrawStage == CDDS_PREPAINT)
	{
		int nValue = GetPos();
		if (nValue != m_nValue)
		{
			m_nValue = nValue;
			m_pItem->SetNumber(nValue);
			m_pItem->OnValueChanged(m_pItem->GetValue());
			m_pItem->GetGrid()->Invalidate(FALSE);
		}
	}

	*pResult = CDRF_DODEFAULT;
}


CCustomItemSlider::CCustomItemSlider(CString strCaption)
	: CXTPPropertyGridItemNumber(strCaption)
{
	m_wndSlider.m_pItem = this;
	m_nFlags = 0;
}

void CCustomItemSlider::OnDeselect()
{
	CXTPPropertyGridItem::OnDeselect();

	if (m_wndSlider.m_hWnd) m_wndSlider.DestroyWindow();
}

void CCustomItemSlider::OnSelect()
{
	CXTPPropertyGridItem::OnSelect();

	CRect rc = GetValueRect();

	if (!m_bReadOnly)
	{

		CWindowDC dc(m_pGrid);
		CXTPFontDC font (&dc, GetGrid()->GetFont());
		m_nWidth = dc.GetTextExtent(_T("XXX")).cx;

		rc.left += m_nWidth + 2;

		if (rc.left >= rc.right)
			return;

		if (!m_wndSlider.m_hWnd)
		{
			m_wndSlider.Create(WS_CHILD|TBS_HORZ, rc, (CWnd*)m_pGrid, 0);

		}
		m_wndSlider.SetPos(GetNumber());
		m_wndSlider.SetRange(0, 100);

		m_wndSlider.MoveWindow(rc);
		m_wndSlider.ShowWindow(SW_SHOW);
	}
}
