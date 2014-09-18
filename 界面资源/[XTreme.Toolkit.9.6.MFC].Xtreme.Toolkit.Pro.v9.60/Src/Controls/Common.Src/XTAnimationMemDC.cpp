// XTAnimationMemDC.cpp : implementation file
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
#include "XTPVC80Helpers.h"  // Visual Studio 2005 helper functions

#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTMemDC.h"
#include "XTAnimationMemDC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CXTAnimationMemDC::LPFNANIMATION CXTAnimationMemDC::m_pCustomAnimation = 0;

//===========================================================================
// CXTAnimationMemDC class
//===========================================================================

CXTAnimationMemDC::CXTAnimationMemDC(CDC* pDC, const CRect& rect, COLORREF clrColor)
: CXTMemDC(pDC, rect, clrColor)
{

}

CXTAnimationMemDC::~CXTAnimationMemDC()
{

}

#define BIT(a, b)   (a & (1 << b))

void CXTAnimationMemDC::Animate(int nType, int nSteps, int nAnimationTime)
{
	//
	// lets say the animation is done in m_nSteps and it should not take longer
	// then m_nAnimationTime then we calculate as follows
	//

	switch(nType)
	{
		case xtWindowsDefault:
			{
				//
				// This registry entry is defined and explained under the
				// following URLs
				//
				// http://www.microsoft.com/WINDOWS2000/techinfo/reskit/en/Regentry/55198.htm
				// http://msdn.microsoft.com/library/en-us/regentry/55198.asp
				// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/regentry/regazbd.asp

				HKEY hKey = NULL;
				DWORD dwType = 0, dwValue = 0, dwSize = sizeof(dwValue);
				::RegOpenKey(HKEY_CURRENT_USER, _T("Control Panel\\Desktop"), &hKey);
				::RegQueryValueEx(hKey, _T("UserPreferencesMask"), NULL, &dwType, (LPBYTE)&dwValue, &dwSize);
				::RegCloseKey(hKey);

				if(BIT(dwValue, 1)) // is Menu animation enabled?
				{
					if(BIT(dwValue, 9)) // if so, which animation?
					{
						nType = xtFade;
					}
					else
					{
						nType = xtSlide, nSteps, nAnimationTime;
					}
					// windows doesn’t support more settings (currently)
				}
			}
			break;

		// choose any animation based on a random number
		case xtRandom:
			{
				switch(RAND_S() % 3)
				{
					case 0:
						nType = xtFade;
						break;

					case 1:
						nType = xtSlide;
						break;

					default:
						nType = xtUnfold;
						break;
				}
			}
			break;
	}

	LPFNANIMATION Animate = m_pCustomAnimation? m_pCustomAnimation: DefaultAnimation;
	Animate(m_rc, m_pDC, this, nType, nSteps, nAnimationTime);
}

void CXTAnimationMemDC::DefaultAnimation(CRect rc, CDC* pDestDC, CDC* pSrcDC, int nType, int nSteps, int nAnimationTime)
{
	CRect r;
	switch(nType)
	{
		case xtSlide:
			{
				const int bottom = rc.bottom - rc.top;
				int i;
				for (i = 0; i < rc.Height();
					i += (1 + (rc.Height() / nSteps)))
				{
					pDestDC->BitBlt(rc.left, rc.top, rc.Width(), i, pSrcDC,
						0, bottom - i, SRCCOPY);
					Sleep(nAnimationTime / nSteps);
				}
			}
			break;

		case xtUnfold:
			{
				r = rc;
				int i;
				for (i = 0; i < nSteps; ++i)
				{
					r.bottom = r.top  + rc.Height() * (i + 1) / nSteps;
					r.right  = r.left + rc.Width()  * (i + 1) / nSteps;
					pDestDC->BitBlt(r.left, r.top, r.Width(), r.Height(), pSrcDC,
						0, 0, SRCCOPY);
					Sleep(nAnimationTime / nSteps);
				}
			}
			break;

		case xtFade:
			{

				const int cx = rc.Width();
				const int cy = rc.Height();

				BITMAPINFOHEADER BMI;
				// Fill in the header info.
				ZeroMemory (&BMI, sizeof (BMI));
				BMI.biSize = sizeof(BITMAPINFOHEADER);
				BMI.biWidth = cx;
				BMI.biHeight = cy;
				BMI.biPlanes = 1;
				BMI.biBitCount = 32;
				BMI.biCompression = BI_RGB;   // No compression

				BYTE * pSrcBits = NULL;
				HBITMAP hbmSrc = CreateDIBSection (NULL, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, 0, 0l);

				BYTE * pSrcBackBits = NULL;
				HBITMAP hbmSrcBack = CreateDIBSection (NULL, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBackBits, 0, 0l);

				BYTE * pDestBits = NULL;
				HBITMAP hbmDest = CreateDIBSection (NULL, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, 0, 0l);

				// Copy our source and destination bitmaps onto our DIBSections,
				// so we can get access to their bits using the BYTE *'s we passed into CreateDIBSection

				CDC dc;
				dc.CreateCompatibleDC(NULL);
				HBITMAP hbmpOld = (HBITMAP) ::SelectObject(dc, hbmSrc);
				::BitBlt(dc, 0, 0, cx, cy, pSrcDC->GetSafeHdc(), 0, 0, SRCCOPY);

				::SelectObject(dc, hbmSrcBack);
				::BitBlt(dc, 0, 0, cx, cy, pDestDC->GetSafeHdc (), rc.left, rc.top, SRCCOPY);

				::SelectObject(dc, hbmDest);

				DWORD dwTimePer = nAnimationTime / nSteps;
				::SelectObject(dc, hbmDest);
				for (int i = 1; i < nSteps; ++i)
				{
					DWORD dwTime = GetTickCount ();
					AlphaBlendU(pDestBits, pSrcBackBits, cx, cy, pSrcBits, (BYTE)((255*i) / nSteps));
					pDestDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &dc, 0, 0, SRCCOPY);
					dwTime = GetTickCount () - dwTime;
					if (dwTime < dwTimePer)
					{
						Sleep(dwTimePer - dwTime);
					}
				}
				::SelectObject(dc, hbmpOld);
				DeleteObject(hbmSrc);
				DeleteObject(hbmSrcBack);
				DeleteObject(hbmDest);
			}
			break;
	}
}

void CXTAnimationMemDC::AlphaBlendU(PBYTE pDest, PBYTE pSrcBack, int cx, int cy, PBYTE pSrc, BYTE byAlpha)
{
	BYTE byDiff = (BYTE)(255 - byAlpha);

	int iRowOffset = 0, j = cy;
	while (j > 0)
	{
		--j;
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrc)[iRowOffset];
		LPBYTE pbSrcBackRGB = (LPBYTE)&((DWORD*)pSrcBack)[iRowOffset];
		LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDest)[iRowOffset];
		iRowOffset += cx;
		int i = cx;
		while (i > 0)
		{
			--i;
			pbDestRGB[0]=(BYTE)((pbSrcBackRGB[0] * byDiff + pbSrcRGB[0] * byAlpha)>>8);
			pbDestRGB[1]=(BYTE)((pbSrcBackRGB[1] * byDiff + pbSrcRGB[1] * byAlpha)>>8);
			pbDestRGB[2]=(BYTE)((pbSrcBackRGB[2] * byDiff + pbSrcRGB[2] * byAlpha)>>8);
			pbSrcRGB += 4;
			pbSrcBackRGB += 4;
			pbDestRGB += 4;
		}
	}
}

void CXTAnimationMemDC::SetCustomAnimation(LPFNANIMATION pCustom)
{
	m_pCustomAnimation = pCustom;
}
