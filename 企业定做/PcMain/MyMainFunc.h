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

	int  static GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	BOOL static WriteMyFile(LPCTSTR m_FileName, LPVOID pData, DWORD nLen);
	void static GetMyToken();
	void static GetMoudleMame(DWORD pId, TCHAR* pExeName);
	BOOL static MyGetKeyStr(HKEY hKey, LPCTSTR Title, LPCTSTR KeyName, BYTE* KeyValue);
	BOOL static GetFileSaveInfo(LPVOID pInfoData, DWORD nInfoLen);
	void static EncryptByte(LPVOID pData, DWORD nLen);
	BOOL static GetStrValue(LPCTSTR pStr, LPCTSTR pName, LPTSTR pValue);
	BOOL static GetUrlFile(char* pUrlPath, char* m_FilePath);
	BOOL static DeleteMyService(LPCTSTR ServiceName);
	BOOL static ReadMyFile(LPCTSTR m_FileName, BYTE** pFileData, DWORD* nFileLen);
	BOOL static CheckProcessIsExist(char* sProcessName);
};

#endif // !defined(AFX_MYMAINFUNC_H__D28C3CBF_CA2A_422F_9219_33B8C7E10917__INCLUDED_)
