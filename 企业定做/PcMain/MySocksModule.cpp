// MySocksModule.cpp: implementation of the CMySocksModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MySocksModule.h"

MYSOCKSINFO m_gSocksInfo = {0};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMySocksModule::CMySocksModule()
{
	m_MainSocket = NULL;
}

CMySocksModule::~CMySocksModule()
{

}

SOCKET CMySocksModule::GetConnectSocket(LPCTSTR pServerAddr, UINT nServerPort)
{
	struct sockaddr_in	m_ServerAddr = {0}; 
	int			m_AddrLen = sizeof(struct sockaddr_in);
	SOCKET		m_Socket = 0;

	char m_sServerIp[MAX_PATH] = {0};
	m_gFunc.strcpy(m_sServerIp, pServerAddr);
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
		m_gFunc.strcpy(m_sServerIp, inet_ntoa(dest.sin_addr));
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
	if(connect(m_Socket, (sockaddr*) &m_ServerAddr, m_AddrLen) == SOCKET_ERROR) 
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

SOCKET CMySocksModule::GetTcpListenSocket(WORD wPort)
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

DWORD CMySocksModule::SSH_ProxyMainSocksThread(LPVOID lPvoid)
{
	DWORD	m_ProxyTypes = 0;
	char	m_ServerAddr[MAX_PATH] = {0};
	WORD	m_ServerPort = {0};
	BOOL	m_IsUdp = FALSE;
	SOCKET	m_ClientSocket = (SOCKET) lPvoid;
	SOCKET	m_LocalSocket = NULL;

	//协商包头
	char* m_DataBuf = new char[65535];
	int ret = recv(m_ClientSocket, m_DataBuf, 2, 0);
	if(ret != 2)
	{
		delete [] m_DataBuf;
		closesocket(m_ClientSocket);
		return 0;
	}
	
	if(m_DataBuf[0] == 0x04 && m_DataBuf[1] == 0x01)
	{
		//SOCKS4
		m_ProxyTypes = PS_PROXY_SOCKS4;
		ret = recv(m_ClientSocket, m_DataBuf + 2, 255, 0);
		if(ret <= 0)
		{
			delete [] m_DataBuf;
			closesocket(m_ClientSocket);
			return 0;
		}

		if(m_DataBuf[4] == 0 && m_DataBuf[5] == 0 && m_DataBuf[6] == 0 && m_DataBuf[7] != 0)
		{
			//域名
			char* pFind = &m_DataBuf[7];
			int nLen = m_gFunc.strlen(pFind);
			pFind += nLen + 1;
			m_gFunc.strcpy(m_ServerAddr, pFind); 
		}
		else
		{
			//IP
			struct in_addr addr = {0}; 
			CopyMemory(&addr.s_addr, m_DataBuf + 4, 4); 
			m_gFunc.strcpy(m_ServerAddr, inet_ntoa(addr)); 
		}
		m_ServerPort = ntohs(*(unsigned short*)(m_DataBuf + 2)); 
	}
	else if(m_DataBuf[0] == 0x05)
	{
		//SOCKS5
		m_ProxyTypes = PS_PROXY_SOCKS5;

		//接受方法长度,过滤掉
		ret = recv(m_ClientSocket, m_DataBuf, m_DataBuf[1], 0);
		if(ret != m_DataBuf[1])
		{
			delete [] m_DataBuf;
			closesocket(m_ClientSocket);
			return 0;
		}

		if(strlen(m_gSocksInfo.m_User) != 0 || strlen(m_gSocksInfo.m_Pass))
		{
			//发送返回包(需要身份验证)
			m_DataBuf[0] = 0x05;
			m_DataBuf[1] = 0x02;
			send(m_ClientSocket, m_DataBuf, 2, 0);

			//标识
			ret = recv(m_ClientSocket, m_DataBuf, 1, 0);
			if(ret != 1 || m_DataBuf[0] != 0x01)
			{
				delete [] m_DataBuf;
				closesocket(m_ClientSocket);
				return 0;
			}

			//用户名长度
			char ch = 0;
			ret = recv(m_ClientSocket, &ch, 1, 0);
			if(ret != 1)
			{
				delete [] m_DataBuf;
				closesocket(m_ClientSocket);
				return 0;
			}

			//用户名
			ret = recv(m_ClientSocket, m_DataBuf, ch, 0);
			if(ret != ch)
			{
				delete [] m_DataBuf;
				closesocket(m_ClientSocket);
				return 0;
			}
			m_DataBuf[ch] = 0x00;
			if(m_gFunc.strcmp(m_gSocksInfo.m_User, m_DataBuf) != 0)
			{
				m_DataBuf[0] = 0x01;
				m_DataBuf[1] = 0x03;
				send(m_ClientSocket, m_DataBuf, 2, 0);
				closesocket(m_ClientSocket);
				delete [] m_DataBuf;
				return 0;
			}
				
			//密码长度
			ch = 0;
			ret = recv(m_ClientSocket, &ch, 1, 0);
			if(ret != 1)
			{
				delete [] m_DataBuf;
				closesocket(m_ClientSocket);
				return 0;
			}

			//密码
			ret = recv(m_ClientSocket, m_DataBuf, ch, 0);
			if(ret != ch)
			{
				delete [] m_DataBuf;
				closesocket(m_ClientSocket);
				return 0;
			}
			m_DataBuf[ch] = 0x00;
			if(m_gFunc.strcmp(m_gSocksInfo.m_Pass, m_DataBuf) != 0)
			{
				m_DataBuf[0] = 0x01;
				m_DataBuf[1] = 0x03;
				send(m_ClientSocket, m_DataBuf, 2, 0);
				closesocket(m_ClientSocket);
				delete [] m_DataBuf;
				return 0;
			}
			m_DataBuf[0] = 0x01;
			m_DataBuf[1] = 0x00;
			send(m_ClientSocket, m_DataBuf, 2, 0);
		}
		else
		{
			//发送返回包(不需要密码验证)
			m_DataBuf[0] = 0x05;
			m_DataBuf[1] = 0;
			send(m_ClientSocket, m_DataBuf, 2, 0);
		}

		//接收字协商包
		ret = recv(m_ClientSocket, m_DataBuf, 4, 0);
		if(ret != 4)
		{
			delete [] m_DataBuf;
			closesocket(m_ClientSocket);
			return 0;
		}

		//取服务器地址信息
		SOCKETRECVINFO m_RecvInfo = {0};
		memcpy(&m_RecvInfo, m_DataBuf, sizeof(SOCKETRECVINFO));
		if(m_RecvInfo.m_CMD == 0x01 || m_RecvInfo.m_CMD == 0x03)
		{
			//TCP连接
			if(m_RecvInfo.m_ATYP == 0x01)
			{
				//IP地址
				ret = recv(m_ClientSocket, m_DataBuf, 6, 0);
				if(ret != 6)
				{
					delete [] m_DataBuf;
					closesocket(m_ClientSocket);
					return 0;
				}

				struct in_addr addr = {0}; 
				memcpy((char* )&addr.s_addr, m_DataBuf, 4); 
				m_gFunc.strcpy(m_ServerAddr, inet_ntoa(addr)); 
				m_ServerPort = ntohs(*(unsigned short*)(m_DataBuf + 4)); 
			}
			else if(m_RecvInfo.m_ATYP == 0x03)
			{
				//域名
				char len = 0;
				ret = recv(m_ClientSocket, &len, 1, 0);
				if(ret != 1)
				{
					delete [] m_DataBuf;
					closesocket(m_ClientSocket);
					return 0;
				}
				ret = recv(m_ClientSocket, m_DataBuf, len + 2, 0);
				if(ret != len + 2)
				{
					delete [] m_DataBuf;
					closesocket(m_ClientSocket);
					return 0;
				}
				memcpy(m_ServerAddr, m_DataBuf, len); 
				m_ServerAddr[len] = 0; 
				m_ServerPort = ntohs(*(unsigned short*)(m_DataBuf + len)); 
			}
			else
			{
				//其他协议,不支持
				delete [] m_DataBuf;
				closesocket(m_ClientSocket);
				return 0;
			}

			if(m_RecvInfo.m_CMD == 0x03)
			{
				m_IsUdp = TRUE;
			}
		}
		else
		{
			//未知连接
			delete [] m_DataBuf;
			closesocket(m_ClientSocket);
			return 0;
		}
	}
	else
	{
		//未知连接
		delete [] m_DataBuf;
		closesocket(m_ClientSocket);
		return 0;
	}

	if(!m_IsUdp)
	{
		sockaddr_in m_addr = {0};
		m_addr.sin_family = AF_INET;
		int	addrlen = sizeof(sockaddr_in);

		//连接目的机器
		m_LocalSocket = GetConnectSocket(m_ServerAddr, m_ServerPort);
		if(m_LocalSocket != NULL)
		{
			getsockname(m_LocalSocket, (sockaddr*) &m_addr, &addrlen);
		}
		
		if(m_ProxyTypes == PS_PROXY_SOCKS5)
		{
			//填充SOCKS5返回包
			m_DataBuf[0] = 0x05;
			if(m_LocalSocket== NULL)
			{
				m_DataBuf[1] = 0x03;
			}
			else
			{
				m_DataBuf[1] = 0x00;
			}
			m_DataBuf[2] = 0x00;
			m_DataBuf[3] = 0x01;
			memcpy(m_DataBuf + 4, &m_addr.sin_addr, sizeof(in_addr));
			*((WORD*) (m_DataBuf + 4 + sizeof(in_addr))) = m_addr.sin_port;
			send(m_ClientSocket, m_DataBuf, 10, 0);
		}
		else if(m_ProxyTypes == PS_PROXY_SOCKS4)
		{
			//填充SOCKS4返回包
			m_gFunc.memset(m_DataBuf, 0, sizeof(m_DataBuf));
			m_DataBuf[0] = 0x00;
			if(m_LocalSocket== NULL)
			{
				m_DataBuf[1] = 91;
			}
			else
			{
				m_DataBuf[1] = 90;
			}
			*((WORD*) (m_DataBuf + 2)) = m_addr.sin_port;
			CopyMemory(m_DataBuf + 4, &m_addr.sin_addr, sizeof(in_addr));
			send(m_ClientSocket, m_DataBuf, 8, 0);
		}

		//清除协商缓冲
		delete [] m_DataBuf;
		if(m_LocalSocket == NULL)
		{
			closesocket(m_ClientSocket);
			return 0;
		}

		//创建缓冲
		LPTCPSENDRECVINFO pInfo = new TCPSENDRECVINFO;
		pInfo->sSend = m_LocalSocket;
		pInfo->sRecv = m_ClientSocket;
		m_gFunc.CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DoTcpSendRecvThread, (LPVOID) pInfo, 0, NULL);

		pInfo = new TCPSENDRECVINFO;
		pInfo->sSend = m_ClientSocket;
		pInfo->sRecv = m_LocalSocket;
		m_gFunc.CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DoTcpSendRecvThread, (LPVOID) pInfo, 0, NULL);
	}
	else
	{
		//客户机器的IP地址和端口
		sockaddr_in m_ClientAddr = {0};
		int	addrlen = sizeof(sockaddr_in);
		m_ClientAddr.sin_family = AF_INET;
		getpeername(m_ClientSocket, (sockaddr*) &m_ClientAddr, &addrlen);
		m_ClientAddr.sin_port = htons(m_ServerPort);

		//获取本机的UDP侦听套接字
		WORD m_UdpPort = 0;
		SOCKET m_LocalSocket = GetUdpListenSocket(&m_UdpPort);

		//获取本机IP地址和端口
		sockaddr_in m_LocalAddr = {0};
		m_LocalAddr.sin_family = AF_INET;
		getsockname(m_ClientSocket, (sockaddr*) &m_LocalAddr, &addrlen);
					
		//填充SOCKS5返回包
		m_DataBuf[0] = 0x05;
		if(m_LocalSocket== NULL)
		{
			m_DataBuf[1] = 0x03;
		}
		else
		{
			m_DataBuf[1] = 0x00;
		}
		m_DataBuf[2] = 0x00;
		m_DataBuf[3] = 0x01;
		memcpy(m_DataBuf + 4, &m_LocalAddr.sin_addr, 4);
		*((WORD*) (m_DataBuf + 4 + sizeof(in_addr))) = htons(m_UdpPort);
		send(m_ClientSocket, m_DataBuf, 10, 0);
		delete [] m_DataBuf;

		if(m_LocalSocket == NULL)
		{		
			closesocket(m_ClientSocket);
			return 0;
		}

		LPUDPSENDRECVINFO pInfo = new UDPSENDRECVINFO;
		memcpy(&pInfo->m_ClientAddr, &m_ClientAddr, sizeof(sockaddr_in));
		pInfo->m_LocalSocket = m_LocalSocket;
		m_gFunc.CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DoUdpSendRecvThread, (LPVOID) pInfo, 0, NULL);

		//开始UDP转发
		while(1)
		{
			char ch = 0;
			int ret = recv(m_ClientSocket, &ch, 1, 0);
			if(ret <= 0)
			{
				break;
			}
		}
		closesocket(m_LocalSocket);
		closesocket(m_ClientSocket);
	}
	return 0;
}

DWORD CMySocksModule::DoUdpSendRecvThread(LPVOID lPvoid)
{
	UDPSENDRECVINFO m_Info = {0};
	memcpy(&m_Info, lPvoid, sizeof(UDPSENDRECVINFO));
	delete (LPUDPSENDRECVINFO) lPvoid;

	char* m_RecvBuf = new char[PS_PROXY_BUFLEN + 1];
	while(m_gFunc.WaitForSingleObject(m_gSocksInfo.hExitEvent, 0) == WAIT_TIMEOUT)
	{
		sockaddr_in m_recvaddr = {0};
		m_recvaddr.sin_family = AF_INET;
		int	addrlen = sizeof(sockaddr_in);
		int ret = recvfrom(m_Info.m_LocalSocket, m_RecvBuf, PS_PROXY_BUFLEN, 0, (sockaddr*) &m_recvaddr, &addrlen);
		if(ret <= 0)
		{
			break;
		}

		if(memcmp(&m_recvaddr.sin_addr, &m_Info.m_ClientAddr.sin_addr, sizeof(in_addr)) == 0)
		{
			char* pCurPoint = m_RecvBuf;

			//来自客户机
			pCurPoint += 3;

			sockaddr_in m_sendaddr = {0};
			m_sendaddr.sin_family = AF_INET;
			int	addrlen = sizeof(sockaddr_in);
			
			//TCP连接
			if(*pCurPoint == 0x01)
			{
				pCurPoint ++;

				//IP地址
				memcpy((char*) &m_sendaddr.sin_addr, pCurPoint, 4);
				pCurPoint += 4;

				//端口
				m_sendaddr.sin_port = *((WORD*) pCurPoint);
				pCurPoint += 2;
			}
			else if(*pCurPoint == 0x03)
			{
				pCurPoint ++;

				//域名长度
				char len = *pCurPoint;
				pCurPoint ++;

				//域名
				char m_ServerAddr[256] = {0};
				memcpy(m_ServerAddr, pCurPoint, len);
				pCurPoint += len;

				PHOSTENT hostinfo = gethostbyname(m_ServerAddr);
				if(hostinfo == NULL)
				{
					continue;
				}
				memcpy(&m_sendaddr.sin_addr, hostinfo->h_addr_list[0], hostinfo->h_length);
				
				//端口
				m_sendaddr.sin_port = *((WORD*) pCurPoint);
				pCurPoint += 2;
			}
			else
			{
				//不支持的协议
				continue;
			}

			//发送到远端服务器
			int ret1 = sendto(m_Info.m_LocalSocket, pCurPoint, ret - (pCurPoint - m_RecvBuf), 
				0, (sockaddr*) &m_sendaddr, sizeof(sockaddr_in));
		}
		else
		{
			char* m_SendBuf = new char[ret + 10];
			
			char* pCurPoint = m_SendBuf;
			pCurPoint += 3;
			
			*pCurPoint = 0x01;
			pCurPoint ++;

			//地址
			memcpy(pCurPoint, &m_recvaddr.sin_addr, 4);
			*pCurPoint += 4;

			//端口
			*((WORD*) pCurPoint) = m_recvaddr.sin_port;
			pCurPoint += 2;

			//实际数据
			memcpy(pCurPoint, m_RecvBuf, ret);

			//发送到客户端
			sendto(m_Info.m_LocalSocket, m_SendBuf, ret + 10, 
				0, (sockaddr*) &m_Info.m_ClientAddr, sizeof(sockaddr_in));
			delete [] m_SendBuf;
		}
	}
	delete [] m_RecvBuf;
	closesocket(m_Info.m_LocalSocket);
	return 0;
}

SOCKET CMySocksModule::GetUdpListenSocket(WORD* wPort)
{
	//建立套接字
	SOCKET Sck = socket(AF_INET, SOCK_DGRAM, 0);
	if(Sck==INVALID_SOCKET)
	{
		return NULL;
	}

	//绑定套接字
	for(int i = 1025; i < 65534; i++)
	{
		sockaddr_in addr = {0};
		addr.sin_family = AF_INET;
		addr.sin_port = htons(i);

		if(bind(Sck, (sockaddr*) &addr, sizeof(addr)) == SOCKET_ERROR)
		{
			closesocket(Sck);
			continue;
		}
		else
		{
			*wPort = i;
			return Sck;
		}
	}

	closesocket(Sck);
	return NULL;
}

DWORD CMySocksModule::SSH_ProxyListenThreadSocks(LPVOID lPvoid)
{
	SOCKET sMainSocket = (SOCKET) lPvoid;
	SOCKET sAccept = NULL;
	while(1)
	{
		//等待客户连接
		if((sAccept = accept(sMainSocket, 0, 0)) == INVALID_SOCKET) 
		{
			break;
		}
		m_gFunc.CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) SSH_ProxyMainSocksThread, (LPVOID) sAccept, 0, NULL);
	}
	return 0;
}

BOOL CMySocksModule::StartWork(LPMYSOCKSINFO pSocketInfo)
{
	CopyMemory(&m_gSocksInfo, pSocketInfo, sizeof(MYSOCKSINFO));
	
	//侦听端口
	m_MainSocket = GetTcpListenSocket(m_gSocksInfo.m_Port);
	if(m_MainSocket == NULL)
	{
		return FALSE;
	}

	//中止事件
	m_gSocksInfo.hExitEvent = m_gFunc.CreateEvent(NULL, TRUE, FALSE, NULL);

	//启动侦听线程
	m_gFunc.CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) SSH_ProxyListenThreadSocks, (LPVOID) m_MainSocket, 0, NULL);
	return TRUE;
}

void CMySocksModule::StopWork()
{
	if(m_MainSocket != NULL)
	{
		closesocket(m_MainSocket);
		m_MainSocket = NULL;
	}

	if(m_gSocksInfo.hExitEvent != NULL)
	{
		m_gFunc.CloseHandle(m_gSocksInfo.hExitEvent);
		m_gSocksInfo.hExitEvent = NULL;
	}
}
