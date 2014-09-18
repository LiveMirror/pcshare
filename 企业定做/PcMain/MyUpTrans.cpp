// MyUpTrans.cpp: implementation of the CMyUpTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyUpTrans.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyUpTrans::CMyUpTrans()
{

}

CMyUpTrans::~CMyUpTrans()
{

}

void CMyUpTrans::StartWork(HWND hWnd)
{
	//连接到服务器
	if(!Create(WM_CONNECT_UP_FILE, hWnd))
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

		m_FileInfo.m_Response = 0;
		HANDLE hFile = m_gFunc.CreateFile(m_FileInfo.m_RemoteFile, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			m_FileInfo.m_Response = DL_FILE_LOSE;
		}
		if(m_FileInfo.m_Response == 0)
		{
			m_gFunc.GetFileSizeEx(hFile, &m_FileInfo.m_FilePoint);
			if(m_FileInfo.m_FilePoint.QuadPart > m_FileInfo.m_FileLen.QuadPart)
			{
				m_gFunc.CloseHandle(hFile);
				m_FileInfo.m_Response = DL_FILE_LEN_ERR;
			}
		}
		if(m_FileInfo.m_Response == 0)
		{
			//移动到末尾
			LARGE_INTEGER nPos;
			nPos.QuadPart = 0;
			m_gFunc.SetFilePointerEx(hFile, nPos, NULL, FILE_END);
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

		//查看是否需要接收
		if(nTransLen.QuadPart == 0)
		{
			m_gFunc.CloseHandle(hFile);
			continue;
		}

		//接收文件实体
		DWORD len = 0;
		DWORD nTmpLen = 1024 * 1024;
		DWORD nRecvLen = 0;
		BYTE* pFileBuf = new BYTE[nTmpLen];
		while(nTransLen.QuadPart > 0)
		{
			len = DWORD (nTransLen.QuadPart > nTmpLen ? nTmpLen : nTransLen.QuadPart);
			if(!RecvData(pFileBuf, len))
			{
				break;
			}
			m_gFunc.WriteFile(hFile, pFileBuf, len, &nRecvLen, NULL);
			nTransLen.QuadPart -= len;
		}
		delete [] pFileBuf;
		m_gFunc.CloseHandle(hFile);
	}
}

