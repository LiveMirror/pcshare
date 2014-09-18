// ShellListCtrl.cpp : implementation file
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
#include "ShellListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShellListCtrl

CShellListCtrl::CShellListCtrl()
{
}

CShellListCtrl::~CShellListCtrl()
{
}


BEGIN_MESSAGE_MAP(CShellListCtrl, CXTShellListCtrl)
	//{{AFX_MSG_MAP(CShellListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShellListCtrl message handlers

void CShellListCtrl::RefreshList()
{
	if ( !m_pTreeCtrl || !::IsWindow( m_pTreeCtrl->m_hWnd ) )
		return;

	CTreeCtrl* pTreeCtrl = ( CTreeCtrl* )m_pTreeCtrl;
	if ( !pTreeCtrl->IsKindOf( RUNTIME_CLASS( CTreeCtrl ) ) )
		return;

	HTREEITEM hItem = pTreeCtrl->GetSelectedItem( );
	if ( !hItem )
		return;

	CObject* pObject = ( CObject* )pTreeCtrl->GetItemData( hItem );
	if ( !pObject )
		return;

	SendMessage( XTWM_SHELL_NOTIFY,
		( WPARAM )SHN_XT_TREESELCHANGE, ( LPARAM )pObject );
}

void CShellListCtrl::OnShowContextMenu(int idCmd, CMINVOKECOMMANDINFO& cmi)
{
	UNREFERENCED_PARAMETER(idCmd);
	UNREFERENCED_PARAMETER(cmi);

	// Refresh list if items were deleted.
	if ( idCmd == 17 )
	{
		RefreshList( );
	}
}
