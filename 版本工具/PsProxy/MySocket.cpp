// MySocket.cpp: implementation of the CMySocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PsProxy.h"
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

BOOL CMySocket::StartWork(LPCTSTR pRemoteAddr, LPCTSTR pRemotePort, LPCTSTR pLocalPort)
{
	lstrcpy(m_SocketInfo.m_RemoteAddr, pRemoteAddr);
	m_SocketInfo.m_LocalPort = StrToInt(pLocalPort);
	m_SocketInfo.m_RemoteProt = StrToInt(pRemotePort);

	SOCKET		Sck = NULL;	
	sockaddr_in addr = {0};
	int optval = 1000000;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons((WORD) m_SocketInfo.m_LocalPort);

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
	m_Socket = Sck;


	//启动线程序
	hListenThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DoListenThread, this, 0, NULL);
	return TRUE;
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
	while(m_SocketInfo.m_LocalPort != 0)
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
	SOCKET sAccept = (SOCKET) lPvoid;
	SOCKET sConnect = GetConnectSocket(m_SocketInfo.m_RemoteAddr, m_SocketInfo.m_RemoteProt);
	if(sConnect == NULL)
	{
		closesocket(sAccept);
		return -1;
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

void CMySocket::Close()
{
	if(m_Socket != NULL)
	{
		closesocket(m_Socket);
		m_Socket = NULL;
		m_SocketInfo.m_LocalPort = 0;
	}
}