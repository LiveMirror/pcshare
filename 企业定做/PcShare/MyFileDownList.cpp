// MyFileDownList.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyFileDownList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFileDownList

IMPLEMENT_DYNCREATE(CMyFileDownList, CXTListView)

CMyFileDownList::CMyFileDownList()
{
	memset(m_SaveListFileName, 0, 256);
	memset(m_Id, 0, 256);
	memset(m_Title, 0, 256);
	bIsUserCancel = FALSE;
	hDlFilethread = NULL;
	m_IsUserExit = FALSE;
	m_IsWork = FALSE;
	hFileSaveList = NULL;

	hMetux = CreateMutex(NULL, FALSE, NULL);
}

CMyFileDownList::~CMyFileDownList()
{
	if(hFileSaveList != NULL)
	{
		CloseHandle(hFileSaveList);
	}

	CloseHandle(hMetux);
}

BEGIN_MESSAGE_MAP(CMyFileDownList, CXTListView)
	//{{AFX_MSG_MAP(CMyFileDownList)
	ON_WM_CREATE()
	ON_WM_COPYDATA()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_FILE_TRANS_START, OnFileTransStart)
	ON_UPDATE_COMMAND_UI(ID_FILE_TRANS_START, OnUpdateFileTransStart)
	ON_COMMAND(ID_FILE_TRANS_STOP, OnFileTransStop)
	ON_UPDATE_COMMAND_UI(ID_FILE_TRANS_STOP, OnUpdateFileTransStop)
	ON_UPDATE_COMMAND_UI(ID_FILE_TRANS_DELETE_ALL, OnUpdateFileTransDeleteAll)
	ON_COMMAND(ID_FILE_TRANS_DELETE_ALL, OnFileTransDeleteAll)
	ON_COMMAND(ID_FILE_TRANS_DELETE, OnFileTransDelete)
	ON_UPDATE_COMMAND_UI(ID_FILE_TRANS_DELETE, OnUpdateFileTransDelete)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RESIZE_WND, OnResizeWnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFileDownList drawing

void CMyFileDownList::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMyFileDownList diagnostics

#ifdef _DEBUG
void CMyFileDownList::AssertValid() const
{
	CXTListView::AssertValid();
}

void CMyFileDownList::Dump(CDumpContext& dc) const
{
	CXTListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyFileDownList message handlers

BOOL CMyFileDownList::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style ^= LVS_REPORT;
	cs.style &= ~WS_BORDER;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return CXTListView::PreCreateWindow(cs);
}

int CMyFileDownList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_imagelist.Create(16, 16, ILC_COLOR8 | ILC_MASK, 1, 1);
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_TRAN));
	GetListCtrl().SetImageList(&m_imagelist, LVSIL_SMALL);
	GetListCtrl().InsertColumn(0, _T("文件名"), LVCFMT_LEFT, 360);
	GetListCtrl().InsertColumn(1, _T("文件大小"), LVCFMT_RIGHT, 120);
	GetListCtrl().InsertColumn(2, _T("进度"), LVCFMT_RIGHT, 100);
	GetListCtrl().InsertColumn(3, _T("状态"), LVCFMT_RIGHT, 60);
	GetListCtrl().InsertColumn(4, _T("速度"), LVCFMT_RIGHT, 80);
	GetListCtrl().InsertColumn(5, _T("剩余时间"), LVCFMT_RIGHT, 120);
	GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT);

	SubclassHeader();
	m_flatHeader.ThawAllColumns();
	m_flatHeader.ShowSortArrow(TRUE);
	OnResizeWnd(0, 0);
	return 0;
}

void CMyFileDownList::DownLoadFileThread(LPVOID lPvoid)
{
	SetThreadAffinityMask(GetCurrentThread(), 0x0E);
	CMyFileDownList* pThis = (CMyFileDownList*) lPvoid;
	pThis->DownLoadFile();
}

BOOL CMyFileDownList::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	LPTCPCONNECTINFO pInfo = (LPTCPCONNECTINFO) pCopyDataStruct->lpData;
	if(pCopyDataStruct->dwData == PS_SOCKET_CONNECT)
	{
		if(m_Tran.DuplicateHttpSocket(pInfo))
		{
			hDlFilethread = (HANDLE) _beginthread(DownLoadFileThread, 0, this);
		}
	}	
	return TRUE;
}

TCHAR* CMyFileDownList::GetId()
{
	return m_Id;
}

TCHAR* CMyFileDownList::GetTitle()
{
	return m_Title;
}

void CMyFileDownList::AddFileDownList(LPMYINSERTDLFILEINFO pDlFileInfo, LPVOID pData, DWORD nLen)
{
	if(nLen == 0)
	{
		return;
	}

	LPCPFILEINFO pFileInfo = (LPCPFILEINFO) pData;
	for(DWORD j = 0; j < nLen / sizeof(CPFILEINFO); j++)
	{
		//文件名称
		TCHAR* pFind = StrRChr(pFileInfo->m_FileName, NULL, _T('\\'));
		if(pFind == NULL)
		{
			continue;
		}
		pFind ++;

		LPINTERFILEINFO pSaveFileInfo = new INTERFILEINFO;
		ZeroMemory(pSaveFileInfo, sizeof(INTERFILEINFO));

		int nItem = GetListCtrl().InsertItem(
				LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT, 
				GetListCtrl().GetItemCount(), pFind,
				INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK, 0, (LPARAM) pSaveFileInfo);

		TCHAR m_TmpText[256] = {0};
		StrFormatByteSize64(pFileInfo->m_FileLen.QuadPart, m_TmpText, 256);
		GetListCtrl().SetItemText(nItem, 1, m_TmpText);

		GetListCtrl().SetItemText(nItem, 3, _T("等待下载"));

		//本地文件名
		wsprintf(pSaveFileInfo->m_LocalFile, _T("%s%s"), pDlFileInfo->m_LocalPath, 
			pFileInfo->m_FileName + lstrlen(pDlFileInfo->m_RemotePath));
		pSaveFileInfo->m_FileLen = pFileInfo->m_FileLen;
		lstrcpy(pSaveFileInfo->m_RemoteFile, pFileInfo->m_FileName);

		pFileInfo ++;
	}

	SaveFileList();

	if(bIsUserCancel)
	{
		return;
	}

	if(m_IsWork)
	{
		return;
	}

	PostDlCmdToClient();
}

void CMyFileDownList::SaveFileList()
{
	WaitForSingleObject(hMetux, 0xffffffff);
	DeleteFile(m_SaveListFileName);

	//保存当前下载列表
	hFileSaveList = CreateFile(m_SaveListFileName, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFileSaveList != INVALID_HANDLE_VALUE)
	{
		//备份当前VIEW名称
		DWORD nRecvLen = 0;
		WriteFile(hFileSaveList, m_Title, 256 * sizeof(TCHAR), &nRecvLen, NULL);

		//保存下载列表
		for(int i = 0; i < GetListCtrl().GetItemCount(); i++)
		{
			LPINTERFILEINFO pTran = (LPINTERFILEINFO) GetListCtrl().GetItemData(i);
			if(pTran != NULL)
			{
				WriteFile(hFileSaveList, pTran, sizeof(INTERFILEINFO), &nRecvLen, NULL);
			}
		}
		CloseHandle(hFileSaveList);
	}
	hFileSaveList = NULL;
	ReleaseMutex(hMetux);
}

void CMyFileDownList::PostDlCmdToClient()
{
	if(m_IsWork)
	{
		return;
	}

	MYCHILDCMDINFO m_Info = {0};
	lstrcpy(m_Info.m_Id, m_Id);
	TCHAR nCWndName[256] = {0};
	wsprintf(nCWndName, _T("%s连接管理"), PS_TITLE);
	HWND hWnd = ::FindWindow(NULL, nCWndName);
	if(hWnd == NULL)
	{
		return;
	}
	
	HWND hMainListWnd = GetMainListWnd();
	
	m_Info.m_Info.m_Command = WM_CONNECT_DL_FILE;

	//取当前窗口
	m_Info.m_Info.m_Hwnd = GetSafeHwnd();
	
	//填充结构
	COPYDATASTRUCT ct = {0};
	ct.lpData = &m_Info;
	ct.cbData = sizeof(MYCHILDCMDINFO);
	ct.dwData = PS_LOCAL_FILETRAN;
	::SendMessage(hMainListWnd, WM_COPYDATA, 0, (LPARAM) &ct);
}

void CMyFileDownList::OnDestroy() 
{
	m_IsUserExit = TRUE;
	m_Tran.Close();
	if(WaitForSingleObject(hDlFilethread, 1000) != WAIT_OBJECT_0)
	{
		TerminateThread(hDlFilethread, 0);
	}

	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		LPINTERFILEINFO pTran = (LPINTERFILEINFO) GetListCtrl().GetItemData(i);
		delete pTran;
	}
	CXTListView::OnDestroy();
}

void CMyFileDownList::InsertLogs(LPCTSTR pText)
{
	//查找主窗口
	TCHAR nCWndName[256] = {0};
	wsprintf(nCWndName, _T("%s下载管理"), PS_TITLE);
	HWND hWnd = ::FindWindow(NULL, nCWndName);
	if(hWnd == NULL)
	{
		return ;
	}
	::SendMessage(hWnd, WM_INSERTLOGS, (WPARAM) pText, (LPARAM) m_Title);
}

/*
__int64 GetMyTime()
{
	static __int64 start = 0;
	static __int64 frequency = 0;

	if (start==0)
	{
	   QueryPerformanceCounter((LARGE_INTEGER*) &start);
	   QueryPerformanceFrequency((LARGE_INTEGER*) &frequency);
	   return 0;
	}

	__int64 counter = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&counter);
	return ((counter - start) / (frequency));
}
*/

void CMyFileDownList::ReNameFileName(TCHAR* pFileName, BOOL bIsBakFileName)
{
	if(bIsBakFileName)
	{
		RemoveDirectory(pFileName);
		lstrcat(pFileName, _T(".bak"));
	}
	else
	{
		//更名
		TCHAR m_NewName[256] = {0};
		lstrcpy(m_NewName, pFileName);
		m_NewName[lstrlen(m_NewName) - 4] = 0x00;
		MoveFileEx(pFileName, m_NewName, MOVEFILE_COPY_ALLOWED);
	}
}

void CMyFileDownList::DownLoadFile()
{
	TCHAR m_LogsText[256] = {0};
	m_IsWork = TRUE;
	while(GetListCtrl().GetItemCount() > 0)
	{
		LPINTERFILEINFO pTran = (LPINTERFILEINFO) GetListCtrl().GetItemData(0);
		ASSERT(pTran);

		INTERFILEINFO m_FileInfo = {0};
		memcpy(&m_FileInfo, pTran, sizeof(INTERFILEINFO));

		GetListCtrl().SetItemText(0, 3, _T("正在下载"));

		BOOL m_IsReName = FALSE;
		if(m_FileInfo.m_LocalFile[lstrlen(m_FileInfo.m_LocalFile) - 4] != _T('.'))
		{
			m_IsReName = TRUE;
			ReNameFileName(m_FileInfo.m_LocalFile, TRUE);
		}

		//创建目录
		MakeFilePath(m_FileInfo.m_LocalFile);
		LARGE_INTEGER nTransLen = {0};

		HANDLE hFile = CreateFile(
					m_FileInfo.m_LocalFile,  
					GENERIC_WRITE,   
					0, 
					NULL,           
					OPEN_ALWAYS,      
					FILE_ATTRIBUTE_NORMAL, 
					NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			wsprintf(m_LogsText, _T("无法打开本地文件[%s]，当前任务放弃"), pTran->m_LocalFile);
			InsertLogs(m_LogsText);

			delete pTran;
			GetListCtrl().DeleteItem(0);
			SaveFileList();
			continue;
		}

		//取当前已下载文件长度
		GetFileSizeEx(hFile, &m_FileInfo.m_FilePoint);
		
		ASSERT(pTran->m_FileLen.QuadPart != 0);

	#ifdef UNICODE
		m_Tran.Convert2ClientANSI(m_FileInfo.m_LocalFile, wcslen(m_FileInfo.m_LocalFile));
		m_Tran.Convert2ClientANSI(m_FileInfo.m_RemoteFile, wcslen(m_FileInfo.m_RemoteFile));
	#endif

		if(!m_Tran.TcpSendData(&m_FileInfo, sizeof(m_FileInfo)) || !m_Tran.TcpRecvData(&m_FileInfo, sizeof(m_FileInfo)))
		{
			CloseHandle(hFile);
			m_IsWork = FALSE;
			if(!m_IsUserExit)
			{
				if(!bIsUserCancel)
				{
					InsertLogs(_T("连接已经异常中断！"));
					GetListCtrl().SetItemText(0, 3, _T("连接中断"));
					PostMessage(WM_COMMAND, (WPARAM) ID_FILE_TRANS_START, 0);
				}
				else
				{
					InsertLogs(_T("用户中止下载"));
					GetListCtrl().SetItemText(0, 3, _T("用户取消"));
				}
			}
			return;
		}

	// 为文件传输做的Ansi到Unicode的转换 [9/19/2007 zhaiyinwei]
	#ifdef UNICODE
		m_Tran.Convert2Unicode((char*) m_FileInfo.m_LocalFile, strlen((char*) m_FileInfo.m_LocalFile));
		m_Tran.Convert2Unicode((char*) m_FileInfo.m_RemoteFile, strlen((char*) m_FileInfo.m_RemoteFile));
	#endif

		//查看返回码
		if(m_FileInfo.m_Response != 0)
		{
			switch(m_FileInfo.m_Response)
			{
				case DL_FILE_LOSE : 
					{
						wsprintf(m_LogsText, _T("无法打开远程文件[%s]，当前任务放弃"), pTran->m_RemoteFile);
						InsertLogs(m_LogsText);
					}
					break;
				case DL_FILE_LEN_ERR :
					{
						wsprintf(m_LogsText, _T("远程文件长度异常[%s]，当前任务放弃"), pTran->m_RemoteFile);
						InsertLogs(m_LogsText);
					}
					break;
				default :
					{
						InsertLogs(_T("未知错误，，当前任务放弃"));
					}
					break;
			}

			CloseHandle(hFile);
			delete pTran;
			GetListCtrl().DeleteItem(0);
			SaveFileList();
			continue;
		}

		nTransLen.QuadPart = m_FileInfo.m_FileLen.QuadPart - m_FileInfo.m_FilePoint.QuadPart;
		if(nTransLen.QuadPart == 0)
		{
			//无须下载
			CloseHandle(hFile);
			delete pTran;
			GetListCtrl().DeleteItem(0);
			SaveFileList();
			if(m_IsReName)
			{
				ReNameFileName(m_FileInfo.m_LocalFile, FALSE);
			}
			continue;
		}

		char m_Text[256] = {0};
		char m_OldText[256] = {0};

		//显示当前文件进度
		double c = 1.0 * pTran->m_FilePoint.QuadPart / pTran->m_FileLen.QuadPart * 100;
		sprintf(m_Text, "%3.1f", c);
		strcat(m_Text, "%");
		if(strcmp(m_Text, m_OldText) != 0)
		{
			strcpy(m_OldText, m_Text);
			MultiByteToWideChar(CP_ACP, 0, m_Text, strlen(m_Text) + 1, m_LogsText, 256);
			GetListCtrl().SetItemText(0, 2, m_LogsText);
		}

		//移动到末尾
		LARGE_INTEGER nPos;
		nPos.QuadPart = 0;
		SetFilePointerEx(hFile, nPos, NULL, FILE_END);

		//接收文件实体
		DWORD len = 0;
		BYTE pFileBuf[65535];
		DWORD nRecvLen = 0;
		while(nTransLen.QuadPart > 0)
		{
//			__int64 nCount_Start = GetMyTime();

			len = DWORD (nTransLen.QuadPart > 65535 ? 65535 : nTransLen.QuadPart);
			if(!m_Tran.TcpRecvData(pFileBuf, len))
			{
				CloseHandle(hFile);
				m_IsWork = FALSE;
				if(!m_IsUserExit)
				{
					m_Tran.Close();
					if(!bIsUserCancel)
					{
						InsertLogs(_T("连接已经异常中断！"));
						GetListCtrl().SetItemText(0, 3, _T("连接中断"));
						PostMessage(WM_COMMAND, (WPARAM) ID_FILE_TRANS_START, 0);
					}
					else
					{
						InsertLogs(_T("用户中止下载"));
						GetListCtrl().SetItemText(0, 3, _T("用户取消"));
					}
				}
				return;
			}

/*			__int64 nCount_End = GetMyTime();

			ASSERT(nCount_End - nCount_Start != 0);

			StrFormatByteSize(len / (nCount_End - nCount_Start), m_LogsText, 256);
			lstrcat(m_LogsText, _T("/秒"));
			GetListCtrl().SetItemText(0, 4, m_LogsText);
*/			
			WriteFile(hFile, pFileBuf, len, &nRecvLen, NULL);
			nTransLen.QuadPart -= len;
			pTran->m_FilePoint.QuadPart += len;

			ASSERT(pTran->m_FileLen.QuadPart != 0);

			//显示当前文件进度
			double c = 1.0 * pTran->m_FilePoint.QuadPart / pTran->m_FileLen.QuadPart * 100;
			sprintf(m_Text, "%3.1f", c);
			strcat(m_Text, "%");
			if(strcmp(m_Text, m_OldText) != 0)
			{
				strcpy(m_OldText, m_Text);
				MultiByteToWideChar(CP_ACP, 0, m_Text, strlen(m_Text) + 1, m_LogsText, 256);
				GetListCtrl().SetItemText(0, 2, m_LogsText);
			}
		}

		//当前文件传输完成
		CloseHandle(hFile);
		delete pTran;
		GetListCtrl().DeleteItem(0);

		if(m_IsReName)
		{
			ReNameFileName(m_FileInfo.m_LocalFile, FALSE);
		}

		//保存当前下载列表
		SaveFileList();
	}

	DeleteFile(m_SaveListFileName);

	m_Tran.Close();
	m_IsWork = FALSE;

	//通知父窗口删除当前视
	GetParent()->PostMessage(WM_DL_DELETE_VIEW, (WPARAM) this, NULL);
}

void CMyFileDownList::OnSize(UINT nType, int cx, int cy) 
{
	CXTListView::OnSize(nType, cx, cy);
	OnResizeWnd(0, 0);	
}

LRESULT CMyFileDownList::OnResizeWnd(WPARAM wParam, LPARAM lParam)
{
	CRect rect;
	GetClientRect(&rect);
	if(rect.Width() > 0)
	{
		GetListCtrl().SetColumnWidth(0, 360);
		GetListCtrl().SetColumnWidth(1, 120);
		GetListCtrl().SetColumnWidth(2, 100);
		GetListCtrl().SetColumnWidth(3, 60);
		GetListCtrl().SetColumnWidth(4, 80);
		GetListCtrl().SetColumnWidth(5, 120);
	}
	return TRUE;
}

void CMyFileDownList::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CPoint m_point;
	CMenu m_menu;
	m_menu.LoadMenu(IDR_MENU_TRAN);
	GetCursorPos(&m_point);
	SetForegroundWindow();   
	CXTPCommandBars::TrackPopupMenu(m_menu.GetSubMenu(0), TPM_RIGHTBUTTON, m_point.x, m_point.y, this);
	m_menu.DestroyMenu();
}

void CMyFileDownList::InitData(LPCTSTR pId, LPCTSTR pTitle, LPCTSTR pSaveFileName)
{
	lstrcpy(m_Id, pId);
	lstrcpy(m_Title, pTitle);

	lstrcpy(m_SaveListFileName, pSaveFileName);

	//取下载列表
	HANDLE hFile = CreateFile(m_SaveListFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	DWORD nFileLen = GetFileSize(hFile, NULL);
	if(nFileLen < 256 * sizeof(TCHAR) + sizeof(INTERFILEINFO))
	{
		CloseHandle(hFile);
		return;
	}
	
	SetFilePointer(hFile, 256 * sizeof(TCHAR), NULL, FILE_BEGIN);
	nFileLen -= 256 * sizeof(TCHAR);

	for(DWORD i = 0; i < nFileLen / sizeof(INTERFILEINFO); i++)
	{
		LPINTERFILEINFO pSaveFileInfo = new INTERFILEINFO;
	
		DWORD nRecvLen = 0;
		ReadFile(hFile, pSaveFileInfo, sizeof(INTERFILEINFO), &nRecvLen, NULL);

		//文件名称
		TCHAR* pFind = StrRChr(pSaveFileInfo->m_LocalFile, NULL, _T('\\'));
		if(pFind == NULL)
		{
			continue;
		}
		pFind ++;

		int nItem = GetListCtrl().InsertItem(
			LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT, GetListCtrl().GetItemCount(), pFind,
			INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK, 0, (LPARAM) pSaveFileInfo);

		//文件长度
		TCHAR m_TmpText[256] = {0};
		StrFormatByteSize64(pSaveFileInfo->m_FileLen.QuadPart, m_TmpText, 256);
		GetListCtrl().SetItemText(nItem, 1, m_TmpText);

		//显示当前文件进度
		WIN32_FILE_ATTRIBUTE_DATA m_FileData = {0};
		GetFileAttributesEx(pSaveFileInfo->m_LocalFile, GetFileExInfoStandard, &m_FileData);
		pSaveFileInfo->m_FilePoint.LowPart = m_FileData.nFileSizeLow;
		pSaveFileInfo->m_FilePoint.HighPart = m_FileData.nFileSizeHigh;
			
		TCHAR m_LogsText[256] = {0};
		char m_Text[256] = {0};
		double c = 1.0 * pSaveFileInfo->m_FilePoint.QuadPart / pSaveFileInfo->m_FileLen.QuadPart * 100;
		sprintf(m_Text, "%3.1f", c);
		strcat(m_Text, "%");
		if(strcmp(m_Text, "0.0%") != 0)
		{
			MultiByteToWideChar(CP_ACP, 0, m_Text, strlen(m_Text) + 1, m_LogsText, 256);
			GetListCtrl().SetItemText(nItem, 2, m_LogsText);
		}
		GetListCtrl().SetItemText(nItem, 3, _T("等待下载"));
	}
	CloseHandle(hFile);
}

void CMyFileDownList::OnFileTransStart() 
{
	bIsUserCancel = FALSE;
	PostDlCmdToClient();
}

void CMyFileDownList::OnUpdateFileTransStart(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_IsWork);
}

void CMyFileDownList::OnFileTransStop() 
{
	bIsUserCancel = TRUE;
	m_Tran.Close();
}

void CMyFileDownList::OnUpdateFileTransStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_IsWork);	
}

void CMyFileDownList::OnUpdateFileTransDeleteAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_IsWork);	
}

void CMyFileDownList::OnFileTransDeleteAll() 
{
	int iCount = GetListCtrl().GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		LPINTERFILEINFO pTran = (LPINTERFILEINFO) GetListCtrl().GetItemData(i);
		delete pTran;
	}
	GetListCtrl().DeleteAllItems();
	DeleteFile(m_SaveListFileName);

	//通知父窗口删除当前视
	GetParent()->PostMessage(WM_DL_DELETE_VIEW, (WPARAM) this, NULL);
}

void CMyFileDownList::OnFileTransDelete() 
{
	int m_Count = GetListCtrl().GetSelectedCount();
	for(int i = 0; i < m_Count; i++)
	{
		int nItem = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
		LPINTERFILEINFO pTran = (LPINTERFILEINFO) GetListCtrl().GetItemData(nItem);
		delete pTran;
		GetListCtrl().DeleteItem(nItem);
	}

	if(GetListCtrl().GetItemCount() > 0)
	{
		SaveFileList();
	}
	else
	{
		DeleteFile(m_SaveListFileName);
		
		//通知父窗口删除当前视
		GetParent()->PostMessage(WM_DL_DELETE_VIEW, (WPARAM) this, NULL);
	}
}

void CMyFileDownList::OnUpdateFileTransDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetListCtrl().GetSelectedCount() > 0	&& !m_IsWork);
}
