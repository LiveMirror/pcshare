// PropertiesDlg.cpp : implementation file
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
#include "BindSample.h"
#include "PropertiesDlg.h"

#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertiesDlg dialog

CPropertiesDlg::CPropertiesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertiesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_strAssemblyName = _T("WindowsApplication1");
	m_strOutputType = _T("WindowsApplication");
	m_strDefaultNamespace = _T("WindowsApplication1");

	m_strApplicationIcon = _T("App.ico");
	m_strProjectFile = _T("WindowsApplication1.csproj");
	m_strProjectFolder = _T("C:\\Documents and Settings\\User\\My Documents\\Visual Studio Projects\\WindowsApplication1\\");
	m_strOutputFile = _T("WindowsApplication1.exe");

	m_strPageLayout = _T("Grid");
	m_strTargetSchema = _T("Internet Explorer 5.0");
	m_strClentScriptLanguage = _T("VBScript");

	m_strCompilationConstants = _T("DEBUG;TRACE");
	m_bOptimizeCode = FALSE;
	m_bCheckForOverflow = FALSE;
	m_bAlowUnsafe = FALSE;
	m_strWarningLevel = _T("Warning level 4");
	m_bTreatWarnings = FALSE;

	m_pSelected = NULL;
}

void CPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertiesDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CPropertiesDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TVN_GETDISPINFO, IDC_LIST_OPTIONS, OnGetdispinfoListOptions)
	ON_NOTIFY(TVN_SELCHANGED, IDC_LIST_OPTIONS, OnSelchangedListOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertiesDlg message handlers

BOOL CPropertiesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);         // Set big icon
	SetIcon(m_hIcon, FALSE);        // Set small icon


	m_wndProperties.SubclassDlgItem(IDC_LIST_OPTIONS, this);

	GetDlgItem(IDC_WORKSPACE)->GetWindowRect(m_rcWorkspace);
	ScreenToClient(m_rcWorkspace);

	HTREEITEM hItemCommon = m_wndProperties.InsertItem(_T("Common Properties"), I_IMAGECALLBACK , I_IMAGECALLBACK );
	CreateGridGeneral(hItemCommon);
	CreateGridDesigner(hItemCommon);

	HTREEITEM hItemConfiguration = m_wndProperties.InsertItem(_T("Configuration Properties"), I_IMAGECALLBACK , I_IMAGECALLBACK );
	CreateGridBuild(hItemConfiguration);


	m_pSelected = &m_wndGridGeneral;


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPropertiesDlg::CreateGridGeneral(HTREEITEM hItemCategory)
{
	m_wndGridGeneral.Create(m_rcWorkspace, this, 0);

	CXTPPropertyGridItem* pCategoryApplication = m_wndGridGeneral.AddCategory(_T("Application"));
	pCategoryApplication->AddChildItem(new CXTPPropertyGridItem(_T("Assembly Name"), m_strAssemblyName, &m_strAssemblyName));
	CXTPPropertyGridItem* pOutputType = pCategoryApplication->AddChildItem(new CXTPPropertyGridItem(_T("Output Type"), m_strOutputType, &m_strOutputType));
	pOutputType->GetConstraints()->AddConstraint(_T("Windows Application"));
	pOutputType->GetConstraints()->AddConstraint(_T("Console Application"));
	pOutputType->GetConstraints()->AddConstraint(_T("Class Library"));
	pOutputType->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
	pOutputType->SetConstraintEdit(TRUE);

	pCategoryApplication->AddChildItem(new CXTPPropertyGridItem(_T("Default Namespace"), m_strDefaultNamespace, &m_strDefaultNamespace));

	// You can bind via constructor or Bind* methods:
	pCategoryApplication->AddChildItem(new CXTPPropertyGridItem(_T("Startup Object"), m_strStartupObject));
	pCategoryApplication->BindToString(&m_strStartupObject);

	pCategoryApplication->AddChildItem(new CXTPPropertyGridItem(_T("Application Icon"), m_strApplicationIcon, &m_strApplicationIcon));

	CXTPPropertyGridItem* pCategoryProject = m_wndGridGeneral.AddCategory(_T("Project"));
	CXTPPropertyGridItem* pProjectFile = pCategoryProject->AddChildItem(new CXTPPropertyGridItem(_T("Project File"), m_strProjectFile, &m_strProjectFile));
	pProjectFile->SetReadOnly(TRUE);
	CXTPPropertyGridItem* pProjectFolder = pCategoryProject->AddChildItem(new CXTPPropertyGridItem(_T("Project Folder"), m_strProjectFolder, &m_strProjectFolder));
	pProjectFolder->SetReadOnly(TRUE);
	CXTPPropertyGridItem* pOutputFile = pCategoryProject->AddChildItem(new CXTPPropertyGridItem(_T("Output File"), m_strOutputFile, &m_strOutputFile));
	pOutputFile->SetReadOnly(TRUE);


	pCategoryApplication->Expand();
	pCategoryProject->Expand();

	HTREEITEM hItem = m_wndProperties.InsertItem(_T("General"), I_IMAGECALLBACK, I_IMAGECALLBACK, hItemCategory);

	m_wndProperties.SetItemData(hItem, (DWORD_PTR)&m_wndGridGeneral);
	m_wndProperties.SetItemData(hItemCategory, (DWORD_PTR)&m_wndGridGeneral);
	m_wndGridGeneral.m_hItem = hItem;
}

void CPropertiesDlg::CreateGridDesigner(HTREEITEM hItemCategory)
{
	m_wndGridDesigner.Create(m_rcWorkspace, this, 0);

	CXTPPropertyGridItem* pCategoryWeb = m_wndGridDesigner.AddCategory(_T("Web Designers"));
	CXTPPropertyGridItem* pPageLayout = pCategoryWeb->AddChildItem(new CXTPPropertyGridItem(_T("Page Layout"), m_strPageLayout, &m_strPageLayout));
	pPageLayout->GetConstraints()->AddConstraint(_T("Flow"));
	pPageLayout->GetConstraints()->AddConstraint(_T("Grid"));
	pPageLayout->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
	pPageLayout->SetConstraintEdit(TRUE);

	pCategoryWeb->AddChildItem(new CXTPPropertyGridItem(_T("Target Schema"), m_strTargetSchema, &m_strTargetSchema));

	CXTPPropertyGridItem* pCategoryScripting = m_wndGridDesigner.AddCategory(_T("Web Scripting"));
	CXTPPropertyGridItem* pScriptLanguage = pCategoryScripting->AddChildItem(new CXTPPropertyGridItem(_T("Clent Script Language"), m_strClentScriptLanguage, &m_strClentScriptLanguage));
	pScriptLanguage->GetConstraints()->AddConstraint(_T("VBScript"));
	pScriptLanguage->GetConstraints()->AddConstraint(_T("JScript"));
	pScriptLanguage->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);



	pCategoryWeb->Expand();
	pCategoryScripting->Expand();

	HTREEITEM hItem = m_wndProperties.InsertItem(_T("Designer"), I_IMAGECALLBACK, I_IMAGECALLBACK, hItemCategory);
	m_wndProperties.SetItemData(hItem, (DWORD_PTR)&m_wndGridDesigner);
	m_wndGridDesigner.m_hItem = hItem;

	m_wndGridDesigner.ShowWindow(SW_HIDE);
}

void CPropertiesDlg::CreateGridBuild(HTREEITEM hItemCategory)
{
	m_wndGridBuild.Create(m_rcWorkspace, this, 0);

	CXTPPropertyGridItem* pCategoryCode = m_wndGridBuild.AddCategory(_T("Code Generation"));
	pCategoryCode->AddChildItem(new CXTPPropertyGridItem(_T("Conditional Compilation Constants"), m_strCompilationConstants, &m_strCompilationConstants));
	pCategoryCode->AddChildItem(new CXTPPropertyGridItemBool(_T("Optimize Code"), m_bOptimizeCode, &m_bOptimizeCode));
	pCategoryCode->AddChildItem(new CXTPPropertyGridItemBool(_T("Check for Arithmetic Overflow"), m_bCheckForOverflow, &m_bCheckForOverflow));
	pCategoryCode->AddChildItem(new CXTPPropertyGridItemBool(_T("Alow unsafe code blocks"), m_bAlowUnsafe , &m_bAlowUnsafe));

	CXTPPropertyGridItem* pCategoryError = m_wndGridBuild.AddCategory(_T("Errors and Warnings"));
	pCategoryError->AddChildItem(new CXTPPropertyGridItem(_T("Warning Level"), m_strWarningLevel, &m_strWarningLevel));
	pCategoryError->AddChildItem(new CXTPPropertyGridItemBool(_T("Treat Warnings As Errors"), m_bTreatWarnings, &m_bTreatWarnings));



	pCategoryCode->Expand();
	pCategoryError->Expand();

	HTREEITEM hItem = m_wndProperties.InsertItem(_T("Build"), I_IMAGECALLBACK, I_IMAGECALLBACK, hItemCategory);
	m_wndProperties.SetItemData(hItem, (DWORD_PTR)&m_wndGridBuild);
	m_wndProperties.SetItemData(hItemCategory, (DWORD_PTR)&m_wndGridBuild);
	m_wndGridBuild.m_hItem = hItem;

	m_wndGridBuild.ShowWindow(SW_HIDE);
}


void CPropertiesDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPropertiesDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPropertiesDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CPropertiesDlg::OnGetdispinfoListOptions(NMHDR* pNMHDR, LRESULT* pResult)
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	if (pTVDispInfo->item.mask & (TVIF_SELECTEDIMAGE | TVIF_IMAGE))
	{
		if (m_wndProperties.GetChildItem(pTVDispInfo->item.hItem) != NULL)
		{
			UINT nState = m_wndProperties.GetItemState(pTVDispInfo->item.hItem, TVIF_STATE);
			pTVDispInfo->item.iSelectedImage = pTVDispInfo->item.iImage = nState & TVIS_EXPANDED? 3: 2;
		}
		else
		{
			ASSERT(m_pSelected);
			pTVDispInfo->item.iSelectedImage = pTVDispInfo->item.iImage =
				(m_pSelected->m_hItem == pTVDispInfo->item.hItem? 0: 1);
		}
	}

	*pResult = 0;
}

void CPropertiesDlg::OnSelchangedListOptions(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if (pNMTreeView->itemNew.lParam == 0)
		return;

	if (pNMTreeView->itemNew.hItem)
	{

		CWnd* pPage = (CWnd*)pNMTreeView->itemNew.lParam;

		if (pPage != m_pSelected)
		{
			ASSERT(pPage);
			ASSERT(pPage->IsKindOf(RUNTIME_CLASS(CXTPPropertyGrid)));

			pPage->ShowWindow(SW_SHOW);
			pPage->EnableWindow();
			pPage->ModifyStyle(0, WS_TABSTOP);

			if (m_pSelected)
			{
				m_pSelected->ShowWindow(SW_HIDE);
			}
			m_pSelected = (CPropertyGridEx*)pPage;

			m_wndProperties.Invalidate(FALSE);
		}

		HTREEITEM hItemParent = m_wndProperties.GetParentItem(pNMTreeView->itemNew.hItem);
		if (m_pSelected != NULL && hItemParent)
		{

			m_wndProperties.SetItemData(hItemParent, (DWORD_PTR)m_pSelected);
		}

	}

	*pResult = 0;
}



void CPropertiesDlg::OnOK()
{
#ifdef _DEBUG
	afxDump << _T("m_strAssemblyName = ") << m_strAssemblyName << _T("\n");
	afxDump << _T("m_strOutputType = ") << m_strOutputType << _T("\n");
	afxDump << _T("m_strDefaultNamespace = ") << m_strDefaultNamespace << _T("\n");
	afxDump << _T("m_strApplicationIcon = ") << m_strApplicationIcon << _T("\n");
	afxDump << _T("m_strProjectFile = ") << m_strProjectFile << _T("\n");
	afxDump << _T("m_strProjectFolder = ") << m_strProjectFolder << _T("\n");
	afxDump << _T("m_strOutputFile = ") << m_strOutputFile << _T("\n");
	afxDump << _T("m_strPageLayout = ") << m_strPageLayout << _T("\n");
	afxDump << _T("m_strTargetSchema = ") << m_strTargetSchema << _T("\n");
	afxDump << _T("m_strClentScriptLanguage = ") << m_strClentScriptLanguage << _T("\n");
	afxDump << _T("m_strCompilationConstants = ") << m_strCompilationConstants << _T("\n");
	afxDump << _T("m_bOptimizeCode = ") << m_bOptimizeCode << _T("\n");
	afxDump << _T("m_bCheckForOverflow = ") << m_bCheckForOverflow << _T("\n");
	afxDump << _T("m_bAlowUnsafe = ") << m_bAlowUnsafe << _T("\n");
	afxDump << _T("m_strWarningLevel = ") << m_strWarningLevel << _T("\n");
	afxDump << _T("m_bTreatWarnings = ") << m_bTreatWarnings << _T("\n");
#endif

	CDialog::OnOK();
}
