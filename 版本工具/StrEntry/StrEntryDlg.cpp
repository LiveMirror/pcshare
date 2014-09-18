// StrEntryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StrEntry.h"
#include "StrEntryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

/////////////////////////////////////////////////////////////////////////////
// CStrEntryDlg dialog

CStrEntryDlg::CStrEntryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStrEntryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStrEntryDlg)
	m_DesStr = _T("");
	m_SrcStr = _T("");
	m_ValueName = _T("m_TmpStr");
	m_IsExt = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStrEntryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStrEntryDlg)
	DDX_Text(pDX, IDC_EDIT_DES, m_DesStr);
	DDX_Text(pDX, IDC_EDIT_SRC, m_SrcStr);
	DDX_Text(pDX, IDC_EDIT_VALUENAME, m_ValueName);
	DDX_Check(pDX, IDC_CHECK_EXT, m_IsExt);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStrEntryDlg, CDialog)
	//{{AFX_MSG_MAP(CStrEntryDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStrEntryDlg message handlers

BOOL CStrEntryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	TRACE("%02x ", (BYTE) '\r');
	TRACE("%02x\n", (BYTE) '\n');

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CStrEntryDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CStrEntryDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CStrEntryDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CStrEntryDlg::OnOK() 
{
	UpdateData();

	char m_TmpStr[256] = {0};
	char m_sStrSrc[1024] = {0};
	lstrcpy(m_sStrSrc, m_SrcStr);
	m_DesStr = "	/*" + m_SrcStr + "*/\r\n";
//	m_DesStr += "	char " + m_ValueName + "[256] = {0};\r\n";
	DWORD len = strlen(m_sStrSrc);

	m_DesStr += "	";
	for(DWORD i = 0, k = 0; i < len; i++, k++)
	{
		if(m_sStrSrc[i] == '\\')
		{
			i++;
			if(m_sStrSrc[i] == 'r' || m_sStrSrc[i] == 'R')
			{
				sprintf(m_TmpStr, "%s[%d] = (char) 0x0d;", m_ValueName, k, (BYTE) m_sStrSrc[i]);
			}
			else if(m_sStrSrc[i] == 'n' || m_sStrSrc[i] == 'N')
			{
				sprintf(m_TmpStr, "%s[%d] = (char) 0x0a;", m_ValueName, k, (BYTE) m_sStrSrc[i]);
			}
			else
			{
				sprintf(m_TmpStr, "%s[%d] = (char) 0x%02x;", m_ValueName, k, (BYTE) m_sStrSrc[i]);
			}
		}
		else
		{
			sprintf(m_TmpStr, "%s[%d] = (char) 0x%02x;", m_ValueName, k, (BYTE) m_sStrSrc[i]);
		}
		m_DesStr += m_TmpStr;
	}

	sprintf(m_sStrSrc, "%s[%d] = 0x00;\r\n", m_ValueName, k);
	m_DesStr += m_sStrSrc;
	UpdateData(FALSE);
}

