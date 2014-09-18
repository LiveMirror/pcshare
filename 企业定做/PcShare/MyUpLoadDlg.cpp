// MyUpLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyUpLoadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyUpLoadDlg dialog


CMyUpLoadDlg::CMyUpLoadDlg(CWnd* pParent)
	: CDialog(CMyUpLoadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyUpLoadDlg)
	m_Value = _T("");
	m_Show = 0;
	m_Param = _T("");
	//}}AFX_DATA_INIT
}


void CMyUpLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyUpLoadDlg)
	DDX_Text(pDX, IDC_EDIT_VALUE, m_Value);
	DDX_Radio(pDX, IDC_RADIO_SHOW, m_Show);
	DDX_Text(pDX, IDC_EDIT_PARAM, m_Param);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyUpLoadDlg, CDialog)
	//{{AFX_MSG_MAP(CMyUpLoadDlg)
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnButtonFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyUpLoadDlg message handlers


void CMyUpLoadDlg::OnOK() 
{
	CDialog::OnOK();
}

void CMyUpLoadDlg::OnButtonFile() 
{
	OPENFILENAME OpenFileName = {0};
	TCHAR szFile[MAX_PATH] = {0};
	TCHAR szFileName[MAX_PATH] = {0};
  	TCHAR szFilter[1024] = {0};
	lstrcpy(szFilter, _T("(*.*)"));
	lstrcpy(szFilter + lstrlen(szFilter) + 1, _T("*.*"));
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
    OpenFileName.lpstrTitle        = _T("选择上传到客户机器的文件");
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = _T("*");
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(!GetOpenFileName(&OpenFileName))
	{
		return;
	}
	m_Value = szFile;
	UpdateData(FALSE);	
}

void CMyUpLoadDlg::OnCancel() 
{
	CDialog::OnCancel();
}
