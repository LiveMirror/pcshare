// XTFunctions.cpp : global functions.
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
#include "XTPColorManager.h"

#include "XTResource.h"
#include "XTDefines.h"
#include "XTHelpers.h"
#include "XTGlobal.h"
#include "XTVC50Helpers.h"

#ifndef _XTP_INCLUDE_CONTROLS
#include "XTVersion.h"
#include "XTWndHook.h"
#endif

#ifdef _XTP_INCLUDE_COMMANDBARS
#include "XTPPaintManager.h"
#include "XTPCommandBars.h"
#include "XTPPopupBar.h"
#include "XTPImageManager.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------------
// helper function which sets the font for a window and all its children
// and also resizes everything according to the new font
//-----------------------------------------------------------------------------

_XT_EXT_CLASS void  AFXAPI XTFuncChangeWindowFont(CWnd* pWnd, CFont* pFont)
{
	CRect windowRect;

	// grab old and new text metrics
	TEXTMETRIC tmOld, tmNew;
	CDC * pDC = pWnd->GetDC();
	CFont * pSavedFont = pDC->SelectObject(pWnd->GetFont());
	pDC->GetTextMetrics(&tmOld);
	pDC->SelectObject(pFont);
	pDC->GetTextMetrics(&tmNew);
	pDC->SelectObject(pSavedFont);
	pWnd->ReleaseDC(pDC);

	long oldHeight = tmOld.tmHeight+tmOld.tmExternalLeading;
	long newHeight = tmNew.tmHeight+tmNew.tmExternalLeading;

	// calculate new dialog window rectangle
	CRect clientRect, newClientRect, newWindowRect;

	pWnd->GetWindowRect(windowRect);
	pWnd->GetClientRect(clientRect);
	long xDiff = windowRect.Width() - clientRect.Width();
	long yDiff = windowRect.Height() - clientRect.Height();

	newClientRect.left = newClientRect.top = 0;
	newClientRect.right = clientRect.right * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
	newClientRect.bottom = clientRect.bottom * newHeight / oldHeight;

	newWindowRect.left = windowRect.left -
		(newClientRect.right - clientRect.right)/2;
	newWindowRect.top = windowRect.top -
		(newClientRect.bottom - clientRect.bottom)/2;
	newWindowRect.right = newWindowRect.left + newClientRect.right + xDiff;
	newWindowRect.bottom = newWindowRect.top + newClientRect.bottom + yDiff;

	pWnd->MoveWindow(newWindowRect);

	pWnd->SetFont(pFont);

	// iterate through and move all child windows and change their font.
	CWnd* pChildWnd = pWnd->GetWindow(GW_CHILD);

	while (pChildWnd)
	{
		pChildWnd->SetFont(pFont);
		pChildWnd->GetWindowRect(windowRect);

		CString strClass;
		::GetClassName(pChildWnd->m_hWnd, strClass.GetBufferSetLength(32), 31);
		strClass.MakeUpper();
		if(strClass==_T("COMBOBOX"))
		{
			CRect rect;
			pChildWnd->SendMessage(CB_GETDROPPEDCONTROLRECT,0,(LPARAM) &rect);
			windowRect.right = rect.right;
			windowRect.bottom = rect.bottom;
		}

		pWnd->ScreenToClient(windowRect);
		windowRect.left = windowRect.left * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
		windowRect.right = windowRect.right * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
		windowRect.top = windowRect.top * newHeight / oldHeight;
		windowRect.bottom = windowRect.bottom * newHeight / oldHeight;
		pChildWnd->MoveWindow(windowRect);

		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}
}

//-----------------------------------------------------------------------------
// FUNCTION: XTFuncDrawEmbossed
//-----------------------------------------------------------------------------

_XT_EXT_CLASS void AFXAPI XTFuncDrawEmbossed(CDC* pDC, CImageList& imageList, int nIndex, CPoint point, BOOL bInColor)
{
	IMAGEINFO imgInfo;
	VERIFY(imageList.GetImageInfo(0, &imgInfo));
	CRect rcImage = imgInfo.rcImage;

	// create memory dc
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	// create mono or color bitmap
	CBitmap bitmap;
	if (bInColor) {
		bitmap.CreateCompatibleBitmap(pDC, rcImage.Width(), rcImage.Height());
	} else {
		bitmap.CreateBitmap(rcImage.Width(), rcImage.Height(), 1, 1, NULL);
	}

	// draw image into memory DC--fill BG white first
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, rcImage.Width(), rcImage.Height(), WHITENESS);
	imageList.Draw(&memDC, nIndex, CPoint(0,0), ILD_TRANSPARENT);

	// Draw using highlight offset by (1,1), then shadow
	CBrush brShadow(GetXtremeColor(COLOR_3DSHADOW));
	CBrush brHilite(GetXtremeColor(COLOR_3DHIGHLIGHT));

	CBrush* pOldBrush = pDC->SelectObject(&brHilite);
	pDC->BitBlt(point.x+1, point.y+1, rcImage.Width(), rcImage.Height(), &memDC, 0, 0, 0xB8074A);

	pDC->SelectObject(&brShadow);
	pDC->BitBlt(point.x, point.y, rcImage.Width(), rcImage.Height(), &memDC, 0, 0, 0xB8074A);

	pDC->SelectObject(pOldBrush);
	memDC.SelectObject(pOldBitmap);
	bitmap.DeleteObject();
}

//-----------------------------------------------------------------------------
// FUNCTION: XTFuncDrawShadedRect
//-----------------------------------------------------------------------------

_XT_EXT_CLASS void AFXAPI XTFuncDrawShadedRect(CDC *pDC, CRect& rect)
{
	// Bit pattern for a monochrome brush with every
	// other pixel turned off
	WORD bits[] = {
		0x0055, 0x00AA, 0x0055, 0x00AA,
		0x0055, 0x00AA, 0x0055, 0x00AA
	};

	// Need a monochrome pattern bitmap
	CBitmap bitmap;
	bitmap.CreateBitmap( 8, 8, 1, 1, &bits );

	// Create the pattern brush
	CBrush brush;
	brush.CreatePatternBrush( &bitmap );
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	COLORREF crBack = pDC->SetBkColor(GetXtremeColor(COLOR_3DFACE));
	COLORREF crText = pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));

	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);

	// Restore the device context
	pDC->SelectObject(pOldBrush);
	pDC->SetBkColor(crBack);
	pDC->SetTextColor(crText);

	brush.DeleteObject();
}

//-----------------------------------------------------------------------------
//  DIBs use RGBQUAD format:
//      0xbb 0xgg 0xrr 0x00
//
//  Reasonably efficient code to convert a COLORREF into an
//  RGBQUAD is byte-order-dependent, so we need different
//  code depending on the byte order we're targeting.
//-----------------------------------------------------------------------------

#define RGB_TO_RGBQUAD(r,g,b)   (RGB(b,g,r))
#define RGBQUAD_INIT(r,g,b)     (RGB_TO_RGBQUAD(r,g,b)), (RGB(r, g, b))
#define CLR_TO_RGBQUAD(clr)     (RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)))

//-----------------------------------------------------------------------------
// CXTReplaceColorHandler - Helper object to replace color in a given DC
//-----------------------------------------------------------------------------

class CXTReplaceColorHandler
{
public:

	// Input: pTargetDC - DC in which to replace colors
	//        nWidth    - width of the rectangle to replace (starting from (0,0))
	//        nHeight   - height of the rectangle to replace the color (starting from (0, 0))
	// Summary: Constructor takes parameters common for the operation.
	CXTReplaceColorHandler(CDC* pTargetDC, int nWidth, int nHeight);

	// Summary:
	virtual ~CXTReplaceColorHandler();

	// Summary: Initialization, returns false if fails to initialize
	bool Init();

	// Summary: Replaces one color with another
	bool ReplaceColor(COLORREF clrFrom, COLORREF clrTo);

protected:

	int         m_nWidth;
	int         m_nHeight;
	bool        m_bOldBitmap;   // true if m_pOldBitmap was initialized
	CDC         m_dcMask;       //
	CDC*        m_pTargetDC;    // a DC to replace colors in place
	CBitmap     m_bitmapMask;   // monochrome bitmap to work with
	CBitmap*    m_pOldBitmap;   // previously selected bitmap
	COLORREF    m_clrOldText;
	COLORREF    m_clrOldBk;
};

CXTReplaceColorHandler::CXTReplaceColorHandler(CDC* pTargetDC, int nWidth, int nHeight)
: m_pOldBitmap(NULL)
, m_bOldBitmap(false)
, m_pTargetDC(pTargetDC)
, m_clrOldBk(pTargetDC->GetBkColor())
, m_clrOldText(pTargetDC->GetTextColor())
, m_nWidth(nWidth)
, m_nHeight(nHeight)
{

}

CXTReplaceColorHandler::~CXTReplaceColorHandler()
{
	if (m_bOldBitmap)
	{
		m_dcMask.SelectObject(m_pOldBitmap);
	}
	m_pTargetDC->SetBkColor(m_clrOldBk);
	m_pTargetDC->SetTextColor(m_clrOldText);
}

bool CXTReplaceColorHandler::Init()
{
	if (!m_dcMask.CreateCompatibleDC(m_pTargetDC))
	{
		return false;
	}
	if (!m_bitmapMask.CreateBitmap(m_nWidth, m_nHeight, 1, 1, NULL))
	{
		return false;
	}
	m_pOldBitmap = m_dcMask.SelectObject(&m_bitmapMask);
	m_bOldBitmap = true;

	return true;
}

bool CXTReplaceColorHandler::ReplaceColor(COLORREF clrFrom, COLORREF clrTo)
{
	// filter out trivial case
	if (clrFrom == clrTo)
	{
		return true;
	}

	// Initialize the mask:
	// 1. Set the background color of the source DC to the color to translate.
	// so 1s would be the bits where color match has been found
	// 2. Create the object mask for the bitmap by performing a BitBlt
	// from the source bitmap to a monochrome bitmap.
	m_pTargetDC->SetBkColor(clrFrom);
	if (!m_dcMask.BitBlt(0, 0, m_nWidth, m_nHeight, m_pTargetDC, 0, 0, SRCCOPY))
	{
		return false;
	}

	// zero out destination pixels we would want to replace with another color
	m_pTargetDC->SetTextColor(RGB(255, 255, 255));
	m_pTargetDC->SetBkColor(RGB(0,0,0));
	if (!m_pTargetDC->BitBlt(0, 0, m_nWidth, m_nHeight, &m_dcMask, 0, 0, SRCAND))
	{
		return false;
	}

	// now, replace zeroed pixels with destination color
	if (clrTo != RGB(0, 0, 0))
	{
		m_pTargetDC->SetBkColor(clrTo);
		m_pTargetDC->SetTextColor(RGB(0, 0, 0));
		if (!m_pTargetDC->BitBlt(0, 0, m_nWidth, m_nHeight, &m_dcMask, 0, 0, SRCPAINT))
		{
			return false;
		}
	}

	// done
	return true;
}

//-----------------------------------------------------------------------------
// CXTLoadResourceHandler - A helper to unlock resource
//-----------------------------------------------------------------------------

class CXTLoadResourceHandler
{
public:
	// Summary:
	CXTLoadResourceHandler(HINSTANCE hInst, HRSRC hRsrc);

	// Summary:
	~CXTLoadResourceHandler();

	// Summary:
	LPBITMAPINFOHEADER GetBitmapHeader() const;

private:
	HGLOBAL m_hglb; //
};

CXTLoadResourceHandler::CXTLoadResourceHandler(HINSTANCE hInst, HRSRC hRsrc)
: m_hglb(::LoadResource(hInst, hRsrc))
{

}

CXTLoadResourceHandler::~CXTLoadResourceHandler()
{
	if (m_hglb)
		::FreeResource(m_hglb);
}

LPBITMAPINFOHEADER CXTLoadResourceHandler::GetBitmapHeader() const
{
	if (m_hglb)
		return (LPBITMAPINFOHEADER)::LockResource(m_hglb);
	return NULL;
}

//-----------------------------------------------------------------------------
// FUNCTION: XTFuncLoadSysColorBitmap(HINSTANCE hInst, HRSRC hRsrc, BOOL bMono)
//-----------------------------------------------------------------------------

_XT_EXT_CLASS HBITMAP AFXAPI XTFuncLoadSysColorBitmap(HINSTANCE hInst, HRSRC hRsrc, BOOL bMono)
{
	static const struct COLORMAP
	{
		DWORD    rgbqFrom;
		COLORREF clrFrom;
		int      iSysColorTo;
	} sysColorMap[] =
	{
		// mapping from color in DIB to system color
		{ RGBQUAD_INIT(0x00, 0x00, 0x00),  COLOR_BTNTEXT },       // black
		{ RGBQUAD_INIT(0x80, 0x80, 0x80),  COLOR_BTNSHADOW },     // dark gray
		{ RGBQUAD_INIT(0xC0, 0xC0, 0xC0),  COLOR_BTNFACE },       // bright gray
		{ RGBQUAD_INIT(0xFF, 0xFF, 0xFF),  COLOR_BTNHIGHLIGHT }   // white
	};


	CXTLoadResourceHandler hglb(hInst, hRsrc);
	LPBITMAPINFOHEADER lpBitmap = hglb.GetBitmapHeader();
	if (lpBitmap == NULL)
		return NULL;

	// figure out what is the size of the palette the bitmap has
	int nColorTableSize;
	if (lpBitmap->biBitCount <= 8)
	{
		nColorTableSize = (1 << lpBitmap->biBitCount);
	}
	else
	{
		// No palette needed for 24 BPP DIB
		nColorTableSize = 0;
	}

	// make copy of BITMAPINFOHEADER so we can modify the color table
	UINT nSize = lpBitmap->biSize + nColorTableSize * sizeof(RGBQUAD);
	LPBITMAPINFOHEADER lpBitmapInfo = (LPBITMAPINFOHEADER)::_alloca(nSize);

	MEMCPY_S(lpBitmapInfo, lpBitmap, nSize);

	bool bConverted = false;
	if (nColorTableSize)
	{
		// color table is in RGBQUAD DIB format
		DWORD* pColorTable = (DWORD*)(((LPBYTE)lpBitmapInfo) + (UINT)lpBitmapInfo->biSize);

		int iColor;
		for (iColor = 0; iColor < nColorTableSize; iColor++)
		{
			// look for matching RGBQUAD color in original
			int i;
			for (i = 0; i < _countof(sysColorMap); i++)
			{
				if (pColorTable[iColor] == sysColorMap[i].rgbqFrom)
				{
					if (bMono)
					{
						// all colors except text become white
						if (sysColorMap[i].iSysColorTo != COLOR_BTNTEXT)
							pColorTable[iColor] = RGB_TO_RGBQUAD(255, 255, 255);
					}
					else
						pColorTable[iColor] =
							CLR_TO_RGBQUAD(::GetSysColor(sysColorMap[i].iSysColorTo));
					break;
				}
			}
		}
		bConverted = true;
	}

	int nWidth = (int)lpBitmapInfo->biWidth;
	int nHeight = (int)lpBitmapInfo->biHeight;
	CWindowDC dcScreen(NULL);
	CBitmap bmp;
	if (!bmp.CreateCompatibleBitmap(&dcScreen, nWidth, nHeight))
	{
		return NULL;
	}

	CDC dcGlyphs;
	if (!dcGlyphs.CreateCompatibleDC(&dcScreen))
	{
		return NULL;
	}
	CXTContextObjectHandler<CBitmap> handlerBmp(&dcGlyphs, &bmp);

	// reserve space for color translation table used when doing direct translation
	struct
	{
		COLORREF clrFrom;
		COLORREF clrTo;
	}
	colorTranslationTable [_countof(sysColorMap)];
	memset(colorTranslationTable, 0, sizeof(colorTranslationTable));

	if (!bConverted)
	{
		// get translation colors
		const int nTestWidth = (_countof(sysColorMap) % 8 + 1) * 8;
		const UINT nTestDibSize = sizeof(BITMAPINFOHEADER) + nTestWidth * 3;
		LPBITMAPINFOHEADER pTestDIB = (LPBITMAPINFOHEADER)::_alloca(nTestDibSize);
		memset(pTestDIB, 0, nTestDibSize);
		pTestDIB->biSize = sizeof(BITMAPINFOHEADER);
		pTestDIB->biWidth = nTestWidth;
		pTestDIB->biHeight = 1;
		pTestDIB->biPlanes = 1;
		pTestDIB->biBitCount = 24;
		pTestDIB->biCompression = BI_RGB;
		pTestDIB->biSizeImage = nTestDibSize - sizeof(BITMAPINFOHEADER);
		pTestDIB->biXPelsPerMeter = lpBitmap->biXPelsPerMeter;
		pTestDIB->biYPelsPerMeter = lpBitmap->biYPelsPerMeter;
		pTestDIB->biClrUsed = 0;
		pTestDIB->biClrImportant = 0;

		LPBYTE lpTestBits = (LPBYTE)(pTestDIB + 1);
		int i;
		for (i = 0; i < _countof(sysColorMap); i++, lpTestBits+=3)
		{
			colorTranslationTable[i].clrFrom = sysColorMap[i].clrFrom;
			COLORREF clrTo;
			if (bMono)
			{
				// all colors except text become white
				if (sysColorMap[i].iSysColorTo != COLOR_BTNTEXT)
				{
					clrTo = RGB(255, 255, 255);
				}
				else
				{
					clrTo = RGB(0, 0, 0);
				}
			}
			else
			{
				clrTo = ::GetSysColor(sysColorMap[i].iSysColorTo);
			}
			colorTranslationTable[i].clrTo = clrTo;
			// RGB QUAD
			lpTestBits[2] = GetRValue(colorTranslationTable[i].clrFrom);
			lpTestBits[1] = GetGValue(colorTranslationTable[i].clrFrom);
			lpTestBits[0] = GetBValue(colorTranslationTable[i].clrFrom);
		}

		// fill in the target bitmap
		if (::StretchDIBits(dcGlyphs,
							0, 0, nTestWidth, 1,
							0, 0, nTestWidth, 1,
							(LPBYTE)(pTestDIB + 1),
							(LPBITMAPINFO)pTestDIB, DIB_RGB_COLORS, SRCCOPY) == GDI_ERROR)
		{
			return NULL;
		}

		for (i = 0; i < _countof(sysColorMap); i++)
		{
			colorTranslationTable[i].clrFrom = dcGlyphs.GetPixel(i, 0);
		}
	}

	// get the pointer to actual bitmap bits
	LPBYTE lpBits = LPBYTE(lpBitmap + 1) + nColorTableSize * sizeof(RGBQUAD);

	// fill in the target bitmap
	if (::StretchDIBits(dcGlyphs, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight,
		lpBits, (LPBITMAPINFO)lpBitmapInfo, DIB_RGB_COLORS, SRCCOPY) == GDI_ERROR)
	{
		return NULL;
	}

	// the bit map is created now, translate colors hard way if that  could not
	// be done via the palette massaging
	if (!bConverted)
	{
		CXTReplaceColorHandler replaceHandler(&dcGlyphs, nWidth, nHeight);
		if (!replaceHandler.Init())
		{
			return NULL;
		}

		int i;
		for (i = 0; i < _countof(colorTranslationTable); i++)
		{
			if (!replaceHandler.ReplaceColor(colorTranslationTable[i].clrFrom,
						colorTranslationTable[i].clrTo))
			{
				return NULL;
			}

		}
	}
	return (HBITMAP)bmp.Detach();;
}

//-----------------------------------------------------------------------------
// FUNCTION: XTFuncChildWindowFromPoint()
//-----------------------------------------------------------------------------

_XT_EXT_CLASS HWND AFXAPI XTFuncChildWindowFromPoint(HWND hWnd, POINT point)
{
	ASSERT(hWnd != NULL);

	// check child windows
	::ClientToScreen(hWnd, &point);

	HWND hWndChild = ::GetWindow(hWnd, GW_CHILD);
	for (; hWndChild != NULL; hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT))
	{
		if ((UINT)(WORD)::GetDlgCtrlID(hWndChild) != (WORD)-1 &&
			(::GetWindowLong(hWndChild, GWL_STYLE) & WS_VISIBLE))
		{
			// see if point hits the child window
			CRect rect;
			::GetWindowRect(hWndChild, rect);
			if (rect.PtInRect(point))
				return hWndChild;
		}
	}

	return NULL;    // not found
}

//-----------------------------------------------------------------------------
// FUNCTION: XTFuncStripMnemonics()
//-----------------------------------------------------------------------------

_XT_EXT_CLASS void AFXAPI XTFuncStripMnemonics(CString& strIn)
{
	for (int i = 0; i < strIn.GetLength() - 1; i++)
	{
		if (strIn[i] == _T('&'))
		{
			// if the second character is an ampersand we do not
			// want to remove it, increment the count and continue.
			if (strIn[i+1] == _T('&')) {
				++i;
			}
			// mnemonic found, remove the underline character.
			else {
				strIn = strIn.Left(i) + strIn.Mid(i + 1);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// FUNCTION: XTFuncPathExists()
//-----------------------------------------------------------------------------

_XT_EXT_CLASS BOOL AFXAPI XTFuncPathExists(LPCTSTR lpszFileName)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(lpszFileName, &fd);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		::FindClose(hFind);
		return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// FUNCTION: XTFuncPathFindNextComponent()
//-----------------------------------------------------------------------------

_XT_EXT_CLASS LPTSTR AFXAPI XTFuncPathFindNextComponent(LPTSTR pszPath)
{
	// We will return static memory
	static TCHAR tNextComponent[MAX_PATH + 1];

	// Make sure we were given a valid path
	if ( pszPath == NULL || _tcslen( pszPath ) == 0 ) {
		return NULL;
	}

	// Find the path delimiter
	TCHAR *cp = _tcschr( pszPath, _T('\\') );
	if ( cp == NULL ) {
		// If we didn't find it, return an empty string
		tNextComponent[0] = 0;
		return tNextComponent;
	}

	// OK, we have the delimiter. Copy the rest of
	// the path to our internal buffer and return it
	STRCPY_S( tNextComponent, MAX_PATH , cp + 1 );
	return tNextComponent;
}

//-----------------------------------------------------------------------------
// FUNCTION: XTFuncCreateView()
//-----------------------------------------------------------------------------

_XT_EXT_CLASS CWnd* AFXAPI XTFuncCreateView(CWnd* pParentWnd, CRuntimeClass *pViewClass, CDocument *pDocument/*=NULL*/, CCreateContext *pContext/*=NULL*/, DWORD dwStyle/*=AFX_WS_DEFAULT_VIEW*/, CWnd* pOwnerWnd/*=NULL*/, UINT nID/*=AFX_IDW_PANE_FIRST*/)
{
#ifdef _DEBUG
	ASSERT(pContext != NULL || pViewClass != NULL);
	ASSERT(pContext != NULL || pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(pContext != NULL || AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
#endif

	if (pOwnerWnd == NULL)
		pOwnerWnd = pParentWnd;

	CCreateContext contextT;
	if (pContext == NULL)
	{
		// if no context specified, generate one from the
		// currently selected client if possible.
		contextT.m_pLastView       = NULL;
		contextT.m_pCurrentFrame   = NULL;
		contextT.m_pNewDocTemplate = NULL;
		contextT.m_pCurrentDoc     = pDocument;
		contextT.m_pNewViewClass   = pViewClass;

		if (pDocument != NULL)
			contextT.m_pNewDocTemplate = pDocument->GetDocTemplate();

		pContext = &contextT;
	}

	CWnd* pWnd = NULL;
	TRY
	{
		pWnd = (CWnd*)pContext->m_pNewViewClass->CreateObject();
		if (pWnd == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE0( "Out of memory creating a view.\n" ); // Note: DELETE_EXCEPTION(e) not required
		return NULL;
	}
	END_CATCH_ALL

	ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL); // not yet created.

	// Create with the right size (wrong position)
	CRect rect(0,0,0,0);
	if (!pWnd->Create(NULL, NULL, dwStyle,
		rect, pParentWnd, nID, pContext))
	{
		TRACE0( "Failed to create docking window view.\n" );
		return NULL;
	}

	pWnd->SetOwner(pOwnerWnd);
	return pWnd;
}


_XT_EXT_CLASS BOOL AFXAPI XTFuncContextMenu(CMenu* pPopup, UINT nFlags, int x, int y, CWnd* pWnd, int nIDBitmap)
{
	BOOL bResult = FALSE;

#ifdef _XTP_INCLUDE_COMMANDBARS
	if (XTPPaintManager())
	{
		CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(NULL);

		if (nIDBitmap > 0)
		{
			CXTPImageManager* pImageManager = new CXTPImageManager();
			pImageManager->SetIcons(nIDBitmap);
			pPopupBar->SetImageManager(pImageManager);
		}


		if (!pPopupBar->LoadMenu(pPopup))
		{
			pPopupBar->InternalRelease();
			return bResult;
		}

		bResult = CXTPCommandBars::TrackPopupMenu(pPopupBar, nFlags|TPM_NONOTIFY, x, y, pWnd);
		pPopupBar->InternalRelease();
	}
	else
	{
		bResult = pPopup->TrackPopupMenu(nFlags, x, y, pWnd);
	}

#else // Toolkit Standard

	CXTCoolMenu coolMenu;
	coolMenu.HookWindow(pWnd);
	{
		XT_MANAGE_STATE
		coolMenu.LoadToolbar(nIDBitmap);
	}
	bResult = pPopup->TrackPopupMenu(nFlags, x, y, pWnd);

#endif
	return bResult;

}
