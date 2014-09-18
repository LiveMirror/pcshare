#if !defined(AFX_MYCLIENTMANAVIEW_H__9A62FDAF_7348_4229_87F0_5BCF0DD1CBF1__INCLUDED_)
#define AFX_MYCLIENTMANAVIEW_H__9A62FDAF_7348_4229_87F0_5BCF0DD1CBF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyClientManaView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyClientManaView view

class CMyClientManaView : public CXTListView
{
protected:
	           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyClientManaView)

	void InsertViewMenu(CMenu* pMenu);

// Attributes
public:
	CMyClientManaView();
	virtual ~CMyClientManaView();
// Operations
public:

	void SetItemPort(LPCTSTR sPort, LPCTSTR sTitle);
	void SetItemNote(LPCTSTR sNote, LPCTSTR sTitle);
	void InsertItem(MyServerTran* pItem, BOOL IsShowLogs);
	MyServerTran* GetCurSelect();
	MyServerTran* GetItemFromId(LPCTSTR sId);
	BOOL DeleteItemFromId(LPCTSTR sId);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyClientManaView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:

	CImageList	m_imagelist;
	HANDLE		m_ExitEvent;
	HANDLE		hKeepAliveThread;

	virtual bool SortList(int nCol, bool bAscending);

	void StartChildProcess(LPCTSTR sChildWndName, DWORD nCmd);
	void StartChildProcess(LPCTSTR sChildWndName, DWORD nCmd, MyServerTran* pItem);
	
	void KeepAlive();
	static void KeepAliveThread(LPVOID lPvoid);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyClientManaView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClientManaTlnt();
	afx_msg void OnUpdateClientManaTlnt(CCmdUI* pCmdUI);
	afx_msg void OnClientManaUnin();
	afx_msg void OnUpdateClientManaUnin(CCmdUI* pCmdUI);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDestroy();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnClientManaFile();
	afx_msg void OnUpdateClientManaFile(CCmdUI* pCmdUI);
	afx_msg void OnClientManaCwnd();
	afx_msg void OnUpdateClientManaCwnd(CCmdUI* pCmdUI);
	afx_msg void OnClientManaKeym();
	afx_msg void OnUpdateClientManaKeym(CCmdUI* pCmdUI);
	afx_msg void OnClientManaFram();
	afx_msg void OnUpdateClientManaFram(CCmdUI* pCmdUI);
	afx_msg void OnClientManaLink();
	afx_msg void OnUpdateClientManaLink(CCmdUI* pCmdUI);
	afx_msg void OnClientManaMess();
	afx_msg void OnUpdateClientManaMess(CCmdUI* pCmdUI);
	afx_msg void OnClientManaMult();
	afx_msg void OnUpdateClientManaMult(CCmdUI* pCmdUI);
	afx_msg void OnClientManaProc();
	afx_msg void OnUpdateClientManaProc(CCmdUI* pCmdUI);
	afx_msg void OnClientManaRegt();
	afx_msg void OnUpdateClientManaRegt(CCmdUI* pCmdUI);
	afx_msg void OnClientManaServ();
	afx_msg void OnUpdateClientManaServ(CCmdUI* pCmdUI);
	afx_msg void OnClientPcRestart();
	afx_msg void OnUpdateClientPcRestart(CCmdUI* pCmdUI);
	afx_msg void OnClientPcShutdown();
	afx_msg void OnUpdateClientPcShutdown(CCmdUI* pCmdUI);
	afx_msg void OnClientUpdateFile();
	afx_msg void OnUpdateClientUpdateFile(CCmdUI* pCmdUI);
	afx_msg void OnClientUploadFile();
	afx_msg void OnUpdateClientUploadFile(CCmdUI* pCmdUI);
	afx_msg void OnModifyTitle();
	afx_msg void OnUpdateModifyTitle(CCmdUI* pCmdUI);
	afx_msg void OnListSelectAll();
	afx_msg void OnUpdateListSelectAll(CCmdUI* pCmdUI);
	afx_msg void OnListSelectNoone();
	afx_msg void OnUpdateListSelectNoone(CCmdUI* pCmdUI);
	afx_msg void OnClientOpenUrl();
	afx_msg void OnUpdateClientOpenUrl(CCmdUI* pCmdUI);
	afx_msg void OnClientStartSocks();
	afx_msg void OnUpdateClientStartSocks(CCmdUI* pCmdUI);
	afx_msg void OnClientStopSocks();
	afx_msg void OnUpdateClientStopSocks(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnKeepAlive(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTranEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnResizeWnd(WPARAM wParam,LPARAM lParam);
	afx_msg void OnViewNormalExt(UINT nID);
	afx_msg	void OnUpdateViewNormalExt(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCLIENTMANAVIEW_H__9A62FDAF_7348_4229_87F0_5BCF0DD1CBF1__INCLUDED_)
