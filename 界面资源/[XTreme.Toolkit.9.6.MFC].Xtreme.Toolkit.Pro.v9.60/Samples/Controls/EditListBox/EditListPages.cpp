// EditListPages.cpp : implementation file
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
#include "resource.h"
#include "EditListPages.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditListPage1 property page

CEditListPage1::CEditListPage1() : CXTResizePropertyPage(CEditListPage1::IDD)
{
	//{{AFX_DATA_INIT(CEditListPage1)
	m_bEnable = TRUE;
	//}}AFX_DATA_INIT
}

CEditListPage1::~CEditListPage1()
{
	while (!m_arTools.IsEmpty())
	{
		TOOLS* pTools = m_arTools.RemoveHead();
		SAFE_DELETE(pTools);
	}
}

void CEditListPage1::DoDataExchange(CDataExchange* pDX)
{
	CXTResizePropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditListPage1)
	DDX_Control(pDX, IDC_CHK_ENABLE, m_chkEnable);
	DDX_Control(pDX, IDC_TXT_EDIT3, m_txtEdit3);
	DDX_Control(pDX, IDC_TXT_EDIT2, m_txtEdit2);
	DDX_Control(pDX, IDC_TXT_EDIT1, m_txtEdit1);
	DDX_Control(pDX, IDC_EDIT3, m_edit3);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_EDIT_LIST, m_editList);
	DDX_Check(pDX, IDC_CHK_ENABLE, m_bEnable);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNCREATE(CEditListPage1, CXTResizePropertyPage)

BEGIN_MESSAGE_MAP(CEditListPage1, CXTResizePropertyPage)
	//{{AFX_MSG_MAP(CEditListPage1)
	ON_LBN_SELCHANGE(IDC_EDIT_LIST, OnSelchangeEditList)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
	ON_COMMAND(ID_CLICKEDIT2_FILEPATH, OnClickedit2Filepath)
	ON_COMMAND(ID_CLICKEDIT2_FILEDIRECTORY, OnClickedit2Filedirectory)
	ON_COMMAND(ID_CLICKEDIT2_FILENAME, OnClickedit2Filename)
	ON_COMMAND(ID_CLICKEDIT2_FILEEXTENSION, OnClickedit2Fileextension)
	ON_COMMAND(ID_CLICKEDIT2_CURRENTLINE, OnClickedit2Currentline)
	ON_COMMAND(ID_CLICKEDIT2_CURRENTCOLUMN, OnClickedit2Currentcolumn)
	ON_COMMAND(ID_CLICKEDIT2_CURRENTTEXT, OnClickedit2Currenttext)
	ON_COMMAND(ID_CLICKEDIT2_CURRENTDIRECTORY, OnClickedit2Currentdirectory)
	ON_COMMAND(ID_CLICKEDIT2_TARGETPATH, OnClickedit2Targetpath)
	ON_COMMAND(ID_CLICKEDIT2_TARGETDIRECTORY, OnClickedit2Targetdirectory)
	ON_COMMAND(ID_CLICKEDIT2_TARGETNAME, OnClickedit2Targetname)
	ON_COMMAND(ID_CLICKEDIT2_TARGETEXTENSION, OnClickedit2Targetextension)
	ON_COMMAND(ID_CLICKEDIT2_TARGETARGUMENTS, OnClickedit2Targetarguments)
	ON_COMMAND(ID_CLICKEDIT2_WORKSPACEDIRECTORY, OnClickedit2Workspacedirectory)
	ON_COMMAND(ID_CLICKEDIT2_WORKSPACENAME, OnClickedit2Workspacename)
	ON_COMMAND(ID_CLICKEDIT3_FILEDIRECTORY, OnClickedit3Filedirectory)
	ON_COMMAND(ID_CLICKEDIT3_FILENAME, OnClickedit3Filename)
	ON_COMMAND(ID_CLICKEDIT3_CURRENTDIRECTORY, OnClickedit3Currentdirectory)
	ON_COMMAND(ID_CLICKEDIT3_TARGETDIRECTORY, OnClickedit3Targetdirectory)
	ON_COMMAND(ID_CLICKEDIT3_TARGETNAME, OnClickedit3Targetname)
	ON_COMMAND(ID_CLICKEDIT3_WORKSPACEDIRECTORY, OnClickedit3Workspacedirectory)
	ON_COMMAND(ID_CLICKEDIT3_WORKSPACENAME, OnClickedit3Workspacename)
	ON_BN_CLICKED(IDC_CHK_ENABLE, OnChkEnable)
	//}}AFX_MSG_MAP
	ON_LBN_XT_LABELEDITEND(IDC_EDIT_LIST, OnLabelEditEnd)
	ON_LBN_XT_LABELEDITCANCEL(IDC_EDIT_LIST, OnLabelEditCancel)
	ON_LBN_XT_NEWITEM(IDC_EDIT_LIST, OnNewItem)
	ON_LBN_XT_DELETEITEM(IDC_EDIT_LIST, OnDeleteItem)
	ON_LBN_XT_MOVEITEMUP(IDC_EDIT_LIST, OnMoveItemUp)
	ON_LBN_XT_MOVEITEMDOWN(IDC_EDIT_LIST, OnMoveItemDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditListPage1 message handlers

void CEditListPage1::AddListItem(LPCTSTR lpszTitle, LPCTSTR strCmd, LPCTSTR strArg, LPCTSTR strInt)
{
	TOOLS* pTools = new TOOLS;
	ASSERT(pTools);

	pTools->strCmd = strCmd;
	pTools->strArg = strArg;
	pTools->strInt = strInt;

	int iItem = m_editList.AddString(lpszTitle);
	m_editList.SetItemData(iItem, (DWORD_PTR)pTools);

	m_arTools.AddTail(pTools);
}

BOOL CEditListPage1::OnInitDialog()
{
	CXTResizePropertyPage::OnInitDialog();

	m_edit1.Initialize(this, BES_XT_CHOOSEFILE);
	m_edit1.SetDlgTitle(_T("Browse"));
	m_edit1.SetDlgFilter(_T("Executable (*.exe)|*.exe|Command (*.com)|*.com|Information (*.pif)|*.pif|Batch (*.bat;*.cmd)|*.bat; *.cmd|All Files (*.*)|*.*||"));

	// Call CXTBrowseEdit::SetListEditStyle to set the popup menu for the
	// browse edit objects.
	m_edit2.Initialize(this, BES_XT_POPUPMENU, IDR_POPUP, 0);
	m_edit3.Initialize(this, BES_XT_POPUPMENU, IDR_POPUP, 1);

	// Call CXTEditListBox::SetListEditStyle to set the type of edit list. You can
	// pass in LBS_XT_NOTOOLBAR if you don't want the toolbar displayed.
	m_editList.SetListEditStyle(_T(" &Menu contents:"),
		LBS_XT_DEFAULT);

	// initialize the edit list box.
	m_editList.Initialize();

	AddListItem(
		_T("Visual Component M&anager"),
		_T("C:\\Program Files\\Internet Explorer\\iexplore.exe"),
		_T("C:\\Program Files\\Microsoft Visual Studio\\Common\\Tools\\VCM\\vcm.vbd"),
		_T("C:\\Program Files\\Microsoft Visual Studio\\Common\\Tools\\VCM\\"));

	AddListItem(
		_T("Re&gister Control"),
		_T("C:\\WINNT\\System32\\regsvr32.exe"),
		_T("/v \"$(TargetPath)\""),
		_T("$(TargetDir)"));

	AddListItem(
		_T("Error Loo&kup"),
		_T("C:\\Program Files\\Microsoft Visual Studio\\Common\\Tools\\errlook.exe"),
		_T(""),
		_T("$ (TargetDir)"));

	AddListItem(
		_T("Activ&eX Control Test Container"),
		_T("C:\\Program Files\\Microsoft Visual Studio\\Common\\Tools\\tstcon32.exe"),
		_T(""),
		_T("$ (TargetDir)"));

	AddListItem(
		_T("OLE/COM Object &Viewer"),
		_T("C:\\Program Files\\Microsoft Visual Studio\\Common\\Tools\\oleview.exe"),
		_T(""),
		_T("C:\\Program Files\\Microsoft Visual Studio\\Common\\Tools\\"));

	AddListItem(
		_T("S&py++"),
		_T("C:\\Program Files\\Microsoft Visual Studio\\Common\\Tools\\spyxx.exe"),
		_T(""),
		_T("C:\\Program Files\\Microsoft Visual Studio\\Common\\Tools\\"));

	AddListItem(
		_T("MFC &Tracer"),
		_T("C:\\Program Files\\Microsoft Visual Studio\\Common\\Tools\\tracer.exe"),
		_T(""),
		_T("C:\\Program Files\\Microsoft Visual Studio\\Common\\Tools\\"));

	m_editList.SetCurSel(0);
	OnSelchangeEditList();

	// Set control resizing.
	SetResize(IDC_EDIT_LIST, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TXT_EDIT1, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT1, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TXT_EDIT2, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT2, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TXT_EDIT3, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT3, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHK_ENABLE, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditListPage1::OnSelchangeEditList()
{
	// TODO: Add your control notification handler code here
	int iIndex = m_editList.GetCurSel();

	if (iIndex == -1)
	{
		m_edit1.SetWindowText(_T(""));
		m_edit2.SetWindowText(_T(""));
		m_edit3.SetWindowText(_T(""));
		return;
	}

	TOOLS* pTools = (TOOLS*)m_editList.GetItemData(iIndex);
	if (pTools != NULL)
	{
		CString strCmd;
		m_edit1.GetWindowText(strCmd);
		if (strCmd.Compare(pTools->strCmd) != 0) {
			m_edit1.SetWindowText(pTools->strCmd);
		}

		CString strArg;
		m_edit2.GetWindowText(strArg);
		if (strArg.Compare(pTools->strArg) != 0) {
			m_edit2.SetWindowText(pTools->strArg);
		}

		CString strInt;
		m_edit3.GetWindowText(strInt);
		if (strInt.Compare(pTools->strInt) != 0) {
			m_edit3.SetWindowText(pTools->strInt);
		}
	}
}

void CEditListPage1::UpdateEditData(int iWhich)
{
	CString strText;

	switch (iWhich)
	{
	case TOOLCMD:
		m_edit1.GetWindowText(strText);
		break;
	case TOOLARG:
		m_edit2.GetWindowText(strText);
		break;
	case TOOLINT:
		m_edit3.GetWindowText(strText);
		break;
	}

	int iItem = m_editList.GetCurSel();

	if (iItem == -1)
		return;

	TOOLS* pTools = (TOOLS*)m_editList.GetItemData(iItem);

	switch (iWhich)
	{
	case TOOLCMD:
		if (pTools && strText.Compare(pTools->strCmd) != 0) {
			pTools->strCmd = strText;
		}
		break;

	case TOOLARG:
		if (pTools && strText.Compare(pTools->strArg) != 0) {
			pTools->strArg = strText;
		}
		break;

	case TOOLINT:
		if (pTools && strText.Compare(pTools->strInt) != 0) {
			pTools->strInt = strText;
		}
		break;
	}
}

void CEditListPage1::OnChangeEdit1()
{
	UpdateEditData(TOOLCMD);
}

void CEditListPage1::OnChangeEdit2()
{
	UpdateEditData(TOOLARG);
}

void CEditListPage1::OnChangeEdit3()
{
	UpdateEditData(TOOLINT);
}

void CEditListPage1::AppendEditItem(LPCTSTR lpszText, int iWhich)
{
	int iIndex = m_editList.GetCurSel();
	TOOLS* pTools = (TOOLS*)m_editList.GetItemData(iIndex);
	if (pTools != NULL)
	{
		switch (iWhich)
		{
		case TOOLCMD:
			{
				pTools->strCmd += lpszText;
				m_edit1.SetWindowText(pTools->strCmd);
				int iSel = pTools->strCmd.GetLength();
				m_edit1.SetSel(iSel,iSel);
			}
			break;
		case TOOLARG:
			{
				pTools->strArg += lpszText;
				m_edit2.SetWindowText(pTools->strArg);
				int iSel = pTools->strArg.GetLength();
				m_edit2.SetSel(iSel,iSel);
			}
			break;
		case TOOLINT:
			{
				pTools->strInt += lpszText;
				m_edit3.SetWindowText(pTools->strInt);
				int iSel = pTools->strInt.GetLength();
				m_edit3.SetSel(iSel,iSel);
			}
			break;
		}
	}
}

void CEditListPage1::OnClickedit2Filepath()
{
	AppendEditItem(_T("$(FilePath)"), TOOLARG);
}

void CEditListPage1::OnClickedit2Filedirectory()
{
	AppendEditItem(_T("$(FileDir)"), TOOLARG);
}

void CEditListPage1::OnClickedit2Filename()
{
	AppendEditItem(_T("$(FileName)"), TOOLARG);
}

void CEditListPage1::OnClickedit2Fileextension()
{
	AppendEditItem(_T("$(FileExt)"), TOOLARG);
}

void CEditListPage1::OnClickedit2Currentline()
{
	AppendEditItem(_T("$(CurLine)"), TOOLARG);
}

void CEditListPage1::OnClickedit2Currentcolumn()
{
	AppendEditItem(_T("$(CurCol)"), TOOLARG);
}

void CEditListPage1::OnClickedit2Currenttext()
{
	AppendEditItem(_T("$(CurText)"), TOOLARG);
}

void CEditListPage1::OnClickedit2Currentdirectory()
{
	AppendEditItem(_T("$(CurDir)"), TOOLARG);
}

void CEditListPage1::OnClickedit2Targetpath()
{
	AppendEditItem(_T("$(TargetPath)"), TOOLARG);
}

void CEditListPage1::OnClickedit2Targetdirectory()
{
	AppendEditItem(_T("$(TargetDir)"), TOOLARG);
}

void CEditListPage1::OnClickedit2Targetname()
{
	AppendEditItem(_T("$(TargetName)"), TOOLARG);
}

void CEditListPage1::OnClickedit2Targetextension()
{
	AppendEditItem(_T("$(TargetExt)"), TOOLARG);
}

void CEditListPage1::OnClickedit2Targetarguments()
{
	AppendEditItem(_T("$(TargetArgs)"), TOOLARG);
}

void CEditListPage1::OnClickedit2Workspacedirectory()
{
	AppendEditItem(_T("$(WkspDir)"), TOOLARG);
}

void CEditListPage1::OnClickedit2Workspacename()
{
	AppendEditItem(_T("$(WkspName)"), TOOLARG);
}

void CEditListPage1::OnClickedit3Filedirectory()
{
	AppendEditItem(_T("$(FileDir)"), TOOLINT);
}

void CEditListPage1::OnClickedit3Filename()
{
	AppendEditItem(_T("$(FileName)"), TOOLINT);
}

void CEditListPage1::OnClickedit3Currentdirectory()
{
	AppendEditItem(_T("$(CurDir)"), TOOLINT);
}

void CEditListPage1::OnClickedit3Targetdirectory()
{
	AppendEditItem(_T("$(TargetDir)"), TOOLINT);
}

void CEditListPage1::OnClickedit3Targetname()
{
	AppendEditItem(_T("$(TargetName)"), TOOLINT);
}

void CEditListPage1::OnClickedit3Workspacedirectory()
{
	AppendEditItem(_T("$(WkspDir)"), TOOLINT);
}

void CEditListPage1::OnClickedit3Workspacename()
{
	AppendEditItem(_T("$(WkspName)"), TOOLINT);
}

void CEditListPage1::OnLabelEditEnd()
{
	TRACE0("CEditListPage1::OnLabelEditEnd() called.\n");
}

void CEditListPage1::OnLabelEditCancel()
{
	TRACE0("CEditListPage1::OnLabelEditCancel() called.\n");
}

void CEditListPage1::OnNewItem()
{
	int iItem = m_editList.GetCurrentIndex();
	if (iItem != -1)
	{
		TOOLS* pTools = new TOOLS;
		ASSERT(pTools);

		pTools->strCmd.Empty();
		pTools->strArg.Empty();
		pTools->strInt.Empty();

		m_editList.SetItemData(iItem, (DWORD_PTR)pTools);
		m_arTools.AddTail(pTools);

		m_editList.SetCurSel(iItem);

		m_edit1.SetWindowText(_T(""));
		m_edit2.SetWindowText(_T(""));
		m_edit3.SetWindowText(_T(""));
	}
}

void CEditListPage1::OnDeleteItem()
{
	OnSelchangeEditList();
}

void CEditListPage1::OnMoveItemUp()
{
	TRACE0("CEditListPage1::OnMoveItemUp() called.\n");
}

void CEditListPage1::OnMoveItemDown()
{
	TRACE0("CEditListPage1::OnMoveItemDown() called.\n");
}

/////////////////////////////////////////////////////////////////////////////
// CEditListPage2 property page

CEditListPage2::CEditListPage2() : CXTResizePropertyPage(CEditListPage2::IDD)
{
	//{{AFX_DATA_INIT(CEditListPage2)
	//}}AFX_DATA_INIT
}

CEditListPage2::~CEditListPage2()
{
}

void CEditListPage2::DoDataExchange(CDataExchange* pDX)
{
	CXTResizePropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditListPage2)
	DDX_Control(pDX, IDC_EDIT_LIST, m_editList);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNCREATE(CEditListPage2, CXTResizePropertyPage)

BEGIN_MESSAGE_MAP(CEditListPage2, CXTResizePropertyPage)
	//{{AFX_MSG_MAP(CEditListPage2)
	//}}AFX_MSG_MAP
	ON_LBN_XT_LABELEDITEND(IDC_EDIT_LIST, OnLabelEditEnd)
	ON_LBN_XT_LABELEDITCANCEL(IDC_EDIT_LIST, OnLabelEditCancel)
	ON_LBN_XT_NEWITEM(IDC_EDIT_LIST, OnNewItem)
	ON_LBN_XT_DELETEITEM(IDC_EDIT_LIST, OnDeleteItem)
	ON_LBN_XT_MOVEITEMUP(IDC_EDIT_LIST, OnMoveItemUp)
	ON_LBN_XT_MOVEITEMDOWN(IDC_EDIT_LIST, OnMoveItemDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditListPage2 message handlers

BOOL CEditListPage2::OnInitDialog()
{
	CXTResizePropertyPage::OnInitDialog();

	// Call SetListEditStyle to set the type of edit list. You can
	// pass in LBS_XT_NOTOOLBAR if you don't want the toolbar displayed.
	m_editList.SetListEditStyle(
		_T(" Search for Files:"), LBS_XT_CHOOSEFILE);

	// initialize the edit list box.
	m_editList.Initialize();

	CString strText;
	int iItem;
	for (iItem = 0; iItem < 25; ++iItem)
	{
		strText.Format (_T("List Item %d"), iItem);
		m_editList.AddString (strText);
	}

	// Set control resizing.
	SetResize(IDC_EDIT_LIST, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditListPage2::OnLabelEditEnd()
{
	TRACE0("CEditListPage2::OnLabelEditEnd() called.\n");
}

void CEditListPage2::OnLabelEditCancel()
{
	TRACE0("CEditListPage2::OnLabelEditCancel() called.\n");
}

void CEditListPage2::OnNewItem()
{
	TRACE0("CEditListPage2::OnNewItem() called.\n");
}

void CEditListPage2::OnDeleteItem()
{
	TRACE0("CEditListPage2::OnDeleteItem() called.\n");
}

void CEditListPage2::OnMoveItemUp()
{
	TRACE0("CEditListPage2::OnMoveItemUp() called.\n");
}

void CEditListPage2::OnMoveItemDown()
{
	TRACE0("CEditListPage2::OnMoveItemDown() called.\n");
}

/////////////////////////////////////////////////////////////////////////////
// CEditListPage3 property page

CEditListPage3::CEditListPage3() : CXTResizePropertyPage(CEditListPage3::IDD)
{
	//{{AFX_DATA_INIT(CEditListPage3)
	//}}AFX_DATA_INIT
}

CEditListPage3::~CEditListPage3()
{
}

void CEditListPage3::DoDataExchange(CDataExchange* pDX)
{
	CXTResizePropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditListPage3)
	DDX_Control(pDX, IDC_EDIT_LIST, m_editList);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNCREATE(CEditListPage3, CXTResizePropertyPage)

BEGIN_MESSAGE_MAP(CEditListPage3, CXTResizePropertyPage)
	//{{AFX_MSG_MAP(CEditListPage3)
	//}}AFX_MSG_MAP
	ON_LBN_XT_LABELEDITEND(IDC_EDIT_LIST, OnLabelEditEnd)
	ON_LBN_XT_LABELEDITCANCEL(IDC_EDIT_LIST, OnLabelEditCancel)
	ON_LBN_XT_NEWITEM(IDC_EDIT_LIST, OnNewItem)
	ON_LBN_XT_DELETEITEM(IDC_EDIT_LIST, OnDeleteItem)
	ON_LBN_XT_MOVEITEMUP(IDC_EDIT_LIST, OnMoveItemUp)
	ON_LBN_XT_MOVEITEMDOWN(IDC_EDIT_LIST, OnMoveItemDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditListPage3 message handlers

BOOL CEditListPage3::OnInitDialog()
{
	CXTResizePropertyPage::OnInitDialog();

	// Call SetListEditStyle to set the type of edit list. You can
	// pass in LBS_XT_NOTOOLBAR if you don't want the toolbar displayed.
	m_editList.SetListEditStyle(
		_T(" Search for Directories:"), LBS_XT_CHOOSEDIR);

	// initialize the edit list box.
	m_editList.Initialize();

	CString strText;
	int iItem;
	for (iItem = 0; iItem < 25; ++iItem)
	{
		strText.Format (_T("List Item %d"), iItem);
		m_editList.AddString (strText);
	}

	// m_editList.SetNewItemDefaultText(_T("C:\\Program Files"));

	// Set control resizing.
	SetResize(IDC_EDIT_LIST, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditListPage3::OnLabelEditEnd()
{
	TRACE0("CEditListPage3::OnLabelEditEnd() called.\n");
}

void CEditListPage3::OnLabelEditCancel()
{
	TRACE0("CEditListPage3::OnLabelEditCancel() called.\n");
}

void CEditListPage3::OnNewItem()
{
	TRACE0("CEditListPage3::OnNewItem() called.\n");
}

void CEditListPage3::OnDeleteItem()
{
	TRACE0("CEditListPage3::OnDeleteItem() called.\n");
}

void CEditListPage3::OnMoveItemUp()
{
	TRACE0("CEditListPage3::OnMoveItemUp() called.\n");
}

void CEditListPage3::OnMoveItemDown()
{
	TRACE0("CEditListPage3::OnMoveItemDown() called.\n");
}

void CEditListPage1::OnChkEnable()
{
	UpdateData();

	m_txtEdit3.EnableWindow(m_bEnable);
	m_txtEdit2.EnableWindow(m_bEnable);
	m_txtEdit1.EnableWindow(m_bEnable);
	m_edit3.EnableWindow(m_bEnable);
	m_edit2.EnableWindow(m_bEnable);
	m_edit1.EnableWindow(m_bEnable);
	m_editList.EnableWindow(m_bEnable);
}
