// PageLookAndBehaviour.cpp : implementation file
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
#include "ToolBox.h"
#include "PageLookAndBehaviour.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if (_MSC_VER <= 1100)

void DDX_Slider(CDataExchange* pDX, int nIDC, int& value)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
		value = (int) ::SendMessage(hWndCtrl, TBM_GETPOS, 0, 0l);
	else
		::SendMessage(hWndCtrl, TBM_SETPOS, TRUE, value);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageLookAndBehaviour property page

CPageLookAndBehaviour::CPageLookAndBehaviour() : CDialog(CPageLookAndBehaviour::IDD)
{
	//{{AFX_DATA_INIT(CPageLookAndBehaviour)
	m_bLargeIcons = FALSE;
	m_bDrawFocusRect = TRUE;
	m_bSelectOnFocus = TRUE;
	m_bToggleButtons = FALSE;
	m_nControlMargins = 0;
	m_nGroupInnerMargins = 50;
	m_nGroupOuterMargins = 0;
	m_nItemsMargins = 0;
	m_bDefaultMargins = TRUE;
	m_bAllowDrag = TRUE;
	//}}AFX_DATA_INIT
}

CPageLookAndBehaviour::~CPageLookAndBehaviour()
{
}

void CPageLookAndBehaviour::OnCancel()
{

}

void CPageLookAndBehaviour::OnOK()
{

}

void CPageLookAndBehaviour::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageLookAndBehaviour)
	DDX_Control(pDX, IDC_COMBO_THEME, m_comboTheme);
	DDX_Control(pDX, IDC_COMBO_LAYOUT, m_comboLayout);
	DDX_Control(pDX, IDC_COMBO_HOTTRACK, m_comboHotTrack);
	DDX_Control(pDX, IDC_COMBO_BEHAVIOUR, m_comboBehaviour);
	DDX_Check(pDX, IDC_CHECK_LARGEICONS, m_bLargeIcons);
	DDX_Check(pDX, IDC_CHECK_DRAWFOCUSRECT, m_bDrawFocusRect);
	DDX_Check(pDX, IDC_CHECK_SELECTONFOCUS, m_bSelectOnFocus);
	DDX_Check(pDX, IDC_CHECK_TOGGLEBUTTONS, m_bToggleButtons);
	DDX_Slider(pDX, IDC_SLIDER_CONTROL, m_nControlMargins);
	DDX_Slider(pDX, IDC_SLIDER_GROUPINNER, m_nGroupInnerMargins);
	DDX_Slider(pDX, IDC_SLIDER_GROUPOUTER, m_nGroupOuterMargins);
	DDX_Slider(pDX, IDC_SLIDER_ITEMS, m_nItemsMargins);
	DDX_Check(pDX, IDC_CHECK_DEFAULTMARGINS, m_bDefaultMargins);
	DDX_Check(pDX, IDC_CHECK_ALLOWDRAG, m_bAllowDrag);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageLookAndBehaviour, CDialog)
	//{{AFX_MSG_MAP(CPageLookAndBehaviour)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_COMBO_BEHAVIOUR, OnSelchangeComboBehaviour)
	ON_CBN_SELCHANGE(IDC_COMBO_THEME, OnSelchangeComboTheme)
	ON_CBN_SELCHANGE(IDC_COMBO_LAYOUT, OnSelchangeComboLayout)
	ON_CBN_SELCHANGE(IDC_COMBO_HOTTRACK, OnSelchangeComboHottrack)
	ON_BN_CLICKED(IDC_CHECK_LARGEICONS, OnCheckLargeicons)
	ON_BN_CLICKED(IDC_CHECK_DRAWFOCUSRECT, OnCheckDrawfocusrect)
	ON_BN_CLICKED(IDC_CHECK_SELECTONFOCUS, OnCheckSelectonfocus)
	ON_BN_CLICKED(IDC_CHECK_TOGGLEBUTTONS, OnCheckTogglebuttons)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_CONTROL, OnReleasedcaptureSliderControl)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_GROUPINNER, OnReleasedcaptureSliderGroupinner)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_GROUPOUTER, OnReleasedcaptureSliderGroupouter)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_ITEMS, OnReleasedcaptureSliderItems)
	ON_BN_CLICKED(IDC_CHECK_DEFAULTMARGINS, OnCheckDefaultmargins)
	ON_BN_CLICKED(IDC_CHECK_ALLOWDRAG, OnCheckAllowDrag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageLookAndBehaviour message handlers


int CPageLookAndBehaviour::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

CXTPTaskPanel* CPageLookAndBehaviour::GetTaskPanel()
{
	CWnd* pSplitterWnd = GetParentFrame()->GetDlgItem(AFX_IDW_PANE_FIRST);
	ASSERT_KINDOF(CSplitterWnd, pSplitterWnd);

	return (CXTPTaskPanel*)pSplitterWnd->GetDlgItem(AFX_IDW_PANE_FIRST);
}


BOOL CPageLookAndBehaviour::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_comboLayout.AddString(_T("xtpTaskItemLayoutDefault"));
	m_comboLayout.AddString(_T("xtpTaskItemLayoutImages"));
	m_comboLayout.AddString(_T("xtpTaskItemLayoutImagesWithText"));
	m_comboLayout.AddString(_T("xtpTaskItemLayoutImagesWithTextBelow"));
	m_comboLayout.SetCurSel(0);

	m_comboHotTrack.AddString(_T("xtpTaskPanelHighlightDefault"));
	m_comboHotTrack.AddString(_T("xtpTaskPanelHighlightText"));
	m_comboHotTrack.AddString(_T("xtpTaskPanelHighlightImage"));
	m_comboHotTrack.AddString(_T("xtpTaskPanelHighlightItem"));
	m_comboHotTrack.AddString(_T("xtpTaskPanelHighlightNone"));
	m_comboHotTrack.SetCurSel(0);

	m_comboBehaviour.AddString(_T("xtpTaskPanelBehaviourExplorer"));
	m_comboBehaviour.AddString(_T("xtpTaskPanelBehaviourList"));
	m_comboBehaviour.AddString(_T("xtpTaskPanelBehaviourToolbox"));
	m_comboBehaviour.SetCurSel(2);

	m_comboTheme.AddString(_T("xtpTaskPanelThemeOffice2000"));
	m_comboTheme.AddString(_T("xtpTaskPanelThemeOffice2003"));
	m_comboTheme.AddString(_T("xtpTaskPanelThemeNativeWinXP"));
	m_comboTheme.AddString(_T("xtpTaskPanelThemeOffice2000Plain"));
	m_comboTheme.AddString(_T("xtpTaskPanelThemeOfficeXPPlain"));
	m_comboTheme.AddString(_T("xtpTaskPanelThemeOffice2003Plain"));
	m_comboTheme.AddString(_T("xtpTaskPanelThemeNativeWinXPPlain"));
	m_comboTheme.AddString(_T("xtpTaskPanelThemeToolbox"));
	m_comboTheme.AddString(_T("xtpTaskPanelThemeToolboxWhidbey"));
	m_comboTheme.AddString(_T("xtpTaskPanelThemeListView"));
	m_comboTheme.AddString(_T("xtpTaskPanelThemeListViewOfficeXP"));
	m_comboTheme.AddString(_T("xtpTaskPanelThemeListViewOffice2003"));
	m_comboTheme.AddString(_T("xtpTaskPanelThemeShortcutBarOffice2003"));
	m_comboTheme.SetCurSel(7);

	GetDlgItem(IDC_CHECK_TOGGLEBUTTONS)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageLookAndBehaviour::OnSelchangeComboBehaviour()
{
	CXTPTaskPanel* pTaskPanel = GetTaskPanel();

	pTaskPanel->SetBehaviour((XTPTaskPanelBehaviour)m_comboBehaviour.GetCurSel());
}

void CPageLookAndBehaviour::OnSelchangeComboTheme()
{
	CXTPTaskPanel* pTaskPanel = GetTaskPanel();

	XTPTaskPanelPaintTheme themeCurrent = (XTPTaskPanelPaintTheme) m_comboTheme.GetCurSel();
	pTaskPanel->SetTheme(themeCurrent);
	pTaskPanel->Reposition();

	OnThemeChanged();


	CXTPCommandBars* pCommandBars = ((CXTPFrameWnd*)AfxGetMainWnd())->GetCommandBars();
	CXTPPaintManager::SetTheme(
		themeCurrent == xtpTaskPanelThemeOffice2000? xtpThemeOffice2000:
		themeCurrent ==xtpTaskPanelThemeNativeWinXP? xtpThemeNativeWinXP:
		themeCurrent ==xtpTaskPanelThemeOffice2000Plain? xtpThemeOffice2000:
		themeCurrent ==xtpTaskPanelThemeOfficeXPPlain? xtpThemeOfficeXP:
		themeCurrent ==xtpTaskPanelThemeNativeWinXPPlain? xtpThemeNativeWinXP:
		themeCurrent ==xtpTaskPanelThemeToolbox? xtpThemeOfficeXP:
		themeCurrent ==xtpTaskPanelThemeToolboxWhidbey? xtpThemeOfficeXP:
		themeCurrent ==xtpTaskPanelThemeListView? xtpThemeOffice2000:
		themeCurrent ==xtpTaskPanelThemeListViewOfficeXP? xtpThemeOfficeXP:
		themeCurrent ==xtpTaskPanelThemeShortcutBarOffice2003? xtpThemeOffice2003:
		xtpThemeOffice2003);
	pCommandBars->RedrawCommandBars();

}

void CPageLookAndBehaviour::OnSelchangeComboLayout()
{
	CXTPTaskPanel* pTaskPanel = GetTaskPanel();

	pTaskPanel->SetItemLayout((XTPTaskPanelItemLayout)m_comboLayout.GetCurSel());
}

void CPageLookAndBehaviour::OnSelchangeComboHottrack()
{
	CXTPTaskPanel* pTaskPanel = GetTaskPanel();

	pTaskPanel->SetHotTrackStyle((XTPTaskPanelHotTrackStyle)m_comboHotTrack.GetCurSel());
}

void CPageLookAndBehaviour::OnCheckLargeicons()
{
	UpdateData();
	CXTPTaskPanel* pTaskPanel = GetTaskPanel();

	pTaskPanel->SetIconSize(m_bLargeIcons? CSize(32, 32): CSize(16, 16));

}

void CPageLookAndBehaviour::OnCheckDrawfocusrect()
{
	UpdateData();
	CXTPTaskPanel* pTaskPanel = GetTaskPanel();

	pTaskPanel->DrawFocusRect(m_bDrawFocusRect);

}

void CPageLookAndBehaviour::OnCheckSelectonfocus()
{
	UpdateData();
	CXTPTaskPanel* pTaskPanel = GetTaskPanel();

	pTaskPanel->SetSelectItemOnFocus(m_bSelectOnFocus);

	if (m_bSelectOnFocus)
	{
		m_bToggleButtons = FALSE;
		UpdateData(FALSE);
	}
	GetDlgItem(IDC_CHECK_TOGGLEBUTTONS)->EnableWindow(!m_bSelectOnFocus);
}

void CPageLookAndBehaviour::OnCheckTogglebuttons()
{
	UpdateData();
}

void CPageLookAndBehaviour::OnReleasedcaptureSliderControl(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	UpdateData();
	CXTPTaskPanel* pTaskPanel = GetTaskPanel();

	int nMargin = 0 + m_nControlMargins * 12 / 100;
	int nGroupSpacing = 0 + m_nControlMargins * 15 / 100;
	pTaskPanel->SetMargins(nMargin, nMargin, nMargin, nMargin, nGroupSpacing);

	m_bDefaultMargins = FALSE;
	UpdateData(FALSE);
	*pResult = 0;
}

void CPageLookAndBehaviour::OnReleasedcaptureSliderGroupinner(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	UpdateData();
	CXTPTaskPanel* pTaskPanel = GetTaskPanel();

	int nMargin = 0 + m_nGroupInnerMargins * 20 / 100;
	pTaskPanel->GetPaintManager()->m_rcGroupInnerMargins.SetRect(nMargin, nMargin, nMargin, nMargin);
	pTaskPanel->Reposition();

	m_bDefaultMargins = FALSE;
	UpdateData(FALSE);
	*pResult = 0;
}

void CPageLookAndBehaviour::OnReleasedcaptureSliderGroupouter(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	UpdateData();
	CXTPTaskPanel* pTaskPanel = GetTaskPanel();

	int nMargin = 0 + m_nGroupOuterMargins * 13 / 100;
	pTaskPanel->GetPaintManager()->m_rcGroupOuterMargins.SetRect(nMargin, nMargin, nMargin, nMargin);
	pTaskPanel->Reposition();

	m_bDefaultMargins = FALSE;
	UpdateData(FALSE);
	*pResult = 0;
}

void CPageLookAndBehaviour::OnReleasedcaptureSliderItems(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	UpdateData();
	CXTPTaskPanel* pTaskPanel = GetTaskPanel();

	int nMargin = 3 + m_nItemsMargins * 15 / 100;
	pTaskPanel->GetPaintManager()->m_rcItemOuterMargins.SetRect(nMargin, nMargin, nMargin, nMargin);
	pTaskPanel->Reposition();

	m_bDefaultMargins = FALSE;
	UpdateData(FALSE);
	*pResult = 0;
}

void CPageLookAndBehaviour::OnThemeChanged()
{
	CXTPTaskPanel* pTaskPanel = GetTaskPanel();

	m_nGroupInnerMargins = 50;
	m_nGroupOuterMargins = 0;
	m_nItemsMargins = 0;

	m_nControlMargins = pTaskPanel->GetPaintManager()->GetControlMargins(pTaskPanel).left * 100 / 12;


	UpdateData(FALSE);
}

void CPageLookAndBehaviour::OnCheckDefaultmargins()
{
	UpdateData();

	CXTPTaskPanel* pTaskPanel = GetTaskPanel();

	pTaskPanel->SetTheme((XTPTaskPanelPaintTheme)m_comboTheme.GetCurSel());
	pTaskPanel->SetMargins(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);

	pTaskPanel->Reposition();

	OnThemeChanged();
	m_nControlMargins = pTaskPanel->GetPaintManager()->m_rcControlMargins.left * 100 / 12;

	UpdateData(FALSE);

}

void CPageLookAndBehaviour::OnCheckAllowDrag()
{
	UpdateData();

	CXTPTaskPanel* pTaskPanel = GetTaskPanel();

	pTaskPanel->AllowDrag(m_bAllowDrag);
}
