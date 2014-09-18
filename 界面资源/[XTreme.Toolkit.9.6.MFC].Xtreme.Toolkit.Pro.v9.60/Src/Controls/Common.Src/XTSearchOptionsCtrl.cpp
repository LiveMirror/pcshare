// CXTSearchOptionsCtrl : implementation file
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
#include "XTResource.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTMemDC.h"
#include "XTResizeRect.h"
#include "XTResizePoint.h"
#include "XTPWinThemeWrapper.h"
#include "XTResize.h"
#include "XTResizeFormView.h"
#include "XTSearchOptionsCtrl.h"
#include "XTPColorManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTSearchOptionsCtrl

CXTSearchOptionsCtrl::CXTSearchOptionsCtrl()
{
	m_strExpandLabel    = _T("");
	m_strContractLabel  = _T("");
	m_iMinSize          = ::GetSystemMetrics(SM_CYMENU);
	m_iMaxSize          = 0;
	m_bExpanded         = true;
	m_dwInitSignature   = 1;
}

CXTSearchOptionsCtrl::~CXTSearchOptionsCtrl()
{
}

IMPLEMENT_DYNAMIC(CXTSearchOptionsCtrl, CStatic)

BEGIN_MESSAGE_MAP(CXTSearchOptionsCtrl, CStatic)
	//{{AFX_MSG_MAP(CXTSearchOptionsCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTWM_INITIAL_UPDATE, OnInitControl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTSearchOptionsCtrl message handlers

void CXTSearchOptionsCtrl::OnEnable(BOOL bEnable)
{
	CStatic::OnEnable(bEnable);

	// enable/disable each of our windows in the hide controls array.
	int iIndex;
	for (iIndex = 0; iIndex < m_arHideCtrls.GetSize(); ++iIndex)
	{
		CWnd* pWndCtrl = (CWnd*)m_arHideCtrls.GetAt(iIndex);
		if (::IsWindow(pWndCtrl->GetSafeHwnd()))
		{
			pWndCtrl->EnableWindow(bEnable);
		}
	}

	// enable/disable each of the controls in our move controls array.
	for (iIndex = 0; iIndex < m_arMoveCtrls.GetSize(); ++iIndex)
	{
		CWnd* pWndCtrl = (CWnd*)m_arMoveCtrls.GetAt(iIndex);
		if (::IsWindow(pWndCtrl->GetSafeHwnd()))
		{
			pWndCtrl->EnableWindow(bEnable);
		}
	}
}

void CXTSearchOptionsCtrl::AddControl(CWnd* pWndCtrl)
{
	ASSERT_VALID(pWndCtrl); // must be valid.
	m_arHideCtrls.Add(pWndCtrl);
}

void CXTSearchOptionsCtrl::MoveControl(CWnd* pWndCtrl)
{
	ASSERT_VALID(pWndCtrl); // must be valid.
	m_arMoveCtrls.Add(pWndCtrl);
}

void CXTSearchOptionsCtrl::SetLabelText(LPCTSTR lpszExpand, LPCTSTR lpszContract)
{
	m_strExpandLabel   = lpszExpand;
	m_strContractLabel = lpszContract;
}

void CXTSearchOptionsCtrl::Expand()
{
	// set our expanded flag to true.
	m_bExpanded = true;

	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	// notify owner of expanding, this is sent prior to our
	// adjusting any controls to allow the parent to prepare
	// itself for resizing.

	if (pOwner != NULL)
	{
		pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			SON_XT_ITEMEXPANDING), (LPARAM)m_hWnd );
	}

	// expand this window.
	CRect rect;
	GetWindowRect(&rect);
	GetParent()->ScreenToClient(&rect);
	rect.bottom = rect.top + m_iMaxSize;
	MoveWindow(&rect);

	// show each of our windows in the hide controls array.
	int iIndex;
	for (iIndex = 0; iIndex < m_arHideCtrls.GetSize(); ++iIndex)
	{
		CWnd* pWndCtrl = (CWnd*)m_arHideCtrls.GetAt(iIndex);
		ASSERT_VALID(pWndCtrl); // must be valid.
		pWndCtrl->ShowWindow(SW_SHOW);
	}

	// move each of the controls in our move controls array.
	for (iIndex = 0; iIndex < m_arMoveCtrls.GetSize(); ++iIndex)
	{
		CWnd* pWndCtrl = (CWnd*)m_arMoveCtrls.GetAt(iIndex);
		ASSERT_VALID(pWndCtrl); // must be valid.

		CWnd* pParentWnd = pWndCtrl->GetParent();
		ASSERT_VALID(pParentWnd);

		CRect rc;
		pWndCtrl->GetWindowRect(&rc);
		pParentWnd->ScreenToClient(&rc);

		int iOffset = m_iMaxSize-m_iMinSize;
		rc.top += iOffset;
		rc.bottom += iOffset;

		// move the control and repaint it.
		pWndCtrl->MoveWindow(&rc);
		pWndCtrl->InvalidateRect(NULL);
	}

	// notify owner that expand has completed.

	if (pOwner != NULL)
	{
		pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			SON_XT_ITEMEXPAND), (LPARAM)m_hWnd );
	}
}

void CXTSearchOptionsCtrl::Contract()
{
	// set our expanded flag to false.
	m_bExpanded = false;

	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	// notify owner of contracting, this is sent prior to our
	// adjusting any controls to allow the parent to prepare
	// itself for resizing.

	if (pOwner != NULL)
	{
		pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			SON_XT_ITEMCONTRACTING), (LPARAM)m_hWnd );
	}

	// contract this window.
	CRect rect;
	GetWindowRect(&rect);

	if (m_iMaxSize == 0)
		m_iMaxSize = rect.Height();

	GetParent()->ScreenToClient(&rect);
	rect.bottom = rect.top + m_iMinSize;
	MoveWindow(&rect);

	// hide each of our windows in the hide controls array.
	int iIndex;
	for (iIndex = 0; iIndex < m_arHideCtrls.GetSize(); ++iIndex)
	{
		CWnd* pWndCtrl = (CWnd*)m_arHideCtrls.GetAt(iIndex);
		ASSERT_VALID(pWndCtrl); // must be valid.
		pWndCtrl->ShowWindow(SW_HIDE);
	}

	// move each of the controls in our move controls array.
	for (iIndex = 0; iIndex < m_arMoveCtrls.GetSize(); ++iIndex)
	{
		CWnd* pWndCtrl = (CWnd*)m_arMoveCtrls.GetAt(iIndex);
		ASSERT_VALID(pWndCtrl); // must be valid.

		CWnd* pParentWnd = pWndCtrl->GetParent();
		ASSERT_VALID(pParentWnd);

		CRect rc;
		pWndCtrl->GetWindowRect(&rc);
		pParentWnd->ScreenToClient(&rc);

		int iOffset = m_iMaxSize-m_iMinSize;
		rc.top -= iOffset;
		rc.bottom -= iOffset;

		// move the control and repaint it.
		pWndCtrl->MoveWindow(&rc);
		pWndCtrl->InvalidateRect(NULL);
	}

	// notify owner that contracting has completed.

	if (pOwner != NULL)
	{
		pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			SON_XT_ITEMCONTRACT), (LPARAM)m_hWnd );
	}
}

BOOL CXTSearchOptionsCtrl::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

void CXTSearchOptionsCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	GetClientRect(&rcClient);

	// exclude the windows in our "hide list" from paint routines.
	int iIndex;
	for (iIndex = 0; iIndex < m_arHideCtrls.GetSize(); ++iIndex)
	{
		CWnd* pWndCtrl = (CWnd*)m_arHideCtrls.GetAt(iIndex);
		ASSERT_VALID(pWndCtrl); // must be valid.

		CRect rcHide;
		pWndCtrl->GetWindowRect(&rcHide);
		ScreenToClient(&rcHide);

		dc.ExcludeClipRect(&rcHide);
	}

	// paint off screen.
	CXTMemDC memDC(&dc, rcClient, GetXtremeColor(COLOR_WINDOW));

	// save the current memDC.
	int iSavedDC = memDC.SaveDC();

	BOOL bIsEnabled = IsWindowEnabled();

	// construct the font's attributes
	if (m_bExpanded)
	{
		memDC.SelectObject(&XTAuxData().font);
		memDC.SetBkColor(bIsEnabled ? GetXtremeColor(COLOR_HIGHLIGHT) :
			GetXtremeColor(COLOR_BTNFACE));
		memDC.SetTextColor(bIsEnabled ? GetXtremeColor(COLOR_HIGHLIGHTTEXT) :
			GetXtremeColor(COLOR_GRAYTEXT));

		// if we are expanded, draw a border and fill caption.
		memDC.Draw3dRect(rcClient, GetXtremeColor(COLOR_HIGHLIGHT),
			GetXtremeColor(COLOR_HIGHLIGHT));

		CRect rcCaption(rcClient);
		rcCaption.bottom = rcCaption.top + m_iMinSize;
		rcCaption.top += 1;
		rcCaption.bottom -= 1;
		memDC.FillSolidRect(rcCaption, GetXtremeColor(COLOR_HIGHLIGHT));
	}
	else
	{
		memDC.SelectObject(&XTAuxData().fontULine);
		memDC.SetBkColor(GetXtremeColor(COLOR_WINDOW));
		memDC.SetTextColor(bIsEnabled ? GetXtremeColor(COLOR_HIGHLIGHT) :
			GetXtremeColor(COLOR_GRAYTEXT));
	}

	// get the size of the label's text.
	CSize size = memDC.GetTextExtent(m_bExpanded
		? m_strExpandLabel : m_strContractLabel);

	// construct the "hot" area used for mouse activation.
	GetWindowRect(&m_rcLabel);
	m_rcLabel.bottom = m_rcLabel.top + m_iMinSize;

	if (m_bExpanded)
		m_rcLabel.left += 3;
	else
		m_rcLabel.right = m_rcLabel.left + size.cx;

	// paint the label.
	ScreenToClient(&m_rcLabel);
	memDC.DrawText(m_bExpanded ? m_strExpandLabel : m_strContractLabel,
		&m_rcLabel, DT_SINGLELINE|DT_LEFT|DT_VCENTER);

	// restore the memDC.
	memDC.RestoreDC(iSavedDC);
}

void CXTSearchOptionsCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_rcLabel.PtInRect(point))
	{
		if (m_bExpanded) {
			Contract();
		}
		else {
			Expand();
		}

		Invalidate();
	}

	CStatic::OnLButtonDown(nFlags, point);
}

BOOL CXTSearchOptionsCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// get the cursor position in client coordinates.
	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);

	// if the cursor is over the label set the hand cursor.
	if( m_rcLabel.PtInRect(point) )
	{
		::SetCursor( XTAuxData().hcurHand );
		return TRUE;
	}

	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT CXTSearchOptionsCtrl::OnInitControl(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	if ((DWORD)lParam == m_dwInitSignature)
	{
		CRect rect;
		GetWindowRect(&rect);

		if (m_iMaxSize == 0)
			m_iMaxSize = rect.Height();

		// update display.
		Invalidate();
		UpdateWindow();
	}

	return 0;
}

void CXTSearchOptionsCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();

	// Initialize the control.
	DeferInitialUpdate();
}

void CXTSearchOptionsCtrl::DeferInitialUpdate()
{
	if (m_dwInitSignature > 0)
	{
		PostMessage(XTWM_INITIAL_UPDATE, 0, ++m_dwInitSignature);
	}
}

int CXTSearchOptionsCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize the control.
	DeferInitialUpdate();

	return 0;
}

void CXTSearchOptionsCtrl::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CStatic::OnWindowPosChanged(lpwndpos);

	// Update the control whenever the window size or position has changed.
	Invalidate();
	UpdateWindow();
}


/////////////////////////////////////////////////////////////////////////////

CXTSearchOptionsView::CResizeWnd::CResizeWnd(CWnd* pWndParent, CWnd* pWndChild, const XT_RESIZEPOINT& ptTopLeft, const XT_RESIZEPOINT& ptTopRight)
: m_pWndChild(pWndChild)
, m_pWndParent(pWndParent)
, m_rcWindow(0,0,0,0)
, m_rcSizing(CXTResizeRect(ptTopLeft.x, ptTopLeft.y, ptTopRight.x, ptTopRight.y))
{
	ASSERT_VALID(m_pWndChild);
	ASSERT_VALID(m_pWndParent);

	m_pWndChild->GetWindowRect(&m_rcWindow);
	m_pWndParent->ScreenToClient(&m_rcWindow);

	// if the control is a group box, then make it transparent
	if (IsGroupBox())
	{
		m_pWndChild->ModifyStyleEx(0, WS_EX_TRANSPARENT,
			SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
	}
}

CXTSearchOptionsView::CResizeWnd::~CResizeWnd()
{
}

bool CXTSearchOptionsView::CResizeWnd::IsGroupBox()
{
	if (!m_pWndChild || !::IsWindow(m_pWndChild->m_hWnd))
		return false;

	TCHAR szClass[8];
	::GetClassName(m_pWndChild->m_hWnd, szClass, 8);

	if (_tcsicmp(szClass, _T("Button")) == 0)
	{
		DWORD dwStyle = ::GetWindowLong(m_pWndChild->m_hWnd, GWL_STYLE);
		if ((dwStyle & (BS_GROUPBOX|WS_TABSTOP)) == BS_GROUPBOX)
		{
			return true;
		}
	}

	return false;
}

bool CXTSearchOptionsView::CResizeWnd::Resize(HDWP& hDWP, float dx)
{
	if (!m_pWndChild || !::IsWindow(m_pWndChild->m_hWnd))
		return false;

	if (dx > 0)
	{
		CXTResizeRect rrcWindow = m_rcWindow;
		CXTResizeRect rrcItem   = m_rcWindow;

		rrcItem.left   += dx * m_rcSizing.left;
		rrcItem.right  += dx * m_rcSizing.right;

		if (rrcItem != rrcWindow)
		{
			// get the size of the dialog item in client coordinates.
			CRect rcOld;
			m_pWndChild->GetWindowRect(&rcOld);
			m_pWndParent->ScreenToClient(&rcOld);

			int x  = (int) rrcItem.left;
			int y  = (int) rcOld.top;
			int cx = (int) rrcItem.Width();
			int cy = (int) rcOld.Height();

			// Set positioning flags
			UINT uFlags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER;

			// if the x-y coordinates have not changed, there is no reason
			// to move the dialog item.
			CRect rcNew = rrcItem;
			if (rcNew.TopLeft() == rcOld.TopLeft())
				uFlags |= SWP_NOMOVE;

			// if the cx-cy size has not changed, there is no reason to
			// size the dialog item.  If size has changed, don't
			// copy bits of the client area (i.e. have them invalidated/redrawn)
			if (rcNew.Size() == rcOld.Size())
				uFlags |= SWP_NOSIZE;
			else
				uFlags |= SWP_NOCOPYBITS;

			::DeferWindowPos(hDWP, m_pWndChild->m_hWnd, 0, x, y,
				cx, cy, uFlags);

			if (hDWP ==  NULL)
			{
				TRACE(_T("DeferWindowPos failed for ID %i\n"), m_pWndChild->GetDlgCtrlID());
				return false;
			}
		}
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CXTSearchOptionsView

IMPLEMENT_DYNAMIC(CXTSearchOptionsView, CFormView)

CXTSearchOptionsView::CXTSearchOptionsView(LPCTSTR lpszTemplateName)
: CFormView(lpszTemplateName)
, m_rcWindow(0,0,0,0)
{

}

CXTSearchOptionsView::CXTSearchOptionsView(UINT nIDTemplate)
: CFormView(nIDTemplate)
, m_rcWindow(0,0,0,0)
{

}

CXTSearchOptionsView::~CXTSearchOptionsView()
{
	while (!m_arResizeWnd.IsEmpty())
	{
		CResizeWnd* pResizeWnd = m_arResizeWnd.RemoveHead();
		SAFE_DELETE(pResizeWnd);
	}
	if (m_hBrush != NULL)
		::DeleteObject(m_hBrush);
}

BEGIN_MESSAGE_MAP(CXTSearchOptionsView, CFormView)
	//{{AFX_MSG_MAP(CXTSearchOptionsView)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTSearchOptionsView message handlers

HBRUSH CXTSearchOptionsView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nCtlColor);

	if (m_hBrush != NULL)
		::DeleteObject(m_hBrush);

	m_hBrush = ::CreateSolidBrush(GetXtremeColor(COLOR_WINDOW));

	pDC->SetBkColor(GetXtremeColor(COLOR_WINDOW));
	pDC->SetTextColor(GetXtremeColor(COLOR_WINDOWTEXT));

	return m_hBrush;
}

LRESULT CXTSearchOptionsView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRet = CFormView::WindowProc(message, wParam, lParam);

	if ( message == WM_INITDIALOG )
	{
		OnInitDialog();
	}

	return lRet;
}

BOOL CXTSearchOptionsView::OnInitDialog()
{
	GetWindowRect(&m_rcWindow);
	return TRUE;
}

void CXTSearchOptionsView::SetResize(int nID, const XT_RESIZEPOINT& ptTopLeft, const XT_RESIZEPOINT& ptTopRight)
{
	CResizeWnd* pResizeWnd = 0;

	// search for an item with the given id
	POSITION pos;
	for ( pos = m_arResizeWnd.GetHeadPosition(); pos; m_arResizeWnd.GetNext(pos))
	{
		CWnd* pWndChild = m_arResizeWnd.GetAt(pos)->m_pWndChild;
		if (pWndChild && ::IsWindow( pWndChild->m_hWnd ) && (pWndChild->GetDlgCtrlID() == nID))
		{
			pResizeWnd = m_arResizeWnd.GetAt(pos);
			break;
		}
	}

	if (pResizeWnd == 0)
	{
		HWND hWndChild = NULL;
		if ( hWndChild == NULL )
		{
			CWnd* pWnd = GetDlgItem( nID );
			if ( pWnd &&  ::IsWindow( pWnd->m_hWnd ) )
			{
				hWndChild = pWnd->m_hWnd;
			}
		}

		CWnd* pWnd = FromHandlePermanent( hWndChild );
		if ( pWnd == NULL )
		{
			// there is no permanent window, create one
			pWnd = new CWnd; // CXTItem handles the delete
			pWnd->Attach( hWndChild );
		}

		CResizeWnd* pResizeWnd = new CResizeWnd(this, pWnd, ptTopLeft, ptTopRight);
		m_arResizeWnd.AddHead(pResizeWnd);
	}
	else
	{
		pResizeWnd->m_rcSizing += CXTResizeRect(ptTopLeft.x, ptTopLeft.y, ptTopRight.x, ptTopRight.y);
	}
}

void CXTSearchOptionsView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	float iOffset = (float)(cx - m_rcWindow.Width());

	HDWP hDWP = ::BeginDeferWindowPos((int)m_arResizeWnd.GetCount());

	for (POSITION pos = m_arResizeWnd.GetHeadPosition(); pos; m_arResizeWnd.GetNext(pos))
	{
		CResizeWnd* pResizeWnd = m_arResizeWnd.GetAt(pos);
		if (pResizeWnd != NULL)
		{
			pResizeWnd->Resize(hDWP, iOffset);
		}
	}

	::EndDeferWindowPos(hDWP);
}

void CXTSearchOptionsView::AdjustScrollSizes(CXTSearchOptionsCtrl* pSOCtrl, bool bAdd)
{
	// get the scroll sizes for the form view.
	int nMapMode;
	CSize sizeTotal, sizePage, sizeLine;
	GetDeviceScrollSizes(nMapMode, sizeTotal, sizePage, sizeLine );

	// adjust the scroll sizes to accomindate for expansion.
	if (bAdd) {
		sizeTotal.cy += (pSOCtrl->GetOffsetSize());
	}
	else {
		sizeTotal.cy -= (pSOCtrl->GetOffsetSize());
	}

	SetScrollSizes(nMapMode, sizeTotal);

	// make sure the window and all siblings are updated.
	RedrawWindow(NULL, NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
}
