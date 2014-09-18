// ListBoxSampleView.cpp : implementation of the CListBoxSampleView class
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
#include "ListBoxSample.h"

#include "ListBoxSampleDoc.h"
#include "ListBoxSampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListBoxSampleView

IMPLEMENT_DYNCREATE(CListBoxSampleView, CEditView)

BEGIN_MESSAGE_MAP(CListBoxSampleView, CEditView)
	//{{AFX_MSG_MAP(CListBoxSampleView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)

	ON_XTP_EXECUTE(ID_EDIT_UNDO, OnEditUndo)

	ON_NOTIFY(XTP_LBN_SELCHANGE, ID_EDIT_UNDO, OnListBoxControlSelChange)
	ON_NOTIFY(XTP_LBN_POPUP, ID_EDIT_UNDO, OnListBoxControlPoup)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListBoxSampleView construction/destruction

CListBoxSampleView::CListBoxSampleView()
{
	// TODO: add construction code here

}

CListBoxSampleView::~CListBoxSampleView()
{
}

BOOL CListBoxSampleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);   // Enable word-wrapping

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CListBoxSampleView drawing

void CListBoxSampleView::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CListBoxSampleView printing

BOOL CListBoxSampleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CListBoxSampleView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing.
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CListBoxSampleView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CListBoxSampleView diagnostics

#ifdef _DEBUG
void CListBoxSampleView::AssertValid() const
{
	CEditView::AssertValid();
}

void CListBoxSampleView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CListBoxSampleDoc* CListBoxSampleView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CListBoxSampleDoc)));
	return (CListBoxSampleDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CListBoxSampleView message handlers


void CListBoxSampleView::OnEditUndo(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (((NMXTPCONTROL*)pNMHDR)->pControl->GetType() == xtpControlSplitButtonPopup)
	{
		AfxMessageBox(_T("Undo last command"));

		*pResult = 1; // Handled;
	}

	CXTPControlListBox* pControlListBox = DYNAMIC_DOWNCAST(CXTPControlListBox, ((NMXTPCONTROL*)pNMHDR)->pControl);
	if (pControlListBox)
	{
		CString str;
		str.Format(_T("Undo last %i actions"), pControlListBox->GetListCtrl()->GetSelCount());

		AfxMessageBox(str);

		*pResult = 1; // Handled;
	}
}


CXTPControlStatic* CListBoxSampleView::FindInfoControl(CXTPControlListBox* pControlListBox)
{
	CXTPCommandBar* pCommandBar = pControlListBox->GetParent();

	for (int i = 0; i < pCommandBar->GetControls()->GetCount(); i++)
	{
		CXTPControlStatic* pControlStatic = DYNAMIC_DOWNCAST(CXTPControlStatic, pCommandBar->GetControl(i));
		if (pControlStatic && pControlStatic->GetID() == pControlListBox->GetID())
		{
			return pControlStatic;
		}

	}
	return NULL;
}



void CListBoxSampleView::OnListBoxControlSelChange(NMHDR* pNMHDR, LRESULT* pRes)
{
	ASSERT(pNMHDR != NULL);
	ASSERT(pRes != NULL);

	CXTPControlListBox* pControlListBox = DYNAMIC_DOWNCAST(CXTPControlListBox, ((NMXTPCONTROL*)pNMHDR)->pControl);
	if (pControlListBox)
	{

		CXTPControlStatic* pInfo = FindInfoControl(pControlListBox);
		if (pInfo)
		{
			CString str;
			str.Format(_T("Undo %i Actions"), pControlListBox->GetListCtrl()->GetSelCount());
			pInfo->SetCaption(str);
			pInfo->DelayRedrawParent();
		}



		*pRes = 1;
	}
}

void CListBoxSampleView::OnListBoxControlPoup(NMHDR* pNMHDR, LRESULT* pRes)
{
	ASSERT(pNMHDR != NULL);
	ASSERT(pRes != NULL);
	CXTPControlListBox* pControlListBox = DYNAMIC_DOWNCAST(CXTPControlListBox, ((NMXTPCONTROL*)pNMHDR)->pControl);
	if (pControlListBox)
	{
		CListBox* pListBox = pControlListBox->GetListCtrl();
		pListBox->ResetContent();

		int nCount = RAND_S() % 20 + 3;

		for (int i = 0; i < nCount; i++)
		{
			CString str;
			str.Format(_T("Undo String %i"), i + 1);
			pListBox->AddString(str);
		}

		CXTPControlStatic* pInfo = FindInfoControl(pControlListBox);
		if (pInfo)
		{
			CString str;
			pInfo->SetCaption(_T("Undo 0 Actions"));
			pInfo->DelayRedrawParent();
		}


		*pRes = 1;
	}
}
