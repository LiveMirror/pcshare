// SearchOptions.cpp : implementation file
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
#include "Grep.h"
#include "SearchOptions.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CSearchBrowseButton::Initialize(CComboBox* pParentWnd, DWORD dwBStyle, UINT nMenu, int nSubMenuIndx)
{
	m_nMenu = nMenu;
	m_nSubMenuIndx = nSubMenuIndx;
	m_dwBStyle = dwBStyle;


	m_pComboBox = pParentWnd;
}

void CSearchBrowseButton::OnClicked()
{
	static BOOL m_bExecute = FALSE;
	if (m_bExecute)
		return;
	m_bExecute = TRUE;

	if (m_dwBStyle == BES_XT_CHOOSEDIR)
	{
		CString strPath;
		m_pComboBox->GetWindowText(strPath);

		// Instantiate a browse for folder dialog object
		CXTBrowseDialog browseDlg;
		browseDlg.SetOwner(GetParent()->GetSafeHwnd());
		browseDlg.SetSelPath((TCHAR*)(LPCTSTR)strPath);

		if( browseDlg.DoModal() == IDOK ) {
			m_pComboBox->SetWindowText( browseDlg.GetSelPath() );
		}

		m_bExecute = FALSE;
		return;
	}

	SetState(TRUE);

	CRect rect;
	GetWindowRect(&rect);

	// loading a user resource.
	CMenu menu;
	menu.LoadMenu(m_nMenu);

	CMenu* pPopup = menu.GetSubMenu(m_nSubMenuIndx);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	int nCmd = XTFuncContextMenu(pPopup, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL |TPM_RETURNCMD,
		rect.right, rect.top, pWndPopupOwner, 0 );

	if (nCmd > 0)
	{
		CString str;
		str.LoadString(nCmd);

		CString strPrev;
		m_pComboBox->GetWindowText(strPrev);
		m_pComboBox->SetWindowText(strPrev + str);


	}

	// Return the button state to normal.
	SetState(FALSE);

	m_bExecute = FALSE;
};
/////////////////////////////////////////////////////////////////////////////
// CSearchOptionsView

IMPLEMENT_DYNCREATE(CSearchOptionsView, CXTResizeFormView)

CSearchOptions::CSearchOptions()
{
	m_bIncludeSubFolders = TRUE;
	m_bRegularExpressions = TRUE;
	m_bMatchCase = FALSE;
	m_bMatchWholeWords = FALSE;

	m_strFind = _T("");
	m_strReplace = _T("");
	m_strPath = _T("");
	m_strFileTypes = _T("*.*");

	pRegExp = 0;
}

void ExchangeList(CXTPPropExchange* pPX,  LPCTSTR pszPropName, CStringList& lst)
{
	CXTPPropExchangeSection secItems(pPX->GetSection(pszPropName));
	CString str, strSection;

	if (pPX->IsStoring())
	{
		DWORD dwCount = (DWORD)lst.GetCount();
		secItems->WriteCount(dwCount);
		
		POSITION pos = lst.GetHeadPosition();
		int i = 0;
		while (pos)
		{
			str = lst.GetNext(pos);				
			strSection.Format(_T("Item%i"), i++);
			PX_String(&secItems, strSection, str, _T(""));
			
		}	
	}
	else
	{
		DWORD nNewCount = secItems->ReadCount();
		for (DWORD i = 0; i < nNewCount; i++)
		{
			strSection.Format(_T("Item%i"), i);

			PX_String(&secItems, strSection, str, _T(""));
			lst.AddTail(str);
		}
	}
	
}

void CSearchOptions::DoPropExchange(CXTPPropExchange* pPX)
{
	pPX->ExchangeSchema();

	PX_Bool(pPX, _T("MatchCase"), m_bMatchCase, FALSE);
	PX_Bool(pPX, _T("MatchWholeWords"), m_bMatchWholeWords, FALSE);
	PX_Bool(pPX, _T("RegularExpressions"), m_bRegularExpressions, TRUE);
	PX_Bool(pPX, _T("IncludeSubFolders"), m_bIncludeSubFolders, TRUE);

	PX_String(pPX, _T("FindString"), m_strFind, _T(""));
	PX_String(pPX, _T("ReplaceString"), m_strReplace, _T(""));
	PX_String(pPX, _T("Path"), m_strPath, _T(""));
	PX_String(pPX, _T("FileTypes"), m_strFileTypes, _T("*.*"));

	ExchangeList(pPX, _T("FileTypesHistory"),m_lstFileTypesHistory);
	ExchangeList(pPX, _T("ReplaceHistory"),m_lstReplaceHistory);
	ExchangeList(pPX, _T("FindHistory"),m_lstFindHistory);
	ExchangeList(pPX, _T("PathHistory"),m_lstPathHistory);
}

CSearchOptions* GetSearchOptions()
{
	static CSearchOptions options;
	return &options;
}



CSearchOptionsView::CSearchOptionsView()
	: CXTResizeFormView(CSearchOptionsView::IDD)
{
	//{{AFX_DATA_INIT(CSearchOptionsView)
	//}}AFX_DATA_INIT
	m_bInitialized = FALSE;
}

CSearchOptionsView::~CSearchOptionsView()
{
}

void CSearchOptionsView::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchOptionsView)
	DDX_Control(pDX, IDC_COMBO_REPLACE, m_cmbReplace);
	DDX_Control(pDX, IDC_COMBO_PATH, m_cmbPath);
	DDX_Control(pDX, IDC_COMBO_FIND, m_cmbFind);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cmbFileTypes);
	DDX_Control(pDX, IDC_BUTTON_REPLACE_EXPAND, m_btnReplaceExpand);
	DDX_Control(pDX, IDC_BUTTON_FIND_EXPAND, m_btnFindExpand);
	DDX_Control(pDX, IDC_BUTTON_PATH_EXPAND, m_btnPathExpand);
	DDX_Control(pDX, IDC_GROUP_FINDOPTIONS, m_wndGroupFindOptions);
	//}}AFX_DATA_MAP
	DDX_CBString(pDX, IDC_COMBO_FIND, GetSearchOptions()->m_strFind);
	DDX_CBString(pDX, IDC_COMBO_REPLACE, GetSearchOptions()->m_strReplace);
	DDX_CBString(pDX, IDC_COMBO_PATH, GetSearchOptions()->m_strPath);
	DDX_Check(pDX, IDC_CHECK_INCLUDE_SUBFOLDERS, GetSearchOptions()->m_bIncludeSubFolders);
	DDX_Check(pDX, IDC_CHECK_REGULAR, GetSearchOptions()->m_bRegularExpressions);
	DDX_Check(pDX, IDC_CHECK_MATCH_CASE, GetSearchOptions()->m_bMatchCase);
	DDX_Check(pDX, IDC_CHECK_MATCH_WHOLE_WORDS, GetSearchOptions()->m_bMatchWholeWords);
	DDX_CBString(pDX, IDC_COMBO_TYPE, GetSearchOptions()->m_strFileTypes);
}


BEGIN_MESSAGE_MAP(CSearchOptionsView, CXTResizeFormView)
	//{{AFX_MSG_MAP(CSearchOptionsView)
	ON_BN_CLICKED(IDC_BUTTON_FINDALL, OnButtonFindAll)
	ON_BN_CLICKED(IDC_BUTTON_REPLACEALL, OnButtonReplaceAll)
	ON_CBN_EDITCHANGE(IDC_COMBO_FIND, UpdateOptions)
	ON_CBN_EDITCHANGE(IDC_COMBO_PATH, UpdateOptions)
	ON_CBN_EDITCHANGE(IDC_COMBO_REPLACE, UpdateOptions)
	ON_BN_CLICKED(IDC_CHECK_REGULAR, UpdateOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchOptionsView diagnostics

#ifdef _DEBUG
void CSearchOptionsView::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

void CSearchOptionsView::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif //_DEBUG


void RestoryCombo(CComboBox& cmb, CStringList& lstHistory, LPCTSTR lpszDefault = 0)
{
	if (lstHistory.GetCount() > 0)
	{
		POSITION pos = lstHistory.GetHeadPosition();
		while (pos)
		{
			cmb.AddString(lstHistory.GetNext(pos));
		}

	}
	else if (lpszDefault)
	{
		cmb.AddString(lpszDefault);
		lstHistory.AddTail(lpszDefault);
	}

}
/////////////////////////////////////////////////////////////////////////////
// CSearchOptionsView message handlers

void CSearchOptionsView::OnInitialUpdate()
{
	CXTResizeFormView::OnInitialUpdate();

	SetScaleToFitSize(CSize(90, 1));

	if (m_bInitialized)
		return;

	SetResize(IDC_COMBO_FIND, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_FIND_EXPAND, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_COMBO_REPLACE, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_REPLACE_EXPAND, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_COMBO_PATH, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_PATH_EXPAND, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_COMBO_TYPE, SZ_TOP_LEFT, SZ_TOP_RIGHT);

	SetResize(IDC_GROUP_FINDOPTIONS, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_FINDALL, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_REPLACEALL, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	m_bInitialized = TRUE;

	m_btnFindExpand.SetBrowseStyle(BES_XT_POPUPMENU);
	m_btnFindExpand.Initialize(&m_cmbFind, BES_XT_POPUPMENU, IDR_SEARCH_POPUP, 0);

	m_btnReplaceExpand.SetBrowseStyle(BES_XT_POPUPMENU);
	m_btnReplaceExpand.Initialize(&m_cmbReplace, BES_XT_POPUPMENU, IDR_SEARCH_POPUP, 1);

	m_btnPathExpand.Initialize(&m_cmbPath, BES_XT_CHOOSEDIR);

	CString strOfficeFont(_T("Tahoma"));
	if (CXTPPaintManager::FontExists( strOfficeFont ))
	{
		CXTPPaintManager::CNonClientMetrics ncm;
		STRCPY_S(ncm.lfMenuFont.lfFaceName, LF_FACESIZE, strOfficeFont);
		m_fontRegular.CreateFontIndirect (&ncm.lfMenuFont);

		XTFuncChangeWindowFont(this, &m_fontRegular);
	}
	UpdateOptions();


	IRegExpPtr regExp;
	HRESULT hr = regExp.CreateInstance(__uuidof(RegExp));
	if (FAILED(hr))
	{
		GetDlgItem(IDC_CHECK_REGULAR)->EnableWindow(FALSE);
	}

	RestoryCombo(m_cmbFileTypes, GetSearchOptions()->m_lstFileTypesHistory, _T("*.cpp;*.h"));
	RestoryCombo(m_cmbFind, GetSearchOptions()->m_lstFindHistory);
	RestoryCombo(m_cmbReplace, GetSearchOptions()->m_lstReplaceHistory);
	RestoryCombo(m_cmbPath, GetSearchOptions()->m_lstPathHistory);


}

BOOL CSearchOptionsView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	return CXTResizeFormView::PreCreateWindow(cs);
}

void CSearchOptionsView::OnButtonFindAll()
{
	UpdateData();

	((CMainFrame*)AfxGetMainWnd())->OnFindAll();
}

BOOL CSearchOptionsView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
	{
		if ((::GetFocus() == m_hWnd) || (::GetParent(::GetFocus()) == m_hWnd) || (::GetParent(::GetParent(::GetFocus())) == m_hWnd))
		{
			if (IsDialogMessage(pMsg))
				return TRUE;
		}
	}

	return CXTResizeFormView::PreTranslateMessage(pMsg);
}

void CSearchOptionsView::UpdateOptions()
{
	UpdateData();

	CSearchOptions* pOptions = GetSearchOptions();
	GetDlgItem(IDC_BUTTON_FINDALL)->EnableWindow(!pOptions->m_strFind.IsEmpty() && !pOptions->m_strPath.IsEmpty());
	GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_FIND_EXPAND)->EnableWindow(pOptions->m_bRegularExpressions);
	GetDlgItem(IDC_BUTTON_REPLACE_EXPAND)->EnableWindow(pOptions->m_bRegularExpressions);

}

void CSearchOptionsView::OnButtonReplaceAll()
{
	((CMainFrame*)AfxGetMainWnd())->OnReplaceAll();
}
