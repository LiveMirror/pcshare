// XTShellTreeCtrl.cpp : implementation file
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

#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTVC50Helpers.h"
#include "XTShellPidl.h"
#include "XTShellSettings.h"
#include "XTTreeBase.h"
#include "XTShellTreeBase.h"
#include "XTShellTreeCtrlView.h"
#include "XTVersionManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTShellTreeView

CXTShellTreeView::CXTShellTreeView()
{
	m_pTreeCtrl = (CTreeCtrl_Friendly*)&GetTreeCtrl();
}

CXTShellTreeView::~CXTShellTreeView()
{

}

IMPLEMENT_DYNCREATE(CXTShellTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CXTShellTreeView, CTreeView)
	ON_SHELLTREE_REFLECT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTShellTreeView message handlers

void CXTShellTreeView::SelectionChanged(HTREEITEM hItem, CString strFolderPath)
{
	if (hItem != NULL && !m_bTunneling)
	{
		CDocument* pDoc = GetDocument( );
		if ( pDoc != NULL )
		{
			pDoc->UpdateAllViews( this, SHN_XT_TREESELCHANGE,
				( CObject* )GetTreeCtrl( ).GetItemData( hItem ) );
		}
	}

	CXTShellTreeBase::SelectionChanged(hItem, strFolderPath);

}

BOOL CXTShellTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CTreeView::PreCreateWindow(cs))
		return FALSE;

	cs.style |= TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT;

	return TRUE;
}


void CXTShellTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// Initialize the image list for the list view and populate it.
	if (InitializeTree( XTOSVersionInfo()->IsWinXPOrGreater() ?
		(TVS_HASBUTTONS|TVS_SINGLEEXPAND|TVS_TRACKSELECT|TVS_SHOWSELALWAYS) :
		(TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT) ))
	{
		PopulateTreeView();
	}
}

void CXTShellTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	UNUSED_ALWAYS(pSender);

	switch (lHint)
	{
	// Update selection of tree view to the
	case SHN_XT_SELECTCHILD:
		{
			XT_LVITEMDATA*  lplvid = (XT_LVITEMDATA*)pHint;
			ASSERT(lplvid != NULL);

			LockWindowUpdate();
			if(!FindTreeItem(GetTreeCtrl().GetSelectedItem(), lplvid))
			{
				// The folder was not found so we send back a message
				// to the listview to execute the itemid
				CDocument* pDoc = GetDocument();
				pDoc->UpdateAllViews(this, SHN_XT_NOFOLDER, (CObject*)lplvid);
			}
			UnlockWindowUpdate();
		}
		break;

	default:
		break;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CXTShellTreeCtrl

IMPLEMENT_DYNAMIC(CXTShellTreeCtrl, CTreeCtrl)

CXTShellTreeCtrl::CXTShellTreeCtrl()
{
	m_pListCtrl    = NULL;
	m_bAutoInit    = TRUE;
	m_bContentInitPending = FALSE;
	m_bTunnelPath = FALSE;

	m_pTreeCtrl = (CTreeCtrl_Friendly*)this;
}

CXTShellTreeCtrl::~CXTShellTreeCtrl()
{
}

BEGIN_MESSAGE_MAP(CXTShellTreeCtrl, CTreeCtrl)
	ON_SHELLTREE_REFLECT()
	ON_WM_CREATE()
	ON_MESSAGE(XTWM_SHELL_NOTIFY, OnUpdateShell)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTShellTreeCtrl message handlers

void CXTShellTreeCtrl::SelectionChanged(HTREEITEM hItem, CString strFolderPath)
{
	if (hItem != NULL && !m_bTunneling)
	{
		CWnd* pWndNotify = m_pListCtrl;
		if (!pWndNotify)
		{
			pWndNotify = GetOwner();
		}

		// notify list control of selection change.
		if (pWndNotify && ::IsWindow(pWndNotify->m_hWnd))
		{
			pWndNotify->SendMessage(XTWM_SHELL_NOTIFY,
				(WPARAM)SHN_XT_TREESELCHANGE, (LPARAM)(CObject*)GetItemData(hItem));
		}
	}
	CXTShellTreeBase::SelectionChanged(hItem, strFolderPath);
}

void CXTShellTreeCtrl::PreSubclassWindow()
{
	CTreeCtrl::PreSubclassWindow();
	DelayContentInit();
}

int CXTShellTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	DelayContentInit();

	return 0;
}

// This function delays content initialization
void CXTShellTreeCtrl::DelayContentInit()
{
	// check to see if not posted already
	if (!m_bContentInitPending)
	{
		// if so, set the flag and post the message to initialize at some later time
		m_bContentInitPending = TRUE;
		PostMessage(XTWM_SHELL_NOTIFY, XTWM_INITIAL_UPDATE);
	}
}



// Processes content pending initialization,
void CXTShellTreeCtrl::ProcessContentInit()
{
	// if the request is pending indeed
	if (m_bContentInitPending)
	{
		// reset the flag and autoinitialize as appropriate
		m_bContentInitPending = FALSE;
		if (m_bAutoInit)
		{
			if (InitializeTree( XTOSVersionInfo()->IsWinXPOrGreater() ?
				(TVS_HASBUTTONS|TVS_SINGLEEXPAND|TVS_TRACKSELECT|TVS_SHOWSELALWAYS) :
				(TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT) ))
			{
				PopulateTreeView();
			}
		}

		// check to see if tunnel initialization is pending
		if (m_bTunnelPath)
		{
			m_bTunnelPath = FALSE;
			TunnelTree(m_strTunnelPath);
		}
	}
}

LRESULT CXTShellTreeCtrl::OnUpdateShell(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	// Update selection of tree view to the
	case SHN_XT_SELECTCHILD:
		{
			XT_LVITEMDATA*  lplvid = (XT_LVITEMDATA*)lParam;
			ASSERT(lplvid != NULL);

			LockWindowUpdate();
			if(!FindTreeItem(GetSelectedItem(), lplvid) && m_pListCtrl != NULL)
			{
				// The folder was not found so we send back a message
				// to the listview to execute the itemid
				m_pListCtrl->SendMessage(XTWM_SHELL_NOTIFY,
					(WPARAM)SHN_XT_NOFOLDER, (LPARAM)(CObject*)lplvid);
			}
			UnlockWindowUpdate();
		}
		break;

	case XTWM_INITIAL_UPDATE:
		ProcessContentInit();
		break;

	default:
		break;
	}

	return 0;
}


void CXTShellTreeCtrl::TunnelTree(CString strFindPath)
{
	// check to see if initial update is pending, delay processing if so
	m_strTunnelPath = strFindPath;
	if (m_bContentInitPending)
	{
		m_bTunnelPath = TRUE;
		return;
	}

	CXTShellTreeBase::TunnelTree(strFindPath);
}
