// EditListPages.h : header file
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

#ifndef __EDITLISTPAGES_H__
#define __EDITLISTPAGES_H__

struct TOOLS
{
	CString strCmd;
	CString strArg;
	CString strInt;
};

typedef CList<TOOLS*,TOOLS*> CToolsList;

/////////////////////////////////////////////////////////////////////////////
// CEditListPage1 dialog

class CEditListPage1 : public CXTResizePropertyPage
{
	DECLARE_DYNCREATE(CEditListPage1)

// Construction
public:
	CEditListPage1();
	~CEditListPage1();

// Dialog Data
	//{{AFX_DATA(CEditListPage1)
	enum { IDD = IDD_PROPPAGE1 };
	CButton m_chkEnable;
	CStatic m_txtEdit3;
	CStatic m_txtEdit2;
	CStatic m_txtEdit1;
	CXTBrowseEdit   m_edit3;
	CXTBrowseEdit   m_edit2;
	CXTBrowseEdit   m_edit1;
	CXTEditListBox  m_editList;
	BOOL    m_bEnable;
	//}}AFX_DATA

	CToolsList m_arTools;

	enum { TOOLCMD, TOOLARG, TOOLINT };

	void AddListItem(LPCTSTR lpszTitle, LPCTSTR lpszCmd, LPCTSTR lpszArg, LPCTSTR lpszInt);
	void AppendEditItem(LPCTSTR lpszText, int iWhich);
	void UpdateEditData(int iWhich);

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEditListPage1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEditListPage1)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeEditList();
	afx_msg void OnChangeEdit1();
	afx_msg void OnChangeEdit2();
	afx_msg void OnChangeEdit3();
	afx_msg void OnClickedit2Filepath();
	afx_msg void OnClickedit2Filedirectory();
	afx_msg void OnClickedit2Filename();
	afx_msg void OnClickedit2Fileextension();
	afx_msg void OnClickedit2Currentline();
	afx_msg void OnClickedit2Currentcolumn();
	afx_msg void OnClickedit2Currenttext();
	afx_msg void OnClickedit2Currentdirectory();
	afx_msg void OnClickedit2Targetpath();
	afx_msg void OnClickedit2Targetdirectory();
	afx_msg void OnClickedit2Targetname();
	afx_msg void OnClickedit2Targetextension();
	afx_msg void OnClickedit2Targetarguments();
	afx_msg void OnClickedit2Workspacedirectory();
	afx_msg void OnClickedit2Workspacename();
	afx_msg void OnClickedit3Filedirectory();
	afx_msg void OnClickedit3Filename();
	afx_msg void OnClickedit3Currentdirectory();
	afx_msg void OnClickedit3Targetdirectory();
	afx_msg void OnClickedit3Targetname();
	afx_msg void OnClickedit3Workspacedirectory();
	afx_msg void OnClickedit3Workspacename();
	afx_msg void OnChkEnable();
	//}}AFX_MSG
	afx_msg void OnLabelEditEnd();
	afx_msg void OnLabelEditCancel();
	afx_msg void OnNewItem();
	afx_msg void OnDeleteItem();
	afx_msg void OnMoveItemUp();
	afx_msg void OnMoveItemDown();
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CEditListPage2 dialog

class CEditListPage2 : public CXTResizePropertyPage
{
	DECLARE_DYNCREATE(CEditListPage2)

// Construction
public:
	CEditListPage2();
	~CEditListPage2();

// Dialog Data
	//{{AFX_DATA(CEditListPage2)
	enum { IDD = IDD_PROPPAGE2 };
	CXTEditListBox  m_editList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEditListPage2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEditListPage2)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnLabelEditEnd();
	afx_msg void OnLabelEditCancel();
	afx_msg void OnNewItem();
	afx_msg void OnDeleteItem();
	afx_msg void OnMoveItemUp();
	afx_msg void OnMoveItemDown();
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CEditListPage3 dialog

class CEditListPage3 : public CXTResizePropertyPage
{
	DECLARE_DYNCREATE(CEditListPage3)

// Construction
public:
	CEditListPage3();
	~CEditListPage3();

// Dialog Data
	//{{AFX_DATA(CEditListPage3)
	enum { IDD = IDD_PROPPAGE3 };
	CXTEditListBox  m_editList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEditListPage3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEditListPage3)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnLabelEditEnd();
	afx_msg void OnLabelEditCancel();
	afx_msg void OnNewItem();
	afx_msg void OnDeleteItem();
	afx_msg void OnMoveItemUp();
	afx_msg void OnMoveItemDown();
	DECLARE_MESSAGE_MAP()

};



#endif // __EDITLISTPAGES_H__
