// XTPCustomizeSheet.cpp : implementation of the CXTPCustomizeSheet class.
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
#include <math.h>
#include "XTPResourceManager.h"
#include "XTPImageManager.h"

#include "XTPCommandBarsResource.h"
#include "XTPCustomizeSheet.h"
#include "XTPControls.h"
#include "XTPToolBar.h"
#include "XTPHookManager.h"
#include "XTPCustomizeTools.h"
#include "XTPMouseManager.h"
#include "XTPControlButton.h"
#include "XTPControlEdit.h"
#include "XTPCommandBars.h"
#include "XTPCustomizeToolbarsPage.h"
#include "XTPCustomizeCommandsPage.h"
#include "XTPShortcutManager.h"
#include "XTPPaintManager.h"
#include "XTPImageEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



class CXTPCustomizeSheet::CCustomizeButton : public CXTPControlButton
{
	virtual void OnExecute()
	{
		CWnd* pOwner = m_pParent->GetOwnerSite();
		m_pParent->GetRootParent()->OnTrackLost();

		NMXTPCONTROL tagNMCONTROL;
		tagNMCONTROL.hdr.code = CBN_XTP_EXECUTE;
		tagNMCONTROL.hdr.idFrom = m_nId;
		tagNMCONTROL.hdr.hwndFrom = 0;
		tagNMCONTROL.pControl = this;

		if (!pOwner->SendMessage(WM_XTP_COMMAND, m_nId, (LPARAM)&tagNMCONTROL))
		{
			pOwner->SendMessage(WM_COMMAND, m_nId);
		}
	}
};

class CXTPCustomizeSheet::CCustomizeEdit : public CXTPControlEdit
{
	BOOL OnSetSelected(int bSelected)
	{
		if (!bSelected && m_pEdit && ::GetFocus() == *m_pEdit)
		{
			SendCommand(FALSE);
		}

		return CXTPControlEdit::OnSetSelected(bSelected);
	}

	virtual void OnExecute()
	{
		SendCommand(TRUE);
	}

	void SendCommand(BOOL bTrackLost)
	{
		CWnd* pOwner = m_pParent->GetOwnerSite();
		if (bTrackLost) m_pParent->OnTrackLost();

		NMXTPCONTROL tagNMCONTROL;
		tagNMCONTROL.hdr.code = CBN_XTP_EXECUTE;
		tagNMCONTROL.hdr.idFrom = m_nId;
		tagNMCONTROL.hdr.hwndFrom = 0;
		tagNMCONTROL.pControl = this;

		pOwner->SendMessage(WM_XTP_COMMAND, m_nId, (LPARAM)&tagNMCONTROL);
	}
};

/////////////////////////////////////////////////////////////////////////////
// CXTPCustomizeSheet

IMPLEMENT_DYNAMIC(CXTPCustomizeSheet, CPropertySheet)

CXTPCustomizeSheet::CXTPCustomizeSheet(CXTPCommandBars* pCommandBars, UINT iSelectPage)
{
	CString strCaption;
	XTPResourceManager()->LoadString(&strCaption, XTP_IDS_CUSTOMIZE);
	Construct(strCaption, pCommandBars->GetSite(), iSelectPage);

	m_pPageToolBars = new CXTPCustomizeToolbarsPage(this);
	AddPage(m_pPageToolBars);

	m_pPageCommands = new CXTPCustomizeCommandsPage(this);
	AddPage(m_pPageCommands);


	m_pWndParent = pCommandBars->GetSite();
	m_pCommandBars = pCommandBars;


	CBitmap bmp;
	XTPResourceManager()->LoadBitmap(&bmp, XTP_ID_CUSTOMIZE_CHANGEBUTTONIMAGE);

	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);
	int nCount = bmpInfo.bmWidth / 16;

	UINT* pIDs = new UINT[nCount];
	for (int i = 0; i < nCount; i++) pIDs[i] = i + 1;
	m_pCustomIcons = new CXTPImageManager;
	VERIFY(m_pCustomIcons->SetIcons(bmp, pIDs, nCount, CSize(16, 16), xtpImageNormal));
	delete[] pIDs;


	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	m_pContextMenu = 0;
	CreateContextMenu();
}

CXTPCustomizeSheet::~CXTPCustomizeSheet()
{
	delete m_pPageToolBars;
	delete m_pPageCommands;

	m_pContextMenu->InternalRelease();
	m_pCustomIcons->InternalRelease();
}

void CXTPCustomizeSheet::CreateContextMenu()
{
	CMenu menu;

	if (!XTPResourceManager()->LoadMenu(&menu, XTP_IDR_CUSTOMIZEMENU))
		return;

	CMenu* pMenu = menu.GetSubMenu(0);
	ASSERT(pMenu);

	m_pContextMenu = CXTPPopupBar::CreatePopupBar(NULL);
	m_pContextMenu->m_pSite = m_pCommandBars->GetSite();
	m_pContextMenu->m_pOwner = this;

	CXTPPaintManager* pPaintManager = m_pCommandBars->GetPaintManager();
	pPaintManager->InternalAddRef();
	m_pContextMenu->SetPaintManager(pPaintManager);

	m_pCustomIcons->InternalAddRef();
	m_pContextMenu->SetImageManager(m_pCustomIcons);

	BOOL bSeparator = FALSE;
	for (int i = 0; i < ::GetMenuItemCount(pMenu->m_hMenu); i++)
	{
		MENUITEMINFO info = { sizeof(MENUITEMINFO), MIIM_TYPE|MIIM_STATE};
		::GetMenuItemInfo(pMenu->m_hMenu, i, TRUE, &info);

		UINT nID = pMenu->GetMenuItemID(i);

		CString strCaption;
		pMenu->GetMenuString(i, strCaption, MF_BYPOSITION);

		if (((info.fType & MFT_SEPARATOR) == MFT_SEPARATOR) || (nID == 0))
			bSeparator = TRUE;
		else
		{
			CXTPControl* pControl = 0;

			if (nID == XTP_ID_CUSTOMIZE_CHANGEBUTTONIMAGE)
			{
				pControl = new CXTPControlPopup();
				CXTPPopupBar* pPopupBar= new CXTPPopupToolBar();
				pPopupBar->m_nBarID = XTP_ID_CUSTOMIZE_CHANGEBUTTONIMAGE;
				((CXTPControlPopup*)pControl)->SetCommandBar(pPopupBar);
				pPopupBar->InternalRelease();
			}
			else if (nID == XTP_ID_CUSTOMIZE_NAME)
				pControl =  new CCustomizeEdit();
			else
				pControl = new CCustomizeButton();

			pControl->SetCaption(strCaption);


			ASSERT(pControl);
			m_pContextMenu->GetControls()->Add(pControl, nID);

			if (bSeparator)
			{
				pControl->SetBeginGroup(TRUE);
				bSeparator = FALSE;
			}
		}
	}

	m_pContextMenu->m_bCustomizeMenu = TRUE;
}

void CXTPCustomizeSheet::AddPage(CPropertyPage* pPage)
{
	CPropertySheet::AddPage(pPage);

	LPCDLGTEMPLATE pResource = XTPResourceManager()->LoadDialogTemplate(pPage->m_psp.pszTemplate);

	if (pResource)
	{
		pPage->m_psp.pResource = pResource;
		pPage->m_psp.dwFlags |= PSP_DLGINDIRECT;
	}
}



BEGIN_MESSAGE_MAP(CXTPCustomizeSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CXTPCustomizeSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_COMMAND(XTP_ID_CUSTOMIZE_DEFAULTSTYLE, OnCustomizeDefaultStyle)
	ON_COMMAND(XTP_ID_CUSTOMIZE_IMAGEANDTEXT, OnCustomizeImageAndText)
	ON_COMMAND(XTP_ID_CUSTOMIZE_TEXTONLY, OnCustomizeTextOnly)
	ON_COMMAND(XTP_ID_CUSTOMIZE_BEGINAGROUP, OnCustomizeBeginGroup)
	ON_COMMAND(XTP_ID_CUSTOMIZE_DELETE, OnCustomizeDelete)
	ON_COMMAND(XTP_ID_CUSTOMIZE_COPYBUTTONIMAGE, OnCustomizeCopyImage)
	ON_COMMAND(XTP_ID_CUSTOMIZE_PASTEBUTTONIMAGE, OnCustomizePasteImage)
	ON_COMMAND(XTP_ID_CUSTOMIZE_RESETBUTTONIMAGE, OnCustomizeResetImage)
	ON_COMMAND(XTP_ID_CUSTOMIZE_RESET, OnCustomizeReset)
	ON_COMMAND(XTP_ID_CUSTOMIZE_EDITBUTTONIMAGE, OnCustomizeEditImage)

	ON_UPDATE_COMMAND_UI(XTP_ID_CUSTOMIZE_DEFAULTSTYLE, OnUpdateCustomizeDefaultStyle)
	ON_UPDATE_COMMAND_UI(XTP_ID_CUSTOMIZE_IMAGEANDTEXT, OnUpdateCustomizeImageAndText)
	ON_UPDATE_COMMAND_UI(XTP_ID_CUSTOMIZE_TEXTONLY, OnUpdateCustomizeTextOnly)
	ON_UPDATE_COMMAND_UI(XTP_ID_CUSTOMIZE_BEGINAGROUP, OnUpdateCustomizeBeginGroup)
	ON_UPDATE_COMMAND_UI(XTP_ID_CUSTOMIZE_NAME, OnUpdateCustomizeName)
	ON_UPDATE_COMMAND_UI(XTP_ID_CUSTOMIZE_COPYBUTTONIMAGE, OnUpdateCustomizeCopyImage)
	ON_UPDATE_COMMAND_UI(XTP_ID_CUSTOMIZE_CHANGEBUTTONIMAGE, OnUpdateCustomizeCopyImage)
	ON_UPDATE_COMMAND_UI(XTP_ID_CUSTOMIZE_PASTEBUTTONIMAGE, OnUpdateCustomizePasteImage)
	ON_UPDATE_COMMAND_UI(XTP_ID_CUSTOMIZE_RESETBUTTONIMAGE, OnUpdateCustomizeResetImage)
	ON_UPDATE_COMMAND_UI(XTP_ID_CUSTOMIZE_RESET, OnUpdateCustomizeReset)
	ON_UPDATE_COMMAND_UI(XTP_ID_CUSTOMIZE_EDITBUTTONIMAGE, OnUpdateCustomizeEditImage)

	ON_XTP_INITCOMMANDSPOPUP()
	ON_MESSAGE(WM_XTP_COMMAND, OnCustomizeName)
	ON_WM_SYSKEYDOWN()

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPCustomizeSheet message handlers


BOOL CXTPCustomizeSheet::OnInitDialog()
{

	BOOL bResult = CPropertySheet::OnInitDialog();


		// Remove the OK button.
	CWnd* pWnd = GetDlgItem( IDOK );
	if ( pWnd && ::IsWindow( pWnd->m_hWnd ) )
	{
		pWnd->ShowWindow( SW_HIDE );
	}

	// Rename the cancel button to 'Close"
	pWnd = GetDlgItem( IDCANCEL );
	if ( pWnd && ::IsWindow( pWnd->m_hWnd ) )
	{
		CString strClose;
		XTPResourceManager()->LoadString(&strClose, XTP_IDS_CUSTOMIZE_CLOSE);

		pWnd->SetWindowText( strClose );
	}


	return bResult;
}




typedef  BOOL (CXTPCustomizeSheet::*FILTERPROC)(WPARAM wParam, LPARAM lParam);

// A hook used in customization sheet to filter keyboard/mouse events
class CXTPCustomizeHook
{
public:


private:
	HHOOK m_hHook;
	FILTERPROC m_pFilter;
	CXTPCustomizeSheet* m_pSheet;

public:

	CXTPCustomizeHook(CXTPCustomizeSheet* pSheet, FILTERPROC pFilter)
		: m_pFilter(pFilter), m_hHook(0), m_pSheet(pSheet)
	{ }


	~CXTPCustomizeHook()
	{
		Unhook();
	}

	void Hook(int idHook, HOOKPROC lpfn)
	{
		ASSERT(m_hHook == 0);
		m_hHook = ::SetWindowsHookEx(idHook, lpfn, NULL, ::GetCurrentThreadId());
	}

	void Unhook()
	{
		if (m_hHook)
		{
			::UnhookWindowsHookEx(m_hHook);
			m_hHook = 0;
		}
	}

	LRESULT HandleEvent(int nCode, WPARAM wParam, LPARAM lParam)
	{

		if (nCode < 0)
			return TRUE;

		if ((m_pSheet->*m_pFilter)(wParam, lParam))
		{
			// process normally
			return  CallNextHookEx(m_hHook, nCode, wParam, lParam);
		}

		return TRUE;

	}
};

// A combination of keyboard/mouse hooks
class CXTPCustomizeHooks
{
private:

	CXTPCustomizeHook m_mouseHook;
	CXTPCustomizeHook m_keyHook;
	static CXTPCustomizeHooks* m_pThis;

public:

	CXTPCustomizeHooks(CXTPCustomizeSheet* pSheet, FILTERPROC FilterMouse, FILTERPROC FilterKey)
	: m_mouseHook(pSheet, FilterMouse)
	, m_keyHook(pSheet,FilterKey)
	{
		ASSERT(m_pThis == 0);  // one instance at a time
		m_pThis = this;
	}

	~CXTPCustomizeHooks()
	{
		Unhook();
		m_pThis = 0;
	}

	void Hook()
	{
		m_mouseHook.Hook(WH_MOUSE, HOOKPROC(MouseProc));
		m_keyHook.Hook(WH_KEYBOARD, HOOKPROC(KeyProc));
	}

	void Unhook()
	{
		m_mouseHook.Unhook();
		m_keyHook.Unhook();
	}

	static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		return m_pThis->m_mouseHook.HandleEvent(nCode, wParam, lParam);
	}

	static LRESULT CALLBACK KeyProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		return m_pThis->m_keyHook.HandleEvent(nCode, wParam, lParam);
	}

	friend class CXTPCustomizeSheet;
};
CXTPCustomizeHooks* CXTPCustomizeHooks::m_pThis = 0;

void CXTPCustomizeSheet::ContextMenu(CPoint point)
{
	if (m_pContextMenu)
	{
		BOOL bLayourRTL = m_pContextMenu->m_pSite->GetExStyle() & WS_EX_LAYOUTRTL;
		CXTPCommandBars::TrackPopupMenu(m_pContextMenu, bLayourRTL? TPM_RIGHTALIGN: 0, point.x, point.y, (CWnd*)m_pCommandBars->GetSite(), NULL, this);
	}
}



INT_PTR CXTPCustomizeSheet::DoModal()
{
	if (CXTPCustomizeHooks::m_pThis != 0)
		return IDCANCEL;

	ASSERT_VALID(this);
	ASSERT(m_hWnd == NULL);


	m_pCommandBars->SetCustomizeMode(TRUE);
	m_pCommandBars->GetDropSource()->m_pSheet = this;

	// register common controls
	VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTLS_REG));
#if _MSC_VER >= 1200 // MFC 6.0
	AfxDeferRegisterClass(AFX_WNDCOMMCTLSNEW_REG);
#endif //_MSC_VER >= 1200

	// finish building PROPSHEETHEADER structure
	BuildPropPageArray();

	// allow OLE servers to disable themselves
	CWinApp* pApp = AfxGetApp();
	if (pApp != NULL)
		pApp->EnableModeless(FALSE);

	// find parent HWND
	HWND hWndParent = m_pWndParent->GetSafeHwnd();


#if _MSC_VER != 1200 // !MFC 6.0
	PROPSHEETHEADER* psh = &m_psh;
#else
	AFX_OLDPROPSHEETHEADER* psh = GetPropSheetHeader();
#endif //_MSC_VER != 1200



	psh->hwndParent = hWndParent;
	CXTPCustomizeHooks hooks(this, &CXTPCustomizeSheet::FilterMouse, &CXTPCustomizeSheet::FilterKey);
	if (hWndParent)
		hooks.Hook();

	HWND hWndCapture = ::GetCapture();
	if (hWndCapture != NULL)
		::SendMessage(hWndCapture, WM_CANCELMODE, 0, 0);

	// setup for modal loop and creation
	m_nModalResult = 0;
	m_nFlags |= WF_CONTINUEMODAL;

	// hook for creation of window
	AfxHookWindowCreate(this);
	psh->dwFlags |= PSH_MODELESS;
	m_nFlags |= WF_CONTINUEMODAL;
	HWND hWnd = (HWND)::PropertySheet((PROPSHEETHEADER*)psh);
#ifdef _DEBUG
	DWORD dwError = ::GetLastError();
#endif
	psh->dwFlags &= ~PSH_MODELESS;
	AfxUnhookWindowCreate();

	// handle error
	if (hWnd == NULL || hWnd == (HWND)-1)
	{
		TRACE1("PropertySheet() failed: GetLastError returned %d\n", dwError);
		m_nFlags &= ~WF_CONTINUEMODAL;
	}

	int nResult = m_nModalResult;
	if (ContinueModal())
	{
		// enter modal loop
		DWORD dwFlags = MLF_SHOWONIDLE;
		if (GetStyle() & DS_NOIDLEMSG)
			dwFlags |= MLF_NOIDLEMSG;
		//CXTCustomizeSheetWndHook hook(m_bAppActive);  // disables window menu from taskbar
		nResult = RunModalLoop(dwFlags);
	}

	// hide the window before enabling parent window, etc.
	if (m_hWnd != NULL)
	{
		SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|
			SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
	}


	hooks.Unhook();
	bool bActivateParent = false;
	if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
	{
		bActivateParent = TRUE;
	}

	// cleanup
	DestroyWindow();

	if (bActivateParent)
	{
		::SetActiveWindow(hWndParent);
	}

	// allow OLE servers to enable themselves
	if (pApp != NULL)
		pApp->EnableModeless(TRUE);


	m_pCommandBars->SetCustomizeMode(FALSE);
	m_pCommandBars->GetDropSource()->m_pSheet = 0;

	return nResult;
}

static BOOL IsDescendant(HWND hwndParent, HWND hwnd)
{
	while (hwnd)
	{
		if (hwndParent == hwnd)
		{
			return TRUE;
		}
		hwnd = ::GetParent(hwnd);
	}
	return FALSE;
}

BOOL CXTPCustomizeSheet::FilterMouse(WPARAM wParam, LPARAM lParam)
{
	LPMOUSEHOOKSTRUCT pMsg = (LPMOUSEHOOKSTRUCT)(LPVOID)lParam;

	if (wParam == WM_MOUSEMOVE || wParam == WM_NCMOUSEMOVE)
		return TRUE;


	if (XTPMouseManager()->IsTrackedLock(NULL))
	{
		CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);
		if (pWnd->IsKindOf(RUNTIME_CLASS(CXTPCommandBar)))
		{
			if (((CXTPCommandBar*)pWnd)->GetCommandBars() == NULL)
				return  TRUE;
		}
		CWnd* pWndParent = pWnd? pWnd->GetParent(): 0;
		if (pWndParent && pWndParent->IsKindOf(RUNTIME_CLASS(CXTPCommandBar)))
		{
			if (((CXTPCommandBar*)pWndParent)->GetCommandBars() == NULL)
				return  TRUE;
		}

		CXTPMouseManager::CTrackArray& arrTracked = XTPMouseManager()->GetTrackArray();
		for (int i = 0; i < arrTracked.GetSize(); i++)
		{
			if (arrTracked[i]->GetCommandBars() == NULL)
			{
				XTPMouseManager()->DeliverMessage(arrTracked[i], wParam, pMsg->pt);
			}
		}
	}

	if (IsDescendant(m_pWndParent->m_hWnd, pMsg->hwnd))
	{
		if (IsDescendant(m_hWnd, pMsg->hwnd))
		{
			return TRUE;
		}
		if (::GetForegroundWindow() && IsDescendant(::GetForegroundWindow(), pMsg->hwnd))
		{
			return TRUE;
		}

		CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);
		CWnd* pWndParent = pWnd? pWnd->GetParent(): 0;

		if (pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CXTPCommandBar)))
		{
			CXTPCommandBars* pCommandBars = ((CXTPCommandBar*)pWnd)->GetCommandBars();
			return  pCommandBars == m_pCommandBars;
		}
		if (pWndParent && pWndParent->IsKindOf(RUNTIME_CLASS(CXTPCommandBar)))
		{
			CXTPCommandBars* pCommandBars = ((CXTPCommandBar*)pWndParent)->GetCommandBars();
			return  pCommandBars == m_pCommandBars;
		}

		// eat this message
		return FALSE;
	}
	return TRUE;
}

BOOL CXTPCustomizeSheet::FilterKey(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// allow keyboard events only if current focus window belongs to this one
	if (IsDescendant(m_hWnd, ::GetFocus()))
	{
		return TRUE;
	}

	CWnd* pWnd = GetFocus();
	CWnd* pWndParent = pWnd? pWnd->GetParent(): 0;
	if (pWndParent && pWndParent->IsKindOf(RUNTIME_CLASS(CXTPCommandBar)))
	{
		return TRUE;
	}

	if (::GetForegroundWindow() && IsDescendant(::GetForegroundWindow(), ::GetFocus()))
	{
		return TRUE;
	}



	// pass thru if the capture is set (toolbar tracking does this)
	if (::GetCapture() != 0)
	{
		return TRUE;
	}

	// eat this message
	return FALSE;
}


void CXTPCustomizeSheet::OnCustomizeDefaultStyle()
{
	CXTPControl* pControl = m_pCommandBars->GetDragControl();
	if (pControl)
	{
		pControl->m_buttonCustomStyle = xtpButtonAutomatic;
		pControl->GetParent()->OnRecalcLayout();
	}
}

void CXTPCustomizeSheet::OnCustomizeImageAndText()
{
	CXTPControl* pControl = m_pCommandBars->GetDragControl();
	if (pControl)
	{
		pControl->m_buttonCustomStyle = xtpButtonIconAndCaption;
		pControl->GetParent()->OnRecalcLayout();
	}
}
void CXTPCustomizeSheet::OnCustomizeTextOnly()
{
	CXTPControl* pControl = m_pCommandBars->GetDragControl();
	if (pControl)
	{
		pControl->m_buttonCustomStyle = xtpButtonCaption;
		pControl->GetParent()->OnRecalcLayout();
	}
}
void CXTPCustomizeSheet::OnCustomizeBeginGroup()
{
	CXTPControl* pControl = m_pCommandBars->GetDragControl();
	if (pControl)
	{
		pControl->SetBeginGroup(!pControl->GetBeginGroup());
		pControl->GetParent()->OnRecalcLayout();
	}
}
void CXTPCustomizeSheet::OnCustomizeDelete()
{
	CXTPControl* pControl = m_pCommandBars->GetDragControl();
	if (pControl)
	{
		GetCommandBars()->SetDragControl(NULL);

		CXTPCommandBar* pCommandBar = pControl->GetParent();
		pCommandBar->GetControls()->Remove(pControl);
		pCommandBar->OnRecalcLayout();
	}
}


void CXTPCustomizeSheet::OnCustomizeEditImage()
{
	XTPMouseManager()->SendTrackLost();

	CXTPControl* pControl = m_pCommandBars->GetDragControl();
	if (pControl && pControl->GetIconId() != 0)
	{
		CSize szIcon = pControl->GetParent()->GetIconSize();

		CXTPImageManagerIcon* pImage = m_pCommandBars->GetImageManager()->GetImage(pControl->GetIconId(), szIcon.cx);

		CXTPImageEditorDlg ed(this);
		if (pImage)
		{
			ed.SetIcon(pImage->GetIcon());
		}
		else
		{
			ed.SetIconSize(szIcon);
		}

		if (ed.DoModal() == IDOK)
		{
			CXTPImageManagerIconHandle hIcon;
			ed.GetIcon(&hIcon);

			if (hIcon.IsEmpty())
				return;

			if (pControl->m_nCustomIconId == 0)
			{
				pControl->m_nCustomIconId = m_pCommandBars->GetImageManager()->AddCustomIcon(hIcon);
			} else
			{
				pImage->SetIcon(hIcon);

			}

			pControl->GetParent()->OnRecalcLayout();
		}
	}

}


void CXTPCustomizeSheet::OnCustomizeCopyImage()
{
	CXTPControl* pControl = m_pCommandBars->GetDragControl();
	if (pControl && pControl->GetIconId() != 0)
	{
		CXTPImageManagerIcon* pImage = m_pCommandBars->GetImageManager()->GetImage(pControl->GetIconId());
		if (pImage)
		{
			CXTPImageManagerIconHandle& hIcon = pImage->GetIcon();
			CBitmap bitmapCopy;

			if (!hIcon.IsAlpha())
			{
				CSize sz = hIcon.GetExtent();

				CWindowDC dc(this);

				CDC memDC;
				memDC.CreateCompatibleDC (&dc);


				if (!bitmapCopy.CreateCompatibleBitmap (&dc, sz.cx, sz.cy))
				{
					return;
				}

				CBitmap* pOldBitmap = memDC.SelectObject (&bitmapCopy);
				memDC.FillSolidRect(0, 0, sz.cx, sz.cy, GetSysColor(COLOR_3DFACE));
				memDC.DrawState(CPoint(0, 0), sz,  hIcon.GetIcon(), (UINT)DSS_NORMAL, HBRUSH(0));
				memDC.SelectObject (pOldBitmap);
			} else
			{
				bitmapCopy.Attach(CXTPImageManagerIcon::CopyAlphaBitmap(hIcon.GetBitmap()));
			}


			if (!OpenClipboard ())
			{
				return;
			}

			if (!::EmptyClipboard ())
			{
				::CloseClipboard ();
				return;
			}

			HANDLE hclipData = ::SetClipboardData (CF_BITMAP, bitmapCopy.Detach ());
			if (hclipData == NULL)
			{
				TRACE (_T ("CXTPCustomizeSheet::OnCustomizeCopyImage() error. Error code = %x\n"), GetLastError ());
			}

			if (hIcon.IsAlpha())
			{
				SetClipboardData(XTPImageManager()->m_nAlphaClipFormat, GlobalAlloc(GMEM_MOVEABLE, 1));
			}

			if (m_pCommandBars->GetImageManager()->GetIconSet(pControl->GetIconId()))
			{
				m_pCommandBars->GetImageManager()->CopyImage(pControl->GetIconId());
			}

			::CloseClipboard ();
		}
	}
}

void CXTPCustomizeSheet::OnCustomizePasteImage()
{
	CXTPControl* pControl = m_pCommandBars->GetDragControl();
	if (pControl)
	{
		COleDataObject data;
		if (!data.AttachClipboard ())
		{
			return;
		}

		if (data.IsDataAvailable(m_pCommandBars->GetImageManager()->m_nImageClipFormat))
		{
			int nCustom = m_pCommandBars->GetImageManager()->PasteCustomImage(data);
			if (nCustom != 0)
			{
				pControl->m_nCustomIconId = nCustom;
				pControl->GetParent()->OnRecalcLayout();
			}

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

		CBitmap* pBmpClip = CBitmap::FromHandle (dataMedium.hBitmap);
		if (pBmpClip == NULL)
		{
			return;
		}

		CXTPImageManagerIconHandle hIcon;

		if (data.IsDataAvailable(XTPImageManager()->m_nAlphaClipFormat))
		{
			hIcon.CopyHandle(dataMedium.hBitmap);
		}
		else
		{
			BITMAP bmp;
			pBmpClip->GetBitmap (&bmp);

			CImageList imgList;
			imgList.Create(bmp.bmWidth, bmp.bmHeight, ILC_COLOR24|ILC_MASK, 0, 1);
			imgList.Add(pBmpClip, GetSysColor(COLOR_3DFACE));

			if (imgList.GetImageCount() != 1)
				return;

			hIcon = imgList.ExtractIcon(0);
		}

		if (hIcon.IsEmpty())
			return;

		pControl->m_nCustomIconId = m_pCommandBars->GetImageManager()->AddCustomIcon(hIcon);
		pControl->GetParent()->OnRecalcLayout();

	}
}
void CXTPCustomizeSheet::OnCustomizeResetImage()
{
	CXTPControl* pControl = m_pCommandBars->GetDragControl();
	if (pControl)
	{
		pControl->m_nCustomIconId = 0;
		pControl->GetParent()->OnRecalcLayout();
	}
}

void CXTPCustomizeSheet::OnCustomizeReset()
{
	CXTPControl* pControl = m_pCommandBars->GetDragControl();
	if (pControl)
	{
		pControl->m_nCustomIconId = 0;
		pControl->m_strCustomCaption = _T("");
		pControl->m_buttonCustomStyle = xtpButtonAutomatic;
		pControl->GetParent()->OnRecalcLayout();
	}
}

void CXTPCustomizeSheet::OnInitCommandsPopup(CXTPPopupBar* pPopupBar)
{
	if (pPopupBar->GetBarID() == XTP_ID_CUSTOMIZE_CHANGEBUTTONIMAGE)
	{
		if (pPopupBar->GetControls()->GetCount() > 0)
			return;

		int nCount = (int)m_pCustomIcons->GetImages()->GetCount();

		for (int i = 0; i < nCount; i++)
		{
			CXTPControl* pControl = pPopupBar->GetControls()->Add(new CCustomizeButton(), XTP_ID_CUSTOMIZE_CHANGEBUTTONIMAGE);
			pControl->SetIconId(i + 1);
			pControl->SetFlags(xtpFlagManualUpdate);
		}

		int nWidth = (int)pow((double)nCount, (double).5) * (16 + 7) + 6;
		pPopupBar->SetWidth(nWidth);
	}
}

BOOL CXTPCustomizeSheet::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (IsCustomizeCommand(nID))
		return CPropertySheet::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

	return m_pCommandBars->GetSite()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

LRESULT CXTPCustomizeSheet::OnCustomizeName(WPARAM wParam, LPARAM lParam)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)lParam;

	if (wParam == XTP_ID_CUSTOMIZE_NAME)
	{
		CXTPControlEdit* pControlEdit = (CXTPControlEdit*)tagNMCONTROL->pControl;
		ASSERT(pControlEdit->GetType() == xtpControlEdit);

		CXTPControl* pControl = m_pCommandBars->GetDragControl();
		if (pControl)
		{
			pControl->m_strCustomCaption = pControlEdit->GetEditText();
			pControl->GetParent()->OnRecalcLayout();
		}

		return TRUE;
	}

	if (wParam == XTP_ID_CUSTOMIZE_CHANGEBUTTONIMAGE)
	{
		UINT nID = tagNMCONTROL->pControl->GetIconId();

		CXTPControl* pControl = m_pCommandBars->GetDragControl();
		if (pControl)
		{
			CXTPImageManagerIconSet* pIconSet = m_pCustomIcons->GetIconSet(nID);
			ASSERT(pIconSet);
			ASSERT(pIconSet->GetIcons()->GetCount() == 1);

			if (pIconSet && pIconSet->GetIcons()->GetCount() == 1)
			{
				POSITION pos = pIconSet->GetIcons()->GetStartPosition();

				CXTPImageManagerIcon* pIcon = NULL;
				pIconSet->GetIcons()->GetNextAssoc(pos, nID, pIcon);


				pControl->m_nCustomIconId = m_pCommandBars->GetImageManager()->AddCustomIcon(pIcon->GetIcon());

				pControl->GetParent()->OnRecalcLayout();
			}
		}

		return TRUE;
	}

	if (!IsCustomizeCommand((int)wParam))
	{
		tagNMCONTROL->pControl->GetParent()->GetRootParent()->OnTrackLost();

		if (m_pCommandBars->GetSite()->SendMessage(WM_XTP_COMMAND, wParam, lParam) == 0)
		{
			m_pCommandBars->GetSite()->SendMessage(WM_COMMAND, wParam);
		}
		return TRUE;
	}

	return FALSE;
}


void CXTPCustomizeSheet::OnUpdateCustomizeDefaultStyle(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(FALSE);

	CXTPControl* pControl = m_pCommandBars->GetDragControl();
	if (pControl && (pControl->GetType() == xtpControlButton || IsPopupControlType(pControl->GetType())))
	{
		pCmdUI->SetCheck(pControl->m_buttonCustomStyle == xtpButtonAutomatic);
	}
	else pCmdUI->Enable(FALSE);
}

void CXTPCustomizeSheet::OnUpdateCustomizeImageAndText(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(FALSE);

	CXTPControl* pControl = m_pCommandBars->GetDragControl();
	if (pControl && (pControl->GetType() == xtpControlButton || IsPopupControlType(pControl->GetType())))
	{
		pCmdUI->SetCheck(pControl->m_buttonCustomStyle == xtpButtonIconAndCaption);
	}
	else pCmdUI->Enable(FALSE);
}

void CXTPCustomizeSheet::OnUpdateCustomizeTextOnly(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(FALSE);

	CXTPControl* pControl = m_pCommandBars->GetDragControl();
	if (pControl && (pControl->GetType() == xtpControlButton || IsPopupControlType(pControl->GetType())))
	{
		pCmdUI->SetCheck(pControl->m_buttonCustomStyle == xtpButtonCaption);
	}
	else pCmdUI->Enable(FALSE);
}

void CXTPCustomizeSheet::OnUpdateCustomizeCopyImage(CCmdUI* pCmdUI)
{
	CXTPControl* pControl = m_pCommandBars->GetDragControl();
	if (pControl)
	{
		XTPControlType controlType = pControl->GetType();
		BOOL bDrawImage = IsPopupControlType(controlType) || (controlType == xtpControlButton);
		pCmdUI->Enable(pControl->GetIconId() > 0 && bDrawImage);
	}
}

void CXTPCustomizeSheet::OnUpdateCustomizePasteImage(CCmdUI* pCmdUI)
{
	CXTPControl* pControl = m_pCommandBars->GetDragControl();
	if (pControl)
	{

		XTPControlType controlType = pControl->GetType();
		BOOL bDrawImage = IsPopupControlType(controlType) || (controlType == xtpControlButton);
		pCmdUI->Enable(::IsClipboardFormatAvailable(CF_BITMAP) && bDrawImage);
	}
}

void CXTPCustomizeSheet::OnUpdateCustomizeEditImage(CCmdUI* pCmdUI)
{
	CXTPControl* pControl = m_pCommandBars->GetDragControl();
	if (pControl)
	{
		XTPControlType controlType = pControl->GetType();
		BOOL bDrawImage = IsPopupControlType(controlType) || (controlType == xtpControlButton);
		pCmdUI->Enable(pControl->GetIconId() > 0 && bDrawImage);
	}

}


void CXTPCustomizeSheet::OnUpdateCustomizeResetImage(CCmdUI* pCmdUI)
{
	CXTPControl* pControl = m_pCommandBars->GetDragControl();
	if (pControl)
	{
		pCmdUI->Enable(pControl->m_nCustomIconId > 0);
	}
}
void CXTPCustomizeSheet::OnUpdateCustomizeReset(CCmdUI* pCmdUI)
{
	CXTPControl* pControl = m_pCommandBars->GetDragControl();
	if (pControl)
	{
		pCmdUI->Enable((pControl->m_nCustomIconId > 0) || (!pControl->m_strCustomCaption.IsEmpty())
			|| (pControl->m_buttonCustomStyle != xtpButtonAutomatic));
	}
}


void CXTPCustomizeSheet::OnUpdateCustomizeBeginGroup(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(FALSE);

	CXTPControl* pControl = m_pCommandBars->GetDragControl();
	if (pControl)
	{
		pCmdUI->SetCheck(pControl->GetBeginGroup());
		pCmdUI->Enable(pControl->GetIndex() != 0);
	}
}

void CXTPCustomizeSheet::OnUpdateCustomizeName(CCmdUI* pCmdUI)
{

	CXTPCommandBar* pCommandBar = (CXTPCommandBar*)pCmdUI->m_pOther;
	if (pCommandBar)
	{
		CXTPControlEdit* pEdit = (CXTPControlEdit*)pCommandBar->GetControls()->GetAt(pCmdUI->m_nIndex);

		CXTPControl* pControl = m_pCommandBars->GetDragControl();
		pEdit->SetEditText(pControl? pControl->GetCaption(): _T(""));
	}

	pCmdUI->Enable(TRUE);
}

void CXTPCustomizeSheet::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CPropertySheet::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CXTPCustomizeSheet::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_SYSKEYDOWN)
	{
		CTabCtrl* pTabCtrl = GetTabControl();
		if (pTabCtrl)
		{
			for (int i = 0; i < pTabCtrl->GetItemCount(); i++)
			{
				TCHAR lpCaption[256];
				TCITEM tcItem;
				tcItem.mask = TCIF_TEXT;
				tcItem.pszText = lpCaption;
				tcItem.cchTextMax = 256;

				if (pTabCtrl->GetItem(i, &tcItem))
				{
					TCHAR* pAmp = _tcsrchr(lpCaption, _T('&'));
					if (pAmp)
					{
						if (XTPShortcutManager()->ToUpper(*(pAmp + 1)) == (TCHAR)pMsg->wParam)
						{
							SetActivePage(i);
							return TRUE;
						}

					}
				}
			}

		}
	}
	return  CPropertySheet::PreTranslateMessage(pMsg);
}
