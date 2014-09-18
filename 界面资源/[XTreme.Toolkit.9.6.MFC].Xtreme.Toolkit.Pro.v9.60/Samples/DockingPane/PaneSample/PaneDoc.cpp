// PaneDoc.cpp : implementation of the CPaneDoc class
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
#include "Pane.h"

#include "PaneDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPaneDoc

IMPLEMENT_DYNCREATE(CPaneDoc, CDocument)

BEGIN_MESSAGE_MAP(CPaneDoc, CDocument)
END_MESSAGE_MAP()


// CPaneDoc construction/destruction

CPaneDoc::CPaneDoc()
{
	// TODO: add one-time construction code here

}

CPaneDoc::~CPaneDoc()
{
}

BOOL CPaneDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CPaneDoc serialization

void CPaneDoc::Serialize(CArchive& ar)
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


// CPaneDoc diagnostics

#ifdef _DEBUG
void CPaneDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPaneDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CPaneDoc commands
