// CustomizePageTools.cpp : implementation file
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
#include "CustomizeDlg.h"
#include "CustomizePageTools.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CToolsList CCustomizePageTools::m_lstTools;

void CToolRec::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_strTitle << m_strArg << m_strCmd << m_strDir;
	}
	else
	{
		ar >> m_strTitle >> m_strArg >> m_strCmd >> m_strDir;

	}
}

template <> void AFXAPI SerializeElements <CToolRec> ( CArchive& ar, CToolRec* pNewTool, int nCount )
{
	for ( int i = 0; i < nCount; i++, pNewTool++ )
	{
		// Serialize each CToolRec object
		pNewTool->Serialize( ar );
	}
}


/////////////////////////////////////////////////////////////////////////////
// CCustomizePageTools property page

IMPLEMENT_DYNCREATE(CCustomizePageTools, CPropertyPage)

CCustomizePageTools::CCustomizePageTools() : CPropertyPage(CCustomizePageTools::IDD)
{
	//{{AFX_DATA_INIT(CCustomizePageTools)
	m_strToolsDir = _T( "" );
	m_strToolsArg = _T( "" );
	m_strToolsCmd = _T( "" );
	//}}AFX_DATA_INIT
}

CCustomizePageTools::~CCustomizePageTools()
{
}

void CCustomizePageTools::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomizePageTools)
	DDX_Control(pDX, IDC_TXT_TOOLSDIR, m_txtToolsDir);
	DDX_Control(pDX, IDC_TXT_TOOLSARG, m_txtToolsArg);
	DDX_Control(pDX, IDC_TXT_TOOLSCMD, m_txtToolsCmd);
	DDX_Control(pDX, IDC_EDIT_TOOLSDIR, m_editToolsDir);
	DDX_Control(pDX, IDC_EDIT_TOOLSARG, m_editToolsArg);
	DDX_Control(pDX, IDC_EDIT_TOOLSCMD, m_editToolsCmd);
	DDX_Control(pDX, IDC_LBOX_TOOLSLIST, m_lboxToolsList);
	DDX_Text(pDX, IDC_EDIT_TOOLSDIR, m_strToolsDir);
	DDX_Text(pDX, IDC_EDIT_TOOLSARG, m_strToolsArg);
	DDX_Text(pDX, IDC_EDIT_TOOLSCMD, m_strToolsCmd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCustomizePageTools, CPropertyPage)
	//{{AFX_MSG_MAP(CCustomizePageTools)
	ON_LBN_SELCHANGE(IDC_LBOX_TOOLSLIST, OnSelchangeEditList)
	ON_EN_CHANGE(IDC_EDIT_TOOLSCMD, OnChangeToolsCmd)
	ON_EN_CHANGE(IDC_EDIT_TOOLSARG, OnChangeToolsArg)
	ON_EN_CHANGE(IDC_EDIT_TOOLSDIR, OnChangeToolsDir)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP

#ifdef _XTP_INCLUDE_CONTROLS
	ON_LBN_XT_LABELEDITEND(IDC_LBOX_TOOLSLIST, OnLabelEditEnd)
	ON_LBN_XT_LABELEDITCANCEL(IDC_LBOX_TOOLSLIST, OnLabelEditCancel)
	ON_LBN_XT_NEWITEM(IDC_LBOX_TOOLSLIST, OnNewItem)
	ON_LBN_XT_PREDELETEITEM(IDC_LBOX_TOOLSLIST, OnPreDeleteItem)
#endif
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomizePageTools message handlers

BOOL CCustomizePageTools::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

#ifdef _XTP_INCLUDE_CONTROLS
	m_editToolsCmd.Initialize(this, BES_XT_CHOOSEFILE);
	m_editToolsCmd.SetDlgTitle( _T("Browse") );

	m_editToolsCmd.SetDlgFilter( _T("Executable (*.exe)|*.exe|Command (*.com)|*.com|Information (*.pif)|*.pif|Batch (*.bat;*.cmd)|*.bat; *.cmd|All Files (*.*)|*.*||"));

	m_lboxToolsList.SetListEditStyle( _T(" &Menu contents:"), LBS_XT_DEFAULT);

	int iArgPopup = 0; //IDR_ARG_POPUP;
	int iDirPopup = 0;//IDR_DIR_POPUP;

	if ( iArgPopup != 0 ) {
		m_editToolsArg.Initialize(this, BES_XT_POPUPMENU, iArgPopup, 0 );
	}
	else {
		m_editToolsArg.Initialize(this, BES_XT_CHOOSEFILE );
	}

	if ( iDirPopup != 0 ) {
		m_editToolsDir.Initialize(this, BES_XT_POPUPMENU, iDirPopup, 0 );
	}
	else {
		m_editToolsDir.Initialize(this, BES_XT_CHOOSEDIR );
	}
#else
	m_lboxToolsList.ModifyStyle(LBS_OWNERDRAWFIXED|LBS_OWNERDRAWVARIABLE|LBS_NOINTEGRALHEIGHT, 0);

#endif

	POSITION pos = m_lstTools.GetHeadPosition();
	while (pos)
	{
		CToolRec* pRec = new CToolRec;
		*pRec = m_lstTools.GetNext(pos);
		int nIndex = m_lboxToolsList.AddString(pRec->m_strTitle);
		m_lboxToolsList.SetItemData(nIndex, (DWORD_PTR)pRec);
	}

	m_lboxToolsList.SetCurSel( 0 );
	OnSelchangeEditList( );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CCustomizePageTools::EnableControls(bool bEnable)
{
	m_txtToolsDir.EnableWindow( bEnable );
	m_txtToolsArg.EnableWindow( bEnable );
	m_txtToolsCmd.EnableWindow( bEnable );
	m_editToolsDir.EnableWindow( bEnable );
	m_editToolsArg.EnableWindow( bEnable );
	m_editToolsCmd.EnableWindow( bEnable );
}

void CCustomizePageTools::OnSelchangeEditList()
{
	int iIndex = m_lboxToolsList.GetCurSel( );
	if ( iIndex == LB_ERR )
	{
		m_strToolsDir = _T( "" );
		m_strToolsArg = _T( "" );
		m_strToolsCmd = _T( "" );

		EnableControls( false );
	}
	else if ( iIndex >= m_lboxToolsList.GetCount( ) )
	{
		return;
	}
	else
	{
		CToolRec* pTool = ( CToolRec* )m_lboxToolsList.GetItemData( iIndex );
		if ( pTool == NULL )
		{
			return;
		}

		m_strToolsDir = pTool->m_strDir;
		m_strToolsArg = pTool->m_strArg;
		m_strToolsCmd = pTool->m_strCmd;

		EnableControls( true );
	}

	UpdateData( FALSE );
}

void CCustomizePageTools::UpdateToolData()
{
	UpdateData();

	// get the index of the currently selected item.
	int iItem = m_lboxToolsList.GetCurSel( );
	if ( iItem == LB_ERR )
	{
		return;
	}

	// get a pointer to the item data for the selection.
	CToolRec* pTool = ( CToolRec* )m_lboxToolsList.GetItemData( iItem );
	if ( pTool != NULL )
	{
		pTool->m_strCmd = m_strToolsCmd;
		pTool->m_strArg = m_strToolsArg;
		pTool->m_strDir = m_strToolsDir;
	}
}

void CCustomizePageTools::OnChangeToolsCmd()
{
	UpdateToolData( );
}

void CCustomizePageTools::OnChangeToolsArg()
{
	UpdateToolData( );
}

void CCustomizePageTools::OnChangeToolsDir()
{
	UpdateToolData( );
}

void CCustomizePageTools::OnLabelEditEnd()
{
#ifdef _XTP_INCLUDE_CONTROLS

	int iIndex = m_lboxToolsList.GetCurrentIndex( );
	CToolRec* pTool = ( CToolRec* )m_lboxToolsList.GetItemData( iIndex );
	if ( pTool != NULL )
	{
		CString strText;
		m_lboxToolsList.GetText( iIndex, strText );

		pTool->m_strTitle = strText;
		m_lboxToolsList.SetCurSel( iIndex );
	}
#endif

}

void CCustomizePageTools::OnLabelEditCancel()
{
#ifdef _XTP_INCLUDE_CONTROLS
	m_lboxToolsList.SetCurSel(
		m_lboxToolsList.GetCurrentIndex( ) );
#endif
}


void CCustomizePageTools::OnNewItem()
{
#ifdef _XTP_INCLUDE_CONTROLS
	int iItem = m_lboxToolsList.GetCurrentIndex( );
	if ( iItem != -1 )
	{
		CString strTitle;
		m_lboxToolsList.GetText( iItem, strTitle );

		CToolRec* pTool = new CToolRec(strTitle);

		m_lboxToolsList.SetCurSel( iItem );

		m_strToolsDir = _T("");
		m_strToolsArg = _T("");
		m_strToolsCmd = _T("");

		m_lboxToolsList.SetItemData( iItem, (DWORD_PTR)pTool);

		EnableControls( true );
		UpdateData( FALSE );
	}
#endif
}

void CCustomizePageTools::OnPreDeleteItem()
{
	int iIndex = m_lboxToolsList.GetCurSel( );
	if ( iIndex == LB_ERR )
		return;

	delete ( CToolRec* )m_lboxToolsList.GetItemData( iIndex );
}


void CCustomizePageTools::UpdateToolsList()
{
	m_lstTools.RemoveAll();

	for (int i = 0; i < m_lboxToolsList.GetCount( ); ++i )
	{
		CToolRec* pOldTool = ( CToolRec* )m_lboxToolsList.GetItemData( i );
		if ( pOldTool == NULL )
			continue;

		m_lstTools.AddTail( *pOldTool );
		delete pOldTool;
	}

	m_lboxToolsList.ResetContent();
}

BOOL CCustomizePageTools::OnKillActive()
{
	return CPropertyPage::OnKillActive();
}

void CCustomizePageTools::OnDestroy()
{
	UpdateToolsList( );
	UpdateIcons();
	CPropertyPage::OnDestroy();
}

static const TCHAR _lpszTools[]     = _T("Tools");

void CCustomizePageTools::SaveTools(LPCTSTR lpszProfileName)
{
	CMemFile file;
	CArchive ar (&file, CArchive::store);
	m_lstTools.Serialize(ar);
	ar.Flush ();

	DWORD dwDataSize = (DWORD)file.GetPosition();
	LPBYTE lpbData = file.Detach();

	AfxGetApp()->WriteProfileBinary(lpszProfileName, _lpszTools, lpbData, dwDataSize);

	ar.Close();
	file.Close();
	free (lpbData);

}

void CCustomizePageTools::UpdateIcons()
{
	int nIconIndex = 8000;

	POSITION pos = m_lstTools.GetHeadPosition();
	while (pos)
	{
		CToolRec& tool = m_lstTools.GetNext(pos);
		tool.m_nIconID = 0;

		CString strPath = tool.m_strCmd;
		TCHAR lpszPath [MAX_PATH];

		if (::SearchPath (NULL, strPath, NULL, MAX_PATH,
			lpszPath, NULL) != 0)
		{
			strPath = lpszPath;
		}

		SHFILEINFO sfi;

		if (::SHGetFileInfo (strPath, 0, &sfi, sizeof(SHFILEINFO),
			SHGFI_ICON | SHGFI_SHELLICONSIZE | SHGFI_SMALLICON))
		{
			HICON hIcon =  sfi.hIcon;

			if (hIcon)
			{
				XTPImageManager()->SetIcon(hIcon, nIconIndex);
				tool.m_nIconID = nIconIndex++;
				DestroyIcon(hIcon);

			}
		}
	}
}


BOOL CCustomizePageTools::LoadTools(LPCTSTR lpszProfileName)
{
	BOOL bResult = FALSE;

	LPBYTE lpbData;
	UINT dwDataSize;
	if (AfxGetApp()->GetProfileBinary(lpszProfileName, _lpszTools, &lpbData, &dwDataSize))
	{
		try
		{
			CMemFile file(lpbData, dwDataSize);
			CArchive ar (&file, CArchive::load);
			m_lstTools.Serialize(ar);

			bResult = TRUE;
		}
		catch (CMemoryException* pEx)
		{
			pEx->Delete ();
		}
		catch (CArchiveException* pEx)
		{
			pEx->Delete ();
		}

		delete[] lpbData;

		UpdateIcons();

	}
	return bResult;
}
