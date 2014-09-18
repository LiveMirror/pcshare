#if !defined(AFX_MYMULTVIEW_H__ED1450FE_6009_4148_BD6F_A3789F17B20D__INCLUDED_)
#define AFX_MYMULTVIEW_H__ED1450FE_6009_4148_BD6F_A3789F17B20D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyMultView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyMultView view

typedef struct _PSPOINT_
{
	short	x;
	short	y;
}PSPOINT, *LPPSPOINT;

typedef struct _MYFRAMEINFO_
{
	WORD m_Width;
	WORD m_Height;
	WORD m_RectSize;
}MYFRAMEINFO, *LPMYFRAMEINFO;

class CMyMultView : public CScrollView
{
protected:
	CMyMultView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyMultView)

// Attributes
public:

// Operations
public:

	BOOL IsCanRecord();
	BOOL IsInit();
	void Start();
	void Stop();
	void Save();
	void StartWork(TCPCONNECTINFO m_SocketInfo, BOOL IsFrame);
	void SaveToAVI();
	BOOL IsRecord();
	CString GetXy();
	BOOL InitRecord();
	
	BOOL			m_IsInput;
	HBITMAP			hMainBitmap;
	BOOL			m_IsRecord;
	BITMAPINFO		m_gBitmapInfo;
	AVISTREAMINFO	m_AviInfo;
	PAVIFILE		pAviFile;
	PAVISTREAM		pAviStream, pCompressStream;
	int				nFrames; 
	CSize			m_FrameSize;
	BYTE*			pRecordData;
	MyServerTran	m_Tran;
	HANDLE			hMetux, hRecordThread, hRecvThread, hWaitRecordEvent;
	
	DWORD			nBitLen;
	HGLOBAL			hMemBmp;
	IStream*		pStmBmp;
	ULARGE_INTEGER	len;

	DWORD			nAviHz;
	LPPSPOINT		pImagePoint;

	void SendCtrlInfo();
	void SetInputEnable(BOOL bIsEnable);
	BOOL ExecCmd(LPMOUSEINFO Info);

	void DoRecord();
	static void DoRecordThread(LPVOID lPvoid);

	void TransVideo();
	static void TransVideoThread(LPVOID lPvoid);

	void TransFrame();
	static void TransFrameThread(LPVOID lPvoid);

private:
	void GetMouseXy(MOUSEINFO& m_MouseInfo, CPoint point);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyMultView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyMultView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMyMultView)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnConnBreak(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYMULTVIEW_H__ED1450FE_6009_4148_BD6F_A3789F17B20D__INCLUDED_)
