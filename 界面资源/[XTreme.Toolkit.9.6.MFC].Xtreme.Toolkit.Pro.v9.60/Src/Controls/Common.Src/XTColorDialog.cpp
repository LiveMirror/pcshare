// XTColorDialog.cpp : implementation file
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
#include "XTPVC80Helpers.h"  // Visual Studio 2005 helper functions
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
#include "XTColorPageCustom.h"
#include "XTColorPageStandard.h"
#include "XTMemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTColorDialog

IMPLEMENT_DYNAMIC(CXTColorDialog, CPropertySheet)

CXTColorDialog::CXTColorDialog(COLORREF clrNew, COLORREF clrCurrent,
	DWORD dwFlags/*=0L*/, CWnd* pWndParent/*=NULL*/)
	: CPropertySheet(XT_IDS_COLOR_CAPTION, pWndParent)
{
	m_psh.dwFlags = (m_psh.dwFlags & ~PSH_HASHELP)|PSH_NOAPPLYNOW;

	m_clrNew     = clrNew;
	m_clrCurrent = clrCurrent;
	m_dwStyle    = dwFlags;

	AddPage(new CXTColorPageStandard(this));
	AddPage(new CXTColorPageCustom(this));

	RevertResourceContext();
}

CXTColorDialog::~CXTColorDialog()
{
	int iPage;
	for (iPage = 0; iPage < GetPageCount(); ++iPage)
	{
		CPropertyPage* pPage = GetPage(iPage);
		SAFE_DELETE (pPage);
	}
}


BEGIN_MESSAGE_MAP(CXTColorDialog, CPropertySheet)
	//{{AFX_MSG_MAP(CXTColorDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTColorDialog message handlers

void CXTColorDialog::CalculateRects()
{
	CRect rcBtnOK;
	CRect rcBtnCancel;
	CRect rcTabCtrl;
	CRect rcItem;

	// get the tab control size.
	CTabCtrl* pTabCtrl = GetTabControl();
	pTabCtrl->GetWindowRect(&rcTabCtrl);
	ScreenToClient(&rcTabCtrl);

	// get the size of the first tab item.
	pTabCtrl->GetItemRect(0, &rcItem);

	// get the OK button size.
	CButton* pBtnOK = (CButton*)GetDlgItem(IDOK);
	pBtnOK->GetWindowRect(&rcBtnOK);
	ScreenToClient(&rcBtnOK);

	// get the Cancel button size.
	CButton* pBtnCancel = (CButton*)GetDlgItem(IDCANCEL);
	pBtnCancel->GetWindowRect(&rcBtnCancel);
	ScreenToClient(&rcBtnCancel);
	rcBtnCancel.OffsetRect(-15,0);

	// resize the tab control
	rcTabCtrl.right  = rcBtnCancel.left - 5;
	rcTabCtrl.bottom = rcBtnCancel.top - 15;
	pTabCtrl->MoveWindow(&rcTabCtrl);

	// reposition the OK button.
	rcBtnOK = rcBtnCancel;
	rcBtnOK.top = rcTabCtrl.top + rcItem.Height() + 1;
	rcBtnOK.bottom = rcBtnOK.top + rcBtnCancel.Height();
	pBtnOK->MoveWindow(&rcBtnOK);

	// reposition the Cancel button.
	rcBtnCancel = rcBtnOK;
	rcBtnCancel.top = rcBtnOK.bottom + 5;
	rcBtnCancel.bottom = rcBtnCancel.top + rcBtnOK.Height();
	pBtnCancel->MoveWindow(&rcBtnCancel);

	// reposition the hex display
	if (::IsWindow(m_editHex.m_hWnd))
	{
		CRect rcHex;
		rcHex = rcBtnCancel;
		rcHex.top = rcBtnCancel.bottom + 5;
		rcHex.bottom = rcHex.top + 18;
		m_editHex.MoveWindow(&rcHex);
	}

	// resize the property sheet.
	CXTPWindowRect rcWindow(this);
	ClientToScreen(&rcTabCtrl);
	rcWindow.bottom = rcTabCtrl.bottom + 10;
	rcWindow.right -= 15;
	MoveWindow(&rcWindow);
}

BOOL CXTColorDialog::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	CalculateRects();
	SetActivePage(0);

	return bResult;
}

void CXTColorDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(GetXtremeColor(COLOR_WINDOWTEXT));
	CXTPFontDC fontDC(&dc, &XTAuxData().font);

	// get the Cancel button size.
	CXTPWindowRect rcBtnCancel(GetDlgItem(IDCANCEL));
	ScreenToClient(&rcBtnCancel);

	// construct the size for the the new / current color box.
	CXTPClientRect rect(this);
	rect.right   = rcBtnCancel.right;
	rect.left    = rcBtnCancel.left;
	rect.bottom -= 32;
	rect.top     = rect.bottom - 66;

	// draw the borders for teh new / current color box.
	if (m_dwStyle & CPS_XT_SHOW3DSELECTION)
	{
		rect.InflateRect(3,3);
		dc.Draw3dRect(&rect, GetXtremeColor(COLOR_3DHILIGHT),
			GetXtremeColor(COLOR_3DDKSHADOW));

		rect.DeflateRect(3,3);
		dc.Draw3dRect(&rect, GetXtremeColor(COLOR_3DSHADOW),
			GetXtremeColor(COLOR_3DHILIGHT));
	}
	else
	{
		dc.Draw3dRect(&rect, GetXtremeColor(COLOR_WINDOWFRAME),
			GetXtremeColor(COLOR_WINDOWFRAME));
	}

	// draw the new text string.
	CRect rcText = rect;
	rcText = rect;
	rcText.top -= 22;
	rcText.bottom = rcText.top + 22;

	CString strText;
	VERIFY(XTPResourceManager()->LoadString(&strText, XT_IDS_NEW));
	dc.DrawText(strText, rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// draw the current text string.
	rcText = rect;
	rcText.top =  rcText.bottom;
	rcText.bottom = rcText.top + 22;

	VERIFY(XTPResourceManager()->LoadString(&strText, XT_IDS_CURRENT));
	dc.DrawText(strText, rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	rect.DeflateRect(1,1);
	int cy = rect.Height()/2;

	// fill the new color box.
	m_rcNew = rect;
	m_rcNew.bottom = rect.top + cy;
	dc.FillSolidRect(&m_rcNew, m_clrNew);

	// fill the current color box.
	m_rcCurrent = rect;
	m_rcCurrent.top = rect.bottom - cy;
	dc.FillSolidRect(&m_rcCurrent, m_clrCurrent);
}

void CXTColorDialog::SetNewColor(COLORREF clr, BOOL bNotify/*=TRUE*/)
{
	m_clrNew = clr;

	if (IsWindowVisible())
	{
		CClientDC dc(this);
		dc.FillSolidRect(&m_rcNew, m_clrNew);

		if (m_dwStyle & CPS_XT_SHOWHEXVALUE) {
			m_editHex.SetWindowText(RGBtoHex(m_clrNew));
		}
	}

	if (bNotify && GetTabControl() && GetTabControl()->GetSafeHwnd())
	{
		int iPage;
		for (iPage = 0; iPage < GetPageCount(); ++iPage)
		{
			GetPage(iPage)->SendMessage(XTWM_UPDATECOLOR,
				(WPARAM)(COLORREF)m_clrNew);
		}
	}
}

void CXTColorDialog::SetCurrentColor(COLORREF clr)
{
	m_clrCurrent = clr;

	if (IsWindowVisible())
	{
		CClientDC dc(this);
		dc.FillSolidRect(&m_rcCurrent, m_clrCurrent);
	}
}

BOOL CXTColorDialog::CopyToClipboard(CString strText)
{
	if (::OpenClipboard(m_hWnd))
	{
		::EmptyClipboard();

		HGLOBAL hGlobalBuff = ::GlobalAlloc(GMEM_MOVEABLE, strText.GetLength() + 1);

		CHAR* szBuffer = (CHAR*)::GlobalLock(hGlobalBuff);

		WCSTOMBS_S(szBuffer, strText, strText.GetLength() + 1);
		::GlobalUnlock(hGlobalBuff);

		if (::SetClipboardData(CF_TEXT, hGlobalBuff) == NULL)
			return FALSE;

		::CloseClipboard();
		return TRUE;
	}

	return FALSE;
}

CString CXTColorDialog::RGBtoHex(COLORREF clr)
{
	int r = GetRValue(clr);
	int g = GetGValue(clr);
	int b = GetBValue(clr);

	CString strHex;

	if (r < 16 && g < 16 && b < 16) {
		strHex.Format(_T("Hex=0%X0%X0%X"), r, g, b);
	}
	else if (r < 16 && g < 16) {
		strHex.Format(_T("Hex=0%X0%X%X"), r, g, b);
	}
	else if (r < 16 && b < 16) {
		strHex.Format(_T("Hex=0%X%X0%X"), r, g, b);
	}
	else if (g < 16 && b < 16) {
		strHex.Format(_T("Hex=%X0%X0%X"), r, g, b);
	}
	else if (r < 16) {
		strHex.Format(_T("Hex=0%X%X%X"), r, g, b);
	}
	else if (g < 16) {
		strHex.Format(_T("Hex=%X0%X%X"), r, g, b);
	}
	else if (b < 16) {
		strHex.Format(_T("Hex=%X%X0%X"), r, g, b);
	}
	else {
		strHex.Format(_T("Hex=%X%X%X"), r, g, b);
	}

	return strHex;
}

int CXTColorDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_dwStyle & CPS_XT_SHOWHEXVALUE)
	{
		if (!m_editHex.CreateEx(WS_EX_STATICEDGE, _T("EDIT"), RGBtoHex(m_clrNew),
			WS_CHILD|WS_VISIBLE|ES_READONLY, CRect(0,0,0,0), this, AFX_IDC_CHANGE))
		{
			TRACE0("Failed to create edit control.\n");
			return -1;
		}

		// Set the font and mask for the edit control.
		m_editHex.SetFont (&XTAuxData().font);
	}

	((CXTColorPageStandard*)GetPage(0))->SetColor(m_clrNew);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CXTThemePropertyPage base class

IMPLEMENT_DYNCREATE(CXTThemePropertyPage, CPropertyPage)

BEGIN_MESSAGE_MAP(CXTThemePropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CXTThemePropertyPage)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTThemePropertyPage construction/destruction

CXTThemePropertyPage::CXTThemePropertyPage(CXTColorDialog* pParentSheet, UINT nIDTemplate, UINT nIDCaption/*=0*/)
: CPropertyPage(nIDTemplate, nIDCaption)
{
	//{{AFX_DATA_INIT(CXTThemePropertyPage)
	//}}AFX_DATA_INIT

	m_psp.dwFlags &= ~PSP_HASHELP;

	ASSERT_VALID(pParentSheet); // must be valid!
	m_pParentSheet = pParentSheet;

	RevertResourceContext();
}

CXTThemePropertyPage::~CXTThemePropertyPage()
{
}

BOOL CXTThemePropertyPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// open Windows XP theme data for property page.
	if (!m_themeWrapper.ThemeDataOpen())
		m_themeWrapper.OpenThemeData(m_hWnd, L"TAB");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTThemePropertyPage::MakeTextTransparent(UINT nIDResource)
{
	CWnd* pWnd = GetDlgItem(nIDResource);
	if (pWnd && ::IsWindow(pWnd->m_hWnd))
	{
		TCHAR szClassName[255];
		::GetClassName(pWnd->GetSafeHwnd(), szClassName, 255);

		CString strClassName = szClassName;
		strClassName.MakeLower();

		if(strClassName ==  _T("static"))
		{
			CString strText;
			pWnd->GetWindowText(strText);

			if (!strText.IsEmpty())
			{
				m_arTransText.Add(pWnd);
				pWnd->ShowWindow(SW_HIDE);
			}
		}
	}
}

void CXTThemePropertyPage::DrawTransparentText(CDC* pDC)
{
	pDC->SetBkMode(TRANSPARENT);

	for (int i = 0; i < m_arTransText.GetSize(); ++i)
	{
		CWnd* pWnd = (CWnd*)m_arTransText.GetAt(i);
		if (pWnd && ::IsWindow(pWnd->m_hWnd))
		{
			CString strText;
			pWnd->GetWindowText(strText);

			if (!strText.IsEmpty())
			{
				CXTPFontDC fontDC(pDC, pWnd->GetFont());

				CRect rc;
				pWnd->GetWindowRect(&rc);
				ScreenToClient(&rc);

				DWORD dwStyle = pWnd->GetStyle();

				int nDTFlags = 0;

				if((dwStyle & SS_NOPREFIX) != 0)
					nDTFlags |= DT_NOPREFIX;

				if((dwStyle & SS_CENTER) != 0)
					nDTFlags |= DT_CENTER;
				else if((dwStyle & SS_RIGHT) != 0)
					nDTFlags |= DT_RIGHT;
				else
					nDTFlags |= DT_LEFT;

				if((dwStyle & SS_LEFTNOWORDWRAP) != 0)
					nDTFlags |= DT_SINGLELINE;
				else
					nDTFlags |= DT_WORDBREAK;

				pDC->DrawText(strText, -1, &rc, nDTFlags);
			}
		}
	}
}

void CXTThemePropertyPage::DrawThemedBack(CDC* pDC, CRect rc)
{
	// draw Windows XP theme if available.
	if (m_themeWrapper.IsAppThemeReady())
	{
		if (m_themeWrapper.DrawThemeBackground(pDC->GetSafeHdc(),
			TABP_BODY, PBS_NORMAL, &rc, NULL) == S_OK)
		{
			return;
		}
	}

	// draw default fill.
	pDC->FillSolidRect(&rc,
		GetXtremeColor(COLOR_3DFACE));
}

void CXTThemePropertyPage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	OnDrawPage(&dc);
}

BOOL CXTThemePropertyPage::OnEraseBkgnd(CDC* pDC)
{
	UNUSED_ALWAYS(pDC);
	return TRUE;
}

void CXTThemePropertyPage::OnDrawPage(CDC* pDC)
{
	DrawThemedBack(pDC, CXTPClientRect(this));
	DrawTransparentText(pDC);
}
