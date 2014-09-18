// MyVideoTrans.h: interface for the CMyVideoTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYVIDEOTRANS_H__BD4BC71F_AA58_4868_A264_E875EC21F5F2__INCLUDED_)
#define AFX_MYVIDEOTRANS_H__BD4BC71F_AA58_4868_A264_E875EC21F5F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyClientTran.h"

using namespace Gdiplus;

// Structures
typedef struct _callbackinfo 
{
    double dblSampleTime;
    long lBufferSize;
    BYTE *pBuffer;
    BITMAPINFOHEADER bih;
}CALLBACKINFO;

BOOL GetDefaultCapDevice(IBaseFilter ** ppCap);

//class CSampleGrabberCB;

class CSampleGrabberCB : public ISampleGrabberCB 
{
public:
    // these will get set by the main thread below. We need to
    // know this in order to write out the bmp
    long lWidth;
    long lHeight;
 
	CALLBACKINFO cb;

	HANDLE hMultSendDataEvent;
	HANDLE hMultSendDataMetux;
    CSampleGrabberCB( )
    {
		hMultSendDataEvent = m_gFunc.CreateEvent(NULL, TRUE, FALSE, NULL);
		hMultSendDataMetux = m_gFunc.CreateMutex(NULL, FALSE, NULL);
		m_gFunc.memset(&cb, 0, sizeof(CALLBACKINFO));
    }  
	
	~CSampleGrabberCB( )
    {
		m_gFunc.CloseHandle(hMultSendDataEvent);
		m_gFunc.CloseHandle(hMultSendDataMetux);

		if (cb.pBuffer != 0)
		{
			delete cb.pBuffer;
			cb.pBuffer = 0;
		}

    }   

    // fake out any COM ref counting
    //
    STDMETHODIMP_(ULONG) AddRef() { return 2; }
    STDMETHODIMP_(ULONG) Release() { return 1; }

    // fake out any COM QI'ing
    //
    STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)
    {
        if( riid == IID_ISampleGrabberCB || riid == IID_IUnknown ) 
        {
            *ppv = (void *) static_cast<ISampleGrabberCB*> ( this );
            return NOERROR;
        }    
        return E_NOINTERFACE;
    }

    // we don't implement this interface for this example
    //
    STDMETHODIMP SampleCB( double SampleTime, IMediaSample * pSample )
    {
        return 0;
    }
    
    STDMETHODIMP BufferCB( double dblSampleTime, BYTE * pBuffer, long lBufferSize )
    {
		if( cb.lBufferSize < lBufferSize )
        {
            delete [] cb.pBuffer;
            cb.pBuffer = NULL;
            cb.lBufferSize = 0;
        }

        // Since we can't access Windows API functions in this callback, just
        // copy the bitmap data to a global structure for later reference.
        cb.dblSampleTime = dblSampleTime;

        // If we haven't yet allocated the data buffer, do it now.
        // Just allocate what we need to store the new bitmap.
        if (!cb.pBuffer)
        {
            cb.pBuffer = new BYTE[lBufferSize];
            cb.lBufferSize = lBufferSize;
        }

        if(!cb.pBuffer )
        {
            cb.lBufferSize = 0;
            return E_OUTOFMEMORY;
        }

		m_gFunc.WaitForSingleObject(hMultSendDataMetux, 0xffffffff);
		m_gFunc.memcpy(cb.pBuffer, pBuffer, lBufferSize);
		m_gFunc.ReleaseMutex(hMultSendDataMetux);

		//通知发送线程取数据
		m_gFunc.SetEvent(hMultSendDataEvent);
        return 0;
    }
};

class CMyVideoTrans : public CMyClientTran  
{
public:
	CMyVideoTrans();
	virtual ~CMyVideoTrans();

	SIZE			m_FrameSize;
	BITMAPINFO		m_BmpInfo;
	BYTE*			pZipData;
	DWORD			nFrames;

	IStream*		pStmBmp;
	HGLOBAL			hMemBmp;
	
	CLSID			eClsid;
	DWORD			nQuality;
	ULARGE_INTEGER	len;
	LARGE_INTEGER	pos;
	EncoderParameters	eParameters;


	CSampleGrabberCB mCB;

	CComPtr< IGraphBuilder > m_pGraph;
	CComPtr< ISampleGrabber > m_pGrabber;
	CComPtr< IBaseFilter > pCap;
	CComPtr<ICaptureGraphBuilder2> pCGB2;
	CComPtr<IPin> pVPPin;
	CComPtr<IBaseFilter> pRenderer;
	CComQIPtr< IVideoWindow, &IID_IVideoWindow > pWindow;
	CComQIPtr< IMediaControl, &IID_IMediaControl > pControl;
	CComQIPtr< IBaseFilter, &IID_IBaseFilter > pGrabBase;

	HRESULT InitStillGraph(int x, int y);
	BOOL SetFormat(ICaptureGraphBuilder2* pBuilder, IBaseFilter* pCap, long lWidth, long lHeight, unsigned short iColorBit, __int64 iRate);
	
	void StartWork(HWND hWnd);
};

#endif // !defined(AFX_MYVIDEOTRANS_H__BD4BC71F_AA58_4868_A264_E875EC21F5F2__INCLUDED_)
