// XTColorPageStandard.cpp : implementation file
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
#include "XTPColorManager.h"
#include "XTPDrawHelpers.h"
#include "XTPWinThemeWrapper.h"

#include "XTResource.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTFunctions.h"
#include "XTWndHook.h"
#include "XTFlatComboBox.h"
#include "XTColorDialog.h"
#include "XTColorPageStandard.h"
#include "XTMemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const COLORREF CXTColorHex::clrSelArray[] =
{
	RGB(0x00, 0x33, 0x66), // row 1
	RGB(0x33, 0x66, 0x99),
	RGB(0x33, 0x66, 0xCC),
	RGB(0x00, 0x33, 0x99),
	RGB(0x00, 0x00, 0x99),
	RGB(0x00, 0x00, 0xCC),
	RGB(0x00, 0x00, 0x66),

	RGB(0x00, 0x66, 0x66), // row 2
	RGB(0x00, 0x66, 0x99),
	RGB(0x00, 0x99, 0xCC),
	RGB(0x00, 0x66, 0xCC),
	RGB(0x00, 0x33, 0xCC),
	RGB(0x00, 0x00, 0xFF),
	RGB(0x33, 0x33, 0xFF),
	RGB(0x33, 0x33, 0x99),

	RGB(0x00, 0x80, 0x80), // row 3
	RGB(0x00, 0x99, 0x99),
	RGB(0x33, 0xCC, 0xCC),
	RGB(0x00, 0xCC, 0xFF),
	RGB(0x00, 0x99, 0xFF),
	RGB(0x00, 0x66, 0xFF),
	RGB(0x33, 0x66, 0xFF),
	RGB(0x33, 0x33, 0xCC),
	RGB(0x66, 0x66, 0x99),

	RGB(0x33, 0x99, 0x66), // row 4
	RGB(0x00, 0xCC, 0x99),
	RGB(0x00, 0xFF, 0xCC),
	RGB(0x00, 0xFF, 0xFF),
	RGB(0x33, 0xCC, 0xFF),
	RGB(0x33, 0x99, 0xFF),
	RGB(0x66, 0x99, 0xFF),
	RGB(0x66, 0x66, 0xFF),
	RGB(0x66, 0x00, 0xFF),
	RGB(0x66, 0x00, 0xCC),

	RGB(0x33, 0x99, 0x33), // row 5
	RGB(0x00, 0xCC, 0x66),
	RGB(0x00, 0xFF, 0x99),
	RGB(0x66, 0xFF, 0xCC),
	RGB(0x66, 0xFF, 0xFF),
	RGB(0x66, 0xCC, 0xFF),
	RGB(0x99, 0xCC, 0xFF),
	RGB(0x99, 0x99, 0xFF),
	RGB(0x99, 0x66, 0xFF),
	RGB(0x99, 0x33, 0xFF),
	RGB(0x99, 0x00, 0xFF),

	RGB(0x00, 0x66, 0x00), // row 6
	RGB(0x00, 0xCC, 0x00),
	RGB(0x00, 0xFF, 0x00),
	RGB(0x66, 0xFF, 0x99),
	RGB(0x99, 0xFF, 0xCC),
	RGB(0xCC, 0xFF, 0xFF),
	RGB(0xCC, 0xEC, 0xFF),
	RGB(0xCC, 0xCC, 0xFF),
	RGB(0xCC, 0x99, 0xFF),
	RGB(0xCC, 0x66, 0xFF),
	RGB(0xCC, 0x00, 0xFF),
	RGB(0x99, 0x00, 0xCC),

	RGB(0x00, 0x33, 0x00), // row 7
	RGB(0x00, 0x80, 0x00),
	RGB(0x33, 0xCC, 0x33),
	RGB(0x66, 0xFF, 0x66),
	RGB(0x99, 0xFF, 0x99),
	RGB(0xCC, 0xFF, 0xCC),
	RGB(0xFF, 0xFF, 0xFF),
	RGB(0xFF, 0xCC, 0xFF),
	RGB(0xFF, 0x99, 0xFF),
	RGB(0xFF, 0x66, 0xFF),
	RGB(0xFF, 0x00, 0xFF),
	RGB(0xCC, 0x00, 0xCC),
	RGB(0x66, 0x00, 0x66),

	RGB(0x33, 0x66, 0x00), // row 8
	RGB(0x00, 0x99, 0x00),
	RGB(0x66, 0xFF, 0x33),
	RGB(0x99, 0xFF, 0x66),
	RGB(0xCC, 0xFF, 0x99),
	RGB(0xFF, 0xFF, 0xCC),
	RGB(0xFF, 0xCC, 0xCC),
	RGB(0xFF, 0x99, 0xCC),
	RGB(0xFF, 0x66, 0xCC),
	RGB(0xFF, 0x33, 0xCC),
	RGB(0xCC, 0x00, 0x99),
	RGB(0x80, 0x00, 0x80),

	RGB(0x33, 0x33, 0x00), // row 9
	RGB(0x66, 0x99, 0x00),
	RGB(0x99, 0xFF, 0x33),
	RGB(0xCC, 0xFF, 0x66),
	RGB(0xFF, 0xFF, 0x99),
	RGB(0xFF, 0xCC, 0x99),
	RGB(0xFF, 0x99, 0x99),
	RGB(0xFF, 0x66, 0x99),
	RGB(0xFF, 0x33, 0x99),
	RGB(0xCC, 0x33, 0x99),
	RGB(0x99, 0x00, 0x99),

	RGB(0x66, 0x66, 0x33), // row 10
	RGB(0x99, 0xCC, 0x00),
	RGB(0xCC, 0xFF, 0x33),
	RGB(0xFF, 0xFF, 0x66),
	RGB(0xFF, 0xCC, 0x66),
	RGB(0xFF, 0x99, 0x66),
	RGB(0xFF, 0x7C, 0x80),
	RGB(0xFF, 0x00, 0x66),
	RGB(0xD6, 0x00, 0x93),
	RGB(0x99, 0x33, 0x66),

	RGB(0x80, 0x80, 0x00), // row 11
	RGB(0xCC, 0xCC, 0x00),
	RGB(0xFF, 0xFF, 0x00),
	RGB(0xFF, 0xCC, 0x00),
	RGB(0xFF, 0x99, 0x33),
	RGB(0xFF, 0x66, 0x00),
	RGB(0xFF, 0x50, 0x50),
	RGB(0xCC, 0x00, 0x66),
	RGB(0x66, 0x00, 0x33),

	RGB(0x99, 0x66, 0x33), // row 12
	RGB(0xCC, 0x99, 0x00),
	RGB(0xFF, 0x99, 0x00),
	RGB(0xCC, 0x66, 0x00),
	RGB(0xFF, 0x33, 0x00),
	RGB(0xFF, 0x00, 0x00),
	RGB(0xCC, 0x00, 0x00),
	RGB(0x99, 0x00, 0x33),

	RGB(0x66, 0x33, 0x00), // row 13
	RGB(0x99, 0x66, 0x00),
	RGB(0xCC, 0x33, 0x00),
	RGB(0x99, 0x33, 0x00),
	RGB(0x99, 0x00, 0x00),
	RGB(0x80, 0x00, 0x00),
	RGB(0xA5, 0x00, 0x21),

	RGB(0xF8, 0xF8, 0xF8), // row 1 ( gray scale )
	RGB(0xDD, 0xDD, 0xDD),
	RGB(0xB2, 0xB2, 0xB2),
	RGB(0x80, 0x80, 0x80),
	RGB(0x5F, 0x5F, 0x5F),
	RGB(0x33, 0x33, 0x33),
	RGB(0x1C, 0x1C, 0x1C),
	RGB(0x08, 0x08, 0x08),

	RGB(0xEA, 0xEA, 0xEA), // row 2 ( gray scale )
	RGB(0xC0, 0xC0, 0xC0),
	RGB(0x96, 0x96, 0x96),
	RGB(0x77, 0x77, 0x77),
	RGB(0x4D, 0x4D, 0x4D),
	RGB(0x29, 0x29, 0x29),
	RGB(0x11, 0x11, 0x11),

	RGB(0xFF, 0xFF, 0xFF),  // large white cell
	RGB(0x00, 0x00, 0x00)   // large black cell
};

bool CXTColorHex::IsValidColor(COLORREF cr) const
{
	int i;
	for (i = 0; i < _countof( clrSelArray ); ++i)
	{
		if ( cr == clrSelArray[i] )
		{
			return true;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CXTColorHex

CXTColorHex::CXTColorHex()
{
	m_bSmallCell   = TRUE;
	m_clrColor     = (COLORREF)-1;
	m_bLBtnDown    = FALSE;
	m_bInitControl = true;
}

CXTColorHex::~CXTColorHex()
{
	while (!m_arCells.IsEmpty())
	{
		HEXCOLOR_CELL* pCI = m_arCells.RemoveHead();
		SAFE_DELETE_AR (pCI->pPoint);
		SAFE_DELETE (pCI);
	}
}

BEGIN_MESSAGE_MAP(CXTColorHex, CStatic)
	//{{AFX_MSG_MAP(CXTColorHex)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTWM_INITIAL_UPDATE, OnInitControl)
END_MESSAGE_MAP()

#define CX_ANCHOR   42
#define CY_ANCHOR   3
#define CX_HEXDISP  182
#define CY_HEXDISP  196

void CXTColorHex::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);
	CXTMemDC memDC(&dc, rc);

	if (m_dcPicker.GetSafeHdc() == NULL)
	{
		// create bitmap
		CBitmap bmpPicker;
		bmpPicker.CreateCompatibleBitmap(&memDC,
			rc.Size().cx, rc.Size().cy);

		// create picker DC
		m_dcPicker.CreateCompatibleDC(&memDC);
		m_dcPicker.SelectObject(&bmpPicker);

		m_dcPicker.SetBkMode(TRANSPARENT);

		CXTColorPageStandard* pParent = DYNAMIC_DOWNCAST(CXTColorPageStandard, GetParent());
		if (pParent)
		{
			pParent->DrawThemedBack(&m_dcPicker, rc);
		}
		else
		{
			m_dcPicker.FillSolidRect(&rc,
				GetXtremeColor(COLOR_3DFACE));
		}

		DrawColorSelector(&m_dcPicker);
	}

	memDC.BitBlt(0, 0, rc.Size().cx, rc.Size().cy, &m_dcPicker, 0, 0, SRCCOPY);
	SelectColorCell(&memDC);
}

BOOL CXTColorHex::OnEraseBkgnd(CDC* pDC)
{
	UNUSED_ALWAYS(pDC);
	return TRUE;
}

void CXTColorHex::SelectColorCell(CDC* pDC)
{
	CXTColorDialog* pParentWnd = (CXTColorDialog*)GetParent()->GetParent();
	ASSERT(pParentWnd);
	if (pParentWnd->GetColor() == m_clrColor)
	{
		if (m_clrColor == RGB(0xff,0xff,0xff))
		{
			CXTPClientRect rcClient(this);
			CPoint ptCurrent = m_ptCurrent;

			m_ptCurrent.x = (rcClient.Width()-14)/2;
			m_ptCurrent.y = 74 + CY_ANCHOR;
			DrawSelectCell(pDC);

			m_ptCurrent.x = (rcClient.Width()-CX_HEXDISP)/2;
			m_ptCurrent.y = 171 + CY_ANCHOR;
			DrawLargeSelectCell(pDC);

			m_ptCurrent = ptCurrent;
		}
		else if (m_clrColor == RGB(0x00,0x00,0x00))
		{
			DrawLargeSelectCell(pDC);
		}
		else if ( IsValidColor( m_clrColor ) )
		{
			if (m_bSmallCell)
			{
				DrawSelectCell(pDC);
			}
			else
			{
				DrawLargeSelectCell(pDC);
			}
		}
	}
}

void CXTColorHex::DrawLargeSelectCell(CDC *pDC)
{
	int x = m_ptCurrent.x;
	int y = m_ptCurrent.y;
	x -= 2;

	COLORREF clr = RGB(0x00,0x00,0x00);

	// outer border
	int i;
	for (i = 0; i < 3; ++i)
		pDC->SetPixel(x+i, y-i, clr);

	pDC->SetPixel(x+3, y-2, clr);
	pDC->SetPixel(x+4, y-3, clr);
	pDC->SetPixel(x+5, y-3, clr);
	pDC->SetPixel(x+6, y-4, clr);
	pDC->SetPixel(x+7, y-4, clr);
	pDC->SetPixel(x+8, y-5, clr);
	pDC->SetPixel(x+9, y-5, clr);
	pDC->SetPixel(x+10, y-6, clr);
	pDC->SetPixel(x+11, y-6, clr);
	pDC->SetPixel(x+12, y-7, clr);
	pDC->SetPixel(x+13, y-7, clr);
	pDC->SetPixel(x+14, y-8, clr);
	pDC->SetPixel(x+15, y-8, clr);
	pDC->SetPixel(x+16, y-8, clr);
	pDC->SetPixel(x+17, y-8, clr);
	pDC->SetPixel(x+18, y-8, clr);
	pDC->SetPixel(x+19, y-7, clr);
	pDC->SetPixel(x+20, y-7, clr);
	pDC->SetPixel(x+21, y-6, clr);
	pDC->SetPixel(x+22, y-6, clr);
	pDC->SetPixel(x+23, y-5, clr);
	pDC->SetPixel(x+24, y-5, clr);
	pDC->SetPixel(x+25, y-4, clr);
	pDC->SetPixel(x+26, y-4, clr);
	pDC->SetPixel(x+27, y-3, clr);
	pDC->SetPixel(x+28, y-3, clr);
	pDC->SetPixel(x+29, y-2, clr);
	pDC->SetPixel(x+30, y-2, clr);
	pDC->SetPixel(x+31, y-1, clr);

	for (i = 0; i < 17; ++i)
		pDC->SetPixel(x+32, y+i, clr);

	pDC->SetPixel(x+31, y+17, clr);
	pDC->SetPixel(x+30, y+18, clr);
	pDC->SetPixel(x+29, y+18, clr);
	pDC->SetPixel(x+28, y+19, clr);
	pDC->SetPixel(x+27, y+19, clr);
	pDC->SetPixel(x+26, y+20, clr);
	pDC->SetPixel(x+25, y+20, clr);
	pDC->SetPixel(x+24, y+21, clr);
	pDC->SetPixel(x+23, y+21, clr);
	pDC->SetPixel(x+22, y+22, clr);
	pDC->SetPixel(x+21, y+22, clr);
	pDC->SetPixel(x+20, y+23, clr);
	pDC->SetPixel(x+19, y+23, clr);
	pDC->SetPixel(x+18, y+24, clr);
	pDC->SetPixel(x+17, y+24, clr);
	pDC->SetPixel(x+16, y+24, clr);
	pDC->SetPixel(x+15, y+24, clr);
	pDC->SetPixel(x+14, y+24, clr);
	pDC->SetPixel(x+13, y+23, clr);
	pDC->SetPixel(x+12, y+23, clr);
	pDC->SetPixel(x+11, y+22, clr);
	pDC->SetPixel(x+10, y+22, clr);
	pDC->SetPixel(x+9, y+21, clr);
	pDC->SetPixel(x+8, y+21, clr);
	pDC->SetPixel(x+7, y+20, clr);
	pDC->SetPixel(x+6, y+20, clr);
	pDC->SetPixel(x+5, y+19, clr);
	pDC->SetPixel(x+4, y+19, clr);
	pDC->SetPixel(x+3, y+18, clr);
	pDC->SetPixel(x+2, y+18, clr);
	pDC->SetPixel(x+1, y+17, clr);

	for (i = 0; i < 17; ++i)
		pDC->SetPixel(x, y+i, clr);

	// inner border
	for (i = 0; i < 15; ++i)
		pDC->SetPixel(x+4, y+1+i, clr);

	pDC->SetPixel(x+5, y+16, clr);
	pDC->SetPixel(x+6, y+16, clr);
	pDC->SetPixel(x+7, y+17, clr);
	pDC->SetPixel(x+8, y+17, clr);
	pDC->SetPixel(x+9,  y+18, clr);
	pDC->SetPixel(x+10, y+18, clr);
	pDC->SetPixel(x+11, y+19, clr);
	pDC->SetPixel(x+12, y+19, clr);
	pDC->SetPixel(x+13, y+20, clr);
	pDC->SetPixel(x+14, y+20, clr);
	pDC->SetPixel(x+15, y+21, clr);
	pDC->SetPixel(x+16, y+21, clr);
	pDC->SetPixel(x+17, y+21, clr);
	pDC->SetPixel(x+18, y+20, clr);
	pDC->SetPixel(x+19, y+20, clr);
	pDC->SetPixel(x+20, y+19, clr);
	pDC->SetPixel(x+21, y+19, clr);
	pDC->SetPixel(x+22, y+18, clr);
	pDC->SetPixel(x+23, y+18, clr);
	pDC->SetPixel(x+24, y+17, clr);
	pDC->SetPixel(x+25, y+17, clr);
	pDC->SetPixel(x+26, y+16, clr);
	pDC->SetPixel(x+27, y+16, clr);

	for (i = 0; i < 15; ++i)
		pDC->SetPixel(x+28, y+1+i, clr);

	pDC->SetPixel(x+27, y, clr);
	pDC->SetPixel(x+26, y, clr);
	pDC->SetPixel(x+25, y-1, clr);
	pDC->SetPixel(x+24, y-1, clr);
	pDC->SetPixel(x+23, y-2, clr);
	pDC->SetPixel(x+22, y-2, clr);
	pDC->SetPixel(x+21, y-3, clr);
	pDC->SetPixel(x+20, y-3, clr);
	pDC->SetPixel(x+19, y-4, clr);
	pDC->SetPixel(x+18, y-4, clr);
	pDC->SetPixel(x+17, y-5, clr);
	pDC->SetPixel(x+16, y-5, clr);
	pDC->SetPixel(x+15, y-5, clr);
	pDC->SetPixel(x+14, y-4, clr);
	pDC->SetPixel(x+13, y-4, clr);
	pDC->SetPixel(x+12, y-3, clr);
	pDC->SetPixel(x+11, y-3, clr);
	pDC->SetPixel(x+10, y-2, clr);
	pDC->SetPixel(x+9,  y-2, clr);
	pDC->SetPixel(x+8, y-1, clr);
	pDC->SetPixel(x+7, y-1, clr);
	pDC->SetPixel(x+6, y, clr);
	pDC->SetPixel(x+5, y, clr);

	// fill inside.
	clr = RGB(0xff,0xff,0xff);

	for (i = 0; i < 17; ++i)
	{
		pDC->SetPixel(x+1, y+i, clr);
		pDC->SetPixel(x+2, y+i, clr);
		pDC->SetPixel(x+3, y+i, clr);
	}

	pDC->SetPixel(x+4, y, clr);
	pDC->SetPixel(x+4, y+16, clr);

	for (i = 0; i < 17; ++i)
	{
		pDC->SetPixel(x+29, y+i, clr);
		pDC->SetPixel(x+30, y+i, clr);
		pDC->SetPixel(x+31, y+i, clr);
	}

	pDC->SetPixel(x+28, y, clr);
	pDC->SetPixel(x+28, y+16, clr);

	for (i = 0; i < 5; ++i)
	{
		pDC->SetPixel(x+2+i, y-1, clr);
		pDC->SetPixel(x+4+i, y-2, clr);
		pDC->SetPixel(x+6+i, y-3, clr);
		pDC->SetPixel(x+8+i, y-4, clr);
		pDC->SetPixel(x+10+i, y-5, clr);
		pDC->SetPixel(x+12+i, y-6, clr);
		pDC->SetPixel(x+16+i, y-6, clr);
		pDC->SetPixel(x+14+i, y-7, clr);
		pDC->SetPixel(x+18+i, y-5, clr);
		pDC->SetPixel(x+20+i, y-4, clr);
		pDC->SetPixel(x+22+i, y-3, clr);
		pDC->SetPixel(x+24+i, y-2, clr);
		pDC->SetPixel(x+26+i, y-1, clr);
		pDC->SetPixel(x+26+i, y+17, clr);
		pDC->SetPixel(x+24+i, y+18, clr);
		pDC->SetPixel(x+22+i, y+19, clr);
		pDC->SetPixel(x+20+i, y+20, clr);
		pDC->SetPixel(x+18+i, y+21, clr);
		pDC->SetPixel(x+14+i, y+23, clr);
		pDC->SetPixel(x+16+i, y+22, clr);
		pDC->SetPixel(x+12+i, y+22, clr);
		pDC->SetPixel(x+10+i, y+21, clr);
		pDC->SetPixel(x+8+i, y+20, clr);
		pDC->SetPixel(x+6+i, y+19, clr);
		pDC->SetPixel(x+4+i, y+18, clr);
		pDC->SetPixel(x+2+i, y+17, clr);
	}
}

void CXTColorHex::DrawSelectCell(CDC *pDC)
{
	COLORREF clr = RGB(0x00,0x00,0x00);
	int x = m_ptCurrent.x;
	int y = m_ptCurrent.y;
	x -= 2; y -= 1;

	// outer border
	int i;
	for (i = 0; i < 11; ++i)
		pDC->SetPixel(x, y+i, clr);

	pDC->SetPixel(x+1, y+11, clr);
	pDC->SetPixel(x+2, y+11, clr);
	pDC->SetPixel(x+3, y+12, clr);
	pDC->SetPixel(x+4, y+12, clr);
	pDC->SetPixel(x+5, y+13, clr);
	pDC->SetPixel(x+6, y+13, clr);

	for ( i = 0; i < 5; ++i)
		pDC->SetPixel(x+7+i, y+14, clr);

	pDC->SetPixel(x+12, y+13, clr);
	pDC->SetPixel(x+13, y+13, clr);
	pDC->SetPixel(x+14, y+12, clr);
	pDC->SetPixel(x+15, y+12, clr);
	pDC->SetPixel(x+16, y+11, clr);
	pDC->SetPixel(x+17, y+11, clr);

	for ( i = 0; i < 11; ++i)
		pDC->SetPixel(x+18, y+i, clr);

	pDC->SetPixel(x+16, y-1, clr);
	pDC->SetPixel(x+17, y-1, clr);
	pDC->SetPixel(x+14, y-2, clr);
	pDC->SetPixel(x+15, y-2, clr);
	pDC->SetPixel(x+12, y-3, clr);
	pDC->SetPixel(x+13, y-3, clr);

	for ( i = 0; i < 5; ++i)
		pDC->SetPixel(x+7+i, y-4, clr);

	pDC->SetPixel(x+5, y-3, clr);
	pDC->SetPixel(x+6, y-3, clr);
	pDC->SetPixel(x+3, y-2, clr);
	pDC->SetPixel(x+4, y-2, clr);
	pDC->SetPixel(x+1, y-1, clr);
	pDC->SetPixel(x+2, y-1, clr);

	// inner border
	for ( i = 0; i < 7; ++i)
		pDC->SetPixel(x+3, y+2+i, clr);

	pDC->SetPixel(x+4, y+9, clr);
	pDC->SetPixel(x+5, y+9, clr);
	pDC->SetPixel(x+6, y+10, clr);
	pDC->SetPixel(x+7, y+10, clr);

	for ( i = 0; i < 3; ++i)
		pDC->SetPixel(x+8+i, y+11, clr);

	pDC->SetPixel(x+11, y+10, clr);
	pDC->SetPixel(x+12, y+10, clr);
	pDC->SetPixel(x+13, y+9, clr);
	pDC->SetPixel(x+14, y+9, clr);

	for ( i = 0; i < 7; ++i)
		pDC->SetPixel(x+15, y+2+i, clr);

	pDC->SetPixel(x+13, y+1, clr);
	pDC->SetPixel(x+14, y+1, clr);
	pDC->SetPixel(x+11, y, clr);
	pDC->SetPixel(x+12, y, clr);

	for ( i = 0; i < 3; ++i)
		pDC->SetPixel(x+8+i, y-1, clr);

	pDC->SetPixel(x+6, y, clr);
	pDC->SetPixel(x+7, y, clr);
	pDC->SetPixel(x+4, y+1, clr);
	pDC->SetPixel(x+5, y+1, clr);

	// fill inside.
	clr = RGB(0xff,0xff,0xff);
	x++;

	for ( i = 0; i < 11; ++i)
	{
		pDC->SetPixel(x, y+i, clr);
		pDC->SetPixel(x+1, y+i, clr);
	}

	for ( i = 0; i < 3; ++i)
		pDC->SetPixel(x+2, y+9+i, clr);

	pDC->SetPixel(x+3, y+10, clr);
	pDC->SetPixel(x+3, y+11, clr);

	for ( i = 0; i < 3; ++i)
		pDC->SetPixel(x+4, y+10+i, clr);

	pDC->SetPixel(x+5, y+11, clr);
	pDC->SetPixel(x+5, y+12, clr);

	for ( i = 0; i < 3; ++i)
		pDC->SetPixel(x+6, y+11+i, clr);

	pDC->SetPixel(x+7, y+12, clr);
	pDC->SetPixel(x+7, y+13, clr);
	pDC->SetPixel(x+8, y+12, clr);
	pDC->SetPixel(x+8, y+13, clr);
	pDC->SetPixel(x+9, y+12, clr);
	pDC->SetPixel(x+9, y+13, clr);

	for ( i = 0; i < 3; ++i)
		pDC->SetPixel(x+10, y+11+i, clr);

	pDC->SetPixel(x+11, y+11, clr);
	pDC->SetPixel(x+11, y+12, clr);

	for ( i = 0; i < 3; ++i)
		pDC->SetPixel(x+12, y+10+i, clr);

	pDC->SetPixel(x+13, y+10, clr);
	pDC->SetPixel(x+13, y+11, clr);

	for ( i = 0; i < 3; ++i)
		pDC->SetPixel(x+14, y+9+i, clr);

	for ( i = 0; i < 11; ++i)
	{
		pDC->SetPixel(x+15, y+i, clr);
		pDC->SetPixel(x+16, y+i, clr);
	}

	for ( i = 0; i < 3; ++i)
		pDC->SetPixel(x+14, y-1+i, clr);

	pDC->SetPixel(x+13, y, clr);
	pDC->SetPixel(x+13, y-1, clr);

	for ( i = 0; i < 3; ++i)
		pDC->SetPixel(x+12, y-2+i, clr);

	pDC->SetPixel(x+11, y-1, clr);
	pDC->SetPixel(x+11, y-2, clr);

	for ( i = 0; i < 3; ++i)
		pDC->SetPixel(x+10, y-3+i, clr);

	pDC->SetPixel(x+7, y-2, clr);
	pDC->SetPixel(x+7, y-3, clr);
	pDC->SetPixel(x+8, y-2, clr);
	pDC->SetPixel(x+8, y-3, clr);
	pDC->SetPixel(x+9, y-2, clr);
	pDC->SetPixel(x+9, y-3, clr);

	for ( i = 0; i < 3; ++i)
		pDC->SetPixel(x+6, y-3+i, clr);

	pDC->SetPixel(x+5, y-1, clr);
	pDC->SetPixel(x+5, y-2, clr);

	for ( i = 0; i < 3; ++i)
		pDC->SetPixel(x+4, y-2+i, clr);

	pDC->SetPixel(x+3, y, clr);
	pDC->SetPixel(x+3, y-1, clr);

	for ( i = 0; i < 3; ++i)
		pDC->SetPixel(x+2, y-1+i, clr);
}

void CXTColorHex::DrawCell(CDC *pDC, CPoint point, COLORREF clr, int l, int u, int r, int d)
{
	HEXCOLOR_CELL* pCI = new HEXCOLOR_CELL;
	ASSERT(pCI != NULL);

	CPen penNew(PS_SOLID, 1, clr);
	CPen* pOldPen = pDC->SelectObject(&penNew);

	pCI->clr          = clr;
	pCI->bSmall       = TRUE;
	pCI->pPoint       = new CPoint[168];
	pCI->direction[0] = l;
	pCI->direction[1] = u;
	pCI->direction[2] = r;
	pCI->direction[3] = d;

	m_arCells.AddTail(pCI);

	if (clr == m_clrColor)
	{
		m_ptCurrent.x = point.x;
		m_ptCurrent.y = point.y;
	}

	int idx = 0;
	int i;
	for (i = 0; i < 9; ++i)
	{
		CPoint point1(point.x, point.y+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
		CPoint point2(point.x+1, point.y+i);
		pDC->SetPixel(point2, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point2;
			idx++;
		}
	}
	for (i = 0; i < 11; ++i)
	{
		CPoint point1(point.x+2, (point.y-1)+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
		CPoint point2(point.x+3, (point.y-1)+i);
		pDC->SetPixel(point2, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point2;
			idx++;
		}
	}
	for (i = 0; i < 13; ++i)
	{
		CPoint point1(point.x+4, (point.y-2)+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
		CPoint point2(point.x+5, (point.y-2)+i);
		pDC->SetPixel(point2, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point2;
			idx++;
		}
	}
	for (i = 0; i < 15; ++i)
	{
		CPoint point1(point.x+6, (point.y-3)+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
		CPoint point2(point.x+7, (point.y-3)+i);
		pDC->SetPixel(point2, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point2;
			idx++;
		}
		CPoint point3(point.x+8, (point.y-3)+i);
		pDC->SetPixel(point3, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point3;
			idx++;
		}
	}
	for (i = 0; i < 13; ++i)
	{
		CPoint point1(point.x+9, (point.y-2)+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
		CPoint point2(point.x+10, (point.y-2)+i);
		pDC->SetPixel(point2, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point2;
			idx++;
		}
	}
	for (i = 0; i < 11; ++i)
	{
		CPoint point1(point.x+11, (point.y-1)+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
		CPoint point2(point.x+12, (point.y-1)+i);
		pDC->SetPixel(point2, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point2;
			idx++;
		}
	}
	for (i = 0; i < 9; ++i)
	{
		CPoint point1(point.x+13, point.y+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
	}

	pDC->SelectObject(pOldPen);
}

void CXTColorHex::DrawLargeCell(CDC *pDC, CPoint point, COLORREF clr, int l, int u, int r, int d)
{
	HEXCOLOR_CELL* pCI = new HEXCOLOR_CELL;
	ASSERT(pCI != NULL);

	CPen penNew(PS_SOLID, 1, clr);
	CPen* pOldPen = pDC->SelectObject(&penNew);

	pCI->clr          = clr;
	pCI->bSmall       = FALSE;
	pCI->pPoint       = new CPoint[691];
	pCI->direction[0] = l;
	pCI->direction[1] = u;
	pCI->direction[2] = r;
	pCI->direction[3] = d;

	m_arCells.AddTail(pCI);

	if (clr == m_clrColor)
	{
		m_ptCurrent.x = point.x;
		m_ptCurrent.y = point.y;
	}

	int idx = 0;
	int i;
	for (i = 0; i < 17; ++i)
	{
		CPoint point1(point.x, point.y+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
		CPoint point2(point.x+1, point.y+i);
		pDC->SetPixel(point2, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point2;
			idx++;
		}
	}
	for (i = 0; i < 19; ++i)
	{
		CPoint point1(point.x+2, (point.y-1)+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
		CPoint point2(point.x+3, (point.y-1)+i);
		pDC->SetPixel(point2, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point2;
			idx++;
		}
	}
	for (i = 0; i < 21; ++i)
	{
		CPoint point1(point.x+4, (point.y-2)+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
		CPoint point2(point.x+5, (point.y-2)+i);
		pDC->SetPixel(point2, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point2;
			idx++;
		}
	}
	for (i = 0; i < 23; ++i)
	{
		CPoint point1(point.x+6, (point.y-3)+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
	}
	for (i = 0; i < 25; ++i)
	{
		CPoint point1(point.x+7, (point.y-4)+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
		CPoint point2(point.x+8, (point.y-4)+i);
		pDC->SetPixel(point2, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point2;
			idx++;
		}
	}
	for (i = 0; i < 27; ++i)
	{
		CPoint point1(point.x+9, (point.y-5)+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
		CPoint point2(point.x+10, (point.y-5)+i);
		pDC->SetPixel(point2, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point2;
			idx++;
		}
	}
	for (i = 0; i < 29; ++i)
	{
		CPoint point1(point.x+11, (point.y-6)+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
		CPoint point2(point.x+12, (point.y-6)+i);
		pDC->SetPixel(point2, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point2;
			idx++;
		}
	}
	for (i = 0; i < 31; ++i)
	{
		CPoint point1(point.x+13, (point.y-7)+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
		CPoint point2(point.x+14, (point.y-7)+i);
		pDC->SetPixel(point2, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point2;
			idx++;
		}
		CPoint point3(point.x+15, (point.y-7)+i);
		pDC->SetPixel(point3, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point3;
			idx++;
		}
	}
	for (i = 0; i < 29; ++i)
	{
		CPoint point1(point.x+16, (point.y-6)+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
		CPoint point2(point.x+17, (point.y-6)+i);
		pDC->SetPixel(point2, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point2;
			idx++;
		}
	}
	for (i = 0; i < 27; ++i)
	{
		CPoint point1(point.x+18, (point.y-5)+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
		CPoint point2(point.x+19, (point.y-5)+i);
		pDC->SetPixel(point2, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point2;
			idx++;
		}
	}
	for (i = 0; i < 25; ++i)
	{
		CPoint point1(point.x+20, (point.y-4)+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
		CPoint point2(point.x+21, (point.y-4)+i);
		pDC->SetPixel(point2, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point2;
			idx++;
		}
	}
	for (i = 0; i < 23; ++i)
	{
		CPoint point1(point.x+22, (point.y-3)+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
	}
	for (i = 0; i < 21; ++i)
	{
		CPoint point1(point.x+23, (point.y-2)+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
		CPoint point2(point.x+24, (point.y-2)+i);
		pDC->SetPixel(point2, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point2;
			idx++;
		}
	}
	for (i = 0; i < 19; ++i)
	{
		CPoint point1(point.x+25, (point.y-1)+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
		CPoint point2(point.x+26, (point.y-1)+i);
		pDC->SetPixel(point2, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point2;
			idx++;
		}
	}
	for (i = 0; i < 17; ++i)
	{
		CPoint point1(point.x+27, point.y+i);
		pDC->SetPixel(point1, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point1;
			idx++;
		}
		CPoint point2(point.x+28, point.y+i);
		pDC->SetPixel(point2, clr);
		if (pCI != NULL) {
			pCI->pPoint[idx] = point2;
			idx++;
		}
	}

	pDC->SelectObject(pOldPen);
}

void CXTColorHex::DrawColorSelector(CDC *pDC)
{
	CXTPClientRect rcClient(this);

	int x = ((rcClient.Width()-CX_HEXDISP)/2)+CX_ANCHOR;
	int y = CY_ANCHOR;

	int i = -1;
	DrawCell(pDC, CPoint(x,    y), clrSelArray[++i], -1, -1,  1, 8);
	DrawCell(pDC, CPoint(x+14, y), clrSelArray[++i],  0, -1,  2, 9);
	DrawCell(pDC, CPoint(x+28, y), clrSelArray[++i],  1, -1,  3, 10);
	DrawCell(pDC, CPoint(x+42, y), clrSelArray[++i],  2, -1,  4, 11);
	DrawCell(pDC, CPoint(x+56, y), clrSelArray[++i],  3, -1,  5, 12);
	DrawCell(pDC, CPoint(x+70, y), clrSelArray[++i],  4, -1,  6, 13);
	DrawCell(pDC, CPoint(x+84, y), clrSelArray[++i],  5, -1,  7, 14);

	x -= 7; y += 12;
	DrawCell(pDC, CPoint(x,    y), clrSelArray[++i],  6, -1,  8, 16);
	DrawCell(pDC, CPoint(x+14, y), clrSelArray[++i],  7,  0,  9, 17);
	DrawCell(pDC, CPoint(x+28, y), clrSelArray[++i],  8,  1, 10, 18);
	DrawCell(pDC, CPoint(x+42, y), clrSelArray[++i],  9,  2, 11, 19);
	DrawCell(pDC, CPoint(x+56, y), clrSelArray[++i], 10,  3, 12, 20);
	DrawCell(pDC, CPoint(x+70, y), clrSelArray[++i], 11,  4, 13, 21);
	DrawCell(pDC, CPoint(x+84, y), clrSelArray[++i], 12,  5, 14, 22);
	DrawCell(pDC, CPoint(x+98, y), clrSelArray[++i], 13,  6, 15, 23);

	x -= 7; y += 12;
	DrawCell(pDC, CPoint(x,     y), clrSelArray[++i], 14,  6, 16, 25);
	DrawCell(pDC, CPoint(x+14,  y), clrSelArray[++i], 15,  7, 17, 26);
	DrawCell(pDC, CPoint(x+28,  y), clrSelArray[++i], 16,  8, 18, 27);
	DrawCell(pDC, CPoint(x+42,  y), clrSelArray[++i], 17,  9, 19, 28);
	DrawCell(pDC, CPoint(x+56,  y), clrSelArray[++i], 18, 10, 20, 29);
	DrawCell(pDC, CPoint(x+70,  y), clrSelArray[++i], 19, 11, 21, 30);
	DrawCell(pDC, CPoint(x+84,  y), clrSelArray[++i], 20, 12, 22, 31);
	DrawCell(pDC, CPoint(x+98,  y), clrSelArray[++i], 21, 13, 23, 32);
	DrawCell(pDC, CPoint(x+112, y), clrSelArray[++i], 22, 14, 24, 33);

	x -= 7; y += 12;
	DrawCell(pDC, CPoint(x,     y), clrSelArray[++i], 23, 14, 25, 35);
	DrawCell(pDC, CPoint(x+14,  y), clrSelArray[++i], 24, 15, 26, 36);
	DrawCell(pDC, CPoint(x+28,  y), clrSelArray[++i], 25, 16, 27, 37);
	DrawCell(pDC, CPoint(x+42,  y), clrSelArray[++i], 26, 17, 28, 38);
	DrawCell(pDC, CPoint(x+56,  y), clrSelArray[++i], 27, 18, 29, 39);
	DrawCell(pDC, CPoint(x+70,  y), clrSelArray[++i], 28, 19, 30, 40);
	DrawCell(pDC, CPoint(x+84,  y), clrSelArray[++i], 29, 20, 31, 41);
	DrawCell(pDC, CPoint(x+98,  y), clrSelArray[++i], 30, 21, 32, 42);
	DrawCell(pDC, CPoint(x+112, y), clrSelArray[++i], 31, 22, 33, 43);
	DrawCell(pDC, CPoint(x+126, y), clrSelArray[++i], 32, 23, 34, 44);

	x -= 7; y += 12;
	DrawCell(pDC, CPoint(x,     y), clrSelArray[++i], 33, 23, 35, 46);
	DrawCell(pDC, CPoint(x+14,  y), clrSelArray[++i], 34, 24, 36, 47);
	DrawCell(pDC, CPoint(x+28,  y), clrSelArray[++i], 35, 25, 37, 48);
	DrawCell(pDC, CPoint(x+42,  y), clrSelArray[++i], 36, 26, 38, 49);
	DrawCell(pDC, CPoint(x+56,  y), clrSelArray[++i], 37, 27, 39, 50);
	DrawCell(pDC, CPoint(x+70,  y), clrSelArray[++i], 38, 28, 40, 51);
	DrawCell(pDC, CPoint(x+84,  y), clrSelArray[++i], 39, 29, 41, 52);
	DrawCell(pDC, CPoint(x+98,  y), clrSelArray[++i], 40, 30, 42, 53);
	DrawCell(pDC, CPoint(x+112, y), clrSelArray[++i], 41, 31, 43, 54);
	DrawCell(pDC, CPoint(x+126, y), clrSelArray[++i], 42, 32, 44, 55);
	DrawCell(pDC, CPoint(x+140, y), clrSelArray[++i], 43, 33, 45, 56);

	x -= 7; y += 12;
	DrawCell(pDC, CPoint(x,     y), clrSelArray[++i], 44, 33, 46, 58);
	DrawCell(pDC, CPoint(x+14,  y), clrSelArray[++i], 45, 34, 47, 59);
	DrawCell(pDC, CPoint(x+28,  y), clrSelArray[++i], 46, 35, 48, 60);
	DrawCell(pDC, CPoint(x+42,  y), clrSelArray[++i], 47, 36, 49, 61);
	DrawCell(pDC, CPoint(x+56,  y), clrSelArray[++i], 48, 37, 50, 62);
	DrawCell(pDC, CPoint(x+70,  y), clrSelArray[++i], 49, 38, 51, 63);
	DrawCell(pDC, CPoint(x+84,  y), clrSelArray[++i], 50, 39, 52, 64);
	DrawCell(pDC, CPoint(x+98,  y), clrSelArray[++i], 51, 40, 53, 65);
	DrawCell(pDC, CPoint(x+112, y), clrSelArray[++i], 52, 41, 54, 66);
	DrawCell(pDC, CPoint(x+126, y), clrSelArray[++i], 53, 42, 55, 67);
	DrawCell(pDC, CPoint(x+140, y), clrSelArray[++i], 54, 43, 56, 68);
	DrawCell(pDC, CPoint(x+154, y), clrSelArray[++i], 55, 44, 57, 69);

	x -= 7; y += 12;
	DrawCell(pDC, CPoint(x,     y), clrSelArray[++i], 56, 44, 58, 70);
	DrawCell(pDC, CPoint(x+14,  y), clrSelArray[++i], 57, 45, 59, 71);
	DrawCell(pDC, CPoint(x+28,  y), clrSelArray[++i], 58, 46, 60, 72);
	DrawCell(pDC, CPoint(x+42,  y), clrSelArray[++i], 59, 47, 61, 73);
	DrawCell(pDC, CPoint(x+56,  y), clrSelArray[++i], 60, 48, 62, 74);
	DrawCell(pDC, CPoint(x+70,  y), clrSelArray[++i], 61, 49, 63, 75);
	DrawCell(pDC, CPoint(x+84,  y), clrSelArray[++i], 62, 50, 64, 76);
	DrawCell(pDC, CPoint(x+98,  y), clrSelArray[++i], 63, 51, 65, 77);
	DrawCell(pDC, CPoint(x+112, y), clrSelArray[++i], 64, 52, 66, 78);
	DrawCell(pDC, CPoint(x+126, y), clrSelArray[++i], 65, 53, 67, 79);
	DrawCell(pDC, CPoint(x+140, y), clrSelArray[++i], 66, 54, 68, 80);
	DrawCell(pDC, CPoint(x+154, y), clrSelArray[++i], 67, 55, 69, 81);
	DrawCell(pDC, CPoint(x+168, y), clrSelArray[++i], 68, 56, 70, 82);

	x += 7; y += 12;
	DrawCell(pDC, CPoint(x,     y), clrSelArray[++i], 69, 57, 71, 82);
	DrawCell(pDC, CPoint(x+14,  y), clrSelArray[++i], 70, 58, 72, 83);
	DrawCell(pDC, CPoint(x+28,  y), clrSelArray[++i], 71, 59, 73, 84);
	DrawCell(pDC, CPoint(x+42,  y), clrSelArray[++i], 72, 60, 74, 85);
	DrawCell(pDC, CPoint(x+56,  y), clrSelArray[++i], 73, 61, 75, 86);
	DrawCell(pDC, CPoint(x+70,  y), clrSelArray[++i], 74, 62, 76, 87);
	DrawCell(pDC, CPoint(x+84,  y), clrSelArray[++i], 75, 63, 77, 88);
	DrawCell(pDC, CPoint(x+98,  y), clrSelArray[++i], 76, 64, 78, 89);
	DrawCell(pDC, CPoint(x+112, y), clrSelArray[++i], 77, 65, 79, 90);
	DrawCell(pDC, CPoint(x+126, y), clrSelArray[++i], 78, 66, 80, 91);
	DrawCell(pDC, CPoint(x+140, y), clrSelArray[++i], 79, 67, 81, 92);
	DrawCell(pDC, CPoint(x+154, y), clrSelArray[++i], 80, 68, 82, 93);

	x += 7; y += 12;
	DrawCell(pDC, CPoint(x,     y), clrSelArray[++i], 81, 70, 83,  93);
	DrawCell(pDC, CPoint(x+14,  y), clrSelArray[++i], 82, 71, 84,  94);
	DrawCell(pDC, CPoint(x+28,  y), clrSelArray[++i], 83, 72, 85,  95);
	DrawCell(pDC, CPoint(x+42,  y), clrSelArray[++i], 84, 73, 86,  96);
	DrawCell(pDC, CPoint(x+56,  y), clrSelArray[++i], 85, 74, 87,  97);
	DrawCell(pDC, CPoint(x+70,  y), clrSelArray[++i], 86, 75, 88,  98);
	DrawCell(pDC, CPoint(x+84,  y), clrSelArray[++i], 87, 76, 89,  99);
	DrawCell(pDC, CPoint(x+98,  y), clrSelArray[++i], 88, 77, 90, 100);
	DrawCell(pDC, CPoint(x+112, y), clrSelArray[++i], 89, 78, 91, 101);
	DrawCell(pDC, CPoint(x+126, y), clrSelArray[++i], 90, 79, 92, 102);
	DrawCell(pDC, CPoint(x+140, y), clrSelArray[++i], 91, 80, 93, 103);

	x += 7; y += 12;
	DrawCell(pDC, CPoint(x,     y), clrSelArray[++i], 92, 82,  94, 103);
	DrawCell(pDC, CPoint(x+14,  y), clrSelArray[++i], 93, 83,  95, 104);
	DrawCell(pDC, CPoint(x+28,  y), clrSelArray[++i], 94, 84,  96, 105);
	DrawCell(pDC, CPoint(x+42,  y), clrSelArray[++i], 95, 85,  97, 106);
	DrawCell(pDC, CPoint(x+56,  y), clrSelArray[++i], 96, 86,  98, 107);
	DrawCell(pDC, CPoint(x+70,  y), clrSelArray[++i], 97, 87,  99, 108);
	DrawCell(pDC, CPoint(x+84,  y), clrSelArray[++i], 98, 88, 100, 109);
	DrawCell(pDC, CPoint(x+98,  y), clrSelArray[++i], 99, 89, 101, 110);
	DrawCell(pDC, CPoint(x+112, y), clrSelArray[++i], 100, 90, 102, 111);
	DrawCell(pDC, CPoint(x+126, y), clrSelArray[++i], 101, 91, 103, 112);

	x += 7; y += 12;
	DrawCell(pDC, CPoint(x,     y), clrSelArray[++i], 102,  93, 104, 112);
	DrawCell(pDC, CPoint(x+14,  y), clrSelArray[++i], 103,  94, 105, 113);
	DrawCell(pDC, CPoint(x+28,  y), clrSelArray[++i], 104,  95, 106, 114);
	DrawCell(pDC, CPoint(x+42,  y), clrSelArray[++i], 105,  96, 107, 115);
	DrawCell(pDC, CPoint(x+56,  y), clrSelArray[++i], 106,  97, 108, 116);
	DrawCell(pDC, CPoint(x+70,  y), clrSelArray[++i], 107,  98, 109, 117);
	DrawCell(pDC, CPoint(x+84,  y), clrSelArray[++i], 108,  99, 110, 118);
	DrawCell(pDC, CPoint(x+98,  y), clrSelArray[++i], 109, 100, 111, 119);
	DrawCell(pDC, CPoint(x+112, y), clrSelArray[++i], 110, 101, 112, 120);

	x += 7; y += 12;
	DrawCell(pDC, CPoint(x,     y), clrSelArray[++i], 111, 103, 113, 120);
	DrawCell(pDC, CPoint(x+14,  y), clrSelArray[++i], 112, 104, 114, 121);
	DrawCell(pDC, CPoint(x+28,  y), clrSelArray[++i], 113, 105, 115, 122);
	DrawCell(pDC, CPoint(x+42,  y), clrSelArray[++i], 114, 106, 116, 123);
	DrawCell(pDC, CPoint(x+56,  y), clrSelArray[++i], 115, 107, 117, 124);
	DrawCell(pDC, CPoint(x+70,  y), clrSelArray[++i], 116, 108, 118, 125);
	DrawCell(pDC, CPoint(x+84,  y), clrSelArray[++i], 117, 109, 119, 126);
	DrawCell(pDC, CPoint(x+98,  y), clrSelArray[++i], 118, 110, 120, 142);

	x += 7; y += 12;
	DrawCell(pDC, CPoint(x,     y), clrSelArray[++i], 119, 112, 121, 121);
	DrawCell(pDC, CPoint(x+14,  y), clrSelArray[++i], 120, 113, 122, 122);
	DrawCell(pDC, CPoint(x+28,  y), clrSelArray[++i], 121, 114, 123, 123);
	DrawCell(pDC, CPoint(x+42,  y), clrSelArray[++i], 122, 115, 124, 124);
	DrawCell(pDC, CPoint(x+56,  y), clrSelArray[++i], 123, 116, 125, 125);
	DrawCell(pDC, CPoint(x+70,  y), clrSelArray[++i], 124, 117, 126, 126);
	DrawCell(pDC, CPoint(x+84,  y), clrSelArray[++i], 125, 118, 142, 142);

	// Grey scale colors.
	x -= 7; y += 25;
	DrawCell(pDC, CPoint(x,     y), clrSelArray[++i], 142, 142, 135, 135);
	DrawCell(pDC, CPoint(x+14,  y), clrSelArray[++i], 135, 135, 136, 136);
	DrawCell(pDC, CPoint(x+28,  y), clrSelArray[++i], 136, 136, 137, 137);
	DrawCell(pDC, CPoint(x+42,  y), clrSelArray[++i], 137, 137, 138, 138);
	DrawCell(pDC, CPoint(x+56,  y), clrSelArray[++i], 138, 138, 139, 139);
	DrawCell(pDC, CPoint(x+70,  y), clrSelArray[++i], 139, 139, 140, 140);
	DrawCell(pDC, CPoint(x+84,  y), clrSelArray[++i], 140, 140, 141, 141);
	DrawCell(pDC, CPoint(x+98,  y), clrSelArray[++i], 141, 141, 143, 143);

	x += 7; y += 12;
	DrawCell(pDC, CPoint(x,     y), clrSelArray[++i], 127, 127, 128, 128);
	DrawCell(pDC, CPoint(x+14,  y), clrSelArray[++i], 128, 128, 129, 129);
	DrawCell(pDC, CPoint(x+28,  y), clrSelArray[++i], 129, 129, 130, 130);
	DrawCell(pDC, CPoint(x+42,  y), clrSelArray[++i], 130, 130, 131, 131);
	DrawCell(pDC, CPoint(x+56,  y), clrSelArray[++i], 131, 131, 132, 132);
	DrawCell(pDC, CPoint(x+70,  y), clrSelArray[++i], 132, 132, 133, 133);
	DrawCell(pDC, CPoint(x+84,  y), clrSelArray[++i], 133, 133, 134, 134);

	x = (rcClient.Width()-CX_HEXDISP)/2;
	DrawLargeCell(pDC, CPoint(x, y-12), clrSelArray[++i], 126, 126, 127, 127);

	x = ((rcClient.Width()-CX_HEXDISP)/2)+(CX_HEXDISP-29);
	DrawLargeCell(pDC, CPoint(x, y-12), clrSelArray[++i], 134, 134,  -1,  -1);
}

void CXTColorHex::UpdateSelection(CPoint point)
{
	COLORREF clr = ColorFromPoint(point);

	if ((clr != (COLORREF)-1) && (m_clrColor != clr))
	{
		m_clrColor = clr;
		GetParent()->SendMessage(XTWM_UPDATECOLOR,
			(WPARAM)(COLORREF)m_clrColor);

		CRect rc;
		GetClientRect(&rc);

		CClientDC dc(this);
		CXTMemDC memDC(&dc, rc);

		memDC.BitBlt(0, 0, rc.Size().cx, rc.Size().cy, &m_dcPicker, 0, 0, SRCCOPY);
		SelectColorCell(&memDC);
	}
}

void CXTColorHex::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (GetFocus() != this)
		SetFocus();

	UpdateSelection(point);
	m_bLBtnDown = TRUE;

	CWnd::OnLButtonDown(nFlags, point);
}


void CXTColorHex::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CStatic::OnLButtonDblClk(nFlags, point);

	if (GetFocus() != this)
		SetFocus();

	COLORREF clr = ColorFromPoint(point);

	if (clr != (COLORREF)-1)
	{
		UpdateSelection(point);
		m_bLBtnDown = TRUE;

		CPropertyPage* pParentPage = (CPropertyPage*)GetParent();
		ASSERT_VALID(pParentPage);

		CPropertySheet* pParentSheet = (CPropertySheet*)pParentPage->GetParent();
		ASSERT_VALID(pParentSheet);

		pParentSheet->EndDialog(IDOK);
	}
}

COLORREF CXTColorHex::ColorFromPoint(CPoint point)
{
	POSITION pos;
	for (pos = m_arCells.GetHeadPosition(); pos; m_arCells.GetNext(pos))
	{
		HEXCOLOR_CELL* pCI = m_arCells.GetAt(pos);
		ASSERT(pCI!= NULL);

		if (pCI == NULL)
			return (COLORREF)-1;

		int nPixelCount = pCI->bSmall ? 168 : 691;
		int i;
		for (i = 0; i < nPixelCount; ++i)
		{
			if (point == pCI->pPoint[i])
			{
				m_ptCurrent   = pCI->pPoint[0];
				m_bSmallCell  = pCI->bSmall;
				return pCI->clr;
			}
		}
	}

	return (COLORREF)-1;
}

void CXTColorHex::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bLBtnDown = FALSE;
	CWnd::OnLButtonUp(nFlags, point);
}

void CXTColorHex::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bLBtnDown) {
		UpdateSelection(point);
	}

	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CXTColorHex::OnInitControl(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (m_bInitControl)
	{
		m_bInitControl = false;
		ModifyStyle(NULL, SS_NOTIFY);
	}

	return 0;
}

void CXTColorHex::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();

	// Initialize the control.
	PostMessage(XTWM_INITIAL_UPDATE);
}

int CXTColorHex::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize the control.
	SendMessage(XTWM_INITIAL_UPDATE);

	return 0;
}

CXTColorHex::HEXCOLOR_CELL* CXTColorHex::GetColorCell(COLORREF clr)
{
	bool bWhite = false;

	POSITION pos;
	for (pos = m_arCells.GetHeadPosition(); pos; m_arCells.GetNext(pos))
	{
		HEXCOLOR_CELL* pCI = m_arCells.GetAt(pos);
		ASSERT(pCI!= NULL);

		if (clr == pCI->clr)
		{
			if (clr == RGB(0xff,0xff,0xff) && bWhite == false)
			{
				if (m_ptCurrent.y >= 176) // big hex selected
				{
					bWhite = true;
					continue;
				}
			}
			return pCI;
		}
	}

	return NULL;
}

void CXTColorHex::SetSelectedColor(int iIndex)
{
	if (iIndex != -1)
	{
		POSITION pos2 = m_arCells.FindIndex(iIndex);
		ASSERT(pos2 != NULL);

		HEXCOLOR_CELL* pClrCell = m_arCells.GetAt(pos2);
		ASSERT(pClrCell != NULL);

		UpdateSelection(pClrCell->pPoint[0]);
	}
}

void CXTColorHex::SetSelectedColor(COLORREF clr)
{
	m_clrColor = clr;
	int iIndex = -1;

	POSITION pos;
	for (pos = m_arCells.GetHeadPosition(); pos; m_arCells.GetNext(pos), ++iIndex)
	{
		HEXCOLOR_CELL* pCI = m_arCells.GetAt(pos);
		ASSERT(pCI!= NULL);

		if (clr == pCI->clr)
		{
			SetSelectedColor(iIndex);
		}
	}
}

BOOL CXTColorHex::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		{
			HEXCOLOR_CELL* pClrCell = GetColorCell(m_clrColor);
			if (pClrCell == NULL)
				pClrCell = GetColorCell(RGB(0x00,0x00,0x00));

			TCHAR vkey = (TCHAR)pMsg->wParam;
			switch (vkey)
			{
			case VK_LEFT:
				SetSelectedColor(pClrCell->direction[0]);
				break;
			case VK_UP:
				SetSelectedColor(pClrCell->direction[1]);
				break;
			case VK_RIGHT:
				SetSelectedColor(pClrCell->direction[2]);
				break;
			case VK_DOWN:
				SetSelectedColor(pClrCell->direction[3]);
				break;
			case VK_RETURN:
			case VK_SPACE:
				CPropertyPage* pParentPage = (CPropertyPage*)GetParent();
				ASSERT_VALID(pParentPage);

				CPropertySheet* pParentSheet = (CPropertySheet*)pParentPage->GetParent();
				ASSERT_VALID(pParentSheet);

				pParentSheet->EndDialog(IDOK);
				break;
			}
		}
		break;
	}

	return CStatic::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CXTColorPageStandard

IMPLEMENT_DYNCREATE(CXTColorPageStandard, CPropertyPage)

BEGIN_MESSAGE_MAP(CXTColorPageStandard, CPropertyPage)
	//{{AFX_MSG_MAP(CXTColorPageStandard)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTWM_UPDATECOLOR, OnUpdateColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTColorPageStandard construction/destruction

CXTColorPageStandard::CXTColorPageStandard(CXTColorDialog* pParentSheet)
: CXTThemePropertyPage(pParentSheet, CXTColorPageStandard::IDD)
{
	//{{AFX_DATA_INIT(CXTColorPageStandard)
	//}}AFX_DATA_INIT
}

CXTColorPageStandard::~CXTColorPageStandard()
{
}

void CXTColorPageStandard::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTColorPageStandard)
	DDX_Control(pDX, XT_IDC_CLR_HEX, m_colorHex);
	//}}AFX_DATA_MAP
}

LRESULT CXTColorPageStandard::OnUpdateColor(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(lParam);
	m_pParentSheet->SetNewColor((COLORREF)wParam, FALSE);
	return 0;
}

BOOL CXTColorPageStandard::OnInitDialog()
{
	CXTThemePropertyPage::OnInitDialog();

	// draw the following items transparent.
	MakeTextTransparent(XT_IDC_TXT_COLORS);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTColorPageStandard::OnDrawPage(CDC* pDC)
{
	CXTThemePropertyPage::OnDrawPage(pDC);
}
