// UpPcShare.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "UpPcShare.h"
#include "UpPcShareDlg.h"
#include "MyUnZipDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern ULONGLONG m_CmdInfo[5];

/////////////////////////////////////////////////////////////////////////////
// CUpPcShareApp

BEGIN_MESSAGE_MAP(CUpPcShareApp, CWinApp)
	//{{AFX_MSG_MAP(CUpPcShareApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpPcShareApp construction

CUpPcShareApp::CUpPcShareApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CUpPcShareApp object

CUpPcShareApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CUpPcShareApp initialization

BOOL CUpPcShareApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	//赋值启动类型
	if(m_CmdInfo[1] != 0)	//有附加程序
	{
		CMyUnZipDlg dlg;
		m_pMainWnd = &dlg;
		memcpy(dlg.sUserId, &m_CmdInfo[2], 24);
		int nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with OK
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with Cancel
		}
	}
	else
	{
		CUpPcShareDlg dlg;
		m_pMainWnd = &dlg;
		int nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with OK
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with Cancel
		}
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL MakeFilePath(TCHAR* pathName)
{
	TCHAR m_TmpStr[100] = {0};		
	TCHAR *pr = NULL;							
	TCHAR *ps = NULL;
	int  nLenth = 0;
	
	TCHAR m_Path[256] = {0};
	lstrcpy(m_Path, pathName);

	//查看是否为文件
	TCHAR* pFind = StrChr(m_Path, _T('.'));
	if(pFind != NULL)
	{
		pFind = StrRChr(m_Path, NULL, _T('\\'));
		if(pFind != NULL)
		{
			*pFind = 0;
		}
	}

	//目录已经存在
	CFileStatus m_FileStatus;
	if(CFile::GetStatus(m_Path,m_FileStatus))
	{
		return TRUE;
	}

	//初始化指针
	ps = m_Path;
	nLenth = lstrlen(m_Path);
	while(1)
	{
		pr = StrRChr(ps, NULL, _T('\\'));
		if(pr == NULL)
		{
			//只有一级目录
			pr = ps + lstrlen(ps) ;
		}
		int len = pr - m_Path;
		memcpy(m_TmpStr, m_Path, len * sizeof(TCHAR));
		if(len > 3)  
		{
			//建立当前目录
			m_TmpStr[len] = _T('\0');
			CreateDirectory(m_TmpStr, NULL);
		}
		ps = pr+1;
		if(ps - m_Path >= nLenth)
		{
			//目录处理完备
			break;
		}
	}

	//确定建立的目录是否存在
	return CFile::GetStatus(m_Path, m_FileStatus);
}

BOOL ReadMyFile(LPCTSTR m_FileName, LPVOID pData, DWORD nLen)
{
	HANDLE hFile = CreateFile(
			m_FileName,  
			GENERIC_READ,   
			FILE_SHARE_READ, 
			NULL,           
			OPEN_EXISTING,      
			FILE_ATTRIBUTE_NORMAL, 
			NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	DWORD nReadLen = 0;
	ReadFile(hFile, pData, nLen, &nReadLen, NULL);
	CloseHandle(hFile);
	return TRUE;
}

BOOL WriteMyFile(LPCTSTR m_FileName, LPVOID pData, DWORD nLen)
{
	HANDLE hFile = CreateFile(
		m_FileName,  
		GENERIC_WRITE,   
		0, 
		NULL,           
		CREATE_NEW,      
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	DWORD nWriteLen = 0;
	WriteFile(hFile, pData, nLen, &nWriteLen, NULL);
	CloseHandle(hFile);
	return TRUE;
}

DWORD GetMySerialNumber()
{
	/*Kernel32.dll*/
	char m_TmpStr[256] = {0};
	m_TmpStr[0] = (char) 0x4b;
	m_TmpStr[1] = (char) 0x65;
	m_TmpStr[2] = (char) 0x72;
	m_TmpStr[3] = (char) 0x6e;
	m_TmpStr[4] = (char) 0x65;
	m_TmpStr[5] = (char) 0x6c;
	m_TmpStr[6] = (char) 0x33;
	m_TmpStr[7] = (char) 0x32;
	m_TmpStr[8] = (char) 0x2e;
	m_TmpStr[9] = (char) 0x64;
	m_TmpStr[10] = (char) 0x6c;
	m_TmpStr[11] = (char) 0x6c;
	m_TmpStr[12] = 0x00;

	HMODULE hModule_Kernel32 = LoadLibraryA(m_TmpStr);

	/*GetVolumeInformationA*/
	m_TmpStr[0] = (char) 0x47;
	m_TmpStr[1] = (char) 0x65;
	m_TmpStr[2] = (char) 0x74;
	m_TmpStr[3] = (char) 0x56;
	m_TmpStr[4] = (char) 0x6f;
	m_TmpStr[5] = (char) 0x6c;
	m_TmpStr[6] = (char) 0x75;
	m_TmpStr[7] = (char) 0x6d;
	m_TmpStr[8] = (char) 0x65;
	m_TmpStr[9] = (char) 0x49;
	m_TmpStr[10] = (char) 0x6e;
	m_TmpStr[11] = (char) 0x66;
	m_TmpStr[12] = (char) 0x6f;
	m_TmpStr[13] = (char) 0x72;
	m_TmpStr[14] = (char) 0x6d;
	m_TmpStr[15] = (char) 0x61;
	m_TmpStr[16] = (char) 0x74;
	m_TmpStr[17] = (char) 0x69;
	m_TmpStr[18] = (char) 0x6f;
	m_TmpStr[19] = (char) 0x6e;
	m_TmpStr[20] = (char) 0x41;
	m_TmpStr[21] = 0x00;

	MyGetVolumeInformation P_GetVolumeInformation =  (MyGetVolumeInformation) GetProcAddress(hModule_Kernel32, m_TmpStr);

	/*c:\\*/
	m_TmpStr[0] = (char) 0x63;
	m_TmpStr[1] = (char) 0x3a;
	m_TmpStr[2] = (char) 0x5c;
	m_TmpStr[3] = 0x00;

	char FileSysNameBuf[1024] = {0};
	DWORD dwSysFlags = 0;
	DWORD nNum =0;
	if(!P_GetVolumeInformation(m_TmpStr, NULL, 0, &nNum, NULL, &dwSysFlags, FileSysNameBuf, 1024))
	{
		FreeLibrary(hModule_Kernel32);
		return 0;
	}
	FreeLibrary(hModule_Kernel32);
	return nNum;
}