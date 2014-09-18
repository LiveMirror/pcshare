// PropertiesView.cpp : implementation file
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
#include "TabbedView.h"
#include "PropertiesView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertiesView

IMPLEMENT_DYNCREATE(CPropertiesView, CFormView)

CPropertiesView::CPropertiesView()
	: CFormView(CPropertiesView::IDD)
{
	//{{AFX_DATA_INIT(CPropertiesView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nAppearance = -1;
	m_nColor = -1;
	m_nLayout = -1;
	m_nPosition = -1;
	m_bLunaColors = FALSE;
	m_bOneNoteColors = FALSE;
}

CPropertiesView::~CPropertiesView()
{
}

void CPropertiesView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertiesView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_APPEARANCE, m_comboAppearance);
	DDX_Control(pDX, IDC_COMBO_POSITION, m_comboPosition);
	DDX_Control(pDX, IDC_COMBO_COLOR, m_comboColor);
	DDX_CBIndex(pDX, IDC_COMBO_APPEARANCE, m_nAppearance);
	DDX_CBIndex(pDX, IDC_COMBO_COLOR, m_nColor);
	DDX_CBIndex(pDX, IDC_COMBO_LAYOUT, m_nLayout);
	DDX_CBIndex(pDX, IDC_COMBO_POSITION, m_nPosition);

	DDX_Check(pDX, IDC_CHECK_LUNA_COLORS, m_bLunaColors);
	DDX_Check(pDX, IDC_CHECK_ONENOTE_COLORS, m_bOneNoteColors);

}


BEGIN_MESSAGE_MAP(CPropertiesView, CFormView)
	//{{AFX_MSG_MAP(CPropertiesView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_COLOR, OnSelchangeComboColor)
	ON_CBN_SELCHANGE(IDC_COMBO_POSITION, OnSelchangeComboPosition)
	ON_CBN_SELCHANGE(IDC_COMBO_APPEARANCE, OnSelchangeComboAppearance)
	ON_BN_CLICKED(IDC_CHECK_LUNA_COLORS, OnCheckLunaColors)
	ON_BN_CLICKED(IDC_CHECK_ONENOTE_COLORS, OnCheckOnenoteColors)
	ON_CBN_SELCHANGE(IDC_COMBO_LAYOUT, OnSelchangeComboLayout)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertiesView diagnostics

#ifdef _DEBUG
void CPropertiesView::AssertValid() const
{
	CFormView::AssertValid();
}

void CPropertiesView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPropertiesView message handlers

void CPropertiesView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, CSize(0)); // Disable Scroll Bars;

	Update();

	m_comboPosition.SetItemData(0, xtpTabPositionTop);
	m_comboPosition.SetItemData(1, xtpTabPositionLeft);
	m_comboPosition.SetItemData(2, xtpTabPositionBottom);
	m_comboPosition.SetItemData(3, xtpTabPositionRight);

}

CXTPTabPaintManager* CPropertiesView::GetPaintManager()
{
	return ((CXTPTabControl*)GetParent())->GetPaintManager();
}
void CPropertiesView::Reposition()
{
	((CXTPTabControl*)GetParent())->Reposition();

}

void CPropertiesView::OnSelchangeComboColor()
{
	UpdateData();

	XTPTabColorStyle color = (XTPTabColorStyle)m_comboColor.GetItemData(m_nColor);

	GetPaintManager()->SetColor(color);

	UpdateControls();
	Reposition();

}

void CPropertiesView::Update()
{
	m_nAppearance = GetPaintManager()->GetAppearance();
	m_nColor = 1;
	m_nLayout = GetPaintManager()->GetLayout();
	m_nPosition = GetPaintManager()->GetPosition();
	m_bLunaColors = !GetPaintManager()->IsLunaColorsDisabled();
	m_bOneNoteColors = GetPaintManager()->m_bOneNoteColors;

	UpdateData(FALSE);
	UpdateControls();
}


void CPropertiesView::OnSelchangeComboPosition()
{
	UpdateData();

	XTPTabPosition position = (XTPTabPosition)m_comboPosition.GetItemData(m_nPosition);

	GetPaintManager()->SetPosition(position);
	Reposition();

}

void CPropertiesView::OnSelchangeComboAppearance()
{
	UpdateData();
	GetPaintManager()->SetAppearance((XTPTabAppearanceStyle)m_nAppearance);

	UpdateControls();
	Reposition();
}

void CPropertiesView::OnSelchangeComboLayout()
{
	UpdateData();
	GetPaintManager()->SetLayout((XTPTabLayoutStyle)m_nLayout);
	Reposition();

}


void CPropertiesView::UpdateControls()
{
	const struct
	{
		int nColorSet;
		LPCTSTR strColorSet;
	}
	colors[] =
	{
		xtpTabColorDefault, _T("xtpTabColorDefault"),
		xtpTabColorVisualStudio, _T("xtpTabColorVisualStudio"),
		xtpTabColorOffice2003, _T("xtpTabColorOffice2003"),
		xtpTabColorWinXP, _T("xtpTabColorWinXP")
	};

	int nSupported = GetPaintManager()->GetAppearanceSet()->GetSupportedColorSets();

	m_comboColor.ResetContent();

	for (int i = 0; i < 4; i++)
	{
		if (nSupported & colors[i].nColorSet)
		{
			int nItem = m_comboColor.AddString(colors[i].strColorSet);
			m_comboColor.SetItemData(nItem, colors[i].nColorSet);
			if (GetPaintManager()->GetColor() == colors[i].nColorSet)
			{
				m_nColor = nItem;
				m_comboColor.SetCurSel(nItem);
			}

		}
	}
	GetDlgItem(IDC_CHECK_LUNA_COLORS)->EnableWindow(m_comboColor.GetItemData(m_nColor) == xtpTabColorOffice2003);
	GetDlgItem(IDC_CHECK_ONENOTE_COLORS)->EnableWindow(m_comboColor.GetItemData(m_nColor) != xtpTabColorWinXP);
}


void CPropertiesView::OnCheckLunaColors()
{
	UpdateData();
	GetPaintManager()->DisableLunaColors(!m_bLunaColors);
	Reposition();
}

void CPropertiesView::OnCheckOnenoteColors()
{
	UpdateData();
	GetPaintManager()->SetOneNoteColors(m_bOneNoteColors);
	Reposition();
}
