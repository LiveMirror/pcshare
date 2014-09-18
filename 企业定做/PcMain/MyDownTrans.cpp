// MyDownTrans.cpp: implementation of the CMyDownTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyDownTrans.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyDownTrans::CMyDownTrans()
{
	
}

CMyDownTrans::~CMyDownTrans()
{
	
}

void CMyDownTrans::StartWork(HWND hWnd)
{
	//连接到服务器
	if(!Create(WM_CONNECT_DL_FILE, hWnd))
	{
		return;
	}

	while(1)
	{
		//接收文件传输信息
		INTERFILEINFO m_FileInfo = {0};
		LARGE_INTEGER nTransLen = {0};
		if(!RecvData(&m_FileInfo, sizeof(INTERFILEINFO)))
		{
			break;
		}

		//打开待传送的文件
		m_FileInfo.m_Response = 0;
		HANDLE hFile = m_gFunc.CreateFile(m_FileInfo.m_RemoteFile, GENERIC_READ, 
			FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			m_FileInfo.m_Response = DL_FILE_LOSE;
		}
		
		//确定发送文件的长度
		if(m_FileInfo.m_Response == 0)
		{
			m_gFunc.GetFileSizeEx(hFile, &nTransLen);
			m_FileInfo.m_FileLen.QuadPart = nTransLen.QuadPart;
			if(m_FileInfo.m_FilePoint.QuadPart > nTransLen.QuadPart)
			{
				m_gFunc.CloseHandle(hFile);
				m_FileInfo.m_Response = DL_FILE_LEN_ERR;
			}
		}

		//确定发送的文件长度
		if(m_FileInfo.m_Response == 0 && m_FileInfo.m_FilePoint.QuadPart > 0)
		{
			//移动到断点位置
			m_gFunc.SetFilePointerEx(hFile, m_FileInfo.m_FilePoint, NULL, FILE_BEGIN);
			nTransLen.QuadPart = m_FileInfo.m_FileLen.QuadPart - m_FileInfo.m_FilePoint.QuadPart;
		}

		//发送返回数据
		if(!SendData(&m_FileInfo, sizeof(INTERFILEINFO)))
		{
			m_gFunc.CloseHandle(hFile);
			break;
		}

		//查看是否需要继续接收
		if(m_FileInfo.m_Response != 0)
		{
			continue;
		}

		//查看是否需要发送
		if(nTransLen.QuadPart == 0)
		{
			m_gFunc.CloseHandle(hFile);
			continue;
		}

		DWORD len = 0;
		DWORD nTmpLen = 1024 * 1024;
		DWORD nSendLen = 0;
		BYTE* pFileBuf = new BYTE[nTmpLen];
		while(nTransLen.QuadPart > 0)
		{
			len = DWORD (nTransLen.QuadPart > nTmpLen ? nTmpLen : nTransLen.QuadPart);
			m_gFunc.ReadFile(hFile, pFileBuf, len, &nSendLen, NULL);
			if(!SendData(pFileBuf, len))
			{
				break;
			}
			nTransLen.QuadPart -= len;
		}
		delete [] pFileBuf;
		m_gFunc.CloseHandle(hFile);
	}
}
