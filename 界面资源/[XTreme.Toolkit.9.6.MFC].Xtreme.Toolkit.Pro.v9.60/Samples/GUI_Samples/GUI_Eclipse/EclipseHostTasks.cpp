// EclipseHostTasks.cpp : implementation file
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
#include "EclipseHostTasks.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEclipseHostTasks

CEclipseHostTasks::CEclipseHostTasks()
{
}

CEclipseHostTasks::~CEclipseHostTasks()
{
}


BEGIN_MESSAGE_MAP(CEclipseHostTasks, CEclipseHost)
	//{{AFX_MSG_MAP(CEclipseHostTasks)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_TASKS_NEW, OnEmptyCommand)
	ON_COMMAND(ID_TASKS_DELETE, OnEmptyCommand)
	ON_UPDATE_COMMAND_UI(ID_TASKS_DELETE, OnUpdateTasksDelete)
	ON_COMMAND(ID_TASKS_FILTERS, OnEmptyCommand)
END_MESSAGE_MAP()

static int arColWidths[] = {
	16,
	16,
	24,
	120,
	120,
	120
};

static CString arColLabels[] = {
	_T("!"),
	_T(" "),
	_T(" "),
	_T("Description"),
	_T("File"),
	_T("Line")
};

/////////////////////////////////////////////////////////////////////////////
// CEclipseHostTasks message handlers

int CEclipseHostTasks::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEclipseHost::OnCreate(lpCreateStruct) == -1)
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


void CEclipseHostTasks::InitListControl()
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
		pHeaderCtrl->FreezeColumn(1);
		pHeaderCtrl->FreezeColumn(2);

		// enable autosizing for columns.
		pHeaderCtrl->EnableAutoSize();

		pHeaderCtrl->SetBitmap(2, IDB_HEADER, HDF_STRING);
	}
#endif

	ListView_SetExtendedListViewStyle(
		m_listCtrl.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	m_listCtrl.InsertItem(0, NULL, 0);
	m_listCtrl.SetItemText(0, 3, _T("Click here to add a new task"));
}

void CEclipseHostTasks::OnEmptyCommand()
{

}

void CEclipseHostTasks::OnUpdateTasksDelete(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_listCtrl.GetSelectedCount() > 0);

}
