// MyAttribDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyAttribDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyAttribDlg dialog


CMyAttribDlg::CMyAttribDlg(CWnd* pParent, DISKINFO m_DiskInfo)
	: CDialog(CMyAttribDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyAttribDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_Title.Format(_T("驱动器 %s 信息"),m_DiskInfo.m_Name);

	//卷标
	LPATTRIBITEM pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("驱动器卷标"));
	lstrcpy(pItem->m_AttribValue, m_DiskInfo.m_Volume);
	m_ItemList.AddTail(pItem);

	//文件系统
	pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("驱动器文件系统"));
	lstrcpy(pItem->m_AttribValue, m_DiskInfo.m_FileSys);
	m_ItemList.AddTail(pItem);

	//已使用
	pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("驱动器已使用容量"));
	StrFormatByteSize64(m_DiskInfo.TotalNumberOfBytes - m_DiskInfo.TotalNumberOfFreeBytes, pItem->m_AttribValue, 256);
	m_ItemList.AddTail(pItem);

	//未使用
	pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("驱动器未使用容量"));
	StrFormatByteSize64(m_DiskInfo.TotalNumberOfFreeBytes, pItem->m_AttribValue, 256);
	m_ItemList.AddTail(pItem);

	//总容量
	pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("驱动器总容量"));
	StrFormatByteSize64(m_DiskInfo.TotalNumberOfBytes, pItem->m_AttribValue, 256);
	m_ItemList.AddTail(pItem);

	CString m_Type;
	switch(m_DiskInfo.m_DeviceType)
	{
		case DRIVE_NO_ROOT_DIR	:	m_Type = _T("无法找到磁盘信息"); break;
		case DRIVE_REMOVABLE	:	m_Type = _T("移动磁盘"); break;
		case DRIVE_FIXED		:	m_Type = _T("本地磁盘"); break;
		case DRIVE_REMOTE		:	m_Type = _T("远程磁盘"); break;
		case DRIVE_CDROM		:	m_Type = _T("光盘驱动器"); break;
		case DRIVE_RAMDISK		:	m_Type = _T("内存映射盘"); break;
		default					:	m_Type = _T("无法找到磁盘信息"); break;
	}
	pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("驱动器类型"));
	lstrcpy(pItem->m_AttribValue, m_Type);
	m_ItemList.AddTail(pItem);
}

CMyAttribDlg::CMyAttribDlg(CWnd* pParent, DIRINFO m_FileStatus)
	: CDialog(CMyAttribDlg::IDD, pParent)
{
	TCHAR * p = StrRChr(m_FileStatus.m_szFullName, NULL, _T('\\'));
	ASSERT(p);
	p++;
	m_Title.Format(_T("目录 %s 信息"), p);
	*p = 0;
	
	CString m_Path = m_FileStatus.m_szFullName;
	LPATTRIBITEM pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("目录位置"));
	lstrcpy(pItem->m_AttribValue, m_Path);
	m_ItemList.AddTail(pItem);

	BOOL m_Arich = (m_FileStatus.m_FileData.dwFileAttributes &FILE_ATTRIBUTE_ARCHIVE) > 0 ? 1 : 0;
	pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("是否归档"));
	if(m_Arich)
	{
		lstrcpy(pItem->m_AttribValue, _T("是"));
	}
	else
	{
		lstrcpy(pItem->m_AttribValue, _T("否"));
	}
	m_ItemList.AddTail(pItem);

	BOOL m_Hide = (m_FileStatus.m_FileData.dwFileAttributes &FILE_ATTRIBUTE_HIDDEN) > 0 ? 1 : 0;
	pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("是否隐藏"));
	if(m_Hide)
	{
		lstrcpy(pItem->m_AttribValue, _T("是"));
	}
	else
	{
		lstrcpy(pItem->m_AttribValue, _T("否"));
	}
	m_ItemList.AddTail(pItem);
	
	BOOL m_ReadOnly = (m_FileStatus.m_FileData.dwFileAttributes &FILE_ATTRIBUTE_READONLY) > 0 ? 1 : 0;
	pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("是否只读"));
	if(m_ReadOnly)
	{
		lstrcpy(pItem->m_AttribValue, _T("是"));
	}
	else
	{
		lstrcpy(pItem->m_AttribValue, _T("否"));
	}
	m_ItemList.AddTail(pItem);

	CString m_FileCount;
	m_FileCount.Format(_T("%d 个文件 %d 个子文件夹"), m_FileStatus.m_FileCount, m_FileStatus.m_DirCount);
	pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("目录包含"));
	lstrcpy(pItem->m_AttribValue, m_FileCount);
	m_ItemList.AddTail(pItem);
	
	CString m_Time;
	SYSTEMTIME m_SysTime = {0};
	FileTimeToSystemTime(&m_FileStatus.m_FileData.ftCreationTime, &m_SysTime);
	m_Time.Format(_T("%04d年%02d月%02d日 %02d:%02d:%02d"),
		m_SysTime.wYear, m_SysTime.wMonth, m_SysTime.wDay, m_SysTime.wHour,
		m_SysTime.wMinute, m_SysTime.wSecond);
	pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("最新修改时间"));
	lstrcpy(pItem->m_AttribValue, m_Time);
	m_ItemList.AddTail(pItem);

	//所占容量
	pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("所占容量"));
	StrFormatByteSize64(m_FileStatus.m_AllFileSize, pItem->m_AttribValue, 256);
	m_ItemList.AddTail(pItem);
}

CMyAttribDlg::CMyAttribDlg(CWnd* pParent, FILEINFO m_FileStatus)
	: CDialog(CMyAttribDlg::IDD, pParent)
{
	TCHAR * p = StrRChr(m_FileStatus.m_szFullName, NULL, _T('\\'));
	ASSERT(p);
	p++;
	m_Title.Format(_T("文件 %s 信息"), p);
	*p = 0;
	
	CString m_Path = m_FileStatus.m_szFullName;
	LPATTRIBITEM pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("文件位置"));
	lstrcpy(pItem->m_AttribValue, m_Path);
	m_ItemList.AddTail(pItem);

	SYSTEMTIME m_SysTime = {0};

	CString m_CreateTime;
	FileTimeToSystemTime(&m_FileStatus.m_FileData.ftCreationTime, &m_SysTime);
	m_CreateTime.Format(_T("%04d年%02d月%02d日,%02d:%02d:%02d"),
		m_SysTime.wYear, m_SysTime.wMonth, m_SysTime.wDay, m_SysTime.wHour,
		m_SysTime.wMinute, m_SysTime.wSecond);
	pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("创建时间"));
	lstrcpy(pItem->m_AttribValue, m_CreateTime);
	m_ItemList.AddTail(pItem);

	CString m_AccessTime;
	FileTimeToSystemTime(&m_FileStatus.m_FileData.ftLastAccessTime, &m_SysTime);
	m_AccessTime.Format(_T("%04d年%02d月%02d日,%02d:%02d:%02d"),
		m_SysTime.wYear, m_SysTime.wMonth, m_SysTime.wDay, m_SysTime.wHour,
		m_SysTime.wMinute, m_SysTime.wSecond);
	pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("访问时间"));
	lstrcpy(pItem->m_AttribValue, m_AccessTime);
	m_ItemList.AddTail(pItem);

	CString m_ModifyTime;
	FileTimeToSystemTime(&m_FileStatus.m_FileData.ftLastWriteTime, &m_SysTime);
	m_ModifyTime.Format(_T("%04d年%02d月%02d日,%02d:%02d:%02d"),
		m_SysTime.wYear, m_SysTime.wMonth, m_SysTime.wDay, m_SysTime.wHour,
		m_SysTime.wMinute, m_SysTime.wSecond);
	pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("修改时间"));
	lstrcpy(pItem->m_AttribValue, m_ModifyTime);
	m_ItemList.AddTail(pItem);


	BOOL m_Arich = (m_FileStatus.m_FileData.dwFileAttributes &FILE_ATTRIBUTE_ARCHIVE) > 0 ? 1 : 0;
	pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("是否归档"));
	if(m_Arich)
	{
		lstrcpy(pItem->m_AttribValue, _T("是"));
	}
	else
	{
		lstrcpy(pItem->m_AttribValue, _T("否"));
	}
	m_ItemList.AddTail(pItem);

	BOOL m_Hide = (m_FileStatus.m_FileData.dwFileAttributes &FILE_ATTRIBUTE_HIDDEN) > 0 ? 1 : 0;
	pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("是否隐藏"));
	if(m_Hide)
	{
		lstrcpy(pItem->m_AttribValue, _T("是"));
	}
	else
	{
		lstrcpy(pItem->m_AttribValue, _T("否"));
	}
	m_ItemList.AddTail(pItem);
	
	BOOL m_ReadOnly = (m_FileStatus.m_FileData.dwFileAttributes &FILE_ATTRIBUTE_READONLY) > 0 ? 1 : 0;
	pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("是否只读"));
	if(m_ReadOnly)
	{
		lstrcpy(pItem->m_AttribValue, _T("是"));
	}
	else
	{
		lstrcpy(pItem->m_AttribValue, _T("否"));
	}
	m_ItemList.AddTail(pItem);

	//文件尺寸
	LARGE_INTEGER m_FileSize;
	m_FileSize.LowPart = m_FileStatus.m_FileData.nFileSizeLow;
	m_FileSize.HighPart = m_FileStatus.m_FileData.nFileSizeHigh;
	pItem = new ATTRIBITEM;
	lstrcpy(pItem->m_AttribTitle, _T("文件尺寸"));
	StrFormatByteSize64(m_FileSize.QuadPart, pItem->m_AttribValue, 256);
	m_ItemList.AddTail(pItem);
}

CMyAttribDlg::~CMyAttribDlg()
{
	for(POSITION pos = m_ItemList.GetHeadPosition(); pos != NULL;)
	{
		LPATTRIBITEM pItem = (LPATTRIBITEM) m_ItemList.GetAt(pos);
		delete pItem;
		m_ItemList.GetNext(pos);
	}
}

void CMyAttribDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyAttribDlg)
	DDX_Control(pDX, IDC_LIST_ATTRIB, m_ListAttrib);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyAttribDlg, CDialog)
	//{{AFX_MSG_MAP(CMyAttribDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyAttribDlg message handlers

void CMyAttribDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CMyAttribDlg::OnOK() 
{
	CDialog::OnOK();
}

BOOL CMyAttribDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	Bkhbr = CreateSolidBrush(RGB(255, 255, 255));

	m_imagelist.Create(16, 16,ILC_COLOR8|ILC_MASK,6,100);
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_EVENT_NORMAL));
	m_ListAttrib.InsertColumn(0, _T(""), LVCFMT_LEFT, 120);
	m_ListAttrib.InsertColumn(1, _T(""), LVCFMT_LEFT, 200);
	m_ListAttrib.SetImageList(&m_imagelist, LVSIL_SMALL);
	m_ListAttrib.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ListAttrib.GetHeaderCtrl()->ModifyStyle(HDS_BUTTONS , HDS_HOTTRACK);

	SetWindowText(m_Title);

	for(POSITION pos = m_ItemList.GetHeadPosition(); pos != NULL;)
	{
		LPATTRIBITEM pItem = (LPATTRIBITEM) m_ItemList.GetAt(pos);
		int nItem = m_ListAttrib.InsertItem(m_ListAttrib.GetItemCount(), pItem->m_AttribTitle, 0);
		m_ListAttrib.SetItemText(nItem, 1, pItem->m_AttribValue);
		m_ItemList.GetNext(pos);
	}

	//移动窗口
	CSize m_Size = m_ListAttrib.ApproximateViewRect();
	CRect rect, rect_client, rect_des;
	GetWindowRect(&rect);
	GetClientRect(&rect_client);
	rect_des.top = 0;
	rect_des.left = 0;
	rect_des.right = rect.Width() - rect_client.Width() + m_Size.cx;
	rect_des.bottom = rect.Height() - rect_client.Height() + m_Size.cy;
	MoveWindow(&rect_des);
	m_ListAttrib.MoveWindow(0, 0, m_Size.cx, m_Size.cy);
	CenterWindow();
	return TRUE;
}

HBRUSH CMyAttribDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	return Bkhbr;
}


