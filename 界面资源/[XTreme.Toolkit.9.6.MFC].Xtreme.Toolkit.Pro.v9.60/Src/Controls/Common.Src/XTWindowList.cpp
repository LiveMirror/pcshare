// XTWindowList.cpp : implementation file
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
#include "XTResource.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTMemDC.h"
#include "XTResizeRect.h"
#include "XTResizePoint.h"
#include "XTPWinThemeWrapper.h"
#include "XTResize.h"
#include "XTResizeDialog.h"
#include "XTButton.h"
#include "XTUtil.h"
#include "XTEditListBox.h"
#include "XTWindowList.h"
#include "XTWndHook.h"
#include "XTPColorManager.h"

#ifndef _XTP_INCLUDE_CONTROLS
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTStandardDefines.h"
#include "XTMenuBar.h"
#include "XTFrameImpl.h"
#include "XTMDIFrameWnd.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTWindowList dialog

CXTWindowList::CXTWindowList(CMDIFrameWnd* pMDIFrameWnd)
	: CXTResizeDialog(CXTWindowList::IDD, pMDIFrameWnd)
{
	//{{AFX_DATA_INIT(CXTWindowList)
	//}}AFX_DATA_INIT

	m_pMDIFrameWnd = pMDIFrameWnd;
	ASSERT_VALID(m_pMDIFrameWnd);

	m_hWndMDIClient = m_pMDIFrameWnd->m_hWndMDIClient;
	ASSERT(m_hWndMDIClient != NULL);

	m_hIcon = NULL;
}

void CXTWindowList::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTWindowList)
	DDX_Control(pDX, XT_IDC_BTN_MINIMIZE, m_btnMinimize);
	DDX_Control(pDX, XT_IDC_BTN_CASCADE, m_btnCascade);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, XT_IDC_LBOX_WINDOWS, m_lboxWindows);
	DDX_Control(pDX, XT_IDC_BTN_TILEVERT, m_btnTileVert);
	DDX_Control(pDX, XT_IDC_BTN_TILEHORZ, m_btnTileHorz);
	DDX_Control(pDX, XT_IDC_BTN_SAVE, m_btnSave);
	DDX_Control(pDX, XT_IDC_BTN_CLOSEWINDOWS, m_btnClose);
	DDX_Control(pDX, XT_IDC_BTN_ACTIVATE, m_btnActivate);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNAMIC(CXTWindowList, CXTResizeDialog)

BEGIN_MESSAGE_MAP(CXTWindowList, CXTResizeDialog)
	//{{AFX_MSG_MAP(CXTWindowList)
	ON_LBN_SELCHANGE(XT_IDC_LBOX_WINDOWS, OnSelchangeLBox)
	ON_BN_CLICKED(XT_IDC_BTN_ACTIVATE, OnBtnActivate)
	ON_BN_CLICKED(XT_IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(XT_IDC_BTN_CLOSEWINDOWS, OnBtnClosewindows)
	ON_BN_CLICKED(XT_IDC_BTN_TILEHORZ, OnBtnTilehorz)
	ON_BN_CLICKED(XT_IDC_BTN_TILEVERT, OnBtnTilevert)
	ON_BN_CLICKED(XT_IDC_BTN_CASCADE, OnBtnCascade)
	ON_BN_CLICKED(XT_IDC_BTN_MINIMIZE, OnBtnMinimize)
	ON_WM_DRAWITEM()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTWindowList message handlers

BOOL CXTWindowList::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	FillMDIWindowList();
	SelectMDIActive();
	SetEnableButtons();

	// Set control resizing.
	SetResize(XT_IDC_LBOX_WINDOWS,      SZ_TOP_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(XT_IDC_BTN_ACTIVATE,      SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDOK,                     SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(XT_IDC_BTN_SAVE,          SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(XT_IDC_BTN_CLOSEWINDOWS,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(XT_IDC_BTN_CASCADE,       SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(XT_IDC_BTN_TILEHORZ,      SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(XT_IDC_BTN_TILEVERT,      SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(XT_IDC_BTN_MINIMIZE,      SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	// Load window placement
	LoadPlacement(_T("CXTWindowList"));

	// If no icon was defined, used XT icon.
	if (m_hIcon == NULL) {
		m_hIcon = AfxGetApp()->LoadIcon(XT_IDI_XTREME);
	}

	SetIcon(m_hIcon, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTWindowList::OnDestroy()
{
	// Save window placement
	SavePlacement(_T("CXTWindowList"));

	CXTResizeDialog::OnDestroy();
}

CString CXTWindowList::GetChildWndText(HWND hWnd) const
{
	CString strTitle = _T("");

	// Get a pointer to the frame window.
	CMDIChildWnd* pChildFrame = (CMDIChildWnd*)CWnd::FromHandle(hWnd);
	if (!IsWindow(pChildFrame->GetSafeHwnd())) {
		return _T("");
	}

	// Get the window text for the frame and use it for the tab label.
	pChildFrame->GetWindowText(strTitle);

	// If the string is empty the document's title.
	if (strTitle.IsEmpty())
	{
		CWnd* pMDIChildWnd = pChildFrame->GetWindow(GW_CHILD);
		while (pMDIChildWnd)
		{
			if (pMDIChildWnd->IsKindOf(RUNTIME_CLASS(CView)))
			{
				// Get a pointer to the view's document.
				CDocument* pDocument = ((CView*)pMDIChildWnd)->GetDocument();
				if (pDocument == NULL) {
					return _T("");
				}

				// Set the return string value
				strTitle = pDocument->GetTitle();

				// Reset the frames title
				pChildFrame->SetWindowText(strTitle);

				// Return the document title.
				return strTitle;
			}
			pMDIChildWnd = pMDIChildWnd->GetWindow(GW_HWNDNEXT);
		}
	}

	// Return the MDI frame window's title.
	return strTitle;
}

void CXTWindowList::InsertMDIChild(CMDIChildWnd* pMDIChildWnd)
{
	ASSERT_VALID(pMDIChildWnd); // must be valid.

	// Get the window text for the frame.
	CString strWindowText = GetChildWndText(pMDIChildWnd->GetSafeHwnd());

	int iIndex = m_lboxWindows.AddString(strWindowText);
	m_lboxWindows.SetItemDataPtr(iIndex, (LPVOID)pMDIChildWnd->GetSafeHwnd());
}

void CXTWindowList::FillMDIWindowList()
{
	m_lboxWindows.ResetContent();

	// Retrieve the current active MDI child window.
	CMDIChildWnd* pActiveChild = (CMDIChildWnd*)m_pMDIFrameWnd->MDIGetActive();
	if (pActiveChild != NULL)
	{
		// Get the first top-level window in the list.
		CMDIChildWnd* pChildFrame = (CMDIChildWnd*)pActiveChild->GetWindow(GW_HWNDLAST);
		while (pChildFrame != NULL)
		{
			if (pChildFrame->IsWindowVisible())
			{
				// Insert the tab and get the next frame on the window manager’s list
				InsertMDIChild(pChildFrame);
			}

			pChildFrame = (CMDIChildWnd*)pChildFrame->GetWindow(GW_HWNDPREV);
		}
	}
}

void CXTWindowList::SelectMDIActive()
{
	int iCount = m_lboxWindows.GetCount();
	if (iCount != LB_ERR && iCount > 0)
	{
		m_lboxWindows.SelItemRange(FALSE, 0, iCount-1);

		HWND hWnd = m_pMDIFrameWnd->MDIGetActive()->GetSafeHwnd();
		int iItem;
		for (iItem = 0; iItem < iCount; iItem++)
		{
			if (hWnd == (HWND)m_lboxWindows.GetItemData(iItem))
			{
				m_lboxWindows.SetSel(iItem);
				return;
			}
		}
	}
}

void CXTWindowList::SetEnableButtons()
{
	int iCount = m_lboxWindows.GetSelCount();

	m_btnActivate.EnableWindow(iCount == 1);
	m_btnSave.EnableWindow(iCount > 0);
	m_btnClose.EnableWindow(iCount > 0);
	m_btnCascade.EnableWindow(iCount > 1);
	m_btnTileVert.EnableWindow(iCount > 1);
	m_btnTileHorz.EnableWindow(iCount > 1);
	m_btnMinimize.EnableWindow(iCount > 0);

	// hide buttons if menubar does not have frame button display
#ifdef _XTP_INCLUDE_CONTROLS

	m_btnCascade.ShowWindow(SW_SHOW);
	m_btnTileVert.ShowWindow(SW_SHOW);
	m_btnTileHorz.ShowWindow(SW_SHOW);
	m_btnMinimize.ShowWindow(SW_SHOW);

#else

	CXTMenuBar* pMenuBar = m_pMDIFrameWnd->GetMenuBar();
	if ( pMenuBar && ::IsWindow( pMenuBar->m_hWnd ) )
	{
		if ( !pMenuBar->IsShowMDIButtons( ) )
		{
			m_btnCascade.ShowWindow(SW_HIDE);
			m_btnTileVert.ShowWindow(SW_HIDE);
			m_btnTileHorz.ShowWindow(SW_HIDE);
			m_btnMinimize.ShowWindow(SW_HIDE);
		}
		else
		{
			m_btnCascade.ShowWindow(SW_SHOW);
			m_btnTileVert.ShowWindow(SW_SHOW);
			m_btnTileHorz.ShowWindow(SW_SHOW);
			m_btnMinimize.ShowWindow(SW_SHOW);
		}
	}
#endif
}

void CXTWindowList::OnSelchangeLBox()
{
	SetEnableButtons();
}

void CXTWindowList::OnBtnActivate()
{
	int iIndex = m_lboxWindows.GetCurSel();
	HWND hWnd = (HWND)m_lboxWindows.GetItemData(iIndex);

	if(hWnd != (HWND)LB_ERR)
	{
		WINDOWPLACEMENT wp;
		::GetWindowPlacement(hWnd, &wp);

		if(wp.showCmd == SW_SHOWMINIMIZED)
			::ShowWindow(hWnd, SW_RESTORE);

		::SendMessage(m_hWndMDIClient,
			WM_MDIACTIVATE, (WPARAM)hWnd, 0);

		CXTResizeDialog::OnOK();
	}
}

void CXTWindowList::OnBtnSave()
{
	int iCount = m_lboxWindows.GetCount();
	if (iCount != LB_ERR && iCount > 0)
	{
		int iItem;
		for (iItem = 0; iItem < iCount; iItem++)
		{
			if (m_lboxWindows.GetSel(iItem) > 0)
			{
				HWND hWnd = (HWND)m_lboxWindows.GetItemData(iItem);
				if (hWnd != NULL)
				{
					CMDIChildWnd* pMDIChildWnd = (CMDIChildWnd*)CWnd::FromHandle(hWnd);
					if (!pMDIChildWnd)
						continue;

					CDocument *pDocument = pMDIChildWnd->GetActiveDocument();
					if(pDocument != NULL) {
						pDocument->DoFileSave();
					}
				}
			}
		}
	}

	FillMDIWindowList();
	SelectMDIActive();
	SetEnableButtons();
}

void CXTWindowList::OnBtnClosewindows()
{
	int iCount = m_lboxWindows.GetCount();
	if (iCount != LB_ERR && iCount > 0)
	{
		int iItem;
		for (iItem = 0; iItem < iCount; iItem++)
		{
			if (m_lboxWindows.GetSel(iItem) > 0)
			{
				HWND hWnd = (HWND)m_lboxWindows.GetItemData(iItem);
				if (hWnd != NULL) {
					::SendMessage(hWnd, WM_CLOSE, 0,0);
				}
			}
		}
	}

	FillMDIWindowList();
	SelectMDIActive();
	SetEnableButtons();
}

void CXTWindowList::OnBtnTilehorz()
{
	CWaitCursor wait;

	int iCount = m_lboxWindows.GetCount();
	if (iCount != LB_ERR && iCount > 0)
	{
		::LockWindowUpdate(m_hWndMDIClient);

		int iItem;
		for (iItem = iCount-1; iItem >= 0; iItem--)
		{
			HWND hWnd=(HWND)m_lboxWindows.GetItemData(iItem);

			if (m_lboxWindows.GetSel(iItem) > 0)
				::ShowWindow(hWnd, SW_RESTORE);
			else
				::ShowWindow(hWnd, SW_MINIMIZE);
		}

		::SendMessage(m_hWndMDIClient, WM_MDITILE, MDITILE_HORIZONTAL, 0);
		::LockWindowUpdate(NULL);
	}
}

void CXTWindowList::OnBtnTilevert()
{
	CWaitCursor wait;

	int iCount = m_lboxWindows.GetCount();
	if (iCount != LB_ERR && iCount > 0)
	{
		::LockWindowUpdate(m_hWndMDIClient);

		int iItem;
		for (iItem = iCount-1; iItem >= 0; iItem--)
		{
			HWND hWnd=(HWND)m_lboxWindows.GetItemData(iItem);

			if (m_lboxWindows.GetSel(iItem) > 0)
				::ShowWindow(hWnd, SW_RESTORE);
			else
				::ShowWindow(hWnd, SW_MINIMIZE);
		}

		::SendMessage(m_hWndMDIClient, WM_MDITILE, MDITILE_VERTICAL, 0);
		::LockWindowUpdate(NULL);
	}
}

void CXTWindowList::OnBtnCascade()
{
	CWaitCursor wait;

	int iCount = m_lboxWindows.GetCount();
	if (iCount != LB_ERR && iCount > 0)
	{
		::LockWindowUpdate(m_hWndMDIClient);

		int iItem;
		for (iItem = iCount-1; iItem >= 0; iItem--)
		{
			HWND hWnd=(HWND)m_lboxWindows.GetItemData(iItem);

			if (m_lboxWindows.GetSel(iItem) > 0)
				::ShowWindow(hWnd, SW_RESTORE);
			else
				::ShowWindow(hWnd, SW_MINIMIZE);
		}

		::SendMessage(m_hWndMDIClient, WM_MDICASCADE, 0, 0);
		::LockWindowUpdate(NULL);
	}
}

void CXTWindowList::OnBtnMinimize()
{
	CWaitCursor wait;

	int iCount = m_lboxWindows.GetCount();
	if (iCount != LB_ERR && iCount > 0)
	{
		::LockWindowUpdate(m_hWndMDIClient);

		int iItem;
		for (iItem = iCount-1; iItem >= 0; iItem--)
		{
			HWND hWnd=(HWND)m_lboxWindows.GetItemData(iItem);

			DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

			if (m_lboxWindows.GetSel(iItem) > 0 && (dwStyle & WS_MINIMIZEBOX))
			{
				::ShowWindow(hWnd, SW_MINIMIZE);
			}
		}

		::SendMessage(m_hWndMDIClient, WM_MDICASCADE, 0, 0);
		::LockWindowUpdate(NULL);
	}
}

void CXTWindowList::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS)
{
	if (nIDCtl == XT_IDC_LBOX_WINDOWS)
	{
		if (lpDIS->itemID == (UINT)LB_ERR)
			return;

		CDC* pDC     = CDC::FromHandle(lpDIS->hDC);
		CRect rcItem = lpDIS->rcItem;
		int iSaveDC  = pDC->SaveDC();

		CSize sizeIcon;
		sizeIcon.cx = ::GetSystemMetrics(SM_CXSMICON);
		sizeIcon.cy = ::GetSystemMetrics(SM_CYSMICON);

		COLORREF crText = ::GetSysColor(COLOR_WINDOWTEXT);
		COLORREF crBack = ::GetSysColor(COLOR_WINDOW);

		if (lpDIS->itemState & ODS_SELECTED)
		{
			crBack = ::GetSysColor(COLOR_HIGHLIGHT);
			crText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
		}

		CRect rcLabel = rcItem;
		rcLabel.left += sizeIcon.cx + 3;

		if (lpDIS->itemAction & (ODA_DRAWENTIRE | ODA_SELECT))
			pDC->FillSolidRect(rcLabel, crBack);

		if (lpDIS->itemAction & ODA_FOCUS)
			pDC->DrawFocusRect(&rcLabel);

		CString strBuffer;
		m_lboxWindows.GetText(lpDIS->itemID, strBuffer);

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(crText);

		rcItem.left += sizeIcon.cx + 4;
		pDC->DrawText(strBuffer, -1, &rcItem,
			DT_LEFT|DT_VCENTER|DT_NOPREFIX| DT_SINGLELINE);
		rcItem.left = lpDIS->rcItem.left;

		// Try to locate the icon for the MDI window by calling WM_GETICON
		// first, this will give us the correct icon if the user has called
		// SetIcon(...) for the child frame.
		HICON hIcon = (HICON)::SendMessage((HWND)lpDIS->itemData, WM_GETICON, FALSE, 0);

		// If the icon returned is NULL, then try using GCL_HICONSM to get the
		// document icon for the child frame
		if (hIcon == NULL) {
			hIcon = (HICON)(ULONG_PTR)::GetClassLongPtr((HWND)lpDIS->itemData, GCL_HICONSM);
		}

		// If no icon was found, then use the default windows logo icon.
		if (hIcon == NULL) {
			hIcon = AfxGetApp()->LoadStandardIcon(IDI_WINLOGO);
		}

		rcItem.left += 2;
		::DrawIconEx(pDC->m_hDC, rcItem.left, rcItem.top, hIcon,
			sizeIcon.cx, sizeIcon.cy, 0, NULL, DI_NORMAL);

		pDC->RestoreDC(iSaveDC);
		return;
	}

	CXTResizeDialog::OnDrawItem(nIDCtl, lpDIS);
}
