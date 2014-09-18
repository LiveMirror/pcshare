// MyMultView.cpp : implementation file
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyMultView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Gdiplus;

/////////////////////////////////////////////////////////////////////////////
// CMyMultView

IMPLEMENT_DYNCREATE(CMyMultView, CScrollView)

CMyMultView::CMyMultView()
{
	hMainBitmap = NULL;
	pRecordData = NULL;
	memset(&m_gBitmapInfo, 0, sizeof(BITMAPINFO));
	memset(&m_AviInfo, 0, sizeof(m_AviInfo));
	pAviFile = NULL;
	pAviStream = NULL; 
	pCompressStream = NULL;
	nFrames = 0;
	m_IsRecord = FALSE;
	m_IsInput = FALSE;

	pStmBmp = NULL; 
	hMemBmp = NULL;
	
	nAviHz = 0;
	pImagePoint = NULL;
	m_FrameSize.cx = 0;
	m_FrameSize.cy = 0;

	hMetux = CreateMutex(NULL, FALSE, NULL);
	hWaitRecordEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	AVIFileInit(); 
}

CMyMultView::~CMyMultView()
{
	Stop();

	if(pStmBmp != NULL) 
	{
		pStmBmp->Release();
	}

	if(hMemBmp != NULL)
	{
		GlobalFree(hMemBmp);
	}

	if(hMainBitmap != NULL)
	{
		DeleteObject(hMainBitmap);
	}
	if(pImagePoint != NULL)
	{
		delete [] pImagePoint;
	}

	AVIFileExit();

	CloseHandle(hWaitRecordEvent);
	CloseHandle(hMetux);
}


BEGIN_MESSAGE_MAP(CMyMultView, CScrollView)
	//{{AFX_MSG_MAP(CMyMultView)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CONNBREAK, OnConnBreak)
END_MESSAGE_MAP()


LRESULT CMyMultView::OnConnBreak(WPARAM wParam, LPARAM lParam)
{
	
	if(lParam == 1)
	{
		MessageBox(_T("初始化视频失败!"), PS_WARNING,  MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(_T("连接已经中断!"), PS_WARNING,  MB_OK|MB_ICONINFORMATION);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMyMultView drawing

void CMyMultView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 0;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CMyMultView::OnDraw(CDC* pDC)
{

}

BOOL CMyMultView::PreCreateWindow(CREATESTRUCT& cs)
{
	//gdiplus
	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	CScrollView::PreCreateWindow(cs);
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return TRUE;	
}

/////////////////////////////////////////////////////////////////////////////
// CMyMultView diagnostics

#ifdef _DEBUG
void CMyMultView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMyMultView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyMultView message handlers

BOOL CMyMultView::InitRecord()
{
	OPENFILENAME OpenFileName = {0};
	TCHAR szFile[MAX_PATH] = {0};
	TCHAR szFileName[MAX_PATH] = {0};
  	TCHAR szFilter[1024] = {0};
	lstrcpy(szFilter, _T("视频文件(*.avi)"));
	lstrcpy(szFilter + lstrlen(szFilter) + 1, _T("*.avi"));
	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
    OpenFileName.hwndOwner         = GetSafeHwnd();
	OpenFileName.lpstrFilter       = szFilter;
    OpenFileName.lpstrCustomFilter = NULL;
    OpenFileName.nMaxCustFilter    = 0;
    OpenFileName.nFilterIndex      = 0;
    OpenFileName.lpstrFile         = szFile;
    OpenFileName.nMaxFile          = sizeof(szFile);
    OpenFileName.lpstrFileTitle    = szFileName;
    OpenFileName.nMaxFileTitle     = sizeof(szFileName);
    OpenFileName.lpstrInitialDir   = NULL;
    OpenFileName.lpstrTitle        = _T("保存当前视频录像");
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = _T("avi");
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(!GetSaveFileName(&OpenFileName))
	{
		return FALSE;
	}

	//检测上次文件是否关闭
	if(pAviStream != NULL)
	{
		AVIStreamClose(pAviStream);
		pAviStream = NULL;
	}
	if(pCompressStream != NULL)
	{
		AVIStreamClose(pCompressStream);
		pCompressStream = NULL;
	}
	if(pAviFile != NULL)
	{
		AVIFileRelease(pAviFile);
		pAviFile = NULL;
	}

	//打开avi文件
	if(AVIFileOpen(&pAviFile, szFile, OF_WRITE | OF_CREATE, NULL) != 0)
	{
		return FALSE;
	}

	//视频信息
	m_AviInfo.fccType = streamtypeVIDEO;
	m_AviInfo.fccHandler = 0;
	m_AviInfo.dwScale = 1;
	m_AviInfo.dwRate = nAviHz; 
	m_AviInfo.dwSuggestedBufferSize = m_gBitmapInfo.bmiHeader.biSizeImage;
	SetRect(&m_AviInfo.rcFrame, 0, 0, m_FrameSize.cx, m_FrameSize.cy);

	//创建文件流
	if(AVIFileCreateStream(pAviFile, &pAviStream, &m_AviInfo) != 0)
	{
		AVIFileRelease(pAviFile);
		pAviFile = NULL;
		return FALSE;
	}

	AVICOMPRESSOPTIONS stCPopt = {0};
	stCPopt.fccType = 0;
	stCPopt.fccHandler = mmioFOURCC('x', 'v', 'i', 'd');
	stCPopt.dwFlags = AVICOMPRESSF_KEYFRAMES|AVICOMPRESSF_VALID;
	stCPopt.dwKeyFrameEvery = 1;
	if(AVIMakeCompressedStream(&pCompressStream, pAviStream, &stCPopt, NULL) != 0)
	{
		AVIStreamClose(pAviStream);
		AVIFileRelease(pAviFile);
		pAviFile = NULL;
		pAviStream = NULL;
		return FALSE;
	}

	//设置流参数
	nFrames = 0;
	if(AVIStreamSetFormat(pCompressStream, nFrames, &m_gBitmapInfo.bmiHeader, sizeof(BITMAPINFOHEADER)) != 0)
	{
		AVIStreamClose(pCompressStream);
		AVIStreamClose(pAviStream);
		AVIFileRelease(pAviFile);
		pAviFile = NULL;
		pAviStream = NULL;
		pCompressStream = NULL;
		return FALSE;
	}

	//指针归位
	pRecordData = new BYTE[m_gBitmapInfo.bmiHeader.biSizeImage];
	m_IsRecord = TRUE;
	
	//创建录像线程
	hRecordThread = (HANDLE) _beginthread(DoRecordThread, NULL, this);
	return TRUE;
}

void CMyMultView::DoRecord()
{
	while(m_IsRecord && (WaitForSingleObject(hWaitRecordEvent, 24) == WAIT_TIMEOUT))
	{
		if(hMainBitmap != NULL)
		{
			WaitForSingleObject(hMetux, 0xffffffff);
			CDC* pDC = GetDC();
			GetDIBits(pDC->GetSafeHdc(), hMainBitmap , 0 , m_FrameSize.cy , pRecordData , &m_gBitmapInfo , DIB_RGB_COLORS);
			ReleaseDC(pDC);
			ReleaseMutex(hMetux);
			if(!AVIStreamWrite(pCompressStream, nFrames, 1, pRecordData, m_gBitmapInfo.bmiHeader.biSizeImage, AVIIF_KEYFRAME, NULL, NULL))
			{
				nFrames ++;
			}
		}
	}
}

void CMyMultView::DoRecordThread(LPVOID lPvoid)
{
	SetThreadAffinityMask(GetCurrentThread(), 0x0e);
	CMyMultView* pThis = (CMyMultView*) lPvoid;
	pThis->DoRecord();
}

BOOL CMyMultView::IsCanRecord()
{
	return (nAviHz != 0 && !m_IsRecord);
}

void CMyMultView::Start()
{
	if(!InitRecord())
	{
		MessageBox(_T("机器没有安装xvid编码器，请到主页下载！"), _T("错误"));
	}
}

void CMyMultView::Stop()
{
	if(!m_IsRecord)
	{
		return;
	}

	m_IsRecord = FALSE;
	SetEvent(hWaitRecordEvent);
	if(WaitForSingleObject(hRecordThread, 1000) != WAIT_OBJECT_0)
	{
		TerminateThread(hRecordThread, 0);
	}

	if(pAviStream != NULL)
	{
		AVIStreamClose(pAviStream);
		pAviStream = NULL;
	}
	if(pCompressStream != NULL)
	{
		AVIStreamClose(pCompressStream);
		pCompressStream = NULL;
	}
	if(pAviFile != NULL)
	{
		AVIFileRelease(pAviFile);
		pAviFile = NULL;
	}
	if(pRecordData != NULL)
	{
		delete [] pRecordData;
		pRecordData = NULL;
	}
}

void CMyMultView::OnPaint() 
{
	CPaintDC dc(this); 

	if(hMainBitmap == NULL)
	{
		return;
	}

	CRect rect;
	GetClientRect(&rect);

	CPoint m_Point = GetDeviceScrollPosition();
	int x = 0, y = 0, xpos = 0, ypos = 0;
	if(m_FrameSize.cx <= rect.Width())
	{
		x = (rect.Width() - m_FrameSize.cx) / 2;
	}
	else
	{
		xpos = m_Point.x;
	}
	if(m_FrameSize.cy <= rect.Height())
	{
		y = (rect.Height() - m_FrameSize.cy) / 2;
	}
	else
	{
		ypos = m_Point.y;
	}

	HDC hDesDC = ::GetDC(GetSafeHwnd());
	HDC hMemDC = CreateCompatibleDC(hDesDC);

	WaitForSingleObject(hMetux, 0xffffffff);
	HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, hMainBitmap);
	BitBlt(hDesDC, x, y, m_FrameSize.cx - m_Point.x, m_FrameSize.cy - m_Point.y, hMemDC, xpos, ypos, SRCCOPY);
	SelectObject(hMemDC, hOldBitmap);
	ReleaseMutex(hMetux);

	DeleteDC(hMemDC);
	DeleteDC(hDesDC);

}

BOOL CMyMultView::OnEraseBkgnd(CDC* pDC) 
{
	CRect rect;
	GetClientRect(&rect);
	if(hMainBitmap == NULL)
	{
		pDC->FillSolidRect(&rect, RGB(0, 0, 0));
		return TRUE;
	}

	CPoint m_Point = GetDeviceScrollPosition();
	int x = 0, y = 0, xpos = 0, ypos = 0;
	if(m_FrameSize.cx <= rect.Width())
	{
		x = (rect.Width() - m_FrameSize.cx) / 2;
	}
	else
	{
		xpos = m_Point.x;
	}
	if(m_FrameSize.cy <= rect.Height())
	{
		y = (rect.Height() - m_FrameSize.cy) / 2;
	}
	else
	{
		ypos = m_Point.y;
	}

	if(rect.Width() > m_FrameSize.cx || rect.Height() > m_FrameSize.cy)
	{
		HDC hDesDC = ::GetDC(GetSafeHwnd());
		HDC hMemDC = CreateCompatibleDC(hDesDC);
		HDC hMemDeskptopDC = CreateCompatibleDC(hDesDC);

		HBITMAP hDesktopBitmap = CreateCompatibleBitmap(hDesDC, rect.Width(), rect.Height());
		HBITMAP hOldDesktopBitmap = (HBITMAP) SelectObject(hMemDeskptopDC, hDesktopBitmap);

		WaitForSingleObject(hMetux, 0xffffffff);
		HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, hMainBitmap);
		BitBlt(hMemDeskptopDC, x, y, m_FrameSize.cx - m_Point.x, m_FrameSize.cy - m_Point.y, hMemDC, xpos, ypos, SRCCOPY);
		SelectObject(hMemDC, hOldBitmap);
		ReleaseMutex(hMetux);

		BitBlt(hDesDC, 0, 0, rect.Width(), rect.Height(), hMemDeskptopDC, 0, 0, SRCCOPY);

		DeleteDC(hMemDC);
		SelectObject(hMemDeskptopDC, hOldDesktopBitmap);
		DeleteObject(hDesktopBitmap);
		DeleteDC(hMemDeskptopDC);
		DeleteDC(hDesDC);
	}
	else
	{
		HDC hDesDC = ::GetDC(GetSafeHwnd());
		HDC hMemDC = CreateCompatibleDC(hDesDC);

		WaitForSingleObject(hMetux, 0xffffffff);
		HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, hMainBitmap);
		BitBlt(hDesDC, x, y, m_FrameSize.cx - m_Point.x, m_FrameSize.cy - m_Point.y, hMemDC, xpos, ypos, SRCCOPY);
		SelectObject(hMemDC, hOldBitmap);
		ReleaseMutex(hMetux);

		DeleteDC(hMemDC);
		DeleteDC(hDesDC);
	}
	return TRUE;
}

BOOL CMyMultView::IsRecord()
{
	return m_IsRecord;
}

BOOL CMyMultView::IsInit()
{
	return (hMainBitmap != NULL);
}

CString CMyMultView::GetXy()
{
	CString m_Str;
	m_Str.Format(_T("当前视频尺寸%d×%d"), m_FrameSize.cx, m_FrameSize.cy);
	return m_Str;
}

void CMyMultView::Save()
{
	OPENFILENAME OpenFileName = {0};
	TCHAR szFile[MAX_PATH] = {0};
	TCHAR szFileName[MAX_PATH] = {0};
  	TCHAR szFilter[1024] = {0};
	lstrcpy(szFilter, _T("图象文件(*.jpg)"));
	lstrcpy(szFilter + lstrlen(szFilter) + 1, _T("*.jpg"));
	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
    OpenFileName.hwndOwner         = GetSafeHwnd();
	OpenFileName.lpstrFilter       = szFilter;
    OpenFileName.lpstrCustomFilter = NULL;
    OpenFileName.nMaxCustFilter    = 0;
    OpenFileName.nFilterIndex      = 0;
    OpenFileName.lpstrFile         = szFile;
    OpenFileName.nMaxFile          = sizeof(szFile);
    OpenFileName.lpstrFileTitle    = szFileName;
    OpenFileName.nMaxFileTitle     = sizeof(szFileName);
    OpenFileName.lpstrInitialDir   = NULL;
    OpenFileName.lpstrTitle        = _T("保存当前视频图象");
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = _T("jpg");
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(!GetSaveFileName(&OpenFileName))
	{
		return;
	}

	EncoderParameters	eParameters;
	CLSID				eClsid;
	DWORD nQuality = 100;
	GetEncoderClsid(L"image/jpeg", &eClsid);
	eParameters.Count = 1;
	eParameters.Parameter[0].Guid = EncoderQuality;
	eParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
	eParameters.Parameter[0].NumberOfValues = 1;
	eParameters.Parameter[0].Value = &nQuality;

	Bitmap* pTempBitmap = Bitmap::FromHBITMAP(hMainBitmap, NULL);
	pTempBitmap->Save(szFile, &eClsid, &eParameters);
	delete pTempBitmap;
}

void CMyMultView::TransVideoThread(LPVOID lPvoid)
{
	SetThreadAffinityMask(GetCurrentThread(), 0x0E);
	CMyMultView* pThis = (CMyMultView*) lPvoid;
	pThis->TransVideo();
}

void CMyMultView::TransVideo()
{
	//视频尺寸
	TCHAR m_sSize[100] = {0};
	GetPrivateProfileString(PS_SHEZHI, _T("视频监控尺寸"), _T("320 x 240"), m_sSize, 99, GetIniFileName());
	CString m_Size = m_sSize;
	m_FrameSize.cx = StrToInt(m_Size.Left(3));
	m_FrameSize.cy = StrToInt(m_Size.Right(3));

	m_gBitmapInfo.bmiHeader.biBitCount = 32;
	m_gBitmapInfo.bmiHeader.biWidth = m_FrameSize.cx;
	m_gBitmapInfo.bmiHeader.biHeight = m_FrameSize.cy;
	m_gBitmapInfo.bmiHeader.biCompression = BI_RGB ;
	m_gBitmapInfo.bmiHeader.biSizeImage = m_FrameSize.cx * m_FrameSize.cy * 4;
	m_gBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_gBitmapInfo.bmiHeader.biPlanes = 1;

	if(!m_Tran.TcpSendData(&m_FrameSize, sizeof(SIZE)))
	{
		PostMessage(WM_CONNBREAK, 0, 1);
		return;
	}

	nBitLen = ((m_FrameSize.cx * 24 + 31) / 32) * 4 * m_FrameSize.cy;
	hMemBmp = GlobalAlloc(GMEM_FIXED, nBitLen);
	CreateStreamOnHGlobal(hMemBmp, FALSE, &pStmBmp);
	len.LowPart = nBitLen;
	pStmBmp->SetSize(len);

	//设置滚动条
	SetScrollSizes(MM_TEXT, m_FrameSize);
	ScrollToPosition(CPoint(0, 0));
	
	BYTE* pImageData = (BYTE*) GlobalLock(hMemBmp);
	DWORD nFrameLen = 0;

	//接收第一帧
	if(!m_Tran.TcpRecvData(&nFrameLen, sizeof(DWORD)) || !m_Tran.TcpRecvData(pImageData, nFrameLen))
	{
		PostMessage(WM_CONNBREAK, 0, 1);
		return;
	}
	Bitmap* pMainBitmap = Bitmap::FromStream(pStmBmp, FALSE);
	Color l = 0;
	pMainBitmap->GetHBITMAP(l, &hMainBitmap);
	delete pMainBitmap;
	InvalidateRect(NULL);

	//接收数据
	int iFrameCount = 0;
	DWORD nTimeCount = GetTickCount();
	while(1)
	{
		if(!m_Tran.TcpRecvData(&nFrameLen, sizeof(DWORD)) || !m_Tran.TcpRecvData(pImageData, nFrameLen))
		{
			break;
		}

		//拷贝图片
		Bitmap*	pTmpBitmap = Bitmap::FromStream(pStmBmp, FALSE);
		Color l = 0;
		pTmpBitmap->GetHBITMAP(l, &hMainBitmap);
		delete pTmpBitmap;

		PostMessage(WM_PAINT, 0, 0);

		//画图
		if(nAviHz == 0)
		{
			if(iFrameCount++ == 10)
			{
				nAviHz = (GetTickCount() - nTimeCount) / 10;
				if(nAviHz >= 1000)
				{
					nAviHz = 1;
				}
				else
				{
					nAviHz = 1000 / nAviHz;
				}

				TRACE(_T("nAviHz = %d\n"), nAviHz);
			}
		}

		/*
		CRect rect;
		GetClientRect(&rect);
		CPoint m_Point = GetDeviceScrollPosition();
		int x = 0, y = 0, xpos = 0, ypos = 0;
		if(m_FrameSize.cx <= rect.Width())
		{
			x = (rect.Width() - m_FrameSize.cx) / 2;
		}
		else
		{
			xpos = m_Point.x;
		}
		if(m_FrameSize.cy <= rect.Height())
		{
			y = (rect.Height() - m_FrameSize.cy) / 2;
		}
		else
		{
			ypos = m_Point.y;
		}

		HDC hDesDC = ::GetDC(GetSafeHwnd());
		HDC hMemDC = CreateCompatibleDC(hDesDC);

		
		WaitForSingleObject(hMetux, 0xffffffff);
		HBITMAP hOldBitmap = (HBITMAP) SelectObject(hMemDC, hMainBitmap);
		BitBlt(hDesDC, x, y, m_FrameSize.cx - m_Point.x, m_FrameSize.cy - m_Point.y, hMemDC, xpos, ypos, SRCCOPY);
		SelectObject(hMemDC, hOldBitmap);
		ReleaseMutex(hMetux);
	
		DeleteDC(hMemDC);
		DeleteDC(hDesDC);
		*/
	}
}

void CMyMultView::TransFrameThread(LPVOID lPvoid)
{
	SetThreadAffinityMask(GetCurrentThread(), 0x0e);
	CMyMultView* pThis = (CMyMultView*) lPvoid;
	pThis->TransFrame();
}

void CMyMultView::TransFrame()
{
	MYFRAMEINFO m_Info = {0};
	if(!m_Tran.TcpRecvData(&m_Info, sizeof(m_Info)))
	{
		PostMessage(WM_CONNBREAK, 0, 0);
		return;
	}

	//肉机端屏幕大小
	m_FrameSize.cx = m_Info.m_Width;
	m_FrameSize.cy = m_Info.m_Height;

	//填充全局bitmapinfo
	m_gBitmapInfo.bmiHeader.biBitCount = 32;
	m_gBitmapInfo.bmiHeader.biWidth = m_FrameSize.cx;
	m_gBitmapInfo.bmiHeader.biHeight = m_FrameSize.cy;
	m_gBitmapInfo.bmiHeader.biCompression = BI_RGB ;
	m_gBitmapInfo.bmiHeader.biSizeImage = m_FrameSize.cx * m_FrameSize.cy * 4;
	m_gBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_gBitmapInfo.bmiHeader.biPlanes = 1;

	//设置滚动条
	SetScrollSizes(MM_TEXT, m_FrameSize);
	ScrollToPosition(CPoint(0, 0));

	//开辟内存缓冲
	hMemBmp = GlobalAlloc(GMEM_FIXED, m_gBitmapInfo.bmiHeader.biSizeImage + 102400);
	if(hMemBmp == NULL) 
	{
		PostMessage(WM_CONNBREAK, 0, 0);
		return;
	}
	CreateStreamOnHGlobal(hMemBmp, FALSE, &pStmBmp);
	if(pStmBmp == NULL) 
	{
		GlobalFree(hMemBmp);
		hMemBmp = NULL;
		PostMessage(WM_CONNBREAK, 0, 0);
		return;
	}
	len.LowPart = m_gBitmapInfo.bmiHeader.biSizeImage + 102400;
	pStmBmp->SetSize(len);

	//接收第一帧图象
	DWORD nFrameSize = 0;
	BYTE* pZipData = (BYTE*) GlobalLock(hMemBmp);
	if(!m_Tran.TcpRecvData(&nFrameSize, sizeof(DWORD)) || !m_Tran.TcpRecvData(pZipData, nFrameSize))
	{
		PostMessage(WM_CONNBREAK, 0, 0);
		return;
	}

	//复原第一帧图象，显示
	Bitmap* pMainBitmap = Bitmap::FromStream(pStmBmp, TRUE);
	Color l = 0;
	pMainBitmap->GetHBITMAP(l, &hMainBitmap);
	delete pMainBitmap;
	InvalidateRect(NULL);

	int	xImageCount = 0;
	int	yImageCount = 0;
	if(m_FrameSize.cx % m_Info.m_RectSize == 0)
	{
		xImageCount = m_FrameSize.cx / m_Info.m_RectSize;
	}
	else
	{
		xImageCount = m_FrameSize.cx / m_Info.m_RectSize + 1;
	}

	if(m_FrameSize.cy % m_Info.m_RectSize == 0)
	{
		yImageCount = m_FrameSize.cy / m_Info.m_RectSize;
	}
	else
	{
		yImageCount = m_FrameSize.cy / m_Info.m_RectSize + 1;
	}

	pImagePoint = new PSPOINT[xImageCount * yImageCount];
	DWORD nImageCount = 0;

	int iFrameCount = 0;
	DWORD nTimeCount = GetTickCount();

	HDC		hWndDC = NULL;
	HDC		hMainDC = NULL;
	HDC		hCurDC = NULL;
	HBITMAP hCurBitmap = NULL;

	//开始接收屏幕数据
	while(1)
	{
		if(!m_Tran.TcpRecvData(&nImageCount, sizeof(DWORD)) || !m_Tran.TcpRecvData(pImagePoint, nImageCount * sizeof(PSPOINT)) ||
			!m_Tran.TcpRecvData(&nFrameSize, sizeof(DWORD)) || !m_Tran.TcpRecvData(pZipData, nFrameSize))
		{
			break;
		}

		//取当前改变的区域
		Bitmap* pCurBitmap = Bitmap::FromStream(pStmBmp);
		pCurBitmap->GetHBITMAP(l, &hCurBitmap);
		delete pCurBitmap;

		hWndDC = ::GetDC(GetSafeHwnd());
		hMainDC = ::CreateCompatibleDC(hWndDC);
		hCurDC = ::CreateCompatibleDC(hWndDC);
		::DeleteDC(hWndDC);

		::SelectObject(hCurDC, hCurBitmap);
		
		WaitForSingleObject(hMetux, 0xffffffff);
		::SelectObject(hMainDC, hMainBitmap);
		for(DWORD i = 0; i < nImageCount; i++)
		{
			::BitBlt(hMainDC, pImagePoint[i].x, pImagePoint[i].y, m_Info.m_RectSize, m_Info.m_RectSize, hCurDC, i * m_Info.m_RectSize, 0, SRCCOPY);
		}
		::SelectObject(hMainDC, NULL);
		ReleaseMutex(hMetux);
				
		::SelectObject(hCurDC, NULL);
		::DeleteObject(hCurBitmap);

		::DeleteDC(hMainDC);
		::DeleteDC(hCurDC);

		//通知view刷新
		PostMessage(WM_PAINT, 0, 0);
		
		if(nAviHz == 0)
		{
			if(iFrameCount++ == 10)
			{
				nAviHz = (GetTickCount() - nTimeCount) / 10;
				if(nAviHz >= 1000)
				{
					nAviHz = 1;
				}
				else
				{
					nAviHz = 1000 / nAviHz;
				}

				TRACE(_T("nAviHz = %d\n"), nAviHz);
			}
		}
	}
	PostMessage(WM_CONNBREAK, 0, 0);
}

void CMyMultView::StartWork(TCPCONNECTINFO m_SocketInfo, BOOL IsFrame)
{
	if(m_Tran.DuplicateHttpSocket(&m_SocketInfo))
	{
		if(IsFrame)
		{
			hRecvThread = (HANDLE) _beginthread(TransFrameThread, 0, this);
		}
		else
		{
			hRecvThread = (HANDLE) _beginthread(TransVideoThread, 0, this);
		}
	}
}

void CMyMultView::OnDestroy() 
{
	m_Tran.Close();
	if(WaitForSingleObject(hRecvThread, 1000) != WAIT_OBJECT_0)
	{
		TerminateThread(hRecvThread, 0);
	}
	CScrollView::OnDestroy();
}

void CMyMultView::SetInputEnable(BOOL bIsEnable)
{
	m_IsInput = bIsEnable;
}

void CMyMultView::SendCtrlInfo()
{
	MOUSEINFO m_MouseInfo = {0};
	m_MouseInfo.cmd = CMD_FRAME_KEY_ALL;
	ExecCmd(&m_MouseInfo);
}

BOOL CMyMultView::ExecCmd(LPMOUSEINFO Info)
{
	if(m_IsInput)
	{
		if(!m_Tran.TcpSendDataMouse(Info))
		{
			return FALSE;
		}
	}
	return TRUE;
}

void CMyMultView::GetMouseXy(MOUSEINFO& m_MouseInfo, CPoint point)
{
	CPoint m_Point = GetDeviceScrollPosition();
	CRect rcClient;
	GetClientRect(&rcClient);
	if(rcClient.Width() > m_FrameSize.cx)
	{
		m_MouseInfo.x = point.x - (rcClient.Width() - m_FrameSize.cx)/2;
	}
	else
	{
		m_MouseInfo.x = (WORD) (point.x + m_Point.x);
	}
	if(rcClient.Height() > m_FrameSize.cy)
	{
		m_MouseInfo.y = point.y - (rcClient.Height() - m_FrameSize.cy)/2;
	}
	else
	{
		m_MouseInfo.y = (WORD) (point.y + m_Point.y);
	}
}

void CMyMultView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonDown(nFlags, point);
	MOUSEINFO m_MouseInfo = {0};
	m_MouseInfo.cmd = CMD_FRAME_MOUSE_STATE;
	m_MouseInfo.state = MOUSEEVENTF_LEFTDOWN;
	GetMouseXy(m_MouseInfo, point);
	ExecCmd(&m_MouseInfo);
}

void CMyMultView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonUp(nFlags, point);

	MOUSEINFO m_MouseInfo = {0};
	m_MouseInfo.cmd = CMD_FRAME_MOUSE_STATE;
	GetMouseXy(m_MouseInfo, point);
	m_MouseInfo.state = MOUSEEVENTF_LEFTUP;
	ExecCmd(&m_MouseInfo);
}

void CMyMultView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CScrollView::OnRButtonDown(nFlags, point);

	MOUSEINFO m_MouseInfo = {0};
	m_MouseInfo.cmd = CMD_FRAME_MOUSE_STATE;
	GetMouseXy(m_MouseInfo, point);
	m_MouseInfo.state = MOUSEEVENTF_RIGHTDOWN;
	ExecCmd(&m_MouseInfo);
}

void CMyMultView::OnRButtonUp(UINT nFlags, CPoint point)
{
	CScrollView::OnRButtonUp(nFlags, point);

	MOUSEINFO m_MouseInfo = {0};
	m_MouseInfo.cmd = CMD_FRAME_MOUSE_STATE;
	GetMouseXy(m_MouseInfo, point);
	m_MouseInfo.state = MOUSEEVENTF_RIGHTUP;
	ExecCmd(&m_MouseInfo);
}

void CMyMultView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonDblClk(nFlags, point);

	MOUSEINFO m_MouseInfo = {0};
	m_MouseInfo.cmd = CMD_FRAME_MOUSE_STATE;
	GetMouseXy(m_MouseInfo, point);

	m_MouseInfo.state = MOUSEEVENTF_LEFTDOWN;
	ExecCmd(&m_MouseInfo);

	m_MouseInfo.state = MOUSEEVENTF_LEFTUP;
	ExecCmd(&m_MouseInfo);

	m_MouseInfo.state = MOUSEEVENTF_LEFTDOWN;
	ExecCmd(&m_MouseInfo);

	m_MouseInfo.state = MOUSEEVENTF_LEFTUP;
	ExecCmd(&m_MouseInfo);
}

void CMyMultView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	CScrollView::OnRButtonDblClk(nFlags, point);

	MOUSEINFO m_MouseInfo = {0};
	m_MouseInfo.cmd = CMD_FRAME_MOUSE_STATE;
	GetMouseXy(m_MouseInfo, point);
	m_MouseInfo.state = MOUSEEVENTF_RIGHTDOWN;
	ExecCmd(&m_MouseInfo);
	
	m_MouseInfo.state = MOUSEEVENTF_RIGHTUP;
	ExecCmd(&m_MouseInfo);

	m_MouseInfo.state = MOUSEEVENTF_RIGHTDOWN;
	ExecCmd(&m_MouseInfo);

	m_MouseInfo.state = MOUSEEVENTF_RIGHTUP;
	ExecCmd(&m_MouseInfo);
}

void CMyMultView::OnMouseMove(UINT nFlags, CPoint point)
{
	CScrollView::OnMouseMove(nFlags, point);
	MOUSEINFO m_MouseInfo = {0};
	m_MouseInfo.cmd = CMD_FRAME_MOUSE_STATE;
	GetMouseXy(m_MouseInfo, point);
	m_MouseInfo.state = 0;
	ExecCmd(&m_MouseInfo);
}

void CMyMultView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);

	MOUSEINFO m_MouseInfo = {0};
	m_MouseInfo.cmd = CMD_FRAME_KEY_STATE;
	SHORT ch = 0;
	ch = GetKeyState(VK_SHIFT);
	m_MouseInfo.x = HIBYTE(ch);
	ch = GetKeyState(VK_CONTROL);
	m_MouseInfo.y = HIBYTE(ch);
	ch = GetKeyState(VK_MENU );
	m_MouseInfo.state = HIBYTE(ch);
	m_MouseInfo.ch = nChar;
	for(UINT i = 0; i < nRepCnt; i++)
	{
		ExecCmd(&m_MouseInfo);
	}
}

void CMyMultView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CScrollView::OnSysKeyDown(nChar, nRepCnt, nFlags);

	MOUSEINFO m_MouseInfo = {0};
	m_MouseInfo.cmd = CMD_FRAME_KEY_STATE;
	SHORT ch = 0;
	ch = GetKeyState(VK_SHIFT);
	m_MouseInfo.x = HIBYTE(ch);
	ch = GetKeyState(VK_CONTROL);
	m_MouseInfo.y = HIBYTE(ch);
	ch = GetKeyState(VK_MENU );
	m_MouseInfo.state = HIBYTE(ch);
	m_MouseInfo.ch = nChar;
	for(UINT i = 0; i < nRepCnt; i++)
	{
		ExecCmd(&m_MouseInfo);
	}
}

BOOL CMyMultView::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	LPTCPCONNECTINFO pInfo = (LPTCPCONNECTINFO) pCopyDataStruct->lpData;
	if(pCopyDataStruct->dwData == PS_SOCKET_CONNECT)
	{
		if(m_Tran.DuplicateHttpSocket(pInfo))
		{
			hRecvThread = (HANDLE) _beginthread(TransVideoThread, 0, this);
		}
	}	
	return TRUE;
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if(wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }
   free(pImageCodecInfo);
   return -1;  // Failure
}
