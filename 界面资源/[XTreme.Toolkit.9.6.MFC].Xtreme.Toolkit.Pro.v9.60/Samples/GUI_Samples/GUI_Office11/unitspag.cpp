// unitspag.cpp : implementation file
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
#include "wordpad.h"
#include "unitspag.h"
#include "helpids.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUnitsPage dialog

const DWORD CUnitsPage::m_nHelpIDs[] =
{
	IDC_BOX, IDH_COMM_GROUPBOX,
	IDC_UNITS_IN, IDH_WORDPAD_INCHES,
	IDC_UNITS_CM, IDH_WORDPAD_CENTIMETERS,
	IDC_UNITS_PT, IDH_WORDPAD_POINTS,
	IDC_UNITS_PI, IDH_WORDPAD_PICAS,
	IDC_WORDSEL, IDH_WORDPAD_OPTIONS_AUTOWORDSEL,
	0, 0
};

CUnitsPage::CUnitsPage() : CCSPropertyPage(CUnitsPage::IDD)
{
	//{{AFX_DATA_INIT(CUnitsPage)
	m_nUnits = -1;
	m_bWordSel = FALSE;
	//}}AFX_DATA_INIT
}


void CUnitsPage::DoDataExchange(CDataExchange* pDX)
{
	CCSPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUnitsPage)
	DDX_Radio(pDX, IDC_UNITS_IN, m_nUnits);
	DDX_Check(pDX, IDC_WORDSEL, m_bWordSel);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUnitsPage, CCSPropertyPage)
	//{{AFX_MSG_MAP(CUnitsPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CUnitsPage message handlers
