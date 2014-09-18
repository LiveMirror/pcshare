// DialogBitmapImages.cpp : implementation file
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
#include "DialogBitmapImages.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDialogBitmapImagesHost::CDialogBitmapImagesHost(UINT nIDTemplate, CResourceManager* pResourceManager)
	: CDialog(nIDTemplate, 0)
{
	m_pResourceManager = pResourceManager;
	m_imageState = xtpImageNormal;
	m_clrMask = RGB(0, 0xFF, 0);
	m_szIcons = 0;
	m_pSelected = 0;


}

/////////////////////////////////////////////////////////////////////////////
// CDialogBitmapImages

CDialogBitmapImages::CDialogBitmapImages()
{
	m_bScrollVisible[0] = m_bScrollVisible[1] = FALSE;
	m_bAllowEdit = TRUE;
}

CDialogBitmapImages::~CDialogBitmapImages()
{
}


BEGIN_MESSAGE_MAP(CDialogBitmapImages, CWnd)
	//{{AFX_MSG_MAP(CDialogBitmapImages)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDialogBitmapImages message handlers

void CDialogBitmapImages::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// its horizontal scroll bar
	int nCurPos = GetScrollPos(SB_HORZ);

	// decide what to do for each diffrent scroll event
	switch(nSBCode)
	{
		case SB_LEFT: nCurPos = 0; break;
		case SB_RIGHT: nCurPos = GetScrollLimit(SB_HORZ); break;
		case SB_LINELEFT: nCurPos = max(nCurPos - 6, 0); break;
		case SB_LINERIGHT: nCurPos = min(nCurPos + 6, GetScrollLimit(SB_HORZ)); break;
		case SB_PAGELEFT: nCurPos = max(nCurPos - CXTPClientRect(this).Width(), 0); break;
		case SB_PAGERIGHT: nCurPos = min(nCurPos + CXTPClientRect(this).Width(), GetScrollLimit(SB_HORZ)); break;

		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:  nCurPos = nPos; break;
	}

	SetScrollPos(SB_HORZ, nCurPos);
	Invalidate(FALSE);

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

int CDialogBitmapImages::GetScrollOffset(int nBar)
{
	if (!m_bScrollVisible[nBar])
		return 0;

	return GetScrollPos(nBar);
}

CPoint CDialogBitmapImages::GetTopLeftPoint()
{
	CDialogBitmapImagesHost* pParent = (CDialogBitmapImagesHost*)GetParent();
	CXTPClientRect rc(this);

	int nCount = (int)pParent->m_arrImages.GetSize();
	CSize szIcons = pParent->m_szIcons;

	int nHeight = rc.Height();

	int nTopOffset = nHeight > szIcons.cy? (nHeight - szIcons.cy) / 2 : 0;

	int nLeftOffset = nTopOffset + szIcons.cx * nCount < rc.Width()? (rc.Width() - szIcons.cx * nCount) / 2: 0;

	nLeftOffset -= GetScrollOffset(SB_HORZ);

	return CPoint(nLeftOffset, nTopOffset);

}

void CDialogBitmapImages::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting

	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint, rc);
	dc.FillSolidRect(rc, GetSysColor(COLOR_3DSHADOW));

	if (!m_bAllowEdit)
	{
		for (int x = 0; x < rc.Width(); x++)
		for (int y = 0; y < rc.Height(); y++)
		{
			COLORREF clr = (((x +3) / 4 )+ ((y +3)/ 4)) % 2 == 0? RGB(255, 255, 255): RGB(235, 235, 235);
			dc.SetPixel(x, rc.Height() - y - 1, clr);
		}
	}

	CDialogBitmapImagesHost* pParent = (CDialogBitmapImagesHost*)GetParent();

	int nCount = (int)pParent->m_arrImages.GetSize();
	CSize szIcons = pParent->m_szIcons;

	CPoint ptTopLeft = GetTopLeftPoint();

	if (m_bAllowEdit)
		dc.FillSolidRect(ptTopLeft.x, ptTopLeft.y, szIcons.cx * nCount, szIcons.cy, pParent->m_clrMask);
	else
	{
		CRect rcFocus(ptTopLeft, CSize(szIcons.cx * nCount, szIcons.cy));
		rcFocus.InflateRect(1, 1);
		dc.DrawFocusRect(rcFocus);
	}

	for (int i = 0; i < nCount; i++)
	{
		CPoint pt(ptTopLeft.x + i * szIcons.cx, ptTopLeft.y);

		CXTPImageManagerIcon* pIcon = pParent->m_arrImages[i].pIcon;
		if (pIcon)
		{
			CXTPImageManagerIconHandle& pHandle = pParent->m_arrImages[i].GetHandle();
			pIcon->Draw(&dc, pt, pHandle, szIcons);
		}
	}

	if (pParent->m_pSelected)
	{
		CPoint pt(ptTopLeft.x + pParent->m_pSelected->nIndex * szIcons.cx, ptTopLeft.y);
		CRect rc(pt, szIcons);

		rc.InflateRect(1, 1);
		dc.Draw3dRect(rc, 0, 0);
		rc.InflateRect(1, 1);
		dc.Draw3dRect(rc, 0, 0);
	}
}

BOOL CDialogBitmapImages::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

CImage* CDialogBitmapImages::HitTest(CPoint point)
{
	CDialogBitmapImagesHost* pParent = (CDialogBitmapImagesHost*)GetParent();

	int nCount = (int)pParent->m_arrImages.GetSize();
	CSize szIcons = pParent->m_szIcons;
	CPoint ptTopLeft = GetTopLeftPoint();

	for (int i = 0; i < nCount; i++)
	{
		CPoint pt(ptTopLeft.x + i * szIcons.cx, ptTopLeft.y);
		CRect rc(pt, szIcons);

		if (rc.PtInRect(point))
			return &pParent->m_arrImages[i];
	}


	return NULL;
}

void CDialogBitmapImages::OnLButtonDown(UINT nFlags, CPoint point)
{
	CImage* pImage = HitTest(point);
	CDialogBitmapImagesHost* pParent = (CDialogBitmapImagesHost*)GetParent();

	if (pParent->m_pSelected != pImage)
	{
		pParent->OnImageChanged(pImage);
	}

	SetFocus();

	CWnd::OnLButtonDown(nFlags, point);
}

void CDialogBitmapImages::OnImageCountChanged()
{
	CDialogBitmapImagesHost* pParent = (CDialogBitmapImagesHost*)GetParent();

	CXTPClientRect rc(this);
	int nCount = (int)pParent->m_arrImages.GetSize();
	CSize szIcons = pParent->m_szIcons;


	int nWidth = szIcons.cx * nCount;

	m_bScrollVisible[SB_HORZ] = rc.Width() < nWidth;

	ShowScrollBar(SB_HORZ, m_bScrollVisible[SB_HORZ]);

	if (m_bScrollVisible[SB_HORZ])
	{
		SCROLLINFO  si ;

		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_PAGE | SIF_RANGE ;


		si.nPage = rc.Width();
		si.nMax = nWidth - 1;
		si.nMin = 0 ;
		SetScrollInfo(SB_HORZ, &si) ;

		EnableScrollBarCtrl(SB_HORZ, TRUE);
	}




	Invalidate(FALSE);
}

void CDialogBitmapImages::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDialogBitmapImagesHost* pParent = (CDialogBitmapImagesHost*)GetParent();
	CArray<CImage, CImage&>& arrImages = pParent->m_arrImages;

	int nSelected = pParent->m_pSelected? pParent->m_pSelected->nIndex: -1;
	int nIndex = nSelected;

	if (nChar == VK_LEFT)
	{
		if (nIndex > 0)
		{
			nIndex--;
		}
		if (nIndex >= 0 && nIndex < arrImages.GetSize())
		{
			if (GetKeyState(VK_CONTROL) >= 0 || !m_bAllowEdit)
			{
				pParent->OnImageChanged(&arrImages[nIndex]);
			}
			else
			{
				CImage image = arrImages[nSelected];
				arrImages.RemoveAt(nSelected);
				arrImages.InsertAt(nIndex, image);

				arrImages[nIndex].nIndex = nIndex;
				arrImages[nSelected].nIndex = nSelected;

				pParent->m_pSelected = &arrImages[nIndex];

				Invalidate(FALSE);
			}
		}
	}
	if (nChar == VK_RIGHT)
	{
		nIndex++;

		if (nIndex >= 0 && nIndex < arrImages.GetSize())
		{
			if (GetKeyState(VK_CONTROL) >= 0 || nIndex == 0 || !m_bAllowEdit)
			{
				pParent->OnImageChanged(&arrImages[nIndex]);
			}
			else
			{
				CImage image = arrImages[nSelected];
				arrImages.RemoveAt(nSelected);
				arrImages.InsertAt(nIndex, image);

				arrImages[nSelected].nIndex = nSelected;
				arrImages[nIndex].nIndex = nIndex;

				pParent->m_pSelected = &arrImages[nIndex];

				Invalidate(FALSE);

			}
		}
	}

	if (nChar == VK_DELETE && m_bAllowEdit)
	{
		if (nIndex >= 0 && nIndex < arrImages.GetSize())
		{
			arrImages.RemoveAt(nIndex);

			for (int i = nIndex; i < arrImages.GetSize(); i++)
				arrImages[i].nIndex = i;

			if (nIndex < arrImages.GetSize())
			{
				pParent->OnImageChanged(&arrImages[nIndex]);
			}
			else
			{
				pParent->OnImageChanged(0);
			}

			OnImageCountChanged();
		}
	}

	if (nChar == VK_INSERT && m_bAllowEdit)
	{
		CImage image;
		image.imageState = (XTPImageState)-1;
		image.nID = 0;
		image.pIcon = 0;
		image.pParent = pParent;

		nIndex = nIndex == -1? (int)arrImages.GetSize(): nIndex;
		arrImages.InsertAt(nIndex, image);

		for (int i = 0; i < arrImages.GetSize(); i++)
			arrImages[i].nIndex = i;

		pParent->OnImageChanged(&arrImages[nIndex]);
		OnImageCountChanged();
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

UINT CDialogBitmapImages::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}
