// XTPImageEditor.cpp : implementation of the CXTPImageEditorDlg class.
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
#include "XTPResourceManager.h"
#include "XTPImageManager.h"
#include "XTPDrawHelpers.h"
#include "XTPColorManager.h"

#include "XTPCommandBarsResource.h"
#include "XTPImageEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define UNDO_COUNT 15
#define CLR_TO_RGBQUAD(clr)     (RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)))

void CXTPImageEditorPicture::CAlphaBitmap::CreateEditorBitmap(int nWidth, int nHeight)
{
	BITMAPINFOHEADER BMI;
	// Fill in the header info.
	ZeroMemory (&BMI, sizeof (BMI));
	BMI.biSize = sizeof(BITMAPINFOHEADER);
	BMI.biWidth = nWidth;
	BMI.biHeight = nHeight;
	BMI.biPlanes = 1;
	BMI.biBitCount = 32;
	BMI.biCompression = BI_RGB;   // No compression

	UINT* pSrcBits = NULL;
	Attach(CreateDIBSection (NULL, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, 0, 0l));
}

/////////////////////////////////////////////////////////////////////////////
// CXTPImageEditorPicture


CXTPImageEditorPicture::CXTPImageEditorPicture()
{
	m_pCurrentBitmap = NULL;
	m_pPictureBitmap = NULL;

	m_pParentWnd = NULL;

	m_bTracked = FALSE;

	m_hCursorLine = AfxGetApp()->LoadCursor(XTP_IDC_COMMANDBARS_LINE);
	m_hCursorFillColor = AfxGetApp()->LoadCursor(XTP_IDC_COMMANDBARS_FILLCOLOR);
	m_hCursorPencil = AfxGetApp()->LoadCursor(XTP_IDC_COMMANDBARS_PENCIL);
	m_hCursorPickColor = AfxGetApp()->LoadCursor(XTP_IDC_COMMANDBARS_PICKCOLOR);

}

CXTPImageEditorPicture::~CXTPImageEditorPicture()
{
	if (m_pPictureBitmap)
	{
		delete m_pPictureBitmap;
	}
	if (m_pCurrentBitmap)
	{
		delete m_pCurrentBitmap;
	}

	while (!m_lstUndo.IsEmpty())
	{
		CBitmap* pBitmap = m_lstUndo.RemoveTail();
		if (pBitmap) delete pBitmap;
	}

	while (!m_lstRedo.IsEmpty())
	{
		CBitmap* pBitmap = m_lstRedo.RemoveTail();
		if (pBitmap) delete pBitmap;
	}

}

void CXTPImageEditorPicture::Init(UINT nID, CXTPImageEditorDlg* pParentWnd)
{
	m_pParentWnd = pParentWnd;

	SubclassDlgItem(nID, pParentWnd);
	m_szPicture = pParentWnd->m_szPicture;

	CRect rc;
	GetWindowRect(rc);

	m_szItem = CSize (rc.Width() / m_szPicture.cx, rc.Height() / m_szPicture.cy);

	CSize sz(m_szItem.cx * m_szPicture.cx, m_szItem.cy * m_szPicture.cy);

	SetWindowPos(&CWnd::wndTop, rc.left, rc.top, sz.cx + 1, sz.cy + 1, SWP_NOMOVE);


	m_dcPicture.CreateCompatibleDC(NULL);
}

XTPImageEditorTools CXTPImageEditorPicture::GetSelectedTool()
{
	ASSERT(m_pParentWnd);
	return m_pParentWnd->m_toolSelected;
}
COLORREF CXTPImageEditorPicture::GetSelectedColor()
{
	ASSERT(m_pParentWnd);
	if (m_pParentWnd->m_pSelected)
	{
		return m_pParentWnd->m_pSelected->GetColor();
	}
	return 0;
}

BEGIN_MESSAGE_MAP(CXTPImageEditorPicture, CStatic)
	//{{AFX_MSG_MAP(CXTPImageEditorPicture)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CAPTURECHANGED()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPImageEditorPicture message handlers

BOOL CXTPImageEditorPicture::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/)
{
	::SetCursor(
		GetSelectedTool() == xtpToolFill? m_hCursorFillColor:
		GetSelectedTool() == xtpToolPencil? m_hCursorPencil:
		GetSelectedTool() == xtpToolGetColor? m_hCursorPickColor: m_hCursorLine
	);

	return TRUE;
}

void CXTPImageEditorPicture::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting
	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint, rc);

	dc.FillSolidRect(rc, RGB(255, 255, 255));

	BOOL bAlpha = m_pPictureBitmap != 0 && m_pPictureBitmap->m_bAlpha;

	PINT pBits = 0;
	PBITMAPINFO pbmi = 0;
	UINT nSize;

	if (m_pPictureBitmap)
	if (!CXTPImageManagerIcon::GetBitmapBits(dc, (HBITMAP)m_pPictureBitmap->GetSafeHandle(), pbmi, (LPVOID&)pBits, nSize))
		return;

	for (int x = 0; x < rc.Width(); x++)
	for (int y = 0; y < rc.Height(); y++)
	{
		CPoint pt = CPoint(m_szPicture.cx * x / rc.Width(), m_szPicture.cy * y / rc.Height());

		COLORREF clr;
		if (bAlpha)
		{
			clr = pBits[(pt.y * pbmi->bmiHeader.biWidth + pt.x)];

			int nAlpha = (clr & 0xFF000000) >> 24;

			int nColor = (((x +3) / 4 )+ ((y +3)/ 4)) % 2 == 0? 255: 235;

			clr = RGB(nColor - (nColor -  GetBValue(clr)) * nAlpha / 255,
					nColor -  (nColor - GetGValue(clr)) * nAlpha / 255,
					nColor -  (nColor - GetRValue(clr)) * nAlpha / 255);


		}
		else
		{

			clr = m_pPictureBitmap == NULL? XTPCOLOR_ERASE: pBits[(pt.y * pbmi->bmiHeader.biWidth + pt.x)];
			clr = CLR_TO_RGBQUAD(clr);

			if (clr == XTPCOLOR_ERASE)
				clr = (((x +3) / 4 )+ ((y +3)/ 4)) % 2 == 0? RGB(255, 255, 255): RGB(235, 235, 235);


		}
		dc.SetPixel(x, rc.Height() - y - 1, clr);
	}

	free(pBits);
	free(pbmi);

	dc.Draw3dRect(rc, 0, 0);
}

COLORREF CXTPImageEditorPicture::GetPicturePixel(CPoint point)
{
	CAlphaBitmap bmp;
	bmp.CreateEditorBitmap(m_szPicture.cx, m_szPicture.cy);

	CBitmap* pOldBitmap = m_dcPicture.SelectObject(&bmp);
	m_dcPicture.FillSolidRect(0, 0, m_szPicture.cx, m_szPicture.cy, XTPCOLOR_ERASE);
	if (m_pPictureBitmap)
		m_dcPicture.DrawState(0, m_szPicture, m_pPictureBitmap, DSS_NORMAL, (CBrush*)NULL);
	COLORREF clr = m_dcPicture.GetPixel(point);


	m_dcPicture.SelectObject(pOldBitmap);
	return clr;
}



CPoint CXTPImageEditorPicture::ClientToPicture(CPoint pt)
{
	ASSERT(m_szItem.cx > 0 && m_szItem.cy > 0);
	return CPoint(pt.x / m_szItem.cx, pt.y / m_szItem.cy);
}

void CXTPImageEditorPicture::CopyBitmap(CAlphaBitmap* pBitmapCopyTo, CAlphaBitmap* pBitmapCopyFrom, CSize sz, CPoint ptOffset, COLORREF clrTransparentFrom, COLORREF clrTransparentTo, CDC* pDC)
{
	CDC dc;
	dc.CreateCompatibleDC(pDC);

	pBitmapCopyTo->CreateEditorBitmap(sz.cx, sz.cy);

	CBitmap* pOldBitmap = dc.SelectObject(pBitmapCopyTo);
	dc.FillSolidRect(0, 0, sz.cx, sz.cy, XTPCOLOR_ERASE);

	if (pBitmapCopyFrom)
	{
		dc.DrawState(ptOffset, sz, pBitmapCopyFrom, DSS_NORMAL, (CBrush*)NULL);

		if (clrTransparentFrom != clrTransparentTo && !pBitmapCopyFrom->m_bAlpha)
		{
			for (int x = 0; x < sz.cx; x++) for (int y = 0; y < sz.cy; y++)
			{
				if (dc.GetPixel(x, y) == clrTransparentFrom) dc.SetPixel(x, y, clrTransparentTo);
			}
		}
	}

	dc.SelectObject(pOldBitmap);

	pBitmapCopyTo->m_bAlpha = pBitmapCopyFrom? pBitmapCopyFrom->m_bAlpha: FALSE;


}

void CXTPImageEditorPicture::OnPictureChanged()
{
	Invalidate(FALSE);
	m_pParentWnd->OnPictureChanged();
}

void CXTPImageEditorPicture::Apply()
{
	while (!m_lstRedo.IsEmpty())
	{
		CBitmap* pBitmap = m_lstRedo.RemoveTail();
		if (pBitmap) delete pBitmap;
	}

	if (m_lstUndo.GetCount() > UNDO_COUNT)
	{
		CBitmap* pBitmap = m_lstUndo.RemoveHead();
		if (pBitmap) delete pBitmap;
	}

	m_lstUndo.AddTail(m_pCurrentBitmap);
	m_pCurrentBitmap = NULL;

	OnPictureChanged();
}


void CXTPImageEditorPicture::OnLButtonDown(UINT nFlags, CPoint point)
{
	ASSERT(m_pCurrentBitmap == NULL);

	switch(GetSelectedTool())
	{
	case xtpToolFill:
		{
			m_pCurrentBitmap =  m_pPictureBitmap;

			m_pPictureBitmap = new CAlphaBitmap();
			CopyBitmap(m_pPictureBitmap, m_pCurrentBitmap, m_szPicture);

			CBitmap* pOldBitmap = m_dcPicture.SelectObject(m_pPictureBitmap);
			CXTPBrushDC brush(m_dcPicture, GetSelectedColor());

			CPoint ptFill(ClientToPicture(point));

			m_dcPicture.ExtFloodFill(ptFill.x, ptFill.y, GetPicturePixel(ptFill), FLOODFILLSURFACE);
			m_dcPicture.SelectObject(pOldBitmap);

			if (m_pCurrentBitmap && m_pCurrentBitmap->m_bAlpha)
				FixAlphaLayer(m_pPictureBitmap, m_pCurrentBitmap);

			Apply();
		}
		break;

	case xtpToolGetColor:
		{
			COLORREF clr = GetPicturePixel(ClientToPicture(point));
			m_pParentWnd->m_wndPicker.SetColor(clr);
			m_pParentWnd->SetSelected(&m_pParentWnd->m_wndPicker);
			m_pParentWnd->m_toolSelected = m_pParentWnd->m_toolPrevious;
		}
		break;
	case xtpToolPencil:
	case xtpToolRectangle:
	case xtpToolCircle:
	case xtpToolLine:
		{

			m_pCurrentBitmap =  m_pPictureBitmap;

			m_pPictureBitmap = new CAlphaBitmap();
			CopyBitmap(m_pPictureBitmap, m_pCurrentBitmap, m_szPicture);

			m_clrDraw = GetSelectedColor() != GetPicturePixel(ClientToPicture(point))? GetSelectedColor(): XTPCOLOR_ERASE;

			m_bTracked = TRUE;
			m_ptTracked = point;
			SetCapture();

			OnMouseMove(nFlags, point);
		}
		break;
	default :
		ASSERT(FALSE);

	}

	CStatic::OnLButtonDown(nFlags, point);
}


void CXTPImageEditorDlg::FixAlphaLayer(CBitmap* pSrc)
{
	if (pSrc == 0)
		return;

	CDC dcSrc;
	dcSrc.CreateCompatibleDC(NULL);

	PINT pBits = 0;
	PBITMAPINFO pBitmapInfo = 0;

	UINT nSize;

	if (!CXTPImageManagerIcon::GetBitmapBits(dcSrc, (HBITMAP)pSrc->GetSafeHandle(), pBitmapInfo, (LPVOID&)pBits, nSize))
		return;

	if (pBits)
	{
		for (UINT i = 0; i < nSize / 4; i++)
		{
			if (pBits[i] == 0) pBits[i] = XTPCOLOR_ERASE;
		}
		SetDIBits(dcSrc,(HBITMAP)pSrc->GetSafeHandle(), 0, pBitmapInfo->bmiHeader.biHeight, pBits, pBitmapInfo, DIB_RGB_COLORS);
	}

	free(pBits);
	free(pBitmapInfo);

}

void CXTPImageEditorPicture::FixAlphaLayer(CAlphaBitmap* pDest, CAlphaBitmap* pSrc)
{
	if (pSrc == 0 || pDest == 0)
		return;

	CDC dcSrc;
	dcSrc.CreateCompatibleDC(NULL);

	PINT pDestBits = 0, pSrcBits = 0;
	PBITMAPINFO pDestBmi = 0, pSrcBmi = 0;

	UINT nSize;

	if (!CXTPImageManagerIcon::GetBitmapBits(dcSrc, (HBITMAP)pDest->GetSafeHandle(), pDestBmi, (LPVOID&)pDestBits, nSize))
		return;

	if (!CXTPImageManagerIcon::GetBitmapBits(dcSrc, (HBITMAP)pSrc->GetSafeHandle(), pSrcBmi, (LPVOID&)pSrcBits, nSize))
	{
		free(pDestBits);
		free(pDestBmi);
		return;
	}

	PINT pFixedBits = NULL;

	HBITMAP hBitmapAlpha = CreateDIBSection(dcSrc, pDestBmi, DIB_RGB_COLORS, (void**)&pFixedBits, NULL, 0);

	if (hBitmapAlpha && pFixedBits)
	{
		MEMCPY_S(pFixedBits, pDestBits, nSize);

		for (UINT i = 0; i < nSize / 4; i++)
		{
			if (pDestBits[i] != pSrcBits[i])
			{
				if (pDestBits[i] != XTPCOLOR_ERASE)
					pFixedBits[i] |= 0xFF000000;
			}
		}

		pDest->DeleteObject();
		pDest->Attach(hBitmapAlpha);
	}

	free(pDestBits);
	free(pDestBmi);
	free(pSrcBits);
	free(pSrcBmi);

}

void CXTPImageEditorPicture::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bTracked)
	{
		if (GetSelectedTool() == xtpToolPencil)
		{
			CAlphaBitmap* pBitmap = new CAlphaBitmap();
			CopyBitmap(pBitmap, m_pPictureBitmap, m_szPicture);

			CBitmap* pOldBitmap = m_dcPicture.SelectObject(pBitmap);

			m_dcPicture.SetPixel(ClientToPicture(point), m_clrDraw);
			m_dcPicture.SelectObject(pOldBitmap);

			if (m_pPictureBitmap->m_bAlpha)
				FixAlphaLayer(pBitmap, m_pPictureBitmap);

			if (m_pPictureBitmap) delete m_pPictureBitmap;

			m_pPictureBitmap = pBitmap;

			Invalidate(FALSE);
			OnPictureChanged();
		}
		else
		{
			CAlphaBitmap* pBitmap = new CAlphaBitmap();
			CopyBitmap(pBitmap, m_pCurrentBitmap, m_szPicture);

			CBitmap* pOldBitmap = m_dcPicture.SelectObject(pBitmap);

			CRect rcReftangle(ClientToPicture(m_ptTracked), ClientToPicture(point));
			rcReftangle.NormalizeRect();

			if (GetSelectedTool() == xtpToolRectangle)
			{
				m_dcPicture.Draw3dRect(rcReftangle, GetSelectedColor(), GetSelectedColor());
			}
			if (GetSelectedTool() == xtpToolCircle)
			{
				CXTPPenDC pen(m_dcPicture, GetSelectedColor());
				CXTPBrushDC brush(m_dcPicture, XTPCOLOR_ERASE);
				m_dcPicture.Ellipse(rcReftangle);
			}
			if (GetSelectedTool() == xtpToolLine)
			{
				CXTPPenDC pen(m_dcPicture, GetSelectedColor());
				m_dcPicture.MoveTo(ClientToPicture(m_ptTracked));
				m_dcPicture.LineTo(ClientToPicture(point));
			}

			m_dcPicture.SelectObject(pOldBitmap);

			if (m_pPictureBitmap->m_bAlpha)
				FixAlphaLayer(pBitmap, m_pCurrentBitmap);

			if (m_pPictureBitmap) delete m_pPictureBitmap;
			m_pPictureBitmap = pBitmap;

			Invalidate(FALSE);
			OnPictureChanged();

		}

	}

	CStatic::OnMouseMove(nFlags, point);
}

void CXTPImageEditorPicture::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bTracked = FALSE;
	ReleaseCapture();

	CStatic::OnLButtonUp(nFlags, point);
}

void CXTPImageEditorPicture::OnCaptureChanged(CWnd *pWnd)
{
	m_bTracked = FALSE;
	Apply();

	CStatic::OnCaptureChanged(pWnd);
}

void CXTPImageEditorPicture::Undo()
{
	if (!m_lstUndo.IsEmpty())
	{
		CAlphaBitmap* pBitmap = m_lstUndo.RemoveTail();

		m_lstRedo.AddTail(m_pPictureBitmap);

		m_pPictureBitmap = pBitmap;

		OnPictureChanged();
	}
}

void CXTPImageEditorPicture::Redo()
{
	if (!m_lstRedo.IsEmpty())
	{
		CAlphaBitmap* pBitmap = m_lstRedo.RemoveTail();

		m_lstUndo.AddTail(m_pPictureBitmap);

		m_pPictureBitmap = pBitmap;

		OnPictureChanged();
	}
}


void CXTPImageEditorPicture::Clear()
{
	m_pCurrentBitmap =  m_pPictureBitmap;
	m_pPictureBitmap = NULL;
	Apply();
}

void CXTPImageEditorPicture::MovePicture(CPoint ptOffset)
{
	m_pCurrentBitmap =  m_pPictureBitmap;

	m_pPictureBitmap = new CAlphaBitmap();
	CopyBitmap(m_pPictureBitmap, m_pCurrentBitmap, m_szPicture, ptOffset);

	Apply();
}


void CXTPImageEditorPicture::Copy()
{
	if (!OpenClipboard ())
	{
		return;
	}

	if (!::EmptyClipboard ())
	{
		::CloseClipboard ();
		return;
	}

	CAlphaBitmap bitmapCopy;
	CopyBitmap(&bitmapCopy, m_pPictureBitmap, m_szPicture, 0, XTPCOLOR_ERASE, GetXtremeColor(COLOR_3DFACE));

	HANDLE hclipData = ::SetClipboardData (CF_BITMAP, bitmapCopy.Detach ());
	if (hclipData == NULL)
	{
		TRACE (_T ("CXTPImageEditorPicture::Copy() error. Error code = %x\n"), GetLastError ());
	}

	if (bitmapCopy.m_bAlpha)
	{
		SetClipboardData(XTPImageManager()->m_nAlphaClipFormat, GlobalAlloc(GMEM_MOVEABLE, 1));
	}

	::CloseClipboard ();
}

void CXTPImageEditorPicture::Paste()
{
	COleDataObject data;
	if (!data.AttachClipboard ())
	{
		return;
	}
	if (!data.IsDataAvailable (CF_BITMAP))
	{
		return;
	}

	tagSTGMEDIUM dataMedium;
	if (!data.GetData (CF_BITMAP, &dataMedium))
	{
		return;
	}

	if (!dataMedium.hBitmap)
		return;

	CAlphaBitmap bmpClip;

	BOOL bAlpha = FALSE;
	FORMATETC fetc;
	static TCHAR sz[256] ;

	data.BeginEnumFormats();
	while (data.GetNextFormat(&fetc))
	{
		if (GetClipboardFormatName( (UINT)fetc.cfFormat, sz, 254 ))
		{
			CString str(sz);
			if (str.Find(_T("DIB32")) > 0)
			{
				bAlpha = TRUE;
				break;
			}
		}
	}

	bmpClip.m_bAlpha = bAlpha? bAlpha: data.IsDataAvailable(XTPImageManager()->m_nAlphaClipFormat);
	bmpClip.Attach(dataMedium.hBitmap);


	m_pCurrentBitmap =  m_pPictureBitmap;

	m_pPictureBitmap = new CAlphaBitmap();
	CopyBitmap(m_pPictureBitmap, &bmpClip, m_szPicture, 0, GetXtremeColor(COLOR_3DFACE), XTPCOLOR_ERASE);

	bmpClip.Detach();

	Apply();

}


void CXTPImageEditorPicture::Load()
{
	USES_CONVERSION;

	CString strFilter;
	VERIFY(XTPResourceManager()->LoadString(&strFilter, XTP_IDS_IMAGEEDITOR_FILTER));
	CFileDialog fd(TRUE, NULL, NULL, OFN_HIDEREADONLY, strFilter);

	if (fd.DoModal() == IDOK)
	{

		CString strExtension = fd.GetFileExt();
		strExtension.MakeLower();

		if (strExtension == _T("ico"))
		{

			HBITMAP hBitmap = CXTPImageManagerIcon::LoadAlphaIcon(fd.GetPathName(), m_szPicture.cx);

			if (!hBitmap)
			{
				HICON hIcon = (HICON)LoadImage(0, fd.GetPathName(),
					IMAGE_ICON, m_szPicture.cx, m_szPicture.cy, LR_CREATEDIBSECTION |LR_LOADFROMFILE);

				if (!hIcon)
					return;

				m_pCurrentBitmap =  m_pPictureBitmap;
				m_pPictureBitmap = new CAlphaBitmap();

				CDC dc;
				dc.CreateCompatibleDC(NULL);

				m_pPictureBitmap->CreateEditorBitmap(m_szPicture.cx, m_szPicture.cy);

				CBitmap* pOldBitmap = dc.SelectObject(m_pPictureBitmap);
				dc.FillSolidRect(0, 0, m_szPicture.cx, m_szPicture.cy, XTPCOLOR_ERASE);

				dc.DrawState(0, CSize(0, 0), hIcon, DSS_NORMAL, (CBrush*)NULL);

				dc.SelectObject(pOldBitmap);

				DestroyIcon(hIcon);
			}
			else
			{

				m_pCurrentBitmap =  m_pPictureBitmap;
				m_pPictureBitmap = new CAlphaBitmap();


				CAlphaBitmap bmpFile(TRUE);
				bmpFile.Attach(hBitmap);

				CopyBitmap(m_pPictureBitmap, &bmpFile, m_szPicture);
			}

			Apply();


		}
		else if (strExtension == _T("bmp"))
		{

			HBITMAP hBmp = (HBITMAP)LoadImage(0, fd.GetPathName(),
				IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION |LR_LOADFROMFILE);

			if (!hBmp)
				return;

			CAlphaBitmap bmpFile(CXTPImageManagerIcon::IsAlphaBitmapFile(fd.GetPathName()));
			bmpFile.Attach(hBmp);

			m_pCurrentBitmap =  m_pPictureBitmap;
			m_pPictureBitmap = new CAlphaBitmap();

			CopyBitmap(m_pPictureBitmap, &bmpFile, m_szPicture);

			Apply();
		}
		else
		{
			LPPICTURE pPict = NULL;

			if (OleLoadPicturePath(T2OLE((LPTSTR)(LPCTSTR)fd.GetPathName()), NULL, 0, 0, IID_IPicture, (LPVOID*)&pPict) == S_OK )
			{

				m_pCurrentBitmap =  m_pPictureBitmap;
				m_pPictureBitmap = new CAlphaBitmap();

				CDC dc;
				dc.CreateCompatibleDC(NULL);

				m_pPictureBitmap->CreateEditorBitmap(m_szPicture.cx, m_szPicture.cy);

				CBitmap* pOldBitmap = dc.SelectObject(m_pPictureBitmap);
				dc.FillSolidRect(0, 0, m_szPicture.cx, m_szPicture.cy, XTPCOLOR_ERASE);

				long hmWidth;
				long hmHeight;

				pPict->get_Width(&hmWidth);
				pPict->get_Height(&hmHeight);

				pPict->Render(dc, 0, 0,
					m_szPicture.cx, m_szPicture.cy, 0, hmHeight-1,
					hmWidth, -hmHeight, 0);

				dc.SelectObject(pOldBitmap);

				Apply();

				pPict->Release();

			}


		}
	}

}


/////////////////////////////////////////////////////////////////////////////
// CXTPImageEditorPicker

IMPLEMENT_DYNAMIC(CXTPImageEditorPicker, CStatic)

CXTPImageEditorPicker::CXTPImageEditorPicker()
{
	m_clr = 0;
	m_bSelected = FALSE;
}


void CXTPImageEditorPicker::SetColor(COLORREF clr)
{
	m_clr = clr;
	if (m_hWnd) Invalidate(FALSE);
}
COLORREF CXTPImageEditorPicker::GetColor()
{
	return m_clr;
}

void CXTPImageEditorPicker::SetSelected(BOOL bSelected)
{
	m_bSelected = bSelected;
	if (m_hWnd) Invalidate(FALSE);
}


BEGIN_MESSAGE_MAP(CXTPImageEditorPicker, CStatic)
	//{{AFX_MSG_MAP(CXTPImageEditorPicker)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPImageEditorPicker message handlers


void CXTPImageEditorPicker::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting
	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint, rc);

	if (m_clr != XTPCOLOR_ERASE)
	{
		dc.FillSolidRect(rc, m_clr);
	}
	else
	{

		CRect rcItem(rc);
		dc.FillSolidRect(rc, RGB(0xFF, 0xFF, 0xFF));
		dc.FillSolidRect(rcItem.left, rcItem.top, rcItem.Width() / 2, rcItem.Height() / 2, RGB(235, 235, 235));
		dc.FillSolidRect(rcItem.left + rcItem.Width() / 2, rcItem.top + rcItem.Height() / 2,
			rcItem.Width() - (rcItem.Width() / 2), rcItem.Height() - (rcItem.Height() / 2), RGB(235, 235, 235));

	}
	if (!m_bSelected)
	{
		dc.Draw3dRect(rc, GetXtremeColor(COLOR_3DDKSHADOW), GetXtremeColor(COLOR_3DDKSHADOW));
	}
	else
	{
		dc.Draw3dRect(rc, 0, 0);
		rc.DeflateRect(1, 1);
		dc.Draw3dRect(rc, RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF));
		rc.DeflateRect(1, 1);
		dc.Draw3dRect(rc, 0, 0);
	}

}

void CXTPImageEditorPicker::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/)
{
	NMHDR mHDR;
	mHDR.hwndFrom = m_hWnd;
	mHDR.code     = NM_CLICK;
	mHDR.idFrom   = GetDlgCtrlID();

	GetParent()->SendMessage(WM_NOTIFY,
		( WPARAM )mHDR.idFrom, ( LPARAM )&mHDR );
}

/////////////////////////////////////////////////////////////////////////////
// CXTPImageEditorPreview

CXTPImageEditorPreview::CXTPImageEditorPreview()
{
}

void CXTPImageEditorPreview::Init(UINT nID, CXTPImageEditorDlg* pParentWnd)
{
	m_pParentWnd = pParentWnd;

	SubclassDlgItem(nID, pParentWnd);
	m_szPicture = pParentWnd->m_szPicture;

	ModifyStyleEx(WS_EX_STATICEDGE, 0);

}


BEGIN_MESSAGE_MAP(CXTPImageEditorPreview, CStatic)
	//{{AFX_MSG_MAP(CXTPImageEditorPreview)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPImageEditorPreview message handlers

void CXTPImageEditorPreview::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting
	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint, rc);
	dc.FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));


	CRect rcButton (10, (rc.Height() - m_szPicture.cy) / 2, 10 + m_szPicture.cx, (rc.Height() - m_szPicture.cy) / 2 + m_szPicture.cy);
	CRect rcFrame(rcButton);
	rcFrame.InflateRect(3, 3);

	dc.Draw3dRect(rcFrame, RGB(0xFF, 0xFF, 0xFF), GetXtremeColor(COLOR_3DSHADOW));
	rcFrame.InflateRect(1, 1);
	dc.Draw3dRect(rcFrame, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DDKSHADOW));

	ASSERT(m_pParentWnd);

	if (m_pParentWnd->m_wndPicture.m_pPictureBitmap)
	{
		if (m_pParentWnd->m_wndPicture.m_pPictureBitmap->m_bAlpha)
		{
			HBITMAP hBitmap = CXTPImageManagerIcon::PreMultiplyAlphaBitmap((HBITMAP)m_pParentWnd->m_wndPicture.m_pPictureBitmap->GetSafeHandle());
			if (hBitmap)
			{
				CXTPImageManagerIcon::DrawAlphaBitmap(&dc, rcButton.TopLeft(), hBitmap, m_szPicture);

				DeleteObject(hBitmap);
			}
		}
		else
		{
			CXTPImageEditorPicture::CAlphaBitmap bmpClient;
			CXTPImageEditorPicture::CopyBitmap(&bmpClient, m_pParentWnd->m_wndPicture.m_pPictureBitmap, m_szPicture, 0, XTPCOLOR_ERASE, GetXtremeColor(COLOR_3DFACE), &dc);

			dc.DrawState(rcButton.TopLeft(), m_szPicture, &bmpClient, DSS_NORMAL, (CBrush*)NULL);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDlgToolBar dialog

BEGIN_MESSAGE_MAP(CXTPImageEditorDlg::CDlgToolBar, CToolBar)
	{ WM_IDLEUPDATECMDUI, 0, 0, 0, AfxSig_lwl, (AFX_PMSG)(AFX_PMSGW)(LRESULT (AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))&CXTPImageEditorDlg::CDlgToolBar::OnIdleUpdateCmdUI },
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPImageEditorDlg dialog

CXTPImageEditorDlg::CXTPImageEditorDlg(CWnd* pParent /*=NULL*/)
{
	InitModalIndirect(XTPResourceManager()->LoadDialogTemplate(MAKEINTRESOURCE(XTP_IDD_IMAGEEDITOR_DIALOG)), pParent);

	m_toolSelected = xtpToolPencil;

	m_pSelected = NULL;
	m_szPicture = CSize(16, 16);



}

void CXTPImageEditorDlg::GetIcon(CXTPImageManagerIconHandle* pHandle)
{
	ASSERT(pHandle);

	if (m_wndPicture.m_pPictureBitmap)
	{
		if (m_wndPicture.m_pPictureBitmap->m_bAlpha)
		{
			pHandle->CopyHandle((HBITMAP)m_wndPicture.m_pPictureBitmap->GetSafeHandle());
		}
		else
		{

			CImageList imgList;
			imgList.Create(m_szPicture.cx, m_szPicture.cy, ILC_COLOR24|ILC_MASK, 0, 1);
			imgList.Add(m_wndPicture.m_pPictureBitmap, XTPCOLOR_ERASE);

			if (imgList.GetImageCount() != 1)
				return;

			*pHandle = imgList.ExtractIcon(0);
		}
	}
}

void CXTPImageEditorDlg::SetIconSize(CSize szIcon)
{
	m_szPicture = szIcon;
}
void CXTPImageEditorDlg::SetIcon(const CXTPImageManagerIconHandle& iconHandle)
{
	if (iconHandle.IsEmpty())
		return;

	ASSERT(m_wndPicture.m_pPictureBitmap == NULL);
	m_szPicture = iconHandle.GetExtent();

	if (iconHandle.IsAlpha())
	{
		CXTPImageEditorPicture::CAlphaBitmap* pBitmap = new CXTPImageEditorPicture::CAlphaBitmap(TRUE);

		pBitmap->Attach(CXTPImageManagerIcon::CopyAlphaBitmap(iconHandle.GetBitmap()));

		FixAlphaLayer(pBitmap);
		m_wndPicture.m_pPictureBitmap = pBitmap;

	}
	else
	{
		CXTPImageEditorPicture::CAlphaBitmap* pBitmap = new CXTPImageEditorPicture::CAlphaBitmap();

		CDC dc;
		dc.CreateCompatibleDC(NULL);

		pBitmap->CreateEditorBitmap(m_szPicture.cx, m_szPicture.cy);

		CBitmap* pOldBitmap = dc.SelectObject(pBitmap);
		dc.FillSolidRect(0, 0, m_szPicture.cx, m_szPicture.cy, XTPCOLOR_ERASE);

		dc.DrawState(0, CSize(0, 0), iconHandle.GetIcon(), DSS_NORMAL, (CBrush*)NULL);

		dc.SelectObject(pOldBitmap);

		m_wndPicture.m_pPictureBitmap = pBitmap;
	}

}

void CXTPImageEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPImageEditorDlg)
	DDX_Control(pDX, XTP_IDC_STATIC_TOOLS, m_gboxTools);
	DDX_Control(pDX, XTP_IDC_STATIC_MOVE, m_gboxMove);
	//}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(CXTPImageEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CXTPImageEditorDlg)
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)

	ON_NOTIFY(NM_CLICK, XTP_ID_CLR_PICKER, OnPickerClick)
	ON_NOTIFY_RANGE(NM_CLICK, XTP_IDS_CLR_BLACK, XTP_IDS_CLR_FUCHSIA, OnSelectorClick)
	ON_NOTIFY(NM_CLICK, XTP_ID_CLR_ERASE, OnEraseClick)

	ON_UPDATE_COMMAND_UI_RANGE(XTP_ID_BUTTON_PENCIL, XTP_ID_BUTTON_CIRCLE, OnUpdateButtonTool)
	ON_COMMAND_RANGE(XTP_ID_BUTTON_PENCIL, XTP_ID_BUTTON_CIRCLE, OnButtonTool)

	ON_UPDATE_COMMAND_UI(XTP_ID_BUTTON_UNDO, OnUpdateButtonUndo)
	ON_COMMAND(XTP_ID_BUTTON_UNDO, OnButtonUndo)

	ON_UPDATE_COMMAND_UI(XTP_ID_BUTTON_REDO, OnUpdateButtonRedo)
	ON_COMMAND(XTP_ID_BUTTON_REDO, OnButtonRedo)

	ON_COMMAND(XTP_ID_BUTTON_CLEAR, OnButtonClear)
	ON_COMMAND(XTP_ID_BUTTON_COPY, OnButtonCopy)
	ON_COMMAND(XTP_ID_BUTTON_PASTE, OnButtonPaste)
	ON_COMMAND(XTP_ID_BUTTON_OPEN, OnButtonOpen)


	//ON_CONTROL_RANGE(BN_CLICKED, XTP_IDC_BUTTON_LEFT, XTP_IDC_BUTTON_DOWN, OnMoveButton)
	ON_COMMAND_RANGE(XTP_ID_BUTTON_LEFT, XTP_ID_BUTTON_RIGHT, OnMoveButton)

	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)


	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPImageEditorDlg message handlers

BOOL CXTPImageEditorDlg::OnInitDialog()
{
	UINT nButtonsTools[] = {
		XTP_ID_BUTTON_PENCIL,
		XTP_ID_BUTTON_FILL,
		XTP_ID_BUTTON_UNDO,
		XTP_ID_BUTTON_REDO,
		XTP_ID_BUTTON_GETCOLOR,
		XTP_ID_BUTTON_LINE,
		XTP_ID_BUTTON_COPY,
		XTP_ID_BUTTON_PASTE,
		XTP_ID_BUTTON_RECTANGLE,
		XTP_ID_BUTTON_CIRCLE,
		XTP_ID_BUTTON_CLEAR,
		XTP_ID_BUTTON_OPEN
	};


	UINT nButtonsMove[] = {
		XTP_ID_BUTTON_LEFT,
		XTP_ID_BUTTON_DOWN,
		XTP_ID_BUTTON_UP,
		XTP_ID_BUTTON_RIGHT
	};

	const struct
	{
		COLORREF clr;
		UINT nID;
		POINT ptOffset;
	}
	colors[] =
	{
		RGB(0x00,0x00,0x00),    XTP_IDS_CLR_BLACK           ,  {0, 0},
		RGB(0x80,0x00,0x00),    XTP_IDS_CLR_DARK_RED        ,  {2, 0},
		RGB(0x80,0x80,0x00),    XTP_IDS_CLR_DARK_YELLOW     ,  {3, 0},
		RGB(0x00,0x00,0x80),    XTP_IDS_CLR_DARK_BLUE       ,  {6, 0},
		RGB(0x00,0x80,0x00),    XTP_IDS_CLR_GREEN           ,  {4, 0},
		RGB(0x00,0x80,0x80),    XTP_IDS_CLR_TEAL            ,  {5, 0},
		RGB(0x00,0x00,0xFF),    XTP_IDS_CLR_BLUE            ,  {6, 1},
		RGB(0x80,0x80,0x80),    XTP_IDS_CLR_GRAY50          ,  {1, 0},
		RGB(0xFF,0x00,0x00),    XTP_IDS_CLR_RED             ,  {2, 1},
		RGB(0x80,0x00,0x80),    XTP_IDS_CLR_VIOLET          ,  {7, 0},
		RGB(0xFF,0x00,0xFF),    XTP_IDS_CLR_PINK            ,  {7, 1},
		RGB(0xFF,0xFF,0x00),    XTP_IDS_CLR_YELLOW          ,  {3, 1},
		RGB(0x00,0xFF,0x00),    XTP_IDS_CLR_BRIGHT_GREEN    ,  {4, 1},
		RGB(0x00,0xFF,0xFF),    XTP_IDS_CLR_TURQUOISE       ,  {5, 1},
		RGB(0xC0,0xC0,0xC0),    XTP_IDS_CLR_GRAY25          ,  {1, 1},
		RGB(0xFF,0xFF,0xFF),    XTP_IDS_CLR_WHITE           ,  {0, 1}
	};

	CDialog::OnInitDialog();

	CRect rcTools;
	m_gboxTools.GetWindowRect(&rcTools);
	ScreenToClient(rcTools);

	m_wndToolbarTools.Create(this, WS_CHILD|WS_VISIBLE|CBRS_SIZE_FIXED|CBRS_TOOLTIPS|CBRS_ALIGN_TOP);
	m_wndToolbarTools.ModifyStyle(0,TBSTYLE_FLAT);
	m_wndToolbarTools.SetButtons(nButtonsTools, 12);


	int i;

	int nCount = m_wndToolbarTools.GetToolBarCtrl().GetButtonCount();
	for(i = 0; i < nCount; i++)
	{
		UINT nStyle = m_wndToolbarTools.GetButtonStyle(i);
		if (((i + 1) % 4) == 0) nStyle |= TBBS_WRAPPED;
		m_wndToolbarTools.SetButtonStyle(i, nStyle);
	}

	CSize sz = m_wndToolbarTools.CalcDynamicLayout(0, LM_HORZ);
	m_wndToolbarTools.SetWindowPos(&CWnd::wndTop, rcTools.left - 7, rcTools.top - 4, sz.cx, sz.cy, 0);


	CRect rcMove;
	m_gboxMove.GetWindowRect(&rcMove);
	ScreenToClient(rcMove);

	m_wndToolbarMove.Create(this, WS_CHILD|WS_VISIBLE|CBRS_SIZE_FIXED|CBRS_TOOLTIPS|CBRS_ALIGN_TOP);
	m_wndToolbarMove.ModifyStyle(0, TBSTYLE_FLAT);
	m_wndToolbarMove.SetButtons(nButtonsMove, 4);


	if (CXTPImageManager::IsAlphaIconsImageListSupported())
	{
		m_ilToolbarTools.Create(16, 16, ILC_COLOR32|ILC_MASK, 0, 1);
		HBITMAP hBitmap = CXTPImageManagerIcon::LoadAlphaBitmap(XTP_IDR_TOOLBAR_TOOLS32);
		m_ilToolbarTools.Add(CBitmap::FromHandle(hBitmap), (CBitmap*)0);

		m_ilToolbarMove.Create(16, 16, ILC_COLOR32|ILC_MASK, 0, 1);

		m_ilToolbarMove.Add(CBitmap::FromHandle(hBitmap), (CBitmap*)0);
		for (i = 0; i < 12; i++) m_ilToolbarMove.Remove(0);
		DeleteObject(hBitmap);
	}
	else
	{
		m_ilToolbarTools.Create(XTP_IDR_TOOLBAR_TOOLS, 16, 1, RGB(192, 192, 192));
		m_ilToolbarMove.Create(XTP_IDR_TOOLBAR_TOOLS, 16, 1, RGB(192, 192, 192));

		for (i = 0; i < 12; i++) m_ilToolbarMove.Remove(0);
	}

	m_wndToolbarTools.GetToolBarCtrl().SendMessage(TB_SETIMAGELIST, 0, (LPARAM)m_ilToolbarTools.GetSafeHandle());
	m_wndToolbarMove.GetToolBarCtrl().SendMessage(TB_SETIMAGELIST, 0, (LPARAM)m_ilToolbarMove.GetSafeHandle());


	sz = m_wndToolbarMove.CalcDynamicLayout(0, LM_HORZ);
	m_wndToolbarMove.SetWindowPos(&CWnd::wndTop, rcMove.left - 7, rcMove.top - 4, sz.cx, sz.cy, 0);



	CRect rcColors;
	GetDlgItem(XTP_IDC_STATIC_COLORS)->GetWindowRect(&rcColors);
	ScreenToClient(rcColors);

	for (i = 0; i < _countof(colors); i++)
	{
		CRect rcItem(CPoint(rcColors.left, rcColors.top - 4), CSize(11, 11));
		rcItem.OffsetRect(colors[i].ptOffset.x * 13 , colors[i].ptOffset.y * 13);

		m_wndSelector[i].Create(NULL, WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcItem, this, colors[i].nID);
		m_wndSelector[i].SetColor(colors[i].clr);

	}
	SetSelected(&m_wndSelector[0]);

	m_wndSelectorErase.SubclassDlgItem(XTP_ID_CLR_ERASE, this);
	m_wndSelectorErase.SetColor(XTPCOLOR_ERASE);

	m_wndPicker.SubclassDlgItem(XTP_ID_CLR_PICKER, this);


	m_wndPicture.Init(XTP_IDC_STATIC_PICTURE, this);
	m_wndPreview.Init(XTP_IDC_STATIC_PREVIEW, this);


	return TRUE;  // return TRUE  unless you set the focus to a control
}


LRESULT CXTPImageEditorDlg::OnKickIdle(WPARAM, LPARAM)
{
	SendMessageToDescendants(WM_IDLEUPDATECMDUI);
	return 0;
}

void CXTPImageEditorDlg::SetSelected(CXTPImageEditorPicker* pPicker)
{
	if (m_pSelected != NULL)
		m_pSelected->SetSelected(FALSE);

	if (pPicker)
		pPicker->SetSelected(TRUE);

	m_pSelected = pPicker;
}

void CXTPImageEditorDlg::OnPickerClick(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	CColorDialog cd(m_wndPicker.GetColor(), 0, this);
	if (cd.DoModal())
	{
		m_wndPicker.SetColor(cd.GetColor());
		SetSelected(&m_wndPicker);
	}

}
void CXTPImageEditorDlg::OnEraseClick(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	CXTPImageEditorPicker* pWnd = DYNAMIC_DOWNCAST(CXTPImageEditorPicker, CWnd::FromHandlePermanent(pNMHDR->hwndFrom));
	ASSERT(pWnd);
	SetSelected(pWnd);

}

void CXTPImageEditorDlg::OnSelectorClick(UINT /*id*/, NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	CXTPImageEditorPicker* pWnd = DYNAMIC_DOWNCAST(CXTPImageEditorPicker, CWnd::FromHandlePermanent(pNMHDR->hwndFrom));
	ASSERT(pWnd);
	SetSelected(pWnd);
}


void CXTPImageEditorDlg::OnUpdateButtonTool(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_toolSelected == (int)pCmdUI->m_nID - XTP_ID_BUTTON_PENCIL);
}

void CXTPImageEditorDlg::OnButtonTool(UINT nID)
{
	m_toolPrevious = m_toolSelected == xtpToolGetColor? m_toolPrevious: m_toolSelected;
	m_toolSelected = (XTPImageEditorTools)(nID - XTP_ID_BUTTON_PENCIL);
}


void CXTPImageEditorDlg::OnPictureChanged()
{
	m_wndPreview.Invalidate(FALSE);
}

void CXTPImageEditorDlg::OnUpdateButtonUndo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_wndPicture.m_lstUndo.IsEmpty());
}

void CXTPImageEditorDlg::OnButtonUndo()
{
	m_wndPicture.Undo();
}

void CXTPImageEditorDlg::OnUpdateButtonRedo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_wndPicture.m_lstRedo.IsEmpty());
}

void CXTPImageEditorDlg::OnButtonRedo()
{
	m_wndPicture.Redo();
}

void CXTPImageEditorDlg::OnButtonClear()
{
	m_wndPicture.Clear();
}
void CXTPImageEditorDlg::OnButtonCopy()
{
	m_wndPicture.Copy();
}
void CXTPImageEditorDlg::OnButtonPaste()
{
	m_wndPicture.Paste();
}
void CXTPImageEditorDlg::OnButtonOpen()
{
	m_wndPicture.Load();
}

void CXTPImageEditorDlg::OnMoveButton(UINT nID)
{
	m_wndPicture.MovePicture(
		nID == XTP_ID_BUTTON_LEFT? CPoint(-1, 0):
		nID == XTP_ID_BUTTON_RIGHT? CPoint(+1, 0):
		nID == XTP_ID_BUTTON_UP? CPoint(0, -1): CPoint(0, +1));
}


BOOL CXTPImageEditorDlg::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	TCHAR szFullText[256];
	CString strTipText;
	UINT nID = (UINT)pNMHDR->idFrom;

	if (nID != 0) // will be zero on a separator
	{
		// don't handle the message if no string resource found
		if (AfxLoadString(nID, szFullText) == 0)
			return FALSE;

		// this is the command id, not the button index
		AfxExtractSubString(strTipText, szFullText, 1, '\n');
	}
#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, strTipText, _countof(pTTTA->szText));
	else
		_mbstowcsz(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, strTipText, _countof(pTTTA->szText));
	else
		lstrcpyn(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#endif
	*pResult = 0;

	// bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
		SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);

	return TRUE;    // message was handled
}
