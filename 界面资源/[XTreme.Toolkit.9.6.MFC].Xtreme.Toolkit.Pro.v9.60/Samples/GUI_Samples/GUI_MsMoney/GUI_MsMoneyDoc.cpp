// GUI_MsMoneyDoc.cpp : implementation of the CGUI_MsMoneyDoc class
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
#include "GUI_MsMoney.h"

#include "GUI_MsMoneyDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUI_MsMoneyDoc

IMPLEMENT_DYNCREATE(CGUI_MsMoneyDoc, CDocument)

BEGIN_MESSAGE_MAP(CGUI_MsMoneyDoc, CDocument)
	//{{AFX_MSG_MAP(CGUI_MsMoneyDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUI_MsMoneyDoc construction/destruction

CGUI_MsMoneyDoc::CGUI_MsMoneyDoc()
{
	// TODO: add one-time construction code here

}

CGUI_MsMoneyDoc::~CGUI_MsMoneyDoc()
{
}

BOOL CGUI_MsMoneyDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGUI_MsMoneyDoc serialization

void CGUI_MsMoneyDoc::Serialize(CArchive& ar)
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
// CGUI_MsMoneyDoc diagnostics

#ifdef _DEBUG
void CGUI_MsMoneyDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGUI_MsMoneyDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGUI_MsMoneyDoc commands
