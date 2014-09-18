// XTPControlPopupColor.cpp : implementation of the CXTPControlPopupColor class.
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
#include "XTPResourceManager.h"
#include "XTPDrawHelpers.h"
#include "XTPColorManager.h"
#include "XTPImageManager.h"

#include "XTPControlPopup.h"
#include "XTPControlButton.h"
#include "XTPControlPopupColor.h"
#include "XTPCommandBarsResource.h"
#include "XTPCommandBar.h"
#include "XTPPaintManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CLR_TO_RGBQUAD(clr)     (RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)))

#define XTP_SELECTOR_SIZE 18
#define XTP_SELECTOR_MARGIN 2

#define XTP_SELECTOR_WIDTH (XTP_SELECTOR_SIZE * 8 + XTP_SELECTOR_MARGIN * 2)
#define XTP_SELECTOR_HEIGHT (XTP_SELECTOR_SIZE * 5 + XTP_SELECTOR_MARGIN * 2)

IMPLEMENT_XTP_CONTROL(CXTPControlPopupColor, CXTPControlPopup)

CXTPControlPopupColor::CXTPControlPopupColor(void)
{
	m_controlType = xtpControlSplitButtonPopup;
	m_clr = (COLORREF)-1;

}

CXTPControlPopupColor::~CXTPControlPopupColor(void)
{
}

void CXTPControlPopupColor::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	CXTPControlPopup::Copy(pControl, bRecursive);
	m_clr = ((CXTPControlPopupColor*)pControl)->m_clr;
}

void CXTPControlPopupColor::RedrawIcon(CXTPImageManagerIcon* pImage, CXTPImageManagerIconHandle* pHandle)
{
	CXTPImageManagerIconHandle& hIcon = pImage->GetIcon();

	if (hIcon.IsEmpty())
		return;

	if (!hIcon.IsAlpha())
	{
		ICONINFO info;

		if (GetIconInfo(hIcon.GetIcon(), &info))
		{
			{
				CXTPCompatibleDC dc(NULL, CBitmap::FromHandle(info.hbmColor));
				CXTPCompatibleDC dcMask(NULL, CBitmap::FromHandle(info.hbmMask));

				BITMAP bmp;
				::GetObject(info.hbmColor, sizeof (BITMAP), &bmp);
				int nHeight = int((double)bmp.bmHeight / 5);

				CRect rc(0, bmp.bmHeight - nHeight, bmp.bmWidth, bmp.bmHeight);
				dc.FillSolidRect(rc, m_clr);
				dcMask.FillSolidRect(rc, 1);
			}

			*pHandle = CreateIconIndirect(&info);

			::DeleteObject(info.hbmMask);
			::DeleteObject(info.hbmColor);
		}
	}
	else
	{
		CDC dcSrc;
		dcSrc.CreateCompatibleDC(NULL);

		PBYTE pBits = 0;
		PBITMAPINFO pBitmapInfo = 0;

		pHandle->CopyHandle(hIcon);

		TRY
		{
			UINT nSize;
			if (!CXTPImageManagerIcon::GetBitmapBits(dcSrc, pHandle->GetBitmap(), pBitmapInfo, (LPVOID&)pBits, nSize))
				AfxThrowMemoryException();

			DWORD dwColor = 0xFF000000 | CLR_TO_RGBQUAD(m_clr);
			int nCount = pBitmapInfo->bmiHeader.biHeight / 5 * pBitmapInfo->bmiHeader.biWidth * 4;

			for (int i = 0; i < nCount; i += 4)
			{
				*LPDWORD(&pBits[i]) = dwColor;
			}

			SetDIBits(dcSrc, pHandle->GetBitmap(), 0, pBitmapInfo->bmiHeader.biHeight, pBits, pBitmapInfo, DIB_RGB_COLORS);


		}
		CATCH (CMemoryException, e)
		{
			TRACE(_T("Failed -- Memory exception thrown."));
		}
		END_CATCH

		free(pBits);
		free(pBitmapInfo);

	}
}

void CXTPControlPopupColor::SetColor(COLORREF clr)
{
	if (clr != m_clr)
	{
		m_clr = clr;

		CXTPImageManagerIconSet* pIconSet = GetImageManager()->GetIconSet(GetIconId());
		if (pIconSet)
		{
			pIconSet->RefreshAll();

			CXTPImageManagerIconSet::CIconSetMap* pMap = pIconSet->GetIcons();

			POSITION pos = pMap->GetStartPosition();
			UINT nWidth;
			CXTPImageManagerIcon* pImage;
			while (pos != NULL)
			{
				pMap->GetNextAssoc( pos, nWidth, pImage);

				CXTPImageManagerIconHandle hIcon;
				RedrawIcon(pImage, &hIcon);

				if (!hIcon.IsEmpty())
				{
					pImage->SetIcon(hIcon);
				}
			}
			m_pParent->Invalidate(FALSE);
		}
	}
}

IMPLEMENT_XTP_CONTROL(CXTPControlButtonColor, CXTPControlButton)

CSize CXTPControlButtonColor::GetSize(CDC* /*pDC*/)
{
	//CSize sz = CXTPControl::GetSize(pDC);
	return CSize(XTP_SELECTOR_WIDTH, GetPaintManager()->GetControlHeight());
}

void CXTPControlButtonColor::Draw(CDC* pDC)
{
	CXTPFontDC font (pDC, GetPaintManager()->GetRegularFont());

	GetPaintManager()->DrawControlEntry(pDC, this);
	CRect rcText = GetRect();
	GetPaintManager()->DrawTextEx(pDC, GetCaption(), &rcText, TRUE, FALSE, TRUE, FALSE, FALSE);
}

struct CXTPControlColorSelector::CColorInfo
{
	CColorInfo(COLORREF _clrButton, UINT _toolTipID)
	{
		clrButton = _clrButton;
		toolTipID = _toolTipID;
	}

	COLORREF clrButton;
	UINT     toolTipID;
	CString strTip;
};

static CXTPControlColorSelector::CColorInfo extendedColors[] =
{
	CXTPControlColorSelector::CColorInfo(RGB(0x00,0x00,0x00),   XTP_IDS_CLR_BLACK),
	CXTPControlColorSelector::CColorInfo(RGB(0x99,0x33,0x00),   XTP_IDS_CLR_BROWN           ),
	CXTPControlColorSelector::CColorInfo(RGB(0x33,0x33,0x00),   XTP_IDS_CLR_OLIVE_GREEN     ),
	CXTPControlColorSelector::CColorInfo(RGB(0x00,0x33,0x00),   XTP_IDS_CLR_DARK_GREEN      ),
	CXTPControlColorSelector::CColorInfo(RGB(0x00,0x33,0x66),   XTP_IDS_CLR_DARK_TEAL       ),
	CXTPControlColorSelector::CColorInfo(RGB(0x00,0x00,0x80),   XTP_IDS_CLR_DARK_BLUE       ),
	CXTPControlColorSelector::CColorInfo(RGB(0x33,0x33,0x99),   XTP_IDS_CLR_INDIGO          ),
	CXTPControlColorSelector::CColorInfo(RGB(0x33,0x33,0x33),   XTP_IDS_CLR_GRAY80          ),
	CXTPControlColorSelector::CColorInfo(RGB(0x80,0x00,0x00),   XTP_IDS_CLR_DARK_RED        ),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF,0x66,0x00),   XTP_IDS_CLR_ORANGE          ),
	CXTPControlColorSelector::CColorInfo(RGB(0x80,0x80,0x00),   XTP_IDS_CLR_DARK_YELLOW     ),
	CXTPControlColorSelector::CColorInfo(RGB(0x00,0x80,0x00),   XTP_IDS_CLR_GREEN           ),
	CXTPControlColorSelector::CColorInfo(RGB(0x00,0x80,0x80),   XTP_IDS_CLR_TEAL            ),
	CXTPControlColorSelector::CColorInfo(RGB(0x00,0x00,0xFF),   XTP_IDS_CLR_BLUE            ),
	CXTPControlColorSelector::CColorInfo(RGB(0x66,0x66,0x99),   XTP_IDS_CLR_BLUEGRAY        ),
	CXTPControlColorSelector::CColorInfo(RGB(0x80,0x80,0x80),   XTP_IDS_CLR_GRAY50          ),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF,0x00,0x00),   XTP_IDS_CLR_RED             ),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF,0x99,0x00),   XTP_IDS_CLR_LIGHTORANGE     ),
	CXTPControlColorSelector::CColorInfo(RGB(0x99,0xCC,0x00),   XTP_IDS_CLR_LIME            ),
	CXTPControlColorSelector::CColorInfo(RGB(0x33,0x99,0x66),   XTP_IDS_CLR_SEA_GREEN       ),
	CXTPControlColorSelector::CColorInfo(RGB(0x33,0xCC,0xCC),   XTP_IDS_CLR_AQUA            ),
	CXTPControlColorSelector::CColorInfo(RGB(0x33,0x66,0xFF),   XTP_IDS_CLR_LIGHT_BLUE      ),
	CXTPControlColorSelector::CColorInfo(RGB(0x80,0x00,0x80),   XTP_IDS_CLR_VIOLET          ),
	CXTPControlColorSelector::CColorInfo(RGB(0x99,0x99,0x99),   XTP_IDS_CLR_GRAY40          ),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF,0x00,0xFF),   XTP_IDS_CLR_PINK            ),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF,0xCC,0x00),   XTP_IDS_CLR_GOLD            ),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF,0xFF,0x00),   XTP_IDS_CLR_YELLOW          ),
	CXTPControlColorSelector::CColorInfo(RGB(0x00,0xFF,0x00),   XTP_IDS_CLR_BRIGHT_GREEN    ),
	CXTPControlColorSelector::CColorInfo(RGB(0x00,0xFF,0xFF),   XTP_IDS_CLR_TURQUOISE       ),
	CXTPControlColorSelector::CColorInfo(RGB(0x00,0xCC,0xFF),   XTP_IDS_CLR_SKY_BLUE        ),
	CXTPControlColorSelector::CColorInfo(RGB(0x99,0x33,0x66),   XTP_IDS_CLR_PLUM            ),
	CXTPControlColorSelector::CColorInfo(RGB(0xC0,0xC0,0xC0),   XTP_IDS_CLR_GRAY25          ),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF,0x99,0xCC),   XTP_IDS_CLR_ROSE            ),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF,0xCC,0x99),   XTP_IDS_CLR_TAN             ),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF,0xFF,0x99),   XTP_IDS_CLR_LIGHT_YELLOW    ),
	CXTPControlColorSelector::CColorInfo(RGB(0xCC,0xFF,0xCC),   XTP_IDS_CLR_LIGHT_GREEN     ),
	CXTPControlColorSelector::CColorInfo(RGB(0xCC,0xFF,0xFF),   XTP_IDS_CLR_LIGHT_TURQUOISE ),
	CXTPControlColorSelector::CColorInfo(RGB(0x99,0xCC,0xFF),   XTP_IDS_CLR_PALE_BLUE       ),
	CXTPControlColorSelector::CColorInfo(RGB(0xCC,0x99,0xFF),   XTP_IDS_CLR_LAVENDER        ),
	CXTPControlColorSelector::CColorInfo(RGB(0xFF,0xFF,0xFF),   XTP_IDS_CLR_WHITE           ),
};

IMPLEMENT_XTP_CONTROL(CXTPControlColorSelector, CXTPControl)


CXTPControlColorSelector::CXTPControlColorSelector()
{
	m_nSelected = -1;
	m_clr = (COLORREF)-1;
	SetFlags(xtpFlagNoMovable);

	int nCount = sizeof(extendedColors) / sizeof(extendedColors[0]);
	for (int i = 0; i < nCount; i++)
	{
		XTPResourceManager()->LoadString(&extendedColors[i].strTip, extendedColors[i].toolTipID);
	}
	m_nPressed = -1;
}

CSize CXTPControlColorSelector::GetSize(CDC* /*pDC*/)
{

	return CSize (XTP_SELECTOR_WIDTH, XTP_SELECTOR_HEIGHT);
}

CString  CXTPControlColorSelector::GetTooltip(LPPOINT pPoint, LPRECT lpRectTip, int* nHit)
{
	if (pPoint == NULL || lpRectTip == NULL || nHit == NULL)
		return  _T("");

	int nIndex = HitTest(*pPoint);
	if (nIndex != -1)
	{
		*nHit = extendedColors[nIndex].toolTipID;
		*lpRectTip = GetRect(nIndex);
		return extendedColors[nIndex].strTip;
	}
	return _T("");
}

CRect CXTPControlColorSelector::GetRect(int nIndex)
{
	int x = nIndex % 8, y = nIndex / 8;

	return CRect(
		CPoint(
			XTP_SELECTOR_MARGIN + m_rcControl.left + x * XTP_SELECTOR_SIZE,
			XTP_SELECTOR_MARGIN + m_rcControl.top + y * XTP_SELECTOR_SIZE),
		CSize(XTP_SELECTOR_SIZE, XTP_SELECTOR_SIZE));
}

int CXTPControlColorSelector::HitTest(CPoint point)
{
	int nCount = sizeof(extendedColors) / sizeof(extendedColors[0]);

	if (!m_rcControl.PtInRect(point))
		return -1;

	for (int i = 0; i < nCount; i++)
	{
		if (GetRect(i).PtInRect(point))
			return i;
	}

	return -1;
}

void CXTPControlColorSelector::Draw(CDC* pDC)
{
	int nCount = sizeof(extendedColors) / sizeof(extendedColors[0]);

	for (int i = 0; i < nCount; i++)
	{
		CRect rcButton = GetRect(i);

		if (i == m_nSelected || extendedColors[i].clrButton == m_clr)
		{
			BOOL bPressed = GetPressed() && i == m_nSelected;
			GetPaintManager()->DrawControlEntry(pDC, rcButton, m_bEnabled, i == m_nSelected, bPressed, extendedColors[i].clrButton == m_clr, FALSE, GetParent()->GetPosition());
		}


		rcButton.DeflateRect(3, 3);
		if (m_bEnabled)
			pDC->FillSolidRect(rcButton, extendedColors[i].clrButton);

		pDC->Draw3dRect(rcButton, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DSHADOW));

	}
}

void CXTPControlColorSelector::OnMouseMove(CPoint point)
{
	int nHit = HitTest(point);
	if (nHit != -1)
	{
		m_nSelected = nHit;
		m_pParent->Invalidate(FALSE);

	}
}

BOOL CXTPControlColorSelector::OnSetSelected(int bSelected)
{
	if (!bSelected) m_nSelected = -1;

	return CXTPControl::OnSetSelected(bSelected);
}

void CXTPControlColorSelector::OnExecute()
{
	if (m_nPressed != -1)
	{
		m_clr = extendedColors[m_nPressed].clrButton;
	}
	CXTPControl::OnExecute();

}


COLORREF CXTPControlColorSelector::GetColor()
{
	return m_clr;
}
void CXTPControlColorSelector::SetColor(COLORREF clr)
{
	m_clr = clr;
	m_pParent->Invalidate(FALSE);
}


void CXTPControlColorSelector::OnClick(BOOL bKeyboard, CPoint pt)
{
	if (!m_bEnabled) return;

	int nHit = HitTest(pt);
	if (nHit == -1) return;

	m_nPressed = nHit;

	if (!bKeyboard)
	{
		ClickToolBarButton();
	}
	else
	{
		OnExecute();
	}
	m_nPressed = -1;
}
