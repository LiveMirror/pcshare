// MyDownTrans.h: interface for the CMyDownTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYDOWNTRANS_H__FCAD7EF7_05B6_4DAB_9AF7_6C5430AE3E1F__INCLUDED_)
#define AFX_MYDOWNTRANS_H__FCAD7EF7_05B6_4DAB_9AF7_6C5430AE3E1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyDownTrans : public CMyClientTran
{
public:
	CMyDownTrans();
	virtual ~CMyDownTrans();

	void StartWork(HWND hWnd);
};

#endif // !defined(AFX_MYDOWNTRANS_H__FCAD7EF7_05B6_4DAB_9AF7_6C5430AE3E1F__INCLUDED_)
