// MyTrans.h: interface for the CMyTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYTRANS_H__B3652F04_7623_4FAB_9914_4B5207B6B8D5__INCLUDED_)
#define AFX_MYTRANS_H__B3652F04_7623_4FAB_9914_4B5207B6B8D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyClientTran  
{
public:
	CMyClientTran();
	virtual ~CMyClientTran();

private:
	SOCKET			m_Socket;
	char			m_Addr[256];
	UINT			m_Port;

	void EncryptByte(LPVOID pData, DWORD nLen);

public:

	BOOL Create(DWORD nCmd, char* m_ServerAddr, UINT m_ServerPort, char* pUrl, UINT nPassWord);
	void Close();
	BOOL GetRealServerInfo(char* m_DdnsUrl, char* pServerAddr, UINT* pServerPort);
	DWORD  GetIpLong(LPCTSTR pServerAddr);
	SOCKET GetConnectSocket(LPCTSTR pServerAddr, UINT nServerPort);
	BOOL SendData(LPVOID data, DWORD len);
	BOOL RecvData(LPVOID data, DWORD len);
};

#endif // !defined(AFX_MYTRANS_H__B3652F04_7623_4FAB_9914_4B5207B6B8D5__INCLUDED_)
