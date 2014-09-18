// summpage.h : header file
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
// CSummPage dialog

class CSummPage : public CPropertyPage
{
		DECLARE_DYNCREATE(CSummPage)

// Construction
public:
		CSummPage();
		~CSummPage();

// Dialog Data
		//{{AFX_DATA(CSummPage)
	enum { IDD = IDD_SUMM_PAGE };
	CString m_strAuthor;
	CString m_strKeywd;
	CString m_strSubj;
	CString m_strCmt;
	CString m_strTempl;
	CString m_strTitle;
	CString m_strAppname;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
		//{{AFX_VIRTUAL(CSummPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
		//{{AFX_MSG(CSummPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
