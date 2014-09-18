// MySocket.h: interface for the CMySocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSOCKET_H__EDEA45F6_0F39_4E53_8BBF_4CE79A00420E__INCLUDED_)
#define AFX_MYSOCKET_H__EDEA45F6_0F39_4E53_8BBF_4CE79A00420E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct _SOCKETINFO_
{
	DWORD	m_Port;
	DWORD	m_Pass;
	SOCKET	m_Socket;
	HANDLE	hMetux;
	HANDLE	hExitEvent;
}SOCKETINFO, *LPSOCKETINFO;

typedef struct _SENDRECVINFO_
{
	SOCKET sSend;
	SOCKET sRecv;
}SENDRECVINFO, *LPSENDRECVINFO;

typedef struct _PROXYLOGININFO_
{
	UINT	m_Cmd;
	DWORD	m_Data;
}PROXYLOGININFO, *LPPROXYLOGININFO;

#define WM_GETDESSOCKET		WM_USER + 100
#define WM_SHOWINFO			WM_USER + 101

#define P_WAIT_ACCEPT_ERROR	1
#define P_CLIENT_CLOSE		2

#define p_CONNECT_START		5000
#define P_CONNECT_CMD		5001
#define P_CONNECT_DATA		5002
#define P_CONNECT_SUCCESS	5003
#define P_CONNECT_ERROR		5004
#define P_CONNECT_ALIVE		5005

#define	p_CONNECT_END		5999

class CMySocket  
{
public:
	CMySocket();
	virtual ~CMySocket();

	SOCKET m_Socket;
	HANDLE hListenThread;

	SOCKET GetListenSocket(WORD wPort);

	SOCKET static GetConnectSocket(LPCTSTR pServerAddr, UINT nServerPort);
	BOOL StartWork(LPCTSTR pPort, LPCTSTR pPass);
	void Close();

	void DoListen();
	static DWORD DoClientTransWork(SOCKET sAccept, UINT nCmd);

	static DWORD DoListenThread(LPVOID lPvoid);
	static DWORD DoChildWorkThread(LPVOID lPvoid);
	static DWORD DoSendRecvThread(LPVOID lPvoid);
};

#endif // !defined(AFX_MYSOCKET_H__EDEA45F6_0F39_4E53_8BBF_4CE79A00420E__INCLUDED_)
