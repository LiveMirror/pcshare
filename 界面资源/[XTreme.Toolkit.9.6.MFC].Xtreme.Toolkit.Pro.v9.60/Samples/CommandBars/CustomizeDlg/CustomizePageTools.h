// CustomizePageTools.h : header file
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

#if !defined(AFX_CUSTOMIZEPAGETOOLS_H__58567F70_9B97_4337_A3D2_78B53624E90B__INCLUDED_)
#define AFX_CUSTOMIZEPAGETOOLS_H__58567F70_9B97_4337_A3D2_78B53624E90B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CToolRec : public CObject
{
public:
	CToolRec(LPCTSTR strTitle = NULL, LPCTSTR strCmd = NULL)
	{
		m_strTitle = strTitle;
		m_strCmd = strCmd;
		m_nIconID = 0;
	}
	CToolRec(const CToolRec& tool)
	{
		m_strTitle = tool.m_strTitle;
		m_strArg = tool.m_strArg;
		m_strCmd = tool.m_strCmd;
		m_strDir = tool.m_strDir;
		m_nIconID = tool.m_nIconID;
	}
	CToolRec* operator=(const CToolRec& tool)
	{
		m_strTitle = tool.m_strTitle;
		m_strArg = tool.m_strArg;
		m_strCmd = tool.m_strCmd;
		m_strDir = tool.m_strDir;
		m_nIconID = tool.m_nIconID;
		return this;
	}
	virtual void Serialize(CArchive& ar);
public:
	UINT m_nIconID;         // Command ID to be associated with this tool.
	CString m_strTitle;
	CString m_strArg;       // String that represents the argument passed to the executable.
	CString m_strCmd;       // String that represents the executable associated with this command.
	CString m_strDir;       // String that represents the default directory.
};

typedef CList<CToolRec, CToolRec&> CToolsList;

/////////////////////////////////////////////////////////////////////////////
// CCustomizePageTools dialog

class CCustomizePageTools : public CPropertyPage
{
	DECLARE_DYNCREATE(CCustomizePageTools)

// Construction
public:
	CCustomizePageTools();
	~CCustomizePageTools();

public:
	static void SaveTools(LPCTSTR lpszProfile);
	static BOOL LoadTools(LPCTSTR lpszProfile);
	static CToolsList m_lstTools;
	static void UpdateIcons();



protected:

// Dialog Data
	//{{AFX_DATA(CCustomizePageTools)
	enum { IDD = IDD_PAGE_TOOLS };
	CStatic m_txtToolsDir;
	CStatic m_txtToolsArg;
	CStatic m_txtToolsCmd;
	CString m_strToolsDir;
	CString m_strToolsArg;
	CString m_strToolsCmd;
	//}}AFX_DATA

#ifdef _XTP_INCLUDE_CONTROLS
	CXTBrowseEdit   m_editToolsDir;
	CXTBrowseEdit   m_editToolsArg;
	CXTBrowseEdit   m_editToolsCmd;
	CXTEditListBox  m_lboxToolsList;
#else
	CEdit   m_editToolsDir;
	CEdit   m_editToolsArg;
	CEdit   m_editToolsCmd;
	CListBox    m_lboxToolsList;

#endif



protected:
	void UpdateToolData();
	void EnableControls(bool bEnable);
	void UpdateToolsList();


	// Ignore:
	//{{AFX_VIRTUAL(CCustomizePageTools)
public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	// Ignore:
	//{{AFX_MSG(CCustomizePageTools)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeEditList();
	afx_msg void OnChangeToolsCmd();
	afx_msg void OnChangeToolsArg();
	afx_msg void OnChangeToolsDir();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	afx_msg void OnLabelEditEnd();
	afx_msg void OnLabelEditCancel();
	afx_msg void OnNewItem();
	afx_msg void OnPreDeleteItem();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMIZEPAGETOOLS_H__58567F70_9B97_4337_A3D2_78B53624E90B__INCLUDED_)
