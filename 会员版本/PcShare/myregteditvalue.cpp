// MyRegEditValue.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyRegtEditValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyRegtEditValue dialog


CMyRegtEditValue::CMyRegtEditValue(LPCTSTR Title, LPCTSTR ItemName, LPCTSTR ItemValue, DWORD nType)
	: CDialog(CMyRegtEditValue::IDD, NULL)
{
	//{{AFX_DATA_INIT(CMyRegtEditValue)
	
	//}}AFX_DATA_INIT
	m_ItemName = ItemName;
	m_ItemValue = ItemValue;
	m_nType = nType;
	m_Title = Title;

	if(m_nType == REG_DWORD)
	{
		TCHAR* pStart = StrChr(ItemValue, _T('('));
		TCHAR* pEnd = StrRChr(ItemValue, NULL, _T(')'));
		if(pStart && pEnd)
		{
			*pEnd = 0;
			m_ItemValue = pStart + 1;
		}
	}
}

void CMyRegtEditValue::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyRegtEditValue)
	DDX_Text(pDX, IDC_EDIT_NAME, m_ItemName);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_ItemValue);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyRegtEditValue, CDialog)
	//{{AFX_MSG_MAP(CMyRegtEditValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyRegtEditValue message handlers

BOOL CMyRegtEditValue::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(m_Title);
	if(m_Title == _T("ÐÂ½¨¼üÖµ"))
	{
		CEdit *pEdit = (CEdit*) GetDlgItem(IDC_EDIT_NAME);
		pEdit->SetReadOnly(FALSE);
	}

	CEdit *pEdit = (CEdit*) GetDlgItem(IDC_EDIT_VALUE);	
	if(m_nType == REG_DWORD)
	{
		pEdit->ModifyStyle(0, ES_NUMBER);
	}
	return TRUE;  
}
