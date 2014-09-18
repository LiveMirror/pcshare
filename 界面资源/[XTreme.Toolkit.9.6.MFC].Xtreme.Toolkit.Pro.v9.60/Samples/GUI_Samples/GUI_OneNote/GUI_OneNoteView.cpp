// GUI_OneNoteView.cpp : implementation of the CGUI_OneNoteView class
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
#include "GUI_OneNote.h"

#include "GUI_OneNoteDoc.h"
#include "CntrItem.h"
#include "GUI_OneNoteView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUI_OneNoteView

IMPLEMENT_DYNCREATE(CGUI_OneNoteView, CRichEditView)

BEGIN_MESSAGE_MAP(CGUI_OneNoteView, CRichEditView)
	//{{AFX_MSG_MAP(CGUI_OneNoteView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRichEditView::OnFilePrintPreview)

	ON_COMMAND_RANGE(ID_EDIT_SELECT_PAGE, ID_FORMAT_NOTEFLAGS_DEFENITION, OnEmptyCommand)

	ON_COMMAND(ID_VIEW_BOLD, CRichEditView::OnCharBold)
	ON_COMMAND(ID_VIEW_ITALIC, CRichEditView::OnCharItalic)
	ON_COMMAND(ID_VIEW_UNDERLINE, CRichEditView::OnCharUnderline)

	ON_UPDATE_COMMAND_UI(ID_VIEW_BOLD, CRichEditView::OnUpdateCharBold)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ITALIC, CRichEditView::OnUpdateCharItalic)
	ON_UPDATE_COMMAND_UI(ID_VIEW_UNDERLINE, CRichEditView::OnUpdateCharUnderline)

	ON_NOTIFY(XTP_FN_GETFORMAT, ID_EDIT_SIZE, OnGetCharFormat)
	ON_NOTIFY(XTP_FN_SETFORMAT, ID_EDIT_SIZE, OnSetCharFormat)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SIZE, OnUpdateComboSize)

	ON_NOTIFY(XTP_FN_SETFORMAT, ID_EDIT_FONT, OnSetCharFormat)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FONT, OnUpdateComboFont)

	ON_UPDATE_COMMAND_UI(ID_TOOLS_PENACTSAS_SELECT, OnUpdateSetCheck)



	ON_UPDATE_COMMAND_UI(ID_FORMAT_FONTCOLOR_SELECTOR, OnUpdateSelectorText)
	ON_XTP_EXECUTE(ID_FORMAT_FONTCOLOR_SELECTOR, OnSelectorText)

	ON_UPDATE_COMMAND_UI(ID_FORMAT_FONTCOLOR, OnUpdateText)
	ON_COMMAND(ID_FORMAT_FONTCOLOR, OnButtonText)

	ON_UPDATE_COMMAND_UI(ID_FORMAT_HIGHLIGHT_SELECTOR, OnUpdateSelectorBack)

	ON_XTP_EXECUTE(ID_FORMAT_HIGHLIGHT_SELECTOR, OnSelectorBack)

	ON_UPDATE_COMMAND_UI(ID_FORMAT_HIGHLIGHT, OnUpdateBack)
	ON_COMMAND(ID_FORMAT_HIGHLIGHT, OnButtonBack)

	ON_UPDATE_COMMAND_UI(XTP_IDS_AUTOMATIC, OnUpdateTextAuto)
	ON_COMMAND(XTP_IDS_AUTOMATIC, OnTextAuto)

	ON_UPDATE_COMMAND_UI(ID_FORMAT_HIGHLIGHT_NONE, OnUpdateBackNone)
	ON_COMMAND(ID_FORMAT_HIGHLIGHT_NONE, OnBackNone)





END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUI_OneNoteView construction/destruction

CGUI_OneNoteView::CGUI_OneNoteView()
{
	m_clr = RGB(255, 0, 0);
	m_clrBack = RGB(255, 255, 100);

}

CGUI_OneNoteView::~CGUI_OneNoteView()
{
}

BOOL CGUI_OneNoteView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CRichEditView::PreCreateWindow(cs);
}

void CGUI_OneNoteView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();


	// Set the printing margins (720 twips = 1/2 inch).
	SetMargins(CRect(720, 720, 720, 720));
}

/////////////////////////////////////////////////////////////////////////////
// CGUI_OneNoteView printing

BOOL CGUI_OneNoteView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


void CGUI_OneNoteView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
	CRichEditView::OnDestroy();
}


/////////////////////////////////////////////////////////////////////////////
// CGUI_OneNoteView diagnostics

#ifdef _DEBUG
void CGUI_OneNoteView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CGUI_OneNoteView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CGUI_OneNoteDoc* CGUI_OneNoteView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGUI_OneNoteDoc)));
	return (CGUI_OneNoteDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGUI_OneNoteView message handlers
void CGUI_OneNoteView::OnEmptyCommand(UINT)
{

}

void CGUI_OneNoteView::OnUpdateSetCheck(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck();

}



void CGUI_OneNoteView::OnGetCharFormat(NMHDR* pNMHDR, LRESULT* pRes)
{
	ASSERT(pNMHDR != NULL);
	ASSERT(pRes != NULL);
	((NMXTPCHARHDR*)pNMHDR)->cf = GetCharFormatSelection();
	*pRes = 1;
}

void CGUI_OneNoteView::OnSetCharFormat(NMHDR* pNMHDR, LRESULT* pRes)
{
	ASSERT(pNMHDR != NULL);
	ASSERT(pRes != NULL);

#if (_MSC_VER < 1300)
	CHARFORMAT& cf = ((NMXTPCHARHDR*)pNMHDR)->cf;
#else
	CHARFORMAT2 cf;
	MEMCPY_S(&cf, &((NMXTPCHARHDR*)pNMHDR)->cf, sizeof(((NMXTPCHARHDR*)pNMHDR)->cf));
#endif
	SetCharFormat(cf);
	*pRes = 1;
}

void CGUI_OneNoteView::OnUpdateComboFont(CCmdUI* pCmd)
{
	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmd->m_pOther;
	if (pToolBar)
	{
		CXTPControlFontComboBox* pFontCombo = (CXTPControlFontComboBox*)pToolBar->GetControls()->GetAt(pCmd->m_nIndex);

		if (pFontCombo->GetType() == xtpControlComboBox)
		{
			pFontCombo->SetCharFormat((CHARFORMAT&)GetCharFormatSelection());
		}
	}
	pCmd->Enable(TRUE);
}

void CGUI_OneNoteView::OnUpdateComboSize(CCmdUI* pCmd)
{
	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmd->m_pOther;
	if (pToolBar)
	{
		CXTPControlSizeComboBox* pSizeCombo = (CXTPControlSizeComboBox*)pToolBar->GetControls()->GetAt(pCmd->m_nIndex);

		if (pSizeCombo->GetType() == xtpControlComboBox)
		{
			CHARFORMAT& cf = GetCharFormatSelection();
			pSizeCombo->SetTwipSize( (cf.dwMask & CFM_SIZE) ? cf.yHeight : -1);
		}
	}
	pCmd->Enable(TRUE);
}




//////////////////////////////////////////////////////////////////////////


void CGUI_OneNoteView::OnUpdateSelectorText(CCmdUI* pCmd)
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

void CGUI_OneNoteView::OnButtonText()
{
	CHARFORMAT& cfm = GetCharFormatSelection( );
	cfm.dwMask |= CFM_COLOR;
	cfm.dwEffects &= ~CFE_AUTOCOLOR ;
	cfm.crTextColor = m_clr;
	GetRichEditCtrl().SetSelectionCharFormat(cfm);

}
void CGUI_OneNoteView::OnSelectorText(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlColorSelector* pControl = (CXTPControlColorSelector*)tagNMCONTROL->pControl;
	m_clr = pControl->GetColor();

	OnButtonText();

	*pResult = 1;
}


void CGUI_OneNoteView::OnUpdateText(CCmdUI* pCmd)
{

	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmd->m_pOther;
	if (pToolBar)
	{
		CXTPControlPopupColor* pPopup = DYNAMIC_DOWNCAST(CXTPControlPopupColor, pToolBar->GetControls()->GetAt(pCmd->m_nIndex));
		if (pPopup) pPopup->SetColor(m_clr);
	}

	pCmd->Enable(TRUE);
}


///////////////////


void CGUI_OneNoteView::OnUpdateSelectorBack(CCmdUI* pCmd)
{
	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmd->m_pOther;
	if (pToolBar)
	{

		CHARFORMAT2 cfm;
		cfm.cbSize =  sizeof(CHARFORMAT2);
		::SendMessage(GetRichEditCtrl().GetSafeHwnd(), EM_GETCHARFORMAT, 1, (LPARAM)&cfm);

		CXTPControlColorSelector* pSelector = (CXTPControlColorSelector*)pToolBar->GetControls()->GetAt(pCmd->m_nIndex);

		pSelector->SetColor(cfm.dwEffects & CFE_AUTOBACKCOLOR? -1: cfm.crBackColor);
	}
	pCmd->Enable(TRUE);
}

void CGUI_OneNoteView::OnButtonBack()
{
	CHARFORMAT2 cfm;
	cfm.cbSize =  sizeof(CHARFORMAT2);
	::SendMessage(GetRichEditCtrl().GetSafeHwnd(), EM_GETCHARFORMAT, 1, (LPARAM)&cfm);

	cfm.dwMask |= CFM_BACKCOLOR;
	cfm.dwEffects &= ~CFE_AUTOBACKCOLOR ;
	cfm.crBackColor = m_clrBack;
	::SendMessage(GetRichEditCtrl().GetSafeHwnd(), EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfm);

}
void CGUI_OneNoteView::OnSelectorBack(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlColorSelector* pControl = (CXTPControlColorSelector*)tagNMCONTROL->pControl;
	m_clrBack = pControl->GetColor();

	OnButtonBack();

	*pResult = 1;
}



void CGUI_OneNoteView::OnUpdateBack(CCmdUI* pCmd)
{

	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmd->m_pOther;
	if (pToolBar)
	{
		CXTPControlPopupColor* pPopup = DYNAMIC_DOWNCAST(CXTPControlPopupColor, pToolBar->GetControls()->GetAt(pCmd->m_nIndex));
		if (pPopup) pPopup->SetColor(m_clrBack);
	}

	pCmd->Enable(TRUE);
}



void CGUI_OneNoteView::OnTextAuto()
{
	CHARFORMAT& cfm = GetCharFormatSelection( );
	cfm.dwMask |= CFM_COLOR;
	cfm.dwEffects |= CFE_AUTOCOLOR ;
	GetRichEditCtrl().SetSelectionCharFormat(cfm);
}

void CGUI_OneNoteView::OnUpdateTextAuto(CCmdUI* pCmd)
{
	CHARFORMAT& cfm = GetCharFormatSelection( );
	pCmd->SetCheck(cfm.dwEffects & CFE_AUTOCOLOR? TRUE: FALSE);
}


void CGUI_OneNoteView::OnBackNone()
{
	CHARFORMAT2 cfm;
	cfm.cbSize =  sizeof(CHARFORMAT2);
	::SendMessage(GetRichEditCtrl().GetSafeHwnd(), EM_GETCHARFORMAT, 1, (LPARAM)&cfm);

	cfm.dwMask |= CFM_BACKCOLOR;
	cfm.dwEffects |= CFE_AUTOBACKCOLOR ;
	::SendMessage(GetRichEditCtrl().GetSafeHwnd(), EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfm);

}

void CGUI_OneNoteView::OnUpdateBackNone(CCmdUI* pCmd)
{
	CHARFORMAT2 cfm;
	cfm.cbSize =  sizeof(CHARFORMAT2);
	::SendMessage(GetRichEditCtrl().GetSafeHwnd(), EM_GETCHARFORMAT, 1, (LPARAM)&cfm);

	pCmd->SetCheck(cfm.dwEffects & CFE_AUTOBACKCOLOR? TRUE: FALSE);
}
