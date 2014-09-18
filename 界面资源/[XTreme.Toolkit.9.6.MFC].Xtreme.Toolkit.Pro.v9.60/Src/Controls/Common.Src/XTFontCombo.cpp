// XTFontCombo.cpp : implementation of the CXTFontCombo class.
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

#include "XTResource.h"
#include "XTDefines.h"
#include "XTUtil.h"
#include "XTGlobal.h"
#include "XTWndHook.h"
#include "XTMemDC.h"
#include "XTFlatComboBox.h"
#include "XTButton.h"
#include "XTEditListBox.h"
#include "XTFontCombo.h"
#include "XTWndShadow.h"
#include "XTPColorManager.h"

#include <windowsx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////
// CXTFontCombo
///////////////////////////////////////////////////////////////////////////

const int BMW = 16;

CXTFontCombo::CXTFontCombo()
{
	m_cyHScroll = ::GetSystemMetrics( SM_CYHSCROLL );
	m_cyEdge    = ::GetSystemMetrics( SM_CYEDGE );

	m_strSymbol = _T( "AbCdEfGhIj" );
	m_eStyle    = xtFontSample;
}

CXTFontCombo::~CXTFontCombo()
{

}

IMPLEMENT_DYNAMIC(CXTFontCombo, CXTFlatComboBox)

BEGIN_MESSAGE_MAP(CXTFontCombo, CXTFlatComboBox)
	//{{AFX_MSG_MAP(CXTFontCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTFontCombo::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	if ( lpDIS->itemID == (UINT)-1 )
	{
		return;
	}

	CDC*  pDC      = CDC::FromHandle( lpDIS->hDC );
	CRect rcItem   = lpDIS->rcItem;
	int   nState   = lpDIS->itemState;
	int   nItem    = lpDIS->itemID;
	int   nIndexDC = pDC->SaveDC( );

	CXTLogFont* pLF = ( CXTLogFont* )GetItemDataPtr( nItem  );
	if ( pLF == NULL )
	{
		return;
	}

	CString strFontName = pLF->lfFaceName;

	// highlight the background if selected.
	if ( nState & ODS_SELECTED )
	{
		pDC->FillSolidRect( rcItem, GetXtremeColor(COLOR_HIGHLIGHT) );
		pDC->SetTextColor( GetXtremeColor(COLOR_HIGHLIGHTTEXT) );
		pDC->DrawFocusRect( &rcItem );
	}
	else
	{
		pDC->FillSolidRect( rcItem, GetXtremeColor(COLOR_WINDOW) );
		pDC->SetTextColor( GetXtremeColor(COLOR_WINDOWTEXT) );
	}

	// true type or device flag set by SetItemData
	if ( pLF->dwType & TRUETYPE_FONTTYPE )
	{
		m_ilFontType.Draw( pDC, 1, rcItem.TopLeft(), ILD_NORMAL );
	}

	// Define the size of the text area to draw
	CRect rcText( rcItem );
	rcText.left += BMW + 6;

	pDC->SetBkMode( TRANSPARENT );

	if ( ( m_eStyle == xtFontBoth ) || ( m_eStyle == xtFontGUI ) )
	{
		// Draw the text item.
		pDC->SelectObject( &XTAuxData().font );
		pDC->DrawText( strFontName, rcText, DT_VCENTER | DT_SINGLELINE );

		rcText.left = rcText.left + CXTFontEnum::Get().GetMaxWidth() + 5;
	}

	if ( ( m_eStyle == xtFontBoth ) || ( m_eStyle == xtFontSample ) )
	{
		if ( ( m_eStyle == xtFontSample ) && ( pLF->lfCharSet == SYMBOL_CHARSET ) )
		{
			pDC->SelectObject( &XTAuxData().font );
			pDC->DrawText( strFontName, rcText, DT_VCENTER | DT_SINGLELINE );

			int iOffset = pDC->GetTextExtent( strFontName ).cx + 5;
			rcText.left += iOffset;

			strFontName = m_strSymbol;
		}

		// Create the font to be displayed and initialize the device context.
		CFont font;
		font.CreateFontIndirect( pLF );

		// Draw the text item.
		pDC->SelectObject( &font );
		pDC->DrawText( strFontName, rcText, DT_VCENTER | DT_SINGLELINE );
	}

	// Restore the original device context.
	pDC->RestoreDC( nIndexDC );
}

void CXTFontCombo::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	ASSERT( lpMIS->CtlType == ODT_COMBOBOX );

	DWORD dwStyle = ::GetWindowLong( m_hWnd, GWL_STYLE );

	// combo box is owner drawn, and must be created with the
	// following styles:

	ASSERT( dwStyle & CBS_OWNERDRAWFIXED );
	ASSERT( dwStyle & CBS_DROPDOWN );
	ASSERT( dwStyle & CBS_SORT );
	ASSERT( dwStyle & CBS_HASSTRINGS );

	lpMIS->itemHeight = ( m_cyHScroll - m_cyEdge );
	lpMIS->itemWidth  = 0;
}

int CXTFontCombo::CompareItem(LPCOMPAREITEMSTRUCT lpCIS)
{
	ASSERT( lpCIS->CtlType == ODT_LISTBOX );

	int iItem1 = ( int )( WORD )lpCIS->itemID1;
	if ( iItem1 == -1 )
	{
		return -1;
	}

	CString strItem1;
	GetLBText( iItem1, strItem1 );

	int iItem2 = ( int )( WORD )lpCIS->itemID2;
	if ( iItem2 == -1 )
	{
		return -1;
	}

	CString strItem2;
	GetLBText( iItem2, strItem2 );

	return strItem1.Collate( strItem2 );
}

void CXTFontCombo::InitControl(LPCTSTR lpszFaceName, UINT nWidth/*=0*/, BOOL bEnable/*=TRUE*/ )
{
	ResetContent();

	if ( nWidth != 0 )
	{
		SetDroppedWidth( nWidth );
	}

	// moved from constructor so resource is loaded when this gets called
	VERIFY( m_ilFontType.Create(
		XT_IDB_FONTTYPE, BMW, 1, RGB( 255,0,255 ) ) );

	EnableAutoCompletion( bEnable );

	CXTFontList& fontList = CXTFontEnum::Get().GetFontList();

	POSITION pos;
	for ( pos = fontList.GetHeadPosition( ); pos; fontList.GetNext( pos ) )
	{
		CXTLogFont& lf = fontList.GetAt( pos );
		int iIndex = AddString( lf.lfFaceName );

		SetItemDataPtr( iIndex, &lf );
	}

	if ( lpszFaceName && _tcslen( lpszFaceName ) )
	{
		SetSelFont( lpszFaceName );
	}
}

bool CXTFontCombo::GetSelFont(CXTLogFont& lf)
{
	int iCurSel = GetCurSel( );
	if ( iCurSel == CB_ERR )
	{
		CString strWindowText;
		GetWindowText( strWindowText );

		iCurSel = SelectString( -1, strWindowText );

		if ( iCurSel == CB_ERR )
		{
			return false;
		}
	}

	CXTLogFont* pLF = ( CXTLogFont* )GetItemDataPtr( iCurSel );
	if ( pLF == NULL )
	{
		return false;
	}

	MEMCPY_S((void*)&lf, (const void*)pLF, sizeof(CXTLogFont));

	return true;
}

bool CXTFontCombo::GetSelFont(CString& strFaceName)
{
	CXTLogFont lf;
	if ( GetSelFont( lf ) )
	{
		strFaceName = lf.lfFaceName;
		return true;
	}
	return false;
}

bool CXTFontCombo::SetSelFont(CXTLogFont& lf)
{
	return SetSelFont( lf.lfFaceName );
}

bool CXTFontCombo::SetSelFont(CString strFaceName)
{
	int iCount = GetCount( );
	int i;
	for ( i = 0; i < iCount; ++i )
	{
		CXTLogFont* pLF = ( CXTLogFont* )GetItemDataPtr( i );
		if ( pLF != NULL )
		{
			if ( strFaceName.CompareNoCase( pLF->lfFaceName ) == 0 )
			{
				SetCurSel( i );
				return true;
			}
		}
	}

	return false;
}
