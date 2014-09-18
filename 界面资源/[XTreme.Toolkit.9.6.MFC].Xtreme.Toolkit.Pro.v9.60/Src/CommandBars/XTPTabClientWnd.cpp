// XTPTabClientWnd.cpp : implementation of the CXTPTabClientWnd class.
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
#include "XTPResource.h"
#include "XTPCommandBarsResource.h"

#include "XTPTabManager.h"
#include "XTPCommandBarsDefines.h"
#include "XTPTabClientWnd.h"
#include "XTPMouseManager.h"
#include "XTPPaintManager.h"
#include "XTPPopupBar.h"
#include "XTPToolBar.h"
#include "XTPCommandBars.h"
#include "XTPFrameWnd.h"
#include "XTPControls.h"
#include "XTPControlExt.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef OIC_WINLOGO
#define OIC_WINLOGO         32517
#endif


#define ACTION_CANCEL       0
#define ACTION_POPUP        1
#define ACTION_ATTACH       2
#define ACTION_INSERTHORIZ  3
#define ACTION_INSERTVERT   4

class CXTPTabClientWnd::CTabClientDropTarget : public COleDropTarget
{
	virtual DROPEFFECT OnDragOver(CWnd* /*pWnd*/, COleDataObject* /*pDataObject*/, DWORD /*dwKeyState*/, CPoint point)
	{
		ASSERT(m_pTabClientWnd);
		CXTPTabManagerItem* pItem = m_pTabClientWnd->HitTest(point);

		if (pItem && !pItem->IsSelected())
		{
			m_pTabClientWnd->MDIActivate(CWnd::FromHandle(pItem->GetHandle()));
		}

		return DROPEFFECT_NONE;
	}
public:
	CXTPTabClientWnd* m_pTabClientWnd;
};

//////////////////////////////////////////////////////////////////////////
// CWorkspace

CXTPTabClientWnd::CWorkspace::CWorkspace()
{
	m_pTabClientWnd = NULL;
	m_hwndClient = NULL;
	m_dHeight = 100;
}

CXTPTabClientWnd::CWorkspace::~CWorkspace()
{

}

void CXTPTabClientWnd::CWorkspace::SetSelectedItem(CXTPTabManagerItem* pItem)
{
	if (m_pSelected != pItem)
	{
		m_hwndClient = pItem? pItem->GetHandle(): 0;
	}

	CXTPTabManager::SetSelectedItem(pItem);

}


BOOL CXTPTabClientWnd::CWorkspace::IsMouseLocked()
{
	return CXTPTabManager::IsMouseLocked() || XTPMouseManager()->IsTrackedLock();
}

void CXTPTabClientWnd::CWorkspace::RedrawControl()
{
	if (!m_pTabClientWnd->m_bLockReposition)
		m_pTabClientWnd->Invalidate(FALSE);
}

void CXTPTabClientWnd::CWorkspace::Reposition()
{
	if (m_pTabClientWnd->m_bLockReposition)
	{
		m_pTabClientWnd->m_bForceToRecalc = TRUE;
		return;
	}
	m_pTabClientWnd->m_bForceToRecalc = FALSE;

	m_pTabClientWnd->GetParentFrame()->RecalcLayout();
}
BOOL CXTPTabClientWnd::CWorkspace::IsAllowReorder()
{
	return m_pTabClientWnd->m_bAllowReorder;
}

void CXTPTabClientWnd::CWorkspace::SetAllowReorder(BOOL bAllowReorder)
{
	m_pTabClientWnd->m_bAllowReorder = bAllowReorder;
}
BOOL CXTPTabClientWnd::CWorkspace::IsDrawStaticFrame()
{
	return GetPaintManager()->m_bStaticFrame && !m_pTabClientWnd->m_bEnableGroups;
}




BOOL CXTPTabClientWnd::CWorkspace::DrawIcon(CDC* pDC, CPoint pt, CXTPTabManagerItem* pItem, BOOL bDraw, CSize& szIcon)
{
	if (GetPaintManager()->m_bShowIcons == FALSE)
		return FALSE;

	if (!bDraw)
	{
		return TRUE;
	}

	HICON hIcon = GetItemIcon(pItem);
	if (hIcon)
	{
		DrawIconEx(pDC->GetSafeHdc(), pt.x, pt.y, hIcon, szIcon.cx, szIcon.cy, 0, NULL, DI_NORMAL);
	}

	return TRUE;
}

CXTPTabPaintManager* CXTPTabClientWnd::CWorkspace::GetPaintManager()
{
	return m_pTabClientWnd->m_pPaintManager;
}


// finds the tab item for specified window. Returns NULL if not found
CXTPTabManagerItem* CXTPTabClientWnd::CWorkspace::FindItem(const HWND hWnd)
{
	// loop through all tab items
	for(int nIndex = 0; nIndex < GetItemCount(); nIndex++)
	{
		// check for window handle
		if(GetItem(nIndex)->GetHandle() == hWnd)
		{
			return GetItem(nIndex);
		}
	}

	return NULL;
}


// add new item into the tab control for specified MDIChild. If bRedraw is
// set to TRUE then framework will be redrawn in order to show new item.
CXTPTabManagerItem* CXTPTabClientWnd::CWorkspace::AddItem(const CWnd* pChildWnd)
{
	ASSERT(pChildWnd!=NULL);
	ASSERT(::IsWindow(pChildWnd->GetSafeHwnd()));

	// make sure we add MDIChild window
	if((pChildWnd->GetExStyle() & WS_EX_MDICHILD) == 0)
		return NULL;

	// save information about new entry
	CXTPTabManagerItem* pItem = CXTPTabManager::AddItem(GetItemCount());
	pItem->SetCaption(m_pTabClientWnd->GetItemText(pChildWnd));

	SetSelectedItem(pItem);

	return pItem;
}

void CXTPTabClientWnd::CWorkspace::OnItemClick(CXTPTabManagerItem* pItem)
{
	ASSERT(pItem);

	m_pTabClientWnd->MDIActivate(CWnd::FromHandle(pItem->GetHandle()));
}

void CXTPTabClientWnd::CWorkspace::OnNavigateButtonClick(UINT nID)
{
	nID;
	ASSERT(nID == xtpTabNavigateButtonClose);

	if (!m_pSelected)
		return;

	m_pTabClientWnd->PostMessage(WM_IDLEUPDATECMDUI);

	HWND hWnd = m_pSelected->GetHandle();
	::SendMessage(hWnd, WM_CLOSE, 0, 0);

}


HICON CXTPTabClientWnd::CWorkspace::GetItemIcon(CXTPTabManagerItem *pItem)
{
	return m_pTabClientWnd->GetItemIcon(pItem);
}

COLORREF CXTPTabClientWnd::CWorkspace::GetItemColor(CXTPTabManagerItem* pItem)
{
	m_pTabClientWnd->GetItemColor(pItem);

	return CXTPTabManager::GetItemColor(pItem);
}

void CXTPTabClientWnd::CWorkspace::ReOrder(HWND hWnd, CPoint pt, CXTPTabManagerItem* pItem)
{
	if (m_pTabClientWnd->GetWorkspaceCount() == 1 && pItem->GetTabManager()->GetItemCount() == 1)
	{
		return;
	}

	m_pSelected = pItem;

	m_pTabClientWnd->m_bLockReposition = TRUE;
	m_pTabClientWnd->SetActiveWorkspace(this);
	m_pTabClientWnd->m_bLockReposition = FALSE;

	if (GetPaintManager()->m_bHotTracking)
		m_pHighlighted = pItem;

	Reposition();

	CArray<CRect, CRect&> arrRects;

	for (int j = 0; j < GetItemCount(); j++)
	{
		CRect rc = GetItem(j)->GetRect();
		arrRects.Add(rc);
	}

	::SetCapture(hWnd);
	BOOL bRefreshCursor = FALSE;
	BOOL bAccept = FALSE;

	while ( TRUE )
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if ( ::GetCapture() != hWnd )
		{
			DispatchMessage (&msg);
			goto ExitLoop;
		}
		switch ( msg.message )
		{
			case WM_MOUSEMOVE:
				{
					pt = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));

					if (m_rcHeaderRect.IsRectEmpty() || m_rcHeaderRect.PtInRect(pt))
					{
						if (bRefreshCursor)
						{
							m_pTabClientWnd->CancelLoop();
							::SetCursor(::LoadCursor(NULL, IDC_ARROW));
							bRefreshCursor = FALSE;

						}
						for (int i = 0; i < arrRects.GetSize(); i++)
						{
							if (i != pItem->GetIndex() && arrRects[i].PtInRect(pt))
							{

								CXTPTabManagerItem* p = pItem;
								m_arrItems[pItem->GetIndex()] = m_arrItems[i];
								m_arrItems[i] = p;

								OnItemsChanged();
								break;
							}
						}

					}
					else if (m_pTabClientWnd->m_bEnableGroups)
					{
						if (!bRefreshCursor)
						{
							m_pTabClientWnd->InitLoop();
							bRefreshCursor = TRUE;
						}
						m_pTabClientWnd->ReorderWorkspace(pt, pItem);
					}
				}

				break;

			case WM_KEYDOWN:
				if ( msg.wParam != VK_ESCAPE )
					break;
			case WM_CANCELMODE:
			case WM_RBUTTONDOWN:
				goto ExitLoop;

			case WM_LBUTTONUP:
				bAccept = TRUE;
				goto ExitLoop;


			default:
				DispatchMessage (&msg);
				break;
		}
	}

ExitLoop:

	ReleaseCapture();
	PerformMouseMove(hWnd, pt);
	OnItemClick(pItem);

	if (bRefreshCursor)
	{
		m_pTabClientWnd->CancelLoop();
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));

		if (bAccept && m_pTabClientWnd->m_nFocusedAction != ACTION_CANCEL)
		{
			if (m_pTabClientWnd->m_nFocusedAction == ACTION_POPUP)
				m_pTabClientWnd->ContextMenu(pt);
			else  m_pTabClientWnd->DoWorkspaceCommand(pItem, m_pTabClientWnd->m_pFocusWorksapce, m_pTabClientWnd->m_nFocusedAction);
		}
	}

	pItem->GetTabManager()->EnsureVisible(pItem);
}

//////////////////////////////////////////////////////////////////////////
// CSingleWorkspace

CXTPTabClientWnd::CSingleWorkspace::~CSingleWorkspace()
{
	DestroyWindow();
}

INT_PTR CXTPTabClientWnd::CSingleWorkspace::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	return PerformToolHitTest(m_hWnd, HitTest(point), pTI);
}

BOOL CXTPTabClientWnd::CSingleWorkspace::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDOWN ||
		pMsg->message == WM_RBUTTONDOWN ||
		pMsg->message == WM_MBUTTONDOWN ||
		pMsg->message == WM_LBUTTONDBLCLK ||
		pMsg->message == WM_RBUTTONDBLCLK)
	{

		if (GetParentFrame()->SendMessage(WM_XTP_PRETRANSLATEMOUSEMSG, (WPARAM)pMsg->message, pMsg->lParam))
			return TRUE;
	}


	return CWnd::PreTranslateMessage(pMsg);
}

void CXTPTabClientWnd::CSingleWorkspace::RedrawControl()
{
	if (!m_pTabClientWnd->m_bLockReposition)
		Invalidate(FALSE);
}

BEGIN_MESSAGE_MAP(CXTPTabClientWnd::CSingleWorkspace, CWnd)
	{ WM_LBUTTONDBLCLK, 0, 0, 0, AfxSig_vwp,   (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&CXTPTabClientWnd::CSingleWorkspace::OnLButtonDblClk },
	{ WM_PAINT, 0, 0, 0, AfxSig_vv,   (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&CXTPTabClientWnd::CSingleWorkspace::OnPaint },
	{ WM_ERASEBKGND, 0, 0, 0, AfxSig_bD,   (AFX_PMSG)(AFX_PMSGW)(BOOL (AFX_MSG_CALL CWnd::*)(CDC*))&CXTPTabClientWnd::CSingleWorkspace::OnEraseBkgnd },
	{ WM_LBUTTONDOWN, 0, 0, 0, AfxSig_vwp,   (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&CXTPTabClientWnd::CSingleWorkspace::OnLButtonDown },
	{ WM_MOUSEMOVE, 0, 0, 0, AfxSig_vwp,   (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&CXTPTabClientWnd::CSingleWorkspace::OnMouseMove },
	{ WM_MOUSELEAVE, 0, 0, 0, AfxSig_vv,   (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&CXTPTabClientWnd::CSingleWorkspace::OnMouseLeave },
	{ WM_RBUTTONDOWN, 0, 0, 0, AfxSig_vwp,   (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&CXTPTabClientWnd::CSingleWorkspace::OnRButtonDown },
END_MESSAGE_MAP()

void CXTPTabClientWnd::CSingleWorkspace::OnRButtonDown(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);
	UNREFERENCED_PARAMETER(point);

}

void CXTPTabClientWnd::CSingleWorkspace::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint, rc);

	if (m_pTabClientWnd)
	{
		CRect rectClient;
		m_pTabClientWnd->GetWindowRect(rectClient);
		ScreenToClient(&rectClient);
		dcPaint.ExcludeClipRect(rectClient);

	}
	GetPaintManager()->DrawTabControl(this, &dc, rc);
}

BOOL CXTPTabClientWnd::CSingleWorkspace::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPTabClientWnd::CSingleWorkspace::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	m_pTabClientWnd->m_bLockUpdate = TRUE;
	m_pTabClientWnd->m_bDelayLock = TRUE;

	PerformClick(m_hWnd, point);


}

void CXTPTabClientWnd::CSingleWorkspace::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnLButtonDown(nFlags, point);
}


void CXTPTabClientWnd::CSingleWorkspace::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
	PerformMouseMove(m_hWnd, point);
}

void CXTPTabClientWnd::CSingleWorkspace::OnMouseLeave()
{
	PerformMouseMove(m_hWnd,CPoint(-1, -1));
}




/////////////////////////////////////////////////////////////////////////////
// CXTPTabClientWnd

IMPLEMENT_DYNAMIC(CXTPTabClientWnd, CWnd)

CXTPTabClientWnd::CXTPTabClientWnd()
{
	m_pParentFrame = NULL;
	m_bAllowReorder = TRUE;

	m_pPaintManager = new CXTPTabPaintManager();
	m_pPaintManager->m_bBoldSelected = TRUE;

	m_themeCommandBars = xtpThemeOffice2000;
	m_bAutoTheme = TRUE;

	m_bLockUpdate = FALSE;
	m_bLockReposition = FALSE;
	m_bForceToRecalc = FALSE;

	m_pActiveWorkspace = NULL;
	m_bHorizSplitting = TRUE;

	m_bRefreshed = FALSE;

	m_bThemedBackColor = TRUE;
	m_bUpdateContents = FALSE;

	m_pDC = NULL;

	m_hCursorHoriz = AfxGetApp()->LoadCursor( XTP_IDC_HSPLITBAR);
	m_hCursorVert = AfxGetApp()->LoadCursor( XTP_IDC_VSPLITBAR);
	m_hCursorNew = AfxGetApp()->LoadCursor( XTP_IDC_WORKSAPCE_NEW);
	m_hCursorDelete = AfxGetApp()->LoadCursor( XTP_IDC_WORKSAPCE_DELETE);

	m_bShowWorkspace = TRUE;

	CImageList il;
	il.Create(XTP_ID_WORKSAPCE_NEWHORIZONTAL, 16, 1, RGB(0, 255, 0));
	UINT nIDs[] = {XTP_ID_WORKSAPCE_NEWHORIZONTAL, XTP_ID_WORKSAPCE_NEWVERTICAL};
	XTPImageManager()->SetIcons(il, nIDs, 2, CSize(16, 16));

	m_dwFlags = 0;
	m_bEnableTooltips = FALSE;

	m_pDropTarget = new CTabClientDropTarget();
	m_pDropTarget->m_pTabClientWnd = this;

	m_bDelayLock = FALSE;

}

CXTPTabClientWnd::~CXTPTabClientWnd()
{
	m_pPaintManager->InternalRelease();

	for (int i = 0; i < GetWorkspaceCount(); i++)
	{
		delete GetWorkspace(i);
	}

	delete m_pDropTarget;

}

CXTPTabManagerItem* CXTPTabClientWnd::FindItem(const HWND hWnd)
{
	// loop through all tab items
	for(int nIndex = 0; nIndex < GetWorkspaceCount(); nIndex++)
	{
		CXTPTabManagerItem* pItem = GetWorkspace(nIndex)->FindItem(hWnd);

		if (pItem)
			return pItem;
	}

	return NULL;
}


CWnd* CXTPTabClientWnd::MDIGetActive()
{
	return (GetParentFrame()->MDIGetActive());

}

void CXTPTabClientWnd::MDIActivate(CWnd* pWnd)
{
	if (!pWnd)
		return;

	GetParentFrame()->MDIActivate(pWnd);
}

CString CXTPTabClientWnd::GetItemText(const CWnd* pChildWnd) const
{
	ASSERT(pChildWnd!=NULL);

	ASSERT(pChildWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)));

	CString sWindowText = (LPCTSTR)((CWnd*)pChildWnd)->
		SendMessage(WM_XTP_GETWINDOWTEXT);

	if(sWindowText.IsEmpty())
	{
		CDocument* pDoc=((CMDIChildWnd*)pChildWnd)->GetActiveDocument();
		if(pDoc != NULL)
			sWindowText=pDoc->GetTitle();

		if(sWindowText.IsEmpty())
			pChildWnd->GetWindowText(sWindowText);
	}

	return sWindowText;
}

COLORREF CXTPTabClientWnd::GetItemColor(CXTPTabManagerItem* pItem)
{
	COLORREF clr = (COLORREF)::SendMessage(pItem->GetHandle(), WM_XTP_GETTABCOLOR, 0, 0);
	if (clr != 0)
	{
		pItem->m_clrItem = clr;
		return clr;
	}
	return 0;
}




HICON CXTPTabClientWnd::GetItemIcon(CXTPTabManagerItem *pItem)
{
	HWND hWnd = pItem->GetHandle();
	HICON hIcon;

	hIcon = (HICON)::SendMessage(hWnd, WM_XTP_GETTABICON, 0, 0);

	if (!hIcon)
		hIcon = (HICON)(ULONG_PTR)::GetClassLongPtr(hWnd, GCL_HICONSM);

	if (!hIcon)
		hIcon = (HICON)::SendMessage(hWnd, WM_GETICON, ICON_SMALL, 0);

	if (!hIcon)
		hIcon = (HICON)(ULONG_PTR)::GetClassLongPtr(hWnd, GCL_HICON);

	if (!hIcon)
		hIcon = (HICON)::SendMessage(hWnd, WM_GETICON, ICON_BIG, 0);

	return hIcon? hIcon: AfxGetApp()->LoadOEMIcon(OIC_WINLOGO);
}


BEGIN_MESSAGE_MAP(CXTPTabClientWnd, CWnd)
	//{{AFX_MSG_MAP(CXTPTabClientWnd)
	ON_MESSAGE(WM_MDIACTIVATE,OnMDIActivate)
	ON_MESSAGE(WM_MDICREATE,OnMDICreate)
	ON_MESSAGE(WM_MDIDESTROY,OnMDIDestroy)
	ON_WM_SIZE()
	ON_MESSAGE_VOID(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_NCACTIVATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONDOWN()

	ON_COMMAND_RANGE(XTP_ID_WORKSAPCE_MOVEPREVIOUS, XTP_ID_WORKSAPCE_NEWVERTICAL, OnWorkspaceCommand)
	ON_UPDATE_COMMAND_UI_RANGE(XTP_ID_WORKSAPCE_MOVEPREVIOUS, XTP_ID_WORKSAPCE_NEWVERTICAL, OnUpdateWorkspaceCommand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CXTPTabClientWnd::CWorkspace*  CXTPTabClientWnd::AddWorksapce(int nIndex)
{
	m_pDropTarget->Revoke();

	CWorkspace* pWorkspace = NULL;
	if (m_bEnableGroups)
	{
		pWorkspace = new CWorkspace();
		m_pDropTarget->Register(this);
	}
	else
	{
		pWorkspace = new CSingleWorkspace();
		VERIFY(((CSingleWorkspace*)pWorkspace)->Create(AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(0, IDC_ARROW)), _T(""), WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_CHILD|WS_VISIBLE, CXTPEmptyRect(), m_pParentFrame, 0));
		((CSingleWorkspace*)pWorkspace)->EnableToolTips(m_bEnableTooltips);
		m_pDropTarget->Register((CSingleWorkspace*)pWorkspace);
	}

	if (nIndex == -1) nIndex = GetWorkspaceCount();
	m_arrWorkspace.InsertAt(nIndex, pWorkspace);

	pWorkspace->m_pTabClientWnd = this;
	pWorkspace->SetActive(FALSE);
	pWorkspace->SetItemMetrics(CSize(70, 18), 0, CSize(220, 0));

	UpdateFlags(pWorkspace);

	m_bForceToRecalc = TRUE;

	return pWorkspace;

}


BOOL CXTPTabClientWnd::Attach(CXTPMDIFrameWnd* pParentFrame,
	BOOL bEnableGroups /*= FALSE*/)
{
	ASSERT(GetWorkspaceCount() == 0);
	ASSERT(pParentFrame!=NULL);
	ASSERT(::IsWindow(pParentFrame->GetSafeHwnd()));

	m_bEnableGroups = bEnableGroups;


	// check if already attached
	if(IsAttached())
	{
		TRACE(_T("CXTPTabClientWnd::Attach: window has already been attached. Call Detach() function before!\n"));
		return FALSE;
	}

	// make sure the specified window is/derived from CMDIFrameWnd class
	if(!pParentFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
	{
		TRACE(_T("CXTPTabClientWnd::Attach: specified frame window is not of CMDIFrameWnd class (or derived)!\n"));
		return FALSE;
	}

	// try to sublass MDIClient window
	if(!SubclassWindow(pParentFrame->m_hWndMDIClient))
	{
		TRACE(_T("CXTPTabClientWnd::Attach: failed to subclass MDI Client window\n"));
		return FALSE;
	}

	// save the pointer to parent MDIFrame
	m_pParentFrame = pParentFrame;

	CheckCommandBarsTheme();
	GetPaintManager()->RefreshMetrics();


	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER);

	// update the size and position of the tab control and MDIClient window
	m_pParentFrame->RecalcLayout();


	// populate tab control with MDIChild windows if any exist at the moment
	UpdateContents();

	return TRUE;
}

void CXTPTabClientWnd::EnableToolTips(BOOL bEnable /*= TRUE*/)
{
	m_bEnableTooltips = bEnable;

	if (m_bEnableGroups)
		CWnd::EnableToolTips(bEnable);


}

void CXTPTabClientWnd::ShowWorkspace(BOOL bShow)
{
	if (!m_bEnableGroups)
	{
		if (GetWorkspaceCount() == 1 && ((CSingleWorkspace*)GetWorkspace(0))->m_hWnd)
		{
			((CSingleWorkspace*)GetWorkspace(0))->ShowWindow(bShow? SW_SHOW: SW_HIDE);
		}

		m_bShowWorkspace = bShow;
		Reposition();

		m_pParentFrame->RecalcLayout();
	}
}

BOOL CXTPTabClientWnd::Detach()
{
	// check if any attached
	if(!IsAttached())
	{
		TRACE(_T("CXTPTabClientWnd::Attach: there is nothing to detach! Window hasn't been attached!\n"));
		return FALSE;
	}

	m_pDropTarget->Revoke();

	for (int i = 0; i < GetWorkspaceCount(); i++)
	{
		delete GetWorkspace(i);
	}
	m_arrWorkspace.RemoveAll();

	m_pActiveWorkspace = NULL;

	// unsubclass MDIClient window
	UnsubclassWindow();

	// update the size and position of the MDIClient window
	if(::IsWindow(m_pParentFrame->GetSafeHwnd()))
		m_pParentFrame->RecalcLayout();

	m_pParentFrame = NULL;

	return TRUE;
}


CString CXTPTabClientWnd::CWorkspace::GetItemTooltip(CXTPTabManagerItem* pItem) const
{
	CString strTooltip = CXTPTabManager::GetItemTooltip(pItem);
	if (!strTooltip.IsEmpty())
		return strTooltip;

	CMDIChildWnd* pChild = DYNAMIC_DOWNCAST(CMDIChildWnd, CWnd::FromHandle(pItem->GetHandle()));
	if (pChild )
	{
		strTooltip = (LPCTSTR)pChild->SendMessage(WM_XTP_GETWINDOWTOOLTIP, (WPARAM)pItem);
		if(!strTooltip.IsEmpty())
			return strTooltip;


		if (pChild->GetActiveDocument())
			strTooltip =  pChild->GetActiveDocument()->GetPathName();

		if(!strTooltip.IsEmpty())
			return strTooltip;
	}
	return pItem->GetCaption();
}



INT_PTR CXTPTabClientWnd::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	if (XTPMouseManager()->IsMouseLocked())
		return -1;

	CXTPTabManagerItem* pItem = HitTest(point);
	if (pItem)
	{
		return pItem->GetTabManager()->PerformToolHitTest(m_hWnd, pItem, pTI);
	}
	return -1;
}


void CXTPTabClientWnd::CheckCommandBarsTheme()
{
	if (!m_bAutoTheme)
		return;

	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (!pCommandBars)
		return;

	XTPPaintTheme themeCommandBars = pCommandBars->GetPaintManager()->GetCurrentTheme();

	if (m_themeCommandBars != themeCommandBars)
	{

		m_pPaintManager->m_bStaticFrame = FALSE;
		m_pPaintManager->m_clientFrame = xtpTabFrameBorder;
		m_pPaintManager->m_bOneNoteColors = FALSE;
		m_pPaintManager->m_bHotTracking = FALSE;

		switch(themeCommandBars)
		{
		case xtpThemeOfficeXP:
			m_pPaintManager->SetAppearance(xtpTabAppearanceVisualStudio);
			m_pPaintManager->m_bStaticFrame = TRUE;
			m_pPaintManager->m_clientFrame = xtpTabFrameSingleLine;
			break;

		case xtpThemeOffice2003:
			m_pPaintManager->m_bDisableLunaColors = FALSE;
			m_pPaintManager->m_bOneNoteColors = TRUE;
			m_pPaintManager->m_bHotTracking = TRUE;
			m_pPaintManager->SetAppearance(xtpTabAppearancePropertyPage2003);
			break;

		case xtpThemeNativeWinXP:
			m_pPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
			m_pPaintManager->SetColor(xtpTabColorWinXP);
			m_pPaintManager->m_bHotTracking = TRUE;
			break;

		case xtpThemeWhidbey:
			m_pPaintManager->m_bStaticFrame = TRUE;
			m_pPaintManager->m_clientFrame = xtpTabFrameSingleLine;
			m_pPaintManager->m_bDisableLunaColors = TRUE;
			m_pPaintManager->SetAppearance(xtpTabAppearancePropertyPage2003);
			m_pPaintManager->SetColor(xtpTabColorWhidbey);
			break;

		default:
			m_pPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
			break;
		}

		m_themeCommandBars = themeCommandBars;

		Refresh();

	}
}

int CXTPTabClientWnd::GetWorkspaceCount() const
{
	return (int)m_arrWorkspace.GetSize();
}

CXTPTabClientWnd::CWorkspace* CXTPTabClientWnd::GetWorkspace(int nIndex) const
{
	return m_arrWorkspace[nIndex];
}


void CXTPTabClientWnd::OnIdleUpdateCmdUI()
{
	if (m_bDelayLock)
	{
		m_bDelayLock = FALSE;
		m_bLockUpdate = FALSE;
	}

	UpdateContents();
}

CXTPTabManagerItem* CXTPTabClientWnd::AddItem(CWnd* pChildWnd)
{
#ifdef _DEBUG
	if (m_bEnableGroups)
	{
		ASSERT((pChildWnd->GetStyle() & WS_MAXIMIZE) == 0);
	}
#endif

	if (GetWorkspaceCount() == 0)
		AddWorksapce();

	CWorkspace* pActiveWorkspace = m_pActiveWorkspace? m_pActiveWorkspace: m_arrWorkspace[GetWorkspaceCount() - 1];

	return pActiveWorkspace->AddItem(pChildWnd);
}


void CXTPTabClientWnd::SetActiveWorkspace(CWorkspace* pWorkspace)
{
	if (m_pActiveWorkspace != pWorkspace)
	{
		if (m_pActiveWorkspace)
			m_pActiveWorkspace->SetActive(FALSE);

		m_pActiveWorkspace = pWorkspace;
		m_pActiveWorkspace->SetActive(TRUE);
	}
}

void CXTPTabClientWnd::WorkspaceToScreen(LPPOINT lpPoint)
{
	if (m_bEnableGroups)
	{
		ClientToScreen(lpPoint);
	}
	else if (GetWorkspaceCount() != 0)
	{
		CSingleWorkspace* pWorkspace = (CSingleWorkspace*)GetWorkspace(0);
		pWorkspace->ClientToScreen(lpPoint);
	}

}
void CXTPTabClientWnd::ScreenToWorkspace(LPPOINT lpPoint)
{
	if (m_bEnableGroups)
	{
		ScreenToClient(lpPoint);
	}
	else if (GetWorkspaceCount() != 0)
	{
		CSingleWorkspace* pWorkspace = (CSingleWorkspace*)GetWorkspace(0);
		pWorkspace->ScreenToClient(lpPoint);
	}
}


void CXTPTabClientWnd::Refresh(BOOL bRecalcLayout)
{
	if (!IsAttached())
		return;

	if (m_bRefreshed)
		return;

	m_bRefreshed = TRUE;

	UpdateContents();

	if (GetWorkspaceCount() != 0)
	{
		CPoint pt;
		GetCursorPos(&pt);
		ScreenToWorkspace(&pt);

		if (m_bEnableGroups)
		{
			OnMouseMove(0, pt);
		} else
		{
			((CSingleWorkspace*)GetWorkspace(0))->OnMouseMove(0, pt);
		}
	}
	if (bRecalcLayout)
	{
		if (m_bEnableGroups)
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER);

		CXTPCommandBars* pCommandBars = GetCommandBars();
		if (pCommandBars)
			pCommandBars->RecalcFrameLayout();
	}

	m_bRefreshed = FALSE;
}


void CXTPTabClientWnd::UpdateContents()
{
	if (m_hWnd == 0 || !IsWindowVisible())
		return;

	if (!m_bShowWorkspace)
		return;

	if (m_bLockUpdate)
		return;

	// check MDI windows
	CMDIFrameWnd* pFrameWnd = GetParentFrame();
	if(pFrameWnd == NULL)
		return;

	if (m_bUpdateContents)
		return;

	CheckCommandBarsTheme();

	m_bUpdateContents = TRUE;
	m_bLockReposition = TRUE;

	// get pointer to currently active MDIChild
	CWnd* pActiveChildWnd = MDIGetActive();

	CXTPTabManagerItem* pActiveItem = NULL, *pNewItem = NULL;

	int i;
	for (i = 0; i < GetWorkspaceCount(); i++)
	{
		for(int nIndex = 0; nIndex < GetWorkspace(i)->GetItemCount(); nIndex++)
			GetWorkspace(i)->GetItem(nIndex)->m_bFound = FALSE;
	}


	// start enumerating from currently active MDIChild
	CWnd* pChildWnd = GetWindow(GW_CHILD);

	// enumerate all child windows
	while (pChildWnd)
	{
		// see if can find
		CXTPTabManagerItem* pFoundItem = FindItem(pChildWnd->GetSafeHwnd());

		if(pFoundItem != NULL)
		{
			if((pChildWnd->GetStyle()&WS_VISIBLE)==WS_VISIBLE)
			{
				// update text if necessary
				pFoundItem->SetCaption(GetItemText(pChildWnd));

			}
			else
			{
				pFoundItem->Remove();
				pFoundItem = NULL;
			}
		}
		else
		{
			if((pChildWnd->GetStyle()&WS_VISIBLE) == WS_VISIBLE)
			{
				// add item
				pNewItem = pFoundItem = AddItem(pChildWnd);
				pFoundItem->m_hWnd = pChildWnd->GetSafeHwnd();

				pFrameWnd->SendMessage(WM_XTP_NEWTABITEM, (WPARAM)pNewItem);

			}
		}
		if (pFoundItem)
		{
			pFoundItem->m_bFound = TRUE;

			if(pChildWnd == pActiveChildWnd)
				pActiveItem = pFoundItem;
		}

		// get next MDIChild
		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}

	for (i = GetWorkspaceCount() - 1; i >= 0; i--)
	{
		CWorkspace* pWorkspace = GetWorkspace(i);
		for(int nIndex = pWorkspace->GetItemCount() - 1; nIndex >= 0; nIndex--)
		{
			CXTPTabManagerItem* pItem = pWorkspace->GetItem(nIndex);
			if (!pItem->m_bFound)
			{
				pItem->Remove();
			}
		}

		if (pWorkspace->GetItemCount() == 0)
		{
			if (pWorkspace == m_pActiveWorkspace)
				m_pActiveWorkspace = NULL;

			CWorkspace* pWorkcpaceGrow = i == 0? (GetWorkspaceCount() > 1? GetWorkspace(i + 1): NULL): GetWorkspace(i - 1);
			if (pWorkcpaceGrow)
				pWorkcpaceGrow->m_dHeight += pWorkspace->m_dHeight;

			delete pWorkspace;
			m_arrWorkspace.RemoveAt(i);

			m_bForceToRecalc = TRUE;

		}
	}

	// set the active item
	if(pActiveItem != NULL)
	{
		if (pActiveItem->GetTabManager()->GetSelectedItem() != pActiveItem)
		{
			pActiveItem->GetTabManager()->SetSelectedItem(pActiveItem);
		}

		SetActiveWorkspace((CWorkspace*)pActiveItem->GetTabManager());

	}

	m_bLockReposition = FALSE;

	if(m_bForceToRecalc)
	{
		// update the size and position of the tab control and MDIClient window
		if(::IsWindow(GetParentFrame()->GetSafeHwnd()))
			GetParentFrame()->RecalcLayout();

		if (pNewItem)
			pNewItem->GetTabManager()->EnsureVisible(pNewItem);


		m_bForceToRecalc = FALSE;
	}

	m_bUpdateContents = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPTabClientWnd message handlers


// handler for WM_MDIACTIVATE message. Will select corresponding
// tab control item
LRESULT CXTPTabClientWnd::OnMDIActivate(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	LRESULT lResult = Default();
	UpdateContents();
	return lResult;
}


// handler for WM_MDICREATE message. Will add new item into the
// tab control
LRESULT CXTPTabClientWnd::OnMDICreate(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	LRESULT lResult = Default();

	if (lResult && m_bEnableGroups)
	{
		CWnd* pWnd = CWnd::FromHandle((HWND)lResult);
		if (pWnd)
		{
			pWnd->ModifyStyle(WS_BORDER | WS_DLGFRAME | WS_THICKFRAME | WS_MAXIMIZEBOX |WS_MINIMIZEBOX, 0);
			pWnd->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
			pWnd->MoveWindow(0, 0, 0, 0);
		}
	}

	UpdateContents();
	return lResult;
}


// handler for WM_MDIDESTROY message. Will remove the correspondent item
// from the tab control
LRESULT CXTPTabClientWnd::OnMDIDestroy(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	LRESULT lResult = Default();
	UpdateContents();
	return lResult;
}

BOOL CXTPTabClientWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest == HTCLIENT && m_bEnableGroups)
	{
		CPoint point;

		GetCursorPos(&point);
		ScreenToClient(&point);

		for (int nIndex = 0; nIndex < GetWorkspaceCount(); nIndex++)
		{
			CWorkspace* pWorkspace = GetWorkspace(nIndex);
			if (pWorkspace->m_rcSplitter.PtInRect(point))
			{
				SetCursor( !m_bHorizSplitting? m_hCursorHoriz: m_hCursorVert );
				return TRUE;
			}
		}
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


#define SPLITTER_HEIGHT 5

void CXTPTabClientWnd::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
	if (GetWorkspaceCount() == 0 || !m_bShowWorkspace)
	{
		Invalidate(FALSE);
		CWnd::CalcWindowRect(lpClientRect, nAdjustType);
		return;
	}

	if (!m_bEnableGroups)
	{
		CSingleWorkspace* pWorkspace = (CSingleWorkspace*)GetWorkspace(0);

		// now do the laying out
		HDWP dwh = BeginDeferWindowPos(2);

		// move tab window
		if(::IsWindow(pWorkspace->m_hWnd) && (pWorkspace->GetStyle()&WS_VISIBLE)==WS_VISIBLE)
		{
			{

				// move tab control
				dwh=::DeferWindowPos(dwh, pWorkspace->m_hWnd,NULL,lpClientRect->left,
					lpClientRect->top,
					lpClientRect->right-lpClientRect->left,
					lpClientRect->bottom-lpClientRect->top, SWP_NOZORDER);


				CClientDC dc(pWorkspace);

				CRect rectTab(lpClientRect);
				rectTab.OffsetRect(-rectTab.TopLeft());

				GetPaintManager()->RepositionTabControl(pWorkspace, &dc, rectTab);

				rectTab = lpClientRect;
				GetPaintManager()->AdjustClientRect(pWorkspace, rectTab);
				*lpClientRect = rectTab;


				pWorkspace->Invalidate(FALSE);

			}

			if(lpClientRect->bottom<lpClientRect->top ||
				lpClientRect->right<lpClientRect->left)
				::memset(lpClientRect,0,sizeof(RECT));

			::ShowWindow(pWorkspace->m_hWnd,SW_SHOWNA);
		}

		// move MDIClient window
		dwh = ::DeferWindowPos(dwh,m_hWnd,NULL,lpClientRect->left,lpClientRect->top,
			lpClientRect->right-lpClientRect->left,
			lpClientRect->bottom-lpClientRect->top,
			SWP_NOZORDER);

		EndDeferWindowPos(dwh);
	}
	else
	{

		CClientDC dc(this);

		CRect rcBorders(2, 2, -2, -2);
		m_pPaintManager->GetAppearanceSet()->DrawWorkspacePart(NULL, rcBorders, xtpTabWorkspacePartWidth);

		CRect rcClient(0, 0, lpClientRect->right-lpClientRect->left + rcBorders.Width(), lpClientRect->bottom-lpClientRect->top + rcBorders.Height());

		int nIndex;
		double dTotalHeight = 0;

		for (nIndex = 0; nIndex < GetWorkspaceCount(); nIndex++)
		{
			dTotalHeight += GetWorkspace(nIndex)->m_dHeight;
		}
		int nTotalHeight = m_bHorizSplitting? rcClient.Height(): rcClient.Width();
		double dHeight = 0;

		for (nIndex = 0; nIndex < GetWorkspaceCount(); nIndex++)
		{
			CWorkspace* pWorkspace = GetWorkspace(nIndex);

			CRect rc(rcClient);
			if (nIndex != GetWorkspaceCount() - 1)
			{
				dHeight += pWorkspace->m_dHeight;

				if (m_bHorizSplitting)
				{
					int nBottom  = int(dHeight * nTotalHeight / dTotalHeight);
					pWorkspace->m_rcSplitter.SetRect(rc.left, nBottom - SPLITTER_HEIGHT, rc.right, nBottom);

					rc.bottom = nBottom - SPLITTER_HEIGHT;
					rcClient.top = nBottom;
				} else
				{
					int nRight = int(dHeight * nTotalHeight / dTotalHeight);
					pWorkspace->m_rcSplitter.SetRect(nRight - SPLITTER_HEIGHT, rc.top, nRight, rc.bottom);

					rc.right = nRight - SPLITTER_HEIGHT;
					rcClient.left = nRight;
				}
			}
			else
			{
				pWorkspace->m_rcSplitter.SetRectEmpty();

			}


			GetPaintManager()->RepositionTabControl(pWorkspace, &dc, rc);
			GetPaintManager()->AdjustClientRect(pWorkspace, rc);

			for (int i = 0; i < pWorkspace->GetItemCount(); i++)
			{
				HWND hWndChild = pWorkspace->GetItem(i)->m_hWnd;
				::MoveWindow(hWndChild, rc.left, rc.top, rc.Width(), rc.Height(), TRUE);
			}
		}

		Invalidate(FALSE);
	}

	CWnd::CalcWindowRect(lpClientRect, nAdjustType);
}

// update the size of the tab control and the MDIClient window
void CXTPTabClientWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// update the contents of the tab control afterwards
	m_bForceToRecalc = TRUE;
	UpdateContents();

}


BOOL CXTPTabClientWnd::OnEraseBkgnd(CDC* )
{
	return TRUE;
}

void CXTPTabClientWnd::OnFillBackground(CDC* pDC, CRect rc)
{
		CXTPCommandBars* pCommandBars = GetCommandBars();
		if (pCommandBars)
		{
			if (!m_bThemedBackColor)
				DefWindowProc(WM_PAINT, (WPARAM)pDC->m_hDC, 0);
			else
				pDC->FillSolidRect(rc, pCommandBars->GetPaintManager()->GetXtremeColor(COLOR_APPWORKSPACE));
		} else
		{
			pDC->FillSolidRect(rc, GetSysColor(COLOR_APPWORKSPACE));
		}

}

void CXTPTabClientWnd::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint, rc);

	if (GetWorkspaceCount() == 0 || !m_bEnableGroups)
	{
		OnFillBackground(&dc, rc);
	}
	else
	{
		for (int i = 0; i < GetWorkspaceCount(); i++)
		{
			CWorkspace* pWorkspace = GetWorkspace(i);
			GetPaintManager()->DrawTabControl(GetWorkspace(i), &dc, pWorkspace->m_rcControl);

			if (!pWorkspace->m_rcSplitter.IsRectEmpty())
			{
				m_pPaintManager->GetAppearanceSet()->DrawWorkspacePart(&dc, pWorkspace->m_rcSplitter, m_bHorizSplitting? xtpTabWorkspacePartHSplitter: xtpTabWorkspacePartVSplitter);
			}
		}
	}
}

void CXTPTabClientWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	if (m_bEnableGroups)
	{
		m_pPaintManager->GetAppearanceSet()->DrawWorkspacePart(NULL, &lpncsp->rgrc[0], xtpTabWorkspacePartWidth);
	}

	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}


BOOL CXTPTabClientWnd::OnNcActivate(BOOL bActive)
{
	if (m_bEnableGroups)
		return TRUE;

	return CWnd::OnNcActivate(bActive);
}

void CXTPTabClientWnd::OnNcPaint()
{
	if (!m_bEnableGroups)
	{
		Default();
		return;
	}

	CWindowDC dcPaint(this);
	CXTPWindowRect rc(this);
	rc.OffsetRect(rc.TopLeft());

	m_pPaintManager->GetAppearanceSet()->DrawWorkspacePart(&dcPaint, rc, xtpTabWorkspacePartBorder);
}

void CXTPTabClientWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonDown(nFlags, point);

}


void CXTPTabClientWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bEnableGroups)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	m_bLockUpdate = TRUE;

	m_bDelayLock = TRUE;

	for (int i = 0; i < GetWorkspaceCount(); i++)
	{
		CWorkspace* pWorkspace = GetWorkspace(i);
		if (pWorkspace->m_rcSplitter.PtInRect(point) && i < GetWorkspaceCount() - 1)
		{
			CXTPSplitterTracker tracker;

			CRect rcFirst = pWorkspace->GetControlRect();
			CRect rcSecond = m_arrWorkspace[i + 1]->GetControlRect();

			CRect rcAvail;
			rcAvail.UnionRect(rcFirst, rcSecond);

			CRect rc = pWorkspace->m_rcSplitter;

			ClientToScreen(&rc);
			ClientToScreen(&rcAvail);
			point = rc.TopLeft();

			if (tracker.Track(this, rcAvail, rc, point, !m_bHorizSplitting))
			{
				double dTotal = pWorkspace->m_dHeight + m_arrWorkspace[i + 1]->m_dHeight;

				double dDelta = !m_bHorizSplitting? double(rc.left + 2 - rcAvail.left) / rcAvail.Width():
					double(rc.top + 2 - rcAvail.top) / rcAvail.Height();

				pWorkspace->m_dHeight = dDelta * dTotal;
				m_arrWorkspace[i + 1]->m_dHeight = dTotal - pWorkspace->m_dHeight;

				GetParentFrame()->RecalcLayout();
			}

			break;
		}

		if (pWorkspace->GetControlRect().PtInRect(point))
		{
			pWorkspace->PerformClick(m_hWnd, point);
			break;
		}
	}

}


void CXTPTabClientWnd::InitLoop()
{
	// handle pending WM_PAINT messages
	MSG msg;
	while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
	{
		if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
			return;
		DispatchMessage(&msg);
	}

	// initialize state
	m_rectLast.SetRectEmpty();
	m_sizeLast.cx = m_sizeLast.cy = 0;
	m_rcGroup.SetRectEmpty();
	m_pFocusWorksapce = NULL;

	// lock window update while dragging
	ASSERT(m_pDC == NULL);
	CWnd* pWnd = CWnd::GetDesktopWindow();
	if (pWnd->LockWindowUpdate())
		m_pDC = pWnd->GetDCEx(NULL, DCX_WINDOW|DCX_CACHE|DCX_LOCKWINDOWUPDATE);
	else
		m_pDC = pWnd->GetDCEx(NULL, DCX_WINDOW|DCX_CACHE);
	ASSERT(m_pDC != NULL);
}

void CXTPTabClientWnd::CancelLoop()
{
	DrawFocusRect(TRUE);    // gets rid of focus rect

	CWnd* pWnd = CWnd::GetDesktopWindow();
	pWnd->UnlockWindowUpdate();
	if (m_pDC != NULL)
	{
		pWnd->ReleaseDC(m_pDC);
		m_pDC = NULL;
	}
}

void CXTPTabClientWnd::DrawFocusRect(BOOL bRemoveRect)
{
	ASSERT(m_pDC != NULL);

	// determine new rect and size
	CBrush* pDitherBrush = CDC::GetHalftoneBrush();
	CRect rect = m_rcGroup;
	ClientToScreen(rect);

	CSize size(GetSystemMetrics(SM_CXFRAME), GetSystemMetrics(SM_CYFRAME));

	if (bRemoveRect)
		size.cx = size.cy = 0;

	// draw it and remember last size
	m_pDC->DrawDragRect(&rect, size, &m_rectLast, m_sizeLast,
		pDitherBrush, pDitherBrush);

	m_rectLast = rect;
	m_sizeLast = size;
}


void CXTPTabClientWnd::ReorderWorkspace(CPoint pt, CXTPTabManagerItem* pItem)
{
	CXTPClientRect rc(this);

	CRect rcGroup(0, 0, 0, 0);
	m_pFocusWorksapce = NULL;
	m_nFocusedAction = ACTION_CANCEL;

	if (rc.PtInRect(pt))
	{
		m_nFocusedAction = ACTION_POPUP;
		::SetCursor(m_hCursorNew);

		for (int i = 0; i < GetWorkspaceCount(); i++)
		{
			CWorkspace* pWorkspace = GetWorkspace(i);

			BOOL bSelfAvail = pItem->GetTabManager() != pWorkspace || pWorkspace->GetItemCount() > 1;

			if (pWorkspace->GetHeaderRect().PtInRect(pt))
			{
				rcGroup = pWorkspace->GetClientRect();

				m_pFocusWorksapce = pWorkspace;
				m_nFocusedAction = ACTION_ATTACH;
				break;
			}

			if ((GetWorkspaceCount() == 1 || m_bHorizSplitting) && bSelfAvail)
			{
				int nOffset = pWorkspace->GetControlRect().bottom - pt.y;
				if (nOffset > 0 && nOffset < 30)
				{
					rcGroup = pWorkspace->GetControlRect();
					rcGroup.top = rcGroup.CenterPoint().y;

					m_pFocusWorksapce = pWorkspace;
					m_nFocusedAction = ACTION_INSERTHORIZ;
					break;
				}

			}

			if ((GetWorkspaceCount() == 1 || !m_bHorizSplitting) && bSelfAvail)
			{
				int nOffset = pWorkspace->GetControlRect().right - pt.x;
				if (nOffset > 0 && nOffset < 30)
				{
					rcGroup = pWorkspace->GetControlRect();
					rcGroup.left = rcGroup.CenterPoint().x;

					m_pFocusWorksapce = pWorkspace;
					m_nFocusedAction = ACTION_INSERTVERT;
					break;
				}
			}
		}
	}
	else
	{
		::SetCursor(m_hCursorDelete);
	}

	if (m_rcGroup != rcGroup)
	{
		m_rcGroup = rcGroup;
		DrawFocusRect();
	}
}

int CXTPTabClientWnd::FindIndex(CXTPTabManager* pWorkspace)
{
	for (int i = 0; i < GetWorkspaceCount(); i++)
	{
		if (GetWorkspace(i) == pWorkspace)
			return i;
	}
	return -1;
}

CXTPTabManagerItem* CXTPTabClientWnd::HitTest(CPoint pt) const
{
	if (!m_bShowWorkspace)
		return NULL;

	for (int i = 0; i < GetWorkspaceCount(); i++)
	{
		CXTPTabManagerItem* pItem = GetWorkspace(i)->HitTest(pt);
		if (pItem)
			return pItem;
	}
	return NULL;
}

CXTPCommandBars* CXTPTabClientWnd::GetCommandBars()
{
	return ((CXTPMDIFrameWnd*)m_pParentFrame)->GetCommandBars();
}

void CXTPTabClientWnd::ContextMenu(CPoint pt)
{
	CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(GetCommandBars());

	ClientToScreen(&pt);

	pPopupBar->GetControls()->Add(new CXTPControlWorkspaceActions, 0);

	CXTPControl* pControl = pPopupBar->GetControls()->Add(xtpControlButton, XTP_ID_WORKSAPCE_CANCEL);
	pControl->SetBeginGroup(TRUE);
	pControl->SetFlags(xtpFlagManualUpdate);

	CXTPCommandBars::TrackPopupMenu(pPopupBar, 0, pt.x, pt.y,GetParentFrame());

	pPopupBar->InternalRelease();

}

void CXTPTabClientWnd::OnWorkspaceCommand(UINT nID)
{
	CXTPTabManagerItem* pItem = NULL;

	if (!IsWorkspaceCommandEnabled(nID, &pItem))
		return;

	ASSERT(pItem);

	switch(nID)
	{
		case XTP_ID_WORKSAPCE_NEWVERTICAL:
			DoWorkspaceCommand(pItem, (CWorkspace*)pItem->GetTabManager(), ACTION_INSERTVERT);
			break;

		case XTP_ID_WORKSAPCE_NEWHORIZONTAL:
			DoWorkspaceCommand(pItem, (CWorkspace*)pItem->GetTabManager(), ACTION_INSERTHORIZ);
			break;

		case XTP_ID_WORKSAPCE_MOVEPREVIOUS:
			{
				int nIndex = FindIndex(pItem->GetTabManager());
				DoWorkspaceCommand(pItem, GetWorkspace(nIndex - 1), ACTION_ATTACH);
			}
			break;
		case XTP_ID_WORKSAPCE_MOVENEXT:
			{
				int nIndex = FindIndex(pItem->GetTabManager());
				DoWorkspaceCommand(pItem, GetWorkspace(nIndex + 1), ACTION_ATTACH);
			}
			break;
	}
}

void CXTPTabClientWnd::OnUpdateWorkspaceCommand(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsWorkspaceCommandEnabled(pCmdUI->m_nID));
}

BOOL CXTPTabClientWnd::IsWorkspaceCommandEnabled(UINT nID, CXTPTabManagerItem** ppItem)
{
	if (!m_bEnableGroups)
		return FALSE;

	CWnd* pWnd = MDIGetActive();
	if (!pWnd)
		return FALSE;

	CXTPTabManagerItem* pItem = FindItem(pWnd->GetSafeHwnd());
	if (!pItem)
		return FALSE;

	if (ppItem)
		*ppItem = pItem;

	switch(nID)
	{
		case XTP_ID_WORKSAPCE_NEWVERTICAL:
			return (GetWorkspaceCount() == 1 || !m_bHorizSplitting) &&  pItem->GetTabManager()->GetItemCount() > 1;

		case XTP_ID_WORKSAPCE_NEWHORIZONTAL:
			return (GetWorkspaceCount() == 1 || m_bHorizSplitting) &&  pItem->GetTabManager()->GetItemCount() > 1;

		case XTP_ID_WORKSAPCE_MOVEPREVIOUS:
			return FindIndex(pItem->GetTabManager()) != 0;

		case XTP_ID_WORKSAPCE_MOVENEXT:
			return FindIndex(pItem->GetTabManager()) != GetWorkspaceCount() - 1;

	}
	return TRUE;
}


void CXTPTabClientWnd::DoWorkspaceCommand(CXTPTabManagerItem* pItem, CWorkspace* pFocusWorksapce, int nAction)
{
	m_bLockUpdate = m_bLockReposition = TRUE;

	switch (nAction)
	{
		case ACTION_ATTACH:
			{
				ASSERT(pFocusWorksapce);
				ASSERT(pFocusWorksapce != pItem->GetTabManager());

				pItem->InternalAddRef();
				pItem->Remove();

				COLORREF clr = pItem->m_clrItem;
				pFocusWorksapce->CXTPTabManager::AddItem(-1, pItem);
				pItem->m_clrItem = clr;

				pFocusWorksapce->SetSelectedItem(pItem);

				break;
			}
		case ACTION_INSERTHORIZ:
		case ACTION_INSERTVERT:
			{
				ASSERT(pFocusWorksapce);

				pItem->InternalAddRef();
				pItem->Remove();

				COLORREF clr = pItem->m_clrItem;

				int nIndex = FindIndex(pFocusWorksapce) + 1;
				CWorkspace* pWorkspace = AddWorksapce(nIndex);

				pWorkspace->CXTPTabManager::AddItem(-1, pItem);

				pItem->m_clrItem = clr;
				pWorkspace->SetSelectedItem(pItem);

				pWorkspace->m_dHeight = pFocusWorksapce->m_dHeight = pFocusWorksapce->m_dHeight / 2;

				m_bHorizSplitting = (nAction == ACTION_INSERTHORIZ);
				break;
			}

	}

	m_bLockUpdate = m_bLockReposition = FALSE;
	m_bForceToRecalc = TRUE;
	UpdateContents();
}

void CXTPTabClientWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bEnableGroups)
	{
		for (int i = 0; i < GetWorkspaceCount(); i++)
		{
			GetWorkspace(i)->PerformMouseMove(m_hWnd, point);
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}


void CXTPTabClientWnd::OnMouseLeave()
{
	if (m_bEnableGroups)
	{
		for (int i = 0; i < GetWorkspaceCount(); i++)
		{
			GetWorkspace(i)->PerformMouseMove(m_hWnd, CPoint(-1, -1));
		}
	}
}



void CXTPTabClientWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnLButtonDown(nFlags, point);
}

void CXTPTabClientWnd::OnSysColorChange()
{
	CWnd::OnSysColorChange();

	GetPaintManager()->RefreshMetrics();

	Refresh();
}

BOOL CXTPTabClientWnd::PreTranslateMessage(MSG* pMsg)
{
	if (m_bEnableGroups)
	{

		if (pMsg->message == WM_LBUTTONDOWN ||
			pMsg->message == WM_RBUTTONDOWN ||
			pMsg->message == WM_MBUTTONDOWN ||
			pMsg->message == WM_LBUTTONDBLCLK ||
			pMsg->message == WM_RBUTTONDBLCLK)
		{
			if (pMsg->hwnd != m_hWnd)
				return CWnd::PreTranslateMessage(pMsg);

			if (GetParentFrame()->SendMessage(WM_XTP_PRETRANSLATEMOUSEMSG, (WPARAM)pMsg->message, pMsg->lParam))
				return TRUE;
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}



CXTPTabPaintManager* CXTPTabClientWnd::GetPaintManager()
{
	return m_pPaintManager;
}

void CXTPTabClientWnd::SetPaintManager(CXTPTabPaintManager* pPaintManager)
{
	m_pPaintManager->InternalRelease();
	m_pPaintManager = pPaintManager;
	m_pPaintManager->RefreshMetrics();

	m_themeCommandBars = xtpThemeCustom;
	m_bAutoTheme = FALSE;

	Refresh();
}

void CXTPTabClientWnd::SetAutoTheme(BOOL bAutoTheme /*= FALSE*/)
{
	m_bAutoTheme = bAutoTheme;
}

BOOL CXTPTabClientWnd::GetAutoTheme()
{
	return m_bAutoTheme;
}

void CXTPTabClientWnd::UpdateFlags(CWorkspace* pWorkspace)
{
	pWorkspace->FindNavigateButton(xtpTabNavigateButtonClose)->SetFlags(m_dwFlags & xtpWorkspaceHideClose? xtpTabNavigateButtonNone: xtpTabNavigateButtonAlways);
	pWorkspace->FindNavigateButton(xtpTabNavigateButtonLeft)->SetFlags(m_dwFlags & xtpWorkspaceHideArrows? xtpTabNavigateButtonAutomatic: xtpTabNavigateButtonAlways);
	pWorkspace->FindNavigateButton(xtpTabNavigateButtonRight)->SetFlags(m_dwFlags & xtpWorkspaceHideArrows? xtpTabNavigateButtonAutomatic: xtpTabNavigateButtonAlways);
}


void CXTPTabClientWnd::SetFlags(DWORD dwFlags)
{
	m_dwFlags = dwFlags;

	for (int i = 0; i < GetWorkspaceCount(); i++)
		UpdateFlags(GetWorkspace(i));

	Reposition();
}

void CXTPTabClientWnd::Reposition()
{
	GetParentFrame()->RecalcLayout();
}

BOOL CXTPTabClientWnd::IsAllowReorder()
{
	return m_bAllowReorder;
}
void CXTPTabClientWnd::SetAllowReorder(BOOL bAllowReorder)
{
	m_bAllowReorder = bAllowReorder;
}

CXTPTabManagerItem* CXTPTabClientWnd::GetSelectedItem ()
{
	if (m_pActiveWorkspace)
		return m_pActiveWorkspace->GetSelectedItem();
	return NULL;
}

CXTPTabManagerItem* CXTPTabClientWnd::GetItem(int nIndex)
{
	for (int i = 0; i < GetWorkspaceCount(); i++)
	{
		CWorkspace* pWorkspace = GetWorkspace(i);
		int nCount = pWorkspace->GetItemCount();

		if (nIndex < nCount)
			return pWorkspace->GetItem(nIndex);

		nIndex -= nCount;
	}
	return NULL;
}

int CXTPTabClientWnd::GetItemCount()
{
	int nCount = 0;

	for (int i = 0; i < GetWorkspaceCount(); i++)
		nCount += GetWorkspace(i)->GetItemCount();

	return nCount;
}


