// DynamicPopupsView.cpp : implementation of the CDynamicPopupsView class
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
#include "DynamicPopups.h"

#include "DynamicPopupsDoc.h"
#include "DynamicPopupsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynamicPopupsView

IMPLEMENT_DYNCREATE(CDynamicPopupsView, CFormView)

BEGIN_MESSAGE_MAP(CDynamicPopupsView, CFormView)
	//{{AFX_MSG_MAP(CDynamicPopupsView)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)

	ON_COMMAND(ID_BUTTON_ADD, OnButtonAdd)
	ON_COMMAND(ID_BUTTON_REMOVE, OnButtonRemove)

	ON_COMMAND(ID_BUTTON_POPUP, OnEmptyCommand)
	ON_COMMAND(ID_COMBO_UNDO, OnEmptyCommand)

	ON_COMMAND_RANGE(FIRST_UNDO_COMMAND, LAST_UNDO_COMMAND, OnUndoClick)

	ON_XTP_EXECUTE(ID_COMBO_ADD, OnComboAdd)
	ON_UPDATE_COMMAND_UI(ID_COMBO_ADD, OnEmptyUpdateCommand)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynamicPopupsView construction/destruction

CDynamicPopupsView::CDynamicPopupsView()
	: CFormView(CDynamicPopupsView::IDD)
{
	//{{AFX_DATA_INIT(CDynamicPopupsView)
	m_strText = _T("");
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CDynamicPopupsView::~CDynamicPopupsView()
{
}

void CDynamicPopupsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDynamicPopupsView)
	DDX_Control(pDX, IDC_LIST, m_wndList);
	DDX_Text(pDX, IDC_EDIT, m_strText);
	//}}AFX_DATA_MAP
}

BOOL CDynamicPopupsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CDynamicPopupsView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	m_wndList.AddString(_T("Undo Edit"));
	m_wndList.AddString(_T("Undo Resize"));
}

/////////////////////////////////////////////////////////////////////////////
// CDynamicPopupsView printing

BOOL CDynamicPopupsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDynamicPopupsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDynamicPopupsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CDynamicPopupsView::OnPrint(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CDynamicPopupsView diagnostics

#ifdef _DEBUG
void CDynamicPopupsView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDynamicPopupsView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDynamicPopupsDoc* CDynamicPopupsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDynamicPopupsDoc)));
	return (CDynamicPopupsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDynamicPopupsView message handlers



void CDynamicPopupsView::OnEmptyCommand()
{

}
void CDynamicPopupsView::OnEmptyUpdateCommand(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CDynamicPopupsView::OnButtonAdd()
{
	UpdateData();
	if (m_strText.GetLength() > 0)
	{
		m_wndList.AddString(m_strText);
	}

}

void CDynamicPopupsView::OnButtonRemove()
{
	int nIndex = m_wndList.GetCurSel();
	if (nIndex != -1)
	{
		m_wndList.DeleteString(nIndex);
	}
}

void CDynamicPopupsView::OnUndoClick(UINT nID)
{
	int nIndex = nID - FIRST_UNDO_COMMAND;

	if (nIndex >= 0 && nIndex < m_wndList.GetCount())
	{
		CString strTitle;
		m_wndList.GetText(nIndex, strTitle);
		AfxMessageBox(strTitle);
	}
}


void CDynamicPopupsView::OnComboAdd(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlComboBox* pCombo = (CXTPControlComboBox*)tagNMCONTROL->pControl;
	ASSERT(pCombo);
	ASSERT(pCombo->GetType() == xtpControlComboBox);

	CString strText;
	pCombo->GetEditCtrl()->GetWindowText(strText);
	if (strText.GetLength() > 0)
	{
		m_wndList.AddString(strText);
		pCombo->AddString(strText);
	}

	*pResult = 1;
}
