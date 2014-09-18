// wordpvw.cpp : implementation of the CWordPadView class
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
#include "wordpad.h"
#include "cntritem.h"
#include "srvritem.h"

#include "wordpdoc.h"
#include "wordpvw.h"
#include "formatta.h"
#include "datedial.h"
#include "formatpa.h"
#include "ruler.h"
#include "strings.h"
#include "colorlis.h"
#include "pageset.h"
#include <penwin.h>

#include "ControlSelector.h"

extern CLIPFORMAT cfEmbeddedObject;
extern CLIPFORMAT cfRTO;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

BOOL CCharFormat::operator==(CCharFormat& cf)
{
	return
	dwMask == cf.dwMask
	&& dwEffects == cf.dwEffects
	&& yHeight == cf.yHeight
	&& yOffset == cf.yOffset
	&& crTextColor == cf.crTextColor
	&& bPitchAndFamily == cf.bPitchAndFamily
#if (_MSC_VER < 1300)
	&& (lstrcmpA(szFaceName, cf.szFaceName) == 0);
#else
	&& (lstrcmp(szFaceName, cf.szFaceName) == 0);
#endif
}

BOOL CParaFormat::operator==(PARAFORMAT& pf)
{
	if(
		dwMask != pf.dwMask
		|| wNumbering != pf.wNumbering
		|| wReserved != pf.wReserved
		|| dxStartIndent != pf.dxStartIndent
		|| dxRightIndent != pf.dxRightIndent
		|| dxOffset != pf.dxOffset
		|| cTabCount != pf.cTabCount
		)
	{
		return FALSE;
	}
	for (int i=0;i<pf.cTabCount;i++)
	{
		if (rgxTabs[i] != pf.rgxTabs[i])
			return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CWordPadView

IMPLEMENT_DYNCREATE(CWordPadView, CRichEditView)

//WM_SETTINGCHANGE -- default printer might have changed
//WM_FONTCHANGE -- pool of fonts changed
//WM_DEVMODECHANGE -- printer settings changes

BEGIN_MESSAGE_MAP(CWordPadView, CRichEditView)
	ON_COMMAND(ID_CANCEL_EDIT_CNTR, OnCancelEditCntr)
	ON_COMMAND(ID_CANCEL_EDIT_SRVR, OnCancelEditSrvr)
	//{{AFX_MSG_MAP(CWordPadView)
	ON_COMMAND(ID_PAGE_SETUP, OnPageSetup)
	ON_COMMAND(ID_CHAR_BOLD, OnCharBold)
	ON_UPDATE_COMMAND_UI(ID_CHAR_BOLD, OnUpdateCharBold)
	ON_COMMAND(ID_CHAR_ITALIC, OnCharItalic)
	ON_UPDATE_COMMAND_UI(ID_CHAR_ITALIC, OnUpdateCharItalic)
	ON_COMMAND(ID_CHAR_UNDERLINE, OnCharUnderline)
	ON_UPDATE_COMMAND_UI(ID_CHAR_UNDERLINE, OnUpdateCharUnderline)
	ON_COMMAND(ID_PARA_CENTER, OnParaCenter)
	ON_UPDATE_COMMAND_UI(ID_PARA_CENTER, OnUpdateParaCenter)
	ON_COMMAND(ID_PARA_LEFT, OnParaLeft)
	ON_UPDATE_COMMAND_UI(ID_PARA_LEFT, OnUpdateParaLeft)
	ON_COMMAND(ID_PARA_RIGHT, OnParaRight)
	ON_UPDATE_COMMAND_UI(ID_PARA_RIGHT, OnUpdateParaRight)
	ON_WM_CREATE()
	ON_COMMAND(ID_INSERT_DATE_TIME, OnInsertDateTime)
	ON_COMMAND(ID_FORMAT_PARAGRAPH, OnFormatParagraph)
	ON_COMMAND(ID_FORMAT_TABS, OnFormatTabs)
	ON_COMMAND(ID_COLOR16, OnColorDefault)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_MEASUREITEM()
	ON_COMMAND(ID_PEN_BACKSPACE, OnPenBackspace)
	ON_COMMAND(ID_PEN_NEWLINE, OnPenNewline)
	ON_COMMAND(ID_PEN_PERIOD, OnPenPeriod)
	ON_COMMAND(ID_PEN_SPACE, OnPenSpace)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_PEN_LENS, OnPenLens)
	ON_COMMAND(ID_PEN_TAB, OnPenTab)
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_SETTINGCHANGE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_INSERT_BULLET, CRichEditView::OnBullet)
	ON_UPDATE_COMMAND_UI(ID_INSERT_BULLET, CRichEditView::OnUpdateBullet)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRichEditView::OnFilePrintPreview)
	ON_COMMAND_RANGE(ID_COLOR0, ID_COLOR16, OnColorPick)
	ON_EN_CHANGE(AFX_IDW_PANE_FIRST, OnEditChange)
	ON_WM_MOUSEACTIVATE()
	ON_REGISTERED_MESSAGE(CWordPadApp::m_nPrinterChangedMsg, OnPrinterChangedMsg)


	ON_NOTIFY(XTP_FN_GETFORMAT, ID_COMBO_SIZE, OnGetCharFormat)
	ON_NOTIFY(XTP_FN_SETFORMAT, ID_COMBO_SIZE, OnSetCharFormat)
	ON_UPDATE_COMMAND_UI(ID_COMBO_SIZE, OnUpdateComboSize)

	ON_NOTIFY(XTP_FN_SETFORMAT, ID_COMBO_FONT, OnSetCharFormat)
	ON_UPDATE_COMMAND_UI(ID_COMBO_FONT, OnUpdateComboFont)

	ON_XTP_EXECUTE(ID_SELECTOR_TEXT, OnSelectorText)
	ON_UPDATE_COMMAND_UI(ID_SELECTOR_TEXT, OnUpdateSelectorText)

	ON_COMMAND(ID_BUTTON_TEXT, OnButtonText)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_TEXT, OnUpdateText)

	ON_COMMAND(XTP_IDS_AUTOMATIC, OnTextAuto)
	ON_UPDATE_COMMAND_UI(XTP_IDS_AUTOMATIC, OnUpdateTextAuto)

	ON_COMMAND(XTP_IDS_MORE_COLORS, OnTextMore)


	ON_XTP_EXECUTE(ID_EDIT_UNDO, OnEditUndo)
	ON_NOTIFY(XTP_LBN_SELCHANGE, ID_EDIT_UNDO, OnListBoxControlSelChange)
	ON_NOTIFY(XTP_LBN_POPUP, ID_EDIT_UNDO, OnListBoxControlPoup)


	ON_NOTIFY(XTP_SLN_SELCHANGE, ID_INSERT_COLUMNS, OnColumnsSelChange)
	ON_NOTIFY(XTP_SLN_SELCHANGE, ID_INSERT_TABLE, OnTableSelChange)

	ON_XTP_EXECUTE(ID_INSERT_TABLE, OnInsertTable)
	ON_XTP_EXECUTE(ID_INSERT_COLUMNS, OnInsertColumns)


END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWordPadView construction/destruction

CWordPadView::CWordPadView()
{
	m_bSyncCharFormat = m_bSyncParaFormat = TRUE;
	m_uTimerID = 0;
	m_bDelayUpdateItems = FALSE;
	m_bOnBar = FALSE;
	m_bInPrint = FALSE;
	m_nPasteType = 0;
	m_rectMargin = theApp.m_rectPageMargin;

	m_clr = (COLORREF)-1;
}

BOOL CWordPadView::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL bRes = CRichEditView::PreCreateWindow(cs);
	cs.style |= ES_SELECTIONBAR;
	return bRes;
}

/////////////////////////////////////////////////////////////////////////////
// CWordPadView attributes

BOOL CWordPadView::IsFormatText()
{
	// this function checks to see if any formatting is not default text
	BOOL bRes = FALSE;
	CHARRANGE cr;
	CCharFormat cf;
	CParaFormat pf;
	GetRichEditCtrl().GetSel(cr);
	GetRichEditCtrl().HideSelection(TRUE, FALSE);
	GetRichEditCtrl().SetSel(0,-1);

	if (!(GetRichEditCtrl().GetSelectionType() & (SEL_OBJECT|SEL_MULTIOBJECT)))
	{
		GetRichEditCtrl().GetSelectionCharFormat(cf);
		if (cf == m_defTextCharFormat)
		{
			GetRichEditCtrl().GetParaFormat(pf);
			if (pf == m_defParaFormat) //compared using CParaFormat::operator==
				bRes = TRUE;
		}
	}

	GetRichEditCtrl().SetSel(cr);
	GetRichEditCtrl().HideSelection(FALSE, FALSE);
	return bRes;
}

HMENU CWordPadView::GetContextMenu(WORD, LPOLEOBJECT, CHARRANGE* )
{
	CRichEditCntrItem* pItem = GetSelectedItem();
	if (pItem == NULL || !pItem->IsInPlaceActive())
	{
		CMenu menuText;
		menuText.LoadMenu(IDR_TEXT_POPUP);
		CMenu* pMenuPopup = menuText.GetSubMenu(0);
		menuText.RemoveMenu(0, MF_BYPOSITION);
		if (!GetSystemMetrics(SM_PENWINDOWS))
		{
			//delete pen specific stuff
			// remove Insert Keystrokes
			pMenuPopup->DeleteMenu(ID_PEN_LENS, MF_BYCOMMAND);
			int nIndex = pMenuPopup->GetMenuItemCount()-1; //index of last item
			// remove Edit Text...
			pMenuPopup->DeleteMenu(nIndex, MF_BYPOSITION);
			// remove separator
			pMenuPopup->DeleteMenu(nIndex-1, MF_BYPOSITION);
		}
		return pMenuPopup->Detach();
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CWordPadView operations

void CWordPadView::WrapChanged()
{
	CWaitCursor wait;
	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT(pFrameWnd != NULL);
	pFrameWnd->SetMessageText(IDS_FORMATTING);
	CWnd* pBarWnd = pFrameWnd->GetMessageBar();
	if (pBarWnd != NULL)
		pBarWnd->UpdateWindow();

	CRichEditView::WrapChanged();

	pFrameWnd->SetMessageText(AFX_IDS_IDLEMESSAGE);
	if (pBarWnd != NULL)
		pBarWnd->UpdateWindow();
}

void CWordPadView::SetUpdateTimer()
{
	if (m_uTimerID != 0) // if outstanding timer kill it
		KillTimer(m_uTimerID);
	m_uTimerID = (UINT)SetTimer(1, 1000, NULL); //set a timer for 1000 milliseconds
	if (m_uTimerID == 0) // no timer available so force update now
		GetDocument()->UpdateAllItems(NULL);
	else
		m_bDelayUpdateItems = TRUE;
}

void CWordPadView::DeleteContents()
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
	CRichEditView::DeleteContents();
	SetDefaultFont(IsTextType(GetDocument()->m_nNewDocType));
}

void CWordPadView::SetDefaultFont(BOOL bText)
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
	m_bSyncCharFormat = m_bSyncParaFormat = TRUE;
	CHARFORMAT* pCharFormat = bText ? &m_defTextCharFormat : &m_defCharFormat;
	// set the default character format -- the FALSE makes it the default
	GetRichEditCtrl().SetSel(0,-1);
	GetRichEditCtrl().SetDefaultCharFormat(*pCharFormat);
	GetRichEditCtrl().SetSelectionCharFormat(*pCharFormat);

	GetRichEditCtrl().SetParaFormat(m_defParaFormat);

	GetRichEditCtrl().SetSel(0,0);
	GetRichEditCtrl().EmptyUndoBuffer();
	GetRichEditCtrl().SetModify(FALSE);
	ASSERT_VALID(this);
}

/////////////////////////////////////////////////////////////////////////////
// CWordPadView drawing

/////////////////////////////////////////////////////////////////////////////
// CWordPadView printing

void CWordPadView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CRichEditView::OnPrint(pDC, pInfo);
	if (pInfo != NULL && pInfo->m_bPreview)
		DrawMargins(pDC);
}

void CWordPadView::DrawMargins(CDC* pDC)
{
	if (pDC->m_hAttribDC != NULL)
	{
		CRect rect;
		rect.left = m_rectMargin.left;
		rect.right = m_sizePaper.cx - m_rectMargin.right;
		rect.top = m_rectMargin.top;
		rect.bottom = m_sizePaper.cy - m_rectMargin.bottom;
		//rect in twips
		int logx = ::GetDeviceCaps(pDC->m_hDC, LOGPIXELSX);
		int logy = ::GetDeviceCaps(pDC->m_hDC, LOGPIXELSY);
		rect.left = MulDiv(rect.left, logx, 1440);
		rect.right = MulDiv(rect.right, logx, 1440);
		rect.top = MulDiv(rect.top, logy, 1440);
		rect.bottom = MulDiv(rect.bottom, logy, 1440);
		CPen pen(PS_DOT, 0, pDC->GetTextColor());
		CPen* ppen = pDC->SelectObject(&pen);
		pDC->MoveTo(0, rect.top);
		pDC->LineTo(10000, rect.top);
		pDC->MoveTo(rect.left, 0);
		pDC->LineTo(rect.left, 10000);
		pDC->MoveTo(0, rect.bottom);
		pDC->LineTo(10000, rect.bottom);
		pDC->MoveTo(rect.right, 0);
		pDC->LineTo(rect.right, 10000);
		pDC->SelectObject(ppen);
	}
}

BOOL CWordPadView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// OLE Client support and commands

inline int roundleast(int n)
{
	int mod = n%10;
	n -= mod;
	if (mod >= 5)
		n += 10;
	else if (mod <= -5)
		n -= 10;
	return n;
}

static void RoundRect(LPRECT r1)
{
	r1->left = roundleast(r1->left);
	r1->right = roundleast(r1->right);
	r1->top = roundleast(r1->top);
	r1->bottom = roundleast(r1->bottom);
}

static void MulDivRect(LPRECT r1, LPRECT r2, int num, int div)
{
	r1->left = MulDiv(r2->left, num, div);
	r1->top = MulDiv(r2->top, num, div);
	r1->right = MulDiv(r2->right, num, div);
	r1->bottom = MulDiv(r2->bottom, num, div);
}

void CWordPadView::OnPageSetup()
{
	CPageSetupDialog dlg;
	PAGESETUPDLG& psd = dlg.m_psd;
	BOOL bMetric = theApp.GetUnits() == 1; //centimeters
	psd.Flags |= PSD_MARGINS | (bMetric ? PSD_INHUNDREDTHSOFMILLIMETERS :
		PSD_INTHOUSANDTHSOFINCHES);
	int nUnitsPerInch = bMetric ? 2540 : 1000;
	MulDivRect(&psd.rtMargin, m_rectMargin, nUnitsPerInch, 1440);
	RoundRect(&psd.rtMargin);
	// get the current device from the app
	PRINTDLG pd;
	pd.hDevNames = NULL;
	pd.hDevMode = NULL;
	theApp.GetPrinterDeviceDefaults(&pd);
	psd.hDevNames = pd.hDevNames;
	psd.hDevMode = pd.hDevMode;
	if (dlg.DoModal() == IDOK)
	{
		RoundRect(&psd.rtMargin);
		MulDivRect(m_rectMargin, &psd.rtMargin, 1440, nUnitsPerInch);
		theApp.m_rectPageMargin = m_rectMargin;
		theApp.SelectPrinter(psd.hDevNames, psd.hDevMode);
		theApp.NotifyPrinterChanged();
	}
	// PageSetupDlg failed
	if (CommDlgExtendedError() != 0)
	{
		CPageSetupDlg dlg;
		dlg.m_nBottomMargin = m_rectMargin.bottom;
		dlg.m_nLeftMargin = m_rectMargin.left;
		dlg.m_nRightMargin = m_rectMargin.right;
		dlg.m_nTopMargin = m_rectMargin.top;
		if (dlg.DoModal() == IDOK)
		{
			m_rectMargin.SetRect(dlg.m_nLeftMargin, dlg.m_nTopMargin,
				dlg.m_nRightMargin, dlg.m_nBottomMargin);
			// m_page will be changed at this point
			theApp.m_rectPageMargin = m_rectMargin;
			theApp.NotifyPrinterChanged();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// OLE Server support

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the server (not the container) causes the deactivation.
void CWordPadView::OnCancelEditSrvr()
{
	GetDocument()->OnDeactivateUI(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CWordPadView diagnostics

#ifdef _DEBUG
void CWordPadView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CWordPadView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CWordPadDoc* CWordPadView::GetDocument() // non-debug version is inline
{
	return (CWordPadDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWordPadView message helpers

/////////////////////////////////////////////////////////////////////////////
// CWordPadView message handlers

int CWordPadView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CRichEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	theApp.m_listPrinterNotify.AddTail(m_hWnd);

	if (theApp.m_bWordSel)
		GetRichEditCtrl().SetOptions(ECOOP_OR, ECO_AUTOWORDSELECTION);
	else
		GetRichEditCtrl().SetOptions(ECOOP_AND, ~(DWORD)ECO_AUTOWORDSELECTION);
//  GetRichEditCtrl().SetOptions(ECOOP_OR, ECO_SELECTIONBAR);

	GetDefaultFont(m_defTextCharFormat, IDS_DEFAULTTEXTFONT);
	GetDefaultFont(m_defCharFormat, IDS_DEFAULTFONT);

	GetRichEditCtrl().GetParaFormat(m_defParaFormat);
	m_defParaFormat.cTabCount = 0;

	return 0;
}

void CWordPadView::GetDefaultFont(CCharFormat& cf, UINT nFontNameID)
{
	USES_CONVERSION;
	CString strDefFont;
	VERIFY(strDefFont.LoadString(nFontNameID));
	ASSERT(cf.cbSize == sizeof(CHARFORMAT));
	cf.dwMask = CFM_BOLD|CFM_ITALIC|CFM_UNDERLINE|CFM_STRIKEOUT|CFM_SIZE|
		CFM_COLOR|CFM_OFFSET|CFM_PROTECTED;
	cf.dwEffects = CFE_AUTOCOLOR;
	cf.yHeight = 200; //10pt
	cf.yOffset = 0;
	cf.crTextColor = RGB(0, 0, 0);
	cf.bCharSet = 0;
	cf.bPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	ASSERT(strDefFont.GetLength() < LF_FACESIZE);
#if (_MSC_VER < 1300)
	lstrcpynA(cf.szFaceName, T2A((LPTSTR) (LPCTSTR) strDefFont), LF_FACESIZE);
#else
	lstrcpyn(cf.szFaceName, strDefFont, LF_FACESIZE);
#endif
	cf.dwMask |= CFM_FACE;
}

void CWordPadView::OnInsertDateTime()
{
	CDateDialog dlg;
	if (dlg.DoModal() == IDOK)
		GetRichEditCtrl().ReplaceSel(dlg.m_strSel);;
}

void CWordPadView::OnFormatParagraph()
{
	CFormatParaDlg dlg(GetParaFormatSelection());
	dlg.m_nWordWrap = m_nWordWrap;
	if (dlg.DoModal() == IDOK)
		SetParaFormat(dlg.m_pf);
}

void CWordPadView::OnFormatTabs()
{
	CFormatTabDlg dlg(GetParaFormatSelection());
	if (dlg.DoModal() == IDOK)
		SetParaFormat(dlg.m_pf);
}

void CWordPadView::OnTextNotFound(LPCTSTR lpStr)
{
	ASSERT_VALID(this);
	MessageBeep(0);
	AfxMessageBox(IDS_FINISHED_SEARCH,MB_OK|MB_ICONINFORMATION);
	CRichEditView::OnTextNotFound(lpStr);
}

void CWordPadView::OnColorPick(UINT nID)
{
	CRichEditView::OnColorPick(CColorMenu::GetColor(nID));
}

void CWordPadView::OnTimer(UINT nIDEvent)
{
	if (m_uTimerID != nIDEvent) // not our timer
		CRichEditView::OnTimer(nIDEvent);
	else
	{
		KillTimer(m_uTimerID); // kill one-shot timer
		m_uTimerID = 0;
		if (m_bDelayUpdateItems)
			GetDocument()->UpdateAllItems(NULL);
		m_bDelayUpdateItems = FALSE;
	}
}

void CWordPadView::OnEditChange()
{
	SetUpdateTimer();
}

void CWordPadView::OnDestroy()
{
	POSITION pos = theApp.m_listPrinterNotify.Find(m_hWnd);
	ASSERT(pos != NULL);
	theApp.m_listPrinterNotify.RemoveAt(pos);

	CRichEditView::OnDestroy();

	if (m_uTimerID != 0) // if outstanding timer kill it
		OnTimer(m_uTimerID);
	ASSERT(m_uTimerID == 0);
}

void CWordPadView::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
	CRichEditView::CalcWindowRect(lpClientRect, nAdjustType);

	if (theApp.m_bWin4 && nAdjustType != 0 && (GetStyle() & WS_VSCROLL))
		lpClientRect->right--;

	// if the ruler is visible then slide the view up under the ruler to avoid
	// showing the top border of the view
	if (GetExStyle() & WS_EX_CLIENTEDGE)
	{
		CFrameWnd* pFrame = GetParentFrame();
		if (pFrame != NULL)
		{
			CRulerBar* pBar = (CRulerBar*)pFrame->GetControlBar(ID_VIEW_RULER);
			if (pBar != NULL)
			{
				BOOL bVis = pBar->IsVisible();
				if (pBar->m_bDeferInProgress)
					bVis = !bVis;
				if (bVis)
					lpClientRect->top -= 2;
			}
		}
	}
}

void CWordPadView::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMIS)
{
	lpMIS->itemID = (UINT)(WORD)lpMIS->itemID;
	CRichEditView::OnMeasureItem(nIDCtl, lpMIS);
}

void CWordPadView::OnPenBackspace()
{
	SendMessage(WM_KEYDOWN, VK_BACK, 0);
	SendMessage(WM_KEYUP, VK_BACK, 0);
}

void CWordPadView::OnPenNewline()
{
	SendMessage(WM_CHAR, '\n', 0);
}

void CWordPadView::OnPenPeriod()
{
	SendMessage(WM_CHAR, '.', 0);
}

void CWordPadView::OnPenSpace()
{
	SendMessage(WM_CHAR, ' ', 0);
}

void CWordPadView::OnPenTab()
{
	SendMessage(WM_CHAR, VK_TAB, 0);
}

void CWordPadView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_F10 && GetKeyState(VK_SHIFT) < 0)
	{
		long nStart, nEnd;
		GetRichEditCtrl().GetSel(nStart, nEnd);
		CPoint pt = GetRichEditCtrl().GetCharPos(nEnd);
		SendMessage(WM_CONTEXTMENU, (WPARAM)m_hWnd, MAKELPARAM(pt.x, pt.y));
	}

	CRichEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}

HRESULT CWordPadView::GetClipboardData(CHARRANGE* lpchrg, DWORD /*reco*/,
	LPDATAOBJECT lpRichDataObj, LPDATAOBJECT* lplpdataobj)
{
	CHARRANGE& cr = *lpchrg;

	if ((cr.cpMax - cr.cpMin == 1) &&
		GetRichEditCtrl().GetSelectionType() == SEL_OBJECT)
	{
		return E_NOTIMPL;
	}

	BeginWaitCursor();
	//create the data source
	COleDataSource* pDataSource = new COleDataSource;

	// put the formats into the data source
	LPENUMFORMATETC lpEnumFormatEtc;
	lpRichDataObj->EnumFormatEtc(DATADIR_SET, &lpEnumFormatEtc);
	if (lpEnumFormatEtc != NULL)
	{
		FORMATETC etc;
		while (lpEnumFormatEtc->Next(1, &etc, NULL) == S_OK)
		{
			STGMEDIUM stgMedium;
			lpRichDataObj->GetData(&etc, &stgMedium);
			pDataSource->CacheData(etc.cfFormat, &stgMedium, &etc);
		}
		lpEnumFormatEtc->Release();
	}

	CEmbeddedItem item(GetDocument(), cr.cpMin, cr.cpMax);
	item.m_lpRichDataObj = lpRichDataObj;
	// get wordpad formats
	item.GetClipboardData(pDataSource);

	// get the IDataObject from the data source
	*lplpdataobj =  (LPDATAOBJECT)pDataSource->GetInterface(&IID_IDataObject);

	EndWaitCursor();
	return S_OK;
}

HRESULT CWordPadView::QueryAcceptData(LPDATAOBJECT lpdataobj,
	CLIPFORMAT* lpcfFormat, DWORD reco, BOOL bReally,
	HGLOBAL hMetaPict)
{
	if (bReally && *lpcfFormat == 0 && (m_nPasteType == 0))
	{
		COleDataObject dataobj;
		dataobj.Attach(lpdataobj, FALSE);
		if (!dataobj.IsDataAvailable(cfRTO)) // native avail, let richedit do as it wants
		{
			if (dataobj.IsDataAvailable(cfEmbeddedObject))
			{
				if (PasteNative(lpdataobj))
					return S_FALSE;
			}
		}
	}
	return CRichEditView::QueryAcceptData(lpdataobj, lpcfFormat, reco, bReally,
		hMetaPict);
}

BOOL CWordPadView::PasteNative(LPDATAOBJECT lpdataobj)
{
	// check data object for wordpad object
	// if true, suck out RTF directly
	FORMATETC etc = {NULL, NULL, DVASPECT_CONTENT, -1, TYMED_ISTORAGE};
	etc.cfFormat = (CLIPFORMAT)cfEmbeddedObject;
	STGMEDIUM stgMedium = {TYMED_ISTORAGE, 0, NULL};

	// create an IStorage to transfer the data in
	LPLOCKBYTES lpLockBytes;
	if (FAILED(::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes)))
		return FALSE;
	ASSERT(lpLockBytes != NULL);

	HRESULT hr = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &stgMedium.pstg);
	lpLockBytes->Release(); //storage addref'd
	if (FAILED(hr))
		return FALSE;

	ASSERT(stgMedium.pstg != NULL);
	CLSID clsid;
	BOOL bRes = FALSE; //let richedit do what it wants
	if (SUCCEEDED(lpdataobj->GetDataHere(&etc, &stgMedium)) &&
		SUCCEEDED(ReadClassStg(stgMedium.pstg, &clsid)) &&
		clsid == GetDocument()->GetClassID())
	{
		//suck out RTF now
		// open Contents stream
		COleStreamFile file;
		CFileException fe;
		if (file.OpenStream(stgMedium.pstg, szContents,
			CFile::modeReadWrite|CFile::shareExclusive, &fe))
		{

			// load it with CArchive (loads from Contents stream)
			CArchive loadArchive(&file, CArchive::load |
				CArchive::bNoFlushOnDelete);
			Stream(loadArchive, TRUE); //stream in selection
			hr = TRUE; // don't let richedit do anything
		}
	}
	::ReleaseStgMedium(&stgMedium);
	return bRes;
}

// things to fix
// if format==0 we are doing a straight EM_PASTE
//  look for native formats
//      richedit specific -- allow richedit to handle (these will be first)
//      look for RTF, CF_TEXT.  If there paste special as these
//  Do standard OLE scenario

// if pasting a particular format (format != 0)
//  if richedit specific, allow through
//  if RTF, CF_TEXT. paste special
//  if OLE format, do standard OLE scenario


void CWordPadView::OnFilePrint()
{
	// don't allow winini changes to occur while printing
	m_bInPrint = TRUE;
	CRichEditView::OnFilePrint();
	// printer may have changed
	theApp.NotifyPrinterChanged(); // this will cause a GetDocument()->PrinterChanged();
	m_bInPrint = FALSE;
}

int CWordPadView::OnMouseActivate(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_bOnBar)
	{
		SetFocus();
		return MA_ACTIVATEANDEAT;
	}
	else
		return CRichEditView::OnMouseActivate(pWnd, nHitTest, message);
}

typedef BOOL (WINAPI *PCWPROC)(HWND, LPSTR, UINT, LPVOID, DWORD, DWORD);
void CWordPadView::OnPenLens()
{
	USES_CONVERSION;
	HINSTANCE hLib = LoadLibrary(_T("PENWIN32.DLL"));
	if (hLib == NULL)
		return;
	PCWPROC pCorrectWriting = (PCWPROC)GetProcAddress(hLib, "CorrectWriting");
	ASSERT(pCorrectWriting != NULL);
	if (pCorrectWriting != NULL)
	{
		CHARRANGE cr;
		GetRichEditCtrl().GetSel(cr);
		int nCnt = 2*(cr.cpMax-cr.cpMin);
		BOOL bSel = (nCnt != 0);
		nCnt = max(1024, nCnt);
		char* pBuf = new char[nCnt];
		pBuf[0] = NULL;
		if (bSel)
			GetRichEditCtrl().GetSelText(pBuf);
		if (pCorrectWriting(m_hWnd, pBuf, nCnt, 0, bSel ? 0 : CWR_INSERT, 0))
			GetRichEditCtrl().ReplaceSel(A2T(pBuf));
		delete [] pBuf;
	}
	FreeLibrary(hLib);
}

LONG CWordPadView::OnPrinterChangedMsg(UINT, LONG)
{
	CDC dc;
	AfxGetApp()->CreatePrinterDC(dc);
	OnPrinterChanged(dc);
	return 0;
}

static void ForwardPaletteChanged(HWND hWndParent, HWND hWndFocus)
{
	// this is a quick and dirty hack to send the WM_QUERYNEWPALETTE to a window that is interested
	HWND hWnd = NULL;
	for (hWnd = ::GetWindow(hWndParent, GW_CHILD); hWnd != NULL; hWnd = ::GetWindow(hWnd, GW_HWNDNEXT))
	{
		if (hWnd != hWndFocus)
		{
			::SendMessage(hWnd, WM_PALETTECHANGED, (WPARAM)hWndFocus, 0L);
			ForwardPaletteChanged(hWnd, hWndFocus);
		}
	}
}

void CWordPadView::OnPaletteChanged(CWnd* pFocusWnd)
{
	ForwardPaletteChanged(m_hWnd, pFocusWnd->GetSafeHwnd());
	// allow the richedit control to realize its palette
	// remove this if if richedit fixes their code so that
	// they don't realize their palette into foreground
	if (::GetWindow(m_hWnd, GW_CHILD) == NULL)
		CRichEditView::OnPaletteChanged(pFocusWnd);
}

static BOOL FindQueryPalette(HWND hWndParent)
{
	// this is a quick and dirty hack to send the WM_QUERYNEWPALETTE to a window that is interested
	HWND hWnd = NULL;
	for (hWnd = ::GetWindow(hWndParent, GW_CHILD); hWnd != NULL; hWnd = ::GetWindow(hWnd, GW_HWNDNEXT))
	{
		if (::SendMessage(hWnd, WM_QUERYNEWPALETTE, 0, 0L))
			return TRUE;
		else if (FindQueryPalette(hWnd))
			return TRUE;
	}
	return FALSE;
}

BOOL CWordPadView::OnQueryNewPalette()
{
	if(FindQueryPalette(m_hWnd))
		return TRUE;
	return CRichEditView::OnQueryNewPalette();
}

void CWordPadView::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CRichEditView::OnSettingChange(uFlags, lpszSection);
	//printer might have changed
	if (!m_bInPrint)
	{
		if (lstrcmpi(lpszSection, _T("windows")) == 0)
			theApp.NotifyPrinterChanged(TRUE); // force update to defaults
	}
}

void CWordPadView::OnSize(UINT nType, int cx, int cy)
{
	CRichEditView::OnSize(nType, cx, cy);
	CRect rect(HORZ_TEXTOFFSET, VERT_TEXTOFFSET, cx, cy);
	GetRichEditCtrl().SetRect(rect);
}

void CWordPadView::OnGetCharFormat(NMHDR* pNMHDR, LRESULT* pRes)
{
	ASSERT(pNMHDR != NULL);
	ASSERT(pRes != NULL);
	((NMXTPCHARHDR*)pNMHDR)->cf = GetCharFormatSelection();
	*pRes = 1;
}

void CWordPadView::OnSetCharFormat(NMHDR* pNMHDR, LRESULT* pRes)
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

void CWordPadView::OnUpdateComboFont(CCmdUI* pCmd)
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

void CWordPadView::OnUpdateComboSize(CCmdUI* pCmd)
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



void CWordPadView::OnUpdateSelectorText(CCmdUI* pCmd)
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

void CWordPadView::OnButtonText()
{
	CHARFORMAT& cfm = GetCharFormatSelection( );
	cfm.dwMask |= CFM_COLOR;
	cfm.dwEffects &= ~CFE_AUTOCOLOR ;
	cfm.crTextColor = m_clr;
	GetRichEditCtrl().SetSelectionCharFormat(cfm);

}
void CWordPadView::OnSelectorText(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	CXTPControlColorSelector* pControl = (CXTPControlColorSelector*)tagNMCONTROL->pControl;
	m_clr = pControl->GetColor();

	OnButtonText();

	*pResult = 1;
}


void CWordPadView::OnUpdateText(CCmdUI* pCmd)
{

	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmd->m_pOther;
	if (pToolBar)
	{
		CXTPControlPopupColor* pPopup = (CXTPControlPopupColor*)pToolBar->GetControls()->GetAt(pCmd->m_nIndex);
		pPopup->SetColor(m_clr);
	}

	pCmd->Enable(TRUE);
}



void CWordPadView::OnTextAuto()
{
	CHARFORMAT& cfm = GetCharFormatSelection( );
	cfm.dwMask |= CFM_COLOR;
	cfm.dwEffects |= CFE_AUTOCOLOR ;
	GetRichEditCtrl().SetSelectionCharFormat(cfm);
	m_clr = GetSysColor(COLOR_BTNTEXT);
}

void CWordPadView::OnUpdateTextAuto(CCmdUI* pCmd)
{
	CHARFORMAT& cfm = GetCharFormatSelection( );
	pCmd->SetCheck(cfm.dwEffects & CFE_AUTOCOLOR? TRUE: FALSE);
}



void CWordPadView::OnTextMore()
{
	CColorDialog cd(m_clr);
	if (cd.DoModal())
	{
		m_clr = cd.GetColor();
		OnButtonText();
	}

}



void CWordPadView::OnEditUndo(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (((NMXTPCONTROL*)pNMHDR)->pControl->GetType() == xtpControlSplitButtonPopup)
	{
		CRichEditView::OnEditUndo();

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

CXTPControlStatic* CWordPadView::FindInfoControl(CXTPControl* pControl)
{
	CXTPCommandBar* pCommandBar = pControl->GetParent();

	for (int i = 0; i < pCommandBar->GetControls()->GetCount(); i++)
	{
		CXTPControlStatic* pControlStatic = DYNAMIC_DOWNCAST(CXTPControlStatic, pCommandBar->GetControl(i));
		if (pControlStatic && pControlStatic->GetID() == pControl->GetID())
		{
			return pControlStatic;
		}

	}
	return NULL;
}


void CWordPadView::OnListBoxControlSelChange(NMHDR* pNMHDR, LRESULT* pRes)
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

void CWordPadView::OnListBoxControlPoup(NMHDR* pNMHDR, LRESULT* pRes)
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



void CWordPadView::OnColumnsSelChange(NMHDR* pNMHDR, LRESULT* pRes)
{
	ASSERT(pNMHDR != NULL);
	ASSERT(pRes != NULL);

	CControlColumns* pControlColumns = DYNAMIC_DOWNCAST(CControlColumns, ((NMXTPCONTROL*)pNMHDR)->pControl);
	if (pControlColumns)
	{

		CXTPControlStatic* pInfo = FindInfoControl(pControlColumns);
		if (pInfo)
		{
			int nCount = pControlColumns->m_szItemsActive.cx;

			CString str = _T("Cancel");
			if (nCount != 0)
			{
				str.Format(_T("%i Columns"), nCount);
			}

			pInfo->SetWidth(pControlColumns->m_szItem.cx * pControlColumns->m_szItemsVisible.cx);


			pInfo->SetCaption(str);
			pInfo->DelayRedrawParent();
		}

		*pRes = 1;
	}

}
void CWordPadView::OnInsertTable(NMHDR* pNMHDR, LRESULT* pResult)
{

	CControlTable* pControlTable = DYNAMIC_DOWNCAST(CControlTable, ((NMXTPCONTROL*)pNMHDR)->pControl);
	if (pControlTable)
	{
		CSize szActive = pControlTable->m_szResult;

		CString str;
		str.Format(_T("<%i x %i Table>"), szActive.cx, szActive.cy);

		GetRichEditCtrl().ReplaceSel(str);

		*pResult = 1; // Handled;
	}
}
void CWordPadView::OnInsertColumns(NMHDR* pNMHDR, LRESULT* pResult)
{

	CControlColumns* pControlColumns = DYNAMIC_DOWNCAST(CControlColumns, ((NMXTPCONTROL*)pNMHDR)->pControl);
	if (pControlColumns)
	{
		int nCount = pControlColumns->m_szResult.cx;

		CString str;
		str.Format(_T("<%i Columns>"), nCount);

		GetRichEditCtrl().ReplaceSel(str);

		*pResult = 1; // Handled;
	}
}


void CWordPadView::OnTableSelChange(NMHDR* pNMHDR, LRESULT* pRes)
{

	ASSERT(pNMHDR != NULL);
	ASSERT(pRes != NULL);

	CControlTable* pControlTable = DYNAMIC_DOWNCAST(CControlTable, ((NMXTPCONTROL*)pNMHDR)->pControl);
	if (pControlTable)
	{

		CXTPControlStatic* pInfo = FindInfoControl(pControlTable);
		if (pInfo)
		{
			CSize szActive = pControlTable->m_szItemsActive;

			CString str = _T("Cancel");
			if (szActive != CSize(0))
			{
				str.Format(_T("%i x %i Table"), szActive.cx, szActive.cy);
			}

			pInfo->SetWidth(pControlTable->m_szItem.cx * pControlTable->m_szItemsVisible.cx);

			pInfo->SetCaption(str);
			pInfo->DelayRedrawParent();
		}

		*pRes = 1;
	}
}
