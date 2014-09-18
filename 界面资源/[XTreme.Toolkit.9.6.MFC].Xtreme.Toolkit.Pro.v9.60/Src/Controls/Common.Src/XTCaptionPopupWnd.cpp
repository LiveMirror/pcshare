// XTCaptionPopupWnd.cpp : implementation of the CXTCaptionPopupWnd class.
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
#include "XTPColorManager.h"
#include "XTPDrawHelpers.h"

#include "XTResource.h"
#include "XTDefines.h"
#include "XTUtil.h"
#include "XTGlobal.h"
#include "XTButton.h"
#include "XTThemeManager.h"
#include "XTCaption.h"
#include "XTCaptionPopupWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define XTCAPTIONPOPUP_CLASSNAME    _T("XTCaptionPopupWnd")  // Window class name

/////////////////////////////////////////////////////////////////////////////
// CXTCaptionPopupWnd
/////////////////////////////////////////////////////////////////////////////

CXTCaptionPopupWnd::CXTCaptionPopupWnd()
{
	m_pChildWnd     = NULL;
	m_pChildParent  = NULL;
	m_pParentWnd    = NULL;

	// register the wnd class.
	RegisterWindowClass();
}

CXTCaptionPopupWnd::~CXTCaptionPopupWnd()
{
	if (::IsWindow(m_hWnd))
		DestroyWindow();
}

IMPLEMENT_DYNAMIC(CXTCaptionPopupWnd, CWnd)

BEGIN_MESSAGE_MAP(CXTCaptionPopupWnd, CWnd)
	//{{AFX_MSG_MAP(CXTCaptionPopupWnd)
	ON_WM_DESTROY()
	ON_BN_CLICKED(XT_IDC_BTN_CLOSE, OnCaptButton)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTCaptionPopupWnd::RegisterWindowClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = ::AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, XTCAPTIONPOPUP_CLASSNAME, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style         = CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc   = ::DefWindowProc;
		wndcls.cbClsExtra    = 0;
		wndcls.cbWndExtra    = 0;
		wndcls.hInstance     = hInst;
		wndcls.hIcon         = NULL;
		wndcls.hCursor       = afxData.hcurArrow;
		wndcls.hbrBackground = ::GetSysColorBrush(COLOR_BTNFACE);
		wndcls.lpszMenuName  = NULL;
		wndcls.lpszClassName = XTCAPTIONPOPUP_CLASSNAME;

		if (!::AfxRegisterClass(&wndcls))
		{
			::AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CXTCaptionPopupWnd::Create(const CRect& rect, CWnd* pParentWnd, CWnd* pChildWnd)
{
	ASSERT_VALID(pParentWnd); // must be valid.
	if (!pParentWnd || !::IsWindow(pParentWnd->m_hWnd))
		return FALSE;

	m_pParentWnd = pParentWnd;

	ASSERT_VALID(pChildWnd); // must be valid.
	if (!pChildWnd || !::IsWindow(pChildWnd->m_hWnd))
		return FALSE;

	m_pChildWnd = pChildWnd;

	// define the styles for this window.
	DWORD dwStyleEx = WS_EX_TOOLWINDOW;
	DWORD dwStyle   = WS_VISIBLE|WS_POPUP|WS_DLGFRAME;

	// call the base class for creation.
	if( !CWnd::CreateEx( dwStyleEx, XTCAPTIONPOPUP_CLASSNAME, _T(""), dwStyle, rect.left, rect.top,
		rect.Width(), rect.Height(), pParentWnd->GetSafeHwnd(), 0, NULL ))
	{
		TRACE0( "Failed to create caption popdown window.\n" );
		return FALSE;
	}

	// set the child windows parent to this popup window, save
	// the previous parent, since we need to reset this when the
	// window is destroyed.
	m_pChildParent = m_pChildWnd->SetParent(this);
	ASSERT(m_pChildParent != NULL);

	// Create the caption.
	CString string;
	VERIFY(XTPResourceManager()->LoadString(&string, XT_IDS_CLICKKEEPOPEN));

	if( !m_Caption.Create( this, string, CPWS_EX_RAISED_EDGE,
		WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE, CXTPEmptyRect() ))
	{
		TRACE0( "Unable to create caption.\n" );
		return FALSE;
	}
	m_Caption.SetTheme(xtThemeDefault);

	// Create the caption button.
	if( !m_CaptionButton.Create( NULL, WS_VISIBLE|BS_ICON|BS_OWNERDRAW|BS_CENTER|BS_VCENTER,
		CXTPEmptyRect(), this, XT_IDC_BTN_CLOSE ))
	{
		TRACE0( "Unable to create caption button.\n" );
		return FALSE;
	}

	CImageList imageList;
	imageList.Create(16, 15, ILC_COLOR8|ILC_MASK, 0, 1);

	CBitmap bmp;
	VERIFY(XTPResourceManager()->LoadBitmap(&bmp, XT_IDB_BTN_OUT));
	imageList.Add(&bmp, RGB(255,0,255));

	// Associate the button icon
	m_hIconOff = imageList.ExtractIcon(0);
	m_hIconOn  = imageList.ExtractIcon(1);
	m_CaptionButton.SetIcon(CSize(16,15), m_hIconOff, m_hIconOn);
	imageList.DeleteImageList();

	if (XTThemeManager()->GetTheme() != xtThemeDefault)
	{
		m_CaptionButton.SetXButtonStyle(
			(m_CaptionButton.GetXButtonStyle() | BS_XT_XPFLAT) & ~BS_XT_FLAT);
	}

	// start the timer after everything has been initialized.
	SetTimer(1, 10, NULL);

	return TRUE;
}

void CXTCaptionPopupWnd::RecalcLayout()
{
	// define the caption and client rect.
	CXTPClientRect rcChild(this);
	rcChild.DeflateRect(2,2);

	// move the window to fit the client area of this window.
	CRect rcCaption(rcChild);
	rcChild.top += 19;
	m_pChildWnd->MoveWindow(rcChild);

	// define the size for the push pin caption.
	rcCaption.bottom = rcCaption.top + 19;
	m_Caption.MoveWindow(&rcCaption);

	// define the size for the caption button.
	CRect rcButton( rcCaption );
	rcButton.DeflateRect(2,2);
	rcButton.left = rcButton.right - 16;

	m_CaptionButton.MoveWindow(&rcButton);
}

BOOL CXTCaptionPopupWnd::ResetParent()
{
	if (!::IsWindow(m_pChildWnd->GetSafeHwnd()))
		return FALSE;

	if (!::IsWindow(m_pChildParent->GetSafeHwnd()))
		return FALSE;

	m_pChildWnd->SetParent(m_pChildParent);
	return TRUE;
}

void CXTCaptionPopupWnd::OnDestroy()
{
	ResetParent();
	CWnd::OnDestroy();
}

void CXTCaptionPopupWnd::OnCaptButton()
{
	// check to see if the parent is valid...
	if (::IsWindow(m_pParentWnd->GetSafeHwnd()))
	{
		// notify that push pin button was pressed.
		m_pParentWnd->SendMessage(CPWN_XT_PUSHPINBUTTON);
	}
}

void CXTCaptionPopupWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CXTPClientRect rc(this);
	rc.DeflateRect(1,1);

	// draw a border for the client area.
	dc.Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW),
		GetXtremeColor(COLOR_3DHILIGHT));

	// update the caption as well.
	m_Caption.Invalidate();
}

BOOL CXTCaptionPopupWnd::SendCancelMessage()
{
	// get a handle to the window which currently
	// has the input focus...
	HWND hWnd_HasFocus = ::GetFocus();

	// caption has focus.
	if (hWnd_HasFocus == m_Caption.GetSafeHwnd())
		return FALSE;

	// caption button has focus.
	if (hWnd_HasFocus == m_CaptionButton.GetSafeHwnd())
		return FALSE;

	// popup window has focus.
	if (hWnd_HasFocus == GetSafeHwnd())
		return FALSE;

	// popup window child has focus.
	if (hWnd_HasFocus == m_pChildWnd->GetSafeHwnd())
		return FALSE;

	// check to see if the parent is valid...
	if (::IsWindow(m_pParentWnd->GetSafeHwnd()))
	{
		// and send cancel message.
		m_pParentWnd->SendMessage(CPWN_XT_PUSHPINCANCEL);
		return TRUE;
	}

	return FALSE;
}

void CXTCaptionPopupWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		SendCancelMessage();
		return;
	}

	CWnd::OnTimer(nIDEvent);
}
