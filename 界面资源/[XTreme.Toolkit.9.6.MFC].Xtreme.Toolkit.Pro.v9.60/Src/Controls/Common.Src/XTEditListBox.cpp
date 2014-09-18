// XTEditListBox.cpp : implementation of the CXTEditListBox class.
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
#include "XTPResourceManager.h"
#include "XTPColorManager.h"
#include "XTPDrawHelpers.h"
#include "XTPWinThemeWrapper.h"

#include "XTResource.h"
#include "XTDefines.h"
#include "XTUtil.h"
#include "XTGlobal.h"
#include "XTWndHook.h"
#include "XTFlatComboBox.h"
#include "XTButton.h"
#include "XTEditListBox.h"
#include "XTBrowseEdit.h"
#include "XTMemDC.h"
#include "XTThemeManager.h"
#include "XTButtonTheme.h"
#include "XTVC50Helpers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTListBox
/////////////////////////////////////////////////////////////////////////////
CXTListBox::CXTListBox()
{

}

CXTListBox::~CXTListBox()
{

}

IMPLEMENT_DYNAMIC(CXTListBox, CListBox)

BEGIN_MESSAGE_MAP(CXTListBox, CListBox)
	//{{AFX_MSG_MAP(CXTListBox)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTListBox::Initialize(bool bAutoFont/*=true*/)
{
	// make sure the style is set to LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS.
	DWORD dwStyle = ::GetWindowLong(GetSafeHwnd(), GWL_STYLE);

	if ((dwStyle & LBS_OWNERDRAWVARIABLE) == 0)
		dwStyle |= LBS_OWNERDRAWVARIABLE;

	if ((dwStyle & LBS_HASSTRINGS) == 0)
		dwStyle |= LBS_HASSTRINGS;

	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);

	// set the font for the list box.
	if (bAutoFont)
	{
		SetFont(&XTAuxData().font);
	}
}

void CXTListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC     = CDC::FromHandle(lpDIS->hDC);
	CRect rcItem = lpDIS->rcItem;

	if ((lpDIS->itemID != (UINT)-1) && (lpDIS->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)))
	{
		int iSavedDC = pDC->SaveDC();

		COLORREF clrWindow = IsWindowEnabled() ? GetXtremeColor(COLOR_WINDOW) : GetXtremeColor(COLOR_3DFACE);
		COLORREF clrWindowText = IsWindowEnabled() ? GetXtremeColor(COLOR_WINDOWTEXT) : GetXtremeColor(COLOR_GRAYTEXT);

		if ((lpDIS->itemState & ODS_SELECTED) != 0)
		{
			clrWindow = IsWindowEnabled() ? GetXtremeColor(COLOR_HIGHLIGHT)  : GetXtremeColor(COLOR_GRAYTEXT);
			clrWindowText = GetXtremeColor(COLOR_HIGHLIGHTTEXT);
		}

		// set the text and text background colors, then repaint the item.
		pDC->SetBkColor(clrWindow);
		pDC->SetTextColor(clrWindowText);
		pDC->FillSolidRect(&rcItem, clrWindow);

		CString strText;
		GetText(lpDIS->itemID, strText);

		rcItem.left += 2;
		pDC->DrawText(strText, &rcItem,
			DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_EXPANDTABS);

		pDC->RestoreDC(iSavedDC);
	}
	if (lpDIS->itemAction & ODA_FOCUS)
		pDC->DrawFocusRect(&lpDIS->rcItem);
}

void CXTListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	lpMIS->itemHeight = ::GetSystemMetrics(SM_CYMENU)-2;
}

BOOL CXTListBox::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTListBox::OnPaint()
{
	CPaintDC dc(this);

	// Get the client rect.
	CRect r;
	GetClientRect(&r);

	// determine the background color.
	COLORREF clrWindow = IsWindowEnabled() ?
		GetXtremeColor(COLOR_WINDOW) : GetXtremeColor(COLOR_3DFACE);

	// Paint to a memory device context to reduce screen flicker.
	CXTMemDC memDC(&dc, r, clrWindow);

	// Let the window do its default painting...
	CListBox::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
}

void CXTListBox::SetRedrawEx(bool bEnable)
{
	// Turn off redraw while scrolling, this will cause paint problems
	// with our control because we are painting off screen to reduce
	// flicker.  The default implementation assumes that WM_ERASEBKGND
	// has repainted the background, which in our case does not happen.

	SetRedraw(bEnable);

	if (bEnable)
	{
		// repaint the list box
		Invalidate();
		UpdateWindow();

		// repaint the scroll bar.
		::SetWindowPos(m_hWnd, NULL, 0,0,0,0,
			SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
	}
}

BOOL CXTListBox::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	SetRedrawEx(false);
	BOOL bReturn = CListBox::OnMouseWheel(nFlags, zDelta, pt);
	SetRedrawEx(true);

	return bReturn;
}

void CXTListBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SetRedrawEx(false);
	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
	SetRedrawEx(true);
}

void CXTListBox::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SetRedrawEx(false);
	CListBox::OnHScroll(nSBCode, nPos, pScrollBar);
	SetRedrawEx(true);
}

//////////////////////////////////////////////////////////////////////
// CXTFontEnum
//////////////////////////////////////////////////////////////////////

const int BMW = 16;
const int BMH = 20;

CXTFontEnum::CXTFontEnum()
{
	Init();
}

CXTFontEnum::~CXTFontEnum()
{

}

CXTFontEnum& CXTFontEnum::Get()
{
	static CXTFontEnum fontEnum;
	return fontEnum;
}

bool CXTFontEnum::DoesFontExist(CString& strFaceName)
{
	POSITION pos;
	for ( pos = m_listFonts.GetHeadPosition( ); pos; m_listFonts.GetNext( pos ) )
	{
		CXTLogFont& lf = m_listFonts.GetAt( pos );
		if ( strFaceName.CompareNoCase( lf.lfFaceName ) == 0 )
		{
			strFaceName = lf.lfFaceName; // adjust case if necessary
			return true;
		}
	}
	return false;
}

CXTLogFont* CXTFontEnum::GetLogFont(CString strFaceName)
{
	POSITION pos;
	for ( pos = m_listFonts.GetHeadPosition( ); pos; m_listFonts.GetNext( pos ) )
	{
		CXTLogFont& lf = m_listFonts.GetAt( pos );
		if ( strFaceName == lf.lfFaceName )
		{
			return &lf;
		}
	}
	return NULL;
}

bool CXTFontEnum::AddFont(const LOGFONT* pLF, DWORD dwType)
{
	// See if the font name has already been added.
	CXTStringHelper strFaceName = pLF->lfFaceName;
	strFaceName.Remove( '@' );

	if ( DoesFontExist( strFaceName ) != 0 )
	{
		return false;
	}

	LOGFONT lfGlobal;
	XTAuxData().font.GetLogFont( &lfGlobal );

	// Save the CXTLogFont struct.
	CXTLogFont lf;

	MEMCPY_S((void*)&lf, (const void*)pLF, sizeof(CXTLogFont));

	lf.dwType   = dwType;
	lf.lfHeight = -( ::GetSystemMetrics( SM_CYVTHUMB )+1 );
	lf.lfWidth  = 0;

	m_listFonts.AddTail( lf );

	return true;
}

BOOL CALLBACK CXTFontEnum::EnumFontFamExProc(ENUMLOGFONTEX* pelf,
	NEWTEXTMETRICEX* /*lpntm*/, DWORD dwFontType, LPARAM lParam)
{
	CXTFontEnum* pFontEnum = (CXTFontEnum*)lParam;
	if ( pFontEnum != NULL )
	{
		if ( pFontEnum->AddFont( &pelf->elfLogFont, dwFontType ) )
		{
			// TODO:
		}
	}

	return TRUE;
}

int CXTFontEnum::GetMaxWidth()
{
	CWindowDC dc( NULL );
	CFont* p = dc.SelectObject( &XTAuxData().font );

	int iMaxWidth = 0;

	POSITION pos;
	for ( pos = m_listFonts.GetHeadPosition( ); pos; m_listFonts.GetNext( pos ) )
	{
		CXTLogFont& lf = m_listFonts.GetAt( pos );

		int nWidth = dc.GetTextExtent( lf.lfFaceName ).cx;
		iMaxWidth = __max(iMaxWidth, nWidth);
	}

	dc.SelectObject( p );
	return iMaxWidth;
}

void CXTFontEnum::Init(CDC* pDC/*=NULL*/, BYTE nCharSet/*=DEFAULT_CHARSET*/)
{
	m_listFonts.RemoveAll();

	// Enumerate all styles of all fonts for the ANSI character set
	CXTLogFont lf;
	lf.lfFaceName[ 0 ] = '\0';
	lf.lfCharSet = nCharSet;

	if ( pDC == NULL || pDC->m_hDC == NULL )
	{
		CWindowDC dc( NULL );
		::EnumFontFamiliesEx( dc.m_hDC, &lf,
			( FONTENUMPROC )EnumFontFamExProc, ( LPARAM )this, 0 );
	}
	else
	{
		::EnumFontFamiliesEx( pDC->m_hDC, &lf,
			( FONTENUMPROC )EnumFontFamExProc, ( LPARAM )this, 0 );
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTFontListBox
/////////////////////////////////////////////////////////////////////////////

CXTFontListBox::CXTFontListBox()
{
	m_strSymbol = _T( "AbCdEfGhIj" );
	m_eStyle    = xtFontSample;

	VERIFY( m_ilFontType.Create(
		XT_IDB_FONTTYPE, BMW, 1, RGB( 255,0,255 ) ) );
}

CXTFontListBox::~CXTFontListBox()
{
}

BEGIN_MESSAGE_MAP(CXTFontListBox, CXTListBox)
	//{{AFX_MSG_MAP(CXTFontListBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTFontListBox message handlers

void CXTFontListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	if ( lpDIS->itemID == (UINT)-1 )
	{
		return;
	}

	CDC*  pDC      = CDC::FromHandle( lpDIS->hDC );
	CRect rcItem   = lpDIS->rcItem;
	int   nState   = lpDIS->itemState;
	int   nAction  = lpDIS->itemAction;
	int   nItem    = lpDIS->itemID;
	int   nIndexDC = pDC->SaveDC( );

	CString strFontName;
	GetText( nItem, strFontName );

	CXTLogFont* pLF = ( CXTLogFont* )GetItemDataPtr( nItem  );
	if ( pLF == NULL )
	{
		return;
	}

	// highlight the background if selected.
	if ( nState & ODS_SELECTED )
	{
		if ( ( nAction & ODA_FOCUS ) && ( nState & ODS_FOCUS ) )
		{
			pDC->FillSolidRect( rcItem, GetXtremeColor(COLOR_HIGHLIGHT) );
			pDC->SetTextColor( GetXtremeColor(COLOR_HIGHLIGHTTEXT) );
			pDC->DrawFocusRect( &rcItem );
		}
		else
		{
			pDC->FillSolidRect( rcItem, GetXtremeColor(COLOR_3DFACE) );
			pDC->SetTextColor( GetXtremeColor(COLOR_WINDOWTEXT) );
		}
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

void CXTFontListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	ASSERT( lpMIS->CtlType == ODT_LISTBOX );

	lpMIS->itemHeight = BMH;
	lpMIS->itemWidth  = 0;
}

int CXTFontListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCIS)
{
	ASSERT( lpCIS->CtlType == ODT_LISTBOX );

	int iItem1 = ( int )( WORD )lpCIS->itemID1;
	if ( iItem1 == -1 )
	{
		return -1;
	}

	CString strItem1;
	GetText( iItem1, strItem1 );

	int iItem2 = ( int )( WORD )lpCIS->itemID2;
	if ( iItem2 == -1 )
	{
		return -1;
	}

	CString strItem2;
	GetText( iItem2, strItem2 );

	return strItem1.Collate( strItem2 );
}

void CXTFontListBox::Initialize(bool /*bAutoFont=true*/)
{
	CXTListBox::Initialize(false);

	CXTFontList& fontList = CXTFontEnum::Get().GetFontList();

	POSITION pos;
	for ( pos = fontList.GetHeadPosition( ); pos; fontList.GetNext( pos ) )
	{
		CXTLogFont& lf = fontList.GetAt( pos );
		int iIndex = AddString( lf.lfFaceName );

		SetItemDataPtr( iIndex, &lf );
	}
}

bool CXTFontListBox::GetSelFont(CXTLogFont& lf)
{
	int iCurSel = GetCurSel( );
	if ( iCurSel == LB_ERR )
	{
		return false;
	}

	CXTLogFont* pLF = ( CXTLogFont* )GetItemDataPtr( iCurSel );
	if ( pLF == NULL )
	{
		return false;
	}

	MEMCPY_S((void*)&lf, (const void*)pLF, sizeof(CXTLogFont));

	return true;
}

bool CXTFontListBox::GetSelFont(CString& strFaceName)
{
	CXTLogFont lf;
	if ( GetSelFont( lf ) )
	{
		strFaceName = lf.lfFaceName;
		return true;
	}
	return false;
}

bool CXTFontListBox::SetSelFont(CXTLogFont& lf)
{
	return SetSelFont( lf.lfFaceName );
}

bool CXTFontListBox::SetSelFont(CString strFaceName)
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
/////////////////////////////////////////////////////////////////////////////
// CXTEditListBox
/////////////////////////////////////////////////////////////////////////////

const int BTN_OFFSET = 22;

CXTEditListBox::CXTEditListBox()
: m_bEnableEdit(true)
, m_pParentWnd(NULL)
{
	m_pItemEdit    = NULL;
	m_nIndex       = -1;
	m_bNewItem     = FALSE;

	m_strItemDefaultText.Empty();

	SetListEditStyle(XT_IDS_DIRECTORIES, LBS_XT_CHOOSEDIR);
}

CXTEditListBox::~CXTEditListBox()
{
}

IMPLEMENT_DYNAMIC(CXTEditListBox, CXTListBox)

BEGIN_MESSAGE_MAP(CXTEditListBox, CXTListBox)
	//{{AFX_MSG_MAP(CXTEditListBox)
	ON_LBN_XT_LABELEDITEND(XT_IDC_LBOX_EDIT, OnEndLabelEdit)
	ON_LBN_XT_NEWITEM(XT_IDC_GROUP_EDIT, OnNewItem)
	ON_LBN_XT_DELETEITEM(XT_IDC_GROUP_EDIT, OnDeleteItem)
	ON_LBN_XT_MOVEITEMUP(XT_IDC_GROUP_EDIT, OnMoveItemUp)
	ON_LBN_XT_MOVEITEMDOWN(XT_IDC_GROUP_EDIT, OnMoveItemDown)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCMBUTTONDOWN()
	ON_WM_NCRBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGED()
	ON_LBN_XT_LABELEDITCANCEL(XT_IDC_LBOX_EDIT, OnEndLabelEdit)
	ON_WM_ENABLE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTEditListBox::OnEndLabelEdit()
{
	// Get a pointer to the owner window.
	CWnd* pOwner = GetOwner();
	ASSERT(pOwner);

	if (m_bNewItem)
	{
		// Delete the temp string.
		DeleteString( m_nIndex );

		// If the text string is empty, then send a cancel message.
		if (m_strItemText.IsEmpty())
		{
			pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
				LBN_XT_LABELEDITCANCEL), (LPARAM)m_hWnd );

			m_nIndex = LB_ERR;
		}

		// The user has entered text, send the new item message
		// and insert the new string.
		else
		{
			m_nIndex = AddString( m_strItemText );
			// NB: SetTopIndex below causes some confusion if you have editing session, change content
			// and then double click on another item - the result looks like misplaced
			// edit control
			//SetTopIndex( m_nIndex );


			pOwner->SendMessage( WM_COMMAND, MAKEWPARAM( GetDlgCtrlID(),
				LBN_XT_NEWITEM ), (LPARAM)m_hWnd );
		}
	}
	else
	{
		// set the index to the current selection.
		m_nIndex = GetCurSel();

		// Get the text for the selected item.
		CString strItemText;
		GetText(m_nIndex, strItemText);

		// If the new string is the same as the old string,
		// then send a cancel message.
		if (strItemText == m_strItemText)
		{
			pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
				LBN_XT_LABELEDITCANCEL), (LPARAM)m_hWnd );
		}

		// The user has updated the text string, delete the
		// existing string and replace it with the new text.
		else
		{
			DWORD_PTR dwData = GetItemData( m_nIndex );
			DeleteString( m_nIndex );
			InsertString( m_nIndex, m_strItemText );
			SetItemData( m_nIndex, dwData);

			// NB: SetTopIndex below causes some confusion if you have editing session, change content
			// and then double click on another item - the result looks like misplaced
			// edit control
			//SetTopIndex( m_nIndex );

			pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
				LBN_XT_LABELEDITEND), (LPARAM)m_hWnd );
		}
	}

	// Set the focus back to the listbox.
	SetCurSel( m_nIndex );
	SetFocus();

	m_pItemEdit = NULL;
}

// This member function will enable editing for the a list box item.
void CXTEditListBox::EditItem(int iItem)
{
	SetFocus();
	SetCurSel(iItem);
	m_nIndex = iItem;
	EditListItem(FALSE);
}

void CXTEditListBox::GetEditItemText()
{
	GetText( m_nIndex, m_strItemText );
}

void CXTEditListBox::EditListItem(BOOL bNewItem)
{
	if (!m_bEnableEdit)
		return;

	// Turn off redraw until we are ready to
	// create the edit field.
	SetRedraw( FALSE );

	// flush the item text string.
	m_strItemText = m_strItemDefaultText;

	// if this is a new item, add a temporary string
	// to the list box and set the selection to it. This
	// is where the in place edit box will appear.
	if ( bNewItem || GetCurSel() == LB_ERR )
	{
		m_nIndex = AddString(m_strItemText);
		SetCurSel( m_nIndex );
		m_bNewItem = TRUE;
	}

	// get the text for the currently selected
	// item and set the new flag to FALSE.
	else
	{
		m_bNewItem = FALSE;

		int iCount = GetCount( );
		m_nIndex = GetCurSel( );

		if ( m_nIndex >= iCount || iCount <= 0 )
		{
			m_nIndex = LB_ERR;
			SetCurSel( LB_ERR );

			// turn redraw back.
			SetRedraw( TRUE );
			Invalidate();

			return;
		}

		GetEditItemText();
	}

	// turn redraw back.
	SetRedraw( TRUE );
	Invalidate();

	// Get the size of the currently selected item.
	CRect rcItem;
	GetItemRect(m_nIndex, rcItem);
	rcItem.InflateRect( -2, 2 );

	// create the edit box.
	m_pItemEdit = new CXTItemEdit( this, rcItem,
		m_strItemText, m_dwLStyle, true );

	// if defined, set the filter for the item edit control.
	if ( !m_strFilter.IsEmpty() ) {
		m_pItemEdit->SetDlgFilter(m_strFilter);
	}
}

void CXTEditListBox::OnNewItem()
{
	CWnd* pOwner = GetOwner();
	ASSERT(pOwner);

	pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
		LBN_XT_PRENEWITEM), (LPARAM)m_hWnd );

	EditListItem(TRUE);
}

void CXTEditListBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDblClk(nFlags, point);
	EditListItem(FALSE);
}

void CXTEditListBox::OnDeleteItem()
{
	CWnd* pOwner = GetOwner();
	ASSERT(pOwner);

	// Send notification to owner that we are about to delete the item
	pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
		LBN_XT_PREDELETEITEM), (LPARAM)m_hWnd );

	int nCurIndex = GetCurSel();    // Current index.

	// If there is a valid selection made...
	if (nCurIndex != LB_ERR)
	{
		// Remove the new current item.
		int nCount = DeleteString(nCurIndex);

		if ((nCount != LB_ERR) && (nCurIndex < nCount)) {
			SetCurSel(nCurIndex);
		}
		else {
			SetCurSel(nCurIndex-1);
		}
	}

	// Set the focus back to the listbox.
	SetFocus();

	// Send notification to owner.
	pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
		LBN_XT_DELETEITEM), (LPARAM)m_hWnd );
}

void CXTEditListBox::OnMoveItemUp()
{
	m_nIndex = GetCurSel(); // Current index.
	int nNewIndex = m_nIndex-1; // New index.

	// If there is a valid selection made, and the
	// selection is not the first item and...
	if ((m_nIndex != LB_ERR) && (m_nIndex != 0))
	{
		// Save the string for the item we are
		// going to swap position with.
		CString strItem;
		GetText(nNewIndex, strItem);

		// Save any item data.
		DWORD_PTR dwNewData = GetItemData(nNewIndex);

		// Remove both the new and current
		// index items...
		DeleteString(nNewIndex);

		// then swap and re-insert them into
		// the list box.
		InsertString(m_nIndex, strItem);

		// Restore any item data.
		SetItemData(m_nIndex, dwNewData);

		// Select the item at the new index.
		SetCurSel(nNewIndex);
	}

	// Set the focus back to the listbox.
	SetFocus();

	CWnd* pOwner = GetOwner();
	ASSERT(pOwner);

	// Send notification to owner.
	pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
		LBN_XT_MOVEITEMUP), (LPARAM)m_hWnd );
}

void CXTEditListBox::OnMoveItemDown()
{
	m_nIndex = GetCurSel(); // Current index.
	int nNewIndex = m_nIndex+1; // New index.

	// If there is a valid selection made, and the
	// selection is not the last item and...
	if ((m_nIndex != LB_ERR) && (nNewIndex < GetCount()))
	{
		// Save the string for the current
		// selection.
		CString strCurItem;
		GetText(m_nIndex, strCurItem);

		// Save any item data.
		DWORD_PTR dwCurData = GetItemData(m_nIndex);

		// Remove both the new and current
		// index items...
		DeleteString(m_nIndex);

		// then swap and re-insert them into
		// the list box.
		InsertString(nNewIndex, strCurItem);

		// Restore any item data.
		SetItemData(nNewIndex, dwCurData);

		// Select the item at the new index.
		SetCurSel(nNewIndex);
	}

	// Set the focus back to the listbox.
	SetFocus();

	CWnd* pOwner = GetOwner();
	ASSERT(pOwner);

	// Send notification to owner.
	pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
		LBN_XT_MOVEITEMDOWN), (LPARAM)m_hWnd );
}

void CXTEditListBox::SetListEditStyle(UINT nTitle, DWORD dwLStyle/*=LBS_XT_DEFAULT*/)
{
	CString string;
	string.LoadString(nTitle);
	SetListEditStyle(string, dwLStyle);
}

void CXTEditListBox::SetListEditStyle(LPCTSTR lpszTitle, DWORD dwLStyle/*=LBS_XT_DEFAULT*/)
{
	m_strTitle = lpszTitle;
	m_dwLStyle = dwLStyle;

	if (m_editGroup.GetSafeHwnd()) {
		m_editGroup.SetWindowText(m_strTitle);
	}
}

void CXTEditListBox::Initialize(bool bAutoFont/*=true*/)
{
	CXTListBox::Initialize(bAutoFont);

	// get a pointer to the parent window.
	m_pParentWnd = GetParent();
	ASSERT_VALID(m_pParentWnd); // must be valid.

	// Create the edit toolbar group.
	if (!HasToolbar())
		return;

	// Create the toolbar.
	if (!CreateEditGroup(bAutoFont))
		return;

	// Resize the edit list which will also cause WM_WINDOWPOSCHANGED
	// to be called to position the toolbar.
	CXTPWindowRect rcWindow(this);
	rcWindow.top += BTN_OFFSET + ::GetSystemMetrics(SM_CXEDGE);
	m_pParentWnd->ScreenToClient(&rcWindow);

	::SetWindowPos( GetSafeHwnd(), NULL, rcWindow.left, rcWindow.top,
		rcWindow.Width(), rcWindow.Height(), SWP_FRAMECHANGED );
}

BOOL CXTEditListBox::CreateEditGroup(bool bAutoFont)
{
	// Create the edit button group that appears at the top of the
	// list edit control.
	DWORD dwStyle = (GetStyle() & WS_VISIBLE) | WS_CHILD | WS_TABSTOP |
		SS_CENTERIMAGE | SS_LEFT | SS_NOTIFY;

	if( !m_editGroup.CreateEx( WS_EX_STATICEDGE, _T("Static"), m_strTitle,
		dwStyle, CRect(0,0,0,0), GetParent(), XT_IDC_GROUP_EDIT ))
	{
		TRACE0( "Unable to create caption.\n" );
		return FALSE;
	}

	m_editGroup.SetOwner(this);
	m_editGroup.Initialize(bAutoFont);

	// enable the window depending on the list box enabled state.
	m_editGroup.EnableWindow( IsWindowEnabled() );

	return TRUE;
}

BOOL CXTEditListBox::PreTranslateMessage(MSG* pMsg)
{
	// Send message to edit window if active.
	if ( m_pItemEdit && ::IsWindow( m_pItemEdit->m_hWnd ) )
	{
		if ( pMsg->wParam == VK_TAB )
		{
			SetFocus( );
			return CWnd::PreTranslateMessage(pMsg);
		}

		return m_pItemEdit->PreTranslateMessage(pMsg);
	}

	if (pMsg->message == WM_KEYUP)
	{
		if (pMsg->wParam == VK_DELETE && GetCurSel() != LB_ERR) {
			OnDeleteItem();
		}
		if(pMsg->wParam == VK_INSERT) {
			OnNewItem();
		}
	}

	if (pMsg->message == WM_SYSKEYUP)
	{
		if ((pMsg->wParam == VK_UP) &&
			(::GetKeyState(VK_MENU) < 0) && GetCurSel() != LB_ERR) {
			OnMoveItemUp();
		}
		if ((pMsg->wParam == VK_DOWN) &&
			(::GetKeyState(VK_MENU) < 0) && GetCurSel() != LB_ERR) {
			OnMoveItemDown();
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CXTEditListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	CXTListBox::OnLButtonDown(nFlags, point);

	// TODO: Add your message handler code here and/or call default
	m_nIndex = GetCurSel();

	if (m_nIndex >= GetCount())
	{
		SetCurSel(GetCount() - 1);
		GetOwner()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), LBN_SELCHANGE), (LPARAM)m_hWnd );
	}

	CRect rcItem;
	if (GetItemRect(m_nIndex, rcItem) != LB_ERR)
	{
		if (!rcItem.PtInRect(point))
		{
			CWnd* pOwner = GetOwner();
			ASSERT(pOwner);

			SetCurSel(-1);
			pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), LBN_SELCHANGE), (LPARAM)m_hWnd );
		}
	}
}

void CXTEditListBox::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_pItemEdit)
	{
		m_pItemEdit->DestroyWindow();
		m_pItemEdit = NULL;
	}

	CListBox::OnNcLButtonDown(nHitTest, point);
}

void CXTEditListBox::OnNcMButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_pItemEdit)
	{
		m_pItemEdit->DestroyWindow();
		m_pItemEdit = NULL;
	}

	CListBox::OnNcMButtonDown(nHitTest, point);
}

void CXTEditListBox::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_pItemEdit)
	{
		m_pItemEdit->DestroyWindow();
		m_pItemEdit = NULL;
	}

	CListBox::OnNcRButtonDown(nHitTest, point);
}

void CXTEditListBox::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CXTListBox::OnWindowPosChanged(lpwndpos);

	// adjust the toolbar size and location.
	RecalcLayout();
}

void CXTEditListBox::RecalcLayout()
{
	if (HasToolbar() && ::IsWindow(m_editGroup.m_hWnd))
	{
		ASSERT_VALID(m_pParentWnd); // must be valid.

		CXTPWindowRect rcWindow(this);
		m_pParentWnd->ScreenToClient(&rcWindow);

		// move the group edit control.
		rcWindow.top   -= BTN_OFFSET;
		rcWindow.bottom = rcWindow.top + BTN_OFFSET;

		::SetWindowPos( m_editGroup.m_hWnd, NULL, rcWindow.left, rcWindow.top,
			rcWindow.Width(), rcWindow.Height(), SWP_FRAMECHANGED );
	}
}

void CXTEditListBox::OnEnable(BOOL bEnable)
{
	CXTListBox::OnEnable(bEnable);

	if (HasToolbar() && ::IsWindow(m_editGroup.m_hWnd))
		m_editGroup.EnableWindow(bEnable);
}

void CXTEditListBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CXTListBox::OnShowWindow(bShow, nStatus);

	if (HasToolbar() && ::IsWindow(m_editGroup.m_hWnd))
		m_editGroup.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
}

/////////////////////////////////////////////////////////////////////////////
// CXTEditListBoxToolBar
/////////////////////////////////////////////////////////////////////////////

CXTEditListBoxToolBar::CXTEditListBoxToolBar() : m_bEnableEdit(true)
{

}

CXTEditListBoxToolBar::~CXTEditListBoxToolBar()
{
}

IMPLEMENT_DYNAMIC(CXTEditListBoxToolBar, CStatic)

BEGIN_MESSAGE_MAP(CXTEditListBoxToolBar, CStatic)
	//{{AFX_MSG_MAP(CXTEditListBoxToolBar)
	ON_BN_CLICKED(XT_IDC_BTN_NEW, OnButtonNew)
	ON_BN_CLICKED(XT_IDC_BTN_DELETE, OnButtonDelete)
	ON_BN_CLICKED(XT_IDC_BTN_UP, OnButtonUp)
	ON_BN_CLICKED(XT_IDC_BTN_DOWN, OnButtonDown)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_ENABLE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTEditListBoxToolBar::PreTranslateMessage(MSG* pMsg)
{
	// Let the ToolTip process this message.
	m_tooltip.RelayEvent(pMsg);
	return CStatic::PreTranslateMessage(pMsg);
}

static UINT _arButtonID[] =
{
	XT_IDC_BTN_NEW,
	XT_IDC_BTN_DELETE,
	XT_IDC_BTN_UP,
	XT_IDC_BTN_DOWN,
};

void CXTEditListBoxToolBar::MoveButtons()
{
	if (!::IsWindow(m_hWnd))
		return;

	// Set the initial size for the first button.
	CXTPClientRect rcClient(this);
	rcClient.left = rcClient.right-BTN_OFFSET;

	for (int i = _countof(m_arButton)-1; i >= 0 ; --i)
	{
		if (::IsWindow(m_arButton[i].GetSafeHwnd()))
		{
			m_arClipRect[i] = rcClient;

			::SetWindowPos(m_arButton[i].GetSafeHwnd(), NULL, rcClient.left, rcClient.top,
				rcClient.Width(), rcClient.Height(), SWP_FRAMECHANGED);
			rcClient.OffsetRect(-BTN_OFFSET,0);
		}
	}

	Invalidate();
}

void CXTEditListBoxToolBar::SetXButtonStyle(DWORD dwxStyle, BOOL bRedraw/*=TRUE*/)
{
	for (int i = 0; i < _countof(m_arButton); ++i)
	{
		if (::IsWindow(m_arButton[i].m_hWnd))
		{
			m_arButton[i].SetXButtonStyle(dwxStyle, bRedraw);
		}
	}
}

void CXTEditListBoxToolBar::Initialize(bool bAutoFont/*=true*/)
{
	// Create the ToolTip control.
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);

	CImageList imageList;
	imageList.Create(14, 12, ILC_COLOR8|ILC_MASK, 0, 1);

	CBitmap bmp;
	VERIFY(XTPResourceManager()->LoadBitmap(&bmp, XT_IDB_LISTEDIT));
	imageList.Add(&bmp, RGB(255,0,255));

	for (int i = 0; i < _countof(m_arButton); ++i)
	{
		if (!m_arButton[i].Create(NULL, WS_CHILD|WS_VISIBLE|BS_ICON|BS_OWNERDRAW|BS_CENTER|BS_VCENTER,
			CRect(0,0,0,0), this, _arButtonID[i]))
		{
			TRACE0("Unable to create edit button.\n");
			continue;
		}

		m_arIcon[i] = imageList.ExtractIcon(i);
		m_arButton[i].SetIcon(CSize(0), m_arIcon[i]);

		CString strToolTip;
		VERIFY(XTPResourceManager()->LoadString(&strToolTip, _arButtonID[i]));
		// Add tooltips to group buttons.
		m_tooltip.AddTool(&m_arButton[i], strToolTip);

		// make sure the button is Windows XP theme compatible using
		// the toolbar button theme.
		m_arButton[i].SetXButtonStyle(BS_XT_FLAT|BS_XT_WINXP_COMPAT);
		m_arButton[i].SetTheme(new CXTToolbarButtonTheme);
	}

	// Move the buttons to their correct location.
	MoveButtons();

	// Set the font for this window.
	if (bAutoFont)
	{
		SetFont(&XTAuxData().font);
	}
}

void CXTEditListBoxToolBar::SendCommand(UINT nCmdID)
{
	if (m_bEnableEdit)
	{
		CWnd* pOwner = GetOwner();
		ASSERT(pOwner);

		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			nCmdID), (LPARAM)m_hWnd);
	}
}

void CXTEditListBoxToolBar::OnButtonNew()
{
	SendCommand(LBN_XT_NEWITEM);
}

void CXTEditListBoxToolBar::OnButtonDelete()
{
	SendCommand(LBN_XT_DELETEITEM);
}

void CXTEditListBoxToolBar::OnButtonUp()
{
	SendCommand(LBN_XT_MOVEITEMUP);
}

void CXTEditListBoxToolBar::OnButtonDown()
{
	SendCommand(LBN_XT_MOVEITEMDOWN);
}

void CXTEditListBoxToolBar::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CStatic::OnWindowPosChanged(lpwndpos);

	// Move the buttons to their correct location.
	MoveButtons();
}

BOOL CXTEditListBoxToolBar::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTEditListBoxToolBar::DrawText(CDC* pDC, CRect& rcClient)
{
	// get the text for the control.
	CString strWindowText;
	GetWindowText(strWindowText);

	if (strWindowText.IsEmpty())
		return;

	// select the font into the device context.
	CFont* pOldFont = pDC->SelectObject(GetFont());

	// determine the location of the text.
	CSize size = pDC->GetTextExtent(strWindowText);
	int iOffset = (rcClient.Height()-size.cy)/2;

	// determine if the text is drawn disabled.
	CBrush brush;
	if (IsWindowEnabled())
		brush.CreateSolidBrush(GetXtremeColor(COLOR_BTNTEXT));
	else
		brush.CreateSolidBrush(GetXtremeColor(COLOR_GRAYTEXT));

	// draw the text and restore device context.
	pDC->DrawState(CPoint(iOffset,iOffset), size,
		strWindowText, DST_PREFIXTEXT|DSS_MONO, TRUE, 0, (CBrush*)&brush);
	pDC->SelectObject(pOldFont);
}

void CXTEditListBoxToolBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// Exclude the buttons from getting repainted.
	int i;
	for (i = 0; i < _countof(m_arClipRect); ++i)
		dc.ExcludeClipRect(&m_arClipRect[i]);

	CRect r;
	GetClientRect(&r);
	CXTMemDC memDC(&dc, r);

	DrawText(&memDC, r);
}

void CXTEditListBoxToolBar::OnEnable(BOOL bEnable)
{
	CStatic::OnEnable(bEnable);

	for (int i = 0; i < _countof(m_arButton); ++i)
	{
		m_arButton[i].EnableWindow(bEnable);
		m_arButton[i].Invalidate();
	}

	Invalidate();
}

void CXTEditListBoxToolBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CStatic::OnShowWindow(bShow, nStatus);

	for (int i = 0; i < _countof(m_arButton); ++i)
	{
		if (::IsWindow(m_arButton[i].GetSafeHwnd()))
			m_arButton[i].ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}
}
