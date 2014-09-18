// XTPPopupItem.cpp: implementation of the CXTPPopupItem class.
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
#include "XTPImageManager.h"

#include "XTPPopupItem.h"
#include "XTPPopupControl.h"
#include "XTPPopupPaintManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPPopupItem

IMPLEMENT_DYNCREATE(CXTPPopupItem, CCmdTarget)


//////////////////////////////////////////////////////////////////////////
// CXTPPopupItem

CXTPPopupItem::CXTPPopupItem(LPRECT rcItem, LPCTSTR lpszCaption, COLORREF clrBackground, COLORREF clrBorder)
{
	m_rcItem = rcItem;
	m_pControl = NULL;
	m_nID = 0;
	m_bButton = FALSE;
	m_nIndex = 0;
	m_ptOffset = CPoint(0, 0);

	m_strCaption = lpszCaption;
	m_clrText = (COLORREF)-1;
	m_bHyperLink = TRUE;
	m_nAlign = DT_LEFT;
	m_bBold = FALSE;
	m_clrBackground = clrBackground;
	m_clrBorder = clrBorder;

	m_bButton = FALSE;

	m_nIconIndex = -1;

	EnableAutomation();


}

CXTPPopupItem::~CXTPPopupItem()
{
}
void CXTPPopupItem::RedrawControl()
{
	ASSERT(m_pControl);
	if(m_pControl)
		m_pControl->RedrawControl();
}


BOOL CXTPPopupItem::IsSelected()
{
	ASSERT(m_pControl);
	return m_pControl->m_pSelected == this;
}

BOOL CXTPPopupItem::IsPressed()
{
	ASSERT(m_pControl);
	return m_pControl->m_pPressed == this;
}

void CXTPPopupItem::OnItemInserted()
{
}


void CXTPPopupItem::Draw(CDC* pDC)
{
	ASSERT(m_pControl);
	if(m_pControl)
		m_pControl->GetPaintManager()->DrawItem(pDC, this);
}

CFont* CXTPPopupItem::GetTextFont()
{
	ASSERT(m_pControl);
	return (m_fntText.m_hObject == NULL)?
		&m_pControl->GetPaintManager()->m_fntText : &m_fntText;
}

void CXTPPopupItem::SetTextFont(CFont * pFntText)
{
	ASSERT(pFntText);

	LOGFONT lf;
	pFntText->GetLogFont(&lf);

	SetTextFont(&lf);
}

void CXTPPopupItem::SetTextFont(PLOGFONT pLogfText)
{
	ASSERT(pLogfText);

	m_fntText.DeleteObject();
	m_fntText.CreateFontIndirect(pLogfText);

	RedrawControl();
}

void CXTPPopupItem::CalculateHeight()
{
	CWindowDC dc(NULL);
	CRect rcText(0, 0, m_rcItem.Width(), 0);

	CXTPFontDC font(&dc, GetTextFont());
	dc.DrawText(m_strCaption, rcText, m_nAlign | DT_CALCRECT);

	m_rcItem.bottom = m_rcItem.top + rcText.Height();
}

void CXTPPopupItem::CalculateWidth()
{
	if(m_nAlign & DT_WORDBREAK) return;

	CWindowDC dc(NULL);
	CRect rcText(0, 0, 0, m_rcItem.Height());

	CFont fntUnderline;
	CFont* pFont = GetTextFont();

	if (IsHyperLink() || IsBold())
	{
		LOGFONT lpLogFont;
		pFont->GetLogFont(&lpLogFont);
		lpLogFont.lfUnderline = IsHyperLink()? true : false;
		lpLogFont.lfWeight = IsBold()? FW_BOLD : FW_NORMAL;

		VERIFY(fntUnderline.CreateFontIndirect(&lpLogFont));
		pFont = &fntUnderline;
	}

	CXTPFontDC font(&dc, pFont);

	dc.DrawText(m_strCaption, rcText, m_nAlign | DT_CALCRECT);

	if(m_nAlign & DT_RIGHT)
	{
		m_rcItem.left = m_rcItem.right - rcText.Width();
	}
	else if(m_nAlign & DT_CENTER)
	{
		LONG lCentrl = m_rcItem.left + m_rcItem.Width()/2;
		m_rcItem.left = lCentrl - rcText.Width()/2;
		m_rcItem.right = m_rcItem.left + rcText.Width();
	}
	else
	{
		m_rcItem.right = m_rcItem.left + rcText.Width();
	}
}




void CXTPPopupItem::SetIcon(HICON hIcon, XTPPopupItemIcon itemIcon)
{
	ASSERT(hIcon);

	m_pControl->GetImageManager()->SetIcon(hIcon, GetIconIndex(), 0,
		itemIcon == xtpPopupItemIconNormal? xtpImageNormal: itemIcon == xtpPopupItemIconSelected? xtpImageHot: xtpImageChecked);
}

BOOL CXTPPopupItem::SetIcons(UINT nIDBitmap, COLORREF clrTransparent, int itemIcon)
{
	CBitmap bmp;
	if (!bmp.LoadBitmap(nIDBitmap))
		return FALSE;

	return SetIcons((HBITMAP)bmp.GetSafeHandle(), clrTransparent, itemIcon);
}


BOOL CXTPPopupItem::SetIcons(HBITMAP hBitmap, COLORREF clrTransparent, int itemIcon)
{

	BITMAP bmpInfo;
	::GetObject(hBitmap, sizeof(BITMAP), &bmpInfo);

	int nCount = (itemIcon & xtpPopupItemIconNormal? 1: 0) +
		(itemIcon & xtpPopupItemIconSelected? 1: 0) +
		(itemIcon & xtpPopupItemIconPressed? 1: 0);

	CImageList il;
	il.Create(bmpInfo.bmWidth / nCount, bmpInfo.bmHeight, ILC_COLOR24|ILC_MASK, 0, 1);
	il.Add(CBitmap::FromHandle(hBitmap), clrTransparent);

	int nIndex = 0;

	if ((itemIcon & xtpPopupItemIconNormal) != 0)
	{
		HICON hIcon = il.ExtractIcon(nIndex++);
		SetIcon(hIcon, xtpPopupItemIconNormal);
		DestroyIcon(hIcon);
	}
	if ((itemIcon & xtpPopupItemIconSelected) != 0)
	{
		HICON hIcon = il.ExtractIcon(nIndex++);
		SetIcon(hIcon, xtpPopupItemIconSelected);
		DestroyIcon(hIcon);
	}

	if ((itemIcon & xtpPopupItemIconPressed) != 0)
	{
		HICON hIcon = il.ExtractIcon(nIndex++);
		SetIcon(hIcon, xtpPopupItemIconPressed);
		DestroyIcon(hIcon);
	}

	return TRUE;
}

CXTPImageManagerIcon* CXTPPopupItem::GetImage()
{
	return m_pControl->GetImageManager()->GetImage(GetIconIndex(), 0);
}

void CXTPPopupItem::CenterIcon()
{
	m_ptOffset = CPoint(0,0);

	CXTPImageManagerIcon* pIcon = GetImage();

	if(!pIcon)
		return;

	CSize sz = pIcon->GetExtent();
	CRect rc = GetRect();

	if(rc.Width() > sz.cx) m_ptOffset.x = rc.Width()/2 - sz.cx/2;
	if(rc.Height() > sz.cy) m_ptOffset.y = rc.Height()/2 - sz.cy/2;

}



