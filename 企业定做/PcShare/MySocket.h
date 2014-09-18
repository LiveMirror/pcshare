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
	char	m_ServerAddr[256];
	char	m_LocalAddr[256];
	DWORD	m_ServerPort;
	DWORD	m_LocalPort;
	SOCKET	m_CmdSocket;
	HANDLE	hMetux;
}SOCKETINFO, *LPSOCKETINFO;

typedef struct _SENDRECVINFO_
{
	SOCKET sSend;
	SOCKET sRecv;
}SENDRECVINFO, *LPSENDRECVINFO;



#define WM_GETDESSOCKET		WM_USER + 100
#define WM_SHOWINFO			WM_USER + 101

#define P_WAIT_ACCEPT_ERROR	1
#define P_CLIENT_CLOSE		2



class CMySocket  
{
public:
	CMySocket();
	virtual ~CMySocket();

	HANDLE hListenThread;
	BOOL	bIsExit;
	SOCKET GetListenSocket(WORD wPort);

	SOCKET static GetConnectSocket(char* pServerAddr, UINT nServerPort);
	BOOL StartWork(SOCKET sCmdSocket);
	void Close();

	void DoListen();
	static DWORD DoListenThread(LPVOID lPvoid);

	static DWORD DoChildWorkThread(LPVOID lPvoid);
	static DWORD DoSendRecvThread(LPVOID lPvoid);
};

#endif // !defined(AFX_MYSOCKET_H__EDEA45F6_0F39_4E53_8BBF_4CE79A00420E__INCLUDED_)
