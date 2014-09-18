#if !defined(AFX_MYWAITTRANSDLG_H__7A8B6ACA_80EC_4FF3_BE1E_6726B0CB4B56__INCLUDED_)
#define AFX_MYWAITTRANSDLG_H__7A8B6ACA_80EC_4FF3_BE1E_6726B0CB4B56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyWaitTransDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyWaitTransDlg dialog
#include "MyBaseWnd.h"

class CMyWaitTransDlg : public CDialog
{
// Construction
public:
	CMyWaitTransDlg(CWnd* pParent);   // standard constructor

	~CMyWaitTransDlg();
// Dialog Data
	//{{AFX_DATA(CMyWaitTransDlg)
	enum { IDD = IDD_DIALOG_WAITTRANS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	//Êý¾ÝÀà
	DWORD	m_Command;	
	DWORD	m_dTransLen;
	BYTE*	m_TransData;
	HANDLE	hThread;
	CWnd*	pTran;

	static void TransDataThread(LPVOID lPvoid);
	void TransData();
	BOOL ReadBag();
	BOOL SendBag();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyWaitTransDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void ShowResponse(UINT m_Response);
	// Generated message map functions
	//{{AFX_MSG(CMyWaitTransDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnTransComplete(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYWAITTRANSDLG_H__7A8B6ACA_80EC_4FF3_BE1E_6726B0CB4B56__INCLUDED_)
