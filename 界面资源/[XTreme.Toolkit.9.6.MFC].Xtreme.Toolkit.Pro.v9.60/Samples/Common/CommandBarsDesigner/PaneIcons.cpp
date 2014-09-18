// PaneIcons.cpp : implementation file
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
#include "PaneIcons.h"
#include "MainFrm.h"
#include "DialogNewIcon.h"
#include "DialogBitmapExport.h"
#include "DialogBitmapImport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_PANEICONS_COMBO 1011





BEGIN_MESSAGE_MAP(CControlIconsView, CWnd)
	//{{AFX_MSG_MAP(CControlIconsView)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


LRESULT CControlIconsView::WindowProc(UINT message,WPARAM wParam,LPARAM lParam)
{
	switch (message)
	{
	case WM_NCPAINT:
		{
			CWnd::WindowProc(message, wParam, lParam);

			CRect rc;
			GetWindowRect(&rc);
			CWindowDC dc(this);
			rc.OffsetRect(-rc.TopLeft());
			COLORREF clr = GetStaticFrameColor();
			dc.Draw3dRect(rc, clr, clr);
			return TRUE;
		}
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CControlIconsView construction/destruction

CControlIconsView::CControlIconsView()
{
	m_pSelected = NULL;
	m_pIconSet = NULL;

	m_bScrollVisible[SB_VERT] = FALSE;
	m_bScrollVisible[SB_HORZ] = FALSE;

}

CControlIconsView::~CControlIconsView()
{
}


#define THUMB_WIDTH (90 - 20)
#define THUMB_HEIGHT (100 - 20)
#define INFO_HEIGHT 15


void CControlIconsViewItem::Draw(CDC* pDC, BOOL bSelected, int x, int y, CSize sz)
{
	CRect rcPicture(x, y, x + sz.cx, y + sz.cy - INFO_HEIGHT);
	CRect rcInfo(CPoint(x, y + sz.cy - INFO_HEIGHT), CSize(sz.cx, INFO_HEIGHT));

	COLORREF clrText = GetXtremeColor(COLOR_BTNTEXT);

	if (!bSelected)
	{
		pDC->FillSolidRect(rcPicture, GetXtremeColor(COLOR_3DFACE));
		pDC->Draw3dRect(rcPicture, GetXtremeColor(COLOR_APPWORKSPACE), GetXtremeColor(COLOR_3DDKSHADOW));

		pDC->FillSolidRect(rcInfo, /*RGB(237, 237, 202)*/GetXtremeColor(XPCOLOR_TAB_INACTIVE_BACK));
		pDC->Draw3dRect(rcInfo, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_BTNHIGHLIGHT));
	} else
	{
		pDC->FillSolidRect(rcPicture, GetXtremeColor(COLOR_HIGHLIGHT));
		pDC->Draw3dRect(rcPicture, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DDKSHADOW));

		pDC->FillSolidRect(rcInfo, GetXtremeColor(COLOR_HIGHLIGHT));
		pDC->Draw3dRect(rcInfo, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_BTNHIGHLIGHT));
		pDC->DrawFocusRect(rcInfo);

		clrText = GetXtremeColor(COLOR_HIGHLIGHTTEXT);
	}

	CString str = imageState == xtpImageNormal? _T("Normal"): imageState == xtpImageChecked? _T("Checked"):
		imageState == xtpImageHot? _T("Hot"): _T("Disabled");

	pDC->SetTextColor(clrText);
	pDC->DrawText(str, rcInfo, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER);

	CSize szDevice = pDC->GetTextExtent(_T("xxx"));

	CRect rcDevice(rcPicture);
	rcDevice.top = rcPicture.bottom - szDevice.cy - 4;

	str.Format(_T("%ix%i"), pIcon->GetWidth(), pIcon->GetHeight());
	pDC->DrawText(str, rcDevice, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER);

	CRect rcImage (rcPicture);
	rcImage.DeflateRect(4, 4, 4, rcDevice.Height());

	CXTPImageManagerIconHandle& hIcon = GetIcon();
	ASSERT(!hIcon.IsEmpty());
	{
		CSize sz(min(pIcon->GetWidth(), rcImage.Width()), min(pIcon->GetHeight(), rcImage.Height()));
		CRect rcImageRect (CPoint(rcImage.CenterPoint().x - sz.cx /2, rcImage.CenterPoint().y - sz.cy /2), sz);

		pIcon->Draw(pDC, rcImageRect.TopLeft(), hIcon);

		rcImageRect.InflateRect(1, 1);
		pDC->Draw3dRect(rcImageRect, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_BTNHIGHLIGHT));
	}

}

CControlIconsViewItem* CControlIconsView::HitTest(CPoint pt)
{
	POSITION pos = m_lstItems.GetHeadPosition();
	while (pos)
	{
		CControlIconsViewItem& item = m_lstItems.GetNext(pos);

		CRect rc(CPoint(- GetScrollOffset(SB_HORZ) + 3 + item.ptPosition.x * THUMB_WIDTH,  - GetScrollOffset(SB_VERT) + 3 + item.ptPosition.y * THUMB_HEIGHT), CSize(THUMB_WIDTH - 5, THUMB_HEIGHT - 5));

		if (rc.PtInRect(pt))
			return &item;
	}
	return NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CControlIconsView drawing

void CControlIconsView::OnPaint()
{
	CPaintDC dcPaint(this);

	CXTPClientRect rc(this);

	CXTPBufferDC dc(dcPaint, rc);
	dc.FillSolidRect(CXTPClientRect(this), GetXtremeColor(COLOR_APPWORKSPACE));

	CXTPFontDC fnt(&dc, XTPPaintManager()->GetRegularFont());
	dc.SetBkMode(TRANSPARENT);


	POSITION pos = m_lstItems.GetHeadPosition();
	while (pos)
	{
		CControlIconsViewItem& item = m_lstItems.GetNext(pos);

		item.Draw(&dc, m_pSelected == &item, - GetScrollOffset(SB_HORZ) + 3 + item.ptPosition.x * THUMB_WIDTH, - GetScrollOffset(SB_VERT) + 3 + item.ptPosition.y * THUMB_HEIGHT, CSize(THUMB_WIDTH - 5, THUMB_HEIGHT - 5));

	}
}

void CControlIconsView::RefreshIcons(CXTPImageManagerIconSet* pIconSet)
{
	DeleteAllItems();

	CXTPImageManagerIconSet::CIconSetMap* pMapIcons = pIconSet->GetIcons();
	m_pIconSet = pIconSet;
	m_pSelected = NULL;

	POSITION pos = pMapIcons->GetStartPosition();
	UINT nWidth;
	CXTPImageManagerIcon* pIcon;

	int nCount = 0;

	while (pos)
	{
		pMapIcons->GetNextAssoc(pos, nWidth, pIcon);

		if (!pIcon->IsScaled())
		{

			CControlIconsViewItem itemNormal(nCount, 0, xtpImageNormal, pIcon);
			CControlIconsViewItem itemHot(nCount, 1, xtpImageHot, pIcon);
			CControlIconsViewItem itemDisabled(nCount, 2, xtpImageDisabled, pIcon);
			CControlIconsViewItem itemChecked(nCount, 3, xtpImageChecked, pIcon);

			m_lstItems.AddTail(itemNormal);
			m_lstItems.AddTail(itemHot);
			m_lstItems.AddTail(itemDisabled);
			m_lstItems.AddTail(itemChecked);

			nCount++;
		}
	}
	UpdateScrollBars();


	Invalidate(FALSE);
}

void CControlIconsView::UpdateScrollBars()
{
	int nCount = (int)m_lstItems.GetCount() / 4;

	int nHeight = 3 + nCount * THUMB_HEIGHT;
	CXTPClientRect rc(this);

	m_bScrollVisible[SB_VERT] = rc.Height() < nHeight;

	ShowScrollBar(SB_VERT, m_bScrollVisible[SB_VERT]);

	if (m_bScrollVisible[SB_VERT])
	{
		SCROLLINFO  si ;

		si.cbSize = sizeof(SCROLLINFO) ;
		si.fMask = SIF_PAGE | SIF_RANGE ;


		si.nPage = rc.Height() ;
		si.nMax = nHeight - 1;
		si.nMin = 0 ;
		SetScrollInfo(SB_VERT, &si) ;

		EnableScrollBarCtrl(SB_VERT, TRUE);
	}


	int nWidth = m_lstItems.GetCount() > 0? 3 + 4 * THUMB_WIDTH: 0;

	m_bScrollVisible[SB_HORZ] = rc.Width() < nWidth;

	ShowScrollBar(SB_HORZ, m_bScrollVisible[SB_HORZ]);

	if (m_bScrollVisible[SB_HORZ])
	{
		SCROLLINFO  si ;

		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_PAGE | SIF_RANGE ;


		si.nPage = rc.Width();
		si.nMax = nWidth - 1;
		si.nMin = 0 ;
		SetScrollInfo(SB_HORZ, &si) ;

		EnableScrollBarCtrl(SB_HORZ, TRUE);
	}

}

void CControlIconsView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	UpdateScrollBars();
}

void CControlIconsView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// its horizontal scroll bar
	int nCurPos = GetScrollPos(SB_VERT);

	// decide what to do for each diffrent scroll event
	switch(nSBCode)
	{
		case SB_LEFT: nCurPos = 0; break;
		case SB_RIGHT: nCurPos = GetScrollLimit(SB_VERT); break;
		case SB_LINELEFT: nCurPos = max(nCurPos - 6, 0); break;
		case SB_LINERIGHT: nCurPos = min(nCurPos + 6, GetScrollLimit(SB_VERT)); break;
		case SB_PAGELEFT: nCurPos = max(nCurPos - CXTPClientRect(this).Height(), 0); break;
		case SB_PAGERIGHT: nCurPos = min(nCurPos + CXTPClientRect(this).Height(), GetScrollLimit(SB_VERT)); break;

		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:  nCurPos = nPos; break;
	}

	SetScrollPos(SB_VERT, nCurPos);
	Invalidate(FALSE);

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CControlIconsView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// its horizontal scroll bar
	int nCurPos = GetScrollPos(SB_HORZ);

	// decide what to do for each diffrent scroll event
	switch(nSBCode)
	{
		case SB_LEFT: nCurPos = 0; break;
		case SB_RIGHT: nCurPos = GetScrollLimit(SB_HORZ); break;
		case SB_LINELEFT: nCurPos = max(nCurPos - 6, 0); break;
		case SB_LINERIGHT: nCurPos = min(nCurPos + 6, GetScrollLimit(SB_HORZ)); break;
		case SB_PAGELEFT: nCurPos = max(nCurPos - CXTPClientRect(this).Width(), 0); break;
		case SB_PAGERIGHT: nCurPos = min(nCurPos + CXTPClientRect(this).Width(), GetScrollLimit(SB_HORZ)); break;

		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:  nCurPos = nPos; break;
	}

	SetScrollPos(SB_HORZ, nCurPos);
	Invalidate(FALSE);

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

int CControlIconsView::GetScrollOffset(int nBar)
{
	if (!m_bScrollVisible[nBar])
		return 0;

	return GetScrollPos(nBar);
}

/////////////////////////////////////////////////////////////////////////////
// CControlIconsView message handlers

BOOL CControlIconsView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CControlIconsView::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	CControlIconsViewItem* pSelected = HitTest(point);

	if (pSelected != m_pSelected)
	{
		m_pSelected = pSelected;
		Invalidate(FALSE);
	}
	SetFocus();
}

void CControlIconsView::Edit()
{
	if (!m_pSelected)
		return;

	CXTPImageEditorDlg id;
	id.SetIcon(m_pSelected->GetIcon());
	if (id.DoModal() == IDOK)
	{
		CXTPImageManagerIconHandle iconHandle;
		id.GetIcon(&iconHandle);

		switch(m_pSelected->imageState)
		{
		case xtpImageNormal: m_pSelected->pIcon->SetNormalIcon(iconHandle); break;
		case xtpImageHot: m_pSelected->pIcon->SetHotIcon(iconHandle); break;
		case xtpImageChecked: m_pSelected->pIcon->SetCheckedIcon(iconHandle); break;
		case xtpImageDisabled: m_pSelected->pIcon->SetDisabledIcon(iconHandle); break;
		}


		m_pPaneIcons->GetImageManager()->RefreshAll();


		m_pPaneIcons->SetModified();
		m_pPaneIcons->RefreshAll();
	}
}

void CControlIconsView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_pSelected)
	{
		Edit();
		return;

	}

	CWnd::OnLButtonDblClk(nFlags, point);
}









/////////////////////////////////////////////////////////////////////////////
// CPaneIcons

CPaneIcons::CPaneIcons()
{
	m_wndIconsView.m_pPaneIcons = this;
	m_pActiveIcons = NULL;
	m_pLastControl = NULL;
}

CPaneIcons::~CPaneIcons()
{
}


BEGIN_MESSAGE_MAP(CPaneIcons, CWnd)
	//{{AFX_MSG_MAP(CPaneIcons)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_PANEICONS_CLEAR, OnPaneiconsClear)
	ON_UPDATE_COMMAND_UI(ID_PANEICONS_CLEAR, OnUpdatePaneiconsClear)
	ON_COMMAND(ID_PANEICONS_NEW, OnPaneiconsNew)
	ON_UPDATE_COMMAND_UI(ID_PANEICONS_NEW, OnUpdatePaneiconsNew)
	ON_COMMAND(ID_PANEICONS_EDIT, OnPaneiconsEdit)
	ON_UPDATE_COMMAND_UI(ID_PANEICONS_EDIT, OnUpdatePaneiconsEdit)
	ON_UPDATE_COMMAND_UI(ID_PANEICONS_IMPORT_AND_EXPORT, OnUpdatePaneiconsImportAndExport)
	ON_COMMAND(ID_IMPORTANDEXPORT_EXPORTTOXML, OnImportandexportExporttoxml)
	ON_COMMAND(ID_IMPORTANDEXPORT_IMPORTFROMXML, OnImportandexportImportfromxml)
	ON_UPDATE_COMMAND_UI(ID_IMPORTANDEXPORT_EXPORTTOXML, OnUpdateXMLCommand)
	ON_UPDATE_COMMAND_UI(ID_IMPORTANDEXPORT_IMPORTFROMXML, OnUpdateXMLCommand)


	ON_COMMAND(ID_IMPORTANDEXPORT_EXPORTTOBITMAP, OnImportandexportExporttobitmap)
	ON_COMMAND(ID_IMPORTANDEXPORT_IMPORTFROMBITMAP, OnImportandexportImportfrombitmap)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(ID_PANEICONS_COMBO, OnComboChanged)
END_MESSAGE_MAP()

CString CPaneIcons::GetStringID(int nID)
{
	return m_pActiveFrame->ResourceManager()->GetStringID(nID);
}


void CPaneIcons::RefreshIcons()
{
	//m_wndListBox.ResetContent();
	m_wndCombo.ResetContent();
	m_wndIconsView.DeleteAllItems();
	m_wndIconsView.m_pIconSet = NULL;
	m_wndIconsView.m_pSelected = NULL;

	if (GetImageManager() == NULL)
	{
		m_wndIconsView.Invalidate(FALSE);
		return;
	}


	CMap<UINT, UINT, CXTPImageManagerIconSet*, CXTPImageManagerIconSet*>* pIcons = GetImageManager()->GetImages();
	UINT nIDCommand;
	CXTPImageManagerIconSet* pIconSet;

	POSITION pos = pIcons->GetStartPosition();
	while (pos)
	{
		pIcons->GetNextAssoc(pos, nIDCommand, pIconSet);

		//if (nIDCommand >= 1000 && nIDCommand < 4000)
		{
			if (pIconSet)
			{
				CString str = GetStringID(pIconSet->GetID());

				int nIndex = m_wndCombo.AddString(str);
				m_wndCombo.SetItemData(nIndex, (DWORD_PTR)pIconSet);


				if (m_pActiveIcons == pIconSet)
				{

					m_wndCombo.SetCurSel(nIndex);
					m_wndIconsView.RefreshIcons(pIconSet);

				}
			}

		}
	}
	m_wndIconsView.Invalidate(FALSE);
}
/////////////////////////////////////////////////////////////////////////////
// CPaneIcons message handlers

CXTPImageManager* CPaneIcons::GetImageManager()
{
	return m_pActiveFrame? m_pActiveFrame->GetCommandBars()->GetImageManager(): NULL;
}

int CPaneIcons::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	VERIFY(m_wndToolBar.CreateToolBar(WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS, this));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_PANE_ICONS));
	m_wndToolBar.GetControls()->FindControl(xtpControlButton, ID_PANEICONS_EDIT, TRUE, FALSE)
		->SetStyle(xtpButtonIconAndCaption);

	CXTPControlPopup* pControl = (CXTPControlPopup*)m_wndToolBar.GetControls()->SetControlType(3, xtpControlButtonPopup);
	pControl->SetStyle(xtpButtonCaption);
	CMenu menu;
	menu.LoadMenu(IDR_PANE_ICONS_IMPORT_AND_EXPORT);
	pControl->GetCommandBar()->LoadMenu(menu.GetSubMenu(0));



	if (!m_wndIconsView.CWnd::CreateEx(WS_EX_STATICEDGE, AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(0, IDC_ARROW)), NULL,
		WS_CHILD|WS_VISIBLE|SS_NOTIFY, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST))
	{
		TRACE0("Failed to create tree control.\n");
		return -1;
	}
	m_wndIconsView.ModifyStyleEx(0, WS_EX_STATICEDGE);

	CFont* pFont = XTPPaintManager()->GetRegularFont();
	LOGFONT lf = {sizeof(LOGFONT)};
	pFont->GetLogFont(&lf);
	m_fntRegular.CreateFontIndirect(&lf);


	m_wndCombo.Create(WS_VISIBLE|WS_CHILD|CBS_DROPDOWNLIST, CRect(0, 0, 0, 200), this, ID_PANEICONS_COMBO);
	m_wndCombo.SetFont(&m_fntRegular);



	return 0;
}

void CPaneIcons::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	int nTop = 0;

	if (m_wndCombo.GetSafeHwnd())
	{
		CRect rc;
		m_wndCombo.GetWindowRect(&rc);

		m_wndCombo.MoveWindow(0, 0, cx, rc.Height());
		nTop += rc.Height();
	}
	if (m_wndToolBar.GetSafeHwnd())
	{
		CSize sz = m_wndToolBar.CalcDockingLayout(cx, /*LM_HIDEWRAP|*/ LM_HORZDOCK|LM_HORZ | LM_COMMIT);

		m_wndToolBar.MoveWindow(0, nTop, cx, sz.cy);
		m_wndToolBar.Invalidate(FALSE);
		nTop += sz.cy;
	}

	if (m_wndIconsView.GetSafeHwnd())
	{
		m_wndIconsView.MoveWindow(0, nTop, cx, cy - nTop);
		m_wndIconsView.Invalidate(FALSE);
	}
}

void CPaneIcons::SetModified()
{
	if (m_pActiveFrame)
	{
		m_pActiveFrame->GetCommandBars()->RedrawCommandBars();
	}
}
void CPaneIcons::RefreshAll()
{
	GetMainFrame()->RefreshPanes(TRUE);
}

void CPaneIcons::Refresh(BOOL bForce)
{

	if ((m_pActiveFrame != GetMainFrame()->GetActiveEmbeddedFrame()) || bForce)
	{
		m_pActiveFrame = GetMainFrame()->GetActiveEmbeddedFrame();
		RefreshIcons();
		return;
	}


	if (m_pActiveFrame)
	{
		CXTPControl* pControl = m_pActiveFrame->GetCommandBars()->GetDragControl();
		if (pControl && m_pLastControl != pControl)
		{
			CXTPImageManagerIconSet* pIconSet = m_pActiveFrame->GetCommandBars()->GetImageManager()->GetIconSet(pControl->GetIconId());
			if (pIconSet && pIconSet != m_pActiveIcons)
			{
				m_pActiveIcons = pIconSet;
				RefreshIcons();
			}
			m_pLastControl = pControl;
		}
	}
}



void CPaneIcons::OnComboChanged()
{
	int nIndex = m_wndCombo.GetCurSel();
	if (nIndex != CB_ERR)
	{
		m_pActiveIcons = (CXTPImageManagerIconSet*)m_wndCombo.GetItemData(nIndex);
	}
	else
	{
		m_pActiveIcons = NULL;
	}
	RefreshIcons();

}

void CPaneIcons::RemoveIcons(CXTPImageManagerIconSet* pIconSet, CXTPImageManagerIcon* pIconRemove)
{
	CXTPImageManagerIconSet::CIconSetMap* pMapIcons = pIconSet->GetIcons();

	POSITION pos = pMapIcons->GetStartPosition();
	UINT nWidth;
	CXTPImageManagerIcon* pIcon;

	while (pos)
	{
		pMapIcons->GetNextAssoc(pos, nWidth, pIcon);
		if (pIcon == pIconRemove)
		{
			pMapIcons->RemoveKey(nWidth);
			delete pIcon;
			break;
		}
	}

	if (pMapIcons->IsEmpty())
	{
		CMap<UINT, UINT, CXTPImageManagerIconSet*, CXTPImageManagerIconSet*>* pIcons = GetImageManager()->GetImages();
		UINT nIDCommand;
		CXTPImageManagerIconSet* pIconSetRemove;

		POSITION pos = pIcons->GetStartPosition();
		while (pos)
		{
			pIcons->GetNextAssoc(pos, nIDCommand, pIconSetRemove);
			if (pIconSetRemove == pIconSet)
			{
				pIcons->RemoveKey(nIDCommand);
				delete pIconSet;
				break;
			}
		}
	}
}

void CPaneIcons::OnPaneiconsClear()
{
	CControlIconsViewItem* pSelected = m_wndIconsView.m_pSelected;
	if (pSelected)
	{
		switch(pSelected->imageState)
		{
			case xtpImageNormal: RemoveIcons(m_pActiveIcons, pSelected->pIcon); break;
			case xtpImageHot: pSelected->pIcon->GetHotIcon().Clear(); break;
			case xtpImageChecked: pSelected->pIcon->GetCheckedIcon().Clear(); break;
			case xtpImageDisabled: pSelected->pIcon->GetDisabledIcon().Clear(); break;
		}

		RefreshIcons();
		SetModified();
	}

}

void CPaneIcons::OnUpdatePaneiconsClear(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_wndIconsView.m_pSelected != NULL);

}

HICON GenerateIcon(int nWidth, int nHeight)
{
	CImageList il;
	il.Create(nWidth, nHeight, ILC_COLOR24 |ILC_MASK, 0, 0);
	il.Add(AfxGetApp()->LoadIcon(IDI_ICONEMPTY));

	return il.ExtractIcon(0);
}

void CPaneIcons::OnPaneiconsNew()
{
	if (!m_pActiveFrame)
		return;

	CDialogNewIcon dni;
	int nIndex = m_wndCombo.GetCurSel();
	if (nIndex != CB_ERR) m_wndCombo.GetLBText(nIndex, dni.m_strId);

	if (dni.DoModal() == IDOK)
	{

		int nIDCommand = m_pActiveFrame->ResourceManager()->GetStringID(dni.m_strId);
		if (nIDCommand == 0)
			return;

		int nWidth = _ttoi(dni.m_strWidth);
		int nHeight = _ttoi(dni.m_strHeight);
		HICON hIcon = GenerateIcon(nWidth, nHeight);
		GetImageManager()->SetIcon(hIcon, nIDCommand);

		m_pActiveIcons = GetImageManager()->GetIconSet(nIDCommand);
		RefreshIcons();

		SetModified();
		RefreshAll();
	}

}

void CPaneIcons::OnUpdatePaneiconsNew(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pActiveFrame != NULL);

}

void CPaneIcons::OnPaneiconsEdit()
{
	if (m_wndIconsView.m_pSelected)
	{
		m_wndIconsView.Edit();
	}
}

void CPaneIcons::OnUpdatePaneiconsEdit(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_wndIconsView.m_pSelected != NULL);
}


void CPaneIcons::OnUpdatePaneiconsImportAndExport(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pActiveFrame != NULL);

}

void CPaneIcons::OnImportandexportExporttoxml()
{
	if (!m_pActiveFrame)
		return;

	CString strFilter = _T("XML Document (*.xml)|*.xml|All files (*.*)|*.*||");

	CFileDialog fd(FALSE, _T("xml"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter);
	if (fd.DoModal() != IDOK)
		return;


	CXTPPropExchangeXMLNode px(FALSE, 0, _T("DesignerFile"));
	CXTPPropExchangeSection secImages(px.GetSection(_T("Images")));
	GetImageManager()->DoPropExchange(&secImages);

	px.SaveToFile(fd.GetFileName());

}

void CPaneIcons::OnImportandexportImportfromxml()
{
	if (!m_pActiveFrame)
		return;

	CString strFilter = _T("XML Document (*.xml)|*.xml|All files (*.*)|*.*||");

	CFileDialog fd(TRUE, _T("xml"), NULL, OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, strFilter);
	if (fd.DoModal() != IDOK)
		return;

	CXTPPropExchangeXMLNode px(TRUE, NULL, _T("DesignerFile"));
	if (!px.LoadFromFile(fd.GetPathName()))
		return;

	CXTPPropExchangeSection secImages(px.GetSection(_T("Images")));
	GetImageManager()->DoPropExchange(&secImages);

	RefreshAll();
	SetModified();
}

void CPaneIcons::OnUpdateXMLCommand(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsXMLEngineAvailable() && m_pActiveFrame != NULL);
}

void CPaneIcons::OnImportandexportExporttobitmap()
{
	if (!m_pActiveFrame)
		return;

	CDialogBitmapExport dbe(GetImageManager(), m_pActiveFrame->ResourceManager());
	dbe.DoModal();
}

void CPaneIcons::OnImportandexportImportfrombitmap()
{
	if (!m_pActiveFrame)
		return;

	CString strFilter = _T("Bitmap Files (*.bmp)|*.bmp|All files (*.*)|*.*||");

	CFileDialog fd(TRUE, _T("bmp"), NULL, OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, strFilter);
	if (fd.DoModal() != IDOK)
		return;

	CDialogBitmapImport dbi(GetImageManager(), fd.GetPathName(), m_pActiveFrame->ResourceManager());
	dbi.DoModal();

	RefreshAll();
	SetModified();
}
