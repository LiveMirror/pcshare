// MySocksModule.h: interface for the CMySocksModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSOCKSMODULE_H__1B4FDF4F_9E61_401F_8DB6_09DE7515ECD4__INCLUDED_)
#define AFX_MYSOCKSMODULE_H__1B4FDF4F_9E61_401F_8DB6_09DE7515ECD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PS_PROXY_SOCKS4			1			
#define PS_PROXY_SOCKS5			2
#define PS_PROXY_BUFLEN			131072

typedef struct _SOCKETRECVINFO_
{
	BYTE	m_VER;
	BYTE	m_CMD;
	BYTE	m_RSV;
	BYTE	m_ATYP;
}SOCKETRECVINFO, *LPSOCKETRECVINFO;

typedef struct _TCPSENDRECVINFO_
{
	SOCKET sSend;
	SOCKET sRecv;
}TCPSENDRECVINFO, *LPTCPSENDRECVINFO;

typedef struct _UDPSENDRECVINFO_
{
	SOCKET		m_LocalSocket;
	sockaddr_in m_ClientAddr;
}UDPSENDRECVINFO, *LPUDPSENDRECVINFO;

extern MYSOCKSINFO m_gSocksInfo;

class CMySocksModule  
{
public:
	CMySocksModule();
	virtual ~CMySocksModule();

	BOOL StartWork(LPMYSOCKSINFO SocketInfo);
	void StopWork();

protected:

	SOCKET m_MainSocket;

	SOCKET static GetConnectSocket(LPCTSTR pServerAddr, UINT nServerPort);
	SOCKET static GetTcpListenSocket(WORD  wPort);
	SOCKET static GetUdpListenSocket(WORD* wPort);

	DWORD static DoUdpSendRecvThread(LPVOID lPvoid);
	DWORD static SSH_ProxyMainSocksThread(LPVOID lPvoid);
	DWORD static SSH_ProxyListenThreadSocks(LPVOID lPvoid);

DWORD static DoTcpSendRecvThread(LPVOID lPvoid)
{
	TCPSENDRECVINFO m_Info = {0};
	memcpy(&m_Info, lPvoid, sizeof(TCPSENDRECVINFO));
	delete (LPTCPSENDRECVINFO) lPvoid;

	struct fd_set sset = {0};
	struct timeval waited = {0};
	waited.tv_sec = 30000000;
	waited.tv_usec= 0;
	int ret = 0;
	BYTE* m_Buf = new BYTE[262144];
	while(m_gFunc.WaitForSingleObject(m_gSocksInfo.hExitEvent, 0) == WAIT_TIMEOUT)
	{
		//等待服务器数据
		FD_ZERO(&sset);
		FD_SET(m_Info.sRecv, &sset);
		if(select(m_Info.sRecv + 1, &sset, NULL, NULL, &waited) <= 0)
		{
			break;
		}

		//接收服务器数据
		ret = recv(m_Info.sRecv, (char*) m_Buf, 262144, 0);
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
};

#endif // !defined(AFX_MYSOCKSMODULE_H__1B4FDF4F_9E61_401F_8DB6_09DE7515ECD4__INCLUDED_)
