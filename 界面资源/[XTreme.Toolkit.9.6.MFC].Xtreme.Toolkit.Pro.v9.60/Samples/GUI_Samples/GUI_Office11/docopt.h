// docopt.h : header file
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
// CDocOptPage dialog

class CDocOptPage : public CCSPropertyPage
{
// Construction
public:
	CDocOptPage();
	CDocOptPage(UINT nIDCaption);
	~CDocOptPage();

// Dialog Data
	//{{AFX_DATA(CDocOptPage)
	enum { IDD = IDD_OPTIONS_WRAP };
	int     m_nWordWrap;
	BOOL    m_bFormatBar;
	BOOL    m_bRulerBar;
	BOOL    m_bStatusBar;
	BOOL    m_bToolBar;
	//}}AFX_DATA


// Overrides
	static const DWORD m_nHelpIDs[];
	virtual const DWORD* GetHelpIDs() {return m_nHelpIDs;}
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDocOptPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDocOptPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

class CEmbeddedOptPage : public CDocOptPage
{
// Construction
public:
	CEmbeddedOptPage();
// Overrides
	BOOL OnInitDialog();
};
