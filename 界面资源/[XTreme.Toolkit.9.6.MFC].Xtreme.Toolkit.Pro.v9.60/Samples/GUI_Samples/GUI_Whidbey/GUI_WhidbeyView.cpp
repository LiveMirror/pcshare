// GUI_WhidbeyView.cpp : implementation of the CGUI_WhidbeyView class
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
#include "GUI_Whidbey.h"

#include "GUI_WhidbeyDoc.h"
#include "GUI_WhidbeyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUI_WhidbeyView

IMPLEMENT_DYNCREATE(CGUI_WhidbeyView, CEditView)

BEGIN_MESSAGE_MAP(CGUI_WhidbeyView, CEditView)
	//{{AFX_MSG_MAP(CGUI_WhidbeyView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)

	ON_XTP_EXECUTE(ID_EDIT_FIND_EX, OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_EX, OnUpdateEditFind)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUI_WhidbeyView construction/destruction

CGUI_WhidbeyView::CGUI_WhidbeyView()
{
	// TODO: add construction code here

}

CGUI_WhidbeyView::~CGUI_WhidbeyView()
{
}

BOOL CGUI_WhidbeyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);   // Enable word-wrapping

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_WhidbeyView drawing

void CGUI_WhidbeyView::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_WhidbeyView printing

BOOL CGUI_WhidbeyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CGUI_WhidbeyView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing.
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CGUI_WhidbeyView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_WhidbeyView diagnostics

#ifdef _DEBUG
void CGUI_WhidbeyView::AssertValid() const
{
	CEditView::AssertValid();
}

void CGUI_WhidbeyView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CGUI_WhidbeyDoc* CGUI_WhidbeyView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGUI_WhidbeyDoc)));
	return (CGUI_WhidbeyDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGUI_WhidbeyView message handlers


void CGUI_WhidbeyView::OnEditFind(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlComboBox* pControl = (CXTPControlComboBox*)tagNMCONTROL->pControl;
	if (pControl->GetType() == xtpControlComboBox)
	{
		CString strFind = pControl->GetEditText();

		if (!FindText(strFind, TRUE,FALSE))
		{
			AfxMessageBox(_T("String not Found"));
		}

		*pResult = 1; // Handled;
	}
}

void CGUI_WhidbeyView::OnUpdateEditFind(CCmdUI* pCmd)
{
	pCmd->Enable(TRUE);
}
