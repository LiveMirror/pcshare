#if !defined(AFX_MYCREATECLIENTDLG_H__4161D257_6B0F_464E_B4D3_20AF765F1F90__INCLUDED_)
#define AFX_MYCREATECLIENTDLG_H__4161D257_6B0F_464E_B4D3_20AF765F1F90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyCreateClientDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyCreateClientDlg dialog

#pragma pack( push )
#pragma pack( 2 )
typedef struct
{
	BYTE	bWidth;               // Width of the image
	BYTE	bHeight;              // Height of the image (times 2)
	BYTE	bColorCount;          // Number of colors in image (0 if >=8bpp)
	BYTE	bReserved;            // Reserved
	WORD	wPlanes;              // Color Planes
	WORD	wBitCount;            // Bits per pixel
	DWORD	dwBytesInRes;         // how many bytes in this resource?
	WORD	nID;                  // the ID
} MEMICONDIRENTRY, *LPMEMICONDIRENTRY;

typedef struct 
{
	WORD			idReserved;   // Reserved
	WORD			idType;       // resource type (1 for icons)
	WORD			idCount;      // how many images?
	MEMICONDIRENTRY	idEntries[1]; // the entries for each image
} MEMICONDIR, *LPMEMICONDIR;
#pragma pack( pop )

typedef struct
{
    TCHAR    	szFileName[256];
    HINSTANCE	hInstance;
 } EXEDLLICONINFO, *LPEXEDLLICONINFO;


#define _EXEC_FILE_			0x05
#define _RESTORE_FILE_		0x06

class CMyCreateClientDlg : public CDialog
{
// Construction
public:

	CMyCreateClientDlg(CWnd* pParent = NULL);   // standard constructor
	~CMyCreateClientDlg();
	
	BOOL SaveFileData();
	BOOL AddDataToPe(BYTE* pSaveData, DWORD DataLen, BYTE* pPeData, DWORD nPeLen, LPCTSTR pPeFile);

	LPBYTE	pDllFileData;
	LPBYTE	pExeFileData;
	LPBYTE	pSysFileData;
	LPBYTE	pComFileData;

	DWORD	m_ExeSize;
	DWORD	m_DllSize;
	DWORD	m_SysSize;
	DWORD	m_ComSize;
	BOOL	m_IsFail;
	BOOL	m_CurSorStatus;

	void UpdateIcon(LPCTSTR pSrcFileName, TCHAR* pDesFileName);

// Dialog Data
	//{{AFX_DATA(CMyCreateClientDlg)
	enum { IDD = IDD_DIALOG_CREATE_CLIENT };
	CComboBox	m_IpList;
	CString	m_Port;
	CString	m_BakUrl;
	CString	m_Title;
	CString	m_Group;
	BOOL	m_IsUrl;
	BOOL	m_IsDel;
	BOOL	m_IsSys;
	BOOL	m_IsKey;
	CString	m_ServiceName;
	CString	m_ServiceTitle;
	CString	m_ServiceView;
	CString	m_Sleeptime;
	CString m_Process;
	CString m_AddFileName;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCreateClientDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyCreateClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonCreate();
	afx_msg void OnButtonFlush();
	afx_msg void OnCheckUrl();
	afx_msg void OnCheckIsDel();
	afx_msg void OnCheckIsSys();
	afx_msg void OnCheckIsKey();
	afx_msg void OnAddFile();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg LRESULT OnSetMyCursor(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCREATECLIENTDLG_H__4161D257_6B0F_464E_B4D3_20AF765F1F90__INCLUDED_)
