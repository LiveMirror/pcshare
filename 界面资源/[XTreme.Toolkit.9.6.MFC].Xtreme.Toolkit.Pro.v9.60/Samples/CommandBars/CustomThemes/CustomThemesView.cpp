// CustomThemesView.cpp : implementation of the CCustomThemesView class
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
#include "CustomThemes.h"

#include "CustomThemesDoc.h"
#include "CustomThemesView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomThemesView

IMPLEMENT_DYNCREATE(CCustomThemesView, CRichEditView)

BEGIN_MESSAGE_MAP(CCustomThemesView, CRichEditView)
	//{{AFX_MSG_MAP(CCustomThemesView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP

	ON_COMMAND(ID_TOOLS_HEX, OnToolsHex)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_HEX, OnUpdateToolsHex)

	ON_UPDATE_COMMAND_UI(ID_SELECTOR_TEXT, OnUpdateSelectorText)
	ON_XTP_EXECUTE(ID_SELECTOR_TEXT, OnSelectorText)

	ON_UPDATE_COMMAND_UI(ID_BUTTON_TEXT, OnUpdateText)
	ON_COMMAND(ID_BUTTON_TEXT, OnButtonText)

	ON_UPDATE_COMMAND_UI(ID_SELECTOR_BACK, OnUpdateSelectorBack)

	ON_XTP_EXECUTE(ID_SELECTOR_BACK, OnSelectorBack)

	ON_UPDATE_COMMAND_UI(ID_BUTTON_BACK, OnUpdateBack)
	ON_COMMAND(ID_BUTTON_BACK, OnButtonBack)

	ON_UPDATE_COMMAND_UI(ID_TEXT_AUTO, OnUpdateTextAuto)
	ON_COMMAND(ID_TEXT_AUTO, OnTextAuto)

	ON_UPDATE_COMMAND_UI(ID_BACK_NONE, OnUpdateBackNone)
	ON_COMMAND(ID_BACK_NONE, OnBackNone)

	ON_COMMAND(ID_TEXT_MORE, OnTextMore)
	ON_COMMAND(ID_BACK_MORE, OnBackMore)

	ON_XTP_EXECUTE(ID_EDIT_FIND, OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)


	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_EX, CRichEditView::OnFilePrint)

	ON_COMMAND(ID_FILE_PRINT_DIRECT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRichEditView::OnFilePrintPreview)

	ON_NOTIFY(XTP_FN_SETFORMAT, ID_COMBO_OWNERDRAW, OnSetCharFormat)
	ON_UPDATE_COMMAND_UI(ID_COMBO_OWNERDRAW, OnUpdateComboFont)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomThemesView construction/destruction

CCustomThemesView::CCustomThemesView()
{
	m_bCheck = TRUE;
	m_clr = RGB(255, 0, 0);
	m_clrBack = RGB(255, 255, 100);


}

CCustomThemesView::~CCustomThemesView()
{
}

BOOL CCustomThemesView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CRichEditView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCustomThemesView drawing

void CCustomThemesView::OnDraw(CDC* /*pDC*/)
{
	CCustomThemesDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CCustomThemesView printing

BOOL CCustomThemesView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


/////////////////////////////////////////////////////////////////////////////
// CCustomThemesView diagnostics

#ifdef _DEBUG
void CCustomThemesView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CCustomThemesView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CCustomThemesDoc* CCustomThemesView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCustomThemesDoc)));
	return (CCustomThemesDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCustomThemesView message handlers




void CCustomThemesView::OnToolsHex()
{
	m_bCheck = !m_bCheck;
}

void CCustomThemesView::OnUpdateToolsHex(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bCheck);

}

void CCustomThemesView::OnRButtonDown(UINT /*nFlags*/, CPoint point)
{
	ClientToScreen(&point);
	CMenu mnu;
	mnu.LoadMenu(IDR_MAINFRAME);
	CXTPCommandBars::TrackPopupMenu(&mnu, TPM_RIGHTBUTTON , point.x, point.y, AfxGetMainWnd(), 0, 0, ((CXTPMDIFrameWnd*)AfxGetMainWnd())->GetCommandBars());
}

void CCustomThemesView::OnUpdateSelectorText(CCmdUI* pCmdUI)
{
	CXTPControlColorSelector* pSelector = DYNAMIC_DOWNCAST(CXTPControlColorSelector, CXTPControl::FromUI(pCmdUI));

	if (pSelector)
	{
		CHARFORMAT& cfm = GetCharFormatSelection( );

		pSelector->SetColor(cfm.dwEffects & CFE_AUTOCOLOR? -1: cfm.crTextColor);
	}
	pCmdUI->Enable(TRUE);
}

void CCustomThemesView::OnButtonText()
{
	CHARFORMAT& cfm = GetCharFormatSelection( );
	cfm.dwMask |= CFM_COLOR;
	cfm.dwEffects &= ~CFE_AUTOCOLOR ;
	cfm.crTextColor = m_clr;
	GetRichEditCtrl().SetSelectionCharFormat(cfm);

}
void CCustomThemesView::OnSelectorText(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlColorSelector* pControl = (CXTPControlColorSelector*)tagNMCONTROL->pControl;
	m_clr = pControl->GetColor();

	OnButtonText();

	*pResult = 1;
}


void CCustomThemesView::OnUpdateText(CCmdUI* pCmdUI)
{

	CXTPControlPopupColor* pPopup = DYNAMIC_DOWNCAST(CXTPControlPopupColor, CXTPControl::FromUI(pCmdUI));
	if (pPopup)
	{
		pPopup->SetColor(m_clr);
	}

	pCmdUI->Enable(TRUE);
}


///////////////////


void CCustomThemesView::OnUpdateSelectorBack(CCmdUI* pCmdUI)
{
	CXTPControlColorSelector* pSelector = DYNAMIC_DOWNCAST(CXTPControlColorSelector, CXTPControl::FromUI(pCmdUI));

	if (pSelector)
	{

		CHARFORMAT2 cfm;
		cfm.cbSize =  sizeof(CHARFORMAT2);
		::SendMessage(GetRichEditCtrl().GetSafeHwnd(), EM_GETCHARFORMAT, 1, (LPARAM)&cfm);

		pSelector->SetColor(cfm.dwEffects & CFE_AUTOBACKCOLOR? -1: cfm.crBackColor);
	}
	pCmdUI->Enable(TRUE);
}

void CCustomThemesView::OnButtonBack()
{
	CHARFORMAT2 cfm;
	cfm.cbSize =  sizeof(CHARFORMAT2);
	::SendMessage(GetRichEditCtrl().GetSafeHwnd(), EM_GETCHARFORMAT, 1, (LPARAM)&cfm);

	cfm.dwMask |= CFM_BACKCOLOR;
	cfm.dwEffects &= ~CFE_AUTOBACKCOLOR ;
	cfm.crBackColor = m_clrBack;
	::SendMessage(GetRichEditCtrl().GetSafeHwnd(), EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfm);

}
void CCustomThemesView::OnSelectorBack(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlColorSelector* pControl = (CXTPControlColorSelector*)tagNMCONTROL->pControl;
	m_clrBack = pControl->GetColor();

	OnButtonBack();

	*pResult = 1;
}



void CCustomThemesView::OnUpdateBack(CCmdUI* pCmdUI)
{

	CXTPControlPopupColor* pPopup = DYNAMIC_DOWNCAST(CXTPControlPopupColor, CXTPControl::FromUI(pCmdUI));
	if (pPopup)
	{
		pPopup->SetColor(m_clrBack);
	}

	pCmdUI->Enable(TRUE);
}



void CCustomThemesView::OnTextAuto()
{
	CHARFORMAT& cfm = GetCharFormatSelection( );
	cfm.dwMask |= CFM_COLOR;
	cfm.dwEffects |= CFE_AUTOCOLOR ;
	GetRichEditCtrl().SetSelectionCharFormat(cfm);
}

void CCustomThemesView::OnUpdateTextAuto(CCmdUI* pCmd)
{
	CHARFORMAT& cfm = GetCharFormatSelection( );
	pCmd->SetCheck(cfm.dwEffects & CFE_AUTOCOLOR? TRUE: FALSE);
}


void CCustomThemesView::OnBackNone()
{
	CHARFORMAT2 cfm;
	cfm.cbSize =  sizeof(CHARFORMAT2);
	::SendMessage(GetRichEditCtrl().GetSafeHwnd(), EM_GETCHARFORMAT, 1, (LPARAM)&cfm);

	cfm.dwMask |= CFM_BACKCOLOR;
	cfm.dwEffects |= CFE_AUTOBACKCOLOR ;
	::SendMessage(GetRichEditCtrl().GetSafeHwnd(), EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfm);

}

void CCustomThemesView::OnUpdateBackNone(CCmdUI* pCmd)
{
	CHARFORMAT2 cfm;
	cfm.cbSize =  sizeof(CHARFORMAT2);
	::SendMessage(GetRichEditCtrl().GetSafeHwnd(), EM_GETCHARFORMAT, 1, (LPARAM)&cfm);

	pCmd->SetCheck(cfm.dwEffects & CFE_AUTOBACKCOLOR? TRUE: FALSE);
}

void CCustomThemesView::OnTextMore()
{
	CColorDialog cd(m_clr);
	if (cd.DoModal())
	{
		m_clr = cd.GetColor();
		OnButtonText();
	}

}

void CCustomThemesView::OnBackMore()
{
	CColorDialog cd(m_clrBack);
	if (cd.DoModal())
	{
		m_clrBack = cd.GetColor();
		OnButtonBack();
	}

}




void CCustomThemesView::OnEditFind(NMHDR* pNMHDR, LRESULT* pResult)
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

void CCustomThemesView::OnUpdateEditFind(CCmdUI* pCmd)
{
	pCmd->Enable(TRUE);
}


void CCustomThemesView::OnSetCharFormat(NMHDR* pNMHDR, LRESULT* pRes)
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

void CCustomThemesView::OnUpdateComboFont(CCmdUI* pCmd)
{
	CXTPControlFontComboBox* pFontCombo = (CXTPControlFontComboBox*)CXTPControl::FromUI(pCmd);

	if (pFontCombo && pFontCombo->GetType() == xtpControlComboBox)
	{
		pFontCombo->SetCharFormat((CHARFORMAT&)GetCharFormatSelection());
	}
	pCmd->Enable(TRUE);
}
