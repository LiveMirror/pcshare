// PaneToolbars.cpp : implementation file
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
#include "CommandBarsDesigner.h"
#include "PaneToolbars.h"
#include "DialogNewToolbar.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


LRESULT CPaneToolbars::CToolbarsTree::WindowProc(UINT message,WPARAM wParam,LPARAM lParam)
{
	switch (message)
	{
	case WM_NCPAINT:
		{
			CTreeCtrl::WindowProc(message, wParam, lParam);

			CRect rc;
			GetWindowRect(&rc);
			CWindowDC dc(this);
			rc.OffsetRect(-rc.TopLeft());
			COLORREF clr = GetStaticFrameColor();
			dc.Draw3dRect(rc, clr, clr);
			return TRUE;
		}
	}
	return CTreeCtrl::WindowProc(message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CPaneToolbars

CPaneToolbars::CPaneToolbars()
{
	m_bLockSelected = FALSE;
}

CPaneToolbars::~CPaneToolbars()
{
}


BEGIN_MESSAGE_MAP(CPaneToolbars, CWnd)
	//{{AFX_MSG_MAP(CPaneToolbars)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_PANETOOLBARS_ADD, OnPanetoolbarsAdd)
	ON_UPDATE_COMMAND_UI(ID_PANETOOLBARS_ADD, OnUpdatePanetoolbarsAdd)
	ON_UPDATE_COMMAND_UI(ID_PANETOOLBARS_DELETE, OnUpdatePanetoolbarsDelete)
	ON_COMMAND(ID_PANETOOLBARS_DELETE, OnPanetoolbarsDelete)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TVN_SELCHANGED, AFX_IDW_PANE_FIRST, OnSelChanged)
	ON_NOTIFY(NM_SETFOCUS, AFX_IDW_PANE_FIRST, OnSetFocus)
END_MESSAGE_MAP()

int CPaneToolbars::BuildToolbarsTree(HTREEITEM hItem, CXTPCommandBar* pCommandBar, BOOL bCheck)
{
	BOOL bToolbar = pCommandBar->GetType() != xtpBarTypePopup;
	int nImageIndex = bToolbar? 0: 1;
	HTREEITEM hItemChild = 0;

	int nTotalCount = 1;

	CString strTitle = pCommandBar->GetTitle();
	if (strTitle.IsEmpty()) strTitle = _T("[Untitled]");

	if (bCheck)
	{
		hItemChild = (HTREEITEM)pCommandBar->GetCommandBarData();

		TVITEM item;
		item.hItem = hItemChild;
		item.mask = TVIF_PARAM;

		if (hItemChild != 0 && m_wndTreeCtrl.SendMessage(TVM_GETITEM, 0, (LPARAM)&item) &&
			(DWORD_PTR)item.lParam == (DWORD_PTR)pCommandBar)
		{
			if (m_wndTreeCtrl.GetItemText(hItemChild) != strTitle)
			{
				m_wndTreeCtrl.SetItemText(hItemChild, strTitle);

			}

		} else
			return 0;

	} else
	{
		hItemChild = m_wndTreeCtrl.InsertItem(strTitle, nImageIndex, nImageIndex, hItem);
		m_wndTreeCtrl.SetItemData(hItemChild, (DWORD_PTR)pCommandBar);
		pCommandBar->SetCommandBarData((DWORD_PTR)hItemChild);

	}

	for (int i = 0; i < pCommandBar->GetControls()->GetCount(); i++)
	{
		CXTPControlPopup* pControl = DYNAMIC_DOWNCAST(CXTPControlPopup, pCommandBar->GetControl(i));
		if (pControl == NULL || pControl->GetID() == XTP_ID_TOOLBAR_EXPAND)
			continue;

		CXTPCommandBar* pPopupBar = pControl->GetCommandBar();

		int nCount = BuildToolbarsTree(hItemChild, pPopupBar, bCheck);
		if (bCheck && nCount == 0)
			return 0;

		nTotalCount += nCount;

	}

	if (!bCheck) m_wndTreeCtrl.Expand (hItemChild, TVE_EXPAND);

	return nTotalCount;
}

void CPaneToolbars::Refresh()
{

	if (!AfxGetMainWnd())
		return;

	if (!m_wndTreeCtrl.GetSafeHwnd())
		return;

	CXTPCommandBars* pCommandBars = ((CMainFrame*)AfxGetMainWnd())->GetActiveCommandBars();

	m_pActiveCommandBars = pCommandBars;

	if (!pCommandBars || pCommandBars->GetCount() == 0)
	{
		m_bLockSelected = TRUE;
		m_wndTreeCtrl.DeleteAllItems();
		m_bLockSelected = FALSE;
		return;
	}


	int nTotalCount = 0;

	for (int i = 0; i < pCommandBars->GetCount(); i++)
	{
		CXTPCommandBar* pCommandBar = pCommandBars->GetAt(i);

		int nCount = BuildToolbarsTree(TVI_ROOT, pCommandBar, TRUE);
		if (nCount == 0)
		{
			nTotalCount = 0;
			break;
		}
		nTotalCount += nCount;
	}
	if (nTotalCount != (int)m_wndTreeCtrl.GetCount())
	{
		nTotalCount = 0;
	}

	if (nTotalCount == 0)
	{
		m_wndTreeCtrl.LockWindowUpdate();
		m_bLockSelected = TRUE;

		m_wndTreeCtrl.DeleteAllItems();

		for (int i = 0; i < pCommandBars->GetCount(); i++)
		{
			CXTPCommandBar* pCommandBar = pCommandBars->GetAt(i);

			BuildToolbarsTree(TVI_ROOT, pCommandBar, FALSE);
		}
		m_wndTreeCtrl.UnlockWindowUpdate();


		((CMainFrame*)AfxGetMainWnd())->m_pActiveCommandBar = NULL;
		((CMainFrame*)AfxGetMainWnd())->RefreshPanes();
		m_bLockSelected = FALSE;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CPaneToolbars message handlers

int CPaneToolbars::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	XTPImageManager()->SetMaskColor(RGB(0, 0xFF, 0));
	VERIFY(m_wndToolBar.CreateToolBar(WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS, this));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_PANE_TOOLBARS));

	m_ilTreeIcons.Create(16, 16, ILC_MASK|ILC_COLOR24, 0, 1);

	CBitmap bmp;
	bmp.LoadBitmap(IDB_PANE_TOOLBARS_ICONS);

	m_ilTreeIcons.Add(&bmp, RGB(0, 255, 0));


	if (!m_wndTreeCtrl.Create(WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE|TVS_HASLINES|
		TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	m_wndTreeCtrl.ModifyStyleEx(0, WS_EX_STATICEDGE);

	m_wndTreeCtrl.SetImageList(&m_ilTreeIcons, TVSIL_NORMAL);


	Refresh();


	return 0;
}

void CPaneToolbars::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CSize sz(0);
	if (m_wndToolBar.GetSafeHwnd())
	{
		sz = m_wndToolBar.CalcDockingLayout(cx, /*LM_HIDEWRAP|*/ LM_HORZDOCK|LM_HORZ | LM_COMMIT);

		m_wndToolBar.MoveWindow(0, 0, cx, sz.cy);
		m_wndToolBar.Invalidate(FALSE);
	}
	if (m_wndTreeCtrl.GetSafeHwnd())
	{
		m_wndTreeCtrl.MoveWindow(0, sz.cy, cx, cy - sz.cy);
	}

}

void CPaneToolbars::OnPanetoolbarsAdd()
{
	CDialogNewToolbar dt;

	CEmbeddedFrame* pFrame = GetMainFrame()->GetActiveEmbeddedFrame();
	if (!pFrame)
		return;

	if (dt.DoModal() == IDOK)
	{
		CXTPToolBar* pCommandBar = (CXTPToolBar*)pFrame->GetCommandBars()->Add(dt.m_strCaption, xtpBarTop);


		pCommandBar->SetBarID(pFrame->ResourceManager()->GetStringID(dt.m_strId));
	}

	Refresh();

}

void CPaneToolbars::OnUpdatePanetoolbarsAdd(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pActiveCommandBars != NULL);

}

void CPaneToolbars::RefreshItem(HTREEITEM hItem)
{
	CXTPCommandBar* pCommandBar = (CXTPCommandBar*)m_wndTreeCtrl.GetItemData(hItem);
	ASSERT(pCommandBar);

	if (((CMainFrame*)AfxGetMainWnd())->m_pActiveCommandBar != pCommandBar)
	{
		((CMainFrame*)AfxGetMainWnd())->m_pActiveCommandBar = pCommandBar;

		((CMainFrame*)AfxGetMainWnd())->RefreshPanes();
	}
}

void CPaneToolbars::OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTREEVIEW* pNMTreeView = (NMTREEVIEW *)pNMHDR;

	if (m_bLockSelected)
		return;

	RefreshItem(pNMTreeView->itemNew.hItem);

	*pResult = 0;
}

void CPaneToolbars::OnSetFocus(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	HTREEITEM hItem = m_wndTreeCtrl.GetSelectedItem();
	if (hItem) RefreshItem(hItem);
}

void CPaneToolbars::OnUpdatePanetoolbarsDelete(CCmdUI* pCmdUI)
{
	HTREEITEM hItem = m_wndTreeCtrl.GetSelectedItem();

	if (hItem)
	{
		CXTPCommandBar* pCommandBar = (CXTPCommandBar*)m_wndTreeCtrl.GetItemData(hItem);

		pCmdUI->Enable((pCommandBar->GetType() == xtpBarTypeNormal) && (pCommandBar->GetPosition() != xtpBarPopup));
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CPaneToolbars::OnPanetoolbarsDelete()
{
	HTREEITEM hItem = m_wndTreeCtrl.GetSelectedItem();

	if (hItem)
	{
		CXTPCommandBar* pCommandBar = (CXTPCommandBar*)m_wndTreeCtrl.GetItemData(hItem);

		if ((pCommandBar->GetType() == xtpBarTypeNormal) && (pCommandBar->GetPosition() != xtpBarPopup))
		{

			((CMainFrame*)AfxGetMainWnd())->m_pActiveCommandBar = NULL;
			m_pActiveCommandBars->SetDragControl(NULL);

			pCommandBar->GetCommandBars()->Remove((CXTPToolBar*)pCommandBar);

			Refresh();
		}

	}
}
