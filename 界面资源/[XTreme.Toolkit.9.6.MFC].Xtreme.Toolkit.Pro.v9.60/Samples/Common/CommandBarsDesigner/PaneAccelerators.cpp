// PaneAccelerators.cpp : implementation file
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
#include "commandbarsdesigner.h"
#include "PaneAccelerators.h"

#include "DialogAccelProperties.h"

#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaneAccelerators

LRESULT CPaneAccelerators::CAcceleratorsList::WindowProc(UINT message,WPARAM wParam,LPARAM lParam)
{
	switch (message)
	{
	case WM_NCPAINT:
		{
			CListCtrl::WindowProc(message, wParam, lParam);

			CRect rc;
			GetWindowRect(&rc);
			CWindowDC dc(this);
			rc.OffsetRect(-rc.TopLeft());
			COLORREF clr = GetStaticFrameColor();
			dc.Draw3dRect(rc, clr, clr);
			return TRUE;
		}
	}
	return CListCtrl::WindowProc(message, wParam, lParam);
}

CPaneAccelerators::CPaneAccelerators()
{
	m_pActiveFrame = 0;
	m_lpAccel = NULL;
}

CPaneAccelerators::~CPaneAccelerators()
{
	if (m_lpAccel) delete[] m_lpAccel;
}


BEGIN_MESSAGE_MAP(CPaneAccelerators, CWnd)
	//{{AFX_MSG_MAP(CPaneAccelerators)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_PANEACELLERATORS_NEW, OnPaneacelleratorsNew)
	ON_UPDATE_COMMAND_UI(ID_PANEACELLERATORS_NEW, OnUpdatePaneacelleratorsNew)
	ON_COMMAND(ID_PANEACELLERATORS_DELETE, OnPaneacelleratorsDelete)
	ON_UPDATE_COMMAND_UI(ID_PANEACELLERATORS_DELETE, OnUpdatePaneacelleratorsDelete)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, AFX_IDW_PANE_FIRST, OnDblClick)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPaneAccelerators message handlers

int CPaneAccelerators::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	VERIFY(m_wndToolBar.CreateToolBar(WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS, this));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_PANE_ACCELERATORS));
	m_wndToolBar.GetControl(0)->SetStyle(xtpButtonIconAndCaption);
	m_wndToolBar.GetControl(1)->SetStyle(xtpButtonIconAndCaption);


	if (!m_wndListCtrl.CWnd::CreateEx(WS_EX_STATICEDGE, WC_LISTVIEW, NULL,
		WS_CHILD|WS_VISIBLE|LVS_REPORT|WS_VSCROLL|LVS_SINGLESEL|LVS_SHOWSELALWAYS,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST))
	{
		TRACE0("Failed to create tree control.\n");
		return -1;
	}
	m_wndListCtrl.ModifyStyleEx(0, WS_EX_STATICEDGE);

	m_wndListCtrl.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 105);
	m_wndListCtrl.InsertColumn(1, _T("Key"), LVCFMT_LEFT, 84);

	ListView_SetExtendedListViewStyleEx(m_wndListCtrl.m_hWnd,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	return 0;
}

void CPaneAccelerators::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);


	int nTop = 0;

	if (m_wndToolBar.GetSafeHwnd())
	{
		CSize sz = m_wndToolBar.CalcDockingLayout(cx, /*LM_HIDEWRAP|*/ LM_HORZDOCK|LM_HORZ | LM_COMMIT);

		m_wndToolBar.MoveWindow(0, nTop, cx, sz.cy);
		m_wndToolBar.Invalidate(FALSE);
		nTop += sz.cy;
	}


	if (m_wndListCtrl.GetSafeHwnd())
	{
		m_wndListCtrl.MoveWindow(0, nTop, cx, cy - nTop);
	}

}

void CPaneAccelerators::RefreshAccels(HACCEL& hAccelTable)
{
	CResourceManager* pResourceManager = ((CEmbeddedFrame*)m_pActiveFrame)->ResourceManager();

	m_wndListCtrl.DeleteAllItems();

	m_nAccelSize = ::CopyAcceleratorTable (hAccelTable, NULL, 0);

	if (m_nAccelSize <= 0)
		return;

	if (m_lpAccel != NULL ) delete[] m_lpAccel;

	m_lpAccel = new ACCEL [m_nAccelSize];
	::CopyAcceleratorTable (hAccelTable, m_lpAccel, m_nAccelSize);

	CString strFirst = _T("");

	for (int i = 0; i < m_nAccelSize; i ++)
	{
		CString strID = pResourceManager->GetStringID(m_lpAccel [i].cmd);
		ASSERT(!strID.IsEmpty());

		CString strKey;
		CXTPShortcutManager::CKeyHelper keyHelper(&m_lpAccel[i]);
		keyHelper.Format(strKey);


		int nIndex = m_wndListCtrl.InsertItem(m_wndListCtrl.GetItemCount(), strID, 0);
		m_wndListCtrl.SetItemText(nIndex, 1, strKey);
		m_wndListCtrl.SetItemData(nIndex, i);

	}

}

void CPaneAccelerators::Refresh(BOOL bForceAll /*= FALSE*/)
{
	if (bForceAll || (m_pActiveFrame != GetMainFrame()->GetActiveEmbeddedFrame()))
	{
		m_pActiveFrame = GetMainFrame()->GetActiveEmbeddedFrame();

		if (m_pActiveFrame)
		{
			RefreshAccels(m_pActiveFrame->m_hAccelTable);
		}
		else
		{
			m_wndListCtrl.DeleteAllItems();
		}
	}
}

BOOL CPaneAccelerators::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_CHAR || pMsg->message == WM_KEYUP)
	{
		if (pMsg->wParam == VK_INSERT) Insert();
		if (pMsg->wParam == VK_RETURN)
		{
			int nItem = m_wndListCtrl.GetNextItem(-1, LVNI_SELECTED);
			if (nItem != -1)
			{
				int nIndex = (int)m_wndListCtrl.GetItemData(nItem);
				Edit(nIndex);
			}
		}
		if (pMsg->wParam == VK_DELETE)
		{
			int nItem = m_wndListCtrl.GetNextItem(-1, LVNI_SELECTED);
			if (nItem != -1)
			{
				int nIndex = (int)m_wndListCtrl.GetItemData(nItem);
				Delete(nIndex);
			}
		}

		m_wndListCtrl.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CPaneAccelerators::Edit(int nIndex)
{
	if (!m_pActiveFrame)
		return;

	CResourceManager* pResourceManager = ((CEmbeddedFrame*)m_pActiveFrame)->ResourceManager();

	ACCEL& accel = m_lpAccel[nIndex];

	CDialogAccelProperties ap;
	ap.m_strId = pResourceManager->GetStringID(accel.cmd);

	ap.m_lpAccel = &accel;

	if (ap.DoModal() == IDOK)
	{
		accel = *ap.m_wndAccel.GetAccel();
		accel.cmd = (WORD)pResourceManager->GetStringID(ap.m_strId);

		DestroyAcceleratorTable(m_pActiveFrame->m_hAccelTable);
		m_pActiveFrame->m_hAccelTable = CreateAcceleratorTable(m_lpAccel, m_nAccelSize);

		RefreshAccels(m_pActiveFrame->m_hAccelTable);

	}
}

void CPaneAccelerators::Delete(int nIndex)
{
	if (!m_pActiveFrame)
		return;

	ASSERT(m_lpAccel);

	if (nIndex < m_nAccelSize - 1)
	{
		m_lpAccel[nIndex] = m_lpAccel[m_nAccelSize - 1];
	}

	DestroyAcceleratorTable(m_pActiveFrame->m_hAccelTable);
	m_pActiveFrame->m_hAccelTable = CreateAcceleratorTable(m_lpAccel, m_nAccelSize - 1);

	RefreshAccels(m_pActiveFrame->m_hAccelTable);
}

void CPaneAccelerators::Insert()
{
	if (!m_pActiveFrame)
		return;

	CResourceManager* pResourceManager = ((CEmbeddedFrame*)m_pActiveFrame)->ResourceManager();

	CDialogAccelProperties ap;
	if (ap.DoModal() == IDOK)
	{
		HACCEL& hAccelTable = m_pActiveFrame->m_hAccelTable;

		m_nAccelSize = ::CopyAcceleratorTable (hAccelTable, NULL, 0);

		ASSERT(m_lpAccel);

		if (m_lpAccel != NULL ) delete[] m_lpAccel;

		m_lpAccel = new ACCEL [m_nAccelSize + 1];
		::CopyAcceleratorTable (hAccelTable, m_lpAccel, m_nAccelSize);


		m_lpAccel[m_nAccelSize] = *ap.m_wndAccel.GetAccel();
		m_lpAccel[m_nAccelSize].cmd = (WORD)pResourceManager->GetStringID(ap.m_strId);

		m_nAccelSize = m_nAccelSize + 1;

		DestroyAcceleratorTable(m_pActiveFrame->m_hAccelTable);
		m_pActiveFrame->m_hAccelTable = CreateAcceleratorTable(m_lpAccel, m_nAccelSize);


		RefreshAccels(m_pActiveFrame->m_hAccelTable);
	}

}

void CPaneAccelerators::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMITEMACTIVATE * pNMItemActivate = (NMITEMACTIVATE  *)pNMHDR;

	if (!m_pActiveFrame)
		return;

	ASSERT(m_lpAccel);

	if (pNMItemActivate->iItem >= 0)
	{

		int nIndex = (int)m_wndListCtrl.GetItemData(pNMItemActivate->iItem);
		Edit(nIndex);
	}

	*pResult = 0;
}

void CPaneAccelerators::OnPaneacelleratorsNew()
{
	Insert();

}

void CPaneAccelerators::OnUpdatePaneacelleratorsNew(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pActiveFrame != NULL);
}

void CPaneAccelerators::OnPaneacelleratorsDelete()
{
	int nItem = m_wndListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if (nItem != -1)
	{
		int nIndex = (int)m_wndListCtrl.GetItemData(nItem);
		Delete(nIndex);
	}

}

void CPaneAccelerators::OnUpdatePaneacelleratorsDelete(CCmdUI* pCmdUI)
{
	int nItem = m_wndListCtrl.GetNextItem(-1, LVNI_SELECTED);
	pCmdUI->Enable(nItem != -1);

}
