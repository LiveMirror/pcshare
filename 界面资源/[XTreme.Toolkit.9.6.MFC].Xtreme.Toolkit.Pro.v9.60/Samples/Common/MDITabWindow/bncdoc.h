// BounceDoc.h : header file
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

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

/////////////////////////////////////////////////////////////////////////////
// CBounceDoc document

class CBounceDoc : public CDocument
{
protected:
	CBounceDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBounceDoc)

// Attributes
public:
	// bounce window client area and bouncing ball color/size parameters
	COLORREF m_clrBall;

	BOOL m_bFastSpeed;          // current speed

	CPoint m_ptPixel;           // pixel size
	CSize m_sizeRadius;         // radius of ball
	CSize m_sizeMove;           // move speed
	CSize m_sizeTotal;          // total size for ball bitmap
	CPoint m_ptCenter;          // current center for the ball

	//state of color buttons
	BOOL m_bBlack;
	BOOL m_bWhite;
	BOOL m_bBlue;
	BOOL m_bRed;
	BOOL m_bGreen;
	BOOL m_bCustom;

	// for replicating bouncing ball
	CBitmap m_bmBall;

// Operations
public:
	void SetCustomBallColor(COLORREF clr);
	void SetBallRadius(CSize radius);
	void ClearAllColors();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBounceDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBounceDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CBounceDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
