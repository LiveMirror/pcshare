// MyDirListView.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyFileListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyDirListView

IMPLEMENT_DYNCREATE(CMyDirListView, CXTListView)

CMyDirListView::CMyDirListView()
{
	m_IsDiskList = FALSE;
}

CMyDirListView::~CMyDirListView()
{
	POSITION pos = m_List.GetHeadPosition();
	LPMYICONINFO pItem = NULL;
	for (int i = 0; i < m_List.GetCount(); i++)
	{
		delete (LPMYICONINFO) m_List.GetNext(pos);
	}
	m_List.RemoveAll();
}


BEGIN_MESSAGE_MAP(CMyDirListView, CXTListView)
	//{{AFX_MSG_MAP(CMyDirListView)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_INSERTMYITEM, OnInsertMyItem)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDirListView drawing

void CMyDirListView::OnDraw(CDC* pDC)
{
//	CDocument* pDoc = GetDocument();

}

/////////////////////////////////////////////////////////////////////////////
// CMyDirListView diagnostics


/////////////////////////////////////////////////////////////////////////////
// CMyDirListView message handlers

void CMyDirListView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	GetParentFrame()->PostMessage(WM_DIRRIGHTRCLICK, 0, 0);
	*pResult = 0;
}

void CMyDirListView::FindMyImage(LPCTSTR ItemName,int &nImage)
{
	TCHAR m_Ext[5] = {0};
	CString m_ItemName = ItemName;
	lstrcpy(m_Ext, m_ItemName.Right(4));
	if(!lstrcmpi(m_Ext, _T(".exe")))
	{
		nImage = 6;
		return;
	}

	POSITION pos = m_List.GetHeadPosition();
	LPMYICONINFO pItem = NULL;
	for (int i = 0; i < m_List.GetCount(); i++)
	{
		LPMYICONINFO pItem = (LPMYICONINFO) m_List.GetNext(pos);
		if(!lstrcmp(pItem->m_Ext, m_Ext))
		{
			nImage = pItem->m_Index;
			return;
		}
	}

	SHFILEINFO m_Info = {0};
	DWORD ret = SHGetFileInfo(m_ItemName,FILE_ATTRIBUTE_NORMAL,
		&m_Info,sizeof(SHFILEINFO),SHGFI_SMALLICON | SHGFI_ICON |SHGFI_USEFILEATTRIBUTES);
	ASSERT(ret);
	nImage = m_imagelist.Add(m_Info.hIcon);
	m_imagelist.SetOverlayImage(nImage, 1);
	pItem = new MYICONINFO;
	lstrcpy(pItem->m_Ext,m_Ext);
	pItem->m_Index = nImage;
	m_List.AddTail(pItem);
}

LRESULT CMyDirListView::OnInsertMyItem(WPARAM wParam, LPARAM lParam)
{
	LPDIRFILELIST pItem = (LPDIRFILELIST) wParam;
	TCHAR *pItemName = StrRChr(pItem->m_ItemPath, NULL, _T('\\'));
	if(pItemName == NULL)
	{
		return FALSE;
	}
	*pItemName = 0x00;

	TCHAR m_TotalSize[256];
	LARGE_INTEGER m_FileSize;
	m_FileSize.LowPart = pItem->m_FileData.nFileSizeLow;
	m_FileSize.HighPart = pItem->m_FileData.nFileSizeHigh;
	StrFormatByteSize64(m_FileSize.QuadPart, m_TotalSize, 256);
	
	//类型
	int  m_Image = 0;
	TCHAR m_ItemType[256] = {0};
	if(pItem->m_FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		//目录
		m_Image = 5;
		lstrcpy(m_ItemType, _T("文件夹"));
	}
	else 
	{
		//文件
		FindMyImage(pItemName + 1, m_Image);
		lstrcpy(m_ItemType, _T("文件"));
	}

	//修改时间
	CString m_AccessTime;
	SYSTEMTIME m_SysTime = {0};
	FileTimeToSystemTime(&pItem->m_FileData.ftLastWriteTime, &m_SysTime);
	m_AccessTime.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), m_SysTime.wYear,
		m_SysTime.wMonth, m_SysTime.wDay, m_SysTime.wHour, m_SysTime.wMinute, m_SysTime.wSecond);

	//增加项
	int nItem = GetListCtrl().InsertItem(GetListCtrl().GetItemCount(), pItemName + 1, m_Image);
	if(nItem == -1)	
	{
		return FALSE;
	}
	GetListCtrl().SetItemText(nItem, 1, m_ItemType);
	GetListCtrl().SetItemText(nItem, 2, m_TotalSize);
	GetListCtrl().SetItemText(nItem, 3, m_AccessTime);
	GetListCtrl().SetItemText(nItem, 4, pItem->m_ItemPath);
	GetListCtrl().SetItemData(nItem, nItem);
	if(lParam != NULL) 
	{
		*((int*) lParam) = nItem;
	}
	return TRUE;
}

void CMyDirListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	if(GetListCtrl().GetSelectedCount() == 1)
	{
		GetParentFrame()->PostMessage(WM_COMMAND, ID_DIR_RIGHT_OPEN, 0);
	}
}

int CMyDirListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_imagelist.Create(16, 16,ILC_COLOR8|ILC_MASK,6,100);
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_DISK_SOFT));
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_DISK_CD));
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_DISK_HARD));
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_DISK_NET));
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_DISK_REMOTE));
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_DIR_NORMAL));
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_FILE_EXE));
	m_imagelist.Add(AfxGetApp()->LoadIcon(IDI_ICON_DISK_SOFT));
	GetListCtrl().SetImageList(&m_imagelist,LVSIL_SMALL);

	GetListCtrl().InsertColumn(0, _T("名称"), LVCFMT_LEFT, 140);
	GetListCtrl().InsertColumn(1, _T("类型"), LVCFMT_LEFT, 170);
	GetListCtrl().InsertColumn(2, _T("大小"), LVCFMT_RIGHT, 100);
	GetListCtrl().InsertColumn(3, _T("可用空间"), LVCFMT_RIGHT,  100);

	SubclassHeader();
	m_flatHeader.ThawAllColumns();
	m_flatHeader.ShowSortArrow(TRUE);
	EnableUserSortColor(true);
	return 0;
}

bool CMyDirListView::SortList(int, bool )
{
	CXTSortClass csc (&GetListCtrl(), m_nSortedCol);
	csc.Sort (m_bAscending, xtSortString);
	return true;
}

BOOL CMyDirListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style ^= LVS_REPORT;	
	return CXTListView::PreCreateWindow(cs);
}

void CMyDirListView::SetMyCoulmn(BOOL IsDisk)
{
	//删除当前记录	
	GetListCtrl().DeleteAllItems();

	while(GetListCtrl().GetHeaderCtrl()->GetItemCount() > 0)
	{
		GetListCtrl().DeleteColumn(0);
	}

	CRect rect;
	GetClientRect(&rect);

	if(IsDisk)
	{
		GetListCtrl().InsertColumn(0, _T("名称"), LVCFMT_LEFT, 140);
		GetListCtrl().InsertColumn(1, _T("类型"), LVCFMT_LEFT, 100);
		GetListCtrl().InsertColumn(2, _T("总大小"), LVCFMT_RIGHT, 100);
		GetListCtrl().InsertColumn(3, _T("可用空间"), LVCFMT_LEFT, rect.Width() - 340);
		m_IsDiskList = TRUE;
	}
	else
	{
		GetListCtrl().InsertColumn(0, _T("名称"), LVCFMT_LEFT, 150);
		GetListCtrl().InsertColumn(1, _T("类型"), LVCFMT_LEFT, 60);
		GetListCtrl().InsertColumn(2, _T("大小"), LVCFMT_RIGHT, 80);
		GetListCtrl().InsertColumn(3, _T("修改时间"), LVCFMT_RIGHT, 140);
		GetListCtrl().InsertColumn(4, _T("所在目录"), LVCFMT_LEFT, rect.Width() - 430 - 30);
		m_IsDiskList = FALSE;
	}
}
