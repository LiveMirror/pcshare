// PaneControls.cpp : implementation file
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
#include "PaneControls.h"
#include "DialogNewControl.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_PANECONTROLS_COMBO 1010


void CPaneControls::CControlsListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rc(lpDrawItemStruct->rcItem);
	CXTPControl* pControl = (CXTPControl*)lpDrawItemStruct->itemData;
	if (pControl)
	{
		BOOL bSelected = ((lpDrawItemStruct->itemAction | ODA_SELECT) && (lpDrawItemStruct->itemState & ODS_SELECTED));
		XTPPaintManager()->DrawListBoxControl(pDC, pControl, rc, bSelected, TRUE);
	}
}

BEGIN_MESSAGE_MAP(CPaneControls::CControlsListBox, CListBox)
	//{{AFX_MSG_MAP(CXTPCustomizeCommandsPage)
	{ WM_LBUTTONDOWN, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(AFX_PMSGW) (static_cast< void (AFX_MSG_CALL CWnd::*)(UINT, CPoint) > (CPaneControls::CControlsListBox::OnLButtonDown)) },
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


LRESULT CPaneControls::CControlsListBox::WindowProc(UINT message,WPARAM wParam,LPARAM lParam)
{
	switch (message)
	{
	case WM_NCPAINT:
		{
			CListBox::WindowProc(message, wParam, lParam);

			CRect rc;
			GetWindowRect(&rc);
			CWindowDC dc(this);
			rc.OffsetRect(-rc.TopLeft());
			COLORREF clr = GetStaticFrameColor();
			dc.Draw3dRect(rc, clr, clr);
			return TRUE;
		}

	case WM_PAINT:
		{
			CPaintDC dc(this);
			CXTPClientRect rc(this);
			CXTPBufferDC memDC(dc, rc);
			XTPPaintManager()->DrawListBoxControl(&memDC, NULL, CXTPClientRect(this), FALSE, TRUE);
			return CListBox::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
		}

	case WM_KEYDOWN:
		if( GetKeyState(VK_CONTROL) & 0x8000 )
		{
			if( wParam == VK_UP )
			{
				GetOwner()->SendMessage(WM_COMMAND, ID_PANECONTROLS_UP);
				return TRUE;
			}

			if( wParam == VK_DOWN )
			{
				GetOwner()->SendMessage(WM_COMMAND, ID_PANECONTROLS_DOWN );
				return TRUE;
			}

		}
		break;

	case WM_ERASEBKGND:
		CDC* pDC = CDC::FromHandle((HDC)wParam);
		XTPPaintManager()->DrawListBoxControl(pDC, NULL, CXTPClientRect(this), FALSE, TRUE);
		return TRUE;
	}

	return CListBox::WindowProc(message, wParam, lParam);
}


void CPaneControls::CControlsListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	CXTPCommandBars* pCommandBars = m_pPaneControls->m_pActiveCommandBars;
	if (!pCommandBars)
	{
		CListBox::OnLButtonDown(nFlags, point);
		return;
	}


	pCommandBars->SetDragControl(NULL);

	BOOL bOutside;
	int nIndex = ( int )ItemFromPoint( point, bOutside );
	CXTPControl* pControl = NULL;
	if (!bOutside)
	{
		if ( SetCurSel( nIndex ) != LB_ERR )
		{
			// Already selected, start drag operation
			pControl = ( CXTPControl* )GetItemData( nIndex );

			// since we are eating the message, if the selection has changed, notify the owner.
			CWnd* pWndOwner = GetOwner();
			if ( pWndOwner )
			{
				SetFocus();

				pWndOwner->PostMessage( WM_COMMAND, MAKEWPARAM( GetDlgCtrlID( ),
					LBN_SELCHANGE ), ( LPARAM )m_hWnd );
			}

			if (pControl)
			{
				pCommandBars->SetDragControl(pControl);

				CXTPCustomizeDropSource* pDropSource = pCommandBars->GetDropSource();
				pDropSource->DoDragDrop(pControl, TRUE);

				return;
			}
		}
	}

	// process as a click first
	CListBox::OnLButtonDown(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// CPaneControls

CPaneControls::CPaneControls()
{
	m_pActiveCommandBars = 0;
	m_pControls = new CXTPControls();
	m_wndListBox.m_pPaneControls = this;
}

CPaneControls::~CPaneControls()
{
	delete m_pControls;
}


BEGIN_MESSAGE_MAP(CPaneControls, CWnd)
	//{{AFX_MSG_MAP(CPaneControls)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_PANECONTROLS_ADD, OnPanecontrolsAdd)
	ON_UPDATE_COMMAND_UI(ID_PANECONTROLS_ADD, OnUpdatePanecontrolsAdd)
	ON_UPDATE_COMMAND_UI(ID_PANECONTROLS_UP, OnUpdatePanecontrolsUp)
	ON_COMMAND(ID_PANECONTROLS_UP, OnPanecontrolsUp)
	ON_COMMAND(ID_PANECONTROLS_DOWN, OnPanecontrolsDown)
	ON_UPDATE_COMMAND_UI(ID_PANECONTROLS_DOWN, OnUpdatePanecontrolsDown)
	ON_COMMAND(ID_PANECONTROLS_DELETE, OnPanecontrolsDelete)
	ON_UPDATE_COMMAND_UI(ID_PANECONTROLS_DELETE, OnUpdatePanecontrolsDelete)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(ID_PANECONTROLS_COMBO, OnCategoryChanged)
	ON_LBN_SELCHANGE(AFX_IDW_PANE_FIRST, OnControlChanged)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPaneControls message handlers

int CPaneControls::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	VERIFY(m_wndToolBar.CreateToolBar(WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS, this));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_PANE_CONTROLS));


	VERIFY(m_wndListBox.Create(LBS_OWNERDRAWFIXED|LBS_NOINTEGRALHEIGHT|LBS_NOTIFY |WS_VSCROLL|WS_VISIBLE|WS_CHILD, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST ));

	m_wndListBox.ModifyStyleEx(0, WS_EX_STATICEDGE);

	CSize sz = XTPPaintManager()->DrawListBoxControl(NULL, NULL, CRect(0, 0, 0, 0), FALSE, FALSE);
	m_wndListBox.SetItemHeight(-1, sz.cy);

	CFont* pFont = XTPPaintManager()->GetRegularFont();
	LOGFONT lf = {sizeof(LOGFONT)};
	pFont->GetLogFont(&lf);
	m_fntRegular.CreateFontIndirect(&lf);

	m_wndListBox.SetFont(&m_fntRegular);

	m_wndCategories.Create(WS_VISIBLE|WS_CHILD|CBS_DROPDOWNLIST, CRect(0, 0, 0, 200), this, ID_PANECONTROLS_COMBO);
	m_wndCategories.SetFont(&m_fntRegular);


	return 0;
}

void CPaneControls::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	int nTop = 0;

	if (m_wndCategories.GetSafeHwnd())
	{
		CRect rc;
		m_wndCategories.GetWindowRect(&rc);

		m_wndCategories.MoveWindow(0, 0, cx, rc.Height());
		nTop += rc.Height();
	}

	if (m_wndToolBar.GetSafeHwnd())
	{
		CSize sz = m_wndToolBar.CalcDockingLayout(cx, /*LM_HIDEWRAP|*/ LM_HORZDOCK|LM_HORZ | LM_COMMIT);

		m_wndToolBar.MoveWindow(0, nTop, cx, sz.cy);
		m_wndToolBar.Invalidate(FALSE);
		nTop += sz.cy;
	}
	if (m_wndListBox.GetSafeHwnd())
	{
		m_wndListBox.MoveWindow(0, nTop, cx, cy - nTop);
		m_wndListBox.Invalidate(FALSE);
	}

}

void CPaneControls::Refresh(BOOL bForce)
{
	CXTPCommandBars* pCommandBars = ((CMainFrame*)AfxGetMainWnd())->GetActiveCommandBars();



	if (m_wndListBox.GetSafeHwnd() == 0 )
	{
		return;
	}

	if (m_pActiveCommandBars != pCommandBars || bForce)
	{
		m_pActiveCommandBars = pCommandBars;

		CXTPControl* pControlSelected = NULL;
		int nIndex = m_wndListBox.GetCurSel();
		if ( nIndex != LB_ERR )
		{
			pControlSelected = ( CXTPControl* )m_wndListBox.GetItemData( nIndex );
		}



		m_wndCategories.ResetContent();

		if (m_pActiveCommandBars != 0)
		{
			m_wndCategories.AddString(_T("All Controls"));

			CXTPControls* pControls = ((CMainFrame*)AfxGetMainWnd())->GetActiveEmbeddedFrame()->m_pControls;

			int nTopIndex = m_wndListBox.GetTopIndex();


			m_wndListBox.LockWindowUpdate();
			m_wndListBox.ResetContent();

			for (int i = 0; i < pControls->GetCount(); i++)
			{
				CXTPControl* pControl = pControls->GetAt(i);
				CString strCategory = pControl->GetCategory();
				if (strCategory.IsEmpty()) strCategory = _T("[Empty]");

				if (m_strSelectedCategory == _T("") || m_strSelectedCategory == strCategory)
				{
					int nIndex = (int)m_wndListBox.SendMessage(LB_INSERTSTRING, m_wndListBox.GetCount(), (LPARAM)pControl);
					if (pControlSelected == pControl)
					{
						m_wndListBox.SetCurSel(nIndex);
					}
				}

				if (m_wndCategories.FindStringExact(0, strCategory) == CB_ERR)
				{
					m_wndCategories.AddString(strCategory);
				}
			}
			m_wndListBox.UnlockWindowUpdate();

			m_wndListBox.SetTopIndex(nTopIndex);

			if (m_wndListBox.GetCurSel() == LB_ERR)
			{
				m_wndListBox.SetCurSel(nIndex == m_wndListBox.GetCount()? nIndex - 1: nIndex);
				OnControlChanged();
			}

			int nIndex = m_wndCategories.FindStringExact(0, m_strSelectedCategory);
			m_wndCategories.SetCurSel(nIndex);

			if (nIndex == CB_ERR && m_wndCategories.GetCount() > 0)
			{
				m_wndCategories.SetCurSel(0);
				if (!m_strSelectedCategory.IsEmpty()) OnCategoryChanged();
			}


		}
		else
		{
			m_wndListBox.ResetContent();
		}
	}
}

void CPaneControls::OnPanecontrolsAdd()
{
	CDialogNewControl dc;

	CEmbeddedFrame* pFrame = GetMainFrame()->GetActiveEmbeddedFrame();
	if (!pFrame)
		return;

	if (!m_strSelectedCategory.IsEmpty())
		dc.m_strCategory = m_strSelectedCategory;

	if (dc.DoModal() == IDOK)
	{
		XTPControlType type = xtpControlButton;

		CString strType = dc.m_strType;

		for (int i = 1; i < _countof(lpTypes); i++)
		{
			if (lpTypes[i] && strType == lpTypes[i])
			{
				type = (XTPControlType)i;
				break;
			}
		}

		CString strID = dc.m_strId;
		UINT nID = pFrame->ResourceManager()->GetStringID(strID);

		CXTPControl* pControl = pFrame->m_pControls->Add(type, nID);
		pControl->SetCaption(dc.m_strCaption);
		pControl->SetCategory(dc.m_strCategory);

		CString strToolTip = dc.m_strCaption;
		CXTPPaintManager::StripMnemonics(strToolTip);
		pControl->SetTooltip(strToolTip);



		int nIndex = (int)m_wndListBox.SendMessage(LB_INSERTSTRING, m_wndListBox.GetCount(), (LPARAM)pControl);
		m_wndListBox.SetCurSel(nIndex);
		pFrame->GetCommandBars()->SetDragControl(pControl);
		GetMainFrame()->RefreshPanes(TRUE);
	}

}

void CPaneControls::OnUpdatePanecontrolsAdd(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pActiveCommandBars != 0);

}


void CPaneControls::OnCategoryChanged()
{
	int nIndex = m_wndCategories.GetCurSel();
	if (nIndex > 0)
	{
		m_wndCategories.GetLBText(nIndex, m_strSelectedCategory);
	}
	else
	{
		m_strSelectedCategory = _T("");
	}
	Refresh(TRUE);

}

void CPaneControls::OnControlChanged()
{
	CXTPCommandBars* pCommandBars = m_pActiveCommandBars;
	if (!pCommandBars)
	{
		return;
	}

	int nIndex = m_wndListBox.GetCurSel();
	if ( nIndex != LB_ERR )
	{
		CXTPControl* pControl = ( CXTPControl* )m_wndListBox.GetItemData( nIndex );

		if (pControl)
		{
			pCommandBars->SetDragControl(pControl);
		}

	}

}

void CPaneControls::OnUpdatePanecontrolsUp(CCmdUI* pCmdUI)
{
	int nIndex = m_wndListBox.GetCurSel();
	pCmdUI->Enable(nIndex > 0);

}

void CPaneControls::OnUpdatePanecontrolsDown(CCmdUI* pCmdUI)
{
	int nIndex = m_wndListBox.GetCurSel();
	pCmdUI->Enable(nIndex != CB_ERR && nIndex < m_wndListBox.GetCount() - 1);
}


void CPaneControls::OnPanecontrolsUp()
{
	int nIndex = m_wndListBox.GetCurSel();
	if (nIndex > 0)
	{
		CXTPControl* pControl = ( CXTPControl* )m_wndListBox.GetItemData( nIndex );
		CXTPControl* pControlBefore = ( CXTPControl* )m_wndListBox.GetItemData( nIndex - 1);

		pControl->GetControls()->MoveBefore(pControl, pControlBefore->GetIndex());
		Refresh(TRUE);
	}
}

void CPaneControls::OnPanecontrolsDown()
{
	int nIndex = m_wndListBox.GetCurSel();
	if (nIndex != CB_ERR && nIndex < m_wndListBox.GetCount() - 1)
	{
		CXTPControl* pControl = ( CXTPControl* )m_wndListBox.GetItemData( nIndex );
		CXTPControl* pControlBefore = ( CXTPControl* )m_wndListBox.GetItemData( nIndex + 1);

		pControl->GetControls()->MoveBefore(pControl, pControlBefore->GetIndex() + 1);
		Refresh(TRUE);
	}

}


void CPaneControls::OnPanecontrolsDelete()
{
	int nIndex = m_wndListBox.GetCurSel();
	if (nIndex != CB_ERR && m_pActiveCommandBars)
	{
		CXTPControl* pControl = ( CXTPControl* )m_wndListBox.GetItemData( nIndex );

		m_pActiveCommandBars->SetDragControl(NULL);

		pControl->GetControls()->Remove(pControl);
		Refresh(TRUE);
	}
}

void CPaneControls::OnUpdatePanecontrolsDelete(CCmdUI* pCmdUI)
{
	int nIndex = m_wndListBox.GetCurSel();
	pCmdUI->Enable(nIndex != CB_ERR);
}
