#if !defined(AFX_MYFILEDOWNLIST_H__A5FFCFE8_4AD6_479A_9CE8_A38A0C8DF703__INCLUDED_)
#define AFX_MYFILEDOWNLIST_H__A5FFCFE8_4AD6_479A_9CE8_A38A0C8DF703__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyFileDownList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyFileDownList view

class CMyFileDownList : public CXTListView
{
protected:
	CMyFileDownList();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyFileDownList)

	MyServerTran m_Tran;
	TCHAR m_Id[256];
	TCHAR m_Title[256];
	BOOL bIsUserCancel;
	TCHAR m_SaveListFileName[256];
	HANDLE hDlFilethread, hFileSaveList;

	BOOL m_IsUserExit;

	BOOL m_IsWork;

	HANDLE			hMetux;

	void SaveFileList();
// Attributes
public:

	void DownLoadFile();
	static void DownLoadFileThread(LPVOID lPvoid);

// Operations
public:

	CImageList m_imagelist;

	TCHAR* GetId();

	TCHAR* GetTitle();
	
	void AddFileDownList(LPMYINSERTDLFILEINFO pDlFileInfo, LPVOID pData, DWORD nLen);
	void PostDlCmdToClient();
	void InsertLogs(LPCTSTR pText);
	void InitData(LPCTSTR pId, LPCTSTR pTitle, LPCTSTR pSaveFileName);
	void ReNameFileName(TCHAR* pFileName, BOOL bIsBakFileName);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFileDownList)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyFileDownList();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyFileDownList)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnFileTransStart();
	afx_msg void OnUpdateFileTransStart(CCmdUI* pCmdUI);
	afx_msg void OnFileTransStop();
	afx_msg void OnUpdateFileTransStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileTransDeleteAll(CCmdUI* pCmdUI);
	afx_msg void OnFileTransDeleteAll();
	afx_msg void OnFileTransDelete();
	afx_msg void OnUpdateFileTransDelete(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnResizeWnd(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFILEDOWNLIST_H__A5FFCFE8_4AD6_479A_9CE8_A38A0C8DF703__INCLUDED_)
