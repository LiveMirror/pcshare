// SimpleGridPage.cpp : implementation file
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
#include "ownerdraw.h"
#include "SimpleGridPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimpleGridPage property page

IMPLEMENT_DYNCREATE(CSimpleGridPage, CPropertyPage)

CSimpleGridPage::CSimpleGridPage() : CPropertyPage(CSimpleGridPage::IDD)
{
	//{{AFX_DATA_INIT(CSimpleGridPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSimpleGridPage::~CSimpleGridPage()
{
}

void CSimpleGridPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimpleGridPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSimpleGridPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSimpleGridPage)
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimpleGridPage message handlers


LRESULT CSimpleGridPage::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_PGN_DRAWITEM)
	{
		DrawItem((LPDRAWITEMSTRUCT)lParam);
		return TRUE;
	}
	return FALSE;
}

BOOL CSimpleGridPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// get the size of the place holder, this will be used when creating the grid.
	CWnd* pPlaceHolder = GetDlgItem(IDC_PLACEHOLDER);

	CRect rc;
	pPlaceHolder->GetWindowRect( &rc );
	ScreenToClient( &rc );

	// create the property grid.
	if ( m_wndPropertyGrid.Create( rc, this, 100))
	{
		m_wndPropertyGrid.ModifyStyle(0, XTP_PGS_OWNERDRAW);

		LOGFONT lf;
		GetFont()->GetLogFont( &lf );

		// create document settings category.
		CXTPPropertyGridItem* pSettings        = m_wndPropertyGrid.AddCategory(_T("Document Settings"));

		// add child items to category.
		CXTPPropertyGridItem* pItemSaveOnClose = pSettings->AddChildItem(new CXTPPropertyGridItemBool(_T("SaveOnClose"), TRUE));
		pSettings->AddChildItem(new CXTPPropertyGridItemFont(_T("WindowFont"), lf));
		pSettings->AddChildItem(new CXTPPropertyGridItemSize(_T("WindowSize"), CSize(100, 100)));

		pSettings->Expand();
		pItemSaveOnClose->Select();

		// create global settings category.
		CXTPPropertyGridItem* pGlobals      = m_wndPropertyGrid.AddCategory(_T("Global Settings"));

		// add child items to category.
		CXTPPropertyGridItem* pItemGreeting = pGlobals->AddChildItem(new CXTPPropertyGridItem(_T("Greeting Text"), _T("Welcome to your application!")));
		pGlobals->AddChildItem(new CXTPPropertyGridItemNumber(_T("ItemsInMRUList"), 4));
		CXTPPropertyGridItem* pItemRate     = pGlobals->AddChildItem(new CXTPPropertyGridItemNumber(_T("MaxRepeatRate"), 10));
		pGlobals->AddChildItem(new CXTPPropertyGridItemColor(_T("ToolbarColor"), RGB(255, 192,128)));

		pItemGreeting->SetReadOnly(TRUE);
		pItemRate->SetDescription(_T("The rate in milliseconds that the text will repeat."));

		// create version category.
		CXTPPropertyGridItem* pVersion      = m_wndPropertyGrid.AddCategory(_T("Version"));

		// add child items to category.
		CXTPPropertyGridItem* pItemVersion  = pVersion->AddChildItem(new CXTPPropertyGridItem(_T("AppVerion"), _T("1.0")));

		pItemVersion->SetReadOnly(TRUE);
		pVersion->Expand();

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSimpleGridPage::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// hackers trick
	class CGridView : public CXTPPropertyGridView
	{
		friend class CSimpleGridPage;
	};

	CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lpDrawItemStruct->itemData;

	COLORREF clrWindow = GetSysColor(COLOR_WINDOW);

	ASSERT(pItem != NULL);

	CGridView* pGrid = (CGridView*)pItem->GetGrid();

	CXTPWindowRect rcWindow(pGrid);
	CRect rc = lpDrawItemStruct->rcItem;

	CXTPBufferDC dc(lpDrawItemStruct->hDC, rc);
	CXTPFontDC font(&dc, pItem->IsCategory()? &pGrid->m_fontBold: &pGrid->m_fontNormal);

	dc.FillSolidRect(rc, clrWindow);
	dc.SetBkMode(TRANSPARENT);


	CRect rcCaption(rc.left, rc.top, rc.left + pGrid->GetDividerPos(), rc.bottom);

	COLORREF clrLine = GetSysColor(COLOR_3DFACE);
	COLORREF clrFore = GetSysColor(COLOR_BTNTEXT);
	{
		if (!pItem->IsCategory())
		{

			dc.SetTextColor(pItem->GetReadOnly()? GetSysColor(COLOR_GRAYTEXT): clrFore);

			CRect rcValue(rc);
			rcValue.left = rcCaption.right + 4;
			rcValue.bottom -= 1;

			if (!pItem->OnDrawItemValue(dc, rcValue))
			{
				dc.DrawText( pItem->GetValue(), rcValue,  DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
			}
		}

		dc.SetTextColor(clrFore);

		BOOL bSelected = ((lpDrawItemStruct->itemAction | ODA_SELECT) && (lpDrawItemStruct->itemState & ODS_SELECTED));
		BOOL bFocused = bSelected && GetFocus() && ((GetFocus() == pGrid) || (GetFocus()->GetParent() == pGrid) || (GetFocus()->GetOwner() == pGrid));
		if (bFocused)
		{
			dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
			dc.FillSolidRect(CRect(XTP_PGI_EXPAND_BORDER, rcCaption.top, rcCaption.right, rcCaption.bottom), ::GetSysColor(COLOR_HIGHLIGHT));
		}
		else if (bSelected)
		{
			dc.FillSolidRect(CRect(XTP_PGI_EXPAND_BORDER, rcCaption.top, rcCaption.right, rcCaption.bottom), clrLine);
		}
		dc.FillSolidRect(XTP_PGI_EXPAND_BORDER - 1, rc.top, 1, rc.Height(), clrLine);

		CXTPPenDC pen(dc, clrLine);
		dc.MoveTo(XTP_PGI_EXPAND_BORDER, rc.bottom - 1); dc.LineTo(rc.right, rc.bottom - 1);
		dc.MoveTo(rcCaption.right, rc.top); dc.LineTo(rcCaption.right, rc.bottom - 1);


		CRect rcText(rc);
		rcText.left = max(1, pItem->GetIndent()) * XTP_PGI_EXPAND_BORDER + 3;
		rcText.right = rcCaption.right - 1;
		rcText.bottom -= 1;
		dc.DrawText( pItem->GetCaption(),   rcText,  DT_SINGLELINE|DT_VCENTER);
	}

	if (pGrid->GetCount() == (int)lpDrawItemStruct->itemID + 1)
		dc.FillSolidRect(0, rc.bottom - 1, rc.Width(), 1, clrLine);


	if (pItem->HasChilds())
	{
		CRgn rgn;
		rgn.CreateRectRgnIndirect(&rcCaption);
		dc.SelectClipRgn(&rgn);


		CRect rcSign(CPoint(XTP_PGI_EXPAND_BORDER / 2 - 5, rc.CenterPoint().y - 4), CSize(9, 9));

		if (pItem->GetIndent() > 1)
			rcSign.OffsetRect((pItem->GetIndent() - 1) * XTP_PGI_EXPAND_BORDER, 0);


		CPoint pt = rcSign.CenterPoint();

		CXTPBrushDC brush (dc,  clrWindow);
		CXTPPenDC pen (dc,  clrFore);
		dc.Rectangle(rcSign);

		dc.MoveTo(pt.x - 2, pt.y);
		dc.LineTo(pt.x + 3, pt.y);

		if(!pItem->IsExpanded())
		{
			dc.MoveTo(pt.x, pt.y - 2);
			dc.LineTo(pt.x, pt.y + 3);
		}

		dc.SelectClipRgn(NULL);
	}

}
