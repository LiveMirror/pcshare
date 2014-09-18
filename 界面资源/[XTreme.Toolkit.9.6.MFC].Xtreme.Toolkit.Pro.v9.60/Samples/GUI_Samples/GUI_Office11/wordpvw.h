// wordpvw.h : interface of the CWordPadView class
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

class CWordPadView : public CRichEditView
{
protected: // create from serialization only
	CWordPadView();
	DECLARE_DYNCREATE(CWordPadView)

// Attributes
public:
	UINT m_uTimerID;
	BOOL m_bDelayUpdateItems;
	BOOL m_bInPrint;
	CParaFormat m_defParaFormat;
	CCharFormat m_defCharFormat;
	CCharFormat m_defTextCharFormat;

	CWordPadDoc* GetDocument();
	BOOL IsFormatText();

	virtual HMENU GetContextMenu(WORD seltype, LPOLEOBJECT lpoleobj,
		CHARRANGE* lpchrg);

// Operations
public:
	BOOL PasteNative(LPDATAOBJECT lpdataobj);
	void SetDefaultFont(BOOL bText);
	void SetUpdateTimer();
	void GetDefaultFont(CCharFormat& cf, UINT nFontNameID);
	void DrawMargins(CDC* pDC);
	BOOL SelectPalette();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWordPadView)
	protected:
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL
	BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual HRESULT GetClipboardData(CHARRANGE* lpchrg, DWORD reco,
		LPDATAOBJECT lpRichDataObj, LPDATAOBJECT* lplpdataobj);
	virtual HRESULT QueryAcceptData(LPDATAOBJECT, CLIPFORMAT*, DWORD,
		BOOL, HGLOBAL);
public:
	virtual void WrapChanged();

// Implementation
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL m_bOnBar;
	COLORREF m_clr;

	// OLE Container support

	virtual void DeleteContents();
	virtual void OnTextNotFound(LPCTSTR);

// Generated message map functions
protected:
	afx_msg void OnCancelEditSrvr();
	//{{AFX_MSG(CWordPadView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPageSetup();
	afx_msg void OnInsertDateTime();
	afx_msg void OnFormatParagraph();
	afx_msg void OnFormatTabs();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnPenBackspace();
	afx_msg void OnPenNewline();
	afx_msg void OnPenPeriod();
	afx_msg void OnPenSpace();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnFilePrint();
	afx_msg void OnPenLens();
	afx_msg void OnPenTab();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void OnEditChange();
	afx_msg void OnColorPick(UINT nID);
	afx_msg int OnMouseActivate(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LONG OnPrinterChangedMsg(UINT, LONG);
	afx_msg void OnGetCharFormat(NMHDR* pNMHDR, LRESULT* pRes);
	afx_msg void OnSetCharFormat(NMHDR* pNMHDR, LRESULT* pRes);
	afx_msg void OnUpdateComboFont(CCmdUI* pCmd);
	afx_msg void OnUpdateComboSize(CCmdUI* pCmd);


	afx_msg void OnUpdateSelectorText(CCmdUI* pCmd);
	afx_msg void OnSelectorText(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateText(CCmdUI* pCmd);
	afx_msg void OnButtonText();

	afx_msg void OnTextAuto();
	afx_msg void OnUpdateTextAuto(CCmdUI* pCmd);

	afx_msg void OnTextMore();


	CXTPControlStatic* FindInfoControl(CXTPControl* pControl);

	afx_msg void OnEditUndo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListBoxControlSelChange(NMHDR* pNMHDR, LRESULT* pRes);
	afx_msg void OnListBoxControlPoup(NMHDR* pNMHDR, LRESULT* pRes);

		afx_msg void OnColumnsSelChange(NMHDR* pNMHDR, LRESULT* pRes);
	afx_msg void OnTableSelChange(NMHDR* pNMHDR, LRESULT* pRes);

	afx_msg void OnInsertTable(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnInsertColumns(NMHDR* pNMHDR, LRESULT* pResult);


	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in wordpvw.cpp
inline CWordPadDoc* CWordPadView::GetDocument()
	{ return (CWordPadDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
