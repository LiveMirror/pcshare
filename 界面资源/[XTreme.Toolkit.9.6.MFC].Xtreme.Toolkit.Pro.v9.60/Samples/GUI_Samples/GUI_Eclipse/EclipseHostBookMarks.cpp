// EclipseHostBookMarks.cpp : implementation file
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
#include "gui_eclipse.h"
#include "EclipseHostBookMarks.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEclipseHostBookmarks

CEclipseHostBookmarks::CEclipseHostBookmarks()
{
}

CEclipseHostBookmarks::~CEclipseHostBookmarks()
{
}


BEGIN_MESSAGE_MAP(CEclipseHostBookmarks, CEclipseHost)
	//{{AFX_MSG_MAP(CEclipseHostBookmarks)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CEclipseHostBookmarks message handlers

int CEclipseHostBookmarks::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Define the default style for the output list controls.
	DWORD dwStyle = LVS_REPORT | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS |
		WS_CHILD | WS_VSCROLL | WS_TABSTOP|WS_VISIBLE;

	// Create the list control.
	if (!m_listCtrl.Create( dwStyle, CRect(0,0,0,0), this, 0xff ))
	{
		TRACE0( "Failed to create list control.\n" );
		return FALSE;
	}

	// initialize the list control.
	InitListControl();

	return 0;
}

static int arColWidths[] = {
	16,
	50,
	70,
	60,
	120,
};

static CString arColLabels[] = {
	_T(" "),
	_T("Description"),
	_T("Resource"),
	_T("In Folder"),
	_T("Location")
};

void CEclipseHostBookmarks::InitListControl()
{
	// Insert the columns.
	m_listCtrl.BuildColumns(_countof(arColWidths), arColWidths, arColLabels);

#ifdef _XTP_INCLUDE_CONTROLS
	// Subclass the flat header control.
	m_listCtrl.SubclassHeader();

	// lock the first two colums from sizing operations.
	CXTFlatHeaderCtrl* pHeaderCtrl = m_listCtrl.GetFlatHeaderCtrl( );
	if ( pHeaderCtrl != NULL )
	{
		pHeaderCtrl->FreezeColumn(0);

		// enable autosizing for columns.
		pHeaderCtrl->EnableAutoSize();
	}
#endif

	ListView_SetExtendedListViewStyle(
		m_listCtrl.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

}
