// CntrItem.cpp : implementation of the CGUI_GnomeCntrItem class
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
#include "GUI_Gnome.h"

#include "GUI_GnomeDoc.h"
#include "GUI_GnomeView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUI_GnomeCntrItem implementation

IMPLEMENT_SERIAL(CGUI_GnomeCntrItem, CRichEditCntrItem, 0)

CGUI_GnomeCntrItem::CGUI_GnomeCntrItem(REOBJECT* preo, CGUI_GnomeDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: add one-time construction code here

}

CGUI_GnomeCntrItem::~CGUI_GnomeCntrItem()
{
	// TODO: add cleanup code here

}

/////////////////////////////////////////////////////////////////////////////
// CGUI_GnomeCntrItem diagnostics

#ifdef _DEBUG
void CGUI_GnomeCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CGUI_GnomeCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
