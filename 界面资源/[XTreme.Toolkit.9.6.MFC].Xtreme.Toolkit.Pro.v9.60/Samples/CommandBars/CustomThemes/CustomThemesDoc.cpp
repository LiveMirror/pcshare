// CustomThemesDoc.cpp : implementation of the CCustomThemesDoc class
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_SERIAL(CCustomThemesCntrItem, CRichEditCntrItem, 0)

CCustomThemesCntrItem::CCustomThemesCntrItem(REOBJECT *preo, CCustomThemesDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
}

/////////////////////////////////////////////////////////////////////////////
// CWordPadCntrItem diagnostics

#ifdef _DEBUG
void CCustomThemesCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CCustomThemesCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

CRichEditCntrItem* CCustomThemesDoc::CreateClientItem(REOBJECT* preo) const
{
	// cast away constness of this
	return new CCustomThemesCntrItem(preo, (CCustomThemesDoc*)this);
}
/////////////////////////////////////////////////////////////////////////////
// CCustomThemesDoc

IMPLEMENT_DYNCREATE(CCustomThemesDoc, CRichEditDoc)

BEGIN_MESSAGE_MAP(CCustomThemesDoc, CRichEditDoc)
	//{{AFX_MSG_MAP(CCustomThemesDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomThemesDoc construction/destruction

CCustomThemesDoc::CCustomThemesDoc()
{
	// TODO: add one-time construction code here

}

CCustomThemesDoc::~CCustomThemesDoc()
{
}

BOOL CCustomThemesDoc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCustomThemesDoc serialization

void CCustomThemesDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCustomThemesDoc diagnostics

#ifdef _DEBUG
void CCustomThemesDoc::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

void CCustomThemesDoc::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCustomThemesDoc commands
