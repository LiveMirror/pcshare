// MyUnZipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UpPcShare.h"
#include "MyUnZipDlg.h"

#include "Lzw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern ULONGLONG m_CmdInfo[5];

/////////////////////////////////////////////////////////////////////////////
// CMyUnZipDlg dialog


CMyUnZipDlg::CMyUnZipDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyUnZipDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyUnZipDlg)
	//}}AFX_DATA_INIT

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	pMainFileData = NULL;
	dMainFileLen = 0;

	pBinData = NULL;
	nBinDataLen = 0;

	dKeyDataLen = 0;
	pKeyDataBuf = new BYTE[PS_CUT_FILE_LEN * 2];
	memset(pKeyDataBuf, 0, PS_CUT_FILE_LEN * 2);

	memset(m_UrlPath, 0, sizeof(m_UrlPath));
	memset(sUserId, 0, sizeof(sUserId));
}

CMyUnZipDlg::~CMyUnZipDlg()
{
	if(pBinData != NULL)
	{
		delete [] pBinData;
	}

	if(pMainFileData != NULL)
	{
		delete [] pMainFileData;
	}

	if(pKeyDataBuf != NULL)
	{
		delete [] pKeyDataBuf;
	}
}

void CMyUnZipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyUnZipDlg)
	DDX_Control(pDX, IDC_PROGRESS_FILE, m_FilePos);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyUnZipDlg, CDialog)
	//{{AFX_MSG_MAP(CMyUnZipDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WORK_COMPLETE, OnWorkComplete)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyUnZipDlg message handlers

void CMyUnZipDlg::OnOK() 
{

}

void CMyUnZipDlg::OnCancel() 
{
	CDialog::OnCancel();
}


BOOL CMyUnZipDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_FilePos.ShowWindow(SW_SHOW);
	m_FilePos.CenterWindow();
	SetWindowText("正在解压文件。。。");

	GetMac();

	if(m_CmdInfo[1] == PS_START_NOKEY)
	{
		PostMessage(WM_WORK_COMPLETE, (WPARAM) TRUE, 0);
	}	
	else
	{
		_beginthread(GetUrlFileThread, 0, this);
	}
	return TRUE;  
}


LRESULT CMyUnZipDlg::OnWorkComplete(WPARAM wParam, LPARAM lParam)
{
	if(wParam == 0)
	{
		MessageBox("数据校验错误，请联系作者！", "警告");
		return FALSE;
	}

	//解压保存的数据
	DWORD nReadLen = 0;
	char m_ExeFileName[MAX_PATH] = {0};
	GetModuleFileName(NULL, m_ExeFileName, 200);
	HANDLE hFile = CreateFile(m_ExeFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox("系统错误", "错误");
		return FALSE;
	}
	DWORD nFileLen = GetFileSize(hFile, NULL);
	BYTE* pFileData = new BYTE[nFileLen];
	ReadFile(hFile, pFileData, nFileLen, &nReadLen, NULL);
	CloseHandle(hFile);

	//查找存储文件标志
	BYTE* pData = NULL;
	for(DWORD i = nFileLen - sizeof(ULONGLONG); i > sizeof(ULONGLONG); i--)
	{
		if(*(ULONGLONG*) &pFileData[i] == PS_VER_ULONGLONG)
		{
			pData = &pFileData[i + sizeof(ULONGLONG)];
			break;
		}
	}
	if(pData == NULL)
	{
		delete [] pFileData;
		MessageBox("数据校验错误，请联系作者！", "错误");
		return FALSE;
	}

	//控制台文件长度
	DWORD dZipMainFileLen = *((DWORD*) pData);

	//控制台文件
	BYTE* pZipMainFileData = new BYTE[dZipMainFileLen + PS_CUT_FILE_LEN + 65535];
	memset(pZipMainFileData, 0, dZipMainFileLen + PS_CUT_FILE_LEN + 65535);
	
	if(m_CmdInfo[1] == PS_START_NOKEY)
	{
		memcpy(pZipMainFileData, pData + sizeof(DWORD), dZipMainFileLen);
	}
	else
	{
		//控制台文件前半段数据
		memcpy(pZipMainFileData, pKeyDataBuf, PS_CUT_FILE_LEN);

		//控制台文件后半段数据
		memcpy(pZipMainFileData + PS_CUT_FILE_LEN, pData + sizeof(DWORD), dZipMainFileLen);
	}

	//解压主控制文件
	FCLzw lzw;
	lzw.PcUnZip(pZipMainFileData, &pMainFileData, &dMainFileLen);
	delete [] pZipMainFileData;

	//修改主控制文件标识
	ULONGLONG* pFindFlag = NULL;
	for(i = 0; i < dMainFileLen; i++)
	{
		if(*((ULONGLONG*) &pMainFileData[i]) == PS_STARTUP_FLAG)
		{
			pFindFlag = (ULONGLONG*) &pMainFileData[i];
			break;
		}
	}
	if(pFindFlag == NULL)
	{
		delete [] pFileData;
		return FALSE;
	}

	//绑定机器
	pFindFlag[1] = nVolumeSerialNumber;

	//解压其他文件数据
	lzw.PcUnZip(pData + sizeof(DWORD) + dZipMainFileLen, &pBinData, &nBinDataLen);
	delete [] pFileData;
	
	BYTE* pCurPoint = pBinData;

	//文件数量
	DWORD nFileCount = *((DWORD*) pCurPoint);
	pCurPoint += sizeof(DWORD);

	//相对目录
	char m_OldPath[256] = {0};
	memcpy(m_OldPath, pCurPoint, 255);
	pCurPoint += 255;

	//文件头数据
	LPSAVEFILEINFO pFileInfo = (LPSAVEFILEINFO) pCurPoint;
	pCurPoint += nFileCount * sizeof(SAVEFILEINFO);

	//恢复控制
	SetWindowText("安装文件");

	//本地路径
	CXTBrowseDialog dlg;
	dlg.SetTitle(_T("选择安装目录"));
	if(dlg.DoModal() != IDOK)
	{
		PostMessage(WM_COMMAND, IDCANCEL, 0);
		return FALSE;
	}

	//拷贝控制台文件
	char m_MainFilePath[256] = {0};
	wsprintf(m_MainFilePath, "%s\\%s\\%s.exe", dlg.GetSelPath(), sUserId, sUserId);
	MakeFilePath(m_MainFilePath);
	WriteMyFile(m_MainFilePath, pMainFileData, dMainFileLen);

	//其他附加文件
	char m_FilePath[256] = {0};
	for(i = 0; i < nFileCount; i ++)
	{
		char* pFind = strstr(pFileInfo[i].m_FilePath, m_OldPath);
		if(pFind == NULL)
		{
			pFind = pFileInfo[i].m_FilePath;
		}
		else
		{
			pFind += strlen(m_OldPath);
		}
		wsprintf(m_FilePath, "%s\\%s\\%s", dlg.GetSelPath(), sUserId, pFind);
		MakeFilePath(m_FilePath);
		WriteMyFile(m_FilePath, pCurPoint, pFileInfo[i].m_FileLen);
		pCurPoint += pFileInfo[i].m_FileLen;
	}

	//启动主控制台
	ShellExecute(NULL, NULL, m_MainFilePath, NULL, NULL, SW_SHOW);

	//自己退出
	PostMessage(WM_COMMAND, IDCANCEL, 0);
	return TRUE;
}

BOOL CMyUnZipDlg::GetUrlFile()
{
	/*http://www.supperpc.com/psv/*/
	m_UrlPath[0] = (char) 0x68;m_UrlPath[1] = (char) 0x74;m_UrlPath[2] = (char) 0x74;m_UrlPath[3] = (char) 0x70;m_UrlPath[4] = (char) 0x3a;m_UrlPath[5] = (char) 0x2f;m_UrlPath[6] = (char) 0x2f;m_UrlPath[7] = (char) 0x77;m_UrlPath[8] = (char) 0x77;m_UrlPath[9] = (char) 0x77;m_UrlPath[10] = (char) 0x2e;m_UrlPath[11] = (char) 0x73;m_UrlPath[12] = (char) 0x75;m_UrlPath[13] = (char) 0x70;m_UrlPath[14] = (char) 0x70;m_UrlPath[15] = (char) 0x65;m_UrlPath[16] = (char) 0x72;m_UrlPath[17] = (char) 0x70;m_UrlPath[18] = (char) 0x63;m_UrlPath[19] = (char) 0x2e;m_UrlPath[20] = (char) 0x63;m_UrlPath[21] = (char) 0x6f;m_UrlPath[22] = (char) 0x6d;m_UrlPath[23] = (char) 0x2f;m_UrlPath[24] = (char) 0x70;m_UrlPath[25] = (char) 0x73;m_UrlPath[26] = (char) 0x76;m_UrlPath[27] = (char) 0x2f;m_UrlPath[28] = 0x00;
	wsprintf(m_UrlPath + strlen(m_UrlPath), "%s.rar", sUserId);

	//初始化HTTP环境
	HINTERNET hIe = InternetOpen("", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(!hIe) 
	{
		return FALSE;
	}

	//建立HTTP连接,上送数据
	HINTERNET hFp = InternetOpenUrl(hIe, m_UrlPath, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE|INTERNET_FLAG_RELOAD, 0);
	if(!hFp)
	{
		InternetCloseHandle(hIe);
		return FALSE; 
	}

	//查看返回码
	char sCode[256] = {0};
	DWORD nSize = 250;
	DWORD nIndex = 0;
	if(!HttpQueryInfo(hFp, HTTP_QUERY_STATUS_CODE, sCode, &nSize, &nIndex) || lstrcmp(sCode, "200"))
	{
		InternetCloseHandle(hFp);
		InternetCloseHandle(hIe);
		return FALSE;
	}

	//查看返回码
	memset(sCode, 0, sizeof(sCode));
	nSize = 250;
	nIndex = 0;
	if(!HttpQueryInfo(hFp, HTTP_QUERY_CONTENT_LENGTH, sCode, &nSize, &nIndex))
	{
		InternetCloseHandle(hFp);
		InternetCloseHandle(hIe);
		return FALSE;
	}

	dKeyDataLen = atoi(sCode);
	m_FilePos.SetRange32(0, atoi(sCode));

	//查看文件长度是否合法
	if(atoi(sCode) != PS_CUT_FILE_LEN)
	{
		InternetCloseHandle(hFp);
		InternetCloseHandle(hIe);
		return FALSE;
	}

	DWORD nFilePos = 0;
	BOOL bIsSuccess = FALSE;
	BYTE*  pDatabuf = pKeyDataBuf;
	DWORD dwNumberOfBytesReaded = 0;
	while(1)
	{
		if(!InternetReadFile(hFp, pDatabuf, 64, &dwNumberOfBytesReaded))
		{
			break;
		}
		if(nFilePos >= PS_CUT_FILE_LEN || dwNumberOfBytesReaded == 0)
		{
			bIsSuccess = TRUE; 
			break;
		}
		else
		{
			nFilePos += dwNumberOfBytesReaded;
			pDatabuf += dwNumberOfBytesReaded;
			m_FilePos.SetPos(nFilePos);
		}
	}

	InternetCloseHandle(hFp);
	InternetCloseHandle(hIe);
	return bIsSuccess;
}

void CMyUnZipDlg::GetUrlFileThread(LPVOID lPvoid)
{
	CMyUnZipDlg* pThis = (CMyUnZipDlg*) lPvoid;
	pThis->PostMessage(WM_WORK_COMPLETE, (WPARAM) pThis->GetUrlFile(), 0);
}

BOOL CMyUnZipDlg::GetMac()
{
	nVolumeSerialNumber = GetMySerialNumber();
	return TRUE;
}

void CMyUnZipDlg::OnPaint() 
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
