// MyUpTrans.h: interface for the CMyUpTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYUPTRANS_H__A10BCD60_8DD5_43E4_BB7E_4915214F57B6__INCLUDED_)
#define AFX_MYUPTRANS_H__A10BCD60_8DD5_43E4_BB7E_4915214F57B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyUpTrans : public CMyClientTran
{
public:
	CMyUpTrans();
	virtual ~CMyUpTrans();

	void StartWork(HWND hWnd);
};

#endif // !defined(AFX_MYUPTRANS_H__A10BCD60_8DD5_43E4_BB7E_4915214F57B6__INCLUDED_)
