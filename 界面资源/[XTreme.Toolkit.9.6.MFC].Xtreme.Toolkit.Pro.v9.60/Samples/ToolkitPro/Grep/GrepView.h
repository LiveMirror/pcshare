// GrepView.h : interface of the CGrepView class
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

#if !defined(AFX_GREPVIEW_H__841CCA0D_0740_40E5_810B_7A57EBDD46B7__INCLUDED_)
#define AFX_GREPVIEW_H__841CCA0D_0740_40E5_810B_7A57EBDD46B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGrepRecordPreview : public CXTPReportRecordItemPreview
{
public:
	CGrepRecordPreview(CString strFindLine, CString strReplaceLine)
		:CXTPReportRecordItemPreview(0)
	{
		m_strFindLine = strFindLine;
		m_strReplaceLine = strReplaceLine;

		m_bChanged = strFindLine != strReplaceLine;

		REPLACE_S(m_strFindLine, _T("\t"), _T(" "));
		REPLACE_S(m_strReplaceLine, _T("\t"), _T(" "));

	}
	CString m_strFindLine;
	CString m_strReplaceLine;
	BOOL m_bChanged;

protected:
	int GetPreviewHeight(CDC* pDC, CXTPReportRow* pRow, int nWidth)
	{
		CXTPReportControl* pControl = pRow->GetControl();

		int nIndentWidth = pControl->GetHeaderIndent();
		CRect& rcIndent = pControl->GetPaintManager()->m_rcPreviewIndent;
		CRect rcPreviewItem(nIndentWidth + rcIndent.left, 0, nWidth - rcIndent.right, 0);


		CXTPFontDC font(pDC, &pControl->GetPaintManager()->m_fontPreview);

		CRect rcFind(rcPreviewItem.left, 0, rcPreviewItem.right, 0);
		pDC->DrawText(m_strFindLine, rcFind, DT_WORDBREAK|DT_CALCRECT|DT_NOPREFIX);

		CRect rcReplace(rcPreviewItem.left, 0, rcPreviewItem.right, 0);
		pDC->DrawText(m_strReplaceLine, rcReplace, DT_WORDBREAK|DT_CALCRECT|DT_NOPREFIX);


		return rcFind.Height() + rcReplace.Height() + rcIndent.top + rcIndent.bottom;
	}
	void OnDrawCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* /*pMetrics*/)
	{

		CRect rcItem(pDrawArgs->rcItem);
		CDC* pDC = pDrawArgs->pDC;

		CRect& rcIndent = pDrawArgs->pControl->GetPaintManager()->m_rcPreviewIndent;
		rcItem.DeflateRect(rcIndent.left, rcIndent.top, rcIndent.right, rcIndent.bottom);

		CRect rcFind(rcItem);
		pDC->DrawText(m_strFindLine, rcFind, DT_WORDBREAK|DT_LEFT|DT_NOPREFIX);
		rcFind.bottom = rcFind.top;

		pDC->DrawText(m_strFindLine, rcFind, DT_WORDBREAK|DT_LEFT|DT_NOPREFIX|DT_CALCRECT);

		CRect rcReplace(rcItem);
		rcReplace.top += rcFind.Height();
		if (m_bChanged) if (pDC->GetTextColor() == 0xFF0000) pDC->SetTextColor(0xFF);
		pDC->DrawText(m_strReplaceLine, rcReplace, DT_WORDBREAK|DT_LEFT|DT_NOPREFIX);
	}
};

class CGrepRecord : public CXTPReportRecord
{
public:
	CGrepRecord(CString strPath, CString strDirectory, CString strName,
		CString strFind, long nLine, CString strFindLine, CString strReplaceLine, long nIndex, long nLength, CString strReplace )
	{
		AddItem(new CXTPReportRecordItemText(strName));
		AddItem(new CXTPReportRecordItemText(strDirectory));
		AddItem(new CXTPReportRecordItemText(strFind));
		AddItem(new CXTPReportRecordItemNumber(nLine));
		CXTPReportRecordItem* pItem = AddItem(new CXTPReportRecordItem());
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(strFindLine != strReplaceLine);
		pItem->SetSortPriority(strFindLine != strReplaceLine? 1: 0);
		pItem->SetGroupPriority(strFindLine != strReplaceLine? 1: 0);

		CString strExt;
		int nExt = strName.ReverseFind(_T('.'));
		if (nExt > 0) strExt = strName.Mid(nExt + 1);

		AddItem(new CXTPReportRecordItemText(strExt));

		SetPreviewItem(new CGrepRecordPreview(strFindLine, strReplaceLine));

		m_strPath = strPath;
		m_nIndex = nIndex;
		m_nLength = nLength;
		m_strReplace = strReplace;
	}
	BOOL IsChecked()
	{
		return GetItem(4)->IsChecked();
	}
	void SetChecked(BOOL bChecked)
	{
		GetItem(4)->SetChecked(bChecked);
	}
	BOOL IsChanged()
	{
		return ((CGrepRecordPreview*)GetItemPreview())->m_bChanged;
	}

public:
	CString m_strPath;
	int m_nIndex;
	int m_nLength;
	CString m_strReplace;
};

class CGrepDoc;

class CGrepView : public CXTPReportView
{
protected: // create from serialization only
	CGrepView();
	DECLARE_DYNCREATE(CGrepView)

// Attributes
public:
	CGrepDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrepView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGrepView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void OnReportItemDblClick(NMHDR * pNotifyStruct, LRESULT * /*result*/);
	void OnReportKeyDown(NMHDR * pNotifyStruct, LRESULT * /*result*/);

// Generated message map functions
protected:
	//{{AFX_MSG(CGrepView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GrepView.cpp
inline CGrepDoc* CGrepView::GetDocument()
	{ return (CGrepDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GREPVIEW_H__841CCA0D_0740_40E5_810B_7A57EBDD46B7__INCLUDED_)
