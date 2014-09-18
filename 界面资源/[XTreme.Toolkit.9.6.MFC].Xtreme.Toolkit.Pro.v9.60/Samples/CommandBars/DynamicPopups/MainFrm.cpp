// MainFrm.cpp : implementation of the CMainFrame class
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
#include "DynamicPopups.h"
#include "ControlUndo.h"
#include "ControlFavorites.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	//}}AFX_MSG_MAP

	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_XTP_CREATECONTROL()


	ON_XTP_EXECUTE(ID_FAVORITE_LINK, OnFavorite)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static UINT favoritesicons[] =
{
	ID_FAVORITE_FOLDER,
	ID_FAVORITE_LINK
};


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

		if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!InitCommandBars())
		return -1;

	CXTPPaintManager::SetTheme(xtpThemeOfficeXP);

	CXTPCommandBars* pCommandBars = GetCommandBars();

	pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);


	CXTPToolBar* pCommandBar = (CXTPToolBar*)pCommandBars->Add(_T("Standard"), xtpBarTop);
	if (!pCommandBar ||
		!pCommandBar->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	CXTPToolBar* pExtendedBar = (CXTPToolBar*)pCommandBars->Add(_T("Extended"), xtpBarTop);
	if (!pExtendedBar ||
		!pExtendedBar->LoadToolBar(IDR_TOOLBAR_EXT))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}


	XTPImageManager()->SetMaskColor(RGB(0, 255, 0));
	XTPImageManager()->SetIcons(IDB_FAVORITES, favoritesicons,
		sizeof(favoritesicons)/sizeof(UINT), CSize(16, 16));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::GetFavoriteFolder(CString& strFav)
{
	LPITEMIDLIST pidlFavorites = NULL;
	IMalloc * pMalloc = NULL;
	BOOL bResult = FALSE;
	TCHAR   szPath[MAX_PATH];

	HRESULT hr = SHGetMalloc(&pMalloc);
	if (SUCCEEDED(hr))
	{
		HRESULT hr = SHGetSpecialFolderLocation(NULL, CSIDL_FAVORITES, &pidlFavorites);
		if (hr == NOERROR )
		{
			if (SHGetPathFromIDList( pidlFavorites,szPath))
			{
				strFav = szPath;
				bResult = TRUE;
			}

		}

		pMalloc->Free(pidlFavorites);
		pMalloc->Release();

	}

	if (!bResult)
	{
		HKEY hKey;
		TCHAR sz[MAX_PATH];

		// find out from the registry where the favorites are located.
		if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\User Shell Folders"), &hKey) == ERROR_SUCCESS)
		{
			DWORD dwSize = MAX_PATH;
			RegQueryValueEx(hKey, _T("Favorites"), NULL, NULL, (LPBYTE)sz, &dwSize);
			ExpandEnvironmentStrings(sz, szPath, MAX_PATH);
			RegCloseKey(hKey);
			strFav = szPath;
			bResult = dwSize != 0;
		}

	}
	if (!bResult)
	{
		GetWindowsDirectory (szPath, MAX_PATH);
		strFav = szPath;
		strFav += "\\Favorites";
	}
	return TRUE;
}


int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->nID == ID_BUTTON_POPUP)
	{
		lpCreateControl->controlType = xtpControlSplitButtonPopup;
		return TRUE;
	}
	if (lpCreateControl->nID == ID_BUTTON_UNDO)
	{
		lpCreateControl->pControl = new CControlUndo();
		return TRUE;
	}
	if (lpCreateControl->nID == ID_FAVORITES)
	{
		CString strFav;
		GetFavoriteFolder(strFav);

		lpCreateControl->pControl = new CControlFavorites(strFav);
		return TRUE;

	}

	if (lpCreateControl->nID == ID_COMBO_UNDO)
	{
		lpCreateControl->pControl = new CControlComboBoxUndo();
		return TRUE;
	}


	return FALSE;
}

void CMainFrame::OnCustomize()
{
	CXTPCustomizeSheet cs(GetCommandBars());

	CXTPCustomizeOptionsPage pageOptions(&cs);
	cs.AddPage(&pageOptions);

	CXTPCustomizeCommandsPage* pCommands = cs.GetCommandsPage();
	pCommands->AddCategories(IDR_DYNAMITYPE);

	pCommands->InsertAllCommandsCategory();
	pCommands->InsertBuiltInMenus(IDR_DYNAMITYPE);
	pCommands->InsertNewMenuCategory();


	cs.DoModal();
}

void CMainFrame::OnFavorite(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;
	AfxMessageBox(tagNMCONTROL->pControl->GetParameter());

	*pResult = 1;
}
