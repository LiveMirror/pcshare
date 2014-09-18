// XTBrowseEdit.cpp : implementation of the CXTBrowseEdit class.
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
#include "XTPDrawHelpers.h"
#include "XTPWinThemeWrapper.h"

#include "XTFunctions.h"
#include "XTResource.h"
#include "XTDefines.h"
#include "XTUtil.h"
#include "XTGlobal.h"
#include "XTButton.h"
#include "XTWndHook.h"
#include "XTFlatComboBox.h"
#include "XTBrowseEdit.h"
#include "XTBrowseDialog.h"
#include "XTVersionManager.h"
#include "XTThemeManager.h"
#include "XTButtonTheme.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int CX_BUTTON = ( ::GetSystemMetrics( SM_CXHTHUMB ) +
	( ::GetSystemMetrics( SM_CXEDGE ) * 2 ) );

/////////////////////////////////////////////////////////////////////////////
// CXTBrowseButton class
/////////////////////////////////////////////////////////////////////////////

CXTBrowseButton::CXTBrowseButton()
{
	m_nBorderGap = 0;
	m_nImageGap  = 0;
	m_dwBStyle   = NULL;
	m_pEditWnd   = NULL;
	m_dwxStyle   = BS_XT_SHOWFOCUS|BS_XT_WINXP_COMPAT;

	SetTheme(new CXTDisabledButtonTheme);
}

CXTBrowseButton::~CXTBrowseButton()
{
}

IMPLEMENT_DYNAMIC(CXTBrowseButton, CXTButton)

BEGIN_MESSAGE_MAP(CXTBrowseButton, CXTButton)
	//{{AFX_MSG_MAP(CXTBrowseButton)
	ON_WM_ENABLE()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTBrowseButton::Create(CXTBrowseEdit* pEditWnd, DWORD dwBStyle, UINT nID, UINT nMenu/*=-1*/, UINT nSubMenuIndx/*=-1*/)
{
	ASSERT_VALID(pEditWnd);              // Must be valid
	ASSERT_VALID(pEditWnd->GetParent());

	// Set the pointers to the edit and edit parent windows.
	m_pEditWnd = pEditWnd;

	// Set the browse style and menu resource
	// ID information (if any).
	m_dwBStyle      = dwBStyle;
	m_nMenu         = nMenu;
	m_nSubMenuIndx  = nSubMenuIndx;

	CString strButtonText;

	DWORD dwStyle = (m_pEditWnd->GetStyle() & WS_VISIBLE) |
		WS_CHILD | WS_TABSTOP | BS_CENTER | BS_VCENTER | BS_OWNERDRAW;

	if ((m_dwBStyle & BES_XT_POPUPMENU) == 0)
	{
		VERIFY(XTPResourceManager()->LoadString(&strButtonText, XT_IDS_DOT));
	}
	else
	{
		CImageList imageList;
		imageList.Create(11, 11, ILC_COLOR8|ILC_MASK, 0, 1);

		CBitmap bmp;
		VERIFY(XTPResourceManager()->LoadBitmap(&bmp, XT_IDB_SCROLL_ARW));
		imageList.Add(&bmp, RGB(0xff,0x00,0xff));

		if (imageList.GetImageCount() == 8)
		{
			// extract the right arrow icon from the image list.
			m_hPopupIcon = imageList.ExtractIcon(4);
			ASSERT(m_hPopupIcon != NULL);

			// add the BS_ICON style to create flags.
			dwStyle |= BS_ICON;
		}
	}

	if( !CXTButton::Create(strButtonText, dwStyle, CRect(0,0,0,0),
		m_pEditWnd->GetParent(), nID))
	{
		TRACE0( "Failed to create browse edit button.\n" );
		return FALSE;
	}

	// Set the font for this control.
	if (!strButtonText.IsEmpty())
		SetFont(&XTAuxData().font);

	// set the arrow icon that is displayed on the button.
	if (NULL != (HICON)m_hPopupIcon)
		SetIcon(CSize(10,10), m_hPopupIcon);

	// set the tab order and enable the window based on parent's state.
	SetWindowPos( m_pEditWnd, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE );
	EnableWindow( m_pEditWnd->IsWindowEnabled( ));

	return TRUE;
}

void CXTBrowseButton::SetBrowseStyle(DWORD dwxStyle)
{
	m_dwBStyle = dwxStyle;

	if ((m_dwBStyle & BES_XT_POPUPMENU) == 0)
	{
		CString strButtonText;
		VERIFY(XTPResourceManager()->LoadString(&strButtonText, XT_IDS_DOT));
		SetWindowText(strButtonText);
	}
	else
	{
		CImageList imageList;
		imageList.Create(11, 11, ILC_COLOR8|ILC_MASK, 0, 1);

		CBitmap bmp;
		VERIFY(XTPResourceManager()->LoadBitmap(&bmp, XT_IDB_SCROLL_ARW));
		imageList.Add(&bmp, RGB(0xff,0x00,0xff));

		if (imageList.GetImageCount() == 8)
		{
			// extract the right arrow icon from the image list.
			m_hPopupIcon = imageList.ExtractIcon(4);
			ASSERT(m_hPopupIcon != NULL);

			// add the BS_ICON style to create flags.
			ModifyStyle(0, BS_ICON);

			// set the arrow icon that is displayed on the button.
			if (NULL != (HICON)m_hPopupIcon)
				SetIcon(CSize(10,10), m_hPopupIcon);
		}
	}
}

BOOL CXTBrowseButton::PreTranslateMessage(MSG* pMsg)
{
	// activate if space or enter key pressed...
	if (pMsg->message == WM_KEYDOWN &&
		(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_SPACE))
	{
		OnClicked();
		return TRUE; // eat message...
	}

	return CXTButton::PreTranslateMessage(pMsg);
}

void CXTBrowseButton::OnClicked()
{
	if (m_pEditWnd)
	{
		m_pEditWnd->OnBrowse();
	}
}

void CXTBrowseButton::OnEnable(BOOL bEnable)
{
	CXTButton::OnEnable(bEnable);
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// CXTBrowseEdit class
/////////////////////////////////////////////////////////////////////////////

CXTBrowseEdit::CXTBrowseEdit()
{
	m_nGap            = ::GetSystemMetrics(SM_CXEDGE);
	m_dwBStyle        = BES_XT_CHOOSEDIR;
	m_nMenu           = 0;
	m_nSubMenuIndx    = 0;
	m_strFileName     = _T("");
	m_dwFlags         = OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_EXPLORER;
	m_pParentWnd      = NULL;
	m_pFileDialog     = NULL;
	m_bOpenFileDialog = TRUE;
	m_bBrowsing       = false;

	VERIFY(XTPResourceManager()->LoadString(&m_strDefExt, XT_IDS_DEFEXT));
	VERIFY(XTPResourceManager()->LoadString(&m_strFilter, XT_IDS_DEFFILT));
	VERIFY(XTPResourceManager()->LoadString(&m_strTitle, XT_IDS_DEFTITLE));
}

CXTBrowseEdit::~CXTBrowseEdit()
{
}

IMPLEMENT_DYNAMIC(CXTBrowseEdit, CXTEdit)

BEGIN_MESSAGE_MAP(CXTBrowseEdit, CXTEdit)
	//{{AFX_MSG_MAP(CXTBrowseEdit)
	ON_WM_ENABLE()
	ON_WM_SHOWWINDOW()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTBrowseEdit::OnEnable(BOOL bEnable)
{
	CXTEdit::OnEnable(bEnable);

	if (::IsWindow(m_btnBrowse.GetSafeHwnd()))
		m_btnBrowse.EnableWindow(bEnable);

	Invalidate();
}

void CXTBrowseEdit::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CXTEdit::OnShowWindow( bShow, nStatus );

	if (::IsWindow(m_btnBrowse.GetSafeHwnd()))
		m_btnBrowse.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
}

bool CXTBrowseEdit::Initialize(CWnd* pParentWnd, DWORD dwBStyle, UINT nMenu/*=0*/, int nSubMenuIndx/*=0*/)
{
	if (!CXTEdit::Initialize(pParentWnd))
		return false;

	// only create a browse button if one was specified.
	if (m_dwBStyle & (BES_XT_CHOOSEDIR|BES_XT_CHOOSEFILE|BES_XT_POPUPMENU))
	{
		// set browse edit style and menu indexes.
		m_dwBStyle     = dwBStyle;
		m_nMenu        = nMenu;
		m_nSubMenuIndx = nSubMenuIndx;

		// Create the browse button associated with the browse edit control.
		if (!m_btnBrowse.Create(this, m_dwBStyle, XT_IDC_BTN_BROWSE,
			m_nMenu, m_nSubMenuIndx))
		{
			TRACE1("Error creating browse button for ID %d.\n", GetDlgCtrlID());
			return false;
		}

		CXTPWindowRect rcWindow(this);
		rcWindow.right -= CX_BUTTON + m_nGap;

		ASSERT_VALID(m_pParentWnd); // must be valid.
		m_pParentWnd->ScreenToClient(&rcWindow);

		// set the window position to accommodate for the browse button.
		::SetWindowPos( GetSafeHwnd(), NULL, rcWindow.left, rcWindow.top,
			rcWindow.Width(), rcWindow.Height(), SWP_FRAMECHANGED );

		// position browse button.
		PositionBrowseButton();
	}

	return true;
}

void CXTBrowseEdit::PositionBrowseButton()
{
	if (::IsWindow(m_btnBrowse.GetSafeHwnd()))
	{
		CXTPWindowRect rcWindow(this);

		// construct the size of the browse button.
		CRect rcButton;
		rcButton.left   = rcWindow.right + m_nGap;
		rcButton.right  = rcButton.left + CX_BUTTON;
		rcButton.top    = rcWindow.top;
		rcButton.bottom = rcWindow.top + rcWindow.Height();

		ASSERT_VALID(m_pParentWnd); // must be valid.
		m_pParentWnd->ScreenToClient(&rcButton);

		// set the window position and make sure tab order is correct.
		::SetWindowPos( m_btnBrowse.GetSafeHwnd(), GetSafeHwnd(),
			rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), SWP_FRAMECHANGED );

		// construct the area to be updated on the parent.
		CRect rcRedraw = rcWindow;
		rcRedraw.right = rcButton.right;

		// refresh parent window.
		m_pParentWnd->InvalidateRect(&rcRedraw);
	}
}

void CXTBrowseEdit::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CXTEdit::OnWindowPosChanged(lpwndpos);

	// position browse button.
	PositionBrowseButton();
}

void CXTBrowseEdit::ChooseDirectory()
{
	m_bBrowsing = true;

	// Get the current (if any) text that is displayed in
	// the edit control, this will be the initial path for the
	// browse dialog to start from.
	CString strPath;
	GetWindowText(strPath);

	// Instantiate a browse for folder dialog object
	CXTBrowseDialog browseDlg;
	browseDlg.SetOwner(GetParent()->GetSafeHwnd());
	browseDlg.SetTitle((TCHAR*)(LPCTSTR)m_strTitle);
	browseDlg.SetSelPath((TCHAR*)(LPCTSTR)strPath);

	if( browseDlg.DoModal() == IDOK ) {
		SetWindowText( browseDlg.GetSelPath() );
	}

	m_bBrowsing = false;
}

void CXTBrowseEdit::ChooseFile()
{
	m_bBrowsing = true;

	if ( m_pFileDialog )
	{
		if ( m_pFileDialog->DoModal( ) == IDOK )
			SetWindowText( m_pFileDialog->GetPathName( ) );
	}
	else
	{
		CFileDialog dlg( m_bOpenFileDialog, m_strDefExt, m_strFileName,
			m_dwFlags, m_strFilter, m_pParentWnd );

		// Check to see if this is Windows 2000 or later, if so use the
		// Windows 2000 version of OPENFILENAME.

		if (XTOSVersionInfo()->IsWin2KOrGreater() && sizeof(OPENFILENAME) < 88 && dlg.m_ofn.lStructSize < 88)
		{
			// Windows 2000 version of OPENFILENAME has three extra members,
			// this was copied from newer version of commdlg.h...

			struct OPENFILENAMEEX
			{
				void*  pvReserved; // 4 bytes
				DWORD  dwReserved; // 4 bytes
				DWORD  FlagsEx;    // 4 bytes
			};

			dlg.m_ofn.lStructSize += sizeof(OPENFILENAMEEX); // should equal an additional 12 bytes;
		}

		if( dlg.DoModal() == IDOK )
		{
			SetWindowText( dlg.GetPathName( ));
		}
	}

	m_bBrowsing = false;
}

void CXTBrowseEdit::PopupMenu()
{
	m_bBrowsing = true;

	ASSERT(m_nMenu != 0);
	if (::IsWindow(m_btnBrowse.GetSafeHwnd()))
		m_btnBrowse.SetState(TRUE);

	CRect rect;
	m_btnBrowse.GetWindowRect(&rect);

	// loading a user resource.
	CMenu menu;
	VERIFY(XTPResourceManager()->LoadMenu(&menu, m_nMenu));

	CMenu* pPopup = menu.GetSubMenu(m_nSubMenuIndx);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	XTFuncContextMenu(pPopup, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
		rect.right, rect.top, pWndPopupOwner, 0 );

	// Return the button state to normal.
	if (::IsWindow(m_btnBrowse.GetSafeHwnd()))
		m_btnBrowse.SetState(FALSE);

	m_bBrowsing = false;
}

void CXTBrowseEdit::OnBrowse()
{
	// If this is a browse directory control.
	switch (m_dwBStyle & (BES_XT_CHOOSEDIR|BES_XT_CHOOSEFILE|BES_XT_POPUPMENU))
	{
	case BES_XT_CHOOSEDIR:
		ChooseDirectory();
		break;
	case BES_XT_CHOOSEFILE:
		ChooseFile();
		break;
	case BES_XT_POPUPMENU:
		PopupMenu();
		break;
	}

	SetSel(0,-1);
	SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// CXTItemEdit class
/////////////////////////////////////////////////////////////////////////////

CXTItemEdit::CXTItemEdit(CWnd* pParent, const CRect& rect, CString& strWindowText, DWORD dwBStyle/*=BES_XT_CHOOSEDIR*/, bool bAutoDelete/*=true*/)
: m_strWindowText(strWindowText)
, m_bClosePosted(false)
, m_bModified(false)
, m_bAutoDelete(bAutoDelete)
, m_bEscapeKey(false)
{
	// Initialize defaults
	m_nGap       = 0;
	m_dwBStyle   = dwBStyle;
	m_pParentWnd = pParent;

	// Create the edit control.
	if (!Create( WS_BORDER|WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL,
		rect, pParent, XT_IDC_LBOX_EDIT))
		return;

	// Initialize the edit control.
	if (!Initialize(pParent, dwBStyle))
		return;

	// Set the window text and select it.
	SetWindowText(m_strWindowText);
	SetSel(0,-1);
	SetFocus();
}

CXTItemEdit::~CXTItemEdit()
{
}

IMPLEMENT_DYNAMIC(CXTItemEdit, CXTBrowseEdit)

BEGIN_MESSAGE_MAP(CXTItemEdit, CXTBrowseEdit)
	//{{AFX_MSG_MAP(CXTItemEdit)
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTItemEdit::OnKillFocus(CWnd* pNewWnd)
{
	CXTBrowseEdit::OnKillFocus(pNewWnd);

	if (!::IsWindow(m_btnBrowse.GetSafeHwnd()) || pNewWnd != &m_btnBrowse)
	{
		ASSERT_VALID(this);
		EndLabelEdit();
	}
}

void CXTItemEdit::PostNcDestroy()
{
	if (m_bAutoDelete) {
		delete this;
	}
}

BOOL CXTItemEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
	{
		if ((pMsg->wParam == VK_RETURN) ||
			(pMsg->wParam == VK_ESCAPE))
		{
			if (pMsg->wParam == VK_ESCAPE) {
				m_bEscapeKey = true;
			}

			EndLabelEdit();
			return TRUE;
		}
	}

	return CXTBrowseEdit::PreTranslateMessage(pMsg);
}

void CXTItemEdit::EndLabelEdit()
{
	if (m_bClosePosted) return;

	if (::IsWindow(m_btnBrowse.GetSafeHwnd()))
		m_btnBrowse.PostMessage( WM_CLOSE );

	if (::IsWindow(GetSafeHwnd()))
		PostMessage( WM_CLOSE );

	m_bClosePosted = true;

	if (m_bModified == false)
	{
		CWnd* pOwner = GetOwner();
		ASSERT_VALID(pOwner);

		if (m_bEscapeKey == false)
		{
			m_bModified = true;

			// get the window text and pass it on to the
			// notification window...
			GetWindowText(m_strWindowText);

			// Notify of label edit.
			pOwner->SendMessage( WM_COMMAND, MAKEWPARAM( GetDlgCtrlID(),
				BEN_XT_LABELEDITEND), (LPARAM)GetSafeHwnd() );
		}
		else
		{
			// Notify of label edit.
			pOwner->SendMessage( WM_COMMAND, MAKEWPARAM( GetDlgCtrlID(),
				BEN_XT_LABELEDITCANCEL), (LPARAM)GetSafeHwnd() );
		}
	}
}

void CXTItemEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ((nChar == VK_RETURN) ||
		(nChar == VK_ESCAPE))
	{
		if (nChar == VK_ESCAPE)
			m_bEscapeKey = TRUE;

		EndLabelEdit();
		return;
	}

	CXTBrowseEdit::OnChar(nChar, nRepCnt, nFlags);
}
