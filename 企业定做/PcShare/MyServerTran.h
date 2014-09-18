// MyTrans.h: interface for the CMyTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYTRANS_H__B3652F04_7623_4FAB_9914_4B5207B6B8D5__INCLUDED_)
#define AFX_MYTRANS_H__B3652F04_7623_4FAB_9914_4B5207B6B8D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MyServerTran  
{
public:
	MyServerTran();
	virtual ~MyServerTran();

	BOOL			m_IsCheck;
	TCHAR			m_ProxyIp[256];

protected:
	SOCKET			m_Socket;
	DWORD			m_Port;
	LOGININFO		m_LoginInfo;
	BOOL			m_IsProxy;
	TCHAR			m_IpAddr[256];
	TCHAR			m_Title[256];				
	TCHAR			m_ViewName[256];
	TCHAR			m_Note[256];
	TCHAR			m_Id[256];
	TCHAR			m_RealAddr[256];
		
public:

	BOOL DuplicateHttpSocket(LPTCPCONNECTINFO pInfo);
	BOOL CreateSocket(SOCKET s, BOOL bIsProxy);
	void Close();
	BOOL SendCmd(DWORD nCmd, HWND hWnd);
	BOOL RecvDataWithCmd(LPBYTE* pRecvData, DWORD* dRecvLen, DWORD* dCmd);
	BOOL SendDataWithCmd(LPVOID pSendData, DWORD dSendLen, DWORD nCmd);
	SOCKET GetSafeSocket();
	void SetUnBlock(HWND hWnd);

	BOOL CheckTitle(LPCTSTR sTitle);
	BOOL CheckId(LPCTSTR sId);
	BOOL CheckSocket(SOCKET s);

	void SendAliveCmd();

	const MyServerTran& operator=(const MyServerTran& SrcTran);

	LPCTSTR GetTitle();
	LPCTSTR GetViewName();
	LPCTSTR GetId();
	LPCTSTR GetNote();
	LPCTSTR GetRealAddr();

	BOOL IsInit();
	void SetNote(LPCTSTR sNote);
	void SetViewName(LPCTSTR sViewName);
	LPLOGININFO GetLoginInfo();

	BOOL TcpSendDataMouse(LPVOID data);

	//发送数据
BOOL TcpSendData(LPVOID data, DWORD len)
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
BOOL TcpRecvData(LPVOID data, DWORD len)
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


private:

	void FillMyInfo();

	// 传输层 Unicode 转换[9/16/2007 zhaiyinwei]
	BOOL ConvertRecvDataWithCmd(LPBYTE pRecvData, DWORD dRecvLen, DWORD dCmd);
	BOOL ConvertSendDataWithCmd(LPVOID pSendData, DWORD dSendLen, DWORD nCmd);
	UINT m_nMineCodePage;	//本机代码页
public:
	// 按代码页转换字符串
	int CodePageConvert(char* pszData, DWORD &nDataLen, int nSrcCodePage,int nDestCodePage);
	int Convert2Unicode(char* pszData, DWORD nDataLen, int nSrcCodePage = -2);
	int Convert2ClientANSI(wchar_t* pszData, DWORD nDataLen, int nClientCodePage = -2);
};

#endif // !defined(AFX_MYTRANS_H__B3652F04_7623_4FAB_9914_4B5207B6B8D5__INCLUDED_)
