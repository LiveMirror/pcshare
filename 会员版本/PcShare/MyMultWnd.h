// MainFrm.h : interface of the CMyMultWnd class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__6859D03F_B529_4FC8_9328_0220F6770EAE__INCLUDED_)
#define AFX_MAINFRM_H__6859D03F_B529_4FC8_9328_0220F6770EAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyMultView.h"

class CMyMultWnd : public CMyBaseWnd
{
	
public:
	CMyMultWnd();
	DECLARE_DYNCREATE(CMyMultWnd)

// Attributes
public:

	virtual BOOL StartWork(LPCTSTR sCmdStr);


// Operations
public:

	CMyMultView*		pMultView;
	CMyImageList		m_ToolBarList;
	CToolBar			m_wndToolBar;

	HANDLE			hThreadAudio;

	WAVEFORMATEX	m_WaveInfo;
	MMCKINFO		m_MMCKInfoParent;
	MMCKINFO		m_MMCKInfoChild;

	BOOL			m_IsExit, m_IsInitAudio, m_IsInitVideo;
	BOOL			m_IsAudioRecord, m_IsAudioInit, m_IsVideoInit;

	HMMIO			hAudioFile;

	void TransAudio();
	static void TransAudioThread(LPVOID lPvoid);

	CPtrList m_BufList;

	MyServerTran	m_AudioTran;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyMultWnd)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyMultWnd();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members

	int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);

// Generated message map functions
protected:
	//{{AFX_MSG(CMyMultWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnUpdateMultStart(CCmdUI* pCmdUI);
	afx_msg void OnMultStart();
	afx_msg void OnMultStop();
	afx_msg void OnUpdateMultStop(CCmdUI* pCmdUI);
	afx_msg void OnMultSave();
	afx_msg void OnUpdateMultSave(CCmdUI* pCmdUI);
	afx_msg void OnAudioInit();
	afx_msg void OnUpdateAudioInit(CCmdUI* pCmdUI);
	afx_msg void OnAudioStart();
	afx_msg void OnUpdateAudioStart(CCmdUI* pCmdUI);
	afx_msg void OnAudioStop();
	afx_msg void OnUpdateAudioStop(CCmdUI* pCmdUI);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnVideoInit();
	afx_msg void OnUpdateVideoInit(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnUpdatePane(CCmdUI* pCmdUI);
	afx_msg LRESULT OnWOM_DONE(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMultInsertBuf(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__6859D03F_B529_4FC8_9328_0220F6770EAE__INCLUDED_)
