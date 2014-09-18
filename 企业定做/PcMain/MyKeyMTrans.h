// MyKeyMonTrans.h: interface for the CMyKeyMTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYKEYMONTRANS_H__B56715F4_723A_460F_BD0A_6A3F4AF94D1E__INCLUDED_)
#define AFX_MYKEYMONTRANS_H__B56715F4_723A_460F_BD0A_6A3F4AF94D1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMyKeyMTrans : public CMyClientTran
{
public:
	CMyKeyMTrans();
	virtual ~CMyKeyMTrans();
	virtual BOOL StartWork(HWND hWnd);

	HANDLE hExitEvent;

	void CheckSocket();
	DWORD static CheckSocketThread(LPVOID);
};

#endif // !defined(AFX_MYKEYMONTRANS_H__B56715F4_723A_460F_BD0A_6A3F4AF94D1E__INCLUDED_)
