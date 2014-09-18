// SrcModifyToolDlg.cpp : implementation file
//
#include "stdafx.h"
#include "SrcModifyTool.h"
#include "SrcModifyToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSrcModifyToolDlg dialog

CSrcModifyToolDlg::CSrcModifyToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSrcModifyToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSrcModifyToolDlg)
	m_strRunState = _T("");
	m_strSrcPath = _T("");

	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSrcModifyToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSrcModifyToolDlg)
	DDX_Control(pDX, IDC_EDIT_RUNSTATE, m_ctRunState);
	DDX_Text(pDX, IDC_EDIT_RUNSTATE, m_strRunState);
	DDX_Text(pDX, IDC_EDIT_SRCPATH,  m_strSrcPath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSrcModifyToolDlg, CDialog)
	//{{AFX_MSG_MAP(CSrcModifyToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_FOLDERBROWSE, OnBtnFolderbrowse)
	ON_BN_CLICKED(IDD_BTN_MODIFYCODE, OnBtnModifycode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSrcModifyToolDlg message handlers
BOOL CSrcModifyToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0)== IDM_ABOUTBOX);
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
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSrcModifyToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSrcModifyToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width()- cxIcon + 1)/ 2;
		int y = (rect.Height()- cyIcon + 1)/ 2;

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
HCURSOR CSrcModifyToolDlg::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}
int CSrcModifyToolDlg::GetDir(CString& dir )
{	
	char strDir[MAX_PATH];
	char path[MAX_PATH];

	BROWSEINFO bi;
 
	HWND hwnd=this->GetSafeHwnd(); 
	bi.hwndOwner=hwnd; 
	bi.pidlRoot=NULL;//
	bi.pszDisplayName=strDir;//
	bi.lpszTitle ="裁剪结果输出目录";// 


	bi.lpfn=NULL;
	LPITEMIDLIST pidl=SHBrowseForFolder(&bi);
	if(!pidl)
	  return 1;

	
	SHGetPathFromIDList(pidl,path);
	//SHCreateDirectoryEx(NULL, path, NULL );


	CString strPathSep;
	strPathSep.Format("%s\\",path);
	if(strPathSep.Right(2)=="\\\\")
	{
	   strPathSep=path;
	}
	dir= strPathSep ;
	return 0 ;
}
void CSrcModifyToolDlg::OnBtnFolderbrowse()
{
    CString path ;
    GetDir(m_strSrcPath);
	UpdateData(FALSE);
}

char* FindMyChar(char* pStr, char cFind)
{
	char* pStart = pStr;
	while(*pStart != 0x00)
	{
		if(*pStart == cFind)
		{
			return pStart;
		}
		else if(*pStart == ' ' || *pStart == 9)
		{
			pStart++;
		}
		else
		{
			pStr = pStart;
			return NULL;
		}
	}
	return NULL;
}

char* GetMyChar(char* pStr, char cFind)
{
	char* pStart = pStr;
	while(*pStart != 0x00)
	{
		if(*pStart == cFind)
		{
			return pStart;
		}
		pStart++;
	}
	return NULL;
}

BOOL CSrcModifyToolDlg::ModifySingleSrcFile(char *szSrcFilename, char *szAsmOpCodes)
{
	CharLower(szSrcFilename);
	if(strstr(szSrcFilename, "\\pcshare\\") != NULL)
	{
//		return TRUE;
	}

	CFile m_File;
	if(!m_File.Open(szSrcFilename, CFile::modeRead))
	{
		return FALSE;
	}

	DWORD nFileLen = m_File.GetLength();
	if(nFileLen == 0)
	{
		m_File.Close();
		return FALSE;
	}

	char* pSrcFileBuf = new char[nFileLen + 65535];
	memset(pSrcFileBuf, 0, nFileLen + 65535);
	m_File.Read(pSrcFileBuf, nFileLen);
	m_File.Close();

	//开始修改文件
	if(!m_File.Open(szSrcFilename, CFile::modeCreate|CFile::modeWrite))
	{
		delete [] pSrcFileBuf;
		return FALSE;
	}

	//确定起点
	char* pCurPoint = pSrcFileBuf;
	char* pSavePoint = pSrcFileBuf;
	while((DWORD) (pCurPoint - pSrcFileBuf) < nFileLen)
	{
		//找到')'
		pCurPoint = GetMyChar(pSavePoint, ')');
		if(pCurPoint == NULL)
		{
			break;
		}

		pCurPoint++;
		pSavePoint = pCurPoint;

		//找到'/r'
		pCurPoint = FindMyChar(pSavePoint, '\r');
		if(pCurPoint == NULL)
		{
			pCurPoint = pSavePoint;
			continue;
		}

		pCurPoint++;
		pSavePoint = pCurPoint;

		//找到'/n'
		pCurPoint = FindMyChar(pSavePoint, '\n');
		if(pCurPoint == NULL)
		{
			pCurPoint = pSavePoint;
			continue;
		}

		pCurPoint++;
		pSavePoint = pCurPoint;

		//找到'{'
		pCurPoint = FindMyChar(pSavePoint, '{');
		if(pCurPoint != NULL)
		{
			break;
		}
		else
		{
			pCurPoint = pSavePoint;
		}
	}

	if(pCurPoint == NULL)
	{
		m_File.Write(pSrcFileBuf, nFileLen);
		m_File.Close();
		delete [] pSrcFileBuf;
		return TRUE;
	}
	else
	{
		//保存数据
		m_File.Write(pSrcFileBuf, pCurPoint - pSrcFileBuf);
	}
	
	while(*pCurPoint != 0x00)
	{
		if(*pCurPoint == ';' && *(pCurPoint + 1) == '\r' && *(pCurPoint + 2) == '\n')
		{
			//查看是否为return
			char* pRchr = pCurPoint;
			while(*pRchr != '\n')
			{
				pRchr --;
			}
			char* pTmpStr = new char[pCurPoint - pRchr + 100];
			memset(pTmpStr, 0, pCurPoint - pRchr + 100);
			memcpy(pTmpStr, pRchr, pCurPoint - pRchr + 1);
			if(strstr(pTmpStr, "return") == NULL && strstr(pTmpStr, " theApp;") == NULL && strstr(pTmpStr, "};") == NULL && strstr(pTmpStr, "break;") == NULL)
			{
					//插入代码
					m_File.Write(";\r\n", 3);
					m_File.Write(szAsmOpCodes, strlen(szAsmOpCodes));
			}
			else
			{
				m_File.Write(pCurPoint, 1);
			}
			delete [] pTmpStr;
		}
		else
		{
			m_File.Write(pCurPoint, 1);
		}
		pCurPoint++;
	}
	m_File.Close();

	delete [] pSrcFileBuf;
	return TRUE;
}

void CSrcModifyToolDlg::BatchModifySrcFiles(CString strSrcFilePath, char* szAsmOpCode)
{	

	UpdateData(TRUE);
	if (m_strSrcPath.GetAllocLength()<1)
	{
		AfxMessageBox("请选择路径!");
		return ;
	}
	CFileFind finder;
	
	// build a string with wildcards
	CString strWildcard(strSrcFilePath);
	//strWildcard += _T("\\*.*");
	strWildcard =strSrcFilePath+ _T("\\*.*");
	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);
	
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
		{
			continue;
		}
		
		// if it's a directory, recursively search it
		if (finder.IsDirectory())
		{
			CString str = finder.GetFilePath();
			BatchModifySrcFiles(str,szAsmOpCode);
		}
		else
		{
			CString strFilename = finder.GetFilePath();
			CString strExt = strFilename.Right(4);
			strExt.MakeLower();
			if(strExt.Right(4)== ".cpp"||strExt.Right(2)== ".c")
			{
				char szFilename[256];
				strcpy(szFilename,strFilename);
				ModifySingleSrcFile(szFilename,szAsmOpCode);
				m_strRunState += szFilename;
				m_strRunState += "\r\n";
				m_ctRunState.SetWindowText(m_strRunState);
				m_ctRunState.Invalidate();
			}
		}
	}
	
	finder.Close();
}

void CSrcModifyToolDlg::OnBtnModifycode()
{	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strSrcPath.GetAllocLength() < 1 )
	{
		AfxMessageBox("请选择路径!");
		return ;
	}

	m_strRunState = "";
	char szOldAsmOpCode[2048] = {0};
	char szNewAsmOpCode[2048] = {0};
	strcpy(szOldAsmOpCode,
		"{m_Ext_A[1] += 0x23435; m_Ext_A[2] -= 0x88677;}"
	//	"if(m_Ext_Value ++ > 1000000) m_Ext_Value = PS_USER_ID;\r\n"
		);
	BatchModifySrcFiles(m_strSrcPath,szOldAsmOpCode);
	MessageBox("OK，此次操作完毕！");
	UpdateData(FALSE);
}

void CSrcModifyToolDlg::EnCryptString(char* szSrcStr)
{	
	int i;
	if (strlen(szSrcStr)< 1)
	{
		return ;
	}

	for(i=0; i<(int)strlen(szSrcStr); i++)
	{
		*(szSrcStr+i)+= 1;
	}
	return ;
}

BOOL CSrcModifyToolDlg::ModifySingleSrcFileStrRes(char *szSrcFilename, char* szModifyFlag)
{	

	CFile hFile;
	long  nOldFileSize = 0;
	char  *pSrcFileContent;
	int nStart = 0,nEnd = 0;
	CString strReplaceStr;
	CString strOldCode;
	CString strNewCode;
	
	hFile.Open(szSrcFilename,CFile::modeReadWrite);
	if(hFile == -1)
		return FALSE;
	nOldFileSize = hFile.GetLength();
	pSrcFileContent = new char[nOldFileSize+1];
	hFile.ReadHuge(pSrcFileContent,nOldFileSize);
	*(pSrcFileContent+nOldFileSize)= 0x00;
	CString strSrcCode = pSrcFileContent;
	delete pSrcFileContent;
	pSrcFileContent = NULL;


	nStart = strSrcCode.Find(szModifyFlag,nStart);
	while (nStart != -1)
	{
		nStart += strlen(szModifyFlag);
		nEnd = strSrcCode.Find("\r\n",nStart);
		strOldCode = strSrcCode.Mid(nStart,nEnd-nStart);
		strNewCode = GetReplaceCode(strOldCode);
		strOldCode = szModifyFlag + strOldCode;
		strNewCode = szModifyFlag + strNewCode;
		strSrcCode.Delete(nStart-strlen(szModifyFlag),strOldCode.GetLength());
		strSrcCode.Insert(nStart-strlen(szModifyFlag),strNewCode);
		nStart = strSrcCode.Find(szModifyFlag,nEnd);
	}

	hFile.SetLength(0);
	hFile.SeekToBegin();
	nOldFileSize = strSrcCode.GetLength();
	char *pSrcFile;
	pSrcFile = new char[nOldFileSize*2];
	memset(pSrcFile,0,nOldFileSize*2);
	strcpy(pSrcFile,strSrcCode);
	hFile.Write(pSrcFile,strlen(pSrcFile));
	hFile.Close();
	delete pSrcFile;
	
	return TRUE;
}

void CSrcModifyToolDlg::GetModifyCode(char *szFlagCode)
{	
	DWORD dwLen = 0;
	char *p1,*p2;
	char szTargetStr[0x400];
	CString strTemp,strTemp1,strTemp2;
	p1 = strchr(szFlagCode,(int)',');
	p1++;
	p2 = strchr(p1,(int)',');
	dwLen = (DWORD)((DWORD)p2-(DWORD)p1);
	memcpy(szTargetStr,p1,dwLen);
	if (szTargetStr[0] == 'L')
	{
		p2 = szTargetStr+2;
		*(p2+dwLen-3)= 0;
		EnCryptString(p2);
		strTemp = p1+2;
		strTemp1 = strTemp.Left(strlen(p2));
		strTemp2 = p2;
		strTemp.Replace(strTemp1,strTemp2);
		strcpy(p1+2,strTemp);
	}
	else
	{
		p2 = szTargetStr+1;
		*(p2+dwLen-2)= 0;
		EnCryptString(p2);
		strTemp = p1+1;
		strTemp1 = strTemp.Left(strlen(p2));
		strTemp2 = p2;
		strTemp.Replace(strTemp1,strTemp2);
		strcpy(p1+1,strTemp);
	}
}

CString CSrcModifyToolDlg::GetReplaceCode(CString strSrcCode)
{	
	int nOpFunctionNameLen = 0;
	CString strOpFunctionName;
	nOpFunctionNameLen = strSrcCode.Find("(",0);
	strOpFunctionName = strSrcCode.Left(nOpFunctionNameLen);
	if (strOpFunctionName == "memcpy")
	{
		return GetMemcpyReplaceCode(strSrcCode);
	}
	if (strOpFunctionName == "strcpy")
	{
		return GetStrcpyReplaceCode(strSrcCode);
	}
	
	return "";
}

CString CSrcModifyToolDlg::GetMemcpyReplaceCode(CString strSrcCode)
{	
	int		i;
	int		nVarLen;
	int		nStart = 0,nEnd = 0;
	char	szVarValue[0x400] = {0};
	CString strNewAddCode;
	CString strTemp;
	CString strReplaceStr;
	CString strVarName;
	CString strVarValue;
	
	nStart = strSrcCode.Find("(",0);
	nStart++;
	strReplaceStr = strSrcCode;
	nEnd = strReplaceStr.Find(",",nStart);
	strVarName = strReplaceStr.Mid(nStart,nEnd-nStart);
	
	nStart = nEnd;
	nStart++;
	nEnd = strReplaceStr.Find(",",nStart);
	strVarValue = strReplaceStr.Mid(nStart,nEnd-nStart);
	strVarValue.Replace("\\\\","\\");
	
	if (strVarValue.Left(1)=="L")
	{
		strcpy(szVarValue,strVarValue.Right(strVarValue.GetLength()-2));
		nVarLen = strlen(szVarValue);
		nVarLen--;
		szVarValue[nVarLen] = 0;
		EnCryptString(szVarValue);
		strNewAddCode = strSrcCode;
		strNewAddCode += "\r\n";
		for (i=0; i<nVarLen; i++)
		{
			strTemp.Format("\t%s[%d] = L'%c';\r\n",strVarName,i,szVarValue[i]);
			strNewAddCode += strTemp;
		}
		strTemp.Format("\tDeCryptWCHARString(%s);\r\n\r\n",strVarName);
		strNewAddCode += strTemp;
		strNewAddCode += "\t//*&^%$#@!End";
		strReplaceStr = strSrcCode;
		strReplaceStr.Replace(strSrcCode,strNewAddCode);
	}
	else
	{
		strcpy(szVarValue,strVarValue.Right(strVarValue.GetLength()-1));
		nVarLen = strlen(szVarValue);
		nVarLen--;
		szVarValue[nVarLen] = 0;
		EnCryptString(szVarValue);
		strNewAddCode = strSrcCode;
		strNewAddCode += "\r\n";
		for (i=0; i<nVarLen; i++)
		{
			strTemp.Format("\t%s[%d] = '%c';\r\n",strVarName,i,szVarValue[i]);
			strNewAddCode += strTemp;
		}
		strTemp.Format("\tDeCryptString(%s);\r\n\r\n",strVarName);
		strNewAddCode += strTemp;
		strNewAddCode += "\t//*&^%$#@!End";
		strReplaceStr = strSrcCode;
		strReplaceStr.Replace(strSrcCode,strNewAddCode);
	}
	return strReplaceStr;
}

CString CSrcModifyToolDlg::GetStrcpyReplaceCode(CString strSrcCode)
{	
	int		nVarLen;
	int		nStart = 0,nEnd = 0;
	char	szVarValue[0x400] = {0};
	CString strNewAddCode;
	CString strTemp;
	CString strReplaceStr;
	CString strVarName;
	CString strVarValue;
	
	nStart = strSrcCode.Find("(",0);
	nStart++;
	strReplaceStr = strSrcCode;
	nEnd = strReplaceStr.Find(",",nStart);
	strVarName = strReplaceStr.Mid(nStart,nEnd-nStart);
	
	nStart = nEnd;
	nStart++;
	nEnd = strReplaceStr.Find(")",nStart);
	strVarValue = strReplaceStr.Mid(nStart,nEnd-nStart);
	strVarValue.Replace("\\\\","\\");
	
	strcpy(szVarValue,strVarValue.Right(strVarValue.GetLength()-1));
	nVarLen = strlen(szVarValue);
	nVarLen--;
	szVarValue[nVarLen] = 0;
	EnCryptString(szVarValue);
	strNewAddCode = strSrcCode;
	strNewAddCode += "\r\n";

	strTemp.Format("\tstrcpy(%s, \"%s\");\r\n",strVarName,(char*)szVarValue);
	strNewAddCode += strTemp;

	strTemp.Format("\tDeCryptString(%s);\r\n\r\n",strVarName);
	strNewAddCode += strTemp;
	strNewAddCode += "\t//*&^%$#@!End";
	strReplaceStr = strSrcCode;
	strReplaceStr.Replace(strSrcCode,strNewAddCode);
	return strReplaceStr;
}

void CSrcModifyToolDlg::OnCancel() 
{
	
	CDialog::OnCancel();
}
