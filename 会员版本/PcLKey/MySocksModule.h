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

typedef struct _MYSOCKSINFO_
{
	WORD	m_Port;
	char	m_User[64];
	char	m_Pass[32];
	HANDLE	hExitEvent;
}MYSOCKSINFO, *LPMYSOCKSINFO;

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

	void static DoUdpSendRecvThread(LPVOID lPvoid);
	void static DoTcpSendRecvThread(LPVOID lPvoid);
	void static SSH_ProxyMainSocksThread(LPVOID lPvoid);
	void static SSH_ProxyListenThreadSocks(LPVOID lPvoid);
};

#endif // !defined(AFX_MYSOCKSMODULE_H__1B4FDF4F_9E61_401F_8DB6_09DE7515ECD4__INCLUDED_)
