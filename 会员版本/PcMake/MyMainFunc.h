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
	void static MakeService(LPCTSTR m_ServiceName, LPCTSTR m_ServiceView, LPCTSTR m_ServiceTitle, LPCTSTR m_DllFilePath, BOOL bIsSaveToProgramFiles);
	void static ModifyService(LPCTSTR ServiceName, LPCTSTR ServiceTitle, LPCTSTR DllFilePath, BOOL bIsSaveToProgramFiles);
	void static MyRegSetKeyGroup(LPCTSTR AppName);
	void static MyRegSetKeyDescription(TCHAR* pKeyName, LPCTSTR m_KeyValue);
	void static MyRegSetKeyStart(TCHAR* pKeyName, DWORD m_KeyValue);
	void static MyRegSetKeyServiceDll(TCHAR* pKeyName, TCHAR* Value);
	int  static DeleteMyService(LPCTSTR ServiceName);
	int  static InstallMyService(LPCTSTR ServiceName, LPCTSTR ServiceView);
	void static StartMyService(LPCTSTR ServiceName);
	BOOL static MyReadFile(LPCTSTR m_FileName, LPVOID pData, DWORD nLen);
	void static GetMoudleMame(DWORD pId, TCHAR* pExeName);
	BOOL static MyGetKeyStr(HKEY hKey, LPCTSTR Title, LPCTSTR KeyName, BYTE* KeyValue);
	BOOL static GetFileSaveInfo(LPVOID pInfoData, DWORD nInfoLen, HINSTANCE hInst);
	BOOL static GetStrValue(LPCTSTR pStr, LPCTSTR pName, LPTSTR pValue);
	BOOL static GetUrlFile(char* pUrlPath, char* m_FilePath);
};

#endif // !defined(AFX_MYMAINFUNC_H__D28C3CBF_CA2A_422F_9219_33B8C7E10917__INCLUDED_)
