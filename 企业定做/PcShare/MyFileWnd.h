#if !defined(AFX_MAINFRAME_H__2A4291AB_BF7B_419B_B116_547EAD573A48__INCLUDED_)
#define AFX_MAINFRAME_H__2A4291AB_BF7B_419B_B116_547EAD573A48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyFileWnd frame

#include "MyFileTreeView.h"
#include "MyFileListView.h"
#include "MyFileFindView.h"

class CMyFileWnd : public CMyBaseWnd
{
	DECLARE_DYNCREATE(CMyFileWnd)
public:

	CMyFileWnd();
	~CMyFileWnd();
// Attributes
public:

// Operations
public:

	BOOL	m_IsFindFile;
	virtual BOOL StartWork(LPCTSTR sCmdStr);
	
protected:  

	// ”¿‡
	CMyDirListView*		pDirListView;
	CMyDirTreeView*		pDirTreeView;
	CMyFileFindView*	pDirFindView;
	CMyImageList		m_ToolBarList;
	CSplitterWndFile	m_WndSplitter;
	CToolBar			m_wndToolBar;
	
	void DeleteTreeChild(HTREEITEM hItem);
	void GetDiskInfo(HTREEITEM hItem);
	BOOL GetMyDirLeftPath(HTREEITEM hItem, TCHAR *Path);
	void DisplayButton(CTreeCtrl *pTree, HTREEITEM hti, int nChild);

	CPtrList m_DlgList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFileWnd)
	public:
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyFileWnd)
	afx_msg void OnClose();
	afx_msg void OnUpdatePane(CCmdUI* pCmdUI);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDirFlush();
	afx_msg void OnUpdateDirFlush(CCmdUI* pCmdUI);
	afx_msg void OnDirPrev();
	afx_msg void OnUpdateDirPrev(CCmdUI* pCmdUI);
	afx_msg void OnDirRightDelete();
	afx_msg void OnDirRightExec();
	afx_msg void OnUpdateDirRightExec(CCmdUI* pCmdUI);
	afx_msg void OnDirRightOpen();
	afx_msg void OnUpdateDirRightOpen(CCmdUI* pCmdUI);
	afx_msg void OnDirRightRename();
	afx_msg void OnUpdateDirRightRename(CCmdUI* pCmdUI);
	afx_msg void OnDirRightSelall();
	afx_msg void OnUpdateDirRightSelall(CCmdUI* pCmdUI);
	afx_msg void OnDirCreate();
	afx_msg void OnUpdateDirCreate(CCmdUI* pCmdUI);
	afx_msg void OnDirRightDownload();
	afx_msg void OnUpdateDirRightDownload(CCmdUI* pCmdUI);
	afx_msg void OnDirRightUpload();
	afx_msg void OnDirRightAttrib();
	afx_msg void OnUpdateDirRightAttrib(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDirRightDelete(CCmdUI* pCmdUI);
	afx_msg void OnDirFindFile();
	afx_msg void OnUpdateDirFindFile(CCmdUI* pCmdUI);
	//}}AFX_MSG
	
	afx_msg LRESULT OnDlDeleteView(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDirRightRClick(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnTreeSelChanged(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnFindFile(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRAME_H__2A4291AB_BF7B_419B_B116_547EAD573A48__INCLUDED_)
