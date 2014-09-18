// ScribDoc.h : interface of the CScribbleDoc class
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

// Forward declaration of data structure class
class CScribbleItem;

/////////////////////////////////////////////////////////////////////////////
// class CStroke
//
// A stroke is a series of connected points in the scribble drawing.
// A scribble document may have multiple strokes.

class CStroke : public CObject
{
public:
	CStroke(UINT nPenWidth);

protected:
	CStroke();
	DECLARE_SERIAL(CStroke)

// Attributes
protected:
	UINT                   m_nPenWidth;    // one pen width applies to entire stroke
public:
	CArray<CPoint,CPoint>  m_pointArray;   // series of connected points
	CRect               m_rectBounding; // smallest rect that surrounds all
	                                    // of the points in the stroke
	                                    // measured in MM_LOENGLISH units
	                                    // (0.01 inches, with Y-axis inverted)
public:
	CRect& GetBoundingRect() { return m_rectBounding; }

// Operations
public:
	BOOL DrawStroke(CDC* pDC);
	void FinishStroke();

public:
	virtual void Serialize(CArchive& ar);
};



class CScribbleDoc : public COleServerDoc
{
protected: // create from serialization only
	CScribbleDoc();
	DECLARE_DYNCREATE(CScribbleDoc)

// Attributes
protected:
	// The document keeps track of the current pen width on
	// behalf of all views. We'd like the user interface of
	// Scribble to be such that if the user chooses the Draw
	// Thick Line command, it will apply to all views, not just
	// the view that currently has the focus.

	UINT            m_nPenWidth;        // current user-selected pen width
	BOOL            m_bThickPen;        // TRUE if current pen is thick
	UINT            m_nThinWidth;
	UINT            m_nThickWidth;
	CPen            m_penCur;           // pen created according to
	                                    // user-selected pen style (width)
public:
	CTypedPtrList<CObList,CStroke*>     m_strokeList;
	CPen*           GetCurrentPen() { return &m_penCur; }

protected:
	CSize           m_sizeDoc;
public:
	CSize GetDocSize() { return m_sizeDoc; }
	CScribbleItem* GetEmbeddedItem()
		{ return (CScribbleItem*)COleServerDoc::GetEmbeddedItem(); }

// Operations
public:
	CStroke* NewStroke();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScribbleDoc)
	protected:
	virtual COleServerItem* OnGetEmbeddedItem();
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ReplacePen();
	void OnSetItemRects(LPCRECT lpPosRect, LPCRECT lpClipRect);

public:
	virtual ~CScribbleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void            InitDocument();

// Generated message map functions
protected:
	//{{AFX_MSG(CScribbleDoc)
	afx_msg void OnEditClearAll();
	afx_msg void OnPenThickOrThin();
	afx_msg void OnUpdateEditClearAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePenThickOrThin(CCmdUI* pCmdUI);
	afx_msg void OnPenWidths();
	afx_msg void OnEditCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
