// TearOffPopupsView.cpp : implementation of the CTearOffPopupsView class
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
#include "TearOffPopups.h"

#include "TearOffPopupsDoc.h"
#include "CntrItem.h"
#include "TearOffPopupsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTearOffPopupsView

IMPLEMENT_DYNCREATE(CTearOffPopupsView, CRichEditView)

BEGIN_MESSAGE_MAP(CTearOffPopupsView, CRichEditView)
	//{{AFX_MSG_MAP(CTearOffPopupsView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRichEditView::OnFilePrintPreview)

	ON_COMMAND(ID_FORMAT_STYLE_BOLD, OnCharBold)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_STYLE_BOLD, OnUpdateCharBold)

	ON_COMMAND(ID_FORMAT_STYLE_ITALIC, OnCharItalic)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_STYLE_ITALIC, OnUpdateCharItalic)

	ON_COMMAND(ID_FORMAT_STYLE_UNDERLINE, OnCharUnderline)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_STYLE_UNDERLINE, OnUpdateCharUnderline)


	ON_UPDATE_COMMAND_UI(XTP_IDS_AUTOMATIC, OnUpdateTextAuto)
	ON_COMMAND(XTP_IDS_AUTOMATIC, OnTextAuto)

	ON_COMMAND(XTP_IDS_MORE_COLORS, OnTextMore)

	ON_COMMAND(ID_BUTTON_TEXT, OnButtonText)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_TEXT, OnUpdateText)

	ON_XTP_EXECUTE(ID_SELECTOR_TEXT, OnSelectorText)
	ON_UPDATE_COMMAND_UI(ID_SELECTOR_TEXT, OnUpdateSelectorText)


END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTearOffPopupsView construction/destruction

CTearOffPopupsView::CTearOffPopupsView()
{
	m_clrText = RGB(255, 0, 0);

}

CTearOffPopupsView::~CTearOffPopupsView()
{
}

BOOL CTearOffPopupsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CRichEditView::PreCreateWindow(cs);
}

void CTearOffPopupsView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();


	// Set the printing margins (720 twips = 1/2 inch).
	SetMargins(CRect(720, 720, 720, 720));
}

/////////////////////////////////////////////////////////////////////////////
// CTearOffPopupsView printing

BOOL CTearOffPopupsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


void CTearOffPopupsView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
	CRichEditView::OnDestroy();
}


/////////////////////////////////////////////////////////////////////////////
// CTearOffPopupsView diagnostics

#ifdef _DEBUG
void CTearOffPopupsView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CTearOffPopupsView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CTearOffPopupsDoc* CTearOffPopupsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTearOffPopupsDoc)));
	return (CTearOffPopupsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTearOffPopupsView message handlers






void CTearOffPopupsView::OnTextAuto()
{
	CHARFORMAT& cfm = GetCharFormatSelection( );
	cfm.dwMask |= CFM_COLOR;
	cfm.dwEffects |= CFE_AUTOCOLOR ;
	GetRichEditCtrl().SetSelectionCharFormat(cfm);
}

void CTearOffPopupsView::OnUpdateTextAuto(CCmdUI* pCmd)
{
	CHARFORMAT& cfm = GetCharFormatSelection( );
	pCmd->SetCheck(cfm.dwEffects & CFE_AUTOCOLOR? TRUE: FALSE);
}


void CTearOffPopupsView::OnTextMore()
{
	CColorDialog cd(m_clrText);
	if (cd.DoModal())
	{
		m_clrText = cd.GetColor();
		OnButtonText();
	}

}

void CTearOffPopupsView::OnButtonText()
{
	CHARFORMAT& cfm = GetCharFormatSelection( );
	cfm.dwMask |= CFM_COLOR;
	cfm.dwEffects &= ~CFE_AUTOCOLOR ;
	cfm.crTextColor = m_clrText;
	GetRichEditCtrl().SetSelectionCharFormat(cfm);

}


void CTearOffPopupsView::OnUpdateText(CCmdUI* pCmd)
{

	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmd->m_pOther;
	if (pToolBar)
	{
		CXTPControlPopupColor* pPopup = (CXTPControlPopupColor*)pToolBar->GetControls()->GetAt(pCmd->m_nIndex);
		pPopup->SetColor(m_clrText);
	}

	pCmd->Enable(TRUE);
}

void CTearOffPopupsView::OnSelectorText(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlColorSelector* pControl = (CXTPControlColorSelector*)tagNMCONTROL->pControl;
	m_clrText = pControl->GetColor();

	OnButtonText();

	*pResult = 1;
}


void CTearOffPopupsView::OnUpdateSelectorText(CCmdUI* pCmd)
{
	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmd->m_pOther;
	if (pToolBar)
	{
		CHARFORMAT& cfm = GetCharFormatSelection( );
		CXTPControlColorSelector* pSelector = (CXTPControlColorSelector*)pToolBar->GetControls()->GetAt(pCmd->m_nIndex);
		pSelector->SetColor(cfm.dwEffects & CFE_AUTOCOLOR? -1: cfm.crTextColor);
	}
	pCmd->Enable(TRUE);
}
