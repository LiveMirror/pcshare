// GUI_GnomeView.cpp : implementation of the CGUI_GnomeView class
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
#include "GUI_Gnome.h"

#include "GUI_GnomeDoc.h"
#include "CntrItem.h"
#include "GUI_GnomeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUI_GnomeView

IMPLEMENT_DYNCREATE(CGUI_GnomeView, CRichEditView)

BEGIN_MESSAGE_MAP(CGUI_GnomeView, CRichEditView)
	//{{AFX_MSG_MAP(CGUI_GnomeView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRichEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUI_GnomeView construction/destruction

CGUI_GnomeView::CGUI_GnomeView()
{
	// TODO: add construction code here

}

CGUI_GnomeView::~CGUI_GnomeView()
{
}

BOOL CGUI_GnomeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CRichEditView::PreCreateWindow(cs);
}

void CGUI_GnomeView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();


	// Set the printing margins (720 twips = 1/2 inch).
	SetMargins(CRect(720, 720, 720, 720));
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_GnomeView printing

BOOL CGUI_GnomeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


void CGUI_GnomeView::OnDestroy()
{
	CRichEditView::OnDestroy();
}


/////////////////////////////////////////////////////////////////////////////
// CGUI_GnomeView diagnostics

#ifdef _DEBUG
void CGUI_GnomeView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CGUI_GnomeView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CGUI_GnomeDoc* CGUI_GnomeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGUI_GnomeDoc)));
	return (CGUI_GnomeDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGUI_GnomeView message handlers
