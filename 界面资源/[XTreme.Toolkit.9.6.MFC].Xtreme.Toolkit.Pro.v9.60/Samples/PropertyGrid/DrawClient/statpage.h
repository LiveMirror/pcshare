// statpage.h : header file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// ©1998-2005 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CStatPage dialog

class CStatPage : public CPropertyPage
{
		DECLARE_DYNCREATE(CStatPage)

// Construction
public:
		CStatPage();
		~CStatPage();

// Dialog Data
		//{{AFX_DATA(CStatPage)
	enum { IDD = IDD_STAT_PAGE };
	CString m_strSavedBy;
	CString m_strRevNum;
	CString m_strEditTime;
	CString m_strLastPrint;
	CString m_strCreateDate;
	CString m_strLastSave;
	CString m_strNumPages;
	CString m_strNumWords;
	CString m_strNumChars;
	CString m_strSecurity;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
		//{{AFX_VIRTUAL(CStatPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
		//{{AFX_MSG(CStatPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
