// MySocket.h: interface for the CMySocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSOCKET_H__EDEA45F6_0F39_4E53_8BBF_4CE79A00420E__INCLUDED_)
#define AFX_MYSOCKET_H__EDEA45F6_0F39_4E53_8BBF_4CE79A00420E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMySocket  
{
public:
	CMySocket();
	virtual ~CMySocket();

	SOCKET m_Socket;
	HANDLE hListenThread;

	SOCKET static GetConnectSocket(LPCTSTR pServerAddr, UINT nServerPort);
	BOOL StartWork(LPCTSTR pRemoteAddr, LPCTSTR pRemotePort, LPCTSTR pLocalPort);
	void Close();

	void DoListen();
	static DWORD DoListenThread(LPVOID lPvoid);

	static DWORD DoChildWorkThread(LPVOID lPvoid);
	static DWORD DoSendRecvThread(LPVOID lPvoid);
};

#endif // !defined(AFX_MYSOCKET_H__EDEA45F6_0F39_4E53_8BBF_4CE79A00420E__INCLUDED_)
