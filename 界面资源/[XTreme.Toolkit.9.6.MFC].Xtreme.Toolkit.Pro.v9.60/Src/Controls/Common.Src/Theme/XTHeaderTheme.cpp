// XTHeaderTheme.cpp: implementation of the CXTHeaderTheme class.
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
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTResource.h"
#include "XTVC50Helpers.h"
#include "XTPWinThemeWrapper.h"
#include "XTFlatHeaderCtrl.h"
#include "XTPColorManager.h"
#include "XTPDrawHelpers.h"
#include "XTPManageState.h"
#include "XTThemeManager.h"
#include "XTHeaderTheme.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_THEME_FACTORY(CXTHeaderTheme, CXTHeaderThemeOfficeXP, CXTHeaderThemeOffice2003)


//=============================================================================
// CXTHeaderTheme
//=============================================================================

CXTHeaderTheme::CXTHeaderTheme()
: m_sizePadding(6,0)
, m_sizeArrow(8,7)
, m_dwDrawStyle(0)
, m_iArrowPadding(10)
, m_bUseWinThemes(false)
{
	m_dwDrawStyle = XTTHEME_WINXPTHEMES|XTTHEME_HOTTRACKING|XTTHEME_SORTARROW;
	RefreshMetrics();
}

CXTHeaderTheme::~CXTHeaderTheme()
{

}

void CXTHeaderTheme::RefreshMetrics()
{
	m_crText        = ::GetSysColor(COLOR_BTNTEXT);
	m_cr3DFace      = ::GetSysColor(COLOR_3DFACE);
	m_cr3DShadow    = ::GetSysColor(COLOR_3DSHADOW);
	m_cr3DDkShadow  = ::GetSysColor(COLOR_3DDKSHADOW);
	m_cr3DHighLight = ::GetSysColor(COLOR_3DHIGHLIGHT);
}

void CXTHeaderTheme::OnDrawBackground(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect rcItem = lpDIS->rcItem;

	// fill background
	pDC->FillSolidRect(&rcItem, m_cr3DFace);
	rcItem.right += 2;

	// draw 3D borders.
	pDC->Draw3dRect(&rcItem, m_cr3DHighLight, m_cr3DDkShadow);
	rcItem.DeflateRect(1,1);
	pDC->Draw3dRect(&rcItem, m_cr3DFace, m_cr3DShadow);
}

void CXTHeaderTheme::OnDrawItemBackground(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	if (lpDIS->itemState == ODS_SELECTED)
	{
		pDC->Draw3dRect(&lpDIS->rcItem, m_cr3DShadow, m_cr3DShadow);

		CXTPPenDC penDC(pDC->m_hDC, m_cr3DHighLight);
		pDC->MoveTo(lpDIS->rcItem.right, 1);
		pDC->LineTo(lpDIS->rcItem.right, lpDIS->rcItem.bottom-1);

		penDC.Color(m_cr3DFace);
		pDC->MoveTo(lpDIS->rcItem.left+1, lpDIS->rcItem.bottom-2);
		pDC->LineTo(lpDIS->rcItem.right-1, lpDIS->rcItem.bottom-2);
	}
	else
	{
		CXTPPenDC penDC(pDC->m_hDC, m_cr3DShadow);
		pDC->MoveTo(lpDIS->rcItem.right-2, 1);
		pDC->LineTo(lpDIS->rcItem.right-2, lpDIS->rcItem.bottom-1);

		penDC.Color(m_cr3DDkShadow);
		pDC->MoveTo(lpDIS->rcItem.right-1, 0);
		pDC->LineTo(lpDIS->rcItem.right-1, lpDIS->rcItem.bottom-1);

		penDC.Color(m_cr3DHighLight);
		pDC->MoveTo(lpDIS->rcItem.right, 1);
		pDC->LineTo(lpDIS->rcItem.right, lpDIS->rcItem.bottom-1);
	}
}

BOOL CXTHeaderTheme::OnDrawBackgroundWinThemed(LPDRAWITEMSTRUCT lpDIS)
{
	if ((GetDrawStyle() & XTTHEME_HOTTRACKING) == 0)
		return FALSE;

	if (m_themeWrapper.DrawThemeBackground(lpDIS->hDC,
		0, HIS_NORMAL, &lpDIS->rcItem, NULL) != S_OK)
	{
		TRACE0("Error drawing background using WinTheme API.\n");
		return FALSE;
	}

	return TRUE;
}

BOOL CXTHeaderTheme::OnDrawItemBackgroundWinThemed(LPDRAWITEMSTRUCT lpDIS)
{
	int iState = HIS_NORMAL;

	if (lpDIS->itemState & ODS_HOTLIGHT)
		iState = HIS_HOT;

	if (lpDIS->itemState & ODS_SELECTED)
		iState = HIS_PRESSED;

	if (m_themeWrapper.DrawThemeBackground(lpDIS->hDC,
		HP_HEADERITEM, iState, &lpDIS->rcItem, NULL) != S_OK)
	{
		TRACE0("Error drawing background using WinTheme API.\n");
		return FALSE;
	}

	return TRUE;
}

void CXTHeaderTheme::DrawSortArrow(CDC* pDC, LPPOINT ptsArrow, BOOL bAscending)
{
	CXTPPenDC dcPen(pDC->m_hDC, bAscending ? m_cr3DHighLight : m_cr3DShadow);
	pDC->MoveTo(ptsArrow[2]);
	pDC->LineTo(ptsArrow[bAscending ? 1 : 0]);
	pDC->LineTo(ptsArrow[bAscending ? 0 : 1]);

	dcPen.Color(bAscending ? m_cr3DShadow : m_cr3DHighLight);
	pDC->MoveTo(ptsArrow[bAscending ? 0 : 2]);
	pDC->LineTo(ptsArrow[bAscending ? 2 : 1]);
}

void CXTHeaderTheme::OnDrawItemSortArrow(LPDRAWITEMSTRUCT lpDIS, BOOL bAscending)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect rcItem = lpDIS->rcItem;

	CPoint point;
	point.x = rcItem.right + m_iArrowPadding;
	point.y = (rcItem.top - 1) + (rcItem.Height() - m_sizeArrow.cy) / 2;

	if (point.x < rcItem.left)
		return;

	if (lpDIS->itemState == ODS_SELECTED)
		point.Offset(1,1);

	CRect rcArrow;
	rcArrow.left   = point.x;
	rcArrow.top    = point.y;
	rcArrow.right  = point.x + m_sizeArrow.cx;
	rcArrow.bottom = point.y + m_sizeArrow.cy;

	int x = point.x + (m_sizeArrow.cx/2);
//  int y = point.y + (m_sizeArrow.cy/2);

	POINT ptsArrow[3];
	ptsArrow[0].x = x-m_sizeArrow.cx/2;
	ptsArrow[0].y = bAscending ? rcArrow.bottom : rcArrow.top;
	ptsArrow[1].x = x+m_sizeArrow.cx/2;
	ptsArrow[1].y = bAscending ? rcArrow.bottom : rcArrow.top;
	ptsArrow[2].x = x;
	ptsArrow[2].y = bAscending ? rcArrow.top : rcArrow.bottom;

	DrawSortArrow(pDC, ptsArrow, bAscending);
}

void CXTHeaderTheme::OnDrawItemText(LPDRAWITEMSTRUCT lpDIS, CXTHeaderCtrl* pHeaderCtrl)
{
	ASSERT_VALID(pHeaderCtrl);

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	LPHDITEM lpHDI = (LPHDITEM)lpDIS->itemData;

	// prepare the device context to draw text.
	CXTPFontDC fontDC(pDC, pHeaderCtrl->GetFont());

	// construct text size.
	CRect rcItem(lpDIS->rcItem);
	rcItem.DeflateRect(m_sizePadding.cx, 0);

	BOOL bSortAsc;
	UINT nSortItem = pHeaderCtrl->GetSortedCol(&bSortAsc);
	if (nSortItem == lpDIS->itemID)
		rcItem.right -= m_sizeArrow.cx + m_iArrowPadding*2;

	CRect rcText(rcItem);
	CSize sizeText = pDC->GetTextExtent(lpHDI->pszText);

	rcText.top = (m_sizePadding.cy > 0) ?
		m_sizePadding.cy : (rcItem.Height()-sizeText.cy)/2;
	rcText.bottom = rcText.top + sizeText.cy;

	if (lpHDI->fmt & HDF_BITMAP && lpHDI->hbm != NULL)
		OnDrawItemBitmap(lpDIS, rcText, pHeaderCtrl);

	// determine justification for text.
	switch (lpHDI->fmt & HDF_JUSTIFYMASK)
	{
	case HDF_LEFT:
		rcText.right = rcText.left + sizeText.cx;
		break;
	case HDF_CENTER:
		rcText.left += (rcItem.Width()-sizeText.cx)/2;
		rcText.right = rcText.left + sizeText.cx;
		break;
	case HDF_RIGHT:
		rcText.left = rcText.right - sizeText.cx;
		break;
	}

	// size text to fit available display area.
	rcText.left  = __max(rcText.left,  rcItem.left);
	rcText.right = __min(rcText.right, rcItem.right);

	// check if we are sorting.
	if (HasSortArrow() && nSortItem == lpDIS->itemID)
	{
		lpDIS->rcItem = rcText;
		OnDrawItemSortArrow(lpDIS, bSortAsc);
	}

	if (rcText.Width() > 0)
	{
		if (lpDIS->itemState == ODS_SELECTED)
			rcText.OffsetRect(1,1);

		// draw text.
		pDC->DrawText(lpHDI->pszText, &rcText,
			DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	}
}

void CXTHeaderTheme::DrawHeader(CDC* pDC, CXTHeaderCtrl* pHeaderCtrl)
{
	ASSERT_VALID(pHeaderCtrl);

	CXTPClientRect rcClient(pHeaderCtrl);

	DRAWITEMSTRUCT dis;
	dis.CtlType    = ODT_BUTTON;
	dis.CtlID      = 0;
	dis.itemID     = 0;
	dis.itemAction = ODA_DRAWENTIRE;
	dis.itemState  = 0;
	dis.hwndItem   = pHeaderCtrl->m_hWnd;
	dis.hDC        = pDC->m_hDC;
	dis.rcItem     = rcClient;
	dis.itemData   = 0;

	// draw background.
	if (!m_bUseWinThemes || !OnDrawBackgroundWinThemed(&dis))
		OnDrawBackground(&dis);

	// set text attributes.
	CXTPFontDC fontDC(pDC, pHeaderCtrl->GetFont());
	pDC->SetBkColor(m_cr3DFace);
	pDC->SetTextColor(m_crText);
	pDC->SetBkMode(TRANSPARENT);

	// Set up the header item order array.
	HD_ITEM hdi;
	::ZeroMemory(&hdi, sizeof(HD_ITEM));

	hdi.fmt  = HDF_STRING|HDF_LEFT|HDF_BITMAP;
	hdi.mask = HDI_WIDTH|HDI_TEXT|HDI_FORMAT|HDI_ORDER|HDI_BITMAP|HDI_LPARAM;

	int cxy        = 0;
	int iItem      = 0;
	int iItemCount = pHeaderCtrl->GetItemCount();
	int *plItems   = new int[iItemCount];

	for (iItem = 0; iItem < iItemCount; iItem++)
	{
		pHeaderCtrl->GetItem(iItem, &hdi);
		plItems[hdi.iOrder] = iItem;
	}

	hdi.fmt  = HDF_STRING|HDF_LEFT|HDF_BITMAP;
	hdi.mask = HDI_WIDTH|HDI_TEXT|HDI_FORMAT|HDI_BITMAP|HDI_LPARAM;

	// get cursor position.
	CPoint point;
	::GetCursorPos(&point);
	pHeaderCtrl->ScreenToClient(&point);

	// Draw each header item
	for (iItem = 0; iItem < iItemCount; ++iItem)
	{
		// Get the header item text and format
		TCHAR szBuffer[256];
		hdi.pszText    = szBuffer;
		hdi.cchTextMax = 255;
		pHeaderCtrl->GetItem(plItems[iItem], &hdi);

		// initialize draw item structure.
		dis.itemID       = plItems[iItem];
		dis.itemState    = 0;
		dis.rcItem       = rcClient;
		dis.rcItem.left  = cxy;
		dis.rcItem.right = dis.rcItem.left + hdi.cxy;
		dis.itemData     = (DWORD_PTR)&hdi;

		// if hot tracking is enabled, set the state.
		if (GetDrawStyle() & XTTHEME_HOTTRACKING)
		{
			if (pHeaderCtrl->HitTest(point) == plItems[iItem])
				dis.itemState = ODS_HOTLIGHT;
		}

		// check if the item is selected
		if (pHeaderCtrl->ItemPressed(plItems[iItem]))
			dis.itemState = ODS_SELECTED;

		// draw the item background.
		if (!m_bUseWinThemes || !OnDrawItemBackgroundWinThemed(&dis))
			OnDrawItemBackground(&dis);

		// draw the item text.
		OnDrawItemText(&dis, pHeaderCtrl);
		cxy += hdi.cxy;
	}

	delete [] plItems;
}

BOOL CXTHeaderTheme::UseWinXPThemes(CXTHeaderCtrl* pHeaderCtrl)
{
	ASSERT_VALID(pHeaderCtrl);

	// if windows xp themes are not allowed, return FALSE.
	if ((GetDrawStyle() & XTTHEME_WINXPTHEMES) == 0)
		return FALSE;

	// if we got this far then we try to load the theme data for
	// this control if it is not currently open.
	if (!m_themeWrapper.ThemeDataOpen())
		m_themeWrapper.OpenThemeData(pHeaderCtrl->m_hWnd, L"HEADER");

	// if our application is not "Theme Ready" meaning that we cannot
	// display Windows XP themes, then return FALSE.
	if (!m_themeWrapper.IsAppThemeReady())
		return FALSE;

	// this will return TRUE if we can display visual styles.
	return m_themeWrapper.ThemeDataOpen();
}

void CXTHeaderTheme::SetDrawStyle(DWORD dwDrawStyle, CXTHeaderCtrl* pHeaderCtrl)
{
	m_dwDrawStyle = dwDrawStyle;
	m_bUseWinThemes = UseWinXPThemes(pHeaderCtrl) && ((m_dwDrawStyle & XTTHEME_WINXPTHEMES) != 0);
}

DWORD CXTHeaderTheme::GetDrawStyle() const
{
	return m_dwDrawStyle;
}

BOOL CXTHeaderTheme::Layout(LPHDLAYOUT lpHDL, CXTHeaderCtrl* pHeaderCtrl)
{
	ASSERT_VALID(pHeaderCtrl);
	return (BOOL)pHeaderCtrl->DefWindowProc(HDM_LAYOUT,
		(WPARAM)0, (LPARAM)lpHDL);
}

BOOL CXTHeaderTheme::SetBitmap(int iCol, UINT uBitmapID, BOOL bRemove, COLORREF crMask, CXTHeaderCtrl* pHeaderCtrl)
{
	ASSERT_VALID(pHeaderCtrl);

	if (bRemove)
	{
		HD_ITEM hdi;
		::ZeroMemory(&hdi, sizeof(HD_ITEM));

		hdi.mask = HDI_BITMAP | HDI_FORMAT;
		pHeaderCtrl->GetItem(iCol, &hdi);

		// free previous GDI resources.
		if (hdi.fmt & HDF_BITMAP && hdi.hbm != NULL)
			::ImageList_Destroy((HIMAGELIST)hdi.hbm);

		hdi.hbm  = NULL;
		hdi.fmt &= ~HDF_BITMAP;

		pHeaderCtrl->SetItem(iCol, &hdi);
	}
	else
	{
		HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetResourceHandle(),
			MAKEINTRESOURCE(uBitmapID), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

		if (hBitmap == NULL)
			return FALSE;

		BITMAP bmpInfo;
		if (::GetObject(hBitmap, sizeof(BITMAP), &bmpInfo) != 0)
		{
			HIMAGELIST hImageList = ::ImageList_Create(bmpInfo.bmWidth,
				bmpInfo.bmHeight, ILC_MASK|ILC_COLOR24, 0, 1);

			if (hImageList != NULL)
			{
				if (::ImageList_AddMasked(hImageList, hBitmap, crMask) != -1)
				{
					HD_ITEM hdi;
					::ZeroMemory(&hdi, sizeof(HD_ITEM));

					hdi.mask = HDI_BITMAP | HDI_FORMAT;
					pHeaderCtrl->GetItem(iCol, &hdi);

					// free previous GDI resources.
					if (hdi.fmt & HDF_BITMAP && hdi.hbm != NULL)
						::ImageList_Destroy((HIMAGELIST)hdi.hbm);

					// we are actually storing a handle to the image list and
					// not the bitmap so we can draw using the mask.
					hdi.hbm = (HBITMAP)hImageList;
					hdi.fmt = HDF_BITMAP;

					pHeaderCtrl->SetItem(iCol, &hdi);

					::DeleteObject(hBitmap);
					return TRUE;
				}
				::ImageList_Destroy(hImageList);
			}
		}

		::DeleteObject(hBitmap);
	}

	return FALSE;
}

void CXTHeaderTheme::OnDrawItemBitmap(LPDRAWITEMSTRUCT lpDIS, CRect& rcText, CXTHeaderCtrl* pHeaderCtrl)
{
	ASSERT_VALID(pHeaderCtrl);

	HDITEM hdi;
	hdi.mask = HDI_BITMAP | HDI_FORMAT;
	pHeaderCtrl->GetItem(lpDIS->itemID, &hdi);

	if (hdi.fmt & HDF_BITMAP && hdi.hbm != NULL)
	{
		HIMAGELIST hImageList = (HIMAGELIST)hdi.hbm;

		int cx, cy;
		if (::ImageList_GetIconSize(hImageList, &cx, &cy))
		{
			rcText.left += cx + m_sizePadding.cx;

			CRect rcItem(lpDIS->rcItem);
			CPoint point(rcItem.left+m_sizePadding.cx, (rcItem.Height()-cy)/2);

			if (lpDIS->itemState == ODS_SELECTED)
				point.Offset(1,1);

			if (point.x+cx > lpDIS->rcItem.right)
				return;

			// draw image transparently.
			::ImageList_Draw(hImageList, 0, lpDIS->hDC, point.x, point.y, ILD_TRANSPARENT);
		}
	}
}

void CXTHeaderTheme::CleanUp(CXTHeaderCtrl* pHeaderCtrl)
{
	ASSERT_VALID(pHeaderCtrl);

	// free GDI resources
	for (int i = 0; i < pHeaderCtrl->GetItemCount(); ++i)
	{
		HDITEM hdi;
		hdi.mask = HDI_FORMAT | HDI_BITMAP;
		pHeaderCtrl->GetItem(i, &hdi);

		if (hdi.fmt & HDF_BITMAP && hdi.hbm != NULL)
		{
			::ImageList_Destroy((HIMAGELIST)hdi.hbm);
		}
	}
}

//=============================================================================
// CXTHeaderThemeOfficeXP
//=============================================================================

CXTHeaderThemeOfficeXP::CXTHeaderThemeOfficeXP()
{
	m_dwDrawStyle = XTTHEME_SORTARROW;
}

CXTHeaderThemeOfficeXP::~CXTHeaderThemeOfficeXP()
{

}

void CXTHeaderThemeOfficeXP::OnDrawBackground(LPDRAWITEMSTRUCT lpDIS)
{
	CRect rcItem(lpDIS->rcItem);
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	// fill background
	pDC->FillSolidRect(&rcItem, m_cr3DFace);
	rcItem.right += 2;

	// draw 3D borders.
	pDC->Draw3dRect(&rcItem, m_cr3DHighLight, m_cr3DShadow);
}

void CXTHeaderThemeOfficeXP::OnDrawItemBackground(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	if (lpDIS->itemState == ODS_SELECTED)
	{
		CXTPPenDC penDC(lpDIS->hDC, m_cr3DShadow);
		pDC->MoveTo(lpDIS->rcItem.right-2, 2);
		pDC->LineTo(lpDIS->rcItem.right-2, lpDIS->rcItem.bottom-2);

		penDC.Color(m_cr3DHighLight);
		pDC->MoveTo(lpDIS->rcItem.right-1, 2);
		pDC->LineTo(lpDIS->rcItem.right-1, lpDIS->rcItem.bottom-2);
	}
	else
	{
		CXTPPenDC penDC(lpDIS->hDC, m_cr3DShadow);
		pDC->MoveTo(lpDIS->rcItem.right-2, 2);
		pDC->LineTo(lpDIS->rcItem.right-2, lpDIS->rcItem.bottom-2);

		penDC.Color(m_cr3DHighLight);
		pDC->MoveTo(lpDIS->rcItem.right-1, 2);
		pDC->LineTo(lpDIS->rcItem.right-1, lpDIS->rcItem.bottom-2);
	}
}

//=============================================================================
// CXTHeaderThemeOffice2003
//=============================================================================

CXTHeaderThemeOffice2003::CXTHeaderThemeOffice2003()
{
	m_dwDrawStyle = XTTHEME_SORTARROW;
	m_sizePadding = CSize(6,2);
}

CXTHeaderThemeOffice2003::~CXTHeaderThemeOffice2003()
{

}

void CXTHeaderThemeOffice2003::OnDrawBackground(LPDRAWITEMSTRUCT lpDIS)
{
	CRect rcItem(lpDIS->rcItem);
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	// fill background
	pDC->FillSolidRect(&rcItem, m_cr3DFace);
	rcItem.right += 2;

	// draw 3D borders.
	pDC->Draw3dRect(&rcItem, m_cr3DHighLight, MixColor(m_cr3DFace, m_cr3DShadow, 0.6));

	CXTPPenDC penDC(lpDIS->hDC, MixColor(m_cr3DFace, m_cr3DShadow, 0.4));
	pDC->MoveTo(rcItem.left,  rcItem.bottom-2);
	pDC->LineTo(rcItem.right, rcItem.bottom-2);

	penDC.Color(MixColor(m_cr3DFace, m_cr3DShadow, 0.25));
	pDC->MoveTo(rcItem.left,  rcItem.bottom-3);
	pDC->LineTo(rcItem.right, rcItem.bottom-3);
}

void CXTHeaderThemeOffice2003::OnDrawItemBackground(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	if (lpDIS->itemState == ODS_SELECTED)
	{
		CXTPPenDC penDC(lpDIS->hDC, m_cr3DShadow);
		pDC->MoveTo(lpDIS->rcItem.right-2, 2);
		pDC->LineTo(lpDIS->rcItem.right-2, lpDIS->rcItem.bottom-4);

		penDC.Color(m_cr3DHighLight);
		pDC->MoveTo(lpDIS->rcItem.right-1, 2);
		pDC->LineTo(lpDIS->rcItem.right-1, lpDIS->rcItem.bottom-4);
	}
	else
	{
		CXTPPenDC penDC(lpDIS->hDC, m_cr3DShadow);
		pDC->MoveTo(lpDIS->rcItem.right-2, 2);
		pDC->LineTo(lpDIS->rcItem.right-2, lpDIS->rcItem.bottom-4);

		penDC.Color(m_cr3DHighLight);
		pDC->MoveTo(lpDIS->rcItem.right-1, 2);
		pDC->LineTo(lpDIS->rcItem.right-1, lpDIS->rcItem.bottom-4);
	}
}

BOOL CXTHeaderThemeOffice2003::Layout(LPHDLAYOUT lpHDL, CXTHeaderCtrl* pHeaderCtrl)
{
	ASSERT_VALID(pHeaderCtrl);

	LPRECT      pRC = lpHDL->prc;
	LPWINDOWPOS pWP = lpHDL->pwpos;

	CWindowDC dc(NULL);
	CXTPFontDC fontDC(&dc, pHeaderCtrl->GetFont());

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	int iHeight = tm.tmHeight + 10;

	pWP->hwndInsertAfter = 0;
	pWP->x     = pRC->left;
	pWP->cx    = pRC->right - pRC->left;
	pWP->cy    = iHeight;
	pWP->y     = pRC->top;
	pRC->top  += iHeight;
	pWP->flags = SWP_NOACTIVATE | SWP_NOZORDER;

	return TRUE;
}

//=============================================================================
// CXTHeaderThemeExplorer
//=============================================================================

CXTHeaderThemeExplorer::CXTHeaderThemeExplorer()
{
	m_dwDrawStyle = XTTHEME_WINXPTHEMES|XTTHEME_HOTTRACKING|XTTHEME_SORTARROW;
	m_sizePadding = CSize(6,0);
	m_sizeArrow   = CSize(8,4);
}

CXTHeaderThemeExplorer::~CXTHeaderThemeExplorer()
{

}

BOOL CXTHeaderThemeExplorer::OnDrawItemBackgroundWinThemed(LPDRAWITEMSTRUCT lpDIS)
{
	return CXTHeaderTheme::OnDrawItemBackgroundWinThemed(lpDIS);
}

BOOL CXTHeaderThemeExplorer::Layout(LPHDLAYOUT lpHDL, CXTHeaderCtrl* pHeaderCtrl)
{
	ASSERT_VALID(pHeaderCtrl);

	LPRECT      pRC = lpHDL->prc;
	LPWINDOWPOS pWP = lpHDL->pwpos;

	CWindowDC dc(NULL);
	CXTPFontDC fontDC(&dc, pHeaderCtrl->GetFont());

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	int iHeight = tm.tmHeight + 6;

	pWP->hwndInsertAfter = 0;
	pWP->x     = pRC->left;
	pWP->cx    = pRC->right - pRC->left;
	pWP->cy    = iHeight;
	pWP->y     = pRC->top;
	pRC->top  += iHeight;
	pWP->flags = SWP_NOACTIVATE | SWP_NOZORDER;

	return TRUE;
}

void CXTHeaderThemeExplorer::DrawSortArrow(CDC* pDC, LPPOINT ptsArrow, BOOL bAscending)
{
	UNREFERENCED_PARAMETER(bAscending);

	CXTPPenDC   dcPen(pDC->m_hDC, RGB(172,168,153));
	CXTPBrushDC dcBrush(pDC->m_hDC, RGB(172,168,153));

	pDC->SetPolyFillMode(WINDING);
	pDC->Polygon(ptsArrow, 3);
}
