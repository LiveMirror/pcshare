// XTPTaskPanelPaintManager.cpp : implementation of the CXTPTaskPanelPaintManager class.
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
#include "XTResource.h"
#include "XTPDrawHelpers.h"
#include "XTPWinThemeWrapper.h"
#include "XTPImageManager.h"

#include "XTPTaskPanel.h"
#include "XTPTaskPanelGroup.h"
#include "XTPTaskPanelGroupItem.h"
#include "XTPTaskPanelItems.h"
#include "XTPTaskPanelPaintManager.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define NORMCOLOR(a) a < 0? 0: a > 255? 255: a
#define HIGHLIGHT_OFFICE2003 2

const CRect CXTPTaskPanelPaintManager::rectDefault(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPTaskPanelPaintManager::CXTPTaskPanelPaintManager()
{
	m_bEmulateEplorerTheme = TRUE;
	m_bBoldCaption = TRUE;
	m_bOfficeHighlight = FALSE;
	m_bInvertDragRect = FALSE;
	m_bCaptionScrollButton = FALSE;

	m_bUseStandardItemsFont = TRUE;
	m_bUseStandardCaptionFont = TRUE;

	m_hLib = LoadLibrary(_T("msimg32.dll"));
	m_pAlphaBlend = NULL;

	if (m_hLib)
	{
		m_pAlphaBlend = (AlphaBlendProc) GetProcAddress(m_hLib, "AlphaBlend");
	}

	m_hGripperNormal = m_hGripperSpecial = 0;

	m_rcGroupOuterMargins.SetRect(0, 0, 0, 0);
	m_rcGroupInnerMargins.SetRect(11, 8, 11, 8);
	m_rcItemOuterMargins.SetRect(0, 0, 0, 0);
	m_rcItemInnerMargins.SetRect(2, 2, 2, 2);

	m_rcControlMargins.SetRect(12, 12, 12, 12);
	m_nGroupSpacing = 15;
	m_bEmbossedDisabledText = FALSE;

	ZeroMemory(m_bmpGrippers, sizeof(m_bmpGrippers));

	RefreshMetrics();

}
void CXTPTaskPanelPaintManager::FreeBitmaps()
{
	for (int i = 0; i < 8; i++)
	{
		if (m_bmpGrippers[i])
		{
			DeleteObject(m_bmpGrippers[i]);
			m_bmpGrippers[i] = 0;
		}
	}
}

CFont* CXTPTaskPanelPaintManager::GetItemFont(CXTPTaskPanelItem* pItem)
{
	if (pItem->GetType() == xtpTaskItemTypeGroup)
		return &m_fntCaption;

	return ((CXTPTaskPanelGroupItem*)pItem)->IsBold()? &m_fntIconBold: &m_fntIcon;
}

CXTPTaskPanelPaintManager::~CXTPTaskPanelPaintManager()
{
	if (m_hLib)
		FreeLibrary(m_hLib);

	if (m_hGripperNormal) DestroyIcon(m_hGripperNormal);
	if (m_hGripperSpecial) DestroyIcon(m_hGripperSpecial);
	FreeBitmaps();
}

CPoint CXTPTaskPanelPaintManager::GetHighlightOffset(CXTPTaskPanelGroupItem* pItem)
{
	if (pItem->IsItemSelected() && !m_bOfficeHighlight)
		return CPoint(1, 1);
	return CPoint(0, 0);
}

void CXTPTaskPanelPaintManager::ConvertToHSL(COLORREF clr, double& h, double& s, double& l)
{
	double r = (double)GetRValue(clr)/255;
	double g = (double)GetGValue(clr)/255;
	double b = (double)GetBValue(clr)/255;

	double maxcolor = __max(r, __max(g, b));
	double mincolor = __min(r, __min(g, b));

	l = (maxcolor + mincolor)/2;

	if (maxcolor == mincolor)
	{
		h = 0;
		s = 0;
	}
	else
	{
		if (l < 0.5)
			s = (maxcolor-mincolor)/(maxcolor+mincolor);
		else
			s = (maxcolor-mincolor)/(2.0-maxcolor-mincolor);

		if (r == maxcolor)
			h = (g-b)/(maxcolor-mincolor);
		else if (g == maxcolor)
			h = 2.0+(b-r)/(maxcolor-mincolor);
		else
			h = 4.0+(r-g)/(maxcolor-mincolor);

		h /= 6.0;
		if (h < 0.0) h += 1;
	}
}

double CXTPTaskPanelPaintManager::ConvertHueToRGB(double temp1, double temp2, double temp3)
{
	if( temp3 < 0 )
		temp3 = temp3+1.0;
	if( temp3 > 1 )
		temp3 = temp3-1.0;

	if( 6.0*temp3 < 1 )
		return (temp1+(temp2-temp1)*temp3*6.0);

	else if( 2.0*temp3 < 1 )
		return temp2;

	else if( 3.0*temp3 < 2.0 )
		return (temp1+(temp2-temp1)*((2.0/3.0)-temp3)*6.0);

	return temp1;
}

COLORREF  CXTPTaskPanelPaintManager::ConvertFromHSL(double h, double s, double l)
{
	double r,g,b;
	double temp1, temp2;

	if (s == 0) {
		r = g = b = l;
	}
	else
	{
		if(l < 0.5)
			temp2 = l*(1.0+s);
		else
			temp2 = l+s-l*s;

		temp1 = 2.0*l-temp2;

		r = ConvertHueToRGB(temp1, temp2, h+1.0/3.0);
		g = ConvertHueToRGB(temp1, temp2, h);
		b = ConvertHueToRGB(temp1, temp2, h-1.0/3.0);
	}

	return RGB((BYTE)(r*255),(BYTE)(g*255),(BYTE)(b*255));
}

void CXTPTaskPanelPaintManager::Triangle(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2, COLORREF clr)
{
	CXTPPenDC pen (*pDC, clr);
	CXTPBrushDC brush (*pDC, clr);

	CPoint pts[3];
	pts[0] = pt0;
	pts[1] = pt1;
	pts[2] = pt2;
	pDC->Polygon(pts, 3);
}

void CXTPTaskPanelPaintManager::DrawScrollArrow(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2, BOOL bEnabled)
{
	if (bEnabled)
	{
		Triangle(pDC, pt0, pt1, pt2, 0);
	} else
	{
		CPoint pt(1, 1);
		Triangle(pDC, pt0 + pt, pt1 + pt, pt2 + pt, GetXtremeColor(COLOR_3DHIGHLIGHT));
		Triangle(pDC, pt0, pt1, pt2, GetXtremeColor(COLOR_3DSHADOW));
	}
}


void CXTPTaskPanelPaintManager::DrawScrollButton(CDC* pDC, CXTPTaskPanelScrollButton* pScrollButton)
{
	if (!pScrollButton->IsVisible())
		return;

	CRect rc = pScrollButton->rcButton;

	if (m_bOfficeHighlight && !m_bCaptionScrollButton)
	{
		Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, pScrollButton->bPressed? m_clrHighlightPressed:
			pScrollButton->bHot? m_clrHighlightHot: GetGroupColors(pScrollButton->pGroupScroll->IsSpecialGroup())->clrClient);
	} else
	{
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));

		if (!pScrollButton->bPressed)
			pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
		else
			pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
	}

	CPoint pt = rc.CenterPoint();

	if (!pScrollButton->bScrollUp)
		DrawScrollArrow(pDC, CPoint(pt.x - 5, pt.y - 2), CPoint(pt.x - 1, pt.y + 2), CPoint(pt.x + 3, pt.y - 2), pScrollButton->bEnabled);
	else
		DrawScrollArrow(pDC, CPoint(pt.x - 5, pt.y + 1), CPoint(pt.x - 1, pt.y - 3), CPoint(pt.x + 3, pt.y + 1), pScrollButton->bEnabled);
}


void CXTPTaskPanelPaintManager::CreateGripperIcon(HICON& hIcon, UINT nIDResource, COLORREF clrGripperBack)
{
	if (hIcon)
	{
		DestroyIcon(hIcon);
		hIcon = 0;
	}

	CBitmap bmpGripper;

	double hueBack, satBack, lumBack;
	ConvertToHSL(clrGripperBack, hueBack, satBack, lumBack);


	CBitmap bmp;
	if (bmp.LoadBitmap(nIDResource))
	{
		BITMAP bmpInfo;
		bmp.GetBitmap(&bmpInfo);

		CWindowDC dc(CWnd::GetDesktopWindow());

		if (bmpGripper.CreateCompatibleBitmap(&dc, bmpInfo.bmWidth, m_nCaptionHeight))
		{
			CXTPCompatibleDC dcTemplate(&dc, &bmp);
			CXTPCompatibleDC dcGripper(&dc, &bmpGripper);

			COLORREF clrTemplateBack = dcTemplate.GetPixel(0, 0);

			double hueTrans, satTrans, lumTrans;
			ConvertToHSL(clrTemplateBack, hueTrans, satTrans, lumTrans);

			for (int x = 0; x < bmpInfo.bmWidth; x++)
			for (int y = 0; y < bmpInfo.bmWidth; y++)
			{
				COLORREF clrTemplate = dcTemplate.GetPixel(x, y);

				if (clrTemplate == clrTemplateBack)
				{
					clrTemplate = RGB(0, 0xFF, 0);
				}
				else
				{

					double huePixel, satPixel, lumPixel;
					ConvertToHSL(clrTemplate, huePixel, satPixel, lumPixel);

					double fLumOffset = lumPixel / lumTrans;
					if (lumPixel <= 0.9 || lumBack > 0.9)
					{
						lumPixel = min(1.0, lumBack * fLumOffset);
					}

					clrTemplate = ConvertFromHSL(hueBack, satBack, lumPixel);
				}

				dcGripper.SetPixel(x, y, clrTemplate);
			}
		}


		CImageList il;
		il.Create(bmpInfo.bmWidth, bmpInfo.bmHeight, ILC_COLOR24|ILC_MASK, 0, 1);

		il.Add(&bmpGripper, RGB(0, 0xFF, 0));

		hIcon = il.ExtractIcon(0);
	}



}

void CXTPTaskPanelPaintManager::PremultplyAlpha(HBITMAP& bmp)
{
	CDC dcSrc;
	dcSrc.CreateCompatibleDC(NULL);

	PBYTE pBits = 0;
	PBITMAPINFO pbmi = 0;

	BITMAPINFO  bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	TRY
	{
		if (GetDIBits(dcSrc, bmp, 0, 0, NULL, (BITMAPINFO*)&bmi, DIB_RGB_COLORS) == 0)
			AfxThrowMemoryException();

		if (bmi.bmiHeader.biBitCount != 32)
			return;

		UINT nSize = bmi.bmiHeader.biSizeImage != 0? bmi.bmiHeader.biSizeImage: bmi.bmiHeader.biHeight * ((bmi.bmiHeader.biWidth * 4 + 3) & ~3);
		pBits = (PBYTE)malloc(nSize);
		if (pBits  == NULL)
			AfxThrowMemoryException();

		if ((pbmi = (PBITMAPINFO)malloc(sizeof(BITMAPINFOHEADER)+sizeof(DWORD)*3 )) == NULL)
			AfxThrowMemoryException();

		MEMCPY_S(pbmi, &bmi, sizeof(BITMAPINFOHEADER));

		if (GetDIBits(dcSrc, bmp, 0, bmi.bmiHeader.biHeight, pBits, pbmi, DIB_RGB_COLORS) == NULL)
			AfxThrowMemoryException();

		for (UINT i = 0; i < nSize; i += 4)
		{
			int nAlpha = pBits[i + 3];

			pBits[i + 0] = (BYTE)(pBits[i + 0] * nAlpha / 255);
			pBits[i + 1] = (BYTE)(pBits[i + 1] * nAlpha / 255);
			pBits[i + 2] = (BYTE)(pBits[i + 2] * nAlpha / 255);

		}

		SetDIBits(dcSrc, bmp, 0, bmi.bmiHeader.biHeight, pBits, pbmi, DIB_RGB_COLORS);

	}
	CATCH (CMemoryException, e)
	{
		TRACE(_T("Failed -- Memory exception thrown."));
	}
	END_CATCH

	free(pBits);
	free(pbmi);



}

void CXTPTaskPanelPaintManager::CreateGripperBitmaps()
{
	m_bUseBitmapGrippers = FALSE;

	CXTPWinThemeWrapper theme;

	WCHAR pszThemeFileName[MAX_PATH];
	WCHAR pszColorBuff[MAX_PATH];

	if (XTPColorManager()->GetCurrentSystemTheme() != xtpSystemThemeUnknown)
	{
		CString strShellPath(pszThemeFileName);
		strShellPath = strShellPath.Left(strShellPath.ReverseFind(_T('\\')) + 1);


		strShellPath = strShellPath + _T("shell\\") + CString(pszColorBuff) + _T("\\shellstyle.dll");

		HMODULE hLib = LoadLibraryEx(strShellPath, 0, LOAD_LIBRARY_AS_DATAFILE);
		if (hLib)
		{

			FreeBitmaps();

			m_bUseBitmapGrippers = TRUE;

			for (int i = 0; i < 8; i++)
			{
				m_bmpGrippers[i] = (HBITMAP)LoadImage(hLib, MAKEINTRESOURCE(100 + i), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

				if (m_bmpGrippers[i] && m_bUseBitmapGrippers)
				{
					PremultplyAlpha(m_bmpGrippers[i]);
				}
				else m_bUseBitmapGrippers = FALSE;
			}
			FreeLibrary(hLib);
		}

	}

	if (!m_bUseBitmapGrippers)
	{
		CreateGripperIcon(m_hGripperSpecial, XTP_IDB_TASKPANEL_GRIPPER_SPECIAL, m_groupSpecial.clrHead.clrDark);
		CreateGripperIcon(m_hGripperNormal, XTP_IDB_TASKPANEL_GRIPPER, m_groupNormal.clrHead.clrDark);
	}
}

void CXTPTaskPanelPaintManager::DrawCaptionGripperSigns(CDC* pDC, CPoint pt, COLORREF clr, BOOL bExpanded)
{
	CXTPPenDC  pen(*pDC, clr);

	if (bExpanded)
	{
		pDC->MoveTo(pt.x - 3, pt.y - 2);pDC->LineTo(pt.x, pt.y - 3- 2); pDC->LineTo(pt.x + 4, pt.y + 1- 2);
		pDC->MoveTo(pt.x - 2, pt.y - 2);pDC->LineTo(pt.x, pt.y - 2- 2); pDC->LineTo(pt.x + 3, pt.y + 1- 2);

		pDC->MoveTo(pt.x - 3, pt.y + 2);pDC->LineTo(pt.x, pt.y - 3 + 2); pDC->LineTo(pt.x + 4, pt.y + 1 + 2);
		pDC->MoveTo(pt.x - 2, pt.y + 2);pDC->LineTo(pt.x, pt.y - 2 + 2); pDC->LineTo(pt.x + 3, pt.y + 1 + 2);
	}
	else
	{
		pDC->MoveTo(pt.x - 3, pt.y - 4);pDC->LineTo(pt.x, pt.y + 3- 4 ); pDC->LineTo(pt.x + 4, pt.y - 1- 4);
		pDC->MoveTo(pt.x - 2, pt.y - 4);pDC->LineTo(pt.x, pt.y + 2- 4); pDC->LineTo(pt.x + 3, pt.y - 1- 4);

		pDC->MoveTo(pt.x - 3, pt.y + 0);pDC->LineTo(pt.x, pt.y + 3 + 0); pDC->LineTo(pt.x + 4, pt.y - 1 + 0);
		pDC->MoveTo(pt.x - 2, pt.y + 0);pDC->LineTo(pt.x, pt.y + 2 + 0); pDC->LineTo(pt.x + 3, pt.y - 1 + 0);
	}
}


CRect CXTPTaskPanelPaintManager::DrawCaptionGripperBitmap(CDC* pDC, BOOL bSpecial, BOOL bExpanded, BOOL bHot, CRect rc)
{

	int nTop = max(rc.top, rc.CenterPoint().y - 10);
	CRect rcGripper(rc.right - 3 - 20, nTop, rc.right - 3, nTop + 20);

	if (m_bUseBitmapGrippers)
	{
		int nIndex = bSpecial? 4: 0;
		nIndex += bExpanded? 0: 2;
		nIndex += bHot? 1: 0;

		CXTPCompatibleDC dcGripper(NULL, CBitmap::FromHandle(m_bmpGrippers[nIndex]));

		if (m_pAlphaBlend)
		{
			BITMAP bmpInfo;
			GetObject(m_bmpGrippers[nIndex], sizeof(BITMAP), &bmpInfo);

			BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, 1};
			m_pAlphaBlend(pDC->GetSafeHdc(), rcGripper.left, rcGripper.top, bmpInfo.bmWidth, bmpInfo.bmHeight,  dcGripper, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, bf);
		}

	}
	else
	{

		HICON hIcon = bSpecial? m_hGripperSpecial: m_hGripperNormal;

		pDC->DrawState(rcGripper.TopLeft(), CSize(19, 19), hIcon, DST_ICON, (CBrush*)NULL);

		XTP_TASKPANEL_GROUPCOLORS& groupColors = bSpecial? m_groupSpecial: m_groupNormal;

		DrawCaptionGripperSigns(pDC, CPoint(rcGripper.CenterPoint().x - 1, rcGripper.CenterPoint().y), bHot? groupColors.clrHeadTextHot: groupColors.clrHeadTextNormal, bExpanded);

	}


	rc.right = rcGripper.left;
	return rc;
}

int CXTPTaskPanelPaintManager::GetCaptionGripperWidth(CXTPTaskPanelGroup* pGroup, CRect rc)
{
	if (!pGroup->IsExpandable())
		return 0;

	if (rc.Width() < 20 + 6)
		return 0;

	if (m_eGripper == xtpTaskPanelGripperBitmap)
		return 23;

	if (m_eGripper == xtpTaskPanelGripperPlain)
		return 23;

	if (m_eGripper == xtpTaskPanelGripperTriangle)
		return 19;

	return 0;
}


CRect CXTPTaskPanelPaintManager::DrawCaptionGripper(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	if (!pGroup->IsExpandable())
		return rc;

	if (rc.Width() < 20 + 6)
		return rc;


	if (m_eGripper == xtpTaskPanelGripperBitmap)
	{

		return DrawCaptionGripperBitmap(pDC, pGroup->IsSpecialGroup(), pGroup->IsExpanded(), pGroup->IsItemHot() , rc);
	}


	if (m_eGripper == xtpTaskPanelGripperPlain)
	{

		XTP_TASKPANEL_GROUPCOLORS& groupColors = pGroup->IsSpecialGroup()? m_groupSpecial: m_groupNormal;

		CRect rcGripper(rc.right - 6 - 17, rc.CenterPoint().y - 8, rc.right - 6, rc.CenterPoint().y + 8);

		if (pGroup->IsItemHot())
		{
			pDC->Draw3dRect(rcGripper, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
		}

		DrawCaptionGripperSigns(pDC, rcGripper.CenterPoint(), pGroup->IsItemHot()? groupColors.clrHeadTextHot: groupColors.clrHeadTextNormal, pGroup->IsExpanded());

		rc.right = rcGripper.left;
	}

	if (m_eGripper == xtpTaskPanelGripperTriangle)
	{

		COLORREF clr =  m_groupNormal.clrClientText;
		CPoint pt(rc.right - 6 - 8 - 5, rc.CenterPoint().y + 2);

		if (pGroup->IsExpanded())
			CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x, pt.y - 2), CPoint(pt.x + 10, pt.y - 2), CPoint(pt.x + 5, pt.y + 5 - 2), clr);
		else
			CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x + 3, pt.y - 5), CPoint(pt.x + 3 + 5, pt.y), CPoint(pt.x + 3, pt.y + 5), clr);
		rc.right = pt.x;
	}
	return rc;
}


COLORREF CXTPTaskPanelPaintManager::BlendColor(COLORREF clrFrom, COLORREF clrTo, int nAlpha)
{
	return RGB(
		NORMCOLOR((GetRValue(clrFrom) * nAlpha + GetRValue(clrTo) * (255 - nAlpha)) / 255),
		NORMCOLOR((GetGValue(clrFrom) * nAlpha + GetGValue(clrTo) * (255 - nAlpha)) / 255),
		NORMCOLOR((GetBValue(clrFrom) * nAlpha + GetBValue(clrTo) * (255 - nAlpha)) / 255));
}

void CXTPTaskPanelPaintManager::SetItemsFontIndirect(LOGFONT* pLogFont, BOOL bUseStandardItemsFont /*= FALSE*/)
{
	m_bUseStandardItemsFont = bUseStandardItemsFont;
	if (!pLogFont)
		return;

	m_fntIcon.DeleteObject();
	m_fntIconBold.DeleteObject();
	m_fntIconHot.DeleteObject();
	m_fntIconHotBold.DeleteObject();


	pLogFont->lfWeight = FW_NORMAL;
	m_fntIcon.CreateFontIndirect(pLogFont);

	pLogFont->lfUnderline = TRUE;
	m_fntIconHot.CreateFontIndirect(pLogFont);

	pLogFont->lfUnderline = FALSE;
	pLogFont->lfWeight = FW_BOLD;
	m_fntIconBold.CreateFontIndirect(pLogFont);

	pLogFont->lfUnderline = TRUE;
	m_fntIconHotBold.CreateFontIndirect(pLogFont);
}

void CXTPTaskPanelPaintManager::SetCaptionFontIndirect(LOGFONT* pLogFont, BOOL bUseStandardCaptionFont /*= FALSE*/)
{
	m_bUseStandardCaptionFont = bUseStandardCaptionFont;
	if (!pLogFont)
		return;

	m_fntCaption.DeleteObject();
	m_fntCaption.CreateFontIndirect(pLogFont);

	CWindowDC dc(CWnd::GetDesktopWindow());
	CXTPFontDC fnt(&dc, &m_fntCaption);
	m_nCaptionHeight = max(25, dc.GetTextExtent(_T(" ")).cy);
}

void CXTPTaskPanelPaintManager::RefreshMetrics()
{
	XTPColorManager()->RefreshColors();


	LOGFONT lfIcon;
	::ZeroMemory(&lfIcon, sizeof(LOGFONT));

	VERIFY(::SystemParametersInfo(SPI_GETICONTITLELOGFONT,sizeof(LOGFONT), &lfIcon, 0));

	if (m_bUseStandardItemsFont)
	{
		SetItemsFontIndirect(&lfIcon, TRUE);
	}

	if (m_bUseStandardCaptionFont)
	{
		lfIcon.lfWeight = m_bBoldCaption? FW_BOLD: FW_NORMAL;
		lfIcon.lfUnderline = FALSE;
		SetCaptionFontIndirect(&lfIcon, TRUE);
	}

	m_bOfficeBorder = FALSE;

	m_groupSpecial.dHeadGradientFactor = m_groupNormal.dHeadGradientFactor = 0;
	m_groupSpecial.clrClientBorder.SetStandardValue(RGB(255, 255, 255));
	m_groupNormal.clrClientBorder.SetStandardValue(RGB(255, 255, 255));


	m_clrBackground.SetStandardValue(GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_WINDOW));

	m_groupSpecial.clrHead.SetStandardValue(GetXtremeColor(COLOR_ACTIVECAPTION), GetXtremeColor(COLOR_ACTIVECAPTION));
	m_groupSpecial.clrClientBorder.SetStandardValue(GetXtremeColor(COLOR_ACTIVECAPTION));
	m_groupSpecial.clrHeadTextHot.SetStandardValue(GetXtremeColor(COLOR_CAPTIONTEXT));
	m_groupSpecial.clrHeadTextNormal.SetStandardValue(GetXtremeColor(COLOR_CAPTIONTEXT));
	m_groupSpecial.clrClient.SetStandardValue(GetXtremeColor(COLOR_WINDOW));

	m_groupNormal.clrHead.SetStandardValue(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DFACE));
	m_groupNormal.clrClientBorder.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_groupNormal.clrHeadTextHot.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrHeadTextNormal.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrClient.SetStandardValue(GetXtremeColor(COLOR_WINDOW));

	m_groupSpecial.clrClientText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrClientText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupSpecial.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupSpecial.clrClientLinkHot.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
	m_groupNormal.clrClientLinkHot.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));

	m_bLeftRoundedCorner = m_bRightRoundedCorner = FALSE;

	m_eGripper = xtpTaskPanelGripperPlain;

	if (m_bOfficeHighlight)
	{
		m_clrHighlightHot.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT));
		m_clrHighlightSelected.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
		m_clrHighlightPressed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
		m_clrHighlightBorder.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));

		if (m_bOfficeHighlight == HIGHLIGHT_OFFICE2003)
		{
			XTPCurrentSystemTheme systemTheme = XTPDrawHelpers()->GetCurrentSystemTheme();

			if (systemTheme != xtpSystemThemeUnknown)
			{
				m_clrHighlightHot.SetStandardValue(RGB(255, 238, 194));
				m_clrHighlightPressed.SetStandardValue(RGB(254, 128, 62));
				m_clrHighlightSelected.SetStandardValue(RGB(255, 192, 111));
			}
			switch (systemTheme)
			{
				case xtpSystemThemeBlue: m_clrHighlightBorder.SetStandardValue(RGB(0, 0, 128)); break;
				case xtpSystemThemeOlive: m_clrHighlightBorder.SetStandardValue(RGB(63, 93, 56)); break;
				case xtpSystemThemeSilver: m_clrHighlightBorder.SetStandardValue(RGB(75, 75, 111)); break;
			}
		}
	}
	else
	{
		m_clrHighlightHot.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
		m_clrHighlightBorder.SetStandardValue(RGB(128, 128, 128), RGB(198, 198, 198));
	}
}

void CXTPTaskPanelPaintManager::FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel)
{
	CXTPClientRect rc(pTaskPanel);

	CRect rcFill(rc.left, 0, rc.right, max(rc.bottom, pTaskPanel->GetClientHeight()));

	XTPDrawHelpers()->GradientFill(pDC, rcFill, m_clrBackground, FALSE);
}


CRect CXTPTaskPanelPaintManager::GetItemOuterMargins(CXTPTaskPanelGroupItem* pItem)
{
	CRect rc = pItem->GetMargins();
	return rc == rectDefault? m_rcItemOuterMargins: rc;
}

CRect CXTPTaskPanelPaintManager::GetItemInnerMargins(CXTPTaskPanelGroupItem* /*pItem*/)
{
	return m_rcItemInnerMargins;
}

CRect CXTPTaskPanelPaintManager::GetGroupOuterMargins(CXTPTaskPanelGroup* pGroup)
{
	CRect rc = pGroup->GetOuterMargins();
	return rc == rectDefault? m_rcGroupOuterMargins: rc;
}
CRect CXTPTaskPanelPaintManager::GetGroupInnerMargins(CXTPTaskPanelGroup* pGroup)
{
	CRect rc = pGroup->GetInnerMargins();
	return rc == rectDefault? m_rcGroupInnerMargins: rc;
}

CRect CXTPTaskPanelPaintManager::GetControlMargins(CXTPTaskPanel* pPanel)
{
	CRect rc = pPanel->GetMargins();
	return rc == rectDefault? m_rcControlMargins: rc;
}

int CXTPTaskPanelPaintManager::GetGroupSpacing(CXTPTaskPanel* pPanel)
{
	return pPanel->m_nGroupSpacing == CW_USEDEFAULT? m_nGroupSpacing: pPanel->m_nGroupSpacing;
}


void CXTPTaskPanelPaintManager::Rectangle(CDC* pDC, CRect rc, COLORREF clrPen, COLORREF clrBrush)
{
	pDC->FillSolidRect(rc, clrBrush);
	pDC->Draw3dRect(rc, clrPen, clrPen);
}

void CXTPTaskPanelPaintManager::DrawGroupItemFrame(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc)
{
	if (m_bOfficeHighlight)
	{
		if (pItem->IsItemSelected() && !pItem->IsItemHot() && !pItem->IsItemPressed()) Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightSelected);
		else if (pItem->IsItemSelected() && pItem->IsItemHot() && !pItem->IsItemPressed()) Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightPressed);
		else if (pItem->IsItemHot() && pItem->IsItemPressed()) Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightPressed);
		else if (pItem->IsItemHot() || pItem->IsItemPressed())  Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightHot);

	}
	else
	{
		if (pItem->IsItemSelected() && (!pItem->IsItemHot() || pItem->IsItemPressed()))
			pDC->FillSolidRect(rc, m_clrHighlightHot);

		if (pItem->IsItemSelected() || (pItem->IsItemPressed() && pItem->IsItemHot()))
			pDC->Draw3dRect(rc, m_clrHighlightBorder.clrLight, m_clrHighlightBorder.clrDark);
		else if (pItem->IsItemHot() || pItem->IsItemPressed())
			pDC->Draw3dRect(rc, m_clrHighlightBorder.clrDark, m_clrHighlightBorder.clrLight);
	}
}

void CXTPTaskPanelPaintManager::DrawDragArrow(CDC* pDC, CXTPTaskPanelItem* pItem, CRect rc, BOOL bAfter)
{
	if (m_bInvertDragRect)
	{
		pDC->InvertRect(rc);
	}
	else
	{
		if (!pItem->IsGroup() && pItem->GetItemGroup()->GetItemLayout() != xtpTaskItemLayoutImages)
		{
			CRect rcMargin = GetControlMargins(pItem->GetTaskPanel());
			rcMargin.InflateRect(GetGroupOuterMargins(pItem->GetItemGroup()));

			CXTPClientRect rcControl(pItem->GetTaskPanel());
			rc.left = rcControl.left + rcMargin.left + 2;
			rc.right = rcControl.right - rcMargin.right - 2;
		}

		int y = bAfter? rc.bottom - 1: rc.top - 1;

		COLORREF clr = RGB(0 /*xFF*/, 0, 0);

		pDC->FillSolidRect(rc.left, y, rc.Width(), 1, clr);
		pDC->FillSolidRect(rc.left + 0, y - 3 , 1, 7, clr);
		pDC->FillSolidRect(rc.left + 1, y - 2 , 1, 5, clr);
		pDC->FillSolidRect(rc.left + 2, y - 1 , 1, 3, clr);
		pDC->FillSolidRect(rc.right - 1, y - 3 , 1, 7, clr);
		pDC->FillSolidRect(rc.right - 2, y - 2 , 1, 5, clr);
		pDC->FillSolidRect(rc.right - 3, y - 1 , 1, 3, clr);

	}
}

CRect CXTPTaskPanelPaintManager::DrawGroupItem(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc, BOOL bDraw)
{
	XTPTaskPanelItemLayout itemLayout = pItem->GetItemGroup()->GetItemLayout();

	CRect rcItem = itemLayout == xtpTaskItemLayoutImagesWithTextBelow? DrawGroupItemImageWithTextBelow(pDC, pItem, rc, bDraw):
		itemLayout == xtpTaskItemLayoutImages? DrawGroupItemImage(pDC, pItem, rc, bDraw):  DrawGroupItemImageWithText(pDC, pItem, rc, bDraw);

	if (bDraw && pItem->IsItemDragOver() && !pItem->IsItemDragging())
	{
		DrawDragArrow(pDC, pItem, rc);
	}
	if (bDraw && pItem->GetItemGroup()->IsItemDragOver() &&
		pItem->GetParentItems()->GetAt(pItem->GetParentItems()->GetCount() - 1) == pItem)
	{
		DrawDragArrow(pDC, pItem, rc, TRUE);
	}

	return rcItem;
}

CRect CXTPTaskPanelPaintManager::DrawGroupItemImageWithText(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc, BOOL bDraw)
{
	CXTPTaskPanelGroup* pGroup  = pItem->GetItemGroup();
	ASSERT(pGroup);

	XTPTaskPanelHotTrackStyle hotTrackStyle = pItem->GetTaskPanel()->GetHotTrackStyle();
	CSize szIcon = pGroup->GetItemIconSize();

	CXTPFontDC font(pDC, (pItem->IsItemHot() && (hotTrackStyle == xtpTaskPanelHighlightText)) ? (pItem->IsBold()? &m_fntIconHotBold: &m_fntIconHot): pItem->IsBold()? &m_fntIconBold: &m_fntIcon);

	CXTPImageManagerIcon* pImage = pItem->GetImage(szIcon.cx);
	BOOL bDrawIcon = pItem->GetType() == xtpTaskItemTypeLink && pImage;
	CRect rcInnerMargins = GetItemInnerMargins(pItem);

	if (bDraw)
	{
		XTP_TASKPANEL_GROUPCOLORS& groupColors = pGroup->IsSpecialGroup()? m_groupSpecial: m_groupNormal;

		if (hotTrackStyle == xtpTaskPanelHighlightItem)
		{
			DrawGroupItemFrame(pDC, pItem, rc);
		}
		rc.DeflateRect(rcInnerMargins.left, rcInnerMargins.top, rcInnerMargins.right, rcInnerMargins.bottom);

		CRect rcText(rc);

		if (hotTrackStyle == xtpTaskPanelHighlightItem)
			rcText.OffsetRect(GetHighlightOffset(pItem));

		if (pItem->GetType() == xtpTaskItemTypeLink)
		{
			if (bDrawIcon)
			{
				if (hotTrackStyle == xtpTaskPanelHighlightImage)
				{
					CRect rcImage(rc.left - rcInnerMargins.left, rc.top - rcInnerMargins.top, rc.left + szIcon.cx + 2, rc.bottom + rcInnerMargins.bottom);
					DrawGroupItemFrame(pDC, pItem, rcImage);
				}
				CPoint ptIcon(rc.TopLeft());

				if (hotTrackStyle == xtpTaskPanelHighlightImage || hotTrackStyle == xtpTaskPanelHighlightItem)
					ptIcon.Offset(GetHighlightOffset(pItem));

				DrawItemImage(pDC, pItem, ptIcon, pImage, szIcon);
			}

			rcText.left += szIcon.cx + 7;
		}

		if (rcText.Width() > 0)
		{
			DrawItemCaption(pDC, pItem, rcText, DT_WORDBREAK|DT_LEFT|DT_NOPREFIX);
		}

		if (pItem->IsItemFocused() && pItem->GetTaskPanel()->IsDrawFocusRect())
		{
			pDC->SetTextColor(groupColors.clrClientText);
			pDC->DrawFocusRect(rc);
		}
		return 0;
	}
	else
	{
		CRect rcText(rc);
		rcText.bottom = rcText.top;
		rcText.left += rcInnerMargins.left;

		if (pItem->GetType() == xtpTaskItemTypeLink)
		rcText.left += szIcon.cx + 7;

		if (rcText.Width() > 0 )
		{
			pDC->DrawText(pItem->GetCaption(), rcText, DT_WORDBREAK|DT_LEFT|DT_CALCRECT|DT_NOPREFIX);
		}
		int nImageHeight = bDrawIcon? szIcon.cy: 0;

		return CRect(rc.left, rc.top, hotTrackStyle == xtpTaskPanelHighlightItem? rc.right: (rcText.right + rcInnerMargins.right),
			rc.top + max(nImageHeight, rcText.Height()) + rcInnerMargins.top + rcInnerMargins.bottom);

	}
}


CRect CXTPTaskPanelPaintManager::DrawGroupItemImageWithTextBelow(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc, BOOL bDraw)
{
	CXTPTaskPanelGroup* pGroup  = pItem->GetItemGroup();
	ASSERT(pGroup);

	XTPTaskPanelHotTrackStyle hotTrackStyle = pItem->GetTaskPanel()->GetHotTrackStyle();
	CSize szIcon = pGroup->GetItemIconSize();

	CXTPFontDC font(pDC, (pItem->IsItemHot() && (hotTrackStyle == xtpTaskPanelHighlightText)) ? (pItem->IsBold()? &m_fntIconHotBold: &m_fntIconHot): pItem->IsBold()? &m_fntIconBold: &m_fntIcon);

	CXTPImageManagerIcon* pImage = pItem->GetImage(szIcon.cx);
	BOOL bDrawIcon = pItem->GetType() == xtpTaskItemTypeLink && pImage;
	CRect rcInnerMargins = GetItemInnerMargins(pItem);

	if (bDraw)
	{
		XTP_TASKPANEL_GROUPCOLORS& groupColors = pGroup->IsSpecialGroup()? m_groupSpecial: m_groupNormal;

		if (hotTrackStyle == xtpTaskPanelHighlightItem)
		{
			DrawGroupItemFrame(pDC, pItem, rc);
		}
		rc.DeflateRect(rcInnerMargins.left, rcInnerMargins.top, rcInnerMargins.right, rcInnerMargins.bottom);

		CRect rcText(rc);
		if (hotTrackStyle == xtpTaskPanelHighlightItem)
			rcText.OffsetRect(GetHighlightOffset(pItem));

		if (pItem->GetType() == xtpTaskItemTypeLink)
		{
			if (bDrawIcon)
			{
				CPoint ptIcon((rc.right + rc.left + 1 - szIcon.cx) /2, rc.top);

				if (hotTrackStyle == xtpTaskPanelHighlightImage)
				{
					CRect rcImage(ptIcon.x - 2,  rc.top - 2, ptIcon.x + szIcon.cx + 2, rc.top + szIcon.cy + 2);
					DrawGroupItemFrame(pDC, pItem, rcImage);
				}

				if (hotTrackStyle == xtpTaskPanelHighlightImage || hotTrackStyle == xtpTaskPanelHighlightItem)
					ptIcon.Offset(GetHighlightOffset(pItem));

				DrawItemImage(pDC, pItem, ptIcon, pImage, szIcon);
			}

			rcText.top += szIcon.cy + 3;
		}

		if (rcText.Width() > 0)
		{
			DrawItemCaption(pDC, pItem, rcText, DT_WORDBREAK|DT_CENTER|DT_NOPREFIX);
		}

		if (pItem->IsItemFocused()  && pItem->GetTaskPanel()->IsDrawFocusRect())
		{
			pDC->SetTextColor(groupColors.clrClientText);
			pDC->DrawFocusRect(rc);
		}
		return 0;
	}
	else
	{
		CRect rcText(rc);
		rcText.bottom = rcText.top;
		rcText.left += rcInnerMargins.left;

		if (rcText.Width() > 0 )
		{
			pDC->DrawText(pItem->GetCaption(), rcText, DT_WORDBREAK|DT_LEFT|DT_CALCRECT|DT_NOPREFIX);
		}
		int nHeight = rcText.Height() + (bDrawIcon? (szIcon.cy + 3): 0);
		int nWidth = max(bDrawIcon? (szIcon.cx + 4): 0, rcText.Width() ) + rcInnerMargins.right + + rcInnerMargins.left;

		if (hotTrackStyle != xtpTaskPanelHighlightItem)
			return CRect(rc.CenterPoint().x - nWidth + nWidth /2, rc.top, rc.CenterPoint().x + nWidth /2, rc.top + nHeight + rcInnerMargins.top + rcInnerMargins.bottom);

		return CRect(rc.left, rc.top, rc.right, rc.top + nHeight + rcInnerMargins.top + rcInnerMargins.bottom);

	}
}


CRect CXTPTaskPanelPaintManager::DrawGroupItemImage(CDC* pDC, CXTPTaskPanelGroupItem* pItem, CRect rc, BOOL bDraw)
{
	CXTPTaskPanelGroup* pGroup  = pItem->GetItemGroup();
	ASSERT(pGroup);

	XTPTaskPanelHotTrackStyle hotTrackStyle = pItem->GetTaskPanel()->GetHotTrackStyle();
	CSize szIcon = pGroup->GetItemIconSize();

	CXTPImageManagerIcon* pImage = pItem->GetImage(szIcon.cx);
	CRect rcInnerMargins = GetItemInnerMargins(pItem);
	BOOL bDrawIcon = pItem->GetType() == xtpTaskItemTypeLink && pImage;

	if (bDraw)
	{
		CPoint ptIcon((rc.right + rc.left - szIcon.cx) /2, (rc.top + rc.bottom - szIcon.cy) /2);
		if (hotTrackStyle != xtpTaskPanelHighlightNone)
		{
			DrawGroupItemFrame(pDC, pItem, rc);
			ptIcon.Offset(GetHighlightOffset(pItem));
		}
		rc.DeflateRect(rcInnerMargins.left, rcInnerMargins.top, rcInnerMargins.right, rcInnerMargins.bottom);

		if (bDrawIcon)
		{
			DrawItemImage(pDC, pItem, ptIcon, pImage, szIcon);
		}

		if (pItem->IsItemFocused() && pItem->GetTaskPanel()->IsDrawFocusRect())
		{
			XTP_TASKPANEL_GROUPCOLORS& groupColors = pGroup->IsSpecialGroup()? m_groupSpecial: m_groupNormal;
			pDC->SetTextColor(groupColors.clrClientText);
			pDC->DrawFocusRect(rc);
		}
		return 0;
	}
	else
	{
		int nHeight = szIcon.cy + rcInnerMargins.top + rcInnerMargins.bottom;
		int nWidth = szIcon.cx + rcInnerMargins.right + rcInnerMargins.left;

		return CRect(rc.CenterPoint().x - nWidth /2, rc.top, rc.CenterPoint().x - nWidth /2 + nWidth, rc.top + nHeight);
	}
}



void CXTPTaskPanelPaintManager::DrawGroupClientItems(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	int nOffset = rc.top - pGroup->GetScrollOffsetPos();

	for (int i = pGroup->GetOffsetItem(); i < pGroup->GetItems()->GetCount(); i++)
	{
		CXTPTaskPanelGroupItem* pItem = pGroup->GetAt(i);

		CRect rcItem = pItem->GetItemRect();
		rcItem.OffsetRect(0, nOffset);

		pItem->OnDrawItem(pDC, rcItem);
	}
}

void CXTPTaskPanelPaintManager::DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	XTP_TASKPANEL_GROUPCOLORS& groupColors = pGroup->IsSpecialGroup()? m_groupSpecial: m_groupNormal;

	pDC->FillSolidRect(rc, groupColors.clrClient);
	pGroup->OnFillClient(pDC, rc);

	if (pGroup->IsExpanded() || pGroup->IsExpanding())
	{
		CXTPPenDC pen(*pDC, groupColors.clrClientBorder);
		pDC->MoveTo(rc.TopLeft());
		pDC->LineTo(rc.left, rc.bottom - 1);
		if (m_bOfficeBorder) pDC->LineTo(rc.right, rc.bottom - 1); else pDC->LineTo(rc.right - 1, rc.bottom - 1);
		if (!m_bOfficeBorder) pDC->LineTo(rc.right - 1, rc.top - 1); else pDC->MoveTo(rc.right - 1, rc.top);
		if (m_bOfficeBorder) pDC->LineTo(rc.TopLeft());

		DrawGroupClientItems(pDC, pGroup, rc);
	}

}

void CXTPTaskPanelPaintManager::DrawGroupClient(CDC* pDC, CXTPTaskPanelGroup* pGroup)
{
	CRect rc = pGroup->GetClientRect();
	if (rc.IsRectEmpty())
		return;

	int nOffset = pGroup->IsExpanding()? pGroup->GetExpandedClientHeight() - rc.Height(): 0;

	CRect rcExpaded(rc);
	if (nOffset)
		rcExpaded.bottom = rcExpaded.top + pGroup->GetExpandedClientHeight();

	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rcExpaded.Width(), rcExpaded.Height());

	CGdiObject* pOldBitmap = dc.SelectObject(&bmp);

	dc.SetWindowOrg(rc.left, rc.top) ;

	int nAlpha = pGroup->IsExpanding()? (255 * rc.Height()) / pGroup->GetExpandedClientHeight(): 255;

	DrawGroupClientFace(&dc, pGroup, rcExpaded);

	dc.SetWindowOrg(0, 0);

	if (m_pAlphaBlend && nAlpha != 255)
	{
		BLENDFUNCTION bf = {AC_SRC_OVER, 0, (BYTE)nAlpha, 0};
		m_pAlphaBlend(pDC->GetSafeHdc(), rc.left, rc.top, rc.Width(), rc.Height(),  dc, 0, nOffset, rc.Width(), rcExpaded.Height() - nOffset, bf);
	}
	else
	{
		pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &dc, 0, nOffset, SRCCOPY);
	}

	dc.SelectObject(pOldBitmap);

}

void CXTPTaskPanelPaintManager::DrawItemCaption(CDC* pDC, CXTPTaskPanelItem* pItem, CRect rcText, UINT nFormat)
{
	if (!pItem->IsGroup())
	{
		XTPTaskPanelHotTrackStyle hotTrackStyle = pItem->GetTaskPanel()->GetHotTrackStyle();

		CXTPTaskPanelGroup* pGroup  = pItem->GetItemGroup();
		XTP_TASKPANEL_GROUPCOLORS& groupColors = pGroup->IsSpecialGroup()? m_groupSpecial: m_groupNormal;

		pDC->SetBkMode(TRANSPARENT);

		if (m_bEmbossedDisabledText && !pItem->GetEnabled())
		{
			pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));
			pDC->DrawText(pItem->GetCaption(), rcText + CPoint(1, 1), nFormat);
		}

		pDC->SetTextColor(!pItem->GetEnabled()? GetXtremeColor(COLOR_GRAYTEXT): pItem->GetType() == xtpTaskItemTypeLink?
			(pItem->IsItemHot() && (hotTrackStyle == xtpTaskPanelHighlightText)) ? groupColors.clrClientLinkHot: groupColors.clrClientLink: groupColors.clrClientText);
	}

	pDC->DrawText(pItem->GetCaption(), rcText, nFormat);
	pItem->SetTextRect(pDC, rcText, nFormat);
}

void CXTPTaskPanelPaintManager::DrawItemImage(CDC* pDC, CXTPTaskPanelItem* pItem, CPoint ptIcon, CXTPImageManagerIcon* pImage, CSize szIcon)
{
	BOOL bHighlight = pItem->IsItemHot() || pItem->IsItemPressed();
	BOOL bChecked = (!pItem->IsGroup() && ((CXTPTaskPanelGroupItem*)pItem)->IsItemSelected());

	pImage->Draw(pDC, ptIcon, !pItem->GetEnabled()? pImage->GetDisabledIcon(): bHighlight? pImage->GetHotIcon(): bChecked? pImage->GetCheckedIcon(): pImage->GetIcon(), szIcon);
}

int CXTPTaskPanelPaintManager::DrawGroupCaption(CDC* pDC, CXTPTaskPanelGroup* pGroup, BOOL bDraw)
{
	if (bDraw)
	{
		CRect rc = pGroup->GetCaptionRect();

		if (rc.IsRectEmpty())
			return 0;

		XTP_TASKPANEL_GROUPCOLORS& groupColors = pGroup->IsSpecialGroup()? m_groupSpecial: m_groupNormal;

		if (pGroup->IsCaptionVisible())
		{

			if (m_bLeftRoundedCorner)
			{
				pDC->ExcludeClipRect(rc.left, rc.top, rc.left + 2, rc.top + 1);
				pDC->ExcludeClipRect(rc.left, rc.top + 1, rc.left + 1, rc.top + 2);
			}

			if (m_bRightRoundedCorner)
			{
				pDC->ExcludeClipRect(rc.right, rc.top, rc.right - 2, rc.top + 1);
				pDC->ExcludeClipRect(rc.right, rc.top + 1, rc.right - 1, rc.top + 2);
			}

			pDC->FillSolidRect(rc, groupColors.clrHead.clrLight);
			CRect rcGradient(rc);
			rcGradient.left += int (rc.Width() * groupColors.dHeadGradientFactor);
			XTPDrawHelpers()->GradientFill(pDC, rcGradient, groupColors.clrHead, TRUE);

			if (pGroup->IsItemFocused() && pGroup->GetTaskPanel()->IsDrawFocusRect())
			{
				pDC->SetTextColor(groupColors.clrHeadTextNormal);
				CRect rcFocus(rc);
				rcFocus.DeflateRect(2, 2);
				pDC->DrawFocusRect(rcFocus);
			}


			CXTPFontDC fnt(pDC, &m_fntCaption);

			CRect rcText = DrawCaptionGripper(pDC, pGroup, rc);

			CSize szIcon = pGroup->GetTaskPanel()->GetGroupIconSize();
			CXTPImageManagerIcon* pImage = pGroup->GetImage(szIcon.cx);

			if (rcText.Width() > szIcon.cx && pImage)
			{
				szIcon.cy = pImage->GetHeight() * szIcon.cx / pImage->GetWidth();
				DrawItemImage(pDC, pGroup, CPoint(rcText.left, rcText.bottom - szIcon.cy), pImage, szIcon);

				rcText.left += szIcon.cx - 12;
			}

			rcText.left += 13;
			rcText.bottom += 2;

			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(!pGroup->GetEnabled()? GetXtremeColor(COLOR_GRAYTEXT):
				pGroup->IsItemHot()? groupColors.clrHeadTextHot: groupColors.clrHeadTextNormal );

			DrawItemCaption(pDC, pGroup, rcText, DT_SINGLELINE|DT_LEFT|DT_END_ELLIPSIS|DT_VCENTER|DT_NOPREFIX);

		}
		else
		{
			pDC->FillSolidRect(rc, groupColors.clrClientBorder);
		}
		if (pGroup->IsItemDragOver() && !(pGroup->IsExpanded() && pGroup->GetItemCount() > 0))
		{
			DrawDragArrow(pDC, pGroup, rc, TRUE);
		}
	}

	return pGroup->IsCaptionVisible()? m_nCaptionHeight: m_bOfficeBorder? 0: 1;
}







//////////////////////////////////////////////////////////////////////////
/// CXTPTaskPanelExplorerTheme

CXTPTaskPanelExplorerTheme::CXTPTaskPanelExplorerTheme()
{
	m_bOfficeHighlight = HIGHLIGHT_OFFICE2003;
	RefreshMetrics();
}

void CXTPTaskPanelExplorerTheme::RefreshMetrics()
{
	CXTPTaskPanelPaintManager::RefreshMetrics();

	XTPCurrentSystemTheme systemTheme = XTPDrawHelpers()->GetCurrentSystemTheme();

	m_bExplorerTheme = FALSE;

	if (!m_bEmulateEplorerTheme && (systemTheme == xtpSystemThemeUnknown))
	{
		return ;
	}
	m_bExplorerTheme = TRUE;

	m_eGripper = xtpTaskPanelGripperBitmap;

	m_bRightRoundedCorner = m_bLeftRoundedCorner = TRUE;
	m_groupNormal.clrClientBorder.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_groupSpecial.clrClientBorder.SetStandardValue(GetXtremeColor(COLOR_WINDOW));

	switch (systemTheme)
	{
		case xtpSystemThemeBlue:
		{
			m_clrBackground.SetStandardValue(RGB(123, 162, 231), RGB(99, 117, 214));

			m_groupSpecial.clrHead.SetStandardValue(RGB(0, 73, 181), RGB(41, 93, 206));
			m_groupSpecial.clrHeadTextNormal.SetStandardValue(RGB(255, 255, 255));
			m_groupSpecial.clrHeadTextHot.SetStandardValue(RGB(66, 142, 255));
			m_groupSpecial.clrClient.SetStandardValue(RGB(239, 243, 255));

			m_groupNormal.clrHead.SetStandardValue(RGB(255, 255, 255), RGB(198, 211, 247));
			m_groupNormal.dHeadGradientFactor = 0.5;
			m_groupNormal.clrHeadTextNormal.SetStandardValue(RGB(33, 93, 198));
			m_groupNormal.clrHeadTextHot.SetStandardValue(RGB(66, 142, 255));
			m_groupNormal.clrClient.SetStandardValue(RGB(214, 223, 247));

			m_groupSpecial.clrClientLink.SetStandardValue(RGB(33, 93, 198));
			m_groupNormal.clrClientLink.SetStandardValue(RGB(33, 93, 198));
			m_groupSpecial.clrClientLinkHot.SetStandardValue(RGB(66, 142, 255));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(66, 142, 255));

			break;

		}
		case xtpSystemThemeOlive:
		{
			m_clrBackground.SetStandardValue(RGB(204, 217, 173), RGB(165, 189, 132));

			m_groupSpecial.clrHead.SetStandardValue(RGB(119, 140, 64), RGB(150, 168, 103));
			m_groupSpecial.clrHeadTextNormal.SetStandardValue(RGB(255, 255, 255));
			m_groupSpecial.clrHeadTextHot.SetStandardValue(RGB(224, 231, 184));
			m_groupSpecial.clrClient.SetStandardValue(RGB(246, 246, 236));

			m_groupNormal.clrHead.SetStandardValue(RGB(255, 252, 236), RGB(224, 231, 184));
			m_groupNormal.clrHeadTextNormal.SetStandardValue(RGB(86, 102, 45));
			m_groupNormal.clrHeadTextHot.SetStandardValue(RGB(114, 146, 29));
			m_groupNormal.clrClient.SetStandardValue(RGB(246, 246, 236));

			m_groupSpecial.clrClientLink.SetStandardValue(RGB(86, 102, 45));
			m_groupNormal.clrClientLink.SetStandardValue(RGB(86, 102, 45));
			m_groupSpecial.clrClientLinkHot.SetStandardValue(RGB(114, 146, 29));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(114, 146, 29));

			break;
		}
		case xtpSystemThemeSilver:
		{
			m_clrBackground.SetStandardValue(RGB(196, 200, 212), RGB(177, 179, 200));

			m_groupSpecial.clrHead.SetStandardValue(RGB(119, 119, 146), RGB(180, 182, 199));
			m_groupSpecial.clrHeadTextNormal.SetStandardValue(RGB(255, 255, 255));
			m_groupSpecial.clrHeadTextHot.SetStandardValue(RGB(230, 230, 230));
			m_groupSpecial.clrClient.SetStandardValue(RGB(240, 241, 245));

			m_groupNormal.clrHead.SetStandardValue(RGB(255, 255, 255), RGB(214, 215, 224));
			m_groupNormal.clrHeadTextNormal.SetStandardValue(RGB(63, 61, 61));
			m_groupNormal.clrHeadTextHot.SetStandardValue(RGB(126, 124, 124));
			m_groupNormal.clrClient.SetStandardValue(RGB(240, 241, 245));

			m_groupSpecial.clrClientLink.SetStandardValue(RGB(63, 61, 61));
			m_groupNormal.clrClientLink.SetStandardValue(RGB(63, 61, 61));
			m_groupSpecial.clrClientLinkHot.SetStandardValue(RGB(126, 124, 124));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(126, 124, 124));

			break;
		}
		default:
		{
			m_clrBackground.SetStandardValue(
				BlendColor(GetXtremeColor(COLOR_INACTIVECAPTION),GetXtremeColor(COLOR_WINDOW), 128),
				BlendColor(GetXtremeColor(COLOR_ACTIVECAPTION), GetXtremeColor(COLOR_WINDOW), 192));

			COLORREF clrClient = BlendColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 24);

			m_groupSpecial.clrHead.SetStandardValue(GetXtremeColor(COLOR_ACTIVECAPTION), BlendColor(GetXtremeColor(COLOR_ACTIVECAPTION), GetXtremeColor(COLOR_WINDOW), 92));

			m_groupSpecial.clrHeadTextNormal.SetStandardValue(GetXtremeColor(COLOR_CAPTIONTEXT));
			m_groupSpecial.clrHeadTextHot.SetStandardValue(BlendColor(GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_HIGHLIGHT), 128));
			m_groupSpecial.clrClient.SetStandardValue(clrClient);


			m_groupNormal.clrHead.SetStandardValue(GetXtremeColor(COLOR_WINDOW), BlendColor(GetXtremeColor(COLOR_INACTIVECAPTION), GetXtremeColor(COLOR_WINDOW), 92));

			m_groupNormal.clrHeadTextNormal.SetStandardValue(BlendColor(GetXtremeColor(COLOR_HIGHLIGHT), GetXtremeColor(COLOR_BTNTEXT), 192));
			m_groupNormal.clrHeadTextHot.SetStandardValue(BlendColor(GetXtremeColor(COLOR_HIGHLIGHT), GetXtremeColor(COLOR_BTNTEXT), 240));
			m_groupNormal.clrClient.SetStandardValue(BlendColor(clrClient, GetXtremeColor(COLOR_ACTIVECAPTION), 230));

			m_groupSpecial.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
			m_groupNormal.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
			m_groupSpecial.clrClientLinkHot.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
			m_groupNormal.clrClientLinkHot.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));

		}
	}

	CreateGripperBitmaps();

}


//////////////////////////////////////////////////////////////////////////
/// CXTPTaskPanelOffice2003Theme

CXTPTaskPanelOffice2003Theme::CXTPTaskPanelOffice2003Theme()
{
	m_bOfficeHighlight = HIGHLIGHT_OFFICE2003;
	m_rcControlMargins = CRect(8, 8, 0, 8);
	m_nGroupSpacing = 15;

	RefreshMetrics();
}

void CXTPTaskPanelOffice2003Theme::RefreshMetrics()
{
	CXTPTaskPanelPaintManager::RefreshMetrics();

	m_eGripper = xtpTaskPanelGripperNone;

	if (m_bUseStandardCaptionFont)
	{
		m_fntCaption.DeleteObject();

		LOGFONT lfIcon;
		VERIFY(::SystemParametersInfo(SPI_GETICONTITLELOGFONT,sizeof(LOGFONT), &lfIcon, 0));

		lfIcon.lfUnderline = FALSE;
		lfIcon.lfWeight = FW_BOLD;
		lfIcon.lfHeight = 18;

		CString strOfficeFont(_T("Tahoma"));
		if (CXTPDrawHelpers::FontExists(strOfficeFont))
		{
			STRCPY_S(lfIcon.lfFaceName, LF_FACESIZE, strOfficeFont);
		}
		m_fntCaption.CreateFontIndirect(&lfIcon);

		CWindowDC dc(CWnd::GetDesktopWindow());
		CXTPFontDC fnt(&dc, &m_fntCaption);
		m_nCaptionHeight = max(20, dc.GetTextExtent(_T(" ")).cy);
	}


	XTPCurrentSystemTheme systemTheme = XTPDrawHelpers()->GetCurrentSystemTheme();

	switch (systemTheme)
	{
		case xtpSystemThemeBlue:

			m_clrBackground.SetStandardValue(RGB(221, 236, 254), RGB(74, 122, 201));
			m_groupNormal.clrHead.SetStandardValue(RGB(196, 219, 249), RGB(101, 143, 224));
			m_groupNormal.clrHeadTextHot.SetStandardValue(RGB(0, 45, 134));
			m_groupNormal.clrHeadTextNormal.SetStandardValue(RGB(0, 45, 134));
			m_groupNormal.clrClient.SetStandardValue(RGB(221, 236, 254));
			m_groupNormal.clrClientLink.SetStandardValue(RGB(0, 61, 178));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(0, 61, 178));
			break;

		case xtpSystemThemeOlive:

			m_clrBackground.SetStandardValue(RGB(243, 242, 231), RGB(190, 198, 152));
			m_groupNormal.clrHead.SetStandardValue(RGB(210, 223, 174), RGB(161, 176, 128));
			m_groupNormal.clrHeadTextHot.SetStandardValue(RGB(90, 107, 70));
			m_groupNormal.clrHeadTextNormal.SetStandardValue(RGB(90, 107, 70));
			m_groupNormal.clrClient.SetStandardValue(RGB(243, 242, 231));
			m_groupNormal.clrClientLink.SetStandardValue(RGB(0, 61, 178));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(0, 61, 178));
			break;

		case xtpSystemThemeSilver:

			m_clrBackground.SetStandardValue(RGB(238, 238, 244), RGB(177, 176, 195));
			m_groupNormal.clrHead.SetStandardValue(RGB(207, 207, 222), RGB(169, 168, 191));
			m_groupNormal.clrHeadTextHot.SetStandardValue(RGB(92, 91, 121));
			m_groupNormal.clrHeadTextNormal.SetStandardValue(RGB(92, 91, 121));
			m_groupNormal.clrClient.SetStandardValue(RGB(238, 238, 244));
			m_groupNormal.clrClientLink.SetStandardValue(RGB(0, 61, 178));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(0, 61, 178));
			break;

		default:
			m_clrBackground.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 50), GetXtremeColor(COLOR_3DFACE));

			m_groupNormal.clrHead.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 39),
				XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 70));

			m_groupNormal.clrHeadTextHot.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
			m_groupNormal.clrHeadTextNormal.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
			m_groupNormal.clrClient.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 49));

			m_groupNormal.clrClientLink.SetStandardValue(RGB(0, 0, 0xFF));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(0, 0, 0xFF));
	}
	m_groupNormal.clrClientBorder.SetStandardValue(GetXtremeColor(COLOR_WINDOW));



	m_groupSpecial = m_groupNormal;


	m_bLeftRoundedCorner = TRUE;
	m_bRightRoundedCorner = FALSE;
	m_bOfficeBorder = TRUE;

}

//////////////////////////////////////////////////////////////////////////
/// CXTPTaskPanelPaintManagerPlain

CXTPTaskPanelPaintManagerPlain::CXTPTaskPanelPaintManagerPlain()
{
	m_bRoundedFrame = TRUE;
	m_bOfficeCaption = FALSE;

	RefreshMetrics();
}

void CXTPTaskPanelPaintManagerPlain::RefreshMetrics()
{
	CXTPTaskPanelPaintManager::RefreshMetrics();

	m_eGripper = xtpTaskPanelGripperTriangle;

	m_clrBackground.SetStandardValue(GetXtremeColor(COLOR_BACKGROUND),GetXtremeColor(COLOR_BACKGROUND));
	m_groupNormal.clrClient.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_groupNormal.clrClientBorder.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrClientLinkHot.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
}

void CXTPTaskPanelPaintManagerPlain::DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	pDC->FillSolidRect(rc, m_groupNormal.clrClient);
	pGroup->OnFillClient(pDC, rc);

	if (pGroup->IsExpanded() || pGroup->IsExpanding())
	DrawGroupClientItems(pDC, pGroup, rc);

}

void CXTPTaskPanelPaintManagerPlain::FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel)
{
	CXTPClientRect rc(pTaskPanel);

	CRect rcFill(rc.left, 0, rc.right, max(rc.bottom, pTaskPanel->GetClientHeight()));

	XTPDrawHelpers()->GradientFill(pDC, rcFill, m_clrBackground, FALSE);

	CXTPTaskPanelItems* pItems = pTaskPanel->GetGroups();
	if (pItems->GetCount() < 1)
		return;

	if (m_bRoundedFrame)
	{
		CRect rcFace (pTaskPanel->GetAt(0)->GetCaptionRect());
		rcFace.bottom = pTaskPanel->GetAt(pItems->GetCount() - 1)->GetClientRect().bottom;

		rcFace.InflateRect(1, 3, 1, 8);
		CXTPPenDC pen(*pDC, m_groupNormal.clrClientBorder);
		CXTPBrushDC brush(*pDC, m_groupNormal.clrClient);

		pDC->RoundRect(rcFace.left, rcFace.top, rcFace.right, rcFace.bottom, 10, 10);
	}
}

int CXTPTaskPanelPaintManagerPlain::DrawGroupCaption(CDC* pDC, CXTPTaskPanelGroup* pGroup, BOOL bDraw)
{
	CXTPFontDC fnt(pDC, &m_fntCaption);

	if (bDraw)
	{
		CRect rc = pGroup->GetCaptionRect();

		if (rc.IsRectEmpty())
			return 0;

		XTP_TASKPANEL_GROUPCOLORS& groupColors =  m_groupNormal;

		if (pGroup->IsCaptionVisible())
		{

			//CRect rcText(rc);
			CRect rcText = DrawCaptionGripper(pDC, pGroup, rc);

			if (!m_bOfficeCaption)
				rcText.DeflateRect(13, 5, 13, 0);


			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(!pGroup->GetEnabled()? GetXtremeColor(COLOR_GRAYTEXT): groupColors.clrClientText);

			DrawItemCaption(pDC, pGroup, rcText, DT_WORDBREAK|DT_LEFT|DT_NOPREFIX);

			if (m_bOfficeCaption)
			{
				pDC->FillSolidRect(rc.left, rc.bottom - 1, rc.Width(), 1, groupColors.clrHead.clrLight);
			}

			if (pGroup->IsItemFocused() && pGroup->GetTaskPanel()->IsDrawFocusRect())
			{
				pDC->SetTextColor(groupColors.clrClientText);
				pDC->DrawText(pGroup->GetCaption(), rcText, DT_WORDBREAK|DT_LEFT|DT_CALCRECT|DT_NOPREFIX);
				pDC->DrawFocusRect(rcText);
			}

		}
		return 0;
	}
	else
	{
		CRect rcText(pGroup->GetTargetCaptionRect());
		rcText.right -= GetCaptionGripperWidth(pGroup, rcText);

		if (!m_bOfficeCaption)
			rcText.DeflateRect(13, 0, 13, 0);

		rcText.top = rcText.bottom = 0;

		CString strCaption = pGroup->GetCaption();
		pDC->DrawText(strCaption.IsEmpty()? _T(" "): strCaption, rcText, DT_WORDBREAK|DT_LEFT|DT_CALCRECT|DT_NOPREFIX);
		return rcText.Height() + 5;
	}
}


//////////////////////////////////////////////////////////////////////////
/// CXTPTaskPanelExplorerThemePlain

CXTPTaskPanelExplorerThemePlain::CXTPTaskPanelExplorerThemePlain()
{
	m_bOfficeHighlight = HIGHLIGHT_OFFICE2003;
	RefreshMetrics();
}

CRect CXTPTaskPanelExplorerThemePlain::DrawCaptionGripper(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	if (!m_bExplorerTheme)
		return DrawCaptionGripper(pDC, pGroup, rc);

	if (!pGroup->IsExpandable())
		return rc;

	if (rc.Width() < 20 + 6)
		return rc;


	return DrawCaptionGripperBitmap(pDC, FALSE, pGroup->IsExpanded(), pGroup->IsItemHot(), rc);

}

void CXTPTaskPanelExplorerThemePlain::RefreshMetrics()
{

	CXTPTaskPanelPaintManagerPlain::RefreshMetrics();

	XTPCurrentSystemTheme systemTheme = XTPDrawHelpers()->GetCurrentSystemTheme();

	m_bExplorerTheme = FALSE;
	if (!m_bEmulateEplorerTheme && (systemTheme == xtpSystemThemeUnknown))
	{
		return ;
	}
	m_bExplorerTheme = TRUE;

	m_eGripper = xtpTaskPanelGripperBitmap;

	m_groupNormal.clrClientBorder.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_groupSpecial.clrClientBorder.SetStandardValue(GetXtremeColor(COLOR_WINDOW));

	switch (systemTheme)
	{
		case xtpSystemThemeBlue:
		{
			m_clrBackground.SetStandardValue(RGB(123, 162, 231), RGB(99, 117, 214));
			m_groupNormal.clrClient.SetStandardValue(RGB(214, 223, 247));

			m_groupNormal.clrClientLink.SetStandardValue(RGB(33, 93, 198));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(66, 142, 255));
			break;

		}
		case xtpSystemThemeOlive:
		{
			m_clrBackground.SetStandardValue(RGB(204, 217, 173), RGB(165, 189, 132));
			m_groupNormal.clrClient.SetStandardValue(RGB(246, 246, 236));

			m_groupNormal.clrClientLink.SetStandardValue(RGB(86, 102, 45));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(114, 146, 29));

			break;
		}
		case xtpSystemThemeSilver:
		{
			m_clrBackground.SetStandardValue(RGB(196, 200, 212), RGB(177, 179, 200));
			m_groupNormal.clrClient.SetStandardValue(RGB(240, 241, 245));

			m_groupNormal.clrClientLink.SetStandardValue(RGB(63, 61, 61));
			m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(126, 124, 124));

			break;
		}
		default:
		{
			m_clrBackground.SetStandardValue(
				BlendColor(GetXtremeColor(COLOR_INACTIVECAPTION),GetXtremeColor(COLOR_WINDOW), 128),
				BlendColor(GetXtremeColor(COLOR_ACTIVECAPTION), GetXtremeColor(COLOR_WINDOW), 192));


			COLORREF clrClient = BlendColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 24);
			m_groupNormal.clrClient.SetStandardValue(BlendColor(clrClient, GetXtremeColor(COLOR_ACTIVECAPTION), 230));

			m_groupNormal.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
			m_groupNormal.clrClientLinkHot.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));

		}
	}
	m_groupSpecial = m_groupNormal;

	CreateGripperBitmaps();

}


//////////////////////////////////////////////////////////////////////////
/// CXTPTaskPanelOfficeXPThemePlain

CXTPTaskPanelOfficeXPThemePlain::CXTPTaskPanelOfficeXPThemePlain()
{
	m_bRoundedFrame = FALSE;
	m_bOfficeCaption = TRUE;
	m_bOfficeHighlight = TRUE;

	RefreshMetrics();
}

void CXTPTaskPanelOfficeXPThemePlain::RefreshMetrics()
{

	CXTPTaskPanelPaintManager::RefreshMetrics();

	m_eGripper = xtpTaskPanelGripperNone;

	COLORREF clrBackground = GetXtremeColor(XPCOLOR_MENUBAR_FACE);
	COLORREF clr3DShadow = GetXtremeColor(COLOR_3DSHADOW);

	m_clrBackground.SetStandardValue(clrBackground, clrBackground);

	m_groupNormal.clrClient.SetStandardValue(clrBackground);
	m_groupNormal.clrHead.SetStandardValue(clr3DShadow, clr3DShadow);
	m_groupNormal.clrClientLink.SetStandardValue(RGB(0, 0, 0xFF));
	m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(0, 0, 0xFF));

	m_groupSpecial = m_groupNormal;

}


//////////////////////////////////////////////////////////////////////////
/// CXTPTaskPanelOffice2003ThemePlain

CXTPTaskPanelOffice2003ThemePlain::CXTPTaskPanelOffice2003ThemePlain()
{
	m_bOfficeHighlight = HIGHLIGHT_OFFICE2003;
	RefreshMetrics();
}

void CXTPTaskPanelOffice2003ThemePlain::RefreshMetrics()
{

	CXTPTaskPanelPaintManager::RefreshMetrics();

	m_eGripper = xtpTaskPanelGripperNone;

	XTPCurrentSystemTheme systemTheme = XTPDrawHelpers()->GetCurrentSystemTheme();

	COLORREF clrBackground;
	COLORREF clr3DShadow;

	switch (systemTheme)
	{
		case xtpSystemThemeBlue:
			clrBackground = RGB(221, 236, 254);
			clr3DShadow = RGB(123, 164, 224);
			break;

		case xtpSystemThemeOlive:
			clrBackground = RGB(243, 242, 231);
			clr3DShadow = RGB(188, 187, 177);
			break;

		case xtpSystemThemeSilver:
			clrBackground = RGB(238, 238, 244);
			clr3DShadow = RGB(161, 160, 187);
			break;

		default:
			clrBackground = XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 50);
			clr3DShadow = GetXtremeColor(COLOR_3DSHADOW);
	}

	m_clrBackground.SetStandardValue(clrBackground, clrBackground);

	m_groupNormal.clrClient.SetStandardValue(clrBackground);
	m_groupNormal.clrHead.SetStandardValue(clr3DShadow, clr3DShadow);
	m_groupNormal.clrClientLink.SetStandardValue(RGB(0, 0, 0xFF));
	m_groupNormal.clrClientLinkHot.SetStandardValue(RGB(0, 0, 0xFF));

	m_groupSpecial = m_groupNormal;

}



//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelToolboxTheme


CXTPTaskPanelToolboxTheme::CXTPTaskPanelToolboxTheme()
{
	m_bBoldCaption = FALSE;
	m_bRoundedFrame = FALSE;
	m_bOfficeHighlight = FALSE;
	m_bInvertDragRect = TRUE;
	m_bCaptionScrollButton = TRUE;

	m_rcGroupInnerMargins = CRect(0, 4, 0, 2);
	m_rcControlMargins = CRect(2, 2, 2, 2);
	m_nGroupSpacing = 1;

	m_nCaptionTextFormat = DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_END_ELLIPSIS;

	RefreshMetrics();
}

void CXTPTaskPanelToolboxTheme::RefreshMetrics()
{
	CXTPTaskPanelPaintManager::RefreshMetrics();

	m_clrBackground.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_groupNormal.clrClient.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_groupNormal.clrClientBorder.SetStandardValue(GetXtremeColor(COLOR_3DFACE));

	m_groupNormal.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrClientLinkHot.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));

	m_groupSpecial = m_groupNormal;

	CWindowDC dc(CWnd::GetDesktopWindow());
	CXTPFontDC fnt(&dc, &m_fntCaption);
	m_nCaptionHeight = max(18, dc.GetTextExtent(_T(" ")).cy + 5);

	if (!m_bOfficeHighlight)
	{
		COLORREF clr3DFace = GetXtremeColor(COLOR_3DFACE);
		COLORREF clr3DHightlight = GetXtremeColor(COLOR_3DHIGHLIGHT);

		int r =  GetRValue(clr3DFace), g = GetGValue(clr3DFace), b = GetBValue(clr3DFace);
		int nDelta =max(max(r, g), b);
		if (r < nDelta) nDelta = min(min(r, g), b);

		m_clrHighlightHot.SetStandardValue(RGB(
			(r + GetRValue(clr3DHightlight))/2,
			(2 * g - nDelta + GetGValue(clr3DHightlight))/2,
			(2 * b - nDelta + GetBValue(clr3DHightlight))/2));
		m_clrHighlightBorder.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
	}
}

CRect CXTPTaskPanelToolboxTheme::DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	CRect rcText(rc);
	pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));

	if (pGroup->IsItemHot() && pGroup->IsItemPressed())
	{
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DDKSHADOW), GetXtremeColor(COLOR_3DHILIGHT));
		rc.DeflateRect(1, 1);
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DFACE));
		rcText.OffsetRect(1, 1);

	} else
	{
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
	}
	return rcText;
}

int CXTPTaskPanelToolboxTheme::DrawGroupCaption(CDC* pDC, CXTPTaskPanelGroup* pGroup, BOOL bDraw)
{
	CXTPFontDC fnt(pDC, &m_fntCaption);

	if (bDraw)
	{
		CRect rc = pGroup->GetCaptionRect();

		if (rc.IsRectEmpty())
			return 0;

		XTP_TASKPANEL_GROUPCOLORS& groupColors =  m_groupNormal;

		if (pGroup->IsCaptionVisible())
		{
			CRect rcText = DrawGroupCaptionBackground(pDC, pGroup, rc);

			rcText.DeflateRect(3, 0);

			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(!pGroup->GetEnabled()? GetXtremeColor(COLOR_GRAYTEXT): groupColors.clrHeadTextNormal);
			DrawItemCaption(pDC, pGroup, rcText, m_nCaptionTextFormat);

			if (pGroup->IsItemDragging())
			{
				pDC->InvertRect(rc);
			}

			if (pGroup->IsItemDragOver() && (m_bInvertDragRect || !(pGroup->IsExpanded() && pGroup->GetItemCount() > 0)))
			{
				DrawDragArrow(pDC, pGroup, rc, TRUE);
			}

			if (pGroup->IsChildItemDragOver() && m_bInvertDragRect)
			{
				pDC->InvertRect(rc);
			}


			if (pGroup->IsItemFocused() && pGroup->GetTaskPanel()->IsDrawFocusRect())
			{
				rcText.DeflateRect(-1, 2);
				pDC->SetTextColor(groupColors.clrClientText);
				pDC->DrawFocusRect(rcText);
			}

		}
		return 0;
	}
	else
	{
		return m_nCaptionHeight;
	}
}


//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelToolboxWhidbeyTheme

CXTPTaskPanelToolboxWhidbeyTheme::CXTPTaskPanelToolboxWhidbeyTheme()
{
	m_bOfficeHighlight = TRUE;
	m_bBoldCaption = TRUE;
	m_bInvertDragRect = FALSE;
	m_bCaptionScrollButton = FALSE;

	RefreshMetrics();
}

CRect CXTPTaskPanelToolboxWhidbeyTheme::DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	CRect rcText(rc);
	if (pGroup->IsItemFocused() && pGroup->GetTaskPanel()->IsDrawFocusRect())
	{
		if (!pGroup->IsItemHot() && !pGroup->IsItemPressed()) Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightSelected);
		else if (pGroup->IsItemHot() && !pGroup->IsItemPressed()) Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightPressed);
		else if (pGroup->IsItemHot() && pGroup->IsItemPressed()) Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightPressed);
		else if (pGroup->IsItemHot() || pGroup->IsItemPressed())  Rectangle(pDC, rc, m_clrHighlightBorder.clrLight, m_clrHighlightHot);

	}
	else
	{
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));

		XTPDrawHelpers()->GradientFill(pDC,
			CRect(rc.left, rc.bottom - 2, rc.right, rc.bottom - 1), GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DHIGHLIGHT), TRUE);
	}

	rcText.DeflateRect(19, 0, 0, 2);

	CRect rcButton(rc.left + 6, rc.CenterPoint().y - 6, rc.left + 6 + 9, rc.CenterPoint().y + 3);

	COLORREF clrButton = GetXtremeColor(COLOR_3DFACE);
	COLORREF clrFrame = GetXtremeColor(COLOR_3DSHADOW);
	pDC->FillSolidRect(rcButton, clrButton);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.top, rcButton.Width() - 2, 1, clrFrame);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.bottom - 1, rcButton.Width() - 2, 1, clrFrame);
	pDC->FillSolidRect(rcButton.left, rcButton.top + 1, 1, rcButton.Height() - 2, clrFrame);
	pDC->FillSolidRect(rcButton.right - 1, rcButton.top + 1, 1, rcButton.Height() - 2, clrFrame);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.top + 1, rcButton.Width() - 2, 3, 0xFFFFFF);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.top + 4, rcButton.Width() - 4, 2, 0xFFFFFF);

	pDC->FillSolidRect(rcButton.left + 2, rcButton.top + 4, rcButton.Width() - 4, 1, 0);
	if (!pGroup->IsExpanded())
		pDC->FillSolidRect(rcButton.left + 4, rcButton.top + 2, 1, rcButton.Height() - 4, 0);

	return rcText;
}

void CXTPTaskPanelToolboxWhidbeyTheme::FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel)
{
	CXTPClientRect rc(pTaskPanel);
	CRect rcFill(rc.left, 0, rc.right, max(rc.bottom, pTaskPanel->GetClientHeight()));

	XTPDrawHelpers()->GradientFill(pDC, rcFill, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DHIGHLIGHT), TRUE);
}

void CXTPTaskPanelToolboxWhidbeyTheme::DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	XTPDrawHelpers()->GradientFill(pDC, rc, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DHIGHLIGHT), TRUE);
	pGroup->OnFillClient(pDC, rc);

	if (pGroup->IsExpanded() || pGroup->IsExpanding())
	DrawGroupClientItems(pDC, pGroup, rc);

}

//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelListViewTheme
CXTPTaskPanelListViewTheme::CXTPTaskPanelListViewTheme()
{
	m_rcGroupInnerMargins.SetRect(8, 6, 8, 6);
	m_rcControlMargins.SetRectEmpty();
	m_nGroupSpacing = 0;
	m_bOfficeHighlight = FALSE;
	m_bInvertDragRect = FALSE;
	m_bCaptionScrollButton = FALSE;
	m_bEmbossedDisabledText = TRUE;

	RefreshMetrics();
}

void CXTPTaskPanelListViewTheme::RefreshMetrics()
{
	CXTPTaskPanelToolboxTheme::RefreshMetrics();

	m_clrBackground.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_groupNormal.clrClient.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_groupNormal.clrClientText.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_groupNormal.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_groupNormal.clrClientLinkHot.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_nCaptionHeight = max(20, m_nCaptionHeight);

	if (!m_bOfficeHighlight)
	{
		m_clrHighlightHot.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
		m_clrHighlightBorder.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT), GetXtremeColor(COLOR_3DFACE));
	}
	m_groupSpecial = m_groupNormal;
}


//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelListViewOfficeXPTheme

CXTPTaskPanelListViewOfficeXPTheme::CXTPTaskPanelListViewOfficeXPTheme()
{
	m_bOfficeHighlight = TRUE;
	m_bEmbossedDisabledText = FALSE;
	RefreshMetrics();
}

void CXTPTaskPanelListViewOfficeXPTheme::RefreshMetrics()
{
	CXTPTaskPanelListViewTheme::RefreshMetrics();

	m_clrBackground.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
	m_groupNormal.clrClient.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
	m_groupNormal.clrClientText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_groupNormal.clrClientLinkHot.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_nCaptionHeight = max(20, m_nCaptionHeight);
	m_groupSpecial = m_groupNormal;
}

//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelListViewOffice2003Theme

CXTPTaskPanelListViewOffice2003Theme::CXTPTaskPanelListViewOffice2003Theme()
{
	m_bOfficeHighlight = HIGHLIGHT_OFFICE2003;
	m_bEmbossedDisabledText = FALSE;
	RefreshMetrics();
}

void CXTPTaskPanelListViewOffice2003Theme::RefreshMetrics()
{
	CXTPTaskPanelListViewOfficeXPTheme::RefreshMetrics();

	m_grcPushed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_grcHot.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT));

	if (!XTPColorManager()->IsLunaColorsDisabled())
	{
		XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

		switch (systemTheme)
		{
		case xtpSystemThemeBlue:
			m_clrBackground.SetStandardValue(RGB(216, 231, 252));
			m_groupNormal.clrClient.SetStandardValue(RGB(216, 231, 252));
			break;
		case xtpSystemThemeOlive:
			m_clrBackground.SetStandardValue(RGB(226, 231, 191));
			m_groupNormal.clrClient.SetStandardValue(RGB(226, 231, 191));
			break;
		case xtpSystemThemeSilver:
			m_clrBackground.SetStandardValue(RGB(223, 223, 234));
			m_groupNormal.clrClient.SetStandardValue(RGB(223, 223, 234));
			break;
		}

		if (systemTheme != xtpSystemThemeUnknown)
		{
			m_grcPushed.SetStandardValue(RGB(232, 127, 8), RGB(247, 218, 124));
			m_grcHot.SetStandardValue(RGB(255, 255, 220), RGB(247, 192, 91));
		}
	}
	m_groupSpecial = m_groupNormal;
}

CRect CXTPTaskPanelListViewOffice2003Theme::DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	if (pGroup->IsItemHot() || pGroup->IsItemPressed())
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, pGroup->IsItemPressed()? m_grcPushed: m_grcHot, FALSE);
	} else
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, XTPColorManager()->grcDockBar.clrDark, XTPColorManager()->grcDockBar.clrLight, FALSE);
	}


	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DHILIGHT), GetXtremeColor(XPCOLOR_FRAME));



	return rc;
}

//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelShortcutBarOffice2003Theme

CXTPTaskPanelShortcutBarOffice2003Theme::CXTPTaskPanelShortcutBarOffice2003Theme()
{
	m_nCaptionTextFormat = DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS;

	RefreshMetrics();
}

void CXTPTaskPanelShortcutBarOffice2003Theme::RefreshMetrics()
{
	CXTPTaskPanelListViewOffice2003Theme::RefreshMetrics();

	CWindowDC dc(CWnd::GetDesktopWindow());
	CXTPFontDC fnt(&dc, &m_fntCaption);
	m_nCaptionHeight = max(22, dc.GetTextExtent(_T(" ")).cy + 3);

	m_groupNormal.clrClient.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_groupNormal.clrClientLink.SetStandardValue(GetXtremeColor(COLOR_WINDOW));

}

CRect CXTPTaskPanelShortcutBarOffice2003Theme::DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	if (pGroup->IsItemHot() || pGroup->IsItemPressed())
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, pGroup->IsItemPressed()? m_grcPushed: m_grcHot, FALSE);
	}
	else
	{
		XTPDrawHelpers()->GradientFill(pDC, rc, XTPColorManager()->grcToolBar, FALSE);
	}

	if (pGroup->GetIndex() != 0)
		pDC->FillSolidRect(rc.left, rc.top, rc.Width(), 1, GetXtremeColor(XPCOLOR_FRAME));

	return rc;
}

void CXTPTaskPanelShortcutBarOffice2003Theme::DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc)
{
	XTPDrawHelpers()->GradientFill(pDC, rc, XTPColorManager()->grcCaption, FALSE);
	pGroup->OnFillClient(pDC, rc);

	if (pGroup->IsExpanded() || pGroup->IsExpanding())
		DrawGroupClientItems(pDC, pGroup, rc);

	pDC->FillSolidRect(rc.left, rc.top, rc.Width(), 1, GetXtremeColor(XPCOLOR_FRAME));

}

void CXTPTaskPanelShortcutBarOffice2003Theme::FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel)
{
	CXTPClientRect rc(pTaskPanel);
	rc.bottom = max(rc.bottom, pTaskPanel->GetClientHeight());

	XTPDrawHelpers()->GradientFill(pDC, rc, XTPColorManager()->grcCaption, FALSE);
}
