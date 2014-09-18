// InSertPsStringDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InSertPsString.h"
#include "InSertPsStringDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInSertPsStringDlg dialog

CInSertPsStringDlg::CInSertPsStringDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInSertPsStringDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInSertPsStringDlg)
	m_FilePath = _T("");
	m_Value = _T("");
	m_ShowText = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	char m_ExeName[256] = {0};
	GetModuleFileName(NULL, m_ExeName, 255);
	m_IniFileName = m_ExeName;
	m_IniFileName.Replace(".exe", ".ini");
}

void CInSertPsStringDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInSertPsStringDlg)
	DDX_Text(pDX, IDC_EDIT_FILEPATH, m_FilePath);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_Value);
	DDX_Text(pDX, IDC_EDIT_SHOWTEXT, m_ShowText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInSertPsStringDlg, CDialog)
	//{{AFX_MSG_MAP(CInSertPsStringDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_INSERT, OnButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_PATH, OnButtonPath)
	ON_BN_CLICKED(IDC_BUTTON_SAVEAS, OnButtonSaveas)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInSertPsStringDlg message handlers

BOOL CInSertPsStringDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	TCHAR m_sFilePath[256] = {0};
	GetPrivateProfileString("setup", _T("path"), _T(""), m_sFilePath, 255, m_IniFileName);
	
	HANDLE hFile = CreateFile(m_sFilePath, GENERIC_READ, FILE_SHARE_READ, NULL,	OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		WritePrivateProfileString("setup", _T("path"), "", m_IniFileName);
		return TRUE;
	}
	DWORD nReadLen = 0;
	DWORD nFileLen = GetFileSize(hFile, NULL);
	char* pData = new char[nFileLen + 1];
	ReadFile(hFile, pData, nFileLen, &nReadLen, NULL);
	pData[nFileLen] = 0;
	CloseHandle(hFile);

	m_ShowText = pData;
	delete [] pData;

	//取当前标识号
	CString m_FindStr = m_ShowText;
	int pos = m_FindStr.ReverseFind('\n');
	if(pos == -1)
	{
		MessageBox("数据非法", "提示");
		return TRUE;
	}
	if(pos == m_ShowText.GetLength() - 1)
	{
		//字串最尾部
		m_FindStr = m_FindStr.Left(m_ShowText.GetLength() - 2);
		pos = m_FindStr.ReverseFind('\n');
		if(pos == -1)
		{
			MessageBox("数据非法", "提示");
			return TRUE;
		}
	}

	//拷贝最后的一项
	m_FindStr = m_FindStr.Right(m_FindStr.GetLength() - pos - 4);

	char m_Text[256] = {0};
	sprintf(m_Text, "当前序列号[%s]", m_FindStr.Left(5));
	SetWindowText(m_Text);
	
	m_FilePath = m_sFilePath;
	UpdateData(FALSE);

	GetDlgItem(IDC_EDIT_SHOWTEXT)->SendMessage(WM_VSCROLL, MAKELONG(SB_BOTTOM, 0), 0L);
	((CEdit*) GetDlgItem(IDC_EDIT_SHOWTEXT))->SetSel(m_ShowText.GetLength(), -1);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CInSertPsStringDlg::OnPaint() 
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
HCURSOR CInSertPsStringDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CInSertPsStringDlg::OnOK() 
{
	
	CDialog::OnOK();
}

void CInSertPsStringDlg::OnCancel() 
{
	
	CDialog::OnCancel();
}

void CInSertPsStringDlg::OnButtonInsert() 
{
	UpdateData();
	if(m_FilePath.IsEmpty() || m_Value.IsEmpty())
	{
		return;
	}

	//查找标识
	CString m_FindStr = "=" + m_Value + "\r\n";

	//查看字串是否存在
	int pos = m_ShowText.Find(m_FindStr, 0);
	if(m_ShowText.Find(m_FindStr, 0) != -1)
	{
		m_FindStr = "字串资源【" + m_Value + " 】已经存在【" + m_ShowText.Mid(pos - 5, 5) + "】";
		MessageBox(m_FindStr, "提示");
		return;
	}

	//取当前标识号
	m_FindStr = m_ShowText;
	pos = m_FindStr.ReverseFind('\n');
	if(pos == -1)
	{
		MessageBox("数据非法", "提示");
		return;
	}
	if(pos == m_ShowText.GetLength() - 1)
	{
		//字串最尾部
		m_FindStr = m_FindStr.Left(m_ShowText.GetLength() - 2);
		pos = m_FindStr.ReverseFind('\n');
		if(pos == -1)
		{
			MessageBox("数据非法", "提示");
			return;
		}
	}
	
	//拷贝最后的一项
	m_FindStr = m_FindStr.Right(m_FindStr.GetLength() - pos - 1);
	DWORD nCount = atoi(m_FindStr.Left(5));
	nCount ++;

	//替换尾部
	char m_Text[256] = {0};
	sprintf(m_Text, "%d=%s\r\n", nCount, m_Value);
	((CEdit*) GetDlgItem(IDC_EDIT_SHOWTEXT))->SetSel(m_ShowText.GetLength(), -1);
	((CEdit*) GetDlgItem(IDC_EDIT_SHOWTEXT))->ReplaceSel(m_Text);
	GetDlgItem(IDC_EDIT_SHOWTEXT)->GetWindowText(m_ShowText);
	GetDlgItem(IDC_EDIT_SHOWTEXT)->SendMessage(WM_VSCROLL, MAKELONG(SB_BOTTOM, 0), 0L);

	//更新序列总数
	sprintf(m_Text, "当前序列号[%d]", nCount);
	SetWindowText(m_Text);

	HANDLE hFile = CreateFile(m_FilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		DWORD ret = GetLastError();
		MessageBox("无法保存文件", "错误");
		return;
	}
	
	UpdateData();
	char* pData = new char[m_ShowText.GetLength() + 1];
	lstrcpy(pData, m_ShowText);
	
	DWORD nWriteLen = 0;
	WriteFile(hFile, pData, m_ShowText.GetLength(), &nWriteLen, NULL);
	CloseHandle(hFile);
	delete [] pData;

}

void CInSertPsStringDlg::OnButtonPath() 
{
	OPENFILENAME OpenFileName = {0};
	TCHAR szFile[MAX_PATH] = {0};
	TCHAR szFileName[MAX_PATH] = {0};
  	TCHAR szFilter[1024] = {0};
	lstrcpy(szFilter, _T("String文件(*.ini)"));
	lstrcpy(szFilter + lstrlen(szFilter) + 1, _T("*.ini"));
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
    OpenFileName.lpstrTitle        = _T("PCSHARE文件");
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = _T("ini");
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(!GetOpenFileName(&OpenFileName))
	{
		return;
	}
	m_FilePath = szFile;
	
	HANDLE hFile = CreateFile(m_FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox("无法打开文件", "错误");
		return;
	}
	DWORD nReadLen = 0;
	DWORD nFileLen = GetFileSize(hFile, NULL);
	char* pData = new char[nFileLen + 1];
	ReadFile(hFile, pData, nFileLen, &nReadLen, NULL);
	pData[nFileLen] = 0;
	CloseHandle(hFile);

	m_ShowText = pData;
	delete [] pData;

	//取当前标识号
	CString m_FindStr = m_ShowText;
	int pos = m_FindStr.ReverseFind('\n');
	if(pos == -1)
	{
		MessageBox("数据非法", "提示");
		return;
	}
	if(pos == m_ShowText.GetLength() - 1)
	{
		//字串最尾部
		m_FindStr = m_FindStr.Left(m_ShowText.GetLength() - 2);
		pos = m_FindStr.ReverseFind('\n');
		if(pos == -1)
		{
			MessageBox("数据非法", "提示");
			return;
		}
	}

	//拷贝最后的一项
	m_FindStr = m_FindStr.Right(m_FindStr.GetLength() - pos - 4);

	char m_Text[256] = {0};
	sprintf(m_Text, "当前序列号[%s]", m_FindStr.Left(5));
	SetWindowText(m_Text);
	UpdateData(FALSE);
	GetDlgItem(IDC_EDIT_SHOWTEXT)->SendMessage(WM_VSCROLL, MAKELONG(SB_BOTTOM, 0), 0L);
	((CEdit*) GetDlgItem(IDC_EDIT_SHOWTEXT))->SetSel(m_ShowText.GetLength(), -1);

	WritePrivateProfileString("setup", _T("path"), m_FilePath, m_IniFileName);
}

void CInSertPsStringDlg::OnButtonSaveas() 
{
	OPENFILENAME OpenFileName = {0};
	TCHAR szFile[MAX_PATH] = {0};
  	TCHAR szFilter[1024] = {0};
	lstrcpy(szFilter, _T("String文件(*.ini)"));
	lstrcpy(szFilter + lstrlen(szFilter) + 1, _T("*.ini"));
	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
    OpenFileName.hwndOwner         = GetSafeHwnd();
	OpenFileName.lpstrFilter       = szFilter;
    OpenFileName.lpstrCustomFilter = NULL;
    OpenFileName.nMaxCustFilter    = 0;
    OpenFileName.nFilterIndex      = 0;
    OpenFileName.lpstrFile         = szFile;
    OpenFileName.nMaxFile          = sizeof(szFile);
    OpenFileName.lpstrFileTitle    = NULL;
    OpenFileName.nMaxFileTitle     = 0;
    OpenFileName.lpstrInitialDir   = NULL;
    OpenFileName.lpstrTitle        = _T("保存String文件");
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = _T("ini");
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(!GetSaveFileName(&OpenFileName))
	{
		return;
	}

	HANDLE hFile = CreateFile(szFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox("无法保存文件", "错误");
		return;
	}
	
	UpdateData();
	char* pData = new char[m_ShowText.GetLength() + 1];
	lstrcpy(pData, m_ShowText);
	
	DWORD nWriteLen = 0;
	WriteFile(hFile, pData, m_ShowText.GetLength(), &nWriteLen, NULL);
	CloseHandle(hFile);
	delete [] pData;
}
