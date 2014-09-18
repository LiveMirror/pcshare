// XTPDockState.cpp : implementation of the CXTPDockState class.
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
#include "XTPImageManager.h"
#include "XTPDrawHelpers.h"
#include "XTPPropExchange.h"

#include "XTPCommandBarsResource.h"
#include "XTPShortcutManager.h"
#include "XTPDockState.h"
#include "XTPCommandBars.h"
#include "XTPDockBar.h"
#include "XTPDockContext.h"
#include "XTPControls.h"
#include "XTPControl.h"
#include "XTPControlPopup.h"
#include "XTPControlButton.h"
#include "XTPControlComboBox.h"
#include "XTPControlEdit.h"
#include "XTPPopupbar.h"
#include "XTPMenuBar.h"
#include "XTPToolBar.h"
#include "XTPDialogBar.h"
#include "XTPPaintManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


AFX_STATIC_DATA const TCHAR _afxVisible[] = _T("Visible");
AFX_STATIC_DATA const TCHAR _afxBarSection[] = _T("%s-Bar%d");
AFX_STATIC_DATA const TCHAR _afxDockBarSection[] = _T("%s-DockBar%d");
AFX_STATIC_DATA const TCHAR _afxSummarySection[] = _T("%s-Summary");
AFX_STATIC_DATA const TCHAR _afxXPos[] = _T("XPos");
AFX_STATIC_DATA const TCHAR _afxYPos[] = _T("YPos");
AFX_STATIC_DATA const TCHAR _afxMRUWidth[] = _T("MRUWidth");
AFX_STATIC_DATA const TCHAR _afxMRUDockID[] = _T("MRUDockID");
AFX_STATIC_DATA const TCHAR _afxMRUDockLeftPos[] = _T("MRUDockLeftPos");
AFX_STATIC_DATA const TCHAR _afxMRUDockRightPos[] = _T("MRUDockRightPos");
AFX_STATIC_DATA const TCHAR _afxMRUDockTopPos[] = _T("MRUDockTopPos");
AFX_STATIC_DATA const TCHAR _afxMRUDockBottomPos[] = _T("MRUDockBottomPos");
AFX_STATIC_DATA const TCHAR _afxMRUFloatXPos[] = _T("MRUFloatXPos");
AFX_STATIC_DATA const TCHAR _afxMRUFloatYPos[] = _T("MRUFloatYPos");

AFX_STATIC_DATA const TCHAR _afxBarID[] = _T("BarID");
AFX_STATIC_DATA const TCHAR _afxFloating[] = _T("Floating");
AFX_STATIC_DATA const TCHAR _afxBars[] = _T("Bars");
AFX_STATIC_DATA const TCHAR _afxScreenCX[] = _T("ScreenCX");
AFX_STATIC_DATA const TCHAR _afxScreenCY[] = _T("ScreenCY");
AFX_STATIC_DATA const TCHAR _afxBar[] = _T("Bar#%d");

AFX_STATIC_DATA const TCHAR _afxCommandBarControlsSection[] = _T("%s-Controls");
AFX_STATIC_DATA const TCHAR _afxCommandBarControls[] = _T("Controls");
AFX_STATIC_DATA const TCHAR _afxCommandBarLoadFromFile[] = _T("LoadFromFile");
AFX_STATIC_DATA const TCHAR _afxDialogBar[] = _T("DialogBar");


class CXTPToolBar::CToolBarInfo
{
public:
	CToolBarInfo();
	~CToolBarInfo();

	BOOL SaveState(LPCTSTR lpszProfileName, int nIndex);
	BOOL LoadState(LPCTSTR lpszProfileName, int nIndex, CXTPDockState* pDockState);

	void DoPropExchange(CXTPPropExchange* pPX, CXTPDockState* pDockState);


// Attributes
	UINT m_nBarID;      // ID of this bar
	BOOL m_bVisible;    // visibility of this bar
	BOOL m_bFloating;   // whether floating or not
	CPoint m_pointPos;  // topleft point of window
	UINT m_nMRUWidth;   // MRUWidth for Dynamic Toolbars
	XTPBarPosition m_uMRUDockPosition;  // most recent docked dockbar
	CRect m_rectMRUDockPos; // most recent docked position
	DWORD m_dwMRUFloatStyle; // most recent floating orientation
	CPoint m_ptMRUFloatPos; // most recent floating position
	CXTPToolBar* m_pBar;    // bar which this refers to (transient)

	BOOL m_bDialogBar;
	CSize m_szDialogBarDockingClient;
	CSize m_szDialogBarFloatingClient;
};



CXTPToolBar::CToolBarInfo::CToolBarInfo()
{
	m_nBarID = 0;
	m_bVisible = m_bFloating  = FALSE;
	m_pBar = NULL;
	m_pointPos.x = m_pointPos.y = -1;
	m_nMRUWidth = 32767;

	m_uMRUDockPosition = xtpBarTop;
	m_rectMRUDockPos.SetRectEmpty();
	m_dwMRUFloatStyle = 0;
	m_ptMRUFloatPos = CPoint(0,0);

	m_bDialogBar = FALSE;
	m_szDialogBarFloatingClient = m_szDialogBarDockingClient = CSize(200, 200);

	ASSERT(sizeof(DWORD) == sizeof(void*));
}
CXTPToolBar::CToolBarInfo::~CToolBarInfo()
{
}


void CXTPToolBar::CToolBarInfo::DoPropExchange(CXTPPropExchange* pPX, CXTPDockState* pDockState)
{
	ASSERT(pDockState!=NULL);

	PX_Long(pPX, _T("BarId"), (long&)m_nBarID, 0);
	PX_Bool(pPX, _T("Visible"), m_bVisible, TRUE);
	PX_Bool(pPX, _T("Floating"), m_bFloating, FALSE);
	PX_Long(pPX, _T("MRUWidth"), (long&)m_nMRUWidth);
	PX_Enum(pPX, _T("MRUDockPosition"), m_uMRUDockPosition, xtpBarTop);
	PX_Point(pPX, _T("PointPos"), m_pointPos, CPoint(0, 0));
	PX_Rect(pPX, _T("MRUDockPos"), m_rectMRUDockPos, CRect(0, 0, 0, 0));
	PX_Point(pPX, _T("MRUFloatPos"), m_ptMRUFloatPos, CPoint(0, 0));

	if (pDockState->GetVersion() > _XTP_SCHEMA_87)
	{
		PX_Bool(pPX, _T("DialogBar"), m_bDialogBar, FALSE);

		if (m_bDialogBar)
		{
			PX_Size(pPX, _T("DialogBarDockingClientSize"), m_szDialogBarDockingClient, CSize(0, 0));
			PX_Size(pPX, _T("DialogBarFloatingClientSize"), m_szDialogBarFloatingClient, CSize(0, 0));
		}
	}
}

BOOL CXTPToolBar::CToolBarInfo::SaveState(LPCTSTR lpszProfileName, int nIndex)
{
	TCHAR szSection[256];
	wsprintf(szSection, _afxBarSection, lpszProfileName, nIndex);

	// delete the section
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileString(szSection, NULL, NULL);

	pApp->WriteProfileInt(szSection, _afxBarID, m_nBarID);
	if (!m_bVisible)
		pApp->WriteProfileInt(szSection, _afxVisible, m_bVisible);
	if (m_bFloating)
	{
		pApp->WriteProfileInt(szSection, _afxFloating, m_bFloating);
	}
	if (m_pointPos.x != -1)
		pApp->WriteProfileInt(szSection, _afxXPos, m_pointPos.x);
	if (m_pointPos.y != -1)
		pApp->WriteProfileInt(szSection, _afxYPos, m_pointPos.y);
	if (m_nMRUWidth != 32767)
		pApp->WriteProfileInt(szSection, _afxMRUWidth, m_nMRUWidth);

	pApp->WriteProfileInt(szSection, _afxMRUDockID, m_uMRUDockPosition);
	pApp->WriteProfileInt(szSection, _afxMRUDockLeftPos, m_rectMRUDockPos.left);
	pApp->WriteProfileInt(szSection, _afxMRUDockTopPos, m_rectMRUDockPos.top);
	pApp->WriteProfileInt(szSection, _afxMRUDockRightPos, m_rectMRUDockPos.right);
	pApp->WriteProfileInt(szSection, _afxMRUDockBottomPos, m_rectMRUDockPos.bottom);
	pApp->WriteProfileInt(szSection, _afxMRUFloatXPos, m_ptMRUFloatPos.x);
	pApp->WriteProfileInt(szSection, _afxMRUFloatYPos, m_ptMRUFloatPos.y);

	pApp->WriteProfileInt(szSection, _afxDialogBar, m_bDialogBar);
	if (m_bDialogBar)
	{
		pApp->WriteProfileInt(szSection, _T("DialogBarDockingClientCx"), m_szDialogBarDockingClient.cx);
		pApp->WriteProfileInt(szSection, _T("DialogBarDockingClientCy"), m_szDialogBarDockingClient.cy);
		pApp->WriteProfileInt(szSection, _T("DialogBarFloatingClientCx"), m_szDialogBarFloatingClient.cx);
		pApp->WriteProfileInt(szSection, _T("DialogBarFloatingClientCy"), m_szDialogBarFloatingClient.cy);
	}

	//m_pBar->m_pControls->SaveState(szSection);

	return TRUE;

}
BOOL CXTPToolBar::CToolBarInfo::LoadState(LPCTSTR lpszProfileName, int nIndex, CXTPDockState* pDockState)
{
	ASSERT(pDockState != NULL);

	CWinApp* pApp = AfxGetApp();
	TCHAR szSection[256];
	wsprintf(szSection, _afxBarSection, lpszProfileName, nIndex);

	m_nBarID = pApp->GetProfileInt(szSection, _afxBarID, 0);
	m_bVisible = (BOOL) pApp->GetProfileInt(szSection, _afxVisible, TRUE);
	m_bFloating = (BOOL) pApp->GetProfileInt(szSection, _afxFloating, FALSE);
	m_pointPos = CPoint(
		pApp->GetProfileInt(szSection, _afxXPos, -1),
		pApp->GetProfileInt(szSection, _afxYPos, -1));

	pDockState->ScalePoint(m_pointPos);

	m_nMRUWidth = pApp->GetProfileInt(szSection, _afxMRUWidth, 32767);

	m_uMRUDockPosition = (XTPBarPosition)pApp->GetProfileInt(szSection, _afxMRUDockID, 0);

	m_rectMRUDockPos = CRect(
		pApp->GetProfileInt(szSection, _afxMRUDockLeftPos, 0),
		pApp->GetProfileInt(szSection, _afxMRUDockTopPos, 0),
		pApp->GetProfileInt(szSection, _afxMRUDockRightPos, 0),
		pApp->GetProfileInt(szSection, _afxMRUDockBottomPos, 0));
	pDockState->ScaleRectPos(m_rectMRUDockPos);

	m_ptMRUFloatPos = CPoint(
		pApp->GetProfileInt(szSection, _afxMRUFloatXPos, 0),
		pApp->GetProfileInt(szSection, _afxMRUFloatYPos, 0));
	pDockState->ScalePoint(m_ptMRUFloatPos);

	m_bDialogBar = pApp->GetProfileInt(szSection, _afxDialogBar, FALSE);
	if (m_bDialogBar)
	{
		m_szDialogBarDockingClient.cx = pApp->GetProfileInt(szSection, _T("DialogBarDockingClientCx"), m_szDialogBarDockingClient.cx);
		m_szDialogBarDockingClient.cy = pApp->GetProfileInt(szSection, _T("DialogBarDockingClientCy"), m_szDialogBarDockingClient.cy);
		m_szDialogBarFloatingClient.cx = pApp->GetProfileInt(szSection, _T("DialogBarFloatingClientCx"), m_szDialogBarFloatingClient.cx);
		m_szDialogBarFloatingClient.cy = pApp->GetProfileInt(szSection, _T("DialogBarFloatingClientCy"), m_szDialogBarFloatingClient.cy);
	}


	return TRUE;
}


void CXTPDockState::ScalePoint(CPoint& pt)
{
	if (m_bScaling)
	{
		CSize sizeDevice = m_rectDevice.Size();

		pt.x = MulDiv(pt.x, sizeDevice.cx, m_sizeLogical.cx);
		pt.y = MulDiv(pt.y, sizeDevice.cy, m_sizeLogical.cy);
	}
	if (pt.x > m_rectClip.right)
		pt.x = m_rectClip.right;
	if (pt.y > m_rectClip.bottom)
		pt.y = m_rectClip.bottom;
}

void CXTPDockState::ScaleRectPos(CRect& rect)
{
	CPoint pt;

	if (m_bScaling)
	{
		pt = rect.TopLeft();
		CSize sizeDevice = m_rectDevice.Size();

		pt.x = MulDiv(pt.x, sizeDevice.cx, m_sizeLogical.cx) - rect.left;
		pt.y = MulDiv(pt.y, sizeDevice.cy, m_sizeLogical.cy) - rect.top;
		rect.OffsetRect(pt);
	}
	pt.x = pt.y = 0;

	if (rect.left > m_rectClip.right)
		pt.x = m_rectClip.right - rect.left;
	if (rect.top > m_rectClip.bottom)
		pt.y = m_rectClip.bottom - rect.top;

	if (!((pt.x == 0) && (pt.y == 0)))
		rect.OffsetRect(pt);
}

CXTPDockState::CXTPDockState(void)
{
	m_dwVersion = _XTP_SCHEMA_CURRENT;

	m_bScaling = FALSE;

	m_rectDevice.left = 0;
	m_rectDevice.top = 0;
	m_rectDevice.right = GetSystemMetrics(SM_CXSCREEN);
	m_rectDevice.bottom = GetSystemMetrics(SM_CYSCREEN);

	m_rectClip = m_rectDevice;
	m_rectClip.right -= GetSystemMetrics(SM_CXICON);
	m_rectClip.bottom -= GetSystemMetrics(SM_CYICON);

}

CXTPDockState::~CXTPDockState(void)
{
	Clear();
}

void CXTPDockState::Clear()
{
	for (int i = 0; i < m_arrBarInfo.GetSize(); i++)
		delete m_arrBarInfo[i];
	m_arrBarInfo.RemoveAll();

}

void CXTPDockState::SaveState(LPCTSTR lpszProfileName)
{
	int nIndex = 0;
	for (int i = 0;i < m_arrBarInfo.GetSize(); i++)
	{
		TOOLBARINFO* pInfo = m_arrBarInfo[i];
		ASSERT(pInfo != NULL);
		if (pInfo->SaveState(lpszProfileName, nIndex))
			nIndex++;
	}


	CWinApp* pApp = AfxGetApp();
	TCHAR szSection[256];
	wsprintf(szSection, _afxSummarySection, lpszProfileName);
	pApp->WriteProfileInt(szSection, _afxBars, nIndex);

	CSize size = GetScreenSize();
	pApp->WriteProfileInt(szSection, _afxScreenCX, size.cx);
	pApp->WriteProfileInt(szSection, _afxScreenCY, size.cy);
}

void CXTPDockState::LoadState(LPCTSTR lpszProfileName)
{
	CWinApp* pApp = AfxGetApp();
	TCHAR szSection[256];
	wsprintf(szSection, _afxSummarySection, lpszProfileName);
	int nBars = pApp->GetProfileInt(szSection, _afxBars, 0);

	CSize size;
	size.cx = pApp->GetProfileInt(szSection, _afxScreenCX, 0);
	size.cy = pApp->GetProfileInt(szSection, _afxScreenCY, 0);
	SetScreenSize(size);

	for (int i = 0; i < nBars; i++)
	{
		TOOLBARINFO* pInfo = new TOOLBARINFO;
		m_arrBarInfo.Add(pInfo);
		pInfo->LoadState(lpszProfileName, i, this);
	}
}

void CXTPDockState::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPropExchangeSection secDockState(pPX->GetSection(_T("DockState")));
	// read/write version info
	if (pPX->IsStoring())
	{
		PX_DWord(&secDockState, _T("Version"), m_dwVersion, 0);

		CSize sz = GetScreenSize();
		PX_Size(&secDockState, _T("ScreenSize"), sz, CSize(0, 0));

		WORD nCount = (WORD)m_arrBarInfo.GetSize();
		PX_UShort(&secDockState, _T("Count"), nCount, 0);


		for (int i = 0; i < m_arrBarInfo.GetSize(); i++)
		{
			CString strSection;
			strSection.Format(_T("BarInfo%i"), i);

			CXTPPropExchangeSection secBarInfo(secDockState->GetSection(strSection));
			m_arrBarInfo[i]->DoPropExchange(&secBarInfo, this);
		}
	}
	else
	{
		Clear(); //empty out dockstate

		PX_DWord(&secDockState, _T("Version"), m_dwVersion, 0);

		CSize size;
		PX_Size(&secDockState, _T("ScreenSize"), size, CSize(0, 0));
		SetScreenSize(size);

		WORD nOldSize = 0;
		PX_UShort(&secDockState, _T("Count"), nOldSize, 0);

		m_arrBarInfo.SetSize(nOldSize);
		for (int i = 0; i < nOldSize; i++)
		{
			m_arrBarInfo[i] = new TOOLBARINFO;

			CString strSection;
			strSection.Format(_T("BarInfo%i"), i);

			CXTPPropExchangeSection secBarInfo(secDockState->GetSection(strSection));

			m_arrBarInfo[i]->DoPropExchange(&secBarInfo, this);
		}
		m_dwVersion = _XTP_SCHEMA_CURRENT;
	}
}

CSize CXTPDockState::GetScreenSize()
{
	return m_rectDevice.Size();
}
void CXTPDockState::SetScreenSize(CSize& size)
{
	m_sizeLogical = size;
	m_bScaling = (size != m_rectDevice.Size());
}

void CXTPToolBar::GetBarInfo(CToolBarInfo* pInfo)
{
// get state info
	pInfo->m_nBarID = m_nBarID;
	pInfo->m_pBar = this;
	pInfo->m_bVisible = IsVisible(); // handles delayed showing and hiding

	pInfo->m_nMRUWidth = m_nMRUWidth;

	CXTPWindowRect rect(this);
	if (m_pDockBar != NULL) m_pDockBar->ScreenToClient(&rect);

	pInfo->m_pointPos = m_hWnd? rect.TopLeft(): m_pDockContext->m_ptMRUFloatPos;

	pInfo->m_bFloating = m_barPosition == xtpBarFloating;

	pInfo->m_uMRUDockPosition = m_pDockContext->m_uMRUDockPosition;
	pInfo->m_rectMRUDockPos = m_pDockContext->m_rectMRUDockPos;
	pInfo->m_ptMRUFloatPos = m_pDockContext->m_ptMRUFloatPos;

	pInfo->m_bDialogBar = IsDialogBar();
	if (pInfo->m_bDialogBar)
	{
		pInfo->m_szDialogBarDockingClient = ((CXTPDialogBar*)this)->m_szDockingClient;
		pInfo->m_szDialogBarFloatingClient = ((CXTPDialogBar*)this)->m_szFloatingClient;
	}
}

void CXTPToolBar::SetBarInfo(CToolBarInfo* pInfo)
{
	UINT nFlags = (pInfo->m_bFloating? 0: SWP_NOSIZE)|SWP_NOACTIVATE|SWP_NOZORDER;

	if (!pInfo->m_bFloating && GetDockBar() == NULL)
		nFlags |= SWP_NOMOVE;

	m_nMRUWidth = pInfo->m_nMRUWidth;

	if (pInfo->m_bDialogBar && IsDialogBar())
	{
		((CXTPDialogBar*)this)->m_szDockingClient = pInfo->m_szDialogBarDockingClient;
		((CXTPDialogBar*)this)->m_szFloatingClient = pInfo->m_szDialogBarFloatingClient;
	}

	ASSERT(m_pDockContext != NULL);
	// You need to call EnableDocking before calling LoadBarState
	m_pDockContext->m_uMRUDockPosition = pInfo->m_uMRUDockPosition;
	m_pDockContext->m_rectMRUDockPos = pInfo->m_rectMRUDockPos;
	m_pDockContext->m_ptMRUFloatPos = pInfo->m_ptMRUFloatPos;

	// move and show/hide the window
	SetWindowPos(NULL, pInfo->m_pointPos.x, pInfo->m_pointPos.y, 0, 0,
		nFlags | (pInfo->m_bVisible ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));

	m_bVisible = pInfo->m_bVisible;

	if (pInfo->m_bFloating) OnRecalcLayout();
}

BOOL CXTPCommandBars::GetControlsFileName(CString& strFileName, LPCTSTR lpszProfileName)
{
	if (!m_strControlsFileName.IsEmpty())
	{
		strFileName = m_strControlsFileName;
		return TRUE;
	}


	TCHAR szBuff[_MAX_PATH];


	VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
	strFileName.Format(_T("%s-%s"), szBuff, lpszProfileName);

	return TRUE;
}

void CXTPCommandBars::SaveBarState(LPCTSTR lpszProfileName, BOOL bOnlyCustomized /*= TRUE*/)
{
	CXTPDockState state;
	GetDockState(state);
	state.SaveState(lpszProfileName);
	SaveDockBarsState(lpszProfileName);

	XTP_COMMANDBARS_PROPEXCHANGE_PARAM param;
	param.pCommandBars = this;
	param.bSaveOnlyCustomized = bOnlyCustomized;

	if (m_bCustomizeAvail)
	{
		CXTPCommandBarList lstCommandBars;
		GenerateCommandBarList(&lstCommandBars, &param);

		CMemFile memFile;
		CArchive ar (&memFile,CArchive::store);

		TCHAR szSection[256];
		wsprintf(szSection, _afxCommandBarControlsSection, lpszProfileName);

		CXTPPropExchangeArchive px(ar);

		px.m_dwData = (DWORD_PTR)&param;

		if (SaveCommandBarList(&px, &lstCommandBars))
		{
			ar.Flush();

			DWORD dwCount = (DWORD)memFile.GetPosition();
			BYTE* pControls = memFile.Detach();

			CString strFileName;
			BOOL bSaveToFile = GetControlsFileName(strFileName, lpszProfileName);

			if (bSaveToFile)
			{
				::DeleteFile((LPCTSTR)strFileName);
			}

			if (dwCount > 16000 && bSaveToFile)
			{
				try
				{
					CFile file;
					if (file.Open(strFileName, CFile::modeWrite|CFile::modeCreate))
					{
						file.Write(pControls, dwCount);
						AfxGetApp()->WriteProfileString(szSection, _afxCommandBarControls, strFileName );
						AfxGetApp()->WriteProfileInt(szSection, _afxCommandBarLoadFromFile, TRUE);
						file.Close();
						SetFileAttributes(strFileName, FILE_ATTRIBUTE_HIDDEN);
					}
				}
				catch (CFileException* pEx)
				{
					TRACE(_T("CXTPCommandBars::SaveBarState. File exception\r\n"));
					pEx->Delete ();
				}
			} else
			{
				AfxGetApp()->WriteProfileBinary(szSection, _afxCommandBarControls, pControls, dwCount);
				AfxGetApp()->WriteProfileInt(szSection, _afxCommandBarLoadFromFile, FALSE);
			}


			ar.Abort();
			memFile.Close();
			free(pControls);
		}
		else
		{
			AfxGetApp()->WriteProfileString(szSection, NULL, NULL);
		}
	}

	if (GetMenuBar() && GetMenuBar()->m_bDirtyMenu)
	{
		CFrameWnd* pFrame = DYNAMIC_DOWNCAST(CFrameWnd, GetSite());
		if (pFrame && pFrame->GetSafeHwnd()) pFrame->OnUpdateFrameMenu(0);
	}
	m_pOptions->bDirtyState = FALSE;
}

void CXTPCommandBars::_LoadControlsPart(CFile& file, XTP_COMMANDBARS_PROPEXCHANGE_PARAM* pParam)
{
	CArchive ar (&file,CArchive::load);

	CXTPPropExchangeArchive px(ar);
	px.m_dwData = (DWORD_PTR)pParam;

	CXTPCommandBarList lstCommandBars;
	if (LoadCommandBarList(&px, &lstCommandBars))
	{
		RestoreCommandBarList(&lstCommandBars, pParam->bLoadSilent);
		m_pMenuBar = DYNAMIC_DOWNCAST(CXTPMenuBar, GetToolBar(XTP_IDR_MENUBAR));
	}

	ar.Close();
}

void CXTPCommandBars::LoadBarState(LPCTSTR lpszProfileName, BOOL bSilent)
{

	XTP_COMMANDBARS_PROPEXCHANGE_PARAM paramT;
	paramT.pCommandBars = this;
	paramT.bLoadSilent = bSilent;

	if (m_bCustomizeAvail)
	{
		TCHAR szSection[256];
		wsprintf(szSection, _afxCommandBarControlsSection, lpszProfileName);

		if (AfxGetApp()->GetProfileInt(szSection, _afxCommandBarLoadFromFile, FALSE))
		{
			CString strFileName = AfxGetApp()->GetProfileString(szSection, _afxCommandBarControls);
			CFile file;
			if (file.Open(strFileName, CFile::modeRead))
			{
				_LoadControlsPart(file, &paramT);
			}

		} else
		{
			BYTE* pControls = NULL;
			UINT dwCount = 0;
			if (AfxGetApp()->GetProfileBinary(szSection, _afxCommandBarControls, &pControls, &dwCount))
			{
				CMemFile memFile(pControls, dwCount);

				_LoadControlsPart(memFile, &paramT);

				memFile.Close();
				delete [] pControls;
			}
		}
	}


	CXTPDockState state;
	state.LoadState(lpszProfileName);

	LoadDockBarsState(lpszProfileName);
	SetDockState(state);

	RecalcFrameLayout(TRUE);
	m_pOptions->bDirtyState = FALSE;

}

void CXTPCommandBars::SaveDockBarsState(LPCTSTR lpszProfileName)
{
	CWinApp* pApp = AfxGetApp();
	TCHAR szSection[256];

	for (int nIndex = 0; nIndex < 4; nIndex++)
	{
		wsprintf(szSection, _afxDockBarSection, lpszProfileName, nIndex);
		pApp->WriteProfileString(szSection, NULL, NULL);

		CXTPDockBar* pDock = m_pDocks[nIndex];
		int nSize = (int)pDock->m_arrBars.GetSize();
		if (nSize > 1)
		{
			int nCount = 0;
			DWORD dwIDPrev = (DWORD)-1;
			for (int i = 0; i < nSize; i++)
			{
				CXTPToolBar* pBar = (CXTPToolBar*)pDock->m_arrBars[i];
				DWORD dwID = pBar? pBar->m_nBarID: 0;
				if (dwIDPrev == 0 &&  dwID == 0)
					continue;
				dwIDPrev = dwID;

				TCHAR buf[16];
				wsprintf(buf, _afxBar, nCount++);
				pApp->WriteProfileInt(szSection, buf, (int)dwID);
			}
			pApp->WriteProfileInt(szSection, _afxBars, nCount);
		}
	}


	CControlBar* pBar = m_pFrame && m_pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd))? ((CFrameWnd*)m_pFrame)->GetControlBar(AFX_IDW_STATUS_BAR): 0;

	if (pBar != NULL && pBar->GetSafeHwnd() && !pBar->IsVisible())
	{
		pApp->WriteProfileInt(lpszProfileName, _T("StatusBar"), FALSE);
	} else
	{
		pApp->WriteProfileString(lpszProfileName, _T("StatusBar"), NULL);
	}
}

void CXTPCommandBars::LoadDockBarsState(LPCTSTR lpszProfileName)
{
	CWinApp* pApp = AfxGetApp();
	TCHAR szSection[256];
	for (int nIndex = 0; nIndex < 4; nIndex++)
	{
		wsprintf(szSection, _afxDockBarSection, lpszProfileName, nIndex);

		CXTPDockBar* pDock = m_pDocks[nIndex];

		int nBars  = pApp->GetProfileInt(szSection, _afxBars, 0);
		if (nBars  > 1)
		{
			for (int i = 0; i < nBars; i++)
			{
				TCHAR buf[16];
				wsprintf(buf, _afxBar, i);
				DWORD dwID = pApp->GetProfileInt(szSection, buf, 0);

				CXTPToolBar* pBar = GetToolBar(dwID);
				if (pBar)
				{
					if (pBar->m_hWnd && pBar->GetParent() != pDock)
						pBar->SetParent(pDock);
					if (pBar->m_pDockBar != NULL)
						pBar->m_pDockBar->RemoveCommandBar(pBar);
					pBar->m_pDockBar = pDock;
					pBar->m_barPosition = pDock->GetPosition();
				}
				pDock->m_arrBars.InsertAt(i, pBar);

			}
		}
	}

	CControlBar* pBar = m_pFrame && m_pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd))? ((CFrameWnd*)m_pFrame)->GetControlBar(AFX_IDW_STATUS_BAR): 0;
	if (pBar != NULL && !pApp->GetProfileInt(lpszProfileName, _T("StatusBar"), TRUE))
	{
		((CFrameWnd*)m_pFrame)->ShowControlBar(pBar, FALSE, FALSE);
	}
}

void CXTPCommandBars::DoPropExchange(CXTPPropExchange* pPX, XTP_COMMANDBARS_PROPEXCHANGE_PARAM* pParam)
{
	XTP_COMMANDBARS_PROPEXCHANGE_PARAM paramT;
	if (pParam == NULL)
		pParam = &paramT;

	pParam->pCommandBars = this;

	pPX->m_dwData = (DWORD_PTR)pParam;

	CXTPDockState state;

	if (pPX->IsStoring())
	{
		if (pParam->bSerializeControls)
		{
			CXTPCommandBarList lstCommandBars;
			GenerateCommandBarList(&lstCommandBars, pParam);

			VERIFY(SaveCommandBarList(pPX, &lstCommandBars));
		}

		if (pParam->bSerializeLayout)
		{
			GetDockState(state);
			CXTPPropExchangeSection secLayout(pPX->GetSection(_T("Layout")));
			state.DoPropExchange(&secLayout);
			SerializeDockBarsState(&secLayout);
		}

		if (GetMenuBar() && GetMenuBar()->m_bDirtyMenu)
		{
			CFrameWnd* pFrame = DYNAMIC_DOWNCAST(CFrameWnd, GetSite());
			if (pFrame && pFrame->GetSafeHwnd()) pFrame->OnUpdateFrameMenu(0);
		}
	}
	else
	{
		if (pParam->bSerializeControls)
		{
			CXTPCommandBarList lstCommandBars;
			if (LoadCommandBarList(pPX, &lstCommandBars))
			{
				RestoreCommandBarList(&lstCommandBars, pParam->bLoadSilent);
				m_pMenuBar = DYNAMIC_DOWNCAST(CXTPMenuBar, GetToolBar(XTP_IDR_MENUBAR));
			}
			else
			{
				return;
			}
		}

		if (pParam->bSerializeLayout)
		{
			CXTPPropExchangeSection secLayout(pPX->GetSection(_T("Layout")));
			state.DoPropExchange(&secLayout);
			SerializeDockBarsState(&secLayout);
			SetDockState(state);
		}

		if (pParam->bSerializeDesignerControls)
		{
			CXTPToolBar* pCommandBar = GetToolBar(0xFFEEFF);

			if (pCommandBar)
			{
				if (m_pDesignerControls)
					m_pDesignerControls->InternalRelease();

				m_pDesignerControls = pCommandBar->GetControls();
				m_pDesignerControls->InternalAddRef();

				Remove(pCommandBar);

				m_pDesignerControls->SetParent(NULL);
				m_pDesignerControls->Remove(m_pDesignerControls->FindControl(xtpControlError, XTP_ID_TOOLBAR_EXPAND, TRUE, FALSE));
				m_pDesignerControls->Remove(m_pDesignerControls->FindControl(xtpControlError, XTP_ID_TOOLBAR_HIDE, TRUE, FALSE));
				m_pDesignerControls->SetCommandBars(this);
			}
		}

		RecalcFrameLayout(TRUE);
	}

	if (pParam->bSerializeImages)
	{
		CXTPPropExchangeSection secImages(pPX->GetSection(_T("Images")));
		GetImageManager()->DoPropExchange(&secImages);
	}

	CFrameWnd* pFrame = DYNAMIC_DOWNCAST(CFrameWnd, m_pFrame);

	if (pParam->bSerializeOptions)
	{
		CXTPPropExchangeSection secShortcuts(pPX->GetSection(_T("Shortcuts")));
		XTPShortcutManager()->DoPropExchange(&secShortcuts, pFrame);

		CXTPPropExchangeSection secOptions(pPX->GetSection(_T("Options")));
		
		XTPPaintTheme theme = GetPaintManager()->BaseTheme();
		PX_Enum(&secOptions, _T("Theme"), theme, xtpThemeOffice2000);
		if (pPX->IsLoading()) SetPaintManager(CXTPPaintManager::CreateTheme(theme));
		
		m_pOptions->DoPropExchange(&secOptions);
		PX_Bool(&secOptions, _T("Customizable"), m_bCustomizeAvail, TRUE);


	}

	if (pParam->bSerializeDesignerControls && pPX->IsLoading())
	{
		m_hDesignerAccels = XTPShortcutManager()->CopyAccelTable(pFrame->m_hAccelTable);

		if (m_bCustomizeAvail)
		{
			for (int i = 0; i < GetCount(); i++)
			{
				CXTPToolBar* pToolBar = GetAt(i);
				pToolBar->GetControls()->CreateOriginalControls();
			}
		}
	}


	m_pOptions->bDirtyState = FALSE;

}

void CXTPCommandBars::SerializeBarState(CArchive& ar, BOOL bSerializeControls, BOOL bOnlyCustomized, BOOL bSilent)
{
	CXTPPropExchangeArchive px(ar);

	XTP_COMMANDBARS_PROPEXCHANGE_PARAM paramT;
	paramT.bSaveOnlyCustomized = bOnlyCustomized;
	paramT.bSerializeControls = bSerializeControls;
	paramT.bLoadSilent = bSilent;

	DoPropExchange(&px, &paramT);
}


void CXTPCommandBars::SerializeDockBarsState(CXTPPropExchange* pPX)
{
	CXTPPropExchangeSection secDockBars(pPX->GetSection(_T("DockBars")));

	for (int nIndex = 0; nIndex < 4; nIndex++)
	{
		CXTPDockBar* pDock = m_pDocks[nIndex];

		CString strSection;
		strSection.Format(_T("DockBar%i"), nIndex);
		CXTPPropExchangeSection secDockBar(secDockBars->GetSection(strSection));

		if (pPX->IsStoring())
		{

			int nSize = (int)pDock->m_arrBars.GetSize();
			DWORD nCount = 0;
			DWORD dwIDPrev = (DWORD)-1;

			for (int i = 0; i < nSize; i++)
			{
				CXTPToolBar* pBar = (CXTPToolBar*)pDock->m_arrBars[i];
				DWORD dwID = pBar? pBar->m_nBarID: 0;
				if (dwIDPrev == 0 &&  dwID == 0) continue;
				dwIDPrev = dwID;
				nCount++;
			}
			if (nCount > 1)
			{
				PX_DWord(&secDockBar, _T("Count"), nCount, 0);
				int j = 0;

				dwIDPrev = (DWORD)-1;
				for (int i = 0; i < nSize; i++)
				{
					CXTPToolBar* pBar = (CXTPToolBar*)pDock->m_arrBars[i];
					DWORD dwID = pBar? pBar->m_nBarID: 0;
					if (dwIDPrev == 0 &&  dwID == 0)
						continue;
					dwIDPrev = dwID;

					CString strSection;
					strSection.Format(_T("Id%i"), j++);
					PX_DWord(&secDockBar, strSection, dwID, 0);
				}
			} else
			{
				nCount = 0;
				PX_DWord(&secDockBar, _T("Count"), nCount, 0);
			}
		}
		else
		{
			DWORD dwID = 0;
			DWORD nBars = 0;
			PX_DWord(&secDockBar, _T("Count"), nBars, 0);

			if (nBars  > 1)
			{
				for (int i = 0; i < (int)nBars; i++)
				{
					CString strSection;
					strSection.Format(_T("Id%i"), i);
					PX_DWord(&secDockBar, strSection, dwID, 0);

					CXTPToolBar* pBar = GetToolBar(dwID);

					if (pBar)
					{
						if (pBar->m_hWnd && pBar->GetParent() != pDock)
							pBar->SetParent(pDock);
						if (pBar->m_pDockBar != NULL)
							pBar->m_pDockBar->RemoveCommandBar(pBar);
						pBar->m_pDockBar = pDock;
						pBar->m_barPosition = pDock->GetPosition();
					}
					pDock->m_arrBars.InsertAt(i, pBar);
				}
			}
		}
	}

}

void CXTPCommandBars::GetDockState(CXTPDockState& state)
{
	state.Clear(); //make sure dockstate is empty
	// get state info for each bar
	POSITION pos = m_lstBars.GetHeadPosition();
	while (pos != NULL)
	{
		CXTPToolBar* pBar = m_lstBars.GetNext(pos);
		ASSERT(pBar != NULL);
		CXTPToolBar::CToolBarInfo* pInfo = new CXTPToolBar::CToolBarInfo;
		pBar->GetBarInfo(pInfo);

		state.m_arrBarInfo.Add(pInfo);
	}
}

void CXTPCommandBars::SetDockState(CXTPDockState& state)
{
	for (int i = 0; i < state.m_arrBarInfo.GetSize(); i++)
	{
		CXTPToolBar::CToolBarInfo* pInfo = state.m_arrBarInfo[i];
		ASSERT(pInfo != NULL);

		pInfo->m_pBar = GetToolBar(pInfo->m_nBarID);

		if (pInfo->m_pBar != NULL)
		{
			if (pInfo->m_bFloating)
			{
				FloatCommandBar(pInfo->m_pBar);
			} else
			{
				VERIFY(pInfo->m_pBar->Create(m_pFrame, FALSE));
			}

			pInfo->m_pBar->SetBarInfo(pInfo);
		}
	}
}


void CXTPCommandBars::GenerateCommandBarList(CXTPCommandBarList* pCommandBarList, XTP_COMMANDBARS_PROPEXCHANGE_PARAM* pParam)
{
	POSITION pos = m_lstBars.GetHeadPosition();

	DWORD nID = 0x1000000;
	while (pos != NULL)
	{
		CXTPToolBar* pBar = m_lstBars.GetNext(pos);

		if (pParam->bSaveOnlyCustomized && pBar->IsBuiltIn() && !pBar->IsTemporary())
		{
			if (!pBar->IsCustomizable())
				continue;

			BOOL bCustomizedMenuBar = FALSE;
			if (pBar->GetType() == xtpBarTypeMenuBar)
			{
				((CXTPMenuBar*)pBar)->CleanupMDIMenus();
				bCustomizedMenuBar = ((CXTPMenuBar*)pBar)->m_mapMDIMenus.GetCount() > 0;
			}

			if (!bCustomizedMenuBar && (pBar->GetControls()->GetOriginalControls() && !pBar->GetControls()->IsChanged()))
				continue;
		}


		pBar->GenerateCommandBarList(nID, pCommandBarList, pParam);
	}
}

void CXTPCommandBar::GenerateCommandBarList(DWORD& nID, CXTPCommandBarList* pCommandBarList, XTP_COMMANDBARS_PROPEXCHANGE_PARAM* pParam)
{
	if (pCommandBarList->Find(this) == NULL)
	{
		pCommandBarList->AddTail(this);
		InternalAddRef();


		if (!IsKindOf(RUNTIME_CLASS(CXTPToolBar))) m_nBarID = nID++;
		else ASSERT(m_nBarID != 0);

		m_pControls->GenerateCommandBarList(nID, pCommandBarList, pParam);
	}
}

void CXTPMenuBar::GenerateCommandBarList(DWORD& nID, CXTPCommandBarList* pCommandBarList, XTP_COMMANDBARS_PROPEXCHANGE_PARAM* pParam)
{
	if (pCommandBarList->Find(this) == NULL)
	{
		CXTPToolBar::GenerateCommandBarList(nID, pCommandBarList, pParam);

		POSITION pos = m_mapMDIMenus.GetStartPosition();
		while (pos)
		{
			UINT nIDCommand;
			CXTPControls* pControls;
			m_mapMDIMenus.GetNextAssoc(pos, nIDCommand, pControls);
			pControls->GenerateCommandBarList(nID, pCommandBarList, pParam);
		}
	}
}

void CXTPControls::GenerateCommandBarList(DWORD& nID, CXTPCommandBarList* pCommandBarList, XTP_COMMANDBARS_PROPEXCHANGE_PARAM* pParam)
{
	for (CXTPControl* pControl = GetFirst(); pControl; GetNext(pControl))
	{
		if (pControl && !pControl->m_bTemporary)
		{
			CXTPCommandBar* pCommandBar = pControl->GetCommandBar();
			if (pCommandBar)
			{
				pCommandBar->GenerateCommandBarList(nID, pCommandBarList, pParam);
			}
		}
	}

	if (m_pOriginalControls && pParam->bSaveOriginalControls)
		m_pOriginalControls->GenerateCommandBarList(nID, pCommandBarList, pParam);
}

BOOL CXTPCommandBars::SaveCommandBarList(CXTPPropExchange* pPX, CXTPCommandBarList* pCommandBarList)
{
	try
	{
		CXTPPropExchangeSection secCommandBars(pPX->GetSection(_T("CommandBars")));

		long nCount = (long)pCommandBarList->GetCount();
		
		CXTPPropExchangeEnumeratorPtr pEnumerator(secCommandBars->GetEnumerator(_T("CommandBar")));
		POSITION posEnum = pEnumerator->GetPosition(nCount, FALSE);

		POSITION pos = pCommandBarList->GetHeadPosition();
		while (pos)
		{
			CXTPPropExchangeSection secItem(pEnumerator->GetNext(posEnum));
			PX_Object(&secItem, pCommandBarList->GetNext(pos), RUNTIME_CLASS(CXTPCommandBar));
		}


	}
	catch (COleException* pEx)
	{
		TRACE(_T("CXTPCommandBars::SaveCommandBarList. OLE exception: %x\r\n"),
			pEx->m_sc);
		pEx->Delete ();
		return FALSE;
	}
	catch (CArchiveException* pEx)
	{
		TRACE(_T("CXTPCommandBars::SaveCommandBarList. Archive exception\r\n"));
		pEx->Delete ();
		return FALSE;
	}
	return TRUE;

}


BOOL CXTPCommandBars::LoadCommandBarList(CXTPPropExchange* pPX, CXTPCommandBarList* pCommandBarList)
{
	try
	{
		CXTPPropExchangeSection secCommandBars(pPX->GetSection(_T("CommandBars")));

		CXTPPropExchangeEnumeratorPtr pEnumerator(secCommandBars->GetEnumerator(_T("CommandBar")));
		POSITION posEnum = pEnumerator->GetPosition(0, FALSE);

		while(posEnum)
		{
			CXTPPropExchangeSection secItem(pEnumerator->GetNext(posEnum));

			CXTPCommandBar* pCommandBar = NULL;
			PX_Object(&secItem, pCommandBar, RUNTIME_CLASS(CXTPCommandBar));

			if (!pCommandBar)
			{
				return FALSE;
			}

			pCommandBar->GetControls()->RefreshIndexes();

			pCommandBarList->AddTail(pCommandBar);
		}




	}
	catch (COleException* pEx)
	{
		TRACE(_T("CXTPCommandBars::LoadCommandBarList. OLE exception: %x\r\n"),
			pEx->m_sc);
		pEx->Delete ();
		return FALSE;
	}
	catch (CArchiveException* pEx)
	{
		TRACE(_T("CXTPCommandBars::LoadCommandBarList. Archive exception\r\n"));
		pEx->Delete ();
		return FALSE;
	}
	return TRUE;

}

void CXTPCommandBars::RestoreCommandBarList(CXTPCommandBarList* pCommandBarList, BOOL bSilent)
{
	POSITION pos = pCommandBarList->GetHeadPosition();

	while (pos)
	{
		CXTPCommandBar* pCommandBar = pCommandBarList->GetNext(pos);
		CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, pCommandBar);

		if (pToolBar)
		{
			CXTPToolBar* pExistsBar = GetToolBar(pToolBar->m_nBarID);

			pCommandBar->RestoreCommandBarList(pCommandBarList);

			if (pExistsBar)
			{
				CXTPControls* pLastControls = pCommandBar->GetControls()->GetOriginalControls();

				if (pLastControls && pExistsBar->GetControls()->GetOriginalControls()
					&& pExistsBar->GetControls()->GetOriginalControls()->Compare(pLastControls)  == FALSE)
				{
					//silent restore if not customized
					if (!bSilent && pLastControls->Compare(pCommandBar->GetControls()) == FALSE)
					{
						CString strMessage, strUpdate;
						VERIFY(XTPResourceManager()->LoadString(&strUpdate, XTP_IDS_CONFIRM_UPDATE));
						strMessage.Format(strUpdate, pExistsBar->m_strTitle);

						if (AfxMessageBox(strMessage, MB_YESNO | MB_ICONQUESTION) == IDNO)
						{
							CXTPControls* pControls = pExistsBar->GetControls()->GetOriginalControls();
							CMDTARGET_ADDREF(pControls);

							pExistsBar->Copy(pCommandBar, FALSE);

							CMDTARGET_RELEASE(pExistsBar->m_pControls->m_pOriginalControls);
							pExistsBar->m_pControls->m_pOriginalControls = pControls;


						}
					}

				} else
				{
					CXTPControls* pOriginalControls = pCommandBar->m_pControls->m_pOriginalControls? NULL:
						pExistsBar->m_pControls->m_pOriginalControls;
					CMDTARGET_ADDREF(pOriginalControls);

					pExistsBar->Copy(pCommandBar, FALSE);

					if (pOriginalControls)
					{
						ASSERT(pExistsBar->m_pControls->m_pOriginalControls == NULL);

						pExistsBar->m_pControls->m_pOriginalControls = pOriginalControls;
					}
				}

			}
			else
			{
				m_lstBars.AddTail(pToolBar);
				pToolBar->InternalAddRef();
				pToolBar->Create(m_pFrame, FALSE);
				pToolBar->EnableDocking(pToolBar->m_dwFlags);

				OnToolBarAdded(pToolBar);
			}

		}

	}
}


void CXTPCommandBar::RestoreCommandBarList(CXTPCommandBarList* pCommandBarList)
{
	m_pControls->RestoreCommandBarList(pCommandBarList);
}

void CXTPMenuBar::RestoreCommandBarList(CXTPCommandBarList* pCommandBarList)
{
	CXTPToolBar::RestoreCommandBarList(pCommandBarList);

	POSITION pos = m_mapMDIMenus.GetStartPosition();
	while (pos)
	{
		UINT nIDCommand;
		CXTPControls* pControls;
		m_mapMDIMenus.GetNextAssoc(pos, nIDCommand, pControls);

		pControls->RestoreCommandBarList(pCommandBarList);
	}
}


void CXTPControls::RestoreCommandBarList(CXTPCommandBarList* pCommandBarList)
{
	for (CXTPControl* pControl = GetFirst(); pControl; GetNext(pControl))
	{
		CXTPControlPopup* pControlPopup = DYNAMIC_DOWNCAST(CXTPControlPopup, pControl);

		if (pControlPopup)
		{
			if (pControlPopup->m_dwCommandBarID != 0)
			{
				CXTPCommandBar* pCommandBar = pCommandBarList->FindCommandBar(pControlPopup->m_dwCommandBarID);
				ASSERT(pCommandBar);

				if (pCommandBar)
				{
					pControlPopup->SetCommandBar(pCommandBar);
					pControlPopup->m_dwCommandBarID = 0;
					pCommandBar->RestoreCommandBarList(pCommandBarList);
				}
			}
		}
	}

	if (m_pOriginalControls)
		m_pOriginalControls->RestoreCommandBarList(pCommandBarList);
}

void CXTPCommandBar::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Enum(pPX, _T("Type"), m_barType, xtpBarTypeNormal);
	PX_Enum(pPX, _T("Position"), m_barPosition, xtpBarTop);
	PX_Long(pPX, _T("BarID"), (long&)m_nBarID, 0);
	PX_Long(pPX, _T("Flags"), (long&)m_dwFlags, 0);
	PX_Long(pPX, _T("Style"), (long&)m_dwStyle, 0);
	PX_String(pPX, _T("Title"), m_strTitle, _T(""));
	PX_Bool(pPX, _T("Customizable"), m_bCustomizable, TRUE);
	PX_Int(pPX, _T("MRUWidth"), m_nMRUWidth, 0);

	if (pPX->GetSchema() > _XTP_SCHEMA_80)
		PX_Size(pPX, _T("IconSize"), m_szIcons, CSize(0, 0));

	if (pPX->IsLoading())
	{
		ASSERT(pPX->m_dwData != 0);
		SetCommandBars(((XTP_COMMANDBARS_PROPEXCHANGE_PARAM*)pPX->m_dwData)->pCommandBars);
	}

	CXTPPropExchangeSection secControls(pPX->GetSection(_T("Controls")));
	m_pControls->DoPropExchange(&secControls);
}


void CXTPToolBar::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPCommandBar::DoPropExchange(pPX);

	PX_Bool(pPX, _T("BuiltIn"), m_bBuiltIn, TRUE);
	PX_Bool(pPX, _T("TearOff"), m_bTearOff, FALSE);

	if (pPX->GetSchema() > _XTP_SCHEMA_87)
		PX_Bool(pPX, _T("Closeable"), m_bCloseable, TRUE);

	if (pPX->GetSchema() > _XTP_SCHEMA_875)
		PX_Bool(pPX, _T("ContextMenuPresent"), m_bContextMenuPresent, TRUE);
}


void CXTPMenuBar::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsStoring())
	{
		if (m_mapMDIMenus.GetCount() > 0)
		{
			SwitchMDIMenu(m_hMenuDefault);
			m_bDirtyMenu = TRUE;
		}
	}

	CXTPToolBar::DoPropExchange(pPX);
	PX_Long(pPX, _T("ResourceId"), (long&)m_nIDResource, 0);

	if (pPX->IsStoring())
	{
		//_XTP_SCHEMA_861

		int i = 0;
		pPX->WriteCount((DWORD)m_mapMDIMenus.GetCount());
		POSITION pos = m_mapMDIMenus.GetStartPosition();
		while (pos)
		{
			UINT nIDCommand;
			CXTPControls* pControls;
			m_mapMDIMenus.GetNextAssoc(pos, nIDCommand, pControls);

			CString strSection;
			strSection.Format(_T("%s%i"), _T("MDIMenu"), i++);

			CXTPPropExchangeSection sec(pPX->GetSection(strSection));
			PX_Long(&sec, _T("Id"), (long&)nIDCommand, 0);
			pControls->DoPropExchange(&sec);
		}
	}
	else
	{
		if (pPX->GetSchema() > _XTP_SCHEMA_86)
		{
			DWORD nCount = (DWORD)pPX->ReadCount();

			for (DWORD i = 0; i < nCount; i++)
			{
				UINT nIDCommand = 0;

				CString strSection;
				strSection.Format(_T("%s%i"), _T("MDIMenu"), i);

				CXTPPropExchangeSection sec(pPX->GetSection(strSection));
				PX_Long(&sec, _T("Id"), (long&)nIDCommand, 0);

				CXTPControls* pControls = new CXTPControls;
				pControls->DoPropExchange(&sec);

				m_mapMDIMenus.SetAt(nIDCommand, pControls);
			}
		}
	}

}


void CXTPPopupBar::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPCommandBar::DoPropExchange(pPX);

	PX_Bool(pPX, _T("TearOffPopup"), m_bTearOffPopup, FALSE);
	PX_String(pPX, _T("TearOffCaption"), m_strTearOffCaption, _T(""));
	PX_Long(pPX, _T("TearOffId"), (long&)m_nTearOffID, 0);
	PX_Int(pPX, _T("TearOffWidth"), m_nTearOffWidth, 0);

	if (pPX->GetSchema() > _XTP_SCHEMA_85)
	{
		PX_Rect(pPX, _T("Borders"), m_rcBorders, CRect(2, 4, 2, 4));
		PX_Bool(pPX, _T("ShowShadow"), m_bShowShadow, TRUE);
	}

}

void CXTPControls::DoPropExchange(CXTPPropExchange* pPX)
{
	ASSERT(pPX->m_dwData != 0);
	XTP_COMMANDBARS_PROPEXCHANGE_PARAM* pParam = (XTP_COMMANDBARS_PROPEXCHANGE_PARAM*)pPX->m_dwData;


	if (pPX->IsStoring())
	{
		long nCount = 0;
		int i;
		for (i = 0; i < m_arrControls.GetSize(); i++)
		{
			CXTPControl* pControl = GetAt(i);
			if (!pControl->m_bTemporary) nCount++;
		}

		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Control")));
		POSITION posEnum = pEnumerator->GetPosition(nCount, FALSE);

		for (i = 0; i < (int)m_arrControls.GetSize(); i++)
		{
			CXTPControl* pControl = GetAt(i);
			if (!pControl->m_bTemporary)
			{
				CXTPPropExchangeSection secItem(pEnumerator->GetNext(posEnum));
				PX_Object(&secItem, pControl, RUNTIME_CLASS(CXTPControl));
			}
		}

		BOOL bOriginalControls = m_pOriginalControls && pParam->bSaveOriginalControls? TRUE: FALSE;
		PX_Bool(pPX, _T("OriginalControls"), bOriginalControls, FALSE);

		if (bOriginalControls)
		{
			CXTPPropExchangeSection secControls(pPX->GetSection(_T("OriginalControls")));
			m_pOriginalControls->DoPropExchange(&secControls);
		}

		BOOL bCustomizedControls = FALSE;
		PX_Bool(pPX, _T("CustomizedControls"), bCustomizedControls, FALSE);

	} else
	{
		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Control")));
		POSITION posEnum = pEnumerator->GetPosition(0, FALSE);

		m_arrControls.RemoveAll();

		while(posEnum)
		{
			CXTPPropExchangeSection secItem(pEnumerator->GetNext(posEnum));

			CXTPControl* pControl = NULL;
			PX_Object(&secItem, pControl, RUNTIME_CLASS(CXTPControl));

			if (!pControl)
				AfxThrowArchiveException(CArchiveException::badClass);

			m_arrControls.Add(pControl);
		}

		RefreshIndexes();
		if (pPX->GetSchema() > _XTP_SCHEMA_80)
		{
			BOOL bOriginalControls = FALSE;
			PX_Bool(pPX, _T("OriginalControls"), bOriginalControls, FALSE);

			if (bOriginalControls)
			{
				if (m_pOriginalControls) m_pOriginalControls->InternalRelease();
				m_pOriginalControls = new CXTPControls;

				CXTPPropExchangeSection secControls(pPX->GetSection(_T("OriginalControls")));
				m_pOriginalControls->DoPropExchange(&secControls);
			}

			BOOL bCustomizedControls = FALSE;
			PX_Bool(pPX, _T("CustomizedControls"), bCustomizedControls, FALSE);

			if (bCustomizedControls)
			{
				// _XTP_SCHEMA_80 compatibility.
				CXTPControls* pCustomizedControls = new CXTPControls;
				CXTPPropExchangeSection secControls(pPX->GetSection(_T("CustomizedControls")));
				pCustomizedControls->DoPropExchange(&secControls);
				pCustomizedControls->InternalRelease();
			}
		}
	}
}

void CXTPControl::DoPropExchange(CXTPPropExchange* pPX)
{
	ASSERT(sizeof(long) == sizeof(DWORD));

	PX_Enum(pPX, _T("Type"), m_controlType, xtpControlButton);
	PX_Int(pPX, _T("Id"), m_nId, -1);
	PX_Int(pPX, _T("IconId"), m_nIconId, 0);
	PX_Bool(pPX, _T("BeginGroup"), m_bBeginGroup, FALSE);
	PX_Long(pPX, _T("Tag"), (long&)m_nTag, 0);
	PX_Long(pPX, _T("Flags"), (long&)m_dwFlags, 0);
	PX_Long(pPX, _T("HideFlags"), (long&)m_dwHideFlags, 0);
	PX_String(pPX, _T("Caption"), m_strCaption, _T(""));
	PX_String(pPX, _T("ShortcutText"), m_strShortcutText, _T(""));
	PX_String(pPX, _T("TooltipText"), m_strTooltipText, _T(""));
	PX_String(pPX, _T("DescriptionText"), m_strDescriptionText, _T(""));
	PX_String(pPX, _T("Parameter"), m_strParameter, _T(""));
	PX_Int(pPX, _T("CustomIconId"), m_nCustomIconId, 0);
	PX_String(pPX, _T("CustomCaption"), m_strCustomCaption, _T(""));


	if (pPX->GetSchema() > _XTP_SCHEMA_71)
		PX_String(pPX, _T("Category"), m_strCategory, _T(""));

	if (m_nCustomIconId > 0)
	{
		CXTPCommandBars* pCommandBars = ((XTP_COMMANDBARS_PROPEXCHANGE_PARAM*)pPX->m_dwData)->pCommandBars;
		ASSERT(pCommandBars->IsKindOf(RUNTIME_CLASS(CXTPCommandBars)));

		CXTPPropExchangeSection secCustomIcon(pPX->GetSection(_T("CustomIcon")));
		pCommandBars->GetImageManager()->DoPropExchange(m_nCustomIconId, &secCustomIcon);
	}

	if (pPX->GetSchema() > _XTP_SCHEMA_85)
	{
		if (!(pPX->IsStoring() && pPX->IsAllowDefaultValues() && m_mapDocTemplatesExcluded.IsEmpty()))
			PX_Serialize(pPX, _T("DocTemplatesExcluded"), &m_mapDocTemplatesExcluded);

		if (!(pPX->IsStoring() && pPX->IsAllowDefaultValues() && m_mapDocTemplatesAssigned.IsEmpty()))
			PX_Serialize(pPX,  _T("DocTemplatesAssigned"), &m_mapDocTemplatesAssigned);

		PX_Bool(pPX,  _T("DefaultItem"), m_bDefaultItem, FALSE);
	}

	if (pPX->GetSchema() > _XTP_SCHEMA_87)
		PX_Bool(pPX,  _T("CloseSubMenuOnClick"), m_bCloseSubMenuOnClick, TRUE);

	if (pPX->GetSchema() > _XTP_SCHEMA_875)
		PX_Enum(pPX, _T("CustomStyle"), m_buttonCustomStyle, xtpButtonAutomatic);
}


void CXTPControlButton::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPControl::DoPropExchange(pPX);

	PX_Enum(pPX, _T("Style"), m_buttonStyle, xtpButtonAutomatic);
}

void CXTPControlPopup::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPControl::DoPropExchange(pPX);

	if (pPX->IsStoring())
	{
		long dwCommandBarID = m_pCommandBar? m_pCommandBar->m_nBarID: 0;
		PX_Long(pPX, _T("CommandBarId"), dwCommandBarID, 0);
	}
	else
	{
		PX_Long(pPX, _T("CommandBarId"), (long&)m_dwCommandBarID, 0);
	}

	if (pPX->GetSchema() > _XTP_SCHEMA_86)
		PX_Enum(pPX, _T("Style"), m_buttonStyle, xtpButtonAutomatic);
}

void CXTPControlComboBox::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPControlPopup::DoPropExchange(pPX);

	PX_Bool(pPX, _T("DropDown"), m_bDropDown, TRUE);
	PX_Int(pPX, _T("Width"), m_nWidth, 0);
	PX_Enum(pPX, _T("ComboStyle"), m_comboStyle, xtpComboNormal);

	if (pPX->IsLoading())
		SetDropDownListStyle(m_bDropDown);

}

void CXTPControlEdit::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPControl::DoPropExchange(pPX);

	if (pPX->GetSchema() > _XTP_SCHEMA_861)
	{
		PX_Bool(pPX, _T("HasLabel"), m_bLabel, FALSE);
		PX_Int(pPX, _T("Width"), m_nWidth, 0);
	}
	if (pPX->GetSchema() > _XTP_SCHEMA_875)
	{
		PX_Bool(pPX, _T("ReadOnly"), m_bReadOnly, FALSE);
	}
}
