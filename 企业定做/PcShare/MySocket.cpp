// MySocket.cpp: implementation of the CMySocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MySocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SOCKETINFO m_SocketInfo = {0};

CMySocket::CMySocket()
{
	bIsExit = FALSE;
}

CMySocket::~CMySocket()
{
	
}

SOCKET CMySocket::GetConnectSocket(char* pServerAddr, UINT nServerPort)
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

SOCKET CMySocket::GetListenSocket(WORD wPort)
{
	SOCKET		Sck = NULL;	
	sockaddr_in addr = {0};
	int optval = 1000000;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(wPort);

	Sck = socket(AF_INET, SOCK_STREAM, 0);
	if(Sck==INVALID_SOCKET)
	{
		return FALSE;
	}

	if(bind(Sck, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		closesocket(Sck);
		return FALSE;
	}

	if(setsockopt(Sck, SOL_SOCKET, SO_SNDTIMEO, (char*) &optval, sizeof(optval)) == SOCKET_ERROR)
	{
		closesocket(Sck);
		return FALSE;
	}

	if(setsockopt(Sck, SOL_SOCKET, SO_RCVTIMEO, (char*) &optval, sizeof(optval))	== SOCKET_ERROR)
	{
		closesocket(Sck);
		return FALSE;
	}

	if(listen(Sck, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(Sck);
		return FALSE;
	}
	return Sck;
}

BOOL CMySocket::StartWork(SOCKET sCmdSocket)
{
	//填充数据
	m_SocketInfo.m_CmdSocket = sCmdSocket;

	strcpy(m_SocketInfo.m_LocalAddr, "127.0.0.1");

	TCHAR m_sPort[100] = {0};
	GetPrivateProfileString(PS_SHEZHI, _T("上线连接端口"), _T("80"), m_sPort, 99, GetIniFileName());
	m_SocketInfo.m_LocalPort = (WORD) StrToInt(m_sPort);

	TCHAR m_sServerAddr[100] = {0};
	GetPrivateProfileString(PS_SHEZHI, _T("中转服务器地址"), _T(""), m_sServerAddr, 99, GetIniFileName());
	WideCharToMultiByte(CP_ACP, 0,	m_sServerAddr, lstrlen(m_sServerAddr) + 1, m_SocketInfo.m_ServerAddr, 256, NULL, NULL);

	GetPrivateProfileString(PS_SHEZHI, _T("中转服务器端口"), _T("30000"), m_sPort, 99, GetIniFileName());
	m_SocketInfo.m_ServerPort = (WORD) StrToInt(m_sPort);

	//启动线程序
	hListenThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DoListenThread, this, 0, NULL);
	return TRUE;
}

void CMySocket::DoListen()
{
	while(1)
	{
		PROXYLOGININFO m_Info = {0};
		int ret = recv(m_SocketInfo.m_CmdSocket, (char*) &m_Info, sizeof(PROXYLOGININFO), 0);
		if(ret == SOCKET_ERROR || ret == 0)
		{
			if(!bIsExit)
			{
				m_SocketInfo.m_CmdSocket = GetConnectSocket(m_SocketInfo.m_ServerAddr, m_SocketInfo.m_ServerPort);
				if(m_SocketInfo.m_CmdSocket == NULL)
				{
					MessageBox(NULL, _T("中转服务器已经断线！"), _T("警告"), MB_OK);
					break;
				}
				else
				{
					TCHAR m_sPass[100] = {0};
					GetPrivateProfileString(PS_SHEZHI, _T("中转服务器密码"), _T(""), m_sPass, 99, GetIniFileName());
					PROXYLOGININFO m_Info = {0};
					m_Info.m_Cmd = P_CONNECT_CMD;
					m_Info.m_Data = StrToInt(m_sPass);
					send(m_SocketInfo.m_CmdSocket, (char*) &m_Info, sizeof(PROXYLOGININFO), 0);
					continue;
				}
			}
			else
			{
				break;
			}
		}
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DoChildWorkThread, (LPVOID) m_Info.m_Data, 0, NULL);
	}
}

DWORD CMySocket::DoListenThread(LPVOID lPvoid)
{
	CMySocket* pThis = (CMySocket*) lPvoid;
	pThis->DoListen();
	return 0;
}

DWORD CMySocket::DoSendRecvThread(LPVOID lPvoid)
{
	SENDRECVINFO m_Info = {0};
	memcpy(&m_Info, lPvoid, sizeof(SENDRECVINFO));
	delete (LPSENDRECVINFO) lPvoid;

	struct fd_set sset = {0};
	struct timeval waited = {0};
	waited.tv_sec = 300000;
	waited.tv_usec= 0;
	int ret = 0;
	BYTE* m_Buf = new BYTE[1048576];
	while(1)
	{
		//等待服务器数据
		FD_ZERO(&sset);
		FD_SET(m_Info.sRecv, &sset);
		if(select(m_Info.sRecv + 1, &sset, NULL, NULL, &waited) <= 0)
		{
			break;
		}

		//接收服务器数据
		ret = recv(m_Info.sRecv, (char*) m_Buf, 1048575, 0);
		if(ret <= 0) 
		{
			break;
		}

		send(m_Info.sSend, (char*) m_Buf, ret, 0);
	}
	delete [] m_Buf;
	closesocket(m_Info.sRecv);
	closesocket(m_Info.sSend);
	return 0;
}

DWORD CMySocket::DoChildWorkThread(LPVOID lPvoid)
{
	DWORD dThreadId = (DWORD) lPvoid;

	//连接到中转服务器
	SOCKET sServer = GetConnectSocket(m_SocketInfo.m_ServerAddr, m_SocketInfo.m_ServerPort);
	if(sServer == NULL)
	{
		return 0;
	}
	
	//发送标志
	PROXYLOGININFO m_Info = {0};
	m_Info.m_Data = (DWORD) lPvoid;
	m_Info.m_Cmd = P_CONNECT_DATA;
	if(send(sServer, (char*) &m_Info, sizeof(PROXYLOGININFO), 0) == SOCKET_ERROR)
	{
		closesocket(sServer);
		return 0;
	}

	//接受标识
	char m_Title[256] = {0};
	if(recv(sServer, m_Title, 22, 0) == SOCKET_ERROR)
	{
		closesocket(sServer);
		return 0;
	}

	//连接到本地
	SOCKET sLocal = GetConnectSocket(m_SocketInfo.m_LocalAddr, m_SocketInfo.m_LocalPort);
	ASSERT(sLocal != NULL);
	
	//发送标识
	send(sLocal, m_Title, 22, 0);

	//创建缓冲
	LPSENDRECVINFO pInfo = new SENDRECVINFO;
	pInfo->sSend = sServer;
	pInfo->sRecv = sLocal;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DoSendRecvThread, (LPVOID) pInfo, 0, NULL);

	pInfo = new SENDRECVINFO;
	pInfo->sSend = sLocal;
	pInfo->sRecv = sServer;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DoSendRecvThread, (LPVOID) pInfo, 0, NULL);
	return 0;	
}

void CMySocket::Close()
{
	bIsExit = TRUE;
}