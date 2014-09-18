// XTPReportFilterEditControl.cpp : implementation of the CXTPReportFilterEditControl class.
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

#include "XTPReportControl.h"
#include "XTPReportColumn.h"
#include "XTPReportColumns.h"
#include "XTPReportFilterEditControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPReportFilterEditControl

CXTPReportFilterEditControl::CXTPReportFilterEditControl()
{
	m_pReportCtrl = NULL;
}

CXTPReportFilterEditControl::~CXTPReportFilterEditControl()
{
}


BEGIN_MESSAGE_MAP(CXTPReportFilterEditControl, CEdit)
	//{{AFX_MSG_MAP(CXTPReportFilterEditControl)
	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPReportFilterEditControl attributes

BOOL CXTPReportFilterEditControl::SetReportCtrl(CXTPReportControl* pReportCtrl)
{
	if(pReportCtrl == NULL)
		return FALSE;

	ASSERT_KINDOF(CXTPReportControl, pReportCtrl);
	m_pReportCtrl = pReportCtrl;

	return TRUE;
}

CXTPReportControl* CXTPReportFilterEditControl::GetReportCtrl()
{
	return m_pReportCtrl;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPReportFilterEditControl operations


/////////////////////////////////////////////////////////////////////////////
// CXTPReportFilterEditControl message handlers

// Handles the EN_CHANGE message
BOOL CXTPReportFilterEditControl::OnChange()
{
	if (!m_pReportCtrl)
		return FALSE;

	// Get text
	CString strNewFilter;
	GetWindowText( strNewFilter );

	// Apply new filter
	if (m_pReportCtrl->GetFilterText() != strNewFilter)
	{
		m_pReportCtrl->SetFilterText(strNewFilter);

		// refresh control
		m_pReportCtrl->Populate();
	}

	// Returning FALSE allows the parent window to also handle the EN_CHANGE message
	return FALSE;
}
