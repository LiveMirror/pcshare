// XTPImageManager.cpp : implementation of the CXTPImageManager class.
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
#include <math.h>
#include <shlwapi.h>
#include "XTPVC80Helpers.h"
#include "XTPColorManager.h"
#include "XTPDrawHelpers.h"
#include "XTPMacros.h"
#include "XTPPropExchange.h"
#include "XTPImageManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPImageManager CXTPImageManager::s_managerInstance;

#define DESTROYICON(hIcon) if (hIcon) { ::DestroyIcon(hIcon); hIcon = 0; }
#define FREE(hHandle) if (hHandle) { free(hHandle); hHandle = 0; }

double CXTPImageManager::m_dDisabledAlphaFactor = 1.0;
double CXTPImageManager::m_dDisabledBrightnessFactor = 0.5;
BOOL CXTPImageManager::m_bAutoResample = FALSE;
CLIPFORMAT CXTPImageManager::m_nImageClipFormat = (CLIPFORMAT)RegisterClipboardFormat(_T("Xtreme ToolBar Image"));
CLIPFORMAT CXTPImageManager::m_nAlphaClipFormat = (CLIPFORMAT)RegisterClipboardFormat(_T("Alpha Bitmap Image"));




// resources are WORD-aligned
#pragma pack(push, 2)

struct CXTPImageManagerIcon::ICONDIRENTRY
{
	BYTE            bWidth;
	BYTE            bHeight;
	BYTE            bColorCount;
	BYTE            bReserved;
	WORD            wPlanes;
	WORD            wBitCount;
	DWORD           dwBytesInRes;
	DWORD           dwImageOffset;
};

struct CXTPImageManagerIcon::ICONDIR
{
	WORD            idReserved;
	WORD            idType;
	WORD            idCount;
	ICONDIRENTRY    idEntries[1];
};

struct CXTPImageManagerIcon::ICONIMAGE
{
	BITMAPINFOHEADER    icHeader;   // DIB header
	RGBQUAD             icColors[1];// Color table
	BYTE                icXOR[1];   // DIB bits for XOR mask
	BYTE                icAND[1];   // DIB bits for AND mask
};


struct CXTPImageManagerIcon::GRPICONDIRENTRY
{
	BYTE   bWidth;                  // Width, in pixels, of the image
	BYTE   bHeight;                 // Height, in pixels, of the image
	BYTE   bColorCount;             // Number of colors in image (0 if >=8bpp)
	BYTE   bReserved;               // Reserved
	WORD   wPlanes;                 // Color Planes
	WORD   wBitCount;               // Bits per pixel
	DWORD  dwBytesInRes;            // how many bytes in this resource?
	WORD   nID;                     // the ID
};

struct CXTPImageManagerIcon::GRPICONDIR
{
	WORD              idReserved;   // Reserved (must be 0)
	WORD              idType;       // Resource type (1 for icons)
	WORD              idCount;      // How many images?
	GRPICONDIRENTRY   idEntries[1]; // The entries for each image
};


#pragma pack(pop)

BOOL CXTPImageManagerIcon::IsAlphaBitmapFile(LPCTSTR pszFileName)
{
	HANDLE hFile = CreateFile(pszFileName, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (!hFile)
		return FALSE;


	DWORD dwBytesRead;

	BITMAPFILEHEADER fileHeader;

	if (!ReadFile(hFile, &fileHeader, sizeof(BITMAPFILEHEADER), &dwBytesRead, 0) || dwBytesRead != sizeof(BITMAPFILEHEADER))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	BITMAPINFOHEADER infoHeader;

	if (!ReadFile(hFile, &infoHeader, sizeof(BITMAPINFOHEADER), &dwBytesRead, 0) || dwBytesRead != sizeof(BITMAPINFOHEADER))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	BOOL bResult = infoHeader.biBitCount == 32;

	CloseHandle(hFile);

	return bResult;
}

HBITMAP CXTPImageManagerIcon::LoadAlphaBitmap(UINT nIDResource)
{
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(nIDResource), RT_BITMAP);

	HRSRC hResource = ::FindResource(hInst, MAKEINTRESOURCE(nIDResource), RT_BITMAP);
	if (hResource == NULL)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(hInst, hResource);
	if (hGlobal == NULL)
		return FALSE;

	LPBITMAPINFO pResourceInfo = (LPBITMAPINFO)::LockResource(hGlobal);
	ASSERT(pResourceInfo != NULL);

	int biSizeImage = pResourceInfo->bmiHeader.biHeight * ((pResourceInfo->bmiHeader.biWidth * 4 + 3) & ~3);

	HBITMAP hbmResult = FALSE;

	if (pResourceInfo->bmiHeader.biBitCount == 32 &&
		SizeofResource(hInst, hResource) >= int(biSizeImage + sizeof(BITMAPINFOHEADER)))
	{
		CDC dcSrc;
		dcSrc.CreateCompatibleDC(NULL);

		PBITMAPINFO pBitmapInfo = (PBITMAPINFO)malloc(sizeof(BITMAPINFOHEADER) + sizeof(DWORD) * 3);
		ASSERT(pBitmapInfo != NULL);

		MEMCPY_S(pBitmapInfo, &pResourceInfo->bmiHeader, sizeof(BITMAPINFOHEADER));
		pBitmapInfo->bmiHeader.biSizeImage = biSizeImage;

		BYTE* pDestBits = NULL;
		HBITMAP hBmp = CreateDIBSection(dcSrc, pBitmapInfo, DIB_RGB_COLORS, (void**)&pDestBits, NULL, 0);

		if (hBmp && pDestBits)
		{
			MEMCPY_S(pDestBits, &pResourceInfo->bmiColors, biSizeImage);
			hbmResult = hBmp;
		}
		FREE(pBitmapInfo);
	}

	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	return hbmResult;

}



HBITMAP CXTPImageManagerIcon::LoadAlphaIcon(LPCTSTR pszFileName, int nWidth)
{

	HANDLE hFile = CreateFile(pszFileName, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (!hFile)
		return NULL;

	DWORD dwBytesRead;

	ICONDIR* pIconDir = (ICONDIR*)malloc( sizeof( ICONDIR ) );

	if (!ReadFile( hFile, pIconDir, sizeof( WORD ) * 3 , &dwBytesRead, NULL ) || dwBytesRead != sizeof( WORD ) * 3)
	{
		free( pIconDir );
		CloseHandle(hFile);
		return FALSE;
	}

	pIconDir = (ICONDIR*)realloc( pIconDir, ( sizeof( WORD ) * 3 ) +
		( sizeof( ICONDIRENTRY ) * pIconDir->idCount ) );

	if (!ReadFile( hFile, pIconDir->idEntries, pIconDir->idCount * sizeof(ICONDIRENTRY),    &dwBytesRead, NULL )
		|| dwBytesRead != pIconDir->idCount * sizeof(ICONDIRENTRY))
	{
		free( pIconDir );
		CloseHandle(hFile);
		return FALSE;
	}

	HBITMAP hBitmap = 0;

	for(int i = 0; i < pIconDir->idCount; i++)
	{
		if (pIconDir->idEntries[i].wBitCount == 32 && (((int)pIconDir->idEntries[i].bWidth == nWidth) || (nWidth == 0)))
		{
			if (!SetFilePointer( hFile, pIconDir->idEntries[i].dwImageOffset,
				NULL, FILE_BEGIN ))
				continue;

			ICONIMAGE* pIconImage = (ICONIMAGE*)malloc( pIconDir->idEntries[i].dwBytesInRes);

			DWORD dwBytesRead;
			ReadFile( hFile, pIconImage, pIconDir->idEntries[i].dwBytesInRes,
				&dwBytesRead, NULL );

			if ((dwBytesRead != pIconDir->idEntries[i].dwBytesInRes) || pIconImage->icHeader.biBitCount != 32)
			{
				free( pIconImage );
				continue;
			}

			CDC dcSrc;
			dcSrc.CreateCompatibleDC(NULL);

			BITMAPINFOHEADER* pHeader = &pIconImage->icHeader;

			pHeader->biHeight /= 2;
			pHeader->biSizeImage = pHeader->biHeight * ((pHeader->biWidth * 4 + 3) & ~3);;

			BYTE* pDest = NULL;
			HBITMAP hBmp = CreateDIBSection(dcSrc, (BITMAPINFO*)pHeader, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);

			if (pDest != NULL && hBmp != NULL)
			{
				MEMCPY_S(pDest, &pIconImage->icColors, pHeader->biSizeImage);
				hBitmap = hBmp;

				free( pIconImage );
				break;
			}

			free( pIconImage );
		}
	}
	free( pIconDir );
	CloseHandle(hFile);

	return hBitmap;

}



//////////////////////////////////////////////////////////////////////////
// CXTPImageManagerIconHandle

CXTPImageManagerIconHandle::CXTPImageManagerIconHandle()
{
	m_hIcon = 0;
	m_hBitmap = m_hAlphaBits = 0;
	m_bClearHandles = TRUE;
}
CXTPImageManagerIconHandle::CXTPImageManagerIconHandle(HICON hIcon)
{
	m_hIcon = hIcon;
	m_hBitmap = m_hAlphaBits = 0;
	m_bClearHandles = FALSE;
}
CXTPImageManagerIconHandle::CXTPImageManagerIconHandle(HBITMAP hBitmap)
{
	m_hIcon = 0;
	m_hBitmap = hBitmap;
	m_hAlphaBits = 0;
	m_bClearHandles = FALSE;
}
CXTPImageManagerIconHandle::CXTPImageManagerIconHandle(const CXTPImageManagerIconHandle& hHandle)
{
	m_hIcon = hHandle.m_hIcon;
	m_hBitmap = hHandle.m_hBitmap;

	m_bClearHandles = FALSE;
	m_hAlphaBits = 0;
}

void CXTPImageManagerIconHandle::CopyHandle(HBITMAP hBitmap)
{
	Clear();
	m_hBitmap = CXTPImageManagerIcon::CopyAlphaBitmap(hBitmap);

	m_bClearHandles = TRUE;
}
void CXTPImageManagerIconHandle::CopyHandle(CXTPImageManagerIconHandle& hHandle)
{
	Clear();

	if (hHandle.m_hIcon) m_hIcon = CopyIcon(hHandle.m_hIcon);
	if (hHandle.m_hBitmap) m_hBitmap = CXTPImageManagerIcon::CopyAlphaBitmap(hHandle.m_hBitmap);

	m_bClearHandles = TRUE;
}

const CXTPImageManagerIconHandle& CXTPImageManagerIconHandle::operator=(const HICON hIcon)
{
	Clear();
	m_hIcon = hIcon;
	m_bClearHandles = TRUE;

	return *this;
}
const CXTPImageManagerIconHandle& CXTPImageManagerIconHandle::operator=(const HBITMAP hBitmap)
{
	Clear();
	m_hBitmap = hBitmap;
	m_bClearHandles = TRUE;
	return *this;
}

CXTPImageManagerIconHandle::~CXTPImageManagerIconHandle()
{
	Clear();
}

BOOL CXTPImageManagerIconHandle::IsEmpty() const
{
	return m_hIcon == 0 && m_hBitmap == 0;
}

BOOL CXTPImageManagerIconHandle::IsAlpha() const
{
	return m_hBitmap != 0;
}
HBITMAP CXTPImageManagerIconHandle::GetBitmap() const
{
	ASSERT(IsAlpha());
	return m_hBitmap;
}
HICON CXTPImageManagerIconHandle::GetIcon() const
{
	return m_hIcon;
}

void CXTPImageManagerIconHandle::Clear()
{
	if (m_bClearHandles)
	{
		if (m_hIcon)
		{
			DestroyIcon(m_hIcon);
		}
		if (m_hBitmap)
		{
			DeleteObject(m_hBitmap);
		}
	}

	if (m_hAlphaBits)
	{
		DeleteObject(m_hAlphaBits);
	}

	m_hIcon = 0;
	m_hBitmap = 0;
	m_hAlphaBits = 0;
}

CSize CXTPImageManagerIconHandle::GetExtent() const
{
	if (m_hIcon)
		return CXTPImageManagerIcon::GetExtent(m_hIcon);

	if (m_hBitmap)
	{
		BITMAP bmpinfo;
		if (::GetObject(m_hBitmap, sizeof(bmpinfo), &bmpinfo))
		{
			return CSize((int)bmpinfo.bmWidth, (int)bmpinfo.bmHeight);
		}
	}
	return 0;

}



BOOL CXTPImageManagerIconHandle::PreMultiply()
{
	if (m_hAlphaBits != 0)
		return TRUE;

	if (m_hIcon != 0)
	{
		ICONINFO info;
		if (GetIconInfo(m_hIcon, &info))
		{

			m_hAlphaBits = CXTPImageManagerIcon::PreMultiplyAlphaBitmap(info.hbmColor);

			::DeleteObject(info.hbmColor);
			::DeleteObject(info.hbmMask);
		}
	}
	else if (m_hBitmap != 0)
	{
		m_hAlphaBits = CXTPImageManagerIcon::PreMultiplyAlphaBitmap(m_hBitmap);
	}
	return m_hAlphaBits != 0;
}

BOOL CXTPImageManagerIcon::IsAlpha()
{
	return m_hIcon.IsAlpha();
}


HBITMAP CXTPImageManagerIcon::PreMultiplyAlphaBitmap(HBITMAP hBitmap)
{

	CDC dcSrc;
	dcSrc.CreateCompatibleDC(NULL);

	PBYTE pBits = 0;
	PBITMAPINFO pBitmapInfo = 0;

	HBITMAP hBitmapAlpha = 0;

	TRY
	{
		UINT nSize;
		if (!GetBitmapBits(dcSrc, hBitmap, pBitmapInfo, (LPVOID&)pBits, nSize))
			return 0;

		BYTE* pDest = NULL;

		hBitmapAlpha = CreateDIBSection(dcSrc, pBitmapInfo, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
		if (pDest == NULL || hBitmapAlpha == NULL)
			AfxThrowMemoryException();

		MEMCPY_S(pDest, pBits, nSize);

		for (UINT i = 0; i < nSize; i += 4)
		{
			int nAlpha = pBits[i + 3];
			pDest[i + 0] = BYTE(pBits[i + 0] * nAlpha / 255);
			pDest[i + 1] = BYTE(pBits[i + 1] * nAlpha / 255);
			pDest[i + 2] = BYTE(pBits[i + 2] * nAlpha / 255);
		}
	}
	CATCH (CMemoryException, e)
	{
		TRACE(_T("Failed -- Memory exception thrown."));
	}
	END_CATCH

	FREE(pBits);
	FREE(pBitmapInfo);

	return hBitmapAlpha;
}

BOOL CXTPImageManagerIcon::GetBitmapBits(CDC& dcSrc, HBITMAP hBitmap, PBITMAPINFO& pBitmapInfo, LPVOID& pBits, UINT& nSize)
{
	if (hBitmap == 0)
		return FALSE;

	BITMAPINFO  bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);


	if (GetDIBits(dcSrc, hBitmap, 0, 0, NULL, (BITMAPINFO*)&bmi, DIB_RGB_COLORS) == 0)
		return FALSE;

	if (bmi.bmiHeader.biBitCount != 32)
		return FALSE;

	nSize = bmi.bmiHeader.biSizeImage != 0? bmi.bmiHeader.biSizeImage: bmi.bmiHeader.biHeight * ((bmi.bmiHeader.biWidth * 4 + 3) & ~3);
	pBits = (PBYTE)malloc(nSize);
	if (pBits  == NULL)
		return FALSE;

	if ((pBitmapInfo = (PBITMAPINFO)malloc(sizeof(BITMAPINFOHEADER)+sizeof(DWORD)*3 )) == NULL)
	{
		FREE(pBits);
		return FALSE;
	}

	MEMCPY_S(pBitmapInfo, &bmi, sizeof(BITMAPINFOHEADER));

	if (GetDIBits(dcSrc, hBitmap, 0, bmi.bmiHeader.biHeight, pBits, pBitmapInfo, DIB_RGB_COLORS) == NULL)
	{
		FREE(pBits);
		FREE(pBitmapInfo);
		return FALSE;
	}
	return TRUE;
}

HBITMAP CXTPImageManagerIcon::CopyAlphaBitmap(HBITMAP hBitmap)
{
	CDC dcSrc;
	dcSrc.CreateCompatibleDC(NULL);

	PBYTE pBits = 0;
	PBITMAPINFO pBitmapInfo = 0;
	UINT nSize;

	if (!GetBitmapBits(dcSrc, hBitmap, pBitmapInfo, (LPVOID&)pBits, nSize))
		return 0;

	BYTE* pDest = NULL;
	HBITMAP hBmp = CreateDIBSection(dcSrc, pBitmapInfo, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);

	if (pDest != NULL && hBmp != NULL)
	{
		MEMCPY_S(pDest, pBits, nSize);
	}

	FREE(pBits);
	FREE(pBitmapInfo);

	return hBmp;

}




//////////////////////////////////////////////////////////////////////////
// CXTPImageManagerIcon
CXTPImageManagerIcon::CXTPImageManagerIcon(UINT nID, int nWidth, int nHeight, CXTPImageManagerIconSet* pIconSet)
: m_nID(nID)
, m_nWidth(nWidth)
, m_nHeight(nHeight)
, m_pIconSet(pIconSet)
{
	m_bScaled       = FALSE;

	EnableAutomation();
}


CXTPImageManagerIcon::~CXTPImageManagerIcon()
{
	Clear(TRUE);
}


void CXTPImageManagerIcon::Clear(BOOL bIcon /* = FALSE */)
{
	if (bIcon)
	{
		m_hIcon.Clear();
	}
	m_hFaded.Clear();
	m_hShadow.Clear();
	m_hHot.Clear();
	m_hChecked.Clear();
	m_hDisabled.Clear();
	m_hDisabledAuto.Clear();
}

void CXTPImageManagerIcon::Refresh()
{
	m_hFaded.Clear();
	m_hShadow.Clear();
	m_hDisabledAuto.Clear();
}

CXTPImageManagerIconHandle& CXTPImageManagerIcon::GetIcon()
{
	return m_hIcon;
}

CXTPImageManagerIconHandle& CXTPImageManagerIcon::GetFadedIcon()
{
	if (m_hFaded.IsEmpty())
	{
		CreateFadedIcon();
	}
	return m_hFaded;
}

CXTPImageManagerIconHandle& CXTPImageManagerIcon::GetShadowIcon()
{
	if (m_hShadow.IsEmpty())
	{
		CreateShadowIcon();
	}
	return m_hShadow;
}

CXTPImageManagerIconHandle& CXTPImageManagerIcon::GetHotIcon()
{
	return !m_hHot.IsEmpty()? m_hHot: m_hIcon;
}

CXTPImageManagerIconHandle& CXTPImageManagerIcon::GetCheckedIcon()
{
	return !m_hChecked.IsEmpty()? m_hChecked: GetHotIcon();
}


CXTPImageManagerIconHandle& CXTPImageManagerIcon::GetDisabledIcon()
{
	if (!m_hDisabled.IsEmpty())
		return m_hDisabled;

	if (!m_hDisabledAuto.IsEmpty())
		return m_hDisabledAuto;

	CreateDisabledIcon();

	return m_hDisabledAuto;
}

void CXTPImageManagerIcon::SetIcon(CXTPImageManagerIconHandle hIcon)
{
	ASSERT(!hIcon.IsEmpty());
	Clear(TRUE);
	m_hIcon.CopyHandle(hIcon);
	CSize sz = GetExtent();

	m_nHeight = sz.cy;
	m_nWidth  = sz.cx;

	m_bScaled = FALSE;

	if (m_pIconSet)
		m_pIconSet->RefreshAll();
}

void CXTPImageManagerIcon::SetNormalIcon(CXTPImageManagerIconHandle hIcon)
{
	ASSERT(!hIcon.IsEmpty());

	Refresh();
	m_hIcon.CopyHandle(hIcon);


#ifdef _DEBUG
	CSize sz = GetExtent();

	ASSERT((int)m_nHeight == sz.cy);
	ASSERT((int)m_nWidth == sz.cx);

#endif
}

void CXTPImageManagerIcon::SetHotIcon(CXTPImageManagerIconHandle hIcon)
{
	Refresh();
	m_hHot.CopyHandle(hIcon);
}

void CXTPImageManagerIcon::SetCheckedIcon(CXTPImageManagerIconHandle hIcon)
{
	Refresh();
	m_hChecked.CopyHandle(hIcon);
}

void CXTPImageManagerIcon::SetDisabledIcon(CXTPImageManagerIconHandle hIcon)
{
	Refresh();
	m_hDisabled.CopyHandle(hIcon);
}


COLORREF CXTPImageManagerIcon::LightenColor(COLORREF clr, double factor)
{
	return RGB(
		factor * 255 + (1.0 - factor) * GetRValue(clr),
		factor * 255 + (1.0 - factor) * GetGValue(clr),
		factor * 255 + (1.0 - factor) * GetBValue(clr)) ;
}

void CXTPImageManagerIcon::CreateFadedIcon()
{
	ASSERT(m_hFaded.IsEmpty());

	ICONINFO info;

	if (!m_hIcon.IsAlpha())
	{
		if (GetIconInfo(m_hIcon.GetIcon(), &info))
		{
			if (!CXTPDrawHelpers::IsLowResolution())
			{
				CXTPCompatibleDC dc(NULL, CBitmap::FromHandle(info.hbmColor));
				CXTPCompatibleDC dcMask(NULL, CBitmap::FromHandle(info.hbmMask));

				BITMAP bmp;
				::GetObject(info.hbmColor, sizeof (BITMAP), &bmp);

				for(int i = 0; i < bmp.bmWidth; i++)
				for(int j = 0; j < bmp.bmHeight; j++)
				{
					COLORREF clr = dc.GetPixel(i, j);
					COLORREF clrMask = dcMask.GetPixel(i, j);
					if (clrMask == 0)
						dc.SetPixel(i, j, LightenColor(clr, .3));
				}
			}

			m_hFaded = CreateIconIndirect(&info);

			::DeleteObject(info.hbmColor);
			::DeleteObject(info.hbmMask);
		}
	}
	else
	{
		CDC dcSrc;
		dcSrc.CreateCompatibleDC(NULL);

		PBYTE pBits = 0;
		PBITMAPINFO pBitmapInfo = 0;

		m_hFaded.CopyHandle(m_hIcon);

		TRY
		{
			UINT nSize;
			if (!GetBitmapBits(dcSrc, m_hFaded.GetBitmap(), pBitmapInfo, (LPVOID&)pBits, nSize))
				AfxThrowMemoryException();

			for (UINT i = 0; i < nSize; i += 4)
			{
				pBits[i+0] = BYTE(.3 * 255 + (1.0 - .3) * pBits[i+0]);
				pBits[i+1] = BYTE(.3 * 255 + (1.0 - .3) * pBits[i+1]);
				pBits[i+2] = BYTE(.3 * 255 + (1.0 - .3) * pBits[i+2]);
			}

			SetDIBits(dcSrc, m_hFaded.GetBitmap(), 0, pBitmapInfo->bmiHeader.biHeight, pBits, pBitmapInfo, DIB_RGB_COLORS);

		}
		CATCH (CMemoryException, e)
		{
			TRACE(_T("Failed -- Memory exception thrown."));
		}
		END_CATCH

		FREE(pBits);
		FREE(pBitmapInfo);
	}
}

void CXTPImageManagerIcon::CreateShadowIcon()
{
	ASSERT(m_hShadow.IsEmpty());

	COLORREF clrBackground = GetXtremeColor(XPCOLOR_HIGHLIGHT);
	COLORREF clrShadow = RGB( GetRValue(clrBackground) * .75, GetGValue(clrBackground ) * .75, GetBValue(clrBackground) * .75);


	if (!GetHotIcon().IsAlpha())
	{
		ICONINFO info;
		if (GetIconInfo(GetHotIcon().GetIcon(), &info))
		{
			{

				CXTPCompatibleDC dc(NULL, CBitmap::FromHandle(info.hbmColor));
				CXTPCompatibleDC dcMask(NULL, CBitmap::FromHandle(info.hbmMask));

				BITMAP bmp;
				::GetObject(info.hbmColor, sizeof (BITMAP), &bmp);

				for(int i = 0; i < bmp.bmWidth; i++)
				for(int j = 0; j < bmp.bmHeight; j++)
				{
					COLORREF clrMask = dcMask.GetPixel(i, j);
					if (clrMask == 0)
						dc.SetPixel(i, j, clrShadow);
				}
			}

			m_hShadow = CreateIconIndirect(&info);

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

		m_hShadow.CopyHandle(GetHotIcon());

		TRY
		{
			UINT nSize;
			if (!GetBitmapBits(dcSrc, m_hShadow.GetBitmap(), pBitmapInfo, (LPVOID&)pBits, nSize))
				AfxThrowMemoryException();

			for (UINT i = 0; i < nSize; i += 4)
			{
				pBits[i + 0] = GetRValue(clrShadow);
				pBits[i + 1] = GetGValue(clrShadow);
				pBits[i + 2] = GetBValue(clrShadow);
			}

			SetDIBits(dcSrc, m_hShadow.GetBitmap(), 0, pBitmapInfo->bmiHeader.biHeight, pBits, pBitmapInfo, DIB_RGB_COLORS);
		}
		CATCH (CMemoryException, e)
		{
			TRACE(_T("Failed -- Memory exception thrown."));
		}
		END_CATCH

		FREE(pBits);
		FREE(pBitmapInfo);
	}
}

void CXTPImageManagerIcon::CreateDisabledIcon()
{
	ICONINFO info;

	if (!m_hIcon.IsAlpha())
	{
		if (GetIconInfo(m_hIcon.GetIcon(), &info))
		{
			{
				CXTPCompatibleDC dc(NULL, CBitmap::FromHandle(info.hbmColor));
				CXTPCompatibleDC dcMask(NULL, CBitmap::FromHandle(info.hbmMask));

				BITMAP bmp;
				::GetObject(info.hbmColor, sizeof (BITMAP), &bmp);

				for(int i = 0; i < bmp.bmWidth; i++)
				for(int j = 0; j < bmp.bmHeight; j++)
				{
					COLORREF clrMask = dcMask.GetPixel(i, j);
					COLORREF clr = dc.GetPixel(i, j);
					if (clrMask == 0)
					{
						double dGray = GetRValue(clr) * 0.299 + GetGValue(clr) * 0.587 + GetBValue(clr) * 0.114;
						int nGray = (BYTE)(pow(dGray / 255.0, CXTPImageManager::m_dDisabledBrightnessFactor) * 255.0);

						dc.SetPixel(i, j, RGB(nGray, nGray, nGray));
					}
				}
			}

			m_hDisabledAuto = CreateIconIndirect(&info);
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

		m_hDisabledAuto.CopyHandle(m_hIcon);

		TRY
		{
			UINT nSize;
			if (!GetBitmapBits(dcSrc, m_hDisabledAuto.GetBitmap(), pBitmapInfo, (LPVOID&)pBits, nSize))
				AfxThrowMemoryException();

			for (UINT i = 0; i < nSize; i += 4)
			{
				double dGray = pBits[i + 0] * 0.299 + pBits[i + 1] * 0.587 + pBits[i + 2] * 0.114;
				pBits[i + 0] = pBits[i + 1] = pBits[i + 2] = (BYTE)(pow(dGray / 255.0, CXTPImageManager::m_dDisabledBrightnessFactor) * 255.0);

				pBits[i + 3] = BYTE(pBits[i + 3]/CXTPImageManager::m_dDisabledAlphaFactor);
			}

			SetDIBits(dcSrc, m_hDisabledAuto.GetBitmap(), 0, pBitmapInfo->bmiHeader.biHeight, pBits, pBitmapInfo, DIB_RGB_COLORS);

		}
		CATCH (CMemoryException, e)
		{
			TRACE(_T("Failed -- Memory exception thrown."));
		}
		END_CATCH

		FREE(pBits);
		FREE(pBitmapInfo);
	}
}

CSize CXTPImageManagerIcon::GetExtent()
{
	return m_hIcon.GetExtent();
}

CSize CXTPImageManagerIcon::GetExtent(HICON hIcon)
{
	ASSERT(hIcon);

	CSize extent(0);
	if (hIcon)
	{
		ICONINFO iconinfo;
		if (::GetIconInfo(hIcon,&iconinfo))
		{
			BITMAP bmpinfo;
			if (::GetObject(iconinfo.hbmMask, sizeof(bmpinfo), &bmpinfo))
			{
				extent.cx = (int)bmpinfo.bmWidth;
				extent.cy = (int)bmpinfo.bmHeight;
				if (!iconinfo.hbmColor)
				{
					// b/w icons have double size for XOR and AND masks
					extent.cy /= 2;
				}
			}
			// cleanup GDI
			::DeleteObject(iconinfo.hbmMask);
			::DeleteObject(iconinfo.hbmColor);
		}
	}
	return extent;
}


HICON CXTPImageManagerIcon::ScaleToFit(HICON hIcon, CSize szExtent, int nWidth)
{
	if (nWidth == 0 || szExtent == CSize(0))
	{
		// invalid arg
		return NULL;
	}
	if (nWidth == szExtent.cx)
		return ::CopyIcon(hIcon);

	CSize szDesiredExtent(nWidth, MulDiv(szExtent.cy, nWidth, szExtent.cx));

	// scale the icon
	CImageList images;
	VERIFY(images.Create(szDesiredExtent.cx, szDesiredExtent.cy, ILC_COLOR32 | ILC_MASK, 1, 1 ) );
	images.Add(hIcon);
	return images.ExtractIcon(0);
}

CXTPImageManagerIcon* CXTPImageManagerIcon::Scale(int nWidth)
{
	ASSERT(m_hIcon.GetIcon());

	if (!m_hIcon.GetIcon())
		return NULL;

	CSize szExtent = GetExtent();

	CXTPImageManagerIcon* pIcon = new CXTPImageManagerIcon(m_nID, nWidth, MulDiv(szExtent.cy, nWidth, szExtent.cx), m_pIconSet);

	if (m_hIcon.GetIcon())
	{
		HICON hIcon = ScaleToFit(m_hIcon.GetIcon(), szExtent, nWidth);
		pIcon->SetIcon(hIcon);
		::DestroyIcon(hIcon);
	}
	if (m_hDisabled.GetIcon())
	{
		HICON hIcon = ScaleToFit(m_hDisabled.GetIcon(), szExtent, nWidth);
		pIcon->SetDisabledIcon(hIcon);
		::DestroyIcon(hIcon);
	}
	if (m_hHot.GetIcon())
	{
		HICON hIcon = ScaleToFit(m_hHot.GetIcon(), szExtent, nWidth);
		pIcon->SetHotIcon(hIcon);
		::DestroyIcon(hIcon);
	}
	if (m_hChecked.GetIcon())
	{
		HICON hIcon = ScaleToFit(m_hChecked.GetIcon(), szExtent, nWidth);
		pIcon->SetCheckedIcon(hIcon);
		::DestroyIcon(hIcon);
	}

	return pIcon;
}


BOOL CXTPImageManagerIcon::GetDIBBitmap(HBITMAP hBitmap, PBYTE& pBits, UINT& nBitsSize, PBITMAPINFO& pBitmapInfo, UINT& nBitmapInfoSize)
{
	pBits = 0;
	pBitmapInfo = 0;

	BITMAPINFO  bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	CDC dcSrc;
	dcSrc.CreateCompatibleDC(NULL);

	TRY
	{
		if (GetDIBits(dcSrc, hBitmap, 0, 0, NULL, (BITMAPINFO*)&bmi, DIB_RGB_COLORS) == 0)
			AfxThrowMemoryException();

		nBitsSize = bmi.bmiHeader.biSizeImage != 0? bmi.bmiHeader.biSizeImage: bmi.bmiHeader.biHeight * ((bmi.bmiHeader.biWidth * 4 + 3) & ~3);
		pBits = (PBYTE)malloc(nBitsSize);
		if (pBits  == NULL)
			AfxThrowMemoryException();

		nBitmapInfoSize = sizeof(BITMAPINFOHEADER) + sizeof(DWORD)*3;
		if ((pBitmapInfo = (PBITMAPINFO)malloc(nBitmapInfoSize)) == NULL)
			AfxThrowMemoryException();

		MEMCPY_S(pBitmapInfo, &bmi, sizeof(BITMAPINFOHEADER));

		if (GetDIBits(dcSrc, hBitmap, 0, bmi.bmiHeader.biHeight, pBits, pBitmapInfo, DIB_RGB_COLORS) == NULL)
			AfxThrowMemoryException();
	}
	CATCH (CMemoryException, e)
	{
		TRACE(_T("Failed -- Memory exception thrown."));
		return FALSE;
	}
	END_CATCH

	return TRUE;
}

HBITMAP CXTPImageManagerIcon::ReadDIBBitmap(CArchive& ar)
{

	PBITMAPINFO pBitmapInfo = 0;
	PBYTE pBits = 0;

	DWORD dwCount = (DWORD) ar.ReadCount();
	DWORD dwBitsCount = 0;

	if (dwCount > 0)
	{
		pBitmapInfo = (PBITMAPINFO)malloc(dwCount);
		ar.Read(pBitmapInfo, dwCount);

		dwBitsCount =  (DWORD) ar.ReadCount();
		pBits = (PBYTE)malloc(dwBitsCount);
		ar.Read(pBits, dwBitsCount);
	} else
	{
		return NULL;
	}

	CDC dcSrc;
	dcSrc.CreateCompatibleDC(NULL);

	BYTE* pDest = NULL;
	HBITMAP hBmp = CreateDIBSection(dcSrc, pBitmapInfo, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);

	MEMCPY_S(pDest, pBits, dwBitsCount);

	FREE(pBits);
	FREE(pBitmapInfo);

	return hBmp;
}

void CXTPImageManagerIcon::WriteDIBBitmap(CArchive& ar, HBITMAP hBitmap)
{
	PBYTE pBits = 0;
	PBITMAPINFO pBitmapInfo = 0;
	UINT nBitsSize;
	UINT nBitmapInfoSize;

	if (GetDIBBitmap(hBitmap, pBits, nBitsSize, pBitmapInfo, nBitmapInfoSize))
	{
		ar.WriteCount(nBitmapInfoSize);
		ar.Write(pBitmapInfo, nBitmapInfoSize);
		ar.WriteCount(nBitsSize);
		ar.Write(pBits, nBitsSize);
	} else
	{
		ar.WriteCount(0);
	}
	FREE(pBits);
	FREE(pBitmapInfo);
}

void CXTPImageManagerIcon::SerializeIcon(CXTPImageManagerIconHandle& hIcon, CArchive& ar)
{
	if (ar.IsStoring())
	{
		ICONINFO info;

		if (!hIcon.IsEmpty())
		{
			if (hIcon.IsAlpha())
			{
				WriteDIBBitmap(ar, hIcon.GetBitmap());
				ar.WriteCount(0);
			}
			else if (GetIconInfo(hIcon.GetIcon(), &info))
			{
				WriteDIBBitmap(ar, info.hbmColor);
				WriteDIBBitmap(ar, info.hbmMask);
			}
			else
			{
				ar.WriteCount(0);
				ar.WriteCount(0);
			}
		}
		else
		{
			ar.WriteCount(0);
			ar.WriteCount(0);
		}
	} else
	{
		hIcon.Clear();

		HBITMAP hbmColor = ReadDIBBitmap(ar);
		HBITMAP hbmMask = ReadDIBBitmap(ar);

		if (hbmColor)
		{
			if (hbmMask == 0)
			{
				hIcon = hbmColor;
				hbmColor = 0;
			} else
			{
				BITMAP bmp;
				::GetObject(hbmColor, sizeof (BITMAP), &bmp);

				CImageList il;
				il.Create(bmp.bmWidth, bmp.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 1);
				il.Add(CBitmap::FromHandle(hbmColor), hbmMask == 0? (CBitmap*)NULL: CBitmap::FromHandle(hbmMask));
				hIcon = il.ExtractIcon(0);
			}

			if (hbmColor) ::DeleteObject(hbmColor);
			if (hbmMask) ::DeleteObject(hbmMask);
		}
	}

}

void CXTPImageManagerIcon::Serialize(CArchive& ar)
{
	BOOL nShema = _XTP_SCHEMA_CURRENT;

	if (ar.IsStoring())
	{
		ar << m_nHeight;
		ar << nShema;

	}
	else
	{
		ar >> m_nHeight;
		ar >> nShema;
	}
	SerializeIcon(m_hIcon, ar);
	SerializeIcon(m_hDisabled, ar);
	SerializeIcon(m_hHot, ar);

	if (nShema > _XTP_SCHEMA_875)
	{
		SerializeIcon(m_hChecked, ar);
	}

}

void CXTPImageManagerIcon::DrawAlphaBitmap(CDC* pDC, CPoint pt, HBITMAP hBitmap, CSize szDest, CSize szSrc)
{
	if (!XTPImageManager()->m_pfnAlphaBlend)
		return;

	if (szSrc == CSize(0))
		szSrc = szDest;

	CXTPCompatibleDC dc(NULL, CBitmap::FromHandle(hBitmap));

	BLENDFUNCTION bf;
	ZeroMemory(&bf, sizeof(BLENDFUNCTION));
	bf.AlphaFormat  = 0x01 ;
	bf.BlendOp =AC_SRC_OVER;
	bf.SourceConstantAlpha = 255;

	XTPImageManager()->m_pfnAlphaBlend(pDC->GetSafeHdc(), pt.x, pt.y,
		szDest.cx, szDest.cy,
		dc, 0, 0, szSrc.cx, szSrc.cy, bf);

}
CXTPImageManager* CXTPImageManagerIcon::GetImageManager()
{
	if (m_pIconSet)
	{
		ASSERT(m_pIconSet->m_pManager);
		return m_pIconSet->m_pManager;
	}

	return XTPImageManager();
}

void CXTPImageManagerIcon::DrawMono(CDC* pDC, CPoint pt, CXTPImageManagerIconHandle& hIcon, COLORREF clrBrush)
{
	if (hIcon.IsAlpha())
		return;

	if (GetImageManager()->m_bDrawReverted)
	{
		CSize szIcon(m_nWidth, m_nHeight);
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(pDC, m_nWidth, m_nHeight);
		{
			CXTPCompatibleDC dc(pDC, &bmp);
			dc.FillSolidRect(0, 0, m_nWidth, m_nHeight, 0xFFFFFF);
			DrawIconEx(dc.GetSafeHdc(), 0, 0, hIcon, szIcon.cx, szIcon.cy, 0, 0, DI_NORMAL);
		}
		pDC->DrawState(pt, szIcon, bmp, (UINT)DSS_MONO, CBrush(clrBrush));

	}
	else
	{
		pDC->DrawState (pt, 0, hIcon, (UINT)DSS_MONO, CBrush(clrBrush));
	}

}


void CXTPImageManagerIcon::Draw(CDC* pDC, CPoint pt, CXTPImageManagerIconHandle& hIcon, CSize szIcon /*= 0*/)
{
	if (hIcon.IsEmpty())
		return;

	if (szIcon.cx == 0)
		szIcon.cx = m_nWidth;

	if (szIcon.cy == 0)
		szIcon.cy = m_nHeight * szIcon.cx / m_nWidth;


	if (!hIcon.IsAlpha())
	{
		if (GetImageManager()->m_bDrawReverted)
		{
			pt.x += szIcon.cx;
			szIcon.cx = -szIcon.cx;
		}

		if (pDC->IsPrinting())
		{
			CImageList il;
			il.Create(szIcon.cx, szIcon.cy, ILC_COLOR24|ILC_MASK, 1, 0);
			il.Add(hIcon.GetIcon());
			il.Draw(pDC, 0, pt, ILD_NORMAL);
		}
		else
		{
			DrawIconEx(pDC->GetSafeHdc(), pt.x, pt.y, hIcon.GetIcon(), szIcon.cx, szIcon.cy, 0, 0, DI_NORMAL);
		}
	}
	else if (hIcon.PreMultiply())
	{
		DrawAlphaBitmap(pDC, pt, hIcon.m_hAlphaBits, szIcon, CSize(m_nWidth, m_nHeight));
	}
}



//////////////////////////////////////////////////////////////////////////
// CXTPImageManagerIconSet

CXTPImageManagerIconSet::CXTPImageManagerIconSet(UINT nID, CXTPImageManager* pManager)
	: m_nID(nID)
{
	ASSERT(pManager);
	m_pManager = pManager;

	EnableAutomation();
}

CXTPImageManagerIconSet::~CXTPImageManagerIconSet()
{
	RemoveAll();
}

void CXTPImageManagerIconSet::RemoveAll()
{
	UINT nWidth;
	CXTPImageManagerIcon* pIcon;

	POSITION pos = m_mapIcons.GetStartPosition();
	while(pos)
	{
		m_mapIcons.GetNextAssoc(pos, nWidth, pIcon);
		pIcon->InternalRelease();
	}
	m_mapIcons.RemoveAll();
}

CXTPImageManagerIcon* CXTPImageManagerIconSet::CreateIcon(UINT nWidth)
{
	CXTPImageManagerIcon* pIcon = 0;
	if (m_mapIcons.Lookup(nWidth, pIcon))
	{
		ASSERT(pIcon->m_nID == m_nID);
		ASSERT(pIcon->m_nWidth == nWidth);
	}
	else
	{
		pIcon = new CXTPImageManagerIcon(m_nID, nWidth, 15, this);
		m_mapIcons.SetAt(nWidth, pIcon);
	}

	ASSERT(pIcon);

	return pIcon;
}

void CXTPImageManagerIconSet::SetIcon(CXTPImageManagerIconHandle hIcon, UINT nWidth)
{
	CXTPImageManagerIcon* pIcon = CreateIcon(nWidth);
	pIcon->SetIcon(hIcon);
}

void CXTPImageManagerIconSet::SetDisabledIcon(CXTPImageManagerIconHandle hIcon, UINT nWidth)
{
	CXTPImageManagerIcon* pIcon = CreateIcon(nWidth);
	pIcon->SetDisabledIcon(hIcon);
}

void CXTPImageManagerIconSet::SetHotIcon(CXTPImageManagerIconHandle hIcon, UINT nWidth)
{
	CXTPImageManagerIcon* pIcon = CreateIcon(nWidth);
	pIcon->SetHotIcon(hIcon);
}

void CXTPImageManagerIconSet::SetCheckedIcon(CXTPImageManagerIconHandle hIcon, UINT nWidth)
{
	CXTPImageManagerIcon* pIcon = CreateIcon(nWidth);
	pIcon->SetCheckedIcon(hIcon);
}


CXTPImageManagerIcon* CXTPImageManagerIconSet::GetIcon(UINT nWidth, BOOL bScaled)
{
	ASSERT(m_mapIcons.GetCount() > 0);

	CXTPImageManagerIcon* pIcon;
	if (m_mapIcons.Lookup(nWidth, pIcon))
	{
		return pIcon;
	}
	if (!bScaled)
		return NULL;

	int nDelta = 0;

	POSITION pos = m_mapIcons.GetStartPosition();
	while(pos)
	{
		UINT nWidthScale;
		CXTPImageManagerIcon* pIconScale;

		m_mapIcons.GetNextAssoc(pos, nWidthScale, pIconScale);
		if (nDelta == 0 || abs(int(nWidthScale - nWidth)) < nDelta)
		{
			pIcon = pIconScale;
			nDelta = abs(int(nWidthScale - nWidth));
		}
	}
	ASSERT(pIcon != 0);

	if (pIcon->IsAlpha() || (nWidth == 0))
		return pIcon;

	CXTPImageManagerIcon* pIconScale = pIcon->Scale(nWidth);
	if (!pIconScale)
		return pIcon;

	pIconScale->m_bScaled = TRUE;
	m_mapIcons.SetAt(nWidth, pIconScale);
	return pIconScale;
}


void CXTPImageManagerIconSet::RefreshAll(void)
{
	POSITION pos = m_mapIcons.GetStartPosition();
	UINT nWidth;
	CXTPImageManagerIcon* pIcon;

	while(pos)
	{
		m_mapIcons.GetNextAssoc(pos, nWidth, pIcon);

		if (pIcon->m_bScaled)
		{
			m_mapIcons.RemoveKey(nWidth);
			pIcon->InternalRelease();

		} else
		{
			pIcon->Refresh();
		}
	}
}



//////////////////////////////////////////////////////////////////////////
// CXTPImageManager

CXTPImageManager::CXTPImageManager(void)
{
	m_clrMask = (COLORREF)-1;

	m_nCustomID = 0x10AAA;


	m_hMsImgDll = ::LoadLibrary( _T("msimg32.dll") );
	m_pfnAlphaBlend = 0;

	if (m_hMsImgDll)
	{
		m_pfnAlphaBlend = (PFNALPHABLEND)::GetProcAddress(m_hMsImgDll, "AlphaBlend");
	}
	EnableAutomation();

	m_bDrawReverted = FALSE;


}

CXTPImageManager::~CXTPImageManager(void)
{
	RemoveAll();

	if (m_hMsImgDll)
	{
		::FreeLibrary( m_hMsImgDll );
	}
}

BOOL CXTPImageManager::LoadToolbar(UINT nIDResourceToolBar, UINT*& pItems, int& nCount, CSize& szIcon)
{
	LPCTSTR lpszResourceName = MAKEINTRESOURCE(nIDResourceToolBar);

	struct CToolBarData
	{
		WORD wVersion;
		WORD wWidth;
		WORD wHeight;
		WORD wItemCount;
		WORD* items()
		{ return (WORD*)(this+1); }
	};

	ASSERT(lpszResourceName != NULL);


	// determine location of the bitmap in resource fork
	HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_TOOLBAR);
	HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_TOOLBAR);
	if (hRsrc == NULL)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
	if (hGlobal == NULL)
		return FALSE;

	CToolBarData* pData = (CToolBarData*)LockResource(hGlobal);
	if (pData == NULL)
		return FALSE;

	ASSERT(pData->wVersion == 1);

	pItems = new UINT[pData->wItemCount];

	nCount = 0;
	for (int i = 0; i < pData->wItemCount; i++)
		if (pData->items()[i]) pItems[nCount++] = pData->items()[i];

	szIcon = CSize(pData->wWidth, pData->wHeight);

	UnlockResource(hGlobal);
	FreeResource(hGlobal);
	return TRUE;
}

BOOL CXTPImageManager::SetIcons(UINT nIDResourceToolBar, CBitmap& bmpIcons, XTPImageState imageState)
{
	UINT* pItems = NULL;
	CSize szIcon(0);
	int nCount = 0;
	BOOL bResult = LoadToolbar(nIDResourceToolBar, pItems, nCount, szIcon);

	if (bResult && !SetIcons(bmpIcons, pItems, nCount, szIcon, imageState))
		bResult = FALSE;

	if (pItems)
	{
		delete[] pItems;
	}

	return bResult;
}
BOOL CXTPImageManager::SetIcons(UINT nIDResourceToolBar, UINT nIDResourceBitmap, XTPImageState imageState)
{
	UINT* pItems = NULL;
	CSize szIcon(0);
	int nCount = 0;
	BOOL bResult = LoadToolbar(nIDResourceToolBar, pItems, nCount, szIcon);

	if (bResult && !SetIcons(nIDResourceBitmap, pItems, nCount, CSize(0, 0), imageState))
		bResult = FALSE;

	if (pItems)
	{
		delete[] pItems;
	}

	return bResult;

}


BOOL CXTPImageManager::SetIcons(UINT nIDResourceToolBar, XTPImageState imageState)
{
	return SetIcons(nIDResourceToolBar, nIDResourceToolBar, imageState);
}

BOOL CXTPImageManager::SetIcons(UINT nIDResourceToolBar, CImageList& imlIcons, XTPImageState imageState)
{
	UINT* pItems = NULL;
	CSize szIcon(0);
	int nCount = 0;
	BOOL bResult = LoadToolbar(nIDResourceToolBar, pItems, nCount, szIcon);

	if (bResult && !SetIcons(imlIcons, pItems, nCount, szIcon, imageState))
		bResult = FALSE;

	if (pItems)
	{
		delete[] pItems;
	}

	return bResult;

}



BOOL CXTPImageManager::SetIcon(CXTPImageManagerIconHandle hIcon, UINT nIDCommand, CSize szIcon, XTPImageState imageState)
{
	if (hIcon.IsEmpty())
		return FALSE;

	if (szIcon == CSize(0))
	{
		szIcon = hIcon.GetExtent();
	}
	if (szIcon == CSize(0))
		return FALSE;


	CXTPImageManagerIconSet* pIconSet;
	if (m_mapImages.Lookup(nIDCommand, pIconSet))
	{
		if (imageState == xtpImageNormal) pIconSet->SetIcon(hIcon, szIcon.cx);
		if (imageState == xtpImageDisabled) pIconSet->SetDisabledIcon(hIcon, szIcon.cx);
		if (imageState == xtpImageHot) pIconSet->SetHotIcon(hIcon, szIcon.cx);
		if (imageState == xtpImageChecked) pIconSet->SetCheckedIcon(hIcon, szIcon.cx);
	}
	else if (imageState == xtpImageNormal)
	{
		pIconSet = new CXTPImageManagerIconSet(nIDCommand, this);
		pIconSet->SetIcon(hIcon, szIcon.cx);
		m_mapImages.SetAt(nIDCommand, pIconSet);
	}
	else return FALSE;

	return TRUE;
}

BOOL CXTPImageManager::SetIcon(UINT nIDResourceIcon, UINT nIDCommand, CSize szIcon, XTPImageState imageState)
{
	LPCTSTR lpszResourceName = MAKEINTRESOURCE(nIDResourceIcon);

	// determine location of the bitmap in resource fork
	HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_GROUP_ICON);
	HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_GROUP_ICON);
	if (hRsrc != NULL)
	{
		// Load and Lock to get a pointer to a GRPICONDIR
		HGLOBAL hGlobal = LoadResource( hInst, hRsrc );

		CXTPImageManagerIcon::GRPICONDIR* lpGrpIconDir = (CXTPImageManagerIcon::GRPICONDIR*)
			LockResource( hGlobal );

		for (WORD i = 0; i < lpGrpIconDir->idCount; i++)
		{
			if (!SetIcon(lpGrpIconDir->idEntries[i].nID, nIDCommand, CSize(lpGrpIconDir->idEntries[i].bWidth,
				lpGrpIconDir->idEntries[i].bHeight), imageState))
				return FALSE;
		}
	}
	else
	{
		HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_ICON);
		HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_ICON);

		if (hRsrc == NULL)
			return FALSE;

		HGLOBAL hGlobal = LoadResource( hInst, hRsrc );
		CXTPImageManagerIcon::ICONIMAGE* lpIconImage = (CXTPImageManagerIcon::ICONIMAGE*)LockResource( hGlobal );

		if(lpIconImage == NULL )
			 return FALSE;

		DWORD dwResourceSize = SizeofResource(hInst, hRsrc);

		CXTPImageManagerIconHandle hIcon;

		if (lpIconImage->icHeader.biBitCount == 32)
		{
			CDC dcSrc;
			dcSrc.CreateCompatibleDC(0);

			BITMAPINFOHEADER* pBitmapInfo = (BITMAPINFOHEADER*)_alloca(sizeof(BITMAPINFOHEADER) + sizeof(DWORD) * 3);
			*pBitmapInfo = lpIconImage->icHeader;

			pBitmapInfo->biHeight /= 2;
			pBitmapInfo->biSizeImage = pBitmapInfo->biHeight * pBitmapInfo->biWidth * 4;

			BYTE* pDest = NULL;
			HBITMAP hBitmap = CreateDIBSection(dcSrc, (BITMAPINFO*)pBitmapInfo, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);

			if (pDest == NULL || hBitmap == NULL)
				return FALSE;

			ASSERT(pBitmapInfo->biSizeImage  <= dwResourceSize - sizeof(BITMAPINFOHEADER));

			MEMCPY_S(pDest, &lpIconImage->icColors, pBitmapInfo->biSizeImage);

			hIcon = hBitmap; // NOTE: it call DeleteObejct in destructor

		}
		else
		{
			hIcon = CreateIconFromResourceEx( (PBYTE)lpIconImage, dwResourceSize, TRUE,
				0x00030000,szIcon.cx, szIcon.cy, LR_DEFAULTCOLOR);
		}

		if (hIcon.IsEmpty())
			return FALSE;

		return SetIcon(hIcon, nIDCommand, szIcon, imageState);

	}
	return TRUE;
}


BOOL CXTPImageManager::SetIcons(CImageList& imlIcons, UINT* pCommands, int nCount, CSize szIcon, XTPImageState imageState)
{
	if (!imlIcons.GetSafeHandle())
		return FALSE;

	if ((nCount == 0) && (pCommands == 0))
	{
		nCount = imlIcons.GetImageCount();
	}

	if (szIcon == CSize(0) && nCount > 0)
	{
		IMAGEINFO imageInfo;
		imlIcons.GetImageInfo(0, &imageInfo);
		szIcon = CRect(imageInfo.rcImage).Size();
	}

	BOOL bIncludeAll = imlIcons.GetImageCount() == nCount;
	int j = 0;

	for (int i = 0; i < nCount; i++)
	{
		if ((pCommands == NULL) || bIncludeAll || (pCommands[i] != 0))
		{
			HICON hIcon = imlIcons.ExtractIcon(j);
			if (hIcon)
			{
				SetIcon(hIcon, pCommands? pCommands[i]: i, szIcon, imageState);
				DestroyIcon(hIcon);
			}
			else
			{
				return FALSE;
			}
			j++;
		}
	}
	return TRUE;
}


BOOL CXTPImageManager::SplitBitmap(HBITMAP hbmSource, int nCount, HBITMAP* pDest)
{
	ASSERT(pDest != NULL);
	ASSERT(sizeof(int) == 4);

	BITMAPINFO  bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	CDC dcSrc;
	dcSrc.CreateCompatibleDC(NULL);

	PINT pBits = 0;
	PBITMAPINFO pBitmapInfo = 0;

	BOOL bResult = TRUE;

	TRY
	{
		if (GetDIBits(dcSrc, hbmSource, 0, 0, NULL, (BITMAPINFO*)&bmi, DIB_RGB_COLORS) == 0)
			AfxThrowMemoryException();

		UINT nSize = bmi.bmiHeader.biSizeImage != 0? bmi.bmiHeader.biSizeImage: bmi.bmiHeader.biHeight * ((bmi.bmiHeader.biWidth * 4 + 3) & ~3);
		pBits = (PINT)malloc(nSize);
		if (pBits  == NULL)
			AfxThrowMemoryException();

		if ((pBitmapInfo = (PBITMAPINFO)malloc(sizeof(BITMAPINFOHEADER) + sizeof(DWORD)*3)) == NULL)
			AfxThrowMemoryException();

		MEMCPY_S(pBitmapInfo, &bmi, sizeof(BITMAPINFOHEADER));

		if (GetDIBits(dcSrc, hbmSource, 0, bmi.bmiHeader.biHeight, pBits, pBitmapInfo, DIB_RGB_COLORS) == NULL)
			AfxThrowMemoryException();

		ASSERT(pBitmapInfo->bmiHeader.biWidth % nCount == 0);

		pBitmapInfo->bmiHeader.biWidth /= nCount;
		pBitmapInfo->bmiHeader.biSizeImage /= nCount;

		PINT* pDestBits = new PINT[nCount];
		for (int i = 0; i < nCount; i++)
		{
			pDest[i] = CreateDIBSection(dcSrc, pBitmapInfo, DIB_RGB_COLORS, (void**)&pDestBits[i], NULL, 0);
			if (!pDest[i])
			{
				for (int j = 0; j < i; j++)
					DeleteObject(pDest[j]);

				delete[] pDestBits;

				AfxThrowMemoryException();
			}
		}

		PINT pBitsIter = pBits;

		for (int y = 0; y < pBitmapInfo->bmiHeader.biHeight; y++)
		{
			for (int i = 0; i < nCount; i++)
			{
				for (int x = 0; x < pBitmapInfo->bmiHeader.biWidth; x++)
				{
					*pDestBits[i]++ = *pBitsIter++;
				}
			}
		}
		delete[] pDestBits;

	}
	CATCH (CMemoryException, e)
	{
		TRACE(_T("Failed -- Memory exception thrown."));
		bResult = FALSE;
	}
	END_CATCH

	FREE(pBits);
	FREE(pBitmapInfo);

	return bResult;

}

COLORREF CXTPImageManager::GetBitmapMaskColor(CBitmap& bmpIcons, CPoint pt)
{
	CXTPCompatibleDC dc(NULL, &bmpIcons);
	return dc.GetPixel(pt);
}

COLORREF CXTPImageManager::GetBitmapMaskColor(UINT nIDBitmap, CPoint pt)
{
	CBitmap bmp;
	VERIFY(bmp.LoadBitmap(nIDBitmap));
	return GetBitmapMaskColor(bmp, pt);
}


HBITMAP CXTPImageManager::ResampleAlphaLayer(UINT nIDBitmap, COLORREF clrMask)
{
	HBITMAP hbmAlpha = CXTPImageManagerIcon::LoadAlphaBitmap(nIDBitmap);
	if (!hbmAlpha)
		return 0;

	CBitmap bmp;
	bmp.Attach(hbmAlpha);

	return CXTPImageManager::ResampleAlphaLayer(bmp, clrMask);
}

HBITMAP CXTPImageManager::ResampleAlphaLayer(HBITMAP bmpAlpha, COLORREF clrMask)
{
	CDC dcSrc;
	dcSrc.CreateCompatibleDC(NULL);

	PBYTE pBits = 0;
	PBITMAPINFO pBitmapInfo = 0;
	UINT nSize;

	if (!CXTPImageManagerIcon::GetBitmapBits(dcSrc, bmpAlpha, pBitmapInfo, (LPVOID&)pBits, nSize))
		return 0;

	PBYTE pTarget = NULL;
	PBYTE pSource = pBits;
	pBitmapInfo->bmiHeader.biBitCount = 24;
	pBitmapInfo->bmiHeader.biSizeImage = 0;
	pBitmapInfo->bmiHeader.biCompression = BI_RGB;

	HBITMAP hBitmapResult = CreateDIBSection(dcSrc, pBitmapInfo, DIB_RGB_COLORS, (void**)&pTarget, NULL, 0);

	if (pTarget && pSource && hBitmapResult)
	{
		BYTE byMaskRed = GetRValue(clrMask);
		BYTE byMaskGreen = GetGValue(clrMask);
		BYTE byMaskBlue = GetBValue(clrMask);

		int nCount = pBitmapInfo->bmiHeader.biHeight * pBitmapInfo->bmiHeader.biWidth;

		for (; nCount--;)
		{
			int iAlpha = pSource[3];

			if (iAlpha < 120)
			{
				pTarget[0] = byMaskRed;
				pTarget[1] = byMaskGreen;
				pTarget[2] = byMaskBlue;
			}
			else
			{
				pTarget[0] = (BYTE)((pSource[0] * (255 - iAlpha) + pSource[0] * iAlpha) >> 8);
				pTarget[1] = (BYTE)((pSource[1] * (255 - iAlpha) + pSource[1] * iAlpha) >> 8);
				pTarget[2] = (BYTE)((pSource[2] * (255 - iAlpha) + pSource[2] * iAlpha) >> 8);
			}
			pTarget += 3;
			pSource += 4;
		}
	}

	FREE(pBits);
	FREE(pBitmapInfo);

	return hBitmapResult;
}

BOOL CXTPImageManager::SetIcons(CBitmap& bmpIcons, UINT* pCommands, int nCount, CSize szIcon, XTPImageState imageState, BOOL bAlpha)
{
	if (szIcon.cx == 0)
		return FALSE;

	if (bAlpha && !IsAlphaIconsSupported() && m_bAutoResample)
	{
		HBITMAP hBitmap = ResampleAlphaLayer(bmpIcons, m_clrMask == COLORREF(-1)? RGB(0, 0xFF, 0): m_clrMask);
		if (!hBitmap)
			return FALSE;

		CBitmap bmpResample;
		bmpResample.Attach(hBitmap);
		return SetIcons(bmpResample, pCommands, nCount, szIcon, imageState, FALSE);
	}

	if (bAlpha)
	{
		BITMAP bmpInfo;
		bmpIcons.GetBitmap(&bmpInfo);
		if (bmpInfo.bmWidth % szIcon.cx != 0)
			return FALSE;

		int nIconCount = bmpInfo.bmWidth / szIcon.cx;

		HBITMAP* pDestBitmap = new HBITMAP[nIconCount];

		if (!SplitBitmap(bmpIcons, nIconCount, pDestBitmap))
		{
			delete[] pDestBitmap;
			return FALSE;
		}
		BOOL bIgnoreNulls = (nIconCount == nCount);

		int j = 0;

		for (int i = 0; i < nCount; i++)
		{
			if ((!pCommands || bIgnoreNulls || pCommands[i]) && j < nIconCount)
			{

				SetIcon(pDestBitmap[j], pCommands? pCommands[i]: i, szIcon, imageState);
				j++;
			}
		}

		for (j = 0; j < nIconCount; j++)
		{
			DeleteObject(pDestBitmap[j]);
		}

		delete[] pDestBitmap;


		return TRUE;

	}
	else
	{
		CImageList imgList;


		if (!imgList.Create(szIcon.cx, szIcon.cy, ILC_COLOR24|ILC_MASK, 0, 1))
			return FALSE;

		COLORREF clrMask = m_clrMask != (COLORREF)-1? m_clrMask: GetBitmapMaskColor(bmpIcons);
		imgList.Add(&bmpIcons, clrMask);

		return SetIcons(imgList, pCommands, nCount, szIcon, imageState);
	}

}


BOOL CXTPImageManager::SetIcons(UINT nIDResourceBitmap, UINT* pCommands, int nCount, CSize szIcon, XTPImageState imageState)
{
	CBitmap bmpIcons;

	BOOL bAlphaBitmap = IsAlphaBitmapResource(nIDResourceBitmap);

	if (bAlphaBitmap)
	{
		HBITMAP hBmp = CXTPImageManagerIcon::LoadAlphaBitmap(nIDResourceBitmap);

		if (!hBmp)
			return FALSE;

		bmpIcons.Attach(hBmp);
	}
	else
	{
		if (!bmpIcons.LoadBitmap(nIDResourceBitmap))
			return FALSE;

	}

	if (szIcon == CSize(0))
	{
		BITMAP bmpInfo;
		bmpIcons.GetBitmap(&bmpInfo);
		szIcon = CSize(bmpInfo.bmWidth / nCount, bmpInfo.bmHeight);
	}

	if (nCount == 0)
	{
		BITMAP bmpInfo;
		bmpIcons.GetBitmap(&bmpInfo);
		nCount = bmpInfo.bmWidth / szIcon.cx;
	}

	return SetIcons(bmpIcons, pCommands, nCount, szIcon, imageState, bAlphaBitmap);
}

BOOL CXTPImageManager::IsAlphaBitmapResource(UINT nIDResourceBitmap)
{

	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(nIDResourceBitmap), RT_BITMAP);
	HRSRC hResource = ::FindResource(hInst, MAKEINTRESOURCE(nIDResourceBitmap), RT_BITMAP);
	if (hResource == NULL)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(hInst, hResource);
	if (hGlobal == NULL)
		return FALSE;

	LPBITMAPINFOHEADER pBitmapInfoHeader = (LPBITMAPINFOHEADER)::LockResource(hGlobal);
	ASSERT(pBitmapInfoHeader != NULL);
	BOOL bAlpahBitmap = pBitmapInfoHeader->biBitCount == 32;
	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	return (bAlpahBitmap);
}

UINT CXTPImageManager::AddCustomIcon(CXTPImageManagerIconHandle hIcon)
{
	CSize szIcon = hIcon.GetExtent();
	m_nCustomID++;

	CXTPImageManagerIconSet* pIconSet = new CXTPImageManagerIconSet(m_nCustomID, this);
	pIconSet->SetIcon(hIcon, szIcon.cx);
	m_mapImages.SetAt(m_nCustomID, pIconSet);

	return m_nCustomID;
}

void CXTPImageManager::CopyImage(UINT nCommand)
{
	CMemFile memFile;
	CArchive ar (&memFile,CArchive::store);

	Serialize(nCommand, ar);
	ar.Flush();

	DWORD dwCount = (DWORD)memFile.GetPosition();
	BYTE* pControls = memFile.Detach();

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwCount);
	if(hGlobal != NULL)
	{
		LPVOID lpData = GlobalLock(hGlobal);

		MEMCPY_S(lpData, pControls, dwCount);

		GlobalUnlock(hGlobal);
		SetClipboardData(m_nImageClipFormat, hGlobal);
	}

	ar.Close();
	memFile.Close();
	free(pControls);
}



UINT CXTPImageManager::PasteCustomImage(COleDataObject& data)
{
	HGLOBAL hGlobal = data.GetGlobalData (m_nImageClipFormat);

	if (hGlobal == NULL) return 0;

	try
	{
		CSharedFile globFile;
		globFile.SetHandle(hGlobal,FALSE);
		CArchive ar (&globFile,CArchive::load);

		m_nCustomID++;
		Serialize(m_nCustomID, ar);

		ar.Close();

	}
	catch (COleException* pEx)
	{
		pEx->Delete ();
		return 0;
	}
	catch (CArchiveException* pEx)
	{
		pEx->Delete ();
		return 0;
	}
	return m_nCustomID;
}



CXTPImageManagerIcon* CXTPImageManager::GetImage(UINT nCommand, int nWidth)
{
	CXTPImageManagerIconSet* pIconSet;
	if (m_mapImages.Lookup(nCommand, pIconSet))
	{
		return pIconSet->GetIcon(nWidth);

	}
	return NULL;
}

BOOL CXTPImageManager::Lookup(UINT nCommand)
{
	return GetIconSet(nCommand) != NULL;
}

CXTPImageManagerIconSet* CXTPImageManager::GetIconSet(UINT nCommand)
{
	CXTPImageManagerIconSet* pIconSet;
	if (m_mapImages.Lookup(nCommand, pIconSet))
	{
		return pIconSet;

	}
	return NULL;

}

void CXTPImageManager::RemoveIcon(UINT nIDCommand)
{
	CXTPImageManagerIconSet* pIconSet;
	if (m_mapImages.Lookup(nIDCommand, pIconSet))
	{
		m_mapImages.RemoveKey(nIDCommand);
		pIconSet->InternalRelease();
	}
}

void CXTPImageManager::RemoveAll(void)
{
	POSITION pos = m_mapImages.GetStartPosition();
	UINT nID;
	CXTPImageManagerIconSet* pIconSet;
	while(pos)
	{
		m_mapImages.GetNextAssoc(pos, nID, pIconSet);
		pIconSet->InternalRelease();
	}
	m_mapImages.RemoveAll();
}

void CXTPImageManager::RefreshAll(void)
{
	POSITION pos = m_mapImages.GetStartPosition();
	UINT nID;
	CXTPImageManagerIconSet* pIconSet;

	while(pos)
	{
		m_mapImages.GetNextAssoc(pos, nID, pIconSet);
		pIconSet->RefreshAll();
	}
}

COLORREF CXTPImageManager::SetMaskColor(COLORREF clrMask)
{
	COLORREF clrReturn = m_clrMask;
	m_clrMask = clrMask;
	return clrReturn;
}
COLORREF CXTPImageManager::GetMaskColor()
{
	return m_clrMask;
}

BOOL CXTPImageManager::IsAlphaIconsSupported()
{
	return m_pfnAlphaBlend != NULL;

}
BOOL CXTPImageManager::IsAlphaIconsImageListSupported()
{
	BOOL bAlphaIcons = FALSE;
	HMODULE hComCtlDll = LoadLibrary(_T("comctl32.dll"));

	if (hComCtlDll)
	{
		typedef HRESULT (CALLBACK *PFNDLLGETVERSION)(DLLVERSIONINFO*);

		PFNDLLGETVERSION pfnDllGetVersion = (PFNDLLGETVERSION)GetProcAddress(hComCtlDll, "DllGetVersion");

		if (pfnDllGetVersion)
		{
			DLLVERSIONINFO dvi;
			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);

			HRESULT hRes = (*pfnDllGetVersion)(&dvi);

			if (SUCCEEDED(hRes) && dvi.dwMajorVersion >= 6)
				bAlphaIcons = TRUE;
		}

		FreeLibrary(hComCtlDll);
	}
	return bAlphaIcons;
}

void CXTPImageManager::DoPropExchange(UINT nCommand, CXTPPropExchange* pPX)
{
	if (nCommand > m_nCustomID) m_nCustomID = nCommand;

	const LPCTSTR lpszCheck = _T("CXTPImageManagerIcons");
	CString strCheck = lpszCheck;

	if (pPX->IsStoring())
	{
		PX_String(pPX, _T("CheckSum"), strCheck, lpszCheck);

		ASSERT(nCommand > 0);

		CXTPImageManagerIconSet* pIconSet = GetIconSet(nCommand);
		ASSERT(pIconSet);

		UINT nWidth;
		CXTPImageManagerIcon* pIcon;
		DWORD dwCount = 0;

		POSITION pos = pIconSet->m_mapIcons.GetStartPosition();
		while (pos)
		{
			pIconSet->m_mapIcons.GetNextAssoc(pos, nWidth, pIcon);
			if (pIcon->m_bScaled)
				continue;

			dwCount++;
		}

		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Icon")));
		POSITION posEnum = pEnumerator->GetPosition(dwCount);

		pos = pIconSet->m_mapIcons.GetStartPosition();
		while (pos)
		{
			pIconSet->m_mapIcons.GetNextAssoc(pos, nWidth, pIcon);
			if (pIcon->m_bScaled)
				continue;

			CXTPPropExchangeSection secIcon(pEnumerator->GetNext(posEnum));

			PX_Long(&secIcon, _T("Width"), (long&)nWidth);
			PX_Serialize(&secIcon, _T("Data"), pIcon);
		}
	}
	else
	{
		PX_String(pPX, _T("CheckSum"), strCheck, lpszCheck);

		if (strCheck != lpszCheck)
			AfxThrowArchiveException(CArchiveException::badClass);


		CXTPImageManagerIconSet* pIconSet;
		if (m_mapImages.Lookup(nCommand, pIconSet))
		{
			pIconSet->RemoveAll();
		} else
		{
			pIconSet = new CXTPImageManagerIconSet(nCommand, this);
			m_mapImages.SetAt(nCommand, pIconSet);
		}

		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Icon")));
		POSITION posEnum = pEnumerator->GetPosition();

		while(posEnum)
		{
			CXTPPropExchangeSection secIcon(pEnumerator->GetNext(posEnum));

			UINT nWidth = 16;
			PX_Long(&secIcon, _T("Width"), (long&)nWidth);

			CXTPImageManagerIcon* pIcon = new CXTPImageManagerIcon(nCommand, nWidth, 15, pIconSet);
			PX_Serialize(&secIcon, _T("Data"), pIcon);

			pIconSet->m_mapIcons.SetAt(nWidth, pIcon);
		}
	}
}

void CXTPImageManager::Serialize(UINT nCommand, CArchive& ar)
{
	CXTPPropExchangeArchive px(ar);
	DoPropExchange(nCommand, &px);
}



void CXTPImageManager::DoPropExchange(CXTPPropExchange* pPX)
{
	CMap<UINT, UINT, CXTPImageManagerIconSet*, CXTPImageManagerIconSet*>* pIcons = GetImages();

	CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("IconSet")));
	POSITION posEnum = pEnumerator->GetPosition((DWORD)pIcons->GetCount());

	if (pPX->IsStoring())
	{
		UINT nIDCommand;
		CXTPImageManagerIconSet* pIconSet;

		POSITION pos = pIcons->GetStartPosition();

		while (pos)
		{
			pIcons->GetNextAssoc(pos, nIDCommand, pIconSet);
			CXTPPropExchangeSection secIconSet(pEnumerator->GetNext(posEnum));
			
			PX_Long(&secIconSet, _T("Id"), (long&)nIDCommand);
			DoPropExchange(nIDCommand, &secIconSet);
		}
	}
	else
	{
		RemoveAll();
		
		while (posEnum)
		{
			CXTPPropExchangeSection secIconSet(pEnumerator->GetNext(posEnum));

			UINT nIDCommand = 0;
			PX_Long(&secIconSet, _T("Id"), (long&)nIDCommand);

			DoPropExchange(nIDCommand, &secIconSet);
		}
	}

}
void CXTPImageManager::Serialize(CArchive& ar)
{
	CXTPPropExchangeArchive px(ar);
	DoPropExchange(&px);
}


void CXTPImageManager::AddIcons(CXTPImageManager* pImageManager)
{
	if (pImageManager == 0)
		return;


	POSITION pos = pImageManager->m_mapImages.GetStartPosition();
	UINT nID;
	CXTPImageManagerIconSet* pIconSet;
	
	while(pos)
	{
		pImageManager->m_mapImages.GetNextAssoc(pos, nID, pIconSet);
		
		if (!Lookup(nID))		
		{
			m_mapImages.SetAt(nID, pIconSet);
			pIconSet->InternalAddRef();
		}
	}
	
}

