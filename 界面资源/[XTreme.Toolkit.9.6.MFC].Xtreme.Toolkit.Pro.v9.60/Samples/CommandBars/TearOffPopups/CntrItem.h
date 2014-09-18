// CntrItem.h : interface of the CTearOffPopupsCntrItem class
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

#if !defined(AFX_CNTRITEM_H__6C133CB1_F8CB_4F15_8686_EC1DE64E20B2__INCLUDED_)
#define AFX_CNTRITEM_H__6C133CB1_F8CB_4F15_8686_EC1DE64E20B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTearOffPopupsDoc;
class CTearOffPopupsView;

class CTearOffPopupsCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CTearOffPopupsCntrItem)

// Constructors
public:
	CTearOffPopupsCntrItem(REOBJECT* preo = NULL, CTearOffPopupsDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CTearOffPopupsDoc* GetDocument()
		{ return (CTearOffPopupsDoc*)CRichEditCntrItem::GetDocument(); }
	CTearOffPopupsView* GetActiveView()
		{ return (CTearOffPopupsView*)CRichEditCntrItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTearOffPopupsCntrItem)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	~CTearOffPopupsCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNTRITEM_H__6C133CB1_F8CB_4F15_8686_EC1DE64E20B2__INCLUDED_)
