// GUI_VisualStudio7View.cpp : implementation of the CGUI_VisualStudio7View class
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
#include "GUI_VisualStudio7.h"

#include "GUI_VisualStudio7Doc.h"
#include "GUI_VisualStudio7View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGUI_VisualStudio7View

IMPLEMENT_DYNCREATE(CGUI_VisualStudio7View, CEditView)

BEGIN_MESSAGE_MAP(CGUI_VisualStudio7View, CEditView)
	//{{AFX_MSG_MAP(CGUI_VisualStudio7View)
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
// CGUI_VisualStudio7View construction/destruction

CGUI_VisualStudio7View::CGUI_VisualStudio7View()
{
	// TODO: add construction code here

}

CGUI_VisualStudio7View::~CGUI_VisualStudio7View()
{
}

BOOL CGUI_VisualStudio7View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//cs.dwExStyle |= WS_EX_STATICEDGE;

	return CEditView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_VisualStudio7View drawing

void CGUI_VisualStudio7View::OnDraw(CDC* /*pDC*/)
{
	CGUI_VisualStudio7Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_VisualStudio7View printing

BOOL CGUI_VisualStudio7View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	//return DoPreparePrinting(pInfo);
	return CEditView::OnPreparePrinting(pInfo);
}

void CGUI_VisualStudio7View::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing.
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CGUI_VisualStudio7View::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_VisualStudio7View diagnostics

#ifdef _DEBUG
void CGUI_VisualStudio7View::AssertValid() const
{
	CEditView::AssertValid();
}

void CGUI_VisualStudio7View::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CGUI_VisualStudio7Doc* CGUI_VisualStudio7View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGUI_VisualStudio7Doc)));
	return (CGUI_VisualStudio7Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGUI_VisualStudio7View message handlers



void CGUI_VisualStudio7View::OnEditFind(NMHDR* pNMHDR, LRESULT* pResult)
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

void CGUI_VisualStudio7View::OnUpdateEditFind(CCmdUI* pCmd)
{
	pCmd->Enable(TRUE);
}
