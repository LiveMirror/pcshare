// CntrItem.h : interface of the CGUI_GnomeCntrItem class
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

#if !defined(AFX_CNTRITEM_H__0CA9409F_ACB3_49BC_8866_CCB3C5382CA1__INCLUDED_)
#define AFX_CNTRITEM_H__0CA9409F_ACB3_49BC_8866_CCB3C5382CA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGUI_GnomeDoc;
class CGUI_GnomeView;

class CGUI_GnomeCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CGUI_GnomeCntrItem)

// Constructors
public:
	CGUI_GnomeCntrItem(REOBJECT* preo = NULL, CGUI_GnomeDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CGUI_GnomeDoc* GetDocument()
		{ return (CGUI_GnomeDoc*)CRichEditCntrItem::GetDocument(); }
	CGUI_GnomeView* GetActiveView()
		{ return (CGUI_GnomeView*)CRichEditCntrItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUI_GnomeCntrItem)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	~CGUI_GnomeCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNTRITEM_H__0CA9409F_ACB3_49BC_8866_CCB3C5382CA1__INCLUDED_)
